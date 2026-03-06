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
#include <aidl/android/hardware/radio/data/PdpProtocolType.h>
#include <aidl/android/hardware/radio/data/LinkAddress.h>

#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::data;


/* Callback class for radio data indication */
class RadioDataIndication : public BnRadioDataIndication {
public:
    RadioDataIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

private:
    std::string convertPdpProtocolTypeToString(::aidl::android::hardware::radio::data::PdpProtocolType pdpType);
    std::string convertVecLinkAddressToCharPtr(const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& records);
    std::string convertVecAidlStringToCharPtr(const std::vector<std::string>& records);

  public:
    virtual ndk::ScopedAStatus dataCallListChanged(
            RadioIndicationType type, const std::vector<SetupDataCallResult>& dcList) override;

    virtual ndk::ScopedAStatus keepaliveStatus(RadioIndicationType type,
                                               const KeepaliveStatus& status) override;

    virtual ndk::ScopedAStatus pcoData(RadioIndicationType type, const PcoDataInfo& pco) override;

    virtual ndk::ScopedAStatus unthrottleApn(RadioIndicationType type,
                                             const DataProfileInfo& dataProfile) override;
    virtual ndk::ScopedAStatus slicingConfigChanged(RadioIndicationType type,
                                                    const SlicingConfig& slicingConfig) override;
};
