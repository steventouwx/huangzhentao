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

#include "radio_messaging_indication.h"
#include <mtk_log.h>
#include "RpDataUtils.h"
#include <prop/properties.h>

#define LOG_TAG "RadioMessagingIndication"

RadioMessagingIndication::RadioMessagingIndication(int slot, RfxDispatchThread* dispatchThread) {
    mDispatchThread = dispatchThread;
    mSlot = slot;
}

ndk::ScopedAStatus RadioMessagingIndication::cdmaNewSms(RadioIndicationType /*type*/,
                                                        const CdmaSmsMessage& /*msg*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioMessagingIndication::cdmaRuimSmsStorageFull(RadioIndicationType /*type*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioMessagingIndication::newBroadcastSms(RadioIndicationType /*type*/,
                                                             const std::vector<uint8_t>& /*data*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioMessagingIndication::newSms(RadioIndicationType type, const std::vector<uint8_t>& pdu) {
    RLOGD("newSms: type = %d", type);
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_RESPONSE_NEW_SMS);
    int32_t support = property_get_int32("persist.vendor.sms_codec_support", 0);
    if (!support) {
      RLOGD("newSms: SMS codec don't supported");
      p.writeByteVector(pdu);
    } else {
        char HEX_CHARS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        std::string ret;
        for (int i = 0; i < pdu.size(); i++) {
        int b;
        b = 0x0f & (pdu[i] >> 4);
        ret.push_back(HEX_CHARS[b]);
        b = 0x0f & pdu[i];
        ret.push_back(HEX_CHARS[b]);
        }
        RpDataUtils::writeStringToParcel(&p, ret.c_str());
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_RESPONSE_NEW_SMS, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioMessagingIndication::newSmsOnSim(RadioIndicationType /*type*/,
                                                         int32_t /*recordNumber*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioMessagingIndication::newSmsStatusReport(
        RadioIndicationType /*type*/, const std::vector<uint8_t>& /*pdu*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioMessagingIndication::simSmsStorageFull(RadioIndicationType /*type*/) {
    return ndk::ScopedAStatus::ok();
}
