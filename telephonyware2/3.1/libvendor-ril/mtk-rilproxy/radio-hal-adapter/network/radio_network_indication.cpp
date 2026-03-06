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

#include "radio_network_indication.h"
#include <aidl/android/hardware/radio/network/RegState.h>
#include <mtk_log.h>
#include "RpDataUtils.h"

#define LOG_TAG "RadioNetworkIndication"
// Constructor definition
RadioNetworkIndication::RadioNetworkIndication(int slot, RfxDispatchThread* dispatchThread) {
    mDispatchThread = dispatchThread;
    mSlot = slot;
}


ndk::ScopedAStatus RadioNetworkIndication::barringInfoChanged(
        RadioIndicationType /*type*/, const CellIdentity& /*cellIdentity*/,
        const std::vector<BarringInfo>& /*barringInfos*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::cdmaPrlChanged(RadioIndicationType /*type*/,
                                                          int32_t /*version*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::cellInfoList(RadioIndicationType type,
                                                        const std::vector<CellInfo>& cellInfo) {
    RLOGD("cellInfoList(%d): type = %s",mSlot, toString(type).c_str());
    int size = cellInfo.size();
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_CELL_INFO_LIST);
    android::status_t status = p.writeInt32(size);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    for (int32_t i = 0; i < size; ++i) {
        RLOGD("%s(%d), index %d, info: %s", __func__, mSlot, i, cellInfo[i].toString().c_str());
        bool registered = cellInfo[i].registered;
        p.writeBool(registered);
        int32_t cennectionStatus = static_cast<int32_t>(cellInfo[i].connectionStatus);
        p.writeInt32(cennectionStatus);
        ::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag tag = cellInfo[i].ratSpecificInfo.getTag();
        p.writeInt32((int32_t)tag);
        switch (tag) {
          case ::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::gsm: {
            auto& gsmData = cellInfo[i].ratSpecificInfo.get<::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::gsm>();
            RpDataUtils::writeStringToParcel(&p, gsmData.cellIdentityGsm.mcc.c_str());
            RpDataUtils::writeStringToParcel(&p, gsmData.cellIdentityGsm.mnc.c_str());
            p.writeInt32(gsmData.cellIdentityGsm.lac);
            p.writeInt32(gsmData.cellIdentityGsm.cid);
            p.writeInt32(gsmData.cellIdentityGsm.arfcn);
            p.writeInt32(gsmData.cellIdentityGsm.bsic);
            RpDataUtils::writeStringToParcel(&p, gsmData.cellIdentityGsm.operatorNames.alphaLong.c_str());
            RpDataUtils::writeStringToParcel(&p, gsmData.cellIdentityGsm.operatorNames.alphaShort.c_str());
            p.writeInt32(gsmData.signalStrengthGsm.signalStrength);
            p.writeInt32(gsmData.signalStrengthGsm.bitErrorRate);
            p.writeInt32(gsmData.signalStrengthGsm.timingAdvance);
            break;
          }
          case ::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::wcdma: {
            auto& wcdmaData = cellInfo[i].ratSpecificInfo.get<::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::wcdma>();
            RpDataUtils::writeStringToParcel(&p, wcdmaData.cellIdentityWcdma.mcc.c_str());
            RpDataUtils::writeStringToParcel(&p, wcdmaData.cellIdentityWcdma.mnc.c_str());
            p.writeInt32(wcdmaData.cellIdentityWcdma.lac);
            p.writeInt32(wcdmaData.cellIdentityWcdma.cid);
            p.writeInt32(wcdmaData.cellIdentityWcdma.psc);
            p.writeInt32(wcdmaData.cellIdentityWcdma.uarfcn);
            RpDataUtils::writeStringToParcel(&p, wcdmaData.cellIdentityWcdma.operatorNames.alphaLong.c_str());
            RpDataUtils::writeStringToParcel(&p, wcdmaData.cellIdentityWcdma.operatorNames.alphaShort.c_str());
            p.writeInt32(wcdmaData.signalStrengthWcdma.signalStrength);
            p.writeInt32(wcdmaData.signalStrengthWcdma.bitErrorRate);
            p.writeInt32(wcdmaData.signalStrengthWcdma.rscp);
            p.writeInt32(wcdmaData.signalStrengthWcdma.ecno);
            break;
          }
          case ::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::tdscdma: {
            RLOGE("don't support , error");
            break;
          }
          case ::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::cdma: {
            auto& cdmaData = cellInfo[i].ratSpecificInfo.get<::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::cdma>();
            p.writeInt32(cdmaData.cellIdentityCdma.networkId);
            p.writeInt32(cdmaData.cellIdentityCdma.systemId);
            p.writeInt32(cdmaData.cellIdentityCdma.baseStationId);
            p.writeInt32(cdmaData.cellIdentityCdma.longitude);
            p.writeInt32(cdmaData.cellIdentityCdma.latitude);
            RpDataUtils::writeStringToParcel(&p, cdmaData.cellIdentityCdma.operatorNames.alphaLong.c_str());
            RpDataUtils::writeStringToParcel(&p, cdmaData.cellIdentityCdma.operatorNames.alphaShort.c_str());
            p.writeInt32(cdmaData.signalStrengthCdma.dbm);
            p.writeInt32(cdmaData.signalStrengthCdma.ecio);
            p.writeInt32(cdmaData.signalStrengthEvdo.dbm);
            p.writeInt32(cdmaData.signalStrengthEvdo.ecio);
            p.writeInt32(cdmaData.signalStrengthEvdo.signalNoiseRatio);
            break;
          }
          case ::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::lte: {
            auto& lteData = cellInfo[i].ratSpecificInfo.get<::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::lte>();
            RpDataUtils::writeStringToParcel(&p, lteData.cellIdentityLte.mcc.c_str());
            RpDataUtils::writeStringToParcel(&p, lteData.cellIdentityLte.mnc.c_str());
            p.writeInt32(lteData.cellIdentityLte.ci);
            p.writeInt32(lteData.cellIdentityLte.pci);
            p.writeInt32(lteData.cellIdentityLte.tac);
            p.writeInt32(lteData.cellIdentityLte.earfcn);
            RpDataUtils::writeStringToParcel(&p, lteData.cellIdentityLte.operatorNames.alphaLong.c_str());
            RpDataUtils::writeStringToParcel(&p, lteData.cellIdentityLte.operatorNames.alphaShort.c_str());
            p.writeInt32(lteData.cellIdentityLte.bandwidth);
            int32_t size = lteData.cellIdentityLte.bands.size();
            p.writeInt32(size);
            if (size == 1) {
              p.writeInt32((int32_t)lteData.cellIdentityLte.bands[0]);
            }
            p.writeInt32(lteData.signalStrengthLte.signalStrength);
            p.writeInt32(lteData.signalStrengthLte.rsrp);
            p.writeInt32(lteData.signalStrengthLte.rsrq);
            p.writeInt32(lteData.signalStrengthLte.rssnr);
            p.writeInt32(lteData.signalStrengthLte.cqi);
            p.writeInt32(lteData.signalStrengthLte.timingAdvance);
            p.writeInt32(lteData.signalStrengthLte.cqiTableIndex);
            break;
          }
          case ::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::nr: {
            auto& nrData = cellInfo[i].ratSpecificInfo.get<::aidl::android::hardware::radio::network::CellInfoRatSpecificInfo::Tag::nr>();
            RpDataUtils::writeStringToParcel(&p, nrData.cellIdentityNr.mcc.c_str());
            RpDataUtils::writeStringToParcel(&p, nrData.cellIdentityNr.mnc.c_str());
            p.writeUint64(nrData.cellIdentityNr.nci);
            p.writeUint32(nrData.cellIdentityNr.pci);
            p.writeInt32(nrData.cellIdentityNr.tac);
            p.writeInt32(nrData.cellIdentityNr.nrarfcn);
            RpDataUtils::writeStringToParcel(&p, nrData.cellIdentityNr.operatorNames.alphaLong.c_str());
            RpDataUtils::writeStringToParcel(&p, nrData.cellIdentityNr.operatorNames.alphaShort.c_str());
            int32_t size = nrData.cellIdentityNr.bands.size();
            p.writeInt32(size);
            if (size == 1) {
              p.writeInt32((int32_t)nrData.cellIdentityNr.bands[0]);
            }
            p.writeInt32(nrData.signalStrengthNr.ssRsrp);
            p.writeInt32(nrData.signalStrengthNr.ssRsrq);
            p.writeInt32(nrData.signalStrengthNr.ssSinr);
            p.writeInt32(nrData.signalStrengthNr.csiRsrp);
            p.writeInt32(nrData.signalStrengthNr.csiRsrq);
            p.writeInt32(nrData.signalStrengthNr.csiSinr);
            p.writeInt32(nrData.signalStrengthNr.csiCqiTableIndex);
            size = nrData.signalStrengthNr.csiCqiReport.size();
            p.writeInt32(size);
            for (int i = 0; i < size; i++) {
              p.writeInt32(nrData.signalStrengthNr.csiCqiReport[i]);
            }
            p.writeInt32(nrData.signalStrengthNr.timingAdvance);
            break;
          }
        }
    }
    p.setDataPosition(0);

    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_CELL_INFO_LIST, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::currentLinkCapacityEstimate(
        RadioIndicationType /*type*/, const LinkCapacityEstimate& /*lce*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::currentPhysicalChannelConfigs(
        RadioIndicationType /*type*/, const std::vector<PhysicalChannelConfig>& /*configs*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::currentSignalStrength(
        RadioIndicationType type, const SignalStrength& signalInd) {
    RLOGD("currentSignalStrength: type = %s SignalStrength: %s ", toString(type).c_str(), signalInd.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_SIGNAL_STRENGTH);
    p.writeInt32(signalInd.gsm.signalStrength);
    p.writeInt32(signalInd.gsm.bitErrorRate);
    p.writeInt32(signalInd.gsm.timingAdvance);
    p.writeInt32(signalInd.cdma.dbm);
    p.writeInt32(signalInd.cdma.ecio);
    p.writeInt32(signalInd.evdo.dbm);
    p.writeInt32(signalInd.evdo.ecio);
    p.writeInt32(signalInd.evdo.signalNoiseRatio);
    p.writeInt32(signalInd.lte.signalStrength);
    p.writeInt32(signalInd.lte.rsrp);
    p.writeInt32(signalInd.lte.rsrq);
    p.writeInt32(signalInd.lte.rssnr);
    p.writeInt32(signalInd.lte.cqi);
    p.writeInt32(signalInd.lte.timingAdvance);
    p.writeInt32(signalInd.lte.cqiTableIndex);
    p.writeInt32(signalInd.tdscdma.signalStrength);
    p.writeInt32(signalInd.tdscdma.bitErrorRate);
    p.writeInt32(signalInd.tdscdma.rscp);
    p.writeInt32(signalInd.wcdma.signalStrength);
    p.writeInt32(signalInd.wcdma.bitErrorRate);
    p.writeInt32(signalInd.wcdma.rscp);
    p.writeInt32(signalInd.wcdma.ecno);
    p.writeInt32(signalInd.nr.ssRsrp);
    p.writeInt32(signalInd.nr.ssRsrq);
    p.writeInt32(signalInd.nr.ssSinr);
    p.writeInt32(signalInd.nr.csiRsrp);
    p.writeInt32(signalInd.nr.csiRsrq);
    p.writeInt32(signalInd.nr.csiSinr);
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_SIGNAL_STRENGTH, &p);
    return ndk::ScopedAStatus::ok();    return ndk::ScopedAStatus::ok();
}


ndk::ScopedAStatus RadioNetworkIndication::imsNetworkStateChanged(RadioIndicationType type) {
    RLOGD("%s(%d): type = %s", __func__, mSlot, toString(type).c_str());
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED);
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::networkScanResult(RadioIndicationType /*type*/,
                                                             const NetworkScanResult& /*result*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::networkStateChanged(RadioIndicationType type) {
    RLOGD("networkStateChanged: type = %d", type);
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED);
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::nitzTimeReceived(RadioIndicationType type,
                                                            const std::string& nitzTime,
                                                            int64_t receivedTime,
                                                            int64_t age) {
    RLOGD("nitzTimeReceived: type = %s, nitzTime = %s, receivedTime = %lld, age = %lld", toString(type).c_str(), nitzTime.c_str(), receivedTime, age);
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(type));
    if (status != android::OK) {
        RLOGE("writeInt32 type failed");
        return ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(RIL_UNSOL_NITZ_TIME_RECEIVED);
    if (status != android::OK) {
        RLOGE("writeInt32 RIL_UNSOL_NITZ_TIME_RECEIVED failed");
        return ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, nitzTime.c_str());
    status = p.writeInt64(receivedTime);
    if (status != android::OK) {
        RLOGE("writeInt64 receivedTime failed");
        return ndk::ScopedAStatus::ok();
    }
    status = p.writeInt64(age);
    if (status != android::OK) {
        RLOGE("writeInt64 age failed");
        return ndk::ScopedAStatus::ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_NITZ_TIME_RECEIVED, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::registrationFailed(RadioIndicationType /*type*/,
                                                              const CellIdentity& /*cellIdentity*/,
                                                              const std::string& /*chosenPlmn*/,
                                                              int32_t /*domain*/,
                                                              int32_t /*causeCode*/,
                                                              int32_t /*additionalCauseCode*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::restrictedStateChanged(RadioIndicationType /*type*/,
                                                                  PhoneRestrictedState /*state*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::suppSvcNotify(RadioIndicationType /*type*/,
                                                         const SuppSvcNotification& /*suppSvc*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::voiceRadioTechChanged(RadioIndicationType /*type*/,
                                                                 RadioTechnology /*rat*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::emergencyNetworkScanResult(
        RadioIndicationType /*type*/, const EmergencyRegResult& /*result*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::cellularIdentifierDisclosed(
        RadioIndicationType /*type*/,
        const CellularIdentifierDisclosure& /*disclosures*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkIndication::securityAlgorithmsUpdated(
        RadioIndicationType /*type*/, const SecurityAlgorithmUpdate& /*securityAlgorithmUpdate*/) {
    return ndk::ScopedAStatus::ok();
}
