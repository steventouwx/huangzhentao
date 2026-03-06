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

#include <aidl/android/hardware/radio/messaging/BnRadioMessagingIndication.h>
#include <aidl/android/hardware/radio/messaging/BnRadioMessagingResponse.h>
#include <aidl/android/hardware/radio/messaging/IRadioMessaging.h>

#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::messaging;

/* Callback class for radio messaging indication */
class RadioMessagingIndication : public BnRadioMessagingIndication {
public:
    RadioMessagingIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus cdmaNewSms(RadioIndicationType type,
                                          const CdmaSmsMessage& msg) override;

    virtual ndk::ScopedAStatus cdmaRuimSmsStorageFull(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus newBroadcastSms(RadioIndicationType type,
                                               const std::vector<uint8_t>& data) override;

    virtual ndk::ScopedAStatus newSms(RadioIndicationType type,
                                      const std::vector<uint8_t>& pdu) override;

    virtual ndk::ScopedAStatus newSmsOnSim(RadioIndicationType type, int32_t recordNumber) override;

    virtual ndk::ScopedAStatus newSmsStatusReport(RadioIndicationType type,
                                                  const std::vector<uint8_t>& pdu) override;

    virtual ndk::ScopedAStatus simSmsStorageFull(RadioIndicationType type) override;
};
