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

#include <aidl/android/hardware/radio/voice/BnRadioVoiceIndication.h>
#include <aidl/android/hardware/radio/voice/BnRadioVoiceResponse.h>
#include <aidl/android/hardware/radio/voice/IRadioVoice.h>

#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::voice;

/* Callback class for radio voice indication */
class RadioVoiceIndication : public BnRadioVoiceIndication {
public:
    RadioVoiceIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus callRing(RadioIndicationType type, bool isGsm,
                                        const CdmaSignalInfoRecord& record) override;

    virtual ndk::ScopedAStatus callStateChanged(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus cdmaCallWaiting(RadioIndicationType type,
                                               const CdmaCallWaiting& callWaitingRecord) override;

    virtual ndk::ScopedAStatus cdmaInfoRec(
            RadioIndicationType type, const std::vector<CdmaInformationRecord>& records) override;

    virtual ndk::ScopedAStatus cdmaOtaProvisionStatus(RadioIndicationType type,
                                                      CdmaOtaProvisionStatus status) override;

    virtual ndk::ScopedAStatus currentEmergencyNumberList(
            RadioIndicationType type,
            const std::vector<EmergencyNumber>& emergencyNumberList) override;

    virtual ndk::ScopedAStatus enterEmergencyCallbackMode(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus exitEmergencyCallbackMode(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus indicateRingbackTone(RadioIndicationType type, bool start) override;

    virtual ndk::ScopedAStatus onSupplementaryServiceIndication(
            RadioIndicationType type, const StkCcUnsolSsResult& ss) override;

    virtual ndk::ScopedAStatus onUssd(RadioIndicationType type, UssdModeType modeType,
                                      const std::string& msg) override;

    virtual ndk::ScopedAStatus resendIncallMute(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus srvccStateNotify(RadioIndicationType type,
                                                SrvccState state) override;

    virtual ndk::ScopedAStatus stkCallControlAlphaNotify(RadioIndicationType type,
                                                         const std::string& alpha) override;

    virtual ndk::ScopedAStatus stkCallSetup(RadioIndicationType type, int64_t timeout) override;
};

