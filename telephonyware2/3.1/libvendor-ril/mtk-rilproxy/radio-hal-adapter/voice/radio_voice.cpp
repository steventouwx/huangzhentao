// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "radio_voice.h"
#include "radio_voice_response.h"
#include "radio_voice_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "RadioVoice"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The radio voice service has died!");
}

RadioVoice::RadioVoice(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IRadioVoice::descriptor + portNames[slot];
    RLOGD("init RadioVoice(%d), name = %s", slot, name.c_str());
    radio_voice = IRadioVoice::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio voice service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_voice->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio voice service binder");
        }
    }


    radioRsp_voice = ndk::SharedRefBase::make<RadioVoiceResponse>(slot, dispatchThread);
    if(radioRsp_voice.get() == nullptr) {
        RLOGE("Failed to create radio voice response");
    }


    radioInd_voice = ndk::SharedRefBase::make<RadioVoiceIndication>(slot, dispatchThread);
    if(radioInd_voice.get() == nullptr) {
        RLOGE("Failed to create radio voice indication");
    }

    if (radio_voice.get() != nullptr) {
        radio_voice->setResponseFunctions(radioRsp_voice, radioInd_voice);
    } else {
        RLOGE("radio_voice is null");
    }

}


int32_t RadioVoice::dial(int32_t serial, const Dial& dialInfo) {
    RLOGD("dial start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->dial(serial, dialInfo);
    if (!ret.isOk()) {
        RLOGE("dial failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::acceptCall(int32_t serial) {
    RLOGD("acceptCall start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->acceptCall(serial);
    if (!ret.isOk()) {
        RLOGE("acceptCall failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::getCurrentCalls(int32_t serial) {
    RLOGD("getCurrentCalls start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->getCurrentCalls(serial);
    if (!ret.isOk()) {
        RLOGE("getCurrentCalls failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::startDtmf(int32_t serial, const std::string& phonenum) {
    RLOGD("startDtmf start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->startDtmf(serial, phonenum);
    if (!ret.isOk()) {
        RLOGE("startDtmf failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::stopDtmf(int32_t serial) {
    RLOGD("stopDtmf start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->stopDtmf(serial);
    if (!ret.isOk()) {
        RLOGE("stopDtmf failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::hangup(int32_t serial, int32_t gsmIndex) {
    RLOGD("hangup start, slotId = %d, serial = %d, gsmIndex = %d ", mSlot, serial, gsmIndex);
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->hangup(serial, gsmIndex);
    if (!ret.isOk()) {
        RLOGE("hangup failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}


int32_t RadioVoice::isVoNrEnabled(int32_t serial) {
    RLOGD("isVoNrEnabled start, slotId = %d, serial = %d", mSlot, serial);
    if (radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->isVoNrEnabled(serial);
    if (!ret.isOk()) {
        RLOGE("isVoNrEnabled failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::setVoNrEnabled(int32_t serial, bool enable) {
    RLOGD("setVoNrEnabled start, slotId = %d, serial = %d, enable = %d", mSlot, serial, enable);
    if (radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->setVoNrEnabled(serial, enable);
    if (!ret.isOk()) {
        RLOGE("setVoNrEnabled failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::setCallWaiting(int32_t serial, bool enable, int32_t serviceClass) {
    RLOGD("setCallWaiting start, slotId = %d, serial = %d, enable = %d, serviceClass = %d", mSlot, serial, enable, serviceClass);
    if (radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->setCallWaiting(serial, enable, serviceClass);
    if (!ret.isOk()) {
        RLOGE("setCallWaiting failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::getCallWaiting(int32_t serial, int32_t serviceClass) {
    RLOGD("getCallWaiting start, slotId = %d, serial = %d, serviceClass = %d", mSlot, serial,serviceClass);
    if (radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->getCallWaiting(serial, serviceClass);
    if (!ret.isOk()) {
        RLOGE("getCallWaiting failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioVoice::getLastCallFailCause(int32_t serial) {
    RLOGD("getLastCallFailCause start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_voice.get() == nullptr) {
        RLOGE("Failed to get radio dial service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_voice->getLastCallFailCause(serial);
    if (!ret.isOk()) {
        RLOGE("getLastCallFailCause failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}