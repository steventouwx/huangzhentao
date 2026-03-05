/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_can.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_can相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file sv_can.h 
  @brief Interface for CAN api

  @detailes
  Quectel AG55x series module gnss service.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly
  
  when     |  who      |    what, where, why
  -------- |  ---      |    ----------------------------------------------------------
  20191209 |  tyler.kuang  |    Created .
  
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/


#ifndef __SV_CAN_H__
#define __SV_CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Check can service initialization is ready. Note that the following APIs can only be invoked 
  after the can service is ready.
  @return 
  @retval SV_ERR_OK - CAN service is ready
  @retval SV_ERR_SERVICE_NOT_READY - CAN service is not ready, need to wait and recheck
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_is_ready(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Inject speed information to CAN service (QDR). Note that the speed unit is m/h
  @param[in] timestamp Timestamp in ms
  @param[in] data Speed info in M/H
  @return 
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_send_speed(int64_t timestamp, uint32_t data);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Inject gear info to CAN service (QDR).
  @param[in] timestamp Timestamp in ms
  @param[in] data Gear info, 0-Park, 1-Neutral, 3-Reverse, 4-Forward
  @return 
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_send_gear(int64_t timestamp, uint32_t data);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Inject speed info bufferd in MCU or kernel to CAN service (QDR). Note that speed unit is M/H
  @param[in] timestamp Timestamp in ms
  @param[in] data Speed info in M/H
  @return 
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_send_speed_buffer(int64_t timestamp, uint32_t data);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Inject gear info bufferd in MCU or kernel to CAN service (QDR).
  @param[in] timestamp Timestamp in ms
  @param[in] data Gear info, 0-Park, 1-Neutral, 3-Reverse, 4-Forward
  @return 
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_send_gear_buffer(int64_t timestamp, uint32_t data);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Inject suspend signal to CAN service, CAN service will save key info to ROM
  @param[in] timestamp Timestamp in ms
  @param[in] data Reserved, not used
  @return 
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_send_suspend(int64_t timestamp, uint32_t data);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Inject resume signal to CAN service
  @param[in] timestamp Timestamp in ms
  @param[in] data Reserved, not used
  @return 
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_send_resume(int64_t timestamp, uint32_t data);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Inject shutdown signal to CAN service, CAN service will save key info to ROM, 
  then system will shut down
  @param[in] timestamp Timestamp in ms
  @param[in] data Reserved, not used
  @return 
  @retval SV_ERR_OK - successful
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_can_send_shutdown(int64_t timestamp, uint32_t data);

#ifdef __cplusplus
}
#endif

#endif

