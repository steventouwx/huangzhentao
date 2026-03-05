/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_wifi.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_wifi相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_WIFI_H__
#define __SV_WIFI_H__

#include "sv_wifi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Wi-Fi enable status callback function.

  @param[in]  pre_status: The previous Wi-Fi enable status.
  @param[in]  status: The current Wi-Fi enable status.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_enable_status_ind_cb_f)(SV_WIFI_ENABLE_STATUS_E pre_status, SV_WIFI_ENABLE_STATUS_E status);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  AP mode status callback function.

  @param[in]  pre_status: The previous AP mode status.
  @param[in]  p_msg: The current AP mode status information.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_ap_status_ind_cb_f)(SV_WIFI_AP_INDEX_E index, SV_WIFI_AP_STATUS_E pre_status, sv_wifi_ap_status_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  STA mode status callback function.

  @param[in]  pre_status: The previous STA mode status.
  @param[in]  p_msg: The current STA mode status information.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_sta_status_ind_cb_f)(SV_WIFI_STA_STATUS_E pre_status, sv_wifi_sta_status_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  STA scan results callback function.

  @param[in]  p_msg: The information list of external hotspots.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_sta_scan_result_ind_cb_f)(sv_wifi_sta_scan_result_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This is a callback function for status of connection between AP and STA device.

  @param[in]  index: The index of AP mode.
  @param[in]  p_msg: The status of connection between AP and STA device.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_ap_sta_connect_ind_cb_f)(SV_WIFI_AP_INDEX_E index, sv_wifi_sta_connect_status_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Wi-Fi service error callback function.

  @param[in]  error: Error code.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This is a callback function for status of P2P enable.

  @param[in]  pre_status: The previous P2P enable status.
  @param[in]  status: The current P2P enable status.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_p2p_enable_status_ind_cb_f)(SV_WIFI_P2P_ENABLE_STATUS_E pre_status, SV_WIFI_P2P_ENABLE_STATUS_E status);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The found p2p device callback function.

  @param[in]  p_msg: The information of p2p device 

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_p2p_dev_found_ind_cb_f)(sv_wifi_p2p_found_dev_info_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Peer p2p device requesting connection callback function.

  @param[in]  p_msg: The information of peer p2p device 

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_p2p_peer_dev_req_ind_cb_f)(sv_wifi_p2p_req_peer_dev_info_t *pmsg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  P2P status callback function.

  @param[in]  pre_status: The previous P2P status.
  @param[in]  status: The current P2P status information.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*sv_wifi_p2p_status_ind_cb_f)(SV_WIFI_P2P_STATUS_E pre_status, sv_wifi_p2p_status_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function initializes Wi-Fi service.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_SERVICE_NOT_READY: Wi-Fi service is not ready. It is recommended to try again.
  @retval  Other value: Failed execution. See sv_type.h for the error code.

  @note  Before other Wi-Fi APIs are used, this function must be called to initialize the Wi-Fi service.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function deinitializes Wi-Fi service.

  @retval  SV_ERR_OK: Successful execution.
           Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the country code of wlan driver

  @param[in]  country_code: The country code to be set. The country code cannot exceed 2 bytes and is not empty. For example,  CN represents China.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.

  @note  Before calling the sv_wifi_enable(), call this function firstly to set country code,
         If this function is not called, The country code "CN" will be used by default.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_country_code_set(const char *country_code);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the country code of wlan driver.

  @param[out]  country_code_buf: The buffer for storing the current country code. The buffer size is recommended to be 3 bytes.
  @param[in]   The length of buffer.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_country_code_get(char *country_code_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables Wi-Fi function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_enable(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function disables Wi-Fi function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_disable(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers Wi-Fi enable status callback function.

  @param[in]  cb: The Wi-Fi enable status callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_set_enable_status_ind_cb(sv_wifi_enable_status_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the Wi-Fi working mode.

  @param[in]  mode: The Wi-Fi working mode to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_work_mode_set(SV_WIFI_WORK_MODE_E mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the current Wi-Fi working mode.

  @param[out]  p_mode: The current Wi-Fi working mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_work_mode_get(SV_WIFI_WORK_MODE_E *p_mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the SSID in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  ssid: The SSID to be set. It should be within 32 bytes and is not empty.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_ssid_set(SV_WIFI_AP_INDEX_E idx, const char *ssid);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the SSID in AP mode.

  @param[in]   idx: The index of AP mode.
  @param[out]  ssid_buf: The buffer for storing the current SSID, the buffer size is recommended to be 33 bytes.
  @param[in]   buf_len: The length of the buffer.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_ssid_get(SV_WIFI_AP_INDEX_E idx, char *ssid_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the SSID hiding status in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  ssid_hidden: The SSID hiding status to be set.
                           0: The SSID is not hidden.
                           1: The SSID is hidden.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_ssid_hidden_set(SV_WIFI_AP_INDEX_E idx, int ssid_hidden);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the SSID hiding status in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[out]  p_ssid_hidden: The current SSID hiding status in AP mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_ssid_hidden_get(SV_WIFI_AP_INDEX_E idx, int *p_ssid_hidden);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the working protocol mode in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  mode: The working protocol mode to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_mode_set(SV_WIFI_AP_INDEX_E idx, SV_WIFI_AP_MODE_TYPE_E mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the working protocol mode in AP mode.

  @param[in]   idx: The index of AP mode.
  @param[out]  p_mode: The current working protocol mode in AP mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_mode_get(SV_WIFI_AP_INDEX_E idx, SV_WIFI_AP_MODE_TYPE_E *p_mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the bandwidth in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  bandwidth: The bandwidth to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_bandwidth_set(SV_WIFI_AP_INDEX_E idx, SV_WIFI_BANDWIDTH_E bandwidth);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the bandwidth in AP mode.

  @param[in]   idx: The index of AP mode.
  @param[out]  p_bandwidth: The current bandwidth in AP mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_bandwidth_get(SV_WIFI_AP_INDEX_E idx, SV_WIFI_BANDWIDTH_E *p_bandwidth);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the channel in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  channel: The channel to be set.
                       0: Adaptive
                       1/2/3/4/5/6/7/8/9/10/11/12/13/14: 2.4G channel.
                       36/40/44/48/52/56/60/64/100/104/108/112/116/120/124/128/132/136/140/144/149/153/157/161/165: 5GHZ channel.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_channel_set(SV_WIFI_AP_INDEX_E idx, int channel);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the channel in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[out]  p_channel: The currently-used channel in AP mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_channel_get(SV_WIFI_AP_INDEX_E idx, int *p_channel);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the maximum number of terminal connections in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  max_sta_num: The maximum number of terminal connections in AP mode to be set. Range: 1~32.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_max_sta_num_set(SV_WIFI_AP_INDEX_E idx, int max_sta_num);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the maximum number of terminal connections in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[out]  p_max_sta_num: The current maximum number of terminal connections in AP mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_max_sta_num_get(SV_WIFI_AP_INDEX_E idx, int *p_max_sta_num);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the country code in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  country_code: The country code to be set. The country code cannot exceed 2 bytes and is not empty. For example,  CN represents China.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_country_code_set(SV_WIFI_AP_INDEX_E idx, const char *country_code);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the country code in AP mode.

  @param[in]   idx: The index of AP mode.
  @param[out]  country_code_buf: The buffer for storing the current country code. The buffer size is recommended to be 3 bytes.
  @param[in]   The length of buffer.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_country_code_get(SV_WIFI_AP_INDEX_E idx, char *country_code_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the AP isolation state.

  @param[in]  idx: The index of AP mode.
  @param[in]  isolate: AP isolation status to be set.
                       0: AP is not isolated.
                       1: AP is isolated.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_isolate_set(SV_WIFI_AP_INDEX_E idx, int isolate);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the AP isolation state.

  @param[in]   idx: The index of AP mode.
  @param[out]  p_isolate: The current AP isolation status.
                          0: AP is not isolated.
                          1: AP is isolated.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_isolate_get(SV_WIFI_AP_INDEX_E idx, int *p_isolate);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets MAC address access rules in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  acl_rule: MAC address access rules to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_mac_acl_rule_set(SV_WIFI_AP_INDEX_E idx, SV_WIFI_MAC_ACL_RULE_E acl_rule);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets MAC address access rules in AP mode.

  @param[in]   idx: The index of AP mode.
  @param[out]  p_acl_rule: The current MAC address access rules.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_mac_acl_rule_get(SV_WIFI_AP_INDEX_E idx, SV_WIFI_MAC_ACL_RULE_E *p_acl_rule);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function adds or deletes the specified MAC address for the current MAC address access rule.

  @param[in]  idx: The index of AP mode.
  @param[in]  cmd: MAC address operation command.
  @param[in]  macaddr: MAC address to be added or deleted., The format of MAC address 
                       must be %02X:%02X:%02X:%02X:%02X:%02X, For example: 1A:2B:3C:4D:56:78.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_acl_mac_set(SV_WIFI_AP_INDEX_E idx, SV_WIFI_ACL_MAC_CMD_E cmd, const char *macaddr);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets all MAC addresses in the current MAC address access rule.

  @param[in]   idx: The index of AP mode.
  @param[out]  p_mac_list: MAC address list.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_acl_mac_get(SV_WIFI_AP_INDEX_E idx, sv_wifi_acl_mac_list_t *p_mac_list);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the authentication in AP mode.

  @param[in]  idx: The index of AP mode.
  @param[in]  p_auth: The authentication information to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_auth_set(SV_WIFI_AP_INDEX_E idx, sv_wifi_ap_auth_t *p_auth);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the authentication information in AP mode.

  @param[in]   idx: The index of AP mode.
  @param[out]  p_auth: The current authentication information.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_auth_get(SV_WIFI_AP_INDEX_E idx, sv_wifi_ap_auth_t *p_auth);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the AP mode status callback function.

  @param[in]  cb: The AP mode status callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_set_status_ind_cb(sv_wifi_ap_status_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function starts the AP function.

  @param[in]  index: The index of AP mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_start(SV_WIFI_AP_INDEX_E index);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function stops the AP function.

  @param[in]  index: The index of AP mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_stop(SV_WIFI_AP_INDEX_E index);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the AP mode status information.

  @param[in]   index: The index of AP mode.
  @param[out]  p_sta: The current AP mode status information.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_ap_get_status(SV_WIFI_AP_INDEX_E index, sv_wifi_ap_status_t *p_sta);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the SSID in STA mode.

  @param[in]  ssid: The SSID to be set. The SSID cannot exceed 32 bytes and is not empty.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_ssid_set(const char *ssid);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the SSID in STA mode.

  @param[out]  ssid_buf: The buffer for storing the current SSID.
  @param[in]   buf_len: The length of the buffer.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_ssid_get(char *ssid_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the authentication in STA mode.

  @param[in]  p_auth: The authentication information to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_auth_set(sv_wifi_sta_auth_t *p_auth);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the authentication information in STA mode.

  @param[out]  p_auth: The current authentication information.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_auth_get(sv_wifi_sta_auth_t *p_auth);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the STA mode status callback function.

  @param[in]  cb: The STA mode status callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_set_status_ind_cb(sv_wifi_sta_status_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function starts the STA function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_start(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function stops the STA function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_stop(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the STA mode status information.

  @param[out]  p_sta: The current STA mode status information.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_get_status(sv_wifi_sta_status_t *p_sta);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function scans for external hotspots in STA mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_start_scan(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the scan result callback function.

  @param[in]  cb: The scan result callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_sta_set_scan_result_ind_cb(sv_wifi_sta_scan_result_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the callback function for status of connection between AP and STA device.

  @param[in]  cb: The callback function for status of connection between AP and STA device.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_set_ap_sta_connect_ind_cb(sv_wifi_ap_sta_connect_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the Wi-Fi service error callback function.

  @param[in]  cb: Wi-Fi service error callback function, executed only when the Wi-Fi service exits abnormally.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_set_service_error_cb(sv_wifi_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the P2P device name.

  @param[in]  dev_name: The device name to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_dev_name_set(const char *dev_name);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the P2P device name.

  @param[out]  dev_name_buf: The buffer for storing the current device name.
  @param[in]   buf_len: The length of the buffer.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_dev_name_get(char *dev_name_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the P2P device type.

  @param[in]  dev_type: The device type to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_dev_type_set(const char *dev_type);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the P2P device type.

  @param[out]  dev_type_buf: The buffer for storing the current device type.
  @param[in]   buf_len: The length of the buffer.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_dev_type_get(char *dev_type_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the operating class and operating channel in P2P mode.

  @param[in]  oper_class:   The operating class to be set.
  @param[in]  oper_channel: The operating channel to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.

  @note  The operating class and operating channel can determine the center frequency,
         The following are the corresponding values.
     --------------------------------------------------------------------------------------------
     |  oper_class  |       oper_channel        |          frequency          | channel spacing |
     |-------------------------------------------------------------------------------------------
     |      81      |           1-13            |   2407 + 5 * oper_channel   |      25MHz      |
     |      82      |            14             |   2414 + 5 * oper_channel   |      25MHz      |
     |      115     |        36,40,44,48        |   5000 + 5 * oper_channel   |      20MHz      |
     |      116     |           36,44           |   5000 + 5 * oper_channel   |      40MHz      |
     |      117     |           40,48           |   5000 + 5 * oper_channel   |      40MHz      |
     |      124     |     149,153,157,161       |   5000 + 5 * oper_channel   |      20MHz      |
     |      125     |    149,153,157,161,165    |   5000 + 5 * oper_channel   |      20MHz      |
     |      126     |          149,157          |   5000 + 5 * oper_channel   |      40MHz      |
     |      127     |          153,161          |   5000 + 5 * oper_channel   |      40MHz      |
     --------------------------------------------------------------------------------------------
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_oper_class_channel_set(int oper_class, int oper_channel);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the operating class and operating channel in P2P mode.

  @param[out]  p_oper_class:   The currently-used operating class in p2p mode.
  @param[out]  p_oper_channel: The currently-used operating channel in p2p mode.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_oper_class_channel_get(int *p_oper_class, int *p_oper_channel);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the suffix of ssid.

  @param[in]  ssid_postfix: The suffix of ssid to be set.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_ssid_postfix_set(const char *ssid_postfix);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the suffix of ssid.

  @param[out]  ssid_postfix_buf: The buffer for storing the current suffix of ssid.
  @param[in]   buf_len: The length of the buffer.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_ssid_postfix_get(char *ssid_postfix_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the P2P enable status callback function.

  @param[in]  cb: P2P enable status callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_set_enable_status_ind_cb(sv_wifi_p2p_enable_status_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables P2P function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_enable(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function disables P2P function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_disable(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the found p2p device callback function.

  @param[in]  cb: The found p2p device callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_set_dev_found_ind_cb(sv_wifi_p2p_dev_found_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function start finding P2P device.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_dev_find_start(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function stop finding P2P device.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_dev_find_stop(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets P2P mode status information.

  @param[out]  p_status: The current P2P mode status information.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_get_status(sv_wifi_p2p_status_t *p_status);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the P2P status callback function.

  @param[in]  cb: The P2P status callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_set_status_ind_cb(sv_wifi_p2p_status_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers peer p2p device requesting connection callback function.

  @param[in]  cb: Peer p2p device requesting connection callback function.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_set_peer_dev_req_ind_cb(sv_wifi_p2p_peer_dev_req_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function establishes a P2P connection.

  @param[in]  macaddr: The MAC address of peer P2P device.
  @param[in]  wps_method: key words: "pbc", "pin" and pin_code, The pin_code is a string that 
                          dynamically generated random from peer display.
  @param[in]  go_intent: Degree of desire to be GO, range: 0-15.

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  SV_ERR_INVALID_ARG: Failed execution due to illegal parameter.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_connect(const char *macaddr, const char *method, int go_intent);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function disconnects a P2P connection

  @retval  SV_ERR_OK: Successful execution.
  @retval  SV_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  SV_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See sv_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_wifi_p2p_disconnect(void);

#ifdef __cplusplus
}
#endif
#endif

