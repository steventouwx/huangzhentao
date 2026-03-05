/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_ms_sms.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_ms_sms相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/**
 * @file sv_ms_sms.h
 * @brief Sms service API.
 *
 * @details 
 * QuecOpen AG550Q&AG551Q series module sms service. support Dual Sim Dual Active(DSDA). 
 *
 * @htmlonly 
 * <span style="font-weight: bold">History</span> 
 * @endhtmlonly
 *
 * when|who|why
 * ----------|--------------|-------------------------------------------
  2021069    |Stan.li         |Created.
 *
 * @copyright Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
 * Quectel Wireless Solution Proprietary and Confidential.
 * 
 */
#ifndef __SV_MS_SMS_H__
#define __SV_MS_SMS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "sv_type.h"
#include "sv_sms_common.h"

/**
 * recv SMS messages asynchronously error callback function
 * @param[in] sim_id sim card id.
 * @param[in] id: ID of the event that sends SMS messages asynchronously.
 * @param[in] result: Result of sending SMS messages asynchronously
 * @return no value
 */
typedef void (*sv_ms_sms_msg_async_cb_f)(int sim_id, int id, int result);

/**
 * send SMS messages asynchronously error callback function
 * @param[in] sim_id sim card id.
 * @param[in] p_msg: Text messages
 * @param[in] timestamp: SMS timestamp
 * @param[in] head: Long message header information
 * @return no value
 */
typedef void (*sv_ms_sms_msg_recv_cb_f)(int sim_id, sv_sms_msg_t *p_msg, sv_sms_timestamp_t *timestamp,
                                             sv_sms_user_data_head_t *head);

/**
 * send PDU short messages asynchronously callback function
 * @param[in] sim_id sim card id.
 * @param[in] id: ID of the event that PDU SMS messages are sent asynchronously
 * @param[in] result: Result of sending PDU SMS messages asynchronously
 * @return no value
 */
typedef void (*sv_ms_sms_pdu_async_cb_f)(int sim_id, int id, int result);

/**
 * recv PDU short messages callback function
 * @param[in] sim_id sim card id.
 * @param[in] p_pdu: PDU SMS message
 * @return no value
 */
typedef void (*sv_ms_sms_pdu_recv_cb_f)(int sim_id, sv_sms_pdu_t *p_pdu);

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
int sv_ms_sms_init(void);

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
int sv_ms_sms_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set sms white-list enable.
  @param[in] sim_id sim id.
  @param[in] enable (TRUE or FALSE).
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_set_white_list_enable(int sim_id, uint8_t enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get sms white-list enable.
  @param[in] sim_id sim id.
  @param[OUT] enable_ptr (TRUE or FALSE).
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_get_white_list_enable(int sim_id, uint8_t *enable_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  added a sms number to white-list.
  @param[in] sim_id sim id.
  @param[in] num_ptr pointer to sms number.
  @param[in] len sms number length.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_set_white_list_add_num(int sim_id, char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete a sms number from white-list.
  @param[in] sim_id sim id.
  @param[in] num_ptr pointer to sms number.
  @param[in] len sms number length.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_set_white_list_del_num(int sim_id, char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete all sms number from white-list.
  @param[in] sim_id sim id.
  @param[in] num_ptr pointer to sms number.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_set_white_list_del_all(int sim_id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get all sms white-list.
  @param[in] sim_id sim id.
  @param[OUT] info_ptr pointer to all sms white-list number.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_get_white_list_all(int sim_id, sv_sms_white_list_info_t *info_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets the service center address.
  @param[in] sim_id sim_id to be used.
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
int sv_ms_sms_set_service_center_addr(int sim_id, char *addr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets the service center address.
  @param[in] sim_id sim_id to be used.
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
int sv_ms_sms_get_service_center_addr(int sim_id, char *addr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends message synchronously.
  @param[in] sim_id sim_id to be used.
  @param[in] p_msg pointer to sv_ms_sms_msg_t.
  @return Whether the message was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_send_msg(int sim_id, sv_sms_msg_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends message asynchronously.
  @param[in] sim_id sim_id to be used.
  @param[in] p_msg  pointer to sv_ms_sms_msg_t
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
int sv_ms_sms_send_msg_async(int sim_id, sv_sms_msg_t *p_msg, int *id, sv_ms_sms_msg_async_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SMS message reception callback hanlder.
  @param[in] sim_id sim_id to be used.
  @param[in] cb message reception callback handler.
  @return Whether the message reception callback hanlder was set successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_set_msg_recv_cb(int sim_id, sv_ms_sms_msg_recv_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends PDU synchronously.
  @param[in] sim_id sim_id to be used.
  @param[in] p_pdu SMS PDU.
  @return Whether the PDU was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_send_pdu(int sim_id, sv_sms_pdu_t *p_pdu);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends PDU asynchronously.
  @param[in] sim_id sim_id to be used.
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
int sv_ms_sms_send_pdu_async(int sim_id, sv_sms_pdu_t *p_pdu, int *id, sv_ms_sms_pdu_async_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets SMS PDU reception callback hanlder.
  @param[in] sim_id sim_id to be used.
  @param[in] cb PDU reception callback handler.
  @return Whether the PDU reception callback hanlder was set successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_sms_set_pdu_recv_cb(int sim_id, sv_ms_sms_pdu_recv_cb_f cb);

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
int sv_ms_sms_set_service_error_cb(sv_sms_service_error_cb_f cb);

#ifdef __cplusplus
}
#endif


#endif

