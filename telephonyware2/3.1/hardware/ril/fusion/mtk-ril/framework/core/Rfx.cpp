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
 * File name:  rfx.cpp
 * Author: Jun Liu (MTK80064)
 * Description:
 * Implement interfaces to initialize RIL Proxy framework and enqueue messages from
 * external modules
 */


/*****************************************************************************
 * Include
 *****************************************************************************/

#include "Rfx.h"
#include "RfxMainThread.h"
#include "RfxRilAdapter.h"
#include "RfxDispatchThread.h"
#include "RfxLog.h"
#include "RfxMclDispatcherThread.h"
#include "RfxDataCloneManager.h"
#include "RfxMclMessage.h"
#include "RfxViaUtils.h"
#ifdef MTK_USE_HIDL
#include <binder/ProcessState.h>
#endif
#include <libmtkrilutils.h>
#include "RfxOpUtils.h"
#include "RfxGwsdUtils.h"
#include "mipc_api.h"
#include "mipc_msg.h"
#include "mipc_msg_host.h"
#include "RfxMipcUtils.h"
#include "RfxAtLine.h"
#include "RfxVsimUtils.h"
#include "RfxIdToMsgIdUtils.h"
#include "RfxMessageId.h"
#include <unistd.h>

/*****************************************************************************
 * Implementation
 *****************************************************************************/
//RfxDispatchThread *dispatchThread;
sp<RfxDispatchThread> dispatchThread;
extern "C"
void mipcIndicationCallback(mipc_msg_t *msg_ptr, void*priv_ptr);


extern "C"
void RIL_onRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen);


#define RFX_LOG_TAG "RFX"

int RFX_SLOT_COUNT = 2;

void mipcIndicationCallback(mipc_msg_t *msg_ptr, void* priv_ptr) {
    RFX_UNUSED(priv_ptr);
    char* temp = NULL;
    char* atcmd_ptr = NULL;
    uint16_t atcommand_len = 0;
    int slotId = RfxMipcUtils::mipcSlotIdChangeToSlotId(msg_ptr->hdr.msg_sim_ps_id);
    int mipcMsgId = msg_ptr->hdr.msg_id;
    int channelId = slotId * RIL_CHANNEL_OFFSET;
    RFX_LOG_D(RFX_LOG_TAG, "IndCallback start slotId:%d,mipcId:%d", slotId, mipcMsgId);
    if (mipcMsgId == MIPC_SYS_AT_IND) {
        temp = (char*)mipc_msg_get_val_ptr(msg_ptr, MIPC_SYS_AT_IND_T_ATCMD, &atcommand_len);
        RFX_LOG_D(RFX_LOG_TAG, "IndCallback str:%s, strlen:%d", temp, atcommand_len);
        if (temp[atcommand_len - 2 ] == '\r' || temp[atcommand_len - 2] == '\n') {
            temp[atcommand_len - 2 ] = '\0';
        }
        if (temp[atcommand_len - 3 ] == '\r' || temp[atcommand_len - 3] == '\n') {
            temp[atcommand_len - 3 ] = '\0';
        }
        while (*temp == '\r' || *temp == '\n') {
            temp++;
        }
        atcmd_ptr = temp;
        RfxAtLine *atLine = new RfxAtLine(atcmd_ptr, NULL);
        sp<RfxMclMessage> msg = RfxMclMessage::obtainRawUrc(channelId, atLine, NULL);
        RfxMclDispatcherThread::waitLooper();
        RfxMclDispatcherThread::enqueueMclMessage(msg);
    }
    RFX_LOG_D(RFX_LOG_TAG, "IndCallback end slotId:%d,mipcId:%d", slotId, mipcMsgId);
}

void rfx_init(void) {
#ifdef MTK_USE_HIDL
    android::ProcessState::initWithDriver("/dev/vndbinder");
#endif
    RFX_SLOT_COUNT = getSimCount();
    RFX_LOG_D(RFX_LOG_TAG, "rfx_init (RFX_SLOT_COUNT=%d)", RFX_SLOT_COUNT);

    // init operator library for operator software decouple
    RfxOpUtils::initOpLibrary();
    // init Gwsd
    RfxGwsdUtils::initHandler();
    // init ViaHandler
    RfxViaUtils::initViaHandler();
    // init VsimHandler
    RfxVsimUtils::initVsimHandler();
    // init MdComm layer
    RfxMclDispatcherThread::init();
    if (RfxRilUtils::isMipcSupport() == true) {
        for (unsigned int i = 0; i < getSimCount(); i++) {
            int ret = 0;
            ret = mipc_msg_register_ind((RfxMipcUtils::slotIdChangeToMipcSlotId(i)),
                    MIPC_SYS_AT_IND, (void*)mipcIndicationCallback, NULL);
            RFX_LOG_D(RFX_LOG_TAG, "mipc_msg_register_ind, slotid:%d,return:%d", i, ret);
        }
    }
    // init TelCore layer
    RfxMainThread::init();
    dispatchThread = RfxDispatchThread::init();
}

void rfx_enqueue_request_message(int request, void *data, size_t datalen, RIL_Token t,
        RIL_SOCKET_ID socketId) {
#if defined(MTK_MTTS_TEST_ENABLE)
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    if (atoi(property_value) == 1) {
        RFX_LOG_D(RFX_LOG_TAG, "test mode on, return not support : %d", request);
        RfxRequestInfo *requestInfo = (RfxRequestInfo *)t;
        sp<RfxMessage> resMsg = RfxMessage::obtainResponse(socketId, request, requestInfo->token,
                INVALID_ID, -1, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj *obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
        return;
    }
#endif
    if (getRildInitDoneState() == false) {
        RFX_LOG_D(RFX_LOG_TAG, "rild init not done, enqueue request:[%d]", request);
        // RIL_onRequestComplete(t, RIL_E_RADIO_NOT_AVAILABLE, NULL, 0);
        // return;
    }

    dispatchThread->enqueueRequestMessage(request, data, datalen, t, socketId);
}

#if defined(ANDROID_MULTI_SIM)
void rfx_enqueue_request_message_client(int request, void *data, size_t datalen,
        RIL_Token t, RIL_SOCKET_ID socketId) {
    RfxRequestInfo *requestInfo = (RfxRequestInfo *)t;
    int clientId = requestInfo->clientId;
#if defined(MTK_MTTS_TEST_ENABLE)
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    if (atoi(property_value) == 1 && (clientId != CLIENT_ID_MTTS1
            && clientId != CLIENT_ID_MTTS2 && clientId != CLIENT_ID_MTTS3
            && clientId != CLIENT_ID_MTTS4)) {
        RFX_LOG_D(RFX_LOG_TAG, "test mode but not MTTS request, return not support : %d", request);
        sp<RfxMessage> resMsg = RfxMessage::obtainResponse(socketId, request, requestInfo->token,
                INVALID_ID, -1, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj *obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
        return;
    }
#endif
    dispatchThread->enqueueRequestMessage(request, data, datalen, t, socketId, clientId);
}
#else
void rfx_enqueue_request_message_client(int request, void *data, size_t datalen,
        RIL_Token t) {
    RfxRequestInfo *requestInfo = (RfxRequestInfo *)t;
    int clientId = requestInfo->clientId;
#if defined(MTK_MTTS_TEST_ENABLE)
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    if (atoi(property_value) == 1 && (clientId != CLIENT_ID_MTTS1
            && clientId != CLIENT_ID_MTTS2 && clientId != CLIENT_ID_MTTS3
            && clientId != CLIENT_ID_MTTS4)) {
        RFX_LOG_D(RFX_LOG_TAG, "test mode but not MTTS request, return not support : %d", request);
        sp<RfxMessage> resMsg = RfxMessage::obtainResponse(RIL_SOCKET_1, request, requestInfo->token,
                INVALID_ID, -1, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj *obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
        return;
    }
#endif
    dispatchThread->enqueueRequestMessage(request, data, datalen, t, RIL_SOCKET_1, clientId);
}
#endif

void rfx_enqueue_sap_request_message(int request, void *data, size_t datalen,
        RIL_Token t, RIL_SOCKET_ID socketId) {
#if defined(MTK_MTTS_TEST_ENABLE)
   char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    if (atoi(property_value) == 1) {
        RFX_LOG_D(RFX_LOG_TAG, "test mode on, return not support : %d", request);
        RfxRequestInfo *requestInfo = (RfxRequestInfo *)t;
        sp<RfxMessage> resMsg = RfxMessage::obtainResponse(socketId, request, requestInfo->token,
                INVALID_ID, -1, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj *obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
        return;
    }
#endif
   dispatchThread->enqueueSapRequestMessage(request, data, datalen, t, socketId);
}

void rfx_update_connection_state(RIL_SOCKET_ID socketId, int isConnected, unsigned int clientType) {
    dispatchThread->updateConnectionState(socketId, isConnected, clientType);
}
