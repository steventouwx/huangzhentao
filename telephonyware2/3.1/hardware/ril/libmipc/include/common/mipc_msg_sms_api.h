#ifndef __MIPC_MSG_SMS_API_H__
#define __MIPC_MSG_SMS_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_set_sca(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_CFG_REQ_T_SET_SCA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_get_sca(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_GET_SCA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_get_sms_state(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_GET_SMS_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_get_store_status(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_GET_STORE_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_prefer_ack(mipc_msg_t *msg_ptr, enum mipc_sms_ack_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_PREFER_ACK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_prefer_storage(mipc_msg_t *msg_ptr, enum mipc_sms_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_PREFER_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_get_all_can_get(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_GET_ALL_CAN_GET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_set_host_mem_available(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_SET_HOST_MEM_AVAILABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_text_mode_param_action(mipc_msg_t *msg_ptr, enum mipc_sms_text_mode_param_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_TEXT_MODE_PARAM_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_text_mode_fo(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_TEXT_MODE_FO, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_text_mode_vp(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_TEXT_MODE_VP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_text_mode_pid(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_TEXT_MODE_PID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_text_mode_dcs(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_TEXT_MODE_DCS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_prefer_storage_c2k(mipc_msg_t *msg_ptr, enum mipc_sms_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_PREFER_STORAGE_C2K, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_w_r_d_storage(mipc_msg_t *msg_ptr, enum mipc_sms_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_W_R_D_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_req_add_save_setting(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_REQ_T_SAVE_SETTING, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sms_format_const_enum mipc_sms_cfg_cnf_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_FORMAT, def_val);
}

static inline char * mipc_sms_cfg_cnf_get_sca(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_CFG_CNF_T_SCA, val_len_ptr);
}

static inline mipc_sms_state_const_enum mipc_sms_cfg_cnf_get_sms_state(mipc_msg_t *msg_ptr, mipc_sms_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_SMS_STATE, def_val);
}

static inline uint16_t mipc_sms_cfg_cnf_get_max_message(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_CNF_T_MAX_MESSAGE, def_val);
}

static inline mipc_sms_ack_const_enum mipc_sms_cfg_cnf_get_prefer_ack(mipc_msg_t *msg_ptr, mipc_sms_ack_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_PREFER_ACK, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_cfg_cnf_get_prefer_storage(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_PREFER_STORAGE, def_val);
}

static inline uint16_t mipc_sms_cfg_cnf_get_used_message(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_CNF_T_USED_MESSAGE, def_val);
}

static inline uint16_t mipc_sms_cfg_cnf_get_total_message(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_CNF_T_TOTAL_MESSAGE, def_val);
}

static inline uint8_t mipc_sms_cfg_cnf_get_text_mode_fo(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_TEXT_MODE_FO, def_val);
}

static inline uint8_t mipc_sms_cfg_cnf_get_text_mode_vp(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_TEXT_MODE_VP, def_val);
}

static inline uint8_t mipc_sms_cfg_cnf_get_text_mode_pid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_TEXT_MODE_PID, def_val);
}

static inline uint8_t mipc_sms_cfg_cnf_get_text_mode_dcs(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_TEXT_MODE_DCS, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_cfg_cnf_get_prefer_storage_c2k(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_PREFER_STORAGE_C2K, def_val);
}

static inline uint16_t mipc_sms_cfg_cnf_get_used_message_c2k(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_CNF_T_USED_MESSAGE_C2K, def_val);
}

static inline uint16_t mipc_sms_cfg_cnf_get_max_message_c2k(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_CNF_T_MAX_MESSAGE_C2K, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_cfg_cnf_get_w_r_d_storage(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_CNF_T_W_R_D_STORAGE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_send_req_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_SEND_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_send_req_add_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_SEND_REQ_T_PDU, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_send_req_add_pdu_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_SEND_REQ_T_PDU_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_send_req_add_save(mipc_msg_t *msg_ptr, enum mipc_sms_send_save_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_SEND_REQ_T_SAVE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_send_req_add_more_msg_to_send(mipc_msg_t *msg_ptr, enum mipc_sms_more_msg_to_send_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_SEND_REQ_T_MORE_MSG_TO_SEND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_send_req_add_pdu_c2k(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_SEND_REQ_T_PDU_C2K, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_send_req_add_num_c2k(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_SEND_REQ_T_NUM_C2K, len, (const void *)value);
}

static inline uint8_t mipc_sms_send_cnf_get_mr(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_SEND_CNF_T_MR, def_val);
}

static inline uint16_t mipc_sms_send_cnf_get_message_index(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_SEND_CNF_T_MESSAGE_INDEX, def_val);
}

static inline mipc_sms_c2k_err_class_const_enum mipc_sms_send_cnf_get_err_class_c2k(mipc_msg_t *msg_ptr, mipc_sms_c2k_err_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_c2k_err_class_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_SEND_CNF_T_ERR_CLASS_C2K, def_val);
}

static inline mipc_sms_format_const_enum mipc_sms_send_cnf_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_SEND_CNF_T_FORMAT, def_val);
}

static inline mipc_sms_c2k_err_code_const_enum mipc_sms_send_cnf_get_err_code_c2k(mipc_msg_t *msg_ptr, mipc_sms_c2k_err_code_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_c2k_err_code_const_enum)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_SEND_CNF_T_ERR_CODE_C2K, def_val);
}

static inline void * mipc_sms_send_cnf_get_ack_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_SEND_CNF_T_ACK_PDU, val_len_ptr);
}

static inline uint16_t mipc_sms_send_cnf_get_msgid_c2k(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_SEND_CNF_T_MSGID_C2K, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_read_req_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_req_add_flag(mipc_msg_t *msg_ptr, enum mipc_sms_flag_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_REQ_T_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_req_add_message_index(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SMS_READ_REQ_T_MESSAGE_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_req_add_status_unchange(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_REQ_T_STATUS_UNCHANGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sms_format_const_enum mipc_sms_read_cnf_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_READ_CNF_T_FORMAT, def_val);
}

static inline uint16_t mipc_sms_read_cnf_get_pdu_count(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_READ_CNF_T_PDU_COUNT, def_val);
}

static inline mipc_sms_pdu_struct4* mipc_sms_read_cnf_get_pdu_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sms_pdu_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_READ_CNF_T_PDU_LIST, val_len_ptr);
}

static inline void * mipc_sms_read_cnf_get_pdu_c2k(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_READ_CNF_T_PDU_C2K, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sms_delete_req_add_flag(mipc_msg_t *msg_ptr, enum mipc_sms_flag_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DELETE_REQ_T_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_delete_req_add_message_index(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SMS_DELETE_REQ_T_MESSAGE_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_delete_req_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DELETE_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_delete_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DELETE_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_get_store_status_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_GET_STORE_STATUS_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_get_store_status_req_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_GET_STORE_STATUS_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sms_store_flag_const_enum mipc_sms_get_store_status_cnf_get_flag(mipc_msg_t *msg_ptr, mipc_sms_store_flag_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_store_flag_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SMS_GET_STORE_STATUS_CNF_T_FLAG, def_val);
}

static inline uint16_t mipc_sms_get_store_status_cnf_get_message_index(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_GET_STORE_STATUS_CNF_T_MESSAGE_INDEX, def_val);
}

static inline uint16_t mipc_sms_get_store_status_cnf_get_max_message(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_GET_STORE_STATUS_CNF_T_MAX_MESSAGE, def_val);
}

static inline uint16_t mipc_sms_get_store_status_cnf_get_used_message(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_GET_STORE_STATUS_CNF_T_USED_MESSAGE, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_get_store_status_cnf_get_storage(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_GET_STORE_STATUS_CNF_T_STORAGE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_write_req_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_WRITE_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_write_req_add_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_WRITE_REQ_T_PDU, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_write_req_add_pdu_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_WRITE_REQ_T_PDU_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_write_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_WRITE_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_write_req_add_status(mipc_msg_t *msg_ptr, enum mipc_sms_status_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_WRITE_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_write_req_add_pdu_c2k(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_WRITE_REQ_T_PDU_C2K, len, (const void *)value);
}

static inline uint16_t mipc_sms_write_cnf_get_message_index(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_WRITE_CNF_T_MESSAGE_INDEX, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_open_cbm_type(mipc_msg_t *msg_ptr, enum mipc_sms_cbm_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SMS_CBM_CFG_REQ_T_OPEN_CBM_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_msg_id_cfg_type(mipc_msg_t *msg_ptr, enum mipc_sms_cbm_cfg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_CFG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_msg_id_range(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_msg_id_single(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_dcs_cfg_type(mipc_msg_t *msg_ptr, enum mipc_sms_cbm_cfg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_REQ_T_DCS_CFG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_dcs_range(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_dcs_single(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_etws_primary_cfg(mipc_msg_t *msg_ptr, enum mipc_sms_etws_primary_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_REQ_T_ETWS_PRIMARY_CFG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_c2k_cbm_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_REQ_T_C2K_CBM_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_req_add_pdu_3gpp_seg_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_REQ_T_PDU_3GPP_SEG_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sms_cbm_type_const_enum mipc_sms_cbm_cfg_cnf_get_open_cbm_type(mipc_msg_t *msg_ptr, mipc_sms_cbm_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_cbm_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_OPEN_CBM_TYPE, def_val);
}

static inline mipc_sms_cbm_cfg_type_const_enum mipc_sms_cbm_cfg_cnf_get_msg_id_cfg_type(mipc_msg_t *msg_ptr, mipc_sms_cbm_cfg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_cbm_cfg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_CFG_TYPE, def_val);
}

static inline uint32_t mipc_sms_cbm_cfg_cnf_get_msg_id_range(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_RANGE, def_val, index);
}

static inline uint16_t mipc_sms_cbm_cfg_cnf_get_msg_id_single(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_SINGLE, def_val, index);
}

static inline mipc_sms_cbm_cfg_type_const_enum mipc_sms_cbm_cfg_cnf_get_dcs_cfg_type(mipc_msg_t *msg_ptr, mipc_sms_cbm_cfg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_cbm_cfg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_DCS_CFG_TYPE, def_val);
}

static inline uint16_t mipc_sms_cbm_cfg_cnf_get_dcs_range(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_DCS_RANGE, def_val, index);
}

static inline uint8_t mipc_sms_cbm_cfg_cnf_get_dcs_single(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_DCS_SINGLE, def_val, index);
}

static inline uint8_t mipc_sms_cbm_cfg_cnf_get_language_single(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_LANGUAGE_SINGLE, def_val, index);
}

static inline mipc_sms_format_const_enum mipc_sms_cbm_cfg_cnf_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_FORMAT, def_val);
}

static inline mipc_boolean_const_enum mipc_sms_cbm_cfg_cnf_get_c2k_cbm_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_CNF_T_C2K_CBM_ENABLE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_scbm_req_add_quit_scbm_mode(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_SCBM_REQ_T_QUIT_SCBM_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_domain_req_add_cgsms_action(mipc_msg_t *msg_ptr, enum mipc_sms_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DOMAIN_REQ_T_CGSMS_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_domain_req_add_cgsms_value(mipc_msg_t *msg_ptr, enum mipc_sms_cgsms_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DOMAIN_REQ_T_CGSMS_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_domain_req_add_sms_ims_action(mipc_msg_t *msg_ptr, enum mipc_sms_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DOMAIN_REQ_T_SMS_IMS_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_domain_req_add_sms_ims_value(mipc_msg_t *msg_ptr, enum mipc_sms_ims_cfg_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DOMAIN_REQ_T_SMS_IMS_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sms_cgsms_const_enum mipc_sms_domain_cnf_get_cgsms_value(mipc_msg_t *msg_ptr, mipc_sms_cgsms_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_cgsms_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_DOMAIN_CNF_T_CGSMS_VALUE, def_val);
}

static inline mipc_sms_ims_cfg_const_enum mipc_sms_domain_cnf_get_sms_ims_value(mipc_msg_t *msg_ptr, mipc_sms_ims_cfg_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ims_cfg_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_DOMAIN_CNF_T_SMS_IMS_VALUE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_set_sca(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_SET_SCA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_save_set(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_SAVE_SET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_get_sca(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_GET_SCA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_mt_handle_mode(mipc_msg_t *msg_ptr, enum mipc_sms_handle_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_MT_HANDLE_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_save_storage(mipc_msg_t *msg_ptr, enum mipc_sms_md_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_SAVE_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_mt_ack_mode(mipc_msg_t *msg_ptr, enum mipc_sms_ack_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_MT_ACK_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_sr_ack_mode(mipc_msg_t *msg_ptr, enum mipc_sms_ack_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_SR_ACK_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_mt_ack_mode_3gpp2(mipc_msg_t *msg_ptr, enum mipc_sms_ack_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_MT_ACK_MODE_3GPP2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cfg_v2_req_add_sr_ack_mode_3gpp2(mipc_msg_t *msg_ptr, enum mipc_sms_ack_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CFG_V2_REQ_T_SR_ACK_MODE_3GPP2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline void * mipc_sms_cfg_v2_cnf_get_sca(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_CFG_V2_CNF_T_SCA, val_len_ptr);
}

static inline mipc_sms_handle_mode_const_enum mipc_sms_cfg_v2_cnf_get_mt_handle_mode(mipc_msg_t *msg_ptr, mipc_sms_handle_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_handle_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_CNF_T_MT_HANDLE_MODE, def_val);
}

static inline mipc_sms_md_storage_const_enum mipc_sms_cfg_v2_cnf_get_save_storage(mipc_msg_t *msg_ptr, mipc_sms_md_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_md_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_CNF_T_SAVE_STORAGE, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_cnf_get_mt_ack_mode(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_CNF_T_MT_ACK_MODE, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_cnf_get_sr_ack_mode(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_CNF_T_SR_ACK_MODE, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_cnf_get_mt_ack_mode_3gpp2(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_CNF_T_MT_ACK_MODE_3GPP2, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_cnf_get_sr_ack_mode_3gpp2(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_CNF_T_SR_ACK_MODE_3GPP2, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_send_v2_req_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_SEND_V2_REQ_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_send_v2_req_add_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_SEND_V2_REQ_T_PDU, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_send_v2_req_add_num_3gpp2(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_SEND_V2_REQ_T_NUM_3GPP2, len, (const void *)value);
}

static inline uint16_t mipc_sms_send_v2_cnf_get_sms_id(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_SEND_V2_CNF_T_SMS_ID, def_val);
}

static inline uint8_t mipc_sms_send_v2_cnf_get_error_class_3gpp2(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_SEND_V2_CNF_T_ERROR_CLASS_3GPP2, def_val);
}

static inline uint8_t mipc_sms_send_v2_cnf_get_cause_code_3gpp2(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_SEND_V2_CNF_T_CAUSE_CODE_3GPP2, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_read_v2_req_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_V2_REQ_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_v2_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_md_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_V2_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_v2_req_add_flag(mipc_msg_t *msg_ptr, enum mipc_sms_flag_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_V2_REQ_T_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_v2_req_add_msg_idx(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SMS_READ_V2_REQ_T_MSG_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_read_v2_req_add_status_unchange(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_READ_V2_REQ_T_STATUS_UNCHANGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_sms_read_v2_cnf_get_msg_idx(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SMS_READ_V2_CNF_T_MSG_IDX, def_val, index);
}

static inline void * mipc_sms_read_v2_cnf_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SMS_READ_V2_CNF_T_PDU, index, val_len_ptr);
}

static inline mipc_sms_status_const_enum mipc_sms_read_v2_cnf_get_status(mipc_msg_t *msg_ptr, mipc_sms_status_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_status_const_enum)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SMS_READ_V2_CNF_T_STATUS, def_val, index);
}

static inline mipc_msg_api_result_enum mipc_sms_delete_v2_req_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DELETE_V2_REQ_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_delete_v2_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_md_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DELETE_V2_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_delete_v2_req_add_flag(mipc_msg_t *msg_ptr, enum mipc_sms_flag_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_DELETE_V2_REQ_T_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_delete_v2_req_add_msg_idx(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SMS_DELETE_V2_REQ_T_MSG_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_get_store_status_v2_req_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_GET_STORE_STATUS_V2_REQ_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_get_store_status_v2_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_md_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_GET_STORE_STATUS_V2_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_sms_get_store_status_v2_cnf_get_total(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_GET_STORE_STATUS_V2_CNF_T_TOTAL, def_val);
}

static inline uint16_t mipc_sms_get_store_status_v2_cnf_get_used(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_GET_STORE_STATUS_V2_CNF_T_USED, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_write_v2_req_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_WRITE_V2_REQ_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_write_v2_req_add_storage(mipc_msg_t *msg_ptr, enum mipc_sms_md_storage_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_WRITE_V2_REQ_T_STORAGE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_write_v2_req_add_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_WRITE_V2_REQ_T_PDU, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_write_v2_req_add_status(mipc_msg_t *msg_ptr, enum mipc_sms_status_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_WRITE_V2_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_sms_write_v2_cnf_get_msg_idx(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_WRITE_V2_CNF_T_MSG_IDX, def_val);
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_etws_primary_cfg(mipc_msg_t *msg_ptr, enum mipc_sms_etws_primary_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_REQ_T_ETWS_PRIMARY_CFG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_REQ_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_ch_cfg_type(mipc_msg_t *msg_ptr, enum mipc_sms_cbm_cfg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_REQ_T_CH_CFG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_ch_range(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_ch_single(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_lan_cfg_type(mipc_msg_t *msg_ptr, enum mipc_sms_cbm_cfg_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_REQ_T_LAN_CFG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_lan_range(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_lan_single(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_cbm_cfg_v2_req_add_seg_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_REQ_T_SEG_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sms_standard_const_enum mipc_sms_cbm_cfg_v2_cnf_get_standard(mipc_msg_t *msg_ptr, mipc_sms_standard_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_standard_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_STANDARD, def_val);
}

static inline mipc_boolean_const_enum mipc_sms_cbm_cfg_v2_cnf_get_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_ENABLE, def_val);
}

static inline mipc_sms_cbm_cfg_type_const_enum mipc_sms_cbm_cfg_v2_cnf_get_ch_cfg_type(mipc_msg_t *msg_ptr, mipc_sms_cbm_cfg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_cbm_cfg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_CH_CFG_TYPE, def_val);
}

static inline uint32_t mipc_sms_cbm_cfg_v2_cnf_get_ch_range(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_CH_RANGE, def_val, index);
}

static inline uint16_t mipc_sms_cbm_cfg_v2_cnf_get_ch_single(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_CH_SINGLE, def_val, index);
}

static inline mipc_sms_cbm_cfg_type_const_enum mipc_sms_cbm_cfg_v2_cnf_get_lan_cfg_type(mipc_msg_t *msg_ptr, mipc_sms_cbm_cfg_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_cbm_cfg_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_LAN_CFG_TYPE, def_val);
}

static inline uint16_t mipc_sms_cbm_cfg_v2_cnf_get_lan_range(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_LAN_RANGE, def_val, index);
}

static inline uint8_t mipc_sms_cbm_cfg_v2_cnf_get_lan_single(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_LAN_SINGLE, def_val, index);
}

static inline uint8_t mipc_sms_cbm_cfg_v2_cnf_get_language_single(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SMS_CBM_CFG_V2_CNF_T_LANGUAGE_SINGLE, def_val, index);
}

static inline mipc_sms_format_const_enum mipc_sms_cfg_ind_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_IND_T_FORMAT, def_val);
}

static inline char * mipc_sms_cfg_ind_get_sca(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_CFG_IND_T_SCA, val_len_ptr);
}

static inline mipc_sms_state_const_enum mipc_sms_cfg_ind_get_sms_state(mipc_msg_t *msg_ptr, mipc_sms_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_IND_T_SMS_STATE, def_val);
}

static inline uint16_t mipc_sms_cfg_ind_get_max_message(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_IND_T_MAX_MESSAGE, def_val);
}

static inline mipc_sms_ack_const_enum mipc_sms_cfg_ind_get_prefer_ack(mipc_msg_t *msg_ptr, mipc_sms_ack_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_IND_T_PREFER_ACK, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_cfg_ind_get_prefer_storage(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_IND_T_PREFER_STORAGE, def_val);
}

static inline uint16_t mipc_sms_cfg_ind_get_used_message(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_IND_T_USED_MESSAGE, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_cfg_ind_get_prefer_storage_c2k(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_IND_T_PREFER_STORAGE_C2K, def_val);
}

static inline uint16_t mipc_sms_cfg_ind_get_used_message_c2k(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_IND_T_USED_MESSAGE_C2K, def_val);
}

static inline uint16_t mipc_sms_cfg_ind_get_max_message_c2k(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_CFG_IND_T_MAX_MESSAGE_C2K, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_cfg_ind_get_w_r_d_storage(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_IND_T_W_R_D_STORAGE, def_val);
}

static inline mipc_sms_format_const_enum mipc_sms_new_sms_ind_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_SMS_IND_T_FORMAT, def_val);
}

static inline uint16_t mipc_sms_new_sms_ind_get_pdu_count(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_SMS_IND_T_PDU_COUNT, def_val);
}

static inline mipc_sms_pdu_struct4* mipc_sms_new_sms_ind_get_pdu_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sms_pdu_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_SMS_IND_T_PDU_LIST, val_len_ptr);
}

static inline void * mipc_sms_new_sms_ind_get_pdu_c2k(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_SMS_IND_T_PDU_C2K, val_len_ptr);
}

static inline mipc_sms_store_flag_const_enum mipc_sms_store_status_ind_get_flag(mipc_msg_t *msg_ptr, mipc_sms_store_flag_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_store_flag_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SMS_STORE_STATUS_IND_T_FLAG, def_val);
}

static inline uint16_t mipc_sms_store_status_ind_get_message_index(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_STORE_STATUS_IND_T_MESSAGE_INDEX, def_val);
}

static inline mipc_sms_storage_const_enum mipc_sms_store_status_ind_get_storage(mipc_msg_t *msg_ptr, mipc_sms_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_STORE_STATUS_IND_T_STORAGE, def_val);
}

static inline void * mipc_sms_new_status_report_ind_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_IND_T_PDU, val_len_ptr);
}

static inline mipc_sms_cbm_type_const_enum mipc_sms_new_cbm_ind_get_cbm_type(mipc_msg_t *msg_ptr, mipc_sms_cbm_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_cbm_type_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_CBM_TYPE, def_val);
}

static inline uint16_t mipc_sms_new_cbm_ind_get_warning_type(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_WARNING_TYPE, def_val);
}

static inline uint16_t mipc_sms_new_cbm_ind_get_message_id(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_MESSAGE_ID, def_val);
}

static inline uint16_t mipc_sms_new_cbm_ind_get_serial_number(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_SERIAL_NUMBER, def_val);
}

static inline uint8_t mipc_sms_new_cbm_ind_get_dcs(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_DCS, def_val);
}

static inline void * mipc_sms_new_cbm_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_DATA, val_len_ptr);
}

static inline void * mipc_sms_new_cbm_ind_get_secur_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_SECUR_INFO, val_len_ptr);
}

static inline mipc_sms_format_const_enum mipc_sms_new_cbm_ind_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_FORMAT, def_val);
}

static inline void * mipc_sms_new_cbm_ind_get_pdu_c2k(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_PDU_C2K, val_len_ptr);
}

static inline void * mipc_sms_new_cbm_ind_get_pdu_3gpp_seg(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_PDU_3GPP_SEG, index, val_len_ptr);
}

static inline void * mipc_sms_new_cbm_ind_get_wac_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_IND_T_WAC_INFO, val_len_ptr);
}

static inline mipc_sms_scbm_status_const_enum mipc_sms_scbm_ind_get_status_update(mipc_msg_t *msg_ptr, mipc_sms_scbm_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_scbm_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_SCBM_IND_T_STATUS_UPDATE, def_val);
}

static inline char * mipc_sms_ext_info_ind_get_epsi(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_EXT_INFO_IND_T_EPSI, val_len_ptr);
}

static inline void * mipc_sms_ext_info_ind_get_esn_old(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_EXT_INFO_IND_T_ESN_OLD, val_len_ptr);
}

static inline void * mipc_sms_ext_info_ind_get_esn_new(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_EXT_INFO_IND_T_ESN_NEW, val_len_ptr);
}

static inline mipc_sms_format_const_enum mipc_sms_dup_new_sms_ind_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_DUP_NEW_SMS_IND_T_FORMAT, def_val);
}

static inline void * mipc_sms_dup_new_sms_ind_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_DUP_NEW_SMS_IND_T_PDU, val_len_ptr);
}

static inline mipc_sms_handle_mode_const_enum mipc_sms_cfg_v2_ind_get_mt_handle_mode(mipc_msg_t *msg_ptr, mipc_sms_handle_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_handle_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_IND_T_MT_HANDLE_MODE, def_val);
}

static inline mipc_sms_md_storage_const_enum mipc_sms_cfg_v2_ind_get_save_storage(mipc_msg_t *msg_ptr, mipc_sms_md_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_md_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_IND_T_SAVE_STORAGE, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_ind_get_mt_ack_mode(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_IND_T_MT_ACK_MODE, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_ind_get_sr_ack_mode(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_IND_T_SR_ACK_MODE, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_ind_get_mt_ack_mode_3gpp2(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_IND_T_MT_ACK_MODE_3GPP2, def_val);
}

static inline mipc_sms_ack_mode_const_enum mipc_sms_cfg_v2_ind_get_sr_ack_mode_3gpp2(mipc_msg_t *msg_ptr, mipc_sms_ack_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_ack_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_CFG_V2_IND_T_SR_ACK_MODE_3GPP2, def_val);
}

static inline mipc_sms_standard_const_enum mipc_sms_new_sms_v2_ind_get_standard(mipc_msg_t *msg_ptr, mipc_sms_standard_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_standard_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_IND_T_STANDARD, def_val);
}

static inline void * mipc_sms_new_sms_v2_ind_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_SMS_V2_IND_T_PDU, val_len_ptr);
}

static inline mipc_sms_md_storage_const_enum mipc_sms_new_sms_v2_ind_get_storage(mipc_msg_t *msg_ptr, mipc_sms_md_storage_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_md_storage_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_IND_T_STORAGE, def_val);
}

static inline uint16_t mipc_sms_new_sms_v2_ind_get_msg_idx(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_SMS_V2_IND_T_MSG_IDX, def_val);
}

static inline mipc_boolean_const_enum mipc_sms_store_status_v2_ind_get_sim_full(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_STORE_STATUS_V2_IND_T_SIM_FULL, def_val);
}

static inline mipc_boolean_const_enum mipc_sms_store_status_v2_ind_get_me_full(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_STORE_STATUS_V2_IND_T_ME_FULL, def_val);
}

static inline mipc_boolean_const_enum mipc_sms_store_status_v2_ind_get_uim_full(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_STORE_STATUS_V2_IND_T_UIM_FULL, def_val);
}

static inline mipc_sms_standard_const_enum mipc_sms_new_status_report_v2_ind_get_standard(mipc_msg_t *msg_ptr, mipc_sms_standard_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_standard_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_IND_T_STANDARD, def_val);
}

static inline void * mipc_sms_new_status_report_v2_ind_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_IND_T_PDU, val_len_ptr);
}

static inline mipc_sms_standard_const_enum mipc_sms_new_cbm_v2_ind_get_standard(mipc_msg_t *msg_ptr, mipc_sms_standard_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_standard_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_STANDARD, def_val);
}

static inline uint16_t mipc_sms_new_cbm_v2_ind_get_warning_type(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_WARNING_TYPE, def_val);
}

static inline uint16_t mipc_sms_new_cbm_v2_ind_get_ch(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_CH, def_val);
}

static inline uint8_t mipc_sms_new_cbm_v2_ind_get_lan(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_LAN, def_val);
}

static inline void * mipc_sms_new_cbm_v2_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_DATA, val_len_ptr);
}

static inline void * mipc_sms_new_cbm_v2_ind_get_secur_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_SECUR_INFO, val_len_ptr);
}

static inline uint16_t mipc_sms_new_cbm_v2_ind_get_sn(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_SN, def_val);
}

static inline void * mipc_sms_new_cbm_v2_ind_get_pdu_3gpp2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_PDU_3GPP2, val_len_ptr);
}

static inline void * mipc_sms_new_cbm_v2_ind_get_pdu_seg(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_PDU_SEG, index, val_len_ptr);
}

static inline void * mipc_sms_new_cbm_v2_ind_get_wac_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_CBM_V2_IND_T_WAC_INFO, val_len_ptr);
}

static inline mipc_sms_standard_const_enum mipc_sms_dup_new_sms_v2_ind_get_standard(mipc_msg_t *msg_ptr, mipc_sms_standard_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_standard_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_DUP_NEW_SMS_V2_IND_T_STANDARD, def_val);
}

static inline void * mipc_sms_dup_new_sms_v2_ind_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_DUP_NEW_SMS_V2_IND_T_PDU, val_len_ptr);
}

static inline mipc_sms_format_const_enum mipc_sms_new_sms_cmd_get_format(mipc_msg_t *msg_ptr, mipc_sms_format_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_format_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_SMS_CMD_T_FORMAT, def_val);
}

static inline uint16_t mipc_sms_new_sms_cmd_get_pdu_count(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SMS_NEW_SMS_CMD_T_PDU_COUNT, def_val);
}

static inline mipc_sms_pdu_struct4* mipc_sms_new_sms_cmd_get_pdu_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sms_pdu_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_SMS_CMD_T_PDU_LIST, val_len_ptr);
}

static inline void * mipc_sms_new_sms_cmd_get_pdu_c2k(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_SMS_CMD_T_PDU_C2K, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_rsp_add_new_sms_ack(mipc_msg_t *msg_ptr, enum mipc_new_sms_ack_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_rsp_add_cause(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_RSP_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_rsp_add_format(mipc_msg_t *msg_ptr, enum mipc_sms_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_RSP_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_rsp_add_err_class_c2k(mipc_msg_t *msg_ptr, enum mipc_sms_c2k_err_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_RSP_T_ERR_CLASS_C2K, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_rsp_add_err_code_c2k(mipc_msg_t *msg_ptr, enum mipc_sms_c2k_err_code_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SMS_NEW_SMS_RSP_T_ERR_CODE_C2K, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_rsp_add_ack_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_NEW_SMS_RSP_T_ACK_PDU, len, (const void *)value);
}

static inline void * mipc_sms_new_status_report_cmd_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_CMD_T_PDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_rsp_add_ack_type(mipc_msg_t *msg_ptr, enum mipc_new_sms_ack_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_rsp_add_cause(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_RSP_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_rsp_add_ack_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_PDU, len, (const void *)value);
}

static inline mipc_sms_standard_const_enum mipc_sms_new_sms_v2_cmd_get_standard(mipc_msg_t *msg_ptr, mipc_sms_standard_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_standard_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_CMD_T_STANDARD, def_val);
}

static inline void * mipc_sms_new_sms_v2_cmd_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_SMS_V2_CMD_T_PDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_v2_rsp_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_RSP_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_v2_rsp_add_ack(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_RSP_T_ACK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_v2_rsp_add_cause(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_RSP_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_v2_rsp_add_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_NEW_SMS_V2_RSP_T_PDU, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_v2_rsp_add_error_class_3gpp2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_RSP_T_ERROR_CLASS_3GPP2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_sms_v2_rsp_add_cause_code_3gpp2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_SMS_V2_RSP_T_CAUSE_CODE_3GPP2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sms_standard_const_enum mipc_sms_new_status_report_v2_cmd_get_standard(mipc_msg_t *msg_ptr, mipc_sms_standard_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sms_standard_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_CMD_T_STANDARD, def_val);
}

static inline void * mipc_sms_new_status_report_v2_cmd_get_pdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_CMD_T_PDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_v2_rsp_add_standard(mipc_msg_t *msg_ptr, enum mipc_sms_standard_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_STANDARD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_v2_rsp_add_ack(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_ACK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_v2_rsp_add_cause(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_v2_rsp_add_pdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_PDU, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_v2_rsp_add_error_class_3gpp2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_ERROR_CLASS_3GPP2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sms_new_status_report_v2_rsp_add_cause_code_3gpp2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_CAUSE_CODE_3GPP2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}


#endif /* __MIPC_MSG_SMS_API_H__ */
