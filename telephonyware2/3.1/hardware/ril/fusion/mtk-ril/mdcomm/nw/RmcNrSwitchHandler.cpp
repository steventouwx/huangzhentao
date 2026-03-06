/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2019. All rights reserved.
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

#include "RmcNrSwitchHandler.h"

static const int request[] = {
    RFX_MSG_REQUEST_NR_SWITCH
};

// 400ms in 50 times, 5s in 176 times, total 90s
#define MAX_NR_SWITCH_RETRY_TIME 226
#define NORMAL_NR_SWITCH_RETRY_INTERVAL 400
#define MAX_NR_SWITCH_RETRY_INTERVAL 5000

#define PROPERTY_NR_SLOTID      "persist.vendor.radio.nrslot"

int RmcNrSwitchHandler::mNrPsSlot = -1;

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_NR_SWITCH);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcNrSwitchHandler, RIL_CMD_PROXY_9);

RmcNrSwitchHandler::RmcNrSwitchHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    char feature[] = "GEMINI_NR_PLUS_SUPPORTED";
    char NR_VER[] = "5G_SUPPORTED";
    m_slot_id = slot_id;
    m_channel_id = channel_id;
    mRetryCount = 1;
    registerToHandleRequest(request, sizeof(request)/sizeof(int));

    if (getFeatureVersion(NR_VER, 0) == 1 && getFeatureVersion(feature, 0) == 0) {
        int slot = 0, currMajorSim = 0;
        char optr[RFX_PROPERTY_VALUE_MAX] = {0};

        rfx_property_get("persist.vendor.radio.simswitch", optr, "1");
        currMajorSim = (atoi(optr)-1);
        slot = getNrPsSlot();
        if (slot == -1) {
            slot = 0;
        }
        if (m_slot_id == currMajorSim) {
            atSendCommand(String8::format("AT+EPSCONFIG=1,\"nr_ps\",%d", (slot+1)));
        }
    }
}

RmcNrSwitchHandler::~RmcNrSwitchHandler() {
}

void RmcNrSwitchHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    // logD(LOG_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(msg->getId()));
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_NR_SWITCH:
            requestNrSwitch(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

int RmcNrSwitchHandler::getNrPsSlot() {
    int mainSlot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    int defaultDataSim = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_DEFAULT_DATA_SIM); // get default data sim slot id.

    if (defaultDataSim == -1) {
        char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get(PROPERTY_NR_SLOTID, stgBuf, "1");
        defaultDataSim = strtol(stgBuf, NULL, 10);
        defaultDataSim--;
    }

    if (mainSlot == 1) { // switch PS if main protocol in slot 2
        defaultDataSim = defaultDataSim ? 0:1;
    }
    logV(LOG_TAG, "getNrPsSlot::mainSlot:%d mNrSlot:%d defaultDataSim:%d",
            mainSlot, mNrPsSlot, defaultDataSim);
    return defaultDataSim;
}

void RmcNrSwitchHandler::UpdateAPNrSlot() {
    int slot = getNrPsSlot();
    int dataSlot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_DEFAULT_DATA_SIM);
    if (dataSlot == -1) { // if data is unknow, use nr ps slot.
        int mainSlot = getNonSlotMclStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
        dataSlot = slot;
        if (mainSlot == 1) { // switch PS if main protocol in slot 2
            dataSlot = slot ? 0:1;
        }
    }
    rfx_property_set(PROPERTY_NR_SLOTID, String8::format("%d", (dataSlot+1)).string());
    getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_KEY_NR_SWITCH_SLOT, (dataSlot+1));
    mNrPsSlot = slot;
    logV(LOG_TAG, "requestNrSwitch ok, mNrSlot=%d dataSlot=%d", mNrPsSlot, dataSlot);
}


void RmcNrSwitchHandler::requestNrSwitch(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    sp<RfxAtResponse> p_response;
    int retryTime;
    int slot = getNrPsSlot();

    if (slot == -1) {
        UpdateAPNrSlot();
        goto finish;
    }
    getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_NR_SWITCHING, true);
    p_response = atSendCommand(String8::format("AT+EPSCONFIG=1,\"nr_ps\",%d", (slot+1)));
    if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
        logV(LOG_TAG, "requestNrSwitch success, slot:%d, getNrPsSlot:%d", slot, getNrPsSlot());
        if (slot != getNrPsSlot()) {
            sp<RfxMclMessage> mclMessage = RfxMclMessage::obtainRequest(msg->getId(),
                    msg->getData(), msg->getSlotId(), msg->getToken(),
                    msg->getSendToMainProtocol(), msg->getRilToken(), ms2ns(10),
                    msg->getTimeStamp(), msg->getAddAtFront());
            RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
            return;
        }
        UpdateAPNrSlot();
        ril_errno = RIL_E_SUCCESS;
    } else {
        logE(LOG_TAG, "requestNrSwitch fail, count=%d, error=%d",
                mRetryCount, p_response->atGetCmeError());
        if (p_response->atGetCmeError() == CME_OPERATION_NOT_ALLOWED_ERR) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            logE(LOG_TAG, "requestNrSwitch: fail, modem report no retry");
        } else {  // retry 40 times if Nr switch fail.
            if (mRetryCount < MAX_NR_SWITCH_RETRY_TIME) {
                mRetryCount++;
                if (mRetryCount <= 50) { // 50*0.4s
                    retryTime = NORMAL_NR_SWITCH_RETRY_INTERVAL;
                } else { // 176*5s
                    retryTime = MAX_NR_SWITCH_RETRY_INTERVAL;
                }
                sp<RfxMclMessage> mclMessage = RfxMclMessage::obtainRequest(msg->getId(),
                        msg->getData(), msg->getSlotId(), msg->getToken(),
                        msg->getSendToMainProtocol(), msg->getRilToken(), ms2ns(retryTime),
                        msg->getTimeStamp(), msg->getAddAtFront());
                RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
                return;
            } else {
                char pErrMsg[] = "requestNrSwitch: retry max asserion!";
                mtkAssert(pErrMsg);
            }
        }
    }
    p_response = NULL;

finish:
    mRetryCount = 1;
    getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_NR_SWITCHING, false);
    sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}
