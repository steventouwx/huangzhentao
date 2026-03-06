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

#include <memory>
#include <cstdint>

#include "radio_voice_response.h"
#include "radio_voice_indication.h"
#include "../radio_service.h"

using namespace aidl::android::hardware::radio::voice;

// The main test class for Radio AIDL Voice.
class RadioVoice : public RadioService {

public:
     RadioVoice(int slot, RfxDispatchThread* dispatchThread);
  public:
    /* radio voice service handle */
    std::shared_ptr<IRadioVoice> radio_voice;
    /* radio voice response handle */
    std::shared_ptr<RadioVoiceResponse> radioRsp_voice;
    /* radio voice indication handle */
    std::shared_ptr<RadioVoiceIndication> radioInd_voice;

    int32_t dial(int32_t serial, const Dial& dialInfo);
    int32_t acceptCall(int32_t serial);
    int32_t getCurrentCalls(int32_t serial);
    int32_t startDtmf(int32_t serial, const std::string& phonenum);
    int32_t stopDtmf(int32_t serial);
    int32_t hangup(int32_t serial, int32_t gsmIndex);
    int32_t isVoNrEnabled(int32_t serial);
    int32_t setVoNrEnabled(int32_t serial, bool enable);
    int32_t setCallWaiting(int32_t serial, bool enable, int32_t serviceClass);
    int32_t getCallWaiting(int32_t serial, int32_t serviceClass);
    int32_t getLastCallFailCause(int32_t serial);
};
