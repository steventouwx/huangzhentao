/**
 * @file ql_voice_common.h
 * @brief Voice service common define
 *
 * @detailes
 * EDIT HISTORY
 * This section contains comments describing changes made to the file.
 * Notice that changes are listed in reverse chronological order.
 *
 * $Header: $
 * when      |what, where, why
 * ----------|---------------------------------------------------------
 * 20250108  |Created
 */

#ifndef __QL_VOICE_COMMON_H__
#define __QL_VOICE_COMMON_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QL_VOICE_MAX_PHONE_NUMBER           (82)
#define QL_VOICE_MAX_RECORDS                (8)
#define QL_VOICE_MAX_DTMF_LENGTH            (20)
#define QL_VOICE_MAX_FW_DETAIL_LENGTH       (13)
#define QL_VOICE_MAX_ECALL_MSD              (140)
#define QL_VOICE_MAX_USSD_LENGTH            (128)   /**  Maximum USSD length. */
#define QL_VOICE_MAX_NUMBER_LENGTH          (32)    /**<  Maximum length of VOICE number. */
#define QL_VOICE_MAX_WHITE_LIST_MEM_SIZE    (512)   /**<  Maximum length of VOICE white-list memory size. */
#define QL_VOICE_MAX_WHITE_LIST_COUNT       (QL_VOICE_MAX_WHITE_LIST_MEM_SIZE / QL_VOICE_MAX_NUMBER_LENGTH) /**<  Maximum length of VOICE white-list count. */

/** enum call state*/
typedef enum
{
    QL_VOICE_STATE_INCOMING = 0x0000, /**< Call is incoming. */
    QL_VOICE_STATE_DIALING  = 0x0001, /**< Dialing state. */
    QL_VOICE_STATE_ALERTING = 0x0002, /**< Alterting. */
    QL_VOICE_STATE_ACTIVE   = 0x0003, /**< Call is active. */
    QL_VOICE_STATE_HOLDING  = 0x0004, /**< Call is on hold. */
    QL_VOICE_STATE_END      = 0x0005, /**< Call is disconnected. */
    QL_VOICE_STATE_WAITING  = 0x0006, /**< Call is waiting. */
    QL_VOICE_STATE_SETUP    = 0x0007, /**< MT Call is in Setup state in 3GPP. */
} QL_VOICE_STATE_E;

/** enum call tech*/
typedef enum
{
    QL_VOICE_TECH_NONE  = 0, /**< None. */
    QL_VOICE_TECH_3GPP  = 1, /**< 3GPP. */
    QL_VOICE_TECH_3GPP2 = 2, /**< 3GPP2. */
} QL_VOICE_TECH_E;

/** enum call direction*/
typedef enum
{
    QL_VOICE_DIR_MO = 0, /**< Mobile originated call */
    QL_VOICE_DIR_MT = 1, /**< Mobile terminated call */
} QL_VOICE_DIR_E;

/**
 * enum call type
 */
typedef enum
{
    QL_VOICE_CALL_TYPE_MIN = 0,
    QL_VOICE_CALL_TYPE_VOICE,     /**< voice call */
    QL_VOICE_CALL_TYPE_EMERGENCY, /**< emergency call */
    QL_VOICE_CALL_TYPE_ECALL,     /**< eCall */
    QL_VOICE_CALL_TYPE_MAX
} QL_voice_call_type_E;

/**
 * enum call alerting type
 */
typedef enum
{
    QL_VOICE_CALL_ALERTING_TYPE_MIN = 0,
    QL_VOICE_CALL_ALERTING_TYPE_LOCAL,    /**< alerting local call */
    QL_VOICE_CALL_ALERTING_TYPE_REMOTE,   /**< alerting remote call */
    QL_VOICE_CALL_ALERTING_TYPE_MAX
} QL_voice_call_alerting_type_E;

/** enum call event type*/
typedef enum
{
    VOICE_CALL_EVENT,   /**< call event*/
    VOICE_MUTE_EVENT,   /**< mute event */
    VOICE_DTMF_EVENT,   /**< dtmf event */
    VOICE_ECALL_EVENT,  /**< ecall event */
    VOICE_ECALL_STATUS, /**< ecall status*/
    VOICE_USSD_EVENT, /**< USSD event*/
} VOICE_EVENT_E;

/**enum call end reason*/
typedef enum
{
    QL_VOICE_END_REASON_NONE                                                    = 0,
    QL_VOICE_END_REASON_OFFLINE                                                 = 1,
    QL_VOICE_END_REASON_CDMA_LOCK                                               = 2,
    QL_VOICE_END_REASON_NO_SRV                                                  = 3,
    QL_VOICE_END_REASON_FADE                                                    = 4,
    QL_VOICE_END_REASON_INTERCEPT                                               = 5,
    QL_VOICE_END_REASON_REORDER                                                 = 6,
    QL_VOICE_END_REASON_REL_NORMAL                                              = 7,
    QL_VOICE_END_REASON_REL_SO_REJ                                              = 8,
    QL_VOICE_END_REASON_INCOM_CALL                                              = 9,
    QL_VOICE_END_REASON_ALERT_STOP                                              = 10,
    QL_VOICE_END_REASON_CLIENT_END                                              = 11,
    QL_VOICE_END_REASON_ACTIVATION                                              = 12,
    QL_VOICE_END_REASON_MC_ABORT                                                = 13,
    QL_VOICE_END_REASON_MAX_ACCESS_PROBE                                        = 14,
    QL_VOICE_END_REASON_PSIST_N                                                 = 15,
    QL_VOICE_END_REASON_UIM_NOT_PRESENT                                         = 16,
    QL_VOICE_END_REASON_ACC_IN_PROG                                             = 17,
    QL_VOICE_END_REASON_ACC_FAIL                                                = 18,
    QL_VOICE_END_REASON_RETRY_ORDER                                             = 19,
    QL_VOICE_END_REASON_CCS_NOT_SUPPORTED_BY_BS                                 = 20,
    QL_VOICE_END_REASON_NO_RESPONSE_FROM_BS                                     = 21,
    QL_VOICE_END_REASON_REJECTED_BY_BS                                          = 22,
    QL_VOICE_END_REASON_INCOMPATIBLE                                            = 23,
    QL_VOICE_END_REASON_ACCESS_BLOCK                                            = 24,
    QL_VOICE_END_REASON_ALREADY_IN_TC                                           = 25,
    QL_VOICE_END_REASON_EMERGENCY_FLASHED                                       = 26,
    QL_VOICE_END_REASON_USER_CALL_ORIG_DURING_GPS                               = 27,
    QL_VOICE_END_REASON_USER_CALL_ORIG_DURING_SMS                               = 28,
    QL_VOICE_END_REASON_USER_CALL_ORIG_DURING_DATA                              = 29,
    QL_VOICE_END_REASON_REDIR_OR_HANDOFF                                        = 30,
    QL_VOICE_END_REASON_ACCESS_BLOCK_ALL                                        = 31,
    QL_VOICE_END_REASON_OTASP_SPC_ERR                                           = 32,
    QL_VOICE_END_REASON_IS707B_MAX_ACC                                          = 33,
    QL_VOICE_END_REASON_ACC_FAIL_REJ_ORD                                        = 34,
    QL_VOICE_END_REASON_ACC_FAIL_RETRY_ORD                                      = 35,
    QL_VOICE_END_REASON_TIMEOUT_T42                                             = 36,
    QL_VOICE_END_REASON_TIMEOUT_T40                                             = 37,
    QL_VOICE_END_REASON_SRV_INIT_FAIL                                           = 38,
    QL_VOICE_END_REASON_T50_EXP                                                 = 39,
    QL_VOICE_END_REASON_T51_EXP                                                 = 40,
    QL_VOICE_END_REASON_RL_ACK_TIMEOUT                                          = 41,
    QL_VOICE_END_REASON_BAD_FL                                                  = 42,
    QL_VOICE_END_REASON_TRM_REQ_FAIL                                            = 43,
    QL_VOICE_END_REASON_TIMEOUT_T41                                             = 44,
    QL_VOICE_END_REASON_INCOM_REJ                                               = 45,
    QL_VOICE_END_REASON_SETUP_REJ                                               = 46,
    QL_VOICE_END_REASON_NETWORK_END                                             = 47,
    QL_VOICE_END_REASON_NO_FUNDS                                                = 48,
    QL_VOICE_END_REASON_NO_GW_SRV                                               = 49,
    QL_VOICE_END_REASON_NO_CDMA_SRV                                             = 50,
    QL_VOICE_END_REASON_NO_FULL_SRV                                             = 51,
    QL_VOICE_END_REASON_MAX_PS_CALLS                                            = 52,
    QL_VOICE_END_REASON_UNKNOWN_SUBSCRIBER                                      = 53,
    QL_VOICE_END_REASON_ILLEGAL_SUBSCRIBER                                      = 54,
    QL_VOICE_END_REASON_BEARER_SERVICE_NOT_PROVISIONED                          = 55,
    QL_VOICE_END_REASON_TELE_SERVICE_NOT_PROVISIONED                            = 56,
    QL_VOICE_END_REASON_ILLEGAL_EQUIPMENT                                       = 57,
    QL_VOICE_END_REASON_CALL_BARRED                                             = 58,
    QL_VOICE_END_REASON_ILLEGAL_SS_OPERATION                                    = 59,
    QL_VOICE_END_REASON_SS_ERROR_STATUS                                         = 60,
    QL_VOICE_END_REASON_SS_NOT_AVAILABLE                                        = 61,
    QL_VOICE_END_REASON_SS_SUBSCRIPTION_VIOLATION                               = 62,
    QL_VOICE_END_REASON_SS_INCOMPATIBILITY                                      = 63,
    QL_VOICE_END_REASON_FACILITY_NOT_SUPPORTED                                  = 64,
    QL_VOICE_END_REASON_ABSENT_SUBSCRIBER                                       = 65,
    QL_VOICE_END_REASON_SHORT_TERM_DENIAL                                       = 66,
    QL_VOICE_END_REASON_LONG_TERM_DENIAL                                        = 67,
    QL_VOICE_END_REASON_SYSTEM_FAILURE                                          = 68,
    QL_VOICE_END_REASON_DATA_MISSING                                            = 69,
    QL_VOICE_END_REASON_UNEXPECTED_DATA_VALUE                                   = 70,
    QL_VOICE_END_REASON_PWD_REGISTRATION_FAILURE                                = 71,
    QL_VOICE_END_REASON_NEGATIVE_PWD_CHECK                                      = 72,
    QL_VOICE_END_REASON_NUM_OF_PWD_ATTEMPTS_VIOLATION                           = 73,
    QL_VOICE_END_REASON_POSITION_METHOD_FAILURE                                 = 74,
    QL_VOICE_END_REASON_UNKNOWN_ALPHABET                                        = 75,
    QL_VOICE_END_REASON_USSD_BUSY                                               = 76,
    QL_VOICE_END_REASON_REJECTED_BY_USER                                        = 77,
    QL_VOICE_END_REASON_REJECTED_BY_NETWORK                                     = 78,
    QL_VOICE_END_REASON_DEFLECTION_TO_SERVED_SUBSCRIBER                         = 79,
    QL_VOICE_END_REASON_SPECIAL_SERVICE_CODE                                    = 80,
    QL_VOICE_END_REASON_INVALID_DEFLECTED_TO_NUMBER                             = 81,
    QL_VOICE_END_REASON_MPTY_PARTICIPANTS_EXCEEDED                              = 82,
    QL_VOICE_END_REASON_RESOURCES_NOT_AVAILABLE                                 = 83,
    QL_VOICE_END_REASON_UNASSIGNED_NUMBER                                       = 84,
    QL_VOICE_END_REASON_NO_ROUTE_TO_DESTINATION                                 = 85,
    QL_VOICE_END_REASON_CHANNEL_UNACCEPTABLE                                    = 86,
    QL_VOICE_END_REASON_OPERATOR_DETERMINED_BARRING                             = 87,
    QL_VOICE_END_REASON_NORMAL_CALL_CLEARING                                    = 88,
    QL_VOICE_END_REASON_USER_BUSY                                               = 89,
    QL_VOICE_END_REASON_NO_USER_RESPONDING                                      = 90,
    QL_VOICE_END_REASON_USER_ALERTING_NO_ANSWER                                 = 91,
    QL_VOICE_END_REASON_CALL_REJECTED                                           = 92,
    QL_VOICE_END_REASON_NUMBER_CHANGED                                          = 93,
    QL_VOICE_END_REASON_PREEMPTION                                              = 94,
    QL_VOICE_END_REASON_DESTINATION_OUT_OF_ORDER                                = 95,
    QL_VOICE_END_REASON_INVALID_NUMBER_FORMAT                                   = 96,
    QL_VOICE_END_REASON_FACILITY_REJECTED                                       = 97,
    QL_VOICE_END_REASON_RESP_TO_STATUS_ENQUIRY                                  = 98,
    QL_VOICE_END_REASON_NORMAL_UNSPECIFIED                                      = 99,
    QL_VOICE_END_REASON_NO_CIRCUIT_OR_CHANNEL_AVAILABLE                         = 100,
    QL_VOICE_END_REASON_NETWORK_OUT_OF_ORDER                                    = 101,
    QL_VOICE_END_REASON_TEMPORARY_FAILURE                                       = 102,
    QL_VOICE_END_REASON_SWITCHING_EQUIPMENT_CONGESTION                          = 103,
    QL_VOICE_END_REASON_ACCESS_INFORMATION_DISCARDED                            = 104,
    QL_VOICE_END_REASON_REQUESTED_CIRCUIT_OR_CHANNEL_NOT_AVAILABLE              = 105,
    QL_VOICE_END_REASON_RESOURCES_UNAVAILABLE_OR_UNSPECIFIED                    = 106,
    QL_VOICE_END_REASON_QOS_UNAVAILABLE                                         = 107,
    QL_VOICE_END_REASON_REQUESTED_FACILITY_NOT_SUBSCRIBED                       = 108,
    QL_VOICE_END_REASON_INCOMING_CALLS_BARRED_WITHIN_CUG                        = 109,
    QL_VOICE_END_REASON_BEARER_CAPABILITY_NOT_AUTH                              = 110,
    QL_VOICE_END_REASON_BEARER_CAPABILITY_UNAVAILABLE                           = 111,
    QL_VOICE_END_REASON_SERVICE_OPTION_NOT_AVAILABLE                            = 112,
    QL_VOICE_END_REASON_ACM_LIMIT_EXCEEDED                                      = 113,
    QL_VOICE_END_REASON_BEARER_SERVICE_NOT_IMPLEMENTED                          = 114,
    QL_VOICE_END_REASON_REQUESTED_FACILITY_NOT_IMPLEMENTED                      = 115,
    QL_VOICE_END_REASON_ONLY_DIGITAL_INFORMATION_BEARER_AVAILABLE               = 116,
    QL_VOICE_END_REASON_SERVICE_OR_OPTION_NOT_IMPLEMENTED                       = 117,
    QL_VOICE_END_REASON_INVALID_TRANSACTION_IDENTIFIER                          = 118,
    QL_VOICE_END_REASON_USER_NOT_MEMBER_OF_CUG                                  = 119,
    QL_VOICE_END_REASON_INCOMPATIBLE_DESTINATION                                = 120,
    QL_VOICE_END_REASON_INVALID_TRANSIT_NW_SELECTION                            = 121,
    QL_VOICE_END_REASON_SEMANTICALLY_INCORRECT_MESSAGE                          = 122,
    QL_VOICE_END_REASON_INVALID_MANDATORY_INFORMATION                           = 123,
    QL_VOICE_END_REASON_MESSAGE_TYPE_NON_IMPLEMENTED                            = 124,
    QL_VOICE_END_REASON_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE         = 125,
    QL_VOICE_END_REASON_INFORMATION_ELEMENT_NON_EXISTENT                        = 126,
    QL_VOICE_END_REASON_CONDITONAL_IE_ERROR                                     = 127,
    QL_VOICE_END_REASON_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE              = 128,
    QL_VOICE_END_REASON_RECOVERY_ON_TIMER_EXPIRED                               = 129,
    QL_VOICE_END_REASON_PROTOCOL_ERROR_UNSPECIFIED                              = 130,
    QL_VOICE_END_REASON_INTERWORKING_UNSPECIFIED                                = 131,
    QL_VOICE_END_REASON_OUTGOING_CALLS_BARRED_WITHIN_CUG                        = 132,
    QL_VOICE_END_REASON_NO_CUG_SELECTION                                        = 133,
    QL_VOICE_END_REASON_UNKNOWN_CUG_INDEX                                       = 134,
    QL_VOICE_END_REASON_CUG_INDEX_INCOMPATIBLE                                  = 135,
    QL_VOICE_END_REASON_CUG_CALL_FAILURE_UNSPECIFIED                            = 136,
    QL_VOICE_END_REASON_CLIR_NOT_SUBSCRIBED                                     = 137,
    QL_VOICE_END_REASON_CCBS_POSSIBLE                                           = 138,
    QL_VOICE_END_REASON_CCBS_NOT_POSSIBLE                                       = 139,
    QL_VOICE_END_REASON_IMSI_UNKNOWN_IN_HLR                                     = 140,
    QL_VOICE_END_REASON_ILLEGAL_MS                                              = 141,
    QL_VOICE_END_REASON_IMSI_UNKNOWN_IN_VLR                                     = 142,
    QL_VOICE_END_REASON_IMEI_NOT_ACCEPTED                                       = 143,
    QL_VOICE_END_REASON_ILLEGAL_ME                                              = 144,
    QL_VOICE_END_REASON_PLMN_NOT_ALLOWED                                        = 145,
    QL_VOICE_END_REASON_LOCATION_AREA_NOT_ALLOWED                               = 146,
    QL_VOICE_END_REASON_ROAMING_NOT_ALLOWED_IN_THIS_LOCATION_AREA               = 147,
    QL_VOICE_END_REASON_NO_SUITABLE_CELLS_IN_LOCATION_AREA                      = 148,
    QL_VOICE_END_REASON_NETWORK_FAILURE                                         = 149,
    QL_VOICE_END_REASON_MAC_FAILURE                                             = 150,
    QL_VOICE_END_REASON_SYNCH_FAILURE                                           = 151,
    QL_VOICE_END_REASON_NETWORK_CONGESTION                                      = 152,
    QL_VOICE_END_REASON_GSM_AUTHENTICATION_UNACCEPTABLE                         = 153,
    QL_VOICE_END_REASON_SERVICE_NOT_SUBSCRIBED                                  = 154,
    QL_VOICE_END_REASON_SERVICE_TEMPORARILY_OUT_OF_ORDER                        = 155,
    QL_VOICE_END_REASON_CALL_CANNOT_BE_IDENTIFIED                               = 156,
    QL_VOICE_END_REASON_INCORRECT_SEMANTICS_IN_MESSAGE                          = 157,
    QL_VOICE_END_REASON_MANDATORY_INFORMATION_INVALID                           = 158,
    QL_VOICE_END_REASON_ACCESS_STRATUM_FAILURE                                  = 159,
    QL_VOICE_END_REASON_INVALID_SIM                                             = 160,
    QL_VOICE_END_REASON_WRONG_STATE                                             = 161,
    QL_VOICE_END_REASON_ACCESS_CLASS_BLOCKED                                    = 162,
    QL_VOICE_END_REASON_NO_RESOURCES                                            = 163,
    QL_VOICE_END_REASON_INVALID_USER_DATA                                       = 164,
    QL_VOICE_END_REASON_TIMER_T3230_EXPIRED                                     = 165,
    QL_VOICE_END_REASON_NO_CELL_AVAILABLE                                       = 166,
    QL_VOICE_END_REASON_ABORT_MSG_RECEIVED                                      = 167,
    QL_VOICE_END_REASON_RADIO_LINK_LOST                                         = 168,
    QL_VOICE_END_REASON_TIMER_T303_EXPIRED                                      = 169,
    QL_VOICE_END_REASON_CNM_MM_REL_PENDING                                      = 170,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_RR_REL_IND                           = 171,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_RR_RANDOM_ACCESS_FAILURE             = 172,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_RRC_REL_IND                          = 173,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_RRC_CLOSE_SESSION_IND                = 174,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_RRC_OPEN_SESSION_FAILURE             = 175,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_LOW_LEVEL_FAIL                       = 176,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_LOW_LEVEL_FAIL_REDIAL_NOT_ALLOWED    = 177,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_LOW_LEVEL_IMMED_RETRY                = 178,
    QL_VOICE_END_REASON_ACCESS_STRATUM_REJ_ABORT_RADIO_UNAVAILABLE              = 179,
    QL_VOICE_END_REASON_SERVICE_OPTION_NOT_SUPPORTED                            = 180,
    QL_VOICE_END_REASON_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_TXN                     = 181,
    QL_VOICE_END_REASON_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_HO                      = 182,
    QL_VOICE_END_REASON_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_CONN_REL                = 183,
    QL_VOICE_END_REASON_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_RLF                     = 184,
    QL_VOICE_END_REASON_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN           = 185,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_SUCCESS                            = 186,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE                            = 187,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_ABORTED                    = 188,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_ACCESS_BARRED              = 189,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_CELL_RESEL                 = 190,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_CONFIG_FAILURE             = 191,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_TIMER_EXPIRED              = 192,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_LINK_FAILURE               = 193,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_NOT_CAMPED                 = 194,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_SI_FAILURE                 = 195,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_EST_FAILURE_CONN_REJECT                = 196,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_REL_NORMAL                             = 197,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_REL_RLF                                = 198,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_REL_CRE_FAILURE                        = 199,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_REL_OOS_DURING_CRE                     = 200,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_REL_ABORTED                            = 201,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_REL_SIB_READ_ERROR                     = 202,
    QL_VOICE_END_REASON_AS_REJ_LRRC_CONN_REL_ABORTED_IRAT_SUCCESS               = 203,
    QL_VOICE_END_REASON_AS_REJ_LRRC_RADIO_LINK_FAILURE                          = 204,
    QL_VOICE_END_REASON_AS_REJ_DETACH_WITH_REATTACH_LTE_NW_DETACH               = 205,
    QL_VOICE_END_REASON_AS_REJ_DETACH_WITH_OUT_REATTACH_LTE_NW_DETACH           = 206,
    QL_VOICE_END_REASON_BAD_REQ_WAIT_INVITE                                     = 207,
    QL_VOICE_END_REASON_BAD_REQ_WAIT_REINVITE                                   = 208,
    QL_VOICE_END_REASON_INVALID_REMOTE_URI                                      = 209,
    QL_VOICE_END_REASON_REMOTE_UNSUPP_MEDIA_TYPE                                = 210,
    QL_VOICE_END_REASON_PEER_NOT_REACHABLE                                      = 211,
    QL_VOICE_END_REASON_NETWORK_NO_RESP_TIME_OUT                                = 212,
    QL_VOICE_END_REASON_NETWORK_NO_RESP_HOLD_FAIL                               = 213,
    QL_VOICE_END_REASON_DATA_CONNECTION_LOST                                    = 214,
    QL_VOICE_END_REASON_UPGRADE_DOWNGRADE_REJ                                   = 215,
    QL_VOICE_END_REASON_SIP_403_FORBIDDEN                                       = 216,
    QL_VOICE_END_REASON_NO_NETWORK_RESP                                         = 217,
    QL_VOICE_END_REASON_UPGRADE_DOWNGRADE_FAILED                                = 218,
    QL_VOICE_END_REASON_UPGRADE_DOWNGRADE_CANCELLED                             = 219,
    QL_VOICE_END_REASON_SSAC_REJECT                                             = 220,
    QL_VOICE_END_REASON_THERMAL_EMERGENCY                                       = 221,
    QL_VOICE_END_REASON_1XCSFB_SOFT_FAILURE                                     = 222,
    QL_VOICE_END_REASON_1XCSFB_HARD_FAILURE                                     = 223,
    QL_VOICE_END_REASON_CONNECTION_EST_FAILURE                                  = 224,
    QL_VOICE_END_REASON_CONNECTION_FAILURE                                      = 225,
    QL_VOICE_END_REASON_RRC_CONN_REL_NO_MT_SETUP                                = 226,
    QL_VOICE_END_REASON_ESR_FAILURE                                             = 227,
    QL_VOICE_END_REASON_MT_CSFB_NO_RESPONSE_FROM_NW                             = 228,
    QL_VOICE_END_REASON_BUSY_EVERYWHERE                                         = 229,
    QL_VOICE_END_REASON_ANSWERED_ELSEWHERE                                      = 230,
    QL_VOICE_END_REASON_RLF_DURING_CC_DISCONNECT                                = 231,
    QL_VOICE_END_REASON_TEMP_REDIAL_ALLOWED                                     = 232,
    QL_VOICE_END_REASON_PERM_REDIAL_NOT_NEEDED                                  = 233,
    QL_VOICE_END_REASON_MERGED_TO_CONFERENCE                                    = 234,
    QL_VOICE_END_REASON_LOW_BATTERY                                             = 235,
    QL_VOICE_END_REASON_CALL_DEFLECTED                                          = 236,
    QL_VOICE_END_REASON_RTP_RTCP_TIMEOUT                                        = 237,
    QL_VOICE_END_REASON_RINGING_RINGBACK_TIMEOUT                                = 238,
    QL_VOICE_END_REASON_REG_RESTORATION                                         = 239,
    QL_VOICE_END_REASON_CODEC_ERROR                                             = 240,
    QL_VOICE_END_REASON_UNSUPPORTED_SDP                                         = 241,
    QL_VOICE_END_REASON_RTP_FAILURE                                             = 242,
    QL_VOICE_END_REASON_QoS_FAILURE                                             = 243,
    QL_VOICE_END_REASON_MULTIPLE_CHOICES                                        = 244,
    QL_VOICE_END_REASON_MOVED_PERMANENTLY                                       = 245,
    QL_VOICE_END_REASON_MOVED_TEMPORARILY                                       = 246,
    QL_VOICE_END_REASON_USE_PROXY                                               = 247,
    QL_VOICE_END_REASON_ALTERNATE_SERVICE                                       = 248,
    QL_VOICE_END_REASON_ALTERNATE_EMERGENCY_CALL                                = 249,
    QL_VOICE_END_REASON_UNAUTHORIZED                                            = 250,
    QL_VOICE_END_REASON_PAYMENT_REQUIRED                                        = 251,
    QL_VOICE_END_REASON_METHOD_NOT_ALLOWED                                      = 252,
    QL_VOICE_END_REASON_NOT_ACCEPTABLE                                          = 253,
    QL_VOICE_END_REASON_PROXY_AUTHENTICATION_REQUIRED                           = 254,
    QL_VOICE_END_REASON_GONE                                                    = 255,
    QL_VOICE_END_REASON_REQUEST_ENTITY_TOO_LARGE                                = 256,
    QL_VOICE_END_REASON_REQUEST_URI_TOO_LARGE                                   = 257,
    QL_VOICE_END_REASON_UNSUPPORTED_URI_SCHEME                                  = 258,
    QL_VOICE_END_REASON_BAD_EXTENSION                                           = 259,
    QL_VOICE_END_REASON_EXTENSION_REQUIRED                                      = 260,
    QL_VOICE_END_REASON_INTERVAL_TOO_BRIEF                                      = 261,
    QL_VOICE_END_REASON_CALL_OR_TRANS_DOES_NOT_EXIST                            = 262,
    QL_VOICE_END_REASON_LOOP_DETECTED                                           = 263,
    QL_VOICE_END_REASON_TOO_MANY_HOPS                                           = 264,
    QL_VOICE_END_REASON_ADDRESS_INCOMPLETE                                      = 265,
    QL_VOICE_END_REASON_AMBIGUOUS                                               = 266,
    QL_VOICE_END_REASON_REQUEST_TERMINATED                                      = 267,
    QL_VOICE_END_REASON_NOT_ACCEPTABLE_HERE                                     = 268,
    QL_VOICE_END_REASON_REQUEST_PENDING                                         = 269,
    QL_VOICE_END_REASON_UNDECIPHERABLE                                          = 270,
    QL_VOICE_END_REASON_SERVER_INTERNAL_ERROR                                   = 271,
    QL_VOICE_END_REASON_NOT_IMPLEMENTED                                         = 272,
    QL_VOICE_END_REASON_BAD_GATEWAY                                             = 273,
    QL_VOICE_END_REASON_SERVER_TIME_OUT                                         = 274,
    QL_VOICE_END_REASON_VERSION_NOT_SUPPORTED                                   = 275,
    QL_VOICE_END_REASON_MESSAGE_TOO_LARGE                                       = 276,
    QL_VOICE_END_REASON_DOES_NOT_EXIST_ANYWHERE                                 = 277,
    QL_VOICE_END_REASON_SESS_DESCR_NOT_ACCEPTABLE                               = 278,
    QL_VOICE_END_REASON_SRVCC_END_CALL                                          = 279,
    QL_VOICE_END_REASON_INTERNAL_ERROR                                          = 280,
    QL_VOICE_END_REASON_SERVER_UNAVAILABLE                                      = 281,
    QL_VOICE_END_REASON_PRECONDITION_FAILURE                                    = 282,
    QL_VOICE_END_REASON_DRVCC_IN_PROG                                           = 283,
    QL_VOICE_END_REASON_DRVCC_END_CALL                                          = 284,
    QL_VOICE_END_REASON_CS_HARD_FAILURE                                         = 285,
    QL_VOICE_END_REASON_CS_ACQ_FAILURE                                          = 286,
    QL_VOICE_END_REASON_FALLBACK_TO_CS                                          = 287,
    QL_VOICE_END_REASON_DEAD_BATTERY                                            = 288,
    QL_VOICE_END_REASON_HO_NOT_FEASIBLE                                         = 289,
    QL_VOICE_END_REASON_PDN_DISCONNECTED                                        = 290,
    QL_VOICE_END_REASON_REJECTED_ELSEWHERE                                      = 291,
    QL_VOICE_END_REASON_CALL_PULLED                                             = 292,
    QL_VOICE_END_REASON_CALL_PULL_OUT_OF_SYNC                                   = 293,
    QL_VOICE_END_REASON_HOLD_RESUME_FAILED                                      = 294,
    QL_VOICE_END_REASON_HOLD_RESUME_CANCELED                                    = 295,
    QL_VOICE_END_REASON_REINVITE_COLLISION                                      = 296,
    QL_VOICE_END_REASON_1XCSFB_MSG_INVAILD                                      = 297,
    QL_VOICE_END_REASON_1XCSFB_MSG_IGNORE                                       = 298,
    QL_VOICE_END_REASON_1XCSFB_FAIL_ACQ_FAIL                                    = 299,
    QL_VOICE_END_REASON_1XCSFB_FAIL_CALL_REL_REL_ORDER                          = 300,
    QL_VOICE_END_REASON_1XCSFB_FAIL_CALL_REL_REORDER                            = 301,
    QL_VOICE_END_REASON_1XCSFB_FAIL_CALL_REL_INTERCEPT_ORDER                    = 302,
    QL_VOICE_END_REASON_1XCSFB_FAIL_CALL_REL_NORMAL                             = 303,
    QL_VOICE_END_REASON_1XCSFB_FAIL_CALL_REL_SO_REJ                             = 304,
    QL_VOICE_END_REASON_1XCSFB_FAIL_CALL_REL_OTASP_SPC_ERR                      = 305,
    QL_VOICE_END_REASON_1XCSFB_FAILURE_SRCH_TT_FAIL                             = 306,
    QL_VOICE_END_REASON_1XCSFB_FAILURE_TCH_INIT_FAIL                            = 307,
    QL_VOICE_END_REASON_1XCSFB_FAILURE_FAILURE_USER_CALL_END                    = 308,
    QL_VOICE_END_REASON_1XCSFB_FAILURE_FAILURE_RETRY_EXHAUST                    = 309,
    QL_VOICE_END_REASON_1XCSFB_FAILURE_FAILURE_CALL_REL_REG_REJ                 = 310,
    QL_VOICE_END_REASON_1XCSFB_FAILURE_FAILURE_CALL_REL_NW_REL_ODR              = 311,
    QL_VOICE_END_REASON_1XCSFB_HO_FAILURE                                       = 312,
    QL_VOICE_END_REASON_EMM_REJ_TIMER_T3417_EXT_EXP                             = 313,
    QL_VOICE_END_REASON_EMM_REJ_TIMER_T3417_EXP                                 = 314,
    QL_VOICE_END_REASON_EMM_REJ_SERVICE_REQ_FAILURE_LTE_NW_REJECT               = 315,
    QL_VOICE_END_REASON_EMM_REJ_SERVICE_REQ_FAILURE_CS_DOMAIN_NOT_AVAILABLE     = 316,
    QL_VOICE_END_REASON_EMM_REJ                                                 = 317,
    QL_VOICE_CALL_END_CAUSE_PROTOCOL_Q850_ERROR                                 = 318,
    QL_VOICE_CALL_END_CAUSE_CSFB_NOT_FEASIBLE_IN_ROAM_CS_NW                     = 319,
    QL_VOICE_CALL_END_CAUSE_SIP_503_SERVER_UNAVAILABLE                          = 320,
    QL_VOICE_CALL_END_CAUSE_INCOMING_REJ_CAUSE_CALL_ONGOING_CW_DISABLED         = 321,
    QL_VOICE_CALL_END_CAUSE_INCOMING_REJ_CAUSE_CALL_ON_OTHER_SUB                = 322,
    QL_VOICE_CALL_END_CAUSE_INCOMING_REJ_CAUSE_1X_COLLISION                     = 323,
    QL_VOICE_CALL_END_CAUSE_SRV_NOT_REGISTERED                                  = 324,
    QL_VOICE_CALL_END_CAUSE_CALL_TYPE_NOT_ALLOWED                               = 325,
    QL_VOICE_CALL_END_CAUSE_EMRG_CALL_ONGOING                                   = 326,
    QL_VOICE_CALL_END_CAUSE_CALL_SETUP_ONGOING                                  = 327,
    QL_VOICE_CALL_END_CAUSE_MAX_CALL_LIMIT_REACHED                              = 328,
    QL_VOICE_CALL_END_CAUSE_UNSUPPORTED_SIP_HDRS                                = 329,
    QL_VOICE_CALL_END_CAUSE_CALL_TRANSFER_ONGOING                               = 330,
    QL_VOICE_CALL_END_CAUSE_PRACK_TIMEOUT                                       = 331,
    QL_VOICE_CALL_END_CAUSE_QOS_FAILURE                                         = 332,
    QL_VOICE_CALL_END_CAUSE_ONGOING_HANDOVER                                    = 333,
    QL_VOICE_CALL_END_CAUSE_VT_WITH_TTY_NOT_ALLOWED                             = 334,
    QL_VOICE_CALL_END_CAUSE_CALL_UPGRADE_ONGOING                                = 335,
    QL_VOICE_CALL_END_CAUSE_CONFERENCE_WITH_TTY_NOT_ALLOWED                     = 336,
    QL_VOICE_CALL_END_CAUSE_CALL_CONFERENCE_ONGOING                             = 337,
    QL_VOICE_CALL_END_CAUSE_VT_WITH_AVPF_NOT_ALLOWED                            = 338,
    QL_VOICE_CALL_END_CAUSE_ENCRYPTION_CALL_ONGOING                             = 339,
    QL_VOICE_CALL_END_CAUSE_INCOM_REJ_CAUSE_UI_NOT_READY                        = 340,
    QL_VOICE_CALL_END_CAUSE_CS_CALL_ONGOING                                     = 341,
    QL_VOICE_CALL_END_CAUSE_UNWANTED_CALL                                       = 342,
    QL_VOICE_CALL_END_CAUSE_INCOMING_REJ_CAUSE_CALL_ONGOING_CB_ENABLED          = 343,
    QL_VOICE_CALL_END_CAUSE_CALL_COMPLETED_ELSEWHERE                            = 344,
    QL_VOICE_CALL_END_CAUSE_RETRY_ON_IMS_WITHOUT_RTT                            = 345,
    QL_VOICE_CALL_END_CAUSE_NOT_ACCEPTABLE_GLOBAL                               = 346,
} QL_VOICE_END_REASON_E;

/** a voice call record information structure*/
typedef struct
{
    uint32_t                id;
    char                    number[QL_VOICE_MAX_PHONE_NUMBER + 1];
    QL_VOICE_STATE_E        state;
    QL_VOICE_TECH_E         tech;
    QL_VOICE_DIR_E          dir;
    QL_VOICE_END_REASON_E   end_reason;
    QL_voice_call_type_E    call_type;
    QL_voice_call_alerting_type_E call_alerting_type;
    uint16_t                sip_error_code;
} ql_voice_record_t;

/** multi voice call record information structure*/
typedef struct
{
    int len;    /**< length of records */
    ql_voice_record_t records[QL_VOICE_MAX_RECORDS];   /**< voice call records  */
} ql_voice_record_array_t;

/**
 * voice call callback function
 * @param[out] *p_arr: voice record info
 * @return no value
 */
typedef void (*ql_voice_call_cb_f)(ql_voice_record_array_t *p_arr);

/** enum voice dtmf event*/
typedef enum
{
    QL_VOICE_DTMF_EVENT_REV_BURST               = 0x00, /**< Sends a CDMA-burst DTMF \n  */
    QL_VOICE_DTMF_EVENT_REV_START_CONT          = 0x01, /**< Starts a continuous DTMF tone \n  */
    QL_VOICE_DTMF_EVENT_REV_STOP_CONT           = 0x02, /**< Stops a continuous DTMF tone \n  */
    QL_VOICE_DTMF_EVENT_FWD_BURST               = 0x03, /**< Received a CDMA-burst DTMF message \n  */
    QL_VOICE_DTMF_EVENT_FWD_START_CONT          = 0x04, /**< Received a start-continuous DTMF tone order \n  */
    QL_VOICE_DTMF_EVENT_FWD_STOP_CONT           = 0x05, /**< Received a stop-continuous DTMF tone order \n  */
    QL_VOICE_DTMF_EVENT_IP_INCOMING_DTMF_START  = 0x06, /**< Received an IP-start continuous DTMF message \n  */
    QL_VOICE_DTMF_EVENT_IP_INCOMING_DTMF_STOP   = 0x07, /**< Received an IP-stop continuous DTMF message  */
} QL_VOICE_DTMF_EVENT_E;

/** The VOICE white-list info structure. */
typedef struct
{
    int  list_count;
    char list_info[QL_VOICE_MAX_WHITE_LIST_COUNT][QL_VOICE_MAX_NUMBER_LENGTH];
} ql_voice_white_list_info_t;

/**
 * voice dtmf event callback function
 * @param[in] id voice call id
 * @param[in] event dtmf event
 * @param[in] digits dtmf digits
 * @param[in] len dtmf digits length
 * @return no value
 */
typedef void (*ql_voice_dtmf_cb_f)(uint32_t id, QL_VOICE_DTMF_EVENT_E event,
                                   char *digits, int len); /**< len: length of digits */

/**
 * enum forwarding condition
 */
typedef enum
{
    QL_VOICE__FW_COND_UNCONDITIONALLY = 0, /**<  Unconditional call forwarding. */
    QL_VOICE__FW_COND_MOBILEBUSY      = 1, /**<  Forward when the mobile device is busy. */
    QL_VOICE__FW_COND_NOREPLY         = 2, /**<  Forward when there is no reply. */
    QL_VOICE__FW_COND_UNREACHABLE     = 3, /**<  Forward when the call is unreachable. */
    QL_VOICE__FW_COND_ALLFORWARDING   = 4, /**<  All forwarding(0-3).*/
    QL_VOICE__FW_COND_ALLCONDITIONAL  = 5, /**<  All conditional forwarding(1-3). */
} QL_VOICE_FW_COND_E;

/** enum voice forwarding type  */
typedef enum
{
    QL_VOICE_FW_TYPE_VOICE         = 0, /**<  Forwarding Voice. */
    QL_VOICE_FW_TYPE_DATA          = 1, /**<  Forwarding Data. */
    QL_VOICE_FW_TYPE_VOICE_DATA    = 2, /**<  Forwarding voice and data. */
} QL_VOICE_FW_TYPE_E;

/**
 * enum voice subscription type
 */
typedef enum
{
    QL_VOICE_PRIMARY_SUBSCRIPTION      = 0,   /**<  PRIMARY SUBSCRIPTION Voice. */
    QL_VOICE_SECONDARY_SUBSCRIPTION    = 1,   /**<  SECONDARY SUBSCRIPTION Voice. */
} QL_VOICE_SUBSCRIPTION_E;

/** call forwarding infomation structure */
typedef struct
{
    int enabled; /**< voice forwarding enable.  */
    int len;    /**< length of details  */
    /** The voice call forwarding detail information structure. */
    struct {
        QL_VOICE_FW_TYPE_E type; /**< voice forwarding type.  */
        char number[QL_VOICE_MAX_PHONE_NUMBER + 1];  /**< voice forwarding number.  */
    } details[QL_VOICE_MAX_FW_DETAIL_LENGTH];
} ql_voice_fw_status_t;

typedef enum
{
    E_QL_VOICE_USSD_MSG_TYPE_NEW_MESSAGE        = 0, /**<  Initiate a new USSD sesion with network. */
    E_QL_VOICE_USSD_MSG_TYPE_REPLY_TO_IND       = 1, /**<  Reply to a USSD indication from the network. */
}ql_voice_ussd_msg_type_t;

typedef enum
{
    E_QL_VOICE_USSD_ENCODING_ASCII              = 0, /**<  ASCII coding scheme. */
    E_QL_VOICE_USSD_ENCODING_8BIT               = 1, /**<  8-bit coding scheme. */
    E_QL_VOICE_USSD_ENCODING_UCS2               = 2, /**<  UCS2. */
}ql_voice_ussd_encoding_type_t;

typedef struct
{
  ql_voice_ussd_msg_type_t      type;           /**<   Whether sending a new request or a reply to indication*/
  ql_voice_ussd_encoding_type_t encoding;       /**<   USSD encoding.*/
  char ussd_string[QL_VOICE_MAX_USSD_LENGTH + 1]; /**<   USSD string.*/
} ql_voice_ussd_info_t;  /* Message */

typedef enum
{
    QL_VOICE_USSD_INDICATION_FURTHER_ACTION_NOT_REQUIRED = 0, /**<  USSD indication requires a USSD reply. */
    QL_VOICE_USSD_INDICATION_FURTHER_ACTION_REQUIRED = 1,     /**<  USSD indication does not require a reply. */
} ql_voice_ussd_notification_t;

typedef struct
{
  ql_voice_ussd_notification_t notification;
  char ussd_string[QL_VOICE_MAX_USSD_LENGTH + 1]; /**<   USSD string.*/
} ql_voice_ussd_event_t;  /* Message */

/**
 * voice ussd callback function
 * @param[in] id voice call id
 * @return no value
 */
typedef void (*ql_voice_ussd_cb_f)(ql_voice_ussd_event_t *p_event);

/** enum ecall type */
typedef enum
{
    QL_VOICE_ECALL_TYPE_TEST        = 1, /**<  Test eCall */
    QL_VOICE_ECALL_TYPE_EMERGENCY   = 2, /**<  Emergency eCall */
    QL_VOICE_ECALL_TYPE_RECONFIG    = 3, /**<  Reconfig eCall */
} QL_VOICE_ECALL_TYPE_E;

/** ecall infomation structure */
typedef struct
{
    uint32_t msd_len;                            /**< Length of Minimum Set of Data */
    uint8_t msd[QL_VOICE_MAX_ECALL_MSD];         /**< Minimum Set of Data */
    QL_VOICE_ECALL_TYPE_E type;                  /**< eCall type */
    int auto_trigger;                            /**< 0 - Manually trigger eCall
                                                      1 - Automatically trigger eCall */
    char test_number[QL_VOICE_MAX_PHONE_NUMBER]; /**< Test number */
} ql_voice_ecall_info_t;

/**enum eCall state */
typedef enum
{
    QL_VOICE_ECALL_STATE_NONE                   = -1, /**<  ECALL unknown state */
    QL_VOICE_ECALL_STATE_INACTIVE               = 0,  /**<  ECALL inactive state */
    QL_VOICE_ECALL_STATE_ORIGINATING_CALL       = 1,  /**<  ECALL originating state */
    QL_VOICE_ECALL_STATE_IN_CALL_TRANSMITTING   = 2,  /**<  ECALL APP TRANSMITTING */
    QL_VOICE_ECALL_STATE_WAITING_FOR_AL_ACK     = 3,  /**<  ECALL waiting ALACK state */
    QL_VOICE_ECALL_STATE_IN_CALL                = 4,  /**<  ECALL in-call state */
    QL_VOICE_ECALL_STATE_IDLE_ALLOW_MT_ECALL    = 5,  /**<  ECALL idle allow mt eCall state */
} QL_VOICE_ECALL_STATE_E;

/** enum eCall mode */
typedef enum
{
    QL_VOICE_ECALL_MODE_NORMAL  = 0, /**< eCall enabled normal mode. */
    QL_VOICE_ECALL_MODE_ONLY    = 1, /**< eCall-only mode. */
    QL_VOICE_ECALL_MODE_DEFAULT = 2, /**< eCall default mode. */
} QL_VOICE_ECALL_MODE_E;

/** eCall configuration construction*/
typedef struct
{
    uint8_t enable;                 /**<  Enable or disable ecall: 0 -- Disable; 1 -- Enable */
    uint8_t voice_mute;             /**<  Enable or disable to mute IVS speaker in MSD
                                          0 - Disable to mute IVS speaker automatical in MSD transmission
                                          1 - Enable to mute IVS speaker automatical in MSD transmission */
    QL_VOICE_ECALL_MODE_E mode;     /**<  The Ecall mode. */
    uint8_t report_event;           /**<  Enable or disable to report ecall event info.
                                          0 - Disable;
                                          1 - Enable */
    uint16_t start_timer;           /**<  The timer of IVS waiting for "START". T5 */
    uint16_t hack_timer;            /**<  The timer of IVS waiting for "HACK". T6 */
    uint16_t trans_timer;           /**<  The timer of MSD transmission. T7 */
    uint8_t fail_redial;            /**<  The dial fail times. */
    uint8_t drop_redial;            /**<  The ecall abnormal disconnenct redial times. */
    uint16_t cleardown_timer;       /**<  The timer of IVS waiting for "ClearDown". T2 */
    uint16_t init_timer;            /**<  The timer of IVS waiting for "InitIATION". T3 */
    uint16_t nad_reg_timer;         /**<  The timer of IVS waiting for "NAD Registration". T9*/
    uint16_t nad_dereg_timer;       /**<  The timer of IVS waiting for "NAD Deregistration". T10*/
    uint8_t standard;               /**<  The ecall system standard.
                                          0 - EU;
                                          1 - ERA_GLONASS */
    uint16_t era_glonass_redial;    /**<  The timer of era glonass redial times.*/
    uint16_t auto_answer;           /**<  The timer of AUTO ANSWER times.*/
    uint8_t ecall_use_sim_num;      /**<  0 -- use input number; 1 -- use SIM SDN/FDN number.*/
    uint16_t T1_timer;              /**<  The timer of IVS cancel eCall is allowed. T1*/
    uint16_t redial_duration;       /**<  The timer of IVS redial duration. ERA_GLONASS standard*/
} ql_voice_ecall_config_t;

/** eCall config items */
typedef enum
{
    QL_VOICE_ECALL_CONFIG_ENABLE                = (1 << 0),     /**< eCall enabled config item. */
    QL_VOICE_ECALL_CONFIG_VOICE_MUTE            = (1 << 1),     /**< eCall voice mute config item. */
    QL_VOICE_ECALL_CONFIG_MODE                  = (1 << 2),     /**< eCall mode config item. */
    QL_VOICE_ECALL_CONFIG_REPORT_EVENT          = (1 << 3),     /**< eCall event report enable config item. */
    QL_VOICE_ECALL_CONFIG_START_TIMER           = (1 << 4),     /**< eCall T5 timer config item. */
    QL_VOICE_ECALL_CONFIG_HACK_TIMER            = (1 << 5),     /**< eCall T6 timer config item. */
    QL_VOICE_ECALL_CONFIG_TRANS_TIMER           = (1 << 6),     /**< eCall T7 timer config item. */
    QL_VOICE_ECALL_CONFIG_FAIL_REDIAL           = (1 << 7),     /**< eCall fail redial config item. */
    QL_VOICE_ECALL_CONFIG_DROP_REDIAL           = (1 << 8),     /**< eCall drop redial config item. */
    QL_VOICE_ECALL_CONFIG_CLEARDOWN_TIMER       = (1 << 9),     /**< eCall T2 timer config item. */
    QL_VOICE_ECALL_CONFIG_INIT_TIMER            = (1 << 10),    /**< eCall T3 timer config item. */
    QL_VOICE_ECALL_CONFIG_NAD_REG_TIMER         = (1 << 11),    /**< eCall T9 timer config item. */
    QL_VOICE_ECALL_CONFIG_NAD_DEREG_TIMER       = (1 << 12),    /**< eCall T10 timer config item. */
    QL_VOICE_ECALL_CONFIG_STANDARD              = (1 << 13),    /**< eCall standard config item. */
    QL_VOICE_ECALL_CONFIG_ERA_GLONASS_REDIAL    = (1 << 14),    /**< eCall Era-glonass redial intercval config item. */
    QL_VOICE_ECALL_CONFIG_AUTO_ANSWER           = (1 << 15),    /**< eCall auto answer timer config item. */
    QL_VOICE_ECALL_CONFIG_USE_SIM_NUM           = (1 << 16),    /**< eCall use sim number config item. */
    QL_VOICE_ECALL_CONFIG_T1_TIMER              = (1 << 17),    /**< eCall T1 timer config item. */
    QL_VOICE_ECALL_CONFIG_REDIAL_DURATION_TIMER = (1 << 18),    /**< eCall redial duration timer config item. */
} QL_VOICE_ECALL_CONFIG_E;

/** enum eCall MSD transmission status */
typedef enum
{
    QL_VOICE_ECALL_MSD_TRANS_STATUS_NONE      = -1, /**< NONE */
    QL_VOICE_ECALL_MSD_TRANSMISSION_STATUS_SUCCESS = 0x00, /**<   Success \n   */
    QL_VOICE_ECALL_MSD_TRANSMISSION_STATUS_FAILURE = 0x01, /**<   Generic failure \n   */
    QL_VOICE_ECALL_MSD_TRANSMISSION_STARTED = 0x02, /**<   MSD transmission started \n  */
    QL_VOICE_ECALL_MSD_NACK_OUT_OF_ORDER = 0x03, /**<   Out of order NACK message detected \n  */
    QL_VOICE_ECALL_MSD_ACK_OUT_OF_ORDER = 0x04, /**<   Out of order ACK message detected \n  */
    QL_VOICE_ECALL_MSD_START_RECEIVED = 0x05, /**<   Start received and SYNC is locked \n  */
    QL_VOICE_ECALL_MSD_LL_ACK_RECEIVED = 0x06, /**<   LL ACKS are recieved \n  */
    QL_VOICE_ECALL_MSD_TX_USER_DEFINED_1 = 0x07, /**<   Customer defined MSD Tx status \n   */
    QL_VOICE_ECALL_MSD_TX_USER_DEFINED_2 = 0x08, /**<   Customer defined MSD Tx status \n   */
    QL_VOICE_ECALL_MSD_TX_USER_DEFINED_3 = 0x09, /**<   Customer defined MSD Tx status \n   */
    QL_VOICE_ECALL_MSD_TRANSMISSION_OUTBAND_STARTED = 0x0A, /**<   Outband MSD transmission started \n   */
    QL_VOICE_ECALL_MSD_TRANSMISSION_OUTBAND_SUCCESS = 0x0B, /**<   Outband MSD transmission success \n   */
    QL_VOICE_ECALL_MSD_TRANSMISSION_OUTBAND_FAILURE = 0x0C /**<   Outband MSD transmission failure   */
} QL_VOICE_ECALL_MSD_TRANS_STATUS_E;

/**
 * eCall status callback function
 * @param[in] id call id
 * @param[in] status MSD transmit status
 * @return no value
 */
typedef void (*ql_voice_ecall_status_cb_f)(uint32_t id, QL_VOICE_ECALL_MSD_TRANS_STATUS_E status);
/**
 *  enum eCall fail
 */
typedef enum
{
    QL_VOICE_ECALL_EVENT_FAIL_NONE                          = 0,
    /**< NONE*/
    QL_VOICE_ECALL_EVENT_FAIL_TIMEOUT                       = 1,
    /**<  Wait for START timeout. */
    QL_VOICE_ECALL_EVENT_FAIL_HACK_TIMEOUT                  = 2,
    /**<  Wait for HACK timeout. */
    QL_VOICE_ECALL_EVENT_FAIL_MSD_TRANS_TIMEOUT             = 3,
    /**<  MSD transmission timeout. */
    QL_VOICE_ECALL_EVENT_FAIL_IVS_RESET_TIMEOUT             = 4,
    /**<  IVS reset. */
    QL_VOICE_ECALL_EVENT_FAIL_CLEAR_DOWN_FALLBACK_TIMEOUT   = 5,
    /**<  clear down fallback timeout. */
    QL_VOICE_ECALL_EVENT_FAIL_IVS_INITIATION_TIMEOUT        = 6,
    /**<  IVS initiation timeout. */
} QL_VOICE_ECALL_EVENT_FAIL_E;

/**
 *  enum eCall process
 */
typedef enum
{
    QL_VOICE_ECALL_EVENT_PROCESS_IVS_NONE = 0,
    /**< NONE*/
    QL_VOICE_ECALL_EVENT_PROCESS_IVS_START_RECEIVED_MSD = 1,
    /**<  IVS Link Layer receives START message and starts to send MSD */
    QL_VOICE_ECALL_EVENT_PROCESS_IVS_NACK_RECEIVED = 2,
    /**<  IVS Link Layer receives NACK message. */
    QL_VOICE_ECALL_EVENT_PROCESS_IVS_ACK_RECEIVED = 3,
    /**<  IVS Link Layer receives the first LACK message. */
    QL_VOICE_ECALL_EVENT_PROCESS_IVS_TX_COMPLETED = 4,
    /**<  IVS MSD transmission is complete. */
    QL_VOICE_ECALL_EVENT_PROCESS_IVS_HLACK_RECEIVED = 5,
    /**<  IVS Link Layer receives first HACK message. */
} QL_VOICE_ECALL_EVENT_PROCESS_E;

/**
 *  enum eCall msd update
 */
typedef enum
{
    QL_VOICE_ECALL_EVENT_MSDUPADTE_NONE                     = -1,
    /**< NONE */
    QL_VOICE_ECALL_EVENT_MSDUPDATE_IVS_UPDATING_MSD         = 0,
    /**<  Indicates MSD has been updated. */
    QL_VOICE_ECALL_EVENT_MSDUPDATE_PSAP_REQURE_UPDATE_MSD   = 1,
    /**<  Indicate to update MSD in 5 seconds. */
    QL_VOICE_ECALL_EVENT_MSDUPDATE_IVS_UPDATE_MSD_TIMEOUT   = 2,
    /**<  Indicate timeout of updating MSD and module starts to transfer the old MSD. */
} QL_VOICE_ECALL_EVENT_MSDUPDATE_E;

/**
 * enum eCall establist
 */
typedef enum
{
    QL_VOICE_ECALL_EVENT_ESTABLISH_NONE    = -1,
    /**< NONE */
    QL_VOICE_ECALL_EVENT_ESTABLISH_SUCCESS = 0,
    /**<  Establish eCall successfully. */
    QL_VOICE_ECALL_EVENT_ESTABLISH_FAIL    = 1,
    /**<  Establish eCall fails. */
} QL_VOICE_ECALL_EVENT_ESTABLISH_E;

/** eCall originate fail redial information structure. */
typedef struct
{
    uint8_t ori_remainder_times;
    /**< Originate fail remainder times*/

    uint16_t time;
    /**< The minimum time duration between the previous call attempt */
} ql_voice_ecall_event_originate_fail_redial_t;  /* Type */

/** eCall drop fail redial information structure. */
typedef struct
{
    uint8_t drop_remainder_times;
    /**< Dorp remainder times*/

    uint16_t time;
    /**< The minimum time duration between the previous call attempt */
} ql_voice_ecall_event_drop_redial_t;  /* Type */

/**
 * enum ecall state
 */
typedef enum
{
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_NONE                          = -1,
    /**< NONE*/
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_START_RECV_SYN                = 0,
    /**<  eCall start recevied synchronization. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_T9_TIMEOUT                    = 1,
    /**<  eCall T9 TIMEOUT. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_T10_TIMEOUT                   = 2,
    /**<  eCall T10 TIMEOUT. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_IVS_ALACK_RECEIVED            = 3,
    /**<  IVS receives ALACK message. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_IVS_LLACK_RECEIVED            = 4,
    /**<  IVS receives LLACK message. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_STOPPED                       = 5,
    /**<  IVS receives STOPPED message. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_ANSWERING_INCOMING_PSAP_ECALL = 6,
    /**<  IVS AUTO ANSWERING INCOMING PSAP ECALL. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_CLEARDOWN_RECEIVED            = 7,
    /**<  IVS receives CLEARDOWN message. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_CALLBACK_TIMEOUT              = 8,
    /**<  eCall CALLBACK TIMEOUT. */
    QL_VOICE_ECALL_EVENT_IVS_NORMAL_CLEARING                        = 9,
    /**<  eCall IVS NORAML CLEATING. */
    QL_VOICE_ECALL_EVENT_IVS_ABNORMAL_CLEARING                      = 10,
    /**<  eCall IVS ABNORAML CLEATING. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_T1_TIMEOUT                    = 11,
    /**<  eCall T1 TIMEOUT. */
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_REDIAL_DURATION_TIMEOUT       = 12,
    /**<  eCall redial duration TIMEOUT. */
} QL_VOICE_ECALL_EVENT_EXTEND_STATE_E;

/** eCall event information structure. */
typedef struct
{
    QL_VOICE_ECALL_EVENT_FAIL_E fail;
    /**< Event of eCall Failed.*/
    QL_VOICE_ECALL_EVENT_PROCESS_E process;
    /**< Event of eCall process.*/
    QL_VOICE_ECALL_EVENT_MSDUPDATE_E msdupdate;
    /**< Event of ecall msd update.*/
    QL_VOICE_ECALL_EVENT_ESTABLISH_E establish;
    /**< Event of eCall establish.*/
    uint16_t hack_code;
    /**< Event of eCall hack code.*/
    ql_voice_ecall_event_originate_fail_redial_t ori_redial;
    /**< Event of originate fail and redial.*/
    ql_voice_ecall_event_drop_redial_t drop_redial;
    /**< Event of drop and redial.*/
    QL_VOICE_ECALL_EVENT_EXTEND_STATE_E extend_state;
    /**< Event of eCall other state.*/
} ql_voice_ecall_event_t;

/**
 * eCall event callback function
 * @param[out] *p_event: eCall event
 * @return no value
 */
typedef void (*ql_voice_ecall_event_cb_f)(ql_voice_ecall_event_t *p_event);

/**
 * voice service callback function
 * @param[in] error: Error number. See QL type.h for details.
 * @return no value
 */
typedef void (*ql_voice_service_error_cb_f)(int error);

/**
 * eCall status callback function
 * @param[in] sim_id sim card id.
 * @param[in] id call id
 * @param[in] status MSD transmit status
 * @return no value
 */
typedef void (*ql_ms_voice_ecall_status_cb_f)(int sim_id, uint32_t id,
                                              QL_VOICE_ECALL_MSD_TRANS_STATUS_E status);

/**
 * eCall event callback function
 * @param[in] sim_id sim card id.
 * @param[out] *p_event: eCall event
 * @return no value
 */
typedef void (*ql_ms_voice_ecall_event_cb_f)(int sim_id, ql_voice_ecall_event_t *p_event);

/**
 * voice service callback function
 * @param[in] sim_id sim card id.
 * @param[in] error: Error number. See QL type.h for details.
 * @return no value
 */
typedef void (*ql_ms_voice_call_cb_f)(int sim_id, ql_voice_record_array_t *p_arr);

/**
 * voice dtmf event callback function
 * @param[in] sim_id sim card id.
 * @param[in] id voice call id
 * @param[in] event dtmf event
 * @param[in] digits dtmf digits
 * @param[in] len dtmf digits length
 * @return no value
 */
typedef void (*ql_ms_voice_dtmf_cb_f)(int sim_id, uint32_t id, QL_VOICE_DTMF_EVENT_E event,
                                      char *digits, int len); /**< len: length of digits */

/**
 * voice ussd callback function
 * @param[in] id voice call id
 * @return no value
 */
typedef void (*ql_ms_voice_ussd_cb_f)(int sim_id,ql_voice_ussd_event_t *p_event);

#ifdef __cplusplus
}
#endif

#endif
