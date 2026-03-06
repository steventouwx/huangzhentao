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
#include <math.h>
#include <telephony/mtk_ril.h>
#include <mipc_msg_tlv_api.h>
#include <mipc_msg_tlv_const.h>
#include "libmtkrilutils.h"
#include "RfxMipcData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxNetworkScanData.h"
#include "RfxNetworkScanResultData.h"
#include "RmmNwNrtRequestHandler.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmmNwNrtReqHdlr"

static const int request[] = {
    RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS,
    RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT,
    RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN,
    RFX_MSG_REQUEST_GET_SUGGESTED_PLMN_LIST
};

static const int events[] = {
    RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN
};

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxNetworkScanData, RfxVoidData, RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringsData, RFX_MSG_REQUEST_GET_SUGGESTED_PLMN_LIST);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmNwNrtRequestHandler, RIL_CMD_PROXY_8);

bool RmmNwNrtRequestHandler::sIncrementalSupport;

RmmNwNrtRequestHandler::RmmNwNrtRequestHandler(int slot_id, int channel_id) :
        RmmNwHandler(slot_id, channel_id) {
    logV(LOG_TAG, "RmmNwNrtRequestHandler[%d] start", slot_id);

    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    registerToHandleEvent(events, sizeof(events)/sizeof(int));
}

RmmNwNrtRequestHandler::~RmmNwNrtRequestHandler() {
}

void RmmNwNrtRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {

    switch(msg->getId()) {
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS:
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT:
            handleAvailableNetworksRequest(msg);
            break;
        case RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN:
            handleNetworkScanRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUGGESTED_PLMN_LIST:
            handleGetSuggestedPlmnListRequest(msg);
            break;
        default:
            logE(LOG_TAG, "Unrecoganized request");
            break;
    }
}

void RmmNwNrtRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN:
            handleNetworkScanEvent();
            break;
        default:
            logE(LOG_TAG, "onHandleEvent, should not be here");
            break;
    }
}


void RmmNwNrtRequestHandler::handleAvailableNetworksRequest(
            const sp<RfxMclMessage>& msg) {
    char **result = NULL;
    char *dataBuff = NULL;
    mipc_nw_extend_provider_struct4 *infoList = NULL;
    sp<RfxMclMessage> response;
    PlmnResponse *respDataSlim = NULL;
    PlmnResponseWithAct *respData = NULL;
    uint16_t read = 0;
    int infoCount, plmnCount, matchResult, lac, rat;
    int i, j, k;
    bool withAct = (msg->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS) ?
        false : true;
    const int strCount = sizeof(PlmnResponseWithAct) / sizeof(char *);
    const int strCountSlim = sizeof(PlmnResponse) / sizeof(char *);

    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NORMAL;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_PLMN_LIST_REQ, msg->getSlotId());
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

    if (cnfData->getData() == NULL
        || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "handleAvailableNetworksRequest: result[%d]", cnfData->getResult());
        goto error;
    }

    infoCount = cnfData->getMipcUint8Val(
            MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_COUNT, 0xff);
    if (infoCount == 0xff) {
        logE(LOG_TAG, "handleAvailableNetworksRequest: list count invalid");
        goto error;
    }

    result = (char **)malloc(infoCount * strCount * sizeof(char *));
    if (result == NULL) {
        logE(LOG_TAG, "handleAvailableNetworksRequest: Insuffient memory");
        goto error;
    }
    dataBuff = (char *)malloc(infoCount * strCount * MAX_OPER_NAME_LENGTH);
    if (dataBuff == NULL) {
       logE(LOG_TAG, "handleAvailableNetworksRequest: Insuffient memory");
       goto error;
    }
    respData = (PlmnResponseWithAct *)result;

    for (i = 0; i < infoCount; i++) {
        respData[i].longAlpha = &dataBuff[i*strCount * MAX_OPER_NAME_LENGTH];
        respData[i].longAlpha[0] = '\0';
        respData[i].shortAlpha = &dataBuff[(i*strCount+1) * MAX_OPER_NAME_LENGTH];
        respData[i].shortAlpha[0] = '\0';
        respData[i].numeric = &dataBuff[(i*strCount+2) * MAX_OPER_NAME_LENGTH];
        respData[i].numeric[0] = '\0';
        respData[i].state = &dataBuff[(i*strCount+3) * MAX_OPER_NAME_LENGTH];
        respData[i].state[0] = '\0';
        respData[i].lacTac = &dataBuff[(i*strCount+4) * MAX_OPER_NAME_LENGTH];
        respData[i].lacTac[0] = '\0';
        respData[i].rat = &dataBuff[(i*strCount+5) * MAX_OPER_NAME_LENGTH];
        respData[i].rat[0] = '\0';
    }

    infoList = (mipc_nw_extend_provider_struct4 *)cnfData->getMipcVal(
            MIPC_NW_GET_PLMN_LIST_CNF_T_EXTEND_INFO_LIST, &read);
    if (read <= 0 || infoList == NULL
        || read % sizeof(*infoList) != 0
        || read / sizeof(*infoList) != infoCount) {
        logE(LOG_TAG, "handleAvailableNetworksRequest: MIPC invalid data,"
                "read[%d] infoList[%d]", read, (int)sizeof(mipc_nw_extend_provider_struct4));
        goto error;
    }

    for (i = 0, j = 0; i < infoCount; i++) {
        logD(LOG_TAG, "handleAvailableNetworksRequest: info[%d]"
                "->numeric[%s], lac[%s], act[%d], stat[%d]",
                i, infoList[i].oper_numeric_name, infoList[i].lac, infoList[i].act, infoList[i].stat);
        if (!withAct) {
            // remove duplicated plmn
            for (k = 0; k < j; k++) {
                if (strcmp(infoList[i].oper_numeric_name, respData[k].numeric) == 0)
                    break;
            }
            if (k < j) continue;
        }

        lac = (int)strtol(infoList[i].lac, NULL, 16);
        if (lac < 0 || lac > 0xffffff) {
            logE(LOG_TAG, "The %dth <lac> invalid[%x]", i, lac);
            goto error;
        }

        rat = (int)infoList[i].act;

        strncpy(respData[j].lacTac, infoList[i].lac, MAX_DATA_SIZE);
        strncpy(respData[j].numeric, infoList[i].oper_numeric_name, MAX_DATA_SIZE);
        respData[j].numeric[MAX_DATA_SIZE] = '\0';

        if (isOp07Support()) {
            matchResult = matchPlmnName(respData[j].longAlpha, respData[j].shortAlpha,
                        respData[j].numeric, lac, rat, MATCH_EONS|MATCH_TS25);
        } else {
            matchResult = matchPlmnName(respData[j].longAlpha, respData[j].shortAlpha,
                        respData[j].numeric, lac, rat);
        }
        if (matchResult == 0) {
            strncpy(respData[j].longAlpha, infoList[i].oper_long_name, MAX_DATA_SIZE);
            respData[j].longAlpha[MAX_DATA_SIZE] = '\0';
            strncpy(respData[j].shortAlpha, infoList[i].oper_short_name, MAX_DATA_SIZE);
            respData[j].shortAlpha[MAX_DATA_SIZE] = '\0';
        }

        switch (infoList[i].act) {
            case 0:     strcpy(respData[j].rat, "2G"); break;
            case 2:     strcpy(respData[j].rat, "3G"); break;
            case 7:
            case 10:    strcpy(respData[j].rat, "4G"); break;
            case 11:
            case 12:
            case 13:    strcpy(respData[j].rat, "5G"); break;
            default:
                logE(LOG_TAG, "The %dth <act> invalid[%d]", i, infoList[i].act);
                goto error;
        }

        switch(infoList[i].stat) {
        case MIPC_NW_PROVIDER_STATE_UNKNOWN:
            strncpy(respData[j].state, "unknown", MAX_DATA_SIZE);
            break;
        case MIPC_NW_PROVIDER_STATE_AVAILABLE:
            strncpy(respData[j].state, "available", MAX_DATA_SIZE);
            break;
        case MIPC_NW_PROVIDER_STATE_CURRENT:
            strncpy(respData[j].state, "current", MAX_DATA_SIZE);
            break;
        case MIPC_NW_PROVIDER_STATE_FORBIDDEN:
            strncpy(respData[j].state, "forbidden", MAX_DATA_SIZE);
            break;
        default:
            logE(LOG_TAG, "The %dth <stat> invalid: %d", i, infoList[i].stat);
            goto error;
        }
        respData[j++].state[MAX_DATA_SIZE] = '\0';
    }

    plmnCount = j;

    logD(LOG_TAG, "handleAvailableNetworksRequest: done, total[%d]", plmnCount);

    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    sPlmnListAbort = false; /* always clear here to prevent race condition scenario */
    if (withAct)
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringsData(result, j * strCount), msg, false);
    else {
        respDataSlim = (PlmnResponse *)malloc(plmnCount * strCountSlim *sizeof(char *));
        if (respDataSlim == NULL) {
            logE(LOG_TAG, "handleAvailableNetworksRequest: Insuffient memory");
        } else {
            for (i = 0; i < j; i++) {
                respDataSlim[i].longAlpha = respData[i].longAlpha;
                respDataSlim[i].shortAlpha = respData[i].shortAlpha;
                respDataSlim[i].numeric = respData[i].numeric;
                respDataSlim[i].state = respData[i].state;
            }
            response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxStringsData((void *)respDataSlim, plmnCount * sizeof(PlmnResponse)), msg, false);
            free(respDataSlim);
        }
    }
    responseToTelCore(response);

    free(dataBuff);
    free(result);

    return;

error:
    if (dataBuff) free(dataBuff);
    if (result) free(result);

    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);

    if (sPlmnListAbort) {
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_CANCELLED, RfxVoidData(), msg, false);
        sPlmnListAbort = false;
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_MODEM_ERR, RfxVoidData(), msg, false);
    }
    responseToTelCore(response);
}

void RmmNwNrtRequestHandler::handleNetworkScanRequest(
        const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    RIL_Errno radioError = RIL_E_INVALID_ARGUMENTS;
    RIL_NetworkScanRequest* reqArgs = (RIL_NetworkScanRequest *)msg->getData()->getData();

    logD(LOG_TAG, "handleNetworkScanRequest: "
            "type[%d], interval[%d], specifiers[%d],"
            "maxSearchTime[%d], mccMncs[%d]"
            "incrementalResults[%d], incrementalResultsPeriodicity[%d],",
            reqArgs->type, reqArgs->interval, reqArgs->specifiers_length,
            reqArgs->maxSearchTime, reqArgs->mccMncs_length,
            reqArgs->incrementalResults, reqArgs->incrementalResultsPeriodicity);

    if (reqArgs->interval < 5 || reqArgs->interval > 300)
        goto error;
    if (reqArgs->specifiers_length <= 0
        || reqArgs->specifiers_length > MAX_RADIO_ACCESS_NETWORKS)
        goto error;

    for (int i = 0; i < reqArgs->specifiers_length; i++) {
        RIL_RadioAccessSpecifier *spfr = &reqArgs->specifiers[i];
        logV(LOG_TAG, "handleNetworkScanRequest: specifiers[%d]"
                "->radio_access_network[%d], bands_length[%d], channels_length[%d]",
                i, spfr->radio_access_network, spfr->bands_length, spfr->channels_length);

        for (int j = 0; j < (spfr->bands_length); j++) {
            switch (spfr->radio_access_network) {
                case GERAN:
                    logV(LOG_TAG, "bands.geran_bands[%d]=%d",
                            j, spfr->bands.geran_bands[j]);
                    break;
                case UTRAN:
                    logV(LOG_TAG, "bands.utran_bands[%d]=%d",
                            j, spfr->bands.utran_bands[j]);
                    break;
                case EUTRAN:
                    logV(LOG_TAG, "bands.eutran_bands[%d]=%d",
                            j, spfr->bands.eutran_bands[j]);
                    break;
                case NEW_RADIO:
                    logV(LOG_TAG, "bands.ngranBands[%d]=%d",
                            j, spfr->bands.ngranBands[j]);
                    break;
                default: break;
            }
        }
        for (int j = 0; j < spfr->channels_length; j++) {
            logV(LOG_TAG, "channels[%d]=%d", j, spfr->channels[j]);
        }
    }
    // for VTS NW scan case.
    if (reqArgs->maxSearchTime == 0 && reqArgs->incrementalResultsPeriodicity == 0) {
        reqArgs->maxSearchTime = 3600;
        if (reqArgs->specifiers_length == 1
                && reqArgs->specifiers[0].radio_access_network == 1) {
            reqArgs->maxSearchTime = 0;
        }
        reqArgs->incrementalResultsPeriodicity = 1;
    }
    if (reqArgs->maxSearchTime < 70 || reqArgs->maxSearchTime > 3600) {
        if (reqArgs->maxSearchTime >= 60 && reqArgs->maxSearchTime < 70)
            radioError = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }
    if (reqArgs->incrementalResultsPeriodicity < 1
        || reqArgs->incrementalResultsPeriodicity > 10)
        goto error;

    for (int i = 0; i < reqArgs->mccMncs_length; i++) {
        logV(LOG_TAG, "mccMncs[%d]=%s", i, reqArgs->mccMncs[i]);
    }

    if (isNetworkScanFullSupport()) {
        if (isAPInCall() == true) {
            radioError = RIL_E_MODEM_ERR;
            goto error;
        }
        sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NETWORK_SCAN;
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);

        sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_START_NETWORK_SCAN_REQ, m_slot_id);

        //type
        reqData->addMipcTlvUint32(MIPC_NW_START_NETWORK_SCAN_REQ_T_SCAN_TYPE,
            (reqArgs->type == RIL_ONE_SHOT) ? MIPC_NW_SCAN_TYPE_ONE_SHOT : MIPC_NW_SCAN_TYPE_PERIODIC);
        //interval
        reqData->addMipcTlvUint32(MIPC_NW_START_NETWORK_SCAN_REQ_T_INTERVAL, reqArgs->interval);
        //maxSearchTime
        reqData->addMipcTlvUint32(MIPC_NW_START_NETWORK_SCAN_REQ_T_MAX_SEARCH_TIME,
            reqArgs->maxSearchTime);
        //incrementalResults
        reqData->addMipcTlvUint32(MIPC_NW_START_NETWORK_SCAN_REQ_T_INCREMENTAL_RESULTS,
            reqArgs->incrementalResults);
        //incrementalResultsPeriodicity
        reqData->addMipcTlvUint32(MIPC_NW_START_NETWORK_SCAN_REQ_T_INCREMENTAL_RESULT_PERIODICITY,
            reqArgs->incrementalResultsPeriodicity);

        mipc_nw_record_info_struct4 info;
        mipc_msg_tlv_array_t *p_array = mipc_msg_add_tlv_array(reqData->getData(),
            MIPC_NW_START_NETWORK_SCAN_REQ_T_RECORD_LIST, (uint16_t)reqArgs->specifiers_length);

        for (size_t i = 0; i < (reqArgs->specifiers_length); i++) {
            memset(&info, 0, sizeof(mipc_nw_record_info_struct4));
            if (reqArgs->specifiers[i].radio_access_network == 0x01) {
                info.rat = 1;
            } else if (reqArgs->specifiers[i].radio_access_network == 0x02) {
                info.rat = 2;
            } else if (reqArgs->specifiers[i].radio_access_network == 0x03) {
                info.rat = 4;
            } else if (reqArgs->specifiers[i].radio_access_network == 0x06) {
                info.rat = 128;
            } else {
                logV(LOG_TAG, "Should not be here");
            }
            info.num_band = reqArgs->specifiers[i].bands_length;
            for (size_t j = 0; j < (reqArgs->specifiers[i].bands_length); j++) {
                info.band[j] = reqArgs->specifiers[i].bands.eutran_bands[j];
            }
            info.num_channel = reqArgs->specifiers[i].channels_length;
            for (size_t j = 0; j < (reqArgs->specifiers[i].channels_length); j++) {
                info.channel[j] = reqArgs->specifiers[i].channels[j];
            }
            //specifiers
            mipc_msg_add_idx(reqData->getData(), p_array, i, sizeof(mipc_nw_record_info_struct4), &info);
        }

        mipc_nw_plmn_info_struct4 mccmnc;
        mipc_msg_tlv_array_t *p_array2 = mipc_msg_add_tlv_array(reqData->getData(),
            MIPC_NW_START_NETWORK_SCAN_REQ_T_PLMN_LIST, (uint16_t)reqArgs->mccMncs_length);

        for (size_t i = 0; i < reqArgs->mccMncs_length; i++) {
            memset(&mccmnc, 0, sizeof(mipc_nw_plmn_info_struct4));
            strncpy(mccmnc.plmn_id, reqArgs->mccMncs[i], MIPC_MAX_PLMN_ID_LEN - 1);
            //mccMncs
            mipc_msg_add_idx(reqData->getData(), p_array2, i, sizeof(mipc_nw_plmn_info_struct4), &mccmnc);
        }

        sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

        if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
            radioError = RIL_E_GENERIC_FAILURE;
            sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
            getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
            sPlmnListAbort = 0; /* always clear here to prevent race condition scenario */
        } else {
            radioError = RIL_E_SUCCESS;
        }
    }else {
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxVoidData(), msg, false);
        responseToTelCore(resp);
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);

        // send simulate network scan event
        sendEvent(RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN, RfxVoidData(), RIL_CMD_PROXY_8, m_slot_id,
                -1, -1, ms2ns(200), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
        return;
    }

error:
    logE(LOG_TAG, "requestStartNetworkScan radioError = %d", (int)radioError);
    resp = RfxMclMessage::obtainResponse(msg->getId(), radioError,
                RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwNrtRequestHandler::handleGetSuggestedPlmnListRequest(const sp<RfxMclMessage>& msg) {
    int *args = (int *)msg->getData()->getData();
    sp<RfxMclMessage> response;
    uint16_t read = 0;

    if (args == NULL || msg->getData()->getDataLength() != 3 * sizeof(int)) {
        logE(LOG_TAG, "handleGetSuggestedPlmnListRequest: invalid args");
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    int rat = args[0];
    int num = args[1];
    int timer = args[2];

    logD(LOG_TAG, "handleGetSuggestedPlmnListRequest rat[%d], num[%d], timer[%d]", rat, num, timer);

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ, m_slot_id);
    reqData->addMipcTlvUint8(
            MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_RAT, rat);
    reqData->addMipcTlvUint8(
            MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_NUM, num);
    reqData->addMipcTlvUint8(
            MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_TIMER, timer);

    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

    if (cnfData->getData() == NULL
        || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "handleGetSuggestedPlmnListRequest: result[%d]", cnfData->getResult());
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    int infoCount = cnfData->getMipcUint8Val(
            MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_COUNT, 0xff);
    if (infoCount == 0xff) {
        logE(LOG_TAG, "handleGetSuggestedPlmnListRequest: list count invalid");
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    mipc_nw_suggested_struct4 *infoList = (mipc_nw_suggested_struct4 *)cnfData->getMipcVal(
           MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_LIST, &read);
    if (read <= 0 || infoList == NULL
            || read % sizeof(*infoList) != 0
            || read / sizeof(*infoList) != infoCount) {
        logE(LOG_TAG, "handleGetSuggestedPlmnListRequest: MIPC invalid data,"
                "read[%d] infoList[%d]", read, (int)sizeof(mipc_nw_suggested_struct4));
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    logD(LOG_TAG, "handleGetSuggestedPlmnListRequest: count[%d]", infoCount);

    char **result = (char **)calloc(infoCount, sizeof(char *));
    char *dataBuff = (char *)malloc(infoCount * MAX_OPER_NAME_LENGTH);
    if (result == NULL || dataBuff == NULL) {
        logE(LOG_TAG, "handleGetSuggestedPlmnListRequest: Insufficient memory");
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        if (result) free(result);
        if (dataBuff) free(dataBuff);
        return;
    }
    memset(dataBuff, 0, infoCount * MAX_OPER_NAME_LENGTH);
    for (int i = 0; i < infoCount; i++) {
        result[i] = &dataBuff[i * MAX_OPER_NAME_LENGTH];
    }

    for (int i = 0; i < infoCount; i++) {
        strncpy(result[i], infoList[i].plmn_id, MAX_OPER_NAME_LENGTH-1);

        logD(LOG_TAG, "handleGetSuggestedPlmnListRequest:"
            "info[%d] - state[%d], plmn[%s], lac[%s], act[%d]",
            i, infoList[i].state, infoList[i].plmn_id, infoList[i].lac, infoList[i].act);
    }

    response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_SUCCESS, RfxStringsData(result, infoCount), msg, false);
    responseToTelCore(response);

    if (result) free(result);
    if (dataBuff) free(dataBuff);
}


void RmmNwNrtRequestHandler::handleNetworkScanEvent() {
    if (sPlmnListAbort) {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
        return;
    }

    sp<RfxMclMessage> urc;
    bool success = false;
    RIL_NetworkScanResult* result = (RIL_NetworkScanResult*)malloc(sizeof(RIL_NetworkScanResult));
    if (result == NULL) {
        logE(LOG_TAG, "handleNetworkScanEvent insufficient memory");
        return;
    }
    memset(result, 0, sizeof(RIL_NetworkScanResult));
    if (isAPInCall()) {
        goto error;
    }

    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NORMAL;
    if (sIncrementalSupport) {
        if (getPartialPlmnList(result) < 0) goto error;
    } else {
        if (getFullPlmnList(result) < 0) goto error;
    }
    success = true;

error:
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);

    releaseNetworkScanResultData(result);
    if (result) free(result);

    if (!success && !sPlmnListAbort) {
        RIL_NetworkScanResult* resp = (RIL_NetworkScanResult*)malloc(sizeof(RIL_NetworkScanResult));
        if (resp == NULL) {
            logE(LOG_TAG, "handleNetworkScanEvent insufficient memory");
            return;
        }
        memset(resp, 0, sizeof(RIL_NetworkScanResult));
        resp->status = COMPLETE;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                m_slot_id, RfxNetworkScanResultData(resp, sizeof(RIL_NetworkScanResult)));
        responseToTelCore(urc);
        free(resp);
    }
    sPlmnListAbort = false;
}

int RmmNwNrtRequestHandler::getFullPlmnList(RIL_NetworkScanResult* result) {
    uint16_t read = 0;
    int i, j;

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_PLMN_LIST_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

    if (cnfData->getData() == NULL
        || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        return -1;
    }

    int infoCount = cnfData->getMipcUint8Val(
            MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_COUNT, 0xff);
    if (infoCount == 0xff) {
        logE(LOG_TAG, "getFullPlmnList: list count invalid");
        return -1;
    }

    mipc_nw_extend_provider_struct4 *infoList = (mipc_nw_extend_provider_struct4 *)cnfData->getMipcVal(
            MIPC_NW_GET_PLMN_LIST_CNF_T_EXTEND_INFO_LIST, &read);
    if (read <= 0 || infoList == NULL
        || read % sizeof(*infoList) != 0
        || read / sizeof(*infoList) != infoCount) {
        logE(LOG_TAG, "getFullPlmnList: MIPC invalid data,"
                "read[%d] infoList[%d]", read, (int)sizeof(mipc_nw_extend_provider_struct4));
        return -1;
    }

    result->network_infos = (RIL_CellInfo_v12*)calloc(infoCount, sizeof(RIL_CellInfo_v12));
    if (result->network_infos == NULL) {
        logE(LOG_TAG, "getFullPlmnList: Insufficient memory");
        return -1;
    }

    for (i = 0, j = 0; i < infoCount; i++) {

        if (infoList[i].act == 11
            && 0xfffffffe == strtol(infoList[i].lac, NULL, 16)) {
            logD(LOG_TAG, "Skip this NSA's NR cell");
            continue;
        }
        if (infoList[i].act != 0
            && infoList[i].act != 2
            && infoList[i].act != 7
            && infoList[i].act != 10
            && infoList[i].act != 11
            && infoList[i].act != 12
            && infoList[i].act != 13) {
            logD(LOG_TAG, "getFullPlmnList: Invalid act");
            return -1;
        }

        RIL_CellInfo_v12 *info = &result->network_infos[j];

        if (fillCellIdentity(info, &infoList[i]) < 0) {
            return -1;
        }

        if (fillSignalStrength(info, &infoList[i]) < 0) {
            return -1;

        }

        switch(infoList[i].stat) {
        case MIPC_NW_PROVIDER_STATE_CURRENT:
            info->registered = 1;
            break;
        case MIPC_NW_PROVIDER_STATE_UNKNOWN:
        case MIPC_NW_PROVIDER_STATE_AVAILABLE:
        case MIPC_NW_PROVIDER_STATE_FORBIDDEN:
            info->registered = 0;
            break;
        default:
            logE(LOG_TAG, "The %dth <stat> invalid: %d", i, infoList[i].stat);
            return -1;
        }

        info->connectionStatus = (infoList[i].register_state == 1) ?
                PRIMARY_SERVING : NONE_SERVING;

        pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
        if (info->registered && info->cellInfoType == RIL_CELL_INFO_TYPE_LTE
            && isInService(sPsRegStateCache[m_slot_id]->registration_state)
            && sPsRegStateCache[m_slot_id]->dcnr_restricted == 0
            && sPsRegStateCache[m_slot_id]->endc_sib == 2) {
            info->CellInfo.lte.cellConfig.isEndcAvailable = 1;
        } else {
            info->CellInfo.lte.cellConfig.isEndcAvailable = 0;
        }
        pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);

        info->timeStamp = infoList[i].timestamp;

        j++;
        result->network_infos_length++;
    }
    if (!sPlmnListAbort) {
        result->status = COMPLETE;

        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                m_slot_id, RfxNetworkScanResultData(result, sizeof(RIL_NetworkScanResult)));
        responseToTelCore(urc);
    }

    return 0;
}

int RmmNwNrtRequestHandler::getPartialPlmnList(RIL_NetworkScanResult* result) {
    uint16_t read = 0;
    const uint8_t ratList[3] = {3, 1, 0};
    const uint8_t plmnCount = 50;
    const uint8_t guardTimer = 0;
    const int ratsCount = sizeof(ratList)/sizeof(ratList[0]);
    int i, j, k;
    sp<RfxMipcData> reqData;
    sp<RfxMipcData> cnfData;

    for (i = 0; i < ratsCount; i++) {
        reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ, m_slot_id);
        reqData->addMipcTlvUint8(
                MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_RAT, ratList[i]);
        reqData->addMipcTlvUint8(
                MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_NUM, plmnCount);
        reqData->addMipcTlvUint8(
                MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_TIMER, guardTimer);

        cnfData = callToMipcMsgSync(reqData, 15*60*1000/*timeout 15m*/);

        if (cnfData->getData() == NULL
                || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
            return -1;
        }
        if (isAPInCall() == true || sPlmnListAbort) {
            return -1;
        }

        int infoCount = cnfData->getMipcUint8Val(
                MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_COUNT, 0xff);
        if (infoCount == 0xff) {
            logE(LOG_TAG, "getPartialPlmnList: list count invalid");
            return -1;
        }
        mipc_nw_suggested_struct4 *infoList = (mipc_nw_suggested_struct4 *)cnfData->getMipcVal(
                MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_LIST, &read);
        if (read <= 0 || infoList == NULL
                || read % sizeof(*infoList) != 0
                || read / sizeof(*infoList) != infoCount) {
            logE(LOG_TAG, "getPartialPlmnList: MIPC invalid data,"
                    "read[%d] infoList[%d]", read, (int)sizeof(mipc_nw_suggested_struct4));
            return -1;
        }
        result->network_infos = (RIL_CellInfo_v12*)calloc(infoCount, sizeof(RIL_CellInfo_v12));
        if (result->network_infos == NULL) {
            logE(LOG_TAG, "getPartialPlmnList: Insufficient memory");
            return -1;
        }

        for (j = 0, k = 0; j < infoCount; i++) {

            if (infoList[j].act == 11
                && 0xfffffffe == strtol(infoList[i].lac, NULL, 16)) {
                logD(LOG_TAG, "Skip this NSA's NR cell");
                continue;
            }

            RIL_CellInfo_v12 *info = &result->network_infos[k];

            switch(infoList[j].state) {
                case MIPC_NW_PROVIDER_STATE_CURRENT:
                    info->registered = 1;
                    break;

                case MIPC_NW_PROVIDER_STATE_UNKNOWN:
                case MIPC_NW_PROVIDER_STATE_AVAILABLE:
                case MIPC_NW_PROVIDER_STATE_FORBIDDEN:
                    info->registered = 0;
                    break;

                default:
                    logE(LOG_TAG, "getPartialPlmnList: [%d]th invalid <state>: %d",
                            j, infoList[i].state);
                    return -1;
            }

            switch(infoList[j].act) {
                case 0:
                    info->cellInfoType = RIL_CELL_INFO_TYPE_GSM;
                    break;
                case 2:
                    info->cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
                    break;
                case 7:    //for  LTE
                case 10:
                    info->cellInfoType = RIL_CELL_INFO_TYPE_LTE;
                    break;
                case 11:
                case 12:
                case 13:
                    info->cellInfoType = RIL_CELL_INFO_TYPE_NR;
                    break;
                default:
                    logE(LOG_TAG, "getPartialPlmnList: [%d]th Invalid act", j);
                    return -1;
            }

            PlmnMatchResult matchResult;
            memset(&matchResult, 0, sizeof(matchResult));
            if (matchPlmn(&matchResult,
                    infoList[j].plmn_id, infoList[j].lac, infoList[j].act,
                    infoList[j].network_long_name, infoList[j].network_short_name) < 0)
                return -1;

            switch(info->cellInfoType) {

            case RIL_CELL_INFO_TYPE_GSM:

                info->CellInfo.gsm.cellIdentityGsm.mcc = matchResult.mcc;
                info->CellInfo.gsm.cellIdentityGsm.mnc = matchResult.mnc;
                info->CellInfo.gsm.cellIdentityGsm.mnc_len = matchResult.mncLen;
                info->CellInfo.gsm.cellIdentityGsm.lac = matchResult.lacTac;
                info->CellInfo.gsm.cellIdentityGsm.cid = INT_MAX;
                info->CellInfo.gsm.cellIdentityGsm.bsic = 0xff;
                info->CellInfo.gsm.cellIdentityGsm.arfcn = INT_MAX;
                info->CellInfo.gsm.cellIdentityGsm.operName.long_name = matchResult.longAlpha;
                info->CellInfo.gsm.cellIdentityGsm.operName.short_name = matchResult.shortAlpha;
                info->CellInfo.gsm.signalStrengthGsm.signalStrength = 99;
                info->CellInfo.gsm.signalStrengthGsm.bitErrorRate = 99;
                info->CellInfo.gsm.signalStrengthGsm.timingAdvance = 0;

                break;

            case RIL_CELL_INFO_TYPE_WCDMA:

                info->CellInfo.wcdma.cellIdentityWcdma.mcc = matchResult.mcc;
                info->CellInfo.wcdma.cellIdentityWcdma.mnc = matchResult.mnc;
                info->CellInfo.wcdma.cellIdentityWcdma.mnc_len = matchResult.mncLen;
                info->CellInfo.wcdma.cellIdentityWcdma.lac = matchResult.lacTac;
                info->CellInfo.wcdma.cellIdentityWcdma.cid = INT_MAX;
                info->CellInfo.wcdma.cellIdentityWcdma.psc = INT_MAX;
                info->CellInfo.wcdma.cellIdentityWcdma.uarfcn = INT_MAX;
                info->CellInfo.wcdma.cellIdentityWcdma.operName.long_name = matchResult.longAlpha;
                info->CellInfo.wcdma.cellIdentityWcdma.operName.short_name = matchResult.shortAlpha;
                info->CellInfo.wcdma.signalStrengthWcdma.signalStrength = 99;
                info->CellInfo.wcdma.signalStrengthWcdma.bitErrorRate = 99;
                info->CellInfo.wcdma.signalStrengthWcdma.rscp = 255;
                info->CellInfo.wcdma.signalStrengthWcdma.ecno = 255;

                break;

            case RIL_CELL_INFO_TYPE_LTE:

                info->CellInfo.lte.cellIdentityLte.mcc = matchResult.mcc;
                info->CellInfo.lte.cellIdentityLte.mnc = matchResult.mnc;
                info->CellInfo.lte.cellIdentityLte.mnc_len = matchResult.mncLen;
                info->CellInfo.lte.cellIdentityLte.tac = matchResult.lacTac;
                info->CellInfo.lte.cellIdentityLte.ci = INT_MAX;
                info->CellInfo.lte.cellIdentityLte.pci = INT_MAX;
                info->CellInfo.lte.cellIdentityLte.earfcn = INT_MAX;
                info->CellInfo.lte.cellIdentityLte.bandwidth = 0;
                info->CellInfo.lte.cellIdentityLte.operName.long_name = matchResult.longAlpha;
                info->CellInfo.lte.cellIdentityLte.operName.short_name = matchResult.shortAlpha;
                info->CellInfo.lte.signalStrengthLte.signalStrength = 99;
                info->CellInfo.lte.signalStrengthLte.rsrp = INT_MAX;
                info->CellInfo.lte.signalStrengthLte.rsrq = INT_MAX;
                info->CellInfo.lte.signalStrengthLte.rssnr = INT_MAX;
                info->CellInfo.lte.signalStrengthLte.cqi = INT_MAX;
                info->CellInfo.lte.signalStrengthLte.timingAdvance = INT_MAX;
                info->CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;

                break;

            case RIL_CELL_INFO_TYPE_NR:

                info->CellInfo.nr.cellidentity.mcc = matchResult.mcc;
                info->CellInfo.nr.cellidentity.mnc = matchResult.mnc;
                info->CellInfo.nr.cellidentity.mnc_len = matchResult.mncLen;
                info->CellInfo.nr.cellidentity.tac = matchResult.lacTac;
                info->CellInfo.nr.cellidentity.nci = LONG_MAX;
                info->CellInfo.nr.cellidentity.pci = INT_MAX;
                info->CellInfo.nr.cellidentity.nrarfcn = INT_MAX;
                info->CellInfo.nr.cellidentity.operName.long_name = matchResult.longAlpha;
                info->CellInfo.nr.cellidentity.operName.short_name = matchResult.shortAlpha;
                info->CellInfo.nr.signalStrength.ssRsrp = INT_MAX;
                info->CellInfo.nr.signalStrength.ssRsrq = INT_MAX;;
                info->CellInfo.nr.signalStrength.ssSinr = INT_MAX;
                info->CellInfo.nr.signalStrength.csiRsrp = INT_MAX;
                info->CellInfo.nr.signalStrength.csiRsrq = INT_MAX;
                info->CellInfo.nr.signalStrength.csiSinr =  INT_MAX;
                info->CellInfo.nr.signalStrength.csiCqiTableIndex = INT_MAX;
                info->CellInfo.nr.signalStrength.csiCqiReportCount = 0;

                break;

            default:
                logE(LOG_TAG, "should not get here");
                if (matchResult.longAlpha) free(matchResult.longAlpha);
                if (matchResult.shortAlpha) free(matchResult.shortAlpha);
                break;
            }

            pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
            if (info->registered && info->cellInfoType == RIL_CELL_INFO_TYPE_LTE
                    && isInService(sPsRegStateCache[m_slot_id]->registration_state)
                    && sPsRegStateCache[m_slot_id]->dcnr_restricted == 0
                    && sPsRegStateCache[m_slot_id]->endc_sib == 2) {
                info->CellInfo.lte.cellConfig.isEndcAvailable = 1;
            } else {
                info->CellInfo.lte.cellConfig.isEndcAvailable = 0;
            }
            pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);

            k++;
            result->network_infos_length++;
        }
        if (i < ratsCount -1) {
            result->status = PARTIAL;

            sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                    m_slot_id, RfxNetworkScanResultData(result, sizeof(RIL_NetworkScanResult)));
            responseToTelCore(urc);
        } else {
            result->status = COMPLETE;

            sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                    m_slot_id, RfxNetworkScanResultData(result, sizeof(RIL_NetworkScanResult)));
            responseToTelCore(urc);
        }
        releaseNetworkScanResultData(result);
    }

    return 0;
}

int RmmNwNrtRequestHandler::fillCellIdentity(RIL_CellInfo_v12 *info,
        const mipc_nw_extend_provider_struct4 *plmnInfo) {
    uint64_t cellId;
    PlmnMatchResult matchResult;

    memset(&matchResult, 0, sizeof(matchResult));
    if (matchPlmn(&matchResult,
            plmnInfo->oper_numeric_name, plmnInfo->lac, plmnInfo->act,
            plmnInfo->oper_long_name, plmnInfo->oper_short_name) < 0)
        return -1;

    cellId = strtoull(plmnInfo->cell_id, NULL, 16);

    switch(plmnInfo->act) {

        case 0:

            info->cellInfoType = RIL_CELL_INFO_TYPE_GSM;
            info->CellInfo.gsm.cellIdentityGsm.mcc = matchResult.mcc;
            info->CellInfo.gsm.cellIdentityGsm.mnc = matchResult.mnc;
            info->CellInfo.gsm.cellIdentityGsm.mnc_len = matchResult.mncLen;
            info->CellInfo.gsm.cellIdentityGsm.lac = matchResult.lacTac;
            info->CellInfo.gsm.cellIdentityGsm.cid =
                    (cellId <= 65535 ? (int)cellId : INT_MAX);
            info->CellInfo.gsm.cellIdentityGsm.bsic =
                    (plmnInfo->bsic_psc_cpid_pci >= 0 && plmnInfo->bsic_psc_cpid_pci < 64) ?
                    plmnInfo->bsic_psc_cpid_pci : 0xff;
            info->CellInfo.gsm.cellIdentityGsm.arfcn =
                    (plmnInfo->freq <= 65535) ? plmnInfo->freq : INT_MAX;
            info->CellInfo.gsm.cellIdentityGsm.operName.long_name = matchResult.longAlpha;
            info->CellInfo.gsm.cellIdentityGsm.operName.short_name = matchResult.shortAlpha;

            break;

        case 2:

            info->cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
            info->CellInfo.wcdma.cellIdentityWcdma.mcc = matchResult.mcc;
            info->CellInfo.wcdma.cellIdentityWcdma.mnc = matchResult.mnc;
            info->CellInfo.wcdma.cellIdentityWcdma.mnc_len = matchResult.mncLen;
            info->CellInfo.wcdma.cellIdentityWcdma.lac = matchResult.lacTac;
            info->CellInfo.wcdma.cellIdentityWcdma.cid =
                    (cellId <= 268435455 ? (int)cellId : INT_MAX);
            info->CellInfo.wcdma.cellIdentityWcdma.psc =
                    (plmnInfo->bsic_psc_cpid_pci >= 0 && plmnInfo->bsic_psc_cpid_pci < 512) ?
                    plmnInfo->bsic_psc_cpid_pci : INT_MAX;
            info->CellInfo.wcdma.cellIdentityWcdma.uarfcn =
                    (plmnInfo->freq <= 65535) ? plmnInfo->freq : INT_MAX;
            info->CellInfo.wcdma.cellIdentityWcdma.operName.long_name = matchResult.longAlpha;
            info->CellInfo.wcdma.cellIdentityWcdma.operName.short_name = matchResult.shortAlpha;

            break;

        case 7:
        case 10:

            info->cellInfoType = RIL_CELL_INFO_TYPE_LTE;
            info->CellInfo.lte.cellIdentityLte.mcc = matchResult.mcc;
            info->CellInfo.lte.cellIdentityLte.mnc = matchResult.mnc;
            info->CellInfo.lte.cellIdentityLte.mnc_len = matchResult.mncLen;
            info->CellInfo.lte.cellIdentityLte.tac = matchResult.lacTac;
            info->CellInfo.lte.cellIdentityLte.ci =
                    (cellId <= 268435455 ? (int)cellId : INT_MAX);
            info->CellInfo.lte.cellIdentityLte.pci =
                    (plmnInfo->bsic_psc_cpid_pci >= 0 && plmnInfo->bsic_psc_cpid_pci <= 503) ?
                    plmnInfo->bsic_psc_cpid_pci : INT_MAX;
            info->CellInfo.lte.cellIdentityLte.earfcn =
                    (plmnInfo->freq <= 262143) ? plmnInfo->freq : INT_MAX;
            info->CellInfo.lte.cellIdentityLte.bandwidth = 0;
            info->CellInfo.lte.cellIdentityLte.operName.long_name = matchResult.longAlpha;
            info->CellInfo.lte.cellIdentityLte.operName.short_name = matchResult.shortAlpha;

            break;

        case 11:
        case 12:
        case 13:

            info->cellInfoType = RIL_CELL_INFO_TYPE_NR;
            info->CellInfo.nr.cellidentity.mcc = matchResult.mcc;
            info->CellInfo.nr.cellidentity.mnc = matchResult.mnc;
            info->CellInfo.nr.cellidentity.mnc_len = matchResult.mncLen;
            info->CellInfo.nr.cellidentity.tac = matchResult.lacTac;
            info->CellInfo.nr.cellidentity.nci =
                    (cellId <= 68719476735 ? cellId : LONG_MAX);
            info->CellInfo.nr.cellidentity.pci =
                    (plmnInfo->bsic_psc_cpid_pci >= 0 && plmnInfo->bsic_psc_cpid_pci <= 1007) ?
                    plmnInfo->bsic_psc_cpid_pci : INT_MAX;
            info->CellInfo.nr.cellidentity.nrarfcn =
                    (plmnInfo->freq <= 3279165) ? plmnInfo->freq : INT_MAX;
            info->CellInfo.nr.cellidentity.operName.long_name = matchResult.longAlpha;
            info->CellInfo.nr.cellidentity.operName.short_name = matchResult.shortAlpha;

            break;

        default:
            logE(LOG_TAG, "should not get here");
            if (matchResult.longAlpha) free(matchResult.longAlpha);
            if (matchResult.shortAlpha) free(matchResult.shortAlpha);
            break;
    }

    return 0;
}

int RmmNwNrtRequestHandler::fillSignalStrength(RIL_CellInfo_v12 *info,
        const mipc_nw_extend_provider_struct4 *plmnInfo) {

    int level;
    int sig1, sig2, sig3, sig4, sig5;

    /* <sig1>
     * For GSM - (0-63, 99) Based on 27.007 8.69
     * For WCDMA - (0-31, 99) Based on 27.007 8.5
     * For TDSCDMA - (25 - 120) based on (dbm x -1)
     * For LTE - (0-31, 99) Based on 27.007 8.5
     * For NR - (0-255) Based on 27.007 8.69 <ss_rsrq>
     */
    sig1 = (int)plmnInfo->csq_rssi;

    /* <sig2>
     * For GSM/WCDMA/LTE - RSSI in qdbM
     * For TDSCDMA - RSCP in qbdm
     * For NR - (0-255) Based on 27.007 8.69 <ss_rsrp>
     */
    sig2 = plmnInfo->sig2;

    /* <sig3>
     * RSCP in qdbM for WCDMA
     * RSRP in qdbM for LTE
     * SS-RSRQ in qdb for NR
     */
    sig3 = plmnInfo->sig3;

    /* <sig4>
     * ECNO in qdbM for WCDMA
     * RSRQ in qdbM for LTE
     * SS-RSRP in qdbm for NR
     */
    sig4 = plmnInfo->sig4;

    /* <sig5>
     * RSSNR in qdbM for LTE
     * SS-SINR in qdb for NR
     */
    sig5 = plmnInfo->sig5;

    switch(plmnInfo->act) {

        case 0:

            level = (sig1 >> 1) + 1;
            info->CellInfo.gsm.signalStrengthGsm.signalStrength =
                    (level >= 0 && level <= 31) ? level : 99;
            info->CellInfo.gsm.signalStrengthGsm.bitErrorRate =
                    (plmnInfo->bit_error_rat <= 7) ? plmnInfo->bit_error_rat : 99;
            info->CellInfo.gsm.signalStrengthGsm.timingAdvance =(int)plmnInfo->timing_advance;

            break;

        case 2:

            info->CellInfo.wcdma.signalStrengthWcdma.signalStrength =
                    sig1 <= 31 ? sig1 : 99;
            info->CellInfo.wcdma.signalStrengthWcdma.bitErrorRate =
                    (plmnInfo->bit_error_rat <= 7) ? plmnInfo->bit_error_rat : 99;
            info->CellInfo.wcdma.signalStrengthWcdma.rscp =
                    (sig3 != INT_MAX && sig3 != 32767) ? ((sig3>>2) + 120) : 255;
            sig4 = (sig4 >> 2) + 49;
            info->CellInfo.wcdma.signalStrengthWcdma.ecno =
                    (sig4 >= 0 && sig4 <= 49) ? sig4 : 255;

            break;

        case 7:
        case 10:

            info->CellInfo.lte.signalStrengthLte.signalStrength =
                    sig1 <= 31 ? sig1 : 99;
            info->CellInfo.lte.signalStrengthLte.rsrp =
                    sig3 != 32767 ? (sig3 >> 2) * (-1) : INT_MAX;
            info->CellInfo.lte.signalStrengthLte.rsrq =
                    sig4 != INT_MAX ? (sig4 >> 2)*(-1) : INT_MAX;
            info->CellInfo.lte.signalStrengthLte.rssnr =
                    sig5 != INT_MAX ? (sig5 >> 2)*10 : INT_MAX;
            info->CellInfo.lte.signalStrengthLte.cqi =
                    plmnInfo->cqi <= 15 ? plmnInfo->cqi : INT_MAX;
            info->CellInfo.lte.signalStrengthLte.timingAdvance =
                    plmnInfo->timing_advance != INT_MAX ? plmnInfo->timing_advance : INT_MAX;
            info->CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;

            break;

        case 11:
        case 12:
        case 13:

            sig3 = (sig3 >> 2) * (-1);
            info->CellInfo.nr.signalStrength.ssRsrp =
                    (sig3 >= 44 && sig3 <= 140) ? sig3 : INT_MAX;
            sig4 = (sig4 >> 2) * (-1);
            info->CellInfo.nr.signalStrength.ssRsrq =
                    (sig4 >= 3 && sig4 <= 20) ? sig4 : INT_MAX;;
            sig5 = (sig5 >> 2) * (-1);
            info->CellInfo.nr.signalStrength.ssSinr =
                    (sig5 >= -23 && sig5 <= 40) ? sig5 : INT_MAX;
            info->CellInfo.nr.signalStrength.csiRsrp = INT_MAX;
            info->CellInfo.nr.signalStrength.csiRsrq = INT_MAX;
            info->CellInfo.nr.signalStrength.csiSinr =  INT_MAX;
            info->CellInfo.nr.signalStrength.csiCqiTableIndex = INT_MAX;
            info->CellInfo.nr.signalStrength.csiCqiReportCount =0;

            break;

        default:
            logE(LOG_TAG, "should not get here");
            break;
    }

    return 0;
}


int RmmNwNrtRequestHandler::matchPlmn(PlmnMatchResult *result,
        const char *numeric, const char *lac, unsigned int act, const char *longName, const char *shortName) {
    char *longAlpha, *shortAlpha;
    int len = strlen(numeric);
    int matchResult;

    if (len < 5 || len > 6)
        return -1;

    result->mcc = 0;
    for (int i = 0; i < 3; i++) {
        result->mcc += pow(10, 2-i) * (numeric[i]-'0');
    }
    result->mncLen = (len == 5) ? 2 : 3;
    result->mnc = 0;
    for (int i = 0; i < result->mncLen; i++) {
        result->mnc += pow(10, result->mncLen - i - 1) * (numeric[i+3] - '0');
    }
    if (result->mcc < 0 || result->mcc > 999 || result->mnc < 0 || result->mnc > 999) {
        logE(LOG_TAG, "numeric code is incorrect");
        return -1;
    }

    result->lacTac = (int)strtol(lac, NULL, 16);
    if (result->lacTac < 0 || result->lacTac > 0xffffff) {
        logE(LOG_TAG, "invalid lacTac[%x]", result->lacTac);
        return -1;
    }

    result->rat = (int)act;

    longAlpha = (char *)malloc(MAX_OPER_NAME_LENGTH);
    if (longAlpha == NULL) {
        logE(LOG_TAG, "insufficient memory");
        return -1;
    }
    shortAlpha = (char *)malloc(MAX_OPER_NAME_LENGTH);
    if (shortAlpha == NULL) {
        logE(LOG_TAG, "insufficient memory");
        free(longAlpha);
        return -1;
    }

    if (isOp07Support()) {
        matchResult = matchPlmnName(longAlpha, shortAlpha,
                numeric, result->lacTac, result->rat, MATCH_EONS|MATCH_TS25);
    } else {
        matchResult = matchPlmnName(longAlpha, shortAlpha,
                numeric, result->lacTac, result->rat);
    }
    if (matchResult == 0) {
        strncpy(longAlpha, longName, MAX_DATA_SIZE);
        longAlpha[MAX_DATA_SIZE] = '\0';
        strncpy(shortAlpha, shortName, MAX_DATA_SIZE);
        shortAlpha[MAX_DATA_SIZE] = '\0';
    }
    result->longAlpha = longAlpha;
    result->shortAlpha = shortAlpha;

    return 0;
}
