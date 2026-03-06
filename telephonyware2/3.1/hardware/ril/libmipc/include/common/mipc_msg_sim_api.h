#ifndef __MIPC_MSG_SIM_API_H__
#define __MIPC_MSG_SIM_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_sim_pin_protect_req_add_pin_type(mipc_msg_t *msg_ptr, enum mipc_sim_pin_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_PIN_PROTECT_REQ_T_PIN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_pin_protect_req_add_pin_op(mipc_msg_t *msg_ptr, enum mipc_sim_pin_protection_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_PIN_PROTECT_REQ_T_PIN_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_pin_protect_req_add_pin_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_PIN_PROTECT_REQ_T_PIN_CODE, len, (const void *)value);
}

static inline mipc_sim_pin_type_const_enum mipc_sim_pin_protect_cnf_get_pin_type(mipc_msg_t *msg_ptr, mipc_sim_pin_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_PIN_PROTECT_CNF_T_PIN_TYPE, def_val);
}

static inline mipc_sim_pin_state_const_enum mipc_sim_pin_protect_cnf_get_pin_state(mipc_msg_t *msg_ptr, mipc_sim_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_PIN_PROTECT_CNF_T_PIN_STATE, def_val);
}

static inline uint32_t mipc_sim_pin_protect_cnf_get_remaining_attempts(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_PIN_PROTECT_CNF_T_REMAINING_ATTEMPTS, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_change_pin_req_add_pin_type(mipc_msg_t *msg_ptr, enum mipc_sim_pin_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANGE_PIN_REQ_T_PIN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_change_pin_req_add_old_pin(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_CHANGE_PIN_REQ_T_OLD_PIN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_change_pin_req_add_new_pin(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_CHANGE_PIN_REQ_T_NEW_PIN, len, (const void *)value);
}

static inline mipc_sim_pin_type_const_enum mipc_sim_change_pin_cnf_get_pin_type(mipc_msg_t *msg_ptr, mipc_sim_pin_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CHANGE_PIN_CNF_T_PIN_TYPE, def_val);
}

static inline mipc_sim_pin_state_const_enum mipc_sim_change_pin_cnf_get_pin_state(mipc_msg_t *msg_ptr, mipc_sim_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CHANGE_PIN_CNF_T_PIN_STATE, def_val);
}

static inline uint32_t mipc_sim_change_pin_cnf_get_remaining_attempts(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_CHANGE_PIN_CNF_T_REMAINING_ATTEMPTS, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_verify_pin_req_add_pin_type(mipc_msg_t *msg_ptr, enum mipc_sim_pin_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_VERIFY_PIN_REQ_T_PIN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_verify_pin_req_add_pin_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_VERIFY_PIN_REQ_T_PIN_CODE, len, (const void *)value);
}

static inline mipc_sim_pin_type_const_enum mipc_sim_verify_pin_cnf_get_pin_type(mipc_msg_t *msg_ptr, mipc_sim_pin_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_VERIFY_PIN_CNF_T_PIN_TYPE, def_val);
}

static inline mipc_sim_pin_state_const_enum mipc_sim_verify_pin_cnf_get_pin_state(mipc_msg_t *msg_ptr, mipc_sim_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_VERIFY_PIN_CNF_T_PIN_STATE, def_val);
}

static inline uint32_t mipc_sim_verify_pin_cnf_get_remaining_attempts(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VERIFY_PIN_CNF_T_REMAINING_ATTEMPTS, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_unblock_pin_req_add_pin_type(mipc_msg_t *msg_ptr, enum mipc_sim_pin_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_unblock_pin_req_add_puk_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UNBLOCK_PIN_REQ_T_PUK_CODE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_unblock_pin_req_add_pin_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_CODE, len, (const void *)value);
}

static inline mipc_sim_pin_type_const_enum mipc_sim_unblock_pin_cnf_get_pin_type(mipc_msg_t *msg_ptr, mipc_sim_pin_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_UNBLOCK_PIN_CNF_T_PIN_TYPE, def_val);
}

static inline mipc_sim_pin_state_const_enum mipc_sim_unblock_pin_cnf_get_pin_state(mipc_msg_t *msg_ptr, mipc_sim_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_UNBLOCK_PIN_CNF_T_PIN_STATE, def_val);
}

static inline uint32_t mipc_sim_unblock_pin_cnf_get_remaining_attempts(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_UNBLOCK_PIN_CNF_T_REMAINING_ATTEMPTS, def_val);
}

static inline mipc_sim_pin_type_const_enum mipc_sim_get_pin_info_cnf_get_pin_type(mipc_msg_t *msg_ptr, mipc_sim_pin_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_PIN_INFO_CNF_T_PIN_TYPE, def_val);
}

static inline mipc_sim_pin_state_const_enum mipc_sim_get_pin_info_cnf_get_pin_state(mipc_msg_t *msg_ptr, mipc_sim_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_PIN_INFO_CNF_T_PIN_STATE, def_val);
}

static inline uint32_t mipc_sim_get_pin_info_cnf_get_remaining_attempts(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_GET_PIN_INFO_CNF_T_REMAINING_ATTEMPTS, def_val);
}

static inline mipc_sim_pin_desc_struct4* mipc_sim_get_pin_list_cnf_get_pin1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_pin_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_PIN_LIST_CNF_T_PIN1, val_len_ptr);
}

static inline mipc_sim_pin_desc_struct4* mipc_sim_get_pin_list_cnf_get_pin2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_pin_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_PIN_LIST_CNF_T_PIN2, val_len_ptr);
}

static inline mipc_sim_pin_desc_struct4* mipc_sim_get_pin_list_cnf_get_nw_pin(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_pin_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_PIN_LIST_CNF_T_NW_PIN, val_len_ptr);
}

static inline mipc_sim_pin_desc_struct4* mipc_sim_get_pin_list_cnf_get_sub_nw_pin(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_pin_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_PIN_LIST_CNF_T_SUB_NW_PIN, val_len_ptr);
}

static inline mipc_sim_pin_desc_struct4* mipc_sim_get_pin_list_cnf_get_sp_pin(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_pin_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_PIN_LIST_CNF_T_SP_PIN, val_len_ptr);
}

static inline mipc_sim_pin_desc_struct4* mipc_sim_get_pin_list_cnf_get_corp_pin(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_pin_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_PIN_LIST_CNF_T_CORP_PIN, val_len_ptr);
}

static inline mipc_sim_pin_desc_struct4* mipc_sim_get_pin_list_cnf_get_sim_pin(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_pin_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_PIN_LIST_CNF_T_SIM_PIN, val_len_ptr);
}

static inline mipc_sim_state_const_enum mipc_sim_state_cnf_get_state(mipc_msg_t *msg_ptr, mipc_sim_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATE_CNF_T_STATE, def_val);
}

static inline uint32_t mipc_sim_state_cnf_get_sim_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_STATE_CNF_T_SIM_ID, def_val);
}

static inline uint32_t mipc_sim_state_cnf_get_ps_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_STATE_CNF_T_PS_ID, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_status_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_STATUS_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sim_status_const_enum mipc_sim_status_cnf_get_status(mipc_msg_t *msg_ptr, mipc_sim_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_STATUS, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_sim_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_SIM_ID, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_ps_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_PS_ID, def_val);
}

static inline mipc_sim_card_present_state_const_enum mipc_sim_status_cnf_get_card_present_state(mipc_msg_t *msg_ptr, mipc_sim_card_present_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_card_present_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_CARD_PRESENT_STATE, def_val);
}

static inline mipc_pin_state_const_enum mipc_sim_status_cnf_get_upin_status(mipc_msg_t *msg_ptr, mipc_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_UPIN_STATUS, def_val);
}

static inline mipc_boolean_const_enum mipc_sim_status_cnf_get_test_sim(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_TEST_SIM, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_gsm_app_idx(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_GSM_APP_IDX, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_cdma_app_idx(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_CDMA_APP_IDX, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_isim_app_idx(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_ISIM_APP_IDX, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_app_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_APP_COUNT, def_val);
}

static inline mipc_app_status_desc_struct4* mipc_sim_status_cnf_get_app_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_app_status_desc_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_STATUS_CNF_T_APP_LIST, val_len_ptr);
}

static inline char * mipc_sim_status_cnf_get_eid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_STATUS_CNF_T_EID, val_len_ptr);
}

static inline char * mipc_sim_status_cnf_get_iccid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_STATUS_CNF_T_ICCID, val_len_ptr);
}

static inline char * mipc_sim_status_cnf_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_STATUS_CNF_T_ATR, val_len_ptr);
}

static inline mipc_boolean_const_enum mipc_sim_status_cnf_get_msisdn_ready(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_MSISDN_READY, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_pin1(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_PIN1, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_pin2(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_PIN2, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_puk1(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_PUK1, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_puk2(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_PUK2, def_val);
}

static inline mipc_app_status_desc_struct4* mipc_sim_status_cnf_get_app_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_app_status_desc_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_STATUS_CNF_T_APP_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_sim_status_cnf_get_phy_slot_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_PHY_SLOT_ID, def_val);
}

static inline uint8_t mipc_sim_status_cnf_get_port_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CNF_T_PORT_ID, def_val);
}

static inline void * mipc_sim_status_cnf_get_eid_byte(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_STATUS_CNF_T_EID_BYTE, val_len_ptr);
}

static inline char * mipc_sim_iccid_cnf_get_iccid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ICCID_CNF_T_ICCID, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_imsi_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_IMSI_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_imsi_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_sim_app_type_ex_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_IMSI_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_sim_imsi_cnf_get_imsi(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_IMSI_CNF_T_IMSI, val_len_ptr);
}

static inline uint8_t mipc_sim_imsi_cnf_get_mnc_len(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_IMSI_CNF_T_MNC_LEN, def_val);
}

static inline uint8_t mipc_sim_msisdn_cnf_get_msisdn_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_MSISDN_CNF_T_MSISDN_COUNT, def_val);
}

static inline mipc_sim_msisdn_struct4* mipc_sim_msisdn_cnf_get_msisdn_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_msisdn_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_MSISDN_CNF_T_MSISDN_LIST, val_len_ptr);
}

static inline mipc_sim_msisdn_struct4* mipc_sim_msisdn_cnf_get_msisdn_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_msisdn_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_MSISDN_CNF_T_MSISDN_TLV_ARRAY, index, val_len_ptr);
}

static inline uint32_t mipc_sim_get_atr_info_cnf_get_atr_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_GET_ATR_INFO_CNF_T_ATR_LEN, def_val);
}

static inline char * mipc_sim_get_atr_info_cnf_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_ATR_INFO_CNF_T_ATR, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_open_channel_req_add_app_id_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_OPEN_CHANNEL_REQ_T_APP_ID_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_open_channel_req_add_app_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_OPEN_CHANNEL_REQ_T_APP_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_open_channel_req_add_p2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_OPEN_CHANNEL_REQ_T_P2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_open_channel_req_add_channel_group(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_OPEN_CHANNEL_REQ_T_CHANNEL_GROUP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_sim_open_channel_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_OPEN_CHANNEL_CNF_T_SW, def_val);
}

static inline uint8_t mipc_sim_open_channel_cnf_get_channel(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_OPEN_CHANNEL_CNF_T_CHANNEL, def_val);
}

static inline void * mipc_sim_open_channel_cnf_get_resp(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_OPEN_CHANNEL_CNF_T_RESP, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_close_channel_req_add_channel_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CLOSE_CHANNEL_REQ_T_CHANNEL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_close_channel_req_add_channel_group(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CLOSE_CHANNEL_REQ_T_CHANNEL_GROUP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_sim_close_channel_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_CLOSE_CHANNEL_CNF_T_SW, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_sim_app_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_session_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_SESSION_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_cmd(mipc_msg_t *msg_ptr, enum mipc_sim_access_command_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_CMD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_file_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FILE_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_p1(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_p2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_p3(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_data_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_path(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_p3_ex(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_restricted_access_req_add_fcp_convert(mipc_msg_t *msg_ptr, enum mipc_sim_fcp_convert_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FCP_CONVERT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_sim_channel_restricted_access_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_channel_restricted_access_cnf_get_resp_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_LEN, def_val);
}

static inline void * mipc_sim_channel_restricted_access_cnf_get_resp_apdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_APDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_channel_generic_access_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_sim_app_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_generic_access_req_add_channel_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_CHANNEL_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_generic_access_req_add_apdu_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_APDU_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_channel_generic_access_req_add_apdu(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_APDU, len, (const void *)value);
}

static inline uint16_t mipc_sim_channel_generic_access_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_channel_generic_access_cnf_get_resp_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_LEN, def_val);
}

static inline void * mipc_sim_channel_generic_access_cnf_get_resp_apdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_APDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_version(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_VERSION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_app_id_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_APP_ID_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_app_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_APP_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_path_id(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_PATH_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_file_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_FILE_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_file_offset(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_FILE_OFFSET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_number_of_bytes(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_NUMBER_OF_BYTES, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_local_pin(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_LOCAL_PIN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_binary_data_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_BINARY_DATA_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_long_apdu_access_req_add_binary_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_REQ_T_BINARY_DATA, len, (const void *)value);
}

static inline uint8_t mipc_sim_long_apdu_access_cnf_get_version(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_CNF_T_VERSION, def_val);
}

static inline uint16_t mipc_sim_long_apdu_access_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_long_apdu_access_cnf_get_data_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_CNF_T_DATA_LEN, def_val);
}

static inline void * mipc_sim_long_apdu_access_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_LONG_APDU_ACCESS_CNF_T_DATA, val_len_ptr);
}

static inline uint8_t mipc_sim_app_list_cnf_get_version(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_APP_LIST_CNF_T_VERSION, def_val);
}

static inline uint8_t mipc_sim_app_list_cnf_get_app_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_APP_LIST_CNF_T_APP_COUNT, def_val);
}

static inline uint8_t mipc_sim_app_list_cnf_get_active_app_idx(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_APP_LIST_CNF_T_ACTIVE_APP_idx, def_val);
}

static inline mipc_sim_app_info_struct4* mipc_sim_app_list_cnf_get_app_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_app_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_APP_LIST_CNF_T_APP_LIST, val_len_ptr);
}

static inline mipc_sim_app_info_struct4* mipc_sim_app_list_cnf_get_app_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_app_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_APP_LIST_CNF_T_APP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_file_status_req_add_version(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_REQ_T_VERSION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_file_status_req_add_aid_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_REQ_T_AID_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_file_status_req_add_aid(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_FILE_STATUS_REQ_T_AID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_file_status_req_add_file_path_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_REQ_T_FILE_PATH_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_file_status_req_add_file_path(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_FILE_STATUS_REQ_T_FILE_PATH, len, (const void *)value);
}

static inline uint8_t mipc_sim_file_status_cnf_get_version(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_VERSION, def_val);
}

static inline uint16_t mipc_sim_file_status_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_SW, def_val);
}

static inline mipc_sim_file_accessibility_const_enum mipc_sim_file_status_cnf_get_file_accessibility(mipc_msg_t *msg_ptr, mipc_sim_file_accessibility_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_file_accessibility_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_FILE_ACCESSIBILITY, def_val);
}

static inline mipc_sim_file_type_const_enum mipc_sim_file_status_cnf_get_file_type(mipc_msg_t *msg_ptr, mipc_sim_file_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_file_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_FILE_TYPE, def_val);
}

static inline mipc_sim_file_structure_const_enum mipc_sim_file_status_cnf_get_file(mipc_msg_t *msg_ptr, mipc_sim_file_structure_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_file_structure_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_FILE, def_val);
}

static inline uint8_t mipc_sim_file_status_cnf_get_item_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_ITEM_COUNT, def_val);
}

static inline uint8_t mipc_sim_file_status_cnf_get_size(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_SIZE, def_val);
}

static inline void * mipc_sim_file_status_cnf_get_lock_status(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_LOCK_STATUS, val_len_ptr);
}

static inline uint16_t mipc_sim_file_status_cnf_get_size_ext(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_FILE_STATUS_CNF_T_SIZE_EXT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_get_reset_req_add_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_RESET_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sim_pass_through_mode_const_enum mipc_sim_get_reset_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_sim_pass_through_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pass_through_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_RESET_CNF_T_MODE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_set_reset_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sim_pass_through_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_RESET_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sim_get_terminal_capability_cnf_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF_T_COUNT, def_val);
}

static inline uint16_t mipc_sim_get_terminal_capability_cnf_get_tc_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF_T_TC_LEN, def_val);
}

static inline void * mipc_sim_get_terminal_capability_cnf_get_tc(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF_T_TC, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_set_terminal_capability_req_add_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ_T_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_terminal_capability_req_add_tc_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ_T_TC_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_terminal_capability_req_add_tc(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ_T_TC, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_set_pin_ex_req_add_pin_type(mipc_msg_t *msg_ptr, enum mipc_sim_pin_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_PIN_EX_REQ_T_PIN_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_pin_ex_req_add_op(mipc_msg_t *msg_ptr, enum mipc_sim_pin_operation_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_PIN_EX_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_pin_ex_req_add_pin_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SET_PIN_EX_REQ_T_PIN_CODE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_set_pin_ex_req_add_new_pin_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SET_PIN_EX_REQ_T_NEW_PIN_CODE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_set_pin_ex_req_add_aid_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_PIN_EX_REQ_T_AID_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_pin_ex_req_add_aid(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SET_PIN_EX_REQ_T_AID, len, (const void *)value);
}

static inline mipc_sim_pin_type_const_enum mipc_sim_set_pin_ex_cnf_get_pin_type(mipc_msg_t *msg_ptr, mipc_sim_pin_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SET_PIN_EX_CNF_T_PIN_TYPE, def_val);
}

static inline mipc_sim_pin_state_const_enum mipc_sim_set_pin_ex_cnf_get_pin_state(mipc_msg_t *msg_ptr, mipc_sim_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SET_PIN_EX_CNF_T_PIN_STATE, def_val);
}

static inline uint32_t mipc_sim_set_pin_ex_cnf_get_remaining_attempts(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SET_PIN_EX_CNF_T_REMAINING_ATTEMPTS, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_get_pin_ex_req_add_version(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_PIN_EX_REQ_T_VERSION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_pin_ex_req_add_aid_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_PIN_EX_REQ_T_AID_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_pin_ex_req_add_aid(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_PIN_EX_REQ_T_AID, len, (const void *)value);
}

static inline mipc_sim_pin_type_const_enum mipc_sim_get_pin_ex_cnf_get_pin_type(mipc_msg_t *msg_ptr, mipc_sim_pin_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_PIN_EX_CNF_T_PIN_TYPE, def_val);
}

static inline mipc_sim_pin_state_const_enum mipc_sim_get_pin_ex_cnf_get_pin_state(mipc_msg_t *msg_ptr, mipc_sim_pin_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_pin_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_PIN_EX_CNF_T_PIN_STATE, def_val);
}

static inline uint32_t mipc_sim_get_pin_ex_cnf_get_remaining_attempts(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_GET_PIN_EX_CNF_T_REMAINING_ATTEMPTS, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_get_gsm_auth_req_add_rand1(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_GSM_AUTH_REQ_T_RAND1, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_get_gsm_auth_req_add_rand2(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_GSM_AUTH_REQ_T_RAND2, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_get_gsm_auth_req_add_rand3(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_GSM_AUTH_REQ_T_RAND3, len, (const void *)value);
}

static inline uint16_t mipc_sim_get_gsm_auth_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_GET_GSM_AUTH_CNF_T_SW, def_val);
}

static inline void * mipc_sim_get_gsm_auth_cnf_get_sres1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_GSM_AUTH_CNF_T_SRES1, val_len_ptr);
}

static inline void * mipc_sim_get_gsm_auth_cnf_get_kc1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_GSM_AUTH_CNF_T_KC1, val_len_ptr);
}

static inline void * mipc_sim_get_gsm_auth_cnf_get_sres2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_GSM_AUTH_CNF_T_SRES2, val_len_ptr);
}

static inline void * mipc_sim_get_gsm_auth_cnf_get_kc2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_GSM_AUTH_CNF_T_KC2, val_len_ptr);
}

static inline void * mipc_sim_get_gsm_auth_cnf_get_sres3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_GSM_AUTH_CNF_T_SRES3, val_len_ptr);
}

static inline void * mipc_sim_get_gsm_auth_cnf_get_kc3(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_GSM_AUTH_CNF_T_KC3, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_get_ext_auth_req_add_ch(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_EXT_AUTH_REQ_T_CH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_ext_auth_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_EXT_AUTH_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_ext_auth_req_add_cmd_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_ext_auth_req_add_cmd_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_get_ext_auth_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_sim_app_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_EXT_AUTH_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_sim_get_ext_auth_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_GET_EXT_AUTH_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_get_ext_auth_cnf_get_rsp_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_LEN, def_val);
}

static inline void * mipc_sim_get_ext_auth_cnf_get_rsp_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_get_facility_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_sim_app_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_FACILITY_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_facility_req_add_facility(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_FACILITY_REQ_T_FACILITY, len, (const void *)value);
}

static inline uint8_t mipc_sim_get_facility_cnf_get_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_FACILITY_CNF_T_STATUS, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_set_facility_req_add_app_id(mipc_msg_t *msg_ptr, enum mipc_sim_app_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_FACILITY_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_facility_req_add_facility(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SET_FACILITY_REQ_T_FACILITY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_set_facility_req_add_pass_word(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SET_FACILITY_REQ_T_PASS_WORD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_set_facility_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_FACILITY_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sim_set_facility_cnf_get_retry_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SET_FACILITY_CNF_T_RETRY_COUNT, def_val);
}

static inline uint8_t mipc_sim_get_euicc_slots_status_cnf_get_slots_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_COUNT, def_val);
}

static inline mipc_sim_slots_info_struct4* mipc_sim_get_euicc_slots_status_cnf_get_slots_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_slots_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_LIST, val_len_ptr);
}

static inline mipc_sim_card_present_state_const_enum mipc_sim_get_euicc_slots_status_cnf_get_card_state(mipc_msg_t *msg_ptr, mipc_sim_card_present_state_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_card_present_state_const_enum)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_CARD_STATE, def_val, index);
}

static inline uint8_t mipc_sim_get_euicc_slots_status_cnf_get_slots_state(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_STATE, def_val, index);
}

static inline uint8_t mipc_sim_get_euicc_slots_status_cnf_get_logical_idx(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_LOGICAL_IDX, def_val, index);
}

static inline char * mipc_sim_get_euicc_slots_status_cnf_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ATR, index, val_len_ptr);
}

static inline char * mipc_sim_get_euicc_slots_status_cnf_get_eid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_EID, index, val_len_ptr);
}

static inline char * mipc_sim_get_euicc_slots_status_cnf_get_iccid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ICCID, index, val_len_ptr);
}

static inline mipc_sim_slots_info_struct4* mipc_sim_get_euicc_slots_status_cnf_get_slots_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_slots_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_eid_byte_struct_struct4* mipc_sim_get_euicc_slots_status_cnf_get_eid_byte_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_eid_byte_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_EID_BYTE_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_sim_access_profile_connect_cnf_get_cur_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_CUR_TYPE, def_val);
}

static inline uint8_t mipc_sim_access_profile_connect_cnf_get_support_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_SUPPORT_TYPE, def_val);
}

static inline char * mipc_sim_access_profile_connect_cnf_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_ATR, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_access_profile_power_on_req_add_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_ACCESS_PROFILE_POWER_ON_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sim_access_profile_power_on_cnf_get_cur_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF_T_CUR_TYPE, def_val);
}

static inline char * mipc_sim_access_profile_power_on_cnf_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF_T_ATR, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_access_profile_reset_req_add_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_ACCESS_PROFILE_RESET_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sim_access_profile_reset_cnf_get_cur_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ACCESS_PROFILE_RESET_CNF_T_CUR_TYPE, def_val);
}

static inline char * mipc_sim_access_profile_reset_cnf_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ACCESS_PROFILE_RESET_CNF_T_ATR, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_access_profile_apdu_req_add_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_ACCESS_PROFILE_APDU_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_access_profile_apdu_req_add_apdu(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_ACCESS_PROFILE_APDU_REQ_T_APDU, len, (const void *)value);
}

static inline char * mipc_sim_access_profile_apdu_cnf_get_apdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ACCESS_PROFILE_APDU_CNF_T_APDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_set_sim_power_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_SIM_POWER_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_sim_power_req_add_sim_power(mipc_msg_t *msg_ptr, enum mipc_sim_power_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_physical_slots_mapping_req_add_slots_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ_T_SLOTS_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_set_physical_slots_mapping_req_add_slots_mapping_list(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ_T_SLOTS_MAPPING_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_session_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_SESSION_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_cla(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_CLA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_ins(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_INS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_p1(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_p2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_p3(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P3, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_data_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_DATA_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_extended_channel_generic_access_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_DATA, len, (const void *)value);
}

static inline uint16_t mipc_sim_extended_channel_generic_access_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_extended_channel_generic_access_cnf_get_resp_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_LEN, def_val);
}

static inline void * mipc_sim_extended_channel_generic_access_cnf_get_resp_apdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_APDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_app_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_APP_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_app_id_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_APP_ID_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_file_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_FILE_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_record_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_RECORD_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_data_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_DATA_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_path(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_PATH, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_record_req_add_pin2(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_PIN2, len, (const void *)value);
}

static inline uint16_t mipc_sim_uicc_file_access_record_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_uicc_file_access_record_cnf_get_resp_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF_T_RESP_LEN, def_val);
}

static inline void * mipc_sim_uicc_file_access_record_cnf_get_resp_apdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF_T_RESP_APDU, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_app_id(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_APP_ID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_app_id_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_APP_ID_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_file_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_FILE_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_offset(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_OFFSET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_data_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_DATA_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_path(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_PATH, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_uicc_file_access_binary_req_add_pin2(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_PIN2, len, (const void *)value);
}

static inline uint16_t mipc_sim_uicc_file_access_binary_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_uicc_file_access_binary_cnf_get_resp_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF_T_RESP_LEN, def_val);
}

static inline void * mipc_sim_uicc_file_access_binary_cnf_get_resp_apdu(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF_T_RESP_APDU, val_len_ptr);
}

static inline uint8_t mipc_sim_get_physical_slots_mapping_cnf_get_active_physical_slot_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_GET_PHYSICAL_SLOTS_MAPPING_CNF_T_ACTIVE_PHYSICAL_SLOT_ID, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_get_sim_auth_req_add_p2(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_SIM_AUTH_REQ_T_P2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_sim_auth_req_add_aid(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_SIM_AUTH_REQ_T_AID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_get_sim_auth_req_add_cmd_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SIM_GET_SIM_AUTH_REQ_T_CMD_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_sim_auth_req_add_cmd_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_GET_SIM_AUTH_REQ_T_CMD_DATA, len, (const void *)value);
}

static inline uint16_t mipc_sim_get_sim_auth_cnf_get_sw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_GET_SIM_AUTH_CNF_T_SW, def_val);
}

static inline uint16_t mipc_sim_get_sim_auth_cnf_get_rsp_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_GET_SIM_AUTH_CNF_T_RSP_LEN, def_val);
}

static inline void * mipc_sim_get_sim_auth_cnf_get_rsp_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_SIM_AUTH_CNF_T_RSP_DATA, val_len_ptr);
}

static inline mipc_sim_crrst_state_const_enum mipc_sim_sml_get_allowed_carriers_cnf_get_state(mipc_msg_t *msg_ptr, mipc_sim_crrst_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_crrst_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_STATE, def_val);
}

static inline uint8_t mipc_sim_sml_get_allowed_carriers_cnf_get_multi_sim_policy(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_MULTI_SIM_POLICY, def_val);
}

static inline mipc_boolean_const_enum mipc_sim_sml_get_allowed_carriers_cnf_get_allowed_carriers_prioritized(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_PRIORITIZED, def_val);
}

static inline uint8_t mipc_sim_sml_get_allowed_carriers_cnf_get_allowed_carriers_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_NUM, def_val);
}

static inline mipc_sim_carrier_struct_struct4* mipc_sim_sml_get_allowed_carriers_cnf_get_allowed_carriers(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_carrier_struct_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS, val_len_ptr);
}

static inline uint8_t mipc_sim_sml_get_allowed_carriers_cnf_get_excluded_carriers_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS_NUM, def_val);
}

static inline mipc_sim_carrier_struct_struct4* mipc_sim_sml_get_allowed_carriers_cnf_get_excluded_carriers(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_carrier_struct_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS, val_len_ptr);
}

static inline mipc_sim_carrier_struct_struct4* mipc_sim_sml_get_allowed_carriers_cnf_get_allowed_carriers_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_carrier_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sim_carrier_struct_struct4* mipc_sim_sml_get_allowed_carriers_cnf_get_excluded_carriers_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_carrier_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sim_carrier_restriction_status_const_enum mipc_sim_sml_get_allowed_carriers_cnf_get_carrier_restriction_status(mipc_msg_t *msg_ptr, mipc_sim_carrier_restriction_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_carrier_restriction_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_CARRIER_RESTRICTION_STATUS, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_multi_sim_policy(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_MULTI_SIM_POLICY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_allowed_carriers_prioritized(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_PRIORITIZED, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_allowed_carriers_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_allowed_carriers(mipc_msg_t *msg_ptr, uint16_t len, mipc_sim_carrier_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_excluded_carriers_num(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_excluded_carriers(mipc_msg_t *msg_ptr, uint16_t len, mipc_sim_carrier_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_allowed_carriers_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_sim_carrier_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_allowed_carriers_req_add_excluded_carriers_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_sim_carrier_struct_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_sim_sml_set_allowed_carriers_cnf_get_allowed_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_CNF_T_ALLOWED_COUNT, def_val);
}

static inline uint8_t mipc_sim_sml_set_allowed_carriers_cnf_get_excluded_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_SET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_COUNT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_enter_sim_depersonalization_req_add_category(mipc_msg_t *msg_ptr, enum mipc_sim_sml_category_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_CATEGORY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_enter_sim_depersonalization_req_add_pin_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_PIN_CODE, len, (const void *)value);
}

static inline mipc_sim_sml_category_const_enum mipc_sim_sml_enter_sim_depersonalization_cnf_get_category(mipc_msg_t *msg_ptr, mipc_sim_sml_category_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_sml_category_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_CATEGORY, def_val);
}

static inline uint8_t mipc_sim_sml_enter_sim_depersonalization_cnf_get_remain_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_REMAIN_COUNT, def_val);
}

static inline void * mipc_sim_sml_enter_sim_depersonalization_cnf_get_remain_count_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_REMAIN_COUNT_LIST, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_get_lock_req_add_category(mipc_msg_t *msg_ptr, enum mipc_sim_sml_category_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_REQ_T_CATEGORY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sim_sml_category_const_enum mipc_sim_sml_get_lock_cnf_get_category(mipc_msg_t *msg_ptr, mipc_sim_sml_category_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_sml_category_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_CATEGORY, def_val);
}

static inline uint8_t mipc_sim_sml_get_lock_cnf_get_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_STATE, def_val);
}

static inline uint8_t mipc_sim_sml_get_lock_cnf_get_retry_cnt(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_RETRY_CNT, def_val);
}

static inline uint8_t mipc_sim_sml_get_lock_cnf_get_autolock_cnt(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_AUTOLOCK_CNT, def_val);
}

static inline uint8_t mipc_sim_sml_get_lock_cnf_get_num_set(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_NUM_SET, def_val);
}

static inline uint8_t mipc_sim_sml_get_lock_cnf_get_total_set(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_TOTAL_SET, def_val);
}

static inline uint8_t mipc_sim_sml_get_lock_cnf_get_key_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_KEY_STATE, def_val);
}

static inline uint8_t mipc_sim_sml_get_lock_cnf_get_max_cnt(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_MAX_CNT, def_val);
}

static inline mipc_boolean_const_enum mipc_sim_sml_get_lock_cnf_get_rsu_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_LOCK_CNF_T_RSU_ENABLE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_lock_req_add_category(mipc_msg_t *msg_ptr, enum mipc_sim_sml_category_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_SET_LOCK_REQ_T_CATEGORY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_lock_req_add_op(mipc_msg_t *msg_ptr, enum mipc_sim_sml_operation_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_SML_SET_LOCK_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_lock_req_add_key(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_SET_LOCK_REQ_T_KEY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_lock_req_add_data_imsi(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_SET_LOCK_REQ_T_DATA_IMSI, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_lock_req_add_gid1(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_SET_LOCK_REQ_T_GID1, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_set_lock_req_add_gid2(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_SET_LOCK_REQ_T_GID2, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_enter_device_depersonalization_req_add_key(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ_T_KEY, len, (const void *)value);
}

static inline uint8_t mipc_sim_sml_enter_device_depersonalization_cnf_get_remain_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_CNF_T_REMAIN_COUNT, def_val);
}

static inline uint8_t mipc_sim_sml_get_dev_lock_cnf_get_lock_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_LOCK_STATE, def_val);
}

static inline uint8_t mipc_sim_sml_get_dev_lock_cnf_get_algo(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_ALGO, def_val);
}

static inline uint8_t mipc_sim_sml_get_dev_lock_cnf_get_max_cnt(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_MAX_CNT, def_val);
}

static inline uint8_t mipc_sim_sml_get_dev_lock_cnf_get_remain_cnt(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_REMAIN_CNT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_rsu_req_add_operator_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_SML_RSU_REQ_T_OPERATOR_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_rsu_req_add_request_id(mipc_msg_t *msg_ptr, enum mipc_sim_sml_rsu_operation_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_SML_RSU_REQ_T_REQUEST_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_rsu_req_add_request_type(mipc_msg_t *msg_ptr, enum mipc_sml_rsu_request_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_SML_RSU_REQ_T_REQUEST_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_rsu_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_RSU_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sim_sml_rsu_req_add_rsv1(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_SML_RSU_REQ_T_RSV1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_rsu_req_add_rsv2(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_SML_RSU_REQ_T_RSV2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_sml_rsu_req_add_rsv_string(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_SML_RSU_REQ_T_RSV_STRING, len, (const void *)value);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_operator_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_OPERATOR_ID, def_val);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_request_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_REQUEST_ID, def_val);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_request_type(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_REQUEST_TYPE, def_val);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_error_code(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_ERROR_CODE, def_val);
}

static inline char * mipc_sim_sml_rsu_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_DATA, val_len_ptr);
}

static inline void * mipc_sim_sml_rsu_cnf_get_time(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_TIME, val_len_ptr);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_version(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_VERSION, def_val);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_status(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_STATUS, def_val);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_rsv1(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_RSV1, def_val);
}

static inline uint32_t mipc_sim_sml_rsu_cnf_get_rsv2(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_RSV2, def_val);
}

static inline char * mipc_sim_sml_rsu_cnf_get_rsv_string(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SML_RSU_CNF_T_RSV_STRING, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_set_aka_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_VSIM_SET_AKA_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sim_vsim_set_aka_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_SET_AKA_CNF_T_RESULT, def_val);
}

static inline uint32_t mipc_sim_vsim_enable_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_ENABLE_CNF_T_RESULT, def_val);
}

static inline uint32_t mipc_sim_vsim_disable_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_DISABLE_CNF_T_RESULT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_plug_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_VSIM_PLUG_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_plug_req_add_sim_type(mipc_msg_t *msg_ptr, enum mipc_sim_vsim_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sim_vsim_plug_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_PLUG_CNF_T_RESULT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_set_timer_req_add_timer(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_VSIM_SET_TIMER_REQ_T_TIMER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sim_vsim_set_timer_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_SET_TIMER_CNF_T_RESULT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_reset_req_add_result(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_VSIM_RESET_REQ_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_reset_req_add_length(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_VSIM_RESET_REQ_T_LENGTH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_reset_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_VSIM_RESET_REQ_T_DATA, len, (const void *)value);
}

static inline uint32_t mipc_sim_vsim_reset_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_RESET_CNF_T_RESULT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_apdu_req_add_length(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SIM_VSIM_APDU_REQ_T_LENGTH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_apdu_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SIM_VSIM_APDU_REQ_T_DATA, len, (const void *)value);
}

static inline uint32_t mipc_sim_vsim_apdu_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_APDU_CNF_T_RESULT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_vsim_auth_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sim_vsim_auth_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_VSIM_AUTH_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sim_vsim_auth_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_VSIM_AUTH_CNF_T_RESULT, def_val);
}

static inline uint32_t mipc_sim_cdma_subscription_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_RESULT, def_val);
}

static inline char * mipc_sim_cdma_subscription_cnf_get_msisdn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_MSISDN, val_len_ptr);
}

static inline uint8_t mipc_sim_cdma_subscription_cnf_get_sid_nid_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_SID_NID_COUNT, def_val);
}

static inline mipc_sid_nid_list_struct4* mipc_sim_cdma_subscription_cnf_get_sid_nid_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sid_nid_list_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_SID_NID_LIST, val_len_ptr);
}

static inline char * mipc_sim_cdma_subscription_cnf_get_vmin(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_VMIN, val_len_ptr);
}

static inline char * mipc_sim_cdma_subscription_cnf_get_vprlid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_VPRLID, val_len_ptr);
}

static inline mipc_sid_nid_list_struct4* mipc_sim_cdma_subscription_cnf_get_sid_nid_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sid_nid_list_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_SID_NID_TLV_ARRAY, index, val_len_ptr);
}

static inline uint32_t mipc_sim_cdma_get_subscription_source_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF_T_RESULT, def_val);
}

static inline uint8_t mipc_sim_cdma_get_subscription_source_cnf_get_uim_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF_T_UIM_STATE, def_val);
}

static inline uint32_t mipc_sim_pin_count_query_cnf_get_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_PIN_COUNT_QUERY_CNF_T_RESULT, def_val);
}

static inline uint8_t mipc_sim_pin_count_query_cnf_get_pin1(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PIN1, def_val);
}

static inline uint8_t mipc_sim_pin_count_query_cnf_get_pin2(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PIN2, def_val);
}

static inline uint8_t mipc_sim_pin_count_query_cnf_get_puk1(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PUK1, def_val);
}

static inline uint8_t mipc_sim_pin_count_query_cnf_get_puk2(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PUK2, def_val);
}

static inline uint32_t mipc_sim_sml_get_network_lock_cnf_get_carrier_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_CARRIER_ID, def_val);
}

static inline uint32_t mipc_sim_sml_get_network_lock_cnf_get_supported_cats(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_SUPPORTED_CATS, def_val);
}

static inline uint32_t mipc_sim_sml_get_network_lock_cnf_get_version(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_VERSION, def_val);
}

static inline uint8_t mipc_sim_sml_get_network_lock_cnf_get_lock_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_LOCK_STATUS, def_val);
}

static inline mipc_boolean_const_enum mipc_sim_sml_get_network_lock_cnf_get_lock_fuse(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_LOCK_FUSE, def_val);
}

static inline mipc_boolean_const_enum mipc_sim_sml_get_network_lock_cnf_get_rsu_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_RSU_ENABLE, def_val);
}

static inline uint16_t mipc_sim_sml_get_network_lock_cnf_get_rule(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_RULE, def_val);
}

static inline uint16_t mipc_sim_sml_get_network_lock_cnf_get_subrule(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_SUBRULE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sim_esim_switch_set_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_ESIM_SWITCH_SET_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sim_esim_switch_get_cnf_get_esim_support(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_SUPPORT, def_val);
}

static inline uint8_t mipc_sim_esim_switch_get_cnf_get_esim_psim_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_PSIM_STATE, def_val);
}

static inline char * mipc_sim_esim_eid_query_cnf_get_eid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ESIM_EID_QUERY_CNF_T_EID, val_len_ptr);
}

static inline void * mipc_sim_esim_eid_query_cnf_get_eid_byte(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ESIM_EID_QUERY_CNF_T_EID_BYTE, val_len_ptr);
}

static inline uint8_t mipc_sim_mep_slots_info_get_cnf_get_phy_slot_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PHY_SLOT_COUNT, def_val);
}

static inline mipc_sim_card_present_state_const_enum mipc_sim_mep_slots_info_get_cnf_get_card_state(mipc_msg_t *msg_ptr, mipc_sim_card_present_state_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_card_present_state_const_enum)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_CARD_STATE, def_val, index);
}

static inline char * mipc_sim_mep_slots_info_get_cnf_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_ATR, index, val_len_ptr);
}

static inline char * mipc_sim_mep_slots_info_get_cnf_get_eid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_EID, index, val_len_ptr);
}

static inline uint8_t mipc_sim_mep_slots_info_get_cnf_get_port_num(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PORT_NUM, def_val, index);
}

static inline mipc_msg_api_result_enum mipc_sim_mep_slots_info_get_cnf_get_port_info(mipc_msg_t *msg_ptr, uint16_t type, uint16_t array_size1, uint16_t array_size2, mipc_port_info_struct4 *value)
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
            mipc_port_info_struct4 *val_ptr = NULL;
            val_ptr = (mipc_port_info_struct4 *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_MSG_GET_TLV_TYPE(tlv_ptr1), idx2, &val_len);
            memcpy((void *)(value + (idx1*array_size2+idx2)), (void *)val_ptr, sizeof(mipc_port_info_struct4));
        }
    }
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sim_mep_mode_const_enum mipc_sim_mep_slots_info_get_cnf_get_mep_mode(mipc_msg_t *msg_ptr, mipc_sim_mep_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_mep_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_MEP_MODE, def_val);
}

static inline mipc_eid_byte_struct_struct4* mipc_sim_mep_slots_info_get_cnf_get_eid_byte_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_eid_byte_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_EID_BYTE_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_mep_slots_mapping_set_req_add_sim_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ_T_SIM_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_mep_slots_mapping_set_req_add_phy_slot_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_mep_slots_mapping_set_req_add_port_id(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint8(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sim_get_psismsc_req_add_app_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_PSISMSC_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_sim_get_psismsc_cnf_get_psismsc_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_PSISMSC_CNF_T_PSISMSC_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_sim_get_smsp_cnf_get_smsp_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_SMSP_CNF_T_SMSP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_get_service_table_req_add_app_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_GET_SERVICE_TABLE_REQ_T_APP_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline void * mipc_sim_get_service_table_cnf_get_ust(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_SERVICE_TABLE_CNF_T_UST, val_len_ptr);
}

static inline void * mipc_sim_get_service_table_cnf_get_ist(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_SERVICE_TABLE_CNF_T_IST, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sim_query_gid_req_add_gid_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SIM_QUERY_GID_REQ_T_GID_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline void * mipc_sim_query_gid_cnf_get_gid1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_QUERY_GID_CNF_T_GID1, val_len_ptr);
}

static inline void * mipc_sim_query_gid_cnf_get_gid2(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_QUERY_GID_CNF_T_GID2, val_len_ptr);
}

static inline char * mipc_sim_get_impi_cnf_get_impi(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_IMPI_CNF_T_IMPI, val_len_ptr);
}

static inline char * mipc_sim_get_impu_cnf_get_impu_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_IMPU_CNF_T_IMPU_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_sim_get_domain_cnf_get_domain(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_GET_DOMAIN_CNF_T_DOMAIN, val_len_ptr);
}

static inline uint8_t mipc_sim_get_pcscf_cnf_get_type_tlv_array(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_GET_PCSCF_CNF_T_TYPE_TLV_ARRAY, def_val, index);
}

static inline void * mipc_sim_get_pcscf_cnf_get_pcscf_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_GET_PCSCF_CNF_T_PCSCF_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sim_state_const_enum mipc_sim_state_ind_get_state(mipc_msg_t *msg_ptr, mipc_sim_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATE_IND_T_STATE, def_val);
}

static inline uint32_t mipc_sim_state_ind_get_sim_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_STATE_IND_T_SIM_ID, def_val);
}

static inline uint32_t mipc_sim_state_ind_get_ps_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_STATE_IND_T_PS_ID, def_val);
}

static inline uint32_t mipc_sim_state_ind_get_is_present(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_STATE_IND_T_IS_PRESENT, def_val);
}

static inline mipc_sim_sub_state_const_enum mipc_sim_state_ind_get_sub_state(mipc_msg_t *msg_ptr, mipc_sim_sub_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_sub_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATE_IND_T_SUB_STATE, def_val);
}

static inline mipc_sim_status_const_enum mipc_sim_status_ind_get_status(mipc_msg_t *msg_ptr, mipc_sim_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_IND_T_STATUS, def_val);
}

static inline uint32_t mipc_sim_status_ind_get_sim_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_STATUS_IND_T_SIM_ID, def_val);
}

static inline uint32_t mipc_sim_status_ind_get_ps_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SIM_STATUS_IND_T_PS_ID, def_val);
}

static inline uint8_t mipc_sim_euicc_slots_status_ind_get_slots_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_COUNT, def_val);
}

static inline mipc_sim_slots_info_struct4* mipc_sim_euicc_slots_status_ind_get_slots_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_slots_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_LIST, val_len_ptr);
}

static inline mipc_sim_card_present_state_const_enum mipc_sim_euicc_slots_status_ind_get_card_state(mipc_msg_t *msg_ptr, mipc_sim_card_present_state_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_card_present_state_const_enum)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_CARD_STATE, def_val, index);
}

static inline uint8_t mipc_sim_euicc_slots_status_ind_get_slots_state(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_STATE, def_val, index);
}

static inline uint8_t mipc_sim_euicc_slots_status_ind_get_logical_idx(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_LOGICAL_IDX, def_val, index);
}

static inline char * mipc_sim_euicc_slots_status_ind_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_ATR, index, val_len_ptr);
}

static inline char * mipc_sim_euicc_slots_status_ind_get_eid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_EID, index, val_len_ptr);
}

static inline char * mipc_sim_euicc_slots_status_ind_get_iccid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_ICCID, index, val_len_ptr);
}

static inline mipc_sim_slots_info_struct4* mipc_sim_euicc_slots_status_ind_get_slots_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sim_slots_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_eid_byte_struct_struct4* mipc_sim_euicc_slots_status_ind_get_eid_byte_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_eid_byte_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_EID_BYTE_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_sim_iccid_ind_get_iccid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_ICCID_IND_T_ICCID, val_len_ptr);
}

static inline uint8_t mipc_sim_event_ind_get_is_sim_inserted(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_EVENT_IND_T_IS_SIM_INSERTED, def_val);
}

static inline mipc_sim_cause_const_enum mipc_sim_event_ind_get_cause(mipc_msg_t *msg_ptr, mipc_sim_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_EVENT_IND_T_CAUSE, def_val);
}

static inline mipc_sim_additional_cause_const_enum mipc_sim_event_ind_get_additional_cause(mipc_msg_t *msg_ptr, mipc_sim_additional_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_additional_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_EVENT_IND_T_ADDITIONAL_CAUSE, def_val);
}

static inline uint8_t mipc_sim_status_change_with_cause_ind_get_is_sim_inserted(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_IS_SIM_INSERTED, def_val);
}

static inline mipc_sim_cause_const_enum mipc_sim_status_change_with_cause_ind_get_cause(mipc_msg_t *msg_ptr, mipc_sim_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_CAUSE, def_val);
}

static inline mipc_sim_additional_cause_const_enum mipc_sim_status_change_with_cause_ind_get_additional_cause(mipc_msg_t *msg_ptr, mipc_sim_additional_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_additional_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_ADDITIONAL_CAUSE, def_val);
}

static inline uint8_t mipc_sim_csim_imsi_change_ind_get_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CSIM_IMSI_CHANGE_IND_T_STATUS, def_val);
}

static inline uint8_t mipc_sim_sml_status_ind_get_lock_rule(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_STATUS_IND_T_LOCK_RULE, def_val);
}

static inline uint16_t mipc_sim_sml_status_ind_get_lock_sub_rule(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_SML_STATUS_IND_T_LOCK_SUB_RULE, def_val);
}

static inline uint8_t mipc_sim_sml_status_ind_get_device_lock_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_STATUS_IND_T_DEVICE_LOCK_STATE, def_val);
}

static inline uint8_t mipc_sim_sml_status_ind_get_rule_policy(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_STATUS_IND_T_RULE_POLICY, def_val);
}

static inline uint8_t mipc_sim_sml_status_ind_get_sim_validity(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_STATUS_IND_T_SIM_VALIDITY, def_val);
}

static inline uint8_t mipc_sim_sml_status_ind_get_device_lock_remain_cnt(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_STATUS_IND_T_DEVICE_LOCK_REMAIN_CNT, def_val);
}

static inline uint8_t mipc_sim_sml_rsu_ind_get_operator_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SML_RSU_IND_T_OPERATOR_ID, def_val);
}

static inline uint16_t mipc_sim_sml_rsu_ind_get_event_id(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SIM_SML_RSU_IND_T_EVENT_ID, def_val);
}

static inline char * mipc_sim_sml_rsu_ind_get_event_string(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SML_RSU_IND_T_EVENT_STRING, val_len_ptr);
}

static inline char * mipc_sim_vsim_apdu_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_VSIM_APDU_IND_T_DATA, val_len_ptr);
}

static inline uint8_t mipc_sim_vsim_event_ind_get_event(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_VSIM_EVENT_IND_T_EVENT, def_val);
}

static inline char * mipc_sim_vsim_event_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_VSIM_EVENT_IND_T_DATA, val_len_ptr);
}

static inline mipc_sim_app_type_ex_const_enum mipc_sim_simapp_ind_get_app_id(mipc_msg_t *msg_ptr, mipc_sim_app_type_ex_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_app_type_ex_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SIMAPP_IND_T_APP_ID, def_val);
}

static inline uint8_t mipc_sim_simapp_ind_get_ch_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SIMAPP_IND_T_CH_ID, def_val);
}

static inline char * mipc_sim_simapp_ind_get_mcc(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMAPP_IND_T_MCC, val_len_ptr);
}

static inline char * mipc_sim_simapp_ind_get_mnc(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMAPP_IND_T_MNC, val_len_ptr);
}

static inline mipc_boolean_const_enum mipc_sim_test_sim_ind_get_test_sim(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_TEST_SIM_IND_T_TEST_SIM, def_val);
}

static inline uint8_t mipc_sim_ct3g_ind_get_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CT3G_IND_T_TYPE, def_val);
}

static inline uint8_t mipc_sim_card_type_ind_get_usim_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CARD_TYPE_IND_T_USIM_STATE, def_val);
}

static inline uint8_t mipc_sim_card_type_ind_get_csim_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CARD_TYPE_IND_T_CSIM_STATE, def_val);
}

static inline uint8_t mipc_sim_card_type_ind_get_isim_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_CARD_TYPE_IND_T_ISIM_STATE, def_val);
}

static inline mipc_sim_event_const_enum mipc_sim_simind_ind_get_event(mipc_msg_t *msg_ptr, mipc_sim_event_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_event_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SIMIND_IND_T_EVENT, def_val);
}

static inline uint8_t mipc_sim_simind_ind_get_app_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SIMIND_IND_T_APP_ID, def_val);
}

static inline char * mipc_sim_simind_ind_get_spn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMIND_IND_T_SPN, val_len_ptr);
}

static inline char * mipc_sim_simind_ind_get_imsi(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMIND_IND_T_IMSI, val_len_ptr);
}

static inline char * mipc_sim_simind_ind_get_gid1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMIND_IND_T_GID1, val_len_ptr);
}

static inline char * mipc_sim_simind_ind_get_pnn_full_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMIND_IND_T_PNN_FULL_NAME, val_len_ptr);
}

static inline char * mipc_sim_simind_ind_get_impi(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMIND_IND_T_IMPI, val_len_ptr);
}

static inline uint8_t mipc_sim_simind_ind_get_file_num(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_SIMIND_IND_T_FILE_NUM, def_val);
}

static inline mipc_file_list_struct4* mipc_sim_simind_ind_get_file_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_file_list_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SIM_SIMIND_IND_T_FILE_LIST, val_len_ptr);
}

static inline mipc_file_list_struct4* mipc_sim_simind_ind_get_file_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_file_list_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_SIMIND_IND_T_FILE_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_sim_eslotesim_state_ind_get_esim_support(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ESLOTESIM_STATE_IND_T_ESIM_SUPPORT, def_val);
}

static inline uint8_t mipc_sim_eslotesim_state_ind_get_esim_psim_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_ESLOTESIM_STATE_IND_T_ESIM_PSIM_STATE, def_val);
}

static inline uint8_t mipc_sim_mep_slots_info_ind_get_phy_slot_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_IND_T_PHY_SLOT_COUNT, def_val);
}

static inline mipc_sim_card_present_state_const_enum mipc_sim_mep_slots_info_ind_get_card_state(mipc_msg_t *msg_ptr, mipc_sim_card_present_state_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_card_present_state_const_enum)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_IND_T_CARD_STATE, def_val, index);
}

static inline char * mipc_sim_mep_slots_info_ind_get_atr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_IND_T_ATR, index, val_len_ptr);
}

static inline char * mipc_sim_mep_slots_info_ind_get_eid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_IND_T_EID, index, val_len_ptr);
}

static inline uint8_t mipc_sim_mep_slots_info_ind_get_port_num(mipc_msg_t *msg_ptr, uint8_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_IND_T_PORT_NUM, def_val, index);
}

static inline mipc_msg_api_result_enum mipc_sim_mep_slots_info_ind_get_port_info(mipc_msg_t *msg_ptr, uint16_t type, uint16_t array_size1, uint16_t array_size2, mipc_port_info_struct4 *value)
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
            mipc_port_info_struct4 *val_ptr = NULL;
            val_ptr = (mipc_port_info_struct4 *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_MSG_GET_TLV_TYPE(tlv_ptr1), idx2, &val_len);
            memcpy((void *)(value + (idx1*array_size2+idx2)), (void *)val_ptr, sizeof(mipc_port_info_struct4));
        }
    }
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sim_mep_mode_const_enum mipc_sim_mep_slots_info_ind_get_mep_mode(mipc_msg_t *msg_ptr, mipc_sim_mep_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sim_mep_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_IND_T_MEP_MODE, def_val);
}

static inline mipc_eid_byte_struct_struct4* mipc_sim_mep_slots_info_ind_get_eid_byte_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_eid_byte_struct_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SIM_MEP_SLOTS_INFO_IND_T_EID_BYTE_TLV_ARRAY, index, val_len_ptr);
}


#endif /* __MIPC_MSG_SIM_API_H__ */
