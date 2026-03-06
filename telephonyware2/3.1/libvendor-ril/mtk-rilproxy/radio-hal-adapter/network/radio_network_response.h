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

/* Callback class for radio network response */
class RadioNetworkResponse : public BnRadioNetworkResponse {

public:
    RadioNetworkResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

public:
    // RadioNetworkResponse(RadioServiceTest& parent_network);
    // virtual ~RadioNetworkResponse() = default;

    RadioResponseInfo rspInfo;
    std::vector<RadioBandMode> radioBandModes;
    std::vector<OperatorInfo> networkInfos;
    bool isNrDualConnectivityEnabled = false;
    int networkTypeBitmapResponse = 0;
    RegStateResult voiceRegResp;
    RegStateResult dataRegResp;
    CellIdentity barringCellIdentity;
    std::vector<BarringInfo> barringInfoList;
    UsageSetting usageSetting;
    std::vector<RadioAccessSpecifier> specifiers;
    bool isCellularIdentifierTransparencyEnabled = false;
    bool isSecurityAlgorithmsUpdatedEnabled = false;

    virtual ndk::ScopedAStatus acknowledgeRequest(int32_t serial) override;

    virtual ndk::ScopedAStatus getAllowedNetworkTypesBitmapResponse(
            const RadioResponseInfo& info, const int32_t networkTypeBitmap) override;

    virtual ndk::ScopedAStatus getAvailableBandModesResponse(
            const RadioResponseInfo& info, const std::vector<RadioBandMode>& bandModes) override;

    virtual ndk::ScopedAStatus getAvailableNetworksResponse(
            const RadioResponseInfo& info, const std::vector<OperatorInfo>& networkInfos) override;

    virtual ndk::ScopedAStatus getBarringInfoResponse(
            const RadioResponseInfo& info, const CellIdentity& cellIdentity,
            const std::vector<BarringInfo>& barringInfos) override;

    virtual ndk::ScopedAStatus getCdmaRoamingPreferenceResponse(const RadioResponseInfo& info,
                                                                CdmaRoamingType type) override;

    virtual ndk::ScopedAStatus getCellInfoListResponse(
            const RadioResponseInfo& info, const std::vector<CellInfo>& cellInfo) override;

    virtual ndk::ScopedAStatus getDataRegistrationStateResponse(
            const RadioResponseInfo& info, const RegStateResult& dataRegResponse) override;

    virtual ndk::ScopedAStatus getImsRegistrationStateResponse(
            const RadioResponseInfo& info, bool isRegistered,
            RadioTechnologyFamily ratFamily) override;

    virtual ndk::ScopedAStatus getNetworkSelectionModeResponse(const RadioResponseInfo& info,
                                                               bool manual) override;

    virtual ndk::ScopedAStatus getOperatorResponse(const RadioResponseInfo& info,
                                                   const std::string& longName,
                                                   const std::string& shortName,
                                                   const std::string& numeric) override;

    virtual ndk::ScopedAStatus getSignalStrengthResponse(
            const RadioResponseInfo& info, const SignalStrength& sigStrength) override;

    virtual ndk::ScopedAStatus getSystemSelectionChannelsResponse(
            const RadioResponseInfo& info,
            const std::vector<RadioAccessSpecifier>& specifier) override;

    virtual ndk::ScopedAStatus getUsageSettingResponse(const RadioResponseInfo& info,
                                                       UsageSetting usageSetting) override;

    virtual ndk::ScopedAStatus getVoiceRadioTechnologyResponse(const RadioResponseInfo& info,
                                                               RadioTechnology rat) override;

    void   handleRegStateResult(const RadioResponseInfo &info,const RegStateResult &regResponse);

    virtual ndk::ScopedAStatus getVoiceRegistrationStateResponse(
        const RadioResponseInfo &info,
        const RegStateResult &voiceRegResponse) override;

    virtual ndk::ScopedAStatus isNrDualConnectivityEnabledResponse(const RadioResponseInfo& info,
                                                                   bool isEnabled) override;

    virtual ndk::ScopedAStatus setAllowedNetworkTypesBitmapResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setBandModeResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setBarringPasswordResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCdmaRoamingPreferenceResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCellInfoListRateResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setIndicationFilterResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setLinkCapacityReportingCriteriaResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setLocationUpdatesResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setNetworkSelectionModeAutomaticResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setNetworkSelectionModeManualResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setNrDualConnectivityStateResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setSignalStrengthReportingCriteriaResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setSuppServiceNotificationsResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setSystemSelectionChannelsResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setUsageSettingResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus startNetworkScanResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus stopNetworkScanResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus supplyNetworkDepersonalizationResponse(
            const RadioResponseInfo& info, int32_t remainingRetries) override;

    virtual ndk::ScopedAStatus setEmergencyModeResponse(
            const RadioResponseInfo& info, const EmergencyRegResult& regState) override;

    virtual ndk::ScopedAStatus triggerEmergencyNetworkScanResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus exitEmergencyModeResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus cancelEmergencyNetworkScanResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setNullCipherAndIntegrityEnabledResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus isNullCipherAndIntegrityEnabledResponse(
            const RadioResponseInfo& info, const bool isEnabled) override;

    virtual ndk::ScopedAStatus isN1ModeEnabledResponse(
            const RadioResponseInfo& info, bool isEnabled) override;

    virtual ndk::ScopedAStatus setN1ModeEnabledResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCellularIdentifierTransparencyEnabledResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus isCellularIdentifierTransparencyEnabledResponse(
            const RadioResponseInfo& info, bool /*enabled*/) override;

    virtual ndk::ScopedAStatus isSecurityAlgorithmsUpdatedEnabledResponse(
            const RadioResponseInfo& info, bool isEnabled) override;

    virtual ndk::ScopedAStatus setSecurityAlgorithmsUpdatedEnabledResponse(
            const RadioResponseInfo& info) override;
};
