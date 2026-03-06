// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2021. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */
#include <cstdint>
#include <mtk_log.h>
#include "RpDataUtils.h"
#include "mtkradioex_network_response.h"

#define LOG_TAG "MtkRadioExNetworkResponse"

// Private constructor definition (empty if nothing specific during construction)
MtkRadioExNetworkResponse::MtkRadioExNetworkResponse(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::abortFemtocellListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo2 = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::cancelAvailableNetworksResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::cfgA2offsetResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::cfgB1offsetResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::clearLteAvailableFileResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::deactivateNrScgCommunicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setCarrierAggregationModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::enableCAPlusBandWidthFilterResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::enableSCGfailureResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::get4x4MimoEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getAllBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::vendor::mediatek::hardware::mtkradioex::network::BandModeInfo& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getApcInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getAvailableNetworksWithActResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::network::OperatorInfoWithAct>& networkInfos) {
    //rspInfo = info;
    //availableNetworks = networkInfos;
    RLOGD("getAvailableNetworksWithActResponse(%d): response: info = %s", mSlot, info.toString().c_str());
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
    int size = networkInfos.size();
    status = p.writeInt32(size);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    for (int i = 0; i < size; ++i) {
        RLOGD("networkInfos[%d]: %s", i, networkInfos[i].toString().c_str());
        RpDataUtils::writeStringToParcel(&p, networkInfos[i].lac.c_str());
        RpDataUtils::writeStringToParcel(&p, networkInfos[i].act.c_str());
        RpDataUtils::writeStringToParcel(&p, networkInfos[i].base.alphaLong.c_str());
        RpDataUtils::writeStringToParcel(&p, networkInfos[i].base.alphaShort.c_str());
        RpDataUtils::writeStringToParcel(&p, networkInfos[i].base.operatorNumeric.c_str());
        status = p.writeInt32(networkInfos[i].base.status);
        if (status != android::OK) {
          RLOGE("writeInt32 failed");
          return ndk::ScopedAStatus().ok();
        }
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getBandPriorityListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getCALinkCapabilityListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<std::string>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getCALinkEnableStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, bool ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getCaBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getCampedFemtoCellInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<std::string>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getCurrentPOLListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<std::string>& polList) {
    rspInfo = info;
    currentPolList = polList;
    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getDeactivateNrScgCommunicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t deactivate, int32_t allowSCGAdd) {
    rspInfo = info;
    deactivateNrScg = (deactivate == 1) ? true : false;
    allowNrScgAdd = (allowSCGAdd == 1) ? true : false;
    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getDisable2GResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getFemtocellListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<std::string>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getIWlanRegistrationStateResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getLte1xRttCellListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::network::Lte1xRttCellInfo>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getLteBsrTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getLteDataResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::vendor::mediatek::hardware::mtkradioex::network::LteData& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getLteRRCStateResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getLteReleaseVersionResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getLteScanDurationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getPOLCapabilityResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getPlmnNameFromSE13TableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getQamEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, bool , bool ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getRoamingEnableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& data) {
    rspInfo = info;
    roamingEnabled = data;
    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getSignalStrengthWithWcdmaEcioResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::vendor::mediatek::hardware::mtkradioex::network::SignalStrengthWithWcdmaEcio& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getSuggestedPlmnListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<std::string>& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getTOEInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& , const std::string& , const std::string& ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getTm9EnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, bool , bool ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::queryFemtoCellSystemSelectionModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::selectFemtocellResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::set4x4MimoEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setApcModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setBandPriorityListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setBgsrchDeltaSleepTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setCALinkEnableStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setDisable2GResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setFemtoCellSystemSelectionModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setLteBandEnableStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setLteBsrTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setLteReleaseVersionResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setLteScanDurationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setNROptionResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setNetworkSelectionModeManualWithActResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setNrBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setPOLEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setQamEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setSearchRatResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setSearchStoredFreqInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setServiceStateToModemResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setTm9EnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::setRoamingEnableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    //parent_network.notify(info.serial);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkResponse::getNitzTimeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& nitzTime, int32_t tz_valid) {
    RLOGD("getNitzTimeResponse(%d): response: info = %s, nitzTime = %s, tz_valid = %d", mSlot, info.toString().c_str(), nitzTime.c_str(), tz_valid);
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
    RpDataUtils::writeStringToParcel(&p, nitzTime.c_str());
    p.writeInt32(tz_valid);
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

