/**
 * @file ql_sms.h
 * @brief Sms service API.
 *
 * @details 
 * QuecOpen AG550Q&AG551Q series module sim service. 
 *
 * @htmlonly 
 * <span style="font-weight: bold">History</span> 
 * @endhtmlonly
 *
 * when|who|why
 * ----------|--------------|-------------------------------------------
  20200107   |solomon.cui     |Add GSM-7bit and ISO 8859-1 conversion.
  20191225   |solomon.cui     |Modify fucntion description.
  20191017   |solomon.cui     |Free async reponse not user data.
  20190815   |solomon.cui     |Add service type for sending message.
  20190627   |solomon.cui     |Support asynchronously send msg and pdu
  20190625   |solomon.cui     |Convert timestamp frome hex to dec.
  20190614   |solomon.cui     |Created .
 *
 * @copyright Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
 * Quectel Wireless Solution Proprietary and Confidential.
 * 
 */

#ifndef __QL_SMS_H__
#define __QL_SMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ql_sms_common.h"

/**
 * send PDU short messages asynchronously callback function
 * @param[in] id: ID of the event that PDU SMS messages are sent asynchronously
 * @param[in] result: Result of sending PDU SMS messages asynchronously
 * @return no value
 */
typedef void (*ql_sms_pdu_async_cb_f)(int id, int result);

/**
 * recv PDU short messages callback function
 * @param[in] p_pdu: PDU SMS message
 * @return no value
 */
typedef void (*ql_sms_pdu_recv_cb_f)(ql_sms_pdu_t *p_pdu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Initializes SMS service.
  @return Whether the SMS service was initialized successfully.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Deinitializes SMS service.
  @return Whether the SMS service was deinitialized successfully.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets the service center address.
  @param[in] addr service center address.
  @param[in] len  service center address length.
  @return Whether the service center address was set successfully.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_service_center_addr(char *addr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets the service center address.
  @param[out] addr service center address.
  @param[in] len  service center address length.
  @return Whether the service center address was successfully obtained.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_get_service_center_addr(char *addr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends message synchronously.
  @param[in] p_msg pointer to ql_sms_msg_t.
  @return Whether the message was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_send_msg(ql_sms_msg_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends message asynchronously.
  @param[in] p_msg  pointer to ql_sms_msg_t
  @param[out] id    id for this async operation
  @param[in] cb     async callback
  @return Whether the message was successfully sent asynchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_send_msg_async(ql_sms_msg_t *p_msg, int *id, ql_sms_msg_async_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SMS message reception callback hanlder.
  @param[in] cb message reception callback handler.
  @return Whether the message reception callback hanlder was set successfully.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_msg_recv_cb(ql_sms_msg_recv_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends PDU synchronously.
  @param[in] p_pdu SMS PDU.
  @return Whether the PDU was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_send_pdu(ql_sms_pdu_t *p_pdu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends PDU asynchronously.
  @param[in] p_pdu sms pdu.
  @param[out] id id for this async operation.
  @param[in] cb async callback.
  @return Whether the PDU was successfully sent asynchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_send_pdu_async(ql_sms_pdu_t *p_pdu, int *id, ql_sms_pdu_async_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SMS PDU reception callback hanlder.
  @param[in] cb PDU reception callback handler.
  @return Whether the PDU reception callback hanlder was set successfully.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_pdu_recv_cb(ql_sms_pdu_recv_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is QL_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return
  QL_ERR_OK - successful
  Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_service_error_cb(ql_sms_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Binds the current control point to a specific subscription.
  @param[in] sub  Subscription type.
  @return Whether the subscription was successfully bound.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_bind_subscription(QL_SMS_SUBSCRIPTION_E sub);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set sms white-list enable.
  @param[in] enable (TRUE or FALSE).
  @return Whether the message was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_white_list_enable(uint8_t enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get sms white-list enable.
  @param[OUT] enable_ptr (TRUE or FALSE).
  @return Whether the message was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_get_white_list_enable(uint8_t *enable_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  added a sms number to white-list.
  @param[in] num_ptr pointer to sms number.
  @return Whether the message was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_white_list_add_num(char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete a sms number from white-list.
  @param[in] num_ptr pointer to sms number.
  @return Whether the message was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_white_list_del_num(char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete all sms number from white-list.
  @param[in] num_ptr pointer to sms number.
  @return Whether the message was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_set_white_list_del_all(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get all sms white-list.
  @param[OUT] info_ptr pointer to all sms white-list number.
  @return Whether the message was successfully sent synchronously.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sms_get_white_list_all(ql_sms_white_list_info_t *info_ptr);
#ifdef __cplusplus
}
#endif

#endif  /* __QL_SMS_H__ */
