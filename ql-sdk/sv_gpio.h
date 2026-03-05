/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_gpio.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_gpio相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_GPIO_H__
#define __SV_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>

/**
 * enum GPIO List
 */
typedef enum{
    PINNAME_BEGIN = -1,     /**<Invalid*/
    PINNAME_GPIO_8=8,       /**<PIN-106*/
    PINNAME_GPIO_9=9,       /**<PIN-108*/
    PINNAME_GPIO_10=10,     /**<PIN-103*/
    PINNAME_GPIO_11=11,     /**<PIN-105*/
    PINNAME_GPIO_14=14,     /**<PIN-80*/
    PINNAME_GPIO_15=15,     /**<PIN-79*/
    PINNAME_GPIO_16=16,     /**<PIN-265*/
    PINNAME_GPIO_17=17,     /**<PIN-263*/
    PINNAME_GPIO_18=18,     /**<PIN-261*/
    PINNAME_GPIO_19=19,     /**<PIN-262*/
    PINNAME_GPIO_20=20,     /**<PIN-73*/
    PINNAME_GPIO_21=21,     /**<PIN-76*/
    PINNAME_GPIO_22=22,     /**<PIN-78*/
    PINNAME_GPIO_23=23,     /**<PIN-75*/
    PINNAME_GPIO_35=35,     /**<PIN-466*/
    PINNAME_GPIO_42=42,     /**<PIN-30*/
    PINNAME_GPIO_43=43,     /**<PIN-36*/
    PINNAME_GPIO_44=44,     /**<PIN-39*/
    PINNAME_GPIO_52=52,     /**<PIN-61*/
    PINNAME_GPIO_53=53,     /**<PIN-62*/
    PINNAME_GPIO_54=54,     /**<PIN-59*/
    PINNAME_GPIO_55=55,     /**<PIN-63*/
    PINNAME_GPIO_64=64,     /**<PIN-298*/
    PINNAME_GPIO_65=65,     /**<PIN-297*/
    PINNAME_GPIO_69=69,     /**<PIN-246*/
    PINNAME_GPIO_74=74,     /**<PIN-81*/
    PINNAME_GPIO_75=75,     /**<PIN-249*/
    PINNAME_GPIO_76=76,     /**<PIN-467*/
    PINNAME_GPIO_77=77,     /**<PIN-243*/
    PINNAME_GPIO_78=78,     /**<PIN-283*/
    PINNAME_GPIO_79=79,     /**<PIN-284*/
    PINNAME_GPIO_80=80,     /**<PIN-289*/
    PINNAME_GPIO_82=82,     /**<PIN-24*/
    PINNAME_GPIO_83=83,     /**<PIN-20*/
    PINNAME_GPIO_84=84,     /**<PIN-22*/
    PINNAME_GPIO_85=85,     /**<PIN-23*/
    PINNAME_GPIO_86=86,     /**<PIN-25*/
    PINNAME_GPIO_87=87,     /**<PIN-21*/
    PINNAME_GPIO_88=88,     /**<PIN-19*/
    PINNAME_GPIO_89=89,     /**<PIN-13*/
    PINNAME_GPIO_90=90,     /**<PIN-14*/
    PINNAME_GPIO_91=91,     /**<PIN-16*/
    PINNAME_GPIO_92=92,     /**<PIN-17*/
    PINNAME_GPIO_93=93,     /**<PIN-15*/
    PINNAME_GPIO_94=94,     /**<PIN-11*/
    PINNAME_GPIO_95=95,     /**<PIN-10*/
    PINNAME_GPIO_96=96,     /**<PIN-31*/
    PINNAME_GPIO_97=97,     /**<PIN-29*/
    PINNAME_GPIO_98=98,     /**<PIN-504*/
    PINNAME_GPIO_99=99,     /**<PIN-506*/
    PINNAME_GPIO_100=100,   /**<PIN-505*/
    PINNAME_GPIO_101=101,   /**<PIN-503*/
    PINNAME_GPIO_102=102,   /**<PIN-207*/
    PINNAME_GPIO_103=103,   /**<PIN-280*/
    PINNAME_GPIO_107=107,   /**<PIN-45*/
    PINNAME_GPIO_108=108,   /**<PIN-169*/
    PINNAME_GPIO_109=109,   /**<PIN-187*/
    PINNAME_GPIO_110=110,   /**<PIN-264*/
    PINNAME_GPIO_111=111,   /**<PIN-267*/
    PINNAME_GPIO_112=112,   /**<PIN-219*/
    PINNAME_GPIO_113=113,   /**<PIN-210*/
    PINNAME_GPIO_114=114,   /**<PIN-216*/
    PINNAME_GPIO_115=115,   /**<PIN-213*/
    PINNAME_GPIO_116=116,   /**<PIN-74*/
    PINNAME_GPIO_117=117,   /**<PIN-71*/
    PINNAME_GPIO_118=118,   /**<PIN-70*/
    PINNAME_GPIO_119=119,   /**<PIN-72*/
    PINNAME_GPIO_120=120,   /**<PIN-484*/
    PINNAME_GPIO_121=121,   /**<PIN-485*/
    PINNAME_GPIO_122=122,   /**<PIN-486*/
    PINNAME_GPIO_123=123,   /**<PIN-64*/
    PINNAME_GPIO_124=124,   /**<PIN-296*/
    PINNAME_GPIO_125=125,   /**<PIN-295*/
    PINNAME_GPIO_126=126,   /**<PIN-102*/
    PINNAME_GPIO_127=127,   /**<PIN-101*/
    PINNAME_GPIO_128=128,   /**<PIN-104*/
    PINNAME_GPIO_129=129,   /**<PIN-100*/
    PINNAME_GPIO_130=130,   /**<PIN-116*/
    PINNAME_GPIO_132=132,   /**<PIN-181*/
    PINNAME_END=133
}ENUM_PINNAME;

/**
 * enum GPIO PIN direction state
 */
typedef enum{
    PINDIRECTION_IN  = 0,	/**< GPIO input direction */
    PINDIRECTION_OUT = 1	/**< GPIO output direction */
}ENUM_PIN_DIRECTION;

/**
 * enum GPIO PIN level state
 */
typedef enum{
    PINLEVEL_LOW  = 0,	/**< GPIO low level */
    PINLEVEL_HIGH = 1	/**< GPIO high level */
}ENUM_PIN_LEVEL;

/**
 * enum GPIO PIN awake state
 */
typedef enum{
    PINAWKAE_DISABLE  = 0, /**< GPIO awake disable */
    PINAWAKE_ENABLE = 1	   /**< GPIO awake enable*/
}ENUM_PIN_AWAKE;

/**
 * enum GPIO PIN PULLSEL state
 */
typedef enum{
    PINPULLSEL_DISABLE  = 0, /**< Disable pull selection */
    PINPULLSEL_PULLDOWN = 1, /**< pull down */
    PINPULLSEL_PULLUP   = 3  /**< pull up */
}ENUM_PIN_PULLSEL;

/**GPIO configure structure*/
typedef struct{
    ENUM_PINNAME           pin_name;	    /**< GPIO pin_name arrey*/
    ENUM_PIN_DIRECTION      pinDirection;   /**< GPIO pinDirection arrey*/
    ENUM_PIN_LEVEL          pinLevel;       /**< GPIO pinLevel arrey*/
    ENUM_PIN_PULLSEL        pinPullSel;	    /**< GPIO pinPullSel arrey*/
}st_gpio_config;

/**
 * enum EINT TYPE
 */
typedef enum {
    EINT_SENSE_NONE,     /**< pin is input, but no an interrupt pin*/
    EINT_SENSE_RISING,   /**< EINT RISING */
    EINT_SENSE_FALLING,  /**< EINT FALLING */
    EINT_SENSE_BOTH      /**< EINT BOTH */
}ENUM_EINT_TYPE;

/*****************************************************************/
/**
  @brief  This function enables the GPIO function of the specified pin,
               and initialize the configurations, including direction,
               level and pull selection.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @param[in] dir The initial direction of GPIO, one value of ENUM_PIN_DIRECTION.
  @param[in] level The initial level of GPIO, one value of ENUM_PIN_LEVEL.
  @param[in] pullSel Pull selection, one value of ENUM_PIN_PULLSEL.
  @return Whether the gpio init was successfully
  @retval RES_OK, this function succeeds.
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  @retval RES_IO_ERR, the function failed other place. For example this GPIO has been using as EINT.
  */
/******************************************************************/
int sv_gpio_init(ENUM_PINNAME       pin_name,
                 ENUM_PIN_DIRECTION  dir,
                 ENUM_PIN_LEVEL      level,
                 ENUM_PIN_PULLSEL    pullSel
                 );

/*****************************************************************/
/**
  @brief   This function enables the GPIO function of the specified pin.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @return Whether the gpio base init was successfully
  @retval RES_OK, this function succeeds.
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  @retval RES_IO_ERR, the function failed other place. For example this GPIO has been using as EINT.
  */
/******************************************************************/
int sv_gpio_base_init(ENUM_PINNAME pin_name );

/*****************************************************************/
/**
  @brief  This function sets the level of the specified GPIO.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @param[in] level The initial level of GPIO, one value of ENUM_PIN_LEVEL.
  @return Whether the gpio set level was successfully
  @retval RES_OK, this function succeeds.
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  @retval RES_IO_ERR, the function failed other place. For example this GPIO has been using as EINT.
  */
/******************************************************************/
int sv_gpio_set_level(ENUM_PINNAME pin_name, ENUM_PIN_LEVEL level);

/*****************************************************************/
/**
  @brief  This function gets the level of the specified GPIO
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @return The level value of the specified GPIO, which is nonnegative integer.
          RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  */
/******************************************************************/
int sv_gpio_get_level(ENUM_PINNAME pin_name);

/*****************************************************************/
/**
  @brief  This function sets the direction of the specified GPIO.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @param[in] dir The initial direction of GPIO, one value of ENUM_PIN_DIRECTION.
  @return Whether set the gpio direction was successfully
  @retval RES_OK, this function succeeds.
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  @retval RES_IO_ERR, the function failed other place. For example this GPIO has been using as EINT.
  */
/******************************************************************/
int sv_gpio_set_direction(ENUM_PINNAME pin_name, ENUM_PIN_DIRECTION dir);

/*****************************************************************/
/**
  @brief  This function gets the direction of the specified GPIO
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @return The direction of the specified GPIO
  @retval 0  INPUT
  @retval 1  OUTPUT
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
          other place. For example this GPIO has been using as EINT.
  */
/******************************************************************/
int sv_gpio_get_direction(ENUM_PINNAME pin_name);

/*****************************************************************/
/**
  @brief  This function sets the interrupt awake of the specified GPIO.
          The GPIO must be configured as  input and interrupt
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @param[in] en 1:enable   0:disable
  @return Whether set the gpio awake was successfully
  @retval RES_OK, this function succeeds.
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  @retval RES_IO_ERR, the function failed
  */
/*****************************************************************/
int sv_gpio_set_awake(ENUM_PINNAME pin_name, ENUM_PIN_AWAKE en);

/*****************************************************************/
/**
  @brief  This function gets the interrupt awake of the specified GPIO.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @return The direction of the specified GPIO
  @retval 0  disable awake
  @retval 1  enable awake
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  */
/******************************************************************/
int sv_gpio_get_awake(ENUM_PINNAME pin_name);

/*****************************************************************/
/**
  @brief  This function sets the pull selection of the specified GPIO.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @param[in] ENUM_PIN_PULLSEL Pull selection, one value of ENUM_PIN_PULLSEL.
  @return Whether set the gpio pull was successfully
  @retval RES_OK, this function succeeds.
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  @retval RES_IO_ERR, the function failed,other place. For example this GPIO has been using as EINT.
  */
/*****************************************************************/
int sv_gpio_set_pull_selection(ENUM_PINNAME pin_name, ENUM_PIN_PULLSEL pullSel);

/*****************************************************************/
/**
  @brief   This function gets the pull selection of the specified GPIO.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @return The pull state of the specified GPIO
  @retval 0 no pull
  @retval 1 pull down
  @retval 3 pull up
  */
/******************************************************************/
int sv_gpio_get_pull_selection(ENUM_PINNAME pin_name);

/*****************************************************************/
/**
  @brief  This function releases the specified GPIO that was
          initialized by calling sv_gpio_init() previously.
          After releasing, the GPIO can be used for other purpose.
  @param[in] pin_name Pin name, one value of ENUM_PINNAME.
  @return Whether the gpio uninit was successfully
  @retval RES_OK, this function succeeds.
  @retval RES_IO_NOT_SUPPORT, the input GPIO is invalid.
  @retval RES_IO_ERR, the function failed,other place. For example this GPIO has been using as EINT.
  */
/*****************************************************************/
int sv_gpio_uninit(ENUM_PINNAME pin_name);

/**
 * Definition for EINT callback function.
 * @param[in] eint_pin_name: EINT pin name, one value of ENUM_PINNAME.
 * @param[in] level: The EINT level value, one value of ENUM_PIN_LEVEL. 0 or 1
 * @return no value
 */
typedef void (*sv_eint_callback)(ENUM_PINNAME eint_pin_name, int level);

/*****************************************************************/
/**
  @brief  Set the interrupt sense mode, and enable interrupt.
  @param[in] eint_pin_name EINT pin name, one value of ENUM_PINNAME
  @param[in] eint_type Interrupt type, level-triggered or edge-triggered.Now, only edge-triggered interrupt is supported.
  @param[in] eint_callback call back function
  @return Whether the gpio eint enable was successfully
  @retval RES_OK this function succeeds.
  @retval OTHER failed to execute the function.
  */
/*****************************************************************/
int sv_eint_enable(ENUM_PINNAME eint_pin_name, ENUM_EINT_TYPE eint_type, sv_eint_callback eint_callback);

/*****************************************************************/
/**
  @brief  Disable the interrupt sense.
  @param[in] eint_pin_name EINT pin name, one value of ENUM_PINNAME
  @return Whether the gpio eint disable was successfully
  @retval RES_OK this function succeeds.
  @retval OTHER failed to execute the function.
  */
/*****************************************************************/
int sv_eint_disable(ENUM_PINNAME eint_pin_name);

#ifdef __cplusplus
}
#endif

#endif  // __SV_GPIO_H__
