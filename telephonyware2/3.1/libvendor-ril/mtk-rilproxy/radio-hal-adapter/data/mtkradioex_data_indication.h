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

// #include <aidl/android/hardware/radio/RadioAccessFamily.h>
// #include <aidl/android/hardware/radio/data/ApnAuthType.h>
// #include <aidl/android/hardware/radio/data/ApnTypes.h>
// #include <aidl/android/hardware/radio/data/DataProfileInfo.h>
// #include <aidl/android/hardware/radio/data/DataRequestReason.h>
// #include <aidl/android/hardware/radio/data/LinkAddress.h>
// #include <aidl/android/hardware/radio/data/OsAppId.h>
// #include <aidl/android/hardware/radio/data/PdpProtocolType.h>
// #include <aidl/android/hardware/radio/data/SliceInfo.h>
#include <aidl/android/hardware/radio/RadioIndicationType.h>

#include <aidl/vendor/mediatek/hardware/mtkradioex/data/BnMtkRadioExDataIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/BnMtkRadioExDataResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/IMtkRadioExData.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/IpDescriptors.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/SetupDataCallResultSlice.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/TrafficDescriptor.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/UrspRule.h>
//#include <aidl/vendor/mediatek/hardware/mtkradioex/assist/BnAssistRadioResponse.h>

#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
//using namespace aidl::android::hardware::radio::data;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::data;
//using namespace aidl::vendor::mediatek::hardware::mtkradioex::assist;


/* Callback class for mtkradioex data indication */
class MtkRadioExDataIndication : public BnMtkRadioExDataIndication {
public:
    MtkRadioExDataIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus dedicatedBearerActivationInd(RadioIndicationType in_type, const DedicateDataCall& in_ddcData) override;
    virtual ndk::ScopedAStatus dedicatedBearerDeactivationInd(RadioIndicationType in_type, int32_t in_cid) override;
    virtual ndk::ScopedAStatus dedicatedBearerModificationInd(RadioIndicationType in_type, const DedicateDataCall& in_ddcData) override;
    virtual ndk::ScopedAStatus mobileDataUsageInd(RadioIndicationType in_type, const std::vector<int32_t>& in_data) override;
    virtual ndk::ScopedAStatus onDsdaChangedInd(RadioIndicationType in_type, int32_t in_mode) override;
    virtual ndk::ScopedAStatus onMdDataRetryCountReset(RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus onPlmnDataInd(RadioIndicationType in_type, const PlmnMvnoInfo& in_plmnMvnoInfo) override;
    virtual ndk::ScopedAStatus pcoDataAfterAttached(RadioIndicationType in_type, const PcoDataAttachedInfo& in_pcoData) override;
    virtual ndk::ScopedAStatus networkRejectCauseInd(RadioIndicationType in_type, const std::vector<int32_t>& in_data) override;
    virtual ndk::ScopedAStatus onNwLimitInd(RadioIndicationType in_type, const std::vector<int32_t>& in_state) override;
    virtual ndk::ScopedAStatus qualifiedNetworkTypesChangedInd(RadioIndicationType in_type, const std::vector<int32_t>& in_data) override;
    virtual ndk::ScopedAStatus postUrspRule(int32_t in_slotIndex, RadioIndicationType in_type, const std::string& in_originalUrsp,
            const std::vector<aidl::vendor::mediatek::hardware::mtkradioex::data::UrspRule>& in_urspRules) override;
    // virtual ndk::SpAIBinder asBinder() override;
    // bool isRemote() override;
};
