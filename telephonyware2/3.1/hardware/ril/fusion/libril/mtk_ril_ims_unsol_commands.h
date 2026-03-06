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
    {RIL_UNSOL_IMS_BEARER_INIT, mtkRadioExIms::imsBearerInitInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_BEARER_STATE_NOTIFY, mtkRadioExIms::imsBearerStateNotifyInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_DATA_INFO_NOTIFY, mtkRadioExIms::imsDataInfoNotifyInd, WAKE_PARTIAL},
// M: IMS_DATA end @}

// M: IMS_SMS start @{
    {RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT_EX, mtkRadioExIms::newSmsStatusReportIndEx, WAKE_PARTIAL},
    {RIL_UNSOL_RESPONSE_NEW_SMS_EX, mtkRadioExIms::newSmsIndEx, WAKE_PARTIAL},
    {RIL_UNSOL_RESPONSE_CDMA_NEW_SMS_EX, mtkRadioExIms::cdmaNewSmsIndEx, WAKE_PARTIAL},
// M: IMS_SMS end @}

// M: IMS_RTT start @{
    {RIL_UNSOL_RTT_MODIFY_RESPONSE, mtkRadioExIms::rttModifyResponseInd, WAKE_PARTIAL},
    {RIL_UNSOL_RTT_TEXT_RECEIVE, mtkRadioExIms::rttTextReceiveInd, WAKE_PARTIAL},
    {RIL_UNSOL_RTT_CAPABILITY_INDICATION, mtkRadioExIms::rttCapabilityIndicationInd, WAKE_PARTIAL},
    {RIL_UNSOL_RTT_MODIFY_REQUEST_RECEIVE, mtkRadioExIms::rttModifyRequestReceiveInd, WAKE_PARTIAL},
    {RIL_UNSOL_AUDIO_INDICATION, mtkRadioExIms::audioIndicationInd, WAKE_PARTIAL},
// M: IMS_RTT end @}

// M: IMS_BASE start @{
    {RIL_UNSOL_VOPS_INDICATION, mtkRadioExIms::sendVopsIndication, WAKE_PARTIAL},
    {RIL_UNSOL_VOLTE_SETTING, mtkRadioExIms::volteSettingInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_REGISTRATION_INFO, mtkRadioExIms::imsRegistrationInfoInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_SUPPORT_ECC, mtkRadioExIms::imsSupportEccInd, WAKE_PARTIAL},
    {RIL_UNSOL_SIP_REG_INFO, mtkRadioExIms::sipRegInfoInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_REGISTRATION_STATE_IND, mtkRadioExIms::imsRegistrationStateInd, WAKE_PARTIAL},
    {RIL_UNSOL_EIREG_INFO_IND, mtkRadioExIms::eiregDataInd, WAKE_PARTIAL},
    {RIL_UNSOL_EREGRT_INFO_IND, mtkRadioExIms::eregrtInfoInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_REG_FLAG_IND, mtkRadioExIms::imsRegFlagInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_RTP_INFO, mtkRadioExIms::imsRtpInfoInd, WAKE_PARTIAL},
    {RIL_UNSOL_ON_XUI, mtkRadioExIms::onXuiInd, WAKE_PARTIAL},
// M: IMS_BASE end @}

// M: IMS_SS start @{
    {RIL_UNSOL_ON_USSI, mtkRadioExIms::onUssiInd, WAKE_PARTIAL},
    {RIL_UNSOL_ON_VOLTE_SUBSCRIPTION, mtkRadioExIms::onVolteSubscriptionInd, WAKE_PARTIAL},
// M: IMS_SS end @}

// M: IMS_ViLTE start @{
    {RIL_UNSOL_VIDEO_CAPABILITY_INDICATOR, mtkRadioExIms::videoCapabilityIndicatorInd, WAKE_PARTIAL},
    {RIL_UNSOL_VIDEO_RINGTONE_EVENT_IND, mtkRadioExIms::videoRingtoneEventInd, WAKE_PARTIAL},
    {RIL_UNSOL_MD_INTERNET_USAGE_IND, mtkRadioExIms::mdInternetUsageIndication, WAKE_PARTIAL},
// M: IMS_ViLTE end @}

// M: IMS_CC start @{
    {RIL_UNSOL_ECT_INDICATION, mtkRadioExIms::ectIndicationInd, WAKE_PARTIAL},
    {RIL_UNSOL_CALL_INFO_INDICATION, mtkRadioExIms::callInfoIndicationInd, WAKE_PARTIAL},
    {RIL_UNSOL_CALLMOD_CHANGE_INDICATOR, mtkRadioExIms::callmodChangeIndicatorInd, WAKE_PARTIAL},
    {RIL_UNSOL_ECONF_RESULT_INDICATION, mtkRadioExIms::econfResultIndicationInd, WAKE_PARTIAL},
    {RIL_UNSOL_SIP_CALL_PROGRESS_INDICATOR, mtkRadioExIms::sipCallProgressIndicatorInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_CONFERENCE_INFO_INDICATION, mtkRadioExIms::onImsConferenceInfoIndication, WAKE_PARTIAL},
    {RIL_UNSOL_LTE_MESSAGE_WAITING_INDICATION, mtkRadioExIms::onLteMessageWaitingIndication, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_DIALOG_INDICATION, mtkRadioExIms::imsDialogIndicationInd, WAKE_PARTIAL},
    {RIL_UNSOL_NO_EMERGENCY_CALLBACK_MODE, mtkRadioExIms::noEmergencyCallbackModeInd, WAKE_PARTIAL},
    {RIL_UNSOL_REDIAL_EMERGENCY_INDICATION, mtkRadioExIms::redialEmergencyIndication, WAKE_PARTIAL},
    {RIL_UNSOL_SPEECH_CODEC_INFO, mtkRadioExIms::speechCodecInfoInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_EVENT_PACKAGE_INDICATION, mtkRadioExIms::imsEventPackageIndicationInd, WAKE_PARTIAL},
    {RIL_UNSOL_SIP_HEADER, mtkRadioExIms::sipHeaderReportInd, WAKE_PARTIAL},
    {RIL_UNSOL_CALL_RAT_INDICATION, mtkRadioExIms::callRatIndication, WAKE_PARTIAL},
    {RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY, mtkRadioExIms::emergencyBearerInfoInd, WAKE_PARTIAL},
    /// M: MT SIP invite info indication
    {RIL_UNSOL_MT_SIP_INVITE_INFO,  mtkRadioExIms::incomingCallSipInviteInd, WAKE_PARTIAL},
// M: IMS_CC end @}

// M: IMS_CONFIG  start @{
    {RIL_UNSOL_IMS_CONFIG_DYNAMIC_IMS_SWITCH_COMPLETE, mtkRadioExIms::imsCfgDynamicImsSwitchCompleteInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_CONFIG_FEATURE_CHANGED, mtkRadioExIms::imsCfgFeatureChangedInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_CONFIG_CONFIG_CHANGED, mtkRadioExIms::imsCfgConfigChangedInd, WAKE_PARTIAL},
    {RIL_UNSOL_IMS_CONFIG_CONFIG_LOADED, mtkRadioExIms::imsCfgConfigLoadedInd, WAKE_PARTIAL},
// M: IMS_CONFIG end @}

// M: IMS_WFC start @{
    {RIL_UNSOL_WIFI_RSSI_MONITORING_CONFIG, mtkRadioExIms::onWifiMonitoringThreshouldChanged, WAKE_PARTIAL},
    {RIL_UNSOL_ACTIVE_WIFI_PDN_COUNT, mtkRadioExIms::onWifiPdnActivate, WAKE_PARTIAL},
    {RIL_UNSOL_WIFI_PDN_ERROR, mtkRadioExIms::onWfcPdnError, WAKE_PARTIAL},
    {RIL_UNSOL_MOBILE_WIFI_HANDOVER, mtkRadioExIms::onPdnHandover, WAKE_PARTIAL},
    {RIL_UNSOL_MOBILE_WIFI_ROVEOUT, mtkRadioExIms::onWifiRoveout, WAKE_PARTIAL},
    {RIL_UNSOL_REQUEST_GEO_LOCATION, mtkRadioExIms::onLocationRequest, WAKE_PARTIAL},
    {RIL_UNSOL_WFC_PDN_STATE, mtkRadioExIms::onWfcPdnStateChanged, WAKE_PARTIAL},
    {RIL_UNSOL_NATT_KEEP_ALIVE_CHANGED, mtkRadioExIms::onNattKeepAliveChanged, WAKE_PARTIAL},
    {RIL_UNSOL_WIFI_PING_REQUEST, mtkRadioExIms::onWifiPingRequest, WAKE_PARTIAL},
    {RIL_UNSOL_WIFI_PDN_OOS, mtkRadioExIms::onWifiPdnOOS, WAKE_PARTIAL},
    {RIL_UNSOL_WIFI_LOCK, mtkRadioExIms::onWifiLock, WAKE_PARTIAL},
    {RIL_UNSOL_SSAC_STATUS,  mtkRadioExIms::onSsacStatus, WAKE_PARTIAL},
// M: IMS_WFC end @}

