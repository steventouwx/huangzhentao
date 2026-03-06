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

#include "RmmImsRttControlUrcHandler.h"
#include <telephony/mtk_ril.h>
#include <string>
#include "rfx_properties.h"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmImsRttControlUrcHandler, RIL_CMD_PROXY_URC);

// register data
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RTT_MODIFY_RESPONSE);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_RTT_TEXT_RECEIVE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RTT_CAPABILITY_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RTT_MODIFY_REQUEST_RECEIVE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_AUDIO_INDICATION);

RmmImsRttControlUrcHandler::RmmImsRttControlUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmmImsRttControlUrcHandler constructor");

    const int urc[] = {
        MIPC_CALL_RTT_AUDIO_IND,                 // +EIMSAUDIOSID
        MIPC_CALL_RTT_CAPABILITY_IND,            // +EIMSTCAP
        MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND,    // +PRTTCALL
        MIPC_CALL_RTT_TEXT_RECEIVE_IND           // +ERTTSTR
    };

    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
}

RmmImsRttControlUrcHandler::~RmmImsRttControlUrcHandler() {
}

void RmmImsRttControlUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    uint16_t mipcMsgId = (int) msg->getMipcData()->getMipcMsgId();
    logD(RFX_LOG_TAG, "onHandleRttUrc: mipc message %04x", mipcMsgId);
    if (mipcMsgId == MIPC_CALL_RTT_TEXT_RECEIVE_IND ) {
        onRttTextReceive(msg);
    } else if (mipcMsgId == MIPC_CALL_RTT_CAPABILITY_IND ) {
        onRttCapabilityIndication(msg);
    } else if (mipcMsgId == MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND ) {
        onRttModifyRequestReceive(msg);
    } else if (mipcMsgId == MIPC_CALL_RTT_AUDIO_IND ) {
        onAudioIndication(msg);
    }
}

void RmmImsRttControlUrcHandler::onRttTextReceive(const sp<RfxMclMessage>& msg) {

    // +ERTTSTR= =<callid>, <len>, <text>
    // const int maxLen = 3;
    RfxMipcData* urc_data = msg->getMipcData();
    uint32_t call_id = (uint32_t)(urc_data->getMipcUint32Val(MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_CALL_ID, 0));
    uint32_t length = (uint32_t)(urc_data->getMipcUint8Val(MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_LEN, 0));
    char *text = (char*)(urc_data->getMipcStr(MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_TEXT));

    String8 callIdStr = String8::format("%d", call_id);
    String8 textLenStr = String8::format("%d", length);
    const char *urcData[] = {callIdStr, textLenStr, text};

    logD(RFX_LOG_TAG, "onRttTextReceive: + ERTTSTR:%d, %d, %s", call_id, length, text);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_RTT_TEXT_RECEIVE, m_slot_id,
               RfxStringsData((char **)urcData, sizeof(urcData) / sizeof(const char*)));
    responseToTelCore(unsol);
}

void RmmImsRttControlUrcHandler::onRttCapabilityIndication(const sp<RfxMclMessage>& msg) {
    // +EIMSTCAP:<call id>,<local text capability>,<remote text capability>
    // <local text status>,<real remote text capability>

    const int maxLen = 5;
    int rfxMsg = RFX_MSG_UNSOL_RTT_CAPABILITY_INDICATION;

    int responseStr[maxLen];
    RfxMipcData* urc_data = msg->getMipcData();
    int call_id = urc_data->getMipcUint32Val(MIPC_CALL_RTT_CAPABILITY_IND_T_CALL_ID, 0);
    int local_cap = urc_data->getMipcUint8Val(MIPC_CALL_RTT_CAPABILITY_IND_T_LOCAL_TEXT_CAPABILITY, 0);
    int remote_cap = urc_data->getMipcUint8Val(MIPC_CALL_RTT_CAPABILITY_IND_T_REMOTE_TEXT_CAPABILITY, 0);
    int local_status = urc_data->getMipcUint8Val(MIPC_CALL_RTT_CAPABILITY_IND_T_LOCAL_TEXT_STATUS, 0);
    int remote_status = urc_data->getMipcUint8Val(MIPC_CALL_RTT_CAPABILITY_IND_T_REAL_REMOTE_TEXT_CAPABILITY, 0);

    responseStr[0] = call_id;
    responseStr[1] = local_cap;
    responseStr[2] = remote_cap;
    responseStr[3] = local_status;
    responseStr[4] = remote_status;
    logD(RFX_LOG_TAG, "onRttCapabilityIndication: + EIMSTCAP:%d, %d, %d, %d, %d", responseStr[0], responseStr[1], responseStr[2], responseStr[3], responseStr[4]);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(responseStr, maxLen));
    responseToTelCore(unsol);
}

void RmmImsRttControlUrcHandler::onRttModifyRequestReceive(const sp<RfxMclMessage>& msg) {
    //+PRTTCALL= <call id>, <op>
    const int maxLen = 2;
    int rfxMsg = RFX_MSG_UNSOL_RTT_MODIFY_REQUEST_RECEIVE;
    int responseStr[maxLen];
    RfxMipcData* urc_data = msg->getMipcData();
    int call_id = urc_data->getMipcUint32Val(MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND_T_CALL_ID, 0);
    int op = urc_data->getMipcUint8Val(MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND_T_OP, 0);

    responseStr[0] = call_id;
    responseStr[1] = op;

    logD(RFX_LOG_TAG, "onRttModifyRequestReceive: + PRTTCALL:%d, %d", call_id, op);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(responseStr, maxLen));
    responseToTelCore(unsol);
}

void RmmImsRttControlUrcHandler::onAudioIndication(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSAUDIOSID:<call id>, <audio>
     * <call id>: call id
     * <audio>: 0:silence; 1:speech;
     */

    const int maxLen = 2;
    int rfxMsg = RFX_MSG_UNSOL_AUDIO_INDICATION;

    int responseStr[maxLen];
    RfxMipcData* urc_data = msg->getMipcData();
    int call_id = urc_data->getMipcUint32Val(MIPC_CALL_RTT_AUDIO_IND_T_CALL_ID, 0);
    int audio = urc_data->getMipcUint8Val(MIPC_CALL_RTT_AUDIO_IND_T_TYPE, 0);

    responseStr[0] = call_id;
    responseStr[1] = audio;

    logD(RFX_LOG_TAG, "onAudioIndication: +EIMSAUDIOSID:%d, %d", call_id, audio);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(responseStr, maxLen));
    responseToTelCore(unsol);
}
