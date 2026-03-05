/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_sms.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_sms相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_SMS_H__
#define __SV_SMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "sv_sms_common.h"

/**
 * send PDU short messages asynchronously callback function
 * @param[in] id: ID of the event that PDU SMS messages are sent asynchronously
 * @param[in] result: Result of sending PDU SMS messages asynchronously
 * @return no value
 */
typedef void (*sv_sms_pdu_async_cb_f)(int id, int result);

/**
 * recv PDU short messages callback function
 * @param[in] p_pdu: PDU SMS message
 * @return no value
 */
typedef void (*sv_sms_pdu_recv_cb_f)(sv_sms_pdu_t *p_pdu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Initializes SMS service.
  @return Whether the SMS service was initialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Deinitializes SMS service.
  @return Whether the SMS service was deinitialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets the service center address.
  @param[in] addr service center address.
  @param[in] len  service center address length.
  @return Whether the service center address was set successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_service_center_addr(char *addr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets the service center address.
  @param[out] addr service center address.
  @param[in] len  service center address length.
  @return Whether the service center address was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_get_service_center_addr(char *addr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends message synchronously.
  @param[in] p_msg pointer to sv_sms_msg_t.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_send_msg(sv_sms_msg_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends message asynchronously.
  @param[in] p_msg  pointer to sv_sms_msg_t
  @param[out] id    id for this async operation
  @param[in] cb     async callback
  @return Whether the message was successfully sent asynchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_send_msg_async(sv_sms_msg_t *p_msg, int *id, sv_sms_msg_async_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SMS message reception callback hanlder.
  @param[in] cb message reception callback handler.
  @return Whether the message reception callback hanlder was set successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_msg_recv_cb(sv_sms_msg_recv_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends PDU synchronously.
  @param[in] p_pdu SMS PDU.
  @return Whether the PDU was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_send_pdu(sv_sms_pdu_t *p_pdu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends PDU asynchronously.
  @param[in] p_pdu sms pdu.
  @param[out] id id for this async operation.
  @param[in] cb async callback.
  @return Whether the PDU was successfully sent asynchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_send_pdu_async(sv_sms_pdu_t *p_pdu, int *id, sv_sms_pdu_async_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SMS PDU reception callback hanlder.
  @param[in] cb PDU reception callback handler.
  @return Whether the PDU reception callback hanlder was set successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_pdu_recv_cb(sv_sms_pdu_recv_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is SV_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_service_error_cb(sv_sms_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Binds the current control point to a specific subscription.
  @param[in] sub  Subscription type.
  @return Whether the subscription was successfully bound.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_bind_subscription(SV_SMS_SUBSCRIPTION_E sub);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set sms white-list enable.
  @param[in] enable (TRUE or FALSE).
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_white_list_enable(uint8_t enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get sms white-list enable.
  @param[OUT] enable_ptr (TRUE or FALSE).
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_get_white_list_enable(uint8_t *enable_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  added a sms number to white-list.
  @param[in] num_ptr pointer to sms number.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_white_list_add_num(char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete a sms number from white-list.
  @param[in] num_ptr pointer to sms number.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_white_list_del_num(char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete all sms number from white-list.
  @param[in] num_ptr pointer to sms number.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_set_white_list_del_all(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get all sms white-list.
  @param[OUT] info_ptr pointer to all sms white-list number.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sms_get_white_list_all(sv_sms_white_list_info_t *info_ptr);
#ifdef __cplusplus
}
#endif

#endif  /* __SV_SMS_H__ */
