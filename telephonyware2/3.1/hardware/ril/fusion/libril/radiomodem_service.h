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

namespace radioModem {
// response
int enableModemResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno err, const void *response, size_t responseLen);


int getBasebandVersionResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responselen);

int getDeviceIdentityResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responselen);

int getHardwareConfigResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responseLen);

int getModemActivityInfoResponse(unsigned int slotId, android::ClientId clientId,
                                int responseType, int serial, RIL_Errno e,
                                const void *response, size_t responselen);

int getModemStackStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);

int getRadioCapabilityResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen);

int nvReadItemResponse(unsigned int slotId, android::ClientId clientId,
                      int responseType, int serial, RIL_Errno e,
                      const void *response, size_t responselen);

int nvResetConfigResponse(unsigned int slotId, android::ClientId clientId,
                         int responseType, int serial, RIL_Errno e,
                         const void *response, size_t responselen);

int nvWriteCdmaPrlResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen);

int nvWriteItemResponse(unsigned int slotId, android::ClientId clientId,
                       int responseType, int serial, RIL_Errno e,
                       const void *response, size_t responselen);

int requestShutdownResponse(unsigned int slotId, android::ClientId clientId,
                           int responseType, int serial, RIL_Errno e,
                           const void *response, size_t responselen);

int sendDeviceStateResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responselen);

int setRadioCapabilityResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen);

int setRadioPowerResponse(unsigned int slotId, android::ClientId clientId,
                         int responseType, int serial, RIL_Errno e, const void *response,
                         size_t responselen);

int getImeiResponse(unsigned int slotId, android::ClientId clientId,
                         int responseType, int serial, RIL_Errno e, const void *response,
                         size_t responselen);
// indication
int hardwareConfigChangedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen);

int modemResetInd(unsigned int slotId,
                  int indicationType, int token, RIL_Errno e, const void *response,
                  size_t responselen);


int radioCapabilityIndicationInd(unsigned int slotId,
                                 int indicationType, int token, RIL_Errno e, const void *response,
                                 size_t responselen);

int radioStateChangedInd(unsigned int slotId,
                          int indicationType, int token, RIL_Errno e, const void *response,
                          size_t responseLen);

int imsRadioStateChangedInd(unsigned int slotId,
                          int indicationType, int token, RIL_Errno e, const void *response,
                          size_t responseLen);

int rilConnectedInd(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responselen);

int onImeiMappingChanged(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responselen);

int clearModemResponseAndIndications(unsigned int slotId);
}  // namespace radioModem


#include <aidl/android/hardware/radio/modem/BnRadioModem.h>

namespace android::hardware::radio::modem {

class RadioModem : public aidl::android::hardware::radio::modem::BnRadioModem {
public:
    ::ndk::ScopedAStatus enableModem(int32_t serial, bool on) override;
    ::ndk::ScopedAStatus getBasebandVersion(int32_t serial) override;
    ::ndk::ScopedAStatus getDeviceIdentity(int32_t serial) override;
    ::ndk::ScopedAStatus getHardwareConfig(int32_t serial) override;
    ::ndk::ScopedAStatus getModemActivityInfo(int32_t serial) override;
    ::ndk::ScopedAStatus getModemStackStatus(int32_t serial) override;
    ::ndk::ScopedAStatus getRadioCapability(int32_t serial) override;
    ::ndk::ScopedAStatus nvReadItem(
            int32_t serial, ::aidl::android::hardware::radio::modem::NvItem itemId) override;
    ::ndk::ScopedAStatus nvResetConfig(
            int32_t serial, ::aidl::android::hardware::radio::modem::ResetNvType type) override;
    ::ndk::ScopedAStatus nvWriteCdmaPrl(int32_t serial, const std::vector<uint8_t>& prl) override;
    ::ndk::ScopedAStatus nvWriteItem(
            int32_t serial, const ::aidl::android::hardware::radio::modem::NvWriteItem& i) override;
    ::ndk::ScopedAStatus requestShutdown(int32_t serial) override;
    ::ndk::ScopedAStatus responseAcknowledgement() override;
    ::ndk::ScopedAStatus sendDeviceState(
            int32_t serial, ::aidl::android::hardware::radio::modem::DeviceStateType stateType,
            bool state) override;
    ::ndk::ScopedAStatus setRadioCapability(
            int32_t s, const ::aidl::android::hardware::radio::modem::RadioCapability& rc) override;
    ::ndk::ScopedAStatus setRadioPower(int32_t serial, bool powerOn, bool forEmergencyCall,
                                       bool preferredForEmergencyCall) override;
    ::ndk::ScopedAStatus setResponseFunctions(
            const std::shared_ptr<::aidl::android::hardware::radio::modem::IRadioModemResponse>&
                    radioModemResponse,
            const std::shared_ptr<::aidl::android::hardware::radio::modem::IRadioModemIndication>&
                    radioModemIndication) override;
    ::ndk::ScopedAStatus getImei(int32_t serial) override;

  public:
    RadioModem(unsigned int slotid);
    unsigned int mSlotId;
};

}  // namespace android::hardware::radio::modem


namespace mtkRadioExModem {
int routeCertificateResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int routeAuthMessageResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int abortCertificateResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int enableCapabilityResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int restartRILDResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen);
int sendAtciResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responseLen);
int sendEmbmsAtCommandResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responseLen);
int sendRequestRawResponse(unsigned int slotId, android::ClientId clientId,
                           int responseType, int serial, RIL_Errno e,
                           const void *response, size_t responseLen);
int sendRequestStringsResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen);
int sendSarIndicatorResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int setModemPowerResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen);
int setTrmResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen);
int setTxPowerResponse(unsigned int slotId, android::ClientId clientId __unused,
                             int responseType, int serial, RIL_Errno e,
                             const void *response, size_t responseLen);
int setTxPowerStatusResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);
int setVendorSettingResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen);
int triggerModeSwitchByEccResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responseLen);
int sendWifiEnabledResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int sendWifiAssociatedResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int sendWifiIpAddressResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int registerCellQltyReportResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno err, const void *response, size_t responseLen);
int getEngineeringModeInfoResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen);
int modifyModemTypeResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen);
int runGbaAuthenticationResponse(unsigned int slotId, android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen);

// indication
int onAtciInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int oemHookRawInd(unsigned int slotId,
                  int indicationType, int token, RIL_Errno e, const void *response,
                  size_t responselen);
int onTxPowerIndication(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onTxPowerStatusIndication(unsigned int slotId, int indicationType,
        int token, RIL_Errno err, const void *response, size_t responseLen);
int onCellularQualityChangedInd(unsigned int slotId, int indicationType, int token,
        RIL_Errno err, const void *response, size_t responseLen);
int onDsbpStateChanged(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int clearMtkModemResponseAndIndications(unsigned int slotId);
int clearMtkCapResponse(unsigned int slotId);
int clearMtkEmResponseAndIndications(unsigned int slotId);
int clearMtkGbaResponse(unsigned int slotId);
//embms INDICATIONS
int eMBMSSessionStatusIndication(unsigned int slotId, int indicationType, int token, RIL_Errno e, const void *response, size_t responselen);
int eMBMSAtInfoIndication(unsigned int slotId, int indicationType, int token, RIL_Errno e, const void *response, size_t responselen);
int clearAssistResponseAndIndications(unsigned int slotId);
int syncAppEventStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void *response, size_t responselen);
}

// namespace mtkRadioExModem


#include <aidl/vendor/mediatek/hardware/mtkradioex/modem/BnMtkRadioExModem.h>

namespace vendor::mediatek::hardware::mtkradioex::modem {

class MtkRadioModem : public aidl::vendor::mediatek::hardware::mtkradioex::modem::BnMtkRadioExModem {
public:
    ::ndk::ScopedAStatus abortCertificate(int32_t serial, int32_t uid, int32_t clientId)override;
    ::ndk::ScopedAStatus enableCapability(int32_t serial, const std::string& id, int32_t uid, int32_t toActive, int32_t clientId) override;
    ::ndk::ScopedAStatus getEngineeringModeInfo(int32_t serial, int32_t index, int32_t clientId) override;
    ::ndk::ScopedAStatus modifyModemType(int32_t serial, int32_t applyType, int32_t modemType, int32_t clientId) override;
    ::ndk::ScopedAStatus restartRILD(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus routeAuthMessage(int32_t serial, int32_t uid, const std::vector<uint8_t>& msg, int32_t clientId) override;
    ::ndk::ScopedAStatus routeCertificate(int32_t serial, int32_t uid, const std::vector<uint8_t>& cert, const std::vector<uint8_t>& msg, int32_t clientId) override;
    ::ndk::ScopedAStatus sendAtciRequest(int32_t serial, const std::vector<uint8_t>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus sendEmbmsAtCommand(int32_t serial, const std::string& data, int32_t clientId) override;
    ::ndk::ScopedAStatus sendRequestRaw(int32_t serial, const std::vector<uint8_t>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus sendRequestStrings(int32_t serial, const std::vector<std::string>& data, int32_t clientId) override;
    ::ndk::ScopedAStatus sendSarIndicator(int32_t serial, int32_t sar_cmd_type, const std::string& sar_parameter, int32_t clientId) override;
    ::ndk::ScopedAStatus setMaxUlSpeed(int32_t serial, int32_t ulSpeed, int32_t clientId) override;
    ::ndk::ScopedAStatus setModemPower(int32_t serial, bool isOn, int32_t clientId) override;
    ::ndk::ScopedAStatus setTrm(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus setTxPower(int32_t serial, int32_t limitpower, int32_t clientId) override;
    ::ndk::ScopedAStatus setTxPowerStatus(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus setVendorSetting(int32_t serial, int32_t setting, const std::string& value, int32_t clientId) override;
    ::ndk::ScopedAStatus triggerModeSwitchByEcc(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus runGbaAuthentication(int32_t serial, const std::string& nafFqdn, const std::string& nafSecureProtocolId, bool forceRun, int32_t netId, int32_t clientId) override;
    ::ndk::ScopedAStatus sendWifiAssociated(int32_t serial, const std::string& ifName, int32_t associated, const std::string& ssid, const std::string& apMac, int32_t mtuSize,
            const std::string& ueMac, int32_t clientId) override;
    ::ndk::ScopedAStatus sendWifiEnabled(int32_t serial, const std::string& ifName, int32_t isWifiEnabled, int32_t clientId) override;

    ::ndk::ScopedAStatus sendWifiIpAddress(int32_t serial, const std::string& ifName, const std::string& ipv4Addr, const std::string& ipv6Addr, int32_t ipv4PrefixLen,
            int32_t ipv6PrefixLen, const std::string& ipv4Gateway, const std::string& ipv6Gateway, int32_t dnsCount, const std::string& dnsServers, int32_t clientId) override;

    ::ndk::ScopedAStatus registerCellQltyReport(int32_t serial, const std::string& registerQuality, const std::string& type, const std::string& thresholdValues,
            const std::string& triggerTime, int32_t clientId) override;

    ::ndk::ScopedAStatus responseAcknowledgementMtk() override;
    ::ndk::ScopedAStatus setResponseFunctionsMtk(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemIndication>& radioIndication) override;
    ::ndk::ScopedAStatus setResponseFunctionsMtkIms(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemIndication>& radioIndication) override;

    ::ndk::ScopedAStatus setResponseFunctionsCap(
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::cap::IMtkRadioExCapRadioResponse>& capRadioResponse) override;

    ::ndk::ScopedAStatus setResponseFunctionsForAtci(
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::atci::IAtciResponse>& atciResponseParam,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::atci::IAtciIndication>& atciIndicationParam) override;

    ::ndk::ScopedAStatus setResponseFunctionsEm(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::em::IEmRadioResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::em::IEmRadioIndication>& radioIndication) override;

    ::ndk::ScopedAStatus setResponseFunctionsGba(
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::modem::IMtkRadioExModemResponse>& radioResponse) override;
    ::ndk::ScopedAStatus setResponseFunctionsAssist(
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::assist::IAssistModemResponse>&
                    radioResponse) override;
    ::ndk::ScopedAStatus syncAppEventStatus(int32_t serial, int32_t event, int32_t status, int32_t clientId) override;

  public:
    MtkRadioModem(unsigned int slotid);
    unsigned int mSlotId;

  public:
    bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
        if (client < 0) {
            return false;
        }
        return clientId == ((unsigned int)client);
    }
};

}  // namespace vendor::mediatek::hardware::mtkradioex::modem
