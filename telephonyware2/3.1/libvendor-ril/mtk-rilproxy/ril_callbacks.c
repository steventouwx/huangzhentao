// SPDX-License-Identifier: MediaTekProprietary
/* //device/system/reference-ril/reference-ril.c
**
** Copyright 2006, The Android Open Source Project
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

#include <../include/telephony/ril_cdma_sms.h>
#include <../include/telephony/librilutils.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <alloca.h>
#include "atchannel.h"
#include "at_tok.h"
#include "misc.h"
#include <getopt.h>
#include <sys/socket.h>
#include <cutils/sockets.h>
#include <termios.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <../include/telephony/ril.h>
#include <../include/telephony/mtk_ril_sp.h>
#include "../include/hardware/qemu_pipe.h"
#include "Rfx.h"
#include "socket_ext_api.h"
#ifdef __SAP_API__
#include <vendor/mediatek/proprietary/hardware/ril/platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
/// @}
#include "pb_decode.h"
#include "pb_encode.h"
#endif
#include <mtk_log.h>

#if defined(__ANDROID__)
#include <prop/properties.h>
#else
#include <signal.h>
#endif

#define LOG_TAG "RIL"
static RIL_RadioState sState = RADIO_STATE_UNAVAILABLE;

#if (SIM_COUNT >= 2)
static RIL_RadioState sState2 = RADIO_STATE_UNAVAILABLE;
#endif
#if (SIM_COUNT >= 3) /* Gemini plus 3 SIM*/
static RIL_RadioState sState3 = RADIO_STATE_UNAVAILABLE;
#endif
#if (SIM_COUNT >= 4) /* Gemini plus 4 SIM*/
static RIL_RadioState sState4 = RADIO_STATE_UNAVAILABLE;
#endif
static pthread_mutex_t s_state_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_state_cond = PTHREAD_COND_INITIALIZER;

static int s_port = -1;
static const char * s_device_path = NULL;
static int          s_device_socket = 0;
/* trigger change to this with s_state_cond */
static int s_closed = 0;

static void usage(char *s)
{
#ifdef RIL_SHLIB
    fprintf(stderr, "rfx-ril requires: -p <tcp port> or -d /dev/tty_device\n");
#else
    fprintf(stderr, "usage: %s [-p <tcp port>] [-d /dev/tty_device]\n", s);
    exit(-1);
#endif
}
static void *
mainLoop(void *param)
{
    AT_DUMP("== ", "entering mainLoop()", -1 );

    rfx_init();
    /// MT2635: add parcel to data resposne
    rfx_check_table_index();
    ril_socket_init();
    RLOGI("Finish ril socket initialization");

    // no need to trigger TRM when application address invalid.
    //registerSignal();
    return 0;
}
void setRadioState(RIL_RadioState newState, RIL_SOCKET_ID rid);

const char * requestToString(int request) {
/*
 cat libs/telephony/ril_commands.h \
 | egrep "^ *{RIL_" \
 | sed -re 's/\{RIL_([^,]+),[^,]+,([^}]+).+/case RIL_\1: return "\1";/'


 cat libs/telephony/ril_unsol_commands.h \
 | egrep "^ *{RIL_" \
 | sed -re 's/\{RIL_([^,]+),([^}]+).+/case RIL_\1: return "\1";/'


*/
    switch(request) {
        case RIL_REQUEST_GET_SIM_STATUS: return "GET_SIM_STATUS";
        case RIL_REQUEST_ENTER_SIM_PIN: return "ENTER_SIM_PIN";
        case RIL_REQUEST_ENTER_SIM_PUK: return "ENTER_SIM_PUK";
        case RIL_REQUEST_ENTER_SIM_PIN2: return "ENTER_SIM_PIN2";
        case RIL_REQUEST_ENTER_SIM_PUK2: return "ENTER_SIM_PUK2";
        case RIL_REQUEST_CHANGE_SIM_PIN: return "CHANGE_SIM_PIN";
        case RIL_REQUEST_CHANGE_SIM_PIN2: return "CHANGE_SIM_PIN2";
        case RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION: return "ENTER_NETWORK_DEPERSONALIZATION";
        case RIL_REQUEST_GET_CURRENT_CALLS: return "GET_CURRENT_CALLS";
        case RIL_REQUEST_DIAL: return "DIAL";
        case RIL_REQUEST_GET_IMSI: return "GET_IMSI";
        case RIL_REQUEST_HANGUP: return "HANGUP";
        case RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND: return "HANGUP_WAITING_OR_BACKGROUND";
        case RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND: return "HANGUP_FOREGROUND_RESUME_BACKGROUND";
        case RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE: return "SWITCH_WAITING_OR_HOLDING_AND_ACTIVE";
        case RIL_REQUEST_CONFERENCE: return "CONFERENCE";
        case RIL_REQUEST_UDUB: return "UDUB";
        case RIL_REQUEST_LAST_CALL_FAIL_CAUSE: return "LAST_CALL_FAIL_CAUSE";
        case RIL_REQUEST_SIGNAL_STRENGTH: return "SIGNAL_STRENGTH";
        case RIL_REQUEST_VOICE_REGISTRATION_STATE: return "VOICE_REGISTRATION_STATE";
        case RIL_REQUEST_DATA_REGISTRATION_STATE: return "DATA_REGISTRATION_STATE";
        case RIL_REQUEST_OPERATOR: return "OPERATOR";
        case RIL_REQUEST_RADIO_POWER: return "RADIO_POWER";
        case RIL_REQUEST_SET_PREFERRED_DATA_MODEM: return "SET_PREFERRED_DATA_MODEM";
        case RIL_REQUEST_DTMF: return "DTMF";
        case RIL_REQUEST_SEND_SMS: return "SEND_SMS";
        case RIL_REQUEST_SEND_SMS_EXPECT_MORE: return "SEND_SMS_EXPECT_MORE";
        case RIL_REQUEST_SETUP_DATA_CALL: return "SETUP_DATA_CALL";
        case RIL_REQUEST_SIM_IO: return "SIM_IO";
        case RIL_REQUEST_SEND_USSD: return "SEND_USSD";
        ///M: For query CNAP
        case RIL_REQUEST_SEND_CNAP: return "SEND_CNAP";
        case RIL_REQUEST_CANCEL_USSD: return "CANCEL_USSD";
        case RIL_REQUEST_GET_CLIR: return "GET_CLIR";
        case RIL_REQUEST_SET_CLIR: return "SET_CLIR";
        case RIL_REQUEST_QUERY_CALL_FORWARD_STATUS: return "QUERY_CALL_FORWARD_STATUS";
        case RIL_REQUEST_SET_CALL_FORWARD: return "SET_CALL_FORWARD";
        case RIL_REQUEST_QUERY_CALL_WAITING: return "QUERY_CALL_WAITING";
        case RIL_REQUEST_SET_CALL_WAITING: return "SET_CALL_WAITING";
        case RIL_REQUEST_SMS_ACKNOWLEDGE: return "SMS_ACKNOWLEDGE";
        case RIL_REQUEST_GET_IMEI: return "GET_IMEI";
        case RIL_REQUEST_GET_IMEISV: return "GET_IMEISV";
        case RIL_REQUEST_ANSWER: return "ANSWER";
        case RIL_REQUEST_DEACTIVATE_DATA_CALL: return "DEACTIVATE_DATA_CALL";
        case RIL_REQUEST_QUERY_FACILITY_LOCK: return "QUERY_FACILITY_LOCK";
        case RIL_REQUEST_SET_FACILITY_LOCK: return "SET_FACILITY_LOCK";
        case RIL_REQUEST_CHANGE_BARRING_PASSWORD: return "CHANGE_BARRING_PASSWORD";
        case RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE: return "QUERY_NETWORK_SELECTION_MODE";
        case RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC: return "SET_NETWORK_SELECTION_AUTOMATIC";
        case RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL: return "SET_NETWORK_SELECTION_MANUAL";
        case RIL_REQUEST_QUERY_AVAILABLE_NETWORKS : return "QUERY_AVAILABLE_NETWORKS ";
        case RIL_REQUEST_DTMF_START: return "DTMF_START";
        case RIL_REQUEST_DTMF_STOP: return "DTMF_STOP";
        case RIL_REQUEST_BASEBAND_VERSION: return "BASEBAND_VERSION";
        case RIL_REQUEST_SEPARATE_CONNECTION: return "SEPARATE_CONNECTION";
        case RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE: return "SET_PREFERRED_NETWORK_TYPE";
        case RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE: return "GET_PREFERRED_NETWORK_TYPE";
        case RIL_REQUEST_GET_NEIGHBORING_CELL_IDS: return "GET_NEIGHBORING_CELL_IDS";
        case RIL_REQUEST_SET_MUTE: return "SET_MUTE";
        case RIL_REQUEST_GET_MUTE: return "GET_MUTE";
        case RIL_REQUEST_QUERY_CLIP: return "QUERY_CLIP";
        case RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE: return "LAST_DATA_CALL_FAIL_CAUSE";
        case RIL_REQUEST_DATA_CALL_LIST: return "DATA_CALL_LIST";
        case RIL_REQUEST_RESET_RADIO: return "RESET_RADIO";
        case RIL_REQUEST_OEM_HOOK_RAW: return "OEM_HOOK_RAW";
        case RIL_REQUEST_OEM_HOOK_STRINGS: return "OEM_HOOK_STRINGS";
        case RIL_REQUEST_SET_BAND_MODE: return "SET_BAND_MODE";
        case RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE: return "QUERY_AVAILABLE_BAND_MODE";
        case RIL_REQUEST_STK_GET_PROFILE: return "STK_GET_PROFILE";
        case RIL_REQUEST_STK_SET_PROFILE: return "STK_SET_PROFILE";
        case RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND: return "STK_SEND_ENVELOPE_COMMAND";
        case RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE: return "STK_SEND_TERMINAL_RESPONSE";
        case RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM: return "STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM";
        case RIL_REQUEST_SCREEN_STATE: return "SCREEN_STATE";
        case RIL_REQUEST_EXPLICIT_CALL_TRANSFER: return "EXPLICIT_CALL_TRANSFER";
        case RIL_REQUEST_SET_LOCATION_UPDATES: return "SET_LOCATION_UPDATES";
        case RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE:return"CDMA_SET_SUBSCRIPTION_SOURCE";
        case RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE:return"CDMA_SET_ROAMING_PREFERENCE";
        case RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE:return"CDMA_QUERY_ROAMING_PREFERENCE";
        case RIL_REQUEST_SET_TTY_MODE:return"SET_TTY_MODE";
        case RIL_REQUEST_QUERY_TTY_MODE:return"QUERY_TTY_MODE";
        case RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE:return"CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE";
        case RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE:return"CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE";
        case RIL_REQUEST_CDMA_FLASH:return"CDMA_FLASH";
        case RIL_REQUEST_CDMA_BURST_DTMF:return"CDMA_BURST_DTMF";
        case RIL_REQUEST_CDMA_SEND_SMS:return"CDMA_SEND_SMS";
        case RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE:return"CDMA_SMS_ACKNOWLEDGE";
        case RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG:return"GSM_GET_BROADCAST_SMS_CONFIG";
        case RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG:return"GSM_SET_BROADCAST_SMS_CONFIG";
        case RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG:return "CDMA_GET_BROADCAST_SMS_CONFIG";
        case RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG:return "CDMA_SET_BROADCAST_SMS_CONFIG";
        case RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION:return "CDMA_SMS_BROADCAST_ACTIVATION";
        case RIL_REQUEST_CDMA_VALIDATE_AND_WRITE_AKEY: return"CDMA_VALIDATE_AND_WRITE_AKEY";
        case RIL_REQUEST_CDMA_SUBSCRIPTION: return"CDMA_SUBSCRIPTION";
        case RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM: return "CDMA_WRITE_SMS_TO_RUIM";
        case RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM: return "CDMA_DELETE_SMS_ON_RUIM";
        case RIL_REQUEST_DEVICE_IDENTITY: return "DEVICE_IDENTITY";
        case RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE: return "EXIT_EMERGENCY_CALLBACK_MODE";
        case RIL_REQUEST_GET_SMSC_ADDRESS: return "GET_SMSC_ADDRESS";
        case RIL_REQUEST_SET_SMSC_ADDRESS: return "SET_SMSC_ADDRESS";
        case RIL_REQUEST_REPORT_SMS_MEMORY_STATUS: return "REPORT_SMS_MEMORY_STATUS";
        case RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING: return "REPORT_STK_SERVICE_IS_RUNNING";
        case RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE: return "CDMA_GET_SUBSCRIPTION_SOURCE";
        case RIL_REQUEST_ISIM_AUTHENTICATION: return "ISIM_AUTHENTICATION";
        case RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU: return "RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU";
        case RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS: return "RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS";
        case RIL_REQUEST_VOICE_RADIO_TECH: return "VOICE_RADIO_TECH";
        case RIL_REQUEST_GET_CELL_INFO_LIST: return"GET_CELL_INFO_LIST";
        case RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE: return"SET_UNSOL_CELL_INFO_LIST_RATE";
        case RIL_REQUEST_SET_INITIAL_ATTACH_APN: return "RIL_REQUEST_SET_INITIAL_ATTACH_APN";
        case RIL_REQUEST_IMS_REGISTRATION_STATE: return "IMS_REGISTRATION_STATE";
        case RIL_REQUEST_IMS_SEND_SMS: return "IMS_SEND_SMS";
        case RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC: return "SIM_TRANSMIT_APDU_BASIC";
        case RIL_REQUEST_SIM_OPEN_CHANNEL: return "SIM_OPEN_CHANNEL";
        case RIL_REQUEST_SIM_CLOSE_CHANNEL: return "SIM_CLOSE_CHANNEL";
        case RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL: return "SIM_TRANSMIT_APDU_CHANNEL";
        case RIL_REQUEST_GET_RADIO_CAPABILITY: return "RIL_REQUEST_GET_RADIO_CAPABILITY";
        case RIL_REQUEST_SET_RADIO_CAPABILITY: return "RIL_REQUEST_SET_RADIO_CAPABILITY";
        case RIL_REQUEST_START_LCE: return "RIL_REQUEST_START_LCE";
        case RIL_REQUEST_STOP_LCE: return "RIL_REQUEST_STOP_LCE";
        case RIL_REQUEST_PULL_LCEDATA: return "RIL_REQUEST_PULL_LCEDATA";
        case RIL_REQUEST_GET_ACTIVITY_INFO: return "RIL_REQUEST_GET_ACTIVITY_INFO";
        case RIL_REQUEST_SET_UICC_SUBSCRIPTION: return "SET_UICC_SUBSCRIPTION";
        case RIL_REQUEST_ALLOW_DATA: return "ALLOW_DATA";
        case RIL_REQUEST_GET_HARDWARE_CONFIG: return "GET_HARDWARE_CONFIG";
        case RIL_REQUEST_SIM_AUTHENTICATION: return "SIM_AUTHENTICATION";
        case RIL_REQUEST_GET_DC_RT_INFO: return "GET_DC_RT_INFO";
        case RIL_REQUEST_SET_DC_RT_INFO_RATE: return "SET_DC_RT_INFO_RATE";
        case RIL_REQUEST_SET_DATA_PROFILE: return "SET_DATA_PROFILE";
        // Added by M begin
        case RIL_REQUEST_QUERY_SIM_NETWORK_LOCK: return "RIL_REQUEST_QUERY_SIM_NETWORK_LOCK";
        case RIL_REQUEST_SET_SIM_NETWORK_LOCK: return "RIL_REQUEST_SET_SIM_NETWORK_LOCK";
        case RIL_REQUEST_GENERAL_SIM_AUTH: return "RIL_REQUEST_GENERAL_SIM_AUTH";
        case RIL_REQUEST_SIM_GET_ATR: return "SIM_GET_ATR";
     // Added by M end
        case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED: return "UNSOL_RESPONSE_RADIO_STATE_CHANGED";
        case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED: return "UNSOL_RESPONSE_CALL_STATE_CHANGED";
        case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED: return "UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED";
        case RIL_UNSOL_RESPONSE_NEW_SMS: return "UNSOL_RESPONSE_NEW_SMS";
        case RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT: return "UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT";
        case RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM: return "UNSOL_RESPONSE_NEW_SMS_ON_SIM";
        case RIL_UNSOL_ON_USSD: return "UNSOL_ON_USSD";
        case RIL_UNSOL_ON_USSD_REQUEST: return "UNSOL_ON_USSD_REQUEST(obsolete)";
        case RIL_UNSOL_NITZ_TIME_RECEIVED: return "UNSOL_NITZ_TIME_RECEIVED";
        case RIL_UNSOL_SIGNAL_STRENGTH: return "UNSOL_SIGNAL_STRENGTH";
        case RIL_UNSOL_STK_SESSION_END: return "UNSOL_STK_SESSION_END";
        case RIL_UNSOL_STK_PROACTIVE_COMMAND: return "UNSOL_STK_PROACTIVE_COMMAND";
        case RIL_UNSOL_STK_EVENT_NOTIFY: return "UNSOL_STK_EVENT_NOTIFY";
        case RIL_UNSOL_STK_CALL_SETUP: return "UNSOL_STK_CALL_SETUP";
        case RIL_UNSOL_SIM_SMS_STORAGE_FULL: return "UNSOL_SIM_SMS_STORAGE_FUL";
        case RIL_UNSOL_SIM_REFRESH: return "UNSOL_SIM_REFRESH";
        case RIL_UNSOL_DATA_CALL_LIST_CHANGED: return "UNSOL_DATA_CALL_LIST_CHANGED";
        case RIL_UNSOL_CALL_RING: return "UNSOL_CALL_RING";
        case RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED: return "UNSOL_RESPONSE_SIM_STATUS_CHANGED";
        case RIL_UNSOL_RESPONSE_CDMA_NEW_SMS: return "UNSOL_NEW_CDMA_SMS";
        case RIL_UNSOL_RESPONSE_NEW_BROADCAST_SMS: return "UNSOL_NEW_BROADCAST_SMS";
        case RIL_UNSOL_CDMA_RUIM_SMS_STORAGE_FULL: return "UNSOL_CDMA_RUIM_SMS_STORAGE_FULL";
        case RIL_UNSOL_RESTRICTED_STATE_CHANGED: return "UNSOL_RESTRICTED_STATE_CHANGED";
        case RIL_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE: return "UNSOL_ENTER_EMERGENCY_CALLBACK_MODE";
        case RIL_UNSOL_CDMA_CALL_WAITING: return "UNSOL_CDMA_CALL_WAITING";
        case RIL_UNSOL_CDMA_OTA_PROVISION_STATUS: return "UNSOL_CDMA_OTA_PROVISION_STATUS";
        case RIL_UNSOL_CDMA_INFO_REC: return "UNSOL_CDMA_INFO_REC";
        case RIL_UNSOL_OEM_HOOK_RAW: return "UNSOL_OEM_HOOK_RAW";
        case RIL_UNSOL_RINGBACK_TONE: return "UNSOL_RINGBACK_TONE";
        case RIL_UNSOL_RESEND_INCALL_MUTE: return "UNSOL_RESEND_INCALL_MUTE";
        case RIL_UNSOL_CDMA_SUBSCRIPTION_SOURCE_CHANGED: return "UNSOL_CDMA_SUBSCRIPTION_SOURCE_CHANGED";
        case RIL_UNSOL_CDMA_PRL_CHANGED: return "UNSOL_CDMA_PRL_CHANGED";
        case RIL_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE: return "UNSOL_EXIT_EMERGENCY_CALLBACK_MODE";
        case RIL_UNSOL_RIL_CONNECTED: return "UNSOL_RIL_CONNECTED";
        case RIL_UNSOL_VOICE_RADIO_TECH_CHANGED: return "UNSOL_VOICE_RADIO_TECH_CHANGED";
        case RIL_UNSOL_CELL_INFO_LIST: return "UNSOL_CELL_INFO_LIST";
        case RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED: return "RESPONSE_IMS_NETWORK_STATE_CHANGED";
        case RIL_UNSOL_IMS_REGISTRATION_INFO: return "RIL_UNSOL_IMS_REGISTRATION_INFO";
        case RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED: return "UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED";
        case RIL_UNSOL_SRVCC_STATE_NOTIFY: return "UNSOL_SRVCC_STATE_NOTIFY";
        case RIL_UNSOL_HARDWARE_CONFIG_CHANGED: return "HARDWARE_CONFIG_CHANGED";
        case RIL_UNSOL_DC_RT_INFO_CHANGED: return "UNSOL_DC_RT_INFO_CHANGED";
        case RIL_REQUEST_SHUTDOWN: return "SHUTDOWN";
        case RIL_UNSOL_RADIO_CAPABILITY: return "RIL_UNSOL_RADIO_CAPABILITY";
        // Added by M begin
        case RIL_UNSOL_SIM_MISSING: return "UNSOL_SIM_MISSING";
        case RIL_UNSOL_SIM_RECOVERY: return "RIL_UNSOL_SIM_RECOVERY";
        case RIL_UNSOL_VIRTUAL_SIM_ON: return "RIL_UNSOL_VIRTUAL_SIM_ON";
        case RIL_UNSOL_VIRTUAL_SIM_OFF: return "RIL_UNSOL_VIRTUAL_SIM_OFF";
        case RIL_UNSOL_SIM_PLUG_OUT: return "RIL_UNSOL_SIM_PLUG_OUT";
        case RIL_UNSOL_SIM_PLUG_IN: return "RIL_UNSOL_SIM_PLUG_IN";
        case RIL_UNSOL_TRAY_PLUG_IN: return "RIL_UNSOL_TRAY_PLUG_IN";
        case RIL_UNSOL_SIM_COMMON_SLOT_NO_CHANGED: return "RIL_UNSOL_SIM_COMMON_SLOT_NO_CHANGED";
        //case RIL_UNSOL_EUSIM_READY: return "RIL_UNSOL_EUSIM_READY";
        case RIL_UNSOL_DATA_ALLOWED: return "RIL_UNSOL_DATA_ALLOWED";
        case RIL_REQUEST_QUERY_PHB_STORAGE_INFO: return "QUERY_PHB_STORAGE_INFO";
        case RIL_REQUEST_WRITE_PHB_ENTRY: return "WRITE_PHB_ENTRY";
        case RIL_REQUEST_READ_PHB_ENTRY: return "READ_PHB_ENTRY";
        case RIL_REQUEST_QUERY_UPB_CAPABILITY: return "RIL_REQUEST_QUERY_UPB_CAPABILITY";
        case RIL_REQUEST_EDIT_UPB_ENTRY: return "RIL_REQUEST_EDIT_UPB_ENTRY";
        case RIL_REQUEST_DELETE_UPB_ENTRY: return "RIL_REQUEST_DELETE_UPB_ENTRY";
        case RIL_REQUEST_READ_UPB_GAS_LIST: return "RIL_REQUEST_READ_UPB_GAS_LIST";
        case RIL_REQUEST_READ_UPB_GRP: return "RIL_REQUEST_READ_UPB_GRP";
        case RIL_REQUEST_WRITE_UPB_GRP: return "RIL_REQUEST_WRITE_UPB_GRP";
        case RIL_REQUEST_GET_PHB_STRING_LENGTH: return "RIL_REQUEST_GET_PHB_STRING_LENGTH";
        case RIL_REQUEST_GET_PHB_MEM_STORAGE: return "RIL_REQUEST_GET_PHB_MEM_STORAGE";
        case RIL_REQUEST_SET_PHB_MEM_STORAGE: return "RIL_REQUEST_SET_PHB_MEM_STORAGE";
        case RIL_REQUEST_READ_PHB_ENTRY_EXT: return "RIL_REQUEST_READ_PHB_ENTRY_EXT";
        case RIL_REQUEST_WRITE_PHB_ENTRY_EXT: return "RIL_REQUEST_WRITE_PHB_ENTRY_EXT";
        case RIL_REQUEST_RESUME_REGISTRATION: return "RIL_REQUEST_RESUME_REGISTRATION";
        case RIL_UNSOL_RESPONSE_PLMN_CHANGED: return "RIL_UNSOL_RESPONSE_PLMN_CHANGED";
        case RIL_UNSOL_RESPONSE_REGISTRATION_SUSPENDED: return "RIL_UNSOL_RESPONSE_REGISTRATION_SUSPENDED";
        case RIL_REQUEST_SET_IMS_ENABLE: return "RIL_REQUEST_SET_IMS_ENABLE";
        case RIL_REQUEST_SET_IMSCFG: return "RIL_REQUEST_SET_IMSCFG";
         // Remote SIM ME Lock Start
        case RIL_UNSOL_MELOCK_NOTIFICATION: return "RIL_UNSOL_MELOCK_NOTIFICATION";
        // Remote SIM ME Lock End
        // External SIM [Start]
        case RIL_UNSOL_VSIM_OPERATION_INDICATION: return "RIL_UNSOL_VSIM_OPERATION_INDICATION";
        // External SIM [End]

        // ECC retry.
        case RIL_UNSOL_STK_SETUP_MENU_RESET: return "RIL_UNSOL_STK_SETUP_MENU_RESET";
        // Added by M end
        // IMS
        case RIL_UNSOL_IMS_ENABLE_DONE: return "RIL_UNSOL_IMS_ENABLE_DONE";
        case RIL_UNSOL_IMS_DISABLE_DONE: return "RIL_UNSOL_IMS_DISABLE_DONE";

        //VOLTE data
        case RIL_UNSOL_DEDICATE_BEARER_ACTIVATED: return "RIL_UNSOL_DEDICATE_BEARER_ACTIVATED";
        case RIL_UNSOL_DEDICATE_BEARER_MODIFIED: return "RIL_UNSOL_DEDICATE_BEARER_MODIFIED";
        case RIL_UNSOL_DEDICATE_BEARER_DEACTIVATED: return "RIL_UNSOL_DEDICATE_BEARER_DEACTIVATED";
        // M: Fast Dormancy
        case RIL_REQUEST_SET_FD_MODE: return "RIL_REQUEST_SET_FD_MODE";
        // SMS part, porting start
        case RIL_REQUEST_WRITE_SMS_TO_SIM: return "RIL_REQUEST_WRITE_SMS_TO_SIM";
        case RIL_REQUEST_DELETE_SMS_ON_SIM: return "RIL_REQUEST_DELETE_SMS_ON_SIM";
        case RIL_REQUEST_GET_SMS_PARAMS: return "RIL_REQUEST_GET_SMS_PARAMS";
        case RIL_REQUEST_SET_SMS_PARAMS: return "RIL_REQUEST_SET_SMS_PARAMS";
        case RIL_REQUEST_GET_SMS_SIM_MEM_STATUS: return "RIL_REQUEST_GET_SMS_SIM_MEM_STATUS";
        case RIL_REQUEST_SET_ETWS: return "RIL_REQUEST_SET_ETWS";
        case RIL_REQUEST_REMOVE_CB_MESSAGE: return "RIL_REQUEST_REMOVE_CB_MESSAGE";
        // SMS part, porting end

        ///M: MTK added Network part start
        case RIL_REQUEST_SET_TRM: return "RIL_REQUEST_SET_TRM";
        case RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT : return "QUERY_AVAILABLE_NETWORKS_WITH_ACT";
        case RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT: return "SET_NETWORK_SELECTION_MANUAL_WIT_ACT";
        case RIL_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS : return "ABORT_QUERY_AVAILABLE_NETWORKS";
        case RIL_REQUEST_GET_FEMTOCELL_LIST: return "RIL_REQUEST_GET_FEMTOCELL_LIST";
        case RIL_REQUEST_ABORT_FEMTOCELL_LIST: return "RIL_REQUEST_ABORT_FEMTOCELL_LIST";
        case RIL_REQUEST_SELECT_FEMTOCELL: return "RIL_REQUEST_SELECT_FEMTOCELL";
        case RIL_REQUEST_GET_POL_CAPABILITY: return "RIL_REQUEST_GET_POL_CAPABILITY";
        case RIL_REQUEST_GET_POL_LIST: return "RIL_REQUEST_GET_POL_LIST";
        case RIL_REQUEST_SET_POL_ENTRY: return "RIL_REQUEST_SET_POL_ENTRY";
        //case RIL_UNSOL_NEIGHBORING_CELL_INFO: return "UNSOL_NEIGHBORING_CELL_INFO";
        case RIL_UNSOL_NETWORK_INFO: return "UNSOL_NETWORK_INFO";
        case RIL_UNSOL_FEMTOCELL_INFO: return "UNSOL_FEMTOCELL_INFO";
        case RIL_UNSOL_INVALID_SIM: return "UNSOL_INVALID_SIM";
        case RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED: return "UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED";
        ///M: MTK added Network part end


        /* M: call control part start */
        case RIL_REQUEST_HANGUP_ALL: return "HANGUP_ALL";
        case RIL_REQUEST_FORCE_RELEASE_CALL: return "FORCE_RELEASE_CALL";
        case RIL_REQUEST_SET_CALL_INDICATION: return "SET_CALL_INDICATION";
        case RIL_REQUEST_EMERGENCY_DIAL: return "EMERGENCY_DIAL";
        //case RIL_REQUEST_SET_ECC_SERVICE_CATEGORY: return "SET_ECC_SERVICE_CATEGORY";
        case RIL_REQUEST_SET_ECC_LIST: return "SET_ECC_LIST";
        //case RIL_REQUEST_SET_SPEECH_CODEC_INFO: return "SET_SPEECH_CODEC_INFO";

        case RIL_UNSOL_CALL_FORWARDING: return "UNSOL_CALL_FORWARDING";
        case RIL_UNSOL_SUPP_SVC_NOTIFICATION: return "UNSOL_SUPP_SVC_NOTIFICATION";
        case RIL_UNSOL_CRSS_NOTIFICATION: return "UNSOL_CRSS_NOTIFICATION";
        case RIL_UNSOL_INCOMING_CALL_INDICATION: return "UNSOL_INCOMING_CALL_INDICATION";
        case RIL_UNSOL_CIPHER_INDICATION: return "UNSOL_CIPHER_INDICATION";
        case RIL_UNSOL_SPEECH_CODEC_INFO: return "UNSOL_SPEECH_CODEC_INFO";
        /* M: call control part end */

        /// M: IMS feature. @{
        case RIL_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER: return "RIL_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER";
        case RIL_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER: return "RIL_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER";
        case RIL_REQUEST_DIAL_WITH_SIP_URI: return "RIL_REQUEST_DIAL_WITH_SIP_URI";
        case RIL_REQUEST_HOLD_CALL: return "RIL_REQUEST_HOLD_CALL";
        case RIL_REQUEST_RESUME_CALL: return "RIL_REQUEST_RESUME_CALL";
        case RIL_UNSOL_ECONF_SRVCC_INDICATION: return "RIL_UNSOL_ECONF_SRVCC_INDICATION";
        case RIL_UNSOL_ECONF_RESULT_INDICATION: return "RIL_UNSOL_ECONF_RESULT_INDICATION";
        /// @}

        case RIL_REQUEST_MODEM_POWEROFF: return "MODEM_POWEROFF";
        case RIL_REQUEST_MODEM_POWERON: return "MODEM_POWERON";
        // M : CC33
        case RIL_REQUEST_SET_REMOVE_RESTRICT_EUTRAN_MODE: return "RIL_REQUEST_SET_REMOVE_RESTRICT_EUTRAN_MODE";
        case RIL_UNSOL_REMOVE_RESTRICT_EUTRAN: return "RIL_UNSOL_REMOVE_RESTRICT_EUTRAN";

        case RIL_UNSOL_SIP_CALL_PROGRESS_INDICATOR: return "RIL_UNSOL_SIP_CALL_PROGRESS_INDICATOR";
        case RIL_UNSOL_CALLMOD_CHANGE_INDICATOR: return "RIL_UNSOL_CALLMOD_CHANGE_INDICATOR";
        case RIL_UNSOL_VIDEO_CAPABILITY_INDICATOR: return "RIL_UNSOL_VIDEO_CAPABILITY_INDICATOR";


        /// M: CC071: Add Customer proprietary-IMS RIL interface. @{
        case RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY: return "RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY";
        /// @}

        // SMS part
        // SMS ready event
        case RIL_UNSOL_SMS_READY_NOTIFICATION: return "RIL_UNSOL_SMS_READY_NOTIFICATION";
        // Memory storage full
        case RIL_UNSOL_ME_SMS_STORAGE_FULL: return "RIL_UNSOL_ME_SMS_STORAGE_FULL";
        // ETWS primary notification
        case RIL_UNSOL_RESPONSE_ETWS_NOTIFICATION: return "RIL_UNSOL_RESPONSE_ETWS_NOTIFICATION";
        // SMS part

        /// M: IMS VoLTE conference dial feature. @{
        case RIL_REQUEST_CONFERENCE_DIAL: return "RIL_REQUEST_CONFERENCE_DIAL";
        /// @}
        case RIL_UNSOL_STK_CC_ALPHA_NOTIFY: return "UNSOL_STK_CC_ALPHA_NOTIFY";
        case RIL_UNSOL_LCEDATA_RECV: return "RIL_UNSOL_LCEDATA_RECV";
        /// M: [C2K][IR][MD-IRAT] URC for GMSS RAT changed. @{
        case RIL_UNSOL_GMSS_RAT_CHANGED: return "RIL_UNSOL_GMSS_RAT_CHANGED";
        /// M: [C2K][IR][MD-IRAT] URC for GMSS RAT changed. @{
        // IMS
        case RIL_UNSOL_IMS_ENABLE_START: return "RIL_UNSOL_IMS_ENABLE_START";
        case RIL_UNSOL_IMS_DISABLE_START: return "RIL_UNSOL_IMS_DISABLE_START";
        case RIL_UNSOL_IMSI_REFRESH_DONE: return "RIL_UNSOL_IMSI_REFRESH_DONE";
        /// M: For 3G VT only @{
        case RIL_REQUEST_VT_DIAL: return "VT_DIAL";
        case RIL_REQUEST_VOICE_ACCEPT: return "VOICE_ACCEPT";
        case RIL_REQUEST_REPLACE_VT_CALL: return "REPLACE_VT_CALL";
        case RIL_UNSOL_VT_STATUS_INFO: return "UNSOL_VT_STATUS_INFO";
        /// @}
        // M: BIP {
        case RIL_UNSOL_STK_BIP_PROACTIVE_COMMAND: return "UNSOL_STK_BIP_PROACTIVE_COMMAND";
        // M: BIP }
        //WorldMode
        case RIL_UNSOL_WORLD_MODE_CHANGED: return "RIL_UNSOL_WORLD_MODE_CHANGED";
        /// M: IMS ViLTE feature. @{
        case RIL_REQUEST_VIDEO_CALL_ACCEPT: return "RIL_REQUEST_VIDEO_CALL_ACCEPT";
        /// @}
        /// M: USSI RIL request and URC strings @{
        case RIL_REQUEST_SEND_USSI: return "RIL_REQUEST_SEND_USSI";
        case RIL_REQUEST_CANCEL_USSI: return "RIL_REQUEST_CANCEL_USSI";
        case RIL_UNSOL_ON_USSI: return "RIL_UNSOL_ON_USSI";
        case RIL_UNSOL_ATCI_RESPONSE: return "RIL_UNSOL_ATCI_RESPONSE";
        case RIL_UNSOL_MODULATION_INFO: return "RIL_UNSOL_MODULATION_INFO";
        /// @}

        /// M: EPDG feature. Update PS state from MAL @{
        case RIL_UNSOL_IMEI_LOCK: return "RIL_UNSOL_IMEI_LOCK";
        case RIL_UNSOL_PHB_READY_NOTIFICATION: return "RIL_UNSOL_PHB_READY_NOTIFICATION";
        case RIL_UNSOL_NETWORK_EVENT: return "UNSOL_NETWORK_EVENT";

        /// M:set Ims capability to MD. @{
        case RIL_REQUEST_SET_VOLTE_ENABLE: return "RIL_REQUEST_SET_VOLTE_ENABLE";
        case RIL_REQUEST_SET_WFC_ENABLE: return "RIL_REQUEST_SET_WFC_ENABLE";
        case RIL_REQUEST_SET_IMS_VIDEO_ENABLE: return "RIL_REQUEST_SET_IMS_VIDEO_ENABLE";
        /// @}

        /// M: IMS ViLTE feature. @{
        case RIL_REQUEST_VT_DIAL_WITH_SIP_URI: return "RIL_REQUEST_VT_DIAL_WITH_SIP_URI";
        /// @}

        /// M: IMS Deregistration @{
        case RIL_REQUEST_IMS_DEREG_NOTIFICATION: return "RIL_REQUEST_IMS_DEREG_NOTIFICATION";
        case RIL_UNSOL_IMS_DEREG_DONE: return "RIL_UNSOL_IMS_DEREG_DONE";
        /// @}

        case RIL_UNSOL_PCO_STATUS: return "RIL_UNSOL_PCO_STATUS";
        // M: IA- change attach APN
        case RIL_UNSOL_DATA_ATTACH_APN_CHANGED: return "RIL_UNSOL_DATA_ATTACH_APN_CHANGED";
        case RIL_UNSOL_TRIGGER_OTASP: return "RIL_UNSOL_TRIGGER_OTASP";
        // MT2635: add request string
        case RIL_LOCAL_REQUEST_GET_OPERATOR_NUMBER: return "RIL_LOCAL_REQUEST_GET_OPERATOR_NUMBER";
        case RIL_REQUEST_ECALL_RESET_IVS: return "RIL_REQUEST_ECALL_RESET_IVS";
        case RIL_REQUEST_ECALL_SET_IVS: return "RIL_REQUEST_ECALL_SET_IVS";
        case RIL_REQUEST_ECALL_SET_TEST_NUM: return "RIL_REQUEST_ECALL_SET_TEST_NUM";
        case RIL_REQUEST_ECALL_SET_RECONF_NUM: return "RIL_REQUEST_ECALL_SET_RECONF_NUM";
        case RIL_REQUEST_ECALL_SET_MSD: return "RIL_REQUEST_ECALL_SET_MSD";
        case RIL_REQUEST_ECALL_SET_PSAP: return "RIL_REQUEST_ECALL_SET_PSAP";
        case RIL_REQUEST_ECALL_MAKE_ECALL: return "RIL_REQUEST_ECALL_MAKE_ECALL";
        case RIL_REQUEST_ECALL_IVS_PUSH_MSD: return "RIL_REQUEST_ECALL_IVS_PUSH_MSD";
        case RIL_REQUEST_ECALL_PSAP_PULL_MSD: return "RIL_REQUEST_ECALL_PSAP_PULL_MSD";
        case RIL_REQUEST_ECALL_SET_MSD_MODE: return "RIL_REQUEST_ECALL_SET_MSD_MODE";
        case RIL_REQUEST_ECALL_FAST_MAKE_ECALL: return "RIL_REQUEST_ECALL_FAST_MAKE_ECALL";
        case RIL_REQUEST_ECALL_CTRL_SEQUENCE: return "RIL_REQUEST_ECALL_CTRL_SEQUENCE";
        case RIL_REQUEST_ECALL_SET_PRI: return "RIL_REQUEST_ECALL_SET_PRI";
        case RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME: return "RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME";
        case RIL_REQUEST_ECALL_SET_REGISTRATION_STATE: return "RIL_REQUEST_ECALL_SET_REGISTRATION_STATE";
        case RIL_REQUEST_ECALL_SET_OPRT_MODE: return "RIL_REQUEST_ECALL_SET_OPRT_MODE";
        case RIL_REQUEST_ECALL_GET_OPRT_MODE: return "RIL_REQUEST_ECALL_GET_OPRT_MODE";
        case RIL_UNSOL_ECALL_INDICATIONS: return "RIL_UNSOL_ECALL_INDICATIONS";
        case RIL_UNSOL_ECALL_MSDHACK: return "RIL_UNSOL_ECALL_MSDHACK";
        case RIL_UNSOL_MAL_RESTART: return "RIL_UNSOL_MAL_RESTART";
        case RIL_REQUEST_SIM_GET_ICCID: return "RIL_REQUEST_SIM_GET_ICCID";
        case RIL_UNSOL_SETUP_PDN_STATE_CHANGED: return "RIL_UNSOL_SETUP_PDN_STATE_CHANGED";
        case RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED: return "RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED";
        case RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED: return "RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED";
#ifdef UPDATE_APN
        case RIL_REQUEST_LOCAL_UPDATE_APN: return "RIL_REQUEST_LOCAL_UPDATE_APN";
        case RIL_REQUEST_LOCAL_GET_APN: return "RIL_REQUEST_LOCAL_GET_APN";
#endif
        case RIL_REQUEST_LOCAL_REG_STATE_TYPE: return "RIL_REQUEST_LOCAL_REG_STATE_TYPE";
        case RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE: return "RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE";
        case RIL_REQUEST_LOCAL_GET_OPERATOR_CODE: return "RIL_REQUEST_LOCAL_GET_OPERATOR_CODE";
        case RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER: return "RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER";
        case RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP: return "SET_ALLOWED_NETWORK_TYPES_BITMAP";
        case RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP: return "GET_ALLOWED_NETWORK_TYPES_BITMAP";
        case RIL_UNSOL_KEEPALIVE_STATUS: return "RIL_UNSOL_KEEPALIVE_STATUS";
        case RIL_REQUEST_STOP_KEEPALIVE: return "RIL_REQUEST_STOP_KEEPALIVE";
        case RIL_REQUEST_START_KEEPALIVE: return "RIL_REQUEST_START_KEEPALIVE";
        case RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED: return "RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED";
        case RIL_REQUEST_SET_USAGE_SETTING: return "RIL_REQUEST_SET_USAGE_SETTING";
        case RIL_REQUEST_GET_USAGE_SETTING: return "RIL_REQUEST_GET_USAGE_SETTING";
        case RIL_REQUEST_ENABLE_VONR: return "RIL_REQUEST_ENABLE_VONR";
        case RIL_REQUEST_IS_VONR_ENABLED: return "RIL_REQUEST_IS_VONR_ENABLED";
        case RIL_REQUEST_GET_NITZ: return "RIL_REQUEST_GET_NITZ";
        case RIL_REQUEST_GET_IMSCFG: return "RIL_REQUEST_GET_IMSCFG";
        default: return "<unknown request>";
    }
}


#ifdef RIL_SHLIB



#define RIL_UNUSED_PARM(a) noopRemoveWarning((void *)&(a));

#define MAX_AT_RESPONSE 0x1000

/* pathname returned from RIL_REQUEST_SETUP_DATA_CALL / RIL_REQUEST_SETUP_DEFAULT_PDP */
#define PPP_TTY_PATH "eth0"

// Default MTU value
#define DEFAULT_MTU 1500

#ifdef USE_TI_COMMANDS

// Enable a workaround
// 1) Make incoming call, do not answer
// 2) Hangup remote end
// Expected: call should disappear from CLCC line
// Actual: Call shows as "ACTIVE" before disappearing
#define WORKAROUND_ERRONEOUS_ANSWER 1

// Some varients of the TI stack do not support the +CGEV unsolicited
// response. However, they seem to send an unsolicited +CME ERROR: 150
#define WORKAROUND_FAKE_CGEV 1
#endif

/* Modem Technology bits */
#define MDM_GSM         0x01
#define MDM_WCDMA       0x02
#define MDM_CDMA        0x04
#define MDM_EVDO        0x08
#define MDM_LTE         0x10

typedef struct {
    int supportedTechs; // Bitmask of supported Modem Technology bits
    int currentTech;    // Technology the modem is currently using (in the format used by modem)
    int isMultimode;

    // Preferred mode bitmask. This is actually 4 byte-sized bitmasks with different priority values,
    // in which the byte number from LSB to MSB give the priority.
    //
    //          |MSB|   |   |LSB
    // value:   |00 |00 |00 |00
    // byte #:  |3  |2  |1  |0
    //
    // Higher byte order give higher priority. Thus, a value of 0x0000000f represents
    // a preferred mode of GSM, WCDMA, CDMA, and EvDo in which all are equally preferrable, whereas
    // 0x00000201 represents a mode with GSM and WCDMA, in which WCDMA is preferred over GSM
    int32_t preferredNetworkMode;
    int subscription_source;

} ModemInfo;

static ModemInfo *sMdmInfo;
// TECH returns the current technology in the format used by the modem.
// It can be used as an l-value
#define TECH(mdminfo)                 ((mdminfo)->currentTech)
// TECH_BIT returns the bitmask equivalent of the current tech
#define TECH_BIT(mdminfo)            (1 << ((mdminfo)->currentTech))
#define IS_MULTIMODE(mdminfo)         ((mdminfo)->isMultimode)
#define TECH_SUPPORTED(mdminfo, tech) ((mdminfo)->supportedTechs & (tech))
#define PREFERRED_NETWORK(mdminfo)    ((mdminfo)->preferredNetworkMode)
// CDMA Subscription Source
#define SSOURCE(mdminfo)              ((mdminfo)->subscription_source)

static int net2modem[] = {
    MDM_GSM | MDM_WCDMA,                                 // 0  - GSM / WCDMA Pref
    MDM_GSM,                                             // 1  - GSM only
    MDM_WCDMA,                                           // 2  - WCDMA only
    MDM_GSM | MDM_WCDMA,                                 // 3  - GSM / WCDMA Auto
    MDM_CDMA | MDM_EVDO,                                 // 4  - CDMA / EvDo Auto
    MDM_CDMA,                                            // 5  - CDMA only
    MDM_EVDO,                                            // 6  - EvDo only
    MDM_GSM | MDM_WCDMA | MDM_CDMA | MDM_EVDO,           // 7  - GSM/WCDMA, CDMA, EvDo
    MDM_LTE | MDM_CDMA | MDM_EVDO,                       // 8  - LTE, CDMA and EvDo
    MDM_LTE | MDM_GSM | MDM_WCDMA,                       // 9  - LTE, GSM/WCDMA
    MDM_LTE | MDM_CDMA | MDM_EVDO | MDM_GSM | MDM_WCDMA, // 10 - LTE, CDMA, EvDo, GSM/WCDMA
    MDM_LTE,                                             // 11 - LTE only
};

static int32_t net2pmask[] = {
    MDM_GSM | (MDM_WCDMA << 8),                          // 0  - GSM / WCDMA Pref
    MDM_GSM,                                             // 1  - GSM only
    MDM_WCDMA,                                           // 2  - WCDMA only
    MDM_GSM | MDM_WCDMA,                                 // 3  - GSM / WCDMA Auto
    MDM_CDMA | MDM_EVDO,                                 // 4  - CDMA / EvDo Auto
    MDM_CDMA,                                            // 5  - CDMA only
    MDM_EVDO,                                            // 6  - EvDo only
    MDM_GSM | MDM_WCDMA | MDM_CDMA | MDM_EVDO,           // 7  - GSM/WCDMA, CDMA, EvDo
    MDM_LTE | MDM_CDMA | MDM_EVDO,                       // 8  - LTE, CDMA and EvDo
    MDM_LTE | MDM_GSM | MDM_WCDMA,                       // 9  - LTE, GSM/WCDMA
    MDM_LTE | MDM_CDMA | MDM_EVDO | MDM_GSM | MDM_WCDMA, // 10 - LTE, CDMA, EvDo, GSM/WCDMA
    MDM_LTE,                                             // 11 - LTE only
};



typedef enum {
    SIM_ABSENT = 0,
    SIM_NOT_READY = 1,
    SIM_READY = 2, /* SIM_READY means the radio state is RADIO_STATE_SIM_READY */
    SIM_PIN = 3,
    SIM_PUK = 4,
    SIM_NETWORK_PERSONALIZATION = 5,
    RUIM_ABSENT = 6,
    RUIM_NOT_READY = 7,
    RUIM_READY = 8,
    RUIM_PIN = 9,
    RUIM_PUK = 10,
    RUIM_NETWORK_PERSONALIZATION = 11
} SIM_Status;

#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
static void onRequest (int request, void *data, size_t datalen, RIL_Token t, RIL_SOCKET_ID socket_id);
#else
static void onRequest (int request, void *data, size_t datalen, RIL_Token t);
#endif



static RIL_RadioState currentState(RIL_SOCKET_ID socket_id);
static int onSupports (int requestCode);
static void onCancel (RIL_Token t);
static const char *getVersion();

const char * requestToString(int request);

pthread_t s_tid_mainloop;
static const struct RIL_Env *s_rilenv = NULL;

#define RIL_onRequestComplete(t, e, response, responselen) s_rilenv->OnRequestComplete(t,e, response, responselen)

#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
#define RIL_onUnsolicitedResponse(a,b,c,d) s_rilenv->OnUnsolicitedResponse(a,b,c,d)
#else
#define RIL_onUnsolicitedResponse(a,b,c) s_rilenv->OnUnsolicitedResponse(a,b,c)
#endif

#define RIL_requestTimedCallback(a,b,c) s_rilenv->RequestTimedCallback(a,b,c)


#ifdef __SAP_API__
typedef struct SapSocketRequest {
    int token;
    MsgHeader* curr;
    struct SapSocketRequest* p_next;
    RIL_SOCKET_ID socketId;
} SapSocketRequest;
static SapSocketRequest *sapPendingRequests = NULL;
static pthread_mutex_t sapPendingRequestsMutex = PTHREAD_MUTEX_INITIALIZER;

/// M: SAP
#define RIL_SAP_onRequestComplete(t, e, response, responselen) s_rilsapenv->OnRequestComplete(t, e , response, responselen)
#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
#define RIL_SAP_onUnsolicitedResponse(unsolResponse, data, datalen, socket_id) s_rilsapenv->OnUnsolicitedResponse(unsolResponse, data, datalen, socket_id)
#else
#define RIL_SAP_onUnsolicitedResponse(unsolResponse, data, datalen) s_rilsapenv->OnUnsolicitedResponse(unsolResponse, data, datalen)
#endif
#define RIL_SAP_requestTimedCallback(a,b,c) s_rilsapenv->RequestTimedCallback(a,b,c)
#define RIL_SAP_requestProxyTimedCallback(a,b,c,d,e) \
        RLOGD("%s request timed callback %s to %s", __FUNCTION__, e, proxyIdToString((int)d)); \
        s_rilsapenv->RequestProxyTimedCallback(a,b,c,(int)d)
#define RIL_SAP_queryMyChannelId(a) s_rilsapenv->QueryMyChannelId(a)
#define RIL_SAP_queryMyProxyIdByThread  s_rilsapenv->QueryMyProxyIdByThread()
#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
static void onSapRequest(int request, void *data, size_t datalen, RIL_Token t,
        RIL_SOCKET_ID socket_id);
#else
static void onSapRequest(int request, void *data, size_t datalen, RIL_Token t);
#endif

//void decodeAndSendSapMessage(void *data, size_t datalen, RIL_SOCKET_ID socketId);

//void sendBtSapResponseComplete(RIL_Token t, RIL_Errno ret, MsgId msgId, void *data);
///
/// M: SAP
void encodeAndSendSapMessage(void *data, RIL_SOCKET_ID socketId) {
    SapSocketRequest *recv = (SapSocketRequest*) data;
    MsgHeader  *reqHeader = recv->curr;
    pb_ostream_t ostream;
    size_t encoded_size = 0;
    uint32_t written_size;
    size_t buffer_size = 0;
    bool success = false;
    SapSocketRequest **pendingRequests = &sapPendingRequests;

    if ((success = pb_get_encoded_size(&encoded_size, MsgHeader_fields,
        reqHeader)) && encoded_size <= INT32_MAX) {
        buffer_size = encoded_size + sizeof(uint32_t);
        uint8_t buffer[buffer_size];
        written_size = htonl((uint32_t) encoded_size);
        ostream = pb_ostream_from_buffer(buffer, buffer_size);
        pb_write(&ostream, (uint8_t *)&written_size, sizeof(written_size));
        success = pb_encode(&ostream, MsgHeader_fields, reqHeader);

        if (success) {
            RLOGD("encodeAndSendSapMessage: Size: %d (0x%x) Size as written: 0x%x", encoded_size,
                    encoded_size, written_size);
            RLOGI("encodeAndSendSapMessage: [%d] > SAP RESPONSE type: %d. id: %d. error: %d",
                    reqHeader->token, reqHeader->type, reqHeader->id, reqHeader->error);

            // enqueue request
            pthread_mutex_lock(&sapPendingRequestsMutex);
            recv->p_next = *pendingRequests;
            *pendingRequests = recv;
            pthread_mutex_unlock(&sapPendingRequestsMutex);
            //RLOGD("encodeAndSendSapMessage add token = %d to queue", reqHeader->token);
            rfx_sap_to_rild_socket(buffer, buffer_size, socketId);
        } else {
            RLOGE("encodeAndSendSapMessage: Error while encoding response of type %d id %d \
                    buffer_size: %d: %s.", reqHeader->type, reqHeader->id, buffer_size,
                    PB_GET_ERROR(&ostream));
        }
    }else {
        RLOGE("Not sending response type %d: encoded_size: %u. encoded size result: %d",
        reqHeader->type, encoded_size, success);
    }

}

SapSocketRequest* checkAndDequeueRequestInfo(int token) {
    SapSocketRequest **ppCur = NULL;
    SapSocketRequest *tmp = NULL;

    pthread_mutex_lock(&sapPendingRequestsMutex);
    for(ppCur = &sapPendingRequests; *ppCur != NULL; ppCur = &((*ppCur)->p_next)) {
        // RLOGD("checkAndDequeueRequestInfo searching...token = %d",
        //        (*ppCur)->curr->token);
        if (token == (*ppCur)->curr->token) {
            tmp = *ppCur;
            *ppCur = (*ppCur)->p_next;
            //RLOGD("checkAndDequeueRequestInfo get!");
            break;
        }
    }
    pthread_mutex_unlock(&sapPendingRequestsMutex);

    return tmp;
}

void decodeAndSendSapMessage(void *data, size_t datalen,
        RIL_SOCKET_ID socketId) {
    int ret;
    MsgHeader  *header;
    pb_istream_t stream;

    stream = pb_istream_from_buffer((uint8_t *)data, datalen);
    header = (MsgHeader *)malloc(sizeof (MsgHeader));
    memset(header, 0, sizeof(MsgHeader));

    if (!pb_decode(&stream, MsgHeader_fields, header) ) {
        RLOGE("Error decoding protobuf buffer : %s", PB_GET_ERROR(&stream));
    }

    RLOGD("decodeAndSendSapMessage id = %d", header->id);
    if (MsgId_RIL_SIM_SAP_STATUS == header->id) {
        // URC
    #if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
        RIL_SAP_onUnsolicitedResponse(header->id, header->payload->bytes, header->payload->size,
            socketId);
    #else
        RIL_SAP_onUnsolicitedResponse(header->id, header->payload->bytes, header->payload->size);
    #endif
    } else {
        //RLOGD("decodeAndSendSapMessage search token = %d", header->token);
        SapSocketRequest *req =  checkAndDequeueRequestInfo(header->token);
        RIL_SAP_onRequestComplete(req, (RIL_Errno) header->error, header->payload->bytes,
                header->payload->size);
        //free(req);
    }
    //free(header);
}

void sendBtSapResponseComplete(RIL_Token t, RIL_Errno ret, MsgId msgId, void *data) {
    const pb_field_t *fields = NULL;
    size_t encoded_size = 0;
    uint32_t written_size;
    size_t buffer_size = 0;
    pb_ostream_t ostream;
    bool success = false;
    ssize_t written_bytes;
    int i = 0;

    RLOGD("sendBtSapResponseComplete, start (%d)", msgId);

    switch (msgId) {
        case MsgId_RIL_SIM_SAP_CONNECT:
            fields = RIL_SIM_SAP_CONNECT_RSP_fields;
            break;
        case MsgId_RIL_SIM_SAP_DISCONNECT:
            fields = RIL_SIM_SAP_DISCONNECT_RSP_fields;
            break;
        case MsgId_RIL_SIM_SAP_APDU:
            fields = RIL_SIM_SAP_APDU_RSP_fields;
            break;
        case MsgId_RIL_SIM_SAP_TRANSFER_ATR:
            fields = RIL_SIM_SAP_TRANSFER_ATR_RSP_fields;
            break;
        case MsgId_RIL_SIM_SAP_POWER:
            fields = RIL_SIM_SAP_POWER_RSP_fields;
            break;
        case MsgId_RIL_SIM_SAP_RESET_SIM:
            fields = RIL_SIM_SAP_RESET_SIM_RSP_fields;
            break;
        case MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL:
            fields = RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP_fields;
            break;
        case MsgId_RIL_SIM_SAP_ERROR_RESP:
            fields = RIL_SIM_SAP_ERROR_RSP_fields;
            break;
        default:
            RLOGE("sendBtSapResponseComplete, MsgId is mistake!");
            return;
    }

    if ((success = pb_get_encoded_size(&encoded_size, fields, data)) &&
            encoded_size <= INT32_MAX) {
        //buffer_size = encoded_size + sizeof(uint32_t);
        buffer_size = encoded_size;
        uint8_t buffer[buffer_size];
        //written_size = htonl((uint32_t) encoded_size);
        ostream = pb_ostream_from_buffer(buffer, buffer_size);
        //pb_write(&ostream, (uint8_t *)&written_size, sizeof(written_size));
        success = pb_encode(&ostream, fields, data);

        if(success) {
            RLOGD("sendBtSapResponseComplete, Size: %d (0x%x) Size as written: 0x%x",
                encoded_size, encoded_size, written_size);
            // Send response
            RIL_SAP_onRequestComplete(t, ret, buffer, buffer_size);
        } else {
            RLOGE("sendBtSapResponseComplete, Encode failed!");
        }
    } else {
        RLOGE("Not sending response type %d: encoded_size: %u. encoded size result: %d",
        msgId, encoded_size, success);
    }
}

///

#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
static void onSapRequest(int request, void *data, size_t datalen, RIL_Token t, RIL_SOCKET_ID socket_id) {
#else
static void onSapRequest(int request, void *data, size_t datalen, RIL_Token t) {
#endif
    RLOGD("onSapRequest: %d", request);
    int socketId = RIL_SOCKET_1;
    #if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
    socketId = socket_id;
    #endif

    if (request < MsgId_RIL_SIM_SAP_CONNECT /* MsgId_UNKNOWN_REQ */ ||
            request > MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL) {
        RLOGD("invalid request");
        RIL_SIM_SAP_ERROR_RSP rsp;
        rsp.dummy_field = 1;
        sendBtSapResponseComplete(t, Error_RIL_E_REQUEST_NOT_SUPPORTED, MsgId_RIL_SIM_SAP_ERROR_RESP
                , &rsp);
        return;
    }

    if (RADIO_STATE_UNAVAILABLE == getRadioState(RIL_SOCKET_1)) {
        RLOGD("RADIO_STATE_UNAVAILABLE and reply failure to Sap message");
        RIL_SIM_SAP_CONNECT_RSP rsp;
        rsp.response = RIL_SIM_SAP_CONNECT_RSP_Response_RIL_E_SAP_CONNECT_FAILURE;
        rsp.has_max_message_size = false;
        rsp.max_message_size = 0;
        sendBtSapResponseComplete(t, Error_RIL_E_RADIO_NOT_AVAILABLE, MsgId_RIL_SIM_SAP_CONNECT,
                &rsp);
        return;
    }

    encodeAndSendSapMessage(t, socketId);

}
#endif


/*** Static Variables ***/
static const RIL_RadioFunctions s_callbacks = {
    RIL_VERSION,
    onRequest,
    currentState,
    onSupports,
    onCancel,
    getVersion
};
const struct RIL_Env *s_rilsapenv;
static const RIL_RadioFunctions s_sapcallbacks = {
    RIL_VERSION,
#ifdef __SAP_API__
    onSapRequest,
#endif
    currentState,
    onSupports,
    onCancel,
    getVersion
};








static int sFD;     /* file desc of AT channel */
static char sATBuffer[MAX_AT_RESPONSE+1];
static char *sATBufferCur = NULL;

static const struct timeval TIMEVAL_SIMPOLL = {1,0};
static const struct timeval TIMEVAL_CALLSTATEPOLL = {0,500000};
static const struct timeval TIMEVAL_0 = {0,0};

//void setRadioState(RIL_RadioState newState, RIL_SOCKET_ID rid);
//RIL_RadioState getRadioState(RIL_SOCKET_ID rid);

static int is3gpp2(int radioTech) {
    switch (radioTech) {
        case RADIO_TECH_IS95A:
        case RADIO_TECH_IS95B:
        case RADIO_TECH_1xRTT:
        case RADIO_TECH_EVDO_0:
        case RADIO_TECH_EVDO_A:
        case RADIO_TECH_EVDO_B:
        case RADIO_TECH_EHRPD:
            return 1;
        default:
            return 0;
    }
}
static void *noopRemoveWarning( void *a ) {
    return a;
}

/*** Callback methods from the RIL library to us ***/

/**
 * Call from RIL to us to make a RIL_REQUEST
 *
 * Must be completed with a call to RIL_onRequestComplete()
 *
 * RIL_onRequestComplete() may be called from any thread, before or after
 * this function returns.
 *
 * Will always be called from the same thread, so returning here implies
 * that the radio is ready to process another command (whether or not
 * the previous command has completed).
 */
#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
static void onRequest (int request, void *data, size_t datalen, RIL_Token t, RIL_SOCKET_ID socket_id){
#else
static void onRequest (int request, void *data, size_t datalen, RIL_Token t){
#endif

    ATResponse *p_response;
    int err;
    RIL_RadioState radioState = currentState(RIL_SOCKET_1);


   if(t==NULL){
       RLOGE("[OnRequest]Error!RIL_Token is NULL");
       return;
   }

#if (SIM_COUNT >= 2) && !defined(MODE_DSSS)
    if (socket_id == RIL_SOCKET_2) {
        radioState = sState2;
    }
#if (SIM_COUNT >= 3)
    else if (socket_id == RIL_SOCKET_3) {
        radioState = sState3;
    }
#if (SIM_COUNT >= 4)
    else if (socket_id == RIL_SOCKET_4) {
        radioState = sState4;
    }
#endif
#endif
#endif

#if defined(__TELEPHONY_WARE__)
        /*
         * Ignore requests when not support
         */
        if(1 != rfx_check_request_support(request)){
            RLOGE("[OnRequest]Error!not support this request, id=%d", request);
            RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        }
#endif


    /* Ignore all requests when RADIO_STATE_UNAVAILABLE.
     */
    if (radioState == RADIO_STATE_UNAVAILABLE &&
        request != RIL_REQUEST_GET_SIM_STATUS &&
        request != RIL_REQUEST_OEM_HOOK_RAW && //This is for ATCI
        request != RIL_REQUEST_OEM_HOOK_STRINGS &&
        request != RIL_REQUEST_GET_RADIO_CAPABILITY &&
        request != RIL_REQUEST_SET_RADIO_CAPABILITY &&
        request != RIL_REQUEST_RESET_RADIO &&
        request != RIL_REQUEST_MODEM_POWERON &&
        request != RIL_REQUEST_MODEM_POWEROFF &&
        request != RIL_REQUEST_RESUME_REGISTRATION &&
        request != RIL_REQUEST_SET_FD_MODE &&
        request != RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING &&
        request != RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE &&
        request != RIL_REQUEST_SET_IMS_ENABLE &&
        request != RIL_REQUEST_SET_IMSCFG &&
        request != RIL_REQUEST_SET_VOLTE_ENABLE &&
        request != RIL_REQUEST_SET_WFC_ENABLE &&
        request != RIL_REQUEST_SET_IMS_VIDEO_ENABLE &&
        request != RIL_REQUEST_ALLOW_DATA &&
        request != RIL_REQUEST_SET_TRM &&
        // M: VzW
        request != RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD &&
            request != RIL_REQUEST_SYNC_APN_TABLE) {
#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
            RLOGD("[RilProxy] onRequest: %s, datalen = %d, radioState:%d, socket_id: %d",
                    requestToString(request), datalen, radioState, socket_id);
#else
            RLOGD("[RilProxy] onRequest: %s, datalen = %d, radioState:%d",
                    requestToString(request), datalen, radioState);
#endif
            RIL_onRequestComplete(t, RIL_E_RADIO_NOT_AVAILABLE, NULL, 0);
            return;
        }
#if defined(MODE_DSSS)   //DSSS mode
    if(2 == rfx_get_disabled_sim()){  //enable slot1, disable slot2
        RLOGD("[RilProxy] onRequest: request = %d, datalen = %d, radioState:%d, slotId = %d",
            request, datalen, radioState, RIL_SOCKET_1);
        rfx_enqueue_request_message(request, data, datalen, t, RIL_SOCKET_1);
    } else {
        RLOGD("[RilProxy] onRequest: request = %d, datalen = %d, radioState:%d, slotId = %d",
            request, datalen, radioState, RIL_SOCKET_2);
        rfx_enqueue_request_message(request, data, datalen, t, RIL_SOCKET_2);
    }
#elif defined(ANDROID_MULTI_SIM)   //DSDS mode
    /// M: ril proxy
    RLOGD("[RilProxy] onRequest: request = %d, datalen = %d, radioState:%d, slotId = %d",
            request, datalen, radioState, socket_id);
    rfx_enqueue_request_message(request, data, datalen, t, socket_id);
#else      //MT 2635
    RLOGD("[RilProxy] onRequest: request = %d, datalen = %d, radioState:%d",
            request, datalen, radioState);
    rfx_enqueue_request_message(request, data, datalen, t, RIL_SOCKET_1);
#endif
    /// M
}


RIL_RadioState getRadioState(RIL_SOCKET_ID rid)
{
    RIL_RadioState radioState = sState;

#if (SIM_COUNT >= 2)
    if (RIL_SOCKET_2 == rid)
        radioState = sState2;
#if (SIM_COUNT >= 3)
    if (RIL_SOCKET_3 == rid)
        radioState = sState3;
#endif
#if (SIM_COUNT >= 4)
    if (RIL_SOCKET_4 == rid)
        radioState = sState4;
#endif
#endif

    RLOGI("getRadioState(): radioState=%d\n", radioState);

    return radioState;
}

/**
 * Synchronous call from the RIL to us to return current radio state.
 * RADIO_STATE_UNAVAILABLE should be the initial state.
 */
static RIL_RadioState
currentState(RIL_SOCKET_ID socket_id)
{
#if defined(MODE_DSSS)
    if(2 == rfx_get_disabled_sim()){  //enable slot1, disable slot2
        getRadioState(RIL_SOCKET_1);
    } else {   //enable slot2, disable slot1
        getRadioState(RIL_SOCKET_2);
    }
#else
    return getRadioState(socket_id);
#endif
}
/**
 * Call from RIL to us to find out whether a specific request code
 * is supported by this implementation.
 *
 * Return 1 for "supported" and 0 for "unsupported"
 */

static int
onSupports (int requestCode)
{
    return 1;
}

static void onCancel (RIL_Token t)
{

}

static const char * getVersion(void)
{
    return "android reference-ril 1.0";
}

int isUserLoad() {
    int isUserLoad = 0;
    char property_value_emulation[PROPERTY_VALUE_MAX] = { 0 };
    char property_value[PROPERTY_VALUE_MAX] = { 0 };
    #ifdef TELEPHONYWARE
    property_get("vendor.ril.emulation.userload", property_value_emulation, "0");
    #else
    property_get("ril.emulation.userload", property_value_emulation, "0");
    #endif
    if(strcmp("1", property_value_emulation) == 0) {
        return 1;
    }
    property_get("ro.build.type", property_value, "");
    isUserLoad = (strcmp("user", property_value) == 0);
    //RLOGD("isUserLoad: %d", isUserLoad);
    return isUserLoad;
}

int isInternalLoad() {
#ifdef __PRODUCTION_RELEASE__
    return 0;
#else
    char property_value[PROPERTY_VALUE_MAX] = { 0 };
    #ifdef TELEPHONYWARE
    property_get("vendor.ril.emulation.production", property_value, "0");
    #else
    property_get("ril.emulation.production", property_value, "0");
    #endif
    return (strcmp("1", property_value) != 0);
#endif /* __PRODUCTION_RELEASE__ */
}

/*
* Purpose:  Function responsible by all signal handlers treatment any new signal must be added here
* Input:      param - signal ID
* Return:    -
*/
void signalTreatment(int param)
{
    RLOGD("signal_no=%d", param);
    switch (param)
    {
    case SIGSEGV:
    case SIGABRT:
        RLOGD("SIGSEGV/SIGABRT");
        property_set("ril.mux.report.case","2");
        property_set("ril.muxreport", "1");
        break;
    default:
        exit(0);
        break;
    }
}

void registerSignal()
{
    //signals treatment
    if ((isInternalLoad() == 0) && (isUserLoad() == 1)) {
        RLOGD("Setup SIGSEGV signal handling");
        signal(SIGSEGV, signalTreatment);
    }
}


/// MT2635:due to libril may be single sim, so we only use single sim api @{
void RFX_onRequestComplete(RIL_Token t, RIL_Errno e, void *response,
        size_t responselen) {
    s_rilenv->OnRequestComplete(t, e, response, responselen);
}

#if defined(ANDROID_MULTI_SIM)
void RFX_onUnsolicitedResponse(int unsolResponse, const void *data,
        size_t datalen, RIL_SOCKET_ID socket_id) {
    #if defined(TELEPHONYWARE)
    /*
     * Ignore requests when not support
     */
    if(1 != rfx_check_unsol_support(unsolResponse)){
        RLOGE("[RFX_onUnsolicitedResponse]Error!not support this unsol, id=%d", unsolResponse);
        return;
    }
    #endif
    #if defined(MODE_DSSS)
    s_rilenv->OnUnsolicitedResponse(unsolResponse, data, datalen);
    #else
    s_rilenv->OnUnsolicitedResponse(unsolResponse, data, datalen, socket_id);
    #endif
}
#else
void RFX_onUnsolicitedResponse(int unsolResponse, const void *data,
        size_t datalen) {
#if defined(TELEPHONYWARE)
    /*
     * Ignore requests when not support
     */
    if(1 != rfx_check_unsol_support(unsolResponse)){
        RLOGE("[RFX_onUnsolicitedResponse]Error!not support this unsol, id=%d", unsolResponse);
        return;
    }
#endif
    s_rilenv->OnUnsolicitedResponse(unsolResponse, data, datalen);
}
#endif

const RIL_RadioFunctions *RIL_Init(const struct RIL_Env *env, int argc, char **argv)
{
    int ret;
    int fd = -1;
    int opt;
    pthread_attr_t attr;

    s_rilenv = env;

    while ( -1 != (opt = getopt(argc, argv, "p:d:s:c:"))) {
        RLOGI("default %c\n", opt);
        switch (opt) {
            case 'p':
                s_port = atoi(optarg);
                if (s_port == 0) {
                    usage(argv[0]);
                    return NULL;
                }
                RLOGI("Opening loopback port %d\n", s_port);
            break;

            case 'd':
                s_device_path = optarg;
                RLOGI("Opening tty device %s\n", s_device_path);
            break;

            case 's':
                s_device_path   = optarg;
                s_device_socket = 1;
                RLOGI("Opening socket %s\n", s_device_path);
            break;

            case 'c':
                RLOGI("Client id received %s\n", optarg);
            break;

            default:
                RLOGI("default %c\n", opt);
                usage(argv[0]);
                return NULL;
        }
    }

    /// No need to check here
    /*if (s_port < 0 && s_device_path == NULL) {
        usage(argv[0]);
        return NULL;
    }*/


    sMdmInfo = calloc(1, sizeof(ModemInfo));
    if (!sMdmInfo) {
        RLOGE("Unable to alloc memory for ModemInfo");
        return NULL;
    }
    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&s_tid_mainloop, &attr, mainLoop, NULL);

    return &s_callbacks;
}

const RIL_RadioFunctions *RIL_SAP_Init(const struct RIL_Env *env, int argc,
        char **argv) {
    RLOGD("RIL_SAP_Init");
    s_rilsapenv = env;
    return &s_sapcallbacks;
}

#else /* RIL_SHLIB */
#ifndef TELE_FWK

int main (int argc, char **argv)
{
    int ret;
    int fd = -1;
    int opt;dsaf
    while ( -1 != (opt = getopt(argc, argv, "p:d:"))) {
        switch (opt) {
            case 'p':
                s_port = atoi(optarg);
                if (s_port == 0) {
                    usage(argv[0]);
                }
                RLOGI("Opening loopback port %d\n", s_port);
            break;

            case 'd':
                s_device_path = optarg;
                RLOGI("Opening tty device %s\n", s_device_path);
            break;

            case 's':
                s_device_path   = optarg;
                s_device_socket = 1;
                RLOGI("Opening socket %s\n", s_device_path);
            break;

            default:
                usage(argv[0]);
        }
    }

    if (s_port < 0 && s_device_path == NULL) {
        usage(argv[0]);
    }

    //RIL_register(&s_callbacks);

    mainLoop(NULL);

    return 0;
}

#endif

#endif

void
setRadioState(RIL_RadioState newState, RIL_SOCKET_ID rid)
{
    RLOGD("setRadioState(%d), rid:%d", newState, rid);
    RIL_RadioState oldState;
    RIL_RadioState *pState = NULL;

    pthread_mutex_lock(&s_state_mutex);

    oldState = sState;
    pState = &sState;

#if (SIM_COUNT >= 2)
    if (RIL_SOCKET_2 == rid) {
        oldState = sState2;
        pState = &sState2;
    }
#endif
#if (SIM_COUNT >= 3) /* Gemini plus 3 SIM*/
    else if (RIL_SOCKET_3 == rid) {
        oldState = sState3;
        pState = &sState3;
    }
#endif
#if (SIM_COUNT >= 4) /* Gemini plus 4SIM*/
    else if (RIL_SOCKET_4 == rid) {
        oldState = sState4;
        pState = &sState4;
    }
#endif



    if (s_closed > 0) {
        // If we're closed, the only reasonable state is
        // RADIO_STATE_UNAVAILABLE
        // This is here because things on the main thread
        // may attempt to change the radio state after the closed
        // event happened in another thread
        newState = RADIO_STATE_UNAVAILABLE;
    }

    if (*pState != newState || s_closed > 0) {
        *pState = newState;

        pthread_cond_broadcast (&s_state_cond);
    }

    pthread_mutex_unlock(&s_state_mutex);


    /* do these outside of the mutex */
    if (*pState != oldState) {
#if RIL_SHLIB
#if defined(ANDROID_MULTI_SIM) && !defined(MODE_DSSS)
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                                    NULL, 0, rid);
        // Sim state can change as result of radio state change
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED,
                                    NULL, 0, rid);
#else
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                                            NULL, 0);
                // Sim state can change as result of radio state change
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED,
                                            NULL, 0);
#endif
#endif
    }
}




/// @}
