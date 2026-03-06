// SPDX-License-Identifier: MediaTekProprietary
#include <aidl/android/hardware/radio/config/IRadioConfig.h>
#include <android/binder_manager.h>
#include <android/binder_ibinder.h>
#include <mtk_log.h>

#include "mtkradioex_modem.h"
#include "../radio_hal_utils.h"


#define LOG_TAG "MtkRadioExModem"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio modem service has died!");
}


MtkRadioExModem::MtkRadioExModem(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IMtkRadioExModem::descriptor + portNames[slot];
    RLOGD("init MtkRadioExModem(%d), name = %s", slot, name.c_str());
    mtkradioex_modem = IMtkRadioExModem::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(mtkradioex_modem.get() == nullptr) {
        RLOGE("Failed to get mtk radio modem service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_modem->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio modem service binder");
        }
    }

    mtkradioexRsp_modem = ndk::SharedRefBase::make<MtkRadioExModemResponse>(slot, dispatchThread);
    if (mtkradioexRsp_modem.get() == nullptr) {
        RLOGE("Failed to create mtk radio modem response");
    }

    mtkradioexInd_modem = ndk::SharedRefBase::make<MtkRadioExModemIndication>(slot, dispatchThread);
    if (mtkradioexInd_modem.get() == nullptr) {
        RLOGE("Failed to create mtk radio modem indication");
    }

    if (mtkradioex_modem.get() != nullptr) {
        mtkradioex_modem->setResponseFunctionsMtk(mtkradioexRsp_modem, mtkradioexInd_modem);
    } else {
        RLOGE("mtkradioex_modem is null");
    }
}

  int32_t MtkRadioExModem::sendRequestRaw(int32_t in_serial, const std::vector<uint8_t>& in_data, int32_t in_clientId) {
    RLOGD("sendRequestRaw start, slotId = %d, serial = %d, clientId = %d", mSlot, in_serial, in_clientId);
    if(mtkradioex_modem.get() == nullptr) {
        RLOGE("Failed to get mtk radio modem service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_modem->sendRequestRaw(in_serial, in_data, in_clientId);
    if (!ret.isOk()) {
        RLOGE("sendRequestRaw failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }

  int32_t MtkRadioExModem::setModemPower(int32_t in_serial, bool in_isOn, int32_t in_clientId) {
    RLOGD("setModemPower start, slotId = %d, serial = %d, ison = %d, clientId = %d", mSlot, in_serial, in_isOn, in_clientId);
    if(mtkradioex_modem.get() == nullptr) {
        RLOGE("Failed to get radio modem service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_modem->setModemPower(in_serial, in_isOn, in_clientId);
    if (!ret.isOk()) {
        RLOGE("setModemPower failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }
