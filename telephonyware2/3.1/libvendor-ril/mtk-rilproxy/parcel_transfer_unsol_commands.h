// SPDX-License-Identifier: MediaTekProprietary
/*
*  Copyright (C) 2014 MediaTek Inc.
*
*  Modification based on code covered by the below mentioned copyright
*  and/or permission notice(s).
*/


/* //hardware/ril/librilmtk/ril_commands.h
**
** Copyright 2014, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
    {RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_NEW_SMS, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM, tranferUnsolRaw},
    {RIL_UNSOL_ON_USSD, tranferUnsolRaw},
    {RIL_UNSOL_ON_USSD_REQUEST, tranferUnsolRaw},
    {RIL_UNSOL_NITZ_TIME_RECEIVED, tranferUnsolRaw},
    {RIL_UNSOL_SIGNAL_STRENGTH, tranferUnsolRaw},
    {RIL_UNSOL_DATA_CALL_LIST_CHANGED, tranferUnsolDataCallList},
    {RIL_UNSOL_SUPP_SVC_NOTIFICATION, tranferUnsolRaw},
    {RIL_UNSOL_STK_SESSION_END, tranferUnsolRaw},
    {RIL_UNSOL_STK_PROACTIVE_COMMAND, tranferUnsolRaw},
    {RIL_UNSOL_STK_EVENT_NOTIFY, tranferUnsolRaw},
    {RIL_UNSOL_STK_CALL_SETUP, tranferUnsolRaw},
    {RIL_UNSOL_SIM_SMS_STORAGE_FULL, tranferUnsolRaw},
    {RIL_UNSOL_SIM_REFRESH, tranferUnsolRaw},
    {RIL_UNSOL_CALL_RING, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_CDMA_NEW_SMS, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_NEW_BROADCAST_SMS, tranferUnsolRaw},
    {RIL_UNSOL_CDMA_RUIM_SMS_STORAGE_FULL, tranferUnsolRaw},
    {RIL_UNSOL_RESTRICTED_STATE_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE, tranferUnsolRaw},
    {RIL_UNSOL_CDMA_CALL_WAITING, tranferUnsolRaw},
    {RIL_UNSOL_CDMA_OTA_PROVISION_STATUS, tranferUnsolRaw},
    {RIL_UNSOL_CDMA_INFO_REC, tranferUnsolRaw},
    {RIL_UNSOL_OEM_HOOK_RAW, tranferUnsolRaw},
    {RIL_UNSOL_RINGBACK_TONE, tranferUnsolRaw},
    {RIL_UNSOL_RESEND_INCALL_MUTE, tranferUnsolRaw},
    {RIL_UNSOL_CDMA_SUBSCRIPTION_SOURCE_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_CDMA_PRL_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE, tranferUnsolRaw},
    {RIL_UNSOL_RIL_CONNECTED, tranferUnsolRilConnected},
    {RIL_UNSOL_VOICE_RADIO_TECH_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_CELL_INFO_LIST, tranferUnsolRaw},
    {RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_SRVCC_STATE_NOTIFY, tranferUnsolRaw},
    {RIL_UNSOL_HARDWARE_CONFIG_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_DC_RT_INFO_CHANGED, tranferUnsolRaw},
    {RIL_UNSOL_RADIO_CAPABILITY, tranferUnsolRaw},
    {RIL_UNSOL_ON_SS, tranferUnsolRaw},
    {RIL_UNSOL_STK_CC_ALPHA_NOTIFY, tranferUnsolRaw},
    {RIL_UNSOL_LCEDATA_RECV, tranferUnsolRaw},
    {RIL_UNSOL_PCO_DATA, NULL},
    {RIL_UNSOL_MODEM_RESTART, NULL},
    {RIL_UNSOL_CARRIER_INFO_IMSI_ENCRYPTION, NULL},
    {RIL_UNSOL_NETWORK_SCAN_RESULT, NULL},
    {RIL_UNSOL_ICC_SLOT_STATUS, NULL},
    {RIL_UNSOL_KEEPALIVE_STATUS, tranferUnsolRaw},

