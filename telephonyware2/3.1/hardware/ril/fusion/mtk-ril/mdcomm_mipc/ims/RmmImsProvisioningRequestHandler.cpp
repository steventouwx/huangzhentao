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

#include "RmmImsProvisioningRequestHandler.h"

static const int requests[] = {
    RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION, //AT+EPVSGET
    RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION, //AT+EPVSSET
};

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData, RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION);
//RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_GET_PROVISION_DONE);


// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmImsProvisioningRequestHandler, RIL_CMD_PROXY_1);

RmmImsProvisioningRequestHandler::RmmImsProvisioningRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmmImsProvisioningRequestHandler constructor");
    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
}

RmmImsProvisioningRequestHandler::~RmmImsProvisioningRequestHandler() {
}

void RmmImsProvisioningRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION:
            requestGetProvisionValue(msg);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION:
            requestSetProvisionValue(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmImsProvisioningRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "onHandleResponse: %d", msg->getId());
    int msgId = msg->getId();
    switch(msgId) {
        case RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION:
            handleGetProvisionValueResponse(msg);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION:
            handleSetProvisionValueResponse(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmImsProvisioningRequestHandler::requestGetProvisionValue(const sp<RfxMclMessage>& msg) {
    char *pvsString = (char *)msg->getData()->getData();
    logD(RFX_LOG_TAG, "requestGetProvisionValue, AT+EPVSGET=\"%s\"", pvsString);
    //atSendCommand(String8::format("AT+EPVSGET=\"%s\"", pvsString));

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_GET_CONFIG_REQ, m_slot_id);
    req_data->addMipcTlvUint32(MIPC_IMS_GET_CONFIG_REQ_T_CLASS, MIPC_IMS_CONFIG_CLASS_IMS_PROVISION);
    req_data->addMipcTlvStr(MIPC_IMS_GET_CONFIG_REQ_T_TYPE, pvsString);
    callToMipcMsgAsync(msg, req_data);
}

void RmmImsProvisioningRequestHandler::handleGetProvisionValueResponse(const sp<RfxMclMessage>& msg) {
    char *pvsString = NULL;
    uint16_t val_len = 0;
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    // send AT command
    RfxMipcData* rsp_data = msg->getMipcData();
    int  result = rsp_data->getResult();
    if (MIPC_RESULT_SUCCESS != result) {
        if (result == (MIPC_RESULT_IMS_EXT_BEGIN + 604)) {
            rilErrNo = RIL_E_OEM_ERROR_24;
            //+CME ERROR: 604 shall deal as RIL_E_OEM_ERROR_24
        } else {
            rilErrNo = RIL_E_GENERIC_FAILURE;
        }
    } else {
        char *temp = (char*) rsp_data->getMipcVal(MIPC_IMS_GET_CONFIG_CNF_T_DATA , &val_len);
        pvsString = (char*)calloc(1, sizeof(char)*(val_len + 1));
        RFX_ASSERT(pvsString != NULL);
        memcpy(pvsString, temp, val_len);
        pvsString[val_len] = '\0';
    }
    logD(RFX_LOG_TAG, "handleGetProvisionValueResponse, result:%x, %.3s, %d", result, pvsString, val_len);

    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxStringData(pvsString, val_len), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    if (pvsString != NULL) {
        free(pvsString);
    }
}

void RmmImsProvisioningRequestHandler::requestSetProvisionValue(const sp<RfxMclMessage>& msg) {
    char **pvsStrings = (char **)msg->getData()->getData();
    logD(RFX_LOG_TAG, "requestSetProvisionValue, AT+EPVSSET=\"%s\",\"%s\"",
            pvsStrings[0], pvsStrings[1]);

    //atSendCommand(String8::format("AT+EPVSSET=\"%s\",\"\"", pvsStrings[0]));

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_IMS_SET_CONFIG_REQ, m_slot_id);
    req_data->addMipcTlvUint32(MIPC_IMS_SET_CONFIG_REQ_T_CLASS, MIPC_IMS_CONFIG_CLASS_IMS_PROVISION);
    req_data->addMipcTlvStr(MIPC_IMS_SET_CONFIG_REQ_T_TYPE, pvsStrings[0]);
    if (pvsStrings[1] != NULL) {
        req_data->addMipcTlv(MIPC_IMS_SET_CONFIG_REQ_T_DATA, strlen(pvsStrings[1]), pvsStrings[1]);
    } else {
        logE(RFX_LOG_TAG, "requestSetProvisionValue, error, type:%s with null data",
            pvsStrings[0]);
    }

    callToMipcMsgAsync(msg, req_data);
}

void RmmImsProvisioningRequestHandler::handleSetProvisionValueResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxMipcData* rsp_data = msg->getMipcData();
    int  result = rsp_data->getResult();

    if (MIPC_RESULT_SUCCESS != result) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    logD(RFX_LOG_TAG, "handleSetProvisionValueResponse, result:%d", result);

    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}
