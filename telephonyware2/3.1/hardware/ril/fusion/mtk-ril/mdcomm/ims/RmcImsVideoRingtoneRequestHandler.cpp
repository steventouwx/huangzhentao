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

#include "RmcImsVideoRingtoneRequestHandler.h"
#include "rfx_properties.h"
#include <telephony/mtk_ril.h>
#include <string>
#include <string.h>
#include "ConfigUtil.h"

static const int requests[] = {
    RFX_MSG_REQUEST_VIDEO_RINGTONE_EVENT,    // AT+EUISINFO
};

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_VIDEO_RINGTONE_EVENT);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcImsVideoRingtoneRequestHandler, RIL_CMD_PROXY_2);

RmcImsVideoRingtoneRequestHandler::RmcImsVideoRingtoneRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmcImsVideoRingtoneRequestHandler constructor");
    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
}

RmcImsVideoRingtoneRequestHandler::~RmcImsVideoRingtoneRequestHandler() {
}

void RmcImsVideoRingtoneRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    //logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_VIDEO_RINGTONE_EVENT:
            sendVideoRingtoneEventRequest(msg);
            break;

        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcImsVideoRingtoneRequestHandler::sendVideoRingtoneEventRequest(const sp<RfxMclMessage>& msg) {
    /* AT+EUISINFO=<call id>,<type>,<data>[,<duration>]
     * <call id>: call id
     * <type>: 1 = button clicked; 2 = coordinates touched; 3 = move
     * <event>: video ringtone event, (button id, cordinates, or start/end cordinates)
     * <duration>: coordinates touched duration
     */
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    int minLen = 3;
    int length = msg->getData()->getDataLength();
    if (length < (minLen * sizeof(char *))) {
        logD(RFX_LOG_TAG, "msg params length is error, actual is %d" , length);
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
        return;
    }
    char** params = (char**)msg->getData()->getData();

    if (params[0] == NULL || strlen(params[0]) != 1) {
        logE(RFX_LOG_TAG, "callId wrong");
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
        return;
    }
    char* callId = params[0];

    if (params[1] == NULL || strlen(params[1]) > 3) {
        //type scope: {1,2,3,150}
        logE(RFX_LOG_TAG, "type wrong");
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
        return;
    }
    char* type = params[1];

    if (params[2] == NULL || strlen(params[2]) > 100) {
        //event string should not more than 100 characters
        logE(RFX_LOG_TAG, "event wrong");
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
        return;
    }
    char* event = params[2];
    char* duration = NULL;
    String8 cmd;

    if (atoi(type) == 2) {
        // 2 means touch, add duration
        duration = params[3];
    }
    logD(RFX_LOG_TAG, "sendVideoRingtoneEventRequest callId:%s, op:%s, event:%s", callId, type, event);

    // send AT command
    if (duration != NULL && strlen(duration) > 0) {
        cmd = String8::format("AT+EUISINFO=%s,%s,\"%s\",%s", callId, type, event, duration);
    } else {
        cmd = String8::format("AT+EUISINFO=%s,%s,\"%s\"", callId, type, event);
    }

    sp<RfxAtResponse> p_response = atSendCommand(cmd);

    if (p_response != NULL) {
        if (p_response->getSuccess() != 1) {
            rilErrNo = RIL_E_GENERIC_FAILURE;
        }
        p_response = NULL;
    }
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}
