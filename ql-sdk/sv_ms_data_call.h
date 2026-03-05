/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_ms_data_call.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_ms_data_call相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/**
 * @file sv_ms_data_call.h
 * @brief Data service API
 *
 * @detailes
 * EDIT HISTORY
 * This section contains comments describing changes made to the file.
 * Notice that changes are listed in reverse chronological order.
 *
 * $Header: $
 * when      |what, where, why
 * ----------|---------------------------------------------------------
 * 20210624  |Created
 */


#ifndef __SV_MS_DATA_CALL_H__
#define __SV_MS_DATA_CALL_H__
#include "sv_data_call_common.h"

#ifdef __cplusplus
extern "C" {
#endif



/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Initialize the data call service
  @return Whether the data call service initialize successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Create a data call instance
  @param[in] call_id The unique identifier of the data call instance, specified by the user
  @param[in] call_name Friendly data call name,  specified by the user
  @param[in] is_background Whether the data call status is maintained by the data call service daemon.
  If it is 0, the data call instance will be deleted after the data call process exits.
  @return Whether the data call service create successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_create(int call_id, const char *call_name, int is_background);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Alloc for a data call configuration instance
  @return Whether the alloc memery successfully
  @retval NULL - Not enough memory
  @retval Other - successful
  */
/*-----------------------------------------------------------------------------------------------*/
sv_data_call_param_t *sv_ms_data_call_param_alloc(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Initialize the data call configuration instance
  @param[in] param Point to the data call configuration instance
  @return Whether the data call configuration instance initialize successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_init(sv_data_call_param_t *param);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Release the data call configuration instance
  @param[in] param Point to the data call configuration instance
  @return Whether the data call configuration instance release successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_free(sv_data_call_param_t *param);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Bind APN ID, range:1-16
  @param[in] param Point to the data call configuration instance
  @param[in] apn_id APN ID, range:1-16
  @return Whether the data call set param point configuration instance successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_apn_id(sv_data_call_param_t *param, int apn_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get APN name from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] buf APN ID
  @return Whether the data call get param point configuration instance successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_apn_id(sv_data_call_param_t *param, int *apn_id);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure APN name
  @param[in] param Point to the data call configuration instance
  @param[in] apn_name APN name
  @return Whether the data call set APN name successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_apn_name(sv_data_call_param_t *param, const char *apn_name);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get APN name from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] buf APN name buffer
  @param[in] buf_len APN name buffer size
  @return Whether the data call get APN name successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_apn_name(sv_data_call_param_t *param, char *buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure APN user name
  @param[in] param Point to the data call configuration instance
  @param[in] user_name APN user name
  @return Whether the data call set APN user name successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_user_name(sv_data_call_param_t *param, const char *user_name);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get APN user name from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] buf APN user name buffer
  @param[in] buf_len APN user name buffer size
  @return Whether the data call get APN user name successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_user_name(sv_data_call_param_t *param, char *buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure APN user password
  @param[in] param Point to the data call configuration instance
  @param[in] user_password APN user password
  @return Whether the data call set APN user password successfully
  @retval SV_ERR_OK - Not enough memory
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_user_password(sv_data_call_param_t *param, const char *user_password);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get APN user password from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] buf APN user password buffer
  @param[in] buf_len APN user password buffer size
  @return Whether the data call get APN user password successfully
  @retval SV_ERR_OK - Not enough memory
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_user_password(sv_data_call_param_t *param, char *buf, int buf_len);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure the data call authentication method
  @param[in] param Point to the data call configuration instance
  @param[in] auth_pref Defined by SV_DATA_CALL_AUTH_PREF_E
  @return Whether the data call set authentication method successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_auth_pref(sv_data_call_param_t *param, int auth_pref);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure the data call authentication method
  @param[in] param Point to the data call configuration instance
  @param[out] p_data Store return value
  @return Whether the data call get authentication method successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_auth_pref(sv_data_call_param_t *param, int *p_data);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure the data call IP version
  @param[in] param Point to the data call configuration instance
  @param[in] ip_ver Defined by SV_NET_IP_VER_E
  @return Whether the data call set IP version successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_ip_version(sv_data_call_param_t *param, int ip_ver);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get IP version from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] p_ver Store return value
  @return Whether the data call get IP version successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_ip_version(sv_data_call_param_t *param, int *p_ver);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure the data call auto reconnection mode
  @param[in] param Point to the data call configuration instance
  @param[in] mode Defined by SV_NET_DATA_CALL_RECONNECT_MODE_E
  @return Whether the data call set auto reconnection mode successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_reconnect_mode(sv_data_call_param_t *param, int reconnect_mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get auto reconnection mode from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] p_mode Store return value
  @return Whether the data call get auto reconnection mode successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_reconnect_mode(sv_data_call_param_t *param, int *p_mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure the data call auto reconnection interval
  @param[in] param Point to the data call configuration instance
  @param[in] time_list Interval time list in ms
  @param[in] num Number of time list
  @return Whether the data call set auto reconnection interval successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_reconnect_interval(sv_data_call_param_t *param, int *time_list, int num);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get auto reconnection interval from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] time_list Store return value
  @param[in,out] p_num
  @return Whether was the get auto reconnection interval from configuration instance successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_reconnect_interval(sv_data_call_param_t *param, int *time_list, int *p_num);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure the data call sim id
  @param[in] param Point to the data call configuration instance
  @param[in] sim_id Store return value
  @return Whether was the configure the data call sim id successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_set_sim_id(sv_data_call_param_t *param, int sim_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the data call sim id from configuration instance
  @param[in] param Point to the data call configuration instance
  @param[out] p_sim_id Store return value
  @return Whether was the get the data call sim id from configuration instancesuccessfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_param_get_sim_id(sv_data_call_param_t *param, int *p_sim_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Configure the specified data call instance
  @param[in] call_id Specify a data call instance
  @param[in] param Point to the data call configuration instance
  @return Whether the data call configure the specified instance successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_config(int call_id, sv_data_call_param_t *param);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the specified data call configuration instance
  @param[in] call_id Specify a data call instance
  @param[in] param Point to the data call configuration instance
  @return Whether the data call get the specified instance successfully
  @retval SV_ERR_OK - Successful
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_get_config(int call_id, sv_data_call_param_t *param);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Start data call
  @param[in] call_id Specify a data call instance
  @return Whether the data call start successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_start(int call_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Stop data call
  @param[in] call_id Specify a data call instance
  @return Whether the data call stop successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_stop(int call_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Delete a data call instance
  @param[in] call_id Specify a data call instance
  @return Whether the data call delte successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_delete(int call_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the current data call instance list
  @param[out] list Data call instance array
  @param[in,out] list_len, in-> Data call instance array size, out->current data call instance number
  @return Whether get the current data call instance list successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_get_list(sv_data_call_item_t *list, int *list_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the data call status
  @param[in] call_id Specify a data call instance
  @param[out] p_sta Point to status instance
  @return Whether get the current data call status successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_get_status(int call_id, sv_data_call_status_t *p_sta);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the packet data transfer statistics from the current data call
  @param[in] call_id Specify a data call instance
  @param[out] p_sta Point to stats instance
  @return Whether get the packet data transfer statistics from the current data call successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_get_pkt_stats(int call_id, sv_data_call_pkt_stats_t *p_sta);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Reset the packet data transfer statistics from the current data call
  @param[in] call_id Specify a data call instance
  @return
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_reset_pkt_stats(int call_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get bit rate of the data call
  @param[in] call_id Specify a data call instance
  @param[out] p_rate The bit rate of data call 
  @return Whether get bit rate of data call successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_get_bit_rate(int call_id, sv_data_call_bit_rate_t *p_rate);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Register data call status change event
  @param[in] cb
  @return Whether the data call register status change event successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_set_status_ind_cb(sv_data_call_status_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set APN related configuration.If the apn does not exist, it is automatically created.
  @param[in] apn_id APN ID, range:1-16
  @param[in] p_info APN configuration
  @return Whether the data call set APN related configuration successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_set_apn_config(int sim_id, int apn_id, sv_data_call_apn_config_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Set APN related configuration. If the apn does not exist, it is automatically created and
  the default parameters are set.
  @param[in] apn_id APN ID, range:1-16
  @param[out] p_info APN configuration
  @return Whether the data call get APN related configuration successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_get_apn_config(int sim_id, int apn_id, sv_data_call_apn_config_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief set timer interval of RRC from connection to release
  @param[in] timer_interval the time interval that module will check iF there any data transfer
  @param[in] send_scri whether send scri to network
  @param[in] call_id the unique identifier of the data call instance, specified by the user
  @return Whether set go_dormant successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_go_dormant(int timer_interval, int send_scri, int call_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief set data filter mode
  @param[in] call_id the unique identifier of the data call instance, specified by the user
  @param[in] mode Whether to enable data filter,1-enable, 0-disable
  @param[in] auto_exit whether to auto exit data filter,1-enable, 0-disable
  @return Whether set data filter mode successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_set_data_filter_mode(int call_id, int mode, int auto_exit);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief get data filter mode
  @param[in] call_id the unique identifier of the data call instance, specified by the user
  @param[out] mode Whether to enable data filter,1-enable, 0-disable
  @return Whether set data filter mode successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_get_data_filter_mode(int call_id, int *mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief add data filter configuration
  @param[in] call_id the unique identifier of the data call instance, specified by the user
  @param[in] p_info Data filter configuration
  @return Whether add data filter configuration successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_add_data_filter(int call_id, sv_data_call_data_filter_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief remove all data filter configuration
  @param[in] call_id the unique identifier of the data call instance, specified by the user
  @return Whether remove data filter configuration successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_remove_all_data_filter(int call_id);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is SV_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return Whether the data call reister service successfully
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_set_service_error_cb(sv_data_call_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Deinitialize the data call service
  @return Whether the call service was deinitialize successfully
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_data_call_deinit(void);


#ifdef __cplusplus
}
#endif

#endif

