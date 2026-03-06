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
#include "RtcOemController.h"
#include "modecontroller/RtcModeSwitchController.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include <libmtkrilutils.h>
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RpOemCtlr"
#define ARRAY_LENGTH(array) (sizeof(array)/sizeof(array[0]))
#define RTC_OEM_STRING_STATUS_SYNC "STATUS_SYNC"
#define RTC_OEM_STRING_SCREEN_STATE_ON "SCREEN_STATE_ON"
#define RTC_OEM_STRING_SCREEN_STATE_OFF "SCREEN_STATE_OFF"
#define RTC_OEM_STRING_AUTO_ANSWER "AUTO_ANSWER"

RFX_IMPLEMENT_CLASS("RtcOemController", RtcOemController, RfxController);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_TX_POWER_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SEND_SAR_IND);

RtcOemController::RtcOemController() : m_pending_request(NULL) {
}

RtcOemController::~RtcOemController() {
}

void RtcOemController::onInit() {
    RfxController::onInit();
    logD(RFX_LOG_TAG, "init()");
    static const int request[] = {
        RFX_MSG_REQUEST_GET_HARDWARE_CONFIG,
        RFX_MSG_REQUEST_DEVICE_IMEI,
        RFX_MSG_REQUEST_GET_IMEI,
        RFX_MSG_REQUEST_DEVICE_IDENTITY,
        RFX_MSG_REQUEST_BASEBAND_VERSION,
        RFX_MSG_REQUEST_OEM_HOOK_RAW,
        RFX_MSG_REQUEST_OEM_HOOK_STRINGS,
        RFX_MSG_REQUEST_SET_TRM,
        RFX_MSG_REQUEST_GET_ACTIVITY_INFO,
        RFX_MSG_REQUEST_QUERY_MODEM_THERMAL,
        RFX_MSG_REQUEST_SET_TX_POWER_STATUS,
        RFX_MSG_REQUEST_GET_PHONE_CAPABILITY,
        RFX_MSG_REQUEST_ENABLE_DSDA_INDICATION,
        RFX_MSG_REQUEST_GET_DSDA_STATUS,
        RFX_MSG_REQUEST_ROUTE_CERTIFICATE,
        RFX_MSG_REQUEST_ROUTE_AUTH,
        RFX_MSG_REQUEST_ENABLE_CAPABILITY,
        RFX_MSG_REQUEST_ABORT_CERTIFICATE,
        RFX_MSG_REQUEST_QUERY_CAPABILITY,
        RFX_MSG_REQUEST_SEND_SAR_IND,
        RFX_MSG_REQUEST_SET_MODEM_CONFIG,
        RFX_MSG_REQUEST_SET_HSR_MODE,
        RFX_MSG_REQUEST_NV_RESET_CONFIG,
    };

    registerToHandleRequest(request, ARRAY_LENGTH(request));
}

bool RtcOemController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff,int radioState) {
    if (message->getId() == RFX_MSG_REQUEST_GET_PHONE_CAPABILITY
            || message->getId() == RFX_MSG_REQUEST_GET_HARDWARE_CONFIG) {
        return false;
    }

    if (isModemPowerOff == false && (
            message->getId() == RFX_MSG_REQUEST_OEM_HOOK_RAW
            || message->getId() == RFX_MSG_REQUEST_OEM_HOOK_STRINGS)) {
        return false;
    }

    if(radioState == (int)RADIO_STATE_OFF && (
            message->getId() == RFX_MSG_REQUEST_DEVICE_IMEI
            || message->getId() == RFX_MSG_REQUEST_GET_IMEI
            || message->getId() == RFX_MSG_REQUEST_DEVICE_IDENTITY
            || message->getId() == RFX_MSG_REQUEST_BASEBAND_VERSION
            || message->getId() == RFX_MSG_REQUEST_SET_TRM
            || message->getId() == RFX_MSG_REQUEST_GET_ACTIVITY_INFO
            || message->getId() == RFX_MSG_REQUEST_QUERY_MODEM_THERMAL
            || message->getId() == RFX_MSG_REQUEST_SET_TX_POWER_STATUS
            || message->getId() == RFX_MSG_REQUEST_ENABLE_DSDA_INDICATION
            || message->getId() == RFX_MSG_REQUEST_GET_DSDA_STATUS
            || message->getId() == RFX_MSG_REQUEST_ROUTE_CERTIFICATE
            || message->getId() == RFX_MSG_REQUEST_ROUTE_AUTH
            || message->getId() == RFX_MSG_REQUEST_ENABLE_CAPABILITY
            || message->getId() == RFX_MSG_REQUEST_ABORT_CERTIFICATE
            || message->getId() == RFX_MSG_REQUEST_QUERY_CAPABILITY
            || message->getId() == RFX_MSG_REQUEST_SET_MODEM_CONFIG
            || message->getId() == RFX_MSG_REQUEST_SET_HSR_MODE
            )) {
        return false;
    }
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

bool RtcOemController::onPreviewMessage(const sp<RfxMessage>& message) {
    if (message->getId() == RFX_MSG_REQUEST_SET_MODEM_CONFIG) {
        if (getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_MODEM_OFF_STATE,
                MODEM_OFF_IN_IDLE) == MODEM_OFF_BY_SIM_SWITCH) {
            // Wait for SIM switch
            return false;
        }
    }

    if (message->getId() == RFX_MSG_REQUEST_DEVICE_IDENTITY) {
        if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_SIM_TASK_READY) == false) {
            // wait for eusim
            return false;
        }
    }

    if (message->getId() == RFX_MSG_REQUEST_GET_HARDWARE_CONFIG) {
        // wait for all channels init done
        if (getRildInitDoneState() == false) {
            return false;
        }
    }

    return true;
}

bool RtcOemController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    if (message->getId() == RFX_MSG_REQUEST_SET_MODEM_CONFIG) {
        if (getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_MODEM_OFF_STATE,
                MODEM_OFF_IN_IDLE) == MODEM_OFF_BY_SIM_SWITCH) {
            // Wait for SIM switch
            return false;
        }
    }

    if (message->getId() == RFX_MSG_REQUEST_DEVICE_IDENTITY) {
        if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_SIM_TASK_READY) == false) {
            // wait for eusim
            return false;
        }
    }

    if (message->getId() == RFX_MSG_REQUEST_GET_HARDWARE_CONFIG) {
        // wait for all channels init done
        if (getRildInitDoneState() == false) {
            return false;
        }
    }

    return true;
}

bool RtcOemController::onHandleRequest(const sp<RfxMessage>& message) {
    int request = message->getId();
    switch(request) {
        case RFX_MSG_REQUEST_OEM_HOOK_RAW:
            onHandleOemHookRaw(message);
            break;
        case RFX_MSG_REQUEST_OEM_HOOK_STRINGS:
            onHandleOemHookStrings(message);
            break;
        case RFX_MSG_REQUEST_GET_ACTIVITY_INFO:
            onHandleGetActivityInfo(message);
            break;
        case RFX_MSG_REQUEST_SET_MODEM_CONFIG:
            onHandleSetModemConfig(message);
            break;
        default:
            requestToMcl(message);
            break;
    }
    return true;
}

bool RtcOemController::onHandleResponse(const sp<RfxMessage>& message) {
    if (message->getId() == RFX_MSG_REQUEST_SET_MODEM_CONFIG) {
        if (message->getError() != RIL_E_SUCCESS) {
            // Enable capability switch if set modem config failed
            int waitModule = getNonSlotScopeStatusManager()->getIntValue(
                    RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE, 0);
            waitModule &= ~(CAPABILITY_SWITCH_WAIT_MODEM_CONFIG);
            getNonSlotScopeStatusManager()->setIntValue(
                    RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE,
                    waitModule);
        }
    }
    responseToRilj(message);
    return true;
}

void RtcOemController::onHandleOemHookRaw(const sp<RfxMessage>& message) {
    char* data = (char *) message->getData()->getData();
    if (data == NULL || strlen(data) < 1) {
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS, message,
                true);
        responseToRilj(responseMsg);
        return;
    }
    int index = needToHidenLog(data);
    if (index >= 0) {
        logD(RFX_LOG_TAG, "onHandleOemHookRaw, data = %s ***", getHidenLogPreFix(index));
    } else {
        logD(RFX_LOG_TAG, "onHandleOemHookRaw, data = %s", data);
    }
    if (isMainProtocolCommand(data)) {
        requestToMcl(message, true);
    } else {
        requestToMcl(message);
    }
}

void RtcOemController::onHandleOemHookStrings(const sp<RfxMessage>& message) {
    char** data = (char **) message->getData()->getData();
    if (data[0] == NULL || strlen(data[0]) < 1) {
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS, message,
                true);
        responseToRilj(responseMsg);
        return;
    }
    int index = needToHidenLog(data[0]);
    if (index >= 0) {
        logD(RFX_LOG_TAG, "onHandleOemHookString, data[0] = %s **", getHidenLogPreFix(index));
    } else {
        logD(RFX_LOG_TAG, "onHandleOemHookString, data[0] = %s", data[0]);
    }

    if (handleExtraState(message)) {
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
                true);
        responseToRilj(responseMsg);
        return;
    } else if (isMainProtocolCommand(data[0])) {
        requestToMcl(message, true);
    } else {
        requestToMcl(message);
    }
}

void RtcOemController::onHandleGetActivityInfo(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "onHandleGetActivityInfo");
    requestToMcl(message, true);
}

void RtcOemController::onHandleSetModemConfig(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "onHandleSetModemConfig");
    if (m_pending_request != NULL) {
        // We don't support handle another request if there is one pending request, for the pending
        // request may reset radio
        logI(RFX_LOG_TAG,
                "onHandleSetModemConfig received a request when there is a pending one");
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message,
                true);
        responseToRilj(responseMsg);
    }
    // Disable capability switch until reset radio
    int waitModule = getNonSlotScopeStatusManager()->getIntValue(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE, 0);
    waitModule |= CAPABILITY_SWITCH_WAIT_MODEM_CONFIG;
    getNonSlotScopeStatusManager()->setIntValue(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE,
            waitModule);
    RtcModeSwitchController* modeSwitchController =
            (RtcModeSwitchController *)findController(RFX_OBJ_CLASS_INFO(RtcModeSwitchController));
    if (modeSwitchController != NULL && modeSwitchController->resetCCapabilitySlot() == true) {
        getNonSlotScopeStatusManager()->registerStatusChanged(
                RFX_STATUS_KEY_MODESWITCH_FINISHED,
                RfxStatusChangeCallback(this, &RtcOemController::onModeSwitchFinished));
        m_pending_request = message;
    } else {
        // Send request to MCL directly if no need to reset C2K capability to slot0
        requestToMcl(message);
    }
}

void RtcOemController::onModeSwitchFinished(RfxStatusKeyEnum key, RfxVariant old_value,
                                                            RfxVariant value) {
    RFX_UNUSED(key);
    logD(RFX_LOG_TAG, "onModeSwitchFinished (%d, %d)", old_value.asInt(), value.asInt());
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(
            RFX_STATUS_KEY_MODESWITCH_FINISHED,
            RfxStatusChangeCallback(this, &RtcOemController::onModeSwitchFinished));
    if (m_pending_request != NULL) {
        requestToMcl(m_pending_request);
        m_pending_request = NULL;
    }
}

bool RtcOemController::isMainProtocolCommand(char* target) {
    if (strstr(target, "AT+EFUN") != NULL ||
              strstr(target, "AT+ESIMMAP") != NULL ||
              strstr(target, "AT+ETXANT") != NULL) {
        return true;
    }
    return false;
}

bool RtcOemController::handleExtraState(const sp<RfxMessage>& message) {
    char** data = (char **) message->getData()->getData();
    if (strstr(data[0], RTC_OEM_STRING_STATUS_SYNC) != NULL) {
        String8 str(data[1]);
        getStatusManager()->setString8Value(RFX_STATUS_KEY_TELEPHONY_ASSISTANT_STATUS, str);
        return true;
    }
    if (strstr(data[0], RTC_OEM_STRING_AUTO_ANSWER) != NULL) {
        String8 str(data[1]);
        getStatusManager()->setString8Value(RFX_STATUS_KEY_AUTO_ANSWER, str);
        return true;
    }
    return false;
}
