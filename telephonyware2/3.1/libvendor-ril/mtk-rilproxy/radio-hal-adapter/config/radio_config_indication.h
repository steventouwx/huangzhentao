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

#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::config;

namespace aidl_radio = ::aidl::android::hardware::radio;

/* Callback class for radio config indication */
class RadioConfigIndication : public BnRadioConfigIndication {
public:
    RadioConfigIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:
    virtual ndk::ScopedAStatus simSlotsStatusChanged(RadioIndicationType type, const std::vector<aidl_radio::config::SimSlotStatus> &slotStatus);
    virtual ndk::ScopedAStatus onSimultaneousCallingSupportChanged(const std::vector<int32_t> &enabledLogicalSlots);
};

