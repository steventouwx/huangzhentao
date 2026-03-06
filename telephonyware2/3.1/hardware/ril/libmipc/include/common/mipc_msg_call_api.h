#ifndef __MIPC_MSG_CALL_API_H__
#define __MIPC_MSG_CALL_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_dial_address(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_dial_address_type(mipc_msg_t *msg_ptr, enum mipc_call_dial_address_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_type(mipc_msg_t *msg_ptr, enum mipc_call_dial_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_DIAL_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_domain(mipc_msg_t *msg_ptr, enum mipc_call_dial_domain_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_DIAL_REQ_T_DOMAIN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_ecc_retry_domain(mipc_msg_t *msg_ptr, enum mipc_call_dial_domain_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_DIAL_REQ_T_ECC_RETRY_DOMAIN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_ecc_category(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_CALL_DIAL_REQ_T_ECC_CATEGORY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_clir(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DIAL_REQ_T_CLIR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_is_ecc_testing(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DIAL_REQ_T_IS_ECC_TESTING, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dial_req_add_clir_ext(mipc_msg_t *msg_ptr, enum mipc_call_clir_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DIAL_REQ_T_CLIR_EXT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ss_req_add_action(mipc_msg_t *msg_ptr, enum mipc_call_ss_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SS_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ss_req_add_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SS_REQ_T_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ss_req_add_ect_type(mipc_msg_t *msg_ptr, enum mipc_call_ect_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_SS_REQ_T_ECT_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ss_req_add_ect_number(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_SS_REQ_T_ECT_NUMBER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_hangup_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_call_hangup_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_HANGUP_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_hangup_req_add_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_HANGUP_REQ_T_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_hangup_req_add_cause(mipc_msg_t *msg_ptr, enum mipc_call_hangup_cause_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_HANGUP_REQ_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_answer_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_call_answer_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_ANSWER_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_answer_req_add_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_ANSWER_REQ_T_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_get_call_status_req_add_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_REQ_T_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_call_get_call_status_cnf_get_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_COUNT, def_val);
}

static inline uint32_t mipc_call_get_call_status_cnf_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_CALLID, def_val, index);
}

static inline mipc_call_direction_const_enum mipc_call_get_call_status_cnf_get_direction(mipc_msg_t *msg_ptr, mipc_call_direction_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_direction_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_DIRECTION, def_val, index);
}

static inline mipc_call_mode_const_enum mipc_call_get_call_status_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_call_mode_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_mode_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_MODE, def_val, index);
}

static inline mipc_call_clcc_state_const_enum mipc_call_get_call_status_cnf_get_call_clcc_state(mipc_msg_t *msg_ptr, mipc_call_clcc_state_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_clcc_state_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_CALL_CLCC_STATE, def_val, index);
}

static inline mipc_call_dial_address_type_const_enum mipc_call_get_call_status_cnf_get_number_type(mipc_msg_t *msg_ptr, mipc_call_dial_address_type_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_dial_address_type_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_NUMBER_TYPE, def_val, index);
}

static inline uint32_t mipc_call_get_call_status_cnf_get_ton(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_TON, def_val, index);
}

static inline char * mipc_call_get_call_status_cnf_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_NUMBER, index, val_len_ptr);
}

static inline mipc_call_rat_const_enum mipc_call_get_call_status_cnf_get_rat(mipc_msg_t *msg_ptr, mipc_call_rat_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_rat_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_RAT, def_val, index);
}

static inline mipc_call_type_const_enum mipc_call_get_call_status_cnf_get_type(mipc_msg_t *msg_ptr, mipc_call_type_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_type_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_TYPE, def_val, index);
}

static inline mipc_call_detail_info_struct4* mipc_call_get_call_status_cnf_get_detail_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_call_detail_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_DETAIL_INFO, index, val_len_ptr);
}

static inline mipc_call_video_cap_struct4* mipc_call_get_call_status_cnf_get_video_cap(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_call_video_cap_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_CALL_STATUS_CNF_T_VIDEO_CAP, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_call_conference_req_add_conf_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_CONFERENCE_REQ_T_CONF_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_conference_req_add_action(mipc_msg_t *msg_ptr, enum mipc_call_conf_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_CONFERENCE_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_conference_req_add_number(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_CONFERENCE_REQ_T_NUMBER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_conference_req_add_target_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_CONFERENCE_REQ_T_TARGET_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_call_reject_cause_const_enum mipc_call_conference_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_call_reject_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_reject_cause_const_enum)mipc_msg_get_val_uint16(msg_ptr, MIPC_CALL_CONFERENCE_CNF_T_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_get_conference_info_req_add_conf_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_REQ_T_CONF_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_call_get_conference_info_cnf_get_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_COUNT, def_val);
}

static inline mipc_call_direction_const_enum mipc_call_get_conference_info_cnf_get_direction(mipc_msg_t *msg_ptr, mipc_call_direction_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_direction_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_DIRECTION, def_val, index);
}

static inline char * mipc_call_get_conference_info_cnf_get_participant_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_NUMBER, index, val_len_ptr);
}

static inline char * mipc_call_get_conference_info_cnf_get_participant_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_NAME, index, val_len_ptr);
}

static inline mipc_conf_participant_status_const_enum mipc_call_get_conference_info_cnf_get_participant_status(mipc_msg_t *msg_ptr, mipc_conf_participant_status_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_conf_participant_status_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_STATUS, def_val, index);
}

static inline char * mipc_call_get_conference_info_cnf_get_participant_user_entity(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_USER_ENTITY, index, val_len_ptr);
}

static inline char * mipc_call_get_conference_info_cnf_get_participant_endpoint_entity(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_ENDPOINT_ENTITY, index, val_len_ptr);
}

static inline uint32_t mipc_call_get_finish_reason_cnf_get_reason(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GET_FINISH_REASON_CNF_T_REASON, def_val);
}

static inline char * mipc_call_get_finish_reason_cnf_get_reason_str(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_GET_FINISH_REASON_CNF_T_REASON_STR, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_call_dtmf_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_dtmf_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_DTMF_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dtmf_req_add_digit(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_DTMF_REQ_T_DIGIT, len, (const void *)value);
}

static inline uint32_t mipc_call_get_ecc_list_cnf_get_info_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_COUNT, def_val);
}

static inline mipc_ecc_info_struct4* mipc_call_get_ecc_list_cnf_get_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ecc_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_LIST, val_len_ptr);
}

static inline mipc_ecc_info_struct4* mipc_call_get_ecc_list_cnf_get_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ecc_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_call_set_ecc_list_req_add_info_count(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_ecc_list_req_add_info_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_ecc_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_set_ecc_list_req_add_info_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_ecc_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_set_flight_mode_ecc_session_req_add_is_flight_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ_T_IS_FLIGHT_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ivs_onekey_ecall_req_add_type(mipc_msg_t *msg_ptr, enum mipc_ecall_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ivs_onekey_ecall_req_add_msd_format(mipc_msg_t *msg_ptr, enum mipc_ecall_msd_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_MSD_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ivs_onekey_ecall_req_add_msd(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_MSD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_ivs_onekey_ecall_req_add_domain(mipc_msg_t *msg_ptr, enum mipc_call_dial_domain_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_DOMAIN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_sip_header_req_add_total(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_SIP_HEADER_REQ_T_TOTAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_sip_header_req_add_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_SIP_HEADER_REQ_T_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_sip_header_req_add_count(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_SIP_HEADER_REQ_T_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_sip_header_req_add_value_pair(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_SET_SIP_HEADER_REQ_T_VALUE_PAIR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_enable_ims_sip_header_report_req_add_call_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_enable_ims_sip_header_report_req_add_header_type(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ_T_HEADER_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_call_additional_info_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_call_additional_info_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_call_additional_info_req_add_type(mipc_msg_t *msg_ptr, enum mipc_call_additional_info_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_call_additional_info_req_add_total(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_TOTAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_call_additional_info_req_add_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_call_additional_info_req_add_count(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_call_additional_info_req_add_additional_info(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_ADDITIONAL_INFO, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_peer_rtt_modify_req_add_call_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_PEER_RTT_MODIFY_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_peer_rtt_modify_req_add_result(mipc_msg_t *msg_ptr, enum mipc_call_peer_rtt_modify_result_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_PEER_RTT_MODIFY_REQ_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_local_rtt_modify_req_add_call_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_LOCAL_RTT_MODIFY_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_local_rtt_modify_req_add_op(mipc_msg_t *msg_ptr, enum mipc_call_local_rtt_modify_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_LOCAL_RTT_MODIFY_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_call_local_rtt_modify_cnf_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_LOCAL_RTT_MODIFY_CNF_T_CALL_ID, def_val);
}

static inline uint8_t mipc_call_local_rtt_modify_cnf_get_result(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_LOCAL_RTT_MODIFY_CNF_T_RESULT, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_rtt_text_req_add_call_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_RTT_TEXT_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_rtt_text_req_add_len(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_RTT_TEXT_REQ_T_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_rtt_text_req_add_text(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_RTT_TEXT_REQ_T_TEXT, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_rtt_text_req_add_m_bit(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_RTT_TEXT_REQ_T_M_BIT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_rtt_mode_req_add_op(mipc_msg_t *msg_ptr, enum mipc_call_rtt_mode_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_RTT_MODE_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_rtt_audio_req_add_call_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_RTT_AUDIO_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_rtt_audio_req_add_enable(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_RTT_AUDIO_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_rcs_state_and_feature_req_add_state(mipc_msg_t *msg_ptr, enum mipc_call_rcs_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_RCS_STATE_AND_FEATURE_REQ_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_rcs_state_and_feature_req_add_feature(mipc_msg_t *msg_ptr, enum mipc_call_rcs_feature_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_RCS_STATE_AND_FEATURE_REQ_T_FEATURE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_update_rcs_session_info_req_add_status(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_UPDATE_RCS_SESSION_INFO_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_call_voice_domain_const_enum mipc_call_get_voice_domain_preference_cnf_get_setting(mipc_msg_t *msg_ptr, mipc_call_voice_domain_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_voice_domain_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF_T_SETTING, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_pull_req_add_uri(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_PULL_REQ_T_URI, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_pull_req_add_type(mipc_msg_t *msg_ptr, enum mipc_call_pull_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_PULL_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_call_tty_mode_const_enum mipc_call_get_tty_mode_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_call_tty_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_tty_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_GET_TTY_MODE_CNF_T_MODE, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_set_ims_call_mode_req_add_op(mipc_msg_t *msg_ptr, enum mipc_call_ims_call_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_SET_IMS_CALL_MODE_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_tty_mode_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_call_tty_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_SET_TTY_MODE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_voice_domain_preference_req_add_setting(mipc_msg_t *msg_ptr, enum mipc_call_voice_domain_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ_T_SETTING, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_conference_dial_req_add_type(mipc_msg_t *msg_ptr, enum mipc_call_conference_dial_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_CONFERENCE_DIAL_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_conference_dial_req_add_clir(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_CONFERENCE_DIAL_REQ_T_CLIR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_conference_dial_req_add_count(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_CONFERENCE_DIAL_REQ_T_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_conference_dial_req_add_dial_address(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_conference_dial_req_add_clir_ext(mipc_msg_t *msg_ptr, enum mipc_call_clir_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_CONFERENCE_DIAL_REQ_T_CLIR_EXT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_mode_req_add_action(mipc_msg_t *msg_ptr, enum mipc_set_gwsd_mode_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_GWSD_MODE_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_mode_req_add_mode(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_GWSD_MODE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_mode_req_add_ka_mode(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_GWSD_MODE_REQ_T_KA_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_mode_req_add_ka_cycle(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_SET_GWSD_MODE_REQ_T_KA_CYCLE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_approve_csfb_req_add_is_approve(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_APPROVE_CSFB_REQ_T_IS_APPROVE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_call_valid_req_add_timer(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_GWSD_CALL_VALID_REQ_T_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_ignore_call_interval_req_add_interval(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_SET_GWSD_IGNORE_CALL_INTERVAL_REQ_T_INTERVAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_ka_pdcp_req_add_pdata(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_SET_GWSD_KA_PDCP_REQ_T_PDATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_set_gwsd_ka_ipdata_req_add_pdata(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_SET_GWSD_KA_IPDATA_REQ_T_PDATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_uis_info_req_add_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_UIS_INFO_REQ_T_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_uis_info_req_add_type(mipc_msg_t *msg_ptr, enum mipc_call_uis_info_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_UIS_INFO_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_uis_info_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_CALL_UIS_INFO_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_uis_info_req_add_duration(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_UIS_INFO_REQ_T_DURATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_auto_answer_req_add_enable(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_AUTO_ANSWER_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_data_prefer_set_req_add_state(mipc_msg_t *msg_ptr, enum mipc_call_prefer_set_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DATA_PREFER_SET_REQ_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_data_prefer_set_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_call_prefer_set_monitor_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DATA_PREFER_SET_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ecc_redial_approve_req_add_approve(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_ECC_REDIAL_APPROVE_REQ_T_APPROVE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_ecc_redial_approve_req_add_call_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_ECC_REDIAL_APPROVE_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_call_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CREATE_SIP_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_num_dc_config(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CREATE_SIP_REQ_T_NUM_DC_CONFIG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dc_type_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dc_type(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_ip_addr_type_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_ip_addr_type(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_udp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_udp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_sctp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_sctp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_udp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_udp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_sctp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_sctp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_addr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_addr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_addr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_addr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_dtls_setup(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_dtls_setup(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_tls_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_tls_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_tls_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_tls_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_fp_hash_func_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_fp_hash_func(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_fp_str_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_fp_str(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_fp_str_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_fp_str(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_max_message_size_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_remote_max_message_size(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_max_message_size_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_local_max_message_size(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_data_b_as_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_data_b_as(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_qos_hint(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_qos_hint_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_num_dcmap(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CREATE_SIP_REQ_T_NUM_DCMAP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_stream_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_priority_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_priority(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dc_config_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dc_config_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_ordered_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_ordered(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_max_retr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_max_retr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_max_time_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_max_time(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_label_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_label(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_subprotocol_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_sip_req_add_dcmap_subprotocol(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_call_dc_create_sip_cnf_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CREATE_SIP_CNF_T_CALL_ID, def_val);
}

static inline uint16_t mipc_call_dc_create_sip_cnf_get_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_CALL_DC_CREATE_SIP_CNF_T_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_call_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_MODIFY_SIP_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_num_dc_config(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_MODIFY_SIP_REQ_T_NUM_DC_CONFIG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dc_type_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dc_type(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_ip_addr_type_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_ip_addr_type(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_udp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_udp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_sctp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_sctp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_udp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_udp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_sctp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_sctp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_addr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_addr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_addr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_addr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_dtls_setup(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_dtls_setup(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_tls_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_tls_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_tls_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_tls_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_fp_hash_func_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_fp_hash_func(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_fp_str_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_fp_str(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_fp_str_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_fp_str(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_max_message_size_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_remote_max_message_size(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_max_message_size_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_local_max_message_size(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_data_b_as_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_data_b_as(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_qos_hint(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_qos_hint_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_num_dcmap(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_MODIFY_SIP_REQ_T_NUM_DCMAP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_stream_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_priority_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_priority(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dc_config_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dc_config_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_ordered_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_ordered(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_max_retr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_max_retr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_max_time_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_max_time(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_label_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_label(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_subprotocol_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_modify_sip_req_add_dcmap_subprotocol(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_call_dc_modify_sip_cnf_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_MODIFY_SIP_CNF_T_CALL_ID, def_val);
}

static inline uint16_t mipc_call_dc_modify_sip_cnf_get_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_CALL_DC_MODIFY_SIP_CNF_T_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_dc_close_sip_req_add_call_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CLOSE_SIP_REQ_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_close_sip_req_add_close_all_stream(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CLOSE_SIP_REQ_T_CLOSE_ALL_STREAM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_close_sip_req_add_num_dcmap_del(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CLOSE_SIP_REQ_T_NUM_DCMAP_DEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_close_sip_req_add_dc_config_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_close_sip_req_add_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_close_sip_req_add_dcmap_stream_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_call_dc_close_sip_cnf_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CLOSE_SIP_CNF_T_CALL_ID, def_val);
}

static inline uint16_t mipc_call_dc_close_sip_cnf_get_cause(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_CALL_DC_CLOSE_SIP_CNF_T_CAUSE, def_val);
}

static inline uint32_t mipc_call_status_ind_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_STATUS_IND_T_CALLID, def_val);
}

static inline mipc_call_direction_const_enum mipc_call_status_ind_get_direction(mipc_msg_t *msg_ptr, mipc_call_direction_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_direction_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_STATUS_IND_T_DIRECTION, def_val);
}

static inline mipc_call_mode_const_enum mipc_call_status_ind_get_mode(mipc_msg_t *msg_ptr, mipc_call_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_mode_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_STATUS_IND_T_MODE, def_val);
}

static inline uint32_t mipc_call_status_ind_get_ton(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_STATUS_IND_T_TON, def_val);
}

static inline char * mipc_call_status_ind_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_STATUS_IND_T_NUMBER, val_len_ptr);
}

static inline mipc_call_type_const_enum mipc_call_status_ind_get_type(mipc_msg_t *msg_ptr, mipc_call_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_STATUS_IND_T_TYPE, def_val);
}

static inline mipc_call_detail_info_struct4* mipc_call_status_ind_get_detail_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_call_detail_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_STATUS_IND_T_DETAIL_INFO, val_len_ptr);
}

static inline mipc_call_video_cap_struct4* mipc_call_status_ind_get_video_cap(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_call_video_cap_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_STATUS_IND_T_VIDEO_CAP, val_len_ptr);
}

static inline mipc_call_msg_type_const_enum mipc_call_status_ind_get_msg_type(mipc_msg_t *msg_ptr, mipc_call_msg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_msg_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_STATUS_IND_T_MSG_TYPE, def_val);
}

static inline mipc_call_disconnected_cause_const_enum mipc_call_status_ind_get_disc_cause(mipc_msg_t *msg_ptr, mipc_call_disconnected_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_disconnected_cause_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_STATUS_IND_T_DISC_CAUSE, def_val);
}

static inline char * mipc_call_status_ind_get_pau(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_STATUS_IND_T_PAU, val_len_ptr);
}

static inline mipc_call_event_const_enum mipc_call_event_ind_get_event(mipc_msg_t *msg_ptr, mipc_call_event_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_event_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_EVENT_IND_T_EVENT, def_val);
}

static inline mipc_call_reject_reason_const_enum mipc_call_event_ind_get_reject_reason(mipc_msg_t *msg_ptr, mipc_call_reject_reason_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_reject_reason_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_EVENT_IND_T_REJECT_REASON, def_val);
}

static inline uint32_t mipc_call_event_ind_get_srvcch(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_EVENT_IND_T_SRVCCH, def_val);
}

static inline char * mipc_call_event_ind_get_redirect_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_EVENT_IND_T_REDIRECT_NUMBER, val_len_ptr);
}

static inline mipc_call_audio_codec_const_enum mipc_call_event_ind_get_audio_codec(mipc_msg_t *msg_ptr, mipc_call_audio_codec_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_audio_codec_const_enum)mipc_msg_get_val_uint16(msg_ptr, MIPC_CALL_EVENT_IND_T_AUDIO_CODEC, def_val);
}

static inline mipc_boolean_const_enum mipc_call_event_ind_get_speech_on(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EVENT_IND_T_SPEECH_ON, def_val);
}

static inline mipc_speech_rat_const_enum mipc_call_event_ind_get_speech_rat(mipc_msg_t *msg_ptr, mipc_speech_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_speech_rat_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_EVENT_IND_T_SPEECH_RAT, def_val);
}

static inline mipc_boolean_const_enum mipc_call_event_ind_get_speech_irho_on(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EVENT_IND_T_SPEECH_IRHO_ON, def_val);
}

static inline mipc_call_rat_const_enum mipc_call_event_ind_get_rat(mipc_msg_t *msg_ptr, mipc_call_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_rat_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EVENT_IND_T_RAT, def_val);
}

static inline uint32_t mipc_call_event_ind_get_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_EVENT_IND_T_COUNT, def_val);
}

static inline uint32_t mipc_call_event_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_EVENT_IND_T_CALL_ID, def_val, index);
}

static inline uint32_t mipc_call_mode_ind_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_MODE_IND_T_CALLID, def_val);
}

static inline mipc_call_mode_const_enum mipc_call_mode_ind_get_mode(mipc_msg_t *msg_ptr, mipc_call_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_mode_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_MODE_IND_T_MODE, def_val);
}

static inline mipc_sdp_direction_const_enum mipc_call_mode_ind_get_sdp_camera_direction(mipc_msg_t *msg_ptr, mipc_sdp_direction_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sdp_direction_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_MODE_IND_T_SDP_CAMERA_DIRECTION, def_val);
}

static inline mipc_sdp_direction_const_enum mipc_call_mode_ind_get_sdp_audio_direction(mipc_msg_t *msg_ptr, mipc_sdp_direction_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sdp_direction_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_MODE_IND_T_SDP_AUDIO_DIRECTION, def_val);
}

static inline mipc_sdp_audio_codec_const_enum mipc_call_mode_ind_get_sdp_audio_codec(mipc_msg_t *msg_ptr, mipc_sdp_audio_codec_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sdp_audio_codec_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_MODE_IND_T_SDP_AUDIO_CODEC, def_val);
}

static inline uint32_t mipc_call_sip_ind_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SIP_IND_T_CALLID, def_val);
}

static inline mipc_sip_direction_const_enum mipc_call_sip_ind_get_direction(mipc_msg_t *msg_ptr, mipc_sip_direction_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sip_direction_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SIP_IND_T_DIRECTION, def_val);
}

static inline mipc_sip_msg_type_const_enum mipc_call_sip_ind_get_msg_type(mipc_msg_t *msg_ptr, mipc_sip_msg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sip_msg_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SIP_IND_T_MSG_TYPE, def_val);
}

static inline mipc_sip_method_const_enum mipc_call_sip_ind_get_method(mipc_msg_t *msg_ptr, mipc_sip_method_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sip_method_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SIP_IND_T_METHOD, def_val);
}

static inline uint32_t mipc_call_sip_ind_get_response_code(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SIP_IND_T_RESPONSE_CODE, def_val);
}

static inline char * mipc_call_sip_ind_get_reason_text(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_SIP_IND_T_REASON_TEXT, val_len_ptr);
}

static inline uint32_t mipc_call_conference_ind_get_conf_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_CONF_CALLID, def_val);
}

static inline uint32_t mipc_call_conference_ind_get_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_COUNT, def_val);
}

static inline mipc_call_direction_const_enum mipc_call_conference_ind_get_direction(mipc_msg_t *msg_ptr, mipc_call_direction_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_direction_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_DIRECTION, def_val, index);
}

static inline char * mipc_call_conference_ind_get_participant_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_NUMBER, index, val_len_ptr);
}

static inline char * mipc_call_conference_ind_get_participant_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_NAME, index, val_len_ptr);
}

static inline mipc_conf_participant_status_const_enum mipc_call_conference_ind_get_participant_status(mipc_msg_t *msg_ptr, mipc_conf_participant_status_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_conf_participant_status_const_enum)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_STATUS, def_val, index);
}

static inline char * mipc_call_conference_ind_get_participant_user_entity(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_USER_ENTITY, index, val_len_ptr);
}

static inline char * mipc_call_conference_ind_get_participant_endpoint_entity(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_ENDPOINT_ENTITY, index, val_len_ptr);
}

static inline uint32_t mipc_call_ims_event_package_ind_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_CALLID, def_val);
}

static inline mipc_ims_event_package_type_const_enum mipc_call_ims_event_package_ind_get_type(mipc_msg_t *msg_ptr, mipc_ims_event_package_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ims_event_package_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_TYPE, def_val);
}

static inline char * mipc_call_ims_event_package_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_DATA, val_len_ptr);
}

static inline mipc_call_ss_code1_const_enum mipc_call_ss_ind_get_code1(mipc_msg_t *msg_ptr, mipc_call_ss_code1_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_ss_code1_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SS_IND_T_CODE1, def_val);
}

static inline mipc_call_ss_code2_const_enum mipc_call_ss_ind_get_code2(mipc_msg_t *msg_ptr, mipc_call_ss_code2_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_ss_code2_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SS_IND_T_CODE2, def_val);
}

static inline uint32_t mipc_call_ss_ind_get_index(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SS_IND_T_INDEX, def_val);
}

static inline char * mipc_call_ss_ind_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_SS_IND_T_NUMBER, val_len_ptr);
}

static inline uint32_t mipc_call_ss_ind_get_toa(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SS_IND_T_TOA, def_val);
}

static inline char * mipc_call_ss_ind_get_subaddr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_SS_IND_T_SUBADDR, val_len_ptr);
}

static inline uint32_t mipc_call_ss_ind_get_satype(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_SS_IND_T_SATYPE, def_val);
}

static inline char * mipc_call_ss_ind_get_raw_string(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_SS_IND_T_RAW_STRING, val_len_ptr);
}

static inline mipc_call_ecbm_mode_const_enum mipc_call_ecbm_change_ind_get_mode(mipc_msg_t *msg_ptr, mipc_call_ecbm_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_ecbm_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_ECBM_CHANGE_IND_T_MODE, def_val);
}

static inline mipc_crss_type_const_enum mipc_call_crss_ind_get_crss_type(mipc_msg_t *msg_ptr, mipc_crss_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_crss_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CRSS_IND_T_CRSS_TYPE, def_val);
}

static inline char * mipc_call_crss_ind_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CRSS_IND_T_NUMBER, val_len_ptr);
}

static inline mipc_number_type_const_enum mipc_call_crss_ind_get_call_number_type(mipc_msg_t *msg_ptr, mipc_number_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_number_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CRSS_IND_T_CALL_NUMBER_TYPE, def_val);
}

static inline mipc_number_presentation_const_enum mipc_call_crss_ind_get_number_presentation(mipc_msg_t *msg_ptr, mipc_number_presentation_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_number_presentation_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CRSS_IND_T_NUMBER_PRESENTATION, def_val);
}

static inline char * mipc_call_crss_ind_get_sub_address(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CRSS_IND_T_SUB_ADDRESS, val_len_ptr);
}

static inline uint8_t mipc_call_crss_ind_get_sa_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CRSS_IND_T_SA_TYPE, def_val);
}

static inline char * mipc_call_crss_ind_get_alphaid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CRSS_IND_T_ALPHAID, val_len_ptr);
}

static inline char * mipc_call_crss_ind_get_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CRSS_IND_T_NAME, val_len_ptr);
}

static inline uint32_t mipc_call_ect_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_ECT_IND_T_CALL_ID, def_val);
}

static inline mipc_boolean_const_enum mipc_call_ect_ind_get_ect_result(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_ECT_IND_T_ECT_RESULT, def_val);
}

static inline uint32_t mipc_call_ect_ind_get_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_ECT_IND_T_CAUSE, def_val);
}

static inline mipc_boolean_const_enum mipc_call_cipher_ind_get_sim_cipher_ind(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CIPHER_IND_T_SIM_CIPHER_IND, def_val);
}

static inline mipc_boolean_const_enum mipc_call_cipher_ind_get_mm_connection(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CIPHER_IND_T_MM_CONNECTION, def_val);
}

static inline mipc_call_cipher_on_status_const_enum mipc_call_cipher_ind_get_cs_cipher_on(mipc_msg_t *msg_ptr, mipc_call_cipher_on_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_cipher_on_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CIPHER_IND_T_CS_CIPHER_ON, def_val);
}

static inline mipc_call_cipher_on_status_const_enum mipc_call_cipher_ind_get_ps_cipher_on(mipc_msg_t *msg_ptr, mipc_call_cipher_on_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_cipher_on_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CIPHER_IND_T_PS_CIPHER_ON, def_val);
}

static inline uint32_t mipc_call_rtt_audio_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_RTT_AUDIO_IND_T_CALL_ID, def_val);
}

static inline mipc_call_rtt_audio_type_const_enum mipc_call_rtt_audio_ind_get_type(mipc_msg_t *msg_ptr, mipc_call_rtt_audio_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_rtt_audio_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_RTT_AUDIO_IND_T_TYPE, def_val);
}

static inline uint32_t mipc_call_rtt_capability_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_RTT_CAPABILITY_IND_T_CALL_ID, def_val);
}

static inline uint8_t mipc_call_rtt_capability_ind_get_local_text_capability(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_RTT_CAPABILITY_IND_T_LOCAL_TEXT_CAPABILITY, def_val);
}

static inline uint8_t mipc_call_rtt_capability_ind_get_remote_text_capability(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_RTT_CAPABILITY_IND_T_REMOTE_TEXT_CAPABILITY, def_val);
}

static inline uint8_t mipc_call_rtt_capability_ind_get_local_text_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_RTT_CAPABILITY_IND_T_LOCAL_TEXT_STATUS, def_val);
}

static inline uint8_t mipc_call_rtt_capability_ind_get_real_remote_text_capability(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_RTT_CAPABILITY_IND_T_REAL_REMOTE_TEXT_CAPABILITY, def_val);
}

static inline uint32_t mipc_call_local_rtt_modify_result_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_LOCAL_RTT_MODIFY_RESULT_IND_T_CALL_ID, def_val);
}

static inline uint8_t mipc_call_local_rtt_modify_result_ind_get_result(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_LOCAL_RTT_MODIFY_RESULT_IND_T_RESULT, def_val);
}

static inline uint32_t mipc_call_peer_rtt_modify_result_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND_T_CALL_ID, def_val);
}

static inline mipc_call_local_rtt_modify_op_const_enum mipc_call_peer_rtt_modify_result_ind_get_op(mipc_msg_t *msg_ptr, mipc_call_local_rtt_modify_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_local_rtt_modify_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND_T_OP, def_val);
}

static inline uint32_t mipc_call_rtt_text_receive_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_CALL_ID, def_val);
}

static inline uint32_t mipc_call_rtt_text_receive_ind_get_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_LEN, def_val);
}

static inline char * mipc_call_rtt_text_receive_ind_get_text(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_TEXT, val_len_ptr);
}

static inline uint8_t mipc_call_rtt_text_receive_ind_get_m_bit(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_M_BIT, def_val);
}

static inline char * mipc_call_rcs_digits_line_ind_get_digits_line(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_RCS_DIGITS_LINE_IND_T_DIGITS_LINE, val_len_ptr);
}

static inline char * mipc_call_display_and_signals_info_ind_get_display(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_DISPLAY, val_len_ptr);
}

static inline uint32_t mipc_call_display_and_signals_info_ind_get_signal_type(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_SIGNAL_TYPE, def_val);
}

static inline uint32_t mipc_call_display_and_signals_info_ind_get_alert_pitch(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_ALERT_PITCH, def_val);
}

static inline uint32_t mipc_call_display_and_signals_info_ind_get_signal(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_SIGNAL, def_val);
}

static inline uint32_t mipc_call_extended_display_info_ind_get_display_tag(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_EXTENDED_DISPLAY_INFO_IND_T_DISPLAY_TAG, def_val);
}

static inline char * mipc_call_extended_display_info_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_EXTENDED_DISPLAY_INFO_IND_T_INFO, val_len_ptr);
}

static inline uint32_t mipc_call_line_control_info_ind_get_polarity_included(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_LINE_CONTROL_INFO_IND_T_POLARITY_INCLUDED, def_val);
}

static inline uint32_t mipc_call_line_control_info_ind_get_toggle_mode(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_LINE_CONTROL_INFO_IND_T_TOGGLE_MODE, def_val);
}

static inline uint32_t mipc_call_line_control_info_ind_get_reverse_polarity(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_LINE_CONTROL_INFO_IND_T_REVERSE_POLARITY, def_val);
}

static inline uint32_t mipc_call_line_control_info_ind_get_power_denial_time(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_LINE_CONTROL_INFO_IND_T_POWER_DENIAL_TIME, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_ext_bit_1(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_EXT_BIT_1, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_number_type(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER_TYPE, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_number_plan(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER_PLAN, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_ext_bit_2(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_EXT_BIT_2, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_pi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_PI, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_si(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_SI, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_ext_bit_3(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_EXT_BIT_3, def_val);
}

static inline uint32_t mipc_call_redirecting_number_info_ind_get_redirection_reason(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_REDIRECTION_REASON, def_val);
}

static inline char * mipc_call_redirecting_number_info_ind_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER, val_len_ptr);
}

static inline mipc_gwsd_event_const_enum mipc_call_gwsd_event_ind_get_event(mipc_msg_t *msg_ptr, mipc_gwsd_event_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_gwsd_event_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GWSD_EVENT_IND_T_EVENT, def_val);
}

static inline uint32_t mipc_call_gwsd_event_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GWSD_EVENT_IND_T_CALL_ID, def_val);
}

static inline uint32_t mipc_call_gwsd_event_ind_get_update_status(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GWSD_EVENT_IND_T_UPDATE_STATUS, def_val);
}

static inline uint32_t mipc_call_gwsd_event_ind_get_ton(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GWSD_EVENT_IND_T_TON, def_val);
}

static inline char * mipc_call_gwsd_event_ind_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_GWSD_EVENT_IND_T_NUMBER, val_len_ptr);
}

static inline uint32_t mipc_call_gwsd_event_ind_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_GWSD_EVENT_IND_T_RESULT, def_val);
}

static inline uint32_t mipc_call_econf_ind_get_conf_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_ECONF_IND_T_CONF_CALL_ID, def_val);
}

static inline uint32_t mipc_call_econf_ind_get_operation(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_ECONF_IND_T_OPERATION, def_val);
}

static inline char * mipc_call_econf_ind_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_ECONF_IND_T_NUMBER, val_len_ptr);
}

static inline uint32_t mipc_call_econf_ind_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_ECONF_IND_T_RESULT, def_val);
}

static inline uint32_t mipc_call_econf_ind_get_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_ECONF_IND_T_CAUSE, def_val);
}

static inline uint32_t mipc_call_econf_ind_get_joined_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_ECONF_IND_T_JOINED_CALL_ID, def_val);
}

static inline uint32_t mipc_call_ims_sip_header_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_IMS_SIP_HEADER_IND_T_CALL_ID, def_val);
}

static inline uint32_t mipc_call_ims_sip_header_ind_get_header_type(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_IMS_SIP_HEADER_IND_T_HEADER_TYPE, def_val);
}

static inline uint32_t mipc_call_ims_sip_header_ind_get_total_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_IMS_SIP_HEADER_IND_T_TOTAL_COUNT, def_val);
}

static inline uint32_t mipc_call_ims_sip_header_ind_get_index(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_IMS_SIP_HEADER_IND_T_INDEX, def_val);
}

static inline char * mipc_call_ims_sip_header_ind_get_value(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_IMS_SIP_HEADER_IND_T_VALUE, val_len_ptr);
}

static inline uint8_t mipc_call_ecc_redial_ind_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_ECC_REDIAL_IND_T_CALL_ID, def_val);
}

static inline mipc_emergency_call_s1_support_const_enum mipc_call_emergency_bearer_support_ind_get_s1_support(mipc_msg_t *msg_ptr, mipc_emergency_call_s1_support_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_emergency_call_s1_support_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_S1_SUPPORT, def_val);
}

static inline mipc_emergency_call_rat_const_enum mipc_call_emergency_bearer_support_ind_get_rat(mipc_msg_t *msg_ptr, mipc_emergency_call_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_emergency_call_rat_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_RAT, def_val);
}

static inline mipc_emergency_call_support_emc_const_enum mipc_call_emergency_bearer_support_ind_get_support_emc(mipc_msg_t *msg_ptr, mipc_emergency_call_support_emc_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_emergency_call_support_emc_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_SUPPORT_EMC, def_val);
}

static inline mipc_emergency_call_emb_iu_supp_const_enum mipc_call_emergency_bearer_support_ind_get_emb_iu_supp(mipc_msg_t *msg_ptr, mipc_emergency_call_emb_iu_supp_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_emergency_call_emb_iu_supp_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMB_IU_SUPP, def_val);
}

static inline mipc_emergency_call_ems_5g_supp_const_enum mipc_call_emergency_bearer_support_ind_get_ems_5g_supp(mipc_msg_t *msg_ptr, mipc_emergency_call_ems_5g_supp_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_emergency_call_ems_5g_supp_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMS_5G_SUPP, def_val);
}

static inline mipc_emergency_call_emf_5g_supp_const_enum mipc_call_emergency_bearer_support_ind_get_emf_5g_supp(mipc_msg_t *msg_ptr, mipc_emergency_call_emf_5g_supp_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_emergency_call_emf_5g_supp_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMF_5G_SUPP, def_val);
}

static inline uint32_t mipc_call_uis_info_ind_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_UIS_INFO_IND_T_CALLID, def_val);
}

static inline mipc_call_uis_info_type_const_enum mipc_call_uis_info_ind_get_type(mipc_msg_t *msg_ptr, mipc_call_uis_info_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_uis_info_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_UIS_INFO_IND_T_TYPE, def_val);
}

static inline char * mipc_call_uis_info_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_UIS_INFO_IND_T_DATA, val_len_ptr);
}

static inline mipc_call_uis_info_result_const_enum mipc_call_uis_info_ind_get_result(mipc_msg_t *msg_ptr, mipc_call_uis_info_result_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_uis_info_result_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_UIS_INFO_IND_T_RESULT, def_val);
}

static inline mipc_call_uis_info_cause_const_enum mipc_call_uis_info_ind_get_cause(mipc_msg_t *msg_ptr, mipc_call_uis_info_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_uis_info_cause_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_UIS_INFO_IND_T_CAUSE, def_val);
}

static inline uint32_t mipc_call_call_additional_info_ind_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_CALLID, def_val);
}

static inline mipc_call_additional_info_mode_const_enum mipc_call_call_additional_info_ind_get_mode(mipc_msg_t *msg_ptr, mipc_call_additional_info_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_additional_info_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_MODE, def_val);
}

static inline mipc_call_additional_info_type_const_enum mipc_call_call_additional_info_ind_get_type(mipc_msg_t *msg_ptr, mipc_call_additional_info_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_additional_info_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_TYPE, def_val);
}

static inline uint32_t mipc_call_call_additional_info_ind_get_total(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_TOTAL, def_val);
}

static inline uint32_t mipc_call_call_additional_info_ind_get_index(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_INDEX, def_val);
}

static inline uint32_t mipc_call_call_additional_info_ind_get_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_COUNT, def_val);
}

static inline char * mipc_call_call_additional_info_ind_get_additional_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_ADDITIONAL_INFO, val_len_ptr);
}

static inline char * mipc_call_mt_sip_invite_ind_get_from_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_MT_SIP_INVITE_IND_T_FROM_NUMBER, val_len_ptr);
}

static inline uint8_t mipc_call_mt_sip_invite_ind_get_total_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_MT_SIP_INVITE_IND_T_TOTAL_COUNT, def_val);
}

static inline uint8_t mipc_call_mt_sip_invite_ind_get_index(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_MT_SIP_INVITE_IND_T_INDEX, def_val);
}

static inline char * mipc_call_mt_sip_invite_ind_get_mt_sip_invite(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_MT_SIP_INVITE_IND_T_MT_SIP_INVITE, val_len_ptr);
}

static inline mipc_recv_dtmf_mode_const_enum mipc_call_recv_dtmf_ind_get_mode(mipc_msg_t *msg_ptr, mipc_recv_dtmf_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_recv_dtmf_mode_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_RECV_DTMF_IND_T_MODE, def_val);
}

static inline char * mipc_call_recv_dtmf_ind_get_digit(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_RECV_DTMF_IND_T_DIGIT, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_CALL_ID, def_val);
}

static inline char * mipc_call_dc_config_init_ind_get_if_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_IF_NAME, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_num_dc_config(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_NUM_DC_CONFIG, def_val);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_index_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_INDEX_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_index(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_INDEX, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_dc_type_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_TYPE_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_dc_type(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_TYPE, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_ip_addr_type_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_IP_ADDR_TYPE_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_ip_addr_type(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_IP_ADDR_TYPE, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_remote_udp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_UDP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_remote_udp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_UDP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_remote_sctp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_SCTP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_remote_sctp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_SCTP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_local_udp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_UDP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_local_udp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_UDP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_local_sctp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_SCTP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_local_sctp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_SCTP_PORT, def_val, index);
}

static inline void * mipc_call_dc_config_init_ind_get_remote_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_call_dc_config_init_ind_get_remote_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_ADDR, index, val_len_ptr);
}

static inline void * mipc_call_dc_config_init_ind_get_local_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_call_dc_config_init_ind_get_local_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_ADDR, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_remote_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_DTLS_SETUP_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_remote_dtls_setup(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_DTLS_SETUP, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_local_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_DTLS_SETUP_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_local_dtls_setup(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_DTLS_SETUP, def_val, index);
}

static inline char * mipc_call_dc_config_init_ind_get_remote_tls_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_TLS_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_remote_tls_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_TLS_ID, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_local_tls_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_TLS_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_local_tls_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_TLS_ID, index, val_len_ptr);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_fp_hash_func_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_FP_HASH_FUNC_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_fp_hash_func(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_FP_HASH_FUNC, def_val, index);
}

static inline char * mipc_call_dc_config_init_ind_get_remote_fp_str_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_FP_STR_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_remote_fp_str(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_FP_STR, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_local_fp_str_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_FP_STR_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_local_fp_str(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_FP_STR, index, val_len_ptr);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_remote_max_message_size_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_remote_max_message_size(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_MAX_MESSAGE_SIZE, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_local_max_message_size_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_local_max_message_size(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_MAX_MESSAGE_SIZE, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_data_b_as_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DATA_B_AS_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_data_b_as(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DATA_B_AS, def_val, index);
}

static inline char * mipc_call_dc_config_init_ind_get_qos_hint(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_QOS_HINT, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_qos_hint_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_QOS_HINT_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_num_dcmap(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_NUM_DCMAP, def_val);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_STREAM_ID_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_dcmap_stream_id(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_STREAM_ID, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_dcmap_priority_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_PRIORITY_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_init_ind_get_dcmap_priority(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_PRIORITY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_dc_config_index_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_CONFIG_INDEX_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_dc_config_index(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_CONFIG_INDEX, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_dcmap_ordered_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_ORDERED_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_init_ind_get_dcmap_ordered(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_ORDERED, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_dcmap_max_retr_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_RETR_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_dcmap_max_retr(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_RETR, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_dcmap_max_time_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_TIME_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_init_ind_get_dcmap_max_time(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_TIME, def_val, index);
}

static inline char * mipc_call_dc_config_init_ind_get_dcmap_label_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_LABEL_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_dcmap_label(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_LABEL, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_dcmap_subprotocol_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_SUBPROTOCOL_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_init_ind_get_dcmap_subprotocol(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_SUBPROTOCOL, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_CALL_ID, def_val);
}

static inline char * mipc_call_dc_config_update_ind_get_if_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IF_NAME, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_num_dc_config(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_NUM_DC_CONFIG, def_val);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_index_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_INDEX_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_index(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_INDEX, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_dc_type_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_TYPE_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_dc_type(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_TYPE, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_ip_addr_type_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IP_ADDR_TYPE_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_ip_addr_type(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IP_ADDR_TYPE, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_remote_udp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_UDP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_remote_udp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_UDP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_remote_sctp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_SCTP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_remote_sctp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_SCTP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_local_udp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_UDP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_local_udp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_UDP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_local_sctp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_SCTP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_local_sctp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_SCTP_PORT, def_val, index);
}

static inline void * mipc_call_dc_config_update_ind_get_remote_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_call_dc_config_update_ind_get_remote_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_ADDR, index, val_len_ptr);
}

static inline void * mipc_call_dc_config_update_ind_get_local_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_call_dc_config_update_ind_get_local_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_ADDR, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_remote_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_DTLS_SETUP_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_remote_dtls_setup(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_DTLS_SETUP, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_local_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_DTLS_SETUP_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_local_dtls_setup(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_DTLS_SETUP, def_val, index);
}

static inline char * mipc_call_dc_config_update_ind_get_remote_tls_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_TLS_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_remote_tls_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_TLS_ID, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_local_tls_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_TLS_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_local_tls_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_TLS_ID, index, val_len_ptr);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_fp_hash_func_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_FP_HASH_FUNC_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_fp_hash_func(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_FP_HASH_FUNC, def_val, index);
}

static inline char * mipc_call_dc_config_update_ind_get_remote_fp_str_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_FP_STR_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_remote_fp_str(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_FP_STR, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_local_fp_str_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_FP_STR_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_local_fp_str(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_FP_STR, index, val_len_ptr);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_remote_max_message_size_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_remote_max_message_size(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_MAX_MESSAGE_SIZE, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_local_max_message_size_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_local_max_message_size(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_MAX_MESSAGE_SIZE, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_data_b_as_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DATA_B_AS_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_data_b_as(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DATA_B_AS, def_val, index);
}

static inline char * mipc_call_dc_config_update_ind_get_qos_hint(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_QOS_HINT, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_qos_hint_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_QOS_HINT_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_num_dcmap(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_NUM_DCMAP, def_val);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_STREAM_ID_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_dcmap_stream_id(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_STREAM_ID, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_dcmap_priority_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_PRIORITY_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_config_update_ind_get_dcmap_priority(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_PRIORITY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_dc_config_index_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_CONFIG_INDEX_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_dc_config_index(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_CONFIG_INDEX, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_dcmap_ordered_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_ORDERED_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_config_update_ind_get_dcmap_ordered(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_ORDERED, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_dcmap_max_retr_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_RETR_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_dcmap_max_retr(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_RETR, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_dcmap_max_time_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_TIME_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_config_update_ind_get_dcmap_max_time(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_TIME, def_val, index);
}

static inline char * mipc_call_dc_config_update_ind_get_dcmap_label_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_LABEL_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_dcmap_label(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_LABEL, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_dcmap_subprotocol_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_SUBPROTOCOL_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_config_update_ind_get_dcmap_subprotocol(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_SUBPROTOCOL, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_config_deinit_ind_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CONFIG_DEINIT_IND_T_CALL_ID, def_val);
}

static inline uint32_t mipc_call_connect_data_channel_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CONNECT_DATA_CHANNEL_IND_T_CALL_ID, def_val);
}

static inline char * mipc_call_connect_data_channel_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CONNECT_DATA_CHANNEL_IND_T_DATA, val_len_ptr);
}

static inline uint32_t mipc_call_close_data_channel_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CLOSE_DATA_CHANNEL_IND_T_CALL_ID, def_val);
}

static inline char * mipc_call_close_data_channel_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CLOSE_DATA_CHANNEL_IND_T_DATA, val_len_ptr);
}

static inline uint32_t mipc_call_create_data_channel_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_CREATE_DATA_CHANNEL_IND_T_CALL_ID, def_val);
}

static inline char * mipc_call_create_data_channel_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_CREATE_DATA_CHANNEL_IND_T_DATA, val_len_ptr);
}

static inline uint32_t mipc_call_approve_incoming_cmd_get_callid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_APPROVE_INCOMING_CMD_T_CALLID, def_val);
}

static inline char * mipc_call_approve_incoming_cmd_get_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_CALL_APPROVE_INCOMING_CMD_T_NUMBER, val_len_ptr);
}

static inline uint32_t mipc_call_approve_incoming_cmd_get_toa(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_APPROVE_INCOMING_CMD_T_TOA, def_val);
}

static inline uint32_t mipc_call_approve_incoming_cmd_get_seq_no(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_APPROVE_INCOMING_CMD_T_SEQ_NO, def_val);
}

static inline mipc_call_mode_const_enum mipc_call_approve_incoming_cmd_get_mode(mipc_msg_t *msg_ptr, mipc_call_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_call_mode_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_CALL_APPROVE_INCOMING_CMD_T_MODE, def_val);
}

static inline uint8_t mipc_call_approve_incoming_cmd_get_evoltesi_flow(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_APPROVE_INCOMING_CMD_T_EVOLTESI_FLOW, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_approve_incoming_rsp_add_is_approve(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_APPROVE_INCOMING_RSP_T_IS_APPROVE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_approve_incoming_rsp_add_cause(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_APPROVE_INCOMING_RSP_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_approve_incoming_rsp_add_callid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_APPROVE_INCOMING_RSP_T_CALLID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_approve_incoming_rsp_add_seq_no(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_CALL_APPROVE_INCOMING_RSP_T_SEQ_NO, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_call_dc_bootstrap_cfg_qry_cmd_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD_T_CALL_ID, def_val);
}

static inline uint16_t mipc_call_dc_bootstrap_cfg_qry_cmd_get_op_id(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD_T_OP_ID, def_val);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_call_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_result(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_num_dc_config_local(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DC_CONFIG_LOCAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_udp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_udp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_sctp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_sctp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_dtls_setup(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_tls_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_tls_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_fp_hash_func_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_fp_hash_func(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_fp_str_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_fp_str(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_max_message_size_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_local_max_message_size(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_data_b_as_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_data_b_as(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_qos_hint_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_qos_hint(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_num_dcmap(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DCMAP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_stream_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_priority_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_priority(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dc_config_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dc_config_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_ordered_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_ordered(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_max_retr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_max_retr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_max_time_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_max_time(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_label_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_label(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_subprotocol_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_bootstrap_cfg_qry_rsp_add_dcmap_subprotocol(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_call_dc_create_cmd_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_CALL_ID, def_val);
}

static inline uint8_t mipc_call_dc_create_cmd_get_num_dc_config(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_NUM_DC_CONFIG, def_val);
}

static inline uint8_t mipc_call_dc_create_cmd_get_index_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_INDEX_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_index(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_INDEX, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_dc_type_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DC_TYPE_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_dc_type(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DC_TYPE, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_ip_addr_type_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_IP_ADDR_TYPE_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_ip_addr_type(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_IP_ADDR_TYPE, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_remote_udp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_UDP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_remote_udp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_UDP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_remote_sctp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_SCTP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_remote_sctp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_SCTP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_local_udp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_UDP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_local_udp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_UDP_PORT, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_local_sctp_port_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_SCTP_PORT_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_local_sctp_port(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_SCTP_PORT, def_val, index);
}

static inline void * mipc_call_dc_create_cmd_get_remote_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_call_dc_create_cmd_get_remote_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_ADDR, index, val_len_ptr);
}

static inline void * mipc_call_dc_create_cmd_get_local_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_call_dc_create_cmd_get_local_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_ADDR, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_create_cmd_get_remote_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_DTLS_SETUP_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_remote_dtls_setup(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_DTLS_SETUP, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_local_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_DTLS_SETUP_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_local_dtls_setup(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_DTLS_SETUP, def_val, index);
}

static inline char * mipc_call_dc_create_cmd_get_remote_tls_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_TLS_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_remote_tls_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_TLS_ID, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_local_tls_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_TLS_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_local_tls_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_TLS_ID, index, val_len_ptr);
}

static inline uint16_t mipc_call_dc_create_cmd_get_fp_hash_func_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_FP_HASH_FUNC_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_fp_hash_func(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_FP_HASH_FUNC, def_val, index);
}

static inline char * mipc_call_dc_create_cmd_get_remote_fp_str_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_FP_STR_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_remote_fp_str(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_FP_STR, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_local_fp_str_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_FP_STR_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_local_fp_str(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_FP_STR, index, val_len_ptr);
}

static inline uint32_t mipc_call_dc_create_cmd_get_remote_max_message_size_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_remote_max_message_size(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_MAX_MESSAGE_SIZE, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_local_max_message_size_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_local_max_message_size(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_MAX_MESSAGE_SIZE, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_data_b_as_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DATA_B_AS_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_data_b_as(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DATA_B_AS, def_val, index);
}

static inline char * mipc_call_dc_create_cmd_get_qos_hint(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_QOS_HINT, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_qos_hint_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_QOS_HINT_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_call_dc_create_cmd_get_num_dcmap(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_NUM_DCMAP, def_val);
}

static inline uint16_t mipc_call_dc_create_cmd_get_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_STREAM_ID_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_dcmap_stream_id(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_STREAM_ID, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_dcmap_priority_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_PRIORITY_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_call_dc_create_cmd_get_dcmap_priority(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_PRIORITY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_dc_config_index_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DC_CONFIG_INDEX_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_dc_config_index(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DC_CONFIG_INDEX, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_dcmap_ordered_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_ORDERED_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_call_dc_create_cmd_get_dcmap_ordered(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_ORDERED, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_dcmap_max_retr_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_RETR_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_dcmap_max_retr(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_RETR, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_dcmap_max_time_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_TIME_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_call_dc_create_cmd_get_dcmap_max_time(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_TIME, def_val, index);
}

static inline char * mipc_call_dc_create_cmd_get_dcmap_label_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_LABEL_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_dcmap_label(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_LABEL, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_dcmap_subprotocol_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_SUBPROTOCOL_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_call_dc_create_cmd_get_dcmap_subprotocol(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_SUBPROTOCOL, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_call_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CREATE_RSP_T_CALL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_result(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CREATE_RSP_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_cause(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_CALL_DC_CREATE_RSP_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_num_dc_config(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CREATE_RSP_T_NUM_DC_CONFIG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dc_type_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dc_type(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_ip_addr_type_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_ip_addr_type(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_udp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_udp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_sctp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_sctp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_udp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_udp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_sctp_port_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_sctp_port(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_addr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_addr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_addr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_addr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_dtls_setup(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_dtls_setup_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_dtls_setup(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_tls_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_tls_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_tls_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_tls_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_fp_hash_func_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_fp_hash_func(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_fp_str_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_fp_str(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_fp_str_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_fp_str(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_max_message_size_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_remote_max_message_size(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_max_message_size_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_local_max_message_size(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_data_b_as_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_data_b_as(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_qos_hint(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_qos_hint_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_num_dcmap(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_CALL_DC_CREATE_RSP_T_NUM_DCMAP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_stream_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_stream_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_priority_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_priority(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dc_config_index_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dc_config_index(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_ordered_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_ordered(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_max_retr_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_max_retr(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_max_time_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_max_time(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_label_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_label(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_subprotocol_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_call_dc_create_rsp_add_dcmap_subprotocol(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}


#endif /* __MIPC_MSG_CALL_API_H__ */