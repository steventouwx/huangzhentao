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
#include <aidl/android/hardware/radio/RadioResponseInfo.h>
#include <aidl/android/hardware/radio/data/PdpProtocolType.h>
#include <aidl/android/hardware/radio/data/LinkAddress.h>
#include <cstdint>
#include <memory>

#include "RfxDispatchThread.h"
#include "../radio_service.h"
using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::data;


/* Callback class for radio data response */
class RadioDataResponse : public BnRadioDataResponse {
public:
    RadioDataResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

private:
    std::string convertPdpProtocolTypeToString(::aidl::android::hardware::radio::data::PdpProtocolType pdpType);
    std::string convertVecLinkAddressToCharPtr(const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& records);
    std::string convertVecAidlStringToCharPtr(const std::vector<std::string>& records);
public:

    RadioResponseInfo rspInfo;
    int32_t allocatedPduSessionId;
    SetupDataCallResult setupDataCallResult;

    virtual ndk::ScopedAStatus acknowledgeRequest(int32_t serial) override;

    virtual ndk::ScopedAStatus allocatePduSessionIdResponse(const RadioResponseInfo& info,
                                                            int32_t id) override;

    virtual ndk::ScopedAStatus cancelHandoverResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus deactivateDataCallResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus getDataCallListResponse(
            const RadioResponseInfo& info,
            const std::vector<SetupDataCallResult>& dcResponse) override;

    virtual ndk::ScopedAStatus getSlicingConfigResponse(
            const RadioResponseInfo& info, const SlicingConfig& slicingConfig) override;

    virtual ndk::ScopedAStatus releasePduSessionIdResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setDataAllowedResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setDataProfileResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setDataThrottlingResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setInitialAttachApnResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setupDataCallResponse(
            const RadioResponseInfo& info, const SetupDataCallResult& dcResponse) override;

    virtual ndk::ScopedAStatus startHandoverResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus startKeepaliveResponse(const RadioResponseInfo& info,
                                                      const KeepaliveStatus& status) override;

    virtual ndk::ScopedAStatus stopKeepaliveResponse(const RadioResponseInfo& info) override;
};

