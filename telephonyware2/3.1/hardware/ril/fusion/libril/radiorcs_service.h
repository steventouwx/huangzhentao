/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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

#ifndef RADIORCS_SERVICE_H
#define RADIORCS_SERVICE_H

#include <telephony/mtk_ril.h>
#include <ril_internal.h>

namespace mtkRadioExRcs {
    int startServiceResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen);
    int stopServiceResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen);
    int registrationInfoResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen);
    int enableDeregIndResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen);
    int resumeDeregistrationResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen);
    int triggerRestorationResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen);
    int sendSipMessageResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t respLen);
    int switchRcsRoiStatusResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int updateRcsCapabilitiesResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int updateRcsSessionInfoResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int getUserAgentResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int registrationInfoInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);
    int deregistraionInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);
    int sipMessageInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);
    int clearMtkRcsResponseAndIndications(unsigned int slotId);
}

#include <aidl/vendor/mediatek/hardware/mtkradioex/rcs/BnMtkRadioExRcs.h>

namespace vendor::mediatek::hardware::mtkradioex::rcs {
    using ::ndk::ScopedAStatus;
    namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::rcs;

    class MtkRadioRcs : public aidl::BnMtkRadioExRcs {
        public:
            ScopedAStatus startRcsService(int32_t serial, int32_t clientId) override;

            ScopedAStatus stopRcsService(int32_t serial, int32_t clientId) override;

            ScopedAStatus requestRegistrationInfo(int32_t serial, int32_t clientId) override;

            ScopedAStatus setImsDeregistrationIndicationEnabled(int32_t serial,
                bool enabled, int32_t clientId) override;

            ScopedAStatus resumeImsDeregistration(int32_t serial, int32_t clientId) override;

            ScopedAStatus triggerRestoration(int32_t serial, int32_t clientId) override;

            ScopedAStatus sendSipMessage(int32_t serial, int32_t requestId, int32_t connId,
                    const std::vector<uint8_t>& msgData, int32_t clientId) override;

            ScopedAStatus switchRcsRoiStatus(int32_t serial, bool status,
                int32_t clientId) override;

            ScopedAStatus updateRcsCapabilities(int32_t serial, int32_t mode,
                const std::string& featureTags, int32_t clientId) override;

            ScopedAStatus updateRcsSessionInfo(int32_t serial, int32_t count,
                int32_t clientId) override;

            ScopedAStatus getUserAgent(int32_t serial, int32_t clientId) override;

            ScopedAStatus responseAcknowledgementMtk() override;

            ScopedAStatus setResponseFunctionsRcs(
                const std::shared_ptr<aidl::IMtkRadioExRcsResponse>& radioResponse,
                const std::shared_ptr<aidl::IMtkRadioExRcsIndication>& radioIndication)  override;

        public:
            MtkRadioRcs(unsigned int slotId);
            uint32_t m_slot_id;

        public:
            bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
                if (client < 0) {
                    return false;
                }
                return clientId == ((unsigned int)client);
            }
    };
}


#endif  // RADIORCS_SERVICE_H
