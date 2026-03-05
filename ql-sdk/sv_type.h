/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_type.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_type相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_TYPE_H__
#define __SV_TYPE_H__
#include <stdint.h>

typedef int sv_err_t ;

#define SV_DEVICE_NAME_MAX_LEN          32

#define SV_EID_BASE      1000
#define SV_EID_EXTEND    10000

#define SV_ERR_OK                       0                   /* Success */
#define SV_ERR_FAILED                   (-(SV_EID_BASE+1))  /* Failed */
#define SV_ERR_INTERNAL                 (-(SV_EID_BASE+2))  /* Internal error */
#define SV_ERR_UNKNOWN                  (-(SV_EID_BASE+3))  /* Unknown error */
#define SV_ERR_GENERIC                  (-(SV_EID_BASE+4))  /* General error */
#define SV_ERR_DISABLED                 (-(SV_EID_BASE+5))  /* Disabled */
#define SV_ERR_UNSUPPORTED              (-(SV_EID_BASE+6))  /* Unspported */
#define SV_ERR_OP_UNSUPPORTED           (-(SV_EID_BASE+7))  /* Operation is not supported */
#define SV_ERR_INTERNAL_EXTENDED        (-(SV_EID_BASE+8))  /* Extended internal error */
#define SV_ERR_BUNDLING_UNSPPORTED      (-(SV_EID_BASE+9))  /* Bundling is not supported */
#define SV_ERR_INVALID_ARG              (-(SV_EID_BASE+10))  /* Invalid argument */
#define SV_ERR_INVALID_INDEX            (-(SV_EID_BASE+11))  /* Invalid index */
#define SV_ERR_INVALID_REQUEST          (-(SV_EID_BASE+12))  /* Invalid request */ 
#define SV_ERR_INVALID_STATE            (-(SV_EID_BASE+13))  /* Invalid state */ 
#define SV_ERR_INVALID_ID               (-(SV_EID_BASE+14))  /* Invalid ID */
#define SV_ERR_INVALID_MSG_ID           (-(SV_EID_BASE+15))  /* Invalid message id */
#define SV_ERR_INVALID_HANDLE           (-(SV_EID_BASE+16))  /* Invalid handle */
#define SV_ERR_INVALID_PROFILE          (-(SV_EID_BASE+17))  /* Invalid profile */
#define SV_ERR_INVALID_PINID            (-(SV_EID_BASE+18))  /* Invalid PIN ID */
#define SV_ERR_INVALID_CALL_ID          (-(SV_EID_BASE+19))  /* Invalid call id */
#define SV_ERR_INVALID_TX_ID            (-(SV_EID_BASE+20))  /* Invalid Tx ID */
#define SV_ERR_INVALID_DATA_FORMAT      (-(SV_EID_BASE+21))  /* Invalid data format */
#define SV_ERR_INVALID_MSG_FORMAT       (-(SV_EID_BASE+22))  /* Invalid message format */ 
#define SV_ERR_INVALID_TRANSITION       (-(SV_EID_BASE+23))  /* Invalid transition */
#define SV_ERR_INVALID_IP_FAMILY        (-(SV_EID_BASE+24))  /* Invalid ip family */
#define SV_ERR_INVALID_OPERATION        (-(SV_EID_BASE+25))  /* Invalid operation */
#define SV_ERR_INVALID_CMD              (-(SV_EID_BASE+26))  /* Invalid command */
#define SV_ERR_INVALID_TYPE             (-(SV_EID_BASE+27))  /* Invalid type */
#define SV_ERR_DENIED                   (-(SV_EID_BASE+28))  /* Operation or access was denied */
#define SV_ERR_RESTRICTED               (-(SV_EID_BASE+29))  /* Resouce is restricted */
#define SV_ERR_NO_MEM                   (-(SV_EID_BASE+30))  /* Memory is not enough */
#define SV_ERR_MEM                      (-(SV_EID_BASE+31))  /* Memory error */
#define SV_ERR_BUF_OVERFLOW             (-(SV_EID_BASE+32))  /* BUffer overflow */
#define SV_ERR_TIMEOUT                  (-(SV_EID_BASE+33))  /* Timeout */
#define SV_ERR_NOT_INIT                 (-(SV_EID_BASE+34))  /* Not initialized */
#define SV_ERR_ABORTED                  (-(SV_EID_BASE+35))  /* Action was aborted */
#define SV_ERR_NETWORK_ABORTED          (-(SV_EID_BASE+36))  /* Network was aborted */
#define SV_ERR_RESOURCE_EXHAUSTED       (-(SV_EID_BASE+37))  /* Resource have been exhausted */
#define SV_ERR_RESOUCE_IN_USE           (-(SV_EID_BASE+38))  /* Resouce in use */
#define SV_ERR_RESOUCE_NOT_ENOUGH       (-(SV_EID_BASE+39))  /* Insufficient resources */
#define SV_ERR_UNABORTABLE_TRANSACTION  (-(SV_EID_BASE+40))  /* Unabortable transaction */
#define SV_ERR_INCORRECT_PIN            (-(SV_EID_BASE+41))  /* Incorrect PIN */
#define SV_ERR_NO_NETWORK_FOUND         (-(SV_EID_BASE+42))  /* No network found */
#define SV_ERR_CALL_FAILED              (-(SV_EID_BASE+43))  /* Call failed */
#define SV_ERR_OUT_OF_CALL              (-(SV_EID_BASE+44))  /* Out of call */
#define SV_ERR_NOT_PROVISIONED          (-(SV_EID_BASE+45))  /* Not provisioned */
#define SV_ERR_MISSING_ARG              (-(SV_EID_BASE+46))  /* Missing argument */
#define SV_ERR_ARG_TOO_LONG             (-(SV_EID_BASE+47))  /* Argument is too long */
#define SV_ERR_DEVICE_NOT_READY         (-(SV_EID_BASE+48))  /* Device is not ready */
#define SV_ERR_OP_DEVICE_UNSUPPORTED    (-(SV_EID_BASE+49))  /* OP device is not supported */
#define SV_ERR_DEVICE_NO_MEM            (-(SV_EID_BASE+50))  /* Device memory is not enough */
#define SV_ERR_DEVICE_MEM               (-(SV_EID_BASE+51))  /* Device memory error */
#define SV_ERR_DEVICE_IN_USE            (-(SV_EID_BASE+52))  /* Device in use */
#define SV_ERR_OP_NETWORK_UNSUPPORTED   (-(SV_EID_BASE+53))  /* OP network is not supported */
#define SV_ERR_NO_EFFECT                (-(SV_EID_BASE+54))  /* No effect */
#define SV_ERR_AUTH_FAILED              (-(SV_EID_BASE+55))  /* Authentication failed */
#define SV_ERR_AUTH_LOCK                (-(SV_EID_BASE+56))  /* Authentication lock error */
#define SV_ERR_PIN_BLOCKED              (-(SV_EID_BASE+57))  /* PIN is blocked */
#define SV_ERR_PIN_PERM_BLOCKED         (-(SV_EID_BASE+58))  /* PIN is permanently blocked */
#define SV_ERR_SIM_NOT_INIT             (-(SV_EID_BASE+59))  /* SIM is not initialized */
#define SV_ERR_SIM_NOT_FOUND            (-(SV_EID_BASE+60))  /* SIM was not found */
#define SV_ERR_REQUESTED_NUM_UNSUPPORTED  (-(SV_EID_BASE+61))  /* Requested number is not supported */
#define SV_ERR_INTERFACE_NOT_FOUND        (-(SV_EID_BASE+62))  /* Interface was not found */
#define SV_ERR_FILE_NOT_FOUND            (-(SV_EID_BASE+63))   /* File was not found */
#define SV_ERR_SERVICE_NOT_FOUND         (-(SV_EID_BASE+64))   /* service was not found */
#define SV_ERR_NO_ENTRY                  (-(SV_EID_BASE+65))   /* No entry */
#define SV_ERR_NETWORK_NOT_READY         (-(SV_EID_BASE+66))   /* Network is not ready */
#define SV_ERR_SERVICE_NOT_READY         (-(SV_EID_BASE+67))   /* Service is not ready */ 
#define SV_ERR_CAUSE_CODE                (-(SV_EID_BASE+68))   /* Cause code error */
#define SV_ERR_MSG_NOT_SENT              (-(SV_EID_BASE+69))   /* Message was not sent */
#define SV_ERR_MSG_DELIVERY_FAILURE      (-(SV_EID_BASE+70))   /* Message delivery failure */
#define SV_ERR_MSG_BLOCKED               (-(SV_EID_BASE+71))   /* Message was blocked */
#define SV_ERR_ENCODING                  (-(SV_EID_BASE+72))   /* Encoding error */
#define SV_ERR_DECODING                  (-(SV_EID_BASE+73))   /* Decoding error */
#define SV_ERR_SESSION_INVALID_TYPE      (-(SV_EID_BASE+74))   /* Invalid session type */
#define SV_ERR_SESSION_INACTIVE          (-(SV_EID_BASE+75))   /* Session is inactive */
#define SV_ERR_SESSION_INVALID           (-(SV_EID_BASE+76))   /* Session is invalid */
#define SV_ERR_SESSION_OWNERSHIP         (-(SV_EID_BASE+77))   /* Session ownership error */
#define SV_ERR_SMSC_ADDR                 (-(SV_EID_BASE+78))   /* Short message service center address error */
#define SV_ERR_INFO_UNAVAILABLE          (-(SV_EID_BASE+79))   /* Information is not available */
#define SV_ERR_SEGMENT_TOO_LONG          (-(SV_EID_BASE+80))   /* Segment is too long */
#define SV_ERR_SEGMENT_ORDER             (-(SV_EID_BASE+81))   /* Segment order error */
#define SV_ERR_MISMATCH                  (-(SV_EID_BASE+82))   /* Something mismatch */ 
#define SV_ERR_ACK_NOT_SENT              (-(SV_EID_BASE+83))   /* Acknowledgement was not sent */
#define SV_ERR_INTERNAL_TIMEOUT          (-(SV_EID_BASE+84))   /* Internal timeout */
#define SV_ERR_FDN_RESTRICT              (-(SV_EID_BASE+85))   /* Fixed dialing number restrict error */
#define SV_ERR_NO_RADIO                  (-(SV_EID_BASE+86))   /* No radio */
#define SV_ERR_INVALID_PB_TYPE           (-(SV_EID_BASE+87))   /* Invalid phonebook type */
#define SV_ERR_PB_NOT_READY              (-(SV_EID_BASE+88))   /* Phonebook not ready */
#define SV_ERR_PB_ACCESS_RESTRICTED      (-(SV_EID_BASE+89))   /* Phonebook access is restricted */
#define SV_ERR_SUPS_FAILURE_CAUSE        (-(SV_EID_BASE+90))   /* SUPS failure cause */
#define SV_ERR_NV_NO_FILE                (-(SV_EID_BASE+91))   /* NV file not exist */
#define SV_ERR_NV_FS_NOT_SUPPORTED       (-(SV_EID_BASE+92))   /* NV file system not supported */
#define SV_ERR_NV_BAD_SIZE               (-(SV_EID_BASE+93))   /* NV bad buffer size */
#define SV_ERR_NV_BAD_BUF                (-(SV_EID_BASE+94))   /* NV buffer is NULL*/
#define SV_ERR_TARGET_EXIST              (-(SV_EID_BASE+95))   /* Target exists */
#define SV_ERR_TARGET_NOT_EXIST          (-(SV_EID_BASE+96))   /* Target does not exist */
#define SV_ERR_NO_PERMISSION             (-(SV_EID_BASE+97))   /* No permission */
#define SV_ERR_MODEM_OFFLINE             (-(SV_EID_BASE+98))   /* Modem subsystem is offline */
#define SV_ERR_SERVICE_ABORT             (-(SV_EID_BASE+99))   /* Service abort */

typedef enum
{
    SV_SIM_MIN = 0,
    SV_SIM_1 = 1,
    SV_SIM_2 = 2,
    SV_SIM_MAX
}SV_SIM_ID_E;

#ifdef SV_FEATURE_MULTI_SIM
#define SV_IS_SIM_VALID(sim_id) ((sim_id)>SV_SIM_MIN && (sim_id)<SV_SIM_MAX)
#else
#define SV_IS_SIM_VALID(sim_id) ((sim_id)>SV_SIM_MIN && (sim_id)<SV_SIM_MAX-1)
#endif

#endif

