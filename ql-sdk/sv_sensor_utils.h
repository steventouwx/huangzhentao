/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_sensor_utils.h
 * 编写者: AutoAgent
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_sensor_utils相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file sv_sensor_utils.h 
  @brief Get sensor data used by QDR

  @detailes
  Quectel AG55x series module IMU sensor API.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly
  
  when     |  who      |    what, where, why
  -------- |  ---      |    ----------------------------------------------------------
  20201030 |  raigor.zhao  |    Add temp data .
  20190715 |  tyler.kuang  |    Created .
  
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

#ifndef __SV_SENSOR_UTILS_H__
#define __SV_SENSOR_UTILS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**<  IMU Sensor Data Type*/
typedef enum
{
    SV_SENSOR_TYPE_ACC = 0,/**< Accelerometer. */
    SV_SENSOR_TYPE_GYU,/**< Gyroscope. */
    SV_SENSOR_TYPE_TEMP,/**< Temperature. */
} SV_SENSOR_TYPE_E;

/**<  IMU Sensor Data Struct*/
typedef struct
{
    int64_t timestamp;/**< in Nanosecond. */ 
    int is_buffer;/**< if is buffer data,0-no; 1-yes. */ 
    SV_SENSOR_TYPE_E type;/**< Sensor Type. */ 
    float data[3];          /**<sensor data, 0-x(or temp),1-y,2-z */
} sv_sensor_data_t;

/**
* define sensor callback function
*@param[out] *p_msg:sensor data struct
*@return no value
*/
typedef void (*sv_sensor_cb_f)(int index, sv_sensor_data_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Initializes sensor api.
  @param[in] void
  @return Whether to successfully initializes sensor interface 
  @retval SV_ERR_OK successful
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sensor_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set the sensor data callback function
  @param[in] cb
  @return Whether to successfully registe sensor callback function 
  @retval SV_ERR_OK successful
  @retval Other error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_sensor_set_cb(sv_sensor_cb_f cb);
#ifdef __cplusplus
}
#endif

#endif

