/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_ms_nw.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_ms_nw相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_MS_NW_H__
#define __SV_MS_NW_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sv_type.h"
#include "sv_nw_common.h"

#define SV_MS_NW_DDS_TEMP_REASON_DSDA_IMPOSSIBLE           ((uint64_t)1)
#define SV_MS_NW_DDS_TEMP_REASON_DDS_INTERNET_UNAVAIL      ((uint64_t)1 << 1)
#define SV_MS_NW_DDS_TEMP_REASON_TX_SHARING                ((uint64_t)1 << 2)
#define SV_MS_NW_DDS_TEMP_REASON_BETTER_ACCESS_AVAIL       ((uint64_t)1 << 3)
#define SV_MS_NW_DDS_TEMP_REASON_CALL_STATUS_CHANGED       ((uint64_t)1 << 4)
#define SV_MS_NW_DDS_TEMP_REASON_ACTIVE_CALL_ON_DDS        ((uint64_t)1 << 5)
#define SV_MS_NW_DDS_TEMP_REASON_TEMP_SWITCH_DISABLED      ((uint64_t)1 << 6)
#define SV_MS_NW_DDS_TEMP_REASON_TEMP_DDS_REC_DISABLED     ((uint64_t)1 << 7)
#define SV_MS_NW_DDS_TEMP_REASON_NON_DDS_INTERNET_UNAVAIL  ((uint64_t)1 << 8)
#define SV_MS_NW_DDS_TEMP_REASON_DATA_OFF                  ((uint64_t)1 << 9)
#define SV_MS_NW_DDS_TEMP_REASON_CONDITION_NOT_MET         ((uint64_t)1 << 10)
#define SV_MS_NW_DDS_TEMP_REASON_EMERGENCY_CALL_ON_GOING   ((uint64_t)1 << 11)

#define SV_MS_NW_DDS_PERM_REASON_TEMP_CLEAN_UP            ((uint64_t)1)
#define SV_MS_NW_DDS_PERM_REASON_DATA_SETTING_OFF         ((uint64_t)1 << 1)
#define SV_MS_NW_DDS_PERM_REASON_PS_INVALID               ((uint64_t)1 << 2)
#define SV_MS_NW_DDS_PERM_REASON_INTERNET_NOT_AVAIL       ((uint64_t)1 << 3)
#define SV_MS_NW_DDS_PERM_REASON_DUAL_DATA_DSDS_SUBMODE   ((uint64_t)1 << 4)
#define SV_MS_NW_DDS_PERM_REASON_DUAL_DATA_IRAT_INIT      ((uint64_t)1 << 5)
#define SV_MS_NW_DDS_PERM_REASON_DUAL_DATA_TX_SHARING     ((uint64_t)1 << 6)

typedef enum {
  SV_MS_NW_MSIM_TYPE_DSDA = 0,
  SV_MS_NW_MSIM_TYPE_DSDS = 1,
  SV_MS_NW_MSIM_TYPE_UNKNOWN = 2
}SV_MS_NW_MSIM_TYPE_E;

typedef enum {
  SV_MS_NW_MSIM_TX_FULL_CONCURRENCY = 0,
  SV_MS_NW_MSIM_TX_SHARING = 1,
  SV_MS_NW_MSIM_TX_UNKNOWN = 2
}SV_MS_NW_MSIM_TX_TYPE_E;

typedef enum {
  SV_MS_NW_MSIM_OP_SUBSTATE_DSDS = 0,
  SV_MS_NW_MSIM_OP_SUBSTATE_DSDA = 1,
  SV_MS_NW_MSIM_OP_SUBSTATE_UNKNOWN = 2
}SV_MS_NW_MSIM_OP_SUBSTATE_E;

typedef enum {
  SV_MS_NW_MSIM_SUB_MODE_DSDS = 0,
  SV_MS_NW_MSIM_SUB_MODE_DSDA = 1,
  SV_MS_NW_MSIM_SUB_MODE_UNKNOWN = 2
}SV_MS_NW_MSIM_SUB_MODE_E;

typedef enum {
  SV_MS_NW_DDS_PRIMARY_SUBS = 1,
  SV_MS_NW_DDS_SECONDARY_SUBS = 2,
  SV_MS_NW_DDS_TERTIARY_SUBS = 3,
  SV_MS_NW_DDS_UNKNOWN_SUB = 4
}SV_MS_NW_DDS_SUB_E;

typedef enum {
  SV_MS_NW_DDS_RECOMMEND_SOURCE_UNSPECIFIED = 0,
  SV_MS_NW_DDS_RECOMMEND_SOURCE_ABSS = 1,
  SV_MS_NW_DDS_RECOMMEND_SOURCE_ABA = 2
}SV_MS_NW_DDS_RECOMMEND_SOURCE_E;

typedef enum {
  SV_MS_NW_DDS_SWITCH_PERMANENT = 0,
  SV_MS_NW_DDS_SWITCH_TEMPORARY = 1
}SV_MS_NW_DDS_SWITCH_TYPE_E;

typedef enum {
  SV_MS_NW_DDS_TEMP_RECOMMEND_LEVEL_REVOKE = 1,
  SV_MS_NW_DDS_TEMP_RECOMMEND_LEVEL_LOW = 2,
  SV_MS_NW_DDS_TEMP_RECOMMEND_LEVEL_HIGH = 3
}SV_MS_NW_DDS_TEMP_RECOMMEND_LEVEL_E;

typedef enum {
  SV_MS_NW_DDS_DYNAMIC_SWITCH_OFF = 0,
  SV_MS_NW_DDS_DYNAMIC_SWTICH_ON = 1,
  SV_MS_NW_DDS_DYNAMIC_SWITCH_RECOMMENDATION = 2
}SV_MS_NW_DDS_DYNAMIC_SWITCHS_E;

typedef enum {
  SV_MS_NW_DDS_DYNAMIC_RECOMMENDATION_THROUGHPUT_BASED = 1,
  SV_MS_NW_DDS_DYNAMIC_RECOMMENDATION_LATENCY_BASED = 2
}SV_MS_NW_DDS_DYNAMIC_RECOMMENDATION_TYPE_E;

typedef enum {
  SV_MS_NW_DDS_DYNAMIC_RECOMMENDATION_START = 1,
  SV_MS_NW_DDS_DYNAMIC_RECOMMENDATION_STOP = 2
}SV_MS_NW_DDS_DYNAMIC_RECOMMENDATION_ACTION_E;

typedef enum {
  SV_MS_NW_DDS_SWITCH_ALLOWED = 0x00,
  SV_MS_NW_DDS_SWITCH_NOT_ALLOWED = 0x01,
  SV_MS_NW_DDS_SWITCH_FAILED = 0x02,
}SV_MS_NW_DDS_SWITCH_RESULT_E;

typedef struct
{
  uint8_t msim_sub_mode_valid;
  SV_MS_NW_MSIM_SUB_MODE_E msim_sub_mode;
  
  uint8_t msim_tx_type_valid;    
  SV_MS_NW_MSIM_TX_TYPE_E msim_tx_type;
  
  uint8_t op_substate_valid;
  SV_MS_NW_MSIM_OP_SUBSTATE_E op_substate;
}sv_ms_nw_msim_status_t;

typedef struct
{
  SV_MS_NW_DDS_SUB_E dds_sub;

  uint8_t source_valid;
  SV_MS_NW_DDS_RECOMMEND_SOURCE_E dds_source;

  uint8_t switch_type_valid; 
  SV_MS_NW_DDS_SWITCH_TYPE_E switch_type;
}sv_ms_nw_dds_switch_info_t;

typedef struct
{
  SV_MS_NW_DDS_SUB_E dds_sub;

  uint8_t switch_type_valid; 
  SV_MS_NW_DDS_SWITCH_TYPE_E switch_type;
}sv_ms_nw_dds_status_info_t;

typedef void (*sv_ms_nw_voice_reg_ind_cb)(int sim_id, sv_nw_reg_status_info_t *p_info);

typedef void (*sv_ms_nw_data_reg_ind_cb)(int sim_id, sv_nw_reg_status_info_t *p_info);

typedef void (*sv_ms_nw_signal_strength_ind_cb)(int sim_id, sv_nw_signal_strength_info_t *p_info, SV_NW_SIGNAL_STRENGTH_LEVEL_E level);

typedef void (*sv_ms_nw_cell_access_status_ind_cb)(int sim_id, SV_NW_CELL_ACCESS_STATE_TYPE_E status);

typedef void (*sv_ms_nw_nitz_time_update_ind_cb)(int sim_id, sv_nw_nitz_time_info_t *p_info);

typedef void (*sv_ms_nw_network_scan_async_cb)(int sim_id, int async_index, sv_nw_scan_result_list_info_t *p_info);

typedef void (*sv_ms_nw_wea_reg_ind_cb)(int sim_id, sv_nw_wea_alert_info_t *p_info);

typedef void (*sv_ms_nw_etws_reg_ind_cb)(int sim_id, sv_nw_etws_alert_info_t *p_info);

typedef void (*sv_ms_nw_ims_reg_status_ind_cb)(int sim_id, uint8_t ims_reg_status);

typedef void (*sv_ms_nw_rf_band_info_ind_cb)(int sim_id, SV_NW_BAND_INFO_E rf_band);

typedef void (*sv_ms_nw_lte_rach_fail_ind_cb)(int sim_id, sv_nw_lte_rach_fail_info_t *p_info);

typedef void (*sv_ms_nw_msim_status_ind_cb)(sv_ms_nw_msim_status_t* p_info);

typedef void (*sv_ms_nw_dds_switch_ind_cb)(SV_MS_NW_DDS_SWITCH_RESULT_E* dds_switch_result);

typedef void (*sv_ms_nw_dds_status_ind_cb)(sv_ms_nw_dds_status_info_t* dds_status_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Initialize NW service.
  @note You must call this function before other functions can be used in this module.
  @return Whether the NW service was successfully intialized.
  @retval SV_ERR_OK successful
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Deinitializes NW service.
  @return Whether the NW service was deinitialized successfully.
  @retval SV_ERR_OK successful.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief scan network status.
  @param[in] sim id number
  @param[out] async_index The index of request msg
  @param[in] async_cb The callback function of request msg
  @return Whether to successfully trigger the network scan operation
  @retval SV_ERR_OK  successful
  @retval SV_ERR_NOT_INIT  uninitialized
  @retval SV_ERR_SERVICE_NOT_READY  service is not ready
  @retval SV_ERR_INVALID_ARG  Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_network_scan(int sim_id, int *async_index, sv_ms_nw_network_scan_async_cb async_cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set power mode.
  @param[in] sim id number
  @param[in] lower_mode, defined by SV_NW_LOWER_POWER_MASK_XXX
  @return Whether to successfully set the power mode
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_power_mode(int sim_id, uint8_t lower_mode);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get power mode.
  @param[in] sim id number
  @param[out] lower_mode, defined by SV_NW_LOWER_POWER_MASK_XXX
  @return Whether to successfully set the power mode
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_power_mode(int sim_id, uint8_t *lower_mode);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set perferred NW mode and roaming indicator.
  @param[in] sim id number
  @param[in] p_info Pointer that point to sv_nw_pref_nwmode_roaming_info_t
  @return Whether to successfully set nwmode and roaming
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_pref_nwmode_roaming(int sim_id, sv_nw_pref_nwmode_roaming_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get perferred NW mode and roaming indicator.
  @param[in] sim id number
  @param[out] p_info Pointer that point to sv_nw_pref_nwmode_roaming_info_t 
  @return Whether to successfully get nwmode and roaming
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_pref_nwmode_roaming(int sim_id, sv_nw_pref_nwmode_roaming_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get mobile operator name.
  @param[in] sim id number
  @param[out] p_info Pointer that point to sv_nw_mobile_operator_name_info_t
  @return Whether to successfully get the mobile operator name
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_mobile_operator_name(int sim_id, sv_nw_mobile_operator_name_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get cell information.
  @param[in] sim id number
  @param[out] p_info Pointer that point to sv_nw_cell_info_t
  @return Whether to successfully get the cell information
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_cell_info(int sim_id, sv_nw_cell_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get voice registration status.
  @param[in] sim id number
  @param[out] p_info Pointer that point to sv_nw_reg_status_info_t
  @return Whether to successfully get the voice registration status
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_voice_reg_status(int sim_id, sv_nw_reg_status_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get data registration status.
  @param[in] sim id number
  @param[out] p_info Pointer that point to sv_nw_reg_status_info_t
  @return Whether to successfully get the data registration status
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_data_reg_status(int sim_id, sv_nw_reg_status_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get current signal strength.
  @param[in] sim id number
  @param[out] p_info Pointer that point to sv_nw_signal_strength_info_t
  @param[out] p_level: signal strength level
  @return Whether to successfully get the signal strength
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_signal_strength(int sim_id, sv_nw_signal_strength_info_t *p_info, SV_NW_SIGNAL_STRENGTH_LEVEL_E* p_level);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get current cell acccess status.
  @param[in] sim id number
  @param[out] p_info Pointer that point to SV_NW_CELL_ACCESS_STATE_TYPE_E
  @return Whether to successfully get the cell access status
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_cell_access_status(int sim_id, SV_NW_CELL_ACCESS_STATE_TYPE_E *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get network time.
  @param[in] sim id number
  @param[out] p_info Pointer that point to sv_nw_nitz_time_info_t 
  @return Whether to successfully get the network time
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_nitz_time_info(int sim_id, sv_nw_nitz_time_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register voice registration event.
  @param[in] sim id number
  @param[in] cb_func Voice registration indication callback function
  @return Whether the voice registration event was successfully registered.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
 */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_voice_reg_ind_cb(int sim_id, sv_ms_nw_voice_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register data registration event.
  @param[in] sim id number
  @param[in] cb_func Data registration indication callback function
  @return Whether the data registration event was successfully registered.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_data_reg_ind_cb(int sim_id, sv_ms_nw_data_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register signal strength event.
  @param[in] sim id number
  @param[in] cb_func Signal strength indication callback function
  @return Whether the signal strength event was successfully registered
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_signal_strength_ind_cb(int sim_id, sv_ms_nw_signal_strength_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register cell access status event.
  @param[in] sim id number
  @param[in] cb_func Cell access status indication callback function
  @return Whether the cell access status event was successfully registered
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_cell_access_status_ind_cb(int sim_id, sv_ms_nw_cell_access_status_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register network time event.
  @param[in] sim id number
  @param[in] cb_func nitz time update indication callback function
  @return Whether the network time event was successfully registered
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_nitz_time_update_ind_cb(int sim_id, sv_ms_nw_nitz_time_update_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register wea alert event.
  @param[in] sim id number
  @param[in] cb_func wea alert indication callback function
  @return Whether the network time event was successfully registered
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_wea_alert_ind_cb(int sim_id, sv_ms_nw_wea_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register etws alert event.
  @param[in] sim id number
  @param[in] cb_func etws alert indication callback function
  @return Whether the network time event was successfully registered
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_etws_alert_ind_cb(int sim_id, sv_ms_nw_etws_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register lte rach fail event.
  @pram[in] sim id number
  @param[in] cb_func lte rach fail indication callback function
  @return Whether the lte rach fail event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_lte_rach_fail_ind_cb(int sim_id, sv_ms_nw_lte_rach_fail_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set wea alert config.
  @param[in] sim id number
  @param[in] item Items to set.
  @param[in] p_info Pointer that point to sv_nw_wea_config_t.
  @return Whether to successfully set the wea config.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_wea_config(int sim_id, int item, sv_nw_wea_config_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets wea config.
  @param[in] sim id number
  @param[out] p_config wea config.
  @return Whether the wea config was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_wea_config(int sim_id, sv_nw_wea_config_t *p_config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set etws alert config.
  @param[in] sim id number
  @param[in] etws config.
  @return Whether to successfully set the etws config.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_etws_config(int sim_id, uint8_t enable_etws);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get etws alert config.
  @param[in] sim id number
  @param[out] p_enable_etws Pointer.
  @return Whether to successfully set the etws config.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_etws_config(int sim_id, uint8_t* p_enable_etws);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get high capability subscription.
  @param[out] sim id number
  @return Whether the high capability subscription was successfully obtained.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_high_cap_sub(int *sim_id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set the SIM card index that can uniquely register to the 5G network. 
  the main difference between high and non-high capability subscription is that high capability 
  subscription can register to 5G network while non-high capability subscription can only 
  register to LTE or GSM.
  @param[in] sim id number
  @return Whether to successfully set the high capability subscription.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_high_cap_sub(int sim_id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Disable NR5G, NR5G_SA and NR5G_NSA can be disabled individually or together.
  @param[in] sim id number
  @param[in] opt_mask Option mask value. Value:
  0                 - Do not disable NR5G
  SV_NW_NR5G_SO_SA  - Disable NR5G SA
  SV_NW_NR5G_SO_NSA - Disable NR5G NSA 
  @return Whether to successfully disable NR5G mode.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_disable_nr5g(int sim_id, uint16_t opt_mask);

/** 
  @brief Get ims configuration.
  @param[out] ims_enable_ptr, Value:
  0  - Dsiabled
  1  - Enabled
  @return Whether to successfully get ims configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_ims_config(int sim_id, uint8_t* ims_enable_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set ims configuration.
  @param[in] ims_enable, Value:
  0  - Disabled
  1  - Enabled
  @return Whether to successfully set ims configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_ims_config(int sim_id, uint8_t ims_enable);

/** 
  @brief Get ims configuration.
  @param[out] ims_config_ptr
  @return Whether to successfully get ims configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_ims_config_ex(int sim_id, sv_nw_ims_config_ex_t* ims_config_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set ims configuration.
  @param[in] ims_config
  @return Whether to successfully set ims configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_ims_config_ex(int sim_id, sv_nw_ims_config_ex_t ims_config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get ims registration status.
  @param[out] ims_reg_status_ptr, Value:
  0  - Unregistered
  1  - Registered
  @return Whether to successfully get ims registration status.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_ims_reg_status(int sim_id, uint8_t* ims_reg_status_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register ims registration status event.
  @param[in] cb_func ims registration status indication callback function
  @return Whether the ims registration status event was successfully registered
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_ims_reg_status_ind_cb(int sim_id, sv_ms_nw_ims_reg_status_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register rf band info event.
  @param[in] sim_id sim id number
  @param[in] cb_func rf band info indication callback function
  @return Whether the rf band info event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_rf_band_info_ind_cb(int sim_id, sv_ms_nw_rf_band_info_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Force module to receive message by 4 antennas under LTE mode.
  This interface will result in increased current consumption. 
  Only for debugging use,do not use for commercial version.
  @param[in] sim_id sim id number
  @param[in] dlm_4rx_enable dlm 4rx enable
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_dlm_4rx_enable(int sim_id, uint8_t dlm_4rx_enable);

/** 
  @brief Get endc configuration.
  @param[in] sim_id sim id number
  @param[out] get_endc_config_ptr
  @return Whether to successfully get endc configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_endc_config(int sim_id, sv_nw_get_endc_config_t* get_endc_config_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set endc configuration.
  @param[in] sim_id sim id number
  @param[in] set_endc_config
  @return Whether to successfully set endc configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_endc_config(int sim_id, sv_nw_set_endc_config_t set_endc_config);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Enables or disables NR5G deprioritization.
  @param[in] sim_id sim id number
  @param[in] deprioritize_nr5g_enable
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_deprioritize_nr5g_enable(int sim_id, uint8_t deprioritize_nr5g_enable);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief WWAN reset without interrupting CV2X stack.
  @param[in] sim_id
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_trigger_wwan_reset(int sim_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the DRX for the device.DRX interface takes effect only after the network injection is successful
  @param[in] sim_id sim id number
  @param[out] get_drx_ptr setting for the device. 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_drx(int sim_id, sv_nw_get_drx_t * get_drx_ptr);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Sets the DRX for the device.DRX interface takes effect only after the network injection is successful
  @param[in] sim_id sim id number
  @param[in] set_drx setting for the device. 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_drx(int sim_id, sv_nw_set_drx_t set_drx);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Gets the negotiated DRX for the device.DRX interface takes effect only after the network injection is successful
  @param[in] sim_id sim id number
  @param[out] get_negotiated_drx_ptr setting for the device. 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_negotiated_drx(int sim_id, sv_nw_get_negotiated_drx_t * get_negotiated_drx_ptr);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set multi sim type
  @param[in] msim_type multi sim type
  @return Whether to successfully set multi sim type
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_pref_msim_type(SV_MS_NW_MSIM_TYPE_E msim_type);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get multi sim status
  @param[out] p_status multi sim status information
  @return Whether to successfully get multi sim status
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_msim_status(sv_ms_nw_msim_status_t* p_status);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief register multi sim status event.
  @param[in] cb_func multi sim status indication callback function
  @return Whether the multi sim status event was successfully registered.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_msim_status_ind_cb(sv_ms_nw_msim_status_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief switch DDS
  @param[in] t_info params for switching DDS
  @return Whether to successfully switch DDS
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_switch_dds(sv_ms_nw_dds_switch_info_t t_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief register DDS switch event.
  @param[in] cb_func DDS switch indication callback function
  @return Whether the DDS switch event was successfully registered.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_dds_switch_ind_cb(sv_ms_nw_dds_switch_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get current DDS status
  @param[out] p_status current DDS status
  @return Whether to successfully get current DDS status
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_get_dds_status(sv_ms_nw_dds_status_info_t* p_status);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief register DDS status event.
  @param[in] cb_func DDS status indication callback function
  @return Whether the DDS status event was successfully registered.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_dds_status_ind_cb(sv_ms_nw_dds_status_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally,
  the callback function will be executed, and the error code is SV_ERR_ABORTED;
  @param[in] sim id number
  @param[in] cb  Callback function 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_nw_set_service_error_cb(sv_nw_service_error_cb_f cb);

#ifdef __cplusplus
}
#endif

#endif

