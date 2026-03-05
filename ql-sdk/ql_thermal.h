/**
 * @file ql_thermal.h
 * @brief thermal mitigation function API.
 *
 * @details 
 * QuecOpen AG59x series module sim service. 
 *
 * @htmlonly 
 * <span style="font-weight: bold">History</span> 
 * @endhtmlonly
 *
 * when|who|why
 * ----------|--------------|-------------------------------------------
  20231124   |trigger.xu    |Created .
 *
 * @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
 * Quectel Wireless Solution Proprietary and Confidential.
 * 
 */

#ifndef __QL_THERMAL_H__
#define __QL_THERMAL_H__
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * recv thermal  message when thermal mitigation level changed
 * @param[in] mitigation_dev_id: cooler name
 * @param[in] mitigation_level: The mitigation level to which the cooler switches
 * @return no value
 */
typedef void (*ql_thermal_msg_recv_cb_f)(char *mitigation_dev_id,uint8_t mitigation_level);

int ql_thermal_qmi_init(void);

/**
 * set the callback function to recv thermal  message when thermal mitigation level changed
 * @param[in] mitigation_dev_id: cooler name
 * @param[in] cb: callback function
 * @retval 0:successfull; -1:error
 */
int ql_thermal_set_msg_recv_cb(char *mitigation_dev_id,ql_thermal_msg_recv_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Deregister callback function "ql_thermal_set_msg_recv_cb"
  @param[in] mitigation_dev_id: cooler name
  @retval 0:successfull; -1:error
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_thermal_deregister_msg_recv_cb(char *mitigation_dev_id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get thermal cooler's mitigation level.
  @param[in] mitigation_dev_id cooler's name
  @param[in] current_mitigation_level mitigation level of cooler
  @param[in] requested_mitigation_level requested mitigation level of cooler
  @retval 0:successfull; -1:error
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_thermal_qmi_get_mitigation_level(char *mitigation_dev_id,uint8_t *current_mitigation_level,uint8_t *requested_mitigation_level);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set thermal cooler's mitigation level.
  @param[in] mitigation_dev_id cooler's name
  @param[in] mitigation_level mitigation level to cooler
  @retval 0:successfull; -1:error
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_thermal_qmi_set_mitigation_level(char *mitigation_dev_id,uint8_t mitigation_level);

#endif