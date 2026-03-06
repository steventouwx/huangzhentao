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

typedef enum {
    SIM_HOTSWAP_PLUG_IN,
    SIM_HOTSWAP_PLUG_OUT,
    SIM_HOTSWAP_RECOVERY,
    SIM_HOTSWAP_MISSING,
    SIM_HOTSWAP_TRAY_PLUG_IN,
    SIM_HOTSWAP_TRAY_PLUG_OUT,
    SIM_HOTSWAP_COMMONSLOT_NO_CHANGED,
} SimHotSwap;

namespace radioSim {

// response
int areUiccApplicationsEnabledResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int changeIccPin2ForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int changeIccPinForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int enableUiccApplicationsResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int getAllowedCarriersResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int getCdmaSubscriptionResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int getCdmaSubscriptionSourceResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int getFacilityLockForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int getIccCardStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int getIMSIForAppResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                        int serial, RIL_Errno e, const void *response, size_t responselen);

int getSimPhonebookRecordsResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int getSimPhonebookCapacityResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int iccCloseLogicalChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int iccCloseLogicalChannelWithSessionInfoResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int iccIOForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int iccOpenLogicalChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int iccTransmitApduBasicChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int iccTransmitApduLogicalChannelResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int reportStkServiceIsRunningResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int requestIccSimAuthenticationResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int sendEnvelopeResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int sendEnvelopeWithStatusResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int sendTerminalResponseToSimResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int setAllowedCarriersResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int setCarrierInfoForImsiEncryptionResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int setCdmaSubscriptionSourceResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int setFacilityLockForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int setSimCardPowerResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int setUiccSubscriptionResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);

int supplyIccPin2ForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int supplyIccPinForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);


int supplyIccPuk2ForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);


int supplyIccPukForAppResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

int supplySimDepersonalizationResponse(unsigned int slotId, android::ClientId clientId __unused,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int updateSimPhonebookRecordsResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);

// indication
int carrierInfoForImsiEncryption(unsigned int slotId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int cdmaSubscriptionSourceChangedInd(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e,
                        const void *response, size_t responselen);

int simPhonebookChanged(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int simPhonebookRecordsReceived(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int simRefreshInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);


int simStatusChangedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int stkEventNotifyInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);


int stkProactiveCommandInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int stkSessionEndInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int subscriptionStatusChangedInd(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responselen);

int uiccApplicationsEnablementChanged(unsigned int slotId, int indicationType, int token,
                        RIL_Errno e, const void *response, size_t responseLen);

int requestIsimAuthenticationResponse(unsigned int slotId,
                        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);
int clearSimResponseAndIndications(unsigned int slotId);
}


namespace mtkRadioExSim {

// MTK-START: SIM
int onCardDetectedInd(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

/// M: SIM: Iccid changed indication
int iccidChangedInd(unsigned int slotId, int indicationType, int token,
                        RIL_Errno err, const void *response, size_t responseLen);

int getATRResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int getIccidResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
                        RIL_Errno e, const void *response, size_t responseLen);

int setSimPowerResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int onVirtualSimStatusChanged(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

int onImeiLock(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

int onImsiRefreshDone(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

int getIccCardStatusResponse_1_2(unsigned int slotId, android::ClientId clientId,
                        int indicationType, int token, RIL_Errno err, const void *response,
                        size_t responseLen);
// MTK-END

// MTK-START: SIM HOT SWAP / SIM RECOVERY
int onSimPlugIn(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

int onSimPlugOut(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

int onSimMissing(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

int onSimRecovery(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);
// MTK-END


// MTK-START: pSIM <-> eSIM switch support
int getSimTypeInfoResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int switchSimTypeResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int simTypeInfoChangedInd(unsigned int slotId, int indicationType, int token,
                        RIL_Errno err, const void *response, size_t responseLen);
// MTK-END


int activateUiccCardRsp(unsigned int slotId, android::ClientId clientId, int responseType,
                        int serial, RIL_Errno e, const void *response, size_t responseLen);

int deactivateUiccCardRsp(unsigned int slotId, android::ClientId clientId, int responseType,
                        int serial, RIL_Errno e, const void *response, size_t responseLen);

int getCurrentUiccCardProvisioningStatusRsp(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responseLen);

// MTK-START: SIM POWER
int onSimPowerChangedInd(unsigned int slotId, int indicationType, int token,
                        RIL_Errno e, const void *response, size_t responseLen);
// MTK-END

// MTK-START: SIM COMMON SLOT
int onSimTrayPlugIn(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);

int onSimCommonSlotNoChanged(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responseLen);
// MTK-END

// External SIM [Start]
int vsimNotificationResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                        int serial, RIL_Errno e, const void *response, size_t responseLen);

int vsimOperationResponse(unsigned int slotId, android::ClientId clientId, int responseType,
                        int serial, RIL_Errno e, const void *response, size_t responseLen);

int onVsimEventIndication(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responselen);
// External SIM [End]

// PHB START
int queryPhbStorageInfoResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int writePhbEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readPhbEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int queryUPBCapabilityResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int editUPBEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int deleteUPBEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readUPBGasListResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readUPBGrpEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int writeUPBGrpEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int getPhoneBookStringsLengthResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int getPhoneBookMemStorageResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int setPhoneBookMemStorageResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readPhoneBookEntryExtResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int writePhoneBookEntryExtResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int queryUPBAvailableResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readUPBEmailEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readUPBSneEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readUPBAnrEntryResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int readUPBAasListResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int phbReadyNotificationInd(unsigned int slotId,
                        int indicationType, int token, RIL_Errno e, const void *response,
                        size_t responselen);

int setPhonebookReadyResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);
// PHB END

// / M: STK, only to build pass {
int bipProactiveCommandInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int onStkMenuResetInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int handleStkCallSetupRequestFromSimWithResCodeResponse(unsigned int slotId,
                        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responselen);
// / M: STK }

// MTK-START: SIM ME LOCK
int queryNetworkLockResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int setNetworkLockResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e,
                        const void *response, size_t responseLen);

int supplyDepersonalizationResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
// MTK-END

// MTK-START: SIM SLOT LOCK
int smlSlotLockInfoChangedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responseLen);

int supplyDeviceNetworkDepersonalizationResponse(unsigned int slotId, android::ClientId clientId,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responselen);
// MTK-END

// SIM RSU
int onRsuSimLockEvent(unsigned int slotId, int indicationType, int token, RIL_Errno e,
                        const void *response, size_t responseLen);

int sendRsuRequestResponse(unsigned int slotId, android::ClientId clientId __unused,
                        int responseType, int serial, RIL_Errno e, const void *response,
                        size_t responseLen);

int onRsuEvent(unsigned int slotId, int indicationType, int token, RIL_Errno e,
                        const void *response, size_t responseLen);
int clearMtkRsuResponseAndIndications(unsigned int slotId);
// MTK-END
int clearMtkSeResponseAndIndications(unsigned int slotId);
int clearMtkSimResponseAndIndications(unsigned int slotId);
}

#include <aidl/android/hardware/radio/sim/BnRadioSim.h>


namespace android::hardware::radio::sim {

class RadioSim : public aidl::android::hardware::radio::sim::BnRadioSim {
public:
    ::ndk::ScopedAStatus areUiccApplicationsEnabled(int32_t serial) override;
    ::ndk::ScopedAStatus changeIccPin2ForApp(int32_t serial, const std::string& oldPin2,
                                             const std::string& newPin2,
                                             const std::string& aid) override;
    ::ndk::ScopedAStatus changeIccPinForApp(int32_t serial, const std::string& oldPin,
                                            const std::string& newPin,
                                            const std::string& aid) override;
    ::ndk::ScopedAStatus enableUiccApplications(int32_t serial, bool enable) override;
    ::ndk::ScopedAStatus getAllowedCarriers(int32_t serial) override;
    ::ndk::ScopedAStatus getCdmaSubscription(int32_t serial) override;
    ::ndk::ScopedAStatus getCdmaSubscriptionSource(int32_t serial) override;
    ::ndk::ScopedAStatus getFacilityLockForApp(int32_t serial, const std::string& facility,
                                               const std::string& password, int32_t serviceClass,
                                               const std::string& appId) override;
    ::ndk::ScopedAStatus getIccCardStatus(int32_t serial) override;
    ::ndk::ScopedAStatus getImsiForApp(int32_t serial, const std::string& aid) override;
    ::ndk::ScopedAStatus getSimPhonebookCapacity(int32_t serial) override;
    ::ndk::ScopedAStatus getSimPhonebookRecords(int32_t serial) override;
    ::ndk::ScopedAStatus iccCloseLogicalChannel(int32_t serial, int32_t channelId) override;
    ::ndk::ScopedAStatus iccCloseLogicalChannelWithSessionInfo(
            int32_t serial,
            const ::aidl::android::hardware::radio::sim::SessionInfo& sessionInfo) override;
    ::ndk::ScopedAStatus iccIoForApp(
            int32_t serial, const ::aidl::android::hardware::radio::sim::IccIo& iccIo) override;
    ::ndk::ScopedAStatus iccOpenLogicalChannel(int32_t serial, const std::string& aid,
                                               int32_t p2) override;
    ::ndk::ScopedAStatus iccTransmitApduBasicChannel(
            int32_t serial, const ::aidl::android::hardware::radio::sim::SimApdu& message) override;
    ::ndk::ScopedAStatus iccTransmitApduLogicalChannel(
            int32_t serial, const ::aidl::android::hardware::radio::sim::SimApdu& message) override;
    ::ndk::ScopedAStatus reportStkServiceIsRunning(int32_t serial) override;
    ::ndk::ScopedAStatus requestIccSimAuthentication(int32_t serial, int32_t authContext,
                                                     const std::string& authData,
                                                     const std::string& aid) override;
    ::ndk::ScopedAStatus responseAcknowledgement() override;
    ::ndk::ScopedAStatus sendEnvelope(int32_t serial, const std::string& command) override;
    ::ndk::ScopedAStatus sendEnvelopeWithStatus(int32_t serial,
                                                const std::string& contents) override;
    ::ndk::ScopedAStatus sendTerminalResponseToSim(int32_t serial,
                                                   const std::string& commandResponse) override;
    ::ndk::ScopedAStatus setAllowedCarriers(
            int32_t serial,
            const ::aidl::android::hardware::radio::sim::CarrierRestrictions& carriers,
            ::aidl::android::hardware::radio::sim::SimLockMultiSimPolicy multiSimPolicy) override;
    ::ndk::ScopedAStatus setCarrierInfoForImsiEncryption(
            int32_t serial,
            const ::aidl::android::hardware::radio::sim::ImsiEncryptionInfo& imsiEncryptionInfo)
            override;
    ::ndk::ScopedAStatus setCdmaSubscriptionSource(
            int32_t serial,
            ::aidl::android::hardware::radio::sim::CdmaSubscriptionSource cdmaSub) override;
    ::ndk::ScopedAStatus setFacilityLockForApp(  //
            int32_t serial, const std::string& facility, bool lockState, const std::string& passwd,
            int32_t serviceClass, const std::string& appId) override;
    ::ndk::ScopedAStatus setResponseFunctions(
            const std::shared_ptr<::aidl::android::hardware::radio::sim::IRadioSimResponse>&
                    radioSimResponse,
            const std::shared_ptr<::aidl::android::hardware::radio::sim::IRadioSimIndication>&
                    radioSimIndication) override;
    ::ndk::ScopedAStatus setSimCardPower(
            int32_t serial, ::aidl::android::hardware::radio::sim::CardPowerState powerUp) override;
    ::ndk::ScopedAStatus setUiccSubscription(
            int32_t serial,
            const ::aidl::android::hardware::radio::sim::SelectUiccSub& uiccSub) override;
    ::ndk::ScopedAStatus supplyIccPin2ForApp(int32_t serial, const std::string& pin2,
                                             const std::string& aid) override;
    ::ndk::ScopedAStatus supplyIccPinForApp(int32_t serial, const std::string& pin,
                                            const std::string& aid) override;
    ::ndk::ScopedAStatus supplyIccPuk2ForApp(int32_t serial, const std::string& puk2,
                                             const std::string& pin2,
                                             const std::string& aid) override;
    ::ndk::ScopedAStatus supplyIccPukForApp(int32_t serial, const std::string& puk,
                                            const std::string& pin,
                                            const std::string& aid) override;
    ::ndk::ScopedAStatus supplySimDepersonalization(
            int32_t serial, ::aidl::android::hardware::radio::sim::PersoSubstate persoType,
            const std::string& controlKey) override;
    ::ndk::ScopedAStatus updateSimPhonebookRecords(
            int32_t serial,
            const ::aidl::android::hardware::radio::sim::PhonebookRecordInfo& recordInfo) override;

    public:
        RadioSim(int slotid);
        int mSlotId;
};

}  // namespace android::hardware::radio::sim


#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/BnMtkRadioExSim.h>

namespace vendor::mediatek::hardware::mtkradioex::sim {
class MtkRadioSim : public aidl::vendor::mediatek::hardware::mtkradioex::sim::BnMtkRadioExSim {
public:
    ::ndk::ScopedAStatus getSimTypeInfo(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus switchSimType(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus getIccid(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus activateUiccCard(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus deactivateUiccCard(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus deleteUPBEntry(int32_t serial, int32_t entryType, int32_t adnIndex, int32_t entryIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus doGeneralSimAuthentication(int32_t serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::SimAuthStructure& simAuth, int32_t clientId) override;
    ::ndk::ScopedAStatus editUPBEntry(int32_t serial, const std::vector<std::string>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus getATR(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getCurrentUiccCardProvisioningStatus(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getPhoneBookMemStorage(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getPhoneBookStringsLength(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus handleStkCallSetupRequestFromSimWithResCode(int32_t serial, int32_t resultCode, int32_t clientId) override;
    ::ndk::ScopedAStatus queryNetworkLock(int32_t serial, int32_t category, int32_t clientId) override;
    ::ndk::ScopedAStatus queryPhbStorageInfo(int32_t serial, int32_t type, int32_t clientId) override;
    ::ndk::ScopedAStatus queryUPBAvailable(int32_t serial, int32_t eftype, int32_t fileIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus readPhbEntry(int32_t serial, int32_t type, int32_t bIndex, int32_t eIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus readPhoneBookEntryExt(int32_t serial, int32_t index1, int32_t index2, int32_t clientId) override;
    ::ndk::ScopedAStatus readUPBAasList(int32_t serial, int32_t startIndex, int32_t endIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus readUPBAnrEntry(int32_t serial, int32_t adnIndex, int32_t fileIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus readUPBEmailEntry(int32_t serial, int32_t adnIndex, int32_t fileIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus readUPBGasList(int32_t serial, int32_t startIndex, int32_t endIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus readUPBGrpEntry(int32_t serial, int32_t adnIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus readUPBSneEntry(int32_t serial, int32_t adnIndex, int32_t fileIndex, int32_t clientId) override;
    ::ndk::ScopedAStatus sendVsimNotification(int32_t serial, int32_t transactionId, int32_t eventId, int32_t simType, int32_t clientId)  override;
    ::ndk::ScopedAStatus sendVsimOperation(int32_t serial, int32_t transactionId, int32_t eventId, int32_t result, int32_t dataLength, const std::vector<uint8_t>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus setNetworkLock(int32_t serial, int32_t category, int32_t lockop, const std::string& password, const std::string& data_imsi, const std::string& gid1,
            const std::string& gid2, int32_t clientId) override;
    ::ndk::ScopedAStatus setPhoneBookMemStorage(int32_t serial, const std::string& storage, const std::string& password, int32_t clientId) override;
    ::ndk::ScopedAStatus setPhonebookReady(int32_t serial, int32_t ready, int32_t clientId) override;
    ::ndk::ScopedAStatus setSimPower(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus writePhbEntry(int32_t serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryStructure& phbEntry, int32_t clientId) override;
    ::ndk::ScopedAStatus writePhoneBookEntryExt(int32_t serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::PhbEntryExt& phbEntryExt, int32_t clientId) override;
    ::ndk::ScopedAStatus writeUPBGrpEntry(int32_t serial, int32_t adnIndex, const std::vector<int32_t>& grpIds, int32_t clientId) override;
    ::ndk::ScopedAStatus supplyDepersonalization(int32_t serial, const std::string& netPin, int32_t type, int32_t clientId) override;
    ::ndk::ScopedAStatus supplyDeviceNetworkDepersonalization(int32_t serial, const std::string& pwd, int32_t clientId) override;
    ::ndk::ScopedAStatus sendRsuRequest(int32_t serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::rsu::RsuRequestInfo& rri, int32_t clientId) override;
    ::ndk::ScopedAStatus queryUPBCapability(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus responseAcknowledgementMtk() override;
    ::ndk::ScopedAStatus setResponseFunctionsMtk(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::sim::IMtkRadioExSimResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::sim::IMtkRadioExSimIndication>& radioIndication) override;
    ::ndk::ScopedAStatus setResponseFunctionsRsu(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::rsu::IRsuRadioResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::rsu::IRsuRadioIndication>& radioIndication) override;
    ::ndk::ScopedAStatus setResponseFunctionsSE(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::se::ISERadioResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::se::ISERadioIndication>& radioIndication) override;

    public:
        MtkRadioSim(int32_t slotid);
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

