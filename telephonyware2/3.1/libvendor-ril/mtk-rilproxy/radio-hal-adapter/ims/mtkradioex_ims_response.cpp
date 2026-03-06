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
#include "mtkradioex_ims_response.h"

#define LOG_TAG "MtkRadioExImsResponse"


MtkRadioExImsResponse::MtkRadioExImsResponse(int slot, RfxDispatchThread* dispatchThread) {
     mSlot = slot;
     mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus MtkRadioExImsResponse::imsBearerStateConfirmResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setImsBearerNotificationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::acknowledgeLastIncomingGsmSmsExResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::acknowledgeLastIncomingCdmaSmsExResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::sendImsSmsExResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const ::aidl::android::hardware::radio::messaging::SendSmsResult& ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setRttModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::sendRttModifyRequestResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::sendRttTextResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::rttModifyRequestResponseResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::toggleRttAudioIndicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setImsRegistrationReportResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::queryVopsStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::sendUssiResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::cancelUssiResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::getXcapStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setupXcapUserAgentStringResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::getBarringCallsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::ims::ImsBarringCall>& ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setBarringCallsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::vtDialResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::vtDialWithSipUriResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::videoCallAcceptResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::videoRingtoneEventResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::dialWithSipUriResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::imsEctCommandResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::pullCallResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::eccRedialApproveResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::conferenceDialResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::forceReleaseCallResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::controlCallResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    RLOGD("controlCallResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::controlImsConferenceCallMemberResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setSipHeaderResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setSipHeaderReportResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setImsCallModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setCallAdditionalInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setImscfgResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    RLOGD("setImscfgResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::getImscfgResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& in_results) {
    RLOGD("getImscfgResponse(%d): response: info = %s, result=%s", mSlot, info.toString().c_str(), in_results.c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    RpDataUtils::writeStringToParcel(&p, in_results.c_str());
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setModemImsCfgResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setImsCfgFeatureValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::getImsCfgFeatureValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setImsCfgProvisionValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::getImsCfgProvisionValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::string& ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::getImsCfgResourceCapValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setWfcProfileResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::setVoiceDomainPreferenceResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::getVoiceDomainPreferenceResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, int32_t ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExImsResponse::querySsacStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& info, const std::vector<int32_t>& ) {
    rspInfo = info;

    return ndk::ScopedAStatus::ok();
}

