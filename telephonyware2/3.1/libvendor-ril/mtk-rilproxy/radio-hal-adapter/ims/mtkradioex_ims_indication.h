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

#include <aidl/vendor/mediatek/hardware/mtkradioex/ims/BnMtkRadioExImsIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/ims/BnMtkRadioExImsResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/mwi/BnMwiRadioIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/mwi/BnMwiRadioResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/ims/IMtkRadioExIms.h>
#include <aidl/android/hardware/radio/RadioIndicationType.h>

#include "RfxDispatchThread.h"


using namespace aidl::vendor::mediatek::hardware::mtkradioex::ims;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::mwi;
using namespace aidl::android::hardware::radio;


/* Callback class for mtkradioex ims indication */
class MtkRadioExImsIndication : public BnMtkRadioExImsIndication {
public:
    MtkRadioExImsIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus imsBearerStateNotify(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_aid, int32_t in_action, const std::string& in_capability) override;
    virtual ndk::ScopedAStatus imsBearerInit(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus imsDataInfoNotify(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_capability, const std::string& in_event, const std::string& in_extra) override;
    virtual ndk::ScopedAStatus cdmaNewSmsEx(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::android::hardware::radio::messaging::CdmaSmsMessage& in_msg) override;
    virtual ndk::ScopedAStatus newSmsStatusReportEx(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<uint8_t>& in_pdu) override;
    virtual ndk::ScopedAStatus newSmsEx(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<uint8_t>& in_pdu) override;
    virtual ndk::ScopedAStatus rttModifyResponse(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_callId, int32_t in_result) override;
    virtual ndk::ScopedAStatus rttTextReceive(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_callId, int32_t in_lenOfString, const std::string& in_text) override;
    virtual ndk::ScopedAStatus rttCapabilityIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_callId, int32_t in_localCap, int32_t in_remoteCap, int32_t in_localStatus, int32_t in_remoteStatus) override;
    virtual ndk::ScopedAStatus rttModifyRequestReceive(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_callId, int32_t in_rttType) override;
    virtual ndk::ScopedAStatus audioIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_callId, int32_t in_audio) override;
    virtual ndk::ScopedAStatus sendVopsIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_vops) override;
    virtual ndk::ScopedAStatus volteSetting(::aidl::android::hardware::radio::RadioIndicationType in_type, bool in_isEnable) override;
    virtual ndk::ScopedAStatus imsRegistrationInfo(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_registerState, int32_t in_capability) override;
    virtual ndk::ScopedAStatus imsSupportEcc(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_supportLteEcc) override;
    virtual ndk::ScopedAStatus sipRegInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_account_id, int32_t in_response_code, const std::vector<std::string>& in_info) override;
    virtual ndk::ScopedAStatus imsRegStatusReport(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::ims::ImsRegStatusInfo& in_report) override;
    virtual ndk::ScopedAStatus imsRegInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_info) override;
    virtual ndk::ScopedAStatus eregrtInfoInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_info) override;
    virtual ndk::ScopedAStatus imsRegFlagInd(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_flag) override;
    virtual ndk::ScopedAStatus imsRtpInfo(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_pdnId, const std::string& in_networkId, const std::string& in_timer, const std::string& in_sendPktLost, const std::string& in_recvPktLost, const std::string& in_jitter, const std::string& in_delay) override;
    virtual ndk::ScopedAStatus onXui(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_accountId, const std::string& in_broadcastFlag, const std::string& in_xuiInfo) override;
    virtual ndk::ScopedAStatus onUssi(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_modeType, const std::string& in_msg) override;
    virtual ndk::ScopedAStatus onVolteSubscription(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_status) override;
    virtual ndk::ScopedAStatus videoCapabilityIndicator(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_callId, const std::string& in_localVideoCap, const std::string& in_remoteVideoCap) override;
    virtual ndk::ScopedAStatus videoRingtoneEventInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<std::string>& in_event) override;
    virtual ndk::ScopedAStatus onMDInternetUsageInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_info) override;
    virtual ndk::ScopedAStatus ectIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_call_id, int32_t in_ectResult, int32_t in_cause) override;
    virtual ndk::ScopedAStatus callInfoIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<std::string>& in_data) override;
    virtual ndk::ScopedAStatus callmodChangeIndicator(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_callId, const std::string& in_callMode, const std::string& in_videoState, const std::string& in_audioDirection, const std::string& in_pau) override;
    virtual ndk::ScopedAStatus econfResultIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_confCallId, const std::string& in_op, const std::string& in_num, const std::string& in_result, const std::string& in_cause, const std::string& in_joinedCallId) override;
    virtual ndk::ScopedAStatus sipCallProgressIndicator(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_callId, const std::string& in_dir, const std::string& in_sipMsgType, const std::string& in_method, const std::string& in_responseCode, const std::string& in_reasonText) override;
    virtual ndk::ScopedAStatus imsConferenceInfoIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::ims::ImsConfParticipant>& in_participants) override;
    virtual ndk::ScopedAStatus lteMessageWaitingIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_callId, const std::string& in_ptype, const std::string& in_urcIdx, const std::string& in_totalUrcCount, const std::string& in_rawData) override;
    virtual ndk::ScopedAStatus imsDialogIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::voice::Dialog>& in_dialogList) override;
    virtual ndk::ScopedAStatus noEmergencyCallbackMode(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus imsRedialEmergencyIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_callId) override;
    virtual ndk::ScopedAStatus speechCodecInfoIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_info) override;
    virtual ndk::ScopedAStatus imsEventPackageIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_callId, const std::string& in_ptype, const std::string& in_urcIdx, const std::string& in_totalUrcCount, const std::string& in_rawData) override;
    virtual ndk::ScopedAStatus sipHeaderReport(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<std::string>& in_data) override;
    virtual ndk::ScopedAStatus callRatIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_domain, int32_t in_rat) override;
    virtual ndk::ScopedAStatus imsCfgDynamicImsSwitchComplete(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus imsCfgConfigChanged(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_phoneId, const std::string& in_configId, const std::string& in_value) override;
    virtual ndk::ScopedAStatus imsCfgFeatureChanged(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_phoneId, int32_t in_featureId, int32_t in_value) override;
    virtual ndk::ScopedAStatus imsCfgConfigLoaded(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus onSsacStatus(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_status) override;
    virtual ndk::ScopedAStatus incomingCallSipInviteIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_callId, const std::string& in_content) override;

};