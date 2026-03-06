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
#include <aidl/android/hardware/radio/RadioResponseInfo.h>

#include "RfxDispatchThread.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::ims;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::mwi;
using namespace aidl::android::hardware::radio;

/* Callback class for mtkradioex ims response */
class MtkRadioExImsResponse : public BnMtkRadioExImsResponse {
public:
    MtkRadioExImsResponse(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    RadioResponseInfo rspInfo;

    virtual ndk::ScopedAStatus imsBearerStateConfirmResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setImsBearerNotificationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus acknowledgeLastIncomingGsmSmsExResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus acknowledgeLastIncomingCdmaSmsExResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus sendImsSmsExResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const ::aidl::android::hardware::radio::messaging::SendSmsResult& in_sms) override;
    virtual ndk::ScopedAStatus setRttModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus sendRttModifyRequestResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus sendRttTextResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus rttModifyRequestResponseResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus toggleRttAudioIndicationResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setImsRegistrationReportResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus queryVopsStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_vops) override;
    virtual ndk::ScopedAStatus sendUssiResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus cancelUssiResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getXcapStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setupXcapUserAgentStringResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getBarringCallsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::ims::ImsBarringCall>& in_calls) override;
    virtual ndk::ScopedAStatus setBarringCallsResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus vtDialResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus vtDialWithSipUriResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus videoCallAcceptResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus videoRingtoneEventResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus dialWithSipUriResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus imsEctCommandResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus pullCallResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus eccRedialApproveResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus conferenceDialResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus forceReleaseCallResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus controlCallResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus controlImsConferenceCallMemberResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setSipHeaderResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setSipHeaderReportResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setImsCallModeResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setCallAdditionalInfoResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setImscfgResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setModemImsCfgResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_results) override;
    virtual ndk::ScopedAStatus setImsCfgFeatureValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getImsCfgFeatureValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_value) override;
    virtual ndk::ScopedAStatus setImsCfgProvisionValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getImsCfgProvisionValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_value) override;
    virtual ndk::ScopedAStatus getImsCfgResourceCapValueResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_value) override;
    virtual ndk::ScopedAStatus setWfcProfileResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus setVoiceDomainPreferenceResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info) override;
    virtual ndk::ScopedAStatus getVoiceDomainPreferenceResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, int32_t in_vdp) override;
    virtual ndk::ScopedAStatus querySsacStatusResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::vector<int32_t>& in_status) override;
    virtual ndk::ScopedAStatus getImscfgResponse(const ::aidl::android::hardware::radio::RadioResponseInfo& in_info, const std::string& in_results) override;
};
