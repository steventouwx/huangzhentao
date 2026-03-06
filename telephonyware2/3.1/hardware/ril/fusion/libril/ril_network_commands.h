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
{RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP, radioNetwork::getAllowedNetworkTypesBitmapResponse},
{RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE, radioNetwork::getAvailableBandModesResponse},
{RIL_REQUEST_QUERY_AVAILABLE_NETWORKS , radioNetwork::getAvailableNetworksResponse},
{RIL_REQUEST_GET_BARRINGINFO, radioNetwork::getBarringInfoResponse},
{RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE, radioNetwork::getCdmaRoamingPreferenceResponse},
{RIL_REQUEST_GET_CELL_INFO_LIST, radioNetwork::getCellInfoListResponse},
{RIL_REQUEST_DATA_REGISTRATION_STATE, radioNetwork::getDataRegistrationStateResponse},
{RIL_REQUEST_IMS_REGISTRATION_STATE, radioNetwork::getImsRegistrationStateResponse},    // IMS
{RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE, radioNetwork::getNetworkSelectionModeResponse},
{RIL_REQUEST_OPERATOR, radioNetwork::getOperatorResponse},
{RIL_REQUEST_SIGNAL_STRENGTH, radioNetwork::getSignalStrengthResponse},
{RIL_REQUEST_GET_SYSTEM_SELECTION_CHANNELS, radioNetwork::getSystemSelectionChannelsResponse},
{RIL_REQUEST_VOICE_RADIO_TECH, radioNetwork::getVoiceRadioTechnologyResponse},
{RIL_REQUEST_VOICE_REGISTRATION_STATE, radioNetwork::getVoiceRegistrationStateResponse},
{RIL_REQUEST_GET_NRDC_STATE, radioNetwork::isNrDualConnectivityEnabledResponse},
{RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP, radioNetwork::setAllowedNetworkTypesBitmapResponse},
{RIL_REQUEST_SET_BAND_MODE, radioNetwork::setBandModeResponse},
{RIL_REQUEST_CHANGE_BARRING_PASSWORD, radioNetwork::setBarringPasswordResponse},  //SS
{RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE, radioNetwork::setCdmaRoamingPreferenceResponse},
{RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE, radioNetwork::setCellInfoListRateResponse},
{RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER, radioNetwork::setIndicationFilterResponse},
{RIL_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA, radioNetwork::setLinkCapacityReportingCriteriaResponse},  //Data
{RIL_REQUEST_SET_LOCATION_UPDATES, radioNetwork::setLocationUpdatesResponse},
{RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC, radioNetwork::setNetworkSelectionModeAutomaticResponse},
{RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL, radioNetwork::setNetworkSelectionModeManualResponse},
{RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN, radioNetwork::setNetworkSelectionModeManualResponse},
{RIL_REQUEST_SET_NRDC_STATE, radioNetwork::setNrDualConnectivityStateResponse},
{RIL_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA, radioNetwork::setSignalStrengthReportingCriteriaResponse},
{RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION, radioNetwork::setSuppServiceNotificationsResponse},   // SS
{RIL_REQUEST_SET_SYSTEM_SELECTION_CHANNELS, radioNetwork::setSystemSelectionChannelsResponse},
{RIL_REQUEST_START_NETWORK_SCAN, radioNetwork::startNetworkScanResponse},
{RIL_REQUEST_STOP_NETWORK_SCAN, radioNetwork::stopNetworkScanResponse},
{RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION, radioNetwork::supplyNetworkDepersonalizationResponse},  //SIM
{RIL_REQUEST_SET_USAGE_SETTING, radioNetwork::setUsageSettingResponse},
{RIL_REQUEST_GET_USAGE_SETTING, radioNetwork::getUsageSettingResponse},
{RIL_REQUEST_SET_EMERGENCY_MODE, radioNetwork::setEmergencyModeResponse},
{RIL_REQUEST_TRIGGER_EMERGENCY_NETWORK_SCAN, radioNetwork::triggerEmergencyNetworkScanResponse},
{RIL_REQUEST_CANCEL_EMERGENCY_NETWORK_SCAN, radioNetwork::cancelEmergencyNetworkScanResponse},
{RIL_REQUEST_EXIT_EMERGENCY_MODE, radioNetwork::exitEmergencyModeResponse},
{RIL_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED, radioNetwork::setNullCipherAndIntegrityEnabledResponse},
{RIL_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED, radioNetwork::isNullCipherAndIntegrityEnabledResponse},
{RIL_REQUEST_IS_N1_MODE_ENABLED, radioNetwork::isN1ModeEnabledResponse},
{RIL_REQUEST_SET_N1_MODE_ENABLED, radioNetwork::setN1ModeEnabledResponse},
{RIL_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED, radioNetwork::isCellularIdentifierTransparencyEnabledResponse},
{RIL_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED, radioNetwork::setCellularIdentifierTransparencyEnabledResponse},
{RIL_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED, radioNetwork::setSecurityAlgorithmsUpdatedEnabledResponse},
{RIL_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED, radioNetwork::isSecurityAlgorithmsUpdatedEnabledResponse},
