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
#include <radiomessaging_service.h>
#include <mtk_log.h>
#include <ril_service.h>
#include <rilAidlUtils.h>

#include <binder/ProcessState.h>
#include <android/binder_manager.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_radio_messaging = ::aidl::android::hardware::radio::messaging;
namespace aidl_mtk_radio_messaging = ::aidl::vendor::mediatek::hardware::mtkradioex::messaging;

using rilAidlUtils::copyAidlStringToRil;
using rilAidlUtils::populateAidlResponseInfo;
using rilAidlUtils::retChecker;

struct RadioMessagingImpl {
    std::shared_ptr<aidl_mtk_radio_messaging::IMtkRadioExMessagingResponse> mtkMessagingResponse;
    std::shared_ptr<aidl_mtk_radio_messaging::IMtkRadioExMessagingIndication>
            mtkMessagingIndication;
    std::shared_ptr<aidl_radio_messaging::IRadioMessagingResponse> messagingResponse;
    std::shared_ptr<aidl_radio_messaging::IRadioMessagingIndication> messagingIndication;
};

RadioMessagingImpl radioMessagingServiceImpl[MAX_SIM_COUNT*(android::CLIENT_TBOX + 1)];

// AOSP radio
// response
int radioMessaging::acknowledgeIncomingGsmSmsWithPduResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "acknowledgeIncomingGsmSmsWithPduResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->acknowledgeIncomingGsmSmsWithPduResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "acknowledgeIncomingGsmSmsWithPduResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "acknowledgeIncomingGsmSmsWithPduResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::acknowledgeLastIncomingCdmaSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingCdmaSmsResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->acknowledgeLastIncomingCdmaSmsResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "acknowledgeLastIncomingCdmaSmsResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "acknowledgeLastIncomingCdmaSmsResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}


int radioMessaging::acknowledgeLastIncomingGsmSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingGsmSmsResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->acknowledgeLastIncomingGsmSmsResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "acknowledgeLastIncomingGsmSmsResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "acknowledgeLastIncomingGsmSmsResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::deleteSmsOnRuimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "deleteSmsOnRuimResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->deleteSmsOnRuimResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "deleteSmsOnRuimResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "deleteSmsOnRuimResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::deleteSmsOnSimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "deleteSmsOnSimResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->deleteSmsOnSimResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "deleteSmsOnSimResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "deleteSmsOnSimResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::getCdmaBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getCdmaBroadcastConfigResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl_radio_messaging::CdmaBroadcastSmsConfigInfo> configs;
        if (response == NULL || responseLen == 0
                || responseLen % sizeof (RIL_CDMA_BroadcastSmsConfigInfo *) != 0) {
            mtkLogE(LOG_TAG, "getCdmaBroadcastConfigResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            int num = responseLen / sizeof(RIL_CDMA_BroadcastSmsConfigInfo *);
            configs.resize(num);
            for (int i = 0 ; i < num; i++) {
                RIL_CDMA_BroadcastSmsConfigInfo *resp =
                         ((RIL_CDMA_BroadcastSmsConfigInfo **) response)[i];

                configs[i].serviceCategory = resp->service_category;
                configs[i].language = resp->language;
                configs[i].selected = resp->selected == 1 ? true : false;
            }
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->getCdmaBroadcastConfigResponse(responseInfo, configs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCdmaBroadcastConfigResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getCdmaBroadcastConfigResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::setCdmaBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setCdmaBroadcastConfigResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->setCdmaBroadcastConfigResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCdmaBroadcastConfigResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setCdmaBroadcastConfigResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

// new added
int radioMessaging::getGsmBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getGsmBroadcastConfigResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl_radio_messaging::GsmBroadcastSmsConfigInfo> configs;
        if (response == NULL || responseLen == 0
                || responseLen % sizeof (RIL_CDMA_BroadcastSmsConfigInfo *) != 0) {
            mtkLogE(LOG_TAG, "getGsmBroadcastConfigResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            int num = responseLen / sizeof(RIL_GSM_BroadcastSmsConfigInfo *);
            configs.resize(num);
            for (int i = 0 ; i < num; i++) {
                RIL_GSM_BroadcastSmsConfigInfo *resp =
                        ((RIL_GSM_BroadcastSmsConfigInfo **) response)[i];
                configs[i].fromServiceId = resp->fromServiceId;
                configs[i].toServiceId = resp->toServiceId;
                configs[i].fromCodeScheme = resp->fromCodeScheme;
                configs[i].toCodeScheme = resp->toCodeScheme;
                configs[i].selected = resp->selected == 1 ? true : false;
            }
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->getGsmBroadcastConfigResponse(responseInfo, configs);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getGsmBroadcastConfigResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getGsmBroadcastConfigResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}


int radioMessaging::getSmscAddressResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getSmscAddressResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->getSmscAddressResponse(responseInfo, convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSmscAddressResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getSmscAddressResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}


int radioMessaging::reportSmsMemoryStatusResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "reportSmsMemoryStatusResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->reportSmsMemoryStatusResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "reportSmsMemoryStatusResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "reportSmsMemoryStatusResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

aidl_radio_messaging::SendSmsResult makeSendSmsResult(aidl_radio::RadioResponseInfo& responseInfo,
        int serial, int responseType, RIL_Errno e, const void *response, size_t responseLen) {
    populateAidlResponseInfo(responseInfo, serial, responseType, e);
    aidl_radio_messaging::SendSmsResult result = {};

    if (response == NULL || responseLen != sizeof(RIL_SMS_Response)) {
        mtkLogE(LOG_TAG, "makeSendSmsResult Invalid response: NULL");
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

int radioMessaging::sendCdmaSmsExpectMoreResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendCdmaSmsExpectMoreResponse: serial %d, slot %d", serial, slotId);

    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};

        aidl_radio_messaging::SendSmsResult result = makeSendSmsResult(responseInfo, serial,
                responseType, e, response, responseLen);

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->sendCdmaSmsExpectMoreResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendCdmaSmsExpectMoreResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "sendCdmaSmsExpectMoreResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::sendCdmaSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendCdmaSmsResponse: serial %d, slot %d", serial, slotId);

    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};

        aidl_radio_messaging::SendSmsResult result = makeSendSmsResult(responseInfo, serial,
                responseType, e, response, responseLen);

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->sendCdmaSmsResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendCdmaSmsResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "sendCdmaSmsResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::sendImsSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendImsSmsResponse: serial %d, slot %d", serial, slotId);

    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};

        aidl_radio_messaging::SendSmsResult result = makeSendSmsResult(responseInfo, serial,
                responseType, e, response, responseLen);

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->sendImsSmsResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendImsSmsResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "sendImsSmsResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::sendSMSExpectMoreResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendSMSExpectMoreResponse: serial %d, slot %d", serial, slotId);

    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};

        aidl_radio_messaging::SendSmsResult result = makeSendSmsResult(responseInfo, serial,
                responseType, e, response, responseLen);

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->sendSmsExpectMoreResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendSMSExpectMoreResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "sendSMSExpectMoreResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::sendSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendSmsResponse: serial %d, slot %d", serial, slotId);

    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};

        aidl_radio_messaging::SendSmsResult result = makeSendSmsResult(responseInfo, serial,
                responseType, e, response, responseLen);

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->sendSmsResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendSmsResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "sendSmsResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::setCdmaBroadcastActivationResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setCdmaBroadcastActivationResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->setCdmaBroadcastActivationResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCdmaBroadcastActivationResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setCdmaBroadcastActivationResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::setGsmBroadcastActivationResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setGsmBroadcastActivationResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->setGsmBroadcastActivationResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setGsmBroadcastActivationResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setGsmBroadcastActivationResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::setGsmBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setGsmBroadcastConfigResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->setGsmBroadcastConfigResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setGsmBroadcastConfigResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setGsmBroadcastConfigResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::setSmscAddressResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setSmscAddressResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->setSmscAddressResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSmscAddressResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setSmscAddressResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::writeSmsToRuimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "writeSmsToRuimResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int retVal = -1;
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "writeSmsToRuimResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            int *p_int = (int *) response;
            retVal = p_int[0];
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->writeSmsToRuimResponse(responseInfo, retVal);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "writeSmsToRuimResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "writeSmsToRuimResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

int radioMessaging::writeSmsToSimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "writeSmsToSimResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].messagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int retVal = -1;
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "writeSmsToSimResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            int *p_int = (int *) response;
            retVal = p_int[0];
        }
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingResponse
                ->writeSmsToSimResponse(responseInfo, retVal);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "writeSmsToSimResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "writeSmsToSimResponse: "
                "radioMessagingServiceImpl[%d]->messagingResponse == NULL", slotId);
    }

    return 0;
}

// indication

bool makeCdmaSmsMessage(aidl_radio_messaging::CdmaSmsMessage &msg,
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

int radioMessaging::cdmaNewSmsInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "cdmaNewSmsInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioMessagingServiceImpl[slotId].messagingIndication != NULL ||
            radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
        aidl_radio_messaging::CdmaSmsMessage msg = {};
        if (!makeCdmaSmsMessage(msg, response, responseLen)) {
            mtkLogE(LOG_TAG, "cdmaNewSmsInd: invalid response");
        }

        if (radioMessagingServiceImpl[slotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingIndication->cdmaNewSms(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), msg);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cdmaNewSmsInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].messagingIndication->cdmaNewSms(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), msg);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "cdmaNewSmsInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaNewSmsInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", slotId);
        mtkLogE(LOG_TAG, "tbox-cdmaNewSmsInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", tboxSlotId);
    }

    return 0;
}

 int radioMessaging::cdmaRuimSmsStorageFullInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "cdmaRuimSmsStorageFullInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioMessagingServiceImpl[slotId].messagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingIndication->cdmaRuimSmsStorageFull(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cdmaRuimSmsStorageFullInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaRuimSmsStorageFullInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", slotId);
    }

    if (radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].messagingIndication->cdmaRuimSmsStorageFull(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cdmaRuimSmsStorageFullInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaRuimSmsStorageFullInd:"
                "tbox-radioMessagingServiceImpl[%d].messagingIndication == NULL", tboxSlotId);
    }

    return 0;
}


int radioMessaging::newBroadcastSmsInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "newBroadcastSmsInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioMessagingServiceImpl[slotId].messagingIndication != NULL ||
            radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newBroadcastSmsInd: invalid response");
            return 0;
        }
        std::vector<uint8_t> data;
        char * resp = (char *) response;
        data.resize(responseLen);
        for (int i = 0 ; i < responseLen; i++) {
            data[i] = resp[i];
        }

        if (radioMessagingServiceImpl[slotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingIndication->newBroadcastSms(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "newBroadcastSmsInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].messagingIndication->newBroadcastSms(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "newBroadcastSmsInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "newBroadcastSmsInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", slotId);
        mtkLogE(LOG_TAG, "tbox-newBroadcastSmsInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", tboxSlotId);
    }

    return 0;
}

int radioMessaging::newSmsInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "newSmsInd, slotid:%d, tboxSlotId:%d, response = %x, responseLen = %d", slotId, tboxSlotId,response,responseLen);
    if (radioMessagingServiceImpl[slotId].messagingIndication != NULL ||
            radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsInd: invalid response");
            return 0;
        }
        uint8_t *bytes = hexStringToBytes(response, responseLen);
        if (bytes == NULL) {
            mtkLogE(LOG_TAG, "newSmsInd: hexStringToBytes failed");
            return 0;
        }
        std::vector<uint8_t> pdu;
        int num = responseLen / 2;
        pdu.resize(num);
        for (int i = 0 ; i < num; i++) {
            pdu[i] = bytes[i];
        }

        if (radioMessagingServiceImpl[slotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingIndication->newSms(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "newSmsInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].messagingIndication->newSms(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "newSmsInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
        free(bytes);
    } else {
        mtkLogE(LOG_TAG, "newSmsInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", slotId);
    }

    return 0;
}

int radioMessaging::newSmsOnSimInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "newSmsOnSimInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioMessagingServiceImpl[slotId].messagingIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsOnSimInd: invalid response");
            return 0;
        }
        int32_t recordNumber = ((int32_t *) response)[0];
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingIndication->newSmsOnSim(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), recordNumber);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "newSmsOnSimInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "newSmsOnSimInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", slotId);
    }

    if (radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsOnSimInd: invalid response");
            return 0;
        }
        int32_t recordNumber = ((int32_t *) response)[0];
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].messagingIndication->newSmsOnSim(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), recordNumber);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "newSmsOnSimInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-newSmsOnSimInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", tboxSlotId);
    }
    return 0;
}

int radioMessaging::newSmsStatusReportInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "newSmsStatusReportInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioMessagingServiceImpl[slotId].messagingIndication != NULL ||
            radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "newSmsStatusReportInd: invalid response");
            return 0;
        }
        uint8_t *bytes = hexStringToBytes(response, responseLen);
        if (bytes == NULL) {
            mtkLogE(LOG_TAG, "newSmsStatusReportInd: hexStringToBytes failed");
            return 0;
        }
        std::vector<uint8_t> pdu;
        int num = responseLen / 2;
        pdu.resize(num);
        for (int i = 0 ; i < num; i++) {
            pdu[i] = bytes[i];
        }

        if (radioMessagingServiceImpl[slotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingIndication->newSmsStatusReport(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "newSmsStatusReportInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
            ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].messagingIndication->newSmsStatusReport(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pdu);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "newSmsStatusReportInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
        free(bytes);
    } else {
        mtkLogE(LOG_TAG, "newSmsStatusReportInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", slotId);
    }

    return 0;
}

int radioMessaging::simSmsStorageFullInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "simSmsStorageFullInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioMessagingServiceImpl[slotId].messagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].messagingIndication->simSmsStorageFull(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simSmsStorageFullInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "simSmsStorageFullInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", slotId);
    }

    if (radioMessagingServiceImpl[tboxSlotId].messagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].messagingIndication->simSmsStorageFull(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simSmsStorageFullInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "simSmsStorageFullInd:"
                "radioMessagingServiceImpl[%d].messagingIndication == NULL", tboxSlotId);
    }
    return 0;
}


int radioMessaging::clearMessagingResponseAndIndications(unsigned int slotId) {
    radioMessagingServiceImpl[slotId].messagingResponse = NULL;
    radioMessagingServiceImpl[slotId]. messagingIndication = NULL;
    return 0;
}

// mtk radio
// response
int mtkRadioExMessaging::getGsmBroadcastActivationRsp(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getGsmBroadcastActivationRsp: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int activation = 0;
        if (response != NULL) {
            int *pInt = (int *) response;
            activation = pInt[0];
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->getGsmBroadcastActivationRsp(responseInfo, activation);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getGsmBroadcastActivationRsp: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getGsmBroadcastActivationRsp: "
                "radioMessagingServiceImpl[%d]->mtkMessagingResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExMessaging::getGsmBroadcastLangsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getGsmBroadcastLangsResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->getGsmBroadcastLangsResponse(responseInfo,
                convertCharPtrToStdString((char *) response));

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getGsmBroadcastLangsResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getGsmBroadcastLangsResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::getSmsMemStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getSmsMemStatusResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        aidl_mtk_radio_messaging::SmsMemStatus params = {};
        if (response == NULL || responseLen != sizeof(RIL_SMS_Memory_Status)) {
            mtkLogE(LOG_TAG, "getSmsMemStatusResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            RIL_SMS_Memory_Status *p_cur = ((RIL_SMS_Memory_Status *) response);
            params.used = p_cur->used;
            params.total = p_cur->total;
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->getSmsMemStatusResponse(responseInfo, params);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSmsMemStatusResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getSmsMemStatusResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::getSmsParametersResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getSmsParametersResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        aidl_mtk_radio_messaging::SmsParams params = {};
        if (response == NULL || responseLen != sizeof(RIL_SmsParams)) {
            mtkLogE(LOG_TAG, "getSmsParametersResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            RIL_SmsParams *p_cur = ((RIL_SmsParams *) response);
            params.format = p_cur->format;
            params.dcs = p_cur->dcs;
            params.vp = p_cur->vp;
            params.pid = p_cur->pid;
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->getSmsParametersResponse(responseInfo, params);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSmsParametersResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getSmsParametersResponse:"
            "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::getSmsRuimMemoryStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getSmsRuimMemoryStatusResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);

        aidl_mtk_radio_messaging::SmsMemStatus status = {};
        if (response == NULL || responseLen != sizeof (RIL_SMS_Memory_Status)) {
            mtkLogE(LOG_TAG, "getSmsRuimMemoryStatusResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            RIL_SMS_Memory_Status *mem_status = (RIL_SMS_Memory_Status*)response;
            status.used = mem_status->used;
            status.total = mem_status->total;
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->getSmsRuimMemoryStatusResponse(responseInfo, status);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSmsParametersResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getSmsRuimMemoryStatusResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::removeCbMsgResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "removeCbMsgResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->removeCbMsgResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "removeCbMsgResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "removeCbMsgResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::setEtwsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setEtwsResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->setEtwsResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setEtwsResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setEtwsResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::setGsmBroadcastLangsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setGsmBroadcastLangsResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->setGsmBroadcastLangsResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setGsmBroadcastLangsResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setGsmBroadcastLangsResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::setSmsParametersResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setSmsParametersResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->setSmsParametersResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSmsParametersResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setSmsParametersResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

int mtkRadioExMessaging::exitSCBMResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "exitSCBMResponse: serial %d, slot %d", serial, slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingResponse
                ->exitSCBMResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitSCBMResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "exitSCBMResponse:"
                "mtkRadioExService[%d]->mRadioResponseMtk == NULL", slotId);
    }
    return 0;
}

// indication
int mtkRadioExMessaging::esnMeidChangeInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "esnMeidChangeInd, slotid:%d", slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "esnMeidChangeInd: invalid response");
            return 0;
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingIndication->esnMeidChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *)response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "esnMeidChangeInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "esnMeidChangeInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "tbox-esnMeidChangeInd: invalid response");
            return 0;
        }

        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication->esnMeidChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *)response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "esnMeidChangeInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-esnMeidChangeInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExMessaging::meSmsStorageFullInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "meSmsStorageFullInd, slotid:%d", slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingIndication->meSmsStorageFullInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "meSmsStorageFullInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "meSmsStorageFullInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "meSmsStorageFullInd, tboxSlotId:%d", tboxSlotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication->meSmsStorageFullInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "meSmsStorageFullInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-meSmsStorageFullInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExMessaging::smsInfoExtInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "smsInfoExtInd, slotid:%d", slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingIndication->smsInfoExtInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *)response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "smsInfoExtInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "smsInfoExtInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "smsInfoExtInd, tboxSlotId:%d", tboxSlotId);
    if (radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication->smsInfoExtInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *)response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "smsInfoExtInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-smsInfoExtInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", tboxSlotId);
    }

    return 0;
}

int mtkRadioExMessaging::smsReadyInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "smsReadyInd, slotid:%d", slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingIndication->smsReadyInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "smsReadyInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "smsReadyInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "smsReadyInd, tboxSlotId:%d", tboxSlotId);
    if (radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication->smsReadyInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "smsReadyInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-smsReadyInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExMessaging::enterSCBMInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "enterSCBMInd, slotid:%d", slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingIndication->enterSCBMInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enterSCBMInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "enterSCBMInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "enterSCBMInd, tboxSlotId:%d", tboxSlotId);
    if (radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication->enterSCBMInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enterSCBMInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-enterSCBMInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExMessaging::exitSCBMInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "exitSCBMInd, slotid:%d", slotId);
    if (radioMessagingServiceImpl[slotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[slotId].mtkMessagingIndication->exitSCBMInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitSCBMInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "exitSCBMInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "exitSCBMInd, slotid:%d", slotId);
    if (radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication != NULL) {
        ndk::ScopedAStatus ret = radioMessagingServiceImpl[tboxSlotId].mtkMessagingIndication->exitSCBMInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "exitSCBMInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-exitSCBMInd:"
                "radioMessagingServiceImpl[%d].mtkMessagingIndication == NULL", tboxSlotId);
    }
    return 0;
}

namespace android::hardware::radio::messaging {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::android::hardware::radio::messaging;
constexpr auto ok = &ScopedAStatus::ok;

RadioMessaging::RadioMessaging(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus RadioMessaging::acknowledgeIncomingGsmSmsWithPdu(
        int32_t serial, bool success, const std::string& ackPdu) {
    mtkLogD(LOG_TAG, "acknowledgeIncomingGsmSmsWithPdu: serial %d", serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId,
            RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU, false,
            2, success ? "1" : "0", ackPdu.c_str());
    return ok();
}

ScopedAStatus RadioMessaging::acknowledgeLastIncomingCdmaSms(
        int32_t serial, const aidl::CdmaSmsAck& smsAck) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingCdmaSms: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE);
    if (pRI == NULL) {
         return ok();
    }
    RIL_CDMA_SMS_Ack rcsa = {};
    rcsa.uErrorClass = (RIL_CDMA_SMS_ErrorClass) smsAck.errorClass;
    rcsa.uSMSCauseCode = smsAck.smsCauseCode;
    CALL_ONREQUEST(pRI->pCI->requestNumber, &rcsa, sizeof(rcsa), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioMessaging::acknowledgeLastIncomingGsmSms(
        int32_t serial, bool success, aidl::SmsAcknowledgeFailCause cause) {
    mtkLogD(LOG_TAG, "acknowledgeLastIncomingGsmSms: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SMS_ACKNOWLEDGE, 2, BOOL_TO_INT(success), cause);
    return ok();
}

ScopedAStatus RadioMessaging::deleteSmsOnRuim(int32_t serial, int32_t index) {
    mtkLogD(LOG_TAG, "deleteSmsOnRuim: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM, 1, index);
    return ok();
}

ScopedAStatus RadioMessaging::deleteSmsOnSim(int32_t serial, int32_t index) {
    mtkLogD(LOG_TAG, "deleteSmsOnSim:: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_DELETE_SMS_ON_SIM, 1, index);
    return ok();
}

ScopedAStatus RadioMessaging::getCdmaBroadcastConfig(int32_t serial) {
    mtkLogD(LOG_TAG, "getCdmaBroadcastConfig:: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG);
    return ok();
}

ScopedAStatus RadioMessaging::getGsmBroadcastConfig(int32_t serial) {
    mtkLogD(LOG_TAG, "getGsmBroadcastConfig: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG);
    return ok();
}

ScopedAStatus RadioMessaging::getSmscAddress(int32_t serial) {
    mtkLogD(LOG_TAG, "getSmscAddress: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_SMSC_ADDRESS);
    return ok();
}

ScopedAStatus RadioMessaging::reportSmsMemoryStatus(int32_t serial, bool available) {
    mtkLogD(LOG_TAG, "reportSmsMemoryStatus: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_REPORT_SMS_MEMORY_STATUS, 1,
        BOOL_TO_INT(available));
    return ok();
}

ScopedAStatus RadioMessaging::responseAcknowledgement() {
    android::releaseWakeLock();
    return ok();
}

void constructCdmaSms(RIL_CDMA_SMS_Message &rcsm, const aidl::CdmaSmsMessage& sms) {
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

ScopedAStatus RadioMessaging::sendCdmaSms(int32_t serial, const aidl::CdmaSmsMessage& sms) {
    mtkLogD(LOG_TAG, "sendCdmaSms: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_CDMA_SEND_SMS);
    if (pRI == NULL) {
        return ok();
    }
    RIL_CDMA_SMS_Message rcsm = {};
    constructCdmaSms(rcsm, sms);
    CALL_ONREQUEST(pRI->pCI->requestNumber, &rcsm, sizeof(rcsm), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioMessaging::sendCdmaSmsExpectMore(int32_t serial,
        const aidl::CdmaSmsMessage& sms) {
    mtkLogD(LOG_TAG, "sendCdmaSmsExpectMore: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_CDMA_SEND_SMS_EXPECT_MORE);
    if (pRI == NULL) {
        return ok();
    }
    RIL_CDMA_SMS_Message rcsm = {};
    constructCdmaSms(rcsm, sms);
    CALL_ONREQUEST(pRI->pCI->requestNumber, &rcsm, sizeof(rcsm), pRI, mSlotId);
    return ok();
}

bool dispatchImsGsmSms(const aidl::ImsSmsMessage& message, RequestInfo *pRI) {
    RIL_IMS_SMS_Message rism = {};
    char **pStrings;
    int countStrings = 2;
    int dataLen = sizeof(char *) * countStrings;

    rism.tech = RADIO_TECH_3GPP;
    rism.retry = BOOL_TO_INT(message.retry);
    rism.messageRef = message.messageRef;

    if (message.gsmMessage.size() != 1) {
        mtkLogE(LOG_TAG, "dispatchImsGsmSms: Invalid len %s",
                requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    }

    pStrings = (char **)calloc(countStrings, sizeof(char *));
    if (pStrings == NULL) {
        mtkLogE(LOG_TAG, "dispatchImsGsmSms: Memory allocation failed for request %s",
                requestToString(pRI->pCI->requestNumber));
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

bool dispatchImsCdmaSms(const aidl::ImsSmsMessage& message, RequestInfo *pRI) {
    ImsCdmaSms temp = {};

    if (message.cdmaMessage.size() != 1) {
        mtkLogE(LOG_TAG, "dispatchImsCdmaSms: Invalid len %s",
                requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    }

    temp.imsSms.tech = RADIO_TECH_3GPP2;
    temp.imsSms.retry = BOOL_TO_INT(message.retry);
    temp.imsSms.messageRef = message.messageRef;
    temp.imsSms.message.cdmaMessage = &temp.cdmaSms;

    constructCdmaSms(temp.cdmaSms, message.cdmaMessage[0]);

    // Vendor code expects payload length to include actual msg payload
    // (sizeof(RIL_CDMA_SMS_Message)) instead of (RIL_CDMA_SMS_Message *) + size of other fields in
    // RIL_IMS_SMS_Message
    int payloadLen = sizeof(RIL_RadioTechnologyFamily) + sizeof(uint8_t) + sizeof(int32_t)
            + sizeof(RIL_CDMA_SMS_Message);

    CALL_ONREQUEST(pRI->pCI->requestNumber, &temp.imsSms, payloadLen, pRI, pRI->socket_id);

    return true;
}

ScopedAStatus RadioMessaging::sendImsSms(int32_t serial, const aidl::ImsSmsMessage& message) {
    mtkLogD(LOG_TAG, "sendImsSms: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_IMS_SEND_SMS);
    if (pRI == NULL) {
        return ok();
    }
    if (aidl_radio::RadioTechnologyFamily::THREE_GPP == message.tech) {
        dispatchImsGsmSms(message, pRI);
    } else if (aidl_radio::RadioTechnologyFamily::THREE_GPP2 == message.tech) {
        dispatchImsCdmaSms(message, pRI);
    } else {
        mtkLogE(LOG_TAG, "sendImsSms: Invalid radio tech %s",
                requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
    }
    return ok();
}

ScopedAStatus RadioMessaging::sendSms(int32_t serial, const aidl::GsmSmsMessage& message) {
    mtkLogD(LOG_TAG, "sendSms: serial %d", serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_SEND_SMS, false,
            2, message.smscPdu.c_str(), message.pdu.c_str());
    return ok();
}

ScopedAStatus RadioMessaging::sendSmsExpectMore(int32_t serial, const aidl::GsmSmsMessage& msg) {
    mtkLogD(LOG_TAG, "sendSmsExpectMore: serial %d", serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_SEND_SMS_EXPECT_MORE, false,
            2, msg.smscPdu.c_str(), msg.pdu.c_str());
    return ok();
}

ScopedAStatus RadioMessaging::setCdmaBroadcastActivation(int32_t serial, bool activate) {
    mtkLogD(LOG_TAG, "setCdmaBroadcastActivation: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION,
            1, BOOL_TO_INT(!activate));
    return ok();
}

ScopedAStatus RadioMessaging::setCdmaBroadcastConfig(
        int32_t serial, const std::vector<aidl::CdmaBroadcastSmsConfigInfo>& cfgInfo) {
    mtkLogD(LOG_TAG, "setCdmaBroadcastConfig: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG);
    if (pRI == NULL) {
        return ok();
    }
    int num = cfgInfo.size();
    RIL_CDMA_BroadcastSmsConfigInfo cdmaBci[num];
    RIL_CDMA_BroadcastSmsConfigInfo *cdmaBciPtrs[num];
    for (int i = 0 ; i < num ; i++ ) {
        cdmaBciPtrs[i] = &cdmaBci[i];
        cdmaBci[i].service_category = cfgInfo[i].serviceCategory;
        cdmaBci[i].language = cfgInfo[i].language;
        cdmaBci[i].selected = BOOL_TO_INT(cfgInfo[i].selected);
    }
    CALL_ONREQUEST(pRI->pCI->requestNumber, cdmaBciPtrs,
            num * sizeof(RIL_CDMA_BroadcastSmsConfigInfo *), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioMessaging::setGsmBroadcastActivation(int32_t serial, bool activate) {
    mtkLogD(LOG_TAG, "setGsmBroadcastActivation: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION,
            1, BOOL_TO_INT(!activate));
    return ok();
}

ScopedAStatus RadioMessaging::setGsmBroadcastConfig(
        int32_t serial, const std::vector<aidl::GsmBroadcastSmsConfigInfo>& configInfo) {
    mtkLogD(LOG_TAG, "setGsmBroadcastConfig: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG);
    if (pRI == NULL) {
        return ok();
    }
    int num = configInfo.size();
    RIL_GSM_BroadcastSmsConfigInfo gsmBci[num];
    RIL_GSM_BroadcastSmsConfigInfo *gsmBciPtrs[num];
    for (int i = 0 ; i < num ; i++ ) {
        gsmBciPtrs[i] = &gsmBci[i];
        gsmBci[i].fromServiceId = configInfo[i].fromServiceId;
        gsmBci[i].toServiceId = configInfo[i].toServiceId;
        gsmBci[i].fromCodeScheme = configInfo[i].fromCodeScheme;
        gsmBci[i].toCodeScheme = configInfo[i].toCodeScheme;
        gsmBci[i].selected = BOOL_TO_INT(configInfo[i].selected);
    }
    CALL_ONREQUEST(pRI->pCI->requestNumber, gsmBciPtrs,
            num * sizeof(RIL_GSM_BroadcastSmsConfigInfo *), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioMessaging::setResponseFunctions(
        const std::shared_ptr<aidl::IRadioMessagingResponse>& messagingResponse,
        const std::shared_ptr<aidl::IRadioMessagingIndication>& messagingIndication) {
    mtkLogD(LOG_TAG, "aosp messaging setResponseFunctions, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp messaging setResponseFunctions: got lock %d", mSlotId);

    radioMessagingServiceImpl[mSlotId].messagingResponse = messagingResponse;
    radioMessagingServiceImpl[mSlotId]. messagingIndication = messagingIndication;
    plusCounterRadioNumOne(mSlotId);

    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        rilAidlUtils::setAospResponseNumberToOne(rilAidlUtils::MESSAGE_INDEX, mSlotId);
    }

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp messaging setResponseFunctions: release lock %d", mSlotId);
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

ScopedAStatus RadioMessaging::setSmscAddress(int32_t serial, const std::string& smsc) {
    mtkLogD(LOG_TAG, "setSmscAddress: serial %d", serial);
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_SET_SMSC_ADDRESS,
            smsc.c_str());
    return ok();
}

ScopedAStatus RadioMessaging::writeSmsToRuim(int32_t serial, const aidl::CdmaSmsWriteArgs& sms) {
    mtkLogD(LOG_TAG, "writeSmsToRuim: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM);
    if (pRI == NULL) {
        return ok();
    }
    RIL_CDMA_SMS_WriteArgs rcsw = {};
    rcsw.status = (int) sms.status;
    constructCdmaSms(rcsw.message, sms.message);
    CALL_ONREQUEST(pRI->pCI->requestNumber, &rcsw, sizeof(rcsw), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioMessaging::writeSmsToSim(int32_t serial, const aidl::SmsWriteArgs& smsWrArgs) {
    mtkLogD(LOG_TAG, "writeSmsToSim: serial %d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_WRITE_SMS_TO_SIM);
    if (pRI == NULL) {
        return ok();
    }
    RIL_SMS_WriteArgs args;
    args.status = (int) smsWrArgs.status;
    if (!copyAidlStringToRil(&args.pdu, smsWrArgs.pdu, pRI)) {
        return ok();
    }
    if (!copyAidlStringToRil(&args.smsc, smsWrArgs.smsc, pRI)) {
        memsetAndFreeStrings(1, args.pdu);
        return ok();
    }
    CALL_ONREQUEST(RIL_REQUEST_WRITE_SMS_TO_SIM, &args, sizeof(args), pRI, mSlotId);
    memsetAndFreeStrings(2, args.smsc, args.pdu);
    return ok();
}

}  // namespace android::hardware::radio::messaging


namespace vendor::mediatek::hardware::mtkradioex::messaging {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::messaging;
constexpr auto ok = &ScopedAStatus::ok;

MtkRadioMessaging::MtkRadioMessaging(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus MtkRadioMessaging::getGsmBroadcastActivation(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getGsmBroadcastActivation: serial %d, clientid %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_GSM_SMS_BROADCAST_ACTIVATION);
    return ok();
}

ScopedAStatus MtkRadioMessaging::getGsmBroadcastLangs(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getGsmBroadcastLangs: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GSM_GET_BROADCAST_LANGUAGE);
    return ok();
}

ScopedAStatus MtkRadioMessaging::getSmsMemStatus(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getSmsMemStatus: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_SMS_SIM_MEM_STATUS);
    return ok();
}

ScopedAStatus MtkRadioMessaging::getSmsParameters(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getSmsParameters: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_SMS_PARAMS);
    return ok();
}

ScopedAStatus MtkRadioMessaging::getSmsRuimMemoryStatus(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getSmsRuimMemoryStatus: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_GET_SMS_RUIM_MEM_STATUS);
    return ok();
}

ScopedAStatus MtkRadioMessaging::removeCbMsg(int32_t serial, int32_t channelId,
       int32_t serialId, int32_t clientId) {
    mtkLogD(LOG_TAG, "removeCbMsg: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, sId(clientId), RIL_REQUEST_REMOVE_CB_MESSAGE, 2, channelId, serialId);
    return ok();
}

ScopedAStatus MtkRadioMessaging::setEtws(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setEtws: serial %d, clientid %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, sId(clientId), RIL_REQUEST_SET_ETWS, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioMessaging::setGsmBroadcastLangs(int32_t serial,
         const std::string& langs, int32_t clientId) {
    mtkLogD(LOG_TAG, "setGsmBroadcastLangs: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchString(serial, sId(clientId), RIL_REQUEST_GSM_SET_BROADCAST_LANGUAGE,
            langs.c_str());
    return ok();
}

bool dispatchSmsParametrs(int serial, int slotId, int request, const aidl::SmsParams& message) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    RIL_SmsParams params;
    memset (&params, 0, sizeof(RIL_SmsParams));

    params.dcs = message.dcs;
    params.format = message.format;
    params.pid = message.pid;
    params.vp = message.vp;

    CALL_ONREQUEST(request, &params, sizeof(params), pRI, slotId);

    return true;
}

ScopedAStatus MtkRadioMessaging::setSmsParameters(int32_t serial,
        const aidl::SmsParams& message, int32_t clientId) {
    mtkLogD(LOG_TAG, "setSmsParameters: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchSmsParametrs(serial, sId(clientId), RIL_REQUEST_SET_SMS_PARAMS, message);
    return ok();
}

ScopedAStatus MtkRadioMessaging::exitSCBM(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "exitSCBM: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, sId(clientId), RIL_REQUEST_EXIT_SMS_SCBM);
    return ok();
}

ScopedAStatus MtkRadioMessaging::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus MtkRadioMessaging::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExMessagingResponse>& radioResponse,
        const std::shared_ptr<aidl::IMtkRadioExMessagingIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk messaging setResponseFunctions, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk messaging setResponseFunctions: got lock %d", mSlotId);

    radioMessagingServiceImpl[mSlotId].mtkMessagingResponse = radioResponse;
    radioMessagingServiceImpl[mSlotId].mtkMessagingIndication = radioIndication;
    plusCounterMtkRadioNumOne(mSlotId);

    rilAidlUtils::setMtkResponseNumberToOne(rilAidlUtils::MESSAGE_INDEX, mSlotId);
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk messaging setResponseFunctions: release lock %d", mSlotId);

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

}

int mtkRadioExMessaging::clearMtkMessagingResponseAndIndications(unsigned int slotId) {
    radioMessagingServiceImpl[slotId].mtkMessagingResponse = NULL;
    radioMessagingServiceImpl[slotId].mtkMessagingIndication = NULL;
    return 0;
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addMessingLock(std::shared_ptr<ndk::ICInterface> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_MESSAGING_PORT_BASE 54000
#define MTK_RADIO_MESSAGING_PORT_BASE 54100

static pthread_t gThreadIds[MAX_SIM_COUNT*2];

void* addRcpVsockRadioMessagingAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_MESSAGING_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_MESSAGING_PORT_BASE + sloid, sloid);

    std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> radioMessagingAidl = ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(sloid);
    addMessingLock(std::static_pointer_cast<ndk::ICInterface>(radioMessagingAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioMessagingAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void* addRcpVsockMtkRadioMessagingAidlService(void* arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, MTK_RADIO_MESSAGING_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", MTK_RADIO_MESSAGING_PORT_BASE + sloid, sloid);
    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkRadiooMessagingAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(sloid);
    addMessingLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadiooMessagingAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);

    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadiooMessagingAidl->asBinder().get()));
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioMessagingVsock(unsigned int &sloid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
            addRcpVsockRadioMessagingAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioMessage");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT], NULL,
            addRcpVsockMtkRadioMessagingAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockMtkRadioMessage");
    }
}
#endif



void publicRadioMessagingAidlService(unsigned int sloid) {

    /*if (sloid == 0) {
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> radioMessagingAidl = ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(0);
        gPublishedHals.push_back(radioMessagingAidl);
        const std::string instance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/slot1";
        binder_status_t status =
                AServiceManager_addService(radioMessagingAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp messaging addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(0);
        gPublishedHals.push_back(mtkRadioMessagingAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/slot1";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioMessagingAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk messaging addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 1) {
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> radioMessagingAidl = ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(1);
        gPublishedHals.push_back(radioMessagingAidl);
        const std::string instance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/slot2";
        binder_status_t status =
                AServiceManager_addService(radioMessagingAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp messaging addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(1);
        gPublishedHals.push_back(mtkRadioMessagingAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/slot2";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioMessagingAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk messaging addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 2) {
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> radioMessagingAidl = ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(2);
        gPublishedHals.push_back(radioMessagingAidl);
        const std::string instance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/slot3";
        binder_status_t status =
                AServiceManager_addService(radioMessagingAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp messaging addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(2);
        gPublishedHals.push_back(mtkRadioMessagingAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/slot3";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioMessagingAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk messaging addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 3) {
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> radioMessagingAidl = ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(3);
        gPublishedHals.push_back(radioMessagingAidl);
        const std::string instance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/slot4";
        binder_status_t status =
                AServiceManager_addService(radioMessagingAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp messaging addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(3);
        gPublishedHals.push_back(mtkRadioMessagingAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/slot4";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioMessagingAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk messaging addservice, slotId:%d, status:%d", sloid, mtkStatus);
    }*/

    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;

    if (sloid == 0) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioMessagingVsock(sloid);
#endif

        // add service for android client
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> tboxRadioMessagingAidl =
                ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioMessagingAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/tboxSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioMessagingAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkTboxRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioMessagingAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/tboxSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioMessagingAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 1) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioMessagingVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> tboxRadioMessagingAidl =
                ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioMessagingAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/tboxSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioMessagingAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkTboxRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioMessagingAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/tboxSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioMessagingAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 2) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioMessagingVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> tboxRadioMessagingAidl =
                ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioMessagingAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioMessagingAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkTboxRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioMessagingAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioMessagingAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 3) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioMessagingVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::messaging::RadioMessaging> tboxRadioMessagingAidl =
                ndk::SharedRefBase::make<android::hardware::radio::messaging::RadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioMessagingAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::messaging::RadioMessaging::descriptor + "/tboxSlot4";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioMessagingAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging> mtkTboxRadioMessagingAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging>(tboxSlotid);
        addMessingLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioMessagingAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::messaging::MtkRadioMessaging::descriptor + "/tboxSlot4";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioMessagingAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox messaging addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    }
}
