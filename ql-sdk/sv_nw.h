/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_nw.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_nw相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/

#ifndef __SV_NW_H__
#define __SV_NW_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sv_nw_common.h"

/**
  @brief This is the callback function for voice-dialing registration event.
  @param[in] p_info  Voice-dialing registration status.
  @retval None
*/
typedef void (*sv_nw_voice_reg_ind_cb)(sv_nw_reg_status_info_t *p_info);

/**
  @brief This is the callback function for data-dialing registration event.
  @param[in] p_info Data-dialing registration status.
  @retval None
*/
typedef void (*sv_nw_data_reg_ind_cb)(sv_nw_reg_status_info_t *p_info);

/**
  @brief This is the callback function for signal strength event.
  @param[in] p_info  Signal strength.
  @param[in] level  Signal strength level.
  @retval None
*/
typedef void (*sv_nw_signal_strength_ind_cb)(sv_nw_signal_strength_info_t *p_info, SV_NW_SIGNAL_STRENGTH_LEVEL_E level);

/**
  @brief This is the callback function for cell access state change event.
  @param[in] status  Current cell access state.
  @retval None
*/
typedef void (*sv_nw_cell_access_status_ind_cb)(SV_NW_CELL_ACCESS_STATE_TYPE_E status);

/**
  @brief This is the callback function for network time update event.
  @param[in]  p_info  Current network time. 
  @retval None
*/
typedef void (*sv_nw_nitz_time_update_ind_cb)(sv_nw_nitz_time_info_t *p_info);

/**
  @brief This is the callback function for asynchronous network scanning.
  @param[in]  index  Asynchronous operation index number.
  @param[in]  p_info  Network scanning result.
  @retval None
*/
typedef void (*sv_nw_network_scan_async_cb)(int async_index, sv_nw_scan_result_list_info_t *p_info);

/**
  @brief This is the callback function for WEA alert information.
  @param[in] p_info  WEA alert information.
  @retval None
*/
typedef void (*sv_nw_wea_reg_ind_cb)(sv_nw_wea_alert_info_t *p_info);

/**
  @brief This is the callback function for ETWS alert information.
  @param[in] p_info  ETWS alert information.
  @retval None
*/
typedef void (*sv_nw_etws_reg_ind_cb)(sv_nw_etws_alert_info_t *p_info);

/**
  @brief This is the callback function for IMS register.
  @param[in]  ims_reg_status  ims register state 
  @retval None
*/
typedef void (*sv_nw_ims_reg_status_ind_cb)(uint8_t ims_reg_status);

/**
  @brief This is the callback function for rf band info.
  @param[in]  rf_band current rf band info. 
  @retval None
*/
typedef void (*sv_nw_rf_band_info_ind_cb)(SV_NW_BAND_INFO_E rf_band);

/**
  @brief This is the callback function for service error event.
  @param[in] error  Service error code.
  @retval None
*/
typedef void (*sv_nw_service_error_cb_f)(int error);

/**
  @brief This is the callback function for LTE RACH fail.
  @param[in]  p_info  lte rach fail info 
  @retval None
*/
typedef void (*sv_nw_lte_rach_fail_ind_cb)(sv_nw_lte_rach_fail_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function initializes the network registration service.
  @note This function must be called prior to any other network information API functions. 
  @return Whether the NW service was successfully intialized.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function scans the current network. The operation takes long time. In order not to affect the normal use of other functions, this function is called asynchronously and the scanning result can be processed in the second parameter (function pointer sv_nw_network_scan_async_cb()) of this function. 
  @param[out] async_index Asynchronous operation index. This value identifies which asynchronous call resulting in the current obtained data when multiple asynchronous operations are executed at the same time.
  @param[in] async_cb Callback function for asynchronous network scanning
  @return Whether to successfully trigger the network scan operation
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT  uninitialized
  @retval SV_ERR_SERVICE_NOT_READY  service is not ready
  @retval SV_ERR_INVALID_ARG  Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_network_scan(int *async_index, sv_nw_network_scan_async_cb async_cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets power mode. You can disable the reporting of the service messages according to your needs to avoid waking up the device abnormally by corresponding events. It is supported to disable the message reporting of several services simultaneously. The setting of this function will not be saved after power-off.
  @param[in] lower_mode power mode,defined by SV_NW_LOWER_POWER_MASK_XXX
  @return Whether to successfully set the power mode
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_power_mode(uint8_t lower_mode);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get power mode.
  @param[out] lower_mode, defined by SV_NW_LOWER_POWER_MASK_XXX
  @return Whether to successfully set the power mode
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_power_mode(uint8_t *lower_mode);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets the preferred network mode and roaming notification status. The setting of this function will be saved in the event of power failure.
  @param[in] p_info  Preferred network and roaming notification status. 
  @return Whether to successfully set nwmode and roaming
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_pref_nwmode_roaming(sv_nw_pref_nwmode_roaming_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets the preferred network mode and roaming notification status.
  @param[out] p_info Preferred network mode and roaming notification status.
  @return Whether to successfully get nwmode and roaming
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_pref_nwmode_roaming(sv_nw_pref_nwmode_roaming_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets the information of a mobile network operator (wireless carrier).
  @param[out] p_info The information of a mobile network operator.
  @return Whether to successfully get the mobile operator name
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_mobile_operator_name(sv_nw_mobile_operator_name_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets information of the serving cell and the neighbour cells.
  @param[out] p_info Cell information.
  @return Whether to successfully get the cell information
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_cell_info(sv_nw_cell_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets the voice-dialing registration information.
  @param[out] p_info  Voice-dialing registration information.
  @return Whether to successfully get the voice registration status
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_voice_reg_status(sv_nw_reg_status_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets data-dialing registration information.
  @param[out] p_info Data-dialing registration information.
  @return Whether to successfully get the data registration status
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_data_reg_status(sv_nw_reg_status_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets the signal strength information and signal strength level. It only returns the signal strength information and signal strength level of the network on which the module registers currently.
  @param[out] p_info Signal strength information.
  @param[out] p_level: The signal strength level converted from p_info.
  @return Whether to successfully get the signal strength
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_signal_strength(sv_nw_signal_strength_info_t *p_info, SV_NW_SIGNAL_STRENGTH_LEVEL_E* p_level);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets the current cell access state.
  @param[out] p_info Cell access state.
  @return Whether to successfully get the cell access status
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_cell_access_status(SV_NW_CELL_ACCESS_STATE_TYPE_E *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets the network time.
  @param[out] p_info Network time information. 
  @return Whether to successfully get the network time
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_nitz_time_info(sv_nw_nitz_time_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets callback function for voice-dialing registration event.
  @param[in] cb_func Callback function for voice-dialing registration event. 
  @return Whether the voice registration event was successfully registered.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
 */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_voice_reg_ind_cb(sv_nw_voice_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets callback function for data-dialing registration event.
  @param[in] cb_func Callback function for data-dialing registration event.
  @return Whether the data registration event was successfully registered.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_data_reg_ind_cb(sv_nw_data_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets the callback function for signal strength event.
  @param[in] cb_func  Callback function for signal strength event.
  @return Whether the signal strength event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_signal_strength_ind_cb(sv_nw_signal_strength_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets the callback function for cell access state change event.
  @param[in] cb_func Callback function for cell access state change event.
  @return Whether the cell access status event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_cell_access_status_ind_cb(sv_nw_cell_access_status_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets the callback function for network time update event.
  @param[in] cb_func Callback function for network time update event. 
  @return Whether the network time event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_nitz_time_update_ind_cb(sv_nw_nitz_time_update_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets the callback function for WEA alert information. 
  @param[in] cb_func Callback function for WEA alert information.
  @return Whether the network time event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_wea_alert_ind_cb(sv_nw_wea_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  This function sets the callback function for ETWS alert information.
  @param[in] cb_func Callback function for ETWS alert information.
  @return Whether the network time event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_etws_alert_ind_cb(sv_nw_etws_reg_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register ims registration status event.
  @param[in] cb_func ims registration status indication callback function
  @return Whether the ims registration status event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_ims_reg_status_ind_cb(sv_nw_ims_reg_status_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register rf band info event.
  @param[in] cb_func rf band info indication callback function
  @return Whether the rf band info event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_rf_band_info_ind_cb(sv_nw_rf_band_info_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief register lte rach fail event.
  @param[in] cb_func lte rach fail indication callback function
  @return Whether the lte rach fail event was successfully registered
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_lte_rach_fail_ind_cb(sv_nw_lte_rach_fail_ind_cb cb_func);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  This function sets WEA configuration items. 
  @param[in] item  Types of WEA
  @param[in] p_info WEA configuration items.
  @return Whether to successfully set the wea config.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_wea_config(int item, sv_nw_wea_config_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  This function gets WEA configurations.
  @param[out] p_config WEA configuration items. 
  @return Whether the wea config was successfully obtained.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_wea_config(sv_nw_wea_config_t *p_config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function configures ETWS.
  @param[in] enable_etws  Enable or disable ETWS.
  @return Whether to successfully set the etws config.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_etws_config(uint8_t enable_etws);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function gets ETWS configuration.
  @param[out] p_enable_etws Indicate whether ETWS is enabled or disabled.
  @return Whether to successfully set the etws config.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_etws_config(uint8_t* p_enable_etws);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  bind subscription 
  @param[in] sub_type subscription type 
  @return Whether to successfully bind subscription.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_bind_subscription(SV_NW_BIND_SUB_TYPE_E sub_type);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get high capability subscription.
  @param[out] p_high_cap pointer that point to SV_NW_BIND_SUB_TYPE_E
  @return Whether the high capability subscription was successfully obtained.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_high_cap_sub(SV_NW_BIND_SUB_TYPE_E *p_high_cap);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set the SIM card index that can uniquely register to the 5G network. 
  the main difference between high and non-high capability subscription is that high capability 
  subscription can register to 5G network while non-high capability subscription can only 
  register to LTE or GSM.
  @param[in] high_cap high capability subscription 
  @return Whether to successfully set the high capability subscription.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_high_cap_sub(SV_NW_BIND_SUB_TYPE_E high_cap);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sets 5G service options. The configurations made by calling this function will take effect immediately and be saved automatically.
  @param[in] opt_mask Disable 5G service options. Currently, only SV_NW_NR5G_SO_NONE, SV_NW_NR5G_SO_NSA, and SV_NW_NR5G_SO_SA are supported. You can disable SA or NSA only, or disable both of them at the same time; or you can enable 5G feature by selecting SV_NW_NR5G_SO_NONE.
  @return Whether to successfully disable NR5G mode.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_disable_nr5g(uint16_t opt_mask);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get ims configuration.
  @param[out] p_ims_enable, Value:
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
int sv_nw_get_ims_config(uint8_t* p_ims_enable);

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
int sv_nw_set_ims_config(uint8_t ims_enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get ims configuration.
  @param[out] ims_config_ptr
  @return Whether to successfully get ims configuration.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_ims_config_ex(sv_nw_ims_config_ex_t* ims_config_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set ims configuration.
  @param[in] ims_config
  @return Whether to successfully set ims configuration.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-------------------------------------------------------------------------------------ss----------*/
int sv_nw_set_ims_config_ex(sv_nw_ims_config_ex_t ims_config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get ims registration status.
  @param[out] ims_reg_status_ptr, Value:
  0  - Unregistered
  1  - Registered
  @return Whether to successfully get ims registration status.
  @retval SV_ERR_OK successful execution.
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_ims_reg_status(uint8_t* ims_reg_status_ptr);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Force module to receive message by 4 antennas under LTE mode.
  This interface will result in increased current consumption. 
  Only for debugging use,do not use for commercial version.
  @param[in] dlm_4rx_enable dlm 4rx enable
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_dlm_4rx_enable(uint8_t dlm_4rx_enable);

/** 
  @brief Get endc configuration.
  @param[out] get_endc_config_ptr
  @return Whether to successfully get endc configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_endc_config(sv_nw_get_endc_config_t* get_endc_config_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set endc configuration.
  @param[in] set_endc_config
  @return Whether to successfully set endc configuration.
  @retval SV_ERR_OK successful
  @retval SV_ERR_NOT_INIT uninitialized
  @retval SV_ERR_SERVICE_NOT_READY service is not ready
  @retval SV_ERR_INVALID_ARG Invalid arguments
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_endc_config(sv_nw_set_endc_config_t set_endc_config);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Enables or disables NR5G deprioritization.
  @param[in] deprioritize_nr5g_enable
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_deprioritize_nr5g_enable(uint8_t deprioritize_nr5g_enable);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief WWAN reset without interrupting CV2X stack.
  @param[in] void
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_trigger_wwan_reset(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the DRX for the device.DRX interface takes effect only after the network injection is successful
  @param[out] get_drx_ptr setting for the device. 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_drx(sv_nw_get_drx_t * get_drx_ptr);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Sets the DRX for the device.DRX interface takes effect only after the network injection is successful
  @param[in] set_drx setting for the device. 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_drx(sv_nw_set_drx_t set_drx);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Gets the negotiated DRX for the device.DRX interface takes effect only after the network injection is successful
  @param[out] get_negotiated_drx_ptr setting for the device. 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_get_negotiated_drx(sv_nw_get_negotiated_drx_t * get_negotiated_drx_ptr);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the callback function for service error event.
  @param[in] cb  Callback function for service error event.
  @retval SV_ERR_OK successful execution.
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_set_service_error_cb(sv_nw_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  This function deinitializes the network registration service. 
  @return Whether the NW service was deinitialized successfully.
  @retval SV_ERR_OK successful execution.
  @retval Others Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_nw_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
