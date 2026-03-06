#ifndef __MIPC_MSG_IMS_API_H__
#define __MIPC_MSG_IMS_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_ims_set_config_req_add_class(mipc_msg_t *msg_ptr, enum mipc_ims_config_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_IMS_SET_CONFIG_REQ_T_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_config_req_add_type(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_SET_CONFIG_REQ_T_TYPE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ims_set_config_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_SET_CONFIG_REQ_T_DATA, len, (const void *)value);
}

static inline void * mipc_ims_set_config_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SET_CONFIG_CNF_T_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ims_get_config_req_add_class(mipc_msg_t *msg_ptr, enum mipc_ims_config_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_IMS_GET_CONFIG_REQ_T_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_get_config_req_add_type(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_GET_CONFIG_REQ_T_TYPE, len, (const void *)value);
}

static inline void * mipc_ims_get_config_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_GET_CONFIG_CNF_T_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ims_get_state_req_add_event(mipc_msg_t *msg_ptr, enum mipc_ims_state_ind_event_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_GET_STATE_REQ_T_EVENT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ims_state_ind_event_const_enum mipc_ims_get_state_cnf_get_event(mipc_msg_t *msg_ptr, mipc_ims_state_ind_event_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_state_ind_event_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_EVENT, def_val);
}

static inline mipc_ims_state_const_enum mipc_ims_get_state_cnf_get_reg_state(mipc_msg_t *msg_ptr, mipc_ims_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_REG_STATE, def_val);
}

static inline uint32_t mipc_ims_get_state_cnf_get_ext_info(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_EXT_INFO, def_val);
}

static inline uint8_t mipc_ims_get_state_cnf_get_wfc(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_WFC, def_val);
}

static inline uint32_t mipc_ims_get_state_cnf_get_account_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_ACCOUNT_ID, def_val);
}

static inline void * mipc_ims_get_state_cnf_get_uri(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_URI, val_len_ptr);
}

static inline uint32_t mipc_ims_get_state_cnf_get_expire_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_EXPIRE_TIME, def_val);
}

static inline mipc_ims_reg_err_code_const_enum mipc_ims_get_state_cnf_get_error_code(mipc_msg_t *msg_ptr, mipc_ims_reg_err_code_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_reg_err_code_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_ERROR_CODE, def_val);
}

static inline char * mipc_ims_get_state_cnf_get_error_message(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_ERROR_MESSAGE, val_len_ptr);
}

static inline mipc_ims_reg_type_const_enum mipc_ims_get_state_cnf_get_reg_type(mipc_msg_t *msg_ptr, mipc_ims_reg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_reg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_STATE_CNF_T_REG_TYPE, def_val);
}

static inline mipc_msg_api_result_enum mipc_ims_set_pdis_req_add_transaction_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_PDIS_REQ_T_TRANSACTION_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_pdis_req_add_method(mipc_msg_t *msg_ptr, enum mipc_pdis_method_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_PDIS_REQ_T_METHOD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_pdis_req_add_is_success(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_PDIS_REQ_T_IS_SUCCESS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_transaction_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_TRANSACTION_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_mod_id(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_MOD_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_result(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_order(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_ORDER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_pref(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_PREF, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_flags(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_FLAGS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_service(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_SERVICE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_regexp(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_REGEXP, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ims_set_naptr_req_add_fqdn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_SET_NAPTR_REQ_T_FQDN, len, (const void *)value);
}

static inline mipc_ims_report_status_const_enum mipc_ims_get_nw_rpt_cnf_get_report_status(mipc_msg_t *msg_ptr, mipc_ims_report_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_report_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_NW_RPT_CNF_T_REPORT_STATUS, def_val);
}

static inline mipc_nw_ims_vops_status_const_enum mipc_ims_get_nw_rpt_cnf_get_nw_ims_vops(mipc_msg_t *msg_ptr, mipc_nw_ims_vops_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_nw_ims_vops_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_NW_RPT_CNF_T_NW_IMS_VOPS, def_val);
}

static inline mipc_msg_api_result_enum mipc_ims_set_test_mode_req_add_test_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_TEST_MODE_REQ_T_TEST_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_reg_state(mipc_msg_t *msg_ptr, enum mipc_ims_reg_state_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_REG_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_reg_type(mipc_msg_t *msg_ptr, enum mipc_ims_reg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_REG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_ext_info(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_EXT_INFO, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_dereg_cause(mipc_msg_t *msg_ptr, enum mipc_ims_dereg_cause_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_DEREG_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_ims_rat(mipc_msg_t *msg_ptr, enum mipc_ims_rat_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_IMS_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_sip_uri_type(mipc_msg_t *msg_ptr, enum mipc_ims_uri_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_SIP_URI_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_detail_ims_state(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_DETAIL_IMS_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_reason(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_REASON, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_eireg_req_add_ims_retry(mipc_msg_t *msg_ptr, enum mipc_ims_retry_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EIREG_REQ_T_IMS_RETRY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_evodata_req_add_en(mipc_msg_t *msg_ptr, enum mipc_ims_evodata_en_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EVODATA_REQ_T_EN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_evodata_req_add_data_mode(mipc_msg_t *msg_ptr, enum mipc_ims_evodata_mode_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EVODATA_REQ_T_DATA_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_evodata_req_add_allow_rat(mipc_msg_t *msg_ptr, enum mipc_ims_evodata_allowrat_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_SET_EVODATA_REQ_T_ALLOW_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_set_evodata_req_add_report_metric(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_IMS_SET_EVODATA_REQ_T_REPORT_METRIC, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_enable_data_channel_req_add_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_ENABLE_DATA_CHANNEL_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ims_ind_type_const_enum mipc_ims_get_pdn_cnf_get_ind_type(mipc_msg_t *msg_ptr, mipc_ims_ind_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_ind_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_IND_TYPE, def_val);
}

static inline uint32_t mipc_ims_get_pdn_cnf_get_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_ID, def_val);
}

static inline mipc_mipc_cid_act_state_const_enum mipc_ims_get_pdn_cnf_get_pdn_state(mipc_msg_t *msg_ptr, mipc_mipc_cid_act_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mipc_cid_act_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_PDN_STATE, def_val);
}

static inline mipc_apn_type_const_enum mipc_ims_get_pdn_cnf_get_apn_type(mipc_msg_t *msg_ptr, mipc_apn_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_APN_TYPE, def_val);
}

static inline uint32_t mipc_ims_get_pdn_cnf_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_INTERFACE_ID, def_val);
}

static inline uint8_t mipc_ims_get_pdn_cnf_get_v4_dns_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_V4_DNS_ADDR_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_ims_get_pdn_cnf_get_v4_dns_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_V4_DNS_ADDR_LIST, val_len_ptr);
}

static inline uint8_t mipc_ims_get_pdn_cnf_get_v6_dns_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_V6_DNS_ADDR_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_ims_get_pdn_cnf_get_v6_dns_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_V6_DNS_ADDR_LIST, val_len_ptr);
}

static inline mipc_ims_pdn_err_cause_const_enum mipc_ims_get_pdn_cnf_get_error_cause(mipc_msg_t *msg_ptr, mipc_ims_pdn_err_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_pdn_err_cause_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_GET_PDN_CNF_T_ERROR_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_ims_cfg_addition_service_req_add_service_cap(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_IMS_CFG_ADDITION_SERVICE_REQ_T_SERVICE_CAP, len, (const void *)value);
}

static inline mipc_ims_config_ind_reason_const_enum mipc_ims_config_ind_get_reason(mipc_msg_t *msg_ptr, mipc_ims_config_ind_reason_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_config_ind_reason_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_CONFIG_IND_T_REASON, def_val);
}

static inline void * mipc_ims_config_ind_get_config_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_CONFIG_IND_T_CONFIG_DATA, val_len_ptr);
}

static inline mipc_ims_state_ind_event_const_enum mipc_ims_state_ind_get_event(mipc_msg_t *msg_ptr, mipc_ims_state_ind_event_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_state_ind_event_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_STATE_IND_T_EVENT, def_val);
}

static inline mipc_ims_state_const_enum mipc_ims_state_ind_get_reg_state(mipc_msg_t *msg_ptr, mipc_ims_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_STATE_IND_T_REG_STATE, def_val);
}

static inline uint32_t mipc_ims_state_ind_get_ext_info(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_STATE_IND_T_EXT_INFO, def_val);
}

static inline uint8_t mipc_ims_state_ind_get_wfc(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_STATE_IND_T_WFC, def_val);
}

static inline uint32_t mipc_ims_state_ind_get_account_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_STATE_IND_T_ACCOUNT_ID, def_val);
}

static inline void * mipc_ims_state_ind_get_uri(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_STATE_IND_T_URI, val_len_ptr);
}

static inline uint32_t mipc_ims_state_ind_get_expire_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_STATE_IND_T_EXPIRE_TIME, def_val);
}

static inline mipc_ims_reg_err_code_const_enum mipc_ims_state_ind_get_error_code(mipc_msg_t *msg_ptr, mipc_ims_reg_err_code_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_reg_err_code_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_STATE_IND_T_ERROR_CODE, def_val);
}

static inline char * mipc_ims_state_ind_get_error_message(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_STATE_IND_T_ERROR_MESSAGE, val_len_ptr);
}

static inline mipc_ims_ecc_ind_rat_const_enum mipc_ims_support_ecc_ind_get_rat(mipc_msg_t *msg_ptr, mipc_ims_ecc_ind_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_ecc_ind_rat_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_SUPPORT_ECC_IND_T_RAT, def_val);
}

static inline mipc_boolean_const_enum mipc_ims_support_ecc_ind_get_support_emc(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_SUPPORT_ECC_IND_T_SUPPORT_EMC, def_val);
}

static inline mipc_ims_ind_type_const_enum mipc_ims_pdn_ind_get_ind_type(mipc_msg_t *msg_ptr, mipc_ims_ind_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_ind_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_PDN_IND_T_IND_TYPE, def_val);
}

static inline uint32_t mipc_ims_pdn_ind_get_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_PDN_IND_T_ID, def_val);
}

static inline mipc_mipc_cid_act_state_const_enum mipc_ims_pdn_ind_get_pdn_state(mipc_msg_t *msg_ptr, mipc_mipc_cid_act_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mipc_cid_act_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_PDN_IND_T_PDN_STATE, def_val);
}

static inline mipc_apn_type_const_enum mipc_ims_pdn_ind_get_apn_type(mipc_msg_t *msg_ptr, mipc_apn_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_PDN_IND_T_APN_TYPE, def_val);
}

static inline uint32_t mipc_ims_pdn_ind_get_interface_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_PDN_IND_T_INTERFACE_ID, def_val);
}

static inline uint8_t mipc_ims_pdn_ind_get_v4_dns_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_PDN_IND_T_V4_DNS_ADDR_COUNT, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_ims_pdn_ind_get_v4_dns_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_IMS_PDN_IND_T_V4_DNS_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v4_addr_struct4* mipc_ims_pdn_ind_get_v4_dns_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_IMS_PDN_IND_T_V4_DNS_ADDR_LIST, index, val_len_ptr);
}

static inline uint8_t mipc_ims_pdn_ind_get_v6_dns_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_PDN_IND_T_V6_DNS_ADDR_COUNT, def_val);
}

static inline mipc_data_v6_addr_struct4* mipc_ims_pdn_ind_get_v6_dns_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_IMS_PDN_IND_T_V6_DNS_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_ims_pdn_ind_get_v6_dns_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_IMS_PDN_IND_T_V6_DNS_ADDR_LIST, index, val_len_ptr);
}

static inline mipc_ims_pdn_err_cause_const_enum mipc_ims_pdn_ind_get_error_cause(mipc_msg_t *msg_ptr, mipc_ims_pdn_err_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_pdn_err_cause_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_PDN_IND_T_ERROR_CAUSE, def_val);
}

static inline uint8_t mipc_ims_naptr_ind_get_transaction_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_NAPTR_IND_T_TRANSACTION_ID, def_val);
}

static inline char * mipc_ims_naptr_ind_get_mod_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_NAPTR_IND_T_MOD_ID, val_len_ptr);
}

static inline char * mipc_ims_naptr_ind_get_fqdn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_NAPTR_IND_T_FQDN, val_len_ptr);
}

static inline mipc_ims_state_const_enum mipc_ims_reg_ind_get_reg_state(mipc_msg_t *msg_ptr, mipc_ims_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_REG_IND_T_REG_STATE, def_val);
}

static inline mipc_ims_reg_type_const_enum mipc_ims_reg_ind_get_reg_type(mipc_msg_t *msg_ptr, mipc_ims_reg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_reg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_REG_IND_T_REG_TYPE, def_val);
}

static inline uint32_t mipc_ims_reg_ind_get_ext_info(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_REG_IND_T_EXT_INFO, def_val);
}

static inline uint8_t mipc_ims_reg_ind_get_dereg_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_REG_IND_T_DEREG_CAUSE, def_val);
}

static inline uint8_t mipc_ims_reg_ind_get_ims_retry(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_REG_IND_T_IMS_RETRY, def_val);
}

static inline uint8_t mipc_ims_reg_ind_get_rat(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_REG_IND_T_RAT, def_val);
}

static inline uint8_t mipc_ims_reg_ind_get_sip_uri_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_REG_IND_T_SIP_URI_TYPE, def_val);
}

static inline mipc_ims_state_const_enum mipc_ims_reg_ind_get_reg_sub_state(mipc_msg_t *msg_ptr, mipc_ims_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_REG_IND_T_REG_SUB_STATE, def_val);
}

static inline uint32_t mipc_ims_sip_reg_info_ind_get_account_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_ACCOUNT_ID, def_val);
}

static inline char * mipc_ims_sip_reg_info_ind_get_direction(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_DIRECTION, val_len_ptr);
}

static inline char * mipc_ims_sip_reg_info_ind_get_sip_msg_type(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_SIP_MSG_TYPE, val_len_ptr);
}

static inline char * mipc_ims_sip_reg_info_ind_get_method(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_METHOD, val_len_ptr);
}

static inline uint32_t mipc_ims_sip_reg_info_ind_get_response_code(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_RESPONSE_CODE, def_val);
}

static inline char * mipc_ims_sip_reg_info_ind_get_reason_phrase(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_REASON_PHRASE, val_len_ptr);
}

static inline char * mipc_ims_sip_reg_info_ind_get_warn_text(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_WARN_TEXT, val_len_ptr);
}

static inline uint32_t mipc_ims_sip_reg_info_ind_get_cseq(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_CSEQ, def_val);
}

static inline uint32_t mipc_ims_sip_reg_info_ind_get_reason_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_REASON_CAUSE, def_val);
}

static inline char * mipc_ims_sip_reg_info_ind_get_reason_text(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_REASON_TEXT, val_len_ptr);
}

static inline char * mipc_ims_sip_reg_info_ind_get_call_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_SIP_REG_INFO_IND_T_CALL_ID, val_len_ptr);
}

static inline uint8_t mipc_ims_vops_ind_get_nwimsvops(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_VOPS_IND_T_NWIMSVOPS, def_val);
}

static inline mipc_ims_vops_n3gpp_enum_const_enum mipc_ims_vops_ind_get_nwimsvops_n3gpp(mipc_msg_t *msg_ptr, mipc_ims_vops_n3gpp_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_vops_n3gpp_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_VOPS_IND_T_NWIMSVOPS_N3GPP, def_val);
}

static inline uint32_t mipc_ims_reg_remain_time_ind_get_reg_remain_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_REG_REMAIN_TIME_IND_T_REG_REMAIN_TIME, def_val);
}

static inline uint32_t mipc_ims_reg_remain_time_ind_get_sub_remain_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_IMS_REG_REMAIN_TIME_IND_T_SUB_REMAIN_TIME, def_val);
}

static inline uint8_t mipc_ims_ui_ind_get_iconflag(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_UI_IND_T_ICONFLAG, def_val);
}

static inline uint8_t mipc_ims_pdis_cmd_get_transaction_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_PDIS_CMD_T_TRANSACTION_ID, def_val);
}

static inline mipc_em_id_type_const_enum mipc_ims_pdis_cmd_get_em_id(mipc_msg_t *msg_ptr, mipc_em_id_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_em_id_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_PDIS_CMD_T_EM_ID, def_val);
}

static inline mipc_pdis_method_type_const_enum mipc_ims_pdis_cmd_get_method(mipc_msg_t *msg_ptr, mipc_pdis_method_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_pdis_method_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_IMS_PDIS_CMD_T_METHOD, def_val);
}

static inline char * mipc_ims_pdis_cmd_get_nw_interface_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_IMS_PDIS_CMD_T_NW_INTERFACE_NAME, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ims_pdis_rsp_add_transaction_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_PDIS_RSP_T_TRANSACTION_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_pdis_rsp_add_method(mipc_msg_t *msg_ptr, enum mipc_pdis_method_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_PDIS_RSP_T_METHOD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ims_pdis_rsp_add_is_success(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_IMS_PDIS_RSP_T_IS_SUCCESS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}


#endif /* __MIPC_MSG_IMS_API_H__ */
