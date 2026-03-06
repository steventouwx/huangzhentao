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

#include "RmmNwAsyncRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxAtLine.h"
#include "RfxMipcData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxRawData.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmmNwAsyncHdlr"


// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmNwAsyncRequestHandler, RIL_CMD_PROXY_3);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_PSEUDO_CELL_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_PSEUDO_CELL_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_POL_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_GET_POL_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_POL_ENTRY);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSscData, RfxVoidData,
        RFX_MSG_REQUEST_SET_SYSTEM_SELECTION_CHANNELS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxSscData,
        RFX_MSG_REQUEST_GET_SYSTEM_SELECTION_CHANNELS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SEARCH_RAT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SEARCH_STORED_FREQUENCY_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_ROAMING_ENABLE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_ROAMING_ENABLE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_AVAILABLE_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_BACKGROUND_SEARCH_TIMER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_LOCATION_UPDATES);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_SERVICE_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_GET_FEMTOCELL_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ABORT_FEMTOCELL_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SELECT_FEMTOCELL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CONFIG_A2_OFFSET);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CONFIG_B1_OFFSET);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_DISABLE_2G);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_DISABLE_2G);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, RFX_MSG_REQUEST_NW_OEM_HOOK_RAW);

RmmNwAsyncRequestHandler::RmmNwAsyncRequestHandler(int slot_id, int channel_id) :
        RmmNwHandler(slot_id, channel_id) {
     const int request[] = {
        RFX_MSG_REQUEST_SET_DISABLE_2G,
        RFX_MSG_REQUEST_GET_DISABLE_2G,
        RFX_MSG_REQUEST_GET_POL_CAPABILITY,
        RFX_MSG_REQUEST_GET_POL_LIST,
        RFX_MSG_REQUEST_SET_POL_ENTRY,
        RFX_MSG_REQUEST_GET_FEMTOCELL_LIST,
        RFX_MSG_REQUEST_ABORT_FEMTOCELL_LIST,
        RFX_MSG_REQUEST_SELECT_FEMTOCELL,
        RFX_MSG_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE,
        RFX_MSG_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT,
        RFX_MSG_REQUEST_SET_SYSTEM_SELECTION_CHANNELS,
        RFX_MSG_REQUEST_GET_SYSTEM_SELECTION_CHANNELS,
        RFX_MSG_REQUEST_SEARCH_RAT,
        RFX_MSG_REQUEST_SEARCH_STORED_FREQUENCY_INFO,
        RFX_MSG_REQUEST_SET_ROAMING_ENABLE,
        RFX_MSG_REQUEST_GET_ROAMING_ENABLE,
        RFX_MSG_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION,
        RFX_MSG_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION,
        RFX_MSG_REQUEST_QUERY_AVAILABLE_BAND_MODE,
        RFX_MSG_REQUEST_SET_BAND_MODE,
        RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION,
        RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION,
        RFX_MSG_REQUEST_SET_BACKGROUND_SEARCH_TIMER,
        RFX_MSG_REQUEST_GET_PSEUDO_CELL_INFO,
        RFX_MSG_REQUEST_SET_PSEUDO_CELL_MODE,
        RFX_MSG_REQUEST_SET_LOCATION_UPDATES,
        RFX_MSG_REQUEST_SET_SERVICE_STATE,
        RFX_MSG_REQUEST_CONFIG_A2_OFFSET,
        RFX_MSG_REQUEST_CONFIG_B1_OFFSET,
        RFX_MSG_REQUEST_NW_OEM_HOOK_RAW
    };
    logV(LOG_TAG, "RmmNwAsyncRequestHandler[%d] start", slot_id);
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
}

RmmNwAsyncRequestHandler::~RmmNwAsyncRequestHandler() {
}

void RmmNwAsyncRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logV(LOG_TAG, "onHandleRequest: %s", RFX_ID_TO_STR(msg->getId()));
    int request = msg->getId();
    switch(request) {

        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT:
            handleSetNwSelectionModeRequest(msg);
            break;

        case RFX_MSG_REQUEST_GET_POL_CAPABILITY:
            handleGetPolCapabilityRequest(msg);
            break;

        case RFX_MSG_REQUEST_GET_POL_LIST:
            handleGetPolListRequest(msg);
            break;

        case RFX_MSG_REQUEST_SET_POL_ENTRY:
            handleSetPolEntryRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_DISABLE_2G:
            handleSetDisable2GRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_DISABLE_2G:
            handleGetDisable2GRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_SYSTEM_SELECTION_CHANNELS:
            handleSetSystemSelectionChannelsRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_SYSTEM_SELECTION_CHANNELS:
            handleGetSystemSelectionChannelsRequest(msg);
            break;
        case RFX_MSG_REQUEST_SEARCH_RAT:
            handleSearchRatRequest(msg);
            break;
        case RFX_MSG_REQUEST_SEARCH_STORED_FREQUENCY_INFO:
            handleSearchStoredFrequencyInfoRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_ROAMING_ENABLE:
            handleSetRoamingEnableRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_ROAMING_ENABLE:
            handleGetRoamingEnableRequest(msg);
            break;
        case RFX_MSG_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION:
            handleDeactivateNrScgCommunicationRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION:
            handleGetDeactivateNrScgCommunicationRequest(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_BAND_MODE:
            handleQueryAvailableBandModeRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_BAND_MODE:
            handleSetBandModeRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION:
            handleSetLteReleaseVersionRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION:
            handleGetLteReleaseVersionRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_BACKGROUND_SEARCH_TIMER:
            handleSetBackgroundSearchTimerRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_PSEUDO_CELL_INFO:
            handleGetPseudoCellInfoRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_PSEUDO_CELL_MODE:
            handleSetPseudoCellModeRequest(msg);
            break;


        case RFX_MSG_REQUEST_SET_LOCATION_UPDATES:
            handleSetLocationUpdatesRequest(msg);
            break;

        case RFX_MSG_REQUEST_SET_SERVICE_STATE:
            handleSetServiceStateRequest(msg);
            break;

        case RFX_MSG_REQUEST_GET_FEMTOCELL_LIST:
            handleGetFemtocellListRequest(msg);
            break;

        case RFX_MSG_REQUEST_ABORT_FEMTOCELL_LIST:
            handleAbortFemtocellListRequest(msg);
            break;

        case RFX_MSG_REQUEST_SELECT_FEMTOCELL:
            handleSelectFemtocellRequest(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE:
            handleQueryFemtocellSystemSelectionModeRequest(msg);
            break;

        case RFX_MSG_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE:
            handleSetFemtocellSystemSelectionModeRequest(msg);
            break;

        case RFX_MSG_REQUEST_CONFIG_A2_OFFSET:
            handleConfigA2OffsetRequest(msg);
            break;

        case RFX_MSG_REQUEST_CONFIG_B1_OFFSET:
            handleConfigB1OffsetRequest(msg);
            break;
        case RFX_MSG_REQUEST_NW_OEM_HOOK_RAW:
            handleNwOemHookRawRequest(msg);
            break;

        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

void RmmNwAsyncRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    logV(LOG_TAG, "onHandleResponse: %s", RFX_ID_TO_STR(msg->getId()));
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT:
            handleSetNwSelectionModeResponse(msg);
            break;

        case RFX_MSG_REQUEST_GET_POL_CAPABILITY:
            handleGetPolCapabilityResponse(msg);
            break;

        case RFX_MSG_REQUEST_GET_POL_LIST:
            handleGetPolListResponse(msg);
            break;

        case RFX_MSG_REQUEST_SET_POL_ENTRY:
            handleSetPolEntryResponse(msg);
            break;
        case RFX_MSG_REQUEST_SET_SYSTEM_SELECTION_CHANNELS:
            handleSetSystemSelectionChannelsResponse(msg);
            break;

        case RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION:
        case RFX_MSG_REQUEST_SEARCH_RAT:
        case RFX_MSG_REQUEST_SEARCH_STORED_FREQUENCY_INFO:
        case RFX_MSG_REQUEST_SET_ROAMING_ENABLE:
        case RFX_MSG_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION:
        // case RFX_MSG_REQUEST_DISABLE_NR:
        case RFX_MSG_REQUEST_SET_BAND_MODE:
        case RFX_MSG_REQUEST_SET_BACKGROUND_SEARCH_TIMER:
        case RFX_MSG_REQUEST_SET_PSEUDO_CELL_MODE:
        case RFX_MSG_REQUEST_SET_LOCATION_UPDATES:
        case RFX_MSG_REQUEST_SET_SERVICE_STATE:
        case RFX_MSG_REQUEST_ABORT_FEMTOCELL_LIST:
        case RFX_MSG_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE:
        case RFX_MSG_REQUEST_SELECT_FEMTOCELL:
        case RFX_MSG_REQUEST_CONFIG_A2_OFFSET:
        case RFX_MSG_REQUEST_CONFIG_B1_OFFSET:
        case RFX_MSG_REQUEST_SET_DISABLE_2G:
            handleVoidResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_DISABLE_2G:
            handleGetDisable2GResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_ROAMING_ENABLE:
            handleGetRoamingEnableResponse(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_BAND_MODE:
            handleQueryAvailableBandModeResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION:
            handleGetLteReleaseVersionResponse(msg);
            break;

        case RFX_MSG_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION:
            handleGetDeactivateNrScgCommunicationResponse(msg);
            break;

        case RFX_MSG_REQUEST_GET_PSEUDO_CELL_INFO:
            handleGetPseudoCellInfoResponse(msg);
            break;

        case RFX_MSG_REQUEST_GET_FEMTOCELL_LIST:
            handleGetFemtocellListResponse(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE:
            handleQueryFemtocellSystemSelectionModeReponse(msg);
            break;

        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}


void RmmNwAsyncRequestHandler::handleSetNwSelectionModeRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError;
    char **args = NULL;
    int argsCount = 0;
    bool testMode = false;
    char *numeric = NULL;
    char *ran = NULL;
    char *manualMode = NULL;

    uint8_t mode = 0;
    uint8_t act = 0xFF;
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_SET_REGISTER_STATE_REQ, msg->getSlotId());

    switch (msg->getId()) {
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
            mode = MIPC_NW_REGISTER_MODE_AUTOMATIC;
            if (isGcfTestMode() || isTestSim(m_slot_id))
                testMode = true;
            break;

        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL:
            mode = MIPC_NW_REGISTER_MODE_MANUAL;
            numeric = (char*)msg->getData()->getData();
            break;

        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT:

            mode = MIPC_NW_REGISTER_MODE_MANUAL;
            args = (char **)msg->getData()->getData();
            argsCount = msg->getData()->getDataLength() / sizeof(char *);
            if (args == NULL || argsCount < 2) {
                radioError = RIL_E_INVALID_ARGUMENTS;
                goto error;
            }
            numeric = args[0];
            ran = args[1];
            if (argsCount == 3 && args[2]) {
                manualMode = args[2];
                if (strcmp("1", manualMode) == 0) {
                    mode = MIPC_NW_REGISTER_MODE_SEMI_AUTO;
                } else if (strcmp("2", manualMode) == 0) {
                    mode = MIPC_NW_REGISTER_MODE_AUTOMATIC;
                }
            }

            break;

        default:
            logE(LOG_TAG, "should not get here");
            break;
    }
    if (mode != MIPC_NW_REGISTER_MODE_AUTOMATIC
            && (numeric == NULL
                || strcmp("12345", numeric) == 0
                || strcmp("123456", numeric) == 0)) {
        radioError = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }
    if (ran) {
        if (msg->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN) {
            switch (atoi(ran)) {
            case 1: act = MIPC_NW_ACT_TYPE_GSM; break;
            case 2: act = MIPC_NW_ACT_TYPE_UMTS; break;
            case 3: act = MIPC_NW_ACT_TYPE_LTE; break;
            case 6: act = MIPC_NW_ACT_TYPE_NG_RAN; break;
            case 0: break;
            default :
                radioError = RIL_E_INVALID_ARGUMENTS;
                goto error;
            }
        } else {
            switch (atoi(ran)) {
            case 0: act = MIPC_NW_ACT_TYPE_GSM; break;
            case 2: act = MIPC_NW_ACT_TYPE_UMTS; break;
            case 7: act = MIPC_NW_ACT_TYPE_LTE; break;
            case 11: act = MIPC_NW_ACT_TYPE_NR; break;
            case 12: act = MIPC_NW_ACT_TYPE_NG_RAN; break;
            default :
                radioError = RIL_E_INVALID_ARGUMENTS;
                goto error;
            }
        }
    }

    reqData->addMipcTlvUint8(
            MIPC_NW_SET_REGISTER_STATE_REQ_T_MODE, mode);
    if (numeric) {
        reqData->addMipcTlvStr(
                MIPC_NW_SET_REGISTER_STATE_REQ_T_OPER, numeric);
    }
    if (act != 0xFF) {
        reqData->addMipcTlvUint8(
                MIPC_NW_SET_REGISTER_STATE_REQ_T_ACT, act);
    }
    if (mode == MIPC_NW_REGISTER_MODE_AUTOMATIC) {
        reqData->addMipcTlvUint8(
                MIPC_NW_SET_REGISTER_STATE_REQ_T_CTRL_MODE, testMode ? 0 : 1);
    }
    logD(LOG_TAG, "handleSetNwSelectionModeRequest: mode[%d],act[%d],testMode[%d]",
            mode, act, testMode);

    callToMipcMsgAsync(msg, reqData);
    isNeedNotifyStateChanged();
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), radioError,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleSetNwSelectionModeResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    uint8_t mode = 0;
    RfxMipcData* cnfData = msg->getMipcData();

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "handleSetNwSelectionModeResponse: fail");
        //always update property PROPERTY_AUTO_RESTORE_RPLMN when fail
        updateAutoRestoreRplmnStatus(msg->getSlotId());

        response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    mode = cnfData->getMipcUint8Val(
        MIPC_NW_SET_REGISTER_STATE_CNF_T_MODE, 0);
    logD(LOG_TAG, "handleSetNwSelectionModeResponse: mode[%d]", mode);

    bool autoMode = (mode == MIPC_NW_REGISTER_MODE_AUTOMATIC
            || mode == MIPC_NW_REGISTER_MODE_SEMI_AUTO);

    pthread_mutex_lock(&(sNwSelModeMutex[m_slot_id]));
    if (autoMode)
        sNwSelMode[m_slot_id] = false;
    else
        sNwSelMode[m_slot_id] = true;
    pthread_mutex_unlock(&(sNwSelModeMutex[m_slot_id]));

    response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleGetPolCapabilityRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_POL_CAPABILITY_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, reqData);
}

void RmmNwAsyncRequestHandler::handleGetPolCapabilityResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    int result[4];
    uint16_t dataLen = 0;
    RfxMipcData* cnfData = msg->getMipcData();
    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        if (cnfData)
            logE(LOG_TAG, "handleGetPolCapabilityResponse: result[%d]", cnfData->getResult());
        else
            logE(LOG_TAG, "handleGetPolCapabilityResponse: cnfData[null]");
        response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    mipc_nw_pol_info_struct4 *info = (mipc_nw_pol_info_struct4 *)cnfData->getMipcVal(
        MIPC_NW_GET_POL_CAPABILITY_CNF_T_POL_INFO, &dataLen);
    if (info == NULL || dataLen != sizeof(mipc_nw_pol_info_struct4)) {
        logE(LOG_TAG, "handleGetPolCapabilityResponse: info[%p], len[%d]", info, dataLen);
        response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    logD(LOG_TAG, "handleGetPolCapabilityResponse: [%d]-[%d],[%d]-[%d]",
        info->start_index, info->end_index, info->format_first, info->format_last);
    result[0] = (int)info->start_index;
    result[1] = (int)info->end_index;
    result[2] = info->format_first;
    result[3] = info->format_last;

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(result, 4), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleGetPolListRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_PREFERRED_PROVIDER_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, reqData);
}

void RmmNwAsyncRequestHandler::handleGetPolListResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    uint16_t dataLen = 0;
    RfxMipcData* cnfData = msg->getMipcData();
    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        if (cnfData)
            logE(LOG_TAG, "handleGetPolListResponse: result[%d]", cnfData->getResult());
        else
            logE(LOG_TAG, "handleGetPolListResponse: cnfData[null]");

        response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    int infoCount = cnfData->getMipcUint8Val(
        MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_COUNT, 0xff);
    mipc_nw_provider_struct4 *info = (mipc_nw_provider_struct4 *)cnfData->getMipcVal(
        MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_LIST, &dataLen);

    if ((infoCount == 0 || infoCount == 0xff) && dataLen == 0) {
        logD(LOG_TAG, "handleGetPolListResponse: info[%p], len[%d], count[%d]",
            info, dataLen, infoCount);
        infoCount = 0;
    } else if (infoCount == 0xff || info == NULL || dataLen == 0
        || (dataLen % sizeof(mipc_nw_provider_struct4) != 0
            && dataLen / sizeof(mipc_nw_provider_struct4) != infoCount)) {
        logE(LOG_TAG, "handleGetPolListResponse: info[%p], len[%d], count[%d]",
            info, dataLen, infoCount);
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    PreferredPlmnResponse *result = (PreferredPlmnResponse *)malloc(infoCount * sizeof(PreferredPlmnResponse));
    char *dataBuff = (char *)malloc(infoCount * RESPONSE_SIZE);
    if (result == NULL || dataBuff == NULL) {
        logE(LOG_TAG, "handleGetPolListResponse: insufficient memory");
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        if (result != NULL) free(result);
        if (dataBuff != NULL) free(dataBuff);
        return;
    }
    memset(dataBuff, 0, infoCount * RESPONSE_SIZE);
    for (int i = 0; i < infoCount; i++) {
        result[i].index = &dataBuff[i*RESPONSE_SIZE];
        result[i].format = &dataBuff[i*RESPONSE_SIZE+MAX_INDEX_LEN];
        result[i].name = &dataBuff[i*RESPONSE_SIZE+MAX_INDEX_LEN+MAX_FORMAT_LEN];
        result[i].act = &dataBuff[i*RESPONSE_SIZE+MAX_INDEX_LEN+MAX_OPER_NAME_LENGTH+MAX_FORMAT_LEN];
    }

    logD(LOG_TAG, "handleGetPolListResponse: count[%d]", infoCount);
    for (int i = 0; i < infoCount; i++) {
        int err = 0;
        err = snprintf(result[i].index, MAX_INDEX_LEN, "%d", info[i].index);
        if (err < 0) {
            logE(LOG_TAG, "handleGetPolListResponse::using snprintf and getting ERROR");
        }
        if (strlen(info[i].network_name) > 0) {
            strncpy(result[i].name, info[i].network_name, MAX_OPER_NAME_LENGTH-1);
            strcpy(result[i].format, "0");
        } else
        if (strlen(info[i].network_short_name) > 0) {
            strncpy(result[i].name, info[i].network_short_name, MAX_OPER_NAME_LENGTH-1);
            strcpy(result[i].format, "1");
        } else {
            strncpy(result[i].name, info[i].plmn_id, MAX_OPER_NAME_LENGTH-1);
            strcpy(result[i].format, "2");
        }
        err = snprintf(result[i].act, MAX_ACT_LEN, "%d", info[i].act);
        if (err < 0) {
            logE(LOG_TAG, "handleGetPolListResponse::using snprintf and getting ERROR");
        }

        logD(LOG_TAG, "handleGetPolListResponse:"
            "info[%d] - index[%d], plmn[%s], act[%x]",
            i, info[i].index, info[i].plmn_id, info[i].act);
        logD(LOG_TAG, "handleGetPolListResponse:"
            "result[%d] - index[%s], name[%s], format[%s] act[%s]",
            i, result[i].index, result[i].name, result[i].format, result[i].act);
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData((char **)result, infoCount*4), msg, false);
    responseToTelCore(response);
    if (result) free(result);
    if (dataBuff) free(dataBuff);
}

void RmmNwAsyncRequestHandler::handleSetPolEntryRequest(const sp<RfxMclMessage>& msg) {
    char **args = (char **)msg->getData()->getData();
    int argsCount = msg->getData()->getDataLength() / sizeof(char *);
    char *plmn = NULL;
    sp<RfxMclMessage> response;

    if (args == NULL || argsCount <= 0 || argsCount > 3) {
        logE(LOG_TAG, "handleSetPolEntryRequest: invalid arguments");
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    uint32_t index = (uint32_t)atoi(args[0]);
    if (argsCount >= 2)
        plmn = args[1];

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_SET_PREFERRED_PROVIDER_REQ, msg->getSlotId());
    mipc_nw_provider_struct4 info;
    memset(&info, 0, sizeof(info));
    info.index = index;
    if (plmn && strlen(plmn) > 0) {
        strncpy(info.plmn_id, plmn, sizeof(info.plmn_id) - 1);
    }
    if (argsCount == 3) {
        info.act = (uint32_t)atoi(args[2]);
    }

    reqData->addMipcTlvUint8(MIPC_NW_SET_PREFERRED_PROVIDER_REQ_T_COUNT, 1);
    reqData->addMipcTlv(
        MIPC_NW_SET_PREFERRED_PROVIDER_REQ_T_LIST, (uint16_t)sizeof(info), &info);

    callToMipcMsgAsync(msg, reqData);
}

void RmmNwAsyncRequestHandler::handleSetPolEntryResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RfxMipcData* cnfData = msg->getMipcData();

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        uint16_t cause = 0;
        if (cnfData) {
            cause = cnfData->getMipcUint16Val(
                    MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_FAIL_CAUSE, 0);
            logE(LOG_TAG, "handleSetPolEntryResponse: result[%d], cause[%d]",
                    cnfData->getResult(), cause);
        } else {
            logE(LOG_TAG, "handleSetPolEntryResponse: cnfData null");
        }
        response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);
}


void RmmNwAsyncRequestHandler::handleSetSystemSelectionChannelsRequest(const sp<RfxMclMessage>& msg) {
    RIL_SystemSelectionChannels* ssc =
            (RIL_SystemSelectionChannels*) msg->getData()->getData();
    sp<RfxMclMessage> resp;
    int k = 0;
    RIL_GeranBands gband;
    RIL_UtranBands uBand;
    RIL_EutranBands eBand;
    RIL_NgranBands nBand;

    String8 log("handleSetSystemSelectionChannelsRequest ");
    log.append(String8::format(",specifyChannels=%d", ssc->specifyChannels));

    mipc_nw_channel_lock_info_v1_struct4 info;
    if (ssc->specifyChannels > 0 && ssc->specifiers_length > 0) {
        sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_CHANNEL_LOCK_REQ, msg->getSlotId());
        mipc_msg_tlv_array_t *p_array = mipc_msg_add_tlv_array(mipcReq->getData(),
            MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_LIST_V1, (uint16_t) ssc->specifiers_length);
        log.append(String8::format(",specifiers_length=%d", ssc->specifiers_length));

        for (int i = 0; i < ssc->specifiers_length; i++) {
            memset(&info, 0, sizeof(mipc_nw_channel_lock_info_v1_struct4));
            // opt
            info.opt = MIPC_NW_CH_LOCK_ENABLE_CH_LK_CBRS;
            log.append(String8::format(",specifiers[%d].radio_access_network=%d", i, ssc->specifiers[i].radio_access_network));
            // <act>
            switch (ssc->specifiers[i].radio_access_network) {
                case GERAN:
                    info.act = 0;
                    break;
                case UTRAN:
                    info.act = 2;
                    break;
                case EUTRAN:
                    info.act = 7;
                    break;
                case NEW_RADIO:
                    info.act = 11;
                    break;
                default:
                    goto error;
            }
            log.append(String8::format(",specifiers[%d].bands_length=%d", i, ssc->specifiers[i].bands_length));
            // <band_info>
            for (int j = 0; j < ssc->specifiers[i].bands_length; j++) {
                switch (ssc->specifiers[i].radio_access_network) {
                    case GERAN:
                        gband = ssc->specifiers[i].bands.geran_bands[j];
                        if (gband > 0 && gband <= GERAN_BAND_MAX) {
                            k = (gband - 1) / 32;
                            info.band_info[k] |= (1 << ((gband - 1)%32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, gband));
                        break;
                    case UTRAN:
                        uBand = ssc->specifiers[i].bands.utran_bands[j];
                        if (uBand > 0 && uBand <= UTRAN_BAND_MAX) {
                            k = (uBand - 1) / 32;
                            info.band_info[k] |= (1 << ((uBand - 1)%32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, uBand));
                        break;
                    case EUTRAN:
                        eBand = ssc->specifiers[i].bands.eutran_bands[j];
                        if (eBand > 0 && eBand <= EUTRAN_BAND_MAX) {
                            k = (eBand - 1) / 32;
                            info.band_info[k] |= (1 << ((eBand - 1) % 32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, eBand));
                        break;
                    case NEW_RADIO:
                        nBand = ssc->specifiers[i].bands.ngranBands[j];
                        if (nBand > 0 && nBand <= NGRAN_BAND_MAX) {
                            k = (nBand - 1) / 32;
                            info.band_info[k] |= (1 << ((nBand - 1) % 32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, nBand));
                        break;
                    default:
                        goto error;
                }
            }
            log.append(String8::format(",specifiers[%d].channels_length=%d", i, ssc->specifiers[i].channels_length));
            // <channel_num>
            info.channel_num = ssc->specifiers[i].channels_length;
            // <arfcn_list>
            for (int j = 0; j < ssc->specifiers[i].channels_length; j++) {
                log.append(String8::format(",specifiers[%d].channels[%d]=%d", i, j, ssc->specifiers[i].channels[j]));
                info.arfcn_list[j] = ssc->specifiers[i].channels[j];
            }
            // cell_id
            info.cell_id = 0xffff;
            // lock_mode
            info.lock_mode = MIPC_NW_CH_LOCK_MODE_UNCHANGED;
            mipc_msg_add_idx(mipcReq->getData(), p_array, i, sizeof(mipc_nw_channel_lock_info_v1_struct4), &info);
        }

        callToMipcMsgAsync(msg, mipcReq);
    } else if (ssc->specifyChannels == 0) {
        // cancel channel lock
        memset(&info, 0, sizeof(mipc_nw_channel_lock_info_v1_struct4));
        sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_CHANNEL_LOCK_REQ, msg->getSlotId());
        mipc_msg_tlv_array_t *p_array = mipc_msg_add_tlv_array(mipcReq->getData(),
            MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_LIST_V1, 1);
        info.opt = MIPC_NW_CH_LOCK_CANCEL;
        info.lock_mode = MIPC_NW_CH_LOCK_MODE_UNCHANGED;
        mipc_msg_add_idx(mipcReq->getData(), p_array, 0, sizeof(mipc_nw_channel_lock_info_v1_struct4), &info);
        callToMipcMsgAsync(msg, mipcReq);
    } else {
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxVoidData(), msg, false);
        responseToTelCore(resp);
    }
    logD(LOG_TAG, "%s", log.string());
    return;
error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_RADIO_NOT_AVAILABLE, RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmmNwAsyncRequestHandler::handleSetSystemSelectionChannelsResponse(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    RIL_Errno rilErrno = result == MIPC_RESULT_SUCCESS ? RIL_E_SUCCESS : RIL_E_RADIO_NOT_AVAILABLE;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleGetSystemSelectionChannelsRequest(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RIL_SystemSelectionChannels* pData = (RIL_SystemSelectionChannels*)alloca(sizeof(RIL_SystemSelectionChannels));
    if (pData == NULL) goto error;
    memset(pData, 0, sizeof(RIL_SystemSelectionChannels));

error:
    /*not support*/
    if (pData) {
        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
        pData->specifyChannels = 0;
        pData->specifiers_length = 0;
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                RfxSscData(pData,sizeof(RIL_SystemSelectionChannels)), msg, false);
        responseToTelCore(response);
    }
}

void RmmNwAsyncRequestHandler::handleSearchRatRequest(const sp<RfxMclMessage>& msg) {

    int *pInt = (int *)msg->getData()->getData();
    uint8_t count = (uint8_t) pInt[0];
    logV(LOG_TAG, "requestSearchRat count: %d", count);
    uint8_t* rat = new uint8_t[count];
    memset(rat, 0, sizeof(uint8_t) * count);
    for (int i = 0 ; i < count ; i++) {
        rat[i] = (uint8_t) pInt[i+1];
        logV(LOG_TAG, "pInt[%d]:%d", i, pInt[i+1]);
    }
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_PREFER_RAT_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_PREFER_RAT_REQ_T_RAT_NUM, count);
    mipcReq->addMipcTlv(MIPC_NW_SET_PREFER_RAT_REQ_T_RAT_LIST, sizeof(uint8_t) * count, rat);
    callToMipcMsgAsync(msg, mipcReq);
    delete[] rat;
}

void RmmNwAsyncRequestHandler::handleSearchStoredFrequencyInfoRequest(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    // pInt[0]:optr, pInt[1]:plmn, pInt[2]:rat, pInt[3]:count, pInt[4...]:arfcn

    if (pInt[0] != 0 && pInt[0] != 1) {
        logE(LOG_TAG, "handleSearchStoredFrequencyInfoRequest invalid optr[%d]", pInt[0]);
        sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                    RfxVoidData(), msg, false);
        responseToTelCore(resp);
        return;
    }
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_OPER, (uint8_t) pInt[0]);
    if (pInt[0] == 1) {
        int plmn = pInt[1];
        uint8_t rat = (uint8_t) pInt[2];
        uint8_t count = (uint8_t) pInt[3];

        mipcReq->addMipcTlvUint8(MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_RAT, rat);
        mipcReq->addMipcTlvStr(MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_PLMN, String8::format("%d", plmn));
        if (count > 0) {
            mipcReq->addMipcTlvUint8(MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_COUNT, count);
            uint32_t* arfcn = new uint32_t[count];
            memset(arfcn, 0, sizeof(uint32_t) * count);
            for (int i = 0 ; i < count ; i++) {
                arfcn[i] = (uint32_t) pInt[i+4];
            }
            mipcReq->addMipcTlv(MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_ARFCN,
                    sizeof(uint32_t) * count, arfcn);
            delete[] arfcn;
        }
    }
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleSetRoamingEnableRequest(const sp<RfxMclMessage>& msg) {

    int *pInt = (int*)msg->getData()->getData();
    // rever the setting from enable(fwk) to bar(md)
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_ROAMING_ENABLE_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_ROAMING_ENABLE_REQ_T_DOM_VOICE, pInt[3] == 0 ? 1 : 0);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_ROAMING_ENABLE_REQ_T_DOM_DATA, pInt[4] == 0 ? 1 : 0);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_ROAMING_ENABLE_REQ_T_INT_VOICE, pInt[1] == 0 ? 1 : 0);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_ROAMING_ENABLE_REQ_T_INT_DATA, pInt[2] == 0 ? 1 : 0);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_ROAMING_ENABLE_REQ_T_LTE_DATA, pInt[5] == 0 ? 1 : 0);
    callToMipcMsgAsync(msg, mipcReq);
    logD(LOG_TAG, "setRoamingEnable [%d,%d,%d,%d,%d]", pInt[3], pInt[4], pInt[1], pInt[2], pInt[5]);
}

void RmmNwAsyncRequestHandler::handleGetRoamingEnableRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_ROAMING_ENABLE_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleGetRoamingEnableResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    int roaming[6] = {1, 0, 1, 1, 1, 1};  // default value
    RfxMipcData* mipcData = msg->getMipcData();
    if (mipcData->getResult() != MIPC_RESULT_SUCCESS) goto error;

    roaming[3] = mipcData->getMipcUint8Val(MIPC_NW_GET_ROAMING_ENABLE_CNF_T_DOM_VOICE, 0);
    roaming[4] = mipcData->getMipcUint8Val(MIPC_NW_GET_ROAMING_ENABLE_CNF_T_DOM_DATA, 0);
    roaming[1] = mipcData->getMipcUint8Val(MIPC_NW_GET_ROAMING_ENABLE_CNF_T_INT_VOICE, 0);
    roaming[2] = mipcData->getMipcUint8Val(MIPC_NW_GET_ROAMING_ENABLE_CNF_T_INT_DATA, 0);
    roaming[5] = mipcData->getMipcUint8Val(MIPC_NW_GET_ROAMING_ENABLE_CNF_T_LTE_DATA, 0);
    logD(LOG_TAG, "getRoamingEnable [%d,%d,%d,%d,%d]", roaming[3], roaming[4], roaming[1], roaming[2], roaming[5]);

    // rever the setting from enable(fwk) to bar(md)
    for (int i = 1; i < 6; i++) {
        roaming[i] = roaming[i] == 0 ? 1:0;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(roaming, 6), msg, false);
    responseToTelCore(resp);
    return;
error:
    logE(LOG_TAG, "getRoamingEnable must never return error when radio is on");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmmNwAsyncRequestHandler::handleDeactivateNrScgCommunicationRequest(const sp<RfxMclMessage>& msg) {
    int deactivate = 0, allowSCGAdd = 1;
    int *pInt = (int *)msg->getData()->getData();
    deactivate = pInt[0];
    allowSCGAdd = pInt[1]; //not used now
    logD(LOG_TAG, "%s: params = %d, %d", __FUNCTION__, deactivate, allowSCGAdd);
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, String8::format("endc_deactivation"));
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA, String8::format("%d,%d", deactivate, 1));
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleGetDeactivateNrScgCommunicationRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_GET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_GET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_GET_CONFIG_REQ_T_TYPE, String8::format("endc_deactivation"));
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleGetDeactivateNrScgCommunicationResponse(const sp<RfxMclMessage>& msg) {
    int tmp[2] = {0};
    int err = 0;
    uint16_t repDataLen = 0;
    char *repData = NULL;
    sp<RfxMclMessage> response;
    RfxAtLine* line = NULL; //data

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    RIL_Errno rilErrno = (result == MIPC_RESULT_SUCCESS) ? RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;

    if (rilErrno == RIL_E_GENERIC_FAILURE) goto error;

    repData = (char*)(mipcData->getMipcVal(MIPC_SYS_GET_CONFIG_CNF_T_DATA, &repDataLen));

    line = new RfxAtLine(repData, NULL);
    if (line != NULL) {
        line->atTokStart(&err);
        if (err < 0) goto error;

        line->atTokNextstr(&err);
        if (err < 0) goto error;

        // get <deactivate>
        tmp[0] = line->atTokNextint(&err);
        if (err < 0) goto error;

        // get <allowSCGAdd>
        tmp[1] = line->atTokNextint(&err);
        if (err < 0) goto error;
        logD(LOG_TAG, "%s: tmp = %d, %d", __FUNCTION__, tmp[0], tmp[1]);
    } else
        goto error;

    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrno,
            RfxIntsData(tmp, 2), msg, false);
    responseToTelCore(response);
    if (line != NULL) {
        delete(line);
        line = NULL;
    }
    return;

error:
    logE(LOG_TAG, "%s, error happens", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    if (line != NULL) {
        delete(line);
        line = NULL;
    }
}

void RmmNwAsyncRequestHandler::handleQueryAvailableBandModeRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_BAND_MODE_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleQueryAvailableBandModeResponse(const sp<RfxMclMessage>& msg) {
    uint32_t gsm_band = 0, umts_band = 0;
    int band_mode[10], index = 0;
    sp<RfxMclMessage> resp;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    if (result != MIPC_RESULT_SUCCESS) {
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
        responseToTelCore(resp);
        return;
    }

    // get supported GSM bands
    gsm_band = mipcData->getMipcUint32Val(MIPC_NW_GET_BAND_MODE_CNF_T_GSM_BAND, 0);

    // get supported UMTS bands
    umts_band = mipcData->getMipcUint32Val(MIPC_NW_GET_BAND_MODE_CNF_T_UMTS_BAND, 0);

    //0 for "unspecified" (selected by baseband automatically)
    band_mode[index++] = BAND_MODE_UNSPECIFIED;

    if (gsm_band !=0 || umts_band != 0) {
        // 1 for "EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
        if ((gsm_band == 0 || (gsm_band | MIPC_SYS_CAP_GSM_BAND_900 | MIPC_SYS_CAP_GSM_BAND_DCS_1800) == gsm_band) &&
                (umts_band == 0 || (umts_band | MIPC_SYS_CAP_UMTS_BAND_I) == umts_band)) {
            band_mode[index++] = BAND_MODE_EURO;
        }

        // 2 for "US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
        if ((gsm_band == 0 || (gsm_band | MIPC_SYS_CAP_GSM_BAND_PCS_850 | MIPC_SYS_CAP_GSM_BAND_PCS_1900) == gsm_band) &&
                (umts_band == 0 || (umts_band | MIPC_SYS_CAP_UMTS_BAND_II | MIPC_SYS_CAP_UMTS_BAND_V) == umts_band)) {
            band_mode[index++] = BAND_MODE_USA;
        }

        // 3 for "JPN band" (WCDMA-800 / WCDMA-IMT-2000)
        if ((umts_band | MIPC_SYS_CAP_UMTS_BAND_I | MIPC_SYS_CAP_UMTS_BAND_VI) == umts_band) {
            band_mode[index++] = BAND_MODE_JPN;
        }

        // 4 for "AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
        if ((gsm_band == 0 || (gsm_band | MIPC_SYS_CAP_GSM_BAND_900 | MIPC_SYS_CAP_GSM_BAND_DCS_1800)==gsm_band) &&
                (umts_band == 0 || (umts_band | MIPC_SYS_CAP_UMTS_BAND_I | MIPC_SYS_CAP_UMTS_BAND_V)==umts_band)) {
            band_mode[index++] = BAND_MODE_AUS;
        }

        // 5 for "AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
        if ((gsm_band == 0 || (gsm_band | MIPC_SYS_CAP_GSM_BAND_900 | MIPC_SYS_CAP_GSM_BAND_DCS_1800)==gsm_band) &&
                (umts_band == 0 || (umts_band | MIPC_SYS_CAP_UMTS_BAND_V)==umts_band)) {
            band_mode[index++] = BAND_MODE_AUS_2;
        }
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxIntsData(band_mode, index), msg, false);
    responseToTelCore(resp);
}

void RmmNwAsyncRequestHandler::handleSetBandModeRequest(const sp<RfxMclMessage>& msg) {
    uint32_t gsm_band, umts_band;
    sp<RfxMclMessage> resp;
    int *pInt = (int *)msg->getData()->getData();
    int req = pInt[0];
    switch (req) {
        case BAND_MODE_UNSPECIFIED: //"unspecified" (selected by baseband automatically)
            gsm_band = 0xff;
            umts_band = 0xffff;
            break;
        case BAND_MODE_EURO: //"EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
            gsm_band = MIPC_SYS_CAP_GSM_BAND_900 | MIPC_SYS_CAP_GSM_BAND_DCS_1800;
            umts_band = MIPC_SYS_CAP_UMTS_BAND_I;
            break;
        case BAND_MODE_USA: //"US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
            gsm_band = MIPC_SYS_CAP_GSM_BAND_PCS_850 | MIPC_SYS_CAP_GSM_BAND_PCS_1900;
            umts_band = MIPC_SYS_CAP_UMTS_BAND_II | MIPC_SYS_CAP_UMTS_BAND_V;
            break;
        case BAND_MODE_JPN: //"JPN band" (WCDMA-800 / WCDMA-IMT-2000)
            gsm_band = 0;
            umts_band = MIPC_SYS_CAP_UMTS_BAND_I | MIPC_SYS_CAP_UMTS_BAND_VI;
            break;
        case BAND_MODE_AUS: //"AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
            gsm_band = MIPC_SYS_CAP_GSM_BAND_900 | MIPC_SYS_CAP_GSM_BAND_DCS_1800;
            umts_band = MIPC_SYS_CAP_UMTS_BAND_I | MIPC_SYS_CAP_UMTS_BAND_V;
            break;
        case BAND_MODE_AUS_2: //"AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
            gsm_band = MIPC_SYS_CAP_GSM_BAND_900 | MIPC_SYS_CAP_GSM_BAND_DCS_1800;
            umts_band = MIPC_SYS_CAP_UMTS_BAND_V;
            break;
        case BAND_MODE_CELL_800: //"Cellular (800-MHz Band)"
        case BAND_MODE_PCS: //"PCS (1900-MHz Band)"
        case BAND_MODE_JTACS: //"Band Class 3 (JTACS Band)"
        case BAND_MODE_KOREA_PCS: //"Band Class 4 (Korean PCS Band)"
        case BAND_MODE_5_450M: //"Band Class 5 (450-MHz Band)"
        case BAND_MODE_IMT2000: // "Band Class 6 (2-GMHz IMT2000 Band)"
        case BAND_MODE_7_700M_2: //"Band Class 7 (Upper 700-MHz Band)"
        case BAND_MODE_8_1800M: //"Band Class 8 (1800-MHz Band)"
        case BAND_MODE_9_900M: //"Band Class 9 (900-MHz Band)"
        case BAND_MODE_10_800M_2: //"Band Class 10 (Secondary 800-MHz Band)"
        case BAND_MODE_EURO_PAMR_400M: //"Band Class 11 (400-MHz European PAMR Band)"
        case BAND_MODE_AWS: //"Band Class 15 (AWS Band)"
        case BAND_MODE_USA_2500M: //"Band Class 16 (US 2.5-GHz Band)"
        default:
            gsm_band = 0;
            umts_band = 0;
            break;
    }

    if (gsm_band != 0 && umts_band != 0) {
        sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_BAND_MODE_REQ, msg->getSlotId());
        mipcReq->addMipcTlvUint32(MIPC_NW_SET_BAND_MODE_REQ_T_GSM_BAND, gsm_band);
        mipcReq->addMipcTlvUint32(MIPC_NW_SET_BAND_MODE_REQ_T_UMTS_BAND, umts_band);
        callToMipcMsgAsync(msg, mipcReq);
    } else {
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_REQUEST_NOT_SUPPORTED, RfxVoidData(), msg, false);
        responseToTelCore(resp);
    }
}

void RmmNwAsyncRequestHandler::handleSetLteReleaseVersionRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;

    if (msg->getData()->getData() == NULL
        || msg->getData()->getDataLength() != sizeof(int)) {
        logE(LOG_TAG, "handleSetLteReleaseVersionRequest: invalid argument");
        response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ, msg->getSlotId());
    int mode = *((int *)msg->getData()->getData());
    uint8_t enable;
    if (mode)
        enable = MIPC_NW_LTE_CARRIER_ARRREGATION_SWITCH_TURNON;
    else
        enable = MIPC_NW_LTE_CARRIER_ARRREGATION_SWITCH_TURNOFF;
    reqData->addMipcTlvUint8(
            MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ_T_STATUS, enable);

    callToMipcMsgAsync(msg, reqData);
}

void RmmNwAsyncRequestHandler::handleGetLteReleaseVersionRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleGetLteReleaseVersionResponse(const sp<RfxMclMessage>& msg) {
    int mode = 0;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    RIL_Errno rilErrno = result == MIPC_RESULT_SUCCESS ? RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;

    mode = mipcData->getMipcUint8Val(MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_CNF_T_STATUS, 0);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrno, RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleSetBackgroundSearchTimerRequest(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int duration = pInt[0];
    logV(LOG_TAG, "requestSetBackgroundSearchTimer duration=%d", duration);

    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, String8::format("%s", "bgsrch_slp_delta"));
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA, String8::format("%d", duration));
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleGetPseudoCellInfoRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_PSEUDO_CELL_INFO_REQ,
            msg->getSlotId());
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleGetPseudoCellInfoResponse(const sp<RfxMclMessage>& msg) {
    int response[16] = {0};
    sp<RfxMclMessage> resp;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    uint8_t apcMode;
    uint8_t urcEnable;
    uint16_t timer;
    uint8_t num;

    if (result != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s, result error: %u", __FUNCTION__, result);
        goto error;
    }

    apcMode = mipcData->getMipcUint8Val(MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_APC_MODE, 0xFF);
    if (apcMode == 0xFF) goto error;
    response[0] = apcMode;
    urcEnable = mipcData->getMipcUint8Val(MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_URC_ENABLE, 0);
    response[1] = urcEnable;
    timer = mipcData->getMipcUint16Val(MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_TIMER, 0);
    response[2] = timer;
    num = mipcData->getMipcUint8Val(MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_CELL_COUNT, 0);
    response[3] = num;
    if (num > 0) {
        uint16_t length = 0;
        void * cellData = mipcData->getMipcVal(MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_LIST,
                &length);
        if (cellData == NULL || length != num * sizeof(mipc_nw_pseudocell_info_struct4)) {
            logE(LOG_TAG, "handleGetPseudoCellInfoReponse, length(%d) is wrong", length);
            goto error;
        }
        mipc_nw_pseudocell_info_struct4 * list = (mipc_nw_pseudocell_info_struct4 *) cellData;
        for (int i = 0; i < num; i++) {
            response[i*6 + 4] = list->type;
            response[i*6 + 5] = atoi(list->plmn);
            response[i*6 + 6] = list->lac;
            response[i*6 + 7] = (int) list->cell_id;
            response[i*6 + 8] = list->arfcn;
            response[i*6 + 9] = list->bsic;
            list ++;
        }
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(response, 16), msg, false);
    responseToTelCore(resp);
    return;

error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}


void RmmNwAsyncRequestHandler::handleSetPseudoCellModeRequest(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    uint8_t apc_mode = (uint8_t) pInt[0];
    uint8_t urc_enable = pInt[1] == 0 ? 0 : 1;
    uint16_t timer = (uint16_t) pInt[2];
   /*
    *  apc_mode = 0: disable APC feature
    *  apc_mode = 1: set APC mode I, if detect a pseudo cell, not attach it
    *  apc_mode = 2: set APC mode II, if detect a pseudo cell, also attach it
    */
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_PSEUDO_CELL_MODE_REQ,
            msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_APC_MODE, apc_mode);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_URC_ENABLE, urc_enable);
    mipcReq->addMipcTlvUint16(MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_TIMER, timer);
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleSetLocationUpdatesRequest(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    // enable->3, disalbe->5
    uint8_t enabled = 5;
    if (pInt && (pInt[0] == 1)) enabled = 3;
    logD(LOG_TAG, "[%s] enabled:%d", __FUNCTION__, enabled);
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ,
            msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ_T_CS_STATE_LEVEL, enabled);
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleSetServiceStateRequest(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    if (pInt && (pInt[0] >= 0 && pInt[0] <= 3)
        && (pInt[1] >= 0 && pInt[1] <= 3)
        && (pInt[2] >= 0 && pInt[2] <= 3)
        && (pInt[3] >= 0 && pInt[3] <= 3)
        && (pInt[4] >= 0 && pInt[4] <= 14)
        && (pInt[5] >= 0 && pInt[5] <= 14)) {
        mipc_nw_ps_cs_reg_roaming_info_struct4 info;
        info.m_voice_reg_state = pInt[0];
        info.m_data_reg_state = pInt[1];
        info.m_voice_roaming_type = pInt[2];
        info.m_data_roaming_type = pInt[3];
        info.m_ril_voice_reg_state = pInt[4];
        info.m_ril_data_reg_state = pInt[5];
        sp<RfxMipcData> mipcReq =
            RfxMipcData::obtainMipcData(MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ,
                                        msg->getSlotId());
        mipcReq->addMipcTlv(MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ_T_PS_CS_REG_ROAMING_INFO,
                            sizeof(info), &info);
        callToMipcMsgAsync(msg, mipcReq);
    } else {
        logE(LOG_TAG, "[%s], invalid parameter!", __FUNCTION__);
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}

void RmmNwAsyncRequestHandler::handleGetFemtocellListRequest(const sp<RfxMclMessage>& msg) {
    m_csgListOngoing = 1;
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_FEMTOCELL_LIST_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, mipcReq);

}

void RmmNwAsyncRequestHandler::handleGetFemtocellListResponse(const sp<RfxMclMessage>& msg) {

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    sp<RfxMclMessage> response;
    char **femtocellList = NULL;
    char *femtocell = NULL;
    int err;
    /* +ECSG: <num_plmn>,<plmn_id>,<act>,<num_csg>,<csg_id>,<csg_icon_type>,<hnb_name>,<sig>[,...]
       AT Response Example
       +ECSG: 3,"46000",2,1,<csg_id_A>,<csg_type_A>,<hnb_name_A>,<sig>,"46002",7,1,<csg_id_B>,<csg_type_B>,<hnb_name_B>,<sig>,"46002",7,1,<csg_id_C>,<csg_type_C>,<hnb_name_C>,<sig> */

    if (result == MIPC_RESULT_SUCCESS) {
        uint8_t num;
        uint16_t dataLen = 0;
        const int SIZE_FEMOTO = 7;
        // <num_plmn>
        num = mipcData->getMipcUint8Val(MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_CELL_COUNT, 0);
        // allocate memory
        femtocellList = (char **) calloc(1, sizeof(char*) * num * SIZE_FEMOTO);
        if (num > 0) {
            void * pData = mipcData->getMipcVal(
                    MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FEMTOCELL_LIST, &dataLen);
            if (pData == NULL || dataLen != num * sizeof(mipc_nw_femtocell_info_struct4)) {
                logE(LOG_TAG, "handleGetFemtocellListResponse, dataLen(%d) is wrong", dataLen);
                goto error;
            }
            femtocell = (char *) calloc(1, num* sizeof(char) * SIZE_FEMOTO * MAX_OPER_NAME_LENGTH);
            if (femtocellList == NULL || femtocell == NULL) {
                logE(LOG_TAG, "requestGetFemtocellList calloc fail");
                goto error;
            }
            mipc_nw_femtocell_info_struct4 * list = (mipc_nw_femtocell_info_struct4 *) pData;
            int i, j;
            for (i = 0, j = 0; i < num; i ++, j += SIZE_FEMOTO) {
                /* <plmn_id>,<act>,<num_csg>,<csg_id>,<csg_icon_type>,<hnb_name> */
                femtocellList[j+0] = &femtocell[(j+0)*MAX_OPER_NAME_LENGTH];
                femtocellList[j+1] = &femtocell[(j+1)*MAX_OPER_NAME_LENGTH];
                femtocellList[j+2] = &femtocell[(j+2)*MAX_OPER_NAME_LENGTH];
                femtocellList[j+3] = &femtocell[(j+3)*MAX_OPER_NAME_LENGTH];
                femtocellList[j+4] = &femtocell[(j+4)*MAX_OPER_NAME_LENGTH];
                femtocellList[j+5] = &femtocell[(j+5)*MAX_OPER_NAME_LENGTH];
                femtocellList[j+6] = &femtocell[(j+6)*MAX_OPER_NAME_LENGTH];

                /* get <plmn_id> numeric code*/
                err = snprintf(femtocellList[j+0],MAX_OPER_NAME_LENGTH, "%s", list->plmn_id);
                if (err < 0) goto error;
                if (strlen(list->plmn_id) <= 0) {
                    logE(LOG_TAG, "[%s]: plmn_id is wrong", __FUNCTION__);
                    goto error;
                }
                // long name
                matchPlmnName(femtocellList[j+1], NULL, list->plmn_id);

                /* get <AcT> 0 is "2G", 2 is "3G", 7 is "4G"*/
                err = snprintf(femtocellList[j+2], MAX_OPER_NAME_LENGTH, "%u", list->act);
                if (err < 0) goto error;

                /* get <csgId> */
                err = snprintf(femtocellList[j+3], MAX_OPER_NAME_LENGTH, "%u", list->csg_id);
                if (err < 0) goto error;

                /* get <csgIconType> */
                err = snprintf(femtocellList[j+4], MAX_OPER_NAME_LENGTH, "%u", list->csg_type);
                if (err < 0) goto error;

                /* get <hnbName> */
                err = snprintf(femtocellList[j+5], MAX_OPER_NAME_LENGTH, "%s", list->hnb_name);
                if (err < 0) goto error;

                /* get <sig> */
                err = snprintf(femtocellList[j+6], MAX_OPER_NAME_LENGTH, "%s", list->sig);
                if (err < 0) goto error;

                logD(LOG_TAG, "requestGetFemtocellList (%s, %s, %s, %s, %s, %s, %s)",
                        femtocellList[j+0],
                        femtocellList[j+1],
                        femtocellList[j+2],
                        femtocellList[j+3],
                        femtocellList[j+4],
                        femtocellList[j+5],
                        femtocellList[j+6]);
                list ++;
            }
        }
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringsData(femtocellList, num*SIZE_FEMOTO), msg, false);
        responseToTelCore(response);
        if (femtocellList) free(femtocellList);
        if (femtocell) free(femtocell);
        m_csgListOngoing = 0;
        m_csgListAbort =0; /* always clear here to prevent race condition scenario */
        return;
    }

error:
    if (femtocellList) free(femtocellList);
    if (femtocell) free(femtocell);
    logE(LOG_TAG, "requestGetFemtocellList must never return error when radio is on");
    if (m_csgListAbort == 1) {
        // requestGetFemtocellList is canceled
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_CANCELLED,
            RfxStringsData(), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxStringsData(), msg, false);
    }
    // response to TeleCore
    responseToTelCore(response);
    m_csgListOngoing = 0;
    m_csgListAbort =0; /* always clear here to prevent race condition scenario */

}

void RmmNwAsyncRequestHandler::handleAbortFemtocellListRequest(const sp<RfxMclMessage>& msg) {
    if (m_csgListOngoing == 1) {
        m_csgListAbort = 1;
       sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
                MIPC_NW_ABORT_FEMTOCELL_LIST_REQ, msg->getSlotId());
        callToMipcMsgAsync(msg, mipcReq);
    } else {
        sp<RfxMclMessage> response;
        sp<RfxAtResponse> p_response;
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
    }
}

void RmmNwAsyncRequestHandler::handleSelectFemtocellRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> p_response;
    sp<RfxMipcData> mipcReq = NULL;
    const char **strings = (const char **)msg->getData()->getData();
    //parameters:  <plmn> , <act> , <csg id>

    // check parameters
    if ((msg->getData()->getDataLength() < (int)(3 * sizeof(char*)))
            || (strings[0] == NULL)
            || (strings[1] == NULL)
            || (strings[2] == NULL)) {
        logE(LOG_TAG, "handleSelectFemtocellRequest parameters wrong datalen = %d",
                msg->getData()->getDataLength());
        goto error;
    }

    // check <plmn> is valid digit
    for (size_t i = 0; i < strlen(strings[0]); i++) {
        if (strings[0][i] < '0' || strings[0][i] > '9') {
            logE(LOG_TAG, "handleSelectFemtocellRequest parameters[0] wrong");
            goto error;
        }
    }

    // check <csg id>
    for (size_t i = 0; i < strlen(strings[2]); i++) {
        if (strings[2][i] < '0' || strings[2][i] > '9') {
            logE(LOG_TAG, "handleSelectFemtocellRequest parameters[2] wrong");
            goto error;
        }
    }
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_SELECT_FEMTOCELL_REQ, msg->getSlotId());
    mipcReq->addMipcTlvStr(MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_PLMN, strings[0]);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_ACT, (uint8_t) (atoi(strings[1])));
    mipcReq->addMipcTlvStr(MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_CSG_ID, strings[2]);
    callToMipcMsgAsync(msg, mipcReq);
    return;

error:
    logE(LOG_TAG, "requestSelectFemtocell parameter error");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleQueryFemtocellSystemSelectionModeRequest(
        const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
            MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleQueryFemtocellSystemSelectionModeReponse(
        const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    sp<RfxMclMessage> response;
    int mode = 0;
    RIL_Errno rilError = RIL_E_SUCCESS;
    if (result == MIPC_RESULT_SUCCESS) {
        mode = mipcData->getMipcUint8Val(
                MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF_T_MODE, 0);
    } else {
        rilError = RIL_E_GENERIC_FAILURE;
        uint16_t error = mipcData->getMipcUint8Val(
                MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF_T_FAIL_CAUSE, 0);
        logE(LOG_TAG, "handleQueryFemtocellSystemSelectionModeReponse: error: %u", error);
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), rilError,
            RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleSetFemtocellSystemSelectionModeRequest(
        const sp<RfxMclMessage>& msg) {

    int *pInt = (int *)msg->getData()->getData();
    uint8_t mode = (uint8_t) pInt[0];
    logD(LOG_TAG, "handleSetFemtocellSystemSelectionModeRequest: mode=%d", mode);

    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
                MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ_T_MODE, mode);
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwAsyncRequestHandler::handleConfigA2OffsetRequest(const sp<RfxMclMessage>& msg) {
    int *args = (int *)msg->getData()->getData();
    int argsCount = msg->getData()->getDataLength() / sizeof(int);
    sp<RfxMclMessage> response;

    if (args == NULL || argsCount != 2) {
        logE(LOG_TAG, "handleConfigA2OffsetRequest: invalid arguments");
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    int offset = args[0];
    int thresh = args[1];

    logD(LOG_TAG, "handleConfigA2OffsetRequest: offset[%d], thresh[%d]", offset, thresh);

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_SET_CONFIG_A2_OFFSET_REQ, msg->getSlotId());
    reqData->addMipcTlvUint8(
            MIPC_NW_SET_CONFIG_A2_OFFSET_REQ_T_OFFSET, (uint8_t)offset);
    reqData->addMipcTlvUint8(
            MIPC_NW_SET_CONFIG_A2_OFFSET_REQ_T_THRESH_BOUND, (uint8_t)thresh);
    callToMipcMsgAsync(msg, reqData);
}

void RmmNwAsyncRequestHandler::handleConfigB1OffsetRequest(const sp<RfxMclMessage>& msg) {
    int *args = (int *)msg->getData()->getData();
    int argsCount = msg->getData()->getDataLength() / sizeof(int);
    sp<RfxMclMessage> response;

    if (args == NULL || argsCount != 2) {
        logE(LOG_TAG, "handleConfigB1OffsetRequest: invalid arguments");
        response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }
    int offset = args[0];
    int thresh = args[1];

    logD(LOG_TAG, "handleConfigB2OffsetRequest: offset[%d], thresh[%d]", offset, thresh);

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_SET_CONFIG_B1_OFFSET_REQ, msg->getSlotId());
    reqData->addMipcTlvUint8(
            MIPC_NW_SET_CONFIG_B1_OFFSET_REQ_T_OFFSET, (uint8_t)offset);
    reqData->addMipcTlvUint8(
            MIPC_NW_SET_CONFIG_B1_OFFSET_REQ_T_THRESH_BOUND, (uint8_t)thresh);
    callToMipcMsgAsync(msg, reqData);
}

void RmmNwAsyncRequestHandler::handleSetDisable2GRequest(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int mode = pInt[0];
    logD(LOG_TAG, "handleSetDisable2GRequest: mode[%d]", mode);
    if (mode == 1 || mode == 0) {
        sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
                MIPC_NW_SET_DISABLE_2G_REQ, msg->getSlotId());
        reqData->addMipcTlvUint8(MIPC_NW_SET_DISABLE_2G_REQ_T_MODE, (uint8_t)mode);
        callToMipcMsgAsync(msg, reqData);
    } else {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}

void RmmNwAsyncRequestHandler::handleGetDisable2GRequest(const sp<RfxMclMessage>& msg) {
    logD(LOG_TAG, "handleGetDisable2GRequest");
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_DISABLE_2G_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, reqData);
}

void RmmNwAsyncRequestHandler::handleGetDisable2GResponse(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    RIL_Errno rilErrno = result == MIPC_RESULT_SUCCESS ? RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;

    int mode = mipcData->getMipcUint8Val(MIPC_NW_GET_DISABLE_2G_CNF_T_MODE, 0);
    logD(LOG_TAG, "handleGetDisable2GResponse, mode[%d]", mode);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrno, RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::handleNwOemHookRawRequest(const sp<RfxMclMessage>& msg) {
    char* request = (char*) (msg->getData()->getData());
    if (!request) goto error;
    logD(LOG_TAG, "handleNwOemHookRawRequest: %s", request);
    if (strStartsWith(request, "AT+E5GOPT=")) {
        setNrOptionRequest(msg);
    } else if (strStartsWith(request, "AT+E5GOPT?")) {
        getNrOptionRequest(msg);
    } else if (strStartsWith(request, "AT+ENRCACOMB=")) {
        setNrCaCombRequest(msg);
    } else if (strStartsWith(request, "AT+ENRCACFG=")) {
        setNrCaCfgRequest(msg);
    } else {
        logE(LOG_TAG, "unsupport request: %s", request);
        goto error;
    }
    return;
error:
    logE(LOG_TAG, "%s error happens at line %d", __FUNCTION__, __LINE__);
    sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_REQUEST_NOT_SUPPORTED, RfxRawData(), msg, false);
    responseToTelCore(resp);
}

void RmmNwAsyncRequestHandler::setNrOptionRequest(const sp<RfxMclMessage>& msg) {
    int err = 0, opt = 0, release = -1;
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    sp<RfxMipcData> mipcReq, mipcRsp;
    sp<RfxMclMessage> response;
    char ok[2] = {'O', 'K'};
    char* data = (char*) (msg->getData()->getData());
    RfxAtLine* line = new RfxAtLine(data, NULL);
    if (!line)  {
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        logE(LOG_TAG, "%s error happens at line %d", __FUNCTION__, __LINE__);
        goto error;
    }
    // go to start position. '='
    line->atTokEqual(&err);
    if (err < 0) {
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        logE(LOG_TAG, "%s error happens at line %d", __FUNCTION__, __LINE__);
        goto error;
    }

    opt = line->atTokNextint(&err);
    if (err < 0) {
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        logE(LOG_TAG, "%s error happens at line %d", __FUNCTION__, __LINE__);
        goto error;
    }
    if (line->atTokHasmore()) {
        release = line->atTokNextint(&err);
        if (err < 0) {
            ril_errno = RIL_E_INVALID_ARGUMENTS;
            logE(LOG_TAG, "%s error happens at line %d", __FUNCTION__, __LINE__);
            goto error;
        }
    }
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_NR_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_NR_REQ_T_NR_OPT, opt);
    if (release !=-1) {
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_NR_REQ_T_ACT_OPERATION, release);
    }
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        ril_errno = RIL_E_MODE_NOT_SUPPORTED;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    RIL_E_SUCCESS, RfxRawData(ok, sizeof(ok)/sizeof(char)), msg, false);
    responseToTelCore(response);
    delete line;
    return;

error:
    if (line) delete line;
    logE(LOG_TAG, "%s: error happens: %d", __FUNCTION__, ril_errno);
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    ril_errno, RfxRawData(), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::getNrOptionRequest(const sp<RfxMclMessage>& msg){
    int opt = 0xFF;
    int err = 0;
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    char* result = NULL;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_NR_REQ, msg->getSlotId());
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        ril_errno = RIL_E_MODE_NOT_SUPPORTED;
        goto error;
    }
    opt = mipcRsp->getMipcUint8Val(MIPC_NW_GET_NR_CNF_T_NR_OPT, 0xFF);
    if (opt == 0xFF) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    err = asprintf(&result, "+E5GOPT: %d\r\nOK", opt);
    if (err < 0 || (result == NULL)) {
        ril_errno = RIL_E_NO_MEMORY;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    ril_errno, RfxRawData(result, strlen(result)), msg, false);
    responseToTelCore(response);
    free(result);
    return;

error:
    logE(LOG_TAG, "%s: error happens", __FUNCTION__);
    if (result != NULL) {
        free(result);
        result = NULL;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    ril_errno, RfxRawData(), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::setNrCaCfgRequest(const sp<RfxMclMessage>& msg) {
    int err = 0, value, valid = 0;
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int tlvArray[8] = {
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TT_SA_ENABLE,
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TF_SA_ENABLE,
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FT_SA_ENABLE,
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FF_SA_ENABLE,
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TT_NSA_ENABLE,
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TF_NSA_ENABLE,
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FT_NSA_ENABLE,
        MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FF_NSA_ENABLE
    };
    sp<RfxMipcData> mipcReq, mipcRsp;
    sp<RfxMclMessage> response;
    char ok[2] = {'O', 'K'};
    char* data = (char*) (msg->getData()->getData());
    RfxAtLine* line = new RfxAtLine(data, NULL);
    if (!line)  {
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        logE(LOG_TAG, "%s error happens when create AtLine", __FUNCTION__);
        goto error;
    }
    // go to start position. '='
    line->atTokEqual(&err);
    if (err < 0) {
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        logE(LOG_TAG, "%s error happens when go to =", __FUNCTION__);
        goto error;
    }
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_CAP_NRCA_OPTION_REQ, msg->getSlotId());
    for (int i = 0; i < 8; i ++) {
        value = line->atTokNextint(&err);
        if (err == 0 && (value == 0 || value == 1)) {
            mipcReq->addMipcTlvUint8(tlvArray[i], value);
            logV(LOG_TAG, "%s: add a tlv as %d for index = %d", __FUNCTION__, value, i);
            valid++;
        }
    }
    logV(LOG_TAG, "%s: parse completed, valid = %d", __FUNCTION__, valid);

    if (valid == 0) {
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        logE(LOG_TAG, "%s no valid configure", __FUNCTION__);
        goto error;
    }
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] callToMipcMsgSync, result = %d", __FUNCTION__, __LINE__,
                mipcRsp->getResult());
        ril_errno = RIL_E_MODE_NOT_SUPPORTED;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    RIL_E_SUCCESS, RfxRawData(ok, sizeof(ok)/sizeof(char)), msg, false);
    responseToTelCore(response);
    delete line;
    return;

error:
    if (line) delete line;
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    ril_errno, RfxRawData(), msg, false);
    responseToTelCore(response);
}

void RmmNwAsyncRequestHandler::setNrCaCombRequest(const sp<RfxMclMessage>& msg) {
    /*
    AT+ENRCACOMB=<isAll>,<tt_sa_count>[, tt_sa_item,enable][,...],
                        <tf_sa_count>[, tf_sa_item,enable][,...],
                        <ft_sa_count>[, ft_sa_item,enale][,...],
                        <ff_sa_count>[, ff_sa_item,enale][,...],
                        <tt_nsa_count>[, tt_nsa_item,enable][,...],
                        <tf_nsa_count>[, tf_nsa_item,enable][,...],
                        <ft_nsa_count>[, ft_nsa_item,enale][,...],
                        <ff_nsa_count>[, ff_nsa_item,enale][,...]
    */
    int err = 0, totalNumer = 0;
    uint8_t enable, isAll = 1;
    int reqCatTlvArray[8] = {
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_SA_SRC_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_SA_SRC_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_SA_SRC_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_SA_SRC_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_NSA_SRC_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_NSA_SRC_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_NSA_SRC_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_NSA_SRC_TLV_ARRAY
    };

    int reqOptTlvArray[8] = {
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_SA_OPTION_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_SA_OPTION_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_SA_OPTION_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_SA_OPTION_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_NSA_OPTION_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_NSA_OPTION_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_NSA_OPTION_TLV_ARRAY,
        MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_NSA_OPTION_TLV_ARRAY
    };

    char* combine;
    RIL_Errno ril_errno = RIL_E_INVALID_ARGUMENTS;
    sp<RfxMipcData> mipcReq, mipcRsp, mipcReqArray[8] = {NULL};
    int mipcReqArrayCount = 0;
    sp<RfxMclMessage> response;
    mipc_msg_api_result_enum result;
    char ok[2] = {'O', 'K'};
    char* data = (char*) (msg->getData()->getData());

    RfxAtLine* line = new RfxAtLine(data, NULL);
    if (!line)  {
        logE(LOG_TAG, "%s error happens when create AtLine", __FUNCTION__);
        goto error;
    }

    // go to start position. '='
    line->atTokEqual(&err);
    if (err < 0) {
        logE(LOG_TAG, "%s error happens when go to =", __FUNCTION__);
        goto error;
    }

    isAll = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "%s error happens when get <isAll>", __FUNCTION__);
        goto error;
    }
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_CAP_BC_LIST_REQ, m_slot_id);
    mipcReqArray[mipcReqArrayCount] = mipcReq;
    mipcReqArrayCount++;
    logV(LOG_TAG, "%s: create a new mipcReq, mipcReqArrayCount = %d", __FUNCTION__, mipcReqArrayCount);

    for (int i = 0; i < 8; i++) {
        int count = line->atTokNextint(&err);
        if (err < 0) {
            continue;
        }
        logV(LOG_TAG, "%s: count = %d for i = %d", __FUNCTION__, count, i);
        if (count > 0) {
            if (totalNumer + count > MIPC_MAX_BC_LIST_COUNT) {
                // in order to make sure the same type in one request
                if (totalNumer == 0) {
                    logE(LOG_TAG, "%s error happens, the number of the type exceed (%d), i = %d",
                                __FUNCTION__,  MIPC_MAX_BC_LIST_COUNT, i);
                    goto error;
                }
                mipcReq->addMipcTlvUint8(MIPC_NW_SET_CAP_BC_LIST_REQ_T_IS_XML_PARSING_DONE, MIPC_BOOLEAN_FALSE);
                logV(LOG_TAG, "%s: add parse done as false for the %d mipc request", __FUNCTION__, mipcReqArrayCount);
                if (mipcReqArrayCount == 8) {
                    logE(LOG_TAG, "%s error happens, request is too much, can not handle it(%d), i = %d",
                                __FUNCTION__,  MIPC_MAX_BC_LIST_COUNT, i);
                    goto error;
                }
                // re-create a new request
                mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_CAP_BC_LIST_REQ, m_slot_id);
                mipcReqArray[mipcReqArrayCount] = mipcReq;
                mipcReqArrayCount++;
                totalNumer = 0;
                logV(LOG_TAG, "%s: create a new mipcReq, mipcReqArrayCount = %d", __FUNCTION__, mipcReqArrayCount);
            }
            totalNumer += count;
            mipc_msg_tlv_array_t *pCatArray = mipc_msg_add_tlv_array(mipcReq->getData(),
            reqCatTlvArray[i], count);
            mipc_msg_tlv_array_t *pOptArray = mipc_msg_add_tlv_array(mipcReq->getData(),
            reqOptTlvArray[i], count);
            for (int j = 0; j < count; j++) {
                combine = line->atTokNextstr(&err);
                if (err < 0 || strlen(combine) == 0) {
                    logE(LOG_TAG, "%s error happens when get combine(%s), i = %d, j = %d",
                            __FUNCTION__, combine, i, j);
                    goto error;
                }
                result = mipc_msg_add_idx(mipcReq->getData(), pCatArray, j, strlen(combine) + 1, combine);
                if (result != MIPC_MSG_API_RESULT_SUCCESS) {
                    logE(LOG_TAG, "%s error happens when add combine(%s), i = %d, j = %d",
                            __FUNCTION__, combine, i, j);
                    goto error;
                }

                enable = line->atTokNextint(&err);
                if (err < 0) {
                    logE(LOG_TAG, "%s error happens when get enable, i = %d, j = %d",
                            __FUNCTION__, i, j);
                    goto error;
                }
                result = mipc_msg_add_idx(mipcReq->getData(), pOptArray, j, sizeof(enable), &enable);
                if (result != MIPC_MSG_API_RESULT_SUCCESS) {
                    logE(LOG_TAG, "%s error happens when add enable(%s), i = %d, j = %d",
                            __FUNCTION__, combine, i, j);
                    goto error;
                }
                logV(LOG_TAG, "add combine %s, enable = %d", combine, enable);
            }
        }
    }
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_CAP_BC_LIST_REQ_T_IS_XML_PARSING_DONE,
            isAll ? MIPC_BOOLEAN_TRUE: MIPC_BOOLEAN_FALSE);
    logV(LOG_TAG, "%s: add parse done as %s for the %d mipc request", __FUNCTION__,
            isAll ? "true" : "false", mipcReqArrayCount);

    // send all requests one by one
    logV(LOG_TAG, "%s: parse completed, mipcReqArrayCount = %d", __FUNCTION__, mipcReqArrayCount);
    for (int num = 0; num < mipcReqArrayCount; num ++) {
        mipcRsp = callToMipcMsgSync(mipcReqArray[num]);
        if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
            logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
            ril_errno = RIL_E_MODE_NOT_SUPPORTED;
            goto error;
        }
    }
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    RIL_E_SUCCESS, RfxRawData(ok, sizeof(ok)/sizeof(char)), msg, false);
    responseToTelCore(response);
    delete line;
    return;

error:
    if (line) delete line;
    response = RfxMclMessage::obtainResponse(msg->getId(),
                    ril_errno, RfxRawData(), msg, false);
    responseToTelCore(response);
}

