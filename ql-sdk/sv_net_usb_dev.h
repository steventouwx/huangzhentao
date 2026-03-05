/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_net_usb_dev.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_net_usb_dev相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/**
 * @file sv_net_usb_dev.h
 * @brief USB net device API
 *
 * @copyright Copyright (c) 2018 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
 * Quectel Wireless Solution Proprietary and Confidential.
 *
 * @detailes
 * EDIT HISTORY
 * This section contains comments describing changes made to the file.
 * Notice that changes are listed in reverse chronological order.
 *
 * $Header: $
 * when      |      who    |what, where, why
 * ----------|-------------|---------------------------------------------------------
 * 20190709  | tyler.kuang |Created
 */

#ifndef __SV_NET_USB_DEV_H__
#define __SV_NET_USB_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "sv_net_common.h"


typedef void (*sv_net_usb_dev_ind_cb_f)(const char *device,
        SV_NET_USB_DEV_STATUS_E pre_status,
        sv_net_usb_dev_t *p_dev);

typedef void (*sv_net_usb_dev_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Initialize the USB net device service
  @return Whether the USB net device service was intialized successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get USB network device status
  @param[in] device Device name, such as ecm0, rndis0..
  @param[out] p_dev Return device status information
  @return Whether was get USB network device status successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval SV_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_get_status(const char *device, sv_net_usb_dev_t *p_dev);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get all USB network device list
  @param[out] list USB network device array
  @param[in,out] list_len, in-> array size, out->current instance number
  @return Whether was get all USB network device list successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_get_list(sv_net_usb_dev_t *list, int *list_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set USB network device MTU. If it does not currently exist, then set it after the device 
  appears.
  @param[in] device Device name, such as ecm0, rndis0..
  @param[in] mtu MTU 
  @return Whether was the USB network device MTU set successfully.
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_set_mtu(const char *device, int mtu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get USB network device MTU
  @param[in] device Device name, such as ecm0, rndis0..
  @param[out] p_mtu Reutn device MTU
  @return Whether was the USB network device MTU get successfully.
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval SV_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_get_mtu(const char *device, int *p_mtu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set USB network device MAC address. If it does not currently exist, then set it after the device 
  appears.
  @param[in] device Device name, such as ecm0, rndis0..
  @param[in] macaddr MAC address, format: XX:XX:XX:XX:XX:XX 
  @return Whether was the USB network device MAC set successfully.
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval SV_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_set_macaddr(const char *device, const char *macaddr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get USB network device MAC address
  @param[in] device Device name, such as ecm0, rndis0..
  @param[out] macaddr_buf Return MAC address
  @param[in] buf_len Buffer size
  @return Whether was the USB network device MAC get successfully. 
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - as defined
  @retval SV_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval SV_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_get_macaddr(const char *device, char *macaddr_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Register USB network device status change event
  @param[in] cb 
  @return Whether was the USB network device register status change event successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_NOT_INIT - uninitialized
  @retval SV_ERR_SERVICE_NOT_READY - service is not ready
  @retval SV_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_set_ind_cb(sv_net_usb_dev_ind_cb_f cb);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is SV_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return Whether was the USB network device registration server error callback successfully
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_set_service_error_cb(sv_net_usb_dev_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Deinitialize the USB net device service
  @return Whether was the USB network device deinitialize successfully
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_net_usb_dev_deinit(void);

#ifdef __cplusplus
} 
#endif

#endif

