/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_type.h 
  @brief This is the public header file which defines data type 
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
  20181024   tyler.kuang  Created .
-------------------------------------------------------------------------------------------------*/

#ifndef __QL_TYPE_H__
#define __QL_TYPE_H__
#include <stdint.h>

typedef int ql_err_t ;

#define QL_DEVICE_NAME_MAX_LEN          32

#define QL_EID_BASE      1000
#define QL_EID_EXTEND    10000


#define QL_ERR_OK                       0                   /* Success */
#define QL_ERR_FAILED                   (-(QL_EID_BASE+1))  /* Failed */
#define QL_ERR_INTERNAL                 (-(QL_EID_BASE+2))  /* Internal error */
#define QL_ERR_UNKNOWN                  (-(QL_EID_BASE+3))  /* Unknown error */
#define QL_ERR_GENERIC                  (-(QL_EID_BASE+4))  /* General error */
#define QL_ERR_DISABLED                 (-(QL_EID_BASE+5))  /* Disabled */
#define QL_ERR_UNSUPPORTED              (-(QL_EID_BASE+6))  /* Unspported */
#define QL_ERR_OP_UNSUPPORTED           (-(QL_EID_BASE+7))  /* Operation is not supported */
#define QL_ERR_INTERNAL_EXTENDED        (-(QL_EID_BASE+8))  /* Extended internal error */
#define QL_ERR_BUNDLING_UNSPPORTED      (-(QL_EID_BASE+9))  /* Bundling is not supported */
#define QL_ERR_INVALID_ARG              (-(QL_EID_BASE+10))  /* Invalid argument */
#define QL_ERR_INVALID_INDEX            (-(QL_EID_BASE+11))  /* Invalid index */
#define QL_ERR_INVALID_REQUEST          (-(QL_EID_BASE+12))  /* Invalid request */ 
#define QL_ERR_INVALID_STATE            (-(QL_EID_BASE+13))  /* Invalid state */ 
#define QL_ERR_INVALID_ID               (-(QL_EID_BASE+14))  /* Invalid ID */
#define QL_ERR_INVALID_MSG_ID           (-(QL_EID_BASE+15))  /* Invalid message id */
#define QL_ERR_INVALID_HANDLE           (-(QL_EID_BASE+16))  /* Invalid handle */
#define QL_ERR_INVALID_PROFILE          (-(QL_EID_BASE+17))  /* Invalid profile */
#define QL_ERR_INVALID_PINID            (-(QL_EID_BASE+18))  /* Invalid PIN ID */
#define QL_ERR_INVALID_CALL_ID          (-(QL_EID_BASE+19))  /* Invalid call id */
#define QL_ERR_INVALID_TX_ID            (-(QL_EID_BASE+20))  /* Invalid Tx ID */
#define QL_ERR_INVALID_DATA_FORMAT      (-(QL_EID_BASE+21))  /* Invalid data format */
#define QL_ERR_INVALID_MSG_FORMAT       (-(QL_EID_BASE+22))  /* Invalid message format */ 
#define QL_ERR_INVALID_TRANSITION       (-(QL_EID_BASE+23))  /* Invalid transition */
#define QL_ERR_INVALID_IP_FAMILY        (-(QL_EID_BASE+24))  /* Invalid ip family */
#define QL_ERR_INVALID_OPERATION        (-(QL_EID_BASE+25))  /* Invalid operation */
#define QL_ERR_INVALID_CMD              (-(QL_EID_BASE+26))  /* Invalid command */
#define QL_ERR_INVALID_TYPE             (-(QL_EID_BASE+27))  /* Invalid type */
#define QL_ERR_DENIED                   (-(QL_EID_BASE+28))  /* Operation or access was denied */
#define QL_ERR_RESTRICTED               (-(QL_EID_BASE+29))  /* Resouce is restricted */
#define QL_ERR_NO_MEM                   (-(QL_EID_BASE+30))  /* Memory is not enough */
#define QL_ERR_MEM                      (-(QL_EID_BASE+31))  /* Memory error */
#define QL_ERR_BUF_OVERFLOW             (-(QL_EID_BASE+32))  /* BUffer overflow */
#define QL_ERR_TIMEOUT                  (-(QL_EID_BASE+33))  /* Timeout */
#define QL_ERR_NOT_INIT                 (-(QL_EID_BASE+34))  /* Not initialized */
#define QL_ERR_ABORTED                  (-(QL_EID_BASE+35))  /* Action was aborted */
#define QL_ERR_NETWORK_ABORTED          (-(QL_EID_BASE+36))  /* Network was aborted */
#define QL_ERR_RESOURCE_EXHAUSTED       (-(QL_EID_BASE+37))  /* Resource have been exhausted */
#define QL_ERR_RESOUCE_IN_USE           (-(QL_EID_BASE+38))  /* Resouce in use */
#define QL_ERR_RESOUCE_NOT_ENOUGH       (-(QL_EID_BASE+39))  /* Insufficient resources */
#define QL_ERR_UNABORTABLE_TRANSACTION  (-(QL_EID_BASE+40))  /* Unabortable transaction */
#define QL_ERR_INCORRECT_PIN            (-(QL_EID_BASE+41))  /* Incorrect PIN */
#define QL_ERR_NO_NETWORK_FOUND         (-(QL_EID_BASE+42))  /* No network found */
#define QL_ERR_CALL_FAILED              (-(QL_EID_BASE+43))  /* Call failed */
#define QL_ERR_OUT_OF_CALL              (-(QL_EID_BASE+44))  /* Out of call */
#define QL_ERR_NOT_PROVISIONED          (-(QL_EID_BASE+45))  /* Not provisioned */
#define QL_ERR_MISSING_ARG              (-(QL_EID_BASE+46))  /* Missing argument */
#define QL_ERR_ARG_TOO_LONG             (-(QL_EID_BASE+47))  /* Argument is too long */
#define QL_ERR_DEVICE_NOT_READY         (-(QL_EID_BASE+48))  /* Device is not ready */
#define QL_ERR_OP_DEVICE_UNSUPPORTED    (-(QL_EID_BASE+49))  /* OP device is not supported */
#define QL_ERR_DEVICE_NO_MEM            (-(QL_EID_BASE+50))  /* Device memory is not enough */
#define QL_ERR_DEVICE_MEM               (-(QL_EID_BASE+51))  /* Device memory error */
#define QL_ERR_DEVICE_IN_USE            (-(QL_EID_BASE+52))  /* Device in use */
#define QL_ERR_OP_NETWORK_UNSUPPORTED   (-(QL_EID_BASE+53))  /* OP network is not supported */
#define QL_ERR_NO_EFFECT                (-(QL_EID_BASE+54))  /* No effect */
#define QL_ERR_AUTH_FAILED              (-(QL_EID_BASE+55))  /* Authentication failed */
#define QL_ERR_AUTH_LOCK                (-(QL_EID_BASE+56))  /* Authentication lock error */
#define QL_ERR_PIN_BLOCKED              (-(QL_EID_BASE+57))  /* PIN is blocked */
#define QL_ERR_PIN_PERM_BLOCKED         (-(QL_EID_BASE+58))  /* PIN is permanently blocked */
#define QL_ERR_SIM_NOT_INIT             (-(QL_EID_BASE+59))  /* SIM is not initialized */
#define QL_ERR_SIM_NOT_FOUND            (-(QL_EID_BASE+60))  /* SIM was not found */
#define QL_ERR_REQUESTED_NUM_UNSUPPORTED  (-(QL_EID_BASE+61))  /* Requested number is not supported */
#define QL_ERR_INTERFACE_NOT_FOUND        (-(QL_EID_BASE+62))  /* Interface was not found */
#define QL_ERR_FILE_NOT_FOUND            (-(QL_EID_BASE+63))   /* File was not found */
#define QL_ERR_SERVICE_NOT_FOUND         (-(QL_EID_BASE+64))   /* service was not found */
#define QL_ERR_NO_ENTRY                  (-(QL_EID_BASE+65))   /* No entry */
#define QL_ERR_NETWORK_NOT_READY         (-(QL_EID_BASE+66))   /* Network is not ready */
#define QL_ERR_SERVICE_NOT_READY         (-(QL_EID_BASE+67))   /* Service is not ready */ 
#define QL_ERR_CAUSE_CODE                (-(QL_EID_BASE+68))   /* Cause code error */
#define QL_ERR_MSG_NOT_SENT              (-(QL_EID_BASE+69))   /* Message was not sent */
#define QL_ERR_MSG_DELIVERY_FAILURE      (-(QL_EID_BASE+70))   /* Message delivery failure */
#define QL_ERR_MSG_BLOCKED               (-(QL_EID_BASE+71))   /* Message was blocked */
#define QL_ERR_ENCODING                  (-(QL_EID_BASE+72))   /* Encoding error */
#define QL_ERR_DECODING                  (-(QL_EID_BASE+73))   /* Decoding error */
#define QL_ERR_SESSION_INVALID_TYPE      (-(QL_EID_BASE+74))   /* Invalid session type */
#define QL_ERR_SESSION_INACTIVE          (-(QL_EID_BASE+75))   /* Session is inactive */
#define QL_ERR_SESSION_INVALID           (-(QL_EID_BASE+76))   /* Session is invalid */
#define QL_ERR_SESSION_OWNERSHIP         (-(QL_EID_BASE+77))   /* Session ownership error */
#define QL_ERR_SMSC_ADDR                 (-(QL_EID_BASE+78))   /* Short message service center address error */
#define QL_ERR_INFO_UNAVAILABLE          (-(QL_EID_BASE+79))   /* Information is not available */
#define QL_ERR_SEGMENT_TOO_LONG          (-(QL_EID_BASE+80))   /* Segment is too long */
#define QL_ERR_SEGMENT_ORDER             (-(QL_EID_BASE+81))   /* Segment order error */
#define QL_ERR_MISMATCH                  (-(QL_EID_BASE+82))   /* Something mismatch */ 
#define QL_ERR_ACK_NOT_SENT              (-(QL_EID_BASE+83))   /* Acknowledgement was not sent */
#define QL_ERR_INTERNAL_TIMEOUT          (-(QL_EID_BASE+84))   /* Internal timeout */
#define QL_ERR_FDN_RESTRICT              (-(QL_EID_BASE+85))   /* Fixed dialing number restrict error */
#define QL_ERR_NO_RADIO                  (-(QL_EID_BASE+86))   /* No radio */
#define QL_ERR_INVALID_PB_TYPE           (-(QL_EID_BASE+87))   /* Invalid phonebook type */
#define QL_ERR_PB_NOT_READY              (-(QL_EID_BASE+88))   /* Phonebook not ready */
#define QL_ERR_PB_ACCESS_RESTRICTED      (-(QL_EID_BASE+89))   /* Phonebook access is restricted */
#define QL_ERR_SUPS_FAILURE_CAUSE        (-(QL_EID_BASE+90))   /* SUPS failure cause */
#define QL_ERR_NV_NO_FILE                (-(QL_EID_BASE+91))   /* NV file not exist */
#define QL_ERR_NV_FS_NOT_SUPPORTED       (-(QL_EID_BASE+92))   /* NV file system not supported */
#define QL_ERR_NV_BAD_SIZE               (-(QL_EID_BASE+93))   /* NV bad buffer size */
#define QL_ERR_NV_BAD_BUF                (-(QL_EID_BASE+94))   /* NV buffer is NULL*/
#define QL_ERR_TARGET_EXIST              (-(QL_EID_BASE+95))   /* Target exists */
#define QL_ERR_TARGET_NOT_EXIST          (-(QL_EID_BASE+96))   /* Target does not exist */
#define QL_ERR_NO_PERMISSION             (-(QL_EID_BASE+97))   /* No permission */
#define QL_ERR_MODEM_OFFLINE             (-(QL_EID_BASE+98))   /* Modem subsystem is offline */
#define QL_ERR_SERVICE_ABORT             (-(QL_EID_BASE+99))   /* Service abort */

typedef enum
{
    QL_SIM_MIN = 0,
    QL_SIM_1 = 1,
    QL_SIM_2 = 2,
    QL_SIM_MAX
}QL_SIM_ID_E;

#ifdef QL_FEATURE_MULTI_SIM
#define QL_IS_SIM_VALID(sim_id) ((sim_id)>QL_SIM_MIN && (sim_id)<QL_SIM_MAX)
#else
#define QL_IS_SIM_VALID(sim_id) ((sim_id)>QL_SIM_MIN && (sim_id)<QL_SIM_MAX-1)
#endif

#endif

