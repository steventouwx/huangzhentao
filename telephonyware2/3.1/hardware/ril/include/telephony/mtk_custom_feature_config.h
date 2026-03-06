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

#ifndef MTK_CUSTOM_FEATURE_CONFIG_H
#define MTK_CUSTOM_FEATURE_CONFIG_H

// Define RILD customized feature here

/**
 * Config if allow ims send VoLTE/VoNR off to modem when no sim inserted.
 * Define 1 if means allow send AT+EIMSCFG=0,...
 * else define 0.
 * mtk internal default config is 0
 */
#define MTK_ALLOW_IMS_OFF_IF_SIM_ABSENT_CONFIG      0

/**
 * Config if enable entitlement provisioning config for OP15.
 * Define 1 if enabled, IMS rild will send EIMSCFG=0...before provisioning enable volte,
 * else define 0.
 * mtk internal default config is 1
 */
#define MTK_IMS_ENABLED_OP15_PROVISIONING_CONFIG      1

/**
 * Config if enabled set user agent.
 * Define 1 if enabled, IMS rild will send user agent to modem earlier after receive mccmnc changed,
 * else define 0.
 * mtk internal default config is 0
 */
#define MTK_IMS_ENABLED_SET_USER_AGENT      0

/**
 * Config WiFi support is explicitly configured in apns-conf.xml or not.
 * Define 1 if APNs in apns-conf.xml support WiFi and explicitly configured with
 * network_type_bitmask(incluing 18), else define 0.
 * When define 0, APNs that support ims/mms/xcap/emergency will be automatically
 * configured to suport WiFi. There is an exception is that there is other APN with same
 * type configured with WiFi support only.
 */
#define MTK_WIFI_APN_BITMASK_EXPLICIT_CONFIG      1

/**
 * Config default 5G nsa display rule for UE.
 * If persist.vendor.radio.nr_display_rule is defined in make file,
 * default value will be overrided, and no need to special configure here.
 * 0: config D
 * 1: config D + Cache
 */
#define MTK_DEFAULT_5G_NSA_DISPLAY_RULE      1

/**
 * enable(1) or disable(0) for urc +ENRSCS.
 */
#define MTK_URC_CONFIG_NR_SYS_INFO           0

/**
 * enable(1) or disable(0) for urc +ECHINFO.
 */
#define MTK_URC_CONFIG_CH_INFO               0

/**
 * enable(1) or disable(0) for urc +ENRCABAND.
 */
#define MTK_URC_CONFIG_NR_CA_BAND            0

/**
 * Configure value for urc +ECAINFO.
 * 0: disable URC
 * 1: enable URC
 * 2: enable URC conditional
 */
#define MTK_URC_CONFIG_CA_INFO               0

/**
 * AT&T used only.
 * config to 1 if app want to enable/disable 2G when ue call setAllowedNetworkType function,
 * and don't want to call other specail hidl/aidl interface additional.
 * eg: if app set allowed network type without 2G, we will disable 2G fisrtly and send rat mode later.
 * eg: if app set allowed network type with 2G, we will enable 2G fisrtly and send rat mode later.
 */
#define MTK_DISABLE_2G_WITH_ALLOWED_NETWORK_TYPE      0

/**
 * Config to 1 if ue want to set data sim's ps registration state as oos
 * when non-data sim is in call.
 */
#define MTK_SPECIFIC_PS_STATE_IN_CALL                 0

/**
 * Config to 1 if UE want to report data registration state as voice state
 * when voice reg state is oos and ims is registered,
 */
#define MTK_SPECIFIC_VOICE_STATE                      0

/**
 * Config to 1 if UE want to enable nw config info urc
 * 4x4mimo, qam_256, qam_ul64
 */
#define MTK_URC_CONFIG_NWCFGINFO                      0

/**
 * Config whether need AP(rild) to report OEM RAW data fail cause.
 * If modem solution is used, this configuration will be ignored.
 */
#define MTK_AP_DATA_FAIL_CAUSE_REPORT 0

/**
 * Config whether need to auto approve incoming call indication (+EAIC) in rild.
 */
#define MTK_AUTO_APPROVE_INCOMING_CALL_INDICATION 0

/**
 * Config whether need AP(rild) to report OEM RAW for CS call disconnect cause.
 * If modem solution is used, this configuration will be ignored.
 */
#define MTK_AP_CS_CALL_DISCONNECT_CAUSE_REPORT 0

/** Config whether send AT+EMCS =1 before turn on radio in RILD.
 * for the project which don't support EMCI.
 */
#define MTK_SEND_EMCS_BEFORE_TURN_ON_RADIO_IN_RILD  0

/**
 * Config whether need AP(rild) to report OEM RAW for IMS call disconnect cause.
 * If modem solution is used, this configuration will be ignored.
 */
#define MTK_AP_IMS_CALL_DISCONNECT_CAUSE_REPORT 0

/**
 * Config whether convert terminated type to notify type.
 * If it's 1, it will always notify user by dialog, ingore the message.
 * else, if message is not null, it can notify user.
 */
#define MTK_USSD_CONVERT_TERMINATED_TO_NOTIFY 1

/**
 * Config if allow notify customization sim refresh type to SIM FW.
 * Define 1 if SIM Framwork can handle both types SIM_INIT_FILE_CHANGE
 * and SIM_INIT_FULL_FILE_CHANGE, else define 0.
 * mtk internal default config is 1.
 */
#define MTK_ALLOW_NOTIFY_CUSTOM_SIMREFRESH_TYPE_CONFIG 1

/**
 * For the scenario as below, currently only for CBRS and Auto Data Switch validation.
 * 1. DDS setup internet pdn
 * 2. NON-DDS setup internet pdn
 * 3. Transmit small amount of packages on NON-DDS
 * By check this key to send command for OCCUPY RF to make sure data
 * can be susscefully transmitted in step3.
 *
 * Should align with Framework configuration of auto_data_switch_ping_test_before_switch.
 */
#define MTK_NON_DDS_RF_OCCUPY_SUPPORT 1

#endif /*MTK_CUSTOM_FEATURE_CONFIG_H*/
