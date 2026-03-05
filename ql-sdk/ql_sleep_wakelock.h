/**
 * @file ql_sleep_wakelock.h
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
#ifndef __QL_SLEEP_WAKELOCK_H__
#define __QL_SLEEP_WAKELOCK_H__

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
  @retval QL_ERR_INVALID_ARG - Invalid argument
  @retval QL_ERR_FAILED - failed
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int ql_slp_wakelock_create(const char *name, size_t len);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Lock the wake lock
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Wake up lock Check whether the lock is successfully enabled
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - Invalid argument
  @retval QL_ERR_FAILED - failed
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int ql_slp_wakelock_lock(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Unlock the wake lock
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Whether the wake lock is successfully unlocked
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - Invalid argument
  @retval QL_ERR_FAILED - failed
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int ql_slp_wakelock_unlock(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get wake lock status
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Whether the wake lock status is obtained successfully
  @retval QL_ERR_INVALID_ARG - Invalid argument
  @retval QL_ERR_FAILED - Get wakelock status failed
  @retval 0 - Unlock wakelock status
  @retval 1 - Lock wakelock status
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int ql_slp_wakelock_status(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Destroy wake lock
  @param[in] fd Descriptor fd returned when the wake lock is created
  @return Whether the wake lock was destroyed successfully
  @retval QL_ERR_OK - successful
  @retval QL_ERR_INVALID_ARG - Invalid argument
  @retval QL_ERR_FAILED - failed
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int ql_slp_wakelock_destroy(int fd);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief enable the autosleep node for lpm
  @param[in] enable Enable or disable autosleep
  @return Whether the autosleep node is set successfully
  @retval QL_ERR_OK - successful
  @retval QL_ERR_FAILED - failed
  @retval Other - error code defined by ql_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
extern int ql_autosleep_enable(char enable);

#ifdef __cplusplus
}
#endif

#endif  //__QL_SLEEP_WAKELOCK_H__