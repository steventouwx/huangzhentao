/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_sensor_utils.h 
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

#ifndef __QL_SENSOR_UTILS_H__
#define __QL_SENSOR_UTILS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**<  IMU Sensor Data Type*/
typedef enum
{
    QL_SENSOR_TYPE_ACC = 0,/**< Accelerometer. */
    QL_SENSOR_TYPE_GYU,/**< Gyroscope. */
    QL_SENSOR_TYPE_TEMP,/**< Temperature. */
} QL_SENSOR_TYPE_E;

/**<  IMU Sensor Data Struct*/
typedef struct
{
    int64_t timestamp;/**< in Nanosecond. */ 
    int is_buffer;/**< if is buffer data,0-no; 1-yes. */ 
    QL_SENSOR_TYPE_E type;/**< Sensor Type. */ 
    float data[3];          /**<sensor data, 0-x(or temp),1-y,2-z */
} ql_sensor_data_t;

/**
* define sensor callback function
*@param[out] *p_msg:sensor data struct
*@return no value
*/
typedef void (*ql_sensor_cb_f)(int index, ql_sensor_data_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Initializes sensor api.
  @param[in] void
  @return Whether to successfully initializes sensor interface 
  @retval QL_ERR_OK successful
  @retval Other error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sensor_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set the sensor data callback function
  @param[in] cb
  @return Whether to successfully registe sensor callback function 
  @retval QL_ERR_OK successful
  @retval Other error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_sensor_set_cb(ql_sensor_cb_f cb);
#ifdef __cplusplus
}
#endif

#endif

