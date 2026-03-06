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
#include <aidl/android/hardware/radio/RadioResponseInfo.h>

#include "RfxDispatchThread.h"

//using namespace aidl::android::hardware::radio::modem;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::assist;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::modem;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::atci;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::cap;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::em;
using namespace aidl::android::hardware::radio;
/* Callback class for mtkradioex modem response */
class MtkRadioExModemResponse : public BnMtkRadioExModemResponse {
public:
    MtkRadioExModemResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    RadioResponseInfo rspInfo;
    virtual ndk::ScopedAStatus getEngineeringModeInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::vector<std::string>& in_result) override;
    virtual ndk::ScopedAStatus modifyModemTypeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_applyType) override;
    virtual ndk::ScopedAStatus restartRILDResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus runGbaAuthenticationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<std::string>& in_resList) override;
    virtual ndk::ScopedAStatus sendEmbmsAtCommandResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo, const std::string& in_data) override;
    virtual ndk::ScopedAStatus sendRequestRawResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<uint8_t>& in_data) override;
    virtual ndk::ScopedAStatus sendRequestStringsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<std::string>& in_data) override;
    virtual ndk::ScopedAStatus sendSarIndicatorResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setModemPowerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setTrmResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setTxPowerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus setTxPowerStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setVendorSettingResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus triggerModeSwitchByEccResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus sendWifiAssociatedResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus sendWifiEnabledResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus sendWifiIpAddressResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_responseInfo) override;
    virtual ndk::ScopedAStatus registerCellQltyReportResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;

};