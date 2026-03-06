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
#include "RtmVsimController.h"
#include <cstring>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <telephony/mtk_ril.h>
#include "RfxVoidData.h"
#include "RfxStringsData.h"
#include "RfxMainThread.h"
#include "RfxRilUtils.h"
#include "RfxStringData.h"
#include "rfx_properties.h"
#include <mtkconfigutils.h>
#include "RfxIntsData.h"

using ::android::String8;
using namespace std;

#define RFX_LOG_TAG "RtmVsim"

RFX_IMPLEMENT_CLASS("RtmVsimController", RtmVsimController, RfxController);


/*****************************************************************************
 * Class RtmVsimController
 *****************************************************************************/
RtmVsimController::RtmVsimController() {
}

RtmVsimController::~RtmVsimController() {
}

void RtmVsimController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logD(RFX_LOG_TAG, "onInit, slot:%d", m_slot_id);
    // register request & URC id list for radio state check
    const int request_id_list[] = {
        RFX_MSG_REQUEST_SIM_VSIM_NOTIFICATION,
        RFX_MSG_REQUEST_SIM_VSIM_OPERATION,
    };

    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int));
}

bool RtmVsimController::onCheckIfRejectMessage(
        const sp<RfxMessage>& msg, bool isModemPowerOff, int radioState) {
    int msgId = msg->getId();

    if (msgId == RFX_MSG_REQUEST_SIM_VSIM_NOTIFICATION ||
            msgId == RFX_MSG_REQUEST_SIM_VSIM_OPERATION) {
        return false;
    }

    return RfxController::onCheckIfRejectMessage(msg, isModemPowerOff, radioState);
}

bool RtmVsimController::onHandleRequest(const sp<RfxMessage>& msg) {
    int msg_id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest %s", msg->toString().string());
    switch (msg_id) {
        case RFX_MSG_REQUEST_SIM_VSIM_NOTIFICATION:
        case RFX_MSG_REQUEST_SIM_VSIM_OPERATION:
            // Send RMM directly
            requestToMcl(msg);
            break;
        default:
            logD(RFX_LOG_TAG, "Not Support the req %s", idToString(msg_id));
            break;
    }

    return true;
}

bool RtmVsimController::onHandleResponse(const sp<RfxMessage>& msg) {
    int msg_id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleResponse %s", idToString(msg_id));
    switch (msg_id) {
        case RFX_MSG_REQUEST_SIM_VSIM_NOTIFICATION:
        case RFX_MSG_REQUEST_SIM_VSIM_OPERATION:
            // Send RILJ directly
            responseToRilj(msg);
            break;
        default:
            logD(RFX_LOG_TAG, "Not Support the req %s", idToString(msg_id));
            break;
    }

    return true;
}

