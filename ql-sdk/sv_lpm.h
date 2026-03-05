/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_lpm.h
 * 编写者: AutoAgent
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_lpm相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/**
 * @file sv_lpm.h
 * @brief Low power manager API 
 *
 * @details
 * QuecOpen AG5xx series module low power manager service API
 *
 * @htmlonly 
 * <span style="font-weight: bold">History</span> 
 * @endhtmlonly
 *
 * when|who|why
 * ----------|--------------|-------------------------------------------
 * 20210908  |  ron.zhang   |Add low power manager service white-list APIS.
 *
 * @copyright Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
 * Quectel Wireless Solution Proprietary and Confidential.
 */

#ifndef __SV_LPM_H__
#define __SV_LPM_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int first;
	int src_node;
	int src_port;
	char data[64];
} sv_ipc_msg_t;

typedef struct {
	int first;
	char dev_name[16];
	char src_ip[48];
	char dst_ip[48];
} sv_network_msg_t;

typedef struct {
	char label[32];
	int first;
	int irq;
	char irq_name[64];
	sv_ipc_msg_t ipc_msg;
	sv_network_msg_t network_msg;
	void *privateData;
} sv_wakeup_info_t;


/**
 * enum call state
 */
typedef enum
{
	E_SV_LPM_FALLING = 0, /**< Falling, Means wakeupin falling to wakeup the module, or wakeupout falling to wakeup mcu. */
	E_SV_LPM_RISING	= 1, /**< Rising, Means  wakeupin rising to wakeup the module,  or wakeupout rising to wakeup  mcu. */
	E_SV_LPM_TTY_WAKEUP = 2, /**< system wakeup by tty [msm_hs_wakeup] interrupt. */
	// follow is customized irq event
	E_SV_LPM_CUSTOMIZED_FIRST_FALLING = 3,
	E_SV_LPM_CUSTOMIZED_FIRST_RISING  = 4,
	E_SV_LPM_CUSTOMIZED_SEC_FALLING = 5,
	E_SV_LPM_CUSTOMIZED_SEC_RISING  = 6,
} sv_lpm_edge_t;

/**
 * Callback function registered via SV_Lpm_Init(),It will be called when wakeup_in state changed.
 * @param[in] lpm_edge: report the wakeup_in state.
 * @return no value
 */
typedef void (*sv_lpm_handler_t)
(
	sv_lpm_edge_t lpm_edge
);

/**
 * Callback function registered via sv_wakeup_report_init(),It will be called when system wakeup.
 * @param[in] lpm_wakeup_info: report wakeup source
 * @return no value
 */
typedef void (*sv_lpm_wakeup_report_handler_t)
(
	sv_wakeup_info_t *msg
);

/**************************************************************/
/**
 @brief Init QL LPM service function.
 @param[in] sv_wakeup_report_handler_t the callback handler.
 @return Whether the LPM service was initialized successfully.
 */
/**************************************************************/
extern void sv_lpm_wakeup_report_init(sv_lpm_wakeup_report_handler_t sv_lpm_report_handler);

/**************************************************************/
/**
 @brief Init QL LPM service function.
 @param[in] sv_lpm_handler the callback handler.
 @return Whether the LPM service was initialized successfully.
 @retval 0  success.
 @retval -1 failed.
 */
/**************************************************************/
extern int sv_lpm_init(sv_lpm_handler_t sv_lpm_handler);

/**************************************************************/
/**
 @brief Deinit QL LPM service function
 @return Whether the LPM service was deinitialized successfully.
 @retval 0  success.
 @retval -1 failed.
 */
/**************************************************************/
extern int sv_lpm_deinit(void);

#ifdef __cplusplus
}
#endif

#endif // __SV_LPM_H__

