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

#include <aidl/android/hardware/radio/RadioAccessFamily.h>
#include <aidl/android/hardware/radio/data/ApnAuthType.h>
#include <aidl/android/hardware/radio/data/ApnTypes.h>
#include <aidl/android/hardware/radio/data/DataProfileInfo.h>
#include <aidl/android/hardware/radio/data/DataRequestReason.h>
#include <aidl/android/hardware/radio/data/LinkAddress.h>
#include <aidl/android/hardware/radio/data/OsAppId.h>
#include <aidl/android/hardware/radio/data/PdpProtocolType.h>
#include <aidl/android/hardware/radio/data/SliceInfo.h>
#include <aidl/android/hardware/radio/RadioResponseInfo.h>

#include <aidl/vendor/mediatek/hardware/mtkradioex/data/BnMtkRadioExDataIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/BnMtkRadioExDataResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/IMtkRadioExData.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/IpDescriptors.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/SetupDataCallResultSlice.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/TrafficDescriptor.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/data/UrspRule.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/assist/BnAssistRadioResponse.h>

#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::data;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::data;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::assist;


/* Callback class for mtkradioex data response */
class MtkRadioExDataResponse : public BnMtkRadioExDataResponse {
public:
    MtkRadioExDataResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    RadioResponseInfo rspInfo;
    int32_t allocatedPduSessionId;
    SetupDataCallResultSlice setupDataCallResult;

    virtual ndk::ScopedAStatus dataConnectionAttachResponse(const RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus dataConnectionDetachResponse(const RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus enableDsdaIndicationResponse(const RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getDsdaStatusResponse(const RadioResponseInfo& in_info, int32_t in_mode) override;
    virtual ndk::ScopedAStatus resetAllConnectionsResponse(const RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus resetMdDataRetryCountResponse(const RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setupDataCallResponseSlice(const RadioResponseInfo& in_info,
            const SetupDataCallResultSlice& in_dcResponse) override;
    // virtual ndk::ScopedAStatus syncDataSettingsToMdResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    // virtual ndk::SpAIBinder asBinder() override;
    // virtual bool isRemote() override;

};

