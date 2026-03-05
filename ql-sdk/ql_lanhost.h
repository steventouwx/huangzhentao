/**
 * @file ql_lanhost.h 
 * @brief LANHOST API 
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
 * when     |    who       |  what, where, why
 * ---------|------------- |-------------------------------------------------------------
 * 20191016 |  tyler.kuang |  Created
 */

#ifndef __QL_LANHOST_H__
#define __QL_LANHOST_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ql_net_common.h"


typedef void (*ql_lanhost_ind_cb_f)(QL_LANHOST_EVENT_TYPE_E event_type, ql_lanhost_t *p_host);

typedef void (*ql_lanhost_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Initialize the lanhost service
  @return Whether the lanhost was initialized successfully.
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_lanhost_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Get all lanhost list, the maximum of lanhost is defined by QL_NET_MAX_LANHOST_NUM 
  @param[out] list Lanhost array
  @param[in,out] list_len, in-> array size, out->current instance number
  @return Whether the lanhost array was get successfully. 
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_lanhost_get_list(ql_lanhost_t *list, int *list_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Register lanhost status change event
  @param[in] cb 
  @return Whether the cb was register successfully.
  @retval QL_ERR_OK - successful
  @retval QL_ERR_NOT_INIT - uninitialized
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready
  @retval QL_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_lanhost_set_ind_cb(ql_lanhost_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is QL_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return Whether the cb was register successfully.
  @retval QL_ERR_OK - successful
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_lanhost_set_service_error_cb(ql_lanhost_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Deinitialize the lanhost service 
  @return Whether deinitialize the lanhost service successfully.
  @retval QL_ERR_OK - successful
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_lanhost_deinit(void);

#ifdef __cplusplus
} 
#endif

#endif

