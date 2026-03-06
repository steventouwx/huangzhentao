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
#include <aidl/android/hardware/radio/RadioIndicationType.h>

#include "RfxDispatchThread.h"

//using namespace aidl::vendor::mediatek::hardware::mtkradioex::sim;
//using namespace aidl::vendor::mediatek::hardware::mtkradioex::rsu;
using namespace aidl::android::hardware::radio;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::voice;


/* Callback class for mtkradioex voice indication */
class MtkRadioExVoiceIndication : public BnMtkRadioExVoiceIndication {
public:
    MtkRadioExVoiceIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;


  public:
    virtual ndk::ScopedAStatus callAdditionalInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CallInfoType in_ciType, const std::vector<std::string>& in_info) override;
    virtual ndk::ScopedAStatus cdmaCallAccepted(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus cfuStatusNotify(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CfuStatusNotification& in_cfuStatus) override;
    virtual ndk::ScopedAStatus cipherIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CipherNotification& in_cipherNotify) override;
    virtual ndk::ScopedAStatus confSRVCC(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_callIds) override;
    virtual ndk::ScopedAStatus crssIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CrssNotification& in_crssNotify) override;
    virtual ndk::ScopedAStatus eccNumIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_ecc_list_with_card, const std::string& in_ecc_list_no_card) override;
    virtual ndk::ScopedAStatus incomingCallIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::IncomingCallNotification& in_inCallNotify) override;
    virtual ndk::ScopedAStatus suppSvcNotifyEx(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::android::hardware::radio::network::SuppSvcNotification& in_suppSvc) override;

};
