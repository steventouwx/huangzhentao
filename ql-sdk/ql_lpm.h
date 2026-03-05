/**
 * @file ql_lpm.h
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

#ifndef __QL_LPM_H__
#define __QL_LPM_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int first;
	int src_node;
	int src_port;
	char data[64];
} ql_ipc_msg_t;

typedef struct {
	int first;
	char dev_name[16];
	char src_ip[48];
	char dst_ip[48];
} ql_network_msg_t;

typedef struct {
	char label[32];
	int first;
	int irq;
	char irq_name[64];
	ql_ipc_msg_t ipc_msg;
	ql_network_msg_t network_msg;
	void *privateData;
} ql_wakeup_info_t;


/**
 * enum call state
 */
typedef enum
{
	E_QL_LPM_FALLING = 0, /**< Falling, Means wakeupin falling to wakeup the module, or wakeupout falling to wakeup mcu. */
	E_QL_LPM_RISING	= 1, /**< Rising, Means  wakeupin rising to wakeup the module,  or wakeupout rising to wakeup  mcu. */
	E_QL_LPM_TTY_WAKEUP = 2, /**< system wakeup by tty [msm_hs_wakeup] interrupt. */
	// follow is customized irq event
	E_QL_LPM_CUSTOMIZED_FIRST_FALLING = 3,
	E_QL_LPM_CUSTOMIZED_FIRST_RISING  = 4,
	E_QL_LPM_CUSTOMIZED_SEC_FALLING = 5,
	E_QL_LPM_CUSTOMIZED_SEC_RISING  = 6,
} ql_lpm_edge_t;

/**
 * Callback function registered via QL_Lpm_Init(),It will be called when wakeup_in state changed.
 * @param[in] lpm_edge: report the wakeup_in state.
 * @return no value
 */
typedef void (*ql_lpm_handler_t)
(
	ql_lpm_edge_t lpm_edge
);

/**
 * Callback function registered via ql_wakeup_report_init(),It will be called when system wakeup.
 * @param[in] lpm_wakeup_info: report wakeup source
 * @return no value
 */
typedef void (*ql_lpm_wakeup_report_handler_t)
(
	ql_wakeup_info_t *msg
);

/**************************************************************/
/**
 @brief Init QL LPM service function.
 @param[in] ql_wakeup_report_handler_t the callback handler.
 @return Whether the LPM service was initialized successfully.
 */
/**************************************************************/
extern void ql_lpm_wakeup_report_init(ql_lpm_wakeup_report_handler_t ql_lpm_report_handler);

/**************************************************************/
/**
 @brief Init QL LPM service function.
 @param[in] ql_lpm_handler the callback handler.
 @return Whether the LPM service was initialized successfully.
 @retval 0  success.
 @retval -1 failed.
 */
/**************************************************************/
extern int ql_lpm_init(ql_lpm_handler_t ql_lpm_handler);

/**************************************************************/
/**
 @brief Deinit QL LPM service function
 @return Whether the LPM service was deinitialized successfully.
 @retval 0  success.
 @retval -1 failed.
 */
/**************************************************************/
extern int ql_lpm_deinit(void);

#ifdef __cplusplus
}
#endif

#endif // __QL_LPM_H__

