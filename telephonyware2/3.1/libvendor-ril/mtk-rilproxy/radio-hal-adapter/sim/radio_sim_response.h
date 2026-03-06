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

#include <cstdint>
#include <memory>
#include <aidl/android/hardware/radio/sim/BnRadioSimResponse.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>
#include <aidl/android/hardware/radio/RadioResponseInfo.h>
#include <aidl/android/hardware/radio/RadioError.h>
#include <aidl/android/hardware/radio/config/IRadioConfig.h>
#include <aidl/android/hardware/radio/config/SimSlotStatus.h>
#include <aidl/android/hardware/radio/network/RegState.h>
#include <aidl/android/hardware/radio/sim/CardStatus.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>

#include "RfxDispatchThread.h"
#include "../radio_service.h"
using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::sim;

/* Callback class for radio SIM response */
class RadioSimResponse : public BnRadioSimResponse {
public:
    RadioSimResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;
  public:

    RadioResponseInfo rspInfo;
    CarrierRestrictions carrierRestrictionsResp;
    SimLockMultiSimPolicy multiSimPolicyResp;
    bool canToggleUiccApplicationsEnablement;
    bool areUiccApplicationsEnabled;
    PhonebookCapacity capacity;
    int32_t updatedRecordIndex;
    std::string imsi;
    CardStatus cardStatus;

    virtual ndk::ScopedAStatus acknowledgeRequest(int32_t serial) override;

    virtual ndk::ScopedAStatus areUiccApplicationsEnabledResponse(const RadioResponseInfo& info,
                                                                  bool enabled) override;

    virtual ndk::ScopedAStatus changeIccPin2ForAppResponse(const RadioResponseInfo& info,
                                                           int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus changeIccPinForAppResponse(const RadioResponseInfo& info,
                                                          int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus enableUiccApplicationsResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus getAllowedCarriersResponse(
            const RadioResponseInfo& info, const CarrierRestrictions& carriers,
            const SimLockMultiSimPolicy multiSimPolicy) override;

    virtual ndk::ScopedAStatus getCdmaSubscriptionResponse(
            const RadioResponseInfo& info, const std::string& mdn, const std::string& hSid,
            const std::string& hNid, const std::string& min, const std::string& prl) override;

    virtual ndk::ScopedAStatus getCdmaSubscriptionSourceResponse(
            const RadioResponseInfo& info, CdmaSubscriptionSource source) override;

    virtual ndk::ScopedAStatus getFacilityLockForAppResponse(const RadioResponseInfo& info,
                                                             int32_t response) override;

    virtual ndk::ScopedAStatus getIccCardStatusResponse(const RadioResponseInfo& info,
                                                        const CardStatus& cardStatus) override;

    virtual ndk::ScopedAStatus getImsiForAppResponse(const RadioResponseInfo& info,
                                                     const std::string& imsi) override;

    virtual ndk::ScopedAStatus getSimPhonebookCapacityResponse(
            const RadioResponseInfo& info, const PhonebookCapacity& capacity) override;

    virtual ndk::ScopedAStatus getSimPhonebookRecordsResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus iccCloseLogicalChannelResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus iccCloseLogicalChannelWithSessionInfoResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus iccIoForAppResponse(const RadioResponseInfo& info,
                                                   const IccIoResult& iccIo) override;

    virtual ndk::ScopedAStatus iccOpenLogicalChannelResponse(
            const RadioResponseInfo& info, int32_t channelId,
            const std::vector<uint8_t>& selectResponse) override;

    virtual ndk::ScopedAStatus iccTransmitApduBasicChannelResponse(
            const RadioResponseInfo& info, const IccIoResult& result) override;

    virtual ndk::ScopedAStatus iccTransmitApduLogicalChannelResponse(
            const RadioResponseInfo& info, const IccIoResult& result) override;

    virtual ndk::ScopedAStatus reportStkServiceIsRunningResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus requestIccSimAuthenticationResponse(
            const RadioResponseInfo& info, const IccIoResult& result) override;

    virtual ndk::ScopedAStatus sendEnvelopeResponse(const RadioResponseInfo& info,
                                                    const std::string& commandResponse) override;

    virtual ndk::ScopedAStatus sendEnvelopeWithStatusResponse(const RadioResponseInfo& info,
                                                              const IccIoResult& iccIo) override;

    virtual ndk::ScopedAStatus sendTerminalResponseToSimResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setAllowedCarriersResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCarrierInfoForImsiEncryptionResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCdmaSubscriptionSourceResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setFacilityLockForAppResponse(const RadioResponseInfo& info,
                                                             int32_t retry) override;

    virtual ndk::ScopedAStatus setSimCardPowerResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setUiccSubscriptionResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus supplyIccPin2ForAppResponse(const RadioResponseInfo& info,
                                                           int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus supplyIccPinForAppResponse(const RadioResponseInfo& info,
                                                          int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus supplyIccPuk2ForAppResponse(const RadioResponseInfo& info,
                                                           int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus supplyIccPukForAppResponse(const RadioResponseInfo& info,
                                                          int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus supplySimDepersonalizationResponse(
            const RadioResponseInfo& info, PersoSubstate persoType,
            int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus updateSimPhonebookRecordsResponse(
            const RadioResponseInfo& info, int32_t updatedRecordIndex) override;
};
