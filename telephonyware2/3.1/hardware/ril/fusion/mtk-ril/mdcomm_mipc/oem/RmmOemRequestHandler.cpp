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

#include "RmmOemRequestHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringsData.h"
#include "RfxStringData.h"
#include "RfxRawData.h"
#include "RfxActivityData.h"
#include <string.h>
#include "RfxVersionManager.h"
#include "ratconfig.h"
#include "RfxRilUtils.h"
#include "RfxPhoneCapabilityData.h"
#include "RfxCertMsgData.h"
#include "RfxCertResponseData.h"
#include "RfxAuthMsgData.h"
#include "RfxAuthResponseData.h"
#include "RfxCapabilityData.h"
#include "RfxQueryCapData.h"
#include <algorithm>
#include <stdexcept>
#include <dlfcn.h>
#include <libmtkrilutils.h>
#include "RfxMipcData.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"

#define RFX_LOG_TAG "RmmOemHandler"

#define PROPERTY_GSM_GCF_TEST_MODE  "vendor.gsm.gcf.testmode"
#define UTILS_PATH "libcapctrl.so"
#define PROPERTY_LOG_CERTIFICATE "vendor.ril.certificate"
#define PROPERTY_LOG_ENABLE_CAPABILITY "vendor.ril.enablecapability"
#define NETWORK_MODE_WCDMA_PREF 3
#define NETWORK_MODE_GSM_UMTS_LTE 7

void* RmmOemRequestHandler::sDlHandler = NULL;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmOemRequestHandler, RIL_CMD_PROXY_3);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData,
        RFX_MSG_REQUEST_DEVICE_IDENTITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData,
        RFX_MSG_REQUEST_OEM_HOOK_RAW);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxStringsData,
        RFX_MSG_REQUEST_OEM_HOOK_STRINGS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxActivityData,
        RFX_MSG_REQUEST_GET_ACTIVITY_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData,
        RFX_MSG_REQUEST_BASEBAND_VERSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_SET_TRM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxPhoneCapabilityData,
        RFX_MSG_REQUEST_GET_PHONE_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCertMsgData, RfxCertResponseData,
        RFX_MSG_REQUEST_ROUTE_CERTIFICATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxAuthMsgData, RfxAuthResponseData,
        RFX_MSG_REQUEST_ROUTE_AUTH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCapabilityData, RfxVoidData,
        RFX_MSG_REQUEST_ENABLE_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_ABORT_CERTIFICATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxQueryCapData, RfxIntsData,
        RFX_MSG_REQUEST_QUERY_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_SET_MODEM_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_SET_HSR_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_NV_RESET_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData,
        RFX_MSG_REQUEST_DEVICE_IMEI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_SET_TX_POWER);

RmmOemRequestHandler::RmmOemRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int request[] = {
        RFX_MSG_REQUEST_DEVICE_IDENTITY,
        RFX_MSG_REQUEST_OEM_HOOK_RAW,
        RFX_MSG_REQUEST_OEM_HOOK_STRINGS,
        RFX_MSG_REQUEST_GET_ACTIVITY_INFO,
        RFX_MSG_REQUEST_BASEBAND_VERSION,
        RFX_MSG_REQUEST_SET_TRM,
        RFX_MSG_REQUEST_DEVICE_IMEI,
        RFX_MSG_REQUEST_SET_TX_POWER_STATUS,
        RFX_MSG_REQUEST_GET_PHONE_CAPABILITY,
        RFX_MSG_REQUEST_ROUTE_CERTIFICATE,
        RFX_MSG_REQUEST_ROUTE_AUTH,
        RFX_MSG_REQUEST_ENABLE_CAPABILITY,
        RFX_MSG_REQUEST_ABORT_CERTIFICATE,
        RFX_MSG_REQUEST_QUERY_CAPABILITY,
        RFX_MSG_REQUEST_SEND_SAR_IND,
        RFX_MSG_REQUEST_SET_MODEM_CONFIG,
        RFX_MSG_REQUEST_SET_HSR_MODE,
        RFX_MSG_REQUEST_NV_RESET_CONFIG,
        RFX_MSG_REQUEST_SET_TX_POWER,
        RFX_MSG_REQUEST_SYNC_APP_EVENT_STATUS,
    };

    registerToHandleRequest(request, sizeof(request)/sizeof(int));

    if (RFX_SLOT_ID_0 == slot_id) {
        requestMdVersion();
    }
    // set imei svn
    // atSendCommand("AT+EGMR=1,9,\"01\"");
    /*  Enable getting CFU info +ECFU and speech info +ESPEECH*/
    int einfo_value;
    einfo_value = 50;//default value.

    /*  Enable getting CFU info +ECFU and speech info +ESPEECH and modem warning +EWARNING(0x100) */
    char modemWarningProperty[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.modem.warning", modemWarningProperty, "0");
    modemWarningProperty[RFX_PROPERTY_VALUE_MAX - 1] = '\0';
    if (strcmp(modemWarningProperty, "1") == 0) {
        /* Enable "+EWARNING" */
       einfo_value |= 512;
    }
    /* Enable response message of call ctrl by sim. */
    einfo_value |= 1024;
    atSendCommand(String8::format("AT+EINFO=%d", einfo_value));

    /* M: Start - abnormal event logging for logger */
    einfo_value |= 8;
    /* Enable smart logging no service notification +ENWINFO */
    atSendCommand(String8::format("AT+EINFO=%d,401,0", einfo_value));
    /* M: End - abnormal event logging for logger */
    requestGetPhoneBaseInfo();
    int mainSlotId = RfxRilUtils::getMajorSim() - 1;
    if (mainSlotId == slot_id) {
        enableMdProtocol();
        updateSupportDSBP();
        requestGetGcfMode();
    }
    rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
    rfx_property_set(PROPERTY_LOG_ENABLE_CAPABILITY, "0");
}

RmmOemRequestHandler::~RmmOemRequestHandler() {
}


void RmmOemRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch (id) {
        case RFX_MSG_REQUEST_DEVICE_IDENTITY:
            requestDeviceIdentity(msg);
            break;
        case RFX_MSG_REQUEST_OEM_HOOK_RAW:
            requestOemHookRaw(msg);
            break;
        case RFX_MSG_REQUEST_OEM_HOOK_STRINGS:
            requestOemHookStrings(msg);
            break;
        case RFX_MSG_REQUEST_DEVICE_IMEI:
            requestDeviceImei(msg);
            break;
        case RFX_MSG_REQUEST_GET_ACTIVITY_INFO:
            requestGetActivityInfo(msg);
            break;
        case RFX_MSG_REQUEST_BASEBAND_VERSION:
            requestBasebandVersion(msg);
            break;
        case RFX_MSG_REQUEST_SET_TRM:
            requestSetTrm(msg);
            break;
        case RFX_MSG_REQUEST_SET_TX_POWER_STATUS:
            requestSetTxPowerStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_PHONE_CAPABILITY:
            requestGetPhoneCapability(msg);
            break;
        case RFX_MSG_REQUEST_ROUTE_CERTIFICATE:
            routeCert(msg);
            break;
        case RFX_MSG_REQUEST_ROUTE_AUTH:
            routeAuth(msg);
            break;
        case RFX_MSG_REQUEST_ENABLE_CAPABILITY:
            enableCapability(msg);
            break;
        case RFX_MSG_REQUEST_ABORT_CERTIFICATE:
            abortCert(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_CAPABILITY:
            queryCapability(msg);
            break;
        case RFX_MSG_REQUEST_SEND_SAR_IND:
            requestSendSarIndicator(msg);
            break;
        case RFX_MSG_REQUEST_SET_MODEM_CONFIG:
            requestSetModemConfig(msg);
            break;
        case RFX_MSG_REQUEST_SET_HSR_MODE:
            requestSetHsrMode(msg);
            break;
        case RFX_MSG_REQUEST_NV_RESET_CONFIG:
            requestNvResetConfig(msg);
            break;
        case RFX_MSG_REQUEST_SET_TX_POWER:
            handleSetTxPowerRequest(msg);
            break;
        case RFX_MSG_REQUEST_SYNC_APP_EVENT_STATUS:
            syncAppEventStatusRequest(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmOemRequestHandler::onHandleTimer() {
    // do something
}

void RmmOemRequestHandler::requestDeviceIdentity(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    char* t_result_ptr;
    uint16_t t_system_len = 0;
    char esn[20 + 1] = {0};
    char meid[32 + 1] = { 0 };
    char** rspIdentity = (char**)calloc(4, sizeof(char*));

    if (rspIdentity == NULL) {
        logE(RFX_LOG_TAG, "requestDeviceIdentity calloc memory fail");
        goto error;
    }

    // Query Imei
    if (mImei.isEmpty()) {
        requestGetPhoneBaseInfo();
    }
    if (!mImei.isEmpty()) {
        rspIdentity[0] = (char *)mImei.string();
    } else {
        logE(RFX_LOG_TAG, "requestDeviceIdentity mImei error");
        goto error;
    }

    // Query ImeiSv
    if (mImeiSv.isEmpty()) {
        requestGetImeisv();
    }
    if (!mImeiSv.isEmpty()) {
        rspIdentity[1] = (char *)mImeiSv.string();
    } else {
        logE(RFX_LOG_TAG, "requestDeviceIdentity mImeiSv error");
        goto error;
    }

    rspIdentity[2] = NULL;
    rspIdentity[3] = NULL;

    if (RatConfig_isC2kSupported()) {
        sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_GET_INFO_REQ, m_slot_id);
        sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
        if (cnfMipc == NULL || cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
            logE(RFX_LOG_TAG, "requestDeviceIdentity mipc error");
            goto error;
        }
        // Query ESN, the string type, and have the end value '\0'
        t_result_ptr = (char *)cnfMipc->getMipcVal(MIPC_SYS_GET_INFO_CNF_T_ESN_V1, &t_system_len);
        if (NULL == t_result_ptr) {
            logE(RFX_LOG_TAG, "requestDeviceIdentity ESN error");
            goto error;
        }

        if (NULL == strncpy(esn, t_result_ptr, t_system_len > 20 ? 20 : t_system_len)) {
            logE(RFX_LOG_TAG, "requestDeviceIdentity strncpy ESN error");
            goto error;
        }
        esn[20] = '\0';

        if (strstr(esn, "0x") != NULL) {
            rspIdentity[2] = esn + 2;
            // logD(RFX_LOG_TAG, "identity.esnHex = %s", rspIdentity[2]);
        } else {
            rspIdentity[2] = esn;
            // logD(RFX_LOG_TAG, "identity.esnDec = %s", rspIdentity[2]);
        }

        //  Query MEID, the string type, and have the end value '\0'
        t_result_ptr = (char *)cnfMipc->getMipcVal(MIPC_SYS_GET_INFO_CNF_T_MEID_V1, &t_system_len);
        if (NULL == t_result_ptr) {
            logE(RFX_LOG_TAG, "requestDeviceIdentity MEID error");
            goto error;
        }

        if (NULL == strncpy(meid, t_result_ptr, t_system_len > 32 ? 32 : t_system_len)) {
            logE(RFX_LOG_TAG, "requestDeviceIdentity strncpy MEID error");
            goto error;
        }
        meid[32] = '\0';
        int flag = 0;
        if (strstr(meid, "0x") != NULL) {
            flag = 1;
            rspIdentity[3] = meid + 2;
            // logD(RFX_LOG_TAG, "identity.meidHex = %s", rspIdentity[3]);
        } else {
            rspIdentity[3] = meid;
            // logD(RFX_LOG_TAG, "identity.meidDec = %s", rspIdentity[3]);
        }

        if (flag == 1 && rspIdentity[3] != NULL) {
            size_t len = strlen(rspIdentity[3]);
            for (size_t i = 0; i < len; i++) {
                if (rspIdentity[3][i] >= 'a' && rspIdentity[3][i] <= 'z') {
                    rspIdentity[3][i] -= 32;
                }
            }
            // logD(RFX_LOG_TAG, "after identity.meidHex = %s", rspIdentity[3]);
        }
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData((char **)rspIdentity, 4), msg, false);
    responseToTelCore(response);
    free(rspIdentity);
    return;
error:
    if (rspIdentity != NULL) {
        free(rspIdentity);
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_EMPTY_RECORD,
            RfxStringsData(), msg, false);
    responseToTelCore(response);
}

void RmmOemRequestHandler::requestOemHookRaw(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    RfxAtLine *pCur = NULL;
    char* data = (char *) msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();
    char* line = NULL;
    int i = 0;
    int strLength = 0;
    int size = -1;
    sp<RfxMclMessage> responseMsg;
    char ok[3] = {'O', 'K', 0};
    char err[] = {'E', 'R', 'R', 'O', 'R', 0};

    if (sendOemToNwModule(data)) {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxRawData(ok, strlen(ok)+1), msg);
        responseToTelCore(responseMsg);
        return;
    }

    if (canSendAtUsingOemHook(data) < 0) {
        logE(RFX_LOG_TAG, "OEM_HOOK_RAW not send %s, fail", data);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxRawData(err, strlen(err) + 1), msg);
        responseToTelCore(responseMsg);
        return;
    }

    int index = needToHidenLog(data);
    if (index >= 0) {
        logD(RFX_LOG_TAG, "data = %s****, length = %d", getHidenLogPreFix(index), datalen);
    } else {
        logD(RFX_LOG_TAG, "data = %s, length = %d", data, datalen);
    }

    pResponse = atSendCommandRaw(data);

    if (pResponse == NULL
            || pResponse->getError() < 0
            || pResponse->getSuccess() == 0
            || pResponse->getFinalResponse() == NULL
            || pResponse->getFinalResponse()->getLine() == NULL) {
        logE(RFX_LOG_TAG, "OEM_HOOK_RAW fail");
        goto error;
    }
    logD(RFX_LOG_TAG, "success = %d, finalResponse", pResponse->getSuccess());

    strLength += 2; //for the pre tag of the first string in response.

    for (pCur = pResponse->getIntermediates(); pCur != NULL; pCur = pCur->getNext()) {
        if (pCur->getLine() == NULL) {
            logE(RFX_LOG_TAG, "pResponse->getIntermediates() is null");
            break;
        }
        logD(RFX_LOG_TAG, "pResponse->getIntermediates() = <%s>", pCur->getLine());
        strLength += (strlen(pCur->getLine()) + 2); //M:To append \r\n
    }
    strLength += (strlen(pResponse->getFinalResponse()->getLine()) + 2);
    logD(RFX_LOG_TAG, "strLength = %d", strLength);

    size = strLength * sizeof(char) + 1;
    line = (char *) alloca(size);
    if (line == NULL) {
        logE(RFX_LOG_TAG, "OOM");
        goto error;
    }
    memset(line, 0, size);
    strncpy(line, "\r\n", 2);
    line[size - 1] = '\0';

    for (i = 0, pCur = pResponse->getIntermediates(); pCur != NULL; pCur = pCur->getNext(), i++) {
       if (pCur->getLine() == NULL) {
           logE(RFX_LOG_TAG, "pCur->getLine() %d  null", i);
           break;
       }
       strncat(line, pCur->getLine(), strlen(pCur->getLine()));
       strncat(line, "\r\n", 2);
       logD(RFX_LOG_TAG, "line[%d] = <%s>", i, line);
    }
    strncat(line, pResponse->getFinalResponse()->getLine(),
            strlen(pResponse->getFinalResponse()->getLine()));
    strncat(line, "\r\n", 2);
    logD(RFX_LOG_TAG, "line = <%s>", line);
    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxRawData(line, strlen(line)), msg);
    responseToTelCore(responseMsg);
    return;

error:
    line = (char *) alloca(10);
    if (line == NULL) {
        logE(RFX_LOG_TAG, "OOM");
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxRawData(), msg);
    } else {
        memset(line, 0, 10);
        strncpy(line, "\r\nERROR\r\n", 9);
        logD(RFX_LOG_TAG, "line = <%s>", line);
        line[9] = '\0';
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxRawData(line, strlen(line)), msg);
    }
    responseToTelCore(responseMsg);

    return;
}

void RmmOemRequestHandler::requestOemHookStrings(const sp<RfxMclMessage>& msg) {
    int i;
    const char ** cur;
    sp<RfxAtResponse> pResponse;
    RfxAtLine *pCur = NULL;
    char** line;
    char **data = (char **) msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();
    int strLength = datalen / sizeof(char *);
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> responseMsg;

    logD(RFX_LOG_TAG, "got OEM_HOOK_STRINGS: 0x%8p %lu", data, (long)datalen);

    if (strLength != 2) {
        /* Non proietary. Loopback! */
        logE(RFX_LOG_TAG, "OEM_HOOK_STRINGS not send to modem since strLength(%d)", strLength);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxStringsData((void *)data,
                datalen), msg);
        responseToTelCore(responseMsg);
        return;
    }

    /* For AT command access */
    cur = (const char **)data;
    int index = needToHidenLog(cur[0]);
    if (index >= 0) {
        logD(RFX_LOG_TAG, "OEM_HOOK_STRINGS : receive %s ****", getHidenLogPreFix(index));
    } else {
        logD(RFX_LOG_TAG, "OEM_HOOK_STRINGS : receive %s", cur[0]);
    }
    if (NULL != cur[0] && strlen(cur[0]) != 0 && strncmp(cur[0],"SET_TRM",7) == 0) {
        int mode = atoi(cur[1]);
        logD(RFX_LOG_TAG, "OEM_HOOK_STRINGS SET_TRM: receive mode %d", mode);
        switch (mode) {
            case 1:
                rfx_property_set("vendor.ril.mux.report.case", "1");
                rfx_property_set("vendor.ril.muxreport", "1");
                break;
            case 2:
                rfx_property_set("vendor.ril.mux.report.case", "2");
                rfx_property_set("vendor.ril.muxreport", "1");
                break;
            default:
                break;
        }
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }  else if (NULL != data[0] && strlen(data[0]) != 0 && strStartsWith(data[0], "SET_SS_PROP")) {
        logD(RFX_LOG_TAG, "Send SET_XCAP_CONFIG_EVENT");
        sendEvent(RFX_MSG_EVENT_SET_XCAP_CONFIG,
                RfxStringsData(msg->getData()->getData(), msg->getData()->getDataLength()),
                RIL_CMD_PROXY_6, m_slot_id, -1, msg->getToken());
        return;
    }

    if (NULL != cur[1] && strlen(cur[1]) != 0) {
        /*
        * Response of these two command would not contain prefix. For example,
        * AT+CGSN
        * 490154203237518
        * OK
        * So, RILD should use atSendCommandNumeric to stroe intermediate instead of atSendCommandMultiline
        */
        if ((strncmp(cur[1],"+CIMI",5) == 0)
                ||(strncmp(cur[1],"+CGSN",5) == 0)
                ||(strncmp(cur[1],"+CCHO:",6) == 0)
                ) {
            pResponse = atSendCommandNumeric(cur[0]);
        } else {
            pResponse = atSendCommandMultiline(cur[0], cur[1]);
        }
    } else {
        pResponse = atSendCommand(cur[0]);
    }

    switch (pResponse->atGetCmeError()) {
        case CME_SUCCESS:
            ret = RIL_E_SUCCESS;
            break;
        case CME_INCORRECT_PASSWORD:
            ret = RIL_E_PASSWORD_INCORRECT;
            break;
        case CME_SIM_PIN_REQUIRED:
        case CME_SIM_PUK_REQUIRED:
            ret = RIL_E_PASSWORD_INCORRECT;
            break;
        case CME_SIM_PIN2_REQUIRED:
            ret = RIL_E_SIM_PIN2;
            break;
        case CME_SIM_PUK2_REQUIRED:
            ret = RIL_E_SIM_PUK2;
            break;
        case CME_OPERATION_NOT_ALLOWED_ERR:
            ret = RIL_E_OPERATION_NOT_ALLOWED;
            break;
        default:
            ret = RIL_E_GENERIC_FAILURE;
            break;
    }
    if (pResponse->isAtResponseFail()) {
        logE(RFX_LOG_TAG, "OEM_HOOK_STRINGS fail");
        goto error;
    }

    if (ret != RIL_E_SUCCESS) {
        goto error;
    }

    /* Count response length */
    strLength = 0;

    for (pCur = pResponse->getIntermediates(); pCur != NULL;
        pCur = pCur->getNext())
        strLength++;

    if (strLength == 0) {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    } else {
        logV(RFX_LOG_TAG, "%d of %s received!",strLength, cur[1]);

        line = (char **) alloca(strLength * sizeof(char *));
        if (line == NULL) {
            logE(RFX_LOG_TAG, "OOM");
            goto error;
        }
        for (i = 0, pCur = pResponse->getIntermediates(); pCur != NULL;
                pCur = pCur->getNext(), i++) {
            line[i] = pCur->getLine();
        }
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxStringsData(line, strLength), msg);
    }
    responseToTelCore(responseMsg);
    return;

error:
    responseMsg = RfxMclMessage::obtainResponse(ret, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::requestGetPhoneBaseInfo() {
    char* result_ptr;
    uint16_t system_len = 0;
    // the length of imei is fixed, 15
    char imei[15 + 1] = {0};
    // the lenght of imeiSV is fixed, 2
    char imeiSv[2 + 1] = {0};
    // the project/flavor info len is fixed, 64
    char proj[64 + 1] = {0};
    char flavor[64 + 1] = {0};
    char *outStr = NULL;

    if (mImei.isEmpty() || mImeiSv.isEmpty()) {
        sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_GET_INFO_REQ, m_slot_id);
        sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);

        if (cnfMipc == NULL || cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
            logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo mipc error");
            return;
        }

        result_ptr = (char *)cnfMipc->getMipcVal(MIPC_SYS_GET_INFO_CNF_T_DEVICE_ID, &system_len);
        if (NULL != result_ptr) {
            if (NULL != strncpy(imei, result_ptr, 15)) {
                imei[15] = '\0';
                // logD(RFX_LOG_TAG, "requestGetPhoneBaseInfo imei:%s, len:%d", imei, system_len);
                mImei = String8(imei);
            }
        } else {
            logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo imei Fail");
        }

        result_ptr = (char *)cnfMipc->getMipcVal(MIPC_SYS_GET_INFO_CNF_T_IMEISV, &system_len);
        if (NULL != result_ptr) {
            if (NULL != strncpy(imeiSv, result_ptr, 2)) {
                imeiSv[2] = '\0';
                // logD(RFX_LOG_TAG, "requestGetPhoneBaseInfo imeisv:%s, len:%d", imeiSv, system_len);
                mImeiSv = String8(imeiSv);
            }
        } else {
            logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo imeisv Fail");
        }

        int mainSlotId = RfxRilUtils::getMajorSim() - 1;
        if (mainSlotId != m_slot_id) {
            return;
        }

        result_ptr = (char *)cnfMipc->getMipcVal(MIPC_SYS_GET_INFO_CNF_T_PROJECT_NAME, &system_len);
        if (NULL != result_ptr) {
            if (NULL == strncpy(proj, result_ptr, 64)) {
                logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo project name copy Fail");
                return;
            }
            proj[64]= '\0';
        } else {
            logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo project name Fail");
            return;
        }

        result_ptr = (char *)cnfMipc->getMipcVal(MIPC_SYS_GET_INFO_CNF_T_FLAVOR_NAME, &system_len);
        if (NULL != result_ptr) {
           if (NULL == strncpy(flavor, result_ptr, 64)) {
                logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo flavor name copy Fail");
                return;
           }
           flavor[64]= '\0';
        } else {
           logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo get flavor name Fail");
        }

        logI(RFX_LOG_TAG, "proj: %s, flavor: %s", proj, flavor);
        if (asprintf(&outStr, "%s(%s)", proj, flavor) < 0) {
            logE(RFX_LOG_TAG, "requestGetPhoneBaseInfo asprintf project and flavor Fail");
            return;
        }

        if (outStr != NULL) {
            rfx_property_set("vendor.gsm.project.baseband", outStr);
            free(outStr);
        }
    } else {
        logD(RFX_LOG_TAG, "requestGetPhoneBaseInfo imei and sv not null");
    }
}


void RmmOemRequestHandler::requestDeviceImei(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    char **responses = NULL;
    char imeiType[1 + 1] = {0};
    sp<RfxMipcData> initMipc = NULL;
    sp<RfxMipcData> cnfMipc = NULL;
    responses = (char**)calloc(3, sizeof(char*));
    if (responses == NULL) {
        logE(RFX_LOG_TAG, "requestDeviceImei calloc fail");
        goto error;
    }

    initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_GET_PRIMARY_IMEI_INFO_REQ, m_slot_id);
    cnfMipc = callToMipcMsgSync(initMipc);
    if (cnfMipc == NULL || cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "requestDeviceImei md not support query primary imei");
        // the type is primary or secondary
        if (m_slot_id == 0) {
            imeiType[0] = '1';
        } else {
            imeiType[0] = '2';
        }
    } else {
        uint8_t primarySlot = cnfMipc->getMipcUint8Val(
                MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF_T_PRIMARY_SLOT_ID, 0xFF);
        if (primarySlot != 0xFF) {
            logI(RFX_LOG_TAG, "requestDeviceImei primary imei slot id(%d)", primarySlot);
            if (primarySlot == m_slot_id) {
                imeiType[0] = '1';
            } else {
                imeiType[0] = '2';
            }
        } else {
            logE(RFX_LOG_TAG, "requestDeviceImei query primary imei slot id fail");
            // the type is primary or secondary
            if (m_slot_id == 0) {
                imeiType[0] = '1';
            } else {
                imeiType[0] = '2';
            }
        }
    }

    imeiType[1] = '\0';
    responses[0] = imeiType;

    // Query Imei
    if (mImei.isEmpty()) {
        requestGetPhoneBaseInfo();
    }
    if (!mImei.isEmpty()) {
        responses[1] = (char *)mImei.string();
    } else {
        logE(RFX_LOG_TAG, "requestDeviceImei mImei error");
        goto error;
    }

    // Query ImeiSv
    if (mImeiSv.isEmpty()) {
        requestGetImeisv();
    }
    if (!mImeiSv.isEmpty()) {
        responses[2] = (char *)mImeiSv.string();
    } else {
        logE(RFX_LOG_TAG, "requestDeviceImei mImeiSv error");
        goto error;
    }

    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData((char **)responses, 3), msg, false);
    responseToTelCore(responseMsg);
    free(responses);
    return;
error:
    if (responses != NULL) {
        free(responses);
    }
    responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::requestGetImeisv() {
    requestGetPhoneBaseInfo();
}

void RmmOemRequestHandler::requestGetActivityInfo(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    RIL_ActivityStatsInfo *activityStatsInfo = NULL;  // RIL_NUM_TX_POWER_LEVELS 5
    int num_tx_levels = 0;
    uint8_t numTxLevels = 0;
    uint16_t val_len = 0;
    mipc_nw_tx_struct4* tx_mode_t = NULL;

    sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(MIPC_NW_GET_ACTIVITY_INFO_REQ, m_slot_id);
    sp<RfxMipcData> mipcCnf = callToMipcMsgSync(mipcMsg);
    if (mipcCnf == NULL || mipcCnf->getData() == NULL || mipcCnf->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "requestGetActivityInfo error");
        responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }

    activityStatsInfo = (RIL_ActivityStatsInfo*)calloc(1, sizeof(RIL_ActivityStatsInfo));
    if (activityStatsInfo == NULL) {
        logE(RFX_LOG_TAG, "requestGetActivityInfo OOM");
        goto error;
    }

    numTxLevels = mipcCnf->getMipcUint8Val(MIPC_NW_GET_ACTIVITY_INFO_CNF_T_NUM_TX_LEVELS, 0);
    num_tx_levels = static_cast<int>(numTxLevels);
    if (num_tx_levels > RIL_NUM_TX_POWER_LEVELS) {
        logE(RFX_LOG_TAG, "requestGetActivityInfo TX level invalid (%d)", num_tx_levels);
        goto error;
    }

    tx_mode_t = (mipc_nw_tx_struct4*)mipc_msg_get_val_ptr(
            mipcCnf->getData(), MIPC_NW_GET_ACTIVITY_INFO_CNF_T_TX, &val_len);

    for (int i = 0; i < num_tx_levels; i++) {
        activityStatsInfo->tx_mode_time_ms[i] =  tx_mode_t->tx[i];
    }

    activityStatsInfo->rx_mode_time_ms = mipcCnf->getMipcUint32Val(
            MIPC_NW_GET_ACTIVITY_INFO_CNF_T_RX, 0);
    activityStatsInfo->sleep_mode_time_ms = mipcCnf->getMipcUint32Val(
            MIPC_NW_GET_ACTIVITY_INFO_CNF_T_SLEEP_TIME, 0);
    activityStatsInfo->idle_mode_time_ms = mipcCnf->getMipcUint32Val(
            MIPC_NW_GET_ACTIVITY_INFO_CNF_T_IDLE_TIME, 0);

    logD(RFX_LOG_TAG, "requestGetActivityInfo Tx/Rx (%d, %d, %d, %d, %d, %d, %d, %d, %d)",
            num_tx_levels,
            activityStatsInfo->tx_mode_time_ms[0], activityStatsInfo->tx_mode_time_ms[1],
            activityStatsInfo->tx_mode_time_ms[2], activityStatsInfo->tx_mode_time_ms[3],
            activityStatsInfo->tx_mode_time_ms[4], activityStatsInfo->rx_mode_time_ms,
            activityStatsInfo->sleep_mode_time_ms, activityStatsInfo->idle_mode_time_ms);

    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxActivityData((void *)activityStatsInfo, sizeof(RIL_ActivityStatsInfo)), msg);
    responseToTelCore(responseMsg);
    free(activityStatsInfo);
    return;
error:
    if (activityStatsInfo != NULL) {
        free(activityStatsInfo);
    }
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::requestBasebandVersion(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    char* result_ptr;
    char* temp;
    // the firmware info len is fixed, 64
    char baseband[64 + 1] = {0};
    uint16_t t_system_len = 0;
    char *ver = NULL;
    int len = 0;

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_GET_INFO_REQ, m_slot_id);
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
    if (cnfMipc == NULL || cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    result_ptr = (char *)cnfMipc->getMipcVal(MIPC_SYS_GET_INFO_CNF_T_FIRMWARE, &t_system_len);
    if (NULL == result_ptr) {
        goto error;
    }

    strncpy(baseband, result_ptr, 64);
    baseband[64]= '\0';
    logI(RFX_LOG_TAG, "requestBasebandVersion:%s", baseband);
    temp = baseband;
    len = strlen(temp);
    if (len == 0) {
        goto error;
    }
    while (len > 0 && (isspace(temp[len - 1]) || (temp[len - 1] != ','))) {
        len--;
    }
    temp[len - 1] = '\0';

    // remove the white space from the beginning
    while ((*temp) != '\0' && isspace(*temp)) {
        temp++;
    }
    ver = temp;

    logI(RFX_LOG_TAG, "requestBasebandVersion last result:%s", temp);
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringData((void *)ver, strlen(ver)), msg);
    responseToTelCore(responseMsg);
    return;

error:
    logE(RFX_LOG_TAG, "requestBasebandVersion Fail");
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::requestSetTrm(const sp<RfxMclMessage>& msg) {
    int* mode = (int*)(msg->getData()->getData());

    logD(RFX_LOG_TAG, "requestSetTrm: %d", *mode);

    switch (*mode) {
        case 1:
            rfx_property_set("vendor.ril.mux.report.case", "1");
            rfx_property_set("vendor.ril.muxreport", "1");
            break;
        case 2:
            rfx_property_set("vendor.ril.mux.report.case", "2");
            rfx_property_set("vendor.ril.muxreport", "1");
            break;
        default:
            break;
    }
    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

/*
    return value for EPCT:
    PS_CONF_TEST_NONE,
    PS_CONF_TEST_CTA,
    PS_CONF_TEST_FTA,
    PS_CONF_TEST_IOT,
    PS_CONF_TEST_OPERATOR,
    PS_CONF_TEST_FACTORY,
    PS_CONF_TEST_END
*/
void RmmOemRequestHandler::requestGetGcfMode() {

    sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(MIPC_NW_GET_PS_TEST_MODE_REQ, m_slot_id);
    sp<RfxMipcData> mipcCnf = callToMipcMsgSync(mipcMsg);
    if (mipcCnf == NULL || mipcCnf->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "MIPC_NW_GET_PS_TEST_MODE_REQ ERROR");
        return;
    }
    uint8_t mode = mipcCnf->getMipcUint8Val(MIPC_NW_GET_PS_TEST_MODE_CNF_T_MODE, 0);

    rfx_property_set(PROPERTY_GSM_GCF_TEST_MODE, String8::format("%d", mode));
    getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_KEY_GCF_TEST_MODE, mode);

    logD(RFX_LOG_TAG, "MIPC_NW_GET_PS_TEST_MODE_REQ mode %d", mode);
}

void RmmOemRequestHandler::requestMdVersion() {
    sp<RfxAtResponse> pResponse = atSendCommandMultiline(String8::format("AT+EMDVER?"),
            (char *) "+EMDVER:");
    if (pResponse == NULL || pResponse->getError() < 0 || pResponse->getSuccess() == 0) {
        logE(RFX_LOG_TAG, "AT+EMDVER? fail");
        if (RfxRilUtils::getRilRunMode() != RIL_RUN_MODE_MOCK) {
            RFX_ASSERT(0);
        }
        return;
    }

    // notify RfxVersionManager
    RfxVersionManager::getInstance()->initVersion(pResponse->getIntermediates());
}

void RmmOemRequestHandler::requestSetTxPowerStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    int* mode = (int*)(msg->getData()->getData());
    sp<RfxAtResponse> atResponse = atSendCommand(String8::format("AT+TXPWRSTUS=%d", mode[0]));
    // check at cmd result, consider default as success
    if (atResponse == NULL || atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
        logE(RFX_LOG_TAG, "requestSetTxPowerStatus: %d failed", mode[0]);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
    }
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::requestGetPhoneCapability(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    RIL_PhoneCapability *pPhoneCapabilityData = NULL;

    pPhoneCapabilityData = (RIL_PhoneCapability*)calloc(1, sizeof(RIL_PhoneCapability));
    if (pPhoneCapabilityData == NULL) {
        logE(RFX_LOG_TAG, "requestGetPhoneCapability OOM");
        goto error;
    }

    pPhoneCapabilityData->maxActiveData = 1;
    pPhoneCapabilityData->maxActiveInternetData = 1;
    pPhoneCapabilityData->isInternetLingeringSupported = 1;

    for (int i = 0; i < MAX_SIM_COUNT; i++) {
        pPhoneCapabilityData->logicalModemList[i].modemId = i;
    }
    // if sim switch, change the logic id? TBD
    logD(RFX_LOG_TAG, "requestGetPhoneCapability (%d, %d, %d)",
            pPhoneCapabilityData->maxActiveData,
            pPhoneCapabilityData->maxActiveInternetData,
            pPhoneCapabilityData->isInternetLingeringSupported);

    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxPhoneCapabilityData((void *)pPhoneCapabilityData, sizeof(RIL_PhoneCapability)),
            msg);
    responseToTelCore(responseMsg);
    free(pPhoneCapabilityData);
    return;

error:
    free(pPhoneCapabilityData);
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);

}

void RmmOemRequestHandler::requestSetModemConfig(const sp<RfxMclMessage>& msg) {
    int* mode = (int*)(msg->getData()->getData());

    logD(RFX_LOG_TAG, "requestSetModemConfig: %d", *mode);

    if (*mode == 1) {
        // switch to SS, we have to switch ps1 to sim1
        int main_sim = RfxRilUtils::getMajorSim();
        if (main_sim != 1) {
            int networkMode = NETWORK_MODE_WCDMA_PREF;
            if (RatConfig_isLteFddSupported() || RatConfig_isLteTddSupported()) { // support LTE
                networkMode = NETWORK_MODE_GSM_UMTS_LTE;
            }
            sp<RfxAtResponse> pResponse =
                    atSendCommand(String8::format("AT+ES3G = 1,%d", networkMode));
            if (pResponse == NULL || pResponse->getError() != 0 || pResponse->getSuccess() != 1) {
                logE(RFX_LOG_TAG, "requestSetModemConfig failed");
                sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                        RfxVoidData(), msg);
                responseToTelCore(responseMsg);
                return;
            }
            rfx_property_set(PROPERTY_3G_SIM, String8::format("%d", 1).string());
            getNonSlotMclStatusManager()->setIntValue(
                    RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0, false, false);
        }
    }
    String8 value;
    value.appendFormat("%d", *mode);
    mtk_property_set("persist.vendor.radio.multisimslot", value.string());
    //mtk_property_set("persist.radio.reboot_on_modem_change", "true");
    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::requestNvResetConfig(const sp<RfxMclMessage>& msg) {
    int* type = (int*)(msg->getData()->getData());
    logD(RFX_LOG_TAG, "requestNvResetConfig: type = %d", *type);

    // type1: modem reset
    // type3: GUTI reset
    if (*type == 1) {
        int mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);

        // only do power off when it is on
        if (!mdOff) {
            getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, true);

            sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(
                    MIPC_NW_SET_RADIO_STATE_REQ, msg->getSlotId());
            mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE, 0);
            mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_HW_STATE, 0);
            mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_CAUSE, 0x80);
            uint32_t timeout = 10 * 60 * 1000;
            callToMipcMsgSync(mipcMsg, timeout);

            logD(RFX_LOG_TAG, "%s(): set radio off before power off MD.", __FUNCTION__);

            // power off modem
            logD(RFX_LOG_TAG, "requestNvResetConfig SET vendor.ril.ipo.radiooff to 1");
            rfx_property_set("vendor.ril.ipo.radiooff","1");
            for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++){
                getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
            }
            logD(RFX_LOG_TAG, "Flight mode power off modem, trigger CCCI level 2 power off");
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
            RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_ENTER_DEEP_FLIGHT_ENHANCED);
#endif
        }

        // power on modem
        logD(RFX_LOG_TAG, "SET vendor.ril.ipo.radiooff to 0");
        rfx_property_set("vendor.ril.ipo.radiooff","0");
        logD(RFX_LOG_TAG, "SET vendor.gsm.ril.eboot to 1");
        rfx_property_set("vendor.gsm.ril.eboot", "1");
        logD(RFX_LOG_TAG, "Flight mode power on modem, trigger CCCI power on modem (new versio)");
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
        RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_LEAVE_DEEP_FLIGHT_ENHANCED);
#endif

    } else if (*type == 3) {
        logD(RFX_LOG_TAG, "requestNvResetConfig: GUTI reset");
        sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(
                MIPC_NW_SET_RADIO_STATE_REQ, msg->getSlotId());
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE, 0);
        // add cause 0x71 for send "AT+ECMSID=1"
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_CAUSE, 0x71);
        uint32_t timeout = 10 * 60 * 1000;
        callToMipcMsgSync(mipcMsg, timeout);

        for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++){
            RIL_RadioState radioState = (RIL_RadioState) getMclStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_RADIO_STATE, 0);

            if (RADIO_STATE_ON == radioState) {
                // send power off urc and power on again
                sp<RfxMipcData> mipcRet = NULL;
                mipc_result_const_enum ret = mipc_result_const_NONE;

                getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);

                logD(RFX_LOG_TAG, "requestNvResetConfig: power on slot%d", i);
                sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(
                        MIPC_NW_SET_RADIO_STATE_REQ, i);
                mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE, 1);

                uint32_t timeout = 10 * 60 * 1000;
                mipcRet = callToMipcMsgSync(mipcMsg, timeout);
                ret = (mipc_result_const_enum) mipcRet->getResult();

                logD(RFX_LOG_TAG, "requestNvResetConfig: ret=%d", ret);
                while (ret != MIPC_RESULT_SUCCESS) {
                    usleep(50*1000);
                    mipcRet = callToMipcMsgSync(mipcMsg, timeout);
                    ret = (mipc_result_const_enum) mipcRet->getResult();
                    logD(RFX_LOG_TAG, "requestNvResetConfig: ret=%d", ret);
                }
                getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_ON);
            }
        }
    }

    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::routeCert(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    sp<RfxMclMessage> responseMsg;
    int err, custId;
    char* rnd;

    RIL_CertMsg* certMsg = (RIL_CertMsg *) msg->getData()->getData();
    if (certMsg == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    int uid = certMsg->uid;
    logD(RFX_LOG_TAG, "routeCert uid: %d, certLenght: %d, msgLength: %d",
            uid, certMsg->certLength, certMsg->msgLength);
    char *certHex = (char *) calloc(certMsg->certLength*2+1, sizeof(char));
    if (certHex == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    byteToString(certMsg->cert, certMsg->certLength, &certHex);
    char *msgHex = (char *)  calloc(certMsg->msgLength*2+1, sizeof(char));
    if (msgHex == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        free(certHex);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    byteToString(certMsg->msg, certMsg->msgLength, &msgHex);

    RIL_CertResponse* certResponse = (RIL_CertResponse*) calloc(1, sizeof(RIL_CertResponse));
    if (certResponse == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        free(certHex);
        free(msgHex);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }

    // phase1
    char* command = getCertPhase1Command(certHex);
    if (command == NULL) {
        logE(RFX_LOG_TAG, "%s: can't get command", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED, RfxVoidData(),
                msg);
        responseToTelCore(responseMsg);
        free(certHex);
        free(msgHex);
        free(certResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    pResponse = atSendCommandSingleline(command, getAuthPrefixCommand());
    free(certHex);
    free(command);
    if (pResponse->isATCmdRspErr()) {
        int cmdError = (int) pResponse->atGetCmeError();
        logD(RFX_LOG_TAG, "routeCert ERROR: %d", cmdError);
        certResponse->error = cmdError;
        responseMsg = RfxMclMessage::obtainResponse((RIL_Errno) cmdError,
                RfxCertResponseData(certResponse, sizeof(RIL_CertResponse)), msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        free(msgHex);
        removeContext(uid);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    RfxAtLine *line = pResponse->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "routeCert parse response error: %d", err);
        certResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxCertResponseData(certResponse, sizeof(RIL_CertResponse)), msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        free(msgHex);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    custId = line->atTokNextint(&err);
    if (err < 0){
        logD(RFX_LOG_TAG, "routeCert get int error: %d", err);
        certResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxCertResponseData(certResponse, sizeof(RIL_CertResponse)), msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        free(msgHex);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    logD(RFX_LOG_TAG, "routeCert custId: %d", custId);

    // phase2
    command = getCertPhase2Command(custId, msgHex);
    if (command == NULL) {
        logE(RFX_LOG_TAG, "%s: can't get command", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED, RfxVoidData(),
                msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        free(msgHex);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    pResponse = atSendCommandSingleline(command, getAuthPrefixCommand());
    free(msgHex);
    free(command);
    if (pResponse->isATCmdRspErr()) {
        int cmdError = (int) pResponse->atGetCmeError();
        logD(RFX_LOG_TAG, "routeCert ERROR: %d", cmdError);
        certResponse->error = cmdError;
        responseMsg = RfxMclMessage::obtainResponse((RIL_Errno) cmdError,
            RfxCertResponseData(certResponse, sizeof(RIL_CertResponse)), msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        removeContext(uid);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    line = pResponse->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "routeCert parse response error: %d", err);
        certResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxCertResponseData(certResponse, sizeof(RIL_CertResponse)), msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    rnd = line->atTokNextstr(&err);
    if (err < 0){
        logD(RFX_LOG_TAG, "routeCert get str error: %d", err);
        certResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxCertResponseData(certResponse, sizeof(RIL_CertResponse)), msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    updateRnd(rnd);
    logD(RFX_LOG_TAG, "routeCert rnd: %s, length = %d", rnd, (int) strlen(rnd));

    certResponse->error = (int) RIL_E_SUCCESS;
    certResponse->rndLength = strlen(rnd)/2;
    certResponse->rnd = (char *) calloc(certResponse->rndLength+1, sizeof(char));
    if (certResponse->rnd == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        free(certResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    stringToByte(rnd, strlen(rnd), &certResponse->rnd);
    certResponse->custId = custId;
    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxCertResponseData(certResponse, sizeof(RIL_CertResponse)), msg);
    responseToTelCore(responseMsg);
    free(certResponse->rnd);
    free(certResponse);
    rfx_property_set(PROPERTY_LOG_CERTIFICATE, "1");
}

void RmmOemRequestHandler::routeAuth(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    RIL_AuthMsg* authMsg = (RIL_AuthMsg *) msg->getData()->getData();
    if (authMsg == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    int uid = authMsg->uid;
    logD(RFX_LOG_TAG, "routeAuth uid: %d, auth = %s", uid, authMsg->msg);
    sp<RfxAtResponse> pResponse;
    int custId, capMask;
    char* devId;
    char* key;
    int err;
    RIL_AuthResponse* authResponse = (RIL_AuthResponse*) calloc(1, sizeof(RIL_AuthResponse));
    if (authResponse == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }

    char *authHex = (char *) calloc(authMsg->msgLength*2+1, sizeof(char));
    if (authHex == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    byteToString(authMsg->msg, authMsg->msgLength, &authHex);
    char* command = getAuthPhaseCommand(authHex);
    if (command == NULL) {
        logE(RFX_LOG_TAG, "%s: can't get command", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED, RfxVoidData(),
                msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        free(authHex);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    pResponse = atSendCommandSingleline(command, getAuthPrefixCommand());
    free(authHex);
    free(command);
    command = NULL;
    if (pResponse->isATCmdRspErr()) {
        int cmdError = (int) pResponse->atGetCmeError();
        logD(RFX_LOG_TAG, "routeAuth ERROR: %d", cmdError);
        authResponse->error = cmdError;
        responseMsg = RfxMclMessage::obtainResponse((RIL_Errno) cmdError,
            RfxAuthResponseData(authResponse, sizeof(RIL_AuthResponse)), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        removeContext(uid);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    RfxAtLine *line = pResponse->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "routeAuth parse response error: %d", err);
        authResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxAuthResponseData(authResponse, sizeof(RIL_AuthResponse)), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    custId = line->atTokNextint(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "routeAuth parse response error: %d", err);
        authResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxAuthResponseData(authResponse, sizeof(RIL_AuthResponse)), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    logD(RFX_LOG_TAG, "routeAuth custId: %d", custId);

    key = line->atTokNextstr(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "routeAuth parse response error: %d", err);
        authResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxAuthResponseData(authResponse, sizeof(RIL_AuthResponse)), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    // logD(RFX_LOG_TAG, "routeAuth key: %s, length = %d", key, (int) strlen(key));

    // store context
    updateContext(uid, custId, key);

    // debug
    //printContext();

    capMask = line->atTokNextint(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "routeAuth parse response error: %d", err);
        authResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxAuthResponseData(authResponse, sizeof(RIL_AuthResponse)), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    // logD(RFX_LOG_TAG, "routeAuth capMask: %d", capMask);

    devId = line->atTokNextstr(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "routeAuth parse response error: %d", err);
        authResponse->error = (int) RIL_E_GENERIC_FAILURE;
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxAuthResponseData(authResponse, sizeof(RIL_AuthResponse)), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    // logD(RFX_LOG_TAG, "routeAuth devId: %s, length = %d", devId, (int) strlen(devId));

    authResponse->error = (int) RIL_E_SUCCESS;
    authResponse->devIdLength = strlen(devId)/2;
    authResponse->devId = (char *) calloc(authResponse->devIdLength+1, sizeof(char));
    if (authResponse->devId == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        free(authResponse);
        rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
        return;
    }
    stringToByte(devId, strlen(devId), &authResponse->devId);
    authResponse->capMask = capMask;
    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxAuthResponseData(authResponse, sizeof(RIL_AuthResponse)), msg);
    responseToTelCore(responseMsg);
    free(authResponse);
    rfx_property_set(PROPERTY_LOG_CERTIFICATE, "1");
}

void RmmOemRequestHandler::enableCapability(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    int errNo = (int) RIL_E_SUCCESS;
    sp<RfxMclMessage> responseMsg;
    RIL_Capability* certMsg = (RIL_Capability *) msg->getData()->getData();
    if (certMsg == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        rfx_property_set(PROPERTY_LOG_ENABLE_CAPABILITY, "0");
        return;
    }
    char * id = certMsg->id;
    int uid = certMsg->uid;
    int toActive = certMsg->toActive;

    char* command = getCapabilityCommand(uid, id, toActive);
    if (command != NULL) {
        pResponse = atSendCommand(command);
        free(command);
        if (pResponse->getSuccess() == 0) {
            errNo = (int) pResponse->atGetCmeError();
            logD(RFX_LOG_TAG, "enableCapability ERROR: %d", errNo);
        }
        responseMsg = RfxMclMessage::obtainResponse((RIL_Errno) errNo,
            RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        if (RIL_E_SUCCESS == errNo) {
            rfx_property_set(PROPERTY_LOG_ENABLE_CAPABILITY, "1");
        } else {
            rfx_property_set(PROPERTY_LOG_ENABLE_CAPABILITY, "0");
        }
    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        rfx_property_set(PROPERTY_LOG_ENABLE_CAPABILITY, "0");
    }
}

void RmmOemRequestHandler::abortCert(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    int errNo = (int) RIL_E_SUCCESS;
    sp<RfxMclMessage> responseMsg;
    int uid = ((int *) msg->getData()->getData())[0];
    char* command = getAbortCommand(uid);
    if (command != NULL) {
        pResponse = atSendCommand(command);
        free(command);
        if (pResponse->getSuccess() == 0) {
            errNo = (int) pResponse->atGetCmeError();
            logD(RFX_LOG_TAG, "abortCert ERROR: %d", errNo);
        } else {
            removeContext(uid);
        }
        responseMsg = RfxMclMessage::obtainResponse((RIL_Errno) errNo,
            RfxVoidData(), msg);
        responseToTelCore(responseMsg);
    } else {
        logD(RFX_LOG_TAG, "%s can't get command", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED,
            RfxVoidData(), msg);
        responseToTelCore(responseMsg);
    }
}

void RmmOemRequestHandler::requestSetHsrMode(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    int errNo = (int) RIL_E_SUCCESS;
    int cmdError = CME_SUCCESS;
    sp<RfxMclMessage> responseMsg;
    int mode = ((int *) msg->getData()->getData())[0];
    char hsrMode[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.hsr.mode", hsrMode, "0");
    hsrMode[RFX_PROPERTY_VALUE_MAX - 1] = '\0';
    logI(RFX_LOG_TAG, "requestSetHsrMode: %d, %s", mode, hsrMode);
    /* 0 for enable hsr and network block mode
       1 for disable hsr and network block mode
       2 only for enable hsr mode
       3 only for enable network block mode
    */
    if (strcmp(hsrMode, "0") != 0 && strcmp(hsrMode, "2") != 0 && strcmp(hsrMode, "3") != 0) {
        logE(RFX_LOG_TAG, "requestSetHsrMode ERROR hsrMode: %s", hsrMode);
        errNo = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    if (mode == 0) {
        if (strcmp(hsrMode, "0") == 0 || strcmp(hsrMode, "2") == 0) {
            pResponse = atSendCommand("AT+EGCMD=343,1,\"00\"");
            if (pResponse->isATCmdRspErr()) {
                cmdError = (int) pResponse->atGetCmeError();
                logE(RFX_LOG_TAG, "requestSetHsrMode 343 ERROR: %d,%d", cmdError, mode);
                errNo = RIL_E_GENERIC_FAILURE;
            }
        }

        if (strcmp(hsrMode, "0") == 0 || strcmp(hsrMode, "3") == 0) {
            pResponse = atSendCommand("AT+EGCMD=450,1,\"00\"");
            if (pResponse->isATCmdRspErr()) {
                cmdError = (int) pResponse->atGetCmeError();
                logE(RFX_LOG_TAG, "requestSetHsrMode 450 ERROR: %d,%d", cmdError, mode);
                errNo = RIL_E_GENERIC_FAILURE;
            }

            pResponse = atSendCommand("AT+EGCMD=470,1,\"00\"");
            if (pResponse->isATCmdRspErr()) {
                cmdError = (int) pResponse->atGetCmeError();
                logE(RFX_LOG_TAG, "requestSetHsrMode 470 ERROR: %d,%d", cmdError, mode);
                errNo = RIL_E_GENERIC_FAILURE;
            }
        }
    } else if (mode == 1) {
        if (strcmp(hsrMode, "0") == 0 || strcmp(hsrMode, "2") == 0) {
            pResponse = atSendCommand("AT+EGCMD=343,1,\"01\"");
            if (pResponse->isATCmdRspErr()) {
                cmdError = (int) pResponse->atGetCmeError();
                logE(RFX_LOG_TAG, "requestSetHsrMode 343 ERROR: %d,%d", cmdError, mode);
                errNo = RIL_E_GENERIC_FAILURE;
            }
        }

        if (strcmp(hsrMode, "0") == 0 || strcmp(hsrMode, "3") == 0) {
            pResponse = atSendCommand("AT+EGCMD=450,1,\"01\"");
            if (pResponse->isATCmdRspErr()) {
                cmdError = (int) pResponse->atGetCmeError();
                logE(RFX_LOG_TAG, "requestSetHsrMode 450 ERROR: %d,%d", cmdError, mode);
                errNo = RIL_E_GENERIC_FAILURE;
            }

            pResponse = atSendCommand("AT+EGCMD=470,1,\"01\"");
            if (pResponse->isATCmdRspErr()) {
                cmdError = (int) pResponse->atGetCmeError();
                logE(RFX_LOG_TAG, "requestSetHsrMode 470 ERROR: %d,%d", cmdError, mode);
                errNo = RIL_E_GENERIC_FAILURE;
            }
        }
    }
error:
    responseMsg = RfxMclMessage::obtainResponse((RIL_Errno) errNo, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::queryCapability(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    sp<RfxMclMessage> responseMsg;
    int err;
    int isEnabled = 0;
    RIL_QueryCap* info = (RIL_QueryCap *) msg->getData()->getData();
    if (info == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY,
            RfxIntsData(&isEnabled, 1), msg);
        responseToTelCore(responseMsg);
        return;
    }

    int id = info->id;
    char* name = info->name;

    char* command = getQueryCapabilityCommand(id, name);
    if (command == NULL) {
        logD(RFX_LOG_TAG, "%s can't get command", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    pResponse = atSendCommandSingleline(command, getQueryPrefixCommand());
    free(command);
    if (pResponse->isATCmdRspErr()) {
        int cmdError = (int) pResponse->atGetCmeError();
        logD(RFX_LOG_TAG, "queryCapability ERROR: %d", cmdError);
        responseMsg = RfxMclMessage::obtainResponse((RIL_Errno) cmdError,
            RfxIntsData(&isEnabled, 1), msg);
        responseToTelCore(responseMsg);
        return;
    }
    RfxAtLine *line = pResponse->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "queryCapability parse response error: %d", err);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxIntsData(&isEnabled, 1), msg);
        responseToTelCore(responseMsg);
        return;
    }
    isEnabled = line->atTokNextint(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "queryCapability parse response error: %d", err);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
            RfxIntsData(&isEnabled, 1), msg);
        responseToTelCore(responseMsg);
        return;
    }
    logD(RFX_LOG_TAG, "queryCapability isEnabled: %d", isEnabled);
    responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxIntsData(&isEnabled, 1), msg);
    responseToTelCore(responseMsg);
}

char* RmmOemRequestHandler::getCertPhase1Command(const char* cert) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)(const char *);
    func = (char *(*)(const char *))
            dlsym(sDlHandler, "getCertPhase1Command");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func(cert);
}

char* RmmOemRequestHandler::getCertPhase2Command(int custId, const char* msg) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)(int, const char*);
    func = (char *(*)(int, const char*))
            dlsym(sDlHandler, "getCertPhase2Command");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func(custId, msg);
}

char* RmmOemRequestHandler::getAuthPhaseCommand(const char* msg) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)(const char *);
    func = (char *(*)(const char *))
            dlsym(sDlHandler, "getAuthPhaseCommand");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func(msg);
}

char* RmmOemRequestHandler::getCapabilityCommand(int uid, char* featureName, int timer) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)(int, char*, int);
    func = (char *(*)(int, char*, int))
            dlsym(sDlHandler, "getCapabilityCommand");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func(uid, featureName, timer);
}

char* RmmOemRequestHandler::getQueryCapabilityCommand(int uid, char* featureName) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)(int, char*);
    func = (char *(*)(int, char*))
            dlsym(sDlHandler, "getQueryCapabilityCommand");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func(uid, featureName);
}

char* RmmOemRequestHandler::getAbortCommand(int uid) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)(int);
    func = (char *(*)(int))
            dlsym(sDlHandler, "getAbortCommand");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func(uid);
}

char* RmmOemRequestHandler::getAuthPrefixCommand() {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)();
    func = (char *(*)())
            dlsym(sDlHandler, "getAuthPrefixCommand");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func();
}

char* RmmOemRequestHandler::getQueryPrefixCommand() {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return NULL;
        }
    }
    dlerror();

    char *(*func)();
    func = (char *(*)())
            dlsym(sDlHandler, "getQueryPrefixCommand");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return NULL;
    }
    return func();
}

void RmmOemRequestHandler::updateContext(int id, int custId, char* key) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return;
        }
    }
    dlerror();

    void (*func)(int, int, char*);
    func = (void(*)(int, int, char*))
            dlsym(sDlHandler, "updateContext");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return;
    }
    return func(id, custId, key);
}

void RmmOemRequestHandler::removeContext(int id) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return;
        }
    }
    dlerror();

    void (*func)(int);
    func = (void(*)(int))
            dlsym(sDlHandler, "removeContext");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return;
    }
    return func(id);
}

static const char* HEX_LOOKUP = "0123456789abcdef";
void RmmOemRequestHandler::byteToString(char* byte, int size, char** output) {
    char *tmp = *output;
    for (int i=0; i<size; i++) {
        *tmp++ = HEX_LOOKUP[byte[i] >> 4];
        *tmp++ = HEX_LOOKUP[byte[i] & 0x0F];
    }
    *tmp = '\0';
}

void RmmOemRequestHandler::stringToByte(char* hexstr, int size, char** output) {
    if (size % 2 != 0) {
        return;
    }
    char *res = *output;
    int finalSize = size / 2;
    for (int i=0, j=0; j<finalSize; i+=2, j++)
        res[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i+1] % 32 + 9) % 25;
    res[finalSize] = '\0';
}

void RmmOemRequestHandler::updateRnd(char *rnd) {
    if (sDlHandler == NULL) {
        sDlHandler = dlopen(UTILS_PATH, RTLD_NOW);
        if (sDlHandler == NULL) {
            logE(RFX_LOG_TAG, "%s, dlopen failed: %s", __FUNCTION__, dlerror());
            return;
        }
    }
    dlerror();

    void (*func)(char *);
    func = (void(*)(char *))
            dlsym(sDlHandler, "updateRnd");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        logE(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s",
                __FUNCTION__, UTILS_PATH, dlsym_error);
        return;
    }
    return func(rnd);
}

bool RmmOemRequestHandler::sendOemToNwModule(char *string) {
    if (strStartsWith(string, "AT+E5GOPT=") ||
        strStartsWith(string, "UPDATE_DISPLAY_RULE")) {
        logV(RFX_LOG_TAG, "sendOemToNwModule RFX_MSG_EVENT_OEM_HOOK_TO_NW");
        sendEvent(RFX_MSG_EVENT_OEM_HOOK_TO_NW, RfxStringData(string),
            RIL_CMD_PROXY_3, m_slot_id);
        if (strStartsWith(string, "AT+E5GOPT=")) return false;
        else if (strStartsWith(string, "UPDATE_DISPLAY_RULE")) return true;
    }
    return false;
}

void RmmOemRequestHandler::requestSendSarIndicator(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    const char **strings = (const char **)msg->getData()->getData();
    if (strings == NULL || strings[0] == NULL || strings[1] == NULL) {
        logE(RFX_LOG_TAG, "requestSendSarIndicator invalid arguments.");
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS,
             RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    int cmdType = atoi(strings[0]);
    logE(RFX_LOG_TAG, "requestSendSarIndicator: cmdType:%d", cmdType);

    if (cmdType != 0 && cmdType != 1) {
        logE(RFX_LOG_TAG, "requestSendSarIndicator invalid type:%d", cmdType);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_SEND_SAR_IND_REQ, m_slot_id);
    if (cmdType == 0) {
        initMipc->addMipcTlvUint8(MIPC_SYS_SEND_SAR_IND_REQ_T_CMD_TYPE, 0);
    } else if (cmdType == 1) {
        initMipc->addMipcTlvUint8(MIPC_SYS_SEND_SAR_IND_REQ_T_CMD_TYPE, 1);
    }

    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
    // check at cmd result, consider default as success
    if (cnfMipc != NULL && cnfMipc->getResult() == MIPC_RESULT_SUCCESS) {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
    } else {
        logE(RFX_LOG_TAG, "requestSendSarIndicator fail, cmdType %d", cmdType);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
    }
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::enableMdProtocol() {
    uint8_t sim_count = (uint8_t)RfxRilUtils::rfxGetSimCount();

    if (sim_count < 1 || sim_count > MAX_SIM_COUNT) {
        logE(RFX_LOG_TAG, "enableMdProtocol SIM count invalid:%d", sim_count);
        return;
    }
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_MULTI_SIM_CONFIG_REQ,
            m_slot_id);
    initMipc->addMipcTlvUint8(MIPC_SYS_MULTI_SIM_CONFIG_REQ_T_MODE, sim_count);
    callToMipcMsgSync(initMipc);
}

void RmmOemRequestHandler::updateSupportDSBP() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get((char *) "persist.vendor.radio.mtk_dsbp_support", prop, "0");

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_SET_DSBP_REQ, m_slot_id);
    initMipc->addMipcTlvUint8(MIPC_SYS_SET_DSBP_REQ_T_MODE, (uint8_t)atoi(prop));
    callToMipcMsgSync(initMipc);
}

void RmmOemRequestHandler::handleSetTxPowerRequest(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int power = pInt[0];
    sp<RfxMclMessage> responseMsg;
    logV(RFX_LOG_TAG, "handleSetTxPowerRequest power=%d", power);

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, msg->getSlotId());
    initMipc->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    initMipc->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, String8::format("%s", "max_pwr_limit"));
    initMipc->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA, String8::format("%d", power));
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
    // check at cmd result, consider default as success
    if (cnfMipc != NULL && cnfMipc->getResult() == MIPC_RESULT_SUCCESS) {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
    } else {
        logE(RFX_LOG_TAG, "requestSendSarIndicator fail");
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
    }
    responseToTelCore(responseMsg);
}

void RmmOemRequestHandler::syncAppEventStatusRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcMsg;
    int* pReqInt = (int*)msg->getData()->getData();
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] event=%d, status:%d", m_slot_id, __FUNCTION__, pReqInt[0], pReqInt[1]);
    if (pReqInt[0] == APP_EVENT_VOIP_STATUS) {
        static int cacheStatus = -1;
        int newStatus = (pReqInt[1] == APP_EVENT_VOIP_STATUS_ONGOING) ? APP_EVENT_VOIP_STATUS_ONGOING : 0;
        if (newStatus != cacheStatus) {
            cacheStatus = newStatus;
            mipcMsg = RfxMipcData::obtainMipcData(MIPC_SYS_SEND_APP_EVENT_REQ, m_slot_id);
            mipc_sys_send_app_event_req_add_voip_status(mipcMsg->getData(), (uint8_t)newStatus);
            callToMipcMsgSync(mipcMsg);
        }
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(response);
}
