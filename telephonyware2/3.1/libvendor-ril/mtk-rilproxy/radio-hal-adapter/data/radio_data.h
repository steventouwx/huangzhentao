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

#include <aidl/android/hardware/radio/data/BnRadioDataIndication.h>
#include <aidl/android/hardware/radio/data/BnRadioDataResponse.h>
#include <aidl/android/hardware/radio/data/IRadioData.h>
#include <aidl/android/hardware/radio/data/KeepaliveRequest.h>

#include <memory>
#include <cstdint>

#include "radio_data_response.h"
#include "radio_data_indication.h"
#include "../radio_service.h"
#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio::data;

// The main test class for Radio AIDL Data.
class RadioData : public RadioService {
public:
    RadioData(int slot, RfxDispatchThread* dispatchThread);

public:

    /* radio data service handle */
    std::shared_ptr<IRadioData> radio_data;
    /* radio data response handle */
    std::shared_ptr<RadioDataResponse> radioRsp_data;
    /* radio data indication handle */
    std::shared_ptr<RadioDataIndication> radioInd_data;
public:
  int32_t deactivateDataCall(int32_t in_serial, int32_t in_cid, ::aidl::android::hardware::radio::data::DataRequestReason in_reason);
  int32_t getDataCallList(int32_t in_serial);
  int32_t setDataAllowed(int32_t in_serial, bool in_allow);
  int32_t setDataProfile(int32_t in_serial, const std::vector<::aidl::android::hardware::radio::data::DataProfileInfo>& in_profiles);
  int32_t setInitialAttachApn(int32_t in_serial, const std::optional<::aidl::android::hardware::radio::data::DataProfileInfo>& in_dataProfileInfo);
  int32_t setupDataCall(int32_t in_serial, ::aidl::android::hardware::radio::AccessNetwork in_accessNetwork, const ::aidl::android::hardware::radio::data::DataProfileInfo& in_dataProfileInfo, bool in_roamingAllowed, ::aidl::android::hardware::radio::data::DataRequestReason in_reason, const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& in_addresses, const std::vector<std::string>& in_dnses, int32_t in_pduSessionId, const std::optional<::aidl::android::hardware::radio::data::SliceInfo>& in_sliceInfo, bool in_matchAllRuleAllowed);
  int32_t startKeepalive(int32_t in_serial, const ::aidl::android::hardware::radio::data::KeepaliveRequest& in_keepalive);
  int32_t stopKeepalive(int32_t in_serial, int32_t in_sessionHandle);
};