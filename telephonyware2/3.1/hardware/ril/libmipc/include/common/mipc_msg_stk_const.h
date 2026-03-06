#ifndef __MIPC_MSG_STK_CONST_H__
#define __MIPC_MSG_STK_CONST_H__

enum MIPC_STK_MSG_enum {
    MIPC_STK_MSG_NONE = 0,
    /* This command is used to set PAC information. */
    MIPC_STK_SET_PAC_REQ                                    = 2049,
    MIPC_STK_SET_PAC_CNF                                    = 2050,

    /* This command is used to acquire proactive command management information indicating whether proactive command will be managed by ME or TE. */
    MIPC_STK_GET_PAC_REQ                                    = 2051,
    MIPC_STK_GET_PAC_CNF                                    = 2052,

    /* This command is used to send terminal response to UICC. */
    MIPC_STK_SEND_TERMINAL_RESPONSE_REQ                     = 2053,
    MIPC_STK_SEND_TERMINAL_RESPONSE_CNF                     = 2054,

    /* This command is used to send envelope to UICC. */
    MIPC_STK_SEND_ENVELOPE_REQ                              = 2055,
    MIPC_STK_SEND_ENVELOPE_CNF                              = 2056,

    /* This command is used to acquire envelope information. */
    MIPC_STK_GET_ENVELOPE_INFO_REQ                          = 2057,
    MIPC_STK_GET_ENVELOPE_INFO_CNF                          = 2058,

    /* This command is used to set the result of host confirmation for BIP open channel command which includes non-null alpha ID. */
    MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ                 = 2059,
    MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_CNF                 = 2060,

    /* This command is used to set the result of host confirmation for BIP open channel command. */
    MIPC_STK_SEND_BIPCONF_REQ                               = 2061,
    MIPC_STK_SEND_BIPCONF_CNF                               = 2062,

    /* This command is used to report proactive command from UICC. */
    MIPC_STK_PAC_IND                                        = 18433,

    /* This command is used to report SIM refresh event. */
    MIPC_STK_SIM_REFRESH_IND                                = 18434,

    /* This command is used to report BIP event. */
    MIPC_STK_BIP_EVENT_NOTIFY_IND                           = 18435,


};

    /* MIPC_MSG.STK_SET_PAC_REQ */
enum mipc_stk_set_pac_req_tlv_enum {
    mipc_stk_set_pac_req_tlv_NONE = 0,
    /* It indicates bitmask; each bit uses PAC to indicate if the host intends to handle/receive a notification for a specific proactive command or not (size is 32 bytes) */
    /* type = byte_array */
    MIPC_STK_SET_PAC_REQ_T_PAC_BITMASK_PTR                  = 0x101,
};

    /* MIPC_MSG.STK_SET_PAC_CNF */
enum mipc_stk_set_pac_cnf_tlv_enum {
    mipc_stk_set_pac_cnf_tlv_NONE = 0,
    /* Each byte identifies the current support for a specific proactive command */
    /* type = byte_array */
    MIPC_STK_SET_PAC_CNF_T_PAC_PROFILE                      = 0x100,
};

    /* MIPC_MSG.STK_GET_PAC_REQ */
enum mipc_stk_get_pac_req_tlv_enum {
    mipc_stk_get_pac_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.STK_GET_PAC_CNF */
enum mipc_stk_get_pac_cnf_tlv_enum {
    mipc_stk_get_pac_cnf_tlv_NONE = 0,
    /* Each byte identifies the current support for a specific proactive command */
    /* type = byte_array */
    MIPC_STK_GET_PAC_CNF_T_PAC_PROFILE                      = 0x100,
};

    /* MIPC_MSG.STK_SEND_TERMINAL_RESPONSE_REQ */
enum mipc_stk_send_terminal_response_req_tlv_enum {
    mipc_stk_send_terminal_response_req_tlv_NONE = 0,
    /* Length of terminal response */
    /* type = uint32_t */
    MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_LEN            = 0x101,
    /* Data of terminal response */
    /* type = byte_array */
    MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_PTR            = 0x102,
};

    /* MIPC_MSG.STK_SEND_TERMINAL_RESPONSE_CNF */
enum mipc_stk_send_terminal_response_cnf_tlv_enum {
    mipc_stk_send_terminal_response_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_STK_SEND_TERMINAL_RESPONSE_CNF_T_STATUS_WORDS      = 0x100,
    /* Length of terminal response */
    /* type = uint32_t */
    MIPC_STK_SEND_TERMINAL_RESPONSE_CNF_T_TR_LEN            = 0x101,
    /* Data of terminal response */
    /* type = byte_array */
    MIPC_STK_SEND_TERMINAL_RESPONSE_CNF_T_TR_PTR            = 0x102,
};

    /* MIPC_MSG.STK_SEND_ENVELOPE_REQ */
enum mipc_stk_send_envelope_req_tlv_enum {
    mipc_stk_send_envelope_req_tlv_NONE = 0,
    /* Length of envelope */
    /* type = uint32_t */
    MIPC_STK_SEND_ENVELOPE_REQ_T_ENVELOPE_LEN               = 0x101,
    /* Data of envelope */
    /* type = byte_array */
    MIPC_STK_SEND_ENVELOPE_REQ_T_ENVELOPE_PTR               = 0x102,
};

    /* MIPC_MSG.STK_SEND_ENVELOPE_CNF */
enum mipc_stk_send_envelope_cnf_tlv_enum {
    mipc_stk_send_envelope_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_STK_SEND_ENVELOPE_CNF_T_STATUS_WORDS               = 0x100,
    /* Hex string of the whole envelope response (BER-TLV) given by UICC */
    /* type = string */
    MIPC_STK_SEND_ENVELOPE_CNF_T_ENVELOPE_RESPONSE          = 0x101,
};

    /* MIPC_MSG.STK_GET_ENVELOPE_INFO_REQ */
enum mipc_stk_get_envelope_info_req_tlv_enum {
    mipc_stk_get_envelope_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.STK_GET_ENVELOPE_INFO_CNF */
enum mipc_stk_get_envelope_info_cnf_tlv_enum {
    mipc_stk_get_envelope_info_cnf_tlv_NONE = 0,
    /* Envelope value with bit format */
    /* type = byte_array */
    MIPC_STK_GET_ENVELOPE_INFO_CNF_T_ENVELOPE_BITMASK       = 0x100,
};

    /* MIPC_MSG.STK_HANDLE_CALL_SETUP_FROM_SIM_REQ */
enum mipc_stk_handle_call_setup_from_sim_req_tlv_enum {
    mipc_stk_handle_call_setup_from_sim_req_tlv_NONE = 0,
    /* Integer (e.g., 0, 1, 32, 33) */
    /* type = uint8_t */
    MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ_T_DATA          = 0x100,
};

    /* MIPC_MSG.STK_HANDLE_CALL_SETUP_FROM_SIM_CNF */
enum mipc_stk_handle_call_setup_from_sim_cnf_tlv_enum {
    mipc_stk_handle_call_setup_from_sim_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.STK_SEND_BIPCONF_REQ */
enum mipc_stk_send_bipconf_req_tlv_enum {
    mipc_stk_send_bipconf_req_tlv_NONE = 0,
    /* The number of the BIP configuration request command */
    /* type = uint32_t */
    MIPC_STK_SEND_BIPCONF_REQ_T_CMD_NUM                     = 0x100,
    /* Send BIP configuration result */
    /* type = uint32_t */
    MIPC_STK_SEND_BIPCONF_REQ_T_RESULT                      = 0x101,
};

    /* MIPC_MSG.STK_SEND_BIPCONF_CNF */
enum mipc_stk_send_bipconf_cnf_tlv_enum {
    mipc_stk_send_bipconf_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.STK_PAC_IND */
enum mipc_stk_pac_ind_tlv_enum {
    mipc_stk_pac_ind_tlv_NONE = 0,
    /* Proactive command type from UICC */
    /* type = uint8_t, refer to STK_PAC_TYPE */
    MIPC_STK_PAC_IND_T_PAC_TYPE                             = 0x100,
    /* Length of proactive command */
    /* type = uint16_t */
    MIPC_STK_PAC_IND_T_PAC_LEN                              = 0x101,
    /* Data of proactive command payload */
    /* type = byte_array */
    MIPC_STK_PAC_IND_T_PAC                                  = 0x8102,
};

    /* MIPC_MSG.STK_SIM_REFRESH_IND */
enum mipc_stk_sim_refresh_ind_tlv_enum {
    mipc_stk_sim_refresh_ind_tlv_NONE = 0,
    /* The result of SIM refresh */
    /* type = uint8_t, refer to SIM_REFRESH_RESULT_TYPE */
    MIPC_STK_SIM_REFRESH_IND_T_SIM_REFRESH_RESULT           = 0x100,
    /* EF ID of the updated file */
    /* type = uint32_t */
    MIPC_STK_SIM_REFRESH_IND_T_EF_ID                        = 0x101,
    /* AID of application which causes SIM refresh */
    /* type = string */
    MIPC_STK_SIM_REFRESH_IND_T_AID                          = 0x102,
};

    /* MIPC_MSG.STK_BIP_EVENT_NOTIFY_IND */
enum mipc_stk_bip_event_notify_ind_tlv_enum {
    mipc_stk_bip_event_notify_ind_tlv_NONE = 0,
    /* Data of BIP command */
    /* type = string */
    MIPC_STK_BIP_EVENT_NOTIFY_IND_T_CMD_DATA                = 0x100,
};




#endif /* __MIPC_MSG_STK_CONST_H__ */
