/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
#include <vector>
#include <string>

#include "RtcMobileWifiController.h"
#include "RfxRootController.h"

#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtcMwi"


RFX_IMPLEMENT_CLASS("RtcMobileWifiController", RtcMobileWifiController, RfxController);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_WFC_PDN_STATE);

RtcMobileWifiController::RtcMobileWifiController() {
    mWfcState = 0;
    mWfcLockState = 0;
    mWfcMergeState = 0;
}

RtcMobileWifiController::~RtcMobileWifiController() {
}

void RtcMobileWifiController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logD(RFX_LOG_TAG, "[onInit]");
    const int request_id_list[] = {
        RFX_MSG_REQUEST_SET_WIFI_ENABLED,
        RFX_MSG_REQUEST_SET_WIFI_ASSOCIATED,
        RFX_MSG_REQUEST_SET_WIFI_SIGNAL_LEVEL,
        RFX_MSG_REQUEST_SET_GEO_LOCATION,
        RFX_MSG_REQUEST_SET_WIFI_IP_ADDRESS,
        RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID,
        RFX_MSG_REQUEST_SET_NATT_KEEP_ALIVE_STATUS,
        RFX_MSG_REQUEST_SET_WFC_CONFIG,
        RFX_MSG_REQUEST_GET_WFC_CONFIG,
        RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE
    };

    const int urc_id_list[] = {
        RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT,
        RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER,
        RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT,
        RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG,
        RFX_MSG_UNSOL_WIFI_PDN_ERROR,
        RFX_MSG_UNSOL_REQUEST_GEO_LOCATION,
        RFX_MSG_UNSOL_NATT_KEEP_ALIVE_CHANGED,
        RFX_MSG_UNSOL_WIFI_PDN_OOS
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int), DEFAULT);
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_WFC_STATE,
        RfxStatusChangeCallback(this, &RtcMobileWifiController::onWFCPDNStatusChanged));

    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_WFC_LOCK_STATE,
        RfxStatusChangeCallback(this, &RtcMobileWifiController::onWFCLockStatusChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_CONNECTION_STATE,
        RfxStatusChangeCallback(this, &RtcMobileWifiController::onConnectionStateChanged));
}

void RtcMobileWifiController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");

    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_WFC_STATE,
        RfxStatusChangeCallback(this, &RtcMobileWifiController::onWFCPDNStatusChanged));

    getNonSlotScopeStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_WFC_LOCK_STATE,
        RfxStatusChangeCallback(this, &RtcMobileWifiController::onWFCLockStatusChanged));

    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_CONNECTION_STATE,
        RfxStatusChangeCallback(this, &RtcMobileWifiController::onConnectionStateChanged));

    RfxController::onDeinit();
}

bool RtcMobileWifiController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    logD(RFX_LOG_TAG,"onHandleRequest: request id %d is %s", msg_id, idToString(msg_id));
    requestToMcl(message);
    return true;
}

bool RtcMobileWifiController::onHandleUrc(const sp<RfxMessage>& message) {
    responseToRilj(message);
    return true;
}

bool RtcMobileWifiController::onHandleResponse(const sp<RfxMessage>& response) {
    int msgId = response->getId();
    logD(RFX_LOG_TAG, "onHandleResponse, handle %s", RFX_ID_TO_STR(msgId));
    responseToRilj(response);
    return true;
}

bool RtcMobileWifiController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff,int radioState) {
    /* Reject the request when modem off */
    if (isModemPowerOff == true) {
        logD(RFX_LOG_TAG, "onCheckIfRejectMessage, id = %d, isModemPowerOff = %d, rdioState = %d",
                message->getId(), isModemPowerOff, radioState);
        return true;
    }

    return false;
}

void RtcMobileWifiController::onWFCPDNStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value,
    RfxVariant new_value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    int state = new_value.asInt();
    mWfcState = state;
    mergeWfcState();

}

void RtcMobileWifiController::onWFCLockStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value,
    RfxVariant new_value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    int state = new_value.asInt();
    mWfcLockState = state;
    mergeWfcState();
}

void RtcMobileWifiController::mergeWfcState(){
    int oldState = mWfcMergeState;
/*
    //no WiFi activility(PDN state 0, lock 0) RAT UNKNOWN
#define REGISTRATION_STATE_NOT_REGISTERED_OR_SEARCHING 0
    //WiFi activility (PDN state 1) RAT ILWAN
#define REGISTRATION_STATE_HOME 1
    //WiFi activility (PDN state 0, lock 1) RAT ILWAN
#define REGISTRATION_STATE_NOT_REGISTERED_SEARCHING 2
*/
    if (mWfcState) {
        mWfcMergeState = REGISTRATION_STATE_HOME;
    }else if (mWfcLockState) {
        mWfcMergeState = REGISTRATION_STATE_NOT_REGISTERED_SEARCHING;
    }else {
        mWfcMergeState = REGISTRATION_STATE_NOT_REGISTERED_OR_SEARCHING;
    }

    if (oldState != mWfcMergeState) {
        sp<RfxMessage> msg;
        std::vector<int> params;
        params.push_back(mWfcMergeState);
        params.push_back(getSlotId());

        logD(RFX_LOG_TAG, "mergeWfcState() state:%d simIdx:%d", params.at(0), params.at(1));
        msg = RfxMessage::obtainUrc(params.at(1), RFX_MSG_URC_WFC_PDN_STATE,
            RfxIntsData(params.data(), (int)params.size()));
        RtcMobileWifiController::onHandleUrc(msg);
    }
}

void RtcMobileWifiController::onConnectionStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    bool oldState = false, newState = false;

    RFX_UNUSED(key);
    oldState = old_value.asBool();
    newState = value.asBool();

    logI(RFX_LOG_TAG, "[%d]onConnectionStateChanged: oldState = %d, newState = %d",
            m_slot_id, oldState, newState);

    if (!oldState && newState) {
        mWfcState = getStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);
        mWfcLockState = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_LOCK_STATE, 0);
        mergeWfcState();
    }
}

