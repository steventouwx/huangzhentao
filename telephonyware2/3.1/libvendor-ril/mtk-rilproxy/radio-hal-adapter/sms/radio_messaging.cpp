// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "radio_messaging.h"
#include "radio_messaging_response.h"
#include "radio_messaging_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "RadioMessaging"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The radio messaging service has died!");
}

RadioMessaging:: RadioMessaging(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IRadioMessaging::descriptor + portNames[slot];
    RLOGD("init RadioMessaging(%d), name = %s", slot, name.c_str());
    radio_messaging = IRadioMessaging::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(radio_messaging.get() == nullptr) {
        RLOGE("Failed to get radio messaging service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_messaging->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio messaging service binder");
        }
    }

    radioRsp_messaging = ndk::SharedRefBase::make<RadioMessagingResponse>(slot, dispatchThread);
    if(radioRsp_messaging.get() == nullptr) {
        RLOGE("Failed to create radio messaging response");
    }

    radioInd_messaging = ndk::SharedRefBase::make<RadioMessagingIndication>(slot,dispatchThread);
    if(radioInd_messaging.get() == nullptr) {
        RLOGE("Failed to create radio messaging indication");
    }

    if (radio_messaging.get() != nullptr) {
        radio_messaging->setResponseFunctions(radioRsp_messaging, radioInd_messaging);
    } else {
        RLOGE("radio_messaging is null");
    }

}


int32_t RadioMessaging::sendSms(int32_t serial, const GsmSmsMessage& message) {
    RLOGD("sendSms start, slotId = %d, serial = %d", mSlot, serial);
    if(radio_messaging.get() == nullptr) {
        RLOGE("Failed to get radio sms service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_messaging->sendSms(serial, message);
    if (!ret.isOk()) {
        RLOGE("sendSms failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioMessaging::acknowledgeLastIncomingGsmSms(int32_t serial, bool success, SmsAcknowledgeFailCause cause) {
    RLOGD("acknowledgeLastIncomingGsmSms start, slotId = %d, serial = %d, success = %d, cause = %d", mSlot, serial, success, cause);
    if(radio_messaging.get() == nullptr) {
        RLOGE("Failed to get radio sms service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_messaging->acknowledgeLastIncomingGsmSms(serial, success, cause);
    if (!ret.isOk()) {
        RLOGE("acknowledgeLastIncomingGsmSms failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}