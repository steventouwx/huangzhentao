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

#include <aidl/vendor/mediatek/hardware/mtkradioex/assist/BnAssistModemResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/modem/BnMtkRadioExModemIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/modem/BnMtkRadioExModemResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/cap/BnMtkRadioExCapRadioResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/atci/BnAtciResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/atci/BnAtciIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/modem/IMtkRadioExModem.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/em/BnEmRadioResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/em/BnEmRadioIndication.h>
#include <aidl/android/hardware/radio/RadioIndicationType.h>
//using namespace aidl::android::hardware::radio::modem;

#include "RfxDispatchThread.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::assist;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::modem;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::atci;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::cap;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::em;
using namespace aidl::android::hardware::radio;

/* Callback class for mtkradioex modem indication */
class MtkRadioExModemIndication : public BnMtkRadioExModemIndication {
public:
    MtkRadioExModemIndication(int slot, RfxDispatchThread* dispatchThread);
private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:
    virtual ndk::ScopedAStatus dsbpStateChanged(::aidl::android::hardware::radio::RadioIndicationType in_indicationType, ::aidl::vendor::mediatek::hardware::mtkradioex::modem::DsbpState in_dsbpState) override;
    virtual ndk::ScopedAStatus eMBMSAtInfoIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_info) override;
    virtual ndk::ScopedAStatus eMBMSSessionStatusIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_status) override;
    virtual ndk::ScopedAStatus oemHookRaw(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<uint8_t>& in_data) override;
    virtual ndk::ScopedAStatus onTxPowerIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_indPower) override;
    virtual ndk::ScopedAStatus onTxPowerStatusIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_indPower) override;
    virtual ndk::ScopedAStatus worldModeChangedIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_modes) override;
    virtual ndk::ScopedAStatus onCellularQualityChangedInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_indStgs) override;

};