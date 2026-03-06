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
#include "RtcWpController.h"
#include "RfxRilUtils.h"
#include "RfxLog.h"
#include "RfxAction.h"
#include "RfxMainThread.h"
#include "capabilityswitch/RtcCapabilityGetController.h"
#include "power/RtcRadioController.h"

#define WP_LOG_TAG "RtcWp"

#define WP_LOCK_IDLE (0)
#define WP_LOCK_BY_WP (1)
#define WP_LOCK_BY_OTHERS (-1)

// World Phone start changing
#define WP_SWITCHING_START (0)
// World Phone end changing
#define WP_SWITCHING_END   (1)
// World Phone not receive response from RMC, but another URC comes again.
#define WP_SWITCHING_SECOND_URC (2)

// No moudle block World Phone change
#define NO_BLOCKING_CHANGING (0)
// One module has blocked World Phone changing
#define ALREADY_BLOCKING_CHANGING (1)

// other module releases block World Phone change
#define NO_BLOCK_WP_CHANGED (0)
// other module wants to block next World Phone change
#define BLOCK_WP_CHANGED (1)

#define WP_COMMAND_MAX_RETRY_COUNT (30)
#define WP_COMMAND_RETRY_TIME (1000)

#define RTC_WP_STRING_WORLD_MODE_ID "worldmodeid,"

#define MODE_SWITCH_RETRY (-1)
#define MODE_SWITCH_ECC_SEARCH (0x7)

#define MODE_SWITCH_EMC_SEARCH_BY_VDM (0x08)
#define MODE_SWIRCH_EMC_SEARCH_BY_GMSS (0x09)

#define ERR_MODEM_NOT_ALLOW (54)
#define VALID_CALL_SLOT_ID (-1)

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcWpController", RtcWpController, RfxController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_WORLD_MODE_RELOAD_MODEM_TYPE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_WORLD_MODE_STORE_MODEM_TYPE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_WORLD_MODE_RESUMING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_WORLD_MODE_STORE_ID);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_WORLD_MODE_RELOAD_ID);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_WORLD_MODE_MODIFY_MODEM_TYPE);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_WORLD_MODE_CHANGED);

int RtcWpController::worldModeBlockedNum = 0;
int RtcWpController::mainSlotId = 0;
int RtcWpController::mainWorldModeId = 0;

RtcWpController::RtcWpController() {
    closeRadioCount = 0;
    openRadioCount = 0;
    mWpTimerHandle = NULL;
    mRetryResumeCount = 0;
    mCallSlotId = VALID_CALL_SLOT_ID;
    mRadioOffValue = 0;
    mModifyRadioNum = 0;
    for (int i = 0; i < MAX_SIM_COUNT; i++) {
        backupRadioPower[i] = false;
    }
}

RtcWpController::~RtcWpController() {
}

void::RtcWpController::onDeinit() {
}

void RtcWpController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    const int request_id_list[] = {
        RFX_MSG_REQUEST_WORLD_MODE_RELOAD_MODEM_TYPE,
        RFX_MSG_REQUEST_WORLD_MODE_STORE_MODEM_TYPE,
        RFX_MSG_REQUEST_WORLD_MODE_RESUMING,
        RFX_MSG_REQUEST_WORLD_MODE_STORE_ID,
        RFX_MSG_REQUEST_WORLD_MODE_RELOAD_ID,
        RFX_MSG_REQUEST_WORLD_MODE_MODIFY_MODEM_TYPE,
    };

    const int urc_id_list[] = {
        RFX_MSG_URC_WORLD_MODE_CHANGED,
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        registerToHandleRequest(i, request_id_list, sizeof(request_id_list)/sizeof(const int));
        registerToHandleUrc(i, urc_id_list, sizeof(urc_id_list)/sizeof(const int));
        getStatusManager(i)->registerStatusChangedEx(RFX_STATUS_KEY_TELEPHONY_ASSISTANT_STATUS,
                RfxStatusChangeCallbackEx(this, &RtcWpController::onTelephonyAssistantStatusChanged));
    }

    closeRadioCount = 0;
    worldModeBlockedNum = 0;
    mCallSlotId = VALID_CALL_SLOT_ID;
    mRadioOffValue = 0;
    mModifyRadioNum = 0;
    getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_END);

    // register callbacks to get required information
    logD(WP_LOG_TAG, "RtcWpController::onInit");
}

bool RtcWpController::onPreviewMessage(const sp<RfxMessage>& message) {
    RFX_MESSAGE_TYPE type = message->getType();
    if (type == URC) {
        if(canhandleMessage(true)) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

bool RtcWpController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    RFX_MESSAGE_TYPE type = message->getType();
    if (type == URC) {
        if(canhandleMessage(false)) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

bool RtcWpController::canhandleMessage(bool log_flag) {
    int blockStatusKeyValue = getBlockedWorldModeStatusKeyValue(log_flag);
    if (log_flag == true) {
        logD(WP_LOG_TAG, "canhandleMessage blockStatusKeyValue = %d", blockStatusKeyValue);
    }
    if (blockStatusKeyValue != WP_LOCK_IDLE) {
        return false;
    }
    return true;
}

bool RtcWpController::onHandleRequest(const sp<RfxMessage>& message) {
    int msgId = message->getId();
    logD(WP_LOG_TAG, "onHandleRequest, handle %s", RFX_ID_TO_STR(msgId));
    switch (msgId) {
    case RFX_MSG_REQUEST_WORLD_MODE_RELOAD_MODEM_TYPE:
    case RFX_MSG_REQUEST_WORLD_MODE_STORE_MODEM_TYPE:
    case RFX_MSG_REQUEST_WORLD_MODE_MODIFY_MODEM_TYPE: {
            requestToMcl(message);
        }
        break;
    default:
        break;
    }
    return true;
}

bool RtcWpController::onHandleUrc(const sp<RfxMessage>& message) {
    int msgId = message->getId();
    int slotId = message->getSlotId();
    int blockStatusKeyValue = getBlockedWorldModeStatusKeyValue(true);

    if (msgId == RFX_MSG_URC_WORLD_MODE_CHANGED) {
        int *pInt = (int *)message->getData()->getData();
        int state = pInt[0];
        int cause = pInt[1];
        logD(WP_LOG_TAG, "onHandleUrc, state = %d, cause=%d", state, cause);
        //world mode change start
        if (state == 0) {
            mainSlotId = slotId;
            int modeState = getNonSlotScopeStatusManager()->getIntValue(
                    RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_END);
            logD(WP_LOG_TAG, "onHandleUrc, modeState = %d, mainSlotId = %d",
                    modeState, mainSlotId);
            // world mode state changing start, no module need to block world mode switch
            if (blockStatusKeyValue == WP_LOCK_IDLE) {
                // set modem off state as world phone, it is mux with sim switch and power.
                setBlockedWorldModeStatusKeyValue(WP_LOCK_BY_WP);
                handleWorldModeChangeStart(true, cause);
            } else if (blockStatusKeyValue == WP_LOCK_BY_WP) {  // world mode not end, modem send URC again
                // when request response back, to resume world mode change
                if ((modeState == WP_SWITCHING_START) || (modeState == WP_SWITCHING_SECOND_URC)) {
                    getNonSlotScopeStatusManager()->setIntValue(
                            RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_SECOND_URC);
                } else {
                    // the first urc not request radio off, so start again.
                    handleWorldModeChangeStart(true, cause);
                }
            }
        } else if (state == 1) {
            //world mode state changing end only worldPhoneInitialize()
            logD(WP_LOG_TAG, "onHandleUrc, worldModeBlockedNum = %d", worldModeBlockedNum);
            if (worldModeBlockedNum == 0) {
                getNonSlotScopeStatusManager()->setIntValue(
                        RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_END);
            }
            responseToRilj(message);
        } else {
            RFX_ASSERT(0);
        }
    } else {
        logD(WP_LOG_TAG, "onHandleUrc, not expected:%s", RFX_ID_TO_STR(msgId));
    }
    return true;
}

bool RtcWpController::onHandleResponse(const sp<RfxMessage>& message) {
    int msgId = message->getId();
    int slotId = message->getSlotId();
    logD(WP_LOG_TAG, "onHandleResponse, handle %s", RFX_ID_TO_STR(msgId));
    switch (msgId) {
    case RFX_MSG_REQUEST_WORLD_MODE_RESUMING :
        processWorldModeResumingResponse(message);
        return true;
    case RFX_MSG_REQUEST_WORLD_MODE_RELOAD_MODEM_TYPE:
    case RFX_MSG_REQUEST_WORLD_MODE_STORE_MODEM_TYPE:
    case RFX_MSG_REQUEST_WORLD_MODE_MODIFY_MODEM_TYPE:
        responseToRilj(message);
        return true;
    case RFX_MSG_REQUEST_WORLD_MODE_RELOAD_ID:
        createNewMessageToRmc(RFX_MSG_REQUEST_WORLD_MODE_STORE_ID, slotId, mainWorldModeId);
        break;
    case RFX_MSG_REQUEST_WORLD_MODE_STORE_ID:
        createNewMessageToRmc(RFX_MSG_REQUEST_RESTART_RILD, slotId, mainWorldModeId);
        break;
    default:
        break;
    }
    return true;
}

void RtcWpController::createNewMessageToRmc(int messageId, int slotId, int value) {
    int reserved[1];
    reserved[0] = value;
    sp<RfxMessage> request = RfxMessage::obtainRequest(slotId,
            messageId, RfxIntsData(reserved, 1));
    if (RFX_MSG_REQUEST_RESTART_RILD == messageId) {
        RfxMainThread::enqueueMessage(request);
    } else {
        requestToMcl(request);
    }
}

void RtcWpController::resumeWorldModeChange() {
    int reserved[1];
    reserved[0] = 0;
    sp<RfxMessage> request = RfxMessage::obtainRequest(mainSlotId,
            RFX_MSG_REQUEST_WORLD_MODE_RESUMING, RfxIntsData(reserved, 1));
    requestToMcl(request);
}

void RtcWpController::onRequestRadioOnDone(int slotId) {
    openRadioCount++;
    logD(WP_LOG_TAG, "openRadioCount slotid=%d openRadioCount=%d, mModifyRadioNum=%d",
            slotId, openRadioCount, mModifyRadioNum);
    if (openRadioCount == mModifyRadioNum) {
        mModifyRadioNum = 0;
        openRadioCount = 0;
        // world mode state changing end
        getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_WORLD_MODE_STATE,
                WP_SWITCHING_END);
    }
}

bool RtcWpController::isValidSim(int slotId) {
    bool ret = true;
    int simType = getStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_CARD_TYPE, 0);
    int enableState = getStatusManager(slotId)->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE, 0);
    int simState = getStatusManager(slotId)->getIntValue(
            RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_UNKNOWN);

    if (simType <= 0 ||
        enableState == UICC_APPLICATIONS_ENABLE_STATE_DISABLED ||
        simState == RFX_SIM_STATE_ABSENT) {
        logD(WP_LOG_TAG, "slotid=%d, invalid sim card", slotId);
        ret = false;
    }

    return ret;
}

void RtcWpController::requestRadioOn(int slotId) {
    logD(WP_LOG_TAG, "restoreRadioPower slotid=%d %d", slotId, backupRadioPower[slotId]);

    if (isValidSim(slotId)) {
        sp<RfxAction> action0 = new RfxAction1<int>(this,
                &RtcWpController::onRequestRadioOnDone, slotId);
        RtcRadioController* radioController = (RtcRadioController *)findController(
                slotId, RFX_OBJ_CLASS_INFO(RtcRadioController));
        radioController->moduleRequestRadioPower(backupRadioPower[slotId], action0,
                RFOFF_CAUSE_DUPLEX_MODE);
    }
}

void RtcWpController::requestRadioOff(int slotId) {
    getStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
    backupRadioPower[slotId] = getStatusManager(slotId)->getBoolValue(
            RFX_STATUS_KEY_REQUEST_RADIO_POWER, false);
    logD(WP_LOG_TAG, "backupRadioPower slotid=%d %d", slotId, backupRadioPower[slotId]);
    RtcRadioController* radioController = (RtcRadioController *)findController(
            slotId, RFX_OBJ_CLASS_INFO(RtcRadioController));
    radioController->moduleRequestRadioPowerCallback(false, RtcModuleRequestRadioCallback(this,
            &RtcWpController::moduleRequestRadioPowerRadioOffDone),
            RFOFF_CAUSE_DUPLEX_MODE);
}

void RtcWpController::resumeWorldModeChangeWithRadioOff(bool isStandAlone) {
    int status[1];
    status[0] = 0;
    // set status key as start state
    getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_START);
    sp<RfxMessage> startUrc = RfxMessage::obtainUrc(
            mainSlotId, RFX_MSG_URC_WORLD_MODE_CHANGED, RfxIntsData(status, 1));
    responseToRilj(startUrc);
    if (isStandAlone == false) {
        mModifyRadioNum = RfxRilUtils::rfxGetSimCount();
        for (int slotId = RFX_SLOT_ID_0; slotId < mModifyRadioNum; slotId++) {
            requestRadioOff(slotId);
        }
    } else {
        // stand alone duplex mode only turn off main protocol raido.
        mModifyRadioNum = 1;
        requestRadioOff(mainSlotId);
    }
}

void RtcWpController::onCallCountChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    int callCount = value.asInt();
    logD(WP_LOG_TAG, "callCount %d", callCount);
    // When current call finished, execute pended world mode.
    if (callCount == 0) {
        mCallSlotId = VALID_CALL_SLOT_ID;
        getStatusManager(slotId)->unRegisterStatusChangedEx(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                RfxStatusChangeCallbackEx(this, &RtcWpController::onCallCountChanged));
        logD(WP_LOG_TAG, "onCallCountChanged Slot %d call finished", slotId);
        resumeWorldModeChangeWithRadioOff(false);
    }
}

void RtcWpController::handleWorldModePendedByCall(int slotId) {
    getStatusManager(slotId)->registerStatusChangedEx(
            RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
            RfxStatusChangeCallbackEx(this, &RtcWpController::onCallCountChanged));
}

int RtcWpController::getCallingSlotId() {
    int callingSlotId = -1;
    for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
        if (getStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0) > 0) {
            callingSlotId = slotId;
            break;
        }
    }
    return callingSlotId;
}

void RtcWpController::onRetryResumeTimeOut() {
    if (mWpTimerHandle != NULL) {
        RfxTimer::stop(mWpTimerHandle);
    }
    mWpTimerHandle = NULL;
    logD(WP_LOG_TAG, "onRetryResumeTimeOut count = %d", mRetryResumeCount);
    if (mRetryResumeCount < WP_COMMAND_MAX_RETRY_COUNT) {
        mRetryResumeCount++;
        mWpTimerHandle = RfxTimer::start(RfxCallback0(this,
                &RtcWpController::resumeWorldModeChange), ms2ns(WP_COMMAND_RETRY_TIME));
    }
}

bool RtcWpController::isStandAloneChange(int cause) {
    if (cause == MODE_SWITCH_EMC_SEARCH_BY_VDM || cause == MODE_SWIRCH_EMC_SEARCH_BY_GMSS) {
        return true;
    }
    return false;
}

void RtcWpController::handleWorldModeChangeStart(bool flag, int cause) {
    if (true == flag) {
        int callingSlotId = getCallingSlotId();
        logD(WP_LOG_TAG, "handleWorldModeChangeStart %d, %d, cause:%d, mCallSlotId:%d",
                flag, callingSlotId, cause, mCallSlotId);
        // IF ECC is calling, resuming radio off not wait call off
        if (isStandAloneChange(cause) == true) {
            resumeWorldModeChangeWithRadioOff(true);
        } else if (callingSlotId >= RFX_SLOT_ID_0 && cause != MODE_SWITCH_ECC_SEARCH) {
            mCallSlotId = callingSlotId;
            handleWorldModePendedByCall(callingSlotId);
        } else {
            if (mCallSlotId != VALID_CALL_SLOT_ID) {
                getStatusManager(mCallSlotId)->unRegisterStatusChangedEx(
                        RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                        RfxStatusChangeCallbackEx(this, &RtcWpController::onCallCountChanged));
                mCallSlotId = VALID_CALL_SLOT_ID;
            }
            resumeWorldModeChangeWithRadioOff(false);
        }
    } else {
        logD(WP_LOG_TAG, "handleWorldModeChangeStart flag = %d", flag);
        onRetryResumeTimeOut();
    }
}

void RtcWpController::processWorldModeResumingResponse(const sp<RfxMessage>& message) {
    int status[1];  // response RILJ, changing success or fail
    int *pInt = (int *)message->getData()->getData();
    int state = pInt[0];  // error number
    int duplexMode = pInt[1];
    int modeChangingState = 1;
    char worldModeState[RFX_PROPERTY_VALUE_MAX] = {0};

    if (message->getError() != RIL_E_SUCCESS && state != ERR_MODEM_NOT_ALLOW) {
        status[0] = -1;
        // retry according to state
        if (mRetryResumeCount < WP_COMMAND_MAX_RETRY_COUNT) {
            handleWorldModeChangeStart(false, MODE_SWITCH_RETRY);
            return;
        }
        logD(WP_LOG_TAG, "processWorldModeResumingResponse not retry %d", mRetryResumeCount);
    } else {
        status[0] = 1;
    }

    if (mWpTimerHandle != NULL) {
        RfxTimer::stop(mWpTimerHandle);
        mWpTimerHandle = NULL;
    }

    mRetryResumeCount = 0;
    modeChangingState = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_END);
    rfx_property_get("persist.vendor.radio.wm_state", worldModeState, "1");
    logD(WP_LOG_TAG, "ResumingResponse, state=%d, status=%d, mode=%d, %d, %s, RadioNum = %d",
            state, status[0], duplexMode, modeChangingState, worldModeState, mModifyRadioNum);
    // a new urc is sent before world change end, so resume again
    if (modeChangingState == WP_SWITCHING_SECOND_URC) {
        // send local request to MCL for resume world mode switch
        // if (strcmp("-1", worldModeState) == 0) {
            handleWorldModeChangeStart(false, MODE_SWITCH_RETRY);
            return;
        // }
        // world mode end, ignore to send reponse to modem once.
    }
    // reset modem off state as idle
    int blockStatusKeyValue = getBlockedWorldModeStatusKeyValue(true);
    if (blockStatusKeyValue == WP_LOCK_BY_WP) {
         setBlockedWorldModeStatusKeyValue(WP_LOCK_IDLE);
    }
    // send end urc to RILJ
    sp<RfxMessage> startUrc = RfxMessage::obtainUrc(mainSlotId, RFX_MSG_URC_WORLD_MODE_CHANGED, RfxIntsData(status, 1));
    responseToRilj(startUrc);
    // radio on and send radio capability
    RtcCapabilityGetController* capabilityGetController =
            (RtcCapabilityGetController *)findController(RFX_OBJ_CLASS_INFO(RtcCapabilityGetController));
    capabilityGetController->updateRadioCapabilityForWMChange(duplexMode);
    if (mModifyRadioNum == RfxRilUtils::rfxGetSimCount()) {
        for (int slotId = RFX_SLOT_ID_0; slotId < mModifyRadioNum; slotId++) {
            requestRadioOn(slotId);
        }
    } else if (mModifyRadioNum == 1) {
        requestRadioOn(mainSlotId);
    }

}

bool RtcWpController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
            bool isModemPowerOff, int radioState) {
    int msgId = message->getId();
    int slotId = message->getSlotId();

    logD(WP_LOG_TAG, "onCheckIfRejectMessage, isMDPowerOff=%d, radioState=%d, msgId=%d, slotId=%d",
            isModemPowerOff, radioState, msgId, slotId);
    return false;
}

bool RtcWpController::isWorldModeSwitching() {
    int modeState = WP_SWITCHING_END;
    modeState = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_END);
    logD(WP_LOG_TAG, "isWorldModeSwitching, modeState = %d", modeState);
    if (modeState != WP_SWITCHING_END) {
        return true;
    }
    return false;
}

int RtcWpController::getBlockedWorldModeStatusKeyValue(bool log_flag) {
    int modemOffState = getNonSlotScopeStatusManager()
            ->getIntValue(RFX_STATUS_KEY_MODEM_OFF_STATE, MODEM_OFF_IN_IDLE);
    if (log_flag == true) {
        logD(WP_LOG_TAG, "RtcWpController::getBlockedStatusKey modemOffState = %d", modemOffState);
    }
    if (MODEM_OFF_IN_IDLE == modemOffState) {
        for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
            RadioPowerLock radioLock = (RadioPowerLock)getStatusManager(slotId)->getIntValue(
                    RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
            logD(WP_LOG_TAG, "getBlockedStatusKey slotId =%d radioLock =%d ", slotId, radioLock);
            if (RADIO_LOCK_IDLE != radioLock) {
                return WP_LOCK_BY_OTHERS;
            }
        }
        // never set blocked status key
        return WP_LOCK_IDLE;
    } else if (MODEM_OFF_BY_WORLD_PHONE == modemOffState) {
        for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
            RadioPowerLock radioLock = (RadioPowerLock)getStatusManager(slotId)->getIntValue(
                    RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
            logD(WP_LOG_TAG, "getBlockedStatusKey radioLock = %d", radioLock);
            if (RADIO_LOCK_BY_WORLD_MODE != radioLock) {
                return WP_LOCK_BY_OTHERS;
            }
        }
        // world phone set blocked status key
        return WP_LOCK_BY_WP;
    } else {
        // other module set blocked status key
        return WP_LOCK_BY_OTHERS;
    }
}

int RtcWpController::setBlockedWorldModeStatusKeyValue(int value) {
    logD(WP_LOG_TAG, "RtcWpController::setBlockedStatusKey value = %d", value);
    if (WP_LOCK_IDLE == value) {
        getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_MODEM_OFF_STATE, MODEM_OFF_IN_IDLE);
        for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
            getStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_IDLE);
        }
    } else if (WP_LOCK_BY_WP == value) {
        getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_MODEM_OFF_STATE, MODEM_OFF_BY_WORLD_PHONE);
        for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
            getStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_BY_WORLD_MODE);
        }
    } else {
        RFX_ASSERT(0);
    }
    return 0;
}

void RtcWpController::onTelephonyAssistantStatusChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    const char* newValue = value.asString8().string();
    logD(WP_LOG_TAG, "AssistantStatusChanged str = %s", newValue);
    char c = ',';
    int id = 0;
    if (strstr(newValue, RTC_WP_STRING_WORLD_MODE_ID) != NULL ) {
        const char* ptr = strchr(newValue, c);
        if (ptr != NULL) {
            id = atoi(ptr + 1);
        }
    } else {
        return;
    }
    logD(WP_LOG_TAG, "AssistantStatusChanged str = %s id = %d", newValue, id);
    int reserved[1];
    reserved[0] = id;
    mainWorldModeId = id;
    sp<RfxMessage> request = RfxMessage::obtainRequest(slotId,
            RFX_MSG_REQUEST_WORLD_MODE_RELOAD_ID, RfxIntsData(reserved, 1));
    requestToMcl(request);
}

void RtcWpController::resetWorldPhoneSettings(void) {
    closeRadioCount = 0;
    mRetryResumeCount = 0;
    mRadioOffValue = 0;
    mModifyRadioNum = 0;
    // world mode state changing end
    getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_WORLD_MODE_STATE, WP_SWITCHING_END);
    // reset modem off state as idle
    int blockStatusKeyValue = getBlockedWorldModeStatusKeyValue(true);
    logD(WP_LOG_TAG, "resetWorldPhoneSettings blockStatusKeyValue:%d", blockStatusKeyValue);
    if (blockStatusKeyValue == WP_LOCK_BY_WP) {
        setBlockedWorldModeStatusKeyValue(WP_LOCK_IDLE);
    }
}

void RtcWpController::moduleRequestRadioPowerRadioOffDone(RIL_Errno err, int slotId) {
    closeRadioCount++;
    int status[1];
    logD(WP_LOG_TAG, "radioOffDone:%d, %d, %d, %d", err, closeRadioCount, slotId, mModifyRadioNum);
    getStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_RADIO_LOCK, RADIO_LOCK_BY_WORLD_MODE);

    if (err == RIL_E_OPERATION_NOT_ALLOWED) {
        mRadioOffValue = RIL_E_OPERATION_NOT_ALLOWED;
    }
    if (closeRadioCount == mModifyRadioNum) {
        if (mRadioOffValue == RIL_E_OPERATION_NOT_ALLOWED) {
            resetWorldPhoneSettings();
            status[0] = 1;
            sp<RfxMessage> startUrc = RfxMessage::obtainUrc(
                    mainSlotId, RFX_MSG_URC_WORLD_MODE_CHANGED, RfxIntsData(status, 1));
            responseToRilj(startUrc);
        } else {
            closeRadioCount = 0;
            resumeWorldModeChange();
        }
    }
}
