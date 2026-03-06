// SPDX-License-Identifier: MediaTekProprietary
#include <aidl/android/hardware/radio/RadioError.h>
#include <android/binder_manager.h>
#include <mtk_log.h>

#include "radio_network.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "RadioNetwork"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The radio network service has died!");
}

RadioNetWork::RadioNetWork(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IRadioNetwork::descriptor + portNames[slot];
    RLOGD("init RadioNetWork(%d), name = %s", slot, name.c_str());
    radio_network = IRadioNetwork::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));

    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get radio network service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_network->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio network service binder");
        }
    }

    radioRsp_network = ndk::SharedRefBase::make<RadioNetworkResponse>(slot, dispatchThread);
    if(radioRsp_network.get() == nullptr) {
        RLOGE("Failed to create radio network response");
    }

    radioInd_network = ndk::SharedRefBase::make<RadioNetworkIndication>(slot, dispatchThread);
    if(radioInd_network.get() == nullptr) {
        RLOGE("Failed to create radio network indication");
    }

    if (radio_network.get() != nullptr) {
        radio_network->setResponseFunctions(radioRsp_network, radioInd_network);
    } else {
        RLOGE("radio_network is null");
    }

}

int32_t RadioNetWork::getVoiceRegistrationState(int32_t in_serial) {
    RLOGD("getVoiceRegistrationState start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get radio network service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getVoiceRegistrationState(in_serial);
    if (!ret.isOk()) {
        RLOGE("getVoiceRegistrationState failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioNetWork::getDataRegistrationState(int32_t in_serial) {
    RLOGD("getDataRegistrationState start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get radio network service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getDataRegistrationState(in_serial);
    if (!ret.isOk()) {
        RLOGE("getDataRegistrationState failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioNetWork::getImsRegistrationState(int32_t in_serial) {
    RLOGD("getImsRegistrationState start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get ra network service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getImsRegistrationState(in_serial);
    if (!ret.isOk()) {
        RLOGE("getImsRegistrationState failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioNetWork::getSignalStrength(int32_t in_serial) {
    RLOGD("getSignalStrength start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get ra network service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getSignalStrength(in_serial);
    if (!ret.isOk()) {
        RLOGE("getSignalStrength failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}


  int32_t RadioNetWork:: setAllowedNetworkTypesBitmap(int32_t in_serial, int32_t networkTypeBitmap) {
    RLOGD("setAllowedNetworkTypesBitmap start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get ra network service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->setAllowedNetworkTypesBitmap(in_serial, networkTypeBitmap);
    if (!ret.isOk()) {
        RLOGE("setAllowedNetworkTypesBitmap failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }


  int32_t RadioNetWork:: getAllowedNetworkTypesBitmap(int32_t in_serial) {
    RLOGD("getAllowedNetworkTypesBitmap start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get ra network service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getAllowedNetworkTypesBitmap(in_serial);
    if (!ret.isOk()) {
        RLOGE("getAllowedNetworkTypesBitmap failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }

  int32_t RadioNetWork:: setIndicationFilter(int32_t serial, int32_t indFilter) {
    RLOGD("setIndicationFilter start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to get ra network service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->setIndicationFilter(serial, indFilter);
    if (!ret.isOk()) {
        RLOGE("setIndicationFilter failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }

  int32_t RadioNetWork:: setCellInfoListRate(int32_t serial, int32_t rate) {
    RLOGD("setCellInfoListRate start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_network.get() == nullptr) {
        RLOGE("Failed to setCellInfoListRate");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->setCellInfoListRate(serial, rate);
    if (!ret.isOk()) {
        RLOGE("setCellInfoListRate failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }

  int32_t RadioNetWork::setUsageSetting(int32_t serial, int32_t usageSetting) {
    RLOGD("setUsageSetting start, slotId = %d, serial = %d", mSlot, serial);
    if (radio_network.get() == nullptr) {
        RLOGE("Failed to setUsageSetting");
        return -1;
    }
    UsageSetting setting;
    if (usageSetting == 1) {
        setting = UsageSetting::VOICE_CENTRIC;
    } else if (usageSetting == 2) {
        setting = UsageSetting::DATA_CENTRIC;
    } else {
        RLOGE("Invalid usageSetting value: %d", usageSetting);
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->setUsageSetting(serial, setting);
    if (!ret.isOk()) {
        RLOGE("setUsageSetting failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }

  int32_t RadioNetWork::getUsageSetting(int32_t in_serial) {
    RLOGD("getUsageSetting start, slotId = %d, serial = %d", mSlot, in_serial);
    if (radio_network.get() == nullptr) {
        RLOGE("Failed to getUsageSetting");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getUsageSetting(in_serial);
    if (!ret.isOk()) {
        RLOGE("getUsageSetting failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }

int32_t RadioNetWork::setNetworkSelectionModeAutomatic(int32_t in_serial) {
    RLOGD("%s start, slotId = %d, serial = %d", __func__, mSlot, in_serial);
    if (radio_network.get() == nullptr) {
        RLOGE("Failed to %s", __func__);
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->setNetworkSelectionModeAutomatic(in_serial);
    if (!ret.isOk()) {
        RLOGE("%s failed!, slotId = %d", __func__, mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioNetWork::setNetworkSelectionModeManual(int32_t in_serial, const std::string& opNumeric, AccessNetwork ran) {
    RLOGD("%s start, slotId = %d, serial = %d, opNumeric = %s, ran = %s", __func__, mSlot, in_serial, opNumeric.c_str(), toString(ran).c_str());
    if (radio_network.get() == nullptr) {
        RLOGE("Failed to %s", __func__);
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->setNetworkSelectionModeManual(in_serial, opNumeric, ran);
    if (!ret.isOk()) {
        RLOGE("%s failed!, slotId = %d", __func__, mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioNetWork::getNetworkSelectionMode(int32_t in_serial) {
    RLOGD("%s start, slotId = %d, serial = %d", __func__, mSlot, in_serial);
    if (radio_network.get() == nullptr) {
        RLOGE("Failed to %s", __func__);
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getNetworkSelectionMode(in_serial);
    if (!ret.isOk()) {
        RLOGE("%s failed!, slotId = %d", __func__, mSlot);
        return -1;
    }
    return 0;
}

  int32_t RadioNetWork::getCellInfoList(int32_t in_serial) {
    RLOGD("getCellInfoList start, slotId = %d, serial = %d", mSlot, in_serial);
    if (radio_network.get() == nullptr) {
        RLOGE("Failed to getCellInfoList");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getCellInfoList(in_serial);
    if (!ret.isOk()) {
        RLOGE("getCellInfoList failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }

  int32_t RadioNetWork::getOperator(int32_t in_serial) {
    RLOGD("getOperator start, slotId = %d, serial = %d", mSlot, in_serial);
    if (radio_network.get() == nullptr) {
        RLOGE("Failed to getOperator");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_network->getOperator(in_serial);
    if (!ret.isOk()) {
        RLOGE("getOperator failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
  }
