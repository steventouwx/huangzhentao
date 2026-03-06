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

#include <telephony/mtk_ril.h>
#include <ril_internal.h>

namespace radioMessaging {
// response
int acknowledgeIncomingGsmSmsWithPduResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int acknowledgeLastIncomingCdmaSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);


int acknowledgeLastIncomingGsmSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int deleteSmsOnRuimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int deleteSmsOnSimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int getCdmaBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int setCdmaBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

// new added
int getGsmBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);


int getSmscAddressResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);


int reportSmsMemoryStatusResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int sendCdmaSmsExpectMoreResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int sendCdmaSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);


int sendImsSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int sendSMSExpectMoreResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int sendSmsResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int setCdmaBroadcastActivationResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int setGsmBroadcastActivationResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int setGsmBroadcastConfigResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int setSmscAddressResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int writeSmsToRuimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int writeSmsToSimResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

// indication

int cdmaNewSmsInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int cdmaRuimSmsStorageFullInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);


int newBroadcastSmsInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int newSmsInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int newSmsOnSimInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int newSmsStatusReportInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int simSmsStorageFullInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int clearMessagingResponseAndIndications(unsigned int slotId);
}


namespace mtkRadioExMessaging {
int getGsmBroadcastActivationRsp(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getGsmBroadcastLangsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getSmsMemStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);

int getSmsParametersResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);

int getSmsRuimMemoryStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int removeCbMsgResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setEtwsResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);

int setGsmBroadcastLangsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setSmsParametersResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);

int exitSCBMResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int esnMeidChangeInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int meSmsStorageFullInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int smsInfoExtInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen);

int smsReadyInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen);

int enterSCBMInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen);

int exitSCBMInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen);
int clearMtkMessagingResponseAndIndications(unsigned int slotId);
}

#include <aidl/android/hardware/radio/messaging/BnRadioMessaging.h>
namespace android::hardware::radio::messaging {

class RadioMessaging : public aidl::android::hardware::radio::messaging::BnRadioMessaging {
public:
    ::ndk::ScopedAStatus acknowledgeIncomingGsmSmsWithPdu(int32_t serial, bool success,
                                                          const std::string& ackPdu) override;
    ::ndk::ScopedAStatus acknowledgeLastIncomingCdmaSms(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::CdmaSmsAck& smsAck) override;
    ::ndk::ScopedAStatus acknowledgeLastIncomingGsmSms(
            int32_t serial, bool success,
            ::aidl::android::hardware::radio::messaging::SmsAcknowledgeFailCause cause) override;
    ::ndk::ScopedAStatus deleteSmsOnRuim(int32_t serial, int32_t index) override;
    ::ndk::ScopedAStatus deleteSmsOnSim(int32_t serial, int32_t index) override;
    ::ndk::ScopedAStatus getCdmaBroadcastConfig(int32_t serial) override;
    ::ndk::ScopedAStatus getGsmBroadcastConfig(int32_t serial) override;
    ::ndk::ScopedAStatus getSmscAddress(int32_t serial) override;
    ::ndk::ScopedAStatus reportSmsMemoryStatus(int32_t serial, bool available) override;
    ::ndk::ScopedAStatus responseAcknowledgement() override;
    ::ndk::ScopedAStatus sendCdmaSms(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::CdmaSmsMessage& sms) override;
    ::ndk::ScopedAStatus sendCdmaSmsExpectMore(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::CdmaSmsMessage& sms) override;
    ::ndk::ScopedAStatus sendImsSms(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::ImsSmsMessage& message) override;
    ::ndk::ScopedAStatus sendSms(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::GsmSmsMessage& message) override;
    ::ndk::ScopedAStatus sendSmsExpectMore(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::GsmSmsMessage& message) override;
    ::ndk::ScopedAStatus setCdmaBroadcastActivation(int32_t serial, bool activate) override;
    ::ndk::ScopedAStatus setCdmaBroadcastConfig(
            int32_t serial,
            const std::vector<
                    ::aidl::android::hardware::radio::messaging::CdmaBroadcastSmsConfigInfo>&
                    configInfo) override;
    ::ndk::ScopedAStatus setGsmBroadcastActivation(int32_t serial, bool activate) override;
    ::ndk::ScopedAStatus setGsmBroadcastConfig(
            int32_t serial,
            const std::vector<
                    ::aidl::android::hardware::radio::messaging::GsmBroadcastSmsConfigInfo>&
                    configInfo) override;
    ::ndk::ScopedAStatus setResponseFunctions(
            const std::shared_ptr<
                    ::aidl::android::hardware::radio::messaging::IRadioMessagingResponse>&
                    radioMessagingResponse,
            const std::shared_ptr<
                    ::aidl::android::hardware::radio::messaging::IRadioMessagingIndication>&
                    radioMessagingIndication) override;
    ::ndk::ScopedAStatus setSmscAddress(int32_t serial, const std::string& smsc) override;
    ::ndk::ScopedAStatus writeSmsToRuim(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::CdmaSmsWriteArgs& cdmaSms) override;
    ::ndk::ScopedAStatus writeSmsToSim(
            int32_t serial,
            const ::aidl::android::hardware::radio::messaging::SmsWriteArgs& smsWriteArgs) override;

  public:
       RadioMessaging(int slotid);
       int mSlotId;
};

}


#include <aidl/vendor/mediatek/hardware/mtkradioex/messaging/BnMtkRadioExMessaging.h>

namespace vendor::mediatek::hardware::mtkradioex::messaging {

class MtkRadioMessaging : public aidl::vendor::mediatek::hardware::mtkradioex::messaging::BnMtkRadioExMessaging {
public:
::ndk::ScopedAStatus getGsmBroadcastActivation(int32_t serial, int32_t clientId) override;
::ndk::ScopedAStatus getGsmBroadcastLangs(int32_t serial, int32_t clientId) override;
::ndk::ScopedAStatus getSmsMemStatus(int32_t serial, int32_t clientId) override;
::ndk::ScopedAStatus getSmsParameters(int32_t serial, int32_t clientId) override;
::ndk::ScopedAStatus getSmsRuimMemoryStatus(int32_t serial, int32_t clientId) override;
::ndk::ScopedAStatus removeCbMsg(int32_t serial, int32_t channelId, int32_t serialId, int32_t clientId) override;
::ndk::ScopedAStatus setEtws(int32_t serial, int32_t mode, int32_t clientId) override;
::ndk::ScopedAStatus setGsmBroadcastLangs(int32_t serial, const std::string& langs, int32_t clientId) override;
::ndk::ScopedAStatus setSmsParameters(int32_t serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::messaging::SmsParams& message, int32_t clientId) override;
::ndk::ScopedAStatus exitSCBM(int32_t serial, int32_t clientId) override;
::ndk::ScopedAStatus responseAcknowledgementMtk() override;
::ndk::ScopedAStatus setResponseFunctionsMtk(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::messaging::IMtkRadioExMessagingResponse>& radioResponse,
        const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::messaging::IMtkRadioExMessagingIndication>& radioIndication) override;

    public:
        MtkRadioMessaging(int slotid);
        int mSlotId;

    public:
        int sId(int clientId) {
            return mSlotId + clientId * MAX_SIM_COUNT;
        }

        bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
            if (client < 0) {
                return false;
            }
            return clientId == ((unsigned int)client);
        }
};

}

