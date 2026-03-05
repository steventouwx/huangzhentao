/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: gpioSysfs.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义gpioSysfs相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file gpioSysfs.h
  @brief gpio API
*/
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  EDIT HISTORY
  This section contains comments describing changes made to the file.
  Notice that changes are listed in reverse chronological order.
  $Header: $
  when                     who                                          what, where, why
  --------             ---                                            ----------------------------------------------------------
  15/07/2016      Stanley.YONG  		   Create
  26/07/2016      Stanley.YONG   		   Add interrupt implementation.
  05/12/2019      larry.zhang		             Code  normalization
  11/13/2020      larry.zhang                           Modify the code format
-------------------------------------------------------------------------------------------------*/

#ifndef __GPIOSYSFS_H__
#define __GPIOSYSFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "sv_gpio.h"
#include <errno.h>

#define off64_t long long

#define boolean unsigned int
#define PAGE_SIZE  4096
typedef void (*_fdMonitor_CB)(ENUM_PINNAME  pin_name, boolean activeLevel, void* param);	/**  */

enum {
        RES_OK = 0,
        RES_BAD_PARAMETER  = -1,     ///< Parameter is invalid.
        RES_IO_NOT_SUPPORT = -2,
        RES_IO_ERROR = -3,
        RES_NOT_IMPLEMENTED = -4
};

/*-----------------------------------------------------------------------------------------------*/
/**
    The value of GPIO low or high.
    Translates to the setting of "value" in sysfs
    */
/*-----------------------------------------------------------------------------------------------*/
typedef enum {
    GPIO_LEVEL_LOW,   /** GPIO Low Value */
    GPIO_LEVEL_HIGH   /** GPIO High Value */
}ENUM_GPIO_LEVEL;

/*-----------------------------------------------------------------------------------------------*/
/**
    The polarity of GPIO level low or high.
    Translates to the setting of "active_low" in sysfs
    */
/*-----------------------------------------------------------------------------------------------*/
typedef enum {
    POLARITY_LEVEL_HIGH,    /** GPIO Active-High, output signal is 1*/
    POLARITY_LEVEL_LOW      /**  GPIO Active-Low, output signal is 0*/
}ENUM_GPIO_POLARITY_LEVEL;

/*-----------------------------------------------------------------------------------------------*/
/**
    The polarity of GPIO direction.
    Translates to the setting of "direction" in sysfs
    */
/*-----------------------------------------------------------------------------------------------*/
typedef enum {
    GPIO_DIR_INPUT,  /** GPIO input direction */
    GPIO_DIR_OUTPUT  /** GPIO output direction*/
}ENUM_GPIO_DIR;

/*-----------------------------------------------------------------------------------------------*/
/**
    The polarity of GPIO interrupt awake to system.
    Translates to the setting of "awake" in sysfs
    */
/*-----------------------------------------------------------------------------------------------*/
typedef enum {
    GPIO_AWAKE_DISABLE,  /** GPIO awake disable */
    GPIO_AWAKE_ENABLE    /** GPIO awake enable*/
}ENUM_GPIO_AWAKE;


/*-----------------------------------------------------------------------------------------------*/
/**
     The mode of GPIO Edge Sensivity.
     */
/*-----------------------------------------------------------------------------------------------*/
typedef enum {
    GPIO_EDGE_SENSE_NONE,    /** pin is input, but no an interrupt pin */
    GPIO_EDGE_SENSE_RISING,  /** Rising edge trigger mode */
    GPIO_EDGE_SENSE_FALLING, /** Falling edge trigger mode */
    GPIO_EDGE_SENSE_BOTH     /** Falling and Rising edge trigger mode  */
}ENUM_GPIO_EDGE_SENSE_MODE;


/*-----------------------------------------------------------------------------------------------*/
/**
     The struct of Sysfs object
      */
/*-----------------------------------------------------------------------------------------------*/
typedef struct {
    ENUM_PINNAME pin_name;       /** GPIO enumeration arrey*/
    unsigned int gpio_num;       /** Pin GPIO number */
    const char gpio_name[10];    /** Pin GPIO Signal Name */ 
    boolean in_use;              /**  Is the GPIO currently used*/
    int monitorfd;               /**The FD of the file bing monitored*/
    _fdMonitor_CB fdMonitor_proc;/** fdMonitor_cb Object associated to this GPIO*/       
    void* param_cb;              /** the parameter can be passed into fdMonitor_cb*/
}st_gpio_sysfs;


/*-----------------------------------------------------------------------------------------------*/
/**
     Export a GPIO in the sysfs.
     @brief
         @param[in] pin_name  GPIO enumeration arrey
     @return
       - RES_OK if exporting was successful
       - RES_IO_NOT_SUPPORT pin is not supported
       - RES_IO_ERROR if it failed
      */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_export(const ENUM_PINNAME pin_name);

/*-----------------------------------------------------------------------------------------------*/
/**
    Unexport a GPIO in the sysfs.
     @brief
         @param[in] pin_name  GPIO enumeration arrey
     @return
      - RES_OK if exporting was successful
      - RES_IO_NOT_SUPPORT pin is not supported
      - RES_IO_ERROR if it failed
    */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_unexport(const ENUM_PINNAME pin_name);

/*-----------------------------------------------------------------------------------------------*/
/**
     setup GPIO Direction INPUT or OUTPUT mode.
     @brief
        @param[in] pin_name  GPIO enumeration arrey
        @param[in] direction   reads as either "in" or "out". This value may
         normally be written. Writing as "out" defaults to
         initializing the value as low. To ensure glitch free
         operation, values "low" and "high" may be written to
         configure the GPIO as an output with that initial value
     @return
      - RES_OK  setting direction was successful
      - RES_IO_NOT_SUPPORT pin is not supported
      - RES_IO_ERROR if it failed
   */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_write_direction(ENUM_PINNAME  pin_name,   ENUM_GPIO_DIR dir );

/*-----------------------------------------------------------------------------------------------*/
/**
      PINname get gpio
       @brief
         @param[in] pin_name  GPIO enumeration arrey
       @return
         -st_gpio_sysfs*  if it ok
         -NULL  if it failed
      */
/*-----------------------------------------------------------------------------------------------*/
st_gpio_sysfs* gpio_sysfs_get_gpio_item_by_pin(const ENUM_PINNAME pin_name);


/*-----------------------------------------------------------------------------------------------*/
/**
      read GPIO Direction INPUT or OUTPUT mode.
     @brief
        @param[in] pin_name  GPIO enumeration arrey
        @param[in] direction  reads as either "in" or "out". This value may
         normally be written. Writing as "out" defaults to
         initializing the value as low. To ensure glitch free
         operation, values "low" and "high" may be written to
         configure the GPIO as an output with that initial value
     @return
     -0  if it output 
     -1  if it input
     */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_read_direction(ENUM_PINNAME pin_name);

/*-----------------------------------------------------------------------------------------------*/
/**
      write value to GPIO output, low or high
      @brief
        @param[in] pin_name  GPIO enumeration arrey
         @param[in] level   High or low
      @return
          - RES_OK  setting direction was successful
          -RES_BAD_PARAMETER  probably not exported
          - RES_IO_NOT_SUPPORT pin is not supported
          - RES_IO_ERROR if it failed
       */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_write_value( ENUM_PINNAME pin_name,  ENUM_GPIO_LEVEL level );

/*-----------------------------------------------------------------------------------------------*/
/**
      read level value from GPIO input mode.
     @brief
        @param[in] pin_name  GPIO enumeration arrey
     @return
          - RES_IO_NOT_SUPPORT pin is not supported
          - 1  if it high
          - 0  if it low
     */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_read_value(ENUM_PINNAME pin_name);

/*-----------------------------------------------------------------------------------------------*/
/**
     setup GPIO interrupt awake  mode.
     @brief
        @param[in] pin_name  GPIO enumeration arrey
        @param[in] en   0:disable awake function   1:enable awake function
         
     @return
      - RES_OK  setting direction was successful
      - RES_IO_NOT_SUPPORT pin is not supported
      - RES_IO_ERROR if it failed
   */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_write_awake( ENUM_PINNAME  pin_name,   ENUM_GPIO_AWAKE  en );

/*-----------------------------------------------------------------------------------------------*/
/**
      read level value from GPIO input mode.
     @brief
        @param[in] pin_name  GPIO enumeration arrey
     @return
          - RES_IO_NOT_SUPPORT pin is not supported
          - 1  if it is enable awake function
          - 0  if it is disable awake function
     */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_read_awake(ENUM_PINNAME pin_name);

/*-----------------------------------------------------------------------------------------------*/
/**
     Open the device file of level value, monitoring correct GPIO value
     @brief
     @param[in] pin_name  GPIO enumeration arrey
      @return
         - RES_IO_NOT_SUPPORT pin is not supported
 
 */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_open_value(ENUM_PINNAME pin_name);

/*-----------------------------------------------------------------------------------------------*/
/**
    Close the device file of level value opened by gpio_sysfs_open_value.
     @brief
     @param[in] handle  file descriptor
 
     @return
       -0 
    */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_close_value(int handle);

/*-----------------------------------------------------------------------------------------------*/
/**
     Rising or Falling of Edge sensitivity,This file exists only if the pin can be configured as an,interrupt generating input pin.
      @brief
       @param[in] pin_name GPIO enumeration arrey
       @param[in] edgeSense The mode of GPIO Edge Sensivity.
      "edge" ... reads as either "none", "rising", "falling", or
               "both". Write these strings to select the signal edge(s)
                   that will make poll(2) on the "value" file return.   
      @return
        - RES_IO_NOT_SUPPORT pin is not supported
        - RES_OK  setting direction was successful
        - RES_IO_ERROR if it failed
     */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_set_edge_sense(ENUM_PINNAME  pin_name,  ENUM_GPIO_EDGE_SENSE_MODE edgeSense);

/*-----------------------------------------------------------------------------------------------*/
/**
      set gpio pull
      @brief
       @param[in] pin_name GPIO enumeration arrey
       @param[in] val The mode of GPIO Edge Sensivity.
            0-----gpio no pull
            1-----gpio pull down
            3-----gpio pull up
       @return
       - RES_IO_NOT_SUPPORT pin is not supported
       - RES_OK  setting direction was successful
       - RES_IO_ERROR if it failed
     */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_write_register(ENUM_PINNAME  pin_name, unsigned char val);


/*-----------------------------------------------------------------------------------------------*/
/**
      get gpio pull
     @brief
   @param[in] pin_name GPIO enumeration arrey
     @return
       - RES_IO_NOT_SUPPORT pin is not supported
       - RES_IO_ERROR if it failed
       -0  not pull
       -1  pull down
       -3  pull up
*/
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_read_register(ENUM_PINNAME  pin_name);

/*-----------------------------------------------------------------------------------------------*/
/**
       setup GPIO polarity.
        @brief
   @param[in] pin_name GPIO enumeration arrey
   @param[in] level  Active-high or active-low
       @return 
         - RES_IO_NOT_SUPPORT pin is not supported
         - RES_OK  setting direction was successful
         - RES_IO_ERROR if it failed  
        */
/*-----------------------------------------------------------------------------------------------*/
int gpio_sysfs_write_polarity(ENUM_PINNAME  pin_name, ENUM_GPIO_POLARITY_LEVEL level );

#ifdef __cplusplus
}
#endif

#endif    //__GPIOSYSFS_H__
