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
#include "RmmGsmSmsRequestHandler.h"
#include <telephony/mtk_ril.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include "RfxMessageId.h"
#include "RmmGsmSmsDefs.h"
#include "RfxLog.h"
#include "RfxStatusDefs.h"
#include "RfxSmsRspData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxSmsSimMemStatusCnfData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxSmsWriteData.h"
#include "RfxGsmCbSmsCfgData.h"
#include "RfxSmsParamsData.h"
#include "RfxRilUtils.h"
#include "RmmBcRangeParser.h"

using ::android::String8;

static const int non_realtime_req_list[] = {
        RFX_MSG_REQUEST_SEND_SMS,
        RFX_MSG_REQUEST_SEND_SMS_EXPECT_MORE,
        RFX_MSG_REQUEST_IMS_SEND_GSM_SMS,
        RFX_MSG_REQUEST_IMS_SEND_GSM_SMS_EX,
};

static const int channel1_req_list[] = {
        RFX_MSG_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU,
        RFX_MSG_REQUEST_WRITE_SMS_TO_SIM,
        RFX_MSG_REQUEST_DELETE_SMS_ON_SIM,
        RFX_MSG_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG,
        RFX_MSG_REQUEST_REPORT_SMS_MEMORY_STATUS,
        RFX_MSG_REQUEST_GET_SMS_SIM_MEM_STATUS,
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE,
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_INTERNAL,
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_EX,
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_TIMEOUT,
        RFX_MSG_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG,
        RFX_MSG_REQUEST_GET_SMSC_ADDRESS,
        RFX_MSG_REQUEST_SET_SMSC_ADDRESS,
        RFX_MSG_REQUEST_GSM_GET_BROADCAST_LANGUAGE,
        RFX_MSG_REQUEST_GSM_SET_BROADCAST_LANGUAGE,
        RFX_MSG_REQUEST_GSM_SMS_BROADCAST_ACTIVATION,
        RFX_MSG_REQUEST_GET_SMS_PARAMS,
        RFX_MSG_REQUEST_SET_SMS_PARAMS,
        RFX_MSG_REQUEST_GET_GSM_SMS_BROADCAST_ACTIVATION
};

static const int event_list[] = {
    RFX_MSG_EVENT_SMS_NEW_SMS_ERR_ACK,
};

// Register handler
RFX_IMPLEMENT_HANDLER_CLASS(RmmGsmSmsRequestHandler, RIL_CMD_PROXY_8);
RFX_REGISTER_HANDLER_CLASS(RmmGsmSmsRequestHandler, RIL_CMD_PROXY_1);

// Register dispatch and response class
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxSmsRspData, \
        RFX_MSG_REQUEST_SEND_SMS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxSmsRspData, \
        RFX_MSG_REQUEST_SEND_SMS_EXPECT_MORE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_EX);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_INTERNAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
        RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_TIMEOUT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSmsWriteData, RfxIntsData, \
        RFX_MSG_REQUEST_WRITE_SMS_TO_SIM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
        RFX_MSG_REQUEST_DELETE_SMS_ON_SIM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
        RFX_MSG_REQUEST_REPORT_SMS_MEMORY_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxSmsSimMemStatusCnfData, \
        RFX_MSG_REQUEST_GET_SMS_SIM_MEM_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, \
        RFX_MSG_REQUEST_GET_SMSC_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, \
        RFX_MSG_REQUEST_SET_SMSC_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, \
        RFX_MSG_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxGsmCbSmsCfgData, \
        RFX_MSG_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxGsmCbSmsCfgData, RfxVoidData, \
        RFX_MSG_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, \
        RFX_MSG_REQUEST_GSM_GET_BROADCAST_LANGUAGE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, \
        RFX_MSG_REQUEST_GSM_SET_BROADCAST_LANGUAGE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
        RFX_MSG_REQUEST_GSM_SMS_BROADCAST_ACTIVATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, \
        RFX_MSG_REQUEST_GET_GSM_SMS_BROADCAST_ACTIVATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxSmsParamsData, \
        RFX_MSG_REQUEST_GET_SMS_PARAMS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSmsParamsData, RfxVoidData, \
        RFX_MSG_REQUEST_SET_SMS_PARAMS);


// Register event
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_SMS_NEW_SMS_ERR_ACK);



/*****************************************************************************
 * Class RmmGsmSmsRequestHandler
 *****************************************************************************/


RmmGsmSmsRequestHandler::RmmGsmSmsRequestHandler(int slot_id, int channel_id) :
        RmmGsmSmsBaseHandler(slot_id, channel_id) {
    setTag(String8("RmmGsmSmsRequest"));
    if (channel_id == RIL_CMD_PROXY_8 && RfxRilUtils::isSmsSupport()) {
        registerToHandleRequest(non_realtime_req_list, sizeof(non_realtime_req_list)/sizeof(int));
    } else if (channel_id == RIL_CMD_PROXY_1 && RfxRilUtils::isSmsSupport()) {
        registerToHandleRequest(channel1_req_list, sizeof(channel1_req_list)/sizeof(int));
        registerToHandleEvent(event_list, sizeof(event_list)/sizeof(int));
    }
}

void RmmGsmSmsRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_SEND_SMS:
        case RFX_MSG_REQUEST_SEND_SMS_EXPECT_MORE:
        case RFX_MSG_REQUEST_IMS_SEND_GSM_SMS:
        case RFX_MSG_REQUEST_IMS_SEND_GSM_SMS_EX:
            handleSendSms(msg);
            break;
        case RFX_MSG_REQUEST_SMS_ACKNOWLEDGE:
        case RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_EX:
        case RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_INTERNAL:
        case RFX_MSG_REQUEST_SMS_ACKNOWLEDGE_TIMEOUT:
            handleSmsAcknowledge(msg);
            break;
        case RFX_MSG_REQUEST_WRITE_SMS_TO_SIM:
            handleWriteSmsToSim(msg);
            break;
        case RFX_MSG_REQUEST_DELETE_SMS_ON_SIM:
            handleDeleteSmsOnSim(msg);
            break;
        case RFX_MSG_REQUEST_REPORT_SMS_MEMORY_STATUS:
            handleReportSmsMemStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMS_SIM_MEM_STATUS:
            handleGetSmsSimMemStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMSC_ADDRESS:
            handleGetSmscAddress(msg);
            break;
        case RFX_MSG_REQUEST_SET_SMSC_ADDRESS:
            handleSetSmscAddress(msg);
            break;
        case RFX_MSG_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU:
            handleSmsAcknowledgeWithPdu(msg);
            break;
        case RFX_MSG_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG:
            handleGetGsmCbSmsCfg(msg, GSM_CB_QUERY_CHANNEL);
            break;
        case RFX_MSG_REQUEST_GSM_GET_BROADCAST_LANGUAGE:
            handleGetGsmCbSmsCfg(msg, GSM_CB_QUERY_LANGUAGE);
            break;
        case RFX_MSG_REQUEST_GET_GSM_SMS_BROADCAST_ACTIVATION:
            handleGetGsmCbSmsCfg(msg, GSM_CB_QUERY_CHANNEL);
            break;
        case RFX_MSG_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG:
            handleSetGsmCbSmsCfg(msg);
            break;
        case RFX_MSG_REQUEST_GSM_SET_BROADCAST_LANGUAGE:
            handleSetGsmCbSmsLanguage(msg);
            break;
        case RFX_MSG_REQUEST_GSM_SMS_BROADCAST_ACTIVATION:
            handleGsmCbSmsActivation(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMS_PARAMS:
            handleGetSmsParams(msg);
            break;
        case RFX_MSG_REQUEST_SET_SMS_PARAMS:
            handleSetSmsParams(msg);
            break;
        default:
            logE(mTag, "Not support the request!");
            break;
    }
}

void RmmGsmSmsRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    switch(msg->getId()) {
        case RFX_MSG_REQUEST_WRITE_SMS_TO_SIM:
            handleWriteSmsToSimRsp(msg);
            break;
        case RFX_MSG_REQUEST_DELETE_SMS_ON_SIM:
            handleDeleteSmsOnSimRsp(msg);
            break;
        case RFX_MSG_REQUEST_REPORT_SMS_MEMORY_STATUS:
            handleReportSmsMemStatusRsp(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMS_SIM_MEM_STATUS:
            handleGetSmsSimMemStatusRsp(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMSC_ADDRESS:
            handleGetSmscAddressRsp(msg);
            break;
        case RFX_MSG_REQUEST_SET_SMSC_ADDRESS:
            handleSetSmscAddressRsp(msg);
            break;
        case RFX_MSG_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG:
            handleGetGsmCbSmsCfgRsp(msg, GSM_CB_QUERY_CHANNEL);
            break;
        case RFX_MSG_REQUEST_GSM_GET_BROADCAST_LANGUAGE:
            handleGetGsmCbSmsCfgRsp(msg, GSM_CB_QUERY_LANGUAGE);
            break;
        case RFX_MSG_REQUEST_GET_GSM_SMS_BROADCAST_ACTIVATION:
            handleGsmGetCbSmsActivationRsp(msg);
            break;
        case RFX_MSG_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG:
        case RFX_MSG_REQUEST_GSM_SET_BROADCAST_LANGUAGE:
        case RFX_MSG_REQUEST_GSM_SMS_BROADCAST_ACTIVATION:
            handleGsmCbCfgRsp(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMS_PARAMS:
            handleGetSmsParamsRsp(msg);
            break;
        case RFX_MSG_REQUEST_SET_SMS_PARAMS:
            handleSetSmsParamsRsp(msg);
            break;
        default:
            logE(mTag, "should not be here");
            break;
    }
}

void RmmGsmSmsRequestHandler::onHandleEvent(const sp < RfxMclMessage > & msg) {
    int event = msg->getId();
    switch(event) {
        case RFX_MSG_EVENT_SMS_NEW_SMS_ERR_ACK:
            sendNewSmsErrorAck(msg);
            break;
        default:
            logE(mTag, "Not support the event!");
            break;
    }
}

void RmmGsmSmsRequestHandler::handleSendSms(const sp<RfxMclMessage>& msg) {
    char** data = (char**)msg->getData()->getData();
    int tpduLen = 0; //tpdu length, not include sca
    int pduLen = 0; //pdu length, include sca
    String8 smsc("");
    String8 tpdu("");
    // +2: one for <length of SMSC>, one for \0
    char pdu[(MAX_SMSC_LENGTH+MAX_TPDU_LENGTH+1)*2+1] = {0};
    char pduBytes[MAX_SMSC_LENGTH+MAX_TPDU_LENGTH+1] = {0};
    sp<RfxMclMessage> rsp;

    // check card status
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        logE(mTag, "SIM card was removed!");
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ABSENT,
                RfxVoidData(), msg, false);
        responseToTelCore(rsp);
        return;
    }

    if (data[0] != NULL) {
        smsc.append(String8::format("%s", data[0]));
    }
    if (data[1] != NULL) {
        tpdu.append(String8::format("%s", data[1]));
    }

    tpduLen = smsPackPdu(((!smsc.isEmpty())? smsc.string() : NULL),
            ((!tpdu.isEmpty())? tpdu.string() : NULL), pdu, true);
    if(tpduLen < 0) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxSmsRspData(NULL, 0), msg, false);
        responseToTelCore(rsp);
        return;
    }
    pduLen = strlen(pdu);
    hexStringToBytes(pdu, pduLen, pduBytes, pduLen / 2);

    sp<RfxMipcData> moData = RfxMipcData::obtainMipcData(MIPC_SMS_SEND_REQ, msg->getSlotId());
    moData->addMipcTlvUint8(MIPC_SMS_SEND_REQ_T_FORMAT, MIPC_SMS_FORMAT_PDU);
    moData->addMipcTlv(MIPC_SMS_SEND_REQ_T_PDU, pduLen / 2, pduBytes);
    moData->addMipcTlvUint8(MIPC_SMS_SEND_REQ_T_PDU_LEN, tpduLen);
    if (msg->getId() == RFX_MSG_REQUEST_SEND_SMS_EXPECT_MORE) {
        moData->addMipcTlvUint8(MIPC_SMS_SEND_REQ_T_MORE_MSG_TO_SEND,
                MIPC_SMS_MORE_MSG_TO_SEND_ENABLE_ONCE);
    }

    uint32_t timeout = 10 * 60 * 1000;
    sp<RfxMipcData> result = callToMipcMsgSync(moData, timeout);
    handleSendSmsRsp(msg, result);
}

void RmmGsmSmsRequestHandler::handleSendSmsRsp(const sp<RfxMclMessage>& msg,
        const sp<RfxMipcData> mipcData) {
    RIL_SMS_Response response;
    memset(&response, 0, sizeof(response));
    int cause;
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
     /* fill error code first */
    response.errorCode = ril_errno;
    char *pduHex;
    uint16_t length = 0;
    uint8_t *pdu;
    int i;

    cause = mipcData->getResult();

    if (cause != MIPC_RESULT_SUCCESS) {
        /****************************************
         * FIXME
         * Need to check what cause should be retry latter.
         ****************************************/
        char value[RFX_PROPERTY_VALUE_MAX] = {0};
        int smsRetry = 0;
        rfx_property_get("ro.vendor.enable.sms.fail.retry", value, "0");
        smsRetry = atoi(value);

        if (cause == MIPC_RESULT_SMS_EXT_MTK_FDN_CHECK_FAILURE) {
            ril_errno = RIL_E_FDN_CHECK_FAILURE;
        } else if (cause == MIPC_RESULT_SMS_EXT_MTK_REQ_RETRY ||
                (cause == MIPC_RESULT_SMS_EXT_CM_SMS_CONNECTION_BROKEN && smsRetry == 1)) {
            ril_errno = RIL_E_SMS_SEND_FAIL_RETRY;
        } else {
            ril_errno = RIL_E_MODEM_ERR;
        }
        response.errorCode = cause & 0xFFFF;
        goto error;
    }
    // Get message reference and ackPDU
    response.messageRef = mipcData->getMipcUint8Val(MIPC_SMS_SEND_CNF_T_MR, 0);
    logD(mTag, "message reference number: %d ", response.messageRef);

    pdu = (uint8_t *)mipcData->getMipcVal(MIPC_SMS_SEND_CNF_T_ACK_PDU, &length);
    pduHex = (char *)malloc(sizeof(char) * (length * 2 + 1));
    RFX_ASSERT(pduHex != NULL);
    //convert bytes to hex string
    for (i = 0; i < length; i++) {
        int ret = sprintf(pduHex + i * 2, "%02x", pdu[i]);
        if (ret < 0) goto error;
    }
    pduHex[i*2] = '\0';
    response.ackPDU = pduHex;

    /* fill success code */
    response.errorCode = RIL_E_SUCCESS;
    rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxSmsRspData((void*)&response, sizeof(response)), msg, false);
    responseToTelCore(rsp);

    if (pduHex != NULL) {
        free(pduHex);
    }
    return;

error:
    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxSmsRspData((void*)&response, sizeof(response)), msg, false);
    responseToTelCore(rsp);
}


void RmmGsmSmsRequestHandler::handleSmsAcknowledge(const sp<RfxMclMessage>& msg) {
    void *pData = msg->getData()->getData();
    int ackSuccess, cause;
    int smsType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
            SMS_INBOUND_NONE);
    int txId = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TXID, 0);
    sp<RfxMclMessage> rsp;

    if (txId != -1) {
        ackSuccess = ((int *)pData)[0];
        cause = ((int *)pData)[1] & 0xFF;

        showCurrIncomingSmsType();
        if (ackSuccess == 1) {
            if (smsType == SMS_INBOUND_3GPP_CMT || smsType == SMS_INBOUND_3GPP_CMTI) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_SMS_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, MIPC_NEW_SMS_ACK_RP_ACK);
                callToMipcMsgCmdResponse(ackData);
            } else if (smsType == SMS_INBOUND_3GPP_CDS) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_STATUS_REPORT_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_TYPE,
                        MIPC_NEW_SMS_ACK_RP_ACK);
                callToMipcMsgCmdResponse(ackData);
            } else {
                logE(mTag, "SMS type is SMS_NONE");
            }
        } else if (ackSuccess == 0) {
            if (smsType == SMS_INBOUND_3GPP_CMT || smsType == SMS_INBOUND_3GPP_CMTI) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_SMS_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, MIPC_NEW_SMS_ACK_RP_ERROR);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_CAUSE, cause);
                callToMipcMsgCmdResponse(ackData);
            } else if (smsType == SMS_INBOUND_3GPP_CDS) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_STATUS_REPORT_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_TYPE,
                        MIPC_NEW_SMS_ACK_RP_ERROR);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_STATUS_REPORT_RSP_T_CAUSE, cause);
                callToMipcMsgCmdResponse(ackData);
            } else {
                logE(mTag, "SMS type is SMS_NONE");
            }
        } else {
            logE(mTag, "unsupported arg to RFX_MSG_REQUEST_SMS_ACKNOWLEDGE");
        }
    } else {
        logE(mTag, "no need to ack");
    }

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE, SMS_INBOUND_NONE);
    rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleWriteSmsToSim(const sp<RfxMclMessage>& msg) {
    RIL_SMS_WriteArgs *p_args = (RIL_SMS_WriteArgs*)(msg->getData()->getData());
    int tpduLen = 0; //tpdu length, not include sca
    int pduLen = 0; //pdu length, include sca
    char pdu[(MAX_SMSC_LENGTH+MAX_TPDU_LENGTH+1)*2+1];   // +2: one for <length of SMSC>, one for \0
    char pduBytes[MAX_SMSC_LENGTH+MAX_TPDU_LENGTH+1];

    sp<RfxMclMessage> rsp;

    // check card status
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        logE(mTag, "SIM card was removed!");
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ABSENT,
                RfxIntsData(), msg, false);
        responseToTelCore(rsp);
        return;
    }
    //check stat
    if (p_args->status < RIL_SMS_REC_UNREAD || p_args->status >= RIL_SMS_MESSAGE_MAX) {
        logE(mTag, "The status is invalid: %d", p_args->status);
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxIntsData(), msg, false);
        responseToTelCore(rsp);
        return;
    }

    // pack PDU with SMSC
    tpduLen = smsPackPdu(p_args->smsc, p_args->pdu, pdu, false);
    if (tpduLen < 0) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxIntsData(), msg, false);
        responseToTelCore(rsp);
        return;
    }

    pduLen = strlen(pdu);
    hexStringToBytes(pdu, pduLen, pduBytes, pduLen / 2);

    sp<RfxMipcData> writeData = RfxMipcData::obtainMipcData(MIPC_SMS_WRITE_REQ, msg->getSlotId());
    writeData->addMipcTlv(MIPC_SMS_WRITE_REQ_T_PDU, pduLen / 2, pduBytes);
    writeData->addMipcTlvUint8(MIPC_SMS_WRITE_REQ_T_PDU_LEN, tpduLen);
    writeData->addMipcTlvUint8(MIPC_SMS_WRITE_REQ_T_STATUS, p_args->status);
    callToMipcMsgAsync(msg, writeData);
}

void RmmGsmSmsRequestHandler::handleWriteSmsToSimRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int cause = mipcData->getResult();

    int response[1];

    if (cause == MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_SUCCESS;
        response[0] = mipcData->getMipcUint16Val(MIPC_SMS_WRITE_CNF_T_MESSAGE_INDEX, 0);
        rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                RfxIntsData((void*)response, sizeof(response)), msg, false);
        responseToTelCore(rsp);
        return;
    }/* else if (cause = MIPC_RESULT_SIM_IS_FULL) {
        ril_errno = RIL_E_SIM_FULL;
    }*/

    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleDeleteSmsOnSim(const sp<RfxMclMessage>& msg) {
    int *pData = (int*)(msg->getData()->getData());
    sp<RfxMclMessage> rsp;
    int index;

    // check card status
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        logE(mTag, "SIM card was removed!");
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ABSENT,
                RfxVoidData(), msg, false);
        responseToTelCore(rsp);
        return;
    }
    index = ((int *)pData)[0];

    sp<RfxMipcData> delData = RfxMipcData::obtainMipcData(MIPC_SMS_DELETE_REQ, msg->getSlotId());

    if (index > 0) {
        delData->addMipcTlvUint8(MIPC_SMS_DELETE_REQ_T_FLAG, MIPC_SMS_FLAG_INDEX);
        delData->addMipcTlvUint16(MIPC_SMS_DELETE_REQ_T_MESSAGE_INDEX, index);
    } else if (-1 == index) {
        // delete all sms on SIM
        delData->addMipcTlvUint8(MIPC_SMS_DELETE_REQ_T_FLAG, MIPC_SMS_FLAG_ALL);
    }

    callToMipcMsgAsync(msg, delData);
}

void RmmGsmSmsRequestHandler::handleDeleteSmsOnSimRsp(const sp<RfxMclMessage>& msg) {
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

void RmmGsmSmsRequestHandler::handleReportSmsMemStatus(const sp<RfxMclMessage>& msg) {
    int *pData = (int*)(msg->getData()->getData());
    int available;
    sp<RfxMclMessage> rsp;

    // check card status
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        logE(mTag, "SIM card was removed!");
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ABSENT,
            RfxVoidData(), msg, false);
        responseToTelCore(rsp);
        return;
    }

    available = ((int *)pData)[0];
    if (available == 1 || available == 0) {
        sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_CFG_REQ, msg->getSlotId());
        initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_SET_HOST_MEM_AVAILABLE,
                available == 1 ? MIPC_BOOLEAN_TRUE: MIPC_BOOLEAN_FALSE);
        callToMipcMsgAsync(msg, initData);
    }
}

void RmmGsmSmsRequestHandler::handleReportSmsMemStatusRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int cause = mipcData->getResult();
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;

    if (cause == MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_SUCCESS;
    }
    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleGetSmsSimMemStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_GET_STORE_STATUS_REQ,
            msg->getSlotId());
    callToMipcMsgAsync(msg, initData);
}

void RmmGsmSmsRequestHandler::handleGetSmsSimMemStatusRsp(const sp<RfxMclMessage>& msg) {
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

void RmmGsmSmsRequestHandler::sendNewSmsErrorAck(const sp<RfxMclMessage>& msg) {
    int* pEventData = (int*)(msg->getData()->getData());
    int errAckType = pEventData[0];
    int txId = pEventData[1];

    sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_SMS_RSP,
            msg->getSlotId(), txId);
    ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, MIPC_NEW_SMS_ACK_RP_ERROR);
    callToMipcMsgCmdResponse(ackData);

    showCurrIncomingSmsType();
    if (errAckType == ERR_ACK_CMT_ACK_RESET || errAckType == ERR_ACK_CDS_ACK_RESET) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE, SMS_INBOUND_NONE);
    }
}

void RmmGsmSmsRequestHandler::handleGetSmscAddress(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> rsp;

    // check card status
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        logE(mTag, "SIM card was removed!");
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ABSENT,
                RfxStringData(NULL, 0), msg, false);
        responseToTelCore(rsp);
        return;
    }

    sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_CFG_REQ, msg->getSlotId());
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_GET_SCA, 1);
    callToMipcMsgAsync(msg, initData);
}

void RmmGsmSmsRequestHandler::handleGetSmscAddressRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int cause = mipcData->getResult();
    char *sca;
    String8 empty("");
    sp<RfxMclMessage> rsp;

    if (cause == MIPC_RESULT_SUCCESS) {
        sca = (char *)mipcData->getMipcVal(MIPC_SMS_CFG_CNF_T_SCA, NULL);
        if (sca == NULL) {
            sca = (char*)empty.string();
        }
        if (sca != NULL) {
            rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxStringData(sca, strlen(sca)), msg, false);
            responseToTelCore(rsp);
            return;
        }
    }
    rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxStringData(NULL, 0), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleSetSmscAddress(const sp<RfxMclMessage>& msg) {
    int len = -1;
    char *sca = NULL;
    char *tempsca = NULL;
    String8 empty("");
    sp<RfxMclMessage> rsp;

    // check card status
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        logE(mTag, "SIM card was removed!");
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ABSENT,
            RfxVoidData(), msg, false);
        responseToTelCore(rsp);
        return;
    }

    sca = ((msg->getData()->getData() != NULL)?
            (char*)(msg->getData()->getData()) : (char*)empty.string());

    if (sca[0] == '\"') {
        int idx = 0;
        int len = strlen(sca);
        //we need remove " and ", we just copy context, so malloc len - 2 + 1
        //such as, change "+12345" to +12345
        tempsca = (char *) malloc ((len - 1) * sizeof(char));
        RFX_ASSERT(tempsca != NULL);
        for (int i = 1; i < len - 1; i++) {
            tempsca[idx++] = sca[i];
        }
        tempsca[idx] = 0;
        sca = tempsca;
    }

    if (sca != NULL) {
        len = strlen(sca);
    }

    if (len < 0 || len > (MAX_SMSC_LENGTH-1)*2)
    {
        logE(mTag, "The input length of the SCA is invalid: %d", len);
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(rsp);
    }
    else
    {
        sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_CFG_REQ, msg->getSlotId());
        initData->addMipcTlvStr(MIPC_SMS_CFG_REQ_T_SET_SCA, sca);
        initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_SAVE_SETTING, MIPC_BOOLEAN_TRUE);
        callToMipcMsgAsync(msg, initData);
    }

    if (tempsca != NULL) {
        free(tempsca);
    }
}

void RmmGsmSmsRequestHandler::handleSetSmscAddressRsp(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int cause = mipcData->getResult();

    if (cause == MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_SUCCESS;
    }

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleSmsAcknowledgeWithPdu(const sp<RfxMclMessage>& msg) {
    const char *ackSuccess;
    const char *pdu;
    char *pduBytes = NULL;
    int pduLen = 0;
    int smsType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
            SMS_INBOUND_NONE);
    int txId = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TXID, 0);
    sp<RfxMclMessage> rsp;

    if (txId != -1) {
        ackSuccess = ((const char **)(msg->getData()->getData()))[0];
        pdu = ((const char **)(msg->getData()->getData()))[1];

        if (pdu != NULL) {
            pduLen = strlen(pdu);
            if (pduLen > 0) {
                pduBytes = (char *)malloc(sizeof(char) * pduLen / 2);
                RFX_ASSERT(pduBytes != NULL);
                hexStringToBytes(pdu, pduLen, pduBytes, pduLen / 2);
            }
        }

        showCurrIncomingSmsType();
        if (*ackSuccess == '1') {
            if (smsType == SMS_INBOUND_3GPP_CMT || smsType == SMS_INBOUND_3GPP_CMTI) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_SMS_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, MIPC_NEW_SMS_ACK_RP_ACK);
                if (pduBytes != NULL) {
                    ackData->addMipcTlv(MIPC_SMS_NEW_SMS_RSP_T_ACK_PDU, pduLen / 2, pduBytes);
                }
                callToMipcMsgCmdResponse(ackData);
            } else if (smsType == SMS_INBOUND_3GPP_CDS) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_STATUS_REPORT_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_TYPE,
                        MIPC_NEW_SMS_ACK_RP_ACK);
                if (pduBytes != NULL) {
                    ackData->addMipcTlv(MIPC_SMS_NEW_SMS_RSP_T_ACK_PDU, pduLen / 2, pduBytes);
                }
                callToMipcMsgCmdResponse(ackData);
            } else {
            logE(mTag, "SMS type is SMS_NONE");
            }
        } else if (*ackSuccess == '0') {
            if (smsType == SMS_INBOUND_3GPP_CMT || smsType == SMS_INBOUND_3GPP_CMTI) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_SMS_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK, MIPC_NEW_SMS_ACK_RP_ERROR);
                if (pduBytes != NULL) {
                    ackData->addMipcTlv(MIPC_SMS_NEW_SMS_RSP_T_ACK_PDU, pduLen / 2, pduBytes);
                }
                callToMipcMsgCmdResponse(ackData);
            } else if (smsType == SMS_INBOUND_3GPP_CDS) {
                sp<RfxMipcData> ackData = RfxMipcData::obtainMipcData(MIPC_SMS_NEW_STATUS_REPORT_RSP,
                        msg->getSlotId(), txId);
                ackData->addMipcTlvUint8(MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_TYPE,
                        MIPC_NEW_SMS_ACK_RP_ERROR);
                if (pduBytes != NULL) {
                    ackData->addMipcTlv(MIPC_SMS_NEW_SMS_RSP_T_ACK_PDU, pduLen / 2, pduBytes);
                }
                callToMipcMsgCmdResponse(ackData);
            } else {
                logE(mTag, "SMS type is SMS_NONE");
            }
        } else {
            logE(mTag, "unsupported arg to RFX_MSG_REQUEST_SMS_ACKNOWLEDGE\n");
        }

        if (pduBytes != NULL) {
            free(pduBytes);
        }
    } else {
        logE(mTag, "no need to ack");
    }

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE, SMS_INBOUND_NONE);
    rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleGetGsmCbSmsCfg(const sp<RfxMclMessage>& msg,
        GSM_CB_QUERY_MODE query) {
    sp<RfxMipcData> cbData = RfxMipcData::obtainMipcData(MIPC_SMS_CBM_CFG_REQ, msg->getSlotId());
    if (query == GSM_CB_QUERY_CHANNEL) {
        cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_CFG_TYPE,
                MIPC_SMS_CBM_CFG_TYPE_QUERY);
    } else {
        cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_DCS_CFG_TYPE, MIPC_SMS_CBM_CFG_TYPE_QUERY);
    }
    callToMipcMsgAsync(msg, cbData);
}

void RmmGsmSmsRequestHandler::handleGetGsmCbSmsCfgRsp(const sp<RfxMclMessage>& msg,
        GSM_CB_QUERY_MODE query) {
    uint16_t singleChNum = 0, rangeChNum = 0, singleLanNum = 0;
    uint16_t *singleCh = NULL;
    uint32_t *rangeCh = NULL;
    uint8_t *singleLan = NULL;
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> rsp;
    int cause = mipcData->getResult();

    if (cause != MIPC_RESULT_SUCCESS) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxStringData(), msg, false);
        responseToTelCore(rsp);
        return;
    }


    if (query == GSM_CB_QUERY_CHANNEL) {
        singleChNum = mipcData->getMipcArrayTlvNum(MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_SINGLE);
        singleCh = (uint16_t*)malloc(sizeof(uint16_t) * singleChNum);
        RFX_ASSERT(singleCh != NULL);
        for (int i = 0; i < singleChNum; i++) {
            singleCh[i] = mipcData->getMipcUint16IdxVal(
                    MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_SINGLE, i, 0xFFFF);
        }

        rangeChNum = mipcData->getMipcArrayTlvNum(MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_RANGE);
        rangeCh = (uint32_t*)malloc(sizeof(uint32_t) * rangeChNum);
        RFX_ASSERT(rangeCh != NULL);
        for (int i = 0; i < rangeChNum; i++) {
            rangeCh[i] = mipcData->getMipcUint32IdxVal(
                    MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_RANGE, i, 0xFFFFFFFF);
        }

        logD(mTag, "get ch singlelen: %d, rangeLen: %d", singleChNum, rangeChNum);

        if (singleChNum > 0 || rangeChNum > 0) {
            // At least one channel id

            // Convert to RIL_GSM_BroadcastSmsConfigInfo
            int num = singleChNum + rangeChNum;
            RIL_GSM_BroadcastSmsConfigInfo **gsmBciPtrs = (RIL_GSM_BroadcastSmsConfigInfo**)calloc(
                    1, sizeof(RIL_GSM_BroadcastSmsConfigInfo*)*num);
            RFX_ASSERT(gsmBciPtrs != NULL);
            for (int i = 0; i < singleChNum; i++) {
                gsmBciPtrs[i] = (RIL_GSM_BroadcastSmsConfigInfo*)calloc(
                        1, sizeof(RIL_GSM_BroadcastSmsConfigInfo));
                RFX_ASSERT(gsmBciPtrs[i] != NULL);
                gsmBciPtrs[i]->fromServiceId = singleCh[i];
                gsmBciPtrs[i]->toServiceId = singleCh[i];
                gsmBciPtrs[i]->selected = 1;
            }
            for (int i = 0; i < rangeChNum; i++) {
                gsmBciPtrs[singleChNum + i] = (RIL_GSM_BroadcastSmsConfigInfo*)calloc(
                        1, sizeof(RIL_GSM_BroadcastSmsConfigInfo));
                RFX_ASSERT(gsmBciPtrs[singleChNum + i] != NULL);
                gsmBciPtrs[singleChNum + i]->fromServiceId = rangeCh[i] >> 16;
                gsmBciPtrs[singleChNum + i]->toServiceId = rangeCh[i] & 0x0000FFFF;
                gsmBciPtrs[singleChNum + i]->selected = 1;
            }

            // Send AP channel configuration
            rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxGsmCbSmsCfgData((void*)gsmBciPtrs,
                    sizeof(RIL_GSM_BroadcastSmsConfigInfo*)*num), msg, false);
            responseToTelCore(rsp);

            // free the output data
            if (gsmBciPtrs != NULL) {
                for (int i = 0; i < num ; i++)
                {
                    if (gsmBciPtrs[i] != NULL) {
                        free(gsmBciPtrs[i]);
                    }
                }
                free(gsmBciPtrs);
            }
        } else {
            rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxGsmCbSmsCfgData(NULL, 0), msg, false);
            responseToTelCore(rsp);
        }

        if (singleCh != NULL) {
            free(singleCh);
        }
        if (rangeCh != NULL) {
            free(rangeCh);
        }
    } else {
        String8 settings("");

        // languages configuration
        singleLanNum = mipcData->getMipcArrayTlvNum(MIPC_SMS_CBM_CFG_CNF_T_LANGUAGE_SINGLE);
        singleLan = (uint8_t*)malloc(sizeof(uint8_t) * singleLanNum);
        RFX_ASSERT(singleLan != NULL);
        for (int i = 0; i < singleLanNum; i++) {
            singleLan[i] = mipcData->getMipcUint8IdxVal(MIPC_SMS_CBM_CFG_CNF_T_LANGUAGE_SINGLE,
                    i, 0xFF);
        }

        logD(mTag, "get language singlelen: %d", singleLanNum);

        for (int i = 0; i < singleLanNum ; i++) {
            if (i != 0) {
                settings.append(",");
            }
            settings.append(String8::format("%d", singleLan[i]));
        }
        logD(mTag, "get language : %s", settings.string());

        if (singleLanNum > 0) {
            rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxStringData((void*)settings.string(), strlen(settings.string())), msg, false);
            responseToTelCore(rsp);
        } else {
            rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxStringData(), msg, false);
            responseToTelCore(rsp);
        }

        if (singleLan != NULL) {
            free(singleLan);
        }
    }
}

void RmmGsmSmsRequestHandler::handleSetGsmCbSmsCfg(const sp<RfxMclMessage>& msg) {
    int length = msg->getData()->getDataLength();
    int num = length/sizeof(RIL_GSM_BroadcastSmsConfigInfo *);
    int start = -1, end = -1;
    int singleNum = 0, rangeNum = 0;
    uint16_t singleCh[MAX_SINGLE_RANGE_NUM];
    uint32_t rangeCh[MAX_RANGE_NUM];
    RIL_GSM_BroadcastSmsConfigInfo **info =
            (RIL_GSM_BroadcastSmsConfigInfo**)msg->getData()->getData();

    //logD(mTag, "Set CB channel array num %d", num);

    //Convert RIL_GSM_BroadcastSmsConfigInfo
    for (int i = 0; i < num ; i++) {
        start = info[i]->fromServiceId;
        end = info[i]->toServiceId;
        if (start == end) {
            // Single channel id
            if (singleNum < MAX_SINGLE_RANGE_NUM) {
                singleCh[singleNum++] = (uint16_t)start;
            }
        } else {
            // A range of channel id
            if (rangeNum < MAX_RANGE_NUM) {
                rangeCh[rangeNum++] = (start << 16) + (0x0000FFFF & end);
            }
        }
    }

    sp<RfxMipcData> cbData = RfxMipcData::obtainMipcData(MIPC_SMS_CBM_CFG_REQ, msg->getSlotId());
    cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_CFG_TYPE, MIPC_SMS_CBM_CFG_TYPE_ALLOW);
    if (singleNum > 0) {
        cbData->addMipcTlvArrayUint16(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_SINGLE, singleNum, singleCh);
    }

    if (rangeNum > 0) {
        cbData->addMipcTlvArrayUint32(MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_RANGE, rangeNum, rangeCh);
    }
    callToMipcMsgAsync(msg, cbData);
}


void RmmGsmSmsRequestHandler::handleSetGsmCbSmsLanguage(const sp<RfxMclMessage>& msg) {
    char *langStr = (char*)msg->getData()->getData();
    logD(mTag, "Set CB langs %s", langStr);
    char langs[MAX_SINGLE_RANGE_NUM][MAX_RANGE_CHARACTER];
    int num = 0;
    RmmBcRangeParser::split(langStr, ",", langs, &num, MAX_SINGLE_RANGE_NUM);
    uint8_t singleLan[MAX_SINGLE_RANGE_NUM];

    logD(mTag, "Set CB langs len: %d", num);

    for (int i = 0; i < num && i < MAX_SINGLE_RANGE_NUM; i++) {
        singleLan[i] = (uint8_t)atoi(langs[i]);
    }
    if (num > MAX_SINGLE_RANGE_NUM) {
        num = MAX_SINGLE_RANGE_NUM;
    }

    sp<RfxMipcData> cbData = RfxMipcData::obtainMipcData(MIPC_SMS_CBM_CFG_REQ, msg->getSlotId());
    cbData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_DCS_CFG_TYPE, MIPC_SMS_CBM_CFG_TYPE_ALLOW);
    if (num > 0) {
        cbData->addMipcTlvArrayUint8(MIPC_SMS_CBM_CFG_REQ_T_DCS_SINGLE, num, singleLan);
    }

    callToMipcMsgAsync(msg, cbData);
}


void RmmGsmSmsRequestHandler::handleGsmCbSmsActivation(const sp<RfxMclMessage>& msg) {
    int activation = ((int *)msg->getData()->getData())[0];
    logD(mTag, "Turn GSM CB %s", ((activation == 0)? "on" : "off"));

    sp<RfxMipcData> actData = RfxMipcData::obtainMipcData(MIPC_SMS_CBM_CFG_REQ, msg->getSlotId());

    if (activation == 0) {
        actData->addMipcTlvUint32(MIPC_SMS_CBM_CFG_REQ_T_OPEN_CBM_TYPE, MIPC_SMS_CBM_TYPE_CMAS |
                MIPC_SMS_CBM_TYPE_ETWS_PRIMARY | MIPC_SMS_CBM_TYPE_ETWS_SECONDARY |
                MIPC_SMS_CBM_TYPE_OTHERS);
        actData->addMipcTlvUint8(MIPC_SMS_CBM_CFG_REQ_T_PDU_3GPP_SEG_ENABLE, MIPC_BOOLEAN_TRUE);
    } else {
        actData->addMipcTlvUint32(MIPC_SMS_CBM_CFG_REQ_T_OPEN_CBM_TYPE, 0);
    }

    callToMipcMsgAsync(msg, actData);
}

void RmmGsmSmsRequestHandler::handleGsmCbCfgRsp(const sp<RfxMclMessage>& msg) {
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


void RmmGsmSmsRequestHandler::handleGsmGetCbSmsActivationRsp(const sp<RfxMclMessage>& msg) {
    int activation[1] = {0};
    int cbmType;
    RfxMipcData* mipcData = msg->getMipcData();
    sp<RfxMclMessage> rsp;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int cause = mipcData->getResult();

    if (cause == MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_SUCCESS;
        cbmType = mipcData->getMipcUint32Val(MIPC_SMS_CBM_CFG_CNF_T_OPEN_CBM_TYPE, 0);
        if (cbmType != 0) {
            activation[0] = 1;
        }
    }
    rsp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxIntsData((void*)activation, sizeof(activation)), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleGetSmsParams(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_CFG_REQ,
            msg->getSlotId());
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_TEXT_MODE_PARAM_ACTION,
            MIPC_SMS_TEXT_MODE_PARAM_ACTION_QUERY);
    callToMipcMsgAsync(msg, initData);
}

void RmmGsmSmsRequestHandler::handleGetSmsParamsRsp(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> rsp;
    RIL_SmsParams smsParams;

    RfxMipcData* mipcData = msg->getMipcData();
    int cause = mipcData->getResult();

    if (cause != MIPC_RESULT_SUCCESS) {
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxSmsParamsData(NULL, 0), msg, false);
        responseToTelCore(rsp);
        return;
    }

    memset(&smsParams, 0, sizeof(RIL_SmsParams));
    smsParams.format = mipcData->getMipcUint8Val(MIPC_SMS_CFG_CNF_T_TEXT_MODE_FO, 0x00);
    smsParams.vp = mipcData->getMipcUint8Val(MIPC_SMS_CFG_CNF_T_TEXT_MODE_VP, 0x00);
    smsParams.pid = mipcData->getMipcUint8Val(MIPC_SMS_CFG_CNF_T_TEXT_MODE_PID, 0x00);
    smsParams.dcs = mipcData->getMipcUint8Val(MIPC_SMS_CFG_CNF_T_TEXT_MODE_DCS, 0x00);

    rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxSmsParamsData((void*)&smsParams, sizeof(smsParams)), msg, false);
    responseToTelCore(rsp);
}

void RmmGsmSmsRequestHandler::handleSetSmsParams(const sp<RfxMclMessage>& msg) {
    RIL_SmsParams *p_smsParams = (RIL_SmsParams *)(msg->getData()->getData());
    sp<RfxMclMessage> rsp;

    if(p_smsParams->format != 0x00 && p_smsParams->format != 0x02) {
        logE(mTag, "invalid validity period format %d", p_smsParams->format);
        rsp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(rsp);
        return;
    }

    sp<RfxMipcData> initData = RfxMipcData::obtainMipcData(MIPC_SMS_CFG_REQ, msg->getSlotId());
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_TEXT_MODE_PARAM_ACTION,
            MIPC_SMS_TEXT_MODE_PARAM_ACTION_SET);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_SAVE_SETTING, MIPC_BOOLEAN_TRUE);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_TEXT_MODE_FO, p_smsParams->format);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_TEXT_MODE_VP, p_smsParams->vp);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_TEXT_MODE_PID, p_smsParams->pid);
    initData->addMipcTlvUint8(MIPC_SMS_CFG_REQ_T_TEXT_MODE_DCS, p_smsParams->dcs);
    callToMipcMsgAsync(msg, initData);
}

void RmmGsmSmsRequestHandler::handleSetSmsParamsRsp(const sp<RfxMclMessage>& msg) {
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
