#ifndef __MIPC_MSG_SS_CONST_H__
#define __MIPC_MSG_SS_CONST_H__

enum MIPC_SS_MSG_enum {
    MIPC_SS_MSG_NONE = 0,
    /* the request command of send USSD */
    MIPC_SS_SEND_USSD_REQ                                   = 1793,
    MIPC_SS_SEND_USSD_CNF                                   = 1794,

    /* the request command of cancel USSD */
    MIPC_SS_CANCEL_USSD_REQ                                 = 1795,
    MIPC_SS_CANCEL_USSD_CNF                                 = 1796,

    /* the request command to set CLIR */
    MIPC_SS_SET_CLIR_REQ                                    = 1797,
    MIPC_SS_SET_CLIR_CNF                                    = 1798,

    /* the request command to get CLIR */
    MIPC_SS_GET_CLIR_REQ                                    = 1799,
    MIPC_SS_GET_CLIR_CNF                                    = 1800,

    /* the request command to set call waiting */
    MIPC_SS_SET_CALL_WAITING_REQ                            = 1801,
    MIPC_SS_SET_CALL_WAITING_CNF                            = 1802,

    /* the request to query call waiting */
    MIPC_SS_QUERY_CALL_WAITING_REQ                          = 1803,
    MIPC_SS_QUERY_CALL_WAITING_CNF                          = 1804,

    /* the request command to set call forward */
    MIPC_SS_SET_CALL_FORWARD_REQ                            = 1805,
    MIPC_SS_SET_CALL_FORWARD_CNF                            = 1806,

    /* the request command to query call forward */
    MIPC_SS_QUERY_CALL_FORWARD_REQ                          = 1807,
    MIPC_SS_QUERY_CALL_FORWARD_CNF                          = 1808,

    /* the request command to lock/unlock for call barring */
    MIPC_SS_SET_CALL_BARRING_REQ                            = 1809,
    MIPC_SS_SET_CALL_BARRING_CNF                            = 1810,

    /* the request command to query call barring */
    MIPC_SS_QUERY_CALL_BARRING_REQ                          = 1811,
    MIPC_SS_QUERY_CALL_BARRING_CNF                          = 1812,

    /* sets a new password for the facility lock function defined by Facility Lock +CLCK */
    MIPC_SS_CHANGE_BARRING_PASSWORD_REQ                     = 1813,
    MIPC_SS_CHANGE_BARRING_PASSWORD_CNF                     = 1814,

    /* enables/disables the presentation of notification result codes from TA to TE */
    MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ                   = 1815,
    MIPC_SS_SET_SUPP_SVC_NOTIFICATION_CNF                   = 1816,

    /* query calling line identification presentation */
    MIPC_SS_QUERY_CLIP_REQ                                  = 1817,
    MIPC_SS_QUERY_CLIP_CNF                                  = 1818,

    /* set calling line identification at the TE enabled or disabled  */
    MIPC_SS_SET_CLIP_REQ                                    = 1819,
    MIPC_SS_SET_CLIP_CNF                                    = 1820,

    /* run Gba Authentication */
    MIPC_SS_RUN_GBA_REQ                                     = 1821,
    MIPC_SS_RUN_GBA_CNF                                     = 1822,

    /* the request command to get COLP */
    MIPC_SS_GET_COLP_REQ                                    = 1823,
    MIPC_SS_GET_COLP_CNF                                    = 1824,

    /* the request command to set COLP */
    MIPC_SS_SET_COLP_REQ                                    = 1825,
    MIPC_SS_SET_COLP_CNF                                    = 1826,

    /* the request command to get COLR */
    MIPC_SS_GET_COLR_REQ                                    = 1827,
    MIPC_SS_GET_COLR_CNF                                    = 1828,

    /* the request command to send Call Name identification Presentation */
    MIPC_SS_SEND_CNAP_REQ                                   = 1829,
    MIPC_SS_SEND_CNAP_CNF                                   = 1830,

    /* the request command to set COLR */
    MIPC_SS_SET_COLR_REQ                                    = 1831,
    MIPC_SS_SET_COLR_CNF                                    = 1832,

    /* the request command to setup XCAP USER AGENT */
    MIPC_SS_SETUP_XCAP_USER_AGENT_REQ                       = 1833,
    MIPC_SS_SETUP_XCAP_USER_AGENT_CNF                       = 1834,

    /* the request command to set XCAP CONFIG */
    MIPC_SS_SET_XCAP_CFG_REQ                                = 1835,
    MIPC_SS_SET_XCAP_CFG_CNF                                = 1836,

    /* USSD received indication */
    MIPC_SS_USSD_IND                                        = 18177,

    /* the unsol supp svc notification ind */
    MIPC_SS_ECMCCSS_IND                                     = 18178,

    /* Call Forward UNSOL ind */
    MIPC_SS_CFU_IND                                         = 18179,

    /* XCAP RCN ind */
    MIPC_SS_XCAP_RCN_IND                                    = 18180,

    /* IMS XUI ind */
    MIPC_SS_IMS_XUI_IND                                     = 18181,


};

    /* MIPC_MSG.SS_SEND_USSD_REQ */
enum mipc_ss_send_ussd_req_tlv_enum {
    mipc_ss_send_ussd_req_tlv_NONE = 0,
    /* data coding scheme */
    /* type = uint32_t */
    MIPC_SS_SEND_USSD_REQ_T_DCS                             = 0x101,
    /* length of ussd payload */
    /* type = uint8_t */
    MIPC_SS_SEND_USSD_REQ_T_PAYLOAD_LEN                     = 0x102,
    /* byte array of ussd payload */
    /* type = byte_array */
    MIPC_SS_SEND_USSD_REQ_T_PAYLOAD                         = 0x8103,
    /* lang of send ussd req; Default value is "en" */
    /* type = string */
    MIPC_SS_SEND_USSD_REQ_T_LANG                            = 0x104,
};

    /* MIPC_MSG.SS_SEND_USSD_CNF */
enum mipc_ss_send_ussd_cnf_tlv_enum {
    mipc_ss_send_ussd_cnf_tlv_NONE = 0,
    /* ussd response */
    /* type = uint8_t, refer to SS_USSD */
    MIPC_SS_SEND_USSD_CNF_T_USSD_RESPONSE                   = 0x100,
    /*  ussd session state */
    /* type = uint8_t, refer to SS_SESSION */
    MIPC_SS_SEND_USSD_CNF_T_USSD_SESSION_STATE              = 0x101,
    /* data coding scheme */
    /* type = uint32_t */
    MIPC_SS_SEND_USSD_CNF_T_DCS                             = 0x102,
    /* length of ussd payload */
    /* type = uint8_t */
    MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_LEN                     = 0x103,
    /* pointer to ussd payload */
    /* type = byte_array */
    MIPC_SS_SEND_USSD_CNF_T_PAYLOAD                         = 0x8104,
    /* length of ussd payload external */
    /* type = uint16_t */
    MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_LEN_EX                  = 0x105,
    /* pointer to ussd payload external */
    /* type = byte_array */
    MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_EX                      = 0x8106,
};

    /* MIPC_MSG.SS_CANCEL_USSD_REQ */
enum mipc_ss_cancel_ussd_req_tlv_enum {
    mipc_ss_cancel_ussd_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_CANCEL_USSD_CNF */
enum mipc_ss_cancel_ussd_cnf_tlv_enum {
    mipc_ss_cancel_ussd_cnf_tlv_NONE = 0,
    /* ussd response */
    /* type = uint8_t, refer to SS_USSD */
    MIPC_SS_CANCEL_USSD_CNF_T_USSD_RESPONSE                 = 0x100,
    /* ussd session state */
    /* type = uint8_t, refer to SS_SESSION */
    MIPC_SS_CANCEL_USSD_CNF_T_USSD_SESSION_STATE            = 0x101,
    /* data coding scheme */
    /* type = uint32_t */
    MIPC_SS_CANCEL_USSD_CNF_T_DCS                           = 0x102,
    /* payload length */
    /* type = uint8_t */
    MIPC_SS_CANCEL_USSD_CNF_T_PAYLOAD_LEN                   = 0x103,
    /* pointer to payload */
    /* type = byte_array */
    MIPC_SS_CANCEL_USSD_CNF_T_PAYLOAD                       = 0x8104,
};

    /* MIPC_MSG.SS_SET_CLIR_REQ */
enum mipc_ss_set_clir_req_tlv_enum {
    mipc_ss_set_clir_req_tlv_NONE = 0,
    /* the value to set CLIR */
    /* type = uint8_t */
    MIPC_SS_SET_CLIR_REQ_T_N_VALUE                          = 0x100,
};

    /* MIPC_MSG.SS_SET_CLIR_CNF */
enum mipc_ss_set_clir_cnf_tlv_enum {
    mipc_ss_set_clir_cnf_tlv_NONE = 0,
    /* err message of set CLIR */
    /* type = string */
    MIPC_SS_SET_CLIR_CNF_T_ERRMESSAGE                       = 0x100,
};

    /* MIPC_MSG.SS_GET_CLIR_REQ */
enum mipc_ss_get_clir_req_tlv_enum {
    mipc_ss_get_clir_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_GET_CLIR_CNF */
enum mipc_ss_get_clir_cnf_tlv_enum {
    mipc_ss_get_clir_cnf_tlv_NONE = 0,
    /* n value of CLIR */
    /* type = uint8_t */
    MIPC_SS_GET_CLIR_CNF_T_CLIR_N                           = 0x100,
    /* m value of CLIR */
    /* type = uint8_t */
    MIPC_SS_GET_CLIR_CNF_T_CLIR_M                           = 0x101,
    /* err message of get CLIR */
    /* type = string */
    MIPC_SS_GET_CLIR_CNF_T_ERRMESSAGE                       = 0x102,
};

    /* MIPC_MSG.SS_SET_CALL_WAITING_REQ */
enum mipc_ss_set_call_waiting_req_tlv_enum {
    mipc_ss_set_call_waiting_req_tlv_NONE = 0,
    /* to enable or disable call waiting */
    /* type = uint8_t, refer to SS_CALL_WAITING */
    MIPC_SS_SET_CALL_WAITING_REQ_T_CW_ENABLE_DISABLE        = 0x100,
    /* service class; Default value is 0 */
    /* type = uint16_t, refer to SS_SERVICE_CLASS */
    MIPC_SS_SET_CALL_WAITING_REQ_T_SERVICE_CLASS            = 0x101,
};

    /* MIPC_MSG.SS_SET_CALL_WAITING_CNF */
enum mipc_ss_set_call_waiting_cnf_tlv_enum {
    mipc_ss_set_call_waiting_cnf_tlv_NONE = 0,
    /* err message of Set Call Waiting */
    /* type = string */
    MIPC_SS_SET_CALL_WAITING_CNF_T_ERRMESSAGE               = 0x100,
};

    /* MIPC_MSG.SS_QUERY_CALL_WAITING_REQ */
enum mipc_ss_query_call_waiting_req_tlv_enum {
    mipc_ss_query_call_waiting_req_tlv_NONE = 0,
    /* service class */
    /* type = uint16_t, refer to SS_SERVICE_CLASS */
    MIPC_SS_QUERY_CALL_WAITING_REQ_T_SERVICE_CLASS          = 0x100,
};

    /* MIPC_MSG.SS_QUERY_CALL_WAITING_CNF */
enum mipc_ss_query_call_waiting_cnf_tlv_enum {
    mipc_ss_query_call_waiting_cnf_tlv_NONE = 0,
    /* active or not active of call waiting */
    /* type = uint8_t, refer to SS_ACTIVE_STATUS */
    MIPC_SS_QUERY_CALL_WAITING_CNF_T_CALL_WAITING_STATUS    = 0x100,
    /* service class */
    /* type = uint16_t, refer to SS_SERVICE_CLASS */
    MIPC_SS_QUERY_CALL_WAITING_CNF_T_SERVICE_CLASS          = 0x101,
    /* number of call waiting */
    /* type = uint8_t */
    MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_COUNT               = 0x102,
    /* list of call waiting */
    /* type = struct_array, refer to ss_call_waiting */
    MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_LIST                = 0x103,
    /* err message of Query Call Waiting */
    /* type = string */
    MIPC_SS_QUERY_CALL_WAITING_CNF_T_ERRMESSAGE             = 0x104,
    /* list of call waiting */
    /* type = struct, refer to ss_call_waiting */
    MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_TLV_ARRAY           = 0x105,
};

    /* MIPC_MSG.SS_SET_CALL_FORWARD_REQ */
enum mipc_ss_set_call_forward_req_tlv_enum {
    mipc_ss_set_call_forward_req_tlv_NONE = 0,
    /* ss operation type */
    /* type = uint8_t, refer to SS_SET_CALL_FORWARD_OPERATION_CODE */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_SS_OPERATION             = 0x100,
    /* call forward reason */
    /* type = uint8_t, refer to SS_CALL_FORWARD_REASON */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON      = 0x101,
    /* call forward dial number; Default value is null character */
    /* type = string */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_DIAL_NUMBER              = 0x8102,
    /* service class; Default value is 0 */
    /* type = uint16_t, refer to SS_SERVICE_CLASS */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_SERVICE_CLASS            = 0x103,
    /* type of address; Default value is 0 */
    /* type = uint16_t */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_TOA                      = 0x104,
    /* call forward timer no reply; Default value is 0 */
    /* type = uint8_t */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_TIMER_SECONDS            = 0x105,
    /* time slot begin of call forward; Default value is null character */
    /* type = string */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_TIME_SLOT_BEGIN          = 0x106,
    /* time slot end of call forward; Default value is null character */
    /* type = string */
    MIPC_SS_SET_CALL_FORWARD_REQ_T_TIME_SLOT_END            = 0x107,
};

    /* MIPC_MSG.SS_SET_CALL_FORWARD_CNF */
enum mipc_ss_set_call_forward_cnf_tlv_enum {
    mipc_ss_set_call_forward_cnf_tlv_NONE = 0,
    /* err message of Set Call Forward */
    /* type = string */
    MIPC_SS_SET_CALL_FORWARD_CNF_T_ERRMESSAGE               = 0x100,
};

    /* MIPC_MSG.SS_QUERY_CALL_FORWARD_REQ */
enum mipc_ss_query_call_forward_req_tlv_enum {
    mipc_ss_query_call_forward_req_tlv_NONE = 0,
    /* query service class; Default value is 0 */
    /* type = uint16_t, refer to SS_SERVICE_CLASS */
    MIPC_SS_QUERY_CALL_FORWARD_REQ_T_SERVICE_CLASS          = 0x100,
    /* call forward reason to query */
    /* type = uint8_t, refer to SS_CALL_FORWARD_REASON */
    MIPC_SS_QUERY_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON    = 0x101,
};

    /* MIPC_MSG.SS_QUERY_CALL_FORWARD_CNF */
enum mipc_ss_query_call_forward_cnf_tlv_enum {
    mipc_ss_query_call_forward_cnf_tlv_NONE = 0,
    /* number of call forward */
    /* type = uint8_t */
    MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_COUNT     = 0x100,
    /* list of call forward, in cf cnf struct timer range is 1 - 30, and 0 means invalid */
    /* type = struct, refer to ss_call_forward */
    MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_LIST      = 0x8101,
    MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_TLV_ARRAY = 0x8101,
    /* err message of Query Call Forward */
    /* type = string */
    MIPC_SS_QUERY_CALL_FORWARD_CNF_T_ERRMESSAGE             = 0x102,
};

    /* MIPC_MSG.SS_SET_CALL_BARRING_REQ */
enum mipc_ss_set_call_barring_req_tlv_enum {
    mipc_ss_set_call_barring_req_tlv_NONE = 0,
    /* Lock,unlock call barring */
    /* type = uint8_t, refer to SS_CALL_BARRING_LOCK */
    MIPC_SS_SET_CALL_BARRING_REQ_T_LOCK                     = 0x100,
    /* facility for call barring */
    /* type = uint8_t, refer to SS_CALL_BARRING_FAC */
    MIPC_SS_SET_CALL_BARRING_REQ_T_FACILITY                 = 0x101,
    /* the call barring password */
    /* type = string */
    MIPC_SS_SET_CALL_BARRING_REQ_T_PASSWORD                 = 0x8102,
    /* set call barring service class; Default value is 0 */
    /* type = uint16_t, refer to SS_SERVICE_CLASS */
    MIPC_SS_SET_CALL_BARRING_REQ_T_SERVICE_CLASS            = 0x103,
    /* Count of dial number for call barring,max is 10 */
    /* type = uint8_t */
    MIPC_SS_SET_CALL_BARRING_REQ_T_DIAL_NUMBER_COUNT        = 0x104,
    /* List of dial number for call barring */
    /* type = string */
    MIPC_SS_SET_CALL_BARRING_REQ_T_DIAL_NUMBER_LIST         = 0x8105,
};

    /* MIPC_MSG.SS_SET_CALL_BARRING_CNF */
enum mipc_ss_set_call_barring_cnf_tlv_enum {
    mipc_ss_set_call_barring_cnf_tlv_NONE = 0,
    /* err message of Set Call Barring */
    /* type = string */
    MIPC_SS_SET_CALL_BARRING_CNF_T_ERRMESSAGE               = 0x100,
};

    /* MIPC_MSG.SS_QUERY_CALL_BARRING_REQ */
enum mipc_ss_query_call_barring_req_tlv_enum {
    mipc_ss_query_call_barring_req_tlv_NONE = 0,
    /* facility for query call barring */
    /* type = uint8_t, refer to SS_CALL_BARRING_FAC */
    MIPC_SS_QUERY_CALL_BARRING_REQ_T_FACILITY               = 0x100,
    /* query service class; Default value is 0 */
    /* type = uint16_t, refer to SS_SERVICE_CLASS */
    MIPC_SS_QUERY_CALL_BARRING_REQ_T_SERVICE_CLASS          = 0x101,
};

    /* MIPC_MSG.SS_QUERY_CALL_BARRING_CNF */
enum mipc_ss_query_call_barring_cnf_tlv_enum {
    mipc_ss_query_call_barring_cnf_tlv_NONE = 0,
    /* call barring status, active, not active, or service class indication */
    /* type = uint16_t */
    MIPC_SS_QUERY_CALL_BARRING_CNF_T_CALL_BARRING_STATUS    = 0x100,
    /* err message of Query Call Barring */
    /* type = string */
    MIPC_SS_QUERY_CALL_BARRING_CNF_T_ERRMESSAGE             = 0x101,
    /* Count of dial number for call barring,max is 10 */
    /* type = uint8_t */
    MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_COUNT      = 0x102,
    /* List of dial number for call barring */
    /* type = struct, refer to ss_cb_dialnumber */
    MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_LIST       = 0x8103,
    MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_TLV_ARRAY  = 0x8103,
};

    /* MIPC_MSG.SS_CHANGE_BARRING_PASSWORD_REQ */
enum mipc_ss_change_barring_password_req_tlv_enum {
    mipc_ss_change_barring_password_req_tlv_NONE = 0,
    /* facility for call barring */
    /* type = uint8_t, refer to SS_CALL_BARRING_FAC */
    MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_FACILITY          = 0x8100,
    /* old password */
    /* type = string */
    MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_OLD_PWD           = 0x8101,
    /* new password */
    /* type = string */
    MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_NEW_PWD           = 0x8102,
    /* new password confirmed; Default value is new password */
    /* type = string */
    MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_NEW_PWD_CONFIRM   = 0x8103,
};

    /* MIPC_MSG.SS_CHANGE_BARRING_PASSWORD_CNF */
enum mipc_ss_change_barring_password_cnf_tlv_enum {
    mipc_ss_change_barring_password_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_SET_SUPP_SVC_NOTIFICATION_REQ */
enum mipc_ss_set_supp_svc_notification_req_tlv_enum {
    mipc_ss_set_supp_svc_notification_req_tlv_NONE = 0,
    /* set the +CSSI intermediate result code presentation status to TE. 0 disable, 1 enable; Default value is 0 */
    /* type = uint8_t */
    MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ_T_STATUS_I        = 0x100,
    /* set the +CSSU unsolicited result code presentation status to TE. 0 disable, 1 enable; Default value is 0 */
    /* type = uint8_t */
    MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ_T_STATUS_U        = 0x101,
};

    /* MIPC_MSG.SS_SET_SUPP_SVC_NOTIFICATION_CNF */
enum mipc_ss_set_supp_svc_notification_cnf_tlv_enum {
    mipc_ss_set_supp_svc_notification_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_QUERY_CLIP_REQ */
enum mipc_ss_query_clip_req_tlv_enum {
    mipc_ss_query_clip_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_QUERY_CLIP_CNF */
enum mipc_ss_query_clip_cnf_tlv_enum {
    mipc_ss_query_clip_cnf_tlv_NONE = 0,
    /* shows the result code presentation status to the TE. 0 disable, 1 enable */
    /* type = uint8_t */
    MIPC_SS_QUERY_CLIP_CNF_T_CODE_STATUS                    = 0x100,
    /* shows the subscriber CLIP service status in the netowrk */
    /* type = uint8_t */
    MIPC_SS_QUERY_CLIP_CNF_T_NW_STATUS                      = 0x101,
    /* err message of query CLIP */
    /* type = string */
    MIPC_SS_QUERY_CLIP_CNF_T_ERRMESSAGE                     = 0x102,
};

    /* MIPC_MSG.SS_SET_CLIP_REQ */
enum mipc_ss_set_clip_req_tlv_enum {
    mipc_ss_set_clip_req_tlv_NONE = 0,
    /* integer type. 0 disable, 1 enable; Default value is 0 */
    /* type = uint8_t */
    MIPC_SS_SET_CLIP_REQ_T_STATUS                           = 0x100,
};

    /* MIPC_MSG.SS_SET_CLIP_CNF */
enum mipc_ss_set_clip_cnf_tlv_enum {
    mipc_ss_set_clip_cnf_tlv_NONE = 0,
    /* err message of set CLIP */
    /* type = string */
    MIPC_SS_SET_CLIP_CNF_T_ERRMESSAGE                       = 0x100,
};

    /* MIPC_MSG.SS_RUN_GBA_REQ */
enum mipc_ss_run_gba_req_tlv_enum {
    mipc_ss_run_gba_req_tlv_NONE = 0,
    /* a string to indicate GBA key */
    /* type = string */
    MIPC_SS_RUN_GBA_REQ_T_NAF_FQDN                          = 0x100,
    /* a string for GBA protocol */
    /* type = string */
    MIPC_SS_RUN_GBA_REQ_T_NAF_SECURE_PROTOCOL_ID            = 0x101,
    /* to indicate force run GBA or using cache. 0: no need. 1: force run */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SS_RUN_GBA_REQ_T_FORCE_RUN                         = 0x102,
    /* to indicate network access */
    /* type = uint32_t */
    MIPC_SS_RUN_GBA_REQ_T_NET_ID                            = 0x103,
};

    /* MIPC_MSG.SS_RUN_GBA_CNF */
enum mipc_ss_run_gba_cnf_tlv_enum {
    mipc_ss_run_gba_cnf_tlv_NONE = 0,
    /* GBA key */
    /* type = string */
    MIPC_SS_RUN_GBA_CNF_T_KEY                               = 0x100,
    /* key length */
    /* type = uint8_t */
    MIPC_SS_RUN_GBA_CNF_T_KEY_LEN                           = 0x101,
    /* bit id */
    /* type = string */
    MIPC_SS_RUN_GBA_CNF_T_BIT_ID                            = 0x102,
    /* the lifetime of key */
    /* type = string */
    MIPC_SS_RUN_GBA_CNF_T_KEY_LIFETIME                      = 0x103,
};

    /* MIPC_MSG.SS_GET_COLP_REQ */
enum mipc_ss_get_colp_req_tlv_enum {
    mipc_ss_get_colp_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_GET_COLP_CNF */
enum mipc_ss_get_colp_cnf_tlv_enum {
    mipc_ss_get_colp_cnf_tlv_NONE = 0,
    /* disable or enable of COLP */
    /* type = uint8_t */
    MIPC_SS_GET_COLP_CNF_T_COLP_N                           = 0x100,
    /* service status in the network for COLP */
    /* type = uint8_t */
    MIPC_SS_GET_COLP_CNF_T_COLP_M                           = 0x101,
    /* err message of get COLP */
    /* type = string */
    MIPC_SS_GET_COLP_CNF_T_ERRMESSAGE                       = 0x102,
};

    /* MIPC_MSG.SS_SET_COLP_REQ */
enum mipc_ss_set_colp_req_tlv_enum {
    mipc_ss_set_colp_req_tlv_NONE = 0,
    /* disable or enable to set COLP */
    /* type = uint8_t */
    MIPC_SS_SET_COLP_REQ_T_N_VALUE                          = 0x100,
};

    /* MIPC_MSG.SS_SET_COLP_CNF */
enum mipc_ss_set_colp_cnf_tlv_enum {
    mipc_ss_set_colp_cnf_tlv_NONE = 0,
    /* err message of set COLP */
    /* type = string */
    MIPC_SS_SET_COLP_CNF_T_ERRMESSAGE                       = 0x100,
};

    /* MIPC_MSG.SS_GET_COLR_REQ */
enum mipc_ss_get_colr_req_tlv_enum {
    mipc_ss_get_colr_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_GET_COLR_CNF */
enum mipc_ss_get_colr_cnf_tlv_enum {
    mipc_ss_get_colr_cnf_tlv_NONE = 0,
    /* status value of COLR */
    /* type = uint8_t */
    MIPC_SS_GET_COLR_CNF_T_STATUS                           = 0x100,
    /* err message of get COLR */
    /* type = string */
    MIPC_SS_GET_COLR_CNF_T_ERRMESSAGE                       = 0x101,
};

    /* MIPC_MSG.SS_SEND_CNAP_REQ */
enum mipc_ss_send_cnap_req_tlv_enum {
    mipc_ss_send_cnap_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_SEND_CNAP_CNF */
enum mipc_ss_send_cnap_cnf_tlv_enum {
    mipc_ss_send_cnap_cnf_tlv_NONE = 0,
    /* enable or disable of CNAP status to the TE */
    /* type = uint8_t, refer to SS_CNAP_STATE */
    MIPC_SS_SEND_CNAP_CNF_T_CNAP_N                          = 0x100,
    /* CNAP service status in the network */
    /* type = uint8_t */
    MIPC_SS_SEND_CNAP_CNF_T_CNAP_M                          = 0x101,
};

    /* MIPC_MSG.SS_SET_COLR_REQ */
enum mipc_ss_set_colr_req_tlv_enum {
    mipc_ss_set_colr_req_tlv_NONE = 0,
    /* the value to set COLR */
    /* type = uint8_t */
    MIPC_SS_SET_COLR_REQ_T_N_VALUE                          = 0x100,
};

    /* MIPC_MSG.SS_SET_COLR_CNF */
enum mipc_ss_set_colr_cnf_tlv_enum {
    mipc_ss_set_colr_cnf_tlv_NONE = 0,
    /* err message of get COLP */
    /* type = string */
    MIPC_SS_SET_COLR_CNF_T_ERRMESSAGE                       = 0x100,
};

    /* MIPC_MSG.SS_SETUP_XCAP_USER_AGENT_REQ */
enum mipc_ss_setup_xcap_user_agent_req_tlv_enum {
    mipc_ss_setup_xcap_user_agent_req_tlv_NONE = 0,
    /* the string to setup XCAP USER AGENT */
    /* type = string */
    MIPC_SS_SETUP_XCAP_USER_AGENT_REQ_T_STR                 = 0x100,
};

    /* MIPC_MSG.SS_SETUP_XCAP_USER_AGENT_CNF */
enum mipc_ss_setup_xcap_user_agent_cnf_tlv_enum {
    mipc_ss_setup_xcap_user_agent_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_SET_XCAP_CFG_REQ */
enum mipc_ss_set_xcap_cfg_req_tlv_enum {
    mipc_ss_set_xcap_cfg_req_tlv_NONE = 0,
    /* the config name of XCAP */
    /* type = string */
    MIPC_SS_SET_XCAP_CFG_REQ_T_CFG_NAME                     = 0x100,
    /* the config value of XCAP */
    /* type = string */
    MIPC_SS_SET_XCAP_CFG_REQ_T_VALUE                        = 0x101,
};

    /* MIPC_MSG.SS_SET_XCAP_CFG_CNF */
enum mipc_ss_set_xcap_cfg_cnf_tlv_enum {
    mipc_ss_set_xcap_cfg_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SS_USSD_IND */
enum mipc_ss_ussd_ind_tlv_enum {
    mipc_ss_ussd_ind_tlv_NONE = 0,
    /* ussd response */
    /* type = uint8_t, refer to SS_USSD */
    MIPC_SS_USSD_IND_T_USSD_RESPONSE                        = 0x100,
    /* ussd session state */
    /* type = uint8_t, refer to SS_SESSION */
    MIPC_SS_USSD_IND_T_USSD_SESSION_STATE                   = 0x101,
    /* data coding scheme */
    /* type = uint32_t */
    MIPC_SS_USSD_IND_T_DCS                                  = 0x102,
    /* length of payload */
    /* type = uint8_t */
    MIPC_SS_USSD_IND_T_PAYLOAD_LEN                          = 0x103,
    /* payload pointer */
    /* type = byte_array */
    MIPC_SS_USSD_IND_T_PAYLOAD                              = 0x104,
    /* length of ussd payload external */
    /* type = uint16_t */
    MIPC_SS_USSD_IND_T_PAYLOAD_LEN_EX                       = 0x105,
    /* pointer to ussd payload external */
    /* type = byte_array */
    MIPC_SS_USSD_IND_T_PAYLOAD_EX                           = 0x8106,
};

    /* MIPC_MSG.SS_ECMCCSS_IND */
enum mipc_ss_ecmccss_ind_tlv_enum {
    mipc_ss_ecmccss_ind_tlv_NONE = 0,
    /* the id of current call */
    /* type = uint32_t */
    MIPC_SS_ECMCCSS_IND_T_CALL_ID                           = 0x100,
    /* urc type */
    /* type = uint32_t */
    MIPC_SS_ECMCCSS_IND_T_SERVICE                           = 0x101,
    /* the raw string of ecmccss */
    /* type = string */
    MIPC_SS_ECMCCSS_IND_T_RAW_STRING                        = 0x102,
};

    /* MIPC_MSG.SS_CFU_IND */
enum mipc_ss_cfu_ind_tlv_enum {
    mipc_ss_cfu_ind_tlv_NONE = 0,
    /* the status of cfu icon */
    /* type = uint8_t, refer to SS_ECFU_ICON_STATUS */
    MIPC_SS_CFU_IND_T_STATUS                                = 0x100,
    /* the line info */
    /* type = uint8_t */
    MIPC_SS_CFU_IND_T_LINE                                  = 0x101,
};

    /* MIPC_MSG.SS_XCAP_RCN_IND */
enum mipc_ss_xcap_rcn_ind_tlv_enum {
    mipc_ss_xcap_rcn_ind_tlv_NONE = 0,
    /* the xcap response code */
    /* type = uint32_t */
    MIPC_SS_XCAP_RCN_IND_T_CODE                             = 0x100,
    /* change code to response */
    /* type = uint8_t */
    MIPC_SS_XCAP_RCN_IND_T_RESPONSE                         = 0x101,
};

    /* MIPC_MSG.SS_IMS_XUI_IND */
enum mipc_ss_ims_xui_ind_tlv_enum {
    mipc_ss_ims_xui_ind_tlv_NONE = 0,
    /* the account number */
    /* type = uint8_t */
    MIPC_SS_IMS_XUI_IND_T_ACCOUNT_ID                        = 0x100,
    /* broadcast flag */
    /* type = uint8_t */
    MIPC_SS_IMS_XUI_IND_T_BROADCAST_FLAG                    = 0x101,
    /* Xui information */
    /* type = string */
    MIPC_SS_IMS_XUI_IND_T_XUI_INFO                          = 0x8102,
};




#endif /* __MIPC_MSG_SS_CONST_H__ */
