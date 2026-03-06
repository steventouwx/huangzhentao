#ifndef __MIPC_MSG_INTERNAL_API_H__
#define __MIPC_MSG_INTERNAL_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_internal_open_req_add_version(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_OPEN_REQ_T_VERSION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_open_req_add_client_name(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_OPEN_REQ_T_CLIENT_NAME, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_internal_open_req_add_usir_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_OPEN_REQ_T_USIR_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_open_req_add_is_meta(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_OPEN_REQ_T_IS_META, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_internal_open_cnf_get_version(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_OPEN_CNF_T_VERSION, def_val);
}

static inline uint32_t mipc_internal_open_cnf_get_timeout(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_OPEN_CNF_T_TIMEOUT, def_val);
}

static inline uint32_t mipc_internal_test_cnf_get_test(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_TEST_CNF_T_TEST, def_val);
}

static inline mipc_msg_api_result_enum mipc_internal_register_ind_req_add_msg_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_INTERNAL_REGISTER_IND_REQ_T_MSG_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_register_ind_req_add_msg_id_group(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_register_ind_req_add_unreg_all(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_REGISTER_IND_REQ_T_UNREG_ALL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_unregister_ind_req_add_msg_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_INTERNAL_UNREGISTER_IND_REQ_T_MSG_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_unregister_ind_req_add_msg_id_group(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_unregister_ind_req_add_unreg_all(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_UNREGISTER_IND_REQ_T_UNREG_ALL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_register_cmd_req_add_msg_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_INTERNAL_REGISTER_CMD_REQ_T_MSG_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_unregister_cmd_req_add_msg_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_INTERNAL_UNREGISTER_CMD_REQ_T_MSG_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_filter_req_add_struct(mipc_msg_t *msg_ptr, uint16_t len, mipc_internal_set_filter_req_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_SET_FILTER_REQ_T_STRUCT, len, (const void *)value);
}

static inline mipc_internal_set_filter_cnf_struct4* mipc_internal_set_filter_cnf_get_struct(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_internal_set_filter_cnf_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_SET_FILTER_CNF_T_STRUCT, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_internal_reset_filter_req_add_struct(mipc_msg_t *msg_ptr, uint16_t len, mipc_internal_reset_filter_req_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_RESET_FILTER_REQ_T_STRUCT, len, (const void *)value);
}

static inline mipc_internal_reset_filter_cnf_struct4* mipc_internal_reset_filter_cnf_get_struct(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_internal_reset_filter_cnf_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_RESET_FILTER_CNF_T_STRUCT, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_internal_eif_req_add_transid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_EIF_REQ_T_TRANSID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_eif_req_add_cmd(mipc_msg_t *msg_ptr, enum mipc_internal_eif_req_cmd_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_EIF_REQ_T_CMD, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_eif_req_add_new_addr(mipc_msg_t *msg_ptr, uint16_t len, mipc_full_addr_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_EIF_REQ_T_NEW_ADDR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_internal_eif_req_add_old_addr(mipc_msg_t *msg_ptr, uint16_t len, mipc_full_addr_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_EIF_REQ_T_OLD_ADDR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_internal_eif_req_add_address_type(mipc_msg_t *msg_ptr, enum mipc_eif_address_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_EIF_REQ_T_ADDRESS_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_long_standby_monitor_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_long_standby_monitor_time_req_add_minute(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_REQ_T_MINUTE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_long_standby_monitor_time_req_add_second(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_TIME_REQ_T_SECOND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_long_standby_monitor_warning_ratio_req_add_m(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_M, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_long_standby_monitor_warning_ratio_req_add_x(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_X, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_long_standby_monitor_warning_ratio_req_add_y(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_Y, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_set_long_standby_monitor_warning_ratio_req_add_z(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_SET_LONG_STANDBY_MONITOR_WARNING_RATIO_REQ_T_Z, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_internal_eif_ind_get_transid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIF_IND_T_TRANSID, def_val);
}

static inline mipc_internal_eif_ind_cmd_const_enum mipc_internal_eif_ind_get_cmd(mipc_msg_t *msg_ptr, mipc_internal_eif_ind_cmd_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_internal_eif_ind_cmd_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIF_IND_T_CMD, def_val);
}

static inline uint32_t mipc_internal_eif_ind_get_cause(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIF_IND_T_CAUSE, def_val);
}

static inline uint32_t mipc_internal_eif_ind_get_mtu(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIF_IND_T_MTU, def_val);
}

static inline uint8_t mipc_internal_eif_ind_get_net_v4_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_COUNT, def_val);
}

static inline mipc_v4_full_addr_struct4* mipc_internal_eif_ind_get_net_v4_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_v4_full_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_LIST, val_len_ptr);
}

static inline uint8_t mipc_internal_eif_ind_get_net_v6_addr_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_COUNT, def_val);
}

static inline mipc_v6_full_addr_struct4* mipc_internal_eif_ind_get_net_v6_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_v6_full_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_LIST, val_len_ptr);
}

static inline mipc_apn_pdp_type_const_enum mipc_internal_eif_ind_get_pdp_type(mipc_msg_t *msg_ptr, mipc_apn_pdp_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_apn_pdp_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIF_IND_T_PDP_TYPE, def_val);
}

static inline mipc_v4_full_addr_struct4* mipc_internal_eif_ind_get_net_v4_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_v4_full_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_v6_full_addr_struct4* mipc_internal_eif_ind_get_net_v6_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_v6_full_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_internal_ho_ind_get_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_HO_IND_T_ID, def_val);
}

static inline mipc_internal_ho_progress_const_enum mipc_internal_ho_ind_get_progress(mipc_msg_t *msg_ptr, mipc_internal_ho_progress_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_internal_ho_progress_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_HO_IND_T_PROGRESS, def_val);
}

static inline mipc_ran_const_enum mipc_internal_ho_ind_get_src_ran(mipc_msg_t *msg_ptr, mipc_ran_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ran_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_HO_IND_T_SRC_RAN, def_val);
}

static inline mipc_ran_const_enum mipc_internal_ho_ind_get_dst_ran(mipc_msg_t *msg_ptr, mipc_ran_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ran_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_HO_IND_T_DST_RAN, def_val);
}

static inline uint8_t mipc_internal_ho_ind_get_is_succ(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_HO_IND_T_IS_SUCC, def_val);
}

static inline mipc_data_v4_addr_struct4* mipc_internal_ho_ind_get_v4_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v4_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_HO_IND_T_V4_ADDR, val_len_ptr);
}

static inline mipc_data_v6_addr_struct4* mipc_internal_ho_ind_get_v6_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_data_v6_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_HO_IND_T_V6_ADDR, val_len_ptr);
}

static inline uint32_t mipc_internal_ho_ind_get_trans_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_HO_IND_T_TRANS_ID, def_val);
}

static inline mipc_track_item_struct4* mipc_internal_mipc_pending_ind_get_track_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_track_item_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_MIPC_PENDING_IND_T_TRACK_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_track_item_struct4* mipc_internal_mipc_pending_ind_get_track_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_track_item_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_MIPC_PENDING_IND_T_TRACK_LIST, index, val_len_ptr);
}

static inline uint32_t mipc_internal_eipport_cmd_get_transid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_TRANSID, def_val);
}

static inline mipc_internal_eipport_action_const_enum mipc_internal_eipport_cmd_get_action(mipc_msg_t *msg_ptr, mipc_internal_eipport_action_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_internal_eipport_action_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_ACTION, def_val);
}

static inline uint8_t mipc_internal_eipport_cmd_get_ifid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_IFID, def_val);
}

static inline mipc_addr_struct4* mipc_internal_eipport_cmd_get_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_ADDR, val_len_ptr);
}

static inline uint8_t mipc_internal_eipport_cmd_get_proto(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_PROTO, def_val);
}

static inline uint16_t mipc_internal_eipport_cmd_get_port(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_PORT, def_val);
}

static inline mipc_msg_api_result_enum mipc_internal_eipport_rsp_add_transid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_EIPPORT_RSP_T_TRANSID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_eipport_rsp_add_action(mipc_msg_t *msg_ptr, enum mipc_internal_eipport_action_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_RSP_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_eipport_rsp_add_result(mipc_msg_t *msg_ptr, enum mipc_internal_eipport_result_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_RSP_T_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_internal_eipspi_cmd_get_transid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_TRANSID, def_val);
}

static inline mipc_internal_eipspi_action_const_enum mipc_internal_eipspi_cmd_get_action(mipc_msg_t *msg_ptr, mipc_internal_eipspi_action_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_internal_eipspi_action_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_ACTION, def_val);
}

static inline mipc_addr_struct4* mipc_internal_eipspi_cmd_get_src_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_SRC_ADDR, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_internal_eipspi_cmd_get_dst_addr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_DST_ADDR, val_len_ptr);
}

static inline mipc_ipsec_proto_enum_const_enum mipc_internal_eipspi_cmd_get_proto(mipc_msg_t *msg_ptr, mipc_ipsec_proto_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ipsec_proto_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_PROTO, def_val);
}

static inline mipc_ipsec_mode_enum_const_enum mipc_internal_eipspi_cmd_get_mode(mipc_msg_t *msg_ptr, mipc_ipsec_mode_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_ipsec_mode_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_MODE, def_val);
}

static inline uint32_t mipc_internal_eipspi_cmd_get_min_spi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_MIN_SPI, def_val);
}

static inline uint32_t mipc_internal_eipspi_cmd_get_max_spi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_MAX_SPI, def_val);
}

static inline uint32_t mipc_internal_eipspi_cmd_get_spi(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_SPI, def_val);
}

static inline mipc_msg_api_result_enum mipc_internal_eipspi_rsp_add_transid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_RSP_T_TRANSID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_eipspi_rsp_add_action(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_EIPSPI_RSP_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_eipspi_rsp_add_spi(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_RSP_T_SPI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_internal_multi_eipspi_free_cmd_get_transid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_TRANSID, def_val);
}

static inline uint32_t mipc_internal_multi_eipspi_free_cmd_get_multi_free_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_MULTI_FREE_COUNT, def_val);
}

static inline mipc_addr_struct4* mipc_internal_multi_eipspi_free_cmd_get_src_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SRC_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_internal_multi_eipspi_free_cmd_get_src_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SRC_ADDR_LIST, index, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_internal_multi_eipspi_free_cmd_get_dst_addr_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_DST_ADDR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_addr_struct4* mipc_internal_multi_eipspi_free_cmd_get_dst_addr_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_DST_ADDR_LIST, index, val_len_ptr);
}

static inline void * mipc_internal_multi_eipspi_free_cmd_get_proto_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_PROTO_LIST, val_len_ptr);
}

static inline void * mipc_internal_multi_eipspi_free_cmd_get_spi_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SPI_LIST, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_internal_multi_eipspi_free_rsp_add_transid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_TRANSID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_multi_eipspi_free_rsp_add_multi_free_count(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_MULTI_FREE_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_multi_eipspi_free_rsp_add_spi_list(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_SPI_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_internal_multi_eipspi_free_rsp_add_status_list(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_STATUS_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_internal_inject_tst_ntf_add_module(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_INJECT_TST_NTF_T_MODULE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_internal_inject_tst_ntf_add_index(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_INTERNAL_INJECT_TST_NTF_T_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_internal_inject_tst_ntf_add_inject_string(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_INTERNAL_INJECT_TST_NTF_T_INJECT_STRING, len, (const void *)value);
}


#endif /* __MIPC_MSG_INTERNAL_API_H__ */
