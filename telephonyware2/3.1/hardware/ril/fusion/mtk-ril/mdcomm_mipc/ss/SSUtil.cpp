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

//#include "GsmUtil.h"
#include "SSUtil.h"
//#include "SSConfig.h"
#include "SuppServDef.h"
#include "rfx_properties.h"
#include "RfxLog.h"

#include <compiler/compiler_utils.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <telephony/mtk_ril.h>


#ifdef RFX_LOG_TAG
#undef RFX_LOG_TAG
#endif
#define RFX_LOG_TAG "SS-UTIL"

/***
 * "AO"  BAOC (Barr All Outgoing Calls) (refer 3GPP TS 22.088 [6] clause 1)
 * "OI"  BOIC (Barr Outgoing International Calls) (refer 3GPP TS 22.088 [6] clause 1)
 * "OX"  BOIC exHC (Barr Outgoing International Calls except to Home Country) (refer 3GPP TS 22.088 [6] clause 1)
 * "AI"  BAIC (Barr All Incoming Calls) (refer 3GPP TS 22.088 [6] clause 2)
 * "IR"  BIC Roam (Barr Incoming Calls when Roaming outside the home country) (refer 3GPP TS 22.088 [6] clause 2)
 * "AB"  All Barring services (refer 3GPP TS 22.030 [19]) (applicable only for <mode>=0)
 * "AG"  All outGoing barring services (refer 3GPP TS 22.030 [19]) (applicable only for <mode>=0)
 * "AC"  All inComing barring services (refer 3GPP TS 22.030 [19]) (applicable only for <mode>=0)
 * "ACR" Incoming Call Barring of Anonymous Communication Rejection (ACR) (3GPP TS 24.611)
 */
const char * callBarFacilityStrings[CB_SUPPORT_NUM] = {
    "AO",
    "OI",
    "OX",
    "AI",
    "IR",
    "AB",
    "AG",
    "AC",
    "ACR"
};

const char * callBarServiceCodeStrings[CB_SUPPORT_NUM] = {
    "33",
    "331",
    "332",
    "35",
    "351",
    "330",
    "333",
    "353",
    "157"
};

const char * GsmCbsDcsStringp[MAX_DCS_SUPPORT] = {"GSM7","8BIT","UCS2"};

const char PAUSE = ',';
const char WAIT = ';';
// const char WILD = 'N';

const uint8_t ssReasonToMipcReasonCodeArray[] = {
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_UNCONDITIONAL,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_BUSY,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_NO_REPLY,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_NOT_REACHABLE,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_ALL,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_ALL_CONDITIONAL,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_NOT_REGISTER
};

uint8_t ssReasonToMipcReasonCode(CallForwardReasonE cfReason)
{
    uint8_t mipcReason = mipc_ss_call_forward_reason_const_NONE;
    if (cfReason >= CF_U && cfReason <= CF_NOTREGIST) {
        mipcReason = ssReasonToMipcReasonCodeArray[(int) cfReason];
    }

    RFX_LOG_D(RFX_LOG_TAG, "ssReasonToMipcReasonCode:%d", mipcReason);
    return mipcReason;
}

int mipcReasonCodeToSsReason(uint8_t cfReason)
{
    int reason = 0;

    for (int i = 0; i <= (int) CF_NOTREGIST; i ++) {
        if (ssReasonToMipcReasonCodeArray[i] == (int) cfReason) {
            reason = i;
            break;
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "mipcReasonCodeToSsReason:%d", reason);
    return reason;
}


uint8_t ssStatusToMipcOpCode(SsStatusE status)
{
    switch (status) {
        case SS_ACTIVATE:
            return MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_ACTIVATE;
            break;
        case SS_DEACTIVATE:
            return MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_DEACTIVATE;
            break;
        case SS_REGISTER:
            return MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_REGISTRATION;
            break;
        case SS_ERASE:
            return MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_ERASURE;
            break;

        // case SS_INTERROGATE:
        //     break;
        default:
            return 0;
            break;
    }
}

/*
 * mipc_ss_call_barring_fac_const_enum
 */
const uint8_t callBarFacToMipcFacCodeArray[] = {
    MIPC_SS_CALL_BARRING_FAC_AO,
    MIPC_SS_CALL_BARRING_FAC_OI,
    MIPC_SS_CALL_BARRING_FAC_OX,
    MIPC_SS_CALL_BARRING_FAC_AI,
    MIPC_SS_CALL_BARRING_FAC_IR,
    MIPC_SS_CALL_BARRING_FAC_AB,
    MIPC_SS_CALL_BARRING_FAC_AG,
    MIPC_SS_CALL_BARRING_FAC_AC,
    MIPC_SS_CALL_BARRING_FAC_ACR
};

int callBarFacToServiceCode(const char * fac)
{
    int i;

    for (i = 0; i < CB_SUPPORT_NUM; i++) {
        if (0 == strcmp(fac, callBarFacilityStrings[i])) {
            break;
        }
    }

    RFX_LOG_D(RFX_LOG_TAG, "callBarFacToServiceCode= %d", i);

    if (i < CB_SUPPORT_NUM && i < sizeof(callBarFacToMipcFacCodeArray) / sizeof(uint8_t)) {
        return (int)callBarFacToMipcFacCodeArray[i];
    } else {
        /* not found! return default */
        return CB_ABS;
    }
}

RIL_Errno convertMipcResultToRil(const char *logTag, mipc_result_const_enum mipcRet)
{
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    AT_CME_Error atError = CME_SUCCESS;

    atError = (AT_CME_Error) (mipcRet & (~MIPC_RESULT_SS_EXT_BEGIN));

    RFX_LOG_D(logTag, "[%s] err:%d, atError:%d", __FUNCTION__, (int) mipcRet,
            ((int) mipcRet) & (~MIPC_RESULT_SS_EXT_BEGIN));

    switch (atError) {
        case CME_SUCCESS:
            ret = RIL_E_SUCCESS;
            break;

        case CME_SIM_PIN_REQUIRED:
            // fall through
            TELEPHONYWARE_FALLTHROUGH;

        case CME_SIM_PUK_REQUIRED:
            ret = RIL_E_PASSWORD_INCORRECT;
            break;

        case CME_SIM_PIN2_REQUIRED:
            ret = RIL_E_SIM_PIN2;
            break;

        case CME_SIM_PUK2_REQUIRED:
            ret = RIL_E_SIM_PUK2;
            break;

        case CME_INCORRECT_PASSWORD:
            ret = RIL_E_PASSWORD_INCORRECT;
            break;

        case CME_CALL_BARRED:
            // fall through
            TELEPHONYWARE_FALLTHROUGH;

        case CME_OPR_DTR_BARRING:
            // ret = RIL_E_GENERIC_FAILURE;
            ret = RIL_E_GENERIC_FAILURE;
            break;

        case CME_PHB_FDN_BLOCKED:
            ret = RIL_E_FDN_CHECK_FAILURE;
            break;

        case CME_403_FORBIDDEN:
            ret = RIL_E_UT_XCAP_403_FORBIDDEN;
            break;

        case CME_404_NOT_FOUND:
            ret = RIL_E_404_NOT_FOUND;
            break;

        case CME_409_CONFLICT:
            ret = RIL_E_409_CONFLICT;
            break;

        case CME_412_PRECONDITION_FAILED:
            ret = RIL_E_412_PRECONDITION_FAILED;
            break;

        case CME_415_UNSUPPORTED_MEDIA_TYPE:
            ret = RIL_E_415_UNSUPPORTED_MEDIA_TYPE;
            break;

        case CME_500_INTERNAL_SERVER_ERROR:
            ret = RIL_E_500_INTERNAL_SERVER_ERROR;
            break;

        case CME_503_SERVICE_UNAVAILABLE:
            ret = RIL_E_503_SERVICE_UNAVAILABLE;
            break;

        case CME_NETWORK_TIMEOUT:
            ret = RIL_E_UT_UNKNOWN_HOST;
            break;

        case CME_OPERATION_NOT_SUPPORTED:
            ret = RIL_E_REQUEST_NOT_SUPPORTED;
            break;

        case CME_844_UNPROVISIONED:
            ret = RIL_E_REQUEST_NOT_SUPPORTED;
            break;

        case CME_845_NO_DATA_CONNECTION:
            ret = RIL_E_845_NO_DATA_CONNECTION;
            break;
        default:
            break;
    }

    return ret;
}

void clearErrorMessageFromXcap(int slotId) {
    char *propName = NULL;
    char propValue[MTK_PROPERTY_VALUE_MAX] = {0};
    int idx = 0;

    vector<string> propNames(50);
    if (asprintf(&propName, "%s.%d",
            (char *) PROPERTY_ERROR_MESSAGE_FROM_XCAP, slotId) < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "Allocate prop name failed");
        return;
    }

    char *tmpPropName = NULL;
    if (asprintf(&tmpPropName, "%s.%d", (char*) propName, idx) < 0) {
        free(propName);

        RFX_LOG_E(RFX_LOG_TAG, "Allocate tmpPropName failed");
        return;
    }

    if (tmpPropName != NULL) {
        propNames[idx] = string(tmpPropName);
        rfx_property_get(propNames[idx].c_str(), propValue, "");
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "clearErrorMessageFromXcap tmpPropName is null!");
    }

    while(strlen(propValue) != 0) {
        rfx_property_set(propNames[idx].c_str(), "");

        RFX_LOG_D(RFX_LOG_TAG, "clear prop: [%s]: [%s]", propNames[idx].c_str(), propValue);

        rfx_property_get(propNames[idx].c_str(), propValue, "");
        RFX_LOG_D(RFX_LOG_TAG, "after clear prop: [%s]: [%s]", propNames[idx].c_str(), propValue);

        idx++;
        if (propName != NULL) {
            propNames[idx] = string(propName) + string(".") + to_string(idx);
            rfx_property_get(propNames[idx].c_str(), propValue, "");
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "clearErrorMessageFromXcap propNames is null!");
        }
    }

    free(propName);
    free(tmpPropName);
}

void setErrorMessageFromXcap(int slotId, AT_CME_Error errorCode, const char* message) {
    char *fullMsg = NULL;

    if (message == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "setErrorMessageFromXcap: NULL string.");
        return;
    }

    switch (errorCode) {
        case CME_409_CONFLICT:
            break;
        case CME_412_PRECONDITION_FAILED:
            break;
        case CME_415_UNSUPPORTED_MEDIA_TYPE:
            break;
        case CME_500_INTERNAL_SERVER_ERROR:
            break;
        case CME_503_SERVICE_UNAVAILABLE:
            break;
        default:
            return;
    }

    int len = strlen(message);

    if (len == 0) {
        RFX_LOG_E(RFX_LOG_TAG, "setErrorMessageFromXcap: strin length is 0.");
        return;
    }

    if (asprintf(&fullMsg, "%d;%s", errorCode, message) < 0)  {
        RFX_LOG_E(RFX_LOG_TAG, "setErrorMessageFromXcap: asprintf failed");
        return;
    }

    if (fullMsg != NULL) {
        len = strlen(fullMsg);
    }

    char *propName = NULL;

    if (asprintf(&propName, "%s.%d", (char *) PROPERTY_ERROR_MESSAGE_FROM_XCAP, slotId) < 0)  {
        free(fullMsg);
        RFX_LOG_E(RFX_LOG_TAG, "setErrorMessageFromXcap: asprintf failed");
        return;
    }

    RFX_LOG_D(RFX_LOG_TAG, "propName: [%s]", propName);
    RFX_LOG_D(RFX_LOG_TAG, "fullMsg: [%s]", fullMsg);

    int idx = 0;

    char *tmpMsg = fullMsg;

    while (len > 0) {
        RFX_LOG_D(RFX_LOG_TAG, "len: [%d], max: [%d]", len, MTK_PROPERTY_VALUE_MAX);

        char tmpStr[MTK_PROPERTY_VALUE_MAX] = {0};
        int copyLen = 0;
        if (len >= MTK_PROPERTY_VALUE_MAX - 1) {
            copyLen = MTK_PROPERTY_VALUE_MAX - 1;
        }

        if (len < MTK_PROPERTY_VALUE_MAX) {
            copyLen = len;
        }

        strncpy(tmpStr, tmpMsg, copyLen);
        tmpMsg += copyLen;
        // printf("%s\n", tmpStr);

        char *tmpPropName = NULL;

        if (asprintf(&tmpPropName, "%s.%d", (char*) propName, idx) < 0) {
            free(fullMsg);
            free(propName);

            RFX_LOG_E(RFX_LOG_TAG, "setErrorMessageFromXcap: asprintf failed");
            return;
        }

        // setMSimProperty(slotId,
        //         propName,
        //         tmpStr);

        RFX_LOG_D(RFX_LOG_TAG, "set xcap error sysprop: [%s][%s]", tmpPropName, tmpStr);
        rfx_property_set(tmpPropName, tmpStr);

        free(tmpPropName);

        idx++;
        len -= (MTK_PROPERTY_VALUE_MAX - 1);
    }

    RFX_LOG_D(RFX_LOG_TAG, "setErrorMessageFromXcap done.");
    free(fullMsg);
    free(propName);
}


/** True if c is ISO-LATIN characters 0-9, *, # , + */
bool isReallyDialable(char c) {
    return (c >= '0' && c <= '9') || c == '*' || c == '#' || c == '+';
}

/** This any anything to the right of this char is part of the
 *  post-dial string (eg this is PAUSE or WAIT)
 */
bool isStartsPostDial (char c) {
    return c == PAUSE || c == WAIT;
}

bool startsWith(const string& target, const string& origin) {
    return target.length() <= origin.length()
        && equal(target.begin(), target.end(), origin.begin());
}

char* encryptString(char* input) {
    if (NULL == input) {
        return NULL;
    }
    int len = strlen(input);

    int pivot = (rand() % 9) + 1;
    char* array = (char *) malloc(len + 2);

    if (array == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "encryptString, malloc failed!");
        return NULL;
    }

    strncpy(array, input, len);
    for (int i = 0; i < len; i++) {
        array[i] = (char) (array[i] + pivot);
    }
    char pivotStr[16] = {0};
    if (sprintf(pivotStr, "%d", pivot) < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "encryptString, sprintf failed!");
        return NULL;
    } else {
        array[len] = pivotStr[0];
        array[len + 1] = '\0';
        return array;
    }
}
