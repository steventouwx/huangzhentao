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

#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/BnMtkRadioExSimIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/BnMtkRadioExSimResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/IMtkRadioExSim.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuRequest.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuRequestInfo.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuResponseInfo.h>
#include <aidl/android/hardware/radio/RadioError.h>
#include <aidl/android/hardware/radio/config/IRadioConfig.h>
#include <aidl/android/hardware/radio/config/SimSlotStatus.h>
#include <aidl/android/hardware/radio/sim/CardStatus.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>
#include <aidl/android/hardware/radio/RadioResponseInfo.h>

#include "RfxDispatchThread.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::sim;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::rsu;
using namespace aidl::android::hardware::radio;
/* Callback class for mtkradioex SIM response */
class MtkRadioExSimResponse : public BnMtkRadioExSimResponse {
public:
    MtkRadioExSimResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:


    RadioResponseInfo rspInfo;
    virtual ndk::ScopedAStatus getIccidResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_response) override;
    virtual ndk::ScopedAStatus activateUiccCardRsp(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_simPowerOnOffResponse) override;
    virtual ndk::ScopedAStatus deactivateUiccCardRsp(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_simPowerOnOffResponse) override;
    virtual ndk::ScopedAStatus deleteUPBEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus editUPBEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getATRResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_response) override;
    virtual ndk::ScopedAStatus getCurrentUiccCardProvisioningStatusRsp(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_simPowerOnOffStatus) override;
    virtual ndk::ScopedAStatus getPhoneBookMemStorageResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbMemStorageResponse& in_phbMemStorage) override;
    virtual ndk::ScopedAStatus getPhoneBookStringsLengthResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_stringLengthInfo) override;
    virtual ndk::ScopedAStatus handleStkCallSetupRequestFromSimWithResCodeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus queryNetworkLockResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_catagory, int32_t in_state, int32_t in_retry_cnt, int32_t in_autolock_cnt, int32_t in_num_set, int32_t in_total_set, int32_t in_key_state) override;
    virtual ndk::ScopedAStatus setNetworkLockResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus queryPhbStorageInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_storageInfo) override;
    virtual ndk::ScopedAStatus queryUPBAvailableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_upbAvailable) override;
    virtual ndk::ScopedAStatus queryUPBCapabilityResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_upbCapability) override;
    virtual ndk::ScopedAStatus readPhbEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryStructure>& in_phbEntries) override;
    virtual ndk::ScopedAStatus readPhoneBookEntryExtResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryExt>& in_phbEntryExts) override;
    virtual ndk::ScopedAStatus readUPBAasListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<std::string>& in_aasList) override;
    virtual ndk::ScopedAStatus readUPBAnrEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryStructure>& in_anrs) override;
    virtual ndk::ScopedAStatus readUPBEmailEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_email) override;
    virtual ndk::ScopedAStatus readUPBGasListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<std::string>& in_gasList) override;
    virtual ndk::ScopedAStatus readUPBGrpEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_grpEntries) override;
    virtual ndk::ScopedAStatus readUPBSneEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_sne) override;
    virtual ndk::ScopedAStatus sendRsuRequestResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::rsu::RsuResponseInfo& in_rri) override;
    virtual ndk::ScopedAStatus vsimNotificationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::VsimEvent& in_event) override;
    virtual ndk::ScopedAStatus vsimOperationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setPhoneBookMemStorageResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setPhonebookReadyResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setSimPowerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus writePhbEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus writePhoneBookEntryExtResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus writeUPBGrpEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus supplyDepersonalizationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_remainingRetries) override;
    virtual ndk::ScopedAStatus supplyDeviceNetworkDepersonalizationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_remainingRetries) override;
    virtual ndk::ScopedAStatus getSimTypeInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::SimTypeInfo& in_typeInfo) override;
    virtual ndk::ScopedAStatus switchSimTypeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;

};