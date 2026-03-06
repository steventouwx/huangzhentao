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

#include "RmmPhbURCHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxMessageId.h"
#include <libmtkrilutils.h>
#include "rfx_properties.h"

#define TRUE  1
#define FALSE 0
#define RFX_LOG_TAG "RmmPhbUrc"

using ::android::String8;

RFX_IMPLEMENT_HANDLER_CLASS(RmmPhbURCHandler, RIL_CMD_PROXY_URC);

RmmPhbURCHandler::RmmPhbURCHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    const int mipcUrc[] = {
        MIPC_PHB_READY_STATE_IND,
    };
    registerToHandleMipcURC(mipcUrc, sizeof(mipcUrc) / sizeof(int));
    rfx_property_get("ro.vendor.mtk_telephony_add_on_policy", prop, "");
    if (strncmp(prop, "0", 1) == 0) {
        isTelephonyAddOnPolicy = true;
    } else {
        isTelephonyAddOnPolicy = false;
    }
}

RmmPhbURCHandler::~RmmPhbURCHandler() {
}

void RmmPhbURCHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    if (mipcData != NULL && mipcData->getMipcMsgId() == MIPC_PHB_READY_STATE_IND) {
        uint8_t readyState = mipcData->getMipcUint8Val(MIPC_PHB_READY_STATE_IND_T_READY, 0);
        logI(RFX_LOG_TAG, "modemNotifyPhbReadyState: %d", readyState);
        if (readyState == 1) {
            onPhbStateChanged(TRUE);
        } else if (readyState == 0) {
            onPhbStateChanged(FALSE);
        }
        return;
    }
}

bool RmmPhbURCHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg,
        RIL_RadioState radioState) {
    bool reject = false;
    RFX_UNUSED(msg);
    logD(RFX_LOG_TAG, "onCheckIfRejectMessage: %d %d", radioState, reject);
    return reject;
}

void RmmPhbURCHandler::onPhbStateChanged(int isPhbReady) {
    bool isModemResetStarted =
        getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    int isSimInserted = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, FALSE);

    logI(RFX_LOG_TAG, "onPhbStateChanged slot=%d, isPhbReady=%d, isTelephonyAddOnPolicy=%d",
            m_slot_id, isPhbReady, isTelephonyAddOnPolicy);

    if (RFX_SLOT_COUNT >= 2) {
        logI(RFX_LOG_TAG, "onPhbStateChanged isSimInserted=%d, isModemResetStarted=%d",
                isSimInserted, isModemResetStarted);

        if (isSimInserted == FALSE) {
            return;
        }

        if ((isPhbReady == TRUE) && (isModemResetStarted)) {
            return;
        }
    }

    if (isPhbReady == FALSE) {
        sendEvent(RFX_MSG_EVENT_PHB_CURRENT_STORAGE_RESET,
                RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);
    }

    // if load support telephony add on policy, not send this event
#if defined(MTK_PHONEBOOK_IN_MODEM_SUPPORT)
    if (isTelephonyAddOnPolicy == false) {
        sendEvent(RFX_MSG_EVENT_PHB_STATE_CHANGED,
                RfxVoidData(), RIL_CMD_PROXY_10, m_slot_id);
    }
#endif
    // response to TeleCore
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_PHB_READY_NOTIFICATION,
            m_slot_id, RfxIntsData((void*)&isPhbReady, sizeof(int)));
    responseToTelCore(urc);
}
