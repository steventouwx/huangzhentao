// SPDX-License-Identifier: MediaTekProprietary
/*
 * mtk_ril_ivt.h
 *
 *  Created on: Jul 11, 2017
 *      Author: mtk54498
 */

#ifndef INCLUDE_MTK_RIL_IVT_H_
#define INCLUDE_MTK_RIL_IVT_H_
#include "ril.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Constants for response types */
#define RESPONSE_SOLICITED 0
#define RESPONSE_UNSOLICITED 1
///
///-----------MTK-RIL-IVT------------///
/// MT2635 vendor ril requests @{
/**
 * RIL_REQUEST_VENDOR_IVT_BASE
 * RIL request sync with mtk ril
 */
#define RIL_REQUEST_VENDOR_IVT_BASE 9000

/**
 * RIL_LOCAL_REQUEST_GET_OPERATOR_NUMBER
 * get operator numeric
 * Only valid when radio state is "RADIO_STATE_ON"
 * "response" is a const char * containing the Operator Numeric
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_LOCAL_REQUEST_GET_OPERATOR_NUMBER (RIL_REQUEST_VENDOR_IVT_BASE + 0)

/**
 * RIL_REQUEST_ECALL_SET_IVS
 *
 * To Enable/Disable IVS mode in Ecall
 *
 * "data" is const int
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_ECALL_SET_IVS (RIL_REQUEST_VENDOR_IVT_BASE + 1)


/**
 * RIL_REQUEST_ECALL_SET_TEST_NUM
 *
 * To set test number for Ecall
 *
 * "data" is const char *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ECALL_SET_TEST_NUM (RIL_REQUEST_VENDOR_IVT_BASE + 2)

/**
 * RIL_REQUEST_ECALL_SET_RECONF_NUM
 *
 * To set reconfigure number for Ecall
 *
 * "data" is const char *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ECALL_SET_RECONF_NUM (RIL_REQUEST_VENDOR_IVT_BASE + 3)

/**
 * RIL_REQUEST_ECALL_SET_MSD
 *
 * To set MSD data for Ecall
 *
 * "data" is a const RIL_ECallSetMSD *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ECALL_SET_MSD (RIL_REQUEST_VENDOR_IVT_BASE + 4)

/**
 * RIL_REQUEST_ECALL_SET_PSAP
 *
 * To Enable/Disable PSAP mode in Ecall
 *
 * "data" is const int
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ECALL_SET_PSAP (RIL_REQUEST_VENDOR_IVT_BASE + 5)

/**
 * RIL_REQUEST_ECALL_MAKE_ECALL
 *
 * To make a ecall
 *
 * "data" is const int
 * "response" is NULL
 *
 * About data calltype:

 * 0 test call
 * 1 reconfiguration call
 * 2 manually initiated eCall
 * 3 automatically initated eCall

 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ECALL_MAKE_ECALL (RIL_REQUEST_VENDOR_IVT_BASE + 6)

/**
 * RIL_REQUEST_ECALL_IVS_PUSH_MSD
 *
 * To trigger IVS push MSD to PSAP
 *
 * "data" is const NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ECALL_IVS_PUSH_MSD (RIL_REQUEST_VENDOR_IVT_BASE + 7)

/**
 * RIL_REQUEST_ECALL_PSAP_PULL_MSD
 *
 * To trigger PSAP to pull MSD from IVS
 *
 * "data" is const NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ECALL_PSAP_PULL_MSD (RIL_REQUEST_VENDOR_IVT_BASE + 8)

/**
 * ATTENTIO: Abandoned request
 * RIL_REQUEST_ECALL_SET_MSD_MODE
 *
 * To set Ecall MSD mode
 *
 * "data" is const int
 * "response" is NULL
 *
 * About data MSD mode:

 * 0 SIM_ECALL_NONE
 * 1 SIM_ECALL_ONLY
 * 2 SIM_ECALL_AND_NORMAL
 * 3 SIM_ECALL_TEST_DISABLE

 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_ECALL_SET_MSD_MODE (RIL_REQUEST_VENDOR_IVT_BASE + 9)

/**
* RIL_REQUEST_ECALL_FAST_MAKE_ECALL
*
* REQUEST to dial the eCall, and then set IVS,MSD, and Send MSD to PSAP
*
* "data" is a RIL_ECallReqMsg *
* "response" is NULL
*
* Valid errors:
*  SUCCESS
*  RADIO_NOT_AVAILABLE (radio resetting)
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_FAST_MAKE_ECALL (RIL_REQUEST_VENDOR_IVT_BASE+10)


/**
* RIL_REQUEST_ECALL_CTRL_SEQUENCE
*
* REQUEST to dial the eCall, and then set IVS,MSD, and Send MSD to PSAP
*
* "data" is char*
* "response" is NULL
*
* Valid errors:
*  SUCCESS
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_CTRL_SEQUENCE (RIL_REQUEST_VENDOR_IVT_BASE+11)


/**
* RIL_REQUEST_ECALL_RESET_IVS
*
* To reset eCall state mechine in IVS modem, connect speech to audio channel upon eCall timer
* (T5/T6/T7) timeout or RIL_UNSOL_ECALL_ALACK_POSITIVE_RECEIVED
* "data" is const NULL
* "response" is NULL
*
* Valid errors:
*  SUCCESS
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_RESET_IVS (RIL_REQUEST_VENDOR_IVT_BASE+12)


/**
* RIL_REQUEST_ECALL_SET_PRI
*
* "data" is const int *
*((const int *)data)[0]/((const int *)data)[1]/((const int *)data)[2]/((const int *)data)[3] shall be one of "1"/"2"/"3"/"4/,
*each represents a kind of eCall number/URI,  the previous URI/number has higher priority when making eCall.
* "1": Customer assigned eCall URI
* "2": eCall URI in USIM
* "3": Customer assigned eCall number
* "4": eCall number in USIM
* e.g 
* If RIL_REQUEST_ECALL_SET_PRI 1 3 2 4 is set,
* It set eCall number priority to  "Customer aasigned eCall URI" > "Customer assigned eCall number" > "eCall URI in USIM" > "eCall number in USIM",
* and MD will try the higher  priority number/URI when making eCall.
*
* "response" is NULL
*
* Valid errors:
*  SUCCESS
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_SET_PRI (RIL_REQUEST_VENDOR_IVT_BASE+13)
#define RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME (RIL_REQUEST_VENDOR_IVT_BASE+14)

/**
* RIL_REQUEST_ECALL_SET_REGISTRATION_STATE
*
* REQUEST to set nad registration state of ecall only sim
*
* "data" is const ints*
*  ((const int *)data)[0] is state, 0-deregister from NW, enter the eCall inactivity procedure;
*                                   1-register to NW, leave the eCall inactivity procedure(not support, reserved for future use)
*                                   2-deregister from NW in eCall normal state
*
* "response" is an int *
* *((int *)response)[0] is 0 for ecall only sim in ecall inactivity procedure
*                              and 1 for normal ecall sim/normal sim in deregistration state
*
* Valid errors:
*  SUCCESS
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_SET_REGISTRATION_STATE (RIL_REQUEST_VENDOR_IVT_BASE+15)

/**
* RIL_REQUEST_ECALL_SET_OPRT_MODE
*
* Sets the service status for operating mode
*
* "data" is int *
*  ((int *)data)[0] is oprt_mode
*   eCall_only: 1
*   eCall normal: 2
*
* "response" is NULL
*
* Valid errors:
*  SUCCESS
*  RADIO_NOT_AVAILABLE (radio resetting)
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_SET_OPRT_MODE (RIL_REQUEST_VENDOR_IVT_BASE+16)

/**
* RIL_REQUEST_ECALL_GET_OPRT_MODE
*
* Gets the service status for eCall operating mode
*
* "data" is NULL
*
* "response" is int*
*  ((int *)data)[0] is oprt_mode
*   eCall none: 0
*   eCall_only: 1
*   eCall and normal: 2
*
* Valid errors:
*  SUCCESS
*  RADIO_NOT_AVAILABLE (radio resetting)
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_GET_OPRT_MODE (RIL_REQUEST_VENDOR_IVT_BASE+17)

#define RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE (RIL_REQUEST_VENDOR_IVT_BASE+18)
#define RIL_REQUEST_LOCAL_REG_STATE_TYPE (RIL_REQUEST_VENDOR_IVT_BASE+19)
#define RIL_REQUEST_LOCAL_GET_OPERATOR_CODE (RIL_REQUEST_VENDOR_IVT_BASE+20)
#define RIL_REQUEST_LOCAL_SIGNAL_STRENGTH (RIL_REQUEST_VENDOR_IVT_BASE+21)
#ifdef UPDATE_APN
//only for telephony service
#define RIL_REQUEST_LOCAL_UPDATE_APN (RIL_REQUEST_VENDOR_IVT_BASE+22)
#define RIL_REQUEST_LOCAL_GET_APN (RIL_REQUEST_VENDOR_IVT_BASE+23)
#endif


/// @}

/// MT2635 vendor ril urcs @{
#define RIL_UNSOL_VENDOR_IVT_BASE 9500

/**
 * RIL_UNSOL_MAL_RESTART
 *
 * send when MAL restart
 *
 * "data" is NULL
 *
 */

#define RIL_UNSOL_MAL_RESTART (RIL_UNSOL_VENDOR_IVT_BASE + 0)

/**
* RIL_UNSOL_ECALL_MSD_STATUS
*
* Indicate if the IVS send MSD successfully
*
* "data" is a RIL_EcallMsdStatusIndMsg *
*
* Valid errors:
*  SUCCESS
*  RADIO_NOT_AVAILABLE (radio resetting)
*  GENERIC_FAILURE
*/
#define RIL_UNSOL_ECALL_MSD_STATUS (RIL_UNSOL_VENDOR_IVT_BASE + 1)
#define RIL_UNSOL_ECALL_MSDHACK (RIL_UNSOL_VENDOR_IVT_BASE + 1)

/**
 * RIL_UNSOL_ECALL_INDICATIONS
 *
 * eCall indication
 *
 * "data" is const RIL_Ecall_Unsol_Indications *
 *
 */
#define RIL_UNSOL_ECALL_INDICATIONS (RIL_UNSOL_VENDOR_IVT_BASE + 2)

/**
 * RIL_UNSOL_SETUP_PDN_STATE_CHANGED
 *
 * send when PND changed
 *
 * "data" is NULL
 * for internal use only, for SDK usage
 */
#define RIL_UNSOL_SETUP_PDN_STATE_CHANGED (RIL_UNSOL_VENDOR_IVT_BASE + 20)

/**
 * RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED
 *
 * send when pdn tear down
 *
 * "data" is NULL
 * for internal use only, for SDK usage
 */

#define RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED (RIL_UNSOL_VENDOR_IVT_BASE + 21)
//only for libvendor-ril
#define RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED (RIL_UNSOL_VENDOR_IVT_BASE + 22)

/// @}

/// M

#define MSD_MAX_LENGTH (140)

///-----------MT2635---------------------///
/// MT2635 vendor data call response @{
typedef struct {
    int             netId;      /* net id */
    int             pdnState;   /* Pdn state, the meaning pls refer to RIL_Data_Call_PdnState_2635*/
    int             status;     /* A RIL_DataCallFailCause, 0 which is PDP_FAIL_NONE if no error */
    int             cId;        /* Context ID, uniquely identifies this call */
    char *          apnType;    /* apn type name*/
    char *          type;       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". If status is
                                   PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED this is the type supported
                                   such as "IP" or "IPV6" */
    char *          ifname;     /* The network interface name */
    char *          addresses;  /* A space-delimited list of addresses with optional "/" prefix length,
                                   e.g., "192.0.1.3" or "192.0.1.11/16 2001:db8::1/64".
                                   May not be empty, typically 1 IPv4 or 1 IPv6 or
                                   one of each. If the prefix length is absent the addresses
                                   are assumed to be point to point with IPv4 having a prefix
                                   length of 32 and IPv6 128. */
    char *          dnses;      /* A space-delimited list of DNS server addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty. */
    char *          gateways;   /* A space-delimited list of default gateway addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty in which case the addresses represent point
                                   to point connections. */
    char *          pcscf;    /* the Proxy Call State Control Function address
                                 via PCO(Protocol Configuration Option) for IMS client. */
    int             mtu;        /* MTU received from network
                                   Value <= 0 means network has either not sent a value or
                                   sent an invalid value */
    int             mtuV6;
} MTK_Data_Call_Response_v1;
/// @}
#ifdef NET_SOCKET_SUPPORT
struct netSocketData {
    int32_t profileId;
    int32_t pdnState;
    int32_t failcause; /* A RIL_DataCallFailCause, 0 which is PDP_FAIL_NONE if no error */
    int32_t online;              /*!< online status. */
    char apnType[32];
    char type[32];
    char ifname[32];
    char addresses[128];
    char dnses[128];
    char gateways[128];
    char pcscf[32];
    int32_t mtu;
    int32_t mtuV6;
};
#endif

/// MT2635 vendor data call response @{
typedef enum {
    PDN_IDLE,
    PDN_CONNECTING,
    PDN_CONNECTED,
    PDN_DISCONNECTING,
    PDN_DISCONNECTED,
    PDN_RETRYING,
    PDN_FAILED,
    PDN_SCANNING,
} RIL_Data_Call_PdnState;
/// @}

/// ecall related info @{
typedef enum{
    EMER_CAT_MANUAL_ECALL = 1,      /* Manual Emergency eCall */
    EMER_CAT_AUTO_ECALL = 2,     /*  Automatic Emergency eCall */
}RIL_ECall_Category;


typedef enum{
    ECALL_TEST = 1,     /* Test eCall */
    ECALL_EMERGENCY = 2,      /* Emergency eCall */
    ECALL_RECONFIG = 3,     /*  Reconfiguration eCall */
}RIL_ECall_Variant;

typedef enum{
    ECALL_MSD_STATUS_SUCCESS = 0,   /* MSD send to PSAP success  */
    ECALL_MSD_STATUS_FAILURE = 1,   /* MSD send to PSAP failure  */
}RIL_Msd_Transmission_Status;

typedef enum{
    DOMAIN_AUTO = 0,      /* Automatic mode - LTE(IMS), WG(CS), 1x(C2K) */
    DOMAIN_CS_ONLY = 1,   /* CS domain only - WG(CS) */
    DOMAIN_3GPP_ONLY = 2, /* 3GPP only - LTE(IMS), WG(CS) */
    DOMAIN_3GPP2 = 3,     /* 3GPP2 only - 1x(C2K)) */
    DOMAIN_IMS_1xCS = 4,  /* IMS and 1x CS only - LTE(IMS), 1x(C2K) */
    DOMAIN_CS_1x = 5,     /* WG CS and 1x CS only - WG(CS), 1x(C2K) */
    DOMAIN_IMS_ONLY = 6,  /* only IMS call allowed */
}RIL_Dial_Domain;

typedef struct{
    int call_id;
    unsigned int length;
    unsigned char* msd_data;
}RIL_ECallSetMSD;

typedef struct{
    RIL_ECall_Category   ecall_cat;
    RIL_ECall_Variant   ecall_variant;
    char* address;
    unsigned int length;
    unsigned char* msd_data;
    RIL_Dial_Domain domain;
}RIL_ECallReqMsg;

typedef struct{
    int arg_num;
    int type;
    char* address;
}RIL_ECallSetNum;

typedef struct{
    RIL_Msd_Transmission_Status msd_tx_status;
    int call_id;
}RIL_EcallMsdStatusIndMsg;

typedef enum{
    RIL_UNSOL_ECALL_SENDING_START = 1,
    RIL_UNSOL_ECALL_SENDING_MSD = 2,
    RIL_UNSOL_ECALL_LLACK_RECEIVED = 3,
    RIL_UNSOL_ECALL_ALACK_POSITIVE_RECEIVED = 4,
    RIL_UNSOL_ECALL_ALACK_CLEARDOWN_RECEIVED = 5,
    RIL_UNSOL_ECALL_DIALING = 9,
    RIL_UNSOL_ECALL_ALERTING = 10,
    RIL_UNSOL_ECALL_ACTIVE = 11,
    RIL_UNSOL_ECALL_DISCONNECTED = 12,
    RIL_UNSOL_ECALL_IMS_ACTIVE = 13,
    RIL_UNSOL_ECALL_IMS_DISCONNECTED = 14,
    RIL_UNSOL_ECALL_ABNORMAL_HANGUP=15,
    RIL_UNSOL_ECALL_IMS_MSD_ACK = 20,
    RIL_UNSOL_ECALL_IMS_UPDATE_MSD = 21,
    RIL_UNSOL_ECALL_IMS_IN_BAND_TRANSFER = 22,
    RIL_UNSOL_ECALL_IMS_MSD_NACK = 23,
    RIL_UNSOL_ECALL_IMS_SRVCC = 24,
    RIL_UNSOL_ECALL_ONLY_DEREGISTRATION = 31,
    RIL_UNSOL_ECALL_MAY_DEREGISTER = 32,
    RIL_UNSOL_ECALL_PSAP_CALLBACK_START = 40,
    RIL_UNSOL_ECALL_PSAP_CALLBACK_IMS_UPDATE_MSD = 41,
    RIL_UNSOL_ECALL_T2_TIMEOUT = 52,
    RIL_UNSOL_ECALL_T5_TIMEOUT = 55,
    RIL_UNSOL_ECALL_T6_TIMEOUT = 56,
    RIL_UNSOL_ECALL_T7_TIMEOUT = 57,
    RIL_UNSOL_ECALL_UNSPECIFIED = 0xffff,
}RIL_ECall_Indication;

typedef struct{
    RIL_ECall_Indication ind;
    int call_id;
}RIL_Ecall_Unsol_Indications;

typedef enum {
    UNKNOWN_ECALL = 0,
    EN16454_ECALL,
    GOST_ECALL,
    NG_ECALL,
} RIL_Ecall_Type;
/// @}
#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MTK_RIL_EX_H_ */
