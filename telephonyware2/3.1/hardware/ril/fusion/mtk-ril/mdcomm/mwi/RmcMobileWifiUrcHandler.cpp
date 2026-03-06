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
// Std include
#include <vector>
#include <string>

// MTK Fusion Fwk include
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"

// MWI Local include
#include "RmcMobileWifiInterface.h"
#include "RmcMobileWifiUrcHandler.h"
#include "RmcMobileWifiRequestHandler.h"

#define RFX_LOG_TAG "RmcMwi"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcMobileWifiUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_WIFI_PDN_ERROR);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_REQUEST_GEO_LOCATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_NATT_KEEP_ALIVE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_WIFI_PDN_OOS);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_WIFI_LOCK);


static const char* urc[] = {
    URC_WFC_WIFI_ROVEOUT,
    URC_PDN_HANDOVER,
    URC_ACTIVE_WIFI_PDN_COUNT,
    URC_WIFI_RSSI_MONITOR_CONFIG,
    URC_WIFI_PDN_ERROR,
    URC_MD_GEO_REQUEST,
    URC_NATT_KEEP_ALIVE_CHANGED,
    URC_WIFI_PDN_OOS,
    URC_WIFI_LOCK,
};

RmcMobileWifiUrcHandler::RmcMobileWifiUrcHandler(
    int slot_id, int channel_id) : RfxBaseHandler(slot_id, channel_id) {
    registerToHandleURC(urc, sizeof(urc) / sizeof(char *));
}

RmcMobileWifiUrcHandler::~RmcMobileWifiUrcHandler() {
}

void RmcMobileWifiUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char *urc = msg->getRawUrc()->getLine();
    // logD(RFX_LOG_TAG, "[onHandleUrc]%s", urc);
    if (strStartsWith(urc, URC_WFC_WIFI_ROVEOUT)) {
        onNotifyWfcWifiRoveout(msg);
    } else if (strStartsWith(urc, URC_PDN_HANDOVER)) {
        onNotifyPdnHandover(msg);
    } else if (strStartsWith(urc, URC_ACTIVE_WIFI_PDN_COUNT)) {
        onNotifyActiveWifiPdnCount(msg);
    } else if (strStartsWith(urc, URC_WIFI_RSSI_MONITOR_CONFIG)) {
        onNotifyWifiRssiMonitoringConfig(msg);
    } else if (strStartsWith(urc, URC_WIFI_PDN_ERROR)) {
        onNotifyWifiPdnError(msg);
    } else if (strStartsWith(urc, URC_MD_GEO_REQUEST)) {
        onNotifyGeoRequest(msg);
    } else if (strStartsWith(urc, URC_NATT_KEEP_ALIVE_CHANGED)) {
        onNotifyNattKeepAliveChanged(msg);
    } else if (strStartsWith(urc, URC_WIFI_PDN_OOS)) {
        onNotifyWifiPdnOOS(msg);
    } else if (strStartsWith(urc, URC_WIFI_LOCK)) {
        onNotifyWifiLock(msg);
    }
}

void RmcMobileWifiUrcHandler::onHandleTimer() {
    // do something
}

void RmcMobileWifiUrcHandler::onNotifyWfcWifiRoveout(const sp<RfxMclMessage>& msg) {
    /*
     * +EWFCRVOUT: <ifname>,<rvout>,<mobike_ind>
     * <ifname>: "wlan0"
     * <rvout>: 1 = Wifi roveout, no LTE for H.O.; 0 = Wifi RSSI acceptable
     * <mobike_ind>: Mobike trigger indication. 0 = not trigger mobike, 1= trigger mobike
     */
    logD(RFX_LOG_TAG, "onNotifyWfcWifiRoveout()");
    const int maxLen = 3;
    int rfxMsg = RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifyPdnHandover(const sp<RfxMclMessage>& msg) {
    /*
     * +EPDNHANDOVER: <pdn_type>,<status>,<src_rat>,<des_rat>
     * <pdn_type>: 0 = IMS, 1 = SMS, 2 = MMS
     * <status>: 0 = start, 1 = success, -1 = failed
     * <src_rat>: source rat, 1 = LTE, 2 = WIFI
     * <des_rat>: destination rat, 1 = LTE, 2 = WIFI
     * <phoneId>: add by MCL
     */
    const int maxLen = 4;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyActiveWifiPdnCount(const sp<RfxMclMessage>& msg) {
    /*
     * +EWIFIPDNACT: <cnt>
     * <cnt>: Wifi PDN count
     * <phoneId>: add by MCL
     */
    logD(RFX_LOG_TAG, "onNotifyActiveWifiPdnCount()");
    const int maxLen = 1;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyWifiRssiMonitoringConfig(const sp<RfxMclMessage>& msg) {
    /*
     * +EWIFIRSSITHRCFG: <en>,<thr_cnt>,<rssi_thr_1>,< rssi_thr_2>,...
     * <en>: enabled, 0 = disable; 1 = enabled
     * <thr_cnt>: threshold count
     * <rssi_thr_1>,< rssi_thr_2>,...: threshold
     * <phoneId>: add by MCL
     */
    const int maxLen = 0;  // dynamic
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyWifiPdnError(const sp<RfxMclMessage>& msg) {
    /*
     * +EWOPDNERR: <cnt>,<cause>,<sub_cause>
     * <cnt>: error cause count
     * <cause>: main cause
     * <sub_cause>: sub cause
     * <phoneId>: add by MCL
     */
    const int maxLen = 3;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_PDN_ERROR;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyGeoRequest(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSGEO: <account_id>,<broadcast_flag>,<latitude>,<longitude>,<accurate>
     * <account_id>: request id, 0~7
     * <broadcast_flag>: 0, 1
     * <latitude>: latitude from GPS, 0 as failed
     * <longitude>: longitude from GPS, 0 as failed
     * <accurate>: accurate from GPS, 0 as failed
     * <method>
     * <city>
     * <state>
     * <zip>
     * <country>
     * <ue_wifi_mac>
     * <confidence>
     */
    // Only send first five parameters and confidence to framework
    const int maxLen = 6;
    int rfxMsg = RFX_MSG_UNSOL_REQUEST_GEO_LOCATION;

    // FCC requirement, accurate with specific confidence
    int err;
    int accountId, broadcastFlag;
    int latitude, longitude, accurate, mdConfidence;
    char* method = NULL;
    char* city = NULL;
    char* state = NULL;
    char* zip = NULL;
    char* country = NULL;
    char* ueWifiMac = NULL;
    RfxAtLine* line = new RfxAtLine(msg->getRawUrc()->getLine(), NULL);
    //logD(RFX_LOG_TAG, "onNotifyGeoRequest:%s", msg->getRawUrc()->getLine());
    char* responseStr[maxLen] = { 0 };
    sp<RfxMclMessage> urc;

    line->atTokStart(&err);
    if (err < 0) goto error;

    accountId = line->atTokNextint(&err);
    if (err < 0) goto error;

    broadcastFlag = line->atTokNextint(&err);
    if (err < 0) goto error;

    latitude = line->atTokNextint(&err);
    if (err < 0) goto error;

    longitude = line->atTokNextint(&err);
    if (err < 0) goto error;

    accurate = line->atTokNextint(&err);
    if (err < 0) goto error;

    method = line->atTokNextstr(&err);
    if (err < 0) goto error;

    city = line->atTokNextstr(&err);
    if (err < 0) goto error;

    state = line->atTokNextstr(&err);
    if (err < 0) goto error;

    zip = line->atTokNextstr(&err);
    if (err < 0) goto error;

    country = line->atTokNextstr(&err);
    if (err < 0) goto error;

    asprintf(&responseStr[0], "%d", accountId);
    asprintf(&responseStr[1], "%d", broadcastFlag);
    asprintf(&responseStr[2], "%d", latitude);
    asprintf(&responseStr[3], "%d", longitude);
    asprintf(&responseStr[4], "%d", accurate);

    ueWifiMac= line->atTokNextstr(&err);
    if (err < 0){
        logW(RFX_LOG_TAG, "onNotifyGeoRequest, AT command did not include ueWifiMac");
    } else {
        logD(RFX_LOG_TAG, "onNotifyGeoRequest, ueWifiMac:%s", ueWifiMac);
        mdConfidence = line->atTokNextint(&err);
        if (err < 0) {
            logW(RFX_LOG_TAG, "onNotifyGeoRequest, AT command did not include confidence");
            asprintf(&responseStr[5], "");
        } else {
            logI(RFX_LOG_TAG, "onNotifyGeoRequest, MD request confidence:%d", mdConfidence);
            asprintf(&responseStr[5], "%d", mdConfidence);
        }
    }

    urc = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
        RfxStringsData(responseStr, maxLen));
    responseToTelCore(urc);

    if(responseStr[0] != NULL) {
        free(responseStr[0]);
        responseStr[0] = NULL;
    }
    if(responseStr[1] != NULL) {
        free(responseStr[1]);
        responseStr[1] = NULL;
    }
    if(responseStr[2] != NULL) {
        free(responseStr[2]);
        responseStr[2] = NULL;
    }
    if(responseStr[3] != NULL) {
        free(responseStr[3]);
        responseStr[3] = NULL;
    }
    if(responseStr[4] != NULL) {
        free(responseStr[4]);
        responseStr[4] = NULL;
    }
    if(responseStr[5] != NULL) {
        free(responseStr[5]);
        responseStr[5] = NULL;
    }

    delete line;
    return;
error:
    logE(RFX_LOG_TAG, "Something wrong with the URC:%s", msg->getRawUrc()->getLine());
    delete line;
}

void RmcMobileWifiUrcHandler::onNotifyNattKeepAliveChanged(const sp<RfxMclMessage>& msg) {
    /*
     * +EWOKEEPALIVE: <enable>,<interval>,<src_ip>,<src_port>,<dst_ip>,<dst_port>
     * <enable>: enabled, 0 = disable; 1 = enabled
     * <interval>: NATT interval, if interval < 20s, wifi NATT keep alive will not start
     * <src_ip>: source IP
     * <src_port>: source port
     * <dst_ip>: destination IP
     * <dst_port>: destination port, if dst_port != 4500, wifi NATT keep alive will not start
     */
    logD(RFX_LOG_TAG, "onNotifyNattKeepAliveChanged()");
    const int maxLen = 6;
    int rfxMsg = RFX_MSG_UNSOL_NATT_KEEP_ALIVE_CHANGED;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifyWifiPdnOOS(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSPDNOOS: <apn>, <cid>, <oos_state>
     * <apn>: apn name
     * <cid>: call id
     * <oos_state>:
     * 0: OOS ended & PDN disconnected
     * 1: OOS started
     * 2: OOS ended & PDN resumed to use
     */
    logD(RFX_LOG_TAG, "onNotifyWifiPdnOOS()");
    const int maxLen = 3;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_PDN_OOS;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifyWifiLock(const sp<RfxMclMessage>& msg) {
    /*
     * +EWIFILOCK: <iface>, <enable>
     * <iface>: interface name
     * <enable>:
     * 0: modem disable wifi lock
     * 1: modem enable wifi lock
     */
    logD(RFX_LOG_TAG, "onNotifyWifiLock()");
    int ret;
    std::vector<char*> data;
    RfxAtLine *urc = msg->getRawUrc();

    if (urc == NULL) return;
    urc->atTokStart(&ret);
    if (ret < 0) {
        return;
    }

    char* iface = urc->atTokNextstr(&ret);
    if (ret < 0) {
        return;
    }
    data.push_back(iface);

    char* enable = urc->atTokNextstr(&ret);
    if (ret < 0) {
        return;
    }
    data.push_back(enable);
    getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_KEY_WFC_LOCK_STATE,
            atoi(enable));

    // not notify to framework. the state could be combined in WFC PDN STATE
    return;
}
