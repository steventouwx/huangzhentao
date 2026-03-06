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
#include <string>
#include <vector>

#include "RfxBaseHandler.h"
#include "RfxHandlerManager.h"
#include "RfxMclDispatcherThread.h"
#include "RfxDispatchThread.h"
#include "RfxIntsData.h"
#include "RfxIdToStringUtils.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxRawData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RfxLog.h"
#include "RfxVersionManager.h"
#include "mipc_msg_host.h"
#include "RfxMipcUtils.h"
#include "mipc_msg.h"
#include <libmtkrilutils.h>
#include <dlfcn.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include <platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
#include "pb_decode.h"
#include "pb_encode.h"
#ifdef __cplusplus
}
#endif

#ifdef MTK_MBRAIN_SUPPORT
#include <aidl/vendor/mediatek/hardware/mbrain/IMBrain.h>
#include <aidl/vendor/mediatek/hardware/mbrain/BnMBrainCallbacks.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/IServiceManager.h>
#include <json/json.h>

using namespace Json;
using ::aidl::vendor::mediatek::hardware::mbrain::IMBrain;
using ::aidl::vendor::mediatek::hardware::mbrain::MBrain_Parcelable;
using ::aidl::vendor::mediatek::hardware::mbrain::MBrain_Event;
#endif

extern "C"
void mipcUrcIndCallback(mipc_msg_t *msg_ptr, void*priv_ptr);
extern "C"
void mipcResponseCallback(mipc_msg_t *msg_ptr, void* priv_ptr);

typedef struct {
    int msgId;
    int timeout;
} MipcAyncTimeOut;

static MipcAyncTimeOut sMipcAyncTimeOut[] = {
    #include "RfxMipcAsyncTimeOut.h"
};

RFX_REGISTER_DATA_TO_URC_ID(RfxRawData, RFX_MSG_URC_SIM_SAP_STATUS);

#define RFX_LOG_TAG "RfxBaseHandler"

void RfxBaseHandler::processMessage(const sp<RfxMclMessage>& msg) {
    if (REQUEST == msg->getType() || SAP_REQUEST == msg->getType()) {
        onHandleRequest(msg);
    } else if (RAW_URC == msg->getType() || MIPC_URC == msg->getType()) {

        // check radio state condition
        RIL_RadioState radioState = (RIL_RadioState) getMclStatusManager()->getIntValue(
                RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
        if (onCheckIfRejectMessage(msg, radioState)) {
            return;
        }

        onHandleUrc(msg);
    } else if (MIPC_RESPONSE == msg->getType() || MIPC_EVENT_RESPONSE == msg->getType()) {
        onHandleResponse(msg);
    } else if (EVENT_MESSAGE == msg->getType()) {
        onHandleEvent(msg);
    } else if (MIPC_CMD == msg->getType()) {
        onHandleCmd(msg);
    }
}

void RfxBaseHandler::registerToHandleRequest(const int *request_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleRequest(this , realChannelId, m_slot_id, request_id_list,
            length);
}

void RfxBaseHandler::registerToHandleURC(const char **urc_prefix_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleUrc(this , realChannelId, m_slot_id, urc_prefix_list, length);
}

void RfxBaseHandler::registerToHandleMipcURC(const int *urc_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleMipcUrc(this , realChannelId, m_slot_id, urc_id_list, length);
    for (unsigned int i = 0; i < length; i++) {
        int ret = 0;
        ret = mipc_msg_register_ind((RfxMipcUtils::slotIdChangeToMipcSlotId(m_slot_id)),
                (uint16_t)urc_id_list[i], (void*)mipcUrcIndCallback, NULL);
        if (ret < 0) {
            logE(RFX_LOG_TAG, "registerToHandleMipcURC error:%d, slotId:%d, mipcId:%04x",
                    ret, m_slot_id, urc_id_list[i]);
            if (ret == ERR_UNEXPECTED_MSG_TYPE) {
                if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                    mtkAssert((char *)"registerToHandleMipcURC unexpected type mipcId");
                }
            }
        }
    }
}

void RfxBaseHandler::registerToMipcInd(const int *urc_id_list, size_t length) {
    for (unsigned int i = 0; i < length; i++) {
        int ret = 0;
        ret = mipc_msg_register_ind((RfxMipcUtils::slotIdChangeToMipcSlotId(m_slot_id)),
                (uint16_t)urc_id_list[i], (void*)mipcUrcIndCallback, NULL);
        logE(RFX_LOG_TAG, "registerToMipcInd error:%d, slotId:%d, mipcId:%04x",
            ret, m_slot_id, urc_id_list[i]);
        if (ret < 0) {
            if (ret == ERR_UNEXPECTED_MSG_TYPE) {
                if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                    mtkAssert((char *)"registerToMipcInd unexpected type mipcId");
                }
            }
        }
    }
}

void RfxBaseHandler::registerToHandleMipcCmd(const int *cmd_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleMipcCmd(this , realChannelId, m_slot_id, cmd_id_list, length);
    for (unsigned int i = 0; i < length; i++) {
        int ret = 0;
        ret = mipc_msg_register_cmd((uint16_t)cmd_id_list[i], (void*)mipcCmdCallback, NULL);
        if (ret < 0) {
            logE(RFX_LOG_TAG, "registerToHandleMipcCmd error:%d, slotId:%d, mipcId:%04x",
                    ret, m_slot_id, cmd_id_list[i]);
            if (ret == ERR_UNEXPECTED_MSG_TYPE) {
                if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                    mtkAssert((char *)"registerToHandleMipcCmd unexpected type mipcId");
                }
            }
        }
    }
}

void RfxBaseHandler::unregisterToHandleMipcURC(const int *urc_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::unregisterToHandleMipcUrc(this , realChannelId, m_slot_id, urc_id_list, length);

    for (unsigned int i = 0; i < length; i++) {
        int ret = 0;
        ret = mipc_msg_unregister_ind_api((RfxMipcUtils::slotIdChangeToMipcSlotId(m_slot_id)),
                (uint16_t)urc_id_list[i]);
        if (ret < 0) {
            logE(RFX_LOG_TAG, "unregisterToHandleMipcURC error:%d, slotId:%d, mipcId:%04x",
                    ret, m_slot_id, urc_id_list[i]);
            if (ret == ERR_UNEXPECTED_MSG_TYPE) {
                if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                    mtkAssert((char *)"unregisterToHandleMipcURC unexpected type mipcId");
                }
            }
        }
    }
}

void RfxBaseHandler::unregisterToMipcInd(const int *urc_id_list, size_t length) {
    for (unsigned int i = 0; i < length; i++) {
        int ret = 0;
        ret = mipc_msg_unregister_ind_api((RfxMipcUtils::slotIdChangeToMipcSlotId(m_slot_id)),
                (uint16_t)urc_id_list[i]);
        logE(RFX_LOG_TAG, "unregisterToMipcInd error:%d, slotId:%d, mipcId:%04x",
                ret, m_slot_id, urc_id_list[i]);
        if (ret < 0) {
            if (ret == ERR_UNEXPECTED_MSG_TYPE) {
                if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                    mtkAssert((char *)"unregisterToMipcInd unexpected type mipcId");
                }
            }
        }
    }
}

extern "C"
void mipcUrcIndCallback(mipc_msg_t *msg_ptr, void* priv_ptr) {
    RFX_UNUSED(priv_ptr);
    RfxMipcData* mipcData = new RfxMipcData(msg_ptr, true);
    sp<RfxMclMessage> msg = RfxMclMessage::obtainMipcUrc(mipcData);
    RFX_LOG_D(RFX_LOG_TAG, "mipcUrcIndCallback mipc slot id %d, mipc id %d",
            mipcData->getMipcSlotId(), mipcData->getMipcMsgId());
    RfxMclDispatcherThread::waitLooper();
    RfxMclDispatcherThread::enqueueMclMessage(msg);
}

extern "C"
void mipcCmdCallback(mipc_msg_t *msg_ptr, void* priv_ptr) {
    RFX_UNUSED(priv_ptr);
    RfxMipcData* mipcData = new RfxMipcData(msg_ptr, true);
    sp<RfxMclMessage> msg = RfxMclMessage::obtainMipcCmd(mipcData);
    RFX_LOG_D(RFX_LOG_TAG, "mipcCmdCallback mipc id %d", mipcData->getMipcMsgId());
    RfxMclDispatcherThread::waitLooper();
    RfxMclDispatcherThread::enqueueMclMessage(msg);
}

void RfxBaseHandler::registerToHandleEvent(const int *event_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleEvent(this , realChannelId, m_slot_id, event_id_list, length);
}

void RfxBaseHandler::registerToHandleEvent(int client_id, const int *event_id_list,
            size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleEvent(this,
            realChannelId, m_slot_id, client_id, event_id_list, length);
}

void RfxBaseHandler::responseToTelCore(const sp<RfxMclMessage>& msg) {
    if (RESPONSE == msg->getType()) {
        RfxDispatchThread::enqueueResponseMessage(msg);
    } else if (URC == msg->getType()) {
        RfxDispatchThread::enqueueUrcMessage(msg);
    } else if (SAP_RESPONSE == msg->getType()) {
        RfxDispatchThread::enqueueSapResponseMessage(msg);
    } else if (SAP_URC == msg->getType()) {
        RfxDispatchThread::enqueueSapUrcMessage(msg);
    } else {
        logE(RFX_LOG_TAG, "responseToTelCore error!");
    }
}

void RfxBaseHandler::sendEvent(int id, const RfxBaseData &data, int channelId,
        int slotId, int clientId, int token, nsecs_t delayTime, MTK_RIL_REQUEST_PRIORITY priority) {
    sp<RfxMclMessage> msg = RfxMclMessage::obtainEvent(id, data, channelId, slotId, clientId,
            token, delayTime, priority);
    sendEvent(msg);
}

void RfxBaseHandler::sendEvent(sp<RfxMclMessage> msg) {
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        if (RfxRilUtils::updateEventToGT(
                msg->getSlotId(),
                msg->getChannelId(),
                msg->getId(),
                msg->getData()->getData(),
                msg->getData()->getDataLength(),
                msg->getClientId(),
                msg->getToken(),
                msg->getDelayTime(),
                msg->getPriority())) {
            return;
        }
    }
    msg->setMainProtocolSlotId(getNonSlotMclStatusManager()->getIntValue(
                    RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0));
    if (msg->getDelayTime() > 0) {
        RfxMclDispatcherThread::enqueueMclMessageDelay(msg);
    } else {
        RfxMclDispatcherThread::enqueueMclMessage(msg);
    }
}

void RfxBaseHandler::sendBtSapStatusInd(RIL_SIM_SAP_STATUS_IND_Status message) {
    // after encoding, send this message to telecore
    int msgId = MsgId_RIL_SIM_SAP_STATUS;
    RIL_SIM_SAP_STATUS_IND unsolMsg;
    size_t encoded_size = 0;
    size_t buffer_size = 0;
    pb_ostream_t ostream;
    bool success = false;
    int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_INIT);

    logD("[BTSAP]", "notifyBtSapStatusInd, (%d)", message);
    unsolMsg.statusChange = message;

    if (message == RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_CARD_RESET &&
            status == BT_SAP_CONNECTION_SETUP) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_ONGOING_CONNECTION);
    }

    // send unsolicited message STATUS_IND
    if ((success = pb_get_encoded_size(&encoded_size, RIL_SIM_SAP_STATUS_IND_fields,
            &unsolMsg)) && encoded_size <= INT32_MAX) {
        buffer_size = encoded_size;
        uint8_t buffer[buffer_size];
        ostream = pb_ostream_from_buffer(buffer, buffer_size);
        success = pb_encode(&ostream, RIL_SIM_SAP_STATUS_IND_fields, &unsolMsg);

        if(success) {
            logD("[BTSAP]", "notifyBtSapStatusInd, Size: %zu (0x%zx)", encoded_size, encoded_size);
            sp<RfxMclMessage> response = RfxMclMessage::obtainSapUrc(RFX_MSG_URC_SIM_SAP_STATUS,
                    m_slot_id, RfxRawData(buffer, buffer_size));
            responseToTelCore(response);
        } else {
            logE("[BTSAP]", "notifyBtSapStatusInd, Encode failed!");
        }
    } else {
        logE("[BTSAP]", "Not sending response type %d: encoded_size: %zu. encoded size result: %d",
                msgId, encoded_size, success);
    }
}

sp<RfxSender> RfxBaseHandler::getSender() {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    return RfxChannelManager::getSender(realChannelId);
}

void RfxBaseHandler::logD(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);
    if (ret >= 0) {
        RfxRilUtils::printLog(DEBUG, tagString,
                String8::format("%s", buf), m_slot_id);
    }
}

void RfxBaseHandler::logI(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);
    if (ret >= 0) {
        RfxRilUtils::printLog(INFO, tagString,
                String8::format("%s", buf), m_slot_id);
    }
}

void RfxBaseHandler::logV(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);
    if (ret >= 0) {
        RfxRilUtils::printLog(VERBOSE, tagString,
                String8::format("%s", buf), m_slot_id);
    }
}

void RfxBaseHandler::logE(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);
    if (ret >= 0) {
        RfxRilUtils::printLog(ERROR, tagString,
                String8::format("%s", buf), m_slot_id);
    }
}

void RfxBaseHandler::logW(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);
    if (ret >= 0) {
        RfxRilUtils::printLog(WARN, tagString,
                String8::format("%s", buf), m_slot_id);
    }
}

RfxMclStatusManager* RfxBaseHandler::getMclStatusManager() {
    return getMclStatusManager(m_slot_id);
}

RfxMclStatusManager* RfxBaseHandler::getMclStatusManager(int slotId) {
    return RfxMclStatusManager::getMclStatusManager(slotId);
}

RfxMclStatusManager* RfxBaseHandler::getNonSlotMclStatusManager() {
    return RfxMclStatusManager::getMclStatusManager(RFX_SLOT_ID_UNKNOWN);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandSingleline(const char *command,
        const char *responsePrefix, RIL_Token ackToken) {
    return getSender()->atSendCommandSinglelineAck(command, responsePrefix,
            ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandSingleline(const String8 &command,
        const char *responsePrefix, RIL_Token ackToken) {
    return getSender()->atSendCommandSinglelineAck(command.string(), responsePrefix,
            ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandNumeric(const char *command,
        RIL_Token ackToken) {
    return getSender()->atSendCommandNumericAck(command, ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommandNumeric(const String8 &command,
        RIL_Token ackToken) {
    return getSender()->atSendCommandNumericAck(command.string(), ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandMultiline(const char *command,
        const char *responsePrefix, RIL_Token ackToken) {
    return getSender()->atSendCommandMultilineAck(command, responsePrefix,
            ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommandMultiline(const String8 &command,
        const char *responsePrefix, RIL_Token ackToken) {
    return getSender()->atSendCommandMultilineAck(command.string(), responsePrefix,
            ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommand(const char *command,
        RIL_Token ackToken) {
    return getSender()->atSendCommandAck(command, ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommand(const String8 &command,
        RIL_Token ackToken) {
    return getSender()->atSendCommandAck(command.string(), ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandRaw(const char *command,
        RIL_Token ackToken) {
    return getSender()->atSendCommandRawAck(command, ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommandRaw(const String8 &command,
        RIL_Token ackToken) {
    return getSender()->atSendCommandRawAck(command.string(), ackToken);
}

bool RfxBaseHandler::sendUserData(int clientId, unsigned char* data, size_t length) {
    return getSender()->sendUserData(clientId, data, length);
}

bool RfxBaseHandler::sendUserData(int clientId, int config, unsigned char* data, size_t length) {
    return getSender()->sendUserData(clientId, config, data, length);
}

int RfxBaseHandler::strStartsWith(const char *line, const char *prefix) {
    return RfxMisc::strStartsWith(line, prefix);
}

const char* RfxBaseHandler::idToString(int id) {
    return RFX_ID_TO_STR(id);
}

int RfxBaseHandler::getFeatureVersion(char *feature, int defaultVaule) {
    return RfxVersionManager::getInstance()->getFeatureVersion(feature, defaultVaule);
}

int RfxBaseHandler::getFeatureVersion(char *feature) {
    return getFeatureVersion(feature, 0);
}

void RfxBaseHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
}

void RfxBaseHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
}

void RfxBaseHandler::onHandleCmd(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
}

void RfxBaseHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
}


void RfxBaseHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    responseToTelCore(msg);
}

typedef int (*MBNotifyMIPCTimeout)(int slotId, int mipcId, int txId, int syncType, const char* extraInfo);

sp<RfxMipcData> RfxBaseHandler::callToMipcMsgSync(sp<RfxMipcData> mipcMsg, uint32_t timeout_ms) {
    mipc_msg_t *msg_cnf_ptr;

    void *hLibHandle;
    MBNotifyMIPCTimeout notifyMIPCTimeout;

    logD(RFX_LOG_TAG, "callToMipcMsgSync mipcSlotId:%d, mipcId:%04x, timeout_ms:%d",
            mipcMsg->getMipcSlotId(), mipcMsg->getMipcMsgId(), timeout_ms);
    mipc_ret_cause_enum causeValue = ERR_NO_ERR;

    if (0 == timeout_ms) {
        causeValue = mipc_msg_sync_with_cause(mipcMsg->getData(), &msg_cnf_ptr);
    } else {
        mipc_msg_set_timeout_once(timeout_ms);
        causeValue = mipc_msg_sync_timeout_with_cause(mipcMsg->getData(), &msg_cnf_ptr);
    }

    if (causeValue != ERR_NO_ERR) {
        logE(RFX_LOG_TAG, "Sync time out error number:%d", causeValue);
    }

    if (causeValue != ERR_WRITE_ERROR && msg_cnf_ptr == NULL) {
        bool mdOff = RfxMclStatusManager::getNonSlotMclStatusManager()->getBoolValue(
                RFX_STATUS_KEY_MODEM_POWER_OFF, false);

        logE(RFX_LOG_TAG, "Sync time out mipcSlotId:%d, mipcId:%04x, timeout_ms:%d, mdoff:%d, causeValue:%d",
                mipcMsg->getMipcSlotId(), mipcMsg->getMipcMsgId(), timeout_ms, mdOff, causeValue);

#ifdef MTK_MBRAIN_SUPPORT
        // call to mbrain binder
        static const std::string serviceName = std::string() + (IMBrain::descriptor ? IMBrain::descriptor : "") + "/default";
        int ret = 0;
        shared_ptr<IMBrain> ptr =  IMBrain::fromBinder(ndk::SpAIBinder(AServiceManager_checkService(serviceName.c_str())));
        if (ptr != nullptr) {
            ptr->IsMBrainSupport(&ret);
            if (ret == 1) {
                MBrain_Parcelable mbrain_parcelable_input;
                MBrain_Event mbEvent = MBrain_Event::MB_TELEPHONY_RILD_EVENT_NOTIFY;
                mbrain_parcelable_input.eventHint = 0;

                Json::Value root = Value(objectValue);
                Json::Value mipcTimeout = Value(objectValue);
                struct timespec spec;
                if (clock_gettime(CLOCK_REALTIME, &spec) == -1) {
                    mipcTimeout["timestamp"] = 0;
                } else {
                    mipcTimeout["timestamp"] = spec.tv_sec * 1000 + spec.tv_nsec / 1e6;
                }
                mipcTimeout["slotId"] = mipcMsg->getMipcSlotId();
                mipcTimeout["mipcId"] = mipcMsg->getMipcMsgId();
                mipcTimeout["txId"] = timeout_ms;
                mipcTimeout["syncType"] = mdOff;
                mipcTimeout["extraInfo"] = "sync";;
                root["mipcTimeout"] = mipcTimeout;

                Json::StreamWriterBuilder builder;
                std::string result = Json::writeString(builder, root);
                mbrain_parcelable_input.privData = String8(result.c_str());
                ndk::ScopedAStatus status = ptr->Notify(mbEvent, mbrain_parcelable_input, &ret);
                if (!status.isOk()) {
                    RFX_LOG_E(RFX_LOG_TAG, "callToMipcMsgSync() binder MBrain fail");
                }
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "callToMipcMsgSync() MBrain not support");
            }
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "callToMipcMsgSync() get MBrain server fail");
        }
#endif

        if (false == mdOff) {
            if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                pid_t pid = getpid();
                char* str = NULL;
                int length = asprintf(&str, "%d", pid);
                if ((length != -1) && (str != NULL)) {
                    rfx_property_set("vendor.ril.mux.report.case", str);
                    rfx_property_set("vendor.ril.muxreport", "1");
                    free(str);
                }
                mtkAssert((char *)"callToMipcMsgSync too long time out, assert!!!");
            } else {
                logE(RFX_LOG_TAG, "Sync time out mipcSlotId:%d, mipcId:%04x, timeout_ms:%d",
                    mipcMsg->getMipcSlotId(), mipcMsg->getMipcMsgId(), timeout_ms);
                rfx_property_set("vendor.ril.mux.report.case", "2");
                rfx_property_set("vendor.ril.muxreport", "1");
            }
        } // false == mdOff
    }

    sp<RfxMipcData> mipcData = RfxMipcData::obtainMipcData(msg_cnf_ptr);
    logD(RFX_LOG_TAG, "callToMipcMsgSync return mipcSlotId:%d, mipcId:%04x",
            mipcData->getMipcSlotId(), mipcData->getMipcMsgId());
    return mipcData;
}

int RfxBaseHandler::callToMipcMsgCmdResponse(sp<RfxMipcData> mipcMsg) {
    int ret = 0;
    ret = mipc_msg_rsp(mipcMsg->getData());
    if (ret < 0) {
        logE(RFX_LOG_TAG, "callToMipcMsgCmdResponse error:%d, mipcId:%04x", ret, mipcMsg->getMipcMsgId());
        if (ret == ERR_UNEXPECTED_MSG_TYPE) {
            if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                mtkAssert((char *)"callToMipcMsgCmdResponse unexpected type mipcId");
            }
        }
    }
    return ret;
}

int RfxBaseHandler::callToMipcMsgNtf(sp<RfxMipcData> mipcMsg) {
    int ret = 0;
    ret = mipc_msg_ntf(mipcMsg->getData());
    if (ret < 0) {
        logE(RFX_LOG_TAG, "callToMipcMsgNtf error:%d, mipcId:%04x", ret, mipcMsg->getMipcMsgId());
        if (ret == ERR_UNEXPECTED_MSG_TYPE) {
            if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                mtkAssert((char *)"callToMipcMsgNtf unexpected type mipcId");
            }
        }
    }
    return ret;
}

int RfxBaseHandler::getMipcAyncTimeOut(int msg_id) {
    // default time out is 300s
    if (NUM_ELEMS(sMipcAyncTimeOut) > 0) {
        for (size_t i = 0; i < NUM_ELEMS(sMipcAyncTimeOut); i++) {
            if (msg_id == sMipcAyncTimeOut[i].msgId) {
                return (sMipcAyncTimeOut[i].timeout * 60);
            }
        }
    }
    return 300;
}

int RfxBaseHandler::checkMipcAsyncMessageQueueTimeOut(int token) {
    sp<RfxMclMessage> frontMsg;
    nsecs_t time;

    frontMsg = RfxMclDispatcherThread::getFrontMclMessageFromPendingQueue();
    if (frontMsg == NULL) {
        return 0;
    }

    time = systemTime(SYSTEM_TIME_MONOTONIC) - frontMsg->getMipcAsyncTimeStamp();
    if (time > 0) {
        long timeSeconds = ns2s(time);
        long timeoutSeconds = getMipcAyncTimeOut(frontMsg->getId());
        if (timeSeconds > timeoutSeconds) {
            int mclToken = frontMsg->getMclToken();
            RFX_LOG_E(RFX_LOG_TAG, "ASync time out (%d)(%d)", token, mclToken);
#ifdef MTK_MBRAIN_SUPPORT
            // call to mbrain binder
            static const std::string serviceName = std::string() + (IMBrain::descriptor ? IMBrain::descriptor : "") + "/default";
            int ret = 0;
            shared_ptr<IMBrain> ptr =  IMBrain::fromBinder(ndk::SpAIBinder(AServiceManager_checkService(serviceName.c_str())));
            if (ptr != nullptr) {
                ptr->IsMBrainSupport(&ret);
                if (ret == 1) {
                    MBrain_Parcelable mbrain_parcelable_input;
                    MBrain_Event mbEvent = MBrain_Event::MB_TELEPHONY_RILD_EVENT_NOTIFY;
                    mbrain_parcelable_input.eventHint = 0;

                    Json::Value root = Value(objectValue);
                    Json::Value mipcTimeout = Value(objectValue);
                    struct timespec spec;
                    if (clock_gettime(CLOCK_REALTIME, &spec) == -1) {
                        mipcTimeout["timestamp"] = 0;
                    } else {
                        mipcTimeout["timestamp"] = spec.tv_sec * 1000 + spec.tv_nsec / 1e6;
                    }
                    mipcTimeout["slotId"] = frontMsg->getSlotId();
                    mipcTimeout["mipcId"] = frontMsg->getId();
                    mipcTimeout["txId"] = timeoutSeconds;
                    mipcTimeout["syncType"] = 1;
                    mipcTimeout["extraInfo"] = frontMsg->toString().string();;
                    root["mipcTimeout"] = mipcTimeout;

                    Json::StreamWriterBuilder builder;
                    std::string result = Json::writeString(builder, root);
                    mbrain_parcelable_input.privData = String8(result.c_str());
                    ndk::ScopedAStatus status = ptr->Notify(mbEvent, mbrain_parcelable_input, &ret);
                    if (!status.isOk()) {
                        RFX_LOG_E(RFX_LOG_TAG, "checkMipcAsyncMessageQueueTimeOut() binder MBrain fail");
                    }
                } else {
                    RFX_LOG_E(RFX_LOG_TAG, "checkMipcAsyncMessageQueueTimeOut() MBrain not support");
                }
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "checkMipcAsyncMessageQueueTimeOut() get MBrain server fail");
            }
#endif
            if (mclToken != token) {
                if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                    RFX_LOG_E(RFX_LOG_TAG, "ASync time out %s, timeout:%ld, timeoutSeconds:%ld",
                            frontMsg->toString().string(), timeSeconds, timeoutSeconds);
                    pid_t pid = getpid();
                    char* str = NULL;
                    int length = asprintf(&str, "%d", pid);
                    if ((length != -1) && (str != NULL)) {
                        rfx_property_set("vendor.ril.mux.report.case", str);
                        rfx_property_set("vendor.ril.muxreport", "1");
                        free(str);
                    }
                    mtkAssert((char *)"ASync time out the front message queue time out, assert!!!");
                } else {
                    RFX_LOG_E(RFX_LOG_TAG, "No Assert! ASync time out %s", frontMsg->toString().string());
                    rfx_property_set("vendor.ril.mux.report.case", "2");
                    rfx_property_set("vendor.ril.muxreport", "1");
                }
            }  // mclToken != token
        }  // timeSeconds > timeoutSeconds
    }  // time > 0
    return 0;
}

int RfxBaseHandler::callToMipcMsgAsync(const sp<RfxMclMessage>& msg, sp<RfxMipcData> mipcData) {
    mipc_ret_cause_enum ret = ERR_NO_ERR;
    MipcAyncCallbackData *callbackData = (MipcAyncCallbackData *)malloc(sizeof(MipcAyncCallbackData));
    if (callbackData == NULL) {
        logE(RFX_LOG_TAG, "callToMipcMsgAsync alloc memory fail");
        return -1;
    }

    callbackData->msg_type = msg->getType();
    callbackData->slot_id = msg->getSlotId();
    callbackData->token = msg->getToken();
    msg->setMipcAsyncTimeStamp(systemTime(SYSTEM_TIME_MONOTONIC));
    // if (msg->getType() == EVENT_MESSAGE) {
        if (msg->getMclToken() < 1) {
            msg->setMclToken(RfxMclMessage::generateMclToken());
        }
        RfxMclDispatcherThread::addMclMessageToPendingQueue(msg);
    // }
    logD(RFX_LOG_TAG, "callToMipcMsgAsync msgType:%d, slotId:%d, token:%d, mclToken:%d, mipcId:%04x",
            msg->getType(), msg->getSlotId(), msg->getToken(),
            msg->getMclToken(), mipcData->getMipcMsgId());
    callbackData->mipcToken = msg->getMclToken();
    ret = mipc_msg_async(mipcData->getData(), (void*)mipcResponseCallback, (void*)callbackData);

    if (ret != ERR_NO_ERR) {
        logE(RFX_LOG_TAG, "callToMipcMsgAsync unexpected type mipcId:%04x", mipcData->getMipcMsgId());
        if (ret == ERR_UNEXPECTED_MSG_TYPE) {
            if ((isInternalLoad() == 1) || (isUserLoad() != 1)) {
                mtkAssert((char *)"callToMipcMsgAsync unexpected type mipcId");
            }
        }
    }

    return ret;
}

extern "C"
void mipcResponseCallback(mipc_msg_t *msg_ptr, void* priv_ptr) {
    RfxMipcData* mipcData = new RfxMipcData(msg_ptr, true);
    MipcAyncCallbackData *callbackData = (MipcAyncCallbackData *)priv_ptr;
    int token = callbackData->token;
    int slotId = callbackData->slot_id;
    int msgType = callbackData->msg_type;
    int mclToken = callbackData->mipcToken;
    free(callbackData);
    callbackData = NULL;

    RfxBaseHandler::checkMipcAsyncMessageQueueTimeOut(mclToken);

    RFX_LOG_D(RFX_LOG_TAG, "mipcResponseCallback mSId:%d, mId:%04x, mT:%d, sId:%d, token:%d, mT:%d",
            mipcData->getMipcSlotId(), mipcData->getMipcMsgId(), msgType, slotId, token, mclToken);

    sp<RfxMclMessage> msg = RfxMclDispatcherThread::getAndRemoveMclMessageFromPendingQueue(mclToken);

    if (msg == NULL) {
        return;
    }

    if (msgType == EVENT_MESSAGE) {
        msg->setType(MIPC_EVENT_RESPONSE);
    } else {
        msg->setType(MIPC_RESPONSE);
    }
    msg->setMipcData(mipcData);
    RfxMclDispatcherThread::waitLooper();
    RfxMclDispatcherThread::enqueueMclMessage(msg);
}

bool RfxBaseHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg,
        RIL_RadioState radioState) {
    RFX_UNUSED(msg);
    RFX_UNUSED(radioState);
    return false;
}

/* An utility function for Urc.
 * To parse the URC and convert to the MCL data without any logic.
 */
void RfxBaseHandler::notifyStringsDataToTcl(
            const sp<RfxMclMessage>& msg, int rfxMsg, int maxLen, bool appendPhoneId) {

    RfxAtLine *urc = msg->getRawUrc();
    if (urc == NULL) return;
    // token init
    int ret;
    urc->atTokStart(&ret);
    if (ret < 0) return;

    std::vector<char*> data;

    // start to access token
    int index = 0;
    while (maxLen == 0 || index < maxLen) {
        char* str = urc->atTokNextstr(&ret);
        if (ret < 0) break;
        if (str == NULL) {
            str = (char*)"";  // assign an empty string
        }
        data.push_back(str);
        ++index;
    }

    // token more than expected
    if (urc->atTokHasmore()) {
        logD(RFX_LOG_TAG, "Wrong URC format: params more than limit");
    }

    if (appendPhoneId) {
        data.push_back((char*)std::to_string(m_slot_id).c_str());
    }

    RfxStringsData rfxData((char**)data.data(), (int)data.size());

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id, rfxData);
    responseToTelCore(unsol);
}

void RfxBaseHandler::notifyIntsDataToTcl(
            const sp<RfxMclMessage>& msg, int rfxMsg, int maxLen, bool appendPhoneId) {
    int err;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) return;

    std::vector<int> params;

    int index = 0;
    while(maxLen == 0 || index < maxLen) {
        int val = line->atTokNextint(&err);
        if (err < 0) break;
        params.push_back(val);
        ++index;
    }

    if (appendPhoneId) {
        params.push_back(m_slot_id);
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
        rfxMsg, m_slot_id, RfxIntsData(params.data(), (int)params.size()));
    responseToTelCore(urc);
}

RIL_Errno RfxBaseHandler::handleCmdWithVoidResponse(
                                       const sp<RfxMclMessage>& msg, String8 cmd) {
    sp<RfxAtResponse> atResponse = atSendCommand(cmd);
    // check at cmd result, consider default as success
    RIL_Errno result = RIL_E_SUCCESS;
    if (atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
        // CC: Special VTS handling for normal call ATD when no SIM (ALPS03640532)
        // Call fail cause will be CM_CALL_REJECTED (21)
        if (atResponse->atGetCmeError() == CME_OPERATION_NOT_ALLOWED_ERR) {
            result = RIL_E_OPERATION_NOT_ALLOWED;
        } else {
            result = RIL_E_INTERNAL_ERR;
        }
        logE(RFX_LOG_TAG, "%d failed!", msg->getId());
    }

    // report to tcl
    sp<RfxMclMessage> mclResponse =
        RfxMclMessage::obtainResponse(msg->getId(), result, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
    return result;
}

unsigned int RfxBaseHandler::getSlotId() {
    return m_slot_id;
}
