/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_data_throughput.h 
  @brief Data Throughput API 
*/
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  Copyright (c) 2024 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  EDIT HISTORY
  This section contains comments describing changes made to the file.
  Notice that changes are listed in reverse chronological order.
  $Header: $
  when       who          what, where, why
  --------   ---          ----------------------------------------------------------
  20240222   ewen.li      Add Data Throughput API
-------------------------------------------------------------------------------------------------*/

#ifndef __QL_DATA_THROUGHPUT_H__
#define __QL_DATA_THROUGHPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    QL_DATA_THROUGHPUT_UPLINK = 0x01, /**< Uplink direction */
    QL_DATA_THROUGHPUT_DOWNLINK = 0x02, /**< Downlink direction */
}QL_DATA_THROUGHPUT_DIRECTION_E;

typedef enum
{
    QL_DATA_THROUGHPUT_REPORT_DISABLED = 0x00, /**< Throughput report status: disabled */
    QL_DATA_THROUGHPUT_REPORT_ENABLED = 0x01,  /**< Throughput report status: enabled */
}QL_DATA_THROUGHPUT_REPORT_STATUS_E;

typedef struct
{
    uint8_t report_status_valid; /**< Must be set to true if report_status is being passed */
    QL_DATA_THROUGHPUT_REPORT_STATUS_E report_status; /**< Reporting status */

    uint8_t actual_interval_valid; /**< Must be set to true if actual_interval is being passed */
    uint32_t actual_interval; /**< The actual interval at which throughput is generated, Unit: ms */
}ql_data_throughput_report_status_t;

typedef struct
{
    uint8_t rate_valid; /**< Must be set to true if rate is being passed */
    uint32_t rate; /**< The current rate, Unit: kbps */
}ql_data_throughput_info_t;

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Data Throughput service error callback function.

  @param[in]  error: Error code.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*ql_data_throughput_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This is a callback function for Data Throughput information.

  @param[in]  The direction of Data Throughput.
  @param[in]  The information of Data Throughput.

  @retval  void
  */
/*-----------------------------------------------------------------------------------------------*/
typedef void (*ql_data_throughput_ind_cb_f)(QL_DATA_THROUGHPUT_DIRECTION_E direction,
                                            ql_data_throughput_info_t *p_msg);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function initializes Data Throughput service.

  @return Whether the Data Throughput service was intialized successfully.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_SERVICE_NOT_READY Data Throughput service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.

  @note You must call this function before other functions can be used in this module.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables Data Throughput report function.

  @param[in]  The direction of Data Throughput
  @param[in]  The report interval of Data Throughput.Unit:ms

  @retval  QL_ERR_OK Successful execution.
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  QL_ERR_NOT_INIT: Failed execution. The Data Throughput service is not initialized.
  @retval  QL_ERR_SERVICE_ABORT: Failed execution due to Data Throughput service error.
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_report_enable(QL_DATA_THROUGHPUT_DIRECTION_E direction, uint32_t report_interval);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function disables Data Throughput report function.

  @param[in]  The direction of Data Throughput

  @retval  QL_ERR_OK Successful execution.
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  QL_ERR_NOT_INIT: Failed execution. The Data Throughput service is not initialized.
  @retval  QL_ERR_SERVICE_ABORT: Failed execution due to Data Throughput service error.
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_report_disable(QL_DATA_THROUGHPUT_DIRECTION_E direction);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function queries the report status of Data Throughput 

  @param[in]  The direction of Data Throughput
  @param[in]  The report status of Data Throughput

  @retval  QL_ERR_OK Successful execution.
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  QL_ERR_NOT_INIT: Failed execution. The Data Throughput service is not initialized.
  @retval  QL_ERR_SERVICE_ABORT: Failed execution due to Data Throughput service error.
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_report_status_query(QL_DATA_THROUGHPUT_DIRECTION_E direction, 
                                           ql_data_throughput_report_status_t *p_status);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers Data Throughput information callback function.

  @param[in]  The Data Throughput information callback function.

  @retval  QL_ERR_OK: Successful execution.
  @retval  QL_ERR_NOT_INIT: Failed execution. The Wi-Fi service is not initialized.
  @retval  QL_ERR_SERVICE_ABORT: Failed execution due to Wi-Fi service error.
  @retval  Other value: Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_ind_cb_set(ql_data_throughput_ind_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function queries the information of Data Throughput

  @param[in]  The direction of Data Throughput.
  @param[in]  The information of Data Throughput.

  @retval  QL_ERR_OK Successful execution.
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  QL_ERR_NOT_INIT: Failed execution. The Data Throughput service is not initialized.
  @retval  QL_ERR_SERVICE_ABORT: Failed execution due to Data Throughput service error.
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_info_query(QL_DATA_THROUGHPUT_DIRECTION_E direction,
                                  ql_data_throughput_info_t *p_info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function registers the Data Throughput service error callback function.

  @param[in]  Data Throughput service error callback function, executed only when the Data Throughput service exits abnormally.

  @retval  QL_ERR_OK: Successful execution.
  @retval  QL_ERR_NOT_INIT: Failed execution. The Data Throughput service is not initialized.
  @retval  QL_ERR_SERVICE_ABORT: Failed execution due to Data Throughput service error.
  @retval  Other value: Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_set_service_error_cb(ql_data_throughput_service_error_cb_f cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function deinitializes Data Throughput service.

  @retval  QL_ERR_OK: Successful execution.
           Other value: Failed execution. See ql_type.h for the error code.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_data_throughput_deinit(void);

#ifdef __cplusplus
}
#endif
#endif

