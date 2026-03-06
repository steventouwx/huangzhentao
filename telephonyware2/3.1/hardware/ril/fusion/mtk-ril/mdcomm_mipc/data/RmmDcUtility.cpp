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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <algorithm>
#include "RmmDcUtility.h"
#include "RfxMclStatusManager.h"
#include "mipc_api.h"
#include "mipc_msg_tlv_const.h"

#define RFX_LOG_TAG "RmmDcUtility"

/*****************************************************************************
 * Class RmmDcUtility
 *****************************************************************************/

static const char PROPERTY_RIL_TEST_SIM[4][35] = {
    "vendor.gsm.sim.ril.testsim",
    "vendor.gsm.sim.ril.testsim.2",
    "vendor.gsm.sim.ril.testsim.3",
    "vendor.gsm.sim.ril.testsim.4",
};

void RmmDcUtility::getPropertyBySlot(int slotId, const char *propertyName, char *propertyValue) {
    char prop[MTK_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get(propertyName, prop, "");

    std::string propContent = std::string(prop);
    propContent.erase(std::remove_if(begin(propContent), end(propContent), ::isspace),
            end(propContent));

    int sepIdx = propContent.find(",");
    std::string prop1 = std::string(propContent);
    std::string prop2 = "";
    if (sepIdx > 0) {
        prop1 = propContent.substr(0, sepIdx);
        prop2 = propContent.substr(sepIdx+1, std::string::npos);
    }

    if (slotId == 0) {
        strncpy(propertyValue, prop1.c_str(), prop1.size());
    } else if (slotId == 1) {
        strncpy(propertyValue, prop2.c_str(), prop2.size());
    }
}

int RmmDcUtility::getAddressType(char* addr) {
    int type = IPV4;
    int length = strlen(addr);
    if (length >= MAX_IPV6_ADDRESS_LENGTH) {
        type = IPV4V6;
    } else if (length >= MAX_IPV4_ADDRESS_LENGTH) {
        type = IPV6;
    }
    return type;
}

const char *
RmmDcUtility::getProfileType(const int profileType) {
    switch (profileType) {
        case RIL_DATA_PROFILE_DEFAULT: return "default";
        case RIL_DATA_PROFILE_TETHERED: return "dun";
        case RIL_DATA_PROFILE_IMS: return "ims";
        case RIL_DATA_PROFILE_FOTA: return "fota";
        case RIL_DATA_PROFILE_CBS: return "cbs";
        default: return RmmDcUtility::getMtkProfileType(profileType);
    }
}

const char *
RmmDcUtility::getMtkProfileType(const int profileType) {
    switch (profileType) {
        case RIL_DATA_PROFILE_VENDOR_MMS: return "mms";
        case RIL_DATA_PROFILE_VENDOR_SUPL: return "supl";
        case RIL_DATA_PROFILE_VENDOR_HIPRI: return "hipri";
        case RIL_DATA_PROFILE_VENDOR_EMERGENCY: return "emergency";
        case RIL_DATA_PROFILE_VENDOR_XCAP: return "xcap";
        case RIL_DATA_PROFILE_VENDOR_RCS: return "rcs";
        case RIL_DATA_PROFILE_VENDOR_BIP: return "bip";
        case RIL_DATA_PROFILE_VENDOR_VSIM: return "vsim";
        case RIL_DATA_PROFILE_VENDOR_MCX: return "mcx";
        default: return "unknown";
    }
}

int RmmDcUtility::getApnTypeId(const int profileType) {
    switch (profileType) {
        case RIL_DATA_PROFILE_DEFAULT: return RIL_APN_TYPE_DEFAULT;
        case RIL_DATA_PROFILE_TETHERED: return RIL_APN_TYPE_DUN;
        case RIL_DATA_PROFILE_IMS: return RIL_APN_TYPE_IMS;
        case RIL_DATA_PROFILE_FOTA: return RIL_APN_TYPE_FOTA;
        case RIL_DATA_PROFILE_CBS: return RIL_APN_TYPE_CBS;
        default: return RmmDcUtility::getMtkApnTypeId(profileType);
    }
}

int RmmDcUtility::getMtkApnTypeId(const int profileType) {
    switch (profileType) {
        case RIL_DATA_PROFILE_VENDOR_MMS: return RIL_APN_TYPE_MMS;
        case RIL_DATA_PROFILE_VENDOR_SUPL: return RIL_APN_TYPE_SUPL;
        case RIL_DATA_PROFILE_VENDOR_HIPRI: return RIL_APN_TYPE_HIPRI;
        case RIL_DATA_PROFILE_VENDOR_EMERGENCY: return RIL_APN_TYPE_EMERGENCY;
        case RIL_DATA_PROFILE_VENDOR_XCAP: return RIL_APN_TYPE_XCAP;
        case RIL_DATA_PROFILE_VENDOR_RCS: return RIL_APN_TYPE_RCS;
        case RIL_DATA_PROFILE_VENDOR_BIP: return RIL_APN_TYPE_BIP;
        case RIL_DATA_PROFILE_VENDOR_VSIM: return RIL_APN_TYPE_VSIM;
        case RIL_DATA_PROFILE_VENDOR_MCX: return RIL_APN_TYPE_MCX;
        case RIL_DATA_PROFILE_VENDOR_ENTERPRISE: return RIL_APN_TYPE_ENTERPRISE;
        default: return APN_TYPE_INVALID;
    }
}

int RmmDcUtility::getMipcApnTypeId(const int profileType) {
    switch (profileType) {
        case RIL_DATA_PROFILE_DEFAULT:          return MIPC_APN_TYPE_DEFAULT;
        case RIL_DATA_PROFILE_TETHERED:         return MIPC_APN_TYPE_DUN;
        case RIL_DATA_PROFILE_IMS:              return MIPC_APN_TYPE_IMS;
        case RIL_DATA_PROFILE_FOTA:             return MIPC_APN_TYPE_FOTA;
        case RIL_DATA_PROFILE_CBS:              return MIPC_APN_TYPE_CBS;
        case RIL_DATA_PROFILE_VENDOR_MMS:       return MIPC_APN_TYPE_MMS;
        case RIL_DATA_PROFILE_VENDOR_SUPL:      return MIPC_APN_TYPE_SUPL;
        case RIL_DATA_PROFILE_VENDOR_HIPRI:     return MIPC_APN_TYPE_HIPRI;
        case RIL_DATA_PROFILE_VENDOR_EMERGENCY: return MIPC_APN_TYPE_EMERGENCY;
        case RIL_DATA_PROFILE_VENDOR_XCAP:      return MIPC_APN_TYPE_XCAP;
        case RIL_DATA_PROFILE_VENDOR_RCS:       return MIPC_APN_TYPE_RCS;
        case RIL_DATA_PROFILE_VENDOR_BIP:       return MIPC_APN_TYPE_BIP;
        case RIL_DATA_PROFILE_VENDOR_VSIM:      return MIPC_APN_TYPE_VSIM;
        // TODO: For MCS, use MIPC defined value after MIPC adding defination
        case RIL_DATA_PROFILE_VENDOR_MCX:       return RIL_APN_TYPE_MCX;
        default: return MIPC_APN_TYPE_UNKNOWN;
    }
}

const char *
RmmDcUtility::getApnType(int apnTypeId) {
    switch (apnTypeId) {
        case RIL_APN_TYPE_DEFAULT: return "default";
        case RIL_APN_TYPE_MMS: return "mms";
        case RIL_APN_TYPE_SUPL: return "supl";
        case RIL_APN_TYPE_DUN: return "dun";
        case RIL_APN_TYPE_HIPRI: return "hipri";
        case RIL_APN_TYPE_FOTA: return "fota";
        case RIL_APN_TYPE_IMS: return "ims";
        case RIL_APN_TYPE_CBS: return "cbs";
        case RIL_APN_TYPE_IA: return "ia";
        case RIL_APN_TYPE_EMERGENCY: return "emergency";
        case RIL_APN_TYPE_MCX: return "mcx";
        case RIL_APN_TYPE_XCAP: return "xcap";
        case RIL_APN_TYPE_RCS: return "rcs";
        case RIL_APN_TYPE_BIP: return "bip";
        case RIL_APN_TYPE_VSIM: return "vsim";
        case RIL_APN_TYPE_ALL: return "default,hipri,mms,supl,dun,fota,cbs";
        case RIL_APN_TYPE_MTKALL: return "default,hipri,mms,supl,dun,fota,cbs"
                                         ",xcap,rcs,bip,vsim";
        default: return "unknown";
    }
}

int RmmDcUtility::getProfileID(int apnTypeId) {
    switch (apnTypeId) {
        case RIL_APN_TYPE_DEFAULT: return RIL_DATA_PROFILE_DEFAULT;
        case RIL_APN_TYPE_DUN: return RIL_DATA_PROFILE_TETHERED;
        case RIL_APN_TYPE_FOTA: return RIL_DATA_PROFILE_FOTA;
        case RIL_APN_TYPE_IMS: return RIL_DATA_PROFILE_IMS;
        case RIL_APN_TYPE_CBS: return RIL_DATA_PROFILE_CBS;
        case RIL_APN_TYPE_MMS: return RIL_DATA_PROFILE_VENDOR_MMS;
        case RIL_APN_TYPE_SUPL: return RIL_DATA_PROFILE_VENDOR_SUPL;
        case RIL_APN_TYPE_XCAP: return RIL_DATA_PROFILE_VENDOR_XCAP;
        case RIL_APN_TYPE_BIP: return RIL_DATA_PROFILE_VENDOR_BIP;
        case RIL_APN_TYPE_HIPRI: return RIL_DATA_PROFILE_VENDOR_HIPRI;
        case RIL_APN_TYPE_EMERGENCY: return RIL_DATA_PROFILE_VENDOR_EMERGENCY;
        case RIL_APN_TYPE_RCS: return RIL_DATA_PROFILE_VENDOR_RCS;
        case RIL_APN_TYPE_VSIM: return RIL_DATA_PROFILE_VENDOR_VSIM;
        case RIL_APN_TYPE_MCX: return RIL_DATA_PROFILE_VENDOR_MCX;
        default: return RIL_DATA_PROFILE_DEFAULT;
    }
}

int RmmDcUtility::getProtocolType(const char* protocol) {
    int type = IPV4;

    if (protocol == NULL) {
        return type;
    }

    if (!strcmp(protocol, SETUP_DATA_PROTOCOL_IP)) {
        type = IPV4;
    } else if (!strcmp(protocol, SETUP_DATA_PROTOCOL_IPV6)) {
        type = IPV6;
    } else if (!strcmp(protocol, SETUP_DATA_PROTOCOL_IPV4V6)) {
        type = IPV4V6;
    }

    return type;
}

const char* RmmDcUtility::getProtocolName(int protocol) {
    const char* name = NULL;
    switch (protocol) {
        case IPV4:
            name = SETUP_DATA_PROTOCOL_IP;
            break;
        case IPV6:
            name = SETUP_DATA_PROTOCOL_IPV6;
            break;
        case IPV4V6:
            name = SETUP_DATA_PROTOCOL_IPV4V6;
            break;
        default:
            name = SETUP_DATA_PROTOCOL_IP;
            break;
    }

    RFX_LOG_D(RFX_LOG_TAG, "The protocol name is %s", name);
    return name;
}

int RmmDcUtility::getProtocolClassBitmap(int protocol) {
    switch (protocol) {
        case IPV4:
            return NETAGENT_ADDR_TYPE_IPV4;
        case IPV6:
            return NETAGENT_ADDR_TYPE_IPV6;
        case IPV4V6:
            return NETAGENT_ADDR_TYPE_ANY;
        default:
            return NETAGENT_ADDR_TYPE_UNKNOWN;
    }
}

int RmmDcUtility::getMipcProtocolType(const char* protocol) {
    int type = MIPC_APN_PDP_TYPE_IPV4;

    if (protocol == NULL) {
        return type;
    }

    if (!strcmp(protocol, SETUP_DATA_PROTOCOL_IP)) {
        type = MIPC_APN_PDP_TYPE_IPV4;
    } else if (!strcmp(protocol, SETUP_DATA_PROTOCOL_IPV6)) {
        type = MIPC_APN_PDP_TYPE_IPV6;
    } else if (!strcmp(protocol, SETUP_DATA_PROTOCOL_IPV4V6)) {
        type = MIPC_APN_PDP_TYPE_IPV4V6;
    }

    RFX_LOG_D(RFX_LOG_TAG, "The protocol type is %d", type);
    return type;
}

int RmmDcUtility::getAuthType(int authType) {
    // TODO: Move the logic of transfer AUTHTYPE_PAP_CHAP to AUTHTYPE_CHAP to DDM.
    // Sync AuthType value(AT+CGAUTH uses) to DDM. Treat AUTHTYPE_PAP_CHAP as
    // AUTHTYPE_CHAP as modem's suggestion, other values just bypass to modem.
    if (authType == AUTHTYPE_PAP_CHAP) {
        return AUTHTYPE_CHAP;
    }

    return authType;
}

int RmmDcUtility::stringToBinaryBase(char *str, int base, int *err) {
    int out;
    unsigned long ul;
    *err = 0;

    if (str == NULL) {
        *err = -1;
        return 0;
    }

    ul = strtoul(str, NULL, base);
    if (ul == ULONG_MAX) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] error on strtoul", __FUNCTION__);
        *err = -2;
        return 0;
    }
    out = (int)ul;

    return out;
}

int RmmDcUtility::isImsSupport() {
    int isImsSupport = 0;
    char prop_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.ims_support", prop_value, "0");
    isImsSupport = atoi(prop_value);
    RFX_LOG_D(RFX_LOG_TAG, "isImsSupport = %d", isImsSupport);
    return isImsSupport;
}

int RmmDcUtility::isTestSim(int slotId) {
    int ret = 0;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get(PROPERTY_RIL_TEST_SIM[slotId], prop, "0");
    ret = atoi(prop);
    return ret;
}

const char *RmmDcUtility::addrTypeToString(ADDRESS_TYPE addrType) {
    switch (addrType) {
        case ADDRESS_NULL: return "NULL";
        case ADDRESS_IPV4: return "IPV4";
        case ADDRESS_IPV6_UNIQUE_LOCAL: return "IPV6 UNIQUE LOCAL";
        case ADDRESS_IPV6_SITE_LOCAL: return "IPV6 SITE LOCAL";
        case ADDRESS_IPV6_LINK_LOCAL: return "IPV6 LINK LOCAL";
        case ADDRESS_IPV6_GLOBAL: return "IPV6 GLOBAL";
        default: return "UNKNOWN";
    }
}

const char *RmmDcUtility::pdnTypeToString(int pdnType) {
    switch (pdnType) {
        case IPV4: return "IPV4";
        case IPV6: return "IPV6";
        case IPV4V6: return "IPV4V6";
        default: return "INVALID PDN TYPE";
    }
}

bool RmmDcUtility::isRcsSupportPcscf() {
    int isRcsSupport = 0;
    int op08 = 0;
    char prop_value[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.mtk_rcs_ua_support", prop_value, "0");
    isRcsSupport = (!strcmp(prop_value, "1")) ? 1 : 0;
    rfx_property_get("persist.vendor.operator.optr", prop_value, "0");
    op08 = (!strcmp(prop_value, OPERATOR_OP08)) ? 1 : 0;

    RFX_LOG_D(RFX_LOG_TAG, "isRcsSupportPcscf = %d:%d", isRcsSupport, op08);

    if (isRcsSupport && op08) {
        return true;
    }
    return false;
}

bool RmmDcUtility::isUserBuild() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("ro.build.type", property_value, "");
    if (strcmp(property_value, "user") == 0) {
        return true;
    }
    return false;
}

int RmmDcUtility::isFastDormancySupport() {
    int isFdSupport = 0;
    char propertyValue[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(PROPERTY_MTK_FD_SUPPORT, propertyValue, "0");
    isFdSupport = atoi(propertyValue);
    return isFdSupport ? 1 : 0;
}

int RmmDcUtility::getMipcApnType(int rilApnType) {
    int mipcApnType = 0;
    if (rilApnType & RIL_APN_TYPE_DEFAULT) {
        mipcApnType |= MIPC_APN_TYPE_DEFAULT;
    }
    if (rilApnType & RIL_APN_TYPE_MMS) {
        mipcApnType |= MIPC_APN_TYPE_MMS;
    }
    if (rilApnType & RIL_APN_TYPE_SUPL) {
        mipcApnType |= MIPC_APN_TYPE_SUPL;
    }
    if (rilApnType & RIL_APN_TYPE_DUN) {
        mipcApnType |= MIPC_APN_TYPE_DUN;
    }
    if (rilApnType & RIL_APN_TYPE_HIPRI) {
        mipcApnType |= MIPC_APN_TYPE_HIPRI;
    }
    if (rilApnType & RIL_APN_TYPE_FOTA) {
        mipcApnType |= MIPC_APN_TYPE_FOTA;
    }
    if (rilApnType & RIL_APN_TYPE_IMS) {
        mipcApnType |= MIPC_APN_TYPE_IMS;
    }
    if (rilApnType & RIL_APN_TYPE_CBS) {
        mipcApnType |= MIPC_APN_TYPE_CBS;
    }
    if (rilApnType & RIL_APN_TYPE_IA) {
        mipcApnType |= MIPC_APN_TYPE_IA;
    }
    if (rilApnType & RIL_APN_TYPE_EMERGENCY) {
        mipcApnType |= MIPC_APN_TYPE_EMERGENCY;
    }
    /* not support yet.
    if (rilApnType & RIL_APN_TYPE_MCX) {
        mipcApnType |= MIPC_APN_TYPE_MCX;
    }*/
    if (rilApnType & RIL_APN_TYPE_XCAP) {
        mipcApnType |= MIPC_APN_TYPE_XCAP;
    }
    if (rilApnType & RIL_APN_TYPE_RCS) {
        mipcApnType |= MIPC_APN_TYPE_RCS;
        if (RmmDcUtility::isRcsSupportPcscf()) {
            mipcApnType |= MIPC_APN_TYPE_RCS_PCSCF;
        }
    }
    if (rilApnType & RIL_APN_TYPE_BIP) {
        mipcApnType |= MIPC_APN_TYPE_BIP;
    }
    if (rilApnType & RIL_APN_TYPE_VSIM) {
        mipcApnType |= MIPC_APN_TYPE_VSIM;
    }
    // No enterprise defination in modem, change to default.
    if (rilApnType & RIL_APN_TYPE_ENTERPRISE) {
        mipcApnType |= MIPC_APN_TYPE_DEFAULT;
    }

    return mipcApnType;
}

int RmmDcUtility::radioTechToAccessNw(RIL_RadioTechnology radioTech) {
    /* AOSP_V1_5::RadioAccessNetworks to 3gpp 27.007 Sec 7.3
       RadioAccessNetworks 0: UNKNOWN
       RadioAccessNetworks 1: GERAN (0 GSM)
       RadioAccessNetworks 2: UTRAN (2 UTRAN)
       RadioAccessNetworks 3: EUTRAN(7 E-UTRAN)
       RadioAccessNetworks 4: NGRAN (12 NG-RAN)
    */
    switch(radioTech) {
        case RADIO_TECH_GSM:
        case RADIO_TECH_GPRS:
        case RADIO_TECH_EDGE:
        case RADIO_TECH_UMTS:
        case RADIO_TECH_IS95A:
        case RADIO_TECH_IS95B:
        case RADIO_TECH_1xRTT:
            return 1;
        case RADIO_TECH_EVDO_0:
        case RADIO_TECH_EVDO_A:
        case RADIO_TECH_HSDPA:
        case RADIO_TECH_HSUPA:
        case RADIO_TECH_HSPA:
        case RADIO_TECH_EVDO_B:
        case RADIO_TECH_EHRPD:
        case RADIO_TECH_HSPAP:
        case RADIO_TECH_TD_SCDMA:
            return 2;
        case RADIO_TECH_LTE:
        case RADIO_TECH_LTE_CA:
            return 3;
        case RADIO_TECH_NR:
            return 4;
        case RADIO_TECH_UNKNOWN:
        case RADIO_TECH_IWLAN:
        default:
            return 0;
    }
}

RIL_SliceServiceType RmmDcUtility::convertIntToSliceServiceType(int sst) {
    if (sst > 0 && sst <= 3) {
        return (RIL_SliceServiceType)sst;
    } else {
        return NONE_TYPE;
    }
}

int RmmDcUtility::charToInt(char aChar) {
    if(isdigit(aChar)) {
        return aChar - 48;
    }
    if( aChar < 'A' || (aChar > 'F' && aChar < 'a') || aChar > 'z' ) {
        return -1;
    }
    if(isalpha(aChar)) {
        return isupper(aChar) ? aChar - 55 : aChar - 87;
    }
    return -1;
}

long RmmDcUtility::hexStrToDec(const char* hexStr) {
    if (hexStr == NULL || strlen(hexStr) == 0) {
        return -1;
    }
    int len = strlen(hexStr);
    int num = 0;
    int i = 0;
    int temp = 0;
    int bits = 0;

    for (; i < len; i++, temp = 0) {
        temp = charToInt(*(hexStr + i));
        bits = (len - i - 1) * 4;
        temp = temp << bits;
        num = num | temp;
    }
    return num;
}

int RmmDcUtility::byteToHexStr(const char *byteArr, int arrLen, char *hexStr, int hexStrLen) {
    int index = 0;
    for (int i = 0; i < arrLen; i++) {
        char hex1;
        char hex2;
        int value = byteArr[i];
        int v1 = value/16;
        int v2 = value%16;
        if (v1 >= 0 && v1 <= 9) {
            hex1 = (char)(48 + v1);
        } else {
            hex1 = (char)(55 + v1);
        }
        if (v2 >= 0 && v2 <= 9) {
            hex2 = (char)(48 + v2);
        } else {
            hex2 = (char)(55 + v2);
        }
        if (hexStrLen <= i) {
            return -1;
        }
        hexStr[index++] = hex1;
        hexStr[index++] = hex2;
    }
    return 0;
}

vector<string> RmmDcUtility::splitString(string str, string delim) {
    string::size_type pos;
    vector<string> result;
    str += delim;
    string::size_type len = str.size();
    for (string::size_type i = 0; i < len; i++) {
        pos = str.find(delim, i);
        if (pos < len) {
            string temp = str.substr(i, pos - i);
            result.push_back(temp);
            i = pos + delim.size() - 1;
        }
    }
    return result;
}

vector<string> RmmDcUtility::nonCompressedColonSepIpV6ToVector(char* input) {
    vector<string> result;
    if (input == NULL || strlen(input) == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s]illegal parameter", __FUNCTION__);
        return result;
    }
    string ipV6Str(input);
    result =  splitString(ipV6Str, ":");
    return result;
}

vector<string> RmmDcUtility::compressedColonSepIpV6ToVector(char* input) {
    vector<string> result;
    if (input == NULL || strlen(input) == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s]illegal parameter", __FUNCTION__);
        return result;
    }
    char ipV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
    char *doubleColonPos = strstr(input, "::");
    if (doubleColonPos == NULL) {
        // non compressed colon separated IPV6 was input, should not happen.
        RFX_LOG_I(RFX_LOG_TAG,
            "[%s]non compressed colon separated IPV6 was input",__FUNCTION__);
        return result;
    }
    int colonCount = 0;
    char *savePtr = NULL;
    char tempInput[MAX_IPV6_ADDRESS_LENGTH] = {0};
    strncat(tempInput, input, MAX_IPV6_ADDRESS_LENGTH - 1);
    char *p = strtok_r(tempInput, ":", &savePtr);
    while(p) {
        colonCount++;
        p = strtok_r(NULL, ":", &savePtr);
    }
    // build IPV6
    strncat(ipV6, input, doubleColonPos - input + 1);
    strncat(ipV6 + strlen(ipV6), "0", MAX_IPV6_ADDRESS_LENGTH - strlen(ipV6));
    for (int i = 0; i < 7 - colonCount; i++) {
        strncat(ipV6 + strlen(ipV6), ":0", MAX_IPV6_ADDRESS_LENGTH - strlen(ipV6));
    }
    strncat(ipV6 + strlen(ipV6), doubleColonPos + 1,
            MAX_IPV6_ADDRESS_LENGTH - strlen(ipV6));
    string ipV6Str(ipV6);
    result = splitString(ipV6Str, ":");
    return result;
}

vector<string> RmmDcUtility::colonSepIpV6ToVector(char* input) {
    vector<string> result;
    if (input == NULL || strlen(input) == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s]illegal parameter", __FUNCTION__);
        return result;
    }
    if (strstr(input, "::") == NULL) {
        result = nonCompressedColonSepIpV6ToVector(input);
    } else {
        result = compressedColonSepIpV6ToVector(input);
    }
    return result;
}

vector<string> RmmDcUtility::dotSepIpV6ToVector(char* input) {
    vector<string> result;
    if (input == NULL || strlen(input) == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s]illegal parameter", __FUNCTION__);
        return result;
    }
    string ipV6Str(input);
    result = splitString(ipV6Str, ".");
    return result;
}

vector<string> RmmDcUtility::ipV6ToVector(char* input) {
    vector<string> result;
    if (input == NULL || strlen(input) == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s]illegal parameter", __FUNCTION__);
        return result;
    }
    if (strstr(input, ":") == NULL) {
        result = dotSepIpV6ToVector(input);
    } else {
        result = colonSepIpV6ToVector(input);
    }
    return result;
}

vector<string> RmmDcUtility::ipV4ToVector(char* input) {
    vector<string> result;
    if (input == NULL || strlen(input) == 0 | strstr(input, ".") == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s]illegal parameter", __FUNCTION__);
        return result;
    }
    string ipV4Str(input);
    result = splitString(ipV4Str, ".");
    return result;
}

int RmmDcUtility::convertFailCauseToRilStandard(int cause, int slodId) {
    // No need to convert causes that bigger than 0x00150000,
    // because these causes are for MIPC only.
    if (cause > MIPC_RESULT_PDN_EXT_CME_ERROR_BEGIN
            && cause < MIPC_RESULT_PDN_EXT_CME_ERROR_END) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] CME CAUSE: 0x%X", slodId, __FUNCTION__, cause);
        cause -= MIPC_RESULT_PDN_EXT_NETWORK_ERROR_BEGIN;
        if (cause == CME_PDP_AUTHENTICATION_FAILED) {
            return PDP_FAIL_USER_AUTHENTICATION;
        } else if (cause == CME_OPERATOR_DETERMINED_BARRING) {
            return PDP_FAIL_OPERATOR_BARRED;
        } else if (cause == CME_MAXIMUM_NMUBER_PDP_CONTEXTS_REACHED) {
            return PDP_FAIL_MAX_ACTIVE_PDP_CONTEXT_REACHED;
        } else if (cause == CME_REQUESTED_APN_NOT_SUPPORTED_IN_RAT_AND_PLMN) {
            return PDP_FAIL_UNSUPPORTED_APN_IN_CURRENT_PLMN;
        } else {
            cause -= CME_CAUSE_BASE_V14;
        }
    } else if (cause > MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_BEGIN
            && cause < MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_END) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] SM CAUSE: 0x%X",
                slodId, __FUNCTION__, cause);
        cause -= MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_BEGIN;
    } else if (cause > MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_BEGIN
            && cause < MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_END) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ESM CAUSE: 0x%X",
                slodId, __FUNCTION__, cause);
        cause -= MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_BEGIN;
    } else if (cause > MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_BEGIN
            && cause < MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_END) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] PAM CAUSE: 0x%X",
                slodId, __FUNCTION__, cause);
        cause -= MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_BEGIN;
    } else if (cause > MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_BEGIN
            && cause < MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_END) {
        cause -= MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_BEGIN;

    } else if (cause > MIPC_RESULT_PDN_EXT_NETWORK_ERROR_BEGIN
            && cause < MIPC_RESULT_PDN_EXT_NETWORK_ERROR_END) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NETWORK CAUSE: 0x%X",
                slodId, __FUNCTION__, cause);
        cause -= MIPC_RESULT_PDN_EXT_NETWORK_ERROR_BEGIN;
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] CAUSE: 0x%X", slodId, __FUNCTION__, cause);
    }

    switch (cause) {
        case 0: return PDP_FAIL_NONE;
        case 0x08: return PDP_FAIL_OPERATOR_BARRED;
        case 0x0E: return PDP_FAIL_NAS_SIGNALLING;
        case 0x18: return PDP_FAIL_MBMS_CAPABILITIES_INSUFFICIENT;
        case 0x19: return PDP_FAIL_LLC_SNDCP;
        case 0x1A: return PDP_FAIL_INSUFFICIENT_RESOURCES;
        case 0x1B: return PDP_FAIL_MISSING_UKNOWN_APN;
        case 0x1C: return PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE;
        case 0x1D: return PDP_FAIL_USER_AUTHENTICATION;
        case 0x1E: return PDP_FAIL_ACTIVATION_REJECT_GGSN;
        case 0x1F: return PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED;
        case 0x20: return PDP_FAIL_SERVICE_OPTION_NOT_SUPPORTED;
        case 0x21: return PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED;
        case 0x22: return PDP_FAIL_SERVICE_OPTION_OUT_OF_ORDER;
        case 0x23: return PDP_FAIL_NSAPI_IN_USE;
        case 0x24: return PDP_FAIL_REGULAR_DEACTIVATION;
        case 0x25: return PDP_FAIL_QOS_NOT_ACCEPTED;
        case 0x26: return PDP_FAIL_NETWORK_FAILURE;
        case 0x27: return PDP_FAIL_UMTS_REACTIVATION_REQ;
        case 0x28: return PDP_FAIL_FEATURE_NOT_SUPP;
        case 0x29: return PDP_FAIL_TFT_SEMANTIC_ERROR;
        case 0x2A: return PDP_FAIL_TFT_SYTAX_ERROR;
        case 0x2B: return PDP_FAIL_UNKNOWN_PDP_CONTEXT;
        case 0x2C: return PDP_FAIL_FILTER_SEMANTIC_ERROR;
        case 0x2D: return PDP_FAIL_FILTER_SYTAX_ERROR;
        case 0x2E: return PDP_FAIL_PDP_WITHOUT_ACTIVE_TFT;
        case 0x2F: return PDP_FAIL_MULTICAST_GROUP_MEMBERSHIP_TIMEOUT;
        case 0x30: return PDP_FAIL_BCM_VIOLATION;
        case 0x31: return PDP_FAIL_LAST_PDN_DISC_NOT_ALLOWED;
        case 0x32: return PDP_FAIL_ONLY_IPV4_ALLOWED;
        case 0x33: return PDP_FAIL_ONLY_IPV6_ALLOWED;
        case 0x34: return PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED;
        case 0x35: return PDP_FAIL_ESM_INFO_NOT_RECEIVED;
        case 0x36: return PDP_FAIL_PDN_CONN_DOES_NOT_EXIST;
        case 0x37: return PDP_FAIL_MULTI_CONN_TO_SAME_PDN_NOT_ALLOWED;
        case 0x38: return PDP_FAIL_COLLISION_WITH_NW_INITIATED_REQUEST;
        case 0x39: return PDP_FAIL_ONLY_IPV4V6_ALLOWED;
        case 0x3A: return PDP_FAIL_ONLY_NON_IP_ALLOWED;
        case 0x3B: return PDP_FAIL_ESM_UNSUPPORTED_QCI_VALUE;
        case 0x3C: return PDP_FAIL_BEARER_HANDLING_NOT_SUPPORT;
        case 0x41: return PDP_FAIL_MAX_ACTIVE_PDP_CONTEXT_REACHED;
        case 0x42: return PDP_FAIL_UNSUPPORTED_APN_IN_CURRENT_PLMN;
        case 0x51: return PDP_FAIL_INVALID_TRANSACTION_ID;
        case 0x5F: return PDP_FAIL_MESSAGE_INCORRECT_SEMANTIC;
        case 0x60: return PDP_FAIL_INVALID_MANDATORY_INFO;
        case 0x61: return PDP_FAIL_MESSAGE_TYPE_UNSUPPORTED;
        case 0x62: return PDP_FAIL_MSG_TYPE_NONCOMPATIBLE_STATE;
        case 0x63: return PDP_FAIL_UNKNOWN_INFO_ELEMENT;
        case 0x64: return PDP_FAIL_CONDITIONAL_IE_ERROR;
        case 0x65: return PDP_FAIL_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE;
        case 0x6F: return PDP_FAIL_PROTOCOL_ERRORS;
        case 0x70: return PDP_FAIL_APN_TYPE_CONFLICT;
        case 0x71: return PDP_FAIL_INVALID_PCSCF_ADDR;
        case 0x72: return PDP_FAIL_INTERNAL_CALL_PREEMPT_BY_HIGH_PRIO_APN;
        case 0x73: return PDP_FAIL_EMM_ACCESS_BARRED;
        case 0x74: return PDP_FAIL_EMERGENCY_IFACE_ONLY;
        case 0x75: return PDP_FAIL_IFACE_MISMATCH;
        case 0x76: return PDP_FAIL_COMPANION_IFACE_IN_USE;
        case 0x77: return PDP_FAIL_IP_ADDRESS_MISMATCH;
        case 0x78: return PDP_FAIL_IFACE_AND_POL_FAMILY_MISMATCH;
        case 0x79: return PDP_FAIL_EMM_ACCESS_BARRED_INFINITE_RETRY;
        case 0x7A: return PDP_FAIL_AUTH_FAILURE_ON_EMERGENCY_CALL;
        case 0x7B: return PDP_FAIL_INVALID_DNS_ADDR;
        case 0x7C: return PDP_FAIL_INVALID_PCSCF_OR_DNS_ADDRESS;
        case 0x7F: return PDP_FAIL_CALL_PREEMPT_BY_EMERGENCY_APN;

        case 0x80: return PDP_FAIL_UE_INITIATED_DETACH_OR_DISCONNECT;
        case 0x7D0: return PDP_FAIL_MIP_FA_REASON_UNSPECIFIED;
        case 0x7D1: return PDP_FAIL_MIP_FA_ADMIN_PROHIBITED;
        case 0x7D2: return PDP_FAIL_MIP_FA_INSUFFICIENT_RESOURCES;
        case 0x7D3: return PDP_FAIL_MIP_FA_MOBILE_NODE_AUTHENTICATION_FAILURE;
        case 0x7D4: return PDP_FAIL_MIP_FA_HOME_AGENT_AUTHENTICATION_FAILURE;
        case 0x7D5: return PDP_FAIL_MIP_FA_REQUESTED_LIFETIME_TOO_LONG;
        case 0x7D6: return PDP_FAIL_MIP_FA_MALFORMED_REQUEST;
        case 0x7D7: return PDP_FAIL_MIP_FA_MALFORMED_REPLY;
        case 0x7D8: return PDP_FAIL_MIP_FA_ENCAPSULATION_UNAVAILABLE;
        case 0x7D9: return PDP_FAIL_MIP_FA_VJ_HEADER_COMPRESSION_UNAVAILABLE;
        case 0x7DA: return PDP_FAIL_MIP_FA_REVERSE_TUNNEL_UNAVAILABLE;
        case 0x7DB: return PDP_FAIL_MIP_FA_REVERSE_TUNNEL_IS_MANDATORY;
        case 0x7DC: return PDP_FAIL_MIP_FA_DELIVERY_STYLE_NOT_SUPPORTED;
        case 0x7DD: return PDP_FAIL_MIP_FA_MISSING_NAI;
        case 0x7DE: return PDP_FAIL_MIP_FA_MISSING_HOME_AGENT;
        case 0x7DF: return PDP_FAIL_MIP_FA_MISSING_HOME_ADDRESS;
        case 0x7E0: return PDP_FAIL_MIP_FA_UNKNOWN_CHALLENGE;
        case 0x7E1: return PDP_FAIL_MIP_FA_MISSING_CHALLENGE;
        case 0x7E2: return PDP_FAIL_MIP_FA_STALE_CHALLENGE;
        case 0x7E3: return PDP_FAIL_MIP_HA_REASON_UNSPECIFIED;
        case 0x7E4: return PDP_FAIL_MIP_HA_ADMIN_PROHIBITED;
        case 0x7E5: return PDP_FAIL_MIP_HA_INSUFFICIENT_RESOURCES;
        case 0x7E6: return PDP_FAIL_MIP_HA_MOBILE_NODE_AUTHENTICATION_FAILURE;
        case 0x7E7: return PDP_FAIL_MIP_HA_FOREIGN_AGENT_AUTHENTICATION_FAILURE;
        case 0x7E8: return PDP_FAIL_MIP_HA_REGISTRATION_ID_MISMATCH;
        case 0x7E9: return PDP_FAIL_MIP_HA_MALFORMED_REQUEST;
        case 0x7EA: return PDP_FAIL_MIP_HA_UNKNOWN_HOME_AGENT_ADDRESS;
        case 0x7EB: return PDP_FAIL_MIP_HA_REVERSE_TUNNEL_UNAVAILABLE;
        case 0x7EC: return PDP_FAIL_MIP_HA_REVERSE_TUNNEL_IS_MANDATORY;
        case 0x7ED: return PDP_FAIL_MIP_HA_ENCAPSULATION_UNAVAILABLE;
        case 0x7EE: return PDP_FAIL_CLOSE_IN_PROGRESS;
        case 0x7EF: return PDP_FAIL_NETWORK_INITIATED_TERMINATION;
        case 0x7F0: return PDP_FAIL_MODEM_APP_PREEMPTED;
        case 0x7F1: return PDP_FAIL_PDN_IPV4_CALL_DISALLOWED;
        case 0x7F2: return PDP_FAIL_PDN_IPV4_CALL_THROTTLED;
        case 0x7F3: return PDP_FAIL_PDN_IPV6_CALL_DISALLOWED;
        case 0x7F4: return PDP_FAIL_PDN_IPV6_CALL_THROTTLED;
        case 0x7F5: return PDP_FAIL_MODEM_RESTART;
        case 0x7F6: return PDP_FAIL_PDP_PPP_NOT_SUPPORTED;
        case 0x7F7: return PDP_FAIL_UNPREFERRED_RAT;
        case 0x7F8: return PDP_FAIL_PHYSICAL_LINK_CLOSE_IN_PROGRESS;
        case 0x7F9: return PDP_FAIL_APN_PENDING_HANDOVER;
        case 0x7FA: return PDP_FAIL_PROFILE_BEARER_INCOMPATIBLE;
        case 0x7FB: return PDP_FAIL_SIM_CARD_CHANGED;
        case 0x7FC: return PDP_FAIL_LOW_POWER_MODE_OR_POWERING_DOWN;
        case 0x7FD: return PDP_FAIL_APN_DISABLED;
        case 0x7FE: return PDP_FAIL_MAX_PPP_INACTIVITY_TIMER_EXPIRED;
        case 0x7FF: return PDP_FAIL_IPV6_ADDRESS_TRANSFER_FAILED;
        case 0x800: return PDP_FAIL_TRAT_SWAP_FAILED;
        case 0x801: return PDP_FAIL_EHRPD_TO_HRPD_FALLBACK;
        case 0x802: return PDP_FAIL_MIP_CONFIG_FAILURE;
        case 0x803: return PDP_FAIL_PDN_INACTIVITY_TIMER_EXPIRED;
        case 0x804: return PDP_FAIL_MAX_IPV4_CONNECTIONS;
        case 0x805: return PDP_FAIL_MAX_IPV6_CONNECTIONS;
        case 0x806: return PDP_FAIL_APN_MISMATCH;
        case 0x807: return PDP_FAIL_IP_VERSION_MISMATCH;
        case 0x808: return PDP_FAIL_DUN_CALL_DISALLOWED;
        case 0x809: return PDP_FAIL_INTERNAL_EPC_NONEPC_TRANSITION;
        case 0x80A: return PDP_FAIL_INTERFACE_IN_USE;
        case 0x80B: return PDP_FAIL_APN_DISALLOWED_ON_ROAMING;
        case 0x80C: return PDP_FAIL_APN_PARAMETERS_CHANGED;
        case 0x80D: return PDP_FAIL_NULL_APN_DISALLOWED;
        case 0x80E: return PDP_FAIL_THERMAL_MITIGATION;
        case 0x80F: return PDP_FAIL_DATA_SETTINGS_DISABLED;
        case 0x810: return PDP_FAIL_DATA_ROAMING_SETTINGS_DISABLED;
        case 0x811: return PDP_FAIL_DDS_SWITCHED;
        case 0x812: return PDP_FAIL_FORBIDDEN_APN_NAME;
        case 0x813: return PDP_FAIL_DDS_SWITCH_IN_PROGRESS;
        case 0x814: return PDP_FAIL_CALL_DISALLOWED_IN_ROAMING;
        case 0x815: return PDP_FAIL_NON_IP_NOT_SUPPORTED;
        case 0x816: return PDP_FAIL_PDN_NON_IP_CALL_THROTTLED;
        case 0x817: return PDP_FAIL_PDN_NON_IP_CALL_DISALLOWED;
        case 0x818: return PDP_FAIL_CDMA_LOCK;
        case 0x819: return PDP_FAIL_CDMA_INTERCEPT;
        case 0x81A: return PDP_FAIL_CDMA_REORDER;
        case 0x81B: return PDP_FAIL_CDMA_RELEASE_DUE_TO_SO_REJECTION;
        case 0x81C: return PDP_FAIL_CDMA_INCOMING_CALL;
        case 0x81D: return PDP_FAIL_CDMA_ALERT_STOP;
        case 0x81E: return PDP_FAIL_CHANNEL_ACQUISITION_FAILURE;
        case 0x81F: return PDP_FAIL_MAX_ACCESS_PROBE;
        case 0x820: return PDP_FAIL_CONCURRENT_SERVICE_NOT_SUPPORTED_BY_BASE_STATION;
        case 0x821: return PDP_FAIL_NO_RESPONSE_FROM_BASE_STATION;
        case 0x822: return PDP_FAIL_REJECTED_BY_BASE_STATION;
        case 0x823: return PDP_FAIL_CONCURRENT_SERVICES_INCOMPATIBLE;
        case 0x824: return PDP_FAIL_NO_CDMA_SERVICE;
        case 0x825: return PDP_FAIL_RUIM_NOT_PRESENT;
        case 0x826: return PDP_FAIL_CDMA_RETRY_ORDER;
        case 0x827: return PDP_FAIL_ACCESS_BLOCK;
        case 0x828: return PDP_FAIL_ACCESS_BLOCK_ALL;
        case 0x829: return PDP_FAIL_IS707B_MAX_ACCESS_PROBES;
        case 0x82A: return PDP_FAIL_THERMAL_EMERGENCY;
        case 0x82B: return PDP_FAIL_CONCURRENT_SERVICES_NOT_ALLOWED;
        case 0x82C: return PDP_FAIL_INCOMING_CALL_REJECTED;
        case 0x82D: return PDP_FAIL_NO_SERVICE_ON_GATEWAY;
        case 0x82E: return PDP_FAIL_NO_GPRS_CONTEXT;
        case 0x82F: return PDP_FAIL_ILLEGAL_MS;
        case 0x830: return PDP_FAIL_ILLEGAL_ME;
        case 0x831: return PDP_FAIL_GPRS_SERVICES_AND_NON_GPRS_SERVICES_NOT_ALLOWED;
        case 0x832: return PDP_FAIL_GPRS_SERVICES_NOT_ALLOWED;
        case 0x833: return PDP_FAIL_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK;
        case 0x834: return PDP_FAIL_IMPLICITLY_DETACHED;
        case 0x835: return PDP_FAIL_PLMN_NOT_ALLOWED;
        case 0x836: return PDP_FAIL_LOCATION_AREA_NOT_ALLOWED;
        case 0x837: return PDP_FAIL_GPRS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN;
        case 0x838: return PDP_FAIL_PDP_DUPLICATE;
        case 0x839: return PDP_FAIL_UE_RAT_CHANGE;
        case 0x83A: return PDP_FAIL_CONGESTION;
        case 0x83B: return PDP_FAIL_NO_PDP_CONTEXT_ACTIVATED;
        case 0x83C: return PDP_FAIL_ACCESS_CLASS_DSAC_REJECTION;
        case 0x83D: return PDP_FAIL_PDP_ACTIVATE_MAX_RETRY_FAILED;
        case 0x83E: return PDP_FAIL_RADIO_ACCESS_BEARER_FAILURE;
        case 0x83F: return PDP_FAIL_ESM_UNKNOWN_EPS_BEARER_CONTEXT;
        case 0x840: return PDP_FAIL_DRB_RELEASED_BY_RRC;
        case 0x841: return PDP_FAIL_CONNECTION_RELEASED;
        case 0x842: return PDP_FAIL_EMM_DETACHED;
        case 0x843: return PDP_FAIL_EMM_ATTACH_FAILED;
        case 0x844: return PDP_FAIL_EMM_ATTACH_STARTED;
        case 0x845: return PDP_FAIL_LTE_NAS_SERVICE_REQUEST_FAILED;
        case 0x846: return PDP_FAIL_DUPLICATE_BEARER_ID;
        case 0x847: return PDP_FAIL_ESM_COLLISION_SCENARIOS;
        case 0x848: return PDP_FAIL_ESM_BEARER_DEACTIVATED_TO_SYNC_WITH_NETWORK;
        case 0x849: return PDP_FAIL_ESM_NW_ACTIVATED_DED_BEARER_WITH_ID_OF_DEF_BEARER;
        case 0x84A: return PDP_FAIL_ESM_BAD_OTA_MESSAGE;
        case 0x84B: return PDP_FAIL_ESM_DOWNLOAD_SERVER_REJECTED_THE_CALL;
        case 0x84C: return PDP_FAIL_ESM_CONTEXT_TRANSFERRED_DUE_TO_IRAT;
        case 0x84D: return PDP_FAIL_DS_EXPLICIT_DEACTIVATION;
        case 0x84E: return PDP_FAIL_ESM_LOCAL_CAUSE_NONE;
        case 0x84F: return PDP_FAIL_LTE_THROTTLING_NOT_REQUIRED;
        case 0x850: return PDP_FAIL_ACCESS_CONTROL_LIST_CHECK_FAILURE;
        case 0x851: return PDP_FAIL_SERVICE_NOT_ALLOWED_ON_PLMN;
        case 0x852: return PDP_FAIL_EMM_T3417_EXPIRED;
        case 0x853: return PDP_FAIL_EMM_T3417_EXT_EXPIRED;
        case 0x854: return PDP_FAIL_RRC_UPLINK_DATA_TRANSMISSION_FAILURE;
        case 0x855: return PDP_FAIL_RRC_UPLINK_DELIVERY_FAILED_DUE_TO_HANDOVER;
        case 0x856: return PDP_FAIL_RRC_UPLINK_CONNECTION_RELEASE;
        case 0x857: return PDP_FAIL_RRC_UPLINK_RADIO_LINK_FAILURE;
        case 0x858: return PDP_FAIL_RRC_UPLINK_ERROR_REQUEST_FROM_NAS;
        case 0x859: return PDP_FAIL_RRC_CONNECTION_ACCESS_STRATUM_FAILURE;
        case 0x85A: return PDP_FAIL_RRC_CONNECTION_ANOTHER_PROCEDURE_IN_PROGRESS;
        case 0x85B: return PDP_FAIL_RRC_CONNECTION_ACCESS_BARRED;
        case 0x85C: return PDP_FAIL_RRC_CONNECTION_CELL_RESELECTION;
        case 0x85D: return PDP_FAIL_RRC_CONNECTION_CONFIG_FAILURE;
        case 0x85E: return PDP_FAIL_RRC_CONNECTION_TIMER_EXPIRED;
        case 0x85F: return PDP_FAIL_RRC_CONNECTION_LINK_FAILURE;
        case 0x860: return PDP_FAIL_RRC_CONNECTION_CELL_NOT_CAMPED;
        case 0x861: return PDP_FAIL_RRC_CONNECTION_SYSTEM_INTERVAL_FAILURE;
        case 0x862: return PDP_FAIL_RRC_CONNECTION_REJECT_BY_NETWORK;
        case 0x863: return PDP_FAIL_RRC_CONNECTION_NORMAL_RELEASE;
        case 0x864: return PDP_FAIL_RRC_CONNECTION_RADIO_LINK_FAILURE;
        case 0x865: return PDP_FAIL_RRC_CONNECTION_REESTABLISHMENT_FAILURE;
        case 0x866: return PDP_FAIL_RRC_CONNECTION_OUT_OF_SERVICE_DURING_CELL_REGISTER;
        case 0x867: return PDP_FAIL_RRC_CONNECTION_ABORT_REQUEST;
        case 0x868: return PDP_FAIL_RRC_CONNECTION_SYSTEM_INFORMATION_BLOCK_READ_ERROR;
        case 0x869: return PDP_FAIL_NETWORK_INITIATED_DETACH_WITH_AUTO_REATTACH;
        case 0x86A: return PDP_FAIL_NETWORK_INITIATED_DETACH_NO_AUTO_REATTACH;
        case 0x86B: return PDP_FAIL_ESM_PROCEDURE_TIME_OUT;
        case 0x86C: return PDP_FAIL_INVALID_CONNECTION_ID;
        case 0x86D: return PDP_FAIL_MAXIMIUM_NSAPIS_EXCEEDED;
        case 0x86E: return PDP_FAIL_INVALID_PRIMARY_NSAPI;
        case 0x86F: return PDP_FAIL_CANNOT_ENCODE_OTA_MESSAGE;
        case 0x870: return PDP_FAIL_RADIO_ACCESS_BEARER_SETUP_FAILURE;
        case 0x871: return PDP_FAIL_PDP_ESTABLISH_TIMEOUT_EXPIRED;
        case 0x872: return PDP_FAIL_PDP_MODIFY_TIMEOUT_EXPIRED;
        case 0x873: return PDP_FAIL_PDP_INACTIVE_TIMEOUT_EXPIRED;
        case 0x874: return PDP_FAIL_PDP_LOWERLAYER_ERROR;
        case 0x875: return PDP_FAIL_PDP_MODIFY_COLLISION;
        case 0x876: return PDP_FAIL_MAXINUM_SIZE_OF_L2_MESSAGE_EXCEEDED;
        case 0x877: return PDP_FAIL_NAS_REQUEST_REJECTED_BY_NETWORK;
        case 0x878: return PDP_FAIL_RRC_CONNECTION_INVALID_REQUEST;
        case 0x879: return PDP_FAIL_RRC_CONNECTION_TRACKING_AREA_ID_CHANGED;
        case 0x87A: return PDP_FAIL_RRC_CONNECTION_RF_UNAVAILABLE;
        case 0x87B: return PDP_FAIL_RRC_CONNECTION_ABORTED_DUE_TO_IRAT_CHANGE;
        case 0x87C: return PDP_FAIL_RRC_CONNECTION_RELEASED_SECURITY_NOT_ACTIVE;
        case 0x87D: return PDP_FAIL_RRC_CONNECTION_ABORTED_AFTER_HANDOVER;
        case 0x87E: return PDP_FAIL_RRC_CONNECTION_ABORTED_AFTER_IRAT_CELL_CHANGE;
        case 0x87F: return PDP_FAIL_RRC_CONNECTION_ABORTED_DURING_IRAT_CELL_CHANGE;
        case 0x880: return PDP_FAIL_IMSI_UNKNOWN_IN_HOME_SUBSCRIBER_SERVER;
        case 0x881: return PDP_FAIL_IMEI_NOT_ACCEPTED;
        case 0x882: return PDP_FAIL_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED;
        case 0x883: return PDP_FAIL_EPS_SERVICES_NOT_ALLOWED_IN_PLMN;
        case 0x884: return PDP_FAIL_MSC_TEMPORARILY_NOT_REACHABLE;
        case 0x885: return PDP_FAIL_CS_DOMAIN_NOT_AVAILABLE;
        case 0x886: return PDP_FAIL_ESM_FAILURE;
        case 0x887: return PDP_FAIL_MAC_FAILURE;
        case 0x888: return PDP_FAIL_SYNCHRONIZATION_FAILURE;
        case 0x889: return PDP_FAIL_UE_SECURITY_CAPABILITIES_MISMATCH;
        case 0x88A: return PDP_FAIL_SECURITY_MODE_REJECTED;
        case 0x88B: return PDP_FAIL_UNACCEPTABLE_NON_EPS_AUTHENTICATION;
        case 0x88C: return PDP_FAIL_CS_FALLBACK_CALL_ESTABLISHMENT_NOT_ALLOWED;
        case 0x88D: return PDP_FAIL_NO_EPS_BEARER_CONTEXT_ACTIVATED;
        case 0x88E: return PDP_FAIL_INVALID_EMM_STATE;
        case 0x88F: return PDP_FAIL_NAS_LAYER_FAILURE;
        case 0x890: return PDP_FAIL_MULTIPLE_PDP_CALL_NOT_ALLOWED;
        case 0x891: return PDP_FAIL_EMBMS_NOT_ENABLED;
        case 0x892: return PDP_FAIL_IRAT_HANDOVER_FAILED;
        case 0x893: return PDP_FAIL_EMBMS_REGULAR_DEACTIVATION;
        case 0x894: return PDP_FAIL_TEST_LOOPBACK_REGULAR_DEACTIVATION;
        case 0x895: return PDP_FAIL_LOWER_LAYER_REGISTRATION_FAILURE;
        case 0x896: return PDP_FAIL_DATA_PLAN_EXPIRED;
        case 0x897: return PDP_FAIL_UMTS_HANDOVER_TO_IWLAN;
        case 0x898: return PDP_FAIL_EVDO_CONNECTION_DENY_BY_GENERAL_OR_NETWORK_BUSY;
        case 0x899: return PDP_FAIL_EVDO_CONNECTION_DENY_BY_BILLING_OR_AUTHENTICATION_FAILURE;
        case 0x89A: return PDP_FAIL_EVDO_HDR_CHANGED;
        case 0x89B: return PDP_FAIL_EVDO_HDR_EXITED;
        case 0x89C: return PDP_FAIL_EVDO_HDR_NO_SESSION;
        case 0x89D: return PDP_FAIL_EVDO_USING_GPS_FIX_INSTEAD_OF_HDR_CALL;
        case 0x89E: return PDP_FAIL_EVDO_HDR_CONNECTION_SETUP_TIMEOUT;
        case 0x89F: return PDP_FAIL_FAILED_TO_ACQUIRE_COLOCATED_HDR;
        case 0x8A0: return PDP_FAIL_OTASP_COMMIT_IN_PROGRESS;
        case 0x8A1: return PDP_FAIL_NO_HYBRID_HDR_SERVICE;
        case 0x8A2: return PDP_FAIL_HDR_NO_LOCK_GRANTED;
        case 0x8A3: return PDP_FAIL_DBM_OR_SMS_IN_PROGRESS;
        case 0x8A4: return PDP_FAIL_HDR_FADE;
        case 0x8A5: return PDP_FAIL_HDR_ACCESS_FAILURE;
        case 0x8A6: return PDP_FAIL_UNSUPPORTED_1X_PREV;
        case 0x8A7: return PDP_FAIL_LOCAL_END;
        case 0x8A8: return PDP_FAIL_NO_SERVICE;
        case 0x8A9: return PDP_FAIL_FADE;
        case 0x8AA: return PDP_FAIL_NORMAL_RELEASE;
        case 0x8AB: return PDP_FAIL_ACCESS_ATTEMPT_ALREADY_IN_PROGRESS;
        case 0x8AC: return PDP_FAIL_REDIRECTION_OR_HANDOFF_IN_PROGRESS;
        case 0x8AD: return PDP_FAIL_EMERGENCY_MODE;
        case 0x8AE: return PDP_FAIL_PHONE_IN_USE;
        case 0x8AF: return PDP_FAIL_INVALID_MODE;
        case 0x8B0: return PDP_FAIL_INVALID_SIM_STATE;
        case 0x8B1: return PDP_FAIL_NO_COLLOCATED_HDR;
        case 0x8B2: return PDP_FAIL_UE_IS_ENTERING_POWERSAVE_MODE;
        case 0x8B3: return PDP_FAIL_DUAL_SWITCH;
        case 0x8B4: return PDP_FAIL_PPP_TIMEOUT;
        case 0x8B5: return PDP_FAIL_PPP_AUTH_FAILURE;
        case 0x8B6: return PDP_FAIL_PPP_OPTION_MISMATCH;
        case 0x8B7: return PDP_FAIL_PPP_PAP_FAILURE;
        case 0x8B8: return PDP_FAIL_PPP_CHAP_FAILURE;
        case 0x8B9: return PDP_FAIL_PPP_CLOSE_IN_PROGRESS;
        case 0x8BA: return PDP_FAIL_LIMITED_TO_IPV4;
        case 0x8BB: return PDP_FAIL_LIMITED_TO_IPV6;
        case 0x8BC: return PDP_FAIL_VSNCP_TIMEOUT;
        case 0x8BD: return PDP_FAIL_VSNCP_GEN_ERROR;
        case 0x8BE: return PDP_FAIL_VSNCP_APN_UNAUTHORIZED;
        case 0x8BF: return PDP_FAIL_VSNCP_PDN_LIMIT_EXCEEDED;
        case 0x8C0: return PDP_FAIL_VSNCP_NO_PDN_GATEWAY_ADDRESS;
        case 0x8C1: return PDP_FAIL_VSNCP_PDN_GATEWAY_UNREACHABLE;
        case 0x8C2: return PDP_FAIL_VSNCP_PDN_GATEWAY_REJECT;
        case 0x8C3: return PDP_FAIL_VSNCP_INSUFFICIENT_PARAMETERS;
        case 0x8C4: return PDP_FAIL_VSNCP_RESOURCE_UNAVAILABLE;
        case 0x8C5: return PDP_FAIL_VSNCP_ADMINISTRATIVELY_PROHIBITED;
        case 0x8C6: return PDP_FAIL_VSNCP_PDN_ID_IN_USE;
        case 0x8C7: return PDP_FAIL_VSNCP_SUBSCRIBER_LIMITATION;
        case 0x8C8: return PDP_FAIL_VSNCP_PDN_EXISTS_FOR_THIS_APN;
        case 0x8C9: return PDP_FAIL_VSNCP_RECONNECT_NOT_ALLOWED;
        case 0x8CA: return PDP_FAIL_IPV6_PREFIX_UNAVAILABLE;
        case 0x8CB: return PDP_FAIL_HANDOFF_PREFERENCE_CHANGED;
        case 0x8CC: return PDP_FAIL_SLICE_REJECTED;
        case 0x8CD: return PDP_FAIL_MATCH_ALL_RULE_NOT_ALLOWED;
        case 0x8CE: return PDP_FAIL_ALL_MATCHING_RULES_FAILED;

        case 0x0E0F: return PDP_FAIL_LOCAL_REJECT_ACT_REQ_DUE_TO_REACH_RETRY_COUNTER;
        case 0x0F46: return PDP_FAIL_TCM_ESM_TIMER_TIMEOUT;
        case 0x0F47: return PDP_FAIL_TCM_ESM_TIMER_TIMEOUT;
        case 0x0F8E: return PDP_FAIL_TCM_ESM_DEACT_DUE_TO_DRB_RELEASE;
        case 0x1402: return PDP_FAIL_PAM_ATT_PDN_ACCESS_REJECT_IMS_PDN_BLOCK_TEMP;

        case 0x1671: return PDP_FAIL_DATA_NOT_ALLOW;
        case 0x16D7: return PDP_FAIL_D2CPM_ALREADY_IN_USE;
        case 0x16F1: return PDP_FAIL_REJECTED_BY_IWLAN;
        case 0x1A27: return PDP_FAIL_VGSM_REACTIVATION_REQUESTED;
        // mapping modem cause VGSM_PROPRIETARY_CAUSE_CORE_SYNC_PDUS_STATUS_WITH_NW
        case 0x1AC0: return PDP_FAIL_TCM_ESM_DEACT_DUE_TO_DRB_RELEASE;

        case 0x2000: return PDP_FAIL_IWLAN_PDN_CONNECTION_REJECTION;
        case 0x2001: return PDP_FAIL_IWLAN_MAX_CONNECTION_REACHED;
        case 0x2031: return PDP_FAIL_IWLAN_SEMANTIC_ERROR_IN_THE_TFT_OPERATION;
        case 0x2032: return PDP_FAIL_IWLAN_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION;
        case 0x2034: return PDP_FAIL_IWLAN_SEMANTIC_ERRORS_IN_PACKET_FILTERS;
        case 0x2035: return PDP_FAIL_IWLAN_SYNTACTICAL_ERRORS_IN_PACKET_FILTERS;
        case 0x2328: return PDP_FAIL_IWLAN_NON_3GPP_ACCESS_TO_EPC_NOT_ALLOWED;
        case 0x2329: return PDP_FAIL_IWLAN_USER_UNKNOWN;
        case 0x232A: return PDP_FAIL_IWLAN_NO_APN_SUBSCRIPTION;
        case 0x232B: return PDP_FAIL_IWLAN_AUTHORIZATION_REJECTED;
        case 0x232E: return PDP_FAIL_IWLAN_ILLEGAL_ME;
        case 0x2904: return PDP_FAIL_IWLAN_NETWORK_FAILURE;
        case 0x2AF9: return PDP_FAIL_IWLAN_RAT_TYPE_NOT_ALLOWED;
        case 0x2AFD: return PDP_FAIL_IWLAN_IMEI_NOT_ACCEPTED;
        case 0x2B03: return PDP_FAIL_IWLAN_PLMN_NOT_ALLOWED;
        case 0x2B2F: return PDP_FAIL_IWLAN_UNAUTHENTICATED_EMERGENCY_NOT_SUPPORTED;
        case 0x4000: return PDP_FAIL_IWLAN_IKEV2_CONFIG_FAILURE;
        case 0x4001: return PDP_FAIL_IWLAN_IKEV2_AUTH_FAILURE;
        case 0x4002: return PDP_FAIL_IWLAN_IKEV2_MSG_TIMEOUT;
        case 0x4003: return PDP_FAIL_IWLAN_IKEV2_CERT_INVALID;
        case 0x4004: return PDP_FAIL_IWLAN_DNS_RESOLUTION_NAME_FAILURE;
        case 0x4005: return PDP_FAIL_IWLAN_DNS_RESOLUTION_TIMEOUT;

        // OEM specific error codes. To be used by OEMs when they don't want to
        // reveal error code which would be replaced by PDP_FAIL_ERROR_UNSPECIFIED
        case 0x1001: return PDP_FAIL_OEM_DCFAILCAUSE_1;
        case 0x1002: return PDP_FAIL_OEM_DCFAILCAUSE_2;
        case 0x1003: return PDP_FAIL_OEM_DCFAILCAUSE_3;
        case 0x1004: return PDP_FAIL_OEM_DCFAILCAUSE_4;
        case 0x1005: return PDP_FAIL_OEM_DCFAILCAUSE_5;
        case 0x1006: return PDP_FAIL_OEM_DCFAILCAUSE_6;
        case 0x1007: return PDP_FAIL_OEM_DCFAILCAUSE_7;
        case 0x1008: return PDP_FAIL_OEM_DCFAILCAUSE_8;
        case 0x1009: return PDP_FAIL_OEM_DCFAILCAUSE_9;
        case 0x100A: return PDP_FAIL_OEM_DCFAILCAUSE_10;
        case 0x100B: return PDP_FAIL_OEM_DCFAILCAUSE_11;
        case 0x100C: return PDP_FAIL_OEM_DCFAILCAUSE_12;
        case 0x100D: return PDP_FAIL_OEM_DCFAILCAUSE_13;
        case 0x100E: return PDP_FAIL_OEM_DCFAILCAUSE_14;
        case 0x100F: return PDP_FAIL_OEM_DCFAILCAUSE_15;

        /* Not mentioned in the specification */
        case -1: return PDP_FAIL_VOICE_REGISTRATION_FAIL;
        case -2: return PDP_FAIL_DATA_REGISTRATION_FAIL;

        /* reasons for data call drop - network/modem disconnect */
        case -3: return PDP_FAIL_SIGNAL_LOST;
        case -4: return PDP_FAIL_PREF_RADIO_TECH_CHANGED;
        case -5: return PDP_FAIL_RADIO_POWER_OFF;
        case -6: return PDP_FAIL_TETHERED_CALL_ACTIVE;
        case -7: return PDP_FAIL_ROUTER_ADVERTISEMENT_FAIL;

        /* Common cause for PDN deactivation by framework or network */
        case 0x10004: return PDP_FAIL_LOST_CONNECTION;
        case 0x10006: return PDP_FAIL_HANDOVER_FAILED;
        case 0x10007: return PDP_FAIL_DUPLICATE_CID;
        case 0x10008: return PDP_FAIL_NO_DEFAULT_DATA;
        case 0x10009: return PDP_FAIL_SERVICE_TEMPORARILY_UNAVAILABLE;
        case 0x1000A: return PDP_FAIL_REQUEST_NOT_SUPPORTED;
        case 0x1000B: return PDP_FAIL_NO_RETRY_FAILURE;

        /* For telephony service data mbrain, need report practical cause */
        // default: return PDP_FAIL_ERROR_UNSPECIFIED;
        default: return cause;
    }
}

bool RmmDcUtility::isVerizonOnSiteLteSimCard(int slotId) {
    String8 iccid = RfxMclStatusManager::getMclStatusManager(slotId)->getString8Value(RFX_STATUS_KEY_SIM_ICCID);
    if (iccid.find(VERIZON_ON_SITE_LTE_ICCID_PREFIX) == 0) {
        return true;
    }
    return false;
}
