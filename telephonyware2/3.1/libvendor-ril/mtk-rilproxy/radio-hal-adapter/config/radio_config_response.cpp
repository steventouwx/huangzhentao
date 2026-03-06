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

#include <cstdint>
#include <mtk_log.h>
#include "RpDataUtils.h"
#include "radio_config_response.h"

#define LOG_TAG "RadioConfigResponse"

RadioConfigResponse::RadioConfigResponse(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus RadioConfigResponse::getHalDeviceCapabilitiesResponse(const RadioResponseInfo& info, bool /*in_modemReducedFeatureSet1*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioConfigResponse::getNumOfLiveModemsResponse(const RadioResponseInfo& info, int8_t /*in_numOfLiveModems*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioConfigResponse::getSimSlotsStatusResponse(const RadioResponseInfo& info,
    const std::vector<::aidl::android::hardware::radio::config::SimSlotStatus>& /*slotStatus*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioConfigResponse::setNumOfLiveModemsResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioConfigResponse::setPreferredDataModemResponse(const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("setPreferredDataModemResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioConfigResponse::setSimSlotsMappingResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioConfigResponse::getPhoneCapabilityResponse(const RadioResponseInfo& info,
    const ::aidl::android::hardware::radio::config::PhoneCapability& /*in_phoneCapability*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioConfigResponse::getSimultaneousCallingSupportResponse(const RadioResponseInfo& info,
    const std::vector<int32_t>& /*enabledLogicalSlots*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}