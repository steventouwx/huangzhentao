// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "mtkradioex_voice.h"
#include "mtkradioex_voice_response.h"
#include "mtkradioex_voice_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "MtkRadioExVoice"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio voice service has died!");
}

 MtkRadioExVoice::MtkRadioExVoice(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IMtkRadioExVoice::descriptor + portNames[slot];
    RLOGD("init MtkRadioExVoice(%d), name = %s", slot, name.c_str());

    mtkradioex_voice = IMtkRadioExVoice::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(mtkradioex_voice.get() == nullptr) {
        RLOGE("Failed to get mtk radio voice service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_voice->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio voice service binder");
        }
    }

    mtkradioexRsp_voice = ndk::SharedRefBase::make<MtkRadioExVoiceResponse>(slot, dispatchThread);
    if(mtkradioexRsp_voice.get() == nullptr) {
        RLOGE("Failed to create mtk radio voice response");
    }

    mtkradioexInd_voice = ndk::SharedRefBase::make<MtkRadioExVoiceIndication>(slot,dispatchThread);
    if(mtkradioexInd_voice.get() == nullptr) {
        RLOGE("Failed to create mtk radio voice indication");
    }



    if (mtkradioex_voice.get() != nullptr) {
        mtkradioex_voice->setResponseFunctionsMtk(mtkradioexRsp_voice, mtkradioexInd_voice);
        mtkradioex_voice->setResponseFunctionsMtkIms(mtkradioexRsp_voice, mtkradioexInd_voice);
        RLOGE("setResponseFunctionsMtkIms@JA");
    } else {
        RLOGE("mtkradioex_voice is null");
    }

 }

 int32_t MtkRadioExVoice::hangupAll(int32_t serial, int32_t clientId) {
    RLOGD("hangupAll start, slotId = %d, serial = %d", mSlot, serial);
    if(mtkradioex_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_voice->hangupAll(serial, clientId);
    if (!ret.isOk()) {
        RLOGE("hangupAll failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}


int32_t MtkRadioExVoice::setCallIndication(int32_t serial, int32_t mode, int32_t callId, int32_t seqNumber, int32_t cause, int32_t clientId) {
    RLOGD("setCallIndication start, slotId = %d, serial = %d", mSlot, serial);
    if(mtkradioex_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_voice->setCallIndication(serial, mode, callId, seqNumber, cause, clientId);
    if (!ret.isOk()) {
        RLOGE("setCallIndication failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}