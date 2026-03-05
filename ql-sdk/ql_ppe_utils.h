/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_ppe_utils.h 
  @brief PPE service API 
*/
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  EDIT HISTORY
  This section contains comments describing changes made to the file.
  Notice that changes are listed in reverse chronological order.
  $Header: $
  when       who          what, where, why
  --------   ---          ----------------------------------------------------------
  20190508   tyler.kuang  Create.
-------------------------------------------------------------------------------------------------*/

#ifndef __QL_PPE_UTILS_H__
#define __QL_PPE_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Generate a PPE certificate and only need to call it once.
  @param[in] as appKey,provided by QIANXUN
  @param[in] ks appSecret,provided by QIANXUN
  @paran[in] pem_path Not currently used, please set to a NULL
  @retval 0 successful.
  @retval 1 failure.
  */
/*-----------------------------------------------------------------------------------------------*/

int ql_ppe_key_gen(const char *as, const char *ks, const char *pem_path);

#ifdef __cplusplus
}
#endif

#endif

