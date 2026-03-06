/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2020. All rights reserved.
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
#include "RtcEccController.h"
#include <libmtkrilutils.h>
#include "RfxLog.h"
#include "ratconfig.h"
#include "rfx_properties.h"

#include "RfxStatusDefs.h"
#include "RfxStatusManager.h"
#include "RfxRilUtils.h"

#include "RtcEccNumberController.h"

#include "sim/RtcCommSimController.h"

#define RFX_LOG_TAG "RtcEcc"

/*****************************************************************************
 * Class RtcEccController
 *
 * This controller is to handle ECC related logics, or as an entry to provide
 * better interface and functions for ECC calls.
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcEccController", RtcEccController, RfxController);

bool RtcEccController::mIsBspProject = false;

/**
 * This static variable remembers if the ECC has been dialed once.
 * (a non-slot status, used for BSP projects only)
 */
bool RtcEccController::mIsBspEccDialed = false;

/**
 * This static variable indicate if radio power on for ECC case exist.
 */
bool RtcEccController::mIsRadioOnForEcc = false;

RtcEccController::RtcEccController() :
        mRadioOnForEccTimeoutHandle(NULL) {
}

RtcEccController::~RtcEccController() {
    logD(RFX_LOG_TAG, "~RtcEccController");

}

void RtcEccController::onInit() {
    RfxController::onInit();
    mIsBspProject = isBspFeatureSupported();
    logD(RFX_LOG_TAG, "onInit !!%s", (mIsBspProject ? " (bsp)" : ""));

    // prepare status manager access
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        m_status_managers[i]  = getStatusManager(i);
        if (m_status_managers[i] == NULL) {
            logE(RFX_LOG_TAG, "init error - prepare status manager NULL (slot: %d)", i);
        }
        m_ecc_num_controller[i] = getEccNumberController(i);
        if (m_ecc_num_controller[i] == NULL) {
            logE(RFX_LOG_TAG, "init error - prepare ecc number ctrl. NULL (slot: %d)", i);
        }
    }
    m_status_managers[MAX_RFX_SLOT_ID] = getNonSlotScopeStatusManager();
    if (m_status_managers[MAX_RFX_SLOT_ID] == NULL) {
        logE(RFX_LOG_TAG, "init error - prepare non-slot status manager NULL");
    }

    if (mIsBspProject) {
        // register callbacks to get indication when Ecc select phone updated.
        m_status_managers[MAX_RFX_SLOT_ID]->registerStatusChanged(
                RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT,
                RfxStatusChangeCallback(this, &RtcEccController::onBspEccSelectPhoneChanged));
    }
}

void RtcEccController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    RfxController::onDeinit();
}

bool RtcEccController::isBspFeatureSupported() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("ro.vendor.mtk_telephony_add_on_policy", prop, "");

    if (strncmp(prop, "0", 1) == 0)
        return true;
    else
        return false;
}


int RtcEccController::getEccPreferredRat() {
    // prefRat  1:GSM_ONLY, 2:CDMA_ONLY, 3:GSM_PREF, 4:CDMA_PREF
    int prefRat = getStatusManager()->
            getIntValue(RFX_STATUS_KEY_ECC_PREFERRED_RAT, 0);
    return prefRat;
}

void RtcEccController::setEccPreferredRat(int eccPreferRat) {
    int currentRat = getEccPreferredRat();
    if (currentRat != eccPreferRat) {
        logI(RFX_LOG_TAG, "%s, prefer rat changed (%d -> %d)", __FUNCTION__,
                        currentRat, eccPreferRat);
        getStatusManager()->setIntValue(RFX_STATUS_KEY_ECC_PREFERRED_RAT, eccPreferRat);
    } else {
        logD(RFX_LOG_TAG, "%s, prefer rat unchanged (%d)", __FUNCTION__,
                                currentRat);
    }
}

bool RtcEccController::checkRequestMsgParam(const sp<RfxMessage>& message) {
    int msg_id = (message != NULL ? message->getId() : -1);
    int msg_slot_id = (message != NULL ? message->getSlotId() : -1);

    if (message == NULL || msg_id == -1 || msg_slot_id == -1) {
        logE(RFX_LOG_TAG, "%s invalid param - message %s(%d, %d)", __FUNCTION__,
                (message == NULL ? "NULL " : ""), msg_id, msg_slot_id);
        return false;
    }

    return true;
}

RIL_Dial *RtcEccController::getDialData(const sp<RfxMessage>& message) {
    if (checkRequestMsgParam(message) == false) {
        logE(RFX_LOG_TAG, "%s check param failed.", __FUNCTION__);
        return NULL;
    }
    RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
    return pDial;
}

/**
 * Function "onPreLocalEmergencyDial"
 *
 * This function provides ECC calls to do handling before emergency dial sent to MCL.
 */
void RtcEccController::onPreLocalEmergencyDial(const sp<RfxMessage>& message) {
    if (checkRequestMsgParam(message) == false) {
        logE(RFX_LOG_TAG, "%s check param failed", __FUNCTION__);
        return;
    }
    int msg_id = message->getId();

    if (mIsBspProject) {
        logD(RFX_LOG_TAG, "%s bsp feature supported.", __FUNCTION__);

        if (getBspEccSelectPhone() == -1) {
            updateBspEccSelectPhone(m_slot_id);
        }

        if (msg_id == RFX_MSG_REQUEST_DIAL ||
                msg_id == RFX_MSG_REQUEST_IMS_DIAL) {
            logD(RFX_LOG_TAG, "%s set ecc prefer rat auto for ecc normal dial.", __FUNCTION__);
            setEccPreferredRat(ECC_PREFER_RAT_AUTO);
        } else {
            calculateBspEccPreferRat(message,
                    (msg_id == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL));
        }
    }
}

/**
 * Function "onPostLocalEmergencyDial"
 *
 * This function provides ECC calls to do handling after emergency dial sent to MCL.
 */
void RtcEccController::onPostLocalEmergencyDial(const sp<RfxMessage>& message) {
    if (checkRequestMsgParam(message) == false) {
        logE(RFX_LOG_TAG, "%s check param failed", __FUNCTION__);
        return;
    }
    if (mIsBspProject) {
        logD(RFX_LOG_TAG, "%s bsp feature supported.", __FUNCTION__);

        setBspEccDialed(true);
    }
}

/**
 * Function "onEccActive"
 *
 * This function provides ECC calls to do handling when emergency call
 * become active.
 */
void RtcEccController::onEccActive() {
    if (mIsBspProject) {
        resetBspEccSelectPhone(); // reset Ecc phone selection
    }
}

/**
 * Function "onEccHangupLocal"
 *
 * This function provides ECC calls to do handling before emergency call
 * being hangup locally.
 */
void RtcEccController::onEccHangupLocal() {
    if (mIsBspProject) {
        resetBspEccSelectPhone(); // reset Ecc phone selection
    }
}

RtcEccNumberController *RtcEccController::getEccNumberController() {
    return getEccNumberController(m_slot_id);
}
RtcEccNumberController *RtcEccController::getEccNumberController(int slotId) {
    RtcEccNumberController *eccNumCtrl = (RtcEccNumberController *)findController(
                    getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));
    if (eccNumCtrl == NULL) {
        logE(RFX_LOG_TAG, "[%d] cannot get ecc number controller (null)", slotId);
    }
    return eccNumCtrl;
}

#define MCC_CHAR_LEN 3
#define CTA_MCC      "460"

bool RtcEccController::isCTAMccExist() {
    bool result = false;
    int simCount = RfxRilUtils::rfxGetSimCount();
    for (int i = 0; i < simCount; i++) {
        if (m_ecc_num_controller[i] == NULL) {
            logD(RFX_LOG_TAG, "%s ecc num controller null (%d)", __FUNCTION__, i);
            continue;
        }
        String8 plmn = m_ecc_num_controller[i]->getPlmn(i);
        // Check if there is CTA MCC in other slot
        char currentMcc[MCC_CHAR_LEN + 1] = {0};
        strncpy(currentMcc, plmn.string(), MCC_CHAR_LEN);
        currentMcc[MCC_CHAR_LEN] = '\0';
        if (strcmp(currentMcc, CTA_MCC) == 0) {
            result = true;
        }
    }
    logD(RFX_LOG_TAG, "%s %s", __FUNCTION__, (result ? "true" : "false"));
    return result;
}

int RtcEccController::getCtVolteSupportType() {
    char ctVolteSupport[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.mtk_ct_volte_support", ctVolteSupport, "0");
    int type = atoi(ctVolteSupport);
    if (type<0 || type>3) {
        logD(RFX_LOG_TAG, "%s invalid ct volte type: %d", __FUNCTION__, type);
        return -1;
    }
    return type;
}

bool RtcEccController::isVolteEnabled() {
    int volteState = getStatusManager()->getIntValue(RFX_STATUS_KEY_VOLTE_STATE, -1);
    logD(RFX_LOG_TAG, "%s volte state: %d", __FUNCTION__, volteState);
    return volteState == 1;
}

/// M: CC: ECC prefer RAT related utils @{
bool RtcEccController::isGsmOnlyNumber(String8 number) {
    return isGsmOnlyNumber(m_slot_id, number);
}

bool RtcEccController::isGsmOnlyNumber(int slotId, String8 number) {
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    return (m_ecc_num_controller[slotId] != NULL ?
            m_ecc_num_controller[slotId]->isGsmOnlyNumber(number) : false);
}

bool RtcEccController::isGsmPreferredNumber(String8 number) {
    return isGsmPreferredNumber(m_slot_id, number);
}

bool RtcEccController::isGsmPreferredNumber(int slotId, String8 number) {
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    return (m_ecc_num_controller[slotId] != NULL ?
            m_ecc_num_controller[slotId]->isGsmPreferredNumber(number) : false);
}

bool RtcEccController::isCdmaPreferredNumber(String8 number) {
    return isCdmaPreferredNumber(m_slot_id, number);
}

bool RtcEccController::isCdmaPreferredNumber(int slotId, String8 number) {
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    return (m_ecc_num_controller[slotId] != NULL ?
            m_ecc_num_controller[slotId]->isCdmaPreferredNumber(number) : false);
}

int RtcEccController::getMainCapabilitySlot() {
    int mainSlot = m_status_managers[MAX_RFX_SLOT_ID]->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    return mainSlot;
}

int RtcEccController::getCPhoneSlotId() {
    int foundCPhone = -1;
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (isCPhone(i)) {
            foundCPhone = i;
            break;
        }
    }
    return foundCPhone;
}

bool RtcEccController::hasC2kRaf() {
    return hasC2kRaf(m_slot_id);
}

bool RtcEccController::hasC2kRaf(int slotId) {
    char temp_str[RFX_PROPERTY_VALUE_MAX] = {0};
    int c_slot = -1;
    memset(temp_str, 0, sizeof(temp_str));
    rfx_property_get("persist.vendor.radio.c_capability_slot", temp_str, "1");
    c_slot = atoi(temp_str) - 1;
    return (c_slot == slotId);
}

bool RtcEccController::isGCDualPhoneExist() {
    int foundCPhone = -1;
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (isCPhone(i)) {
            foundCPhone = i;
            break;
        }
    }
    if (foundCPhone != -1) {
        return true;
    } else {
        return false;
    }
}

bool RtcEccController::isCPhone() {
    return isCPhone(m_slot_id);
}

bool RtcEccController::isCPhone(int slotId) {
    bool isCPhone = false;
    NwsMode nwMode = NWS_MODE_CSFB; // set default G phone
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    nwMode = getNWSelectionMode(slotId);
    if (nwMode == NWS_MODE_CDMALTE) {
        isCPhone = true;
    }
    return isCPhone;
}

bool RtcEccController::isGPhone() {
    return isGPhone(m_slot_id);
}

bool RtcEccController::isGPhone(int slotId) {
    bool isGPhone = true;
    NwsMode nwMode = NWS_MODE_CSFB; // set default G phone
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    nwMode = getNWSelectionMode(slotId);
    if (nwMode == NWS_MODE_CDMALTE) {
        isGPhone = false;
    }
    return isGPhone;
}


NwsMode RtcEccController::getNWSelectionMode() {
    return getNWSelectionMode(m_slot_id);
}

NwsMode RtcEccController::getNWSelectionMode(int slotId) {
    NwsMode nwMode = NWS_MODE_UNKNOWN;
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return NWS_MODE_UNKNOWN;
    }

    nwMode = (NwsMode) m_status_managers[slotId]->
            getIntValue(RFX_STATUS_KEY_NWS_MODE, NWS_MODE_CSFB);
    return nwMode;
}

bool RtcEccController::isAllSimInsert() {
    bool result = true;
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (hasNoSim(i)) {
            result = false;
            break;
        }
    }
    return result;
}

bool RtcEccController::isAllNoSimInsert() {
    bool result = true;
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (!hasNoSim(i)) {
            result = false;
            break;
        }
    }
    return result;
}

bool RtcEccController::hasNoSim() {
    return hasNoSim(m_slot_id);
}

bool RtcEccController::hasNoSim(int slotId) {
    bool result = false;
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    if (getSimState(slotId) == RFX_SIM_STATE_ABSENT ||
            getSimState(slotId) == RFX_SIM_STATE_UNKNOWN) {
        result = true;
    }
    return result;
}

bool RtcEccController::hasLockedSim() {
    return hasLockedSim(m_slot_id);
}

bool RtcEccController::hasLockedSim(int slotId) {
    bool result = false;
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    int simState = getSimState(slotId);
    if (simState != RFX_SIM_STATE_ABSENT
            && simState != RFX_SIM_STATE_UNKNOWN
            && (simState == RFX_SIM_STATE_PIN_REQUIRED
                    || simState == RFX_SIM_STATE_PUK_REQUIRED
                    || simState == RFX_SIM_STATE_NETWORK_LOCKED
                    || simState == RFX_SIM_STATE_PERM_DISABLED)) {
        result = true;
    }
    return result;
}

bool RtcEccController::hasUnlockedSim() {
    return hasUnlockedSim(m_slot_id);
}

bool RtcEccController::hasUnlockedSim(int slotId) {
    bool result = false;
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    int simState = getSimState(slotId);
    if (simState != RFX_SIM_STATE_ABSENT
            && simState != RFX_SIM_STATE_UNKNOWN
            && simState != RFX_SIM_STATE_PIN_REQUIRED
            && simState != RFX_SIM_STATE_PUK_REQUIRED
            && simState != RFX_SIM_STATE_NETWORK_LOCKED
            && simState != RFX_SIM_STATE_PERM_DISABLED) {
        result = true;
    }
    return result;
}

bool RtcEccController::hasDisabledSim() {
    logI(RFX_LOG_TAG, "%s, slot id: %d, status: %d, %d", __FUNCTION__, m_slot_id,
            getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE),
            getStatusManager()->getIntValue(RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE));
    return getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
                    == SIM_POWER_STATE_SIM_OFF
            || getStatusManager()->getIntValue(RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
                    == UICC_APPLICATIONS_ENABLE_STATE_DISABLED;
}

bool RtcEccController::hasLockedOrDisabledSim() {
    return hasLockedSim() || hasDisabledSim();
}

bool RtcEccController::hasNonCdmaSim() {
    return hasNonCdmaSim(m_slot_id);
}
bool RtcEccController::hasNonCdmaSim(int slotId) {
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    if (hasNoSim(slotId)) {
        logD(RFX_LOG_TAG, "[%d] %s, false (no sim insert)", slotId, __FUNCTION__);
        return false;
    }
    if (hasCdmaSim(slotId)) {
        return false;
    }
    return true;
}

bool RtcEccController::hasCdmaSim() {
    return hasCdmaSim(m_slot_id);
}

bool RtcEccController::hasCdmaSim(int slotId) {
    bool result = false;
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    if (hasNoSim(slotId)) {
        logD(RFX_LOG_TAG, "[%d] %s, false (no sim insert)", slotId, __FUNCTION__);
        return false;
    }

    int cardType = m_status_managers[slotId]->getIntValue(RFX_STATUS_KEY_CARD_TYPE, 0);
    if (m_status_managers[slotId]->getBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD)) {
        logD(RFX_LOG_TAG, "%s, [%d] true (CT3G dual mode sim)", __FUNCTION__, slotId);
        result = true;
    } else {
        if (cardType == RFX_CARD_TYPE_RUIM) {
            logD(RFX_LOG_TAG, "%s, [%d] true (Cdma only sim)", __FUNCTION__, slotId);
            result = true;
        } else if (RFX_FLAG_HAS_ALL(cardType, (RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_CSIM))
                || RFX_FLAG_HAS_ALL(cardType, (RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_RUIM))) {
            logD(RFX_LOG_TAG, "%s, [%d] true (Cdma dual mode sim)", __FUNCTION__, slotId);
            result = true;
        }
    }
    if (result == false) {
        logD(RFX_LOG_TAG, "%s, [%d] false (cardType: %d)", __FUNCTION__, slotId, cardType);
    }
    return result;
}

int RtcEccController::getSimState() {
    return getSimState(m_slot_id);
}

int RtcEccController::getSimState(int slotId) {
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return RFX_SIM_STATE_UNKNOWN;
    }
    int simState = m_status_managers[slotId]->getIntValue(RFX_STATUS_KEY_SIM_STATE, 0);
    return simState;
}

bool RtcEccController::isAllSlotInService() {
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (!isInService(i)) {
            logD(RFX_LOG_TAG, "%s, false (slot %d not in service)", __FUNCTION__, i);
            return false;
        }
    }
    return true;
}

bool RtcEccController::isGPhoneInService() {
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (isGPhone(i) && isInService(i)) {
            logD(RFX_LOG_TAG, "%s, true (slot %d in service)", __FUNCTION__, i);
            return true;
        }
    }
    return false;
}

bool RtcEccController::isInService() {
    return isInService(m_slot_id);
}

bool RtcEccController::isInService(int slotId) {
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }

    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState ss = m_status_managers[slotId]
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    int voiceRegState = ss.getRilVoiceRegState();
    int imsRegState = m_status_managers[slotId]->getIntValue(
            RFX_STATUS_KEY_IMS_REGISTRATION_STATE, 0);
    logV(RFX_LOG_TAG, "%s, voiceRegState:%d, imsRegState:%d", __FUNCTION__, voiceRegState,
            imsRegState);

    if (voiceRegState == RIL_REG_STATE_HOME || voiceRegState == RIL_REG_STATE_ROAMING
            || (imsRegState & 1) != 0) {
        logV(RFX_LOG_TAG, "%s, true", __FUNCTION__);
        return true;
    } else {
        logV(RFX_LOG_TAG, "%s, false", __FUNCTION__);
        return false;
    }
}

bool RtcEccController::isOneSlotEmergencyNumber(String8 number) {
    int count = 0;
    for (int i = RFX_SLOT_ID_0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (m_ecc_num_controller[i]->isEmergencyNumber(number)) {
            count++;
        }
    }
    return (count == 1);
}

bool RtcEccController::isSlotEmergencyNumber(String8 number) {
    return isSlotEmergencyNumber(m_slot_id, number);
}

bool RtcEccController::isSlotEmergencyNumber(int slotId, String8 number) {
    if (slotId < RFX_SLOT_ID_0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "%s, invalid slot id: %d", __FUNCTION__, slotId);
        return false;
    }
    return m_ecc_num_controller[slotId]->isEmergencyNumber(number);
}
/// @}

/// M: CC: BSP project ECC retry status @{
bool RtcEccController::isInBspEccRetry() {
    return (mIsBspProject && mIsBspEccDialed);
}

void RtcEccController::setBspEccDialed(bool isDialed) {
    if (!mIsBspEccDialed && isDialed) {
        logD(RFX_LOG_TAG, "static bsp ecc dial state set true", m_slot_id);
        mIsBspEccDialed = true;
    } else if (mIsBspEccDialed && !isDialed){
        logD(RFX_LOG_TAG, "static bsp ecc dial state reset", m_slot_id);
        mIsBspEccDialed = false;
    }
}
/// @}

/// M: CC: BSP project ECC select phone control @{
/**
 * Function "updateBspEccSelectPhone"
 *
 * This function updates selected slot for emergency call.
 * This feature is supported in BSP project currently.
 *
 * Related non-slot status manager key:
 *     RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT
 */
void RtcEccController::updateBspEccSelectPhone(int selectSlot) {
    if ((selectSlot >= 0 && selectSlot < RfxRilUtils::rfxGetSimCount()) || selectSlot == -1) {
        int currSelectSlot = m_status_managers[MAX_RFX_SLOT_ID]->getIntValue(
                RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT, -1);
        if (currSelectSlot == selectSlot) {
            if (selectSlot != -1) {
                logD(RFX_LOG_TAG, "dup update ECC phone selection req. (%d)",
                        currSelectSlot);
            }
        } else {
            m_status_managers[MAX_RFX_SLOT_ID]->setIntValue(
                    RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT, selectSlot);
            logD(RFX_LOG_TAG, "%s ECC phone selection slot (%d -> %d)",
                    (selectSlot != -1 ? "update" : "reset"),
                    currSelectSlot, selectSlot);
        }
    }
}

void RtcEccController::resetBspEccSelectPhone() {
    updateBspEccSelectPhone(-1);
}

int RtcEccController::getBspEccSelectPhone() {
    int currSelectSlot = m_status_managers[MAX_RFX_SLOT_ID]->getIntValue(
            RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT, -1);
    return currSelectSlot;
}

void RtcEccController::onBspEccSelectPhoneChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    RFX_UNUSED(oldValue);
    if (newValue.asInt() == -1) {
        setBspEccDialed(false);
    }
}

/// M: CC: radio power for ecc status indication handling @{
bool RtcEccController::isInRadioOnForEcc() {
    return mIsRadioOnForEcc;
}

void RtcEccController::onDummyCallback() {
    logD(RFX_LOG_TAG, "onDummyCallback");
}

void RtcEccController::setRadioOnForEcc(bool forEcc) {
    // any timeout handler set previously would be stopped when status changed.
    setRadioOnForEcc(forEcc,
            RfxCallback0(this, &RtcEccController::onDummyCallback), 0);
}

// handle ecc controller internal flags and timeout thread.
void RtcEccController::setRadioOnForEcc(bool forEcc,
        const RfxCallback0 &timeoutHandler, long int timeoutMs) {
    logD(RFX_LOG_TAG, "setRadioOnForEcc, mIsRadioOnForEcc=%s, forEcc=%s, %s",
            (mIsRadioOnForEcc ? "t" : "f"),
            (forEcc ? "t" : "f"), (timeoutMs > 0 ? "t" : "f"));
    if (mIsRadioOnForEcc == forEcc) {
        logD(RFX_LOG_TAG, "radio on for ecc state unchanged (%s), return",
                (forEcc ? "t" : "f"));
        return;
    }

    if (mRadioOnForEccTimeoutHandle != NULL) {
        logD(RFX_LOG_TAG, "radio on for ecc timeout handle re-cycle");
        RfxTimer::stop(mRadioOnForEccTimeoutHandle);
        mRadioOnForEccTimeoutHandle = NULL;
    }

    if (!mIsRadioOnForEcc && forEcc) {
        mIsRadioOnForEcc = true;
    } else if (mIsRadioOnForEcc && !forEcc){
        mIsRadioOnForEcc = false;
    }

    if (mIsRadioOnForEcc && timeoutMs > 0) {
        logD(RFX_LOG_TAG, "radio on for ecc timeout handle starts (%li)", timeoutMs);
        mRadioOnForEccTimeoutHandle = RfxTimer::start(timeoutHandler, ms2ns(timeoutMs));
    }
}

int RtcEccController::calculateBspEccPreferRat(const sp<RfxMessage>& message, bool isAuto) {
    int calculatedRat = -1;
    bool bIsGsmOnlyNum = false;
    bool bIsGsmPreferredNum = false;
    bool bIsCdmaPreferredNum = false;
    bool bIsGPhone = false;
    bool bIsCPhone = false;

    if (!mIsBspProject) {
        logD(RFX_LOG_TAG, "%s not bsp project, no prefer ECC rat, return.", __FUNCTION__);
        return calculatedRat;
    }

    if (!RatConfig_isC2kSupported()) {
        logD(RFX_LOG_TAG, "%s isC2kSupported false. Skip.", __FUNCTION__);
    } else {
        RIL_Dial *pEccDial = getDialData(message);
        if (pEccDial == NULL) {
            logE(RFX_LOG_TAG, "%s cannot get dial data. Skip.", __FUNCTION__);
            return -1;
        }
        String8 dialNumber = String8::format("%s", pEccDial->address);

        // query ecc number preference
        if (isGsmOnlyNumber(dialNumber)) {
            bIsGsmOnlyNum = true;
        } else if (isGsmPreferredNumber(dialNumber)) {
            bIsGsmPreferredNum = true;
        } else if (isCdmaPreferredNumber(dialNumber)) {
            bIsCdmaPreferredNum = true;
        }
        bIsGPhone = isGPhone();
        bIsCPhone = isCPhone();

        logD(RFX_LOG_TAG, "%s dialNumber(%s) (%s, %s) (isAuto=%s)", __FUNCTION__,
                RfxRilUtils::pii(RFX_LOG_TAG, dialNumber.string()),
                (bIsGsmOnlyNum ? "g only" : (bIsGsmPreferredNum ? "g pref." :
                        (bIsCdmaPreferredNum ? "c pref." : "no pref." ))),
                (bIsCPhone ? "C phone" : (bIsGPhone ? "G phone" : "unknown phone type")),
                (isAuto ? "t" : "f"));

        if (isGCDualPhoneExist()) {
            int foundCPhone = getCPhoneSlotId();
            logD(RFX_LOG_TAG, "%s G+C phone exist (c slot: %d)", __FUNCTION__, foundCPhone);
            // case 1~4
            // MainPhoneNoSimGsmRule: Select main phone for GSM w/o SIM
            if (!isInBspEccRetry()
                    && isAllNoSimInsert() && getSlotId() == getMainCapabilitySlot()) {
                if (bIsGsmOnlyNum) {
                    calculatedRat = ECC_PREFER_RAT_GSM_ONLY;
                } else if (bIsGsmPreferredNum) {
                    calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                } else if (bIsCdmaPreferredNum && bIsCPhone) {
                    calculatedRat = ECC_PREFER_RAT_CDMA_PREF;
                }
            }
            // OneSlotEmergencyNumberRule: Select default phone if not emergency number of both slots
            if (calculatedRat == -1) {
                if (isOneSlotEmergencyNumber(dialNumber) && isSlotEmergencyNumber(dialNumber)) {
                    if (bIsGsmOnlyNum) {
                        calculatedRat = ECC_PREFER_RAT_GSM_ONLY;
                    } else if (bIsGsmPreferredNum) {
                        calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                    } else {
                        // No need to handle CDMA preferred case
                        // since 110/119/120/122 must be ECC for both slots when mcc=460
                        calculatedRat = ECC_PREFER_RAT_AUTO;
                    }
                }
            }
            // OnlyNumberRule: Select phone based on only number rule
            if (calculatedRat == -1) {
                if (bIsGsmOnlyNum) {
                    calculatedRat = ECC_PREFER_RAT_GSM_ONLY;
                }
            }
            // EccRetryRule: Select ECC retry phone
            if (calculatedRat == -1) {
                if (isInBspEccRetry()) {
                    if (bIsCPhone && bIsCdmaPreferredNum) {
                        calculatedRat = ECC_PREFER_RAT_CDMA_PREF;
                    } else if (bIsGPhone && bIsGsmPreferredNum) {
                        calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                    } else {
                        calculatedRat = ECC_PREFER_RAT_AUTO;
                    }
                }
            }
            /// Select phone based on GSM/CDMA service state below

            // For flight mode ECC, EmergencyRuleHandler won't find GCReadyRule, GsmReadyOnlyRule
            // and CdmaReadyOnlyRule. Skip them here.
            if (!isInRadioOnForEcc()) {
                // GCReadyRule
                if (calculatedRat == -1) {
                    if (!bIsGsmOnlyNum) {
                        if (isAllSlotInService()) {
                            if (bIsGsmPreferredNum) {
                                if ((getCtVolteSupportType() == 2 && isVolteEnabled()) || bIsGPhone) {
                                    calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                                }
                            } else if (bIsCdmaPreferredNum && bIsCPhone) {
                                calculatedRat = ECC_PREFER_RAT_CDMA_PREF;
                            }
                        }
                    }
                }

                // GsmReadyOnlyRule()
                //   => no special ECC preferred rat setting.
                if (calculatedRat == -1) {
                    if (!bIsGsmOnlyNum) {
                        if (!isInService(foundCPhone) && isGPhoneInService() && bIsGPhone) {
                            calculatedRat = ECC_PREFER_RAT_AUTO;
                        }
                    }
                }

                // CdmaReadyOnlyRule()
                if (calculatedRat == -1) {
                    if (!bIsGsmOnlyNum) {
                        if (isInService(foundCPhone) && !isGPhoneInService()) {
                            if (getCtVolteSupportType() != 0 && isVolteEnabled() && bIsCPhone) {
                                if (bIsCdmaPreferredNum) {
                                    calculatedRat = ECC_PREFER_RAT_CDMA_PREF;
                                } else if (bIsGsmPreferredNum) {
                                    calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                                }
                            } else {
                                calculatedRat = ECC_PREFER_RAT_AUTO;
                            }
                        }
                    }
                }
            }

            // GCUnReadyRule()
            if (calculatedRat == -1) {
                if (!bIsGsmOnlyNum) {
                    if ((!isInService(foundCPhone) && !isGPhoneInService())
                            || isInRadioOnForEcc()) {
                        if (bIsGsmPreferredNum) {
                            if (getCtVolteSupportType() == 2 && isVolteEnabled()) {
                                calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                            } else if (bIsGPhone) {
                                calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                            }
                        } else if (bIsCdmaPreferredNum) {
                            if (bIsGPhone && isCTAMccExist() && !hasCdmaSim()
                                    && !hasNoSim() && !hasLockedOrDisabledSim()) {
                                // skip
                            } else if (bIsCPhone) {
                                calculatedRat = ECC_PREFER_RAT_CDMA_PREF;
                            }
                        }

                    }
                }
            }

            // else keep auto

        } else {
            if (isInBspEccRetry()) {
                if (bIsGPhone && (bIsGsmOnlyNum || bIsGsmPreferredNum)) {
                    calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                }
            } else {
                // case 5~7
                // Cannot switch C capability when SIM inserted, no matter in service or not
                if (isAllSimInsert()) {
                    if (bIsGsmOnlyNum) {
                        calculatedRat = ECC_PREFER_RAT_GSM_ONLY;
                    } else if (bIsGsmPreferredNum) {
                        calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                    } else if (bIsCdmaPreferredNum) {
                        if (isCTAMccExist()) {
                            if (isInService(m_slot_id)) {
                                logD(RFX_LOG_TAG,
                                        "%s for G+C project w/o G+C phone,allSimInserted,found in-service phone.",
                                        __FUNCTION__);
                            } else if (hasLockedOrDisabledSim() && hasC2kRaf()) {
                                logD(RFX_LOG_TAG,
                                        "%s for G+C project w/o G+C phone,allSimInserted,found C2k RAF slot with locked sim.",
                                        __FUNCTION__);
                                calculatedRat = ECC_PREFER_RAT_CDMA_PREF;
                            } else if (hasLockedOrDisabledSim()) {
                                logD(RFX_LOG_TAG, "%s phone locked, loc plmn not 460.",
                                        __FUNCTION__);
                            }
                        }
                    }
                } else {
                    if (bIsGsmOnlyNum) {
                        calculatedRat = ECC_PREFER_RAT_GSM_ONLY;
                    } else if (bIsCdmaPreferredNum) {
                        if (isCTAMccExist() && !hasCdmaSim() && !hasNoSim()
                                && !hasLockedOrDisabledSim()) {
                            // 460, not C card, no SIM lock, not disabled
                            // don't set. keep auto.
                        } else {
                            calculatedRat = ECC_PREFER_RAT_CDMA_PREF;
                        }
                    } else if (bIsGPhone && bIsGsmPreferredNum) {
                        calculatedRat = ECC_PREFER_RAT_GSM_PREF;
                    }
                    // else keep auto
                }
            }
        }
    }

    if (calculatedRat == -1) {
        calculatedRat = ECC_PREFER_RAT_AUTO;
    }
    logI(RFX_LOG_TAG, "%s, prefer rat calculated: %d", __FUNCTION__, calculatedRat);
    setEccPreferredRat(calculatedRat);
    return calculatedRat;
}
/// @}
