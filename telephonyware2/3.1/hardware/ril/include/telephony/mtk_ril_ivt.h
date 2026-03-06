/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

//#ifdef TBOX_remove
#ifndef MTK_RIL_IVT_H
#define MTK_RIL_IVT_H

typedef struct{
    int call_id;
    unsigned int length;
    unsigned char* msd_data;
}RIL_ECallSetMSD;

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
    ECALL_PSAP_PULL_MODE = 1,   /*  PSAP will PULL the data */
    ECALL_IVS_PUSH_MODE = 2,    /*  IVS should request PSAP to PULL the data */
}RIL_ECall_Ivs_Config_Mode;

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
    RIL_ECall_Category   ecall_cat;
    RIL_ECall_Variant   ecall_variant;
    char* address;
    unsigned int length;
    unsigned char* msd_data;
    RIL_Dial_Domain domain;
}RIL_ECallReqMsg;

typedef struct _ecall_ctrl_sequence_tmp{
    int     index;
    int     buff_size;
    char    *data;
}ecall_ctrl_sequence_tmp_t;

typedef enum{
    RIL_UNSOL_ECALL_SENDING_START = 1,
    RIL_UNSOL_ECALL_SENDING_MSD = 2,
    RIL_UNSOL_ECALL_LLACK_RECEIVED = 3,
    RIL_UNSOL_ECALL_ALACK_POSITIVE_RECEIVED = 4,
    RIL_UNSOL_ECALL_ALACK_CLEARDOWN_RECEIVED = 5,
    RIL_UNSOL_ECALL_CONNECTED = 11,
    RIL_UNSOL_ECALL_DISCONNECTED = 12,
    RIL_UNSOL_ECALL_IMS_ACTIVE = 13,
    RIL_UNSOL_ECALL_IMS_DISCONNECTED = 14,
    RIL_UNSOL_ECALL_ABNORMAL_HANGUP = 15,
    RIL_UNSOL_ECALL_IMS_MSD_ACK = 20,
    RIL_UNSOL_ECALL_IMS_UPDATE_MSD = 21,
    RIL_UNSOL_ECALL_IMS_IN_BAND_TRANSFER = 22,
    RIL_UNSOL_ECALL_IMS_MSD_NACK = 23,
    RIL_UNSOL_ECALL_IMS_SRVCC = 24,
    RIL_UNSOL_ECALL_IN_BAND_MSD_SUCCESS = 30,
    RIL_UNSOL_ECALL_ONLY_DEREGISTRATION = 31,
    RIL_UNSOL_ECALL_MAY_DEREGISTER = 32,
    RIL_UNSOL_ECALL_UNSPECIFIED = 0xffff,
}RIL_ECall_Indication;

typedef struct{
    RIL_ECall_Indication ind;
    int call_id;
}RIL_Ecall_Unsol_Indications;

typedef struct{
    int arg_num;
    int type;
    char* address;
}RIL_ECallSetNum;

#define MSD_MAX_LENGTH (140)

///-----------MTK-RIL-IVT------------///
/// MT2731 vendor ril requests @{
/**
 * RIL_REQUEST_VENDOR_IVT_BASE
 * RIL request sync with mtk ril
 */
#define RIL_REQUEST_VENDOR_IVT_BASE 9000
/// MT2731 vendor ril urcs @{
#define RIL_UNSOL_VENDOR_IVT_BASE 9500

#define RIL_LOCAL_REQUEST_GET_OPERATOR_NUMBER (RIL_REQUEST_VENDOR_IVT_BASE + 0)

/// M: Ecall @{
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
 * "data" is const char *
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


/**
* RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME
*
* REQUEST to set nad deregistration time for eCall
*
* "data" is const ints
* "response" is NULL
*
* Valid errors:
*  SUCCESS
*  RADIO_NOT_AVAILABLE (radio resetting)
*  GENERIC_FAILURE
*/
#define RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME (RIL_REQUEST_VENDOR_IVT_BASE+14)

/**
* RIL_REQUEST_ECALL_SET_REGISTRATION_STATE
*
* REQUEST to set the registration state of ecall only SIM
*
* "data" is int *
* ((const int *)data)[0] is state
*   0 - deregister from NW, enter the eCall inactivity procedure;
*   1 - register to NW, leave the eCall inactivity procedure (not support, reserved for future use)
* "response" is an int *
* (int *)response)[0]  is 0 for ecall only sim in ecall inactivity procedure
*                     and 1 for normal ecall sim/normal sim in deregistration state
*
* Valid errors:
*  SUCCESS
*  RADIO_NOT_AVAILABLE (radio resetting)
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

/// @}

#endif /*MTK_RIL_H*/
//#endif /*TBOX_remove*/
