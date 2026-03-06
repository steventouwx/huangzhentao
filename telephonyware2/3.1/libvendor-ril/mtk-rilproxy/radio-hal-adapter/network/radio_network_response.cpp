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
#include <stdint.h>
#include <string>
#include "RpDataUtils.h"
#include "binder/IBinder.h"
#include "utils/Errors.h"
#include "radio_network_response.h"

#define LOG_TAG "RadioNetworkResponse"

// Private constructor definition (empty if nothing specific during construction)
RadioNetworkResponse::RadioNetworkResponse(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus RadioNetworkResponse::acknowledgeRequest(int32_t /*serial*/) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getAllowedNetworkTypesBitmapResponse(
        const RadioResponseInfo& info, const int32_t networkTypeBitmap) {
    RLOGD("getAllowedNetworkTypesBitmapResponse(%d): response: info = %s,  networkTypeBitmap = %d",
        mSlot, info.toString().c_str(), networkTypeBitmap);
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(info.type));
    if(status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(info.serial);
    if(status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32((int32_t)(info.error));
    if(status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(networkTypeBitmap);
    if(status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getAvailableBandModesResponse(
        const RadioResponseInfo& info, const std::vector<RadioBandMode>& bandModes) {
    rspInfo = info;
    radioBandModes = bandModes;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getAvailableNetworksResponse(
        const RadioResponseInfo& info, const std::vector<OperatorInfo>& operatorInfos) {
    rspInfo = info;
    networkInfos = operatorInfos;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getBarringInfoResponse(
        const RadioResponseInfo& info, const CellIdentity& cellIdentity,
        const std::vector<BarringInfo>& barringInfos) {
    rspInfo = info;
    barringCellIdentity = cellIdentity;
    barringInfoList = barringInfos;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getCdmaRoamingPreferenceResponse(
        const RadioResponseInfo& info, CdmaRoamingType /*type*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getCellInfoListResponse(const RadioResponseInfo& info, const std::vector<CellInfo>& cellInfo) {
    RLOGD("getCellInfoListResponse(%d): response: info = %s, cellInfo size= %zu", mSlot, info.toString().c_str(), cellInfo.size());
    int size = cellInfo.size();
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
    status = p.writeInt32(size);
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
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getDataRegistrationStateResponse(
        const RadioResponseInfo& info, const RegStateResult& regResponse) {
    RLOGD("getDataRegistrationStateResponse(%d): response: info = %s, regResponse = %s", mSlot, info.toString().c_str(), regResponse.toString().c_str());
    handleRegStateResult(info, regResponse);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getImsRegistrationStateResponse(
        const RadioResponseInfo& info, bool isRegistered, RadioTechnologyFamily ratFamily) {
  RLOGD("getImsRegistrationStateResponse(%d): response: info = %s, isRegistered = %d, ratFamily = %d",
        mSlot, info.toString().c_str(), isRegistered, ratFamily);
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
  status = p.writeInt32(2);
  if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
  }
  status = p.writeInt32(isRegistered ? 1 : 0);
  if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
  }
  status = p.writeInt32((int32_t)(ratFamily));
  if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
  }
  p.setDataPosition(0);
  mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot,
                                          info.serial, (int)info.error, &p);
  return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getNetworkSelectionModeResponse(
        const RadioResponseInfo& info, bool manual) {
  RLOGD("%s(%d): response: info = %s, manual = %d",__func__, mSlot, info.toString().c_str(), manual);
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
  p.writeBool(manual);
  p.setDataPosition(0);
  mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot,
                                          info.serial, (int)info.error, &p);
  return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getOperatorResponse(const RadioResponseInfo& info,
                                                             const std::string& longName,
                                                             const std::string& shortName,
                                                             const std::string& numeric) {
  RLOGD("getOperatorResponse(%d): response: info = %s, longName = %s, shortName = %s, numeric = %s",
        mSlot, info.toString().c_str(), longName.c_str(), shortName.c_str(), numeric.c_str());
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
  RpDataUtils::writeStringToParcel(&p, longName.c_str());
  RpDataUtils::writeStringToParcel(&p, shortName.c_str());
  RpDataUtils::writeStringToParcel(&p, numeric.c_str());
  p.setDataPosition(0);
  mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot,
                                          info.serial, (int)info.error, &p);
  return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getSignalStrengthResponse(
        const RadioResponseInfo& info, const SignalStrength& sigResponse/*sig_strength*/) {
    RLOGD("getSignalStrengthResponse(%d): response: info = %s, regResponse = %s", mSlot, info.toString().c_str(), sigResponse.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    // auto& gsmData = regResponse.cellIdentity.get<::aidl::android::hardware::radio::network::CellIdentity::Tag::gsm>();
    // auto& GW_SignalStrength = sigResponse.signalStrength.get<::aidl::android::hardware::radio::network::SignalStrength::Tag::gsm>();
    p.writeInt32(sigResponse.gsm.signalStrength);
    p.writeInt32(sigResponse.gsm.bitErrorRate);
    p.writeInt32(sigResponse.gsm.timingAdvance);
    p.writeInt32(sigResponse.cdma.dbm);
    p.writeInt32(sigResponse.cdma.ecio);
    p.writeInt32(sigResponse.evdo.dbm);
    p.writeInt32(sigResponse.evdo.ecio);
    p.writeInt32(sigResponse.evdo.signalNoiseRatio);
    p.writeInt32(sigResponse.lte.signalStrength);
    p.writeInt32(sigResponse.lte.rsrp);
    p.writeInt32(sigResponse.lte.rsrq);
    p.writeInt32(sigResponse.lte.rssnr);
    p.writeInt32(sigResponse.lte.cqi);
    p.writeInt32(sigResponse.lte.timingAdvance);
    p.writeInt32(sigResponse.lte.cqiTableIndex);
    p.writeInt32(sigResponse.tdscdma.signalStrength);
    p.writeInt32(sigResponse.tdscdma.bitErrorRate);
    p.writeInt32(sigResponse.tdscdma.rscp);
    p.writeInt32(sigResponse.wcdma.signalStrength);
    p.writeInt32(sigResponse.wcdma.bitErrorRate);
    p.writeInt32(sigResponse.wcdma.rscp);
    p.writeInt32(sigResponse.wcdma.ecno);
    p.writeInt32(sigResponse.nr.ssRsrp);
    p.writeInt32(sigResponse.nr.ssRsrq);
    p.writeInt32(sigResponse.nr.ssSinr);
    p.writeInt32(sigResponse.nr.csiRsrp);
    p.writeInt32(sigResponse.nr.csiRsrq);
    p.writeInt32(sigResponse.nr.csiSinr);
    p.writeInt32(sigResponse.nr.csiCqiTableIndex);
    p.writeInt32(sigResponse.nr.timingAdvance);

  mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot,
                                          info.serial, (int)info.error, &p);
  return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getSystemSelectionChannelsResponse(
        const RadioResponseInfo& info, const std::vector<RadioAccessSpecifier>& specifiers) {
    rspInfo = info;
    this->specifiers = specifiers;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getUsageSettingResponse(const RadioResponseInfo& info,
                                                                 const UsageSetting usageSetting) {
    RLOGD("getUsageSettingResponse(%d): response: info = %s, usageSetting: %d", mSlot, info.toString().c_str(), toString(usageSetting).c_str());
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
    p.writeInt32((int32_t) usageSetting);
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::getVoiceRadioTechnologyResponse(
        const RadioResponseInfo& info, RadioTechnology /*rat*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

void RadioNetworkResponse::handleRegStateResult(const RadioResponseInfo& info, const RegStateResult& regResponse) {
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.writeInt32((int32_t)(regResponse.regState));
    p.writeInt32((int32_t)(regResponse.rat));
    p.writeInt32((int32_t)(regResponse.reasonForDenial));
    RpDataUtils::writeStringToParcel(&p, regResponse.registeredPlmn.c_str());
    ::aidl::android::hardware::radio::network::CellIdentity::Tag tag = regResponse.cellIdentity.getTag();
    p.writeInt32((int32_t)tag);
    switch (tag) {
        case ::aidl::android::hardware::radio::network::CellIdentity::Tag::gsm: {
          auto& gsmData = regResponse.cellIdentity.get<::aidl::android::hardware::radio::network::CellIdentity::Tag::gsm>();
          //p.writeInt32(std::stoi(gsmData.mcc));
          //p.writeInt32(std::stoi(gsmData.mnc));
          //p.writeInt32(gsmData.mnc.length());
          RpDataUtils::writeStringToParcel(&p, gsmData.mcc.c_str());
          RpDataUtils::writeStringToParcel(&p, gsmData.mnc.c_str());
          p.writeInt32(gsmData.lac);
          p.writeInt32(gsmData.cid);
          p.writeInt32(gsmData.arfcn);
          p.writeInt32(gsmData.bsic);
          RpDataUtils::writeStringToParcel(&p, gsmData.operatorNames.alphaLong.c_str());
          RpDataUtils::writeStringToParcel(&p, gsmData.operatorNames.alphaShort.c_str());
          break;
        }
        case ::aidl::android::hardware::radio::network::CellIdentity::Tag::wcdma: {
          auto& wcdmaData = regResponse.cellIdentity.get<::aidl::android::hardware::radio::network::CellIdentity::Tag::wcdma>();
        //   p.writeInt32(std::stoi(wcdmaData.mcc));
        //   p.writeInt32(std::stoi(wcdmaData.mnc));
        //   p.writeInt32(wcdmaData.mnc.length());
          RpDataUtils::writeStringToParcel(&p, wcdmaData.mcc.c_str());
          RpDataUtils::writeStringToParcel(&p, wcdmaData.mnc.c_str());
          p.writeInt32(wcdmaData.lac);
          p.writeInt32(wcdmaData.cid);
          p.writeInt32(wcdmaData.psc);
          p.writeInt32(wcdmaData.uarfcn);
          RpDataUtils::writeStringToParcel(&p, wcdmaData.operatorNames.alphaLong.c_str());
          RpDataUtils::writeStringToParcel(&p, wcdmaData.operatorNames.alphaShort.c_str());
          break;
        }
        case ::aidl::android::hardware::radio::network::CellIdentity::Tag::tdscdma: {
          RLOGE("don't support , error");
          break;
        }
        case ::aidl::android::hardware::radio::network::CellIdentity::Tag::cdma: {
          auto& cdmaData = regResponse.cellIdentity.get<::aidl::android::hardware::radio::network::CellIdentity::Tag::cdma>();
          p.writeInt32(cdmaData.networkId);
          p.writeInt32(cdmaData.systemId);
          p.writeInt32(cdmaData.baseStationId);
          p.writeInt32(cdmaData.longitude);
          p.writeInt32(cdmaData.latitude);
          RpDataUtils::writeStringToParcel(&p, cdmaData.operatorNames.alphaLong.c_str());
          RpDataUtils::writeStringToParcel(&p, cdmaData.operatorNames.alphaShort.c_str());
          break;
        }
        case ::aidl::android::hardware::radio::network::CellIdentity::Tag::lte: {
          auto& lteData = regResponse.cellIdentity.get<::aidl::android::hardware::radio::network::CellIdentity::Tag::lte>();
        //   p.writeInt32(std::stoi(lteData.mcc));
        //   p.writeInt32(std::stoi(lteData.mnc));
        //   p.writeInt32(lteData.mnc.length());
          RpDataUtils::writeStringToParcel(&p, lteData.mcc.c_str());
          RpDataUtils::writeStringToParcel(&p, lteData.mnc.c_str());
          p.writeInt32(lteData.ci);
          p.writeInt32(lteData.pci);
          p.writeInt32(lteData.tac);
          p.writeInt32(lteData.earfcn);
          RpDataUtils::writeStringToParcel(&p, lteData.operatorNames.alphaLong.c_str());
          RpDataUtils::writeStringToParcel(&p, lteData.operatorNames.alphaShort.c_str());
          p.writeInt32(lteData.bandwidth);
          int32_t size = lteData.bands.size();
          p.writeInt32(size);
          if(size == 1) {
            p.writeInt32((int32_t)lteData.bands[0]);
          }
          break;
        }
        case ::aidl::android::hardware::radio::network::CellIdentity::Tag::nr: {
          auto& nrData = regResponse.cellIdentity.get<::aidl::android::hardware::radio::network::CellIdentity::Tag::nr>();
        //   p.writeInt32(std::stoi(nrData.mcc));
        //   p.writeInt32(std::stoi(nrData.mnc));
        //   p.writeInt32(nrData.mnc.length());
          RpDataUtils::writeStringToParcel(&p, nrData.mcc.c_str());
          RpDataUtils::writeStringToParcel(&p, nrData.mnc.c_str());
          p.writeUint64(nrData.nci);
          p.writeUint32(nrData.pci);
          p.writeInt32(nrData.tac);
          p.writeInt32(nrData.nrarfcn);
          RpDataUtils::writeStringToParcel(&p, nrData.operatorNames.alphaLong.c_str());
          RpDataUtils::writeStringToParcel(&p, nrData.operatorNames.alphaShort.c_str());
          int32_t size = nrData.bands.size();
          p.writeInt32(size);
          if(size == 1) {
            p.writeInt32((int32_t)nrData.bands[0]);
          }
          break;
        }
    }
    ::aidl::android::hardware::radio::network::AccessTechnologySpecificInfo::Tag aTag = regResponse.accessTechnologySpecificInfo.getTag();
    p.writeInt32((int32_t)aTag);
    switch (aTag) {
        case ::aidl::android::hardware::radio::network::AccessTechnologySpecificInfo::Tag::cdmaInfo: {
          auto& cdma2000RegistrationInfo =
              regResponse.accessTechnologySpecificInfo.get<::aidl::android::hardware::radio::network::AccessTechnologySpecificInfo::Tag::cdmaInfo>();
          p.writeBool(cdma2000RegistrationInfo.cssSupported);
          p.writeInt32(cdma2000RegistrationInfo.roamingIndicator);
          p.writeInt32(cdma2000RegistrationInfo.systemIsInPrl);
          p.writeInt32(cdma2000RegistrationInfo.defaultRoamingIndicator);
          break;
        }
        case ::aidl::android::hardware::radio::network::AccessTechnologySpecificInfo::Tag::eutranInfo: {
          auto& eutranRegistrationInfo = regResponse.accessTechnologySpecificInfo
                                             .get<::aidl::android::hardware::radio::network::AccessTechnologySpecificInfo::Tag::eutranInfo>();
          p.writeBool(eutranRegistrationInfo.lteVopsInfo.isVopsSupported);
          p.writeBool(eutranRegistrationInfo.lteVopsInfo.isEmcBearerSupported);
          p.writeBool(eutranRegistrationInfo.nrIndicators.isEndcAvailable);
          p.writeBool(eutranRegistrationInfo.nrIndicators.isDcNrRestricted);
          p.writeBool(eutranRegistrationInfo.nrIndicators.isNrAvailable);
          p.writeByte((int8_t)eutranRegistrationInfo.lteAttachResultType);
          p.writeInt32(eutranRegistrationInfo.extraInfo);
          break;
        }
        case ::aidl::android::hardware::radio::network::AccessTechnologySpecificInfo::Tag::ngranNrVopsInfo: {
          auto& ngranNrVopsRegistrationInfo =
              regResponse.accessTechnologySpecificInfo
                  .get<::aidl::android::hardware::radio::network::AccessTechnologySpecificInfo::Tag::ngranNrVopsInfo>();
          p.writeByte(ngranNrVopsRegistrationInfo.vopsSupported);
          p.writeByte(ngranNrVopsRegistrationInfo.emcSupported);
          p.writeByte(ngranNrVopsRegistrationInfo.emfSupported);
          break;
        }
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
}

ndk::ScopedAStatus RadioNetworkResponse::getVoiceRegistrationStateResponse(
    const RadioResponseInfo &info, const RegStateResult &regResponse) {
  RLOGD("getVoiceRegistrationStateResponse(%d): response: info = %s, "
        "regResponse = %s",
        mSlot, info.toString().c_str(), regResponse.toString().c_str());
  handleRegStateResult(info, regResponse);
  return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioNetworkResponse::isNrDualConnectivityEnabledResponse(
        const RadioResponseInfo& info, bool isEnabled) {
    rspInfo = info;
    isNrDualConnectivityEnabled = isEnabled;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setAllowedNetworkTypesBitmapResponse(
        const RadioResponseInfo& info) {
    RLOGD("setAllowedNetworkTypesBitmapResponse(%d): response: info = %s", mSlot, info.toString().c_str());
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

ndk::ScopedAStatus RadioNetworkResponse::setBandModeResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setBarringPasswordResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setCdmaRoamingPreferenceResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setCellInfoListRateResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("setCellInfoListRateResponse(%d): response: info = %s", mSlot, info.toString().c_str());
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

ndk::ScopedAStatus RadioNetworkResponse::setIndicationFilterResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("setIndicationFilterResponse(%d): response: info = %s", mSlot, info.toString().c_str());
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

ndk::ScopedAStatus RadioNetworkResponse::setLinkCapacityReportingCriteriaResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setLocationUpdatesResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setNetworkSelectionModeAutomaticResponse(
        const RadioResponseInfo& info) {
    RLOGD("%s(%d): response: info = %s", __func__, mSlot, info.toString().c_str());
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

ndk::ScopedAStatus RadioNetworkResponse::setNetworkSelectionModeManualResponse(
        const RadioResponseInfo& info) {
    RLOGD("%s(%d): response: info = %s", __func__, mSlot, info.toString().c_str());
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

ndk::ScopedAStatus RadioNetworkResponse::setNrDualConnectivityStateResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setSignalStrengthReportingCriteriaResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setSuppServiceNotificationsResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setSystemSelectionChannelsResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setUsageSettingResponse(const RadioResponseInfo& info) {
    RLOGD("setUsageSettingResponse(%d): response: info = %s", mSlot, info.toString().c_str());
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

ndk::ScopedAStatus RadioNetworkResponse::startNetworkScanResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::stopNetworkScanResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::supplyNetworkDepersonalizationResponse(
        const RadioResponseInfo& info, int32_t /*remainingRetries*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setEmergencyModeResponse(
        const RadioResponseInfo& info, const EmergencyRegResult& /*regState*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::triggerEmergencyNetworkScanResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::exitEmergencyModeResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::cancelEmergencyNetworkScanResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setNullCipherAndIntegrityEnabledResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::isNullCipherAndIntegrityEnabledResponse(
        const RadioResponseInfo& info, bool /*isEnabled*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::isN1ModeEnabledResponse(
        const RadioResponseInfo& info, bool /*isEnabled*/) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setN1ModeEnabledResponse(const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setCellularIdentifierTransparencyEnabledResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::isCellularIdentifierTransparencyEnabledResponse(
        const RadioResponseInfo& info, bool enabled) {
    rspInfo = info;
    this->isCellularIdentifierTransparencyEnabled = enabled;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::setSecurityAlgorithmsUpdatedEnabledResponse(
        const RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RadioNetworkResponse::isSecurityAlgorithmsUpdatedEnabledResponse(
        const RadioResponseInfo& info, bool enabled) {
    rspInfo = info;
    this->isSecurityAlgorithmsUpdatedEnabled = enabled;

    return ndk::ScopedAStatus::ok();
}
