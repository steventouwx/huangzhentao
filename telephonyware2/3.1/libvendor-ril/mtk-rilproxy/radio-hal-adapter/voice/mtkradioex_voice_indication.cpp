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
#include "mtkradioex_voice_indication.h"
#include <mtk_log.h>
#include "RpDataUtils.h"

#define LOG_TAG "MtkRadioExVoiceIndication"
MtkRadioExVoiceIndication:: MtkRadioExVoiceIndication(int slot, RfxDispatchThread* dispatchThread) {
     mSlot = slot;
     mDispatchThread = dispatchThread;
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::callAdditionalInfoInd(::aidl::android::hardware::radio::RadioIndicationType, ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CallInfoType , const std::vector<std::string>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::cdmaCallAccepted(::aidl::android::hardware::radio::RadioIndicationType) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::cfuStatusNotify(::aidl::android::hardware::radio::RadioIndicationType, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CfuStatusNotification& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::cipherIndication(::aidl::android::hardware::radio::RadioIndicationType, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CipherNotification& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::confSRVCC(::aidl::android::hardware::radio::RadioIndicationType, const std::vector<int32_t>& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::crssIndication(::aidl::android::hardware::radio::RadioIndicationType, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::CrssNotification& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::eccNumIndication(::aidl::android::hardware::radio::RadioIndicationType, const std::string& , const std::string& ) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::incomingCallIndication(::aidl::android::hardware::radio::RadioIndicationType type, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::IncomingCallNotification& inCallNotify) {
    RLOGD("callStateChanged: type = %d", type);
    Parcel p;
    p.writeInt32((int32_t)(type));
    p.writeInt32(RIL_UNSOL_INCOMING_CALL_INDICATION);
    p.writeInt32(6);
    RpDataUtils::writeStringToParcel(&p, ((inCallNotify.callId.c_str())));
    RpDataUtils::writeStringToParcel(&p, ((inCallNotify.number.c_str())));
    RpDataUtils::writeStringToParcel(&p, ((inCallNotify.type.c_str())));
    RpDataUtils::writeStringToParcel(&p, ((inCallNotify.callMode.c_str())));
    RpDataUtils::writeStringToParcel(&p, ((inCallNotify.seqNo.c_str())));
    RpDataUtils::writeStringToParcel(&p, ((inCallNotify.redirectNumber.c_str())));
    p.setDataPosition(0);
    mDispatchThread->enqueueUrcMessage(RADIO_TECH_GROUP_GSM, mSlot, RIL_UNSOL_INCOMING_CALL_INDICATION, &p);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus MtkRadioExVoiceIndication::suppSvcNotifyEx(::aidl::android::hardware::radio::RadioIndicationType , const ::aidl::android::hardware::radio::network::SuppSvcNotification& ) {
    return ndk::ScopedAStatus::ok();
}


