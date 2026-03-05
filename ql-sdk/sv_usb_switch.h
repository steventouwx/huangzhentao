/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_usb_switch.h
 * 编写者: AutoAgent
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_usb_switch相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file sv_usb_switch.h 
  @brief ql usb switch head file. 
*/
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  Copyright (c) 2018 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  EDIT HISTORY
  This section contains comments describing changes made to the file.
  Notice that changes are listed in reverse chronological order.
  $Header: $
  when       who          what, where, why
  --------   ---          ----------------------------------------------------------
  20240318   ethan.yao    Add usb enabled interface
  20230424   qing.xu  Created .
-------------------------------------------------------------------------------------------------*/

#ifndef __SV_USB_SWITCH_H__
#define __SV_USB_SWITCH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SV_USB_SWITCH_OPERATE_ADD 1 // add the function config.
#define SV_USB_SWITCH_OPERATE_DEL 2 // delete the function config.

typedef enum {
	SV_USB_SWITCH_FUNC_NONE,
	SV_USB_SWITCH_FUNC_AT, // AT function, the actual function name is cser.dun.0
	SV_USB_SWITCH_FUNC_DM, // DM function, the actual function name is ffs.diag
	SV_USB_SWITCH_FUNC_GNSS, // GNSS function, the actual function name is cser.nmea.1
	SV_USB_SWITCH_FUNC_ECM, // ECM function, the actual function name is gsi.ecm
	SV_USB_SWITCH_FUNC_RNDIS, // RNDIS function, the actual function name is gsi.rndis
	SV_USB_SWITCH_FUNC_MBIM, // MBIM function, the actual function name is gsi.mbim
	SV_USB_SWITCH_FUNC_QDSS, // QDSS function, the actual function name is qdss.qdss_mdm
	SV_USB_SWITCH_FUNC_DPL, // DPL function, the actual function name is gsi.dpl
	SV_USB_SWITCH_FUNC_MAX,
}sv_usb_switch_func_type;

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  switch usb function.
  @param[in] name  switch usb funciton name, define in sv_usb_switch_func_type.
  @param[in] action  switch usb funciton action: SV_USB_SWITCH_OPERATE_ADD/SV_USB_SWITCH_OPERATE_DEL.
  @retval 0  if it ok
  @retval other  if it failed
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_usb_switch_func(sv_usb_switch_func_type name, unsigned char action);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  enable all usb port. It will take effect after reboot.
  @retval 0  if it ok
  @retval other  if it failed
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_enable_usb(void);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  disable all usb port. It will take effect after reboot.
  @retval 0  if it ok
  @retval other  if it failed
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_disable_usb(void);


#ifdef __cplusplus
}
#endif

#endif /* __SV_USB_SWITCH_H__ */
