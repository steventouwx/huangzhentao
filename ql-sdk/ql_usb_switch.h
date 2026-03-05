/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_usb_switch.h 
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

#ifndef __QL_USB_SWITCH_H__
#define __QL_USB_SWITCH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define QL_USB_SWITCH_OPERATE_ADD 1 // add the function config.
#define QL_USB_SWITCH_OPERATE_DEL 2 // delete the function config.

typedef enum {
	QL_USB_SWITCH_FUNC_NONE,
	QL_USB_SWITCH_FUNC_AT, // AT function, the actual function name is cser.dun.0
	QL_USB_SWITCH_FUNC_DM, // DM function, the actual function name is ffs.diag
	QL_USB_SWITCH_FUNC_GNSS, // GNSS function, the actual function name is cser.nmea.1
	QL_USB_SWITCH_FUNC_ECM, // ECM function, the actual function name is gsi.ecm
	QL_USB_SWITCH_FUNC_RNDIS, // RNDIS function, the actual function name is gsi.rndis
	QL_USB_SWITCH_FUNC_MBIM, // MBIM function, the actual function name is gsi.mbim
	QL_USB_SWITCH_FUNC_QDSS, // QDSS function, the actual function name is qdss.qdss_mdm
	QL_USB_SWITCH_FUNC_DPL, // DPL function, the actual function name is gsi.dpl
	QL_USB_SWITCH_FUNC_MAX,
}ql_usb_switch_func_type;

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  switch usb function.
  @param[in] name  switch usb funciton name, define in ql_usb_switch_func_type.
  @param[in] action  switch usb funciton action: QL_USB_SWITCH_OPERATE_ADD/QL_USB_SWITCH_OPERATE_DEL.
  @retval 0  if it ok
  @retval other  if it failed
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_usb_switch_func(ql_usb_switch_func_type name, unsigned char action);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  enable all usb port. It will take effect after reboot.
  @retval 0  if it ok
  @retval other  if it failed
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_enable_usb(void);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  disable all usb port. It will take effect after reboot.
  @retval 0  if it ok
  @retval other  if it failed
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_disable_usb(void);


#ifdef __cplusplus
}
#endif

#endif /* __QL_USB_SWITCH_H__ */
