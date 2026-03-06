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

#include "RmmCatCommonRequestHandler.h"

static const int request_id_list[] = {
        RFX_MSG_REQUEST_STK_SEND_ENVELOPE_COMMAND,
        RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE,
        RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM,
        RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM_WITH_RESULT_CODE,
        RFX_MSG_REQUEST_REPORT_STK_SERVICE_IS_RUNNING,
        RFX_MSG_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS,
        RFX_MSG_REQUEST_STK_SEND_RESPONSE_BY_CMDTYPE,
        // BIP @{
        RFX_MSG_REQUEST_BIP_SEND_CONFIRM_INFO,
        // BIP @}
        // BTSAP @{
        RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS
        // BTSAP @}
};

// modem status query fd
int RmmCatCommonRequestHandler::sMdStatusFd = -1;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmCatCommonRequestHandler, RIL_CMD_PROXY_1);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_STK_IS_RUNNING);


RmmCatCommonRequestHandler::RmmCatCommonRequestHandler(int slot_id,
        int channel_id): RfxBaseHandler(slot_id, channel_id) {
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(int));

    //init member variables
    isEventNotifyQueued = false;
    isProaCmdQueued = false;
    pEventNotifyCmd = NULL;
    pProactiveCmd = NULL;

    //init mMdStatusQueryThread and start the query thread
    memset(&mMdStatusQueryThread, 0, sizeof(pthread_t));
    int mainSlotId = RfxRilUtils::getMajorSim() - 1;
    if (mainSlotId == slot_id) {
        initMdStatusQueryLoop();
    }
}

RmmCatCommonRequestHandler::~RmmCatCommonRequestHandler() {
}

void RmmCatCommonRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s", RFX_ID_TO_STR(request));

    switch(request) {
        case RFX_MSG_REQUEST_REPORT_STK_SERVICE_IS_RUNNING:
            requestReportStkServiceIsRunning(msg);
            break;
        case RFX_MSG_REQUEST_STK_SEND_ENVELOPE_COMMAND:
        case RFX_MSG_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS:
            requestStkSendEnvelopeCommand(msg);
            break;
        case RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE:
            requestStkSendTerminalResponse(msg);
            break;
        case RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM:
        case RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM_WITH_RESULT_CODE:
            requestStkHandleCallSetupRequestedFromSim(msg);
            break;
        case RFX_MSG_REQUEST_STK_SEND_RESPONSE_BY_CMDTYPE:
            requestStkSendResponseByCmdType(msg);
            break;
        // BIP @{
        case RFX_MSG_REQUEST_BIP_SEND_CONFIRM_INFO:
            requestBipSendConfirmInfo(msg);
            break;
        // BIP @}
        // BTSAP @{
        case RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS:
            requestBtSapTransferCardReaderStatus(msg);
            break;
        // BTSAP @}
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmCatCommonRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(RFX_LOG_TAG, "onHandleResponse: %d", msg->getId());
    }

    RfxMipcData* data = msg->getMipcData();
    if (data == NULL) {
        logE(RFX_LOG_TAG, "onHandleResponse: getMipcData() return null");
        responseVoidDataToTcl(msg, RIL_E_GENERIC_FAILURE);
        return;
    }

    logD(RFX_LOG_TAG, "onHandleResponse MipcMsgId:%d, MipcSlotId:%d result:%d",
             data->getMipcMsgId(), data->getMipcSlotId(), data->getResult());
    int ret = 0;
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS:
            ret = handleSendEnvelopeCnf(msg);
            break;
        default:
            responseVoidDataToTcl(msg);
            break;
    }
    if (ret != 0) {
        responseVoidDataToTcl(msg, RIL_E_GENERIC_FAILURE);
    }
}
int RmmCatCommonRequestHandler::handleSendEnvelopeCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    RIL_SIM_IO_Response sr;
    memset(&sr, 0, sizeof(sr));
    char* response = NULL;
    if (data == NULL || data->getResult() != MIPC_RESULT_SUCCESS) {
         return -1;
    }

    uint16_t status = data->getMipcUint32Val(MIPC_STK_SEND_ENVELOPE_CNF_T_STATUS_WORDS, 0);
    sr.sw1 = (status & 0xFF00) >> 8;
    sr.sw2 = status & 0x00FF;
    uint16_t len = 0;
    response = (char *)data->getMipcVal(MIPC_STK_SEND_ENVELOPE_CNF_T_ENVELOPE_RESPONSE,&len);
    logD(RFX_LOG_TAG, "handleSendEnvelopeCnf: %s,%d %d", response,sr.sw1,sr.sw2);
    if (response != NULL) {
        asprintf(&sr.simResponse, "%s", response);
    }

    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg);
    responseToTelCore(mclResponse);

    if (sr.simResponse != NULL) {
        free(sr.simResponse);
        sr.simResponse = NULL;
    }
    return 0;
}

void RmmCatCommonRequestHandler::responseVoidDataToTcl(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    RIL_Errno err = (data != NULL && data->getResult() == MIPC_RESULT_SUCCESS) ?
            RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

void RmmCatCommonRequestHandler::responseVoidDataToTcl(const sp<RfxMclMessage>& msg, RIL_Errno err) {
    sp<RfxMclMessage> mclResponse =
        RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

void RmmCatCommonRequestHandler::requestReportStkServiceIsRunning(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG,  "requestReportStkServiceIsRunning");
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);

    // response to TeleCore
    responseToTelCore(response);
}
void RmmCatCommonRequestHandler::requestStkSendEnvelopeCommand (const sp<RfxMclMessage>& msg)
{
    char* params = (char *)msg->getData()->getData();
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) <=  0) {
         rilErrNo = RIL_E_SIM_ABSENT;
         sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
                 RfxStringData(), msg, false);
          // response to TeleCore
         responseToTelCore(response);
         return;
    }

    // From this version, use AT+CUSATE instead of AT+STKENV
    params = (params == NULL) ? ((char *)("")) : params;
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_STK_SEND_ENVELOPE_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_STK_SEND_ENVELOPE_REQ_T_ENVELOPE_LEN, strlen(params)+1);
    data->addMipcTlvStr(MIPC_STK_SEND_ENVELOPE_REQ_T_ENVELOPE_PTR,params);
    logD(RFX_LOG_TAG, "MIPC_STK_SEND_ENVELOPE_REQ");
    callToMipcMsgAsync(msg, data);
}

void RmmCatCommonRequestHandler::requestStkSendTerminalResponse (const sp<RfxMclMessage>& msg) {
    char* params = (char *)msg->getData()->getData();
    params = (params == NULL) ? ((char *)("")) : params;
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(
            MIPC_STK_SEND_TERMINAL_RESPONSE_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_LEN, strlen(params)+1);
    data->addMipcTlvStr(MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_PTR,params);
    logD(RFX_LOG_TAG, "MIPC_STK_SEND_TERMINAL_RESPONSE_REQ");
    callToMipcMsgAsync(msg, data);
}

void RmmCatCommonRequestHandler::requestStkHandleCallSetupRequestedFromSim (const sp<RfxMclMessage>& msg) {
    RfxIntsData *intsData = (RfxIntsData*)msg->getData();
    int *userConfirm = (int*)intsData->getData();

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(
            MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ_T_DATA, userConfirm[0]);
    logD(RFX_LOG_TAG, "MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ: %d" , userConfirm[0]);
    callToMipcMsgAsync(msg, data);
}

void RmmCatCommonRequestHandler::requestStkSendResponseByCmdType (const sp<RfxMclMessage>& msg) {
    char* cmd = NULL;
    int cmd_type = 0;
    int cmd_qualifier = 0;
    bool err_flag = true;
    RfxIntsData *intsData = NULL;
    int *data = NULL;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    logD(RFX_LOG_TAG,  "requestStkSendResponseByCmdType");

    intsData = (RfxIntsData*)msg->getData();
    if (intsData != NULL) {
        data = (int*)intsData->getData();
        if (data != NULL) {
            cmd_type = data[0];
            cmd_qualifier = data[1];
            err_flag = false;
        }
    }

    if (err_flag) {
        //For parsing error, we just send response to telcore
        rilErrNo = RIL_E_GENERIC_FAILURE;
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    logD(RFX_LOG_TAG,  "requestStkSendResponseByCmdType: cmd_type = %d, cmd_qual = %d .",
        cmd_type, cmd_qualifier);

    if (CMD_SETUP_CALL == cmd_type) {
        sp<RfxMipcData> mipcdata = RfxMipcData::obtainMipcData(
                MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ, msg->getSlotId());
        mipcdata->addMipcTlvUint8(MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ_T_DATA, 34);
        logD(RFX_LOG_TAG, "MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ : 34");
        callToMipcMsgAsync(msg, mipcdata);
        return;
    } else if (CMD_SETUP_MENU == cmd_type || CMD_IDLE_MODEL_TXT == cmd_type) {
        asprintf(&cmd, "810301%02x%02x82028281830100", cmd_type, cmd_qualifier);
    } else {
        asprintf(&cmd, "810301%02x%02x82028281830110", cmd_type, cmd_qualifier);
    }

    sp<RfxMipcData> mipcdata = RfxMipcData::obtainMipcData(
            MIPC_STK_SEND_TERMINAL_RESPONSE_REQ, msg->getSlotId());
    if (cmd != NULL) {
        mipcdata->addMipcTlvUint32(MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_LEN, strlen(cmd)+1);
        mipcdata->addMipcTlvStr(MIPC_STK_SEND_TERMINAL_RESPONSE_REQ_T_TR_PTR,cmd);
        logD(RFX_LOG_TAG, "MIPC_STK_SEND_TERMINAL_RESPONSE_REQ");
        callToMipcMsgAsync(msg, mipcdata);
    }
    free(cmd);
    cmd = NULL;
}

// BIP @{
void RmmCatCommonRequestHandler::requestBipSendConfirmInfo (const sp<RfxMclMessage>& msg) {
    int cmd_num = 0;
    int result_code = 0;
    bool err_flag = true;
    RfxIntsData *intsData = NULL;
    int *data = NULL;
    sp<RfxAtResponse> p_response;

    logD(RFX_LOG_TAG,  "requestBipSendConfirmInfo");

    intsData = (RfxIntsData*)msg->getData();
    if (intsData != NULL) {
        data = (int*)intsData->getData();
        if (data != NULL) {
            cmd_num = data[0];
            //Mapping result code according to spec here
            //Currently we can get 1 or 0 from framework
            result_code = (data[1] == 1) ? RESULT_OK : RESULT_USER_NOT_ACCEPT;
            err_flag = false;
        }
    }

    if (err_flag) {
        //For parsing error, we set cmd_num to default 1, and result_code to 0x3a
        cmd_num = 1;
        result_code = RESULT_BIP_ERROR;
    }

    logD(RFX_LOG_TAG,  "requestBipSendConfirmInfo: cmd_num = %d, result_code = %d .", cmd_num, result_code);

    sp<RfxMipcData> mipcdata = RfxMipcData::obtainMipcData(
            MIPC_STK_SEND_BIPCONF_REQ, msg->getSlotId());
    mipcdata->addMipcTlvUint32(MIPC_STK_SEND_BIPCONF_REQ_T_CMD_NUM, cmd_num);
    mipcdata->addMipcTlvUint32(MIPC_STK_SEND_BIPCONF_REQ_T_RESULT, result_code);
    logD(RFX_LOG_TAG, "MIPC_STK_SEND_BIPCONF_REQ");
    callToMipcMsgAsync(msg, mipcdata);

}
// BIP @}

//BTSAP @{
void RmmCatCommonRequestHandler::requestBtSapTransferCardReaderStatus(const sp<RfxMclMessage>& msg) {
    //RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ *req = NULL;
    RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP rsp;
    //sp<RfxAtResponse> p_response;
    int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS);

    logD(RFX_LOG_TAG, "[BTSAP] requestBtSapTransferCardReaderStatus start, BTSAP status: %d .", status);
    //req = (RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ*)calloc(1, sizeof(RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ));

    rsp.CardReaderStatus = BT_SAP_CARDREADER_RESPONSE_DEFAULT;
    rsp.response = RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_Response_RIL_E_SUCCESS;
    rsp.has_CardReaderStatus = true;   //  always true

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) > 0) {
        logD(RFX_LOG_TAG, "[BTSAP] requestBtSapGetCardStatus, Sim inserted .");
        rsp.CardReaderStatus = rsp.CardReaderStatus | BT_SAP_CARDREADER_RESPONSE_SIM_INSERT;
    } else {
        logD(RFX_LOG_TAG, "[BTSAP] requestBtSapGetCardStatus, But sim not inserted");
        rsp.response = RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_Response_RIL_E_GENERIC_FAILURE;
        sendStkBtSapResponseComplete(msg, RIL_E_GENERIC_FAILURE,
                RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS, &rsp);
        //if (req != NULL) {
        //    free(req);
        //}
        return;
    }

    if (status == BT_SAP_CONNECTION_SETUP || status == BT_SAP_ONGOING_CONNECTION
        || status == BT_SAP_POWER_ON) {
        rsp.CardReaderStatus = rsp.CardReaderStatus | BT_SAP_CARDREADER_RESPONSE_READER_POWER;
    } else {
        // For BT_SAP_INIT, BT_SAP_DISCONNECT and BT_SAP_POWER_OFF, return generic fail
        rsp.response = RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_Response_RIL_E_GENERIC_FAILURE;
    }

    logD(RFX_LOG_TAG, "[BTSAP] requestBtSapGetCardStatus, CardReaderStatus result : %x .", rsp.CardReaderStatus);

    sendStkBtSapResponseComplete(msg, RIL_E_SUCCESS, RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS, &rsp);

    //free(req);
    logD(RFX_LOG_TAG, "[BTSAP] requestBtSapGetCardStatus end .");
}

void RmmCatCommonRequestHandler::sendStkBtSapResponseComplete(const sp<RfxMclMessage>& msg,
        RIL_Errno ret, int msgId, void *data) {
    const pb_field_t *fields = NULL;
    size_t encoded_size = 0;
    uint32_t written_size = 0;
    size_t buffer_size = 0;
    pb_ostream_t ostream;
    bool success = false;
    sp<RfxMclMessage> response = NULL;

    logD(RFX_LOG_TAG, "[BTSAP] sendStkBtSapResponseComplete, start .");
    switch (msgId) {
        case RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS:
            fields = RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_fields;
            break;
        default:
            logD(RFX_LOG_TAG, "[BTSAP] sendStkBtSapResponseComplete, MsgId is mistake!");
            return;
    }

    if ((success = pb_get_encoded_size(&encoded_size, fields, data)) &&
            encoded_size <= INT32_MAX) {
        buffer_size = encoded_size;
        uint8_t buffer[buffer_size];
        ostream = pb_ostream_from_buffer(buffer, buffer_size);
        success = pb_encode(&ostream, fields, data);

        if(success) {
            logD(RFX_LOG_TAG, "[BTSAP] sendStkBtSapResponseComplete, Size: %zu (0x%zx) Size as written: 0x%x",
                encoded_size, encoded_size, written_size);
            response = RfxMclMessage::obtainSapResponse(msgId, ret,
                    RfxRawData((void*)buffer, buffer_size), msg, false);
            responseToTelCore(response);
        } else {
            logD(RFX_LOG_TAG, "[BTSAP] sendStkBtSapResponseComplete, Encode failed!");
        }
    } else {
        logD(RFX_LOG_TAG, "Not sending response type %d: encoded_size: %zu. encoded size result: %d",
            msgId, encoded_size, success);
    }
}
// BTSAP @}

void RmmCatCommonRequestHandler::initMdStatusQueryLoop() {
    logD(RFX_LOG_TAG, "initMdStatusQueryLoop");
    mMdStatusQueryThread = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int result = pthread_create(&mMdStatusQueryThread, &attr, mdStatusQueryLoop, NULL);
    if (result != 0) {
        logE("initMdStatusQueryLoop: Failed to create dispatch thread: %s", strerror(result));
        return;
    }
}

#if defined(MTK_RIL_THIN_MD_SUPPORT)
void *RmmCatCommonRequestHandler::mdStatusQueryLoop(void* params __unused) {
    return NULL;
}

void RmmCatCommonRequestHandler::getResetLock() {
}

void RmmCatCommonRequestHandler::releaseResetLock() {
}
#else
void *RmmCatCommonRequestHandler::mdStatusQueryLoop(void* params __unused) {
    int count = -1;
    int mdStatus = -1;

    //  dev/ccci_mdx_sta: contains all modem status
    //  dev/ccci_md1_sta: contains gsm modem status
    //  dev/ccci_md3_sta: contains c2k modem status
    sMdStatusFd = open("/dev/ccci_mdx_sta", O_RDWR);
    RfxRilUtils::printLog(INFO, String8::format("%s", RFX_LOG_TAG),
            String8::format("mdStatusQueryLoop: fd = %d", sMdStatusFd), RFX_SLOT_ID_0);

    if (sMdStatusFd < 0) {
        RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG),
                String8::format("ccci_mdx_sta port is not available!"), RFX_SLOT_ID_0);
        return NULL;
    }

    getResetLock();
    MdStatusEvent *buffer = (MdStatusEvent *) calloc(1, sizeof(MdStatusEvent));
    if (buffer == NULL) {
        RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG),
                String8::format("mdStatusQueryLoop: calloc buffer error!"), RFX_SLOT_ID_0);
        releaseResetLock();
        return NULL;
    }

    do {
        count = read(sMdStatusFd, buffer, sizeof(MdStatusEvent));
        if (mdStatus == buffer->event_type) {
            continue;
        }
        RfxRilUtils::printLog(INFO, String8::format("%s", RFX_LOG_TAG),
                String8::format(
                        "mdStatusQueryLoop: count: %d modem id: %d, event type: %d, reason: %s",
                        count, buffer->md_id, buffer->event_type, buffer->reason), RFX_SLOT_ID_0);
        // update local mdStatus and property
        mdStatus = buffer->event_type;
        if (MD_STA_EV_READY == mdStatus) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "ready");
        } else if (MD_STA_EV_RESET_REQUEST == mdStatus) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "reset");
        } else if (MD_STA_EV_STOP_REQUEST) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "stop");
        } else if (MD_STA_EV_ENTER_FLIGHT_REQUEST == mdStatus
            || MD_STA_EV_LEAVE_FLIGHT_REQUEST == mdStatus
            || MD_STA_EV_ENTER_FLIGHT_E_REQUEST == mdStatus
            || MD_STA_EV_LEAVE_FLIGHT_E_REQUEST == mdStatus) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "flightmode");
        } else if (MD_STA_EV_EXCEPTION == mdStatus) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "exception");
        } else {
            RfxRilUtils::printLog(INFO, String8::format("%s", RFX_LOG_TAG),
                    String8::format("mdStatusQueryLoop: Not cared status!"), RFX_SLOT_ID_0);
        }

        if (MD_STA_EV_RESET_REQUEST == buffer->event_type
            || MD_STA_EV_LEAVE_FLIGHT_REQUEST == buffer->event_type
            || MD_STA_EV_LEAVE_FLIGHT_E_REQUEST == buffer->event_type) {
            RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG),
                    String8::format("mdStatusQueryLoop: RESET/STOP/ENTER_FLIGHT, break!"),
                    RFX_SLOT_ID_0);
            break;
        }
    } while(1);

    releaseResetLock();
    free(buffer);
    close(sMdStatusFd);

    return NULL;
}

// get reset lock at bootup
void RmmCatCommonRequestHandler::getResetLock() {
    int ret = ioctl(sMdStatusFd, CCCI_IOC_HOLD_RST_LOCK, -1);
    RfxRilUtils::printLog(INFO, String8::format("%s", RFX_LOG_TAG),
            String8::format("getResetLock result: %d", ret), RFX_SLOT_ID_0);
}

// release reset lock at following case
// 1. Leave flight mode
// 2. Receive MD_STA_EV_RESET_REQUEST
// 3. Receive MD_STA_EV_STOP_REQUEST
void RmmCatCommonRequestHandler::releaseResetLock() {
    int ret = ioctl(sMdStatusFd, CCCI_IOC_FREE_RST_LOCK, -1);
    RfxRilUtils::printLog(INFO, String8::format("%s", RFX_LOG_TAG),
            String8::format("releaseResetLock result: %d", ret), RFX_SLOT_ID_0);
}
#endif
