/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2017. All rights reserved.
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
#include "RmmCdmaSmsHandler.h"
#include "RfxCdmaSmsMsgData.h"
#include "RfxCdmaSmsAckData.h"
#include "RfxRuimSmsWriteData.h"
#include "RfxSmsSimMemStatusCnfData.h"
#include "RmmBcRangeParser.h"
#include "RfxCdmaCbSmsCfgData.h"
#include "RmmCdmaSmsConverter.h"
#include "RfxStringData.h"
#include "RfxIntsData.h"
#include "RfxSmsRspData.h"
#include "RfxVoidData.h"
#include "RfxLog.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxRuimSmsWriteData, RfxIntsData, RFX_MSG_REQUEST_CDMA_WRITE_SMS_TO_RUIM);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CDMA_DELETE_SMS_ON_RUIM);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxVoidData, RfxSmsSimMemStatusCnfData, RFX_MSG_REQUEST_GET_SMS_RUIM_MEM_STATUS);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaCbSmsCfgData, RmmVoidRsp, RFX_MSG_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxVoidData, RfxCdmaCbSmsCfgData, RFX_MSG_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG);

/*****************************************************************************
 * Class RmmCdmaSmsReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmCdmaSmsReqHandler, RIL_CMD_PROXY_1);

RmmCdmaSmsReqHandler::RmmCdmaSmsReqHandler(int slotId, int channelId) :
    RmmBaseHandler(slotId, channelId) {
    setTag(String8("RmmCdmaSmsReqHandler"));
    const int request[] = {
        RFX_MSG_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION,
        RFX_MSG_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG,
        RFX_MSG_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG,
        RFX_MSG_REQUEST_CDMA_DELETE_SMS_ON_RUIM,
        RFX_MSG_REQUEST_CDMA_WRITE_SMS_TO_RUIM,
        RFX_MSG_REQUEST_GET_SMS_RUIM_MEM_STATUS,
    };
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmmCdmaSmsReqHandler::~RmmCdmaSmsReqHandler() {
}

void RmmCdmaSmsReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_CDMA_WRITE_SMS_TO_RUIM:
            handleWriteSmsToRuim(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_DELETE_SMS_ON_RUIM:
            handleDeleteSmsOnRuim(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMS_RUIM_MEM_STATUS:
            handleGetSmsRuimMemStatus(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION:
            handleCdmaCbSmsActivation(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG:
            handleSetCdmaCbSmsCfg(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG:
            handleGetCdmaCbSmsCfg(msg);
            break;
        default:
            break;
    }
}

void RmmCdmaSmsReqHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    switch(msg->getId()) {
        case RFX_MSG_REQUEST_CDMA_WRITE_SMS_TO_RUIM:
            handleWriteSmsToRuimRsp(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_DELETE_SMS_ON_RUIM:
            handleDeleteSmsOnRuimRsp(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMS_RUIM_MEM_STATUS:
            handleGetSmsRuimMemStatusRsp(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION:
            handleCdmaCbSmsCfgRsp(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG:
            handleCdmaCbSmsCfgRsp(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG:
            handleGetCdmaCbSmsCfgRsp(msg);
            break;
        default:
            break;
    }
}

void RmmCdmaSmsReqHandler::handleWriteSmsToRuim(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> rsp;
    RIL_CDMA_SMS_WriteArgs* writeArgs = (RIL_CDMA_SMS_WriteArgs*)msg->getData()->getData();
    String8 hexPdu("");
    String8 number("");
    char *pduBytes = NULL;
    if (!RmmCdmaSmsConverter::toHexPdu(writeArgs->message, hexPdu, number)) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SYSTEM_ERR,
                RfxIntsData(), msg, false);
        responseToTelCore(rsp);
        return ;
    }

    int pduLen = strlen(hexPdu.string());
    pduBytes = (char *) malloc (sizeof(char) * pduLen / 2);
    RFX_ASSERT(pduBytes != NULL);
    RmmCdmaSmsConverter::hexStringToBytes(hexPdu.string(), pduLen, pduBytes, pduLen / 2);

    sp<RfxMipcData> writeData = RfxMipcData::obtainMipcData(MIPC_SMS_WRITE_REQ,
        msg->getSlotId());
    writeData->addMipcTlvUint8(MIPC_SMS_WRITE_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU_3GPP2);
    writeData->addMipcTlv(MIPC_SMS_WRITE_REQ_T_PDU, pduLen / 2, pduBytes);
    writeData->addMipcTlvUint8(MIPC_SMS_WRITE_REQ_T_STATUS, writeArgs->status);
    callToMipcMsgAsync(msg, writeData);

    if (pduBytes != NULL) {
        free(pduBytes);
    }
}

void RmmCdmaSmsReqHandler::handleWriteSmsToRuimRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_SYSTEM_ERR;
    int cause = mipcData->getResult();

    int response[1];

    if (cause == MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_SUCCESS;
        response[0] = mipcData->getMipcUint16Val(MIPC_SMS_WRITE_CNF_T_MESSAGE_INDEX, 0);
        rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                RfxIntsData((void*)response, sizeof(response)), msg, false);
        responseToTelCore(rsp);
        return;
    }

    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmCdmaSmsReqHandler::handleDeleteSmsOnRuim(const sp<RfxMclMessage>& msg) {
    int *pData = (int*)(msg->getData()->getData());
    sp<RfxMclMessage> rsp;
    int index;

    index = ((int *)pData)[0];

    sp<RfxMipcData> delData = RfxMipcData::obtainMipcData(MIPC_SMS_DELETE_REQ, msg->getSlotId());
    delData->addMipcTlvUint8(MIPC_SMS_DELETE_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU_3GPP2);

    if (-1 == index) {
        // delete all sms on RUIM
        delData->addMipcTlvUint8(MIPC_SMS_DELETE_REQ_T_FLAG, MIPC_SMS_FLAG_ALL);
    } else {
        delData->addMipcTlvUint8(MIPC_SMS_DELETE_REQ_T_FLAG, MIPC_SMS_FLAG_INDEX);
        delData->addMipcTlvUint16(MIPC_SMS_DELETE_REQ_T_MESSAGE_INDEX, index - 1);
    }

    callToMipcMsgAsync(msg, delData);
}

void RmmCdmaSmsReqHandler::handleDeleteSmsOnRuimRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int cause = mipcData->getResult();

    if (cause == MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_SUCCESS;
    }

    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}
void RmmCdmaSmsReqHandler::handleGetSmsRuimMemStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_GET_STORE_STATUS_REQ,
            msg->getSlotId());
    initData->addMipcTlvUint8(MIPC_SMS_GET_STORE_STATUS_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU_3GPP2);
    callToMipcMsgAsync(msg, initData);
}

void RmmCdmaSmsReqHandler::handleGetSmsRuimMemStatusRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> rsp;
    RIL_SMS_Memory_Status mem_status = {0, 0};
    int cause = mipcData->getResult();

    if (cause == MIPC_RESULT_SUCCESS) {
        mem_status.used = mipcData->getMipcUint16Val(
                MIPC_SMS_GET_STORE_STATUS_CNF_T_USED_MESSAGE, 0);
        mem_status.total = mipcData->getMipcUint16Val(
                MIPC_SMS_GET_STORE_STATUS_CNF_T_MAX_MESSAGE, 0);
        ril_errno = RIL_E_SUCCESS;
    }

    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxSmsSimMemStatusCnfData((void*)&mem_status, sizeof(mem_status)), msg, false);
    responseToTelCore(rsp);
}

void RmmCdmaSmsReqHandler::handleCdmaCbSmsActivation(const sp<RfxMclMessage>& msg) {
    int activation = ((int *)msg->getData()->getData())[0];
    //logD(mTag, "Turn Cdma CB %s", ((activation == 0)? "off" : "on"));

    sp<RfxMipcData> actData = RfxMipcData::obtainMipcData(MIPC_SMS_CBM_CFG_REQ, msg->getSlotId());

    if (activation == 0) {
        actData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_C2K_CBM_ENABLE, MIPC_BOOLEAN_FALSE);
    } else {
        actData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_C2K_CBM_ENABLE, MIPC_BOOLEAN_TRUE);
    }

    callToMipcMsgAsync(msg, actData);
}

void RmmCdmaSmsReqHandler::handleCdmaCbSmsCfgRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int cause = mipcData->getResult();

    if (cause == MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_SUCCESS;
    }
    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmCdmaSmsReqHandler::handleSetCdmaCbSmsCfg(const sp<RfxMclMessage>& msg) {
    int length = msg->getData()->getDataLength();
    int num = length/sizeof(RIL_CDMA_BroadcastSmsConfigInfo *);
    uint16_t chs[MAX_SINGLE_RANGE_NUM];
    uint8_t langs[MAX_SINGLE_RANGE_NUM];
    RIL_CDMA_BroadcastSmsConfigInfo **info =
            (RIL_CDMA_BroadcastSmsConfigInfo**)msg->getData()->getData();
    if (num <= 0) {
        RIL_Errno ril_errno = RIL_E_SUCCESS;
        logE(mTag, "handleSetCdmaCbSmsCfg num:%d", num);
        if (num < 0) {
            ril_errno = RIL_E_GENERIC_FAILURE;
        }
        sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                RfxVoidData(), msg, false);
        responseToTelCore(rsp);
        return;
    }
    int selected = info[0]->selected;

    //Convert RIL_CDMA_BroadcastSmsConfigInfo
    for (int i = 0; i < num && i < MAX_SINGLE_RANGE_NUM; i++) {
        chs[i] = (uint16_t)info[i]->service_category;
        langs[i] = (uint8_t)info[i]->language;
    }
    if (num > MAX_SINGLE_RANGE_NUM) {
        num = MAX_SINGLE_RANGE_NUM;
    }

    sp<RfxMipcData> cbData = RfxMipcData::obtainMipcData(MIPC_SMS_CBM_CFG_REQ, msg->getSlotId());

    if (selected == 1) {
        cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_CFG_TYPE,
                MIPC_SMS_CBM_CFG_TYPE_ALLOW);
        cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_DCS_CFG_TYPE,
                MIPC_SMS_CBM_CFG_TYPE_ALLOW);
    } else {
        cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_CFG_TYPE, MIPC_SMS_CBM_CFG_TYPE_BAN);
        cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_DCS_CFG_TYPE,
                MIPC_SMS_CBM_CFG_TYPE_BAN);
    }
    cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU_3GPP2);

    if (num > 0) {
        cbData->addMipcTlvArrayUint16(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_SINGLE, num, chs);
        cbData->addMipcTlvArrayUint8(MIPC_SMS_CBM_CFG_REQ_T_DCS_SINGLE, num, langs);
    }
    callToMipcMsgAsync(msg, cbData);
}

void RmmCdmaSmsReqHandler::handleGetCdmaCbSmsCfg(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> cbData = RfxMipcData::obtainMipcData(MIPC_SMS_CBM_CFG_REQ, msg->getSlotId());

    cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU_3GPP2);
    cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_CFG_TYPE,
            MIPC_SMS_CBM_CFG_TYPE_QUERY);
    cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_DCS_CFG_TYPE, MIPC_SMS_CBM_CFG_TYPE_QUERY);
    callToMipcMsgAsync(msg, cbData);
}

void RmmCdmaSmsReqHandler::handleGetCdmaCbSmsCfgRsp(const sp<RfxMclMessage>& msg) {
    uint16_t singleChNum = 0, singleLanNum = 0;
    uint16_t *singleCh = NULL;
    uint8_t *singleLan = NULL;
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> rsp;
    int cause = mipcData->getResult();

    if (cause != MIPC_RESULT_SUCCESS) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxCdmaCbSmsCfgData(NULL, 0), msg, false);
        responseToTelCore(rsp);
        return;
    }

    uint8_t activation = mipcData->getMipcUint8Val(MIPC_SMS_CBM_CFG_REQ_T_C2K_CBM_ENABLE,
            MIPC_BOOLEAN_FALSE);
    if (activation == MIPC_BOOLEAN_FALSE) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxCdmaCbSmsCfgData(NULL, 0), msg, false);
        responseToTelCore(rsp);
        return;
    }

    singleChNum = mipcData->getMipcArrayTlvNum(MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_SINGLE);
    singleCh = (uint16_t*)malloc(sizeof(uint16_t) * singleChNum);
    RFX_ASSERT(singleCh != NULL);
    for (int i = 0; i < singleChNum; i++) {
        singleCh[i] = mipcData->getMipcUint16IdxVal(
                MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_SINGLE, i, 0xFFFF);
    }
    logD(mTag, "get ch singlelen: %d", singleChNum);

    singleLanNum = mipcData->getMipcArrayTlvNum(MIPC_SMS_CBM_CFG_CNF_T_LANGUAGE_SINGLE);
    singleLan = (uint8_t*)malloc(sizeof(uint8_t) * singleLanNum);
    RFX_ASSERT(singleLan != NULL);
    for (int i = 0; i < singleLanNum; i++) {
        singleLan[i] = mipcData->getMipcUint8IdxVal(MIPC_SMS_CBM_CFG_CNF_T_LANGUAGE_SINGLE,
                i, 0xFF);
    }
    logD(mTag, "get language singlelen: %d", singleLanNum);

    if (singleChNum > 0 || singleLanNum > 0) {
        // Convert to RIL_CDMA_BroadcastSmsConfigInfo
        int num = singleChNum + singleLanNum;
        RIL_CDMA_BroadcastSmsConfigInfo **cdmaBciPtrs = (RIL_CDMA_BroadcastSmsConfigInfo**)calloc(
                1, sizeof(RIL_CDMA_BroadcastSmsConfigInfo*)*num);
        RFX_ASSERT(cdmaBciPtrs != NULL);

        for (int i = 0; i < singleChNum; i++) {
            cdmaBciPtrs[i] = (RIL_CDMA_BroadcastSmsConfigInfo*)calloc(
                    1, sizeof(RIL_CDMA_BroadcastSmsConfigInfo));
            RFX_ASSERT(cdmaBciPtrs[i] != NULL);
            cdmaBciPtrs[i]->service_category = singleCh[i];
            cdmaBciPtrs[i]->language = 0;
            cdmaBciPtrs[i]->selected = 1;
        }

        for (int i = 0; i < singleLanNum ; i++) {
            cdmaBciPtrs[singleChNum + i] = (RIL_CDMA_BroadcastSmsConfigInfo*)calloc(
                    1, sizeof(RIL_CDMA_BroadcastSmsConfigInfo));
            RFX_ASSERT(cdmaBciPtrs[singleChNum + i] != NULL);
            cdmaBciPtrs[singleChNum + i]->service_category = 0;
            cdmaBciPtrs[singleChNum + i]->language = singleLan[i];
            cdmaBciPtrs[singleChNum + i]->selected = 1;
        }

        // Send AP configuration
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxCdmaCbSmsCfgData((void*)cdmaBciPtrs,
                sizeof(RIL_CDMA_BroadcastSmsConfigInfo*)*num), msg, false);
        responseToTelCore(rsp);

        // free the output data
        if (cdmaBciPtrs != NULL) {
            for (int i = 0; i < num ; i++)
            {
                if (cdmaBciPtrs[i] != NULL) {
                    free(cdmaBciPtrs[i]);
                }
            }
            free(cdmaBciPtrs);
        }
    } else {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxCdmaCbSmsCfgData(NULL, 0), msg, false);
        responseToTelCore(rsp);
    }

    if (singleCh != NULL) {
        free(singleCh);
    }

    if (singleLan != NULL) {
        free(singleLan);
    }
}

/*****************************************************************************
 * Class RmmCdmaMoSmsReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmCdmaMoSmsReqHandler, RIL_CMD_PROXY_8);

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsMsgData, RfxSmsRspData, RFX_MSG_REQUEST_CDMA_SEND_SMS);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsMsgData, RfxSmsRspData, RFX_MSG_REQUEST_CDMA_SEND_SMS_EXPECT_MORE);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsMsgData, RfxSmsRspData, RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsMsgData, RfxSmsRspData, RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS_EX);

RmmCdmaMoSmsReqHandler::RmmCdmaMoSmsReqHandler(int slotId, int channelId) :
    RmmBaseHandler(slotId, channelId) {
    const int request[] = {
            RFX_MSG_REQUEST_CDMA_SEND_SMS,
            RFX_MSG_REQUEST_CDMA_SEND_SMS_EXPECT_MORE,
            RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS,
            RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS_EX
    };
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmmCdmaMoSmsReqHandler::~RmmCdmaMoSmsReqHandler() {
}
void RmmCdmaMoSmsReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_CDMA_SEND_SMS:
        case RFX_MSG_REQUEST_CDMA_SEND_SMS_EXPECT_MORE:
            handleSendCdmaSms(msg, true);
            break;
        case RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS:
        case RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS_EX:
            handleSendCdmaSms(msg, false);
            break;
        default:
            break;
    }
}

bool RmmCdmaMoSmsReqHandler::isCtSimCard() {
    bool ret = false;
    int type = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE);
    if (type == CT_4G_UICC_CARD ||
            type == CT_UIM_SIM_CARD ||
            type == CT_3G_UIM_CARD) {
        ret = true;
    }
    return ret;
}

void RmmCdmaMoSmsReqHandler::handleSendCdmaSms(const sp<RfxMclMessage>& msg,
        bool needBearerReplyOpt) {
    RIL_CDMA_SMS_Message* data = (RIL_CDMA_SMS_Message*)msg->getData()->getData();
    RIL_CDMA_SMS_Message  cdmaMsg = *data;

    String8 hexPdu("");
    String8 number("");
    char *pduBytes = NULL;
    sp<RfxMclMessage> rsp;

    if (isCtSimCard()) {
        /*
         * According to the spec of China Telecom, it needs the teleservice
         * id of long sms is TELESERVICE_WMT(0x1002). However, AOSP will use
         * TELESERVICE_WEMT(0x1005) as the teleservice id in SmsMessage.java.
         * In fact, most of China Telecom's network will work fine for 0x1005.
         * Only in Senzhen we will meet the problem that the message center cannot
         * handle the teleservice id 0x1005, so we need to convert the teleservice
         * id from 0x1005 to 0x1002 to make sure the long sms can work in all China
         * Telecom's network.
         */
        if (cdmaMsg.uTeleserviceID == TELESERVICE_WEMT) {
            cdmaMsg.uTeleserviceID = TELESERVICE_WMT;
        }
    }

    bool ret = RmmCdmaSmsConverter::toHexPdu(cdmaMsg, hexPdu, number, needBearerReplyOpt);
    if(!ret) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxSmsRspData(NULL, 0), msg, false);
        responseToTelCore(rsp);
        return;
    }

    int pduLen = strlen(hexPdu.string());
    pduBytes = (char *) malloc (sizeof(char) * pduLen / 2);
    RFX_ASSERT(pduBytes != NULL);
    RmmCdmaSmsConverter::hexStringToBytes(hexPdu.string(), pduLen, pduBytes, pduLen / 2);

    sp<RfxMipcData> moData = RfxMipcData::obtainMipcData(MIPC_SMS_SEND_REQ, msg->getSlotId());
    moData->addMipcTlvUint8(MIPC_SMS_SEND_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU_3GPP2);
    moData->addMipcTlv(MIPC_SMS_SEND_REQ_T_PDU_C2K, pduLen / 2, pduBytes);
    moData->addMipcTlvStr(MIPC_SMS_SEND_REQ_T_NUM_C2K, number);

    uint32_t timeout = 10 * 60 * 1000;
    sp<RfxMipcData> result = callToMipcMsgSync(moData, timeout);
    handleSendCdmaSmsRsp(msg, result);
    if (pduBytes != NULL) {
        free(pduBytes);
    }
}

void RmmCdmaMoSmsReqHandler::handleSendCdmaSmsRsp(const sp<RfxMclMessage>& msg,
        const sp<RfxMipcData> mipcData) {
    RIL_SMS_Response response;
    int cause;
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
     /* fill error code first */
    response.errorCode = ril_errno;

    cause = mipcData->getResult();

    if (cause != MIPC_RESULT_SUCCESS) {
        int errorClass = mipcData->getMipcUint8Val(MIPC_SMS_SEND_CNF_T_ERR_CLASS_C2K, 0);
        int errorCode = mipcData->getMipcUint16Val(MIPC_SMS_SEND_CNF_T_ERR_CODE_C2K, 0);
        if (errorClass == MIPC_SMS_C2K_ERR_CLASS_TEMP) {
            if (MIPC_SMS_C2K_ERR_CODE_RUIM_ABSENT== errorCode) {
                ril_errno = RIL_E_SIM_ABSENT;
            }
        } else if ((errorClass == MIPC_SMS_C2K_ERR_CLASS_PERMANENT) &&
                (errorCode == MIPC_SMS_C2K_ERR_CODE_FDN_CHECK)) {
            ril_errno = RIL_E_FDN_CHECK_FAILURE;
        }
        response.errorCode = ril_errno;
    } else {
        // Get message reference
        response.messageRef = mipcData->getMipcUint16Val(MIPC_SMS_SEND_CNF_T_MSGID_C2K, 0);

        response.ackPDU = NULL;

        /* fill success code */
        response.errorCode = RIL_E_SUCCESS;
        ril_errno = RIL_E_SUCCESS;
    }
    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxSmsRspData((void*)&response, sizeof(response)), msg, false);
    responseToTelCore(rsp);
}


/*****************************************************************************
 * Class RmmCdmaMtSmsAckReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmCdmaMtSmsAckReqHandler, RIL_CMD_PROXY_7);
/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsAckData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsAckData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsAckData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_INTERNAL);

RFX_REGISTER_DATA_TO_REQUEST_ID(
        RfxCdmaSmsAckData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_TIMEOUT);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxCdmaSmsAckData, RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK);

RmmCdmaMtSmsAckReqHandler::RmmCdmaMtSmsAckReqHandler(int slotId, int channelId) :
    RmmBaseHandler(slotId, channelId) {
    setTag(String8("RmmCdmaMtSmsAckReqHandler"));
    const int request[] = {
        RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE,
        RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX,
        RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_INTERNAL,
        RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_TIMEOUT
    };
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
    const int event[] = {
       RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK
    };
    registerToHandleEvent(event, sizeof(event) / sizeof(int));
}

RmmCdmaMtSmsAckReqHandler::~RmmCdmaMtSmsAckReqHandler() {
}

void RmmCdmaMtSmsAckReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE:
        case RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX:
        case RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_INTERNAL:
        case RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_TIMEOUT:
            handleCdmaSmsAcknowledge(msg);
            break;
        default:
            break;
    }
}
void RmmCdmaMtSmsAckReqHandler::handleCdmaSmsAcknowledge(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> rsp;
    String8 hexPdu;

    //for VMI and some c2k sms, no need to ack
    int replySeqNo = getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CDMA_SMS_REPLY_SEQ_NO,
            -1);
    int mtType = getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            CDMA_SMS_INBOUND_NONE);
    if (replySeqNo == -1 || mtType == CDMA_SMS_INBOUND_VMI) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_NO_SMS_TO_ACK,
                RfxVoidData(), msg, false);
        responseToTelCore(rsp);
        getMclStatusManager()->setIntValue(
                RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
                CDMA_SMS_INBOUND_NONE);
        int vmi = getMclStatusManager()->getIntValue(
                RFX_STATUS_KEY_CDMA_PENDING_VMI, -1);
        if (vmi != -1){
            int slotId = getMclStatusManager()->getSlotId();
            sendEvent(RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI,
                    RfxIntsData(&vmi, 1), RIL_CMD_PROXY_URC, slotId, -1, -1, 100);
        }
        return;
    }

    if (mtType != CDMA_SMS_INBOUND_NONE) {
        // Ack to md
        int txId = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_INBOUND_SMS_TXID, 0);
        sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_SMS_RSP,
                msg->getSlotId(), txId);
        ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, MIPC_NEW_SMS_ACK_RP_ACK);
        ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_FORMAT, MIPC_SMS_FORMAT_PDU_3GPP2);
        callToMipcMsgCmdResponse(ackData);
    } else {
        logE(mTag, "SMS type is SMS_NONE");
    }

    getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            CDMA_SMS_INBOUND_NONE);
    rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmCdmaMtSmsAckReqHandler::onHandleEvent(const sp<RfxMclMessage> & msg) {
    if (msg->getId() == RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK) {
        sendCdmaSmsErrorAck(msg);
    }
}
void RmmCdmaMtSmsAckReqHandler::sendCdmaSmsErrorAck(const sp<RfxMclMessage>& msg) {
    RIL_CDMA_SMS_Ack* ack = (RIL_CDMA_SMS_Ack*)msg->getData()->getData();
    int txId = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_INBOUND_SMS_PRE_TXID, 0);

    sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_SMS_RSP,
            msg->getSlotId(), txId);
    ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, MIPC_NEW_SMS_ACK_RP_ERROR);
    ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_ERR_CLASS_C2K, ack->uErrorClass);
    ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_ERR_CODE_C2K, ack->uSMSCauseCode);
    callToMipcMsgCmdResponse(ackData);
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE, CDMA_SMS_INBOUND_NONE);
}



/*****************************************************************************
 * Register Data Class
 *****************************************************************************/

RFX_REGISTER_DATA_TO_URC_ID(
        RfxCdmaSmsMsgData, RFX_MSG_URC_CDMA_NEW_SMS);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_CDMA_NEW_SMS_CMD);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI);

/*****************************************************************************
 * Class RmmCdmaSmsUrcHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmCdmaSmsUrcHandler, RIL_CMD_PROXY_URC);

static const int urc_list[] = {
    MIPC_SMS_NEW_SMS_IND,
};

RmmCdmaSmsUrcHandler::RmmCdmaSmsUrcHandler(int slotId, int channelId) :
    RmmBaseHandler(slotId, channelId) {
    setTag(String8("RmmCdmaSmsUrcHandler"));

    registerToHandleMipcURC(urc_list, sizeof(urc_list)/sizeof(int));

    const int event[] = {
       RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI,
       RFX_MSG_EVENT_CDMA_NEW_SMS_CMD
    };
    registerToHandleEvent(event, sizeof(event) / sizeof(int));
}

RmmCdmaSmsUrcHandler::~RmmCdmaSmsUrcHandler() {
}

void RmmCdmaSmsUrcHandler::handleNewCdmaSmsCmd(const sp<RfxMclMessage>& msg) {
    // last mt sms does not complete yet, return
    int mtType = getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            CDMA_SMS_INBOUND_NONE);
    if (mtType != CDMA_SMS_INBOUND_NONE) {
        RIL_CDMA_SMS_Ack ack;
        ack.uErrorClass = (RIL_CDMA_SMS_ErrorClass)2;
        ack.uSMSCauseCode = 33;
        sendEvent(RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK,
                RfxCdmaSmsAckData(&ack, sizeof(RIL_CDMA_SMS_Ack)),
                RIL_CMD_PROXY_7, m_slot_id);
        return;
    }

    getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            CDMA_SMS_INBOUND_COMM);

    int txId = getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_PRE_TXID, 0);
    getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TXID,
            txId);

    // get the pdu
    char *pdu = (char *)msg->getData()->getData();
    String8 pduHex(pdu);

    // convert hex string to RIL_CDMA_SMS_Message
    int replySeqNo = -1; // fot cmd mt, need ack, it should has value
    RIL_CDMA_SMS_Message cdmaMsg = {};
    RmmCdmaSmsConverter::toMessage(&cdmaMsg, pduHex, &replySeqNo);

    getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_CDMA_SMS_REPLY_SEQ_NO,
            replySeqNo);

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_CDMA_NEW_SMS,
            m_slot_id, RfxCdmaSmsMsgData(&cdmaMsg, sizeof(RIL_CDMA_SMS_Message)));
    responseToTelCore(unsol);
}

void RmmCdmaSmsUrcHandler::handleNewCdmaSmsUrc(const sp<RfxMclMessage>& msg) {
    uint16_t length = 0;
    uint8_t *pdu = (uint8_t *)msg->getMipcData()->getMipcVal(MIPC_SMS_NEW_SMS_IND_T_PDU_C2K,
            &length);

    if (pdu == NULL) {
        return;
    }

    String8 pduHex("");
    //convert bytes to hex string
    for (int i = 0; i < length; i++) {
        pduHex.appendFormat("%02x", pdu[i]);
    }

    // convert hex string to RIL_CDMA_SMS_Message
    int replySeqNo;//it should be -1
    RIL_CDMA_SMS_Message cdmaMsg;
    RmmCdmaSmsConverter::toMessage(&cdmaMsg, pduHex, &replySeqNo);

    int type = CDMA_SMS_INBOUND_COMM;
    if (cdmaMsg.uTeleserviceID == TELESERVICE_VMI) {
        type = CDMA_SMS_INBOUND_VMI;
    }

    // last mt sms does not complete yet, return
    int lastType = getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            CDMA_SMS_INBOUND_NONE);
    if (lastType != CDMA_SMS_INBOUND_NONE) {
        if (type == CDMA_SMS_INBOUND_VMI) {
            //save the vmi number
            getMclStatusManager()->setIntValue(
                    RFX_STATUS_KEY_CDMA_PENDING_VMI, pdu[length - 1]);
        }
        return;
    }

    getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            type);
    if (type == CDMA_SMS_INBOUND_VMI) {
        getMclStatusManager()->setIntValue(
                RFX_STATUS_KEY_CDMA_PENDING_VMI, -1);
    } else {
        getMclStatusManager()->setIntValue(
                RFX_STATUS_KEY_CDMA_SMS_REPLY_SEQ_NO,
                -1); //no need ack
    }

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_CDMA_NEW_SMS,
            m_slot_id, RfxCdmaSmsMsgData(&cdmaMsg, sizeof(RIL_CDMA_SMS_Message)));
    responseToTelCore(unsol);
}

void RmmCdmaSmsUrcHandler::handleCdmaPendingVMI(const sp<RfxMclMessage>& msg) {
    // last mt sms does not complete yet, return
    int lastType = getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            CDMA_SMS_INBOUND_NONE);
    if (lastType != CDMA_SMS_INBOUND_NONE) {
        return;
    }

    getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
            CDMA_SMS_INBOUND_VMI);
    getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_CDMA_PENDING_VMI, -1);
    int vmi = *((int*)msg->getData()->getData());
    if (vmi > 99) {
        vmi = 99;
    }
    vmi = ((vmi / 10) << 4) | (vmi % 10);
    String8 hexPdu("");
    hexPdu.appendFormat("000002100302020040080c0003100000010210000B01%02x", vmi);
    // convert hex string to RIL_CDMA_SMS_Message
    int replySeqNo;//it should be -1
    RIL_CDMA_SMS_Message cdmaMsg;
    RmmCdmaSmsConverter::toMessage(&cdmaMsg, hexPdu, &replySeqNo);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_CDMA_NEW_SMS,
            m_slot_id, RfxCdmaSmsMsgData(&cdmaMsg, sizeof(RIL_CDMA_SMS_Message)));
    responseToTelCore(unsol);
}

void RmmCdmaSmsUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        logD(mTag, "onHandleUrc msgId:%d MipcMsgId:%d, MipcSlotId:%d",
            msg->getId(), msg->getMipcData()->getMipcMsgId(), msg->getMipcData()->getMipcSlotId());
        switch (msg->getMipcData()->getMipcMsgId()) {
            // no need to ack (for verizon)
            case MIPC_SMS_NEW_SMS_IND:
                uint8_t format;
                format = msg->getMipcData()->getMipcUint8Val(MIPC_SMS_NEW_SMS_IND_T_FORMAT,
                    MIPC_SMS_FORMAT_PDU_3GPP);
                if (format == MIPC_SMS_FORMAT_PDU_3GPP2) {
                    handleNewCdmaSmsUrc(msg);
                } else if (format == MIPC_SMS_FORMAT_PDU_3GPP) {
                    mipc_sms_pdu_struct4 * smsPdulist = (mipc_sms_pdu_struct4 *)msg->getMipcData()
                            ->getMipcVal(MIPC_SMS_NEW_SMS_IND_T_PDU_LIST, NULL);
                    if (smsPdulist != NULL) {
                        // get <tpdu length> without sca
                        int tpduLen = smsPdulist->pdu_len;
                        // pdu length with sca
                        int pduLen = smsPdulist->pdu[0] + tpduLen + 1;

                        uint8_t *smsPdu = smsPdulist->pdu;

                        //convert bytes to hex string
                        String8 pduHex("");
                        for (int i = 0; i < pduLen; i++) {
                             pduHex.appendFormat("%02x", smsPdu[i]);
                        }

                        sendEvent(RFX_MSG_EVENT_GSM_NEW_SMS_IND,
                                RfxStringData((char*)pduHex.string(),
                                2 * pduLen), RIL_CMD_PROXY_URC, m_slot_id);
                    }
                }
                break;
            default:
                break;
        }
    }
}

void RmmCdmaSmsUrcHandler::onHandleEvent(const sp<RfxMclMessage> & msg) {
    switch (msg->getId()) {
        case RFX_MSG_EVENT_CDMA_NEW_SMS_CMD:
            handleNewCdmaSmsCmd(msg);
            break;
        case RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI:
            handleCdmaPendingVMI(msg);
            break;
        default:
            break;
    }
}

