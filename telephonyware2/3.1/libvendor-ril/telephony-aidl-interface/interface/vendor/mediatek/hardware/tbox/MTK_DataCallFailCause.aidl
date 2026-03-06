/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
package vendor.mediatek.hardware.tbox;

@VintfStability
@Backing(type="int")
enum MTK_DataCallFailCause {
    PDP_FAIL_NONE = 0,
    /*
     * No error, connection ok
     *
     *
     * an integer cause code defined in TS 24.008
     * section 6.1.3.1.3 or TS 24.301 Release 8+ Annex B.
     * If the implementation does not have access to the exact cause codes,
     * then it should return one of the following values,
     * as the UI layer needs to distinguish these
     * cases for error notification and potential retries.
     */
    PDP_FAIL_OPERATOR_BARRED = 0x08,
    /*
     * no retry
     */
    PDP_FAIL_NAS_SIGNALLING = 0x0E,
    PDP_FAIL_MBMS_CAPABILITIES_INSUFFICIENT = 0x18,
    /*
     * M
     */
    PDP_FAIL_LLC_SNDCP = 0x19,
    PDP_FAIL_INSUFFICIENT_RESOURCES = 0x1A,
    PDP_FAIL_MISSING_UKNOWN_APN = 0x1B,
    /*
     * no retry
     */
    PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE = 0x1C,
    /*
     * no retry
     */
    PDP_FAIL_USER_AUTHENTICATION = 0x1D,
    /*
     * no retry
     */
    PDP_FAIL_ACTIVATION_REJECT_GGSN = 0x1E,
    /*
     * no retry
     */
    PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED = 0x1F,
    PDP_FAIL_SERVICE_OPTION_NOT_SUPPORTED = 0x20,
    /*
     * no retry
     */
    PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED = 0x21,
    /*
     * no retry
     */
    PDP_FAIL_SERVICE_OPTION_OUT_OF_ORDER = 0x22,
    PDP_FAIL_NSAPI_IN_USE = 0x23,
    /*
     * no retry
     */
    PDP_FAIL_REGULAR_DEACTIVATION = 0x24,
    /*
     * possibly restart radio,
     * based on framework config
     */
    PDP_FAIL_QOS_NOT_ACCEPTED = 0x25,
    PDP_FAIL_NETWORK_FAILURE = 0x26,
    PDP_FAIL_UMTS_REACTIVATION_REQ = 0x27,
    PDP_FAIL_FEATURE_NOT_SUPP = 0x28,
    PDP_FAIL_TFT_SEMANTIC_ERROR = 0x29,
    PDP_FAIL_TFT_SYTAX_ERROR = 0x2A,
    PDP_FAIL_UNKNOWN_PDP_CONTEXT = 0x2B,
    PDP_FAIL_FILTER_SEMANTIC_ERROR = 0x2C,
    PDP_FAIL_FILTER_SYTAX_ERROR = 0x2D,
    PDP_FAIL_PDP_WITHOUT_ACTIVE_TFT = 0x2E,
    PDP_FAIL_MULTICAST_GROUP_MEMBERSHIP_TIMEOUT = 0x2F,
    /*
     * M
     */
    PDP_FAIL_BCM_VIOLATION = 0x30,
    /*
     * M
     */
    PDP_FAIL_LAST_PDN_DISC_NOT_ALLOWED = 0x31,
    /*
     * M
     */
    PDP_FAIL_ONLY_IPV4_ALLOWED = 0x32,
    /*
     * no retry
     */
    PDP_FAIL_ONLY_IPV6_ALLOWED = 0x33,
    /*
     * no retry
     */
    PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED = 0x34,
    PDP_FAIL_ESM_INFO_NOT_RECEIVED = 0x35,
    PDP_FAIL_PDN_CONN_DOES_NOT_EXIST = 0x36,
    PDP_FAIL_MULTI_CONN_TO_SAME_PDN_NOT_ALLOWED = 0x37,
    PDP_FAIL_COLLISION_WITH_NW_INITIATED_REQUEST = 0x38,
    /*
     * M
     */
    PDP_FAIL_ESM_UNSUPPORTED_QCI_VALUE = 0x3B,
    /*
     * M
     */
    PDP_FAIL_BEARER_HANDLING_NOT_SUPPORT = 0x3C,
    /*
     * M
     */
    PDP_FAIL_MAX_ACTIVE_PDP_CONTEXT_REACHED = 0x41,
    PDP_FAIL_UNSUPPORTED_APN_IN_CURRENT_PLMN = 0x42,
    PDP_FAIL_INVALID_TRANSACTION_ID = 0x51,
    PDP_FAIL_MESSAGE_INCORRECT_SEMANTIC = 0x5F,
    PDP_FAIL_INVALID_MANDATORY_INFO = 0x60,
    PDP_FAIL_MESSAGE_TYPE_UNSUPPORTED = 0x61,
    PDP_FAIL_MSG_TYPE_NONCOMPATIBLE_STATE = 0x62,
    PDP_FAIL_UNKNOWN_INFO_ELEMENT = 0x63,
    PDP_FAIL_CONDITIONAL_IE_ERROR = 0x64,
    PDP_FAIL_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE = 0x65,
    PDP_FAIL_PROTOCOL_ERRORS = 0x6F,
    /*
     * no retry
     */
    PDP_FAIL_APN_TYPE_CONFLICT = 0x70,
    PDP_FAIL_INVALID_PCSCF_ADDR = 0x71,
    PDP_FAIL_INTERNAL_CALL_PREEMPT_BY_HIGH_PRIO_APN = 0x72,
    PDP_FAIL_EMM_ACCESS_BARRED = 0x73,
    PDP_FAIL_EMERGENCY_IFACE_ONLY = 0x74,
    PDP_FAIL_IFACE_MISMATCH = 0x75,
    PDP_FAIL_COMPANION_IFACE_IN_USE = 0x76,
    PDP_FAIL_IP_ADDRESS_MISMATCH = 0x77,
    PDP_FAIL_IFACE_AND_POL_FAMILY_MISMATCH = 0x78,
    PDP_FAIL_EMM_ACCESS_BARRED_INFINITE_RETRY = 0x79,
    PDP_FAIL_AUTH_FAILURE_ON_EMERGENCY_CALL = 0x7A,
    PDP_FAIL_LOCAL_REJECT_ACT_REQ_DUE_TO_REACH_RETRY_COUNTER = 0x0E0F,
    /*
     * M
     */
    PDP_FAIL_TCM_ESM_TIMER_TIMEOUT = 0x0F46,
    /*
     * M
     */
    PDP_FAIL_PAM_ATT_PDN_ACCESS_REJECT_IMS_PDN_BLOCK_TEMP = 0x1402,
    /*
     * M
     */
    PDP_FAIL_DATA_NOT_ALLOW = 0x1671,
    /*
     * M
     */
    PDP_FAIL_OEM_DCFAILCAUSE_1 = 0x1001,
    PDP_FAIL_OEM_DCFAILCAUSE_2 = 0x1002,
    PDP_FAIL_OEM_DCFAILCAUSE_3 = 0x1003,
    PDP_FAIL_OEM_DCFAILCAUSE_4 = 0x1004,
    PDP_FAIL_OEM_DCFAILCAUSE_5 = 0x1005,
    PDP_FAIL_OEM_DCFAILCAUSE_6 = 0x1006,
    PDP_FAIL_OEM_DCFAILCAUSE_7 = 0x1007,
    PDP_FAIL_OEM_DCFAILCAUSE_8 = 0x1008,
    PDP_FAIL_OEM_DCFAILCAUSE_9 = 0x1009,
    PDP_FAIL_OEM_DCFAILCAUSE_10 = 0x100A,
    PDP_FAIL_OEM_DCFAILCAUSE_11 = 0x100B,
    PDP_FAIL_OEM_DCFAILCAUSE_12 = 0x100C,
    PDP_FAIL_OEM_DCFAILCAUSE_13 = 0x100D,
    PDP_FAIL_OEM_DCFAILCAUSE_14 = 0x100E,
    PDP_FAIL_OEM_DCFAILCAUSE_15 = 0x100F,
    /*
     * Not mentioned in the specification
     */
    PDP_FAIL_VOICE_REGISTRATION_FAIL = -1,
    PDP_FAIL_DATA_REGISTRATION_FAIL = -2,
    /*
     * reasons for data call drop - network/modem disconnect
     */
    PDP_FAIL_SIGNAL_LOST = -3,
    PDP_FAIL_PREF_RADIO_TECH_CHANGED = -4,
    /*
     * preferred technology has changed, should retry
     * with parameters appropriate for new technology
     */
    PDP_FAIL_RADIO_POWER_OFF = -5,
    /*
     * data call was disconnected because radio was resetting,
     * powered off - no retry
     */
    PDP_FAIL_TETHERED_CALL_ACTIVE = -6,
    /*
     * data call was disconnected by modem because tethered
     * mode was up on same APN/data profile - no retry until
     * tethered call is off
     */
    PDP_FAIL_ROUTER_ADVERTISEMENT_FAIL = -7,
    /*
     * for RA fail cause
     *
     *
     * Common cause for PDN deactivation by framework or network
     */
    PDP_FAIL_LOST_CONNECTION = 0x10004,
    PDP_FAIL_ERROR_UNSPECIFIED = 0xffff,
}
