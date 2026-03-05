/**
 * @file ql_net_usb_dev.h
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

#ifndef __QL_NET_USB_DEV_H__
#define __QL_NET_USB_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ql_net_common.h"


typedef void (*ql_net_usb_dev_ind_cb_f)(const char *device,
        QL_NET_USB_DEV_STATUS_E pre_status,
        ql_net_usb_dev_t *p_dev);

typedef void (*ql_net_usb_dev_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Initialize the USB net device service
  @return Whether the USB net device service was intialized successfully
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get USB network device status
  @param[in] device Device name, such as ecm0, rndis0..
  @param[out] p_dev Return device status information
  @return Whether was get USB network device status successfully
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval QL_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_get_status(const char *device, ql_net_usb_dev_t *p_dev);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get all USB network device list
  @param[out] list USB network device array
  @param[in,out] list_len, in-> array size, out->current instance number
  @return Whether was get all USB network device list successfully
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_get_list(ql_net_usb_dev_t *list, int *list_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set USB network device MTU. If it does not currently exist, then set it after the device 
  appears.
  @param[in] device Device name, such as ecm0, rndis0..
  @param[in] mtu MTU 
  @return Whether was the USB network device MTU set successfully.
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_set_mtu(const char *device, int mtu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get USB network device MTU
  @param[in] device Device name, such as ecm0, rndis0..
  @param[out] p_mtu Reutn device MTU
  @return Whether was the USB network device MTU get successfully.
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval QL_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_get_mtu(const char *device, int *p_mtu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Set USB network device MAC address. If it does not currently exist, then set it after the device 
  appears.
  @param[in] device Device name, such as ecm0, rndis0..
  @param[in] macaddr MAC address, format: XX:XX:XX:XX:XX:XX 
  @return Whether was the USB network device MAC set successfully.
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval QL_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_set_macaddr(const char *device, const char *macaddr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get USB network device MAC address
  @param[in] device Device name, such as ecm0, rndis0..
  @param[out] macaddr_buf Return MAC address
  @param[in] buf_len Buffer size
  @return Whether was the USB network device MAC get successfully. 
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval QL_ERR_INTERFACE_NOT_FOUND - device does not exist
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_get_macaddr(const char *device, char *macaddr_buf, int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Register USB network device status change event
  @param[in] cb 
  @return Whether was the USB network device register status change event successfully
  @retval QL_ERR_OK - successful
  @retval QL_ERR_NOT_INIT - uninitialized
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready
  @retval QL_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_set_ind_cb(ql_net_usb_dev_ind_cb_f cb);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is QL_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return Whether was the USB network device registration server error callback successfully
  @retval QL_ERR_OK - successful
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_set_service_error_cb(ql_net_usb_dev_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Deinitialize the USB net device service
  @return Whether was the USB network device deinitialize successfully
  @retval QL_ERR_OK - successful
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_net_usb_dev_deinit(void);

#ifdef __cplusplus
} 
#endif

#endif

