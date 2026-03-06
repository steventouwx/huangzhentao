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
#include "radio_sim_response.h"

#define LOG_TAG "RadioSimResponse"

RadioSimResponse::RadioSimResponse(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus RadioSimResponse::acknowledgeRequest(int32_t /*serial*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::areUiccApplicationsEnabledResponse(
        const RadioResponseInfo& info, bool enabled) {
    rspInfo = info;
    areUiccApplicationsEnabled = enabled;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::changeIccPin2ForAppResponse(const RadioResponseInfo& info,
                                                                 int32_t remainingRetries) {
    rspInfo = info;
    RLOGD("changeIccPin2ForAppResponse(%d): response: info = %s remainingRetries = %d", mSlot, info.toString().c_str(), remainingRetries);
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
    status = p.writeInt32((int32_t)(remainingRetries));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::changeIccPinForAppResponse(const RadioResponseInfo& info,
                                                                int32_t remainingRetries) {
    rspInfo = info;
    RLOGD("changeIccPinForAppResponse(%d): response: info = %s remainingRetries = %d", mSlot, info.toString().c_str(), remainingRetries);
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
    status = p.writeInt32((int32_t)(remainingRetries));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::enableUiccApplicationsResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getAllowedCarriersResponse(
        const RadioResponseInfo& info, const CarrierRestrictions& carriers,
        SimLockMultiSimPolicy multiSimPolicy) {
    rspInfo = info;
    carrierRestrictionsResp = carriers;
    multiSimPolicyResp = multiSimPolicy;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getCdmaSubscriptionResponse(
        const RadioResponseInfo& info, const std::string& /*mdn*/, const std::string& /*hSid*/,
        const std::string& /*hNid*/, const std::string& /*min*/, const std::string& /*prl*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getCdmaSubscriptionSourceResponse(
        const RadioResponseInfo& info, CdmaSubscriptionSource /*source*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getFacilityLockForAppResponse(const RadioResponseInfo& info,
                                                                   int32_t /*response*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getIccCardStatusResponse(const RadioResponseInfo& info,
                                                              const CardStatus& card_status) {
    RLOGD("getImsiForAppResponse(%d): response: info = %s, CardStatus = %s", mSlot, info.toString().c_str(), card_status.toString().c_str());
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
    p.writeInt32(card_status.cardState);
    p.writeInt32((int32_t)(card_status.universalPinState));
    p.writeInt32(card_status.gsmUmtsSubscriptionAppIndex);
    p.writeInt32(card_status.cdmaSubscriptionAppIndex);
    p.writeInt32(card_status.imsSubscriptionAppIndex);

    int32_t num_apps = card_status.applications.size();
    p.writeInt32(num_apps);
    for (int i = 0; i < num_apps; i++) {
        p.writeInt32((card_status.applications)[i].appType);
        p.writeInt32((card_status.applications)[i].appState);
        p.writeInt32((int32_t)((card_status.applications)[i].persoSubstate));
        RpDataUtils::writeStringToParcel(&p, ((card_status.applications)[i].aidPtr.c_str()));
        RpDataUtils::writeStringToParcel(&p, ((card_status.applications)[i].appLabelPtr.c_str()));
        p.writeInt32((card_status.applications)[i].pin1Replaced);
        p.writeInt32((int32_t)((card_status.applications)[i].pin1));
        p.writeInt32((int32_t)((card_status.applications)[i].pin2));
    }

    p.writeInt32(card_status.slotMap.physicalSlotId);
    RpDataUtils::writeStringToParcel(&p, card_status.atr.c_str());
    RpDataUtils::writeStringToParcel(&p, card_status.iccid.c_str());

    RpDataUtils::writeStringToParcel(&p, card_status.eid.c_str());
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getImsiForAppResponse(const RadioResponseInfo& info,
                                                           const std::string& imsi_str) {
    RLOGD("getImsiForAppResponse(%d): response: info = %s, imsi = %s",mSlot, info.toString().c_str(), imsi_str.c_str());
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
    RpDataUtils::writeStringToParcel(&p, imsi_str.c_str());
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getSimPhonebookCapacityResponse(
        const RadioResponseInfo& info, const PhonebookCapacity& pbCapacity) {
    rspInfo = info;
    capacity = pbCapacity;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::getSimPhonebookRecordsResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::iccCloseLogicalChannelResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::iccCloseLogicalChannelWithSessionInfoResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::iccIoForAppResponse(const RadioResponseInfo& info,
                                                         const IccIoResult& iccIo) {
RLOGD("iccIoForAppResponse(%d): response: info = %s, sw1 = %d , sw2 = %d , simResponse = %s .",mSlot, info.toString().c_str(), iccIo.sw1, iccIo.sw2, iccIo.simResponse.c_str());
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
    status = p.writeInt32((int32_t)(3));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(iccIo.sw1));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(iccIo.sw2));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    RpDataUtils::writeStringToParcel(&p, iccIo.simResponse.c_str());
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::iccOpenLogicalChannelResponse(
        const RadioResponseInfo& info, int32_t /*channelId*/,
        const std::vector<uint8_t>& /*selectResponse*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::iccTransmitApduBasicChannelResponse(
        const RadioResponseInfo& info, const IccIoResult& /*result*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::iccTransmitApduLogicalChannelResponse(
        const RadioResponseInfo& info, const IccIoResult& /*result*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::reportStkServiceIsRunningResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::requestIccSimAuthenticationResponse(
        const RadioResponseInfo& info, const IccIoResult& /*result*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::sendEnvelopeResponse(const RadioResponseInfo& info,
                                                          const std::string& /*commandResponse*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::sendEnvelopeWithStatusResponse(const RadioResponseInfo& info,
                                                                    const IccIoResult& /*iccIo*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::sendTerminalResponseToSimResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::setAllowedCarriersResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::setCarrierInfoForImsiEncryptionResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::setCdmaSubscriptionSourceResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::setFacilityLockForAppResponse(const RadioResponseInfo& info,
                                                                   int32_t retry) {
    rspInfo = info;
    RLOGD("setFacilityLockForAppResponse(%d): response: info = %s retry = %d", mSlot, info.toString().c_str(), retry);
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
    status = p.writeInt32((int32_t)(retry));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::setSimCardPowerResponse(const RadioResponseInfo& info) {
    RLOGD("setSimCardPowerResponse(%d): response: info = %s", mSlot, info.toString().c_str());
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
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioSimResponse::setUiccSubscriptionResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::supplyIccPin2ForAppResponse(const RadioResponseInfo& info,
                                                                 int32_t remainingRetries) {
    rspInfo = info;
    RLOGD("supplyIccPin2ForAppResponse(%d): response: info = %s remainingRetries = %d", mSlot, info.toString().c_str(), remainingRetries);
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
    status = p.writeInt32((int32_t)(remainingRetries));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::supplyIccPinForAppResponse(const RadioResponseInfo& info,
                                                                int32_t remainingRetries) {
    rspInfo = info;
    RLOGD("supplyIccPinForAppResponse(%d): response: info = %s remainingRetries = %d", mSlot, info.toString().c_str(), remainingRetries);
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
    status = p.writeInt32((int32_t)(remainingRetries));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::supplyIccPuk2ForAppResponse(const RadioResponseInfo& info,
                                                                 int32_t remainingRetries) {
    rspInfo = info;
    RLOGD("supplyIccPinForAppResponse(%d): response: info = %s remainingRetries = %d", mSlot, info.toString().c_str(), remainingRetries);
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
    status = p.writeInt32((int32_t)(remainingRetries));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::supplyIccPukForAppResponse(const RadioResponseInfo& info,
                                                                int32_t remainingRetries) {
    rspInfo = info;
    RLOGD("supplyIccPinForAppResponse(%d): response: info = %s remainingRetries = %d", mSlot, info.toString().c_str(), remainingRetries);
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
    status = p.writeInt32((int32_t)(remainingRetries));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::supplySimDepersonalizationResponse(
        const RadioResponseInfo& info, PersoSubstate /*persoType*/, int32_t /*remainingRetries*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioSimResponse::updateSimPhonebookRecordsResponse(
        const RadioResponseInfo& info, int32_t recordIndex) {
    rspInfo = info;
    updatedRecordIndex = recordIndex;

    return ndk::ScopedAStatus::ok();
}
