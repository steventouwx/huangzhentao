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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmmSmlRequestHandler.h"
#include "RmmSimBaseHandler.h"
#include "RmmCommSimDefs.h"
#include "RmmSmlDefs.h"
#include "RmmSmlRequestHandler.h"
#include "RfxRsuRequestData.h"
#include "RfxRsuResponseData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxSimMeLockCatData.h"
#include "RfxVoidData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include "RfxSimCarrRstData.h"

#include <telephony/mtk_ril.h>
#include <time.h>
#include <sys/time.h>
#include <libmtkrilutils.h>
#include <prop/properties.h>

using ::android::String8;
using namespace std;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmSmlRequestHandler, RIL_CMD_PROXY_1);

static const int ch1ReqList[] = {
    RFX_MSG_REQUEST_SIM_ENTER_NETWORK_DEPERSONALIZATION,
    RFX_MSG_REQUEST_SIM_ENTER_DEPERSONALIZATION,
    RFX_MSG_REQUEST_ENTER_SIM_DEPERSONALIZATION,
    RFX_MSG_REQUEST_GET_ALLOWED_CARRIERS,
    RFX_MSG_REQUEST_SET_ALLOWED_CARRIERS,
    RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK,
    RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK,
    RFX_MSG_REQUEST_SIM_ENTER_DEVICE_NETWORK_DEPERSONALIZATION,
    RFX_MSG_REQUEST_SML_RSU_REQUEST,
    //from op radio
//    RFX_MSG_REQUEST_SEND_RSU_REQUEST,
    RFX_MSG_REQUEST_CLIENT_RSU,

    /**
     * XXX_SUBLOCK_XXX are hidl only API, and they are implemented by
     * RFX_MSG_REQUEST_SML_RSU_REQUEST in aidl client.
     * All logic related to these API can be removed when hidl phase out.
     */
    RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS,
    RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS,
    RFX_MSG_REQUEST_GET_SUBLOCK_VERSION,
    RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME,
};

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_SIM_ENTER_NETWORK_DEPERSONALIZATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_SIM_ENTER_DEPERSONALIZATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_ENTER_SIM_DEPERSONALIZATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimCarrRstData, RfxIntsData,
        RFX_MSG_REQUEST_SET_ALLOWED_CARRIERS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxSimCarrRstData,
        RFX_MSG_REQUEST_GET_ALLOWED_CARRIERS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxSimMeLockCatData,
        RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData,
        RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_SIM_ENTER_DEVICE_NETWORK_DEPERSONALIZATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRsuRequestData, RfxRsuResponseData,
        RFX_MSG_REQUEST_SML_RSU_REQUEST);
//RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRsuRequestData, RfxRsuResponseData,
//        RFX_MSG_REQUEST_SEND_RSU_REQUEST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxRsuResponseData,
        RFX_MSG_REQUEST_CLIENT_RSU);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_GET_SUBLOCK_VERSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME);
/*****************************************************************************
 * RmmSmlRequestHandler
 *****************************************************************************/

RmmSmlRequestHandler::RmmSmlRequestHandler(int slot_id, int channel_id) :
        RmmSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmmSmlRequest"));

    if (channel_id == RIL_CMD_PROXY_1) {
        registerToHandleRequest(ch1ReqList, sizeof(ch1ReqList)/sizeof(int));
    }
    const int events[] = {
        RFX_MSG_EVENT_SIM_RESET,
    };
    registerToHandleEvent(events, sizeof(events)/sizeof(int));
}

RmmSmlRequestHandler::~RmmSmlRequestHandler() {
}

void RmmSmlRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    logD(mTag, "onHandleRequest: msg: %d, token:%d", request, msg->getToken());
    switch(request) {
        case RFX_MSG_REQUEST_SIM_ENTER_NETWORK_DEPERSONALIZATION:
            handleEnterNetworkDepersonalization(msg);
            break;
        case RFX_MSG_REQUEST_SIM_ENTER_DEPERSONALIZATION:
            handleEnterDepersonalization(msg);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_DEPERSONALIZATION:
            handleEnterSimDepersonalization(msg);
            break;
        case RFX_MSG_REQUEST_SET_ALLOWED_CARRIERS:
            handleSetAllowedCarriers(msg);
            break;
        case RFX_MSG_REQUEST_GET_ALLOWED_CARRIERS:
            handleGetAllowedCarriers(msg);
            break;
        case RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK:
            handleQuerySimNetworkLock(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK:
            handleSetSimNetworkLock(msg);
            break;
        case RFX_MSG_REQUEST_SIM_ENTER_DEVICE_NETWORK_DEPERSONALIZATION:
            handleEnterDeviceNetworkDepersonalization(msg);
            break;
        case RFX_MSG_REQUEST_SML_RSU_REQUEST:
            handleRsuRequest(msg);
            break;
        case RFX_MSG_REQUEST_CLIENT_RSU:
            handleRsuClientRequest(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS:
            handleReqSubsidyLockUpdateSettings(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS:
            handleReqSubsidyLockGetModemStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_VERSION:
            handleReqSubsidyLockGetVersion(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME:
            handleReqSubsidyLockGetUnlockTime(msg);
            break;
        default:
            logE(mTag, "Not support the request!");
            break;
    }
}

void RmmSmlRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    logD(mTag, "onHandleResponse: msg: %d, token:%d", request, msg->getToken());
    switch(request) {
        case RFX_MSG_REQUEST_SIM_ENTER_NETWORK_DEPERSONALIZATION:
            handleEnterNetworkDepersonalizationResp(msg);
            break;
        case RFX_MSG_REQUEST_SIM_ENTER_DEPERSONALIZATION:
            handleEnterDepersonalizationResp(msg);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_DEPERSONALIZATION:
            handleEnterSimDepersonalizationResp(msg);
            break;
        case RFX_MSG_REQUEST_SET_ALLOWED_CARRIERS:
            handleSetAllowedCarriersResp(msg);
            break;
        case RFX_MSG_REQUEST_GET_ALLOWED_CARRIERS:
            handleGetAllowedCarriersResp(msg);
            break;
        case RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK:
            handleQuerySimNetworkLockResp(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK:
            handleSetSimNetworkLockResp(msg);
            break;
        case RFX_MSG_REQUEST_SIM_ENTER_DEVICE_NETWORK_DEPERSONALIZATION:
            handleEnterDeviceNetworkDepersonalizationResp(msg);
            break;
        case RFX_MSG_REQUEST_SML_RSU_REQUEST:
            handleRsuRequestResp(msg);
            break;
        case RFX_MSG_REQUEST_CLIENT_RSU:
            handleRsuClientRequestResp(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS:
            handleReqSubsidyLockUpdateSettingsResp(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS:
            handleReqSubsidyLockGetModemStatusResp(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_VERSION:
            handleReqSubsidyLockGetVersionResp(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME:
            handleReqSubsidyLockGetUnlockTimeResp(msg);
            break;
        case RFX_MSG_EVENT_SIM_RESET:
            logD(mTag, "RFX_MSG_EVENT_SIM_RESET done!");
            break;
        default:
            logE(mTag, "Not support the request!");
            break;
    }
}

void RmmSmlRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();
    switch(event) {
        case RFX_MSG_EVENT_SIM_RESET:
            handleEventResetSim(msg);
            break;
        default:
            logD(mTag, "onHandleEvent, Not support the event %d!", event);
            break;
    }
}

/*
 * Depersonalize the current sim lock which block the operation.
 *
 * Format of message input data
 *  string array: [key]
 *
 * Format of message response data:
 *  int32 array: [remainRetry]
 */
void RmmSmlRequestHandler::handleEnterNetworkDepersonalization(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_INVALID_ARGUMENTS;
    int retryCount = -1;
    RfxBaseData *rbd = msg->getData();
    RFX_ASSERT(rbd != NULL);
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ,
            msg->getSlotId());
    const char**  strings = (const char**)rbd->getData();

    if (strings == NULL) {
        logE(mTag, "handleEnterNetworkDepersonalization: strings is NULL!");
        goto error;
    }
    if (strings[0] == NULL) {
        logE(mTag, "handleEnterNetworkDepersonalization: strings[0] is NULL!");
        goto error;
    }

    logD(mTag, "handleEnterNetworkDepersonalization: ***");

    //CAT UNKNOWN means to depersonalize the current sim lock
    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_CATEGORY,
            MIPC_SIM_SML_CATEGORY_CAT_UNKNOWN);
    initMipc->addMipcTlvStr(MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_PIN_CODE, strings[0]);
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;

error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&retryCount, sizeof(retryCount)), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleEnterNetworkDepersonalizationResp(const sp<RfxMclMessage>& msg)
 {
    RfxMipcData* mipcData = msg->getMipcData();
    uint8_t category = 0;
    int retryCount = -1;
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleEnterNetworkDepersonalizationResp: error=%d", mipcRet);
        if (mipcRet == MIPC_RESULT_SIM_EXT_INCORRECT_PASS_WORD) {
            ret = RIL_E_PASSWORD_INCORRECT;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }

    category = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_CATEGORY,
            255);
    uint8_t remain = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_REMAIN_COUNT,
             255);
    if (remain != 255) {
        retryCount = remain;
    }
    logD(mTag, "handleEnterNetworkDepersonalizationResp: category=%d, remain=%d",
            category, retryCount);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&retryCount, sizeof(retryCount)), msg, false);
    responseToTelCore(response);

}

/**
 * Convert the PersoSubState defined in IRadio to cat in +ESMLCK
 * perso_substate: value of PersoSubState
 * return: cat
 *         -1  -- not supported PersoSubState
 */
int RmmSmlRequestHandler::convertPersoStateToCat(int perso_substate) {
    int cat = -1;
    switch (perso_substate) {
        case SIM_NETWORK:
        case SIM_NETWORK_PUK:
            cat = 0;
            break;
        case SIM_NETWORK_SUBSET:
        case SIM_NETWORK_SUBSET_PUK:
            cat = 1;
            break;
        case SIM_SERVICE_PROVIDER:
        case SIM_SERVICE_PROVIDER_PUK:
            cat = 2;
            break;
        case SIM_CORPORATE:
        case SIM_CORPORATE_PUK:
            cat = 3;
            break;
        case SIM_SIM:
        case SIM_SIM_PUK:
            cat = 4;
            break;
        case SIM_NS_SP:
        case SIM_NS_SP_PUK:
            cat = 5;
            break;
        default:
            break;

    };
    return cat;
}

/**
 * Convert the cat in +ESMLCK to PersoSubState defined in IRadio
 * cat: cat in +ESMLCK
 * return: persoSubState
 */
int RmmSmlRequestHandler::convertCatToPersoState(int cat) {
    int persoSubState = UNKNOWN;
    switch (cat) {
        case AT_ESMLCK_CAT_N:
            persoSubState = SIM_NETWORK;
            break;
        case AT_ESMLCK_CAT_NS:
            persoSubState = SIM_NETWORK_SUBSET;
            break;
        case AT_ESMLCK_CAT_SP:
            persoSubState = SIM_SERVICE_PROVIDER;
            break;
        case AT_ESMLCK_CAT_C:
            persoSubState = SIM_CORPORATE;
            break;
        case AT_ESMLCK_CAT_SIM:
            persoSubState = SIM_SIM;
            break;
        case SIM_NS_SP_PUK:
            persoSubState = SIM_NS_SP;
            break;
        default:
            break;

    };
    return persoSubState;
}

/*
 * Format of message input data
 *  string array: [key, persoSubState]
 *
 * Format of message response data:
 *  int32 array: [persoSubState, remainRetry]
 */

void RmmSmlRequestHandler::handleEnterSimDepersonalization(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_INVALID_ARGUMENTS;
    int retryCount = -1;
    char* key = NULL;
    int persoSubState = -1;
    int cat = -1;
    char** strings = (char**)(msg->getData()->getData());
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ,
            msg->getSlotId());

    if (strings == NULL || strings[0] == NULL || strings[1] == NULL) {
         ret = RIL_E_INVALID_ARGUMENTS;
         logD(mTag, "handleEnterSimDepersonalization invalid arguments.");
         goto error;
    }

    key = (char*)strings[0];
    persoSubState = atoi(strings[1]);
    if (persoSubState < SIM_NETWORK || persoSubState > SIM_NS_SP_PUK) {
         ret = RIL_E_INVALID_ARGUMENTS;
         logD(mTag, "handleEnterSimDepersonalization invalid arguments.");
         goto error;
    }
    cat = convertPersoStateToCat(persoSubState);
    if (cat < 0) {
        ret = RIL_E_REQUEST_NOT_SUPPORTED;
        logE(mTag, "not support persoSubState = %d", persoSubState);
        goto error;
    }

    logD(mTag, "handleEnterSimDepersonalization: ***");

    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_CATEGORY,
            cat);
    initMipc->addMipcTlvStr(MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_PIN_CODE, key);
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;

error:
    //[personSubState, remainingCount]
    int responseData[2];
    responseData[0] = persoSubState;
    responseData[1] = retryCount;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&responseData, sizeof(responseData)), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleEnterSimDepersonalizationResp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    uint8_t cat = 0;
    int persoSubState = UNKNOWN;
    int retryCount = -1;
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleEnterNetworkDepersonalizationResp: error=%d", mipcRet);
        if (mipcRet == MIPC_RESULT_SIM_EXT_INCORRECT_PASS_WORD) {
            ret = RIL_E_PASSWORD_INCORRECT;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }
    //Get category and remainRetry even at failure for these values should be returned.
    cat = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_CATEGORY,
            255);
    persoSubState = convertCatToPersoState(cat);
    uint8_t remain = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_REMAIN_COUNT,
             255);
    if (remain != 255) {
        retryCount = remain;
    }
    logD(mTag, "handleEnterNetworkDepersonalizationResp: perso=%d, remain=%d",
            persoSubState, retryCount);

    //[personSubState, remainingCount]
    int responseData[2];
    responseData[0] = persoSubState;
    responseData[1] = retryCount;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&responseData, sizeof(responseData)), msg, false);
    responseToTelCore(response);
}

/**
 * Customer specific operation, support some additional cats.
 * Format of message input data
 *  string array: [key, persoSubState]
 *
 * Format of message response data:
 *  int32 array: [remainRetry]
 */
void RmmSmlRequestHandler::handleEnterDepersonalization(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_INTERNAL_ERR;
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ,
            msg->getSlotId());
    int retryCount = -1;
    char* key = NULL;
    int persoSubState = -1;
    int cat = -1;
    char** strings = (char**)(msg->getData()->getData());
    if (strings == NULL || strings[0] == NULL || strings[1] == NULL) {
         ret = RIL_E_INVALID_ARGUMENTS;
         logD(mTag, "handleEnterDepersonalization invalid arguments.");
         goto error;
    }

    key = (char*)strings[0];
    persoSubState = atoi(strings[1]);
    if (persoSubState < SIM_NETWORK || persoSubState > SIM_NS_SP_PUK) {
         ret = RIL_E_INVALID_ARGUMENTS;
         logD(mTag, "handleEnterDepersonalization invalid arguments.");
         goto error;
    }
    cat = convertPersoStateToCat(persoSubState);
    if (persoSubState >= 8 && persoSubState <= 11) {
        cat = 9;
    }
    if (cat < 0) {
        ret = RIL_E_REQUEST_NOT_SUPPORTED;
        logE(mTag, "handleEnterDepersonalization: not support persoSubState = %d", persoSubState);
        goto error;
    }

    logD(mTag, "handleEnterDepersonalization: ***");

    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_CATEGORY,
            cat);
    initMipc->addMipcTlvStr(MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_PIN_CODE, key);
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;

error:
    //[personSubState, remainingCount]
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&retryCount, sizeof(retryCount)), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleEnterDepersonalizationResp(const sp<RfxMclMessage>& msg) {
    logD(mTag, "handleEnterDepersonalizationResp");
    handleEnterNetworkDepersonalizationResp(msg);

    RfxMipcData* mipcData = msg->getMipcData();
    uint16_t dataLen = 0;
    uint8_t *remainArray = (uint8_t *)mipcData->getMipcVal(
            MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_REMAIN_COUNT_LIST,
            &dataLen);
    logD(mTag, "handleEnterDepersonalizationResp dataLen=%d", dataLen);
    if (remainArray != NULL && dataLen > 0) {
        String8 lockCount("gsm.sim");
        lockCount.append((m_slot_id == 0)? ".num.simlock" : String8::format("%d%s", m_slot_id,
                ".num.simlock"));

        String8 value(String8::format("%d", remainArray[0]));
        for (int j = 1; j < dataLen; j++) {
            value.append(",");
            value.append(String8::format("%d", remainArray[j]));
        }

        rfx_property_set(lockCount, value.string());
        logD(mTag, "handleEnterDepersonalizationResp lockCount: %s, value: %s",
                lockCount.string(), value.string());
    }

}

/**
 * MAX length of members of mipc_sim_carrier_struct_struct4
 */
#define MAX_MCC_MNC_LEN     4
#define MAX_MATCH_DATA_LEN  21

/**
 * Allocate array of mipc_sim_carrier_struct_struct4 with @count items. And initialize it by
 * @pRilCarriers
 *
 * return
 *      allocated array on success
 *      NULL on failure.
 */
mipc_sim_carrier_struct_struct4* RmmSmlRequestHandler::
        allocMipcCarriers(const RIL_Carrier *pRilCarriers, int count, RIL_Errno &error) {
    if (pRilCarriers == NULL || count <= 0) {
        logE(mTag, "allocMipcCarriers: no need to allocate.");
        error = RIL_E_INVALID_ARGUMENTS;
        return NULL;
    }

    //validated the data
    int i=0;
    for (i=0; i<count; i++) {
        if (pRilCarriers[i].mcc == NULL || strlen(pRilCarriers[i].mcc) >= MAX_MCC_MNC_LEN) {
            logE(mTag, "allocMipcCarriers: invalid mcc.");
            error = RIL_E_INVALID_ARGUMENTS;
            return NULL;
        }
        if (pRilCarriers[i].mnc == NULL || strlen(pRilCarriers[i].mnc) >= MAX_MCC_MNC_LEN) {
            logE(mTag, "allocMipcCarriers: invalid mnc.");
            error = RIL_E_INVALID_ARGUMENTS;
            return NULL;
        }
        if (pRilCarriers[i].match_data != NULL
                && strlen(pRilCarriers[i].match_data) >= MAX_MATCH_DATA_LEN) {
            logE(mTag, "allocMipcCarriers: invalid match_data.");
            error = RIL_E_INVALID_ARGUMENTS;
            return NULL;
        }
    }
    mipc_sim_carrier_struct_struct4 *pMipcCarriers =
            (mipc_sim_carrier_struct_struct4 *)calloc(count, sizeof(mipc_sim_carrier_struct_struct4));
    if (pMipcCarriers == NULL) {
        logE(mTag, "allocMipcCarriers: no memory!!");
        error = RIL_E_RADIO_NOT_AVAILABLE;
        return NULL;
    }

    for (i=0; i<count; i++) {
        strcpy(pMipcCarriers[i].mcc, pRilCarriers[i].mcc);
        strcpy(pMipcCarriers[i].mnc, pRilCarriers[i].mnc);
        if (pRilCarriers[i].match_data != NULL) {
            strcpy(pMipcCarriers[i].match_data, pRilCarriers[i].match_data);
        }
        pMipcCarriers[i].match_cat = pRilCarriers[i].match_type;

    }
    return pMipcCarriers;
}

void RmmSmlRequestHandler::freeMipcCarriers(mipc_sim_carrier_struct_struct4 *pMipcCarriers) {
    if (pMipcCarriers != NULL) {
        free(pMipcCarriers);
    }
}

/**
 * Message input format
 *  RIL_CarrierRestrictionsWithPriority
 */
void RmmSmlRequestHandler::handleSetAllowedCarriers(const sp < RfxMclMessage > & msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RIL_CarrierRestrictionsWithPriority *crp = (RIL_CarrierRestrictionsWithPriority*)(
            msg->getData()->getData());
    mipc_sim_carrier_struct_struct4 *pAllowedCarriers = NULL;
    mipc_sim_carrier_struct_struct4 *pExcludedCarriers = NULL;

    logD(mTag, "handleSetAllowedCarriers: MultiSimPolicy %d,  Prioritized %d",
            crp->simLockMultiSimPolicy, crp->allowedCarriersPrioritized);

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ,
            msg->getSlotId());
    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_MULTI_SIM_POLICY,
            crp->simLockMultiSimPolicy);
    uint8_t allowedCarriersPrioritized =
            crp->allowedCarriersPrioritized ? MIPC_BOOLEAN_TRUE : MIPC_BOOLEAN_FALSE;
    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_PRIORITIZED,
            allowedCarriersPrioritized);
    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_NUM,
            (uint8_t)(crp->len_allowed_carriers));
    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS_NUM,
            (uint8_t)(crp->len_excluded_carriers));
    //allowed carriers
    if (crp->len_allowed_carriers > 0) {
        pAllowedCarriers
                = allocMipcCarriers(crp->allowed_carriers, crp->len_allowed_carriers, ret);
        if (ret != RIL_E_SUCCESS) {
            logE(mTag, "handleSetAllowedCarriers: allocate allowed carriers error!!!");
            goto error;
        }
        initMipc->addMipcTlv(
                MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS,
                sizeof(mipc_sim_carrier_struct_struct4) * crp->len_allowed_carriers,
                pAllowedCarriers);
    }
    //excluded carriers
    if (crp->len_excluded_carriers > 0) {
        pExcludedCarriers
                = allocMipcCarriers(crp->excluded_carriers, crp->len_excluded_carriers, ret);
        if (ret != RIL_E_SUCCESS) {
            logE(mTag, "handleSetAllowedCarriers: allocate excluded carriers error!!!");
            goto error;
        }
        initMipc->addMipcTlv(
                MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS,
                sizeof(mipc_sim_carrier_struct_struct4) * crp->len_excluded_carriers,
                pExcludedCarriers);
    }

    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    freeMipcCarriers(pAllowedCarriers);
    freeMipcCarriers(pExcludedCarriers);
    return;

error:
    freeMipcCarriers(pAllowedCarriers);
    freeMipcCarriers(pExcludedCarriers);
    int allowedCount = 0;
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&allowedCount, sizeof(int)), msg, false);

    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleSetAllowedCarriersResp(const sp < RfxMclMessage > & msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int allowedCount = 0;
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleSetAllowedCarriersResp: error=%d", mipcRet);
        ret = RIL_E_RADIO_NOT_AVAILABLE;
    }

    allowedCount = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_SET_ALLOWED_CARRIERS_CNF_T_ALLOWED_COUNT,
            0);

    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&allowedCount, sizeof(int)), msg, false);
    logE(mTag, "handleSetAllowedCarriersResp: ret=%d", ret);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleGetAllowedCarriers(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;

    logD(mTag, "handleGetAllowedCarriers...");

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_REQ,
            msg->getSlotId());
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;
}

/**
 * Allocate RIL_CArrier array and fill with data from pMipcCarriers.
 */
RIL_Carrier *RmmSmlRequestHandler::allocRilCarriers(
        const mipc_sim_carrier_struct_struct4 *pMipcCarriers,
        int count, RIL_Errno &error) {
    if (pMipcCarriers == NULL || count <= 0) {
        logE(mTag, "allocRilCarriers: invalid arguments.");
        error = RIL_E_MODEM_ERR;
        return NULL;
    }

    RIL_Carrier *pRilCarriers = (RIL_Carrier *)calloc(count, sizeof(RIL_Carrier));
    if (pRilCarriers == NULL) {
        logE(mTag, "allocRilCarriers: calloc error.");
        error = RIL_E_NO_MEMORY;
        return NULL;
    }
    for (int i=0; i<count; i++) {
        /*
         * mcc/mnc/match_data can be shared with pMipcCarriers for
         * pRilCarriers is only valid in handleGetAllowedCarrierResp scope.
         */
        pRilCarriers[i].match_type = (RIL_CarrierMatchType)pMipcCarriers[i].match_cat;
        pRilCarriers[i].mcc = (char*)(pMipcCarriers[i].mcc);
        pRilCarriers[i].mnc = (char*)(pMipcCarriers[i].mnc);
        pRilCarriers[i].match_data = (char*)(pMipcCarriers[i].match_data);
    }
    return pRilCarriers;
}

void RmmSmlRequestHandler::freeRilCarriers(RIL_Carrier *pRilCarriers) {
    if (pRilCarriers != NULL) {
        free(pRilCarriers);
    }
}

void RmmSmlRequestHandler::handleGetAllowedCarriersResp(const sp < RfxMclMessage > & msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    uint8_t state = 0;
    uint8_t status = 0;
    int multiSimPolicy = 0;
    int allowedCarriersPrioritized = 0;
    int allowedCount = 0;
    int excludedCount = 0;
    uint16_t dataLen = 0;
    mipc_sim_carrier_struct_struct4 *pMipcAllowedCarriers=NULL, *pMipcExcludedCarriers = NULL;
    RIL_CarrierRestrictionsWithPriority result;

    memset(&result, 0, sizeof(result));
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();

#ifndef SML_UT_TEST
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleGetAllowedCarriersResp: error=%d", mipcRet);
        goto error;
    }

    state = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_STATE,
            MIPC_SIM_CRRST_STATE_SIM_CRRST_STATE_NULL);
    multiSimPolicy = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_MULTI_SIM_POLICY,
            0);
    allowedCarriersPrioritized = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_PRIORITIZED,
            0);
    allowedCount = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_NUM,
            0);
    excludedCount = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS_NUM,
            0);
    status = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_CARRIER_RESTRICTION_STATUS,
            0);
    result.simLockMultiSimPolicy = multiSimPolicy;
    result.status = status;
    result.allowedCarriersPrioritized = allowedCarriersPrioritized;
    result.len_allowed_carriers = allowedCount;
    result.len_excluded_carriers = excludedCount;

#else //SML_UT_TEST
    logE(mTag, "SML_UT_TEST!!! handleGetAllowedCarriersResp: error=%d", mipcRet);
    result.simLockMultiSimPolicy = 1;
    result.status = 2;
    result.allowedCarriersPrioritized = 0;
    result.len_allowed_carriers = 0;
    result.len_excluded_carriers = 0;
#endif  //SML_UT_TEST

    if (allowedCount > 0) {
        pMipcAllowedCarriers = (mipc_sim_carrier_struct_struct4 *)mipcData->getMipcVal(
                MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS,
                &dataLen);
        if (pMipcAllowedCarriers == NULL
                || dataLen != allowedCount * sizeof(mipc_sim_carrier_struct_struct4)) {
            logE(mTag, "handleGetAllowedCarriersResp: allowedCount=%d but data error!",
                    allowedCount);
            goto error;
        }
        result.allowed_carriers = allocRilCarriers(pMipcAllowedCarriers, allowedCount, ret);
        if (ret != RIL_E_SUCCESS) {
            logE(mTag, "handleGetAllowedCarriersResp: allocated allowed carriers error!");
            goto error;
        }
    }
    if (excludedCount > 0) {
        pMipcExcludedCarriers = (mipc_sim_carrier_struct_struct4 *)mipcData->getMipcVal(
                    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS,
                    &dataLen);
        if (pMipcExcludedCarriers == NULL
                || dataLen != excludedCount * sizeof(mipc_sim_carrier_struct_struct4)) {
            logE(mTag, "handleGetAllowedCarriersResp: excludedCount=%d but data error!",
                    excludedCount);
            goto error;
        }

        result.excluded_carriers = allocRilCarriers(pMipcExcludedCarriers, excludedCount, ret);
        if (ret != RIL_E_SUCCESS) {
            logE(mTag, "handleGetAllowedCarriersResp: allocated excluded carriers error!");
            goto error;
        }
    }

    logD(mTag, "handleGetAllowedCarriersResp return");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxSimCarrRstData(
            (void*)(&result), sizeof(RIL_CarrierRestrictionsWithPriority)), msg, false);
    responseToTelCore(response);
    freeRilCarriers(result.allowed_carriers);
    freeRilCarriers(result.excluded_carriers);
    return;

error:
    freeRilCarriers(result.allowed_carriers);
    freeRilCarriers(result.excluded_carriers);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_RADIO_NOT_AVAILABLE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

/**
 * Message input format
 *  int array: [cat]
 */
void RmmSmlRequestHandler::handleQuerySimNetworkLock(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    int* cat = (int*)(msg->getData()->getData());
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_GET_LOCK_REQ,
            msg->getSlotId());

    if (cat == NULL || cat[0] < 0 || cat[0] >= MAX_SIM_ME_LOCK_CAT_NUM) {
        ret = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }
    logD(mTag, "handleQuerySimNetworkLock cat: %d", cat[0]);

    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_GET_LOCK_REQ_T_CATEGORY,
            (uint8_t)cat[0]);
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxSimMeLockCatData(NULL, 0), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleQuerySimNetworkLockResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    RIL_SimMeLockCatInfo lockstate;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleQuerySimNetworkLockResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    lockstate.catagory = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_CATEGORY,
            0);
    lockstate.state = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_STATE,
            0);
    lockstate.retry_cnt = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_RETRY_CNT,
            0);
    lockstate.autolock_cnt = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_AUTOLOCK_CNT,
            0);
    lockstate.num_set = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_NUM_SET,
            0);
    lockstate.total_set = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_TOTAL_SET,
            0);
    lockstate.key_state = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_KEY_STATE,
            0);
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxSimMeLockCatData((RIL_SimMeLockCatInfo*)&lockstate, sizeof(RIL_SimMeLockCatInfo)),
            msg,
            false);
    logD(mTag, "handleQuerySimNetworkLockResp: cat=%d, state=%d, retry=%d",
            lockstate.catagory, lockstate.state, lockstate.retry_cnt);
    responseToTelCore(response);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxSimMeLockCatData(NULL, 0), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleSetSimNetworkLock(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    char** strings = (char**)(msg->getData()->getData());
    int cat = 0, op = 0;
    char* key = NULL;
    char* imsi = NULL;
    char* gid1 = NULL;
    char* gid2 = NULL;
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_SET_LOCK_REQ,
            msg->getSlotId());

    if (strings == NULL || strings[0] == NULL ||
            strings[1] == NULL) {
         ret = RIL_E_INVALID_ARGUMENTS;
         logD(mTag, "handleSetSimNetworkLock invalid arguments.");
         goto error;
    }

    // strings[0]: cat
    // strings[1]: op
    cat = atoi(strings[0]);
    op = atoi(strings[1]);
    key = (strings[2] == NULL) ? (char*)"" : (char*)strings[2];
    imsi = (strings[3] == NULL) ? (char*)"" : (char*)strings[3];
    gid1 = (strings[4] == NULL) ? (char*)"" : (char*)strings[4];
    gid2 = (strings[5] == NULL) ? (char*)"" : (char*)strings[5];

    logD(mTag, "simNetworkLock strings %s,%s ...\n", strings[0], strings[1]);

    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_SET_LOCK_REQ_T_CATEGORY,
            cat);
    initMipc->addMipcTlvUint8(
            MIPC_SIM_SML_SET_LOCK_REQ_T_OP,
            op);
    initMipc->addMipcTlvStr(
            MIPC_SIM_SML_SET_LOCK_REQ_T_KEY,
            key);
    initMipc->addMipcTlvStr(
            MIPC_SIM_SML_SET_LOCK_REQ_T_DATA_IMSI,
            imsi);
    if (strlen(gid1) > 0) {
        initMipc->addMipcTlvStr(
                MIPC_SIM_SML_SET_LOCK_REQ_T_GID1,
                gid1);
    }
    if (strlen(gid2) > 0) {
        initMipc->addMipcTlvStr(
                MIPC_SIM_SML_SET_LOCK_REQ_T_GID2,
                gid2);
    }
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                        RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleSetSimNetworkLockResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleSetSimNetworkLockResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
    }
    logD(mTag, "handleSetSimNetworkLockResp: ret=%d", ret);
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                        RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleEnterDeviceNetworkDepersonalization(
        const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    char** strings = (char**)(msg->getData()->getData());
    int remainRetry = 0;
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ,
            msg->getSlotId());

    if (strings == NULL || strings[0] == NULL) {
         ret = RIL_E_INVALID_ARGUMENTS;
         logD(mTag, "handleEnterDeviceNetworkDepersonalization invalid arguments.");
         goto error;
    }
    logD(mTag, "handleEnterDeviceNetworkDepersonalization lockPin : *** \n");

    initMipc->addMipcTlvStr(
            MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ_T_KEY,
            strings[0]);
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&remainRetry, sizeof(remainRetry)), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::setSimSlotLockDeviceLockInfoProp(int remaining) {
    rfx_property_set(PROPERTY_SIM_SLOT_LOCK_DEVICE_LOCK_REMAIN_COUNT,
            String8::format("%d", remaining).string());
}

void RmmSmlRequestHandler::handleEnterDeviceNetworkDepersonalizationResp(
        const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    int remainRetry = 0;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleEnterDeviceNetworkDepersonalizationResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
    } else {
        remainRetry = mipcData->getMipcUint8Val(
                MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_CNF_T_REMAIN_COUNT,
                0);
    }

    //save to system property
    setSimSlotLockDeviceLockInfoProp(remainRetry);
    logD(mTag, "handleEnterDeviceNetworkDepersonalizationResp: ret=%d, remain=%d",
            ret, remainRetry);
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData((void*)&remainRetry, sizeof(remainRetry)), msg, false);
    responseToTelCore(response);

}

/********** RSU operation *********/
#ifndef MTOHLL
#define MTOHLL(x) (x)
#endif
#define mipc_msg_get_val_uint64(m, t, def_val) \
    ((mipc_msg_get_val_ptr(m, t, NULL)) ? \
    MTOHLL(*(uint64_t *)mipc_msg_get_val_ptr(m, t, NULL)) : def_val)

void RmmSmlRequestHandler::handleRsuRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_RSU_REQ,
            msg->getSlotId());

    RIL_RsuRequest* request = (RIL_RsuRequest*)(msg->getData()->getData());
    logD(mTag, "[RSU-SIMLOCK] handleRsuRequest opId = %d, requestId = %d",
            request->opId, request->requestId);

    int32_t tempOpId = request->opId;
    int32_t tempRequestId = request->requestId;

    if (request->opId == RSU_OP_COMMON) {
        switch (request->requestId) {
            case RSU_COMMON_REQUEST_GET_LOCK_STATUS:
                tempOpId = getOpIdForCommon();
                if (tempOpId < 0) {
                    logE(mTag, "[RSU-SIMLOCK] handleRsuRequest: invalid opId");
                    goto error;
                }
                break;
            case RSU_COMMON_REQUEST_VERIFY_PIN:
                if (request->reserveInt1 != RIL_RSU_LOCK_TYPE_TF_STATUS) {
                    logE(mTag, "[RSU-SIMLOCK] handleRsuRequest: COMMON VERIFY PIN: unknow lock type: %d",
                            request->reserveInt1);
                    goto error;
                }
                tempOpId = RSU_OP_TF_LEGACY;
                break;
            case RSU_COMMON_REQUEST_GET_LOCK_INFO:
                handleCommonRsuGetLockInfo(msg);
                return;
            case RSU_COMMON_REQUEST_GET_CAT_INFO:
                handleCommonRsuGetCatInfo(msg);
                return;
            default:
                logE(mTag, "[RSU-SIMLOCK] handleRsuRequest: no match common requestId!");
                goto error;
        }
    }

    tempRequestId = convertRequestIdToL5(request->opId, request->requestId);
    if (tempRequestId < 0) {
        logE(mTag, "[RSU-SIMLOCK] handleRsuRequest: invalid requestId");
        goto error;
    }

    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_OPERATOR_ID,
            tempOpId);
    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_REQUEST_ID,
            tempRequestId);

    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_RSV1,
            request->reserveInt1);
    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_RSV2,
            request->reserveInt2);
    initMipc->addMipcTlvStr(
            MIPC_SIM_SML_RSU_REQ_T_DATA,
            request->data);
    initMipc->addMipcTlvStr(
            MIPC_SIM_SML_RSU_REQ_T_RSV_STRING,
            request->reserveString1);

    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;

error:
    RIL_RsuResponse* rsuResponse = (RIL_RsuResponse*)calloc(1, sizeof(RIL_RsuResponse));

    RFX_ASSERT(rsuResponse != NULL);
    rsuResponse->opId = request->opId;
    rsuResponse->requestId = request->requestId;
    rsuResponse->requestType = request->requestType;

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxRsuResponseData(rsuResponse, sizeof(RIL_RsuResponse)), msg);
    free(rsuResponse);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleRsuRequestResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    RIL_RsuRequest* request = (RIL_RsuRequest*)(msg->getData()->getData());
    int convertRequestId;
    int msgId = msg->getId();
    int operatorId = -1;
    int requestId = -1;
    int requestType = 0;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    logI(mTag, "handleRsuRequestResp: mipcRet=%d", mipcRet);
    /**
     * RSU_COMMON_REQUEST_GET_LOCK_INFO and RSU_COMMON_REQUEST_GET_CAT_INFO
     * not support by MIPC_SIM_SML_RSU_REQ while they share the same response function of
     * RFX_MSG_REQUEST_SML_RSU_REQUEST
     */
    if (request->opId == RSU_OP_COMMON && (request->requestId == RSU_COMMON_REQUEST_GET_LOCK_INFO
            || request->requestId == RSU_COMMON_REQUEST_GET_CAT_INFO)) {
        switch (request->requestId) {
            case RSU_COMMON_REQUEST_GET_LOCK_INFO:
                handleCommonRsuGetLockInfoResp(msg);
                break;
            case RSU_COMMON_REQUEST_GET_CAT_INFO:
                handleCommonRsuGetCatInfoResp(msg);
                break;
            default:
                logE(mTag, "handleRsuRequestResp: no match RSU_OP_COMMON requestId: %d!",
                        request->requestId);
                break;
        }
        return;
    }

    RIL_RsuResponse* rsuResponse = (RIL_RsuResponse*)calloc(1, sizeof(RIL_RsuResponse));
    int opId = mipcData->getMipcUint32Val(
            MIPC_SIM_SML_RSU_CNF_T_OPERATOR_ID,
            -1);
    int reqId = mipcData->getMipcUint32Val(
            MIPC_SIM_SML_RSU_CNF_T_REQUEST_ID,
            -1);

    /*
     * For RFX_MSG_REQUEST_CLIENT_RSU, data in request msg is not RIL_RsuRequest.
     */
    if (msgId == RFX_MSG_REQUEST_CLIENT_RSU) {
        char * pdata = (char*)(msg->getData()->getData());
        char temp[3];
        const int minDataLength = 4;

        if (pdata != NULL && msg->getData()->getDataLength() >= minDataLength
                && strlen(pdata) >= minDataLength) {
            temp[0] = *pdata;
            temp[1] = *(pdata + 1);
            temp[2] = '\0';
            operatorId = htoi(temp);
            temp[0] = *(pdata + 2);
            temp[1] = *(pdata + 3);
            temp[2] = '\0';
            requestId = htoi(temp);
        } else {
            logE(mTag, "handleRsuRequestResp pdata is null!");
            goto error;
        }
    } else {
        operatorId = request->opId;
        requestId = request->requestId;
        requestType = request->requestType;
    }

    if (mipcRet != MIPC_RESULT_SUCCESS) {
        ret = RIL_E_GENERIC_FAILURE;
        if (mipcRet == MIPC_RESULT_SIM_EXT_REBOOT_REQUIRED) {
            if (operatorId == RSU_OP_ATT_LEGACY || operatorId == RSU_OP_TMO_LEGACY) {
                ret = RIL_E_REBOOT_REQUEST;
            } else if (operatorId == RSU_OP_KDDI) {
                ret = RIL_E_SUCCESS;
            } else if (operatorId == RSU_OP_JIO_LEGACY) {
                ret = RIL_E_SUCCESS;
                for (int i=0; i<RfxRilUtils::rfxGetSimCount(); i++) {
                    logD(mTag, "handleRsuRequestResp: sendEvent to %d", i);
                    sendEvent(RFX_MSG_EVENT_SIM_RESET, RfxVoidData(), RIL_CMD_PROXY_7, i);
                }
            }
        }
    }

    RFX_ASSERT(rsuResponse != NULL);
    rsuResponse->opId = operatorId;
    rsuResponse->requestId = requestId;
    rsuResponse->requestType = requestType;
    logD(mTag, "handleRsuRequestResp: opId=%d, requestId=%d",
            operatorId, requestId);

#ifdef SML_UT_TEST
{
    logE(mTag, "handleRsuRequestResp: ##############fake data for UT!!!!!");
    mipcRet = MIPC_RESULT_SUCCESS;
    ret = RIL_E_SUCCESS;
}
#else
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        goto error;
    }
#endif
    convertRequestId = convertRequestIdfromL5(operatorId, reqId);

    logD(mTag, "handleRsuRequestResp: opId=%d, reqId=%d, convertRequestId=%d",
                opId, reqId, convertRequestId);
    if((opId != operatorId || convertRequestId != requestId) && operatorId != RSU_OP_COMMON) {
        logE(mTag, "handleRsuRequestResp: Error message opId=%d, reqId=%d, convertRequestId=%d",
                opId, reqId, convertRequestId);
        ret = RIL_E_GENERIC_FAILURE;
    } else {
        rsuResponse->time = mipc_msg_get_val_uint64(mipcData->getData(),
                MIPC_SIM_SML_RSU_CNF_T_TIME,
                0);
        rsuResponse->version = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_VERSION,
                0);
        rsuResponse->status = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_STATUS,
                0);
        rsuResponse->reserveInt1 = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_RSV1,
                0);
        rsuResponse->reserveInt2 = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_RSV2,
                0);
        rsuResponse->errCode = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_ERROR_CODE,
                0);
        rsuResponse->data = mipcData->getMipcStr(MIPC_SIM_SML_RSU_CNF_T_DATA);
        rsuResponse->reserveString1 = mipcData->getMipcStr(MIPC_SIM_SML_RSU_CNF_T_RSV_STRING);
    }
#ifdef SML_UT_TEST
    rsuResponse->time = 0x50000000300ULL;
    rsuResponse->version = (0x5<<16) | 0x2; //major 5, minor 2
    rsuResponse->status = 1;
    rsuResponse->reserveInt1 = 100;
    rsuResponse->reserveInt2 = 200;
    rsuResponse->errCode = 1;
    logI(mTag, "fake data, request->data=%s", request->data);
    if (request->data != NULL || strlen(request->data) == 0) {
        rsuResponse->data = "0123456789ABCD";
    } else {
        rsuResponse->data = request->data;
    }
    rsuResponse->reserveString1 = "ABCDEF123456";
    logI(mTag, "handleRsuRequestResp: opId=%d, reqId=%d, reqType=%d, "
            "time=%lld, status=%d, version=%d, "
            "reserveInt1=%d, reserveInt2=%d, data=%s, reserveString1=%s",
            rsuResponse->opId,
            rsuResponse->requestId,
            rsuResponse->requestType,
            rsuResponse->time,
            rsuResponse->status,
            rsuResponse->version,
            rsuResponse->reserveInt1,
            rsuResponse->reserveInt2,
            rsuResponse->data,
            rsuResponse->reserveString1 );
#endif
    response = RfxMclMessage::obtainResponse(msg->getId(),
            ret, RfxRsuResponseData(rsuResponse, sizeof(RIL_RsuResponse)), msg);
    free(rsuResponse);
    responseToTelCore(response);
    return;

error:
    RFX_ASSERT(rsuResponse != NULL);
    rsuResponse->opId = operatorId;
    rsuResponse->requestId = requestId;
    rsuResponse->requestType = requestType;

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxRsuResponseData(rsuResponse, sizeof(RIL_RsuResponse)), msg);
    free(rsuResponse);
    responseToTelCore(response);
}

/**
 * Get Current Rsu Operator Id
 */
int32_t RmmSmlRequestHandler::getOpIdForCommon() {
    int policy = 0;
    int32_t opId = -1;

    policy = getNonSlotMclStatusManager()->getIntValue(RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY, 0);
    logD(mTag, "[RSU-SIMLOCK] getOpIdForCommon policy = %d", policy);
    if (policy == SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL_AND_RSU_VZW) {
        opId = RSU_OP_VZW;
    } else if (policy == SML_SLOT_LOCK_POLICY_LK_SLOTA_RSU_TMO) {
        opId = RSU_OP_TMO;
    } else {
        char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
        rfx_property_get("ro.vendor.sim_me_lock_mode", property_value, "");
        if ((strlen(property_value) > 0) && (strcmp(property_value, "1203") == 0)){
           opId = RSU_OP_VZW;
        } else if ((strlen(property_value) > 0) && (strcmp(property_value, "800") == 0)) {
           opId = RSU_OP_TMO;
        }
    }

    logD(mTag, "[RSU-SIMLOCK] getOpIdForCommon opId = %d", opId);
    return opId;
}

int32_t RmmSmlRequestHandler::getRequestIdForCommon(int32_t commRequestId) {
    int32_t requestId = -1;
    if (commRequestId == RSU_COMMON_REQUEST_VERIFY_PIN) {
        requestId = MIPC_SIM_SML_RSU_OPERATION_VERIFY_TF_STATUS;
    } else if (commRequestId == RSU_COMMON_REQUEST_GET_LOCK_STATUS) {
        requestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS;
    }
    logD(mTag, "[RSU-SIMLOCK] getRequestIdForCommon requestId = %d", requestId);
    return requestId;
}

int32_t RmmSmlRequestHandler::convertRequestIdToL5(int32_t opId, int32_t requestId) {
    int32_t ModemRequestId = -1;
    switch(opId) {
        case RSU_OP_VZW:
            if (requestId == RSU_REQUEST_INIT_REQUEST) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_INIT;
            } else if (requestId == RSU_REQUEST_GET_SHARED_KEY) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY;
            } else if (requestId == RSU_REQUEST_UPDATE_LOCK_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA;
            } else if (requestId == RSU_REQUEST_GET_LOCK_VERSION) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION;
            } else if (requestId == RSU_REQUEST_GET_LOCK_STATUS) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS;
            } else if (requestId == RSU_REQUEST_UNLOCK_TIMER) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_VZW: no match requestId!");
            }
            break;

        case RSU_OP_KDDI:
            if (requestId == RSU_REQUEST_SET_SIM_LOCK_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA;
            } else if (requestId == RSU_REQUEST_SET_GET_ALLOWED_LOCK_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_ALLOWED_LOCK_DATA;
            } else if (requestId == RSU_REQUEST_SET_GET_EXCLUDED_LOCK_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_EXCLUDED_LOCK_DATA;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_KDDI: no match requestId!");
            }
            break;

        case RSU_OP_TMO:
            if (requestId == RSU_REQUEST_GET_SHARED_KEY) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY;
            } else if (requestId == RSU_REQUEST_INIT_REQUEST) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_INIT;
            } else if (requestId == RSU_REQUEST_UPDATE_LOCK_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA;
            } else if (requestId == RSU_REQUEST_GET_LOCK_VERSION) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION;
            } else if (requestId == RSU_REQUEST_GET_LOCK_STATUS) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_TMO: no match requestId!");
            }
            break;

        case RSU_OP_TFN:
            if (requestId == TFN_RSU_REQUEST_GET_IMEI) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_IMEI;
            } else if (requestId == TFN_RSU_REQUEST_PROCESS_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA;
            } else if (requestId == TFN_RSU_REQUEST_GET_LOCK_STATUS) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS;
            } else if (requestId == TFN_RSU_REQUEST_GENERATE_REQUEST) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_SIGNED_TOKEN;
            } else if (requestId == TFN_RSU_REQUEST_RELOCK) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_RELOCK;
            } else if (requestId == TFN_RSU_REQUEST_GET_KEY) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_TFN: no match requestId!");
            }
            break;

        case RSU_OP_ATT_LEGACY:
            if (requestId == RSU_REQUEST_GET_SHARED_KEY) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY;
            } else if (requestId == RSU_REQUEST_UPDATE_LOCK_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA;
            } else if (requestId == RSU_REQUEST_RESET_LOCK_DATA) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_RESET_LOCK_DATA;
            } else if (requestId == RSU_REQUEST_GET_LOCK_STATUS) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS;
            } else if (requestId == RSU_REQUEST_GET_LOCK_VERSION) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION;
            } else if (requestId == RSU_REQUEST_GET_MODEM_CONFIG) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_MD_CONFIG;
            } else if (requestId == RSU_REQUEST_GET_API_VERSION) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_API_VERSION;
            } else if (requestId == RSU_REQUEST_GET_SIM_SLOTS) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_SIM_SLOTS;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_ATT_LEGACY: no match requestId!");
            }
            break;

        case RSU_OP_SPRINT_LEGACY:
            if (requestId == RSU_REQUEST_SPRINT_GET_NONCE) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY;
            } else if (requestId == RSU_REQUEST_SPRINT_UPDATE_LOCK) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA;
            } else if (requestId == RSU_REQUEST_SPRINT_GET_UNLOCK_CODE) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_SPRINT_LEGACY: no match requestId!");
            }
            break;

        case RSU_OP_COMMON:
            ModemRequestId = getRequestIdForCommon(requestId);
            break;

        case RSU_OP_JIO_LEGACY:
            if (requestId == SUBSIDY_UPDATE_SETTINGS) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA;
            } else if (requestId == SUBSIDY_GET_MODEM_STATUS) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS;
            } else if (requestId == SUBSIDY_GET_VERSION) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION;
            } else if (requestId == SUBSIDY_GET_UNLOCK_TIME) {
                ModemRequestId = MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_JIO_LEGACY: no match requestId!");
            }
            break;

        default:
            logD(mTag, "[RSU-SIMLOCK] convertRequestId: no match opId!");
            ModemRequestId = requestId;
            break;
    }

    logD(mTag, "[RSU-SIMLOCK] convertRequestId ModemRequestId = %d", ModemRequestId);
    return ModemRequestId;
}

int32_t RmmSmlRequestHandler::convertRequestIdfromL5(int32_t opId, int32_t requestId) {
    int32_t retRequestId = -1;
    switch(opId) {
        case RSU_OP_VZW:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_INIT) {
                retRequestId = RSU_REQUEST_INIT_REQUEST;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY) {
                retRequestId = RSU_REQUEST_GET_SHARED_KEY;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
                retRequestId = RSU_REQUEST_UPDATE_LOCK_DATA;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION) {
                retRequestId = RSU_REQUEST_GET_LOCK_VERSION;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
                retRequestId = RSU_REQUEST_GET_LOCK_STATUS;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER) {
                retRequestId = RSU_REQUEST_UNLOCK_TIMER;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_VZW: no match requestId!");
            }
            break;
        case RSU_OP_KDDI:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
                retRequestId = RSU_REQUEST_SET_SIM_LOCK_DATA;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_ALLOWED_LOCK_DATA) {
                retRequestId = RSU_REQUEST_SET_GET_ALLOWED_LOCK_DATA;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_EXCLUDED_LOCK_DATA) {
                retRequestId = RSU_REQUEST_SET_GET_EXCLUDED_LOCK_DATA;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_KDDI: no match requestId!");
            }
            break;
        case RSU_OP_TMO:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY) {
                retRequestId = RSU_REQUEST_GET_SHARED_KEY;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_INIT) {
                retRequestId = RSU_REQUEST_INIT_REQUEST;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
                retRequestId = RSU_REQUEST_UPDATE_LOCK_DATA;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION) {
                retRequestId = RSU_REQUEST_GET_LOCK_VERSION;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
                retRequestId = RSU_REQUEST_GET_LOCK_STATUS;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_TMO: no match requestId!");
            }
            break;
        case RSU_OP_TFN:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_IMEI) {
                retRequestId = TFN_RSU_REQUEST_GET_IMEI;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
                retRequestId = TFN_RSU_REQUEST_PROCESS_DATA;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
                retRequestId = TFN_RSU_REQUEST_GET_LOCK_STATUS;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_SIGNED_TOKEN) {
                retRequestId = TFN_RSU_REQUEST_GENERATE_REQUEST;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_RELOCK) {
                retRequestId = TFN_RSU_REQUEST_RELOCK;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY) {
                retRequestId = TFN_RSU_REQUEST_GET_KEY;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_TFN: no match requestId!");
            }
            break;
        case RSU_OP_ATT_LEGACY:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY) {
                retRequestId = RSU_REQUEST_GET_SHARED_KEY;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
                retRequestId = RSU_REQUEST_UPDATE_LOCK_DATA;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_RESET_LOCK_DATA) {
                retRequestId = RSU_REQUEST_RESET_LOCK_DATA;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
                retRequestId = RSU_REQUEST_GET_LOCK_STATUS;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION) {
                retRequestId = RSU_REQUEST_GET_LOCK_VERSION;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_MD_CONFIG) {
                retRequestId = RSU_REQUEST_GET_MODEM_CONFIG;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_API_VERSION) {
                retRequestId = RSU_REQUEST_GET_API_VERSION;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_SIM_SLOTS) {
                retRequestId = RSU_REQUEST_GET_SIM_SLOTS;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_ATT_LEGACY: no match requestId!");
            }
            break;
        case RSU_OP_SPRINT_LEGACY:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY) {
                retRequestId = RSU_REQUEST_SPRINT_GET_NONCE;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
                retRequestId = RSU_REQUEST_SPRINT_UPDATE_LOCK;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
                retRequestId = RSU_REQUEST_SPRINT_GET_UNLOCK_CODE;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_SPRINT_LEGACY: no match requestId!");
            }
            break;
        case RSU_OP_COMMON:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_VERIFY_TF_STATUS) {
                retRequestId = RSU_COMMON_REQUEST_VERIFY_PIN;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
                retRequestId = RSU_COMMON_REQUEST_GET_LOCK_STATUS;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_COMMON: no match requestId!");
            }
            break;
        case RSU_OP_JIO_LEGACY:
            if (requestId == MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
                retRequestId = SUBSIDY_UPDATE_SETTINGS;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
                retRequestId = SUBSIDY_GET_MODEM_STATUS;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION) {
                retRequestId = SUBSIDY_GET_VERSION;
            } else if (requestId == MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER) {
                retRequestId = SUBSIDY_GET_UNLOCK_TIME;
            } else {
                logD(mTag, "[RSU-SIMLOCK] RSU_OP_JIO_LEGACY: no match requestId!");
            }
            break;
        default:
            logD(mTag, "[RSU-SIMLOCK] convertRequestId: no match opId!");
            retRequestId = requestId;
    }

    logD(mTag, "[RSU-SIMLOCK] convertRequestId retRequestId = %d", retRequestId);
    return retRequestId;
}

void RmmSmlRequestHandler::handleCommonRsuGetLockInfo(const sp<RfxMclMessage>& msg) {
    logD(mTag, "[RSU-SIMLOCK] handleCommonRsuGetLockInfo...");
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_GET_NETWORK_LOCK_REQ,
            msg->getSlotId());

    callToMipcMsgAsync(msg, initMipc);
    return;
}

void RmmSmlRequestHandler::handleCommonRsuGetLockInfoResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    RIL_RsuResponse* rsuResponse = (RIL_RsuResponse*)calloc(1, sizeof(RIL_RsuResponse));
    RIL_RsuRequest* request = (RIL_RsuRequest*)(msg->getData()->getData());

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleCommonRsuGetLockInfoResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
    }

    RFX_ASSERT(rsuResponse != NULL);
    rsuResponse->opId = request->opId;
    rsuResponse->requestId = request->requestId;
    rsuResponse->requestType = request->requestType;
    logE(mTag, "handleCommonRsuGetLockInfoResp: request->opId=%d, request->requestId=%d",
            request->opId, request->requestId);

    uint8_t carrierId = mipcData->getMipcUint32Val(
            MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_CARRIER_ID,
            0xff);
    int isRsu = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_RSU_ENABLE,
            0);
    int lockStatus = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_LOCK_STATUS,
            0);
    int supportedCats = mipcData->getMipcUint32Val(
            MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_SUPPORTED_CATS,
            0);
    rsuResponse->version = mipcData->getMipcUint32Val(
            MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_VERSION,
            0x01);

    logD(mTag, "[RSU-SIMLOCK] handleCommonRsuGetLockInfoResp, carrierId=%d, isRsu=%d, lockStatus=%d, version=%d",
            carrierId, isRsu, lockStatus, rsuResponse->version);
    int rule = smlGetLockRule();
    int subrule = 0;
    int isLockFuse = smlIsEnabled();

    logD(mTag, "[RSU-SIMLOCK] handleCommonRsuGetLockInfoResp, isLockFuse=%d", isLockFuse);

    //compute the status value
    rsuResponse->status |= supportedCats << LOCK_INFO_SUPPORTED_CATS_OFFSET;
    rsuResponse->status |= (carrierId << LOCK_INFO_CARRIER_OFFSET)
            | ((lockStatus << LOCK_INFO_LOCK_STATUS_OFFSET) & LOCK_INFO_LOCK_STATUS_MASK)
            | (isLockFuse << LOCK_INFO_LOCK_FUSE_OFFSET)
            | (isRsu << LOCK_INFO_IS_RSU_OFFSET);
    rsuResponse->reserveInt1 = rule | (subrule << 8);

    logD(mTag, "[RSU-SIMLOCK] handleCommonRsuGetLockInfoResp, status=0X%X, rule=%d, version=%d",
            rsuResponse->status, rule, rsuResponse->version);
    response = RfxMclMessage::obtainResponse(msg->getId(),
            ret, RfxRsuResponseData(rsuResponse, sizeof(RIL_RsuResponse)), msg);
    free(rsuResponse);
    responseToTelCore(response);
}

//All SIM slots require to have a valid carrier SIM
#define SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS           0
//Each SIM slot is locked to a different carrier
#define SIMLOCK_SLOT_POLICY_UNIQUE_FOR_EACH_SLOT        1
//only primary slot requres a valid carrier SIM
#define SIMLOCK_SLOT_POLICY_LOCK_ONLY_SLOT_1            2
//if primary slot has a valid carrier SIM other slots work with any SIM
#define SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_SLOT_1   3
//if any slot has a valid carrier SIM other slots work with any SIM
#define SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_A_SLOT   4

int RmmSmlRequestHandler::smlGetLockRule() {
    int policy = SIMLOCK_SLOT_POLICY_SAME_ON_ALL_SLOTS;

    int mtkPolicy = getNonSlotMclStatusManager()->getIntValue(RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY, 0);
    switch (mtkPolicy) {
        case SML_SLOT_LOCK_POLICY_ONLY_SLOT1:
        case SML_SLOT_LOCK_POLICY_ONLY_SLOT2:
            policy = SIMLOCK_SLOT_POLICY_LOCK_ONLY_SLOT_1;
            break;

        case SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL:
            policy = SIMLOCK_SLOT_POLICY_UNIQUE_FOR_EACH_SLOT;
            break;

        case SML_SLOT_LOCK_POLICY_LK_SLOT1:
        case SML_SLOT_LOCK_POLICY_LK_SLOT2:
            policy = SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_SLOT_1;
            break;

        case SML_SLOT_LOCK_POLICY_LK_SLOTA:
        case SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_ECC_FOR_VALID_NO_SERVICE:
            policy = SIMLOCK_SLOT_POLICY_VALID_CARD_MUST_ON_A_SLOT;
            break;

        case SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_CS:
            logE(mTag, "Not support MTK_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_CS!");
            break;

        case SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_REVERSE:
            logE(mTag, "Not support MTK_LOCK_POLICY_LK_SLOTA_RESTRICT_REVERSE!");
            break;

        default:
            break;
    }

    return policy;
}

/*
 * If sim lock is enabled or configured in MD side
 *
 * return:
 *     1 -- configured
 *     0 -- not configured
 */
int RmmSmlRequestHandler::smlIsEnabled() {
    int state = 0;
    //state: 0 -- sim lock enabled, 1 -- not enabled
    state = getNonSlotMclStatusManager()->getIntValue(RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_LOCK_STATE,
            0);
    return !state;
}

void RmmSmlRequestHandler::handleCommonRsuGetCatInfo(const sp<RfxMclMessage>& msg) {
    logD(mTag, "[RSU-SIMLOCK] handleCommonRsuGetCatInfo...");
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_GET_LOCK_REQ,
            msg->getSlotId());

    RIL_RsuRequest* request = (RIL_RsuRequest*)(msg->getData()->getData());
    logD(mTag, "[RSU-SIMLOCK] handleCommonRsuGetCatInfo: cat=%d", request->reserveInt1);

    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_GET_LOCK_REQ_T_CATEGORY,
            request->reserveInt1);

    callToMipcMsgAsync(msg, initMipc);
    return;
}

void RmmSmlRequestHandler::handleCommonRsuGetCatInfoResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    RIL_RsuResponse* rsuResponse = (RIL_RsuResponse*)calloc(1, sizeof(RIL_RsuResponse));
    RIL_RsuRequest* request = (RIL_RsuRequest*)(msg->getData()->getData());

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleCommonRsuGetCatInfoResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
    }

    RFX_ASSERT(rsuResponse != NULL);
    rsuResponse->opId = request->opId;
    rsuResponse->requestId = request->requestId;
    rsuResponse->requestType = request->requestType;
    logE(mTag, "handleCommonRsuGetCatInfoResp: request->opId=%d, request->requestId=%d",
            request->opId, request->requestId);

    uint8_t maxRetry = 0;
    uint8_t remainRetry = 0;
    int isLocked = 0;
    int isRsu = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_RSU_ENABLE,
            0);
    int cat = mipcData->getMipcUint8Val(
            MIPC_SIM_SML_GET_LOCK_CNF_T_CATEGORY,
            0);

    logE(mTag, "handleCommonRsuGetCatInfoResp: isRsu=%d, cat=%d", isRsu, cat);

    if (isRsu == 0) {
        maxRetry = mipcData->getMipcUint8Val(
                MIPC_SIM_SML_GET_LOCK_CNF_T_MAX_CNT,
                0);
        remainRetry = mipcData->getMipcUint8Val(
                MIPC_SIM_SML_GET_LOCK_CNF_T_RETRY_CNT,
                0);
        isLocked = mipcData->getMipcUint8Val(
                MIPC_SIM_SML_GET_LOCK_CNF_T_STATE,
                0);
        logE(mTag, "handleCommonRsuGetCatInfoResp: maxRetry=%d, remainRetry=%d, isLocked=%d",
                maxRetry, remainRetry, isLocked);
    }

    //compute the status value
    rsuResponse->status |= cat << CAT_INFO_CAT_OFFSET;
    rsuResponse->status |= maxRetry << CAT_INFO_MAX_RETRY_OFFSET;
    rsuResponse->status |= (remainRetry << CAT_INFO_REMAIN_RETRY_OFFSET)
            | (isLocked << CAT_INFO_IS_LOCKED_OFFSET);
    logD(mTag, "[RSU-SIMLOCK] handleCommonRsuGetCatInfo, status=0X%X", rsuResponse->status);

    response = RfxMclMessage::obtainResponse(msg->getId(),
            ret, RfxRsuResponseData(rsuResponse, sizeof(RIL_RsuResponse)), msg);
    free(rsuResponse);
    responseToTelCore(response);
}

// Hexa To Int
unsigned int RmmSmlRequestHandler::htoi(char* szHex) {
    unsigned int hex = 0;
    int nibble;
    while (*szHex) {
        hex <<= 4;

        if (*szHex >= '0' && *szHex <= '9') {
            nibble = *szHex - '0';
        } else if (*szHex >= 'a' && *szHex <= 'f') {
            nibble = *szHex - 'a' + 10;
        } else if (*szHex >= 'A' && *szHex <= 'F') {
            nibble = *szHex - 'A' + 10;
        } else {
            nibble = 0;
        }

        hex |= nibble;

        szHex++;
    }

    return hex;
}

/**
 * Message data:
 *  pdata[0], pdata[1]: opId in hex format
 *  pdata[2], pdata[3]: reqeustId in hex format
 *  pdata[4]: data
 */
void RmmSmlRequestHandler::handleRsuClientRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_RSU_REQ,
            msg->getSlotId());

    char * pdata = (char*)(msg->getData()->getData());
    char temp[3];
    const int minDataLength = 4;

    if (pdata != NULL && msg->getData()->getDataLength() >= minDataLength
            && strlen(pdata) >= minDataLength) {
        temp[0] = *pdata;
        temp[1] = *(pdata + 1);
        temp[2] = '\0';
        unsigned int opId = htoi(temp);
        temp[0] = *(pdata + 2);
        temp[1] = *(pdata + 3);
        temp[2] = '\0';
        unsigned int reqId = htoi(temp);
        RIL_RsuRequest request;
        memset(&request, 0, sizeof(request));
        request.opId = opId;
        request.requestId = reqId;
        request.data = pdata + 4;
        int32_t tempRequestId = convertRequestIdToL5(request.opId, request.requestId);
        logD(mTag, "[RSU-SIMLOCK] handleRsuClientRequest opId=%d, requestId=%d, tempRequestId=%d",
                request.opId, request.requestId, tempRequestId);
        initMipc->addMipcTlvUint32(
                MIPC_SIM_SML_RSU_REQ_T_OPERATOR_ID,
                request.opId);
        initMipc->addMipcTlvUint32(
                MIPC_SIM_SML_RSU_REQ_T_REQUEST_ID,
                tempRequestId);
        initMipc->addMipcTlvUint32(
                MIPC_SIM_SML_RSU_REQ_T_RSV1,
                request.reserveInt1);
        initMipc->addMipcTlvUint32(
                MIPC_SIM_SML_RSU_REQ_T_RSV2,
                request.reserveInt2);
        initMipc->addMipcTlvStr(
                MIPC_SIM_SML_RSU_REQ_T_DATA,
                request.data);
        logMipcData(initMipc);
        callToMipcMsgAsync(msg, initMipc);
    } else {
        logE(mTag, "[RSU-SIMLOCK] handleRsuClientRequest pdata is null!");
        sp<RfxMclMessage> mclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg);
        responseToTelCore(mclResponse);
    }
    return;
}

void RmmSmlRequestHandler::handleRsuClientRequestResp(const sp<RfxMclMessage>& msg) {
    logD(mTag, "[RSU-SIMLOCK] handleRsuClientRequestResp...");
    handleRsuRequestResp(msg);
}

char* RmmSmlRequestHandler::getReducedLog(char* data) {
    static char dataToPrint[8];
    memset(dataToPrint, 0, sizeof(dataToPrint));
    int len = 0;
    if (data == NULL) {
        logD(mTag, "getReducedLog: dataToPrint is null");
    } else {
        len = strlen(data);
        if (len > 6) {
            strncpy(dataToPrint, data, 6);
        } else {
            strcpy(dataToPrint, data);
        }
    }
    return dataToPrint;
}

void RmmSmlRequestHandler::handleReqSubsidyLockUpdateSettings(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    int result[2] = {0};
    result[0] = SUBSIDY_UPDATE_SETTINGS;    // SUBSIDYLOCK COMMAND = 1
    char *current_data = (char*)(msg->getData()->getData());
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_RSU_REQ,
            msg->getSlotId());

    //Check data and print some log
    int data_size = 0;
    int blob_status = 0;
    int data_length = msg->getData()->getDataLength();
    logI(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings data_length=%d", data_length);
    if (data_length > 0 && current_data != NULL) {
        if (current_data[data_length - 1] != '\0') {
          logE(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings blob is not null-terminated\n");
          ret = RIL_E_GENERIC_FAILURE;
          goto done;
        }
    } else if (current_data != NULL) {
          logE(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings invalid data_length\n");
          ret = RIL_E_GENERIC_FAILURE;
          goto done;
    }
    data_size = (current_data != NULL ? strlen(current_data) : 0);
    if (data_size < 8) {
        logE(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings blob size < 8\n");
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }

    // parse blob status to get lock status bit. 0 : SUBSIDYLOCK, 1: PERMANENT_UNLOCK
    blob_status = current_data[7] - '0';
    logD(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings blob_status: %d, strings %s***, size: %d",
            blob_status, getReducedLog(current_data), data_size);

    if (blob_status == 0) {
        logD(mTag, "[SUBSIDY_LOCK]blob update request to SUBSIDYLOCK");
    } else {
        logD(mTag, "[SUBSIDY_LOCK]blob update request to PERMANENT_UNLOCK");
    }

    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_OPERATOR_ID,
            RSU_OP_JIO_LEGACY);
    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_REQUEST_ID,
            MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA);
    initMipc->addMipcTlvStr(
            MIPC_SIM_SML_RSU_REQ_T_DATA,
            current_data);
    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;
done:
    result[1] = (int) ret;
    logD(mTag, "reqType = %d , result[1] = %d, ril_error = %d", result[0], result[1], ret);
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                            RfxIntsData((void*)result, 2*sizeof(int)), msg, false);
    responseToTelCore(response);
}

void RmmSmlRequestHandler::handleReqSubsidyLockUpdateSettingsResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    int result[2] = {0};
    result[0] = SUBSIDY_UPDATE_SETTINGS;    // SUBSIDYLOCK COMMAND = 1

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleReqSubsidyLockUpdateSettingsResp: error=%d", mipcRet);
        if (mipcRet == MIPC_RESULT_SIM_EXT_REBOOT_REQUIRED) {
            for (int i=0; i<RfxRilUtils::rfxGetSimCount(); i++) {
                logD(mTag, "handleReqSubsidyLockUpdateSettings: sendEvent to %d", i);
                sendEvent(RFX_MSG_EVENT_SIM_RESET, RfxVoidData(), RIL_CMD_PROXY_7, i);
            }
        } else {
            ret = RIL_E_GENERIC_FAILURE;
        }
    }
    int opId = mipcData->getMipcUint32Val(
            MIPC_SIM_SML_RSU_CNF_T_OPERATOR_ID,
            -1);
    int reqId = mipcData->getMipcUint32Val(
            MIPC_SIM_SML_RSU_CNF_T_REQUEST_ID,
            -1);

    if(opId != RSU_OP_JIO_LEGACY || reqId != MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA) {
        logE(mTag, "handleReqSubsidyLockUpdateSettingsResp: Error opId=%d, reqId=%d",
                opId, reqId);
    }

    result[1] = (int) ret;
    logD(mTag, "handleReqSubsidyLockUpdateSettingsResp ret = %d",ret);
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                            RfxIntsData((void*)result, 2*sizeof(int)), msg, false);
    responseToTelCore(response);
}

/**
 * Common operation to get something for subsidy lock
 */
void RmmSmlRequestHandler::handleReqSubsidyLockGet(const sp<RfxMclMessage>& msg, uint32_t reqId)
 {
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_SML_RSU_REQ,
            msg->getSlotId());

    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_OPERATOR_ID,
            RSU_OP_JIO_LEGACY);
    initMipc->addMipcTlvUint32(
            MIPC_SIM_SML_RSU_REQ_T_REQUEST_ID,
            reqId);

    logMipcData(initMipc);
    callToMipcMsgAsync(msg, initMipc);
    return;
}

/**
 * AT+ESLBLOB=2
 *
 * +ESLBLOB:<status>
 *
 *  In response:
 *    result[0]: SUBSIDYLOCK COMMAND -- 2
 *    result[1]:  status:
 *      valid values: 0/1
 *           0 -- permanent unlocked
 *           1 -- subsidty locked
 *      others: error
 */
void RmmSmlRequestHandler::handleReqSubsidyLockGetModemStatus(const sp<RfxMclMessage>& msg) {
    logD(mTag, "[SUBSIDY_LOCK]handleReqSubsidyLockGetModemStatus\n");
    handleReqSubsidyLockGet(msg, MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS);
}

void RmmSmlRequestHandler::handleReqSubsidyLockGetModemStatusResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    int result[2] = {0};
    result[0] = SUBSIDY_GET_MODEM_STATUS;    // SUBSIDYLOCK COMMAND = 2
    result[1] = -1;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleReqSubsidyLockGetModemStatusResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
    } else {
        int opId = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_OPERATOR_ID,
                -1);
        int reqId = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_REQUEST_ID,
                -1);

        if(opId != RSU_OP_JIO_LEGACY || reqId != MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS) {
            logE(mTag, "handleReqSubsidyLockGetModemStatusResp: Error opId=%d, reqId=%d",
                    opId, reqId);
            goto done;
        }

        result[1] = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_STATUS,
                -1);
        logD(mTag, "handleReqSubsidyLockGetModemStatusResp: modem status status = %d", result[1]);
    }

done:
    logD(mTag, "[SUBSIDY_LOCK]done.\n");
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                            RfxIntsData((void*)result, sizeof(result)), msg, false);
    responseToTelCore(response);
}

/**
  * AT+ESLBLOB=6
  *
  * +ESLBLOB:<major_ver>,<minor_ver>
  *
  *  In response:
  *    result[0]: SUBSIDYLOCK COMMAND -- 3
  *    result[1]: return status: 0 -- success, 1 -- fail
  *    result[2]: version
  */
void RmmSmlRequestHandler::handleReqSubsidyLockGetVersion(const sp<RfxMclMessage>& msg) {
    logD(mTag, "[SUBSIDY_LOCK]handleReqSubsidyLockGetVersion\n");
    handleReqSubsidyLockGet(msg, MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION);
}

void RmmSmlRequestHandler::handleReqSubsidyLockGetVersionResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    int result[3] = {0};
    result[0] = SUBSIDY_GET_VERSION;    // SUBSIDYLOCK COMMAND = 3

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleReqSubsidyLockGetVersionResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
    } else {
        int opId = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_OPERATOR_ID,
                -1);
        int reqId = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_REQUEST_ID,
                -1);

        if(opId != RSU_OP_JIO_LEGACY || reqId != MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION) {
            logE(mTag, "handleReqSubsidyLockGetVersionResp: Error opId=%d, reqId=%d",
                    opId, reqId);
            ret = RIL_E_GENERIC_FAILURE;
            goto done;
        }

        result[2] = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_VERSION,
                0);
        logD(mTag, "handleReqSubsidyLockGetVersionResp: version = %d", result[2]);
    }

done:
    logD(mTag, "[SUBSIDY_LOCK]done.\n");
    if (ret == RIL_E_SUCCESS) {
        result[1] = 0;
    } else {
        result[1] = 1;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                            RfxIntsData((void*)result, sizeof(result)), msg, false);
    responseToTelCore(response);
}

/**
  * AT+ESLBLOB=7
  *
  * +ESLBLOB:<unlock_time_left>
  *
  *  In response:
  *    result[0]: SUBSIDYLOCK COMMAND -- 4
  *    result[1]: return status: 0 -- success, 1 -- fail
  *    result[2]: High word value
  *    result[3]: Low word value
  */
void RmmSmlRequestHandler::handleReqSubsidyLockGetUnlockTime(const sp<RfxMclMessage>& msg) {
    logD(mTag, "[SUBSIDY_LOCK]handleReqSubsidyLockGetUnlockTime\n");
    handleReqSubsidyLockGet(msg, MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER);
}

#define HIWORD(x) (uint32_t)(((x) & 0xFFFFFFFF00000000)>>32)
#define LOWORD(x) (uint32_t)((x) & 0xFFFFFFFF)
void RmmSmlRequestHandler::handleReqSubsidyLockGetUnlockTimeResp(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    int result[4] = {0};
    result[0] = SUBSIDY_GET_UNLOCK_TIME;    // SUBSIDYLOCK COMMAND = 4
    uint64_t unlockTime = 0;

    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    if (mipcRet != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleReqSubsidyLockGetUnlockTimeResp: error=%d", mipcRet);
        ret = RIL_E_GENERIC_FAILURE;
    } else {
        int opId = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_OPERATOR_ID,
                -1);
        int reqId = mipcData->getMipcUint32Val(
                MIPC_SIM_SML_RSU_CNF_T_REQUEST_ID,
                -1);

        if(opId != RSU_OP_JIO_LEGACY || reqId != MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER) {
            logE(mTag, "handleReqSubsidyLockGetUnlockTimeResp: Error opId=%d, reqId=%d",
                    opId, reqId);
            ret = RIL_E_GENERIC_FAILURE;
            goto done;
        }

        unlockTime = mipc_msg_get_val_uint64(mipcData->getData(),
                MIPC_SIM_SML_RSU_CNF_T_TIME,
                0);
        result[2] = HIWORD(unlockTime);
        result[3] = LOWORD(unlockTime);
        logD(mTag, "handleReqSubsidyLockGetUnlockTimeResp: HW = %d, LW=%d", result[2], result[3]);
    }

done:
    logD(mTag, "[SUBSIDY_LOCK]done.\n");
    if (ret == RIL_E_SUCCESS) {
        result[1] = 0;
    } else {
        result[1] = 1;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                            RfxIntsData((void*)result, sizeof(result)), msg, false);
    responseToTelCore(response);
}

/**
 * Reset sim by "AT+EBTSAP=0" / "AT+EBTSAP=1"
 */
void RmmSmlRequestHandler::handleEventResetSim(const sp < RfxMclMessage > & msg) {
    RFX_UNUSED(msg);
    sp<RfxMipcData> mipcConnect = RfxMipcData::obtainMipcData(
            MIPC_SIM_ACCESS_PROFILE_CONNECT_REQ,
            msg->getSlotId());
    callToMipcMsgAsync(msg, mipcConnect);

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(
            MIPC_SIM_ACCESS_PROFILE_DISCONNECT_REQ,
            msg->getSlotId());
    callToMipcMsgAsync(msg, mipcConnect);
}

#ifdef SML_DEBUG

typedef enum {
    TLV_TYPE_UINT8 = 0,
    TLV_TYPE_UINT16,
    TLV_TYPE_UINT32,
    TLV_TYPE_UINT64,
    TLV_TYPE_STRING,
    TLV_TYPE_CARRIERS_ARRAY,
}TlvType;

typedef struct {
    int tlvId;
    int tlvType;
    const char *name;
}TlvItem;

typedef struct {
    int mipcId;
    const char *name;
    TlvItem *tlvs;
}MipcTlvProto;

TlvItem simDepersoTlvs[] = {
    {MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_CATEGORY, TLV_TYPE_UINT8, "CATEGORY"},
    {MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_PIN_CODE, TLV_TYPE_STRING, "PIN_CODE"},
    {-1, -1, NULL}
};

TlvItem getAllowedCarriersTlvs[] = {
    {-1, -1, NULL}
};

TlvItem setAllowedCarriersTlvs[] = {
    { MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_MULTI_SIM_POLICY,
      TLV_TYPE_UINT8,
      "MULTI_SIM_POLICY"
    },
    { MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_PRIORITIZED,
      TLV_TYPE_UINT8,
      "ALLOWED_CARRIERS_PRIORITIZED"
    },
    { MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_NUM,
      TLV_TYPE_UINT8,
      "ALLOWED_CARRIERS_NUM"
    },
    { MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS_NUM,
      TLV_TYPE_UINT8,
      "EXCLUDED_CARRIERS_NUM"
    },
    { MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS,
      TLV_TYPE_CARRIERS_ARRAY,
      "ALLOWED_CARRIERS"
    },
    { MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS,
      TLV_TYPE_CARRIERS_ARRAY,
      "EXCLUDED_CARRIERS"
    },
    {-1, -1, NULL}
};

TlvItem enterDeviceDepersoTlvs[] = {
    { MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ_T_KEY,
      TLV_TYPE_STRING,
      "KEY"
    },
    {-1, -1, NULL}
};

TlvItem queryNetworkLockTlvs[] = {
    { MIPC_SIM_SML_GET_LOCK_REQ_T_CATEGORY,
      TLV_TYPE_UINT8,
      "CATEGORY"
    },
    {-1, -1, NULL}
};

TlvItem setNetworkLockTlvs[] = {
    { MIPC_SIM_SML_SET_LOCK_REQ_T_CATEGORY,
      TLV_TYPE_UINT8,
      "CATEGORY"
    },
    { MIPC_SIM_SML_SET_LOCK_REQ_T_OP,
      TLV_TYPE_UINT8,
      "OP"
    },
    { MIPC_SIM_SML_SET_LOCK_REQ_T_KEY,
      TLV_TYPE_STRING,
      "KEY"
    },
    { MIPC_SIM_SML_SET_LOCK_REQ_T_DATA_IMSI,
      TLV_TYPE_STRING,
      "IMSI"
    },
    { MIPC_SIM_SML_SET_LOCK_REQ_T_GID1,
      TLV_TYPE_STRING,
      "GID1"
    },
    { MIPC_SIM_SML_SET_LOCK_REQ_T_GID2,
      TLV_TYPE_STRING,
      "GID2"
    },
    {-1, -1, NULL}
};

TlvItem sendRsuTlvs[] = {
    { MIPC_SIM_SML_RSU_REQ_T_OPERATOR_ID,
      TLV_TYPE_UINT32,
      "opId"
    },
    { MIPC_SIM_SML_RSU_REQ_T_REQUEST_ID,
      TLV_TYPE_UINT32,
      "requestId"
    },
    { MIPC_SIM_SML_RSU_REQ_T_RSV1,
      TLV_TYPE_UINT32,
      "rsvInt1"
    },
    { MIPC_SIM_SML_RSU_REQ_T_RSV2,
      TLV_TYPE_UINT32,
      "rsvInt2"
    },
    { MIPC_SIM_SML_RSU_REQ_T_DATA,
      TLV_TYPE_STRING,
      "data"
    },
    { MIPC_SIM_SML_RSU_REQ_T_RSV_STRING,
      TLV_TYPE_STRING,
      "rsvString"
    },
    {-1, -1, NULL}
};

static const MipcTlvProto smlMipcProto[] = {
    {
        MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ,
        "MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ",
        simDepersoTlvs
    },
    {
        MIPC_SIM_SML_GET_ALLOWED_CARRIERS_REQ,
        "MIPC_SIM_SML_GET_ALLOWED_CARRIERS_REQ",
        getAllowedCarriersTlvs
    },
    {
        MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ,
        "MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ",
        setAllowedCarriersTlvs
    },
    {
        MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ,
        "MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ",
        enterDeviceDepersoTlvs
    },
    {
        MIPC_SIM_SML_GET_LOCK_REQ,
        "MIPC_SIM_SML_GET_LOCK_REQ",
        queryNetworkLockTlvs
    },
    {
        MIPC_SIM_SML_SET_LOCK_REQ,
        "MIPC_SIM_SML_SET_LOCK_REQ",
        setNetworkLockTlvs
    },
    {
        MIPC_SIM_SML_RSU_REQ,
        "MIPC_SIM_SML_RSU_REQ",
        sendRsuTlvs
    },
    { -1, NULL, NULL}
};

void RmmSmlRequestHandler::logMipcData(sp<RfxMipcData> mipcData) {
    int mipcId = mipcData->getMipcMsgId();
    for (unsigned int i=0; smlMipcProto[i].name != NULL; i++) {
        if (smlMipcProto[i].mipcId == mipcId) {
            logD(mTag, "mipc %s start =====", smlMipcProto[i].name);
            TlvItem *tlv = smlMipcProto[i].tlvs;
            if ( tlv != NULL) {
                while (tlv->name != NULL) {
                    if (tlv->tlvType == TLV_TYPE_STRING) {
                        logD(mTag, "\t\t%s = %s", tlv->name,
                                mipcData->getMipcStr(tlv->tlvId));
                    } else if (tlv->tlvType == TLV_TYPE_UINT8) {
                        logD(mTag, "\t\t%s = %d", tlv->name,
                                (uint32_t)mipcData->getMipcUint8Val(tlv->tlvId, -1));
                    } else if (tlv->tlvType == TLV_TYPE_UINT16) {
                        logD(mTag, "\t\t%s = %d", tlv->name,
                                (uint32_t)mipcData->getMipcUint16Val(tlv->tlvId, -1));
                    } else if (tlv->tlvType == TLV_TYPE_UINT32) {
                        logD(mTag, "\t\t%s = %d", tlv->name,
                                (uint32_t)mipcData->getMipcUint32Val(tlv->tlvId, -1));
                    } else if(tlv->tlvType == TLV_TYPE_CARRIERS_ARRAY) {
                        int length = 0;
                        int lengthId = 0;
                        mipc_sim_carrier_struct_struct4 *pCarriers = NULL;
                        uint16_t dataLen = 0;
                        if (tlv->tlvId
                                == MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS) {
                            lengthId = MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_NUM;
                        } else if (tlv->tlvId
                                == MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS) {
                            lengthId = MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS_NUM;
                        }
                        length = (uint32_t)mipcData->getMipcUint8Val(lengthId, 0);
                        pCarriers = (mipc_sim_carrier_struct_struct4 *)mipcData->getMipcVal(
                                            MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS,
                                            &dataLen);
                        logD(mTag, "pCarriers: %p, dataLen: %d:", pCarriers, dataLen);
                        if (pCarriers != NULL) {
                            for (int k=0; k<length; k++) {
                                logD(mTag, "\t\t%s[%d]:", tlv->name, k);
                                logD(mTag, "\t\tmatch_cat: %d:", pCarriers[k].match_cat);
                                logD(mTag, "\t\tmcc: %s:", pCarriers[k].mcc);
                                logD(mTag, "\t\tmnc: %s:", pCarriers[k].mnc);
                                logD(mTag, "\t\tmatch_data: %s:", pCarriers[k].match_data);
                            }
                        }
                    } else {
                        logD(mTag, "logMipcData: Unknown tlvType: ", tlv->tlvType);
                    }


                    tlv++;
                }
            }
            logD(mTag, "mipc %s end =======", smlMipcProto[i].name);
            return;
        }
    }
    logD(mTag, "logMipcData: Unknown mipc: ", mipcId);
}
#else //SML_DEBUG
void RmmSmlRequestHandler::logMipcData(sp<RfxMipcData>) {
}
#endif //SML_DEBUG
