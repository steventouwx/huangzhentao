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
// M: IMS_DATA start @{
    {RIL_REQUEST_IMS_BEARER_STATE_CONFIRM, mtkRadioExIms::imsBearerStateConfirmResponse},
    {RIL_REQUEST_SET_IMS_BEARER_NOTIFICATION, mtkRadioExIms::setImsBearerNotificationResponse},
// M: IMS_DATA end @}

// M: IMS_SMS start @{
    {RIL_REQUEST_SMS_ACKNOWLEDGE_EX, mtkRadioExIms::acknowledgeLastIncomingGsmSmsExResponse},
    {RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX, mtkRadioExIms::acknowledgeLastIncomingCdmaSmsExResponse},
    {RIL_REQUEST_IMS_SEND_SMS_EX, mtkRadioExIms::sendImsSmsExResponse},
// M: IMS_SMS end @}

// M: IMS_RTT start @{
    {RIL_REQUEST_SET_RTT_MODE, mtkRadioExIms::setRttModeResponse},
    {RIL_REQUEST_SEND_RTT_MODIFY_REQUEST, mtkRadioExIms::sendRttModifyRequestResponse},
    {RIL_REQUEST_SEND_RTT_TEXT, mtkRadioExIms::sendRttTextResponse},
    {RIL_REQUEST_RTT_MODIFY_REQUST_RESPONSE, mtkRadioExIms::rttModifyRequestResponseResponse},
    {RIL_REQUEST_TOGGLE_RTT_AUDIO_INDICATION, mtkRadioExIms::toggleRttAudioIndicationResponse},
// M: IMS_RTT end @}

// M: IMS_BASE start @{
    {RIL_REQUEST_SET_IMS_REGISTRATION_REPORT, mtkRadioExIms::setImsRegistrationReportResponse},
    {RIL_REQUEST_QUERY_VOPS_STATUS, mtkRadioExIms::queryVopsStatusResponse},
// M: IMS_BASE end @}

// M: IMS_SS start @{
    {RIL_REQUEST_SEND_USSI, mtkRadioExIms::sendUssiResponse},
    {RIL_REQUEST_CANCEL_USSI, mtkRadioExIms::cancelUssiResponse},
    {RIL_REQUEST_GET_XCAP_STATUS, mtkRadioExIms::getXcapStatusResponse},
    {RIL_REQUEST_SETUP_XCAP_USER_AGENT_STRING, mtkRadioExIms::setupXcapUserAgentStringResponse},
    {RIL_REQUEST_GET_BARRING_SEPCIFIC_CALL, mtkRadioExIms::getBarringCallsResponse},
    {RIL_REQUEST_SET_BARRING_SEPCIFIC_CALL, mtkRadioExIms::setBarringCallsResponse},
// M: IMS_SS end @}

// M: IMS_ViLTE start @{
    {RIL_REQUEST_VIDEO_CALL_ACCEPT, mtkRadioExIms::videoCallAcceptResponse},
    {RIL_REQUEST_IMS_VT_DIAL, mtkRadioExIms::imsVtDialResponse},
    {RIL_REQUEST_VT_DIAL_WITH_SIP_URI, mtkRadioExIms::vtDialWithSipUriResponse},
    {RIL_REQUEST_VIDEO_CALL_ACCEPT, mtkRadioExIms::videoCallAcceptResponse},
    {RIL_REQUEST_VIDEO_RINGTONE_EVENT, mtkRadioExIms::videoRingtoneEventResponse},
// M: IMS_ViLTE end @}

// M: IMS_CC start @{
    {RIL_REQUEST_DIAL_WITH_SIP_URI, mtkRadioExIms::dialWithSipUriResponse},
    {RIL_REQUEST_IMS_ECT, mtkRadioExIms::imsEctCommandResponse},
    {RIL_REQUEST_PULL_CALL, mtkRadioExIms::pullCallResponse},
    {RIL_REQUEST_ECC_REDIAL_APPROVE, mtkRadioExIms::eccRedialApproveResponse},
    {RIL_REQUEST_CONFERENCE_DIAL, mtkRadioExIms::conferenceDialResponse},
    {RIL_REQUEST_FORCE_RELEASE_CALL, mtkRadioExIms::forceReleaseCallResponse},
    {RIL_REQUEST_HOLD_CALL, mtkRadioExIms::controlCallResponse},
    {RIL_REQUEST_RESUME_CALL, mtkRadioExIms::controlCallResponse},
    {RIL_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER, mtkRadioExIms::controlImsConferenceCallMemberResponse},
    {RIL_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER, mtkRadioExIms::controlImsConferenceCallMemberResponse},
    {RIL_REQUEST_SET_SIP_HEADER, mtkRadioExIms::setSipHeaderResponse},
    {RIL_REQUEST_SIP_HEADER_REPORT, mtkRadioExIms::setSipHeaderReportResponse},
    {RIL_REQUEST_SET_IMS_CALL_MODE, mtkRadioExIms::setImsCallModeResponse},
    {RIL_REQUEST_SET_CALL_ADDITIONAL_INFO, mtkRadioExIms::setCallAdditionalInfoResponse},
// M: IMS_CC end @}

// M: IMS_CONFIG  start @{
    // [IMS] Enable/Disable IMS Features
    {RIL_REQUEST_SET_IMSCFG, mtkRadioExIms::setImscfgResponse},
    // [IMS] Get IMS Features
    {RIL_REQUEST_GET_IMSCFG, mtkRadioExIms::getImscfgResponse},
    // [IMS] Set IMS configuration to modem
    {RIL_REQUEST_SET_MD_IMSCFG, mtkRadioExIms::setModemImsCfgResponse},
    // [IMS][Telephonyware] Set IMS Config Feature Value
    {RIL_REQUEST_IMS_CONFIG_SET_FEATURE, mtkRadioExIms::setImsCfgFeatureValueResponse},
    // [IMS][Telephonyware] Set IMS Config Feature Value
    {RIL_REQUEST_IMS_CONFIG_GET_FEATURE, mtkRadioExIms::getImsCfgFeatureValueResponse},
    // [IMS][Telephonyware] Set IMS Config Provision Value
    {RIL_REQUEST_IMS_CONFIG_SET_PROVISION, mtkRadioExIms::setImsCfgProvisionValueResponse},
    // [IMS][Telephonyware] Get IMS Config Provision Value
    {RIL_REQUEST_IMS_CONFIG_GET_PROVISION, mtkRadioExIms::getImsCfgProvisionValueResponse},
    // [IMS][Telephonyware] Get IMS Config Resource Cap Value
    {RIL_REQUEST_IMS_CONFIG_GET_RESOURCE_CAP, mtkRadioExIms::getImsCfgResourceCapValueResponse},
    {RIL_REQUEST_SET_WFC_PROFILE, mtkRadioExIms::setWfcProfileResponse},
    {RIL_REQUEST_SET_VOICE_DOMAIN_PREFERENCE, mtkRadioExIms::setVoiceDomainPreferenceResponse},
    {RIL_REQUEST_GET_VOICE_DOMAIN_PREFERENCE, mtkRadioExIms::getVoiceDomainPreferenceResponse},
// M: IMS_CONFIG end @}

// M: IMS_WFC start @{
    {RIL_REQUEST_SET_WIFI_ENABLED, mtkRadioExIms::setWifiEnabledResponse},
    {RIL_REQUEST_SET_WIFI_ASSOCIATED, mtkRadioExIms::setWifiAssociatedResponse},
    {RIL_REQUEST_SET_WFC_CONFIG, mtkRadioExIms::setWfcConfigResponse},
    {RIL_REQUEST_GET_WFC_CONFIG, mtkRadioExIms::getWfcConfigResponse},
    {RIL_REQUEST_SET_WIFI_SIGNAL_LEVEL, mtkRadioExIms::setWifiSignalLevelResponse},
    {RIL_REQUEST_SET_WIFI_IP_ADDRESS, mtkRadioExIms::setWifiIpAddressResponse},
    {RIL_REQUEST_SET_GEO_LOCATION, mtkRadioExIms::setLocationInfoResponse},
    {RIL_REQUEST_SET_EMERGENCY_ADDRESS_ID, mtkRadioExIms::setEmergencyAddressIdResponse},
    {RIL_REQUEST_SET_NATT_KEEP_ALIVE_STATUS, mtkRadioExIms::setNattKeepAliveStatusResponse},
    {RIL_REQUEST_SET_WIFI_PING_RESULT, mtkRadioExIms::setWifiPingResultResponse},
    {RIL_REQUEST_NOTIFY_EPDG_SCREEN_STATE, mtkRadioExIms::notifyEPDGScreenStateResponse},
    {RIL_REQUEST_QUERY_SSAC_STATUS, mtkRadioExIms::querySsacStatusResponse},
// M: IMS_WFC end @}

