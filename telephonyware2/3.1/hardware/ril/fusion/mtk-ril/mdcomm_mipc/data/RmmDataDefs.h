/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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

#ifndef __RMM_DATA_DEFS_H__
#define __RMM_DATA_DEFS_H__

#include <telephony/mtk_ril.h>
#include "NetAgentCommon.h"

#define ME_PDN_ACT "ME PDN ACT"
#define NW_ACT "NW ACT"
#define NW_MODIFY "NW MODIFY"
#define NW_PDN_ACT "NW PDN ACT"
#define NW_PDN_DEACT "NW PDN DEACT"
#define ME_PDN_DEACT "ME PDN DEACT"
#define NW_DEACT "NW DEACT"
#define ME_DEACT "ME DEACT"
#define NW_REACT "NW REACT"

// M: Test mode.
#define CLEAR_PDN_TABLE "CLEAR PDN TABLE"
#define CLEAR_ALL_PDN_TABLE "CLEAR ALL PDN TABLE"
#define SET_TRANSACTION_INTERFACE_ID "SET TRANSACTION INTERFACE ID"
#define REMOVE_TRANSACTION_INTERFACE_ID "REMOVE TRANSACTION INTERFACE ID"
#define REMOVE_ALL_TRANSACTION_INTERFACE_ID "REMOVE ALL TRANSACTION INTERFACE ID"

#define MAX_APN_NAME_LENGTH 100

#define AUTHTYPE_NONE       0
#define AUTHTYPE_PAP        1
#define AUTHTYPE_CHAP       2
#define AUTHTYPE_PAP_CHAP   3

#define SETUP_DATA_PROTOCOL_IP      "IP"
#define SETUP_DATA_PROTOCOL_IPV6    "IPV6"
#define SETUP_DATA_PROTOCOL_IPV4V6  "IPV4V6"

#define INVALID_AID -1
#define INVALID_INTERFACE_ID -1
#define INVALID_TRANS_INTF_ID -1
#define INVALID_LIFE_TIME -1
#define INVALID_VALUE -1

#define INVALID_PDN_TYPE 0
#define IPV4        1
#define IPV6        2
#define IPV4V6      3

#define NULL_IPV4_ADDRESS "0.0.0.0"
#define MAX_IPV4_ADDRESS_LENGTH 17
//xxx.xxx.xxx.xxx
#define MAX_IPV6_ADDRESS_LENGTH 65
//xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx
//xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx

#define MAX_NUM_IPV6_ADDRESS_NUMBER 2
#define MAX_NUM_DNS_ADDRESS_NUMBER 3

#define PROPERTY_DATA_NETWORK_TYPE "gsm.network.type"

#define IMS_EIMS_PDN_INFO "vendor.ims.eims.pdn.info"

#define OPERATOR_OP01 "OP01"
#define OPERATOR_OP07 "OP07"
#define OPERATOR_OP08 "OP08"
#define OPERATOR_OP09 "OP09"
#define OPERATOR_OP12 "OP12"
#define OPERATOR_OP20 "OP20"
#define SEGDEFAULT "SEGDEFAULT"

#define IPV6_PREFIX "FE80:0000:0000:0000:"
#define NULL_ADDR ""

#define REQUEST_TYPE_NORMAL 0
#define REQUEST_TYPE_HANDOVER 3

#define DC_NEW "new"
#define DC_UPDATE "update"
#define DC_CHANGE "dcchg"
#define DC_ERROR "err"
#define DC_DEACT_INFO "deact_info"
#define DC_ADDRESS "addr"
#define DC_RAN "ran"
#define DC_ENTERPRISE "ENTERPRISE"
#define DC_CBS "CBS"

#define BEARER_BITMASK_OFFSET 11
#define ALL_BEARER_EXCLUDE_IWLAN_BITMASK 0x7ffdffff
#define ALL_BEARER_BITMASK 0x7fffffff

#define TRANSACTION_ID_OFFSET 100

#define NO_REASON 0

#define PREFER_DATA_SLOT_PROPERTY "vendor.ril.data.prefer_data_slot"

// Fast dormancy begin
#define PROPERTY_MTK_FD_SUPPORT    "ro.vendor.mtk_fd_support"
#define PROPERTY_FD_ON_ONLY_R8_NETWORK  "persist.vendor.radio.fd.on.only.r8"
#define PROPERTY_RIL_FD_MODE       "vendor.ril.fd.mode"

#define PROPERTY_FD_SCREEN_ON_TIMER     "persist.vendor.radio.fd.counter"
#define PROPERTY_FD_SCREEN_ON_R8_TIMER  "persist.vendor.radio.fd.r8.counter"
#define PROPERTY_FD_SCREEN_OFF_TIMER    "persist.vendor.radio.fd.off.counter"
#define PROPERTY_FD_SCREEN_OFF_R8_TIMER "persist.vendor.radio.fd.off.r8.counter"

/* Fast dormancy related timer: units: 0.1 sec */
#define DEFAULT_FD_SCREEN_ON_TIMER "150"
#define DEFAULT_FD_SCREEN_ON_R8_TIMER "150"
#define DEFAULT_FD_SCREEN_OFF_TIMER "50"
#define DEFAULT_FD_SCREEN_OFF_R8_TIMER "50"

#define RF_OCCUPY_EXPIRATION_TIME 7000e6
#define MSPM_SESSION_PROCEDURE_NDDS_DATA_VALIDATION "MSPM_NDDS_DATA_VALIDATION_SESSION"

#define STRING_SBP_L5_AUTO_DEACT_RSP "SBP_L5_AUTO_DEACT_RSP"

static const char PROPERTY_FD_TIMER[4][50] = {
    PROPERTY_FD_SCREEN_OFF_TIMER,
    PROPERTY_FD_SCREEN_ON_TIMER,
    PROPERTY_FD_SCREEN_OFF_R8_TIMER,
    PROPERTY_FD_SCREEN_ON_R8_TIMER
};

#define FASTDORMANCY_ON 0x3FFFFF
#define FASTDORMANCY_LEGACY_OFF 0x400000
#define FASTDORMANCY_LOW_DATA_EXPECTED 2
#define FASTDORMANCY_PARAMETER_LENGTH 2
#define FASTDORMANCY_SCREEN_STATE_INDEX 3
// Fast dormancy end

#define COUNT_LINK_CAPACITY_ESTIMATE 4
#define COUNT_NETWORK_REJECT_CAUSE 3
#define COUNT_MOBILE_DATA_USAGE 4
#define COUNT_DSDA_STATUS 4
#define DSDA_BASE_NEW_VERSION 2

#define BTOS(x) ((x) ? "true" : "false")

#define AT_LINE_FREE(line)   \
if (line) {                  \
    delete line;             \
    line = NULL;             \
}

#define FREEIF(data)    \
if (data != NULL) {     \
    free(data);         \
    data = NULL;        \
}

#define UNSPECIFIED_IPV6_GATEWAY    "::"

#define CME_CAUSE_BASE_V14 0x64 // Defined in 3GPP TS 27.007 V14

#define PDN_HANDOVER_INFO_SIZE 3
#define PDN_HANDOVER_INFO_CID 0
#define PDN_HANDOVER_INFO_OLD_RAT 1
#define PDN_HANDOVER_INFO_NEW_RAT 2

#define PCO_OPTION_FF00 "FF00"
#define PCO_OPTION_FF01 "FF01"
#define PCO_OPTION_FF02 "FF02"
#define PCO_OPTION_FF03 "FF03"

typedef enum {
    ACT_CNF = 0,
    GET_CNF = 1,
    ACT_IND = 2,
    MOD_IND = 3
} DCProfileSource;

#define MIPC_DATA_T_ID(SRC) MIPC_DATA_##SRC##_T_ID
#define DC_T_ID(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_ID(ACT_CALL_CNF) : \
                     (SRC==GET_CNF ? MIPC_DATA_T_ID(GET_CALL_CNF) : \
                     (SRC==ACT_IND ? MIPC_DATA_T_ID(ACT_CALL_IND) : \
                      MIPC_DATA_T_ID(MOD_CALL_IND))))

#define MIPC_DATA_T_APN(SRC) MIPC_DATA_##SRC##_T_APN
#define DC_T_APN(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_APN(ACT_CALL_CNF) : \
                      (SRC==GET_CNF ? MIPC_DATA_T_APN(GET_CALL_CNF) : \
                      (SRC==ACT_IND ? MIPC_DATA_T_APN(ACT_CALL_IND) : \
                       MIPC_DATA_T_APN(MOD_CALL_IND))))

#define MIPC_DATA_T_PDP_TYPE(SRC) MIPC_DATA_##SRC##_T_PDP_TYPE
#define DC_T_PDP_TYPE(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PDP_TYPE(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_PDP_TYPE(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_PDP_TYPE(ACT_CALL_IND) : \
                            MIPC_DATA_T_PDP_TYPE(MOD_CALL_IND))))

#define MIPC_DATA_T_V4_COUNT(SRC) MIPC_DATA_##SRC##_T_V4_COUNT
#define DC_T_V4_COUNT(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V4_COUNT(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_V4_COUNT(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_V4_COUNT(ACT_CALL_IND) : \
                            MIPC_DATA_T_V4_COUNT(MOD_CALL_IND))))

#define MIPC_DATA_T_V4_0(SRC) MIPC_DATA_##SRC##_T_V4_0
#define DC_T_V4_0(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V4_0(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V4_0(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V4_0(ACT_CALL_IND) : \
                        MIPC_DATA_T_V4_0(MOD_CALL_IND))))

#define MIPC_DATA_T_V4_1(SRC) MIPC_DATA_##SRC##_T_V4_1
#define DC_T_V4_1(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V4_1(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V4_1(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V4_1(ACT_CALL_IND) : \
                        MIPC_DATA_T_V4_1(MOD_CALL_IND))))

#define MIPC_DATA_T_V4_2(SRC) MIPC_DATA_##SRC##_T_V4_2
#define DC_T_V4_2(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V4_2(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V4_2(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V4_2(ACT_CALL_IND) : \
                        MIPC_DATA_T_V4_2(MOD_CALL_IND))))

#define MIPC_DATA_T_V4_3(SRC) MIPC_DATA_##SRC##_T_V4_3
#define DC_T_V4_3(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V4_3(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V4_3(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V4_3(ACT_CALL_IND) : \
                        MIPC_DATA_T_V4_3(MOD_CALL_IND))))

#define MIPC_DATA_T_V6_COUNT(SRC) MIPC_DATA_##SRC##_T_V6_COUNT
#define DC_T_V6_COUNT(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V6_COUNT(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_V6_COUNT(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_V6_COUNT(ACT_CALL_IND) : \
                            MIPC_DATA_T_V6_COUNT(MOD_CALL_IND))))

#define MIPC_DATA_T_V6_0(SRC) MIPC_DATA_##SRC##_T_V6_0
#define DC_T_V6_0(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V6_0(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V6_0(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V6_0(ACT_CALL_IND) : \
                        MIPC_DATA_T_V6_0(MOD_CALL_IND))))

#define MIPC_DATA_T_V6_1(SRC) MIPC_DATA_##SRC##_T_V6_1
#define DC_T_V6_1(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V6_1(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V6_1(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V6_1(ACT_CALL_IND) : \
                        MIPC_DATA_T_V6_1(MOD_CALL_IND))))

#define MIPC_DATA_T_V6_2(SRC) MIPC_DATA_##SRC##_T_V6_2
#define DC_T_V6_2(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V6_2(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V6_2(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V6_2(ACT_CALL_IND) : \
                        MIPC_DATA_T_V6_2(MOD_CALL_IND))))

#define MIPC_DATA_T_V6_3(SRC) MIPC_DATA_##SRC##_T_V6_3
#define DC_T_V6_3(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_V6_3(ACT_CALL_CNF) : \
                       (SRC==GET_CNF ? MIPC_DATA_T_V6_3(GET_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_V6_3(ACT_CALL_IND) : \
                        MIPC_DATA_T_V6_3(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V4_COUNT(SRC) MIPC_DATA_##SRC##_T_DNS_V4_COUNT
#define DC_T_DNS_V4_COUNT(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V4_COUNT(ACT_CALL_CNF) : \
                               (SRC==GET_CNF ? MIPC_DATA_T_DNS_V4_COUNT(GET_CALL_CNF) : \
                               (SRC==ACT_IND ? MIPC_DATA_T_DNS_V4_COUNT(ACT_CALL_IND) : \
                                MIPC_DATA_T_DNS_V4_COUNT(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V4_0(SRC) MIPC_DATA_##SRC##_T_DNS_V4_0
#define DC_T_DNS_V4_0(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V4_0(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V4_0(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V4_0(ACT_CALL_IND) : \
                            MIPC_DATA_T_DNS_V4_0(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V4_1(SRC) MIPC_DATA_##SRC##_T_DNS_V4_1
#define DC_T_DNS_V4_1(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V4_1(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V4_1(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V4_1(ACT_CALL_IND) : \
                            MIPC_DATA_T_DNS_V4_1(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V4_2(SRC) MIPC_DATA_##SRC##_T_DNS_V4_2
#define DC_T_DNS_V4_2(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V4_2(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V4_2(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V4_2(ACT_CALL_IND) : \
                            MIPC_DATA_DNS_T_V4_2(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V4_3(SRC) MIPC_DATA_##SRC##_T_DNS_V4_3
#define DC_T_DNS_V4_3(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V4_3(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V4_3(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V4_3(ACT_CALL_IND) : \
                            MIPC_DATA_T_DNS_V4_3(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V6_COUNT(SRC) MIPC_DATA_##SRC##_T_DNS_V6_COUNT
#define DC_T_DNS_V6_COUNT(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V6_COUNT(ACT_CALL_CNF) : \
                               (SRC==GET_CNF ? MIPC_DATA_T_DNS_V6_COUNT(GET_CALL_CNF) : \
                               (SRC==ACT_IND ? MIPC_DATA_T_DNS_V6_COUNT(ACT_CALL_IND) : \
                                MIPC_DATA_T_DNS_V6_COUNT(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V6_0(SRC) MIPC_DATA_##SRC##_T_DNS_V6_0
#define DC_T_DNS_V6_0(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V6_0(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V6_0(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V6_0(ACT_CALL_IND) : \
                            MIPC_DATA_T_DNS_V6_0(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V6_1(SRC) MIPC_DATA_##SRC##_T_DNS_V6_1
#define DC_T_DNS_V6_1(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V6_1(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V6_1(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V6_1(ACT_CALL_IND) : \
                            MIPC_DATA_T_DNS_V6_1(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V6_2(SRC) MIPC_DATA_##SRC##_T_DNS_V6_2
#define DC_T_DNS_V6_2(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V6_2(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V6_2(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V6_2(ACT_CALL_IND) : \
                            MIPC_DATA_T_DNS_V6_2(MOD_CALL_IND))))

#define MIPC_DATA_T_DNS_V6_3(SRC) MIPC_DATA_##SRC##_T_DNS_V6_3
#define DC_T_DNS_V6_3(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_DNS_V6_3(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_DNS_V6_3(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_DNS_V6_3(ACT_CALL_IND) : \
                            MIPC_DATA_T_DNS_V6_3(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V4_COUNT(SRC) MIPC_DATA_##SRC##_T_PCSCF_V4_COUNT
#define DC_T_PCSCF_V4_COUNT(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V4_COUNT(ACT_CALL_CNF) : \
                                 (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V4_COUNT(GET_CALL_CNF) : \
                                 (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V4_COUNT(ACT_CALL_IND) : \
                                  MIPC_DATA_T_PCSCF_V4_COUNT(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V4_0(SRC) MIPC_DATA_##SRC##_T_PCSCF_V4_0
#define DC_T_PCSCF_V4_0(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V4_0(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V4_0(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V4_0(ACT_CALL_IND) : \
                              MIPC_DATA_T_PCSCF_V4_0(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V4_1(SRC) MIPC_DATA_##SRC##_T_PCSCF_V4_1
#define DC_T_PCSCF_V4_1(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V4_1(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V4_1(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V4_1(ACT_CALL_IND) : \
                              MIPC_DATA_T_PCSCF_V4_1(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V4_2(SRC) MIPC_DATA_##SRC##_T_PCSCF_V4_2
#define DC_T_PCSCF_V4_2(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V4_2(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V4_2(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V4_2(ACT_CALL_IND) : \
                              MIPC_DATA_PCSCF_T_V4_2(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V4_3(SRC) MIPC_DATA_##SRC##_T_PCSCF_V4_3
#define DC_T_PCSCF_V4_3(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V4_3(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V4_3(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V4_3(ACT_CALL_IND) : \
                              MIPC_DATA_T_PCSCF_V4_3(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V6_COUNT(SRC) MIPC_DATA_##SRC##_T_PCSCF_V6_COUNT
#define DC_T_PCSCF_V6_COUNT(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V6_COUNT(ACT_CALL_CNF) : \
                                 (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V6_COUNT(GET_CALL_CNF) : \
                                 (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V6_COUNT(ACT_CALL_IND) : \
                                  MIPC_DATA_T_PCSCF_V6_COUNT(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V6_0(SRC) MIPC_DATA_##SRC##_T_PCSCF_V6_0
#define DC_T_PCSCF_V6_0(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V6_0(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V6_0(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V6_0(ACT_CALL_IND) : \
                              MIPC_DATA_T_PCSCF_V6_0(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V6_1(SRC) MIPC_DATA_##SRC##_T_PCSCF_V6_1
#define DC_T_PCSCF_V6_1(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V6_1(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V6_1(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V6_1(ACT_CALL_IND) : \
                              MIPC_DATA_T_PCSCF_V6_1(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V6_2(SRC) MIPC_DATA_##SRC##_T_PCSCF_V6_2
#define DC_T_PCSCF_V6_2(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V6_2(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V6_2(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V6_2(ACT_CALL_IND) : \
                              MIPC_DATA_T_PCSCF_V6_2(MOD_CALL_IND))))

#define MIPC_DATA_T_PCSCF_V6_3(SRC) MIPC_DATA_##SRC##_T_PCSCF_V6_3
#define DC_T_PCSCF_V6_3(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_PCSCF_V6_3(ACT_CALL_CNF) : \
                             (SRC==GET_CNF ? MIPC_DATA_T_PCSCF_V6_3(GET_CALL_CNF) : \
                             (SRC==ACT_IND ? MIPC_DATA_T_PCSCF_V6_3(ACT_CALL_IND) : \
                              MIPC_DATA_T_PCSCF_V6_3(MOD_CALL_IND))))

#define MIPC_DATA_T_GW_V4(SRC) MIPC_DATA_##SRC##_T_GW_V4
#define DC_T_GW_V4(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_GW_V4(ACT_CALL_CNF) : \
                        (SRC==GET_CNF ? MIPC_DATA_T_GW_V4(GET_CALL_CNF) : \
                        (SRC==ACT_IND ? MIPC_DATA_T_GW_V4(ACT_CALL_IND) : \
                         MIPC_DATA_T_GW_V4(MOD_CALL_IND))))

#define MIPC_DATA_T_GW_V6(SRC) MIPC_DATA_##SRC##_T_GW_V6
#define DC_T_GW_V6(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_GW_V6(ACT_CALL_CNF) : \
                        (SRC==GET_CNF ? MIPC_DATA_T_GW_V6(GET_CALL_CNF) : \
                        (SRC==ACT_IND ? MIPC_DATA_T_GW_V6(ACT_CALL_IND) : \
                         MIPC_DATA_T_GW_V6(MOD_CALL_IND))))

#define MIPC_DATA_T_MTU_V4(SRC) MIPC_DATA_##SRC##_T_MTU_V4
#define DC_T_MTU_V4(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_MTU_V4(ACT_CALL_CNF) : \
                         (SRC==GET_CNF ? MIPC_DATA_T_MTU_V4(GET_CALL_CNF) : \
                         (SRC==ACT_IND ? MIPC_DATA_T_MTU_V4(ACT_CALL_IND) : \
                          MIPC_DATA_T_MTU_V4(MOD_CALL_IND))))

#define MIPC_DATA_T_MTU_V6(SRC) MIPC_DATA_##SRC##_T_MTU_V6
#define DC_T_MTU_V6(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_MTU_V6(ACT_CALL_CNF) : \
                         (SRC==GET_CNF ? MIPC_DATA_T_MTU_V6(GET_CALL_CNF) : \
                         (SRC==ACT_IND ? MIPC_DATA_T_MTU_V6(ACT_CALL_IND) : \
                          MIPC_DATA_T_MTU_V6(MOD_CALL_IND))))

#define MIPC_DATA_T_INTERFACE_ID(SRC) MIPC_DATA_##SRC##_T_INTERFACE_ID
#define DC_T_INTERFACE_ID(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_INTERFACE_ID(ACT_CALL_CNF) : \
                               (SRC==GET_CNF ? MIPC_DATA_T_INTERFACE_ID(GET_CALL_CNF) : \
                               (SRC==ACT_IND ? MIPC_DATA_T_INTERFACE_ID(ACT_CALL_IND) : \
                                MIPC_DATA_T_INTERFACE_ID(MOD_CALL_IND))))

#define MIPC_DATA_T_P_ID(SRC) MIPC_DATA_##SRC##_T_P_ID
#define DC_T_P_ID(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_P_ID(ACT_CALL_CNF) : \
                       (SRC==ACT_IND ? MIPC_DATA_T_P_ID(ACT_CALL_IND) : \
                        MIPC_DATA_T_P_ID(MOD_CALL_IND)))

#define MIPC_DATA_T_FB_ID(SRC) MIPC_DATA_##SRC##_T_FB_ID
#define DC_T_FB_ID(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_FB_ID(ACT_CALL_CNF) : \
                        MIPC_DATA_T_FB_ID(ACT_CALL_IND))

#define MIPC_DATA_T_TRANS_ID(SRC) MIPC_DATA_##SRC##_T_TRANS_ID
#define DC_T_TRANS_ID(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_TRANS_ID(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_TRANS_ID(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_TRANS_ID(ACT_CALL_IND) : \
                            MIPC_DATA_T_TRANS_ID(MOD_CALL_IND))))

#define MIPC_DATA_T_RAN_INFO(SRC) MIPC_DATA_##SRC##_T_RAN_INFO
#define DC_T_RAN_INFO(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_RAN_INFO(ACT_CALL_CNF) : \
                           (SRC==GET_CNF ? MIPC_DATA_T_RAN_INFO(GET_CALL_CNF) : \
                           (SRC==ACT_IND ? MIPC_DATA_T_RAN_INFO(ACT_CALL_IND) : \
                            MIPC_DATA_T_RAN_INFO(MOD_CALL_IND))))


#define MIPC_DATA_T_BEARER_ID(SRC) MIPC_DATA_##SRC##_T_BEARER_ID
#define DC_T_BEARER_ID(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_BEARER_ID(ACT_CALL_CNF) : \
                            (SRC==GET_CNF ? MIPC_DATA_T_BEARER_ID(GET_CALL_CNF) : \
                            (SRC==ACT_IND ? MIPC_DATA_T_BEARER_ID(ACT_CALL_IND) : \
                             MIPC_DATA_T_BEARER_ID(MOD_CALL_IND))))

#define MIPC_DATA_T_IM_CN_SIGNALLING_FLAG(SRC) MIPC_DATA_##SRC##_T_IM_CN_SIGNALLING_FLAG
#define DC_T_IM_CN_SIGNALLING_FLAG(SRC) (SRC==ACT_CNF ? MIPC_DATA_T_IM_CN_SIGNALLING_FLAG(ACT_CALL_CNF) : \
                            (SRC==GET_CNF ? MIPC_DATA_T_IM_CN_SIGNALLING_FLAG(GET_CALL_CNF) : \
                            (SRC==ACT_IND ? MIPC_DATA_T_IM_CN_SIGNALLING_FLAG(ACT_CALL_IND) : \
                             MIPC_DATA_T_IM_CN_SIGNALLING_FLAG(MOD_CALL_IND))))

typedef struct {
    int transIntfId; // trans_id * 100 + interface_id
    int interfaceId;
    int primaryAid;
    int aid;
    int ddcId;  // for dedicate bearer
    int isDedicateBearer;
    int isEmergency;  // 0: normal, 1: emergency PDN
    int active;  // 0: inactive, 1: link down, 2: active
    int signalingFlag;
    char apn[MAX_APN_NAME_LENGTH];
    char addressV4[MAX_IPV4_ADDRESS_LENGTH];
    Ipv6Info addressV6Info[MAX_NUM_IPV6_ADDRESS_NUMBER];
    char dnsV4[MAX_NUM_DNS_ADDRESS_NUMBER][MAX_IPV4_ADDRESS_LENGTH];
    char dnsV6[MAX_NUM_DNS_ADDRESS_NUMBER][MAX_IPV6_ADDRESS_LENGTH];
    int bearerId;
    char pcscf[MAX_PCSCF_NUMBER][MAX_IPV6_ADDRESS_LENGTH];
    int mtuV4;
    int mtuV6;
    int rat;    /* 0: UNSPEC, 1: MOBILE_3GPP, 2: WIFI, 3: MOBILE_3GPP2
                   4: TYPE_MAX, 0xFE: RDS_RAN_NONE, 0xFF: RDS_RAN_DENY */
    int reason;  // -1: no cause, 0: ipv4 only, 1: ipv6 only, 2: single bearer only allowed
    int deactReason;  // for pdn deact reason
    int pdnType; //for pdn type
    int profileId;
    int sscMode;
    int accessNetwork;      /* Only for QOS
                               AOSP_V1_5::RadioAccessNetworks to 3gpp 27.007 Sec 7.3
                               RadioAccessNetworks 0: UNKNOWN
                               RadioAccessNetworks 1: GERAN (0 GSM)
                               RadioAccessNetworks 2: UTRAN (2 UTRAN)
                               RadioAccessNetworks 3: EUTRAN(7 E-UTRAN)
                               RadioAccessNetworks 4: NGRAN (12 NG-RAN) */

    RIL_Qos * defaultQos;   /* Default bearer QoS. Applicable to LTE and NR */

    int qosSessionCount;
    RIL_QosSession * qosSession;/* Qos Sessions associated with a dedicated bearer*/

    int handoverFailureMode;    /* Specifies the fallback mode on an IWLAN
                                   handover failure. LEGACY = 0, DO_FALLBACK = 1,
                                   NO_FALLBACK_RETRY_HANDOVER = 2
                                   NO_FALLBACK_RETRY_SETUP_NORMAL = 3*/
    int pduSessionId;           /* The allocated pdu session id for this data call.
                                   A value of 0 means no pdu session id was
                                   attached to this call.
                                   Reference: 3GPP TS 24.007 section 11.2.3.1b */
    RIL_SliceInfo * optionalSliceInfo;  /* Slice used for this data call. It is valid
                                           only when this data call is on
                                           AccessNetwork:NGRAN.*/
    int tdCount;                 /* Count of TrafficDescriptors*/
    RIL_TrafficDescriptor * TDs; /* TrafficDescriptors for which this data call
                                    must be used. It only includes the TDs for
                                    which a data call has been requested so far;
                                    it is not an exhaustive list.*/
} PdnInfo;

typedef struct {
    int activeCid;
    int reason;
} MePdnActiveInfo;

typedef enum {
    DATA_STATE_INACTIVE = 0,
    DATA_STATE_LINKDOWN = 1,
    DATA_STATE_ACTIVE = 2
} PDN_STATUS;

typedef enum {
    NO_CAUSE = -1,
    IPV4_ONLY = 0,
    IPV6_ONLY = 1,
    SINGLE_BEARER_ONLY_ALLOWED = 2
} PDN_REASON;

typedef enum {
    LOCAL_REJECT_CAUSE = -2,
    CHANNEL_CLOSED = -1,
    PDN_FAILED = 0,
    PDN_SUCCESS = 1
} PDN_SETUP_STATUS;

typedef enum {
    NETAGENT_ADDR_TYPE_UNKNOWN    = 0,
    NETAGENT_ADDR_TYPE_IPV4       = 1,
    NETAGENT_ADDR_TYPE_IPV6       = 2,
    NETAGENT_ADDR_TYPE_IPV4V6     = 3,
    NETAGENT_ADDR_TYPE_ANY        = 4
} NETAGENT_ADDR_TYPE;

typedef enum {
    NETAGENT_IFST_NONE        = 0,
    NETAGENT_IFST_UP          = 16
} NETAGENT_IFST_STATUS;

typedef enum {
    IMS_RELEASE_PDN = 0,
    IMS_REQUEST_PDN = 1
} IMS_NOTIFY_TYPE;

typedef enum {
    ADDRESS_NULL = 0,
    ADDRESS_IPV4 = 1,
    ADDRESS_IPV6_UNIQUE_LOCAL = 2,
    ADDRESS_IPV6_SITE_LOCAL = 3,
    ADDRESS_IPV6_LINK_LOCAL = 4,
    ADDRESS_IPV6_GLOBAL = 5
} ADDRESS_TYPE;

typedef enum {
    DC_EVENT_UNKNOWN = 0x0,
    DC_EVENT_DATA_RESET_NOTIFY = 0x81, //129
    DC_EVENT_IP_NORMAL_CHANGE = 0x100,
    DC_EVENT_NO_RA = 0x101,
    DC_EVENT_HANDOVER_NORMAL_CHANGE = 0x200
} DC_CHANGE_EVENT;

typedef enum {
    PS_ACT_GSM = 0,
    PS_ACT_GSM_COMPACT,
    PS_ACT_UTRAN,
    PS_ACT_GSM_W_EGPRS,
    PS_ACT_UTRAN_W_HSDPA,
    PS_ACT_UTRAN_W_HSUPA,
    PS_ACT_UTRAN_W_HSDPA_AND_HSUPA,
    PS_ACT_E_UTRAN,
} PS_ACCESS_TECHNOLOGY;

typedef enum {
    DRB_STATE = 0,
    DRB_ACT,
    DRB_NUM,
} DRB_PARAMETER;

typedef enum {
    PDN_MD_INITIATED = 1,
    PDN_AP_INITIATED,
} PDN_INITIATOR;

/**
 * AOSP reasons are defined by REQUEST_REASON_XXX in DataService.java
 * and MTK reasons are defined by ACTIVATE_REASON_XXX in MtkRILConstants.java.
 * Please make sure the values defined here are the same as those in the upper layers.
 */
typedef enum {
    ACT_REASON_UNKNOWN = 0,
    ACT_REASON_AOSP_NORMAL = 1,
    ACT_REASON_AOSP_HANDOVER = 3,
} PDN_ACT_REASON;

/**
 * AOSP reasons are defined by REQUEST_REASON_XXX in DataService.java
 * and MTK reasons are defined by DEACTIVATE_REASON_XXX in MtkRILConstants.java.
 * Please make sure the values defined here are the same as those in the upper layers.
 */
typedef enum {
    DEACT_REASON_UNKNOWN = 0,
    DEACT_REASON_AOSP_NORMAL = 1,
    DEACT_REASON_AOSP_SHUTDOWN = 2,
    DEACT_REASON_AOSP_HANDOVER = 3,
    DEACT_REASON_NORMAL = 2001,
    DEACT_REASON_RA_INITIAL_FAIL = 2002,
    DEACT_REASON_NO_PCSCF = 2003,
    DEACT_REASON_RA_REFRESH_FAIL = 2004,
    DEACT_REASON_APN_CHANGED = 2005,
    DEACT_REASON_TEMP_DATA_SWITCHING = 2006,
} PDN_DEACT_REASON;

/**
 * These are EGACT deactivate reasons used by modem, add/remove/modify should be reviewed by
 * both AP and MD.
 */
typedef enum {
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_DONT_CARE = 0, //EGACT_CAUSE_DONT_CARE
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_DEACT_NORMAL = 1, //EGACT_CAUSE_DEACT_NORMAL
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_DEACT_RA_INITIAL_ERROR = 2, //EGACT_CAUSE_DEACT_RA_INITIAL_ERROR
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_DEACT_NO_PCSCF = 3, //EGACT_CAUSE_DEACT_NO_PCSCF
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_DEACT_RA_REFRESH_ERROR = 4, //EGACT_CAUSE_DEACT_RA_REFRESH_ERROR
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_DEACT_RA_ERROR = 5, //EGACT_CAUSE_DEACT_RA_ERROR
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_USER_FORCE_TO_LOCAL_RELEASE = 6, //Used in user wants to deact a PDN immediately (without peer to peer air message waiting)
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_AOSP_SHUTDOWN = 7,
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_AOSP_HANDOVER = 8,
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_APN_CHANGED = 9,
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_IMS_HANDOVER = 10,
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_TEMP_DATA_SWITCHING = 11,
    ATCMD_DEACT_CAUSE_EGACT_CAUSE_AOSP_KEEP_INTERNET = 12,
} ATCMD_DEACT_CAUSE_ENUM;

typedef enum {
    SSC_UNKNOWN = -1,
    SSC_MODE1 = 0,
    SSC_MODE2 = 1,
    SSC_MODE3 = 2,
} SSC_MODE;

typedef enum {
    DATA_CONNECTION_DETACH = 0,
    DATA_CONNECTION_ATTACH = 1,
} DATA_CONNECTION_REGISTRATION;

typedef enum {
    ACTIVE_STATUS_INACTIVE = 0,
    ACTIVE_STATUS_DORMANT = 1,
    ACTIVE_STATUS_ACTIVE = 2,
} ACTIVE_STATUS;

typedef enum {
    DRDSDA_STATUS_INACTIVE = 0,
    DRDSDA_STATUS_ACTIVE = 1,
    DRDSDA_STATUS_NOT_SUPPORT = 2,
} DRDSDA_STATUS;

typedef enum {
    DATA_CALL_RESPONSE_TYPE_USE = 0, // For fallback
    DATA_CALL_RESPONSE_TYPE_UNUSE = 1, // For fallback
    DATA_CALL_RESPONSE_TYPE_DEACT = 2, // For data deact
} DATA_CALL_RESPONSE_TYPE;

/**
 * Used for how to update data call active status and profile when send data call response.
 * DATA_CALL_PROFILE_UPDATE_TYPE_NONE: no need update data active status and profile.
 * DATA_CALL_PROFILE_UPDATE_TYPE_ALL: update all data active status and profile.
 * DATA_CALL_PROFILE_UPDATE_TYPE_WITH_AID:
 * only update data active status and profile for the specified AID.
 */
typedef enum {
    ACTIVE_STATUS_AND_PROFILE_UPDATE_TYPE_NONE = -1,
    ACTIVE_STATUS_AND_PROFILE_UPDATE_TYPE_ALL = 0,
    ACTIVE_STATUS_AND_PROFILE_UPDATE_TYPE_WITH_AID = 1,
} ACTIVE_STATUS_AND_PROFILE_UPDATE_TYPE;

#endif /* __RMM_DATA_DEFS_H__ */
