// SPDX-License-Identifier: MediaTekProprietary
/*
 * Copyright (C) 2022 The Android Open Source Project
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

#include <aidl/android/hardware/radio/ims/BnRadioImsIndication.h>
#include <aidl/android/hardware/radio/ims/BnRadioImsResponse.h>
#include <aidl/android/hardware/radio/ims/IRadioIms.h>
#include <aidl/android/hardware/radio/RadioIndicationType.h>
#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::ims;

/* Callback class for radio ims indication */
class RadioImsIndication : public BnRadioImsIndication {
public:
    RadioImsIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus onConnectionSetupFailure(RadioIndicationType type,
            int32_t token, const ConnectionFailureInfo& info) override;

    virtual ndk::ScopedAStatus notifyAnbr(RadioIndicationType type, ImsStreamType mediaType,
            ImsStreamDirection direction, int bitsPerSecond) override;

    virtual ndk::ScopedAStatus triggerImsDeregistration(RadioIndicationType type,
            ImsDeregistrationReason reason) override;
};