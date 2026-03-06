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
#include <aidl/android/hardware/radio/RadioIndicationType.h>
#include "RfxDispatchThread.h"

// #include "mtkradioex_aidl_hal_utils.h"
// #include "radio_config_utils.h"
// #include "radio_sim_utils.h"
// #include "radio_network_utils.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::network;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::smartratswitch;
using namespace aidl::android::hardware::radio;

/* Callback class for radio network indication */
class MtkRadioExNetworkIndication : public BnMtkRadioExNetworkIndication {
public:
    MtkRadioExNetworkIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus currentSignalStrengthWithWcdmaEcioInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::network::SignalStrengthWithWcdmaEcio& in_signalStrength) override;
    virtual ndk::ScopedAStatus networkBandInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_state) override;
    virtual ndk::ScopedAStatus networkInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<std::string>& in_networkinfo) override;
    virtual ndk::ScopedAStatus nrCaBandChangeInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_bands) override;
    virtual ndk::ScopedAStatus nrSysInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_nrSysInfos) override;
    virtual ndk::ScopedAStatus on5GUWInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_data) override;
    virtual ndk::ScopedAStatus onMccMncChanged(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_mccmnc) override;
    virtual ndk::ScopedAStatus onNwCfgInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, bool in_mimo, bool in_qam_256, bool in_qam_ul64) override;
    virtual ndk::ScopedAStatus onNwRrcStateInd(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_rat, int32_t in_state) override;
    virtual ndk::ScopedAStatus onPseudoCellInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_cellInfo) override;
    virtual ndk::ScopedAStatus responseCsNetworkStateChangeInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<std::string>& in_state) override;
    virtual ndk::ScopedAStatus responseFemtocellInfo(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<std::string>& in_info) override;
    virtual ndk::ScopedAStatus responseLteNetworkInfo(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_lteBand) override;
    virtual ndk::ScopedAStatus responseModulationInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_modulation) override;
    virtual ndk::ScopedAStatus responseNetworkEventInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_event) override;
    virtual ndk::ScopedAStatus responsePsNetworkStateChangeInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_state) override;
    virtual ndk::ScopedAStatus sib16TimeInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_sib16Time, int64_t in_receivedTime) override;
    virtual ndk::ScopedAStatus toeInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_longName, const std::string& in_shortName, const std::string& in_numeric) override;
    virtual ndk::ScopedAStatus iwlanRegistrationStateInd(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_state) override;

};