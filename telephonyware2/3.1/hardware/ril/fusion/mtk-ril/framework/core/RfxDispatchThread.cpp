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
#include "RfxMclStatusManager.h"
#include "RfxDispatchThread.h"
#include "RfxIdToMsgIdUtils.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "libmtkrilutils.h"

#include "RfxMclStatusManager.h"

#ifdef MTK_MBRAIN_SUPPORT
#include <aidl/vendor/mediatek/hardware/mbrain/IMBrain.h>
#include <aidl/vendor/mediatek/hardware/mbrain/BnMBrainCallbacks.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/IServiceManager.h>
#include <json/json.h>

using namespace Json;
using ::aidl::vendor::mediatek::hardware::mbrain::IMBrain;
using ::aidl::vendor::mediatek::hardware::mbrain::MBrain_Parcelable;
using ::aidl::vendor::mediatek::hardware::mbrain::MBrain_Event;
#endif

/*************************************************************
 * RfxDispatchThread
 *************************************************************/
extern "C"
void *rfx_process_request_messages_loop(void *arg);
extern "C"
void *rfx_process_response_messages_loop(void *arg);
extern "C"
void *rfx_process_urc_messages_loop(void *arg);
extern "C"
void *rfx_process_status_sync_messages_loop(void *arg);

sp<RfxDispatchThread> RfxDispatchThread::s_self = NULL;

RfxDispatchThread::RfxDispatchThread() {
    memset(&requestThreadId, 0, sizeof(pthread_t));
    memset(&responseThreadId, 0, sizeof(pthread_t));
    memset(&urcThreadId, 0, sizeof(pthread_t));
    memset(&statusSyncThreadId, 0, sizeof(pthread_t));
}

RfxDispatchThread::~RfxDispatchThread() {

}

#define RFX_LOG_TAG "RfxDisThread"

sp<RfxDispatchThread> RfxDispatchThread::init() {
    RFX_LOG_D(RFX_LOG_TAG, "RfxDispatchThread init");
    s_self = new RfxDispatchThread();
    s_self->run("Ril Proxy request dispatch thread");
    return s_self;
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
        RFX_LOG_E(RFX_LOG_TAG, "pthread_create request failed with result:%d", result);
    }

    // init process message thread (response)
    pthread_attr_t resAttr;
    PthreadPtr resPptr = rfx_process_response_messages_loop;
    pthread_attr_init(&resAttr);
    pthread_attr_setdetachstate(&resAttr, PTHREAD_CREATE_DETACHED);

    //Start request processing loop thread
    result = pthread_create(&responseThreadId, &resAttr, resPptr, this);
    if(result < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "pthread_create response failed with result:%d", result);
    }

    // init process message thread (urc)
    pthread_attr_t urcAttr;
    PthreadPtr urcPptr = rfx_process_urc_messages_loop;
    pthread_attr_init(&urcAttr);
    pthread_attr_setdetachstate(&urcAttr, PTHREAD_CREATE_DETACHED);

    //Start request processing loop thread
    result = pthread_create(&urcThreadId, &urcAttr, urcPptr, this);
    if(result < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "pthread_create urc failed with result:%d", result);
    }

    // init process message (update status)
    pthread_attr_t statusSyncAttr;
    PthreadPtr statusSyncPptr = rfx_process_status_sync_messages_loop;
    pthread_attr_init(&statusSyncAttr);
    pthread_attr_setdetachstate(&statusSyncAttr, PTHREAD_CREATE_DETACHED);

    //Start request processing loop thread
    result = pthread_create(&statusSyncThreadId, &statusSyncAttr, statusSyncPptr, this);
    if(result < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "pthread_create status failed with result:%d", result);
    }

    return false;
}

void RfxDispatchThread::enqueueRequestMessage(int request, void *data, size_t datalen,
        RIL_Token t, RIL_SOCKET_ID socket_id, int clientId) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueRequestMessage: request: %d", request);
    RfxRequestInfo *requestInfo = (RfxRequestInfo *)t;

    // use msgId to create RfxMessage
    int msgId = request;
    if (RFX_MESSAGE_ID_BEGIN >= request) {
        msgId = RfxIdToMsgIdUtils::idToMsgId(request);
    }
    if (INVALID_ID == msgId) {
        // send NOT_SUPPORT to RILJ
        sp<RfxMessage> resMsg = RfxMessage::obtainResponse(socket_id, request, requestInfo->token,
                INVALID_ID, -1, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj *obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
    } else {
        sp<RfxMessage> msg = RfxMessage::obtainRequest(socket_id, msgId,
                requestInfo->token, data, datalen, t, clientId);
        MessageObj *obj = createMessageObj(msg);
        dispatchRequestQueue.enqueue(obj);
    }
}

void RfxDispatchThread::enqueueSapRequestMessage(int request, void *data, size_t datalen,
        RIL_Token t, RIL_SOCKET_ID socket_id) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueSapRequestMessage: request: %d", request);
    RfxSapSocketRequest *sapRequest = (RfxSapSocketRequest *)t;

    // use msgId to create RfxMessage
    int msgId = RfxIdToMsgIdUtils::sapIdToMsgId(request);
    if (INVALID_ID == msgId) {
        // send NOT_SUPPORT to BT process
        sp<RfxMessage> resMsg = RfxMessage::obtainSapResponse(socket_id, request, sapRequest->token,
                INVALID_ID, -1, (RIL_Errno) 3 /* SAP error number is different with RIL. 3 is not_support */,
                NULL, 0, t);
        MessageObj *obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
    } else {
        sp<RfxMessage> msg = RfxMessage::obtainSapRequest(socket_id, msgId, sapRequest->token,
                data, datalen, t);
        MessageObj *obj = createMessageObj(msg);
        dispatchRequestQueue.enqueue(obj);
    }
}

typedef int (*MBNotifyRILDResponseTimeout)(int slotId, int requestId, const char* extraInfo);

void RfxDispatchThread::enqueueResponseMessage(const sp<RfxMclMessage>& msg) {
    void *hLibHandle;
    MBNotifyRILDResponseTimeout notifyRILDResponseTimeout;

    MessageObj *obj = pendingQueue.checkAndDequeue(msg->getToken());
    if (obj == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "enqueueResponseMessage(): No correspending request!");
        return;
    }

    MessageObj *frontObj = NULL;
    // user debug load to check and fail to trigger assert
    // user load to add mbrain
    if ((frontObj = pendingQueue.getFront()) != NULL) {
        if (frontObj->msg != NULL) {
            sp<RfxMessage> frontMsg = frontObj->msg;
            nsecs_t time;
            time = systemTime(SYSTEM_TIME_MONOTONIC) - frontMsg->getPTimeStamp();
            if (time > 0) {
                long timeSeconds = ns2s(time);
                // 20 minutes time out, 1200 = 20 * 60
                if (timeSeconds > 1200) {
                    bool mdOff = RfxMclStatusManager::getNonSlotMclStatusManager()->getBoolValue(
                            RFX_STATUS_KEY_MODEM_POWER_OFF, false);
                    RFX_LOG_E(RFX_LOG_TAG, "enqueueResponseMessage():time out seconds(%d)(%d)(msg = %s)",
                            timeSeconds, mdOff, frontMsg->toString().string());

#ifdef MTK_MBRAIN_SUPPORT
                    static const std::string serviceName = std::string() + (IMBrain::descriptor ? IMBrain::descriptor : "") + "/default";
                    int ret = 0;
                    shared_ptr<IMBrain> ptr =  IMBrain::fromBinder(ndk::SpAIBinder(AServiceManager_checkService(serviceName.c_str())));
                    if (ptr != nullptr) {
                        ptr->IsMBrainSupport(&ret);
                        if (ret == 1) {
                            MBrain_Parcelable mbrain_parcelable_input;
                            MBrain_Event mbEvent = MBrain_Event::MB_TELEPHONY_RILD_EVENT_NOTIFY;
                            mbrain_parcelable_input.eventHint = 0;

                            Json::Value root = Value(objectValue);
                            Json::Value rildResponseTimeout = Value(objectValue);
                            struct timespec spec;
                            if (clock_gettime(CLOCK_REALTIME, &spec) == -1) {
                                rildResponseTimeout["timestamp"] = 0;
                            } else {
                                rildResponseTimeout["timestamp"] = spec.tv_sec * 1000 + spec.tv_nsec / 1e6;
                            }
                            rildResponseTimeout["slotId"] = frontMsg->getSlotId();
                            rildResponseTimeout["requestId"] = frontMsg->getId();
                            rildResponseTimeout["extraInfo"] = frontMsg->toString().string();
                            root["rildResponseTimeout"] = rildResponseTimeout;

                            Json::StreamWriterBuilder builder;
                            std::string result = Json::writeString(builder, root);
                            mbrain_parcelable_input.privData = String8(result.c_str());
                            ndk::ScopedAStatus status = ptr->Notify(mbEvent, mbrain_parcelable_input, &ret);
                            if (!status.isOk()) {
                                RFX_LOG_E(RFX_LOG_TAG, "enqueueResponseMessage() binder MBrain fail");
                            }
                        } else {
                            RFX_LOG_E(RFX_LOG_TAG, "enqueueResponseMessage() MBrain not support");
                        }
                    } else {
                        RFX_LOG_E(RFX_LOG_TAG, "enqueueResponseMessage() get MBrain server fail");
                    }
#endif

                    if (false == mdOff && isUserDebugLoad() == 1) {
                        pid_t pid = getpid();
                        char* str = NULL;
                        int length = asprintf(&str, "%d", pid);
                        if ((length != -1) && (str != NULL)) {
                            rfx_property_set("vendor.ril.mux.report.case", str);
                            rfx_property_set("vendor.ril.muxreport", "1");
                            free(str);
                        }
                        RFX_ASSERT(0);
                    }
                } // timeSeconds > 1200
            } // time > 0
        } // frontObj->msg != NULL
        delete(frontObj);
    }

    sp<RfxMessage> message = RfxMessage::obtainResponse(msg->getSlotId(), obj->msg->getPId(),
            obj->msg->getPToken(), obj->msg->getId(), obj->msg->getToken(), msg->getError(),
            msg->getData(), obj->msg->getPTimeStamp(), obj->msg->getRilToken(),
            obj->msg->getClientId());
    MessageObj *dispatchObj = createMessageObj(message);

    dispatchResponseQueue.enqueue(dispatchObj);
    delete(obj);
}

void RfxDispatchThread::enqueueSapResponseMessage(const sp<RfxMclMessage>& msg) {
    MessageObj *obj = pendingQueue.checkAndDequeue(msg->getToken());
    if (obj == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "enqueueSapResponseMessage(): No correspending request!");
        return;
    }

    sp<RfxMessage> message = RfxMessage::obtainSapResponse(msg->getSlotId(), obj->msg->getPId(),
            obj->msg->getPToken(), obj->msg->getId(), obj->msg->getToken(), msg->getError(),
            msg->getData(), obj->msg->getPTimeStamp(), obj->msg->getRilToken());
    MessageObj *dispatchObj = createMessageObj(message);

    dispatchResponseQueue.enqueue(dispatchObj);
    delete(obj);
}

void RfxDispatchThread::enqueueUrcMessage(const sp<RfxMclMessage>& msg) {

    sp<RfxMessage> message = RfxMessage::obtainUrc(msg->getSlotId(), msg->getId(),
            *(msg->getData()));
    MessageObj *obj = createMessageObj(message);

    dispatchUrcQueue.enqueue(obj);
}

void RfxDispatchThread::enqueueSapUrcMessage(const sp<RfxMclMessage>& msg) {

    sp<RfxMessage> message = RfxMessage::obtainSapUrc(msg->getSlotId(), msg->getId(),
            *(msg->getData()));
    MessageObj *obj = createMessageObj(message);

    dispatchResponseQueue.enqueue(obj);
}

void RfxDispatchThread::enqueueStatusSyncMessage(const sp<RfxMclMessage>& msg) {

    sp<RfxMessage> message = RfxMessage::obtainStatusSync(msg->getSlotId(), msg->getStatusKey(),
            msg->getStatusValue(), msg->getForceNotify(), msg->getIsDefault(),
            msg->getIsUpdateForMock());
    MessageObj *obj = createMessageObj(message);

    dispatchStatusSyncQueue.enqueue(obj);
}

void RfxDispatchThread::updateConnectionState(RIL_SOCKET_ID socket_id, int isConnected, unsigned int clientType) {
    if (clientType == 0) {  // client type is RADIO_AOSP
        sp<RfxMessage> message = RfxMessage::obtainStatusSync(socket_id, RFX_STATUS_CONNECTION_STATE,
                RfxVariant((isConnected ? true : false)), true, false, false);
        MessageObj *obj = createMessageObj(message);
        dispatchStatusSyncQueue.enqueue(obj);
    } else if (clientType == 1) {  // client type is RADIO_AOSP
        sp<RfxMessage> message = RfxMessage::obtainStatusSync(socket_id, RFX_STATUS_MTK_RADIO_CONNECTION_STATE,
                RfxVariant((isConnected ? true : false)), true, false, false);
        MessageObj *obj = createMessageObj(message);
        dispatchStatusSyncQueue.enqueue(obj);
    }
}

void RfxDispatchThread::addMessageToPendingQueue(const sp<RfxMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "addMessageToPendingQueue pRequest = %d, pToken = %d, token = %d",
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

void RfxDispatchThread::processStatusSyncMessageLooper() {

    MessageObj *obj = dispatchStatusSyncQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete(obj);
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
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_request_messages_loop");
    while (1) {
        RfxDispatchThread *dispatchThread = (RfxDispatchThread *) arg;
        dispatchThread->processRequestMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_request_messages_loop close");
    return NULL;
}

extern "C"
void *rfx_process_response_messages_loop(void *arg) {
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_response_messages_loop");
    while (1) {
        RfxDispatchThread *dispatchThread = (RfxDispatchThread *) arg;
        dispatchThread->processResponseMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_response_messages_loop close");
    return NULL;
}

extern "C"
void *rfx_process_urc_messages_loop(void *arg) {
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_urc_messages_loop");
    while (1) {
        RfxDispatchThread *dispatchThread = (RfxDispatchThread *) arg;
        dispatchThread->processUrcMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_urc_messages_loop close");
    return NULL;
}

extern "C"
void *rfx_process_status_sync_messages_loop(void *arg) {
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_status_sync_messages_loop");
    while (1) {
        RfxDispatchThread *dispatchThread = (RfxDispatchThread *) arg;
        dispatchThread->processStatusSyncMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_status_sync_messages_loop close");
    return NULL;
}

/*void RfxDispatchThread::enqueueRequestAckMessage(RILD_RadioTechnology_Group source, int slotId,
        int token, Parcel* parcel) {
    MessageObj *obj = pendingQueue.getClonedObj(token);
    if (obj == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "enqueueRequestAckMessage(): No correspending request!");
        return;
    }

    sp<RfxMessage> message;
    // TODO: FIXME
    if (obj->msg->getClientId() != -1) {
        //message = RfxMessage::obtainRequestAck(slotId, obj->msg->getPId(),
        //obj->msg->getPToken(), obj->msg->getId(), obj->msg->getToken(), source,
        //parcel, obj->msg->getClientId(), obj->msg->getPTimeStamp());
    } else {
        //message = RfxMessage::obtainRequestAck(obj->msg->getSlotId(), obj->msg->getPId(),
        //obj->msg->getPToken(), obj->msg->getId(), obj->msg->getToken(), source,
        //parcel, obj->msg->getPTimeStamp(), obj->msg->getRilToken());
    }
    message->setSentOnCdmaCapabilitySlot(obj->msg->getSentOnCdmaCapabilitySlot());
    MessageObj *dispatchObj = createMessageObj(message);

    dispatchResponseQueue.enqueue(dispatchObj);
    delete(obj);

}*/
