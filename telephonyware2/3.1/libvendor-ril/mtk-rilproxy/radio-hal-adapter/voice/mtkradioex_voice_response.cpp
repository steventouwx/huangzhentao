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
#include "mtkradioex_voice_response.h"

#define LOG_TAG "MtkRadioExVoiceResponse"

MtkRadioExVoiceResponse::MtkRadioExVoiceResponse(int slot, RfxDispatchThread* dispatchThread) {
     mSlot = slot;
     mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::hangupAllResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("hangupAllResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::hangupWithReasonResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::getCallSubAddressResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::getColpResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t , int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::getColrResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::getEccNumResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::queryCallForwardInTimeSlotStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::voice::CallForwardInfoEx>& ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::resetSuppServResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::sendCnapResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t , int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setCallForwardInTimeSlotResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setCallIndicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;
    RLOGD("setCallIndicationResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setCallSubAddressResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setCallValidTimerResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setClipResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setColpResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setColrResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setEccModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setEccNumResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setGwsdModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setIgnoreSameNumberIntervalResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setKeepAliveByIpDataResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setKeepAliveByPDCPCtrlPDUResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceResponse::setSuppServPropertyResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

