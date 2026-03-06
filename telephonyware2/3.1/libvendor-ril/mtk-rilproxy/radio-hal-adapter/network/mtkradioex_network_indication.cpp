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

#include <aidl/android/hardware/radio/RadioTechnology.h>
#include <algorithm>
#include <mtk_log.h>
#include <stdint.h>
#include "RpDataUtils.h"
#include "utils/Errors.h"
#include "mtkradioex_network_indication.h"

#define LOG_TAG "MtkRadioExNetworkIndication"

// Constructor definition
MtkRadioExNetworkIndication::MtkRadioExNetworkIndication(int slot, RfxDispatchThread* dispatchThread){
    mDispatchThread = dispatchThread;
    mSlot = slot;
}


ndk::ScopedAStatus MtkRadioExNetworkIndication::currentSignalStrengthWithWcdmaEcioInd(::aidl::android::hardware::radio::RadioIndicationType , const ::aidl::vendor::mediatek::hardware::mtkradioex::network::SignalStrengthWithWcdmaEcio& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::networkBandInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::networkInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<std::string>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::nrCaBandChangeInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::nrSysInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::on5GUWInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::onMccMncChanged(::aidl::android::hardware::radio::RadioIndicationType , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::onNwCfgInfoInd(::aidl::android::hardware::radio::RadioIndicationType , bool , bool , bool ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::onNwRrcStateInd(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::onPseudoCellInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::responseCsNetworkStateChangeInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<std::string>& in_state ) {
    RLOGD("responseCsNetworkStateChangeInd(%d): type = %s", mSlot, toString(in_type).c_str());

    Parcel p;
    android::status_t status = p.writeInt32(static_cast<int32_t>(in_type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    int32_t numStates = static_cast<int32_t>(in_state.size());
    status = p.writeInt32(numStates);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    for (const auto& state : in_state) {
      RpDataUtils::writeStringToParcel(&p, state.c_str());
      RLOGD("responseCsNetworkStateChangeInd: cs state = %s", state.c_str());
    }

    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED, &p);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::responseFemtocellInfo(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<std::string>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::responseLteNetworkInfo(::aidl::android::hardware::radio::RadioIndicationType , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::responseModulationInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::responseNetworkEventInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::responsePsNetworkStateChangeInd(::aidl::android::hardware::radio::RadioIndicationType type, const std::vector<int32_t>& in_state) {
    RLOGD("responsePsNetworkStateChangeInd(%d): type = %d, state_size = %d", mSlot, type, in_state.size());
    Parcel p;
    android::status_t status = p.writeInt32((int32_t)(type));
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    status = p.writeInt32(RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    int32_t num = in_state.size();
    status = p.writeInt32(num);
    if (status != android::OK) {
        RLOGE("writeInt32 failed");
        return ndk::ScopedAStatus().ok();
    }
    for(const auto a : in_state) {
        status = p.writeInt32(a);
        if (status != android::OK) {
            RLOGE("writeInt32 failed");
            return ndk::ScopedAStatus().ok();
        }
        RLOGD("responsePsNetworkStateChangeInd: ps state = %d", a);
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::sib16TimeInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , int64_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::toeInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExNetworkIndication::iwlanRegistrationStateInd(::aidl::android::hardware::radio::RadioIndicationType , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

