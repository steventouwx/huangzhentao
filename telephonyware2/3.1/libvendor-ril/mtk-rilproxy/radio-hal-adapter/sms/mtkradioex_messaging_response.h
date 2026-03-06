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
#include <aidl/android/hardware/radio/messaging/IRadioMessaging.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/messaging/BnMtkRadioExMessagingIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/messaging/BnMtkRadioExMessagingResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/messaging/IMtkRadioExMessaging.h>
#include <aidl/android/hardware/radio/RadioResponseInfo.h>

#include "RfxDispatchThread.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::messaging;
using namespace aidl::android::hardware::radio::messaging;
using namespace aidl::android::hardware::radio;
/* Callback class for mtkradioex messaging response */
class MtkRadioExMessagingResponse : public BnMtkRadioExMessagingResponse {
public:
    MtkRadioExMessagingResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    RadioResponseInfo rspInfo;
    virtual ndk::ScopedAStatus getGsmBroadcastActivationRsp(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_active) override;
    virtual ndk::ScopedAStatus getGsmBroadcastLangsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_langs) override;
    virtual ndk::ScopedAStatus getSmsMemStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::messaging::SmsMemStatus& in_status) override;
    virtual ndk::ScopedAStatus getSmsParametersResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::messaging::SmsParams& in_param) override;
    virtual ndk::ScopedAStatus getSmsRuimMemoryStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::vendor::mediatek::hardware::mtkradioex::messaging::SmsMemStatus& in_memStatus) override;
    virtual ndk::ScopedAStatus removeCbMsgResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setEtwsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setGsmBroadcastLangsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setSmsParametersResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus exitSCBMResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
};

