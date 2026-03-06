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
#include "RtmRadioController.h"
#include "RfxMainThread.h"
#include "utils/RefBase.h"
#include "utils/Errors.h"
#include <string.h>
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxRilUtils.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtmRadioCont"
#define ARRAY_LENGTH(array) (sizeof(array)/sizeof(array[0]))
#define DELAY_RADIO_OFF_TIMER 1000 // 1s

TimerHandle RtmRadioController::mTimerHandle = NULL;
bool RtmRadioController::isOnSimDetecting = false;
bool RtmRadioController::isDelayedPowerOff = false;

extern "C" void setRadioState(RIL_RadioState newState, RIL_SOCKET_ID rid);


RFX_IMPLEMENT_CLASS("RtmRadioController", RtmRadioController, RfxController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_THERMAL_RADIO_OFF);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_THERMAL_RADIO_ON);

RtmRadioController::RtmRadioController() {
}

RtmRadioController::~RtmRadioController() {
}

void RtmRadioController::onInit() {
    RfxController::onInit();
    logD(RFX_LOG_TAG, "init()");

    static const int request[] = {
        RFX_MSG_REQUEST_RADIO_POWER,
        RFX_MSG_REQUEST_BOOT_TURN_ON_RADIO,
        RFX_MSG_REQUEST_ENABLE_MODEM,
        RFX_MSG_REQUEST_THERMAL_RADIO_OFF,
        RFX_MSG_REQUEST_THERMAL_RADIO_ON,
    };

    getStatusManager()->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_UNAVAILABLE);
    registerToHandleRequest(request, ARRAY_LENGTH(request));
    registerForStatusChange();

    mEmptyCallback = RtmModuleRequestRadioCallback();
    logD(RFX_LOG_TAG, "init():Done");
}

void RtmRadioController::registerForStatusChange() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("ro.vendor.mtk_telephony_add_on_policy", prop, "");

    logD(RFX_LOG_TAG, "registerForStatusChange, add_on_policy = %s", prop);
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallback(this, &RtmRadioController::onRadioStateChanged));
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
            RfxStatusChangeCallback(this, &RtmRadioController::onCardTypeChanged));
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SIM_STATE,
            RfxStatusChangeCallback(this, &RtmRadioController::onSimStateChanged));
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE,
            RfxStatusChangeCallback(this, &RtmRadioController::onSimEnableStateChanged));
}

void RtmRadioController::onRadioStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    //sync with rild's state
    int newValue = value.asInt();
    int oldValue = old_value.asInt();
    bool requestPower = getStatusManager()->getBoolValue(RFX_STATUS_KEY_REQUEST_RADIO_POWER, false);
    if ((requestPower == false) && ((RIL_RadioState)newValue == RADIO_STATE_ON) &&
            ((RIL_RadioState)oldValue != RADIO_STATE_UNAVAILABLE)) {
        logI(RFX_LOG_TAG, "radio State: %d -> %d, STATUS_KEY_REQUEST_RADIO_POWER = false not update to RILJ",
                oldValue, newValue);
    } else {
        if (newValue != oldValue) {
            logI(RFX_LOG_TAG, "radio State: %d -> %d, using google native API for urc", oldValue, newValue);
            setRadioState((RIL_RadioState)newValue, (RIL_SOCKET_ID)m_slot_id);
        } else {
            logI(RFX_LOG_TAG, "radio state not change (%d), do not update to RILJ", newValue);
        }
    }
}

void RtmRadioController::onCardTypeChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    char filghtMode[RFX_PROPERTY_VALUE_MAX] = { 0 };
    bool isFlightMode = false;
    int newValue = value.asInt();
    int oldValue = old_value.asInt();

    rfx_property_get("persist.vendor.radio.airplane.mode.on", filghtMode, "false");
    if (strcmp("true", filghtMode) == 0) {
        isFlightMode = true;
    }
    if ((RFX_STATUS_KEY_CARD_TYPE == key) && (oldValue == -1) && (newValue > 0)
            && (isFlightMode == false)) {
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_REQUEST_RADIO_POWER, true);
    }

    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (getStatusManager(i)->getIntValue(RFX_STATUS_KEY_CARD_TYPE, 0) == -1) {
            logE(RFX_LOG_TAG, "onCardTypeChanged: only one card ready, unregister %d and return",
                    m_slot_id);
            // unregister
            getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
                   RfxStatusChangeCallback(this, &RtmRadioController::onCardTypeChanged));
            return;
        }
    }

    int simStatus = 0;
    for (int i = 0 ; i < RfxRilUtils::rfxGetSimCount(); i++) {
        simStatus |= ((getStatusManager(i)->getIntValue(RFX_STATUS_KEY_CARD_TYPE, 0)> 0) ? 1 : 0) << i;
    }
    logD(RFX_LOG_TAG, "onCardTypeChanged: simStatus %d", simStatus);

    RadioPowerLock radioLock =
            (RadioPowerLock) getStatusManager(m_slot_id)->getIntValue(
            RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);

    if (radioLock != RADIO_LOCK_IDLE) {
        logI(RFX_LOG_TAG, "Radio is locked. radioLock: %d", radioLock);
        getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
                RfxStatusChangeCallback(this, &RtmRadioController::onCardTypeChanged));
        return;
    }

    getStatusManager()->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_BY_RADIO);
    sp<RfxMessage> reqMsg = RfxMessage::obtainRequest(m_slot_id,
            RFX_MSG_REQUEST_BOOT_TURN_ON_RADIO, RfxIntsData(&simStatus, 1));
    requestToMcl(reqMsg);

    // unregister
    logD(RFX_LOG_TAG, "onCardTypeChanged: unregister %d", m_slot_id);
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
            RfxStatusChangeCallback(this, &RtmRadioController::onCardTypeChanged));
}

bool RtmRadioController::isInCalling() {
    bool ret = false;
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        int call_count = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0);
        bool ecc_mode = getStatusManager(i)->getBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE, false);
        int cb_mode = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE, 0);
        if (call_count > 0 || cb_mode == 1 || ecc_mode == true) {
            ret = true;
            logD(RFX_LOG_TAG, "isInCalling = true(%d, %d, %d)", call_count, ecc_mode, cb_mode);
            break;
        }
    }
    if (!ret) {
        bool power_on_for_ecc = getNonSlotScopeStatusManager()->getBoolValue(
                RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC, false);
        if (power_on_for_ecc) {
            ret = true;
            logD(RFX_LOG_TAG, "isInCalling = true(power on for ecc)");
        }
    }
    if (!ret) {
        int ecc_slot = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT, -1);
        if (ecc_slot != -1) {
            ret = true;
            logD(RFX_LOG_TAG, "isInCalling = true(ecc retry)");
        }
    }
    return ret;
}

bool RtmRadioController::isNetworkRegistered(int slotId) {
    bool ret = false;
    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState state = getStatusManager(slotId)
        ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    int voiceRegState = state.getRilVoiceRegState();

    if (voiceRegState == RIL_REG_STATE_HOME ||
        voiceRegState == RIL_REG_STATE_ROAMING) {
        ret = true;
        logD(RFX_LOG_TAG, "network is registered, state = %d", voiceRegState);
    }
    return ret;
}

bool RtmRadioController::isRadioLocked() {
    bool ret = false;
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        RadioPowerLock radioLock = (RadioPowerLock)getStatusManager(i)->getIntValue(
                RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
        if (radioLock != RADIO_LOCK_IDLE) {
            ret = true;
            logD(RFX_LOG_TAG, "isRadioLocked = true(%d)", radioLock);
            break;
        }
    }
    return ret;
}

bool RtmRadioController::isValidSim(int slotId) {
    bool ret = true;
    int simType = getStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_CARD_TYPE, 0);
    int enableState = getStatusManager(slotId)->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE, 0);

    if (simType <= 0 || enableState == UICC_APPLICATIONS_ENABLE_STATE_DISABLED) {
        ret = false;
    }

    return ret;
}

void RtmRadioController::onSimStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    int newValue = value.asInt();
    int oldValue = old_value.asInt();
    char filghtMode[RFX_PROPERTY_VALUE_MAX] = { 0 };

    rfx_property_get("persist.vendor.radio.airplane.mode.on", filghtMode, "false");
    if (strcmp("true", filghtMode) == 0) {
        logI(RFX_LOG_TAG, "onSimStateChanged %d -> %d, in filghtMode", oldValue, newValue);
        return;
    }
    logI(RFX_LOG_TAG, "onSimStateChanged %d -> %d", oldValue, newValue);
    if ((isValidSim(m_slot_id) == false) && (newValue != RFX_SIM_STATE_ABSENT)) {
        logD(RFX_LOG_TAG, "onSimStateChanged, slot_%d is invalid", m_slot_id);
        newValue = RFX_SIM_STATE_ABSENT;
    }

    if (newValue == RFX_SIM_STATE_ABSENT || newValue == RFX_SIM_STATE_READY) {
        if (mTimerHandle != NULL) {
            logD(RFX_LOG_TAG, "onSimStateChanged, stop timer");
            RfxTimer::stop(mTimerHandle);
        }

        isOnSimDetecting = true;
        isDelayedPowerOff = false;
        mTimerHandle = RfxTimer::start(RfxCallback0(this,
                &RtmRadioController::onTimeout), ms2ns(DELAY_RADIO_OFF_TIMER));
        logD(RFX_LOG_TAG, "onSimStateChanged, start timer");
        updateRadioState();
    }
}

void RtmRadioController::onSimEnableStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    int newValue = value.asInt();
    int oldValue = old_value.asInt();

    logI(RFX_LOG_TAG, "onSimEnableStateChanged %d -> %d", oldValue, newValue);

    if (newValue == UICC_APPLICATIONS_ENABLE_STATE_ENABLED ||
            newValue == UICC_APPLICATIONS_ENABLE_STATE_DISABLED) {
        if (mTimerHandle != NULL) {
            logD(RFX_LOG_TAG, "onSimEnableStateChanged, stop timer");
            RfxTimer::stop(mTimerHandle);
        }

        isOnSimDetecting = true;
        isDelayedPowerOff = false;
        mTimerHandle = RfxTimer::start(RfxCallback0(this,
                &RtmRadioController::onTimeout), ms2ns(DELAY_RADIO_OFF_TIMER));
        logD(RFX_LOG_TAG, "onSimEnableStateChanged, start timer");
        updateRadioState();
    }
}

void RtmRadioController::onCallStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    RFX_UNUSED(value);

    logD(RFX_LOG_TAG, "onCallStateChanged");
    if (isInCalling() == false) {
        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            getStatusManager(i)->unRegisterStatusChanged(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                    RfxStatusChangeCallback(this, &RtmRadioController::onCallStateChanged));
            getStatusManager(i)->unRegisterStatusChanged(RFX_STATUS_KEY_EMERGENCY_MODE,
                    RfxStatusChangeCallback(this, &RtmRadioController::onCallStateChanged));
            getStatusManager(i)->unRegisterStatusChanged(RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE,
                    RfxStatusChangeCallback(this, &RtmRadioController::onCallStateChanged));
        }
        getNonSlotScopeStatusManager()->unRegisterStatusChanged(
                RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC,
                RfxStatusChangeCallback(this, &RtmRadioController::onCallStateChanged));
        getNonSlotScopeStatusManager()->unRegisterStatusChanged(
                RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT,
                RfxStatusChangeCallback(this, &RtmRadioController::onCallStateChanged));
        updateRadioState();
    }
}

void RtmRadioController::onRadioLockChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    RFX_UNUSED(value);

    logD(RFX_LOG_TAG, "onRadioLockChanged");
    if (isRadioLocked() == false) {
        for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            getStatusManager(i)->unRegisterStatusChanged(RFX_STATUS_KEY_RADIO_LOCK,
                    RfxStatusChangeCallback(this, &RtmRadioController::onRadioLockChanged));
        }
        updateRadioState();
    }
}

void RtmRadioController::onServiceStateChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    RFX_UNUSED(value);

    logD(RFX_LOG_TAG, "onServiceStateChanged");
    if (isNetworkRegistered(slotId) == false) {
        getStatusManager(slotId)->unRegisterStatusChangedEx(RFX_STATUS_KEY_SERVICE_STATE,
                RfxStatusChangeCallbackEx(this, &RtmRadioController::onServiceStateChanged));
        updateRadioState();
    }
}

void RtmRadioController::onTimeout() {
    logD(RFX_LOG_TAG, "onTimeout");
    isOnSimDetecting = false;
    mTimerHandle = NULL;

    if (isDelayedPowerOff == true) {
        isDelayedPowerOff = false;
        updateRadioState();
    }
}

void RtmRadioController::updateRadioState() {
    // latest radio power requst from upper layer
    bool requestRadioPower[MAX_SIM_COUNT];
    // current radio state
    int currentRadioPower[MAX_SIM_COUNT];
    // SIM state
    int simState[MAX_SIM_COUNT];
    bool tempRadioPower[MAX_SIM_COUNT];
    int simCount = RfxRilUtils::rfxGetSimCount();

    // check radio lock state before send request
    if (isRadioLocked() == true) {
        logD(RFX_LOG_TAG, "updateRadioState, in radio lock state, pending request");
        for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            getStatusManager(i)->registerStatusChanged(RFX_STATUS_KEY_RADIO_LOCK,
                    RfxStatusChangeCallback(this, &RtmRadioController::onRadioLockChanged));
        }
        return;
    }

    // caculate the radio state by sim inserted state and java framework requested state
    for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
        requestRadioPower[slotId] = getStatusManager(slotId)->getBoolValue(
                RFX_STATUS_KEY_REQUEST_RADIO_POWER, false);
        currentRadioPower[slotId] = getStatusManager(slotId)->getIntValue(
                RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_ON);
        simState[slotId] = getStatusManager(slotId)->getIntValue(
                RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_UNKNOWN);

        if (isValidSim(slotId) == false && simState[slotId] != RFX_SIM_STATE_ABSENT) {
            logD(RFX_LOG_TAG, "updateRadioState, slot%d is invalid", slotId);
            simState[slotId] = RFX_SIM_STATE_ABSENT;
        }

        if (simState[slotId] == RFX_SIM_STATE_ABSENT) {
            // SIM absent state, set radio power off
            tempRadioPower[slotId] = false;
            simCount --;
        } else if (simState[slotId] == RFX_SIM_STATE_UNKNOWN ||
                simState[slotId] == RFX_SIM_STATE_NOT_READY) {
            // SIM transitional state, keep current radio state
            if (currentRadioPower[slotId] == RADIO_STATE_ON){
                tempRadioPower[slotId] = true;
            } else {
                tempRadioPower[slotId] = false;
            }
        }else {
            // SIM instered state, follow the upper layer request radio state
            tempRadioPower[slotId] = requestRadioPower[slotId];
        }

        logI(RFX_LOG_TAG, "updateRadioState, SIM%d state:%d, request:%d, current:%d", slotId + 1,
                simState[slotId], requestRadioPower[slotId], currentRadioPower[slotId]);
    }

    // in no sim inserted state, power on main protocol slot, and power off other slots
    int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    if (simCount == 0 && mainSlot >= 0 && mainSlot < MAX_SIM_COUNT) {
        logD(RFX_LOG_TAG, "updateRadioState, power on main slot: SIM%d", mainSlot);
        tempRadioPower[mainSlot] = requestRadioPower[mainSlot];
    }

    // send request if needed
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if ((tempRadioPower[i] == true && (currentRadioPower[i] != RADIO_STATE_ON)) ||
            (tempRadioPower[i] == false && (currentRadioPower[i] == RADIO_STATE_ON))) {
            logD(RFX_LOG_TAG, "updateRadioState, send req, SIM%d=%d", i + 1, tempRadioPower[i]);
            if (tempRadioPower[i] == false) {
                // pending radio off request in calling state
                if (isInCalling() == true) {
                    logD(RFX_LOG_TAG, "updateRadioState, pending radio off due to phone call");
                    for (int j = 0; j < RfxRilUtils::rfxGetSimCount(); j++) {
                        getStatusManager(j)->registerStatusChanged(
                                RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, RfxStatusChangeCallback(
                                this, &RtmRadioController::onCallStateChanged));
                        getStatusManager(j)->registerStatusChanged(
                                RFX_STATUS_KEY_EMERGENCY_MODE, RfxStatusChangeCallback(
                                this, &RtmRadioController::onCallStateChanged));
                        getStatusManager(j)->registerStatusChanged(
                                RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE, RfxStatusChangeCallback(
                                this, &RtmRadioController::onCallStateChanged));
                    }
                    getNonSlotScopeStatusManager()->registerStatusChanged(
                            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC,
                            RfxStatusChangeCallback(this,
                                    &RtmRadioController::onCallStateChanged));
                    getNonSlotScopeStatusManager()->registerStatusChanged(
                            RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT,
                            RfxStatusChangeCallback(this,
                                    &RtmRadioController::onCallStateChanged));
                    continue;
                }

                // pending radio off request before network deregistered
                if (isNetworkRegistered(i) == true) {
                    logD(RFX_LOG_TAG, "updateRadioState, pending radio off due to network state");
                    getStatusManager(i)->registerStatusChangedEx(
                            RFX_STATUS_KEY_SERVICE_STATE, RfxStatusChangeCallbackEx(
                            this, &RtmRadioController::onServiceStateChanged));
                    continue;
                }

                if (isOnSimDetecting == true) {
                    isDelayedPowerOff = true;
                    logI(RFX_LOG_TAG, "updateRadioState, pending radio off due to sim detecting");
                } else {
                    RtmRadioController* radio_controller = (RtmRadioController *)findController(
                            i, RFX_OBJ_CLASS_INFO(RtmRadioController));
                    radio_controller->moduleRequestRadioPower(
                            tempRadioPower[i], NULL, RFOFF_CAUSE_UNSPECIFIED);
                }
            } else {
                RtmRadioController* radio_controller = (RtmRadioController *)findController(
                        i, RFX_OBJ_CLASS_INFO(RtmRadioController));
                radio_controller->moduleRequestRadioPower(
                        tempRadioPower[i], NULL, RFOFF_CAUSE_UNSPECIFIED);
            }
        }
    }
}

bool RtmRadioController::onPreviewMessage(const sp<RfxMessage>& message) {
    return canHandleRequest(message);
}

bool RtmRadioController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    return canHandleRequest(message);
}

bool RtmRadioController::canHandleRequest(const sp<RfxMessage>& message) {
    bool ret = true;
    int id = message->getId();
    if (message->getType() == REQUEST) {
        RadioPowerLock radioLock = (RadioPowerLock) getStatusManager()
                ->getIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
        if (radioLock == RADIO_LOCK_BY_THERMAL) {
            if (id != RFX_MSG_REQUEST_THERMAL_RADIO_ON) {
                ret = false;
            }
        } else if (radioLock != RADIO_LOCK_IDLE) {
            logD(RFX_LOG_TAG, "radio is locked by other module, %d", (int) radioLock);
            ret = false;
        }
    }
    return ret;
}

bool RtmRadioController::onHandleRequest(const sp<RfxMessage>& message) {
    int id = message->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch (id) {
        case RFX_MSG_REQUEST_RADIO_POWER:
            requestRadioPower(message);
            break;
        case RFX_MSG_REQUEST_ENABLE_MODEM:
            requestModemPower(message);
            break;
        case RFX_MSG_REQUEST_THERMAL_RADIO_OFF:
            requestRadioPowerByThermal(false);
            break;
        case RFX_MSG_REQUEST_THERMAL_RADIO_ON:
            requestRadioPowerByThermal(true);
            break;
        default:
            break;
    }
    return true;
}

void RtmRadioController::requestModemPower(const sp<RfxMessage>& message) {
    int state = ((int *)message->getData()->getData())[0]; // power on/off
    bool modemEnabled = getStatusManager()->getBoolValue(RFX_STATUS_KEY_REQUEST_MODEM_POWER, true);
    logI(RFX_LOG_TAG, "requestModemPower, slotId:%d, state:%d, modemEnabled:%d",
            m_slot_id, state, modemEnabled);

    if (state) {
        // request modem power on
        if (modemEnabled == false) {
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_REQUEST_MODEM_POWER, true);
        } else {
            logD(RFX_LOG_TAG, "modem is already enabled");
        }
        sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_E_SUCCESS, message);
        responseToRilj(response);
    } else {
        // request modem power off
        requestRadioPower(message);
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_REQUEST_MODEM_POWER, false);
    }
}

void RtmRadioController::requestRadioPower(const sp<RfxMessage>& message) {
    // update radio lock
    getStatusManager()->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_BY_RADIO);

    int inputParam[3] = {0, 0, 0};
    inputParam[0] = ((int *)message->getData()->getData())[0]; // power on/off
    if (message->getId() == RFX_MSG_REQUEST_RADIO_POWER) {
        inputParam[1] = ((int *)message->getData()->getData())[1]; // for ECC
        inputParam[2] = ((int *)message->getData()->getData())[2]; // prefer ECC
    }
    logI(RFX_LOG_TAG, "requestRadioPower, slotId:%d, onOff: %d, forEcc %d, preEcc %d",
            m_slot_id, inputParam[0], inputParam[1], inputParam[2]);

    bool modemPowerOff = getNonSlotScopeStatusManager()->getBoolValue(
            RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    bool waitToRestart = getNonSlotScopeStatusManager()->getBoolValue(
            RFX_STATUS_KEY_HAD_POWER_OFF_MD, false);
    bool modemEnabled = getStatusManager()->getBoolValue(
            RFX_STATUS_KEY_REQUEST_MODEM_POWER, true);
    if (modemPowerOff || waitToRestart || (modemEnabled == false)) {
        logD(RFX_LOG_TAG, "modem is power off or RILD is restarting, radio is not available");
        getStatusManager()->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
        sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_E_RADIO_NOT_AVAILABLE, message);
        responseToRilj(response);
        return;
    }

    if (inputParam[0]) {
        // request radio on
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_REQUEST_RADIO_POWER, true);
        bool isSimInserted = true;
        bool isForEcc = false;
        bool isForCtaMode = false;
        int simState[MAX_SIM_COUNT];
        int insertedSimCount = 0;
        int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);

        for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
            simState[slotId] = getStatusManager(slotId)->getIntValue(
                    RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_UNKNOWN);
            if (isValidSim(slotId) == false && simState[slotId] != RFX_SIM_STATE_ABSENT) {
                logD(RFX_LOG_TAG, "requestRadioPower, slot%d is invalid", slotId);
                 simState[slotId] = RFX_SIM_STATE_ABSENT;
            }
            if (simState[slotId] != RFX_SIM_STATE_ABSENT) {
                insertedSimCount++;
            }
        }

        if (simState[m_slot_id] == RFX_SIM_STATE_ABSENT) {
            isSimInserted = false;
        }
        if (inputParam[1] == true || inputParam[2] == true) {
            isForEcc = true;
        }
        if (insertedSimCount == 0 && mainSlot == m_slot_id) {
            isForCtaMode = true;
        }
        // power on radio only for below cases:
        // 1. SIM inserted
        // 2. ECC requested
        // 3. In CTA mode
        logD(RFX_LOG_TAG, "requestRadioPower, simInserted:%d, forECC:%d, forCTA: %d",
                isSimInserted, isForEcc, isForCtaMode);
        if (isSimInserted == true || isForEcc == true || isForCtaMode == true) {
            sp<RfxMessage> sendMsg = RfxMessage::obtainRequest(message->getId(),
                    RfxIntsData(inputParam, 3), message, false);
            requestToMcl(sendMsg);
        } else {
            logD(RFX_LOG_TAG, "requestRadioPower, reject request.");
            getStatusManager()->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
            sp<RfxMessage> response = RfxMessage::obtainResponse(
                    RIL_E_OPERATION_NOT_ALLOWED, message);
            responseToRilj(response);
            return;
        }
    } else {
        // request radio off
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_REQUEST_RADIO_POWER, false);

        // EFUN enhancement. always add cause to radio off request
        int content[4] = {inputParam[0], inputParam[1], inputParam[2], RFOFF_CAUSE_POWER_OFF};
        sp<RfxMessage> sendMsg = RfxMessage::obtainRequest(message->getId(),
                RfxIntsData(content, 4), message, false);
        requestToMcl(sendMsg);
    }
}

bool RtmRadioController::onHandleUrc(const sp<RfxMessage>& message) {
    int id = message->getId();
    logD(RFX_LOG_TAG, "onHandleUrc, slotId: %d, MsgId: %s", m_slot_id, idToString(id));
    return true;
}

bool RtmRadioController::onHandleResponse(const sp<RfxMessage>& message) {
    int id = message->getId();
    logD(RFX_LOG_TAG, "onHandleResponse, slotId: %d, MsgId: %s", m_slot_id, idToString(id));
    switch (id) {
        case RFX_MSG_REQUEST_RADIO_POWER:
        case RFX_MSG_REQUEST_BOOT_TURN_ON_RADIO:
        case RFX_MSG_REQUEST_ENABLE_MODEM:
            handleRadioPowerResponse(message);
            break;
        default:
            break;
    }
    return true;
}

bool RtmRadioController::handleRadioPowerResponse(const sp<RfxMessage>& message) {
    // update radio lock
    getStatusManager()->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);

    sp<RfxAction> action = findAction(mActionMap, message);
    if ((sp<RfxAction>) NULL != action) {
        action->act();
        return true;
    }

    if (message->getId() != RFX_MSG_REQUEST_BOOT_TURN_ON_RADIO) {
        responseToRilj(message);
    }
    return true;
}

bool RtmRadioController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff,int radioState) {
    logD(RFX_LOG_TAG, "onCheckIfRejectMessage, id = %d, isModemPowerOff = %d, radioState = %d",
            message->getId(), isModemPowerOff, radioState);
    if (isModemPowerOff) {
        logI(RFX_LOG_TAG, "onCheckIfRejectMessage, id = %d, isModemPowerOff = %d, radioState = %d",
                message->getId(), isModemPowerOff, radioState);
        return true;
    } else {
        return false;
    }
}

sp<RfxAction> RtmRadioController::findAction(std::unordered_map<int, sp<RfxAction>>& actionMap,
        const sp<RfxMessage>& msg) {
    std::unordered_map<int, sp<RfxAction>>::const_iterator result =
            actionMap.find(msg->getToken());

    if (result == actionMap.end()) {
        logD(RFX_LOG_TAG, "findAction, no action, PToken=%d", msg->getPToken());
        return NULL;
    }
    sp<RfxAction> action = result->second;
    actionMap.erase(msg->getToken());
    logD(RFX_LOG_TAG, "findAction, found action, PToken = %d", msg->getPToken());
    return action;
}

void RtmRadioController::moduleRequestRadioPower(bool power, const sp<RfxAction>& action,
        RadioCause cause) {
    logD(RFX_LOG_TAG, "moduleRequestRadioPower: slot%d, requestPower:%d, caller: %d",
            m_slot_id, power, cause);
    int desirePower[4];
    desirePower[0] = power ? 1 : 0;
    desirePower[1] = 0;
    desirePower[2] = 0;
    desirePower[3] = (int) cause;
    sp<RfxMessage> radioRequest = RfxMessage::obtainRequest(m_slot_id, RFX_MSG_REQUEST_RADIO_POWER,
            RfxIntsData(desirePower, 4));
    if ((sp<RfxAction>) NULL != action) {
        mActionMap.insert({radioRequest->getToken(), action});
    } else {
        logD(RFX_LOG_TAG, "moduleRequestRadioPower, no action, PToken=%d",
                radioRequest->getPToken());
    }

    bool modemPowerOff = getNonSlotScopeStatusManager()->getBoolValue(
            RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    if (modemPowerOff) {
        logD(RFX_LOG_TAG, "modemPowerOff, do not execute moduleRequestRadioPower");
        return;
    }

    // To avoid conflict with request from framework, we should execute request directly
    // update radio lock
    getStatusManager()->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_BY_RADIO);
    requestToMcl(radioRequest);
}

void RtmRadioController::moduleRequestRadioPowerCallback(bool power,
        const RtmModuleRequestRadioCallback& callback, RadioCause cause) {
    // TODO: phase-out on Gen98
    RFX_UNUSED(power);
    RFX_UNUSED(callback);
    RFX_UNUSED(cause);
}

void RtmRadioController::requestRadioPowerByThermal(bool power) {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (power == false) {
            sp<RfxAction> action0 = new RfxAction1<int>(
                    this, &RtmRadioController::onRequestRadioOffDone, i);
            RtmRadioController* radio_controller = (RtmRadioController *)findController(
                    i, RFX_OBJ_CLASS_INFO(RtmRadioController));
            radio_controller->moduleRequestRadioPower(false, action0, RFOFF_CAUSE_POWER_OFF);
        } else {
            getStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
        }
    }
}

void RtmRadioController::onRequestRadioOffDone(int slotId) {
    logD(RFX_LOG_TAG, "onRequestRadioOffDone slotid=%d", slotId);
    getStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_BY_THERMAL);
}


