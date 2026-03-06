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
#include "RmcVsimRequestHandler.h"
#include "libmtkrilutils.h"

#define RFX_LOG_TAG "RmcVsim"

int RmcVsimRequestHandler::sBeingPlugOut[MAX_SIM_COUNT] = {0};
int RmcVsimRequestHandler::sVsimMdWaiting[MAX_SIM_COUNT] = {-1, -1, -1, -1};
int RmcVsimRequestHandler::sVsimPlugInOut[MAX_SIM_COUNT] = {0};

static const int chVsimReqList[] = {
    RFX_MSG_REQUEST_SIM_VSIM_NOTIFICATION,
    RFX_MSG_REQUEST_SIM_VSIM_OPERATION,
};

static const int chVsimEventList[] = {
    RFX_MSG_EVENT_SIM_VSIM_PLUG_OUT,
    RFX_MSG_EVENT_SIM_VSIM_AUTH_REQUEST,
    RFX_MSG_EVENT_SIM_VSIM_RELEASE_AKA_SIM,
    RFX_MSG_EVENT_SIM_VSIM_SEND_ERROR_RESPONSE,
    RFX_MSG_EVENT_VSIM_RELEASE_RF,
};

RFX_IMPLEMENT_HANDLER_CLASS(RmcVsimRequestHandler, RIL_CMD_PROXY_11);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVsimEventData, RfxIntsData,
        RFX_MSG_REQUEST_SIM_VSIM_NOTIFICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVsimOpEventData, RfxIntsData,
        RFX_MSG_REQUEST_SIM_VSIM_OPERATION);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxVsimEventData, RFX_MSG_EVENT_SIM_VSIM_PLUG_OUT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_SIM_VSIM_AUTH_REQUEST);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SIM_VSIM_RELEASE_AKA_SIM);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SIM_VSIM_SEND_ERROR_RESPONSE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_VSIM_RELEASE_RF);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RmcVsimRequestHandler::RmcVsimRequestHandler(int slot_id, int channel_id):
        RfxBaseHandler(slot_id, channel_id) {
    int is_support = RfxRilUtils::isExternalSimSupport();
    logD(RFX_LOG_TAG, "RmmVsimHandler construct,slot:%d, channel:%d(%d), support:%d", slot_id,
            channel_id, RIL_CMD_PROXY_11, is_support);
    if (is_support != 0) {
        registerToHandleRequest(chVsimReqList, sizeof(chVsimReqList) / sizeof(int));
        registerToHandleEvent(chVsimEventList, sizeof(chVsimEventList) / sizeof(int));
    }
}

RmcVsimRequestHandler::~RmcVsimRequestHandler() {
}

void RmcVsimRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_SIM_VSIM_NOTIFICATION:
            handleVsimNotification(msg);
            break;
        case RFX_MSG_REQUEST_SIM_VSIM_OPERATION:
            handleVsimOperation(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Not support the request!");
            break;
    }
}

void RmcVsimRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();
    switch(event) {
        case RFX_MSG_EVENT_SIM_VSIM_PLUG_OUT:
            handleVsimNotification(msg);
            break;
        case RFX_MSG_EVENT_SIM_VSIM_AUTH_REQUEST:
            handleVsimAuthRequestEvent(msg);
            break;
        case RFX_MSG_EVENT_SIM_VSIM_RELEASE_AKA_SIM:
            handleReleaseAkaSim(msg);
            break;
        case RFX_MSG_EVENT_SIM_VSIM_SEND_ERROR_RESPONSE:
            sendVsimErrorResponse(msg);
            break;
        case RFX_MSG_EVENT_VSIM_RELEASE_RF:
            handleReleaseRf(msg);
            break;
        default:
            logD(RFX_LOG_TAG, "Not support the event %d!", event);
            break;
    }
}

void RmcVsimRequestHandler::setMdWaitingResponse(int slot_id, int waiting) {
    if (slot_id > -1 && slot_id < MAX_SIM_COUNT) {
        sVsimMdWaiting[slot_id] = waiting;
    }
}

int RmcVsimRequestHandler::getMdWaitingResponse(int slot_id) {
    if (slot_id > -1 && slot_id < MAX_SIM_COUNT) {
        return sVsimMdWaiting[slot_id];
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[VSIM] getMdWaitingResponse invalid slot id:%d", slot_id);
        return -1;
    }
}

void RmcVsimRequestHandler::setVsimPlugInOutEvent(int slot_id, int flag) {
    sVsimPlugInOut[slot_id] = flag;
}

int RmcVsimRequestHandler::getVsimPlugInOutEvent(int slot_id) {
    return sVsimPlugInOut[slot_id];
}

/**
 * [AT+ERSA AT Command Usage]
 * AT+ERSA = <msg_id>[, <parameter1> [, <parameter2> [, <\A1K>]]]
 *
 * <msg_id>:
 * 0 (APDU request)  // Send APDU execute result to MD
 *       <parameter1>: apdu_status // failure or success, 1: success, 0: failure
 *       <parameter2>: response apdu segment  // The return data from card (string)
 * 1 (event: card reset)
 *       <parameter1>: ATR // (string)
 * 2 (event: card error)
 *       <parameter1>: error cause // Not define the error code yet, currently MD only handle driver's hot swap signal (recovery is trigger by Status Word)
 * 3 (event: card hot swap out)
 * 4 (event: card hot swap in)
 */
void RmcVsimRequestHandler::handleVsimNotification(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    int err;
    String8 cmd("");
    RIL_Errno ret = RIL_E_SUCCESS;
    RIL_VsimEvent *event = (RIL_VsimEvent *)msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();
    sp<RfxMclMessage> response;
    int slot_id = msg->getSlotId();
    int mdOff = getNonSlotMclStatusManager()->getBoolValue(
            RFX_STATUS_KEY_MODEM_POWER_OFF, false);

    logD(RFX_LOG_TAG, "[VSIM]requestVsimNotification, event_Id: %d, sim_type: %d, datalen: %d",
         event->eventId, event->sim_type, datalen);

    // Check if any not response URC, if so send error response first.
    if (getMdWaitingResponse(slot_id) != VSIM_MD_WAITING_RESET) {
        if (event->eventId == REQUEST_TYPE_ENABLE_EXTERNAL_SIM ||
            event->eventId == REQUEST_TYPE_PLUG_IN) {
            sendVsimErrorResponse(msg);
        }
    }

    if (event->eventId == REQUEST_TYPE_ENABLE_EXTERNAL_SIM) {
        setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.enabled", (char*)"1");
        if (isBeingPlugOut(slot_id) == 1) {
            logW(RFX_LOG_TAG, "[VSIM]Enable vsim when it is being plug out.");
            int count = 0;
            while((true == getNonSlotMclStatusManager()->getBoolValue(
                    RFX_STATUS_KEY_CAPABILITY_SWITCH_URC_CHANNEL, false)) && count < 100) {
                usleep(500*1000);
                count++;
            }
            if (100 == count) {
                logE(RFX_LOG_TAG, "[VSIM]Switch URC channel didn't finish.");
                ret = RIL_E_GENERIC_FAILURE;
                goto done;
            }
            atSendCommand("AT+ERSA=3");
            sBeingPlugOut[slot_id] = 0;
        }
        if (RfxRilUtils::isNonDsdaRemoteSupport()) {
            if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
                rfx_property_set("vendor.gsm.prefered.rsim.slot",
                                 String8::format("%d", slot_id).string());
            }
            cmd.append("AT+EAPVSIM=1,0,1");

        } else if (RfxRilUtils::isSwitchVsimWithHotSwap()) {
            cmd.append("AT+EAPVSIM=1,1");
        } else {
            response = RfxMclMessage::obtainResponse(
                    msg->getId(), ret,
                    RfxVsimEventData((void*)event, sizeof(RIL_VsimEvent)), msg, false);
            responseToTelCore(response);
            return;
        }
    } else if (event->eventId == REQUEST_TYPE_DISABLE_EXTERNAL_SIM) {
        setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.enabled", (char*)"0");
        setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.inserted", (char*)"0");
        setExternalSimProperty(slot_id, (char*)"persist.vendor.radio.external.sim", (char*)"0");
        setExternalSimProperty(slot_id, (char*)"persist.vendor.radio.vsim.timeout", (char*)"");
        char property[RFX_PROPERTY_VALUE_MAX] = {0};
        rfx_property_get("vendor.gsm.prefered.aka.sim.slot", property, "-1");
        if (atoi(property) == slot_id) {
            rfx_property_set("vendor.gsm.prefered.aka.sim.slot", "-1");
        }
        memset(property, 0, sizeof(property));
        rfx_property_get("vendor.gsm.prefered.rsim.slot", property, "-1");
        if (atoi(property) == slot_id) {
            rfx_property_set("vendor.gsm.prefered.rsim.slot", "-1");
        }
        if (getMdWaitingResponse(slot_id) != VSIM_MD_WAITING_RESET) {
            sendVsimErrorResponse(msg);
        }
        if (isBeingPlugOut(slot_id) == 1) {
            logW(RFX_LOG_TAG, "[VSIM]Disable vsim when it is being plug out.");
            atSendCommand("AT+ERSA=3");
            sBeingPlugOut[slot_id] = 0;
        }
        if (RfxRilUtils::isNonDsdaRemoteSupport()) {
            if (RfxRilUtils::isExternalSimOnlySlot(slot_id) > 0 &&
                !RfxRilUtils::isNonDsdaRemoteSupport()) {
                cmd.append("AT+EAPVSIM=1,0,1");
            } else {
                cmd.append("AT+EAPVSIM=0,0,1");
            }
        } else if (RfxRilUtils::isSwitchVsimWithHotSwap()) {
            cmd.append("AT+EAPVSIM=0");
        } else {
            response = RfxMclMessage::obtainResponse(
                    msg->getId(), ret,
                    RfxVsimEventData((void*)event, sizeof(RIL_VsimEvent)), msg, false);
            responseToTelCore(response);
            return;
        }
    } else if (event->eventId == REQUEST_TYPE_PLUG_IN) {
        char property[RFX_PROPERTY_VALUE_MAX] = {0};
        getMSimProperty(slot_id, (char*)"vendor.gsm.external.sim.enabled", property);
        if (atoi(property) != 0) {
            setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.inserted",
                            (char*)String8::format("%d", event->sim_type).string());
        }
        sBeingPlugOut[slot_id] = 0;
        if (RfxRilUtils::isNonDsdaRemoteSupport()) {
            if (event->sim_type == SIM_TYPE_LOCAL_SIM) {
                cmd.append("AT+EAPVSIM=1,1,1");
            } else if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
                cmd.append("AT+ERSIM");
            }
        } else {
            cmd.append("AT+ERSA=4");
        }
    } else if (event->eventId == REQUEST_TYPE_PLUG_OUT) {
        if (getMdWaitingResponse(slot_id) != VSIM_MD_WAITING_RESET) {
            sendVsimErrorResponse(msg);
        }
        if (getNonSlotMclStatusManager()->getBoolValue(
                RFX_STATUS_KEY_CAPABILITY_SWITCH_URC_CHANNEL, false)) {
            logW(RFX_LOG_TAG, "[VSIM] Plug out VSIM when switching channel.");
            sBeingPlugOut[slot_id] = 1;
            goto done;
        } else {
            setExternalSimProperty(
                    slot_id, (char*)"vendor.gsm.external.sim.inserted", (char*)"0");
            cmd.append("AT+ERSA=3");
        }
    } else if (event->eventId == REQUEST_TYPE_SET_PERSIST_TYPE) {
        setExternalSimProperty(slot_id, (char*)"persist.vendor.radio.external.sim",
                        (char*)String8::format("%d", event->sim_type).string());
        goto done;
    } else if (event->eventId == REQUEST_TYPE_SET_MAPPING_INFO) {
        if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
            rfx_property_set("vendor.gsm.prefered.rsim.slot",
                             (char*)String8::format("%d", slot_id).string());
            goto done;
        } else {
            rfx_property_set("vendor.gsm.prefered.aka.sim.slot",
                             (char*)String8::format("%d", slot_id).string());
            cmd.append("AT+ERSIMAKA=1");
        }
    } else if (event->eventId == REQUEST_TYPE_RESET_MAPPING_INFO) {
        if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
            rfx_property_set("vendor.gsm.prefered.rsim.slot", "-1");
        }
        goto done;
    } else if (event->eventId == EVENT_TYPE_SEND_RSIM_AUTH_IND) {
        if (RfxRilUtils::getRsimAuthOngoing(slot_id) != 1) {
            // Start to RSIM authenication procedure, we need to occupy RF for AKA-SIM
            setVsimAuthTimeoutDuration(slot_id);
            cmd.append("AT+ERSIMAUTH=1");
            RfxRilUtils::setRsimAuthOngoing(slot_id, 1);
        } else {
            logI(RFX_LOG_TAG, "[VSIM] RSIM_AUTH_IND when RF is already occupied.");
            goto done;
        }
    } else if (event->eventId == EVENT_TYPE_RECEIVE_RSIM_AUTH_RSP) {
        if (RfxRilUtils::getRsimAuthOngoing(slot_id) == 1) {
            // Complete RSIM authenication procedure, we need to release RF for AKA-SIM
            cmd.append("AT+ERSIMAUTH=0");
            RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
        } else {
            logI(RFX_LOG_TAG, "[VSIM] RSIM_AUTH_RSP when RF is already released.");
            goto done;
        }
    } else if (event->eventId == REQUEST_TYPE_SET_TIMEOUT_TIMER) {
        setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.timeout",
                        (char*)String8::format("%d", event->sim_type).string());
        goto done;
    } else if (event->eventId == REQUEST_TYPE_SET_PERSIST_TIMEOUT) {
        setExternalSimProperty(slot_id, (char*)"persist.vendor.radio.vsim.timeout",
                        (char*)String8::format("%d", event->sim_type).string());
        goto done;
    } else if (event->eventId == EVENT_TYPE_RSIM_AUTH_DONE) {
        rfx_property_set("vendor.gsm.prefered.aka.sim.slot", "-1");
        cmd.append("AT+ERSIMAKA=0");
    } else if (event->eventId == EVENT_TYPE_EXTERNAL_SIM_CONNECTED) {
        rfx_property_set("vendor.gsm.external.sim.connected",
                         String8::format("%d", event->sim_type).string()); //reuse sim_type as connected flag
        if (event->sim_type == 0) {
            handleVsimDisconnected(msg);
        }
        goto done;
    } else {
        logE(RFX_LOG_TAG, "[VSIM] requestVsimNotification wrong event id.");
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }

    if (mdOff == 0 || ((event->eventId == EVENT_TYPE_SEND_RSIM_AUTH_IND) ||
        (event->eventId == EVENT_TYPE_RECEIVE_RSIM_AUTH_RSP))) {

        p_response = atSendCommand(cmd);
        cmd.clear();

        err = p_response->getError();
        if (err < 0) {
            logE(RFX_LOG_TAG, "[VSIM] requestVsimNotification Fail");
            ret = RIL_E_GENERIC_FAILURE;
            goto done;
        }

        if (0 == p_response->getSuccess()) {
            switch (p_response->atGetCmeError()) {
                logW(RFX_LOG_TAG, "[VSIM] requestVsimNotification p_response = %d /n",
                        p_response->atGetCmeError());
                default:
                    ret = RIL_E_GENERIC_FAILURE;
                    goto done;
            }
        }
    } else {
        logE(RFX_LOG_TAG, "[VSIM] requestVsimNotification s_md_off, return error.");
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }

done:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), ret,
            RfxVsimEventData((void*)event, sizeof(RIL_VsimEvent)), msg, false);
    responseToTelCore(response);
    logW(RFX_LOG_TAG, "[VSIM] requestVsimNotification Done");
}

void RmcVsimRequestHandler::handleVsimOperation(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    int err;
    String8 cmd("");
    int slot_id = msg->getSlotId();
    RIL_Errno ret = RIL_E_SUCCESS;

    RIL_VsimOperationEvent *response = (RIL_VsimOperationEvent *)msg->getData()->getData();
    sp<RfxMclMessage> rsp;

    if (getMdWaitingResponse(slot_id) == VSIM_MD_WAITING_RESET) {
        ret = RIL_E_GENERIC_FAILURE;
        logW(RFX_LOG_TAG, "[VSIM] requestVsimOperation no urc waiting.");
        goto done;
    }

    if (response->eventId == MSG_ID_UICC_RESET_RESPONSE &&
        getMdWaitingResponse(slot_id) == VSIM_MD_WAITING_ATR) {
        cmd.append(String8::format("AT+ERSA=1, %d, \"%s\"",
                   ((response->result < 0) ? 0 : 1), response->data));
        setMdWaitingResponse(slot_id, VSIM_MD_WAITING_RESET);
    } else if (response->eventId == MSG_ID_UICC_APDU_RESPONSE &&
               getMdWaitingResponse(slot_id) == VSIM_MD_WAITING_APDU) {
        cmd.append(String8::format("AT+ERSA=0, \"%s\"", response->data));
        setMdWaitingResponse(slot_id, VSIM_MD_WAITING_RESET);
    } else if (response->eventId == MSG_ID_UICC_TEST_MODE_REQUEST) {
        RfxVsimUtils::setVerifiedVsimClient(RfxVsimUtils::checkIsTestVsimComponent(
                (unsigned char *)response->data,
                strlen(response->data)));
        goto done;
    } else {
        logW(RFX_LOG_TAG, "[VSIM]requestVsimOperation, eventId not support: %d",
             response->eventId);
        goto done;
    }

    p_response = atSendCommand(cmd);
    cmd.clear();
    err = p_response->getError();
    if (err < 0) {
        logE(RFX_LOG_TAG, "[VSIM] requestVsimOperation Fail");
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }

    if (0 == p_response->getSuccess()) {
        switch (p_response->atGetCmeError()) {
            logE(RFX_LOG_TAG, "[VSIM] requestVsimOperation p_response = %d /n",
                    p_response->atGetCmeError());
            default:
                ret = RIL_E_GENERIC_FAILURE;
                goto done;
        }
    }

done:
    rsp = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxIntsData(), msg, false);
    responseToTelCore(rsp);
}

void RmcVsimRequestHandler::handleVsimDisconnected(const sp<RfxMclMessage>& /*msg*/) {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        int waiting_response = getMdWaitingResponse(i);
        if (waiting_response == VSIM_MD_WAITING_APDU) {
            sendEvent(RFX_MSG_EVENT_SIM_VSIM_SEND_ERROR_RESPONSE,
                    RfxVoidData(), RIL_CMD_PROXY_11, i);
        }
        int auth_ongoing = RfxRilUtils::getRsimAuthOngoing(i);
        int aka_slot = getAkaSimSlot();
        if (auth_ongoing == 1 && i == aka_slot) {
            sendEvent(RFX_MSG_EVENT_VSIM_RELEASE_RF, RfxVoidData(), RIL_CMD_PROXY_11, i);
        }
        logI(RFX_LOG_TAG,
                "[VSIM] handleVsimDisconnected sim:%d, aka:%d, waiting:%d, authOngoing:%d", i,
                aka_slot, waiting_response, auth_ongoing);
    }
}

void RmcVsimRequestHandler::sendVsimErrorResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxAtResponse> p_response;
    int err;
    String8 cmd("");
    int slot_id = msg->getSlotId();

    logE(RFX_LOG_TAG, "[VSIM] sendVsimErrorResponse modem waiting:%d",
         getMdWaitingResponse(slot_id));

    if (getMdWaitingResponse(slot_id) == VSIM_MD_WAITING_RESET) {
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }
    logE(RFX_LOG_TAG, "[VSIM]sendVsimErrorResponse2 %d",
         getMdWaitingResponse(slot_id));

    if (getMdWaitingResponse(slot_id) == VSIM_MD_WAITING_ATR) {
        cmd.append(String8::format("AT+ERSA=%d, 0, \"0000\"", VSIM_MD_WAITING_ATR));
    } else if (getMdWaitingResponse(slot_id) == VSIM_MD_WAITING_APDU) {
        cmd.append(String8::format("AT+ERSA=%d,\"0000\"", VSIM_MD_WAITING_APDU));
    } else {
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }
    logE(RFX_LOG_TAG, "[VSIM]sendVsimErrorResponse3");
    setMdWaitingResponse(slot_id, VSIM_MD_WAITING_RESET);

    p_response = atSendCommand(cmd);
    cmd.clear();
    err = p_response->getError();
    if (err < 0) {
        logE(RFX_LOG_TAG, "[VSIM] sendVsimErrorResponse Fail");
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }

    if (0 == p_response->getSuccess()) {
        switch (p_response->atGetCmeError()) {
            logW(RFX_LOG_TAG, "[VSIM] sendVsimErrorResponse p_response = %d /n",
                 p_response->atGetCmeError());
            default:
                ret = RIL_E_GENERIC_FAILURE;
                goto done;
        }
    }
done:
    logD(RFX_LOG_TAG, "[VSIM] sendVsimErrorResponse ret:%d", ret);
}

int RmcVsimRequestHandler::isSingeleRfRemoteSimSupport() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("ro.vendor.mtk_non_dsda_rsim_support", property_value, "0");
    return atoi(property_value);
}

void RmcVsimRequestHandler::setVsimAuthTimeoutDuration(int slot_id) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxAtResponse> p_response;
    String8 cmd("");
    char vsim_timeout_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    int err;

    getMSimProperty(slot_id, (char*)"vendor.gsm.external.sim.timeout",
            vsim_timeout_prop);

    int timeOut = (RfxVsimUtils::isInternalTestVsimComponent() == 1) ?
            (atoi(vsim_timeout_prop) + 1) : 6;

    /* AT+ERSIMATO  - Sets the RSIM Authentication Time Out value
     * AT+ERSIMATO=<RSIM_Auth_Time_Out>
     *
     * < RSIM_Auth_Time_Out > Integer type: Timer value in seconds
     *
     * success: OK
     * fail: ERROR
     *
     * Timer T with the set value started on AT+ERSIMAUTH = <Start>
     * Expects the AT+ERSIMAUTH = <Finish/Abort> before the expiry of the timer T.
     * If Timer T expired before Finish/Abort then URC +ERSIMAUTH: 0 sent on AKASIM to indicate abort of Auth procedure.
     */
    cmd.append(String8::format("AT+ERSIMATO=%d", timeOut));

    p_response = atSendCommand(cmd);
    cmd.clear();
    err = p_response->getError();

    if (err < 0) {
        logE(RFX_LOG_TAG, "[VSIM] setVsimAuthTimeoutDuration Fail");
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }

    if (0 == p_response->getSuccess()) {
        switch (p_response->atGetCmeError()) {
            logW(RFX_LOG_TAG, "[VSIM] setVsimAuthTimeoutDuration p_response = %d /n",
                 p_response->atGetCmeError());
            default:
                ret = RIL_E_GENERIC_FAILURE;
                goto done;
        }
    }

done:
    logD(RFX_LOG_TAG, "[VSIM] sendVsimErrorResponse ret:%d", ret);
}

void RmcVsimRequestHandler::handleVsimAuthRequestEvent(const sp<RfxMclMessage>& msg) {
    char* urc = (char *) msg->getData()->getData();
    String8 ss(urc);
    RIL_VsimOperationEvent event;
    int event_type;
    int err = 0;
    RfxAtLine *pLine = NULL;
    int slot_id = msg->getSlotId();

    memset(&event, 0, sizeof(event));
    pLine = new RfxAtLine(urc, NULL);
//    logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: urc = %s", urc);

    if (strStartsWith(urc, "+ERSAAUTH:")) {
        logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: +ERSAAUTH");
        /**
             * [+ERSAATUH URC Command Usage]
             * +ERSAAUTH: <type>, "APDU"
             *
             * <type>:
             * 0 (start authenication procedure)
             */
        pLine->atTokStart(&err);
        logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: err %d", err);
        if (err < 0) {
            goto done;
        }
        event_type = pLine->atTokNextint(&err);
        logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: err2 %d", err);
        if (err < 0) {
            logE(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent get type fail!");
            goto done;
        }

        if (RfxRilUtils::isVsimEnabledBySlot(slot_id) == 0 && (event_type == 0)) {
            sendVsimErrorResponse(msg);
            logW(RFX_LOG_TAG, "[VSIM] handleVsimAuthRequestEvent received URC during vsim disabled.");
            delete(pLine);
            return;
        }
        logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: event_type %d", event_type);

        switch (event_type) {
            case 0:
                // Authencation procedure, notify ExternalSimMgr to occupy AKA-SIM's RF
                /* Mark out for the RsimAuthOngoing is set to AKA SIM. Set it to RSIM may cause error
                 * if switch the AKA SIM and RSIM next time for this state can't be reset at RSIM.
                 */
                //RfxRilUtils::setRsimAuthOngoing(slot_id, 1);
                event.eventId = MSG_ID_UICC_AUTHENTICATION_REQUEST_IND;

                if(pLine->atTokHasmore()){
                    event.data = pLine->atTokNextstr(&err);
                    if(err < 0) goto done;

                    event.data_length = strlen(event.data);
                }
                break;

            default:
                logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: +ERSAAUTH unsupport type");
        }
    }

done:
    logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: eventId = %d", event.eventId);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_VSIM_OPERATION_INDICATION,
                                                       slot_id,
                                                       RfxVsimOpIndicationData((void*)&event,
                                                       sizeof(event)));
    delete(pLine);
    responseToTelCore(unsol);
}

void RmcVsimRequestHandler::handleReleaseAkaSim(const sp<RfxMclMessage>& /*msg*/) {
    sp<RfxAtResponse> p_response;
    String8 cmd("");
    char akasim_prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("vendor.gsm.prefered.aka.sim.slot", akasim_prop, "-1");

    if (atoi(akasim_prop) != -1) {
        rfx_property_set("vendor.gsm.prefered.aka.sim.slot", "-1");
        cmd.append("AT+ERSIMAKA=0");

        p_response = atSendCommand(cmd);
        cmd.clear();
        //err = p_response->getError();
    }
}

void RmcVsimRequestHandler::handleReleaseRf(const sp<RfxMclMessage>& msg) {
    int aka_slot = getAkaSimSlot();
    int slot_id = msg->getSlotId();
    if (aka_slot == slot_id && RfxRilUtils::getRsimAuthOngoing(slot_id) == 1) {
        atSendCommand("AT+ERSIMAUTH=0");
        RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
    } else {
        logI(RFX_LOG_TAG, "[VSIM]handleReleaseRf when RF is already released, aka:%d", aka_slot);
    }
}

int RmcVsimRequestHandler::getAkaSimSlot() {
    char akasim_prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("vendor.gsm.prefered.aka.sim.slot", akasim_prop, "-1");
    return atoi(akasim_prop);
}

int RmcVsimRequestHandler::isBeingPlugOut(int slot_id) {
    return sBeingPlugOut[slot_id];
}

// External SIM [End]
