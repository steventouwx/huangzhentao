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
#include "RfxVoidData.h"
#include "RfxIntsData.h"

#include "RmmVtReqHandler.h"
#include "RmmVtDataThreadController.h"

#include "RfxVtSendMsgData.h"
#include "RmmVtMsgParser.h"

// for send MCL request to myself
#include "RfxMclDispatcherThread.h"
#include "RfxMclMessage.h"
#include "RfxMessageId.h"

// for socket / thread
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cutils/sockets.h>
#include <sys/socket.h>
#include <sys/un.h>

#define RFX_LOG_TAG "VT RIL RMM"

/*****************************************************************************
 * Class RmmVtReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmVtReqHandler, RIL_CMD_PROXY_IMS);

static int mSimOpIdTable[MAX_SIM_COUNT] = {0};

RmmVtReqHandler::RmmVtReqHandler(int slot_id, int channel_id)
: RfxBaseHandler(slot_id, channel_id) {

    RFX_LOG_I(RFX_LOG_TAG, "[RMM VT REQ HDLR] RmmVtReqHandler create (slot_id = %d)", slot_id);

    mIsReadySent = false;

    if (!RmmVtReqHandler::isImsVideoCallon()) {
        return;
    }

    const int requestList[] = {
        RFX_MSG_REQUEST_GET_INFO,
        RFX_MSG_REQUEST_UPDATE_OPID,
    };

    registerToHandleRequest(requestList, sizeof(requestList)/sizeof(int));

    const int CommonEventList[] = {
        RFX_MSG_EVENT_CALL_STATUS_UPDATE,
        RFX_MSG_EVENT_VT_SEND_MSG,
        RFX_MSG_EVENT_VT_COMMON_DATA
    };

    registerToHandleEvent(CommonEventList, sizeof(CommonEventList) / sizeof(int));

    const int imsEventList[] = {
        RFX_MSG_EVENT_IMS_DATA,
    };

    registerToHandleEvent(RIL_IMS_Client_VILTE, imsEventList, sizeof(imsEventList) / sizeof(int));

    // create thread controller
    if (slot_id == 0) {
        mThdController = new RmmVtDataThreadController();
        mThdController->start();
        reportRildStateReady(); //report rild ready
    }

    char feature[] = "IMS SUBMARINE";
    int support = getFeatureVersion(feature);

    char dev[RIL_SUPPORT_CHANNELS_MAX_NAME_LEN] = {0};
    // IMS SUBMARINE, 0:not support, 1:support
    RFX_LOG_I(RFX_LOG_TAG, "is SUBMARINE Support, %s = %d", feature, support);
    int real_channel_id = slot_id * RIL_CHANNEL_OFFSET + channel_id;
    if (support) {
        if (slot_id == 0) {
            strncpy(dev, RIL_TTY_CHANNEL_IMS, RIL_SUPPORT_CHANNELS_MAX_NAME_LEN - 1);
        } else {
            RFX_LOG_I(RFX_LOG_TAG, "submaring, and current not SIM1, just skip because ccci_vts to RIL_CMD_IMS for all SIMs");
            RfxChannelManager::initImsFd(real_channel_id, IMS_FD_USELESS);
            return;
        }
    } else {
        //here channel id is always RIL_CMD_IMS, need to consider for SIM2
        RFX_LOG_I(RFX_LOG_TAG, "not SUBMARINE, target dev: %s", RfxChannelManager::getMuxPath(real_channel_id));
        strncpy(dev, RfxChannelManager::getMuxPath(real_channel_id), RIL_SUPPORT_CHANNELS_MAX_NAME_LEN - 1);
    }
    int count = 0;
    RfxChannel *channel = RfxChannelManager::getChannel(real_channel_id);
    while (channel->getSender()->getFd() < 0) {
        RFX_LOG_I(RFX_LOG_TAG, "no sender fd, begin to open, real_channel_id:%d", real_channel_id);
        int fd = open(dev, O_RDWR);
        if (fd < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "could not connect to %s: %s, count:%d",
                    dev, strerror(errno), count);

            if (count == 10) {
                fd = -1;
                RFX_LOG_E(RFX_LOG_TAG, "giveup connectting to %s", dev);
                break;
            }
            count++;
            usleep(100 * 1000);
            /* never returns */
        } else {
            RFX_LOG_I(RFX_LOG_TAG, "open dev successfully %s", dev);
            struct termios ios;
            tcgetattr(fd, &ios);
            ios.c_lflag = 0; /* disable ECHO, ICANON, etc... */
            ios.c_iflag = 0;
            tcsetattr(fd, TCSANOW, &ios);
            RfxChannelManager::initImsFd(real_channel_id, fd);
        }
        RFX_LOG_I(RFX_LOG_TAG, "getinfo,real_channel_id=%d, senderFd=%d", real_channel_id, channel->getSender()->getFd());
    }
}

RmmVtReqHandler::~RmmVtReqHandler() {
}

bool RmmVtReqHandler::isImsVideoCallon(void) {
    bool isViLTESupport = RfxRilUtils::isVilteSupport();
    bool isViWiFiSupport = RfxRilUtils::isViwifiSupport();

    if (isViLTESupport || isViWiFiSupport) {
        if (isVTLogEnable()) {
            RFX_LOG_I(RFX_LOG_TAG, "[RMM VT REQ HDLR] isImsVideoCallon : True");
        }
        return true;
    } else {
        if (isVTLogEnable()) {
            RFX_LOG_I(RFX_LOG_TAG, "[RMM VT REQ HDLR] isImsVideoCallon : False");
        }
        return false;
    }
}

bool RmmVtReqHandler::isVTLogEnable(void) {
    char vt_log[100] = {0};

    rfx_property_get("persist.vendor.rilvt.log_enable", vt_log, "0");
    return (atoi(vt_log) == 1)? true: false;
}

void RmmVtReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {

    int request = msg->getId();

    RFX_LOG_I(RFX_LOG_TAG, "[Handle REQ] request = %d", request);

    switch (request) {
        case RFX_MSG_REQUEST_GET_INFO:
            handleRequestGetInfo(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_OPID:
            handleRequestUpdateOpid(msg);
            break;
        default:
            RFX_LOG_I(RFX_LOG_TAG, "unknown request, ignore!");
            break;
    }
}

void RmmVtReqHandler::onHandleEvent(const sp<RfxMclMessage> & msg) {

    int event = msg->getId();

    RFX_LOG_I(RFX_LOG_TAG, "[Handle EVT] event = %d", event);

    switch (event) {
        case RFX_MSG_EVENT_CALL_STATUS_UPDATE:
            handleEventCallStatusUpdate(msg);
            break;
        case RFX_MSG_EVENT_VT_SEND_MSG:
            handleEventVtSendMsg(msg);
            break;
        case RFX_MSG_EVENT_IMS_DATA:
        case RFX_MSG_EVENT_VT_COMMON_DATA:
            handleEventVtReceiveMsg(msg);
            break;
        default:
            RFX_LOG_I(RFX_LOG_TAG, "[Handle EVT] unknown event, ignore!");
            break;
    }
}

void RmmVtReqHandler::handleRequestGetInfo(const sp<RfxMclMessage>& msg) {

    //const RIL_VT_CALL_STATUS_UPDATE *pRspData = (const RIL_VT_CALL_STATUS_UPDATE *)msg->getData()->getData();
    //int call_id = pRspData->call_id;
    //int call_state = pRspData->call_state;

    //sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    //sp<RfxMclMessage> urc;

    // send AT cmd
    /*
    p_response = atSendCommand(String8::format("AT+XXXXX=%d", call_state));

    if (p_response->isAtResponseFail()) {
                cause = p_response->atGetCmeError();
        RFX_LOG_E(RFX_LOG_TAG, "handleRequestGetInfo: response error");
        goto error;
    }
    */

    // send URC
    /*
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_GET_INFO, m_slot_id, RfxIntsData(&allowed, 1));
    responseToTelCore(urc);
    */

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(response);

    return;
/*
error:
    RFX_LOG_E(RFX_LOG_TAG, "[GET  INFO ] ERROR cause= %d", cause);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE, RfxIntsData(&cause, 1), msg, true);
    responseToTelCore(response);
    return;
*/
}

void RmmVtReqHandler::handleEventCallStatusUpdate(const sp<RfxMclMessage>& msg) {

    const RIL_VT_CALL_STATUS_UPDATE *pRspData = (const RIL_VT_CALL_STATUS_UPDATE *)msg->getData()->getData();
    int call_id = pRspData->call_id;
    int call_state = pRspData->call_state;

    RFX_LOG_D(RFX_LOG_TAG, "[STATUS UPD] call_id = %d, call_state = %d", call_id, call_state);

    return;
}

void RmmVtReqHandler::handleEventVtSendMsg(const sp<RfxMclMessage>& msg) {
    const RIL_VT_SERVICE_MSG *pSendMsg = (const RIL_VT_SERVICE_MSG *)msg->getData()->getData();

    RFX_LOG_D(RFX_LOG_TAG, "[VTREQ SENT] pSendMsg->size = %d, pSendMsg->data = %lu",
                pSendMsg->size, (unsigned long) pSendMsg->data);

    int msgId = (* ((int*)pSendMsg->data));
    if (msgId == MSG_ID_WRAP_IMSVT_RILD_VTSERVICE_STATUS) {

        VT_STATUS_REQ* srvStatusStruct = (VT_STATUS_REQ* )(pSendMsg->data + 8);
        if (1 == srvStatusStruct->status) {
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] RILD_IMS_CLIENT_READY");

            if (mIsReadySent) {
                RFX_LOG_I(RFX_LOG_TAG, "mIsReadySent is true, return");
                return;
            }

            // Notify RIL FWK that we are ready to recv message (VTService).
            // Sometimes remote side may be ready earler than us and send message firstly
            // so we need to tell FWK when we can recv message or the earlier message will lost.
            sendUserData(RIL_IMS_Client_VILTE, RIL_IMS_CLIENT_READY, NULL, 0);
            mIsReadySent = true;
        } else {
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] RILD_IMS_CLIENT_NOT_READY");
            sendUserData(RIL_IMS_Client_VILTE, RIL_IMS_CLIENT_NOT_READY, NULL, 0);
            mIsReadySent = false;
        }

        return;
    } else if (msgId == MSG_ID_WRAP_IMSVT_RILD_GET_OP_ID_REQ) {
        VT_GET_OP_ID_REQ* srvOpIdStruct = (VT_GET_OP_ID_REQ* )(pSendMsg->data + 8);
        int sim_slot_id = srvOpIdStruct->sim_slot_id;

        RFX_LOG_I(RFX_LOG_TAG, "sim_slot_id = %d", sim_slot_id);

        if (sim_slot_id >= 0 && sim_slot_id < MAX_SIM_COUNT && mSimOpIdTable[sim_slot_id] >= 0) {
            //report only have valid oprator id
            reportOperatorId(sim_slot_id, mSimOpIdTable[sim_slot_id]);
        } else {
            RFX_LOG_I(RFX_LOG_TAG, "invalid operator id!!!");
        }
        return;
    }

    if (msgId == MSG_ID_WRAP_IMSVT_IMCB_GET_CAP_RSP) {

        VT_IMCB_CAP* capRspStruct = (VT_IMCB_CAP* )(pSendMsg->data + 8);

        // temp log for get cap
        if (isVTLogEnable()) {

            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] accout_id = %d", capRspStruct->accout_id);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] video_cap_num = %d", capRspStruct->video_cap_num);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] profile_level_id = %d", capRspStruct->video_cap[0].profile_level_id);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] max_recv_level = %d", capRspStruct->video_cap[0].max_recv_level);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] packetization_mode = %d", capRspStruct->video_cap[0].packetization_mode);

            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] bitrate_info format = %d", capRspStruct->bitrate_info[0].format);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] bitrate_info profile = %d", capRspStruct->bitrate_info[0].profile);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] bitrate_info level = %d", capRspStruct->bitrate_info[0].level);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] bitrate_info minbitrate = %d", capRspStruct->bitrate_info[0].minbitrate);
            RFX_LOG_I(RFX_LOG_TAG, "[VTREQ SENT] bitrate_info bitrate = %d", capRspStruct->bitrate_info[0].bitrate);

        }
    }

    // send to RIL FWK
    sendUserData(RIL_IMS_Client_VILTE, (unsigned char* ) pSendMsg->data, pSendMsg->size);

    return;
}

void RmmVtReqHandler::handleEventVtReceiveMsg(const sp<RfxMclMessage>& msg) {
    const char *pRecvMsg = (const char *)msg->getData()->getData();
    int length = msg->getData()->getDataLength();

    RFX_LOG_I(RFX_LOG_TAG, "[VTREQ RECV] data length = %d, slot = %d\n", length, m_slot_id);

    //Add operator id for MSG_ID_WRAP_IMSVT_IMCB_GET_CAP_IND
    int msgId = (* ((int*)pRecvMsg));
    if (msgId == MSG_ID_WRAP_IMSVT_IMCB_GET_CAP_IND) {

        VT_IMCB_CAPIND* capIndStruct = (VT_IMCB_CAPIND* )(pRecvMsg + 8);
        int sim_slot_id = capIndStruct->sim_slot_id;

        //this file is only for GEN98 or newer, if the operator code is 0 from modem, it mean test sim without operator information
        //just handle it as a test sim and report to upper layer, codec has default case for operatorID 0.
        if (capIndStruct->operator_code == 0) {
            RFX_LOG_I(RFX_LOG_TAG, "OP ID from modem is 0, mayby test SIM without operator ID");
        }
        //just save the operator ID from modem.
        mSimOpIdTable[sim_slot_id] = capIndStruct->operator_code;
    }
    //no need to check opid preserved loacally, just send to VTService.
    sendMsgToVTS((char*)pRecvMsg, length, "handleRequestVtReceiveMsg");

    return;
}

void RmmVtReqHandler::handleRequestUpdateOpid(const sp<RfxMclMessage>& msg) {
    int *data = (int *)msg->getData()->getData();
    int slot_id = ((int *)data)[0];
    int operator_id = ((int *)data)[1];

    RFX_LOG_I(RFX_LOG_TAG, "no need set mSimOpIdTable[%d] = %d here any more\n", slot_id, operator_id);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmmVtReqHandler::reportRildStateReady() {

    if (isVTLogEnable()) {
        RFX_LOG_I(RFX_LOG_TAG, "reportRildStateReady");
    }

    char* outBuffer = NULL;
    int msgType = MSG_ID_WRAP_IMSVT_RILD_RILD_STATUS;
    int RildIndLength = sizeof(VT_RIL_STATUS_IND);
    int length = RildIndLength + 1 + 8;

    outBuffer = (char* ) calloc(length, sizeof(char));

    if (outBuffer == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "fail to calloc outBuffer");
        return;
    }

    (* ((int*)outBuffer))       = msgType;
    (* ((int*)outBuffer + 1))   = RildIndLength;

    VT_RIL_STATUS_IND RildStatusInd;
    memset(&RildStatusInd, 0, RildIndLength);

    RildStatusInd.sim_slot_id = 0;
    RildStatusInd.status = 1;  // 1:Ready, 0:Not readay

    memcpy(outBuffer + 8, &RildStatusInd, RildIndLength);

    sendMsgToVTS(outBuffer, length, "ReportVTRildStatus");
    free(outBuffer);

}

void RmmVtReqHandler::reportOperatorId(int slot_id, int op_id) {

    RFX_LOG_I(RFX_LOG_TAG, "reportOperatorId");

    char* outBuffer = NULL;
    int msgType = MSG_ID_WRAP_IMSVT_RILD_GET_OP_ID_RSP;
    int OpIdRspLength = sizeof(VT_GET_OP_ID_RSP);
    int length = OpIdRspLength + 1 + 8;

    outBuffer = (char* ) calloc(length, sizeof(char));

    if (outBuffer == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "fail to calloc outBuffer");
        return;
    }

    (* ((int*)outBuffer))       = msgType;
    (* ((int*)outBuffer + 1))   = OpIdRspLength;

    VT_GET_OP_ID_RSP OpIdRsp;
    memset(&OpIdRsp, 0, OpIdRspLength);

    OpIdRsp.sim_slot_id = slot_id;
    OpIdRsp.op_id = op_id;

    memcpy(outBuffer + 8, &OpIdRsp, OpIdRspLength);

    sendMsgToVTS(outBuffer, length, "reportOperatorId");
    free(outBuffer);
}


void RmmVtReqHandler::sendMsgToVTS(char* outBuffer, int length, const char* user) {
    sp<RmmVtSharedMemory> Mem  = RmmVtDataThreadController::getSharedMem();

    while (1) {
        if (!Mem->checkState(VT_RIL_SHARE_DATA_STATUS_RECV_NONE)) {
            // wait VT RIL thread finish sending data to vt service
            usleep(200);

        } else {
            Mem->lock(user);
            break;
        }
    }

    Mem->setSlotId(m_slot_id);
    Mem->setSize(length);
    Mem->setData(outBuffer, length);

    Mem->setState(VT_RIL_SHARE_DATA_STATUS_RECV_DATA);

    Mem->signal(user);

    Mem->unlock(user);

}
