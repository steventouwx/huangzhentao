// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "mtkradioex_messaging.h"
#include "radio_messaging_response.h"
#include "radio_messaging_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "MtkRadioExMessaging"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio messaging service has died!");
}

MtkRadioExMessaging::MtkRadioExMessaging(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IMtkRadioExMessaging::descriptor + portNames[slot];
    RLOGD("init MtkRadioExMessaging(%d), name = %s", slot, name.c_str());
    mtkradioex_messaging = IMtkRadioExMessaging::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(mtkradioex_messaging.get() == nullptr) {
        RLOGE("Failed to get mtk radio messaging service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_messaging->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio messaging service binder");
        }
    }


    mtkradioexRsp_messaging = ndk::SharedRefBase::make<MtkRadioExMessagingResponse>(slot, dispatchThread);
    if(mtkradioexRsp_messaging.get() == nullptr) {
        RLOGE("Failed to create mtk radio messaging response");
    }

    mtkradioexInd_messaging = ndk::SharedRefBase::make<MtkRadioExMessagingIndication>(slot, dispatchThread);
    if(mtkradioexInd_messaging.get() == nullptr) {
        RLOGE("Failed to create mtk radio messaging indication");
    }

    if (mtkradioex_messaging.get() != nullptr) {
        mtkradioex_messaging->setResponseFunctionsMtk(mtkradioexRsp_messaging, mtkradioexInd_messaging);
    } else {
        RLOGE("mtkradioex_messaging is null");
    }
}