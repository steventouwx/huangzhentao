#ifndef __MIPC_MSG_PHB_API_H__
#define __MIPC_MSG_PHB_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_op(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_type(mipc_msg_t *msg_ptr, enum mipc_phb_ef_file_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_adn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_ADN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_ef_entry_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_EF_ENTRY_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_ton(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_TON, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_aas_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_AAS_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_grp_count(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_GRP_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_grp_id_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_line(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_LINE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_phb_set_upb_entry_req_add_encode_method(mipc_msg_t *msg_ptr, enum mipc_phb_encode_method_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_UPB_ENTRY_REQ_T_ENCODE_METHOD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_upb_anr_email_sne_entry_req_add_type(mipc_msg_t *msg_ptr, enum mipc_phb_ef_file_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_upb_anr_email_sne_entry_req_add_adn_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_ADN_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_upb_anr_email_sne_entry_req_add_ef_entry_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_EF_ENTRY_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_phb_anr_entry_struct4* mipc_phb_get_upb_anr_email_sne_entry_cnf_get_phb_entry(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_anr_entry_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_PHB_ENTRY, val_len_ptr);
}

static inline mipc_phb_email_str_struct4* mipc_phb_get_upb_anr_email_sne_entry_cnf_get_email(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_email_str_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_EMAIL, val_len_ptr);
}

static inline mipc_phb_name_str_struct4* mipc_phb_get_upb_anr_email_sne_entry_cnf_get_snestr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_name_str_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_SNESTR, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_phb_get_upb_aas_gas_grp_list_entry_req_add_type(mipc_msg_t *msg_ptr, enum mipc_phb_ef_file_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_upb_aas_gas_grp_list_entry_req_add_bindex(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ_T_BINDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_upb_aas_gas_grp_list_entry_req_add_eindex(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ_T_EINDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_aas_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_AAS_COUNT, def_val);
}

static inline mipc_phb_name_str_struct4* mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_aas_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_name_str_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_AAS_LIST, val_len_ptr);
}

static inline uint32_t mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_gas_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GAS_COUNT, def_val);
}

static inline mipc_phb_name_str_struct4* mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_gas_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_name_str_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GAS_LIST, val_len_ptr);
}

static inline uint32_t mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_grp_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GRP_COUNT, def_val);
}

static inline uint32_t mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_grp_id_list(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GRP_ID_LIST, def_val, index);
}

static inline mipc_phb_name_str_struct4* mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_aas_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_name_str_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_AAS_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_phb_name_str_struct4* mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_get_gas_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_name_str_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GAS_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_phb_get_phb_storage_info_req_add_storage_type(mipc_msg_t *msg_ptr, enum mipc_phb_stroage_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_PHB_STORAGE_INFO_REQ_T_STORAGE_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_phb_get_phb_storage_info_cnf_get_used(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_USED, def_val);
}

static inline uint32_t mipc_phb_get_phb_storage_info_cnf_get_total(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_TOTAL, def_val);
}

static inline uint32_t mipc_phb_get_phb_storage_info_cnf_get_nlength(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_NLENGTH, def_val);
}

static inline uint32_t mipc_phb_get_phb_storage_info_cnf_get_tlength(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_TLENGTH, def_val);
}

static inline mipc_phb_stroage_type_const_enum mipc_phb_get_phb_storage_info_cnf_get_storage_type(mipc_msg_t *msg_ptr, mipc_phb_stroage_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_phb_stroage_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_STORAGE_TYPE, def_val);
}

static inline mipc_msg_api_result_enum mipc_phb_set_phb_mem_storage_req_add_storage_type(mipc_msg_t *msg_ptr, enum mipc_phb_stroage_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_PHB_MEM_STORAGE_REQ_T_STORAGE_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_phb_mem_storage_req_add_password(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_PHB_SET_PHB_MEM_STORAGE_REQ_T_PASSWORD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_phb_get_phb_entry_req_add_bindex(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_PHB_ENTRY_REQ_T_BINDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_phb_entry_req_add_eindex(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_PHB_ENTRY_REQ_T_EINDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_phb_entry_req_add_storage_type(mipc_msg_t *msg_ptr, enum mipc_phb_stroage_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_PHB_ENTRY_REQ_T_STORAGE_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_phb_entry_req_add_ext(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_PHB_GET_PHB_ENTRY_REQ_T_EXT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_phb_get_phb_entry_cnf_get_entry_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_COUNT, def_val);
}

static inline mipc_phb_entry_struct4* mipc_phb_get_phb_entry_cnf_get_entry_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_entry_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_LIST, val_len_ptr);
}

static inline mipc_phb_entry_struct4* mipc_phb_get_phb_entry_cnf_get_entry_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_phb_entry_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_phb_set_phb_entry_req_add_type(mipc_msg_t *msg_ptr, enum mipc_phb_stroage_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_SET_PHB_ENTRY_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_phb_entry_req_add_ext(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_PHB_SET_PHB_ENTRY_REQ_T_EXT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_set_phb_entry_req_add_entry(mipc_msg_t *msg_ptr, uint16_t len, mipc_phb_entry_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_PHB_SET_PHB_ENTRY_REQ_T_ENTRY, len, (const void *)value);
}

static inline uint32_t mipc_phb_get_phb_stringslength_cnf_get_max_num_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_NUM_LEN, def_val);
}

static inline uint32_t mipc_phb_get_phb_stringslength_cnf_get_max_alpha_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_ALPHA_LEN, def_val);
}

static inline uint32_t mipc_phb_get_phb_stringslength_cnf_get_max_aas_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_AAS_LEN, def_val);
}

static inline uint32_t mipc_phb_get_phb_stringslength_cnf_get_max_gas_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_GAS_LEN, def_val);
}

static inline uint32_t mipc_phb_get_phb_stringslength_cnf_get_max_sne_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_SNE_LEN, def_val);
}

static inline uint32_t mipc_phb_get_phb_stringslength_cnf_get_max_email_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_EMAIL_LEN, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_num_anr(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_ANR, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_num_email(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_EMAIL, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_num_sne(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_SNE, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_num_aas(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_AAS, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_len_aas(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_LEN_AAS, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_num_gas(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_GAS, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_len_gas(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_LEN_GAS, def_val);
}

static inline uint32_t mipc_phb_get_upb_capability_cnf_get_num_grp(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_GRP, def_val);
}

static inline mipc_msg_api_result_enum mipc_phb_get_phb_available_req_add_type(mipc_msg_t *msg_ptr, enum mipc_phb_ef_file_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_phb_get_phb_available_req_add_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_phb_get_phb_available_cnf_get_max_num(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_NUM, def_val);
}

static inline uint32_t mipc_phb_get_phb_available_cnf_get_available_num(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_AVAILABLE_NUM, def_val);
}

static inline uint32_t mipc_phb_get_phb_available_cnf_get_max_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_LEN, def_val);
}

static inline uint8_t mipc_phb_ready_state_ind_get_ready(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_PHB_READY_STATE_IND_T_READY, def_val);
}


#endif /* __MIPC_MSG_PHB_API_H__ */
