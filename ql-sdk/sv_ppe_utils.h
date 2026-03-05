/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_ppe_utils.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_ppe_utils相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file sv_ppe_utils.h 
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

#ifndef __SV_PPE_UTILS_H__
#define __SV_PPE_UTILS_H__

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

int sv_ppe_key_gen(const char *as, const char *ks, const char *pem_path);

#ifdef __cplusplus
}
#endif

#endif

