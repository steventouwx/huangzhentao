#ifndef __MIPC_MSG_APN_API_H__
#define __MIPC_MSG_APN_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_SET_IA_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_pdp_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_IA_REQ_T_PDP_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_roaming_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_IA_REQ_T_ROAMING_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_auth_type(mipc_msg_t *msg_ptr, enum mipc_apn_auth_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_IA_REQ_T_AUTH_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_userid(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_SET_IA_REQ_T_USERID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_password(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_SET_IA_REQ_T_PASSWORD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_bearer_bitmask(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_SET_IA_REQ_T_BEARER_BITMASK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_compression(mipc_msg_t *msg_ptr, enum mipc_apn_compression_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_IA_REQ_T_COMPRESSION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_req_add_change_reason(mipc_msg_t *msg_ptr, enum mipc_apn_change_reason_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_IA_REQ_T_CHANGE_REASON, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_apn_set_ia_cnf_get_ia_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_SET_IA_CNF_T_IA_COUNT, def_val);
}

static inline mipc_apn_ia_struct4* mipc_apn_set_ia_cnf_get_ia_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_ia_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_SET_IA_CNF_T_IA_LIST, val_len_ptr);
}

static inline mipc_apn_ia_struct4* mipc_apn_set_ia_cnf_get_ia_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_ia_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_SET_IA_CNF_T_IA_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_apn_get_ia_cnf_get_ia_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_GET_IA_CNF_T_IA_COUNT, def_val);
}

static inline mipc_apn_ia_struct4* mipc_apn_get_ia_cnf_get_ia_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_ia_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_GET_IA_CNF_T_IA_LIST, val_len_ptr);
}

static inline mipc_apn_ia_struct4* mipc_apn_get_ia_cnf_get_ia_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_ia_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_GET_IA_CNF_T_IA_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_plmn_id(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_PLMN_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_apn_type(mipc_msg_t *msg_ptr, enum mipc_apn_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_APN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_pdp_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_PDP_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_roaming_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_ROAMING_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_auth_type(mipc_msg_t *msg_ptr, enum mipc_apn_auth_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_AUTH_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_userid(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_USERID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_password(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_PASSWORD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_bearer_bitmask(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_BEARER_BITMASK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_compression(mipc_msg_t *msg_ptr, enum mipc_apn_compression_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_COMPRESSION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_req_add_enabled(mipc_msg_t *msg_ptr, enum mipc_apn_enabled_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_REQ_T_ENABLED, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_apn_add_profile_cnf_get_apn_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_CNF_T_APN_COUNT, def_val);
}

static inline mipc_apn_profile_struct4* mipc_apn_add_profile_cnf_get_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_ADD_PROFILE_CNF_T_APN_LIST, val_len_ptr);
}

static inline mipc_apn_profile_struct4* mipc_apn_add_profile_cnf_get_apn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_ADD_PROFILE_CNF_T_APN_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_apn_list_profile_cnf_get_apn_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_LIST_PROFILE_CNF_T_APN_COUNT, def_val);
}

static inline mipc_apn_profile_struct4* mipc_apn_list_profile_cnf_get_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_LIST_PROFILE_CNF_T_APN_LIST, val_len_ptr);
}

static inline mipc_apn_profile_struct4* mipc_apn_list_profile_cnf_get_apn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_LIST_PROFILE_CNF_T_APN_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_apn_del_profile_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_DEL_PROFILE_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_apn_del_profile_cnf_get_apn_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_DEL_PROFILE_CNF_T_APN_COUNT, def_val);
}

static inline mipc_apn_profile_struct4* mipc_apn_del_profile_cnf_get_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_DEL_PROFILE_CNF_T_APN_LIST, val_len_ptr);
}

static inline mipc_apn_profile_struct4* mipc_apn_del_profile_cnf_get_apn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_DEL_PROFILE_CNF_T_APN_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_apn_set_profile_status_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_apn_profile_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_PROFILE_STATUS_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_apn_list_md_profile_cnf_get_apn_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_LIST_MD_PROFILE_CNF_T_APN_COUNT, def_val);
}

static inline mipc_md_apn_profile_struct4* mipc_apn_list_md_profile_cnf_get_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_md_apn_profile_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_LIST_MD_PROFILE_CNF_T_APN_LIST, val_len_ptr);
}

static inline mipc_md_apn_profile_struct4* mipc_apn_list_md_profile_cnf_get_apn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_md_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_LIST_MD_PROFILE_CNF_T_APN_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_req_add_apn_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_OP12_APN_REQ_T_APN_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_req_add_class(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_OP12_APN_REQ_T_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_req_add_network_identifier(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_SET_OP12_APN_REQ_T_NETWORK_IDENTIFIER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_req_add_pdp_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_OP12_APN_REQ_T_PDP_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_req_add_apn_bearer(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_SET_OP12_APN_REQ_T_APN_BEARER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_req_add_enabled(mipc_msg_t *msg_ptr, enum mipc_apn_enabled_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_OP12_APN_REQ_T_ENABLED, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_req_add_apn_timer(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_SET_OP12_APN_REQ_T_APN_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_req_add_apn_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_VZW_APN_REQ_T_APN_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_req_add_class(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_VZW_APN_REQ_T_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_req_add_network_identifier(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_SET_VZW_APN_REQ_T_NETWORK_IDENTIFIER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_req_add_pdp_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_VZW_APN_REQ_T_PDP_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_req_add_apn_bearer(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_APN_SET_VZW_APN_REQ_T_APN_BEARER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_req_add_enabled(mipc_msg_t *msg_ptr, enum mipc_apn_enabled_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_VZW_APN_REQ_T_ENABLED, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_req_add_apn_timer(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_SET_VZW_APN_REQ_T_APN_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_timer_req_add_apn_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_OP12_APN_TIMER_REQ_T_APN_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_timer_req_add_max_conn(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_OP12_APN_TIMER_REQ_T_MAX_CONN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_timer_req_add_max_conn_t(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_OP12_APN_TIMER_REQ_T_MAX_CONN_T, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_timer_req_add_wait_time(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_SET_OP12_APN_TIMER_REQ_T_WAIT_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_op12_apn_timer_req_add_throttle_time(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_SET_OP12_APN_TIMER_REQ_T_THROTTLE_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_timer_req_add_apn_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_VZW_APN_TIMER_REQ_T_APN_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_timer_req_add_max_conn(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_VZW_APN_TIMER_REQ_T_MAX_CONN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_timer_req_add_max_conn_t(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_VZW_APN_TIMER_REQ_T_MAX_CONN_T, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_timer_req_add_wait_time(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_SET_VZW_APN_TIMER_REQ_T_WAIT_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_set_vzw_apn_timer_req_add_throttle_time(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_APN_SET_VZW_APN_TIMER_REQ_T_THROTTLE_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_list_req_add_profile_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_LIST_REQ_T_PROFILE_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_list_req_add_profile_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_apn_profile_v2_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_apn_add_profile_list_req_add_profile_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_apn_profile_v2_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_apn_add_profile_list_cnf_get_profile_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_ADD_PROFILE_LIST_CNF_T_PROFILE_COUNT, def_val);
}

static inline mipc_apn_profile_v2_struct4* mipc_apn_add_profile_list_cnf_get_profile_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_v2_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_ADD_PROFILE_LIST_CNF_T_PROFILE_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_apn_profile_v2_struct4* mipc_apn_add_profile_list_cnf_get_profile_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_apn_profile_v2_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_ADD_PROFILE_LIST_CNF_T_PROFILE_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_apn_set_ia_md_prefer_req_add_ia_md_prefer(mipc_msg_t *msg_ptr, enum mipc_ia_md_prefer_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_APN_SET_IA_MD_PREFER_REQ_T_IA_MD_PREFER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_apn_op12_chg_ind_get_apn_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_OP12_CHG_IND_T_APN_COUNT, def_val);
}

static inline mipc_op12_apn_profile_legacy_struct4* mipc_apn_op12_chg_ind_get_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_op12_apn_profile_legacy_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_OP12_CHG_IND_T_APN_LIST, val_len_ptr);
}

static inline mipc_op12_apn_profile_struct4* mipc_apn_op12_chg_ind_get_apn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_op12_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_OP12_CHG_IND_T_APN_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_op12_apn_profile_struct4* mipc_apn_op12_chg_ind_get_ext_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_op12_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_OP12_CHG_IND_T_EXT_APN_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_apn_vzw_chg_ind_get_apn_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_APN_VZW_CHG_IND_T_APN_COUNT, def_val);
}

static inline mipc_op12_apn_profile_legacy_struct4* mipc_apn_vzw_chg_ind_get_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_op12_apn_profile_legacy_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_APN_VZW_CHG_IND_T_APN_LIST, val_len_ptr);
}

static inline mipc_op12_apn_profile_struct4* mipc_apn_vzw_chg_ind_get_apn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_op12_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_VZW_CHG_IND_T_APN_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_op12_apn_profile_struct4* mipc_apn_vzw_chg_ind_get_ext_apn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_op12_apn_profile_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_APN_VZW_CHG_IND_T_EXT_APN_LIST, index, val_len_ptr);
}


#endif /* __MIPC_MSG_APN_API_H__ */
