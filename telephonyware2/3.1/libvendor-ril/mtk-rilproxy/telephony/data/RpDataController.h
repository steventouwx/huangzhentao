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

#ifndef __RP_DATA_CONTROLLER_H__
#define __RP_DATA_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "DcTracker.h"
#include "RfxTimer.h"

#include <utils/Vector.h>

using ::android::Message;
using ::android::Condition;
using ::android::Mutex;

/*****************************************************************************
 * Class RpDataController
 *****************************************************************************/
class RpDataController : public RfxController {
    RFX_DECLARE_CLASS(RpDataController);  // Required: declare this class

public:
    RpDataController();

    virtual ~RpDataController();
    void notifyWorld(const sp<RfxMessage>& response);
    void retryRequest(string apnType, const sp<RfxMessage>& request, string reason, long delay);
    void onTimer();
    void onApnReady();

    void sendToMD(const sp<RfxMessage>& request);
    void sendReponseToClient(RIL_Errno e,const sp<RfxMessage>& msg);
    int getRilVoiceRegState();
    bool isRadioOn();
    bool isAllCallingStateIdle();
    bool isDataSupportConcurrent();
    bool isPsRestricted();
    bool isRoaming();
    bool isAttached();

    class DataRequestHandleThread: public Thread {
    public:
        DataRequestHandleThread(RpDataController* dataController) : mDataController(dataController),m_looper(NULL){}
        virtual ~DataRequestHandleThread() {}
        sp<Looper> getLooper();

    protected:
        RpDataController* mDataController;
        virtual bool threadLoop();
    private:
        sp<Looper> m_looper;
    };
    class DataRequestHandler: public MessageHandler {
    public:
        DataRequestHandler(RpDataController* dataController, const sp<RfxMessage>& msg) : mDataController(dataController), m_msg(msg) {}
        virtual ~DataRequestHandler() {}

    public:
        void handleMessage(const Message& message);
        // data message reference
        sp<RfxMessage> m_msg;
        // dummy message that makes handler happy
        Message m_dummyMsg;
    private:
        RpDataController* mDataController;
    };

    sp<DataRequestHandleThread> mDataRequestHandleThread;
    sp<DataRequestHandler> mDataRequestHandler;
    // send message to request handler
    Mutex mRequestMutex;
    Condition mRequestCondition;
    bool isRequestToModem;

    sp<DataRequestHandler> sendMessage(const sp<RfxMessage>& msg, int delayms);

// Override
protected:
    void sendToGsm(const sp<RfxMessage>& message);
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);

    virtual void handleSetupDataRequest(const sp<RfxMessage>& request);
    virtual void handleDeactivateDataRequest(const sp<RfxMessage>& request);

    virtual void handleSetupDataResponse(const sp<RfxMessage>& response);
    virtual void handleDeactivateDataResponse(const sp<RfxMessage>& response);

    virtual void handleDataCallListChangeUrc(const sp<RfxMessage>& message);

    virtual void handleGetLastFailCauseRequest(const sp<RfxMessage>& request);
    virtual void handleGetLastFailCauseResponse(const sp<RfxMessage>& response);

    virtual void handleGetDataCallListRequest(const sp<RfxMessage>& request);
    virtual void handleGetDataCallListResponse(const sp<RfxMessage>& response);
    virtual void handleSetDataAllowRequest(const sp<RfxMessage>& request);
    virtual void handleSetDataAllowResponse(const sp<RfxMessage>& response);

    virtual void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    virtual void onServiceStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    virtual void onCallStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    virtual void onPsRestrictStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

private:
    // Private functions
    void updateDataConnectionStatus(int oldCount, int newCount);

    class RetryInfo {
    public:
        string apnType;
        sp<RfxMessage> request;
        string reason;

        RetryInfo():
            apnType(""),
            request(NULL),
            reason("") {}
    };

private:
    DcTracker mDcTracker;
    TimerHandle mRetryTimerHandle;

    RetryInfo mRetryInfo;
};

#endif /* __RP_DATA_CONTROLLER_H__ */

