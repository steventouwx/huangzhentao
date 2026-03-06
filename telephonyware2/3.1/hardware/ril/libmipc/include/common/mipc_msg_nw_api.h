#ifndef __MIPC_MSG_NW_API_H__
#define __MIPC_MSG_NW_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_nw_radio_state_const_enum mipc_nw_get_radio_state_cnf_get_sw_state(mipc_msg_t *msg_ptr, mipc_nw_radio_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_radio_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RADIO_STATE_CNF_T_SW_STATE, def_val);
}

static inline mipc_nw_radio_state_const_enum mipc_nw_get_radio_state_cnf_get_hw_state(mipc_msg_t *msg_ptr, mipc_nw_radio_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_radio_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RADIO_STATE_CNF_T_HW_STATE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_radio_state_req_add_sw_state(mipc_msg_t *msg_ptr, enum mipc_nw_radio_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_radio_state_req_add_cause(mipc_msg_t *msg_ptr, enum mipc_nw_radio_state_cause_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RADIO_STATE_REQ_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_radio_state_req_add_hw_state(mipc_msg_t *msg_ptr, enum mipc_nw_radio_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RADIO_STATE_REQ_T_HW_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_radio_state_const_enum mipc_nw_set_radio_state_cnf_get_sw_state(mipc_msg_t *msg_ptr, mipc_nw_radio_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_radio_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_RADIO_STATE_CNF_T_SW_STATE, def_val);
}

static inline mipc_nw_radio_state_const_enum mipc_nw_set_radio_state_cnf_get_hw_state(mipc_msg_t *msg_ptr, mipc_nw_radio_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_radio_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_RADIO_STATE_CNF_T_HW_STATE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_nw_register_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_format(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_oper(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_OPER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_act(mipc_msg_t *msg_ptr, enum mipc_nw_act_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_ACT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_arfcn(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_ARFCN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_rat_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_RAT_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_block(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_BLOCK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_ctrl_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_CTRL_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_search_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_SEARCH_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_register_state_req_add_rsrp(mipc_msg_t *msg_ptr, int32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int32(msg_ptr, MIPC_NW_SET_REGISTER_STATE_REQ_T_RSRP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_reg_state_struct4* mipc_nw_set_register_state_cnf_get_state(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_state_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_STATE, val_len_ptr);
}

static inline uint16_t mipc_nw_set_register_state_cnf_get_nw_err(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_NW_ERR, def_val);
}

static inline mipc_nw_register_mode_const_enum mipc_nw_set_register_state_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_nw_register_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_register_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_MODE, def_val);
}

static inline mipc_nw_data_speed_const_enum mipc_nw_set_register_state_cnf_get_data_speed(mipc_msg_t *msg_ptr, mipc_nw_data_speed_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_speed_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_DATA_SPEED, def_val);
}

static inline char * mipc_nw_set_register_state_cnf_get_nw_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_NW_NAME, val_len_ptr);
}

static inline char * mipc_nw_set_register_state_cnf_get_roaming_text(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_ROAMING_TEXT, val_len_ptr);
}

static inline uint16_t mipc_nw_set_register_state_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_FAIL_CAUSE, def_val);
}

static inline char * mipc_nw_set_register_state_cnf_get_nw_long_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_NW_LONG_NAME, val_len_ptr);
}

static inline mipc_nw_reg_info_struct4* mipc_nw_set_register_state_cnf_get_ps_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_PS_INFO, val_len_ptr);
}

static inline mipc_nw_reg_state_v1_struct4* mipc_nw_set_register_state_cnf_get_state_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_state_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_REGISTER_STATE_CNF_T_STATE_V1, val_len_ptr);
}

static inline mipc_nw_reg_state_struct4* mipc_nw_get_register_state_cnf_get_state(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_state_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_STATE, val_len_ptr);
}

static inline uint16_t mipc_nw_get_register_state_cnf_get_nw_err(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_NW_ERR, def_val);
}

static inline mipc_nw_register_mode_const_enum mipc_nw_get_register_state_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_nw_register_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_register_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_MODE, def_val);
}

static inline mipc_nw_data_speed_const_enum mipc_nw_get_register_state_cnf_get_data_speed(mipc_msg_t *msg_ptr, mipc_nw_data_speed_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_speed_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_DATA_SPEED, def_val);
}

static inline char * mipc_nw_get_register_state_cnf_get_nw_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_NW_NAME, val_len_ptr);
}

static inline char * mipc_nw_get_register_state_cnf_get_roaming_text(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_ROAMING_TEXT, val_len_ptr);
}

static inline uint16_t mipc_nw_get_register_state_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_FAIL_CAUSE, def_val);
}

static inline char * mipc_nw_get_register_state_cnf_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_ID, val_len_ptr);
}

static inline char * mipc_nw_get_register_state_cnf_get_nw_long_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_NW_LONG_NAME, val_len_ptr);
}

static inline mipc_nw_reg_info_struct4* mipc_nw_get_register_state_cnf_get_ps_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_PS_INFO, val_len_ptr);
}

static inline mipc_nw_reg_state_v1_struct4* mipc_nw_get_register_state_cnf_get_state_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_state_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_STATE_V1, val_len_ptr);
}

static inline mipc_encoding_scheme_const_enum mipc_nw_get_register_state_cnf_get_short_name_encoding_scheme(mipc_msg_t *msg_ptr, mipc_encoding_scheme_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_encoding_scheme_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_SHORT_NAME_ENCODING_SCHEME, def_val);
}

static inline void * mipc_nw_get_register_state_cnf_get_plmn_short_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_SHORT_NAME, val_len_ptr);
}

static inline mipc_encoding_scheme_const_enum mipc_nw_get_register_state_cnf_get_long_name_encoding_scheme(mipc_msg_t *msg_ptr, mipc_encoding_scheme_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_encoding_scheme_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_LONG_NAME_ENCODING_SCHEME, def_val);
}

static inline void * mipc_nw_get_register_state_cnf_get_plmn_long_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_LONG_NAME, val_len_ptr);
}

static inline mipc_plmn_name_source_const_enum mipc_nw_get_register_state_cnf_get_plmn_name_source(mipc_msg_t *msg_ptr, mipc_plmn_name_source_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_plmn_name_source_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_NAME_SOURCE, def_val);
}

static inline uint8_t mipc_nw_get_plmn_list_cnf_get_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_COUNT, def_val);
}

static inline mipc_nw_provider_struct4* mipc_nw_get_plmn_list_cnf_get_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_get_plmn_list_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_PLMN_LIST_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_extend_provider_struct4* mipc_nw_get_plmn_list_cnf_get_extend_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_extend_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PLMN_LIST_CNF_T_EXTEND_INFO_LIST, val_len_ptr);
}

static inline mipc_nw_provider_struct4* mipc_nw_get_plmn_list_cnf_get_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_extend_provider_struct4* mipc_nw_get_plmn_list_cnf_get_extend_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_extend_provider_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_PLMN_LIST_CNF_T_EXTEND_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_ps_req_add_action(mipc_msg_t *msg_ptr, enum mipc_nw_ps_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_PS_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_ps_req_add_ctrl_mode(mipc_msg_t *msg_ptr, enum mipc_nw_ps_ctrl_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_PS_REQ_T_CTRL_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_ps_const_enum mipc_nw_set_ps_cnf_get_state(mipc_msg_t *msg_ptr, mipc_nw_ps_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ps_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_PS_CNF_T_STATE, def_val);
}

static inline mipc_nw_ps_const_enum mipc_nw_set_ps_cnf_get_tach(mipc_msg_t *msg_ptr, mipc_nw_ps_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ps_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_PS_CNF_T_TACH, def_val);
}

static inline mipc_nw_data_speed_const_enum mipc_nw_set_ps_cnf_get_data_speed(mipc_msg_t *msg_ptr, mipc_nw_data_speed_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_speed_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_PS_CNF_T_DATA_SPEED, def_val);
}

static inline uint8_t mipc_nw_set_ps_cnf_get_nw_frequency(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_PS_CNF_T_NW_FREQUENCY, def_val);
}

static inline uint16_t mipc_nw_set_ps_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_SET_PS_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_ps_const_enum mipc_nw_get_ps_cnf_get_state(mipc_msg_t *msg_ptr, mipc_nw_ps_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ps_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PS_CNF_T_STATE, def_val);
}

static inline mipc_nw_ps_const_enum mipc_nw_get_ps_cnf_get_tach(mipc_msg_t *msg_ptr, mipc_nw_ps_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ps_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PS_CNF_T_TACH, def_val);
}

static inline mipc_nw_data_speed_const_enum mipc_nw_get_ps_cnf_get_data_speed(mipc_msg_t *msg_ptr, mipc_nw_data_speed_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_speed_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_PS_CNF_T_DATA_SPEED, def_val);
}

static inline uint8_t mipc_nw_get_ps_cnf_get_nw_frequency(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PS_CNF_T_NW_FREQUENCY, def_val);
}

static inline uint16_t mipc_nw_get_ps_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_PS_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_ps_reg_info_struct4* mipc_nw_get_ps_cnf_get_reg_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ps_reg_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_REG_INFO, val_len_ptr);
}

static inline mipc_nw_cell_type_const_enum mipc_nw_get_ps_cnf_get_cell_type(mipc_msg_t *msg_ptr, mipc_nw_cell_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cell_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PS_CNF_T_CELL_TYPE, def_val);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_get_ps_cnf_get_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_get_ps_cnf_get_nsa_ext_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_NSA_EXT_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_get_ps_cnf_get_gsm_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_GSM_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_get_ps_cnf_get_umts_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_UMTS_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_get_ps_cnf_get_lte_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_LTE_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_get_ps_cnf_get_nr_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_NR_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_get_ps_cnf_get_cdma_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_CDMA_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_ps_reg_info_v1_struct8* mipc_nw_get_ps_cnf_get_reg_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ps_reg_info_v1_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_REG_INFO_V1, val_len_ptr);
}

static inline mipc_nw_nr_cell_v2_struct8* mipc_nw_get_ps_cnf_get_nsa_ext_cell_info_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v2_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_NSA_EXT_CELL_INFO_V2, val_len_ptr);
}

static inline mipc_nw_nr_cell_v2_struct8* mipc_nw_get_ps_cnf_get_nr_cell_info_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v2_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_NR_CELL_INFO_V2, val_len_ptr);
}

static inline mipc_nw_cdma_cell_v1_struct4* mipc_nw_get_ps_cnf_get_cdma_cell_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PS_CNF_T_CDMA_CELL_INFO_V1, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_req_add_signal_strength_interval(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_REQ_T_SIGNAL_STRENGTH_INTERVAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_req_add_rssi_threshold(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_REQ_T_RSSI_THRESHOLD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_req_add_err_rate_threshold(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_REQ_T_ERR_RATE_THRESHOLD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_req_add_rsrp_threshold(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_REQ_T_RSRP_THRESHOLD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_req_add_snr_threshold(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_REQ_T_SNR_THRESHOLD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_req_add_threshold_mode(mipc_msg_t *msg_ptr, enum mipc_nw_signal_threshold_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SIGNAL_REQ_T_THRESHOLD_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_nw_set_signal_cnf_get_rssi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RSSI, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_err_rate(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_ERR_RATE, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_rsrp(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RSRP, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_snr(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_SNR, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_signal_strength_interval(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_SIGNAL_STRENGTH_INTERVAL, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_rssi_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RSSI_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_err_rate_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_ERR_RATE_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_rsrp_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RSRP_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_set_signal_cnf_get_snr_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_SNR_THRESHOLD, def_val);
}

static inline uint16_t mipc_nw_set_signal_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_signal_type_const_enum mipc_nw_set_signal_cnf_get_signal_type(mipc_msg_t *msg_ptr, mipc_nw_signal_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_signal_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_SIGNAL_TYPE, def_val);
}

static inline mipc_nw_gsm_signal_strength_struct4* mipc_nw_set_signal_cnf_get_gsm_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_GSM_SIGNAL, val_len_ptr);
}

static inline mipc_nw_umts_signal_strength_struct4* mipc_nw_set_signal_cnf_get_umts_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_UMTS_SIGNAL, val_len_ptr);
}

static inline mipc_nw_lte_signal_strength_struct4* mipc_nw_set_signal_cnf_get_lte_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_LTE_SIGNAL, val_len_ptr);
}

static inline mipc_nw_nr_signal_strength_struct4* mipc_nw_set_signal_cnf_get_nr_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_NR_SIGNAL, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_struct4* mipc_nw_set_signal_cnf_get_raw_signal_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_LIST, val_len_ptr);
}

static inline uint8_t mipc_nw_set_signal_cnf_get_raw_signal_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_COUNT, def_val);
}

static inline mipc_nw_cdma_signal_strength_struct4* mipc_nw_set_signal_cnf_get_cdma_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_CDMA_SIGNAL, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_struct4* mipc_nw_set_signal_cnf_get_raw_signal_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_v1_struct4* mipc_nw_set_signal_cnf_get_raw_signal_info_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_rssi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RSSI, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_err_rate(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_ERR_RATE, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_rsrp(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RSRP, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_snr(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_SNR, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_signal_strength_interval(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_SIGNAL_STRENGTH_INTERVAL, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_rssi_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RSSI_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_err_rate_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_ERR_RATE_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_rsrp_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RSRP_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_get_signal_cnf_get_snr_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_SNR_THRESHOLD, def_val);
}

static inline uint16_t mipc_nw_get_signal_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_signal_type_const_enum mipc_nw_get_signal_cnf_get_signal_type(mipc_msg_t *msg_ptr, mipc_nw_signal_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_signal_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_SIGNAL_TYPE, def_val);
}

static inline mipc_nw_gsm_signal_strength_struct4* mipc_nw_get_signal_cnf_get_gsm_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_GSM_SIGNAL, val_len_ptr);
}

static inline mipc_nw_umts_signal_strength_struct4* mipc_nw_get_signal_cnf_get_umts_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_UMTS_SIGNAL, val_len_ptr);
}

static inline mipc_nw_lte_signal_strength_struct4* mipc_nw_get_signal_cnf_get_lte_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_LTE_SIGNAL, val_len_ptr);
}

static inline mipc_nw_nr_signal_strength_struct4* mipc_nw_get_signal_cnf_get_nr_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_struct4* mipc_nw_get_signal_cnf_get_raw_signal_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_LIST, val_len_ptr);
}

static inline uint8_t mipc_nw_get_signal_cnf_get_raw_signal_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_COUNT, def_val);
}

static inline mipc_nw_cdma_signal_strength_struct4* mipc_nw_get_signal_cnf_get_cdma_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_CDMA_SIGNAL, val_len_ptr);
}

static inline mipc_nw_lte_signal_strength_v1_struct4* mipc_nw_get_signal_cnf_get_lte_signal_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_signal_strength_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_LTE_SIGNAL_V1, val_len_ptr);
}

static inline mipc_nw_nr_signal_strength_v1_struct4* mipc_nw_get_signal_cnf_get_nr_signal_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL_V1, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_struct4* mipc_nw_get_signal_cnf_get_raw_signal_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_nr_signal_strength_v3_struct4* mipc_nw_get_signal_cnf_get_nr_signal_v3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_v3_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL_V3, val_len_ptr);
}

static inline mipc_nw_lte_signal_strength_v2_struct4* mipc_nw_get_signal_cnf_get_lte_signal_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_signal_strength_v2_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_LTE_SIGNAL_V2, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_v1_struct4* mipc_nw_get_signal_cnf_get_raw_signal_info_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_nw_get_preferred_provider_cnf_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_COUNT, def_val);
}

static inline mipc_nw_provider_struct4* mipc_nw_get_preferred_provider_cnf_get_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_get_preferred_provider_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_provider_struct4* mipc_nw_get_preferred_provider_cnf_get_nw_provider_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_NW_PROVIDER_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_preferred_provider_req_add_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_PREFERRED_PROVIDER_REQ_T_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_preferred_provider_req_add_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_provider_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_PREFERRED_PROVIDER_REQ_T_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_preferred_provider_req_add_nw_provider_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_provider_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_nw_set_preferred_provider_cnf_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_COUNT, def_val);
}

static inline mipc_nw_provider_struct4* mipc_nw_set_preferred_provider_cnf_get_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_set_preferred_provider_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_provider_struct4* mipc_nw_set_preferred_provider_cnf_get_nw_provider_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_NW_PROVIDER_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_home_provider_req_add_provider(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_provider_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_HOME_PROVIDER_REQ_T_PROVIDER, len, (const void *)value);
}

static inline mipc_nw_provider_struct4* mipc_nw_set_home_provider_cnf_get_provider(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SET_HOME_PROVIDER_CNF_T_PROVIDER, val_len_ptr);
}

static inline uint16_t mipc_nw_set_home_provider_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_SET_HOME_PROVIDER_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_provider_struct4* mipc_nw_get_home_provider_cnf_get_provider(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_HOME_PROVIDER_CNF_T_PROVIDER, val_len_ptr);
}

static inline uint16_t mipc_nw_get_home_provider_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_HOME_PROVIDER_CNF_T_FAIL_CAUSE, def_val);
}

static inline char * mipc_nw_get_ia_status_cnf_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_IA_STATUS_CNF_T_APN, val_len_ptr);
}

static inline uint8_t mipc_nw_get_ia_status_cnf_get_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_IA_STATUS_CNF_T_RAT, def_val);
}

static inline mipc_apn_pdp_type_const_enum mipc_nw_get_ia_status_cnf_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_IA_STATUS_CNF_T_PDP_TYPE, def_val);
}

static inline mipc_apn_auth_type_const_enum mipc_nw_get_ia_status_cnf_get_auth_type(mipc_msg_t *msg_ptr, mipc_apn_auth_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_auth_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_IA_STATUS_CNF_T_AUTH_TYPE, def_val);
}

static inline char * mipc_nw_get_ia_status_cnf_get_userid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_IA_STATUS_CNF_T_USERID, val_len_ptr);
}

static inline char * mipc_nw_get_ia_status_cnf_get_password(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_IA_STATUS_CNF_T_PASSWORD, val_len_ptr);
}

static inline uint32_t mipc_nw_get_ia_status_cnf_get_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_IA_STATUS_CNF_T_CAUSE, def_val);
}

static inline mipc_nw_nitz_info_struct4* mipc_nw_get_nitz_cnf_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nitz_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_NITZ_CNF_T_INFO, val_len_ptr);
}

static inline uint8_t mipc_nw_get_nitz_cnf_get_tz_valid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NITZ_CNF_T_TZ_VALID, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_idle_hint_req_add_status(mipc_msg_t *msg_ptr, enum mipc_nw_fast_dormancy_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_IDLE_HINT_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_idle_hint_req_add_param_1(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_IDLE_HINT_REQ_T_PARAM_1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_idle_hint_req_add_param_2(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_IDLE_HINT_REQ_T_PARAM_2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_fast_dormancy_const_enum mipc_nw_set_idle_hint_cnf_get_status(mipc_msg_t *msg_ptr, mipc_nw_fast_dormancy_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_fast_dormancy_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_IDLE_HINT_CNF_T_STATUS, def_val);
}

static inline uint16_t mipc_nw_set_idle_hint_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_SET_IDLE_HINT_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_fast_dormancy_const_enum mipc_nw_get_idle_hint_cnf_get_status(mipc_msg_t *msg_ptr, mipc_nw_fast_dormancy_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_fast_dormancy_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_IDLE_HINT_CNF_T_STATUS, def_val);
}

static inline uint16_t mipc_nw_get_idle_hint_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_IDLE_HINT_CNF_T_FAIL_CAUSE, def_val);
}

static inline uint8_t mipc_nw_get_idle_hint_cnf_get_r8_fd_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_IDLE_HINT_CNF_T_R8_FD_STATUS, def_val);
}

static inline uint32_t mipc_nw_get_base_stations_cnf_get_gsm_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_COUNT, def_val);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_get_base_stations_cnf_get_gsm_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_get_base_stations_cnf_get_umts_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_COUNT, def_val);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_get_base_stations_cnf_get_umts_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_get_base_stations_cnf_get_tdscdma_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_TDSCDMA_CELL_COUNT, def_val);
}

static inline mipc_nw_tdscdma_cell_struct4* mipc_nw_get_base_stations_cnf_get_tdscdma_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_tdscdma_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_TDSCDMA_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_get_base_stations_cnf_get_lte_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_COUNT, def_val);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_get_base_stations_cnf_get_lte_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_get_base_stations_cnf_get_cdma_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_COUNT, def_val);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_get_base_stations_cnf_get_cdma_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_get_base_stations_cnf_get_nr_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_COUNT, def_val);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_get_base_stations_cnf_get_nr_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_get_base_stations_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_lte_cell_v1_struct4* mipc_nw_get_base_stations_cnf_get_lte_cell_list_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST_V1, index, val_len_ptr);
}

static inline mipc_nw_nr_cell_v1_struct4* mipc_nw_get_base_stations_cnf_get_nr_cell_list_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST_V1, index, val_len_ptr);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_get_base_stations_cnf_get_gsm_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_get_base_stations_cnf_get_umts_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_tdscdma_cell_struct4* mipc_nw_get_base_stations_cnf_get_tdscdma_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_tdscdma_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_TDSCDMA_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_get_base_stations_cnf_get_lte_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_get_base_stations_cnf_get_cdma_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_get_base_stations_cnf_get_nr_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_nr_cell_v2_struct8* mipc_nw_get_base_stations_cnf_get_nr_cell_v2_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v2_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_V2_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_cell_v2_struct4* mipc_nw_get_base_stations_cnf_get_lte_cell_v2_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_v2_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_V2_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cdma_cell_v1_struct4* mipc_nw_get_base_stations_cnf_get_cdma_cell_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_location_info_struct4* mipc_nw_get_location_info_cnf_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_location_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_LOCATION_INFO_CNF_T_INFO, val_len_ptr);
}

static inline uint16_t mipc_nw_get_location_info_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_LOCATION_INFO_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_rat_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RAT_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_rat_req_add_prefer_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RAT_REQ_T_PREFER_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_rat_req_add_bit_rat(mipc_msg_t *msg_ptr, enum mipc_nw_bit_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_RAT_REQ_T_BIT_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_provider_name_req_add_plmn_id(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_REQ_T_PLMN_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_get_provider_name_req_add_lac(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_REQ_T_LAC, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_provider_name_req_add_option(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_REQ_T_OPTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_provider_name_req_add_opl_index(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_REQ_T_OPL_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_nw_get_provider_name_cnf_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_ID, val_len_ptr);
}

static inline uint32_t mipc_nw_get_provider_name_cnf_get_lac(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_LAC, def_val);
}

static inline char * mipc_nw_get_provider_name_cnf_get_nw_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME, val_len_ptr);
}

static inline char * mipc_nw_get_provider_name_cnf_get_nw_name_long(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME_LONG, val_len_ptr);
}

static inline mipc_nw_name_pair_struct4* mipc_nw_get_provider_name_cnf_get_eons_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_name_pair_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_EONS_NAME, val_len_ptr);
}

static inline mipc_nw_name_pair_struct4* mipc_nw_get_provider_name_cnf_get_nitz_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_name_pair_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_NITZ_NAME, val_len_ptr);
}

static inline mipc_nw_name_pair_struct4* mipc_nw_get_provider_name_cnf_get_ts25_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_name_pair_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_TS25_NAME, val_len_ptr);
}

static inline mipc_encoding_scheme_const_enum mipc_nw_get_provider_name_cnf_get_short_name_encoding_scheme(mipc_msg_t *msg_ptr, mipc_encoding_scheme_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_encoding_scheme_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_SHORT_NAME_ENCODING_SCHEME, def_val);
}

static inline void * mipc_nw_get_provider_name_cnf_get_plmn_short_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_SHORT_NAME, val_len_ptr);
}

static inline mipc_encoding_scheme_const_enum mipc_nw_get_provider_name_cnf_get_long_name_encoding_scheme(mipc_msg_t *msg_ptr, mipc_encoding_scheme_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_encoding_scheme_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_LONG_NAME_ENCODING_SCHEME, def_val);
}

static inline void * mipc_nw_get_provider_name_cnf_get_plmn_long_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_LONG_NAME, val_len_ptr);
}

static inline mipc_plmn_name_source_const_enum mipc_nw_get_provider_name_cnf_get_plmn_name_source(mipc_msg_t *msg_ptr, mipc_plmn_name_source_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_plmn_name_source_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_NAME_SOURCE, def_val);
}

static inline mipc_nw_access_tech_const_enum mipc_nw_get_rat_cnf_get_act(mipc_msg_t *msg_ptr, mipc_nw_access_tech_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_access_tech_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RAT_CNF_T_ACT, def_val);
}

static inline uint8_t mipc_nw_get_rat_cnf_get_gprs_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RAT_CNF_T_GPRS_STATUS, def_val);
}

static inline uint8_t mipc_nw_get_rat_cnf_get_rat_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RAT_CNF_T_RAT_MODE, def_val);
}

static inline uint8_t mipc_nw_get_rat_cnf_get_prefer_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RAT_CNF_T_PREFER_RAT, def_val);
}

static inline mipc_nw_rat_lock_const_enum mipc_nw_get_rat_cnf_get_lock(mipc_msg_t *msg_ptr, mipc_nw_rat_lock_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_rat_lock_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RAT_CNF_T_LOCK, def_val);
}

static inline mipc_nw_bit_rat_const_enum mipc_nw_get_rat_cnf_get_bit_rat(mipc_msg_t *msg_ptr, mipc_nw_bit_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_bit_rat_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_RAT_CNF_T_BIT_RAT, def_val);
}

static inline mipc_nw_bit_rat_const_enum mipc_nw_get_rat_cnf_get_radio_capability(mipc_msg_t *msg_ptr, mipc_nw_bit_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_bit_rat_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_RAT_CNF_T_RADIO_CAPABILITY, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_req_add_nr_opt(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_REQ_T_NR_OPT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_req_add_act_operation(mipc_msg_t *msg_ptr, enum mipc_nw_vg_option_operation_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_REQ_T_ACT_OPERATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_cs_reg_info_struct4* mipc_nw_get_cs_cnf_get_reg_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cs_reg_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_REG_INFO, val_len_ptr);
}

static inline mipc_nw_cell_type_const_enum mipc_nw_get_cs_cnf_get_cell_type(mipc_msg_t *msg_ptr, mipc_nw_cell_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cell_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CS_CNF_T_CELL_TYPE, def_val);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_get_cs_cnf_get_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_get_cs_cnf_get_gsm_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_GSM_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_get_cs_cnf_get_umts_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_UMTS_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_get_cs_cnf_get_lte_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_LTE_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_get_cs_cnf_get_nr_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_NR_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_get_cs_cnf_get_cdma_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_CDMA_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_cs_reg_info_v1_struct4* mipc_nw_get_cs_cnf_get_reg_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cs_reg_info_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_REG_INFO_V1, val_len_ptr);
}

static inline mipc_nw_nr_cell_v2_struct8* mipc_nw_get_cs_cnf_get_nr_cell_info_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v2_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_NR_CELL_INFO_V2, val_len_ptr);
}

static inline mipc_nw_cdma_cell_v1_struct4* mipc_nw_get_cs_cnf_get_cdma_cell_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CS_CNF_T_CDMA_CELL_INFO_V1, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_get_band_mode_req_add_option(mipc_msg_t *msg_ptr, enum mipc_nw_band_option_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_BAND_MODE_REQ_T_OPTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sys_cap_gsm_band_const_enum mipc_nw_get_band_mode_cnf_get_gsm_band(mipc_msg_t *msg_ptr, mipc_sys_cap_gsm_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_gsm_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BAND_MODE_CNF_T_GSM_BAND, def_val);
}

static inline mipc_sys_cap_umts_band_const_enum mipc_nw_get_band_mode_cnf_get_umts_band(mipc_msg_t *msg_ptr, mipc_sys_cap_umts_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_umts_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_BAND_MODE_CNF_T_UMTS_BAND, def_val);
}

static inline mipc_sys_lte_band_struct4* mipc_nw_get_band_mode_cnf_get_lte_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_lte_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BAND_MODE_CNF_T_LTE_BAND, val_len_ptr);
}

static inline mipc_sys_nr_band_struct4* mipc_nw_get_band_mode_cnf_get_nr_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_nr_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BAND_MODE_CNF_T_NR_BAND, val_len_ptr);
}

static inline mipc_sys_nr_band_struct4* mipc_nw_get_band_mode_cnf_get_nr_nsa_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_nr_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BAND_MODE_CNF_T_NR_NSA_BAND, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_band_mode_req_add_gsm_band(mipc_msg_t *msg_ptr, enum mipc_sys_cap_gsm_band_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_BAND_MODE_REQ_T_GSM_BAND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_band_mode_req_add_umts_band(mipc_msg_t *msg_ptr, enum mipc_sys_cap_umts_band_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_BAND_MODE_REQ_T_UMTS_BAND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_band_mode_req_add_lte_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_lte_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_BAND_MODE_REQ_T_LTE_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_band_mode_req_add_nr_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_nr_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_BAND_MODE_REQ_T_NR_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_band_mode_req_add_nr_nsa_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_nr_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_BAND_MODE_REQ_T_NR_NSA_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_channel_lock_req_add_ch_lock_info_list_count(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_LIST_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_channel_lock_req_add_ch_lock_info_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_channel_lock_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_channel_lock_req_add_ch_lock_info_tlv_array_v1(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_channel_lock_info_v1_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_channel_lock_req_add_ch_lock_info_list_v1(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_channel_lock_info_v1_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_nw_pol_info_struct4* mipc_nw_get_pol_capability_cnf_get_pol_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pol_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_POL_CAPABILITY_CNF_T_POL_INFO, val_len_ptr);
}

static inline mipc_nw_pol_info_v1_struct4* mipc_nw_get_pol_capability_cnf_get_pol_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pol_info_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_POL_CAPABILITY_CNF_T_POL_INFO_V1, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_prefer_rat_req_add_rat_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_PREFER_RAT_REQ_T_RAT_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_prefer_rat_req_add_rat_list(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_PREFER_RAT_REQ_T_RAT_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_get_operator_name_req_add_plmn_id(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_GET_OPERATOR_NAME_REQ_T_PLMN_ID, len, (const void *)value);
}

static inline mipc_nw_provider_struct4* mipc_nw_get_operator_name_cnf_get_provider_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_OPERATOR_NAME_CNF_T_PROVIDER_INFO, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_endc_config_req_add_endc_state(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ENDC_CONFIG_REQ_T_ENDC_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_lte_carrier_aggregation_switch_req_add_status(mipc_msg_t *msg_ptr, enum mipc_nw_lte_carrier_arrregation_switch_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_ps_cs_registration_state_roaming_type_req_add_ps_cs_reg_roaming_info(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_ps_cs_reg_roaming_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ_T_PS_CS_REG_ROAMING_INFO, len, (const void *)value);
}

static inline mipc_nw_lte_carrier_arrregation_switch_const_enum mipc_nw_get_lte_carrier_aggregation_switch_cnf_get_status(mipc_msg_t *msg_ptr, mipc_nw_lte_carrier_arrregation_switch_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_lte_carrier_arrregation_switch_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_CNF_T_STATUS, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_get_current_band_info_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_CURRENT_BAND_INFO_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_current_band_info_cnf_get_band_list_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_LIST_COUNT, def_val);
}

static inline uint8_t mipc_nw_get_current_band_info_cnf_get_band_list(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_LIST, def_val);
}

static inline uint8_t mipc_nw_get_current_band_info_cnf_get_band_width_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_WIDTH_COUNT, def_val);
}

static inline uint32_t mipc_nw_get_current_band_info_cnf_get_band_width_list(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_WIDTH_LIST, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_measurement_req_add_action(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_CELL_MEASUREMENT_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_cell_measurement_req_add_lte_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_lte_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CELL_MEASUREMENT_REQ_T_LTE_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_measurement_req_add_nr_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_nr_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CELL_MEASUREMENT_REQ_T_NR_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_measurement_req_add_scan_type(mipc_msg_t *msg_ptr, enum mipc_nw_cellmeasurement_scan_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_CELL_MEASUREMENT_REQ_T_SCAN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_cell_measurement_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_nw_cellmeasurement_bit_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_NW_CELL_MEASUREMENT_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_cell_measurement_cnf_get_cell_list_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_CELL_LIST_COUNT, def_val);
}

static inline mipc_nw_cellmeasurement_info_struct4* mipc_nw_cell_measurement_cnf_get_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cellmeasurement_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_CELL_LIST, val_len_ptr);
}

static inline mipc_cell_plmn_struct4* mipc_nw_cell_measurement_cnf_get_plmn_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_cell_plmn_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_PLMN_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_nw_cell_measurement_cnf_get_lte_cell_list_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_LIST_COUNT, def_val);
}

static inline mipc_nw_cellmeasurement_info_struct4* mipc_nw_cell_measurement_cnf_get_lte_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cellmeasurement_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_LIST, val_len_ptr);
}

static inline mipc_cell_plmn_struct4* mipc_nw_cell_measurement_cnf_get_lte_plmn_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_cell_plmn_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_PLMN_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_nw_cell_measurement_cnf_get_nr_cell_list_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_LIST_COUNT, def_val);
}

static inline mipc_nw_cellmeasurement_info_struct4* mipc_nw_cell_measurement_cnf_get_nr_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cellmeasurement_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_LIST, val_len_ptr);
}

static inline mipc_cell_plmn_struct4* mipc_nw_cell_measurement_cnf_get_nr_plmn_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_cell_plmn_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_PLMN_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cell_band_bandwidth_struct4* mipc_nw_cell_measurement_cnf_get_lte_cell_band_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_BAND_LIST, val_len_ptr);
}

static inline mipc_nw_cell_band_bandwidth_struct4* mipc_nw_cell_measurement_cnf_get_nr_cell_band_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_BAND_LIST, val_len_ptr);
}

static inline mipc_nw_cellmeasurement_info_struct4* mipc_nw_cell_measurement_cnf_get_lte_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cellmeasurement_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_measurement_cnf_get_lte_plmn_tlv_array(mipc_msg_t *msg_ptr, uint16_t type, uint16_t array_size1, uint16_t array_size2, mipc_cell_plmn_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    uint8_t idx1, idx2;
    uint8_t get_array_size1 = 0, get_array_size2 = 0;
    mipc_msg_tlv_t *tlv_ptr1;
    get_array_size1 = mipc_msg_get_tlv_num(msg_ptr, type);
    if(get_array_size1 > array_size1) return MIPC_MSG_API_RESULT_FAIL;
    for (idx1 = 0; idx1 < array_size1; idx1++) {
        tlv_ptr1 = mipc_msg_get_idx(msg_ptr, type, idx1);
        get_array_size2 = mipc_msg_get_tlv_num(msg_ptr, MIPC_MSG_GET_TLV_TYPE(tlv_ptr1));
        if(get_array_size2 > array_size2) return MIPC_MSG_API_RESULT_FAIL;
        for (idx2 = 0; idx2 < array_size2; idx2++) {
            uint16_t val_len = 0;
            mipc_cell_plmn_struct4 *val_ptr = NULL;
            val_ptr = (mipc_cell_plmn_struct4 *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_MSG_GET_TLV_TYPE(tlv_ptr1), idx2, &val_len);
            memcpy((void *)(value + (idx1*array_size2+idx2)), (void *)val_ptr, sizeof(mipc_cell_plmn_struct4));
        }
    }
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_cellmeasurement_info_struct4* mipc_nw_cell_measurement_cnf_get_nr_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cellmeasurement_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_measurement_cnf_get_nr_plmn_tlv_array(mipc_msg_t *msg_ptr, uint16_t type, uint16_t array_size1, uint16_t array_size2, mipc_cell_plmn_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    uint8_t idx1, idx2;
    uint8_t get_array_size1 = 0, get_array_size2 = 0;
    mipc_msg_tlv_t *tlv_ptr1;
    get_array_size1 = mipc_msg_get_tlv_num(msg_ptr, type);
    if(get_array_size1 > array_size1) return MIPC_MSG_API_RESULT_FAIL;
    for (idx1 = 0; idx1 < array_size1; idx1++) {
        tlv_ptr1 = mipc_msg_get_idx(msg_ptr, type, idx1);
        get_array_size2 = mipc_msg_get_tlv_num(msg_ptr, MIPC_MSG_GET_TLV_TYPE(tlv_ptr1));
        if(get_array_size2 > array_size2) return MIPC_MSG_API_RESULT_FAIL;
        for (idx2 = 0; idx2 < array_size2; idx2++) {
            uint16_t val_len = 0;
            mipc_cell_plmn_struct4 *val_ptr = NULL;
            val_ptr = (mipc_cell_plmn_struct4 *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_MSG_GET_TLV_TYPE(tlv_ptr1), idx2, &val_len);
            memcpy((void *)(value + (idx1*array_size2+idx2)), (void *)val_ptr, sizeof(mipc_cell_plmn_struct4));
        }
    }
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_cell_band_bandwidth_struct4* mipc_nw_cell_measurement_cnf_get_lte_cell_band_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_BAND_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cell_band_bandwidth_struct4* mipc_nw_cell_measurement_cnf_get_nr_cell_band_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_BAND_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cellmeasurement_info_v1_struct8* mipc_nw_cell_measurement_cnf_get_lte_cell_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cellmeasurement_info_v1_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cellmeasurement_info_v1_struct8* mipc_nw_cell_measurement_cnf_get_nr_cell_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cellmeasurement_info_v1_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cell_band_bandwidth_v1_struct4* mipc_nw_cell_measurement_cnf_get_lte_cell_band_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_BAND_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cell_band_bandwidth_v1_struct4* mipc_nw_cell_measurement_cnf_get_nr_cell_band_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_BAND_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_lte_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_lte_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_LTE_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_nr_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_nr_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_lte_cell_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_LTE_CELL_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_lte_cell_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_lte_cell_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_LTE_CELL_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_nr_cell_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_CELL_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_nr_cell_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_nr_cell_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_CELL_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_lte_cell_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_lte_cell_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_nr_cell_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_nr_cell_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_cell_band_white_list_lock_req_add_nr_cell_v2_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_nr_cell_v2_struct8 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_get_cell_band_bandwidth_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_cell_band_bandwidth_cnf_get_num_serving_cell(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NUM_SERVING_CELL, def_val);
}

static inline mipc_nw_cell_band_bandwidth_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_lte_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_cell_band_bandwidth_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_nr_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cell_band_bandwidth_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_SERVING_CELL_LIST, val_len_ptr);
}

static inline uint8_t mipc_nw_get_cell_band_bandwidth_cnf_get_lte_dl_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_DL_SERVING_CELL_COUNT, def_val);
}

static inline uint8_t mipc_nw_get_cell_band_bandwidth_cnf_get_lte_ul_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_UL_SERVING_CELL_COUNT, def_val);
}

static inline uint8_t mipc_nw_get_cell_band_bandwidth_cnf_get_nr_dl_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_DL_SERVING_CELL_COUNT, def_val);
}

static inline uint8_t mipc_nw_get_cell_band_bandwidth_cnf_get_nr_ul_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_UL_SERVING_CELL_COUNT, def_val);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_lte_dl_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_DL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_lte_ul_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_UL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_nr_dl_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_DL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_nr_ul_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_UL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_lte_dl_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_DL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_lte_ul_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_UL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_nr_dl_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_DL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_nr_ul_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_UL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_umts_cell_frequency_info_struct4* mipc_nw_get_cell_band_bandwidth_cnf_get_umts_cell_frequency_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_frequency_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_UMTS_CELL_FREQUENCY_INFO, val_len_ptr);
}

static inline uint8_t mipc_nw_get_nr_cnf_get_nr_opt(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_CNF_T_NR_OPT, def_val);
}

static inline mipc_nw_srxlev_info_struct4* mipc_nw_get_srxlev_cnf_get_lte_srxlev_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_srxlev_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SRXLEV_CNF_T_LTE_SRXLEV_INFO, val_len_ptr);
}

static inline mipc_nw_srxlev_info_struct4* mipc_nw_get_srxlev_cnf_get_nr_srxlev_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_srxlev_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SRXLEV_CNF_T_NR_SRXLEV_INFO, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_roaming_mode_req_add_roaming_mode(mipc_msg_t *msg_ptr, enum mipc_nw_roaming_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ROAMING_MODE_REQ_T_ROAMING_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_roaming_mode_const_enum mipc_nw_get_roaming_mode_cnf_get_roaming_mode(mipc_msg_t *msg_ptr, mipc_nw_roaming_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_roaming_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ROAMING_MODE_CNF_T_ROAMING_MODE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_urc_enable_req_add_type(mipc_msg_t *msg_ptr, enum mipc_nw_ind_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_URC_ENABLE_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_urc_enable_req_add_enable(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_URC_ENABLE_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_suggested_plmn_list_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_suggested_plmn_list_req_add_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_suggested_plmn_list_req_add_timer(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_suggested_plmn_list_cnf_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_COUNT, def_val);
}

static inline mipc_nw_suggested_struct4* mipc_nw_get_suggested_plmn_list_cnf_get_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_suggested_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_get_suggested_plmn_list_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_suggested_struct4* mipc_nw_get_suggested_plmn_list_cnf_get_plmn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_suggested_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_PLMN_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_suggested_v1_struct4* mipc_nw_get_suggested_plmn_list_cnf_get_plmn_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_suggested_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_PLMN_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_search_store_frequency_info_req_add_oper(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_OPER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_search_store_frequency_info_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_search_store_frequency_info_req_add_plmn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_PLMN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_search_store_frequency_info_req_add_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_search_store_frequency_info_req_add_arfcn(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_arfcn_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_ARFCN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_search_store_frequency_info_req_add_arfcn_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_arfcn_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_select_femtocell_req_add_plmn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_PLMN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_select_femtocell_req_add_act(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_ACT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_select_femtocell_req_add_csg_id(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_CSG_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_config_a2_offset_req_add_offset(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CONFIG_A2_OFFSET_REQ_T_OFFSET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_config_a2_offset_req_add_thresh_bound(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CONFIG_A2_OFFSET_REQ_T_THRESH_BOUND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_config_b1_offset_req_add_offset(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CONFIG_B1_OFFSET_REQ_T_OFFSET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_config_b1_offset_req_add_thresh_bound(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CONFIG_B1_OFFSET_REQ_T_THRESH_BOUND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_report_anbr_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REPORT_ANBR_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_report_anbr_req_add_ebi(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REPORT_ANBR_REQ_T_EBI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_report_anbr_req_add_is_ul(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REPORT_ANBR_REQ_T_IS_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_report_anbr_req_add_beare_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REPORT_ANBR_REQ_T_BEARE_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_report_anbr_req_add_bitrate(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_NW_SET_REPORT_ANBR_REQ_T_BITRATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_report_anbr_req_add_pdu_session_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REPORT_ANBR_REQ_T_PDU_SESSION_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_report_anbr_req_add_ext_param(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_REPORT_ANBR_REQ_T_EXT_PARAM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_network_event_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NETWORK_EVENT_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_enable_ca_plus_filter_req_add_enable(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ENABLE_CA_PLUS_FILTER_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_femtocell_list_cnf_get_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_CELL_COUNT, def_val);
}

static inline mipc_nw_femtocell_info_struct4* mipc_nw_get_femtocell_list_cnf_get_femtocell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_femtocell_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FEMTOCELL_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_get_femtocell_list_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_femtocell_info_struct4* mipc_nw_get_femtocell_list_cnf_get_femtocell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_femtocell_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FEMTOCELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_femtocell_info_v1_struct4* mipc_nw_get_femtocell_list_cnf_get_femtocell_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_femtocell_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FEMTOCELL_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_pseudo_cell_mode_req_add_apc_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_APC_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_pseudo_cell_mode_req_add_urc_enable(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_URC_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_pseudo_cell_mode_req_add_timer(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_pseudo_cell_info_cnf_get_apc_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_APC_MODE, def_val);
}

static inline uint8_t mipc_nw_get_pseudo_cell_info_cnf_get_urc_enable(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_URC_ENABLE, def_val);
}

static inline uint16_t mipc_nw_get_pseudo_cell_info_cnf_get_timer(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_TIMER, def_val);
}

static inline uint8_t mipc_nw_get_pseudo_cell_info_cnf_get_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_CELL_COUNT, def_val);
}

static inline mipc_nw_pseudocell_info_struct4* mipc_nw_get_pseudo_cell_info_cnf_get_pseudocell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pseudocell_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_get_pseudo_cell_info_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_pseudocell_info_struct4* mipc_nw_get_pseudo_cell_info_cnf_get_pseudocell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pseudocell_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_pseudocell_info_v1_struct8* mipc_nw_get_pseudo_cell_info_cnf_get_pseudocell_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pseudocell_info_v1_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_roaming_enable_req_add_protocol_index(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ROAMING_ENABLE_REQ_T_PROTOCOL_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_roaming_enable_req_add_dom_voice(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ROAMING_ENABLE_REQ_T_DOM_VOICE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_roaming_enable_req_add_dom_data(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ROAMING_ENABLE_REQ_T_DOM_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_roaming_enable_req_add_int_voice(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ROAMING_ENABLE_REQ_T_INT_VOICE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_roaming_enable_req_add_int_data(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ROAMING_ENABLE_REQ_T_INT_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_roaming_enable_req_add_lte_data(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ROAMING_ENABLE_REQ_T_LTE_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_roaming_enable_cnf_get_protocol_index(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ROAMING_ENABLE_CNF_T_PROTOCOL_INDEX, def_val);
}

static inline uint8_t mipc_nw_get_roaming_enable_cnf_get_dom_voice(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ROAMING_ENABLE_CNF_T_DOM_VOICE, def_val);
}

static inline uint8_t mipc_nw_get_roaming_enable_cnf_get_dom_data(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ROAMING_ENABLE_CNF_T_DOM_DATA, def_val);
}

static inline uint8_t mipc_nw_get_roaming_enable_cnf_get_int_voice(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ROAMING_ENABLE_CNF_T_INT_VOICE, def_val);
}

static inline uint8_t mipc_nw_get_roaming_enable_cnf_get_int_data(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ROAMING_ENABLE_CNF_T_INT_DATA, def_val);
}

static inline uint8_t mipc_nw_get_roaming_enable_cnf_get_lte_data(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ROAMING_ENABLE_CNF_T_LTE_DATA, def_val);
}

static inline uint16_t mipc_nw_get_roaming_enable_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_ROAMING_ENABLE_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_femtocell_system_selection_mode_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_query_femtocell_system_selection_mode_cnf_get_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF_T_MODE, def_val);
}

static inline uint16_t mipc_nw_query_femtocell_system_selection_mode_cnf_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF_T_FAIL_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_nw_ind_report_level_req_add_ps_state_level(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ_T_PS_STATE_LEVEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nw_ind_report_level_req_add_cs_state_level(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ_T_CS_STATE_LEVEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_disable_2g_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_DISABLE_2G_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_disable_2g_cnf_get_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_DISABLE_2G_CNF_T_MODE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_smart_rat_switch_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_nw_rat_switch_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SMART_RAT_SWITCH_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_smart_rat_switch_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SMART_RAT_SWITCH_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_smart_rat_switch_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_nw_rat_switch_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_SMART_RAT_SWITCH_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_smart_rat_switch_cnf_get_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_SMART_RAT_SWITCH_CNF_T_STATE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_vss_antenna_conf_req_add_antenna_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_VSS_ANTENNA_CONF_REQ_T_ANTENNA_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_vss_antenna_conf_cnf_get_antenna_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_VSS_ANTENNA_CONF_CNF_T_ANTENNA_TYPE, def_val);
}

static inline int32_t mipc_nw_vss_antenna_info_cnf_get_primary_antenna_rssi(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_VSS_ANTENNA_INFO_CNF_T_PRIMARY_ANTENNA_RSSI, def_val);
}

static inline int32_t mipc_nw_vss_antenna_info_cnf_get_relative_phase(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_VSS_ANTENNA_INFO_CNF_T_RELATIVE_PHASE, def_val);
}

static inline int32_t mipc_nw_vss_antenna_info_cnf_get_secondary_antenna_rssi(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_VSS_ANTENNA_INFO_CNF_T_SECONDARY_ANTENNA_RSSI, def_val);
}

static inline int32_t mipc_nw_vss_antenna_info_cnf_get_phase1(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_VSS_ANTENNA_INFO_CNF_T_PHASE1, def_val);
}

static inline int32_t mipc_nw_vss_antenna_info_cnf_get_rx_state_0(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_VSS_ANTENNA_INFO_CNF_T_RX_STATE_0, def_val);
}

static inline int32_t mipc_nw_vss_antenna_info_cnf_get_rx_state_1(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_VSS_ANTENNA_INFO_CNF_T_RX_STATE_1, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_radio_capability_req_add_radio_capability(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_RADIO_CAPABILITY_REQ_T_RADIO_CAPABILITY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_radio_capability_req_add_switch_mode_by_data(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RADIO_CAPABILITY_REQ_T_SWITCH_MODE_BY_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cdma_roaming_preference_req_add_roaming_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_REQ_T_ROAMING_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_cdma_roaming_preference_cnf_get_roaming_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_CNF_T_ROAMING_TYPE, def_val);
}

static inline uint8_t mipc_nw_get_barring_info_cnf_get_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_MODE, def_val);
}

static inline uint8_t mipc_nw_get_barring_info_cnf_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT, def_val);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_barring_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST, val_len_ptr);
}

static inline uint8_t mipc_nw_get_barring_info_cnf_get_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_RAT, def_val);
}

static inline uint8_t mipc_nw_get_barring_info_cnf_get_count_umts(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT_UMTS, def_val);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_barring_list_umts(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST_UMTS, val_len_ptr);
}

static inline uint8_t mipc_nw_get_barring_info_cnf_get_count_lte(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT_LTE, def_val);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_barring_list_lte(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST_LTE, val_len_ptr);
}

static inline uint8_t mipc_nw_get_barring_info_cnf_get_count_nr(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT_NR, def_val);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_barring_list_nr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST_NR, val_len_ptr);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_barring_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_umts_barring_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_UMTS_BARRING_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_lte_barring_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_LTE_BARRING_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_get_barring_info_cnf_get_nr_barring_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_NR_BARRING_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_v1_struct4* mipc_nw_get_barring_info_cnf_get_barring_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_v1_struct4* mipc_nw_get_barring_info_cnf_get_umts_barring_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_UMTS_BARRING_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_v1_struct4* mipc_nw_get_barring_info_cnf_get_lte_barring_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_LTE_BARRING_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_v1_struct4* mipc_nw_get_barring_info_cnf_get_nr_barring_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_BARRING_INFO_CNF_T_NR_BARRING_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_nw_get_ehrpd_info_cnf_get_rev(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_REV, def_val);
}

static inline uint16_t mipc_nw_get_ehrpd_info_cnf_get_mcc(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_MCC, def_val);
}

static inline uint16_t mipc_nw_get_ehrpd_info_cnf_get_mnc(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_MNC, def_val);
}

static inline uint16_t mipc_nw_get_ehrpd_info_cnf_get_nid(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_NID, def_val);
}

static inline uint16_t mipc_nw_get_ehrpd_info_cnf_get_sid(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_SID, def_val);
}

static inline uint16_t mipc_nw_get_ehrpd_info_cnf_get_bs_id(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_BS_ID, def_val);
}

static inline uint32_t mipc_nw_get_ehrpd_info_cnf_get_bs_lat(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_BS_LAT, def_val);
}

static inline uint32_t mipc_nw_get_ehrpd_info_cnf_get_bs_long(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_BS_LONG, def_val);
}

static inline char * mipc_nw_get_ehrpd_info_cnf_get_sector_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_SECTOR_ID, val_len_ptr);
}

static inline char * mipc_nw_get_ehrpd_info_cnf_get_subnet_mask(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_EHRPD_INFO_CNF_T_SUBNET_MASK, val_len_ptr);
}

static inline uint32_t mipc_nw_get_egmss_cnf_get_rat(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_EGMSS_CNF_T_RAT, def_val);
}

static inline uint32_t mipc_nw_get_egmss_cnf_get_mcc(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_EGMSS_CNF_T_MCC, def_val);
}

static inline uint32_t mipc_nw_get_egmss_cnf_get_status(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_EGMSS_CNF_T_STATUS, def_val);
}

static inline uint32_t mipc_nw_get_egmss_cnf_get_cur_reported_rat(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_EGMSS_CNF_T_CUR_REPORTED_RAT, def_val);
}

static inline uint32_t mipc_nw_get_egmss_cnf_get_is_home_country(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_EGMSS_CNF_T_IS_HOME_COUNTRY, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cache_endc_connect_mode_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cache_endc_connect_mode_req_add_timer1(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_TIMER1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cache_endc_connect_mode_req_add_timer2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_TIMER2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_ps_test_mode_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_nw_ps_test_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_PS_TEST_MODE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_ps_test_mode_req_add_profile(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_PS_TEST_MODE_REQ_T_PROFILE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_ps_test_mode_const_enum mipc_nw_get_ps_test_mode_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_nw_ps_test_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ps_test_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_PS_TEST_MODE_CNF_T_MODE, def_val);
}

static inline uint32_t mipc_nw_get_ps_test_mode_cnf_get_profile(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_PS_TEST_MODE_CNF_T_PROFILE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_report_criteria_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_report_criteria_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_nw_signal_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_report_criteria_req_add_signal_measurement(mipc_msg_t *msg_ptr, enum mipc_nw_signal_measurement_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_SIGNAL_MEASUREMENT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_report_criteria_req_add_hysteresis_ms(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_HYSTERESIS_MS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_report_criteria_req_add_hysteresis_db(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_HYSTERESIS_DB, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_report_criteria_req_add_threshold(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_threshold_array_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_THRESHOLD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_signal_report_criteria_req_add_threshold_v1(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_threshold_array_v1_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_THRESHOLD_V1, len, (const void *)value);
}

static inline mipc_nw_ecainfo_struct4* mipc_nw_get_ecainfo_cnf_get_ecainfo(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ecainfo_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_ECAINFO_CNF_T_ECAINFO, val_len_ptr);
}

static inline mipc_nw_tx_struct4* mipc_nw_get_activity_info_cnf_get_tx(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_tx_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_ACTIVITY_INFO_CNF_T_TX, val_len_ptr);
}

static inline uint32_t mipc_nw_get_activity_info_cnf_get_rx(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_ACTIVITY_INFO_CNF_T_RX, def_val);
}

static inline uint32_t mipc_nw_get_activity_info_cnf_get_sleep_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_ACTIVITY_INFO_CNF_T_SLEEP_TIME, def_val);
}

static inline uint32_t mipc_nw_get_activity_info_cnf_get_idle_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_ACTIVITY_INFO_CNF_T_IDLE_TIME, def_val);
}

static inline uint8_t mipc_nw_get_activity_info_cnf_get_num_tx_levels(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ACTIVITY_INFO_CNF_T_NUM_TX_LEVELS, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_ca_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_nw_set_ca_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CA_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_ca_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_nw_ca_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CA_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_lte_rrc_state_cnf_get_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_RRC_STATE_CNF_T_MODE, def_val);
}

static inline mipc_lte_rrc_state_const_enum mipc_nw_get_lte_rrc_state_cnf_get_state(mipc_msg_t *msg_ptr, mipc_lte_rrc_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_lte_rrc_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_RRC_STATE_CNF_T_STATE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_get_lte_1xrtt_cell_list_req_add_type(mipc_msg_t *msg_ptr, enum mipc_cell_list_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_LTE_1XRTT_CELL_LIST_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_lte_1xrtt_cell_list_cnf_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_COUNT, def_val);
}

static inline mipc_1xrtt_cell_info_struct4* mipc_nw_get_lte_1xrtt_cell_list_cnf_get_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_1xrtt_cell_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_CELL_LIST, val_len_ptr);
}

static inline mipc_1xrtt_cell_info_struct4* mipc_nw_get_lte_1xrtt_cell_list_cnf_get_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_1xrtt_cell_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_get_ca_link_capability_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_ca_link_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_CA_LINK_CAPABILITY_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_ca_link_capability_cnf_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_COUNT, def_val);
}

static inline mipc_band_combo_info_struct4* mipc_nw_get_ca_link_capability_cnf_get_band_combo(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_band_combo_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_BAND_COMBO, val_len_ptr);
}

static inline mipc_band_combo_info_struct4* mipc_nw_get_ca_link_capability_cnf_get_band_combo_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_band_combo_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_BAND_COMBO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_get_ca_link_enable_status_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_ca_link_enable_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_ca_link_enable_status_req_add_band_combo(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ_T_BAND_COMBO, len, (const void *)value);
}

static inline mipc_band_combo_enable_status_const_enum mipc_nw_get_ca_link_enable_status_cnf_get_status(mipc_msg_t *msg_ptr, mipc_band_combo_enable_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_band_combo_enable_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CA_LINK_ENABLE_STATUS_CNF_T_STATUS, def_val);
}

static inline mipc_tm9_enable_status_const_enum mipc_nw_get_tm9_enable_status_cnf_get_tm9_fdd_setting(mipc_msg_t *msg_ptr, mipc_tm9_enable_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_tm9_enable_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_FDD_SETTING, def_val);
}

static inline mipc_tm9_enable_status_const_enum mipc_nw_get_tm9_enable_status_cnf_get_tm9_tdd_setting(mipc_msg_t *msg_ptr, mipc_tm9_enable_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_tm9_enable_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_TDD_SETTING, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_tm9_enable_status_req_add_type(mipc_msg_t *msg_ptr, enum mipc_tm9_setting_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_TM9_ENABLE_STATUS_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_tm9_enable_status_req_add_status(mipc_msg_t *msg_ptr, enum mipc_tm9_enable_status_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_TM9_ENABLE_STATUS_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_omadm_conf_req_add_type(mipc_msg_t *msg_ptr, enum mipc_omadm_node_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_OMADM_CONF_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_nw_get_omadm_conf_cnf_get_node_value(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_OMADM_CONF_CNF_T_NODE_VALUE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_omadm_conf_req_add_type(mipc_msg_t *msg_ptr, enum mipc_omadm_node_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_OMADM_CONF_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_omadm_conf_req_add_node_value(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_OMADM_CONF_REQ_T_NODE_VALUE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_get_ca_band_mode_req_add_primary_id(mipc_msg_t *msg_ptr, int32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int32(msg_ptr, MIPC_NW_GET_CA_BAND_MODE_REQ_T_PRIMARY_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_ca_band_struct4* mipc_nw_get_ca_band_mode_cnf_get_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ca_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CA_BAND_MODE_CNF_T_BAND, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_ca_link_enable_status_req_add_list_type(mipc_msg_t *msg_ptr, enum mipc_ca_comb_list_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_LIST_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_ca_link_enable_status_req_add_ca_comb_list(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_CA_COMB_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_ca_link_enable_status_req_add_link_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_LINK_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_reg_state(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_REG_STATE, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_mcc(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_MCC, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_mnc(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_MNC, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_cell_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_CELL_ID, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_band(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_BAND, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_bandwidth(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_BANDWIDTH, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_ul_channel(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_UL_CHANNEL, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_dl_channel(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_DL_CHANNEL, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_rssi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_RSSI, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_rsrp(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_RSRP, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_rsrq(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_RSRQ, def_val);
}

static inline uint32_t mipc_nw_get_lte_data_cnf_get_tx_power(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_LTE_DATA_CNF_T_TX_POWER, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_tuw_timer_length_req_add_tuw_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_TUW_TIMER_LENGTH_REQ_T_TUW_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_tuw_timer_length_req_add_tuw(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_tuw_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_TUW_TIMER_LENGTH_REQ_T_TUW, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_tuw_timer_length_req_add_tuw_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_tuw_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_tuw_timer_length_req_add_tuw_v1_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_tuw_info_v1_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint32_t mipc_nw_get_tuw_timer_length_cnf_get_tuw1(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_TUW_TIMER_LENGTH_CNF_T_TUW1, def_val);
}

static inline uint32_t mipc_nw_get_tuw_timer_length_cnf_get_tuw2(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_TUW_TIMER_LENGTH_CNF_T_TUW2, def_val);
}

static inline uint32_t mipc_nw_get_tuw_timer_length_cnf_get_tuw3(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_TUW_TIMER_LENGTH_CNF_T_TUW3, def_val);
}

static inline uint8_t mipc_nw_get_5guw_info_cnf_get_display_5guw(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_5GUW_INFO_CNF_T_DISPLAY_5GUW, def_val);
}

static inline uint8_t mipc_nw_get_5guw_info_cnf_get_on_n77_band(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_5GUW_INFO_CNF_T_ON_N77_BAND, def_val);
}

static inline uint8_t mipc_nw_get_5guw_info_cnf_get_on_fr2_band(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_5GUW_INFO_CNF_T_ON_FR2_BAND, def_val);
}

static inline uint8_t mipc_nw_get_5guw_info_cnf_get_5guw_allowed(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_5GUW_INFO_CNF_T_5GUW_ALLOWED, def_val);
}

static inline int32_t mipc_nw_get_nr_ca_band_cnf_get_is_endc(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_GET_NR_CA_BAND_CNF_T_IS_ENDC, def_val);
}

static inline uint8_t mipc_nw_get_nr_ca_band_cnf_get_band_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_CA_BAND_CNF_T_BAND_NUM, def_val);
}

static inline mipc_nr_ca_band_struct4* mipc_nw_get_nr_ca_band_cnf_get_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nr_ca_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_NR_CA_BAND_CNF_T_BAND, val_len_ptr);
}

static inline int32_t mipc_nw_get_nr_scs_cnf_get_scs(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_GET_NR_SCS_CNF_T_SCS, def_val);
}

static inline mipc_physical_channel_info_v1_struct4* mipc_nw_get_physical_channel_configs_cnf_get_physical_channel_configs_list_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_physical_channel_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF_T_PHYSICAL_CHANNEL_CONFIGS_LIST_V1, index, val_len_ptr);
}

static inline mipc_physical_channel_info_v2_struct4* mipc_nw_get_physical_channel_configs_cnf_get_physical_channel_configs_v2_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_physical_channel_info_v2_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF_T_PHYSICAL_CHANNEL_CONFIGS_V2_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_os_id_update_req_add_os_id_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_os_id_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_pref_nssai_req_add_preferred_nssai_3gpp_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_s_nssai_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_pref_nssai_req_add_preferred_nssai_non3gpp_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_s_nssai_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_s_nssai_struct_struct4* mipc_nw_set_pref_nssai_cnf_get_preferred_nssai_3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SET_PREF_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_LIST, index, val_len_ptr);
}

static inline mipc_s_nssai_struct_struct4* mipc_nw_set_pref_nssai_cnf_get_preferred_nssai_non3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SET_PREF_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_default_nssai_req_add_default_configured_nssai_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_s_nssai_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_s_nssai_struct_struct4* mipc_nw_set_default_nssai_cnf_get_default_configured_nssai_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SET_DEFAULT_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_get_nssai_req_add_nssai_type(mipc_msg_t *msg_ptr, enum mipc_nssai_type_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_NSSAI_REQ_T_NSSAI_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_nssai_req_add_plmn_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_GET_NSSAI_REQ_T_PLMN_ID, len, (const void *)value);
}

static inline mipc_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_default_configured_nssai_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_LIST, index, val_len_ptr);
}

static inline mipc_rejected_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_rejected_nssai_3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_rejected_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_LIST, index, val_len_ptr);
}

static inline mipc_rejected_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_rejected_nssai_non3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_rejected_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_LIST, index, val_len_ptr);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_configured_nssai_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_LIST, index, val_len_ptr);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_allowed_nssai_3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_LIST, index, val_len_ptr);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_allowed_nssai_non3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_LIST, index, val_len_ptr);
}

static inline mipc_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_preferred_nssai_3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_LIST, index, val_len_ptr);
}

static inline mipc_s_nssai_struct_struct4* mipc_nw_get_nssai_cnf_get_preferred_nssai_non3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_5guc_req_add_refresh_timer_length(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_5GUC_REQ_T_REFRESH_TIMER_LENGTH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_5guc_req_add_nsa_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_nr_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_5GUC_REQ_T_NSA_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_5guc_req_add_sa_band(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_nr_band_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_5GUC_REQ_T_SA_BAND, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_5guc_req_add_bw_check_enable(mipc_msg_t *msg_ptr, enum mipc_nw_bw_check_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_5GUC_REQ_T_BW_CHECK_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_5guc_req_add_bw_check_threshold(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_5GUC_REQ_T_BW_CHECK_THRESHOLD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_nw_get_5guc_setting_cnf_get_refresh_timer_length(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_5GUC_SETTING_CNF_T_REFRESH_TIMER_LENGTH, def_val);
}

static inline mipc_sys_nr_band_struct4* mipc_nw_get_5guc_setting_cnf_get_nsa_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_nr_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_5GUC_SETTING_CNF_T_NSA_BAND, val_len_ptr);
}

static inline mipc_sys_nr_band_struct4* mipc_nw_get_5guc_setting_cnf_get_sa_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_nr_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_5GUC_SETTING_CNF_T_SA_BAND, val_len_ptr);
}

static inline mipc_nw_bw_check_const_enum mipc_nw_get_5guc_setting_cnf_get_bw_check_enable(mipc_msg_t *msg_ptr, mipc_nw_bw_check_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_bw_check_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_5GUC_SETTING_CNF_T_BW_CHECK_ENABLE, def_val);
}

static inline uint32_t mipc_nw_get_5guc_setting_cnf_get_bw_check_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_5GUC_SETTING_CNF_T_BW_CHECK_THRESHOLD, def_val);
}

static inline mipc_nw_5guc_state_const_enum mipc_nw_get_5guc_info_cnf_get_display_5guc(mipc_msg_t *msg_ptr, mipc_nw_5guc_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_5guc_state_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_5GUC_INFO_CNF_T_DISPLAY_5GUC, def_val);
}

static inline mipc_nw_uc_band_const_enum mipc_nw_get_5guc_info_cnf_get_on_uc_band(mipc_msg_t *msg_ptr, mipc_nw_uc_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uc_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_5GUC_INFO_CNF_T_ON_UC_BAND, def_val);
}

static inline uint32_t mipc_nw_get_5guc_info_cnf_get_agg_bw(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_GET_5GUC_INFO_CNF_T_AGG_BW, def_val);
}

static inline char * mipc_nw_get_first_plmn_cnf_get_mcc(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_FIRST_PLMN_CNF_T_MCC, val_len_ptr);
}

static inline char * mipc_nw_get_first_plmn_cnf_get_mnc(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_FIRST_PLMN_CNF_T_MNC, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_ue_usage_setting_req_add_usage_setting(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UE_USAGE_SETTING_REQ_T_USAGE_SETTING, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_get_ue_usage_setting_cnf_get_usage_setting(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UE_USAGE_SETTING_CNF_T_USAGE_SETTING, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_status_req_add_status(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAG_STATUS_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_select_mode_req_add_select_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_SELECT_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_select_mode_req_add_plmn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_PLMN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_select_mode_req_add_cag_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_CAG_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_select_mode_req_add_act(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_ACT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_plmn_cag_info_struct4* mipc_nw_get_cag_list_cnf_get_plmn_cag_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_cag_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CAG_LIST_CNF_T_PLMN_CAG_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_allowed_mcc_list_req_add_action(mipc_msg_t *msg_ptr, enum mipc_nw_allowed_mcc_list_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ALLOWED_MCC_LIST_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_allowed_mcc_list_req_add_allowed_mcc_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_allowed_mcc_list_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_ALLOWED_MCC_LIST_REQ_T_ALLOWED_MCC_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_n3_power_state_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_n3_power_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_N3_POWER_STATE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_n3_gw_type_req_add_n3iwf(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_N3_GW_TYPE_REQ_T_N3IWF, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_n3_gw_type_req_add_tngf(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_N3_GW_TYPE_REQ_T_TNGF, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_n3_gw_type_req_add_wagf(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_N3_GW_TYPE_REQ_T_WAGF, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_edrx_setting_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_EDRX_SETTING_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_edrx_setting_req_add_act(mipc_msg_t *msg_ptr, enum mipc_nw_edrx_access_technology_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_EDRX_SETTING_REQ_T_ACT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_edrx_setting_req_add_requested_edrx_value(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_EDRX_SETTING_REQ_T_REQUESTED_EDRX_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_edrx_setting_req_add_requested_paging_time_window(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_EDRX_SETTING_REQ_T_REQUESTED_PAGING_TIME_WINDOW, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_edrx_access_technology_enum_const_enum mipc_nw_get_edrx_setting_cnf_get_act(mipc_msg_t *msg_ptr, mipc_nw_edrx_access_technology_enum_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_edrx_access_technology_enum_const_enum)mipc_msg_get_idx_uint8(msg_ptr, MIPC_NW_GET_EDRX_SETTING_CNF_T_ACT, def_val, index);
}

static inline uint8_t mipc_nw_get_edrx_setting_cnf_get_requested_edrx_value(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_NW_GET_EDRX_SETTING_CNF_T_REQUESTED_EDRX_VALUE, def_val, index);
}

static inline mipc_nw_edrx_access_technology_enum_const_enum mipc_nw_set_edrx_cnf_get_act(mipc_msg_t *msg_ptr, mipc_nw_edrx_access_technology_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_edrx_access_technology_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_EDRX_CNF_T_ACT, def_val);
}

static inline uint8_t mipc_nw_set_edrx_cnf_get_requested_edrx_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_EDRX_CNF_T_REQUESTED_EDRX_VALUE, def_val);
}

static inline uint8_t mipc_nw_set_edrx_cnf_get_nw_provided_edrx_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_EDRX_CNF_T_NW_PROVIDED_EDRX_VALUE, def_val);
}

static inline uint8_t mipc_nw_set_edrx_cnf_get_paging_time_window(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_EDRX_CNF_T_PAGING_TIME_WINDOW, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_power_saving_mode_setting_req_add_psm_mode(mipc_msg_t *msg_ptr, enum mipc_nw_psm_mode_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_POWER_SAVING_MODE_SETTING_REQ_T_PSM_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_power_saving_mode_setting_req_add_requested_periodic_tau_value(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_POWER_SAVING_MODE_SETTING_REQ_T_REQUESTED_PERIODIC_TAU_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_power_saving_mode_setting_req_add_requested_active_time_value(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_POWER_SAVING_MODE_SETTING_REQ_T_REQUESTED_ACTIVE_TIME_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_psm_mode_enum_const_enum mipc_nw_get_power_saving_mode_setting_cnf_get_psm_mode(mipc_msg_t *msg_ptr, mipc_nw_psm_mode_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_psm_mode_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_POWER_SAVING_MODE_SETTING_CNF_T_PSM_MODE, def_val);
}

static inline uint8_t mipc_nw_get_power_saving_mode_setting_cnf_get_requested_periodic_tau_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_POWER_SAVING_MODE_SETTING_CNF_T_REQUESTED_PERIODIC_TAU_VALUE, def_val);
}

static inline uint8_t mipc_nw_get_power_saving_mode_setting_cnf_get_requested_active_time_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_POWER_SAVING_MODE_SETTING_CNF_T_REQUESTED_ACTIVE_TIME_VALUE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_mobile_initiated_connection_only_mode_req_add_requested_mico_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ_T_REQUESTED_MICO_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_mobile_initiated_connection_only_mode_req_add_requested_active_time(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ_T_REQUESTED_ACTIVE_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_nw_set_mobile_initiated_connection_only_mode_cnf_get_current_mico_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_CURRENT_MICO_MODE, def_val);
}

static inline uint8_t mipc_nw_set_mobile_initiated_connection_only_mode_cnf_get_raai_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_RAAI_VALUE, def_val);
}

static inline uint8_t mipc_nw_set_mobile_initiated_connection_only_mode_cnf_get_sprt_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_SPRT_VALUE, def_val);
}

static inline uint8_t mipc_nw_set_mobile_initiated_connection_only_mode_cnf_get_allocated_active_time(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_ALLOCATED_ACTIVE_TIME, def_val);
}

static inline uint8_t mipc_nw_get_mobile_initiated_connection_only_mode_cnf_get_requested_mico_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_REQUESTED_MICO_MODE, def_val);
}

static inline uint8_t mipc_nw_get_mobile_initiated_connection_only_mode_cnf_get_current_mico_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_CURRENT_MICO_MODE, def_val);
}

static inline uint8_t mipc_nw_get_mobile_initiated_connection_only_mode_cnf_get_raai_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_RAAI_VALUE, def_val);
}

static inline uint8_t mipc_nw_get_mobile_initiated_connection_only_mode_cnf_get_sprt_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_SPRT_VALUE, def_val);
}

static inline uint8_t mipc_nw_get_mobile_initiated_connection_only_mode_cnf_get_requested_active_time(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_REQUESTED_ACTIVE_TIME, def_val);
}

static inline uint8_t mipc_nw_get_mobile_initiated_connection_only_mode_cnf_get_allocated_active_time(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_ALLOCATED_ACTIVE_TIME, def_val);
}

static inline mipc_lte_cc_meas_info_struct4* mipc_nw_get_latest_ca_measurement_cnf_get_lte_cc_meas_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_lte_cc_meas_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_LTE_CC_MEAS_LIST, index, val_len_ptr);
}

static inline mipc_nr_cc_meas_info_struct4* mipc_nw_get_latest_ca_measurement_cnf_get_nr_cc_meas_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nr_cc_meas_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_NR_CC_MEAS_LIST, index, val_len_ptr);
}

static inline mipc_lte_cc_meas_info_v1_struct4* mipc_nw_get_latest_ca_measurement_cnf_get_lte_cc_meas_tlv_array_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_lte_cc_meas_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_LTE_CC_MEAS_TLV_ARRAY_V1, index, val_len_ptr);
}

static inline mipc_nr_cc_meas_info_v1_struct4* mipc_nw_get_latest_ca_measurement_cnf_get_nr_cc_meas_tlv_array_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nr_cc_meas_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_NR_CC_MEAS_TLV_ARRAY_V1, index, val_len_ptr);
}

static inline uint8_t mipc_nw_get_current_cell_info_cnf_get_connect_act(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_CONNECT_ACT, def_val);
}

static inline mipc_wcdma_current_cell_info_struct4* mipc_nw_get_current_cell_info_cnf_get_wcdma_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_wcdma_current_cell_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_WCDMA_CELL_INFO, val_len_ptr);
}

static inline mipc_current_cell_info_struct4* mipc_nw_get_current_cell_info_cnf_get_lte_current_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_current_cell_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_LTE_CURRENT_CELL_LIST, index, val_len_ptr);
}

static inline mipc_current_cell_info_struct4* mipc_nw_get_current_cell_info_cnf_get_nr_current_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_current_cell_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_NR_CURRENT_CELL_LIST, index, val_len_ptr);
}

static inline mipc_current_cell_info_v1_struct4* mipc_nw_get_current_cell_info_cnf_get_lte_current_cell_tlv_array_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_current_cell_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_LTE_CURRENT_CELL_TLV_ARRAY_V1, index, val_len_ptr);
}

static inline mipc_current_cell_info_v1_struct4* mipc_nw_get_current_cell_info_cnf_get_nr_current_cell_tlv_array_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_current_cell_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_NR_CURRENT_CELL_TLV_ARRAY_V1, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_selection_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_nw_cag_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAG_SELECTION_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_selection_req_add_format(mipc_msg_t *msg_ptr, enum mipc_nw_cag_info_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAG_SELECTION_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_selection_req_add_cag_info(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_CAG_SELECTION_REQ_T_CAG_INFO, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cag_selection_req_add_act(mipc_msg_t *msg_ptr, enum mipc_nw_act_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAG_SELECTION_REQ_T_ACT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_act_type_const_enum mipc_nw_set_cag_selection_cnf_get_act(mipc_msg_t *msg_ptr, mipc_nw_act_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_act_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_CAG_SELECTION_CNF_T_ACT, def_val);
}

static inline mipc_nw_cag_mode_const_enum mipc_nw_get_cag_selection_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_nw_cag_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cag_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CAG_SELECTION_CNF_T_MODE, def_val);
}

static inline mipc_nw_cag_info_format_const_enum mipc_nw_get_cag_selection_cnf_get_format(mipc_msg_t *msg_ptr, mipc_nw_cag_info_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cag_info_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CAG_SELECTION_CNF_T_FORMAT, def_val);
}

static inline char * mipc_nw_get_cag_selection_cnf_get_cag_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CAG_SELECTION_CNF_T_CAG_INFO, val_len_ptr);
}

static inline mipc_nw_act_type_const_enum mipc_nw_get_cag_selection_cnf_get_act(mipc_msg_t *msg_ptr, mipc_nw_act_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_act_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CAG_SELECTION_CNF_T_ACT, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_auto_update_nitz_req_add_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_AUTO_UPDATE_NITZ_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_boolean_const_enum mipc_nw_get_auto_update_nitz_cnf_get_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_AUTO_UPDATE_NITZ_CNF_T_ENABLE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_tt_sa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TT_SA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_tf_sa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TF_SA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_ft_sa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FT_SA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_ff_sa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FF_SA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_tt_nsa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TT_NSA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_tf_nsa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TF_NSA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_ft_nsa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FT_NSA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_nrca_option_req_add_ff_nsa_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FF_NSA_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tt_sa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tf_sa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ft_sa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ff_sa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tt_nsa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tf_nsa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ft_nsa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ff_nsa_src_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tt_sa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tf_sa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ft_sa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ff_sa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tt_nsa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_tf_nsa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ft_nsa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_ff_nsa_option_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_cap_bc_list_req_add_is_xml_parsing_done(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CAP_BC_LIST_REQ_T_IS_XML_PARSING_DONE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_congestion_cfg_req_add_nw_congestion_cfg(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_congestion_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CONGESTION_CFG_REQ_T_NW_CONGESTION_CFG, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_congestion_cfg_req_add_nw_congestion_cfg_v1(mipc_msg_t *msg_ptr, uint16_t len, mipc_nw_congestion_info_v1_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_CONGESTION_CFG_REQ_T_NW_CONGESTION_CFG_V1, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_congestion_cfg_req_add_nw_congestion_cfg_v1_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_congestion_info_v1_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_nw_get_enwcfginfo_cnf_get_4x4mino_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ENWCFGINFO_CNF_T_4X4MINO_STATE, def_val);
}

static inline uint8_t mipc_nw_get_enwcfginfo_cnf_get_256qam_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ENWCFGINFO_CNF_T_256QAM_STATE, def_val);
}

static inline uint8_t mipc_nw_get_enwcfginfo_cnf_get_64qam_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ENWCFGINFO_CNF_T_64QAM_STATE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_barring_rlf_config_req_add_threshold(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_BARRING_RLF_CONFIG_REQ_T_THRESHOLD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_barring_rlf_config_req_add_watch_period(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_NW_SET_BARRING_RLF_CONFIG_REQ_T_WATCH_PERIOD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_barring_rlf_config_req_add_barring_time(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_BARRING_RLF_CONFIG_REQ_T_BARRING_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_rx_mimo_cap_req_add_ctrl_operation(mipc_msg_t *msg_ptr, enum mipc_rx_mimo_ctrl_operation_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RX_MIMO_CAP_REQ_T_CTRL_OPERATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_rx_mimo_cap_req_add_release_level(mipc_msg_t *msg_ptr, enum mipc_rx_mimo_release_level_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RX_MIMO_CAP_REQ_T_RELEASE_LEVEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_mipi_req_add_op(mipc_msg_t *msg_ptr, enum mipc_nw_mipi_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_MIPI_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_mipi_req_add_port(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_MIPI_REQ_T_PORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_mipi_req_add_rw_type(mipc_msg_t *msg_ptr, enum mipc_nw_mipi_rw_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_MIPI_REQ_T_RW_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_mipi_req_add_usid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_MIPI_REQ_T_USID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_mipi_req_add_addr(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_NW_MIPI_REQ_T_ADDR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_mipi_req_add_data(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_MIPI_REQ_T_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_mipi_cause_const_enum mipc_nw_mipi_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_nw_mipi_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_mipi_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_MIPI_CNF_T_CAUSE, def_val);
}

static inline mipc_nw_mipi_op_const_enum mipc_nw_mipi_cnf_get_op(mipc_msg_t *msg_ptr, mipc_nw_mipi_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_mipi_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_MIPI_CNF_T_OP, def_val);
}

static inline uint32_t mipc_nw_mipi_cnf_get_data(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_MIPI_CNF_T_DATA, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_bpi_req_add_op(mipc_msg_t *msg_ptr, enum mipc_nw_bpi_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_BPI_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_bpi_req_add_data(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_BPI_REQ_T_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_bpi_op_const_enum mipc_nw_bpi_cnf_get_op(mipc_msg_t *msg_ptr, mipc_nw_bpi_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_bpi_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_BPI_CNF_T_OP, def_val);
}

static inline uint32_t mipc_nw_bpi_cnf_get_data(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_BPI_CNF_T_DATA, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_endc_deact_req_add_deactivate_mode(mipc_msg_t *msg_ptr, enum mipc_nw_endc_deactivate_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ENDC_DEACT_REQ_T_DEACTIVATE_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_endc_deact_req_add_allow_scg_add(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ENDC_DEACT_REQ_T_ALLOW_SCG_ADD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_endc_deact_req_add_send_fake_a2_event(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ENDC_DEACT_REQ_T_SEND_FAKE_A2_EVENT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_endc_deactivate_mode_const_enum mipc_nw_get_endc_deact_cnf_get_deactivate_mode(mipc_msg_t *msg_ptr, mipc_nw_endc_deactivate_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_endc_deactivate_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ENDC_DEACT_CNF_T_DEACTIVATE_MODE, def_val);
}

static inline mipc_boolean_const_enum mipc_nw_get_endc_deact_cnf_get_allow_scg_add(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ENDC_DEACT_CNF_T_ALLOW_SCG_ADD, def_val);
}

static inline mipc_boolean_const_enum mipc_nw_get_endc_deact_cnf_get_send_fake_a2_event(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ENDC_DEACT_CNF_T_SEND_FAKE_A2_EVENT, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_sa_silence_req_add_silence_mode(mipc_msg_t *msg_ptr, enum mipc_nw_sa_slience_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_SA_SILENCE_REQ_T_SILENCE_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_sa_slience_mode_const_enum mipc_nw_get_sa_silence_cnf_get_silence_mode(mipc_msg_t *msg_ptr, mipc_nw_sa_slience_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_sa_slience_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_SA_SILENCE_CNF_T_SILENCE_MODE, def_val);
}

static inline mipc_nw_sa_blocking_mode_const_enum mipc_nw_get_sa_silence_cnf_get_blocking_mode(mipc_msg_t *msg_ptr, mipc_nw_sa_blocking_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_sa_blocking_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_SA_SILENCE_CNF_T_BLOCKING_MODE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_tx_power_reduction_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_nw_tx_rat_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_TX_POWER_REDUCTION_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_tx_power_reduction_req_add_txpwr_backoff(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_TX_POWER_REDUCTION_REQ_T_TXPWR_BACKOFF, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_tx_power_reduction_req_add_cg_type(mipc_msg_t *msg_ptr, enum mipc_nw_cg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_TX_POWER_REDUCTION_REQ_T_CG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_tx_rat_mode_const_enum mipc_nw_get_tx_power_reduction_cnf_get_rat(mipc_msg_t *msg_ptr, mipc_nw_tx_rat_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_tx_rat_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_TX_POWER_REDUCTION_CNF_T_RAT, def_val);
}

static inline uint8_t mipc_nw_get_tx_power_reduction_cnf_get_txpwr_backoff(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_TX_POWER_REDUCTION_CNF_T_TXPWR_BACKOFF, def_val);
}

static inline mipc_nw_cg_type_const_enum mipc_nw_get_tx_power_reduction_cnf_get_cg_type(mipc_msg_t *msg_ptr, mipc_nw_cg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_TX_POWER_REDUCTION_CNF_T_CG_TYPE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_lte_overheating_req_add_oh_op(mipc_msg_t *msg_ptr, enum mipc_nw_uai_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_LTE_OVERHEATING_REQ_T_OH_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_lte_overheating_req_add_cat_dl(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CAT_DL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_lte_overheating_req_add_cat_ul(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CAT_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_lte_overheating_req_add_cc_dl(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CC_DL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_lte_overheating_req_add_cc_ul(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CC_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_sase_cause_const_enum mipc_nw_set_lte_overheating_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_nw_sase_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_sase_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_LTE_OVERHEATING_CNF_T_CAUSE, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_lte_overheating_cnf_get_oh_op(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_OVERHEATING_CNF_T_OH_OP, def_val);
}

static inline uint8_t mipc_nw_get_lte_overheating_cnf_get_cat_dl(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CAT_DL, def_val);
}

static inline uint8_t mipc_nw_get_lte_overheating_cnf_get_cat_ul(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CAT_UL, def_val);
}

static inline uint8_t mipc_nw_get_lte_overheating_cnf_get_cc_dl(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CC_DL, def_val);
}

static inline uint8_t mipc_nw_get_lte_overheating_cnf_get_cc_ul(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CC_UL, def_val);
}

static inline mipc_nw_overheating_status_const_enum mipc_nw_get_lte_overheating_cnf_get_oh_status(mipc_msg_t *msg_ptr, mipc_nw_overheating_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_overheating_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_LTE_OVERHEATING_CNF_T_OH_STATUS, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_oh_op(mipc_msg_t *msg_ptr, enum mipc_nw_uai_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_OH_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_cc_dl(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_CC_DL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_cc_ul(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_CC_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_bw_fr1_dl(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR1_DL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_bw_fr1_ul(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR1_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_bw_fr2_dl(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR2_DL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_bw_fr2_ul(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR2_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_mimo_fr1_dl(mipc_msg_t *msg_ptr, enum mipc_nw_mimo_fr_dl_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR1_DL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_mimo_fr1_ul(mipc_msg_t *msg_ptr, enum mipc_nw_mimo_fr_ul_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR1_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_mimo_fr2_dl(mipc_msg_t *msg_ptr, enum mipc_nw_mimo_fr_dl_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR2_DL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_nr_overheating_req_add_mimo_fr2_ul(mipc_msg_t *msg_ptr, enum mipc_nw_mimo_fr_ul_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR2_UL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_sase_cause_const_enum mipc_nw_set_nr_overheating_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_nw_sase_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_sase_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_NR_OVERHEATING_CNF_T_CAUSE, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_nr_overheating_cnf_get_oh_op(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_OH_OP, def_val);
}

static inline uint8_t mipc_nw_get_nr_overheating_cnf_get_cc_dl(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_CC_DL, def_val);
}

static inline uint8_t mipc_nw_get_nr_overheating_cnf_get_cc_ul(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_CC_UL, def_val);
}

static inline uint8_t mipc_nw_get_nr_overheating_cnf_get_bw_fr1_dl(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR1_DL, def_val);
}

static inline uint8_t mipc_nw_get_nr_overheating_cnf_get_bw_fr1_ul(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR1_UL, def_val);
}

static inline uint8_t mipc_nw_get_nr_overheating_cnf_get_bw_fr2_dl(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR2_DL, def_val);
}

static inline uint8_t mipc_nw_get_nr_overheating_cnf_get_bw_fr2_ul(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR2_UL, def_val);
}

static inline mipc_nw_mimo_fr_dl_const_enum mipc_nw_get_nr_overheating_cnf_get_mimo_fr1_dl(mipc_msg_t *msg_ptr, mipc_nw_mimo_fr_dl_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_mimo_fr_dl_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR1_DL, def_val);
}

static inline mipc_nw_mimo_fr_ul_const_enum mipc_nw_get_nr_overheating_cnf_get_mimo_fr1_ul(mipc_msg_t *msg_ptr, mipc_nw_mimo_fr_ul_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_mimo_fr_ul_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR1_UL, def_val);
}

static inline mipc_nw_mimo_fr_dl_const_enum mipc_nw_get_nr_overheating_cnf_get_mimo_fr2_dl(mipc_msg_t *msg_ptr, mipc_nw_mimo_fr_dl_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_mimo_fr_dl_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR2_DL, def_val);
}

static inline mipc_nw_mimo_fr_ul_const_enum mipc_nw_get_nr_overheating_cnf_get_mimo_fr2_ul(mipc_msg_t *msg_ptr, mipc_nw_mimo_fr_ul_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_mimo_fr_ul_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR2_UL, def_val);
}

static inline mipc_nw_overheating_status_const_enum mipc_nw_get_nr_overheating_cnf_get_oh_status(mipc_msg_t *msg_ptr, mipc_nw_overheating_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_overheating_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_NR_OVERHEATING_CNF_T_OH_STATUS, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_cg_type(mipc_msg_t *msg_ptr, enum mipc_nw_cg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_CG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_uai_type(mipc_msg_t *msg_ptr, enum mipc_nw_uai_power_saving_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_UAI_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_op(mipc_msg_t *msg_ptr, enum mipc_nw_uai_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param1(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param3(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM3, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param4(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM4, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param5(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM5, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param6(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM6, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param7(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM7, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_uai_power_saving_req_add_param8(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM8, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_sase_cause_const_enum mipc_nw_set_uai_power_saving_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_nw_sase_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_sase_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_UAI_POWER_SAVING_CNF_T_CAUSE, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_mcg_max_cc(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_MAX_CC, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_scg_max_cc(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_MAX_CC, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_mcg_mimo(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_MIMO, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_scg_mimo(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_MIMO, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_mcg_drx(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_DRX, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_scg_drx(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_DRX, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_mcg_bw(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_BW, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_scg_bw(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_BW, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_mcg_min_sched(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_MIN_SCHED, def_val);
}

static inline mipc_nw_uai_req_op_const_enum mipc_nw_get_uai_power_saving_cnf_get_scg_min_sched(mipc_msg_t *msg_ptr, mipc_nw_uai_req_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uai_req_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_MIN_SCHED, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_rrc_release_preference_req_add_release_op(mipc_msg_t *msg_ptr, enum mipc_nw_rrc_release_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_RRC_RELEASE_PREFERENCE_REQ_T_RELEASE_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_sase_cause_const_enum mipc_nw_set_rrc_release_preference_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_nw_sase_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_sase_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_RRC_RELEASE_PREFERENCE_CNF_T_CAUSE, def_val);
}

static inline mipc_nw_rrc_release_op_const_enum mipc_nw_get_rrc_release_preference_cnf_get_release_status(mipc_msg_t *msg_ptr, mipc_nw_rrc_release_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_rrc_release_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_RRC_RELEASE_PREFERENCE_CNF_T_RELEASE_STATUS, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_fake_ri_req_add_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_FAKE_RI_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_fake_ri_req_add_band(mipc_msg_t *msg_ptr, enum mipc_nw_fake_ri_band_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_FAKE_RI_REQ_T_BAND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_fake_ri_req_add_cg_type(mipc_msg_t *msg_ptr, enum mipc_nw_cg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_FAKE_RI_REQ_T_CG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_fake_ri_req_add_ctrl_rank(mipc_msg_t *msg_ptr, enum mipc_nw_fake_ri_ctrl_rank_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_FAKE_RI_REQ_T_CTRL_RANK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_fake_ri_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_nw_fake_ri_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_FAKE_RI_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_get_fake_ri_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_nw_fake_ri_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_FAKE_RI_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_boolean_const_enum mipc_nw_get_fake_ri_cnf_get_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_FAKE_RI_CNF_T_ENABLE, def_val);
}

static inline mipc_nw_fake_ri_band_const_enum mipc_nw_get_fake_ri_cnf_get_band(mipc_msg_t *msg_ptr, mipc_nw_fake_ri_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_fake_ri_band_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_FAKE_RI_CNF_T_BAND, def_val);
}

static inline mipc_nw_cg_type_const_enum mipc_nw_get_fake_ri_cnf_get_cg_type(mipc_msg_t *msg_ptr, mipc_nw_cg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_FAKE_RI_CNF_T_CG_TYPE, def_val);
}

static inline mipc_nw_fake_ri_ctrl_rank_const_enum mipc_nw_get_fake_ri_cnf_get_ctrl_rank(mipc_msg_t *msg_ptr, mipc_nw_fake_ri_ctrl_rank_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_fake_ri_ctrl_rank_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_FAKE_RI_CNF_T_CTRL_RANK, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_mr_thresh_bound_req_add_conf_info(mipc_msg_t *msg_ptr, uint16_t len, mipc_lte_bound_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_NW_SET_MR_THRESH_BOUND_REQ_T_CONF_INFO, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_delete_ftai_entry_req_add_ftai_del_attr(mipc_msg_t *msg_ptr, enum mipc_nw_ftai_attr_all_with_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_DELETE_FTAI_ENTRY_REQ_T_FTAI_DEL_ATTR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_delete_ftai_entry_req_add_tai_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_ftai_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_set_no_service_timer_req_add_no_service_timer(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_NW_SET_NO_SERVICE_TIMER_REQ_T_NO_SERVICE_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_nw_get_no_service_timer_cnf_get_no_service_timer(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_GET_NO_SERVICE_TIMER_CNF_T_NO_SERVICE_TIMER, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_set_dynamic_ant_bias_req_add_scen_idx(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_DYNAMIC_ANT_BIAS_REQ_T_SCEN_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_dynamic_ant_bias_req_add_sim_idx(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_DYNAMIC_ANT_BIAS_REQ_T_SIM_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_dynamic_ant_bias_req_add_bit_rat(mipc_msg_t *msg_ptr, enum mipc_nw_bit_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_NW_SET_DYNAMIC_ANT_BIAS_REQ_T_BIT_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_absolute_max_tx_power_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ABSOLUTE_MAX_TX_POWER_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_absolute_max_tx_power_req_add_nv_write(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ABSOLUTE_MAX_TX_POWER_REQ_T_NV_WRITE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_absolute_max_tx_power_req_add_band_power_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_band_power_pair_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_nw_cscon_status_struct4* mipc_nw_get_cscon_state_cnf_get_cscon_state(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cscon_status_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_CSCON_STATE_CNF_T_CSCON_STATE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_da_prefer_custom_thresh_req_add_rsrp_thresh(mipc_msg_t *msg_ptr, int16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int16(msg_ptr, MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_REQ_T_RSRP_THRESH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_da_prefer_custom_thresh_req_add_rsrq_thresh(mipc_msg_t *msg_ptr, int16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int16(msg_ptr, MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_REQ_T_RSRQ_THRESH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_da_prefer_custom_thresh_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_max_eirp_req_add_rat(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_MAX_EIRP_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_max_eirp_req_add_nv_write(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_MAX_EIRP_REQ_T_NV_WRITE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_max_eirp_req_add_band_power_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_nw_band_power_pair_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_nw_clear_nssai_req_add_clear_nssai_option(mipc_msg_t *msg_ptr, enum mipc_nw_clear_nssai_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_CLEAR_NSSAI_REQ_T_CLEAR_NSSAI_OPTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_trigger_network_service_scan_req_add_action(mipc_msg_t *msg_ptr, enum mipc_nw_scan_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_trigger_network_service_scan_req_add_service_type(mipc_msg_t *msg_ptr, enum mipc_nw_service_scan_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_SERVICE_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_trigger_network_service_scan_req_add_available_rat(mipc_msg_t *msg_ptr, enum mipc_nw_scan_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_AVAILABLE_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_trigger_network_service_scan_req_add_prefer_rat(mipc_msg_t *msg_ptr, enum mipc_nw_prefer_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_PREFER_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_trigger_network_service_scan_req_add_is_need_retry(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_IS_NEED_RETRY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_nw_set_config_req_add_cellular_id_disclosure(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_CONFIG_REQ_T_CELLULAR_ID_DISCLOSURE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_config_result_const_enum mipc_nw_set_config_cnf_get_cellular_id_disclosure_result(mipc_msg_t *msg_ptr, mipc_nw_config_result_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_config_result_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE_RESULT, def_val);
}

static inline mipc_boolean_const_enum mipc_nw_set_config_cnf_get_configured_cellular_id_disclosure(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SET_CONFIG_CNF_T_CONFIGURED_CELLULAR_ID_DISCLOSURE, def_val);
}

static inline mipc_msg_api_result_enum mipc_nw_get_config_req_add_retrieve_cellular_id_disclosure(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_GET_CONFIG_REQ_T_RETRIEVE_CELLULAR_ID_DISCLOSURE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_config_result_const_enum mipc_nw_get_config_cnf_get_cellular_id_disclosure_result(mipc_msg_t *msg_ptr, mipc_nw_config_result_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_config_result_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE_RESULT, def_val);
}

static inline mipc_boolean_const_enum mipc_nw_get_config_cnf_get_cellular_id_disclosure(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE, def_val);
}

static inline mipc_nw_reg_state_struct4* mipc_nw_register_ind_get_state(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_state_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_STATE, val_len_ptr);
}

static inline uint16_t mipc_nw_register_ind_get_nw_err(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_REGISTER_IND_T_NW_ERR, def_val);
}

static inline mipc_nw_register_mode_const_enum mipc_nw_register_ind_get_mode(mipc_msg_t *msg_ptr, mipc_nw_register_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_register_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_REGISTER_IND_T_MODE, def_val);
}

static inline mipc_nw_data_speed_const_enum mipc_nw_register_ind_get_data_speed(mipc_msg_t *msg_ptr, mipc_nw_data_speed_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_speed_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_REGISTER_IND_T_DATA_SPEED, def_val);
}

static inline char * mipc_nw_register_ind_get_nw_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_NW_NAME, val_len_ptr);
}

static inline char * mipc_nw_register_ind_get_roaming_text(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_ROAMING_TEXT, val_len_ptr);
}

static inline uint16_t mipc_nw_register_ind_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_REGISTER_IND_T_FAIL_CAUSE, def_val);
}

static inline char * mipc_nw_register_ind_get_nw_long_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_NW_LONG_NAME, val_len_ptr);
}

static inline mipc_nw_reg_info_struct4* mipc_nw_register_ind_get_ps_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_PS_INFO, val_len_ptr);
}

static inline mipc_nw_reg_state_v1_struct4* mipc_nw_register_ind_get_state_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_state_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_STATE_V1, val_len_ptr);
}

static inline mipc_encoding_scheme_const_enum mipc_nw_register_ind_get_short_name_encoding_scheme(mipc_msg_t *msg_ptr, mipc_encoding_scheme_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_encoding_scheme_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_REGISTER_IND_T_SHORT_NAME_ENCODING_SCHEME, def_val);
}

static inline void * mipc_nw_register_ind_get_plmn_short_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_PLMN_SHORT_NAME, val_len_ptr);
}

static inline mipc_encoding_scheme_const_enum mipc_nw_register_ind_get_long_name_encoding_scheme(mipc_msg_t *msg_ptr, mipc_encoding_scheme_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_encoding_scheme_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_REGISTER_IND_T_LONG_NAME_ENCODING_SCHEME, def_val);
}

static inline void * mipc_nw_register_ind_get_plmn_long_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_REGISTER_IND_T_PLMN_LONG_NAME, val_len_ptr);
}

static inline mipc_plmn_name_source_const_enum mipc_nw_register_ind_get_plmn_name_source(mipc_msg_t *msg_ptr, mipc_plmn_name_source_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_plmn_name_source_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_REGISTER_IND_T_PLMN_NAME_SOURCE, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_rssi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_RSSI, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_err_rate(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_ERR_RATE, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_rsrp(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_RSRP, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_snr(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_SNR, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_signal_strength_interval(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_SIGNAL_STRENGTH_INTERVAL, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_rssi_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_RSSI_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_err_rate_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_ERR_RATE_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_rsrp_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_RSRP_THRESHOLD, def_val);
}

static inline uint32_t mipc_nw_signal_ind_get_snr_threshold(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_SIGNAL_IND_T_SNR_THRESHOLD, def_val);
}

static inline mipc_nw_signal_type_const_enum mipc_nw_signal_ind_get_signal_type(mipc_msg_t *msg_ptr, mipc_nw_signal_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_signal_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SIGNAL_IND_T_SIGNAL_TYPE, def_val);
}

static inline mipc_nw_gsm_signal_strength_struct4* mipc_nw_signal_ind_get_gsm_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_GSM_SIGNAL, val_len_ptr);
}

static inline mipc_nw_umts_signal_strength_struct4* mipc_nw_signal_ind_get_umts_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_UMTS_SIGNAL, val_len_ptr);
}

static inline mipc_nw_lte_signal_strength_struct4* mipc_nw_signal_ind_get_lte_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_LTE_SIGNAL, val_len_ptr);
}

static inline mipc_nw_nr_signal_strength_struct4* mipc_nw_signal_ind_get_nr_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_NR_SIGNAL, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_struct4* mipc_nw_signal_ind_get_raw_signal_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_LIST, val_len_ptr);
}

static inline uint8_t mipc_nw_signal_ind_get_raw_signal_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_COUNT, def_val);
}

static inline mipc_nw_cdma_signal_strength_struct4* mipc_nw_signal_ind_get_cdma_signal(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_signal_strength_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_CDMA_SIGNAL, val_len_ptr);
}

static inline mipc_nw_lte_signal_strength_v1_struct4* mipc_nw_signal_ind_get_lte_signal_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_signal_strength_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_LTE_SIGNAL_V1, val_len_ptr);
}

static inline mipc_nw_nr_signal_strength_v1_struct4* mipc_nw_signal_ind_get_nr_signal_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_NR_SIGNAL_V1, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_struct4* mipc_nw_signal_ind_get_raw_signal_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_nr_signal_strength_v3_struct4* mipc_nw_signal_ind_get_nr_signal_v3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_v3_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_NR_SIGNAL_V3, val_len_ptr);
}

static inline mipc_nw_lte_signal_strength_v2_struct4* mipc_nw_signal_ind_get_lte_signal_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_signal_strength_v2_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_LTE_SIGNAL_V2, val_len_ptr);
}

static inline mipc_nw_raw_signal_info_v1_struct4* mipc_nw_signal_ind_get_raw_signal_info_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_raw_signal_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_ps_const_enum mipc_nw_ps_ind_get_tach(mipc_msg_t *msg_ptr, mipc_nw_ps_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ps_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_PS_IND_T_TACH, def_val);
}

static inline mipc_nw_data_speed_const_enum mipc_nw_ps_ind_get_data_speed(mipc_msg_t *msg_ptr, mipc_nw_data_speed_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_speed_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_PS_IND_T_DATA_SPEED, def_val);
}

static inline uint8_t mipc_nw_ps_ind_get_nw_frequency(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_PS_IND_T_NW_FREQUENCY, def_val);
}

static inline mipc_nw_ps_reg_info_struct4* mipc_nw_ps_ind_get_reg_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ps_reg_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_REG_INFO, val_len_ptr);
}

static inline mipc_nw_cell_type_const_enum mipc_nw_ps_ind_get_cell_type(mipc_msg_t *msg_ptr, mipc_nw_cell_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cell_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_PS_IND_T_CELL_TYPE, def_val);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_ps_ind_get_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_ps_ind_get_gsm_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_GSM_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_ps_ind_get_umts_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_UMTS_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_ps_ind_get_lte_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_LTE_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_ps_ind_get_nr_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_NR_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_ps_ind_get_cdma_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_CDMA_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_ps_reg_info_v1_struct8* mipc_nw_ps_ind_get_reg_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ps_reg_info_v1_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_REG_INFO_V1, val_len_ptr);
}

static inline mipc_nw_nr_cell_v2_struct8* mipc_nw_ps_ind_get_nr_cell_info_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v2_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_NR_CELL_INFO_V2, val_len_ptr);
}

static inline mipc_nw_cdma_cell_v1_struct4* mipc_nw_ps_ind_get_cdma_cell_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PS_IND_T_CDMA_CELL_INFO_V1, val_len_ptr);
}

static inline uint8_t mipc_nw_radio_ind_get_sw_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_RADIO_IND_T_SW_STATE, def_val);
}

static inline uint8_t mipc_nw_radio_ind_get_hw_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_RADIO_IND_T_HW_STATE, def_val);
}

static inline char * mipc_nw_ia_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_IA_IND_T_APN, val_len_ptr);
}

static inline uint8_t mipc_nw_ia_ind_get_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_IA_IND_T_RAT, def_val);
}

static inline mipc_apn_pdp_type_const_enum mipc_nw_ia_ind_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_IA_IND_T_PDP_TYPE, def_val);
}

static inline mipc_apn_auth_type_const_enum mipc_nw_ia_ind_get_auth_type(mipc_msg_t *msg_ptr, mipc_apn_auth_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_auth_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_IA_IND_T_AUTH_TYPE, def_val);
}

static inline char * mipc_nw_ia_ind_get_userid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_IA_IND_T_USERID, val_len_ptr);
}

static inline char * mipc_nw_ia_ind_get_password(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_IA_IND_T_PASSWORD, val_len_ptr);
}

static inline uint32_t mipc_nw_ia_ind_get_apn_index(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_IA_IND_T_APN_INDEX, def_val);
}

static inline uint32_t mipc_nw_ia_ind_get_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_IA_IND_T_CAUSE, def_val);
}

static inline mipc_nw_nitz_info_struct4* mipc_nw_nitz_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nitz_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_NITZ_IND_T_INFO, val_len_ptr);
}

static inline mipc_nitz_ind_type_const_enum mipc_nw_nitz_ind_get_type(mipc_msg_t *msg_ptr, mipc_nitz_ind_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nitz_ind_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NITZ_IND_T_TYPE, def_val);
}

static inline mipc_nw_location_info_struct4* mipc_nw_location_info_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_location_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_LOCATION_INFO_IND_T_INFO, val_len_ptr);
}

static inline mipc_nw_reg_change_info_struct4* mipc_nw_cs_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_change_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_INFO, val_len_ptr);
}

static inline mipc_nw_cell_type_const_enum mipc_nw_cs_ind_get_cell_type(mipc_msg_t *msg_ptr, mipc_nw_cell_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cell_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CS_IND_T_CELL_TYPE, def_val);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_cs_ind_get_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_cs_ind_get_gsm_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_GSM_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_cs_ind_get_umts_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_UMTS_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_cs_ind_get_lte_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_LTE_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_cs_ind_get_nr_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_NR_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_cs_ind_get_cdma_cell_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_CDMA_CELL_INFO, val_len_ptr);
}

static inline mipc_nw_reg_change_info_v1_struct8* mipc_nw_cs_ind_get_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_reg_change_info_v1_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_INFO_V1, val_len_ptr);
}

static inline mipc_nw_nr_cell_v2_struct8* mipc_nw_cs_ind_get_nr_cell_info_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v2_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_NR_CELL_INFO_V2, val_len_ptr);
}

static inline mipc_nw_cdma_cell_v1_struct4* mipc_nw_cs_ind_get_cdma_cell_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_v1_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CS_IND_T_CDMA_CELL_INFO_V1, val_len_ptr);
}

static inline mipc_nw_cscon_status_struct4* mipc_nw_cscon_ind_get_status(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cscon_status_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CSCON_IND_T_STATUS, val_len_ptr);
}

static inline uint8_t mipc_nw_preferred_provider_ind_get_provider_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_PREFERRED_PROVIDER_IND_T_PROVIDER_COUNT, def_val);
}

static inline mipc_nw_provider_struct4* mipc_nw_preferred_provider_ind_get_provider_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PREFERRED_PROVIDER_IND_T_PROVIDER_LIST, val_len_ptr);
}

static inline mipc_nw_provider_struct4* mipc_nw_preferred_provider_ind_get_nw_provider_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_provider_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_PREFERRED_PROVIDER_IND_T_NW_PROVIDER_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_nw_cainfo_ind_get_lte_dl_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CAINFO_IND_T_LTE_DL_SERVING_CELL_COUNT, def_val);
}

static inline uint8_t mipc_nw_cainfo_ind_get_lte_ul_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CAINFO_IND_T_LTE_UL_SERVING_CELL_COUNT, def_val);
}

static inline uint8_t mipc_nw_cainfo_ind_get_nr_dl_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CAINFO_IND_T_NR_DL_SERVING_CELL_COUNT, def_val);
}

static inline uint8_t mipc_nw_cainfo_ind_get_nr_ul_serving_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CAINFO_IND_T_NR_UL_SERVING_CELL_COUNT, def_val);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_lte_dl_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_LTE_DL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_lte_ul_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_LTE_UL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_nr_dl_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_NR_DL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_nr_ul_serving_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_NR_UL_SERVING_CELL_LIST, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_lte_dl_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_LTE_DL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_lte_ul_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_LTE_UL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_nr_dl_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_NR_DL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_nr_ca_info_struct4* mipc_nw_cainfo_ind_get_nr_ul_serving_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_nr_ca_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_CAINFO_IND_T_NR_UL_SERVING_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline uint32_t mipc_nw_eons_ind_get_pnn(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_EONS_IND_T_PNN, def_val);
}

static inline uint32_t mipc_nw_eons_ind_get_opl(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_EONS_IND_T_OPL, def_val);
}

static inline uint32_t mipc_nw_ciev_ind_get_ciev_type(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_CIEV_IND_T_CIEV_TYPE, def_val);
}

static inline uint32_t mipc_nw_ciev_ind_get_ecbm_status(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_CIEV_IND_T_ECBM_STATUS, def_val);
}

static inline char * mipc_nw_ciev_ind_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CIEV_IND_T_PLMN_ID, val_len_ptr);
}

static inline char * mipc_nw_ciev_ind_get_nw_name_long(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CIEV_IND_T_NW_NAME_LONG, val_len_ptr);
}

static inline char * mipc_nw_ciev_ind_get_nw_name_short(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CIEV_IND_T_NW_NAME_SHORT, val_len_ptr);
}

static inline uint32_t mipc_nw_ciev_ind_get_prl_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_CIEV_IND_T_PRL_ID, def_val);
}

static inline uint32_t mipc_nw_egmss_ind_get_rat(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_EGMSS_IND_T_RAT, def_val);
}

static inline uint32_t mipc_nw_egmss_ind_get_mcc(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_EGMSS_IND_T_MCC, def_val);
}

static inline uint32_t mipc_nw_egmss_ind_get_status(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_EGMSS_IND_T_STATUS, def_val);
}

static inline uint32_t mipc_nw_egmss_ind_get_cur_reported_rat(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_EGMSS_IND_T_CUR_REPORTED_RAT, def_val);
}

static inline uint32_t mipc_nw_egmss_ind_get_is_home_country(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_EGMSS_IND_T_IS_HOME_COUNTRY, def_val);
}

static inline mipc_nw_data_speed_const_enum mipc_nw_psbearer_ind_get_cell_data_speed_support(mipc_msg_t *msg_ptr, mipc_nw_data_speed_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_speed_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_PSBEARER_IND_T_CELL_DATA_SPEED_SUPPORT, def_val);
}

static inline mipc_nw_data_bearer_capability_const_enum mipc_nw_psbearer_ind_get_max_data_bearer_capability(mipc_msg_t *msg_ptr, mipc_nw_data_bearer_capability_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_data_bearer_capability_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_PSBEARER_IND_T_MAX_DATA_BEARER_CAPABILITY, def_val);
}

static inline uint32_t mipc_nw_psbearer_ind_get_sec_cell_num_in_dl(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_PSBEARER_IND_T_SEC_CELL_NUM_IN_DL, def_val);
}

static inline uint32_t mipc_nw_psbearer_ind_get_sec_cell_num_in_ul(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_PSBEARER_IND_T_SEC_CELL_NUM_IN_UL, def_val);
}

static inline uint32_t mipc_nw_ecell_ind_get_gsm_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_ECELL_IND_T_GSM_CELL_COUNT, def_val);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_ecell_ind_get_gsm_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_GSM_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_ecell_ind_get_umts_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_ECELL_IND_T_UMTS_CELL_COUNT, def_val);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_ecell_ind_get_umts_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_UMTS_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_ecell_ind_get_lte_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_ECELL_IND_T_LTE_CELL_COUNT, def_val);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_ecell_ind_get_lte_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_LTE_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_ecell_ind_get_cdma_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_ECELL_IND_T_CDMA_CELL_COUNT, def_val);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_ecell_ind_get_cdma_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_CDMA_CELL_LIST, val_len_ptr);
}

static inline uint32_t mipc_nw_ecell_ind_get_nr_cell_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_ECELL_IND_T_NR_CELL_COUNT, def_val);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_ecell_ind_get_nr_cell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_NR_CELL_LIST, val_len_ptr);
}

static inline uint16_t mipc_nw_ecell_ind_get_fail_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_ECELL_IND_T_FAIL_CAUSE, def_val);
}

static inline mipc_nw_lte_cell_v1_struct4* mipc_nw_ecell_ind_get_lte_cell_tlv_array_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_LTE_CELL_TLV_ARRAY_V1, index, val_len_ptr);
}

static inline mipc_nw_lte_cell_v1_struct4* mipc_nw_ecell_ind_get_lte_cell_list_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_LTE_CELL_LIST_V1, index, val_len_ptr);
}

static inline mipc_nw_nr_cell_v1_struct4* mipc_nw_ecell_ind_get_nr_cell_tlv_array_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_NR_CELL_TLV_ARRAY_V1, index, val_len_ptr);
}

static inline mipc_nw_nr_cell_v1_struct4* mipc_nw_ecell_ind_get_nr_cell_list_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_NR_CELL_LIST_V1, index, val_len_ptr);
}

static inline mipc_nw_lte_cell_struct4* mipc_nw_ecell_ind_get_lte_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_LTE_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_nr_cell_struct4* mipc_nw_ecell_ind_get_nr_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_NR_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_gsm_cell_struct4* mipc_nw_ecell_ind_get_gsm_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_gsm_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_GSM_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_umts_cell_struct4* mipc_nw_ecell_ind_get_umts_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_umts_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_UMTS_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cdma_cell_struct4* mipc_nw_ecell_ind_get_cdma_cell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_CDMA_CELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_nr_cell_v2_struct8* mipc_nw_ecell_ind_get_nr_cell_v2_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_cell_v2_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_NR_CELL_V2_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_lte_cell_v2_struct4* mipc_nw_ecell_ind_get_lte_cell_v2_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_lte_cell_v2_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_LTE_CELL_V2_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_cdma_cell_v1_struct4* mipc_nw_ecell_ind_get_cdma_cell_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_cdma_cell_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_ECELL_IND_T_CDMA_CELL_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_anbr_info_struct4* mipc_nw_anbr_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_anbr_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_ANBR_IND_T_INFO, val_len_ptr);
}

static inline mipc_nw_irat_info_struct4* mipc_nw_irat_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_irat_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_IRAT_IND_T_INFO, val_len_ptr);
}

static inline uint8_t mipc_nw_ereginfo_ind_get_act(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EREGINFO_IND_T_ACT, def_val);
}

static inline uint8_t mipc_nw_ereginfo_ind_get_event_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EREGINFO_IND_T_EVENT_TYPE, def_val);
}

static inline mipc_nw_signal_modulation_const_enum mipc_nw_emodcfg_ind_get_modulation(mipc_msg_t *msg_ptr, mipc_nw_signal_modulation_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_signal_modulation_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EMODCFG_IND_T_MODULATION, def_val);
}

static inline uint8_t mipc_nw_epcellinfo_ind_get_lte_band(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EPCELLINFO_IND_T_LTE_BAND, def_val);
}

static inline uint8_t mipc_nw_pseudo_cell_ind_get_cell_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_PSEUDO_CELL_IND_T_CELL_COUNT, def_val);
}

static inline mipc_nw_pseudocell_info_struct4* mipc_nw_pseudo_cell_ind_get_pseudocell_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pseudocell_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PSEUDO_CELL_IND_T_PSEUDOCELL_LIST, val_len_ptr);
}

static inline mipc_nw_pseudocell_info_struct4* mipc_nw_pseudo_cell_ind_get_pseudocell_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pseudocell_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_PSEUDO_CELL_IND_T_PSEUDOCELL_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_pseudocell_info_v1_struct8* mipc_nw_pseudo_cell_ind_get_pseudocell_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_pseudocell_info_v1_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_PSEUDO_CELL_IND_T_PSEUDOCELL_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline uint16_t mipc_nw_network_info_ind_get_type(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_NETWORK_INFO_IND_T_TYPE, def_val);
}

static inline char * mipc_nw_network_info_ind_get_nw_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_NETWORK_INFO_IND_T_NW_INFO, val_len_ptr);
}

static inline char * mipc_nw_mccmnc_ind_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_MCCMNC_IND_T_PLMN_ID, val_len_ptr);
}

static inline uint8_t mipc_nw_physical_channel_configs_ind_get_physical_ch_info_list_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_LIST_COUNT, def_val);
}

static inline mipc_physical_channel_info_struct4* mipc_nw_physical_channel_configs_ind_get_physical_ch_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_physical_channel_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_LIST, val_len_ptr);
}

static inline mipc_physical_channel_info_v1_struct4* mipc_nw_physical_channel_configs_ind_get_physical_ch_info_tlv_array_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_physical_channel_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_TLV_ARRAY_V1, index, val_len_ptr);
}

static inline mipc_physical_channel_info_v1_struct4* mipc_nw_physical_channel_configs_ind_get_physical_ch_info_list_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_physical_channel_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_LIST_V1, index, val_len_ptr);
}

static inline mipc_physical_channel_info_struct4* mipc_nw_physical_channel_configs_ind_get_physical_ch_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_physical_channel_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_physical_channel_info_v2_struct4* mipc_nw_physical_channel_configs_ind_get_physical_ch_info_v2_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_physical_channel_info_v2_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_V2_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_nw_otacmsg_ind_get_ota_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_OTACMSG_IND_T_OTA_STATE, def_val);
}

static inline uint8_t mipc_nw_barring_info_ind_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_BARRING_INFO_IND_T_COUNT, def_val);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_barring_info_ind_get_barring_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_BARRING_INFO_IND_T_BARRING_LIST, val_len_ptr);
}

static inline uint8_t mipc_nw_barring_info_ind_get_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_BARRING_INFO_IND_T_RAT, def_val);
}

static inline mipc_nw_barring_info_struct4* mipc_nw_barring_info_ind_get_barring_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_BARRING_INFO_IND_T_BARRING_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_nw_barring_info_v1_struct4* mipc_nw_barring_info_ind_get_barring_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_barring_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_NW_BARRING_INFO_IND_T_BARRING_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline uint32_t mipc_nw_radio_capability_ind_get_radio_capability(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_RADIO_CAPABILITY_IND_T_RADIO_CAPABILITY, def_val);
}

static inline mipc_nw_bit_rat_const_enum mipc_nw_current_rat_ind_get_current_bit_rat(mipc_msg_t *msg_ptr, mipc_nw_bit_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_bit_rat_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_CURRENT_RAT_IND_T_CURRENT_BIT_RAT, def_val);
}

static inline mipc_nw_rat_mode_const_enum mipc_nw_current_rat_ind_get_current_rat(mipc_msg_t *msg_ptr, mipc_nw_rat_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_rat_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CURRENT_RAT_IND_T_CURRENT_RAT, def_val);
}

static inline uint8_t mipc_nw_current_rat_ind_get_prefer_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CURRENT_RAT_IND_T_PREFER_RAT, def_val);
}

static inline mipc_nw_ps_reg_info_struct4* mipc_nw_camp_state_ind_get_reg_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ps_reg_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAMP_STATE_IND_T_REG_INFO, val_len_ptr);
}

static inline char * mipc_nw_camp_state_ind_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAMP_STATE_IND_T_PLMN_ID, val_len_ptr);
}

static inline char * mipc_nw_camp_state_ind_get_nw_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAMP_STATE_IND_T_NW_NAME, val_len_ptr);
}

static inline mipc_nw_ps_reg_info_v1_struct8* mipc_nw_camp_state_ind_get_reg_info_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_ps_reg_info_v1_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CAMP_STATE_IND_T_REG_INFO_V1, val_len_ptr);
}

static inline uint8_t mipc_nw_nr_switch_ind_get_nr_sim(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NR_SWITCH_IND_T_NR_SIM, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_TYPE, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_is_1x_femtocell(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_IS_1X_FEMTOCELL, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_is_evdo_femtocell(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_IS_EVDO_FEMTOCELL, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_is_femtocell(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_IS_FEMTOCELL, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_domain(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_DOMAIN, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_STATE, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_act(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_ACT, def_val);
}

static inline uint8_t mipc_nw_femtocell_info_ind_get_is_csg_cell(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_IS_CSG_CELL, def_val);
}

static inline uint32_t mipc_nw_femtocell_info_ind_get_csg_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_CSG_ID, def_val);
}

static inline uint16_t mipc_nw_femtocell_info_ind_get_csg_icon_type(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_CSG_ICON_TYPE, def_val);
}

static inline uint16_t mipc_nw_femtocell_info_ind_get_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_CAUSE, def_val);
}

static inline char * mipc_nw_femtocell_info_ind_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_PLMN_ID, val_len_ptr);
}

static inline char * mipc_nw_femtocell_info_ind_get_oper_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_OPER_NAME, val_len_ptr);
}

static inline char * mipc_nw_femtocell_info_ind_get_hnbname(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_FEMTOCELL_INFO_IND_T_HNBNAME, val_len_ptr);
}

static inline uint32_t mipc_nw_etxpwr_ind_get_act(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_ETXPWR_IND_T_ACT, def_val);
}

static inline int32_t mipc_nw_etxpwr_ind_get_tx_power(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_ETXPWR_IND_T_TX_POWER, def_val);
}

static inline uint16_t mipc_nw_etxpwrstus_ind_get_event(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_NW_ETXPWRSTUS_IND_T_EVENT, def_val);
}

static inline int16_t mipc_nw_etxpwrstus_ind_get_sar_scenario_index(mipc_msg_t *msg_ptr, int16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int16_t)mipc_msg_get_val_int16(msg_ptr, MIPC_NW_ETXPWRSTUS_IND_T_SAR_SCENARIO_INDEX, def_val);
}

static inline mipc_nw_iwlan_status_const_enum mipc_nw_iwlan_ind_get_status(mipc_msg_t *msg_ptr, mipc_nw_iwlan_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_iwlan_status_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_IWLAN_IND_T_STATUS, def_val);
}

static inline mipc_nw_ch_info_rat_const_enum mipc_nw_ch_info_ind_get_rat(mipc_msg_t *msg_ptr, mipc_nw_ch_info_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ch_info_rat_const_enum)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_CH_INFO_IND_T_RAT, def_val);
}

static inline int32_t mipc_nw_ch_info_ind_get_band(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_CH_INFO_IND_T_BAND, def_val);
}

static inline int32_t mipc_nw_ch_info_ind_get_channel(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_CH_INFO_IND_T_CHANNEL, def_val);
}

static inline int32_t mipc_nw_ch_info_ind_get_is_endc(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_CH_INFO_IND_T_IS_ENDC, def_val);
}

static inline uint8_t mipc_nw_nruw_info_ind_get_display_5guw(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRUW_INFO_IND_T_DISPLAY_5GUW, def_val);
}

static inline uint8_t mipc_nw_nruw_info_ind_get_on_n77_band(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRUW_INFO_IND_T_ON_N77_BAND, def_val);
}

static inline uint8_t mipc_nw_nruw_info_ind_get_on_fr2_band(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRUW_INFO_IND_T_ON_FR2_BAND, def_val);
}

static inline uint8_t mipc_nw_nruw_info_ind_get_5guw_allowed(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRUW_INFO_IND_T_5GUW_ALLOWED, def_val);
}

static inline int32_t mipc_nw_nr_ca_band_ind_get_is_endc(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_NR_CA_BAND_IND_T_IS_ENDC, def_val);
}

static inline uint8_t mipc_nw_nr_ca_band_ind_get_band_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NR_CA_BAND_IND_T_BAND_NUM, def_val);
}

static inline mipc_nr_ca_band_struct4* mipc_nw_nr_ca_band_ind_get_band(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nr_ca_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_NR_CA_BAND_IND_T_BAND, val_len_ptr);
}

static inline int32_t mipc_nw_nr_scs_ind_get_scs(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_NW_NR_SCS_IND_T_SCS, def_val);
}

static inline mipc_nw_5guc_state_const_enum mipc_nw_nruc_info_ind_get_display_5guc(mipc_msg_t *msg_ptr, mipc_nw_5guc_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_5guc_state_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_NRUC_INFO_IND_T_DISPLAY_5GUC, def_val);
}

static inline mipc_nw_uc_band_const_enum mipc_nw_nruc_info_ind_get_on_uc_band(mipc_msg_t *msg_ptr, mipc_nw_uc_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_uc_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_NRUC_INFO_IND_T_ON_UC_BAND, def_val);
}

static inline uint32_t mipc_nw_nruc_info_ind_get_agg_bw(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_NRUC_INFO_IND_T_AGG_BW, def_val);
}

static inline char * mipc_nw_first_plmn_ind_get_mcc(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_FIRST_PLMN_IND_T_MCC, val_len_ptr);
}

static inline char * mipc_nw_first_plmn_ind_get_mnc(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_FIRST_PLMN_IND_T_MNC, val_len_ptr);
}

static inline mipc_n3_reg_state_const_enum mipc_nw_n3_reg_state_ind_get_reg_state(mipc_msg_t *msg_ptr, mipc_n3_reg_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_n3_reg_state_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_N3_REG_STATE_IND_T_REG_STATE, def_val);
}

static inline uint32_t mipc_nw_n3_reg_state_ind_get_gw_type(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_N3_REG_STATE_IND_T_GW_TYPE, def_val);
}

static inline mipc_n3_device_type_const_enum mipc_nw_n3_reg_state_ind_get_device_type(mipc_msg_t *msg_ptr, mipc_n3_device_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_n3_device_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_N3_REG_STATE_IND_T_DEVICE_TYPE, def_val);
}

static inline uint32_t mipc_nw_n3_reg_state_ind_get_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_N3_REG_STATE_IND_T_CAUSE, def_val);
}

static inline mipc_nw_edrx_access_technology_enum_const_enum mipc_nw_edrx_ind_get_act(mipc_msg_t *msg_ptr, mipc_nw_edrx_access_technology_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_edrx_access_technology_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EDRX_IND_T_ACT, def_val);
}

static inline uint8_t mipc_nw_edrx_ind_get_requested_edrx_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EDRX_IND_T_REQUESTED_EDRX_VALUE, def_val);
}

static inline uint8_t mipc_nw_edrx_ind_get_nw_provided_edrx_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EDRX_IND_T_NW_PROVIDED_EDRX_VALUE, def_val);
}

static inline uint8_t mipc_nw_edrx_ind_get_paging_time_window(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EDRX_IND_T_PAGING_TIME_WINDOW, def_val);
}

static inline uint8_t mipc_nw_mobile_initiated_connection_only_mode_ind_get_current_mico_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_CURRENT_MICO_MODE, def_val);
}

static inline uint8_t mipc_nw_mobile_initiated_connection_only_mode_ind_get_raai_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_RAAI_VALUE, def_val);
}

static inline uint8_t mipc_nw_mobile_initiated_connection_only_mode_ind_get_sprt_value(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_SPRT_VALUE, def_val);
}

static inline uint8_t mipc_nw_mobile_initiated_connection_only_mode_ind_get_allocated_active_time(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_ALLOCATED_ACTIVE_TIME, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_auth_rej_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_AUTH_REJ_CAUSE, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_mm_lu_rej_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_MM_LU_REJ_CAUSE, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_attach_rej_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_ATTACH_REJ_CAUSE, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_reg_update_rej_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_REG_UPDATE_REJ_CAUSE, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_mm_lu_attempt_counter(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_MM_LU_ATTEMPT_COUNTER, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_attach_attempt_counter(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_ATTACH_ATTEMPT_COUNTER, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_reg_update_attempt_counter(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_REG_UPDATE_ATTEMPT_COUNTER, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_RAT, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_service_rej_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_SERVICE_REJ_CAUSE, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_esm_rej_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_ESM_REJ_CAUSE, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_proc_result(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_PROC_RESULT, def_val);
}

static inline uint8_t mipc_nw_enwrejac_ind_get_nas_proc(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWREJAC_IND_T_NAS_PROC, def_val);
}

static inline char * mipc_nw_enwrejac_ind_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_ENWREJAC_IND_T_PLMN_ID, val_len_ptr);
}

static inline mipc_nw_basement_status_const_enum mipc_nw_basement_detection_ind_get_status(mipc_msg_t *msg_ptr, mipc_nw_basement_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_basement_status_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_BASEMENT_DETECTION_IND_T_STATUS, def_val);
}

static inline uint8_t mipc_nw_enwcfginfo_ind_get_4x4mino_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWCFGINFO_IND_T_4X4MINO_STATE, def_val);
}

static inline uint8_t mipc_nw_enwcfginfo_ind_get_256qam_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWCFGINFO_IND_T_256QAM_STATE, def_val);
}

static inline uint8_t mipc_nw_enwcfginfo_ind_get_64qam_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_ENWCFGINFO_IND_T_64QAM_STATE, def_val);
}

static inline mipc_release_preference_uai_report_status_const_enum mipc_nw_nrrc_rrc_release_uai_ind_get_report_status(mipc_msg_t *msg_ptr, mipc_release_preference_uai_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_release_preference_uai_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_RRC_RELEASE_UAI_IND_T_REPORT_STATUS, def_val);
}

static inline uint8_t mipc_nw_nrrc_rrc_release_uai_ind_get_connected_reporting(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_RRC_RELEASE_UAI_IND_T_CONNECTED_REPORTING, def_val);
}

static inline mipc_ue_preferred_rrc_state_const_enum mipc_nw_nrrc_rrc_release_uai_ind_get_preferred_rrc_state(mipc_msg_t *msg_ptr, mipc_ue_preferred_rrc_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ue_preferred_rrc_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_RRC_RELEASE_UAI_IND_T_PREFERRED_RRC_STATE, def_val);
}

static inline uint8_t mipc_nw_nrrc_powersaving_uai_ind_get_cell_group_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_CELL_GROUP_ID, def_val);
}

static inline mipc_nrrc_preference_report_status_const_enum mipc_nw_nrrc_powersaving_uai_ind_get_maxcc_preference_status(mipc_msg_t *msg_ptr, mipc_nrrc_preference_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nrrc_preference_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MAXCC_PREFERENCE_STATUS, def_val);
}

static inline mipc_nrrc_preference_report_status_const_enum mipc_nw_nrrc_powersaving_uai_ind_get_maxmimo_layerpreference_status(mipc_msg_t *msg_ptr, mipc_nrrc_preference_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nrrc_preference_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MAXMIMO_LAYERPREFERENCE_STATUS, def_val);
}

static inline mipc_nrrc_preference_report_status_const_enum mipc_nw_nrrc_powersaving_uai_ind_get_drx_preference_status(mipc_msg_t *msg_ptr, mipc_nrrc_preference_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nrrc_preference_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_DRX_PREFERENCE_STATUS, def_val);
}

static inline mipc_nrrc_preference_report_status_const_enum mipc_nw_nrrc_powersaving_uai_ind_get_maxbw_preference_status(mipc_msg_t *msg_ptr, mipc_nrrc_preference_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nrrc_preference_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MAXBW_PREFERENCE_STATUS, def_val);
}

static inline mipc_nrrc_preference_report_status_const_enum mipc_nw_nrrc_powersaving_uai_ind_get_minschedulingoffset_preference_status(mipc_msg_t *msg_ptr, mipc_nrrc_preference_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nrrc_preference_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MINSCHEDULINGOFFSET_PREFERENCE_STATUS, def_val);
}

static inline mipc_overheating_uai_report_status_const_enum mipc_nw_nrrc_overheatingassistance_uai_ind_get_report_status(mipc_msg_t *msg_ptr, mipc_overheating_uai_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_overheating_uai_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_OVERHEATINGASSISTANCE_UAI_IND_T_REPORT_STATUS, def_val);
}

static inline mipc_nrrc_timer_name_const_enum mipc_nw_nrrc_timer_status_ind_get_timer_name(mipc_msg_t *msg_ptr, mipc_nrrc_timer_name_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nrrc_timer_name_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_TIMER_STATUS_IND_T_TIMER_NAME, def_val);
}

static inline mipc_nrrc_timer_status_const_enum mipc_nw_nrrc_timer_status_ind_get_timer_status(mipc_msg_t *msg_ptr, mipc_nrrc_timer_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nrrc_timer_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_NRRC_TIMER_STATUS_IND_T_TIMER_STATUS, def_val);
}

static inline uint32_t mipc_nw_nrrc_timer_status_ind_get_timer_value(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_NW_NRRC_TIMER_STATUS_IND_T_TIMER_VALUE, def_val);
}

static inline mipc_nw_epsfb_state_enum_const_enum mipc_nw_epsfb_ind_get_state(mipc_msg_t *msg_ptr, mipc_nw_epsfb_state_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_epsfb_state_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EPSFB_IND_T_STATE, def_val);
}

static inline mipc_nw_epsfb_type_enum_const_enum mipc_nw_epsfb_ind_get_type(mipc_msg_t *msg_ptr, mipc_nw_epsfb_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_epsfb_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_EPSFB_IND_T_TYPE, def_val);
}

static inline mipc_nw_tau_fail_cause_enum_const_enum mipc_nw_tau_fail_ind_get_cause(mipc_msg_t *msg_ptr, mipc_nw_tau_fail_cause_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_tau_fail_cause_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_TAU_FAIL_IND_T_CAUSE, def_val);
}

static inline mipc_nw_nr_signal_strength_v2_struct4* mipc_nw_get_signal_cnf_get_nr_signal_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_v2_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL_V2, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_os_id_update_req_add_os_id_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_OS_ID_UPDATE_REQ_T_OS_ID_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_nw_nr_signal_strength_v2_struct4* mipc_nw_signal_ind_get_nr_signal_v2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nr_signal_strength_v2_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_SIGNAL_IND_T_NR_SIGNAL_V2, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_nw_set_allowed_security_algo_config_req_add_allow_null_algo(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ_T_ALLOW_NULL_ALGO, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_boolean_const_enum mipc_nw_get_allowed_security_algo_config_cnf_get_allow_null_algo(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_CNF_T_ALLOW_NULL_ALGO, def_val);
}

static inline char * mipc_nw_cellular_id_disclosure_ind_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_PLMN_ID, val_len_ptr);
}

static inline mipc_nw_cellular_id_type_const_enum mipc_nw_cellular_id_disclosure_ind_get_cellular_id_type(mipc_msg_t *msg_ptr, mipc_nw_cellular_id_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_cellular_id_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_CELLULAR_ID_TYPE, def_val);
}

static inline mipc_nw_nas_msg_type_const_enum mipc_nw_cellular_id_disclosure_ind_get_nas_msg_type(mipc_msg_t *msg_ptr, mipc_nw_nas_msg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_nas_msg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_NAS_MSG_TYPE, def_val);
}

static inline mipc_boolean_const_enum mipc_nw_cellular_id_disclosure_ind_get_is_emergency(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_IS_EMERGENCY, def_val);
}

static inline mipc_nw_connection_event_const_enum mipc_nw_security_algos_ind_get_connection_event(mipc_msg_t *msg_ptr, mipc_nw_connection_event_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_connection_event_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SECURITY_ALGOS_IND_T_CONNECTION_EVENT, def_val);
}

static inline mipc_nw_integrity_algo_const_enum mipc_nw_security_algos_ind_get_integrity_algo(mipc_msg_t *msg_ptr, mipc_nw_integrity_algo_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_integrity_algo_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SECURITY_ALGOS_IND_T_INTEGRITY_ALGO, def_val);
}

static inline mipc_nw_encryption_algo_const_enum mipc_nw_security_algos_ind_get_encryption_algo(mipc_msg_t *msg_ptr, mipc_nw_encryption_algo_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_encryption_algo_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SECURITY_ALGOS_IND_T_ENCRYPTION_ALGO, def_val);
}

static inline mipc_boolean_const_enum mipc_nw_security_algos_ind_get_is_unprotected_emergency(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_NW_SECURITY_ALGOS_IND_T_IS_UNPROTECTED_EMERGENCY, def_val);
}

#endif /* __MIPC_MSG_NW_API_H__ */
