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

#include "radio_data_indication.h"
#include <cstdint>
#include <mtk_log.h>
#include "RpDataUtils.h"

RadioDataIndication::RadioDataIndication(int slot, RfxDispatchThread* dispatchThread) {
    mDispatchThread = dispatchThread;
    mSlot = slot;
}

std::string RadioDataIndication::convertPdpProtocolTypeToString(::aidl::android::hardware::radio::data::PdpProtocolType pdpType) {
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

std::string RadioDataIndication::convertVecLinkAddressToCharPtr(const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& records) {
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

std::string RadioDataIndication::convertVecAidlStringToCharPtr(const std::vector<std::string>& records) {
    std::ostringstream ss;

    for (size_t i = 0; i < records.size(); ++i) {
        if (i > 0) {
            ss << " ";
        }
        ss << records[i];
    }

    return ss.str();
}

ndk::ScopedAStatus RadioDataIndication::dataCallListChanged(
        RadioIndicationType type, const std::vector<SetupDataCallResult>& dcList) {
    RLOGD("dataCallListChanged: type = %d", type);
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_DATA_CALL_LIST_CHANGED);
    p.writeInt32(11); //version
    int32_t num = dcList.size();
    p.writeInt32(num); //number
    for (int32_t i = 0; i < num; i++) {
        RLOGD("RadioDataIndication dataCallListChanged: dcList[%d],cause=%d,suggestedRetryTime=%d,cid=%d,active=%d",
            i,dcList[i].cause,dcList[i].suggestedRetryTime,dcList[i].cid,dcList[i].active);
        RLOGD("type=%s,ifname=%s,addresses=%s,dnses=%s,gateways=%s,pcscf=%s",
            convertPdpProtocolTypeToString(dcList[i].type).c_str(),
            dcList[i].ifname.c_str(),
            convertVecLinkAddressToCharPtr(dcList[i].addresses).c_str(),
            convertVecAidlStringToCharPtr(dcList[i].dnses).c_str(),
            convertVecAidlStringToCharPtr(dcList[i].gateways).c_str(),
            convertVecAidlStringToCharPtr(dcList[i].pcscf).c_str());
        p.writeInt32((int32_t)(dcList[i].cause)); //DataCallFailCause
        p.writeInt32(dcList[i].suggestedRetryTime);
        p.writeInt32(dcList[i].cid);
        p.writeInt32(dcList[i].active);
        RpDataUtils::writeStringToParcel(&p,  convertPdpProtocolTypeToString(dcList[i].type).c_str());
        RpDataUtils::writeStringToParcel(&p,  dcList[i].ifname.c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecLinkAddressToCharPtr(dcList[i].addresses).c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcList[i].dnses).c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcList[i].gateways).c_str());
        RpDataUtils::writeStringToParcel(&p,  convertVecAidlStringToCharPtr(dcList[i].pcscf).c_str());
        p.writeInt32(dcList[i].mtuV4);
        p.writeInt32(dcList[i].mtuV6);
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_DATA_CALL_LIST_CHANGED, &p);
    return ndk::ScopedAStatus::ok();
}


ndk::ScopedAStatus RadioDataIndication::keepaliveStatus(RadioIndicationType type,
                                                        const KeepaliveStatus& status) {
    RLOGD("keepaliveStatus: type = %d, status=%s", type, status.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_KEEPALIVE_STATUS);
    p.writeInt32(status.sessionHandle);
    p.writeInt32(status.code);
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_KEEPALIVE_STATUS, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataIndication::pcoData(RadioIndicationType /*type*/,
                                                const PcoDataInfo& /*pco*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataIndication::unthrottleApn(RadioIndicationType /*type*/,
                                                      const DataProfileInfo& /*dataProfileInfo*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioDataIndication::slicingConfigChanged(
        RadioIndicationType /*type*/, const SlicingConfig& /*slicingConfig*/) {
    return ndk::ScopedAStatus::ok();
}
