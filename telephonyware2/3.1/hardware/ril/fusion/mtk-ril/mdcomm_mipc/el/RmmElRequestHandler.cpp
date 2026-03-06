/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2020. All rights reserved.
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
#include "RmmElRequestHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringsData.h"
#include "RfxStringData.h"
#include "RfxRawData.h"
#include <string.h>
#include "RfxVersionManager.h"
#include "ratconfig.h"
#include "RfxRilUtils.h"
#include <dlfcn.h>
#include <libmtkrilutils.h>
#include "RfxEcholocateResponseData.h"
#define RFX_LOG_TAG "RmmElRequestHandler"
// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmElRequestHandler, RIL_CMD_PROXY_3);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxEcholocateResponseData,
        RFX_MSG_REQUEST_GET_ECHOLOCATE_METRICS);
RmmElRequestHandler::RmmElRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int request[] = {
        RFX_MSG_REQUEST_GET_ECHOLOCATE_METRICS,
    };
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
}
RmmElRequestHandler::~RmmElRequestHandler() {
}
void RmmElRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch (id) {
        case RFX_MSG_REQUEST_GET_ECHOLOCATE_METRICS:
            requestGetEcholocateMetrics(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}
void RmmElRequestHandler::onHandleTimer() {
    // do something
}
void RmmElRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch (id) {
        default:
            logE(RFX_LOG_TAG, "should not be here");
            break;
    }
}
int RmmElRequestHandler::getEcholocateCmd(int index, char** cmdString) {
    logD(RFX_LOG_TAG, "getEcholocateMetricsOpcode index = %d ", index);
    int code = CMD_SINGLE_RESP;
    if (index >= ECHOLOCATE_RF_METRICS) {
        switch ((RIL_EcholocateRFType) index) {
            case GET_RF_NTA_PRIMARY:
                strncat(*cmdString, "AT+EDMFAPP=3,2704", strlen("AT+EDMFAPP=3,2704"));
                break;
            case GET_RF_API_VERSION:
                strncat(*cmdString, "AT+EDMFAPP=3,2705", strlen("AT+EDMFAPP=3,2705"));
                break;
            case GET_RF_SRS_CONFIG:
                strncat(*cmdString, "AT+EDMFAPP=3,2706", strlen("AT+EDMFAPP=3,2706"));
                break;
            default:
                logE(RFX_LOG_TAG, "getEcholocateRfOpcode no match op error: %d ", index);
                strncat(*cmdString, "ERROR", strlen("ERROR"));
                code = CMD_ERROR;
                break;
        }
        return code;
    }
    if (index >= ECHOLOCATE_VOICE_QOE) {
        switch ((RIL_EcholocateVoiceQoEType) index) {
            case GET_VoiceAccessNetworkStateSignal:
                strncat(*cmdString, "AT+EDMFAPP=3,2101", strlen("AT+EDMFAPP=3,2101"));
                break;
            default:
                logE(RFX_LOG_TAG, "getEcholocateVoiceQoEOpcode no match op error: %d ", index);
                strncat(*cmdString, "ERROR", strlen("ERROR"));
                code = CMD_ERROR;
            break;
        }
        return code;
    }
    if (index >= ECHOLOCATE_API_5GSA_BASE) {
        switch ((RIL_EcholocateDataMetrics5gSaType) index) {
            case GET_5GSA_DL_CARRIER:
                strncat(*cmdString, "AT+EDMFAPP=3,2407", strlen("AT+EDMFAPP=3,2407"));
                code = CMD_MULTI_RESP;
                break;
            case GET_5GSA_UL_CARRIER:
                strncat(*cmdString, "AT+EDMFAPP=3,2408", strlen("AT+EDMFAPP=3,2408"));
                code = CMD_MULTI_RESP;
                break;
            case GET_5GSA_RRC_LOG:
                strncat(*cmdString, "AT+EDMFAPP=3,2409", strlen("AT+EDMFAPP=3,2409"));
                break;
            case GET_5GSA_NETWORK_LOG:
                strncat(*cmdString, "AT+EDMFAPP=3,2410", strlen("AT+EDMFAPP=3,2410"));
                break;
            case GET_5GSA_API_VERSION:
                strncat(*cmdString, "AT+EDMFAPP=3,2412", strlen("AT+EDMFAPP=3,2412"));
                break;
            default:
                logE(RFX_LOG_TAG, "getEcholocateMetricsOpcode no match op error: %d ", index);
                strncat(*cmdString, "ERROR", strlen("ERROR"));
                code = CMD_ERROR;
            break;
        }
        return code;
    }
    switch((RIL_EcholocateDataMetricsType) index) {
        case GET_NETWORK_IDENTITY_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2204", strlen("AT+EDMFAPP=3,2204"));
            break;
        case GET_SIGNAL_CONDITION_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2205", strlen("AT+EDMFAPP=3,2205"));
            break;
        case GET_COMMON_RF_CONFIGURATION_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2206", strlen("AT+EDMFAPP=3,2206"));
            break;
        case GET_DOWNLINK_CARRIER_INFO_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2207", strlen("AT+EDMFAPP=3,2207"));
            break;
        case GET_UPLINK_CARRIER_INFO_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2208", strlen("AT+EDMFAPP=3,2208"));
            break;
        case GET_DOWNLINK_RF_CONFIGURATION_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2209", strlen("AT+EDMFAPP=3,2209"));
            break;
        case GET_UPLINK_RF_CONFIGURATION_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2210", strlen("AT+EDMFAPP=3,2210"));
            break;
        case GET_BEARER_CONFIGURATION_METHOD:
            strncat(*cmdString, "AT+EDMFAPP=3,2211", strlen("AT+EDMFAPP=3,2211"));
            break;
        case GET_ENDCLTELOG:
            strncat(*cmdString, "AT+EDMFAPP=3,2304", strlen("AT+EDMFAPP=3,2304"));
            break;
        case GET_5GNRMMWCELLLOG:
            strncat(*cmdString, "AT+EDMFAPP=3,2305", strlen("AT+EDMFAPP=3,2305"));
            break;
        case GET_SSBBEANLOG:
            strncat(*cmdString, "AT+EDMFAPP=3,2306", strlen("AT+EDMFAPP=3,2306"));
            break;
        case GET_ENDCUPLINGLOG:
            strncat(*cmdString, "AT+EDMFAPP=3,2307", strlen("AT+EDMFAPP=3,2307"));
            break;
        case GET_5GUILOG:
            strncat(*cmdString, "AT+EDMFAPP=3,2308", strlen("AT+EDMFAPP=3,2308"));
            break;
        case GET_API_VERSION:
            strncat(*cmdString, "AT+EDMFAPP=3,2203", strlen("AT+EDMFAPP=3,2203"));
            break;
        case GET_NETWORKTYPE_SEARCHING:
            strncat(*cmdString, "AT+EGREG?", strlen("AT+EGREG?"));
            break;
        case GET_DATA_SETTING_METHOD:
        default:
            logE(RFX_LOG_TAG, "getEcholocateMetricsOpcode no match op error: %d ", index);
            strncat(*cmdString, "ERROR", strlen("ERROR"));
            code = CMD_ERROR;
            break;
    }
    return code;
}
// Query command format: AT+EDMFAPP=<op_code>,<cmd_code>
// ex. AT+EDMFAPP=3,2409
// Response format:
// CMD_SINGLE_RESP: +EDMFAPP=<op_code>,<cmd_code>,[<data>,...<data>]
// CMD_MULTI_RESP: +EDMFAPP=<index>,<op_code>,<cmd_code>,[<data>,...<data>]
void RmmElRequestHandler::requestGetEcholocateMetrics(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> responseMsg;
    sp<RfxAtResponse> pResponse;
    RfxAtLine *pCur = NULL;
    unsigned int strLength = 0;
    int index = ((int *) msg->getData()->getData())[0];
    int skip;
    char* reg_info;
    int err = -1;
    char* tmp = NULL;
    char* resultData = NULL;
    unsigned int count = 0;
    std::string outString = "empty";
    logD(RFX_LOG_TAG, "%s: %d: ", __FUNCTION__, index);
    RIL_EcholocateResponse* elResponse =
        (RIL_EcholocateResponse*) calloc(1, sizeof(RIL_EcholocateResponse));
    if (elResponse == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    char *cmdString = (char *) calloc(100, sizeof(char));
    if (cmdString == NULL) {
        logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
        free(elResponse);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    int code = getEcholocateCmd(index, &cmdString);
    if (code == CMD_ERROR) {
        logE(RFX_LOG_TAG, "[Echolocate] invalid command %d", index);
        responseMsg = RfxMclMessage::obtainResponse(RIL_E_GENERIC_FAILURE, RfxVoidData(), msg);
        free(elResponse);
        free(cmdString);
        responseToTelCore(responseMsg);
        return;
    }
    if (strncmp(cmdString, "AT+EDMFAPP", strlen("AT+EDMFAPP")) == 0) {
        logD(RFX_LOG_TAG, "[Echolocate] send %s", cmdString);
        pResponse = atSendCommandMultiline(String8::format("%s", cmdString).string(),
                (char *) "+EDMFAPP:");
        if (pResponse == NULL
                || pResponse->getError() != 0
                || pResponse->getSuccess() == 0
                || pResponse->getIntermediates() == NULL) {
            logE(RFX_LOG_TAG, "%s: error", __FUNCTION__);
            goto error;
        }
        for (pCur = pResponse->getIntermediates(); pCur != NULL; pCur = pCur->getNext()) {
            count++;
        }
        if (count == 0) {
            logE(RFX_LOG_TAG, "%s: with empty result", __FUNCTION__);
            goto response;
        } else {
            logD(RFX_LOG_TAG, "%s: with count: %u", __FUNCTION__, count);
        }
        count = 0;
        for (pCur = pResponse->getIntermediates(); pCur != NULL; pCur = pCur->getNext()) {
            tmp = pCur->getLine();
            logD(RFX_LOG_TAG, "result %s", tmp);
            // go to start position
            pCur->atTokStart(&err);
            if (err < 0) goto error;
            // Skip <index> for multiple command
            //if (code == CMD_MULTI_RESP) {
            //    skip = pCur->atTokNextint(&err);
            //    if (err < 0) goto error;
            //}
            // Skip <op_code>
            skip = pCur->atTokNextint(&err);
            if (err < 0) goto error;
            // Skip <cmd_code>
            skip = pCur->atTokNextint(&err);
            if (err < 0) goto error;
            tmp = pCur->getCurrentLine();
            logD(RFX_LOG_TAG, "result %s", tmp);
            if (tmp != NULL && strlen(tmp) > 0) {
                strLength += strlen(tmp);
                if (count == 0) {
                    outString = String8::format("%s", tmp).string();
                } else {
                    outString = outString + "," + String8::format("%s", tmp).string();
                }
                count++;
            }
        }
        if (strLength == 0) {
            logD(RFX_LOG_TAG, "%s: strLength zero", __FUNCTION__);
            goto response;
        }
        // Add 1 for end char and (count-1) for "," adjustment when adding lines.
        resultData = (char *) calloc(strLength + count, sizeof(char));
        if (resultData == NULL) {
            logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
            goto error;
        }
        // size(outString)=strLength+count-1 for "," adjustment when adding lines.
        strncpy(resultData, outString.c_str(), (strLength + count - 1)*sizeof(char));
        elResponse->result = resultData;
        elResponse->resultLen = strlen(elResponse->result);
        logD(RFX_LOG_TAG, "[EcholocateMetrics] result %s %d",
            elResponse->result, elResponse->resultLen);
    } else {
        pResponse = atSendCommandSingleline(String8::format("AT+EGREG?").string(),
                (char *) "+EGREG:");
        if (pResponse == NULL
               || pResponse->getError() != 0
               || pResponse->getSuccess() == 0
               || pResponse->getIntermediates() == NULL) {
           logE(RFX_LOG_TAG, "[Echolocate] nwtype error");
           goto error;
        }
        pCur = pResponse->getIntermediates();
        tmp = pCur->getLine();
        logE(RFX_LOG_TAG, "result %s", tmp);
        // go to start position
        pCur->atTokStart(&err);
        if (err < 0) goto error;
        skip = pCur->atTokNextint(&err);
        if (err < 0) goto error;
        reg_info = pCur->atTokNextstr(&err);
        strLength = strlen(reg_info);
        resultData = (char *) calloc(strLength, sizeof(char));
        if (resultData == NULL) {
            logE(RFX_LOG_TAG, "%s: OOM", __FUNCTION__);
            free(elResponse);
            free(cmdString);
            responseMsg = RfxMclMessage::obtainResponse(RIL_E_NO_MEMORY, RfxVoidData(), msg);
            responseToTelCore(responseMsg);
            return;
        }
        strncpy(resultData, reg_info,  strlen(reg_info));
        elResponse->result = resultData;
        elResponse->resultLen = strlen(reg_info);
        logE(RFX_LOG_TAG, "reg_info %s", elResponse->result);
    }
response:
    elResponse->index = index;
    elResponse->error = 0;
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxEcholocateResponseData(elResponse, sizeof(RIL_EcholocateResponse)), msg);
    if (resultData != NULL) free(resultData);
    if (elResponse != NULL) free(elResponse);
    if (cmdString != NULL) free(cmdString);
    responseToTelCore(responseMsg);
    return;
error:
    logD(RFX_LOG_TAG, "requestGetEcholocateMetrics error: %d", err);
    elResponse->result = NULL;
    elResponse->resultLen = 0;
    elResponse->error = 1002;
    elResponse->index = index;
    responseMsg = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxEcholocateResponseData(elResponse, sizeof(RIL_EcholocateResponse)), msg);
    free(elResponse);
    free(cmdString);
    free(resultData);
    responseToTelCore(responseMsg);
    return;
}
