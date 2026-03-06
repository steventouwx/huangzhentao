#ifndef __MIPC_MSG_SYS_API_H__
#define __MIPC_MSG_SYS_API_H__

#include "mipc_msg_tlv_const.h"
#include "mipc_msg.h"


static inline mipc_sys_cap_cellular_class_const_enum mipc_sys_get_info_cnf_get_sys_cellular_class(mipc_msg_t *msg_ptr, mipc_sys_cap_cellular_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_cellular_class_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_SYS_CELLULAR_CLASS, def_val);
}

static inline mipc_sys_voice_class_const_enum mipc_sys_get_info_cnf_get_voice_class(mipc_msg_t *msg_ptr, mipc_sys_voice_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_voice_class_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_VOICE_CLASS, def_val);
}

static inline mipc_sys_sim_class_const_enum mipc_sys_get_info_cnf_get_sim_class(mipc_msg_t *msg_ptr, mipc_sys_sim_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_sim_class_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_SIM_CLASS, def_val);
}

static inline mipc_sys_cap_data_const_enum mipc_sys_get_info_cnf_get_data_class(mipc_msg_t *msg_ptr, mipc_sys_cap_data_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_data_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_DATA_CLASS, def_val);
}

static inline mipc_sys_cap_gsm_band_const_enum mipc_sys_get_info_cnf_get_gsm_band_class(mipc_msg_t *msg_ptr, mipc_sys_cap_gsm_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_gsm_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_GSM_BAND_CLASS, def_val);
}

static inline mipc_sys_cap_umts_band_const_enum mipc_sys_get_info_cnf_get_umts_band_class(mipc_msg_t *msg_ptr, mipc_sys_cap_umts_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_umts_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_UMTS_BAND_CLASS, def_val);
}

static inline mipc_sys_cap_tds_band_const_enum mipc_sys_get_info_cnf_get_tds_band_class(mipc_msg_t *msg_ptr, mipc_sys_cap_tds_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_tds_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_TDS_BAND_CLASS, def_val);
}

static inline mipc_sys_cap_cdma_band_const_enum mipc_sys_get_info_cnf_get_c2k_band_class(mipc_msg_t *msg_ptr, mipc_sys_cap_cdma_band_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_cdma_band_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_C2K_BAND_CLASS, def_val);
}

static inline mipc_sys_lte_band_struct4* mipc_sys_get_info_cnf_get_lte_band_class(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_lte_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_LTE_BAND_CLASS, val_len_ptr);
}

static inline mipc_sys_nr_band_struct4* mipc_sys_get_info_cnf_get_nr_band_class(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_nr_band_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_NR_BAND_CLASS, val_len_ptr);
}

static inline mipc_sys_cap_sms_const_enum mipc_sys_get_info_cnf_get_sms_caps(mipc_msg_t *msg_ptr, mipc_sys_cap_sms_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_sms_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_SMS_CAPS, def_val);
}

static inline mipc_sys_cap_ctrl_const_enum mipc_sys_get_info_cnf_get_ctrl_caps(mipc_msg_t *msg_ptr, mipc_sys_cap_ctrl_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_ctrl_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_CTRL_CAPS, def_val);
}

static inline mipc_sys_auth_algo_const_enum mipc_sys_get_info_cnf_get_auth_algo_caps(mipc_msg_t *msg_ptr, mipc_sys_auth_algo_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_auth_algo_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_AUTH_ALGO_CAPS, def_val);
}

static inline mipc_sys_cap_service_const_enum mipc_sys_get_info_cnf_get_service_caps(mipc_msg_t *msg_ptr, mipc_sys_cap_service_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_service_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_SERVICE_CAPS, def_val);
}

static inline mipc_sys_modem_struct8* mipc_sys_get_info_cnf_get_multi_md(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_modem_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_MULTI_MD, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_device_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_DEVICE_ID, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_manufctr(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_MANUFCTR, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_firmware(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_FIRMWARE, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_hardware(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_HARDWARE, val_len_ptr);
}

static inline uint16_t mipc_sys_get_info_cnf_get_max_active_ctxt(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_MAX_ACTIVE_CTXT, def_val);
}

static inline uint16_t mipc_sys_get_info_cnf_get_executor_idx(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_EXECUTOR_IDX, def_val);
}

static inline char * mipc_sys_get_info_cnf_get_custom_class_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_CUSTOM_CLASS_NAME, val_len_ptr);
}

static inline mipc_sys_product_type_const_enum mipc_sys_get_info_cnf_get_product_type(mipc_msg_t *msg_ptr, mipc_sys_product_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_product_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_PRODUCT_TYPE, def_val);
}

static inline char * mipc_sys_get_info_cnf_get_esn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_ESN, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_imeisv(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_IMEISV, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_meid(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_MEID, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_firmware_custom(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_FIRMWARE_CUSTOM, val_len_ptr);
}

static inline void * mipc_sys_get_info_cnf_get_hardware_id(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_HARDWARE_ID, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_serial_number(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_SERIAL_NUMBER, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_project_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_PROJECT_NAME, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_flavor_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_FLAVOR_NAME, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_esn_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_ESN_V1, val_len_ptr);
}

static inline char * mipc_sys_get_info_cnf_get_meid_v1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_MEID_V1, val_len_ptr);
}

static inline mipc_sys_cap_cellular_class_const_enum mipc_sys_get_info_cnf_get_sys_cellular_class_v1(mipc_msg_t *msg_ptr, mipc_sys_cap_cellular_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_cap_cellular_class_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_INFO_CNF_T_SYS_CELLULAR_CLASS_V1, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_at_req_add_atcmd(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_AT_REQ_T_ATCMD, len, (const void *)value);
}

static inline char * mipc_sys_at_cnf_get_atcmd(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_AT_CNF_T_ATCMD, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_reboot_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_reboot_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_REBOOT_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_reboot_req_add_timeout(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_REBOOT_REQ_T_TIMEOUT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_mapping_req_add_msg(mipc_msg_t *msg_ptr, enum mipc_sys_sim_ps_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_GET_MAPPING_REQ_T_MSG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sys_get_mapping_cnf_get_mapping_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_COUNT, def_val);
}

static inline mipc_sys_mapping_struct4* mipc_sys_get_mapping_cnf_get_mapping_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_mapping_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_LIST, val_len_ptr);
}

static inline uint32_t mipc_sys_get_mapping_cnf_get_mapping_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_TLV_ARRAY, def_val, index);
}

static inline mipc_msg_api_result_enum mipc_sys_set_mapping_req_add_mapping_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_MAPPING_REQ_T_MAPPING_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mapping_req_add_mapping_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_mapping_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_MAPPING_REQ_T_MAPPING_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_mapping_req_add_mapping_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_idx_uint32(msg_ptr, array, index, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sys_set_mapping_cnf_get_mapping_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_SET_MAPPING_CNF_T_MAPPING_COUNT, def_val);
}

static inline mipc_sys_mapping_struct4* mipc_sys_set_mapping_cnf_get_mapping_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_mapping_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SET_MAPPING_CNF_T_MAPPING_LIST, val_len_ptr);
}

static inline uint32_t mipc_sys_set_mapping_cnf_get_mapping_tlv_array(mipc_msg_t *msg_ptr, uint32_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_idx_uint32(msg_ptr, MIPC_SYS_SET_MAPPING_CNF_T_MAPPING_TLV_ARRAY, def_val, index);
}

static inline uint32_t mipc_sys_get_thermal_sensor_num_cnf_get_num(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_NUM_CNF_T_NUM, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_get_thermal_sensor_info_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_thermal_sensor_info_cnf_get_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_ID, def_val);
}

static inline char * mipc_sys_get_thermal_sensor_info_cnf_get_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_NAME, val_len_ptr);
}

static inline uint8_t mipc_sys_get_thermal_sensor_info_cnf_get_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_TYPE, def_val);
}

static inline uint8_t mipc_sys_get_thermal_sensor_info_cnf_get_meas_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MEAS_TYPE, def_val);
}

static inline int32_t mipc_sys_get_thermal_sensor_info_cnf_get_min_tempature(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MIN_TEMPATURE, def_val);
}

static inline int32_t mipc_sys_get_thermal_sensor_info_cnf_get_max_tempature(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MAX_TEMPATURE, def_val);
}

static inline uint32_t mipc_sys_get_thermal_sensor_info_cnf_get_accuracy(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_ACCURACY, def_val);
}

static inline uint32_t mipc_sys_get_thermal_sensor_info_cnf_get_resolution(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_RESOLUTION, def_val);
}

static inline int32_t mipc_sys_get_thermal_sensor_info_cnf_get_warn_tempature(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_WARN_TEMPATURE, def_val);
}

static inline int32_t mipc_sys_get_thermal_sensor_info_cnf_get_hw_shutdown_temperature(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_HW_SHUTDOWN_TEMPERATURE, def_val);
}

static inline uint32_t mipc_sys_get_thermal_sensor_info_cnf_get_min_sampling_period(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MIN_SAMPLING_PERIOD, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_sensor_req_add_config_count(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_THERMAL_SENSOR_REQ_T_CONFIG_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_sensor_req_add_config_list(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_thermal_sensor_config_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_THERMAL_SENSOR_REQ_T_CONFIG_LIST, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_sensor_req_add_config_tlv_array(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_sys_thermal_sensor_config_e_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_sensor_req_add_config_e(mipc_msg_t *msg_ptr, mipc_msg_tlv_array_t *array, uint16_t index, uint16_t len, mipc_sys_thermal_sensor_config_e_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_idx(msg_ptr, array, index, len, (const void *)value);
}

static inline uint8_t mipc_sys_set_thermal_sensor_cnf_get_config_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_COUNT, def_val);
}

static inline mipc_sys_thermal_sensor_config_struct4* mipc_sys_set_thermal_sensor_cnf_get_config_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_config_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_LIST, val_len_ptr);
}

static inline mipc_sys_thermal_sensor_config_e_struct4* mipc_sys_set_thermal_sensor_cnf_get_config_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_config_e_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sys_thermal_sensor_config_e_struct4* mipc_sys_set_thermal_sensor_cnf_get_config_e(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_config_e_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_E, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_get_thermal_sensor_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline int32_t mipc_sys_get_thermal_sensor_cnf_get_temperature(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_CNF_T_TEMPERATURE, def_val);
}

static inline uint32_t mipc_sys_get_thermal_actuator_num_cnf_get_num(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_NUM_CNF_T_NUM, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_actuator_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_THERMAL_ACTUATOR_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_actuator_req_add_level(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_THERMAL_ACTUATOR_REQ_T_LEVEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_actuator_req_add_state(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_THERMAL_ACTUATOR_REQ_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_thermal_actuator_info_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_thermal_actuator_info_cnf_get_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_ID, def_val);
}

static inline char * mipc_sys_get_thermal_actuator_info_cnf_get_name(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_NAME, val_len_ptr);
}

static inline uint32_t mipc_sys_get_thermal_actuator_info_cnf_get_total_level(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_TOTAL_LEVEL, def_val);
}

static inline uint32_t mipc_sys_get_thermal_actuator_info_cnf_get_current_level(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_CURRENT_LEVEL, def_val);
}

static inline uint32_t mipc_sys_get_thermal_actuator_info_cnf_get_user_impact(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_USER_IMPACT, def_val);
}

static inline uint32_t mipc_sys_get_thermal_actuator_info_cnf_get_efficiency(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_EFFICIENCY, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_config_req_add_class(mipc_msg_t *msg_ptr, enum mipc_sys_config_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_CONFIG_REQ_T_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_config_req_add_type(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_CONFIG_REQ_T_TYPE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_config_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_CONFIG_REQ_T_DATA, len, (const void *)value);
}

static inline void * mipc_sys_set_config_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SET_CONFIG_CNF_T_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_get_config_req_add_class(mipc_msg_t *msg_ptr, enum mipc_sys_config_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_CONFIG_REQ_T_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_config_req_add_type(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_GET_CONFIG_REQ_T_TYPE, len, (const void *)value);
}

static inline void * mipc_sys_get_config_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_CONFIG_CNF_T_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_reg_config_req_add_class(mipc_msg_t *msg_ptr, enum mipc_sys_config_class_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_REG_CONFIG_REQ_T_CLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_reg_config_req_add_type(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_REG_CONFIG_REQ_T_TYPE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_adpclk_req_add_state(mipc_msg_t *msg_ptr, enum mipc_sys_adpclk_state_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_ADPCLK_REQ_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sys_get_adpclk_cnf_get_freq_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_ADPCLK_CNF_T_FREQ_INFO_COUNT, def_val);
}

static inline mipc_sys_adpclk_freq_info_struct8* mipc_sys_get_adpclk_cnf_get_freq_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_adpclk_freq_info_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_ADPCLK_CNF_T_FREQ_INFO_LIST, val_len_ptr);
}

static inline mipc_sys_adpclk_freq_info_struct8* mipc_sys_get_adpclk_cnf_get_freq_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_adpclk_freq_info_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_GET_ADPCLK_CNF_T_FREQ_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_md_log_mode_req_add_mode(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MD_LOG_MODE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_md_log_mode_cnf_get_mode(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_MD_LOG_MODE_CNF_T_MODE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_md_log_level_req_add_level(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MD_LOG_LEVEL_REQ_T_LEVEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_md_log_level_cnf_get_level(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_MD_LOG_LEVEL_CNF_T_LEVEL, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_md_log_location_req_add_enable(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MD_LOG_LOCATION_REQ_T_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_md_log_location_cnf_get_enable(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_MD_LOG_LOCATION_CNF_T_ENABLE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_write_nvram_req_add_lid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_WRITE_NVRAM_REQ_T_LID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_write_nvram_req_add_file_idx(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_WRITE_NVRAM_REQ_T_FILE_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_write_nvram_req_add_record_idx(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_WRITE_NVRAM_REQ_T_RECORD_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_write_nvram_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_WRITE_NVRAM_REQ_T_DATA, len, (const void *)value);
}

static inline uint32_t mipc_sys_write_nvram_cnf_get_data_len(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_WRITE_NVRAM_CNF_T_DATA_LEN, def_val);
}

static inline mipc_sys_nvram_cause_const_enum mipc_sys_write_nvram_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_sys_nvram_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_nvram_cause_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_WRITE_NVRAM_CNF_T_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_read_nvram_req_add_lid(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_READ_NVRAM_REQ_T_LID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_read_nvram_req_add_file_idx(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_READ_NVRAM_REQ_T_FILE_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_read_nvram_req_add_record_idx(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_READ_NVRAM_REQ_T_RECORD_IDX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline void * mipc_sys_read_nvram_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_READ_NVRAM_CNF_T_DATA, val_len_ptr);
}

static inline mipc_sys_nvram_cause_const_enum mipc_sys_read_nvram_cnf_get_cause(mipc_msg_t *msg_ptr, mipc_sys_nvram_cause_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_nvram_cause_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_READ_NVRAM_CNF_T_CAUSE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_auth_req_add_op(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_AUTH_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_auth_req_add_encdata(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_AUTH_REQ_T_ENCDATA, len, (const void *)value);
}

static inline void * mipc_sys_auth_cnf_get_rand(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_AUTH_CNF_T_RAND, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_dat_req_add_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_DAT_REQ_T_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_dat_cnf_get_index(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_DAT_CNF_T_INDEX, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_op(mipc_msg_t *msg_ptr, enum mipc_sys_mcf_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_config_type(mipc_msg_t *msg_ptr, enum mipc_sys_mcf_config_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_CONFIG_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_dump_lids(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_MCF_REQ_T_DUMP_LIDS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_path_type(mipc_msg_t *msg_ptr, enum mipc_sys_mcf_path_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_PATH_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_trigger_dsbp(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_TRIGGER_DSBP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_action(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_ACTION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_format(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_FORMAT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_num(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_MCF_REQ_T_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_len(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_rec_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_MCF_REQ_T_REC_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_value(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_MCF_REQ_T_VALUE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_config1(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_MCF_REQ_T_CONFIG1, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_config(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_MCF_REQ_T_CONFIG, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_mcf_req_add_is_reset(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MCF_REQ_T_IS_RESET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_mcf_cnf_get_mcf_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_MCF_CNF_T_MCF_RESULT, def_val);
}

static inline uint32_t mipc_sys_mcf_cnf_get_dsbp_result(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_MCF_CNF_T_DSBP_RESULT, def_val);
}

static inline mipc_sys_mcf_op_const_enum mipc_sys_mcf_cnf_get_op(mipc_msg_t *msg_ptr, mipc_sys_mcf_op_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_mcf_op_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_CNF_T_OP, def_val);
}

static inline mipc_sys_mcf_config_type_const_enum mipc_sys_mcf_cnf_get_config_type(mipc_msg_t *msg_ptr, mipc_sys_mcf_config_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_mcf_config_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_CNF_T_CONFIG_TYPE, def_val);
}

static inline mipc_sys_mcf_path_type_const_enum mipc_sys_mcf_cnf_get_path_type(mipc_msg_t *msg_ptr, mipc_sys_mcf_path_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_mcf_path_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_CNF_T_PATH_TYPE, def_val);
}

static inline uint8_t mipc_sys_mcf_cnf_get_action(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_CNF_T_ACTION, def_val);
}

static inline uint8_t mipc_sys_mcf_cnf_get_format(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_CNF_T_FORMAT, def_val);
}

static inline uint8_t mipc_sys_mcf_cnf_get_len(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_CNF_T_LEN, def_val);
}

static inline void * mipc_sys_mcf_cnf_get_value(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_MCF_CNF_T_VALUE, val_len_ptr);
}

static inline char * mipc_sys_mcf_cnf_get_config1(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_MCF_CNF_T_CONFIG1, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_fcc_lock_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_FCC_LOCK_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_time_req_add_year(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_TIME_REQ_T_YEAR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_time_req_add_month(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_TIME_REQ_T_MONTH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_time_req_add_day(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_TIME_REQ_T_DAY, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_time_req_add_hour(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_TIME_REQ_T_HOUR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_time_req_add_minute(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_TIME_REQ_T_MINUTE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_time_req_add_second(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_TIME_REQ_T_SECOND, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_time_req_add_tz_off_min(mipc_msg_t *msg_ptr, int32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_int32(msg_ptr, MIPC_SYS_SET_TIME_REQ_T_TZ_OFF_MIN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_time_cnf_get_year(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_YEAR, def_val);
}

static inline uint32_t mipc_sys_get_time_cnf_get_month(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_MONTH, def_val);
}

static inline uint32_t mipc_sys_get_time_cnf_get_day(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_DAY, def_val);
}

static inline uint32_t mipc_sys_get_time_cnf_get_hour(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_HOUR, def_val);
}

static inline uint32_t mipc_sys_get_time_cnf_get_minute(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_MINUTE, def_val);
}

static inline uint32_t mipc_sys_get_time_cnf_get_second(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_SECOND, def_val);
}

static inline uint32_t mipc_sys_get_time_cnf_get_timestamp(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_TIMESTAMP, def_val);
}

static inline int32_t mipc_sys_get_time_cnf_get_tz_off_min(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_SYS_GET_TIME_CNF_T_TZ_OFF_MIN, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_sar_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_sar_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_SAR_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_sar_req_add_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_SAR_REQ_T_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sys_sar_mode_const_enum mipc_sys_set_sar_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_sys_sar_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_sar_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_SET_SAR_CNF_T_MODE, def_val);
}

static inline uint32_t mipc_sys_set_sar_cnf_get_index(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_SET_SAR_CNF_T_INDEX, def_val);
}

static inline mipc_sys_sar_mode_const_enum mipc_sys_get_sar_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_sys_sar_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_sar_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_SAR_CNF_T_MODE, def_val);
}

static inline uint32_t mipc_sys_get_sar_cnf_get_index(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_SAR_CNF_T_INDEX, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_power_saving_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_POWER_SAVING_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_connectivity_statistics_req_add_read_flag(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_READ_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_connectivity_statistics_req_add_start(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_START, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_connectivity_statistics_req_add_stop(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_STOP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_connectivity_statistics_req_add_period_value(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_PERIOD_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_connectivity_statistics_cnf_get_sms_tx_counter(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_SMS_TX_COUNTER, def_val);
}

static inline uint32_t mipc_sys_connectivity_statistics_cnf_get_sms_rx_counter(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_SMS_RX_COUNTER, def_val);
}

static inline uint32_t mipc_sys_connectivity_statistics_cnf_get_tx_data(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_TX_DATA, def_val);
}

static inline uint32_t mipc_sys_connectivity_statistics_cnf_get_rx_data(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_RX_DATA, def_val);
}

static inline uint32_t mipc_sys_connectivity_statistics_cnf_get_max_message_size(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_MAX_MESSAGE_SIZE, def_val);
}

static inline uint32_t mipc_sys_connectivity_statistics_cnf_get_average_message_size(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_AVERAGE_MESSAGE_SIZE, def_val);
}

static inline uint32_t mipc_sys_connectivity_statistics_cnf_get_period_value(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_PERIOD_VALUE, def_val);
}

static inline void * mipc_sys_connectivity_statistics_cnf_get_tx_data_ext(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_TX_DATA_EXT, val_len_ptr);
}

static inline void * mipc_sys_connectivity_statistics_cnf_get_rx_data_ext(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_RX_DATA_EXT, val_len_ptr);
}

static inline uint32_t mipc_sys_query_sbp_cnf_get_sbp_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_QUERY_SBP_CNF_T_SBP_ID, def_val);
}

static inline uint32_t mipc_sys_query_sbp_cnf_get_sim_sbp_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_QUERY_SBP_CNF_T_SIM_SBP_ID, def_val);
}

static inline char * mipc_sys_query_sbp_cnf_get_sbp_feature_byte(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_QUERY_SBP_CNF_T_SBP_FEATURE_BYTE, val_len_ptr);
}

static inline char * mipc_sys_query_sbp_cnf_get_sbp_data_byte(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_QUERY_SBP_CNF_T_SBP_DATA_BYTE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_tx_ind_interval_req_add_interval(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_TX_IND_INTERVAL_REQ_T_INTERVAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_account_id(mipc_msg_t *msg_ptr, enum mipc_sys_account_id_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCOUNT_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_broadcast_flag(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_BROADCAST_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_latitude(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_LATITUDE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_longitude(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_LONGITUDE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_accuracy(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_method(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_METHOD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_city(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_CITY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_state(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_STATE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_zip(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_ZIP, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_country_code(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_COUNTRY_CODE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_ue_wlan_mac(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_UE_WLAN_MAC, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_confidence(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_CONFIDENCE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_altitude(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_ALTITUDE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_accuracy_semi_major_axis(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_SEMI_MAJOR_AXIS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_accuracy_semi_minor_axis(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_SEMI_MINOR_AXIS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_geo_location_req_add_accuracy_vertical_axis(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_VERTICAL_AXIS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_dsbp_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_dsbp_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_DSBP_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_send_sar_ind_req_add_cmd_type(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SEND_SAR_IND_REQ_T_CMD_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_send_sar_ind_req_add_cmd_param(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SEND_SAR_IND_REQ_T_CMD_PARAM, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_silent_reboot_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SILENT_REBOOT_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_multi_sim_config_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_MULTI_SIM_CONFIG_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_reboot_set_req_add_mode(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_REBOOT_SET_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_thermal_actuator_req_add_get_actuator_num(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_REQ_T_GET_ACTUATOR_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_thermal_actuator_req_add_actuator_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_REQ_T_ACTUATOR_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_thermal_actuator_cnf_get_actuator_num(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_CNF_T_ACTUATOR_NUM, def_val);
}

static inline mipc_sys_thermal_actuator_state_info_struct4* mipc_sys_get_thermal_actuator_cnf_get_actuator_state_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_actuator_state_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_THERMAL_ACTUATOR_CNF_T_ACTUATOR_STATE_INFO, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_get_thermal_sensor_runtime_req_add_get_md_auto_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_REQ_T_GET_MD_AUTO_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_thermal_sensor_runtime_req_add_sensor_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_REQ_T_SENSOR_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_boolean_const_enum mipc_sys_get_thermal_sensor_runtime_cnf_get_md_auto_enable(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF_T_MD_AUTO_ENABLE, def_val);
}

static inline mipc_sys_thermal_trip_map_struct4* mipc_sys_get_thermal_sensor_runtime_cnf_get_trip_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_trip_map_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF_T_TRIP_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sys_thermal_trip_map_struct4* mipc_sys_get_thermal_sensor_runtime_cnf_get_trip(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_trip_map_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF_T_TRIP, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_runtime_req_add_reset_cfg_from_nv(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_RESET_CFG_FROM_NV, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_runtime_req_add_md_auto_enable(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_MD_AUTO_ENABLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_runtime_req_add_trip_change(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_thermal_trip_change_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_TRIP_CHANGE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_thermal_runtime_req_add_save_cfg_to_nv(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_SAVE_CFG_TO_NV, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_or_get_sbp_info_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_sbp_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_or_get_sbp_info_req_add_feature_int(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_FEATURE_INT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_or_get_sbp_info_req_add_feature_str(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_FEATURE_STR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_or_get_sbp_info_req_add_data(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_or_get_sbp_info_req_add_param(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_PARAM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sys_set_or_get_sbp_info_cnf_get_data(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_SET_OR_GET_SBP_INFO_CNF_T_DATA, def_val);
}

static inline mipc_boolean_const_enum mipc_sys_get_all_thermal_info_cnf_get_auto_flag(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_AUTO_FLAG, def_val);
}

static inline mipc_sys_thermal_sensor_info_e_struct4* mipc_sys_get_all_thermal_info_cnf_get_thermal_temp_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_info_e_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_THERMAL_TEMP_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sys_thermal_sensor_info_e_struct4* mipc_sys_get_all_thermal_info_cnf_get_thermal_temp_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_info_e_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_THERMAL_TEMP_INFO, index, val_len_ptr);
}

static inline mipc_sys_thermal_actuator_state_info_struct4* mipc_sys_get_all_thermal_info_cnf_get_actuator_state_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_actuator_state_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_ACTUATOR_STATE_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sys_thermal_actuator_state_info_struct4* mipc_sys_get_all_thermal_info_cnf_get_actuator_state_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_actuator_state_info_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_ACTUATOR_STATE_INFO, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_meta_req_add_local(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_META_REQ_T_LOCAL, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_meta_req_add_peer(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_META_REQ_T_PEER, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_meta_req_add_checksum(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_META_REQ_T_CHECKSUM, len, (const void *)value);
}

static inline void * mipc_sys_meta_cnf_get_local(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_META_CNF_T_LOCAL, val_len_ptr);
}

static inline void * mipc_sys_meta_cnf_get_peer(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_META_CNF_T_PEER, val_len_ptr);
}

static inline void * mipc_sys_meta_cnf_get_checksum(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_META_CNF_T_CHECKSUM, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_dmf_req_add_category_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_DMF_REQ_T_CATEGORY_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_dmf_req_add_rawdata(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_DMF_REQ_T_RAWDATA, len, (const void *)value);
}

static inline uint32_t mipc_sys_dmf_cnf_get_category_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_DMF_CNF_T_CATEGORY_ID, def_val);
}

static inline void * mipc_sys_dmf_cnf_get_rawdata_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_DMF_CNF_T_RAWDATA_TLV_ARRAY, index, val_len_ptr);
}

static inline char * mipc_sys_dmf_cnf_get_atstring_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_DMF_CNF_T_ATSTRING_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_meta_forwarder_ctrl_req_add_selector(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_META_FORWARDER_CTRL_REQ_T_SELECTOR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_meta_forwarder_ctrl_req_add_checksum(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_META_FORWARDER_CTRL_REQ_T_CHECKSUM, len, (const void *)value);
}

static inline void * mipc_sys_meta_forwarder_ctrl_cnf_get_selector(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_META_FORWARDER_CTRL_CNF_T_SELECTOR, val_len_ptr);
}

static inline void * mipc_sys_meta_forwarder_ctrl_cnf_get_checksum(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_META_FORWARDER_CTRL_CNF_T_CHECKSUM, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_md_log_flush_interval_req_add_flush_interval(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MD_LOG_FLUSH_INTERVAL_REQ_T_FLUSH_INTERVAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_md_log_flush_interval_cnf_get_flush_interval(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_MD_LOG_FLUSH_INTERVAL_CNF_T_FLUSH_INTERVAL, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_proxy_key(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_PROXY_KEY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_if_id(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IF_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_hitchhike_interval(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_HITCHHIKE_INTERVAL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_hb_pattern(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_HB_PATTERN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_hb_ack_pattern(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_HB_ACK_PATTERN, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_allow_dynamic_cycle(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_ALLOW_DYNAMIC_CYCLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_cycle_value(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_CYCLE_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_max_cycle(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_MAX_CYCLE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_cycle_step(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_CYCLE_STEP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_cycle_step_success_num(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_CYCLE_STEP_SUCCESS_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_ipv4_tos(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV4_TOS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_ipv4_ttl(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV4_TTL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_ipv4_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV4_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_ipv6_flow_label(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV6_FLOW_LABEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_ipv6_hop_limit(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV6_HOP_LIMIT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_ipv6_tclass(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV6_TCLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_tcp_rcv_wscale(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_RCV_WSCALE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_tcp_snd_wscale(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_SND_WSCALE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_tcp_wscale_ok(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_WSCALE_OK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_tcp_snd_window(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_SND_WINDOW, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_tcp_rcv_window(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_RCV_WINDOW, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_tcp_snd_nxt_seq(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_SND_NXT_SEQ, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_tcp_rcv_nxt_seq(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_RCV_NXT_SEQ, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_v4_src_addr(mipc_msg_t *msg_ptr, uint16_t len, mipc_data_v4_addr_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_V4_SRC_ADDR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_v4_dest_addr(mipc_msg_t *msg_ptr, uint16_t len, mipc_data_v4_addr_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_V4_DEST_ADDR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_v6_src_addr(mipc_msg_t *msg_ptr, uint16_t len, mipc_data_v6_addr_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_V6_SRC_ADDR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_v6_dest_addr(mipc_msg_t *msg_ptr, uint16_t len, mipc_data_v6_addr_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_V6_DEST_ADDR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_is_ipv6(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_IS_IPV6, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_protocol(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_PROTOCOL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_src_port(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_SRC_PORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_dest_port(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_DEST_PORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_establish_req_add_max_retry_times(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_ESTABLISH_REQ_T_MAX_RETRY_TIMES, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_sys_hba_establish_cnf_get_proxy_key(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_HBA_ESTABLISH_CNF_T_PROXY_KEY, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_ctrl_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_hba_ctrl_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_ctrl_req_add_proxy_key(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_CTRL_REQ_T_PROXY_KEY, len, (const void *)value);
}

static inline mipc_sys_hba_ctrl_mode_const_enum mipc_sys_hba_ctrl_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_sys_hba_ctrl_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_hba_ctrl_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_MODE, def_val);
}

static inline char * mipc_sys_hba_ctrl_cnf_get_proxy_key(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_PROXY_KEY, val_len_ptr);
}

static inline uint8_t mipc_sys_hba_ctrl_cnf_get_ipv4_tos(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_IPV4_TOS, def_val);
}

static inline uint8_t mipc_sys_hba_ctrl_cnf_get_ipv4_ttl(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_IPV4_TTL, def_val);
}

static inline uint16_t mipc_sys_hba_ctrl_cnf_get_ipv4_id(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_IPV4_ID, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_ipv6_flow_label(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_IPV6_FLOW_LABEL, def_val);
}

static inline uint8_t mipc_sys_hba_ctrl_cnf_get_ipv6_hop_limit(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_IPV6_HOP_LIMIT, def_val);
}

static inline uint8_t mipc_sys_hba_ctrl_cnf_get_ipv6_tclass(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_IPV6_TCLASS, def_val);
}

static inline uint8_t mipc_sys_hba_ctrl_cnf_get_tcp_rcv_wscale(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_TCP_RCV_WSCALE, def_val);
}

static inline uint8_t mipc_sys_hba_ctrl_cnf_get_tcp_snd_wscale(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_TCP_SND_WSCALE, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_tcp_wscale_ok(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_TCP_WSCALE_OK, def_val);
}

static inline uint16_t mipc_sys_hba_ctrl_cnf_get_tcp_snd_window(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_TCP_SND_WINDOW, def_val);
}

static inline uint16_t mipc_sys_hba_ctrl_cnf_get_tcp_rcv_window(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_TCP_RCV_WINDOW, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_tcp_snd_nxt_seq(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_TCP_SND_NXT_SEQ, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_tcp_rcv_nxt_seq(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_TCP_RCV_NXT_SEQ, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_send_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_SEND_COUNT, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_recv_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_RECV_COUNT, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_short_rrc_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_SHORT_RRC_COUNT, def_val);
}

static inline uint32_t mipc_sys_hba_ctrl_cnf_get_hitchhike_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_HITCHHIKE_COUNT, def_val);
}

static inline uint16_t mipc_sys_hba_ctrl_cnf_get_current_cycle(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_CTRL_CNF_T_CURRENT_CYCLE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_send_now_req_add_proxy_key(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_SEND_NOW_REQ_T_PROXY_KEY, len, (const void *)value);
}

static inline char * mipc_sys_hba_send_now_cnf_get_proxy_key(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_HBA_SEND_NOW_CNF_T_PROXY_KEY, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_proxy_key(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_PROXY_KEY, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_ipv4_tos(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_IPV4_TOS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_ipv4_ttl(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_IPV4_TTL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_ipv4_id(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_IPV4_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_ipv6_flow_label(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_IPV6_FLOW_LABEL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_ipv6_hop_limit(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_IPV6_HOP_LIMIT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_ipv6_tclass(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_IPV6_TCLASS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_tcp_rcv_wscale(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_TCP_RCV_WSCALE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_tcp_snd_wscale(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_TCP_SND_WSCALE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_tcp_wscale_ok(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_TCP_WSCALE_OK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_tcp_snd_window(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_TCP_SND_WINDOW, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_tcp_rcv_window(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_TCP_RCV_WINDOW, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_tcp_snd_nxt_seq(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_TCP_SND_NXT_SEQ, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_hba_resume_req_add_tcp_rcv_nxt_seq(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_HBA_RESUME_REQ_T_TCP_RCV_NXT_SEQ, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_sys_hba_resume_cnf_get_proxy_key(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_HBA_RESUME_CNF_T_PROXY_KEY, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_mia_start_scenario_req_add_scenario_metrics(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_mia_metrics_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_MIA_START_SCENARIO_REQ_T_SCENARIO_METRICS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_mia_stop_scenario_req_add_scenario_metrics(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_mia_metrics_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_MIA_STOP_SCENARIO_REQ_T_SCENARIO_METRICS, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_mia_update_metrics_req_add_scenario_metrics(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_mia_metrics_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_MIA_UPDATE_METRICS_REQ_T_SCENARIO_METRICS, len, (const void *)value);
}

static inline mipc_sys_log_flush_trigger_result_const_enum mipc_sys_trigger_md_log_flush_cnf_get_trigger_result(mipc_msg_t *msg_ptr, mipc_sys_log_flush_trigger_result_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_log_flush_trigger_result_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_TRIGGER_MD_LOG_FLUSH_CNF_T_TRIGGER_RESULT, def_val);
}

static inline uint32_t mipc_sys_get_md_log_flush_status_cnf_get_status(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_MD_LOG_FLUSH_STATUS_CNF_T_STATUS, def_val);
}

static inline uint32_t mipc_sys_get_gnss_coclock_nvdata_cnf_get_c0(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF_T_C0, def_val);
}

static inline uint32_t mipc_sys_get_gnss_coclock_nvdata_cnf_get_c1(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF_T_C1, def_val);
}

static inline uint32_t mipc_sys_get_gnss_coclock_nvdata_cnf_get_capid_temp(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF_T_CAPID_TEMP, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_sbp_info_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_sbp_set_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_SBP_INFO_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_sbp_info_req_add_feature_int(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_SBP_INFO_REQ_T_FEATURE_INT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_sbp_info_req_add_feature_str(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_SBP_INFO_REQ_T_FEATURE_STR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_sbp_info_req_add_data(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_SBP_INFO_REQ_T_DATA, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_sbp_info_req_add_param(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_SBP_INFO_REQ_T_PARAM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sys_set_sbp_info_cnf_get_data(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_SET_SBP_INFO_CNF_T_DATA, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_get_sbp_info_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_sbp_get_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_GET_SBP_INFO_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_sbp_info_req_add_feature_int(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_SBP_INFO_REQ_T_FEATURE_INT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_sbp_info_req_add_feature_str(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_GET_SBP_INFO_REQ_T_FEATURE_STR, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_get_sbp_info_req_add_param(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_GET_SBP_INFO_REQ_T_PARAM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sys_get_sbp_info_cnf_get_data(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_SBP_INFO_CNF_T_DATA, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_recording_tx_cond_iq_dump_req_add_capture_mode(mipc_msg_t *msg_ptr, enum mipc_sys_set_recording_tx_cond_iq_dump_req_capture_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_CAPTURE_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_recording_tx_cond_iq_dump_req_add_capture_condition(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_CAPTURE_CONDITION, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_recording_tx_cond_iq_dump_req_add_param3(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_PARAM3, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_recording_tx_cond_iq_dump_req_add_param4(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_PARAM4, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_sys_wakeup_reason_const_enum mipc_sys_get_wakeup_reason_cnf_get_wakeup_reason(mipc_msg_t *msg_ptr, mipc_sys_wakeup_reason_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_wakeup_reason_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_WAKEUP_REASON_CNF_T_WAKEUP_REASON, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_sleep_timer_req_add_value(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_SLEEP_TIMER_REQ_T_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_sleep_timer_cnf_get_value(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_SLEEP_TIMER_CNF_T_VALUE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_sleep_mode_req_add_sleep_mode(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_SLEEP_MODE_REQ_T_SLEEP_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_send_consys_wifi_req_add_payload_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_SEND_CONSYS_WIFI_REQ_T_PAYLOAD_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_send_consys_wifi_req_add_payload(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SEND_CONSYS_WIFI_REQ_T_PAYLOAD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_send_consys_bt_req_add_payload_len(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_SEND_CONSYS_BT_REQ_T_PAYLOAD_LEN, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_send_consys_bt_req_add_payload(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SEND_CONSYS_BT_REQ_T_PAYLOAD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_md_log_filter_req_add_raw_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_MD_LOG_FILTER_REQ_T_RAW_DATA, len, (const void *)value);
}

static inline mipc_sys_md_log_result_const_enum mipc_sys_set_md_log_filter_cnf_get_setting_result(mipc_msg_t *msg_ptr, mipc_sys_md_log_result_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_md_log_result_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_SET_MD_LOG_FILTER_CNF_T_SETTING_RESULT, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_spv_control_req_add_control_type(mipc_msg_t *msg_ptr, enum mipc_sys_spv_control_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONTROL_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_spv_control_req_add_control(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONTROL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_spv_control_req_add_config1(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONFIG1, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_spv_control_req_add_config2(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONFIG2, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_spv_control_req_add_config3(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONFIG3, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_frame_sync_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_FRAME_SYNC_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_tx_pause_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_TX_PAUSE_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_mws_tx_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_MWS_TX_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_rx_protect_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_RX_PROTECT_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_inact_duration_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_INACT_DURATION_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_pwr_backoff_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_PWR_BACKOFF_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_rrc_state_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_RRC_STATE_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_tx_port_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_TX_PORT_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_resend_support(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_RESEND_SUPPORT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_idc_cfg_req_add_gpio_cfg(mipc_msg_t *msg_ptr, uint16_t len, mipc_sys_idc_gpio_cfg_struct4 *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_IDC_CFG_REQ_T_GPIO_CFG, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_forward_at_req_add_atcmd(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_FORWARD_AT_REQ_T_ATCMD, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_forward_at_req_add_forward_type(mipc_msg_t *msg_ptr, enum mipc_sys_forward_at_req_forward_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_FORWARD_AT_REQ_T_FORWARD_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline char * mipc_sys_forward_at_cnf_get_atcmd(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_FORWARD_AT_CNF_T_ATCMD, val_len_ptr);
}

static inline mipc_sys_factory_mode_const_enum mipc_sys_get_factory_mode_cnf_get_mode(mipc_msg_t *msg_ptr, mipc_sys_factory_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_factory_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_FACTORY_MODE_CNF_T_MODE, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_set_factory_mode_req_add_mode(mipc_msg_t *msg_ptr, enum mipc_sys_factory_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_FACTORY_MODE_REQ_T_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_coex_uart_test_mode_req_add_op(mipc_msg_t *msg_ptr, enum mipc_sys_coex_uart_mode_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_COEX_UART_TEST_MODE_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_write_coex_uart_req_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_WRITE_COEX_UART_REQ_T_DATA, len, (const void *)value);
}

static inline void * mipc_sys_read_coex_uart_cnf_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_READ_COEX_UART_CNF_T_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_coex_uart_loopback_mode_req_add_op(mipc_msg_t *msg_ptr, enum mipc_sys_coex_uart_mode_op_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_COEX_UART_LOOPBACK_MODE_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_lowv_actuator_req_add_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_LOWV_ACTUATOR_REQ_T_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_lowv_actuator_req_add_state(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_LOWV_ACTUATOR_REQ_T_STATE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_lowv_actuator_req_add_get_actuator_num(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_GET_LOWV_ACTUATOR_REQ_T_GET_ACTUATOR_NUM, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_get_lowv_actuator_req_add_actuator_id(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_GET_LOWV_ACTUATOR_REQ_T_ACTUATOR_ID, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_lowv_actuator_cnf_get_actuator_num(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_LOWV_ACTUATOR_CNF_T_ACTUATOR_NUM, def_val);
}

static inline mipc_sys_thermal_actuator_state_info_struct4* mipc_sys_get_lowv_actuator_cnf_get_actuator_state_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_actuator_state_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_LOWV_ACTUATOR_CNF_T_ACTUATOR_STATE_INFO, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_mspm_session_req_add_type(mipc_msg_t *msg_ptr, enum mipc_mspm_session_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_MSPM_SESSION_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mspm_session_req_add_procedure(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_MSPM_SESSION_REQ_T_PROCEDURE, len, (const void *)value);
}

static inline mipc_msg_api_result_enum mipc_sys_set_mspm_session_req_add_block_mode(mipc_msg_t *msg_ptr, enum mipc_mspm_block_mode_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_MSPM_SESSION_REQ_T_BLOCK_MODE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mspm_session_req_add_wait_time(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MSPM_SESSION_REQ_T_WAIT_TIME, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_errc_offset_req_add_valid_flag(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_ERRC_OFFSET_REQ_T_VALID_FLAG, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_errc_offset_req_add_nonnbr_nonoffset(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_SET_ERRC_OFFSET_REQ_T_NONNBR_NONOFFSET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_errc_offset_req_add_nbr_offset(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_SET_ERRC_OFFSET_REQ_T_NBR_OFFSET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_errc_offset_req_add_nonnbr_offset(mipc_msg_t *msg_ptr, uint16_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint16(msg_ptr, MIPC_SYS_SET_ERRC_OFFSET_REQ_T_NONNBR_OFFSET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_control_type(mipc_msg_t *msg_ptr, enum mipc_sys_mddbg_control_type_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_CONTROL_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_control(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_CONTROL, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_pool_index(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_POOL_INDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_start_count(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_START_COUNT, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_scan_buffer_no(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_SCAN_BUFFER_NO, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_enable_set(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_ENABLE_SET, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_timer_value(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_TIMER_VALUE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_vpeindex(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_VPEINDEX, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_selectwp(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_SELECTWP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_wp_addr(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_WP_ADDR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_selectbp(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_SELECTBP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_bp_addr(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_BP_ADDR, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_addr_mask(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_ADDR_MASK, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_set_mddbg_control_req_add_type(mipc_msg_t *msg_ptr, uint32_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_TYPE, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint32_t mipc_sys_get_tcxo_info_cnf_get_tcxo_off_counter(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TCXO_INFO_CNF_T_TCXO_OFF_COUNTER, def_val);
}

static inline uint32_t mipc_sys_get_tcxo_info_cnf_get_tcxo_off_duration(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_TCXO_INFO_CNF_T_TCXO_OFF_DURATION, def_val);
}

static inline mipc_msg_api_result_enum mipc_sys_trigger_chip_diagnosis_req_add_op(mipc_msg_t *msg_ptr, enum mipc_sys_chip_diagnosis_operation_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint32(msg_ptr, MIPC_SYS_TRIGGER_CHIP_DIAGNOSIS_REQ_T_OP, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_trigger_chip_diagnosis_req_add_dvfs_switch(mipc_msg_t *msg_ptr, enum mipc_boolean_const_enum value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_TRIGGER_CHIP_DIAGNOSIS_REQ_T_DVFS_SWITCH, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline mipc_msg_api_result_enum mipc_sys_send_app_event_req_add_voip_status(mipc_msg_t *msg_ptr, uint8_t value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    mipc_msg_add_tlv_uint8(msg_ptr, MIPC_SYS_SEND_APP_EVENT_REQ_T_VOIP_STATUS, value);
    return MIPC_MSG_API_RESULT_SUCCESS;
}

static inline uint8_t mipc_sys_get_primary_imei_info_cnf_get_primary_slot_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF_T_PRIMARY_SLOT_ID, def_val);
}

static inline char * mipc_sys_get_primary_imei_info_cnf_get_primary_imei(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF_T_PRIMARY_IMEI, val_len_ptr);
}

static inline char * mipc_sys_get_primary_imei_info_cnf_get_imei_svn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF_T_IMEI_SVN, val_len_ptr);
}

static inline char * mipc_sys_at_ind_get_atcmd(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_AT_IND_T_ATCMD, val_len_ptr);
}

static inline int32_t mipc_sys_thermal_sensor_ind_get_temperature(mipc_msg_t *msg_ptr, int32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (int32_t)mipc_msg_get_val_int32(msg_ptr, MIPC_SYS_THERMAL_SENSOR_IND_T_TEMPERATURE, def_val);
}

static inline mipc_sys_thermal_sensor_config_struct4* mipc_sys_thermal_sensor_ind_get_threshold(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_config_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_THERMAL_SENSOR_IND_T_THRESHOLD, val_len_ptr);
}

static inline uint8_t mipc_sys_thermal_sensor_ind_get_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_THERMAL_SENSOR_IND_T_INFO_COUNT, def_val);
}

static inline mipc_sys_thermal_sensor_info_struct4* mipc_sys_thermal_sensor_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_THERMAL_SENSOR_IND_T_INFO, val_len_ptr);
}

static inline mipc_sys_thermal_sensor_config_e_struct4* mipc_sys_thermal_sensor_ind_get_config_e(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_config_e_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_THERMAL_SENSOR_IND_T_CONFIG_E, val_len_ptr);
}

static inline mipc_sys_thermal_sensor_info_e_struct4* mipc_sys_thermal_sensor_ind_get_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_info_e_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_THERMAL_SENSOR_IND_T_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline mipc_sys_thermal_sensor_info_e_struct4* mipc_sys_thermal_sensor_ind_get_info_e(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_thermal_sensor_info_e_struct4*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_THERMAL_SENSOR_IND_T_INFO_E, index, val_len_ptr);
}

static inline mipc_sys_config_change_reason_const_enum mipc_sys_config_ind_get_reason(mipc_msg_t *msg_ptr, mipc_sys_config_change_reason_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_config_change_reason_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONFIG_IND_T_REASON, def_val);
}

static inline mipc_sys_config_class_const_enum mipc_sys_config_ind_get_class(mipc_msg_t *msg_ptr, mipc_sys_config_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_config_class_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONFIG_IND_T_CLASS, def_val);
}

static inline uint8_t mipc_sys_adpclk_ind_get_freq_info_count(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_ADPCLK_IND_T_FREQ_INFO_COUNT, def_val);
}

static inline mipc_sys_adpclk_freq_info_struct8* mipc_sys_adpclk_ind_get_freq_info_list(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_adpclk_freq_info_struct8*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_ADPCLK_IND_T_FREQ_INFO_LIST, val_len_ptr);
}

static inline mipc_sys_adpclk_freq_info_struct8* mipc_sys_adpclk_ind_get_freq_info_tlv_array(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr, uint32_t index)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_sys_adpclk_freq_info_struct8*)mipc_msg_get_idx_ptr(msg_ptr, MIPC_SYS_ADPCLK_IND_T_FREQ_INFO_TLV_ARRAY, index, val_len_ptr);
}

static inline uint8_t mipc_sys_mcf_ind_get_type(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_IND_T_TYPE, def_val);
}

static inline uint8_t mipc_sys_mcf_ind_get_result(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MCF_IND_T_RESULT, def_val);
}

static inline uint32_t mipc_sys_sbp_ind_get_sbp_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_SBP_IND_T_SBP_ID, def_val);
}

static inline uint32_t mipc_sys_sbp_ind_get_sim_sbp_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_SBP_IND_T_SIM_SBP_ID, def_val);
}

static inline uint32_t mipc_sys_el2_ip_ul_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_EL2_IP_UL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_el2_ip_dl_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_EL2_IP_DL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_el2_mac_ul_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_EL2_MAC_UL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_el2_mac_dl_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_EL2_MAC_DL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_el2_pdcp_ul_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_EL2_PDCP_UL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_el2_pdcp_dl_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_EL2_PDCP_DL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_nl2_mac_ul_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_NL2_MAC_UL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_nl2_mac_dl_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_NL2_MAC_DL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_nl2_pdcp_ul_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_NL2_PDCP_UL_IND_T_TX_BPS, def_val);
}

static inline uint32_t mipc_sys_nl2_pdcp_dl_ind_get_tx_bps(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_NL2_PDCP_DL_IND_T_TX_BPS, def_val);
}

static inline mipc_sys_account_id_const_enum mipc_sys_geo_location_ind_get_account_id(mipc_msg_t *msg_ptr, mipc_sys_account_id_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_account_id_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_ACCOUNT_ID, def_val);
}

static inline mipc_boolean_const_enum mipc_sys_geo_location_ind_get_broadcast_flag(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_BROADCAST_FLAG, def_val);
}

static inline char * mipc_sys_geo_location_ind_get_latitude(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_LATITUDE, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_longitude(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_LONGITUDE, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_accuracy(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_ACCURACY, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_method(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_METHOD, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_city(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_CITY, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_state(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_STATE, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_zip(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_ZIP, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_country_code(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_COUNTRY_CODE, val_len_ptr);
}

static inline char * mipc_sys_geo_location_ind_get_ue_wlan_mac(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_UE_WLAN_MAC, val_len_ptr);
}

static inline uint32_t mipc_sys_geo_location_ind_get_confidence(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GEO_LOCATION_IND_T_CONFIDENCE, def_val);
}

static inline mipc_md_init_id_const_enum mipc_sys_md_init_ind_get_init_id(mipc_msg_t *msg_ptr, mipc_md_init_id_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_md_init_id_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_MD_INIT_IND_T_INIT_ID, def_val);
}

static inline char * mipc_sys_warning_ind_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_WARNING_IND_T_INFO, val_len_ptr);
}

static inline uint8_t mipc_sys_nv_sig_err_ind_get_error_code(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_NV_SIG_ERR_IND_T_ERROR_CODE, def_val);
}

static inline uint8_t mipc_sys_vodata_statistics_ind_get_sim_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_VODATA_STATISTICS_IND_T_SIM_ID, def_val);
}

static inline uint32_t mipc_sys_vodata_statistics_ind_get_tx_bytes(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_VODATA_STATISTICS_IND_T_TX_BYTES, def_val);
}

static inline uint32_t mipc_sys_vodata_statistics_ind_get_rx_bytes(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_VODATA_STATISTICS_IND_T_RX_BYTES, def_val);
}

static inline uint32_t mipc_sys_vodata_statistics_ind_get_tx_pkt(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_VODATA_STATISTICS_IND_T_TX_PKT, def_val);
}

static inline uint32_t mipc_sys_vodata_statistics_ind_get_rx_pkt(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_VODATA_STATISTICS_IND_T_RX_PKT, def_val);
}

static inline mipc_boolean_const_enum mipc_sys_thermal_actuator_ind_get_ims_only_ind(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_THERMAL_ACTUATOR_IND_T_IMS_ONLY_IND, def_val);
}

static inline mipc_boolean_const_enum mipc_sys_thermal_actuator_ind_get_flight_mode_ind(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_THERMAL_ACTUATOR_IND_T_FLIGHT_MODE_IND, def_val);
}

static inline mipc_boolean_const_enum mipc_sys_thermal_actuator_ind_get_charge_ind(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_THERMAL_ACTUATOR_IND_T_CHARGE_IND, def_val);
}

static inline uint32_t mipc_sys_dmf_urc_ind_get_category_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_DMF_URC_IND_T_CATEGORY_ID, def_val);
}

static inline void * mipc_sys_dmf_urc_ind_get_rawdata(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_DMF_URC_IND_T_RAWDATA, val_len_ptr);
}

static inline char * mipc_sys_dmf_urc_ind_get_atstring(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_DMF_URC_IND_T_ATSTRING, val_len_ptr);
}

static inline void * mipc_sys_meta_control_ind_get_systrace(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_META_CONTROL_IND_T_SYSTRACE, val_len_ptr);
}

static inline char * mipc_sys_hba_timeout_ind_get_proxy_key(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_PROXY_KEY, val_len_ptr);
}

static inline uint8_t mipc_sys_hba_timeout_ind_get_ipv4_tos(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_IPV4_TOS, def_val);
}

static inline uint8_t mipc_sys_hba_timeout_ind_get_ipv4_ttl(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_IPV4_TTL, def_val);
}

static inline uint16_t mipc_sys_hba_timeout_ind_get_ipv4_id(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_IPV4_ID, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_ipv6_flow_label(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_IPV6_FLOW_LABEL, def_val);
}

static inline uint8_t mipc_sys_hba_timeout_ind_get_ipv6_hop_limit(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_IPV6_HOP_LIMIT, def_val);
}

static inline uint8_t mipc_sys_hba_timeout_ind_get_ipv6_tclass(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_IPV6_TCLASS, def_val);
}

static inline uint8_t mipc_sys_hba_timeout_ind_get_tcp_rcv_wscale(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_RCV_WSCALE, def_val);
}

static inline uint8_t mipc_sys_hba_timeout_ind_get_tcp_snd_wscale(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_SND_WSCALE, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_tcp_wscale_ok(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_WSCALE_OK, def_val);
}

static inline uint16_t mipc_sys_hba_timeout_ind_get_tcp_snd_window(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_SND_WINDOW, def_val);
}

static inline uint16_t mipc_sys_hba_timeout_ind_get_tcp_rcv_window(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_RCV_WINDOW, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_tcp_snd_nxt_seq(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_SND_NXT_SEQ, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_tcp_rcv_nxt_seq(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_RCV_NXT_SEQ, def_val);
}

static inline uint16_t mipc_sys_hba_timeout_ind_get_current_cycle(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_CURRENT_CYCLE, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_send_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_SEND_COUNT, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_recv_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_RECV_COUNT, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_short_rrc_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_SHORT_RRC_COUNT, def_val);
}

static inline uint32_t mipc_sys_hba_timeout_ind_get_hitchhike_count(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_HBA_TIMEOUT_IND_T_HITCHHIKE_COUNT, def_val);
}

static inline uint8_t mipc_sys_hba_hw_filter_src_state_ind_get_hw_filter_src_state(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_HBA_HW_FILTER_SRC_STATE_IND_T_HW_FILTER_SRC_STATE, def_val);
}

static inline mipc_boolean_const_enum mipc_sys_timer_expire_ind_get_timer_expire(mipc_msg_t *msg_ptr, mipc_boolean_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_boolean_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_TIMER_EXPIRE_IND_T_TIMER_EXPIRE, def_val);
}

static inline uint16_t mipc_sys_idc_frame_cfg_ind_get_frame_duration(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_IDC_FRAME_CFG_IND_T_FRAME_DURATION, def_val);
}

static inline uint16_t mipc_sys_idc_frame_cfg_ind_get_fs_offset(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_IDC_FRAME_CFG_IND_T_FS_OFFSET, def_val);
}

static inline uint16_t mipc_sys_idc_frame_cfg_ind_get_fs_jitter(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_IDC_FRAME_CFG_IND_T_FS_JITTER, def_val);
}

static inline uint16_t mipc_sys_idc_frame_cfg_ind_get_period_duration_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SYS_IDC_FRAME_CFG_IND_T_PERIOD_DURATION_TLV_ARRAY, def_val, index);
}

static inline mipc_sys_idc_period_type_const_enum mipc_sys_idc_frame_cfg_ind_get_period_type_tlv_array(mipc_msg_t *msg_ptr, mipc_sys_idc_period_type_const_enum def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_idc_period_type_const_enum)mipc_msg_get_idx_uint8(msg_ptr, MIPC_SYS_IDC_FRAME_CFG_IND_T_PERIOD_TYPE_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_sys_idc_scan_freq_ind_get_scan_freq_low_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SYS_IDC_SCAN_FREQ_IND_T_SCAN_FREQ_LOW_TLV_ARRAY, def_val, index);
}

static inline uint16_t mipc_sys_idc_scan_freq_ind_get_scan_freq_high_tlv_array(mipc_msg_t *msg_ptr, uint16_t def_val, uint32_t index)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_idx_uint16(msg_ptr, MIPC_SYS_IDC_SCAN_FREQ_IND_T_SCAN_FREQ_HIGH_TLV_ARRAY, def_val, index);
}

static inline uint8_t mipc_sys_idc_channel_param_ind_get_channel_enable(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_CHANNEL_ENABLE, def_val);
}

static inline uint16_t mipc_sys_idc_channel_param_ind_get_rx_center_freq(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_RX_CENTER_FREQ, def_val);
}

static inline uint16_t mipc_sys_idc_channel_param_ind_get_tx_center_freq(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_TX_CENTER_FREQ, def_val);
}

static inline uint16_t mipc_sys_idc_channel_param_ind_get_rx_channel_bw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_RX_CHANNEL_BW, def_val);
}

static inline uint16_t mipc_sys_idc_channel_param_ind_get_tx_channel_bw(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_TX_CHANNEL_BW, def_val);
}

static inline mipc_sys_idc_channel_type_const_enum mipc_sys_idc_channel_param_ind_get_channel_type(mipc_msg_t *msg_ptr, mipc_sys_idc_channel_type_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_idc_channel_type_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_CHANNEL_TYPE, def_val);
}

static inline void * mipc_sys_dmf_em_icd_info_ind_get_dmf_rawdata(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_DMF_EM_ICD_INFO_IND_T_DMF_RAWDATA, val_len_ptr);
}

static inline char * mipc_sys_forward_at_ind_get_atcmd(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_FORWARD_AT_IND_T_ATCMD, val_len_ptr);
}

static inline uint8_t mipc_sys_primary_imei_switch_ind_get_primary_slot_id(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_PRIMARY_IMEI_SWITCH_IND_T_PRIMARY_SLOT_ID, def_val);
}

static inline char * mipc_sys_primary_imei_switch_ind_get_primary_imei(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_PRIMARY_IMEI_SWITCH_IND_T_PRIMARY_IMEI, val_len_ptr);
}

static inline char * mipc_sys_primary_imei_switch_ind_get_imei_svn(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_PRIMARY_IMEI_SWITCH_IND_T_IMEI_SVN, val_len_ptr);
}

static inline mipc_sys_reboot_mode_const_enum mipc_sys_reboot_cmd_get_mode(mipc_msg_t *msg_ptr, mipc_sys_reboot_mode_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_reboot_mode_const_enum)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_REBOOT_CMD_T_MODE, def_val);
}

static inline uint32_t mipc_sys_reboot_cmd_get_timeout(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_REBOOT_CMD_T_TIMEOUT, def_val);
}

static inline uint32_t mipc_sys_reboot_cmd_get_sbp_id(mipc_msg_t *msg_ptr, uint32_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint32_t)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_REBOOT_CMD_T_SBP_ID, def_val);
}

static inline mipc_sys_config_class_const_enum mipc_sys_set_config_dipc_cmd_get_class(mipc_msg_t *msg_ptr, mipc_sys_config_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_config_class_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_SET_CONFIG_DIPC_CMD_T_CLASS, def_val);
}

static inline char * mipc_sys_set_config_dipc_cmd_get_type(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SET_CONFIG_DIPC_CMD_T_TYPE, val_len_ptr);
}

static inline void * mipc_sys_set_config_dipc_cmd_get_data(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SET_CONFIG_DIPC_CMD_T_DATA, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_set_config_dipc_rsp_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_SET_CONFIG_DIPC_RSP_T_DATA, len, (const void *)value);
}

static inline mipc_sys_config_class_const_enum mipc_sys_config_needed_to_update_cmd_get_class(mipc_msg_t *msg_ptr, mipc_sys_config_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_config_class_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_CMD_T_CLASS, def_val);
}

static inline char * mipc_sys_config_needed_to_update_cmd_get_type(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_CMD_T_TYPE, val_len_ptr);
}

static inline mipc_sys_config_class_const_enum mipc_sys_get_config_dipc_cmd_get_class(mipc_msg_t *msg_ptr, mipc_sys_config_class_const_enum def_val)
{
    if (!msg_ptr) return def_val;
    return (mipc_sys_config_class_const_enum)mipc_msg_get_val_uint32(msg_ptr, MIPC_SYS_GET_CONFIG_DIPC_CMD_T_CLASS, def_val);
}

static inline char * mipc_sys_get_config_dipc_cmd_get_type(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_GET_CONFIG_DIPC_CMD_T_TYPE, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_get_config_dipc_rsp_add_data(mipc_msg_t *msg_ptr, uint16_t len, const void  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_GET_CONFIG_DIPC_RSP_T_DATA, len, (const void *)value);
}

static inline mipc_nw_nitz_info_struct4* mipc_sys_set_time_cmd_get_info(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (mipc_nw_nitz_info_struct4*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SET_TIME_CMD_T_INFO, val_len_ptr);
}

static inline uint8_t mipc_sys_set_time_cmd_get_tz_valid(mipc_msg_t *msg_ptr, uint8_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint8_t)mipc_msg_get_val_uint8(msg_ptr, MIPC_SYS_SET_TIME_CMD_T_TZ_VALID, def_val);
}

static inline uint16_t mipc_sys_send_consys_wifi_cmd_get_payload_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_SEND_CONSYS_WIFI_CMD_T_PAYLOAD_LEN, def_val);
}

static inline void * mipc_sys_send_consys_wifi_cmd_get_payload(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SEND_CONSYS_WIFI_CMD_T_PAYLOAD, val_len_ptr);
}

static inline uint16_t mipc_sys_send_consys_bt_cmd_get_payload_len(mipc_msg_t *msg_ptr, uint16_t def_val)
{
    if (!msg_ptr) return def_val;
    return (uint16_t)mipc_msg_get_val_uint16(msg_ptr, MIPC_SYS_SEND_CONSYS_BT_CMD_T_PAYLOAD_LEN, def_val);
}

static inline void * mipc_sys_send_consys_bt_cmd_get_payload(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (void *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_SEND_CONSYS_BT_CMD_T_PAYLOAD, val_len_ptr);
}

static inline char * mipc_sys_at_cmd_get_atcmd(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_AT_CMD_T_ATCMD, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_at_rsp_add_atcmd(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_AT_RSP_T_ATCMD, len, (const void *)value);
}

static inline char * mipc_sys_forward_at_cmd_get_atcmd(mipc_msg_t *msg_ptr, uint16_t *val_len_ptr)
{
    if (!msg_ptr) { if (val_len_ptr) {*val_len_ptr = 0;} return 0;}
    return (char *)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_FORWARD_AT_CMD_T_ATCMD, val_len_ptr);
}

static inline mipc_msg_api_result_enum mipc_sys_forward_at_rsp_add_atcmd(mipc_msg_t *msg_ptr, uint16_t len, char  *value)
{
    if (!msg_ptr) return MIPC_MSG_API_RESULT_FAIL;
    if (!value) return MIPC_MSG_API_RESULT_FAIL;
    return mipc_msg_add_tlv(msg_ptr, MIPC_SYS_FORWARD_AT_RSP_T_ATCMD, len, (const void *)value);
}


#endif /* __MIPC_MSG_SYS_API_H__ */
