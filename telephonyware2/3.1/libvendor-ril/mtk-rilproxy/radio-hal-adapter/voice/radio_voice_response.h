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
#include <aidl/android/hardware/radio/RadioResponseInfo.h>


#include "RfxDispatchThread.h"
#include "../radio_service.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::voice;


/* Callback class for radio voice response */
class RadioVoiceResponse : public BnRadioVoiceResponse {

public:
    RadioVoiceResponse(int slot, RfxDispatchThread* dispatchThread);
private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

public:

    RadioResponseInfo rspInfo;
    std::vector<Call> currentCalls;

    virtual ndk::ScopedAStatus acceptCallResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus acknowledgeRequest(int32_t serial) override;

    virtual ndk::ScopedAStatus cancelPendingUssdResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus conferenceResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus dialResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus emergencyDialResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus exitEmergencyCallbackModeResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus explicitCallTransferResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus getCallForwardStatusResponse(
            const RadioResponseInfo& info,
            const std::vector<CallForwardInfo>& call_forwardInfos) override;

    virtual ndk::ScopedAStatus getCallWaitingResponse(const RadioResponseInfo& info, bool enable,
                                                      int32_t serviceClass) override;

    virtual ndk::ScopedAStatus getClipResponse(const RadioResponseInfo& info,
                                               ClipStatus status) override;

    virtual ndk::ScopedAStatus getClirResponse(const RadioResponseInfo& info, int32_t n,
                                               int32_t m) override;

    virtual ndk::ScopedAStatus getCurrentCallsResponse(const RadioResponseInfo& info,
                                                       const std::vector<Call>& calls) override;

    virtual ndk::ScopedAStatus getLastCallFailCauseResponse(
            const RadioResponseInfo& info, const LastCallFailCauseInfo& failCauseInfo) override;

    virtual ndk::ScopedAStatus getMuteResponse(const RadioResponseInfo& info, bool enable) override;

    virtual ndk::ScopedAStatus getPreferredVoicePrivacyResponse(const RadioResponseInfo& info,
                                                                bool enable) override;

    virtual ndk::ScopedAStatus getTtyModeResponse(const RadioResponseInfo& info,
                                                  TtyMode mode) override;

    virtual ndk::ScopedAStatus handleStkCallSetupRequestFromSimResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus hangupConnectionResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus hangupForegroundResumeBackgroundResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus hangupWaitingOrBackgroundResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus isVoNrEnabledResponse(const RadioResponseInfo& info,
                                                     bool enable) override;

    virtual ndk::ScopedAStatus rejectCallResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus sendBurstDtmfResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus sendCdmaFeatureCodeResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus sendDtmfResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus sendUssdResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus separateConnectionResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCallForwardResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCallWaitingResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setClirResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setMuteResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setPreferredVoicePrivacyResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setTtyModeResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setVoNrEnabledResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus startDtmfResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus stopDtmfResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus switchWaitingOrHoldingAndActiveResponse(
            const RadioResponseInfo& info) override;
};
