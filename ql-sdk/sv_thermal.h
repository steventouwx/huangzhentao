/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_thermal.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_thermal相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_THERMAL_H__
#define __SV_THERMAL_H__
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
typedef void (*sv_thermal_msg_recv_cb_f)(char *mitigation_dev_id,uint8_t mitigation_level);

int sv_thermal_qmi_init(void);

/**
 * set the callback function to recv thermal  message when thermal mitigation level changed
 * @param[in] mitigation_dev_id: cooler name
 * @param[in] cb: callback function
 * @retval 0:successfull; -1:error
 */
int sv_thermal_set_msg_recv_cb(char *mitigation_dev_id,sv_thermal_msg_recv_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Deregister callback function "sv_thermal_set_msg_recv_cb"
  @param[in] mitigation_dev_id: cooler name
  @retval 0:successfull; -1:error
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_thermal_deregister_msg_recv_cb(char *mitigation_dev_id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief get thermal cooler's mitigation level.
  @param[in] mitigation_dev_id cooler's name
  @param[in] current_mitigation_level mitigation level of cooler
  @param[in] requested_mitigation_level requested mitigation level of cooler
  @retval 0:successfull; -1:error
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_thermal_qmi_get_mitigation_level(char *mitigation_dev_id,uint8_t *current_mitigation_level,uint8_t *requested_mitigation_level);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief set thermal cooler's mitigation level.
  @param[in] mitigation_dev_id cooler's name
  @param[in] mitigation_level mitigation level to cooler
  @retval 0:successfull; -1:error
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_thermal_qmi_set_mitigation_level(char *mitigation_dev_id,uint8_t mitigation_level);

#endif