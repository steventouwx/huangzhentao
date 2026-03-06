// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "mtkradioex_sim.h"
#include "radio_sim_response.h"
#include "radio_sim_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "MtkRadioExSim"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio sim service has died!");
}

MtkRadioExSim::MtkRadioExSim(int slot, RfxDispatchThread* dispatchThread)
{
    mSlot = slot;
    std::string name = std::string() + IMtkRadioExSim::descriptor + portNames[slot];
    RLOGD("init MtkRadioExSim(%d), name = %s", slot, name.c_str());
    mtkradioex_sim = IMtkRadioExSim::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(mtkradioex_sim.get() == nullptr) {
        RLOGE("Failed to get mtk radio sim service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_sim->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio sim service binder");
        }
    }

    mtkradioexRsp_sim = ndk::SharedRefBase::make<MtkRadioExSimResponse>(slot, dispatchThread);
    if(mtkradioexRsp_sim.get() == nullptr) {
        RLOGE("Failed to create mtk radio sim response");
    }

    mtkradioexInd_sim = ndk::SharedRefBase::make<MtkRadioExSimIndication>(slot, dispatchThread);
    if(mtkradioexInd_sim.get() == nullptr) {
        RLOGE("Failed to create mtk radio sim indication");
    }

    if (mtkradioex_sim.get() != nullptr) {
        mtkradioex_sim->setResponseFunctionsMtk(mtkradioexRsp_sim, mtkradioexInd_sim);
    } else {
        RLOGE("mtkradioex_sim is null");
    }

}

int32_t MtkRadioExSim::getIccid(int32_t in_serial, int32_t in_clientId) {
    RLOGD("getIccid start, slotId = %d, serial = %d, client_id=%d", mSlot, in_serial, in_clientId);
    if(mtkradioex_sim.get() == nullptr) {
        RLOGE("Failed to get mtk radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_sim->getIccid(in_serial, in_clientId);
    if (!ret.isOk()) {
        RLOGE("getIccid failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}