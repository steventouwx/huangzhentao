/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include "RfxHandlerManager.h"
#include <utility>
#include "RfxChannelManager.h"
#include "RfxMisc.h"
#include "RfxOpUtils.h"
#include "RfxVoidData.h"
#include "RfxDispatchThread.h"
#include <libmtkrilutils.h>
#include <RfxRilDumpInfo.h>
#include "RfxRilUtils.h"
#include "RfxMclDispatcherThread.h"

#define RFX_LOG_TAG "RfxHandlerMgr"

RfxHandlerManager* RfxHandlerManager::s_self = NULL;

Mutex RfxHandlerManager::sWaitChannelDoneMutex;
int RfxHandlerManager::sCreateChannelNumber = 0;

RfxHandlerManager* RfxHandlerManager::init() {
    if (s_self == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "init");
        s_self = new RfxHandlerManager();
    }
    return s_self;
}

void RfxHandlerManager::registerHandler(RfxCreateHandlerFuncptr func_ptr, unsigned int c_id,
        int slot_category, bool isOpReplaced, bool isOpHandler) {

    if (isOpReplaced && RfxOpUtils::getOpHandler() != NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "skip creating om handler, it will be repaced by op handler");
        return;
    } else if (RfxOpUtils::getOpHandler() == NULL && isOpHandler) {
        RFX_LOG_D(RFX_LOG_TAG, "skip creating op handler under om package");
        return;
    }

    // Because using dynamic  initialization to register request/urc/event, we need to ensure that RfxHandlerManager
    // has created first
    init();
    RFX_LOG_D(RFX_LOG_TAG, "registerHandler slot_category = %d, c_id = %d, func_ptr = %p",
            slot_category, c_id, func_ptr);
    if (SLOT == slot_category) {
        s_self->registerInternal(s_self->m_slot_handler_list[c_id], func_ptr, c_id);
    }
    // else {
    //    s_self->registerInternal(s_self->m_non_slot_handler_list[c_id], func_ptr, c_id);
    // }
 }

void RfxHandlerManager::registerInternal(Vector<RfxCreateHandlerFuncptr> &list,
        RfxCreateHandlerFuncptr func_ptr, int c_id) {
    RFX_UNUSED(c_id);
    list.add(func_ptr);
}

void RfxHandlerManager::registerToHandleRequest(RfxBaseHandler *handler, unsigned int channel_id,
        int slot_id, const int *request_id_list, int length) {
    init();
#if defined(MTK_ONY_FOR_RILD_DEBUG)
    for (size_t i = 0; i < length; i++) {
        int id = request_id_list[i];
        int ret = RfxDataCloneManager::checkDataIfRegister(id, REQUEST);
        if (ret == 0) {
            RFX_LOG_E(RFX_LOG_TAG, "registerToHandleRequest fail :%d(%s)", id, RFX_ID_TO_STR(id));
        }
    }
#endif
    s_self->registerInternal(s_self->m_request_list[channel_id], handler, channel_id, slot_id,
            request_id_list, length);
}

void RfxHandlerManager::registerToHandleMipcUrc(RfxBaseHandler *handler, unsigned int channel_id,
        int slot_id, const int *mipc_urc_id_list, int length) {
    init();
    s_self->registerInternal(s_self->m_mipc_urc_list[channel_id], handler, channel_id, slot_id,
            mipc_urc_id_list, length);
}


void RfxHandlerManager::registerToHandleMipcCmd(RfxBaseHandler *handler, unsigned int channel_id,
        int slot_id, const int *mipc_cmd_id_list, int length) {
    init();
    s_self->registerInternal(s_self->m_mipc_cmd_list[channel_id], handler, channel_id, slot_id,
            mipc_cmd_id_list, length);
}

void RfxHandlerManager::unregisterToHandleMipcUrc(RfxBaseHandler *handler, unsigned int channel_id,
        int slot_id, const int *mipc_urc_id_list, int length) {
    s_self->m_mutex[channel_id].lock();
    SortedVector<RfxHandlerRegisterEntry> list = s_self->m_mipc_urc_list[channel_id];
    for (int i = 0; i < length; i++) {
        RfxHandlerRegisterEntry query_entry(handler, channel_id, slot_id, mipc_urc_id_list[i], String8(), false);
        ssize_t index = list.indexOf(query_entry);
        if (index >= 0) {
            list.removeAt(index);
        }
        RFX_LOG_D(RFX_LOG_TAG, "unregisterToHandleMipcUrc, handler = %p, channel = %s, id = %d, index = %zd",
                handler, RfxChannelManager::proxyIdToString(channel_id), mipc_urc_id_list[i], index);
    }
    s_self->m_mutex[channel_id].unlock();
}

void RfxHandlerManager::registerToHandleUrc(RfxBaseHandler *handler, unsigned int channel_id, int slot_id,
        const char **urc_prefix_list, int length, bool needAllMatch) {
    init();
    s_self->registerInternal(s_self->m_urc_list[channel_id], handler, channel_id, slot_id,
            urc_prefix_list, length, needAllMatch);
}

void RfxHandlerManager::registerToHandleEvent(RfxBaseHandler *handler, unsigned int channel_id, int slot_id,
        const int *event_id_list, int length) {
    init();
#if defined(MTK_ONY_FOR_RILD_DEBUG)
    for (size_t i = 0; i < length; i++) {
        int id = event_id_list[i];
        int ret = RfxDataCloneManager::checkDataIfRegister(id, EVENT_MESSAGE);
        if (ret == 0) {
            RFX_LOG_E(RFX_LOG_TAG, "registerToHandleEvent fail :%d(%s)", id, RFX_ID_TO_STR(id));
        }
    }
#endif
    s_self->registerInternal(s_self->m_event_list[channel_id], handler, channel_id, slot_id,
            event_id_list, length);
}

void RfxHandlerManager::registerToHandleEvent(RfxBaseHandler *handler, unsigned int channel_id, int slot_id,
        int client_id, const int *event_id_list, int length) {
    init();
    s_self->registerInternal(s_self->m_event_list[channel_id], handler, channel_id, slot_id,
            client_id, event_id_list, length);
}


void RfxHandlerManager::registerInternal(SortedVector<RfxHandlerRegisterEntry> &list,
        RfxBaseHandler *handler, unsigned int channel_id, int slot_id, const int *id_list, int length) {
    s_self->m_mutex[channel_id].lock();
    // wp<RfxBaseHandler> ptr;

    for (int i = 0; i < length; i++) {
        RFX_LOG_V(RFX_LOG_TAG, "registerInternal, register handler = %p, channel = %s, id = %d",
                handler, RfxChannelManager::proxyIdToString(channel_id), id_list[i]);
        // ptr = handler;
        RfxHandlerRegisterEntry entry(handler, channel_id, slot_id, id_list[i], String8(), false);
        size_t old_size = list.size();
        list.add(entry);
        if (list.size() == old_size) {
            RfxBaseHandler *dup_handler = findMsgHandlerInternal(list, channel_id, slot_id,
                    id_list[i], -1, NULL, false);
            if (handler == dup_handler) {
                continue; // same registry, framework allows
            }
            RFX_LOG_E(RFX_LOG_TAG, "duplicate register handler = %p, dup_handler = %p, id = %d,\
channel_id = %d, slot_id = %d", handler, dup_handler, id_list[i], channel_id,
                    slot_id);
            RFX_ASSERT(0);
        }
    }
    s_self->m_mutex[channel_id].unlock();
}


void RfxHandlerManager::registerInternal(SortedVector<RfxHandlerRegisterEntry> &list,
        RfxBaseHandler *handler, unsigned int channel_id, int slot_id, int client_id,
        const int *id_list, int length) {
    s_self->m_mutex[channel_id].lock();
    // wp<RfxBaseHandler> ptr;

    for (int i = 0; i < length; i++) {
        RFX_LOG_D(RFX_LOG_TAG, "registerInternal, register handler = %p, channel = %s, id = %d, \
client_id = %d", handler, RfxChannelManager::proxyIdToString(channel_id), id_list[i], client_id);
        // ptr = handler;
        RfxHandlerRegisterEntry entry(handler, channel_id, slot_id, id_list[i], client_id,
                String8(), false);
        size_t old_size = list.size();
        list.add(entry);

        if (list.size() == old_size) {
            RfxBaseHandler *dup_handler = findMsgHandlerInternal(list, channel_id, slot_id,
                    client_id, id_list[i], NULL, false);
            if (handler == dup_handler) {
                continue; // same registry, framework allows
            }
            RFX_LOG_E(RFX_LOG_TAG, "duplicate register handler = %p, dup_handler = %p, id = %d,\
                    channel_id = %d, slot_id = %d, client_id = %d", handler, dup_handler,
                    id_list[i], channel_id, slot_id, client_id);
            RFX_ASSERT(0);
        }
    }
    s_self->m_mutex[channel_id].unlock();
}

void RfxHandlerManager::registerInternal(SortedVector<RfxHandlerRegisterEntry> &list,
        RfxBaseHandler *handler, unsigned int channel_id, int slot_id, const char **urc_list, int length,
        bool needAllMatch) {
    s_self->m_mutex[channel_id].lock();
    //wp<RfxBaseHandler> ptr;

    for (int i = 0; i < length; i++) {
        RFX_LOG_D(RFX_LOG_TAG, "registerInternal, register handler = %p, channel = %s, urc = %s",
                handler, RfxChannelManager::proxyIdToString(channel_id), urc_list[i]);
        // ptr = handler;
        RfxHandlerRegisterEntry entry(handler, channel_id, slot_id, -1, urc_list[i], needAllMatch);
        size_t old_size = list.size();
        list.add(entry);
        if (list.size() == old_size) {
            RfxBaseHandler *dup_handler = findMsgHandlerInternal(list, channel_id, slot_id, -1,
                    -1, urc_list[i], needAllMatch);
            if (handler == dup_handler) {
                continue; // same registry, framework allows
            }
            RFX_LOG_E(RFX_LOG_TAG, "duplicate register handler = %p, dup_handler = %p, raw_urc =\
%s, channel_id = %d, slot_id = %d", handler, dup_handler, urc_list[i], channel_id,
                    slot_id);
            RFX_ASSERT(0);
        }
    }
    s_self->m_mutex[channel_id].unlock();
}

RfxBaseHandler* RfxHandlerManager::findMsgHandler(SortedVector<RfxHandlerRegisterEntry> &list,
        unsigned int channelId, int slotId, int id, int clientId, const char* urc) {
    RfxBaseHandler* handler = NULL;
    m_mutex[channelId].lock();
    handler = findMsgHandlerInternal(list, channelId, slotId, id, clientId, urc, false);
    m_mutex[channelId].unlock();
    return handler;
}

RfxBaseHandler* RfxHandlerManager::findMsgHandlerInternal(
        SortedVector<RfxHandlerRegisterEntry> &list, unsigned int channelId, int slotId, int id,
                int clientId, const char* urc, bool needAllMatch) {
    if (NULL == urc) {
        RfxHandlerRegisterEntry query_entry(NULL, channelId, slotId, id, clientId,
                String8(), needAllMatch);
        ssize_t index = list.indexOf(query_entry);
        if (index >= 0) {
            const RfxHandlerRegisterEntry &item = list.itemAt(index);
            //return item.m_handler.promote().get();
            RFX_LOG_D(RFX_LOG_TAG, "findMsgHandlerInternal, (request) channel id = %d, slot id =\
%d, request = %d, client = %d, raw_urc = %s, index = %zd", item.m_channel_id, item.m_slot_id,
                    item.m_id, item.m_client_id, item.m_raw_urc.string(), index);
            return item.m_handler;
        }
    } else {
        for (size_t i = 0; i < list.size(); i++) {
            const RfxHandlerRegisterEntry &item = list.itemAt(i);
            if (needAllMatch) {
                if (String8(urc) == item.m_raw_urc) {
                    RFX_LOG_D(RFX_LOG_TAG, "findMsgHandlerInternal, (specific urc) channel id = %d,\
slot id = %d, request = %d, raw_urc = %s", item.m_channel_id, item.m_slot_id, item.m_id,
                            item.m_raw_urc.string());
                    return item.m_handler;
                }
            } else {
                if (RfxMisc::strStartsWith(urc, item.m_raw_urc.string())) {
                    RFX_LOG_D(RFX_LOG_TAG, "findMsgHandlerInternal, (urc) channel id = %d, \
slot id = %d, request = %d, raw_urc = %s", item.m_channel_id, item.m_slot_id, item.m_id,
                            item.m_raw_urc.string());
                    return item.m_handler;
                }
            }
        }
    }
    return NULL;
}

int RfxHandlerManager::findMsgChannel(int type, int slot_id, int id, int client_id,
        const char *urc) {
    SortedVector<RfxHandlerRegisterEntry>* list = s_self->findListByType(type);
    int index = 0;
    char urc_temp[MAX_HIDEN_LOG_LEN] = {0};
    if (urc != NULL && (index = needToHidenLog(urc)) >= 0) {
        strncpy(urc_temp, (String8::format("%s:***", getHidenLogPreFix(index))).string(),
                (MAX_HIDEN_LOG_LEN - 1));
    }
    RFX_LOG_V(RFX_LOG_TAG, "findMsgChannel, type = %d, slot_id = %d, id = %d, client_id = %d, \
            urc = %s", type, slot_id, id, client_id, (strlen(urc_temp) == 0 ? urc : urc_temp));
    if (NULL == urc) {
        RfxHandlerRegisterEntry query_entry(NULL, -1, slot_id, id, client_id,
                String8(), false);
        int offset = slot_id * RIL_CHANNEL_OFFSET;
        for (int i = 0; i < RIL_CHANNEL_OFFSET; i++) {
            unsigned int targetChannel = i + offset;
            Mutex::Autolock autoLock(s_self->m_mutex[targetChannel]);
            ssize_t index = list[targetChannel].indexOf(query_entry);
            // RFX_LOG_D(RFX_LOG_TAG, "findMsgChannel, query_entry, slot id = %d, request = %d",
            //         query_entry.m_slot_id, query_entry.m_id);
            if (index >= 0) {
                const RfxHandlerRegisterEntry &item = list[targetChannel].itemAt(index);
                RFX_LOG_D(RFX_LOG_TAG, "findMsgChannel, (request) channel id = %d, slot id = %d, \
request = %d, client_id = %d, raw_urc = %s, index = %zd", item.m_channel_id, item.m_slot_id,
                        item.m_id, item.m_client_id, item.m_raw_urc.string(), index);
                return item.m_channel_id;
            }
        }
    } else {
        int offset = slot_id * RIL_CHANNEL_OFFSET;
        for(int i = 0; i < RIL_CHANNEL_OFFSET; i++) {
            unsigned int targetChannel = i + offset;
            Mutex::Autolock autoLock(s_self->m_mutex[targetChannel]);
            SortedVector<RfxHandlerRegisterEntry> targetList = list[targetChannel];
            int size = targetList.size();
            for (int j = 0; j < size; j++) {
                const RfxHandlerRegisterEntry &item = targetList.itemAt(j);
                if (item.mNeedAllMatch) {
                    if (String8(urc) == item.m_raw_urc) {
                        RFX_LOG_D(RFX_LOG_TAG, "findMsgChannel, (specific urc) channel id = %d,\
slot id = %d, request = %d, client_id = %d, raw_urc = %s", item.m_channel_id, item.m_slot_id, item.m_id,
                                item.m_client_id, item.m_raw_urc.string());
                        return item.m_channel_id;
                    }
                } else {
                    if (RfxMisc::strStartsWith(urc, item.m_raw_urc.string())) {
                        RFX_LOG_D(RFX_LOG_TAG, "findMsgChannel, (urc) channel id = %d,\
slot id = %d, request = %d, client_id = %d, raw_urc = %s",
                                item.m_channel_id, item.m_slot_id, item.m_id,
                                item.m_client_id, item.m_raw_urc.string());
                        return item.m_channel_id;
                    }
                }
            }
        }
    }
    return -1;
}

void RfxHandlerManager::processMessage(const sp<RfxMclMessage>& msg) {
    // dispatch to correspend handler
    SortedVector <RfxHandlerRegisterEntry> list = s_self->findListByChannel(msg->getType(),
            msg->getChannelId());
    int slotId;
    if (msg->getSendToMainProtocol()) {
        slotId = RfxMclStatusManager::getMclStatusManager(RFX_SLOT_ID_UNKNOWN)->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    } else {
        slotId = msg->getSlotId();
    }
    RfxBaseHandler* handler;
    if (MIPC_URC == msg->getType() || MIPC_CMD == msg->getType()) {
        handler = s_self->findMsgHandler(list, msg->getChannelId(),
                slotId, msg->getMipcData()->getMipcMsgId(), msg->getClientId(),
                (msg->getRawUrc() == NULL ? NULL : msg->getRawUrc()->getLine()));
    } else {
        handler = s_self->findMsgHandler(list, msg->getChannelId(),
                slotId, msg->getId(), msg->getClientId(),
                (msg->getRawUrc() == NULL ? NULL : msg->getRawUrc()->getLine()));
    }
    if (handler != NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "processMessage, handler: %p, message = %s. execute on %s",
                handler, msg->toString().string(),
                RfxChannelManager::proxyIdToString(msg->getChannelId()));
        handler->processMessage(msg);
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "processMessage, no one register id: %d", msg->getId());
        // send response with error code: not support
        sp<RfxMclMessage> resMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED,
                RfxVoidData(), msg);
        RfxDispatchThread::enqueueResponseMessage(resMsg);
    }
}

void RfxHandlerManager::initHandler(int channel_id) {
    multimap<int, RfxCreateHandlerFuncptr>::iterator iter, beg, end;
    int count = 0;
    // for slot
    int slot = channel_id/RIL_PROXY_OFFSET;
    unsigned int target_channel = channel_id%RIL_PROXY_OFFSET;
    count = s_self->m_slot_handler_list[target_channel].size();
    RFX_LOG_D(RFX_LOG_TAG, "initHandler slot handler count = %d, channel = %s", count,
            RfxChannelManager::proxyIdToString(channel_id));
    for (int i = 0; i < count; i++) {
        RFX_LOG_D(RFX_LOG_TAG, "initHandler counter = %d", i);
        RfxCreateHandlerFuncptr ptr =
                (RfxCreateHandlerFuncptr) s_self->m_slot_handler_list[target_channel].itemAt(i);
        ptr(slot, target_channel/*Hanlder will not get real channel*/);
    }

    // LibDumpRildInfo::createNewDumpMessage(5);
    int supportChannel = RfxChannelManager::getSupportChannels();
    sWaitChannelDoneMutex.lock();
    sCreateChannelNumber++;
    RFX_LOG_D(RFX_LOG_TAG, "initHandler create channel num = %d, %d,%d",
            sCreateChannelNumber, supportChannel, channel_id);
    if (sCreateChannelNumber == supportChannel) {
        // rild init done
        RFX_LOG_I(RFX_LOG_TAG, "initHandler create all channels done!");
        setRildInitDoneState(1);
        RfxMclStatusManager::getNonSlotMclStatusManager()->setBoolValue(
                RFX_STATUS_KEY_RILD_INIT_HANDLER_DONE, true);
        if (RfxRilUtils::isMipcSupport() ==  true) {
            sp<RfxMclMessage> oemMsg = RfxMclMessage::obtainEvent(RFX_MSG_EVENT_CHANNEL_INIT_DONE,
                    RfxVoidData(), RIL_CMD_PROXY_9, RfxRilUtils::getMajorSim() - 1);
            RfxMclDispatcherThread::enqueueMclMessage(oemMsg);
        }
        // LibDumpRildInfo::setCreateChannelDone(true);
    }
    sWaitChannelDoneMutex.unlock();
}

SortedVector<RfxHandlerRegisterEntry>* RfxHandlerManager::findListByType(int type) {
    switch(type) {
        case REQUEST:
        case SAP_REQUEST:
        case MIPC_RESPONSE:
            return m_request_list;
        case RAW_URC:
            return m_urc_list;
        case EVENT_MESSAGE:
        case MIPC_EVENT_RESPONSE:
            return m_event_list;
        case MIPC_URC:
            return m_mipc_urc_list;
        case MIPC_CMD:
            return m_mipc_cmd_list;
        default:
            RFX_LOG_E(RFX_LOG_TAG, "unknow type to find list");
            RFX_ASSERT(0);
    }
}

SortedVector<RfxHandlerRegisterEntry> RfxHandlerManager::findListByChannel(int type,
        unsigned int channel_id) {
    RFX_ASSERT(channel_id < RfxChannelManager::getSupportChannels());
    switch(type) {
        case REQUEST:
        case SAP_REQUEST:
        case MIPC_RESPONSE:
            return m_request_list[channel_id];
        case RAW_URC:
            return m_urc_list[channel_id];
        case EVENT_MESSAGE:
        case MIPC_EVENT_RESPONSE:
            return m_event_list[channel_id];
        case MIPC_URC:
            return m_mipc_urc_list[channel_id];
        case MIPC_CMD:
            return m_mipc_cmd_list[channel_id];
        default:
            RFX_LOG_E(RFX_LOG_TAG, "unknow type = %d or channel id = %d", type, channel_id);
            RFX_ASSERT(0);
    }
}
