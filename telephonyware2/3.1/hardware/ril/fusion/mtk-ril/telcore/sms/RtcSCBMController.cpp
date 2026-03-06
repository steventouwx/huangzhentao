/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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
#include "RfxController.h"
#include "RtcSCBMController.h"
#include "RfxMessage.h"
#include "RfxMessageId.h"
#include "RfxLog.h"
#include "RfxVoidData.h"

/*****************************************************************************
 * Class RtcSCBMController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcSCBMController", RtcSCBMController, RfxController);

RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_ENTER_SMS_SCBM);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_EXIT_SMS_SCBM);

RtcSCBMController::RtcSCBMController() : m_timer(NULL) {
    setTag(String8("RtcSCBM"));
    logD(mTag, "RtcSCBMController: constructor");
}

RtcSCBMController::~RtcSCBMController() {
}

void RtcSCBMController::onInit() {
    RfxController::onInit();
    const int request_id_list[] = {
        RFX_MSG_REQUEST_EXIT_SMS_SCBM,
    };
    const int urc_id_list[] = {
        RFX_MSG_URC_ENTER_SMS_SCBM,
        RFX_MSG_URC_EXIT_SMS_SCBM,
    };
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int), DEFAULT);
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));
}

void RtcSCBMController::onDeinit() {
    RfxController::onDeinit();
}

bool RtcSCBMController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch(msg_id) {
        case RFX_MSG_REQUEST_EXIT_SMS_SCBM:
            if (m_timer != NULL) {
                logD(mTag, "Ap exit SCBM. Stop SCBM timer");
                RfxTimer::stop(m_timer);
                m_timer = NULL;
            }
            requestToMcl(message);
            break;
        default:
            break;
    }
    return true;
}

void RtcSCBMController::onTimer() {
    logD(mTag, "Timeout. Exit SCBM.");
    m_timer = NULL;
    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_EXIT_SMS_SCBM,
            RfxVoidData());
    requestToMcl(msg);
}

bool RtcSCBMController::onHandleUrc(const sp<RfxMessage>& msg) {
    int msg_id = msg->getId();
    switch (msg_id) {
        case RFX_MSG_URC_ENTER_SMS_SCBM:
            if (m_timer != NULL) {
                logD(mTag, "Stop previous SCBM timer");
                RfxTimer::stop(m_timer);
                m_timer = NULL;
            }
            logD(mTag, "Start SCBM Timer 5mins.");
            m_timer = RfxTimer::start(RfxCallback0(this, &RtcSCBMController::onTimer),
                    s2ns(SCM_EXIT_TIME));
            responseToRilj(msg);
            break;
        case RFX_MSG_URC_EXIT_SMS_SCBM:
            if (m_timer != NULL) {
                logD(mTag, "Stop SCBM timer");
                RfxTimer::stop(m_timer);
                m_timer = NULL;
            }
            responseToRilj(msg);
            break;
        default:
            logD(mTag, "Not Support the urc %s", idToString(msg_id));
            break;
    }
    return true;
}

