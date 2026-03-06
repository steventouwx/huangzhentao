// SPDX-License-Identifier: MediaTekProprietary
#pragma once
#include <aidl/android/hardware/radio/sim/BnRadioSimIndication.h>
#include <aidl/android/hardware/radio/sim/BnRadioSimResponse.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>
#include <memory>
#include <cstdint>

#include "radio_sim_response.h"
#include "radio_sim_indication.h"
#include "../radio_service.h"

using namespace aidl::android::hardware::radio::sim;

class RadioSim : public RadioService {
private:
    /* data */
public:
    /* radio voice service handle */
    std::shared_ptr<IRadioSim> radio_sim;
    /* radio SIM response handle */
    std::shared_ptr<RadioSimResponse> radioRsp_sim;
    /* radio SIM indication handle */
    std::shared_ptr<RadioSimIndication> radioInd_sim;
public:
     RadioSim(int slot, RfxDispatchThread* dispatchThread);

public:
    int32_t getImsiForApp(int32_t in_serial, const std::string& in_aid);
    int32_t getIccCardStatus(int32_t in_serial);
    int32_t supplyIccPinForApp(int32_t in_serial, const std::string& in_pin, const std::string& in_aid);
    int32_t supplyIccPin2ForApp(int32_t in_serial, const std::string& in_pin2, const std::string& in_aid);
    int32_t supplyIccPukForApp(int32_t in_serial, const std::string& in_puk, const std::string& in_pin, const std::string& in_aid);
    int32_t supplyIccPuk2ForApp(int32_t in_serial, const std::string& in_puk2, const std::string& in_pin2, const std::string& in_aid);
    int32_t changeIccPinForApp(int32_t in_serial, const std::string& old_pin, const std::string& new_pin, const std::string& in_aid);
    int32_t changeIccPin2ForApp(int32_t in_serial, const std::string& old_pin2, const std::string& new_pin2, const std::string& in_aid);
    int32_t setFacilityLockForApp(int32_t in_serial, const std::string& facility, int32_t lockState, const std::string& password, int32_t serviceClass, const std::string& appId);
    int32_t iccIoForApp(int32_t in_serial, const ::aidl::android::hardware::radio::sim::IccIo& iccIo);
    int32_t setSimCardPower(int32_t in_serial, ::aidl::android::hardware::radio::sim::CardPowerState powerUp);
};
