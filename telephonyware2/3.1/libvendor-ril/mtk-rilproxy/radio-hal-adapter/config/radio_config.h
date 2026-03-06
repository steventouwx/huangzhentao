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

#include <memory>
#include <cstdint>

#include "radio_config_response.h"
#include "radio_config_indication.h"
#include "../radio_service.h"

using namespace aidl::android::hardware::radio::config;

// The main test class for Radio AIDL Config.
class RadioConfig : public RadioService {

public:
     RadioConfig(int slot, RfxDispatchThread* dispatchThread);
  public:
    /* radio config service handle */
    std::shared_ptr<IRadioConfig> radio_config;
    /* radio config response handle */
    std::shared_ptr<RadioConfigResponse> radioRsp_config;
    /* radio config indication handle */
    std::shared_ptr<RadioConfigIndication> radioInd_config;

    int32_t setPreferredDataModem(int32_t serial, int8_t modemId);
};
