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
#include "RtmEccNumberController.h"
#include "RfxAtLine.h"
#include "tinyxml.h"
#include <libmtkrilutils.h>
#include "RfxEmergencyNumberListData.h"
#include "RfxRilUtils.h"

#define RFX_LOG_TAG "RtmEccNumberController"

#define MAX_PROP_CHARS       92
#define MCC_CHAR_LEN         3
#define PLMN_CHAR_LEN        6
#define ESIMS_CAUSE_RECOVERY 14
#define MAX_ECC_NUM          16
#define MAX_ECC_BUF_SIZE     (MAX_ECC_NUM * 8 + 1)
#define MAX_PRINT_CHAR       2

#define DELAY_SET_FWK_READY_TIMER 5000 //5s

/*****************************************************************************
 * Class RtmEccNumberController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtmEccNumberController", RtmEccNumberController, RfxController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_ECC_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_GET_ECC_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEmergencyNumberListData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_SYNC_ECC_NUM_TO_MD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_GET_ECC_NUM_FROM_MD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_HANDLE_SCREEN_STATE_CHANGE);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_UNSOL_ECC_NUM);
RFX_REGISTER_DATA_TO_URC_ID(RfxEmergencyNumberListData, RFX_MSG_URC_EMERGENCY_NUMBER_LIST);

RtmEccNumberController::RtmEccNumberController() :
        mIsSimInsert(false),
        mXmlEccSource(NULL),
        mFrameworkEccSource(NULL),
        mPropertyEccSource(NULL),
        mTestEccSource(NULL),
        mModemEccSource(NULL),
        mIsFwkReady(false),
        mIsPendingUpdate(false),
        mTimerHandle(NULL),
        mEccPreference(NULL) {
}

RtmEccNumberController::~RtmEccNumberController() {

    for (int i = 0; i < (int)mApEccNumberSourceList.size(); i++) {
        if (mApEccNumberSourceList[i] != NULL) {
            delete(mApEccNumberSourceList[i]);
        }
    }
    mApEccNumberSourceList.clear();
    delete(mModemEccSource);
    delete(mEccPreference);
}

EmergencyCallRouting RtmEccNumberController::getEmergencyCallRouting(String8 number) {
    if (number.isEmpty()) {
        logE(RFX_LOG_TAG, "[%s] Empty number return unknown", __FUNCTION__);
        return ECC_ROUTING_UNKNOWN;
    }

    // Only print first two characters
    char maskNumber[MAX_PRINT_CHAR + 1] = {0};
    strncpy(maskNumber, number.string(), MAX_PRINT_CHAR);
    maskNumber[MAX_PRINT_CHAR] = '\0';

    for (int i = 0; i < (int)mEccList.size(); i++) {
        if (mEccList[i].number == number) {
            // For special/fake emergency number, it should dial using normal call routing when
            // SIM ready, for other cases, it should still dial using emergency routing
            int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
            if ((mEccList[i].condition == CONDITION_MMI) &&
                    (simState == RFX_SIM_STATE_READY)) {
                logI(RFX_LOG_TAG, "[%s] %s* is a special ECC", __FUNCTION__, maskNumber);
                return ECC_ROUTING_NORMAL;
            } else {
                logI(RFX_LOG_TAG, "[%s] %s* is a ECC, condition: %d, simState: %d",
                        __FUNCTION__, maskNumber, mEccList[i].condition, simState);
                return ECC_ROUTING_EMERGENCY;
            }
        }
    }
    logI(RFX_LOG_TAG, "[%s] %s* is not a ECC", __FUNCTION__, maskNumber);
    return ECC_ROUTING_UNKNOWN;
}

bool RtmEccNumberController::isEmergencyNumber(String8 number) {
    if (number.isEmpty()) {
        logE(RFX_LOG_TAG, "[%s] Empty number return false", __FUNCTION__);
        return false;
    }

    // Only print first two characters
    char maskNumber[MAX_PRINT_CHAR + 1] = {0};
    strncpy(maskNumber, number.string(), MAX_PRINT_CHAR);
    maskNumber[MAX_PRINT_CHAR] = '\0';

    for (int i = 0; i < (int)mEccList.size(); i++) {
        if (mEccList[i].number == number) {
            logI(RFX_LOG_TAG, "[%s] %s* is a ECC with condition: %d", __FUNCTION__,
                    maskNumber, mEccList[i].condition);
            return true;
        }
    }
    logI(RFX_LOG_TAG, "[%s] %s* is not a ECC", __FUNCTION__, maskNumber);
    return false;
}

int RtmEccNumberController::getServiceCategory(String8 number) {
    // Only print first two characters
    char maskNumber[MAX_PRINT_CHAR + 1] = {0};
    strncpy(maskNumber, number.string(), MAX_PRINT_CHAR);
    maskNumber[MAX_PRINT_CHAR] = '\0';

    for (int i = 0; i < (int)mEccList.size(); i++) {
        if ((mEccList[i].number == number) &&
                (mEccList[i].categories != ECC_CATEGORY_NOT_DEFINED)) {
            logI(RFX_LOG_TAG, "[%s] %s* category is %d", __FUNCTION__, maskNumber,
                    mEccList[i].categories);
            return mEccList[i].categories;
        }
    }

    logI(RFX_LOG_TAG, "[%s] %s* category is 0", __FUNCTION__, maskNumber);
    return 0;
}

bool RtmEccNumberController::isGsmOnlyNumber(String8 number) {
    if (mEccPreference != NULL) {
        return mEccPreference->isGsmOnlyNumber(number);
    }
    return false;
}

bool RtmEccNumberController::isGsmPreferredNumber(String8 number) {
    if (mEccPreference != NULL) {
        return mEccPreference->isGsmPreferredNumber(number);
    }
    return false;
}

bool RtmEccNumberController::isCdmaPreferredNumber(String8 number) {
    if (mEccPreference != NULL) {
        return mEccPreference->isCdmaPreferredNumber(number);
    }
    return false;
}


void RtmEccNumberController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    logV(RFX_LOG_TAG, "[%s]", __FUNCTION__);

    const int urc_id_list[] = {
        RFX_MSG_URC_EMERGENCY_NUMBER_LIST
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));

    const int requests[] = {
        RFX_MSG_REQUEST_SET_ECC_NUM,
        RFX_MSG_REQUEST_GET_ECC_NUM,
        RFX_MSG_REQUEST_LOCAL_SYNC_ECC_NUM_TO_MD,
        RFX_MSG_REQUEST_LOCAL_GET_ECC_NUM_FROM_MD,
        RFX_MSG_REQUEST_LOCAL_HANDLE_SCREEN_STATE_CHANGE
    };
    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));

    // register callbacks to get card type change event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onCardTypeChanged));

    // register callbacks to get PLMN(MCC,MNC) change event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onPlmnChanged));

    // register callbacks to get PLMN(MCC,MNC) report for emergency call
    // For No SIM case, EOPS may update too later before make emergency call
    // and PLMN specific emergency numbers will not be detected in this case.
    // To enhance the PLMN update flow, modem will update PLMN directly through
    // URC +EOPSU
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_MCCMNC_CHANGED,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onPlmnChanged));

    // register callbacks to get sim state event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SIM_STATE,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onSimStateChanged));

    // register callbacks to get sim state event
    getStatusManager()->registerStatusChanged(RFX_STATUS_CONNECTION_STATE,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onConnectionStateChanged));

    // register callbacks to get GSM UICC MCC/MNC
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_UICC_GSM_NUMERIC,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onGsmUiccMccMncChanged));

    // register callbacks to get CDMA UICC MCC/MNC
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_UICC_CDMA_NUMERIC,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onCdmaUiccMccMncChanged));

    // register callbacks to set ECC list to modem when SIM switch done
    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onCapabilitySwitchStateChanged));

    // register callbacks to listen screen on/off to register/unregister the ECC list indication
    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SCREEN_STATE,
        RfxStatusChangeCallback(this, &RtmEccNumberController::onScreenStateChanged));

    initEmergencyNumberSource();

    mEccPreference = new EccNumberPreference(m_slot_id);
    mEccPreference->update(getPlmn(m_slot_id));
}

void RtmEccNumberController::initEmergencyNumberSource() {
    mApEccNumberSourceList.clear();

    // Init property ECC if property is set
    char eccCount[MAX_PROP_CHARS] = {0};
    rfx_property_get(PROPERTY_ECC_COUNT, eccCount, "0");
    if (atoi(eccCount) > 0) {
        logI(RFX_LOG_TAG, "[%s] OEM property exist, init property source", __FUNCTION__);
        mPropertyEccSource = new OemPropertyEccNumberSource(m_slot_id);
        mApEccNumberSourceList.push_back(mPropertyEccSource);
    }

    // Init XML ECC if XML file is exist
    FILE* file = fopen(ECC_PATH, "r");
    // Try to find a light way to check file exist
    if (file != NULL) {
        logD(RFX_LOG_TAG, "[%s] XML file exist, init XML source", __FUNCTION__);
        mXmlEccSource = new XmlEccNumberSource(m_slot_id);
        mApEccNumberSourceList.push_back(mXmlEccSource);
        if (fclose(file) != 0) {
            logE(RFX_LOG_TAG, "[%s] Close XML file fail", __FUNCTION__);
        }
    }

    mTestEccSource = new TestEccNumberSource(m_slot_id);
    mApEccNumberSourceList.push_back(mTestEccSource);

    mApEccList.clear();

    mModemEccSource = new ModemEccNumberSource(m_slot_id);
    mEccList.clear();
}

void RtmEccNumberController::onCardTypeChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    if (oldValue.asInt() != newValue.asInt()) {
        logI(RFX_LOG_TAG, "[%s] oldValue %d, newValue %d", __FUNCTION__,
                oldValue.asInt(), newValue.asInt());
        mIsSimInsert = newValue.asInt() == 0 ? false : true;
        mTestEccSource->update(mIsSimInsert);
        updateEmergencySourcesForAllSimStateChanges(hasSimInAnySlot());
    }
}

void RtmEccNumberController::onPlmnChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    bool isChange = false;
    logV(RFX_LOG_TAG, "[%s] oldValue %s, newValue %s", __FUNCTION__,
        (const char *)(oldValue.asString8()), (const char *)(newValue.asString8()));

    if (newValue.asString8().length() < MCC_CHAR_LEN) {
        logV(RFX_LOG_TAG, "[%s] MCC length error !", __FUNCTION__);
        return;
    }

    // The sequence of modem report network ecc list and camp on cell is not fixed.
    // So there will be cases when modem report network ecc list (+CEN1/+CEN2) and
    // AP will save the network ECC then before camp on cell, EOPS will be 000000.
    // Thus if AP query PLMN at this time, onMccMncIndication() will be called and
    // previous network ECC saved will be cleared. To fix this timing issue, we'll
    // not reset network ECC when PLMN change to "000000".
    if (strcmp((const char *)newValue.asString8(), "000000") == 0 ||
            strcmp((const char *)newValue.asString8(), "FFFFFF") == 0) {
        logV(RFX_LOG_TAG, "[%s] don't reset before camp on cell", __FUNCTION__);
        return;
    }

    isChange = updateEmergencySourcesForPlmnChange(hasSimInAnySlot());

    if (isChange) {
        syncEccToModem();
    }
}


// Update special emergency number when sim state changed
// For special emergency number:
//     SIM ready: Normal call routing
//     Others: Emergency call routing
void RtmEccNumberController::onSimStateChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);

    if (oldValue.asInt() != newValue.asInt()) {
        logV(RFX_LOG_TAG,"[%s] update special emergency number", __FUNCTION__);
        updateSpecialEmergencyNumber();
    }
}

void RtmEccNumberController::onConnectionStateChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(key);

    logD(RFX_LOG_TAG,"[%s] status: %d, mIsFwkReady: %d, mIsPendingUpdate: %d",
            __FUNCTION__, newValue.asBool(), mIsFwkReady, mIsPendingUpdate);

    if (mTimerHandle != NULL) {
        RfxTimer::stop(mTimerHandle);
        mTimerHandle = NULL;
    }

    if (!newValue.asBool()) {
        mIsFwkReady = false;
    } else {
        // Force update ECC list to framework when RILD/Framework connection
        // changes to 1 no matter from 0->1 or 1->1
        // For phone process die case, the connection status will change from 1->1
        mIsPendingUpdate = true;
        // Delay a timer to notify framework
        logV(RFX_LOG_TAG,"[%s] Delay to sent emergency list", __FUNCTION__);
        mTimerHandle = RfxTimer::start(RfxCallback0(this,
                &RtmEccNumberController::delaySetFwkReady), ms2ns(DELAY_SET_FWK_READY_TIMER));
    }
}

void RtmEccNumberController::onGsmUiccMccMncChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(key);

    logV(RFX_LOG_TAG, "[%s] oldValue %s, newValue %s", __FUNCTION__,
        (const char *)(oldValue.asString8()), (const char *)(newValue.asString8()));

    if (updateEmergencySourcesForPlmnChange(true)) {
        syncEccToModem();
    }
}

void RtmEccNumberController::onCdmaUiccMccMncChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(key);

    logV(RFX_LOG_TAG, "[%s] oldValue %s, newValue %s", __FUNCTION__,
        (const char *)(oldValue.asString8()), (const char *)(newValue.asString8()));

    if (updateEmergencySourcesForPlmnChange(true)) {
        syncEccToModem();
    }
}

void RtmEccNumberController::onCapabilitySwitchStateChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(key);
    if (newValue.asInt() == CAPABILITY_SWITCH_STATE_ENDING) {
        logD(RFX_LOG_TAG, "[%s] SIM switch done, sync ecc list to modem", __FUNCTION__);
        syncEccToModem();
    }
}

void RtmEccNumberController::onScreenStateChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(newValue);
    RFX_UNUSED(key);
    requestToMcl(RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_LOCAL_HANDLE_SCREEN_STATE_CHANGE, RfxVoidData()));
}


void RtmEccNumberController::delaySetFwkReady() {
    logD(RFX_LOG_TAG, "[%s] mIsFwkReady: %d, mIsPendingUpdate: %d", __FUNCTION__,
            mIsFwkReady, mIsPendingUpdate);
    mIsFwkReady = true;
    if (mIsPendingUpdate) {
        updateEccToFramework();
        mIsPendingUpdate = false;
    }
}

bool RtmEccNumberController::onHandleUrc(const sp<RfxMessage>& message) {
    int msgId = message->getId();

    switch (msgId) {
        case RFX_MSG_URC_EMERGENCY_NUMBER_LIST:
            handleModemEccChange(message);
            break;
        default:
            break;
    }

    return true;
}

void RtmEccNumberController::handleModemEccChange(const sp<RfxMessage>& message){
    mEccList.clear();
    logV(RFX_LOG_TAG, "[%s]", __FUNCTION__);
    RIL_EmergencyNumber *pEccList = (RIL_EmergencyNumber*)(message->getData()->getData());
    int eccCount = message->getData()->getDataLength() / sizeof(RIL_EmergencyNumber);
    if (mModemEccSource != NULL) {
        mModemEccSource->update(pEccList, eccCount);
        mModemEccSource->addToEccList(mEccList);
        updateEccToFramework();
    }
}

void RtmEccNumberController::createEmergencyNumberListResponse(RIL_EmergencyNumber *data) {
    if (data == NULL) {
        return;
    }

    for (int i = 0; i < mEccList.size(); i++) {
        asprintf(&(data[i].number), "%s", mEccList[i].number.string());
        // AOSP expect mcc/mnc not NULL
        char currentMcc[MCC_CHAR_LEN + 1] = {0};
        char currentMnc[MCC_CHAR_LEN + 1] = {0};
        String8 plmn = getPlmn(m_slot_id);
        if (plmn.length() > MCC_CHAR_LEN) {
            strncpy(currentMcc, plmn.string(), MCC_CHAR_LEN);
            currentMcc[MCC_CHAR_LEN] = '\0';
            strncpy(currentMnc, plmn.string() + MCC_CHAR_LEN, MCC_CHAR_LEN);
            currentMnc[MCC_CHAR_LEN] = '\0';
        }
        logD(RFX_LOG_TAG, "[%s] plmn:%s,mcc:%s,mnc:%s", __FUNCTION__, plmn.string(),
                currentMcc, currentMnc);
        asprintf(&(data[i].mcc), "%s", currentMcc);
        asprintf(&(data[i].mnc), "%s", currentMnc);
        if (mEccList[i].categories == ECC_CATEGORY_NOT_DEFINED) {
            data[i].categories = 0;
        } else {
            data[i].categories = mEccList[i].categories;
        }
        data[i].urns = NULL; // not used yet
        // Map EmergencyNumberSource to RIL_EmergencyNumberSource (AOSP value)
        // Convert all extended source type to MODEM_CONFIG
        data[i].sources = (mEccList[i].sources & 0x0F) |
                (((mEccList[i].sources & 0xF0) > 0) ? MODEM_CONFIG : 0);
    }
}

void RtmEccNumberController::freeEmergencyNumberListResponse(RIL_EmergencyNumber *data) {
    for (int i = 0; i < mEccList.size(); i++) {
        FREEIF(data[i].number);
        FREEIF(data[i].mcc);
        FREEIF(data[i].mnc);
    }
    FREEIF(data);
}

void RtmEccNumberController::createApEmergencyNumberList(RIL_EmergencyNumber *data) {
    if (data == NULL) {
        return;
    }

    for (int i = 0; i < mApEccList.size(); i++) {
        asprintf(&(data[i].number), "%s", mApEccList[i].number.string());
        asprintf(&(data[i].mcc), "%s", mApEccList[i].mcc.string());
        asprintf(&(data[i].mnc), "%s", mApEccList[i].mnc.string());
        if (mApEccList[i].categories == ECC_CATEGORY_NOT_DEFINED) {
            data[i].categories = 0;
        } else {
            data[i].categories = mApEccList[i].categories;
        }
        data[i].isFake = mApEccList[i].condition == CONDITION_MMI ? 1 : 0;
    }
}

void RtmEccNumberController::freeApEmergencyNumberList(RIL_EmergencyNumber *data) {
    for (int i = 0; i < mApEccList.size(); i++) {
        FREEIF(data[i].number);
        FREEIF(data[i].mcc);
        FREEIF(data[i].mnc);
    }
    FREEIF(data);
}

void RtmEccNumberController::syncEccToModem() {
    mApEccList.clear();
    for (int i = 0; i < (int)mApEccNumberSourceList.size(); i++) {
        mApEccNumberSourceList[i]->addToEccList(mApEccList);
    }

    // Remove non emergency numbers with condition NEVER after add all
    // emergency numbers with different source.
    removeNonEmergencyNumbers();

    dumpApEccList();

    if (mApEccList.size() > 0) {
        int length = mApEccList.size() * sizeof(RIL_EmergencyNumber);
        RIL_EmergencyNumber *pData = (RIL_EmergencyNumber *)calloc(1, length);
        RFX_ASSERT(pData != NULL);
        createApEmergencyNumberList(pData);

        requestToMcl(RfxMessage::obtainRequest(getSlotId(),
                        RFX_MSG_REQUEST_LOCAL_SYNC_ECC_NUM_TO_MD,
                        RfxEmergencyNumberListData(pData, length)));

        freeApEmergencyNumberList(pData);
    } else {
        // need to clear ECC list by set zero size ecc list
        requestToMcl(RfxMessage::obtainRequest(getSlotId(),
                        RFX_MSG_REQUEST_LOCAL_SYNC_ECC_NUM_TO_MD,
                        RfxEmergencyNumberListData(NULL, 0)));
    }
}


void RtmEccNumberController::updateEccToFramework() {
    String8 eccList = String8("");
    for (int i = 0; i < (int)mEccList.size(); i++) {
        // Skip duplicate emergency number
        if (!isEccMatchInList(mEccList[i].number, eccList)) {
            if (eccList == "") {
                eccList += mEccList[i].number;
            } else {
                eccList += String8(",") + mEccList[i].number;
            }
        }
    }

    logD(RFX_LOG_TAG, "[%s] mIsFwkReady: %d, mIsPendingUpdate: %d, ecc list: %s", __FUNCTION__,
            mIsFwkReady, mIsPendingUpdate, (char*)eccList.string());

    // For test purpose only
    // For framework solution, it will set ril.ecclist in framework
    // So we should not override it in RILD
    // if (mFrameworkEccSource == NULL) {
        // Set ril.ecclist property (For backward compatible)
        rfx_property_set(PROPERTY_ECC_LIST[m_slot_id], eccList.string());
    // }

    // Save special ECC list to property
    updateSpecialEmergencyNumber();

    // Send URC to framework (Q AOSP)
    char testMode[MTK_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.ril.ecc.test_mode", testMode, "0");
    if (mIsFwkReady || atoi(testMode) != 0) {
        int length = mEccList.size() * sizeof(RIL_EmergencyNumber);
        RIL_EmergencyNumber *pResponse = (RIL_EmergencyNumber *)calloc(1, length);
        if (pResponse == NULL) {
            return;
        }
        createEmergencyNumberListResponse(pResponse);
        sp<RfxMessage> urc = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_URC_EMERGENCY_NUMBER_LIST,
                    RfxEmergencyNumberListData(pResponse, length));
        responseToRilj(urc);
        freeEmergencyNumberListResponse(pResponse);

        // Send URC to framework (Legacy)
        urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_UNSOL_ECC_NUM,
                RfxStringData((char *)eccList.string()));
        responseToRilj(urc);
        mIsPendingUpdate = false;
    } else {
        mIsPendingUpdate = true;
    }

    dumpEccList();
}

void RtmEccNumberController::updateSpecialEmergencyNumber() {
    String8 specialEccList = String8("");
    int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
    if (simState == RFX_SIM_STATE_READY) {
        for (int i = 0; i < (int)mEccList.size(); i++) {
            if (mEccList[i].condition == CONDITION_MMI) {
                if (specialEccList == "") {
                    specialEccList += mEccList[i].number;
                } else {
                    specialEccList += String8(",") + mEccList[i].number;
                }
            }
        }
    }
    rfx_property_set(PROPERTY_SPECIAL_ECC_LIST[m_slot_id], specialEccList.string());
    logV(RFX_LOG_TAG, "[%s] simState: %d, list: %s", __FUNCTION__, simState,
            (char*)specialEccList.string());
}

String8 RtmEccNumberController::convertPlmnForRoaming(String8 plmn) {
    // Convert for CT roaming PLMN
    if (plmn == String8("20404") && isCtCard()) {
        return String8("46011");
    }
    return plmn;
}

String8 RtmEccNumberController::getPlmn(int slot) {
    String8 networkPlmn = getStatusManager(slot)->getString8Value(
            RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED, String8(""));
    String8 eccPlmn = getStatusManager(slot)->getString8Value(
                RFX_STATUS_KEY_MCCMNC_CHANGED, String8(""));
    String8 gsmUiccPlmn = getStatusManager(slot)->getString8Value(
                RFX_STATUS_KEY_UICC_GSM_NUMERIC, String8(""));
    String8 cdmaUiccPlmn = getStatusManager(slot)->getString8Value(
                RFX_STATUS_KEY_UICC_CDMA_NUMERIC, String8(""));
    RIL_RadioState radioState = (RIL_RadioState)getStatusManager(slot)->getIntValue(
            RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_ON);
    logD(RFX_LOG_TAG, "[%s] nwPlmn:%s,eccPlmn:%s,gsmUiccPlmn:%s,cdmaUiccPlmn:%s,radioState:%d",
            __FUNCTION__, (char *)networkPlmn.string(), (char *)eccPlmn.string(),
            (char *)gsmUiccPlmn.string(), (char *)cdmaUiccPlmn.string(), radioState);
    if (radioState == RADIO_STATE_ON) {
        // Use NW PLMN prior than SIM PLMN
        if (networkPlmn.string() != NULL && networkPlmn.length() > MCC_CHAR_LEN &&
            // Don't return invalid network PLMN here, instead we
            // should try to get from SIM/UICC MCC/MNC
            networkPlmn != "000000" && networkPlmn != "FFFFFF") {
            return networkPlmn;
        }
        if (eccPlmn.string() != NULL && eccPlmn.length() > MCC_CHAR_LEN &&
                // Don't return invalid eccPlmn PLMN here, instead we
                // should try to get from SIM/UICC MCC/MNC
                eccPlmn != "000000" && eccPlmn != "FFFFFF") {
            return eccPlmn;
        }
        if (gsmUiccPlmn.string() != NULL && gsmUiccPlmn.length() > MCC_CHAR_LEN) {
            return convertPlmnForRoaming(gsmUiccPlmn);
        }
        if (cdmaUiccPlmn.string() != NULL && cdmaUiccPlmn.length() > MCC_CHAR_LEN) {
            return cdmaUiccPlmn;
        }
    } else {
        // Use SIM PLMN prior than NW PLMN
        if (gsmUiccPlmn.string() != NULL && gsmUiccPlmn.length() > MCC_CHAR_LEN) {
            return convertPlmnForRoaming(gsmUiccPlmn);
        }
        if (cdmaUiccPlmn.string() != NULL && cdmaUiccPlmn.length() > MCC_CHAR_LEN) {
            return cdmaUiccPlmn;
        }
        if (networkPlmn.string() != NULL && networkPlmn.length() > MCC_CHAR_LEN &&
            // Don't return invalid network PLMN here, instead we
            // should try to get from SIM/UICC MCC/MNC
            networkPlmn != "000000" && networkPlmn != "FFFFFF") {
            return networkPlmn;
        }
        if (eccPlmn.string() != NULL && eccPlmn.length() > MCC_CHAR_LEN &&
                // Don't return invalid eccPlmn PLMN here, instead we
                // should try to get from SIM/UICC MCC/MNC
                eccPlmn != "000000" && eccPlmn != "FFFFFF") {
            return eccPlmn;
        }
    }

    // No PLMN detect, return ""
    logD(RFX_LOG_TAG, "[%s] No PLMN detected!", __FUNCTION__);
    return String8("");
}

bool RtmEccNumberController::isEccMatchInList(String8 number, String8 eccList) {
    // Add match boundary char ',' for easy match
    String8 searchEccList = String8(",") + eccList + String8(",");
    if (searchEccList.find(String8(",") + number + String8(",")) != -1) {
        return true;
    }

    return false;
}

String8 RtmEccNumberController::getSourcesString(int sources) {
    String8 sourcesString = String8("");
    if (sources & SOURCE_NETWORK) {
        sourcesString += String8("|") + String8("Network");
    }
    if (sources & SOURCE_SIM) {
        sourcesString += String8("|") + String8("SIM");
    }
    if (sources & SOURCE_CONFIG) {
        sourcesString += String8("|") + String8("Config");
    }
    if (sources & SOURCE_DEFAULT) {
        sourcesString += String8("|") + String8("Default");
    }
    if (sources & SOURCE_OEM_PROPERTY) {
        sourcesString += String8("|") + String8("Property");
    }
    if (sources & SOURCE_FRAMEWORK) {
        sourcesString += String8("|") + String8("Framework");
    }
    if (sources & SOURCE_TEST) {
        sourcesString += String8("|") + String8("TEST");
    }
    if (sources & SOURCE_MCF) {
        sourcesString += String8("|") + String8("MCF");
    }
    return sourcesString;
}

void RtmEccNumberController::dumpEccList() {
    for (int i = 0; i < (int)mEccList.size(); i++) {
        logD(RFX_LOG_TAG, "[%s] ECC [%d][%s,%s,%s,%d,%d,%s]",
                __FUNCTION__, i,
                mEccList[i].number.string(),
                mEccList[i].mcc.string(),
                mEccList[i].mnc.string(),
                mEccList[i].categories,
                mEccList[i].condition,
                getSourcesString(mEccList[i].sources).string());
    }
    if (ECC_DEBUG == 1) {
        testEcc();
    }
}

void RtmEccNumberController::dumpApEccList() {
    for (int i = 0; i < (int)mApEccList.size(); i++) {
        logD(RFX_LOG_TAG, "[%s] AP ECC [%d][%s,%s,%s,%d,%d,%s]",
                __FUNCTION__, i,
                mApEccList[i].number.string(),
                mApEccList[i].mcc.string(),
                mApEccList[i].mnc.string(),
                mApEccList[i].categories,
                mApEccList[i].condition,
                getSourcesString(mApEccList[i].sources).string());
    }
}

void RtmEccNumberController::testEcc() {
    logD(RFX_LOG_TAG, "[testEcc] start==================");
    String8 testEccList[] = {
            String8("111"),
            String8("222"),
            String8("123"),
            String8("112"),
            String8("911"),
            String8("000"),
            String8("08"),
            String8("110"),
            String8("119"),
            String8("120"),
            String8("122"),
            String8("10086")};
    for (int i = 0; i < (int)(sizeof(testEccList) / sizeof(testEccList[0])); i ++) {
        logD(RFX_LOG_TAG, "[testEcc] %s EccRouting: %d, category: %d", testEccList[i].string(),
                getEmergencyCallRouting(testEccList[i]), getServiceCategory(testEccList[i]));
        logD(RFX_LOG_TAG, "[testEcc] number %s preference[%d,%d,%d]", testEccList[i].string(),
                isGsmOnlyNumber(testEccList[i]),
                isGsmPreferredNumber(testEccList[i]),
                isCdmaPreferredNumber(testEccList[i]));
    }
    logD(RFX_LOG_TAG, "[testEcc] end==================");
}

bool RtmEccNumberController::onHandleRequest(const sp<RfxMessage>& message) {
    logV(RFX_LOG_TAG, "[%d]Handle request %s",
            message->getPToken(), RFX_ID_TO_STR(message->getId()));

    switch (message->getId()) {
    case RFX_MSG_REQUEST_SET_ECC_NUM:
        handleSetEccNum(message);
        break;
    case RFX_MSG_REQUEST_GET_ECC_NUM:
        handleGetEccNum(message);
        break;
    default:
        logD(RFX_LOG_TAG, "unknown request, ignore!");
        break;
    }
    return true;
}

bool RtmEccNumberController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    logD(RFX_LOG_TAG, "[%d][%s] responseId: %s",
            m_slot_id, __FUNCTION__ , idToString(msg_id));

    switch (msg_id) {
        case RFX_MSG_REQUEST_LOCAL_GET_ECC_NUM_FROM_MD:
            handleGetEccResponse(message);
            break;
        case RFX_MSG_REQUEST_LOCAL_SYNC_ECC_NUM_TO_MD:
            if (message->getError() != RIL_E_SUCCESS) {
                // Retry to sync again
                logE(RFX_LOG_TAG, "[%d][%s] Sync ECC fail, retry again!",
                        m_slot_id, __FUNCTION__);
                //syncEccToModem();
            }
            break;
        case RFX_MSG_REQUEST_LOCAL_HANDLE_SCREEN_STATE_CHANGE:
            break;
        default:
            logE(RFX_LOG_TAG, "[%d][%s] unknown response, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
    return true;
}

void RtmEccNumberController::handleGetEccResponse(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    // Do nothing, ecc list will update to framework in upcoming MD ECC change
    // indication
}

void RtmEccNumberController::handleSetEccNum(const sp<RfxMessage>& message) {
    const char **strings = (const char **)message->getData()->getData();
    if (strings == NULL || (strings[0] == NULL && strings[1] == NULL)) {
        logE(RFX_LOG_TAG, "handleSetEccNum invalid arguments.");
        sp<RfxMessage> responseMsg =
                RfxMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS, message, true);
        responseToRilj(responseMsg);
        return;
    }

    logD(RFX_LOG_TAG, "handleSetEccNum EccListWithCard: %s, EccListNoCard: %s",
            strings[0], strings[1]);

    // Create mFrameworkEccSource when needed
    if (mFrameworkEccSource == NULL) {
        mFrameworkEccSource = new FrameworkEccNumberSource(m_slot_id);
        mApEccNumberSourceList.push_back(mFrameworkEccSource);
    }

    mFrameworkEccSource->set(strings[0], strings[1]);
    mFrameworkEccSource->update(mIsSimInsert);

    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
            true);
    responseToRilj(responseMsg);

    // Sync ECC to modem which will trigger modem update all ecc to framework
    syncEccToModem();
}

void RtmEccNumberController::handleGetEccNum(const sp<RfxMessage>& message) {
    logV(RFX_LOG_TAG, "handleGetEccNum");
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
            true);
    responseToRilj(responseMsg);

    requestToMcl(RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_LOCAL_GET_ECC_NUM_FROM_MD, RfxVoidData()));
}

bool RtmEccNumberController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff, int radioState) {
    int msgId = message->getId();
    if((radioState == (int)RADIO_STATE_OFF) &&
            (msgId == RFX_MSG_REQUEST_SET_ECC_NUM ||
             msgId == RFX_MSG_REQUEST_GET_ECC_NUM)) {
        return false;
    } else if ((radioState == (int)RADIO_STATE_UNAVAILABLE) &&
            (msgId == RFX_MSG_REQUEST_SET_ECC_NUM ||
             msgId == RFX_MSG_REQUEST_GET_ECC_NUM)) {
        return false;
    }
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

bool RtmEccNumberController::isCtCard() {
    bool ret = false;
    int type = getStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE);
    if (type == CT_4G_UICC_CARD ||
            type == CT_UIM_SIM_CARD ||
            type == CT_3G_UIM_CARD) {
        ret = true;
    }
    return ret;
}

bool RtmEccNumberController::updateEmergencySourcesForPlmnChange(bool isSimInsert) {
    bool isChange = false;
    // reload XML ECC
    if (mXmlEccSource != NULL &&
            mXmlEccSource->update(getPlmn(m_slot_id), isSimInsert)) {
        isChange = true;
    }

    // reload property ECC
    if (mPropertyEccSource != NULL &&
            mPropertyEccSource->update(getPlmn(m_slot_id), isSimInsert)) {
        isChange = true;
    }

    // Update peer slot emergency numbers if possible
    int simCount = RfxRilUtils::rfxGetSimCount();
    for (int i = 0; i < simCount; i++) {
        if (i != m_slot_id) {
            RtmEccNumberController* eccController = (RtmEccNumberController *)findController(
                    i, RFX_OBJ_CLASS_INFO(RtmEccNumberController));
            if (eccController != NULL) {
                logV(RFX_LOG_TAG, "[%s] update ECC for peer slot %d", __FUNCTION__, i);
                if (eccController->updateEmergencySourcesFromPeer(getPlmn(m_slot_id))) {
                    eccController->syncEccToModem();
                }
            }
        }
    }

    if (mEccPreference != NULL) {
        mEccPreference->update(getPlmn(m_slot_id));
    }

    return isChange;
}

// Update emergency number when PLMN changes from peer slot
// For example: SIM1 have SIM and SIM2 no SIM, SIM2 emergency number will not update
// for PLMN because it can't get valid PLMN. In this case, we can update SIM2 emergency
// numbers if SIM1 got PLMN.
bool RtmEccNumberController::updateEmergencySourcesFromPeer(String8 plmn) {
    if (getPlmn(m_slot_id).length() > MCC_CHAR_LEN || plmn.length() < MCC_CHAR_LEN
            || mIsSimInsert) {
        logD(RFX_LOG_TAG, "[%s] No need mIsSimInsert: %d", __FUNCTION__, mIsSimInsert);
        return false;
    }

    logI(RFX_LOG_TAG, "[%s] Update for peer PLMN: %s", __FUNCTION__, plmn.string());

    bool isChange = false;
    // reload XML ECC
    if (mXmlEccSource != NULL &&
            mXmlEccSource->update(plmn, hasSimInAnySlot())) {
        isChange = true;
    }

    // reload property ECC
    if (mPropertyEccSource != NULL &&
            mPropertyEccSource->update(plmn, hasSimInAnySlot())) {
        isChange = true;
    }

    if (mEccPreference != NULL) {
        mEccPreference->update(plmn);
    }

    return isChange;
}

void RtmEccNumberController::removeNonEmergencyNumbers() {
    for (Vector<EmergencyNumber>::iterator iter = mEccList.begin(); iter != mEccList.end();) {
        if (iter != NULL) {
            if ((*iter).condition == (int)CONDITION_NEVER) {
                logD(RFX_LOG_TAG, "[%s] Remove %s", __FUNCTION__, (*iter).number.string());
                iter = mEccList.erase(iter);
            } else {
                ++iter;
            }
        }
    }
}

bool RtmEccNumberController::isSimInserted() {
    return mIsSimInsert;
}

bool RtmEccNumberController::hasSimInAnySlot() {
    if (mIsSimInsert) {
        return true;
    }
    int simCount = RfxRilUtils::rfxGetSimCount();
    for (int i = 0; i < simCount; i++) {
        RtmEccNumberController* eccNumberController = (RtmEccNumberController *)findController(
                i, RFX_OBJ_CLASS_INFO(RtmEccNumberController));
        if (eccNumberController != NULL && eccNumberController->isSimInserted()) {
            return true;
        }
    }
    return false;
}

bool RtmEccNumberController::updateForSimStateChanges(bool isSimInserted) {
    logV(RFX_LOG_TAG, "[%s] isSimInserted: %d", __FUNCTION__, isSimInserted);

    bool isChange = false;

    if (mFrameworkEccSource != NULL &&
            mFrameworkEccSource->update(isSimInserted)) {
        isChange = true;
    }

    // reload XML ECC
    if (mXmlEccSource != NULL &&
            mXmlEccSource->update(getPlmn(m_slot_id), isSimInserted)) {
        isChange = true;
    }

    // reload property ECC
    if (mPropertyEccSource != NULL &&
            mPropertyEccSource->update(getPlmn(m_slot_id), isSimInserted)) {
        isChange = true;
    }

    return isChange;
}

void RtmEccNumberController::updateEmergencySourcesForAllSimStateChanges(bool isSimInserted) {
    int simCount = RfxRilUtils::rfxGetSimCount();
    for (int i = 0; i < simCount; i++) {
        RtmEccNumberController* eccNumberController = (RtmEccNumberController *)findController(
                i, RFX_OBJ_CLASS_INFO(RtmEccNumberController));
        if (eccNumberController != NULL) {
            if (eccNumberController->updateForSimStateChanges(isSimInserted)) {
                eccNumberController->syncEccToModem();
            }
        }
    }
}

void RtmEccNumberController::setPreferenceList(int preferenceType, String8 preferenceList) {
    if (mEccPreference != NULL) {
        mEccPreference->setPreferenceList(preferenceType, preferenceList);
    }
}

Vector<EmergencyNumber>* RtmEccNumberController::getEccList() {
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        return &mEccList;
    }
    return NULL;
}