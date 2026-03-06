/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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
#include <ril_service.h>
#include <mtk_log.h>
#include <binder/ProcessState.h>
#include <android/binder_manager.h>
#include <libmtkrilutils.h>
#include <radiosim_service.h>
#include <rilAidlUtils.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

#define BOOL_TO_INT(x) (x ? 1 : 0)
namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_radio_sim = ::aidl::android::hardware::radio::sim;
namespace aidl_radio_config = ::aidl::android::hardware::radio::config;
namespace aidl_mtkradioex_rsu = ::aidl::vendor::mediatek::hardware::mtkradioex::rsu;
namespace aidl_mtkradioex_se = ::aidl::vendor::mediatek::hardware::mtkradioex::se;
namespace aidl_mtkradioex_sim = ::aidl::vendor::mediatek::hardware::mtkradioex::sim;

using rilAidlUtils::populateAidlResponseInfo;
using rilAidlUtils::convertIntToAidlRadioIndicationType;
using rilAidlUtils::copyAidlStringToRil;
using rilAidlUtils::dispatchAidlRaw;
using rilAidlUtils::retChecker;

extern "C" unsigned int toRealSlot(unsigned int slotId);

struct RadioSimImpl {
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::sim::IMtkRadioExSimResponse> mtkSimResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::sim::IMtkRadioExSimIndication> mtkSimIndication;
    std::shared_ptr<aidl_mtkradioex_rsu::IRsuRadioResponse> mtkRsuResponse;
    std::shared_ptr<aidl_mtkradioex_rsu::IRsuRadioIndication> mtkRsuIndication;
    std::shared_ptr<aidl_mtkradioex_se::ISERadioResponse> mtkSeResponse;
    std::shared_ptr<aidl_mtkradioex_se::ISERadioIndication> mtkSeIndication;
    std::shared_ptr<::aidl::android::hardware::radio::sim::IRadioSimResponse> simResponse;
    std::shared_ptr<::aidl::android::hardware::radio::sim::IRadioSimIndication> simIndication;
};

RadioSimImpl radioSimServiceImpl[MAX_SIM_COUNT*(android::CLIENT_TBOX_IMS+1)];

aidl_radio_sim::IccIoResult responseIccIo(aidl_radio::RadioResponseInfo& responseInfo, int serial,
                        int responseType,RIL_Errno e, const void *response, size_t responseLen) {
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    aidl_radio_sim::IccIoResult result = {};

    if (response == NULL || responseLen != sizeof(RIL_SIM_IO_Response)) {
        mtkLogE(LOG_TAG, "responseIccIo Invalid response: NULL");
        if (e == RIL_E_SUCCESS) {
            responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }
        result.simResponse = std::string("");
    } else {
        RIL_SIM_IO_Response *resp = (RIL_SIM_IO_Response *) response;
        result.sw1 = resp->sw1;
        result.sw2 = resp->sw2;
        result.simResponse = convertCharPtrToStdString(resp->simResponse);
    }
    return result;
}

int simResponseInt(aidl_radio::RadioResponseInfo&  responseInfo, int serial, int responseType,
        RIL_Errno e, const void *response, size_t responseLen) {
    populateAidlResponseInfo(responseInfo, serial, responseType, e);
    int ret = -1;

    if (response == NULL || responseLen != sizeof(int)) {
        mtkLogE(LOG_TAG, "simResponseInt: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        int *p_int = (int *) response;
        ret = p_int[0];
    }
    return ret;
}

int simResponseIntOrEmpty(aidl_radio::RadioResponseInfo&  responseInfo, int serial,
        int responseType, RIL_Errno e, const void *response, size_t responseLen) {
    populateAidlResponseInfo(responseInfo, serial, responseType, e);
    int ret = -1;

    if (response == NULL && responseLen == 0) {
        // Earlier RILs did not send a response for some cases although the interface
        // expected an integer as response. Do not return error if response is empty. Instead
        // Return -1 in those cases to maintain backward compatibility.
    } else if (response == NULL || responseLen != sizeof(int)) {
        mtkLogE(LOG_TAG, "simResponseIntOrEmpty: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        int *p_int = (int *) response;
        ret = p_int[0];
    }
    return ret;
}


//////////AOSP radio
// response
int radioSim::areUiccApplicationsEnabledResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        bool enable = false;
        if (response == NULL || responselen != sizeof(int)) {
            mtkLogE(LOG_TAG, "areUiccApplicationsEnabledResponse invalid response");
            if (response != NULL) {
                if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            enable = ((int32_t *) response)[0];
            mtkLogD(LOG_TAG, "areUiccApplicationsEnabledResponse %d", enable);
        }
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->areUiccApplicationsEnabledResponse(
                responseInfo, enable);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "areUiccApplicationsEnabledResponse: invalid ret response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].areUiccApplicationsEnabledResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::changeIccPin2ForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->changeIccPin2ForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "changeIccPin2ForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].changeIccPin2ForAppResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::changeIccPinForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->changeIccPinForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "changeIccPinForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].changeIccPinForAppResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::enableUiccApplicationsResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->enableUiccApplicationsResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enableUiccApplicationsResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].enableUiccApplicationsResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::getAllowedCarriersResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogI(LOG_TAG, "getAllowedCarriersResponse: serial %d", serial);

    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio_sim::CarrierRestrictions carriers = {};
        aidl_radio_sim::SimLockMultiSimPolicy multisimPolicy
                = aidl_radio_sim::SimLockMultiSimPolicy::NO_MULTISIM_POLICY;
        if ((response == NULL) || (responseLen != sizeof(RIL_CarrierRestrictionsWithPriority))) {
            mtkLogE(LOG_TAG, "getAllowedCarriersResponse RIL_E_REQUEST_NOT_SUPPORTED");
        } else {
            RIL_CarrierRestrictionsWithPriority *pCr
                    = (RIL_CarrierRestrictionsWithPriority *)response;

            carriers.allowedCarriers.resize(pCr->len_allowed_carriers);
            for (int i = 0; i < pCr->len_allowed_carriers; i++) {
                RIL_Carrier *carrier = pCr->allowed_carriers + i;
                carriers.allowedCarriers[i].mcc = convertCharPtrToStdString(carrier->mcc);
                carriers.allowedCarriers[i].mnc = convertCharPtrToStdString(carrier->mnc);
                carriers.allowedCarriers[i].matchType = (int)(carrier->match_type);
                carriers.allowedCarriers[i].matchData =
                        convertCharPtrToStdString(carrier->match_data);
            }

            carriers.excludedCarriers.resize(pCr->len_excluded_carriers);
            for (int i = 0; i < pCr->len_excluded_carriers; i++) {
                RIL_Carrier *carrier = pCr->excluded_carriers + i;
                carriers.excludedCarriers[i].mcc = convertCharPtrToStdString(carrier->mcc);
                carriers.excludedCarriers[i].mnc = convertCharPtrToStdString(carrier->mnc);
                carriers.excludedCarriers[i].matchType = (int)(carrier->match_type);
                carriers.excludedCarriers[i].matchData =
                        convertCharPtrToStdString(carrier->match_data);
            }

            carriers.allowedCarriersPrioritized = pCr->allowedCarriersPrioritized;
            carriers.status = (aidl_radio_sim::CarrierRestrictions:: \
                    CarrierRestrictionStatus)(pCr->status);
            multisimPolicy = (aidl_radio_sim::SimLockMultiSimPolicy)
                    pCr->simLockMultiSimPolicy;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse
                    ->getAllowedCarriersResponse(responseInfo, carriers, multisimPolicy);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getAllowedCarriersResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getAllowedCarriersResponse: radioSimServiceImpl[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioSim::getCdmaSubscriptionResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        int numStrings = responselen / sizeof(char *);
        std::string emptyString;

        if (response == NULL || numStrings != 5) {
            mtkLogE(LOG_TAG, "getOperatorResponse: invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->getCdmaSubscriptionResponse(
                    responseInfo, emptyString, emptyString, emptyString, emptyString, emptyString);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getCdmaSubscriptionResponse: invalid ret response");
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        } else {
            char **resp = (char **) response;
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->getCdmaSubscriptionResponse(
                    responseInfo,
                    convertCharPtrToStdString(resp[0]),
                    convertCharPtrToStdString(resp[1]),
                    convertCharPtrToStdString(resp[2]),
                    convertCharPtrToStdString(resp[3]),
                    convertCharPtrToStdString(resp[4]));
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getCdmaSubscriptionResponse: invalid response");
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getCdmaSubscriptionResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::getCdmaSubscriptionSourceResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseInt(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->getCdmaSubscriptionSourceResponse(
                responseInfo, (aidl_radio_sim::CdmaSubscriptionSource)rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCdmaSubscriptionSourceResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getCdmaSubscriptionSourceResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::getFacilityLockForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseInt(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->getFacilityLockForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getFacilityLockForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getFacilityLockForAppResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::getIccCardStatusResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int token, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, token, responseType, e);

        aidl_radio_sim::CardStatus cardStatus = {};
        aidl_radio_config::SlotPortMapping slotPortMapping = {};
        RIL_CardStatus_v8 *p_cur = ((RIL_CardStatus_v8 *) response);
        if (response == NULL || responselen != sizeof(RIL_CardStatus_v8)
                || p_cur->gsm_umts_subscription_app_index >= p_cur->num_applications
                || p_cur->cdma_subscription_app_index >= p_cur->num_applications
                || p_cur->ims_subscription_app_index >= p_cur->num_applications) {
            mtkLogE(LOG_TAG, "getIccCardStatusResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            cardStatus.cardState = (int) p_cur->card_state;
            cardStatus.universalPinState = (aidl_radio_sim::PinState) p_cur->universal_pin_state;
            cardStatus.gsmUmtsSubscriptionAppIndex = p_cur->gsm_umts_subscription_app_index;
            cardStatus.cdmaSubscriptionAppIndex = p_cur->cdma_subscription_app_index;
            cardStatus.imsSubscriptionAppIndex = p_cur->ims_subscription_app_index;

            std::vector<aidl_radio_sim::AppStatus> appStatus;
            appStatus.resize(p_cur->num_applications);

            RIL_AppStatus *rilAppStatus = p_cur->applications;
            for (int i = 0; i < p_cur->num_applications; i++) {
                appStatus[i].appType = (int) rilAppStatus[i].app_type;
                appStatus[i].appState = (int) rilAppStatus[i].app_state;
                appStatus[i].persoSubstate = (aidl_radio_sim::PersoSubstate) rilAppStatus[i].perso_substate;
                appStatus[i].aidPtr = convertCharPtrToStdString(rilAppStatus[i].aid_ptr);
                appStatus[i].appLabelPtr = convertCharPtrToStdString(rilAppStatus[i].app_label_ptr);
                appStatus[i].pin1Replaced = rilAppStatus[i].pin1_replaced;
                appStatus[i].pin1 = (aidl_radio_sim::PinState) rilAppStatus[i].pin1;
                appStatus[i].pin2 = (aidl_radio_sim::PinState) rilAppStatus[i].pin2;
            }

            cardStatus.applications = appStatus;

            cardStatus.atr = convertCharPtrToStdString(p_cur->atr);
            cardStatus.iccid = convertCharPtrToStdString(p_cur->iccId);
            cardStatus.eid = convertCharPtrToStdString(p_cur->eid);

            slotPortMapping.physicalSlotId = p_cur->slotPort.physicalSlotId;
            slotPortMapping.portId = p_cur->slotPort.portId;

            cardStatus.slotMap = slotPortMapping;
            cardStatus.supportedMepMode =
                    (aidl_radio_config::MultipleEnabledProfilesMode) p_cur->mepMode;
        }
        /// TBD
        /// Gloable card state
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId]
                .simResponse->getIccCardStatusResponse(responseInfo, cardStatus);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getIccCardStatusResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].supplyIccPinForAppResponse == NULL", slotId);
    }

    return 0;
}

int radioSim::getIMSIForAppResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                        int serial, RIL_Errno e, const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->getImsiForAppResponse(
                responseInfo, convertCharPtrToStdString((char*) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getIMSIForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getIMSIForAppResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::getSimPhonebookRecordsResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    mtkLogD(LOG_TAG, "getSimPhonebookRecordsResponse: serial %d, slotId %d", serial, slotId);

    if (radioSimServiceImpl[slotId].simResponse == NULL) {
          mtkLogE("%s: radioSimServiceImpl[slotId].simResponse == NULL", __FUNCTION__, slotId);
          return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret =
            radioSimServiceImpl[slotId].simResponse->getSimPhonebookRecordsResponse(responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "getSimPhonebookRecordsResponse: slotId = %d, binder fail!", slotId);
    }
    return 0;
}

int radioSim::getSimPhonebookCapacityResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    mtkLogD(LOG_TAG, "getSimPhonebookCapacityResponse: serial %d, slotId %d", serial, slotId);

    if (radioSimServiceImpl[slotId].simResponse == NULL) {
        mtkLogE("%s: radioSimServiceImpl[slotId].simResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    aidl_radio::sim::PhonebookCapacity pbCapacity = {};
    if (response != NULL && responselen % sizeof(int) == 0) {
        int *pInt = (int *)response;
        pbCapacity.maxAdnRecords = (int32_t)pInt[0];
        pbCapacity.usedAdnRecords = (int32_t)pInt[1];
        pbCapacity.maxEmailRecords = (int32_t)pInt[2];
        pbCapacity.usedEmailRecords = (int32_t)pInt[3];
        pbCapacity.maxAdditionalNumberRecords = (int32_t)pInt[4];
        pbCapacity.usedAdditionalNumberRecords = (int32_t)pInt[5];
        pbCapacity.maxNameLen = (int32_t)pInt[6];
        pbCapacity.maxNumberLen = (int32_t)pInt[7];
        pbCapacity.maxEmailLen = (int32_t)pInt[8];
        pbCapacity.maxAdditionalNumberLen = (int32_t)pInt[9];
    }
    ndk::ScopedAStatus ret =
            radioSimServiceImpl[slotId].simResponse->getSimPhonebookCapacityResponse(
            responseInfo, pbCapacity);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "getSimPhonebookCapacityResponse: slotId = %d, binder fail!", slotId);
    }
    return 0;
}

int radioSim::iccCloseLogicalChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->iccCloseLogicalChannelResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "iccCloseLogicalChannelResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].iccCloseLogicalChannelResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::iccCloseLogicalChannelWithSessionInfoResponse(unsigned int slotId,
                        android::ClientId clientId, int responseType, int serial,
                        RIL_Errno e, const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->iccCloseLogicalChannelWithSessionInfoResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "iccCloseLogicalChannelWithSessionInfoResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].iccCloseLogicalChannelWithSessionInfoResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::iccIOForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio_sim::IccIoResult iccIoResult = responseIccIo(
                responseInfo, serial, responseType, e, response, responselen);

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->iccIoForAppResponse(
                responseInfo, iccIoResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "iccIOForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].iccIOForAppResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::iccOpenLogicalChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        int channelId = -1;
        std::vector<uint8_t> selectResponse;
        int numInts = responselen / sizeof(int);
        if (response == NULL || responselen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "iccOpenLogicalChannelResponse invalid response");
            if (response != NULL) {
                if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            int *pInt = (int *) response;
            channelId = pInt[0];
            selectResponse.resize(numInts - 1);
            for (int i = 1; i < numInts; i++) {
                selectResponse[i - 1] = (uint8_t) pInt[i];
            }
        }

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->iccOpenLogicalChannelResponse(
                responseInfo, channelId, selectResponse);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "iccOpenLogicalChannelResponse: invalid ret response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].iccOpenLogicalChannelResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::iccTransmitApduBasicChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio_sim::IccIoResult iccIoResult = responseIccIo(
                responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->iccTransmitApduBasicChannelResponse(
                responseInfo, iccIoResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "iccTransmitApduBasicChannelResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].iccTransmitApduBasicChannelResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::iccTransmitApduLogicalChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio_sim::IccIoResult iccIoResult = responseIccIo(
                responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->iccTransmitApduLogicalChannelResponse(
                responseInfo, iccIoResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "iccTransmitApduLogicalChannelResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].iccTransmitApduLogicalChannelResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::reportStkServiceIsRunningResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        mtkLogD(LOG_TAG, "reportStkServiceIsRunningResponse: serial %d", serial);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->reportStkServiceIsRunningResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG,
                    "reportStkServiceIsRunningResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "reportStkServiceIsRunningResponse: radioSimServiceImpl[%d]->simResponse == NULL",
                slotId);
    }
    return 0;
}

int radioSim::requestIccSimAuthenticationResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio_sim::IccIoResult iccIoResult = responseIccIo(
                responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->requestIccSimAuthenticationResponse(
                responseInfo, iccIoResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "requestIccSimAuthenticationResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].requestIccSimAuthenticationResponse == NULL", slotId);
    }
    return 0;
}


int radioSim::sendEnvelopeResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        mtkLogD(LOG_TAG, "sendEnvelopeResponse: serial %d", serial);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->sendEnvelopeResponse(
                responseInfo, convertCharPtrToStdString((char *)response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendEnvelopeResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "sendEnvelopeResponse: radioSimServiceImpl[%d]->simResponse == NULL",
                slotId);
    }

    return 0;
}

int radioSim::sendEnvelopeWithStatusResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        mtkLogD(LOG_TAG, "sendEnvelopeWithStatusResponse: serial %d", serial);
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio_sim::IccIoResult  result = responseIccIo(responseInfo, serial, responseType, e,
                response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->sendEnvelopeWithStatusResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG,"sendEnvelopeWithStatusResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "sendEnvelopeWithStatusResponse: radioSimServiceImpl[%d]->simResponse == NULL",
                slotId);
    }
    return 0;
}

int radioSim::sendTerminalResponseToSimResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        mtkLogD(LOG_TAG, "sendTerminalResponseToSimResponse: serial %d", serial);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->sendTerminalResponseToSimResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG,
                    "sendTerminalResponseToSimResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "sendTerminalResponseToSimResponse: radioSimServiceImpl[%d]->simResponse == NULL",
                slotId);
    }
    return 0;
}

int radioSim::setAllowedCarriersResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    mtkLogI(LOG_TAG, "setAllowedCarriersResponse: serial %d", serial);

    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->setAllowedCarriersResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setAllowedCarriersResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setAllowedCarriersResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioSim::setCarrierInfoForImsiEncryptionResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setCarrierInfoForImsiEncryptionResponse: serial %d", serial);
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->setCarrierInfoForImsiEncryptionResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCarrierInfoForImsiEncryptionResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setCarrierInfoForImsiEncryptionResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioSim::setCdmaSubscriptionSourceResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->setCdmaSubscriptionSourceResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCdmaSubscriptionSourceResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].setCdmaSubscriptionSourceResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::setFacilityLockForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->setFacilityLockForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getFacilityLockForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getFacilityLockForAppResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::setSimCardPowerResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->setSimCardPowerResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSimCardPowerResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].setSimCardPowerResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::setUiccSubscriptionResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->setUiccSubscriptionResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setUiccSubscriptionResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].setUiccSubscriptionResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::supplyIccPin2ForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->supplyIccPin2ForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplyIccPin2ForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].supplyIccPin2ForAppResponse == NULL", slotId);
    }
    return 0;
}


int radioSim::supplyIccPinForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->supplyIccPinForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplyIccPinForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].supplyIccPinForAppResponse == NULL", slotId);
    }
    return 0;
}


int radioSim::supplyIccPuk2ForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->supplyIccPuk2ForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplyIccPuk2ForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].supplyIccPuk2ForAppResponse == NULL", slotId);
    }
    return 0;
}


int radioSim::supplyIccPukForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int rsp = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responselen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse->supplyIccPukForAppResponse(
                responseInfo, rsp);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplyIccPukForAppResponse: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].supplyIccPukForAppResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::supplySimDepersonalizationResponse(unsigned int slotId,
                        android::ClientId clientId __unused, int responseType, int serial,
                        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "supplySimDepersonalizationResponse: serial %d", serial);

    if (radioSimServiceImpl[slotId].simResponse != NULL) {
        int32_t remainingRetry = -1;
        aidl_radio_sim::PersoSubstate persoType =
                aidl_radio_sim::PersoSubstate::SIM_NETWORK;
        aidl_radio::RadioResponseInfo responseInfo = {};
        if (response != NULL && responseLen == 2 * sizeof(int32_t)) {
            persoType = (aidl_radio_sim::PersoSubstate)(((int*)response)[0]);
            remainingRetry = ((int*)response)[1];
            mtkLogD(LOG_TAG, "supplySimDepersonalizationResponse: persoType:%d, remainRetry:%d",
                    persoType, remainingRetry);
        } else {
            mtkLogE(LOG_TAG, "supplySimDepersonalizationResponse: responseLen:%d", (int)responseLen);
            if (e == RIL_E_SUCCESS) {
                e = RIL_E_INVALID_RESPONSE;
            }
        }
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simResponse
                ->supplySimDepersonalizationResponse(responseInfo, persoType, remainingRetry);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplySimDepersonalizationResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }

    } else {
        mtkLogE(LOG_TAG,
                "supplySimDepersonalizationResponse: "
                "radioSimServiceImpl[%d]->networkResponse == NULL", slotId);
    }
    return 0;
}

int radioSim::updateSimPhonebookRecordsResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    mtkLogD(LOG_TAG, "updateSimPhonebookRecordsResponse: serial %d, slotId %d", serial, slotId);

    if (radioSimServiceImpl[slotId].simResponse == NULL) {
        mtkLogE("%s: radioSimServiceImpl[slotId].simResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    int *pInt = (int *)response;
    int entryIndex = (int32_t)pInt[0];
    ndk::ScopedAStatus ret =
            radioSimServiceImpl[slotId].simResponse->updateSimPhonebookRecordsResponse(
            responseInfo, entryIndex);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "updateSimPhonebookRecordsResponse: slotId = %d, binder fail!", slotId);
    }
    return 0;
}

// indication
int radioSim::carrierInfoForImsiEncryption(unsigned int slotId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    return 0;
}

int radioSim::cdmaSubscriptionSourceChangedInd(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e,
                        const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[slotId].simIndication != NULL ||
            radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        if (response == NULL || responselen != sizeof(int)) {
            mtkLogE(LOG_TAG, "cdmaSubscriptionSourceChangedInd: invalid response");
            return 0;
        }
        int32_t cdmaSource = ((int32_t *) response)[0];
        if (radioSimServiceImpl[slotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->cdmaSubscriptionSourceChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    (aidl_radio_sim::CdmaSubscriptionSource) cdmaSource);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cdmaSubscriptionSourceChangedInd: invalid ret response");
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->cdmaSubscriptionSourceChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    (aidl_radio_sim::CdmaSubscriptionSource) cdmaSource);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "tbox-cdmaSubscriptionSourceChangedInd: invalid ret response");
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].cdmaSubscriptionSourceChangedInd == NULL", slotId);
    }
    return 0;
}

int radioSim::simPhonebookChanged(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simIndication != NULL) {
        mtkLogD(LOG_TAG, "simPhonebookChanged slotId:%d", slotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->simPhonebookChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simPhonebookChanged: slotId = %d, binder fail!", slotId);
        }
    } else {
        mtkLogE(LOG_TAG, "simPhonebookChanged: radioSimServiceImpl[%d].simIndication == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        mtkLogD(LOG_TAG, "simPhonebookChanged slotId:%d", tboxSlotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->simPhonebookChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simPhonebookChanged: tboxSlotId = %d, binder fail!", tboxSlotId);
        }
    } else {
        mtkLogE(LOG_TAG, "simPhonebookChanged: tbox-radioSimServiceImpl[%d].simIndication == NULL", tboxSlotId);
    }
    return 0;
}

int radioSim::simPhonebookRecordsReceived(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[slotId].simIndication != NULL ||
            radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        mtkLogD(LOG_TAG, "simPhonebookRecordsReceived slotId:%d", slotId);

        std::vector<aidl_radio::sim::PhonebookRecordInfo> records;
        aidl_radio::sim::PbReceivedStatus status = aidl_radio::sim::PbReceivedStatus::PB_RECEIVED_OK;

        if (response == NULL || responselen == 0 || responselen % sizeof (RIL_PHB_ENTRY *) != 0) {
            records.resize(0);
            status = aidl_radio::sim::PbReceivedStatus::PB_RECEIVED_ERROR;
            mtkLogE(LOG_TAG, "simPhonebookRecordsReceived: inavalid response slotId:%d", slotId);
        } else {
            int num = responselen / sizeof(RIL_PHB_ENTRY *);
            RIL_PHB_ENTRY **phbEntryExtResponse = (RIL_PHB_ENTRY **)response;
            if (num == 1 && phbEntryExtResponse[0]->hidden == 0x1234) {
                mtkLogE(LOG_TAG, "simPhonebookRecordsReceived: no sim contacts");
                records.resize(0);
                if (radioSimServiceImpl[slotId].simIndication != NULL) {
                    status = aidl_radio::sim::PbReceivedStatus::PB_RECEIVED_FINAL;
                    ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->simPhonebookRecordsReceived(
                            rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), status, records);
                    if (!ret.isOk()) {
                        mtkLogE(LOG_TAG, "simPhonebookRecordsReceived: slotId = %d, binder fail!", slotId);
                    }
                }

                if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
                    status = aidl_radio::sim::PbReceivedStatus::PB_RECEIVED_FINAL;
                    ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->simPhonebookRecordsReceived(
                            rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), status, records);
                    if (!ret.isOk()) {
                        mtkLogE(LOG_TAG, "simPhonebookRecordsReceived: tboxSlotId = %d, binder fail!", tboxSlotId);
                    }
                }
                return 0;
            }

            records.resize(num);
            for (int i = 0; i < num; i++) {
                if (phbEntryExtResponse[i]->hidden == 1) {
                    status = aidl_radio::sim::PbReceivedStatus::PB_RECEIVED_FINAL;
                }
                records[i].recordId = phbEntryExtResponse[i]->index;
                records[i].name = convertCharPtrToStdString(phbEntryExtResponse[i]->text);
                records[i].number = convertCharPtrToStdString(phbEntryExtResponse[i]->number);
                mtkLogD(LOG_TAG, "simPhonebookRecordsReceived records[%d].recordId=%d, name=%s, number=%s",
                        i, phbEntryExtResponse[i]->index, phbEntryExtResponse[i]->text, phbEntryExtResponse[i]->number);

                if (phbEntryExtResponse[i]->email != NULL && strlen(phbEntryExtResponse[i]->email) > 0) {
                    records[i].emails.resize(1);
                    records[i].emails[0] = convertCharPtrToStdString(phbEntryExtResponse[i]->email);
                    mtkLogD(LOG_TAG, "simPhonebookRecordsReceived records[%d].email=%s",
                            i, phbEntryExtResponse[i]->email);
                } else {
                    records[i].emails.resize(0);
                }

                if (phbEntryExtResponse[i]->adnumber != NULL && strlen(phbEntryExtResponse[i]->adnumber) > 0) {
                    records[i].additionalNumbers.resize(1);
                    records[i].additionalNumbers[0] = convertCharPtrToStdString(phbEntryExtResponse[i]->adnumber);
                    mtkLogD(LOG_TAG, "Received records[%d].adnumber=%s", i, phbEntryExtResponse[i]->adnumber);
                } else {
                    records[i].additionalNumbers.resize(0);
                }
            }
        }

        if (radioSimServiceImpl[slotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->simPhonebookRecordsReceived(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), status, records);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "simPhonebookRecordsReceived: slotId = %d, binder fail!", slotId);
            }
        }

        if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->simPhonebookRecordsReceived(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), status, records);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "simPhonebookRecordsReceived: tboxSlotId = %d, binder fail!", tboxSlotId);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "simPhonebookRecordsReceived: radioSimServiceImpl[%d].simIndication == NULL", slotId);
    }
    return 0;
}

int radioSim::simRefreshInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    if (response == NULL || responselen != sizeof(RIL_SimRefreshResponse_v7)) {
        mtkLogE(LOG_TAG, "simRefreshInd: invalid response");
        return 0;
    }

    aidl_radio_sim::SimRefreshResult refreshResult = {};
    RIL_SimRefreshResponse_v7 *simRefreshResponse = ((RIL_SimRefreshResponse_v7 *) response);
    refreshResult.type = (int) simRefreshResponse->result;
    refreshResult.efId = simRefreshResponse->ef_id;
    refreshResult.aid = convertCharPtrToStdString(simRefreshResponse->aid);
    mtkLogD(LOG_TAG, "simRefreshInd: type %d efId %d", refreshResult.type, refreshResult.efId);

    if (radioSimServiceImpl[slotId].simIndication != NULL) {
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->simRefresh(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                refreshResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simRefreshInd: invalid ret response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].simRefresh == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->simRefresh(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                refreshResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-simRefreshInd: invalid ret response");
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].simRefresh == NULL", tboxSlotId);
    }

    unsigned int seSlotId = slotId + android::CLIENT_SE * MAX_SIM_COUNT;
    if (radioSimServiceImpl[seSlotId].simIndication != NULL) {
        ndk::ScopedAStatus ret = radioSimServiceImpl[seSlotId].simIndication->simRefresh(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                refreshResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simRefreshInd: invalid response");
            retChecker->aospAidlReturnStatusChecker(seSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].simRefresh == NULL", seSlotId);
    }
    return 0;
}


int radioSim::simStatusChangedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simIndication != NULL) {
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->simStatusChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simStatusChangedInd: invalid response");
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].simStatusChanged == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->simStatusChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-simStatusChangedInd: invalid response");
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].simStatusChanged == NULL", tboxSlotId);
    }

    unsigned int seSlotId = slotId + android::CLIENT_SE * MAX_SIM_COUNT;
    if (radioSimServiceImpl[seSlotId].simIndication != NULL) {
        ndk::ScopedAStatus ret = radioSimServiceImpl[seSlotId].simIndication->simStatusChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simStatusChangedInd: invalid response");
            retChecker->aospAidlReturnStatusChecker(seSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].simStatusChanged == NULL", seSlotId);
    }
    return 0;
}

int radioSim::stkEventNotifyInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "stkEventNotifyInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "stkEventNotifyInd, slotid:%d", slotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->stkEventNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkEventNotifyInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkEventNotifyInd: radioSimServiceImpl[%d].simIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "tbox-stkEventNotifyInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "stkEventNotifyInd, tboxSlotId:%d", tboxSlotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->stkEventNotify(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkEventNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkEventNotifyInd: tbox-radioSimServiceImpl[%d].simIndication == NULL",
                tboxSlotId);
    }

    return 0;
}

int radioSim::stkProactiveCommandInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "stkProactiveCommandInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "stkProactiveCommandInd, slotid:%d", slotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->stkProactiveCommand(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkProactiveCommandInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkProactiveCommandInd: radioSimServiceImpl[%d].simIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "tbox-stkProactiveCommandInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "stkProactiveCommandInd, tboxSlotId:%d", tboxSlotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->stkProactiveCommand(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkProactiveCommandInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkProactiveCommandInd: tbox-radioSimServiceImpl[%d].simIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioSim::stkSessionEndInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    if (radioSimServiceImpl[slotId].simIndication != NULL) {
        mtkLogD(LOG_TAG, "stkSessionEndInd, slotid:%d", slotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->stkSessionEnd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkSessionEndInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkSessionEndInd: radioSimServiceImpl[%d].simIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        mtkLogD(LOG_TAG, "stkSessionEndInd, tboxSlotId:%d", tboxSlotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->stkSessionEnd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stkSessionEndInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "stkSessionEndInd: tbox-radioSimServiceImpl[%d].simIndication == NULL",
                tboxSlotId);
    }

    return 0;
}

int radioSim::subscriptionStatusChangedInd(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[slotId].simIndication != NULL ||
            radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        if (response == NULL || responselen != sizeof(int)) {
            mtkLogE(LOG_TAG, "subscriptionStatusChangedInd: invalid response");
            return 0;
        }
        bool active = ((int32_t *) response)[0];
        if (radioSimServiceImpl[slotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->subscriptionStatusChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), active);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "subscriptionStatusChangedInd: invalid ret response");
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }
        if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].simIndication->subscriptionStatusChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), active);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "tbox-subscriptionStatusChangedInd: invalid ret response");
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].subscriptionStatusChanged == NULL", slotId);
    }
    return 0;
}

int radioSim::uiccApplicationsEnablementChanged(unsigned int slotId, int indicationType, int token,
                        RIL_Errno e, const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[slotId].simIndication != NULL ||
            radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
        if (response == NULL || responselen != sizeof(int)) {
            mtkLogE(LOG_TAG, "uiccApplicationsEnablementChanged: invalid response");
            return 0;
        }
        bool enable = ((int32_t *) response)[0];
        if (radioSimServiceImpl[slotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->uiccApplicationsEnablementChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), enable);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "uiccApplicationsEnablementChanged: invalid ret response");
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioSimServiceImpl[tboxSlotId].simIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].simIndication->uiccApplicationsEnablementChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), enable);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "tbox-uiccApplicationsEnablementChanged: invalid ret response");
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].uiccApplicationsEnablementChanged == NULL", slotId);
    }
    return 0;
}

int radioSim::requestIsimAuthenticationResponse(unsigned int slotId,
                        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    return 0;
}

int radioSim::clearSimResponseAndIndications(unsigned int slotId) {
    radioSimServiceImpl[slotId].simResponse = NULL;
    radioSimServiceImpl[slotId].simIndication = NULL;
    return 0;
}

namespace android::hardware::radio::sim {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::android::hardware::radio::sim;
constexpr auto ok = &ScopedAStatus::ok;

RadioSim::RadioSim(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus RadioSim::areUiccApplicationsEnabled(int32_t serial) {
    mtkLogD(LOG_TAG, "areUiccApplicationsEnabled: slotid:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_ARE_UICC_APPLICATIONS_ENABLED);
    return ok();
}

ScopedAStatus RadioSim::changeIccPin2ForApp(int32_t serial, const std::string& oldPin2,
                                            const std::string& newPin2, const std::string& aid) {
    mtkLogD(LOG_TAG, "changeIccPin2ForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_CHANGE_SIM_PIN2, true,
            3, oldPin2.c_str(), newPin2.c_str(), aid.c_str());
    return ok();
}

ScopedAStatus RadioSim::changeIccPinForApp(int32_t serial, const std::string& oldPin,
                                           const std::string& newPin, const std::string& aid) {
    mtkLogD(LOG_TAG, "changeIccPinForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_CHANGE_SIM_PIN, true, 3,
            oldPin.c_str(), newPin.c_str(), aid.c_str());
    return ok();
}

ScopedAStatus RadioSim::enableUiccApplications(int32_t serial, bool enable) {
    mtkLogD(LOG_TAG, "enableUiccApplications: slotid:%d, serial %d", mSlotId, serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_ENABLE_UICC_APPLICATIONS, 1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus RadioSim::getAllowedCarriers(int32_t serial) {
    mtkLogD(LOG_TAG, "getAllowedCarriers:: slotId:%d, serial:%d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_ALLOWED_CARRIERS);
    return ok();
}

ScopedAStatus RadioSim::getCdmaSubscription(int32_t serial) {
    mtkLogD(LOG_TAG, "getCdmaSubscription: slotid:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_CDMA_SUBSCRIPTION);
    return ok();
}

ScopedAStatus RadioSim::getCdmaSubscriptionSource(int32_t serial) {
    mtkLogD(LOG_TAG, "getCdmaSubscriptionSource: slotid:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE);
    return ok();
}

ScopedAStatus RadioSim::getFacilityLockForApp(  //
        int32_t serial, const std::string& facility, const std::string& password,
        int32_t serviceClass, const std::string& appId) {
    mtkLogD(LOG_TAG, "getFacilityLockForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_QUERY_FACILITY_LOCK,
            true,
            4, facility.c_str(), password.c_str(),
            (std::to_string(serviceClass)).c_str(), appId.c_str());
    return ok();
}

ScopedAStatus RadioSim::getIccCardStatus(int32_t serial) {
    mtkLogD(LOG_TAG, "getIccCardStatus: slotid:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_SIM_STATUS);
    return ok();
}

ScopedAStatus RadioSim::getImsiForApp(int32_t serial, const std::string& aid) {
    mtkLogD(LOG_TAG, "getImsiForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_GET_IMSI, false,
            1, aid.c_str());
    return ok();
}

ScopedAStatus RadioSim::getSimPhonebookCapacity(int32_t serial) {
    mtkLogD(LOG_TAG, "getSimPhonebookCapacity: slotid:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_SIM_PHONEBOOK_CAPACITY);
    return ok();
}

ScopedAStatus RadioSim::getSimPhonebookRecords(int32_t serial) {
    mtkLogD(LOG_TAG, "getSimPhonebookRecords: slotid:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_SIM_PHONEBOOK_RECORDS);
    return ok();
}

ScopedAStatus RadioSim::iccCloseLogicalChannel(int32_t serial, int32_t channelId) {
    mtkLogD(LOG_TAG, "iccCloseLogicalChannel: slotid:%d, serial %d", mSlotId, serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SIM_CLOSE_CHANNEL, 2, channelId, 0);
    return ok();
}

ScopedAStatus RadioSim::iccCloseLogicalChannelWithSessionInfo(int32_t serial,
        const aidl::SessionInfo& sessionInfo) {
    mtkLogD(LOG_TAG, "iccCloseLogicalChannelWithSessionInfo: slotid:%d, serial %d", mSlotId, serial);
    int isEs10 = sessionInfo.isEs10 ? 1: 0;
    dispatchInts(serial, mSlotId, RIL_REQUEST_SIM_CLOSE_CHANNEL_WITH_SESSION, 2, sessionInfo.sessionId, isEs10);
    return ok();
}

ScopedAStatus RadioSim::iccIoForApp(int32_t serial, const aidl::IccIo& iccIo) {
    mtkLogD(LOG_TAG, "iccIoForApp: slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_SIM_IO);
    if (pRI == NULL) {
        return ok();
    }

    RIL_SIM_IO_v6 rilIccIo = {};
    rilIccIo.command = iccIo.command;
    rilIccIo.fileid = iccIo.fileId;
    if (!copyAidlStringToRil(&rilIccIo.path, iccIo.path, pRI)) {
        return ok();
    }

    rilIccIo.p1 = iccIo.p1;
    rilIccIo.p2 = iccIo.p2;
    rilIccIo.p3 = iccIo.p3;

    if (!copyAidlStringToRil(&rilIccIo.data, iccIo.data, pRI)) {
        memsetAndFreeStrings(1, rilIccIo.path);
        return ok();
    }

    if (!copyAidlStringToRil(&rilIccIo.pin2, iccIo.pin2, pRI)) {
        memsetAndFreeStrings(2, rilIccIo.path, rilIccIo.data);
        return ok();
    }

    if (!copyAidlStringToRil(&rilIccIo.aidPtr, iccIo.aid, pRI)) {
        memsetAndFreeStrings(3, rilIccIo.path, rilIccIo.data, rilIccIo.pin2);
        return ok();
    }

    CALL_ONREQUEST(RIL_REQUEST_SIM_IO, &rilIccIo, sizeof(rilIccIo), pRI, mSlotId);

    memsetAndFreeStrings(4, rilIccIo.path, rilIccIo.data, rilIccIo.pin2, rilIccIo.aidPtr);
    return ok();
}

ScopedAStatus RadioSim::iccOpenLogicalChannel(int32_t serial, const std::string& aid, int32_t p2) {
    mtkLogD(LOG_TAG, "[%d] iccOpenLogicalChannel: serial %d", mSlotId, serial);
    if (s_vendorFunctions->version < 15) {
        rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_SIM_OPEN_CHANNEL, aid.c_str());
    } else {
        RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
                RIL_REQUEST_SIM_OPEN_CHANNEL);
        if (pRI == NULL) {
            return ok();
        }

        RIL_OpenChannelParams params = {};
        params.p2 = p2;

        if (!copyAidlStringToRil(&params.aidPtr, aid, pRI)) {
            return ok();
        }

        CALL_ONREQUEST(pRI->pCI->requestNumber, &params, sizeof(params), pRI, mSlotId);
        memsetAndFreeStrings(1, params.aidPtr);
    }
    return ok();
}

bool dispatchIccApdu(int serial, int slotId, int request,  const aidl::SimApdu& message) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_SIM_APDU apdu = {};

    apdu.sessionid = message.sessionId;
    apdu.cla = message.cla;
    apdu.instruction = message.instruction;
    apdu.p1 = message.p1;
    apdu.p2 = message.p2;
    apdu.p3 = message.p3;
    apdu.isEs10 = message.isEs10;

    if (!copyAidlStringToRil(&apdu.data, message.data, pRI)) {
        return false;
    }

    CALL_ONREQUEST(request, &apdu, sizeof(apdu), pRI, slotId);

    memsetAndFreeStrings(1, apdu.data);

    return true;
}

ScopedAStatus RadioSim::iccTransmitApduBasicChannel(int32_t serial, const aidl::SimApdu& message) {
    mtkLogD(LOG_TAG, "iccTransmitApduBasicChannel: slotid:%d, serial %d", mSlotId, serial);
    dispatchIccApdu(serial, mSlotId, RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC, message);
    return ok();
}

ScopedAStatus RadioSim::iccTransmitApduLogicalChannel(int32_t serial,
                                                      const aidl::SimApdu& message) {
    mtkLogD(LOG_TAG, "iccTransmitApduLogicalChannel: slotid:%d, serial %d", mSlotId, serial);
    dispatchIccApdu(serial, mSlotId, RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL, message);
    return ok();
}

ScopedAStatus RadioSim::reportStkServiceIsRunning(int32_t serial) {
    mtkLogD(LOG_TAG, "reportStkServiceIsRunning: slotid:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING);
    return ok();
}

ScopedAStatus RadioSim::requestIccSimAuthentication(  //
        int32_t serial, int32_t authContext, const std::string& authData, const std::string& aid) {
    mtkLogD(LOG_TAG, "requestIccSimAuthentication: slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_SIM_AUTHENTICATION);
    if (pRI == NULL) {
        return ok();
    }

    RIL_SimAuthentication pf = {};

    pf.authContext = authContext;

    if (!copyAidlStringToRil(&pf.authData, authData, pRI)) {
        return ok();
    }

    if (!copyAidlStringToRil(&pf.aid, aid, pRI)) {
        memsetAndFreeStrings(1, pf.authData);
        return ok();
    }

    CALL_ONREQUEST(pRI->pCI->requestNumber, &pf, sizeof(pf), pRI, mSlotId);

    memsetAndFreeStrings(2, pf.authData, pf.aid);

    return ok();
}

ScopedAStatus RadioSim::responseAcknowledgement() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus RadioSim::sendEnvelope(int32_t serial, const std::string& command) {
    mtkLogD(LOG_TAG, "sendEnvelope: serial %d", serial);
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND,
            command);
    return ok();
}

ScopedAStatus RadioSim::sendEnvelopeWithStatus(int32_t serial, const std::string& contents) {
    mtkLogD(LOG_TAG, "sendEnvelopeWithStatus: serial %d", serial);
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS,
            contents);
    return ok();
}

ScopedAStatus RadioSim::sendTerminalResponseToSim(int32_t serial,
                                                  const std::string& commandResponse) {
    mtkLogD(LOG_TAG, "sendTerminalResponseToSim: serial %d", serial);
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE,
            commandResponse);
    return ok();
}

ScopedAStatus RadioSim::setAllowedCarriers(  //
        int32_t serial, const aidl::CarrierRestrictions& carriers, aidl::SimLockMultiSimPolicy mp) {
    mtkLogI(LOG_TAG, "setAllowedCarriers: %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_ALLOWED_CARRIERS);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setAllowedCarriers: pRI request failed!");
        return ok();
    }

    RIL_CarrierRestrictionsWithPriority cr = {};
    RIL_Carrier *allowedCarriers = NULL;
    RIL_Carrier *excludedCarriers = NULL;

    cr.len_allowed_carriers = carriers.allowedCarriers.size();
    allowedCarriers = (RIL_Carrier *)calloc(cr.len_allowed_carriers, sizeof(RIL_Carrier));
    if (allowedCarriers == NULL) {
        mtkLogE(LOG_TAG, "setAllowedCarriers: allowedCarriers memory allocation failed  %s",
                requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return ok();
    }
    cr.allowed_carriers = allowedCarriers;

    cr.len_excluded_carriers = carriers.excludedCarriers.size();
    excludedCarriers = (RIL_Carrier *)calloc(cr.len_excluded_carriers, sizeof(RIL_Carrier));
    if (excludedCarriers == NULL) {
        mtkLogE(LOG_TAG, "setAllowedCarriers: excludedCarriers memory allocation failed %s",
                requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
#ifdef MEMSET_FREED
        memset(allowedCarriers, 0, cr.len_allowed_carriers * sizeof(RIL_Carrier));
#endif
        free(allowedCarriers);
        return ok();
    }
    cr.excluded_carriers = excludedCarriers;

    // Copy allowed carriers.
    for (int i = 0; i < cr.len_allowed_carriers; i++) {
        allowedCarriers[i].mcc = (char *)carriers.allowedCarriers[i].mcc.c_str();
        allowedCarriers[i].mnc = (char *)carriers.allowedCarriers[i].mnc.c_str();
        allowedCarriers[i].match_type = (RIL_CarrierMatchType) carriers.allowedCarriers[i].matchType;
        allowedCarriers[i].match_data = (char *)carriers.allowedCarriers[i].matchData.c_str();
    }

    // Copy exluded carriers.
    for (int i = 0; i < cr.len_excluded_carriers; i++) {
        excludedCarriers[i].mcc = (char *)carriers.excludedCarriers[i].mcc.c_str();
        excludedCarriers[i].mnc = (char *)carriers.excludedCarriers[i].mnc.c_str();
        excludedCarriers[i].match_type =
                (RIL_CarrierMatchType) carriers.excludedCarriers[i].matchType;
        excludedCarriers[i].match_data = (char *)carriers.excludedCarriers[i].matchData
                .c_str();
    }

    // Copy allowedCarriersPrioritized.
    cr.allowedCarriersPrioritized = carriers.allowedCarriersPrioritized;
    cr.status = (int)(carriers.status);

    // Copy SimLockMultiSimPolicy.
    cr.simLockMultiSimPolicy = (int) mp;

    CALL_ONREQUEST(pRI->pCI->requestNumber, &cr, sizeof(RIL_CarrierRestrictionsWithPriority),
            pRI, mSlotId);

#ifdef MEMSET_FREED
    memset(allowedCarriers, 0, cr.len_allowed_carriers * sizeof(RIL_Carrier));
    memset(excludedCarriers, 0, cr.len_excluded_carriers * sizeof(RIL_Carrier));
#endif
    free(allowedCarriers);
    free(excludedCarriers);
    return ok();
}

ScopedAStatus RadioSim::setCarrierInfoForImsiEncryption(
        int32_t serial, const aidl::ImsiEncryptionInfo& encryptionInfo) {

    mtkLogD(LOG_TAG, "setCarrierInfoForImsiEncryption: slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial,
            mSlotId, RIL_REQUEST_SET_CARRIER_INFO_IMSI_ENCRYPTION);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setCarrierInfoForImsiEncryption: pRI == NULL");
        return ok();
    }

    RIL_CarrierInfoForImsiEncryption imsiEncryption = {};

    if (!copyAidlStringToRil(&imsiEncryption.mnc, encryptionInfo.mnc, pRI)) {
        return ok();
    }
    if (!copyAidlStringToRil(&imsiEncryption.mcc, encryptionInfo.mcc, pRI)) {
        memsetAndFreeStrings(1, imsiEncryption.mnc);
        return ok();
    }
    if (!copyAidlStringToRil(&imsiEncryption.keyIdentifier, encryptionInfo.keyIdentifier, pRI)) {
        memsetAndFreeStrings(2, imsiEncryption.mnc, imsiEncryption.mcc);
        return ok();
    }
    imsiEncryption.carrierKeyLength = encryptionInfo.carrierKey.size();
    if (imsiEncryption.carrierKeyLength > 0) {
        imsiEncryption.carrierKey = new uint8_t[imsiEncryption.carrierKeyLength];
    } else {
        imsiEncryption.carrierKey = NULL;
    }
    memcpy(imsiEncryption.carrierKey, encryptionInfo.carrierKey.data(), imsiEncryption.carrierKeyLength);
    imsiEncryption.expirationTime = encryptionInfo.expirationTime;
    // CALL_ONREQUEST(pRI->pCI->requestNumber, &imsiEncryption,
    //        sizeof(RIL_CarrierInfoForImsiEncryption), pRI, mSlotId);
    sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    delete[](imsiEncryption.carrierKey);
    memsetAndFreeStrings(3, imsiEncryption.mnc, imsiEncryption.mcc, imsiEncryption.keyIdentifier);
    return ok();
}

ScopedAStatus RadioSim::setCdmaSubscriptionSource(int32_t serial,
                                                  aidl::CdmaSubscriptionSource cdmaSub) {
    mtkLogD(LOG_TAG, "setCdmaSubscriptionSource: slotid:%d, serial %d", mSlotId, serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE, 1, cdmaSub);
    return ok();
}

ScopedAStatus RadioSim::setFacilityLockForApp(  //
        int32_t serial, const std::string& facility, bool lockState, const std::string& password,
        int32_t serviceClass, const std::string& appId) {
    mtkLogD(LOG_TAG, "setFacilityLockForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_SET_FACILITY_LOCK, true,
            5, facility.c_str(), lockState ? "1" : "0", password.c_str(),
            (std::to_string(serviceClass)).c_str(), appId.c_str() );
    return ok();
}

ScopedAStatus RadioSim::setResponseFunctions(
        const std::shared_ptr<aidl::IRadioSimResponse>& simResponse,
        const std::shared_ptr<aidl::IRadioSimIndication>& simIndication) {
    mtkLogD(LOG_TAG, "aosp sim setResponseFunctions, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp sim setResponseFunctions: got lock %d", mSlotId);

    radioSimServiceImpl[mSlotId].simResponse = simResponse;
    radioSimServiceImpl[mSlotId].simIndication = simIndication;
    plusCounterRadioNumOne(mSlotId);

    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        rilAidlUtils::setAospResponseNumberToOne(rilAidlUtils::SIM_INDEX, mSlotId);
    }

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp sim setResponseFunctions: release lock %d", mSlotId);
    // client is connected. Send initial indications.
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        if (rilAidlUtils::checkIfSetAllAospResponseDone(mSlotId) == 1) {
            if(client == android::CLIENT_TBOX) {
                android::onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::TBOX_RADIO_AOSP);
            } else {
                android::onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::RADIO_AOSP);
            }
        }
    }
    return ok();
}

ScopedAStatus RadioSim::setSimCardPower(int32_t serial, aidl::CardPowerState powerUp) {
    mtkLogD(LOG_TAG, "setSimCardPower: slotid:%d, serial %d", mSlotId, serial);

    if (powerUp == aidl::CardPowerState::POWER_UP) {
        dispatchInts(serial, mSlotId, RIL_REQUEST_SET_SIM_CARD_POWER, 1, 4);
    } else if (powerUp == aidl::CardPowerState::POWER_DOWN) {
        dispatchInts(serial, mSlotId, RIL_REQUEST_SET_SIM_CARD_POWER, 1, 3);
    } else {
        dispatchInts(serial, mSlotId, RIL_REQUEST_SET_SIM_CARD_POWER, 1, (int) powerUp);
    }
    return ok();
}

ScopedAStatus RadioSim::setUiccSubscription(int32_t serial, const aidl::SelectUiccSub& uiccSub) {
    mtkLogD(LOG_TAG, "setUiccSubscription: slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_UICC_SUBSCRIPTION);

    if (pRI == NULL) {
        return ok();
    }

    RIL_SelectUiccSub rilUiccSub = {};

    rilUiccSub.slot = uiccSub.slot;
    rilUiccSub.app_index = uiccSub.appIndex;
    rilUiccSub.sub_type = (RIL_SubscriptionType) uiccSub.subType;
    rilUiccSub.act_status = (RIL_UiccSubActStatus) uiccSub.actStatus;

    CALL_ONREQUEST(pRI->pCI->requestNumber, &rilUiccSub, sizeof(rilUiccSub), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioSim::supplyIccPin2ForApp(int32_t serial, const std::string& pin2,
                                            const std::string& aid) {
    mtkLogD(LOG_TAG, "supplyIccPin2ForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_ENTER_SIM_PIN2,
            true, 2, pin2.c_str(), aid.c_str());
    return ok();
}

ScopedAStatus RadioSim::supplyIccPinForApp(int32_t serial, const std::string& pin,
                                           const std::string& aid) {
    mtkLogD(LOG_TAG, "supplyIccPinForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_ENTER_SIM_PIN,
            true, 2, pin.c_str(), aid.c_str());
    return ok();
}

ScopedAStatus RadioSim::supplyIccPuk2ForApp(int32_t serial, const std::string& puk2,
                                            const std::string& pin2, const std::string& aid) {
    mtkLogD(LOG_TAG, "supplyIccPuk2ForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_ENTER_SIM_PUK2,
            true, 3, puk2.c_str(), pin2.c_str(), aid.c_str());
    return ok();
}

ScopedAStatus RadioSim::supplyIccPukForApp(int32_t serial, const std::string& puk,
                                           const std::string& pin, const std::string& aid) {
    mtkLogD(LOG_TAG, "supplyIccPukForApp: slotid:%d, serial %d", mSlotId, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_ENTER_SIM_PUK,
            true, 3, puk.c_str(), pin.c_str(), aid.c_str());
    return ok();
}

ScopedAStatus RadioSim::supplySimDepersonalization(int32_t serial, aidl::PersoSubstate pss,
                                                   const std::string& controlKey) {
     mtkLogD(LOG_TAG, "supplySimDepersonalization: serial %d, slotId:%d", serial, mSlotId);
     rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_ENTER_SIM_DEPERSONALIZATION, true,
            2, controlKey.c_str(), std::to_string((int)pss).c_str());
    return ok();
}


bool dispatchAidlPhbEntryExtByModem(int serial, int slotId, int request,
        const aidl::PhonebookRecordInfo& recordInfo) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_PHB_ENTRY pbe;
    pbe.index = recordInfo.recordId;

    mtkLogI(LOG_TAG, "dispatchAidlPhbEntryExtByModem name:%s, len:%d, number:%s",
            recordInfo.name.c_str(), strlen(recordInfo.name.c_str()), recordInfo.number.c_str());

    for (int i = 0; i < strlen(recordInfo.name.c_str()); i++) {
        mtkLogI(LOG_TAG, "dispatchAidlPhbEntryExtByModem name[%d]:%x", i, recordInfo.name.c_str()[i]);
    }

    if (!rilAidlUtils::copyAidlStringToRil(&pbe.text, recordInfo.name, pRI)) {
        return false;
    }

    if (!rilAidlUtils::copyAidlStringToRil(&pbe.number, recordInfo.number, pRI)) {
        memsetAndFreeStrings(1, pbe.text);
        return false;
    }

    int emailCount = recordInfo.emails.size();

    if (emailCount >= 1) {
        if (!rilAidlUtils::copyAidlStringToRil(&pbe.email, recordInfo.emails[0], pRI)) {
            memsetAndFreeStrings(1, pbe.number);
            memsetAndFreeStrings(1, pbe.text);
            return false;
        }
    }

    int anrCount = recordInfo.additionalNumbers.size();

    if (anrCount >= 1) {
        if (!rilAidlUtils::copyAidlStringToRil(&pbe.adnumber, recordInfo.additionalNumbers[0], pRI)) {
            memsetAndFreeStrings(1, pbe.number);
            memsetAndFreeStrings(1, pbe.text);
            if (emailCount >= 1) {
                memsetAndFreeStrings(1, pbe.email);
            }
            return false;
        }
    }

#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(request, &pbe, sizeof(pbe), pRI, pRI->socket_id);
#else
    s_vendorFunctions->onRequest(request, &pbe, sizeof(pbe), pRI);
#endif

    memsetAndFreeStrings(1, pbe.number);
    memsetAndFreeStrings(1, pbe.text);
    if (emailCount >= 1) {
        memsetAndFreeStrings(1, pbe.email);
    }
    if (anrCount >= 1) {
        memsetAndFreeStrings(1, pbe.adnumber);
    }
    return true;
}


ScopedAStatus RadioSim::updateSimPhonebookRecords(int32_t serial,
                                                  const aidl::PhonebookRecordInfo& recordInfo) {
    mtkLogD(LOG_TAG, "updateSimPhonebookRecords: slotId:%d, serial %d", mSlotId, serial);
    dispatchAidlPhbEntryExtByModem(serial, mSlotId,
            RIL_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS, recordInfo);
    return ok();
}

}  // namespace android::hardware::radio::sim

/////////mtk radio
// MTK-START: SIM
int mtkRadioExSim::onCardDetectedInd(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onCardDetectedInd: slotId %d", slotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onCardDetectedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onCardDetectedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onCardDetectedInd == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onCardDetectedInd: tboxSlotId %d", tboxSlotId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onCardDetectedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onCardDetectedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onCardDetectedInd == NULL", tboxSlotId);
    }
    return 0;
}

/// M: SIM: Iccid changed indication
int mtkRadioExSim::iccidChangedInd(unsigned int slotId, int indicationType, int token,
                    RIL_Errno err, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "iccidChangedInd: Invalid response: NULL");
            return 0;
        }
        mtkLogD(LOG_TAG, "iccidChangedInd[%d]: %.3s", slotId, (char*) response);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->iccidChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "iccidChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].iccidChangedInd == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "tbox-iccidChangedInd: Invalid response: NULL");
            return 0;
        }
        mtkLogD(LOG_TAG, "tbox-iccidChangedInd[%d]: %.3s", tboxSlotId, (char*) response);
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->iccidChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-iccidChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].iccidChangedInd == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExSim::simTypeInfoChangedInd(unsigned int slotId, int indicationType, int token,
                    RIL_Errno err, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "simTypeInfoChangedInd: Invalid response: NULL");
            return 0;
        }
        mtkLogD(LOG_TAG, "simTypeInfoChangedInd, slotId:%d", slotId);
        RIL_SimTypeInfo *simTypeInfo = (RIL_SimTypeInfo *) response;
        aidl_mtkradioex_sim::SimTypeInfo typeInfo = {};
        typeInfo.support = simTypeInfo->isSupport;
        typeInfo.simType = simTypeInfo->simType;
        typeInfo.source = simTypeInfo->actionSource;
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->simTypeInfoChangedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                typeInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simTypeInfoChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].simTypeInfoChangedInd == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "tbox-simTypeInfoChangedInd: Invalid response: NULL");
            return 0;
        }
        mtkLogD(LOG_TAG, "tbox-simTypeInfoChangedInd, tboxSlotId:%d", tboxSlotId);
        RIL_SimTypeInfo *simTypeInfo = (RIL_SimTypeInfo *) response;
        aidl_mtkradioex_sim::SimTypeInfo typeInfo = {};
        typeInfo.support = simTypeInfo->isSupport;
        typeInfo.simType = simTypeInfo->simType;
        typeInfo.source = simTypeInfo->actionSource;
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->simTypeInfoChangedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                typeInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-simTypeInfoChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].simTypeInfoChangedInd == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExSim::getATRResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    if (clientId == android::CLIENT_SE) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioSimServiceImpl[realSlotId].mtkSeResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ndk::ScopedAStatus ret = radioSimServiceImpl[realSlotId].mtkSeResponse->getATRResponse(
                    responseInfo,
                    convertCharPtrToStdString((char *) response));
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getATRResponse: invalid ret response");
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::SE_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].mtkSeResponse == NULL", realSlotId);
        }
        return 0;
    }

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->getATRResponse(
                responseInfo,
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getATRResponse: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getATRResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::getIccidResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                    int serial, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->getIccidResponse(
                responseInfo, convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getIccidResponse: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getIccidResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::getSimTypeInfoResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                    int serial, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getSimTypeInfoResponse: invalid response");
            aidl_mtkradioex_sim::SimTypeInfo typeInfo;
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->getSimTypeInfoResponse(
                    responseInfo, typeInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getSimTypeInfoResponse: invalid ret response");
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            RIL_SimTypeInfo *simTypeInfo = ((RIL_SimTypeInfo*) response);
            aidl_mtkradioex_sim::SimTypeInfo typeInfo = {};
            typeInfo.support = simTypeInfo->isSupport;
            typeInfo.simType = simTypeInfo->simType;
            typeInfo.source = simTypeInfo->actionSource;
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->getSimTypeInfoResponse(
                    responseInfo, typeInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getSimTypeInfoResponse: invalid ret response");
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getSimTypeInfoResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::switchSimTypeResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                    int serial, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->switchSimTypeResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "switchSimTypeResponse: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].switchSimTypeResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::setSimPowerResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->setSimPowerResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSimPowerResponse: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].setSimPowerResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::onVirtualSimStatusChanged(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        mtkLogD(LOG_TAG, "onVirtualSimStatusChanged");
        int32_t simInserted = ((int32_t *) response)[0];
        if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onVirtualSimStatusChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), simInserted);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onVirtualSimStatusChanged: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
            ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onVirtualSimStatusChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), simInserted);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onVirtualSimStatusChanged: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "onVirtualSimStatusChanged: mtkRadioExService[%d]->mRadioIndicationMtk == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::onImeiLock(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onImeiLock");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onImeiLock(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onImeiLock: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onImeiLock == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onImeiLock");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onImeiLock(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onImeiLock: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onImeiLock == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExSim::onImsiRefreshDone(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onImsiRefreshDone");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onImsiRefreshDone(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onImsiRefreshDone: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onImsiRefreshDone == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onImsiRefreshDone");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onImsiRefreshDone(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onImsiRefreshDone: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onImsiRefreshDone == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExSim::getIccCardStatusResponse_1_2(unsigned int slotId, android::ClientId clientId,
                    int indicationType, int token, RIL_Errno err, const void *response,
                    size_t responseLen) {
    return 0;
}
// MTK-END

// MTK-START: SIM HOT SWAP / SIM RECOVERY
int mtkRadioExSim::onSimPlugIn(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimPlugIn");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_PLUG_IN,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimPlugIn: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onSimPlugIn == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "tbox-onSimPlugIn");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_PLUG_IN,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onSimPlugIn: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onSimPlugIn == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExSim::onSimPlugOut(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimPlugOut");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_PLUG_OUT,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimPlugOut: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onSimPlugOut == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "tbox-onSimPlugOut");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_PLUG_OUT,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onSimPlugOut: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onSimPlugOut == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExSim::onSimMissing(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimMissing");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_MISSING,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimMissing: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onSimMissing == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimMissing");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_MISSING,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onSimMissing: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onSimMissing == NULL", tboxSlotId);
    }

    return 0;
}

int mtkRadioExSim::onSimRecovery(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimRecovery");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_RECOVERY,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimRecovery: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onSimRecovery == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimRecovery");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_RECOVERY,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onSimRecovery: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onSimRecovery == NULL", tboxSlotId);
    }
    return 0;
}
// MTK-END

int mtkRadioExSim::activateUiccCardRsp(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
         aidl_radio::RadioResponseInfo responseInfo = {};
        int simPowerOnOffResponse
                = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->activateUiccCardRsp(
                responseInfo, simPowerOnOffResponse);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "activateUiccCardRsp: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].activateUiccCardRsp == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::deactivateUiccCardRsp(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int simPowerOnOffResponse
                = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->deactivateUiccCardRsp(
                responseInfo, simPowerOnOffResponse);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "deactivateUiccCardRsp: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].deactivateUiccCardRsp == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::getCurrentUiccCardProvisioningStatusRsp(unsigned int slotId,
                        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int simPowerOnOffResponse
                = simResponseIntOrEmpty(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->getCurrentUiccCardProvisioningStatusRsp(
                responseInfo, simPowerOnOffResponse);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCurrentUiccCardProvisioningStatusRsp: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].getCurrentUiccCardProvisioningStatusRsp == NULL", slotId);
    }
    return 0;
}

// MTK-START: SIM POWER
int mtkRadioExSim::onSimPowerChangedInd(unsigned int slotId, int indicationType, int token,
                    RIL_Errno e, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "onSimPowerChangedInd: Invalid response: NULL");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);

        mtkLogD(LOG_TAG, "onSimPowerChangedInd responselen: %zu, sizeof(int): %lu, numInt: %d",
                responseLen, (unsigned long)sizeof(int), numInts);

        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onSimPowerChangedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimPowerChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onSimPowerChangedInd == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "onSimPowerChangedInd: Invalid response: NULL");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);

        mtkLogD(LOG_TAG, "onSimPowerChangedInd responselen: %zu, sizeof(int): %lu, numInt: %d",
                responseLen, (unsigned long)sizeof(int), numInts);

        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onSimPowerChangedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onSimPowerChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onSimPowerChangedInd == NULL", tboxSlotId);
    }
    return 0;
}
// MTK-END

// MTK-START: SIM COMMON SLOT
int mtkRadioExSim::onSimTrayPlugIn(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimTrayPlugIn");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_TRAY_PLUG_IN,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimTrayPlugIn: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onSimTrayPlugIn == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimTrayPlugIn");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_TRAY_PLUG_IN,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onSimTrayPlugIn: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onSimTrayPlugIn == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExSim::onSimCommonSlotNoChanged(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimCommonSlotNoChanged");
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_COMMONSLOT_NO_CHANGED,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimCommonSlotNoChanged: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "radioSimServiceImpl[%d].onSimCommonSlotNoChanged == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        mtkLogD(LOG_TAG, "onSimCommonSlotNoChanged");
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onSimHotSwapInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                (int32_t) SIM_HOTSWAP_COMMONSLOT_NO_CHANGED,
                std::string(""));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-onSimCommonSlotNoChanged: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioSimServiceImpl[%d].onSimCommonSlotNoChanged == NULL", tboxSlotId);
    }
    return 0;
}
// MTK-END

// External SIM [Start]
int mtkRadioExSim::vsimNotificationResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e, const void *response,
                    size_t responseLen) {
    mtkLogD(LOG_TAG, "vsimNotificationResponse: serial %d, error: %d", serial, e);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        aidl_mtkradioex_sim::VsimEvent params = {};
        if (response == NULL || responseLen != sizeof(RIL_VsimEvent)) {
            mtkLogE(LOG_TAG, "vsimNotificationResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            RIL_VsimEvent *p_cur = ((RIL_VsimEvent *) response);
            params.transactionId = p_cur->transaction_id;
            params.eventId = p_cur->eventId;
            params.simType = p_cur->sim_type;
        }
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->vsimNotificationResponse(
                responseInfo, params);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vsimNotificationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }

    } else {
        mtkLogE(LOG_TAG, "vsimNotificationResponse: mtkRadioExService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::vsimOperationResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e, const void *response,
                    size_t responseLen) {
    mtkLogD(LOG_TAG, "vsimOperationResponse: serial %d", serial);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->vsimOperationResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vsimOperationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "vsimOperationResponse: mtkRadioExService[%d]->mRadioResponseMtk == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::onVsimEventIndication(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responselen) {
    mtkLogD(LOG_TAG, "onVsimEventIndication: indicationType %d", indicationType);

    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "onVsimEventIndication: invalid response");
            return 0;
        }

        aidl_mtkradioex_sim::VsimOperationEvent event = {};
        RIL_VsimOperationEvent *response_data = (RIL_VsimOperationEvent *)response;
        event.transactionId = response_data->transaction_id;
        event.eventId = response_data->eventId;
        event.result = response_data->result;
        event.dataLength = response_data->data_length;
        event.data = convertCharPtrToStdString(response_data->data);

        //mtkLogD(LOG_TAG, "onVsimEventIndication: id=%d, data_length=%d, data=%s", event.eventId, response_data->data_length, response_data->data);

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onVsimEventIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), event);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onVsimEventIndication: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "onVsimEventIndication: mtkRadioExService[%d]->mRadioIndicationMtk == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "onVsimEventIndication: invalid response");
            return 0;
        }

        aidl_mtkradioex_sim::VsimOperationEvent event = {};
        RIL_VsimOperationEvent *response_data = (RIL_VsimOperationEvent *)response;
        event.transactionId = response_data->transaction_id;
        event.eventId = response_data->eventId;
        event.result = response_data->result;
        event.dataLength = response_data->data_length;
        event.data = convertCharPtrToStdString(response_data->data);

        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onVsimEventIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), event);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onVsimEventIndication: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "onVsimEventIndication: mtkRadioExService[%d]->mRadioIndicationMtk == NULL",
                tboxSlotId);
    }
    return 0;
}
// External SIM [End]

// PHB START
int mtkRadioExSim::queryPhbStorageInfoResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "queryPhbStorageInfoResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> storageInfo;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "queryPhbStorageInfoResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            storageInfo.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                storageInfo[i] = ((int32_t) pInt[i]);
            }
        }
        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->queryPhbStorageInfoResponse(responseInfo,
                storageInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "queryPhbStorageInfoResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "queryPhbStorageInfoResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::writePhbEntryResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "writePhbEntryResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->writePhbEntryResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "writePhbEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "writePhbEntryResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }
    return 0;
}

void convertRilPhbEntryStructureToAidl(const void *response, size_t responseLen,
        std::vector<aidl_mtkradioex_sim::PhbEntryStructure>& resultList) {
    int num = responseLen / sizeof(RIL_PhbEntryStructure*);
    RIL_PhbEntryStructure **phbEntryResponse = (RIL_PhbEntryStructure **) response;
    resultList.resize(num);
    for (int i = 0; i < num; i++) {
        resultList[i].type = phbEntryResponse[i]->type;
        resultList[i].index = phbEntryResponse[i]->index;
        resultList[i].number = convertCharPtrToStdString(phbEntryResponse[i]->number);
        resultList[i].ton = phbEntryResponse[i]->ton;
        resultList[i].alphaId = convertCharPtrToStdString(phbEntryResponse[i]->alphaId);
    }
}

int mtkRadioExSim::readPhbEntryResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readPhbEntryResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl_mtkradioex_sim::PhbEntryStructure> result;

        if (response == NULL || responseLen % sizeof(RIL_PhbEntryStructure*) != 0) {
            mtkLogE(LOG_TAG, "readPhbEntryResponse: invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            convertRilPhbEntryStructureToAidl(response, responseLen, result);
        }

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->readPhbEntryResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readPhbEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readPhbEntryResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExSim::queryUPBCapabilityResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "queryUPBCapabilityResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> upbCapability;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "queryUPBCapabilityResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            upbCapability.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                upbCapability[i] = ((int32_t) pInt[i]);
            }
        }
        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->queryUPBCapabilityResponse(responseInfo,
                upbCapability);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "queryUPBCapabilityResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "queryUPBCapabilityResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::editUPBEntryResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "editUPBEntryResponse: serial:%d, clientId:%d, slotId:%d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->editUPBEntryResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "editUPBEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "editUPBEntryResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::deleteUPBEntryResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "deleteUPBEntryResponse: serial %d, clientId:%d, slotId:%d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->deleteUPBEntryResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "deleteUPBEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "deleteUPBEntryResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::readUPBGasListResponse(unsigned int slotId, android::ClientId clientId,
                    int responseType, int serial, RIL_Errno e,
                    const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readUPBGasListResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<std::string> gasList;
        int numStrings = responseLen / sizeof(char*);
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "readUPBGasListResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            char **pString = (char **) response;
            gasList.resize(numStrings);
            for (int i = 0; i < numStrings; i++) {
                gasList[i] = convertCharPtrToStdString(pString[i]);
            }
        }
        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->readUPBGasListResponse(responseInfo,
                gasList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readUPBGasListResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readUPBGasListResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExSim::readUPBGrpEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readUPBGrpEntryResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> grpEntries;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "readUPBGrpEntryResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            grpEntries.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                grpEntries[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->readUPBGrpEntryResponse(responseInfo,
                grpEntries);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readUPBGrpEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readUPBGrpEntryResponse: mtkRadioExService[%d]->mRadioResponseMtk == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::writeUPBGrpEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "writeUPBGrpEntryResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->writeUPBGrpEntryResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "writeUPBGrpEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "writeUPBGrpEntryResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExSim::getPhoneBookStringsLengthResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getPhoneBookStringsLengthResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> stringLengthInfo;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "getPhoneBookStringsLengthResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            stringLengthInfo.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                stringLengthInfo[i] = ((int32_t) pInt[i]);
            }
        }
        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->getPhoneBookStringsLengthResponse(responseInfo,
                stringLengthInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getPhoneBookStringsLengthResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getPhoneBookStringsLengthResponse: radioSimServiceImpl[%d]->mtkSimResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExSim::getPhoneBookMemStorageResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getPhoneBookMemStorageResponse: serial %d, clientId:%d, slotId:%d",
            serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        aidl_mtkradioex_sim::PhbMemStorageResponse phbMemStorage;
        if (response == NULL || responseLen != sizeof(RIL_PHB_MEM_STORAGE_RESPONSE)) {
            mtkLogE(LOG_TAG, "getPhoneBookMemStorageResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            RIL_PHB_MEM_STORAGE_RESPONSE *resp = (RIL_PHB_MEM_STORAGE_RESPONSE *)response;
            phbMemStorage.storage = convertCharPtrToStdString(resp->storage);
            phbMemStorage.used = resp->used;
            phbMemStorage.total = resp->total;
        }

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->getPhoneBookMemStorageResponse(responseInfo,
                phbMemStorage);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getPhoneBookMemStorageResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getPhoneBookMemStorageResponse: radioSimServiceImpl[%d]->mtkSimResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExSim::setPhoneBookMemStorageResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setPhoneBookMemStorageResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->setPhoneBookMemStorageResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setPhoneBookMemStorageResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setPhoneBookMemStorageResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }

    return 0;
}

void convertRilPhbEntryExtStrucutreToAidl(const void *response, size_t responseLen,
        std::vector<aidl_mtkradioex_sim::PhbEntryExt>& resultList) {
    int num = responseLen / sizeof(RIL_PHB_ENTRY *);

    RIL_PHB_ENTRY **phbEntryExtResponse = (RIL_PHB_ENTRY **) response;
    resultList.resize(num);
    for (int i = 0; i < num; i++) {
        resultList[i].index = phbEntryExtResponse[i]->index;
        resultList[i].number = convertCharPtrToStdString(phbEntryExtResponse[i]->number);
        resultList[i].type = phbEntryExtResponse[i]->type;
        resultList[i].text = convertCharPtrToStdString(phbEntryExtResponse[i]->text);
        resultList[i].hidden = phbEntryExtResponse[i]->hidden;
        resultList[i].group = convertCharPtrToStdString(phbEntryExtResponse[i]->group);
        resultList[i].adnumber = convertCharPtrToStdString(phbEntryExtResponse[i]->adnumber);
        resultList[i].adtype = phbEntryExtResponse[i]->adtype;
        resultList[i].secondtext = convertCharPtrToStdString(phbEntryExtResponse[i]->secondtext);
        resultList[i].email = convertCharPtrToStdString(phbEntryExtResponse[i]->email);
    }
}

int mtkRadioExSim::readPhoneBookEntryExtResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readPhoneBookEntryExtResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl_mtkradioex_sim::PhbEntryExt> result;
        if (response == NULL || responseLen % sizeof(RIL_PHB_ENTRY *) != 0) {
            mtkLogE(LOG_TAG, "readPhoneBookEntryExtResponse: invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            convertRilPhbEntryExtStrucutreToAidl(response, responseLen, result);
        }

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->readPhoneBookEntryExtResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readPhoneBookEntryExtResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readPhoneBookEntryExtResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::writePhoneBookEntryExtResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "writePhoneBookEntryExtResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->writePhoneBookEntryExtResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "writePhoneBookEntryExtResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "writePhoneBookEntryExtResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::queryUPBAvailableResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "queryUPBAvailableResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> upbAvailable;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "queryUPBAvailableResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            upbAvailable.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                upbAvailable[i] = ((int32_t) pInt[i]);
            }
        }
        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->queryUPBAvailableResponse(responseInfo,
                upbAvailable);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "queryUPBAvailableResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "queryUPBAvailableResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExSim::readUPBEmailEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readUPBEmailEntryResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->readUPBEmailEntryResponse(responseInfo,
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readUPBEmailEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readUPBEmailEntryResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExSim::readUPBSneEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readUPBSneEntryResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->readUPBSneEntryResponse(responseInfo,
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readUPBSneEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readUPBSneEntryResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExSim::readUPBAnrEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readUPBAnrEntryResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl_mtkradioex_sim::PhbEntryStructure> result;
        if (response == NULL || responseLen % sizeof(RIL_PhbEntryStructure*) != 0) {
            mtkLogD(LOG_TAG, "readUPBAnrEntryResponse: invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            convertRilPhbEntryStructureToAidl(response, responseLen, result);
        }

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->readUPBAnrEntryResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readUPBAnrEntryResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readUPBAnrEntryResponse: mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::readUPBAasListResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "readUPBAasListResponse: serial %d, clientId %d, slotId %d", serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<std::string> aasList;
        int numStrings = responseLen / sizeof(char *);
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "readUPBAasListResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            char **pString = (char **) response;
            // aasList.resize(numStrings);
            for (int i = 0; i < numStrings; i++) {
                aasList.push_back(convertCharPtrToStdString(pString[i]));
            }
        }
        ndk::ScopedAStatus ret
                = radioSimServiceImpl[slotId].mtkSimResponse->readUPBAasListResponse(responseInfo,
                aasList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "readUPBAasListResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "readUPBAasListResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExSim::phbReadyNotificationInd(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "phbReadyNotificationInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "phbReadyNotificationInd, slotId:%d", slotId);
        int32_t isPhbReady = ((int32_t *) response)[0];
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->phbReadyNotification(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), isPhbReady);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "phbReadyNotificationInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "phbReadyNotificationInd: radioSimServiceImpl[%d].mtkSimIndication == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::setPhonebookReadyResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setPhonebookReadyResponse: serial %d, clientId %d, slotId %d",
            serial, clientId, slotId);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->setPhonebookReadyResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setPhonebookReadyResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setPhonebookReadyResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }
    return 0;
}
// PHB END

// / M: STK, only to build pass {
int mtkRadioExSim::bipProactiveCommandInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    return 0;
}

int mtkRadioExSim::onStkMenuResetInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen) {
    return 0;
}

int mtkRadioExSim::handleStkCallSetupRequestFromSimWithResCodeResponse(unsigned int slotId,
                        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "handleStkCallSetupRequestFromSimWithResCodeResponse: serial %d", serial);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->
                handleStkCallSetupRequestFromSimWithResCodeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "handleStkCallSetupRequestFromSimWithResCodeResponse:"
                    "slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "handleStkCallSetupRequestFromSimWithResCodeResponse:"
                "radioSimServiceImpl[%d]->mtkSimResponse == NULL", slotId);
    }

    return 0;
}
// / M: STK }

// MTK-START: SIM ME LOCK
int mtkRadioExSim::queryNetworkLockResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "queryNetworkLockResponse: serial %d", serial);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        RIL_SimMeLockCatInfo *pLockCatInfo = NULL;
        int category = -1, state = -1, retry_cnt = -1, autolock_cnt = -1,
                num_set = -1, total_set = -1, key_state = -1;

        if (response == NULL || responseLen != sizeof(RIL_SimMeLockCatInfo)) {
            mtkLogE(LOG_TAG, "queryNetworkLockResponse: Invalid response");
            if (e == RIL_E_SUCCESS) e = RIL_E_INVALID_RESPONSE;
        } else {
            pLockCatInfo = (RIL_SimMeLockCatInfo *) response;

            category = pLockCatInfo->catagory;
            state = pLockCatInfo->state;
            retry_cnt = pLockCatInfo->retry_cnt;
            autolock_cnt = pLockCatInfo->autolock_cnt;
            num_set = pLockCatInfo->num_set;
            total_set = pLockCatInfo->total_set;
            key_state = pLockCatInfo->key_state;
        }
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->queryNetworkLockResponse(
                responseInfo, category, state, retry_cnt, autolock_cnt, num_set,
                total_set, key_state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "queryNetworkLockResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "queryNetworkLockResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::setNetworkLockResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setNetworkLockResponse: serial %d", serial);
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->setNetworkLockResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setNetworkLockResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setNetworkLockResponse: radioSimServiceImpl[%d].mtkSimResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExSim::supplyDepersonalizationResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responseLen) {
    mtkLogD(LOG_TAG, "supplyDepersonalizationResponse: serial %d", serial);
    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int remain = rilAidlUtils::responseIntOrEmpty(responseInfo, serial, responseType, e,
                response, responseLen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->
                supplyDepersonalizationResponse(responseInfo, remain);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplyDepersonalizationResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "supplyDepersonalizationResponse: "
                "radioSimServiceImpl[%d].mtkSimResponse== NULL", slotId);
    }
    return 0;
}
// MTK-END

// MTK-START: SIM SLOT LOCK
int mtkRadioExSim::smlSlotLockInfoChangedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "smlSlotLockInfoChangedInd: invalid response!");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "smlSlotLockInfoChangedInd responselen: %zu, sizeof(int): %lu, numInt: %d",
                responseLen, (unsigned long)sizeof(int), numInts);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication
                ->smlSlotLockInfoChangedInd(convertIntToAidlRadioIndicationType(indicationType),
                        data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "smlSlotLockInfoChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
         }
    } else {
        mtkLogE(LOG_TAG, "smlSlotLockInfoChangedInd: mtkRadioExService[%d]/mRadioIndication is NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "smlSlotLockInfoChangedInd: invalid response!");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "tbox-smlSlotLockInfoChangedInd responselen: %zu, sizeof(int): %lu, numInt: %d",
                responseLen, (unsigned long)sizeof(int), numInts);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication
                ->smlSlotLockInfoChangedInd(convertIntToAidlRadioIndicationType(indicationType),
                        data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "tbox-smlSlotLockInfoChangedInd: invalid ret response");
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
         }
    } else {
        mtkLogE(LOG_TAG, "tbox-smlSlotLockInfoChangedInd: mtkRadioExService[%d]/mRadioIndication is NULL",
                tboxSlotId);
    }    
    return 0;
}

int mtkRadioExSim::supplyDeviceNetworkDepersonalizationResponse(unsigned int slotId,
                        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "supplyDeviceNetworkDepersonalizationResponse: serial %d", serial);

    if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int remain = rilAidlUtils::responseIntOrEmpty(responseInfo, serial, responseType, e,
                response, responseLen);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->
                supplyDeviceNetworkDepersonalizationResponse(responseInfo, remain);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplyDeviceNetworkDepersonalizationResponse: slotId %d, binder fail",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "supplyDeviceNetworkDepersonalizationResponse: "
                "radioSimServiceImpl[slotId].mtkSimResponse == NULL", slotId);
    }
    return 0;
}
// MTK-END

// SIM RSU
int mtkRadioExSim::onRsuSimLockEvent(unsigned int slotId, int indicationType, int token,
                        RIL_Errno e, const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "onRsuSimLockEvent: invalid response!");
            return 0;
        }
        int32_t eventId = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "[RSU-SIMLOCK] onRsuSimLockEvent: eventId %d", eventId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onRsuSimLockEvent(
                convertIntToAidlRadioIndicationType(indicationType), eventId);
         if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "onRsuSimLockEvent: invalid ret response");
             retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
         }

    } else {
        mtkLogE(LOG_TAG, "[RSU-SIMLOCK] onRsuSimLockEvent:"
                "radioSimServiceImpl[%d]->onRsuSimLockEvent == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "onRsuSimLockEvent: invalid response!");
            return 0;
        }
        int32_t eventId = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "[RSU-SIMLOCK] onRsuSimLockEvent: eventId %d", eventId);
        ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onRsuSimLockEvent(
                convertIntToAidlRadioIndicationType(indicationType), eventId);
         if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "onRsuSimLockEvent: invalid ret response");
             retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
         }

    } else {
        mtkLogE(LOG_TAG, "[RSU-SIMLOCK] onRsuSimLockEvent:"
                "radioSimServiceImpl[%d]->onRsuSimLockEvent == NULL", slotId);
    }
    return 0;
}

int mtkRadioExSim::sendRsuRequestResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendRsuRequestResponse: serial %d, clientId: %d", serial, clientId);

    if (clientId == android::CLIENT_RSU) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        mtkLogD(LOG_TAG, "sendRsuRequestResponse: CLIENT_RSU request, slotId=%d, realSlotId=%d",
                slotId, realSlotId);
        if (radioSimServiceImpl[realSlotId].mtkRsuResponse == NULL) {
            mtkLogE(LOG_TAG, "[RSU-SIMLOCK] sendRsuRequestResponse:"
                    "radioSimServiceImpl[%d].mtkRsuResponse == NULL", realSlotId);
            return 0;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        aidl_mtkradioex_rsu::RsuResponseInfo result = {};
        if (response == NULL || responseLen != sizeof(RIL_RsuResponse)) {
            mtkLogE(LOG_TAG, "sendRsuRequestResponse: response == NULL");
        } else {
            result.opId = ((RIL_RsuResponse*)(response))->opId;
            result.requestId = ((RIL_RsuResponse*)(response))->requestId;
            result.errCode = ((RIL_RsuResponse*)(response))->errCode;
            result.data = convertCharPtrToStdString(((RIL_RsuResponse*)(response))->data);
            result.time = ((RIL_RsuResponse*)(response))->time;
            result.version = ((RIL_RsuResponse*)(response))->version;
            result.status = ((RIL_RsuResponse*)(response))->status;
            result.reserveInt1 = ((RIL_RsuResponse*)(response))->reserveInt1;
            result.reserveInt2 = ((RIL_RsuResponse*)(response))->reserveInt2;
            result.reserveString1 =
                    convertCharPtrToStdString(((RIL_RsuResponse*)(response))->reserveString1);
        }

        ndk::ScopedAStatus ret = radioSimServiceImpl[realSlotId].mtkRsuResponse->
                        sendRsuResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendRsuRequestResponse: slotId %d, binder fail",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        if (radioSimServiceImpl[slotId].mtkSimResponse != NULL) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            populateAidlResponseInfo(responseInfo, serial, responseType, e);

            aidl_mtkradioex_rsu::RsuResponseInfo result = {};
            if (response == NULL || responseLen != sizeof(RIL_RsuResponse)) {
                mtkLogE(LOG_TAG, "sendRsuRequestResponse: response == NULL");
            } else {
                result.opId = ((RIL_RsuResponse*)(response))->opId;
                result.requestId = ((RIL_RsuResponse*)(response))->requestId;
                result.errCode = ((RIL_RsuResponse*)(response))->errCode;
                result.data = convertCharPtrToStdString(((RIL_RsuResponse*)(response))->data);
                result.time = ((RIL_RsuResponse*)(response))->time;
                result.version = ((RIL_RsuResponse*)(response))->version;
                result.status = ((RIL_RsuResponse*)(response))->status;
                result.reserveInt1 = ((RIL_RsuResponse*)(response))->reserveInt1;
                result.reserveInt2 = ((RIL_RsuResponse*)(response))->reserveInt2;
                result.reserveString1 =
                        convertCharPtrToStdString(((RIL_RsuResponse*)(response))->reserveString1);
            }

            ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimResponse->
                            sendRsuRequestResponse(responseInfo, result);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "sendRsuRequestResponse: slotId %d, binder fail",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "sendRsuRequestResponse: radioSimServiceImpl[%u].mtkSimResponse"
                    "==NULL", slotId);
        }
    }
    return 0;
}

int mtkRadioExSim::onRsuEvent(unsigned int slotId, int indicationType, int token, RIL_Errno e,
                        const void *response, size_t responseLen) {
    if (radioSimServiceImpl[slotId].mtkSimIndication != NULL) {
         // It's for 93 and later RILD now.
         mtkLogD(LOG_TAG,"[RSU-SIMLOCK] onRsuEvent");
         char** strings = (char**)response;
         ndk::ScopedAStatus ret = radioSimServiceImpl[slotId].mtkSimIndication->onRsuEvent(
                 convertIntToAidlRadioIndicationType(indicationType),
                 atoi(strings[0]), convertCharPtrToStdString(strings[1]));
         if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "onRsuEvent: invalid ret response");
             retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
         }
    } else {
         mtkLogE(LOG_TAG,"onRsuEvent: radioService[%u].mtkSimIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioSimServiceImpl[tboxSlotId].mtkSimIndication != NULL) {
         // It's for 93 and later RILD now.
         mtkLogD(LOG_TAG,"[RSU-SIMLOCK] onRsuEvent");
         char** strings = (char**)response;
         ndk::ScopedAStatus ret = radioSimServiceImpl[tboxSlotId].mtkSimIndication->onRsuEvent(
                 convertIntToAidlRadioIndicationType(indicationType),
                 atoi(strings[0]), convertCharPtrToStdString(strings[1]));
         if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "tbox_onRsuEvent: invalid ret response");
             retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
         }
    } else {
         mtkLogE(LOG_TAG,"onRsuEvent: tbox_radioService[%u].mtkSimIndication == NULL", tboxSlotId);
    }
    return 0;
}
// MTK-END

namespace vendor::mediatek::hardware::mtkradioex::sim {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::sim;
constexpr auto ok = &ScopedAStatus::ok;

MtkRadioSim::MtkRadioSim(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus MtkRadioSim::switchSimType(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "switchSimType: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_SWITCH_SIM_TYPE, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioSim::getIccid(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getIccid: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_SIM_GET_ICCID);
    return ok();
}

ScopedAStatus MtkRadioSim::getSimTypeInfo(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getSimTypeInfo: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_SIM_TYPE_INFO);
    return ok();
}


ScopedAStatus MtkRadioSim::activateUiccCard(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "activateUiccCard: slotid:%d, serial %d, clientId %d", mSlotId, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_ACTIVATE_UICC_CARD);
    return ok();
}

ScopedAStatus MtkRadioSim::deactivateUiccCard(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "deactivateUiccCard: slotid:%d, serial %d, clientId %d", mSlotId, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_DEACTIVATE_UICC_CARD);
    return ok();
}

ScopedAStatus MtkRadioSim::deleteUPBEntry(int32_t serial, int32_t entryType, int32_t adnIndex, int32_t entryIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "deleteUPBEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DELETE_UPB_ENTRY,
           3, entryType, adnIndex, entryIndex);
    return ok();
}

bool dispatchSimGeneralAuth(int serial, int slotId, int request, const aidl_mtkradioex_sim::SimAuthStructure& simAuth) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_SimAuthStructure rilSimAuth;
    rilSimAuth.sessionId= simAuth.sessionId;
    rilSimAuth.mode= simAuth.mode;

    if (!copyAidlStringToRil(&rilSimAuth.param1, simAuth.param1, pRI)) {
        return false;
    }

    if (!copyAidlStringToRil(&rilSimAuth.param2, simAuth.param2, pRI)) {
        memsetAndFreeStrings(1, rilSimAuth.param1);
        return false;
    }

    rilSimAuth.tag = simAuth.tag;

    #if defined(ANDROID_MULTI_SIM)
        s_vendorFunctions->onRequest(request, &rilSimAuth, sizeof(rilSimAuth), pRI,
                pRI->socket_id);
    #else
        s_vendorFunctions->onRequest(request, &rilSimAuth, sizeof(rilSimAuth), pRI);
    #endif

    memsetAndFreeStrings(1, rilSimAuth.param1);
    memsetAndFreeStrings(1, rilSimAuth.param2);

    return true;
}

ScopedAStatus MtkRadioSim::doGeneralSimAuthentication(int32_t serial, const aidl_mtkradioex_sim::SimAuthStructure& simAuth, int32_t clientId) {
    mtkLogD(LOG_TAG, "doGeneralSimAuthentication: slotid:%d, serial:%d, clientId:%d", mSlotId, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchSimGeneralAuth(serial, mSlotId, RIL_REQUEST_GENERAL_SIM_AUTH, simAuth);
    return ok();
}

ScopedAStatus MtkRadioSim::editUPBEntry(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "editUPBEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_EDIT_UPB_ENTRY, data);
    return ok();
}

ScopedAStatus MtkRadioSim::getATR(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getATR: slotid:%d, serial %d, clientId %d", mSlotId, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_SE) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SIM_GET_ATR);
    return ok();
}

ScopedAStatus MtkRadioSim::getCurrentUiccCardProvisioningStatus(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getCurrentUiccCardProvisioningStatus: slotid:%d, serial %d, clientId:%d",
            mSlotId, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_CURRENT_UICC_CARD_PROVISIONING_STATUS);
    return ok();
}

ScopedAStatus MtkRadioSim::getPhoneBookMemStorage(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getPhoneBookMemStorage: serial %d, clientId:%d, slotId:%d",
            serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_PHB_MEM_STORAGE);
    return ok();
}

ScopedAStatus MtkRadioSim::getPhoneBookStringsLength(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getPhoneBookStringsLength: serial:%d, clientId:%d, slotId:%d",
            serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_PHB_STRING_LENGTH);
    return ok();
}

ScopedAStatus MtkRadioSim::handleStkCallSetupRequestFromSimWithResCode(int32_t serial, int32_t resultCode, int32_t clientId) {
    mtkLogD(LOG_TAG, "handleStkCallSetupRequestFromSimWithResCode: serial:%d, clientId:%d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId,
            RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM_WITH_RESULT_CODE,1, resultCode);
    return ok();
}

ScopedAStatus MtkRadioSim::queryNetworkLock(int32_t serial, int32_t category, int32_t clientId) {
    mtkLogD(LOG_TAG, "queryNetworkLock: serial %d, client:%d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_QUERY_SIM_NETWORK_LOCK, 1, category);
    return ok();
}

ScopedAStatus MtkRadioSim::queryPhbStorageInfo(int32_t serial, int32_t type, int32_t clientId) {
    mtkLogD(LOG_TAG, "queryPhbStorageInfo: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_QUERY_PHB_STORAGE_INFO,
            1, type);
    return ok();
}

ScopedAStatus MtkRadioSim::queryUPBAvailable(int32_t serial, int32_t eftype, int32_t fileIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "queryUPBAvailable: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_QUERY_UPB_AVAILABLE,
            2, eftype, fileIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::readPhbEntry(int32_t serial, int32_t type, int32_t bIndex, int32_t eIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "readPhbEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_PHB_ENTRY,
            3, type, bIndex, eIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::readPhoneBookEntryExt(int32_t serial, int32_t index1, int32_t index2, int32_t clientId) {
    mtkLogD(LOG_TAG, "readPhoneBookEntryExt: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_PHB_ENTRY_EXT,
            2, index1, index2);
    return ok();
}

ScopedAStatus MtkRadioSim::readUPBAasList(int32_t serial, int32_t startIndex, int32_t endIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "readUPBAasList: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_UPB_AAS_LIST,
            2, startIndex, endIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::readUPBAnrEntry(int32_t serial, int32_t adnIndex, int32_t fileIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "readUPBAnrEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_ANR_ENTRY,
            2, adnIndex, fileIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::readUPBEmailEntry(int32_t serial, int32_t adnIndex, int32_t fileIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "readUPBEmailEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_EMAIL_ENTRY,
            2, adnIndex, fileIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::readUPBGasList(int32_t serial, int32_t startIndex, int32_t endIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "readUPBGasList: serial %d, clinetId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_UPB_GAS_LIST,
            2, startIndex, endIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::readUPBGrpEntry(int32_t serial, int32_t adnIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "readUPBGrpEntry: serial %d, clientId %d, slotId %d",
            serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_UPB_GRP,
            1, adnIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::readUPBSneEntry(int32_t serial, int32_t adnIndex, int32_t fileIndex, int32_t clientId) {
    mtkLogD(LOG_TAG, "readUPBSneEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_READ_SNE_ENTRY,
            2, adnIndex, fileIndex);
    return ok();
}

ScopedAStatus MtkRadioSim::sendRsuRequest(int32_t serial,
        const aidl_mtkradioex_rsu::RsuRequestInfo& rri,
        int32_t clientId) {
    mtkLogD(LOG_TAG, "[RSU-SIMLOCK] sendRsuRequest clientId %d", clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RSU) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    android::RequestInfo *pRI = android::addRequestToList(serial,
        clientId * MAX_SIM_COUNT + mSlotId, RIL_REQUEST_SML_RSU_REQUEST);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "[RSU-SIMLOCK] sendRsuRequest pRI is NULL");
        return ok();
    }

    RIL_RsuRequest r_rri = {};
    r_rri.opId = (int)rri.opId;
    r_rri.requestId = (int)rri.requestId;
    r_rri.requestType = (int)rri.requestType;
    r_rri.reserveInt1 = (int)rri.reserveInt1;
    r_rri.reserveInt2 = (int)rri.reserveInt2;

    if (!copyAidlStringToRil(&r_rri.data, rri.data, pRI, true)) {
        return ok();
    }
    if (!copyAidlStringToRil(&r_rri.reserveString1, rri.reserveString1, pRI, true)) {
        memsetAndFreeStrings(1, r_rri.data);
        return ok();
    }

    CALL_ONREQUEST(RIL_REQUEST_SML_RSU_REQUEST, &r_rri,
            sizeof(r_rri), pRI, pRI->socket_id);

    memsetAndFreeStrings(2, r_rri.data, r_rri.reserveString1);
    return ok();
}

// External SIM [START]
bool dispatchVsimEvent(int serial, int slotId, int request,
        uint32_t transactionId, uint32_t eventId, uint32_t simType) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_VsimEvent args;
    args.transaction_id = transactionId;
    args.eventId = eventId;
    args.sim_type = simType;

#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(request, &args, sizeof(args), pRI, pRI->socket_id);
#else
    s_vendorFunctions->onRequest(request, &args, sizeof(args), pRI);
#endif

    return true;
}

#define VSIM_OPERATION_DATA_MAX_LEN (512)
bool dispatchVsimOperationEvent(int serial, int slotId, int request,
        uint32_t transactionId, uint32_t eventId, int32_t result,
        int32_t dataLength, const std::vector<uint8_t>& data) {
    mtkLogD(LOG_TAG, "dispatchVsimOperationEvent: enter id=%d", eventId);
    if (dataLength < 0 || dataLength > VSIM_OPERATION_DATA_MAX_LEN || dataLength > data.size()) {
        mtkLogE(LOG_TAG, "dispatchVsimOperationEvent, invalid dataLength=%d, datasize=%zu.",
                dataLength, data.size());
        return false;
    }
    RIL_VsimOperationEvent args;

    memset (&args, 0, sizeof(args));

    // Transcation id
    args.transaction_id = transactionId;
    // Event id
    args.eventId = eventId;
    // Result
    args.result = result;
    // Data length
    args.data_length = dataLength;

    // Data array
    const uint8_t *uData = data.data();
    args.data= (char  *) calloc(1, (sizeof(char) * args.data_length * 2) + 1);
    if (args.data == NULL) {
        mtkLogE(LOG_TAG, "dispatchVsimOperationEvent: args.data is NULL.");
        return false;
    }
    memset(args.data, 0, ((sizeof(char) * args.data_length * 2) + 1));
    for (int i = 0; i < args.data_length; i++) {
        if (sprintf((args.data + (i*2)), "%02X", uData[i]) < 0) {
            mtkLogE(LOG_TAG, "dispatchVsimOperationEvent: sprintf < 0");
            free(args.data);
            return false;
        }
    }

    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "dispatchVsimOperationEvent: pRI is NULL.");
        free(args.data);
        return false;
    }

    //mtkLogD(LOG_TAG, "dispatchVsimOperationEvent: id=%d, data=%s", args.eventId, args.data);

#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(request, &args, sizeof(args), pRI, pRI->socket_id);
#else
    s_vendorFunctions->onRequest(request, &args, sizeof(args), pRI);
#endif

    free(args.data);

    return true;
}

ScopedAStatus MtkRadioSim::sendVsimNotification(int32_t serial, int32_t transactionId,
        int32_t eventId, int32_t simType, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendVsimNotification: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVsimEvent(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_VSIM_NOTIFICATION,
            transactionId, eventId, simType);
    return ok();
}

ScopedAStatus MtkRadioSim::sendVsimOperation(int32_t serial, int32_t transactionId, int32_t eventId,
        int32_t result, int32_t dataLength, const std::vector<uint8_t>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendVsimOperation: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVsimOperationEvent(serial, mSlotId + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_VSIM_OPERATION,
            transactionId, eventId, result, dataLength, data);
    return ok();
}
// External SIM [END]


ScopedAStatus MtkRadioSim::setNetworkLock(int32_t serial, int32_t category, int32_t lockop, const std::string& password, const std::string& data_imsi, const std::string& gid1,
        const std::string& gid2, int32_t clientId) {
    mtkLogD(LOG_TAG, "setNetworkLock: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_SET_SIM_NETWORK_LOCK,
            true,
            6,
            std::to_string((int) category).c_str(),
            std::to_string((int) lockop).c_str(),
            password.c_str(),
            data_imsi.c_str(),
            gid1.c_str(),
            gid2.c_str());
    return ok();
}

ScopedAStatus MtkRadioSim::setPhoneBookMemStorage(int32_t serial, const std::string& storage, const std::string& password, int32_t clientId) {
    mtkLogD(LOG_TAG, "setPhoneBookMemStorage: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_PHB_MEM_STORAGE, true,
            2, storage.c_str(), password.c_str());
    return ok();
}

ScopedAStatus MtkRadioSim::setPhonebookReady(int32_t serial, int32_t ready, int32_t clientId) {
    mtkLogD(LOG_TAG, "RadioImpl::setPhonebookReady: serial %d, ready %d, clientId:%d, slotId %d",
            serial, ready, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_PHONEBOOK_READY, 1, ready);
    return ok();
}

ScopedAStatus MtkRadioSim::setSimPower(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setSimPower: slotid:%d, serial %d, clientId %d", mSlotId, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_SIM_POWER, 1, mode);
    return ok();
}

bool dispatchPhbEntry(int serial, int slotId, int request,
                              const aidl_mtkradioex_sim::PhbEntryStructure& phbEntry) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_PhbEntryStructure pbe;
    pbe.type = phbEntry.type;
    pbe.index = phbEntry.index;

    if (!copyAidlStringToRil(&pbe.number, phbEntry.number, pRI)) {
        return false;
    }
    pbe.ton = phbEntry.ton;

    if (!copyAidlStringToRil(&pbe.alphaId, phbEntry.alphaId, pRI)) {
        memsetAndFreeStrings(1, pbe.number);
        return false;
    }

#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(request, &pbe, sizeof(pbe), pRI,
            pRI->socket_id);
#else
    s_vendorFunctions->onRequest(request, &pbe, sizeof(pbe), pRI);
#endif

    memsetAndFreeStrings(1, pbe.number);
    memsetAndFreeStrings(1, pbe.alphaId);

    return true;
}

ScopedAStatus MtkRadioSim::writePhbEntry(int32_t serial, const aidl::PhbEntryStructure& phbEntry, int32_t clientId) {
    mtkLogD(LOG_TAG, "writePhbEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchPhbEntry(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_WRITE_PHB_ENTRY, phbEntry);
    return ok();
}


bool dispatchAidlPhbEntryExt(int serial, int slotId, int request,
                              const aidl_mtkradioex_sim::PhbEntryExt& phbEntryExt) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_PHB_ENTRY pbe;
    pbe.index = phbEntryExt.index;

    if (!copyAidlStringToRil(&pbe.number, phbEntryExt.number, pRI)) {
        return false;
    }
    pbe.type = phbEntryExt.type;

    if (!copyAidlStringToRil(&pbe.text, phbEntryExt.text, pRI)) {
        memsetAndFreeStrings(1, pbe.number);
        return false;
    }
    pbe.hidden = phbEntryExt.hidden;

    if (!copyAidlStringToRil(&pbe.group, phbEntryExt.group, pRI)) {
        memsetAndFreeStrings(1, pbe.number);
        memsetAndFreeStrings(1, pbe.text);
        return false;
    }

    if (!copyAidlStringToRil(&pbe.adnumber, phbEntryExt.adnumber, pRI)) {
        memsetAndFreeStrings(1, pbe.number);
        memsetAndFreeStrings(1, pbe.text);
        memsetAndFreeStrings(1, pbe.group);
        return false;
    }
    pbe.adtype = phbEntryExt.adtype;

    if (!copyAidlStringToRil(&pbe.secondtext, phbEntryExt.secondtext, pRI)) {
        memsetAndFreeStrings(1, pbe.number);
        memsetAndFreeStrings(1, pbe.text);
        memsetAndFreeStrings(1, pbe.group);
        memsetAndFreeStrings(1, pbe.adnumber);
        return false;
    }

    if (!copyAidlStringToRil(&pbe.email, phbEntryExt.email, pRI)) {
        memsetAndFreeStrings(1, pbe.number);
        memsetAndFreeStrings(1, pbe.text);
        memsetAndFreeStrings(1, pbe.group);
        memsetAndFreeStrings(1, pbe.adnumber);
        memsetAndFreeStrings(1, pbe.secondtext);
        return false;
    }

#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(request, &pbe, sizeof(pbe), pRI,
            pRI->socket_id);
#else
    s_vendorFunctions->onRequest(request, &pbe, sizeof(pbe), pRI);
#endif

    memsetAndFreeStrings(1, pbe.number);
    memsetAndFreeStrings(1, pbe.text);
    memsetAndFreeStrings(1, pbe.group);
    memsetAndFreeStrings(1, pbe.adnumber);
    memsetAndFreeStrings(1, pbe.secondtext);
    memsetAndFreeStrings(1, pbe.email);

    return true;
}

ScopedAStatus MtkRadioSim::writePhoneBookEntryExt(int32_t serial, const aidl::PhbEntryExt& phbEntryExt, int32_t clientId) {
    mtkLogD(LOG_TAG, "writePhoneBookEntryExt: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchAidlPhbEntryExt(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_WRITE_PHB_ENTRY_EXT,
            phbEntryExt);
    return ok();
}

bool dispatchGrpEntry(int serial, int slotId, int request, int adnIndex, const std::vector<int32_t>& grpId) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    int countInts = grpId.size() + 1;
    int *pInts = (int *)calloc(countInts, sizeof(int));

    if (pInts == NULL) {
        mtkLogE(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(request));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }
    pInts[0] = adnIndex;
    for (int i = 1; i < countInts; i++) {
        pInts[i] = grpId[i-1];
    }

#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(request, pInts, countInts * sizeof(int), pRI,
            pRI->socket_id);
#else
    s_vendorFunctions->onRequest(request, pInts, countInts * sizeof(int), pRI);
#endif

    if (pInts != NULL) {
#ifdef MEMSET_FREED
        memset(pInts, 0, countInts * sizeof(int));
#endif
        free(pInts);
    }
    return true;
}

ScopedAStatus MtkRadioSim::writeUPBGrpEntry(int32_t serial, int32_t adnIndex, const std::vector<int32_t>& grpIds, int32_t clientId) {
    mtkLogD(LOG_TAG, "writeUPBGrpEntry: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchGrpEntry(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_WRITE_UPB_GRP, adnIndex, grpIds);
    return ok();
}

ScopedAStatus MtkRadioSim::queryUPBCapability(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "queryUPBCapability: serial %d, clientId:%d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_QUERY_UPB_CAPABILITY);
    return ok();
}

ScopedAStatus MtkRadioSim::supplyDepersonalization(int32_t serial, const std::string& netPin,
        int32_t type, int32_t clientId) {
    mtkLogD(LOG_TAG, "supplyDepersonalization: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_ENTER_DEPERSONALIZATION, true, 2,
            netPin.c_str(), std::to_string((int) type).c_str());
    return ok();
}

ScopedAStatus MtkRadioSim::supplyDeviceNetworkDepersonalization(int32_t serial,
        const std::string& pwd, int32_t clientId) {
    mtkLogD(LOG_TAG, "supplyDeviceNetworkDepersonalizationResponse: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(
            serial,
            mSlotId, RIL_REQUEST_ENTER_DEVICE_NETWORK_DEPERSONALIZATION,
            true,
            1,
            pwd.c_str());
    return ok();
}

ScopedAStatus MtkRadioSim::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus MtkRadioSim::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExSimResponse>& radioResponse,
        const std::shared_ptr<aidl::IMtkRadioExSimIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk sim setResponseFunctions, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk sim setResponseFunctions: got lock %d", mSlotId);

    radioSimServiceImpl[mSlotId].mtkSimResponse = radioResponse;
    radioSimServiceImpl[mSlotId].mtkSimIndication = radioIndication;
    plusCounterMtkRadioNumOne(mSlotId);

    rilAidlUtils::setMtkResponseNumberToOne(rilAidlUtils::SIM_INDEX, mSlotId);
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk sim setResponseFunctions: release lock %d", mSlotId);

    // client is connected. Send initial indications.
    android::ClientId client = getClientBySlot(mSlotId);
    if (rilAidlUtils::checkIfSetAllMtkResponseDone(mSlotId) == 1) {
        if (client == android::CLIENT_TBOX) {
            android::onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::TBOX_RADIO_MTK);
        } else {
            android::onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::RADIO_MTK);
        }
    }

    return ok();
}

ScopedAStatus MtkRadioSim::setResponseFunctionsRsu(
        const std::shared_ptr<aidl_mtkradioex_rsu::IRsuRadioResponse>& rsuResponse,
        const std::shared_ptr<aidl_mtkradioex_rsu::IRsuRadioIndication>& rsuIndication) {
    mtkLogD(LOG_TAG, "setResponseFunctionsRsu, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsRsu: got lock %d", mSlotId);

    radioSimServiceImpl[mSlotId].mtkRsuResponse = rsuResponse;
    radioSimServiceImpl[mSlotId].mtkRsuIndication = rsuIndication;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsRsu: release lock %d", mSlotId);
    return ok();
}

ScopedAStatus MtkRadioSim::setResponseFunctionsSE(
        const std::shared_ptr<aidl_mtkradioex_se::ISERadioResponse>& radioResponse,
        const std::shared_ptr<aidl_mtkradioex_se::ISERadioIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "setResponseFunctionsSE, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsSE: got lock %d", mSlotId);

    radioSimServiceImpl[mSlotId].mtkSeResponse = radioResponse;
    radioSimServiceImpl[mSlotId].mtkSeIndication = radioIndication;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsSE: release lock %d", mSlotId);
    return ok();
}
}

int mtkRadioExSim::clearMtkSimResponseAndIndications(unsigned int slotId) {
    radioSimServiceImpl[slotId].mtkSimResponse = NULL;
    radioSimServiceImpl[slotId].mtkSimIndication = NULL;
    return 0;
}

int mtkRadioExSim::clearMtkRsuResponseAndIndications(unsigned int slotId) {
    int realSlotId = slotId % MAX_SIM_COUNT;
    radioSimServiceImpl[realSlotId].mtkRsuResponse = NULL;
    radioSimServiceImpl[realSlotId].mtkRsuIndication = NULL;
    return 0;
}

int mtkRadioExSim::clearMtkSeResponseAndIndications(unsigned int slotId) {
    int realSlotId = slotId % MAX_SIM_COUNT;
    radioSimServiceImpl[realSlotId].mtkSeResponse = NULL;
    radioSimServiceImpl[realSlotId].mtkSeIndication = NULL;
    return 0;
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addSimLock(std::shared_ptr<ndk::ICInterface> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_SIM_PORT_BASE 52000
#define MTK_RADIO_SIM_PORT_BASE 52100

static pthread_t gThreadIds[MAX_SIM_COUNT*4];

void* addRcpVsockRadioSimAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_SIM_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_SIM_PORT_BASE + sloid, sloid);

    std::shared_ptr<android::hardware::radio::sim::RadioSim> radioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(sloid);
    addSimLock(std::static_pointer_cast<ndk::ICInterface>(radioSimAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioSimAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void* addRcpVsockMtkRadioSimAidlService(void* arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, MTK_RADIO_SIM_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", MTK_RADIO_SIM_PORT_BASE + sloid, sloid);
    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkRadioSimAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(sloid);
    addSimLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioSimAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);

    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadioSimAidl->asBinder().get()));
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioSimVsock(unsigned int &sloid, unsigned int &imsSlotid, unsigned int &seSlotid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
            addRcpVsockRadioSimAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioSim");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT], NULL,
            addRcpVsockRadioSimAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(imsSlotid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioSim");
    }

    if (pthread_create(&gThreadIds[sloid+ MAX_SIM_COUNT*2], NULL,
            addRcpVsockRadioSimAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(seSlotid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioSim");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT*3], NULL,
            addRcpVsockMtkRadioSimAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockMtkRadioSim");
    }
}
#endif


void publicRadioSimAidlService(unsigned int sloid) {
    unsigned int imsSlotid = sloid + + android::CLIENT_IMS *MAX_SIM_COUNT;
    unsigned int seSlotid = sloid + android::CLIENT_SE * MAX_SIM_COUNT;
    /*if (sloid == 0) {
        std::shared_ptr<android::hardware::radio::sim::RadioSim> radioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(0);
        gPublishedHals.push_back(radioSimAidl);
        const std::string instance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/slot1";
        binder_status_t status =
                AServiceManager_addService(radioSimAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(imsSlotid);
        gPublishedHals.push_back(RadioSimAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/imsSlot1";
        binder_status_t imsStatus =
                AServiceManager_addService(RadioSimAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        gPublishedHals.push_back(RadioSimAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/se1";
        binder_status_t seStatus =
                AServiceManager_addService(RadioSimAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for SE, seSlotid:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkRadioSimAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(0);
        gPublishedHals.push_back(mtkRadioSimAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/slot1";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioSimAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk sim addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 1) {
        std::shared_ptr<android::hardware::radio::sim::RadioSim> radioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(1);
        gPublishedHals.push_back(radioSimAidl);
        const std::string instance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/slot2";
        binder_status_t status =
                AServiceManager_addService(radioSimAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(imsSlotid);
        gPublishedHals.push_back(RadioSimAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/imsSlot2";
        binder_status_t imsStatus =
                AServiceManager_addService(RadioSimAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        gPublishedHals.push_back(RadioSimAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/se2";
        binder_status_t seStatus =
                AServiceManager_addService(RadioSimAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for SE, seSlotid:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkRadioSimAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(1);
        gPublishedHals.push_back(mtkRadioSimAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/slot2";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioSimAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk sim addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 2) {
        std::shared_ptr<android::hardware::radio::sim::RadioSim> radioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(2);
        gPublishedHals.push_back(radioSimAidl);
        const std::string instance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/slot3";
        binder_status_t status =
                AServiceManager_addService(radioSimAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(imsSlotid);
        gPublishedHals.push_back(RadioSimAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/imsSlot3";
        binder_status_t imsStatus =
                AServiceManager_addService(RadioSimAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        gPublishedHals.push_back(RadioSimAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/se3";
        binder_status_t seStatus =
                AServiceManager_addService(RadioSimAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for SE, seSlotid:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkRadioSimAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(2);
        gPublishedHals.push_back(mtkRadioSimAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/slot3";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioSimAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk sim addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 3) {
        std::shared_ptr<android::hardware::radio::sim::RadioSim> radioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(3);
        gPublishedHals.push_back(radioSimAidl);
        const std::string instance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/slot4";
        binder_status_t status =
                AServiceManager_addService(radioSimAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(imsSlotid);
        gPublishedHals.push_back(RadioSimAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/imsSlot4";
        binder_status_t imsStatus =
                AServiceManager_addService(RadioSimAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for IMS, imsSlotid:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> RadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        gPublishedHals.push_back(RadioSimAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/se4";
        binder_status_t seStatus =
                AServiceManager_addService(RadioSimAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "aosp sim addservice for SE, seSlotid:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkRadioSimAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(3);
        gPublishedHals.push_back(mtkRadioSimAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/slot4";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioSimAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk sim addservice, slotId:%d, status:%d", sloid, mtkStatus);
    }*/

    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;
    unsigned int tboxImsSlotid = sloid + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;

   if (sloid == 0) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioSimVsock(sloid, imsSlotid, seSlotid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioSimAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxImsSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxImsSlot1";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioSimAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for IMS, imsSlotid:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSe1";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioSimAidlforSe->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for Se, seSlotid:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkTboxRadioSimAidl =
                        ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioSimAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/tboxSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioSimAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox sim addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 1) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioSimVsock(sloid, imsSlotid, seSlotid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioSimAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxImsSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxImsSlot2";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioSimAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for IMS, imsSlotid:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSe2";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioSimAidlforSe->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for Se, seSlotid:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkTboxRadioSimAidl =
                        ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioSimAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/tboxSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioSimAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox sim addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 2) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioSimVsock(sloid, imsSlotid, seSlotid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioSimAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxImsSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxImsSlot3";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioSimAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for IMS, imsSlotid:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSe3";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioSimAidlforSe->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for Se, seSlotid:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkTboxRadioSimAidl =
                        ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioSimAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioSimAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox sim addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 3) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioSimVsock(sloid, imsSlotid, seSlotid);
#endif

       // add service for yocto client
        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidl = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSlot4";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioSimAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(tboxImsSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxImsSlot4";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioSimAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for IMS, imsSlotid:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::sim::RadioSim> tboxRadioSimAidlforSe = ndk::SharedRefBase::make<android::hardware::radio::sim::RadioSim>(seSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioSimAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::sim::RadioSim::descriptor + "/tboxSe4";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioSimAidlforSe->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox sim addservice for Se, seSlotid:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim> mtkTboxRadioSimAidl =
                        ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim>(tboxSlotid);
        addSimLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioSimAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::sim::MtkRadioSim::descriptor + "/tboxSlot4";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioSimAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox sim addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    }
}
