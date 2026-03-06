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
#include <telephony/ril.h>
#include <ril_internal.h>
#include <ril_service.h>
#include <radioims_service.h>
#include <mtk_log.h>
#include <rilAidlUtils.h>
#include <binder/ProcessState.h>
#include <android/binder_manager.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

namespace aidl_radio_ims = ::aidl::android::hardware::radio::ims;
namespace aidl_ims_mtk = ::aidl::vendor::mediatek::hardware::mtkradioex;
namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_radio_messaging = ::aidl::android::hardware::radio::messaging;

using ::aidl::android::hardware::radio::RadioError;
using ::aidl::android::hardware::radio::RadioResponseInfo;
using rilAidlUtils::copyAidlStringToRil;
using rilAidlUtils::populateAidlResponseInfo;
using rilAidlUtils::retChecker;
using aidl_ims_mtk::ims::ImsRegStatusInfo;
using aidl_ims_mtk::ims::ImsConfParticipant;
using aidl_ims_mtk::voice::Dialog;

extern "C" unsigned int toRealSlot(unsigned int slotId);

struct RadioImsImpl {
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::ims::IMtkRadioExImsResponse> mtkImsResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::ims::IMtkRadioExImsIndication> mtkImsIndication;
    std::shared_ptr<::aidl::android::hardware::radio::ims::IRadioImsResponse> imsResponse;
    std::shared_ptr<::aidl::android::hardware::radio::ims::IRadioImsIndication> imsIndication;
};

struct RadioMwiImpl {
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::mwi::IMwiRadioResponse> mtkMwiResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::mwi::IMwiRadioIndication> mtkMwiIndication;
};

#ifndef WIFI_SNR_UNKNOW
#define WIFI_SNR_UNKNOW 60
#endif
#define IMS_MAX_MD_CFG_KEYS_LEN (64*10)
#define IMS_MAX_MD_CFG_VALS_LEN (256*10)


RadioImsImpl radioImsServiceImpl[MAX_SIM_COUNT*(android::CLIENT_TBOX_IMS + 1)];
RadioMwiImpl radioMwiServiceImpl[MAX_SIM_COUNT*(android::CLIENT_TBOX + 1)];
static RIL_ImsPendingInd *s_ims_pending_ind[MAX_SIM_COUNT] = {NULL};

void notifyMtkImsServiceReady(unsigned int slot) {
    // Fire pending indication here because the function must be called after all event
    // registrations are ready in the constructor of the class ImsService.java
    //unsigned int slot = toRealSlot(mSlotId);
    RIL_ImsPendingInd *node = s_ims_pending_ind[slot];
    RIL_ImsPendingInd *pre_node = NULL;
    while (node != NULL) {
        // Fire pending indication
        mtkLogI(LOG_TAG, "notifyMtkImsServiceReady, [%d]Fire Pending indication %s(%d)", slot,
                android::requestToString(node->request), node->slot);
        switch(node->request) {
            case RIL_UNSOL_VOPS_INDICATION:
                mtkRadioExIms::sendVopsIndication(
                        node->slot, node->responseType, 0, RIL_E_SUCCESS, node->data,
                        node->data_size);
                break;

            case RIL_UNSOL_VOLTE_SETTING:
                mtkRadioExIms::volteSettingInd(
                        node->slot, node->responseType, 0, RIL_E_SUCCESS, node->data,
                        node->data_size);
                break;
            case RIL_UNSOL_IMS_REG_FLAG_IND:
                mtkRadioExIms::imsRegFlagInd(
                        node->slot, node->responseType, 0, RIL_E_SUCCESS, node->data,
                        node->data_size);
                break;
            case RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY:
                mtkRadioExIms::emergencyBearerInfoInd(
                        node->slot, node->responseType, 0, RIL_E_SUCCESS, node->data,
                        node->data_size);
                break;
            case RIL_UNSOL_ON_XUI:
                mtkRadioExIms::onXuiInd(
                        node->slot, node->responseType, 0, RIL_E_SUCCESS, node->data,
                        node->data_size);
                break;
        }
        pre_node = node;
        node = node->next;
        if (pre_node->data != NULL) {
            if (pre_node->request == RIL_UNSOL_ON_XUI) {
                char **data = (char **)pre_node->data;
                int numStrings = pre_node->data_size / sizeof(char *);
                for (int i = 0; i < numStrings; i++) {
                    if (data[i] != NULL) {
                        free(data[i]);
                    }
                }
            }
            free(pre_node->data);
        }
        free(pre_node);
    }
    s_ims_pending_ind[slot] = NULL;
    //return Void();
}

void copyStringArrayToPended(char ** nodeData, const void* data, int size) {
    char **resp = (char **) data;
    int numStrings = size / sizeof(char *);
    for (int i = 0; i < numStrings; i++) {
        int len = 0;
        if (nodeData[i] != NULL) {
            free(nodeData[i]);
            nodeData[i] = NULL;
        }

        if (resp[i] != NULL) {
            len = strlen(resp[i]);
        }
        if (len > 0) {
            nodeData[i] = (char*)calloc(1, len + 1);
            if (nodeData[i] == NULL) {
                mtkLogE(LOG_TAG, "copyStringArrayToPended, data[%d] calloc NULL.", i);
            } else {
                memcpy(nodeData[i], resp[i], len);
            }
        }
    }

}

void appendMtkImsind(unsigned int slotId, int request, int responseType,
        const void* data, int size) {
    if (request != RIL_UNSOL_VOPS_INDICATION
            && request != RIL_UNSOL_VOLTE_SETTING
            && request != RIL_UNSOL_IMS_REG_FLAG_IND
            && request != RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY
            && request != RIL_UNSOL_ON_XUI) {
        mtkLogE(LOG_TAG,
            "appendMtkImsind, Request %s no need to append into IMS pending ind list",
            android::requestToString(request));
        return;
    }

    bool ignore_duplicate_request = false;
    bool is_addNode = true;
    // If it is not necessary to keep multiple same request id in the pendding list,
    // to add the request id here
    if (request == RIL_UNSOL_VOPS_INDICATION
        || request == RIL_UNSOL_VOLTE_SETTING
        || request == RIL_UNSOL_IMS_REG_FLAG_IND
        || request == RIL_UNSOL_ON_XUI) {
        ignore_duplicate_request = true;
    }

    unsigned int slot = slotId;//toRealSlot(slotId);
    // Cache the indication
    RIL_ImsPendingInd *node = s_ims_pending_ind[slot];
    while (node != NULL) {
        if (ignore_duplicate_request && node->request == request) {
            mtkLogD(LOG_TAG,
                "appendMtkImsind, Request %s(%d) is already in IMS pending ind list, only copy new data",
                android::requestToString(request), slot);
            if (data != NULL) {
                if (node->data != NULL) {
                    if (node->request == RIL_UNSOL_ON_XUI) {
                        char **pData = (char **)node->data;
                        int numStrings = node->data_size / sizeof(char *);
                        for (int i = 0; i < numStrings; i++) {
                            if (pData[i] != NULL) {
                                free(pData[i]);
                            }
                        }
                    }
                    free(node->data);
                }
                node->data = (void*) calloc(1, size);
                if (node->data == NULL) {
                    mtkLogE(LOG_TAG, "appendMtkImsind, node->data calloc NULL.");
                } else {
                    if (request == RIL_UNSOL_ON_XUI) {
                        copyStringArrayToPended((char**)(node->data), data, size);
                        node->data_size = size;
                    } else {
                        memcpy(node->data, data, size);
                        node->data_size = size;
                    }
                }
            }
            return;
        }

        if (node->next != NULL) {
            node = node->next;
        } else {
            break;
        }
    }

    if (node == NULL) {
        node = (RIL_ImsPendingInd  *) calloc(1, sizeof(RIL_ImsPendingInd));
        if (node == NULL) {
            mtkLogE(LOG_TAG, "appendMtkImsind, node calloc NULL.");
        } else {
            node->slot = slotId;
            node->request = request;
            node->responseType = responseType;
            node->next = NULL;
            if (data != NULL) {
                node->data = (void*) calloc(1, size);
                if (node->data == NULL) {
                    mtkLogE(LOG_TAG, "appendMtkImsind, node->data calloc NULL.");
                    is_addNode = false;
                    free(node);
                } else {
                    if (request == RIL_UNSOL_ON_XUI) {
                        copyStringArrayToPended((char**)(node->data), data, size);
                        node->data_size = size;
                    } else {
                        memcpy(node->data, data, size);
                        node->data_size = size;
                    }
                }
            }
            if (is_addNode) {
                s_ims_pending_ind[slot] = node;
            }
        }
    } else {
        RIL_ImsPendingInd *item =
                (RIL_ImsPendingInd  *) calloc(1, sizeof(RIL_ImsPendingInd));
        if (item == NULL) {
            mtkLogE(LOG_TAG, "appendMtkImsind, item calloc NULL.");
        } else {
            item->slot = slotId;
            item->request = request;
            item->responseType = responseType;
            item->next = NULL;
            if (data != NULL) {
                item->data = (void*) calloc(1, size);
                if (item->data == NULL) {
                    mtkLogE(LOG_TAG, "appendMtkImsind, item->data calloc NULL.");
                    is_addNode = false;
                    free(item);
                } else {
                    if (request == RIL_UNSOL_ON_XUI) {
                        copyStringArrayToPended((char**)(item->data), data, size);
                        item->data_size = size;
                    } else {
                        memcpy(item->data, data, size);
                        item->data_size = size;
                    }
                }
            }
            if (is_addNode) {
                node->next = item;
            }
        }
    }

    mtkLogI(LOG_TAG,
        "appendMtkImsind, Request %s(%d) already append into IMS pending ind list",
        android::requestToString(request), slot);
}

int imsResponseIntOrEmpty(RadioResponseInfo& responseInfo, int serial, int responseType, RIL_Errno e,
               const void *response, size_t responseLen) {
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    int ret = -1;

    if (response == NULL && responseLen == 0) {
        // Earlier RILs did not send a response for some cases although the interface
        // expected an integer as response. Do not return error if response is empty. Instead
        // Return -1 in those cases to maintain backward compatibility.
    } else if (response == NULL || responseLen != sizeof(int)) {
        mtkLogE(LOG_TAG, "imsResponseIntOrEmpty: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
    } else {
        int *p_int = (int *) response;
        ret = p_int[0];
    }
    return ret;
}

int imsResponseInt(RadioResponseInfo& responseInfo, int serial, int responseType, RIL_Errno e,
               const void *response, size_t responseLen) {
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    int ret = -1;

    if (response == NULL || responseLen != sizeof(int)) {
        mtkLogE(LOG_TAG, "imsResponseInt: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
    } else {
        int *p_int = (int *) response;
        ret = p_int[0];
    }
    return ret;
}

// response
int radioIms::setSrvccCallInfoResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioImsServiceImpl[slotId].imsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].imsResponse->setSrvccCallInfoResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSrvccCallInfoResponse: invalid response");
            // retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioImsServiceImpl[%d].setSrvccCallInfoResponse == NULL", slotId);
    }

    return 0;
}

int radioIms::updateImsRegistrationInfoResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioImsServiceImpl[slotId].imsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].imsResponse->updateImsRegistrationInfoResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "updateImsRegistrationInfoResponse: invalid response");
            // retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioImsServiceImpl[%d].updateImsRegistrationInfoResponse == NULL", slotId);
    }

    return 0;
}

int radioIms::startImsTrafficResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioImsServiceImpl[slotId].imsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].imsResponse->startImsTrafficResponse(
                responseInfo, {});
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "startImsTrafficResponse: invalid response");
            // retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioImsServiceImpl[%d].startImsTrafficResponse == NULL", slotId);
    }

    return 0;
}

int radioIms::stopImsTrafficResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioImsServiceImpl[slotId].imsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].imsResponse->stopImsTrafficResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "stopImsTrafficResponse: invalid response");
            // retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioImsServiceImpl[%d].stopImsTrafficResponse == NULL", slotId);
    }

    return 0;
}

int radioIms::triggerEpsFallbackResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioImsServiceImpl[slotId].imsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].imsResponse->triggerEpsFallbackResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "triggerEpsFallbackResponse: invalid response");
            // retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioImsServiceImpl[%d].triggerEpsFallbackResponse == NULL", slotId);
    }

    return 0;
}

int radioIms::sendAnbrQueryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioImsServiceImpl[slotId].imsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].imsResponse->sendAnbrQueryResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendAnbrQueryResponse: invalid response");
            // retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioImsServiceImpl[%d].sendAnbrQueryResponse == NULL", slotId);
    }

    return 0;
}

int radioIms::updateImsCallStatusResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen) {
    if (radioImsServiceImpl[slotId].imsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].imsResponse->updateImsCallStatusResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "updateImsCallStatusResponse: invalid response");
            // retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioImsServiceImpl[%d].updateImsCallStatusResponse == NULL", slotId);
    }

    return 0;
}

//indication
int radioIms::onConnectionSetupFailure(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen) {
    return 0;
}

int radioIms::notifyAnbr(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen) {
    return 0;
}

int radioIms::triggerImsDeregistration(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen) {
    return 0;
}

int radioIms::clearImsResponseAndIndications(unsigned int slotId) {
    radioImsServiceImpl[slotId].imsResponse = NULL;
    radioImsServiceImpl[slotId].imsIndication = NULL;
    return 0;
}

namespace android::hardware::radio::ims {

using ::ndk::ScopedAStatus;
// namespace aidl_radio_ims = ::aidl::android::hardware::radio::ims;
constexpr auto ok = &ScopedAStatus::ok;

RadioIms::RadioIms(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus RadioIms::setSrvccCallInfo(int32_t serial,
            const std::vector<::aidl_radio_ims::SrvccCall>& srvccCalls) {
    mtkLogD(LOG_TAG, "setSrvccCallInfo: not support slotid:%d, serial %d", mSlotId, serial);

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_SET_SRVCC_CALL_INFO);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_INVALID_STATE);
    }

    return ok();
}

ScopedAStatus RadioIms::updateImsRegistrationInfo(int32_t serial,
            const ::aidl_radio_ims::ImsRegistration& imsRegistration) {
    mtkLogD(LOG_TAG, "updateImsRegistrationInfo: not support slotid:%d, serial %d", mSlotId, serial);

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_UPDATE_IMS_REGISTRATION_INFO);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_INVALID_STATE);
    }

    return ok();
}

ScopedAStatus RadioIms::startImsTraffic(int32_t serial, int token,
            ::aidl_radio_ims::ImsTrafficType imsTrafficType,
            ::aidl::android::hardware::radio::AccessNetwork accessNetworkType,
            ::aidl_radio_ims::ImsCall::Direction trafficDirection) {
    mtkLogD(LOG_TAG, "startImsTraffic: not support slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_START_IMS_TRAFFIC);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_INVALID_STATE);
    }

    return ok();
}

ScopedAStatus RadioIms::stopImsTraffic(int32_t serial, int32_t token) {
    mtkLogD(LOG_TAG, "stopImsTraffic: not support slotid:%d, serial %d", mSlotId, serial);

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_STOP_IMS_TRAFFIC);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_INVALID_STATE);
    }

    return ok();
}

ScopedAStatus RadioIms::triggerEpsFallback(int32_t serial,
            ::aidl_radio_ims::EpsFallbackReason reason) {
    mtkLogD(LOG_TAG, "triggerEpsFallback: not support slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_TRIGGER_EPS_FALLBACK);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_INVALID_STATE);
    }

    return ok();
}

ScopedAStatus RadioIms::setResponseFunctions(
            const std::shared_ptr<::aidl_radio_ims::IRadioImsResponse>& radioImsResponse,
            const std::shared_ptr<::aidl_radio_ims::IRadioImsIndication>& radioImsIndication) {
    mtkLogD(LOG_TAG, "aosp ims setResponseFunctions, slotId:%d", mSlotId);
    if (!isValidSlotId(mSlotId)) {
        return ok();
    }
    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp ims setResponseFunctions: got lock %d", mSlotId);

    radioImsServiceImpl[mSlotId].imsResponse = radioImsResponse;
    radioImsServiceImpl[mSlotId].imsIndication = radioImsIndication;
    plusCounterRadioNumOne(mSlotId);

    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX_IMS) {
        rilAidlUtils::setAospResponseNumberToOne(rilAidlUtils::IMS_INDEX, mSlotId);
    }

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp ims setResponseFunctions: release lock %d", mSlotId);
    // client is connected. Send initial indications.
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX_IMS) {
        if (rilAidlUtils::checkIfSetAllAospResponseDone(mSlotId) == 1) {
            if(client == android::CLIENT_TBOX_IMS) {
                android::onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::TBOX_RADIO_AOSP);
            } else {
                android::onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::RADIO_AOSP);
            }
        }
    }
    return ok();
}

ScopedAStatus RadioIms::sendAnbrQuery(int32_t serial,
            ::aidl_radio_ims::ImsStreamType mediaType,
            ::aidl_radio_ims::ImsStreamDirection direction,
            int32_t bitsPerSecond) {
    mtkLogD(LOG_TAG, "sendAnbrQuery: not support slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_SEND_ANBR_QUERY);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_INVALID_STATE);
    }

    return ok();
}

ScopedAStatus RadioIms::updateImsCallStatus(int32_t serial,
            const std::vector<::aidl_radio_ims::ImsCall>& imsCalls) {
    mtkLogD(LOG_TAG, "updateImsCallStatus: not support slotid:%d, serial %d", mSlotId, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_UPDATE_IMS_CALL_STATUS);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_INVALID_STATE);
    }

    return ok();
}

}

// M: IMS_DATA start @{
int mtkRadioExIms::imsBearerStateConfirmResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "imsBearerStateConfirmResponse: slotId:%d, serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret =
                radioImsServiceImpl[realSlotId].mtkImsResponse->imsBearerStateConfirmResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "imsBearerStateConfirmResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "imsBearerStateConfirmResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                             slotId);
    }
    return 0;
}

int mtkRadioExIms::setImsBearerNotificationResponse(unsigned int slotId, android::ClientId clientId,
                                           int responseType, int token, RIL_Errno e,
                                           const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setImsBearerNotificationResponse: slotId:%d, token %d", slotId, token);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, token, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->setImsBearerNotificationResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setImsBearerNotificationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setImsBearerNotificationResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::imsBearerStateNotifyInd(unsigned int slotId,
                                  int indicationType, int serial, RIL_Errno e,
                                  const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + android::CLIENT_TBOX * MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "imsBearerStateNotifyInd: slotId %d, tboxSlotId: %d", slotId, tboxSlotId);
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: invalid response");
            return 0;
        }

        RIL_IMS_BearerNotification *p_cur = (RIL_IMS_BearerNotification *) response;
        int aid = p_cur->aid;
        int action = p_cur->action;
        std::string type = convertCharPtrToStdString(p_cur->type);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsBearerStateNotify(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    aid, action, type);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsBearerStateNotify(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    aid, action, type);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }
    return 0;
}

int mtkRadioExIms::imsBearerInitInd(unsigned int slotId,
                            int indicationType, int serial, RIL_Errno e,
                            const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "imsBearerInitInd slotId:%d", slotId);
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "imsBearerInitInd: invalid response");
            return 0;
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsBearerInit(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "imsBearerInitInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "imsBearerInitInd: radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + android::CLIENT_TBOX * MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "imsBearerInitInd tboxSlotId:%d", tboxSlotId);
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "tbox-imsBearerInitInd: invalid response");
            return 0;
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsBearerInit(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "imsBearerInitInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-imsBearerInitInd: radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }

    return 0;
}

int mtkRadioExIms::imsDataInfoNotifyInd(unsigned int slotId,
                                  int indicationType, int serial, RIL_Errno e,
                                  const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + android::CLIENT_TBOX * MAX_SIM_COUNT;
    mtkLogD(LOG_TAG, "imsDataInfoNotifyInd slotId:%d", slotId);
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "imsDataInfoNotifyInd: invalid response");
            return 0;
        }

        RIL_IMS_DataInfoNotify *p_cur = (RIL_IMS_DataInfoNotify *) response;
        std::string type = convertCharPtrToStdString(p_cur->type);
        std::string event = convertCharPtrToStdString(p_cur->event);
        std::string extra = convertCharPtrToStdString(p_cur->extra);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsDataInfoNotify(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    type, event, extra);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsDataInfoNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsDataInfoNotify(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    type, event, extra);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsDataInfoNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "imsDataInfoNotifyInd: radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }
    return 0;
}
// M: IMS_DATA end @}

// M: IMS_SMS start @{
int mtkRadioExIms::acknowledgeLastIncomingGsmSmsExResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingGsmSmsExResponse:"
            "serial %d, slotid:%d", serial, slotId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse
                ->acknowledgeLastIncomingGsmSmsExResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "acknowledgeLastIncomingGsmSmsExResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "acknowledgeLastIncomingGsmSmsExResponse: radioImsServiceImpl[%d].mtkImsResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExIms::acknowledgeLastIncomingCdmaSmsExResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingCdmaSmsExResponse:"
            "serial %d, slotid:%d", serial, slotId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse
                ->acknowledgeLastIncomingCdmaSmsExResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "acknowledgeLastIncomingCdmaSmsExResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "acknowledgeLastIncomingCdmaSmsExResponse: mtkRadioExService[%d]->mRadioResponse "
                "== NULL", slotId);
    }
    return 0;
}

aidl_radio_messaging::SendSmsResult makeSendSmsResultEx(aidl_radio::RadioResponseInfo& responseInfo,
        int serial, int responseType, RIL_Errno e, const void *response, size_t responseLen) {
    populateAidlResponseInfo(responseInfo, serial, responseType, e);
    aidl_radio_messaging::SendSmsResult result = {};

    if (response == NULL || responseLen != sizeof(RIL_SMS_Response)) {
        mtkLogE(LOG_TAG, "makeSendSmsResultEx Invalid response: NULL");
        if (e == RIL_E_SUCCESS) {
            responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }
        result.ackPDU = std::string("");
    } else {
        RIL_SMS_Response *resp = (RIL_SMS_Response *) response;
        result.messageRef = resp->messageRef;
        result.ackPDU = convertCharPtrToStdString(resp->ackPDU);
        result.errorCode = resp->errorCode;
    }
    return result;
}

int mtkRadioExIms::sendImsSmsExResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendImsSmsExResponse: serial %d, slotid:%d", serial, slotId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};

        aidl_radio_messaging::SendSmsResult result = makeSendSmsResultEx(responseInfo, serial,
                responseType, e, response, responseLen);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse
                ->sendImsSmsExResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendImsSmsExResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sendImsSmsExResponse: "
                "radioImsServiceImpl[%d]->mtkImsResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExIms::newSmsStatusReportIndEx(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "newSmsStatusReportIndEx, slotid:%d", slotId);
    if (radioImsServiceImpl[slotId].mtkImsResponse != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsStatusReportIndEx: invalid response");
            return 0;
        }
        uint8_t *bytes = hexStringToBytes(response, responseLen);
        if (bytes == NULL) {
            mtkLogE(LOG_TAG, "newSmsStatusReportIndEx: hexStringToBytes failed");
            return 0;
        }
        std::vector<uint8_t> pdu;
        int num = responseLen / 2;
        pdu.resize(num);
        for (int i = 0 ; i < num; i++) {
            pdu[i] = bytes[i];
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->newSmsStatusReportEx(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "newSmsStatusReportIndEx: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
        free(bytes);
    } else {
        mtkLogE(LOG_TAG, "newSmsStatusReportIndEx:"
                "radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "newSmsStatusReportIndEx, tboxSlotId:%d", tboxSlotId);
    if (radioImsServiceImpl[tboxSlotId].mtkImsResponse != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsStatusReportIndEx: invalid response");
            return 0;
        }
        uint8_t *bytes = hexStringToBytes(response, responseLen);
        if (bytes == NULL) {
            mtkLogE(LOG_TAG, "newSmsStatusReportIndEx: hexStringToBytes failed");
            return 0;
        }
        std::vector<uint8_t> pdu;
        int num = responseLen / 2;
        pdu.resize(num);
        for (int i = 0 ; i < num; i++) {
            pdu[i] = bytes[i];
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->newSmsStatusReportEx(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "newSmsStatusReportIndEx: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
        free(bytes);
    } else {
        mtkLogE(LOG_TAG, "newSmsStatusReportIndEx:"
                "radioImsServiceImpl[%d].mtkImsIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::newSmsIndEx(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "newSmsIndEx, slotid:%d", slotId);
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsIndEx: invalid response");
            return 0;
        }
        uint8_t *bytes = hexStringToBytes(response, responseLen);
        if (bytes == NULL) {
            mtkLogE(LOG_TAG, "newSmsIndEx: hexStringToBytes failed");
            return 0;
        }
        std::vector<uint8_t> pdu;
        int num = responseLen / 2;
        pdu.resize(num);
        for (int i = 0 ; i < num; i++) {
            pdu[i] = bytes[i];
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->newSmsEx(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "newSmsIndEx: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
        free(bytes);
    } else {
        mtkLogE(LOG_TAG, "newSmsIndEx:"
                "radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "newSmsIndEx, tboxSlotId:%d", tboxSlotId);
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsIndEx: invalid response");
            return 0;
        }
        uint8_t *bytes = hexStringToBytes(response, responseLen);
        if (bytes == NULL) {
            mtkLogE(LOG_TAG, "tbox-newSmsIndEx: hexStringToBytes failed");
            return 0;
        }
        std::vector<uint8_t> pdu;
        int num = responseLen / 2;
        pdu.resize(num);
        for (int i = 0 ; i < num; i++) {
            pdu[i] = bytes[i];
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->newSmsEx(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "newSmsIndEx: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
        free(bytes);
    } else {
        mtkLogE(LOG_TAG, "newSmsIndEx:"
                "tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL", tboxSlotId);
    }
    return 0;
}

bool makeCdmaSmsMessageEx(aidl_radio_messaging::CdmaSmsMessage &msg,
        const void *response, size_t responseLen) {
    if (response == NULL || responseLen != sizeof(RIL_CDMA_SMS_Message)) {
        return false;
    }

    RIL_CDMA_SMS_Message *rilMsg = (RIL_CDMA_SMS_Message *) response;
    msg.teleserviceId = rilMsg->uTeleserviceID;
    msg.isServicePresent = rilMsg->bIsServicePresent;
    msg.serviceCategory = rilMsg->uServicecategory;
    msg.address.digitMode = rilMsg->sAddress.digit_mode;
    msg.address.isNumberModeDataNetwork =
            (rilMsg->sAddress.number_mode == RIL_CDMA_SMS_NUMBER_MODE_DATA_NETWORK) ? true : false;
    msg.address.numberType = rilMsg->sAddress.number_type;
    msg.address.numberPlan = rilMsg->sAddress.number_plan;

    int digitLimit = MIN((rilMsg->sAddress.number_of_digits), RIL_CDMA_SMS_ADDRESS_MAX);
    msg.address.digits.resize(digitLimit);
    for (int i = 0 ; i < digitLimit; i++) {
        msg.address.digits[i] = rilMsg->sAddress.digits[i];
    }

    msg.subAddress.subaddressType = rilMsg->sSubAddress.subaddressType;
    msg.subAddress.odd = rilMsg->sSubAddress.odd;

    digitLimit= MIN((rilMsg->sSubAddress.number_of_digits), RIL_CDMA_SMS_SUBADDRESS_MAX);
    msg.subAddress.digits.resize(digitLimit);
    for (int i = 0 ; i < digitLimit; i++) {
        msg.subAddress.digits[i] = rilMsg->sSubAddress.digits[i];
    }

    digitLimit = MIN((rilMsg->uBearerDataLen), RIL_CDMA_SMS_BEARER_DATA_MAX);
    msg.bearerData.resize(digitLimit);
    for (int i = 0 ; i < digitLimit; i++) {
        msg.bearerData[i] = rilMsg->aBearerData[i];
    }
    return true;
}

int mtkRadioExIms::cdmaNewSmsIndEx(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "cdmaNewSmsIndEx, slotid:%d", slotId);
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        aidl_radio_messaging::CdmaSmsMessage msg = {};
        if (!makeCdmaSmsMessageEx(msg, response, responseLen)) {
            mtkLogE(LOG_TAG, "cdmaNewSmsIndEx: invalid response");
        }
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->cdmaNewSmsEx(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), msg);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cdmaNewSmsIndEx: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaNewSmsIndEx:"
                "radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "cdmaNewSmsIndEx, tboxSlotId:%d", tboxSlotId);
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        aidl_radio_messaging::CdmaSmsMessage msg = {};
        if (!makeCdmaSmsMessageEx(msg, response, responseLen)) {
            mtkLogE(LOG_TAG, "tbox-cdmaNewSmsIndEx: invalid response");
        }
        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->cdmaNewSmsEx(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), msg);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cdmaNewSmsIndEx: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-cdmaNewSmsIndEx:"
                "radioImsServiceImpl[%d].mtkImsIndication == NULL", tboxSlotId);
    }
    return 0;
}
// M: IMS_SMS end @}

// M: IMS_RTT start @{
int mtkRadioExIms::setRttModeResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setRttModeResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 setRttModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setImsBearerNotificationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setRttModeResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                        slotId);
    }

    return 0;
}

int mtkRadioExIms::sendRttModifyRequestResponse(unsigned int slotId, android::ClientId clientId,
                                        int responseType, int serial, RIL_Errno e,
                                        const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "sendRttModifyRequestResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 sendRttModifyRequestResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setImsBearerNotificationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sendRttModifyRequestResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                                  slotId);
    }

    return 0;
}

int mtkRadioExIms::sendRttTextResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "sendRttTextResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 sendRttTextResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setImsBearerNotificationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sendRttTextResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                        slotId);
    }

    return 0;
}

int mtkRadioExIms::rttModifyRequestResponseResponse(unsigned int slotId, android::ClientId clientId,
                                            int responseType, int serial, RIL_Errno e,
                                            const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "rttModifyRequestResponseResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 rttModifyRequestResponseResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setImsBearerNotificationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "rttModifyRequestResponseResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                                      slotId);
    }

    return 0;
}

int mtkRadioExIms::toggleRttAudioIndicationResponse(unsigned int slotId, android::ClientId clientId,
                                        int responseType, int serial, RIL_Errno e, const void *response,
                                        size_t responselen) {
    mtkLogD(LOG_TAG, "toggleRttAudioIndicationResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 toggleRttAudioIndicationResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setImsBearerNotificationResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "toggleRttAudioIndicationResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                                  slotId);
    }

    return 0;
}

int mtkRadioExIms::rttModifyResponseInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        int *resp = (int *) response;
        int numStrings = responselen / sizeof(resp[0]);
        mtkLogD(LOG_TAG, "rttModifyResponseInd: slotId = %d numStrings = %d responseLen = %d ",
                                                                 slotId, numStrings, responselen);
        if(numStrings < 2) {
            mtkLogE(LOG_TAG, "rttModifyResponseInd : responselen < 2");
            return 0;
        }
        if(numStrings > 0) {
            int i =0;
            while(i < numStrings) {
                    mtkLogD(LOG_TAG, "rttModifyResponseInd: response[%d] =  %d; ", i, resp[i]);
                    i++;
            }
        }
        int callId = resp[0];
        int result = resp[1];

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[slotId].mtkImsIndication->rttModifyResponse(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, result);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[tboxSlotId].mtkImsIndication->rttModifyResponse(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, result);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "rttModifyResponseInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                         slotId);
    }

    return 0;
}

int mtkRadioExIms::rttTextReceiveInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        char **resp = (char **) response;
        int numStrings = responselen / sizeof(char *);
        if(numStrings < 3) {
            mtkLogE(LOG_TAG, "rttTextReceiveInd: items length invalid, slotId = %d numStrings = %d resposneLen = %d",
                                                              slotId, numStrings, responselen);
            return 0;
        }

        int callId = atoi(resp[0]);
        int length = atoi(resp[1]);
        std::string text = convertCharPtrToStdString(resp[2]);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[slotId].mtkImsIndication->rttTextReceive(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, length, text);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[tboxSlotId].mtkImsIndication->rttTextReceive(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, length, text);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "rttTextReceiveInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                      slotId);
    }

    return 0;
}

int mtkRadioExIms::rttCapabilityIndicationInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        int *resp = (int *) response;
        int numStrings = responselen/ sizeof(resp[0]);
        mtkLogD(LOG_TAG, "rttCapabilityIndicationInd: slotId = %d numString = %d responselen = %d",
                                                                       slotId, numStrings, responselen);
        if(numStrings < 5) {
            numStrings = responselen / sizeof(int);
            if(numStrings < 5) {
                mtkLogD(LOG_TAG, "rttCapabilityIndicationInd : numStrings < 5");
                return 0;
            }
            int callId = ((int32_t *) resp)[0];
            int localCap = ((int32_t *) resp)[1];
            int remoteCap = ((int32_t *) resp)[2];
            int localStatus = ((int32_t *) resp)[3];
            int remoteStatus = ((int32_t *) resp)[4];
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->rttCapabilityIndication(
                                   rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                  callId, localCap, remoteCap, localStatus, remoteStatus);
        }
        if(numStrings > 0) {
            int i =0;
            while(i < numStrings) {
                    mtkLogD(LOG_TAG, "rttCapabilityIndicationInd: response[%d] =  %d; ", i, resp[i]);
                    i++;
            }
        }
        int callId = resp[0];
        int localCap = resp[1];
        int remoteCap = resp[2];
        int localStatus = resp[3];
        int remoteStatus = resp[4];

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[slotId].mtkImsIndication->rttCapabilityIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, localCap, remoteCap, localStatus, remoteStatus);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[tboxSlotId].mtkImsIndication->rttCapabilityIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, localCap, remoteCap, localStatus, remoteStatus);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "rttCapabilityIndicationInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                               slotId);
    }

    return 0;
}

int mtkRadioExIms::rttModifyRequestReceiveInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        int *resp = (int *) response;
        int numStrings = responselen / sizeof(int);
        if(numStrings < 2) {
            mtkLogE(LOG_TAG, "rttModifyRequestReceiveInd: items length invalid, slotId = %d",
                                                                       slotId);
            return 0;
        }

        int callId = ((int32_t *) resp)[0];
        int type = ((int32_t *) resp)[1];

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[slotId].mtkImsIndication->rttModifyRequestReceive(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, type);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[tboxSlotId].mtkImsIndication->rttModifyRequestReceive(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, type);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "rttModifyRequestReceiveInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                               slotId);
    }

    return 0;
}

int mtkRadioExIms::audioIndicationInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        int *resp = (int *) response;
        int numStrings = responselen / sizeof(resp[0]);
        if(numStrings < 2) {
            mtkLogE(LOG_TAG, "audioIndicationInd: items length invalid, slotId = %d",
                                                                       slotId);
            return 0;
        }

        int callId = resp[0];
        int audio = resp[1];

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[slotId].mtkImsIndication->audioIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, audio);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret =  radioImsServiceImpl[tboxSlotId].mtkImsIndication->audioIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, audio);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "imsBearerStateNotifyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "audioIndicationInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                               slotId);
    }

    return 0;
}
// M: IMS_RTT end @}


// M: IMS_BASE start @{
int mtkRadioExIms::setImsRegistrationReportResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setImsRegistrationReportResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->setImsRegistrationReportResponse(
                                                       responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setImsRegistrationReportResponse: radioImsServiceImpl[%d]->mtkImsResponse == NULL",
                                                                                 slotId);
    }

    return 0;
}

int mtkRadioExIms::queryVopsStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "[%d]queryVopsStatusResponse: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "queryVopsStatusResponse: invalid response!");
            return 0;
        }
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        int *resp = (int *) response;
        int numStrings = responseLen / sizeof(int);
        if(numStrings < 1) {
            mtkLogE(LOG_TAG, "queryVopsStatusResponse: items length invalid, slotId = %d", slotId);
            return 0;
        }

        int vops = ((int32_t *) resp)[1];

        mtkLogI(LOG_TAG, "queryVopsStatusResponse - vops = %d", vops);
        ndk::ScopedAStatus ret =
                radioImsServiceImpl[realSlotId].mtkImsResponse->queryVopsStatusResponse(
                responseInfo, vops);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "queryVopsStatusResponse: "
                "radioImsServiceImpl[%d].mtkImsResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExIms::sendVopsIndication(unsigned int slotId,
                                int indicationType, int token, RIL_Errno e,
                                const void *response, size_t responselen) {
    bool perform = false;
    //unsigned int rcsSlotId = toClientSlot(slotId, android::CLIENT_RCS);

    /*TODO if (radioImsServiceImpl[rcsSlotId]->mRadioIndicationRcs == NULL) {
       mtkLogE(LOG_TAG, "imsVoPSIndication: radioService[%d]->mRadioIndicationRcs == NULL", rcsSlotId);
    } else {
       perform = true;
    }*/

    if (radioImsServiceImpl[slotId].mtkImsIndication == NULL) {
        mtkLogE(LOG_TAG, "sendVopsIndication: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
        appendMtkImsind(slotId, RIL_UNSOL_VOPS_INDICATION, indicationType, response,
                    responselen);
    } else {
        perform = true;
    }

    if (perform) {
        int *resp = (int *) response;
        int numStrings = responselen / sizeof(int);
        if(numStrings < 1) {
            mtkLogE(LOG_TAG, "sendVopsIndication: items length invalid, slotId = %d", slotId);
            return 0;
        }

        int vops = ((int32_t *) resp)[0];

        mtkLogD(LOG_TAG, "sendVopsIndication - vops = %d", vops);

         /*TODO if (radioImsServiceImpl[rcsSlotId]->mRadioIndicationRcs != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[rcsSlotId]->
                                     mRadioIndicationRcs->sendVopsIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     vops);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }*/

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->sendVopsIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    vops);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                appendMtkImsind(slotId, RIL_UNSOL_VOPS_INDICATION, indicationType,
                    response, responselen);
            }
        }
    }

    return 0;
}

int mtkRadioExIms::volteSettingInd(unsigned int slotId,
                           int indicationType, int token, RIL_Errno e,
                           const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int))) {
            mtkLogE(LOG_TAG, "volteSettingInd: invalid response");
            return 0;
        }

        int status = ((int32_t *) response)[0];
        bool isEnable = (status == 1) ? true : false;

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->volteSetting(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    isEnable);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                appendMtkImsind(slotId, RIL_UNSOL_VOLTE_SETTING, indicationType, response,
                            responseLen);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->volteSetting(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    isEnable);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                appendMtkImsind(tboxSlotId, RIL_UNSOL_VOLTE_SETTING, indicationType, response,
                            responseLen);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
            "volteSettingInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
            slotId);
        appendMtkImsind(slotId, RIL_UNSOL_VOLTE_SETTING, indicationType, response,
                    responseLen);
    }

    return 0;
}

int mtkRadioExIms::imsRegistrationInfoInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e,
                              const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int) * 2)) {
            mtkLogE(LOG_TAG, "imsRegistrationInfoInd: invalid response");
            return 0;
        }

        int status = ((int32_t *) response)[0];
        int capacity = ((int32_t *) response)[1];

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsRegistrationInfo(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                status, capacity);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
            "imsRegistrationInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
            slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int) * 2)) {
            mtkLogE(LOG_TAG, "tbox-imsRegistrationInfoInd: invalid response");
            return 0;
        }

        int status = ((int32_t *) response)[0];
        int capacity = ((int32_t *) response)[1];

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsRegistrationInfo(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                status, capacity);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
            "tbox-imsRegistrationInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
            tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::imsSupportEccInd(unsigned int slotId,
                     int indicationType, int token, RIL_Errno e,
                     const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < sizeof(int)) {
            mtkLogE(LOG_TAG, "imsSupportEccInd: invalid response");
            return 0;
        }

        int status = ((int32_t *) response)[0];

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsSupportEcc(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                status);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
            "imsSupportEccInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
            slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < sizeof(int)) {
            mtkLogE(LOG_TAG, "tbox-msSupportEccInd: invalid response");
            return 0;
        }

        int status = ((int32_t *) response)[0];

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsSupportEcc(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                status);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
            "tbox-imsSupportEccInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
            tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::sipRegInfoInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || (responseLen % sizeof(RIL_SipRegInfo*) != 0)) {
            mtkLogE(LOG_TAG, "sipRegInfoInd: invalid response:%p, responseLen:%zu",
                response, responseLen);
            return 0;
        }

        RIL_SipRegInfo* data = (RIL_SipRegInfo*)response;
        std::vector<std::string> info;
        if (data->cseq != NULL && data->reason_cause != NULL) {
            info.resize(9);
            info[5] = convertCharPtrToStdString(data->cseq);  //convertCharPtrToHidlString
            info[6] = convertCharPtrToStdString(data->reason_cause);
            info[7] = convertCharPtrToStdString(data->reason_text);
            info[8] = convertCharPtrToStdString(data->callId);
        } else {
            info.resize(5);
        }
        info[0] = convertCharPtrToStdString(data->direction); //convertCharPtrToHidlString
        info[1] = convertCharPtrToStdString(data->sip_msg_type);
        info[2] = convertCharPtrToStdString(data->method);
        info[3] = convertCharPtrToStdString(data->reason_phrase);
        info[4] = convertCharPtrToStdString(data->warn_text);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->sipRegInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    data->account_id,
                    data->response_code, info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->sipRegInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    data->account_id,
                    data->response_code, info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "sipRegInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExIms::imsRegistrationStateInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e,
                              const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "imsRegistrationStateInd: invalid response");
            return 0;
        }

        ImsRegStatusInfo info = {};
        RIL_ImsRegInfo *pImsInfo = (RIL_ImsRegInfo *)response;
        info.report_type = pImsInfo->report_type;
        info.account_id = pImsInfo->account_id;
        info.expire_time = pImsInfo->expire_time;
        info.error_code = pImsInfo->error_code;
        info.uri = convertCharPtrToStdString(pImsInfo->uri); //convertCharPtrToHidlString
        info.error_msg = convertCharPtrToStdString(pImsInfo->error_msg); //convertCharPtrToHidlString

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsRegStatusReport(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsRegStatusReport(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsRegistrationStateInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::eiregDataInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e,
                              const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "eiregDataInd: invalid response");
            return 0;
        }

        RIL_EiregData* data = (RIL_EiregData*)response;
        std::vector<int32_t> info;
        info.resize(7);
        info[0] = data->reg_state;
        info[1] = data->reg_type;
        info[2] = data->ext_info;
        info[3] = data->dereg_cause;
        info[4] = data->ims_retry;
        info[5] = data->rat;
        info[6] = data->sip_uri_type;

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsRegInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsRegInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "eiregDataInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::eregrtInfoInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e,
                              const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "eregrtInfoInd: invalid response");
            return 0;
        }

        RIL_EregrtInfo* data = (RIL_EregrtInfo*)response;
        std::vector<int32_t> info;
        info.resize(4);
        info[0] = data->rereg_flag;
        info[1] = data->rereg_remain_time;
        info[2] = data->resub_flag;
        info[3] = data->resub_remain_time;

        mtkLogI(LOG_TAG, "eregrtInfoInd: (%d, %d, %d, %d)", info[0], info[1], info[2], info[3]);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->eregrtInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->eregrtInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "eregrtInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::imsRegFlagInd(unsigned int slotId,
                           int indicationType, int token, RIL_Errno e,
                           const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int))) {
            mtkLogE(LOG_TAG, "imsRegFlagInd: invalid response");
            return 0;
        }

        int flag = ((int32_t *) response)[0];

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsRegFlagInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                flag);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "imsRegFlagInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                    slotId);
        appendMtkImsind(slotId, RIL_UNSOL_IMS_REG_FLAG_IND, indicationType, response,
                    responseLen);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int))) {
            mtkLogE(LOG_TAG, "imsRegFlagInd: invalid response");
            return 0;
        }

        int flag = ((int32_t *) response)[0];

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsRegFlagInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                flag);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-imsRegFlagInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                    tboxSlotId);
        appendMtkImsind(tboxSlotId, RIL_UNSOL_IMS_REG_FLAG_IND, indicationType, response,
                    responseLen);
    }
    return 0;
}

int mtkRadioExIms::imsRtpInfoInd(unsigned int slotId,
                         int indicationType, int token, RIL_Errno e, const void *response,
                         size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        std::string pdnId;
        std::string networkId;
        std::string timer;
        std::string sendPktLost;
        std::string recvPktLost;
        std::string jitter;
        std::string delay;
        int numStrings = responseLen / sizeof(char *);

        if (response == NULL || numStrings < 5) {
            mtkLogE(LOG_TAG, "imsRtpInfoInd Invalid response: NULL");
            return 0;
        } else {
            char **resp = (char **) response;
            pdnId = convertCharPtrToStdString(resp[0]);  // convertCharPtrToHidlString
            networkId = convertCharPtrToStdString(resp[1]);
            timer = convertCharPtrToStdString(resp[2]);
            sendPktLost = convertCharPtrToStdString(resp[3]);
            recvPktLost = convertCharPtrToStdString(resp[4]);
            jitter = convertCharPtrToStdString(resp[5]);
            delay = convertCharPtrToStdString(resp[6]); //convertCharPtrToHidlString
        }

        mtkLogD(LOG_TAG, "imsRtpInfoInd");

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsRtpInfo(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    pdnId, networkId, timer, sendPktLost, recvPktLost,
                    jitter, delay);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsRtpInfo(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    pdnId, networkId, timer, sendPktLost, recvPktLost,
                    jitter, delay);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "imsRtpInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                   slotId);
    }

    return 0;
}

int mtkRadioExIms::onXuiInd(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
   if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        std::string accountId;
        std::string broadcastFlag;
        std::string xuiInfo;
        int numStrings = responseLen / sizeof(char *);

        if (response == NULL || numStrings < 3) {
            mtkLogE(LOG_TAG, "onXuiInd Invalid response: NULL");
            return 0;
        } else {
            char **resp = (char **) response;
            accountId = convertCharPtrToStdString(resp[0]); //convertCharPtrToHidlString
            broadcastFlag = convertCharPtrToStdString(resp[1]); //convertCharPtrToHidlString
            xuiInfo = convertCharPtrToStdString(resp[2]); //convertCharPtrToHidlString
        }

        mtkLogD(LOG_TAG, "onXuiInd");

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->onXui(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    accountId, broadcastFlag, xuiInfo);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                appendMtkImsind(slotId, RIL_UNSOL_ON_XUI,
                        indicationType, response, responseLen);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->onXui(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                    accountId, broadcastFlag, xuiInfo);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                appendMtkImsind(tboxSlotId, RIL_UNSOL_ON_XUI,
                        indicationType, response, responseLen);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
            "onXuiInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
            slotId);
        appendMtkImsind(slotId, RIL_UNSOL_ON_XUI, indicationType, response,
                responseLen);
    }

    return 0;
}

int mtkRadioExIms::clearMtkImsResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioImsServiceImpl[realSlotId].mtkImsResponse = NULL;
    radioImsServiceImpl[realSlotId].mtkImsIndication = NULL;
    return 0;
}

// M: IMS_BASE end @}


// M: IMS_SS IND  start @{
int mtkRadioExIms::sendUssiResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "sendUssiResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if(clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->sendUssiResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioImsServiceImpl[%d].mtkImsResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int mtkRadioExIms::cancelUssiResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "cancelUssiResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if(clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->cancelUssiResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioImsServiceImpl[%d].mtkImsResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int mtkRadioExIms::getXcapStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getXcapStatusResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if(clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->getXcapStatusResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioImsServiceImpl[%d].mtkImsResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int mtkRadioExIms::setupXcapUserAgentStringResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setupXcapUserAgentStringResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if(clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->setupXcapUserAgentStringResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioImsServiceImpl[%d].mtkImsResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int mtkRadioExIms::getBarringCallsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getBarringCallsResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if(clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<aidl_ims_mtk::ims::ImsBarringCall> barringCall;

        if (response == NULL || responseLen % sizeof(RIL_BarringCallInfo *) != 0) {
            mtkLogE(LOG_TAG, "getBarringCallsResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int num = responseLen / sizeof(RIL_BarringCallInfo *);
            barringCall.resize(num);
            for (int i = 0 ; i < num; i++) {
                RIL_BarringCallInfo *resp = ((RIL_BarringCallInfo **) response)[i];
                barringCall[i].status = resp->status;
                barringCall[i].serviceClass = resp->serviceClass;
                barringCall[i].toa = resp->toa;
                barringCall[i].number = convertCharPtrToStdString(resp->number);
            }
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->getBarringCallsResponse(
                responseInfo, barringCall);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioImsServiceImpl[%d].mtkImsResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int mtkRadioExIms::setBarringCallsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setBarringCallsResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if(clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->setBarringCallsResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioImsServiceImpl[%d].mtkImsResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int mtkRadioExIms::onUssiInd(unsigned int slotId,
                     int indicationType, int token, RIL_Errno e, const void *response,
                     size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen != 2 * sizeof(char *)) {
            mtkLogE(LOG_TAG, "onUssiInd: invalid response");
            return 0;
        }

        char **strings = (char **) response;
        char *mode = strings[0];

        mtkLogD(LOG_TAG, "onUssiInd: mode %s, slotId %d", mode, slotId);

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->onUssi(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                atoi(mode), convertCharPtrToStdString(strings[1]));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onXuiInd: radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen != 2 * sizeof(char *)) {
            mtkLogE(LOG_TAG, "tbox-onUssiInd: invalid response");
            return 0;
        }

        char **strings = (char **) response;
        char *mode = strings[0];

        mtkLogD(LOG_TAG, "onUssiInd: mode %s, tboxSlotId %d", mode, tboxSlotId);

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->onUssi(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                atoi(mode), convertCharPtrToStdString(strings[1]));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onXuiInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onVolteSubscriptionInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < sizeof(int)) {
            mtkLogE(LOG_TAG, "onVolteSubscription: invalid response");
            return 0;
        }

        int status = ((int32_t *) response)[0];

        mtkLogD(LOG_TAG, "onVolteSubscriptionInd: status %d, slotId %d", status, slotId);

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->onVolteSubscription(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                status);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "onVolteSubscriptionInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < sizeof(int)) {
            mtkLogE(LOG_TAG, "tbox-onVolteSubscription: invalid response");
            return 0;
        }

        int status = ((int32_t *) response)[0];

        mtkLogD(LOG_TAG, "onVolteSubscriptionInd: status %d, tboxSlotId %d", status, tboxSlotId);

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->onVolteSubscription(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                status);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "onVolteSubscriptionInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }
    return 0;
}
// M: IMS_SS IND end @}


// M: IMS_ViLTE start @{
int mtkRadioExIms::imsVtDialResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "imsVtDialResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->vtDialResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "imsVtDialResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExIms::vtDialWithSipUriResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "vtDialWithSipUriResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->vtDialWithSipUriResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "vtDialWithSipUriResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                              slotId);
    }

    return 0;
}

int mtkRadioExIms::videoCallAcceptResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "videoCallAcceptResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->videoCallAcceptResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "videoCallAcceptResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                           slotId);
    }
    return 0;
}

int mtkRadioExIms::videoRingtoneEventResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogI(LOG_TAG, "videoRingtoneEventResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->videoRingtoneEventResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "videoRingtoneEventResponse: radioImsServiceImpl[%d].mtkImsResponse"
                " == NULL", slotId);
    }
    return 0;
}

int mtkRadioExIms::videoCapabilityIndicatorInd(unsigned int slotId,
                                       int indicationType, int token, RIL_Errno e,
                                       const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL ) {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        if(numStrings < 3) {
            mtkLogE(LOG_TAG, "videoCapabilityIndicatorInd: items length invalid, slotId = %d",
                                                                        slotId);
            return 0;
        }

        std::string callId = convertCharPtrToStdString(resp[0]);
        std::string localVideoCaoability = convertCharPtrToStdString(resp[1]);
        std::string remoteVideoCaoability = convertCharPtrToStdString(resp[2]);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->videoCapabilityIndicator(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, localVideoCaoability, remoteVideoCaoability);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->videoCapabilityIndicator(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, localVideoCaoability, remoteVideoCaoability);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "videoCapabilityIndicatorInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                                slotId);
    }

    return 0;
}

int mtkRadioExIms::videoRingtoneEventInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if ((response == NULL && responseLen != 0) || responseLen % sizeof(char*) != 0) {
            mtkLogE(LOG_TAG, "videoRingtoneEventInd: invalid response");
            return 0;
        }
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        std::vector<std::string> data;
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
            mtkLogD(LOG_TAG, "imsVideoRingtoneEventInd:: %d: %s", i, resp[i]);
        }

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->
                    videoRingtoneEventInd(rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->
                    videoRingtoneEventInd(rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "imsVideoRingtoneEventInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }
   return 0;
}

int mtkRadioExIms::mdInternetUsageIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "mdInternetUsageIndication: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "mdInternetUsageIndication");

        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);

        std::vector<int32_t> data;
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
              data[i] = (int32_t) pInt[i];
        }

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->onMDInternetUsageInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->onMDInternetUsageInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "mdInternetUsageIndication: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    return 0;
}
// M: IMS_ViLTE end @}

// M: IMS_CC start @{
int mtkRadioExIms::dialWithSipUriResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "dialWithSipUriResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 dialWithSipUriResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "dialWithSipUriResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::imsEctCommandResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "imsEctCommandResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->imsEctCommandResponse(responseInfo);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "imsEctCommandResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                           slotId);
    }
    return 0;
}

int mtkRadioExIms::pullCallResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "pullCallResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->pullCallResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "pullCallResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                      slotId);
    }
    return 0;
}

int mtkRadioExIms::eccRedialApproveResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "eccRedialApproveResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->eccRedialApproveResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "eccRedialApproveResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                           slotId);
    }
    return 0;
}

int mtkRadioExIms::conferenceDialResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "conferenceDialResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->conferenceDialResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "conferenceDialResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                            slotId);
    }

    return 0;
}

int mtkRadioExIms::forceReleaseCallResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "forceReleaseCallResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->forceReleaseCallResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "forceReleaseCallResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                              slotId);
    }
    return 0;
}

int mtkRadioExIms::controlCallResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    unsigned int realSlotId = (clientId == android::CLIENT_IMS) ? (slotId % MAX_SIM_COUNT) : slotId;
    mtkLogD(LOG_TAG, "controlCallResponse: serial %d clientId %d slotId %d realSlotId %d", serial, clientId, slotId, realSlotId);
    if ((clientId == android::CLIENT_IMS || clientId == android::CLIENT_TBOX_IMS) && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->controlCallResponse(responseInfo);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "controlCallResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                      slotId);
    }
    return 0;
}

int mtkRadioExIms::controlImsConferenceCallMemberResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "controlImsConfCallMemberRsp: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
            radioImsServiceImpl[realSlotId].mtkImsResponse->controlImsConferenceCallMemberResponse(responseInfo);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "controlImsConfCallMemberRsp: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                             slotId);
    }

    return 0;
}

int mtkRadioExIms::setSipHeaderResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "setSipHeaderResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 setSipHeaderResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setSipHeaderResponse: radioService[%d]->mRadioResponseIms == NULL",
                                                                            slotId);
    }

    return 0;
}

int mtkRadioExIms::setSipHeaderReportResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "setSipHeaderReportResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 setSipHeaderReportResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setSipHeaderReportResponse: radioService[%d]->mRadioResponseIms == NULL",
                                                                            slotId);
    }

    return 0;
}

int mtkRadioExIms::setImsCallModeResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "setImsCallModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 setImsCallModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setImsCallModeResponse: radioService[%d]->mRadioResponseIms == NULL",
                                                                            slotId);
    }

    return 0;
}

int mtkRadioExIms::setCallAdditionalInfoResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogI(LOG_TAG, "setCallAdditionalInfoResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                setCallAdditionalInfoResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setCallAdditionalInfoResponse: radioImsServiceImpl[%d].mtkImsResponse"
                " == NULL", slotId);
    }

    return 0;
}

int mtkRadioExIms::ectIndicationInd(unsigned int slotId,
                            int indicationType, int token, RIL_Errno e,
                            const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int) * 3)) {
            mtkLogE(LOG_TAG, "ectIndicationInd: invalid response");
            return 0;
        }

        int callId = ((int32_t *) response)[0];
        int ectResult = ((int32_t *) response)[1];
        int cause = ((int32_t *) response)[2];

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->ectIndication(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 callId, ectResult, cause);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "ectIndicationInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                     slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int) * 3)) {
            mtkLogE(LOG_TAG, "tbox-ectIndicationInd: invalid response");
            return 0;
        }

        int callId = ((int32_t *) response)[0];
        int ectResult = ((int32_t *) response)[1];
        int cause = ((int32_t *) response)[2];

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->ectIndication(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 callId, ectResult, cause);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "ectIndicationInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                     tboxSlotId);
    }

    return 0;
}

int mtkRadioExIms::callInfoIndicationInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e,
                                 const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    char **resp = (char **) response;
    int numStrings = responseLen / sizeof(char *);
    if(numStrings < 5) {
        mtkLogE(LOG_TAG, "callInfoIndicationInd: items length is invalid, slot = %d", slotId);
        return 0;
    }

    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        std::vector<std::string> data;
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
            // mtkLogD(LOG_TAG, "callInfoIndicationInd:: %d: %s", i, resp[i]);
        }

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->callInfoIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     data);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->callInfoIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     data);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "callInfoIndicationInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                          slotId);
    }

    return 0;
}

int mtkRadioExIms::callmodChangeIndicatorInd(unsigned int slotId,
                                    int indicationType, int token, RIL_Errno e,
                                    const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        if(numStrings < 5) {
            mtkLogE(LOG_TAG, "callmodChangeIndicatorInd: items length invalid, slotId = %d",
                                                                       slotId);
            return 0;
        }

        std::string callId = convertCharPtrToStdString(resp[0]);
        std::string callMode = convertCharPtrToStdString(resp[1]);
        std::string videoState = convertCharPtrToStdString(resp[2]);
        std::string autoDirection = convertCharPtrToStdString(resp[3]);
        std::string pau = convertCharPtrToStdString(resp[4]);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->callmodChangeIndicator(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, callMode, videoState, autoDirection, pau);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }


        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->callmodChangeIndicator(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, callMode, videoState, autoDirection, pau);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "callmodChangeIndicatorInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                              slotId);
    }

    return 0;
}

int mtkRadioExIms::econfResultIndicationInd(unsigned int slotId,
                                    int indicationType, int token, RIL_Errno e,
                                    const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        if(numStrings < 5) {
            mtkLogE(LOG_TAG, "econfResultIndicationInd: items length invalid, slotId = %d",
                                                                     slotId);
            return 0;
        }

        std::string confCallId = convertCharPtrToStdString(resp[0]);
        std::string op = convertCharPtrToStdString(resp[1]);
        std::string num = convertCharPtrToStdString(resp[2]);
        std::string result = convertCharPtrToStdString(resp[3]);
        std::string cause = convertCharPtrToStdString(resp[4]);
        std::string joinedCallId;
        if(numStrings > 5) {
            joinedCallId = convertCharPtrToStdString(resp[5]);
        }

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->econfResultIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     confCallId, op, num, result, cause, joinedCallId);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->econfResultIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     confCallId, op, num, result, cause, joinedCallId);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "econfResultIndicationInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                             slotId);
    }

    return 0;
}

int mtkRadioExIms::sipCallProgressIndicatorInd(unsigned int slotId,
                                       int indicationType, int token, RIL_Errno e,
                                       const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        if(numStrings < 5) {
            mtkLogE(LOG_TAG, "sipCallProgressIndicatorInd: items length invalid, slotId = %d",
                                                                        slotId);
            return 0;
        }

        std::string callId = convertCharPtrToStdString(resp[0]);
        std::string dir = convertCharPtrToStdString(resp[1]);
        std::string sipMsgType = convertCharPtrToStdString(resp[2]);
        std::string method = convertCharPtrToStdString(resp[3]);
        std::string responseCode = convertCharPtrToStdString(resp[4]);
        std::string reasonText;
        if(numStrings > 5) {
            reasonText = convertCharPtrToStdString(resp[5]);
        }

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->sipCallProgressIndicator(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, dir, sipMsgType, method, responseCode, reasonText);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->sipCallProgressIndicator(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, dir, sipMsgType, method, responseCode, reasonText);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    }
    else {
        mtkLogE(LOG_TAG, "sipCallProgressIndicatorInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                                slotId);
    }

    return 0;
}
int mtkRadioExIms::onImsConferenceInfoIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        std::vector<ImsConfParticipant> participants;
        if (response == NULL || responseLen % sizeof(RIL_Conference_Participants) != 0) {
            mtkLogE(LOG_TAG, "imsConferenceInfoIndication Invalid response: NULL");
            return 0;
        } else {
            int num = responseLen / sizeof(RIL_Conference_Participants);
            participants.resize(num);
            for (int i = 0; i< num; i++) {
                RIL_Conference_Participants p_cur = ((RIL_Conference_Participants *) response)[i];
                participants[i].user_addr = convertCharPtrToStdString(p_cur.useraddr);
                participants[i].end_point = convertCharPtrToStdString(p_cur.end_point);
                participants[i].entity = convertCharPtrToStdString(p_cur.entity);
                participants[i].display_text = convertCharPtrToStdString(p_cur.display_text);
                participants[i].status = convertCharPtrToStdString(p_cur.status);
            }
        }

        mtkLogD(LOG_TAG, "imsConferenceInfoIndication");

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsConferenceInfoIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), participants);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsConferenceInfoIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), participants);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "imsConferenceInfoIndication: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                              slotId);
    }

    return 0;
}

int mtkRadioExIms::onLteMessageWaitingIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        std::string callId;
        std::string pType;
        std::string urcIdx;
        std::string totalUrcCount;
        std::string rawData;
        int numStrings = responseLen / sizeof(char *);

        if (response == NULL || numStrings < 5) {
            mtkLogE(LOG_TAG, "lteMessageWaitingIndication Invalid response: NULL");
            return 0;
        } else {
            char **resp = (char **) response;
            callId = convertCharPtrToStdString(resp[0]);
            pType = convertCharPtrToStdString(resp[1]);
            urcIdx = convertCharPtrToStdString(resp[2]);
            totalUrcCount = convertCharPtrToStdString(resp[3]);
            rawData = convertCharPtrToStdString(resp[4]);
        }

        mtkLogD(LOG_TAG, "lteMessageWaitingIndication");

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->lteMessageWaitingIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, pType, urcIdx, totalUrcCount, rawData);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->lteMessageWaitingIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, pType, urcIdx, totalUrcCount, rawData);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "lteMessageWaitingIndication: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                              slotId);
    }

    return 0;
}

int mtkRadioExIms::imsDialogIndicationInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        mtkLogE(LOG_TAG, "imsDialogIndicationInd: response:%p, responseLen:%lu"
                , response, responseLen);
        if ((response == NULL && responseLen != 0) || responseLen % sizeof(RIL_DialogInfo*) != 0) {
            mtkLogE(LOG_TAG, "imsDialogIndicationInd: invalid response");
            return 0;
        }

        std::vector<Dialog> dialogList;
        int num = responseLen / sizeof(RIL_DialogInfo*);
        mtkLogE(LOG_TAG, "imsDialogIndicationInd num:%d", num);

        dialogList.resize(num);
        for (int i = 0; i < num; i++) {
            RIL_DialogInfo *dialog = ((RIL_DialogInfo **) response)[i];
            dialogList[i].dialogId = dialog->dialogId;
            dialogList[i].callState = dialog->callState;
            dialogList[i].callType = dialog->callType;
            dialogList[i].isCallHeld = dialog->isCallHeld;
            dialogList[i].isPullable = dialog->isPullable;
            dialogList[i].isMt = dialog->isMt;
            dialogList[i].address = convertCharPtrToStdString(dialog->address);
            dialogList[i].remoteAddress = convertCharPtrToStdString(dialog->remoteAddress);
            mtkLogE(LOG_TAG,"imsDialogIndicationInd: dialog[%d] Id:%d", i, dialogList[i].dialogId);
        }

        mtkLogE(LOG_TAG, "imsDialogIndicationInd");

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsDialogIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dialogList);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsDialogIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dialogList);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "imsDialogIndicationInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::noEmergencyCallbackModeInd(unsigned int slotId,
                                        int indicationType, int token, RIL_Errno e, const void *response,
                                        size_t responseLen) {
    // Currently only sync noEmergencyCallbackModeInd to IMS
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->
                                 noEmergencyCallbackMode(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "noEmergencyCallbackModeInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                            slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    // Currently only sync noEmergencyCallbackModeInd to IMS
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->
                                 noEmergencyCallbackMode(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "noEmergencyCallbackModeInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                            tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::redialEmergencyIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "redialEmergencyIndication: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "redialEmergencyIndication");

        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        if(numStrings < 1) {
            mtkLogE(LOG_TAG, "redialEmergencyIndication: items length invalid, slotId = %d", slotId);
            return 0;
        }

        std::string callId = convertCharPtrToStdString(resp[0]);

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->
                                 imsRedialEmergencyIndication(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 callId);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "redialEmergencyIndication: radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "redialEmergencyIndication: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "redialEmergencyIndication");

        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        if(numStrings < 1) {
            mtkLogE(LOG_TAG, "redialEmergencyIndication: items length invalid, slotId = %d", slotId);
            return 0;
        }

        std::string callId = convertCharPtrToStdString(resp[0]);

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->
                                 imsRedialEmergencyIndication(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 callId);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "redialEmergencyIndication: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL", tboxSlotId);
    }

    return 0;
}

int mtkRadioExIms::speechCodecInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "ims speechCodecInfoInd: invalid response");
            return 0;
        }
        int32_t info = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "ims speechCodecInfoInd: %d", info);
        ndk::ScopedAStatus ret =
                radioImsServiceImpl[slotId].mtkImsIndication->speechCodecInfoIndication(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "ims speechCodecInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "tbox ims speechCodecInfoInd: invalid response");
            return 0;
        }
        int32_t info = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "ims speechCodecInfoInd: %d", info);
        ndk::ScopedAStatus ret =
                radioImsServiceImpl[tboxSlotId].mtkImsIndication->speechCodecInfoIndication(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "ims speechCodecInfoInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::imsEventPackageIndicationInd(unsigned int slotId,
                                        int indicationType, int token, RIL_Errno e,
                                        const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        std::string callId;
        std::string pType;
        std::string urcIdx;
        std::string totalUrcCount;
        std::string rawData;
        int numStrings = responseLen / sizeof(char *);

        if (response == NULL || numStrings < 5) {
            mtkLogE(LOG_TAG, "imsEventPackageIndication Invalid response: NULL");
            return 0;
        } else {
            char **resp = (char **) response;
            callId = convertCharPtrToStdString(resp[0]);
            pType = convertCharPtrToStdString(resp[1]);
            urcIdx = convertCharPtrToStdString(resp[2]);
            totalUrcCount = convertCharPtrToStdString(resp[3]);
            rawData = convertCharPtrToStdString(resp[4]);
        }

        mtkLogD(LOG_TAG, "imsEventPackageIndication");

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsEventPackageIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, pType, urcIdx, totalUrcCount, rawData);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsEventPackageIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     callId, pType, urcIdx, totalUrcCount, rawData);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "imsEventPackageIndication: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                                                                              slotId);
    }

    return 0;
}

int mtkRadioExIms::sipHeaderReportInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e,
                                 const void *response, size_t responseLen) {
    char **resp = (char **) response;
    int numStrings = responseLen / sizeof(char *);
    if(numStrings < 5) {
        mtkLogE(LOG_TAG, "sipHeaderReportInd: items length is invalid, slot = %d", slotId);
        return 0;
    }

    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        std::vector<std::string>data;
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
            mtkLogD(LOG_TAG, "sipHeaderReportInd:: %d: %s", i, resp[i]);
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->sipHeaderReport(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 data);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sipHeaderReportInd: radioService[%d]->mRadioIndicationIms == NULL",
                                                                          slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        std::vector<std::string>data;
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
            mtkLogD(LOG_TAG, "sipHeaderReportInd:: %d: %s", i, resp[i]);
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->sipHeaderReport(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 data);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sipHeaderReportInd: tbox-radioService[%d]->mRadioIndicationIms == NULL",
                                                                          tboxSlotId);
    }

    return 0;
}

int mtkRadioExIms::callRatIndication(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e,
                                 const void *response, size_t responseLen) {
    int *resp = (int *) response;
    int numInts = responseLen / sizeof(int);
    if (numInts < 1) {
        mtkLogE(LOG_TAG, "callRatIndication: items length invalid, slotId = %d", slotId);
        return 0;
    }

    int domain = ((int32_t *) resp)[0];
    int rat = 0;
    if (numInts >= 2) {
        rat = ((int32_t *) resp)[1];
    }

    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->callRatIndication(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 domain, rat);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "callRatIndication: radioService[%d]->mRadioIndicationIms == NULL",
                                                                          slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->callRatIndication(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 domain, rat);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "callRatIndication: tbox-radioService[%d]->mRadioIndicationIms == NULL",
                                                                          tboxSlotId);
    }

    return 0;
}

int mtkRadioExIms::emergencyBearerInfoInd(unsigned int slotId, int indicationType,
                      int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "emergencyBearerInfoInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "emergencyBearerInfoInd");
        int32_t s1Support = ((int32_t *) response)[0];
        // We use same interface to update ECC support or not.
        // So value will be transformed here.
        // 0 => 2
        // 1 => 3
        s1Support += 2;
        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->
                                 imsSupportEcc(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 s1Support);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            appendMtkImsind(slotId, RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY,
                    indicationType, response, responseLen);
        }
    } else {
        mtkLogE(LOG_TAG, "emergencyBearerInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL", slotId);
        appendMtkImsind(slotId, RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY,
                indicationType, response, responseLen);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "emergencyBearerInfoInd: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "emergencyBearerInfoInd");
        int32_t s1Support = ((int32_t *) response)[0];
        // We use same interface to update ECC support or not.
        // So value will be transformed here.
        // 0 => 2
        // 1 => 3
        s1Support += 2;
        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->
                                 imsSupportEcc(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 s1Support);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            appendMtkImsind(tboxSlotId, RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY,
                    indicationType, response, responseLen);
        }
    } else {
        mtkLogE(LOG_TAG, "emergencyBearerInfoInd: radioImsServiceImpl[%d].mtkImsIndication == NULL", tboxSlotId);
        appendMtkImsind(tboxSlotId, RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY,
                indicationType, response, responseLen);
    }

    return 0;
}

int mtkRadioExIms::incomingCallSipInviteInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL ||
            radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        if (response == NULL || numStrings < 2) {
            mtkLogE(LOG_TAG, "incomingCallSipInviteInd Invalid response: NULL");
            return 0;
        }

        std::string number = convertCharPtrToStdString(resp[0]);
        std::string content = convertCharPtrToStdString(resp[1]);

        if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
            mtkLogD(LOG_TAG, "incomingCallSipInviteInd");
            ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->
                                     incomingCallSipInviteIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     number, content);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
            }
        }

        if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
            mtkLogD(LOG_TAG, "incomingCallSipInviteInd");
            ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->
                                     incomingCallSipInviteIndication(
                                     rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                     number, content);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(
                    slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG,
                "incomingCallSipInviteInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }
    return 0;
}


// M: IMS_CC end @}

// M: IMS_CONFIG  start @{
int mtkRadioExIms::setImscfgResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    unsigned int realSlotId = (clientId == android::CLIENT_IMS) ? (slotId % MAX_SIM_COUNT) : slotId;
    mtkLogD(LOG_TAG, "setImscfgResponse: serial %d clientId %d slotId %d realSlotId %d", serial, clientId, slotId, realSlotId);
    if (clientId == android::CLIENT_IMS || clientId == android::CLIENT_TBOX_IMS) {
        if (radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
            RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ndk::ScopedAStatus ret =
                radioImsServiceImpl[realSlotId].mtkImsResponse->setImscfgResponse(responseInfo);

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "setImscfgResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL", slotId);
        }
    }
    return 0;
}

int mtkRadioExIms::getImscfgResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    unsigned int realSlotId = (clientId == android::CLIENT_IMS) ? (slotId % MAX_SIM_COUNT) : slotId;
    mtkLogD(LOG_TAG, "getImscfgResponse: serial %d clientId %d slotId %d realSlotId %d", serial, clientId, slotId, realSlotId);
    if (clientId == android::CLIENT_IMS || clientId == android::CLIENT_TBOX_IMS) {
        if (radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
            RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
            ndk::ScopedAStatus ret =
                radioImsServiceImpl[realSlotId].mtkImsResponse->getImscfgResponse(responseInfo, convertCharPtrToStdString((char *) response));

            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "controlCallResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL", slotId);
        }
    }
    return 0;
}

int mtkRadioExIms::setModemImsCfgResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setModemImsCfgResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->setModemImsCfgResponse(
                responseInfo,
                convertCharPtrToStdString((char *) response)); // convertCharPtrToHidlString
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setModemImsCfgResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::setImsCfgFeatureValueResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setImsCfgFeatureValueResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                setImsCfgFeatureValueResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setImsCfgFeatureValueResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::getImsCfgFeatureValueResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "getImsCfgFeatureValueResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int value = imsResponseIntOrEmpty(responseInfo, serial, responseType, e, response,
                                       responselen);
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                getImsCfgFeatureValueResponse(responseInfo, value);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getImsCfgFeatureValueResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::setImsCfgProvisionValueResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setImsCfgProvisionValueResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                setImsCfgProvisionValueResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "setImsCfgProvisionValueResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::getImsCfgProvisionValueResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "getImsCfgProvisionValueResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                getImsCfgProvisionValueResponse(
                responseInfo,
                convertCharPtrToStdString((char *) response)); // convertCharPtrToHidlString
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getImsCfgProvisionValueResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::getImsCfgResourceCapValueResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "getImsCfgResourceCapValueResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int value = imsResponseIntOrEmpty(responseInfo, serial, responseType, e, response,
                                       responselen);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                getImsCfgResourceCapValueResponse(responseInfo, value);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getImsCfgResourceCapValueResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::setWfcProfileResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setWfcProfileResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 setWfcProfileResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setWfcProfileResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                                                                           slotId);
    }

    return 0;
}

int mtkRadioExIms::setVoiceDomainPreferenceResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "setVoiceDomainPreferenceResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 setVoiceDomainPreferenceResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setVoiceDomainPreferenceResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::getVoiceDomainPreferenceResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "getVoiceDomainPreferenceResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_IMS && radioImsServiceImpl[realSlotId].mtkImsResponse!= NULL) {
        RadioResponseInfo responseInfo = {};
        int rspRet = imsResponseInt(responseInfo, serial, responseType, e, response, responselen);

        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse->
                                 getVoiceDomainPreferenceResponse(responseInfo, rspRet);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getVoiceDomainPreferenceResponse: radioImsServiceImpl[%d].mtkImsResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExIms::imsCfgDynamicImsSwitchCompleteInd(unsigned int slotId,
                                             int indicationType, int token, RIL_Errno e,
                                             const void *response,
                                             size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        mtkLogD(LOG_TAG, "imsCfgDynamicImsSwitchCompleteInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsCfgDynamicImsSwitchComplete(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgDynamicImsSwitchCompleteInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        mtkLogD(LOG_TAG, "tbox-imsCfgDynamicImsSwitchCompleteInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsCfgDynamicImsSwitchComplete(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgDynamicImsSwitchCompleteInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL",
                tboxSlotId);
    }    
    return 0;
}

int mtkRadioExIms::imsCfgFeatureChangedInd(unsigned int slotId,
                                   int indicationType, int token, RIL_Errno e, const void *response,
                                   size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        int feature_id = ((int32_t *) response)[0];
        int value = ((int32_t *) response)[1];
        mtkLogD(LOG_TAG, "imsCfgFeatureChangedInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsCfgFeatureChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), slotId, feature_id, value);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgFeatureChangedInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        int feature_id = ((int32_t *) response)[0];
        int value = ((int32_t *) response)[1];
        mtkLogD(LOG_TAG, "imsCfgFeatureChangedInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsCfgFeatureChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), slotId, feature_id, value);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgFeatureChangedInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExIms::imsCfgConfigChangedInd(unsigned int slotId,
                                   int indicationType, int token, RIL_Errno e, const void *response,
                                   size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        char** data = (char**) response;
        std::string config_id;
        std::string value;
        config_id = convertCharPtrToStdString(data[0]); //convertCharPtrToHidlString
        value = convertCharPtrToStdString(data[1]); //convertCharPtrToHidlString
        mtkLogD(LOG_TAG, "imsCfgConfigChangedInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsCfgConfigChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                slotId, config_id, value);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgConfigChangedInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        char** data = (char**) response;
        std::string config_id;
        std::string value;
        config_id = convertCharPtrToStdString(data[0]); //convertCharPtrToHidlString
        value = convertCharPtrToStdString(data[1]); //convertCharPtrToHidlString
        mtkLogD(LOG_TAG, "imsCfgConfigChangedInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsCfgConfigChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                slotId, config_id, value);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgConfigChangedInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::imsCfgConfigLoadedInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e,
                                 const void *response,
                                 size_t responseLen) {
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        mtkLogD(LOG_TAG, "imsCfgConfigLoadedInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->imsCfgConfigLoaded(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgConfigLoadedInd: radioImsServiceImpl[%d].mtkImsIndication == NULL",
                slotId);
    }

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        mtkLogD(LOG_TAG, "imsCfgConfigLoadedInd");

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->imsCfgConfigLoaded(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "imsCfgConfigLoadedInd: tbox-radioImsServiceImpl[%d].mtkImsIndication == NULL",
                tboxSlotId);
    }

    return 0;
}
// M: IMS_CONFIG end @}

// M: IMS_WFC start @{
int mtkRadioExIms::setWifiEnabledResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setWifiEnabledResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setWifiEnabledResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setWifiAssociatedResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setWifiAssociatedResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setWifiAssociatedResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setWfcConfigResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setWfcConfigResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setWfcConfigResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::getWfcConfigResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "getWfcConfigResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            int state = rilAidlUtils::responseIntOrEmpty(responseInfo, serial,
                    responseType, err, response, responseLen);
            mtkLogD(LOG_TAG, "[%s] state:%d", __FUNCTION__, state);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->getWfcConfigResponse(responseInfo, state);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setWifiSignalLevelResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setWifiSignalLevelResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setWifiSignalLevelResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setWifiIpAddressResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setWifiIpAddressResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setWifiIpAddressResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setLocationInfoResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setLocationInfoResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setLocationInfoResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setEmergencyAddressIdResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setEmergencyAddressIdResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setEmergencyAddressIdResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setNattKeepAliveStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setNattKeepAliveStatusResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setNattKeepAliveStatusResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::setWifiPingResultResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "setWifiPingResultResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->setWifiPingResultResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::notifyEPDGScreenStateResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen){
    mtkLogD(LOG_TAG, "notifyEPDGScreenStateResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioMwiServiceImpl[realSlotId].mtkMwiResponse != NULL) {
        if (clientId == android::CLIENT_IMS) {
            aidl_radio::RadioResponseInfo responseInfo = {};
            rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
            ndk::ScopedAStatus ret = radioMwiServiceImpl[realSlotId].mtkMwiResponse
                    ->notifyEPDGScreenStateResponse(responseInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_WFC);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: wrong clinet id = %d", __func__, clientId);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::querySsacStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "querySsacStatusResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioImsServiceImpl[realSlotId].mtkImsResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);
        std::vector<int32_t> status;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "querySsacStatusResponse Invalid response: NULL");
            if (err == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            int *pInt = (int *) response;
            int numInts = responseLen / sizeof(int);
            status.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                status[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioImsServiceImpl[realSlotId].mtkImsResponse
                ->querySsacStatusResponse(responseInfo, status);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioImsServiceImpl[%d].mtkImsResponse == NULL", __func__, realSlotId);
    }
    return 0;
}

int mtkRadioExIms::onWifiMonitoringThreshouldChanged(unsigned int slotId, int indicationType,
        int token, RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWifiMonitoringThreshouldChanged slotId: %d, responseLen: %zu, numInts: %d",
                slotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWifiMonitoringThreshouldChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWifiMonitoringThreshouldChanged tboxSlotId: %d, responseLen: %zu, numInts: %d",
                tboxSlotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onWifiMonitoringThreshouldChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }

    return 0;
}

int mtkRadioExIms::onWifiPdnActivate(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWifiPdnActivate slotId: %d, responseLen: %zu, numInts: %d",
                slotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWifiPdnActivate(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWifiPdnActivate tboxSlotId: %d, responseLen: %zu, numInts: %d",
                tboxSlotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onWifiPdnActivate(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }
    return 0;
}

int mtkRadioExIms::onWfcPdnError(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWfcPdnError slotId: %d, responseLen: %zu, numInts: %d",
                slotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWfcPdnError(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWfcPdnError tboxSlotId: %d, responseLen: %zu, numInts: %d",
                tboxSlotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onWfcPdnError(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onPdnHandover(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onPdnHandover slotId: %d, responseLen: %zu, numInts: %d",
                slotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onPdnHandover(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onPdnHandover tboxSlotId: %d, responseLen: %zu, numInts: %d",
                tboxSlotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onPdnHandover(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }

    return 0;
}

int mtkRadioExIms::onWifiRoveout(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onWifiRoveout slotId: %d, responseLen: %zu, numStrings: %d",
                slotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWifiRoveout(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onWifiRoveout tboxSlotId: %d, responseLen: %zu, numStrings: %d",
                tboxSlotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onWifiRoveout(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onLocationRequest(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onLocationRequest slotId: %d, responseLen: %zu, numStrings: %d",
                slotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onLocationRequest(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onLocationRequest tboxSlotId: %d, responseLen: %zu, numStrings: %d",
                tboxSlotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onLocationRequest(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onWfcPdnStateChanged(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWfcPdnStateChanged slotId: %d, responseLen: %zu, numInts: %d",
                slotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWfcPdnStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWfcPdnStateChanged tboxSlotId: %d, responseLen: %zu, numInts: %d",
                tboxSlotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onWfcPdnStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onNattKeepAliveChanged(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onNattKeepAliveChanged slotId: %d, responseLen: %zu, numStrings: %d",
                slotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onNattKeepAliveChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onNattKeepAliveChanged tboxSlotId: %d, responseLen: %zu, numStrings: %d",
                tboxSlotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onNattKeepAliveChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onWifiPingRequest(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWifiPingRequest slotId: %d, responseLen: %zu, numInts: %d",
                slotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWifiPingRequest(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onWifiPingRequest tboxSlotId: %d, responseLen: %zu, numInts: %d",
                tboxSlotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onWifiPingRequest(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onWifiPdnOOS(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onWifiPdnOOS slotId: %d, responseLen: %zu, numStrings: %d",
                slotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWifiPdnOOS(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onWifiPdnOOS tboxSlotId: %d, responseLen: %zu, numStrings: %d",
                tboxSlotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[tboxSlotId].mtkMwiIndication->onWifiPdnOOS(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onWifiLock(unsigned int slotId, int indicationType,
        int token, RIL_Errno err, const void *response, size_t responseLen){
    if (radioMwiServiceImpl[slotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onWifiLock slotId: %d, responseLen: %zu, numStrings: %d",
                slotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWifiLock(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMwiServiceImpl[tboxSlotId].mtkMwiIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "%s:  tbox-invalid response", __func__);
            return 0;
        }

        std::vector<std::string> indStgs;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        mtkLogD(LOG_TAG, "onWifiLock tboxSlotId: %d, responseLen: %zu, numStrings: %d",
                tboxSlotId, responseLen, numStrings);
        indStgs.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            indStgs[i] = convertCharPtrToStdString(resp[i]);
        }

        ndk::ScopedAStatus ret = radioMwiServiceImpl[slotId].mtkMwiIndication->onWifiLock(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_WFC);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::onSsacStatus(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen){
    if (radioImsServiceImpl[slotId].mtkImsIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onSsacStatus slotId: %d, responseLen: %zu, numInts: %d",
                slotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[slotId].mtkImsIndication->onSsacStatus(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_IMS * MAX_SIM_COUNT, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioImsServiceImpl[tboxSlotId].mtkImsIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: tbox-invalid response", __func__);
            return 0;
        }

        std::vector<int32_t> indStgs;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "onSsacStatus tboxSlotId: %d, responseLen: %zu, numInts: %d",
                tboxSlotId, responseLen, numInts);
        indStgs.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            indStgs[i] = (int32_t) pInt[i];
        }

        ndk::ScopedAStatus ret = radioImsServiceImpl[tboxSlotId].mtkImsIndication->onSsacStatus(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indStgs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __func__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT, ret, android::TBOX_IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioMwiServiceImpl[%d].mtkMwiIndication == NULL", __func__, tboxSlotId);
    }
    return 0;
}

int mtkRadioExIms::clearMwiResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioMwiServiceImpl[realSlotId].mtkMwiResponse = NULL;
    radioMwiServiceImpl[realSlotId].mtkMwiIndication = NULL;
    return 0;
}
// M: IMS_WFC end @}

namespace vendor::mediatek::hardware::mtkradioex::ims {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::ims;
namespace aidl_messaging = ::aidl::android::hardware::radio::messaging;
constexpr auto ok = &ScopedAStatus::ok;

MtkRadioIms::MtkRadioIms(int slotid) {
    mSlotId = slotid;
}

// M: IMS_DATA start @{
ScopedAStatus MtkRadioIms::imsBearerStateConfirm(int32_t serial, int32_t aid, int32_t action, int32_t status, int32_t clientId) {
    mtkLogD(LOG_TAG, "responseBearerStateConfirm: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_BEARER_STATE_CONFIRM, 3, aid, action, status);
    return ok();
}

ScopedAStatus MtkRadioIms::setImsBearerNotification(int32_t serial, int32_t enable, int32_t clientId) {
    mtkLogD(LOG_TAG, "setImsBearerNotification: serial:  %d, enable: %d", serial, enable);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_IMS_BEARER_NOTIFICATION, 1, enable);
    return ok();
}
// M: IMS_DATA end @}

// M: IMS_SMS start @{
ScopedAStatus MtkRadioIms::acknowledgeLastIncomingCdmaSmsEx(int32_t serial,
        const aidl_messaging::CdmaSmsAck& smsAck, int32_t clientId) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingCdmaSmsEx: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    android::RequestInfo *pRI = android::addRequestToList(
            serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX);
    if (pRI == NULL) {
        return ok();
    }

    RIL_CDMA_SMS_Ack rcsa = {};

    rcsa.uErrorClass = (RIL_CDMA_SMS_ErrorClass) smsAck.errorClass;
    rcsa.uSMSCauseCode = smsAck.smsCauseCode;

    CALL_ONREQUEST(pRI->pCI->requestNumber, &rcsa, sizeof(rcsa), pRI, pRI->socket_id);
    return ok();
}

ScopedAStatus MtkRadioIms::acknowledgeLastIncomingGsmSmsEx(int32_t serial, bool success,
        aidl_messaging::SmsAcknowledgeFailCause cause, int32_t clientId) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingGsmSmsEx: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SMS_ACKNOWLEDGE_EX, 2,
            BOOL_TO_INT(success), cause);
    return ok();
}

bool dispatchImsGsmSmsEx(const aidl_messaging::ImsSmsMessage& message, android::RequestInfo *pRI) {
    RIL_IMS_SMS_Message rism = {};
    char **pStrings;
    int countStrings = 2;
    int dataLen = sizeof(char *) * countStrings;

    rism.tech = RADIO_TECH_3GPP;
    rism.retry = BOOL_TO_INT(message.retry);
    rism.messageRef = message.messageRef;

    if (message.gsmMessage.size() != 1) {
        mtkLogE(LOG_TAG, "dispatchImsGsmSmsEx: Invalid len %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    }

    pStrings = (char **)calloc(countStrings, sizeof(char *));
    if (pStrings == NULL) {
        mtkLogE(LOG_TAG, "dispatchImsGsmSmsEx: Memory allocation failed for request %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }

    if (!copyAidlStringToRil(&pStrings[0], message.gsmMessage[0].smscPdu, pRI)) {
#ifdef MEMSET_FREED
        memset(pStrings, 0, dataLen);
#endif
        free(pStrings);
        return false;
    }

    if (!copyAidlStringToRil(&pStrings[1], message.gsmMessage[0].pdu, pRI)) {
        memsetAndFreeStrings(1, pStrings[0]);
#ifdef MEMSET_FREED
        memset(pStrings, 0, dataLen);
#endif
        free(pStrings);
        return false;
    }

    rism.message.gsmMessage = pStrings;
    CALL_ONREQUEST(pRI->pCI->requestNumber, &rism, sizeof(RIL_RadioTechnologyFamily) +
            sizeof(uint8_t) + sizeof(int32_t) + dataLen, pRI, pRI->socket_id);

    for (int i = 0 ; i < countStrings ; i++) {
        memsetAndFreeStrings(1, pStrings[i]);
    }

#ifdef MEMSET_FREED
    memset(pStrings, 0, dataLen);
#endif
    free(pStrings);

    return true;
}

struct ImsCdmaSms {
    RIL_IMS_SMS_Message imsSms;
    RIL_CDMA_SMS_Message cdmaSms;
};

void constructCdmaSmsEx(RIL_CDMA_SMS_Message &rcsm, const aidl_messaging::CdmaSmsMessage& sms) {
    rcsm.uTeleserviceID = sms.teleserviceId;
    rcsm.bIsServicePresent = BOOL_TO_INT(sms.isServicePresent);
    rcsm.uServicecategory = sms.serviceCategory;
    rcsm.sAddress.digit_mode = (RIL_CDMA_SMS_DigitMode) sms.address.digitMode;
    rcsm.sAddress.number_mode = (RIL_CDMA_SMS_NumberMode) sms.address.isNumberModeDataNetwork;
    rcsm.sAddress.number_type = (RIL_CDMA_SMS_NumberType) sms.address.numberType;
    rcsm.sAddress.number_plan = (RIL_CDMA_SMS_NumberPlan) sms.address.numberPlan;

    rcsm.sAddress.number_of_digits = sms.address.digits.size();
    int digitLimit= MIN((rcsm.sAddress.number_of_digits), RIL_CDMA_SMS_ADDRESS_MAX);
    for (int i = 0; i < digitLimit; i++) {
        rcsm.sAddress.digits[i] = sms.address.digits[i];
    }

    rcsm.sSubAddress.subaddressType = (RIL_CDMA_SMS_SubaddressType) sms.subAddress.subaddressType;
    rcsm.sSubAddress.odd = BOOL_TO_INT(sms.subAddress.odd);

    rcsm.sSubAddress.number_of_digits = sms.subAddress.digits.size();
    digitLimit= MIN((rcsm.sSubAddress.number_of_digits), RIL_CDMA_SMS_SUBADDRESS_MAX);
    for (int i = 0; i < digitLimit; i++) {
        rcsm.sSubAddress.digits[i] = sms.subAddress.digits[i];
    }

    rcsm.uBearerDataLen = sms.bearerData.size();
    digitLimit= MIN((rcsm.uBearerDataLen), RIL_CDMA_SMS_BEARER_DATA_MAX);
    for (int i = 0; i < digitLimit; i++) {
        rcsm.aBearerData[i] = sms.bearerData[i];
    }
}

bool dispatchImsCdmaSmsEx(const aidl_messaging::ImsSmsMessage& message, android::RequestInfo *pRI) {
    ImsCdmaSms temp = {};

    if (message.cdmaMessage.size() != 1) {
        mtkLogE(LOG_TAG, "dispatchImsCdmaSmsEx: Invalid len %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    }

    temp.imsSms.tech = RADIO_TECH_3GPP2;
    temp.imsSms.retry = BOOL_TO_INT(message.retry);
    temp.imsSms.messageRef = message.messageRef;
    temp.imsSms.message.cdmaMessage = &temp.cdmaSms;

    constructCdmaSmsEx(temp.cdmaSms, message.cdmaMessage[0]);

    // Vendor code expects payload length to include actual msg payload
    // (sizeof(RIL_CDMA_SMS_Message)) instead of (RIL_CDMA_SMS_Message *) + size of other fields in
    // RIL_IMS_SMS_Message
    int payloadLen = sizeof(RIL_RadioTechnologyFamily) + sizeof(uint8_t) + sizeof(int32_t)
            + sizeof(RIL_CDMA_SMS_Message);

    CALL_ONREQUEST(pRI->pCI->requestNumber, &temp.imsSms, payloadLen, pRI, pRI->socket_id);

    return true;
}

ScopedAStatus MtkRadioIms::sendImsSmsEx(int32_t serial,
        const aidl_messaging::ImsSmsMessage& message, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendImsSmsEx: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    android::RequestInfo *pRI = android::addRequestToList(serial,
            mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_SEND_SMS_EX);
    if (pRI == NULL) {
        return ok();
    }
    if (aidl_radio::RadioTechnologyFamily::THREE_GPP == message.tech) {
        dispatchImsGsmSmsEx(message, pRI);
    } else if (aidl_radio::RadioTechnologyFamily::THREE_GPP2 == message.tech) {
        dispatchImsCdmaSmsEx(message, pRI);
    } else {
        mtkLogE(LOG_TAG, "sendImsSmsEx: Invalid radio tech %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
    }
    return ok();
}
// M: IMS_SMS end @}

// M: IMS_RTT start @{
ScopedAStatus MtkRadioIms::rttModifyRequestResponse(int32_t serial, int32_t callId, int32_t result, int32_t clientId) {
    mtkLogD(LOG_TAG, "rttModifyRequestResponse: serial %d", serial);

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_RTT_MODIFY_REQUST_RESPONSE, 2,
                 callId, result);
    return ok();
}

ScopedAStatus MtkRadioIms::sendRttModifyRequest(int32_t serial, int32_t callId, int32_t newMode, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendRttModifyRequest: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SEND_RTT_MODIFY_REQUEST, 2,
                 callId, newMode);
    return ok();
}

ScopedAStatus MtkRadioIms::sendRttText(int32_t serial, int32_t callId, int32_t lenOfString, const std::string& text, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendRttText: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    std::string strCallId = std::to_string(callId);
    std::string strlenOfString = std::to_string(lenOfString);
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SEND_RTT_TEXT, false, 3,
                    strCallId.c_str(), strlenOfString.c_str(), text.c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::setRttMode(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setRttMode: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_RTT_MODE, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioIms::toggleRttAudioIndication(int32_t serial, int32_t callId, int32_t audio, int32_t clientId) {
    mtkLogD(LOG_TAG, "toggleRttAudioIndication: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_TOGGLE_RTT_AUDIO_INDICATION, 2,
                 callId, audio);
    return ok();
}
// M: IMS_RTT end @}

// M: IMS_SS start @{
ScopedAStatus MtkRadioIms::cancelUssi(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "cancelPendingUssi: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_CANCEL_USSI);
    return ok();
}

ScopedAStatus MtkRadioIms::getXcapStatus(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getXcapStatus: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_XCAP_STATUS);
    return ok();
}

ScopedAStatus MtkRadioIms::sendUssi(int32_t serial,
        const std::string& ussiString, int32_t clientId)  {
    mtkLogD(LOG_TAG, "sendUssi: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    rilAidlUtils::dispatchString(serial, mSlotId + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_SEND_USSI, ussiString.c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::setupXcapUserAgentString(int32_t serial,
        const std::string& userAgent, int32_t clientId) {
    mtkLogD(LOG_TAG, "setupXcapUserAgentString: serial %d, userAgent %s",
            serial, userAgent.c_str());
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_SETUP_XCAP_USER_AGENT_STRING, true, 1, userAgent.c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::getBarringCalls(
        int32_t serial, int32_t serviceClass, int32_t clientId) {
    mtkLogD(LOG_TAG, "getBarringCalls: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_GET_BARRING_SEPCIFIC_CALL, 1, serviceClass);
    return ok();
}

ScopedAStatus MtkRadioIms::setBarringCalls(int32_t serial,
        const std::vector<aidl_ims_mtk::ims::ImsBarringCall>& calls, int32_t clientId) {
    mtkLogD(LOG_TAG, "setBarringCalls: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    android::RequestInfo *pRI = android::addRequestToList(serial,
        mSlotId + clientId * MAX_SIM_COUNT,
        RIL_REQUEST_SET_BARRING_SEPCIFIC_CALL);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setBarringCalls: memory allocation failed for pRI");
        return ok();
    }

    int count = calls.size();
    RIL_BarringCallInfo **ppBaringCall =
            (RIL_BarringCallInfo **) calloc(count, sizeof(RIL_BarringCallInfo *));
    if (ppBaringCall == NULL) {
        mtkLogE(LOG_TAG, "setBarringCalls: memory allocation failed for ppBaringCall");
        return ok();
    }

    for (int i = 0; i < count; i++) {
        ppBaringCall[i] = (RIL_BarringCallInfo *) calloc(1, sizeof(RIL_BarringCallInfo));
        if (ppBaringCall[i] == NULL) {
            mtkLogE(LOG_TAG, "setBarringCalls: memory allocation failed for pBaringCall");
            // Free previously allocated memory before returning
            for (int j = 0; j < i; j++) {
                free(ppBaringCall[j]);
            }
            free(ppBaringCall);
            return ok();
        }

        ppBaringCall[i]->status = calls[i].status;
        ppBaringCall[i]->serviceClass = calls[i].serviceClass;
        ppBaringCall[i]->toa = calls[i].toa;

        if (!rilAidlUtils::copyAidlStringToRil(&ppBaringCall[i]->number, calls[i].number, pRI)) {
            mtkLogE(LOG_TAG, "setBarringCalls: memory allocation failed for number");
            // Free previously allocated memory before returning
            for (int j = 0; j <= i; j++) {
                free(ppBaringCall[j]);
            }
            free(ppBaringCall);
            return ok();
        }
    }

    CALL_ONREQUEST(pRI->pCI->requestNumber,
            ppBaringCall,
            count * sizeof(RIL_BarringCallInfo *),
            pRI, pRI->socket_id);

    for (int j = 0; j < count; j++) {
        if (ppBaringCall[j]->number != NULL) {
            memsetAndFreeStrings(1, ppBaringCall[j]->number);
        }

#ifdef MEMSET_FREED
        memset(ppBaringCall[j], 0, sizeof(RIL_BarringCallInfo));
#endif
        free(ppBaringCall[j]);
    }

#ifdef MEMSET_FREED
    memset(ppBaringCall, 0, sizeof(ppBaringCall));
#endif
    free(ppBaringCall);

    return ok();
}
// M: IMS_SS end @}

// M: IMS_BASE start @{
ScopedAStatus MtkRadioIms::queryVopsStatus(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "queryVopsStatus: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_QUERY_VOPS_STATUS);
    return ok();
}

ScopedAStatus MtkRadioIms::setImsRegistrationReport(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "setImsRegistrationReport: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_IMS_REGISTRATION_REPORT);
    return ok();
}
// M: IMS_BASE end @}

// M: IMS_CONFIG  start @{
ScopedAStatus MtkRadioIms::getImsCfgFeatureValue(int32_t serial, int32_t featureId, int32_t network, int32_t clientId) {
    mtkLogD(LOG_TAG, "getImsCfgFeatureValue: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_CONFIG_GET_FEATURE,
                 2, featureId, network);
    return ok();
}

ScopedAStatus MtkRadioIms::getImsCfgProvisionValue(int32_t serial, int32_t configId, int32_t clientId) {
    mtkLogD(LOG_TAG, "getImsCfgProvisionValue: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_CONFIG_GET_PROVISION,
                 1, configId);
    return ok();
}

ScopedAStatus MtkRadioIms::getImsCfgResourceCapValue(int32_t serial, int32_t featureId, int32_t clientId) {
    mtkLogD(LOG_TAG, "getImsCfgResourceCapValue: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_CONFIG_GET_RESOURCE_CAP,
                 1, featureId);
    return ok();
}

ScopedAStatus MtkRadioIms::getVoiceDomainPreference(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getVoiceDomainPreference: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_VOICE_DOMAIN_PREFERENCE);
    return ok();
}

ScopedAStatus MtkRadioIms::setImsCfgFeatureValue(int32_t serial, int32_t featureId, int32_t network, int32_t value, int32_t isLast, int32_t clientId) {
    mtkLogD(LOG_TAG, "setImsCfgFeatureValue: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_CONFIG_SET_FEATURE,
                 4, featureId, network, value, isLast);
    return ok();
}

ScopedAStatus MtkRadioIms::setImsCfgProvisionValue(int32_t serial, int32_t configId, const std::string& value, int32_t clientId) {
    mtkLogD(LOG_TAG, "setImsCfgProvisionValue: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_CONFIG_SET_PROVISION,false,
                    2, std::to_string(configId).c_str(), value.c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::setImscfg(int32_t serial, bool volteEnable, bool vilteEnable, bool vowifiEnable, bool viwifiEnable, bool smsEnable, bool eimsEnable, int32_t clientId) {
    mtkLogD(LOG_TAG, "setImscfg: serial %d, clientId %d, slotId %d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    int slotId = mSlotId;
    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_IMS) {
        slotId += clientId * MAX_SIM_COUNT;
    }
    dispatchInts(serial, slotId, RIL_REQUEST_SET_IMSCFG, 6,
                 BOOL_TO_INT(volteEnable),
                 BOOL_TO_INT(vilteEnable),
                 BOOL_TO_INT(vowifiEnable),
                 BOOL_TO_INT(viwifiEnable),
                 BOOL_TO_INT(smsEnable),
                 BOOL_TO_INT(eimsEnable));
    return ok();
}

ScopedAStatus MtkRadioIms::getImscfg(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getImscfg: serial %d, clientId %d, slotId %d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    int slotId = mSlotId;
    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_IMS) {
        slotId += clientId * MAX_SIM_COUNT;
    }
    dispatchVoid(serial, slotId, RIL_REQUEST_GET_IMSCFG);
    return ok();
}

ScopedAStatus MtkRadioIms::setVoiceDomainPreference(int32_t serial, int32_t vdp, int32_t clientId) {
    mtkLogD(LOG_TAG, "setVoiceDomainPreference: %d", vdp);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_VOICE_DOMAIN_PREFERENCE, 1, vdp);

    return ok();
}

ScopedAStatus MtkRadioIms::setWfcProfile(int32_t serial, int32_t wfcPreference, int32_t clientId) {
    mtkLogD(LOG_TAG, "setWfcProfile: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_WFC_PROFILE, 1, wfcPreference);
    return ok();
}

ScopedAStatus MtkRadioIms::setModemImsCfg(int32_t serial, const std::string& keys, const std::string& values, int32_t type, int32_t clientId) {
    mtkLogD(LOG_TAG, "setModemImsCfg: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    if (strlen(keys.c_str()) > IMS_MAX_MD_CFG_KEYS_LEN || strlen(keys.c_str()) > IMS_MAX_MD_CFG_VALS_LEN) {
        android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_MD_IMSCFG);
        if (pRI == NULL) {
            mtkLogE(LOG_TAG, "setModemImsCfg: pRI == NULL");
            return ok();
        }
        mtkLogE(LOG_TAG, "setModemImsCfg: keys or values too long");
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
    } else {
        rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_MD_IMSCFG, false, 3, keys.c_str(), values.c_str(),
                std::to_string(type).c_str());
    }
    return ok();
}
// M: IMS_CONFIG end @}

// M: IMS_CC start @{
ScopedAStatus MtkRadioIms::setCallAdditionalInfo(int32_t serial, const std::vector<std::string>& info, int32_t clientId) {
    mtkLogD(LOG_TAG, "setCallAdditionalInfo: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_CALL_ADDITIONAL_INFO, info);
    return ok();
}

ScopedAStatus MtkRadioIms::setImsCallMode(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setImsCallMode: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    android::RequestInfo *pRI = android::addRequestToList(serial,
                 mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_IMS_CALL_MODE);
    if (pRI != nullptr) {
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    } else {
        // Handle the error appropriately if pRI is null
        ALOGE("Failed to add request to list: pRI is null");
        return ok();
    }
    // dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_IMS_CALL_MODE, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioIms::conferenceDial(int32_t serial,
        const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::ConferenceDial& dialInfo,
        int32_t clientId) {
    mtkLogD(LOG_TAG, "conferenceDial: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    int request = RIL_REQUEST_CONFERENCE_DIAL;
    android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT, request);
    if (pRI == NULL) {
        return ok();
    }

    int countStrings = dialInfo.dialNumbers.size() + 3;
    char **pStrings;
    pStrings = (char **)calloc(countStrings, sizeof(char *));
    if (pStrings == NULL) {
        mtkLogE(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(request));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return ok();
    }

    if(!rilAidlUtils::copyAidlStringToRil(&pStrings[0], dialInfo.isVideoCall ? "1":"0", pRI)) {
        free(pStrings);
        return ok();
    }

    if(!rilAidlUtils::copyAidlStringToRil(&pStrings[1],
            std::to_string(dialInfo.dialNumbers.size()).c_str(), pRI)) {
        memsetAndFreeStrings(1, pStrings[0]);
        free(pStrings);
        return ok();
    }

    int i = 0;
    for (i = 0; i < (int) dialInfo.dialNumbers.size(); i++) {
        if (!rilAidlUtils::copyAidlStringToRil(&pStrings[i + 2], dialInfo.dialNumbers[i], pRI, true)) {
            for (int j = 0; j < i + 2; j++) {
                memsetAndFreeStrings(1, pStrings[j]);
            }
            free(pStrings);
            return ok();
        }
    }

    if(!rilAidlUtils::copyAidlStringToRil(&pStrings[i + 2],
        std::to_string(dialInfo.clir).c_str(), pRI)) {
        for (int j = 0; j < (int) dialInfo.dialNumbers.size() + 2; j++) {
            memsetAndFreeStrings(1, pStrings[j]);
        }
        free(pStrings);
        return ok();
    }

#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(request, pStrings, countStrings * sizeof(char *), pRI,
            pRI->socket_id);
#else
    s_vendorFunctions->onRequest(request, pStrings, countStrings * sizeof(char *), pRI);
#endif

    if (pStrings != NULL) {
        for (int j = 0 ; j < countStrings ; j++) {
            memsetAndFreeStrings(1, pStrings[j]);
        }

#ifdef MEMSET_FREED
        memset(pStrings, 0, countStrings * sizeof(char *));
#endif
        free(pStrings);
    }

    return ok();
}

ScopedAStatus MtkRadioIms::controlCall(int32_t serial, int32_t controlType, int32_t callId, int32_t clientId) {
    mtkLogD(LOG_TAG, "controlCall: serial %d, controlType %d, callId %d ,", serial, controlType, callId);
    int slotId = mSlotId;
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_IMS) {
        slotId += clientId * MAX_SIM_COUNT;
    }
    if (controlType == 0) {
        dispatchInts(serial, slotId, RIL_REQUEST_HOLD_CALL, 1, callId);
    } else if (controlType == 1) {
        dispatchInts(serial, slotId, RIL_REQUEST_RESUME_CALL, 1, callId);
    }
    return ok();
}

ScopedAStatus MtkRadioIms::controlImsConferenceCallMember(int32_t serial, int32_t controlType, int32_t confCallId, const std::string& address, int32_t callId, int32_t clientId) {
    mtkLogD(LOG_TAG, "controlImsConferenceCallMember: serial %d, controlType %d", serial, controlType);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    if (controlType == 0) {
        rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER, true, 3,
                std::to_string(confCallId).c_str(), address.c_str(),
                std::to_string(callId).c_str());
    } else if (controlType == 1) {
        rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER, true, 3,
                std::to_string(confCallId).c_str(), address.c_str(),
                std::to_string(callId).c_str());
    }

    return ok();
}

ScopedAStatus MtkRadioIms::dialWithSipUri(int32_t serial, const std::string& address, int32_t clientId) {
    mtkLogD(LOG_TAG, "dialWithSipUri: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    rilAidlUtils::dispatchString(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DIAL_WITH_SIP_URI, address.c_str());

    return ok();
}

ScopedAStatus MtkRadioIms::eccRedialApprove(int32_t serial, int32_t approve, int32_t callId, int32_t clientId) {
    mtkLogD(LOG_TAG, "eccRedialApprove: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ECC_REDIAL_APPROVE, 2, approve, callId);
    return ok();
}

ScopedAStatus MtkRadioIms::forceReleaseCall(int32_t serial, int32_t callId, int32_t clientId) {
    mtkLogD(LOG_TAG, "forceHangup: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_FORCE_RELEASE_CALL, 1,
                 callId);

    return ok();
}

ScopedAStatus MtkRadioIms::imsEctCommand(int32_t serial, const std::string& number, int32_t type, int32_t clientId) {
    mtkLogD(LOG_TAG, "imsEctCommand: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    std::string strType = std::to_string(type);
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IMS_ECT, false,
            2, (const char *)number.c_str(), (const char *)strType.c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::pullCall(int32_t serial, const std::string& target, bool isVideoCall, int32_t clientId) {
    mtkLogD(LOG_TAG, "pullCall: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_PULL_CALL, false, 2, target.c_str(),
                 isVideoCall ? "1":"0");

    return ok();
}

ScopedAStatus MtkRadioIms::setSipHeader(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "setSipHeader: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    android::RequestInfo *pRI = android::addRequestToList(serial,
                 mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_SIP_HEADER);
    if (pRI != nullptr) {
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    } else {
        // Handle the error appropriately if pRI is null
        ALOGE("Failed to add request to list: pRI is null");
        return ok();
    }
    // rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_SIP_HEADER, data);
    return ok();
}

ScopedAStatus MtkRadioIms::setSipHeaderReport(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "setSipHeaderReport: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    android::RequestInfo *pRI = android::addRequestToList(serial,
                 mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SIP_HEADER_REPORT);
    if (pRI != nullptr) {
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    } else {
        // Handle the error appropriately if pRI is null
        ALOGE("Failed to add request to list: pRI is null");
        return ok();
    }
    // rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SIP_HEADER_REPORT, data);
    return ok();
}
// M: IMS_CC end @}

// M: IMS_ViLTE start @{
ScopedAStatus MtkRadioIms::videoCallAccept(int32_t serial, int32_t videoMode, int32_t callId, int32_t clientId) {
    mtkLogD(LOG_TAG, "videoCallAccept: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    dispatchInts(serial,  mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_VIDEO_CALL_ACCEPT, 2, videoMode, callId);
    return ok();
}

ScopedAStatus MtkRadioIms::videoRingtoneEventRequest(int32_t serial, const std::vector<std::string>& event, int32_t clientId) {
    mtkLogD(LOG_TAG, "videoRingtoneEventRequest: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial,  mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_VIDEO_RINGTONE_EVENT, event);
    return ok();
}

ScopedAStatus MtkRadioIms::vtDial(int32_t serial, const ::aidl::android::hardware::radio::voice::Dial& dialInfo, int32_t clientId) {
    mtkLogD(LOG_TAG, "vtDial: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    int requestId = RIL_REQUEST_VT_DIAL;
    if(clientId == android::CLIENT_IMS) {
        requestId = RIL_REQUEST_IMS_VT_DIAL;
    }
    android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT, requestId);
    if (pRI == NULL) {
        return ok();
    }
    RIL_Dial dial = {};
    RIL_UUS_Info uusInfo = {};
    int32_t sizeOfDial = sizeof(dial);
    if (!rilAidlUtils::copyAidlStringToRil(&dial.address, dialInfo.address, pRI)) {
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
            if (!rilAidlUtils::copyAidlStringToRil(&uusInfo.uusData,
                                     dialInfo.uusInfo[0].uusData, pRI)) {
                memsetAndFreeStrings(1, dial.address);
                return ok();
            }
            uusInfo.uusLength = dialInfo.uusInfo[0].uusData.size();
        }
        dial.uusInfo = &uusInfo;
    }
#if defined(ANDROID_MULTI_SIM)
    s_vendorFunctions->onRequest(requestId,
                                 &dial, sizeOfDial, pRI, pRI->socket_id);
#else
    s_vendorFunctions->onRequest(requestId,
                                 &dial, sizeOfDial, pRI);
#endif
    memsetAndFreeStrings(2, dial.address, uusInfo.uusData);
    return ok();
}

ScopedAStatus MtkRadioIms::vtDialWithSipUri(int32_t serial, const std::string& address, int32_t clientId) {
    mtkLogD(LOG_TAG, "vtDialWithSipUri: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchString(serial,  mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_VT_DIAL_WITH_SIP_URI, address.c_str());

    return ok();
}
// M: IMS_ViLTE end @}

// M: IMS_WFC start @{
ScopedAStatus MtkRadioIms::querySsacStatus(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d", __func__, serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_QUERY_SSAC_STATUS);
    return ok();
}

ScopedAStatus MtkRadioIms::getWfcConfig(int32_t serial, int32_t setting, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, setting %d, clientId %d", __func__, serial, setting, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_WFC_CONFIG, 1,
            setting);
    return ok();
}

ScopedAStatus MtkRadioIms::notifyEPDGScreenState(int32_t serial, int32_t state, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_NOTIFY_EPDG_SCREEN_STATE, 1,
                 state);
    return ok();
}

ScopedAStatus MtkRadioIms::setEmergencyAddressId(int32_t serial, const std::string& aid, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_EMERGENCY_ADDRESS_ID, true, 1, aid.c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::setLocationInfo(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_GEO_LOCATION, data);
    return ok();
}

ScopedAStatus MtkRadioIms::setNattKeepAliveStatus(int32_t serial, const std::string& ifName, bool enable, const std::string& srcIp, int32_t srcPort, const std::string& dstIp, int32_t dstPort,
        int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_NATT_KEEP_ALIVE_STATUS, true, 6,
            ifName.c_str(),
            enable ? "1" : "0",
            srcIp.c_str(),
            std::to_string(srcPort).c_str(),
            dstIp.c_str(),
            std::to_string(dstPort).c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::setWfcConfig(int32_t serial, int32_t setting, const std::string& ifName, const std::string& value, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, setting %d, clientId %d", __func__, serial, setting, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_WFC_CONFIG, true, 3,
            std::to_string(setting).c_str(),
            ifName.c_str(),
            value.c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::setWifiAssociated(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_WIFI_ASSOCIATED, data);
    return ok();
}

ScopedAStatus MtkRadioIms::setWifiEnabled(int32_t serial, const std::string& ifName, int32_t isWifiEnabled, int32_t isFlightModeOn, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_WIFI_ENABLED, true, 3,
        ifName.c_str(), std::to_string(isWifiEnabled).c_str(), std::to_string(isFlightModeOn).c_str());
    return ok();
}

ScopedAStatus MtkRadioIms::setWifiIpAddress(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_WIFI_IP_ADDRESS, data);
    return ok();
}

ScopedAStatus MtkRadioIms::setWifiPingResult(int32_t serial, int32_t rat, int32_t latency, int32_t pktloss, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_WIFI_PING_RESULT, 3,
                 rat,
                 latency,
                 pktloss);
    return ok();
}

ScopedAStatus MtkRadioIms::setWifiSignalLevel(int32_t serial, int32_t rssi, int32_t snr, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_WIFI_SIGNAL_LEVEL, true, 3,
            "wlan", std::to_string(rssi).c_str(),
            (snr == WIFI_SNR_UNKNOW) ? "unknow" : std::to_string(snr).c_str());
    return ok();
}
// M: IMS_WFC end @}

ScopedAStatus MtkRadioIms::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus MtkRadioIms::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExImsResponse>& radioResponse,
        const std::shared_ptr<aidl::IMtkRadioExImsIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk ims setResponseFunctionsMtk, slotId:%d", mSlotId);
    // if (!isValidSlotId(mSlotId)) {
    //     return ok();
    // }
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk ims setResponseFunctionsMtk: got lock %d", mSlotId);

    radioImsServiceImpl[mSlotId].mtkImsResponse = radioResponse;
    radioImsServiceImpl[mSlotId].mtkImsIndication = radioIndication;
    android::ClientId client = getClientBySlot(mSlotId);

    if(client == android::CLIENT_TBOX_IMS) {
        plusCounterMtkRadioNumOne(mSlotId );
    } else {
        notifyMtkImsServiceReady(mSlotId);
        plusCounterMtkRadioNumOne(mSlotId + android::CLIENT_IMS * MAX_SIM_COUNT);
    }

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk ims setResponseFunctionsMtk: release lock %d", mSlotId);
    // client is connected. Send initial indications.
    if (client == android::CLIENT_TBOX_IMS) {
        android::onNewCommandConnect((RIL_SOCKET_ID) (mSlotId), android::TBOX_IMS_MTK);
    } else {
        android::onNewCommandConnect((RIL_SOCKET_ID) (mSlotId + android::CLIENT_IMS * MAX_SIM_COUNT), android::IMS_MTK);
    }
    return ok();
}

ScopedAStatus MtkRadioIms::setResponseFunctionsMwi(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::mwi::IMwiRadioResponse>& radioResponse,
        const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::mwi::IMwiRadioIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk mwi setResponseFunctions, slotId:%d", mSlotId);
    if (!isValidSlotId(mSlotId)) {
        return ok();
    }
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk mwi setResponseFunctions: got lock %d", mSlotId);
    radioMwiServiceImpl[mSlotId].mtkMwiResponse = radioResponse;
    radioMwiServiceImpl[mSlotId].mtkMwiIndication = radioIndication;
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk mwi setResponseFunctions: release lock %d", mSlotId);
    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_TBOX_IMS) {
        android::onNewCommandConnect((RIL_SOCKET_ID) (mSlotId), android::TBOX_IMS_WFC);
    } else {
        android::onNewCommandConnect((RIL_SOCKET_ID) (mSlotId + android::CLIENT_IMS * MAX_SIM_COUNT), android::IMS_WFC);
    }
    return ok();
}

}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addImsLock(std::shared_ptr<ndk::ICInterface> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_IMS_PORT_BASE 56000
#define MTK_RADIO_IMS_PORT_BASE 56100

static pthread_t gThreadIds[MAX_SIM_COUNT*2];

void* addRcpVsockRadioImsAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_IMS_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_IMS_PORT_BASE + sloid, sloid);

    std::shared_ptr<android::hardware::radio::ims::RadioIms> radioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(sloid);
    addImsLock(std::static_pointer_cast<ndk::ICInterface>(radioImsAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioImsAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void* addRcpVsockMtkRadioImsAidlService(void* arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, MTK_RADIO_IMS_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", MTK_RADIO_IMS_PORT_BASE + sloid, sloid);
    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkRadioImsAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(sloid);
    addImsLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioImsAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);

    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadioImsAidl->asBinder().get()));
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioImsVsock(unsigned int &sloid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
            addRcpVsockRadioImsAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioIms");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT], NULL,
            addRcpVsockMtkRadioImsAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockMtkRadioIms");
    }
}
#endif


void publicRadioImsAidlService(unsigned int sloid) {
    const char *mtkRadioImsDescriptor = "";
    if (vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms::descriptor != NULL) {
        mtkRadioImsDescriptor = vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms::descriptor;
    }

    /*if (sloid == 0) {
        std::shared_ptr<android::hardware::radio::ims::RadioIms> radioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(0);
        gPublishedHals.push_back(radioImsAidl);
        const std::string instance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/slot1";
        binder_status_t status =
                AServiceManager_addService(radioImsAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp ims addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(0);
        gPublishedHals.push_back(mtkRadioImsAidl);
        const std::string mtkInstance = std::string() + mtkRadioImsDescriptor + "/slot1";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioImsAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk ims addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 1) {
        std::shared_ptr<android::hardware::radio::ims::RadioIms> radioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(1);
        gPublishedHals.push_back(radioImsAidl);
        const std::string instance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/slot2";
        binder_status_t status =
                AServiceManager_addService(radioImsAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp ims addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(1);
        gPublishedHals.push_back(mtkRadioImsAidl);
        const std::string mtkInstance = std::string() + mtkRadioImsDescriptor + "/slot2";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioImsAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk ims addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 2) {
        std::shared_ptr<android::hardware::radio::ims::RadioIms> radioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(2);
        gPublishedHals.push_back(radioImsAidl);
        const std::string instance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/slot3";
        binder_status_t status =
                AServiceManager_addService(radioImsAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp ims addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(2);
        gPublishedHals.push_back(mtkRadioImsAidl);
        const std::string mtkInstance = std::string() + mtkRadioImsDescriptor + "/slot3";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioImsAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk ims addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 3) {
        std::shared_ptr<android::hardware::radio::ims::RadioIms> radioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(3);
        gPublishedHals.push_back(radioImsAidl);
        const std::string instance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/slot4";
        binder_status_t status =
                AServiceManager_addService(radioImsAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp ims addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(3);
        gPublishedHals.push_back(mtkRadioImsAidl);
        const std::string mtkInstance = std::string() +mtkRadioImsDescriptor + "/slot4";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioImsAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk ims addservice, slotId:%d, status:%d", sloid, mtkStatus);
    }*/

    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;

    if (sloid == 0) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioImsVsock(sloid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::ims::RadioIms> tboxRadioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioImsAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/tboxImsSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioImsAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox ims addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkTboxRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioImsAidl));
        const std::string mtkTboxInstance = std::string() + mtkRadioImsDescriptor + "/tboxImsSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioImsAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ims addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 1) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioImsVsock(sloid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::ims::RadioIms> tboxRadioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioImsAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/tboxImsSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioImsAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox ims addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkTboxRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioImsAidl));
        const std::string mtkTboxInstance = std::string() + mtkRadioImsDescriptor + "/tboxImsSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioImsAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ims addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 2) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioImsVsock(sloid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::ims::RadioIms> tboxRadioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioImsAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/tboxImsSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioImsAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox ims addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkTboxRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioImsAidl));
        const std::string mtkTboxInstance = std::string() + mtkRadioImsDescriptor + "/tboxImsSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioImsAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ims addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 3) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioImsVsock(sloid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::ims::RadioIms> tboxRadioImsAidl = ndk::SharedRefBase::make<android::hardware::radio::ims::RadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioImsAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::ims::RadioIms::descriptor + "/tboxImsSlot4";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioImsAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox ims addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms> mtkTboxRadioImsAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::ims::MtkRadioIms>(tboxSlotid);
        addImsLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioImsAidl));
        const std::string mtkTboxInstance = std::string() + mtkRadioImsDescriptor + "/tboxImsSlot4";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioImsAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox ims addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    }
}
