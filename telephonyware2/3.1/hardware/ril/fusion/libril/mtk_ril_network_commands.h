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
{RIL_REQUEST_ABORT_FEMTOCELL_LIST, mtkRadioExNetwork::abortFemtocellListResponse},
{RIL_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS, mtkRadioExNetwork::cancelAvailableNetworksResponse},
{RIL_REQUEST_CONFIG_A2_OFFSET, mtkRadioExNetwork::cfgA2offsetResponse},
{RIL_REQUEST_CONFIG_B1_OFFSET, mtkRadioExNetwork::cfgB1offsetResponse},
{RIL_REQUEST_CLEAR_LTE_AVAILABLE_FILE, mtkRadioExNetwork::clearLteAvailableFileResponse},
{RIL_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION, mtkRadioExNetwork::deactivateNrScgCommunicationResponse},
{RIL_REQUEST_SET_CARRIER_AGGREGATION_MODE, mtkRadioExNetwork::setCarrierAggregationModeResponse},
{RIL_REQUEST_ENABLE_CA_PLUS_FILTER, mtkRadioExNetwork::enableCAPlusBandWidthFilterResponse},
{RIL_REQUEST_ENABLE_SCG_FAILURE, mtkRadioExNetwork::enableSCGfailureResponse},
{RIL_REQUEST_GET_4X4MIMO_ENABLED, mtkRadioExNetwork::get4x4MimoEnabledResponse},
{RIL_REQUEST_GET_ALL_BAND_MODE, mtkRadioExNetwork::getAllBandModeResponse},
{RIL_REQUEST_GET_PSEUDO_CELL_INFO, mtkRadioExNetwork::getApcInfoResponse},
{RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT, mtkRadioExNetwork::getAvailableNetworksWithActResponse},
{RIL_REQUEST_GET_BAND_MODE, mtkRadioExNetwork::getBandModeResponse},
{RIL_REQUEST_GET_BAND_PRIORITY_LIST, mtkRadioExNetwork::getBandPriorityListResponse},
{RIL_REQUEST_GET_CA_LINK_CAPABILITY_LIST, mtkRadioExNetwork::getCALinkCapabilityListResponse},
{RIL_REQUEST_GET_CA_LINK_ENABLE_STATUS, mtkRadioExNetwork::getCALinkEnableStatusResponse},
{RIL_REQUEST_GET_CA_BAND_MODE, mtkRadioExNetwork::getCaBandModeResponse},
{RIL_REQUEST_GET_CAMPED_FEMTO_CELL_INFO, mtkRadioExNetwork::getCampedFemtoCellInfoResponse},
{RIL_REQUEST_GET_POL_LIST, mtkRadioExNetwork::getCurrentPOLListResponse},
{RIL_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION, mtkRadioExNetwork::getDeactivateNrScgCommunicationResponse},
{RIL_REQUEST_GET_DISABLE_2G, mtkRadioExNetwork::getDisable2GResponse},
{RIL_REQUEST_GET_FEMTOCELL_LIST, mtkRadioExNetwork::getFemtocellListResponse},
{RIL_REQUEST_IWLAN_REGISTRATION_STATE, mtkRadioExNetwork::getIWlanRegistrationStateResponse},
{RIL_REQUEST_GET_LTE_1XRTT_CELL_LIST, mtkRadioExNetwork::getLte1xRttCellListResponse},
{RIL_REQUEST_GET_LTE_BSR_TIMER, mtkRadioExNetwork::getLteBsrTimerResponse},
{RIL_REQUEST_GET_LTE_DATA, mtkRadioExNetwork::getLteDataResponse},
{RIL_REQUEST_GET_LTE_RRC_STATE, mtkRadioExNetwork::getLteRRCStateResponse},
{RIL_REQUEST_GET_LTE_RELEASE_VERSION, mtkRadioExNetwork::getLteReleaseVersionResponse},
{RIL_REQUEST_GET_LTE_SCAN_DURATION, mtkRadioExNetwork::getLteScanDurationResponse},
{RIL_REQUEST_GET_POL_CAPABILITY, mtkRadioExNetwork::getPOLCapabilityResponse},
{RIL_REQUEST_GET_TS25_NAME, mtkRadioExNetwork::getPlmnNameFromSE13TableResponse},
{RIL_REQUEST_GET_QAM_ENABLED, mtkRadioExNetwork::getQamEnabledResponse},
{RIL_REQUEST_GET_ROAMING_ENABLE, mtkRadioExNetwork::getRoamingEnableResponse},
{RIL_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO, mtkRadioExNetwork::getSignalStrengthWithWcdmaEcioResponse},
{RIL_REQUEST_GET_SUGGESTED_PLMN_LIST, mtkRadioExNetwork::getSuggestedPlmnListResponse},
{RIL_REQUEST_GET_TOE_INFO, mtkRadioExNetwork::getTOEInfoResponse},
{RIL_REQUEST_GET_TM9_ENABLED, mtkRadioExNetwork::getTm9EnabledResponse},
{RIL_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE, mtkRadioExNetwork::queryFemtoCellSystemSelectionModeResponse},
{RIL_REQUEST_SELECT_FEMTOCELL, mtkRadioExNetwork::selectFemtocellResponse},
{RIL_REQUEST_SET_4X4MIMO_ENABLED, mtkRadioExNetwork::set4x4MimoEnabledResponse},
{RIL_REQUEST_SET_PSEUDO_CELL_MODE, mtkRadioExNetwork::setApcModeResponse},
{RIL_REQUEST_SET_BAND_PRIORITY_LIST, mtkRadioExNetwork::setBandPriorityListResponse},
{RIL_REQUEST_SET_BACKGROUND_SEARCH_TIMER, mtkRadioExNetwork::setBgsrchDeltaSleepTimerResponse},
{RIL_REQUEST_SET_CA_LINK_ENABLE_STATUS, mtkRadioExNetwork::setCALinkEnableStatusResponse},
{RIL_REQUEST_SET_DISABLE_2G, mtkRadioExNetwork::setDisable2GResponse},
{RIL_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE, mtkRadioExNetwork::setFemtoCellSystemSelectionModeResponse},
{RIL_REQUEST_SET_LTE_BAND_ENABLE_STATUS, mtkRadioExNetwork::setLteBandEnableStatusResponse},
{RIL_REQUEST_SET_LTE_BSR_TIMER, mtkRadioExNetwork::setLteBsrTimerResponse},
{RIL_REQUEST_SET_LTE_RELEASE_VERSION, mtkRadioExNetwork::setLteReleaseVersionResponse},
{RIL_REQUEST_SET_LTE_SCAN_DURATION, mtkRadioExNetwork::setLteScanDurationResponse},
{RIL_REQUEST_SET_NR_OPTION, mtkRadioExNetwork::setNROptionResponse},
{RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT, mtkRadioExNetwork::setNetworkSelectionModeManualWithActResponse},
{RIL_REQUEST_SET_NR_BAND_MODE, mtkRadioExNetwork::setNrBandModeResponse},
{RIL_REQUEST_SET_POL_ENTRY, mtkRadioExNetwork::setPOLEntryResponse},
{RIL_REQUEST_SET_QAM_ENABLED, mtkRadioExNetwork::setQamEnabledResponse},
{RIL_REQUEST_SEARCH_RAT, mtkRadioExNetwork::setSearchRatResponse},
{RIL_REQUEST_SEARCH_STORED_FREQUENCY_INFO, mtkRadioExNetwork::setSearchStoredFreqInfoResponse},
{RIL_REQUEST_SET_SERVICE_STATE, mtkRadioExNetwork::setServiceStateToModemResponse},
{RIL_REQUEST_SET_TM9_ENABLED, mtkRadioExNetwork::setTm9EnabledResponse},
{RIL_REQUEST_SET_ROAMING_ENABLE, mtkRadioExNetwork::setRoamingEnableResponse},
//unused item
//{RIL_REQUEST_SET_SMART_RAT_SWITCH, mtkRadioExNetwork::setSmartSceneSwitchResponse},
{RIL_REQUEST_GET_NITZ, mtkRadioExNetwork::getNitzTimeResponse},

{RIL_REQUEST_GET_SMART_RAT_SWITCH, mtkRadioExSmartRatSwitch::getSmartRatSwitchResponse},
{RIL_REQUEST_SMART_RAT_SWITCH, mtkRadioExSmartRatSwitch::smartRatSwitchResponse},
