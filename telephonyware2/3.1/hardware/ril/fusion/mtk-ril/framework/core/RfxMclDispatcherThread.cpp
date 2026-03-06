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

#include "RfxMclDispatcherThread.h"
#include "RfxLog.h"
#include "RfxHandlerManager.h"
#include "RfxMclStatusManager.h"
#include "RfxFragmentEncoder.h"
#include "utils/Mutex.h"
#include "RfxDispatchThread.h"
#include "RfxVoidData.h"
#include "RfxVersionManager.h"

#define RFX_LOG_TAG "RfxMclDisThread"

using ::android::Mutex;

sp<RfxMclDispatcherThread> RfxMclDispatcherThread::s_self = NULL;
Mutex RfxMclDispatcherThread::sWaitLooperMutex;

static sem_t sWaitLooperSem;
static bool sNeedWaitLooper = true;

/*****************************************************************************
 * Class RfxMclBaseMessenger
 *****************************************************************************/
void RfxMclBaseMessenger::handleMessage(const Message& message) {
    onHandleMessage(message);
}

/*****************************************************************************
 * Class RfxMclMessenger
 *****************************************************************************/
void RfxMclMessenger::onHandleMessage(const Message& message) {
    RFX_UNUSED(message);
    if (STATUS_SYNC == msg->getType()) {
        RfxMclStatusManager* statusMgr = RfxMclStatusManager::getMclStatusManager(
                msg->getSlotId());
        statusMgr->setValueByRfx(msg->getStatusKey(), msg->getStatusValue(),
                msg->getForceNotify(), msg->getIsDefault(), true, msg->getIsUpdateForMock());
    } else {
        int channelId;
        if (MIPC_URC == msg->getType() || MIPC_CMD == msg->getType()) {
            RFX_LOG_D("RfxMclMessenger", "mipcid:%d, mipctype:%d",
                    msg->getMipcData()->getMipcMsgId(), msg->getType());
            channelId = RfxHandlerManager::findMsgChannel(msg->getType(), msg->getSlotId(),
                    msg->getMipcData()->getMipcMsgId(), msg->getClientId(),
                    (msg->getRawUrc() == NULL ? NULL : msg->getRawUrc()->getLine()));
        } else if (msg->getSendToMainProtocol()) {
            int mainSlotId =
                    RfxMclStatusManager::getMclStatusManager(RFX_SLOT_ID_UNKNOWN)->getIntValue(
                    RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
            RFX_LOG_I("RfxMclMessenger", "%s(%d) dispatchs to main protocol: %d",
                    RFX_ID_TO_STR(msg->getId()), msg->getId(), mainSlotId);
            channelId = RfxHandlerManager::findMsgChannel(msg->getType(), mainSlotId,
                    msg->getId(), msg->getClientId(),
                    (msg->getRawUrc() == NULL ? NULL : msg->getRawUrc()->getLine()));
        } else {
            channelId = RfxHandlerManager::findMsgChannel(msg->getType(), msg->getSlotId(),
                    msg->getId(), msg->getClientId(),
                    (msg->getRawUrc() == NULL ? NULL : msg->getRawUrc()->getLine()));
        }
        msg->setChannelId(channelId);

        // execute message
        if (channelId != -1) {
            RfxChannel* channel = RfxChannelManager::getChannel(channelId);
            if (msg->getAddAtFront()) {
                channel->enqueueMessageFront(msg);
            } else {
                channel->enqueueMessage(msg);
            }
        } else {
            RFX_LOG_D("RfxMclMessenger", "onHandleMessage, no one register id(%d), type(%d)",
                    msg->getId(), msg->getType());
            if (RAW_URC == msg->getType() || EVENT_MESSAGE == msg->getType()) {
                return;
            }
            // send response with error code: not support
            sp<RfxMclMessage> resMsg = RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED,
                    RfxVoidData(), msg);
            RfxDispatchThread::enqueueResponseMessage(resMsg);
        }
    }
}

/*****************************************************************************
 * Class RfxMclDispatcherThread
 *****************************************************************************/


MclMessageObj* createMclMessageObj(const sp<RfxMclMessage>& message) {
    MclMessageObj *obj = new MclMessageObj();
    obj->msg = message;
    obj->p_next = NULL;
    return obj;
}

void RfxMclDispatcherThread::addMclMessageToPendingQueue(const sp<RfxMclMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "addMclMessageToPendingQueue Request = %d, MclToken = %d, token = %d",
            message->getId(), message->getMclToken(), message->getToken());
    MclMessageObj *obj = createMclMessageObj(message);
    mclPendingQueue.enqueue(obj);
}

sp<RfxMclMessage> RfxMclDispatcherThread::getAndRemoveMclMessageFromPendingQueue(int mclToken) {
    MclMessageObj *obj = mclPendingQueue.checkAndDequeue(mclToken);
    if (obj == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "getAndRemoveMclMessageFromPendingQueue: No correspending(%d) request!", mclToken);
        return NULL;
    }

    sp<RfxMclMessage> message = obj->msg;
    delete(obj);
    return message;
}

sp<RfxMclMessage> RfxMclDispatcherThread::getFrontMclMessageFromPendingQueue(void) {
    MclMessageObj *obj = mclPendingQueue.getFront();
    if (obj == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "getFrontMclMessageFromPendingQueue: No correspending request!");
        return NULL;
    }

    sp<RfxMclMessage> message = obj->msg;
    return message;
}

RfxMclDispatcherThread::RfxMclDispatcherThread() {
}

void RfxMclDispatcherThread::init() {
    RFX_LOG_D(RFX_LOG_TAG, "init");
    sem_init(&sWaitLooperSem, 0, 0);
    s_self = new RfxMclDispatcherThread();
    s_self->run("RILD MCL Dispatcher Thread");
    RFX_LOG_D(RFX_LOG_TAG, "init3");
}

bool RfxMclDispatcherThread::threadLoop() {
    RFX_LOG_D(RFX_LOG_TAG, "threadLoop");
    m_looper = Looper::prepare(0);

    RfxVersionManager::init();
    RfxMclStatusManager::init();
    RfxFragmentEncoder::init();
    RfxChannelManager::init();

    sem_post(&sWaitLooperSem);

    int result;
    do {
        result = m_looper->pollAll(-1);
        RFX_LOG_D(RFX_LOG_TAG, "threadLoop, result = %d", result);
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);

    RFX_ASSERT(0); // Can't go here
    return false;
}

void RfxMclDispatcherThread::enqueueMclMessage(const sp<RfxMclMessage>& message) {
    if (!RfxRilUtils::isInLogReductionList(message->getId())) {
        RFX_LOG_D(RFX_LOG_TAG, "enqueueMclMessage: %s", message->toString().string());
    }
    sp<MessageHandler> handler = new RfxMclMessenger(message);
    s_self->m_looper->sendMessage(handler, s_self->m_dummy_msg);
}

void RfxMclDispatcherThread::enqueueMclMessageFront(const sp<RfxMclMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueMclMessage: %s", message->toString().string());
    sp<MessageHandler> handler = new RfxMclMessenger(message);
    s_self->m_looper->sendMessageAtTime(0, handler, s_self->m_dummy_msg);
}

void RfxMclDispatcherThread::enqueueMclMessageDelay(const sp<RfxMclMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueMclMessage: %s", message->toString().string());
    sp<MessageHandler> handler = new RfxMclMessenger(message);
    s_self->m_looper->sendMessageDelayed(message->getDelayTime(), handler, s_self->m_dummy_msg);
}

sp<Looper> RfxMclDispatcherThread::waitLooper() {
    RFX_ASSERT(s_self != NULL);

    if (sNeedWaitLooper == false) {
        return s_self->m_looper;
    }

    sWaitLooperMutex.lock();
    if (sNeedWaitLooper) {
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() begin");
        sem_wait(&sWaitLooperSem);
        sNeedWaitLooper = false;
        sem_destroy(&sWaitLooperSem);
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() end");
    }
    sWaitLooperMutex.unlock();
    return s_self->m_looper;
}

/*void RfxMclDispatcherThread::enqueueMessenger(const sp<RfxMclBaseMessenger> &messenger) {
    sp<MessageHandler> handler = messenger;
    s_self->m_looper->sendMessage(handler, s_self->m_dummy_msg);
}*/
