// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
 /*
  * rfx_diaptch_thread.cpp
  *
  *  Created on: 2015/8/04
  *  Author: MTK10602
  *
  */


#include <stdlib.h>
#include "RfxDispatchThread.h"
#include "RfxParcelTransfer.h"

/*************************************************************
 * RfxDispatchThread
 *************************************************************/
extern "C"
void *rfx_process_request_messages_loop(void *arg);
extern "C"
void *rfx_process_response_messages_loop(void *arg);
extern "C"
void *rfx_process_urc_messages_loop(void *arg);

sp<RfxDispatchThread> RfxDispatchThread::s_self = NULL;

RfxDispatchThread::RfxDispatchThread() {
    memset(&requestThreadId, 0, sizeof(pthread_t));
    memset(&responseThreadId, 0, sizeof(pthread_t));
    memset(&urcThreadId, 0, sizeof(pthread_t));
}

RfxDispatchThread::~RfxDispatchThread() {

}

#define LOG_TAG "RfxDispatchThread"

RfxDispatchThread* RfxDispatchThread::init() {
    RFX_LOG_D(LOG_TAG, "RfxDispatchThread init");
    s_self = new RfxDispatchThread();
    s_self->run("Ril Proxy request dispatch thread");
    return s_self.get();
}

bool RfxDispatchThread::threadLoop() {
    // init process message thread (request)
    pthread_attr_t reqAttr;
    PthreadPtr reqPptr = rfx_process_request_messages_loop;
    int result;
    pthread_attr_init(&reqAttr);
    pthread_attr_setdetachstate(&reqAttr, PTHREAD_CREATE_DETACHED);

    //Start request processing loop thread
    result = pthread_create(&requestThreadId, &reqAttr, reqPptr, this);
    if(result < 0) {
        RFX_LOG_D(LOG_TAG, "pthread_create failed with result:%d", result);
    }

    // init process message thread (response)
    pthread_attr_t resAttr;
    PthreadPtr resPptr = rfx_process_response_messages_loop;
    pthread_attr_init(&resAttr);
    pthread_attr_setdetachstate(&resAttr, PTHREAD_CREATE_DETACHED);

    //Start request processing loop thread
    result = pthread_create(&responseThreadId, &resAttr, resPptr, this);
    if(result < 0) {
        RFX_LOG_D(LOG_TAG, "pthread_create failed with result:%d", result);
    }

    // init process message thread (urc)
    pthread_attr_t urcAttr;
    PthreadPtr urcPptr = rfx_process_urc_messages_loop;
    pthread_attr_init(&urcAttr);
    pthread_attr_setdetachstate(&urcAttr, PTHREAD_CREATE_DETACHED);

    //Start request processing loop thread
    result = pthread_create(&urcThreadId, &urcAttr, urcPptr, this);
    if(result < 0) {
        RFX_LOG_D(LOG_TAG, "pthread_create failed with result:%d", result);
    }

    return false;
}

void RfxDispatchThread::enqueueRequestMessage(int request, void *data, size_t datalen,
        RIL_Token t, RIL_SOCKET_ID socket_id) {
    RequestInfo *requestInfo = (RequestInfo *)t;

    // get default target
    ///[MT2635] only support 5M at present
    RILD_RadioTechnology_Group dest = RADIO_TECH_GROUP_GSM;
    // get parcel (execute dataToParcel)
    Parcel *parcel = RfxTransferUtils::dataToParcel(request, requestInfo->token, data, datalen, t);

    sp<RfxMessage> msg = RfxMessage::obtainRequest(socket_id, dest, request, requestInfo->token,
            parcel, t);
    MessageObj *obj = createMessageObj(msg);

    dispatchRequestQueue.enqueue(obj);
}

void RfxDispatchThread::enqueueRequestMessageOld(int request, int slotId,
            RILD_RadioTechnology_Group dest, int token, void *data) {
    sp<RfxMessage> msg = RfxMessage::obtainRequest(slotId, dest, request, token, (Parcel*) data);
    // RFX_LOG_D(LOG_TAG, "enqueueRequestMessage(): pRequest = %d, pToken = %d, token = %d",
    //        msg->getPId(), msg->getPToken(), msg->getToken());
    MessageObj *obj = createMessageObj(msg);
    dispatchRequestQueue.enqueue(obj);
}

void RfxDispatchThread::enqueueRequestMessage(int request, int slotId,
            RILD_RadioTechnology_Group dest, int token, void *data, int clientId) {
    sp<RfxMessage> msg =
        RfxMessage::obtainRequest(slotId, dest, request, token, (Parcel*) data, clientId);
    RFX_LOG_D(LOG_TAG, "enqueueRequestMessage(): pRequest = %d, pToken = %d, token = %d, clientId = %d",
            msg->getPId(), msg->getPToken(), msg->getToken(), clientId);
    MessageObj *obj = createMessageObj(msg);
    dispatchRequestQueue.enqueue(obj);
}

void RfxDispatchThread::enqueueResponseMessage(RILD_RadioTechnology_Group source, int slotId,
        int token, int error, Parcel *parcel) {
    MessageObj *obj = pendingQueue.checkAndDequeue(token);
    if (obj == NULL) {
        RFX_LOG_W(LOG_TAG, "enqueueResponseMessage(): No correspending request!");
        return;
    }

#if (defined(TELEPHONYWARE) || defined(TELE_FWK))
    parcel->setDataPosition(sizeof(int32_t)*3);
    RFX_LOG_D(LOG_TAG, "enqueueResponseMessage() before transfer:%d", error);
    Parcel *temp = RfxParcelTransfer::RespParcelTransfer(obj->msg->getId(), parcel, &error);
    //delete parcel;
    if(NULL == temp){
        RFX_LOG_W(LOG_TAG, "RespParcelTransfer function not defined!");
        return;
    }
    RFX_LOG_D(LOG_TAG, "enqueueResponseMessage() after transfer:%d", error);
#endif

    sp<RfxMessage> message;
    // FIXME: can use the same obj?
    if (obj->msg->getClientId() != -1) {
    RFX_LOG_D(LOG_TAG, "enqueueResponseMessage() obtainResponse");
        message = RfxMessage::obtainResponse(slotId, obj->msg->getPId(),
                obj->msg->getPToken(), obj->msg->getId(), obj->msg->getToken(),
                (RIL_Errno)error, source, temp, obj->msg->getClientId(),
                obj->msg->getRilToken(), obj->msg->getPTimeStamp());
    } else {
        message = RfxMessage::obtainResponse(obj->msg->getSlotId(),
                obj->msg->getPId(), obj->msg->getPToken(), obj->msg->getId(),
                obj->msg->getToken(), (RIL_Errno)error, source, temp,
                obj->msg->getRilToken(), obj->msg->getTimeStamp());
    }

    RFX_LOG_D(LOG_TAG, "enqueueResponseMessage() obtainResponse after");

    MessageObj *dispatchObj = createMessageObj(message);
    RFX_LOG_D(LOG_TAG, "enqueueResponseMessage() createMessageObj after");
    dispatchResponseQueue.enqueue(dispatchObj);
    delete(obj);
}

void RfxDispatchThread::enqueueUrcMessage(RILD_RadioTechnology_Group source, int slotId,
        int urcId, Parcel *parcel) {

#if (defined(TELEPHONYWARE) || defined(TELE_FWK))
        Parcel *temp = RfxParcelTransfer::UnsolParcelTransfer(&urcId, parcel);
        //delete parcel;
        if(NULL == temp){
            RFX_LOG_D(LOG_TAG, "UnsolParcelTransfer function not defined!");
            return;
        }
#endif
    sp<RfxMessage> message = RfxMessage::obtainUrc(slotId, urcId, source, temp);
    MessageObj *obj = createMessageObj(message);

    dispatchUrcQueue.enqueue(obj);
}

void RfxDispatchThread::addMessageToPendingQueue(const sp<RfxMessage>& message) {
    RFX_LOG_D(LOG_TAG, "addMessageToPendingQueue pRequest = %d, pToken = %d, token = %d",
            message->getPId(), message->getPToken(), message->getToken());
    MessageObj *obj = createMessageObj(message);
    pendingQueue.enqueue(obj);
}

void RfxDispatchThread::processRequestMessageLooper() {

    MessageObj *obj = dispatchRequestQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete(obj);
}

void RfxDispatchThread::processResponseMessageLooper() {

    MessageObj *obj = dispatchResponseQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete(obj);
}

void RfxDispatchThread::processUrcMessageLooper() {

    MessageObj *obj = dispatchUrcQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete(obj);
}

void RfxDispatchThread::clearPendingQueue() {
    RFX_LOG_D(LOG_TAG, "clearPendingQueue start");
    while (pendingQueue.empty() == 0) {
        MessageObj *obj = pendingQueue.dequeue();
        sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_E_RADIO_NOT_AVAILABLE, obj->msg);
        RfxRilAdapter *adapter = RfxRilAdapter::getInstance();
        if (adapter != NULL) {
            adapter->responseToRilj(response);
        }
        delete(obj);
    }
    RFX_LOG_D(LOG_TAG, "clearPendingQueue end");
}

MessageObj* createMessageObj(const sp<RfxMessage>&
        message) {
    MessageObj *obj = new MessageObj();
    obj->msg = message;
    obj->p_next = NULL;
    return obj;
}

extern "C"
void *rfx_process_request_messages_loop(void *arg) {
    RFX_LOG_D(LOG_TAG, "rfx_process_request_messages_loop");
    while (1) {
        RfxDispatchThread *dispatchThread = (RfxDispatchThread *) arg;
        dispatchThread->processRequestMessageLooper();
    }
    RFX_LOG_D(LOG_TAG, "rfx_process_request_messages_loop close");
    return NULL;
}

extern "C"
void *rfx_process_response_messages_loop(void *arg) {
    RFX_LOG_D(LOG_TAG, "rfx_process_response_messages_loop");
    while (1) {
        RfxDispatchThread *dispatchThread = (RfxDispatchThread *) arg;
        dispatchThread->processResponseMessageLooper();
    }
    RFX_LOG_D(LOG_TAG, "rfx_process_response_messages_loop close");
    return NULL;
}

extern "C"
void *rfx_process_urc_messages_loop(void *arg) {
    RFX_LOG_D(LOG_TAG, "rfx_process_urc_messages_loop");
    while (1) {
        RfxDispatchThread *dispatchThread = (RfxDispatchThread *) arg;
        dispatchThread->processUrcMessageLooper();
    }
    RFX_LOG_D(LOG_TAG, "rfx_process_urc_messages_loop close");
    return NULL;
}
