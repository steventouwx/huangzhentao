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

#include "RmcOemRequestHandler.h"
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

#define RFX_LOG_TAG "RmcOemHandler"

#define PROPERTY_GSM_GCF_TEST_MODE  "vendor.gsm.gcf.testmode"
#define UTILS_PATH "libcapctrl.so"
#define PROPERTY_LOG_CERTIFICATE "vendor.ril.certificate"
#define PROPERTY_LOG_ENABLE_CAPABILITY "vendor.ril.enablecapability"
#define NETWORK_MODE_WCDMA_PREF 3
#define NETWORK_MODE_GSM_UMTS_LTE 7

void* RmcOemRequestHandler::sDlHandler = NULL;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcOemRequestHandler, RIL_CMD_PROXY_3);
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
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData,
        RFX_MSG_REQUEST_QUERY_MODEM_THERMAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_SET_TRM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData,
        RFX_MSG_REQUEST_GET_IMEI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxPhoneCapabilityData,
        RFX_MSG_REQUEST_GET_PHONE_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_ENABLE_DSDA_INDICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData,
        RFX_MSG_REQUEST_GET_DSDA_STATUS);
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
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData,
        RFX_MSG_EVENT_OEM_RAW_URC);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData,
        RFX_MSG_REQUEST_DEVICE_IMEI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_SET_TX_POWER);

RmcOemRequestHandler::RmcOemRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int request[] = {
        RFX_MSG_REQUEST_DEVICE_IMEI,
        RFX_MSG_REQUEST_DEVICE_IDENTITY,
        RFX_MSG_REQUEST_OEM_HOOK_RAW,
        RFX_MSG_REQUEST_OEM_HOOK_STRINGS,
        RFX_MSG_REQUEST_GET_ACTIVITY_INFO,
        RFX_MSG_REQUEST_BASEBAND_VERSION,
        RFX_MSG_REQUEST_QUERY_MODEM_THERMAL,
        RFX_MSG_REQUEST_SET_TRM,
        RFX_MSG_REQUEST_GET_IMEI,
        RFX_MSG_REQUEST_SET_TX_POWER_STATUS,
        RFX_MSG_REQUEST_GET_PHONE_CAPABILITY,
        RFX_MSG_REQUEST_ENABLE_DSDA_INDICATION,
        RFX_MSG_REQUEST_GET_DSDA_STATUS,
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
    };
    const int event[] = {
        RFX_MSG_EVENT_OEM_RAW_URC
    };
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    registerToHandleEvent(event, sizeof(event)/sizeof(int));

    if (RFX_SLOT_ID_0 == slot_id) {
        requestMdVersion();
    }
    /*  Enable getting CFU info +ECFU and speech info +ESPEECH*/
    int einfo_value;
    einfo_value = 50;//default value.

    /*  Enable getting CFU info +ECFU and speech info +ESPEECH and modem warning +EWARNING(0x100) */
    char modemWarningProperty[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.modem.warning", modemWarningProperty, "0");
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
    requestGetImei();
    requestGetImeisv();
    int mainSlotId = RfxRilUtils::getMajorSim() - 1;
    if (mainSlotId == slot_id) {
        requestGetGcfMode();
        bootupGetBasebandProject();
    }
    rfx_property_set(PROPERTY_LOG_CERTIFICATE, "0");
    rfx_property_set(PROPERTY_LOG_ENABLE_CAPABILITY, "0");
}

RmcOemRequestHandler::~RmcOemRequestHandler() {
}

void RmcOemRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
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
        case RFX_MSG_REQUEST_GET_IMEI:
            requestGetImei(msg);
            break;
        case RFX_MSG_REQUEST_GET_ACTIVITY_INFO:
            requestGetActivityInfo(msg);
            break;
        case RFX_MSG_REQUEST_BASEBAND_VERSION:
            requestBasebandVersion(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_MODEM_THERMAL:
            requestQueryThermal(msg);
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
        case RFX_MSG_REQUEST_ENABLE_DSDA_INDICATION:
            requestEnableDsdaIndication(msg);
            break;
        case RFX_MSG_REQUEST_GET_DSDA_STATUS:
            requestGetDsdaStatus(msg);
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
            requestSetTxPower(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcOemRequestHandler::onHandleTimer() {
    // do something
}

void RmcOemRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch (id) {
        case RFX_MSG_EVENT_OEM_RAW_URC:
            handleRawUrc(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "should not be here");
            break;
    }
}

void RmcOemRequestHandler::requestDeviceImei(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    sp<RfxAtResponse> p_response;
    char **responses = (char**)calloc(1, sizeof(char*) * 3);

    if (responses == NULL) {
        logE(RFX_LOG_TAG, "responses calloc fail");
        goto error;
    }

    // the type is primary or secondary
    responses[0] = (char*)alloca(sizeof(char) * (1 + 1));
    if (m_slot_id == 0) {
        responses[0][0] = '1';
    } else {
        responses[0][0] = '2';
    }
    responses[0][1] = '\0';

    // Query IMEI
    p_response = atSendCommandNumeric("AT+CGSN");

    if (p_response == NULL
            || p_response->getError() != 0
            || p_response->getSuccess() == 0
            || p_response->getIntermediates() == NULL) {
        goto error;
    }

    responses[1] = p_response->getIntermediates()->getLine();

    // Query ImeiSv
    if (mImeiSv.isEmpty()) {
        requestGetImeisv();
    }

    if (!mImeiSv.isEmpty()) {
        responses[2] = (char *)mImeiSv.string();
    } else {
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

void RmcOemRequestHandler::requestDeviceIdentity(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxAtResponse> p_responseGSN;
    sp<RfxAtResponse> p_responseMEID;
    sp<RfxMclMessage> response;
    int err = 0;
    RfxAtLine* line = NULL;
    char *tmp = NULL;
    RIL_IDENTITY identity;
    memset(&identity, 0, sizeof(identity));

    // Query IMEI
    p_response = atSendCommandNumeric("AT+CGSN");

    if (p_response == NULL
            || p_response->getError() != 0
            || p_response->getSuccess() == 0
            || p_response->getIntermediates() == NULL) {
        goto error;
    }

    identity.imei = p_response->getIntermediates()->getLine();

    // Query ImeiSv
    if (mImeiSv.isEmpty()) {
        requestGetImeisv();
    }
    if (!mImeiSv.isEmpty()) {
        identity.imeisv = (char *)mImeiSv.string();
    } else {
        goto error;
    }

    if (RatConfig_isC2kSupported()) {
        // Query ESN
        p_responseGSN = atSendCommandMultiline("AT+GSN", "+GSN:");

        if (p_responseGSN == NULL
                || p_responseGSN->getError() != 0
                || p_responseGSN->getSuccess() == 0
                || p_responseGSN->getIntermediates() == NULL) {
            goto error;
        }

        line = p_responseGSN->getIntermediates();

        line->atTokStart(&err);
        if (err < 0) goto error;

        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        if (strstr(tmp, "0x") != NULL) {
            identity.esnHex = tmp + 2;
            //logD(RFX_LOG_TAG, "identity.esnHex = %s", identity.esnHex);
        } else {
            identity.esnDec = tmp;
            //logD(RFX_LOG_TAG, "identity.esnDec = %s", identity.esnDec);
        }

        line = p_responseGSN->getIntermediates()->getNext();
        if (line != NULL) {
            line->atTokStart(&err);
            if (err < 0) goto error;

            tmp = line->atTokNextstr(&err);
            if (err < 0) goto error;

            if (strstr(tmp, "0x") != NULL) {
                identity.esnHex = tmp + 2;
                //logD(RFX_LOG_TAG, "identity.esnHex = %s", identity.esnHex);
            } else {
                identity.esnDec = tmp;
                //logD(RFX_LOG_TAG, "identity.esnDec = %s", identity.esnDec);
            }
        }

        //  Query MEID
        p_responseMEID = atSendCommandMultiline("AT^MEID", "^MEID:");

        if (p_responseMEID == NULL
                || p_responseMEID->getError() != 0
                || p_responseMEID->getSuccess() == 0
                || p_responseMEID->getIntermediates() == NULL) {
            goto error;
        }

        line = p_responseMEID->getIntermediates();

        line->atTokStart(&err);
        if (err < 0) goto error;

        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        if (strstr(tmp, "0x") != NULL) {
            identity.meidHex = tmp + 2;
            //logD(RFX_LOG_TAG, "identity.meidHex = %s", identity.meidHex);
        } else {
            identity.meidDec = tmp;
            //logD(RFX_LOG_TAG, "identity.meidDec = %s", identity.meidDec);
        }

        line = p_responseMEID->getIntermediates()->getNext();
        if (line != NULL) {
            line->atTokStart(&err);
            if (err < 0) goto error;

            tmp = line->atTokNextstr(&err);
            if (err < 0) goto error;

            if (strstr(tmp, "0x") != NULL) {
                identity.meidHex = tmp + 2;
                //logD(RFX_LOG_TAG, "identity.meidHex = %s", identity.meidHex);
            } else {
                identity.meidDec = tmp;
                //logD(RFX_LOG_TAG, "identity.meidDec = %s", identity.meidDec);
            }
        }

        if (identity.meidHex != NULL) {
            for (size_t i = 0; i < strlen(identity.meidHex); i++) {
                if (identity.meidHex[i] >= 'a' && identity.meidHex[i] <= 'z') {
                    identity.meidHex[i] -= 32;
                }
            }
        }

        // Query UIMID
        // no need query, framework does not need this value
        identity.uimid = (char*)"0x0";
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData(&identity, 4 * sizeof(char*)), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(RFX_LOG_TAG, "requestDeviceIdentity error");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_EMPTY_RECORD,
            RfxStringsData(), msg, false);
    responseToTelCore(response);
}

void RmcOemRequestHandler::requestOemHookRaw(const sp<RfxMclMessage>& msg) {
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
    logD(RFX_LOG_TAG, "success = %d, finalResponse", pResponse->getSuccess(),
            pResponse->getFinalResponse()->getLine());

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
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxRawData(line, strlen(line)), msg);
    }
    responseToTelCore(responseMsg);

    return;
}

void RmcOemRequestHandler::requestOemHookStrings(const sp<RfxMclMessage>& msg) {
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

void RmcOemRequestHandler::requestGetImei() {
    sp<RfxMclMessage> responseMsg;

    sp<RfxAtResponse> pResponse = atSendCommandNumeric("AT+CGSN");
    if (!pResponse->isAtResponseFail()) {
        mImei = String8(pResponse->getIntermediates()->getLine());
        //logD(RFX_LOG_TAG, "imei: %s", mImei.string());
    } else {
        logE(RFX_LOG_TAG, "requestGetImei send at command Fail");
    }
}

void RmcOemRequestHandler::requestGetImei(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;

    if (mImei.isEmpty()) {
        sp<RfxAtResponse> pResponse = atSendCommandNumeric("AT+CGSN");
        if (!pResponse->isAtResponseFail()) {
            mImei = String8(pResponse->getIntermediates()->getLine());
            //logD(RFX_LOG_TAG, "imei: %s", mImei.string());
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                    RfxStringData((void *)mImei.string(), strlen(mImei.string())), msg);
        } else {
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                    RfxVoidData(), msg);
            logE(RFX_LOG_TAG, "requestGetImei send at command Fail");
        }

    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxStringData((void *)mImei.string(), strlen(mImei.string())), msg);
    }
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::requestGetImeisv() {
    int err = 0;
    sp<RfxAtResponse> pResponse = atSendCommandSingleline("AT+EGMR=0,9", "+EGMR:");

    if (!pResponse->isAtResponseFail()) {
        char* sv = NULL;
        RfxAtLine* line = pResponse->getIntermediates();
        line->atTokStart(&err);
        if(err >= 0) {
            sv = line->atTokNextstr(&err);
            if(err >= 0) {
                mImeiSv = String8(sv);
                //logD(RFX_LOG_TAG, "imeisv: %s", mImeiSv.string());
            } else {
                logE(RFX_LOG_TAG, "requestGetImeisv atTokNextstr fail");
            }
        } else {
            logE(RFX_LOG_TAG, "requestGetImeisv atTokStart fail");
        }
    } else {
        logE(RFX_LOG_TAG, "requestGetImeisv send AT command fail");
    }
}

void RmcOemRequestHandler::requestGetActivityInfo(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    int err;
    RIL_ActivityStatsInfo *activityStatsInfo; // RIL_NUM_TX_POWER_LEVELS 5
    int num_tx_levels = 0;
    int op_code = 0;

    sp<RfxAtResponse> pResponse = atSendCommandSingleline("AT+ERFTX=11", "+ERFTX:");

    if (pResponse == NULL
            || pResponse->getError() != 0
            || pResponse->getSuccess() == 0
            || pResponse->getIntermediates() == NULL) {
        logE(RFX_LOG_TAG, "requestGetActivityInfo error");
        responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    activityStatsInfo = (RIL_ActivityStatsInfo*)calloc(1, sizeof(RIL_ActivityStatsInfo));
    if (activityStatsInfo == NULL) {
        logE(RFX_LOG_TAG, "OOM");
        return;
    }
    RfxAtLine* line = pResponse->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;
    op_code = line->atTokNextint(&err);
    if (err < 0 || op_code != 11) goto error;
    num_tx_levels = line->atTokNextint(&err);
    if (err < 0) goto error;
    if (num_tx_levels > RIL_NUM_TX_POWER_LEVELS) {
        logD(RFX_LOG_TAG, "requestGetActivityInfo TX level invalid (%d)", num_tx_levels);
        goto error;
    }
    for (int i = 0; i < num_tx_levels; i++) {
        activityStatsInfo->tx_mode_time_ms[i] =  line->atTokNextint(&err);
        if (err < 0) goto error;
    }
    activityStatsInfo->rx_mode_time_ms = line->atTokNextint(&err);
    if (err < 0) goto error;
    activityStatsInfo->sleep_mode_time_ms = line->atTokNextint(&err);
    if (err < 0) goto error;
    activityStatsInfo->idle_mode_time_ms = line->atTokNextint(&err);
    if (err < 0) goto error;

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
    logE(RFX_LOG_TAG, "requestGetActivityInfo error");
    free(activityStatsInfo);
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::requestBasebandVersion(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    int err, len;
    char *ver = NULL;
    char *tmp = NULL;
    sp<RfxAtResponse> pResponse = atSendCommandMultiline("AT+CGMR", "+CGMR:");

    if (pResponse == NULL
            || pResponse->getError() != 0
            || pResponse->getSuccess() == 0) {
        logE(RFX_LOG_TAG, "requestBasebandVersion error");
        responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }

    if(pResponse->getIntermediates() != NULL) {
        RfxAtLine* line = pResponse->getIntermediates();
        line->atTokStart(&err);
        if(err < 0) goto error;
        ver = line->atTokNextstr(&err);
        if(err < 0) goto error;
    } else {
        logE(RFX_LOG_TAG,
                "Retry AT+CGMR without expecting +CGMR prefix");
        pResponse = atSendCommandRaw("AT+CGMR");

        if (pResponse == NULL
                || pResponse->getError() != 0
                || pResponse->getSuccess() == 0
                || pResponse->getIntermediates() == NULL) {
            logE(RFX_LOG_TAG, "requestBasebandVersion error");
            responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                    RfxVoidData(), msg);
            responseToTelCore(responseMsg);
            return;
        }
        if(pResponse->getIntermediates() != 0) {
            tmp = pResponse->getIntermediates()->getLine();
            len = strlen(tmp);
            while( len > 0 && isspace(tmp[len-1]) )
                len --;
            tmp[len] = '\0';

            // remove the white space from the beginning
            while( (*tmp) != '\0' &&  isspace(*tmp) )
                tmp++;
            ver = tmp;
        }
    }
    // logE(RFX_LOG_TAG, "ver: %s", ver);
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringData((void *)ver, strlen(ver)), msg);
    responseToTelCore(responseMsg);
    return;

error:
    logE(RFX_LOG_TAG, "requestBasebandVersion error");
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::bootupGetBasebandProject() {
    sp<RfxMclMessage> responseMsg;
    int err;
    char *proj = NULL;
    char *flavor = NULL;
    char *outStr = NULL;
    RfxAtLine *line1 = NULL;
    RfxAtLine *line2 = NULL;
    sp<RfxAtResponse> pResponse = NULL;
    sp<RfxAtResponse> pResponse2 = NULL;
    pResponse = atSendCommandSingleline("AT+EGMR=0,4", "+EGMR:");
    if (pResponse == NULL
            || pResponse->getError() != 0
            || pResponse->getSuccess() == 0
            || pResponse->getIntermediates() == NULL) {
        goto error;
    }
    if(pResponse->getIntermediates() != 0) {
        line1 = pResponse->getIntermediates();
        line1->atTokStart(&err);
        if(err < 0) goto error;
        proj = line1->atTokNextstr(&err);
        if(err < 0) goto error;
    }
    pResponse2 = atSendCommandSingleline("AT+EGMR=0,13", "+EGMR:");
    if (pResponse2 == NULL
            || pResponse2->getError() != 0
            || pResponse2->getSuccess() == 0
            || pResponse2->getIntermediates() == NULL) {
        goto error;
    }
    if(pResponse2->getIntermediates() != 0) {
        line2 = pResponse2->getIntermediates();
        line2->atTokStart(&err);
        if(err < 0) goto error;
        flavor = line2->atTokNextstr(&err);
        if(err < 0) goto error;
    }
    logE(RFX_LOG_TAG, "proj: %s, flavor: %s", proj, flavor);
    asprintf(&outStr, "%s(%s)",proj ,flavor);
    rfx_property_set("vendor.gsm.project.baseband", outStr);
    if (outStr != NULL) {
        free(outStr);
    }
    return;

error:
    logE(RFX_LOG_TAG, "bootupGetBasebandProject error");
}

void RmcOemRequestHandler::requestQueryThermal(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    sp<RfxMclMessage> resMsg;
    int err = 0;

    char *data = (char *) msg->getData()->getData();
    if (data != NULL) {
        data[strlen(data)-1] = 0;
    }
    logI(RFX_LOG_TAG, "requestQueryThermal Enter: %s", data);
    /*
    * thermal service have two action.
    * 1. set threshold at bootup
    *     => thermal service sends command with four parameter. So, AT+ETHERMAL=x,x,x
    * 2. query template of modem
    *     => the second parameter is -1. So, RILD will send "AT+ETHERMAL" to modem
    */
    if(atoi(data) == -1){
        // Enhancement for thermal: Do not query temperature if all radio is off
        int index;
        for (index = 0; index < RFX_SLOT_COUNT; index++) {
            RIL_RadioState radioState = (RIL_RadioState) getMclStatusManager(index)->getIntValue(
                    RFX_STATUS_KEY_RADIO_STATE, 0);
            if (RADIO_STATE_ON == radioState) {
                break;
            }
        }
        if (RFX_SLOT_COUNT == index) {
            logI(RFX_LOG_TAG, "requestQueryThermal: all radio is off, return error");
            resMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                    RfxVoidData(), msg);
        } else {
            pResponse = atSendCommandSingleline((char *) "AT+ETHERMAL", (char *) "+ETHERMAL:");
            if (pResponse->getError() < 0 || pResponse->getSuccess() == 0) {
                logI(RFX_LOG_TAG, "requestQueryThermal error");
                resMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                        RfxVoidData(), msg);
            } else {
                logI(RFX_LOG_TAG, "requestQueryThermal success");
                RfxAtLine *line = pResponse->getIntermediates();
                line->atTokStart(&err);
                if (err == 0){
                    resMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                            RfxStringData(line->getCurrentLine(), strlen(line->getCurrentLine())),
                            msg);
                } else {
                    logI(RFX_LOG_TAG, "requestQueryThermal token start error");
                    resMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                        RfxVoidData(), msg);
                }
            }
        }
    } else {
        pResponse = atSendCommandSingleline(String8::format("AT+ETHERMAL=%s", data).string(),
                (char*) "+ETHERMAL:");
        if (pResponse->getError() < 0 || pResponse->getSuccess() == 0) {
            logI(RFX_LOG_TAG, "requestQueryThermal error");
            resMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                    RfxVoidData(), msg);
        } else {
            logI(RFX_LOG_TAG, "requestQueryThermal success");
            resMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                    RfxVoidData(), msg);
        }
    }
    responseToTelCore(resMsg);
}

void RmcOemRequestHandler::requestSetTrm(const sp<RfxMclMessage>& msg) {
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
void RmcOemRequestHandler::requestGetGcfMode() {
    int err = 0;
    int ret = 0;

    sp<RfxAtResponse> pResponse = atSendCommandSingleline(String8::format("AT+EPCT?"),
            (char *) "+EPCT:");

    if (pResponse->getError() < 0 || pResponse->getSuccess() == 0) {
        // assume radio is off
        logD(RFX_LOG_TAG, "AT+EPCT return ERROR");
        return;
    }

    RfxAtLine *line = pResponse->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) {
        logD(RFX_LOG_TAG, "AT+EPCT return ERROR");
        return;
    }

    ret = line->atTokNextint(&err);
    if (err < 0){
        logD(RFX_LOG_TAG, "AT+EPCT return ERROR");
        return;
    }

    rfx_property_set(PROPERTY_GSM_GCF_TEST_MODE, String8::format("%d", ret));
    getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_KEY_GCF_TEST_MODE, ret);

    // logD(RFX_LOG_TAG, "AT+EPCT return %d", ret);
}

void RmcOemRequestHandler::requestMdVersion() {
    sp<RfxAtResponse> pResponse = atSendCommandMultiline(String8::format("AT+EMDVER?"),
            (char *) "+EMDVER:");
    if (pResponse->getError() < 0 || pResponse->getSuccess() == 0) {
        logE(RFX_LOG_TAG, "AT+EMDVER? fail");
        return;
    }

    // notify RfxVersionManager
    RfxVersionManager::getInstance()->initVersion(pResponse->getIntermediates());
}

void RmcOemRequestHandler::requestSetTxPowerStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    int* mode = (int*)(msg->getData()->getData());
    sp<RfxAtResponse> atResponse = atSendCommand(String8::format("AT+TXPWRSTUS=%d", mode[0]));
    // check at cmd result, consider default as success
    if (atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
        logE(RFX_LOG_TAG, "requestSetTxPowerStatus: %d failed", mode[0]);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
    }
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::requestGetPhoneCapability(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    RIL_PhoneCapability *pPhoneCapabilityData = NULL;
    char feature[] = "EDATASIM Supported";
    int isEDataSimSupported = getFeatureVersion(feature);

    pPhoneCapabilityData = (RIL_PhoneCapability*)calloc(1, sizeof(RIL_PhoneCapability));
    if (pPhoneCapabilityData == NULL) {
        logE(RFX_LOG_TAG, "OOM");
        goto error;
    }

    pPhoneCapabilityData->maxActiveData = 1;
    pPhoneCapabilityData->maxActiveInternetData = 1;
    pPhoneCapabilityData->isInternetLingeringSupported = (isEDataSimSupported == 1 ? 1 : 0);

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
    logE(RFX_LOG_TAG, "requestGetPhoneCapability error");
    free(pPhoneCapabilityData);
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);

}

void RmcOemRequestHandler::requestEnableDsdaIndication(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    int* enable = (int*)(msg->getData()->getData());
    sp<RfxAtResponse> atResponse = atSendCommand(String8::format("AT+EDSDA=%d", enable[0]));
    // check at cmd result, consider default as success
    if (atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
        logE(RFX_LOG_TAG, "requestEnableDsdaIndication: %d failed", enable[0]);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED,
                RfxVoidData(), msg);
    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
    }
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::requestGetDsdaStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    sp<RfxAtResponse> pResponse = atSendCommandSingleline("AT+EDSDA?", "+EDSDA:");
    if (pResponse != NULL && !pResponse->isAtResponseFail()) {
        RfxAtLine* line = pResponse->getIntermediates();
        int err;
        int mode = 0;
        line->atTokStart(&err);
        if (err < 0) goto error;
        mode = line->atTokNextint(&err);
        getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_DSDA_MODE, mode);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxIntsData(&mode, 1), msg);
    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED,
                RfxVoidData(), msg);
        logE(RFX_LOG_TAG, "requestGetDsdaStatus send at command Fail");
    }

    responseToTelCore(responseMsg);
error:
    logE(RFX_LOG_TAG, "requestGetDsdaStatus error");
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_REQUEST_NOT_SUPPORTED,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::requestSetModemConfig(const sp<RfxMclMessage>& msg) {
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
            if (pResponse->getError() != 0 || pResponse->getSuccess() != 1) {
                logE(RFX_LOG_TAG, "requestSetModemConfig failed");
                sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                        RfxVoidData(), msg);
                return;
            }
            rfx_property_set(PROPERTY_3G_SIM, String8::format("%d", 1).string());
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

void RmcOemRequestHandler::requestNvResetConfig(const sp<RfxMclMessage>& msg) {
    int* type = (int*)(msg->getData()->getData());
    logD(RFX_LOG_TAG, "requestNvResetConfig: type = %d", *type);

    // type1: modem reset
    // type3: GUTI reset
    if (*type == 1) {
        int mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);

        // only do power off when it is on
        if (!mdOff) {
            atSendCommand("AT+EFUN=0");
            getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, true);
            atSendCommand("AT+EMDT=0");
            logD(RFX_LOG_TAG, "%s(): set radio off before power off MD.", __FUNCTION__);
            atSendCommand("AT+EPOF");

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
        atSendCommand("AT+EFUN=0");
        atSendCommand("AT+ECMSID=1");

        int mainSlotId = RfxRilUtils::getMajorSim() - 1;
        int slotMode = 0;
        int targetMode = 0;

        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            RIL_RadioState radioState = (RIL_RadioState) getMclStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_RADIO_STATE, 0);
            if (RADIO_STATE_ON == radioState) {
                slotMode = 1;
            } else {
                slotMode = 0;
            }

            switch(mainSlotId) {
                case RFX_SLOT_ID_0:
                    slotMode = slotMode << i;
                    break;
                case RFX_SLOT_ID_1:
                    if (RFX_SLOT_ID_0 == i) {
                        slotMode = slotMode << 1;
                    } else if (RFX_SLOT_ID_2 == i) {
                        slotMode = slotMode << 2;
                    } else if (RFX_SLOT_ID_3 == i) {
                        slotMode = slotMode << 3;
                    }
                    break;
                case RFX_SLOT_ID_2:
                    if (RFX_SLOT_ID_0 == i) {
                        slotMode = slotMode << 1;
                    } else if (RFX_SLOT_ID_1 == i) {
                        slotMode = slotMode << 2;
                    } else if (RFX_SLOT_ID_3 == i) {
                        slotMode = slotMode << 3;
                    }
                    break;
                case RFX_SLOT_ID_3:
                    if (RFX_SLOT_ID_0 == i) {
                        slotMode = slotMode << 1;
                    } else if (RFX_SLOT_ID_1 == i) {
                        slotMode = slotMode << 2;
                    } else if (RFX_SLOT_ID_2 == i) {
                        slotMode = slotMode << 3;
                    }
                    break;
                default:
                    break;
            }
            targetMode |= slotMode;
        }
        logD(RFX_LOG_TAG, "requestNvResetConfig, main slot = %d, target mode = %d",
                mainSlotId, targetMode);

        if (targetMode > 0) {
            sp<RfxAtResponse> response =
                atSendCommand(String8().format(("AT+EFUN=%d"), targetMode));
            // add retry mechanism
            int retryCount = 0;
            while (response->getSuccess() == 0 && retryCount < 20) {
                response = atSendCommand(String8().format(("AT+EFUN=%d"), targetMode));
                retryCount++;
                usleep(500*1000);
            }

            if (response->getSuccess() == 1) {
                for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++){
                    RIL_RadioState radioState =
                            (RIL_RadioState) getMclStatusManager(i)->getIntValue(
                            RFX_STATUS_KEY_RADIO_STATE, 0);
                    if (RADIO_STATE_ON == radioState) {
                        // send power off urc and power on again
                        getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE,
                            RADIO_STATE_OFF);
                        logD(RFX_LOG_TAG, "requestNvResetConfig: power on slot%d", i);
                        getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE,
                            RADIO_STATE_ON);
                    }
                }
            }
        }
    }

    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::routeCert(const sp<RfxMclMessage>& msg) {
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
    command = NULL;
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

void RmcOemRequestHandler::routeAuth(const sp<RfxMclMessage>& msg) {
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

void RmcOemRequestHandler::enableCapability(const sp<RfxMclMessage>& msg) {
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

void RmcOemRequestHandler::abortCert(const sp<RfxMclMessage>& msg) {
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


void RmcOemRequestHandler::requestSetHsrMode(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> pResponse;
    int errNo = (int) RIL_E_SUCCESS;
    int cmdError = CME_SUCCESS;
    sp<RfxMclMessage> responseMsg;
    int mode = ((int *) msg->getData()->getData())[0];
    char hsrMode[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.hsr.mode", hsrMode, "0");
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

void RmcOemRequestHandler::queryCapability(const sp<RfxMclMessage>& msg) {
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

char* RmcOemRequestHandler::getCertPhase1Command(const char* cert) {
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

char* RmcOemRequestHandler::getCertPhase2Command(int custId, const char* msg) {
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

char* RmcOemRequestHandler::getAuthPhaseCommand(const char* msg) {
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

char* RmcOemRequestHandler::getCapabilityCommand(int uid, char* featureName, int timer) {
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

char* RmcOemRequestHandler::getQueryCapabilityCommand(int uid, char* featureName) {
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

char* RmcOemRequestHandler::getAbortCommand(int uid) {
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

char* RmcOemRequestHandler::getAuthPrefixCommand() {
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

char* RmcOemRequestHandler::getQueryPrefixCommand() {
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

void RmcOemRequestHandler::updateContext(int id, int custId, char* key) {
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

void RmcOemRequestHandler::removeContext(int id) {
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
void RmcOemRequestHandler::byteToString(char* byte, int size, char** output) {
    char *tmp = *output;
    for (int i=0; i<size; i++) {
        *tmp++ = HEX_LOOKUP[byte[i] >> 4];
        *tmp++ = HEX_LOOKUP[byte[i] & 0x0F];
    }
    *tmp = '\0';
}

void RmcOemRequestHandler::stringToByte(char* hexstr, int size, char** output) {
    if (size % 2 != 0) {
        return;
    }
    char *res = *output;
    int finalSize = size / 2;
    for (int i=0, j=0; j<finalSize; i+=2, j++)
        res[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i+1] % 32 + 9) % 25;
    res[finalSize] = '\0';
}

void RmcOemRequestHandler::updateRnd(char *rnd) {
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

bool RmcOemRequestHandler::sendOemToNwModule(char *string) {
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

void RmcOemRequestHandler::requestSendSarIndicator(const sp<RfxMclMessage>& msg) {
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
    const char* cmdParameter = strings[1];
    logE(RFX_LOG_TAG, "requestSendSarIndicator: cmdType: %d, cmdParameter:%s",cmdType,cmdParameter);
    String8 cmdStr;
    switch(cmdType) {
        case 0:
            cmdStr = String8::format("AT+ERFIDX=%s", cmdParameter);
            break;
        case 1:
            cmdStr = String8::format("AT+ERFTX=%s", cmdParameter);
            break;
        default:
            break;
    }

    if (cmdStr.isEmpty()) {
        logE(RFX_LOG_TAG, "requestSendSarIndicator: cmdStr == NULL");
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_INVALID_ARGUMENTS,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    sp<RfxAtResponse> atResponse = atSendCommand(cmdStr);
    // check at cmd result, consider default as success
    if (atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
        logE(RFX_LOG_TAG, "requestSendSarIndicator: %s", cmdStr.string());
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg);
    } else {
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
    }
    responseToTelCore(responseMsg);
}

void RmcOemRequestHandler::handleRawUrc(const sp<RfxMclMessage>& msg) {
    char enabled[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get(ENABLE_OEM_URC_PROP, enabled, "0");
    const char *data = (const char *)msg->getData()->getData();
    if (NULL != data && atoi(enabled) == 1) {
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_OEM_HOOK_RAW, m_slot_id,
                RfxStringData((char *)String8::format("%s", data).string()));
        responseToTelCore(urc);
    }
}

void RmcOemRequestHandler::requestSetTxPower(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, power = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    power = pInt[0];

    p_response = atSendCommand(String8::format("AT+EGMC=1, \"max_pwr_limit\",%d", power));
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}
