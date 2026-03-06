// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "mtkradioex_network.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "MtkRadioExNetWork"
// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio network service has died!");
}

// Initialize the static instance pointer to nullptr
//MtkRadioExNetWork* MtkRadioExNetWork::instance = nullptr;

// Private constructor definition (empty if nothing to initialize)
MtkRadioExNetWork::MtkRadioExNetWork(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IMtkRadioExNetwork::descriptor + portNames[slot];
    RLOGD("init MtkRadioExNetWork(%d), name = %s", slot, name.c_str());
    mtkradioex_network = IMtkRadioExNetwork::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(mtkradioex_network.get() == nullptr) {
        RLOGE("Failed to get mtkradioex network service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_network->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio network service binder");
        }
    }

    mtkradioexRsp_network = ndk::SharedRefBase::make<MtkRadioExNetworkResponse>(slot, dispatchThread);
    if(mtkradioexRsp_network.get() == nullptr) {
        RLOGE("Failed to create mtkradioex network response object");
    }

    mtkradioexInd_network = ndk::SharedRefBase::make<MtkRadioExNetworkIndication>(slot, dispatchThread);
    if(mtkradioexInd_network.get() == nullptr) {
        RLOGE("Failed to create mtkradioex network indication object");
    }

    if (mtkradioex_network.get() != nullptr) {
        mtkradioex_network->setResponseFunctionsMtk(mtkradioexRsp_network, mtkradioexInd_network);
    } else {
        RLOGE("mtkradioex_network is null");
    }

}

int32_t MtkRadioExNetWork::getAvailableNetworksWithAct(int serial, int32_t in_clientId) {
    RLOGD("getAvailableNetworksWithAct start, slotId = %d, serial = %d, clientId = %d", mSlot, serial, in_clientId);
    if(mtkradioex_network.get() == nullptr) {
        RLOGE("Failed to get mtk radio network service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_network->getAvailableNetworksWithAct(serial, in_clientId);
    if (!ret.isOk()) {
        RLOGE("getAvailableNetworksWithAct failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t MtkRadioExNetWork::getNitzTime(int serial, int32_t in_clientId) {
    RLOGD("getNitzTime start, slotId = %d, serial = %d, clientId = %d", mSlot, serial, in_clientId);
    if(mtkradioex_network.get() == nullptr) {
        RLOGE("Failed to get mtk radio network service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_network->getNitzTime(serial, in_clientId);
    if (!ret.isOk()) {
        RLOGE("getNitzTime failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}
