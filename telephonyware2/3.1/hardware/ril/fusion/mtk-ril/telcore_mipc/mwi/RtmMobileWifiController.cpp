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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <vector>
#include <string>
#include <dlfcn.h>

#include "RtmMobileWifiController.h"
#include "RfxRootController.h"

#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxRilUtils.h"
#include "RfxVoidData.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtmMwi"
#define LINK_LIBWPFA_PATH "libwpfa.so"

RFX_IMPLEMENT_CLASS("RtmMobileWifiController", RtmMobileWifiController, RfxController);

static bool sNeedInitWpfa = true;
void *dlTelcoreWpfaHandle = NULL;

RtmMobileWifiController::RtmMobileWifiController() {
}

RtmMobileWifiController::~RtmMobileWifiController() {
}

void RtmMobileWifiController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logD(RFX_LOG_TAG, "[onInit]");
    const int request_id_list[] = {
        RFX_MSG_REQUEST_SET_WIFI_ENABLED,
        RFX_MSG_REQUEST_SET_WIFI_ASSOCIATED,
        RFX_MSG_REQUEST_SET_WFC_CONFIG,
        RFX_MSG_REQUEST_GET_WFC_CONFIG,
        RFX_MSG_REQUEST_SET_WIFI_SIGNAL_LEVEL,
        RFX_MSG_REQUEST_SET_GEO_LOCATION,
        RFX_MSG_REQUEST_SET_WIFI_IP_ADDRESS,
        RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID,
        RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE
    };

    const int urc_id_list[] = {
        RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT,
        RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER,
        RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT,
        RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG,
        RFX_MSG_UNSOL_WIFI_PDN_ERROR,
        RFX_MSG_UNSOL_REQUEST_GEO_LOCATION,
        RFX_MSG_UNSOL_WIFI_PDN_OOS,
        RFX_MSG_URC_WFC_PDN_STATE
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int), DEFAULT);
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));

    /**
     * WPFA initial: check IMS SUBMARINE
     *
     *    Enable : Create WPFA service
     *    Disable: Do not create WPFA service
     */
    char imsSubmarinefeature[] = "IMS SUBMARINE";
    int imsSubmarinesupport = getFeatureVersion(imsSubmarinefeature);
    int (*doWpfaInit)() = NULL;
    logD(RFX_LOG_TAG, "MD Feature: imsSubmarinesupport=%d", imsSubmarinesupport);

    dlTelcoreWpfaHandle = dlopen(LINK_LIBWPFA_PATH, RTLD_NOW);
    if (dlTelcoreWpfaHandle == NULL) {
        logI(RFX_LOG_TAG, "[%s] dlopen: module = %s, failed (%s)", __FUNCTION__, LINK_LIBWPFA_PATH, dlerror());
    } else {
        logI(RFX_LOG_TAG, "[%s] dlopen: module = %s, pass", __FUNCTION__, LINK_LIBWPFA_PATH);
        doWpfaInit = (int (*)()) dlsym(dlTelcoreWpfaHandle, "wpfaInit");
    }

    if (imsSubmarinesupport == 1) {
        if (sNeedInitWpfa == true) {
            logI(RFX_LOG_TAG, "init wpfa sNeedInitWpfa:%d", sNeedInitWpfa);
            sNeedInitWpfa = false;
            if (doWpfaInit != NULL) {
                doWpfaInit();
            } else {
                logI(RFX_LOG_TAG, "doWpfaInit function is not working.");
            }
        }
    }
}

void RtmMobileWifiController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    if (dlTelcoreWpfaHandle != NULL)
        dlclose(dlTelcoreWpfaHandle);
    RfxController::onDeinit();
}

bool RtmMobileWifiController::onHandleRequest(const sp<RfxMessage>& message) {
    requestToMcl(message);
    return true;
}

bool RtmMobileWifiController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    logD(RFX_LOG_TAG,"onHandleUrc: urc id %d is %s", msg_id, idToString(msg_id));
    responseToRilj(message);
    return true;
}

bool RtmMobileWifiController::onHandleResponse(const sp<RfxMessage>& response) {
    int msg_id = response->getId();
    logD(RFX_LOG_TAG,"onHandleResponse: request id %d is %s", msg_id, idToString(msg_id));
    responseToRilj(response);
    return true;
}

bool RtmMobileWifiController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff,int radioState) {
    /* Reject the request when modem off */
    if (isModemPowerOff == true) {
        logD(RFX_LOG_TAG, "onCheckIfRejectMessage, id = %d, isModemPowerOff = %d, rdioState = %d",
                message->getId(), isModemPowerOff, radioState);
        return true;
    }

    return false;
}
