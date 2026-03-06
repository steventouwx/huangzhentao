// SPDX-License-Identifier: MediaTekProprietary
/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <aidl/android/hardware/radio/network/BnRadioNetworkIndication.h>
#include <aidl/android/hardware/radio/network/BnRadioNetworkResponse.h>
#include <aidl/android/hardware/radio/network/IRadioNetwork.h>
#include <aidl/android/hardware/radio/RadioError.h>
#include <aidl/android/hardware/radio/config/IRadioConfig.h>
#include <aidl/android/hardware/radio/config/SimSlotStatus.h>
#include <aidl/android/hardware/radio/network/RegState.h>
#include <aidl/android/hardware/radio/sim/CardStatus.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>

#include "RfxDispatchThread.h"
using namespace aidl::android::hardware::radio;
using aidl::android::hardware::radio::config::SimSlotStatus;
using aidl::android::hardware::radio::network::RegState;
using aidl::android::hardware::radio::sim::CardStatus;
using namespace aidl::android::hardware::radio::network;

/* Callback class for radio network indication */
class RadioNetworkIndication : public BnRadioNetworkIndication {

public:
    RadioNetworkIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

public:
    // Disabled copy constructor and copy assignment operator
    RadioNetworkIndication(const RadioNetworkIndication&) = delete;
    RadioNetworkIndication& operator=(const RadioNetworkIndication&) = delete;

    // Method to access the singleton instance
    static RadioNetworkIndication* getInstance();

  public:

    virtual ndk::ScopedAStatus barringInfoChanged(
            RadioIndicationType type, const CellIdentity& cellIdentity,
            const std::vector<BarringInfo>& barringInfos) override;

    virtual ndk::ScopedAStatus cdmaPrlChanged(RadioIndicationType type, int32_t version) override;

    virtual ndk::ScopedAStatus cellInfoList(RadioIndicationType type,
                                            const std::vector<CellInfo>& records) override;

    virtual ndk::ScopedAStatus currentLinkCapacityEstimate(
            RadioIndicationType type, const LinkCapacityEstimate& lce) override;

    virtual ndk::ScopedAStatus currentPhysicalChannelConfigs(
            RadioIndicationType type, const std::vector<PhysicalChannelConfig>& configs) override;

    virtual ndk::ScopedAStatus currentSignalStrength(RadioIndicationType type,
                                                     const SignalStrength& signalStrength) override;

    virtual ndk::ScopedAStatus imsNetworkStateChanged(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus networkScanResult(RadioIndicationType type,
                                                 const NetworkScanResult& result) override;

    virtual ndk::ScopedAStatus networkStateChanged(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus nitzTimeReceived(RadioIndicationType type,
                                                const std::string& nitzTime, int64_t receivedTimeMs,
                                                int64_t ageMs) override;

    virtual ndk::ScopedAStatus registrationFailed(RadioIndicationType type,
                                                  const CellIdentity& cellIdentity,
                                                  const std::string& chosenPlmn, int32_t domain,
                                                  int32_t causeCode,
                                                  int32_t additionalCauseCode) override;

    virtual ndk::ScopedAStatus restrictedStateChanged(RadioIndicationType type,
                                                      PhoneRestrictedState state) override;

    virtual ndk::ScopedAStatus suppSvcNotify(RadioIndicationType type,
                                             const SuppSvcNotification& suppSvc) override;

    virtual ndk::ScopedAStatus voiceRadioTechChanged(RadioIndicationType type,
                                                     RadioTechnology rat) override;

    virtual ndk::ScopedAStatus emergencyNetworkScanResult(
            RadioIndicationType type, const EmergencyRegResult& result) override;

    virtual ndk::ScopedAStatus cellularIdentifierDisclosed(
            RadioIndicationType type, const CellularIdentifierDisclosure& disclosures) override;

    virtual ndk::ScopedAStatus securityAlgorithmsUpdated(
            RadioIndicationType type,
            const SecurityAlgorithmUpdate& securityAlgorithmUpdate) override;
};
