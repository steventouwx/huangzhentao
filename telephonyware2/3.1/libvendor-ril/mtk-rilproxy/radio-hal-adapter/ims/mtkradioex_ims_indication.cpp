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

#include "mtkradioex_ims_indication.h"
#include <mtk_log.h>
#include "RpDataUtils.h"

#define LOG_TAG "MtkRadioExSimIndication"

MtkRadioExImsIndication::MtkRadioExImsIndication(int slot, RfxDispatchThread* dispatchThread) {
     mSlot = slot;
     mDispatchThread = dispatchThread;
}
ndk::ScopedAStatus MtkRadioExImsIndication::imsBearerStateNotify(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsBearerInit(::aidl::android::hardware::radio::RadioIndicationType ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsDataInfoNotify(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::cdmaNewSmsEx(::aidl::android::hardware::radio::RadioIndicationType , const ::aidl::android::hardware::radio::messaging::CdmaSmsMessage& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::newSmsStatusReportEx(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<uint8_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::newSmsEx(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<uint8_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::rttModifyResponse(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::rttTextReceive(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::rttCapabilityIndication(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t , int32_t , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::rttModifyRequestReceive(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::audioIndication(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::sendVopsIndication(::aidl::android::hardware::radio::RadioIndicationType , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::volteSetting(::aidl::android::hardware::radio::RadioIndicationType , bool ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsRegistrationInfo(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsSupportEcc(::aidl::android::hardware::radio::RadioIndicationType , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::sipRegInfoInd(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t , const std::vector<std::string>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsRegStatusReport(::aidl::android::hardware::radio::RadioIndicationType , const ::aidl::vendor::mediatek::hardware::mtkradioex::ims::ImsRegStatusInfo& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsRegInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::eregrtInfoInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsRegFlagInd(::aidl::android::hardware::radio::RadioIndicationType , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsRtpInfo(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::onXui(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::onUssi(::aidl::android::hardware::radio::RadioIndicationType , int32_t , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::onVolteSubscription(::aidl::android::hardware::radio::RadioIndicationType , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::videoCapabilityIndicator(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::videoRingtoneEventInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<std::string>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::onMDInternetUsageInd(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::ectIndication(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::callInfoIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<std::string>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::callmodChangeIndicator(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::econfResultIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::sipCallProgressIndicator(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsConferenceInfoIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::ims::ImsConfParticipant>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::lteMessageWaitingIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsDialogIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::voice::Dialog>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::noEmergencyCallbackMode(::aidl::android::hardware::radio::RadioIndicationType ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsRedialEmergencyIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::speechCodecInfoIndication(::aidl::android::hardware::radio::RadioIndicationType , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsEventPackageIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string& , const std::string& , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::sipHeaderReport(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<std::string>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::callRatIndication(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsCfgDynamicImsSwitchComplete(::aidl::android::hardware::radio::RadioIndicationType ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsCfgConfigChanged(::aidl::android::hardware::radio::RadioIndicationType , int32_t , const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsCfgFeatureChanged(::aidl::android::hardware::radio::RadioIndicationType , int32_t , int32_t , int32_t ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::imsCfgConfigLoaded(::aidl::android::hardware::radio::RadioIndicationType ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::onSsacStatus(::aidl::android::hardware::radio::RadioIndicationType , const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsIndication::incomingCallSipInviteIndication(::aidl::android::hardware::radio::RadioIndicationType , const std::string& , const std::string&) {
    return ndk::ScopedAStatus::ok();
}

