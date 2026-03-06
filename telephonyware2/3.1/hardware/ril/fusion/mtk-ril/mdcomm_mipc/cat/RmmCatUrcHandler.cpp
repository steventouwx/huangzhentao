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

#include "RmmCatUrcHandler.h"
#include <libmtkrilutils.h>


#define RFX_LOG_TAG "RmmCatUrcHandler"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmCatUrcHandler, RIL_CMD_PROXY_URC);


RmmCatUrcHandler::RmmCatUrcHandler(int slot_id, int channel_id) :
    RfxBaseHandler(slot_id, channel_id) {
    const int urc[] = {
         MIPC_STK_PAC_IND,
         MIPC_STK_SIM_REFRESH_IND,
         MIPC_STK_BIP_EVENT_NOTIFY_IND
    };
    logI(RFX_LOG_TAG, "RmmCatUrcHandler init slotid %d, ch id %d", slot_id, channel_id);
    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
    /// Enable proactive command by sending MIPC_STK_SET_PAC_REQ. this is special requirement
    /// from STK MIPC. Data card or Colgin platform need this to tell L5 which command they
    /// want to handle. set 0xff to enable or 0x00 to disable. @{
    uint8_t value[32];
    uint8_t len = 32;
    memset(value, 0xFF, len);
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_STK_SET_PAC_REQ, slot_id);
    data->addMipcTlv(MIPC_STK_SET_PAC_REQ_T_PAC_BITMASK_PTR, len, value);
    logD(RFX_LOG_TAG, "RmmCatUrcHandler enable proactive cmd by MIPC_STK_SET_PAC_REQ");
    callToMipcMsgSync(data);
    /// @}
}

RmmCatUrcHandler::~RmmCatUrcHandler() {
}

void RmmCatUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleEvent: msg id: %d", id);
    switch (id) {
        default:
            logD(RFX_LOG_TAG, "onHandleEvent: should not be here");
            break;
    }
}

void RmmCatUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        logD(RFX_LOG_TAG, "onHandleUrc MipcMsgId:%d, MipcSlotId:%d",
            msg->getMipcData()->getMipcMsgId(), msg->getMipcData()->getMipcSlotId());
        uint16_t msgId = msg->getMipcData()->getMipcMsgId();
        switch (msgId) {
            case MIPC_STK_PAC_IND:
                handleStkInd(msg);
                break;

            case MIPC_STK_SIM_REFRESH_IND:
                handleStkSimRefreshInd(msg);
                break;

            case MIPC_STK_BIP_EVENT_NOTIFY_IND:
                handleBipEventNotify(msg);
                break;

            default:
            logE(LOG_TAG, "Should not be here");
            break;
        }
    }
}

void RmmCatUrcHandler::handleStkInd(const sp<RfxMclMessage>& msg) {
    char* payload = NULL;
    uint16_t len = 0;
    RfxMipcData* mipcData = msg->getMipcData();
    uint8_t type = mipcData->getMipcUint8Val(MIPC_STK_PAC_IND_T_PAC_TYPE, 0);
    if(type == 2) {
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_STK_SESSION_END, m_slot_id,
                       RfxVoidData());
        responseToTelCore(urc);
        return;
    }

    payload = (char *)mipcData->getMipcVal(MIPC_STK_PAC_IND_T_PAC, &len);
    char* payload_string = (char*)calloc(1, sizeof(char)*(len + 1));
    RFX_ASSERT(payload_string != NULL);
    memset(payload_string, 0x0, len + 1);
    memcpy(payload_string, payload, len);
    logD(RFX_LOG_TAG, "handleStkInd: %s, %d", payload_string, len);

    int urcType = 0;
    if(type == 0) {
        urcType = RFX_MSG_URC_STK_PROACTIVE_COMMAND;
    } else if (type ==1) {
        urcType = RFX_MSG_URC_STK_EVENT_NOTIFY;
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(urcType, m_slot_id,
                RfxStringData(payload_string));
    responseToTelCore(urc);
    free(payload_string);
}
void RmmCatUrcHandler::handleBipEventNotify(const sp<RfxMclMessage>& msg) {
    char *cmd = NULL;
    uint16_t  len = 0;
    RfxMipcData* mipcData = msg->getMipcData();
    cmd = (char *)mipcData->getMipcVal(MIPC_STK_BIP_EVENT_NOTIFY_IND_T_CMD_DATA,&len);
    char* cmd_string = (char*)calloc(1, sizeof(char)*(len + 1));
    RFX_ASSERT(cmd_string != NULL);
    memset(cmd_string, 0x0, len + 1);
    memcpy(cmd_string, cmd, len);

    logD(RFX_LOG_TAG, "handleBipEventNotify: %s", cmd_string);
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_STK_EVENT_NOTIFY, m_slot_id,
            RfxStringData(cmd_string));
    responseToTelCore(urc);
    free(cmd_string);
}

void RmmCatUrcHandler::onHandleTimer() {
}

void RmmCatUrcHandler::handleStkSimRefreshInd(const sp<RfxMclMessage>& msg) {
    RIL_SimRefreshResponse_v7 response;
    RfxMipcData* mipcData = msg->getMipcData();

    uint8_t refreshType = mipcData->getMipcUint8Val(MIPC_STK_SIM_REFRESH_IND_T_SIM_REFRESH_RESULT, 0);
    uint32_t efId = mipcData->getMipcUint32Val(MIPC_STK_SIM_REFRESH_IND_T_EF_ID, 0);
    uint16_t len = 0;
    char* aid = (char *)mipcData->getMipcVal(MIPC_STK_SIM_REFRESH_IND_T_AID, &len);
    char* aid_string = (char*)calloc(1, sizeof(char)*(len + 1));
    RFX_ASSERT(aid_string != NULL);
    memcpy(aid_string, aid, len);
    aid_string[len] = '\0';

    logD(RFX_LOG_TAG, "MIPC_STK_SIM_REFRESH_IND type=%d efId=%d aid=%s",refreshType, efId, aid_string);
    memset(&response, 0, sizeof(RIL_SimRefreshResponse_v7));
    response.aid = aid_string;
    response.ef_id = (int)efId;   // default EFID
    response.result = (RIL_SimRefreshResult)refreshType;

    if(!isAllowNotifyCustomizationSimRefreshTypeConfigured()
            && (response.result == SIM_INIT_FILE_CHANGE
                || response.result ==SIM_INIT_FULL_FILE_CHANGE)) {
        response.result = SIM_INIT;
    }

    //Todo: Add the data structure for RIL_SimRefreshResponse_v7
    sp<RfxMclMessage> refresh_urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_REFRESH, m_slot_id,
    RfxSimRefreshData((void*)&response, sizeof(RIL_SimRefreshResponse_v7)));
    responseToTelCore(refresh_urc);
    free(aid_string);
}

bool RmmCatUrcHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg, RIL_RadioState radioState) {
    uint16_t msgId = msg->getMipcData()->getMipcMsgId();
    bool reject = false;
    if (RADIO_STATE_UNAVAILABLE == radioState) {
        if (msgId == MIPC_STK_PAC_IND || msgId == MIPC_STK_SIM_REFRESH_IND) {
            reject = false;
        } else {
            reject = true;
        }
    }
    logD(RFX_LOG_TAG, "onCheckIfRejectMessage: %d %d", radioState, reject);
    return reject;
}
