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

#include <aidl/android/hardware/radio/config/BnRadioConfigIndication.h>
#include <aidl/android/hardware/radio/config/BnRadioConfigResponse.h>
#include <aidl/android/hardware/radio/config/IRadioConfig.h>
#include <aidl/android/hardware/radio/RadioResponseInfo.h>
#include <aidl/android/hardware/radio/config/SimSlotStatus.h>
#include <aidl/android/hardware/radio/config/PhoneCapability.h>

#include "RfxDispatchThread.h"
#include "../radio_service.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::config;


/* Callback class for radio config response */
class RadioConfigResponse : public BnRadioConfigResponse {

public:
    RadioConfigResponse(int slot, RfxDispatchThread* dispatchThread);
private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

public:

    RadioResponseInfo rspInfo;

    virtual ndk::ScopedAStatus getHalDeviceCapabilitiesResponse(const RadioResponseInfo& in_info, bool in_modemReducedFeatureSet1) override;
    virtual ndk::ScopedAStatus getNumOfLiveModemsResponse(const RadioResponseInfo& in_info, int8_t in_numOfLiveModems) override;
    virtual ndk::ScopedAStatus getSimSlotsStatusResponse(const RadioResponseInfo& info, const std::vector<::aidl::android::hardware::radio::config::SimSlotStatus>& slotStatus) override;
    virtual ndk::ScopedAStatus setNumOfLiveModemsResponse(const RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setPreferredDataModemResponse(const RadioResponseInfo& info) override;
    virtual ndk::ScopedAStatus setSimSlotsMappingResponse(const RadioResponseInfo& info) override;
    virtual ndk::ScopedAStatus getPhoneCapabilityResponse(const RadioResponseInfo& in_info, const ::aidl::android::hardware::radio::config::PhoneCapability& in_phoneCapability) override;
    virtual ndk::ScopedAStatus getSimultaneousCallingSupportResponse(const RadioResponseInfo& info, const std::vector<int32_t>& enabledLogicalSlots) override;
    // int setPreferredDataModemResponse(unsigned int slotId, android::ClientId clientId, int responseType,
    //         int token, RIL_Errno e, const void *response, size_t responselen);
    // int simSlotStatusChangedInd(unsigned int slotId,
    //         int indicationType, int token, RIL_Errno e, const void *response,
    //         size_t responselen);
    // int getPhoneCapabilityResponse(unsigned int slotId, android::ClientId clientId, int responseType,
    //         int token, RIL_Errno e, const void *response, size_t responselen);
    // int setModemsConfigResponse(unsigned int slotId, android::ClientId clientId, int responseType,
    //         int token, RIL_Errno e, const void *response, size_t responselen);
    // int onSimultaneousCallingSupportChangedInd(unsigned int slotId,
    //         int indicationType, int token, RIL_Errno e, const void *response,
    //         size_t responselen);
    // int getSimultaneousCallingSupportResponse(unsigned int slotId, android::ClientId clientId,
    //         int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);

};
