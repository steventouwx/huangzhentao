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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxMessageId.h"
#include "RmmCommSmsRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxStatusDefs.h"
#include "RfxVoidData.h"

using ::android::String8;

static const int event_list[] = {
    RFX_MSG_EVENT_SMS_NOTIFY_SMS_READY,
};

// Register handler
RFX_IMPLEMENT_HANDLER_CLASS(RmmCommSmsRequestHandler, RIL_CMD_PROXY_1);

// Register unsolicited message
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SMS_READY_NOTIFICATION);

// Register event
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SMS_NOTIFY_SMS_READY);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmmCommSmsRequestHandler::RmmCommSmsRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
        setTag(String8("RmmCommSmsReq"));
        registerToHandleEvent(event_list, sizeof(event_list)/sizeof(int));
}

void RmmCommSmsRequestHandler::onHandleEvent(const sp < RfxMclMessage > & msg) {
    int event = msg->getId();

    switch(event) {
        case RFX_MSG_EVENT_SMS_NOTIFY_SMS_READY:
            notifySmsReady(msg);
            break;
        default:
            logE(mTag, "Not support the event!");
            break;
    }
}

void RmmCommSmsRequestHandler::notifySmsReady(const sp<RfxMclMessage>& msg) {
    logD(mTag, "notifySmsReady");

    RFX_UNUSED(msg);
    /*
     * If modem turns off, RILD should not send any AT command to modem.
     * If RILD sends the at commands to Modem(off), it will block the
     * MUXD/CCCI channels
     */
    if (getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false)) {
        logD(mTag, "Modem turns off");
        return;
    }

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SMS_READY_NOTIFICATION,
            m_slot_id, RfxVoidData());
    responseToTelCore(unsol);

    sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_CFG_REQ, msg->getSlotId());
    // To set PDU mode must be when SMS task is ready in modem.
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_PREFER_ACK, MIPC_SMS_ACK_NEEDED);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_PREFER_STORAGE, MIPC_SMS_STORAGE_TE);

    callToMipcMsgSync(initData);

    checkSMSC();
}

void RmmCommSmsRequestHandler::checkSMSC(void) {
    String8 smscName("vendor.ril.no.smsc");
    smscName.append(String8::format(".%d", (m_slot_id + 1)));
    rfx_property_set(smscName, "0");

    // check card status
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        logE(mTag, "SIM card was removed!");
        return;
    }

    sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_CFG_REQ, m_slot_id);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_GET_SCA, 1);
    sp<RfxMipcData> result = callToMipcMsgSync(initData);

    char *sca = NULL;
    if (result != NULL) {
        int cause = result->getResult();
        if (cause == MIPC_RESULT_SUCCESS) {
            sca = (char *)result->getMipcVal(MIPC_SMS_CFG_CNF_T_SCA, NULL);
        }
    }

    if (sca == NULL || strcmp(sca, "") == 0 || strcmp(sca, "7") == 0
            || strcmp(sca, "+7") == 0) {
        logD(mTag, "set no smsc property to 1");
        rfx_property_set(smscName, "1");
    }
}
