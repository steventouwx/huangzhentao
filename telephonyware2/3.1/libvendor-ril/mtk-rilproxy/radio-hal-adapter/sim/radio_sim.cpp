// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "radio_sim.h"
#include "radio_sim_response.h"
#include "radio_sim_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "RadioSim"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The radio sim service has died!");
}

RadioSim::RadioSim(int slot, RfxDispatchThread* dispatchThread)
{
    mSlot = slot;
    std::string name = std::string() + IRadioSim::descriptor + portNames[slot];
    RLOGD("init RadioSim(%d), name = %s", slot, name.c_str());
    radio_sim = IRadioSim::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_sim->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio sim service binder");
        }
    }

    radioRsp_sim = ndk::SharedRefBase::make<RadioSimResponse>(slot, dispatchThread);
    if(radioRsp_sim.get() == nullptr) {
        RLOGE("Failed to create radio sim response");
    }

    radioInd_sim = ndk::SharedRefBase::make<RadioSimIndication>(slot, dispatchThread);
    if(radioInd_sim.get() == nullptr) {
        RLOGE("Failed to create radio sim indication");
    }

    if (radio_sim.get() != nullptr) {
        radio_sim->setResponseFunctions(radioRsp_sim, radioInd_sim);
    } else {
        RLOGE("radio_sim is null");
    }


}

int32_t RadioSim::getImsiForApp(int32_t in_serial, const std::string& in_aid) {
    RLOGD("getImsiForApp start, slotId = %d, serial = %d, aid = %s", mSlot, in_serial, in_aid.c_str());
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->getImsiForApp(in_serial, in_aid);
    if (!ret.isOk()) {
        RLOGE("getImsiForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::getIccCardStatus(int32_t in_serial) {
    RLOGD("getIccCardStatus start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->getIccCardStatus(in_serial);
    if (!ret.isOk()) {
        RLOGE("getIccCardStatus failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::supplyIccPinForApp(int32_t in_serial, const std::string& in_pin, const std::string& in_aid){
    RLOGD("supplyIccPinForApp start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->supplyIccPinForApp(in_serial, in_pin, in_aid);
    if (!ret.isOk()) {
        RLOGE("supplyIccPinForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::supplyIccPin2ForApp(int32_t in_serial, const std::string& in_pin, const std::string& in_aid){
    RLOGD("supplyIccPin2ForApp start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->supplyIccPinForApp(in_serial, in_pin, in_aid);
    if (!ret.isOk()) {
        RLOGE("supplyIccPin2ForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::supplyIccPukForApp(int32_t in_serial, const std::string& in_puk, const std::string& in_pin, const std::string& in_aid){
    RLOGD("supplyIccPukForApp start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->supplyIccPukForApp(in_serial, in_puk, in_pin, in_aid);
    if (!ret.isOk()) {
        RLOGE("supplyIccPukForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::supplyIccPuk2ForApp(int32_t in_serial, const std::string& in_puk, const std::string& in_pin, const std::string& in_aid){
    RLOGD("supplyIccPuk2ForApp start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->supplyIccPuk2ForApp(in_serial, in_puk, in_pin, in_aid);
    if (!ret.isOk()) {
        RLOGE("supplyIccPuk2ForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::changeIccPinForApp(int32_t in_serial, const std::string& old_pin, const std::string& new_pin,  const std::string& in_aid){
    RLOGD("changeIccPinForApp start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->changeIccPinForApp(in_serial, old_pin, new_pin, in_aid);
    if (!ret.isOk()) {
        RLOGE("changeIccPinForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::changeIccPin2ForApp(int32_t in_serial, const std::string& old_pin2, const std::string& new_pin2,  const std::string& in_aid){
    RLOGD("changeIccPin2ForApp start, slotId = %d, serial = %d", mSlot, in_serial);
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->changeIccPin2ForApp(in_serial, old_pin2, new_pin2, in_aid);
    if (!ret.isOk()) {
        RLOGE("changeIccPin2ForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::setFacilityLockForApp(int32_t in_serial, const std::string& facility, int32_t lockState, const std::string& password, int32_t serviceClass, const std::string& appId){
    RLOGD("setFacilityLockForApp start, slotId = %d, serial = %d", mSlot, in_serial);
    RLOGD("setFacilityLockForApp facility = %s, lockState = %d, password = %s serviceClass = %d appId = %s", facility.c_str(), lockState, password.c_str(), serviceClass, appId.c_str());
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->setFacilityLockForApp(in_serial, facility, lockState, password, serviceClass, appId);
    if (!ret.isOk()) {
        RLOGE("setFacilityLockForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::iccIoForApp(int32_t in_serial, const ::aidl::android::hardware::radio::sim::IccIo& iccIo){
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to get radio sim service");
        return -1;
    }
    ndk::ScopedAStatus ret = radio_sim->iccIoForApp(in_serial, iccIo);
    if (!ret.isOk()) {
        RLOGE("iccIoForApp failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioSim::setSimCardPower(int32_t in_serial, ::aidl::android::hardware::radio::sim::CardPowerState powerUp){
    if(radio_sim.get() == nullptr) {
        RLOGE("Failed to setSimCardPower service");
        return -1;
    }
    RLOGD("setSimCardPower start, slotId = %d, serial = %d mode = %d ", mSlot, in_serial, powerUp);
    ndk::ScopedAStatus ret = radio_sim->setSimCardPower(in_serial, powerUp);
    if (!ret.isOk()) {
        RLOGE("setSimCardPower failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}