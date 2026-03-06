/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
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
#ifndef __RMM_SML_DEFS_H__
#define __RMM_SML_DEFS_H__

/**
 * cat values in AT+ESMLCK = <cat>, <op>, [,<key> [,<data_imsi> [,<data_gid1> [, <data_gid2>]]]]
 */
enum {
    AT_ESMLCK_CAT_N = 0,    //network
    AT_ESMLCK_CAT_NS,       //network subset
    AT_ESMLCK_CAT_SP,       //Service Provider
    AT_ESMLCK_CAT_C,        //Corperate
    AT_ESMLCK_CAT_SIM,      //SIM
    AT_ESMLCK_CAT_NS_SP,    //MTK extended cat: mcc/mnc + IMSI digit (6, 7) + GID1
    AT_ESMLCK_CAT_SIM_C,    //MTK extended cat: IMSI + GID1 + GID2
    AT_ESMLCK_CAT_SIZE,     //reserved
    AT_ESMLCK_CAT_DUAL_SIM_LOCK_CONTROL,    //customer specific
    AT_ESMLCK_CAT_MASTER_KEY,               ////customer specific
}AT_ESMLCK_CAT;

/**
 * Operator Id
 */
typedef enum {          // Operator ID
    RSU_OP_VZW = 1,     // Verizon
    RSU_OP_KDDI = 2,    // KDDI
    RSU_OP_TMO = 3,     // TMO RSU
    RSU_OP_TFN = 6,     // Tracfone RSU
    RSU_OP_ATT_LEGACY = 100,   //ATT RSU with specific at command
    RSU_OP_TMO_LEGACY = 101,   //TMO RSU with specific at command
    RSU_OP_SPRINT_LEGACY = 102,   //SPRINT RSU with specific at command
    RSU_OP_JIO_LEGACY = 103,   //JIO Subsidy lock with specific at command
    RSU_OP_TF_LEGACY = 104,
    RSU_OP_COMMON = 200, //Common sim me lock request
} RIL_RSU_OPERATOR;

typedef enum {                        // Request function ID
    RSU_REQUEST_INIT_REQUEST = 0,     // VzW, TMO
    RSU_REQUEST_GET_SHARED_KEY,       // VzW(HMAC), TMO/ATT(SHARED KEY)
    RSU_REQUEST_UPDATE_LOCK_DATA,     // VzW, TMO, ATT, RJIO
    RSU_REQUEST_GET_LOCK_VERSION,     // VzW, TMO, ATT, RJIO
    RSU_REQUEST_RESET_LOCK_DATA,      // TMO, ATT
    RSU_REQUEST_GET_LOCK_STATUS,      // VzW, TMO, ATT, RJIO
    RSU_REQUEST_GET_MODEM_CONFIG,     //ATT
    RSU_REQUEST_GET_API_VERSION,      //ATT
    RSU_REQUEST_GET_SIM_SLOTS,        //ATT
    RSU_REQUEST_UNLOCK_TIMER = 50,    // VzW only now for delay timer. Not defined request
                                      // index by customer, extension start from index 50
    RSU_REQUEST_SET_SIM_LOCK_DATA,    // KDDI
    RSU_REQUEST_SET_GET_ALLOWED_LOCK_DATA,    // KDDI
    RSU_REQUEST_SET_GET_EXCLUDED_LOCK_DATA,    // KDDI
} RIL_RSU_REQUEST;

enum {
    TFN_RSU_REQUEST_GET_IMEI = 1,
    TFN_RSU_REQUEST_PROCESS_DATA,
    TFN_RSU_REQUEST_GET_LOCK_STATUS,
    TFN_RSU_REQUEST_GENERATE_REQUEST,
    TFN_RSU_REQUEST_RELOCK,
    TFN_RSU_REQUEST_GET_KEY,
};

/*
 * SPRINT/BOOST RSU REQUEST ID -- RSU_OP_SPRINT_LEGACY = 102
 */
/*
 * Get unlock code
 * Request:
 *     opId: RSU_OP_SPRINT_LEGACY -- 102
 *     requestId: RSU_REQUEST_GET_LOCK_STATUS -- 5
 * Response:
 *     status: 0/1/2
 *     reserveInt1: nvInfo
 *         lowest byte: nonceNV
 *             0 -- Failed to read from NVRAM
 *             1 -- Nonce is not stored
 *             2 -- Nonce is stored
 *         2nd byte: signatureNV
 *             0 -- Failed to read from NVRAM
 *             1 -- Signature is not stored
 *             2 -- Signature is stored with unlock code 0
 *             3 -- Signature is stored with unlock code 1
 *             4 -- Signature is stored with unlock code 2
 * AT CMD:
 *     +EUULK?
 * response: +EUULK:<unlock_code>
 * <unlock code>: integer type
 *     0 : device must only recognize and enable an inserted Sprint UICC.
 *     1 : device must recognize and enable any inserted international (Not a USA MCC) SIM
 *      as well as a Sprint SIM.
 *     2 : device must recognize any inserted UICC, International or Domestic.
 *
 * AT command
 *  AT+EUULK=4
 * response:
 *  +EUULK:<nonceNV>,<signatureNV>
 *  OK
 *
 *   <nonceNV>
 *       0 -- Failed to read from NVRAM
 *       1 -- Nonce is not stored
 *       2 -- Nonce is stored
 *   <signatureNV>
 *       0 -- Failed to read from NVRAM
 *       1 -- Signature is not stored
 *       2 -- Signature is stored with unlock code 0
 *       3 -- Signature is stored with unlock code 1
 *       4 -- Signature is stored with unlock code 2
 */
#define RSU_REQUEST_SPRINT_GET_UNLOCK_CODE      RSU_REQUEST_GET_LOCK_STATUS

/*
 * Get Nonce
 * Request:
 *     opId: RSU_OP_SPRINT_LEGACY -- 102
 *     requestId: RSU_REQUEST_GET_SHARED_KEY -- 1
 *     data: <device_id>
 * Response:
 *     data: <encrypted nonce>
 * AT CMD:
 *     +EUULK=1,<device id>
 * response: +EUULK: <encrypted nonce >
 * <device id>: String type
 *     The identifier of the Nonce generation, can use IMEI or MEID
 *     IMEI : string with 16 digit
 *     MEID : string with 15 digit and append ‘Z’ in the last.
 * <encrypted nonce>: String type
 *     The encrypted nonce will encode with B64 format
 */
#define RSU_REQUEST_SPRINT_GET_NONCE            RSU_REQUEST_GET_SHARED_KEY

/*
 * Verify Signature and then save unlock code, signature to NVRAM.
 *  If failed, reset all security context to default and lock the phone back to SPRINT only.
 * Request:
 *     opId: RSU_OP_SPRINT_LEGACY -- 102
 *     requestId: RSU_REQUEST_UPDATE_LOCK_DATA -- 2
 *     reserveInt1: <unlock_code>
 *     data: <signature>
 * Response:
 *     data: <encrypted nonce>
 * AT CMD:
 *     +EUULK=2,<unlock code>,<signature>
 * response:
 *     RIL_E_SUCCESS or error
 *
 * <signature>: String type
 *     The UICC Unlock Signature will be created using UICC Unlock Code and Nonce,
 *     encode with B64 format
 */
#define RSU_REQUEST_SPRINT_UPDATE_LOCK          RSU_REQUEST_UPDATE_LOCK_DATA

/*
 * COMMON RSU REQUEST ID -- RSU_OP_COMMON = 200
 * This is only used by MTK internally to get some sime lock status
 */
typedef enum {                            // Rsu common Request function ID
    /*
     * Get information of sim lock
     * Request:
     *     opId: RSU_OP_COMMON -- 200
     *     requestId: RSU_COMMON_REQUEST_GET_LOCK_INFO
     * Response:
     *     status:
     *         lowest word: Or bits of supported categories
     *             enum {
     *                SML_CATEGORY_3GPP_NW = 0,
     *                SML_CATEGORY_3GPP_NS,
     *                SML_CATEGORY_3GPP_SP,
     *                SML_CATEGORY_3GPP_CP,
     *                SML_CATEGORY_3GPP_SIM,
     *                SML_CATEGORY_NS_SP,
     *                SML_CATEGORY_SIM_C,
     *             }
     *         3ird byte: carrierId
     *             enum {
     *                  SML_CARRIER_NONE = 0,
     *                  SML_CARRIER_SPRINT_RSU,
     *                  SML_CARRIER_TMO_TT_RSU,
     *                  SML_CARRIER_TMO_MODEM_RSU,
     *                  SML_CARRIER_TFN,
     *                  SML_CARRIER_VZW_RSU,
     *                  SML_CARRIER_ATT_RSU,
     *                  SML_CARRIER_OTHERS = 254,
     *                  SML_CARRIER_KUNOWN = 255,
     *              }SML_CARRIER_ID;
     *         24th ~ 27th bit: sim me lock status
     *             0 -- unlocked
     *             1 -- locked
     *         28th bit: lockFuse.
     *             0 -- not fused
     *             1 -- fused build
     *         29th bit: unlock by passward or RSU
     *             0 -- passward
     *             1 -- rsu
     *     version: version
     *     reserveInt1:
     *         lowest byte: lock policy, see definition of SmlSlotLockPolicy.
     *             0x00 -- SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS,
     *                     All SIM slots require to have a valid carrier SIM
     *             0x01 -- SIMLOCK_SLOT_POLICY_UNIQUE_FOR_EACH_SLOT,
     *                     Each SIM slot is locked to a different carrier
     *             0x02 -- SIMLOCK_SLOT_POLICY_LOCK_ONLY_SLOT_1,
     *                     only primary slot requres a valid carrier SIM
     *             0x03 -- SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_SLOT_1,
     *                     if primary slot has a valid carrier SIM other slots work with any SIM
     *             0x04 -- SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_A_SLOT,
     *                     if any slot has a valid carrier SIM other slots work with any SIM
     *         2nd byte: sub policy
	 *             0x00 SIMLOCK_SUBSCRIPTION_POLICY_INDEPENDENT
	 *                      doesn't matter what APPs are available on the SIM
	 *             0x01 SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_REQUIRED
	 *                      USIM APP must be present on the SIM
	 *             0x02 SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP2_REQUIRED
	 *                      CSIM APP must be present on the SIM
	 *             0x03 SIMLOCK_SUBSCRIPTION_POLICY_VALID_BOTH_REQUIRED
	 *                      Both USIM and CSIM APPs must be present on the SIM
	 *             0x04 SIMLOCK_SUBSCRIPTION_POLICY_VALID_3GPP_OR_3GPP2_REQUIRED
	 *                      at least on of  USIM or CSIM APPs must be present on the SIM
     */
    RSU_COMMON_REQUEST_GET_LOCK_INFO = 1,
    /*
     * Get information of specific category
     * Request:
     *     opId: RSU_OP_COMMON -- 200
     *     requestId: RSU_COMMON_REQUEST_GET_CAT_INFO
     *     reserveInt1: category
     * Response:
     *     status:
     *         lowest byte: cat
     *         2nd byte: max_retry
     *         3ird byte: remain_retry
     *         24th bit: is blacklist or not.
     *         25th bit: is allowed to reactiviate.
     *         26th bit: is locked or not
     */
    RSU_COMMON_REQUEST_GET_CAT_INFO = 2,
    /*
     * Verify pin
     * Request:
     *     opId: RSU_OP_COMMON -- 200
     *     requestId: RSU_COMMON_REQUEST_VERIFY_PIN
     *     data: pin
     *     reserveInt1: lockType -- RIL_RSU_LOCK_TYPE
     * Response:
     *     success or fail
     */
    RSU_COMMON_REQUEST_VERIFY_PIN = 3,
    /*
     * Get information of specific category
     * Request:
     *     opId: RSU_OP_COMMON -- 200
     *     requestId: RSU_COMMON_REQUEST_GET_LOCK_STATUS
     * Response:
     *     status:
     *        0 -- locked
     *        1 -- temporary unlocked
     *        2 -- permanent unlocked
     *       -1 -- error
     */
    RSU_COMMON_REQUEST_GET_LOCK_STATUS = 4,
} RIL_RSU_COMMON_REQUEST;

//Components of RsuResponse.status for COMMON_REQUEST_GET_LOCK_INFO
#define LOCK_INFO_SUPPORTED_CATS_MASK       0xFFFF
#define LOCK_INFO_SUPPORTED_CATS_OFFSET     0
#define LOCK_INFO_CARRIER_MASK              0xFF0000
#define LOCK_INFO_CARRIER_OFFSET            16
#define LOCK_INFO_LOCK_STATUS_MASK          0xF000000
#define LOCK_INFO_LOCK_STATUS_OFFSET        24
#define LOCK_INFO_LOCK_FUSE_MASK            (0x1 << 28)
#define LOCK_INFO_LOCK_FUSE_OFFSET          28
#define LOCK_INFO_IS_RSU_MASK               (0x1 << 29)
#define LOCK_INFO_IS_RSU_OFFSET             29
//Components of RsuResponse.status for MESSAGE_COMMON_GET_CAT_INFO
#define CAT_INFO_CAT_MASK                   0xFF
#define CAT_INFO_CAT_OFFSET                 0
#define CAT_INFO_MAX_RETRY_MASK             0xFF00
#define CAT_INFO_MAX_RETRY_OFFSET           8
#define CAT_INFO_REMAIN_RETRY_MASK          0xFF0000
#define CAT_INFO_REMAIN_RETRY_OFFSET        16
#define CAT_INFO_IS_BLACKLIST_MASK          (0x1 << 24)
#define CAT_INFO_IS_BLACKLIST_OFFSET        24
#define CAT_INFO_IS_REACTIVE_MASK           (0x1 << 25)
#define CAT_INFO_IS_REACTIVE_OFFSET         25
#define CAT_INFO_IS_LOCKED_MASK             (0x1 << 26)
#define CAT_INFO_IS_LOCKED_OFFSET           26

/**
 * Available values for lockType of RSU_COMMON_REQUEST_VERIFY_PIN
 */
typedef enum {
    RIL_RSU_LOCK_TYPE_INALID = 0,
    RIL_RSU_LOCK_TYPE_TF_STATUS = 1,
} RIL_RSU_LOCK_TYPE;

/**
 * Request ID for JIO subsidy lock
 */
typedef enum {
    SUBSIDY_UPDATE_SETTINGS = 1,    //AT+ESLBLOB=1,<data_length>,<lock_data>,<is_last>
    SUBSIDY_GET_MODEM_STATUS = 2,   //AT+ESLBLOB=2
    SUBSIDY_GET_VERSION = 3,        //AT+ESLBLOB=6
    SUBSIDY_GET_UNLOCK_TIME = 4,    //AT+ESLBLOB=7
} RIL_RSU_REQUEST_JIO;

/**
 * carrier_id availabe values in +ESMLGEN:<carrier_id>
 */
enum {
    SML_CARRIER_NONE = 0,
    SML_CARRIER_SPRINT_RSU,
    SML_CARRIER_TMO_TT_RSU,
    SML_CARRIER_TMO_MODEM_RSU,
    SML_CARRIER_TFN,
    SML_CARRIER_VZW_RSU,
    SML_CARRIER_ATT_RSU,
    SML_CARRIER_OTHERS = 254,
    SML_CARRIER_KUNOWN = 255,
}SML_CARRIER_ID;
#endif /* __RMM_SML_DEFS_H__ */
