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
#include "RtcEccNumberController.h"
#include "RfxAtLine.h"
#include "tinyxml.h"
#include <libmtkrilutils.h>
#include "RfxEmergencyNumberListData.h"
#include "RfxRilUtils.h"

#define RFX_LOG_TAG "RtcEccNumberController"

#define MAX_PROP_CHARS       92
#define MCC_CHAR_LEN         3
#define PLMN_CHAR_LEN        6
#define ESIMS_CAUSE_RECOVERY 14
#define MAX_ECC_NUM          16
#define MAX_ECC_BUF_SIZE     (MAX_ECC_NUM * 8 + 1)
#define MAX_PRINT_CHAR       2

#define DELAY_SET_FWK_READY_TIMER 5000 //5s

#define CHAR_BYTE_LENGTH     8
#define MAX_URN_LENGTH       50

/*****************************************************************************
 * Class RtcEccNumberController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcEccNumberController", RtcEccNumberController, RfxController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_ECC_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_GET_ECC_NUM);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_UNSOL_ECC_NUM);
RFX_REGISTER_DATA_TO_URC_ID(RfxEmergencyNumberListData, RFX_MSG_URC_EMERGENCY_NUMBER_LIST);

RtcEccNumberController::RtcEccNumberController() :
        mGsmEcc(""), mC2kEcc(""),
        mCachedGsmUrc(NULL),
        mCachedC2kUrc(NULL),
        mIsSimInsert(false),
        mSimEccSource(NULL),
        mNetworkEccSource(NULL),
        mDefaultEccSource(NULL),
        mXmlEccSource(NULL),
        mFrameworkEccSource(NULL),
        mPropertyEccSource(NULL),
        mTestEccSource(NULL),
        mIsFwkReady(false),
        mIsPendingUpdate(false),
        mTimerHandle(NULL),
        mEccPreference(NULL),
        mNeedRefreshNwEcc(false), mNeedRefreshExtNwEcc(false),
        mNwEccPlmn(""), mExtNwEccValidity(-1) {
    char testMode[MTK_PROPERTY_VALUE_MAX] = {0};
    mtk_property_get("persist.vendor.ril.test_mode", testMode, "0");
    if (atoi(testMode) != 0) {
        mIsFwkReady = true;
        logD(RFX_LOG_TAG, "[%s] Set framework ready for test mode", __FUNCTION__);
    }
    mNwEccList.clear();
    mExtNwEccList.clear();
}

RtcEccNumberController::~RtcEccNumberController() {
    if (mCachedC2kUrc != NULL) {
        delete(mCachedC2kUrc);
    }
    if (mCachedGsmUrc != NULL) {
        delete(mCachedGsmUrc);
    }

    for (int i = 0; i < (int)mEccNumberSourceList.size(); i++) {
        if (mEccNumberSourceList[i] != NULL) {
            delete(mEccNumberSourceList[i]);
        }
    }
    mEccNumberSourceList.clear();
}

EmergencyCallRouting RtcEccNumberController::getEmergencyCallRouting(String8 number) {
    if (number.isEmpty()) {
        logE(RFX_LOG_TAG, "[%s] Empty number return unknown", __FUNCTION__);
        return ECC_ROUTING_UNKNOWN;
    }

    // Only print first two characters
    char maskNumber[MAX_PRINT_CHAR + 1] = {0};
    strncpy(maskNumber, number.string(), MAX_PRINT_CHAR);
    maskNumber[MAX_PRINT_CHAR] = '\0';

    int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
    for (int i = 0; i < (int)mEccList.size(); i++) {
        if (mEccList[i].number == number) {
            // For special/fake emergency number, it should dial using normal call routing when
            // SIM ready, for other cases, it should still dial using emergency routing
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

bool RtcEccNumberController::isEmergencyNumber(String8 number) {
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

int RtcEccNumberController::getServiceCategory(String8 number) {
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

int RtcEccNumberController::getServiceCategoryAndUrn(String8 number, String8 &urn) {
    // Only print first two characters
    char maskNumber[MAX_PRINT_CHAR + 1] = {0};
    strncpy(maskNumber, number.string(), MAX_PRINT_CHAR);
    maskNumber[MAX_PRINT_CHAR] = '\0';

    for (int i = 0; i < (int)mEccList.size(); i++) {
        if ((mEccList[i].number == number) &&
                (mEccList[i].categories != ECC_CATEGORY_NOT_DEFINED)) {
            RFX_LOG_I(RFX_LOG_TAG, "[%s] %s* category is %d, urn is: %s", __FUNCTION__, maskNumber,
                    mEccList[i].categories,
                    (char*)mEccList[i].urn.string());
            urn = mEccList[i].urn;
            return mEccList[i].categories;
        }
    }

    RFX_LOG_I(RFX_LOG_TAG, "[%s] %s* category is 0", __FUNCTION__, maskNumber);
    return 0;
}


bool RtcEccNumberController::isGsmOnlyNumber(String8 number) {
    if (mEccPreference != NULL) {
        return mEccPreference->isGsmOnlyNumber(number);
    }
    return false;
}

bool RtcEccNumberController::isGsmPreferredNumber(String8 number) {
    if (mEccPreference != NULL) {
        return mEccPreference->isGsmPreferredNumber(number);
    }
    return false;
}

bool RtcEccNumberController::isCdmaPreferredNumber(String8 number) {
    if (mEccPreference != NULL) {
        return mEccPreference->isCdmaPreferredNumber(number);
    }
    return false;
}


void RtcEccNumberController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    logV(RFX_LOG_TAG, "[%s]", __FUNCTION__);

    const int urc_id_list[] = {
        RFX_MSG_URC_CC_GSM_SIM_ECC,
        RFX_MSG_URC_CC_C2K_SIM_ECC,
        RFX_MSG_URC_NW_ECC
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));

    const int requests[] = {
        RFX_MSG_REQUEST_SET_ECC_NUM,
        RFX_MSG_REQUEST_GET_ECC_NUM,
    };
    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));

    // register callbacks to get card type change event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onCardTypeChanged));

    // register callbacks to get PLMN(MCC,MNC) change event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onPlmnChanged));

    // register callbacks to get PLMN(MCC,MNC) report for emergency call
    // For No SIM case, EOPS may update too later before make emergency call
    // and PLMN specific emergency numbers will not be detected in this case.
    // To enhance the PLMN update flow, modem will update PLMN directly through
    // URC +EOPSU
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_MCCMNC_CHANGED,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onPlmnChanged));

    // register callbacks to get sim recovery event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SIM_ESIMS_CAUSE,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onSimRecovery));

    // register callbacks to get sim state event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SIM_STATE,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onSimStateChanged));

    // register callbacks to get sim state event
    getStatusManager()->registerStatusChanged(RFX_STATUS_CONNECTION_STATE,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onConnectionStateChanged));

    // register callbacks to get GSM UICC MCC/MNC
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_UICC_GSM_NUMERIC,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onGsmUiccMccMncChanged));

    // register callbacks to get CDMA UICC MCC/MNC
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_UICC_CDMA_NUMERIC,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onCdmaUiccMccMncChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
        RfxStatusChangeCallback(this, &RtcEccNumberController::onRadioStateChanged));

    initEmergencyNumberSource();

    mEccPreference = new EccNumberPreference(m_slot_id);
    mEccPreference->update(getPlmn(m_slot_id));
}

void RtcEccNumberController::initEmergencyNumberSource() {
    mNetworkEccSource = new NetworkEccNumberSource(m_slot_id);
    mSimEccSource = new SimEccNumberSource(m_slot_id);
    mDefaultEccSource = new DefaultEccNumberSource(m_slot_id);

    mEccNumberSourceList.clear();
    // Add ECC source by priority
    mEccNumberSourceList.push_back(mNetworkEccSource);
    mEccNumberSourceList.push_back(mSimEccSource);
    mEccNumberSourceList.push_back(mDefaultEccSource);

    // Init property ECC if property is set
    char eccCount[MAX_PROP_CHARS] = {0};
    rfx_property_get(PROPERTY_ECC_COUNT, eccCount, "0");
    if (atoi(eccCount) > 0) {
        logD(RFX_LOG_TAG, "[%s] OEM property exist, init property source", __FUNCTION__);
        mPropertyEccSource = new OemPropertyEccNumberSource(m_slot_id);
        mEccNumberSourceList.push_back(mPropertyEccSource);
    }

    // Init XML ECC if XML file is exist
    FILE* file = fopen(ECC_PATH, "r");
    // Try to find a light way to check file exist
    if (file != NULL) {
        logD(RFX_LOG_TAG, "[%s] XML file exist, init XML source", __FUNCTION__);
        mXmlEccSource = new XmlEccNumberSource(m_slot_id);
        mEccNumberSourceList.push_back(mXmlEccSource);
        if (fclose(file) != 0) {
            logE(RFX_LOG_TAG, "[%s] Close XML file fail", __FUNCTION__);
        }
    }

    mTestEccSource = new TestEccNumberSource(m_slot_id);
    mEccNumberSourceList.push_back(mTestEccSource);

    mEccList.clear();
}

void RtcEccNumberController::onCardTypeChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    if (oldValue.asInt() != newValue.asInt()) {
        logV(RFX_LOG_TAG, "[%s] oldValue %d, newValue %d", __FUNCTION__,
            oldValue.asInt(), newValue.asInt());
        if (newValue.asInt() == 0) {
            logD(RFX_LOG_TAG,"[%s], reset SIM/NW ECC due to No SIM", __FUNCTION__);
            mGsmEcc = String8("");
            mC2kEcc = String8("");
            // Clear network ECC when SIM removed according to spec.
            rfx_property_set(PROPERTY_NW_ECC_LIST[m_slot_id], "");
            mNwEccList.clear();
            mExtNwEccList.clear();
            mIsSimInsert = false;
            mSimEccSource->update(String8(""), String8(""));
            updateEmergencySourcesForAllSimStateChanges(hasSimInAnySlot());
            mTestEccSource->update(false);
            updateNetworkEccList(true);
        } else if (!isCdmaCard(newValue.asInt())) {
            // no CSIM or RUIM application, clear CDMA ecc property
            logV(RFX_LOG_TAG,"[%s], Remove C2K ECC due to No C2K SIM", __FUNCTION__);
            mC2kEcc = String8("");
            mSimEccSource->update(mGsmEcc, String8(""));
            updateEmergencyNumber();
        }
    }
}

bool RtcEccNumberController::isCtPlmn(String8 plmn) {
    return plmn == "46011" || plmn == "46003";
}

bool RtcEccNumberController::isValidPlmn(String8 plmn) {
    if (plmn.length() < MCC_CHAR_LEN ||
            plmn == "000000" ||
            plmn == "00000"  ||
            plmn == "FFFFFF" ||
            plmn == "FFFFF") {
        return false;
    }
    return true;
}

void RtcEccNumberController::onPlmnChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    RFX_UNUSED(oldValue);
    bool isNwEccChange = false;
    String8 newPlmn = newValue.asString8();

    // The sequence of modem report network ecc list and camp on cell is not fixed.
    // So there will be cases when modem report network ecc list (+CEN1/+CEN2) and
    // AP will save the network ECC then before camp on cell, EOPS will be 000000.
    // Thus if AP query PLMN at this time, onMccMncIndication() will be called and
    // previous network ECC saved will be cleared. To fix this timing issue, we'll
    // not reset network ECC when PLMN change to "000000".
    if (!isValidPlmn(newPlmn)) {
        logV(RFX_LOG_TAG, "[%s] Invalid PLMN: %s", __FUNCTION__,
                (char*)(newPlmn.string()));
        return;
    }

    // MCC change, clear network ecc and ext network ecc
    if (isValidPlmn(mNwEccPlmn) && strncmp((const char *)(mNwEccPlmn.string()),
            (const char *)(newPlmn.string()), MCC_CHAR_LEN)) {
        mNwEccList.clear();
        // EENL valid within country(MCC)
        mExtNwEccList.clear();
        isNwEccChange = true;
    } else if (newPlmn != mNwEccPlmn) {
        // EENL valid within PLMN
        if (mExtNwEccValidity == 1) {
            mExtNwEccList.clear();
            isNwEccChange = true;
        }
    }

    // CT new ECC requirement 2023 customization
    // When roaming to non CT network, CT  network ECC list (110/119/120/122)
    // should be cleared and must dial using normal call for these numbers
    if (isCtPlmn(mNwEccPlmn) && !isCtPlmn(newPlmn)) {
        logI(RFX_LOG_TAG, "[%s] Clear CT network ECC for roaming", __FUNCTION__);
        mNwEccList.clear();
        isNwEccChange = true;
    }

    if (isNwEccChange) {
        updateNetworkEccList(false);
    }

    logD(RFX_LOG_TAG, "[%s] valid:%d, network Plmn:%s, reg Plmn:%s, isChange:%d", __FUNCTION__,
            mExtNwEccValidity, (char*)mNwEccPlmn.string(), (char*)(newValue.asString8().string()),
            isNwEccChange);
    if (updateEmergencySourcesForPlmnChange(hasSimInAnySlot()) || isNwEccChange) {
        updateEmergencyNumber();
    }
}

void RtcEccNumberController::onSimRecovery(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    RFX_UNUSED(oldValue);

    if (newValue.asInt() == ESIMS_CAUSE_RECOVERY) {
        logD(RFX_LOG_TAG, "[%s] parse from cached URC", __FUNCTION__);

        // Need parse from cached ECC URC when SIM recovery because when
        // sim lost it will clear ECC in card type change event
        parseSimEcc(mCachedGsmUrc, true);
        parseSimEcc(mCachedC2kUrc, false);
        mSimEccSource->update(mGsmEcc, mC2kEcc);
        updateEmergencyNumber();
    }
}

// Update special emergency number when sim state changed
// For special emergency number:
//     SIM ready: Normal call routing
//     Others: Emergency call routing
void RtcEccNumberController::onSimStateChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);

    if (oldValue.asInt() != newValue.asInt()) {
        logV(RFX_LOG_TAG,"[%s] update special emergency number", __FUNCTION__);
        updateSpecialEmergencyNumber();
    }
}

void RtcEccNumberController::onConnectionStateChanged(RfxStatusKeyEnum key,
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
                &RtcEccNumberController::delaySetFwkReady), ms2ns(DELAY_SET_FWK_READY_TIMER));
    }
}

void RtcEccNumberController::onGsmUiccMccMncChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(key);

    logV(RFX_LOG_TAG, "[%s] oldValue %s, newValue %s", __FUNCTION__,
        (const char *)(oldValue.asString8()), (const char *)(newValue.asString8()));

    if (updateEmergencySourcesForPlmnChange(true)) {
        updateEmergencyNumber();
    }
}

void RtcEccNumberController::onCdmaUiccMccMncChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(key);
    bool isChange = false;

    logV(RFX_LOG_TAG, "[%s] oldValue %s, newValue %s", __FUNCTION__,
        (const char *)(oldValue.asString8()), (const char *)(newValue.asString8()));

    // Ignore CT CSIM ECC and update
    logD(RFX_LOG_TAG, "[%s] isCtCard %d, plmn: %s", __FUNCTION__, isCtCard(),
            (const char *)newValue.asString8());
    if ((isCtCard() || isCtPlmn(newValue.asString8())) && !mC2kEcc.isEmpty()) {
        logI(RFX_LOG_TAG, "[%s] Ignore CSIM ECC for CT card", __FUNCTION__);
        mC2kEcc = "";
        mSimEccSource->update(mGsmEcc, mC2kEcc);
        isChange = true;
    }

    if (updateEmergencySourcesForPlmnChange(true) || isChange) {
        updateEmergencyNumber();
    }
}

void RtcEccNumberController::onRadioStateChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(oldValue);
    RFX_UNUSED(key);
    if (newValue.asInt() == RADIO_STATE_OFF) {
        // Specification of 38.523-1 Table 11.4.8.3.2-1 step3,4
        logI(RFX_LOG_TAG, "[%s] radio off, clear network ECC list", __FUNCTION__);
        rfx_property_set(PROPERTY_NW_ECC_LIST[m_slot_id], "");
        mNwEccList.clear();
        mExtNwEccList.clear();
        updateNetworkEccList(true);
    }
}

void RtcEccNumberController::delaySetFwkReady() {
    logD(RFX_LOG_TAG, "[%s] mIsFwkReady: %d, mIsPendingUpdate: %d", __FUNCTION__,
            mIsFwkReady, mIsPendingUpdate);
    mIsFwkReady = true;
    if (mIsPendingUpdate) {
        updateEmergencyNumber();
        mIsPendingUpdate = false;
    }
}

bool RtcEccNumberController::onHandleUrc(const sp<RfxMessage>& message) {
    int msgId = message->getId();

    switch (msgId) {
        case RFX_MSG_URC_CC_GSM_SIM_ECC:
            handleGsmSimEcc(message);
            break;
        case RFX_MSG_URC_CC_C2K_SIM_ECC:
            handleC2kSimEcc(message);
            break;
        case RFX_MSG_URC_NW_ECC:
            handleNetworkEcc(message);
            break;
        default:
            break;
    }

    return true;
}

/*
 * [MD1 EF ECC URC format]
 * + ESMECC: <m>[,<number>,<service category>[,<number>,<service category>]]
 * <m>: number of ecc entry
 * <number>: ecc number
 * <service category>: service category
 * Ex.
 * URC string:+ESIMECC:3,115,4,334,5,110,1
 *
 * Note:If it has no EF ECC, RtcEccNumberController will receive "0"
*/
void RtcEccNumberController::handleGsmSimEcc(const sp<RfxMessage>& message){
    if (mCachedGsmUrc != NULL) {
        delete(mCachedGsmUrc);
    }
    // SIM ECC URC come earlier then other SIM status key
    mIsSimInsert = true;

    mCachedGsmUrc = new RfxAtLine((const char* )(message->getData()->getData()), NULL);
    //RFX_UNUSED(message);
    //mCachedGsmUrc = new RfxAtLine("AT< +ESIMECC: 16,112,0,911,0,123,1,125,2,119,4,146,8,144,16,126,32,111,64,141,34,147,84,146,65,013,1,014,1,015,1,016,1", NULL);

    parseSimEcc(mCachedGsmUrc, true);
    mSimEccSource->update(mGsmEcc, mC2kEcc);
    updateEmergencySourcesForAllSimStateChanges(true);
    mTestEccSource->update(true);
    updateEmergencyNumber();
}

/*
 * [MD3 EF ECC URC format]
 * +CECC:<m>[,<number [,<number >]]
 * <m>: number of ecc entry
 * <number>: ecc number
 * Ex.
 * URC string:2,115,334
 *
 * Note:If it has no EF ECC, RtcEccNumberController will receive "0"
 *
*/
void RtcEccNumberController::handleC2kSimEcc(const sp<RfxMessage>& message){
    // SIM ECC URC come earlier then other SIM status key
    mIsSimInsert = true;

    // Ignore CT CSIM ECC and update
    String8 plmn = getPlmn(m_slot_id);
    logD(RFX_LOG_TAG, "[%s] isCtCard %d, plmn: %s", __FUNCTION__, isCtCard(),
            (const char *)plmn.string());
    if (isCtCard() || isCtPlmn(plmn)) {
        logI(RFX_LOG_TAG, "[%s] Ignore CSIM ECC for CT card", __FUNCTION__);
        return;
    }

    if (mCachedC2kUrc != NULL) {
        delete(mCachedC2kUrc);
    }

    mCachedC2kUrc = new RfxAtLine((const char* )(message->getData()->getData()), NULL);

    parseSimEcc(mCachedC2kUrc, false);
    mSimEccSource->update(mGsmEcc, mC2kEcc);
    updateEmergencySourcesForAllSimStateChanges(true);
    mTestEccSource->update(true);
    updateEmergencyNumber();
}

void RtcEccNumberController::parseSimEcc(RfxAtLine *line, bool isGsm) {
    String8 writeEcc = String8("");
    int err = 0;
    int count = 0;

    if (line == NULL) {
        logE(RFX_LOG_TAG, "[%s] error: line is NULL", __FUNCTION__);
        return;
    }

    logV(RFX_LOG_TAG, "[%s] line: %s", __FUNCTION__, line->getLine());

    line->atTokStart(&err);
    if (err < 0) goto error;

    // get ECC number count
    count = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            if (isGsm) {
                char* ecc = line->atTokNextstr(&err);
                if (err < 0) goto error;
                char* eccCategory = line->atTokNextstr(&err);
                if (err < 0) goto error;
                writeEcc.appendFormat("%s,%s;", ecc, eccCategory);
            } else {
                char* ecc = line->atTokNextstr(&err);
                if (err < 0) goto error;
                writeEcc.appendFormat("%s,", ecc);
            }
        }
    } else {
        logV(RFX_LOG_TAG, "[%s] There is no ECC number stored in SIM", __FUNCTION__);
    }

    if (isGsm) {
        mGsmEcc = writeEcc;
        logD(RFX_LOG_TAG,"[%s] mGsmEcc: %s", __FUNCTION__, mGsmEcc.string());
    } else {
        mC2kEcc = writeEcc;
        logD(RFX_LOG_TAG,"[%s] mC2kEcc: %s", __FUNCTION__, mC2kEcc.string());
    }

    return;
error:
    logE(RFX_LOG_TAG, "[%s] parsing error!", __FUNCTION__);
}

void RtcEccNumberController::handleNetworkEcc(const sp<RfxMessage>& message){
    RfxAtLine *line = new RfxAtLine((const char* )(message->getData()->getData()), NULL);
    int err = 0;
    if (line == NULL) {
        logE(RFX_LOG_TAG, "[%s] NULL line", __FUNCTION__);
        return;
    }

    String8 ss(line->getLine());
    line->atTokStart(&err);
    if (err < 0) goto error;
    if (ss.find("+CEN1") == 0) {
        // Handle +CEN1 URC
        // Example: +CEN1:1,460,011
        line->atTokNextint(&err);
        if (err < 0) goto error;
        char* mcc = line->atTokNextstr(&err);
        if (err < 0 || mcc == NULL || strlen(mcc) <= 0) goto error;

        char oldMcc[MCC_CHAR_LEN + 1] = {0};
        strncpy(oldMcc, (const char *)mNwEccPlmn.string(), MCC_CHAR_LEN);
        oldMcc[MCC_CHAR_LEN] = '\0';
        if (strcmp(oldMcc, mcc) != 0) {
            // MCC is changed, clear mapping list
            //rfx_property_set(PROPERTY_NW_ECC_LIST[m_slot_id], "");
            mNwEccList.clear();
            mExtNwEccList.clear();
            updateNetworkEccList(true);
            logD(RFX_LOG_TAG, "[%s] Set nw ECC MCC from %s to %s", __FUNCTION__, oldMcc, mcc);
        }

        if (mcc != NULL && strlen(mcc) > 0) {
            mNwEccPlmn = String8(mcc);
        }
        char* mnc = line->atTokNextstr(&err);
        if (err < 0) goto error;

        if (mnc != NULL && strlen(mnc) > 0) {
            mNwEccPlmn = mNwEccPlmn + String8(mnc);
        }
        logV(RFX_LOG_TAG, "[%s] mNwEccPlmn: %s", __FUNCTION__, (char *)mNwEccPlmn.string());
        mNeedRefreshNwEcc = true;
        mNeedRefreshExtNwEcc = true;
    } else if (ss.find("+CEN2") == 0) {
        // Handle +CEN2 URC
        // Example: +CEN2:4,119
        int category = line->atTokNextint(&err);
        if (err < 0) goto error;
        char* number = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // Clear network ECC after receive first CEN2 no matter MCC change or not
        if (mNeedRefreshNwEcc) {
            //rfx_property_set(PROPERTY_NW_ECC_LIST[m_slot_id], "");
            mNwEccList.clear();
            mNeedRefreshNwEcc = false;
            logD(RFX_LOG_TAG, "[%s] clear NW ECC after receive first CEN2", __FUNCTION__);
        }

        bool isExist = false;
        for (Vector<EmergencyNumber>::iterator iter = mNwEccList.begin();
                iter != mNwEccList.end();) {
            if (iter != NULL) {
                if ((*iter).number == String8(number)) {
                    isExist = true;
                    (*iter).categories = category;
                    logV(RFX_LOG_TAG, "[%s] Update ECC number:%s, category:%d", __FUNCTION__,
                            number, category);
                }
                ++iter;
            }
        }
        if (!isExist) {
            EmergencyNumber en;
            en.number = String8(number);
            en.mcc = "";
            en.mnc = "";
            en.urn = "";
            en.categories = category;
            en.sources = SOURCE_NETWORK;
            en.condition = CONDITION_ALWAYS;
            mNwEccList.push_back(en);
            logV(RFX_LOG_TAG, "[%s] Add ECC number:%s, category:%d", __FUNCTION__,
                    number, category);
        }
        updateNetworkEccList(true);
    }else if (ss.find("+CEN3") == 0) {
        // Handle +CEN3 URC
        // Example: +CEN3:<validity>
        //    0: EENL valid within country(MCC)
        //    1: EENL valid within PLMN)
        int validity = line->atTokNextint(&err);
        if (err < 0) goto error;
        mExtNwEccValidity = validity;
    } else if (ss.find("+CEN4") == 0){
        // Clear network ECC after receive first CEN4 no matter MCC change or not
        if (mNeedRefreshExtNwEcc) {
            mExtNwEccList.clear();
            mNeedRefreshExtNwEcc = false;
            logD(RFX_LOG_TAG, "[%s] clear Ext NW ECC after receive first CEN4", __FUNCTION__);
        }
        // +CEN4: <number-ext>, <sub-services>
        // Ex:
        //    +CEN4: "110", "police"
        //    +CEN4: "119", "fire"
        //    +CEN4: "119", "ambulance"
        char* number = line->atTokNextstr(&err);
        if (err < 0) goto error;
        char* urn = line->atTokNextstr(&err);
        if (err < 0) goto error;
        char strUrn[MAX_URN_LENGTH + 1] = {0};
        int category = (int)getCategoryFromUrn(urn, strUrn);
        bool isExist = false;
        for (Vector<EmergencyNumber>::iterator iter = mExtNwEccList.begin();
                iter != mExtNwEccList.end();) {
            if (iter != NULL) {
                if ((*iter).number == String8(number)) {
                    isExist = true;
                    (*iter).categories = (*iter).categories | category;
                }
                ++iter;
            }
        }
        if (!isExist) {
            EmergencyNumber en;
            en.number = String8(number);
            en.mcc = "";
            en.mnc = "";
            en.urn = String8(strUrn);
            en.categories = category;
            en.sources = SOURCE_NETWORK;
            en.condition = CONDITION_ALWAYS;
            mExtNwEccList.push_back(en);
        }
        updateNetworkEccList(true);

    }
    delete(line);
    return;
error:
    logE(RFX_LOG_TAG, "[%s] parsing error!", __FUNCTION__);
    delete(line);
}

bool RtcEccNumberController::isCdmaCard(int cardType) {
     if ((cardType & RFX_CARD_TYPE_RUIM) > 0 ||
         (cardType & RFX_CARD_TYPE_CSIM) > 0) {
         return true;
     }
     return false;
}

void RtcEccNumberController::createEmergencyNumberListResponse(RIL_EmergencyNumber *data) {
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
        logV(RFX_LOG_TAG, "[%s] plmn:%s,mcc:%s,mnc:%s", __FUNCTION__, plmn.string(),
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

void RtcEccNumberController::freeEmergencyNumberListResponse(RIL_EmergencyNumber *data) {
    for (int i = 0; i < mEccList.size(); i++) {
        FREEIF(data[i].number);
        FREEIF(data[i].mcc);
        FREEIF(data[i].mnc);
    }
    FREEIF(data);
}

void RtcEccNumberController::updateEmergencyNumber() {
    mEccList.clear();
    for (int i = 0; i < (int)mEccNumberSourceList.size(); i++) {
        mEccNumberSourceList[i]->addToEccList(mEccList);
    }

    // Remove non emergency numbers with condition NEVER after add all
    // emergency numbers with different source.
    removeNonEmergencyNumbers();

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

    // For framework solution, it will set ril.ecclist in framework
    // So we should not override it in RILD
    if (mFrameworkEccSource == NULL) {
        // Set ril.ecclist property (For backward compatible)
        rfx_property_set(PROPERTY_ECC_LIST[m_slot_id], eccList.string());
    }

    // Save special ECC list to property
    updateSpecialEmergencyNumber();

    // Send URC to framework (Q AOSP)
    if (mIsFwkReady) {
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

void RtcEccNumberController::updateSpecialEmergencyNumber() {
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

String8 RtcEccNumberController::convertPlmnForRoaming(String8 plmn) {
    // Convert for CT roaming PLMN
    if (plmn == String8("20404") && isCtCard()) {
        return String8("46011");
    }
    return plmn;
}

String8 RtcEccNumberController::getPlmn(int slot) {
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
    logV(RFX_LOG_TAG, "[%s] nwPlmn:%s,eccPlmn:%s,gsmUiccPlmn:%s,cdmaUiccPlmn:%s,radioState:%d",
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
    logV(RFX_LOG_TAG, "[%s] No PLMN detected!", __FUNCTION__);
    return String8("");
}

bool RtcEccNumberController::isEccMatchInList(String8 number, String8 eccList) {
    // Add match boundary char ',' for easy match
    String8 searchEccList = String8(",") + eccList + String8(",");
    if (searchEccList.find(String8(",") + number + String8(",")) != -1) {
        return true;
    }

    return false;
}

String8 RtcEccNumberController::getSourcesString(int sources) {
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
    return sourcesString;
}

void RtcEccNumberController::dumpEccList() {
    for (int i = 0; i < (int)mEccList.size(); i++) {
        logV(RFX_LOG_TAG, "[%s] ECC [%d][%s,%s,%s,%d,%d,%s]",
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

void RtcEccNumberController::testEcc() {
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

bool RtcEccNumberController::onHandleRequest(const sp<RfxMessage>& message) {
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

void RtcEccNumberController::handleSetEccNum(const sp<RfxMessage>& message) {
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
        mEccNumberSourceList.push_back(mFrameworkEccSource);
    }

    mFrameworkEccSource->set(strings[0], strings[1]);
    mFrameworkEccSource->update(mIsSimInsert);

    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
            true);
    responseToRilj(responseMsg);

    // sent update ecc list to upper layer
    updateEmergencyNumber();
}

void RtcEccNumberController::handleGetEccNum(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "handleGetEccNum");
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
            true);
    responseToRilj(responseMsg);

    // Response ecc number
    updateEmergencyNumber();
}

bool RtcEccNumberController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
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

bool RtcEccNumberController::isCtCard() {
    bool ret = false;
    int type = getStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE);
    if (type == CT_4G_UICC_CARD ||
            type == CT_UIM_SIM_CARD ||
            type == CT_3G_UIM_CARD) {
        ret = true;
    }
    return ret;
}

bool RtcEccNumberController::updateEmergencySourcesForPlmnChange(bool isSimInsert) {
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
            RtcEccNumberController* eccController = (RtcEccNumberController *)findController(
                    i, RFX_OBJ_CLASS_INFO(RtcEccNumberController));
            if (eccController != NULL) {
                logV(RFX_LOG_TAG, "[%s] update ECC for peer slot %d", __FUNCTION__, i);
                if (eccController->updateEmergencySourcesFromPeer(getPlmn(m_slot_id))) {
                    eccController->updateEmergencyNumber();
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
bool RtcEccNumberController::updateEmergencySourcesFromPeer(String8 plmn) {
    if (getPlmn(m_slot_id).length() > MCC_CHAR_LEN || plmn.length() < MCC_CHAR_LEN
            || mIsSimInsert) {
        logD(RFX_LOG_TAG, "[%s] No need mIsSimInsert: %d", __FUNCTION__, mIsSimInsert);
        return false;
    }

    logD(RFX_LOG_TAG, "[%s] Update for peer PLMN: %s", __FUNCTION__, plmn.string());

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

void RtcEccNumberController::removeNonEmergencyNumbers() {
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

bool RtcEccNumberController::isSimInserted() {
    return mIsSimInsert;
}

bool RtcEccNumberController::hasSimInAnySlot() {
    if (mIsSimInsert) {
        return true;
    }
    int simCount = RfxRilUtils::rfxGetSimCount();
    for (int i = 0; i < simCount; i++) {
        RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
                i, RFX_OBJ_CLASS_INFO(RtcEccNumberController));
        if (eccNumberController != NULL && eccNumberController->isSimInserted()) {
            return true;
        }
    }
    return false;
}

void RtcEccNumberController::updateForSimStateChanges(bool isSimInserted) {
    logV(RFX_LOG_TAG, "[%s] isSimInserted: %d", __FUNCTION__, isSimInserted);
    mDefaultEccSource->update(isSimInserted);
    if (mFrameworkEccSource != NULL) {
        mFrameworkEccSource->update(isSimInserted);
    }
    updateEmergencySourcesForPlmnChange(isSimInserted);
}

void RtcEccNumberController::updateEmergencySourcesForAllSimStateChanges(bool isSimInserted) {
    int simCount = RfxRilUtils::rfxGetSimCount();
    for (int i = 0; i < simCount; i++) {
        RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
                i, RFX_OBJ_CLASS_INFO(RtcEccNumberController));
        if (eccNumberController != NULL) {
            eccNumberController->updateForSimStateChanges(isSimInserted);
            eccNumberController->updateEmergencyNumber();
        }
    }
}

void RtcEccNumberController::updateNetworkEccList(bool notify) {
    String8 listString = String8("");

    for (int i = 0; i < mNwEccList.size(); i++) {
        if (listString.length() > 0) {
            listString = listString + String8(";");
        }
        listString = listString + mNwEccList[i].number;
        listString.appendFormat(",%d", mNwEccList[i].categories);
    }
    logV(RFX_LOG_TAG, "[%s] mNwEccList string: %s", __FUNCTION__, (char*)listString.string());
    for (int i = 0; i < mExtNwEccList.size(); i++) {
        if (listString.length() > 0) {
            listString = listString + String8(";");
        }
        listString = listString + mExtNwEccList[i].number;
        listString.appendFormat(",%d", mExtNwEccList[i].categories);
    }
    logD(RFX_LOG_TAG, "[%s] All ecc list string: %s", __FUNCTION__, (char*)listString.string());
    // Only for UT
    char listProp[MTK_PROPERTY_VALUE_MAX + 1] = {0};
    strncpy(listProp, (const char *)listString.string(), MTK_PROPERTY_VALUE_MAX);
    listProp[MTK_PROPERTY_VALUE_MAX] = '\0';
    rfx_property_set(PROPERTY_NW_ECC_LIST[m_slot_id], listProp);

    mNetworkEccSource->update(mNwEccList, mExtNwEccList);
    if (notify) {
        updateEmergencyNumber();
    }
}

unsigned char RtcEccNumberController::hexChar2Number(char hex) {
    unsigned char value = 0;
    if (hex >= '0' && hex <= '9') {
        value = hex - '0';
    } else if (hex >= 'A' && hex <= 'Z') {
        value = hex - 'A' + 10;
    } else if (hex >= 'a' && hex <= 'z') {
        value = hex - 'a' + 10;
    }
    return value;
}

unsigned char RtcEccNumberController::strHex2Byte(char*pHex) {
    unsigned char value = 0;
    value = hexChar2Number(pHex[0]) << 4;
    value |= hexChar2Number(pHex[1]);
    return value;
}

int RtcEccNumberController::gsm7bitToAscii(char *pDst, char *pSrc, int maxLen) {
    int cnt = 0;
    unsigned char nLeft = 1;
    unsigned char fillValue =0;
    unsigned char oldFillValue = 0;
    int srcLength = strlen(pSrc);
    for (int i = 0; i < srcLength; i += 2) {
        if (cnt >= maxLen) {
            logE(RFX_LOG_TAG, "[%s] cnt: %d, maxLen: %d", __FUNCTION__, cnt, maxLen);
            break;
        }

        *pDst = strHex2Byte(pSrc);
        fillValue = (unsigned char)*pDst;
        fillValue >>= (CHAR_BYTE_LENGTH - nLeft);
        *pDst <<=(nLeft - 1);
        *pDst &= 0x7F;
        *pDst |= oldFillValue;
        oldFillValue = fillValue;
        pDst++;
        cnt++;
        nLeft++;
        if (nLeft == CHAR_BYTE_LENGTH) {
            *pDst = oldFillValue;
            pDst++;
            cnt++;
            nLeft = 1;
            oldFillValue = 0;
        }
        pSrc += 2;
    }
    *pDst = '\0';
    return cnt;
}

EmergencyServiceCategory RtcEccNumberController::getCategoryFromUrn(char* urn, char *strUrn) {
    if (urn == NULL || strlen(urn) <= 0 || strlen(urn) > MAX_URN_LENGTH) {
        logE(RFX_LOG_TAG, "[%s] urn error !", __FUNCTION__);
        return UNSPECIFIED;
    }

    int cnt = gsm7bitToAscii(strUrn, urn, MAX_URN_LENGTH);
    logD(RFX_LOG_TAG, "[%s] 7BIT URN: %s, ASCII URN: %s, cnt: %d", __FUNCTION__, urn, strUrn, cnt);

    if (strcmp(strUrn, "police") == 0) {
        return POLICE;
    } else if (strcmp(strUrn, "ambulance") == 0) {
        return AMBULANCE;
    } else if (strcmp(strUrn, "fire") == 0) {
        return FIRE_BRIGADE;
    } else if (strcmp(strUrn, "marine") == 0) {
        return MARINE_GUARD;
    } else if (strcmp(strUrn, "mountain") == 0) {
        return MOUNTAIN_RESCUE;
    } else if (strcmp(strUrn, "manual") == 0) {
        return MIEC;
    } else if (strcmp(strUrn, "automatic") == 0) {
        return AIEC;
    } else if (strcmp(strUrn, "sos") == 0) {
        return UNSPECIFIED;
    } else if (strlen(strUrn) > 0) {
        return EXTENDED_SUB_SERVICE;
    }

    return UNSPECIFIED;
}
