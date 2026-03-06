/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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

// Std include
#include <string>
#include <vector>
#include <arpa/inet.h>

// MTK Fusion Fwk include
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"

// MWI Local include
#include "RmmMobileWifiRequestHandler.h"
#include "RmmMobileWifiUrcHandler.h"

#define RFX_LOG_TAG "RmmMwi"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmMobileWifiUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_WIFI_PDN_ERROR);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_REQUEST_GEO_LOCATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_WIFI_PDN_OOS);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_SSAC_STATUS);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_WFC_PDN_STATE);

static const int ind[] = {
    MIPC_WFC_WIFI_PDN_COUNT_IND,
    MIPC_WFC_PDN_HO_IND,
    MIPC_WFC_ROVE_OUT_IND,
    MIPC_WFC_SSAC_IND,
    MIPC_WFC_WIFI_PDN_ERR_IND,
    MIPC_WFC_WIFI_PDN_OOS_IND,
    MIPC_WFC_WFC_IND,
    MIPC_SYS_GEO_LOCATION_IND,
};

static const int cmd[] = {
    MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD,
};

RmmMobileWifiUrcHandler::RmmMobileWifiUrcHandler(
    int slot_id, int channel_id) : RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmmMobileWifiUrcHandler:%d", slot_id);
    registerToHandleMipcURC(ind, sizeof(ind) / sizeof(int));
    registerToHandleMipcCmd(cmd, sizeof(cmd) / sizeof(int));
}

RmmMobileWifiUrcHandler::~RmmMobileWifiUrcHandler() {
}

void RmmMobileWifiUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    uint16_t msgId = msg->getMipcData()->getMipcMsgId();
    //logD(RFX_LOG_TAG, "[onHandleUrc]%d", msgId);

    switch (msgId) {
        case MIPC_WFC_ROVE_OUT_IND:
            onNotifyWfcWifiRoveout(msg);
            break;
        case MIPC_WFC_PDN_HO_IND:
            onNotifyPdnHandover(msg);
            break;
        case MIPC_WFC_WIFI_PDN_COUNT_IND:
            onNotifyActiveWifiPdnCount(msg);
            break;
        case MIPC_WFC_WIFI_PDN_ERR_IND:
            onNotifyWifiPdnError(msg);
            break;
        case MIPC_SYS_GEO_LOCATION_IND:
            onNotifyGeoRequest(msg);
            break;
        case MIPC_WFC_WIFI_PDN_OOS_IND:
            onNotifyWifiPdnOOS(msg);
            break;
        case MIPC_WFC_SSAC_IND:
            onNotifySsacStatus(msg);
            break;
        case MIPC_WFC_WFC_IND:
            onNotifyWifiPdnStatusChanged(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "onHandleUrc: no handle");
            break;
    }
}

void RmmMobileWifiUrcHandler::onHandleCmd(const sp<RfxMclMessage>& msg) {
    uint16_t msgId = msg->getMipcData()->getMipcMsgId();
    logD(RFX_LOG_TAG, "[onHandleCmd]%d", msgId);

     switch (msgId) {
        case MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD:
            onNotifyWifiRssiMonitoringConfig(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "onHandleCmd: no handle");
            break;
    }
}

void RmmMobileWifiUrcHandler::onHandleTimer() {
    // do something
}

void RmmMobileWifiUrcHandler::onNotifyWifiPdnStatusChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData* urcData = msg->getMipcData();
    uint8_t state = urcData->getMipcUint8Val(MIPC_WFC_WFC_IND_T_WIFI_IMS_PDN_STATE, MIPC_WFC_WIFI_PDN_STATE_INVALID);
    char* ifname = urcData->getMipcStr(MIPC_WFC_WFC_IND_T_IFNAME);
    vector<int> params;
    params.push_back(state);
    params.push_back(m_slot_id);

    if (state == MIPC_WFC_WIFI_PDN_STATE_NOT_REGISTERED_WITH_LOCK && wpfaFlashIpRoute != NULL && ifname != NULL)
        wpfaFlashIpRoute(ifname);

    logD(RFX_LOG_TAG, "onNotifyWifiPdnStatusChanged state:%d simIdx:%d", params.at(0), params.at(1));
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_WFC_PDN_STATE, m_slot_id,
        RfxIntsData(params.data(), (int)params.size()));
    responseToTelCore(unsol);
}

void RmmMobileWifiUrcHandler::onNotifyWfcWifiRoveout(const sp<RfxMclMessage>& msg) {
    /*
     * +EWFCRVOUT: <ifname>,<rvout>,<mobike_ind>
     * <ifname>: "wlan0"
     * <rvout>: 1 = Wifi roveout, no LTE for H.O.; 0 = Wifi RSSI acceptable
     * <mobike_ind>: Mobike trigger indication. 0 = not trigger mobike, 1= trigger mobike
     */
    const int maxLen = 3;
    char* response[maxLen];
    int rfxMsg = RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT;

    RfxMipcData* urcData = msg->getMipcData();
    char* ifname = urcData->getMipcStr(MIPC_WFC_ROVE_OUT_IND_T_IFNAME);
    int roveOut = (int)urcData->getMipcUint8Val(MIPC_WFC_ROVE_OUT_IND_T_RVOUT, MIPC_BOOLEAN_FALSE);
    int mobikeInd = (int)urcData->getMipcUint8Val(MIPC_WFC_ROVE_OUT_IND_T_MOBIKE_IND, MIPC_BOOLEAN_FALSE);

    response[0] = ifname;
    asprintf(&response[1], "%d", roveOut);
    asprintf(&response[2], "%d", mobikeInd);
    logD(RFX_LOG_TAG, "onNotifyWifiPdnOOS +EWFCRVOUT:%s,%s,%s",
            response[0], response[1], response[2]);

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxStringsData(response, maxLen));
    responseToTelCore(unsol);

    free(response[1]);
    free(response[2]);
}

void RmmMobileWifiUrcHandler::onNotifyPdnHandover(const sp<RfxMclMessage>& msg) {
    /*
     * +EPDNHANDOVER: <pdn_type>,<status>,<src_rat>,<des_rat>
     * <pdn_type>: 0 = IMS, 1 = SMS, 2 = MMS
     * <status>: 0 = start, 1 = success, -1 = failed
     * <src_rat>: source rat, 1 = LTE, 2 = WIFI
     * <des_rat>: destination rat, 1 = LTE, 2 = WIFI
     * <phoneId>: add by MCL
     */
    const int maxLen = 5;
    int response[maxLen] = {0};
    int rfxMsg = RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER;

    RfxMipcData* urcData = msg->getMipcData();
    response[0] = urcData->getMipcUint8Val(MIPC_WFC_PDN_HO_IND_T_PDN_TYPE, MIPC_WFC_PDN_TYPE_IMS);
    response[1] = urcData->getMipcUint8Val(MIPC_WFC_PDN_HO_IND_T_STATUS, MIPC_WFC_PDN_HO_STATUS_FAIL);
    if (response[1] == MIPC_WFC_PDN_HO_STATUS_FAIL) {
        //mapping to WifiOffloadManager handover status code.
        response[1] = -1;
    }
    response[2] = urcData->getMipcUint8Val(MIPC_WFC_PDN_HO_IND_T_SRC_RAT, MIPC_WFC_RAT_INVALID);
    response[3] = urcData->getMipcUint8Val(MIPC_WFC_PDN_HO_IND_T_DST_RAT, MIPC_WFC_RAT_INVALID);
    response[4] = m_slot_id;

    logD(RFX_LOG_TAG, "onNotifyPdnHandover +EPDNHANDOVER:%d,%d,%d,%d",
            response[0], response[1], response[2], response[3]);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(response, maxLen));
    responseToTelCore(unsol);
}

void RmmMobileWifiUrcHandler::onNotifyActiveWifiPdnCount(const sp<RfxMclMessage>& msg) {
    /*
     * +EWIFIPDNACT: <cnt>
     * <cnt>: Wifi PDN count
     * <phoneId>: add by MCL
     */
    const int maxLen = 2;
    int response[maxLen] = {0};
    int rfxMsg = RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT;

    RfxMipcData* urcData = msg->getMipcData();
    response[0] = urcData->getMipcUint8Val(MIPC_WFC_WIFI_PDN_COUNT_IND_T_COUNT, 0);
    response[1] = m_slot_id;

    logD(RFX_LOG_TAG, "onNotifyActiveWifiPdnCount +EWIFIPDNACT:%d", response[0]);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(response, maxLen));
    responseToTelCore(unsol);
}

void RmmMobileWifiUrcHandler::onNotifyWifiRssiMonitoringConfig(const sp<RfxMclMessage>& msg) {
    /*
     * +EWIFIRSSITHRCFG: <en>,<thr_cnt>,<rssi_thr_1>,< rssi_thr_2>,...
     * <en>: enabled, 0 = disable; 1 = enabled
     * <thr_cnt>: threshold count
     * <rssi_thr_1>,< rssi_thr_2>,...: threshold
     * <phoneId>: add by MCL
     */
    RfxMipcData* urcData = msg->getMipcData();
    uint8_t enable = urcData->getMipcUint8Val(MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD_T_ENABLE_NTF, MIPC_BOOLEAN_FALSE);

    uint16_t count = 0;
    mipc_msg_t* data = urcData->getData();
    count = mipc_msg_get_tlv_num(data, MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD_T_RSSI_THRESHOLD);
    logD(RFX_LOG_TAG, "onNotifyWifiRssiMonitoringConfig enable:%d, count:%d", enable, count);

    if (count > 0) {
        int16_t threshold[count];
        for (uint16_t i = 0; i < count; i++) {
            threshold[i] = urcData->getMipcInt16IdxVal(MIPC_WFC_CELL_SIGNAL_IND_T_VALUE, i, 0);
            //threshold[i] = mipc_wfc_register_wifi_signal_ntf_cmd_get_rssi_threshold(data, 0, i);
            if (UNIT_TEST) {
                logD(RFX_LOG_TAG, "onNotifyWifiRssiMonitoringConfig threshold:%d", threshold[i]);
            }
        }

        const int maxLen = count + 3;
        int response[maxLen];
        int rfxMsg = RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG;
        response[0] = enable;
        response[1] = count;
        for (int j = 2; j < maxLen; j++) {
            response[j] = threshold[j - 2];
        }
        response[maxLen - 1] = m_slot_id;

        sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(response, maxLen));
        responseToTelCore(unsol);

        //send success reponse to L5 to sync state.
        sp<RfxMipcData> mipcResponse = RfxMipcData::obtainMipcData(
                MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_RSP, msg->getSlotId(),
                urcData->getMipcMsgTxId());
        mipcResponse->addMipcTlvUint32(MIPC_T_RESULT, MIPC_RESULT_SUCCESS);
        callToMipcMsgCmdResponse(mipcResponse);
    }
}

void RmmMobileWifiUrcHandler::onNotifyWifiPdnError(const sp<RfxMclMessage>& msg) {
    /*
     * +EWOPDNERR: <cnt>,<cause>,<sub_cause>
     * <cnt>: error cause count
     * <cause>: main cause
     * <sub_cause>: sub cause
     * <phoneId>: add by MCL
     */
    const int maxLen = 5;
    int response[maxLen] = {0};
    int rfxMsg = RFX_MSG_UNSOL_WIFI_PDN_ERROR;

    RfxMipcData* urcData = msg->getMipcData();
    response[1] = urcData->getMipcInt32Val(MIPC_WFC_WIFI_PDN_ERR_IND_T_CAUSE, 0);
    response[2] = urcData->getMipcInt32Val(MIPC_WFC_WIFI_PDN_ERR_IND_T_SUB_CAUSE, 0);
    if (response[1] != 0 || response[2] != 0) {
        response[0] = 1; //error cause count
    }
    response[3] = m_slot_id;
    response[4] = (int)urcData->getMipcUint8Val(MIPC_WFC_WIFI_PDN_ERR_IND_T_IS_LAST_RETRY,
            MIPC_BOOLEAN_FALSE);

    logD(RFX_LOG_TAG, "onNotifyWifiPdnError +EWOPDNERR:%d,%d,%d,%d",
            response[0], response[1], response[2], response[4]);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(response, maxLen));
    responseToTelCore(unsol);
}

void RmmMobileWifiUrcHandler::onNotifyGeoRequest(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSGEO: <account_id>,<broadcast_flag>,<latitude>,<longitude>,<accurate>
     * <account_id>: request id, 0~7
     * <broadcast_flag>: 0, 1
     * <latitude>: latitude from GPS, 0 as failed
     * <longitude>: longitude from GPS, 0 as failed
     * <accurate>: accurate from GPS, 0 as failed
     * <confidence>: confidence which used for transferred accurate
     */
    const int maxLen = 6;
    char* response[maxLen];
    int rfxMsg = RFX_MSG_UNSOL_REQUEST_GEO_LOCATION;

    RfxMipcData* urcData = msg->getMipcData();
    int accountId = (int)urcData->getMipcUint8Val(MIPC_SYS_GEO_LOCATION_IND_T_ACCOUNT_ID, 0);
    int broadcastFlag = (int)urcData->getMipcUint8Val(MIPC_SYS_GEO_LOCATION_IND_T_BROADCAST_FLAG, 0);
    char* latitude = urcData->getMipcStr(MIPC_SYS_GEO_LOCATION_IND_T_LATITUDE);
    char* longitude = urcData->getMipcStr(MIPC_SYS_GEO_LOCATION_IND_T_LONGITUDE);
    char* accurate = urcData->getMipcStr(MIPC_SYS_GEO_LOCATION_IND_T_ACCURACY);
    int confidence = urcData->getMipcUint32Val(MIPC_SYS_GEO_LOCATION_IND_T_CONFIDENCE, 0);
    if (UNIT_TEST) {
        logD(RFX_LOG_TAG, "onNotifyGeoRequest accountId:%d,broadcastFlag:%d,confidence:%d",
                accountId, broadcastFlag, confidence);
    }

    asprintf(&response[0], "%d", accountId);
    asprintf(&response[1], "%d", broadcastFlag);
    response[2] = latitude;
    response[3] = longitude;
    response[4] = accurate;
    asprintf(&response[5], "%d", confidence);

    if (UNIT_TEST) {
        logD(RFX_LOG_TAG, "onNotifyGeoRequest +EIMSGEO:%s,%s,%s,%s,%s,%s",
                response[0], response[1], response[2], response[3], response[4], response[5]);
    } else {
        logD(RFX_LOG_TAG, "onNotifyGeoRequest accountId:%s,accurate:%s,confidence:%s",
                response[0], response[4], response[5]);
    }
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxStringsData(response, maxLen));
    responseToTelCore(unsol);

    free(response[0]);
    free(response[1]);
    free(response[5]);
}

void RmmMobileWifiUrcHandler::onNotifyWifiPdnOOS(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSPDNOOS: <apn>, <cid>, <oos_state>
     * <apn>: apn name
     * <cid>: call id
     * <oos_state>:
     * 0: OOS ended & PDN disconnected
     * 1: OOS started
     * 2: OOS ended & PDN resumed to use
     */
    const int maxLen = 3;
    char* response[maxLen];
    int rfxMsg = RFX_MSG_UNSOL_WIFI_PDN_OOS;

    RfxMipcData* urcData = msg->getMipcData();
    char* apn = urcData->getMipcStr(MIPC_WFC_WIFI_PDN_OOS_IND_T_APN);
    int callId = (int)urcData->getMipcUint8Val(MIPC_WFC_WIFI_PDN_OOS_IND_T_CID, 0);
    int state = (int)urcData->getMipcUint8Val(MIPC_WFC_WIFI_PDN_OOS_IND_T_OOS_STATE, MIPC_WFC_PDN_OOS_STATE_FAIL);

    response[0] = apn;
    asprintf(&response[1], "%d", callId);
    asprintf(&response[2], "%d", state);

    logD(RFX_LOG_TAG, "onNotifyWifiPdnOOS +EIMSPDNOOS:%s,%s,%s",
            response[0], response[1], response[2]);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxStringsData(response, maxLen));
    responseToTelCore(unsol);

    free(response[1]);
    free(response[2]);
}

void RmmMobileWifiUrcHandler::onNotifySsacStatus(const sp<RfxMclMessage>& msg) {
    /*
     * +ESSAC: <BFVoice>,<BFVideo>,<BTVoice>,<BTVideo>
     */
    const int maxLen = 5;
    int response[maxLen] = {0};
    int rfxMsg = RFX_MSG_UNSOL_SSAC_STATUS;

    RfxMipcData* urcData = msg->getMipcData();
    response[0] = urcData->getMipcUint16Val(MIPC_WFC_SSAC_IND_T_BF_VOICE, 0);
    response[1] = urcData->getMipcUint16Val(MIPC_WFC_SSAC_IND_T_BF_VIDEO, 0);
    response[2] = urcData->getMipcUint16Val(MIPC_WFC_SSAC_IND_T_BT_VOICE, 0);
    response[3] = urcData->getMipcUint16Val(MIPC_WFC_SSAC_IND_T_BT_VIDEO, 0);
    response[4] = m_slot_id;

    logD(RFX_LOG_TAG, "onNotifySsacStatus +ESSAC:%d,%d,%d,%d",
            response[0], response[1], response[2], response[3]);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
            RfxIntsData(response, maxLen));
    responseToTelCore(unsol);
}
