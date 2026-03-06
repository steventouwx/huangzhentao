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

#include <cstddef>
#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <mtk_log.h>
#include <radiomodem_service.h>
#include <rilAidlUtils.h>
#include <binder/ProcessState.h>
#include <android/binder_manager.h>
#include <libmtkrilutils.h>
#include <ril_service.h>
#include <mtk_properties.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mtkconfigutils.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

namespace aidl_radio = ::aidl::android::hardware::radio;

using rilAidlUtils::retChecker;

struct RadioModemImpl {
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemResponse> mtkModemResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemIndication> mtkModemIndication;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemResponse> mtkModemImsResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemIndication> mtkModemImsIndication;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::em::IEmRadioResponse> mtkEmResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::em::IEmRadioIndication> mtkEmIndication;
    std::shared_ptr<::aidl::android::hardware::radio::modem::IRadioModemResponse> modemResponse;
    std::shared_ptr<::aidl::android::hardware::radio::modem::IRadioModemIndication> modemIndication;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::cap::IMtkRadioExCapRadioResponse> mtkCapResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::atci::IAtciResponse> mtkAtciResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::atci::IAtciIndication> mtkAtciIndication;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemResponse> mtkModemGbaResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::assist::IAssistModemResponse> mtkModemAssistResponse;
};

static const char PROPERTY_DC_CALL_ENABLE[4][32] = {
    "persist.vendor.mtk.dch.enable",
    "persist.vendor.mtk.dch.enable.2",
    "persist.vendor.mtk.dch.enable.3",
    "persist.vendor.mtk.dch.enable.4"
};

RadioModemImpl radioModemServiceImpl[MAX_SIM_COUNT*(android::CLIENT_TBOX_IMS + 1)];
static RIL_ImsPendingInd *s_ims_pending_ind[MAX_SIM_COUNT] = {NULL};
static timer_t sAospModemRilServiceDogTimer;
static timer_t sMtkModemRilServiceDogTimer;

void notifyModemImsServiceReady(unsigned int slot) {
    // Fire pending indication here because the function must be called after all event
    // registrations are ready in the constructor of the class ImsService.java
    //unsigned int slot = toRealSlot(mSlotId);
    slot = slot % MAX_SIM_COUNT;
    RIL_ImsPendingInd *node = s_ims_pending_ind[slot];
    RIL_ImsPendingInd *pre_node = NULL;
    while (node != NULL) {
        // Fire pending indication
        mtkLogI(LOG_TAG, "notifyModemImsServiceReady, [%d]Fire Pending indication %s(%d)",
                slot, android::requestToString(node->request), node->slot);
        switch(node->request) {
            case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED:
                radioModem::imsRadioStateChangedInd(
                        node->slot, node->responseType, 0, RIL_E_SUCCESS, node->data,
                        node->data_size);
                break;
        }
        pre_node = node;
        node = node->next;
        if (pre_node->data != NULL) {
            free(pre_node->data);
        }
        free(pre_node);
    }
    s_ims_pending_ind[slot] = NULL;
}

void appendImsIndIntoPendingList(unsigned int slotId, int request, int responseType,
        const void* data, int size) {
    slotId = slotId % MAX_SIM_COUNT;
    if (request != RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED) {
        mtkLogE(LOG_TAG,
            "appendImsIndIntoPendingList, Request %s no need to append into modem_ims pending ind list",
               android::requestToString(request));
        return;
    }

    bool ignore_duplicate_request = false;
    bool is_addNode = true;
    // If it is not necessary to keep multiple same request id in the pendding list,
    // to add the request id here
    if (request == RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED) {
        ignore_duplicate_request = true;
    }

    unsigned int slot = slotId % MAX_SIM_COUNT;//toRealSlot(slotId);
    // Cache the indication
    RIL_ImsPendingInd *node = s_ims_pending_ind[slot];
    while (node != NULL) {
        if (ignore_duplicate_request && node->request == request) {
            mtkLogD(LOG_TAG,
                "appendImsIndIntoPendingList, Request %s(%d) is already in modem_ims pending ind list",
                android::requestToString(request), slot);
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
            mtkLogE(LOG_TAG, "appendImsIndIntoPendingList: node calloc NULL.");
        } else {
            node->slot = slotId;
            node->request = request;
            node->responseType = responseType;
            node->next = NULL;
            if (data != NULL) {
                node->data = (void*) calloc(1, size);
                if (node->data == NULL) {
                    mtkLogE(LOG_TAG, "appendImsIndIntoPendingList, node->data calloc NULL.");
                    is_addNode = false;
                    free(node);
                } else {
                    memcpy(node->data, data, size);
                    node->data_size = size;
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
            mtkLogE(LOG_TAG, "appendImsIndIntoPendingList: item calloc NULL.");
        } else {
            item->slot = slotId;
            item->request = request;
            item->responseType = responseType;
            item->next = NULL;
            if (data != NULL) {
                item->data = (void*) calloc(1, size);
                if (item->data == NULL) {
                    mtkLogE(LOG_TAG, "appendImsIndIntoPendingList, item->data calloc NULL.");
                    is_addNode = false;
                    free(item);
                } else {
                    memcpy(item->data, data, size);
                    item->data_size = size;
                }
            }
            if (is_addNode) {
                node->next = item;
            }
        }
    }

    mtkLogI(LOG_TAG,
        "appendImsIndIntoPendingList, Request %s(%d) already append into modem_ims pending ind list",
        android::requestToString(request), slot);
}

void radioModemAcknowledgeRequest(unsigned int slotId, int serial) {
    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->acknowledgeRequest(serial);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "radioModemAcknowledgeRequest: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioModemAcknowledgeRequest: radioModemServiceImpl[%d]->modemResponse == NULL", slotId);
    }
}

// AOSP radio
int radioModem::enableModemResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "enableModemResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, err);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->enableModemResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enableModemResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "enableModemResponse: radioModemServiceImpl[%d]->modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem::getBasebandVersionResponse(unsigned int slotId, android::ClientId clientId __unused,
                                     int responseType, int serial, RIL_Errno e,
                                     const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getBasebandVersionResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::string final_res("");
        if (response != NULL) {
            std::string res((char*) response);
            size_t slot_count = getSimCount();
            /*
             * We use the actual used slot count instead of MAX_SIM_COUNT.
             * To prevent any problem after multi-sim switching,
             * we set slot_count to 2 at least.
             */
            if (slot_count < 2) slot_count = 2;
            size_t max_len = (90 /* fwk max property size*/ / slot_count) - 1 /* -1 for ',' */;
            final_res = res.substr(0, max_len);
        }
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getBasebandVersionResponse(
                responseInfo, final_res);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getBasebandVersionResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getBasebandVersionResponse: radioModemServiceImpl[%d]->modemResponse == NULL", slotId);
    }

    return 0;
}

int radioModem::getDeviceIdentityResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responselen) {
    mtkLogD(LOG_TAG, "getDeviceIdentityResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        int numStrings = responselen / sizeof(char *);
        std::string emptyString("");
        if (response == NULL || numStrings != 4) {
            mtkLogE(LOG_TAG, "getDeviceIdentityResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getDeviceIdentityResponse(
                    responseInfo, emptyString, emptyString, emptyString, emptyString);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getDeviceIdentityResponse: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        } else {
            char **resp = (char **) response;
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getDeviceIdentityResponse(
                    responseInfo,
                    convertCharPtrToStdString(resp[0]),
                    convertCharPtrToStdString(resp[1]),
                    convertCharPtrToStdString(resp[2]),
                    convertCharPtrToStdString(resp[3]));
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "getDeviceIdentityResponse: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "getDeviceIdentityResponse: radioModemServiceImpl[%d]->modemResponse == NULL", slotId);
    }
    return 0;
}

void convertRilHardwareConfigListToAidl(const void *response, size_t responseLen,
        std::vector<aidl_radio::modem::HardwareConfig>& records) {
    int num = responseLen / sizeof(RIL_HardwareConfig);
    records.resize(num);

    RIL_HardwareConfig *rilHardwareConfig = (RIL_HardwareConfig *) response;
    for (int i = 0; i < num; i++) {
        records[i].type = static_cast<int32_t>(rilHardwareConfig[i].type);
        records[i].uuid = convertCharPtrToStdString(rilHardwareConfig[i].uuid);
        records[i].state = static_cast<int32_t>(rilHardwareConfig[i].state);
        switch (rilHardwareConfig[i].type) {
            case RIL_HARDWARE_CONFIG_MODEM: {
                records[i].modem.resize(1);
                records[i].sim.resize(0);
                aidl_radio::modem::HardwareConfigModem *hwConfigModem = &records[i].modem[0];
                hwConfigModem->rat = aidl_radio::RadioTechnology(rilHardwareConfig[i].cfg.modem.rat);
                hwConfigModem->maxVoiceCalls = rilHardwareConfig[i].cfg.modem.maxVoice;
                hwConfigModem->maxDataCalls = rilHardwareConfig[i].cfg.modem.maxData;
                hwConfigModem->maxStandby = rilHardwareConfig[i].cfg.modem.maxStandby;
                break;
            }

            case RIL_HARDWARE_CONFIG_SIM: {
                records[i].sim.resize(1);
                records[i].modem.resize(0);
                records[i].sim[0].modemUuid =
                        convertCharPtrToStdString(rilHardwareConfig[i].cfg.sim.modemUuid);
                break;
            }
        }
    }
}


int radioModem::getHardwareConfigResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getHardwareConfigResponse: slotId:%d, serial:%d", slotId, serial);
    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl_radio::modem::HardwareConfig> result;
        if ((response == NULL && responseLen != 0)
                || responseLen % sizeof(RIL_HardwareConfig) != 0) {
            mtkLogE(LOG_TAG, "hardwareConfigChangedInd: invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            if (response != NULL) {
                convertRilHardwareConfigListToAidl(response, responseLen, result);
            } else {
                mtkLogE(LOG_TAG, "getHardwareConfigResponse: slotId = %d, response == NULL", slotId);
            }
        }

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getHardwareConfigResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getHardwareConfigResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getHardwareConfigResponse: radioModemServiceImpl[%d]->modemResponse == NULL", slotId);
    }

    return 0;
}

int radioModem::getModemActivityInfoResponse(unsigned int slotId, android::ClientId clientId,
                                int responseType, int serial, RIL_Errno e,
                                const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "getModemActivityInfoResponse: slotId:%d, serial %d", slotId, serial);
    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        aidl_radio::modem::ActivityStatsInfo info;
        if (response == NULL || responselen != sizeof(RIL_ActivityStatsInfo)) {
            mtkLogE(LOG_TAG, "getModemActivityInfoResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            RIL_ActivityStatsInfo *resp = (RIL_ActivityStatsInfo *)response;
            info.sleepModeTimeMs = resp->sleep_mode_time_ms;
            info.idleModeTimeMs = resp->idle_mode_time_ms;
            aidl_radio::modem::ActivityStatsTechSpecificInfo techSpecificInfo;
            techSpecificInfo.rat = aidl_radio::AccessNetwork(aidl_radio::AccessNetwork::UNKNOWN),
            techSpecificInfo.frequencyRange = static_cast<int32_t>(aidl_radio::modem::ActivityStatsTechSpecificInfo::FREQUENCY_RANGE_UNKNOWN);
            techSpecificInfo.rxModeTimeMs = resp->rx_mode_time_ms;
            for(int i = 0; i < RIL_NUM_TX_POWER_LEVELS; i++) {
                techSpecificInfo.txmModetimeMs.push_back(resp->tx_mode_time_ms[i]);
            }
            info.techSpecificInfo.push_back(techSpecificInfo);
        }

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getModemActivityInfoResponse(responseInfo,
                info);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getModemActivityInfoResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getModemActivityInfoResponse: radioModemServiceImpl[%d]->modemResponse == NULLL",
                slotId);
    }
    return 0;
}

int radioModem::getModemStackStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "getModemStackStatusResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
#if defined(ANDROID_MULTI_SIM)
        RIL_RadioState radioState = s_vendorFunctions->onStateRequest((RIL_SOCKET_ID)slotId);
#else
        RIL_RadioState radioState = s_vendorFunctions->onStateRequest();
#endif
        bool  isEnable;
        if (radioState == RIL_RadioState::RADIO_STATE_ON) {
            isEnable = true;
        } else if (radioState == RIL_RadioState::RADIO_STATE_OFF) {
            isEnable = false;
        } else {
            isEnable = false;
            e = RIL_E_RADIO_NOT_AVAILABLE;
        }
        mtkLogD(LOG_TAG, "getModemStackStatusResponse: slotid:%d, isEnable %d, e = %d", slotId, isEnable, e);

        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getModemStackStatusResponse(
                responseInfo, isEnable);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getModemStackStatusResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getModemStackStatusResponse: radioModemServiceImpl[%d].modemResponse == NULL",
                slotId);
    }
    return 0;
}

void convertRilRadioCapabilityToAidl(const void *response, size_t responseLen, aidl_radio::modem::RadioCapability& rc) {
    RIL_RadioCapability *rilRadioCapability = (RIL_RadioCapability *) response;
    rc.session = rilRadioCapability->session;
    rc.phase = rilRadioCapability->phase;
    rc.raf = rilRadioCapability->rat;
    rc.logicalModemUuid = convertCharPtrToStdString(rilRadioCapability->logicalModemUuid);
    rc.status =rilRadioCapability->status;
}

void responseAidlRadioCapability(aidl_radio::RadioResponseInfo& responseInfo, int serial,
        int responseType, RIL_Errno e, const void *response, size_t responseLen, aidl_radio::modem::RadioCapability& rc) {
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    if (response == NULL || responseLen != sizeof(RIL_RadioCapability)) {
        mtkLogE(LOG_TAG, "responseRadioCapability: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        std::string emptyString("");
        rc.logicalModemUuid = emptyString;
    } else {
        convertRilRadioCapabilityToAidl(response, responseLen, rc);
    }
}

int radioModem::getRadioCapabilityResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getRadioCapabilityResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio::modem::RadioCapability result = {};

        responseAidlRadioCapability(responseInfo, serial, responseType, e, response, responseLen,
                result);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getRadioCapabilityResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getRadioCapabilityResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getRadioCapabilityResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem::nvReadItemResponse(unsigned int slotId, android::ClientId clientId,
                      int responseType, int serial, RIL_Errno e,
                      const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "nvReadItemResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->nvReadItemResponse(
                responseInfo,
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "nvReadItemResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "nvReadItemResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem::nvResetConfigResponse(unsigned int slotId, android::ClientId clientId,
                         int responseType, int serial, RIL_Errno e,
                         const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "nvResetConfigResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->nvResetConfigResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "nvResetConfigResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "nvResetConfigResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }

    return 0;
}

int radioModem::nvWriteCdmaPrlResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "nvWriteCdmaPrlResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->nvWriteCdmaPrlResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "nvWriteCdmaPrlResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "nvWriteCdmaPrlResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem:: nvWriteItemResponse(unsigned int slotId, android::ClientId clientId,
                       int responseType, int serial, RIL_Errno e,
                       const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "nvWriteItemResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->nvWriteItemResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "nvWriteItemResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "nvWriteItemResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem::requestShutdownResponse(unsigned int slotId, android::ClientId clientId,
                           int responseType, int serial, RIL_Errno e,
                           const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "requestShutdownResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->requestShutdownResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "requestShutdownResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "requestShutdownResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem::sendDeviceStateResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "sendDeviceStateResponse: slotId:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[slotId].modemResponse->sendDeviceStateResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendDeviceStateResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "sendDeviceStateResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem::setRadioCapabilityResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setRadioCapabilityResponse: slotid:%d, serial %d", slotId, serial);

    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio::modem::RadioCapability result = {};
        responseAidlRadioCapability(responseInfo, serial, responseType, e, response, responseLen,
                result);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->setRadioCapabilityResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendDeviceStateResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setRadioCapabilityResponse: radioModemServiceImpl[%d].modemResponse == NULL", slotId);
    }

    return 0;
}

int radioModem::setRadioPowerResponse(unsigned int slotId, android::ClientId clientId,
                         int responseType, int serial, RIL_Errno e, const void *response,
                         size_t responselen) {
    mtkLogD(LOG_TAG, "setRadioPowerResponse: slotid:%d, serial %d", slotId, serial);
    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->setRadioPowerResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setRadioPowerResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setRadioPowerResponse: radioModemServiceImpl[%d]->modemResponse == NULL", slotId);
    }
    return 0;
}

int radioModem::getImeiResponse(unsigned int slotId, android::ClientId clientId,
                         int responseType, int serial, RIL_Errno e, const void *response,
                         size_t responselen) {
    mtkLogD(LOG_TAG, "getImeiResponse: slotid:%d, serial %d", slotId, serial);
    if (radioModemServiceImpl[slotId].modemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        aidl_radio::modem::ImeiInfo result = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int numStrings = responselen / sizeof(char*);
        if (response == NULL || responselen % sizeof(char*) != 0 || numStrings != 3) {
            mtkLogE(LOG_TAG, "getImeiResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            //split response, type.imei,svn
            char **pString = (char **) response;
            if (strncmp(pString[0], "1", 1) == 0) {
                result.type = aidl_radio::modem::ImeiInfo::ImeiType::PRIMARY;
            } else if (strncmp(pString[0], "2", 1) == 0) {
                result.type = aidl_radio::modem::ImeiInfo::ImeiType::SECONDARY;
            }
            result.imei = convertCharPtrToStdString(pString[1]);
            result.svn = convertCharPtrToStdString(pString[2]);
        }
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemResponse->getImeiResponse(responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getImeiResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getImeiResponse: radioModemServiceImpl[%d]->modemResponse == NULL", slotId);
    }
   return 0;
}

int radioModem::hardwareConfigChangedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "hardwareConfigChangedInd slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioModemServiceImpl[slotId].modemIndication != NULL ||
            radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
        if (response == NULL || responselen % sizeof(RIL_HardwareConfig) != 0) {
            mtkLogE(LOG_TAG, "hardwareConfigChangedInd: invalid response");
            return 0;
        }

        std::vector<aidl_radio::modem::HardwareConfig> configs;
        convertRilHardwareConfigListToAidl(response, responselen, configs);

        if (radioModemServiceImpl[slotId].modemIndication != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemIndication->hardwareConfigChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), configs);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "hardwareConfigChangedInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].modemIndication->hardwareConfigChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), configs);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "hardwareConfigChangedInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "hardwareConfigChangedInd: radioModemServiceImpl[%d].modemIndication == NULL",
                slotId);
        mtkLogE(LOG_TAG, "tbox-hardwareConfigChangedInd: radioModemServiceImpl[%d].modemIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioModem::modemResetInd(unsigned int slotId,
                  int indicationType, int token, RIL_Errno e, const void *response,
                  size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "modemResetInd slotId:%d, slotId, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioModemServiceImpl[slotId].modemIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "modemResetInd: invalid response");
            return 0;
        }

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemIndication->modemReset(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "modemResetInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "modemResetInd: radioModemServiceImpl[%d].modemIndication == NULL", slotId);
    }

    if (radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
        if (response == NULL || responselen == 0) {
            mtkLogE(LOG_TAG, "tbox-modemResetInd: invalid response");
            return 0;
        }

        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].modemIndication->modemReset(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "modemResetInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "modemResetInd: tbox-radioModemServiceImpl[%d].modemIndication == NULL", tboxSlotId);
    }
    return 0;
}


int radioModem::radioCapabilityIndicationInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e, const void *response,
                                 size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "radioCapabilityIndicationInd slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioModemServiceImpl[slotId].modemIndication != NULL ||
            radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
        if (response == NULL || responselen != sizeof(RIL_RadioCapability)) {
            mtkLogE(LOG_TAG, "radioCapabilityIndicationInd: invalid response");
            mtkLogE(LOG_TAG, "tbox-radioCapabilityIndicationInd: invalid response");
            return 0;
        }

        aidl_radio::modem::RadioCapability rc = {};
        convertRilRadioCapabilityToAidl(response, responselen, rc);
        if (radioModemServiceImpl[slotId].modemIndication != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemIndication->radioCapabilityIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), rc);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "radioCapabilityIndicationInd: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].modemIndication->radioCapabilityIndication(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), rc);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "radioCapabilityIndicationInd: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "radioCapabilityIndicationInd: radioModemServiceImpl[%d].modemIndication == NULL",
                slotId);
        mtkLogE(LOG_TAG, "tbox-radioCapabilityIndicationInd: radioModemServiceImpl[%d].modemIndication == NULL",
                tboxSlotId);
    }

    return 0;
}

int radioModem::radioStateChangedInd(unsigned int slotId,
                          int indicationType, int token, RIL_Errno e, const void *response,
                          size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if(s_vendorFunctions == NULL) {
        mtkLogE(LOG_TAG, "radioStateChangedInd: service is not ready");
        return 0;
    }

#if defined(ANDROID_MULTI_SIM)
    RIL_RadioState radioState = s_vendorFunctions->onStateRequest((RIL_SOCKET_ID)slotId);
#else
    RIL_RadioState radioState = s_vendorFunctions->onStateRequest();
#endif
    mtkLogD(LOG_TAG, "radioStateChangedInd: radioState %d, slot = %d", radioState, slotId);

    if (radioModemServiceImpl[slotId].modemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemIndication->radioStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                aidl_radio::modem::RadioState(radioState));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "radioStateChanged: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioStateChanged: radioModemServiceImpl[%d].modemIndication == NULL", slotId);
    }

    // add indication to CLIENT_TBOX
    if (radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].modemIndication->radioStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                aidl_radio::modem::RadioState(radioState));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "radioStateChanged: slotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-radioStateChanged: radioModemServiceImpl[%d].modemIndication == NULL", tboxSlotId);
    }

    // add indication to CLIENT_IMS
    imsRadioStateChangedInd(slotId, indicationType, token, e, response, responseLen);

    unsigned int seSlotId = slotId + android::CLIENT_SE * MAX_SIM_COUNT;
    if (radioModemServiceImpl[seSlotId].modemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[seSlotId].modemIndication->radioStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                aidl_radio::modem::RadioState(radioState));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "radioStateChangedInd: seSlotId = %d, binder fail!", seSlotId);
            retChecker->aospAidlReturnStatusChecker(seSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioStateChangedInd: radioModemServiceImpl[%d].modemIndication == NULL", seSlotId);
    }

    // Send to EM(TODO)
    //slotId = slotId % MAX_SIM_COUNT;
    if (radioModemServiceImpl[slotId].mtkEmIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkEmIndication->radioStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                aidl_radio::modem::RadioState(radioState));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "radioStateChanged: slotId = %d, binder fail!", slotId);
        }
    } else {
        mtkLogE(LOG_TAG, "radioStateChanged: radioModemServiceImpl[%d].mtkEmIndication == NULL", slotId);
    }
    return 0;
}

int radioModem::imsRadioStateChangedInd(unsigned int slotId,
                     int indicationType, int token, RIL_Errno e, const void *response,
                     size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if(s_vendorFunctions == NULL) {
        mtkLogE(LOG_TAG, "imsRadioStateChangedInd: service is not ready");
        return 0;
    }
#if defined(ANDROID_MULTI_SIM)
    RIL_RadioState radioState = s_vendorFunctions->onStateRequest((RIL_SOCKET_ID)slotId);
#else
    RIL_RadioState radioState = s_vendorFunctions->onStateRequest();
#endif
    mtkLogD(LOG_TAG, "imsRadioStateChangedInd: radioState %d, slot = %d", radioState, slotId);

    unsigned int imsSlotId = slotId + android::CLIENT_IMS *MAX_SIM_COUNT;
    if (radioModemServiceImpl[imsSlotId].modemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[imsSlotId].modemIndication->radioStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                aidl_radio::modem::RadioState(radioState));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "imsRadioStateChangedInd: imsSlotId = %d, binder fail!", imsSlotId);
            appendImsIndIntoPendingList(slotId, RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                        indicationType, response, responseLen);
        }
    } else {
        mtkLogE(LOG_TAG, "imsRadioStateChangedInd: ims radioModemServiceImpl[%d].modemIndication == NULL", imsSlotId);
        appendImsIndIntoPendingList(slotId, RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                    indicationType, response, responseLen);
    }

    unsigned int tboxImsSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX_IMS;
    if (radioModemServiceImpl[tboxImsSlotId].modemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxImsSlotId].modemIndication->radioStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                aidl_radio::modem::RadioState(radioState));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "imsRadioStateChangedInd: tboxImsSlotId = %d, binder fail!", tboxImsSlotId);
            appendImsIndIntoPendingList(slotId, RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                        indicationType, response, responseLen);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-imsRadioStateChangedInd: ims radioModemServiceImpl[%d].modemIndication == NULL", imsSlotId);
        appendImsIndIntoPendingList(slotId, RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                    indicationType, response, responseLen);
    }
    return 0;
}

int radioModem::rilConnectedInd(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "rilConnectedInd, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioModemServiceImpl[slotId].modemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemIndication->rilConnected(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "rilConnectedInd: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "rilConnectedInd: radioModemServiceImpl[%d].modemIndication == NULL", slotId);
    }

    if (radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].modemIndication->rilConnected(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "rilConnectedInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-rilConnectedInd: radioModemServiceImpl[%d].modemIndication == NULL", tboxSlotId);
    }
    return 0;
}

int radioModem::onImeiMappingChanged(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responselen) {
    int tboxSlotId = slotId + MAX_SIM_COUNT * (android::CLIENT_COUNT-1);
    mtkLogD(LOG_TAG, "onImeiMappingChanged, slotid:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioModemServiceImpl[slotId].modemIndication != NULL ||
            radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
        aidl_radio::modem::ImeiInfo result = {};
        int numStrings = responselen / sizeof(char*);
        if (response == NULL || responselen % sizeof(char*) != 0 || numStrings != 3) {
            mtkLogE(LOG_TAG, "onImeiMappingChanged Invalid response: NULL");
            return 0;
        }
        //split response, type.imei,svn
        char **pString = (char **) response;
        if (strncmp(pString[0], "1", 1) == 0) {
            result.type = aidl_radio::modem::ImeiInfo::ImeiType::PRIMARY;
        } else if (strncmp(pString[0], "2", 1) == 0) {
            result.type = aidl_radio::modem::ImeiInfo::ImeiType::SECONDARY;
        }
        result.imei = convertCharPtrToStdString(pString[1]);
        result.svn = convertCharPtrToStdString(pString[2]);
        if (radioModemServiceImpl[slotId].modemIndication != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].modemIndication->onImeiMappingChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), result);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onImeiMappingChanged: slotId = %d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioModemServiceImpl[tboxSlotId].modemIndication != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].modemIndication->onImeiMappingChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), result);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onImeiMappingChanged: tboxSlotId = %d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }    
    } else {
        mtkLogE(LOG_TAG, "onImeiMappingChanged:radioModemServiceImpl[%d].modemIndication == NULL", slotId);
    }
    return 0;
}


int radioModem::clearModemResponseAndIndications(unsigned int slotId) {
    radioModemServiceImpl[slotId].modemResponse = NULL;
    radioModemServiceImpl[slotId].modemIndication = NULL;
    return 0;
}


namespace android::hardware::radio::modem {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::android::hardware::radio::modem;
constexpr auto ok = &ScopedAStatus::ok;

RadioModem::RadioModem(unsigned int slotid) {
    mSlotId = slotid;
}

ScopedAStatus RadioModem::enableModem(int32_t serial, bool on) {
    mtkLogD(LOG_TAG, "enableModem:: slotId:%d, serial %d on %d", mSlotId, serial, on);
    dispatchInts(serial, mSlotId, RIL_REQUEST_ENABLE_MODEM, 1, BOOL_TO_INT(on));
    return ok();
}

ScopedAStatus RadioModem::getBasebandVersion(int32_t serial) {
    mtkLogD(LOG_TAG, "getBasebandVersion:: slotId:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_BASEBAND_VERSION);
    return ok();
}

ScopedAStatus RadioModem::getDeviceIdentity(int32_t serial) {
    mtkLogD(LOG_TAG, "getDeviceIdentity:: slotId:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_DEVICE_IDENTITY);
    return ok();
}

ScopedAStatus RadioModem::getHardwareConfig(int32_t serial) {
    mtkLogD(LOG_TAG, "getHardwareConfig:: slotId:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_HARDWARE_CONFIG);
    return ok();
}

ScopedAStatus RadioModem::getModemActivityInfo(int32_t serial) {
    mtkLogD(LOG_TAG, "getModemActivityInfo:: slotId:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_ACTIVITY_INFO);
    return ok();
}

ScopedAStatus RadioModem::getModemStackStatus(int32_t serial) {
    // Retrive Radio State
    mtkLogD(LOG_TAG, "getModemStackStatus:: radioState slotId:%d, serial:%d", mSlotId, serial);
    radioModem::getModemStackStatusResponse(mSlotId, getClientBySlot(mSlotId), RESPONSE_SOLICITED,
            serial, RIL_E_SUCCESS, NULL, 0);
    return ok();
}

ScopedAStatus RadioModem::getRadioCapability(int32_t serial) {
    mtkLogD(LOG_TAG, "getRadioCapability:: slotId:%d, serial:%d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_RADIO_CAPABILITY);
    return ok();
}

ScopedAStatus RadioModem::nvReadItem(int32_t serial, aidl::NvItem itemId) {
    mtkLogD(LOG_TAG, "nvReadItem:: slotId:%d, serial %d", mSlotId, serial);
    android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_NV_READ_ITEM);
    if (pRI == NULL) {
        return ok();
    }

    RIL_NV_ReadItem nvri = {};
    nvri.itemID = (RIL_NV_Item) itemId;

    CALL_ONREQUEST(pRI->pCI->requestNumber, &nvri, sizeof(nvri), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioModem::nvResetConfig(int32_t serial, aidl::ResetNvType resetType) {
    int rilResetType = -1;
    mtkLogD(LOG_TAG, "nvResetConfig:: slotId:%d, serial:%d", mSlotId, serial);
    /* Convert ResetNvType to RIL.h values
     * RIL_REQUEST_NV_RESET_CONFIG
     * 1 - reload all NV items
     * 2 - erase NV reset (SCRTN)
     * 3 - factory reset (RTN)
     */
    switch(resetType) {
        case aidl::ResetNvType::RELOAD:
            rilResetType = 1;
        break;
        case aidl::ResetNvType::ERASE:
            rilResetType = 2;
        break;
        case aidl::ResetNvType::FACTORY_RESET:
            rilResetType = 3;
        break;
    }
    dispatchInts(serial, mSlotId, RIL_REQUEST_NV_RESET_CONFIG, 1, rilResetType);
    return ok();
}

ScopedAStatus RadioModem::nvWriteCdmaPrl(int32_t serial, const std::vector<uint8_t>& prl) {
    mtkLogD(LOG_TAG, "nvWriteCdmaPrl:: slotId:%d, serial:%d", mSlotId, serial);
    rilAidlUtils::dispatchAidlRaw(serial, mSlotId, RIL_REQUEST_NV_WRITE_CDMA_PRL, prl);
    return ok();
}

ScopedAStatus RadioModem::nvWriteItem(int32_t serial, const aidl::NvWriteItem& item) {
    mtkLogD(LOG_TAG, "nvWriteItem: slotId:%d, serial:%d", mSlotId, serial);
    android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_NV_WRITE_ITEM);
    if (pRI == NULL) {
        return ok();
    }

    RIL_NV_WriteItem nvwi = {};

    nvwi.itemID = (RIL_NV_Item) item.itemId;

    if (!rilAidlUtils::copyAidlStringToRil(&nvwi.value, item.value, pRI)) {
        return ok();
    }

    CALL_ONREQUEST(pRI->pCI->requestNumber, &nvwi, sizeof(nvwi), pRI, mSlotId);

    memsetAndFreeStrings(1, nvwi.value);
    return ok();
}

ScopedAStatus RadioModem::requestShutdown(int32_t serial) {
    mtkLogD(LOG_TAG, "requestShutdown:: slotId:%d, serial %d", mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_SHUTDOWN);
    return ok();
}

ScopedAStatus RadioModem::sendDeviceState(int32_t serial, aidl::DeviceStateType type, bool state) {
    mtkLogD(LOG_TAG, "sendDeviceState:: slotId:%d, serial %d", mSlotId, serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SEND_DEVICE_STATE, 2, (int) type,
            BOOL_TO_INT(state));
    return ok();
}

ScopedAStatus RadioModem::setRadioCapability(int32_t serial, const aidl::RadioCapability& rc) {
    mtkLogD(LOG_TAG, "setRadioCapability:: slotId:%d, serial:%d", mSlotId, serial);
    android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_SET_RADIO_CAPABILITY);
    if (pRI == NULL) {
        return ok();
    }

    RIL_RadioCapability rilRc = {};

    // TODO : set rilRc.version using HIDL version ?
    rilRc.session = rc.session;
    rilRc.phase = (int) rc.phase;
    rilRc.rat = (int) rc.raf;
    rilRc.status = (int) rc.status;
    memset(rilRc.logicalModemUuid, 0x00, MAX_UUID_LENGTH);
    strncpy(rilRc.logicalModemUuid, rc.logicalModemUuid.c_str(), MAX_UUID_LENGTH - 1);

    CALL_ONREQUEST(pRI->pCI->requestNumber, &rilRc, sizeof(rilRc), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioModem::setRadioPower(int32_t serial, bool powerOn, bool forEmergencyCall,
                                        bool preferredForEmergencyCall) {
    mtkLogD(LOG_TAG, "setRadioPower:: slotId:%d, serial %d on %d, isEcc %d, preEcc, %d", mSlotId, serial, powerOn,
            forEmergencyCall, preferredForEmergencyCall);
    dispatchInts(serial, mSlotId, RIL_REQUEST_RADIO_POWER, 3, BOOL_TO_INT(powerOn),
            BOOL_TO_INT(forEmergencyCall), BOOL_TO_INT(preferredForEmergencyCall));
    return ok();
}

ScopedAStatus RadioModem::getImei(int32_t serial) {
    mtkLogD(LOG_TAG, "aosp modem getImei, slotId:%d", mSlotId);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_DEVICE_IMEI);
    return ok();
}

ScopedAStatus RadioModem::responseAcknowledgement() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus RadioModem::setResponseFunctions(
        const std::shared_ptr<aidl::IRadioModemResponse>& modemResponse,
        const std::shared_ptr<aidl::IRadioModemIndication>& modemIndication) {
    mtkLogD(LOG_TAG, "aosp modem setResponseFunctions, slotId:%d", mSlotId);

    if (mSlotId < getSimCount()) {
        rilAidlUtils::ril_service_stop_watch_dog(sAospModemRilServiceDogTimer);
        rilAidlUtils::ril_service_start_watch_dog(sAospModemRilServiceDogTimer, 30 * 1000);
    }

    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp modem setResponseFunctions: got lock %d", mSlotId);

    radioModemServiceImpl[mSlotId].modemResponse = modemResponse;
    radioModemServiceImpl[mSlotId].modemIndication = modemIndication;
    plusCounterRadioNumOne(mSlotId);

    android::ClientId client = getClientBySlot(mSlotId);

    if (android::CLIENT_IMS == client) {
        notifyModemImsServiceReady(mSlotId);
    } else if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        rilAidlUtils::setAospResponseNumberToOne(rilAidlUtils::MODEM_INDEX, mSlotId);
    }

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp modem setResponseFunctions: release lock %d", mSlotId);
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

}  // android::hardware::radio::modem


// mtkradio

enum VendorSetting {
    VENDOR_SETTING_CXP_CONFIG_OPTR,
    VENDOR_SETTING_CXP_CONFIG_SPEC,
    VENDOR_SETTING_CXP_CONFIG_SEG,
    VENDOR_SETTING_CXP_CONFIG_SBP,
    VENDOR_SETTING_CXP_CONFIG_SUBID,
    VENDOR_SETTING_BIP_PDN_REUSE,
    VENDOR_SETTING_BIP_OVERRIDE_APN,
    VENDOR_SETTING_BIP_PDN_NAME_REUSE,
    VENDOR_SETTING_RADIO_AIRPLANE_MODE,
    VENDOR_SETTING_RADIO_SIM_MODE,
    VENDOR_SETTING_RADIO_SILENT_REBOOT,
    VENDOR_SETTING_VOLTE_ENABLE,
    VENDOR_SETTING_VILTE_ENABLE,
    VENDOR_SETTING_VIWIFI_ENABLE,
    VENDOR_SETTING_RCS_UA_ENABLE,
    VENDOR_SETTING_DATA_SSC_MODE,
    VENDOR_SETTING_WFC_ENABLE,
    VENDOR_SETTING_DCH_ENABLE,
};


namespace mtk_aidl = ::aidl::vendor::mediatek::hardware::mtkradioex;
namespace vendor::mediatek::hardware::mtkradioex::modem {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::modem;
namespace aidl_cap = ::aidl::vendor::mediatek::hardware::mtkradioex::cap;
namespace aidl_atci = ::aidl::vendor::mediatek::hardware::mtkradioex::atci;

constexpr auto ok = &ScopedAStatus::ok;

MtkRadioModem::MtkRadioModem(unsigned int slotid) {
    mSlotId = slotid;
}

ScopedAStatus MtkRadioModem::abortCertificate(int32_t serial, int32_t uid, int32_t clientId) {
    mtkLogD(LOG_TAG, "abortCertificate: serial: %d, clientId %d, slotId:%d",
            serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_CAP)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ABORT_CERTIFICATE, 1, uid);
    return ok();
}

ScopedAStatus MtkRadioModem::enableCapability(int32_t serial, const std::string& id,
        int32_t uid, int32_t toActive, int32_t clientId) {
    mtkLogD(LOG_TAG, "enableCapability: serial: %d, clientId %d, slotId:%d",
            serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_CAP)) {
        return ok();
    }

    android::RequestInfo *pRI = android::addRequestToList(serial,
            mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ENABLE_CAPABILITY);
    if (pRI == NULL) {
        return ok();
    }

    RIL_Capability capMsg;
    if (!rilAidlUtils::copyAidlStringToRil(&capMsg.id, id, pRI)) {
        return ok();
    }
    capMsg.uid = uid;
    capMsg.toActive = toActive;
    CALL_ONREQUEST(RIL_REQUEST_ENABLE_CAPABILITY, &capMsg, sizeof(capMsg), pRI, pRI->socket_id);
    memsetAndFreeStrings(1, capMsg.id);

    return ok();
}

ScopedAStatus MtkRadioModem::getEngineeringModeInfo(int32_t serial, int32_t index, int32_t clientId) {
    mtkLogD(LOG_TAG, "getEngineeringModeInfo:%d, %d, %d", serial, index, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    dispatchInts(serial, mSlotId, RIL_REQUEST_GET_ECHOLOCATE_METRICS, 1, index);
    return ok();
}

ScopedAStatus MtkRadioModem::modifyModemType(int32_t serial, int32_t applyType, int32_t modemType,
        int32_t clientId) {
    mtkLogD(LOG_TAG, "RadioImpl::modifyModemType:%d, %d, %d", serial, applyType, modemType);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_EM) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_MODIFY_MODEM_TYPE,
            2, applyType, modemType);
    return ok();
}

ScopedAStatus MtkRadioModem::restartRILD(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "restartRILD: serial: %d, clientId %d, slotId:%d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_RESTART_RILD);
    return ok();
}

ScopedAStatus MtkRadioModem::routeAuthMessage(int32_t serial, int32_t uid,
        const std::vector<uint8_t>& msg, int32_t clientId) {
    mtkLogD(LOG_TAG, "routeAuthMessage: serial: %d, clientId %d, slotId:%d",
            serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_CAP)) {
        return ok();
    }

    android::RequestInfo *pRI = android::addRequestToList(serial,
            mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ROUTE_AUTH);
    if (pRI == NULL) {
        return ok();
    }

    RIL_AuthMsg authMsg;
    authMsg.uid = uid;
    authMsg.msg = (char*) calloc(1, sizeof(char) * msg.size());
    if ((authMsg.msg != NULL) && (msg.data() != NULL)) {
        memcpy(authMsg.msg, msg.data(), (sizeof(char) * msg.size()));
    }
    authMsg.msgLength = msg.size();
    mtkLogD(LOG_TAG, "routeAuthMessage: msg legnth %d", authMsg.msgLength);

    CALL_ONREQUEST(RIL_REQUEST_ROUTE_AUTH, &authMsg, sizeof(authMsg), pRI, pRI->socket_id);
    free(authMsg.msg);
    return ok();
}

ScopedAStatus MtkRadioModem::routeCertificate(int32_t serial, int32_t uid,
        const std::vector<uint8_t>& cert, const std::vector<uint8_t>& msg, int32_t clientId) {
    mtkLogD(LOG_TAG, "routeCertificate: serial: %d, clientId %d, slotId:%d",
            serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_CAP)) {
        return ok();
    }

    android::RequestInfo *pRI = android::addRequestToList(serial,
            mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ROUTE_CERTIFICATE);
    if (pRI == NULL) {
        return ok();
    }

    RIL_CertMsg certMsg;
    certMsg.uid = uid;
    certMsg.cert = (char*) calloc(1, sizeof(char) * cert.size());
    if ((certMsg.cert != NULL) && (cert.data() != NULL)) {
        memcpy(certMsg.cert, cert.data(), (sizeof(char) * cert.size()));
    }
    certMsg.certLength = cert.size();
    certMsg.msg = (char*) calloc(1, sizeof(char) * msg.size());
    if ((certMsg.msg != NULL) && (msg.data() != NULL)) {
        memcpy(certMsg.msg, msg.data(), (sizeof(char) * msg.size()));
    }
    certMsg.msgLength = msg.size();
    mtkLogD(LOG_TAG, "routeCertificate: cert length %d, msg legnth %d", certMsg.certLength,
            certMsg.msgLength);

    CALL_ONREQUEST(RIL_REQUEST_ROUTE_CERTIFICATE, &certMsg, sizeof(certMsg), pRI, pRI->socket_id);
    free(certMsg.cert);
    free(certMsg.msg);
    return ok();
}

ScopedAStatus MtkRadioModem::sendAtciRequest(int32_t serial, const std::vector<uint8_t>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendAtciRequest: serial %d, clientId %d, slotId %d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchAidlRaw(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_OEM_HOOK_ATCI_INTERNAL, data);
    return ok();
}

ScopedAStatus MtkRadioModem::sendEmbmsAtCommand(int32_t serial, const std::string& data, int32_t clientId) {
    return ok();
}


ScopedAStatus MtkRadioModem::sendRequestRaw(int32_t serial, const std::vector<uint8_t>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendRequestRaw: serial %d, clientId %d, slotId %d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_EM) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    int slotId = mSlotId + clientId * MAX_SIM_COUNT;
    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_TBOX) {
        slotId = mSlotId;
    }
    rilAidlUtils::dispatchAidlRaw(serial, slotId, RIL_REQUEST_OEM_HOOK_RAW, data);
    return ok();
}

ScopedAStatus MtkRadioModem::sendRequestStrings(int32_t serial, const std::vector<std::string>& data, int32_t clientId) {
    mtkLogD(LOG_TAG, "sendRequestStrings: slotId:%d, serial %d, clientId %d", mSlotId, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_EM) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_OEM_HOOK_STRINGS, data);
    return ok();
}

ScopedAStatus MtkRadioModem::sendSarIndicator(int32_t serial, int32_t sar_cmd_type, const std::string& sar_parameter, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, par:%s, clientId %d, slotId %d", __func__,
            serial, sar_parameter.c_str(), clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SEND_SAR_IND, true, 2,
            (std::to_string(sar_cmd_type)).c_str(),
            sar_parameter.c_str());
    return ok();
}

ScopedAStatus MtkRadioModem::setMaxUlSpeed(int32_t serial, int32_t ulSpeed, int32_t clientId) {
    // not support
    return ok();
}

ScopedAStatus MtkRadioModem::setModemPower(int32_t serial, bool isOn, int32_t clientId) {
    mtkLogD(LOG_TAG, "setModemPower: serial: %d, isOn: %d, clientId %d, slotId %d", serial, isOn, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    if (isOn) {
        dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_MODEM_POWERON);
    } else {
        dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_MODEM_POWEROFF);
    }
    return ok();
}

ScopedAStatus MtkRadioModem::setTrm(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setTrm: serial: %d, mode: %d, clientId %d, slotId %d", serial, mode, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_EM) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_TRM, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioModem::setTxPower(int32_t serial, int32_t limitpower, int32_t clientId) {
    mtkLogD(LOG_TAG, "setTxPower: serial %d, clientId %d, slotId %d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_TX_POWER, 1, limitpower);
    return ok();
}

ScopedAStatus MtkRadioModem::setTxPowerStatus(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogI(LOG_TAG, "setTxPowerStatus: serial %d, enale:%d, clientId:%d, slotId %d", serial, mode, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_TX_POWER_STATUS, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioModem::setVendorSetting(int32_t serial, int32_t setting, const std::string& value, int32_t clientId) {
    mtkLogD(LOG_TAG, "setVendorSetting: serial: %d, setting %d, slotId %d, clientId %d",
            serial, setting, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    switch (setting) {
        // example//
        // case xxx :
        //    mtk_property_set(xxx, yyy);
        //    break;
        case VendorSetting::VENDOR_SETTING_CXP_CONFIG_OPTR:
            mtk_property_set("persist.vendor.operator.optr", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_CXP_CONFIG_SPEC:
            mtk_property_set("persist.vendor.operator.spec", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_CXP_CONFIG_SEG:
            mtk_property_set("persist.vendor.operator.seg", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_CXP_CONFIG_SBP: {
            mtk_property_set("persist.vendor.mtk_usp_md_sbp_code", value.c_str());

            // Enable silent reboot to avoid duplicate SIM PIN after modem reset
            mtk_property_set("vendor.gsm.ril.eboot", "1");

            // Write MD SBP ID for CCCI
            int fd;
            char *fdValue = NULL;
            int size;
            char buf[9];
            int ret = -1;
            int valueSize = 0;

            // set umask for necessary file permission
            umask(S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH);
            fd = open("/mnt/vendor/nvdata/APCFG/APRDCL/CXP_SBP", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            // Recovery umask to RILD default setting
            umask(S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
            if (fd >= 0) {
                fdValue = (char*) malloc(20);
                if (fdValue == NULL) {
                    mtkLogE(LOG_TAG, "SetVendorSetting: NULL memory allocated");
                    close(fd);
                    return ok();
                }
                memset(fdValue, 0x00, 20);
                valueSize = (strlen(value.c_str()) > 8)? 8:strlen(value.c_str());
                memcpy(fdValue, value.c_str(), valueSize);

                size = snprintf(buf, 9, "%s", fdValue);
                ret = write(fd, buf, size);
                mtkLogE(LOG_TAG, "[CXP][%s]fd:%d, write ret value:%d, error=%d\n",
                        __func__, fd, ret, errno);
                free(fdValue);
                close(fd);
            } else {
                mtkLogE(LOG_TAG, "[CXP][%s]Open NVDATA CXP_SBP fail, fd:%d, error=%d\n",
                        __func__, fd, errno);
            }
            break;
        }
        case VendorSetting::VENDOR_SETTING_CXP_CONFIG_SUBID:
            mtk_property_set("persist.vendor.operator.subid", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_BIP_PDN_REUSE:
            mtk_property_set("vendor.ril.pdn.reuse", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_BIP_OVERRIDE_APN:
            mtk_property_set("vendor.ril.pdn.overrideApn", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_BIP_PDN_NAME_REUSE:
            mtk_property_set("vendor.ril.pdn.name.reuse", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_RADIO_AIRPLANE_MODE:
            mtk_property_set("persist.vendor.radio.airplane.mode.on",
                atoi(value.c_str())? "true": "false");
            break;
        case VendorSetting::VENDOR_SETTING_RADIO_SIM_MODE:
            mtk_property_set("persist.vendor.radio.sim.mode", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_RCS_UA_ENABLE:
            mtk_property_set("persist.vendor.mtk_rcs_ua_support", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_RADIO_SILENT_REBOOT:
            mtk_property_set("vendor.gsm.ril.eboot", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_VOLTE_ENABLE: {
                FeatureValue feature;
                if (strlen(value.c_str()) == 1) {
                    strncpy(feature.value, value.c_str(), strlen(value.c_str()) + 1);
                    mtkSetFeature(CONFIG_VOLTE, &feature);
                }
            }
            break;
        case VendorSetting::VENDOR_SETTING_VILTE_ENABLE: {
                FeatureValue feature;
                if (strlen(value.c_str()) == 1) {
                    strncpy(feature.value, value.c_str(), strlen(value.c_str()) + 1);
                    mtkSetFeature(CONFIG_VILTE, &feature);
                }
            }
            break;
        case VendorSetting::VENDOR_SETTING_VIWIFI_ENABLE: {
                FeatureValue feature;
                if (strlen(value.c_str()) == 1) {
                    strncpy(feature.value, value.c_str(), strlen(value.c_str()) + 1);
                    mtkSetFeature(CONFIG_VIWIFI, &feature);
                }
            }
            break;
        case VendorSetting::VENDOR_SETTING_WFC_ENABLE:
            mtk_property_set("persist.vendor.mtk_wfc_support", value.c_str());
            break;
        case VendorSetting::VENDOR_SETTING_DCH_ENABLE:
            mtkLogD(LOG_TAG, "SetVendorSetting: dch enable len:%d", strlen(value.c_str()));
            if (strlen(value.c_str()) == 1) {
                mtk_property_set(PROPERTY_DC_CALL_ENABLE[mSlotId], value.c_str());
                if (atoi(value.c_str()) > 0) {
                    mtk_property_set("persist.vendor.mtk_dc_call_capability", value.c_str());
                }
                rilAidlUtils::dispatchString(serial, mSlotId + clientId * MAX_SIM_COUNT,
                    RIL_REQUEST_DCH_ENABLE_REQ, value.c_str());
            }
            break;
        default :
            break;
    }
    mtkRadioExModem::setVendorSettingResponse(mSlotId + clientId * MAX_SIM_COUNT,
                                    getClientBySlot(mSlotId + clientId * MAX_SIM_COUNT), RESPONSE_SOLICITED,
                                  serial, RIL_E_SUCCESS, NULL, 0);
    return ok();
}

ScopedAStatus MtkRadioModem::triggerModeSwitchByEcc(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogI(LOG_TAG, "triggerModeSwitchByEcc: serial %d, mode:%d, clientId:%d, slotId %d",
            serial, mode, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SWITCH_MODE_FOR_ECC, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioModem::runGbaAuthentication(int32_t serial, const std::string& nafFqdn,
        const std::string& nafSecureProtocolId, bool forceRun, int32_t netId, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, slotId %d, clientId %d", __func__, serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_IMS) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_GBA) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX_IMS)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_RUN_GBA, true, 4,
            nafFqdn.c_str(),
            nafSecureProtocolId.c_str(),
            forceRun ? "1" : "0",
            std::to_string(netId).c_str());
    return ok();
}

ScopedAStatus MtkRadioModem::sendWifiAssociated(int32_t serial, const std::string& ifName, int32_t associated, const std::string& ssid, const std::string& apMac, int32_t mtuSize,
        const std::string& ueMac, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, slotId %d, clientId %d", __func__, serial, mSlotId, clientId);
    return ok();
}

ScopedAStatus MtkRadioModem::sendWifiEnabled(int32_t serial, const std::string& ifName, int32_t isWifiEnabled, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, slotId %d, clientId %d", __func__, serial, mSlotId, clientId);
    return ok();
}

ScopedAStatus MtkRadioModem::sendWifiIpAddress(int32_t serial, const std::string& ifName, const std::string& ipv4Addr, const std::string& ipv6Addr, int32_t ipv4PrefixLen,
            int32_t ipv6PrefixLen, const std::string& ipv4Gateway, const std::string& ipv6Gateway, int32_t dnsCount, const std::string& dnsServers, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, slotId %d, clientId %d", __func__, serial, mSlotId, clientId);
    return ok();
}

ScopedAStatus MtkRadioModem::registerCellQltyReport(int32_t serial, const std::string& registerQuality, const std::string& type, const std::string& thresholdValues,
            const std::string& triggerTime, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, slotId %d, clientId %d", __func__, serial, mSlotId, clientId);
    return ok();
}

ScopedAStatus MtkRadioModem::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus MtkRadioModem::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExModemResponse>& radioResponse, const std::shared_ptr<aidl::IMtkRadioExModemIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk modem setResponseFunctions, slotId:%d", mSlotId);

    if (mSlotId < getSimCount()) {
        rilAidlUtils::ril_service_stop_watch_dog(sMtkModemRilServiceDogTimer);
        rilAidlUtils::ril_service_start_watch_dog(sMtkModemRilServiceDogTimer, 30 * 1000);
    }

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk modem setResponseFunctions: got lock %d", mSlotId);

    radioModemServiceImpl[mSlotId].mtkModemResponse = radioResponse;
    radioModemServiceImpl[mSlotId].mtkModemIndication = radioIndication;
    plusCounterMtkRadioNumOne(mSlotId);

    rilAidlUtils::setMtkResponseNumberToOne(rilAidlUtils::MODEM_INDEX, mSlotId);
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk modem setResponseFunctions: release lock %d", mSlotId);

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

ScopedAStatus MtkRadioModem::setResponseFunctionsMtkIms(const std::shared_ptr<aidl::IMtkRadioExModemResponse>& radioResponse, const std::shared_ptr<aidl::IMtkRadioExModemIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk modem setResponseFunctionsMtkIms, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk modem setResponseFunctionsMtkIms: got lock %d", mSlotId);

    radioModemServiceImpl[mSlotId].mtkModemImsResponse = radioResponse;
    radioModemServiceImpl[mSlotId].mtkModemImsIndication = radioIndication;
    plusCounterMtkRadioNumOne(mSlotId + android::CLIENT_IMS * MAX_SIM_COUNT);

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk modem setResponseFunctionsMtkIms: release lock %d", mSlotId);

    return ok();
}

ScopedAStatus MtkRadioModem::setResponseFunctionsCap(
        const std::shared_ptr<aidl_cap::IMtkRadioExCapRadioResponse>& capRadioResponse) {
    mtkLogD(LOG_TAG, "mtk cap setResponseFunctions, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk cap setResponseFunctions: got lock %d", mSlotId);

    radioModemServiceImpl[mSlotId].mtkCapResponse = capRadioResponse;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk cap setResponseFunctions: release lock %d", mSlotId);

    return ok();
}

ScopedAStatus MtkRadioModem::setResponseFunctionsForAtci(
        const std::shared_ptr<aidl_atci::IAtciResponse>& atciResponseParam,
        const std::shared_ptr<aidl_atci::IAtciIndication>& atciIndicationParam) {
    mtkLogD(LOG_TAG, "mtk modem setResponseFunctionsForAtci, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk modem setResponseFunctionsForAtci: got lock %d", mSlotId);

    radioModemServiceImpl[mSlotId].mtkAtciResponse = atciResponseParam;
    radioModemServiceImpl[mSlotId].mtkAtciIndication = atciIndicationParam;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk modem setResponseFunctionsForAtci: release lock %d", mSlotId);

    return ok();
}

ScopedAStatus MtkRadioModem::setResponseFunctionsEm(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::em::IEmRadioResponse>& radioResponse,
 const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::em::IEmRadioIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk EM setResponseFunctions, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk EM setResponseFunctions: got lock %d", mSlotId);

    radioModemServiceImpl[mSlotId].mtkEmResponse = radioResponse;
    radioModemServiceImpl[mSlotId].mtkEmIndication = radioIndication;
    plusCounterMtkRadioNumOne(mSlotId);

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk EM setResponseFunctions: release lock %d", mSlotId);

    return ok();
}

ScopedAStatus MtkRadioModem::setResponseFunctionsGba(
            const std::shared_ptr<aidl::IMtkRadioExModemResponse>& radioResponse) {
    mtkLogD(LOG_TAG, "mtk Gba setResponseFunctionsGba, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk Gba setResponseFunctionsGba: got lock %d", mSlotId);

    radioModemServiceImpl[mSlotId].mtkModemGbaResponse = radioResponse;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk Gba setResponseFunctionsGba: release lock %d", mSlotId);

    return ok();
}

ScopedAStatus MtkRadioModem::setResponseFunctionsAssist(
        const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::assist::IAssistModemResponse>& radioResponse) {
    mtkLogD(LOG_TAG, "setResponseFunctionsAssist, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsAssist: got lock %d", mSlotId);
    radioModemServiceImpl[mSlotId].mtkModemAssistResponse = radioResponse;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsAssist: release lock %d", mSlotId);
    return ok();
}

ScopedAStatus MtkRadioModem::syncAppEventStatus(int32_t serial, int32_t event, int32_t status, int32_t clientId) {
    mtkLogD(LOG_TAG, "syncAppEventStatus: serial:%d slotId:%d clientId:%d", serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_ASSIST)) {
        return ok();
    }

    int requestId = RIL_REQUEST_SYNC_APP_EVENT_STATUS;
    android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT, requestId);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "syncAppEventStatus: pRI is NULL!!");
        return ok();
    }
    // legacy chips before gen98 don't support, directly return.
    char mipcSupport[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("ro.vendor.mtk_mipc_support", mipcSupport, "0");
    if (mipcSupport[0] == '0') {
        mtkLogD(LOG_TAG, "syncAppEventStatus: not supported!");
        sendErrorResponse(pRI, RIL_E_SUCCESS);
        return ok();
    }

    int32_t events[APP_EVENT_STATUS_ARRAY_LEN] = {};
    events[APP_EVENT_STATUS_KEY_INDEX] = event;
    events[APP_EVENT_STATUS_VALUE_INDEX] = status;
    CALL_ONREQUEST(requestId, events, sizeof(events), pRI, mSlotId + clientId * MAX_SIM_COUNT);
    return ok();
}

}

int mtkRadioExModem::routeCertificateResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "routeCertificateResponse: slotId:%d, serial %d", slotId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_CAP &&
            radioModemServiceImpl[realSlotId].mtkCapResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<uint8_t> data;
        int custId = 0;

        if (response == NULL) {
            mtkLogE(LOG_TAG, "routeCertificateResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            RIL_CertResponse* responseData = (RIL_CertResponse *) response;
            if (e == RIL_E_SUCCESS) {
                data.resize(responseData->rndLength);
                for (int i = 0 ; i < responseData->rndLength; i++) {
                    data[i] = responseData->rnd[i];
                }
                custId = responseData->custId;
            }
        }

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[realSlotId].mtkCapResponse->routeCertificateResponse(
                responseInfo, data, custId);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "routeCertificateResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::CAP_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "routeCertificateResponse"
                ": radioModemServiceImpl[%d]->mtkCapResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExModem::routeAuthMessageResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "routeAuthMessageResponse: slotId:%d, serial %d", slotId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_CAP
            && radioModemServiceImpl[realSlotId].mtkCapResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<uint8_t> data;
        int capMask = 0;

        if (response == NULL) {
            mtkLogE(LOG_TAG, "routeAuthMessageResponse: Invalid response");
            if (e == RIL_E_SUCCESS) {
                responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
            }
        } else {
            RIL_AuthResponse* responseData = (RIL_AuthResponse *) response;
            if (e == RIL_E_SUCCESS) {
                data.resize(responseData->devIdLength);
                for (int i = 0 ; i < responseData->devIdLength; i++) {
                    data[i] = responseData->devId[i];
                }
                capMask = responseData->capMask;
            }
        }

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[realSlotId].mtkCapResponse->routeAuthMessageResponse(
                responseInfo, data, capMask);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "routeAuthMessageResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::CAP_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "routeAuthMessageResponse"
                ": radioModemServiceImpl[%d]->mtkCapResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExModem::enableCapabilityResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "enableCapabilityResponse: slotId:%d, serial %d", slotId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_CAP &&
            radioModemServiceImpl[realSlotId].mtkCapResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[realSlotId].mtkCapResponse->enableCapabilityResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enableCapabilityResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::CAP_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "enableCapabilityResponse"
                ": radioModemServiceImpl[%d]->mtkCapResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExModem::abortCertificateResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "abortCertificateResponse: slotId:%d, serial %d", slotId, serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_CAP &&
            radioModemServiceImpl[realSlotId].mtkCapResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[realSlotId].mtkCapResponse->abortCertificateResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "abortCertificateResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::CAP_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "abortCertificateResponse"
                ": radioModemServiceImpl[%d]->mtkCapResponse == NULL", slotId);
    }

    return 0;
}

int mtkRadioExModem::restartRILDResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "restartRILDResponse: slotId:%d, serial %d", slotId, serial);
    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[slotId].mtkModemResponse->restartRILDResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "restartRILDResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "restartRILDResponse: radioModemServiceImpl[%d].mtkModemResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExModem::modifyModemTypeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "modifyModemTypeResponse: slotId:%d, serial %d", slotId, serial);
    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int n = rilAidlUtils::responseIntOrEmpty(responseInfo, serial, responseType, e,
                response, responseLen);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemResponse->modifyModemTypeResponse(
                responseInfo, n);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "modifyModemTypeResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "modifyModemTypeResponse: radioModemServiceImpl[%d].mtkModemResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExModem::getEngineeringModeInfoResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG,"[Echolocate] getEngineeringModeInfoResponse serial %d slotId %d", serial, slotId);

    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<std::string> matricsList;
        char *saveptr = NULL;

        if (response == NULL) {
            mtkLogE(LOG_TAG, "getEngineeringModeInfoResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }else {
            RIL_EcholocateResponse* responseData = (RIL_EcholocateResponse *) response;
            if (e != RIL_E_SUCCESS || responseData->error != 0 || responseData->resultLen == 0) {
                mtkLogE(LOG_TAG, "getEngineeringModeInfoResponse: error %d %d index %d resultlen = %d",
                        e, responseData->error, responseData->index, responseData->resultLen);

                if (e == RIL_E_SUCCESS) {
                    responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
                }

                ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemResponse->
                            getEngineeringModeInfoResponse(responseInfo, matricsList);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "getEngineeringModeInfoResponse: slotId = %d, binder fail!", slotId);
                    retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
                }
            } else {
                mtkLogD(LOG_TAG,"getEngineeringModeInfoResponse: [%d] %s",
                        responseData->resultLen, responseData->result);
                char* buf = (char*) calloc(1, sizeof(char) * (strlen(responseData->result)+1));
                if (buf == NULL) {
                    mtkLogE(LOG_TAG, "getEngineeringModeInfoResponse: Memory allocation failed for request");
                    return 0;
                }
                memcpy(buf, responseData->result, strlen(responseData->result));
                buf[strlen(responseData->result)] = '\0';
                char* token = strstr(buf, ",");
                int length =0;
                int i = 0;
                while (token != NULL) {
                    length++;
                    token = strstr(token + 1, ",");
                }
                length++;
                matricsList.resize(length);
                token = strtok_r(responseData->result, ",", &saveptr);
                while (token != NULL && i >= 0 && i < length) {
                    mtkLogD(LOG_TAG,"getEngineeringModeInfoResponse: %d (%d) : %s" , length, i, token);
                    matricsList[i++] = convertCharPtrToStdString(token);
                    token = strtok_r(NULL, ",", &saveptr);
                }

                ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemResponse->
                            getEngineeringModeInfoResponse(responseInfo, matricsList);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "getEngineeringModeInfoResponse: slotId = %d, binder fail!", slotId);
                    retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
                }
                free(buf);
            }
        }
    }else {
        mtkLogE(LOG_TAG, "getEngineeringModeInfoResponse: mRadioResponseMtk == NULL");
    }
    return 0;
}

int mtkRadioExModem::sendAtciResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendAtciResponse: slotid:%d, serial %d", slotId, serial);

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    std::vector<uint8_t> data;

    if (response == NULL) {
        mtkLogE(LOG_TAG, "sendAtciResponse: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        int num = responseLen / sizeof(uint8_t);
        // data.resize(num);
        uint8_t *resp = (uint8_t *) response;
        for (int i = 0; i < num; i++) {
            data.push_back(resp[i]);
        }
    }

    if (radioModemServiceImpl[slotId].mtkAtciResponse != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkAtciResponse->
                sendAtciResponse(responseInfo, data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendAtciResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sendAtciResponse: radioModemServiceImpl[%d]->mtkModemResponse == NULL",
                slotId);
    }

    return 0;
}

int mtkRadioExModem::sendEmbmsAtCommandResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responseLen) {
    return 0;
}

int mtkRadioExModem::sendRequestRawResponse(unsigned int slotId, android::ClientId clientId,
                           int responseType, int serial, RIL_Errno e,
                           const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendRequestRawResponse: slotid:%d, serial %d", slotId, serial);

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    std::vector<uint8_t> data;

    if (response == NULL) {
        mtkLogE(LOG_TAG, "sendRequestRawResponse: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        int num = responseLen / sizeof(uint8_t);
        // data.resize(num);
        uint8_t *resp = (uint8_t *) response;
        for (int i = 0; i < num; i++) {
            data.push_back(resp[i]);
        }
    }

    if (clientId == android::CLIENT_EM) {
        int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioModemServiceImpl[realSlotId].mtkEmResponse != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[realSlotId].mtkEmResponse->
                    sendRequestRawResponse(responseInfo, data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "sendRequestRawResponse: realSlotId = %d, binder fail!", realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::EM_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "sendRequestRawResponse: radioModemServiceImpl[%d]->mtkEmResponse == NULL",
                    realSlotId);
        }
    } else {
        if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemResponse->
                    sendRequestRawResponse(responseInfo, data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "sendRequestRawResponse: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "sendRequestRawResponse: radioModemServiceImpl[%d]->mtkModemResponse == NULL",
                    slotId);
        }
    }
    return 0;
}

int mtkRadioExModem::sendRequestStringsResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendRequestStringsResponse: serial %d, slotId:%d", serial, slotId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    std::vector<std::string> data;

    if ((response == NULL && responseLen != 0) || responseLen % sizeof(char *) != 0) {
        mtkLogE(LOG_TAG, "sendRequestStringsResponse Invalid response: NULL");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
        }
    }

    if (clientId == android::CLIENT_EM) {
        int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioModemServiceImpl[realSlotId].mtkEmResponse != NULL) {
            ndk::ScopedAStatus ret
                    = radioModemServiceImpl[realSlotId].mtkEmResponse->sendRequestStringsResponse(
                    responseInfo, data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "sendRequestStringsResponse: realSlotId = %d, binder fail!",
                        realSlotId);
                retChecker->mtkAidlReturnStatusChecker(realSlotId, ret, android::EM_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "sendRequestStringsResponse: radioModemServiceImpl[%d]->mtkEmResponse == "
                    "NULL", realSlotId);
        }
    } else {
        if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
            ndk::ScopedAStatus ret
                    = radioModemServiceImpl[slotId].mtkModemResponse->sendRequestStringsResponse(
                    responseInfo, data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "sendRequestStringsResponse: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "sendRequestStringsResponse: radioModemServiceImpl[%d]->mtkModemResponse == "
                    "NULL", slotId);
        }
    }
    return 0;
}

int mtkRadioExModem::sendSarIndicatorResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendSarIndicatorResponse: serial:%d, slot:%d", serial, slotId);

    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[slotId].mtkModemResponse->sendSarIndicatorResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "sendSarIndicatorResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sendSarIndicatorResponse: mtkRadioExService[%d]->mtkModemResponse"
                "== NULL", slotId);
    }

    return 0;
}

int mtkRadioExModem::setModemPowerResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setModemPowerResponse: serial %d", serial);

    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemResponse->setModemPowerResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setModemPowerResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setModemPowerResponse: radioModemServiceImpl[%d].mtkModemResponse "
                "== NULL", slotId);
    }
    return 0;
}


int mtkRadioExModem::setTrmResponse(unsigned int slotId, android::ClientId clientId __unused, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setTrmResponse: serial:%d, slotId:%d", serial, slotId);

    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
                radioModemServiceImpl[slotId].mtkModemResponse->setTrmResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setTrmResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setTrmResponse: radioModemServiceImpl[%d].mtkModemResponse == NULL",
                slotId);
    }
    return 0;
}


int mtkRadioExModem::setTxPowerResponse(unsigned int slotId, android::ClientId clientId __unused,
                             int responseType, int serial, RIL_Errno e,
                             const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setTxPowerResponse: serial:%d, slotId:%d", serial, slotId);

    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[slotId].mtkModemResponse->setTxPowerResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setTrmResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setTxPowerResponse: radioModemServiceImpl[%d].mtkModemResponse "
                "== NULL", slotId);
    }

    return 0;
}

int mtkRadioExModem::setTxPowerStatusResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setTxPowerStatusResponse: serial:%d, slotId:%d", serial, slotId);

    if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret =
                radioModemServiceImpl[slotId].mtkModemResponse->setTxPowerStatusResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setTxPowerStatusResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setTxPowerStatusResponse: radioModemServiceImpl[%d].mtkModemResponse == NULL", slotId);
    }
    return 0;
}

int mtkRadioExModem::setVendorSettingResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setVendorSettingResponse: serial:%d, slotId:%d", serial, slotId);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (clientId == android::CLIENT_RILJ && radioModemServiceImpl[realSlotId].mtkModemResponse!= NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[realSlotId].mtkModemResponse->setVendorSettingResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setVendorSettingResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else if (clientId == android::CLIENT_IMS && radioModemServiceImpl[realSlotId].mtkModemImsResponse!= NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret
                = radioModemServiceImpl[realSlotId].mtkModemImsResponse->setVendorSettingResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setVendorSettingResponse for ims: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setVendorSettingResponse: Client %d mtkRadioExService[%d] "
                "== NULL", clientId, slotId);
    }

    return 0;
}

int mtkRadioExModem::triggerModeSwitchByEccResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "triggerModeSwitchByEccResponse: serial:%d, slotId:%d", serial, slotId);

    if (radioModemServiceImpl[slotId].mtkModemResponse!= NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemResponse->
                triggerModeSwitchByEccResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "triggerModeSwitchByEccResponse: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "triggerModeSwitchByEccResponse: radioModemServiceImpl[%d].mtkModemResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExModem::sendWifiEnabledResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendWifiEnabledResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    return 0;
}

int mtkRadioExModem::sendWifiAssociatedResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendWifiAssociatedResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    return 0;
}

int mtkRadioExModem::sendWifiIpAddressResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "sendWifiIpAddressResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    return 0;
}

int mtkRadioExModem::registerCellQltyReportResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "registerCellQltyReportResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    return 0;
}

int mtkRadioExModem::runGbaAuthenticationResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "runGbaAuthenticationResponse: serial %d, slotId %d, clientId %d",
            serial, slotId, clientId);

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    std::vector<std::string> data;

    if ((response == NULL && responseLen != 0) || responseLen % sizeof(char *) != 0) {
        mtkLogE(LOG_TAG, "runGbaAuthenticationResponse Invalid response: NULL");
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
        }
    }

    if (clientId == android::CLIENT_IMS) {
        int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioModemServiceImpl[realSlotId].mtkModemImsResponse != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[realSlotId].mtkModemImsResponse
                    ->runGbaAuthenticationResponse(responseInfo, data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::IMS_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: radioModemServiceImpl[%d]->mtkModemImsResponse == NULL",
                    __func__, realSlotId);
        }
    } else if (clientId == android::CLIENT_GBA) {
        int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioModemServiceImpl[realSlotId].mtkModemGbaResponse != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[realSlotId].mtkModemGbaResponse
                    ->runGbaAuthenticationResponse(responseInfo, data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, realSlotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::GBA_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: radioModemServiceImpl[%d]->mtkModemGbaResponse == NULL",
                    __func__, realSlotId);
        }
    } else {
        if (radioModemServiceImpl[slotId].mtkModemResponse != NULL) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemResponse
                    ->runGbaAuthenticationResponse(responseInfo, data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __func__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "%s: radioModemServiceImpl[%d]->mtkModemResponse == NULL",
                    __func__, slotId);
        }
    }

    return 0;
}

int mtkRadioExModem::onAtciInd(unsigned int slotId,
                         int indicationType, int token, RIL_Errno e, const void *response,
                         size_t responseLen) {
    if (response == NULL || responseLen == 0) {
        mtkLogE(LOG_TAG, "onAtciInd: invalid response, slotId:%d", slotId);
        return 0;
    }

    std::vector<uint8_t> data;
    int num = responseLen / sizeof(uint8_t);
    // data.resize(num);
    uint8_t *resp = (uint8_t *) response;
    for (int i = 0; i < num; i++) {
        data.push_back(resp[i]);
    }

    mtkLogD(LOG_TAG, "onAtciInd, slotId:%d", slotId);
    if (radioModemServiceImpl[slotId].mtkAtciIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkAtciIndication->atciInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onAtciInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onAtciInd: radioModemServiceImpl[%d].mtkModemIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "onAtciInd, tboxSlotId:%d", tboxSlotId);
    if (radioModemServiceImpl[tboxSlotId].mtkAtciIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].mtkAtciIndication->atciInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onAtciInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-onAtciInd: radioModemServiceImpl[%d].mtkModemIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExModem::oemHookRawInd(unsigned int slotId,
                         int indicationType, int token, RIL_Errno e, const void *response,
                         size_t responseLen) {
    if (response == NULL || responseLen == 0) {
        mtkLogE(LOG_TAG, "oemHookRawInd: invalid response, slotId:%d", slotId);
        return 0;
    }

    std::vector<uint8_t> data;
    int num = responseLen / sizeof(uint8_t);
    // data.resize(num);
    uint8_t *resp = (uint8_t *) response;
    for (int i = 0; i < num; i++) {
        data.push_back(resp[i]);
    }

    mtkLogD(LOG_TAG, "oemHookRawInd, slotId:%d", slotId);
    if (radioModemServiceImpl[slotId].mtkModemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemIndication->oemHookRaw(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "oemHookRawInd: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "oemHookRawInd: radioModemServiceImpl[%d].mtkModemIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "oemHookRawInd, tboxSlotId:%d", tboxSlotId);
    if (radioModemServiceImpl[slotId].mtkModemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].mtkModemIndication->oemHookRaw(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "oemHookRawInd: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-oemHookRawInd: radioModemServiceImpl[%d].mtkModemIndication == NULL", tboxSlotId);
    }

    // send to EM
    //slotId = slotId % MAX_SIM_COUNT;
    if (radioModemServiceImpl[slotId].mtkEmIndication != NULL) {
        std::string str(data.begin(), data.end());
        if ((str.find("EMCFRPT") > 0) || (str.find("EDMFAPP:8") > 0) || (str.find("ERFIDX") > 0)) {
            ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkEmIndication->oemHookRaw(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "oemHookRawInd: slotId = %d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::EM_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "oemHookRawInd: radioModemServiceImpl[%d].mtkEmIndication == NULL", slotId);
    }
    return 0;
}

int mtkRadioExModem::onTxPowerIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen) {
    if (response == NULL) {
        mtkLogE(LOG_TAG, "%s, slotId:%d: invalid response", __func__, slotId);
        return 0;
    }
    std::vector<int32_t> indTxPower;
    int *pInt = (int *) response;
    int numInts = responseLen / sizeof(int);
    mtkLogE(LOG_TAG, "onTxPowerIndication responselen: %lu, sizeof(int): %lu, numInts: %d, slotId:%d",
            responseLen, (unsigned long)sizeof(int), numInts, slotId);
    indTxPower.resize(numInts);
    for (int i = 0; i < numInts; i++) {
        indTxPower[i] = (int32_t) pInt[i];
    }

    if (radioModemServiceImpl[slotId].mtkModemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemIndication
                ->onTxPowerIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indTxPower);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onTxPowerIndication: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onTxPowerIndication radioModemServiceImpl[%d].mtkModemIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioModemServiceImpl[tboxSlotId].mtkModemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].mtkModemIndication
                ->onTxPowerIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indTxPower);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onTxPowerIndication: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-onTxPowerIndication radioModemServiceImpl[%d].mtkModemIndication == NULL", tboxSlotId);
    }

    // send to EM
    if (radioModemServiceImpl[slotId].mtkEmIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkEmIndication
                ->onTxPowerIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indTxPower);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onTxPowerIndication: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::EM_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onTxPowerIndication radioModemServiceImpl[%d].mtkEmIndication == NULL", slotId);
    }
    return 0;
}

int mtkRadioExModem::onTxPowerStatusIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen) {
    if (response == NULL) {
        mtkLogE(LOG_TAG, "%s: invalid response slotId %d", __func__, slotId);
        return 0;
    }

    std::vector<int32_t> indTxPower;
    int *pInt = (int *) response;
    int numInts = responseLen / sizeof(int);
    mtkLogE(LOG_TAG, "onTxPowerStatusIndication responselen: %lu, sizeof(int): %lu, numInts: %d, slotId:%d",
            responseLen, (unsigned long)sizeof(int), numInts, slotId);
    indTxPower.resize(numInts);
    for (int i = 0; i < numInts; i++) {
        indTxPower[i] = (int32_t) pInt[i];
    }

    if (radioModemServiceImpl[slotId].mtkModemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemIndication
                ->onTxPowerStatusIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indTxPower);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onTxPowerStatusIndication: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onTxPowerStatusIndication radioModemServiceImpl[%d].mtkModemIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioModemServiceImpl[tboxSlotId].mtkModemIndication != NULL) {
        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].mtkModemIndication
                ->onTxPowerStatusIndication(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), indTxPower);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onTxPowerStatusIndication: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "Tbox-onTxPowerStatusIndication radioModemServiceImpl[%d].mtkModemIndication == NULL", tboxSlotId);
    }
    return 0;
}

int mtkRadioExModem::eMBMSSessionStatusIndication(unsigned int slotId, int indicationType, int token, RIL_Errno e, const void *response, size_t responselen) {
    return 0;
}


int mtkRadioExModem::eMBMSAtInfoIndication(unsigned int slotId, int indicationType, int token, RIL_Errno e, const void *response, size_t responselen) {
    return 0;
}

int mtkRadioExModem::onDsbpStateChanged(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    if (radioModemServiceImpl[slotId].mtkModemIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG,"onDsbpStateChanged: invalid indication");
            return 0;
        }

        int *state = (int *)response;
        mtk_aidl::modem::DsbpState dsbpState = (mtk_aidl::modem::DsbpState) state[0];
        mtkLogI(LOG_TAG, "onDsbpStateChanged slot: %d, state: %d", slotId, state[0]);

        ndk::ScopedAStatus ret = radioModemServiceImpl[slotId].mtkModemIndication
                ->dsbpStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dsbpState);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onDsbpStateChanged: slotId = %d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "onDsbpStateChanged: radioModemServiceImpl[%d]->mtkModemIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioModemServiceImpl[tboxSlotId].mtkModemIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG,"tbox-onDsbpStateChanged: invalid indication");
            return 0;
        }

        int *state = (int *)response;
        mtk_aidl::modem::DsbpState dsbpState = (mtk_aidl::modem::DsbpState) state[0];
        mtkLogI(LOG_TAG, "onDsbpStateChanged slot: %d, state: %d", slotId, state[0]);

        ndk::ScopedAStatus ret = radioModemServiceImpl[tboxSlotId].mtkModemIndication
                ->dsbpStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dsbpState);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onDsbpStateChanged: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "tbox-onDsbpStateChanged: radioModemServiceImpl[%d]->mtkModemIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExModem::onCellularQualityChangedInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen) {
    return 0;
}

int mtkRadioExModem::clearMtkModemResponseAndIndications(unsigned int slotId) {
    radioModemServiceImpl[slotId].mtkModemResponse = NULL;
    radioModemServiceImpl[slotId].mtkModemIndication = NULL;
    radioModemServiceImpl[slotId].mtkAtciResponse = NULL;
    radioModemServiceImpl[slotId].mtkAtciIndication = NULL;
    return 0;
}

int mtkRadioExModem::clearMtkCapResponse(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioModemServiceImpl[realSlotId].mtkCapResponse = NULL;
    return 0;
}

int mtkRadioExModem::clearMtkEmResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioModemServiceImpl[realSlotId].mtkEmResponse = NULL;
    radioModemServiceImpl[realSlotId].mtkEmIndication = NULL;
    return 0;
}

int mtkRadioExModem::clearMtkGbaResponse(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioModemServiceImpl[realSlotId].mtkModemGbaResponse = NULL;
    return 0;
}

int mtkRadioExModem::clearAssistResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioModemServiceImpl[realSlotId].mtkModemAssistResponse = NULL;
    return 0;
}

int mtkRadioExModem::syncAppEventStatusResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "syncAppEventStatusResponse, serial:%d, slotId:%d", serial, slotId);
    if (clientId == android::CLIENT_ASSIST) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioModemServiceImpl[realSlotId].mtkModemAssistResponse == NULL) {
            mtkLogE(LOG_TAG, "syncAppEventStatusResponse assit:"
                    "radioModemServiceImpl[%d].mtkModemAssistResponse == NULL", slotId);
            return 0;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioModemServiceImpl[realSlotId].mtkModemAssistResponse
                ->syncAppEventStatusResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "syncAppEventStatusResponse assit: slotId:%d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::ASSIST_MTK);
        }
    }

    return 0;
}

void asop_modem_ril_service_watch_dog(sigval_t /* sig */) {
    unsigned int simCount = getSimCount();
    if (simCount > MAX_SIM_COUNT) {
        mtkLogE(LOG_TAG, "aosp ril modem service watch dog time out, err simCount(%d)", simCount);
        return;
    }

    for (unsigned int i = 0; i < simCount; i++) {
        if (radioModemServiceImpl[i].modemResponse == NULL ||
                    radioModemServiceImpl[i].modemIndication == NULL) {
            mtkLogE(LOG_TAG, "aosp ril modem service watch dog time out err(%d)", i);
        }
    }

    char initState[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("vendor.ril.muxreport.run", initState, "1");
    initState[MTK_PROPERTY_VALUE_MAX - 1] = '\0';
    if (strncmp(initState, "2", 1) == 0) {
        mtkLogE(LOG_TAG, "ril service watch dog time out err, rild init not done(%s)", initState);
        // trigger RILD restart
        mtk_property_set("vendor.ril.muxreport.run", "0");
        mtk_property_set("vendor.ril.mux.report.case", "2");
        mtk_property_set("vendor.ril.muxreport", "1");
        mtk_property_set("vendor.ril.mtk.restart", "1");
    }

    mtkLogD(LOG_TAG, "aosp ril modem service watch dog time out!");
}

void mtk_modem_ril_service_watch_dog(sigval_t /* sig */) {
    unsigned int simCount = getSimCount();
    if (simCount > MAX_SIM_COUNT) {
        mtkLogE(LOG_TAG, "mtk ril modem service watch dog time out, err simCount(%d)", simCount);
        return;
    }

    for (unsigned int i = 0; i < simCount; i++) {
        if (radioModemServiceImpl[i].mtkModemResponse == NULL ||
                radioModemServiceImpl[i].mtkModemIndication == NULL) {
            mtkLogE(LOG_TAG, "mtk modem ril service watch dog time out err(%d)", i);
        }
    }

    mtkLogD(LOG_TAG, "mtk modem ril service watch dog time out!");
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addModemLock(std::shared_ptr<ndk::ICInterface> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_MODEM_PORT_BASE 53000
#define MTK_RADIO_MODEM_PORT_BASE 53100

static pthread_t gThreadIds[MAX_SIM_COUNT*4];

void* addRcpVsockRadioModemAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_MODEM_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_MODEM_PORT_BASE + sloid, sloid);

    std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidl = ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(sloid);
    addModemLock(std::static_pointer_cast<ndk::ICInterface>(radioModemAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioModemAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void* addRcpVsockMtkRadioModemAidlService(void* arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, MTK_RADIO_MODEM_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", MTK_RADIO_MODEM_PORT_BASE + sloid, sloid);
    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkRadioModemAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(sloid);
    addModemLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioModemAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);

    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadioModemAidl->asBinder().get()));
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioModemVsock(unsigned int &sloid, unsigned int &imsSlotid, unsigned int &seSlotid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
            addRcpVsockRadioModemAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioModem");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT], NULL,
            addRcpVsockRadioModemAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(imsSlotid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioModem");
    }

    if (pthread_create(&gThreadIds[sloid+ MAX_SIM_COUNT*2], NULL,
            addRcpVsockRadioModemAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(seSlotid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioModem");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT*3], NULL,
            addRcpVsockMtkRadioModemAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockMtkRadioModem");
    }
}
#endif



void publicRadioModemAidlService(unsigned int sloid) {
    unsigned int imsSlotid = sloid + android::CLIENT_IMS *MAX_SIM_COUNT;
    unsigned int seSlotid = sloid + android::CLIENT_SE * MAX_SIM_COUNT;

    /*if (sloid == 0) {
        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(0);
        gPublishedHals.push_back(radioModemAidl);
        const std::string instance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/slot1";
        binder_status_t status =
                AServiceManager_addService(radioModemAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "new aosp modem addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(imsSlotid);
        gPublishedHals.push_back(radioModemAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/imsSlot1";
        binder_status_t imsStatus =
                AServiceManager_addService(radioModemAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp modem addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        gPublishedHals.push_back(radioModemAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/se1";
        binder_status_t seStatus =
                AServiceManager_addService(radioModemAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp modem addservice for SE, slotId:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(0);
        gPublishedHals.push_back(mtkRadioModemAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/slot1";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioModemAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "new mtk modem addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 1) {
        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(1);
        gPublishedHals.push_back(radioModemAidl);
        const std::string instance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/slot2";
        binder_status_t status =
                AServiceManager_addService(radioModemAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(imsSlotid);
        gPublishedHals.push_back(radioModemAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/imsSlot2";
        binder_status_t imsStatus =
                AServiceManager_addService(radioModemAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        gPublishedHals.push_back(radioModemAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/se2";
        binder_status_t seStatus =
                AServiceManager_addService(radioModemAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice for SE, slotId:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(1);
        gPublishedHals.push_back(mtkRadioModemAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/slot2";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioModemAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk modem addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 2) {
        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(2);
        gPublishedHals.push_back(radioModemAidl);
        const std::string instance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/slot3";
        binder_status_t status =
                AServiceManager_addService(radioModemAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(imsSlotid);
        gPublishedHals.push_back(radioModemAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/imsSlot3";
        binder_status_t imsStatus =
                AServiceManager_addService(radioModemAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        gPublishedHals.push_back(radioModemAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/se3";
        binder_status_t seStatus =
                AServiceManager_addService(radioModemAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice for SE, slotId:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(2);
        gPublishedHals.push_back(mtkRadioModemAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/slot3";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioModemAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk modem addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 3) {
        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(3);
        gPublishedHals.push_back(radioModemAidl);
        const std::string instance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/slot4";
        binder_status_t status =
                AServiceManager_addService(radioModemAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(imsSlotid);
        gPublishedHals.push_back(radioModemAidlforIms);
        const std::string imsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/imsSlot4";
        binder_status_t imsStatus =
                AServiceManager_addService(radioModemAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> radioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        gPublishedHals.push_back(radioModemAidlforSe);
        const std::string seInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/se4";
        binder_status_t seStatus =
                AServiceManager_addService(radioModemAidlforSe->asBinder().get(), seInstance.c_str());
        mtkLogD(LOG_TAG, "aosp modem addservice for SE, slotId:%d, status:%d", seSlotid, seStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(3);
        gPublishedHals.push_back(mtkRadioModemAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/slot4";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioModemAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk modem addservice, slotId:%d, status:%d", sloid, mtkStatus);
    }*/

    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;
    unsigned int tboxImsSlotid = sloid + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;

    if (sloid == 0) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioModemVsock(sloid, imsSlotid, seSlotid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioModemAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxImsSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxImsSlot1";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSe1";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for Se, slotId:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkTboxRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioModemAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/tboxSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioModemAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "new mtk tbox modem addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 1) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioModemVsock(sloid, imsSlotid, seSlotid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioModemAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxImsSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxImsSlot2";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSe2";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for Se, slotId:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkTboxRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioModemAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/tboxSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioModemAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "new mtk tbox modem addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 2) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioModemVsock(sloid, imsSlotid, seSlotid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioModemAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxImsSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxImsSlot3";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSe3";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for Se, slotId:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkTboxRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioModemAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioModemAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "new mtk tbox modem addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 3) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioModemVsock(sloid, imsSlotid, seSlotid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidl =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSlot4";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioModemAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforIms =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(tboxImsSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforIms));
        const std::string tboxImsInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxImsSlot4";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<android::hardware::radio::modem::RadioModem> tboxRadioModemAidlforSe =
                ndk::SharedRefBase::make<android::hardware::radio::modem::RadioModem>(seSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioModemAidlforSe));
        const std::string tboxSeInstance = std::string() + android::hardware::radio::modem::RadioModem::descriptor + "/tboxSe4";
        binder_status_t tboxSeStatus =
                AServiceManager_addService(tboxRadioModemAidlforIms->asBinder().get(), tboxSeInstance.c_str());
        mtkLogD(LOG_TAG, "new aosp tbox modem addservice for Se, slotId:%d, status:%d", seSlotid, tboxSeStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem> mtkTboxRadioModemAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem>(tboxSlotid);
        addModemLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioModemAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::modem::MtkRadioModem::descriptor + "/tboxSlot4";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioModemAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "new mtk tbox modem addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    }


    if (sloid == 0) {
        rilAidlUtils::ril_service_init_watch_dog(&sAospModemRilServiceDogTimer, asop_modem_ril_service_watch_dog);
        rilAidlUtils::ril_service_init_watch_dog(&sMtkModemRilServiceDogTimer, mtk_modem_ril_service_watch_dog);
    }
}
