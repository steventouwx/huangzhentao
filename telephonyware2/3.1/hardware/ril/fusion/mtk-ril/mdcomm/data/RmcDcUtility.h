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

#ifndef __RMC_DC_UTILITY_H__
#define __RMC_DC_UTILITY_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcDataDefs.h"
#include "RfxLog.h"
#include <ctype.h>
#include <string>
#include <stdlib.h>
#include "rfx_properties.h"
#include <telephony/mtk_ril.h>

#define APN_TYPE_INVALID -1
#define MIN_PLMN_LENTH           5
#define MAX_NSSAIS               5
#define MAX_NSSAI_TYPE           5
#define MAX_SST_CHAR             2
#define MAX_SD_CHAR              6
#define MAX_CAUSE_CHAR           1
#define DNN_TAG                  "dnn="
#define TD_APPID_TAG             "app_id="
#define TD_OSID_TAG              "os_id="
#define RSD_SSC_MOD_TAG          "ssc_mode="
#define RSD_SNSSAI_TAG           "s-nssai="
#define RSD_SESSION_TYPE_TAG     "pdu_session_type="
#define OS_ID_UUID_LEN           16
#define APP_ID_LEN_INDEX         16
#define APP_ID_INDEX             17

#define VERIZON_ON_SITE_LTE_ICCID_PREFIX "89999480"

/*****************************************************************************
 * Class RmcDcUtility
 *****************************************************************************/
class RmcDcUtility {
    public:
        static int getAddressType(char* addr);
        static const char* getProfileType(const int profileType);
        static const char* getMtkProfileType(const int profileType);
        static int getApnTypeId(const int profileType);
        static int getMtkApnTypeId(const int profileType);
        static const char* getApnType(int apnTypeId);
        static int getProfileID(int apnTypeId);
        static int getProtocolType(const char* protocol);
        static const char* getProtocolName(int protocol);
        static int getProtocolClassBitmap(int protocol);
        static int getAuthType(int authType);
        static int stringToBinaryBase(char *str, int base, int *err);
        static int isImsSupport();
        static const char* addrTypeToString(ADDRESS_TYPE addrType);
        static const char* pdnTypeToString(int pdnType);
        static void getPropertyBySlot(int slotId, const char *pPropertyName, char *pPropertyValue);
        static const char* checkRcsSupportPcscf();
        static bool isUserBuild();
        static bool isSupportOverrideModemDefaultApn();
        static bool isSupportDefaultImsApnSettings();
        static bool isXfrmSupport();
        static bool isMdrsraSupport(int mdrsraVersion, int mdrsraV3Supported);
        static int charToInt(char aChar);
        static long hexStrToDec(const char* hexStr);
        static int radioTechToAccessNw(RIL_RadioTechnology radioTech);
        static void convertHexStrWithoutBackslashToIntArray(uint8_t *appId, char *hexStr);
        static void convertHexStrWithBackslashToIntArray(uint8_t *appId, char *hexStr);
        static void convertHexStringToIntArray(uint8_t *appId, char *hexStr);
        static void convertStringToIntArray(uint8_t *appId, char *hexStr);
        static bool parseTdFromString(RIL_TrafficDescriptor* td, char* tdStr);
        static void parseRsdFromString(RIL_RouteSelectionDescriptor* rsd, char* rsdStr);
        static void parseNssaiFromString(RIL_SliceInfo *nssai, int *count, RIL_SliceStatus status,
                char* nssaiStr);
        static RIL_SliceServiceType convertIntToSliceServiceType(int sst);
        static int isTestSim(int slodId);
        static RIL_DataCallFailCause convertFailCauseToRilStandard(int cause, int slodId);
        static bool isVerizonOnSiteLteSimCard(int slotId);
};
#endif /* __RMC_DC_UTILITY_H__ */
