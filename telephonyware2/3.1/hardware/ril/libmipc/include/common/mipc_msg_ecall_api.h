#ifndef __MIPC_MSG_ECALL_API_H__
#define __MIPC_MSG_ECALL_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_ecall_ivs_update_msd_req_add_msd_format(mipc_msg_t *msg_ptr, enum mipc_ecall_msd_format_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_ECALL_IVS_UPDATE_MSD_REQ_T_MSD_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_update_msd_req_add_msd_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_ECALL_IVS_UPDATE_MSD_REQ_T_MSD_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_test_addr_req_add_addr_type(mipc_msg_t *msg_ptr, enum mipc_call_dial_address_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDR_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_test_addr_req_add_address(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDRESS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_reconf_addr_req_add_addr_type(mipc_msg_t *msg_ptr, enum mipc_call_dial_address_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDR_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_reconf_addr_req_add_address(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDRESS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_addr_pri_req_add_first_pri(mipc_msg_t *msg_ptr, enum mipc_ecall_address_priority_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_FIRST_PRI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_addr_pri_req_add_second_pri(mipc_msg_t *msg_ptr, enum mipc_ecall_address_priority_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_SECOND_PRI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_addr_pri_req_add_third_pri(mipc_msg_t *msg_ptr, enum mipc_ecall_address_priority_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_THIRD_PRI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_addr_pri_req_add_fourth_pri(mipc_msg_t *msg_ptr, enum mipc_ecall_address_priority_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_FOURTH_PRI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ecall_sim_type_const_enum mipc_ecall_ivs_get_sim_info_cnf_get_sim_type(mipc_msg_t *msg_ptr, mipc_ecall_sim_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ecall_sim_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_SIM_TYPE, def_val);
}

static inline char * mipc_ecall_ivs_get_sim_info_cnf_get_test_ecall_uri(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_TEST_ECALL_URI, val_len_ptr);
}

static inline char * mipc_ecall_ivs_get_sim_info_cnf_get_test_ecall_num(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_TEST_ECALL_NUM, val_len_ptr);
}

static inline char * mipc_ecall_ivs_get_sim_info_cnf_get_reconf_ecall_uri(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_RECONF_ECALL_URI, val_len_ptr);
}

static inline char * mipc_ecall_ivs_get_sim_info_cnf_get_reconf_ecall_num(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_RECONF_ECALL_NUM, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer1(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer2(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer3(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER3, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer5(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER5, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer6(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER6, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer7(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER7, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer9(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER9, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_config_timer_req_add_timer10(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER10, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer1(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER1, def_val);
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer2(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER2, def_val);
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer3(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER3, def_val);
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer5(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER5, def_val);
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer6(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER6, def_val);
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer7(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER7, def_val);
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer9(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER9, def_val);
}

static inline uint16_t mipc_ecall_ivs_config_timer_cnf_get_timer10(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER10, def_val);
}

static inline mipc_msg_api_result_enum mipc_ecall_ivs_set_ecall_mode_req_add_ecall_mode(mipc_msg_t *msg_ptr, enum mipc_ecall_mode_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_ECALL_IVS_SET_ECALL_MODE_REQ_T_ECALL_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_ecall_mode_type_const_enum mipc_ecall_ivs_get_ecall_mode_cnf_get_ecall_mode(mipc_msg_t *msg_ptr, mipc_ecall_mode_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ecall_mode_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_ECALL_IVS_GET_ECALL_MODE_CNF_T_ECALL_MODE, def_val);
}

static inline mipc_ecall_t10_status_const_enum mipc_ecall_ivs_get_ecall_mode_cnf_get_timer10_status(mipc_msg_t *msg_ptr, mipc_ecall_t10_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ecall_t10_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_ECALL_IVS_GET_ECALL_MODE_CNF_T_TIMER10_STATUS, def_val);
}

static inline mipc_ecall_status_const_enum mipc_ecall_status_ind_get_state(mipc_msg_t *msg_ptr, mipc_ecall_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ecall_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_ECALL_STATUS_IND_T_STATE, def_val);
}

static inline uint8_t mipc_ecall_status_ind_get_call_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_ECALL_STATUS_IND_T_CALL_ID, def_val);
}


#endif /* __MIPC_MSG_ECALL_API_H__ */
