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
 * MediaTek Inc. (C) 2015. All rights reserved.
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

#include "RpDataController.h"
#include "RpDataUtils.h"
#include "RfxNwServiceState.h"

#include "apn/RpApnController.h"

#include "RfxStatusDefs.h"
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <cutils/jstring.h>

#include <string>
using namespace std;

#define RFX_LOG_TAG "RpDataController"
#define LOG_TAG "RpDataController"

/*****************************************************************************
 * Class RfxDataController
 * The class is created if the slot is single mode, LWG or C,
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RpDataController", RpDataController, RfxController);

RpDataController::RpDataController() :
    mRetryTimerHandle(NULL){
}

RpDataController::~RpDataController() {
}

void RpDataController::onInit() {
    RfxController::onInit();  // Required: invoke super class implementation

    logD(RFX_LOG_TAG, "onInit");

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallback(this, &RpDataController::onRadioStateChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SERVICE_STATE,
            RfxStatusChangeCallback(this, &RpDataController::onServiceStateChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CALL_STATE,
                RfxStatusChangeCallback(this, &RpDataController::onCallStateChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_PS_RESTRICT_STATE,
                    RfxStatusChangeCallback(this, &RpDataController::onPsRestrictStateChanged));

    const int request_id_list[] = {
    RIL_REQUEST_SETUP_DATA_CALL,  // 27
            RIL_REQUEST_DEACTIVATE_DATA_CALL,  // 41
            RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE, // 56
            RIL_REQUEST_DATA_CALL_LIST,  // 57
            RIL_REQUEST_ALLOW_DATA,  // 123

            };

    const int urc_id_list[] = {
    RIL_UNSOL_DATA_CALL_LIST_CHANGED,  // 1010
            };

    // register request & URC id list
    registerToHandleRequest(request_id_list,
            sizeof(request_id_list) / sizeof(int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list) / sizeof(int));

    RpApnController *apnController = (RpApnController *)findController(
                        RFX_OBJ_CLASS_INFO(RpApnController));

    if (apnController != NULL)
        apnController->apn_ready_singal.connect(this, &RpDataController::onApnReady);

    // set RpDataController
    RpDataController::mDcTracker.setRpDataController(this);

    isRequestToModem = false;

    mDataRequestHandleThread = new DataRequestHandleThread(this);
    mDataRequestHandleThread->run("dataRequestThread");

}

void RpDataController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
        RfxStatusChangeCallback(this, &RpDataController::onRadioStateChanged));

    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_SERVICE_STATE,
        RfxStatusChangeCallback(this, &RpDataController::onServiceStateChanged));

    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CALL_STATE,
        RfxStatusChangeCallback(this, &RpDataController::onCallStateChanged));

    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_PS_RESTRICT_STATE,
                    RfxStatusChangeCallback(this, &RpDataController::onPsRestrictStateChanged));

    RfxController::onDeinit();
}

bool RpDataController::DataRequestHandleThread::threadLoop() {
    RLOGI("DataRequestHandleThread threadLoop");

    // start message loop
    m_looper = Looper::prepare(0);
    int result;
    do {
        result = m_looper->pollAll(-1);
        RLOGD("DataRequestHandleThread threadLoop, pull message result = %d", result);
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);
    return true;
}

sp<Looper> RpDataController::DataRequestHandleThread::getLooper() {
    return m_looper;
}

void RpDataController::DataRequestHandler::handleMessage(const Message& message) {
    RLOGD("handleMessage msg token=%d", m_msg->getToken());
    if (m_msg->getClientId() > 0) {
        mDataController->mDcTracker.requestNetwork(m_msg);
    } else {
        mDataController->sendToGsm(m_msg);
    }
    mDataController->mRequestMutex.lock();
    if(mDataController->isRequestToModem) {
        nsecs_t mRequestTimeout = 3e11; //300s
        RLOGD("handleMessage wait response");

        mDataController->mRequestCondition.waitRelative(mDataController->mRequestMutex, mRequestTimeout);
    }
    mDataController->mRequestMutex.unlock();
}

sp<RpDataController::DataRequestHandler> RpDataController::sendMessage(const sp<RfxMessage>& msg, int delayms) {
    logI(RFX_LOG_TAG, "sendMessage msg token=%d delayms=%d", msg->getToken(), delayms);
    // Create a handler to handle this message
    sp<DataRequestHandler> handler = new DataRequestHandler(this,msg);
    // Send message to looper
    if (delayms > 0) {
        mDataRequestHandleThread->getLooper()->sendMessageDelayed(ms2ns(delayms),
                handler, handler->m_dummyMsg);
    } else {
        mDataRequestHandleThread->getLooper()->sendMessage(handler, handler->m_dummyMsg);
    }
    return handler;
}

bool RpDataController::onHandleRequest(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "Handle request %s", RpDataUtils::requestOrUrcToString(message->getId()));

    switch (message->getId()) {
    case RIL_REQUEST_SETUP_DATA_CALL:
        handleSetupDataRequest(message);
        break;
    case RIL_REQUEST_DEACTIVATE_DATA_CALL:
        handleDeactivateDataRequest(message);
        break;
    case RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE:
        handleGetLastFailCauseRequest(message);
        break;
    case RIL_REQUEST_DATA_CALL_LIST:
        handleGetDataCallListRequest(message);
        break;
    case RIL_REQUEST_ALLOW_DATA:
        handleSetDataAllowRequest(message);
        break;
    default:
        logW(RFX_LOG_TAG, "unknown request, ignore!");
        break;
    }
    return true;
}

bool RpDataController::onHandleResponse(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "Handle response %s.", RpDataUtils::requestOrUrcToString(message->getId()));

    switch (message->getId()) {
    case RIL_REQUEST_SETUP_DATA_CALL:
        handleSetupDataResponse(message);
        break;
    case RIL_REQUEST_DEACTIVATE_DATA_CALL:
        handleDeactivateDataResponse(message);
        break;
    case RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE:
        handleGetLastFailCauseResponse(message);
        break;
    case RIL_REQUEST_DATA_CALL_LIST:
        handleGetDataCallListResponse(message);
        break;
    case RIL_REQUEST_ALLOW_DATA:
        handleSetDataAllowResponse(message);
        break;
    default:
        logW(RFX_LOG_TAG, "unknown response, ignore!");
        break;
    }
    return true;
}

bool RpDataController::onHandleUrc(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "Handle URC %s", RpDataUtils::requestOrUrcToString(message->getId()));

    switch (message->getId()) {
    case RIL_UNSOL_DATA_CALL_LIST_CHANGED: {
        handleDataCallListChangeUrc(message);
        break;
    }
    default:
        logW(RFX_LOG_TAG, "unknown urc, ignore!");
        break;
    }
    return true;
}
void RpDataController::handleSetDataAllowRequest(const sp<RfxMessage>& request) {
    RpDataController::sendToGsm(request);
}

void RpDataController::handleSetDataAllowResponse(const sp<RfxMessage>& response) {
    responseToRilj(response);
}


void RpDataController::handleGetLastFailCauseRequest(const sp<RfxMessage>& request) {
    RpDataController::sendToGsm(request);
}

void RpDataController::handleGetLastFailCauseResponse(const sp<RfxMessage>& response) {
    logD(RFX_LOG_TAG, "handleGetLastFailCauseResponse with clientId: %d, with token: %d",
            response->getClientId(), response->getToken());
    responseToRilj(response);
    if (response->getClientId() > 0 || mDcTracker.isLocalCallFailPdnrequest(response)) {
        RpDataController::mDcTracker.onGetDataCallFailCauseCompleted(response);
    }
}

void RpDataController::handleGetDataCallListRequest(const sp<RfxMessage>& request) {
    RpDataController::sendToGsm(request);
}

void RpDataController::handleGetDataCallListResponse(const sp<RfxMessage>& response) {
    responseToRilj(response);
}

void RpDataController::handleSetupDataRequest(const sp<RfxMessage>& request) {
    logD(RFX_LOG_TAG, "handleSetupDataRequest with clientId: %d, with token: %d",
            request->getClientId(), request->getToken());

    sendMessage(request, 0);
}

void RpDataController::handleDeactivateDataRequest(const sp<RfxMessage>& request) {
    logD(RFX_LOG_TAG, "handleDeactivateDataRequest with clientId: %d, with token: %d",
            request->getClientId(), request->getToken());

    if (request->getClientId() > 0) {
        RpDataController::mDcTracker.deactiveNetwork(request);
    } else {
        RpDataController::sendToGsm(request);
    }
}

void RpDataController::handleSetupDataResponse(const sp<RfxMessage>& response) {
    logD(RFX_LOG_TAG, "handleSetupDataResponse with clientId: %d, with token: %d",
            response->getClientId(), response->getToken());
    mRequestMutex.lock();
    logI(RFX_LOG_TAG,"handleSetupDataResponse wait release");
    mRequestCondition.signal();
    isRequestToModem = false;
    mRequestMutex.unlock();

    if (response->getClientId() > 0 || mDcTracker.isLocalSetupPdnRequest(response)) {
        RpDataController::mDcTracker.onSetupConnectionCompleted(response);
    } else {
        responseToRilj(response);
    }
}

void RpDataController::handleDeactivateDataResponse(const sp<RfxMessage>& response) {
    logD(RFX_LOG_TAG, "handleDeactivateDataResponse with clientId: %d, with token: %d",
            response->getClientId(), response->getToken());

    if (response->getClientId() > 0) {
        RpDataController::mDcTracker.onDeactiveConnectionCompleted(response);
    } else {
        responseToRilj(response);
    }
}

void RpDataController::handleDataCallListChangeUrc(const sp<RfxMessage>& message) {
    RpDataController::mDcTracker.onDataCallListChanged(message);

    responseToRilj(message);
}

void RpDataController::updateDataConnectionStatus(int oldCount, int newCount) {
    logD(RFX_LOG_TAG, "updateDataConnectionStatus oldCount %d, newCount %d", oldCount, newCount);
    if (oldCount != newCount) {
        if (oldCount == 0) {
            getStatusManager()->setIntValue(RFX_STATUS_KEY_DATA_CONNECTION, DATA_STATE_CONNECTED);
        } else if (newCount == 0) {
            getStatusManager()->setIntValue(RFX_STATUS_KEY_DATA_CONNECTION, DATA_STATE_DISCONNECTED);
        }
    }
}

void RpDataController::sendToGsm(const sp<RfxMessage>& request) {
    logD(RFX_LOG_TAG, "request: %s, send to Gsm, with token %d",
            RpDataUtils::requestOrUrcToString(request->getId()), request->getToken());

    requestToRild(request);
}

void RpDataController::onPsRestrictStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    bool oldPsRestrict = (bool)old_value.asBool();
    bool newPsRestrict = (bool)value.asBool();

    logD(RFX_LOG_TAG, "onPsRestrictStateChanged with old ps restrict value: %s, with new ps restrict value: %s",
            RpDataUtils::b2s(oldPsRestrict).c_str(), RpDataUtils::b2s(newPsRestrict).c_str());

    if (!newPsRestrict && oldPsRestrict) {
        RpDataController::mDcTracker.onPsRestrict(false);
    } else if (newPsRestrict && !oldPsRestrict) {
        RpDataController::mDcTracker.onPsRestrict(true);
    } else {
        logD(RFX_LOG_TAG, "onPsRestrictStateChanged: ps restrict state not changed.");
    }
}

void RpDataController::onRadioStateChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);

    RIL_RadioState radioState = (RIL_RadioState) value.asInt();
    logI(RFX_LOG_TAG, "radioState %d", radioState);

    if (radioState == RADIO_STATE_UNAVAILABLE) {
        // do something on not available, reset status
        getStatusManager()->setIntValue(RFX_STATUS_KEY_DATA_CONNECTION, DATA_STATE_DISCONNECTED);
    }
}

void RpDataController::onServiceStateChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant newValue) {
    RFX_UNUSED(key);

    RfxNwServiceState oldSS = (RfxNwServiceState) old_value.asServiceState();
    RfxNwServiceState newSS = (RfxNwServiceState) newValue.asServiceState();

    bool oldPsAttached = oldSS.isInService(oldSS.getRilDataRegState());
    bool newPsAttached = newSS.isInService(newSS.getRilDataRegState());

    logD(RFX_LOG_TAG, "onServiceStateChanged with old ps attached value: %s, with new ps attached value: %s",
            RpDataUtils::b2s(oldPsAttached).c_str(), RpDataUtils::b2s(newPsAttached).c_str());

    if (newPsAttached && !oldPsAttached) {
        RpDataController::mDcTracker.onDataConnectionAttached();
    } else if (oldPsAttached && !newPsAttached) {
        RpDataController::mDcTracker.onDataConnectionDetached();
    } else {
        logW(RFX_LOG_TAG, "onServiceStateChanged: ps attached state not changed.");
    }
}

void RpDataController::onCallStateChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant newValue) {

    RfxCallState nwCS = (RfxCallState) newValue.asCallState();

    logD(RFX_LOG_TAG, "onCallStateChanged %s", nwCS.toString().string());

    int callState = nwCS.getCallState();
    if (callState == CallStateEnum::CALL_STATE_CONNECTED) {
        RpDataController::mDcTracker.onCallStateStart();
    } else {
        RpDataController::mDcTracker.onCallStateEnd();
    }
}

void RpDataController::notifyWorld(const sp<RfxMessage>& response) {
    logD(RFX_LOG_TAG, "notifyWorld %s process done.", RpDataUtils::requestOrUrcToString(response->getId()));

    responseToRilj(response);
}

void RpDataController::retryRequest(string apnType, const sp<RfxMessage>& request, string reason, long delay) {

    logD(RFX_LOG_TAG, "RpDataController::retryRequest, retry with apnType: %s, request: %s, delay time: %ld",
            apnType.c_str(),
            RpDataUtils::requestOrUrcToString(request->getId()),
            delay);
    logD(RFX_LOG_TAG, "Step1,  mRetryTimerHandle=%p", mRetryTimerHandle.get());
    if (RpDataController::mRetryTimerHandle != NULL) {
        RfxTimer::stop(mRetryTimerHandle);
    }
    logD(RFX_LOG_TAG, "Step2,  mRetryTimerHandle=%p", mRetryTimerHandle.get());
    // anyway record the latest retry info.
    RpDataController::mRetryInfo.apnType = apnType;
    RpDataController::mRetryInfo.request = request;
    RpDataController::mRetryInfo.reason = reason;
    logD(RFX_LOG_TAG, "Step3,  mRetryTimerHandle=%p", mRetryTimerHandle.get());
    RpDataController::mRetryTimerHandle = RfxTimer::start(RfxCallback0(this,
            &RpDataController::onTimer), ms2ns(delay));
}

void RpDataController::onTimer() {
    string apnType = RpDataController::mRetryInfo.apnType;
    sp<RfxMessage> request = RpDataController::mRetryInfo.request;
    string reason = RpDataController::mRetryInfo.reason;

    logD(RFX_LOG_TAG, "RpDataController::onTimer, retry with apnType: %s, request: %s, delay time: %s",
            apnType.c_str(),
            RpDataUtils::requestOrUrcToString(request->getId()),
            reason.c_str());

    RpDataController::mDcTracker.trySetupData(apnType, request, reason);
}

void RpDataController::onApnReady() {
    logD(RFX_LOG_TAG, "RpDataController::onApnReady");

    RpApnController *apnController = (RpApnController *)findController(
                        RFX_OBJ_CLASS_INFO(RpApnController));
    if (apnController != NULL) {
        Vector<ApnsettingT*>* list = apnController->getApnInformation("*");
        RpDataController::mDcTracker.onApnReady(list);
    }
}

int RpDataController::getRilVoiceRegState() {
    int radioTech = RIL_RadioTechnology::RADIO_TECH_UNKNOWN;
    RfxNwServiceState ss = getStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE);
    radioTech = ss.getRilVoiceRadioTech();

    logD(RFX_LOG_TAG, "RpDataController::isRadioOn, with result: %d", radioTech);
    return radioTech;
}

bool RpDataController::isRadioOn() {
    bool result = false;
    int radioState = getStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, RIL_RadioState::RADIO_STATE_UNAVAILABLE);
    if (radioState == RIL_RadioState::RADIO_STATE_ON) {
        result = true;
    } else {
        result = false;
    }

    logD(RFX_LOG_TAG, "RpDataController::isRadioOn, with result: %s", RpDataUtils::b2s(result).c_str());
    return result;

}

bool RpDataController::isAllCallingStateIdle() {
    bool result = false;
    RfxCallState cs = getStatusManager()->getCallStateValue(RFX_STATUS_KEY_CALL_STATE);
    int callState = cs.getCallState();
    if (callState == CallStateEnum::CALL_STATE_CONNECTED) {
        result = false;
    } else {
        result = true;
    }

    logD(RFX_LOG_TAG, "RpDataController::isAllCallingStateIdle, with result: %s", RpDataUtils::b2s(result).c_str());
    return result;
}

bool RpDataController::isDataSupportConcurrent() {
    bool result = false;
    RfxNwServiceState ss = getStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE);
    result = ss.isConcurrentVoiceAndDataAllowed();

    logD(RFX_LOG_TAG, "RpDataController::isDataSupportConcurrent, with result: %s", RpDataUtils::b2s(result).c_str());
    return result;
}

bool RpDataController::isPsRestricted() {
    bool result = false;
    result = getStatusManager()->getBoolValue(RFX_STATUS_KEY_PS_RESTRICT_STATE, false);

    logD(RFX_LOG_TAG, "RpDataController::isPsRestricted, with result: %s", RpDataUtils::b2s(result).c_str());
    return result;
}

bool RpDataController::isRoaming() {
    bool result = false;
    RfxNwServiceState ss = getStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE);
    result = ss.getDataRoaming();

    logD(RFX_LOG_TAG, "RpDataController::isRoaming, with result: %s", RpDataUtils::b2s(result).c_str());
    return result;
}

bool RpDataController::isAttached() {
    bool result = false;
    RfxNwServiceState ss = getStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE);
    result = ss.isInService(ss.getRilDataRegState());

    logD(RFX_LOG_TAG, "RpDataController::isAttached, with result: %s", RpDataUtils::b2s(result).c_str());
    return result;
}

void RpDataController::sendToMD(const sp<RfxMessage>& request) {
    logD(RFX_LOG_TAG, "%s send to MD.", RpDataUtils::requestOrUrcToString(request->getId()));

    RpDataController::sendToGsm(request);;
}
void RpDataController::sendReponseToClient(RIL_Errno e, const sp<RfxMessage>& msg) {
    logD(RFX_LOG_TAG, "sendToClient(), %s send to Client with clientId: %d, with token: %d",
            RpDataUtils::requestOrUrcToString(msg->getId()), msg->getClientId(), msg->getToken());

    sp<RfxMessage> response = RfxMessage::obtainResponse(e, msg);
    RpDataController::responseToRilj(response);
}
