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

#include "RmmSuppServUrcEntryHandler.h"
//#include "GsmUtil.h"
#include "SSUtil.h"
//#include "SSConfig.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxRilUtils.h"
#include "RfxSuppServNotificationData.h"
#include "mipc_msg_tlv_const.h"


RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_ON_USSD);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_CALL_FORWARDING);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_ON_USSI);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_ON_XUI);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ON_VOLTE_SUBSCRIPTION);

RFX_IMPLEMENT_HANDLER_CLASS(RmmSuppServUrcEntryHandler, RIL_CMD_PROXY_URC);


RmmSuppServUrcEntryHandler::RmmSuppServUrcEntryHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int urc[] = {
        MIPC_SS_USSD_IND,
        MIPC_SS_CFU_IND,
        MIPC_SS_XCAP_RCN_IND,
        MIPC_SS_IMS_XUI_IND
    };

    registerToHandleMipcURC(urc, sizeof(urc) / sizeof(int));
}

RmmSuppServUrcEntryHandler::~RmmSuppServUrcEntryHandler() {
}

void RmmSuppServUrcEntryHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        logE(TAG, "[%s] %d getMipcData() return null", __FUNCTION__, msg->getId());

        return;
    }

    logD(TAG, "[%s] mipcMsgId:%d [%d]",
            __FUNCTION__,  mipcData->getMipcMsgId(), mipcData->getMipcSlotId());

    switch (mipcData->getMipcMsgId()) {
        case MIPC_SS_USSD_IND:
            handleUssdInd(msg);
            break;

        case MIPC_SS_CFU_IND:
            handleCfuNotify(msg);
            break;

        case MIPC_SS_XCAP_RCN_IND:
            handleXcapResponseCodeNotify(msg);
            break;
        case MIPC_SS_IMS_XUI_IND:
            handleOnXui(msg);
            break;

        default:
            break;
        }
}

void RmmSuppServUrcEntryHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
}

void RmmSuppServUrcEntryHandler::handleUssdInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    // mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();

    /* mipc_ss_ussd_const_enum */
    int mode = 0;
    int payloadLen = 0;
    uint16_t len = 0;
    char *finalData[2];
    char *payload = NULL;

    sp<RfxMclMessage> urc;

    /* Initialize finalData */
    for(int i = 0; i < 2; i++) {
        finalData[i] = NULL;
    }

    /*
     * <m>:
     * 0   no further user action required (network initiated USSD Notify, or no further information needed after mobile initiated operation)
     * 1   further user action required (network initiated USSD Request, or further information needed after mobile initiated operation)
     * 2   USSD terminated by network
     * 3   other local client has responded
     * 4   operation not supported
     * 5   network time out
     */
    mode = (int) mipcData->getMipcUint8Val(MIPC_SS_USSD_IND_T_USSD_RESPONSE, 0);

    if (mode < 0 || mode > 5) {
        mode = MIPC_SS_USSD_NO_ACTION_REQUIRED;
        logE(TAG, "[%s] set default mode", __FUNCTION__);
    }

    /**
     * According the USSD response format: "+CUSD: <m>[,<str>,<dcs>]",
     * the number of stings only could be 1 or 3. And we don't need to return dcs back
     * to framework if number of strings equal to 3.
     */
    asprintf(&finalData[0], "%d", mode);

    payloadLen = (int) mipcData->getMipcUint8Val(MIPC_SS_USSD_IND_T_PAYLOAD_LEN, 0);

    if (payloadLen > 0) {
        payload = (char *) mipcData->getMipcVal(MIPC_SS_USSD_IND_T_PAYLOAD, &len);

        finalData[1] = (char *) alloca((payloadLen + 1 ) * sizeof(char));
        if (finalData[1] != NULL) {
            memset(finalData[1], 0, (payloadLen + 1 ) * sizeof(char));
            memcpy(finalData[1], payload, payloadLen);
        } else {
            logE(TAG, "[%s] alloca failed!", __FUNCTION__);
        }
    }

    logD(TAG, "[%s] mode:%d, content:%s", __FUNCTION__, (int) mode,
            (finalData[1] == NULL ? "null" : finalData[1]));

    // change to notify mode, or else it'll be treated as error.
    if (mode == MIPC_SS_USSD_TERMINATED_BY_NW && finalData[1] != NULL) {
        finalData[0] = strdup("0");
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ON_USSD, m_slot_id, RfxStringsData(finalData, 2));
    responseToTelCore(urc);

    if (finalData[0] != NULL) {
        free(finalData[0]);
    }
}

void RmmSuppServUrcEntryHandler::handleCfuNotify(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int response[2] = {0}; /* status, line */

    sp<RfxMclMessage> urc;

    response[0] = (int) mipcData->getMipcUint8Val(MIPC_SS_CFU_IND_T_STATUS, 0);
    response[1] = (int) mipcData->getMipcUint8Val(MIPC_SS_CFU_IND_T_LINE, 0);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CALL_FORWARDING,
            m_slot_id, RfxIntsData(response, 2));
    responseToTelCore(urc);
    logD(TAG, "[%s] Send RIL_UNSOL_CALL_FORWARDING", __FUNCTION__);
}

/*
 * XUI information.
 +EIMSXUI: <account_id>, <broadcast_flag>,<xui_info>
 < account_id >: account number 0~7
 < broadcast_flag >: broadcast flag 0~1
 < xui_info >: Xui information
 */
void RmmSuppServUrcEntryHandler::handleOnXui(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> urc;

    char* p_data[4];
    int tmp = 0;

    tmp = (int) mipcData->getMipcUint8Val(MIPC_SS_IMS_XUI_IND_T_ACCOUNT_ID, 0);
    asprintf(&p_data[0], "%d", tmp);

    tmp = (int) mipcData->getMipcUint8Val(MIPC_SS_IMS_XUI_IND_T_BROADCAST_FLAG, 0);
    asprintf(&p_data[1], "%d", tmp);

    p_data[2] = mipcData->getMipcStr(MIPC_SS_IMS_XUI_IND_T_XUI_INFO);

    asprintf(&p_data[3], "%d", m_slot_id);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ON_XUI, m_slot_id, RfxStringsData(p_data, 4));
    responseToTelCore(urc);

    free(p_data[0]);
    free(p_data[1]);
    free(p_data[3]);

    logD(TAG, "[%s] Send RFX_MSG_UNSOL_ON_XUI", __FUNCTION__);
}

/*
 * XCAP Response Code Notify.
 * Modem SSDS module reports the HTTP reponse code from XCAP server when everytime
 * SSDS receive the response from utinterface. According to this information,
 * AP can recognize if current SIM card is a VoLTE subscriber or not.
 *
 * +EXCAPRCN: <code>
 * <code>:  response code from utinterface (HTTP response code, SS error code)
 * VoLTE card, return 1
 * Non VoLTE card, return 2
 * Unknown 0
 */
void RmmSuppServUrcEntryHandler::handleXcapResponseCodeNotify(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> urc;

    int code = 0;
    int response = 0;

    code = (int) mipcData->getMipcUint32Val(MIPC_SS_XCAP_RCN_IND_T_CODE, 0);
    response = (int) mipcData->getMipcUint8Val(MIPC_SS_XCAP_RCN_IND_T_RESPONSE, 0);
    logD(TAG, "[%s] code:%d, response:%d", __FUNCTION__, response, code, response);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ON_VOLTE_SUBSCRIPTION, m_slot_id,
            RfxIntsData(&response, 1));
    responseToTelCore(urc);
}

