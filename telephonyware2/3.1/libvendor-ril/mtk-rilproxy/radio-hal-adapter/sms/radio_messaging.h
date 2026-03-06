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

#include <memory>
#include <cstdint>

#include "radio_messaging_response.h"
#include "radio_messaging_indication.h"
#include "../radio_service.h"

using namespace aidl::android::hardware::radio::messaging;

// The main test class for Radio AIDL Messaging.
class RadioMessaging : public RadioService {
public:
     RadioMessaging(int slot, RfxDispatchThread* dispatchThread);
  public:

    /* radio messaging service handle */
    std::shared_ptr<IRadioMessaging> radio_messaging;
    /* radio messaging response handle */
    std::shared_ptr<RadioMessagingResponse> radioRsp_messaging;
    /* radio messaging indication handle */
    std::shared_ptr<RadioMessagingIndication> radioInd_messaging;
    int32_t sendSms(int32_t serial, const GsmSmsMessage& message);
    int32_t acknowledgeLastIncomingGsmSms(int32_t serial, bool success, SmsAcknowledgeFailCause cause);
};
