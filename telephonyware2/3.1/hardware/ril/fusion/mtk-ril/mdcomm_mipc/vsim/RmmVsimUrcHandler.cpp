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

// External SIM [Start]
/*****************************************************************************
 * Include
 *****************************************************************************/
#include <prop/properties.h>
#include <telephony/mtk_ril.h>
#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxLog.h"
#include "RfxRilUtils.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxVsimEventData.h"
#include "RfxVsimOpEventData.h"
#include "RfxVsimOpIndicationData.h"
#include "RfxVsimUtils.h"
#include "RmmVsimRequestHandler.h"
#include "RmmVsimUrcHandler.h"
#include "libmtkrilutils.h"

#define RFX_LOG_TAG "RmmVsim"

RFX_REGISTER_DATA_TO_URC_ID(RfxVsimOpIndicationData, RFX_MSG_URC_SIM_VSIM_OPERATION_INDICATION);

RFX_IMPLEMENT_HANDLER_CLASS(RmmVsimUrcHandler, RIL_CMD_PROXY_URC);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmmVsimUrcHandler::RmmVsimUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int urc[] = {
        MIPC_SIM_VSIM_RESET_IND,
        MIPC_SIM_VSIM_APDU_IND,
        MIPC_SIM_VSIM_EVENT_IND,
    };
    logI(RFX_LOG_TAG, "RmmVsimUrcHandler init slot: %d, ch id %d", slot_id, channel_id);
    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
}

RmmVsimUrcHandler::~RmmVsimUrcHandler() {
}

void RmmVsimUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "empty urc");
        return;
    }
    RIL_VsimOperationEvent oper_event;
    int urc = msg->getMipcData()->getMipcMsgId();
    int slot_id = msg->getSlotId();
    int event_type = 0;
    RFX_LOG_D(RFX_LOG_TAG, "onHandleUrc %s", msg->toString().string());

    memset(&oper_event, 0, sizeof(oper_event));
    switch (urc) {
        case MIPC_SIM_VSIM_RESET_IND:
            RmmVsimRequestHandler::setMdWaitingResponse(slot_id, VSIM_MD_WAITING_ATR);
            RfxVsimUtils::setVerifiedVsimClient(0);
            oper_event.eventId = MSG_ID_UICC_RESET_REQUEST;
            if (!RfxRilUtils::isVsimClientConnected()) {
                RIL_VsimEvent event;
                event.transaction_id = 0;
                event.eventId = REQUEST_TYPE_PLUG_OUT;
                event.sim_type = 0;
                sendEvent(RFX_MSG_EVENT_SIM_VSIM_PLUG_OUT,
                          RfxVsimEventData(&event, sizeof(RIL_VsimEvent)),
                          RIL_CMD_PROXY_11, slot_id);
                RFX_LOG_W(RFX_LOG_TAG,
                     "[VSIM]handleUrc: recevied VSIM reset under VSIM client disconnected. %d",
                     RFX_MSG_EVENT_SIM_VSIM_PLUG_OUT);
                return;
            }
            break;
        case MIPC_SIM_VSIM_APDU_IND:
            oper_event.eventId = MSG_ID_UICC_APDU_REQUEST;
            oper_event.data = msg->getMipcData()->getMipcStr(MIPC_SIM_VSIM_APDU_IND_T_DATA);
            oper_event.data_length = strlen(oper_event.data);
            RmmVsimRequestHandler::setMdWaitingResponse(slot_id, VSIM_MD_WAITING_APDU);
            break;
        case MIPC_SIM_VSIM_EVENT_IND:
            event_type = msg->getMipcData()->getMipcInt8Val(MIPC_SIM_VSIM_EVENT_IND_T_EVENT, 0);
            if (0 == event_type) {
                RmmVsimRequestHandler::setMdWaitingResponse(slot_id, VSIM_MD_WAITING_APDU);
                sendEvent(RFX_MSG_EVENT_SIM_VSIM_AUTH_REQUEST,
                        RfxStringData(msg->getMipcData()->getMipcStr(
                                MIPC_SIM_VSIM_EVENT_IND_T_DATA)),
                        RIL_CMD_PROXY_11, slot_id);
            } else if(1 == event_type) {
                char* auth_state = msg->getMipcData()->getMipcStr(MIPC_SIM_VSIM_EVENT_IND_T_DATA);
                if (strcmp(auth_state, "done") == 0) {
                    RFX_LOG_D(RFX_LOG_TAG, "[VSIM]handleVsimAuthAckEvent: auth done");
                    RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
                    oper_event.eventId = MSG_ID_UICC_AUTHENTICATION_DONE_IND;
                } else if (strcmp(auth_state, "abort") == 0) {
                    RFX_LOG_D(RFX_LOG_TAG, "[VSIM]handleVsimAuthAckEvent: auth abort");
                    oper_event.eventId = MSG_ID_UICC_AUTHENTICATION_ABORT_IND;
                    char rsim_prop[RFX_PROPERTY_VALUE_MAX] = {0};
                    rfx_property_get("vendor.gsm.prefered.rsim.slot", rsim_prop, "-1");
                    int rsim_slot = atoi(rsim_prop);

                    RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
                    if (rsim_slot > -1 && rsim_slot < MAX_SIM_COUNT) {
                        RfxRilUtils::setRsimAuthOngoing(rsim_slot, 0);
                    }
                } else {
                    RFX_LOG_E(RFX_LOG_TAG, "Not support the auth state:%s", auth_state);
                }
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "Not support the type:%d", event_type);
                return;
            }
            break;
        default:
            RFX_LOG_D(RFX_LOG_TAG, "Not support the urc id %d!", urc);
            return;
    }
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(
            RFX_MSG_URC_SIM_VSIM_OPERATION_INDICATION,
            slot_id,
            RfxVsimOpIndicationData((void*)&oper_event,
                                    sizeof(oper_event)));

    responseToTelCore(unsol);
}


// External SIM [End]
