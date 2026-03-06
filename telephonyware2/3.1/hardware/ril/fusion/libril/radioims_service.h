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


namespace radioIms {
// response
int setSrvccCallInfoResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
int updateImsRegistrationInfoResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
int startImsTrafficResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
int stopImsTrafficResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
int triggerEpsFallbackResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
int sendAnbrQueryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
int updateImsCallStatusResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

//indication
int onConnectionSetupFailure(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen);
int notifyAnbr(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen);
int triggerImsDeregistration(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen);

int clearImsResponseAndIndications(unsigned int slotId);
}

namespace mtkRadioExIms {
// M: IMS_DATA start @{
int imsBearerStateConfirmResponse(unsigned int slotId, android::ClientId clientId,
                                           int responseType, int token, RIL_Errno e,
                                           const void *response, size_t responselen);

int setImsBearerNotificationResponse(unsigned int slotId, android::ClientId clientId,
                                           int responseType, int token, RIL_Errno e,
                                           const void *response, size_t responselen);

int imsBearerStateNotifyInd(unsigned int slotId,
                           int indicationType, int token, RIL_Errno e,
                           const void *response, size_t responseLen);

int imsBearerInitInd(unsigned int slotId,
                     int indicationType, int token, RIL_Errno e,
                     const void *response, size_t responseLen);

int imsDataInfoNotifyInd(unsigned int slotId,
                           int indicationType, int token, RIL_Errno e,
                           const void *response, size_t responseLen);
// M: IMS_DATA end @}

// M: IMS_SMS start @{
int acknowledgeLastIncomingGsmSmsExResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);
int acknowledgeLastIncomingCdmaSmsExResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);
int sendImsSmsExResponse(unsigned int slotId, android::ClientId clientId __unused,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responseLen);
int newSmsStatusReportIndEx(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e, const void *response,
                                 size_t responseLen);
int newSmsIndEx(unsigned int slotId, int indicationType,
                     int token, RIL_Errno e, const void *response, size_t responseLen);
int cdmaNewSmsIndEx(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
// M: IMS_SMS end @}

// M: IMS_RTT start @{
int setRttModeResponse(unsigned int slotId, android::ClientId clientId __unused,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responselen);
int sendRttModifyRequestResponse(unsigned int slotId, android::ClientId clientId __unused,
                                        int responseType, int serial, RIL_Errno e,
                                        const void *response, size_t responselen);
int sendRttTextResponse(unsigned int slotId, android::ClientId clientId __unused,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responselen);
int rttModifyRequestResponseResponse(unsigned int slotId, android::ClientId clientId __unused,
                                            int responseType, int serial, RIL_Errno e,
                                            const void *response, size_t responselen);
int toggleRttAudioIndicationResponse(unsigned int slotId, android::ClientId clientId __unused,
                                        int responseType, int serial, RIL_Errno e, const void *response,
                                        size_t responselen);
int rttModifyResponseInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen);
int rttTextReceiveInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen);
int rttCapabilityIndicationInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen);
int rttModifyRequestReceiveInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen);
int audioIndicationInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responselen);
// M: IMS_RTT end @}

// M: IMS_BASE start @{
int setImsRegistrationReportResponse(unsigned int slotId, android::ClientId clientId __unused,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responselen);
int queryVopsStatusResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int sendVopsIndication(unsigned int slotId,
                                int indicationType, int token, RIL_Errno e,
                                const void *response, size_t responselen);//RCS+IMS
//int setVendorSettingResponse(unsigned int slotId, android::ClientId clientId,
//        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);//cs+ims
int volteSettingInd(unsigned int slotId,
                           int indicationType, int token, RIL_Errno e,
                           const void *response, size_t responseLen);
int imsRegistrationInfoInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e,
                              const void *response, size_t responseLen);
int imsSupportEccInd(unsigned int slotId,
                     int indicationType, int token, RIL_Errno e,
                     const void *response, size_t responseLen);
int sipRegInfoInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen);
int imsRegistrationStateInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int eiregDataInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int eregrtInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int imsRegFlagInd(unsigned int slotId,
                int indicationType, int token, RIL_Errno e,
                const void *response, size_t responseLen);
int imsRtpInfoInd(unsigned int slotId,
                  int indicationType, int token, RIL_Errno e, const void *response,
                  size_t responseLen);
int onXuiInd(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen);
int clearMtkImsResponseAndIndications(unsigned int slotId);
// M: IMS_BASE end @}

// M: IMS_SS start @{
int sendUssiResponse(unsigned int slotId, android::ClientId clientId __unused, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int cancelUssiResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int getXcapStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int setupXcapUserAgentStringResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int getBarringCallsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int setBarringCallsResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int onUssiInd(unsigned int slotId,
                     int indicationType, int token, RIL_Errno e, const void *response,
                     size_t responseLen);
int onVolteSubscriptionInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
// M: IMS_SS end @}

// M: IMS_ViLTE start @{
int imsVtDialResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int vtDialWithSipUriResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int videoCallAcceptResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int videoRingtoneEventResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int videoCapabilityIndicatorInd(unsigned int slotId,
                                       int indicationType, int token, RIL_Errno e,
                                       const void *response, size_t responseLen);
int videoRingtoneEventInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int mdInternetUsageIndication(unsigned int slotId, int indicationType,
                      int token, RIL_Errno e, const void *response, size_t responseLen);
// M: IMS_ViLTE end @}

// M: IMS_CC start @{
int dialWithSipUriResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int imsEctCommandResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int pullCallResponse(unsigned int slotId, android::ClientId clientId __unused,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responselen);
int eccRedialApproveResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int conferenceDialResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int forceReleaseCallResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int controlCallResponse(unsigned int slotId, android::ClientId clientId __unused, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int controlImsConferenceCallMemberResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int setSipHeaderResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int setSipHeaderReportResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int setImsCallModeResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int setCallAdditionalInfoResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen);
int ectIndicationInd(unsigned int slotId,
                            int indicationType, int token, RIL_Errno e,
                            const void *response, size_t responseLen);
int callInfoIndicationInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e,
                                 const void *response, size_t responseLen);
int callmodChangeIndicatorInd(unsigned int slotId,
                                    int indicationType, int token, RIL_Errno e,
                                    const void *response, size_t responseLen);
int econfResultIndicationInd(unsigned int slotId,
                                    int indicationType, int token, RIL_Errno e,
                                    const void *response, size_t responseLen);
int sipCallProgressIndicatorInd(unsigned int slotId,
                                       int indicationType, int token, RIL_Errno e,
                                       const void *response, size_t responseLen);
int onImsConferenceInfoIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen);
int onLteMessageWaitingIndication(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen);
int imsDialogIndicationInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int noEmergencyCallbackModeInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e, const void *response,
                                 size_t responselen);
int redialEmergencyIndication(unsigned int slotId, int indicationType,
                      int token, RIL_Errno e, const void *response, size_t responseLen);
int speechCodecInfoInd(unsigned int slotId, int indicationType,
                       int token, RIL_Errno e, const void *response, size_t responseLen);
int imsEventPackageIndicationInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e,
                                 const void *response, size_t responseLen);
int sipHeaderReportInd(unsigned int slotId,
    int indicationType, int token, RIL_Errno e, const void *response, size_t responselen);
int callRatIndication(unsigned int slotId,
    int indicationType, int token, RIL_Errno e, const void *response, size_t responselen);
int emergencyBearerInfoInd(unsigned int slotId, int indicationType,
                      int token, RIL_Errno e, const void *response, size_t responseLen);

int incomingCallSipInviteInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen);

// M: IMS_CC end @}

// M: IMS_CONFIG  start @{
int setImscfgResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int getImscfgResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int setModemImsCfgResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int setImsCfgFeatureValueResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int getImsCfgFeatureValueResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int setImsCfgProvisionValueResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int getImsCfgProvisionValueResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int getImsCfgResourceCapValueResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int setWfcProfileResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
int setVoiceDomainPreferenceResponse(unsigned int slotId, android::ClientId clientId __unused,
                            int responseType, int serial, RIL_Errno e,
                            const void *response,
                            size_t responselen);
int getVoiceDomainPreferenceResponse(unsigned int slotId, android::ClientId clientId __unused,
                            int responseType, int serial, RIL_Errno e,
                            const void *response,
                            size_t responselen);
int imsCfgDynamicImsSwitchCompleteInd(unsigned int slotId,
                                             int indicationType, int token, RIL_Errno e,
                                             const void *response,
                                             size_t responseLen);
int imsCfgFeatureChangedInd(unsigned int slotId,
                                   int indicationType, int token, RIL_Errno e, const void *response,
                                   size_t responseLen);
int imsCfgConfigChangedInd(unsigned int slotId,
                                   int indicationType, int token, RIL_Errno e, const void *response,
                                   size_t responseLen);
int imsCfgConfigLoadedInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e,
                                 const void *response,
                                 size_t responseLen);
// M: IMS_CONFIG end @}

// M: IMS_WFC start @{
int clearMwiResponseAndIndications(unsigned int slotId);
int setWifiEnabledResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int setWifiAssociatedResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int setWfcConfigResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno err, const void *response, size_t responseLen);
int getWfcConfigResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno err, const void *response, size_t responseLen);
int setWifiSignalLevelResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen);
int setWifiIpAddressResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int setLocationInfoResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int setEmergencyAddressIdResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen);
int setNattKeepAliveStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen);
int setWifiPingResultResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int notifyEPDGScreenStateResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen);
int querySsacStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int onWifiMonitoringThreshouldChanged(unsigned int slotId, int indicationType,
        int token, RIL_Errno err, const void *response, size_t responseLen);
int onWifiPdnActivate(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onWfcPdnError(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onPdnHandover(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onWifiRoveout(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onLocationRequest(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onWfcPdnStateChanged(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onNattKeepAliveChanged(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onWifiPingRequest(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onWifiPdnOOS(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onWifiLock(unsigned int slotId, int indicationType,
        int token, RIL_Errno err, const void *response, size_t responselen);
int onSsacStatus(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
// M: IMS_WFC end @}

}

#include <aidl/android/hardware/radio/ims/BnRadioIms.h>

namespace android::hardware::radio::ims {
class RadioIms : public aidl::android::hardware::radio::ims::BnRadioIms {
    ::ndk::ScopedAStatus setSrvccCallInfo(int32_t serial,
                const std::vector<::aidl::android::hardware::radio::ims::SrvccCall>& srvccCalls) override;
    ::ndk::ScopedAStatus updateImsRegistrationInfo(int32_t serial,
                const ::aidl::android::hardware::radio::ims::ImsRegistration& imsRegistration) override;
    ::ndk::ScopedAStatus startImsTraffic(int32_t serial, int token,
                ::aidl::android::hardware::radio::ims::ImsTrafficType imsTrafficType,
                ::aidl::android::hardware::radio::AccessNetwork accessNetworkType,
                ::aidl::android::hardware::radio::ims::ImsCall::Direction trafficDirection) override;
    ::ndk::ScopedAStatus stopImsTraffic(int32_t serial, int32_t token) override;
    ::ndk::ScopedAStatus triggerEpsFallback(int32_t serial,
                ::aidl::android::hardware::radio::ims::EpsFallbackReason reason) override;
    ::ndk::ScopedAStatus setResponseFunctions(
                const std::shared_ptr<::aidl::android::hardware::radio::ims::IRadioImsResponse>& radioImsResponse,
                const std::shared_ptr<::aidl::android::hardware::radio::ims::IRadioImsIndication>& radioImsIndication) override;
    ::ndk::ScopedAStatus sendAnbrQuery(int32_t serial,
                ::aidl::android::hardware::radio::ims::ImsStreamType mediaType,
                ::aidl::android::hardware::radio::ims::ImsStreamDirection direction,
                int32_t bitsPerSecond) override;
    ::ndk::ScopedAStatus updateImsCallStatus(int32_t serial,
                const std::vector<::aidl::android::hardware::radio::ims::ImsCall>& imsCalls) override;

  public:
    RadioIms(int slotid);
    int mSlotId;

  private:
    bool isValidSlotId(int slotId) {
        return ((slotId >= 0) && (slotId < MAX_SIM_COUNT));
    }
};
}

#include <aidl/vendor/mediatek/hardware/mtkradioex/ims/BnMtkRadioExIms.h>

namespace vendor::mediatek::hardware::mtkradioex::ims {

class MtkRadioIms : public aidl::vendor::mediatek::hardware::mtkradioex::ims::BnMtkRadioExIms {
public:
// M: IMS_DATA start @{
    ::ndk::ScopedAStatus imsBearerStateConfirm(int32_t serial, int32_t aid, int32_t action, int32_t status, int32_t clientId) override;
    ::ndk::ScopedAStatus setImsBearerNotification(int32_t serial, int32_t enable, int32_t clientId) override;
// M: IMS_DATA end @}
// M: IMS_SMS start @{
    ::ndk::ScopedAStatus acknowledgeLastIncomingCdmaSmsEx(int32_t serial, const ::aidl::android::hardware::radio::messaging::CdmaSmsAck& smsAck, int32_t clientId) override;
    ::ndk::ScopedAStatus acknowledgeLastIncomingGsmSmsEx(int32_t serial, bool success, ::aidl::android::hardware::radio::messaging::SmsAcknowledgeFailCause cause, int32_t clientId) override;
    ::ndk::ScopedAStatus sendImsSmsEx(int32_t serial, const ::aidl::android::hardware::radio::messaging::ImsSmsMessage& message, int32_t clientId) override;
// M: IMS_SMS end @}
// M: IMS_RTT start @{
    ::ndk::ScopedAStatus rttModifyRequestResponse(int32_t serial, int32_t callId, int32_t result, int32_t clientId) override;
    ::ndk::ScopedAStatus sendRttModifyRequest(int32_t serial, int32_t callId, int32_t newMode, int32_t clientId) override;
    ::ndk::ScopedAStatus sendRttText(int32_t serial, int32_t callId, int32_t lenOfString, const std::string& text, int32_t clientId) override;
    ::ndk::ScopedAStatus setRttMode(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus toggleRttAudioIndication(int32_t serial, int32_t callId, int32_t audio, int32_t clientId) override;
// M: IMS_RTT end @}
// M: IMS_BASE start @{
    ::ndk::ScopedAStatus queryVopsStatus(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus setImsRegistrationReport(int32_t serial, int32_t clientId) override;
// M: IMS_BASE end @}
// M: IMS_SS start @{
    ::ndk::ScopedAStatus cancelUssi(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getXcapStatus(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus sendUssi(int32_t serial, const std::string& ussiString, int32_t clientId) override;
    ::ndk::ScopedAStatus setupXcapUserAgentString(int32_t serial, const std::string& userAgent, int32_t clientId) override;
    ::ndk::ScopedAStatus getBarringCalls(int32_t serial, int32_t serviceClass, int32_t clientId) override;
    ::ndk::ScopedAStatus setBarringCalls(int32_t serial,
            const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::ims::ImsBarringCall>& calls, int32_t clientId) override;
// M: IMS_SS end @}
// M: IMS_ViLTE start @{
    ::ndk::ScopedAStatus videoCallAccept(int32_t serial, int32_t videoMode, int32_t callId, int32_t clientId) override;
    ::ndk::ScopedAStatus videoRingtoneEventRequest(int32_t serial, const std::vector<std::string>& event, int32_t clientId) override;
    ::ndk::ScopedAStatus vtDial(int32_t serial, const ::aidl::android::hardware::radio::voice::Dial& dialInfo, int32_t clientId) override;
    ::ndk::ScopedAStatus vtDialWithSipUri(int32_t serial, const std::string& address, int32_t clientId) override;
// M: IMS_ViLTE end @}
// M: IMS_CC start @{
    ::ndk::ScopedAStatus conferenceDial(int32_t serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::voice::ConferenceDial& dailInfo, int32_t clientId) override;
    ::ndk::ScopedAStatus controlCall(int32_t serial, int32_t controlType, int32_t callId, int32_t clientId) override;
    ::ndk::ScopedAStatus controlImsConferenceCallMember(int32_t serial, int32_t controlType, int32_t confCallId, const std::string& address, int32_t callId, int32_t clientId) override;
    ::ndk::ScopedAStatus dialWithSipUri(int32_t serial, const std::string& address, int32_t clientId) override;
    ::ndk::ScopedAStatus eccRedialApprove(int32_t serial, int32_t approve, int32_t callId, int32_t clientId) override;
    ::ndk::ScopedAStatus forceReleaseCall(int32_t serial, int32_t callId, int32_t clientId) override;
    ::ndk::ScopedAStatus imsEctCommand(int32_t serial, const std::string& number, int32_t type, int32_t clientId) override;
    ::ndk::ScopedAStatus pullCall(int32_t serial, const std::string& target, bool isVideoCall, int32_t clientId) override;
    ::ndk::ScopedAStatus setCallAdditionalInfo(int32_t serial, const std::vector<std::string>& info, int32_t clientId) override;
    ::ndk::ScopedAStatus setImsCallMode(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus setSipHeader(int32_t serial, const std::vector<std::string>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus setSipHeaderReport(int32_t serial, const std::vector<std::string>& data, int32_t clientId) override;
// M: IMS_CC end @}
// M: IMS_CONFIG  start @{
    ::ndk::ScopedAStatus getImsCfgFeatureValue(int32_t serial, int32_t featureId, int32_t network, int32_t clientId) override;
    ::ndk::ScopedAStatus getImsCfgProvisionValue(int32_t serial, int32_t configId, int32_t clientId) override;
    ::ndk::ScopedAStatus getImsCfgResourceCapValue(int32_t serial, int32_t featureId, int32_t clientId) override;
    ::ndk::ScopedAStatus getVoiceDomainPreference(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus setImsCfgFeatureValue(int32_t serial, int32_t featureId, int32_t network, int32_t value, int32_t isLast, int32_t clientId) override;
    ::ndk::ScopedAStatus setImsCfgProvisionValue(int32_t serial, int32_t configId, const std::string& value, int32_t clientId) override;
    ::ndk::ScopedAStatus setImscfg(int32_t serial, bool volteEnable, bool vilteEnable, bool vowifiEnable, bool viwifiEnable, bool smsEnable, bool eimsEnable, int32_t clientId) override;
    ::ndk::ScopedAStatus getImscfg(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus setVoiceDomainPreference(int32_t serial, int32_t vdp, int32_t clientId) override;
    ::ndk::ScopedAStatus setWfcProfile(int32_t serial, int32_t wfcPreference, int32_t clientId) override;
    ::ndk::ScopedAStatus setModemImsCfg(int32_t serial, const std::string& keys, const std::string& values, int32_t type, int32_t clientId) override;
// M: IMS_CONFIG end @}
// M: IMS_WFC start @{
    ::ndk::ScopedAStatus querySsacStatus(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getWfcConfig(int32_t serial, int32_t setting, int32_t clientId) override;
    ::ndk::ScopedAStatus notifyEPDGScreenState(int32_t serial, int32_t state, int32_t clientId) override;
    ::ndk::ScopedAStatus setEmergencyAddressId(int32_t serial, const std::string& aid, int32_t clientId) override;
    ::ndk::ScopedAStatus setLocationInfo(int32_t serial, const std::vector<std::string>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus setNattKeepAliveStatus(int32_t serial, const std::string& ifName, bool enable, const std::string& srcIp, int32_t srcPort, const std::string& dstIp, int32_t dstPort,
            int32_t clientId) override;
    ::ndk::ScopedAStatus setWfcConfig(int32_t serial, int32_t setting, const std::string& ifName, const std::string& value, int32_t clientId) override;
    ::ndk::ScopedAStatus setWifiAssociated(int32_t serial, const std::vector<std::string>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus setWifiEnabled(int32_t serial, const std::string& ifName, int32_t isWifiEnabled, int32_t isFlightModeOn, int32_t clientId) override;
    ::ndk::ScopedAStatus setWifiIpAddress(int32_t serial, const std::vector<std::string>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus setWifiPingResult(int32_t serial, int32_t rat, int32_t latency, int32_t pktloss, int32_t clientId) override;
    ::ndk::ScopedAStatus setWifiSignalLevel(int32_t serial, int32_t rssi, int32_t snr, int32_t clientId) override;
    ::ndk::ScopedAStatus setResponseFunctionsMwi(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::mwi::IMwiRadioResponse>& radioResponse,
        const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::mwi::IMwiRadioIndication>& radioIndication) override;
// M: IMS_WFC end @}
    ::ndk::ScopedAStatus responseAcknowledgementMtk() override;
    ::ndk::ScopedAStatus setResponseFunctionsMtk(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::ims::IMtkRadioExImsResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::ims::IMtkRadioExImsIndication>& radioIndication) override;

    public:
        MtkRadioIms(int slotid);
        int mSlotId;

    private:
        bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
            if (client < 0) {
                return false;
            }
            return clientId == ((unsigned int)client);
        }

        bool isValidSlotId(int slotId) {
            return ((slotId >= 0) && (slotId < MAX_SIM_COUNT));
        }
};

}
