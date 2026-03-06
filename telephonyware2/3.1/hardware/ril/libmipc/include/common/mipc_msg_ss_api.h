#ifndef __MIPC_MSG_SS_API_H__
#define __MIPC_MSG_SS_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_ss_send_ussd_req_add_dcs(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SS_SEND_USSD_REQ_T_DCS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_send_ussd_req_add_payload_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SEND_USSD_REQ_T_PAYLOAD_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_send_ussd_req_add_payload(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SEND_USSD_REQ_T_PAYLOAD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_send_ussd_req_add_lang(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SEND_USSD_REQ_T_LANG, len, (const void *)value);
}

static inline mipc_ss_ussd_const_enum mipc_ss_send_ussd_cnf_get_ussd_response(mipc_msg_t *msg_ptr, mipc_ss_ussd_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_ussd_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_SEND_USSD_CNF_T_USSD_RESPONSE, def_val);
}

static inline mipc_ss_session_const_enum mipc_ss_send_ussd_cnf_get_ussd_session_state(mipc_msg_t *msg_ptr, mipc_ss_session_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_session_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_SEND_USSD_CNF_T_USSD_SESSION_STATE, def_val);
}

static inline uint32_t mipc_ss_send_ussd_cnf_get_dcs(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SS_SEND_USSD_CNF_T_DCS, def_val);
}

static inline uint8_t mipc_ss_send_ussd_cnf_get_payload_len(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_LEN, def_val);
}

static inline void * mipc_ss_send_ussd_cnf_get_payload(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SEND_USSD_CNF_T_PAYLOAD, val_len_ptr);
}

static inline uint16_t mipc_ss_send_ussd_cnf_get_payload_len_ex(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_LEN_EX, def_val);
}

static inline void * mipc_ss_send_ussd_cnf_get_payload_ex(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_EX, val_len_ptr);
}

static inline mipc_ss_ussd_const_enum mipc_ss_cancel_ussd_cnf_get_ussd_response(mipc_msg_t *msg_ptr, mipc_ss_ussd_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_ussd_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_CANCEL_USSD_CNF_T_USSD_RESPONSE, def_val);
}

static inline mipc_ss_session_const_enum mipc_ss_cancel_ussd_cnf_get_ussd_session_state(mipc_msg_t *msg_ptr, mipc_ss_session_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_session_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_CANCEL_USSD_CNF_T_USSD_SESSION_STATE, def_val);
}

static inline uint32_t mipc_ss_cancel_ussd_cnf_get_dcs(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SS_CANCEL_USSD_CNF_T_DCS, def_val);
}

static inline uint8_t mipc_ss_cancel_ussd_cnf_get_payload_len(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_CANCEL_USSD_CNF_T_PAYLOAD_LEN, def_val);
}

static inline void * mipc_ss_cancel_ussd_cnf_get_payload(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_CANCEL_USSD_CNF_T_PAYLOAD, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_set_clir_req_add_n_value(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CLIR_REQ_T_N_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_ss_set_clir_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SET_CLIR_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline uint8_t mipc_ss_get_clir_cnf_get_clir_n(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_GET_CLIR_CNF_T_CLIR_N, def_val);
}

static inline uint8_t mipc_ss_get_clir_cnf_get_clir_m(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_GET_CLIR_CNF_T_CLIR_M, def_val);
}

static inline char * mipc_ss_get_clir_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_GET_CLIR_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_waiting_req_add_cw_enable_disable(mipc_msg_t *msg_ptr, enum mipc_ss_call_waiting_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CALL_WAITING_REQ_T_CW_ENABLE_DISABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_waiting_req_add_service_class(mipc_msg_t *msg_ptr, enum mipc_ss_service_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SS_SET_CALL_WAITING_REQ_T_SERVICE_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_ss_set_call_waiting_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SET_CALL_WAITING_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_query_call_waiting_req_add_service_class(mipc_msg_t *msg_ptr, enum mipc_ss_service_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SS_QUERY_CALL_WAITING_REQ_T_SERVICE_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ss_active_status_const_enum mipc_ss_query_call_waiting_cnf_get_call_waiting_status(mipc_msg_t *msg_ptr, mipc_ss_active_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_active_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_QUERY_CALL_WAITING_CNF_T_CALL_WAITING_STATUS, def_val);
}

static inline mipc_ss_service_class_const_enum mipc_ss_query_call_waiting_cnf_get_service_class(mipc_msg_t *msg_ptr, mipc_ss_service_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_service_class_const_enum)mipc_msg_get_val_uint16(msg_ptr, MIPC_SS_QUERY_CALL_WAITING_CNF_T_SERVICE_CLASS, def_val);
}

static inline uint8_t mipc_ss_query_call_waiting_cnf_get_cw_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_COUNT, def_val);
}

static inline mipc_ss_call_waiting_struct4* mipc_ss_query_call_waiting_cnf_get_cw_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ss_call_waiting_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_LIST, val_len_ptr);
}

static inline char * mipc_ss_query_call_waiting_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_QUERY_CALL_WAITING_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_ss_call_waiting_struct4* mipc_ss_query_call_waiting_cnf_get_cw_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ss_call_waiting_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_ss_operation(mipc_msg_t *msg_ptr, enum mipc_ss_set_call_forward_operation_code_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_SS_OPERATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_call_forward_reason(mipc_msg_t *msg_ptr, enum mipc_ss_call_forward_reason_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_dial_number(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_DIAL_NUMBER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_service_class(mipc_msg_t *msg_ptr, enum mipc_ss_service_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_SERVICE_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_toa(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_TOA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_timer_seconds(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_TIMER_SECONDS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_time_slot_begin(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_TIME_SLOT_BEGIN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_forward_req_add_time_slot_end(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SET_CALL_FORWARD_REQ_T_TIME_SLOT_END, len, (const void *)value);
}

static inline char * mipc_ss_set_call_forward_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SET_CALL_FORWARD_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_query_call_forward_req_add_service_class(mipc_msg_t *msg_ptr, enum mipc_ss_service_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SS_QUERY_CALL_FORWARD_REQ_T_SERVICE_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_query_call_forward_req_add_call_forward_reason(mipc_msg_t *msg_ptr, enum mipc_ss_call_forward_reason_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_QUERY_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_ss_query_call_forward_cnf_get_call_forward_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_COUNT, def_val);
}

static inline mipc_ss_call_forward_struct4* mipc_ss_query_call_forward_cnf_get_call_forward_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ss_call_forward_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_ss_call_forward_struct4* mipc_ss_query_call_forward_cnf_get_call_forward_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ss_call_forward_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_LIST, index, val_len_ptr);
}

static inline char * mipc_ss_query_call_forward_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_QUERY_CALL_FORWARD_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_barring_req_add_lock(mipc_msg_t *msg_ptr, enum mipc_ss_call_barring_lock_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CALL_BARRING_REQ_T_LOCK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_barring_req_add_facility(mipc_msg_t *msg_ptr, enum mipc_ss_call_barring_fac_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CALL_BARRING_REQ_T_FACILITY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_barring_req_add_password(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SET_CALL_BARRING_REQ_T_PASSWORD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_barring_req_add_service_class(mipc_msg_t *msg_ptr, enum mipc_ss_service_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SS_SET_CALL_BARRING_REQ_T_SERVICE_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_barring_req_add_dial_number_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CALL_BARRING_REQ_T_DIAL_NUMBER_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_call_barring_req_add_dial_number_list(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline char * mipc_ss_set_call_barring_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SET_CALL_BARRING_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_query_call_barring_req_add_facility(mipc_msg_t *msg_ptr, enum mipc_ss_call_barring_fac_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_QUERY_CALL_BARRING_REQ_T_FACILITY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_query_call_barring_req_add_service_class(mipc_msg_t *msg_ptr, enum mipc_ss_service_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SS_QUERY_CALL_BARRING_REQ_T_SERVICE_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_ss_query_call_barring_cnf_get_call_barring_status(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SS_QUERY_CALL_BARRING_CNF_T_CALL_BARRING_STATUS, def_val);
}

static inline char * mipc_ss_query_call_barring_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_QUERY_CALL_BARRING_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline uint8_t mipc_ss_query_call_barring_cnf_get_dial_number_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_COUNT, def_val);
}

static inline mipc_ss_cb_dialnumber_struct4* mipc_ss_query_call_barring_cnf_get_dial_number_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ss_cb_dialnumber_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_ss_cb_dialnumber_struct4* mipc_ss_query_call_barring_cnf_get_dial_number_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_ss_cb_dialnumber_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_LIST, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_change_barring_password_req_add_facility(mipc_msg_t *msg_ptr, enum mipc_ss_call_barring_fac_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_FACILITY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_change_barring_password_req_add_old_pwd(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_OLD_PWD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_change_barring_password_req_add_new_pwd(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_NEW_PWD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_change_barring_password_req_add_new_pwd_confirm(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_NEW_PWD_CONFIRM, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_set_supp_svc_notification_req_add_status_i(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ_T_STATUS_I, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_set_supp_svc_notification_req_add_status_u(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ_T_STATUS_U, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_ss_query_clip_cnf_get_code_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_QUERY_CLIP_CNF_T_CODE_STATUS, def_val);
}

static inline uint8_t mipc_ss_query_clip_cnf_get_nw_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_QUERY_CLIP_CNF_T_NW_STATUS, def_val);
}

static inline char * mipc_ss_query_clip_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_QUERY_CLIP_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_set_clip_req_add_status(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_CLIP_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_ss_set_clip_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SET_CLIP_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_run_gba_req_add_naf_fqdn(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_RUN_GBA_REQ_T_NAF_FQDN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_run_gba_req_add_naf_secure_protocol_id(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_RUN_GBA_REQ_T_NAF_SECURE_PROTOCOL_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_run_gba_req_add_force_run(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_RUN_GBA_REQ_T_FORCE_RUN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ss_run_gba_req_add_net_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SS_RUN_GBA_REQ_T_NET_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_ss_run_gba_cnf_get_key(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_RUN_GBA_CNF_T_KEY, val_len_ptr);
}

static inline uint8_t mipc_ss_run_gba_cnf_get_key_len(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_RUN_GBA_CNF_T_KEY_LEN, def_val);
}

static inline char * mipc_ss_run_gba_cnf_get_bit_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_RUN_GBA_CNF_T_BIT_ID, val_len_ptr);
}

static inline char * mipc_ss_run_gba_cnf_get_key_lifetime(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_RUN_GBA_CNF_T_KEY_LIFETIME, val_len_ptr);
}

static inline uint8_t mipc_ss_get_colp_cnf_get_colp_n(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_GET_COLP_CNF_T_COLP_N, def_val);
}

static inline uint8_t mipc_ss_get_colp_cnf_get_colp_m(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_GET_COLP_CNF_T_COLP_M, def_val);
}

static inline char * mipc_ss_get_colp_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_GET_COLP_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_set_colp_req_add_n_value(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_COLP_REQ_T_N_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_ss_set_colp_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SET_COLP_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline uint8_t mipc_ss_get_colr_cnf_get_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_GET_COLR_CNF_T_STATUS, def_val);
}

static inline char * mipc_ss_get_colr_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_GET_COLR_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_ss_cnap_state_const_enum mipc_ss_send_cnap_cnf_get_cnap_n(mipc_msg_t *msg_ptr, mipc_ss_cnap_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_cnap_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_SEND_CNAP_CNF_T_CNAP_N, def_val);
}

static inline uint8_t mipc_ss_send_cnap_cnf_get_cnap_m(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_SEND_CNAP_CNF_T_CNAP_M, def_val);
}

static inline mipc_msg_api_result_enum mipc_ss_set_colr_req_add_n_value(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SS_SET_COLR_REQ_T_N_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_ss_set_colr_cnf_get_errmessage(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_SET_COLR_CNF_T_ERRMESSAGE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ss_setup_xcap_user_agent_req_add_str(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SETUP_XCAP_USER_AGENT_REQ_T_STR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_set_xcap_cfg_req_add_cfg_name(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SET_XCAP_CFG_REQ_T_CFG_NAME, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ss_set_xcap_cfg_req_add_value(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SS_SET_XCAP_CFG_REQ_T_VALUE, len, (const void *)value);
}

static inline mipc_ss_ussd_const_enum mipc_ss_ussd_ind_get_ussd_response(mipc_msg_t *msg_ptr, mipc_ss_ussd_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_ussd_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_USSD_IND_T_USSD_RESPONSE, def_val);
}

static inline mipc_ss_session_const_enum mipc_ss_ussd_ind_get_ussd_session_state(mipc_msg_t *msg_ptr, mipc_ss_session_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_session_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_USSD_IND_T_USSD_SESSION_STATE, def_val);
}

static inline uint32_t mipc_ss_ussd_ind_get_dcs(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SS_USSD_IND_T_DCS, def_val);
}

static inline uint8_t mipc_ss_ussd_ind_get_payload_len(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_USSD_IND_T_PAYLOAD_LEN, def_val);
}

static inline void * mipc_ss_ussd_ind_get_payload(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_USSD_IND_T_PAYLOAD, val_len_ptr);
}

static inline uint16_t mipc_ss_ussd_ind_get_payload_len_ex(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SS_USSD_IND_T_PAYLOAD_LEN_EX, def_val);
}

static inline void * mipc_ss_ussd_ind_get_payload_ex(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_USSD_IND_T_PAYLOAD_EX, val_len_ptr);
}

static inline uint32_t mipc_ss_ecmccss_ind_get_call_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SS_ECMCCSS_IND_T_CALL_ID, def_val);
}

static inline uint32_t mipc_ss_ecmccss_ind_get_service(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SS_ECMCCSS_IND_T_SERVICE, def_val);
}

static inline char * mipc_ss_ecmccss_ind_get_raw_string(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_ECMCCSS_IND_T_RAW_STRING, val_len_ptr);
}

static inline mipc_ss_ecfu_icon_status_const_enum mipc_ss_cfu_ind_get_status(mipc_msg_t *msg_ptr, mipc_ss_ecfu_icon_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ss_ecfu_icon_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_CFU_IND_T_STATUS, def_val);
}

static inline uint8_t mipc_ss_cfu_ind_get_line(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_CFU_IND_T_LINE, def_val);
}

static inline uint32_t mipc_ss_xcap_rcn_ind_get_code(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SS_XCAP_RCN_IND_T_CODE, def_val);
}

static inline uint8_t mipc_ss_xcap_rcn_ind_get_response(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_XCAP_RCN_IND_T_RESPONSE, def_val);
}

static inline uint8_t mipc_ss_ims_xui_ind_get_account_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_IMS_XUI_IND_T_ACCOUNT_ID, def_val);
}

static inline uint8_t mipc_ss_ims_xui_ind_get_broadcast_flag(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SS_IMS_XUI_IND_T_BROADCAST_FLAG, def_val);
}

static inline char * mipc_ss_ims_xui_ind_get_xui_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SS_IMS_XUI_IND_T_XUI_INFO, val_len_ptr);
}


#endif /* __MIPC_MSG_SS_API_H__ */
