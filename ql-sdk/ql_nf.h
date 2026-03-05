/**
 * @file ql_nf.h 
 * @brief Network framework API 
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
 * 20181024  | tyler.kuang |Created
 */

#ifndef __QL_NF_H__
#define __QL_NF_H__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ql_net_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char name[QL_NET_MAX_NAME_LEN]; /**<Device name*/
    char ifname[QL_NET_MAX_NAME_LEN];/**<Interface name*/
    char pre_status[16];/**<Prev status*/
    char status[16];/**<Now status*/
    uint8_t has_addr;/**<IPV4 Valid when data call state is connected*/
    ql_net_addr_t addr;/**<IPV4 infomation*/
    uint8_t has_addr6;/**<IPV6 Valid when data call state is connected*/
    ql_net_addr6_t addr6;/**<IPV6 infomation*/
} ql_nf_interface_status_t;

typedef void (*ql_nf_interface_status_ind_cb_f)(ql_nf_interface_status_t *p_msg);

typedef void (*ql_nf_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Initialize network service
  @retval @return Whether the network service was successfully initialize
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_nf_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Send a command and wait for a response
  @param[in] cmd Command
  @param[out] resp_buf Response 
  @param[in] resp_len Response buffer length 
  @return Whether the command was successfully send .
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_nf_cmd(const char *cmd, char *resp_buf, int resp_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Send a command and wait for a response
  @param[in] cmd Command
  @param[out] resp_buf Response 
  @param[in] resp_len Response buffer length 
  @return Whether the json command was successfully send
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - as defined
  @retval QL_ERR_UNKNOWN - unknown error, failed to connect to service
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready, need to retry
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_nf_cmd_json(const char *cmd, char *resp_buf, int resp_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Register NF interface status change event event
  @param[in] cb 
  @return Whether callbalck was successfully registed
  @retval QL_ERR_OK - successful
  @retval QL_ERR_NOT_INIT - uninitialized
  @retval QL_ERR_SERVICE_NOT_READY - service is not ready
  @retval QL_ERR_INVALID_ARG - Invalid arguments
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_nf_set_interface_status_ind_cb(ql_nf_interface_status_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is QL_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return Whether callbalck was successfully registed
  @retval QL_ERR_OK - successful
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_nf_set_service_error_cb(ql_nf_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Deinitialize the data call service
  @return Whether was the data call service successfully deinitialize
  @retval QL_ERR_OK - successful
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_nf_deinit(void);

#ifdef __cplusplus
} 
#endif

#endif

