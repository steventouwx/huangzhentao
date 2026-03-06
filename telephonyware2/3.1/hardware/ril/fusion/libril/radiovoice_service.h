/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2022. All rights reserved.
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
#include <aidl/android/hardware/radio/voice/BnRadioVoice.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/voice/BnMtkRadioExVoice.h>

namespace aosp_voice = ::aidl::android::hardware::radio::voice;
namespace mtk_voice  = ::aidl::vendor::mediatek::hardware::mtkradioex::voice;

using ::ndk::ScopedAStatus;
using ::android::ClientId;

namespace radioVoice {
// response
int acceptCallResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int conferenceResponse(unsigned int slotId, ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);

int dialResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int emergencyDialResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int exitEmergencyCallbackModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int explicitCallTransferResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getCallForwardStatusResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getCallWaitingResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getClipResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getClirResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getCurrentCallsResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getLastCallFailCauseResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getMuteResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getPreferredVoicePrivacyResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int getTTYModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int handleStkCallSetupRequestFromSimResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int hangupConnectionResponse(unsigned int slotId, ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);

int hangupForegroundResumeBackgroundResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int hangupWaitingOrBackgroundResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int isVoNrEnabledResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int rejectCallResponse(unsigned int slotId, ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);

int sendBurstDtmfResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int sendCDMAFeatureCodeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int sendDtmfResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int separateConnectionResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int setCallForwardResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int setCallWaitingResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int setClirResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setMuteResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int setPreferredVoicePrivacyResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int setTTYModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int setVoNrEnabledResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int startDtmfResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);

int stopDtmfResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responseLen);


int switchWaitingOrHoldingAndActiveResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int cancelPendingUssdResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);

int sendUssdResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);


// indication
int callRingInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int callStateChangedInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno e, const void *response, size_t responseLen);

int cdmaCallWaitingInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int cdmaInfoRecInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int cdmaOtaProvisionStatusInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int currentEmergencyNumberListInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int enterEmergencyCallbackModeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int exitEmergencyCallbackModeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int indicateRingbackToneInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int onSupplementaryServiceIndicationInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen);

int resendIncallMuteInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int srvccStateNotifyInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int stkCallControlAlphaNotifyInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response,
        size_t responseLen);

int stkCallSetupInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int clearVoiceResponseAndIndications(unsigned int slotId);

int onUssdInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);
}  // namespace radioVoice


namespace mtkRadioExVoice {
// Response
int hangupAllResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int hangupWithReasonResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int getCallSubAddressResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int getColpResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int getColrResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int getEccNumResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int queryCallForwardInTimeSlotStatusResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int resetSuppServResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int sendCnapResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setCallForwardInTimeSlotResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setCallIndicationResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setCallSubAddressResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setCallValidTimerResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setClipResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setColpResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setColrResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setEccModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setEccNumResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setGwsdModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setIgnoreSameNumberIntervalResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setKeepAliveByIpDataResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setKeepAliveByPDCPCtrlPDUResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

int setSuppServPropertyResponse(unsigned int slotId, ClientId clientId,
                      int responseType, int serial, RIL_Errno e, const void *response,
                      size_t responselen);

// Indication
int callAdditionalInfoInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int cdmaCallAcceptedInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int cfuStatusNotifyInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int cipherIndicationInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int confSRVCCInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int crssNotifyInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int eccNumIndication(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int incomingCallIndicationInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);

int suppSvcNotifyExInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);
int clearMtkVoiceResponseAndIndications(unsigned int slotId);
int clearMtkVoiceImsResponseAndIndications(unsigned int slotId);

}  // namespace mtkRadioExVoice



namespace android::hardware::radio::voice {

class RadioVoice : public aosp_voice::BnRadioVoice {
public:
    ScopedAStatus acceptCall(int32_t serial) override;
    ScopedAStatus conference(int32_t serial) override;
    ScopedAStatus dial(
            int32_t serial, const aosp_voice::Dial& dialInfo) override;
    ScopedAStatus emergencyDial(
            int32_t serial, const aosp_voice::Dial& dialInfo,
            int32_t categories,
            const std::vector<std::string>& urns,
            aosp_voice::EmergencyCallRouting routing,
            bool hasKnownUserIntentEmergency, bool isTesting) override;
    ScopedAStatus exitEmergencyCallbackMode(int32_t serial) override;
    ScopedAStatus explicitCallTransfer(int32_t serial) override;
    ScopedAStatus getCallForwardStatus(
            int32_t serial,
            const aosp_voice::CallForwardInfo& callInfo) override;
    ScopedAStatus getCallWaiting(int32_t serial, int32_t serviceClass) override;
    ScopedAStatus getClip(int32_t serial) override;
    ScopedAStatus getClir(int32_t serial) override;
    ScopedAStatus getCurrentCalls(int32_t serial) override;
    ScopedAStatus getLastCallFailCause(int32_t serial) override;
    ScopedAStatus getMute(int32_t serial) override;
    ScopedAStatus getPreferredVoicePrivacy(int32_t serial) override;
    ScopedAStatus getTtyMode(int32_t serial) override;
    ScopedAStatus handleStkCallSetupRequestFromSim(int32_t serial, bool accept) override;
    ScopedAStatus hangup(int32_t serial, int32_t gsmIndex) override;
    ScopedAStatus hangupForegroundResumeBackground(int32_t serial) override;
    ScopedAStatus hangupWaitingOrBackground(int32_t serial) override;
    ScopedAStatus isVoNrEnabled(int32_t serial) override;
    ScopedAStatus rejectCall(int32_t serial) override;
    ScopedAStatus responseAcknowledgement() override;
    ScopedAStatus sendBurstDtmf(int32_t serial, const std::string& dtmf, int32_t on,
                                       int32_t off) override;
    ScopedAStatus sendCdmaFeatureCode(int32_t serial, const std::string& fcode) override;
    ScopedAStatus sendDtmf(int32_t serial, const std::string& s) override;
    ScopedAStatus separateConnection(int32_t serial, int32_t gsmIndex) override;
    ScopedAStatus setCallForward(
            int32_t serial,
            const aosp_voice::CallForwardInfo& callInfo) override;
    ScopedAStatus setCallWaiting(int32_t serial, bool enable, int32_t serviceClass) override;
    ScopedAStatus setClir(int32_t serial, int32_t status) override;
    ScopedAStatus setMute(int32_t serial, bool enable) override;
    ScopedAStatus setPreferredVoicePrivacy(int32_t serial, bool enable) override;
    ScopedAStatus setResponseFunctions(
            const std::shared_ptr<aosp_voice::IRadioVoiceResponse>&
                    radioVoiceResponse,
            const std::shared_ptr<aosp_voice::IRadioVoiceIndication>&
                    radioVoiceIndication) override;
    ScopedAStatus setTtyMode(int32_t serial,
                                    aosp_voice::TtyMode mode) override;
    ScopedAStatus setVoNrEnabled(int32_t serial, bool enable) override;
    ScopedAStatus startDtmf(int32_t serial, const std::string& s) override;
    ScopedAStatus stopDtmf(int32_t serial) override;
    ScopedAStatus switchWaitingOrHoldingAndActive(int32_t serial) override;
    ScopedAStatus cancelPendingUssd(int32_t serial) override;
    ScopedAStatus sendUssd(int32_t serial, const std::string& ussd) override;

public:
    explicit RadioVoice(int slotid);
    int mSlotId;
};

}  // namespace android::hardware::radio::voice


namespace vendor::mediatek::hardware::mtkradioex::voice {

class MtkRadioVoice : public mtk_voice::BnMtkRadioExVoice {
public:
    ScopedAStatus hangupAll(int32_t serial, int32_t clientId) override;
    ScopedAStatus hangupWithReason(int32_t serial,
            int32_t callId, int32_t reason, int32_t clientId) override;
    ScopedAStatus getCallSubAddress(int32_t serial, int32_t clientId) override;
    ScopedAStatus getColp(int32_t serial, int32_t clientId) override;
    ScopedAStatus getColr(int32_t serial, int32_t clientId) override;
    ScopedAStatus getEccNum(int32_t serial, int32_t clientId) override;
    ScopedAStatus queryCallForwardInTimeSlotStatus(int32_t in_serial,
            const mtk_voice::CallForwardInfoEx& in_callInfoEx, int32_t in_clientId) override;
    ScopedAStatus resetSuppServ(int32_t serial, int32_t clientId) override;
    ScopedAStatus responseAcknowledgementMtk() override;
    ScopedAStatus sendCnap(int32_t serial,
            const std::string& cnapssMessage, int32_t clientId) override;
    ScopedAStatus setBarringPasswordCheckedByNW(int32_t serial,
            const std::string& facility, const std::string& oldPassword,
            const std::string& newPassword,
            const std::string& cfmPassword,
            int32_t clientId) override;
    ScopedAStatus setCallForwardInTimeSlot(int32_t serial,
            const mtk_voice::CallForwardInfoEx& callInfoEx, int32_t clientId) override;
    ScopedAStatus setCallIndication(int32_t serial, int32_t mode, int32_t callId,
            int32_t seqNumber, int32_t cause, int32_t clientId) override;
    ScopedAStatus setCallSubAddress(int32_t serial, bool enable, int32_t clientId) override;
    ScopedAStatus setCallValidTimer(int32_t serial, int32_t timer, int32_t clientId) override;
    ScopedAStatus setClip(int32_t serial, int32_t clipEnable, int32_t clientId) override;
    ScopedAStatus setColp(int32_t serial, int32_t colpEnable, int32_t clientId) override;
    ScopedAStatus setColr(int32_t serial, int32_t colrEnable, int32_t clientId) override;
    ScopedAStatus setEccMode(int32_t serial, const std::string& number, int32_t enable,
            int32_t airplaneMode, int32_t imsReg, int32_t clientId) override;
    ScopedAStatus setEccNum(int32_t serial, const std::string& ecc_list_with_card,
            const std::string& ecc_list_no_card, int32_t clientId) override;
    ScopedAStatus setGwsdMode(int32_t serial, const std::vector<std::string>& data,
            int32_t clientId) override;
    ScopedAStatus setIgnoreSameNumberInterval(int32_t serial, int32_t interval,
            int32_t clientId) override;
    ScopedAStatus setKeepAliveByIpData(int32_t serial, const std::string& config,
            int32_t clientId) override;
    ScopedAStatus setKeepAliveByPDCPCtrlPDU(int32_t serial, const std::string& config,
            int32_t clientId) override;
    ScopedAStatus setResponseFunctionsMtk(
        const std::shared_ptr<mtk_voice::IMtkRadioExVoiceResponse>& radioResponse,
        const std::shared_ptr<mtk_voice::IMtkRadioExVoiceIndication>& radioIndication) override;
    ScopedAStatus setResponseFunctionsMtkIms(
        const std::shared_ptr<mtk_voice::IMtkRadioExVoiceResponse>& radioResponse,
        const std::shared_ptr<mtk_voice::IMtkRadioExVoiceIndication>& radioIndication) override;
    ScopedAStatus setSuppServProperty(int32_t serial, const std::string& name,
        const std::string& value, int32_t clientId) override;

public:
    explicit MtkRadioVoice(int slotid);

public:
    int mSlotId;

public:
    int sId(int clientId) {
        if(clientId == android::CLIENT_RILJ || clientId == android::CLIENT_IMS) {
                return mSlotId + clientId * MAX_SIM_COUNT;
        } else if(clientId == android::CLIENT_TBOX || clientId == android::CLIENT_TBOX_IMS) {
                return mSlotId;
        }
        return -1;
    }

    bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
        if (client < 0) {
            return false;
        }
        return clientId == ((unsigned int)client);
    }
};

}

