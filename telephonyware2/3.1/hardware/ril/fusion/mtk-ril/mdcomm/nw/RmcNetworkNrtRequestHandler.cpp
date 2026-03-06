/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2019. All rights reserved.
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

#include "RmcNetworkNrtRequestHandler.h"
#include "rfx_properties.h"
#include "ViaBaseHandler.h"
#include "RfxViaUtils.h"
#include <libmtkrilutils.h>

static const int request[] = {
    RFX_MSG_REQUEST_GET_FEMTOCELL_LIST,
    RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS,
    RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT,
    RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN
};

static const int events[] = {
    RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN
};

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxNetworkScanData, RfxVoidData, RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_GET_FEMTOCELL_LIST);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcNetworkNrtRequestHandler, RIL_CMD_PROXY_8);

RmcNetworkNrtRequestHandler::RmcNetworkNrtRequestHandler(int slot_id, int channel_id) :
        RmcNetworkHandler(slot_id, channel_id),
        mPlmn_list_format(0) {
    int err;
    sp<RfxAtResponse> p_response;
    logV(LOG_TAG, "%s[%d] start", __FUNCTION__, slot_id);
    m_slot_id = (unsigned int) slot_id;
    m_channel_id = channel_id;
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    registerToHandleEvent(events, sizeof(events)/sizeof(int));

    p_response = atSendCommand("AT+COPS=3,3");
    err = p_response->getError();
    if (err >= 0 || p_response->getSuccess() != 0) {
        mPlmn_list_format = 1;
    }

}

RmcNetworkNrtRequestHandler::~RmcNetworkNrtRequestHandler() {
}

void RmcNetworkNrtRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    // logD(LOG_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(msg->getId()));
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS:
            requestQueryAvailableNetworks(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT:
            requestQueryAvailableNetworksWithAct(msg);
            break;
        case RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN:
            requestStartNetworkScan(msg);
            break;
        case RFX_MSG_REQUEST_GET_FEMTOCELL_LIST:
            requestGetFemtocellList(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}


void RmcNetworkNrtRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    switch (id) {
        case RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN:
            if (at_eplwss_support) {
                handleSimulatedNetworkScanResultByRat();
            } else {
                handleSimulatedNetworkScan();
            }
            break;
        default:
            logE(LOG_TAG, "onHandleEvent, should not be here");
            break;
    }
}

void RmcNetworkNrtRequestHandler::requestQueryAvailableNetworks(const sp<RfxMclMessage>& msg) {
    int err = 0, len = 0, i = 0, j = 0, k = 0, num = 0, num_filter = 0, rat = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    char **response = NULL, **response_filter = NULL;
    char *tmp, *block_p = NULL;
    char *lacStr = NULL;
    unsigned int lac = 0;

    // logD(LOG_TAG, "requestQueryAvailableNetworks set plmnListOngoing flag");
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NORMAL;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);
    p_response = atSendCommandSingleline("AT+COPS=?", "+COPS:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    line = p_response->getIntermediates();
    // count the number of operator
    tmp = line->getLine();
    len = strlen(tmp);
    for(i = 0, num = 0, num_filter = 0; i < len ; i++ ) {
        // here we assume that there is no nested ()
        if (tmp[i] == '(') {
            num++;
            num_filter++;
        } else if (tmp[i] == ',' && tmp[i+1] == ',') {
            break;
        }
    }

    // +COPS: (2,"Far EasTone","FET","46601",0),(...),...,,(0, 1, 3),(0-2)
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    response = (char **) calloc(1, sizeof(char*) * num *4); // for string, each one is 20 bytes
    block_p = (char *) calloc(1, num* sizeof(char)*4*MAX_OPER_NAME_LENGTH);
    lacStr = (char *) calloc(1, num* sizeof(char)*4+1);

    if (response == NULL || block_p == NULL || lacStr == NULL) {
        logE(LOG_TAG, "requestQueryAvailableNetworks calloc fail");
        goto error;
    }

    for (i = 0, j=0 ; i < num ; i++, j+=4) {
        /* get "(<stat>" */
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        response[j+0] = &block_p[(j+0)*MAX_OPER_NAME_LENGTH];
        response[j+1] = &block_p[(j+1)*MAX_OPER_NAME_LENGTH];
        response[j+2] = &block_p[(j+2)*MAX_OPER_NAME_LENGTH];
        response[j+3] = &block_p[(j+3)*MAX_OPER_NAME_LENGTH];

        switch(tmp[1]) {
        case '0':
            err = sprintf(response[j+3], "unknown");
            break;
        case '1':
            err = sprintf(response[j+3], "available");
            break;
        case '2':
            err = sprintf(response[j+3], "current");
            break;
        case '3':
            err = sprintf(response[j+3], "forbidden");
            break;
        default:
            logE(LOG_TAG, "The %d-th <stat> is an invalid value!!!  : %d", i, tmp[1]);
            goto error;
        }
        if (err < 0) {
            logE(LOG_TAG, "sprintf stat fail");
        }

        /* get long name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
        err = sprintf(response[j+0], "%s", tmp);
        if (err < 0) {
            logE(LOG_TAG, "sprintf response[j+0] fail");
        }

        /* get short name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
        err = sprintf(response[j+1], "%s", tmp);
        if (err < 0) {
            logE(LOG_TAG, "sprintf response[j+1] fail");
        }

        /* get <oper> numeric code*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
        err = sprintf(response[j+2], "%s", tmp);
        if (err < 0) {
            logE(LOG_TAG, "sprintf response[j+2] fail");
        }

        // ALPS00353868 START
        /*plmn_list_format.  0: standard +COPS format , 1: standard +COPS format plus <lac> */
        if(mPlmn_list_format == 1) {
            /* get <lac> numeric code*/
                tmp = line->atTokNextstr(&err);
            if (err < 0) {
                logE(LOG_TAG, "No <lac> in +COPS response");
                goto error;
            }
            memcpy(&(lacStr[i*4]), tmp, 4);
            lac = (unsigned int) strtoul(tmp, NULL, 16);
        }
        // ALPS00353868 END

        /* get <AcT> 0 is "2G", 2 is "3G", 7 is "4G", 11 is "5G"*/
        rat = line->atTokNextint(&err);
        if (err < 0) goto error;

        len = strlen(response[j+2]);
        if (len == 5 || len == 6) {
            if (isOp08Support()) {
                // AT&T consider EONS/TS25 in result.
                err = getOperatorNamesFromNumericCodeByDisplay(
                        response[j+2], lac, response[j+0], response[j+1],
                        MAX_OPER_NAME_LENGTH, (DISPLAY_EONS | DISPLAY_TS25), rat);
            } else {
                err = getOperatorNamesFromNumericCode(
                        response[j+2], lac, response[j+0], response[j+1], MAX_OPER_NAME_LENGTH, rat);
            }
            if(err < 0) goto error;
        } else {
            logE(LOG_TAG, "The length of the numeric code is incorrect");
            goto error;
        }

        // check if this plmn is redundant
        for (k=0; k < j; k+=4)
        {
            // compare numeric
            if (0 == strcmp(response[j+2], response[k+2])) {
                if (0 == strcmp("current", response[j+3])) {
                    err = sprintf(response[k+3], "current");
                }
                response[j+0] = response[j+1] = response[j+2] = response[j+3] = (char *)"";
                num_filter--;
                break;
            }
        }
    }

    // filter the response
    response_filter = (char**)calloc(1, sizeof(char*) * num_filter * 4);
    if (NULL == response_filter) {
        logE(LOG_TAG, "malloc response_filter failed");
        goto error;
    }

    for (i=0, j=0, k=0; i < num; i++, j+=4) {
        if (0 < strlen(response[j+2])) {
            response_filter[k+0] = response[j+0];
            response_filter[k+1] = response[j+1];
            response_filter[k+2] = response[j+2];
            response_filter[k+3] = response[j+3];
            k += 4;
        }
    }

    logD(LOG_TAG, "requestQueryAvailableNetworks sucess, clear plmnListOngoing and plmnListAbort flag");
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    mPlmnListAbort = 0; /* always clear here to prevent race condition scenario */
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData(response_filter, num_filter*4), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    free(response);
    free(response_filter);
    free(block_p);
    free(lacStr);
    return;
error:
    // logE(LOG_TAG, "FREE!!");
    if (block_p) free(block_p);
    if (response) free(response);
    if (lacStr) free(lacStr);

    logE(LOG_TAG, "requestQueryAvailableNetworks must never return error when radio is on, plmnListAbort=%d",
            mPlmnListAbort);
    if (mPlmnListAbort == 1){
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_CANCELLED,
                RfxVoidData(), msg, false);
    } else {
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
                RfxVoidData(), msg, false);
    }
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    mPlmnListAbort =0; /* always clear here to prevent race condition scenario */
    // response to TeleCore
    responseToTelCore(resp);
}

void RmcNetworkNrtRequestHandler::requestQueryAvailableNetworksWithAct(const sp<RfxMclMessage>& msg) {
    int err = 0, len = 0, i = 0, j = 0, num = 0, rat = 0;
    char **response = NULL;
    char *tmp, *block_p = NULL;
    char *lacStr = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    unsigned int lac = 0;

    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NORMAL;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);
    p_response = atSendCommandSingleline("AT+COPS=?", "+COPS:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    line = p_response->getIntermediates();
    // count the number of operator
    tmp = line->getLine();
    len = strlen(tmp);
    for(i = 0, num = 0; i < len ; i++ ) {
        // here we assume that there is no nested ()
        if (tmp[i] == '(') {
            num++;
        } else if (tmp[i] == ',' && tmp[i+1] == ',') {
            break;
        }
    }

    // +COPS: (2,"Far EasTone","FET","46601",0),(...),...,,(0, 1, 3),(0-2)
    line->atTokStart(&err);
    if (err < 0) goto error;

    response = (char **) calloc(1, sizeof(char*) * num *6); // for string, each one is 25 bytes
    block_p = (char *) calloc(1, num* sizeof(char)*6*MAX_OPER_NAME_LENGTH);
    lacStr = (char *) calloc(1, num* sizeof(char)*4+1);

    if (response == NULL || block_p == NULL || lacStr == NULL) {
        logE(LOG_TAG, "requestQueryAvailableNetworksWithAct calloc fail");
        goto error;
    }

    for (i = 0, j=0 ; i < num ; i++, j+=6) {
        /* get "(<stat>" */
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        response[j+0] = &block_p[(j+0)*MAX_OPER_NAME_LENGTH];
        response[j+1] = &block_p[(j+1)*MAX_OPER_NAME_LENGTH];
        response[j+2] = &block_p[(j+2)*MAX_OPER_NAME_LENGTH];
        response[j+3] = &block_p[(j+3)*MAX_OPER_NAME_LENGTH];
        response[j+4] = &block_p[(j+4)*MAX_OPER_NAME_LENGTH];
        response[j+5] = &block_p[(j+5)*MAX_OPER_NAME_LENGTH];

        switch(tmp[1]) {
        case '0':
            err = sprintf(response[j+3], "unknown");
            break;
        case '1':
            err = sprintf(response[j+3], "available");
            break;
        case '2':
            err = sprintf(response[j+3], "current");
            break;
        case '3':
            err = sprintf(response[j+3], "forbidden");
            break;
        default:
            logE(LOG_TAG, "The %d-th <stat> is an invalid value!!! : %d", i, tmp[1]);
            goto error;
        }
        if (err < 0) {
            logE(LOG_TAG, "sprintf stat fail");
        }

        /* skip long name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* skip short name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* get <oper> numeric code*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
        err = sprintf(response[j+2], "%s", tmp);
        if (err < 0) {
            logE(LOG_TAG, "sprintf response[j+2] fail");
        }

        // ALPS00353868 START
        /*plmn_list_format.  0: standard +COPS format , 1: standard +COPS format plus <lac> */
        if (mPlmn_list_format == 1) {
            /* get <lac> numeric code*/
            tmp = line->atTokNextstr(&err);
            if (err < 0){
            logE(LOG_TAG, "No <lac> in +COPS response");
                goto error;
            }
            memcpy(&(lacStr[i*4]), tmp, 4);
            lac = (unsigned int) strtoul(tmp, NULL, 16);
            err = sprintf(response[j+4], "%s", tmp);
            if (err < 0) {
                logE(LOG_TAG, "sprintf response[j+4] fail");
            }
        }
        // ALPS00353868 END

        /* get <AcT> 0 is "2G", 2 is "3G", 7 is "4G", 11 is "5G" */
        rat = line->atTokNextint(&err);
        if (err < 0) goto error;

        len = strlen(response[j+2]);
        if (len == 5 || len == 6) {
            err = getOperatorNamesFromNumericCode(
                      response[j+2], lac, response[j+0], response[j+1], MAX_OPER_NAME_LENGTH, rat);
            if (err < 0) goto error;
        } else {
            logE(LOG_TAG, "The length of the numeric code is incorrect");
            goto error;
        }

        switch(rat) {
        case 0:
            err = sprintf(response[j+5], "2G");
            break;
        case 2:
            err = sprintf(response[j+5], "3G");
            break;
        case 7:    //for  LTE
            err = sprintf(response[j+5], "4G");
            break;
        case 11:
        case 12:
        case 13:
            err = sprintf(response[j+5], "5G");
            break;
        default:
            logE(LOG_TAG, "The %d-th <Act> is an invalid value!!! : %d", i, tmp[1]);
            goto error;
        }
        if (err < 0) {
            logE(LOG_TAG, "sprintf rat fail");
        }
    }

    logD(LOG_TAG, "requestQueryAvailableNetworksWithAct sucess, clear plmnListOngoing and plmnListAbort flag");
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    mPlmnListAbort =0; /* always clear here to prevent race condition scenario */
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData(response, num*6), msg, false);
    // response to TeleCore
    responseToTelCore(resp);

    free(response);
    free(block_p);
    free(lacStr);
    return;
error:
    logE(LOG_TAG, "FREE!!");
    if (block_p) free(block_p);
    if (response) free(response);
    if (lacStr) free(lacStr);
    logV(LOG_TAG, "requestQueryAvailableNetworksWithAct must never return error when radio is on, plmnListAbort=%d",
            mPlmnListAbort);
    if (mPlmnListAbort == 1){
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_CANCELLED,
                RfxVoidData(), msg, false);
    } else {
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    }
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    mPlmnListAbort =0; /* always clear here to prevent race condition scenario */
    responseToTelCore(resp);
}

void RmcNetworkNrtRequestHandler::requestStartNetworkScan(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    RIL_Errno ril_err = RIL_E_INVALID_ARGUMENTS;
    RIL_NetworkScanRequest* p_args = (RIL_NetworkScanRequest*) msg->getData()->getData();
    logV(LOG_TAG, "requestStartNetworkScan type=%d", p_args->type);
    logV(LOG_TAG, "requestStartNetworkScan interval=%d", p_args->interval);
    if (p_args->interval < 5 || p_args->interval > 300) goto error;
    logV(LOG_TAG, "requestStartNetworkScan specifiers_length=%d", p_args->specifiers_length);
    if (p_args->specifiers_length <= 0 || p_args->specifiers_length > MAX_RADIO_ACCESS_NETWORKS)
        goto error;
    for (size_t i = 0; i < (p_args->specifiers_length); i++) {
        logV(LOG_TAG, "requestStartNetworkScan p_args->specifiers[%zu]->radio_access_network=%d",
                i, p_args->specifiers[i].radio_access_network);
        logV(LOG_TAG, "requestStartNetworkScan p_args->specifiers[%zu]->bands_length=%d",
                i, p_args->specifiers[i].bands_length);
        for (size_t j = 0; j < (p_args->specifiers[i].bands_length); j++) {
            if (p_args->specifiers[i].radio_access_network == 0x01) {
                logV(LOG_TAG, "requestStartNetworkScan p_args->specifiers[%zu].bands.geran_bands[%zu]=%d",
                        i, j, p_args->specifiers[i].bands.geran_bands[j]);
            } else if (p_args->specifiers[i].radio_access_network == 0x02) {
                logV(LOG_TAG, "requestStartNetworkScan p_args->specifiers[%zu].bands.utran_bands[%zu]=%d",
                        i, j, p_args->specifiers[i].bands.utran_bands[j]);
            } else if (p_args->specifiers[i].radio_access_network == 0x03) {
                logV(LOG_TAG, "requestStartNetworkScan p_args->specifiers[%zu].bands.eutran_bands[%zu]=%d",
                    i, j, p_args->specifiers[i].bands.eutran_bands[j]);
            } else {
                logV(LOG_TAG, "Should not be here");
            }
        }
        logV(LOG_TAG, "requestStartNetworkScan p_args->specifiers[%zu]->channels_length=%d",
                i, p_args->specifiers[i].channels_length);
        for (size_t j = 0; j < (p_args->specifiers[i].channels_length); j++) {
            logV(LOG_TAG, "requestStartNetworkScan p_args->specifiers[%zu]->channels[%zu]=%d",
                    i, j, p_args->specifiers[i].channels[j]);
        }
    }
    // for VTS NW scan case.
    if (p_args->maxSearchTime == 0 && p_args->incrementalResultsPeriodicity == 0) {
        p_args->maxSearchTime = 3600;
        if (p_args->specifiers_length == 1 && p_args->specifiers[0].radio_access_network == 1) {
            p_args->maxSearchTime = 0;
        }
        p_args->incrementalResultsPeriodicity = 1;
    }
    logV(LOG_TAG, "requestStartNetworkScan maxSearchTime=%d", p_args->maxSearchTime);
    if (p_args->maxSearchTime < 70 || p_args->maxSearchTime > 3600) {
        if (p_args->maxSearchTime >= 60 && p_args->maxSearchTime < 70) ril_err = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }
    logV(LOG_TAG, "requestStartNetworkScan incrementalResults=%d", p_args->incrementalResults);
    logV(LOG_TAG, "requestStartNetworkScan incrementalResultsPeriodicity=%d", p_args->incrementalResultsPeriodicity);
    if (p_args->incrementalResultsPeriodicity < 1 || p_args->incrementalResultsPeriodicity > 10) goto error;
    logV(LOG_TAG, "requestStartNetworkScan mccMncs_length=%d", p_args->mccMncs_length);
    for (size_t i = 0; i < (p_args->mccMncs_length); i++) {
        logV(LOG_TAG, "requestStartNetworkScan mccMncs[%zu]=%s", i, p_args->mccMncs[i]);
    }

    if (isNetworkScanFullSupport()) {
        if (isAPInCall() == true) {
            ril_err = RIL_E_MODEM_ERR;
            goto error;
        }
        sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NETWORK_SCAN;
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);
        //format AT command
        String8 cmd("");
        sp<RfxAtResponse> p_response;
        cmd.append(String8::format("AT+ENWSCN=%d,%d,%d,%d,%d,%d", p_args->type, p_args->interval,
            p_args->maxSearchTime, p_args->incrementalResults, p_args->incrementalResultsPeriodicity,
            p_args->specifiers_length));

        for (size_t i = 0; i < (p_args->specifiers_length); i++) {
            if (p_args->specifiers[i].radio_access_network == 0x01) {
                cmd.append(String8::format(",%d,%d", 1, p_args->specifiers[i].bands_length));//gsm
                for (size_t j = 0; j < (p_args->specifiers[i].bands_length); j++) {
                    cmd.append(String8::format(",%d", p_args->specifiers[i].bands.geran_bands[j]));
                }
            } else if (p_args->specifiers[i].radio_access_network == 0x02) {
                cmd.append(String8::format(",%d,%d", 2, p_args->specifiers[i].bands_length));//umts
                for (size_t j = 0; j < (p_args->specifiers[i].bands_length); j++) {
                    cmd.append(String8::format(",%d", p_args->specifiers[i].bands.utran_bands[j]));
                }
            } else if (p_args->specifiers[i].radio_access_network == 0x03) {
                cmd.append(String8::format(",%d,%d", 4, p_args->specifiers[i].bands_length));//LTE
                for (size_t j = 0; j < (p_args->specifiers[i].bands_length); j++) {
                    cmd.append(String8::format(",%d", p_args->specifiers[i].bands.eutran_bands[j]));
                }
            } else if (p_args->specifiers[i].radio_access_network == 0x06) {
                cmd.append(String8::format(",%d,%d", 128, p_args->specifiers[i].bands_length));//NR
                for (size_t j = 0; j < (p_args->specifiers[i].bands_length); j++) {
                    cmd.append(String8::format(",%d", p_args->specifiers[i].bands.ngranBands[j]));
                }
            } else {
                logV(LOG_TAG, "Should not be here");
            }
            cmd.append(String8::format(",%d", p_args->specifiers[i].channels_length));
            for (size_t j = 0; j < (p_args->specifiers[i].channels_length); j++) {
                cmd.append(String8::format(",%d", p_args->specifiers[i].channels[j]));
            }
        }

        cmd.append(String8::format(",%d", p_args->mccMncs_length));
        for (size_t j = 0; j < p_args->mccMncs_length; j++) {
            cmd.append(String8::format(",\"%s\"", p_args->mccMncs[j]));
        }
        p_response = atSendCommand(cmd);
        if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
            ril_err = RIL_E_SUCCESS;
        } else {
            ril_err = RIL_E_GENERIC_FAILURE;
            sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
            getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
            mPlmnListAbort = 0; /* always clear here to prevent race condition scenario */
        }
    } else {
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxVoidData(), msg, false);
        responseToTelCore(resp);

        // send simulate network scan event
        sendEvent(RFX_MSG_EVENT_SIMULATE_NETWORK_SCAN, RfxVoidData(), RIL_CMD_PROXY_8, m_slot_id,
                -1, -1, ms2ns(200), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
        return;
    }

error:
    logE(LOG_TAG, "requestStartNetworkScan ril_err = %d", (int)ril_err);
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_err,
                RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;
}

int RmcNetworkNrtRequestHandler::simulateNetowrkScanByEXOPL(RIL_NetworkScanResult* result) {
    /*
    +EXOPL: <plmn_count>,
        (<stat>,<Long_alphanumeric_oper>,<short_alphanumeric_oper>,<numeric_oper>,<lac/tac>,<AcT>,
        <registered>,<timestamp_type>,<timestamp>,<connection_stat>,<cellid>,<freq>,<BSIC/PSC/CPID/PCI>,
        <sig1>,<sig2,<sig3>,<sig4>,<sig5>,<bit_error_rat>,<timing_advance>,<CQI>)
    */
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;

    int err = 0;
    int plmn_count = 0, stat = 0, lac_tac = 0, act = 0, registered = 0, timestamp_type = 0,
            timestamp = 0, connection_stat = 0, freq = 0, bsic_psc_cpid_pci = 0,
            sig1 = 0, sig2 = 0, sig3 = 0, sig4 = 0, sig5 = 0, bit_error_rat = 0,
            timing_advance = 0, cqi = 0;
    char* long_oper = NULL, *short_oper = NULL, *numeric_oper = NULL;
    unsigned long long cellid = 0;

    int aosp_gsm_sig = 0;

    int mcc = 0, mnc = 0, len = 0, mnc_len = 0, index = 0;
    char mccmnc[8] = {0};
    char longname[MAX_OPER_NAME_LENGTH] = {0};
    char shortname[MAX_OPER_NAME_LENGTH] = {0};
    char *tmp = NULL;

    /* init result */
    result->network_infos_length = 0;

    p_response = atSendCommandSingleline("AT+EXOPL=?", "+EXOPL:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    line = p_response->getIntermediates();
    if (line == NULL) goto error;
    line->atTokStart(&err);
    if (err < 0) goto error;

    // <plmn_count>
    plmn_count = line->atTokNextint(&err);
    if (err < 0) goto error;

    result->network_infos = (RIL_CellInfo_v12*) calloc(plmn_count, sizeof(RIL_CellInfo_v12));
    if (result->network_infos == NULL) goto error;
    memset(result->network_infos, 0, plmn_count*sizeof(RIL_CellInfo_v12));

    /* example
    (1,"46605","46605","46605","2AFA",7,0,2,1674641,0,"01BEC723",3775,41,25,-252,32767,-58,51,4294967295,2147483647,2147483647)
    */
    for (int i = 0; i < plmn_count ; i++) {
        // clear the tmp value for this round.
        memset(mccmnc, 0, 8);
        mcc = 0;
        mnc = 0;
        len = 0;
        mnc_len = 0;
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);

        /* <stat> 0 - Unknown; 1 - Available; 2 - Current; 3 - Forbidden */
        tmp = line->atTokNextstr(&err); // there is ( at the begining
        if (err < 0 || tmp[0] != '(') goto error;
        stat = atoi(&tmp[1]);

        /* <Long_alphanumeric_oper> */
        long_oper = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* <short_alphanumeric_oper> */
        short_oper = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* <numeric_oper> */
        numeric_oper = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* <lac/tac> */
        lac_tac = (unsigned int) line->atTokNexthexint(&err);
        if (err < 0) lac_tac = INT_MAX;

        /* <AcT> 0 - GSM; 2 - UMTS; 7 - LTE; 11 - NR */
        act = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <registered> 0 - Non serving; 1 - Serving */
        registered = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <timestamp_type> */
        timestamp_type = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <timestamp> */
        timestamp = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <connection_stat> 0 - None; 1 - Primary Serving Cell; 2 - Secondary serving cell */
        connection_stat = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <cellid> */
        tmp = line->atTokNextstr(&err);
        if (err < 0) cellid = INT_MAX;
        else cellid = strtoull(tmp, NULL, 16);

        /* <freq> ARFCN for 2G; UARFCN for 3G; EARFCN for 4G; NR-ARFCN for 5G */
        freq = line->atTokNextint(&err);
        if (err < 0) freq = INT_MAX;

        /* <BSIC/PSC/CPID/PCI> */
        bsic_psc_cpid_pci = line->atTokNextint(&err);
        if (err < 0) bsic_psc_cpid_pci = INT_MAX;

        /* <sig1>
         * For GSM - (0-63, 99) Based on 27.007 8.69
         * For WCDMA - (0-31, 99) Based on 27.007 8.5
         * For TDSCDMA - (25 - 120) based on (dbm x -1)
         * For LTE - (0-31, 99) Based on 27.007 8.5
         * For NR - (0-255) Based on 27.007 8.69 <ss_rsrq>
         */
        sig1 = line->atTokNextint(&err);
        if (err < 0) sig1 = INT_MAX;

        /* <sig2>
         * For GSM/WCDMA/LTE - RSSI in qdbM
         * For TDSCDMA - RSCP in qbdm
         * For NR - (0-255) Based on 27.007 8.69 <ss_rsrp>
         */
        sig2 = line->atTokNextint(&err);
        if (err < 0) sig2 = INT_MAX;

        /* <sig3>
         * RSCP in qdbM for WCDMA
         * RSRP in qdbM for LTE
         * SS-RSRQ in qdb for NR
         */
        sig3 = line->atTokNextint(&err);
        if (err < 0) sig3 = INT_MAX;

        /* <sig4>
         * ECNO in qdbM for WCDMA
         * RSRQ in qdbM for LTE
         * SS-RSRP in qdbm for NR
         */
        sig4 = line->atTokNextint(&err);
        if (err < 0) sig4 = INT_MAX;

        /* <sig5>
         * RSSNR in qdbM for LTE
         * SS-SINR in qdb for NR
         */
        sig5 = line->atTokNextint(&err);
        if (err < 0) sig5 = INT_MAX;

        /* <bit_error_rat> */
        bit_error_rat = line->atTokNextint(&err);
        if (err < 0 || bit_error_rat == 4294967295) bit_error_rat = INT_MAX;

        /* <timing_advance> */
        timing_advance = line->atTokNextint(&err);
        if (err < 0) timing_advance = INT_MAX;

        /* <CQI> */
        tmp = line->atTokNextstr(&err); // there is ) at the end
        if (err < 0) cqi = INT_MAX;
        else cqi = atoi(tmp);

        char* s_lac_tac = NULL, *s_cellid = NULL, *s_bsic_psc_cpid_pci = NULL;
        s_lac_tac = getMask(lac_tac);
        s_cellid = getMask(cellid);
        s_bsic_psc_cpid_pci = getMask(bsic_psc_cpid_pci);
        if (!(s_lac_tac == NULL || s_cellid == NULL || s_bsic_psc_cpid_pci == NULL)) {
            logD(LOG_TAG, "%d: %d,%s,%s,%s,%s,%d,%d,%d,%d,%d,%s,%d,%s,"
                "%d,%d,%d,%d,%d,%d,%d,%d",
                i, stat, (long_oper==NULL?"": long_oper), (short_oper==NULL?"": short_oper),
                (numeric_oper==NULL?"":numeric_oper), s_lac_tac, act, registered, timestamp_type,
                timestamp, connection_stat, s_cellid, freq, s_bsic_psc_cpid_pci, sig1, sig2, sig3,
                sig4, sig5, bit_error_rat, timing_advance, cqi);
        }
        if (s_lac_tac) free(s_lac_tac);
        if (s_cellid) free(s_cellid);
        if (s_bsic_psc_cpid_pci) free(s_bsic_psc_cpid_pci);

        /* parse end */

        if (act == 11 && lac_tac == 0xFFFFFE) {
            logD(LOG_TAG, "Skip this NSA's NR cell");
            continue;
        }

        len = strlen(numeric_oper);
        if (len == 5) {
            mnc_len = 2;
            mcc = (numeric_oper[0]-'0') * 100 + (numeric_oper[1]-'0') * 10 + (numeric_oper[2]-'0');
            mnc = (numeric_oper[3]-'0') * 10 + (numeric_oper[4]-'0');
        } else if (len == 6) {
            mnc_len = 3;
            mcc = (numeric_oper[0]-'0') * 100 + (numeric_oper[1]-'0') * 10 + (numeric_oper[2]-'0');
            mnc = (numeric_oper[3]-'0') * 100 + (numeric_oper[4]-'0') * 10 + (numeric_oper[5]-'0');
        } else {
            logE(LOG_TAG, "The length of the numeric code is incorrect");
            goto error;
        }

        /* start to fill the result */
        if (registered == 1) {
            result->network_infos[index].connectionStatus = PRIMARY_SERVING;
        } else {
            result->network_infos[index].connectionStatus = NONE_SERVING;
        }

        if (stat == 2) {
            result->network_infos[index].registered = 1;
        } else {
            result->network_infos[index].registered = 0;
        }

        switch(act) {
        case 0:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mcc = mcc;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mnc = mnc;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.lac = lac_tac;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.cid = (int) cellid;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.arfcn = freq;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.bsic =
                    bsic_psc_cpid_pci == INT_MAX ? 0xFF : bsic_psc_cpid_pci;
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[index].CellInfo.gsm.cellIdentityGsm.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.gsm.cellIdentityGsm.operName.short_name),
                    "%s", shortname);
            aosp_gsm_sig = (sig1 >> 1) + 1; // (0~31)
            if (aosp_gsm_sig > 31) aosp_gsm_sig = 31;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.signalStrength =
                    sig1 == INT_MAX ? 99 : aosp_gsm_sig;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.bitErrorRate =
                    bit_error_rat == INT_MAX ? 99 : bit_error_rat;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.timingAdvance = timing_advance;
            break;
        case 2:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mcc = mcc;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mnc = mnc;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.lac = lac_tac;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.cid = (int) cellid;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.psc = bsic_psc_cpid_pci;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.uarfcn = freq;
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.signalStrength =
                    sig1 == INT_MAX ? 99 : sig1;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate =
                    bit_error_rat == INT_MAX ? 99 : bit_error_rat;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.rscp =
                    (sig3 == INT_MAX || sig3 == 32767) ? 255 : ((sig3>>2) + 120) ;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.ecno =
                    sig4 == INT_MAX ? 255 : (49 + (sig4>>2));
            break;
        case 7:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_LTE;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mcc = mcc;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mnc = mnc;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.tac = lac_tac;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.ci = (int) cellid;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.pci = bsic_psc_cpid_pci;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.earfcn = freq;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.bandwidth = 0;
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[index].CellInfo.lte.cellIdentityLte.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.lte.cellIdentityLte.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.lte.signalStrengthLte.signalStrength =
                    sig1 == INT_MAX ? 99 : sig1;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rsrp =
                    sig3 == 32767 ? INT_MAX : (sig3>>2)*-1;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rsrq =
                    sig4 == INT_MAX ? INT_MAX : (sig4>>2)*-1;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rssnr =
                    sig5 == INT_MAX ? INT_MAX : (sig5>>2)*10;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.cqi = cqi;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.timingAdvance = timing_advance;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;
            pthread_mutex_lock(&mdEgregMutex[m_slot_id]);
            if (result->network_infos[index].registered == 1 &&
                isInService(mdEgreg[m_slot_id]->stat) &&
                mdEgreg[m_slot_id]->dcnr_restricted == 0 &&
                mdEgreg[m_slot_id]->endc_sib == 2) {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 1;
            } else {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 0;
            }
            pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);
            break;
        case 11:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_NR;
            result->network_infos[index].CellInfo.nr.cellidentity.mcc = mcc;
            result->network_infos[index].CellInfo.nr.cellidentity.mnc = mnc;
            result->network_infos[index].CellInfo.nr.cellidentity.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.nr.cellidentity.nci =
                    cellid == INT_MAX ? 0xFFFFFFFFFFFFFFFE : (uint64_t) cellid;
            result->network_infos[index].CellInfo.nr.cellidentity.nrarfcn = (int32_t) freq;
            result->network_infos[index].CellInfo.nr.cellidentity.tac = (int32_t) lac_tac;
            result->network_infos[index].CellInfo.nr.cellidentity.pci = (uint32_t) bsic_psc_cpid_pci;
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH, act);
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrp = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrq = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiSinr =  INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrp =
                    sig3 == INT_MAX ? INT_MAX : (sig3>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrq =
                    sig4 == INT_MAX ? INT_MAX : (sig4>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.ssSinr =
                    sig5 == INT_MAX ? INT_MAX : (sig5>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiTableIndex = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiReportCount = 0;
            break;
        default:
            goto error;
        }
        index+=1;
        result->network_infos_length+=1;
    }

    return 0;
error:
    // because AP would try simulateNetowrkScanByCOPS
    // so we have to make sure we clear allocated memory for operators
    cleanNetworkScanResultOperator(result);
    if (result->network_infos != NULL) {
        free(result->network_infos);
        result->network_infos = NULL;
    }
    return -1;
}

int RmcNetworkNrtRequestHandler::simulateNetowrkScanByCOPS(RIL_NetworkScanResult* result) {
    int INVALID_SIG = 0x7FFFFFFF;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;
    int err = 0, len = 0, num = 0, mcc = 0, mnc = 0, mnc_len = 0, rat = 0, index = 0;
    unsigned int lac = 0;
    char *tmp = NULL;
    char mccmnc[8] = {0};
    char longname[MAX_OPER_NAME_LENGTH] = {0};
    char shortname[MAX_OPER_NAME_LENGTH] = {0};

    p_response = atSendCommandSingleline("AT+COPS=?", "+COPS:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    line = p_response->getIntermediates();
    // count the number of operator
    tmp = line->getLine();
    len = strlen(tmp);
    for(int i = 0; i < len ; i++ ) {
        // here we assume that there is no nested ()
        if (tmp[i] == '(') {
            num++;
        } else if (tmp[i] == ',' && tmp[i+1] == ',') {
            break;
        }
    }
    result->network_infos_length = 0;
    result->network_infos = (RIL_CellInfo_v12*) calloc(num, sizeof(RIL_CellInfo_v12));
    if (result->network_infos == NULL) goto error;
    memset(result->network_infos, 0, num*sizeof(RIL_CellInfo_v12));

    // +COPS: (2,"Far EasTone","FET","46601",0),(...),...,,(0, 1, 3),(0-2)
    line->atTokStart(&err);
    if (err < 0) goto error;

    for (int i = 0; i < num ; i++) {
        // clear the tmp value for this round.
        memset(mccmnc, 0, 8);
        mcc = 0;
        mnc = 0;
        lac = 0;
        mnc_len = 0;
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);

        /* get "(<stat>" */
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // 0: unknown, 1: available, 2: current, 3: forbidden
        if (tmp[1] == '2') {
            result->network_infos[index].registered = 1;
            result->network_infos[index].connectionStatus = PRIMARY_SERVING;
        } else {
            result->network_infos[index].registered = 0;
            result->network_infos[index].connectionStatus = NONE_SERVING;
        }

        /* skip long name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* skip short name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* get <oper> numeric code*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
        // we only have 7 space for string and 1 for end of string.
        // we need eAct to fill the result, so we keep it before we get eAct.
        err = snprintf(mccmnc, 7, "%s", tmp);
        if (err < 0) {
            logE(LOG_TAG, "snprintf mccmnc fail");
        }

        /*plmn_list_format.  0: standard +COPS format , 1: standard +COPS format plus <lac> */
        if (mPlmn_list_format == 1) {
            /* get <lac> numeric code*/
            tmp = line->atTokNextstr(&err);
            if (err < 0){
            logE(LOG_TAG, "No <lac> in +COPS response");
                goto error;
            }
            lac = (unsigned int) strtoul(tmp, NULL, 16);
        }

        /* get <AcT> 0 is "2G", 2 is "3G", 7 is "4G"*/
        rat = line->atTokNextint(&err);
        if (err < 0) goto error;

        if (rat == 11 && lac == 0xFFFFFE) {
            logD(LOG_TAG, "Skip this NSA's NR cell");
            continue; // we skip NSA's NR cell
        }

        switch(rat) {
        case 0:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
            break;
        case 2:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
            break;
        case 7:    //for  LTE
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_LTE;
            break;
        case 11:
        case 12:
        case 13:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_NR;
            break;
        default:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_NONE;
            goto error;
        }

        len = strlen(mccmnc);
        if (len == 5) {
            mnc_len = 2;
            mcc = (mccmnc[0]-'0') * 100 + (mccmnc[1]-'0') * 10 + (mccmnc[2]-'0');
            mnc = (mccmnc[3]-'0') * 10 + (mccmnc[4]-'0');
        } else if (len == 6) {
            mnc_len = 3;
            mcc = (mccmnc[0]-'0') * 100 + (mccmnc[1]-'0') * 10 + (mccmnc[2]-'0');
            mnc = (mccmnc[3]-'0') * 100 + (mccmnc[4]-'0') * 10 + (mccmnc[5]-'0');
        } else {
            logE(LOG_TAG, "The length of the numeric code is incorrect");
            goto error;
        }

        // start to fill the network_info[i]
        switch(result->network_infos[index].cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM:
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mcc = mcc;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mnc = mnc;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.lac = lac;
            getOperatorNamesFromNumericCode(
                    mccmnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[index].CellInfo.gsm.cellIdentityGsm.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.gsm.cellIdentityGsm.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.signalStrength = 99;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.bitErrorRate = 99;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.timingAdvance = INVALID_SIG;
            break;
        case RIL_CELL_INFO_TYPE_WCDMA:
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mcc = mcc;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mnc = mnc;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.lac = lac;
            getOperatorNamesFromNumericCode(
                    mccmnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.signalStrength = 99;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate = 99;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.rscp = 255;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.ecno = 255;
            break;
        case RIL_CELL_INFO_TYPE_LTE:
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mcc = mcc;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mnc = mnc;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.tac = lac;
            getOperatorNamesFromNumericCode(
                    mccmnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[index].CellInfo.lte.cellIdentityLte.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.lte.cellIdentityLte.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.lte.signalStrengthLte.signalStrength = 99;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rsrp = INVALID_SIG;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rsrq = INVALID_SIG;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rssnr = INVALID_SIG;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.cqi = INVALID_SIG;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.timingAdvance = INVALID_SIG;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;
            pthread_mutex_lock(&mdEgregMutex[m_slot_id]);
            if (result->network_infos[index].registered == 1 &&
                isInService(mdEgreg[m_slot_id]->stat) &&
                mdEgreg[m_slot_id]->dcnr_restricted == 0 &&
                mdEgreg[m_slot_id]->endc_sib == 2) {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 1;
            } else {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 0;
            }
            pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);
            break;
        case RIL_CELL_INFO_TYPE_NR:
            result->network_infos[index].CellInfo.nr.cellidentity.mcc = mcc;
            result->network_infos[index].CellInfo.nr.cellidentity.mnc = mnc;
            result->network_infos[index].CellInfo.nr.cellidentity.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.nr.cellidentity.nci = 0xFFFFFFFFFFFFFFFE;
            result->network_infos[index].CellInfo.nr.cellidentity.nrarfcn = 2147483647; // unavailable
            result->network_infos[index].CellInfo.nr.cellidentity.tac = lac;
            result->network_infos[index].CellInfo.nr.cellidentity.pci = 2147483647; // unavailable
            getOperatorNamesFromNumericCode(
                    mccmnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH, 11);
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrp = INVALID_SIG; // unavailable
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrq = INVALID_SIG; // unavailable
            result->network_infos[index].CellInfo.nr.signalStrength.csiSinr =  INVALID_SIG; // unavailable
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrp = INVALID_SIG; // unavailable
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrq = INVALID_SIG; // unavailable
            result->network_infos[index].CellInfo.nr.signalStrength.ssSinr = INVALID_SIG; // unavailable
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiTableIndex =  INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiReportCount = 0;
            break;
        case RIL_CELL_INFO_TYPE_TD_SCDMA:
        case RIL_CELL_INFO_TYPE_CDMA:
        case RIL_CELL_INFO_TYPE_NONE:
            break;
        }
        index+=1;
        result->network_infos_length+=1;
    }

    return 0;
error:
    // we will clean allocated memory for operators in handleSimulatedNetworkScan
    return -1;
}

void RmcNetworkNrtRequestHandler::handleSimulatedNetworkScan() {

    RIL_NetworkScanResult* result = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
    if (result == NULL) {
        logE(LOG_TAG, "handleSimulatedNetworkScan result = null");
        return;
    }
    memset(result, 0, sizeof(RIL_NetworkScanResult));
    result->status = COMPLETE; // for now we don't support PARTIAL.
    sp<RfxMclMessage> urc;
    bool isSuccess = false;
    if (isAPInCall() == true) {
        goto error;
    }
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NORMAL;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);
    // EXOPL support signal, we try it first.
    if (simulateNetowrkScanByEXOPL(result) < 0) {
        if (mPlmnListAbort == 1 || simulateNetowrkScanByCOPS(result) < 0) goto error;
    }
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
            m_slot_id, RfxNetworkScanResultData((void*) result, sizeof(RIL_NetworkScanResult)));
    // response to TeleCore
    responseToTelCore(urc);
    isSuccess = true;

    // release any resource, it should be as the same as RfxNetworkScanResultData's destructor
error:
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);

    RIL_NetworkScanResult * pData = (RIL_NetworkScanResult *) result;
    cleanNetworkScanResultOperator(result);
    if (pData->network_infos != NULL) free(pData->network_infos);
    free(pData);

    // If this failure comes from ABORT, sending URC will cause JE.
    // So we send URC only when mPlmnListAbort is false.
    if (isSuccess != true && mPlmnListAbort == 0) {
        RIL_NetworkScanResult* resp = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
        if (resp == NULL) {
            logE(LOG_TAG, "handleSimulatedNetworkScan resp==null");
            mPlmnListAbort = 0;
            return;
        }
        memset(resp, 0, sizeof(RIL_NetworkScanResult));
        resp->status = COMPLETE; // for now we don't support PARTIAL.
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                m_slot_id, RfxNetworkScanResultData((void*) resp, sizeof(RIL_NetworkScanResult)));
        // response to TeleCore
        responseToTelCore(urc);
        free(resp);
    }
    mPlmnListAbort = 0; /* always clear here to prevent race condition scenario */
}

bool RmcNetworkNrtRequestHandler::handleSimulatedNetworkScanResultByRat(int networkType) {
    sp<RfxMclMessage> urc;
    RIL_NetworkScanResult* result = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
    if (result == NULL) {
        logE(LOG_TAG, "handleSimulatedNetworkScanResultByRat result = null");
        return false;
    }
    memset(result, 0, sizeof(RIL_NetworkScanResult));
    result->status = PARTIAL;

    int INVALID_SIG = 0x7FFFFFFF;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;
    int err = 0, len = 0, num = 0, mcc = 0, mnc = 0, mnc_len = 0, rat = 0;
    unsigned int lac = 0;
    char *tmp = NULL;
    char mccmnc[8] = {0};
    char longname[MAX_OPER_NAME_LENGTH] = {0};
    char shortname[MAX_OPER_NAME_LENGTH] = {0};
    bool success = false;

    p_response = atSendCommandSingleline(String8::format("AT+EPLWSS=%d, 50, 0", networkType), "+EPLWSS:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    line = p_response->getIntermediates();
    // count the number of operator
    tmp = line->getLine();
    len = strlen(tmp);
    for(int i = 0; i < len ; i++ ) {
        // here we assume that there is no nested ()
        if (tmp[i] == '(') {
            num++;
        } else if (tmp[i] == ',' && tmp[i+1] == ',') {
            break;
        }
    }
    result->network_infos_length = num;
    result->network_infos = (RIL_CellInfo_v12*) calloc(num, sizeof(RIL_CellInfo_v12));
    if (result->network_infos == NULL) goto error;
    memset(result->network_infos, 0, num*sizeof(RIL_CellInfo_v12));

    line->atTokStart(&err);
    if (err < 0) goto error;

    for (int i = 0; i < num ; i++) {
        // clear the tmp value for this round.
        memset(mccmnc, 0, 8);
        mcc = 0;
        mnc = 0;
        lac = 0;
        mnc_len = 0;
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);

        /* get "(<stat>" */
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // 0: unknown, 1: available, 2: current, 3: forbidden
        if (tmp[1] == '2') {
            result->network_infos[i].registered = 1;
            result->network_infos[i].connectionStatus = PRIMARY_SERVING;
        } else {
            result->network_infos[i].registered = 0;
            result->network_infos[i].connectionStatus = NONE_SERVING;
        }

        /* skip long name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* skip short name*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* get <oper> numeric code*/
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
        // we only have 7 space for string and 1 for end of string.
        // we need eAct to fill the result, so we keep it before we get eAct.
        err = snprintf(mccmnc, 7, "%s", tmp);
        if (err < 0) {
            logE(LOG_TAG, "snprintf mccmnc fail");
        }

        /* get <lac> numeric code*/
        tmp = line->atTokNextstr(&err);
        if (err < 0){
            logE(LOG_TAG, "No <lac> in +EPLWSS response");
            goto error;
        }
        lac = (unsigned int) strtoul(tmp, NULL, 16);

        /* get <AcT> 0 is "2G", 2 is "3G", 7 is "4G"*/
        rat = line->atTokNextint(&err);
        if (err < 0) goto error;

        switch(rat) {
        case 0:
            result->network_infos[i].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
            break;
        case 2:
            result->network_infos[i].cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
            break;
        case 7:    //for  LTE
            result->network_infos[i].cellInfoType = RIL_CELL_INFO_TYPE_LTE;
            break;
        case 11:
        default:
            result->network_infos[i].cellInfoType = RIL_CELL_INFO_TYPE_NONE;
            goto error;
        }

        len = strlen(mccmnc);
        if (len == 5) {
            mnc_len = 2;
            mcc = (mccmnc[0]-'0') * 100 + (mccmnc[1]-'0') * 10 + (mccmnc[2]-'0');
            mnc = (mccmnc[3]-'0') * 10 + (mccmnc[4]-'0');
        } else if (len == 6) {
            mnc_len = 3;
            mcc = (mccmnc[0]-'0') * 100 + (mccmnc[1]-'0') * 10 + (mccmnc[2]-'0');
            mnc = (mccmnc[3]-'0') * 100 + (mccmnc[4]-'0') * 10 + (mccmnc[5]-'0');
        } else {
            logE(LOG_TAG, "The length of the numeric code is incorrect");
            goto error;
        }

        // start to fill the network_info[i]
        switch(result->network_infos[i].cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM:
            result->network_infos[i].CellInfo.gsm.cellIdentityGsm.mcc = mcc;
            result->network_infos[i].CellInfo.gsm.cellIdentityGsm.mnc = mnc;
            result->network_infos[i].CellInfo.gsm.cellIdentityGsm.mnc_len = mnc_len;
            result->network_infos[i].CellInfo.gsm.cellIdentityGsm.lac = lac;
            getOperatorNamesFromNumericCode(
                    mccmnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.short_name),
                    "%s", shortname);
            result->network_infos[i].CellInfo.gsm.signalStrengthGsm.signalStrength = 99;
            result->network_infos[i].CellInfo.gsm.signalStrengthGsm.bitErrorRate = 99;
            result->network_infos[i].CellInfo.gsm.signalStrengthGsm.timingAdvance = INVALID_SIG;
            break;
        case RIL_CELL_INFO_TYPE_WCDMA:
            result->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.mcc = mcc;
            result->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.mnc = mnc;
            result->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.mnc_len = mnc_len;
            result->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.lac = lac;
            getOperatorNamesFromNumericCode(
                    mccmnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name),
                    "%s", shortname);
            result->network_infos[i].CellInfo.wcdma.signalStrengthWcdma.signalStrength = 99;
            result->network_infos[i].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate = 99;
            result->network_infos[i].CellInfo.wcdma.signalStrengthWcdma.rscp = 255;
            result->network_infos[i].CellInfo.wcdma.signalStrengthWcdma.ecno = 255;
            break;
        case RIL_CELL_INFO_TYPE_LTE:
            result->network_infos[i].CellInfo.lte.cellIdentityLte.mcc = mcc;
            result->network_infos[i].CellInfo.lte.cellIdentityLte.mnc = mnc;
            result->network_infos[i].CellInfo.lte.cellIdentityLte.mnc_len = mnc_len;
            result->network_infos[i].CellInfo.lte.cellIdentityLte.tac = lac;
            getOperatorNamesFromNumericCode(
                    mccmnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH);
            asprintf(&(result->network_infos[i].CellInfo.lte.cellIdentityLte.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[i].CellInfo.lte.cellIdentityLte.operName.short_name),
                    "%s", shortname);
            result->network_infos[i].CellInfo.lte.signalStrengthLte.signalStrength = 99;
            result->network_infos[i].CellInfo.lte.signalStrengthLte.rsrp = INVALID_SIG;
            result->network_infos[i].CellInfo.lte.signalStrengthLte.rsrq = INVALID_SIG;
            result->network_infos[i].CellInfo.lte.signalStrengthLte.rssnr = INVALID_SIG;
            result->network_infos[i].CellInfo.lte.signalStrengthLte.cqi = INVALID_SIG;
            result->network_infos[i].CellInfo.lte.signalStrengthLte.timingAdvance = INVALID_SIG;
            result->network_infos[i].CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;
            break;
        case RIL_CELL_INFO_TYPE_NR:
        case RIL_CELL_INFO_TYPE_TD_SCDMA:
        case RIL_CELL_INFO_TYPE_CDMA:
        case RIL_CELL_INFO_TYPE_NONE:
            break;
        }
    }
    success = true;
error:
    if (success && num > 0) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                m_slot_id, RfxNetworkScanResultData((void*) result, sizeof(RIL_NetworkScanResult)));
        // response to TeleCore
        responseToTelCore(urc);
    }
    RIL_NetworkScanResult * pData = (RIL_NetworkScanResult *) result;
    if (pData->network_infos_length > 0 && pData->network_infos != NULL) {
        int countCell = pData->network_infos_length;
        for (int i = 0; i < countCell; i++) {
            switch(pData->network_infos[i].cellInfoType) {
                case RIL_CELL_INFO_TYPE_GSM:
                    if (pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.long_name)
                        free(pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.long_name);
                    if (pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.short_name)
                        free(pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.short_name);
                    break;
                case RIL_CELL_INFO_TYPE_WCDMA:
                    if (pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name)
                        free(pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                    if(pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name)
                        free(pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                    break;
                case RIL_CELL_INFO_TYPE_LTE:
                    if (pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.long_name)
                        free(pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.long_name);
                    if (pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.short_name)
                        free(pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.short_name);
                    break;
                case RIL_CELL_INFO_TYPE_TD_SCDMA:
                case RIL_CELL_INFO_TYPE_CDMA:
                case RIL_CELL_INFO_TYPE_NR:
                case RIL_CELL_INFO_TYPE_NONE:
                    break;
            }
        }
    }
    if (pData->network_infos != NULL) free(pData->network_infos);
    free(pData);
    return success;
}

bool RmcNetworkNrtRequestHandler::handleSimulatedNetworkScanResultByRat() {
    RIL_NetworkScanResult* result = NULL;
    sp<RfxMclMessage> urc;
    bool ever_success = false;
    int rat_list[] = {3, 1, 0}; // 3:4G, 1:3G, 0:2G
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NORMAL;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, true);
    for (int i = 0; i < (sizeof(rat_list) / sizeof(int)); i++) {
        if (isAPInCall() == true || mPlmnListAbort == 1) {
            goto error;
        }
        ever_success = ever_success | handleSimulatedNetworkScanResultByRat(rat_list[i]);
    }
error:
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
    if (mPlmnListAbort == 0) {
        if (!ever_success) {
            return false;
        }
        result = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
        if (result != NULL) {
            result->status = COMPLETE;
            urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                    m_slot_id, RfxNetworkScanResultData((void*) result, sizeof(RIL_NetworkScanResult)));
            // response to TeleCore
            responseToTelCore(urc);
        } else {
            logE(LOG_TAG, "handleSimulatedNetworkScanResultByRat result = null");
        }
    }
    if (result) free(result);
    mPlmnListAbort = 0;
    return true;
}

void RmcNetworkNrtRequestHandler::requestGetFemtocellList(const sp<RfxMclMessage>& msg) {
    /* +ECSG: <num_plmn>,<plmn_id>,<act>,<num_csg>,<csg_id>,<csg_icon_type>,<hnb_name>,<sig>[,...]
       AT Response Example
       +ECSG: 3,"46000",2,1,<csg_id_A>,<csg_type_A>,<hnb_name_A>,<sig>,"46002",7,1,<csg_id_B>,<csg_type_B>,<hnb_name_B>,<sig>,"46002",7,1,<csg_id_C>,<csg_type_C>,<hnb_name_C>,<sig> */
    int err, i, j, num, act, csgId ,csgIconType,numCsg, sig;
    sp<RfxMclMessage> response;
    RfxAtLine* line;
    sp<RfxAtResponse> p_response;
    char **femtocellList = NULL;
    char *femtocell = NULL, *plmn_id = NULL, *hnb_name = NULL;
    char shortname[MAX_OPER_NAME_LENGTH];
    int size_femto = 7;
    int support_sig = 1;

    m_csgListOngoing[m_slot_id] = true;
    // check <sig> available
    p_response = atSendCommandSingleline("AT+ECSG=6", "+ECSG");
    if (p_response == NULL ||
            p_response->getError() != 0 ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        logE(LOG_TAG, "modem does not support AT+ECELL=6. abort = %d", m_csgListAbort[m_slot_id]);
        if (m_csgListAbort[m_slot_id]) goto error;
        support_sig = 0;

        // send AT command
        p_response = atSendCommandSingleline("AT+ECSG=0", "+ECSG");

        // check error
        if (p_response == NULL ||
                p_response->getError() != 0 ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
            goto error;
        }
    }

    // handle intermediate
    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // <num_plmn>
    num = line->atTokNextint(&err);
    if (err < 0) goto error;

    // allocate memory
    femtocellList = (char **) calloc(1, sizeof(char*) * num * size_femto);
    femtocell = (char *) calloc(1, num* sizeof(char) * size_femto * MAX_OPER_NAME_LENGTH);

    if (femtocellList == NULL || femtocell == NULL) {
        logE(LOG_TAG, "requestGetFemtocellList calloc fail");
        goto error;
    }

    for (i = 0, j = 0; i < num; i++, j+=size_femto) {
        /* <plmn_id>,<act>,<num_csg>,<csg_id>,<csg_icon_type>,<hnb_name> */
        femtocellList[j+0] = &femtocell[(j+0)*MAX_OPER_NAME_LENGTH];
        femtocellList[j+1] = &femtocell[(j+1)*MAX_OPER_NAME_LENGTH];
        femtocellList[j+2] = &femtocell[(j+2)*MAX_OPER_NAME_LENGTH];
        femtocellList[j+3] = &femtocell[(j+3)*MAX_OPER_NAME_LENGTH];
        femtocellList[j+4] = &femtocell[(j+4)*MAX_OPER_NAME_LENGTH];
        femtocellList[j+5] = &femtocell[(j+5)*MAX_OPER_NAME_LENGTH];
        femtocellList[j+6] = &femtocell[(j+6)*MAX_OPER_NAME_LENGTH];

        /* get <plmn_id> numeric code*/
        plmn_id = line->atTokNextstr(&err);
        if (err < 0) goto error;
        sprintf(femtocellList[j+0], "%s", plmn_id);

        int len = strlen(femtocellList[j+0]);
        if (len == 5 || len == 6) {
            err = getOperatorNamesFromNumericCode(
                      femtocellList[j+0], femtocellList[j+1],shortname, MAX_OPER_NAME_LENGTH);
            if (err < 0) goto error;
        } else {
            goto error;
        }

        /* get <AcT> 0 is "2G", 2 is "3G", 7 is "4G"*/
        act = line->atTokNextint(&err);
        if (err < 0) goto error;
        sprintf(femtocellList[j+2], "%d", act);

        /* get <num_csg> fwk no need*/
        numCsg = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* get <csgId> */
        csgId = line->atTokNextint(&err);
        if (err < 0) goto error;
        sprintf(femtocellList[j+3], "%d", csgId);

        /* get <csgIconType> */
        csgIconType = line->atTokNextint(&err);
        if (err < 0) goto error;
        sprintf(femtocellList[j+4], "%d", csgIconType);

        /* get <hnbName> */
        hnb_name = line->atTokNextstr(&err);
        if (err < 0) goto error;
        sprintf(femtocellList[j+5], "%s", hnb_name);

        if (support_sig == 1) {
            /* get <sig> */
            sig = line->atTokNextint(&err);
            if (err < 0) goto error;
            sprintf(femtocellList[j+6], "%d", sig);
        } else {
            sprintf(femtocellList[j+6], "-200"); // fake low sig
        }

        logD(LOG_TAG, "requestGetFemtocellList (%s, %s, %s, %s, %s, %s, %s)",
                femtocellList[j+0],
                femtocellList[j+1],
                femtocellList[j+2],
                femtocellList[j+3],
                femtocellList[j+4],
                femtocellList[j+5],
                femtocellList[j+6]);
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData(femtocellList, num*size_femto), msg, false);
    responseToTelCore(response);
    free(femtocellList);
    free(femtocell);

    m_csgListOngoing[m_slot_id] = false;
    m_csgListAbort[m_slot_id] = false; /* always clear here to prevent race condition scenario */
    return;

error:
    logE(LOG_TAG, "requestGetFemtocellList must never return error when radio is on");
    if (m_csgListAbort[m_slot_id]) {
        // requestGetFemtocellList is canceled
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_CANCELLED,
            RfxStringsData(), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxStringsData(), msg, false);
    }
    // response to TeleCore
    responseToTelCore(response);
    if (femtocellList != NULL) free(femtocellList);
    if (femtocell) free(femtocell);
    m_csgListOngoing[m_slot_id] = false;
    m_csgListAbort[m_slot_id] =false; /* always clear here to prevent race condition scenario */
}
