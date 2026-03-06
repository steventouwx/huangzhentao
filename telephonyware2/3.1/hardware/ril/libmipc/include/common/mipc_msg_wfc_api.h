#ifndef __MIPC_MSG_WFC_API_H__
#define __MIPC_MSG_WFC_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_wfc_register_cell_signal_ind_req_add_signal_type(mipc_msg_t *msg_ptr, enum mipc_wfc_signal_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_SIGNAL_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_register_cell_signal_ind_req_add_enable_rpt(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_ENABLE_RPT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_register_cell_signal_ind_req_add_time(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_register_cell_signal_ind_req_add_threshold_in(mipc_msg_t *msg_ptr, int16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int16(msg_ptr, MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_THRESHOLD_IN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_register_cell_signal_ind_req_add_threshold_out(mipc_msg_t *msg_ptr, int16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int16(msg_ptr, MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_THRESHOLD_OUT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_register_cell_signal_ind_req_add_threshold_ext(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, int16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_int16(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint16_t mipc_wfc_cssac_cnf_get_bf_voice(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_CSSAC_CNF_T_BF_VOICE, def_val);
}

static inline uint16_t mipc_wfc_cssac_cnf_get_bf_video(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_CSSAC_CNF_T_BF_VIDEO, def_val);
}

static inline uint16_t mipc_wfc_cssac_cnf_get_bt_voice(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_CSSAC_CNF_T_BT_VOICE, def_val);
}

static inline uint16_t mipc_wfc_cssac_cnf_get_bt_video(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_CSSAC_CNF_T_BT_VIDEO, def_val);
}

static inline mipc_msg_api_result_enum mipc_wfc_set_emc_aid_req_add_aid(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_SET_EMC_AID_REQ_T_AID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_cfg_req_add_location_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_CFG_REQ_T_LOCATION_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_cfg_req_add_wfc_prefer(mipc_msg_t *msg_ptr, enum mipc_wfc_prefer_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_CFG_REQ_T_WFC_PREFER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_set_eap_version_req_add_ap_filter_ver(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_WFC_SET_EAP_VERSION_REQ_T_AP_FILTER_VER, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_wfc_set_eap_version_cnf_get_md_filter_ver(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_WFC_SET_EAP_VERSION_CNF_T_MD_FILTER_VER, def_val);
}

static inline mipc_wfc_signal_type_const_enum mipc_wfc_cell_signal_ind_get_signal_type(mipc_msg_t *msg_ptr, mipc_wfc_signal_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_signal_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_CELL_SIGNAL_IND_T_SIGNAL_TYPE, def_val);
}

static inline int16_t mipc_wfc_cell_signal_ind_get_value(mipc_msg_t *msg_ptr, int16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int16_t)mipc_msg_get_val_int16(msg_ptr, MIPC_WFC_CELL_SIGNAL_IND_T_VALUE, def_val);
}

static inline uint8_t mipc_wfc_wifi_pdn_count_ind_get_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WIFI_PDN_COUNT_IND_T_COUNT, def_val);
}

static inline mipc_wfc_pdn_type_const_enum mipc_wfc_pdn_ho_ind_get_pdn_type(mipc_msg_t *msg_ptr, mipc_wfc_pdn_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_pdn_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_PDN_HO_IND_T_PDN_TYPE, def_val);
}

static inline mipc_wfc_pdn_ho_status_const_enum mipc_wfc_pdn_ho_ind_get_status(mipc_msg_t *msg_ptr, mipc_wfc_pdn_ho_status_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_pdn_ho_status_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_PDN_HO_IND_T_STATUS, def_val);
}

static inline mipc_wfc_rat_const_enum mipc_wfc_pdn_ho_ind_get_src_rat(mipc_msg_t *msg_ptr, mipc_wfc_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_rat_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_PDN_HO_IND_T_SRC_RAT, def_val);
}

static inline mipc_wfc_rat_const_enum mipc_wfc_pdn_ho_ind_get_dst_rat(mipc_msg_t *msg_ptr, mipc_wfc_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_rat_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_PDN_HO_IND_T_DST_RAT, def_val);
}

static inline char * mipc_wfc_rove_out_ind_get_ifname(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_ROVE_OUT_IND_T_IFNAME, val_len_ptr);
}

static inline mipc_boolean_const_enum mipc_wfc_rove_out_ind_get_rvout(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_ROVE_OUT_IND_T_RVOUT, def_val);
}

static inline mipc_boolean_const_enum mipc_wfc_rove_out_ind_get_mobike_ind(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_ROVE_OUT_IND_T_MOBIKE_IND, def_val);
}

static inline uint16_t mipc_wfc_ssac_ind_get_bf_voice(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_SSAC_IND_T_BF_VOICE, def_val);
}

static inline uint16_t mipc_wfc_ssac_ind_get_bf_video(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_SSAC_IND_T_BF_VIDEO, def_val);
}

static inline uint16_t mipc_wfc_ssac_ind_get_bt_voice(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_SSAC_IND_T_BT_VOICE, def_val);
}

static inline uint16_t mipc_wfc_ssac_ind_get_bt_video(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_SSAC_IND_T_BT_VIDEO, def_val);
}

static inline int32_t mipc_wfc_wifi_pdn_err_ind_get_cause(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_WFC_WIFI_PDN_ERR_IND_T_CAUSE, def_val);
}

static inline int32_t mipc_wfc_wifi_pdn_err_ind_get_sub_cause(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_WFC_WIFI_PDN_ERR_IND_T_SUB_CAUSE, def_val);
}

static inline mipc_boolean_const_enum mipc_wfc_wifi_pdn_err_ind_get_last_retry(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WIFI_PDN_ERR_IND_T_IS_LAST_RETRY, def_val);
}

static inline char * mipc_wfc_wifi_pdn_oos_ind_get_apn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_WIFI_PDN_OOS_IND_T_APN, val_len_ptr);
}

static inline uint8_t mipc_wfc_wifi_pdn_oos_ind_get_cid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WIFI_PDN_OOS_IND_T_CID, def_val);
}

static inline mipc_wfc_pdn_oos_state_const_enum mipc_wfc_wifi_pdn_oos_ind_get_oos_state(mipc_msg_t *msg_ptr, mipc_wfc_pdn_oos_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_pdn_oos_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WIFI_PDN_OOS_IND_T_OOS_STATE, def_val);
}

static inline mipc_wfc_wifi_pdn_state_const_enum mipc_wfc_wfc_ind_get_wifi_pdn_state(mipc_msg_t *msg_ptr, mipc_wfc_wifi_pdn_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_wifi_pdn_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WFC_IND_T_WIFI_PDN_STATE, def_val);
}

static inline uint8_t mipc_wfc_wfc_ind_get_data_sim(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WFC_IND_T_DATA_SIM, def_val);
}

static inline char * mipc_wfc_wfc_ind_get_ifname(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_WFC_IND_T_IFNAME, val_len_ptr);
}

static inline mipc_boolean_const_enum mipc_wfc_wfc_ind_get_lock(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WFC_IND_T_LOCK, def_val);
}

static inline mipc_wfc_wifi_pdn_state_const_enum mipc_wfc_wfc_ind_get_wifi_ims_pdn_state(mipc_msg_t *msg_ptr, mipc_wfc_wifi_pdn_state_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_wifi_pdn_state_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_WFC_IND_T_WIFI_IMS_PDN_STATE, def_val);
}

static inline mipc_wfc_rat_const_enum mipc_wfc_ping_cmd_get_rat(mipc_msg_t *msg_ptr, mipc_wfc_rat_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_wfc_rat_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_PING_CMD_T_RAT, def_val);
}

static inline mipc_msg_api_result_enum mipc_wfc_ping_rsp_add_rat(mipc_msg_t *msg_ptr, enum mipc_wfc_rat_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_PING_RSP_T_RAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_ping_rsp_add_ave_latency(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_WFC_PING_RSP_T_AVE_LATENCY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_ping_rsp_add_loss_rate(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_PING_RSP_T_LOSS_RATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_wfc_get_mac_cmd_get_ifname(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_GET_MAC_CMD_T_IFNAME, val_len_ptr);
}

static inline void * mipc_wfc_get_mac_cmd_get_ip(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_GET_MAC_CMD_T_IP, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_wfc_get_mac_rsp_add_get_result(mipc_msg_t *msg_ptr, int32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int32(msg_ptr, MIPC_WFC_GET_MAC_RSP_T_GET_RESULT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_get_mac_rsp_add_ifname(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_GET_MAC_RSP_T_IFNAME, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_get_mac_rsp_add_ip(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_GET_MAC_RSP_T_IP, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_get_mac_rsp_add_mac(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_GET_MAC_RSP_T_MAC, len, (const void *)value);
}

static inline mipc_boolean_const_enum mipc_wfc_natt_keep_alive_cmd_get_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_ENABLE, def_val);
}

static inline uint16_t mipc_wfc_natt_keep_alive_cmd_get_interval(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_INTERVAL, def_val);
}

static inline void * mipc_wfc_natt_keep_alive_cmd_get_src_ip(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_SRC_IP, val_len_ptr);
}

static inline uint16_t mipc_wfc_natt_keep_alive_cmd_get_src_port(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_SRC_PORT, def_val);
}

static inline void * mipc_wfc_natt_keep_alive_cmd_get_dst_ip(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_DST_IP, val_len_ptr);
}

static inline mipc_boolean_const_enum mipc_wfc_natt_keep_alive_cmd_get_dst_port(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint16(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_DST_PORT, def_val);
}

static inline mipc_msg_api_result_enum mipc_wfc_natt_keep_alive_rsp_add_ifname(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_IFNAME, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_natt_keep_alive_rsp_add_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_natt_keep_alive_rsp_add_src_ip(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_SRC_IP, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_natt_keep_alive_rsp_add_src_port(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_SRC_PORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_natt_keep_alive_rsp_add_dst_ip(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_DST_IP, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_natt_keep_alive_rsp_add_dst_port(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_DST_PORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_boolean_const_enum mipc_wfc_register_wifi_signal_ntf_cmd_get_enable_ntf(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD_T_ENABLE_NTF, def_val);
}

static inline int16_t mipc_wfc_register_wifi_signal_ntf_cmd_get_rssi_threshold(mipc_msg_t *msg_ptr, int16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (int16_t)mipc_msg_get_idx_int16(msg_ptr, MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD_T_RSSI_THRESHOLD, def_val, index);
}

static inline uint32_t mipc_wfc_set_dl_filter_cmd_get_fid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_WFC_SET_DL_FILTER_CMD_T_FID, def_val);
}

static inline mipc_wfp_filter_struct_struct4* mipc_wfc_set_dl_filter_cmd_get_wfp_filter(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_wfp_filter_struct_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_SET_DL_FILTER_CMD_T_WFP_FILTER, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_wfc_set_dl_filter_rsp_add_fid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_WFC_SET_DL_FILTER_RSP_T_FID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_wfc_del_dl_filter_cmd_get_fid(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_WFC_DEL_DL_FILTER_CMD_T_FID, def_val);
}

static inline mipc_msg_api_result_enum mipc_wfc_del_dl_filter_rsp_add_fid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_WFC_DEL_DL_FILTER_RSP_T_FID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_signal_ntf_add_ifname(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_SIGNAL_NTF_T_IFNAME, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_signal_ntf_add_rssi(mipc_msg_t *msg_ptr, int16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int16(msg_ptr, MIPC_WFC_WIFI_SIGNAL_NTF_T_RSSI, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_signal_ntf_add_snr(mipc_msg_t *msg_ptr, int16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int16(msg_ptr, MIPC_WFC_WIFI_SIGNAL_NTF_T_SNR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_epdg_screen_state_ntf_add_state(mipc_msg_t *msg_ptr, enum mipc_epdg_screen_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_EPDG_SCREEN_STATE_NTF_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ifname(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IFNAME, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_cause(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_CAUSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_associated(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_ASSOCIATED, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ssid(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_SSID, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ap_mac(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_AP_MAC, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_wifi_type(mipc_msg_t *msg_ptr, enum mipc_wifi_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_WIFI_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_mtu(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_MTU, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ue_mac(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_UE_MAC, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ipv4(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IPV4, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ipv4_prefix_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IPV4_PREFIX_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ipv4_gateway(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IPV4_GATEWAY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ipv6(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IPV6, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ipv6_prefix_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IPV6_PREFIX_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ipv6_gateway(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IPV6_GATEWAY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_dns(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_ip_update(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_IP_UPDATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_wifi_type_str(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_WIFI_TYPE_STR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_wifi_extend_state(mipc_msg_t *msg_ptr, enum mipc_wifi_extend_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_WIFI_EXTEND_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_conn_state(mipc_msg_t *msg_ptr, enum mipc_wifi_conn_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_CONN_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_conn_ready(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_CONN_READY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_wfc_wifi_info_ntf_add_apm_state(mipc_msg_t *msg_ptr, enum mipc_wifi_apm_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_WFC_WIFI_INFO_NTF_T_APM_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}


#endif /* __MIPC_MSG_WFC_API_H__ */
