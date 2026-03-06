#ifndef __MIPC_MSG_DATA_API_H__
#define __MIPC_MSG_DATA_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_apn_type(mipc_msg_t *msg_ptr, enum mipc_apn_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_APN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_pdp_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_PDP_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_roaming_type(mipc_msg_t *msg_ptr, enum mipc_apn_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_ROAMING_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_auth_type(mipc_msg_t *msg_ptr, enum mipc_apn_auth_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_AUTH_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_userid(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_USERID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_password(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_PASSWORD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_ipv4v6_fallback(mipc_msg_t *msg_ptr, enum mipc_data_fallback_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_IPV4V6_FALLBACK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_bearer_bitmask(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_BEARER_BITMASK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_reuse_flag(mipc_msg_t *msg_ptr, enum mipc_data_reuse_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_REUSE_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_if_type(mipc_msg_t *msg_ptr, enum mipc_data_bind_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_IF_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_apn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_APN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_ursp_traffic_desc(mipc_msg_t *msg_ptr, uint16_t len, mipc_ursp_traffic_desc_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_URSP_TRAFFIC_DESC, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_ursp_ue_local_conf(mipc_msg_t *msg_ptr, uint16_t len, mipc_ursp_ue_local_conf_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_URSP_UE_LOCAL_CONF, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_ursp_eval_flag(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_URSP_EVAL_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_if_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_IF_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_is_handover(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_IS_HANDOVER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_mbs_session_info_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_mbs_session_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_act_call_req_add_tsn_info(mipc_msg_t *msg_ptr, uint16_t len, mipc_data_tsn_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_ACT_CALL_REQ_T_TSN_INFO, len, (const void *)value);
}

static inline uint8_t mipc_data_act_call_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_CID, def_val);
}

static inline uint8_t mipc_data_act_call_cnf_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_ID, def_val);
}

static inline char * mipc_data_act_call_cnf_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_APN, val_len_ptr);
}

static inline mipc_apn_pdp_type_const_enum mipc_data_act_call_cnf_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PDP_TYPE, def_val);
}

static inline uint8_t mipc_data_act_call_cnf_get_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_cnf_get_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_cnf_get_dns_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_dns_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_dns_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_dns_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_dns_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_cnf_get_dns_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_dns_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_dns_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_dns_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_dns_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_cnf_get_pcscf_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_cnf_get_pcscf_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_pcscf_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_3, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_gw_v4(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_GW_V4, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_gw_v6(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_GW_V6, val_len_ptr);
}

static inline uint32_t mipc_data_act_call_cnf_get_mtu_v4(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_MTU_V4, def_val);
}

static inline uint32_t mipc_data_act_call_cnf_get_mtu_v6(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_MTU_V6, def_val);
}

static inline uint32_t mipc_data_act_call_cnf_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_INTERFACE_ID, def_val);
}

static inline uint8_t mipc_data_act_call_cnf_get_p_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_P_ID, def_val);
}

static inline uint8_t mipc_data_act_call_cnf_get_fb_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_FB_ID, def_val);
}

static inline uint32_t mipc_data_act_call_cnf_get_ipv4_netmask(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_IPv4_NETMASK, def_val);
}

static inline uint32_t mipc_data_act_call_cnf_get_ipv6_netmask(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_IPv6_NETMASK, def_val);
}

static inline uint32_t mipc_data_act_call_cnf_get_trans_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_TRANS_ID, def_val);
}

static inline mipc_ran_const_enum mipc_data_act_call_cnf_get_ran_info(mipc_msg_t *msg_ptr, mipc_ran_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ran_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_RAN_INFO, def_val);
}

static inline uint32_t mipc_data_act_call_cnf_get_bearer_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_BEARER_ID, def_val);
}

static inline uint8_t mipc_data_act_call_cnf_get_im_cn_signalling_flag(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_IM_CN_SIGNALLING_FLAG, def_val);
}

static inline uint32_t mipc_data_act_call_cnf_get_mtu_ethernet(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_MTU_ETHERNET, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_dns_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_dns_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_DNS_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_cnf_get_pcscf_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_cnf_get_pcscf_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_PCSCF_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_mbs_session_update_ind_struct4* mipc_data_act_call_cnf_get_mbs_session_update_ind(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbs_session_update_ind_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_MBS_SESSION_UPDATE_IND, val_len_ptr);
}

static inline mipc_cell_global_id_struct_struct4* mipc_data_act_call_cnf_get_cell_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_cell_global_id_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_CELL_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_tac_struct_struct4* mipc_data_act_call_cnf_get_tac_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_tac_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_CNF_T_TAC_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_deact_call_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_deact_call_req_add_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_deact_call_req_add_deact_reason(mipc_msg_t *msg_ptr, enum mipc_deact_reason_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_REQ_T_DEACT_REASON, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_data_deact_call_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_CNF_T_CID, def_val);
}

static inline uint8_t mipc_data_deact_call_cnf_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_CNF_T_ID, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_get_call_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_CALL_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_call_req_add_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_CALL_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_data_get_call_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_CID, def_val);
}

static inline uint8_t mipc_data_get_call_cnf_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_ID, def_val);
}

static inline char * mipc_data_get_call_cnf_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_APN, val_len_ptr);
}

static inline mipc_apn_pdp_type_const_enum mipc_data_get_call_cnf_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PDP_TYPE, def_val);
}

static inline uint8_t mipc_data_get_call_cnf_get_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_get_call_cnf_get_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_get_call_cnf_get_dns_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_dns_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_dns_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_dns_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_dns_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_get_call_cnf_get_dns_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_dns_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_dns_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_dns_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_dns_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_get_call_cnf_get_pcscf_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_get_call_cnf_get_pcscf_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_pcscf_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_3, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_gw_v4(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_GW_V4, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_gw_v6(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_GW_V6, val_len_ptr);
}

static inline uint32_t mipc_data_get_call_cnf_get_mtu_v4(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_MTU_V4, def_val);
}

static inline uint32_t mipc_data_get_call_cnf_get_mtu_v6(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_MTU_V6, def_val);
}

static inline uint32_t mipc_data_get_call_cnf_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_INTERFACE_ID, def_val);
}

static inline uint32_t mipc_data_get_call_cnf_get_ipv4_netmask(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_IPv4_NETMASK, def_val);
}

static inline uint32_t mipc_data_get_call_cnf_get_ipv6_netmask(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_IPv6_NETMASK, def_val);
}

static inline mipc_apn_type_const_enum mipc_data_get_call_cnf_get_apn_type(mipc_msg_t *msg_ptr, mipc_apn_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_APN_TYPE, def_val);
}

static inline uint32_t mipc_data_get_call_cnf_get_trans_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_TRANS_ID, def_val);
}

static inline mipc_ran_const_enum mipc_data_get_call_cnf_get_ran_info(mipc_msg_t *msg_ptr, mipc_ran_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ran_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_RAN_INFO, def_val);
}

static inline uint32_t mipc_data_get_call_cnf_get_bearer_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_BEARER_ID, def_val);
}

static inline uint8_t mipc_data_get_call_cnf_get_im_cn_signalling_flag(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_IM_CN_SIGNALLING_FLAG, def_val);
}

static inline uint8_t mipc_data_get_call_cnf_get_qfi(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_QFI, def_val);
}

static inline uint32_t mipc_data_get_call_cnf_get_mtu_ethernet(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_MTU_ETHERNET, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_dns_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_dns_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_DNS_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_get_call_cnf_get_pcscf_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_get_call_cnf_get_pcscf_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_CALL_CNF_T_PCSCF_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_packet_filter_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_packet_filter_req_add_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_packet_filter_req_add_filter_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_REQ_T_FILTER_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_packet_filter_req_add_filter_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_data_packet_filter_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_REQ_T_FILTER_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_packet_filter_req_add_filter_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_data_packet_filter_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_data_set_packet_filter_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_CNF_T_CID, def_val);
}

static inline uint8_t mipc_data_set_packet_filter_cnf_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_CNF_T_ID, def_val);
}

static inline uint8_t mipc_data_set_packet_filter_cnf_get_filter_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_CNF_T_FILTER_COUNT, def_val);
}

static inline mipc_data_packet_filter_struct4* mipc_data_set_packet_filter_cnf_get_filter_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_packet_filter_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_CNF_T_FILTER_LIST, val_len_ptr);
}

static inline mipc_data_packet_filter_struct4* mipc_data_set_packet_filter_cnf_get_filter_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_packet_filter_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_SET_PACKET_FILTER_CNF_T_FILTER_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_get_packet_filter_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_PACKET_FILTER_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_packet_filter_req_add_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_PACKET_FILTER_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_data_get_packet_filter_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PACKET_FILTER_CNF_T_CID, def_val);
}

static inline uint8_t mipc_data_get_packet_filter_cnf_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PACKET_FILTER_CNF_T_ID, def_val);
}

static inline uint8_t mipc_data_get_packet_filter_cnf_get_filter_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PACKET_FILTER_CNF_T_FILTER_COUNT, def_val);
}

static inline mipc_data_packet_filter_struct4* mipc_data_get_packet_filter_cnf_get_filter_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_packet_filter_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_PACKET_FILTER_CNF_T_FILTER_LIST, val_len_ptr);
}

static inline mipc_data_packet_filter_struct4* mipc_data_get_packet_filter_cnf_get_filter_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_packet_filter_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_PACKET_FILTER_CNF_T_FILTER_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_get_pco_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_PCO_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_pco_req_add_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_PCO_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_pco_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_GET_PCO_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_get_pco_req_add_apn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_GET_PCO_REQ_T_APN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_pco_req_add_pco_ie(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_GET_PCO_REQ_T_PCO_IE, len, (const void *)value);
}

static inline uint8_t mipc_data_get_pco_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PCO_CNF_T_CID, def_val);
}

static inline uint8_t mipc_data_get_pco_cnf_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PCO_CNF_T_ID, def_val);
}

static inline uint8_t mipc_data_get_pco_cnf_get_pco_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PCO_CNF_T_PCO_COUNT, def_val);
}

static inline mipc_data_pco_ie_struct4* mipc_data_get_pco_cnf_get_pco_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_pco_ie_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_PCO_CNF_T_PCO_LIST, val_len_ptr);
}

static inline mipc_data_pco_ie_struct4* mipc_data_get_pco_cnf_get_pco_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_pco_ie_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_PCO_CNF_T_PCO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_data_allow_req_add_clear(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_DATA_ALLOW_REQ_T_CLEAR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline void * mipc_data_get_md_data_call_list_cnf_get_cid_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_MD_DATA_CALL_LIST_CNF_T_CID_LIST, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_config_req_add_mobile_data(mipc_msg_t *msg_ptr, enum mipc_data_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_CONFIG_REQ_T_MOBILE_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_config_req_add_data_roaming(mipc_msg_t *msg_ptr, enum mipc_data_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_CONFIG_REQ_T_DATA_ROAMING, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_config_req_add_volte(mipc_msg_t *msg_ptr, enum mipc_data_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_CONFIG_REQ_T_VOLTE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_config_req_add_ims_test_mode(mipc_msg_t *msg_ptr, enum mipc_data_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_CONFIG_REQ_T_IMS_TEST_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_config_req_add_data_domestic_roaming(mipc_msg_t *msg_ptr, enum mipc_data_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_CONFIG_REQ_T_DATA_DOMESTIC_ROAMING, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_config_req_add_data_international_roaming(mipc_msg_t *msg_ptr, enum mipc_data_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_CONFIG_REQ_T_DATA_INTERNATIONAL_ROAMING, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_config_req_add_default_data_sim_card(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_CONFIG_REQ_T_DEFAULT_DATA_SIM_CARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_data_config_type_const_enum mipc_data_get_config_cnf_get_mobile_data(mipc_msg_t *msg_ptr, mipc_data_config_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_config_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CONFIG_CNF_T_MOBILE_DATA, def_val);
}

static inline mipc_data_config_type_const_enum mipc_data_get_config_cnf_get_data_roaming(mipc_msg_t *msg_ptr, mipc_data_config_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_config_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CONFIG_CNF_T_DATA_ROAMING, def_val);
}

static inline mipc_data_config_type_const_enum mipc_data_get_config_cnf_get_volte(mipc_msg_t *msg_ptr, mipc_data_config_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_config_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CONFIG_CNF_T_VOLTE, def_val);
}

static inline mipc_data_config_type_const_enum mipc_data_get_config_cnf_get_ims_test_mode(mipc_msg_t *msg_ptr, mipc_data_config_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_config_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CONFIG_CNF_T_IMS_TEST_MODE, def_val);
}

static inline mipc_data_config_type_const_enum mipc_data_get_config_cnf_get_data_domestic_roaming(mipc_msg_t *msg_ptr, mipc_data_config_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_config_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CONFIG_CNF_T_DATA_DOMESTIC_ROAMING, def_val);
}

static inline mipc_data_config_type_const_enum mipc_data_get_config_cnf_get_data_international_roaming(mipc_msg_t *msg_ptr, mipc_data_config_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_config_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CONFIG_CNF_T_DATA_INTERNATIONAL_ROAMING, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_abort_call_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_ABORT_CALL_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_get_call_info_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_GET_CALL_INFO_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_get_call_info_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_CALL_INFO_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_call_info_req_add_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_CALL_INFO_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_data_get_call_info_cnf_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_INFO_CNF_T_APN, val_len_ptr);
}

static inline uint8_t mipc_data_get_call_info_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_INFO_CNF_T_CID, def_val);
}

static inline uint8_t mipc_data_get_call_info_cnf_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_CALL_INFO_CNF_T_ID, def_val);
}

static inline mipc_data_nitz_info_struct4* mipc_data_get_call_info_cnf_get_estblished_time(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_nitz_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_INFO_CNF_T_ESTBLISHED_TIME, val_len_ptr);
}

static inline mipc_data_nitz_info_struct4* mipc_data_get_call_info_cnf_get_end_time(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_nitz_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_CALL_INFO_CNF_T_END_TIME, val_len_ptr);
}

static inline uint32_t mipc_data_get_call_info_cnf_get_reject_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_CALL_INFO_CNF_T_REJECT_CAUSE, def_val);
}

static inline uint8_t mipc_data_get_pdp_cid_cnf_get_min_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PDP_CID_CNF_T_MIN_CID, def_val);
}

static inline uint8_t mipc_data_get_pdp_cid_cnf_get_max_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PDP_CID_CNF_T_MAX_CID, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_retry_timer_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_data_retry_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_RETRY_TIMER_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_retry_timer_req_add_apn_name(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_RETRY_TIMER_REQ_T_APN_NAME, len, (const void *)value);
}

static inline mipc_data_retry_type_const_enum mipc_data_retry_timer_cnf_get_retry_type(mipc_msg_t *msg_ptr, mipc_data_retry_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_retry_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_RETRY_TIMER_CNF_T_RETRY_TYPE, def_val);
}

static inline uint32_t mipc_data_retry_timer_cnf_get_retry_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_RETRY_TIMER_CNF_T_RETRY_TIME, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_data_lce_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_hysteresis_ms(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_HYSTERESIS_MS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_hysteresis_dl_kbps(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_HYSTERESIS_DL_KBPS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_hysteresis_ul_kbps(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_HYSTERESIS_UL_KBPS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_threshold_dl_kbps_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_DL_KBPS_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_threshold_dl_kbps_list(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_DL_KBPS_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_threshold_ul_kbps_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_UL_KBPS_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_threshold_ul_kbps_list(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_UL_KBPS_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_link_capacity_reporting_criteria_req_add_access_network(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_ACCESS_NETWORK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_dedicate_bearer_info_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_DEDICATE_BEARER_INFO_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_data_secondary_pdp_context_info_struct4* mipc_data_get_dedicate_bearer_info_cnf_get_context_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_secondary_pdp_context_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_DEDICATE_BEARER_INFO_CNF_T_CONTEXT_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_secondary_pdp_context_info_struct4* mipc_data_get_dedicate_bearer_info_cnf_get_context_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_secondary_pdp_context_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_DEDICATE_BEARER_INFO_CNF_T_CONTEXT_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_get_qos_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_QOS_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_data_qos_info_struct4* mipc_data_get_qos_cnf_get_qos_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_qos_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_QOS_CNF_T_QOS_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_qos_info_struct4* mipc_data_get_qos_cnf_get_qos_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_qos_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_QOS_CNF_T_QOS_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_get_tft_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_TFT_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_data_tft_info_struct4* mipc_data_get_tft_cnf_get_tft_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_tft_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_TFT_CNF_T_TFT_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_tft_info_struct4* mipc_data_get_tft_cnf_get_tft_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_tft_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_TFT_CNF_T_TFT_LIST, index, val_len_ptr);
}

static inline mipc_data_tft_info_v1_struct4* mipc_data_get_tft_cnf_get_tft_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_tft_info_v1_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_TFT_CNF_T_TFT_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_lgdcont_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_LGDCONT_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_lgdcont_req_add_apn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_LGDCONT_REQ_T_APN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_lgdcont_req_add_request_type(mipc_msg_t *msg_ptr, enum mipc_data_lgdcont_req_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_LGDCONT_REQ_T_REQUEST_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_lgdcont_req_add_rat_type(mipc_msg_t *msg_ptr, enum mipc_data_lgdcont_rat_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_LGDCONT_REQ_T_RAT_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_nssai_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_NSSAI_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_nssai_req_add_apn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_NSSAI_REQ_T_APN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_nssai_req_add_snssai(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_NSSAI_REQ_T_SNSSAI, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_keepalive_req_add_stop_keepalive(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_KEEPALIVE_REQ_T_STOP_KEEPALIVE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_keepalive_req_add_start_keepalive(mipc_msg_t *msg_ptr, uint16_t len, mipc_data_start_keepalive_request_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_KEEPALIVE_REQ_T_START_KEEPALIVE, len, (const void *)value);
}

static inline uint32_t mipc_data_keepalive_cnf_get_session_handle(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_KEEPALIVE_CNF_T_SESSION_HANDLE, def_val);
}

static inline mipc_data_keepalive_status_code_const_enum mipc_data_keepalive_cnf_get_status_code(mipc_msg_t *msg_ptr, mipc_data_keepalive_status_code_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_keepalive_status_code_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_KEEPALIVE_CNF_T_STATUS_CODE, def_val);
}

static inline mipc_dsda_allowed_type_const_enum mipc_data_get_dsda_state_cnf_get_dsda_allowed(mipc_msg_t *msg_ptr, mipc_dsda_allowed_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dsda_allowed_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_DSDA_STATE_CNF_T_DSDA_ALLOWED, def_val);
}

static inline mipc_dsda_state_type_const_enum mipc_data_get_dsda_state_cnf_get_dsda_state(mipc_msg_t *msg_ptr, mipc_dsda_state_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dsda_state_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_DSDA_STATE_CNF_T_DSDA_STATE, def_val);
}

static inline mipc_dr_dsda_type_const_enum mipc_data_get_dsda_state_cnf_get_is_dr_dsda(mipc_msg_t *msg_ptr, mipc_dr_dsda_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dr_dsda_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_DSDA_STATE_CNF_T_IS_DR_DSDA, def_val);
}

static inline mipc_dr_dsds_type_const_enum mipc_data_get_dsda_state_cnf_get_is_dr_dsds(mipc_msg_t *msg_ptr, mipc_dr_dsds_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dr_dsds_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_DSDA_STATE_CNF_T_IS_DR_DSDS, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_get_5gqos_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_5GQOS_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_data_5gqos_info_struct4* mipc_data_get_5gqos_cnf_get_qos_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_5gqos_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_5GQOS_CNF_T_QOS_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_5gqos_info_struct4* mipc_data_get_5gqos_cnf_get_qos_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_5gqos_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_5GQOS_CNF_T_QOS_LIST, index, val_len_ptr);
}

static inline mipc_data_5gqos_info_v1_struct8* mipc_data_get_5gqos_cnf_get_qos_v1_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_5gqos_info_v1_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_5GQOS_CNF_T_QOS_V1_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_psi_req_add_action(mipc_msg_t *msg_ptr, enum mipc_psi_action_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_PSI_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_psi_req_add_psi(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_PSI_REQ_T_PSI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_psi_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_PSI_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_psi_req_add_apn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_PSI_REQ_T_APN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_data_set_psi_cnf_get_psi(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_SET_PSI_CNF_T_PSI, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_get_nssai_req_add_nssai_type(mipc_msg_t *msg_ptr, enum mipc_nssai_type_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_REQ_T_NSSAI_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_nssai_req_add_plmn_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_GET_NSSAI_REQ_T_PLMN_ID, len, (const void *)value);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_default_configured_nssai_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_NUM, def_val);
}

static inline mipc_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_default_configured_nssai_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_default_configured_nssai_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_rejected_nssai_3gpp_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_NUM, def_val);
}

static inline mipc_rejected_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_rejected_nssai_3gpp_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_rejected_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_rejected_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_rejected_nssai_3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_rejected_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_rejected_nssai_non3gpp_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_NUM, def_val);
}

static inline mipc_rejected_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_rejected_nssai_non3gpp_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_rejected_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_rejected_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_rejected_nssai_non3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_rejected_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_configured_nssai_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_NUM, def_val);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_configured_nssai_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_configured_nssai_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_allowed_nssai_3gpp_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_NUM, def_val);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_allowed_nssai_3gpp_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_allowed_nssai_3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_allowed_nssai_non3gpp_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_NUM, def_val);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_allowed_nssai_non3gpp_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_plmn_specific_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_allowed_nssai_non3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_plmn_specific_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_preferred_nssai_3gpp_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_NUM, def_val);
}

static inline mipc_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_preferred_nssai_3gpp_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_preferred_nssai_3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_data_get_nssai_cnf_get_preferred_nssai_non3gpp_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_NUM, def_val);
}

static inline mipc_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_preferred_nssai_non3gpp_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_s_nssai_struct_struct4* mipc_data_get_nssai_cnf_get_preferred_nssai_non3gpp_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_s_nssai_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_get_ursp_route_profile_req_add_cid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_ursp_route_profile_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_data_get_ursp_route_profile_cnf_get_cid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_CID, def_val);
}

static inline uint32_t mipc_data_get_ursp_route_profile_cnf_get_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ID, def_val);
}

static inline mipc_ursp_ue_local_conf_struct_struct4* mipc_data_get_ursp_route_profile_cnf_get_est_req_param(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ursp_ue_local_conf_struct_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_EST_REQ_PARAM, val_len_ptr);
}

static inline uint32_t mipc_data_get_ursp_route_profile_cnf_get_attr(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ATTR, def_val);
}

static inline uint32_t mipc_data_get_ursp_route_profile_cnf_get_route_supp_profile_list_num(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ROUTE_SUPP_PROFILE_LIST_NUM, def_val);
}

static inline mipc_ursp_get_route_supp_profile_ind_struct_struct4* mipc_data_get_ursp_route_profile_cnf_get_route_supp_profile_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ursp_get_route_supp_profile_ind_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ROUTE_SUPP_PROFILE_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_ursp_get_route_supp_profile_ind_struct_struct4* mipc_data_get_ursp_route_profile_cnf_get_route_supp_profile_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ursp_get_route_supp_profile_ind_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ROUTE_SUPP_PROFILE_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_ursp_preconf_ue_policy_req_add_plmn_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ_T_PLMN_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_ursp_preconf_ue_policy_req_add_rule_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ_T_RULE_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ursp_preconf_ue_policy_req_add_rule_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_ursp_rule_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_ursp_preconf_ue_policy_req_add_rule_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_ursp_rule_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_get_ursp_ue_policy_req_add_plmn_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_GET_URSP_UE_POLICY_REQ_T_PLMN_ID, len, (const void *)value);
}

static inline void * mipc_data_get_ursp_ue_policy_cnf_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_PLMN_ID, val_len_ptr);
}

static inline uint8_t mipc_data_get_ursp_ue_policy_cnf_get_rule_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RULE_NUM, def_val);
}

static inline mipc_ursp_rule_struct_struct4* mipc_data_get_ursp_ue_policy_cnf_get_rule_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ursp_rule_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RULE_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_ursp_rule_struct_struct4* mipc_data_get_ursp_ue_policy_cnf_get_rule_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ursp_rule_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RULE_LIST, index, val_len_ptr);
}

static inline void * mipc_data_get_ursp_ue_policy_cnf_get_raw_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RAW_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_reserved_if_id_req_add_if_id_list(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_RESERVED_IF_ID_REQ_T_IF_ID_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_pco_req_add_apn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_PCO_REQ_T_APN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_pco_req_add_apn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_SET_PCO_REQ_T_APN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_pco_req_add_pco_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_data_pco_ie_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_pco_req_add_pco_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_data_pco_ie_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_paging_req_add_paging_restrictions(mipc_msg_t *msg_ptr, enum mipc_data_paging_restrictions_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_PAGING_REQ_T_PAGING_RESTRICTIONS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_paging_req_add_cid(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_data_paging_restrictions_const_enum mipc_data_get_paging_cnf_get_paging_restrictions(mipc_msg_t *msg_ptr, mipc_data_paging_restrictions_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_paging_restrictions_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_PAGING_CNF_T_PAGING_RESTRICTIONS, def_val);
}

static inline uint8_t mipc_data_get_paging_cnf_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_DATA_GET_PAGING_CNF_T_CID, def_val, index);
}

static inline mipc_msg_api_result_enum mipc_data_mod_call_req_add_cid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_DATA_MOD_CALL_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_mod_call_req_add_mbs_session_info_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_mbs_session_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint32_t mipc_data_mod_call_cnf_get_cid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_CNF_T_CID, def_val);
}

static inline mipc_mbs_session_update_ind_struct4* mipc_data_mod_call_cnf_get_mbs_session_update_ind(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbs_session_update_ind_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_CNF_T_MBS_SESSION_UPDATE_IND, val_len_ptr);
}

static inline mipc_cell_global_id_struct_struct4* mipc_data_mod_call_cnf_get_cell_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_cell_global_id_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_CNF_T_CELL_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_tac_struct_struct4* mipc_data_mod_call_cnf_get_tac_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_tac_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_CNF_T_TAC_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_tsn_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_TSN_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_tsn_req_add_pmic(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_TSN_REQ_T_PMIC, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_tsn_req_add_mt_pmic_track_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_TSN_REQ_T_MT_PMIC_TRACK_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline void * mipc_data_set_tsn_cnf_get_pmic(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_SET_TSN_CNF_T_PMIC, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_set_bearer_info_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_BEARER_INFO_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_bearer_info_req_add_state(mipc_msg_t *msg_ptr, enum mipc_data_ims_bearer_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_BEARER_INFO_REQ_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_bearer_info_req_add_type(mipc_msg_t *msg_ptr, enum mipc_data_ims_bearer_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_BEARER_INFO_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_bearer_info_req_add_pdp_type(mipc_msg_t *msg_ptr, enum mipc_data_ims_bearer_pdp_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_BEARER_INFO_REQ_T_PDP_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_global_set(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_GLOBAL_SET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_ip_type(mipc_msg_t *msg_ptr, enum mipc_packet_filter_ip_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_IP_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_protocol(mipc_msg_t *msg_ptr, enum mipc_packet_filter_protocol_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_PROTOCOL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_ip_src(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_IP_SRC, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_ip_dst(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_IP_DST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_src_port(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_SRC_PORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_req_add_dst_port(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_DST_PORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_del_ip_tuple_packet_filter_req_add_global_set(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_DEL_IP_TUPLE_PACKET_FILTER_REQ_T_GLOBAL_SET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_del_ip_tuple_packet_filter_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_DEL_IP_TUPLE_PACKET_FILTER_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_mode_req_add_global_set(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_GLOBAL_SET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_mode_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_set_ip_tuple_packet_filter_mode_req_add_operation(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_OPERATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_ip_tuple_packet_filter_mode_req_add_global_set(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_GLOBAL_SET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_get_ip_tuple_packet_filter_mode_req_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_data_get_ip_tuple_packet_filter_mode_cnf_get_filter_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_CNF_T_FILTER_COUNT, def_val);
}

static inline uint8_t mipc_data_get_ip_tuple_packet_filter_mode_cnf_get_filter_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_CNF_T_FILTER_MODE, def_val);
}

static inline mipc_ursp_ue_policy_plmn_struct4* mipc_data_get_ursp_ue_policy_plmn_list_cnf_get_plmn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ursp_ue_policy_plmn_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_GET_URSP_UE_POLICY_PLMN_LIST_CNF_T_PLMN_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_data_act_packet_route_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_ipc_packet_route_app_id_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_PACKET_ROUTE_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ipc_packet_route_app_id_enum_const_enum mipc_data_act_packet_route_cnf_get_app_id(mipc_msg_t *msg_ptr, mipc_ipc_packet_route_app_id_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ipc_packet_route_app_id_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_PACKET_ROUTE_CNF_T_APP_ID, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_ifup_packet_route_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_ipc_packet_route_app_id_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ipc_packet_route_app_id_enum_const_enum mipc_data_ifup_packet_route_cnf_get_app_id(mipc_msg_t *msg_ptr, mipc_ipc_packet_route_app_id_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ipc_packet_route_app_id_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_CNF_T_APP_ID, def_val);
}

static inline void * mipc_data_dc_data_path_config_sync_cnf_get_client_ip(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_CLIENT_IP, val_len_ptr);
}

static inline void * mipc_data_dc_data_path_config_sync_cnf_get_server_ip(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_SERVER_IP, val_len_ptr);
}

static inline uint16_t mipc_data_dc_data_path_config_sync_cnf_get_dc_mss(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_DC_MSS, def_val);
}

static inline uint8_t mipc_data_act_call_ind_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_CID, def_val);
}

static inline uint8_t mipc_data_act_call_ind_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_ID, def_val);
}

static inline char * mipc_data_act_call_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_APN, val_len_ptr);
}

static inline mipc_apn_pdp_type_const_enum mipc_data_act_call_ind_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PDP_TYPE, def_val);
}

static inline uint8_t mipc_data_act_call_ind_get_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_ind_get_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_ind_get_dns_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_dns_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_dns_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_dns_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_dns_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_ind_get_dns_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_dns_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_dns_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_dns_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_dns_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_ind_get_pcscf_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_pcscf_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_pcscf_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_pcscf_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_pcscf_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_act_call_ind_get_pcscf_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_pcscf_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_pcscf_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_pcscf_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_pcscf_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_3, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_gw_v4(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_GW_V4, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_gw_v6(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_GW_V6, val_len_ptr);
}

static inline uint32_t mipc_data_act_call_ind_get_mtu_v4(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_MTU_V4, def_val);
}

static inline uint32_t mipc_data_act_call_ind_get_mtu_v6(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_MTU_V6, def_val);
}

static inline uint32_t mipc_data_act_call_ind_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_INTERFACE_ID, def_val);
}

static inline uint8_t mipc_data_act_call_ind_get_p_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_P_ID, def_val);
}

static inline uint8_t mipc_data_act_call_ind_get_fb_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_FB_ID, def_val);
}

static inline uint32_t mipc_data_act_call_ind_get_trans_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_TRANS_ID, def_val);
}

static inline uint32_t mipc_data_act_call_ind_get_ipv4_netmask(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_IPv4_NETMASK, def_val);
}

static inline mipc_ran_const_enum mipc_data_act_call_ind_get_ran_info(mipc_msg_t *msg_ptr, mipc_ran_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ran_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_RAN_INFO, def_val);
}

static inline uint32_t mipc_data_act_call_ind_get_bearer_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_BEARER_ID, def_val);
}

static inline uint8_t mipc_data_act_call_ind_get_im_cn_signalling_flag(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_IM_CN_SIGNALLING_FLAG, def_val);
}

static inline uint32_t mipc_data_act_call_ind_get_mtu_ethernet(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_MTU_ETHERNET, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_dns_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_dns_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_DNS_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_act_call_ind_get_pcscf_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_act_call_ind_get_pcscf_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_PCSCF_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_boolean_const_enum mipc_data_act_call_ind_get_is_dedicated_bearer(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_ACT_CALL_IND_T_IS_DEDICATED_BEARER, def_val);
}

static inline uint8_t mipc_data_deact_call_ind_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_IND_T_CID, def_val);
}

static inline uint8_t mipc_data_deact_call_ind_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_IND_T_ID, def_val);
}

static inline uint8_t mipc_data_deact_call_ind_get_res(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DEACT_CALL_IND_T_RES, def_val);
}

static inline uint32_t mipc_data_deact_call_ind_get_new_res(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_DEACT_CALL_IND_T_NEW_RES, def_val);
}

static inline uint8_t mipc_data_mod_call_ind_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_CID, def_val);
}

static inline uint8_t mipc_data_mod_call_ind_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_ID, def_val);
}

static inline char * mipc_data_mod_call_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_APN, val_len_ptr);
}

static inline mipc_apn_pdp_type_const_enum mipc_data_mod_call_ind_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PDP_TYPE, def_val);
}

static inline uint8_t mipc_data_mod_call_ind_get_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_mod_call_ind_get_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_mod_call_ind_get_dns_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_dns_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_dns_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_dns_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_dns_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_mod_call_ind_get_dns_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_dns_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_dns_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_dns_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_dns_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_V6_3, val_len_ptr);
}

static inline uint8_t mipc_data_mod_call_ind_get_pcscf_v4_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v4_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_0, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v4_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_1, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v4_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_2, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v4_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_3, val_len_ptr);
}

static inline uint8_t mipc_data_mod_call_ind_get_pcscf_v6_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v6_0(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_0, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v6_1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_1, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v6_2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_2, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_pcscf_v6_3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_3, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_gw_v4(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_GW_V4, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_gw_v6(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_GW_V6, val_len_ptr);
}

static inline uint32_t mipc_data_mod_call_ind_get_mtu_v4(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_MTU_V4, def_val);
}

static inline uint32_t mipc_data_mod_call_ind_get_mtu_v6(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_MTU_V6, def_val);
}

static inline uint32_t mipc_data_mod_call_ind_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_INTERFACE_ID, def_val);
}

static inline uint32_t mipc_data_mod_call_ind_get_trans_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_TRANS_ID, def_val);
}

static inline mipc_ran_const_enum mipc_data_mod_call_ind_get_ran_info(mipc_msg_t *msg_ptr, mipc_ran_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ran_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_RAN_INFO, def_val);
}

static inline uint32_t mipc_data_mod_call_ind_get_bearer_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_BEARER_ID, def_val);
}

static inline uint8_t mipc_data_mod_call_ind_get_im_cn_signalling_flag(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_IM_CN_SIGNALLING_FLAG, def_val);
}

static inline uint8_t mipc_data_mod_call_ind_get_p_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_P_ID, def_val);
}

static inline uint32_t mipc_data_mod_call_ind_get_mtu_ethernet(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_MTU_ETHERNET, def_val);
}

static inline uint32_t mipc_data_mod_call_ind_get_change_reason(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_CHANGE_REASON, def_val);
}

static inline mipc_data_mod_event_type_const_enum mipc_data_mod_call_ind_get_event_type(mipc_msg_t *msg_ptr, mipc_data_mod_event_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_mod_event_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_EVENT_TYPE, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_dns_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_dns_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_DNS_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_data_mod_call_ind_get_pcscf_ipv4_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_IPV4_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_data_mod_call_ind_get_pcscf_ipv6_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PCSCF_IPV6_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_boolean_const_enum mipc_data_mod_call_ind_get_is_dedicated_bearer(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_IS_DEDICATED_BEARER, def_val);
}

static inline uint8_t mipc_data_mod_call_ind_get_mt_pmic_track_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_MT_PMIC_TRACK_ID, def_val);
}

static inline void * mipc_data_mod_call_ind_get_pmic(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_CALL_IND_T_PMIC, val_len_ptr);
}

static inline uint8_t mipc_data_mod_pco_ind_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_PCO_IND_T_CID, def_val);
}

static inline uint8_t mipc_data_mod_pco_ind_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_PCO_IND_T_ID, def_val);
}

static inline uint8_t mipc_data_mod_pco_ind_get_pco_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_MOD_PCO_IND_T_PCO_COUNT, def_val);
}

static inline mipc_data_pco_ie_struct4* mipc_data_mod_pco_ind_get_pco_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_pco_ie_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MOD_PCO_IND_T_PCO_LIST, val_len_ptr);
}

static inline mipc_data_pco_ie_struct4* mipc_data_mod_pco_ind_get_pco_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_pco_ie_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_MOD_PCO_IND_T_PCO_TLV_ARRAY, index, val_len_ptr);
}

static inline uint32_t mipc_data_wwan_act_call_ind_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_INTERFACE_ID, def_val);
}

static inline uint32_t mipc_data_wwan_act_call_ind_get_cid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_CID, def_val);
}

static inline char * mipc_data_wwan_act_call_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_APN, val_len_ptr);
}

static inline mipc_apn_type_const_enum mipc_data_wwan_act_call_ind_get_apn_type(mipc_msg_t *msg_ptr, mipc_apn_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_APN_TYPE, def_val);
}

static inline mipc_apn_pdp_type_const_enum mipc_data_wwan_act_call_ind_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_PDP_TYPE, def_val);
}

static inline uint32_t mipc_data_wwan_act_call_ind_get_v4_mtu(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_MTU, def_val);
}

static inline uint32_t mipc_data_wwan_act_call_ind_get_v6_mtu(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_MTU, def_val);
}

static inline uint8_t mipc_data_wwan_act_call_ind_get_v4_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_ADDR_COUNT, def_val);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_v4_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_ADDR_LIST, val_len_ptr);
}

static inline uint8_t mipc_data_wwan_act_call_ind_get_v6_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_ADDR_COUNT, def_val);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_v6_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_ADDR_LIST, val_len_ptr);
}

static inline uint8_t mipc_data_wwan_act_call_ind_get_dns_v4_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V4_ADDR_COUNT, def_val);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_dns_v4_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V4_ADDR_LIST, val_len_ptr);
}

static inline uint8_t mipc_data_wwan_act_call_ind_get_dns_v6_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V6_ADDR_COUNT, def_val);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_dns_v6_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V6_ADDR_LIST, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_v4_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_v6_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_dns_v4_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V4_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_data_wwan_act_call_ind_get_dns_v6_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V6_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline uint32_t mipc_data_wwan_deact_call_ind_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_DEACT_CALL_IND_T_INTERFACE_ID, def_val);
}

static inline uint32_t mipc_data_wwan_deact_call_ind_get_cid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_DEACT_CALL_IND_T_CID, def_val);
}

static inline char * mipc_data_wwan_deact_call_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_WWAN_DEACT_CALL_IND_T_APN, val_len_ptr);
}

static inline mipc_apn_type_const_enum mipc_data_wwan_deact_call_ind_get_apn_type(mipc_msg_t *msg_ptr, mipc_apn_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_WWAN_DEACT_CALL_IND_T_APN_TYPE, def_val);
}

static inline uint32_t mipc_data_md_act_call_ind_get_cid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MD_ACT_CALL_IND_T_CID, def_val);
}

static inline char * mipc_data_md_act_call_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_MD_ACT_CALL_IND_T_APN, val_len_ptr);
}

static inline uint32_t mipc_data_md_act_call_ind_get_apn_idx(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MD_ACT_CALL_IND_T_APN_IDX, def_val);
}

static inline uint32_t mipc_data_md_deact_call_ind_get_cid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MD_DEACT_CALL_IND_T_CID, def_val);
}

static inline char * mipc_data_iwlan_priority_list_ind_get_cmd(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_CMD, val_len_ptr);
}

static inline char * mipc_data_iwlan_priority_list_ind_get_type(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_TYPE, val_len_ptr);
}

static inline mipc_mipc_eiwlpl_priority_type_const_enum mipc_data_iwlan_priority_list_ind_get_setup_priority(mipc_msg_t *msg_ptr, mipc_mipc_eiwlpl_priority_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mipc_eiwlpl_priority_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_SETUP_PRIORITY, def_val);
}

static inline mipc_mipc_eiwlpl_priority_type_const_enum mipc_data_iwlan_priority_list_ind_get_cellular_priority(mipc_msg_t *msg_ptr, mipc_mipc_eiwlpl_priority_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mipc_eiwlpl_priority_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_CELLULAR_PRIORITY, def_val);
}

static inline mipc_mipc_eiwlpl_priority_type_const_enum mipc_data_iwlan_priority_list_ind_get_wifi_priority(mipc_msg_t *msg_ptr, mipc_mipc_eiwlpl_priority_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mipc_eiwlpl_priority_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_WIFI_PRIORITY, def_val);
}

static inline char * mipc_data_iwlan_priority_list_ind_get_description(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_DESCRIPTION, val_len_ptr);
}

static inline uint8_t mipc_data_iwlan_priority_list_ind_get_rat_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_RAT_NUM, def_val);
}

static inline void * mipc_data_iwlan_priority_list_ind_get_rat_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_RAT_LIST, val_len_ptr);
}

static inline uint32_t mipc_data_link_capacity_estimate_ind_get_dl_kbps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_DL_KBPS, def_val);
}

static inline uint32_t mipc_data_link_capacity_estimate_ind_get_ul_kbps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_UL_KBPS, def_val);
}

static inline uint32_t mipc_data_link_capacity_estimate_ind_get_second_dl_kbps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_SECOND_DL_KBPS, def_val);
}

static inline uint32_t mipc_data_link_capacity_estimate_ind_get_second_ul_kbps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_SECOND_UL_KBPS, def_val);
}

static inline uint32_t mipc_data_nw_limit_ind_get_state(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_NW_LIMIT_IND_T_STATE, def_val);
}

static inline char * mipc_data_timer_ind_get_src_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_TIMER_IND_T_SRC_ID, val_len_ptr);
}

static inline char * mipc_data_timer_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_TIMER_IND_T_APN, val_len_ptr);
}

static inline uint32_t mipc_data_timer_ind_get_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_TIMER_IND_T_CAUSE, def_val);
}

static inline uint32_t mipc_data_timer_ind_get_timer_state(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_TIMER_IND_T_TIMER_STATE, def_val);
}

static inline uint32_t mipc_data_timer_ind_get_expire_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_TIMER_IND_T_EXPIRE_TIME, def_val);
}

static inline uint32_t mipc_data_keepalive_status_ind_get_session_handle(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_KEEPALIVE_STATUS_IND_T_SESSION_HANDLE, def_val);
}

static inline mipc_data_keepalive_status_code_const_enum mipc_data_keepalive_status_ind_get_status_code(mipc_msg_t *msg_ptr, mipc_data_keepalive_status_code_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_keepalive_status_code_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_KEEPALIVE_STATUS_IND_T_STATUS_CODE, def_val);
}

static inline uint32_t mipc_data_mobile_data_usage_ind_get_tx_bytes(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOBILE_DATA_USAGE_IND_T_TX_BYTES, def_val);
}

static inline uint32_t mipc_data_mobile_data_usage_ind_get_tx_packets(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOBILE_DATA_USAGE_IND_T_TX_PACKETS, def_val);
}

static inline uint32_t mipc_data_mobile_data_usage_ind_get_rx_bytes(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOBILE_DATA_USAGE_IND_T_RX_BYTES, def_val);
}

static inline uint32_t mipc_data_mobile_data_usage_ind_get_rx_packets(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_MOBILE_DATA_USAGE_IND_T_RX_PACKETS, def_val);
}

static inline uint16_t mipc_data_network_reject_cause_ind_get_emm_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_EMM_CAUSE, def_val);
}

static inline uint16_t mipc_data_network_reject_cause_ind_get_esm_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_ESM_CAUSE, def_val);
}

static inline uint16_t mipc_data_network_reject_cause_ind_get_event(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_EVENT, def_val);
}

static inline mipc_dsda_allowed_type_const_enum mipc_data_dsda_state_ind_get_dsda_allowed(mipc_msg_t *msg_ptr, mipc_dsda_allowed_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dsda_allowed_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DSDA_STATE_IND_T_DSDA_ALLOWED, def_val);
}

static inline mipc_dsda_state_type_const_enum mipc_data_dsda_state_ind_get_dsda_state(mipc_msg_t *msg_ptr, mipc_dsda_state_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dsda_state_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DSDA_STATE_IND_T_DSDA_STATE, def_val);
}

static inline mipc_dr_dsda_type_const_enum mipc_data_dsda_state_ind_get_is_dr_dsda(mipc_msg_t *msg_ptr, mipc_dr_dsda_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dr_dsda_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DSDA_STATE_IND_T_IS_DR_DSDA, def_val);
}

static inline mipc_dr_dsds_type_const_enum mipc_data_dsda_state_ind_get_is_dr_dsds(mipc_msg_t *msg_ptr, mipc_dr_dsds_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_dr_dsds_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_DSDA_STATE_IND_T_IS_DR_DSDS, def_val);
}

static inline uint8_t mipc_data_umts_ps_state_ind_get_conn_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_UMTS_PS_STATE_IND_T_CONN_STATUS, def_val);
}

static inline char * mipc_data_retry_timer_ind_get_apn_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_RETRY_TIMER_IND_T_APN_NAME, val_len_ptr);
}

static inline uint8_t mipc_data_ursp_reeval_ind_get_id_list_len(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_URSP_REEVAL_IND_T_ID_LIST_LEN, def_val);
}

static inline void * mipc_data_ursp_reeval_ind_get_id_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_URSP_REEVAL_IND_T_ID_LIST, val_len_ptr);
}

static inline mipc_ursp_reeval_type_enum_const_enum mipc_data_ursp_reeval_ind_get_event(mipc_msg_t *msg_ptr, mipc_ursp_reeval_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ursp_reeval_type_enum_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_URSP_REEVAL_IND_T_EVENT, def_val);
}

static inline void * mipc_data_ursp_ue_policy_chg_ind_get_plmn_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_URSP_UE_POLICY_CHG_IND_T_PLMN_ID, val_len_ptr);
}

static inline mipc_ursp_reeval_type_enum_const_enum mipc_data_ursp_ue_policy_chg_ind_get_event(mipc_msg_t *msg_ptr, mipc_ursp_reeval_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ursp_reeval_type_enum_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_URSP_UE_POLICY_CHG_IND_T_EVENT, def_val);
}

static inline uint32_t mipc_data_pdn_nw_cause_ind_get_emm_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_PDN_NW_CAUSE_IND_T_EMM_CAUSE, def_val);
}

static inline uint32_t mipc_data_pdn_nw_cause_ind_get_esm_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_DATA_PDN_NW_CAUSE_IND_T_ESM_CAUSE, def_val);
}

static inline char * mipc_data_pdn_nw_cause_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_PDN_NW_CAUSE_IND_T_APN, val_len_ptr);
}

static inline mipc_data_paging_restrict_result_const_enum mipc_data_paging_restrictions_ind_get_paging_restrict_result(mipc_msg_t *msg_ptr, mipc_data_paging_restrict_result_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_data_paging_restrict_result_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_PAGING_RESTRICTIONS_IND_T_PAGING_RESTRICT_RESULT, def_val);
}

static inline uint8_t mipc_data_tsn_time_ind_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_TSN_TIME_IND_T_CID, def_val);
}

static inline mipc_data_tsn_time_struct4* mipc_data_tsn_time_ind_get_nw_ref_time(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_tsn_time_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_DATA_TSN_TIME_IND_T_NW_REF_TIME, val_len_ptr);
}

static inline mipc_ipc_packet_route_app_id_enum_const_enum mipc_data_ifup_packet_route_ind_get_app_id(mipc_msg_t *msg_ptr, mipc_ipc_packet_route_app_id_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ipc_packet_route_app_id_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_APP_ID, def_val);
}

static inline uint8_t mipc_data_ifup_packet_route_ind_get_net_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_NET_TYPE, def_val);
}

static inline uint8_t mipc_data_ifup_packet_route_ind_get_nw_itf_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_NW_ITF_ID, def_val);
}

static inline uint8_t mipc_data_ifup_packet_route_ind_get_hw_ul_q(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_HW_UL_Q, def_val);
}

static inline mipc_ipc_packet_route_app_id_enum_const_enum mipc_data_ifup_packet_route_cmd_get_app_id(mipc_msg_t *msg_ptr, mipc_ipc_packet_route_app_id_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ipc_packet_route_app_id_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_APP_ID, def_val);
}

static inline uint8_t mipc_data_ifup_packet_route_cmd_get_net_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_NET_TYPE, def_val);
}

static inline uint8_t mipc_data_ifup_packet_route_cmd_get_nw_itf_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_NW_ITF_ID, def_val);
}

static inline uint8_t mipc_data_ifup_packet_route_cmd_get_hw_ul_q(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_HW_UL_Q, def_val);
}

static inline mipc_msg_api_result_enum mipc_data_ifup_packet_route_rsp_add_app_id(mipc_msg_t *msg_ptr, enum mipc_ipc_packet_route_app_id_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_IFUP_PACKET_ROUTE_RSP_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_ntf_add_cid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_NTF_T_CID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_ntf_add_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_NTF_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_call_ntf_add_response(mipc_msg_t *msg_ptr, enum mipc_act_call_ntf_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_CALL_NTF_T_RESPONSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_data_act_packet_route_ntf_add_app_id(mipc_msg_t *msg_ptr, enum mipc_ipc_packet_route_app_id_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_DATA_ACT_PACKET_ROUTE_NTF_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}


#endif /* __MIPC_MSG_DATA_API_H__ */
