#ifndef __MIPC_MSG_EMBMS_API_H__
#define __MIPC_MSG_EMBMS_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline uint8_t mipc_embms_emslu_cnf_get_is_enabled(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_EMSLU_CNF_T_IS_ENABLED, def_val);
}

static inline uint16_t mipc_embms_emslu_cnf_get_session_count(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_EMBMS_EMSLU_CNF_T_SESSION_COUNT, def_val);
}

static inline mipc_embms_session_info_struct4* mipc_embms_emslu_cnf_get_session_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_session_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_EMSLU_CNF_T_SESSION_LIST, val_len_ptr);
}

static inline mipc_embms_session_info_struct4* mipc_embms_emslu_cnf_get_session_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_session_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_EMSLU_CNF_T_SESSION_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_embms_get_sai_list_cnf_get_is_enabled(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_GET_SAI_LIST_CNF_T_IS_ENABLED, def_val);
}

static inline mipc_embms_sai_cf_info_struct4* mipc_embms_get_sai_list_cnf_get_sai_cf_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_sai_cf_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_CF_INFO, val_len_ptr);
}

static inline uint8_t mipc_embms_get_sai_list_cnf_get_sai_nf_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_NF_COUNT, def_val);
}

static inline mipc_embms_sai_nf_info_struct4* mipc_embms_get_sai_list_cnf_get_sai_nf_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_sai_nf_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_NF_LIST, val_len_ptr);
}

static inline mipc_embms_sai_nf_info_struct4* mipc_embms_get_sai_list_cnf_get_sai_nf_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_sai_nf_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_NF_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_embms_notify_hvolte_status_req_add_status(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EMBMS_NOTIFY_HVOLTE_STATUS_REQ_T_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_embms_set_service_enable_req_add_broadcast_enable(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EMBMS_SET_SERVICE_ENABLE_REQ_T_BROADCAST_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_embms_set_service_enable_req_add_multicast_enable(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EMBMS_SET_SERVICE_ENABLE_REQ_T_MULTICAST_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_embms_get_service_enable_cnf_get_broadcast_enable(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_GET_SERVICE_ENABLE_CNF_T_BROADCAST_ENABLE, def_val);
}

static inline uint8_t mipc_embms_get_service_enable_cnf_get_multicast_enable(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_GET_SERVICE_ENABLE_CNF_T_MULTICAST_ENABLE, def_val);
}

static inline mipc_msg_api_result_enum mipc_embms_set_broadcast_config_req_add_action(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EMBMS_SET_BROADCAST_CONFIG_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_embms_set_broadcast_config_req_add_tmgi(mipc_msg_t *msg_ptr, uint16_t len, mipc_mbms_tmgi_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_EMBMS_SET_BROADCAST_CONFIG_REQ_T_TMGI, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_embms_set_broadcast_config_req_add_usd_fsai(mipc_msg_t *msg_ptr, uint16_t len, mipc_mbms_usd_fsai_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_EMBMS_SET_BROADCAST_CONFIG_REQ_T_USD_FSAI, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_embms_update_mbs_session_req_add_mbs_option(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_MBS_OPTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_embms_update_mbs_session_req_add_session_id_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_SESSION_ID_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_embms_update_mbs_session_req_add_tmgi(mipc_msg_t *msg_ptr, uint16_t len, mipc_mbms_tmgi_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_TMGI, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_embms_update_mbs_session_req_add_ip_address(mipc_msg_t *msg_ptr, uint16_t len, mipc_mbms_ip_address_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_IP_ADDRESS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_embms_update_mbs_session_req_add_cell_id_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_mbms_cell_id_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_embms_update_mbs_session_req_add_tac_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_mbms_tac_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_embms_query_mbs_multicast_remain_time_req_add_tgmi(mipc_msg_t *msg_ptr, uint16_t len, mipc_mbms_tmgi_info_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_REQ_T_TGMI, len, (const void *)value);
}

static inline uint8_t mipc_embms_query_mbs_multicast_remain_time_cnf_get_remaining_time(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_CNF_T_REMAINING_TIME, def_val);
}

static inline uint8_t mipc_embms_emsrv_ind_get_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_EMSRV_IND_T_STATUS, def_val);
}

static inline mipc_embms_area_id_info_struct4* mipc_embms_emsrv_ind_get_area_id_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_area_id_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_EMSRV_IND_T_AREA_ID_INFO, val_len_ptr);
}

static inline uint16_t mipc_embms_emslui_ind_get_num_sessions(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_EMBMS_EMSLUI_IND_T_NUM_SESSIONS, def_val);
}

static inline mipc_embms_session_info_struct4* mipc_embms_emslui_ind_get_sessions_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_session_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_EMSLUI_IND_T_SESSIONS_INFO, val_len_ptr);
}

static inline mipc_embms_nb_req_info_struct4* mipc_embms_emsailnf_ind_get_mbms_nb_freq_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_nb_req_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_EMSAILNF_IND_T_MBMS_NB_FREQ_INFO, val_len_ptr);
}

static inline uint16_t mipc_embms_emsess_ind_get_num_sessions(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_EMBMS_EMSESS_IND_T_NUM_SESSIONS, def_val);
}

static inline mipc_embms_session_info_struct4* mipc_embms_emsess_ind_get_mbms_session_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_embms_session_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_EMSESS_IND_T_MBMS_SESSION_INFO, val_len_ptr);
}

static inline uint8_t mipc_embms_emsess_ind_get_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_EMSESS_IND_T_CAUSE, def_val);
}

static inline uint8_t mipc_embms_emsess_ind_get_sub_cause(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_EMSESS_IND_T_SUB_CAUSE, def_val);
}

static inline uint8_t mipc_embms_ehvolte_ind_get_mode(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_EHVOLTE_IND_T_MODE, def_val);
}

static inline uint8_t mipc_embms_service_coverage_ind_get_mbms_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_SERVICE_COVERAGE_IND_T_MBMS_STATUS, def_val);
}

static inline mipc_mbms_session_status_struct4* mipc_embms_broadcast_session_list_ind_get_session_status_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_session_status_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_BROADCAST_SESSION_LIST_IND_T_SESSION_STATUS_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_mbms_freq_info_struct4* mipc_embms_mbs_freq_ind_get_freq_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_freq_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_MBS_FREQ_IND_T_FREQ_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_embms_session_status_ind_get_service_status(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_SESSION_STATUS_IND_T_SERVICE_STATUS, def_val);
}

static inline mipc_mbms_tmgi_info_struct4* mipc_embms_session_status_ind_get_tmgi_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_tmgi_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_SESSION_STATUS_IND_T_TMGI_INFO, val_len_ptr);
}

static inline mipc_mbms_usd_fsai_info_struct4* mipc_embms_sai_intra_list_ind_get_usd_fsai(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_usd_fsai_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_SAI_INTRA_LIST_IND_T_USD_FSAI, val_len_ptr);
}

static inline mipc_mbms_fsai_neighbor_info_struct4* mipc_embms_sai_neighbor_list_ind_get_fsai_neighbor_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_fsai_neighbor_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_SAI_NEIGHBOR_LIST_IND_T_FSAI_NEIGHBOR_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_mbms_tmgi_info_struct4* mipc_embms_mbs_multicast_area_info_update_ind_get_tmgi_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_tmgi_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_MBS_MULTICAST_AREA_INFO_UPDATE_IND_T_TMGI_TLV_ARRAY, index, val_len_ptr);
}

static inline void * mipc_embms_mbs_multicast_area_info_update_ind_get_area_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_MBS_MULTICAST_AREA_INFO_UPDATE_IND_T_AREA_INFO_LIST, val_len_ptr);
}

static inline mipc_mbms_tmgi_info_struct4* mipc_embms_mbs_session_update_ind_get_tmgi_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_tmgi_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_TMGI_INFO, val_len_ptr);
}

static inline uint8_t mipc_embms_mbs_session_update_ind_get_psi(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_PSI, def_val);
}

static inline mipc_mbms_ip_address_info_struct4* mipc_embms_mbs_session_update_ind_get_ip_address(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_ip_address_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_IP_ADDRESS, val_len_ptr);
}

static inline mipc_mbs_decision_const_enum mipc_embms_mbs_session_update_ind_get_mbs_decision(mipc_msg_t *msg_ptr, mipc_mbs_decision_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mbs_decision_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_MBS_DECISION, def_val);
}

static inline mipc_mbs_reject_cause_const_enum mipc_embms_mbs_session_update_ind_get_mbs_rej_cause(mipc_msg_t *msg_ptr, mipc_mbs_reject_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mbs_reject_cause_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_MBS_REJ_CAUSE, def_val);
}

static inline void * mipc_embms_mbs_session_update_ind_get_mbs_sec_container_raw(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_MBS_SEC_CONTAINER_RAW, val_len_ptr);
}

static inline mipc_mbms_cell_id_info_struct4* mipc_embms_mbs_session_update_ind_get_cell_id_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_cell_id_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_CELL_ID_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_mbms_tac_info_struct4* mipc_embms_mbs_session_update_ind_get_tac_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_mbms_tac_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_TAC_TLV_ARRAY, index, val_len_ptr);
}


#endif /* __MIPC_MSG_EMBMS_API_H__ */
