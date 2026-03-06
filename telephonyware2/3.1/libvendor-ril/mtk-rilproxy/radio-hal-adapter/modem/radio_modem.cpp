// SPDX-License-Identifier: MediaTekProprietary
#include <aidl/android/hardware/radio/config/IRadioConfig.h>
#include <android/binder_manager.h>
#include <android/binder_ibinder.h>
#include <mtk_log.h>

#include "radio_modem.h"
#include "../radio_hal_utils.h"
#include "radio_modem_indication.h"
#include "radio_modem_response.h"

#define LOG_TAG "RadioModem"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The radio modem service has died!");
}

// Constructor definition
RadioModem::RadioModem(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IRadioModem::descriptor + portNames[slot];
    RLOGD("init RadioModem(%d), name = %s", slot, name.c_str());
    radio_modem = IRadioModem::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(radio_modem.get() == nullptr) {
        RLOGE("Failed to get radio modem service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_modem->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio modem service binder");
        }
    }

    radioRsp_modem = ndk::SharedRefBase::make<RadioModemResponse>(slot, dispatchThread);
    if(radioRsp_modem.get() == nullptr) {
        RLOGE("Failed to create radio modem response");
    }

    radioInd_modem = ndk::SharedRefBase::make<RadioModemIndication>(slot, dispatchThread);
    if(radioInd_modem.get() == nullptr) {
        RLOGE("Failed to create radio modem indication");
    }

    if (radio_modem.get() != nullptr) {
        radio_modem->setResponseFunctions(radioRsp_modem, radioInd_modem);
    } else {
        RLOGE("radio_modem is null");
    }

}

int32_t RadioModem::getDeviceIdentity(int32_t in_serial) {
    RLOGD("getDeviceIdentity start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_modem.get() == nullptr) {
        RLOGE("Failed to get radio modem service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_modem->getDeviceIdentity(in_serial);
    if (!ret.isOk()) {
        RLOGE("getDeviceIdentity failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioModem::setRadioPower(int32_t in_serial, bool in_powerOn, bool in_forEmergencyCall, bool in_preferredForEmergencyCall) {
    RLOGD("setRadioPower start, slotId = %d, serial=%d, powerOn=%d, emergencyCall=%d, preferred=%d",
    mSlot, in_serial, in_powerOn, in_forEmergencyCall, in_preferredForEmergencyCall);
    if(radio_modem.get() == nullptr) {
        RLOGE("Failed to get radio modem service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_modem->setRadioPower(in_serial, in_powerOn, in_forEmergencyCall, in_preferredForEmergencyCall);
    if (!ret.isOk()) {
        RLOGE("setRadioPower failed, slotId = %d", mSlot);
        return -1;
    }
    RLOGD("setRadioPower end, slotId= %d", mSlot);
    return 0;
}
