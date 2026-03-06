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

#include "RmmSuppServRequestHandler.h"
#include "RfxMessageId.h"
//#include "GsmUtil.h"
#include "SSUtil.h"
//#include "SSConfig.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include "rfx_properties.h"
#include "RfxBarringCallInfosData.h"

#include <compiler/compiler_utils.h>
#include <dlfcn.h>
#include <libmtkrilutils.h>
#include <string.h>
#include <telephony/mtk_ril.h>


RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData,  RfxVoidData, RFX_MSG_REQUEST_SEND_USSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData,    RfxVoidData, RFX_MSG_REQUEST_CANCEL_USSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData,    RfxIntsData, RFX_MSG_REQUEST_GET_CLIR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData,    RfxVoidData, RFX_MSG_REQUEST_SET_CLIR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCallForwardInfoData, RfxCallForwardInfosData, RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCallForwardInfoData, RfxVoidData, RFX_MSG_REQUEST_SET_CALL_FORWARD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCallForwardInfoExData, RfxCallForwardInfosExData, RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCallForwardInfoExData, RfxVoidData, RFX_MSG_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_QUERY_CALL_WAITING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData,    RfxVoidData, RFX_MSG_REQUEST_SET_CALL_WAITING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_QUERY_CALL_BARRING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_SET_CALL_BARRING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_CHANGE_BARRING_PASSWORD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData,    RfxIntsData, RFX_MSG_REQUEST_QUERY_CLIP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData,    RfxVoidData, RFX_MSG_REQUEST_SET_CLIP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData,    RfxVoidData, RFX_MSG_REQUEST_SET_SUPP_SVC_NOTIFICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData,    RfxIntsData, RFX_MSG_REQUEST_GET_COLP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData,    RfxVoidData, RFX_MSG_REQUEST_SET_COLP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData,    RfxIntsData, RFX_MSG_REQUEST_GET_COLR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData,    RfxVoidData, RFX_MSG_REQUEST_SET_COLR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData,    RfxIntsData, RFX_MSG_REQUEST_SEND_CNAP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData,  RfxVoidData, RFX_MSG_REQUEST_SEND_USSI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData,    RfxVoidData, RFX_MSG_REQUEST_CANCEL_USSI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SETUP_XCAP_USER_AGENT_STRING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxBarringCallInfosData, RFX_MSG_REQUEST_GET_BARRING_SEPCIFIC_CALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxBarringCallInfosData, RfxVoidData, RFX_MSG_REQUEST_SET_BARRING_SEPCIFIC_CALL);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringsData, RFX_MSG_EVENT_SET_XCAP_CONFIG);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmSuppServRequestHandler, RIL_CMD_PROXY_6);


static const int requests[] = {
    RFX_MSG_REQUEST_SEND_USSD,
    RFX_MSG_REQUEST_CANCEL_USSD,
    RFX_MSG_REQUEST_SEND_USSI,
    RFX_MSG_REQUEST_CANCEL_USSI,
    RFX_MSG_REQUEST_GET_CLIR,
    RFX_MSG_REQUEST_SET_CLIR,
    RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS,
    RFX_MSG_REQUEST_SET_CALL_FORWARD,
    RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT,
    RFX_MSG_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT,
    RFX_MSG_REQUEST_QUERY_CALL_WAITING,
    RFX_MSG_REQUEST_SET_CALL_WAITING,
    RFX_MSG_REQUEST_QUERY_CALL_BARRING,
    RFX_MSG_REQUEST_SET_CALL_BARRING,
    RFX_MSG_REQUEST_CHANGE_BARRING_PASSWORD,
    RFX_MSG_REQUEST_QUERY_CLIP,
    RFX_MSG_REQUEST_SET_CLIP,
    RFX_MSG_REQUEST_GET_COLP,
    RFX_MSG_REQUEST_SET_COLP,
    RFX_MSG_REQUEST_GET_COLR,
    RFX_MSG_REQUEST_SET_COLR,
    RFX_MSG_REQUEST_SEND_CNAP,
    RFX_MSG_REQUEST_SET_SUPP_SVC_NOTIFICATION,
    RFX_MSG_REQUEST_SETUP_XCAP_USER_AGENT_STRING,
    RFX_MSG_REQUEST_GET_BARRING_SEPCIFIC_CALL,
    RFX_MSG_REQUEST_SET_BARRING_SEPCIFIC_CALL
};

static const int event[] = {
    RFX_MSG_EVENT_SET_XCAP_CONFIG
};


RmmSuppServRequestHandler::RmmSuppServRequestHandler(int slot_id, int channel_id) :
    RmmSuppServRequestBaseHandler(slot_id, channel_id) {

    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
    registerToHandleEvent(event, sizeof(event)/sizeof(int));

    // always set 1, EM will use it.
    rfx_property_set(PROPERTY_MODEM_VERSION, "1");
}

RmmSuppServRequestHandler::~RmmSuppServRequestHandler() {
}

void RmmSuppServRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int requestId = msg->getId();
    int ret = RIL_E_SUCCESS;
    sp<RfxMclMessage> resMsg = NULL;

    logD(TAG, "onHandleRequest: %d", requestId);

    switch(requestId) {
        case RFX_MSG_REQUEST_SEND_USSD:
            // fall through
            TELEPHONYWARE_FALLTHROUGH;

        case RFX_MSG_REQUEST_SEND_USSI:
            ret = requestSendUssd(msg);
            break;

        case RFX_MSG_REQUEST_CANCEL_USSD:
            // fall through
            TELEPHONYWARE_FALLTHROUGH;

        case RFX_MSG_REQUEST_CANCEL_USSI:
            ret = requestCancelUssd(msg);
            break;

        case RFX_MSG_REQUEST_GET_CLIR:
            ret = requestGetClir(msg);
            if (ret != RIL_E_SUCCESS) {
                int results[2] = {0};
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(results, sizeof(results) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_SET_CLIR:
            ret = requestSetClir(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS:
            ret = requestQueryCallForwardStatus(msg);
            break;

        case RFX_MSG_REQUEST_SET_CALL_FORWARD:
            ret = requestSetCallForward(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT:
            ret = requestQueryCallForwardInTimeSlotStatus(msg);
            break;

        case RFX_MSG_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT:
            ret = requestSetCallForwardInTimeSlot(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_WAITING:
            ret = requestQueryCallWaiting(msg);
            if (ret != RIL_E_SUCCESS) {
                int results[2] = {0};
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(results, sizeof(results) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_SET_CALL_WAITING:
            ret = requestSetCallWaiting(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_BARRING:
            ret = requestQueryCallBarring(msg);
            if (ret != RIL_E_SUCCESS) {
                int result = 0;
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(&result, sizeof(result) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_SET_CALL_BARRING:
            ret = requestSetCallBarring(msg);
            if (ret != RIL_E_SUCCESS) {
                int result = 0;
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(&result, sizeof(result) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_CHANGE_BARRING_PASSWORD:
            ret = requestChangeBarringPassword(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_CLIP:
            ret = requestQueryClip(msg);
            if (ret != RIL_E_SUCCESS) {
                int result = 0;
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(&result, sizeof(result) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_SET_CLIP:
            ret = requestSetClip(msg);
            break;

        case RFX_MSG_REQUEST_GET_COLP:
            ret = requestGetColp(msg);
            if (ret != RIL_E_SUCCESS) {
                int results[2] = {0};
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(results, sizeof(results) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_SET_COLP:
            ret = requestSetColp(msg);
            break;

        case RFX_MSG_REQUEST_GET_COLR:
            ret = requestGetColr(msg);
            if (ret != RIL_E_SUCCESS) {
                int result = 0;
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(&result, sizeof(result) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_SET_COLR:
            ret = requestSetColr(msg);
            break;

        case RFX_MSG_REQUEST_SEND_CNAP:
            ret = requestSendCnap(msg);
            if (ret != RIL_E_SUCCESS) {
                int results[2] = {0};
                resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                        RfxIntsData(results, sizeof(results) / sizeof(int)), msg, false);
            }
            break;

        case RFX_MSG_REQUEST_SET_SUPP_SVC_NOTIFICATION:
            ret = requestSetSuppSvcNotification(msg);
            break;

        case RFX_MSG_REQUEST_SETUP_XCAP_USER_AGENT_STRING:
            ret = requestSetupXcapUserAgentString(msg);
            break;

        case RFX_MSG_REQUEST_GET_BARRING_SEPCIFIC_CALL:
            ret = requestGetBarringCalls(msg);
            break;

        case RFX_MSG_REQUEST_SET_BARRING_SEPCIFIC_CALL:
            ret = requestSetBarringCalls(msg);
            break;

        default:
            logE(TAG, "Should not be here");
            break;
    }

    // Error occurs when sending.
    if (ret != RIL_E_SUCCESS) {
        if (resMsg == NULL) {
            resMsg = RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno) ret,
                    RfxVoidData(), msg, false);
        }
        responseToTelCore(resMsg);
    }
}

void RmmSuppServRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    switch(id) {
        case RFX_MSG_EVENT_SET_XCAP_CONFIG:
            requestSetXcapConfig(msg);
            break;
        default:
            logE(TAG, "should not be here");
            break;
    }
}

void RmmSuppServRequestHandler::onHandleTimer() {
    // do something
}

int RmmSuppServRequestHandler::requestGetClir(const sp<RfxMclMessage>& msg) {
    return requestClirOperation(msg);
}

int RmmSuppServRequestHandler::requestSetClir(const sp<RfxMclMessage>& msg) {
    return requestClirOperation(msg);
}

int RmmSuppServRequestHandler::requestQueryCallForwardStatus(const sp<RfxMclMessage>& msg) {
    return requestCallForwardOperation(msg, CCFC_E_QUERY);
}

int RmmSuppServRequestHandler::requestSetCallForward(const sp<RfxMclMessage>& msg) {
    return requestCallForwardOperation(msg, CCFC_E_SET);
}

int RmmSuppServRequestHandler::requestQueryCallForwardInTimeSlotStatus(const sp<RfxMclMessage>& msg) {
    return requestCallForwardExOperation(msg, CCFC_E_QUERY);
}

int RmmSuppServRequestHandler::requestSetCallForwardInTimeSlot(const sp<RfxMclMessage>& msg) {
    return requestCallForwardExOperation(msg, CCFC_E_SET);
}

int RmmSuppServRequestHandler::requestQueryCallWaiting(const sp<RfxMclMessage>& msg) {
    return requestCallWaitingOperation(msg, CW_E_QUERY);
}

int RmmSuppServRequestHandler::requestSetCallWaiting(const sp<RfxMclMessage>& msg) {
    return requestCallWaitingOperation(msg, CW_E_SET);
}

int RmmSuppServRequestHandler::requestQueryCallBarring(const sp<RfxMclMessage>& msg) {
    return requestCallBarring(msg, CB_E_QUERY);
}

int RmmSuppServRequestHandler::requestSetCallBarring(const sp<RfxMclMessage>& msg) {
    return requestCallBarring(msg, CB_E_SET);
}

int RmmSuppServRequestHandler::requestGetColp(const sp<RfxMclMessage>& msg) {
    return requestColpOperation(msg);
}

int RmmSuppServRequestHandler::requestSetColp(const sp<RfxMclMessage>& msg) {
    return requestColpOperation(msg);
}

int RmmSuppServRequestHandler::requestGetColr(const sp<RfxMclMessage>& msg) {
    return requestColrOperation(msg);
}

int RmmSuppServRequestHandler::requestSetColr(const sp<RfxMclMessage>& msg) {
    return requestColrOperation(msg);
}

