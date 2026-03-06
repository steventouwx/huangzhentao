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

namespace radioNetwork {
void setNitzTimeReceived(unsigned int slotId, int64_t timeReceived);

// response
int getAllowedNetworkTypesBitmapResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e, const void *response,
                                   size_t responselen);

int getAvailableBandModesResponse(unsigned int slotId, android::ClientId clientId,
                                 int responseType, int serial, RIL_Errno e, const void *response,
                                 size_t responselen);

int getAvailableNetworksResponse(unsigned int slotId, android::ClientId clientId,
                                int responseType, int serial, RIL_Errno e, const void *response,
                                size_t responselen);

int getBarringInfoResponse(unsigned int slotId, android::ClientId clientId,
                                int responseType, int serial, RIL_Errno e, const void *response,
                                size_t responselen);

int getCdmaRoamingPreferenceResponse(unsigned int slotId, android::ClientId clientId,
                                    int responseType, int serial, RIL_Errno e, const void *response,
                                    size_t responselen);

int getCellInfoListResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responseLen);

int getDataRegistrationStateResponse(unsigned int slotId, android::ClientId clientId,
                                    int responseType, int serial, RIL_Errno e, const void *response,
                                    size_t responselen);

int getImsRegistrationStateResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e,
                                   const void *response, size_t responselen);

int getNetworkSelectionModeResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e, const void *response,
                                   size_t responselen);

int getOperatorResponse(unsigned int slotId, android::ClientId clientId,
                       int responseType, int serial, RIL_Errno e, const void *response,
                       size_t responselen);

int getSignalStrengthResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responseLen);

int getSystemSelectionChannelsResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responselen);

int getVoiceRadioTechnologyResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e,
                                   const void *response, size_t responselen);


int getVoiceRegistrationStateResponse(unsigned int slotId, android::ClientId clientId,
                                     int responseType, int serial, RIL_Errno e, const void *response,
                                     size_t responselen);

int isNrDualConnectivityEnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);

int setAllowedNetworkTypesBitmapResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e, const void *response,
                                   size_t responselen);

int setBandModeResponse(unsigned int slotId, android::ClientId clientId,
                       int responseType, int serial, RIL_Errno e, const void *response,
                       size_t responselen);


int setBarringPasswordResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responselen);

int setCdmaRoamingPreferenceResponse(unsigned int slotId, android::ClientId clientId,
                                    int responseType, int serial, RIL_Errno e, const void *response,
                                    size_t responselen);

int setCellInfoListRateResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responselen);

int setIndicationFilterResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responselen);

int setLinkCapacityReportingCriteriaResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);


int setLocationUpdatesResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responselen);

int setNetworkSelectionModeAutomaticResponse(unsigned int slotId, android::ClientId clientId,
                                            int responseType, int serial, RIL_Errno e,
                                            const void *response, size_t responselen);

int setNetworkSelectionModeManualResponse(unsigned int slotId, android::ClientId clientId,
                                         int responseType, int serial, RIL_Errno e, const void *response,
                                         size_t responselen);

int setNrDualConnectivityStateResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);

int setSignalStrengthReportingCriteriaResponse(unsigned int slotId, android::ClientId clientId,
        int indicationType, int token, RIL_Errno err, const void *response, size_t responseLen);

int setSuppServiceNotificationsResponse(unsigned int slotId, android::ClientId clientId,
                                       int responseType, int serial, RIL_Errno e, const void *response,
                                       size_t responselen);

int setSystemSelectionChannelsResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responselen);

int startNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responselen);

int stopNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);

int supplyNetworkDepersonalizationResponse(unsigned int slotId, android::ClientId clientId,
                                          int responseType, int serial, RIL_Errno e,
                                          const void *response, size_t responselen);
int setUsageSettingResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int getUsageSettingResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);

int setEmergencyModeResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int triggerEmergencyNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int exitEmergencyModeResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int cancelEmergencyNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int setNullCipherAndIntegrityEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int isNullCipherAndIntegrityEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int isN1ModeEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int setN1ModeEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int isCellularIdentifierTransparencyEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int setCellularIdentifierTransparencyEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int setSecurityAlgorithmsUpdatedEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);
int isSecurityAlgorithmsUpdatedEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen);


// indication

int barringInfoChangedInd(unsigned int slotId,
                      int indicationType, int token, RIL_Errno e, const void *response,
                      size_t responselen);

int cdmaPrlChangedInd(unsigned int slotId,
                      int indicationType, int token, RIL_Errno e, const void *response,
                      size_t responselen);

int cellInfoListInd(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responselen);

int currentLinkCapacityEstimate(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen);

int currentPhysicalChannelConfigs(unsigned int slotId, int indicationType,
        int token, RIL_Errno err, const void *response, size_t responseLen);


int currentSignalStrengthInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e,
                             const void *response, size_t responselen);

int imsNetworkStateChangedInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e, const void *response,
                              size_t responselen);

int networkScanResultInd(unsigned int slotId,
                         int indicationType, int token, RIL_Errno e, const void *response,
                         size_t responselen);

int networkStateChangedInd(unsigned int slotId, int indType,
                                int token, RIL_Errno e, const void *response, size_t responselen);

int nitzTimeReceivedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int onRegistrationFailedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responselen);

int restrictedStateChangedInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e, const void *response,
                              size_t responselen);

int suppSvcNotifyInd(unsigned int slotId, int indicationType,
                     int token, RIL_Errno e, const void *response, size_t responselen);


int voiceRadioTechChangedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen);

int emergencyNetworkScanResultInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen);

int cellularIdentifierDisclosedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen);

int securityAlgorithmsUpdatedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen);

int clearNetworkResponseAndIndications(unsigned int slotId);
}


namespace mtkRadioExNetwork {
// Response
int abortFemtocellListResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int cancelAvailableNetworksResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int cfgA2offsetResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int cfgB1offsetResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int clearLteAvailableFileResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int deactivateNrScgCommunicationResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setCarrierAggregationModeResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int enableCAPlusBandWidthFilterResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);

int enableSCGfailureResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int get4x4MimoEnabledResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getAllBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getApcInfoResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getAvailableNetworksWithActResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getBandPriorityListResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getCALinkCapabilityListResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getCALinkEnableStatusResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getCaBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getCampedFemtoCellInfoResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getCurrentPOLListResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getDeactivateNrScgCommunicationResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getDisable2GResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getFemtocellListResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getIWlanRegistrationStateResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getLte1xRttCellListResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getLteBsrTimerResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getLteDataResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getLteRRCStateResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getLteReleaseVersionResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getLteScanDurationResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getPOLCapabilityResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getPlmnNameFromSE13TableResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getQamEnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getRoamingEnableResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getSignalStrengthWithWcdmaEcioResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getSuggestedPlmnListResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getTOEInfoResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getTm9EnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int queryFemtoCellSystemSelectionModeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int selectFemtocellResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int set4x4MimoEnabledResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setApcModeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setBandPriorityListResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setBgsrchDeltaSleepTimerResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setCALinkEnableStatusResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setDisable2GResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setFemtoCellSystemSelectionModeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setLteBandEnableStatusResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setLteBsrTimerResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setLteReleaseVersionResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);

int setLteScanDurationResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setNROptionResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setNetworkSelectionModeManualWithActResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setNrBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setPOLEntryResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setQamEnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setSearchRatResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setSearchStoredFreqInfoResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setServiceStateToModemResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setTm9EnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int setRoamingEnableResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int getNitzTimeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,size_t responseLen);

// Indication
int currentSignalStrengthWithWcdmaEcioInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int networkBandInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int networkInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int nrCaBandChangeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int nrSysInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int on5GUWInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int onMccMncChangedInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int currentNwCfgInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int currentRrcStateInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int onPseudoCellInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int responseCsNetworkStateChangeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int responseFemtocellInfo(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int responseLteNetworkInfo(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int responseModulationInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int responseNetworkEventInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int responsePsNetworkStateChangeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int sib16TimeInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int toeInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);

int iwlanRegistrationStateInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
int clearMtkNetworkResponseAndIndications(unsigned int slotId);
int clearMtkSmartRatResponseAndIndications(unsigned int slotId);
}

namespace mtkRadioExSmartRatSwitch {
// Response
int getSmartRatSwitchResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int smartRatSwitchResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen);

int smartRatSwitchInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen);
}

#include <aidl/android/hardware/radio/network/BnRadioNetwork.h>
namespace android::hardware::radio::network {

class RadioNetwork :public aidl::android::hardware::radio::network::BnRadioNetwork {
public:
    ::ndk::ScopedAStatus getAllowedNetworkTypesBitmap(int32_t serial) override;
    ::ndk::ScopedAStatus getAvailableBandModes(int32_t serial) override;
    ::ndk::ScopedAStatus getAvailableNetworks(int32_t serial) override;
    ::ndk::ScopedAStatus getBarringInfo(int32_t serial) override;
    ::ndk::ScopedAStatus getCdmaRoamingPreference(int32_t serial) override;
    ::ndk::ScopedAStatus getCellInfoList(int32_t serial) override;
    ::ndk::ScopedAStatus getDataRegistrationState(int32_t serial) override;
    ::ndk::ScopedAStatus getImsRegistrationState(int32_t serial) override;
    ::ndk::ScopedAStatus getNetworkSelectionMode(int32_t serial) override;
    ::ndk::ScopedAStatus getOperator(int32_t serial) override;
    ::ndk::ScopedAStatus getSignalStrength(int32_t serial) override;
    ::ndk::ScopedAStatus getSystemSelectionChannels(int32_t serial) override;
    ::ndk::ScopedAStatus getVoiceRadioTechnology(int32_t serial) override;
    ::ndk::ScopedAStatus getVoiceRegistrationState(int32_t serial) override;
    ::ndk::ScopedAStatus isNrDualConnectivityEnabled(int32_t serial) override;
    ::ndk::ScopedAStatus responseAcknowledgement() override;
    ::ndk::ScopedAStatus setAllowedNetworkTypesBitmap(
            int32_t serial, int32_t networkTypeBitmap) override;
    ::ndk::ScopedAStatus setBandMode(
            int32_t serial, ::aidl::android::hardware::radio::network::RadioBandMode mode) override;
    ::ndk::ScopedAStatus setBarringPassword(int32_t serial, const std::string& facility,
                                            const std::string& oldPassword,
                                            const std::string& newPassword) override;
    ::ndk::ScopedAStatus setCdmaRoamingPreference(
            int32_t serial,
            ::aidl::android::hardware::radio::network::CdmaRoamingType type) override;
    ::ndk::ScopedAStatus setCellInfoListRate(int32_t serial, int32_t rate) override;
    ::ndk::ScopedAStatus setIndicationFilter(
            int32_t serial, int32_t indicationFilter) override;
    ::ndk::ScopedAStatus setLinkCapacityReportingCriteria(
            int32_t serial, int32_t hysteresisMs, int32_t hysteresisDlKbps,
            int32_t hysteresisUlKbps, const std::vector<int32_t>& thresholdsDownlinkKbps,
            const std::vector<int32_t>& thresholdsUplinkKbps,
            ::aidl::android::hardware::radio::AccessNetwork accessNetwork) override;
    ::ndk::ScopedAStatus setLocationUpdates(int32_t serial, bool enable) override;
    ::ndk::ScopedAStatus setNetworkSelectionModeAutomatic(int32_t serial) override;
    ::ndk::ScopedAStatus setNetworkSelectionModeManual(
            int32_t serial, const std::string& operatorNumeric,
            ::aidl::android::hardware::radio::AccessNetwork ran) override;
    ::ndk::ScopedAStatus setNrDualConnectivityState(
            int32_t serial,
            ::aidl::android::hardware::radio::network::NrDualConnectivityState nrSt) override;
    ::ndk::ScopedAStatus setResponseFunctions(
            const std::shared_ptr<::aidl::android::hardware::radio::network::IRadioNetworkResponse>&
                    radioNetworkResponse,
            const std::shared_ptr<
                    ::aidl::android::hardware::radio::network::IRadioNetworkIndication>&
                    radioNetworkIndication) override;
    ::ndk::ScopedAStatus setSignalStrengthReportingCriteria(
            int32_t serial,
            const std::vector<::aidl::android::hardware::radio::network::SignalThresholdInfo>&
                    signalThresholdInfos) override;
    ::ndk::ScopedAStatus setSuppServiceNotifications(int32_t serial, bool enable) override;
    ::ndk::ScopedAStatus setSystemSelectionChannels(
            int32_t serial, bool specifyChannels,
            const std::vector<::aidl::android::hardware::radio::network::RadioAccessSpecifier>&
                    specifiers) override;
    ::ndk::ScopedAStatus startNetworkScan(
            int32_t serial,
            const ::aidl::android::hardware::radio::network::NetworkScanRequest& request) override;
    ::ndk::ScopedAStatus stopNetworkScan(int32_t serial) override;
    ::ndk::ScopedAStatus supplyNetworkDepersonalization(int32_t serial,
                                                        const std::string& netPin) override;
    ::ndk::ScopedAStatus setUsageSetting(
            int32_t serial,
            ::aidl::android::hardware::radio::network::UsageSetting usageSetting) override;
    ::ndk::ScopedAStatus getUsageSetting(int32_t serial) override;
    ::ndk::ScopedAStatus setEmergencyMode(int32_t serial,
            ::aidl::android::hardware::radio::network::EmergencyMode emcModeType) override;
    ::ndk::ScopedAStatus triggerEmergencyNetworkScan(int32_t serial,
            const ::aidl::android::hardware::radio::network::EmergencyNetworkScanTrigger& request) override;
    ::ndk::ScopedAStatus cancelEmergencyNetworkScan(int32_t serial, bool resetScan) override;
    ::ndk::ScopedAStatus exitEmergencyMode(int32_t serial) override;
    ::ndk::ScopedAStatus setNullCipherAndIntegrityEnabled(int32_t serial, bool enabled) override;
    ::ndk::ScopedAStatus isNullCipherAndIntegrityEnabled(int32_t serial) override;
    ::ndk::ScopedAStatus isN1ModeEnabled(int32_t serial) override;
    ::ndk::ScopedAStatus setN1ModeEnabled(int32_t serial, bool enable) override;
    ::ndk::ScopedAStatus isCellularIdentifierTransparencyEnabled(int32_t serial) override;
    ::ndk::ScopedAStatus setCellularIdentifierTransparencyEnabled(int32_t serial, bool enabled) override;
    ::ndk::ScopedAStatus setSecurityAlgorithmsUpdatedEnabled(int32_t serial, bool enable) override;
    ::ndk::ScopedAStatus isSecurityAlgorithmsUpdatedEnabled(int32_t serial) override;


    public:
        RadioNetwork(unsigned int slotid);
        unsigned int mSlotId;
};

}  // namespace android::hardware::radio::network


#include <aidl/vendor/mediatek/hardware/mtkradioex/network/BnMtkRadioExNetwork.h>

namespace vendor::mediatek::hardware::mtkradioex::network {

class MtkRadioNetwork : public aidl::vendor::mediatek::hardware::mtkradioex::network::BnMtkRadioExNetwork {
public:
    ::ndk::ScopedAStatus abortFemtocellList(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus cancelAvailableNetworks(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus cfgA2offset(int32_t serial, int32_t offset, int32_t threshBound, int32_t clientId) override;
    ::ndk::ScopedAStatus cfgB1offset(int32_t serial, int32_t offset, int32_t threshBound, int32_t clientId) override;
    ::ndk::ScopedAStatus clearLteAvailableFile(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus deactivateNrScgCommunication(int32_t serial, bool deactivate, bool allowSCGAdd, int32_t clientId) override;
    ::ndk::ScopedAStatus setCarrierAggregationMode(int32_t serial, int32_t mode, int32_t option, int32_t linkType, int32_t clientId) override;
    ::ndk::ScopedAStatus enableCAPlusBandWidthFilter(int32_t serial, bool enable, int32_t clientId) override;
    ::ndk::ScopedAStatus enableSCGfailure(int32_t serial, bool enable, int32_t T1, int32_t P1, int32_t T2, int32_t clientId) override;
    ::ndk::ScopedAStatus get4x4MimoEnabled(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getAllBandMode(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getApcInfo(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getAvailableNetworksWithAct(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getBandMode(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getBandPriorityList(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getCALinkCapabilityList(int32_t serial, int32_t linkType, int32_t clientId) override;
    ::ndk::ScopedAStatus getCALinkEnableStatus(int32_t serial, const std::string& bandsCombo, int32_t linkType, int32_t clientId) override;
    ::ndk::ScopedAStatus getCaBandMode(int32_t serial, int32_t primaryBandId, int32_t clientId) override;
    ::ndk::ScopedAStatus getCampedFemtoCellInfo(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getCurrentPOLList(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getDeactivateNrScgCommunication(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getDisable2G(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getFemtocellList(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getIWlanRegistrationState(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getLte1xRttCellList(int32_t serial, bool available, int32_t clientId) override;
    ::ndk::ScopedAStatus getLteBsrTimer(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getLteData(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getLteRRCState(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getLteReleaseVersion(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getLteScanDuration(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getPOLCapability(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getPlmnNameFromSE13Table(int32_t serial, int32_t mcc, int32_t mnc, int32_t clientId) override;
    ::ndk::ScopedAStatus getQamEnabled(int32_t serial, bool ulOrDl, int32_t clientId) override;
    ::ndk::ScopedAStatus getRoamingEnable(int32_t serial, int32_t phoneId, int32_t clientId) override;
    ::ndk::ScopedAStatus getSignalStrengthWithWcdmaEcio(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getSmartRatSwitch(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus getSuggestedPlmnList(int32_t serial, int32_t rat, int32_t num, int32_t timer, int32_t clientId) override;
    ::ndk::ScopedAStatus getTOEInfo(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus getTm9Enabled(int32_t serial, bool fddOrTdd, int32_t clientId) override;
    ::ndk::ScopedAStatus queryFemtoCellSystemSelectionMode(int32_t serial, int32_t clientId) override;
    ::ndk::ScopedAStatus selectFemtocell(int32_t serial, const std::string& operatorNumeric, const std::string& act, const std::string& csgId, int32_t clientId) override;
    ::ndk::ScopedAStatus set4x4MimoEnabled(int32_t serial, int32_t enabled_bitmask, int32_t clientId) override;
    ::ndk::ScopedAStatus setApcMode(int32_t serial, int32_t mode, int32_t reportMode, int32_t interval, int32_t clientId) override;
    ::ndk::ScopedAStatus setBandPriorityList(int32_t serial, const std::vector<int32_t>& bandPriList, int32_t clientId) override;
    ::ndk::ScopedAStatus setBgsrchDeltaSleepTimer(int32_t serial, int32_t sleepDuration, int32_t clientId) override;
    ::ndk::ScopedAStatus setCALinkEnableStatus(int32_t serial, bool status, const std::string& bandsCombo, int32_t linkType, int32_t clientId) override;
    ::ndk::ScopedAStatus setDisable2G(int32_t serial, bool mode, int32_t clientId) override;
    ::ndk::ScopedAStatus setFemtoCellSystemSelectionMode(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus setLteBandEnableStatus(int32_t serial, int32_t bandId, bool status, int32_t clientId) override;
    ::ndk::ScopedAStatus setLteBsrTimer(int32_t serial, int32_t timer, int32_t clientId) override;
    ::ndk::ScopedAStatus setLteReleaseVersion(int32_t serial, int32_t mode, int32_t clientId) override;
    ::ndk::ScopedAStatus setLteScanDuration(int32_t serial, int32_t duration, int32_t clientId) override;
    ::ndk::ScopedAStatus setNROption(int32_t serial, int32_t option, int32_t clientId) override;
    ::ndk::ScopedAStatus setNetworkSelectionModeManualWithAct(int32_t serial, const std::string& operatorNumeric, const std::string& act, const std::string& mode, int32_t clientId) override;
    ::ndk::ScopedAStatus setNrBandMode(int32_t serial, const std::vector<int32_t>& saEnable, const std::vector<int32_t>& saDisable, const std::vector<int32_t>& nsaEnable,
            const std::vector<int32_t>& nsaDisable, int32_t clientId) override;
    ::ndk::ScopedAStatus setPOLEntry(int32_t serial, int32_t index, const std::string& numeric, int32_t nAct, int32_t clientId) override;
    ::ndk::ScopedAStatus setQamEnabled(int32_t serial, bool ulOrDl, bool enabled, int32_t clientId) override;
    ::ndk::ScopedAStatus setSearchRat(int32_t serial, const std::vector<int32_t>& rat, int32_t clientId) override;
    ::ndk::ScopedAStatus setSearchStoredFreqInfo(int32_t serial, int32_t operation, int32_t plmn_id, int32_t rat, const std::vector<int32_t>& freq, int32_t clientId) override;
    ::ndk::ScopedAStatus setServiceStateToModem(int32_t serial, int32_t voiceRegState, int32_t dataRegState, int32_t voiceRoamingType, int32_t dataRoamingType, int32_t rilVoiceRegState,
            int32_t rilDataRegState, int32_t clientId) override;
    ::ndk::ScopedAStatus setTm9Enabled(int32_t serial, bool fddOrTdd, bool enabled, int32_t clientId) override;
    ::ndk::ScopedAStatus smartRatSwitch(int32_t serial, int32_t mode, int32_t rat, int32_t clientId) override;
    ::ndk::ScopedAStatus setRoamingEnable(int32_t serial, const std::vector<int32_t>& config, int32_t clientId) override;
    ::ndk::ScopedAStatus getNitzTime(int32_t in_serial, int32_t in_clientId) override;
    ::ndk::ScopedAStatus responseAcknowledgementMtk() override;
    ::ndk::ScopedAStatus setResponseFunctionsMtk(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::network::IMtkRadioExNetworkResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::network::IMtkRadioExNetworkIndication>& radioIndication) override;
    ::ndk::ScopedAStatus setResponseFunctionsSmartRatSwitch(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::smartratswitch::ISmartRatSwitchRadioResponse>& radioResponse,
            const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::smartratswitch::ISmartRatSwitchRadioIndication>& radioIndication) override;


    public:
        MtkRadioNetwork(unsigned int slotid);
        unsigned int mSlotId;

    public:
        bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
            if (client < 0) {
                return false;
            }
            return clientId == ((unsigned int)client);
        }
};

}
