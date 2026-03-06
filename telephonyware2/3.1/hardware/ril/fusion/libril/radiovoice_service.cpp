/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2022. All rights reserved.
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

#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <radiovoice_service.h>
#include <ril_service.h>
#include <mtk_log.h>
#include <binder/ProcessState.h>
#include <android/binder_manager.h>
#include <rilAidlUtils.h>
#include <mtk_properties.h>
#include "rilAidlUtils.h"
#include "ril_context.h"

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_radio_network = ::aidl::android::hardware::radio::network;

using rilAidlUtils::copyAidlStringToRil;
using rilAidlUtils::populateAidlResponseInfo;
using rilAidlUtils::retChecker;

struct RadioVoiceImpl {
    std::shared_ptr<mtk_voice::IMtkRadioExVoiceResponse> mtkVoiceResponse;
    std::shared_ptr<mtk_voice::IMtkRadioExVoiceIndication> mtkVoiceIndication;
    std::shared_ptr<mtk_voice::IMtkRadioExVoiceResponse> mtkImsVoiceResponse;
    std::shared_ptr<mtk_voice::IMtkRadioExVoiceIndication> mtkImsVoiceIndication;
    std::shared_ptr<aosp_voice::IRadioVoiceResponse> voiceResponse;
    std::shared_ptr<aosp_voice::IRadioVoiceIndication> voiceIndication;
};

RadioVoiceImpl radioVoiceServiceImpl[MAX_SIM_COUNT*(android::CLIENT_TBOX_IMS + 1)];

static bool isValidSlotId(int slotId) {
    return (slotId >= 0) &&
            (slotId < sizeof(radioVoiceServiceImpl) / sizeof(radioVoiceServiceImpl[0]));
}

static aosp_voice::AudioQuality convertCallsSpeechCodecToAidlAudioQuality(int speechCodec) {
    /**
     *   MtkSpeechCodecTypes
     *     NONE(0),
     *     QCELP13K(0x0001),
     *     EVRC(0x0002),
     *     EVRC_B(0x0003),
     *     EVRC_WB(0x0004),
     *     EVRC_NW(0x0005),
     *     AMR_NB(0x0006),
     *     AMR_WB(0x0007),
     *     GSM_EFR(0x0008),
     *     GSM_FR(0x0009),
     *     GSM_HR(0x000A);
     */
    switch (speechCodec) {
    case 0x0006:
        return aosp_voice::AudioQuality::AMR;
    case 0x0007:
        return aosp_voice::AudioQuality::AMR_WB;
    case 0x0008:
        return aosp_voice::AudioQuality::GSM_EFR;
    case 0x0009:
        return aosp_voice::AudioQuality::GSM_FR;
    case 0x000A:
        return aosp_voice::AudioQuality::GSM_HR;
    case 0x0002:
        return aosp_voice::AudioQuality::EVRC;
    case 0x0003:
        return aosp_voice::AudioQuality::EVRC_B;
    case 0x0004:
        return aosp_voice::AudioQuality::EVRC_WB;
    case 0x0005:
        return aosp_voice::AudioQuality::EVRC_NW;
    default:
        return aosp_voice::AudioQuality::UNSPECIFIED;
    }
}

bool dispatchCallForwardStatus(int serial, int slotId, int request,
        const aosp_voice::CallForwardInfo& callInfo) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_CallForwardInfo cf;
    cf.status = (int) callInfo.status;
    cf.reason = callInfo.reason;
    cf.serviceClass = callInfo.serviceClass;
    cf.toa = callInfo.toa;
    cf.timeSeconds = callInfo.timeSeconds;

    if (!copyAidlStringToRil(&cf.number, callInfo.number, pRI)) {
        return false;
    }

    CALL_ONREQUEST(request, &cf, sizeof(cf), pRI, slotId);

    memsetAndFreeStrings(1, cf.number);

    return true;
}

bool dispatchCallForwardInTimeSlotStatus(int serial, int slotId, int request,
        const mtk_voice::CallForwardInfoEx& callInfoEx) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_CallForwardInfoEx cfEx;
    cfEx.status = (int) callInfoEx.status;
    cfEx.reason = callInfoEx.reason;
    cfEx.serviceClass = callInfoEx.serviceClass;
    cfEx.toa = callInfoEx.toa;
    cfEx.timeSeconds = callInfoEx.timeSeconds;

    if (!copyAidlStringToRil(&cfEx.number, callInfoEx.number, pRI)) {
        return false;
    }

    if (!copyAidlStringToRil(&cfEx.timeSlotBegin, callInfoEx.timeSlotBegin, pRI)) {
        memsetAndFreeStrings(1, cfEx.number);
        return false;
    }

    if (!copyAidlStringToRil(&cfEx.timeSlotEnd, callInfoEx.timeSlotEnd, pRI)) {
        memsetAndFreeStrings(2, cfEx.number, cfEx.timeSlotBegin);
        return false;
    }

    CALL_ONREQUEST(request, &cfEx, sizeof(cfEx), pRI, pRI->socket_id);

    memsetAndFreeStrings(3, cfEx.number, cfEx.timeSlotBegin, cfEx.timeSlotEnd);

    return true;
}

// AOSP radio
// response
int radioVoice::acceptCallResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen) {
    mtkLogD(LOG_TAG, "acceptCallResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->acceptCallResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "acceptCallResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "acceptCallResponse: "
                "radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }

    return 0;
}

int radioVoice::conferenceResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "conferenceResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->conferenceResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "conferenceResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "conferenceResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::dialResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "dialResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->dialResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "dialResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "dialResponse: radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::emergencyDialResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "emergencyDialResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->emergencyDialResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "emergencyDialResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "emergencyDialResponse:"
                "radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::exitEmergencyCallbackModeResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "exitEmergencyCallbackModeResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                exitEmergencyCallbackModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitEmergencyCallbackModeResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "exitEmergencyCallbackModeResponse:"
                "radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::explicitCallTransferResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "explicitCallTransferResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                explicitCallTransferResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "explicitCallTransferResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "explicitCallTransferResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::getCallForwardStatusResponse(unsigned int slotId, ClientId clientId,
                                int responseType, int serial, RIL_Errno e, const void *response,
                                size_t responseLen) {
    mtkLogD(LOG_TAG, "getCallForwardStatusResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<aosp_voice::CallForwardInfo> callForwardInfos;

        if (response == NULL || responseLen % sizeof(RIL_CallForwardInfo *) != 0) {
            mtkLogE(LOG_TAG, "getCallForwardStatusResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int num = responseLen / sizeof(RIL_CallForwardInfo *);
            callForwardInfos.resize(num);
            for (int i = 0 ; i < num; i++) {
                RIL_CallForwardInfo *resp = ((RIL_CallForwardInfo **) response)[i];
                callForwardInfos[i].status = resp->status;
                callForwardInfos[i].reason = resp->reason;
                callForwardInfos[i].serviceClass = resp->serviceClass;
                callForwardInfos[i].toa = resp->toa;
                callForwardInfos[i].number = convertCharPtrToStdString(resp->number);
                callForwardInfos[i].timeSeconds = resp->timeSeconds;
            }
        }

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->getCallForwardStatusResponse(
                responseInfo, callForwardInfos);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCallForwardStatusResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
            "getCallForwardStatusResponse: radioVoiceServiceImpl[%d]->mRadioResponse == NULL",
            slotId);
    }

    return 0;
}

int radioVoice::getCallWaitingResponse(unsigned int slotId, ClientId clientId,
                          int responseType, int serial, RIL_Errno e, const void *response,
                          size_t responseLen) {
    mtkLogD(LOG_TAG, "getCallWaitingResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        bool enable = false;
        int serviceClass = -1;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || numInts != 2) {
            mtkLogE(LOG_TAG, "getCallWaitingResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            enable = pInt[0] == 1 ? true : false;
            serviceClass = pInt[1];
        }

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->getCallWaitingResponse(
                responseInfo, enable, serviceClass);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCallWaitingResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
            "getCallWaitingResponse: radioVoiceServiceImpl[%d]->mRadioResponse == NULL", slotId);
    }

    return 0;
}

int radioVoice::getClipResponse(unsigned int slotId, ClientId clientId,
                   int responseType, int serial, RIL_Errno e, const void *response,
                   size_t responseLen) {
    mtkLogD(LOG_TAG, "getClipResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        int val = -1;
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "getClipResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *p_int = (int *) response;
            val = p_int[0];
        }

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->getClipResponse(
                responseInfo, (aosp_voice::ClipStatus) val);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getClipResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getClipResponse: radioVoiceServiceImpl[%d]->mRadioResponse == NULL", slotId);
    }

    return 0;
}

int radioVoice::getClirResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getClirResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        int n = -1, m = -1;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || numInts != 2) {
            mtkLogE(LOG_TAG, "getClirResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            n = pInt[0];
            m = pInt[1];
        }

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->getClirResponse(
                responseInfo, n, m);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getClirResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getClirResponse: radioVoiceServiceImpl[%d]->mRadioResponse == NULL", slotId);
    }

    return 0;
}

int radioVoice::getCurrentCallsResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getCurrentCallsResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        std::vector<aosp_voice::Call> calls;
        if (response == NULL || (responseLen % sizeof(RIL_Call *)) != 0) {
            mtkLogE(LOG_TAG, "getCurrentCallsResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                e = RIL_E_INVALID_RESPONSE;
            }
        } else {
            int num = responseLen / sizeof(RIL_Call *);
            calls.resize(num);
            for (int i = 0 ; i < num ; i++) {
                RIL_Call *p_cur = ((RIL_Call **) response)[i];
                /* each call info */
                calls[i].state = p_cur->state;
                calls[i].index = p_cur->index;
                calls[i].toa = p_cur->toa;
                calls[i].isMpty = p_cur->isMpty;
                calls[i].isMT = p_cur->isMT;
                calls[i].als = p_cur->als;
                calls[i].isVoice = p_cur->isVoice;
                calls[i].isVoicePrivacy = p_cur->isVoicePrivacy;
                calls[i].number = convertCharPtrToStdString(p_cur->number);
                calls[i].numberPresentation = p_cur->numberPresentation;
                calls[i].name = convertCharPtrToStdString(p_cur->name);
                calls[i].namePresentation = p_cur->namePresentation;
                if (p_cur->uusInfo != NULL && p_cur->uusInfo->uusData != NULL) {
                    RIL_UUS_Info *uusInfo = p_cur->uusInfo;
                    calls[i].uusInfo.resize(1);
                    calls[i].uusInfo[0].uusType = uusInfo->uusType;
                    calls[i].uusInfo[0].uusDcs =  uusInfo->uusDcs;
                    // convert uusInfo->uusData to a null-terminated string
                    char *nullTermStr = strndup(uusInfo->uusData, uusInfo->uusLength);
                    calls[i].uusInfo[0].uusData = convertCharPtrToStdString(nullTermStr);
                    free(nullTermStr);
                }
                calls[i].audioQuality =
                        convertCallsSpeechCodecToAidlAudioQuality(p_cur->speechCodec);
                calls[i].forwardedNumber = convertCharPtrToStdString(p_cur->forwardedNumber);
            }
        }
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                getCurrentCallsResponse(responseInfo, calls);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCurrentCallsResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getCurrentCallsResponse:"
                "radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::getLastCallFailCauseResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getLastCallFailCauseResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aosp_voice::LastCallFailCauseInfo info = {};
        info.vendorCause = std::string();
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getLastCallFailCauseResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) e = RIL_E_INVALID_RESPONSE;
        } else if (responseLen == sizeof(int)) {
            int *pInt = (int *) response;
            info.causeCode = aosp_voice::LastCallFailCause(pInt[0]);
        } else if (responseLen == sizeof(RIL_LastCallFailCauseInfo))  {
            RIL_LastCallFailCauseInfo *pFailCauseInfo = (RIL_LastCallFailCauseInfo *) response;
            info.causeCode = aosp_voice::LastCallFailCause(pFailCauseInfo->cause_code);
            info.vendorCause = convertCharPtrToStdString(pFailCauseInfo->vendor_cause);
        } else {
            mtkLogE(LOG_TAG, "getLastCallFailCauseResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) e = RIL_E_INVALID_RESPONSE;
        }
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                getLastCallFailCauseResponse(responseInfo, info);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getLastCallFailCauseResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getLastCallFailCauseResponse:"
                "radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::getMuteResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getMuteResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        bool enable = false;
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "getMuteResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) e = RIL_E_INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            enable = pInt[0] == 1 ? true : false;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                getMuteResponse(responseInfo, enable);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getMuteResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getMuteResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::getPreferredVoicePrivacyResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getPreferredVoicePrivacyResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        bool enable = false;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || numInts != 1) {
            mtkLogE(LOG_TAG, "getPreferredVoicePrivacyResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) e = RIL_E_INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            enable = pInt[0] == 1 ? true : false;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->getPreferredVoicePrivacyResponse(
                responseInfo, enable);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getPreferredVoicePrivacyResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getPreferredVoicePrivacyResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::getTTYModeResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getTTYModeResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aosp_voice::TtyMode mode = aosp_voice::TtyMode::OFF;
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "getTTYModeResponse: Invalid response");
            if (e == RIL_E_SUCCESS) e = RIL_E_INVALID_RESPONSE;
        } else {
            int *p_int = (int *) response;
            mode = aosp_voice::TtyMode(p_int[0]);
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->getTtyModeResponse(
                responseInfo, mode);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getTTYModeResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getTTYModeResponse: radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::handleStkCallSetupRequestFromSimResponse(unsigned int slotId, ClientId clientId,
                                            int responseType, int serial, RIL_Errno e,
                                            const void *response, size_t responselen) {
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        mtkLogD(LOG_TAG, "handleStkCallSetupRequestFromSimResponse: serial %d", serial);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                handleStkCallSetupRequestFromSimResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG,
                    "handleStkCallSetupRequestFromSimResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "handleStkCallSetupRequestFromSimResponse:"
                "radioVoiceServiceImpl[%d]->voiceResponse == NULL", slotId);
    }
    return 0;
}

int radioVoice::hangupConnectionResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "hangupConnectionResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->hangupConnectionResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "hangupConnectionResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "hangupConnectionResponse: radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::hangupForegroundResumeBackgroundResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "hangupForegroundResumeBackgroundResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->hangupForegroundResumeBackgroundResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "hangupForegroundResumeBackgroundResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "hangupForegroundResumeBackgroundResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::hangupWaitingOrBackgroundResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "hangupWaitingOrBackgroundResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->hangupWaitingOrBackgroundResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "hangupWaitingOrBackgroundResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "hangupWaitingOrBackgroundResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }

    return 0;
}
// new added
int radioVoice::isVoNrEnabledResponse(unsigned int slotId, ClientId clientId,
                                     int responseType, int serial, RIL_Errno e, const void *response,
                                     size_t responselen) {
    mtkLogD(LOG_TAG, "isVoNrEnabledResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        bool enable = 0;
        if (response == NULL || responselen != sizeof(int)) {
            mtkLogE(LOG_TAG, "isVoNrEnabledResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                e = RIL_E_INVALID_RESPONSE;
            }
        } else {
            int *pInt = (int *) response;
            enable = pInt[0] == 1 ? true : false;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->isVoNrEnabledResponse(
                responseInfo, enable);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "isVoNrEnabledResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "isVoNrEnabledResponse: radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::rejectCallResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "rejectCallResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->rejectCallResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "rejectCallResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "rejectCallResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::sendBurstDtmfResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendBurstDtmfResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->sendBurstDtmfResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendBurstDtmfResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "sendBurstDtmfResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::sendCDMAFeatureCodeResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendCDMAFeatureCodeResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->sendCdmaFeatureCodeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendCDMAFeatureCodeResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "sendCDMAFeatureCodeResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::sendDtmfResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendDtmfResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->sendDtmfResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendDtmfResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "sendDtmfResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::separateConnectionResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "separateConnectionResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->separateConnectionResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "separateConnectionResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "separateConnectionResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::setCallForwardResponse(unsigned int slotId, ClientId clientId,
                          int responseType, int serial, RIL_Errno e, const void *response,
                          size_t responselen) {
    mtkLogD(LOG_TAG, "setCallForwardResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->setCallForwardResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCallForwardResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setCallForwardResponse: radioVoiceServiceImpl[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::setCallWaitingResponse(unsigned int slotId, ClientId clientId,
                          int responseType, int serial, RIL_Errno e, const void *response,
                          size_t responselen) {
    mtkLogD(LOG_TAG, "setCallWaitingResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->setCallWaitingResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCallWaitingResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setCallWaitingResponse: radioVoiceServiceImpl[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::setClirResponse(unsigned int slotId, ClientId clientId,
                   int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setClirResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->setClirResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setClirResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setClirResponse: radioVoiceServiceImpl[%d]->mRadioResponse == NULL", slotId);
    }

    return 0;
}

int radioVoice::setMuteResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setMuteResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->setMuteResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setMuteResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setMuteResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::setPreferredVoicePrivacyResponse(unsigned int slotId,
        ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setPreferredVoicePrivacyResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->setPreferredVoicePrivacyResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setPreferredVoicePrivacyResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setPreferredVoicePrivacyResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::setTTYModeResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setTTYModeResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->setTtyModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setTTYModeResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setTTYModeResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}
// new added
int radioVoice::setVoNrEnabledResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setVoNrEnabledResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->setVoNrEnabledResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setVoNrEnabledResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setVoNrEnabledResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::startDtmfResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "startDtmfResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->startDtmfResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "startDtmfResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "startDtmfResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::stopDtmfResponse(unsigned int slotId, ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "stopDtmfResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->stopDtmfResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stopDtmfResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stopDtmfResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }
    return 0;
}


int radioVoice::switchWaitingOrHoldingAndActiveResponse(unsigned int slotId,
        ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "switchWaitingOrHoldingAndActiveResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .voiceResponse->switchWaitingOrHoldingAndActiveResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "switchWaitingOrHoldingAndActiveResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "switchWaitingOrHoldingAndActiveResponse:"
                " radioVoiceServiceImpl[%d]->voiceResponse == NULL",
                slotId);
    }

    return 0;
}

int radioVoice::cancelPendingUssdResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);

    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                cancelPendingUssdResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "%s: radioVoiceServiceImpl[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int radioVoice::sendUssdResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);

    if (radioVoiceServiceImpl[slotId].voiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceResponse->
                sendUssdResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "%s: radioVoiceServiceImpl[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}


static void convertRilCdmaSignalInfoRecordToHal(RIL_CDMA_SignalInfoRecord *signalInfoRecord,
        aosp_voice::CdmaSignalInfoRecord& record) {
    record.isPresent = signalInfoRecord->isPresent;
    record.signalType = signalInfoRecord->signalType;
    record.alertPitch = signalInfoRecord->alertPitch;
    record.signal = signalInfoRecord->signal;
}

// indication
int radioVoice::callRingInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "callRingInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        bool isGsm;
        aosp_voice::CdmaSignalInfoRecord record = {};
        if (response == NULL || responseLen == 0) {
            isGsm = true;
        } else {
            isGsm = false;
            if (responseLen != sizeof (RIL_CDMA_SignalInfoRecord)) {
                mtkLogE(LOG_TAG, "callRingInd: invalid response");
                return 0;
            }
            convertRilCdmaSignalInfoRecordToHal((RIL_CDMA_SignalInfoRecord *) response, record);
        }

        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->callRing(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    isGsm,
                    record);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "callRingInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->callRing(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    isGsm,
                    record);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "callRingInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "callRingInd: radioVoiceServiceImpl[%d].voiceIndication == NULL", slotId);
    }
    return 0;
}

int radioVoice::callStateChangedInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "callStateChangedInd, slotid:%d", slotId);
    char isImsCall[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("vendor.ril.isimscall", isImsCall, "0");
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL && (NULL != strstr(isImsCall, "0"))) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->callStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "callStateChangedInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "callStateChangedInd: radioVoiceServiceImpl[%d].voiceIndication == NULL isInImsCall = %s",
                slotId, isImsCall);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "callStateChangedInd, tboxSlotId:%d", tboxSlotId);
    if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->callStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "callStateChangedInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "callStateChangedInd: tbox-radioVoiceServiceImpl[%d].voiceIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioVoice::cdmaCallWaitingInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "cdmaCallWaitingInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_CDMA_CallWaiting_v6)) {
            mtkLogE(LOG_TAG, "cdmaCallWaitingInd: invalid response");
            return 0;
        }

        aosp_voice::CdmaCallWaiting callWaitingRecord = {};
        RIL_CDMA_CallWaiting_v6 *callWaitingRil = ((RIL_CDMA_CallWaiting_v6 *) response);
        callWaitingRecord.number = convertCharPtrToStdString(callWaitingRil->number);
        callWaitingRecord.numberPresentation = callWaitingRil->numberPresentation;
        callWaitingRecord.name = convertCharPtrToStdString(callWaitingRil->name);
        convertRilCdmaSignalInfoRecordToHal(&callWaitingRil->signalInfoRecord,
                callWaitingRecord.signalInfoRecord);
        callWaitingRecord.numberType = callWaitingRil->number_type;
        callWaitingRecord.numberPlan = callWaitingRil->number_plan;

        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->cdmaCallWaiting(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    callWaitingRecord);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cdmaCallWaitingInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->cdmaCallWaiting(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    callWaitingRecord);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cdmaCallWaitingInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaCallWaitingInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::cdmaInfoRecInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "cdmaInfoRecInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_CDMA_InformationRecords)) {
            mtkLogE(LOG_TAG, "cdmaInfoRecInd: invalid response");
            return 0;
        }

        std::vector<aosp_voice::CdmaInformationRecord> records;
        RIL_CDMA_InformationRecords *recordsRil = (RIL_CDMA_InformationRecords *) response;

        char* string8 = NULL;
        int num = MIN(recordsRil->numberOfInfoRecs, RIL_CDMA_MAX_NUMBER_OF_INFO_RECS);
        if (recordsRil->numberOfInfoRecs > RIL_CDMA_MAX_NUMBER_OF_INFO_RECS) {
            mtkLogE(LOG_TAG, "cdmaInfoRecInd: received %d recs which is more than %d, dropping "
                    "additional ones", recordsRil->numberOfInfoRecs,
                    RIL_CDMA_MAX_NUMBER_OF_INFO_RECS);
        }
        records.resize(num);
        for (int i = 0 ; i < num ; i++) {
            aosp_voice::CdmaInformationRecord *record = &records[i];
            RIL_CDMA_InformationRecord *infoRec = &recordsRil->infoRec[i];
            record->name = infoRec->name;
            // All vectors should be size 0 except one which will be size 1. Set everything to
            // size 0 initially.
            record->display.resize(0);
            record->number.resize(0);
            record->signal.resize(0);
            record->redir.resize(0);
            record->lineCtrl.resize(0);
            record->clir.resize(0);
            record->audioCtrl.resize(0);
            switch (infoRec->name) {
                case RIL_CDMA_DISPLAY_INFO_REC:
                case RIL_CDMA_EXTENDED_DISPLAY_INFO_REC: {
                    if (infoRec->rec.display.alpha_len > CDMA_ALPHA_INFO_BUFFER_LENGTH_MTK) {
                        mtkLogE(LOG_TAG, "cdmaInfoRecInd: invalid display info response length %d "
                                "expected not more than %d", (int) infoRec->rec.display.alpha_len,
                                CDMA_ALPHA_INFO_BUFFER_LENGTH_MTK);
                        return 0;
                    }
                    string8 = (char*) malloc((infoRec->rec.display.alpha_len + 1) * sizeof(char));
                    if (string8 == NULL) {
                        mtkLogE(LOG_TAG, "cdmaInfoRecInd: Memory allocation failed for "
                                "responseCdmaInformationRecords");
                        return 0;
                    }
                    memcpy(string8, infoRec->rec.display.alpha_buf, infoRec->rec.display.alpha_len);
                    string8[(int)infoRec->rec.display.alpha_len] = '\0';

                    record->display.resize(1);
                    record->display[0].alphaBuf = string8;
                    free(string8);
                    string8 = NULL;
                    break;
                }

                case RIL_CDMA_CALLED_PARTY_NUMBER_INFO_REC:
                case RIL_CDMA_CALLING_PARTY_NUMBER_INFO_REC:
                case RIL_CDMA_CONNECTED_NUMBER_INFO_REC: {
                    if (infoRec->rec.number.len > CDMA_NUMBER_INFO_BUFFER_LENGTH_MTK) {
                        mtkLogE(LOG_TAG, "cdmaInfoRecInd: invalid display info response length %d "
                                "expected not more than %d", (int) infoRec->rec.number.len,
                                CDMA_NUMBER_INFO_BUFFER_LENGTH_MTK);
                        return 0;
                    }
                    string8 = (char*) malloc((infoRec->rec.number.len + 1) * sizeof(char));
                    if (string8 == NULL) {
                        mtkLogE(LOG_TAG, "cdmaInfoRecInd: Memory allocation failed for "
                                "responseCdmaInformationRecords");
                        return 0;
                    }
                    memcpy(string8, infoRec->rec.number.buf, infoRec->rec.number.len);
                    string8[(int)infoRec->rec.number.len] = '\0';

                    record->number.resize(1);
                    record->number[0].number = string8;
                    free(string8);
                    string8 = NULL;
                    record->number[0].numberType = infoRec->rec.number.number_type;
                    record->number[0].numberPlan = infoRec->rec.number.number_plan;
                    record->number[0].pi = infoRec->rec.number.pi;
                    record->number[0].si = infoRec->rec.number.si;
                    break;
                }

                case RIL_CDMA_SIGNAL_INFO_REC: {
                    record->signal.resize(1);
                    record->signal[0].isPresent = infoRec->rec.signal.isPresent;
                    record->signal[0].signalType = infoRec->rec.signal.signalType;
                    record->signal[0].alertPitch = infoRec->rec.signal.alertPitch;
                    record->signal[0].signal = infoRec->rec.signal.signal;
                    break;
                }

                case RIL_CDMA_REDIRECTING_NUMBER_INFO_REC: {
                    if (infoRec->rec.redir.redirectingNumber.len >
                                                  CDMA_NUMBER_INFO_BUFFER_LENGTH_MTK) {
                        mtkLogE(LOG_TAG, "cdmaInfoRecInd: invalid display info response length %d "
                                "expected not more than %d\n",
                                (int)infoRec->rec.redir.redirectingNumber.len,
                                CDMA_NUMBER_INFO_BUFFER_LENGTH_MTK);
                        return 0;
                    }
                    string8 = (char*) malloc((infoRec->rec.redir.redirectingNumber.len + 1) *
                            sizeof(char));
                    if (string8 == NULL) {
                        mtkLogE(LOG_TAG, "cdmaInfoRecInd: Memory allocation failed for "
                                "responseCdmaInformationRecords");
                        return 0;
                    }
                    memcpy(string8, infoRec->rec.redir.redirectingNumber.buf,
                            infoRec->rec.redir.redirectingNumber.len);
                    string8[(int)infoRec->rec.redir.redirectingNumber.len] = '\0';

                    record->redir.resize(1);
                    record->redir[0].redirectingNumber.number = string8;
                    free(string8);
                    string8 = NULL;
                    record->redir[0].redirectingNumber.numberType =
                            infoRec->rec.redir.redirectingNumber.number_type;
                    record->redir[0].redirectingNumber.numberPlan =
                            infoRec->rec.redir.redirectingNumber.number_plan;
                    record->redir[0].redirectingNumber.pi = infoRec->rec.redir.redirectingNumber.pi;
                    record->redir[0].redirectingNumber.si = infoRec->rec.redir.redirectingNumber.si;
                    record->redir[0].redirectingReason =
                            infoRec->rec.redir.redirectingReason;
                    break;
                }

                case RIL_CDMA_LINE_CONTROL_INFO_REC: {
                    record->lineCtrl.resize(1);
                    record->lineCtrl[0].lineCtrlPolarityIncluded =
                            infoRec->rec.lineCtrl.lineCtrlPolarityIncluded;
                    record->lineCtrl[0].lineCtrlToggle = infoRec->rec.lineCtrl.lineCtrlToggle;
                    record->lineCtrl[0].lineCtrlReverse = infoRec->rec.lineCtrl.lineCtrlReverse;
                    record->lineCtrl[0].lineCtrlPowerDenial =
                            infoRec->rec.lineCtrl.lineCtrlPowerDenial;
                    break;
                }

                case RIL_CDMA_T53_CLIR_INFO_REC: {
                    record->clir.resize(1);
                    record->clir[0].cause = infoRec->rec.clir.cause;
                    break;
                }

                case RIL_CDMA_T53_AUDIO_CONTROL_INFO_REC: {
                    record->audioCtrl.resize(1);
                    record->audioCtrl[0].upLink = infoRec->rec.audioCtrl.upLink;
                    record->audioCtrl[0].downLink = infoRec->rec.audioCtrl.downLink;
                    break;
                }

                case RIL_CDMA_T53_RELEASE_INFO_REC:
                    mtkLogE(LOG_TAG, "cdmaInfoRecInd: RIL_CDMA_T53_RELEASE_INFO_REC: INVALID");
                    return 0;

                default:
                    mtkLogE(LOG_TAG, "cdmaInfoRecInd: Incorrect name value");
                    return 0;
            }
        }

        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->cdmaInfoRec(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    records);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cdmaInfoRecInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->cdmaInfoRec(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    records);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cdmaInfoRecInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaInfoRecInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::cdmaOtaProvisionStatusInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e, const void *response,
                              size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
          mtkLogE(LOG_TAG, "cdmaOtaProvisionStatusInd: invalid response");
          return 0;
        }
        int32_t status = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "cdmaOtaProvisionStatusInd: status %d", status);
        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->cdmaOtaProvisionStatus(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    (aosp_voice::CdmaOtaProvisionStatus) status);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->cdmaOtaProvisionStatus(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    (aosp_voice::CdmaOtaProvisionStatus) status);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaOtaProvisionStatusInd: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }
    return 0;
}

void convertRilEmergencyNumberListToHal(const void *response, size_t responseLen,
        std::vector<aosp_voice::EmergencyNumber>& records) {
    int num = responseLen / sizeof(RIL_EmergencyNumber);
    records.resize(num);

    RIL_EmergencyNumber *rilEccList = (RIL_EmergencyNumber *) response;
    for (int i = 0; i < num; i++) {
        records[i].number = convertCharPtrToStdString((const char*)(rilEccList[i].number));
        records[i].mcc = convertCharPtrToStdString((const char*)(rilEccList[i].mcc));
        records[i].mnc = convertCharPtrToStdString((const char*)(rilEccList[i].mnc));
        // AOSP don't expect MTK extended sub service category, so convert to unspedificed
        records[i].categories = rilEccList[i].categories ==
                EmergencyServiceCategory::EXTENDED_SUB_SERVICE ?
                EmergencyServiceCategory::UNSPECIFIED : rilEccList[i].categories;
        std::vector<std::string> urns;
        records[i].urns = urns;
        records[i].sources = rilEccList[i].sources;
    }
}

int radioVoice::currentEmergencyNumberListInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "currentEmergencyNumberListInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "currentEmergencyNumberListInd: invalid response");
            return 0;
        }
        std::vector<aosp_voice::EmergencyNumber> result;
        convertRilEmergencyNumberListToHal(response, responseLen, result);

        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->currentEmergencyNumberList(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    result);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "currentEmergencyNumberListInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->currentEmergencyNumberList(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    result);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "currentEmergencyNumberListInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "currentEmergencyNumberListInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }
    return 0;
}

int radioVoice::enterEmergencyCallbackModeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "enterEmergencyCallbackModeInd, slotid:%d", slotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->enterEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enterEmergencyCallbackModeInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "enterEmergencyCallbackModeInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "enterEmergencyCallbackModeInd, tboxSlotId:%d", tboxSlotId);
    if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->enterEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enterEmergencyCallbackModeInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "enterEmergencyCallbackModeInd: tbox-radioVoiceServiceImpl[%d].voiceIndication == NULL",
                tboxSlotId);
    }

    // sync enterEmergencyCallbackModeInd to IMS
    unsigned int imsSlotId = slotId + android::CLIENT_IMS * MAX_SIM_COUNT;
    if (radioVoiceServiceImpl[imsSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[imsSlotId].voiceIndication->enterEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enterEmergencyCallbackModeInd: imsSlotId = %d, binder fail!", imsSlotId);
            retChecker->aospAidlReturnStatusChecker(imsSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "enterEmergencyCallbackModeInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                imsSlotId);
    }

    // sync enterEmergencyCallbackModeInd to tbox IMS
    unsigned int imsTboxSlotId = slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;
    if (radioVoiceServiceImpl[imsTboxSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[imsTboxSlotId].voiceIndication->enterEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enterEmergencyCallbackModeInd: imsTboxSlotId = %d, binder fail!", imsTboxSlotId);
            retChecker->aospAidlReturnStatusChecker(imsTboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-enterEmergencyCallbackModeInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                imsTboxSlotId);
    }

    return 0;
}

int radioVoice::exitEmergencyCallbackModeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "exitEmergencyCallbackModeInd, slotid:%d", slotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->exitEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitEmergencyCallbackModeInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "exitEmergencyCallbackModeInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "exitEmergencyCallbackModeInd, tboxSlotId:%d", tboxSlotId);
    if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->exitEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitEmergencyCallbackModeInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-exitEmergencyCallbackModeInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                tboxSlotId);
    }

    // sync exitEmergencyCallbackModeInd to IMS
    unsigned int imsSlotId = slotId + android::CLIENT_IMS * MAX_SIM_COUNT;
    if (radioVoiceServiceImpl[imsSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[imsSlotId].voiceIndication->exitEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitEmergencyCallbackModeInd: imsSlotId = %d, binder fail!", imsSlotId);
            retChecker->aospAidlReturnStatusChecker(imsSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "exitEmergencyCallbackModeInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                imsSlotId);
    }

    // sync exitEmergencyCallbackModeInd to tbox IMS
    unsigned int imsTboxSlotId = slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;
    if (radioVoiceServiceImpl[imsTboxSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[imsTboxSlotId].voiceIndication->exitEmergencyCallbackMode(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitEmergencyCallbackModeInd: imsTboxSlotId = %d, binder fail!", imsTboxSlotId);
            retChecker->aospAidlReturnStatusChecker(imsTboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-exitEmergencyCallbackModeInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                imsTboxSlotId);
    }
    return 0;
}

int radioVoice::indicateRingbackToneInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "indicateRingbackToneInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "indicateRingbackToneInd: invalid response");
            return 0;
        }
        bool start = ((int32_t *) response)[0];

        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->indicateRingbackTone(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    start);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "indicateRingbackToneInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->indicateRingbackTone(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    start);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "indicateRingbackToneInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "indicateRingbackToneInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }

    return 0;
}

bool isCfQueryServiceType(RIL_SsServiceType serType, RIL_SsRequestType reqType) {
    if ((reqType == SS_INTERROGATION) &&
        (serType == SS_CFU ||
         serType == SS_CF_BUSY ||
         serType == SS_CF_NO_REPLY ||
         serType == SS_CF_NOT_REACHABLE ||
         serType == SS_CF_ALL ||
         serType == SS_CF_ALL_CONDITIONAL)) {
        return true;
    }

    return false;
}

int radioVoice::onUssdInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != 2 * sizeof(char *)) {
            mtkLogE(LOG_TAG, "onUssdInd: invalid response");
            return 0;
        }

        char **strings = (char **) response;
        char *mode = strings[0];

        mtkLogD(LOG_TAG, "onUssdInd: mode %s", mode);

        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->onUssd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    (aosp_voice::UssdModeType) atoi(mode),
                    convertCharPtrToStdString(strings[1]));
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->onUssd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    (aosp_voice::UssdModeType) atoi(mode),
                    convertCharPtrToStdString(strings[1]));
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "%s: radioVoiceServiceImpl[%d]->mRadioIndication == NULL",
                __FUNCTION__, slotId);
    }

    return 0;
}

int radioVoice::onSupplementaryServiceIndicationInd(unsigned int slotId,
                                        int indicationType, int token, RIL_Errno e,
                                        const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_StkCcUnsolSsResponse)) {
            mtkLogE(LOG_TAG, "onSupplementaryServiceIndicationInd: invalid response");
            return 0;
        }

        RIL_StkCcUnsolSsResponse *rilSsResponse = (RIL_StkCcUnsolSsResponse *) response;
        aosp_voice::StkCcUnsolSsResult ss = {};
        ss.serviceType = (int) rilSsResponse->serviceType;
        ss.requestType = (int) rilSsResponse->requestType;
        ss.teleserviceType = (int) rilSsResponse->teleserviceType;
        ss.serviceClass = rilSsResponse->serviceClass;
        ss.result = (aidl_radio::RadioError) rilSsResponse->result;

        if (isCfQueryServiceType(rilSsResponse->serviceType, rilSsResponse->requestType)) {
            mtkLogD(LOG_TAG, "onSupplementaryServiceIndicationInd CF type, num of Cf elements %d",
                    rilSsResponse->cfData.numValidIndexes);
            if (rilSsResponse->cfData.numValidIndexes > aosp_voice::CfData::NUM_SERVICE_CLASSES) {
                mtkLogE(LOG_TAG, "onSupplementaryServiceIndicationInd numValidIndexes is greater "
                        "than max value %d, truncating it to max value",
                        aosp_voice::CfData::NUM_SERVICE_CLASSES);
                rilSsResponse->cfData.numValidIndexes = aosp_voice::CfData::NUM_SERVICE_CLASSES;
            }

            ss.cfData.resize(1);
            ss.ssInfo.resize(0);

            /* number of call info's */
            ss.cfData[0].cfInfo.resize(rilSsResponse->cfData.numValidIndexes);

            for (int i = 0; i < rilSsResponse->cfData.numValidIndexes; i++) {
                 RIL_CallForwardInfo cf = rilSsResponse->cfData.cfInfo[i];
                 aosp_voice::CallForwardInfo *cfInfo = &ss.cfData[0].cfInfo[i];

                 cfInfo->status = cf.status;
                 cfInfo->reason = cf.reason;
                 cfInfo->serviceClass = cf.serviceClass;
                 cfInfo->toa = cf.toa;
                 cfInfo->number = convertCharPtrToStdString(cf.number);
                 cfInfo->timeSeconds = cf.timeSeconds;
                 mtkLogD(LOG_TAG, "onSupplementaryServiceIndicationInd: "
                        "Data: %d,reason=%d,cls=%d,toa=%d,num=%s,tout=%d],", cf.status,
                        cf.reason, cf.serviceClass, cf.toa, (char*)cf.number, cf.timeSeconds);
            }
        } else {
            ss.ssInfo.resize(1);
            ss.cfData.resize(0);

            /* each int */
            ss.ssInfo[0].ssInfo.resize(aosp_voice::SsInfoData::SS_INFO_MAX);
            for (int i = 0; i < aosp_voice::SsInfoData::SS_INFO_MAX; i++) {
                 mtkLogD(LOG_TAG,
                        "onSupplementaryServiceIndicationInd: Data: %d", rilSsResponse->ssInfo[i]);
                 ss.ssInfo[0].ssInfo[i] = rilSsResponse->ssInfo[i];
            }
        }

        mtkLogD(LOG_TAG, "onSupplementaryServiceIndicationInd, slotid:%d", slotId);

        if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->onSupplementaryServiceIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), ss);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->onSupplementaryServiceIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), ss);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "%s: radioVoiceServiceImpl[%d].voiceIndication == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int radioVoice::resendIncallMuteInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "resendIncallMuteInd, slotid:%d", slotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->resendIncallMute(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "resendIncallMuteInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "resendIncallMuteInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "resendIncallMuteInd, tboxSlotId:%d", tboxSlotId);
    if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->resendIncallMute(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "resendIncallMuteInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-resendIncallMuteInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioVoice::srvccStateNotifyInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen) {
    mtkLogD(LOG_TAG, "srvccStateNotifyInd, slotid:%d", slotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: invalid response");
            return 0;
        }
        aosp_voice::SrvccState state =
                aosp_voice::SrvccState(((int32_t *) response)[0]);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->srvccStateNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "srvccStateNotifyInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "srvccStateNotifyInd, tboxSlotId:%d", tboxSlotId);
    if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: invalid response");
            return 0;
        }
        aosp_voice::SrvccState state =
                aosp_voice::SrvccState(((int32_t *) response)[0]);
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->srvccStateNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "srvccStateNotifyInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                tboxSlotId);
    }

    // sync srvccStateNotifyInd to IMS
    unsigned int imsSlotId = slotId + android::CLIENT_IMS * MAX_SIM_COUNT;
    if (radioVoiceServiceImpl[imsSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: invalid response");
            return 0;
        }
        aosp_voice::SrvccState state =
                aosp_voice::SrvccState(((int32_t *) response)[0]);
        ScopedAStatus ret = radioVoiceServiceImpl[imsSlotId].voiceIndication->srvccStateNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: slotId = %d, binder fail!", imsSlotId);
            retChecker->aospAidlReturnStatusChecker(imsSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "srvccStateNotifyInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                imsSlotId);
    }

    // sync srvccStateNotifyInd to tbox IMS
    unsigned int imsTboxSlotId = slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;
    if (radioVoiceServiceImpl[imsTboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: invalid response");
            return 0;
        }
        aosp_voice::SrvccState state =
                aosp_voice::SrvccState(((int32_t *) response)[0]);
        ScopedAStatus ret = radioVoiceServiceImpl[imsTboxSlotId].voiceIndication->srvccStateNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "srvccStateNotifyInd: imsTboxSlotId = %d, binder fail!", imsTboxSlotId);
            retChecker->aospAidlReturnStatusChecker(imsTboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-srvccStateNotifyInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                imsTboxSlotId);
    }
    return 0;
}

int radioVoice::stkCallControlAlphaNotifyInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e, const void *response,
                                 size_t responselen) {
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
        if (response == NULL || responselen ==0) {
            mtkLogE(LOG_TAG, "stkCallControlAlphaNotifyInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "stkCallControlAlphaNotifyInd, slotid:%d", slotId);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->stkCallControlAlphaNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkCallControlAlphaNotifyInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkCallSetupInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responselen ==0) {
            mtkLogE(LOG_TAG, "tbox-stkCallControlAlphaNotifyInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "stkCallControlAlphaNotifyInd, tboxSlotId:%d", tboxSlotId);
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->stkCallControlAlphaNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkCallControlAlphaNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-stkCallSetupInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioVoice::stkCallSetupInd(unsigned int slotId, int indicationType,
                    int token, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "stkCallSetupInd, slotid:%d", slotId);
    if (radioVoiceServiceImpl[slotId].voiceIndication != NULL) {
        if (response == NULL || responselen != sizeof(int)) {
            mtkLogE(LOG_TAG, "stkCallSetupInd: invalid response");
            return 0;
        }
        int32_t timeout = ((int32_t *) response)[0];

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].voiceIndication->stkCallSetup(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                timeout);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkCallSetupInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkCallSetupInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "stkCallSetupInd, tboxSlotId:%d", tboxSlotId);
    if (radioVoiceServiceImpl[tboxSlotId].voiceIndication != NULL) {
        if (response == NULL || responselen != sizeof(int)) {
            mtkLogE(LOG_TAG, "tbox-stkCallSetupInd: invalid response");
            return 0;
        }
        int32_t timeout = ((int32_t *) response)[0];

        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].voiceIndication->stkCallSetup(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                timeout);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkCallSetupInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-stkCallSetupInd: radioVoiceServiceImpl[%d].voiceIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioVoice::clearVoiceResponseAndIndications(unsigned int slotId) {
    radioVoiceServiceImpl[slotId].voiceResponse = NULL;
    radioVoiceServiceImpl[slotId].voiceIndication = NULL;
    return 0;
}

// mtk radio
int mtkRadioExVoice::hangupAllResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "hangupAllResponse: serial = %d, slotId = %d", serial, slotId);
    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId]
                    .mtkImsVoiceResponse->hangupAllResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "hangupAllResponse: slotId = %d, binder fail!",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "hangupAllResponse:"
                    " radioVoiceServiceImpl[%d]->mtkImsVoiceResponse == NULL",
                    slotId);
        }
    } else {
        if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                    .mtkVoiceResponse->hangupAllResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "hangupAllResponse: slotId = %d, binder fail!",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "hangupAllResponse:"
                    " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                    slotId);
        }
    }
    return 0;
}

int mtkRadioExVoice::hangupWithReasonResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "hangupWithReasonResponse: serial = %d, slotId = %d", serial, slotId);
    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId]
                    .mtkImsVoiceResponse->hangupWithReasonResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "hangupWithReasonResponse: slotId = %d, binder fail!",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "hangupWithReasonResponse:"
                    " radioVoiceServiceImpl[%d]->mtkImsVoiceResponse == NULL",
                    slotId);
        }
    } else {
        if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                    .mtkVoiceResponse->hangupWithReasonResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "hangupWithReasonResponse: slotId = %d, binder fail!",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "hangupWithReasonResponse:"
                    " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                    slotId);
        }
    }
    return 0;
}

int mtkRadioExVoice::getCallSubAddressResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responseLen) {
    mtkLogD(LOG_TAG, "getCallSubAddressResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        int enabled = 0;
        int *resp = (int *) response;
        int numInts = responseLen / sizeof(int);
        if (numInts < 1) {
            mtkLogE(LOG_TAG, "getCallSubAddressResponse: items length invalid, slotId = %d",
                    slotId);
        } else {
            enabled = ((int32_t *) resp)[0];
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->getCallSubAddressResponse(responseInfo, enabled);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCallSubAddressResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getCallSubAddressResponse:"
                " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExVoice::getColpResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responseLen) {
    mtkLogD(LOG_TAG, "getColpResponse: serial %d, slotId = %u", serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    populateAidlResponseInfo(responseInfo, serial, responseType, e);

    int n = -1, m = -1;
    int numInts = responseLen / sizeof(int);
    if (response == NULL || numInts != 2) {
        mtkLogE(LOG_TAG, "getColpResponse Invalid response: NULL");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        int *pInt = (int *) response;
        n = pInt[0];
        m = pInt[1];
    }

    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse->getColpResponse(
                    responseInfo, n, m);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getColpResponse: slotId = %u, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "getColpResponse: radioVoiceServiceImpl[%u]->mtkImsVoiceResponse == NULL",
                    slotId);
        }

        return 0;
    }

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->getColpResponse(
                responseInfo, n, m);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getColpResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getColpResponse: radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExVoice::getColrResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responseLen) {
    mtkLogD(LOG_TAG, "getColrResponse: serial %d, slotId = %u", serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    populateAidlResponseInfo(responseInfo, serial, responseType, e);

    int val = -1;
    if (response == NULL || responseLen != sizeof(int)) {
        mtkLogE(LOG_TAG, "getColrResponse: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        int *p_int = (int *) response;
        val = p_int[0];
    }

    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse->getColrResponse(
                    responseInfo, val);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getColrResponse: slotId = %u, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "getColrResponse: radioVoiceServiceImpl[%u]->mtkImsVoiceResponse == NULL",
                    slotId);
        }

        return 0;
    }

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->getColrResponse(
                responseInfo, val);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getColrResponse: slotId = %u, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getColrResponse: radioVoiceServiceImpl[%u]->mtkVoiceResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExVoice::getEccNumResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "getEccNumResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->getEccNumResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getEccNumResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getEccNumResponse:"
                " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExVoice::queryCallForwardInTimeSlotStatusResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responseLen) {
    mtkLogD(LOG_TAG, "queryCallForwardInTimeSlotStatusResponse: serial %d, slotId = %u",
            serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    populateAidlResponseInfo(responseInfo, serial, responseType, e);

    std::vector<mtk_voice::CallForwardInfoEx> callForwardInfoExs;

    if (response == NULL || responseLen % sizeof(RIL_CallForwardInfoEx *) != 0) {
        mtkLogE(LOG_TAG, "queryCallForwardInTimeSlotStatusResponse Invalid response: NULL");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        int num = responseLen / sizeof(RIL_CallForwardInfoEx *);
        callForwardInfoExs.resize(num);
        for (int i = 0 ; i < num; i++) {
            RIL_CallForwardInfoEx *resp = ((RIL_CallForwardInfoEx **) response)[i];
            callForwardInfoExs[i].status = resp->status;
            callForwardInfoExs[i].reason = resp->reason;
            callForwardInfoExs[i].serviceClass = resp->serviceClass;
            callForwardInfoExs[i].toa = resp->toa;
            callForwardInfoExs[i].number = convertCharPtrToStdString(resp->number);
            callForwardInfoExs[i].timeSeconds = resp->timeSeconds;
            callForwardInfoExs[i].timeSlotBegin = convertCharPtrToStdString(resp->timeSlotBegin);
            callForwardInfoExs[i].timeSlotEnd = convertCharPtrToStdString(resp->timeSlotEnd);
        }
    }

    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId]
                    .mtkImsVoiceResponse->queryCallForwardInTimeSlotStatusResponse(
                            responseInfo, callForwardInfoExs);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %u, binder fail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "%s: radioVoiceServiceImpl[%u]->mtkImsVoiceResponse == NULL",
                    __FUNCTION__, slotId);
        }

        return 0;
    }

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->queryCallForwardInTimeSlotStatusResponse(
                        responseInfo, callForwardInfoExs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "%s: radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int mtkRadioExVoice::resetSuppServResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "resetSuppServResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->resetSuppServResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "resetSuppServResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "resetSuppServResponse: radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExVoice::sendCnapResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responseLen) {
    mtkLogD(LOG_TAG, "sendCnapResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        int n = -1, m = -1;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || numInts != 2) {
            mtkLogE(LOG_TAG, "sendCnapResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            n = pInt[0];
            m = pInt[1];
        }

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->sendCnapResponse(
                responseInfo, n, m);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendCnapResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "sendCnapResponse: radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExVoice::setCallForwardInTimeSlotResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setCallForwardInTimeSlotResponse: serial %d, slotId = %u", serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    populateAidlResponseInfo(responseInfo, serial, responseType, e);

    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId]
                    .mtkImsVoiceResponse->setCallForwardInTimeSlotResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %u, binder fail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "%s: radioVoiceServiceImpl[%u]->mtkImsVoiceResponse == NULL",
                    __FUNCTION__, slotId);
        }

        return 0;
    }

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setCallForwardInTimeSlotResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %u, binder fail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "%s: radioVoiceServiceImpl[%u]->mtkVoiceResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}


int mtkRadioExVoice::setCallIndicationResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setCallIndicationResponse: serial %d slotId %d  clientId %d ", serial, slotId, clientId);
    if (clientId == android::CLIENT_IMS || clientId == android::CLIENT_TBOX) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId]
                    .mtkImsVoiceResponse->setCallIndicationResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "setCallIndicationResponse: slotId = %d, binder fail!",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else if (radioVoiceServiceImpl[slotId].mtkImsVoiceResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                    .mtkImsVoiceResponse->setCallIndicationResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "setCallIndicationResponse: slotId = %d, binder fail!",
                        slotId);
                //retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "setCallIndicationResponse:"
                    " radioVoiceServiceImpl[%d]->mtkImsVoiceResponse == NULL",
                    slotId);
        }
    } else {
        mtkLogE(LOG_TAG, "clientId !== android::CLIENT_IMS");
        if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            mtkLogE(LOG_TAG, "radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL");
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            mtkLogE(LOG_TAG, "populateAidlResponseInfo");
            ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                    .mtkVoiceResponse->setCallIndicationResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "setCallIndicationResponse: slotId = %d, binder fail!",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "setCallIndicationResponse:"
                    " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                    slotId);
        }
    }
    return 0;
}

int mtkRadioExVoice::setCallSubAddressResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setCallSubAddressResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setCallSubAddressResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCallSubAddressResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setCallSubAddressResponse:"
                " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExVoice::setCallValidTimerResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setCallValidTimerResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setCallValidTimerResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCallValidTimerResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setCallValidTimerResponse: radioVoiceServiceImpl[%d].mtkVoiceResponse == NULL",
                                                                                      slotId);
    }

    return 0;
}



int mtkRadioExVoice::setClipResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setClipResponse: serial %d, slotId = %u", serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    populateAidlResponseInfo(responseInfo, serial, responseType, e);

    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse->setClipResponse(
                    responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "setClipResponse: slotId = %u, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "setClipResponse: radioVoiceServiceImpl[%u]->mtkImsVoiceResponse == NULL",
                    slotId);
        }

        return 0;
    }

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->setClipResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setClipResponse: slotId = %u, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setClipResponse: radioVoiceServiceImpl[%u]->mtkVoiceResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExVoice::setColpResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setColpResponse: serial %d, slotId = %u", serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    populateAidlResponseInfo(responseInfo, serial, responseType, e);

    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse->setColpResponse(
                    responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "setColpResponse: slotId = %u, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "setColpResponse: radioVoiceServiceImpl[%u]->mtkImsVoiceResponse == NULL",
                    slotId);
        }

        return 0;
    }

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->setColpResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setColpResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setColpResponse: radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExVoice::setColrResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setColrResponse: serial %d, slotId = %u", serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    populateAidlResponseInfo(responseInfo, serial, responseType, e);

    if (clientId == android::CLIENT_IMS) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse->setColrResponse(
                    responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "setColrResponse: slotId = %u, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG,
                    "setColrResponse: radioVoiceServiceImpl[%u]->mtkImsVoiceResponse == NULL",
                    slotId);
        }

        return 0;
    }

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->setColrResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setColrResponse: slotId = %u, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setColrResponse: radioVoiceServiceImpl[%u]->mtkVoiceResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExVoice::setEccModeResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setEccModeResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setEccModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setEccModeResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setEccModeResponse:"
                " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExVoice::setEccNumResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setEccNumResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setEccNumResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setEccNumResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setEccNumResponse:"
                " radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExVoice::setGwsdModeResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setGwsdModeResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setGwsdModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setGwsdModeResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setGwsdModeResponse: radioVoiceServiceImpl[%d].mtkVoiceResponse == NULL",
                                                                                      slotId);
    }

    return 0;
}

int mtkRadioExVoice::setIgnoreSameNumberIntervalResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setIgnoreSameNumberIntervalResponse: serial %d", serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setIgnoreSameNumberIntervalResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setIgnoreSameNumberIntervalResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setIgnoreSameNumberIntervalResponse: radioVoiceServiceImpl[%d].mtkVoiceResponse == NULL",
                                                                                      slotId);
    }

    return 0;
}
int mtkRadioExVoice::setKeepAliveByIpDataResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __func__, serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setKeepAliveByIpDataResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setKeepAliveByIpDataResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setKeepAliveByIpDataResponse: radioVoiceServiceImpl[%d].mtkVoiceResponse == NULL",
                                                                                      slotId);
    }

    return 0;
}


int mtkRadioExVoice::setKeepAliveByPDCPCtrlPDUResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __func__, serial);
    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId]
                .mtkVoiceResponse->setKeepAliveByPDCPCtrlPDUResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setKeepAliveByIpDataResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setKeepAliveByPDCPCtrlPDUResponse: radioVoiceServiceImpl[%d].mtkVoiceResponse == NULL",
                                                                                      slotId);
    }

    return 0;
}

int mtkRadioExVoice::setSuppServPropertyResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen) {
    mtkLogD(LOG_TAG, "setSuppServPropertyResponse: serial %d", serial);

    if (radioVoiceServiceImpl[slotId].mtkVoiceResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceResponse->setSuppServPropertyResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSuppServPropertyResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setSuppServPropertyResponse: radioVoiceServiceImpl[%d]->mtkVoiceResponse == NULL",
                slotId);
    }

    return 0;
}


int mtkRadioExVoice::callAdditionalInfoInd(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (response == NULL) {
        mtkLogE(LOG_TAG, "callAdditionalInfoInd invalid response");
        return 0;
    }

    char **resp = (char **) response;
    int mode = atoi(resp[0]);
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    // mode >= 100, IMS Call's Additional Info Indication, otherwise CS's
    if (mode >= 100) {
        if (radioVoiceServiceImpl[slotId].mtkImsVoiceIndication != NULL ||
                radioVoiceServiceImpl[tboxSlotId].mtkImsVoiceIndication != NULL) {
            std::vector<std::string> data;
            int numStrings = responseLen / sizeof(char *) - 1;
            data.resize(numStrings);
            mtk_voice::CallInfoType ciType =
                    mtk_voice::CallInfoType(atoi(resp[0]));
            for (int i = 0; i < numStrings; i++) {
                data[i] = convertCharPtrToStdString((resp[i + 1]));
            }

            if (radioVoiceServiceImpl[slotId].mtkImsVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkImsVoiceIndication->callAdditionalInfoInd(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        ciType,
                        data);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "callAdditionalInfoInd: slotId = %d, binder fail!", slotId);
                    unsigned int imsSlotId = slotId + android::CLIENT_IMS * MAX_SIM_COUNT;
                    retChecker->mtkAidlReturnStatusChecker(imsSlotId, ret, android::IMS_MTK);
                }
            }

            if (radioVoiceServiceImpl[tboxSlotId].mtkImsVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkImsVoiceIndication->callAdditionalInfoInd(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        ciType,
                        data);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "callAdditionalInfoInd: slotId = %d, binder fail!", slotId);
                    unsigned int imsSlotId = slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;
                    retChecker->mtkAidlReturnStatusChecker(imsSlotId, ret, android::TBOX_IMS_MTK);
                }
            }
        } else {
            mtkLogE(LOG_TAG,
                    "callAdditionalInfoInd: radioVoiceServiceImpl[%d].mtkImsVoiceIndication == NULL",
                    slotId);
        }
    } else {
        if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
            std::vector<std::string> data;
            char **resp = (char **) response;
            int numStrings = responseLen / sizeof(char *) - 1;
            data.resize(numStrings);
            mtk_voice::CallInfoType ciType =
                    mtk_voice::CallInfoType(atoi(resp[0]));
            for (int i = 0; i < numStrings; i++) {
                data[i] = convertCharPtrToStdString((resp[i + 1]));
            }


            if (radioVoiceServiceImpl[slotId].mtkImsVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->callAdditionalInfoInd(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        ciType,
                        data);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "callAdditionalInfoInd: slotId = %d, binder fail!", slotId);
                    retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
                }
            }

            if (radioVoiceServiceImpl[tboxSlotId].mtkImsVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->callAdditionalInfoInd(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        ciType,
                        data);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "callAdditionalInfoInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                    retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::RADIO_MTK);
                }
            }
        } else {
            mtkLogE(LOG_TAG,
                    "callAdditionalInfoInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                    slotId);
        }
    }

    return 0;
}

int mtkRadioExVoice::cdmaCallAcceptedInd(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responselen) {
    if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
        mtkLogD(LOG_TAG, "cdmaCallAcceptedInd: slotId=%d", slotId);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->cdmaCallAccepted(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cdmaCallAcceptedInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "cdmaCallAcceptedInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
        mtkLogD(LOG_TAG, "cdmaCallAcceptedInd: tboxSlotId=%d", tboxSlotId);
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->cdmaCallAccepted(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cdmaCallAcceptedInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-cdmaCallAcceptedInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                tboxSlotId);
    }

    return 0;
}

int mtkRadioExVoice::cfuStatusNotifyInd(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
        if (response == NULL || responseLen != 2 * sizeof(int)) {
            mtkLogE(LOG_TAG, "cfuStatusNotifyInd: invalid response");
            return 0;
        }

        mtk_voice::CfuStatusNotification cfuStatus = {};
        int *csn = (int *) response;
        cfuStatus.status = csn[0];
        cfuStatus.lineId = csn[1];

        mtkLogD(LOG_TAG, "cfuStatusNotifyInd: status = %d, line = %d", cfuStatus.status, cfuStatus.lineId);
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->cfuStatusNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                cfuStatus);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cfuStatusNotifyInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "cfuStatusNotifyInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
        if (response == NULL || responseLen != 2 * sizeof(int)) {
            mtkLogE(LOG_TAG, "cfuStatusNotifyInd: invalid response");
            return 0;
        }

        mtk_voice::CfuStatusNotification cfuStatus = {};
        int *csn = (int *) response;
        cfuStatus.status = csn[0];
        cfuStatus.lineId = csn[1];

        mtkLogD(LOG_TAG, "cfuStatusNotifyInd: status = %d, line = %d", cfuStatus.status, cfuStatus.lineId);
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->cfuStatusNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                cfuStatus);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cfuStatusNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "cfuStatusNotifyInd: tbox-radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExVoice::cipherIndicationInd(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
        mtkLogD(LOG_TAG, "cipherIndicationInd: slotId=%d", slotId);
        if (response == NULL || responseLen != 4 * sizeof(char *)) {
            mtkLogE(LOG_TAG, "cipherInd: invalid response");
            return 0;
        }
        mtk_voice::CipherNotification cipherNotify;
        char **strings = (char **) response;
        // +ECIPH:  <sim_cipher_ind>,<mm_connection>,<cs_cipher_on>,<ps_cipher_on>
        cipherNotify.simCipherStatus = convertCharPtrToStdString(strings[0]);
        cipherNotify.sessionStatus = convertCharPtrToStdString(strings[1]);
        cipherNotify.csStatus = convertCharPtrToStdString(strings[2]);
        cipherNotify.psStatus = convertCharPtrToStdString(strings[3]);
        mtkLogD(LOG_TAG, "cipherInd: %s, %s, %s, %s", strings[0], strings[1], strings[2], strings[3]);

        if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->cipherIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    cipherNotify);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cipherIndicationInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->cipherIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    cipherNotify);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cipherIndicationInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "cipherIndicationInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExVoice::confSRVCCInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "confSRVCCInd: invalid response");
            return 0;
        }

        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        data.resize(numInts);
        int *pInt = (int *) response;

        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }

        if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->confSRVCC(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "confSRVCCInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->confSRVCC(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "confSRVCCInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "confSRVCCInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExVoice::crssNotifyInd(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL ||
            radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_CrssNotification)) {
            mtkLogE(LOG_TAG, "crssNotifyInd: invalid response");
            return 0;
        }
        mtk_voice::CrssNotification crssNotify;
        RIL_CrssNotification *crss = (RIL_CrssNotification *) response;
        crssNotify.code = crss->code;
        crssNotify.type = crss->type;
        crssNotify.number = convertCharPtrToStdString(crss->number);
        crssNotify.alphaid = convertCharPtrToStdString(crss->alphaid);
        crssNotify.cli_validity = crss->cli_validity;

        mtkLogD(LOG_TAG, "crssNotifyInd: code %d type %d cli_validity %d",
                crssNotify.code, crssNotify.type, crssNotify.cli_validity);

        if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->crssIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    crssNotify);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "crssNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->crssIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    crssNotify);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "crssNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "crssNotifyInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExVoice::eccNumIndication(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responselen) {
    if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "[%s] invalid response", __FUNCTION__);
            return 0;
        }
        char *resp = (char *) response;
        ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->eccNumIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString(resp),
                convertCharPtrToStdString(resp));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "eccNumIndication: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "eccNumIndication: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "[%s] tbox-invalid response", __FUNCTION__);
            return 0;
        }
        char *resp = (char *) response;
        ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->eccNumIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString(resp),
                convertCharPtrToStdString(resp));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "eccNumIndication: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-eccNumIndication: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExVoice::incomingCallIndicationInd(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (response == NULL) {
        mtkLogE(LOG_TAG, "incomingCallIndicationInd: response is NULL");
        return 0;
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    char **resp = (char **) response;
    int numStrings = responseLen / sizeof(char *);
    if (numStrings < 7) {
        mtkLogE(LOG_TAG, "incomingCallIndicationInd: items length is invalid, slot = %d", slotId);
        return 0;
    }
mtkLogE(LOG_TAG, "incomingCallIndicationInd  tboxSlotId = %d radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication = %d", tboxSlotId, radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication == NULL);
    int mode = atoi(resp[3]);
    if (mode < 20) {
        mtk_property_set("vendor.ril.isimscall","0");
        if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL ||
                radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
            mtk_voice::IncomingCallNotification inCallNotify;
            char **strings = (char **) response;
            // EAIC: <callId>, <number>, <type>, <call mode>, <seq no>
            inCallNotify.callId = convertCharPtrToStdString(strings[0]);
            inCallNotify.number = convertCharPtrToStdString(strings[1]);
            inCallNotify.type = convertCharPtrToStdString(strings[2]);
            inCallNotify.callMode = convertCharPtrToStdString(strings[3]);
            inCallNotify.seqNo = convertCharPtrToStdString(strings[4]);
            inCallNotify.redirectNumber = convertCharPtrToStdString(strings[5]);

            if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->incomingCallIndication(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        inCallNotify);

                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "incomingCallIndicationInd: slotId = %d, binder fail!", slotId);
                    retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
                }
            }

            if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->incomingCallIndication(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        inCallNotify);

                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "incomingCallIndicationInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                    retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
                }
            }
        } else {
            mtkLogE(LOG_TAG,
                    "incomingCallIndicationInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                    slotId);
        }
    } else {
        mtk_property_set("vendor.ril.isimscall","1");
        if (radioVoiceServiceImpl[slotId].mtkImsVoiceIndication != NULL ||
                radioVoiceServiceImpl[tboxSlotId].mtkImsVoiceIndication != NULL) {
            
            mtk_voice::IncomingCallNotification inCallNotify;
            // EAIC: <callId>, <number>, <type>, <call mode>, <seq no>,
            // [<redirect_num>],[<digit_to_line_num>]
            inCallNotify.callId = convertCharPtrToStdString(resp[0]);
            inCallNotify.number = convertCharPtrToStdString(resp[1]);
            inCallNotify.type = convertCharPtrToStdString(resp[2]);
            inCallNotify.callMode = convertCharPtrToStdString(resp[3]);
            inCallNotify.seqNo = convertCharPtrToStdString(resp[4]);
            inCallNotify.redirectNumber = convertCharPtrToStdString(resp[5]);
            inCallNotify.toNumber = convertCharPtrToStdString(resp[6]);

            if (radioVoiceServiceImpl[slotId].mtkImsVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkImsVoiceIndication->incomingCallIndication(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        inCallNotify);

                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "incomingCallIndicationInd: slotId = %d, binder fail!", slotId);
                    unsigned int imsSlotId = slotId + android::CLIENT_IMS * MAX_SIM_COUNT;
                    retChecker->mtkAidlReturnStatusChecker(imsSlotId, ret, android::IMS_MTK);
                }
            }

            if (radioVoiceServiceImpl[tboxSlotId].mtkImsVoiceIndication != NULL) {
                ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkImsVoiceIndication->incomingCallIndication(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        inCallNotify);

                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "incomingCallIndicationInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                    unsigned int imsSlotId = slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;
                    retChecker->mtkAidlReturnStatusChecker(imsSlotId, ret, android::TBOX_IMS_MTK);
                }
            }
        } else {
            mtkLogE(LOG_TAG,
                    "incomingCallIndicationInd: radioVoiceServiceImpl[%d].mtkImsVoiceIndication == NULL",
                    slotId);
        }
    }

    return 0;
}

int mtkRadioExVoice::suppSvcNotifyExInd(unsigned int slotId, int indicationType,
                int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL ||
            radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_SuppSvcNotification)) {
            mtkLogE(LOG_TAG, "suppSvcNotifyExInd: invalid response");
            return 0;
        }

        aidl_radio_network::SuppSvcNotification suppSvc;
        RIL_SuppSvcNotification *ssn = (RIL_SuppSvcNotification *) response;
        suppSvc.isMT = ssn->notificationType;
        suppSvc.code = ssn->code;
        suppSvc.index = ssn->index;
        suppSvc.type = ssn->type;
        suppSvc.number = convertCharPtrToStdString(ssn->number);

        mtkLogD(LOG_TAG, "suppSvcNotifyExInd: isMT %d code %d index %d type %d",
                suppSvc.isMT, suppSvc.code, suppSvc.index, suppSvc.type);

        if (radioVoiceServiceImpl[slotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[slotId].mtkVoiceIndication->suppSvcNotifyEx(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    suppSvc);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "suppSvcNotifyExInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication != NULL) {
            ScopedAStatus ret = radioVoiceServiceImpl[tboxSlotId].mtkVoiceIndication->suppSvcNotifyEx(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    suppSvc);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "suppSvcNotifyExInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "suppSvcNotifyExInd: radioVoiceServiceImpl[%d].mtkVoiceIndication == NULL",
                slotId);
    }

    return 0;
}



namespace android::hardware::radio::voice {

namespace aidl = ::aidl::android::hardware::radio::voice;
constexpr auto ok = &ScopedAStatus::ok;

RadioVoice::RadioVoice(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus RadioVoice::acceptCall(int32_t serial) {
    mtkLogD(LOG_TAG, "acceptCall: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_ANSWER);
    return ok();
}

ScopedAStatus RadioVoice::conference(int32_t serial) {
    mtkLogD(LOG_TAG, "conference: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_CONFERENCE);
    return ok();
}

ScopedAStatus RadioVoice::dial(int32_t serial, const aidl::Dial& dialInfo) {
    mtkLogD(LOG_TAG, "dial: serial %d", serial);

    int requestId = RIL_REQUEST_DIAL;
    if ((getClientBySlot(mSlotId) == android::CLIENT_IMS) || (getClientBySlot(mSlotId) == android::CLIENT_TBOX)) {
        requestId = RIL_REQUEST_IMS_DIAL;
    }
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, requestId);
    if (pRI == NULL) {
        return ok();
    }
    RIL_Dial dial = {};
    RIL_UUS_Info uusInfo = {};
    int32_t sizeOfDial = sizeof(dial);

    if (!copyAidlStringToRil(&dial.address, dialInfo.address, pRI)) {
        return ok();
    }
    dial.clir = (int) dialInfo.clir;

    if (dialInfo.uusInfo.size() != 0) {
        uusInfo.uusType = (RIL_UUS_Type) dialInfo.uusInfo[0].uusType;
        uusInfo.uusDcs = (RIL_UUS_DCS) dialInfo.uusInfo[0].uusDcs;

        if (dialInfo.uusInfo[0].uusData.size() == 0) {
            uusInfo.uusData = NULL;
            uusInfo.uusLength = 0;
        } else {
            if (!copyAidlStringToRil(&uusInfo.uusData, dialInfo.uusInfo[0].uusData, pRI)) {
                memsetAndFreeStrings(1, dial.address);
                return ok();
            }
            uusInfo.uusLength = dialInfo.uusInfo[0].uusData.size();
        }

        dial.uusInfo = &uusInfo;
    }

    CALL_ONREQUEST(requestId, &dial, sizeOfDial, pRI, mSlotId);

    memsetAndFreeStrings(2, dial.address, uusInfo.uusData);

    return ok();
}

ScopedAStatus RadioVoice::emergencyDial(
        int32_t serial, const aidl::Dial& dialInfo,
        int32_t categories,
        const std::vector<std::string>& urns, aidl::EmergencyCallRouting routing,
        bool hasKnownUserIntentEmerg, bool isTesting) {
    int requestId = RIL_REQUEST_EMERGENCY_DIAL;
    if (getClientBySlot(mSlotId) == android::CLIENT_IMS) {
        requestId = RIL_REQUEST_IMS_EMERGENCY_DIAL;
    }

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, requestId);
    if (pRI == NULL) {
        return ok();
    }
    RIL_Emergency_Dial emergencyDial = {};
    RIL_Dial dialData = {};
    RIL_UUS_Info uusInfo = {};
    int32_t sizeOfEmergencyDial = sizeof(emergencyDial);

    // copy RIL_Dial
    if (!copyAidlStringToRil(&dialData.address, dialInfo.address, pRI)) {
        return ok();
    }

    dialData.clir = (int) dialInfo.clir;

    // copy RIL_UUS_Info
    if (dialInfo.uusInfo.size() != 0) {
        uusInfo.uusType = (RIL_UUS_Type) dialInfo.uusInfo[0].uusType;
        uusInfo.uusDcs = (RIL_UUS_DCS) dialInfo.uusInfo[0].uusDcs;

        if (dialInfo.uusInfo[0].uusData.size() == 0) {
            uusInfo.uusData = NULL;
            uusInfo.uusLength = 0;
        } else {
            if (!copyAidlStringToRil(&uusInfo.uusData, dialInfo.uusInfo[0].uusData, pRI)) {
                memsetAndFreeStrings(1, dialData.address);
                return ok();
            }
            uusInfo.uusLength = dialInfo.uusInfo[0].uusData.size();
        }

        dialData.uusInfo = &uusInfo;
    }

    emergencyDial.dialData = &dialData;
    emergencyDial.serviceCategory = (EmergencyServiceCategory) categories;
    emergencyDial.routing = (EmergencyCallRouting) routing;
    emergencyDial.isTesting = isTesting;

    CALL_ONREQUEST(requestId, &emergencyDial, sizeOfEmergencyDial, pRI, mSlotId);

    memsetAndFreeStrings(2, dialData.address, uusInfo.uusData);
    return ok();
}

ScopedAStatus RadioVoice::exitEmergencyCallbackMode(int32_t serial) {
    mtkLogD(LOG_TAG, "exitEmergencyCallbackMode: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE);
    return ok();
}

ScopedAStatus RadioVoice::explicitCallTransfer(int32_t serial) {
    mtkLogD(LOG_TAG, "explicitCallTransfer: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_EXPLICIT_CALL_TRANSFER);
    return ok();
}

ScopedAStatus RadioVoice::getCallForwardStatus(int32_t serial,
                                               const aidl::CallForwardInfo& callInfo) {
    mtkLogD(LOG_TAG, "getCallForwardStatus: serial %d", serial);
    dispatchCallForwardStatus(serial, mSlotId, RIL_REQUEST_QUERY_CALL_FORWARD_STATUS, callInfo);
    return ok();
}

ScopedAStatus RadioVoice::getCallWaiting(int32_t serial, int32_t serviceClass) {
    mtkLogD(LOG_TAG, "getCallWaiting: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_QUERY_CALL_WAITING, 1, serviceClass);
    return ok();
}

ScopedAStatus RadioVoice::getClip(int32_t serial) {
    mtkLogD(LOG_TAG, "getClip: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_QUERY_CLIP);
    return ok();
}

ScopedAStatus RadioVoice::getClir(int32_t serial) {
    mtkLogD(LOG_TAG, "explicitCallTransfer: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_CLIR);
    return ok();
}

ScopedAStatus RadioVoice::getCurrentCalls(int32_t serial) {
    mtkLogD(LOG_TAG, "getCurrentCalls: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_CURRENT_CALLS);
    return ok();
}

ScopedAStatus RadioVoice::getLastCallFailCause(int32_t serial) {
    mtkLogD(LOG_TAG, "getLastCallFailCause: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_LAST_CALL_FAIL_CAUSE);
    return ok();
}

ScopedAStatus RadioVoice::getMute(int32_t serial) {
    mtkLogD(LOG_TAG, "getMute: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_MUTE);
    return ok();
}

ScopedAStatus RadioVoice::getPreferredVoicePrivacy(int32_t serial) {
    mtkLogD(LOG_TAG, "getPreferredVoicePrivacy: serial %d", serial);
    dispatchVoid(serial, mSlotId,
            RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE);
    return ok();
}

ScopedAStatus RadioVoice::getTtyMode(int32_t serial) {
    mtkLogD(LOG_TAG, "getTTYMode: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_QUERY_TTY_MODE);
    return ok();
}

ScopedAStatus RadioVoice::handleStkCallSetupRequestFromSim(int32_t serial, bool accept) {
    mtkLogD(LOG_TAG, "handleStkCallSetupRequestFromSim: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM,
            1, BOOL_TO_INT(accept));
    return ok();
}

ScopedAStatus RadioVoice::hangup(int32_t serial, int32_t gsmIndex) {
    mtkLogD(LOG_TAG, "hangup: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_HANGUP, 1, gsmIndex);
    return ok();
}

ScopedAStatus RadioVoice::hangupForegroundResumeBackground(int32_t serial) {
    mtkLogD(LOG_TAG, "hangupForegroundResumeBackground: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND);
    return ok();
}

ScopedAStatus RadioVoice::hangupWaitingOrBackground(int32_t serial) {
    mtkLogD(LOG_TAG, "hangupWaitingOrBackground: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND);
    return ok();
}

ScopedAStatus RadioVoice::isVoNrEnabled(int32_t serial) {
    // TODO(b/203699028): can't call isVoNrEnabledResponse with 1.6 callback
    dispatchVoid(serial, mSlotId, RIL_REQUEST_IS_VONR_ENABLED);
    return ok();
}

ScopedAStatus RadioVoice::rejectCall(int32_t serial) {
    mtkLogD(LOG_TAG, "rejectCall: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_UDUB);
    return ok();
}

ScopedAStatus RadioVoice::responseAcknowledgement() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus RadioVoice::sendBurstDtmf(int32_t serial, const std::string& dtmf, int32_t on,
                                        int32_t off) {
    mtkLogD(LOG_TAG, "sendBurstDtmf: serial %d", serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_CDMA_BURST_DTMF, false,
            3, dtmf.c_str(), (std::to_string(on)).c_str(),
            (std::to_string(off)).c_str());
    return ok();
}

ScopedAStatus RadioVoice::sendCdmaFeatureCode(int32_t serial, const std::string& featureCode) {
    mtkLogD(LOG_TAG, "sendCDMAFeatureCode: serial %d", serial);
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_CDMA_FLASH,
            featureCode);
    return ok();
}

ScopedAStatus RadioVoice::sendDtmf(int32_t serial, const std::string& s) {
    mtkLogD(LOG_TAG, "sendDtmf: serial %d", serial);
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_DTMF, s);
    return ok();
}

ScopedAStatus RadioVoice::separateConnection(int32_t serial, int32_t gsmIndex) {
    mtkLogD(LOG_TAG, "separateConnection: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SEPARATE_CONNECTION, 1, gsmIndex);
    return ok();
}

ScopedAStatus RadioVoice::setCallForward(int32_t serial, const aidl::CallForwardInfo& callInfo) {
    mtkLogD(LOG_TAG, "setCallForward: serial %d", serial);
    dispatchCallForwardStatus(serial, mSlotId, RIL_REQUEST_SET_CALL_FORWARD, callInfo);
    return ok();
}

ScopedAStatus RadioVoice::setCallWaiting(int32_t serial, bool enable, int32_t serviceClass) {
    mtkLogD(LOG_TAG, "setCallWaiting: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_CALL_WAITING, 2, BOOL_TO_INT(enable),
            serviceClass);
    return ok();
}

ScopedAStatus RadioVoice::setClir(int32_t serial, int32_t status) {
    mtkLogD(LOG_TAG, "setClir: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_CLIR, 1, status);
    return ok();
}

ScopedAStatus RadioVoice::setMute(int32_t serial, bool enable) {
    mtkLogD(LOG_TAG, "setMute: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_MUTE, 1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus RadioVoice::setPreferredVoicePrivacy(int32_t serial, bool enable) {
    mtkLogD(LOG_TAG, "setPreferredVoicePrivacy: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE,
            1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus RadioVoice::setResponseFunctions(
        const std::shared_ptr<aidl::IRadioVoiceResponse>& voiceResponse,
        const std::shared_ptr<aidl::IRadioVoiceIndication>& voiceIndication) {
    mtkLogD(LOG_TAG, "aosp voice setResponseFunctions, slotId:%d", mSlotId);
    if (!isValidSlotId(mSlotId)) {
        return ok();
    }
    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp voice setResponseFunctions: got lock %d", mSlotId);

    radioVoiceServiceImpl[mSlotId].voiceResponse = voiceResponse;
    radioVoiceServiceImpl[mSlotId].voiceIndication = voiceIndication;
    rilContext->plusCounterRadioNumOne(mSlotId);

    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        rilContext->setAospResponseNumberToOne(rilAidlUtils::VOICE_INDEX, mSlotId);
    }

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp voice setResponseFunctions: release lock %d", mSlotId);
    // client is connected. Send initial indications.
    if (client == android::CLIENT_IMS) {
        rilContext->onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::IMS_AOSP);
    } else if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        if (rilContext->checkIfSetAllAospResponseDone(mSlotId) == 1) {
            if(client == android::CLIENT_TBOX) {
                rilContext->onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::TBOX_RADIO_AOSP);
            } else {
                rilContext->onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::RADIO_AOSP);
            }
        }
    }
    return ok();
}

ScopedAStatus RadioVoice::setTtyMode(int32_t serial, aidl::TtyMode mode) {
    mtkLogD(LOG_TAG, "setTTYMode: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_TTY_MODE, 1, mode);
    return ok();
}

ScopedAStatus RadioVoice::setVoNrEnabled(int32_t serial, bool enable) {
    // TODO(b/203699028): should set `persist.radio.is_vonr_enabled_` property instead
    dispatchInts(serial, mSlotId, RIL_REQUEST_ENABLE_VONR, 1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus RadioVoice::startDtmf(int32_t serial, const std::string& s) {
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_DTMF_START,
            s);
    return ok();
}

ScopedAStatus RadioVoice::stopDtmf(int32_t serial) {
    mtkLogD(LOG_TAG, "stopDtmf: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_DTMF_STOP);
    return ok();
}

ScopedAStatus RadioVoice::switchWaitingOrHoldingAndActive(int32_t serial) {
    mtkLogD(LOG_TAG, "switchWaitingOrHoldingAndActive: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE);
    return ok();
}

ScopedAStatus RadioVoice::cancelPendingUssd(int32_t serial) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_CANCEL_USSD);
    return ok();
}

ScopedAStatus RadioVoice::sendUssd(int32_t serial, const std::string& ussd) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_SEND_USSD, ussd.c_str());
    return ok();
}

}  // namespace android::hardware::radio::voice



namespace vendor::mediatek::hardware::mtkradioex::voice {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::voice;
constexpr auto ok = &ScopedAStatus::ok;

MtkRadioVoice::MtkRadioVoice(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus MtkRadioVoice::hangupAll(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "hangupAll: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_HANGUP_ALL);
    return ok();
}


ScopedAStatus MtkRadioVoice::hangupWithReason(int32_t serial, int32_t callId,
        int32_t reason, int32_t clientId) {
    mtkLogD(LOG_TAG, "hangupWithReason: serial %d, callId %d, reason %d, clientId %d",
            serial, callId, reason, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ)  &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, sId(clientId), RIL_REQUEST_HANGUP_WITH_REASON, 2, callId, reason);
    return ok();
}


ScopedAStatus MtkRadioVoice::getCallSubAddress(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getCallSubAddress: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_CALL_SUB_ADDRESS);
    return ok();
}

ScopedAStatus MtkRadioVoice::getColp(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getColp: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_COLP);
    return ok();
}

ScopedAStatus MtkRadioVoice::getColr(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getColr: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_COLR);
    return ok();
}


ScopedAStatus MtkRadioVoice::getEccNum(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getEccNum clientId %d", clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_ECC_NUM);
    return ok();
}

ScopedAStatus MtkRadioVoice::queryCallForwardInTimeSlotStatus(int32_t serial,
        const mtk_voice::CallForwardInfoEx& callInfoEx,
        int32_t clientId) {
    mtkLogD(LOG_TAG, "queryCallForwardInTimeSlotStatus: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchCallForwardInTimeSlotStatus(serial, sId(clientId),
            RIL_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT, callInfoEx);
    return ok();
}

ScopedAStatus MtkRadioVoice::sendCnap(int32_t serial, const std::string& cnapssMessage,
        int32_t clientId) {
    mtkLogD(LOG_TAG, "sendCnap: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_SEND_CNAP, cnapssMessage.c_str());
    return ok();
}

ScopedAStatus MtkRadioVoice::setBarringPasswordCheckedByNW(int32_t serial,
        const std::string& facility, const std::string& oldPassword,
        const std::string& newPassword,
        const std::string& cfmPassword, int32_t clientId) {
    mtkLogD(LOG_TAG, "setBarringPasswordCheckedByNW: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_CHANGE_BARRING_PASSWORD, true,
            4, facility.c_str(), oldPassword.c_str(), newPassword.c_str(), cfmPassword.c_str());
    return ok();
}

ScopedAStatus MtkRadioVoice::setCallForwardInTimeSlot(int32_t serial,
        const aidl::CallForwardInfoEx& callInfoEx, int32_t clientId) {
    mtkLogD(LOG_TAG, "setCallForwardInTimeSlot: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchCallForwardInTimeSlotStatus(serial, sId(clientId),
            RIL_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT, callInfoEx);
    return ok();
}

ScopedAStatus MtkRadioVoice::setCallIndication(int32_t serial, int32_t mode, int32_t callId,
        int32_t seqNumber, int32_t cause, int32_t clientId) {
    mtkLogD(LOG_TAG, "setCallIndication: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    mtkLogD(LOG_TAG, "sId(clientId = %d, clientId %d, ", sId(clientId), clientId);
    dispatchInts(serial, sId(clientId), RIL_REQUEST_SET_CALL_INDICATION, 4,
            mode, callId, seqNumber, cause);
    return ok();
}

ScopedAStatus MtkRadioVoice::setCallSubAddress(int32_t serial, bool enable, int32_t clientId) {
    mtkLogD(LOG_TAG, "setCallSubAddress: enable = %d, clientId %d", enable, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    dispatchInts(serial, sId(clientId), RIL_REQUEST_SET_CALL_SUB_ADDRESS, 1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus MtkRadioVoice::setCallValidTimer(int32_t serial, int32_t timer, int32_t clientId) {
    mtkLogD(LOG_TAG, "setCallValidTimer: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_GWSD_CALL_VALID, 1, timer);
    return ok();
}

ScopedAStatus MtkRadioVoice::setEccMode(int32_t serial, const std::string& number,
        int32_t enable, int32_t airplaneMode, int32_t imsReg, int32_t clientId) {
    mtkLogD(LOG_TAG, "setEccMode: serial %d, enable %d, airplaneMode %d, imsReg %d, clientId %d",
            serial, enable, airplaneMode, imsReg, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, sId(clientId), RIL_REQUEST_SET_ECC_MODE, true, 4,
        number.c_str(),
        std::to_string(enable).c_str(),
        std::to_string(airplaneMode).c_str(),
        std::to_string(imsReg).c_str());

    return ok();
}

ScopedAStatus MtkRadioVoice::setEccNum(int32_t serial, const std::string& eccListWithCard,
        const std::string& eccListNoCard, int32_t clientId) {
    mtkLogI(LOG_TAG, "setEccNum: eccListWithCard %s, eccListNoCard %s, clientId %d",
            eccListWithCard.c_str(), eccListNoCard.c_str(), clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, sId(clientId), RIL_REQUEST_SET_ECC_NUM, true, 2,
            eccListWithCard.c_str(), eccListNoCard.c_str());
    return ok();
}

ScopedAStatus MtkRadioVoice::setGwsdMode(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "setGwsdMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_SET_GWSD_MODE, data);
    return ok();
}

ScopedAStatus MtkRadioVoice::setIgnoreSameNumberInterval(int32_t serial, int32_t interval, int32_t clientId) {
    mtkLogD(LOG_TAG, "setIgnoreSameNumberInterval: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_GWSD_IGNORE_CALL_INTERVAL, 1, interval);
    return ok();
}

ScopedAStatus MtkRadioVoice::setKeepAliveByIpData(int32_t serial, const std::string& config, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_SET_GWSD_KEEP_ALIVE_IPDATA, config);
    return ok();
}

ScopedAStatus MtkRadioVoice::setKeepAliveByPDCPCtrlPDU(int32_t serial, const std::string& config, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_SET_GWSD_KEEP_ALIVE_PDCP, config);
    return ok();
}

ScopedAStatus MtkRadioVoice::setSuppServProperty(int32_t serial, const std::string& name, const std::string& value, int32_t clientId) {
    mtkLogD(LOG_TAG, "setSuppServProperty: serial %d, name %s, value %s, clientId %d",
            serial, name.c_str(), value.c_str(), clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_SET_SS_PROPERTY, true,
            2, name.c_str(), value.c_str());
    return ok();
}

ScopedAStatus MtkRadioVoice::setClip(int32_t serial, int32_t clipEnable, int32_t clientId) {
    mtkLogD(LOG_TAG, "setClip: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, sId(clientId), RIL_REQUEST_SET_CLIP, 1, clipEnable);
    return ok();
}

ScopedAStatus MtkRadioVoice::setColp(int32_t serial, int32_t colpEnable, int32_t clientId) {
    mtkLogD(LOG_TAG, "setColp: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, sId(clientId), RIL_REQUEST_SET_COLP, 1, colpEnable);
    return ok();
}

ScopedAStatus MtkRadioVoice::setColr(int32_t serial, int32_t colrEnable, int32_t clientId) {
    mtkLogD(LOG_TAG, "setColr: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, sId(clientId), RIL_REQUEST_SET_COLR, 1, colrEnable);
    return ok();
}

ScopedAStatus MtkRadioVoice::resetSuppServ(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "resetSuppServ: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_RESET_SUPP_SERV);
    return ok();
}

ScopedAStatus MtkRadioVoice::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus MtkRadioVoice::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExVoiceResponse>& radioResponse, const std::shared_ptr<aidl::IMtkRadioExVoiceIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk voice setResponseFunctions, slotId:%d", mSlotId);
    if (!isValidSlotId(mSlotId)) {
        return ok();
    }
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk voice setResponseFunctions: got lock %d", mSlotId);

    radioVoiceServiceImpl[mSlotId].mtkVoiceResponse = radioResponse;
    radioVoiceServiceImpl[mSlotId].mtkVoiceIndication = radioIndication;
    rilContext->plusCounterMtkRadioNumOne(mSlotId);

    rilContext->setMtkResponseNumberToOne(rilAidlUtils::VOICE_INDEX, mSlotId);
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk voice setResponseFunctions: release lock %d", mSlotId);

    // client is connected. Send initial indications.
    android::ClientId client = getClientBySlot(mSlotId);
    if (rilContext->checkIfSetAllMtkResponseDone(mSlotId) == 1) {
        if (client == android::CLIENT_TBOX) {
            rilContext->onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::TBOX_RADIO_MTK);
        } else {
            rilContext->onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::RADIO_MTK);
        }
    }

    return ok();
}

ScopedAStatus MtkRadioVoice::setResponseFunctionsMtkIms(const std::shared_ptr<aidl::IMtkRadioExVoiceResponse>& radioResponse, const std::shared_ptr<aidl::IMtkRadioExVoiceIndication>& radioIndication) {

    if (!isValidSlotId(mSlotId)) {
        return ok();
    }
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk voice setResponseFunctionsMtkIms: got lock %d", mSlotId);

    mtkLogD(LOG_TAG, "mtk voice setResponseFunctionsMtkIms, slotId:%d", mSlotId);
    radioVoiceServiceImpl[mSlotId].mtkImsVoiceResponse = radioResponse;
    radioVoiceServiceImpl[mSlotId].mtkImsVoiceIndication = radioIndication;
    rilContext->plusCounterMtkRadioNumOne(mSlotId + android::CLIENT_IMS * MAX_SIM_COUNT);

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk voice setResponseFunctionsMtkIms: release lock %d", mSlotId);
    return ok();
}

}

int mtkRadioExVoice::clearMtkVoiceResponseAndIndications(unsigned int slotId) {
    radioVoiceServiceImpl[slotId].mtkVoiceResponse = NULL;
    radioVoiceServiceImpl[slotId].mtkVoiceIndication = NULL;
    return 0;
}

int mtkRadioExVoice::clearMtkVoiceImsResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioVoiceServiceImpl[realSlotId].mtkImsVoiceResponse = NULL;
    radioVoiceServiceImpl[realSlotId].mtkImsVoiceIndication = NULL;
    return 0;
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addVoiceLock(std::shared_ptr<ndk::ICInterface> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_VOICE_PORT_BASE 55000
#define MTK_RADIO_VOICE_PORT_BASE 55100

static pthread_t gThreadIds[MAX_SIM_COUNT*4];

void* addRcpVsockRadioVoiceAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_VOICE_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_VOICE_PORT_BASE + sloid, sloid);

    std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidl = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(sloid);
    addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioVoiceAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void* addRcpVsockMtkRadioVoiceAidlService(void* arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, MTK_RADIO_VOICE_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", MTK_RADIO_VOICE_PORT_BASE + sloid, sloid);
    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(sloid);
    addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioVoiceAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);

    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadioVoiceAidl->asBinder().get()));
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioVoiceVsock(unsigned int &sloid, unsigned int &imsSlotid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
            addRcpVsockRadioVoiceAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioVoice");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT], NULL,
            addRcpVsockRadioVoiceAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(imsSlotid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioVoice");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT*2], NULL,
            addRcpVsockMtkRadioVoiceAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockMtkRadioVoice");
    }
}
#endif

void publicRadioVoiceAidlService(unsigned int sloid) {
    unsigned int imsSlotid = sloid + android::CLIENT_IMS * MAX_SIM_COUNT;

    const char *radioVoiceDescriptor = "";
    const char *mtkRadioVoiceDescriptor = "";

    if (android::hardware::radio::voice::RadioVoice::descriptor != NULL) {
        radioVoiceDescriptor = android::hardware::radio::voice::RadioVoice::descriptor;
    }
    if (vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice::descriptor != NULL) {
        mtkRadioVoiceDescriptor = vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice::descriptor;
    }

    /*if (sloid == 0) {
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidl = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(0);
        gPublishedHals.push_back(radioVoiceAidl);
        const std::string instance = std::string() + radioVoiceDescriptor + "/slot1";
        binder_status_t status =
                AServiceManager_addService(radioVoiceAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(imsSlotid);
        gPublishedHals.push_back(radioVoiceAidlforIms);
        const std::string imsInstance = std::string() + radioVoiceDescriptor + "/imsSlot1";
        binder_status_t imsStatus =
                AServiceManager_addService(radioVoiceAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(0);
        gPublishedHals.push_back(mtkRadioVoiceAidl);
        const std::string mtkInstance = std::string() + mtkRadioVoiceDescriptor + "/slot1";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioVoiceAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk voice addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 1) {
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidl = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(1);
        gPublishedHals.push_back(radioVoiceAidl);
        const std::string instance = std::string() + radioVoiceDescriptor + "/slot2";
        binder_status_t status =
                AServiceManager_addService(radioVoiceAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(imsSlotid);
        gPublishedHals.push_back(radioVoiceAidlforIms);
        const std::string imsInstance = std::string() + radioVoiceDescriptor + "/imsSlot2";
        binder_status_t imsStatus =
                AServiceManager_addService(radioVoiceAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(1);
        gPublishedHals.push_back(mtkRadioVoiceAidl);
        const std::string mtkInstance = std::string() + mtkRadioVoiceDescriptor + "/slot2";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioVoiceAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk voice addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 2) {
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidl = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(2);
        gPublishedHals.push_back(radioVoiceAidl);
        const std::string instance = std::string() + radioVoiceDescriptor + "/slot3";
        binder_status_t status =
                AServiceManager_addService(radioVoiceAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(imsSlotid);
        gPublishedHals.push_back(radioVoiceAidlforIms);
        const std::string imsInstance = std::string() + radioVoiceDescriptor + "/imsSlot3";
        binder_status_t imsStatus =
                AServiceManager_addService(radioVoiceAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(2);
        gPublishedHals.push_back(mtkRadioVoiceAidl);
        const std::string mtkInstance = std::string() + mtkRadioVoiceDescriptor + "/slot3";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioVoiceAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk voice addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 3) {
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidl = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(3);
        gPublishedHals.push_back(radioVoiceAidl);
        const std::string instance = std::string() + radioVoiceDescriptor + "/slot4";
        binder_status_t status =
                AServiceManager_addService(radioVoiceAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(imsSlotid);
        gPublishedHals.push_back(radioVoiceAidlforIms);
        const std::string imsInstance = std::string() + radioVoiceDescriptor + "/imsSlot4";
        binder_status_t imsStatus =
                AServiceManager_addService(radioVoiceAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp voice addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(3);
        gPublishedHals.push_back(mtkRadioVoiceAidl);
        const std::string mtkInstance = std::string() + mtkRadioVoiceDescriptor + "/slot4";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioVoiceAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk voice addservice, slotId:%d, status:%d", sloid, mtkStatus);
    }*/

    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;
    unsigned int tboxImsSlotid = sloid + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;

    if (sloid == 0) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioVoiceVsock(sloid, imsSlotid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTbox));
        const std::string tboxInstance = std::string() + radioVoiceDescriptor + "/tboxSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioVoiceAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidlforTbox = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioVoiceAidlforTbox));
        const std::string mtkTboxInstance = std::string() + mtkRadioVoiceDescriptor + "/tboxSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkRadioVoiceAidlforTbox->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox voice addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTboxIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxImsSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTboxIms));
        const std::string tboxImsInstance = std::string() + radioVoiceDescriptor + "/tboxImsSlot1";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(radioVoiceAidlforTboxIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);
    } else if (sloid == 1) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioVoiceVsock(sloid, imsSlotid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTbox));
        const std::string tboxInstance = std::string() + radioVoiceDescriptor + "/tboxSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioVoiceAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidlforTbox = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioVoiceAidlforTbox));
        const std::string mtkTboxInstance = std::string() + mtkRadioVoiceDescriptor + "/tboxSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkRadioVoiceAidlforTbox->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox voice addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTboxIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxImsSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTboxIms));
        const std::string tboxImsInstance = std::string() + radioVoiceDescriptor + "/tboxImsSlot2";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(radioVoiceAidlforTboxIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);
    } else if (sloid == 2) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioVoiceVsock(sloid, imsSlotid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTbox));
        const std::string tboxInstance = std::string() + radioVoiceDescriptor + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioVoiceAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidlforTbox = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioVoiceAidlforTbox));
        const std::string mtkTboxInstance = std::string() + mtkRadioVoiceDescriptor + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkRadioVoiceAidlforTbox->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox voice addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTboxIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxImsSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTboxIms));
        const std::string tboxImsInstance = std::string() + radioVoiceDescriptor + "/tboxImsSlot3";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(radioVoiceAidlforTboxIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);
    } else if (sloid == 3) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioVoiceVsock(sloid, imsSlotid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTbox));
        const std::string tboxInstance = std::string() + radioVoiceDescriptor + "/tboxSlot4";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioVoiceAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice> mtkRadioVoiceAidlforTbox = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::voice::MtkRadioVoice>(tboxSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioVoiceAidlforTbox));
        const std::string mtkTboxInstance = std::string() + mtkRadioVoiceDescriptor + "/tboxSlot4";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkRadioVoiceAidlforTbox->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox voice addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);

        std::shared_ptr<android::hardware::radio::voice::RadioVoice> radioVoiceAidlforTboxIms = ndk::SharedRefBase::make<android::hardware::radio::voice::RadioVoice>(tboxImsSlotid);
        addVoiceLock(std::static_pointer_cast<ndk::ICInterface>(radioVoiceAidlforTboxIms));
        const std::string tboxImsInstance = std::string() + radioVoiceDescriptor + "/tboxImsSlot4";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(radioVoiceAidlforTboxIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox voice addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);
    }
}
