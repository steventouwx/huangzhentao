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
#include <aidl/android/hardware/radio/modem/BnRadioModemIndication.h>
#include <aidl/android/hardware/radio/modem/BnRadioModemResponse.h>
#include <aidl/android/hardware/radio/modem/IRadioModem.h>

#include "radio_modem_response.h"
#include "radio_modem_indication.h"
#include "../radio_service.h"
//#include <aidl/android/hardware/radio/modem/ImeiInfo.h>

//#include "radio_aidl_hal_utils.h"

using namespace aidl::android::hardware::radio::modem;

class RadioModem : public RadioService {
public:
    // Private static instance of RadioModem
    //static RadioModem* instance;

    // Private constructor to prevent multiple instantiations.
    RadioModem(int slot, RfxDispatchThread* dispatchThread);

private:
    /* radio modem service handle */
    std::shared_ptr<IRadioModem> radio_modem;
    /* radio modem response handle */
    std::shared_ptr<RadioModemResponse> radioRsp_modem;
    /* radio modem indication handle */
    std::shared_ptr<RadioModemIndication> radioInd_modem;

public:
    int32_t getDeviceIdentity(int32_t in_serial);
    int32_t setRadioPower(int32_t in_serial, bool in_powerOn, bool in_forEmergencyCall, bool in_preferredForEmergencyCall);
};
