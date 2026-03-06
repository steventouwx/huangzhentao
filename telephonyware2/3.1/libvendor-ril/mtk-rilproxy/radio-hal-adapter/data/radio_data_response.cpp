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
#include <string>
#include "RpDataUtils.h"
#include "radio_data_response.h"
#define LOG_TAG "RadioDataResponse"

RadioDataResponse::RadioDataResponse(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus RadioDataResponse::acknowledgeRequest(int32_t /*serial*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::allocatePduSessionIdResponse(const RadioResponseInfo& info,
                                                                   int32_t id) {
    rspInfo = info;
    allocatedPduSessionId = id;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::cancelHandoverResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::deactivateDataCallResponse(const RadioResponseInfo& info) {
    RLOGD("deactivateDataCallResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::getDataCallListResponse(
        const RadioResponseInfo& info, const std::vector<SetupDataCallResult>& dcResponse) {
    RLOGD("getDataCallListResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.writeInt32(11); //version
    int32_t num = dcResponse.size();
    p.writeInt32(num); //number
    for (int32_t i = 0; i < num; i++) {
        p.writeInt32((int32_t)(dcResponse[i].cause)); //DataCallFailCause
        p.writeInt32(dcResponse[i].suggestedRetryTime);
        p.writeInt32(dcResponse[i].cid);
        p.writeInt32(dcResponse[i].active);
        RpDataUtils::writeStringToParcel(&p,  convertPdpProtocolTypeToString(dcResponse[i].type).c_str());
        RpDataUtils::writeStringToParcel(&p,  dcResponse[i].ifname.c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecLinkAddressToCharPtr(dcResponse[i].addresses).c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcResponse[i].dnses).c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcResponse[i].gateways).c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcResponse[i].pcscf).c_str());
        p.writeInt32(dcResponse[i].mtuV4);
        p.writeInt32(dcResponse[i].mtuV6);
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioDataResponse::getSlicingConfigResponse(
        const RadioResponseInfo& info, const SlicingConfig& /*slicingConfig*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::releasePduSessionIdResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::setDataAllowedResponse(const RadioResponseInfo& info) {
    RLOGD("setDataAllowedResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::setDataProfileResponse(const RadioResponseInfo& info) {
    RLOGD("setDataProfileResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::setDataThrottlingResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::setInitialAttachApnResponse(const RadioResponseInfo& info) {
    RLOGD("setInitialAttachApnResponse(%d): response: info = %s",mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

std::string RadioDataResponse::convertPdpProtocolTypeToString(::aidl::android::hardware::radio::data::PdpProtocolType pdpType) {
    switch (pdpType) {
        case ::aidl::android::hardware::radio::data::PdpProtocolType::IP:
            return "IP";
        case ::aidl::android::hardware::radio::data::PdpProtocolType::IPV6:
            return "IPV6";
        case ::aidl::android::hardware::radio::data::PdpProtocolType::IPV4V6:
            return "IPV4V6";
        case ::aidl::android::hardware::radio::data::PdpProtocolType::PPP:
            return "PPP";
        default:
            return "IP";
    }
}

std::string RadioDataResponse::convertVecLinkAddressToCharPtr(const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& records) {
    std::ostringstream ss;

    for (size_t i = 0; i < records.size(); ++i) {
        if (i > 0) {
            ss << " ";
        }

        ss << records[i].address;

        // ss << "&" << records[i].addressProperties;
        // ss << "&" << records[i].deprecationTime;
        // ss << "&" << records[i].expirationTime;
    }

    return ss.str();
}

std::string RadioDataResponse::convertVecAidlStringToCharPtr(const std::vector<std::string>& records) {
    std::ostringstream ss;

    for (size_t i = 0; i < records.size(); ++i) {
        if (i > 0) {
            ss << " ";
        }
        ss << records[i];
    }

    return ss.str();
}


ndk::ScopedAStatus RadioDataResponse::setupDataCallResponse(const RadioResponseInfo& info,
                                                            const SetupDataCallResult& dcResponse) {
    RLOGD("setupDataCallResponse(%d): response: info = %s, dcResponse = %s", mSlot, info.toString().c_str(), dcResponse.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.writeInt32(11); //version
    p.writeInt32(1); //number
    p.writeInt32((int32_t)(dcResponse.cause)); //DataCallFailCause
    p.writeInt32(dcResponse.suggestedRetryTime);
    p.writeInt32(dcResponse.cid);
    p.writeInt32(dcResponse.active);
    RpDataUtils::writeStringToParcel(&p,  convertPdpProtocolTypeToString(dcResponse.type).c_str());
    RpDataUtils::writeStringToParcel(&p,  dcResponse.ifname.c_str());
    RpDataUtils::writeStringToParcel(&p,  convertVecLinkAddressToCharPtr(dcResponse.addresses).c_str());
    RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcResponse.dnses).c_str());
    RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcResponse.gateways).c_str());
    RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcResponse.pcscf).c_str());
    p.writeInt32(dcResponse.mtuV4);
    p.writeInt32(dcResponse.mtuV6);
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioDataResponse::startHandoverResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::startKeepaliveResponse(const RadioResponseInfo& info,
                                                             const KeepaliveStatus& status) {
    RLOGD("startKeepaliveResponse(%d): response: info = %s, status=%s",mSlot, info.toString().c_str(), status.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.writeInt32(status.sessionHandle);
    p.writeInt32(status.code);
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataResponse::stopKeepaliveResponse(const RadioResponseInfo& info) {
    RLOGD("stopKeepaliveResponse(%d): response: info = %s",mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}
