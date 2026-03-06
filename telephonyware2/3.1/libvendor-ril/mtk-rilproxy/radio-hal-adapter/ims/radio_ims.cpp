// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "radio_ims.h"
#include "radio_ims_response.h"
#include "radio_ims_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "RadioIms"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The radio ims service has died!");
}

RadioIms::RadioIms(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IRadioIms::descriptor + imsPortNames[slot];
    RLOGD("init RadioIms(%d), name = %s", slot, name.c_str());
    radio_ims = IRadioIms::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if (radio_ims.get() == nullptr) {
        RLOGD("Failed to get radio IMS service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_ims->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio ims service binder");
        }
    }

    radioRsp_ims = ndk::SharedRefBase::make<RadioImsResponse>(slot, dispatchThread);
    if (radioRsp_ims.get() == nullptr) {
        RLOGE("Failed to create radio IMS response");
    }

    radioInd_ims = ndk::SharedRefBase::make<RadioImsIndication>(slot, dispatchThread);
    if (radioInd_ims.get() == nullptr) {
        RLOGE("Failed to create radio ims indication");
    }

    if (radio_ims.get() != nullptr) {
        radio_ims->setResponseFunctions(radioRsp_ims, radioInd_ims);
    } else {
        RLOGE("radio_ims is null");
    }

}