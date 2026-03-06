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
#include "../radio_service.h"
using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::messaging;


/* Callback class for radio messaging response */
class RadioMessagingResponse : public BnRadioMessagingResponse {
public:
    RadioMessagingResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

public:


    RadioResponseInfo rspInfo;
    SendSmsResult sendSmsResult;

    virtual ndk::ScopedAStatus acknowledgeIncomingGsmSmsWithPduResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus acknowledgeLastIncomingCdmaSmsResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus acknowledgeLastIncomingGsmSmsResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus acknowledgeRequest(int32_t serial) override;

    virtual ndk::ScopedAStatus deleteSmsOnRuimResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus deleteSmsOnSimResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus getCdmaBroadcastConfigResponse(
            const RadioResponseInfo& info,
            const std::vector<CdmaBroadcastSmsConfigInfo>& configs) override;

    virtual ndk::ScopedAStatus getGsmBroadcastConfigResponse(
            const RadioResponseInfo& info,
            const std::vector<GsmBroadcastSmsConfigInfo>& configs) override;

    virtual ndk::ScopedAStatus getSmscAddressResponse(const RadioResponseInfo& info,
                                                      const std::string& smsc) override;

    virtual ndk::ScopedAStatus reportSmsMemoryStatusResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus sendCdmaSmsExpectMoreResponse(const RadioResponseInfo& info,
                                                             const SendSmsResult& sms) override;

    virtual ndk::ScopedAStatus sendCdmaSmsResponse(const RadioResponseInfo& info,
                                                   const SendSmsResult& sms) override;

    virtual ndk::ScopedAStatus sendImsSmsResponse(const RadioResponseInfo& info,
                                                  const SendSmsResult& sms) override;

    virtual ndk::ScopedAStatus sendSmsExpectMoreResponse(const RadioResponseInfo& info,
                                                         const SendSmsResult& sms) override;

    virtual ndk::ScopedAStatus sendSmsResponse(const RadioResponseInfo& info,
                                               const SendSmsResult& sms) override;

    virtual ndk::ScopedAStatus setCdmaBroadcastActivationResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setCdmaBroadcastConfigResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setGsmBroadcastActivationResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setGsmBroadcastConfigResponse(
            const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus setSmscAddressResponse(const RadioResponseInfo& info) override;

    virtual ndk::ScopedAStatus writeSmsToRuimResponse(const RadioResponseInfo& info,
                                                      int32_t index) override;

    virtual ndk::ScopedAStatus writeSmsToSimResponse(const RadioResponseInfo& info,
                                                     int32_t index) override;
};

