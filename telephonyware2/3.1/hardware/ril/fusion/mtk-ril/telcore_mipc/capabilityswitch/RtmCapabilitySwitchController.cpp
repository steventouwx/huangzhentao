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
#include "utils/Timers.h"
#include "libmtkrilutils.h"
#include "ratconfig.h"
#include "rfx_properties.h"
#include "RtmCapabilitySwitchController.h"
#include "RfxRadioCapabilityData.h"
#include "RfxMessageId.h"
#include "RfxRilUtils.h"
#include "RfxAction.h"
#include "RtmCapabilityGetController.h"
#include "RtmCapabilitySwitchChecker.h"
#include "RtmCapabilitySwitchUtil.h"
#include "power/RtmRadioController.h"

#define RFX_LOG_TAG "RtmCapa"
#define UNKNOW_SLOT  (-1)

/*****************************************************************************
 * Class RtmCapabilitySwitchController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtmCapabilitySwitchController", RtmCapabilitySwitchController, RfxController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRadioCapabilityData, RfxRadioCapabilityData, RFX_MSG_REQUEST_SET_RADIO_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxRadioCapabilityData, RFX_MSG_REQUEST_GET_RADIO_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_CAPABILITY_SWITCH_SET_MAJOR_SIM);

RtmCapabilitySwitchController::RtmCapabilitySwitchController() :
        m_request_count(0), m_max_capability(0), m_new_main_slot(0),
        m_new_main_slot_by_modem_id(-1), m_is_started(false), m_pending_request(NULL),
        m_checker_controller(NULL) {
    logI(RFX_LOG_TAG, "constructor entered");
}

RtmCapabilitySwitchController::~RtmCapabilitySwitchController() {
}

void RtmCapabilitySwitchController::onInit() {
    RfxController::onInit();

    const int request_id_list[] = {
        RFX_MSG_REQUEST_SET_RADIO_CAPABILITY,
        RFX_MSG_REQUEST_GET_RADIO_CAPABILITY,
        RFX_MSG_REQUEST_CAPABILITY_SWITCH_SET_MAJOR_SIM,
    };

    logD(RFX_LOG_TAG, "onInit");
    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        registerToHandleRequest(i, request_id_list, sizeof(request_id_list) / sizeof(const int));
    }
    resetLock(MAX_RFX_SLOT_ID, RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE);
    if (isPendingUndoneSwitch()) {
        m_new_main_slot = getUndoneSwitch();
        getNonSlotScopeStatusManager()->registerStatusChanged(
                RFX_STATUS_KEY_RILD_INIT_HANDLER_DONE, RfxStatusChangeCallback(this,
                &RtmCapabilitySwitchController::onRildInitDone));
    }
    if(getGeminiMode() == GEMINI_MODE_L_AND_L &&
            RfxRilUtils::rfxGetSimCount() == 2 && RfxRilUtils::isCtVolteSupport()) {
        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
             getStatusManager(i)->registerStatusChanged(
                     RFX_STATUS_KEY_VOLTE_STATE, RfxStatusChangeCallback(
                     this, &RtmCapabilitySwitchController::onVolteStateChanged));
        }
    }

    m_checker_controller = (RtmCapabilitySwitchChecker*)
            findController(RFX_OBJ_CLASS_INFO(RtmCapabilitySwitchChecker));
}

bool RtmCapabilitySwitchController::isPendingUndoneSwitch() {
    int undone_sim = getUndoneSwitch() + 1;
    int main_sim = RfxRilUtils::getMajorSim();
    if (undone_sim != 0 && undone_sim != main_sim) {
        return true;
    }
    return false;
}

int RtmCapabilitySwitchController::getUndoneSwitch() {
    char temp[RFX_PROPERTY_VALUE_MAX] = { 0 };
    int ret;
    rfx_property_get("persist.vendor.radio.pendcapswt", temp, "-1");
    ret = atoi(temp);
    if ((ret < -1) || (ret >= RfxRilUtils::rfxGetSimCount())) {
        ret = -1;
    }
    return ret;
}

bool RtmCapabilitySwitchController::isReadyForMessage(const sp<RfxMessage>& message, bool log) {
    int msg_id = message->getId();
    if (msg_id == RFX_MSG_REQUEST_SET_RADIO_CAPABILITY) {
        RIL_RadioCapability *capability = (RIL_RadioCapability *)message->getData()->getData();
        if (capability->phase != RC_PHASE_FINISH ) {
            return m_checker_controller->isReadyForSwitch(log);
        }
    }
    return true;
}

bool RtmCapabilitySwitchController::onPreviewMessage(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    if (msg_id == RFX_MSG_REQUEST_SET_RADIO_CAPABILITY) {
        RIL_RadioCapability *capability = (RIL_RadioCapability *)message->getData()->getData();
        if (capability->phase == RC_PHASE_APPLY) {
            // record new main slot
            if (strstr(capability->logicalModemUuid, "0") != NULL) {
                m_new_main_slot = message->getSlotId();
                rfx_property_set("persist.vendor.radio.pendcapswt",
                                 String8::format("%d", m_new_main_slot).string());
            }
            logD(RFX_LOG_TAG, "onPreview: new_main:%d", m_new_main_slot);
        }
    }
    return isReadyForMessage(message, true);
}

bool RtmCapabilitySwitchController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    return isReadyForMessage(message, false);
}

void RtmCapabilitySwitchController::calculateNewMainSlot(int capability, int slot, char* modemId) {
    int diff = (m_max_capability ^ capability);
    if (strstr(modemId, "0") != NULL) {
        m_new_main_slot_by_modem_id = slot;
    } else if ((strcmp(modemId, "") == 0) && (m_new_main_slot_by_modem_id == -1)) {
        logD(RFX_LOG_TAG, "calculateMainSlot, modemId is wrong, switch to default data sim");
        m_new_main_slot_by_modem_id = getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_DEFAULT_DATA_SIM);
    }
    logD(RFX_LOG_TAG,
         "calculateMainSlot,maxCap=%d, newMainSlot=%d, cap=%d, slot=%d, id=%s, newMainSlotById=%d",
         m_max_capability, m_new_main_slot, capability, slot, modemId, m_new_main_slot_by_modem_id);

    if (diff & RAF_LTE) {
        if (capability & RAF_LTE) {
            m_max_capability = capability;
            m_new_main_slot = slot;
        }
    } else if (diff & RAF_TD_SCDMA) {
        if (capability & RAF_TD_SCDMA) {
            m_max_capability = capability;
            m_new_main_slot = slot;
        }
    } else if (diff & RAF_UMTS) {
        if (capability & RAF_UMTS) {
            m_max_capability = capability;
            m_new_main_slot = slot;
        }
    }
    if ((m_request_count == RfxRilUtils::rfxGetSimCount()) &&
            (m_new_main_slot_by_modem_id != -1)) {
        m_new_main_slot = m_new_main_slot_by_modem_id;
        logD(RFX_LOG_TAG, "calculateMainSlot newMainSlot=%d", m_new_main_slot);
    }
}

bool RtmCapabilitySwitchController::onHandleRequest(const sp<RfxMessage> &message) {
    int msg_id = message->getId();
    RIL_RadioCapability *capability = NULL;

    switch (msg_id) {
        case RFX_MSG_REQUEST_SET_RADIO_CAPABILITY:
            capability = (RIL_RadioCapability *)message->getData()->getData();
            logI(RFX_LOG_TAG,
                 "RadioCapability version=%d, session=%d, phase=%d, rat=%d, logicMD=%s, status=%d",
                 capability->version, capability->session, capability->phase, capability->rat,
                 capability->logicalModemUuid, capability->status);
            switch (capability->phase) {
                case RC_PHASE_START: {
                    if ((strcmp(capability->logicalModemUuid, "") == 0) &&
                            (capability->rat == 0)) {
                        logI(RFX_LOG_TAG, "invalid modem uuid, return!");
                        responseToRilj(RfxMessage::obtainResponse(
                                       RIL_E_INVALID_ARGUMENTS, message, false));
                        return true;
                    }
                    m_request_count = 0;
                    m_max_capability = 0;
                    m_new_main_slot = 0;
                    m_new_main_slot_by_modem_id = -1;
                    m_is_started = true;
                    responseToRilj(RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true));
                    return true;
                }
                case RC_PHASE_APPLY: {
                    if (m_is_started == true) {
                        // Legacy three-phases solution
                        m_request_count++;
                        calculateNewMainSlot(capability->rat, message->getSlotId(),
                                             capability->logicalModemUuid);
                        if (m_request_count == RfxRilUtils::rfxGetSimCount()) {
                            m_request_count = 0;
                            rfx_property_set("vendor.ril.rc.session.id1",
                                             String8::format("%d", capability->session).string());
                            if (m_checker_controller->isSkipCapabilitySwitch(m_new_main_slot)) {
                                m_is_started = false;

                                // update state key to broadcast the skipped sim switch
                                getNonSlotScopeStatusManager()->setIntValue(
                                        RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,
                                        CAPABILITY_SWITCH_STATE_NO_NEED);
                                getNonSlotScopeStatusManager()->setIntValue(
                                        RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,
                                        CAPABILITY_SWITCH_STATE_IDLE);

                                //No need to do, send urc to rilj directly
                                sp<RfxMessage> set_capability_request =
                                        RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
                                responseToRilj(set_capability_request);
                                updateRadioCapability();
                                return true;
                            }
                            m_pending_request = message;
                            handleRequestSwitchCapability();
                        } else {
                            sp<RfxMessage> set_capability_request =
                                    RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
                            responseToRilj(set_capability_request);
                        }
                    } else {
                        // New one-physe solution, only one apply request to new main slot
                        if (!m_checker_controller->isSkipCapabilitySwitch(m_new_main_slot)) {
                            m_pending_request = message;
                            handleRequestSwitchCapability();
                        } else {
                            sp<RfxMessage> set_capability_request =
                                    RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
                            responseToRilj(set_capability_request);
                        }
                    }
                    return true;
                }
                case RC_PHASE_FINISH: {
                    sp<RfxMessage> set_capability_request =
                            RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
                    responseToRilj(set_capability_request);
                    return true;
                }
                default:
                    sp<RfxMessage> set_capability_request =
                            RfxMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS, message, true);
                    responseToRilj(set_capability_request);
                    return true;
            }
            break;
        case RFX_MSG_REQUEST_GET_RADIO_CAPABILITY:
            requestToMcl(message);
            break;
        default:
            break;
    }
    return true;
}

/*
void RtmCapabilitySwitchController::powerOffRadio() {
    m_close_radio_count = 0;
    int radio_capability;
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        radio_capability = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
        if ((radio_capability & RAF_CDMA_GROUP) != 0) {
            resetLock(i, RFX_STATUS_KEY_RADIO_LOCK);
            sp<RfxAction> action0 = new RfxAction1<int>(
                    this, &RtmCapabilitySwitchController::onRequestRadioOffDone, i);
            RtmRadioController* radio_controller =
                    (RtmRadioController *)findController(i, RFX_OBJ_CLASS_INFO(RtmRadioController));
            radio_controller->moduleRequestRadioPower(false, action0, RFOFF_CAUSE_SIM_SWITCH);
        }
    }

    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        radio_capability = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
        if ((radio_capability & RAF_CDMA_GROUP) == 0) {
            resetLock(i, RFX_STATUS_KEY_RADIO_LOCK);
            sp<RfxAction> action0 = new RfxAction1<int>(
                    this, &RtmCapabilitySwitchController::onRequestRadioOffDone, i);
            RtmRadioController* radio_controller =
                    (RtmRadioController *)findController(i, RFX_OBJ_CLASS_INFO(RtmRadioController));
            radio_controller->moduleRequestRadioPower(false, action0, RFOFF_CAUSE_SIM_SWITCH);
        }
    }
}

void RtmCapabilitySwitchController::onRequestRadioOffDone(int slotId) {
    m_close_radio_count++;
    logD(RFX_LOG_TAG, "onRequestRadioOffDone slotid=%d closeRadioCount=%d", slotId,
         m_close_radio_count);
    setLock(slotId, RFX_STATUS_KEY_RADIO_LOCK);
    if (m_close_radio_count == RfxRilUtils::rfxGetSimCount()) {
        m_close_radio_count = 0;
        requestSetMajorSim();
    }
}
*/

void RtmCapabilitySwitchController::requestSetMajorSim() {
    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };

    rfx_property_get("persist.vendor.radio.simswitch", tempstr, "1");
    int current_main_slot = atoi(tempstr) - 1;
    int msg_data[1];
    msg_data[0] = m_new_main_slot;
    sp<RfxMessage> msg = RfxMessage::obtainRequest(
            current_main_slot, RFX_MSG_REQUEST_CAPABILITY_SWITCH_SET_MAJOR_SIM,
            RfxIntsData(msg_data, 1));
    requestToMcl(msg);
}

void RtmCapabilitySwitchController::processSetMajorSimResponse(const sp<RfxMessage> &message) {
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    int session_id;

    rfx_property_get("vendor.ril.rc.session.id1", property_value, "-1");
    session_id = atoi(property_value);
    if (message->getError() != RIL_E_SUCCESS) {
        if (session_id != -1) {
            int msg_data[1];
            msg_data[0] = m_new_main_slot;
            //retry if session hasn't been terminated
            sp<RfxMessage> msg = RfxMessage::obtainRequest(
                    message->getSlotId(), message->getId(), RfxIntsData(msg_data, 1));
            requestToMcl(msg);
        }
    } else {
        int modem_off_state = getLockState(MAX_RFX_SLOT_ID, RFX_STATUS_KEY_MODEM_OFF_STATE);
        if (modem_off_state == MODEM_OFF_BY_SIM_SWITCH) {
            resetLock(MAX_RFX_SLOT_ID, RFX_STATUS_KEY_MODEM_OFF_STATE);
        }
        getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,
                CAPABILITY_SWITCH_STATE_ENDING);
        if (m_pending_request != NULL) {
            sp<RfxMessage> set_capability_request =
                    RfxMessage::obtainResponse(RIL_E_SUCCESS, m_pending_request, true);
            responseToRilj(set_capability_request);
            m_pending_request = NULL;
        }
        rfx_property_set("persist.vendor.radio.pendcapswt", String8::format("%d", -1).string());
        updateRadioCapability();
        notifySetRatDone();
    }
}

bool RtmCapabilitySwitchController::onHandleResponse(const sp<RfxMessage> &message) {
    int msg_id = message->getId();

    logD(RFX_LOG_TAG, "onHandleResponse:%s", idToString(msg_id));
    switch (msg_id) {
        case RFX_MSG_REQUEST_CAPABILITY_SWITCH_SET_MAJOR_SIM: {
            processSetMajorSimResponse(message);
            break;
        }
        case RFX_MSG_REQUEST_GET_RADIO_CAPABILITY: {
            responseToRilj(message);
            break;
        }
        default:
            break;
    }
    return true;
}

bool RtmCapabilitySwitchController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff, int radioState) {
    RFX_UNUSED(radioState);
    int msg_id = message->getId();
    //logD(RFX_LOG_TAG, "onCheckIfRejectMessage, msg_id: %s", idToString(msg_id));
    if (msg_id == RFX_MSG_REQUEST_GET_RADIO_CAPABILITY ||
        (msg_id == RFX_MSG_REQUEST_SET_RADIO_CAPABILITY && !isModemPowerOff) ||
        msg_id == RFX_MSG_REQUEST_CAPABILITY_SWITCH_SET_MAJOR_SIM) {
        return false;
    } else {
        return true;
    }
}

void RtmCapabilitySwitchController::onRildInitDone(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant new_value) {
    logI(RFX_LOG_TAG, "onRildInitDone:%s(%d==>%d)",
         RfxStatusManager::getKeyString(key), old_value.asBool(), new_value.asBool());

    // rild not ready
    if (new_value.asBool() == false) {
        return;
    }

    getNonSlotScopeStatusManager()->unRegisterStatusChanged(
            RFX_STATUS_KEY_RILD_INIT_HANDLER_DONE,
            RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onRildInitDone));

    if (m_checker_controller->isReadyForSwitch(true) && m_is_started == false) {
        checkAndSwitchCapability();
    } else {
        logD(RFX_LOG_TAG, "onRildInitDone, not ready for switch");
        registerStatusKeys();
    }
}

void RtmCapabilitySwitchController::onVolteStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant new_value) {
    int old_state = old_value.asInt();
    int high_priority_slot = m_checker_controller->getHigherPrioritySlot();
    logD(RFX_LOG_TAG, "onVolteStateChanged:%s(%s==>%s), high_priority_slot = %d",
         RfxStatusManager::getKeyString(key), old_value.toString().string(),
         new_value.toString().string(), high_priority_slot);

    if (high_priority_slot >= 0 && old_state != -1) {
        if (m_checker_controller->isReadyForSwitch(true) && m_is_started == false) {
            checkAndSwitchCapability();
        } else {
            logD(RFX_LOG_TAG, "onVolteStateChanged, not ready for switch");
            registerStatusKeys();
        }
    }
}

void RtmCapabilitySwitchController::checkAndSwitchCapability() {
    int high_priority_slot = m_checker_controller->getHigherPrioritySlot();
    logD(RFX_LOG_TAG, "checkAndSwitchCapability, high_priority_slot=%d", high_priority_slot);

    if (high_priority_slot >= 0) {
        m_new_main_slot = high_priority_slot;
    }
    if (m_checker_controller->isSkipCapabilitySwitch(m_new_main_slot)) {
        return;
    }
    handleRequestSwitchCapability();
}

void RtmCapabilitySwitchController::registerStatusKeys() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        getStatusManager(i)->registerStatusChanged(
                RFX_STATUS_KEY_RADIO_LOCK,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->registerStatusChanged(
                RFX_STATUS_KEY_EMERGENCY_MODE,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->registerStatusChanged(
                RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->registerStatusChanged(
                RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->registerStatusChanged(
                RFX_STATUS_KEY_SIM_STATE,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        if(getGeminiMode() == GEMINI_MODE_L_AND_L && RfxRilUtils::rfxGetSimCount() == 2) {
            getStatusManager(i)->registerStatusChanged(
                    RFX_STATUS_KEY_GSM_IMSI, RfxStatusChangeCallback(
                    this, &RtmCapabilitySwitchController::onStatusKeyChanged));
            getStatusManager(i)->registerStatusChanged(
                    RFX_STATUS_KEY_C2K_IMSI, RfxStatusChangeCallback(
                    this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        }
    }
    getNonSlotScopeStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_MODEM_OFF_STATE,
            RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
    getNonSlotScopeStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE,
            RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
    getNonSlotScopeStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,
            RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
}

void RtmCapabilitySwitchController::unregisterStatusKeys() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        getStatusManager(i)->unRegisterStatusChanged(
                RFX_STATUS_KEY_RADIO_LOCK,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->unRegisterStatusChanged(
                RFX_STATUS_KEY_EMERGENCY_MODE,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->unRegisterStatusChanged(
                RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->unRegisterStatusChanged(
                RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        getStatusManager(i)->unRegisterStatusChanged(
                RFX_STATUS_KEY_SIM_STATE,
                RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        if(getGeminiMode() == GEMINI_MODE_L_AND_L && RfxRilUtils::rfxGetSimCount() == 2) {
            getStatusManager(i)->unRegisterStatusChanged(
                    RFX_STATUS_KEY_GSM_IMSI, RfxStatusChangeCallback(
                    this, &RtmCapabilitySwitchController::onStatusKeyChanged));
            getStatusManager(i)->unRegisterStatusChanged(
                    RFX_STATUS_KEY_C2K_IMSI, RfxStatusChangeCallback(
                    this, &RtmCapabilitySwitchController::onStatusKeyChanged));
        }
    }
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(
            RFX_STATUS_KEY_MODEM_OFF_STATE,
            RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE,
            RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,
            RfxStatusChangeCallback(this, &RtmCapabilitySwitchController::onStatusKeyChanged));

}

void RtmCapabilitySwitchController::onStatusKeyChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant new_value) {
    if ((key == RFX_STATUS_KEY_GSM_IMSI) || (key == RFX_STATUS_KEY_C2K_IMSI)) {
        logD(RFX_LOG_TAG, "onStatusKeyChanged:%s", RfxStatusManager::getKeyString(key));
    } else {
        logD(RFX_LOG_TAG, "onStatusKeyChanged:%s(%s==>%s)", RfxStatusManager::getKeyString(key),
                old_value.toString().string(), new_value.toString().string());
    }

    if (m_checker_controller->isReadyForSwitch(true) && m_is_started == false) {
        unregisterStatusKeys();
        checkAndSwitchCapability();
    }
}

/*
//TODO: move to op plug-in
bool RtmCapabilitySwitchController::isSatisfyOperatorRules(int new_major_slot) {
    bool ret = true;
    char optr_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.operator.optr", optr_value, "0");
    if (strcmp(optr_value, OPERATOR_OP01) == 0) {
        ret = isSatisfyOp01Rules(new_major_slot);
    }
    return ret;
}

bool RtmCapabilitySwitchController::isSatisfyOp01Rules(int new_major_slot) {
    bool ret = true;
    bool has_op01_sim = false;
    int insert_status[4] = {0}; // 0: no sim, 1: op01 sim, 2: other sim
    int sim_count = RfxRilUtils::rfxGetSimCount();
    char sim_mode_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.sim.mode", sim_mode_prop, "0");
    int simMode = atoi(sim_mode_prop);

    // get sim card insert status
    for (int i = 0; i < sim_count; i++) {
        int sim_state = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_SIM_STATE);
        if (sim_state == RFX_SIM_STATE_READY && (((simMode >> i) & 1) == 1)) {
            if (RtmCapabilitySwitchUtil::isOp01Sim(getImsi(i))) {
                insert_status[i] = 1;
                has_op01_sim = true;
            } else {
                insert_status[i] = 2;
            }
        } else {
            insert_status[i] = 0;
        }
    }
    logD(RFX_LOG_TAG, "isSatisfyOp01Rules, insert_status=%d,%d, simMode=%d, new_major_slot=%d",
            insert_status[0], insert_status[1], simMode, new_major_slot);

    // switch to non-op01 sim
    if (has_op01_sim && (insert_status[new_major_slot] != 1)) {
        ret = false;
    }
    return ret;
}
*/

void RtmCapabilitySwitchController::handleRequestSwitchCapability() {
    setAllLocks();
    // backupRadioPower();
    // powerOffRadio();
    requestSetMajorSim();
}

void RtmCapabilitySwitchController::setAllLocks(void) {
    setLock(MAX_RFX_SLOT_ID, RFX_STATUS_KEY_MODEM_OFF_STATE);
    setLock(MAX_RFX_SLOT_ID, RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE);
}

void RtmCapabilitySwitchController::setLock(int slot_id, RfxStatusKeyEnum key) {
    switch (key) {
        case RFX_STATUS_KEY_MODEM_OFF_STATE: {
            getNonSlotScopeStatusManager()->setIntValue(key, MODEM_OFF_BY_SIM_SWITCH);
            break;
        }
        case RFX_STATUS_KEY_RADIO_LOCK: {
            if (slot_id >= RFX_SLOT_ID_0 && slot_id < MAX_RFX_SLOT_ID) {
                getStatusManager(slot_id)->setIntValue(key, RADIO_LOCK_BY_SIM_SWITCH);
            }
            break;
        }
        case RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE: {
            getNonSlotScopeStatusManager()->setIntValue(key, CAPABILITY_SWITCH_STATE_START);
            break;
        }

        default:
            break;
    }
}

int RtmCapabilitySwitchController::getLockState(int slot_id, RfxStatusKeyEnum key) {
    int ret = -1;
    switch (key) {
        case RFX_STATUS_KEY_MODEM_OFF_STATE: {
            ret = getNonSlotScopeStatusManager()->getIntValue(
                    key, MODEM_OFF_IN_IDLE);
            break;
        }
        case RFX_STATUS_KEY_RADIO_LOCK: {
            if (slot_id >= RFX_SLOT_ID_0 && slot_id < MAX_RFX_SLOT_ID) {
                ret = getStatusManager(slot_id)->getIntValue(key, RADIO_LOCK_IDLE);
            }
            break;
        }
        case RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE: {
            ret = getNonSlotScopeStatusManager()->getIntValue(key);
            break;
        }

        default:
            break;
    }

    return ret;
}

void RtmCapabilitySwitchController::resetLock(int slot_id, RfxStatusKeyEnum key) {
    switch (key) {
        case RFX_STATUS_KEY_MODEM_OFF_STATE: {
            getNonSlotScopeStatusManager()->setIntValue(key, MODEM_OFF_IN_IDLE);
            break;
        }
        case RFX_STATUS_KEY_RADIO_LOCK: {
            if (slot_id >= RFX_SLOT_ID_0 && slot_id < MAX_RFX_SLOT_ID) {
                getStatusManager(slot_id)->setIntValue(key, RADIO_LOCK_IDLE);
            }
            break;
        }
        case RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE: {
            getNonSlotScopeStatusManager()->setIntValue(key, CAPABILITY_SWITCH_STATE_IDLE);
            break;
        }

        default:
            break;
    }
}

/*
void RtmCapabilitySwitchController::backupRadioPower() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        m_backup_radio_power[i] = getStatusManager(i)->getBoolValue(
                RFX_STATUS_KEY_REQUEST_RADIO_POWER, false);
        logD(RFX_LOG_TAG, "m_backup_radio_power[%d]=%d", i, m_backup_radio_power[i]);
    }
}

void RtmCapabilitySwitchController::powerOnRadio() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        RadioPowerLock radio_lock = (RadioPowerLock) getLockState(i, RFX_STATUS_KEY_RADIO_LOCK);
        logD(RFX_LOG_TAG, "powerOnRadio[%d]:%d,radio_lock:%d,start:%d", i, m_backup_radio_power[i],
             radio_lock, m_is_started);
        if (radio_lock == RADIO_LOCK_BY_SIM_SWITCH) {
            resetLock(i, RFX_STATUS_KEY_RADIO_LOCK);
        }

        if (m_is_started == false && m_backup_radio_power[i] == true) {
            // Three-phase solution will power on radio from Java
            RtmRadioController* radio_controller = (RtmRadioController *)findController(
                    i, RFX_OBJ_CLASS_INFO(RtmRadioController));
            radio_controller->moduleRequestRadioPower(m_backup_radio_power[i], NULL,
                                                      RFOFF_CAUSE_SIM_SWITCH);
        }

    }
}
*/

void RtmCapabilitySwitchController::notifySetRatDone() {
    int state = getLockState(MAX_RFX_SLOT_ID, RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE);
    logD(RFX_LOG_TAG, "notifySetRatDone, state: %d", state);
    if (state == CAPABILITY_SWITCH_STATE_ENDING) {
        getNonSlotScopeStatusManager()->setIntValue(
                RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE, CAPABILITY_SWITCH_STATE_IDLE);
        resetLock(MAX_RFX_SLOT_ID, RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE);
        m_is_started = false;
    }

    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        RadioPowerLock radio_lock = (RadioPowerLock) getLockState(i, RFX_STATUS_KEY_RADIO_LOCK);
        if (radio_lock == RADIO_LOCK_BY_SIM_SWITCH) {
            resetLock(i, RFX_STATUS_KEY_RADIO_LOCK);
        }
    }
}

void RtmCapabilitySwitchController::updateRadioCapability() {
    RtmCapabilityGetController* get_controller = (RtmCapabilityGetController *)
            findController(RFX_OBJ_CLASS_INFO(RtmCapabilityGetController));
    if (get_controller != NULL) {
        char temp[RFX_PROPERTY_VALUE_MAX] = { 0 };
        memset(temp, 0, sizeof(temp));
        rfx_property_get("persist.vendor.radio.c_capability_slot", temp, "1");
        int cslot = atoi(temp) - 1;
        get_controller->updateRadioCapability(cslot);
    }
}
