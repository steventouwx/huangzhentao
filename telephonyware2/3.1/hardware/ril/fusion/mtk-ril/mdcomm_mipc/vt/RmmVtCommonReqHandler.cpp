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

#include "RmmVtCommonReqHandler.h"

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
#include <prop/properties.h>
#include <cutils/sockets.h>
#include <sys/socket.h>
#include <sys/un.h>

#define RFX_LOG_TAG "VT RIL CMN RMM"

/*****************************************************************************
 * Class RmmVtCommonReqHandler
 *****************************************************************************/

RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_REPORT_ANBR);

RFX_IMPLEMENT_HANDLER_CLASS(RmmVtCommonReqHandler, RIL_CMD_PROXY_2);

RmmVtCommonReqHandler::RmmVtCommonReqHandler(int slot_id, int channel_id)
: RfxBaseHandler(slot_id, channel_id) {

    logI(RFX_LOG_TAG, "[RMM VT REQ HDLR] RmmVtCommonReqHandler create (slot_id = %d)", slot_id);

    const int CommonEventList[] = {
        RFX_MSG_EVENT_REPORT_ANBR,
    };

    registerToHandleEvent(CommonEventList, sizeof(CommonEventList) / sizeof(int));

    const int requestList[] = {
        RFX_MSG_REQUEST_ENABLE_ANBR,
    };

    registerToHandleRequest(requestList, sizeof(requestList)/sizeof(int));
}

RmmVtCommonReqHandler::~RmmVtCommonReqHandler() {
}

void RmmVtCommonReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {

    int request = msg->getId();

    logI(RFX_LOG_TAG, "[Handle REQ] request = %d", request);

    switch (request) {
        case RFX_MSG_REQUEST_ENABLE_ANBR:
            logI(RFX_LOG_TAG, "[Handle REQ] RFX_MSG_REQUEST_ENABLE_ANBR");
            handleEnableANBR(msg);
            break;
        default:
            logI(RFX_LOG_TAG, "[Handle REQ] unknown request, ignore!");
            break;
    }
}

void RmmVtCommonReqHandler::onHandleEvent(const sp<RfxMclMessage> & msg) {

    int event = msg->getId();

    logI(RFX_LOG_TAG, "[Handle EVT] event = %d", event);

    switch (event) {
        case RFX_MSG_EVENT_REPORT_ANBR:
            logI(RFX_LOG_TAG, "[Handle EVT] RFX_MSG_EVENT_REPORT_ANBR");
            handleReportANBR(msg);
            break;

        default:
            logI(RFX_LOG_TAG, "[Handle EVT] unknown event, ignore!");
            break;
    }
}

void RmmVtCommonReqHandler::handleReportANBR(const sp<RfxMclMessage>& msg) {
    RFX_LOG_I(RFX_LOG_TAG, "handleReportANBR");


    RFX_LOG_I(RFX_LOG_TAG, "[handleReportANBR] let's begin");

    if (msg == NULL || msg->getData() == NULL ) {
        RFX_LOG_I(RFX_LOG_TAG, "[handleReportANBR] invalid");
        return;
    }

    int *pInt = (int *)msg->getData()->getData();
    if (pInt == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[handleReportANBR] pInt is null");
        return;
    }

    int is_ul = pInt[0];
    int ebi = pInt[1];
    int bitrate = pInt[2];
    int bearer_id = pInt[3];
    int pdu_session_id = pInt[4];
    int ext_param = pInt[5];; //for future usage

    logI(RFX_LOG_TAG, "[handleReportANBR] is_ul = %d, ebi = %d, bitrate = %d, bearer_id = %d, pdu_session_id = %d, ext_param = %d\n",
            is_ul, ebi, bitrate, bearer_id, pdu_session_id, ext_param);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_NW_SET_REPORT_ANBR_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_MODE, 2);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_EBI, ebi);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_IS_UL, is_ul);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_BEARE_ID, bearer_id);
    data->addMipcTlvUint16(MIPC_NW_SET_REPORT_ANBR_REQ_T_BITRATE, bitrate);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_PDU_SESSION_ID, pdu_session_id);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_EXT_PARAM, ext_param);
    logD(RFX_LOG_TAG, "MIPC_NW_SET_REPORT_ANBR_REQ");
    callToMipcMsgAsync(msg, data);
}

void RmmVtCommonReqHandler::handleEnableANBR(const sp<RfxMclMessage>& msg) {

    int *pInt = (int *)msg->getData()->getData();
    int enable = pInt[0];

    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    logI(RFX_LOG_TAG, "[handleEnableANBR] enable = %d\n", enable);

/*
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_NW_SET_REPORT_ANBR_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_MODE,  enable);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_EBI,  0);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_IS_UL,  0);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_BEARE_ID,  0);
    data->addMipcTlvUint16(MIPC_NW_SET_REPORT_ANBR_REQ_T_BITRATE,  0);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_PDU_SESSION_ID,  0);
    data->addMipcTlvUint8(MIPC_NW_SET_REPORT_ANBR_REQ_T_EXT_PARAM,  0);
    logD(RFX_LOG_TAG, "MIPC_NW_SET_REPORT_ANBR_REQ");
    callToMipcMsgAsync(msg, data);
*/
    // response to TeleCore
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);

}
