/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2018. All rights reserved.
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

#include "TelematicsMtkRilAdapter.h"
#include "property/mtk_properties.h"
#include "log/mtk_log.h"
#include "ratconfig.h"
#include <telephony/mtk_ril_ivt.h>
#include <librilmtk/ril_ex.h>
#include <mtkconfigutils.h>
#include "telephony/mtk_ims_config.h"
#include <openssl/sha.h>
#include "ims/config/ImsConfigUtils.h"
#undef LOG_TAG
#define LOG_TAG "TelematicsMtkRilAdapter"
#define IMS_REGISTRATION_STATUS_ON 1

static const char PROPERTY_VOLTE_ENABLE[4][30] = {
    "persist.vendor.volte.enable1",
    "persist.vendor.volte.enable2",
    "persist.vendor.volte.enable3",
    "persist.vendor.volte.enable4"
};

static const char PROPERTY_VONR_ENABLE[4][30] = {
    "persist.vendor.vonr.enable1",
    "persist.vendor.vonr.enable2",
    "persist.vendor.vonr.enable3",
    "persist.vendor.vonr.enable4"
};

static const char PROPERTY_ALL_ECC[4][30] = {
    "ril.ecclist",
    "ril.ecclist1",
    "ril.ecclist2",
    "ril.ecclist3",
};

#define ICCID_HASH_LENGTH      40

extern "C" RIL_SLOT_ID mappingSocketIdToSlotId(RIL_SOCKET_ID socket_id);

extern "C" const char * requestToString(int request);

namespace mtk {

char TelematicsMtkRilAdapter::sLastBootIccId[MAX_SIM_COUNT][ICCID_HASH_LENGTH] = {{0}, {0}, {0}, {0}};
char TelematicsMtkRilAdapter::sCurrentIccId[MAX_SIM_COUNT][ICCID_HASH_LENGTH] = {{0}, {0}, {0}, {0}};
const char TelematicsMtkRilAdapter::PROPERTY_LAST_ICCID_SIM[MAX_SIM_COUNT][ICCID_HASH_LENGTH] = {
    "persist.vendor.radio.ls1icid",
    "persist.vendor.radio.ls2icid",
    "persist.vendor.radio.ls3icid",
    "persist.vendor.radio.ls4icid"
};

TelematicsMtkRilAdapter::TelematicsMtkRilAdapter(const struct RIL_Env *env) :
        MtkRilAdapter(env), mIsSmsReady(false),
        mWaitForRadioPowerOffForECall(false),
        mShouldRecoverRadioState(false),
        mRejectRadioPowerDuringECall(false),
        mEcallSlotId(-1),
        mEcallToken(NULL) {
    for (int i = 0; i < MAX_SIM_COUNT; i++) {
        mIsImsReg[i] = 0;
        mOnceStartIms[i]=0;
        mSrvccState[i] = SRVCC_STATE_NONE;
        mImsEccCount[i] = 0;
        mCsCallStateChanged[i] = false;
    }

    /* DSDS ECall feature */
    for (int i = 0; i < MAX_SIM_COUNT; i++) {
        mRadioState[i] = RADIO_STATE_OFF;
        mDesiredPowerState[i] = 0;
    }

    memset(&mEcallMessage, 0, sizeof(mEcallMessage));

    for (int slot = 0; slot < MAX_SIM_COUNT; slot++) {
        mtk_property_get(PROPERTY_LAST_ICCID_SIM[slot], sLastBootIccId[slot], "null");
        mtk_property_get(PROPERTY_LAST_ICCID_SIM[slot], sCurrentIccId[slot], "null");
    }
}

TelematicsMtkRilAdapter::~TelematicsMtkRilAdapter() {
    // TODO Auto-generated destructor stub
}

void TelematicsMtkRilAdapter::onLibRilReady() {
    mtkLogD(LOG_TAG, "Notify mtk-ril sms fwk ready");
    issueLocalRequest(RIL_REQUEST_SET_SMS_FWK_READY, NULL, 0, RIL_SOCKET_1);
#if defined(ANDROID_MULTI_SIM)
    issueLocalRequest(RIL_REQUEST_SET_SMS_FWK_READY, NULL, 0, RIL_SOCKET_2);
#endif

}

int TelematicsMtkRilAdapter::onHandleRequest(int request, void *data, size_t datalen, RIL_Token t,
        int slotId) {
    MtkRilAdapter::onHandleRequest(request, data, datalen, t, slotId);
    switch (request) {
    // case RIL_REQUEST_SET_IMS_ENABLE:
    //     handleRequestEnableImsInternal(slotId, data, t);
    //     return request;
    // case RIL_REQUEST_SET_IMSCFG:
    //     request = handleRequestSetImsCfgInternal(slotId, data, t);
    //     return request;
    case RIL_REQUEST_SEND_SMS:
    case RIL_REQUEST_SEND_SMS_EXPECT_MORE:
    case RIL_REQUEST_IMS_SEND_SMS:

    case RIL_REQUEST_IMS_SEND_SMS_EX:
        request = handleSmsRequestInternal(request, data, datalen, t, slotId);
        break;
    case RIL_REQUEST_ECALL_FAST_MAKE_ECALL:
    case RIL_REQUEST_ECALL_MAKE_ECALL:
        if (mRejectRadioPowerDuringECall && slotId != mEcallSlotId) {
            android::RequestInfo *pRI = (android::RequestInfo *)t;
            if (pRI->local != 1) {
                mtkLogI(LOG_TAG, "Reject ECall on slot %d because slot %d in ECall", slotId, mEcallSlotId);
                requestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
                return -1;
            }
        }
        request = handleRequestMakeEcall(request, data, datalen, t, slotId);
        break;
    case RIL_REQUEST_EMERGENCY_DIAL:
        mImsEccCount[slotId]++;
        break;
    case RIL_REQUEST_DIAL:
        request = handleRequestDial(request, data, datalen, t, slotId);
        break;
    case RIL_REQUEST_RADIO_POWER:
        // DSDS ECall feature. Reject radio on for opposite SIM during ECall.
        if (data != NULL) {
            int radioState = ((int *) data)[0];
            mDesiredPowerState[slotId] = radioState;
            if (mRejectRadioPowerDuringECall && radioState > 0 && slotId != mEcallSlotId) {
                mtkLogI(LOG_TAG, "Reject radio on for opposite SIM during ECall");
                requestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
                return -1;
            }
        }
        break;
    default:
        break;
    }

    if (mImsEccCount[slotId] > 0 || (mIsImsReg[slotId] == IMS_REGISTRATION_STATUS_ON
            && mSrvccState[slotId] != SRVCC_STATE_COMPLETED && !mCsCallStateChanged[slotId])) {
        request = transferToImsRequest(request);
    }
    return request;
}

int TelematicsMtkRilAdapter::transferToImsRequest(int request) {
    int actRequest = request;
    switch (request) {
    case RIL_REQUEST_DIAL:
        actRequest = RIL_REQUEST_IMS_DIAL;
        break;
    case RIL_REQUEST_EMERGENCY_DIAL:
        actRequest = RIL_REQUEST_IMS_EMERGENCY_DIAL;
        break;
    case RIL_REQUEST_GET_CURRENT_CALLS:
        actRequest = RIL_REQUEST_IMS_GET_CURRENT_CALLS;
        break;
    case RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND:
        actRequest = RIL_REQUEST_IMS_HANGUP_WAITING_OR_BACKGROUND;
        break;
    case RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND:
        actRequest = RIL_REQUEST_IMS_HANGUP_FOREGROUND_RESUME_BACKGROUND;
        break;
    case RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
        actRequest = RIL_REQUEST_IMS_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE;
        break;
    default:
        actRequest = request;
        break;
    }
    if (request != actRequest) {
        mtkLogD(LOG_TAG, "Transfer IMS request (%s) to (%s)",
                requestToString(request), requestToString(actRequest));
    }
    return actRequest;
}

int TelematicsMtkRilAdapter::onHandleSapRequest(int request, void */*data*/, size_t /*datalen*/,
        RIL_Token /*t*/, int /*slotId*/) {
    return request;
}

void TelematicsMtkRilAdapter::updateCurrerntIccidForNull(int slotId) {
    int length = snprintf(sCurrentIccId[slotId], ICCID_HASH_LENGTH, "%s", "null");
    if (length < 0) {
        mtkLogD(LOG_TAG, "updateCurrerntIccidForNull[%d], update current Icc_id error.",
                slotId);
    }
}

void TelematicsMtkRilAdapter::updateIccId(size_t datalen, int slotId, void *data) {
    char new_iccid[MTK_PROPERTY_VALUE_MAX] = { 0 };
    memcpy(new_iccid, data, datalen);
    new_iccid[MTK_PROPERTY_VALUE_MAX-1] = '\0';
    if(datalen == 0 ) {
        updateCurrerntIccidForNull(slotId);
        mtkLogD(LOG_TAG, "updateIccId[%d], length = 0, just return", slotId);
        return;
    }
    if(datalen > 0 && (strcmp(new_iccid, "N/A") == 0)) {
        updateCurrerntIccidForNull(slotId);
        mtkLogD(LOG_TAG, "updateIccId[%d], new_iccid=%s, just return", slotId, new_iccid);
        return;
    }
    std::string hashedResult = hash256(new_iccid);
    if (!hashedResult.empty()) {
        if (strlen(sCurrentIccId[slotId]) != 0
                && (strcmp(sCurrentIccId[slotId], hashedResult.c_str()) == 0)) {
            mtkLogD(LOG_TAG, "updateIccId[%d], the same value, just return", slotId);
            return;
        }
        mtkLogD(LOG_TAG, "updateIccId[%d], new_iccid[%d] = %s ->hash to-> %s",
                slotId, slotId, new_iccid, hashedResult.c_str());
        mtkLogD(LOG_TAG, "updateIccId[%d], update sCurrentIccId[%d] = %s -> %s",
                slotId, slotId, sCurrentIccId[slotId], hashedResult.c_str());
        mtkLogD(LOG_TAG, "updateIccId[%d], update sLastBootIccId[%d] = %s -> %s",
                slotId, slotId, sLastBootIccId[slotId], sCurrentIccId[slotId]);
        int length = snprintf(sLastBootIccId[slotId], ICCID_HASH_LENGTH, "%s", sCurrentIccId[slotId]);
        if (length < 0) {
            mtkLogD(LOG_TAG, "updateIccId, update sLastBootIccId[%d] fail", slotId);
        }
        length = snprintf(sCurrentIccId[slotId], ICCID_HASH_LENGTH, "%s",
                hashedResult.c_str());
        if (length < 0) {
            mtkLogD(LOG_TAG, "updateIccId[%d], hash256 ICC id error.", slotId);
        }
    } else {
        mtkLogD(LOG_TAG, "updateIccId[%d], hash256 result is empty.", slotId);
    }
}

int TelematicsMtkRilAdapter::onHandleUnsolicitedResponse(int unsolResponse, void *data,
        size_t  datalen , int slotId) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    if(unsolResponse != RIL_UNSOL_IMS_CONFIG_DYNAMIC_IMS_SWITCH_COMPLETE &&
        unsolResponse != RIL_UNSOL_ICCID_CHANGED && unsolResponse != RIL_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED &&
        unsolResponse != RIL_UNSOL_IMS_REGISTRATION_INFO) {
            //mtkLogI(LOG_TAG, "onHandleUnsolicitedResponse MTK_HYP_DUAL_TELE_SUPPORT yes ,don't support telematics adapter. just return");
            return unsolResponse;
        }
#endif
    switch (unsolResponse) {
    case RIL_UNSOL_IMS_REGISTRATION_INFO:
        handleImsRegStateResponse(data, slotId);
        break;
    case RIL_UNSOL_SMS_READY_NOTIFICATION:
        mIsSmsReady = true;
        break;
    case RIL_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED:
        mCsCallStateChanged[slotId] = false;
        return RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED;
    case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED:
        mCsCallStateChanged[slotId] = true;
        break;
    // case RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED:
    //     return RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED;
    case RIL_UNSOL_SRVCC_STATE_NOTIFY:
        onHandleUrcSrvcc(data, slotId);
        break;
    case RIL_UNSOL_ECALL_INDICATIONS:
        onHandleEcallIndications(data, slotId);
        break;
    case RIL_UNSOL_IMS_CONFIG_DYNAMIC_IMS_SWITCH_COMPLETE:
        handleImsConfigDynamicImsSwitchComplete(slotId);
        break;
    case RIL_UNSOL_ICCID_CHANGED:
        updateIccId(datalen, slotId, data);
        break;
    default:
        break;
    }
    return unsolResponse;
}

void TelematicsMtkRilAdapter::handleImsRegStateResponse(const void *data, int slotId) {
    if (data != NULL) {
        mIsImsReg[slotId] = ((int *) data)[0];
    }
}

void TelematicsMtkRilAdapter::onHandleUrcSrvcc(const void *data, int slotId) {
    if (data != NULL) {
        mSrvccState[slotId] = ((int *) data)[0];
        mtkLogD(LOG_TAG, "onHandleUrcSrvcc SRVCC state %d", mSrvccState[slotId]);

        if (mSrvccState[slotId] == SRVCC_STATE_COMPLETED && mImsEccCount[slotId] > 0) {
            mImsEccCount[slotId] = 0;
        }
    }
}

int TelematicsMtkRilAdapter::onHandleSapUnsolicitedResponse(int unsolResponse, void */*data*/,
        size_t /*datalen*/, int /*slotId*/) {
    return unsolResponse;
}

void TelematicsMtkRilAdapter::onHandleSetRadioState(RIL_RadioState oldState,
        RIL_RadioState newState, RIL_SOCKET_ID socId) {
    mtkLogD(LOG_TAG, "onHandleSetRadioState[%d] newState=%d", socId, newState);
    mRadioState[socId] = newState;
    if (oldState != newState && newState == RADIO_STATE_ON) {

        mtkLogD(LOG_TAG, "onHandleSetRadioState, set ims config cause radio power on, newState=%d mOnceStartIms[%d]=%d", newState, socId, mOnceStartIms[socId]);
        if (mOnceStartIms[socId] == 1) {
            mtkLogD(LOG_TAG, "onHandleSetRadioState, set ims config again, radio power on, before RIL_UNSOL_IMS_CONFIG_DYNAMIC_IMS_SWITCH_COMPLETE failed");
            handleImsConfigDynamicImsSwitchComplete(socId);
        }
    } else if (oldState != newState && newState == RADIO_STATE_OFF) {
        mOnceStartIms[socId] == 0;
    }

    // DSDS ECall feature
    // Send fast make ecall request after power off radio of opposite SIM.
    // See more in turnOffRadioBeforeFastECall().
    if (mWaitForRadioPowerOffForECall && newState == RADIO_STATE_OFF
            && socId != mEcallSlotId) {
        mtkLogD(LOG_TAG, "onHandleSetRadioState, ok to make fast ecall");
        RIL_ECallReqMsg message;
        copyEcallData(&message, &mEcallMessage);
        free(mEcallMessage.address);
        free(mEcallMessage.msd_data);
        mEcallMessage.address = NULL;
        mEcallMessage.msd_data = NULL;

        mWaitForRadioPowerOffForECall = false;
        issueLocalRequest(RIL_REQUEST_ECALL_FAST_MAKE_ECALL,
                &message, sizeof(message), (RIL_SOCKET_ID)mEcallSlotId);
        free(message.address);
        free(message.msd_data);
    }
}

void TelematicsMtkRilAdapter::onHandleRequestComplete(RIL_Token t, RIL_Errno e, void *response,
        size_t responselen) {
    if (t == NULL) return;
    android::RequestInfo *pRI = (android::RequestInfo *)t;
    int request = pRI->pCI->requestNumber;
    int origRequest = request;

    switch (request) {
    case RIL_REQUEST_GET_CURRENT_CALLS:
        handleGetCurrentCallsResponse(pRI, e, response, responselen);
        break;
    case RIL_REQUEST_ECALL_FAST_MAKE_ECALL:
    case RIL_REQUEST_ECALL_MAKE_ECALL:
        handleMakeECallResponse(pRI, e, response, responselen);
        break;
    case RIL_REQUEST_DIAL:
    case RIL_REQUEST_EMERGENCY_DIAL:
        handleEmergencyDialResponse(pRI, e, response, responselen);
        break;
    default:
        break;
    }
}

void TelematicsMtkRilAdapter::onHandleSapRequestComplete(RIL_Token /*t*/, RIL_Errno /*e*/, void */*response*/,
        size_t /*responselen*/) {

}

void TelematicsMtkRilAdapter::handleGetCurrentCallsResponse(
        android::RequestInfo *pRI, RIL_Errno e, void */*response*/, size_t responselen) {
    int slotId = (int) pRI->socket_id;

    if (e == RIL_E_SUCCESS && responselen == 0) {
        if (mSrvccState[slotId] != SRVCC_STATE_NONE) {
            mtkLogD(LOG_TAG, "handleGetCurrentCallsResponse reset SRVCC state on slot%d", slotId);
            mSrvccState[slotId] = SRVCC_STATE_NONE;
        }
        if (mImsEccCount[slotId] > 0) {
            mtkLogD(LOG_TAG, "handleGetCurrentCallsResponse reset mImsEccCount on slot%d", slotId);
            mImsEccCount[slotId] = 0;
        }
        if (mCsCallStateChanged[slotId]) {
            mtkLogD(LOG_TAG, "handleGetCurrentCallsResponse reset mCsCallStateChanged on slot%d",
                    slotId);
            mCsCallStateChanged[slotId] = false;
        }
    }
    mtkLogD(LOG_TAG, "handleGetCurrentCallsResponse e %d, SRVCC state: %d, ImsEccCount: %d, "
            "CsCallStateChanged: %d",
            e, mSrvccState[slotId], mImsEccCount[slotId], mCsCallStateChanged[slotId]);
}

int TelematicsMtkRilAdapter::handleRequestMakeEcall(
    int request, void * data , size_t datalen, RIL_Token t, int slotId) {

    char prop_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("persist.vendor.ims_ecall_support", prop_value, "0");
    int imsEcallSupport = atoi(prop_value);

    if (mIsImsReg[slotId] == IMS_REGISTRATION_STATUS_ON) {
        if (request == RIL_REQUEST_ECALL_FAST_MAKE_ECALL) {
            if (!imsEcallSupport) {
                mSrvccState[slotId] = SRVCC_STATE_COMPLETED;
            } else {
                mImsEccCount[slotId]++;
            }
        } else {
            mSrvccState[slotId] = SRVCC_STATE_COMPLETED;
        }
    }

    if (imsEcallSupport) {
        mImsEccCount[slotId]++;
    }
    mtkLogD(LOG_TAG, "handleRequestMakeEcall Set SRVCC state %d, ImsEccCount %d",
            mSrvccState[slotId], mImsEccCount[slotId]);

    // DSDS ECall feature
    if (request == RIL_REQUEST_ECALL_FAST_MAKE_ECALL) {
        return turnOffRadioBeforeFastECall(request, data, datalen, t, slotId);
    }

    return request;
}

int TelematicsMtkRilAdapter::handleRequestDial(
        int request, void * data, size_t /*datalen*/, RIL_Token t, int slotId) {
    int ret = 0;
    if (data != NULL) {
        RIL_Dial *pDial = (RIL_Dial*) data;
        String8 dialNumber = String8::format("%s", pDial->address);
        char eccListProp[MTK_PROPERTY_VALUE_MAX] = {0};
        mtk_property_get(PROPERTY_ALL_ECC[slotId], eccListProp, "");
        String8 eccList = String8(eccListProp);
        mtkLogD(LOG_TAG, "handleRequestDial All ECC eccList: %s", eccList.string());
        if (isEccMatchInList(dialNumber, eccList)) {
            mImsEccCount[slotId]++;
            mtkLogD(LOG_TAG, "handleRequestDial isEmergencyDial: %s", dialNumber.string());
        } else if (mImsEccCount[slotId] > 0) {
            mtkLogI(LOG_TAG, "handleRequestDial reject normal dial when IMS ECC exist: %s",
                    dialNumber.string());
            requestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
            return -1;
        }
    } else {
        mtkLogE(LOG_TAG, "handleRequestDial request data is null");
    }
    return request;
}

bool TelematicsMtkRilAdapter::isEccMatchInList(String8 number, String8 eccList) {
    // Add match boundary char ',' for easy match
    String8 searchEccList = String8(",") + eccList + String8(",");
    if (searchEccList.find(String8(",") + number + String8(",")) != -1) {
        return true;
    }

    return false;
}

void TelematicsMtkRilAdapter::handleEmergencyDialResponse(
        android::RequestInfo *pRI, RIL_Errno e, void *response, size_t responselen) {
    int slotId = (int) pRI->socket_id;
    if (e != RIL_E_SUCCESS && mImsEccCount[slotId] > 0) {
        mImsEccCount[slotId]--;
    }
    mtkLogD(LOG_TAG, "handleEmergencyDialResponse e:%d, ImsEccCount %d", e, mImsEccCount[slotId]);
}

void TelematicsMtkRilAdapter::handleMakeECallResponse(
        android::RequestInfo *pRI, RIL_Errno e, void *response, size_t responselen) {
    // DSDS ECall feature. If call blocked because another eCall already exists, should return fail,
    // and don't recover radio.
    if (e == RIL_E_REQUEST_RATE_LIMITED) {
        if (mEcallToken != NULL) {
            requestComplete(mEcallToken, RIL_E_GENERIC_FAILURE, NULL, 0);
            mEcallToken = NULL;
        }
        return;
    }

    // Recover the radio of opposite SIM.
    if (e != RIL_E_SUCCESS) {
        mRejectRadioPowerDuringECall = false;
        if (mShouldRecoverRadioState) {
            mShouldRecoverRadioState = false;
            requestOppositeRadioPower(1, mEcallSlotId);
        }
    }

    // In other cases, return response normally.
    if (mEcallToken != NULL) {
        requestComplete(mEcallToken, e, NULL, 0);
        mEcallToken = NULL;
    }

    // DSDS ECall feature - End

    int slotId = (int) pRI->socket_id;
    if (e != RIL_E_SUCCESS && mSrvccState[slotId] != SRVCC_STATE_NONE) {
        mSrvccState[slotId] = SRVCC_STATE_NONE;
    }
    mtkLogD(LOG_TAG, "handleMakeECallResponse e:%d, SRVCC state %d", e, mSrvccState[slotId]);
}

void TelematicsMtkRilAdapter::handleRequestEnableImsInternal(int slotId, void *data, RIL_Token t) {
    if (data != NULL) {

        android::RequestInfo *pRI = (android::RequestInfo *)t;
        RIL_SOCKET_ID socId = pRI->socket_id;
        int *enable = (int*)data;
        sendEnableImsRequestInternal(socId, enable[0]);
        mtkLogD(LOG_TAG, "handleRequestEnableImsInternal request data is %d, slotId: %d", enable[0], slotId);
    } else {
        mtkLogE(LOG_TAG, "handleRequestEnableImsInternal request data is null");
    }
}

void TelematicsMtkRilAdapter::sendEnableImsRequestInternal(RIL_SOCKET_ID socId, int volte_enable) {
    //int slotId = mappingSocketIdToSlotId(socId);
    int slotId = socId;
    char prop_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get(PROPERTY_VOLTE_ENABLE[slotId], prop_value, "0");
    int volte = atoi(prop_value);
    int vonr = 0;
    if(!volte) {
        mtkLogW(LOG_TAG, "sendEnableImsRequestInternal[%d], volte is disabled, just return", slotId);
        return;
    } else {
        char prop_vonr_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
        mtk_property_get(PROPERTY_VONR_ENABLE[slotId], prop_vonr_value, "0");
        vonr = atoi(prop_vonr_value);
        mtkLogW(LOG_TAG, "sendEnableImsRequestInternal[%d], vonr: %d", slotId, vonr);
    }

    if (volte_enable == 1) {
        int volte[4] = {FEATURE_TYPE_VOICE_OVER_LTE, NETWORK_TYPE_LTE, 1, -1};
        issueLocalRequest(RIL_REQUEST_IMS_CONFIG_SET_FEATURE, volte, 4 * sizeof(int), socId);
        mtkLogD(LOG_TAG, "sendEnableImsRequestInternal[%d], request to enable IMS VOLTE",socId);

        if(vonr) {
            int vonr[4] = {FEATURE_TYPE_VOICE_OVER_NR, NETWORK_TYPE_NR, 1, 1};
            issueLocalRequest(RIL_REQUEST_IMS_CONFIG_SET_FEATURE, vonr, 4 * sizeof(int), socId);
            mtkLogD(LOG_TAG, "sendEnableImsRequestInternal[%d], request to enable IMS VONR",socId);
        }
    } else {
        int volte[4] = {FEATURE_TYPE_VOICE_OVER_LTE, NETWORK_TYPE_LTE, 0, -1};
        issueLocalRequest(RIL_REQUEST_IMS_CONFIG_SET_FEATURE, volte, 4 * sizeof(int), socId);
        mtkLogD(LOG_TAG, "endEnableImsRequestInternal[%d], request to disable IMS VOLTE", socId);

        int vonr[4] = {FEATURE_TYPE_VOICE_OVER_NR, NETWORK_TYPE_NR, 0, 1};
        issueLocalRequest(RIL_REQUEST_IMS_CONFIG_SET_FEATURE, vonr, 4 * sizeof(int), socId);
        mtkLogD(LOG_TAG, "endEnableImsRequestInternal[%d], request to disable IMS VONR", socId);
    }
}

void TelematicsMtkRilAdapter::handleImsConfigDynamicImsSwitchComplete(RIL_SOCKET_ID socId) {
    if(mRadioState[socId] != RADIO_STATE_ON){
        mOnceStartIms[socId] = 1;
        mtkLogD(LOG_TAG, "handleImsConfigDynamicImsSwitchComplete[%d],radio isn't ON, mRadioState[%d]=%d, "
                "just return.", socId, socId, mRadioState[socId]);
        return;
    }

    if(strcmp(sCurrentIccId[socId], "null") == 0) {
        mtkLogD(LOG_TAG, "handleImsConfigDynamicImsSwitchComplete[%d],found null ICCID, just return.", socId);
        return;
    }
    if (strcmp(sCurrentIccId[socId], sLastBootIccId[socId]) == 0) {
        if(isOp09SimCard(socId)){
            int volte = ImsConfigUtils::getFeaturePropValue(ImsConfigUtils::PROPERTY_VOLTE_ENALBE, socId);
            mtkLogD(LOG_TAG,"handleImsConfigDynamicImsSwitchComplete[%d], enable VOLTE = %d,"
                    "for OP09SIM Card",socId, volte);
            if (volte) {
                sendEnableImsRequestInternal(socId, volte);
            }
        } else {
            mtkLogD(LOG_TAG, "handleImsConfigDynamicImsSwitchComplete[%d],found same ICCID, just return."
                "sCurrentIccId[%d]=%s sLastBootIccId[%d] = %s",
                socId,socId,sCurrentIccId[socId], socId, sLastBootIccId[socId]);
        }
        return;
    } else {
        int length = snprintf(sLastBootIccId[socId], ICCID_HASH_LENGTH, "%s", sCurrentIccId[socId]);
        if (length < 0) {
            mtkLogD(LOG_TAG, "handleImsConfigDynamicImsSwitchComplete[%d], update sLastBootIccId fail", socId);
        }
    }
    sendEnableImsRequestInternal(socId, 1);
}

int TelematicsMtkRilAdapter::handleRequestSetImsCfgInternal(int slotId, void *data, RIL_Token t) {
    if (data != NULL) {
        int *enable = (int*)data;
        mtkLogD(LOG_TAG, "[soltId:%d]handleRequestSetImsCfgInternal %d,%d,%d,%d,%d,%d",
                slotId, enable[0], enable[1], enable[2], enable[3], enable[4], enable[5]);
        requestComplete(t, RIL_E_SUCCESS, NULL, 0);
        sendEnableImsRequestInternal(slotId, enable[0]);
    } else {
        mtkLogE(LOG_TAG, "handleRequestSetImsCfgInternal request data is null");
        requestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
    return -1;
}

int TelematicsMtkRilAdapter::handleSmsRequestInternal(int request, void */*data*/, size_t /*datalen*/,
        RIL_Token t, int slotId) {
    // if (!mIsSmsReady) {
    //     mtkLogI(LOG_TAG, "sms md not ready, return error!");
    //     requestComplete(t, RIL_E_SMS_SEND_FAIL_RETRY, NULL, 0);
    //     return -1;
    // }
    if (RIL_REQUEST_IMS_SEND_SMS == request || RIL_REQUEST_IMS_SEND_SMS_EX == request) {
        if (!mIsImsReg[slotId]) {
            mtkLogI(LOG_TAG, "ims not ready, can't send ims sms, return error!");
            requestComplete(t, RIL_E_SMS_SEND_FAIL_RETRY, NULL, 0);
            return -1;
        }
    }
    return request;
}

const std::string TelematicsMtkRilAdapter::hash256(const char* str) {
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    SHA256_Final(hash, &sha256);

    std::string tmp = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        int length = snprintf(buf, sizeof(buf), "%02x", hash[i]);
        if (length >= 0) {
            tmp = tmp + buf;
        }
    }
    return tmp;
}

bool TelematicsMtkRilAdapter::isOp09SimCard(int slot_id) {
    std::string iccidPre("gsm.sim.preiccid_");
    iccidPre.append(String8::format("%d", slot_id).string());
    char prop_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get(iccidPre.c_str(), prop_value, "0");
    bool isOp09Card = ImsConfigUtils::isOp09SimCard(prop_value);
    mtkLogI(LOG_TAG, "isOp09SimCard[%d], isOp09Card:%d", slot_id, isOp09Card);
    return isOp09Card;
}

// DSDS ECall feature - Start
/*
    Turn off radio power of opposite SIM before ECall if needed.
*/
int TelematicsMtkRilAdapter::turnOffRadioBeforeFastECall(int request, void *data, size_t datalen,
        RIL_Token t, int slotId) {
    FeatureValue featurevalue;
    memset(featurevalue.value, 0, sizeof(featurevalue.value));
    mtkGetFeature(CONFIG_SIM_MODE, &featurevalue);

    if (strncmp(featurevalue.value, "dsds", strlen("dsds")) == 0 && data != NULL) {
        mRejectRadioPowerDuringECall = true;
        mEcallSlotId = slotId;
        mtkLogD(LOG_TAG, "set mEcallSlotId = %d", mEcallSlotId);
        int oppositeSlotId = (slotId == 0) ? 1 : 0;
        mtkLogD(LOG_TAG, "current opposite mRadioState = %d, mDesiredPowerState = %d",
                mRadioState[oppositeSlotId], mDesiredPowerState[oppositeSlotId]);
        if (mRadioState[oppositeSlotId] != RADIO_STATE_OFF
                || mDesiredPowerState[oppositeSlotId] > 0) {
            // Store fast make ecall message
            copyEcallData(&mEcallMessage, (RIL_ECallReqMsg *)data);
            mWaitForRadioPowerOffForECall = true;
            mShouldRecoverRadioState = true;
            requestOppositeRadioPower(0, slotId);
            mEcallToken = t;
            // Don't send ECall request or response now, need to wait until radio on.
            return -1;
        }
    }

    return request;
}
/*
    Set the radio of opposite SIM.
*/
void TelematicsMtkRilAdapter::requestOppositeRadioPower(int power, int slotId) {
    RIL_SOCKET_ID oppositeSocketId = (slotId == 1) ? RIL_SOCKET_1 : RIL_SOCKET_2;
    mtkLogD(LOG_TAG, "requestOppositeRadioPower, oppositeSocketId = %d", oppositeSocketId);
    issueLocalRequest(RIL_REQUEST_RADIO_POWER, &power, sizeof(int), oppositeSocketId);
}

/*
    Indications received to recover the radio of opposite SIM.
    RIL_UNSOL_ECALL_ONLY_DEREGISTRATION reported when the following condications are fullfilled:
        1. ECall ended
        2. ECall SIM type is SIM_ECALL_ONLY
        3. Receive indication of expiration of deregist
    RIL_UNSOL_ECALL_MAY_DEREGISTER reported when the following condications are fullfilled:
        1. ECall ended
        2. ECall SIM type is not SIM_ECALL_ONLY
        3. T9 timer configured by "vendor.ecall.timer9.sec" expired
    BTW, also need to send ECall indication to upper layer.
*/
void TelematicsMtkRilAdapter::onHandleEcallIndications(void *data, int slotId) {
    if (data != NULL) {
        RIL_ECall_Indication ind = ((RIL_Ecall_Unsol_Indications *) data)->ind;
        mtkLogD(LOG_TAG, "onHandleEcallIndications, ind = %d", ind);
        if (ind == RIL_UNSOL_ECALL_ONLY_DEREGISTRATION || ind == RIL_UNSOL_ECALL_MAY_DEREGISTER) {
            mRejectRadioPowerDuringECall = false;
            if (mShouldRecoverRadioState) {
                mShouldRecoverRadioState = false;
                requestOppositeRadioPower(1, mEcallSlotId);
            }
        }
    }
}

void TelematicsMtkRilAdapter::copyEcallData(RIL_ECallReqMsg *dst, RIL_ECallReqMsg *src) {
    dst->ecall_cat = src->ecall_cat;
    dst->ecall_variant = src->ecall_variant;
    int size = strlen(src->address) + 1;
    dst->address = (char *)calloc(1, size);
    if (dst->address != NULL) {
        strncpy(dst->address, src->address, size);
    }
    dst->length = src->length;
    dst->msd_data = (unsigned char *)calloc(1, src->length);
    if (dst->msd_data != NULL) {
        memcpy(dst->msd_data, src->msd_data, src->length);
    }
}
// DSDS ECall feature - End
} /* namespace mtk */
