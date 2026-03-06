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
#include <aidl/android/hardware/radio/network/BnRadioNetworkIndication.h>
#include <aidl/android/hardware/radio/network/BnRadioNetworkResponse.h>
#include <aidl/android/hardware/radio/network/IRadioNetwork.h>
#include <stdint.h>
#include "radio_network_response.h"
#include "radio_network_indication.h"
#include "../radio_service.h"

//#include "radio_aidl_hal_utils.h"

using namespace aidl::android::hardware::radio::network;

class RadioNetWork : public RadioService{
private:
    // Private static instance of RadioNetWork
    //static RadioNetWork* instance;

    // Private constructor to prevent multiple instantiations.
    /* radio network service handle */
    std::shared_ptr<IRadioNetwork> radio_network;
    /* radio network response handle */
    std::shared_ptr<RadioNetworkResponse> radioRsp_network;
    /* radio network indication handle */
    std::shared_ptr<RadioNetworkIndication> radioInd_network;
public:
    // Public accessor for the instance
    RadioNetWork(int solt, RfxDispatchThread* dispatchThread);
public:
  int32_t getVoiceRegistrationState(int32_t in_serial);
  int32_t getDataRegistrationState(int32_t in_serial);
  int32_t getImsRegistrationState(int32_t in_serial);
  int32_t getSignalStrength(int32_t in_serial);
  int32_t setAllowedNetworkTypesBitmap(int32_t serial, int32_t networkTypeBitmap);
  int32_t getAllowedNetworkTypesBitmap(int32_t serial);
  int32_t setIndicationFilter(int32_t serial, int32_t indFilter);
  int32_t setCellInfoListRate(int32_t serial, int32_t rate);
  int32_t setUsageSetting(int32_t serial, int32_t usageSetting);
  int32_t getUsageSetting(int32_t in_serial);
  int32_t setNetworkSelectionModeAutomatic(int32_t in_serial);
  int32_t setNetworkSelectionModeManual(int32_t in_serial,  const std::string& opNumeric, AccessNetwork ran);
  int32_t getNetworkSelectionMode(int32_t in_serial);
  int32_t getCellInfoList(int32_t in_serial);
  int32_t getOperator(int32_t in_serial);
};
