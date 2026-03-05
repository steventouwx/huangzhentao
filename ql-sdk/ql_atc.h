/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_atc.h 
  @brief AT command API

  @detailes
  Quectel AG55x series module ATC service.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly

  when     |  who        |  what, where, why
  -------- |  ---        |  ----------------------------------------------------------
  20191225 |  stan.li    |  Modify function description.
  20191225 |  solomon.cui|  Fix possible null pointer dereference.
  20190702 |  stan.li    |  Created .

  Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

#ifndef __QL_ATC_H__
#define __QL_ATC_H__

#ifdef __cplusplus
extern "C" {
#endif

#define QL_ATC_MAX_REQ_MSG_LEN   513    /**<  Maximum request message size. */
#define QL_ATC_MAX_RESP_MSG_LEN  4097   /**<  Maximum response message size. */


/**
  @brief This function is the callback function indicating AT command sent asynchronously.
  @param[in]  async_index 	Index value of AT commands sent asynchronously.
  @param[in]  req_buf	 AT command sent asynchronously
  @param[in]  rsp_buf	The result of sending AT command asynchronously
  @retval None
*/
typedef void (*ql_atc_async_cb)(int async_index, char *req_buf, char *rsp_buf);

/**
  @brief  This function is the callback function indicating ATC service error. 
  @param[in]  error  	Error code
  @retval None
*/
typedef void (*ql_atc_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function initializes the ATC service. 
  @note You must call this function before other functions can be used in this module.
  @return Whether the ATC service was successfully intialized.
  @retval QL_ERR_OK	the ATC service is initialized successfully.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Others failed to initialize the ATC service. Please refer to ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_atc_init(void);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sends AT command asynchronously. Some AT commands to execute may take a long time, so this function is designed to send AT command asynchronously in order not to affect the use of other functions.
  @param[in] req_buf AT command.
  @param[out] async_index  Index value of AT command sent asynchronously to identify AT command sent asynchronously at different times and to help the callback function identify the result obtained comes from when AT command is sent asynchronously
  @param[in] async_cb Async callback function
  @return Whether to successfully send the AT command asynchronously
  @retval QL_ERR_OK  asynchronously send AT command successfully.
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval Others failed to send AT command asynchronously. Please refer to ql-sdk/api/include/ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_atc_send_async(char *req_buf, int *async_index, void *async_cb);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief This function sends AT command until the AT command is successfully executed.
  @param[in] req_buf AT command.
  @param[in] rsp_buf AT command execution result.
  @param[in] rsp_len Length of rsp_buf.
  @return Whether to successfully send AT command
  @retval QL_ERR_OK execute AT command successfully.
  @retval QL_ERR_NOT_INIT uninitialized
  @retval QL_ERR_SERVICE_NOT_READY service is not ready
  @retval Others failed to execute AT command successfully. Please refer to ql-sdk/api/include/ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_atc_send(char *req_buf, char *rsp_buf, int rsp_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the callback function when service error occurs.
  @param[in] cb  Callback function whose type is ql_atc_service_error_cb_f.
  @retval QL_ERR_OK  set the callback function successfully.
  @retval Others failed to set the callback function. See ql-sdk/api/include/ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_atc_set_service_error_cb(ql_atc_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function deinitializes the ATC service. Please initialize the ATC service again before using any ATC related features.
  @return Whether the DM service was successfully deintialized.
  @retval QL_ERR_OK successful.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_atc_deinit(void);


#ifdef __cplusplus
}
#endif


#endif
