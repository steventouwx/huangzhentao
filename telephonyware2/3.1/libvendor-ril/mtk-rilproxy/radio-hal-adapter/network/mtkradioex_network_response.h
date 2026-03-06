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

#pragma once
#include <aidl/vendor/mediatek/hardware/mtkradioex/network/BnMtkRadioExNetworkIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/network/BnMtkRadioExNetworkResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/smartratswitch/BnSmartRatSwitchRadioIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/smartratswitch/BnSmartRatSwitchRadioResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/network/IMtkRadioExNetwork.h>

#include "RfxDispatchThread.h"
// #include "mtkradioex_aidl_hal_utils.h"
// #include "radio_config_utils.h"
// #include "radio_sim_utils.h"
// #include "radio_network_utils.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::network;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::smartratswitch;
using namespace aidl::android::hardware::radio;

//class MtkRadioExNetworkTest;

/* Callback class for mtkradioex network response */
class MtkRadioExNetworkResponse : public BnMtkRadioExNetworkResponse {
public:
    MtkRadioExNetworkResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:
    // MtkRadioExNetworkResponse(MtkRadioExServiceTest& parent_network);
    // virtual ~MtkRadioExNetworkResponse() = default;

    RadioResponseInfo rspInfo;
    RadioResponseInfo rspInfo2;
    std::vector<OperatorInfoWithAct> availableNetworks;
    std::vector<std::string> currentPolList;
    std::vector<int32_t> roamingEnabled;
    bool deactivateNrScg;
    bool allowNrScgAdd;

    virtual ndk::ScopedAStatus abortFemtocellListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus cancelAvailableNetworksResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus cfgA2offsetResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus cfgB1offsetResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus clearLteAvailableFileResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus deactivateNrScgCommunicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setCarrierAggregationModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus enableCAPlusBandWidthFilterResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus enableSCGfailureResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus get4x4MimoEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_enabled_bitmask) override;
    virtual ndk::ScopedAStatus getAllBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::network::BandModeInfo& in_data) override;
    virtual ndk::ScopedAStatus getApcInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_cellInfo) override;
    virtual ndk::ScopedAStatus getAvailableNetworksWithActResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::network::OperatorInfoWithAct>& in_networkInfosWithAct) override;
    virtual ndk::ScopedAStatus getBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_data) override;
    virtual ndk::ScopedAStatus getBandPriorityListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_bandPriList) override;
    virtual ndk::ScopedAStatus getCALinkCapabilityListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<std::string>& in_linkCapabilityList) override;
    virtual ndk::ScopedAStatus getCALinkEnableStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, bool in_status) override;
    virtual ndk::ScopedAStatus getCaBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_data) override;
    virtual ndk::ScopedAStatus getCampedFemtoCellInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<std::string>& in_femto) override;
    virtual ndk::ScopedAStatus getCurrentPOLListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::vector<std::string>& in_polList) override;
    virtual ndk::ScopedAStatus getDeactivateNrScgCommunicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, int32_t in_deactivate, int32_t in_allowSCGAdd) override;
    virtual ndk::ScopedAStatus getDisable2GResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, int32_t in_mode) override;
    virtual ndk::ScopedAStatus getFemtocellListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::vector<std::string>& in_femtoList) override;
    virtual ndk::ScopedAStatus getIWlanRegistrationStateResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, int32_t in_state) override;
    virtual ndk::ScopedAStatus getLte1xRttCellListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::network::Lte1xRttCellInfo>& in_list) override;
    virtual ndk::ScopedAStatus getLteBsrTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_timer) override;
    virtual ndk::ScopedAStatus getLteDataResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::network::LteData& in_data) override;
    virtual ndk::ScopedAStatus getLteRRCStateResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_state) override;
    virtual ndk::ScopedAStatus getLteReleaseVersionResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_mode) override;
    virtual ndk::ScopedAStatus getLteScanDurationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_duration) override;
    virtual ndk::ScopedAStatus getPOLCapabilityResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::vector<int32_t>& in_polCapability) override;
    virtual ndk::ScopedAStatus getPlmnNameFromSE13TableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_name) override;
    virtual ndk::ScopedAStatus getQamEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, bool in_ulOrDl, bool in_enabled) override;
    virtual ndk::ScopedAStatus getRoamingEnableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::vector<int32_t>& in_data) override;
    virtual ndk::ScopedAStatus getSignalStrengthWithWcdmaEcioResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::network::SignalStrengthWithWcdmaEcio& in_signalStrength) override;
    virtual ndk::ScopedAStatus getSuggestedPlmnListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::vector<std::string>& in_plmnList) override;
    virtual ndk::ScopedAStatus getTOEInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_longName, const std::string& in_shortName, const std::string& in_numeric) override;
    virtual ndk::ScopedAStatus getTm9EnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, bool in_fddOrTdd, bool in_enabled) override;
    virtual ndk::ScopedAStatus queryFemtoCellSystemSelectionModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, int32_t in_mode) override;
    virtual ndk::ScopedAStatus selectFemtocellResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus set4x4MimoEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setApcModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setBandPriorityListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setBgsrchDeltaSleepTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setCALinkEnableStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setDisable2GResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setFemtoCellSystemSelectionModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setLteBandEnableStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setLteBsrTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setLteReleaseVersionResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setLteScanDurationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setNROptionResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setNetworkSelectionModeManualWithActResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setNrBandModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setPOLEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setQamEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setSearchRatResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setSearchStoredFreqInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setServiceStateToModemResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setTm9EnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setRoamingEnableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus getNitzTimeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::string& nitzTime, int32_t tz_valid) override;
};
