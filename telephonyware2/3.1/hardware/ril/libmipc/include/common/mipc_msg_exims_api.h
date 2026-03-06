#ifndef __MIPC_MSG_EXIMS_API_H__
#define __MIPC_MSG_EXIMS_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_exims_update_session_status_req_add_session_type(mipc_msg_t *msg_ptr, enum mipc_exims_action_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_SESSION_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_session_status_req_add_status(mipc_msg_t *msg_ptr, enum mipc_exims_session_status_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_session_status_req_add_callback_mode_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_CALLBACK_MODE_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_session_status_req_add_is_airplane_mode_emergency(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_IS_AIRPLANE_MODE_EMERGENCY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_get_call_domain_select_req_add_call_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_get_call_domain_select_req_add_call_type(mipc_msg_t *msg_ptr, enum mipc_exims_call_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_CALL_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_get_call_domain_select_req_add_available_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_AVAILABLE_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_get_call_domain_select_req_add_prefer_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_PREFER_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_exims_rat_const_enum mipc_exims_get_call_domain_select_cnf_get_rat(mipc_msg_t *msg_ptr, mipc_exims_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_rat_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_CNF_T_RAT, def_val);
}

static inline mipc_exims_call_domain_cause_const_enum mipc_exims_get_call_domain_select_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_exims_call_domain_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_call_domain_cause_const_enum)mipc_msg_get_val_uint16(msg_ptr, MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_CNF_T_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_call_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_call_type(mipc_msg_t *msg_ptr, enum mipc_exims_call_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CALL_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_call_status(mipc_msg_t *msg_ptr, enum mipc_exims_call_status_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CALL_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_cause(mipc_msg_t *msg_ptr, enum mipc_exims_call_failure_cause_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_is_far_end(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_IS_FAR_END, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_dir(mipc_msg_t *msg_ptr, enum mipc_call_direction_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_DIR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_address(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_ADDRESS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_exims_update_call_status_event_req_add_address_type(mipc_msg_t *msg_ptr, enum mipc_call_dial_address_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_ADDRESS_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_ims_ho_status_event_req_add_status(mipc_msg_t *msg_ptr, enum mipc_exims_ho_status_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_ims_ho_status_event_req_add_source_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_SOURCE_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_ims_ho_status_event_req_add_target_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_TARGET_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_ims_ho_status_event_req_add_result(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_get_sms_domain_select_req_add_sms_type(mipc_msg_t *msg_ptr, enum mipc_sms_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ_T_SMS_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_get_sms_domain_select_req_add_available_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ_T_AVAILABLE_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_get_sms_domain_select_req_add_prefer_domain(mipc_msg_t *msg_ptr, enum mipc_exims_sms_domain_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ_T_PREFER_DOMAIN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_exims_rat_const_enum mipc_exims_get_sms_domain_select_cnf_get_available_rat(mipc_msg_t *msg_ptr, mipc_exims_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_rat_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_CNF_T_AVAILABLE_RAT, def_val);
}

static inline mipc_exims_sms_domain_const_enum mipc_exims_get_sms_domain_select_cnf_get_sms_domain(mipc_msg_t *msg_ptr, mipc_exims_sms_domain_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_sms_domain_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_CNF_T_SMS_DOMAIN, def_val);
}

static inline mipc_msg_api_result_enum mipc_exims_update_sms_status_event_req_add_dir(mipc_msg_t *msg_ptr, enum mipc_sms_direction_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_DIR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_sms_status_event_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_sms_status_event_req_add_sms_domain(mipc_msg_t *msg_ptr, enum mipc_exims_sms_domain_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_SMS_DOMAIN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_sms_status_event_req_add_sms_type(mipc_msg_t *msg_ptr, enum mipc_sms_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_SMS_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_sms_status_event_req_add_status(mipc_msg_t *msg_ptr, enum mipc_exims_sms_status_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_reg_status_req_add_state(mipc_msg_t *msg_ptr, enum mipc_exims_reg_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_reg_status_req_add_type(mipc_msg_t *msg_ptr, enum mipc_exims_reg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_reg_status_req_add_ext_info(mipc_msg_t *msg_ptr, enum mipc_exims_ext_info_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_EXT_INFO, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_reg_status_req_add_dereg_cause(mipc_msg_t *msg_ptr, enum mipc_exims_dereg_cause_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_DEREG_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_reg_status_req_add_rat(mipc_msg_t *msg_ptr, enum mipc_exims_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_reg_status_req_add_sip_uri_type(mipc_msg_t *msg_ptr, enum mipc_exims_uri_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_SIP_URI_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_reg_status_req_add_ims_retry(mipc_msg_t *msg_ptr, enum mipc_exims_retry_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_IMS_RETRY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_config_req_add_is_ims_cfg_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_IMS_CFG_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_config_req_add_vo3gpp_cfg(mipc_msg_t *msg_ptr, enum mipc_exims_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_EXIMS_UPDATE_CONFIG_REQ_T_Vo3GPP_CFG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_config_req_add_vi3gpp_cfg(mipc_msg_t *msg_ptr, enum mipc_exims_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_EXIMS_UPDATE_CONFIG_REQ_T_Vi3GPP_CFG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_config_req_add_is_vowifi_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_VoWiFi_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_config_req_add_is_viwifi_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_ViWiFi_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_config_req_add_is_ims_sms_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_IMS_SMS_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_update_config_req_add_emergency_pdx_guard_timer(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_EXIMS_UPDATE_CONFIG_REQ_T_EMERGENCY_PDX_GUARD_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ecc_info_struct4* mipc_exims_get_ecc_list_cnf_get_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ecc_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EXIMS_GET_ECC_LIST_CNF_T_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_exims_set_scm_req_add_application(mipc_msg_t *msg_ptr, enum mipc_scm_application_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_SET_SCM_REQ_T_APPLICATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_set_scm_req_add_indication(mipc_msg_t *msg_ptr, enum mipc_scm_indication_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_SET_SCM_REQ_T_INDICATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_set_service_session_req_add_service_type(mipc_msg_t *msg_ptr, enum mipc_exims_service_type_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_SET_SERVICE_SESSION_REQ_T_SERVICE_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_set_service_session_req_add_service_status(mipc_msg_t *msg_ptr, enum mipc_exims_service_status_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_SET_SERVICE_SESSION_REQ_T_SERVICE_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_set_uac_req_add_service_type(mipc_msg_t *msg_ptr, enum mipc_exims_service_type_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_SET_UAC_REQ_T_SERVICE_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_exims_set_uac_req_add_service_status(mipc_msg_t *msg_ptr, enum mipc_exims_service_status_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EXIMS_SET_UAC_REQ_T_SERVICE_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_exims_uac_cause_enum_const_enum mipc_exims_set_uac_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_exims_uac_cause_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_uac_cause_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_SET_UAC_CNF_T_CAUSE, def_val);
}

static inline mipc_boolean_const_enum mipc_exims_imsvops_ind_get_is_support_imsvops(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_IMSVOPS_IND_T_IS_SUPPORT_IMSVOPS, def_val);
}

static inline mipc_ims_vops_n3gpp_enum_const_enum mipc_exims_imsvops_ind_get_nwimsvops_n3gpp(mipc_msg_t *msg_ptr, mipc_ims_vops_n3gpp_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_vops_n3gpp_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_IMSVOPS_IND_T_NWIMSVOPS_N3GPP, def_val);
}

static inline mipc_exims_rat_const_enum mipc_exims_emergency_service_support_ind_get_rat(mipc_msg_t *msg_ptr, mipc_exims_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_rat_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_EXIMS_EMERGENCY_SERVICE_SUPPORT_IND_T_RAT, def_val);
}

static inline mipc_boolean_const_enum mipc_exims_emergency_service_support_ind_get_is_support_emc(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_EMERGENCY_SERVICE_SUPPORT_IND_T_IS_SUPPORT_EMC, def_val);
}

static inline mipc_boolean_const_enum mipc_exims_emergency_bearer_support_ind_get_is_support_s1(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_IS_SUPPORT_S1, def_val);
}

static inline mipc_exims_emergency_support_const_enum mipc_exims_emergency_bearer_support_ind_get_ems_5g_supp(mipc_msg_t *msg_ptr, mipc_exims_emergency_support_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_emergency_support_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_EMS_5G_SUPP, def_val);
}

static inline mipc_exims_emergency_support_const_enum mipc_exims_emergency_bearer_support_ind_get_emf_5g_supp(mipc_msg_t *msg_ptr, mipc_exims_emergency_support_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_emergency_support_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_EMF_5G_SUPP, def_val);
}

static inline mipc_emergency_call_emb_iu_supp_const_enum mipc_exims_emergency_bearer_support_ind_get_emb_iu_supp(mipc_msg_t *msg_ptr, mipc_emergency_call_emb_iu_supp_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_emergency_call_emb_iu_supp_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_EMB_IU_SUPP, def_val);
}

static inline mipc_exims_call_bar_const_enum mipc_exims_unlock_call_bar_ind_get_bar(mipc_msg_t *msg_ptr, mipc_exims_call_bar_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_call_bar_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_UNLOCK_CALL_BAR_IND_T_BAR, def_val);
}

static inline mipc_exims_sms_bar_const_enum mipc_exims_unlock_sms_bar_ind_get_bar(mipc_msg_t *msg_ptr, mipc_exims_sms_bar_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_sms_bar_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_UNLOCK_SMS_BAR_IND_T_BAR, def_val);
}

static inline mipc_exims_reg_bar_const_enum mipc_exims_unlock_reg_bar_ind_get_bar(mipc_msg_t *msg_ptr, mipc_exims_reg_bar_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_exims_reg_bar_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EXIMS_UNLOCK_REG_BAR_IND_T_BAR, def_val);
}


#endif /* __MIPC_MSG_EXIMS_API_H__ */
