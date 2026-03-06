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

#include <aidl/vendor/mediatek/hardware/mtkradioex/voice/BnMtkRadioExVoiceIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/voice/BnMtkRadioExVoiceResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/voice/IMtkRadioExVoice.h>
#include <aidl/android/hardware/radio/RadioResponseInfo.h>

#include "RfxDispatchThread.h"

//using namespace aidl::vendor::mediatek::hardware::mtkradioex::sim;
//using namespace aidl::vendor::mediatek::hardware::mtkradioex::rsu;
using namespace aidl::android::hardware::radio;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::voice;



/* Callback class for radio voice response */
class MtkRadioExVoiceResponse : public BnMtkRadioExVoiceResponse {
public:
    MtkRadioExVoiceResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;


  public:

    RadioResponseInfo rspInfo;

    virtual ndk::ScopedAStatus hangupAllResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus hangupWithReasonResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getCallSubAddressResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_enable) override;
    virtual ndk::ScopedAStatus getColpResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_n, int32_t in_m) override;
    virtual ndk::ScopedAStatus getColrResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_n) override;
    virtual ndk::ScopedAStatus getEccNumResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus queryCallForwardInTimeSlotStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::voice::CallForwardInfoEx>& in_callForwardInfoExs) override;
    virtual ndk::ScopedAStatus resetSuppServResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus sendCnapResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_n, int32_t in_m) override;
    virtual ndk::ScopedAStatus setCallForwardInTimeSlotResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setCallIndicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setCallSubAddressResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setCallValidTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setClipResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setColpResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setColrResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setEccModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setEccNumResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setGwsdModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setIgnoreSameNumberIntervalResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setKeepAliveByIpDataResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setKeepAliveByPDCPCtrlPDUResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setSuppServPropertyResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;

};

