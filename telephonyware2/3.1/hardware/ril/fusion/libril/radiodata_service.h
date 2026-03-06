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

#ifndef __RADIODATA_SERVICE__H__
#define __RADIODATA_SERVICE__H__

#include <telephony/mtk_ril.h>
#include <ril_internal.h>

namespace radioData {
///response
int allocatePduSessionIdResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void* response, size_t responseLen);

int cancelHandoverResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void* response, size_t responseLen);

int deactivateDataCallResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responselen);

int getDataCallListResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responseLen);

int getSlicingConfigResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int releasePduSessionIdResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void* response, size_t responseLen);

int setDataAllowedResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen);

int setDataProfileResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen);

int setDataThrottlingResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void* response, size_t responseLen);

int setInitialAttachApnResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responselen);

int setupDataCallResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e, const void *response,
                          size_t responseLen);

int startHandoverResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void* response, size_t responseLen);


int startKeepaliveResponse(unsigned int slotId, android::ClientId clientId,
                           int responseType, int serial, RIL_Errno e,
                           const void *response, size_t responselen);

int stopKeepaliveResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen);

//indication
int dataCallListChangedInd(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen);


int keepaliveStatusInd(unsigned int slotId,
                       int indicationType, int token, RIL_Errno e, const void *response,
                       size_t responselen);

int pcoDataInd(unsigned int slotId,
               int indicationType, int token, RIL_Errno e, const void *response,
               size_t responselen);

// new add
int unthrottleApnInd(unsigned int slotId,
               int indicationType, int token, RIL_Errno e, const void *response,
               size_t responselen);

int slicingConfigChangedInd(unsigned int slotId,
               int indicationType, int token, RIL_Errno e, const void *response,
               size_t responselen);

int clearDataResponseAndIndications(unsigned int slotId);
}


namespace mtkRadioExData {
    int syncDataSettingsToMdResponse(unsigned int slotId, android::ClientId clientId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
    int resetMdDataRetryCountResponse(unsigned int slotId, android::ClientId clientId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
    int dataConnectionAttachResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int dataConnectionDetachResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int resetAllConnectionsResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
    int enableDsdaIndicationResponse(unsigned int slotId, android::ClientId clientId __unused,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int getDsdaStatusResponse(unsigned int slotId, android::ClientId clientId __unused,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int setFdModeResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int serial, RIL_Errno e, const void *response, size_t responselen);
    int setupDataCallSliceResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
    int mobileDataUsageInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
            const void *response, size_t responseLen);
    int onDsdaChangedInd(unsigned int slotId,
            int responseType, int token, RIL_Errno e, const void *response, size_t responselen);
    int onMdDataRetryCountReset(unsigned int slotId, int indicationType, int token,
            RIL_Errno e, const void *response, size_t responseLen);
    int onPlmnDataInd(unsigned int slotId, int indicationType, int token,
            RIL_Errno err, const void *response, size_t responseLen);
    int pcoDataAfterAttachedInd(unsigned int slotId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
    int networkRejectCauseInd(unsigned int slotId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
    int onNwLimitInd(unsigned int slotId, int indicationType, int token,
            RIL_Errno err, const void *response, size_t responseLen);
    int qualifiedNetworkTypesChangedInd(unsigned int slotId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
    int clearMtkDataResponseAndIndications(unsigned int slotId);
    int clearAssistResponseAndIndications(unsigned int slotId);
    // URSP
    int postUrspRule(unsigned int slotId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
}

#include <aidl/android/hardware/radio/data/BnRadioData.h>

namespace android::hardware::radio::data {
class RadioData : public aidl::android::hardware::radio::data::BnRadioData {
public:
    ::ndk::ScopedAStatus allocatePduSessionId(int32_t serial) override;
    ::ndk::ScopedAStatus cancelHandover(int32_t serial, int32_t callId) override;
    ::ndk::ScopedAStatus deactivateDataCall(
            int32_t serial, int32_t cid,
            ::aidl::android::hardware::radio::data::DataRequestReason reason) override;
    ::ndk::ScopedAStatus getDataCallList(int32_t serial) override;
    ::ndk::ScopedAStatus getSlicingConfig(int32_t serial) override;
    ::ndk::ScopedAStatus releasePduSessionId(int32_t serial, int32_t id) override;
    ::ndk::ScopedAStatus responseAcknowledgement() override;
    ::ndk::ScopedAStatus setDataAllowed(int32_t serial, bool allow) override;
    ::ndk::ScopedAStatus setDataProfile(
            int32_t serial,
            const std::vector<::aidl::android::hardware::radio::data::DataProfileInfo>& profiles)
            override;
    ::ndk::ScopedAStatus setDataThrottling(
            int32_t serial,
            ::aidl::android::hardware::radio::data::DataThrottlingAction dataThrottlingAction,
            int64_t completionDurationMillis) override;
    ::ndk::ScopedAStatus setInitialAttachApn(
            int32_t serial,
            const std::optional<::aidl::android::hardware::radio::data::DataProfileInfo>& dpInfo) override;
    ::ndk::ScopedAStatus setResponseFunctions(
            const std::shared_ptr<::aidl::android::hardware::radio::data::IRadioDataResponse>&
                    radioDataResponse,
            const std::shared_ptr<::aidl::android::hardware::radio::data::IRadioDataIndication>&
                    radioDataIndication) override;
    ::ndk::ScopedAStatus setupDataCall(
            int32_t serial, ::aidl::android::hardware::radio::AccessNetwork accessNetwork,
            const ::aidl::android::hardware::radio::data::DataProfileInfo& dataProfileInfo,
            bool roamingAllowed, ::aidl::android::hardware::radio::data::DataRequestReason reason,
            const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& addresses,
            const std::vector<std::string>& dnses, int32_t pduSessionId,
            const std::optional<::aidl::android::hardware::radio::data::SliceInfo>& sliceInfo,
            bool matchAllRuleAllowed) override;
    ::ndk::ScopedAStatus startHandover(int32_t serial, int32_t callId) override;
    ::ndk::ScopedAStatus startKeepalive(
            int32_t serial,
            const ::aidl::android::hardware::radio::data::KeepaliveRequest& keepalive) override;
    ::ndk::ScopedAStatus stopKeepalive(int32_t serial, int32_t sessionHandle) override;

  public:
    RadioData(int slotid);
    int mSlotId;
};

}  // android::hardware::radio::data


#include <aidl/vendor/mediatek/hardware/mtkradioex/data/BnMtkRadioExData.h>

namespace aidl_radio_data = ::aidl::android::hardware::radio::data;
namespace aidl_mtk_radio_data = ::aidl::vendor::mediatek::hardware::mtkradioex::data;

namespace vendor::mediatek::hardware::mtkradioex::data {

class MtkRadioData : public aidl::vendor::mediatek::hardware::mtkradioex::data::BnMtkRadioExData {
public:
    ::ndk::ScopedAStatus dataConnectionAttach(int32_t serial, int32_t type, int32_t clientId) override;
    ::ndk::ScopedAStatus dataConnectionDetach(int32_t serial, int32_t type, int32_t clientId) override;
    ::ndk::ScopedAStatus enableDsdaIndication(int32_t serial, bool enable, int32_t clientId) override;
    ::ndk::ScopedAStatus getDsdaStatus(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus resetAllConnections(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus resetMdDataRetryCount(int32_t serial, const std::string& apn, int32_t clientId) override;
    ::ndk::ScopedAStatus setFdMode(int32_t serial, int32_t mode, int32_t param1, int32_t param2,
            int32_t clientId) override;
    ::ndk::ScopedAStatus syncDataSettingsToMd(int32_t serial, const std::vector<int32_t>& settings,
            int32_t clientId) override;
    ::ndk::ScopedAStatus responseAcknowledgementMtk() override;
    ::ndk::ScopedAStatus setResponseFunctionsMtk(
            const std::shared_ptr<aidl_mtk_radio_data::IMtkRadioExDataResponse>& radioResponse,
            const std::shared_ptr<aidl_mtk_radio_data::IMtkRadioExDataIndication>& radioIndication) override;
    ::ndk::ScopedAStatus setResponseFunctionsAssist(
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::assist::IAssistRadioResponse>&
                    radioResponse) override;
    ::ndk::ScopedAStatus setupDataCallSlice(
            int32_t serial, ::aidl::android::hardware::radio::AccessNetwork accessNetwork,
            const ::aidl::android::hardware::radio::data::DataProfileInfo& dataProfileInfo,
            bool roamingAllowed, ::aidl::android::hardware::radio::data::DataRequestReason reason,
            const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& addresses,
            const std::vector<std::string>& dnses, int32_t pduSessionId,
            const std::optional<::aidl::android::hardware::radio::data::SliceInfo>& sliceInfo,
            bool matchAllRuleAllowed,
            const std::optional<aidl_mtk_radio_data::TrafficDescriptor>& trafficDescriptor,
            const std::optional<std::string>& verifyDescriptor,
            int32_t responseMode,
            int32_t clientId) override;

  public:
    MtkRadioData(int slotid);
    int mSlotId;

  public:
    bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
        if (client < 0) {
            return false;
        }
        return clientId == ((unsigned int)client);
    }
};
}

#define FREEIF(data)    \
if (data != NULL) {     \
    free(data);         \
    data = NULL;        \
}

#endif
