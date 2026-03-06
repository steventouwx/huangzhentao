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

#include "RmmCapabilitySwitchURCHandler.h"
#include "RmmCapabilitySwitchUtil.h"
#include "RmmCapabilitySwitchRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxMessageId.h"
#include "RfxRadioCapabilityData.h"
#include "rfx_properties.h"

#define RFX_LOG_TAG "RmmCapa"

RFX_IMPLEMENT_HANDLER_CLASS(RmmCapabilitySwitchURCHandler, RIL_CMD_PROXY_URC);

Mutex RmmCapabilitySwitchURCHandler::s_switch_urc_channel_mutex;
int RmmCapabilitySwitchURCHandler::s_urc_count = 0;

RmmCapabilitySwitchURCHandler::RmmCapabilitySwitchURCHandler(int slot_id, int channel_id):
    RfxBaseHandler(slot_id, channel_id) {
    logI(RFX_LOG_TAG, "constructor slot_id = %d, channel_id = %d", slot_id, channel_id);
    const int mipc_urc[] = {
        MIPC_NW_RADIO_CAPABILITY_IND,
        MIPC_NW_RADIO_IND,
    };
    registerToHandleMipcURC(mipc_urc, sizeof(mipc_urc)/sizeof(int));
}

RmmCapabilitySwitchURCHandler::~RmmCapabilitySwitchURCHandler() {
}

void RmmCapabilitySwitchURCHandler::onHandleUrc(const sp<RfxMclMessage> &msg) {
    if (msg->getMipcData() == NULL) {
        logE(RFX_LOG_TAG, "empty urc");
        return;
    }
    uint16_t urc = msg->getMipcData()->getMipcMsgId();
    int slot_id = msg->getSlotId();
    uint32_t capability = 0;
    uint8_t radio_state = 0;
    int cap_state = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE);
    switch (urc) {
        case MIPC_NW_RADIO_CAPABILITY_IND:
            capability = msg->getMipcData()->getMipcUint32Val(
                    MIPC_NW_RADIO_CAPABILITY_IND_T_RADIO_CAPABILITY, 0);
            logI(RFX_LOG_TAG, "MIPC_NW_RADIO_CAPABILITY_IND capability[%d]: 0x%X.", slot_id,
                    capability);
            if ((capability & 16) != 0) {   // Has C2K capability
                char* slot_str = NULL;
                asprintf(&slot_str, "%d", slot_id + 1);
                if (slot_str != NULL) {
                    rfx_property_set("persist.vendor.radio.c_capability_slot", slot_str);
                    free(slot_str);
                }
            }
            break;

        case MIPC_NW_RADIO_IND:
            {
                radio_state = msg->getMipcData()->getMipcUint8Val(
                        MIPC_NW_RADIO_IND_T_SW_STATE, 0);
                if (cap_state == CAPABILITY_SWITCH_STATE_START) {
                    // send radio off URC
                    logD(RFX_LOG_TAG, "MIPC_NW_RADIO_IND, set radio state: %d", radio_state);
                    if (radio_state) {
                        getMclStatusManager(slot_id)->setIntValue(
                                RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_ON);
                    } else {
                        getMclStatusManager(slot_id)->setIntValue(
                                RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
                    }
                }
                break;
            }

        default:
            logI(RFX_LOG_TAG, "Not support the urc id %d!", urc);
            break;
    }
}

void RmmCapabilitySwitchURCHandler::handleSimSwitchUrc(const sp<RfxMclMessage>& msg, int version) {
    RFX_UNUSED(msg);
    RFX_UNUSED(version);
    /*
    int err;
    int new_major_slot;
    int old_major_slot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    //parse urc info
    logI(RFX_LOG_TAG, "handleSimSwitchUrc begin, old_major_slot=%d, urc_count=%d",
         old_major_slot, s_urc_count);
    RfxAtLine* urc_line = msg->getRawUrc();
    urc_line->atTokStart(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleSimSwitchUrc err %d", err);
        return;
    }
    new_major_slot = urc_line->atTokNextint(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleSimSwitchUrc first int err %d %d", err, new_major_slot);
        return;
    }
    if (version == 0) {
        int i;
        for (i = 0; new_major_slot > 1; i++) {
            new_major_slot = new_major_slot >> 1;
        }
        new_major_slot = i;
    } else {
        new_major_slot = urc_line->atTokNextint(&err) - 1;
        if (err < 0) {
            logE(RFX_LOG_TAG, "handleSimSwitchUrc second int err %d %d", err, new_major_slot);
            return;
        }
    }
    s_switch_urc_channel_mutex.lock();
    if (checkIfSwitch()) {
        logI(RFX_LOG_TAG, "switch urc channels");
        RmmCapabilitySwitchRequestHandler::setSIMInitState(0);
        for(int i = RIL_URC; i < RfxChannelManager::getSupportChannels(); i+=RIL_CHANNEL_OFFSET) {
            lockReaderMutex(i);
        }
        RmmCapabilitySwitchRequestHandler::switchChannel(RIL_URC, old_major_slot, new_major_slot);
        getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CAPABILITY_SWITCH_URC_CHANNEL,
                                                   false);
        for(int i = RIL_CMD_11; i < RfxChannelManager::getSupportChannels();
                i += RIL_CHANNEL_OFFSET) {
            RmmCapabilitySwitchRequestHandler::lockRestartMutex(i);
        }
        RmmCapabilitySwitchRequestHandler::switchChannel(RIL_CMD_11, old_major_slot,
                new_major_slot);
        for(int i = RIL_CMD_11; i < RfxChannelManager::getSupportChannels();
                i += RIL_CHANNEL_OFFSET) {
            RmmCapabilitySwitchRequestHandler::unlockRestartMutex(i);
        }
        for(int i = RIL_URC; i < RfxChannelManager::getSupportChannels(); i+=RIL_CHANNEL_OFFSET) {
            unlockReaderMutex(i);
        }
    }
    s_switch_urc_channel_mutex.unlock();
    */
}

void RmmCapabilitySwitchURCHandler::lockReaderMutex(int channel_id) {
    RFX_UNUSED(channel_id);
    /*
    RfxChannel *p_channel;
    RfxChannelContext *p_channel_context;

    p_channel = RfxChannelManager::getChannel(channel_id);
    p_channel_context = p_channel->getContext();
    p_channel_context->m_readerMutex.lock();
    */
}

void RmmCapabilitySwitchURCHandler::unlockReaderMutex(int channel_id) {
    RFX_UNUSED(channel_id);
    /*
    RfxChannel *p_channel;
    RfxChannelContext *p_channel_context;

    p_channel = RfxChannelManager::getChannel(channel_id);
    p_channel_context = p_channel->getContext();
    p_channel_context->m_readerMutex.unlock();
    */
}

bool RmmCapabilitySwitchURCHandler::checkIfSwitch() {
    /*
    bool ret = getNonSlotMclStatusManager()->
            getBoolValue(RFX_STATUS_KEY_CAPABILITY_SWITCH_URC_CHANNEL, false);
    if (ret) {
        s_urc_count++;
        if (RfxRilUtils::rfxGetSimCount() == s_urc_count) {
            s_urc_count = 0;
            return true;
        }
    }
    */
    return false;
}
