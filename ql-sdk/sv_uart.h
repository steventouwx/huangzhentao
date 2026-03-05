/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_uart.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_uart相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

  
#ifndef __SV_UART_H__
#define __SV_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * enum Flowctrl List
 */
typedef enum {
    FC_NONE = 0,   /**< None Flow Control*/
    FC_RTSCTS, 	   /**< Hardware Flow Control (rtscts)*/
    FC_XONXOFF 	   /**< Software Flow Control (xon/xoff)  */
}Enum_FlowCtrl;

/**
 * enum parity bit
 */
typedef enum {
    PB_NONE = 0, 		/**< none parity check*/
    PB_ODD,			/**< odd parity check */
    PB_EVEN 			/**< even parity check*/
}Enum_ParityBit;

/**
 * enum DataBit List
 */
typedef enum {
    DB_CS5 = 5, /**< 5 data bits*/
    DB_CS6 = 6, /**< 6 data bits*/
    DB_CS7 = 7, /**< 7 data bits*/
    DB_CS8 = 8  /**< 8 data bits*/
}Enum_DataBit;

/**
 * enum stop bit List
 */
typedef enum {
    SB_1 = 1,  /**< 1 bit stop*/
    SB_2 = 2   /**< 2 bit stop*/
}Enum_StopBit;

/**
 * enum BaudRate List
 */
typedef enum {
	B_300 		= 300,/**< baudrate 300*/
	B_600 		= 600,/**< baudrate 600*/
	B_1200 		= 1200,/**< baudrate 1200*/
	B_2400 		= 2400,/**< baudrate 2400*/
	B_4800 		= 4800,/**< baudrate 4800*/
	B_9600 		= 9600,/**< baudrate 9600*/
	B_19200 	= 19200,/**< baudrate 19200*/
	B_38400 	= 38400,/**< baudrate 38400*/
	B_57600 	= 57600,/**< baudrate 57600*/
	B_115200 	= 115200,/**< baudrate 115200*/
	B_230400 	= 230400,/**< baudrate 230400*/
	B_460800 	= 460800,/**< baudrate 460800*/
	B_921600	= 921600,/**< baudrate 921600*/
	B_1000000	= 1000000,/**< baudrate 1000000*/
	B_2000000	= 2000000,/**< baudrate 2000000*/
	B_3000000	= 3000000/**< baudrate 3000000*/
}Enum_BaudRate;

/**UART DCB structure*/
typedef struct {
    Enum_BaudRate       baudrate; 		/**< baudrate enumeration arrey*/
    Enum_DataBit       	databit;		/**< databit enumeration arrey*/
    Enum_StopBit       	stopbit;		/**< stopbit enumeration arrey*/
    Enum_ParityBit      parity;			/**< parity enumeration arrey*/
    Enum_FlowCtrl       flowctrl;		/**< flowctrl enumeration arrey*/
}ST_UARTDCB;

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Open uart device api
  @param[in] port     uart devices path
  @param[in] flags    property setting
  @return
         -fd   file descriptor
         -SV_ERR_TARGET_NOT_EXIST   devices does not exist
 */
/*-----------------------------------------------------------------------------------------------*/
int  sv_uart_open(const char* port, int flags);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Uart write api
  @param[in] fd      file descriptor for sv_uart_open return value 
  @param[in] buf    write buffer
  @param[in] buf_len   write buffer len
  @return 
         -size     write data size 
         -SV_ERR_FAILED   if it write failed
         -SV_ERR_INVALID_ARG   invalid  arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int  sv_uart_write(int fd, const char* buf, unsigned int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Uart read api
  @param[in] fd      file descriptor for sv_uart_open return value 
  @param[in] buf    read buffer
  @param[in] buf_len   read buffer len
  @return
      -other     write data size 
      -SV_ERR_INVALID_ARG   invalid  arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int  sv_uart_read(int fd, char* buf, unsigned int buf_len);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Uart ioctrl api
  @param[in] fd      file descriptor for sv_uart_open return value 
  @param[in] cmd    ioctl cmd
  @return
  @retval 1     ioctl error
  @retval 0       if it ok
  @retval SV_ERR_INVALID_ARG   invalid  arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int  sv_uart_ioctl(int fd, unsigned int cmd, void* pValue);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Setting parameter api
  @param[in] fd      file descriptor for sv_uart_open return value 
  @param[in] dcb   ST_UARTDCB sturct for uart parameter
  @return
  @retval SV_ERR_OK     if it ok
  @retval SV_ERR_INVALID_ARG   invalid  arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int  sv_uart_setdcb(int fd, ST_UARTDCB *dcb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Get  parameter api
  @param[in] fd      file descriptor for sv_uart_open return value 
  @param[in] dcb   ST_UARTDCB sturct for uart parameter
  @return
  @retval SV_ERR_OK     if it ok
  @retval SV_ERR_INVALID_ARG   invalid  arguments
  */
/*-----------------------------------------------------------------------------------------------*/
int  sv_uart_getdcb(int fd, ST_UARTDCB *dcb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Close uart device api
  @param[in] fd      file descriptor for sv_uart_open return value 
  @return
  @retval 0       if it ok
  @retval other  if it failed
  */
/*-----------------------------------------------------------------------------------------------*/
int  sv_uart_close(int fd);

#ifdef __cplusplus
}
#endif

#endif //__SV_UART_H__
