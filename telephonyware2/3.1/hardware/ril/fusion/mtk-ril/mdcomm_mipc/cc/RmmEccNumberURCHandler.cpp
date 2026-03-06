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

#include "RmmEccNumberURCHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxMipcData.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"

#define RFX_LOG_TAG "RmmEccNumberUrcHandler"

RFX_IMPLEMENT_HANDLER_CLASS(RmmEccNumberUrcHandler, RIL_CMD_PROXY_URC);


RmmEccNumberUrcHandler::RmmEccNumberUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int urc[] = {
        MIPC_CALL_ECC_LIST_CHANGE_IND,
    };
    logD(RFX_LOG_TAG, "init slotid %d, ch id %d", slot_id, channel_id);
    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
    const int request[] = {
        RFX_MSG_REQUEST_LOCAL_HANDLE_SCREEN_STATE_CHANGE
    };
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmmEccNumberUrcHandler::~RmmEccNumberUrcHandler() {
}

void RmmEccNumberUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData()->getMipcMsgId() == MIPC_CALL_ECC_LIST_CHANGE_IND) {
        logD(RFX_LOG_TAG, "[%s] receive MIPC_CALL_ECC_LIST_CHANGE_IND", __FUNCTION__);
        sendEvent(RFX_MSG_EVENT_MD_ECC_CHANGE, RfxVoidData(),
                RIL_CMD_PROXY_2, m_slot_id);
    }
}

void RmmEccNumberUrcHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_LOCAL_HANDLE_SCREEN_STATE_CHANGE: {
            // Handle screen state change
            int screenState = getNonSlotMclStatusManager()->getIntValue(
                    RFX_STATUS_KEY_SCREEN_STATE);
            const int urc[] = {
                MIPC_CALL_ECC_LIST_CHANGE_IND,
            };
            logD(RFX_LOG_TAG, "[%s] screenState: %d", __FUNCTION__, screenState);
            if (screenState == 1) {
                registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
                logD(RFX_LOG_TAG, "[%s] Trigger update ECC list for scrren on", __FUNCTION__);
                sendEvent(RFX_MSG_EVENT_MD_ECC_CHANGE, RfxVoidData(),
                        RIL_CMD_PROXY_2, m_slot_id);
            } else {
                unregisterToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
            }
            sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                    RIL_E_SUCCESS, RfxVoidData(), msg);
            responseToTelCore(response);
            break;
        }
        default:
            logE(RFX_LOG_TAG, "[%s]Should not be here", __FUNCTION__);
            break;
    }
}
