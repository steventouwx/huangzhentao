// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "mtkradioex_ecall.h"
#include "mtkradioex_ecall_response.h"
#include "mtkradioex_ecall_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "MtkRadioExEcall"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio ecall service has died!");
}

 MtkRadioExEcall::MtkRadioExEcall(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IMtkRadioExEcall::descriptor + portNames[slot];
    RLOGD("init MtkRadioExEcall(%d), name = %s", slot, name.c_str());

    mtkradioex_ecall = IMtkRadioExEcall::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get mtk radio ecall service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_ecall->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio ecall service binder");
        }
    }

    mtkradioexRsp_ecall = ndk::SharedRefBase::make<MtkRadioExEcallResponse>(slot, dispatchThread);
    if(mtkradioexRsp_ecall.get() == nullptr) {
        RLOGE("Failed to create mtk radio ecall response");
    }

    mtkradioexInd_ecall = ndk::SharedRefBase::make<MtkRadioExEcallIndication>(slot,dispatchThread);
    if(mtkradioexInd_ecall.get() == nullptr) {
        RLOGE("Failed to create mtk radio ecall indication");
    }

    if (mtkradioex_ecall.get() != nullptr) {
        mtkradioex_ecall->setResponseFunctionsMtk(mtkradioexRsp_ecall, mtkradioexInd_ecall);
        // mtkradioex_voice->setResponseFunctionsMtkIms(mtkradioexRsp_voice, mtkradioexInd_voice);
        // RLOGE("setResponseFunctionsMtkIms@JA");
    } else {
        RLOGE("mtkradioex_ecall is null");
    }

 }

 int32_t MtkRadioExEcall::resetIvs(int32_t serial, int32_t clientId) {
    RLOGD("resetIvs start, slotId = %d, serial = %d", mSlot, serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->resetIvs(serial, clientId);
    if (!ret.isOk()) {
        RLOGE("resetIvs failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

 int32_t MtkRadioExEcall::setMSD(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetMSD& in_setMSD, int32_t in_clientId) {
    RLOGD("setMSD start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->setMSD(in_serial, in_setMSD, in_clientId);
    if (!ret.isOk()) {
        RLOGE("setMSD failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

 int32_t MtkRadioExEcall::setTestNum(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_testNum, int32_t in_clientId) {
    RLOGD("setTestNum start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->setTestNum(in_serial, in_testNum, in_clientId);
    if (!ret.isOk()) {
        RLOGE("setTestNum failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

 int32_t MtkRadioExEcall::setReconfNum(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_reconfNum, int32_t in_clientId) {
    RLOGD("setReconfNum start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->setReconfNum(in_serial, in_reconfNum, in_clientId);
    if (!ret.isOk()) {
        RLOGE("setReconfNum failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
 }

 int32_t MtkRadioExEcall::makeFastEcall(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallReqMsg& in_reqMsg, int32_t in_clientId) {
    RLOGD("makeFastEcall start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall(in_serial, in_reqMsg, in_clientId);
    if (!ret.isOk()) {
        RLOGE("makeFastEcall failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
 }

 int32_t MtkRadioExEcall::setEmsdPri(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallPRI& in_pri, int32_t in_clientId) {
    RLOGD("setEmsdPri start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->setEmsdPri(in_serial, in_pri, in_clientId);
    if (!ret.isOk()) {
        RLOGE("setEmsdPri failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
 }

 int32_t MtkRadioExEcall::setNadDeregTime(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallTime& in_time, int32_t in_clientId) {
    RLOGD("setNadDeregTime start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->setNadDeregTime(in_serial, in_time, in_clientId);
    if (!ret.isOk()) {
        RLOGE("setNadDeregTime failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
 }

 int32_t MtkRadioExEcall::setOprtMode(int32_t in_serial, int32_t mode, int32_t in_clientId) {
    RLOGD("setOprtMode start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->setOprtMode(in_serial, mode, in_clientId);
    if (!ret.isOk()) {
        RLOGE("setOprtMode failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
 }

 int32_t MtkRadioExEcall::getOprtMode(int32_t in_serial, int32_t in_clientId) {
    RLOGD("getOprtMode start, serial = %d", in_serial);
    if(mtkradioex_ecall.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ecall->getOprtMode(in_serial, in_clientId);
    if (!ret.isOk()) {
        RLOGE("getOprtMode failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
 }

