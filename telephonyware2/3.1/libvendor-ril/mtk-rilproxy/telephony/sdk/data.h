// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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


#ifndef MTK_RILPROXY_TELEPHONY_SDK_DATA_H_
#define MTK_RILPROXY_TELEPHONY_SDK_DATA_H_

#define TELEPHONY_SERVICE "mtk.telephony" /*well-known bus name */
#define TELEPHONY_DATA_INTERFACE "mtk.telephony.Data" /*interface name*/
#define TELEPHONY_DATA_PATH "/mtk/telephony/data" /*object name*/


#define TEXT_APN_TYPE_ALL "*"
#define TEXT_APN_TYPE_DEFAULT "default"
#define TEXT_APN_TYPE_IMS "ims"
#define TEXT_APN_TYPE_MMS "mms"
#define TEXT_APN_TYPE_SUPL "supl"
#define TEXT_APN_TYPE_DUN "dun"
#define TEXT_APN_TYPE_HIPRI "hipri"
#define TEXT_APN_TYPE_FOTA "fota"
#define TEXT_APN_TYPE_CBS "cbs"
#define TEXT_APN_TYPE_EMERGENCY "emergency"
#define TEXT_APN_TYPE_IA "ia"
#define TEXT_APN_TYPE_DM "dm"
#define TEXT_APN_TYPE_WAP "wap"
#define TEXT_APN_TYPE_NET "net"
#define TEXT_APN_TYPE_CMMAIL "cmmail"
#define TEXT_APN_TYPE_TETHERING "tethering"
#define TEXT_APN_TYPE_RCSE "rcse"
#define TEXT_APN_TYPE_XCAP "xcap"
#define TEXT_APN_TYPE_RCS "rcs"
#define TEXT_APN_TYPE_BIP "bip"

//for IOT card type
#define TEXT_IOT_APN_TYPE_DEFAULT "iot_default"
#define TEXT_IOT_APN_TYPE_NET_0 "iot_net_0"
#define TEXT_IOT_APN_TYPE_NET_1 "iot_net_1"
#define TEXT_IOT_APN_TYPE_NET_2 "iot_net_2"
#define TEXT_IOT_APN_TYPE_NET_3 "iot_net_3"
#define TEXT_IOT_APN_TYPE_NET_4 "iot_net_4"
#define TEXT_IOT_APN_TYPE_NET_5 "iot_net_5"
#define TEXT_IOT_APN_TYPE_NET_6 "iot_net_6"


typedef enum {
    IDLE = 0,
    CONNECTING = 1,
    CONNECTED = 2,
    DISCONNECTING = 3,
    DISCONNECTED = 4,
    RETRYING = 5,
    FAILED = 6,
} SDK_PdnState;

typedef struct {
    int             netId;      /* net id */
    int             active;     /* A SDK_PdnState */
    int             status;     /* A RIL_DataCallFailCause in file ril.h, 0 which is PDP_FAIL_NONE if no error */
    int             cid;        /* Context ID, uniquely identifies this call */
    char *          apnType;    /* APN_TYPE_* */
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
    char *          pcscf;      /* the Proxy Call State Control Function address
                                 via PCO(Protocol Configuration Option) for IMS client. */
    int             mtu;        /* MTU received from network
                                   Value <= 0 means network has either not sent a value or
                                   sent an invalid value */
} SDK_DataCallResponse;

#endif /* MTK_RILPROXY_TELEPHONY_SDK_DATA_H_ */
