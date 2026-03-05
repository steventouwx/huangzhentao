/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_voice.h
 * 编写者: AutoAgent
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_voice相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/**
 * @file sv_voice.h
 * @brief Voice service API
 *
 * @detailes
 * EDIT HISTORY
 * This section contains comments describing changes made to the file.
 * Notice that changes are listed in reverse chronological order.
 *
 * $Header: $
 * when      |what, where, why
 * ----------|---------------------------------------------------------
 * 20190625  |Created
 */

#ifndef __SV_VOICE_H__
#define __SV_VOICE_H__

#include "sv_voice_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Initializes voice service.
  @return Whether the voice service was initialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Deinitializes voice service.
  @return Whether the voice service was deinitialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Dials a call.
  @param[in] num    phone number to dial.
  @param[in] len    length of phone number, should be less than 
                    or euqnal to SV_VOICE_MAX_PHONE_NUMBER.
  @param[out] id    call id.
  @return Whether a voice call was successfully dialed.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_dial(char *num, int len, uint32_t *id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Cancels dialing with given id.
  @param[in] id call id returned from dial.
  @return Whether the voice call was successfully cancelled.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_cancel_dial(uint32_t id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  hangup all dialing.
  @return Whether all voice call were successfully hangup.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_hangup_all(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Answers the call.
  @param[in] id call id returned from dial.
  @return Whether the voice call was successfully answered.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_answer(uint32_t id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Hangs up the call.
  @param[in] id call id returned from dial.
  @return Whether the voice call was successfully hung up.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_hangup(uint32_t id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Holds the call when mutil calls is activated.
  @param[in] id call id returned from dial.
  @return Whether the voice call was successfully held.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_hold(uint32_t id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Releases the call from hold when mutil calls is activated.
  @param[in] id call id returned from dial.
  @return Whether the voice call was successfully unheld.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_unhold(uint32_t id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Joins the call conference.
  @return Whether to join the voice call conference successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_join_conference(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Leaves the call conference.
  @return Whether to leave the voice call conference successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_leave_conference(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets call records.
  @param[in] p_arr  pointer to sv_voice_record_array_t.
  @return Whether the call records were successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_get_records(sv_voice_record_array_t *p_arr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Registers or Unregisters forwarding.
  @param[in] reg    0 - unregister, 1 - register.
  @param[in] cond   forwarding condition.
  @param[in] num    phone number.
  @param[in] len    length of phone numebr.
  @return Whether the voice call forward was registered or unregistered successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_forwarding(int reg, SV_VOICE_FW_COND_E cond, char *num, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets forwarding status.
  @param[in]  cond      forwarding condition.
  @param[out] p_status  pointer to sv_voice_fw_status_t.
  @return Whether the voice call forward status was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_get_forwarding_status(SV_VOICE_FW_COND_E cond, sv_voice_fw_status_t *p_status);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Enables or disables call waiting.
  @param[in] enable 0 - disable, other - enable.
  @return Whether the voice call waiting was enabled or disabled successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_waiting(int enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets call waiting status.
  @param[out] enabled 0 - waiting is disabled, 1 - waiting is enabled.
  @return Whether the voice call waiting status was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_get_waiting_status(int *enabled);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Enables or disables auto answer.
  @param[in] enable 0 - disable, other - enable.
  @param[in] sec    wait this `sec' seconds before auto answer.
  @return Whether the voice call autoanswer was enabled or disabled successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_autoanswer(int enable, uint32_t sec);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sends a DTMF(Dual Tone Multi Frequency) character over the call ID.
  @param[in] id call id returned from dial.
  @param[in] c DTMF character to be sent. Valid DTMF characters are 0-9, A-D, '*', '#'.
  @return Whether a DTMF character was successfully sent.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_send_dtmf_char(uint32_t id, char c);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets voice call callback handler.
  @param[in] cb call back handler.
  @return Whether the voice call callback handler was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_set_call_cb(sv_voice_call_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets voice dtmf callback handler.
  @param[in] cb call back handler.
  @return Whether the voice call DTMF repcetion callback handler was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_set_dtmf_cb(sv_voice_dtmf_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Dials eCall.
  @param[in] p_info eCall info.
  @return Whether a eCall was successfully dialed.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_dial(sv_voice_ecall_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Hangs up eCall.
  @return Whether the eCall was successfully hung up.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_hangup(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Updates eCall MSD.
  @param[in] msd Minimum Set of Data.
  @param[in] msd_len Length of Minimum Set of Data.
  @return Whether the eCall MSD was successfully updated.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_update_msd(const uint8_t *msd, uint32_t msd_len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Pushes eCall MSD.
  @param[out] state eCall state.
  @return Whether the eCall MSD was successfully pushed.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_push_msd(SV_VOICE_ECALL_STATE_E *state);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Gets eCall config.
  @param[in] p_config eCall config.
  @return Whether the eCall config was successfully obtained.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_get_config(sv_voice_ecall_config_t *p_config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets eCall config.
  @param[in] item Items to set.
  @param[in] p_config eCall config.
  @return Whether the eCall config was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_set_config(int item, sv_voice_ecall_config_t *p_config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets eCall event callback handler.
  @param[in] cb call back handler.
  @return Whether the eCall event callback handler was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_set_event_cb(sv_voice_ecall_event_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets eCall status callback handler.
  @param[in] cb call back handler.
  @return Whether the eCall status callback handler was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_ecall_set_status_cb(sv_voice_ecall_status_cb_f cb);

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
int sv_voice_set_service_error_cb(sv_voice_service_error_cb_f cb);

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
int sv_voice_bind_subscription(SV_VOICE_SUBSCRIPTION_E sub);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Send USSD messages.
  @param[in] pt_ussd ussd msg.
  @return Whether a USSD msg successfully sent.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_send_ussd(sv_voice_ussd_info_t *pt_ussd);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Cancel USSD messages.
  @return Whether a cancel USSD msg send.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_cancel_ussd(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Sets ussd callback handler.
  @param[in] cb recv ussd back handler.
  @return Whether the recv ussd callback handler was successfully set.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_set_recv_ussd_cb(sv_voice_ussd_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set voice white-list enable.
  @param[in] enable (TRUE or FALSE).
  @return Whether the white list set was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_set_white_list_enable(uint8_t enable);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get voice white-list enable.
  @param[OUT] enable_ptr (TRUE or FALSE).
  @return Whether the white list get was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_get_white_list_enable(uint8_t *enable_ptr);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  added a voice number to white-list.
  @param[in] num_ptr pointer to voice number.
  @return Whether the white list add was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_set_white_list_add_num(char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete a voice number from white-list.
  @param[in] num_ptr pointer to voice number.
  @return Whether the white list delete was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_set_white_list_del_num(char *num_ptr, int len);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  delete all voice number from white-list.
  @param[in] num_ptr pointer to voice number.
  @return Whether the white list delete all was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_set_white_list_del_all(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get all voice white-list.
  @param[OUT] info_ptr pointer to all voice white-list number.
  @return Whether the white list get all was successfully sent synchronously.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_UNKNOWN unknown error, failed to connect to service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_voice_get_white_list_all(sv_voice_white_list_info_t *info_ptr);

#ifdef __cplusplus
}
#endif

#endif  /* __SV_VOICE_H__ */

