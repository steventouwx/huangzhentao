#ifndef __MIPC_MSG_LOC_API_H__
#define __MIPC_MSG_LOC_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"

static inline mipc_msg_api_result_enum mipc_loc_agps_sync_status_req_add_operation(mipc_msg_t *msg_ptr, enum mipc_gps_status_control_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_AGPS_SYNC_STATUS_REQ_T_OPERATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_sync_status_req_add_assist_req(mipc_msg_t *msg_ptr, int32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int32(msg_ptr, MIPC_LOC_AGPS_SYNC_STATUS_REQ_T_ASSIST_REQ, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_send_pmtk_req_add_pmtk(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_AGPS_SEND_PMTK_REQ_T_PMTK, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_agps_sync_capabilities_req_add_gps_satellite_support(mipc_msg_t *msg_ptr, int8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int8(msg_ptr, MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_GPS_SATELLITE_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_sync_capabilities_req_add_glonass_satellite_support(mipc_msg_t *msg_ptr, int8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int8(msg_ptr, MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_GLONASS_SATELLITE_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_sync_capabilities_req_add_beidou_satellite_support(mipc_msg_t *msg_ptr, int8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int8(msg_ptr, MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_BEIDOU_SATELLITE_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_sync_capabilities_req_add_gaileo_satellite_support(mipc_msg_t *msg_ptr, int8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int8(msg_ptr, MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_GAILEO_SATELLITE_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_query_md_time_cmd_req_add_time_sync_operation(mipc_msg_t *msg_ptr, enum mipc_gps_time_sync_operation_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ_T_TIME_SYNC_OPERATION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_query_md_time_cmd_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_sync_location_em_config_req_add_command(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ_T_COMMAND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_sync_location_em_config_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ_T_DATA, len, (const void *)value);
}

static inline uint8_t mipc_loc_lbs_sync_location_em_config_cnf_get_result(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF_T_RESULT, def_val);
}

static inline void * mipc_loc_lbs_sync_location_em_config_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF_T_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_set_location_em_msg_report_req_add_command(mipc_msg_t *msg_ptr, enum mipc_lbs_em_msg_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_REQ_T_COMMAND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_get_custom_debug_req_add_action_type(mipc_msg_t *msg_ptr, enum mipc_lbs_debug_req_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_LBS_GET_CUSTOM_DEBUG_REQ_T_ACTION_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_get_custom_debug_req_add_msg_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_LBS_GET_CUSTOM_DEBUG_REQ_T_MSG_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_lppe_service_req_add_protocol_type(mipc_msg_t *msg_ptr, enum mipc_lppe_protocol_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_REQ_T_PROTOCOL_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_lppe_service_req_add_msg_type(mipc_msg_t *msg_ptr, enum mipc_lppe_msg_action_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_REQ_T_MSG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_lppe_service_req_add_msg_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_REQ_T_MSG_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_lppe_service_fwk_req_add_action_type(mipc_msg_t *msg_ptr, enum mipc_lppe_supl_req_action_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_FWK_REQ_T_ACTION_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_lppe_service_fwk_req_add_msg_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_FWK_REQ_T_MSG_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_sync_aidl_req_add_action_type(mipc_msg_t *msg_ptr, enum mipc_lbs_aidl_req_action_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_LBS_SYNC_AIDL_REQ_T_ACTION_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_lbs_sync_aidl_req_add_msg_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_LBS_SYNC_AIDL_REQ_T_MSG_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_agps_ni_req_add_session_id(mipc_msg_t *msg_ptr, int32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int32(msg_ptr, MIPC_LOC_AGPS_NI_REQ_T_SESSION_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_ni_req_add_user_response(mipc_msg_t *msg_ptr, int32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int32(msg_ptr, MIPC_LOC_AGPS_NI_REQ_T_USER_RESPONSE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_get_ha_gnss_assist_data_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_agps_set_asn_information_req_add_action_type(mipc_msg_t *msg_ptr, enum mipc_gps_asn_info_action_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ_T_ACTION_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_set_asn_information_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_agps_get_tsx_data_req_add_action_type(mipc_msg_t *msg_ptr, enum mipc_gps_tsx_data_action_enum_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_LOC_AGPS_GET_TSX_DATA_REQ_T_ACTION_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_loc_agps_get_tsx_data_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_AGPS_GET_TSX_DATA_REQ_T_DATA, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_loc_agps_sync_mnld_ver_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_LOC_AGPS_SYNC_MNLD_VER_REQ_T_DATA, len, (const void *)value);
}

static inline uint32_t mipc_loc_lbs_location_em_msg_ind_get_msg_type(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_LOC_LBS_LOCATION_EM_MSG_IND_T_MSG_TYPE, def_val);
}

static inline void * mipc_loc_lbs_location_em_msg_ind_get_msg_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_LBS_LOCATION_EM_MSG_IND_T_MSG_DATA, val_len_ptr);
}

static inline mipc_lbs_debug_ind_enum_const_enum mipc_loc_lbs_custom_debug_ind_get_action_type(mipc_msg_t *msg_ptr, mipc_lbs_debug_ind_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_lbs_debug_ind_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_LBS_CUSTOM_DEBUG_IND_T_ACTION_TYPE, def_val);
}

static inline void * mipc_loc_lbs_custom_debug_ind_get_msg_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_LBS_CUSTOM_DEBUG_IND_T_MSG_DATA, val_len_ptr);
}

static inline mipc_lppe_protocol_enum_const_enum mipc_loc_lbs_lppe_service_ind_get_protocol_type(mipc_msg_t *msg_ptr, mipc_lppe_protocol_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_lppe_protocol_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_IND_T_PROTOCOL_TYPE, def_val);
}

static inline mipc_lppe_msg_action_enum_const_enum mipc_loc_lbs_lppe_service_ind_get_msg_type(mipc_msg_t *msg_ptr, mipc_lppe_msg_action_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_lppe_msg_action_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_IND_T_MSG_TYPE, def_val);
}

static inline void * mipc_loc_lbs_lppe_service_ind_get_msg_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_IND_T_MSG_DATA, val_len_ptr);
}

static inline mipc_lppe_supl_ind_action_enum_const_enum mipc_loc_lbs_lppe_service_fwk_ind_get_action_type(mipc_msg_t *msg_ptr, mipc_lppe_supl_ind_action_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_lppe_supl_ind_action_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_FWK_IND_T_ACTION_TYPE, def_val);
}

static inline void * mipc_loc_lbs_lppe_service_fwk_ind_get_msg_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_LBS_LPPE_SERVICE_FWK_IND_T_MSG_DATA, val_len_ptr);
}

static inline mipc_lbs_aidl_ind_action_enum_const_enum mipc_loc_lbs_aidl_ind_get_action_type(mipc_msg_t *msg_ptr, mipc_lbs_aidl_ind_action_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_lbs_aidl_ind_action_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_LBS_AIDL_IND_T_ACTION_TYPE, def_val);
}

static inline void * mipc_loc_lbs_aidl_ind_get_msg_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_LBS_AIDL_IND_T_MSG_DATA, val_len_ptr);
}

static inline mipc_agps_status_cmd_enum_const_enum mipc_loc_agps_status_ind_get_operation(mipc_msg_t *msg_ptr, mipc_agps_status_cmd_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_agps_status_cmd_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_STATUS_IND_T_OPERATION, def_val);
}

static inline int32_t mipc_loc_agps_status_ind_get_show_gps_icon(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_LOC_AGPS_STATUS_IND_T_SHOW_GPS_ICON, def_val);
}

static inline mipc_mnl_agps_open_type_enum_const_enum mipc_loc_agps_status_ind_get_open_type(mipc_msg_t *msg_ptr, mipc_mnl_agps_open_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_open_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_STATUS_IND_T_OPEN_TYPE, def_val);
}

static inline mipc_mnl_agps_open_requestor_enum_const_enum mipc_loc_agps_status_ind_get_requestor(mipc_msg_t *msg_ptr, mipc_mnl_agps_open_requestor_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_open_requestor_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_STATUS_IND_T_REQUESTOR, def_val);
}

static inline void * mipc_loc_agps_status_ind_get_requestor_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_STATUS_IND_T_REQUESTOR_ID, val_len_ptr);
}

static inline uint8_t mipc_loc_agps_status_ind_get_emergency_call(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_STATUS_IND_T_EMERGENCY_CALL, def_val);
}

static inline int32_t mipc_loc_agps_status_ind_get_flag(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_LOC_AGPS_STATUS_IND_T_FLAG, def_val);
}

static inline mipc_mnl_agps_ni_notify_type_enum_const_enum mipc_loc_agps_ni_ind_get_operation(mipc_msg_t *msg_ptr, mipc_mnl_agps_ni_notify_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_ni_notify_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_NI_IND_T_OPERATION, def_val);
}

static inline int32_t mipc_loc_agps_ni_ind_get_session_id(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_LOC_AGPS_NI_IND_T_SESSION_ID, def_val);
}

static inline mipc_mnl_agps_ni_type_enum_const_enum mipc_loc_agps_ni_ind_get_ni_type(mipc_msg_t *msg_ptr, mipc_mnl_agps_ni_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_ni_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_NI_IND_T_NI_TYPE, def_val);
}

static inline void * mipc_loc_agps_ni_ind_get_requestor_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_NI_IND_T_REQUESTOR_ID, val_len_ptr);
}

static inline void * mipc_loc_agps_ni_ind_get_client_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_NI_IND_T_CLIENT_NAME, val_len_ptr);
}

static inline mipc_mnl_agps_ni_encoding_type_enum_const_enum mipc_loc_agps_ni_ind_get_req_id_encoding(mipc_msg_t *msg_ptr, mipc_mnl_agps_ni_encoding_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_ni_encoding_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_NI_IND_T_REQ_ID_ENCODING, def_val);
}

static inline mipc_mnl_agps_ni_encoding_type_enum_const_enum mipc_loc_agps_ni_ind_get_cl_name_encoding(mipc_msg_t *msg_ptr, mipc_mnl_agps_ni_encoding_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_ni_encoding_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_NI_IND_T_CL_NAME_ENCODING, def_val);
}

static inline mipc_mnl_agps_location_control_enum_const_enum mipc_loc_agps_location_ind_get_operation(mipc_msg_t *msg_ptr, mipc_mnl_agps_location_control_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_location_control_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_OPERATION, def_val);
}

static inline void * mipc_loc_agps_location_ind_get_latitude(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_LATITUDE, val_len_ptr);
}

static inline void * mipc_loc_agps_location_ind_get_longitude(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_LONGITUDE, val_len_ptr);
}

static inline int8_t mipc_loc_agps_location_ind_get_altitude_used(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_ALTITUDE_USED, def_val);
}

static inline void * mipc_loc_agps_location_ind_get_altitude(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_ALTITUDE, val_len_ptr);
}

static inline int8_t mipc_loc_agps_location_ind_get_speed_used(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_SPEED_USED, def_val);
}

static inline void * mipc_loc_agps_location_ind_get_speed(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_SPEED, val_len_ptr);
}

static inline int8_t mipc_loc_agps_location_ind_get_bearing_used(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_BEARING_USED, def_val);
}

static inline void * mipc_loc_agps_location_ind_get_bearing(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_BEARING, val_len_ptr);
}

static inline int8_t mipc_loc_agps_location_ind_get_accuracy_used(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_ACCURACY_USED, def_val);
}

static inline void * mipc_loc_agps_location_ind_get_accuracy(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_ACCURACY, val_len_ptr);
}

static inline int8_t mipc_loc_agps_location_ind_get_timestamp_used(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_TIMESTAMP_USED, def_val);
}

static inline void * mipc_loc_agps_location_ind_get_timestamp(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_TIMESTAMP, val_len_ptr);
}

static inline int8_t mipc_loc_agps_location_ind_get_source_used(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_SOURCE_USED, def_val);
}

static inline mipc_mnl_agps_location_type_enum_const_enum mipc_loc_agps_location_ind_get_source(mipc_msg_t *msg_ptr, mipc_mnl_agps_location_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_mnl_agps_location_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_LOCATION_IND_T_SOURCE, def_val);
}

static inline mipc_agps_md_time_action_enum_const_enum mipc_loc_agps_md_time_cmd_ind_get_operation(mipc_msg_t *msg_ptr, mipc_agps_md_time_action_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_agps_md_time_action_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_MD_TIME_CMD_IND_T_OPERATION, def_val);
}

static inline void * mipc_loc_agps_md_time_cmd_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_MD_TIME_CMD_IND_T_DATA, val_len_ptr);
}

static inline void * mipc_loc_agps_ha_gnss_assist_data_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_HA_GNSS_ASSIST_DATA_IND_T_DATA, val_len_ptr);
}

static inline mipc_agps_tsx_data_action_enum_const_enum mipc_loc_agps_tsx_ind_get_operation(mipc_msg_t *msg_ptr, mipc_agps_tsx_data_action_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_agps_tsx_data_action_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_TSX_IND_T_OPERATION, def_val);
}

static inline void * mipc_loc_agps_tsx_ind_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_TSX_IND_T_DATA, val_len_ptr);
}

static inline mipc_agps_asn_data_type_enum_const_enum mipc_loc_agps_asn_information_ind_get_asn_type(mipc_msg_t *msg_ptr, mipc_agps_asn_data_type_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_agps_asn_data_type_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_ASN_INFORMATION_IND_T_ASN_TYPE, def_val);
}

static inline void * mipc_loc_agps_asn_information_ind_get_asn1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_ASN_INFORMATION_IND_T_ASN1, val_len_ptr);
}

static inline mipc_agps_version_action_enum_const_enum mipc_loc_agps_capabilities_sync_ind_get_operation(mipc_msg_t *msg_ptr, mipc_agps_version_action_enum_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_agps_version_action_enum_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_OPERATION, def_val);
}

static inline int32_t mipc_loc_agps_capabilities_sync_ind_get_version(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_VERSION, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_sib8_16_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_SIB8_16_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_gps_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_GPS_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_glonass_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_GLONASS_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_beidou_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_BEIDOU_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_galileo_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_GALILEO_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_a_gps_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_GPS_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_a_glonass_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_GLONASS_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_a_beidou_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_BEIDOU_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_a_galileo_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_GALILEO_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_lppe_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_LPPE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_qzss_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_QZSS_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_navic_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_NAVIC_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_a_qzss_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_QZSS_SATELLITE_ENABLE, def_val);
}

static inline int8_t mipc_loc_agps_capabilities_sync_ind_get_a_navic_satellite_enable(mipc_msg_t *msg_ptr, int8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int8_t)mipc_msg_get_val_int8(msg_ptr, MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_NAVIC_SATELLITE_ENABLE, def_val);
}

static inline void * mipc_loc_agps_pmtk_ind_get_pmtk(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_LOC_AGPS_PMTK_IND_T_PMTK, val_len_ptr);
}


#endif /* __MIPC_MSG_LOC_API_H__ */
