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

#define LOG_TAG "RadioConfig_service"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>

#include <utils/SystemClock.h>
#include <inttypes.h>
#include <libmtkrilutils.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <mtkconfigutils.h>
#include <radioConfig_service.h>
#include <ril_service.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <rilAidlUtils.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

using android::CommandInfo;
using android::RequestInfo;
using android::requestToString;
using android::status_t;
using android::sp;
using android::OK;
using rilAidlUtils::retChecker;


#if defined(ANDROID_MULTI_SIM)
#define CONFIG_CALL_ONREQUEST(a, b, c, d, e) \
        s_radioConfigFunctions->onRequest((a), (b), (c), (d), ((RIL_SOCKET_ID)(e)))
#else
#define CONFIG_CALL_ONREQUEST(a, b, c, d, e) \
        s_radioConfigFunctions->onRequest((a), (b), (c), (d))
#endif


extern "C" int toRealSlot(unsigned int slotId);

RIL_RadioFunctions *s_radioConfigFunctions = NULL;
static CommandInfo *s_commands;

#include <binder/ProcessState.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>


struct RadioConfigAidlImpl {
    std::shared_ptr<::aidl::android::hardware::radio::config::IRadioConfigResponse> configResponse;
    std::shared_ptr<::aidl::android::hardware::radio::config::IRadioConfigIndication> configIndication;
};

RadioConfigAidlImpl radioConfigServiceImpl[MAX_SIM_COUNT * (android::CLIENT_TBOX + 1)];
// void publicRadioConfigAidlService(void);


// counter used for synchronization. It is incremented every time response callbacks are updated.
volatile int32_t mCounterRadioConfig[MAX_SIM_COUNT * (android::CLIENT_TBOX + 1)];

extern bool isMtkFwkAddonNotExisted(unsigned int slotId);

bool dispatchConfigVoid(int serial, int slotId, int request) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }
    CONFIG_CALL_ONREQUEST(request, NULL, 0, pRI, slotId);
    return true;
}

static void fillEnabledLogicalSlots(std::vector<int32_t> &enabledLogicalSlots,
        const void *response, size_t responselen) {
    int num = 0;
    if ((response != NULL) &&
            (responselen % sizeof(int32_t) == 0) &&
            ((num = responselen / sizeof(int32_t)) == 2)) {
        int32_t *pData = (int *)response;
        if ((pData[0] == 0 && pData[1] == 1)  || (pData[0] == 1 && pData[1] == 0)) {
            enabledLogicalSlots.resize(num);
            enabledLogicalSlots[0] = pData[0];
            enabledLogicalSlots[1] = pData[1];
        }
    }
}

void radioConfig::registerService(RIL_RadioFunctions *callbacks, CommandInfo *commands) {
    using namespace android::hardware;

    s_radioConfigFunctions = callbacks;
    s_commands = commands;
#ifdef MTK_USE_AIDL
    // ABinderProcess_setThreadPoolMaxThreadCount(0);
    // ABinderProcess_startThreadPool();
    // publicRadioConfigAidlService();
#endif
}

//// support aidl
namespace aidl_radio = ::aidl::android::hardware::radio;

namespace android::hardware::radio::config {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::android::hardware::radio::config;
constexpr auto ok = &ScopedAStatus::ok;

RadioConfig::RadioConfig(int slotid){
    mSlotId = slotid;
}

ScopedAStatus RadioConfig::getHalDeviceCapabilities(int32_t serial) {
    if (radioConfigServiceImpl[mSlotId].configResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, RESPONSE_SOLICITED, RIL_E_SUCCESS);
        bool modemReducedFeatureSet1 = false;

        ndk::ScopedAStatus ret = radioConfigServiceImpl[mSlotId].configResponse->
                getHalDeviceCapabilitiesResponse(responseInfo, modemReducedFeatureSet1);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getHalDeviceCapabilities slotId:%d, :binder fail!", mSlotId);
            aidlRadioConfig::checkAidlReturnStatus(mSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getHalDeviceCapabilitiesResponse: radioConfigServiceImpl[%d].configResponse is NULL", mSlotId);
    }
    return ok();
}

ScopedAStatus RadioConfig::getNumOfLiveModems(int32_t serial) {
    if (radioConfigServiceImpl[mSlotId].configResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, RESPONSE_SOLICITED, RIL_E_SUCCESS);

        uint8_t numOfLiveModems = (uint8_t)getSimCount();
        ndk::ScopedAStatus ret = radioConfigServiceImpl[mSlotId].configResponse->
                getNumOfLiveModemsResponse(responseInfo, numOfLiveModems);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getNumOfLiveModems slotId:%d, :binder fail!", mSlotId);
            aidlRadioConfig::checkAidlReturnStatus(mSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getModemsConfig: radioConfigServiceImpl[%d].configResponse is NULL", mSlotId);
    }
    return ok();
}

ScopedAStatus RadioConfig::getPhoneCapability(int32_t serial) {
    mtkLogD(LOG_TAG, "RadioConfig::getPhoneCapability serial=%d", serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_GET_PHONE_CAPABILITY);
    if (pRI == NULL) {
        return ok();
    }
    CONFIG_CALL_ONREQUEST(pRI->pCI->requestNumber, NULL, 0, pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioConfig::getSimSlotsStatus(int32_t serial) {
    mtkLogD(LOG_TAG, "RadioConfig::getSimSlotsStatus");
    dispatchConfigVoid(serial, mSlotId, RIL_REQUEST_GET_SLOT_STATUS);
    return ok();
}

ScopedAStatus RadioConfig::setNumOfLiveModems(int32_t serial, int8_t numOfLiveModems) {
    mtkLogD(LOG_TAG, "radioConfig::setNumOfLiveModems serial=%d", serial);
    aidl_radio::RadioResponseInfo responseInfo = {};
    RIL_Errno rilError = RIL_E_SUCCESS;
    if (numOfLiveModems <= 0 || numOfLiveModems > 3) {
        rilError = RIL_E_INVALID_ARGUMENTS;
    }

    if (rilError != RIL_E_SUCCESS) {
        // not support
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, RESPONSE_SOLICITED, rilError);

        ndk::ScopedAStatus ret = radioConfigServiceImpl[mSlotId].configResponse->
                setNumOfLiveModemsResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setNumOfLiveModems, slotId:%d, binder fail!", mSlotId);
            aidlRadioConfig::checkAidlReturnStatus(mSlotId, ret);
        }
    } else {
        RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
                RIL_REQUEST_SET_MODEM_CONFIG);
        if (pRI == NULL) {
            return ok();
        }
        int mode[] = {(int)numOfLiveModems};
        CONFIG_CALL_ONREQUEST(pRI->pCI->requestNumber, mode, sizeof(int), pRI, mSlotId);
    }
    return ok();
}

ScopedAStatus RadioConfig::setPreferredDataModem(int32_t serial, int8_t modemId) {
    mtkLogD(LOG_TAG, "radioConfig::setPreferredDataModem serial=%d, modemId=%d", serial, modemId);
    mtk_property_set("vendor.ril.data.preferred_data_mode", "1");
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_PREFERRED_DATA_MODEM);
    if (pRI == NULL) {
        return ok();
    }
    int mId[] = {(int)modemId};
    CONFIG_CALL_ONREQUEST(pRI->pCI->requestNumber, mId, sizeof(int), pRI, mSlotId);
    return ok();
}

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    // if (cookie == nullptr) {
    //     mtkLogW(LOG_TAG, "aidl radioConfig line to deatch recipientCallback, cookie is null");
    //     return;
    // }
    int slotId = static_cast<int>(reinterpret_cast<std::uintptr_t>(cookie));
    mtkLogI(LOG_TAG, "aidl radioConfig line to deatch recipientCallback, slot=%d", slotId);

    ndk::ScopedAStatus ret = ndk::ScopedAStatus::fromExceptionCodeWithMessage(EX_ILLEGAL_STATE ,(rilAidlUtils::binderDiedError).c_str());
    aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
}

ScopedAStatus RadioConfig::setResponseFunctions(
        const std::shared_ptr<aidl::IRadioConfigResponse>& radioConfigResponse,
        const std::shared_ptr<aidl::IRadioConfigIndication>& radioConfigIndication) {
    mtkLogD(LOG_TAG, "aidl radioConfig setResponseFunctions");
    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    radioConfigServiceImpl[mSlotId].configResponse = radioConfigResponse;
    auto binder = radioConfigResponse->asBinder();  // Retrieve Native AIBinder from wrapper if needed
    if (binder != nullptr) {
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);
        if (AIBinder_linkToDeath(binder.get(), deathRecipient, reinterpret_cast<void*>(static_cast<std::uintptr_t>(mSlotId))) !=
            STATUS_OK) {
            mtkLogW(LOG_TAG, "Failed to link death observer to the config binder, slot = %d", mSlotId);
        }
    } else {
        mtkLogW(LOG_TAG, "radio config Binder retrieval failed, slot = %d", mSlotId);
    }
    radioConfigServiceImpl[mSlotId].configIndication = radioConfigIndication;
    mCounterRadioConfig[mSlotId]++;
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    return ok();
}

ScopedAStatus RadioConfig::setSimSlotsMapping(  //
        int32_t serial, const std::vector<aidl::SlotPortMapping>& slotMap) {
    mtkLogD(LOG_TAG, "radioConfig::setSimSlotsMapping ");
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
                                                 RIL_REQUEST_SET_LOGICAL_TO_PHYSICAL_SLOT_MAPPING);
    if (pRI == NULL) {
        return ok();
    }
    int num = slotMap.size();
    int map[2 * num];
    for (int i = 0; i < num; i++) {
        map[2 * i] = slotMap[i].physicalSlotId;
        map[2 * i + 1] = slotMap[i].portId;
    }

    CONFIG_CALL_ONREQUEST(pRI->pCI->requestNumber, map, 2 * num * sizeof(int), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioConfig::getSimultaneousCallingSupport(int32_t serial) {
    dispatchConfigVoid(serial, mSlotId, RIL_REQUEST_GET_SIMULTANEOUS_CALLING_SUPPORT);
    return ok();
}

}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addConfigLock(std::shared_ptr<android::hardware::radio::config::RadioConfig> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_CONFIG_PORT_BASE 60000
static pthread_t gThreadIds[MAX_SIM_COUNT*2];

void* addRcpVsockRadioConfigAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    using android::hardware::radio::config::RadioConfig;
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "addRcpVsockRadioConfigAidlService setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_CONFIG_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "addRcpVsockRadioConfigAidlService setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "addRcpVsockRadioConfigAidlService VSOCK listen port: %u sloid:%d", RADIO_CONFIG_PORT_BASE + sloid, sloid);

    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    std::shared_ptr<android::hardware::radio::config::RadioConfig> radioConfigAidl = ndk::SharedRefBase::make<android::hardware::radio::config::RadioConfig>(sloid);
    addConfigLock(radioConfigAidl);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioConfigAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "addRcpVsockRadioConfigAidlService VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "addRcpVsockRadioConfigAidlService VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioConfigVsock(unsigned int sloid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
                     addRcpVsockRadioConfigAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioData");
    }
    mtkLogE(LOG_TAG, "create thread for VsockRadioData");
}
#endif
// publicRadioEcallAidlService
// void publicRadioConfigAidlService(void) {
//     /*std::shared_ptr<android::hardware::radio::config::RadioConfig> radioConfigAidl = ndk::SharedRefBase::make<android::hardware::radio::config::RadioConfig>(0);
//     gPublishedHals.push_back(radioConfigAidl);
//     const std::string instance = std::string() + android::hardware::radio::config::RadioConfig::descriptor + "/default";
//     binder_status_t status =
//         AServiceManager_addService(radioConfigAidl->asBinder().get(), instance.c_str());
//     mtkLogD(LOG_TAG, "aosp radioConfig addService status: %d", status);*/
// #ifdef MTK_HYP_DUAL_TELE_SUPPORT
//     createRadioConfigVsock(0);
// #endif
// }

void publicRadioConfigAidlService(unsigned int sloid) {
    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;

    const char *radioConfigDescriptor = "";
    if (android::hardware::radio::config::RadioConfig::descriptor != NULL) {
        radioConfigDescriptor = android::hardware::radio::config::RadioConfig::descriptor;
    }

    if (sloid == 0) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioConfigVsock(sloid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::config::RadioConfig> radioConfigAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::config::RadioConfig>(tboxSlotid);
        addConfigLock(radioConfigAidlforTbox);
        const std::string tboxInstance = std::string() + radioConfigDescriptor + "/tboxSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioConfigAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox config addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);
    } else if (sloid == 1) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioConfigVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::config::RadioConfig> radioConfigAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::config::RadioConfig>(tboxSlotid);
        addConfigLock(radioConfigAidlforTbox);
        const std::string tboxInstance = std::string() + radioConfigDescriptor + "/tboxSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioConfigAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox config addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);
    } else if (sloid == 2) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioConfigVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::config::RadioConfig> radioConfigAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::config::RadioConfig>(tboxSlotid);
        addConfigLock(radioConfigAidlforTbox);
        const std::string tboxInstance = std::string() + radioConfigDescriptor + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioConfigAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox config addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);
    } else if (sloid == 3) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        createRadioConfigVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::config::RadioConfig> radioConfigAidlforTbox = ndk::SharedRefBase::make<android::hardware::radio::config::RadioConfig>(tboxSlotid);
        addConfigLock(radioConfigAidlforTbox);
        const std::string tboxInstance = std::string() + radioConfigDescriptor + "/tboxSlot4";
        binder_status_t tboxStatus =
                AServiceManager_addService(radioConfigAidlforTbox->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox config addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);
    }

}

void aidlRadioConfig::checkAidlReturnStatus(unsigned int slotId, ndk::ScopedAStatus& ret) {
    if ((((ret.getStatus() == STATUS_DEAD_OBJECT) || (ret.getStatus() == STATUS_NO_INIT)) &&
         ret.getExceptionCode() == EX_TRANSACTION_FAILED) ||
        (ret.getExceptionCode() == EX_ILLEGAL_STATE && ret.getMessage() != nullptr &&
         std::string(ret.getMessage()) == rilAidlUtils::binderDiedError)) {
        mtkLogE(LOG_TAG, "radioconfig checkAidlReturnStatus: unable to call response/indication");
        // Remote process hosting the callbacks must be dead. Reset the callback objects;
        // there's no other recovery to be done here. When the client process is back up, it will
        // call setResponseFunctions()
        // Caller should already hold rdlock, release that first
        // note the current counter to avoid overwriting updates made by another thread before
        // write lock is acquired.
        int counter = mCounterRadioConfig[slotId];
        pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(slotId);
        int gotRLock = 0;
        if (radio::isTidInRLock((int) slotId, pthread_self()) != -1) {
            // had got the lock
            gotRLock = 1;
            radio::unlockRadioServiceRlock(radioServiceRwlockPtr, (int) slotId);
            mtkLogV(LOG_TAG, "radioconfig checkAidlReturnStatus: release r lock %d", slotId);
        } else {
            mtkLogE(LOG_TAG, "radioconfig checkAidlReturnStatus: didn't get r lock %d", slotId);
        }

        // acquire wrlock
        ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int) slotId);
        mtkLogD(LOG_TAG, "radioconfig checkAidlReturnStatus: got lock %d", slotId);
        // make sure the counter value has not changed
        if (counter == mCounterRadioConfig[slotId]) {
            mtkLogI(LOG_TAG, "radioconfig checkAidlReturnStatus[%d]: reset response/indication", slotId);
            radioConfigServiceImpl[slotId].configResponse = NULL;
            radioConfigServiceImpl[slotId].configIndication = NULL;
            mCounterRadioConfig[slotId]++;
        } else {
            mtkLogE(LOG_TAG, "radioconfig checkAidlReturnStatus: not resetting resFunc as they likely"
                    "got updated on another thread");
        }
        // release wrlock
        ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int) slotId);
        mtkLogV(LOG_TAG, "radioconfig checkAidlReturnStatus: release lock %d", slotId);
        if (gotRLock == 1) {
            // Reacquire rdlock
            radio::lockRadioServiceRlock(radioServiceRwlockPtr, (int)slotId);
            mtkLogV(LOG_TAG, "radioconfig checkAidlReturnStatus: got r lock %d", slotId);
        } else {
            mtkLogE(LOG_TAG, "checkAidlReturnStatus: don't need to re-get r lock %d", slotId);
        }
    } else {
        mtkLogE(LOG_TAG, "radioconfig checkAidlReturnStatus binder fail(%d)(%d)(%d)",
                slotId, ret.getStatus(), ret.getExceptionCode());
        mtkAssert((char*)"radioconfig checkAidlReturnStatus binder fail");
    }
}

int aidlRadioConfig::getSimSlotsStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "getSimSlotsStatusResponse: response");
    if (radioConfigServiceImpl[slotId].configResponse != NULL) {
        mtkLogD(LOG_TAG, "radioConfigServiceImpl[%d].configResponse != NULL", slotId);
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl_radio::config::SimSlotStatus> slotStatus = {};
        if (response == NULL || responselen % sizeof(RIL_SimSlotStatus *) != 0) {
            mtkLogE(LOG_TAG, "getSimSlotsStatusResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int num = responselen / sizeof(RIL_SimSlotStatus *);
            slotStatus.resize(num);
            for (int i = 0; i < num; i++) {
                RIL_SimSlotStatus *p_cur = ((RIL_SimSlotStatus **)response)[i];
                slotStatus[i].cardState = p_cur->card_state;
                slotStatus[i].atr = convertCharPtrToStdString(p_cur->atr);
                slotStatus[i].eid = convertCharPtrToStdString(p_cur->eid);

                if (p_cur->mepVersion == 1) {
                    for (int j = 0; j < p_cur->port_count; j++) {
                        aidl_radio::config::SimPortInfo simPortInfo;
                        simPortInfo.portActive = p_cur->info[j].portActive;
                        simPortInfo.logicalSlotId = p_cur->info[j].logicalSlotId;
                        simPortInfo.iccId = convertCharPtrToStdString(p_cur->info[j].iccId);
                        slotStatus[i].portInfo.push_back(simPortInfo);
                    }
                } else {
                    /// Meaning old modem not support MEP, just porting for aidl structure
                    aidl_radio::config::SimPortInfo simPortInfo;
                    if (p_cur->slotState == 0) {
                        simPortInfo.portActive = false;
                    } else {
                        simPortInfo.portActive = true;
                    }
                    simPortInfo.logicalSlotId = p_cur->logicalSlotId;
                    simPortInfo.iccId = convertCharPtrToStdString(p_cur->iccId);
                    slotStatus[i].portInfo.push_back(simPortInfo);
                }
                slotStatus[i].supportedMepMode =
                        (aidl_radio::config::MultipleEnabledProfilesMode) p_cur->mepMode;
            }
        }
        ndk::ScopedAStatus ret = radioConfigServiceImpl[slotId].configResponse->getSimSlotsStatusResponse(
                responseInfo, slotStatus);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSimSlotsStatusResponse: slotId = %d, binder fail!", slotId);
            aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getSimSlotsStatusResponse: radioConfigServiceImpl[%d].configResponse is NULL", slotId);
    }

    return 0;
}

int aidlRadioConfig::setSimSlotsMappingResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int serial, RIL_Errno e, const void *response, size_t responselen) {
    if (radioConfigServiceImpl[slotId].configResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioConfigServiceImpl[slotId].configResponse->
                setSimSlotsMappingResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setPreferredDataModemResponse slotId: binder fail!", slotId);
            aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setSimSlotsMappingResponse: radioConfigServiceImpl[%d].configResponse is NULL", slotId);
    }
    return 0;
}

int aidlRadioConfig::setPreferredDataModemResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int serial, RIL_Errno e, const void *response, size_t responselen) {
    if (radioConfigServiceImpl[slotId].configResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioConfigServiceImpl[slotId].configResponse->setPreferredDataModemResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setPreferredDataModemResponse slotId:%d: binder fail!", slotId);
            aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setPreferredDataModemResponse: radioConfigServiceImpl[%d].configResponse is NULL", slotId);
    }
    return 0;
}

int aidlRadioConfig::simSlotStatusChangedInd(unsigned int slotId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen) {
    if (radioConfigServiceImpl[slotId].configIndication != NULL) {
        mtkLogD(LOG_TAG, "radioConfigServiceImpl[%d]->configIndication != NULL", slotId);
        if (response == NULL || responselen % sizeof(RIL_SimSlotStatus *) != 0) {
            mtkLogE(LOG_TAG, "simSlotStatusChangedInd: Invalid response");
            return 0;
        }
        std::vector<aidl_radio::config::SimSlotStatus> slotStatus = {};

        int num = responselen / sizeof(RIL_SimSlotStatus *);
        slotStatus.resize(num);
        for (int i = 0; i < num; i++) {
            RIL_SimSlotStatus *p_cur = ((RIL_SimSlotStatus **)response)[i];
            slotStatus[i].cardState = p_cur->card_state;
            slotStatus[i].atr = convertCharPtrToStdString(p_cur->atr);
            slotStatus[i].eid = convertCharPtrToStdString(p_cur->eid);

            if (p_cur->mepVersion == 1) {
                for (int j = 0; j < p_cur->port_count; j++) {
                    aidl_radio::config::SimPortInfo simPortInfo;
                    simPortInfo.portActive = p_cur->info[j].portActive;
                    simPortInfo.logicalSlotId = p_cur->info[j].logicalSlotId;
                    simPortInfo.iccId = convertCharPtrToStdString(p_cur->info[j].iccId);
                    slotStatus[i].portInfo.push_back(simPortInfo);
                }
            } else {
                /// Meaning old modem not support MEP, just porting for aidl structure
                aidl_radio::config::SimPortInfo simPortInfo;
                if (p_cur->slotState == 0) {
                    simPortInfo.portActive = false;
                } else {
                    simPortInfo.portActive = true;
                }
                simPortInfo.logicalSlotId = p_cur->logicalSlotId;
                simPortInfo.iccId = convertCharPtrToStdString(p_cur->iccId);
                slotStatus[i].portInfo.push_back(simPortInfo);
            }
            slotStatus[i].supportedMepMode =
                    (aidl_radio::config::MultipleEnabledProfilesMode) p_cur->mepMode;
        }

        ndk::ScopedAStatus ret = radioConfigServiceImpl[slotId].configIndication
                ->simSlotsStatusChanged(rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                slotStatus);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "simSlotStatusChangedInd slotId:%d :binder fail!", slotId);
            aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioConfigServiceImpl[%d]->configIndication is NULL", slotId);
    }
    return 0;
}

int aidlRadioConfig::onSimultaneousCallingSupportChangedInd(
            unsigned int slotId, int indicationType, int token,
            RIL_Errno e, const void *response, size_t responselen) {
    if ((radioConfigServiceImpl[slotId].configIndication != NULL) && (slotId == 0)) {
        std::vector<int32_t> enabledLogicalSlots;
        fillEnabledLogicalSlots(enabledLogicalSlots, response, responselen);
        ndk::ScopedAStatus ret = radioConfigServiceImpl[slotId].configIndication
                ->onSimultaneousCallingSupportChanged(enabledLogicalSlots);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onSimultaneousCallingSupportChangedInd slotId:%d :binder fail!",
                    slotId);
            aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "radioConfigServiceImpl[%d]->configIndication is NULL or wrong slot id", slotId);
    }
    return 0;
}


int aidlRadioConfig::getPhoneCapabilityResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int serial, RIL_Errno e, const void *response, size_t responselen) {
    if (radioConfigServiceImpl[slotId].configResponse != NULL) {
        aidl_radio::config::PhoneCapability phoneCapability = {};
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        if (response == NULL) {
            mtkLogE(LOG_TAG, "getPhoneCapabilityResponse: Invalid response");
        } else {
            unsigned int mdMaxSimCount = getMdMaxSimCount();
            RIL_PhoneCapability *pPhoneCapability = ((RIL_PhoneCapability *) response);
            phoneCapability.maxActiveData = pPhoneCapability->maxActiveData;
            phoneCapability.maxActiveInternetData = pPhoneCapability->maxActiveInternetData;
            phoneCapability.isInternetLingeringSupported
                    = pPhoneCapability->isInternetLingeringSupported;
            phoneCapability.logicalModemIds.resize(mdMaxSimCount);
            for (unsigned int i = 0; i < mdMaxSimCount; i++) {
                phoneCapability.logicalModemIds.push_back(pPhoneCapability->logicalModemList[i].modemId);
            }
            // for cellular DSDA with simultaneous calling support, it should be 2.
            if (true == isMipcSupport()) {
                phoneCapability.maxActiveVoice =  2;
            } else {
                phoneCapability.maxActiveVoice =  1;
            }
        }
        ndk::ScopedAStatus ret = radioConfigServiceImpl[slotId].configResponse->
                getPhoneCapabilityResponse(responseInfo, phoneCapability);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getPhoneCapabilityResponse slotId:%d :binder fail!", slotId);
            aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getPhoneCapabilityResponse: mRadioConfigResponse is NULL");
    }

    return 0;
}

int aidlRadioConfig::setModemsConfigResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int serial, RIL_Errno e, const void *response, size_t responselen) {
    if (radioConfigServiceImpl[slotId].configResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioConfigServiceImpl[slotId].configResponse->
                setNumOfLiveModemsResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setModemsConfigResponse slotId:%d:binder fail!", slotId);
            aidlRadioConfig::checkAidlReturnStatus(slotId, ret);
        }
        if (e == RIL_E_SUCCESS) {
            // create local request to reset modem
            mtkLogI(LOG_TAG, "setModemsConfig successfully, reset modem/TRM");
            mtk_property_set("vendor.ril.disable.eboot", "1");
            RequestInfo *pRI = android::addRequestToList(0, slotId, RIL_REQUEST_RESTART_RILD);
            if (pRI == NULL) {
                mtkLogE(LOG_TAG, "setModemsConfigResponse: pRI is NULL");
                return 0;
            }
            pRI->local = 1;

            CONFIG_CALL_ONREQUEST(RIL_REQUEST_RESTART_RILD, NULL, 0, pRI, slotId);
        }
    } else {
        mtkLogE(LOG_TAG, "setModemsConfigResponse radioConfigServiceImpl[%d].configResponse != NULL is NULL", slotId);
    }
    return 0;
}

// void aidlRadioConfig::clearRadioConfigResponseAndIndications() {
//     radioConfigServiceImpl.configResponse = nullptr;
//     radioConfigServiceImpl.configIndication = nullptr;
// }

int aidlRadioConfig::getSimultaneousCallingSupportResponse(unsigned int slotId,
            android::ClientId clientId, int responseType,
            int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "getSimultaneousCallingSupportResponse: response %d", slotId);
    if (radioConfigServiceImpl[slotId].configResponse != NULL && slotId == 0) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> enabledLogicalSlots;
        fillEnabledLogicalSlots(enabledLogicalSlots, response, responselen);
        ndk::ScopedAStatus ret =
                radioConfigServiceImpl[slotId].configResponse->getSimultaneousCallingSupportResponse(
                        responseInfo, enabledLogicalSlots);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSimultaneousCallingSupportResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->radioConfigAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "getSimultaneousCallingSupportResponse:"
                "radioConfigServiceImpl[%d].configResponse is NULL or wrong slot id", slotId);
    }
    return 0;
}