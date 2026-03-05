/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_dm.h 
  @brief device management API

  @detailes
  Quectel AG55x series module dm service.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly
  
  when     |  who      |    what, where, why
  -------- |  ---      |    ----------------------------------------------------------
  20200316 |  stan.li  |    Optimize the ql_dm_get_modem_state interface
  20191224 |  stan.li  |    Add radio on/off API
  20190625 |  stan.li  |    Created .
  
  Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

#ifndef __QL_DM_H__
#define __QL_DM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_dm_common.h"

/**
  @brief This function is callback function indicating air plane mode event.
  @param[in] air_plane_mode air plane mode
  @retval no value.
*/
typedef void (*ql_dm_air_plane_mode_ind_cb)(QL_DM_AIR_PLANE_MODE_TYPE_E air_plane_mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function initializes device management service.
  @note You must call this function before other functions can be used in this module.
  @return Whether the DM service was successfully intialized.
  @retval QL_ERR_OK The device management service is initialized successfully.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_init(void);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Denitialize DM service.
  @return Whether the DM service was successfully deintialized.
  @retval QL_ERR_OK successful.
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get device software version.
  @param[out] soft_ver Return software version
  @param[in] soft_ver_len The length of soft_ver
  @return Whether to successfully get the software version
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_software_version(char *soft_ver, int soft_ver_len);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get modem state.
  @details QL_DM_MODEM_STATE_ONLINE,if modem starts normally.
  @details QL_DM_MODEM_STATE_OFFLINE,in modem starts abnormally.
  @details QL_DM_MODEM_STATE_UNKNOWN,unknown error.
  @param[out] modem_state The state of modem
  @return Whether to successfully get the modem state
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_modem_state(QL_DM_MODEM_STATE_TYPE_E *modem_state);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register modem state event.
  @param[in] cb_func modem state indication callback function
  @return Whether the modem state event was successfully registered.
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_modem_state_change_ind_cb(ql_dm_modem_state_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register modem offline reason event.
  @param[in] cb_func modem offline reason indication callback function
  @return Whether the modem offline reason event was successfully registered.
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_modem_offline_reason_ind_cb(ql_dm_modem_offline_reason_ind_cb cb_func);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get module temperature.
  @param[out] temperature The current temperature
  @return Whether to successfully get the temperature
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_temperature(float *temperature);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get module temperature.
  @param[in] index The index of thermal-zone
  @param[out] temperature The current temperature
  @return Whether to successfully get the temperature
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_temperature_ex(int index,float *temperature);
    
/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get device serial numbers.
  @param[out] p_info Pointer that point to ql_dm_device_serial_numbers_info_t
  @return Whether to successfully get the serial numbers
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_device_serial_numbers(ql_dm_device_serial_numbers_info_t *p_info);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get device firmware revision identification.
  @param[out] firmware_rev_id Return device firmware revision id
  @param[in] firmware_rev_id_len The length of firmware_rev_id
  @return Whether to successfully get the firmware revision id
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_device_firmware_rev_id(char *firmware_rev_id, int firmware_rev_id_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief get radio mode.
  @param[out] p_info Pointer that point to QL_DM_RADIO_MODE_TYPE_E
  @return Whether to successfully get the radio mode
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_radio_mode(QL_DM_RADIO_MODE_TYPE_E *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get air plane mode.
  @param[out] p_info Pointer that point to QL_DM_AIR_PLANE_MODE_TYPE_E
  @return Whether to successfully get the air plane mode
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_air_plane_mode(QL_DM_AIR_PLANE_MODE_TYPE_E *p_info);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set air plane mode.
  @param[in] air_plane_mode 1:ON, 2:OFF, others: unavailable
  @return Whether to successfully set the air plane mode
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_air_plane_mode(QL_DM_AIR_PLANE_MODE_TYPE_E air_plane_mode);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register air plane mode event.
  @param[in] cb_func Air plane mode indication callback function
  @return Whether the air plane mode event was successfully registered.
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_air_plane_mode_ind_cb(ql_dm_air_plane_mode_ind_cb cb_func);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get cpu occupancy.
  @param[out] cpu_occupancy The percentage of cpu occupancy
  @return Whether to successfully get the cpu occupancy
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_cpu_occupancy(float *cpu_occupancy);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get mem usage.
  @param[out] mem_use The percentage of mem usage
  @return Whether to successfully get the memory usage
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_mem_usage(float *mem_use);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get NV item value.
  @param[in] nv_item_name The NV item name that is either NV item id or NV item path
  @param[out] nv_item_value The NV value buf of nv_item_name
  param[in] nv_item_value_len The length of nv_item_value
  param[out] nv_len The real length of nv_item_name
  @return Whether to successfully get the NV value
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_nv_item_value(char *nv_item_name, unsigned char *nv_item_value, int nv_item_value_len, 
        int *nv_len);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set NV item value.
  @param[in] nv_item_name The NV item name that is either NV item id or NV item path
  @param[in] nv_item_value The NV value of nv_item_name
  @param[in] nv_item_value_len The length of nv_item_value
  param[out] nv_len The real length of nv_item_name
  @return Whether to successfully set the NV value
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_nv_item_value(char *nv_item_name, unsigned char *nv_item_value, int nv_item_value_len, 
        int *nv_len);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set radio on, its function is the same as at+cfun=1.
  @return Whether to successfully set the radio on
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_radio_on(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set radio off, its function is the same as at+cfun=0.
  @return Whether to successfully set the radio off
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_radio_off(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get modem mem and CPU utilization.
  @param[out] mem_use The percentage of modem utilization
  @return Whether to successfully get the modem utilization
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_modem_cpu_occupancy(float *cpu_occupancy);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get modem mem utilization.
  @param[out] mem_use The percentage of modem utilization
  @return Whether to successfully get the modem utilization
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_modem_mem_usage(float *mem_use);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get QOOS enable state
  @param[out] enable The enable state of QOOS
  @return Whether to successfully get the QOOS enable state
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_qoos_enable(QL_DM_QOOS_ENABLE_TYPE_E *enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set QOOS enable state
  @param[in] enable The enable state of QOOS
  @return Whether to successfully set the QOOS enable state
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_qoos_enable(QL_DM_QOOS_ENABLE_TYPE_E enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get QOOS configuration
  @param[out] config The configuration of QOOS
  @return Whether to successfully get the QOOS configuration
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_qoos_config(ql_dm_qoos_config_t *config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set QOOS configuration
  @param[in] config The configuration of QOOS
  @return Whether to successfully set the QOOS configuration
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_qoos_config(ql_dm_qoos_config_t config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get MSSR(Modem SubSysem Reset) level.
  @param[out] p_level The MSSR level
  @return Whether to successfully get the MSSR level
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_mssr_level(int *p_level);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set MSSR(Modem SubSysem Reset) level.
  @param[in] level The MSSR level
  @return Whether to successfully set the MSSR level
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_mssr_level(int level);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  bind subscription 
  @param[in] sub_type subscription type 
  @return Whether to successfully bind subscription.
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_bind_subscription(QL_DM_BIND_SUB_TYPE_E sub_type);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is QL_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return QL_ERR_OK - successful
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_service_error_cb(ql_dm_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get module the last time shutdown reason 
  @param[out] shutdown_reason the shutdown reason
  @return QL_ERR_OK - successful
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_shutdown_reason(QL_DM_SHUTDOWN_REASON_E *shutdown_reason);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get module this time bootup reason 
  @param[out] bootup_reason the bootup reason
  @return QL_ERR_OK - successful
  @retval Others Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_bootup_reason(QL_DM_BOOT_UP_REASON_E *bootup_reason);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set modem restart 
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_modem_restart(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set poweroff
  @param[in] poweroff mode
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_poweroff(QL_DM_POWEROFF_MODE_E mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get cpu list state
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_cpu_state(ql_dm_cpu_list_state_t *info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set cpu state
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_cpu_state(int cpu_index, int enable);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Clean QXDM log mask 
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_log_mask_clear(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief get capture log periodically
  @param[out] enable enable or disable periodical log
  @prarm[out] log_time indicating the capture log time, minual value 30s. unit is ms
  @param[out] interval_time indicating the interval time, minual value 1min. unit is ms
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_period_log_enable(int *enable, int *log_time, int *interval_time);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief set capture log periodically
  @param[in] enable enable or disable periodical log
  @prarm[in] log_time indicating the capture log time, minual value 30s. unit is ms
  @param[in] interval_time indicating the interval time, minual value 1min. unit is ms
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_period_log_enable(int enable, int log_time, int interval_time);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Set the callback function to notify the application layer to restart after detecting the recovery of ims apn loss.
  @param[in] cb Callback function 
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_set_mbn_recovery_cb(ql_dm_mbn_recovery_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get active antenna.
  @param[out] p_info Pointer that point to QL_DM_ANTENNA_E
  @return Whether to successfully get the air plane mode
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_get_active_antenna(QL_DM_ANTENNA_E *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief switch antenna.
  @param[in] antenna - 'QL_DM_ANTENNA_E' data
  @return Whether to successfully set the air plane mode
  @retval QL_ERR_OK successful
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval QL_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_dm_switch_antenna(QL_DM_ANTENNA_E antenna);

#ifdef __cplusplus
}
#endif


#endif

