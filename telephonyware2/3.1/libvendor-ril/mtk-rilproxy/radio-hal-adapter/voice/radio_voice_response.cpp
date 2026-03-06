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
#include "radio_voice_response.h"

#define LOG_TAG "RadioVoiceResponse"

RadioVoiceResponse::RadioVoiceResponse(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus RadioVoiceResponse::acceptCallResponse(const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("dialResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::acknowledgeRequest(int32_t /*serial*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::cancelPendingUssdResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::conferenceResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::dialResponse(const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("dialResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::emergencyDialResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::exitEmergencyCallbackModeResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::explicitCallTransferResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getCallForwardStatusResponse(
        const RadioResponseInfo& info, const std::vector<CallForwardInfo>& /*callForwardInfos*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getCallWaitingResponse(const RadioResponseInfo& info,
                                                              bool enable,
                                                              int32_t serviceClass) {
    rspInfo = info;
    RLOGD("dialResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeBool(enable);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(serviceClass);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getClipResponse(const RadioResponseInfo& info,
                                                       ClipStatus /*status*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getClirResponse(const RadioResponseInfo& info, int32_t /*n*/,
                                                       int32_t /*m*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getCurrentCallsResponse(const RadioResponseInfo& info,
                                                               const std::vector<Call>& calls) {
    rspInfo = info;
    currentCalls = calls;
    RLOGD("getCurrentCallsResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    int32_t num = calls.size();
    status = p.writeInt32(num);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    for (int32_t i = 0; i < num; i++) {
        p.writeInt32(calls[i].state);
        p.writeInt32(calls[i].index);
        p.writeInt32(calls[i].toa);
        p.writeInt32(calls[i].isMpty);
        p.writeInt32(calls[i].isMT);
        p.writeInt32(calls[i].als);
        p.writeInt32(calls[i].isVoice);
        p.writeInt32(calls[i].isVoicePrivacy);
        RpDataUtils::writeStringToParcel(&p, calls[i].number.c_str());
        p.writeInt32(calls[i].numberPresentation);
        RpDataUtils::writeStringToParcel(&p, calls[i].name.c_str());
        p.writeInt32(calls[i].namePresentation);
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getLastCallFailCauseResponse(
        const RadioResponseInfo& info, const LastCallFailCauseInfo& failCauseInfo) {
    rspInfo = info;
    RLOGD("getLastCallFailCauseResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(static_cast<int>(failCauseInfo.causeCode));
    RLOGE("LastCallFailCauseInfo code = %d",static_cast<int>(failCauseInfo.causeCode));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getMuteResponse(const RadioResponseInfo& info,
                                                       bool /*enable*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getPreferredVoicePrivacyResponse(
        const RadioResponseInfo& info, bool /*enable*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::getTtyModeResponse(const RadioResponseInfo& info,
                                                          TtyMode /*mode*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::handleStkCallSetupRequestFromSimResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::hangupConnectionResponse(const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("hangupConnectionResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::hangupForegroundResumeBackgroundResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::hangupWaitingOrBackgroundResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::isVoNrEnabledResponse(const RadioResponseInfo& info, bool enabled) {
    RLOGD("isVoNrEnabledResponse(%d): response: info = %s, enabled = %d", mSlot, info.toString().c_str(), enabled);
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeBool(enabled);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::rejectCallResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::sendBurstDtmfResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::sendCdmaFeatureCodeResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::sendDtmfResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::sendUssdResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::separateConnectionResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::setCallForwardResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::setCallWaitingResponse(const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("setCallWaitingResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::setClirResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::setMuteResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::setPreferredVoicePrivacyResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::setTtyModeResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::setVoNrEnabledResponse(const RadioResponseInfo& info) {
    RLOGD("setVoNrEnabledResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::startDtmfResponse(const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("dialResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::stopDtmfResponse(const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("dialResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioVoiceResponse::switchWaitingOrHoldingAndActiveResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}
