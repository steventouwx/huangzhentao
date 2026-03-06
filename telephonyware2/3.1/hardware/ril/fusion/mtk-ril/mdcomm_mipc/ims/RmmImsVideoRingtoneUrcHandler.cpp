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

#include "RmmImsVideoRingtoneUrcHandler.h"
#include <telephony/mtk_ril.h>

#include "rfx_properties.h"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmImsVideoRingtoneUrcHandler, RIL_CMD_PROXY_URC);

// register data
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_VIDEO_RINGTONE_EVENT_IND);

RmmImsVideoRingtoneUrcHandler::RmmImsVideoRingtoneUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmmImsVideoRingtoneUrcHandler constructor");

    //int m_slot_id = slot_id;

    const int urc[] = {
         MIPC_CALL_UIS_INFO_IND,
    };

    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
}

RmmImsVideoRingtoneUrcHandler::~RmmImsVideoRingtoneUrcHandler() {
}

void RmmImsVideoRingtoneUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        logD(RFX_LOG_TAG, "onHandleUrc MipcMsgId:%d, MipcSlotId:%d",
                msg->getMipcData()->getMipcMsgId(), msg->getMipcData()->getMipcSlotId());
        uint16_t msgId = msg->getMipcData()->getMipcMsgId();
        switch (msgId) {
            case MIPC_CALL_UIS_INFO_IND:
                handleVideoRingtoneEventIndMessage(msg);
                break;

            default:
                logE(RFX_LOG_TAG, "Should not be here");
            break;
        }
    } else {
        logE(RFX_LOG_TAG, "onHandleUrc, getMipcData NULL");
    }
}

void RmmImsVideoRingtoneUrcHandler::handleVideoRingtoneEventIndMessage(const sp<RfxMclMessage>& msg) {
    // +EUISINFO:<call_id>, <msg_type>, <data>, <result>, <cause>
    const int maxLen = 5;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_VIDEO_RINGTONE_EVENT_IND;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

