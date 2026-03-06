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

#include <aidl/android/hardware/radio/modem/BnRadioModemIndication.h>
#include <aidl/android/hardware/radio/modem/BnRadioModemResponse.h>
#include <aidl/android/hardware/radio/modem/IRadioModem.h>
//#include <aidl/android/hardware/radio/modem/ImeiInfo.h>
#include "RfxDispatchThread.h"

/* Callback class for radio modem indication */
using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::modem;
class RadioModemIndication : public BnRadioModemIndication {
public:
    RadioModemIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;
// private:
//     // Private default constructor
//     RadioModemIndication();

//     // Private instance pointer
//     static RadioModemIndication* instance;

// public:
//     // Disabled copy constructor and copy assignment operator
//     RadioModemIndication(const RadioModemIndication&) = delete;
//     RadioModemIndication& operator=(const RadioModemIndication&) = delete;

//     // Method to access the singleton instance
//     static RadioModemIndication* getInstance();

public:
    virtual ndk::ScopedAStatus hardwareConfigChanged(
            RadioIndicationType type, const std::vector<HardwareConfig>& configs) override;

    virtual ndk::ScopedAStatus modemReset(RadioIndicationType type,
                                          const std::string& reason) override;

    virtual ndk::ScopedAStatus radioCapabilityIndication(RadioIndicationType type,
                                                         const RadioCapability& rc) override;

    virtual ndk::ScopedAStatus radioStateChanged(RadioIndicationType type,
                                                 RadioState radioState) override;

    virtual ndk::ScopedAStatus rilConnected(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus onImeiMappingChanged(RadioIndicationType type,
            const ::aidl::android::hardware::radio::modem::ImeiInfo& imeiInfo) override;
};
