/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_sleep_wakelock.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_sleep_wakelock相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/**
 * @file sv_sleep_wakelock.h
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
 * ----------|-----------|----------------------------------------------
 * 20230523  |  quectel  | Add low power manager service white-list APIS.
 *
 * @copyright Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
 * Quectel Wireless Solution Proprietary and Confidential.
 */
#ifndef __SV_SLEEP_WAKELOCK_H__
#define __SV_SLEEP_WAKELOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Create wake lock
  @param[in] name wake lock name
  @param[in] len wake lock len
  @return Whether the wake lock is created successfully
  @retval fd - successful
  @retval SV_ERR_INVALID_ARG - Invalid argument
  @retval SV_ERR_FAILED - failed
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int sv_slp_wakelock_create(const char *name, size_t len);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Lock the wake lock
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Wake up lock Check whether the lock is successfully enabled
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - Invalid argument
  @retval SV_ERR_FAILED - failed
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int sv_slp_wakelock_lock(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Unlock the wake lock
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Whether the wake lock is successfully unlocked
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - Invalid argument
  @retval SV_ERR_FAILED - failed
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int sv_slp_wakelock_unlock(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get wake lock status
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Whether the wake lock status is obtained successfully
  @retval SV_ERR_INVALID_ARG - Invalid argument
  @retval SV_ERR_FAILED - Get wakelock status failed
  @retval 0 - Unlock wakelock status
  @retval 1 - Lock wakelock status
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int sv_slp_wakelock_status(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Destroy wake lock
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Whether the wake lock was destroyed successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_INVALID_ARG - Invalid argument
  @retval SV_ERR_FAILED - failed
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int sv_slp_wakelock_destroy(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief enable the autosleep node for lpm
  @param[in] enable Enable or disable autosleep
  @return Whether the autosleep node is set successfully
  @retval SV_ERR_OK - successful
  @retval SV_ERR_FAILED - failed
  @retval Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int sv_autosleep_enable(char enable);

#ifdef __cplusplus
}
#endif

#endif  //__SV_SLEEP_WAKELOCK_H__