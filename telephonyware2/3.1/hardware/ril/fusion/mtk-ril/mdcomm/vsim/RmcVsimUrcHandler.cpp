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
#include "RmcVsimRequestHandler.h"
#include "RfxVsimUtils.h"
#include "RmcVsimUrcHandler.h"
#include "libmtkrilutils.h"

#define RFX_LOG_TAG "RmcVsim"

RFX_REGISTER_DATA_TO_URC_ID(RfxVsimOpIndicationData, RFX_MSG_URC_SIM_VSIM_OPERATION_INDICATION);

RFX_IMPLEMENT_HANDLER_CLASS(RmcVsimUrcHandler, RIL_CMD_PROXY_URC);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcVsimUrcHandler::RmcVsimUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {
        "+ERSAIND:",
        "+ERSAAUTH:",
        "+ERSIMAUTH:",
    };
    logI(RFX_LOG_TAG, "RmcVsimUrcHandler init slot: %d, ch id %d", slot_id, channel_id);
   registerToHandleURC(urc, sizeof(urc)/sizeof(char *));
}

RmcVsimUrcHandler::~RmcVsimUrcHandler() {
}

void RmcVsimUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    String8 ss(msg->getRawUrc()->getLine());
    if (strStartsWith(ss, "+ERSAIND:")) {
        handleVsimEventDetected(msg);
    } else if (strStartsWith(ss, "+ERSAAUTH:") ||
               strStartsWith(ss, "+ERSIMAUTH:")) {
        handleVsimAuthEventDetected(msg);
    }
}

void RmcVsimUrcHandler::handleVsimAuthEventDetected(const sp<RfxMclMessage>& msg) {
    // In case of receving +ERSAAUTH event, we need to occupy RF in AKA-SIM slot. (stack 2 in modem)
    RIL_VsimOperationEvent event;
    int event_type = 0;
    RfxAtLine *line = msg->getRawUrc();
    char rsim_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    char akasim_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    String8 ss(line->getLine());
    int err = 0;
    int slotId = 0;
    int slot_id = msg->getSlotId();

    memset(&event, 0, sizeof(event));
//    RFX_LOG_D(RFX_LOG_TAG, "[VSIM]handleVsimAuthEventDetected: ss = %s", (char*)line->getLine());

    if (strStartsWith(ss, "+ERSAAUTH:")) {
        RmcVsimRequestHandler::setMdWaitingResponse(slot_id, VSIM_MD_WAITING_APDU);
        sendEvent(RFX_MSG_EVENT_SIM_VSIM_AUTH_REQUEST,
                RfxStringData((char*)line->getLine()),
                  RIL_CMD_PROXY_11, slot_id);
        return;
    } else if (strStartsWith(ss, "+ERSIMAUTH:")) {
        /**
         * [+ERSIMAUTH URC Command Usage]
         * +ERSAAUTH: <type>
         *
         * <type>:
         * 0 (abort authenication procedure)
         * 1 (complete authenication procedure)
         */

        line->atTokStart(&err);
        if (err < 0) {
            goto done;
        }

        event_type = line->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[VSIM]onVsimAuthEventDetected get type fail!");
            goto done;
        }

        switch (event_type) {
            case 0:
                // Abort authenication, send notify to Jave layer directly
                RFX_LOG_D(RFX_LOG_TAG, "[VSIM]handleVsimAuthAckEvent: auth abort");
                rfx_property_get("vendor.gsm.prefered.rsim.slot", rsim_prop, "-1");

                if (atoi(rsim_prop) == -1) {
                    goto done;
                }
                slotId = atoi(rsim_prop);

                RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
                RfxRilUtils::setRsimAuthOngoing(slotId, 0);
                event.eventId = MSG_ID_UICC_AUTHENTICATION_ABORT_IND;
                break;
            case 1:
                // Complete authenication, send notify to Jave layer directly
                RFX_LOG_D(RFX_LOG_TAG, "[VSIM]handleVsimAuthAckEvent: auth abort");
                RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
                event.eventId = MSG_ID_UICC_AUTHENTICATION_DONE_IND;
                break;
            default:
                RFX_LOG_D(RFX_LOG_TAG, "[VSIM]onVsimAuthEventDetected: +ERSIMAUTH: unsupport type");
        }

        if (RfxVsimUtils::isInternalTestVsimComponent() == 0) {
            // Release AKA SIM stack
            rfx_property_get("vendor.gsm.prefered.aka.sim.slot", akasim_prop, "-1");

            if (atoi(akasim_prop) == -1) {
                goto done;
            }
            sendEvent(RFX_MSG_EVENT_SIM_VSIM_RELEASE_AKA_SIM, RfxVoidData(),
                    RIL_CMD_PROXY_11, slot_id);
        }
    }

done:
    RFX_LOG_D(RFX_LOG_TAG, "[VSIM]onVsimAuthEventDetected: eventId = %d", event.eventId);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_VSIM_OPERATION_INDICATION,
                                                       slot_id,
                                                       RfxVsimOpIndicationData((void*)&event,
                                                                               sizeof(event)));
    responseToTelCore(unsol);
}

void RmcVsimUrcHandler::handleVsimEventDetected(const sp<RfxMclMessage>& msg) {
    RIL_VsimOperationEvent event;
    int event_type;
    int err = 0;
    RfxAtLine *line = msg->getRawUrc();
    int slot_id = msg->getSlotId();

    memset(&event, 0, sizeof(event));
    // 256 bytes for data + 5 bytes header information: CLA,INS,P1,P2,P3 */
    //  #define APDU_REQ_MAX_LEN (261)
    // 256 bytes for data + 2 bytes status word SW1 and SW2 */
    //  #define APDU_RSP_MAX_LEN (258)
    //  --> Need to *2, since the received data is hex string.

    /**
     * [+ERSAIND URC Command Usage]
     * +ERSAIND: <msg_id>[, <parameter1> [, <parameter2> [, <\A1K>]]]
     *
     * <msg_id>:
     * 0 (APDU indication) // Send APDU to AP
     *       <parameter1>: command apdu // APDU data need send to card (string)
     * 1 (card reset indication)
     * 2 (power-down indication)
     *       <parameter1>: mode // Not define mode yet, [MTK]0 is default
     * 3 (vsim trigger plug out)
     * 4 (vsim trigger plug in)
     * 5 (tray plug out when vsim has been enabled)
     * 6 (tray plug in when vsim has been enabled)
     */

    line->atTokStart(&err);
    if (err < 0) {
        goto done;
    }

    event_type = line->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[VSIM]onVsimEventDetected get type fail!");
        goto done;
    }

    if ((RfxRilUtils::isVsimEnabledBySlot(slot_id) == 0 ||
        RmcVsimRequestHandler::isBeingPlugOut(slot_id) == 1) &&
        (event_type == 0 || event_type == 1)) {
        RmcVsimRequestHandler::setMdWaitingResponse(slot_id, event_type);
        sendEvent(RFX_MSG_EVENT_SIM_VSIM_SEND_ERROR_RESPONSE, RfxVoidData(),
                RIL_CMD_PROXY_11, slot_id);
        RFX_LOG_W(RFX_LOG_TAG, "[VSIM] onVsimEventDetected received URC during vsim disabled.");
        return;
    }

    switch (event_type) {
        case 0:
            event.eventId = MSG_ID_UICC_APDU_REQUEST; //REQUEST_TYPE_APDU_EVENT;

            if (line->atTokHasmore()) {
                event.data = line->atTokNextstr(&err);
                if (err < 0) {
                    RFX_LOG_E(RFX_LOG_TAG, "[VSIM]onVsimEventDetected get string fail!");
                    goto done;
                }
                event.data_length = strlen(event.data);
             }
            RmcVsimRequestHandler::setMdWaitingResponse(slot_id, VSIM_MD_WAITING_APDU);
            break;
        case 1:
            RFX_LOG_D(RFX_LOG_TAG, "[VSIM]onVsimEventDetected: card reset request");
            RmcVsimRequestHandler::setMdWaitingResponse(slot_id, VSIM_MD_WAITING_ATR);
            event.eventId = MSG_ID_UICC_RESET_REQUEST; //REQUEST_TYPE_ATR_EVENT;
            RfxVsimUtils::setVerifiedVsimClient(0);
            if (!RfxRilUtils::isVsimClientConnected()) {
                RIL_VsimEvent event;
                event.transaction_id = 0;
                event.eventId = REQUEST_TYPE_PLUG_OUT;
                event.sim_type = 0;
                sendEvent(RFX_MSG_EVENT_SIM_VSIM_PLUG_OUT,
                          RfxVsimEventData(&event, sizeof(RIL_VsimEvent)),
                          RIL_CMD_PROXY_11, slot_id);
                RFX_LOG_W(RFX_LOG_TAG,
                     "[VSIM]onVsimEventDetected: recevied VSIM reset under VSIM client disconnected. %d",
                     RFX_MSG_EVENT_SIM_VSIM_PLUG_OUT);
                return;
            }
            break;
        case 2:
            RFX_LOG_D(RFX_LOG_TAG, "[VSIM]onVsimEventDetected: card power down");
            RmcVsimRequestHandler::setMdWaitingResponse(slot_id, VSIM_MD_WAITING_RESET);
            return;
            /*event.eventId = REQUEST_TYPE_CARD_POWER_DOWN;
            event.result = 0;   //Use result filed to record mode information
            if (line->atTokHasmore()) {
                 event.result = line->atTokNextint(&err);
                 if (err < 0) {
                     RFX_LOG_E(RFX_LOG_TAG, "[VSIM]onVsimEventDetected get power down mode fail!");
                     goto done;
                 }
            }
            break;*/
        case 3:
            RFX_LOG_D(RFX_LOG_TAG, "[VSIM]onVsimEventDetected: VSIM_TRIGGER_PLUG_OUT");
            RmcVsimRequestHandler::setVsimPlugInOutEvent(slot_id, VSIM_TRIGGER_PLUG_OUT);
            return;
        case 4:
            RFX_LOG_D(RFX_LOG_TAG, "[VSIM]onVsimEventDetected: VSIM_TRIGGER_PLUG_IN");
            RmcVsimRequestHandler::setVsimPlugInOutEvent(slot_id, VSIM_TRIGGER_PLUG_IN);
            return;
        case 5:
            event.eventId = MSG_ID_UICC_TRAY_PLUG_OUT_WHEN_VSIM_ENABLED;
            break;
        case 6:
            event.eventId = MSG_ID_UICC_TRAY_PLUG_IN_WHEN_VSIM_ENABLED;
            break;
        default:
            RFX_LOG_D(RFX_LOG_TAG, "[VSIM]onVsimEventDetected: unsupport type");
            return;
    }

done:
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_VSIM_OPERATION_INDICATION,
                                                       slot_id,
                                                       RfxVsimOpIndicationData((void*)&event,
                                                                               sizeof(event)));
    responseToTelCore(unsol);
}


// External SIM [End]
