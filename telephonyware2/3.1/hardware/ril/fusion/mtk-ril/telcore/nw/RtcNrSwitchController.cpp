/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2019. All rights reserved.
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
#include "RtcNrSwitchController.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define NRSW_CTRL_TAG "RtcNrSwCtrl"

RFX_IMPLEMENT_CLASS("RtcNrSwitchController", RtcNrSwitchController, RfxController);

#define PROPERTY_NR_SLOTID      "persist.vendor.radio.nrslot"

RtcNrSwitchController::RtcNrSwitchController() :
    mCheckerController(NULL),
    mNrSlot(0),
    mPreferredDataSlot(-1),
    mTriggerRetry(false),
    mCapaNoNeed(0) {
}

RtcNrSwitchController::~RtcNrSwitchController() {
}

void RtcNrSwitchController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logV(NRSW_CTRL_TAG, "[onInit]");
    char feature[] = "GEMINI_NR_PLUS_SUPPORTED";
    char NR_VER[] = "5G_SUPPORTED";

    const int request_id_list[] = {
        RFX_MSG_REQUEST_NR_SWITCH
    };

    if (getFeatureVersion(NR_VER, 0) == 1 && getFeatureVersion(feature, 0) == 0) {
        registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int), DEFAULT);
        char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
        int val = 0;
        rfx_property_get(PROPERTY_NR_SLOTID, stgBuf, "0");
        val = strtol(stgBuf, NULL, 10);
        if (val == 0) {
            val = 1;
            rfx_property_set(PROPERTY_NR_SLOTID, String8::format("%d", 1).string());
        }
        getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_NR_SWITCH_SLOT, val);

        // NR switch follow default data
        getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_DEFAULT_DATA_SIM,
                RfxStatusChangeCallback(this, &RtcNrSwitchController::onDefaultDataChanged));

        // NR switch after SIM switch
        getNonSlotScopeStatusManager()->registerStatusChanged(
                RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,
                 RfxStatusChangeCallback(this, &RtcNrSwitchController::onCapabilitySwitchStatusChanged));

        for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
            getStatusManager(slotId)->registerStatusChangedEx(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                    RfxStatusChangeCallbackEx(this, &RtcNrSwitchController::onApVoiceCallCountChanged));
        }

        // Check NR switching state
        getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_IS_NR_SWITCHING,
                RfxStatusChangeCallback(this, &RtcNrSwitchController::onNrSwitchRetry));
    } else if (getFeatureVersion(NR_VER, 0) == 1 && getFeatureVersion(feature, 0) == 1) {
        char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
        int val = 0;
        rfx_property_get(PROPERTY_NR_SLOTID, stgBuf, "-1");
        val = strtol(stgBuf, NULL, 10);
        if (val != -1) {
            rfx_property_set(PROPERTY_NR_SLOTID, String8::format("%d", 0).string());
        }
    }
}

bool RtcNrSwitchController::onHandleResponse(const sp<RfxMessage>& response) {
    int msg_id = response->getId();

    switch (msg_id) {
        case RFX_MSG_REQUEST_NR_SWITCH:
            responseNrSwitch(response);
            return true;
        default:
            break;
    }
    return true;
}

void RtcNrSwitchController::responseNrSwitch(const sp<RfxMessage>& response) {
    logV(NRSW_CTRL_TAG, "[responseNrSwitch] error: %d ", response->getError());
}

int RtcNrSwitchController::getNrSlot() {
    char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
    int val = 0;
    rfx_property_get(PROPERTY_NR_SLOTID, stgBuf, "1");
    val = strtol(stgBuf, NULL, 10);
    mNrSlot = val-1;
    logV(NRSW_CTRL_TAG, "getNrSlot:: mNrSlot:%d", mNrSlot);
    return mNrSlot;
}

void RtcNrSwitchController::switchNrSlot() {
    if (isNRSwitching() == true) {
        mTriggerRetry = true;
        int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
        int defaultDataSim = getNonSlotScopeStatusManager()
                ->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1); // get default data sim slot id.
        if (mainSlot == 1) { // switch PS if main protocol in slot 2
            defaultDataSim = defaultDataSim ? 0:1;
        }
        logV(NRSW_CTRL_TAG, "NR is in switching! mPreferredDataSlot:%d, defaultDataSim:%d",
                mPreferredDataSlot, defaultDataSim);
        mPreferredDataSlot = defaultDataSim;
        return;
    } else if (isNetworkScanOngoing() != -1) {
       RtcRatSwitchController* nwRatController = (RtcRatSwitchController *)findController
               (isNetworkScanOngoing(), RFX_OBJ_CLASS_INFO(RtcRatSwitchController));
       sp<RfxAction> action = new RfxAction0(this,
                &RtcNrSwitchController::onLocalAbortAvailableNetworkDone);
        nwRatController->setLocalAbortAvailableNetwork(action);
    } else {
        int OriginalDataSim = -1, defaultDataSim = -1;
        int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
        OriginalDataSim = getNonSlotScopeStatusManager()
                ->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, 0); // get default data sim slot id.
        defaultDataSim = OriginalDataSim;
        if (mainSlot == 1) { // switch PS if main protocol in slot 2
            defaultDataSim = defaultDataSim ? 0:1;
        }
        logV(NRSW_CTRL_TAG, "switchNrSlot:defaultSim:%d, PreferredSlot:%d, OriginalSim:%d, NrSlot:%d",
                defaultDataSim, mPreferredDataSlot, OriginalDataSim, getNrSlot());

        if ((OriginalDataSim != -1 && mPreferredDataSlot != defaultDataSim) ||
                (OriginalDataSim != -1 && getNrSlot() != OriginalDataSim)) {
            getNonSlotScopeStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_NR_SWITCHING, true);
            sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(mainSlot,
                    RFX_MSG_REQUEST_NR_SWITCH, RfxIntsData(&defaultDataSim, 1));
            requestToMcl(reqToRild);
            mPreferredDataSlot = defaultDataSim;
        }
    }
}

void RtcNrSwitchController::switchNrSlot(int nrSlot) {
    if (isNRSwitching() == true) {
        logV(NRSW_CTRL_TAG, "NR is in switching! mPreferredDataSlot:%d, nrSlot:%d",
                mPreferredDataSlot, nrSlot);
        mTriggerRetry = true;
        mPreferredDataSlot = nrSlot;
        return;
    } else if (isNetworkScanOngoing() != -1) {
       RtcRatSwitchController* nwRatController = (RtcRatSwitchController *)findController
               (isNetworkScanOngoing(), RFX_OBJ_CLASS_INFO(RtcRatSwitchController));
       sp<RfxAction> action = new RfxAction0(this,
                &RtcNrSwitchController::onLocalAbortAvailableNetworkDone);
        nwRatController->setLocalAbortAvailableNetwork(action);
    } else {
        int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
            logV(NRSW_CTRL_TAG, "switchNrSlot:: mainSlot:%d nrSlot:%d", mainSlot, nrSlot);
        getNonSlotScopeStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_NR_SWITCHING, true);
        sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(mainSlot,
                RFX_MSG_REQUEST_NR_SWITCH, RfxIntsData(&nrSlot, 1));
        requestToMcl(reqToRild);
        mPreferredDataSlot = nrSlot;
    }
}

void RtcNrSwitchController::onDefaultDataChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    int old_mode = old_value.asInt();
    int new_mode = value.asInt();
    char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
    int sw_disable = 0;
    mCheckerController = (RtcCapabilitySwitchChecker*)
            findController(RFX_OBJ_CLASS_INFO(RtcCapabilitySwitchChecker));

    int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    bool isSkipNrSwitch = (mCheckerController->isReadyForSwitch(false) &&
        mCheckerController->isInPukLock(false) == false);

    rfx_property_get("ro.vendor.mtk_disable_cap_switch", stgBuf, "0");
    sw_disable = strtol(stgBuf, NULL, 10);
    // trigger data switch if DATA SIM is not in main slot when SIM switch enable
    if (sw_disable != 1) {
        if (mainSlot != new_mode &&
                mCapaNoNeed == 0) {
            logV(NRSW_CTRL_TAG, "newmode:%d  mainSlot:%d, swdisable:%d, SkipSwitch:%d, skip switch!",
                    new_mode, mainSlot, sw_disable, isSkipNrSwitch);
            if (isSkipNrSwitch == true) {
                if (isNRSwitching() == true) {
                    if (mainSlot == 1) { // switch PS if main protocol in slot 2
                        new_mode = new_mode ? 0:1;
                    }
                    mPreferredDataSlot = new_mode;
                }
                return;
            }
        }
        mCapaNoNeed = 0;
    }

    if (new_mode != -1 && old_mode != new_mode) {
        if (mainSlot == 1) { // switch PS if main protocol in slot 2
            new_mode = new_mode ? 0:1;
        }
        logV(NRSW_CTRL_TAG, "DefaultDataChanged:: mainSlot:%d, defaultDataSim:%d  mPreferredDataSlot:%d",
                mainSlot, new_mode, mPreferredDataSlot);
        if (old_mode == -1 || new_mode != mPreferredDataSlot) {
            switchNrSlot(new_mode);
        }
    }
}

void RtcNrSwitchController::onCapabilitySwitchStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    int new_mode = value.asInt();

    if (new_mode == CAPABILITY_SWITCH_STATE_ENDING ||
                new_mode == CAPABILITY_SWITCH_STATE_NO_NEED) {
        int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
        int defaultDataSim = getNonSlotScopeStatusManager()
                ->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM); // get default data sim slot id.
        if (defaultDataSim == -1) {
            defaultDataSim = getNrSlot();
        }

        if (mainSlot < 0) {
            logE(NRSW_CTRL_TAG, "Capability Changed:: mainSlot:%d error", mainSlot);
            return;
        } else if (mainSlot == 1) { // switch PS if main protocol in slot 2
            defaultDataSim = defaultDataSim ? 0:1;
        }

        logV(NRSW_CTRL_TAG, "Capability Changed:: mainSlot:%d, defaultDataSim:%d mPreferredDataSlot:%d",
                mainSlot, defaultDataSim, mPreferredDataSlot);
        if (new_mode == CAPABILITY_SWITCH_STATE_NO_NEED &&
                defaultDataSim == mPreferredDataSlot) {
            mCapaNoNeed = 1;
            return;
        }
        switchNrSlot(defaultDataSim);
    }
}

void RtcNrSwitchController::onApVoiceCallCountChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    int oldMode = old_value.asInt();
    int mode = value.asInt();

    logV(NRSW_CTRL_TAG, "%s, slotId:%d, key:%d oldMode:%d, mode:%d",
            __FUNCTION__, slotId, key, oldMode, mode);
    if (mode == 0 && oldMode > 0) {
        switchNrSlot();
    }
}

void RtcNrSwitchController::onLocalAbortAvailableNetworkDone() {
    logV(NRSW_CTRL_TAG, "Local abort done");
    switchNrSlot();
}

int RtcNrSwitchController::isNetworkScanOngoing() {
    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        if (getStatusManager(slotId)->getBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING) == true) {
            return slotId;
        }
    }
    return -1;
}

bool RtcNrSwitchController::isNRSwitching() {
    return getNonSlotScopeStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_NR_SWITCHING, false);
}


void RtcNrSwitchController::onNrSwitchRetry(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    int oldMode = old_value.asBool();
    int mode = value.asBool();

    logV(NRSW_CTRL_TAG, "%s, key:%d oldMode:%d, mode:%d, mTriggerRetry:%d",
            __FUNCTION__, key, oldMode, mode, mTriggerRetry);
    if (mTriggerRetry == true && mode == false) {
        int OriginalDataSim = -1, defaultDataSim = -1;
        int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
        OriginalDataSim = getNonSlotScopeStatusManager()
                ->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, 0); // get default data sim slot id.
        defaultDataSim = OriginalDataSim;
        if (mainSlot == 1) { // switch PS if main protocol in slot 2
            defaultDataSim = defaultDataSim ? 0:1;
        }
        logV(NRSW_CTRL_TAG, "switchNrSlot:defaultSim:%d, PreferredSlot:%d, OriginalSim:%d, NrSlot:%d",
                defaultDataSim, mPreferredDataSlot, OriginalDataSim, getNrSlot());
        getNonSlotScopeStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_NR_SWITCHING, true);
        mTriggerRetry = false;
        sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(mainSlot,
                RFX_MSG_REQUEST_NR_SWITCH, RfxIntsData(&defaultDataSim, 1));
        requestToMcl(reqToRild);
        mPreferredDataSlot = defaultDataSim;
    }
}
