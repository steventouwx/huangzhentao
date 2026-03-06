#ifndef __MIPC_MSG_INTERNAL_CONST_H__
#define __MIPC_MSG_INTERNAL_CONST_H__

enum MIPC_INTERNAL_MSG_enum {
    MIPC_INTERNAL_MSG_NONE = 0,
    /* MIPC_INTERNAL_OPEN_REQ is sent by the AP to inform modem that AP will start to interacting with the modem. Modem will response MIPC_INTERNAL_OPEN_CNF after MIPC channel is successfully opened, which means that AP can control the full modem functions by MIPC commands. */
    MIPC_INTERNAL_OPEN_REQ                                  = 769,
    MIPC_INTERNAL_OPEN_CNF                                  = 770,

    /*
      After AP finishes running tasks by MIPC commands, AP needs to send MIPC_INTERNAL_CLOSE_REQ to modem to terminate the session between AP and modem.
      When the section is successfully terminated, modem will send out MIPC_INTERNAL_CLOSE_CNF and will not send out any MIPC message until next MIPC_INTERNAL_OPEN_REQ.
    */
    MIPC_INTERNAL_CLOSE_REQ                                 = 771,
    MIPC_INTERNAL_CLOSE_CNF                                 = 772,

    /*  */
    MIPC_INTERNAL_TEST_REQ                                  = 773,
    MIPC_INTERNAL_TEST_CNF                                  = 774,

    /*
      This command is used to inform modem the message ID of corresponding SIM or PS ID unsolicited events and modem will send the result of registration in MIPC_INTERNAL_REGISTER_IND_CNF.
      There are 6 rules of MIPC IND message registration as follows:
          (1) The process is required to register the MIPC IND message again when the MIPC channel is reopened.
          (2) Each SIM ID of MIPC IND message is mutual independence. For example, the process can register the MIPC IND message with SIM1 and SIM2 respectively.
          (3) Each PS ID of MIPC IND message is mutual independence. For example, the process can register the MIPC IND message with PS0 and PS1 respectively.
          (4) Each process in AP is mutual independence, indicating that process A won't be affected by the MIPC IND messages which are not registered by process A.
          (5) One MIPC IND message can be registered by multiple process.
          (6) When the process doesn't intend to receive unsolicited events anymore, it can adopt MIPC_INTERNAL_UNREGISTER_IND_REQ.
      
If the MIPC message ID is not in the range of MIPC IND message, MIPC_RESULT_INVALID_PARAMETERS would be returned in MIPC_T_RESULT.
    */
    MIPC_INTERNAL_REGISTER_IND_REQ                          = 775,
    MIPC_INTERNAL_REGISTER_IND_CNF                          = 776,

    /* When the process does not intend to receive unsolicited events anymore, it can send MIPC_INTERNAL_UNREGISTER_IND_REQ to modem. */
    MIPC_INTERNAL_UNREGISTER_IND_REQ                        = 777,
    MIPC_INTERNAL_UNREGISTER_IND_CNF                        = 778,

    /*
      This command is used to register the corresponding MIPC CMD message to receive MIPC CMD message from modem and modem will send the result of registration in MIPC_INTERNAL_REGISTER_CMD_CNF.
      The 3 rules of MIPC CMD message registration are described as follows:
          (1) The process is required to register the MIPC CMD message again when the MIPC channel is reopened.
          (2) One MIPC CMD message can only be registered by one process, so the value of SIM_PS_ID in MIPC header should be SYS_SIM_PS_ALL(0xFF).
          (3) When the process doesn't intend to receive unsolicited events anymore, it can adopt MIPC_INTERNAL_UNREGISTER_CMD_REQ.
      
If the value of SIM_PS_ID in MIPC header is not SYS_SIM_PS_ALL(0xFF) or the MIPC CMD message has been registered, MIPC_RESULT_INVALID_PARAMETERS would be returned in MIPC_T_RESULT.
    */
    MIPC_INTERNAL_REGISTER_CMD_REQ                          = 779,
    MIPC_INTERNAL_REGISTER_CMD_CNF                          = 780,

    /*
      When the process does not intend to receive unsolicited events anymore, it can adopt MIPC_INTERNAL_UNREGISTER_CMD_REQ.
      If the value of SIM_PS_ID in MIPC header is not SYS_SIM_PS_ALL(0xFF), MIPC_RESULT_INVALID_PARAMETERS would be returned in MIPC_T_RESULT.
    */
    MIPC_INTERNAL_UNREGISTER_CMD_REQ                        = 781,
    MIPC_INTERNAL_UNREGISTER_CMD_CNF                        = 782,

    /*  */
    MIPC_INTERNAL_SET_FILTER_REQ                            = 783,
    MIPC_INTERNAL_SET_FILTER_CNF                            = 784,

    /*  */
    MIPC_INTERNAL_RESET_FILTER_REQ                          = 785,
    MIPC_INTERNAL_RESET_FILTER_CNF                          = 786,

    /*  */
    MIPC_INTERNAL_EIF_REQ                                   = 787,
    MIPC_INTERNAL_EIF_CNF                                   = 788,

    /* low power set long standby monitor req */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_REQ              = 789,
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_CNF              = 790,

    /* low power set long standby monitor time req */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_REQ         = 791,
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_CNF         = 792,

    /* low power set long standby monitor req */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ = 793,
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_CNF = 794,

    /* for test only */
    MIPC_INTERNAL_TEST_IND                                  = 17153,

    /* unsolicited network interface control indication */
    MIPC_INTERNAL_EIF_IND                                   = 17154,

    /* PDN handover indication (ex. L->W or W->L) */
    MIPC_INTERNAL_HO_IND                                    = 17155,

    /* This command is used to notify that there is an MIPC pending too long or there are too much MIPC under pending. */
    MIPC_INTERNAL_MIPC_PENDING_IND                          = 17156,

    /*  */
    MIPC_INTERNAL_TEST_CMD                                  = 33537,
    MIPC_INTERNAL_TEST_RSP                                  = 33538,

    /*  */
    MIPC_INTERNAL_EIPPORT_CMD                               = 33539,
    MIPC_INTERNAL_EIPPORT_RSP                               = 33540,

    /*  */
    MIPC_INTERNAL_EIPSPI_CMD                                = 33541,
    MIPC_INTERNAL_EIPSPI_RSP                                = 33542,

    /* free multi ip spi */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD                     = 33543,
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP                     = 33544,

    /* for test only */
    MIPC_INTERNAL_TEST_NTF                                  = 49921,

    /* inject tst string */
    MIPC_INTERNAL_INJECT_TST_NTF                            = 49922,


};

    /* MIPC_MSG.INTERNAL_OPEN_REQ */
enum mipc_internal_open_req_tlv_enum {
    mipc_internal_open_req_tlv_NONE = 0,
    /* The version which is used to negotiated between AP and modem */
    /* type = uint32_t */
    MIPC_INTERNAL_OPEN_REQ_T_VERSION                        = 0x100,
    /* Due to the different process can interact with modem by different communication port, the name is used to identify the process in the log. */
    /* type = string */
    MIPC_INTERNAL_OPEN_REQ_T_CLIENT_NAME                    = 0x101,
    /* It indicates whether host supports the User Sensitive Information Removal feature. If the feature option is on, the sensitive data such as password, phone number etc., will be removed in the logging tool; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_INTERNAL_OPEN_REQ_T_USIR_SUPPORT                   = 0x102,
    /* It indicates whether the port is meta port.; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_INTERNAL_OPEN_REQ_T_IS_META                        = 0x103,
};

    /* MIPC_MSG.INTERNAL_OPEN_CNF */
enum mipc_internal_open_cnf_tlv_enum {
    mipc_internal_open_cnf_tlv_NONE = 0,
    /* The version which is supported in the modem */
    /* type = uint32_t */
    MIPC_INTERNAL_OPEN_CNF_T_VERSION                        = 0x100,
    /* Default timeout value configured by modem */
    /* type = uint32_t */
    MIPC_INTERNAL_OPEN_CNF_T_TIMEOUT                        = 0x101,
};

    /* MIPC_MSG.INTERNAL_CLOSE_REQ */
enum mipc_internal_close_req_tlv_enum {
    mipc_internal_close_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_CLOSE_CNF */
enum mipc_internal_close_cnf_tlv_enum {
    mipc_internal_close_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_TEST_REQ */
enum mipc_internal_test_req_tlv_enum {
    mipc_internal_test_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_TEST_CNF */
enum mipc_internal_test_cnf_tlv_enum {
    mipc_internal_test_cnf_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_TEST_CNF_T_TEST                           = 0x100,
};

    /* MIPC_MSG.INTERNAL_REGISTER_IND_REQ */
enum mipc_internal_register_ind_req_tlv_enum {
    mipc_internal_register_ind_req_tlv_NONE = 0,
    /* MIPC message ID which is required to identify the MIPC IND message of unsolicited events expected to be received by the process.; Default value is 0 */
    /* type = uint16_t */
    MIPC_INTERNAL_REGISTER_IND_REQ_T_MSG_ID                 = 0x100,
    /* enhancement for MSG_ID(could use TLV array format to register couples MIPC_INDs in one MIPC_MSG) */
    /* type = uint16_t */
    MIPC_INTERNAL_REGISTER_IND_REQ_T_MSG_ID_GROUP           = 0x101,
    /* This field is used to unregister all MIPC IND messages in related PS/SIM ID. If TLV UNREG_ALL is present, modem will process this TLV first to reset all previous registration status then process the other TLVs for registration. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_INTERNAL_REGISTER_IND_REQ_T_UNREG_ALL              = 0x102,
};

    /* MIPC_MSG.INTERNAL_REGISTER_IND_CNF */
enum mipc_internal_register_ind_cnf_tlv_enum {
    mipc_internal_register_ind_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_UNREGISTER_IND_REQ */
enum mipc_internal_unregister_ind_req_tlv_enum {
    mipc_internal_unregister_ind_req_tlv_NONE = 0,
    /* MIPC message ID which is required to identify the MIPC IND message of unsolicited events expected not to be received by the process.; Default value is 0 */
    /* type = uint16_t */
    MIPC_INTERNAL_UNREGISTER_IND_REQ_T_MSG_ID               = 0x100,
    /* enhancement for MSG_ID(could use TLV array format to register couples MIPC_INDs in one MIPC_MSG) */
    /* type = uint16_t */
    MIPC_INTERNAL_UNREGISTER_IND_REQ_T_MSG_ID_GROUP         = 0x101,
    /* if user want to unregister all the MIPC_IND in relate PS/SIM; if this tlv is TRUE, MSG_ID/MSG_ID_GROUP would be ignored; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_INTERNAL_UNREGISTER_IND_REQ_T_UNREG_ALL            = 0x102,
};

    /* MIPC_MSG.INTERNAL_UNREGISTER_IND_CNF */
enum mipc_internal_unregister_ind_cnf_tlv_enum {
    mipc_internal_unregister_ind_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_REGISTER_CMD_REQ */
enum mipc_internal_register_cmd_req_tlv_enum {
    mipc_internal_register_cmd_req_tlv_NONE = 0,
    /* MIPC message ID which is required to identify the MIPC CMD message of unsolicited events expected to be received by the process.; Default value is 0 */
    /* type = uint16_t */
    MIPC_INTERNAL_REGISTER_CMD_REQ_T_MSG_ID                 = 0x100,
};

    /* MIPC_MSG.INTERNAL_REGISTER_CMD_CNF */
enum mipc_internal_register_cmd_cnf_tlv_enum {
    mipc_internal_register_cmd_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_UNREGISTER_CMD_REQ */
enum mipc_internal_unregister_cmd_req_tlv_enum {
    mipc_internal_unregister_cmd_req_tlv_NONE = 0,
    /* MIPC message ID which is required to identify the MIPC CMD message of unsolicited events expected not to be received by the process.; Default value is 0 */
    /* type = uint16_t */
    MIPC_INTERNAL_UNREGISTER_CMD_REQ_T_MSG_ID               = 0x100,
};

    /* MIPC_MSG.INTERNAL_UNREGISTER_CMD_CNF */
enum mipc_internal_unregister_cmd_cnf_tlv_enum {
    mipc_internal_unregister_cmd_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_SET_FILTER_REQ */
enum mipc_internal_set_filter_req_tlv_enum {
    mipc_internal_set_filter_req_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to internal_set_filter_req */
    MIPC_INTERNAL_SET_FILTER_REQ_T_STRUCT                   = 0x100,
};

    /* MIPC_MSG.INTERNAL_SET_FILTER_CNF */
enum mipc_internal_set_filter_cnf_tlv_enum {
    mipc_internal_set_filter_cnf_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to internal_set_filter_cnf */
    MIPC_INTERNAL_SET_FILTER_CNF_T_STRUCT                   = 0x100,
};

    /* MIPC_MSG.INTERNAL_RESET_FILTER_REQ */
enum mipc_internal_reset_filter_req_tlv_enum {
    mipc_internal_reset_filter_req_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to internal_reset_filter_req */
    MIPC_INTERNAL_RESET_FILTER_REQ_T_STRUCT                 = 0x100,
};

    /* MIPC_MSG.INTERNAL_RESET_FILTER_CNF */
enum mipc_internal_reset_filter_cnf_tlv_enum {
    mipc_internal_reset_filter_cnf_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to internal_reset_filter_cnf */
    MIPC_INTERNAL_RESET_FILTER_CNF_T_STRUCT                 = 0x100,
};

    /* MIPC_MSG.INTERNAL_EIF_REQ */
enum mipc_internal_eif_req_tlv_enum {
    mipc_internal_eif_req_tlv_NONE = 0,
    /*  */
    /* type = uint32_t */
    MIPC_INTERNAL_EIF_REQ_T_TRANSID                         = 0x100,
    /*  */
    /* type = uint8_t, refer to INTERNAL_EIF_REQ_CMD */
    MIPC_INTERNAL_EIF_REQ_T_CMD                             = 0x101,
    /*  */
    /* type = struct, refer to full_addr */
    MIPC_INTERNAL_EIF_REQ_T_NEW_ADDR                        = 0x8102,
    /*  */
    /* type = struct, refer to full_addr */
    MIPC_INTERNAL_EIF_REQ_T_OLD_ADDR                        = 0x8103,
    /* pdp type for interface up; Default value is 1 */
    /* type = uint8_t, refer to EIF_ADDRESS_TYPE */
    MIPC_INTERNAL_EIF_REQ_T_ADDRESS_TYPE                    = 0x104,
};

    /* MIPC_MSG.INTERNAL_EIF_CNF */
enum mipc_internal_eif_cnf_tlv_enum {
    mipc_internal_eif_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_SET_LONG_STANDBY_MONITOR_REQ */
enum mipc_internal_set_long_standby_monitor_req_tlv_enum {
    mipc_internal_set_long_standby_monitor_req_tlv_NONE = 0,
    /* mode of low power set long standby monitor */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_REQ_T_MODE       = 0x100,
};

    /* MIPC_MSG.INTERNAL_SET_LONG_STANDBY_MONITOR_CNF */
enum mipc_internal_set_long_standby_monitor_cnf_tlv_enum {
    mipc_internal_set_long_standby_monitor_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_REQ */
enum mipc_internal_set_long_standby_monitor_time_req_tlv_enum {
    mipc_internal_set_long_standby_monitor_time_req_tlv_NONE = 0,
    /* minute */
    /* type = uint32_t */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_REQ_T_MINUTE = 0x100,
    /* second */
    /* type = uint8_t */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_REQ_T_SECOND = 0x101,
};

    /* MIPC_MSG.INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_CNF */
enum mipc_internal_set_long_standby_monitor_time_cnf_tlv_enum {
    mipc_internal_set_long_standby_monitor_time_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ */
enum mipc_internal_set_long_standby_monitor_warning_ratio_req_tlv_enum {
    mipc_internal_set_long_standby_monitor_warning_ratio_req_tlv_NONE = 0,
    /* m */
    /* type = uint32_t */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_M = 0x100,
    /* x */
    /* type = uint32_t */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_X = 0x101,
    /* y */
    /* type = uint32_t */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_Y = 0x102,
    /* z */
    /* type = uint32_t */
    MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_Z = 0x103,
};

    /* MIPC_MSG.INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_CNF */
enum mipc_internal_set_long_standby_monitor_warning_ratio_cnf_tlv_enum {
    mipc_internal_set_long_standby_monitor_warning_ratio_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_TEST_IND */
enum mipc_internal_test_ind_tlv_enum {
    mipc_internal_test_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_EIF_IND */
enum mipc_internal_eif_ind_tlv_enum {
    mipc_internal_eif_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_EIF_IND_T_TRANSID                         = 0x100,
    /* type = uint8_t, refer to INTERNAL_EIF_IND_CMD */
    MIPC_INTERNAL_EIF_IND_T_CMD                             = 0x101,
    /* type = uint32_t */
    MIPC_INTERNAL_EIF_IND_T_CAUSE                           = 0x102,
    /* type = uint32_t */
    MIPC_INTERNAL_EIF_IND_T_MTU                             = 0x103,
    /* type = uint8_t */
    MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_COUNT               = 0x104,
    /* type = struct_array, refer to v4_full_addr */
    MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_LIST                = 0x8105,
    /* type = uint8_t */
    MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_COUNT               = 0x106,
    /* type = struct_array, refer to v6_full_addr */
    MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_LIST                = 0x8107,
    /* pdp type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_INTERNAL_EIF_IND_T_PDP_TYPE                        = 0x108,
    /* type = struct, refer to v4_full_addr */
    MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_TLV_ARRAY           = 0x8109,
    /* type = struct, refer to v6_full_addr */
    MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_TLV_ARRAY           = 0x810A,
};

    /* MIPC_MSG.INTERNAL_HO_IND */
enum mipc_internal_ho_ind_tlv_enum {
    mipc_internal_ho_ind_tlv_NONE = 0,
    /* data call id */
    /* type = uint8_t */
    MIPC_INTERNAL_HO_IND_T_ID                               = 0x100,
    /* HO current progress */
    /* type = uint8_t, refer to INTERNAL_HO_PROGRESS */
    MIPC_INTERNAL_HO_IND_T_PROGRESS                         = 0x102,
    /* source RAN */
    /* type = uint8_t, refer to RAN */
    MIPC_INTERNAL_HO_IND_T_SRC_RAN                          = 0x103,
    /* target RAN */
    /* type = uint8_t, refer to RAN */
    MIPC_INTERNAL_HO_IND_T_DST_RAN                          = 0x104,
    /* HO success ot not */
    /* type = uint8_t */
    MIPC_INTERNAL_HO_IND_T_IS_SUCC                          = 0x105,
    /* v4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_INTERNAL_HO_IND_T_V4_ADDR                          = 0x8106,
    /* v6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_INTERNAL_HO_IND_T_V6_ADDR                          = 0x8107,
    /*  network transmition id */
    /* type = uint32_t */
    MIPC_INTERNAL_HO_IND_T_TRANS_ID                         = 0x108,
};

    /* MIPC_MSG.INTERNAL_MIPC_PENDING_IND */
enum mipc_internal_mipc_pending_ind_tlv_enum {
    mipc_internal_mipc_pending_ind_tlv_NONE = 0,
    /* Current track inused (Modem use track to record pending control message) */
    /* type = struct, refer to track_item */
    MIPC_INTERNAL_MIPC_PENDING_IND_T_TRACK_LIST             = 0x100,
    MIPC_INTERNAL_MIPC_PENDING_IND_T_TRACK_TLV_ARRAY        = 0x100,
};

    /* MIPC_MSG.INTERNAL_TEST_CMD */
enum mipc_internal_test_cmd_tlv_enum {
    mipc_internal_test_cmd_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_TEST_RSP */
enum mipc_internal_test_rsp_tlv_enum {
    mipc_internal_test_rsp_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_EIPPORT_CMD */
enum mipc_internal_eipport_cmd_tlv_enum {
    mipc_internal_eipport_cmd_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPPORT_CMD_T_TRANSID                     = 0x100,
    /* type = uint8_t, refer to INTERNAL_EIPPORT_ACTION */
    MIPC_INTERNAL_EIPPORT_CMD_T_ACTION                      = 0x101,
    /* type = uint8_t */
    MIPC_INTERNAL_EIPPORT_CMD_T_IFID                        = 0x102,
    /* type = struct, refer to addr */
    MIPC_INTERNAL_EIPPORT_CMD_T_ADDR                        = 0x8103,
    /* type = uint8_t */
    MIPC_INTERNAL_EIPPORT_CMD_T_PROTO                       = 0x104,
    /* type = uint16_t */
    MIPC_INTERNAL_EIPPORT_CMD_T_PORT                        = 0x105,
};

    /* MIPC_MSG.INTERNAL_EIPPORT_RSP */
enum mipc_internal_eipport_rsp_tlv_enum {
    mipc_internal_eipport_rsp_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPPORT_RSP_T_TRANSID                     = 0x100,
    /* type = uint8_t, refer to INTERNAL_EIPPORT_ACTION */
    MIPC_INTERNAL_EIPPORT_RSP_T_ACTION                      = 0x101,
    /* type = uint8_t, refer to INTERNAL_EIPPORT_RESULT */
    MIPC_INTERNAL_EIPPORT_RSP_T_RESULT                      = 0x102,
};

    /* MIPC_MSG.INTERNAL_EIPSPI_CMD */
enum mipc_internal_eipspi_cmd_tlv_enum {
    mipc_internal_eipspi_cmd_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPSPI_CMD_T_TRANSID                      = 0x100,
    /* type = uint8_t, refer to INTERNAL_EIPSPI_ACTION */
    MIPC_INTERNAL_EIPSPI_CMD_T_ACTION                       = 0x101,
    /* type = struct, refer to addr */
    MIPC_INTERNAL_EIPSPI_CMD_T_SRC_ADDR                     = 0x8102,
    /* type = struct, refer to addr */
    MIPC_INTERNAL_EIPSPI_CMD_T_DST_ADDR                     = 0x8103,
    /* type = uint8_t, refer to IPSEC_PROTO_ENUM */
    MIPC_INTERNAL_EIPSPI_CMD_T_PROTO                        = 0x104,
    /* type = uint8_t, refer to IPSEC_MODE_ENUM */
    MIPC_INTERNAL_EIPSPI_CMD_T_MODE                         = 0x105,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPSPI_CMD_T_MIN_SPI                      = 0x106,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPSPI_CMD_T_MAX_SPI                      = 0x107,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPSPI_CMD_T_SPI                          = 0x8108,
};

    /* MIPC_MSG.INTERNAL_EIPSPI_RSP */
enum mipc_internal_eipspi_rsp_tlv_enum {
    mipc_internal_eipspi_rsp_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPSPI_RSP_T_TRANSID                      = 0x100,
    /* type = uint8_t */
    MIPC_INTERNAL_EIPSPI_RSP_T_ACTION                       = 0x101,
    /* type = uint32_t */
    MIPC_INTERNAL_EIPSPI_RSP_T_SPI                          = 0x8102,
};

    /* MIPC_MSG.INTERNAL_MULTI_EIPSPI_FREE_CMD */
enum mipc_internal_multi_eipspi_free_cmd_tlv_enum {
    mipc_internal_multi_eipspi_free_cmd_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_TRANSID           = 0x100,
    /* type = uint32_t */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_MULTI_FREE_COUNT  = 0x8101,
    /* type = struct_array, refer to addr */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SRC_ADDR_LIST     = 0x8102,
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SRC_ADDR_TLV_ARRAY = 0x8102,
    /* type = struct_array, refer to addr */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_DST_ADDR_LIST     = 0x8103,
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_DST_ADDR_TLV_ARRAY = 0x8103,
    /* type = byte_array, refer to IPSEC_PROTO_ENUM */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_PROTO_LIST        = 0x104,
    /* it should be a uint32 list */
    /* type = byte_array */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SPI_LIST          = 0x8105,
};

    /* MIPC_MSG.INTERNAL_MULTI_EIPSPI_FREE_RSP */
enum mipc_internal_multi_eipspi_free_rsp_tlv_enum {
    mipc_internal_multi_eipspi_free_rsp_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_TRANSID           = 0x100,
    /* type = uint32_t */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_MULTI_FREE_COUNT  = 0x8101,
    /* it should be a uint32 list */
    /* type = byte_array */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_SPI_LIST          = 0x8102,
    /* type = byte_array, refer to SPI_STATUS_ENUM */
    MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_STATUS_LIST       = 0x8103,
};

    /* MIPC_MSG.INTERNAL_TEST_NTF */
enum mipc_internal_test_ntf_tlv_enum {
    mipc_internal_test_ntf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.INTERNAL_INJECT_TST_NTF */
enum mipc_internal_inject_tst_ntf_tlv_enum {
    mipc_internal_inject_tst_ntf_tlv_NONE = 0,
    /* type = string */
    MIPC_INTERNAL_INJECT_TST_NTF_T_MODULE                   = 0x100,
    /* type = uint8_t */
    MIPC_INTERNAL_INJECT_TST_NTF_T_INDEX                    = 0x101,
    /* type = string */
    MIPC_INTERNAL_INJECT_TST_NTF_T_INJECT_STRING            = 0x102,
};




#endif /* __MIPC_MSG_INTERNAL_CONST_H__ */
