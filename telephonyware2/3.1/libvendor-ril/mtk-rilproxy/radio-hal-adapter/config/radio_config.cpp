// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "radio_config.h"
#include "radio_config_response.h"
#include "radio_config_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "RadioConfig"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio config service has died!");
}

RadioConfig::RadioConfig(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IRadioConfig::descriptor + portNames[slot];
    RLOGD("init RadioConfig(%d), name = %s", slot, name.c_str());
    radio_config = IRadioConfig::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(radio_config.get() == nullptr) {
        RLOGE("Failed to get radio config service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_config->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio config service binder");
        }
    }


    radioRsp_config = ndk::SharedRefBase::make<RadioConfigResponse>(slot, dispatchThread);
    if(radioRsp_config.get() == nullptr) {
        RLOGE("Failed to create radio config response");
    }


    radioInd_config = ndk::SharedRefBase::make<RadioConfigIndication>(slot, dispatchThread);
    if(radioInd_config.get() == nullptr) {
        RLOGE("Failed to create radio config indication");
    }

    if (radio_config.get() != nullptr) {
        radio_config->setResponseFunctions(radioRsp_config, radioInd_config);
    } else {
        RLOGE("radio_config is null");
    }

}

int32_t RadioConfig::setPreferredDataModem(int32_t serial, int8_t modemId) {
    RLOGD("dial start, slotId = %d, serial = %d, input value = %d", mSlot, serial, modemId);
    if(radio_config.get() == nullptr) {
        RLOGE("Failed to get radio config service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_config->setPreferredDataModem(serial, modemId);
    if (!ret.isOk()) {
        RLOGE("setPreferredDataModem failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}