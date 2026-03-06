#ifndef __MIPC_MSG_STK_API_H__
#define __MIPC_MSG_STK_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_stk_set_pac_req_add_pac_bitmask_ptr(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_STK_SET_PAC_REQ_T_PAC_BITMASK_PTR, len, (const void *)value);
}

static inline void * mipc_stk_set_pac_cnf_get_pac_profile(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_SET_PAC_CNF_T_PAC_PROFILE, val_len_ptr);
}

static inline void * mipc_stk_get_pac_cnf_get_pac_profile(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_GET_PAC_CNF_T_PAC_PROFILE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_stk_send_terminal_response_req_add_tr_len(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_stk_send_terminal_response_req_add_tr_ptr(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_PTR, len, (const void *)value);
}

static inline uint16_t mipc_stk_send_terminal_response_cnf_get_status_words(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_STK_SEND_TERMINAL_RESPONSE_CNF_T_STATUS_WORDS, def_val);
}

static inline uint32_t mipc_stk_send_terminal_response_cnf_get_tr_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_STK_SEND_TERMINAL_RESPONSE_CNF_T_TR_LEN, def_val);
}

static inline void * mipc_stk_send_terminal_response_cnf_get_tr_ptr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_SEND_TERMINAL_RESPONSE_CNF_T_TR_PTR, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_stk_send_envelope_req_add_envelope_len(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_STK_SEND_ENVELOPE_REQ_T_ENVELOPE_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_stk_send_envelope_req_add_envelope_ptr(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_STK_SEND_ENVELOPE_REQ_T_ENVELOPE_PTR, len, (const void *)value);
}

static inline uint16_t mipc_stk_send_envelope_cnf_get_status_words(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_STK_SEND_ENVELOPE_CNF_T_STATUS_WORDS, def_val);
}

static inline char * mipc_stk_send_envelope_cnf_get_envelope_response(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_SEND_ENVELOPE_CNF_T_ENVELOPE_RESPONSE, val_len_ptr);
}

static inline void * mipc_stk_get_envelope_info_cnf_get_envelope_bitmask(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_GET_ENVELOPE_INFO_CNF_T_ENVELOPE_BITMASK, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_stk_handle_call_setup_from_sim_req_add_data(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ_T_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_stk_send_bipconf_req_add_cmd_num(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_STK_SEND_BIPCONF_REQ_T_CMD_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_stk_send_bipconf_req_add_result(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_STK_SEND_BIPCONF_REQ_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_stk_pac_type_const_enum mipc_stk_pac_ind_get_pac_type(mipc_msg_t *msg_ptr, mipc_stk_pac_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_stk_pac_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_STK_PAC_IND_T_PAC_TYPE, def_val);
}

static inline uint16_t mipc_stk_pac_ind_get_pac_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_STK_PAC_IND_T_PAC_LEN, def_val);
}

static inline void * mipc_stk_pac_ind_get_pac(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_PAC_IND_T_PAC, val_len_ptr);
}

static inline mipc_sim_refresh_result_type_const_enum mipc_stk_sim_refresh_ind_get_sim_refresh_result(mipc_msg_t *msg_ptr, mipc_sim_refresh_result_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_refresh_result_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_STK_SIM_REFRESH_IND_T_SIM_REFRESH_RESULT, def_val);
}

static inline uint32_t mipc_stk_sim_refresh_ind_get_ef_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_STK_SIM_REFRESH_IND_T_EF_ID, def_val);
}

static inline char * mipc_stk_sim_refresh_ind_get_aid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_SIM_REFRESH_IND_T_AID, val_len_ptr);
}

static inline char * mipc_stk_bip_event_notify_ind_get_cmd_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_STK_BIP_EVENT_NOTIFY_IND_T_CMD_DATA, val_len_ptr);
}


#endif /* __MIPC_MSG_STK_API_H__ */
