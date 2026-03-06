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

#include <cstdint>
#include <mtk_log.h>
#include "RpDataUtils.h"
#include "mtkradioex_sim_response.h"

#define LOG_TAG "MtkRadioExSimResponse"

MtkRadioExSimResponse::MtkRadioExSimResponse(int slot, RfxDispatchThread* dispatchThread) {
     mSlot = slot;
     mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus MtkRadioExSimResponse::getIccidResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& in_response) {
    RLOGD("getIccidResponse: response(%d): info = %s, iccid = %s", mSlot, info.toString().c_str(), in_response.c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    if(info.error != RadioError::NONE) {
        RpDataUtils::writeStringToParcel(&p, "");
    } else {
        RpDataUtils::writeStringToParcel(&p, in_response.c_str());
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::activateUiccCardRsp(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::deactivateUiccCardRsp(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::deleteUPBEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::editUPBEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::getATRResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::getCurrentUiccCardProvisioningStatusRsp(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::getPhoneBookMemStorageResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbMemStorageResponse& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::getPhoneBookStringsLengthResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::handleStkCallSetupRequestFromSimWithResCodeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::queryNetworkLockResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t , int32_t , int32_t , int32_t , int32_t , int32_t , int32_t ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::setNetworkLockResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::queryPhbStorageInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::queryUPBAvailableResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::queryUPBCapabilityResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readPhbEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryStructure>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readPhoneBookEntryExtResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryExt>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readUPBAasListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<std::string>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readUPBAnrEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryStructure>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readUPBEmailEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readUPBGasListResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<std::string>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readUPBGrpEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::readUPBSneEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::sendRsuRequestResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::vendor::mediatek::hardware::mtkradioex::rsu::RsuResponseInfo& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::vsimNotificationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::VsimEvent& ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::vsimOperationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::setPhoneBookMemStorageResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::setPhonebookReadyResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::setSimPowerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::writePhbEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::writePhoneBookEntryExtResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::writeUPBGrpEntryResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::supplyDepersonalizationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::supplyDeviceNetworkDepersonalizationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::getSimTypeInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::SimTypeInfo&) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExSimResponse::switchSimTypeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
     rspInfo = info;

     return ndk::ScopedAStatus::ok();
}
