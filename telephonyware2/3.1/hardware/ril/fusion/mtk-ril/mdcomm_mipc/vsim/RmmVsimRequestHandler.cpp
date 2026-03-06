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

#define VSIM_NO_RESPONSE_TIMEOUT_DURATION "13"
#define VSIM_MODEM_CAPABILITY "3"

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

int RmmVsimRequestHandler::sVsimMdWaiting[MAX_SIM_COUNT] = {-1, -1, -1, -1};
int RmmVsimRequestHandler::sVsimPlugInOut[MAX_SIM_COUNT] = {0};

RFX_IMPLEMENT_HANDLER_CLASS(RmmVsimRequestHandler, RIL_CMD_PROXY_11);

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

RmmVsimRequestHandler::RmmVsimRequestHandler(int slot_id, int channel_id):
        RfxBaseHandler(slot_id, channel_id) {
    int is_support = RfxRilUtils::isExternalSimSupport();
    logD(RFX_LOG_TAG, "RmmVsimRequestHandler construct,slot:%d, channel:%d(%d), support:%d",
            slot_id, channel_id, RIL_CMD_PROXY_11, is_support);
    if (is_support != 0) {
        registerToHandleRequest(chVsimReqList, sizeof(chVsimReqList) / sizeof(int));
        registerToHandleEvent(chVsimEventList, sizeof(chVsimEventList) / sizeof(int));
        initVsimConfiguration();
        requestSetAkaSim();
        requestSwitchExternalSim();
    }
}

RmmVsimRequestHandler::~RmmVsimRequestHandler() {
}

void RmmVsimRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
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

void RmmVsimRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
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

void RmmVsimRequestHandler::requestSetAkaSim() {
//    char vsim_enabled_prop[RFX_PROPERTY_VALUE_MAX] = {0};
//    char vsim_inserted_prop[RFX_PROPERTY_VALUE_MAX] = {0};
//    char persist_vsim_inserted_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    char rsim_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    char akasim_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    sp<RfxMipcData> initMipc = NULL;
    sp<RfxMipcData> result = NULL;

    rfx_property_get("vendor.gsm.prefered.rsim.slot", rsim_prop, "-1");
    rfx_property_get("vendor.gsm.prefered.aka.sim.slot", akasim_prop, "-1");

    if (atoi(rsim_prop) == -1 || atoi(akasim_prop) != m_slot_id) {
        logE(RFX_LOG_TAG, "[VSIM] requestSetAkaSim rsim=%d, aka-sim=%d",
                atoi(rsim_prop), atoi(akasim_prop));
        return;
    }

    /* If rsim enabled, then should set aka sim protocol in case of it haven't be disabled. */
    if (atoi(akasim_prop) >= 0) {
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_SET_AKA_REQ, m_slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_SET_AKA_REQ_T_MODE, (uint8_t)1);
        result = callToMipcMsgSync(initMipc);
    }
}

void RmmVsimRequestHandler::requestSwitchExternalSim() {
    char vsim_enabled_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    char vsim_inserted_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    char persist_vsim_inserted_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    char rsim_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    sp<RfxMipcData> initMipc = NULL;
    sp<RfxMipcData> result = NULL;

    getMSimProperty(m_slot_id, (char*)"vendor.gsm.external.sim.enabled", vsim_enabled_prop);
    getMSimProperty(m_slot_id, (char*)"vendor.gsm.external.sim.inserted", vsim_inserted_prop);
    rfx_property_get("vendor.gsm.prefered.rsim.slot", rsim_prop, "-1");

    if (RfxRilUtils::isPersistExternalSimDisabled()) {
        rfx_property_set("persist.vendor.radio.external.sim", "");
    } else {
        getMSimProperty(m_slot_id, (char*)"persist.vendor.radio.external.sim",
                persist_vsim_inserted_prop);
    }

    if (atoi(persist_vsim_inserted_prop) > 0) {
        logD(RFX_LOG_TAG, "[VSIM] persist.vendor.radio.external.sim is 1.");
        setExternalSimProperty(m_slot_id, (char*)"vendor.gsm.external.sim.inserted",
                (char*)"1");
    }

    if (atoi(persist_vsim_inserted_prop) > 0
            || (atoi(vsim_enabled_prop) > 0 && atoi(vsim_inserted_prop) > 0)) {
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_ENABLE_REQ, m_slot_id);
        result = callToMipcMsgSync(initMipc);
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_PLUG_REQ, m_slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_MODE, (uint8_t)0);
        if (atoi(rsim_prop) == m_slot_id) {
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE, (uint8_t)1); //REMOTE_SIM
        } else {
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE, (uint8_t)0); //LOCAL_SIM
        }
        setVsimPlugInOutEvent(m_slot_id, VSIM_TRIGGER_PLUG_IN);
        result = callToMipcMsgSync(initMipc);
    } else {
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_DISABLE_REQ, m_slot_id);
        result = callToMipcMsgSync(initMipc);
    }
}

void RmmVsimRequestHandler::initVsimConfiguration() {
    char persist[RFX_PROPERTY_VALUE_MAX] = { 0 };
    int simCount = RfxRilUtils::rfxGetSimCount();
    int remoteSimSlot = RfxRilUtils::getRemoteSimSlot();
    int akaSimSlot = RfxRilUtils::getAkaSimSlot();
    if (remoteSimSlot >= simCount || akaSimSlot >= simCount) {
        rfx_property_set("vendor.gsm.prefered.rsim.slot", "-1");
        rfx_property_set("vendor.gsm.prefered.aka.sim.slot", "-1");
        setExternalSimProperty(remoteSimSlot, (char*)"vendor.gsm.external.sim.enabled", (char*)"0");
        setExternalSimProperty(remoteSimSlot, (char*)"vendor.gsm.external.sim.inserted",
                (char*)"0");
        if (RfxRilUtils::isVsimEnabledBySlot(akaSimSlot) == 1) {
            setExternalSimProperty(akaSimSlot, (char*)"vendor.gsm.external.sim.enabled",
                    (char*)"0");
            setExternalSimProperty(akaSimSlot, (char*)"vendor.gsm.external.sim.inserted",
                    (char*)"0");
        }
    }

    getMSimProperty(m_slot_id, (char*)"persist.vendor.radio.external.sim", persist);
    persist[RFX_PROPERTY_VALUE_MAX - 1] = '\0';

    if (strcmp("1", persist) == 0) {
        setExternalSimProperty(m_slot_id, (char*)"vendor.gsm.external.sim.enabled", (char*)"1");
    }
    setExternalSimProperty(m_slot_id, (char*)"vendor.gsm.external.sim.timeout",
            (char*)VSIM_NO_RESPONSE_TIMEOUT_DURATION);
    setExternalSimProperty(m_slot_id, (char*)"vendor.gsm.modem.vsim.capability",
            (char*)VSIM_MODEM_CAPABILITY);
}

void RmmVsimRequestHandler::setVsimPlugInOutEvent(int slot_id, int flag) {
    sVsimPlugInOut[slot_id] = flag;
}

int RmmVsimRequestHandler::getVsimPlugInOutEvent(int slot_id) {
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
void RmmVsimRequestHandler::handleVsimNotification(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    RIL_VsimEvent *event = (RIL_VsimEvent *)msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();
    sp<RfxMclMessage> response;
    sp<RfxMipcData> initMipc = NULL;
    sp<RfxMipcData> result = NULL;
    int slot_id = msg->getSlotId();

    logD(RFX_LOG_TAG,
            "[%d]requestVsimNotification, event_id:%d, sim_type:%d, datalen:%d, msg:%s",
            slot_id, event->eventId, event->sim_type, datalen,
            msg->toString().string());
    if (event->eventId == REQUEST_TYPE_ENABLE_EXTERNAL_SIM) {
        setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.enabled", (char*)"1");
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_ENABLE_REQ, slot_id);
        result = callToMipcMsgSync(initMipc);
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
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_DISABLE_REQ, slot_id);
        result = callToMipcMsgSync(initMipc);
    } else if (event->eventId == REQUEST_TYPE_PLUG_IN) {
        char property[RFX_PROPERTY_VALUE_MAX] = {0};
        getMSimProperty(slot_id, (char*)"vendor.gsm.external.sim.enabled", property);
        if (atoi(property) != 0) {
            setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.inserted",
                            (char*)String8::format("%d", event->sim_type).string());
        }
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_PLUG_REQ, slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_MODE, (uint8_t)0);
        if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE, (uint8_t)1); //REMOTE_SIM
        } else {
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE, (uint8_t)0); //LOCAL_SIM
        }
        setVsimPlugInOutEvent(slot_id, VSIM_TRIGGER_PLUG_IN);
        result = callToMipcMsgSync(initMipc);
    } else if (event->eventId == REQUEST_TYPE_PLUG_OUT) {
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_PLUG_REQ, slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_MODE, (uint8_t)1);
        if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE, (uint8_t)1); //REMOTE_SIM
        } else {
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE, (uint8_t)0); //LOCAL_SIM
        }
        setVsimPlugInOutEvent(slot_id, VSIM_TRIGGER_PLUG_OUT);
        result = callToMipcMsgSync(initMipc);
    } else if (event->eventId == REQUEST_TYPE_SET_PERSIST_TYPE) {
        setExternalSimProperty(slot_id, (char*)"persist.vendor.radio.external.sim",
                        (char*)String8::format("%d", event->sim_type).string());
    } else if (event->eventId == REQUEST_TYPE_SET_MAPPING_INFO) {
        if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
            rfx_property_set("vendor.gsm.prefered.rsim.slot",
                             (char*)String8::format("%d", slot_id).string());
        } else {
            rfx_property_set("vendor.gsm.prefered.aka.sim.slot",
                             (char*)String8::format("%d", slot_id).string());
            initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_SET_AKA_REQ, slot_id);
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_SET_AKA_REQ_T_MODE, (uint8_t)1);
            result = callToMipcMsgSync(initMipc);
        }
    } else if (event->eventId == REQUEST_TYPE_RESET_MAPPING_INFO) {
        if (event->sim_type == SIM_TYPE_REMOTE_SIM) {
            rfx_property_set("vendor.gsm.prefered.rsim.slot", "-1");
        }
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_SET_AKA_REQ, slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_SET_AKA_REQ_T_MODE, (uint8_t)0);
        result = callToMipcMsgSync(initMipc);
    } else if (event->eventId == EVENT_TYPE_SEND_RSIM_AUTH_IND) {
        if (RfxRilUtils::getRsimAuthOngoing(slot_id) != 1) {
            // Start to RSIM authenication procedure, we need to occupy RF for AKA-SIM
            setVsimAuthTimeoutDuration(slot_id);
            initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_AUTH_REQ, slot_id);
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_AUTH_REQ_T_MODE, (uint8_t)1);
            result = callToMipcMsgSync(initMipc);
            RfxRilUtils::setRsimAuthOngoing(slot_id, 1);
        } else {
            RFX_LOG_I(RFX_LOG_TAG, "[VSIM] RSIM_AUTH_IND when RF is already occupied.");
        }
    } else if (event->eventId == EVENT_TYPE_RECEIVE_RSIM_AUTH_RSP) {
        if (RfxRilUtils::getRsimAuthOngoing(slot_id) == 1) {
            // Complete RSIM authenication procedure, we need to release RF for AKA-SIM
            initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_AUTH_REQ, slot_id);
            initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_AUTH_REQ_T_MODE, (uint8_t)0);
            result = callToMipcMsgSync(initMipc);
            RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
        } else {
            RFX_LOG_I(RFX_LOG_TAG, "[VSIM] RSIM_AUTH_RSP when RF is already released.");
        }
    } else if (event->eventId == REQUEST_TYPE_SET_TIMEOUT_TIMER) {
        setExternalSimProperty(slot_id, (char*)"vendor.gsm.external.sim.timeout",
                        (char*)String8::format("%d", event->sim_type).string());
    } else if (event->eventId == REQUEST_TYPE_SET_PERSIST_TIMEOUT) {
        setExternalSimProperty(slot_id, (char*)"persist.vendor.radio.vsim.timeout",
                        (char*)String8::format("%d", event->sim_type).string());
    } else if (event->eventId == EVENT_TYPE_RSIM_AUTH_DONE) {
        rfx_property_set("vendor.gsm.prefered.aka.sim.slot", "-1");
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_SET_AKA_REQ, slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_SET_AKA_REQ_T_MODE, (uint8_t)0);
        result = callToMipcMsgSync(initMipc);
    } else if (event->eventId == EVENT_TYPE_EXTERNAL_SIM_CONNECTED) {
        //reuse sim_type as connected flag
        rfx_property_set("vendor.gsm.external.sim.connected",
                         String8::format("%d", event->sim_type).string());
        if (event->sim_type == 0) {
            handleVsimDisconnected(msg);
        }
    } else {
        logE(RFX_LOG_TAG, "[VSIM] requestVsimNotification wrong event id.");
        ret = RIL_E_GENERIC_FAILURE;
    }
    if (result != NULL) {
        ret = (RIL_Errno)result->getResult();
    }
    response = RfxMclMessage::obtainResponse(
            msg->getId(), ret,
            RfxVsimEventData((void*)event, sizeof(RIL_VsimEvent)), msg, false);
    responseToTelCore(response);
    logD(RFX_LOG_TAG, "[VSIM] requestVsimNotification Done");
}

void RmmVsimRequestHandler::handleVsimOperation(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    int slot_id = msg->getSlotId();
    RIL_VsimOperationEvent *response = (RIL_VsimOperationEvent *)msg->getData()->getData();
    sp<RfxMclMessage> rsp;
    sp<RfxMipcData> initMipc = NULL;
    sp<RfxMipcData> result = NULL;

    if (response->eventId == MSG_ID_UICC_RESET_RESPONSE) {
        setMdWaitingResponse(slot_id, VSIM_MD_WAITING_RESET);
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_RESET_REQ, slot_id);
        initMipc->addMipcTlvUint32(MIPC_SIM_VSIM_RESET_REQ_T_RESULT,
                ((response->result < 0) ? 0 : 1));
        initMipc->addMipcTlvUint32(MIPC_SIM_VSIM_RESET_REQ_T_LENGTH, strlen(response->data) + 1);
        initMipc->addMipcTlvStr(MIPC_SIM_VSIM_RESET_REQ_T_DATA, (response->data));
        result = callToMipcMsgSync(initMipc);
    } else if (response->eventId == MSG_ID_UICC_APDU_RESPONSE) {
        setMdWaitingResponse(slot_id, VSIM_MD_WAITING_RESET);
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_APDU_REQ, slot_id);
        initMipc->addMipcTlvUint32(MIPC_SIM_VSIM_APDU_REQ_T_LENGTH, strlen(response->data) + 1);
        initMipc->addMipcTlvStr(MIPC_SIM_VSIM_APDU_REQ_T_DATA, (response->data));
        result = callToMipcMsgSync(initMipc);
    } else if (response->eventId == MSG_ID_UICC_TEST_MODE_REQUEST) {
        RfxVsimUtils::setVerifiedVsimClient(RfxVsimUtils::checkIsTestVsimComponent(
                (unsigned char *)response->data,
                strlen(response->data)));
    } else {
        logD(RFX_LOG_TAG, "[VSIM]requestVsimOperation, eventId not support: %d",
             response->eventId);
    }

    if (result != NULL) {
        ret = (RIL_Errno)result->getResult();
        RFX_LOG_I(RFX_LOG_TAG, "[VSIM]requestVsimOperation, ret:%d", ret);
    }
    rsp = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxIntsData(), msg, false);
    responseToTelCore(rsp);
}

void RmmVsimRequestHandler::handleVsimDisconnected(const sp<RfxMclMessage>& /*msg*/) {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        int waiting_response = getMdWaitingResponse(i);
        if (waiting_response == VSIM_MD_WAITING_APDU) {
            sendEvent(RFX_MSG_EVENT_SIM_VSIM_SEND_ERROR_RESPONSE, RfxVoidData(),
                      RIL_CMD_PROXY_11, i);
        }
        int auth_ongoing = RfxRilUtils::getRsimAuthOngoing(i);
        int aka_slot = getAkaSimSlot();
        if (auth_ongoing == 1 && i == aka_slot) {
            sendEvent(RFX_MSG_EVENT_VSIM_RELEASE_RF, RfxVoidData(), RIL_CMD_PROXY_11, i);
        }
        RFX_LOG_I(RFX_LOG_TAG,
                "[VSIM] handleVsimDisconnected sim:%d, aka:%d, waiting:%d, authOngoing:%d", i,
                aka_slot, waiting_response, auth_ongoing);
    }
}

void RmmVsimRequestHandler::setMdWaitingResponse(int slot_id, int waiting) {
    if (slot_id > -1 && slot_id < MAX_SIM_COUNT) {
        sVsimMdWaiting[slot_id] = waiting;
    }
}

int RmmVsimRequestHandler::getMdWaitingResponse(int slot_id) {
    if (slot_id > -1 && slot_id < MAX_SIM_COUNT) {
        return sVsimMdWaiting[slot_id];
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[VSIM] getMdWaitingResponse invalid slot id:%d", slot_id);
        return -1;
    }
}

void RmmVsimRequestHandler::sendVsimErrorResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_SUCCESS;
    sp<RfxMipcData> initMipc = NULL;
    sp<RfxMipcData> result = NULL;
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
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_RESET_REQ, slot_id);
        initMipc->addMipcTlvUint32(MIPC_SIM_VSIM_RESET_REQ_T_RESULT, 0);
        initMipc->addMipcTlvUint32(MIPC_SIM_VSIM_RESET_REQ_T_LENGTH, 5);
        initMipc->addMipcTlvStr(MIPC_SIM_VSIM_RESET_REQ_T_DATA, "0000");
        result = callToMipcMsgSync(initMipc);
        ret = (RIL_Errno)(result->getResult());
    } else if (getMdWaitingResponse(slot_id) == VSIM_MD_WAITING_APDU) {
        initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_APDU_REQ, slot_id);
        initMipc->addMipcTlvUint32(MIPC_SIM_VSIM_APDU_REQ_T_LENGTH, 5);
        initMipc->addMipcTlvStr(MIPC_SIM_VSIM_APDU_REQ_T_DATA, "0000");
        result = callToMipcMsgSync(initMipc);
        ret = (RIL_Errno)(result->getResult());
    } else {
        ret = RIL_E_GENERIC_FAILURE;
        goto done;
    }
    logE(RFX_LOG_TAG, "[VSIM]sendVsimErrorResponse3");
    setMdWaitingResponse(slot_id, VSIM_MD_WAITING_RESET);

done:
    logD(RFX_LOG_TAG, "[VSIM] sendVsimErrorResponse ret:%d", ret);
}

int RmmVsimRequestHandler::isSingeleRfRemoteSimSupport() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("ro.vendor.mtk_non_dsda_rsim_support", property_value, "0");
    return atoi(property_value);
}

void RmmVsimRequestHandler::setVsimAuthTimeoutDuration(int slot_id) {
    char vsim_timeout_prop[RFX_PROPERTY_VALUE_MAX] = {0};

    getMSimProperty(slot_id, (char*)"vendor.gsm.external.sim.timeout",
            vsim_timeout_prop);

    int timeOut = (RfxVsimUtils::isInternalTestVsimComponent() == 1) ?
            (atoi(vsim_timeout_prop) + 1) : 6;

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_SET_TIMER_REQ, slot_id);
    initMipc->addMipcTlvUint32(MIPC_SIM_VSIM_SET_TIMER_REQ_T_TIMER, (uint32_t)timeOut);
    sp<RfxMipcData> result = callToMipcMsgSync(initMipc);

    logD(RFX_LOG_TAG, "[VSIM] sendVsimErrorResponse ret:%d", result->getResult());
}

void RmmVsimRequestHandler::handleVsimAuthRequestEvent(const sp<RfxMclMessage>& msg) {
    RIL_VsimOperationEvent oper_event;
    int slot_id = msg->getSlotId();

    memset(&oper_event, 0, sizeof(oper_event));
    if (RfxRilUtils::isVsimEnabledBySlot(slot_id) == 0) {
        sendVsimErrorResponse(msg);
        logW(RFX_LOG_TAG,
                "[VSIM] handleVsimAuthRequestEvent received URC during vsim disabled.");
        return;
    }
    setMdWaitingResponse(slot_id, VSIM_MD_WAITING_APDU);
    oper_event.eventId = MSG_ID_UICC_AUTHENTICATION_REQUEST_IND;
    oper_event.data = (char*)(msg->getData()->getData());
    oper_event.data_length = strlen(oper_event.data);
    logW(RFX_LOG_TAG, "[VSIM]handleVsimAuthRequestEvent: eventId = %d", oper_event.eventId);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_VSIM_OPERATION_INDICATION,
                                                       slot_id,
                                                       RfxVsimOpIndicationData((void*)&oper_event,
                                                       sizeof(oper_event)));
    responseToTelCore(unsol);
}

void RmmVsimRequestHandler::handleReleaseAkaSim(const sp<RfxMclMessage>& msg) {
    char akasim_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    int slot_id = msg->getSlotId();
    rfx_property_get("vendor.gsm.prefered.aka.sim.slot", akasim_prop, "-1");

    if (atoi(akasim_prop) != -1) {
        rfx_property_set("vendor.gsm.prefered.aka.sim.slot", "-1");

        sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_SET_AKA_REQ, slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_SET_AKA_REQ_T_MODE, (uint8_t)0);
        sp<RfxMipcData> result = callToMipcMsgSync(initMipc);
    }
}

void RmmVsimRequestHandler::handleReleaseRf(const sp<RfxMclMessage>& msg) {
    int aka_slot = getAkaSimSlot();
    int slot_id = msg->getSlotId();
    if (aka_slot == slot_id && RfxRilUtils::getRsimAuthOngoing(slot_id) == 1) {
        sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SIM_VSIM_AUTH_REQ, slot_id);
        initMipc->addMipcTlvUint8(MIPC_SIM_VSIM_AUTH_REQ_T_MODE, (uint8_t)0);
        sp<RfxMipcData> result = callToMipcMsgSync(initMipc);
        RfxRilUtils::setRsimAuthOngoing(slot_id, 0);
    } else {
        RFX_LOG_I(RFX_LOG_TAG, "[VSIM]handleReleaseRf when RF is already released, aka:%d",
                aka_slot);
    }
}

int RmmVsimRequestHandler::getAkaSimSlot() {
    char akasim_prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("vendor.gsm.prefered.aka.sim.slot", akasim_prop, "-1");
    return atoi(akasim_prop);
}


// External SIM [End]
