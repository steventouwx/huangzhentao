/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2023. All rights reserved.
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

#ifndef RADIODCH_SERVICE_H
#define RADIODCH_SERVICE_H

#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <ril_service.h>
#include <rilAidlUtils.h>

namespace mtkRadioExDch {
    int dcBootstrapCfgQueryCmdRspCnf(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int dcCreateSipResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int dcModifySipResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int dcCloseSipResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int dcCreateCmdRspCnf(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int dcEnableRsp(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

    int dcBootstrapCfgQueryCmd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responselen);
    int dcConfigInitInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responselen);
    int dcConfigUpdateInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responselen);
    int dcConfigDeinitInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responselen);
    int dcCreateCmd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responselen);

    int clearMtkDchResponseAndIndications(unsigned int slotId);

    // MTK solution start @{
    int clearMtkDchInternalResponseAndIndications(unsigned int slotId);
    int dchDataActPacketRouteResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int dchDataIfupPacketRouteResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int dchDataPathConfigSyncResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int dchCreateDataChannelResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int dchCloseDataChannelResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int dchDataIfupPacketRouteInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responseLen);
    int dchConnectDataChannelInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responseLen);
    int dchCloseDataChannelInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responseLen);
    int dchCreateDataChannelInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responseLen);
    // MTK solution end }@
} // namespace mtkRadioExDch

#include <aidl/vendor/mediatek/hardware/mtkradioex/dch/BnMtkRadioExDc.h>

namespace vendor::mediatek::hardware::mtkradioex::dch {
    class MtkRadioDch : public aidl::vendor::mediatek::hardware::mtkradioex::dch::BnMtkRadioExDc {
    public:
        ::ndk::ScopedAStatus dcBootstrapCfgQueryCmdRsp(int32_t serial, int8_t callId, int8_t result,
                    const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfigLocal>& dataList,
                    int32_t clientId) override;
        ::ndk::ScopedAStatus dcCreateSipRequest(int32_t serial, int8_t callId,
                    const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfig>& dataList,
                    int32_t clientId) override;
        ::ndk::ScopedAStatus dcModifySipRequest(int32_t serial, int8_t callId,
                    const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfig>& dataList,
                    int32_t clientId) override;
        ::ndk::ScopedAStatus dcCloseSipRequest(int32_t serial, int8_t callId, int8_t closeAllStream,
                    const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcMapDelStruct>& dataList,
                    int32_t clientId) override;
        ::ndk::ScopedAStatus dcCreateCmdRsp(int32_t serial, int8_t callId, int8_t result, int32_t cause,
                    const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfig>& dataList,
                    int32_t clientId) override;
        ::ndk::ScopedAStatus responseAcknowledgementMtk() override;
        ::ndk::ScopedAStatus setResponseFunctionsDc(
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::dch::IMtkRadioExDcResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::dch::IMtkRadioExDcIndication>& radioIndication) override;

        // MTK solution start @{
        ::ndk::ScopedAStatus setResponseFunctionsDchInternal(
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::dch::IMtkRadioExDcResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::dch::IMtkRadioExDcIndication>& radioIndication) override;
        ::ndk::ScopedAStatus dchDataActPacketRouteRequest(int32_t serial, int32_t clientId) override;
        ::ndk::ScopedAStatus dchDataIfupPacketRouteRequest(int32_t serial, int32_t clientId) override;
        ::ndk::ScopedAStatus dchDataPathConfigSyncRequest(int32_t serial, int32_t clientId) override;
        ::ndk::ScopedAStatus dchCreateDataChannelRequest(int32_t serial, int32_t callId,
                    const std::string& xmlStrData, int32_t clientId)  override;
        ::ndk::ScopedAStatus dchCloseDataChannelRequest(int32_t serial, int32_t callId,
                    const std::string& xmlStrData, int32_t clientId)  override;
        // MTK solution end }@

    public:
        explicit MtkRadioDch(unsigned int slotId);

        unsigned int mSlotId;
        bool mIsDcSupport = false;
        // MTK solution start }@
        bool mIsDcInternalSupport = false;
        // MTK solution end }@

   private:
        bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId, int serial, int request) {
            if (client < 0 || clientId != ((unsigned int)client)) {
                return false;
            }
            if (!mIsDcSupport) {
                android::RequestInfo *pRI = android::addRequestToList(serial,
                             mSlotId + clientId * MAX_SIM_COUNT, request);
                if (pRI != NULL) sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
                return false;
            }

            return true;
        }

        // MTK solution start @{
        bool checkMtkRequestClientIdSupport(int32_t client, android::ClientId clientId, int serial, int request) {
            if (client < 0 || clientId != ((unsigned int)client)) {
                return false;
            }
            if (!mIsDcInternalSupport) {
                android::RequestInfo *pRI = android::addRequestToList(serial,
                             mSlotId + clientId * MAX_SIM_COUNT, request);
                if (pRI != NULL) sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
                return false;
            }

            return true;
        }
        // MTK solution end }@

        bool isValidSlotId(int slotId) {
            return (slotId >= 0) && (slotId < MAX_SIM_COUNT);
        }
    };

}

#endif  // RADIODCH_SERVICE_H
