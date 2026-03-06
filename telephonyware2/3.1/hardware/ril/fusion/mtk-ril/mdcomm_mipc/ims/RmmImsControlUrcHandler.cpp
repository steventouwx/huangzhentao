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

#include <mipc_msg.h>
#include "RmmImsControlUrcHandler.h"
#include "RfxSipRegInfoData.h"
#include "RfxEiregData.h"
#include "RfxEregrtData.h"
#include "RfxMipcData.h"
#include <telephony/mtk_ril.h>
#include <vector>

#include "rfx_properties.h"
#include <string>

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmImsControlUrcHandler, RIL_CMD_PROXY_URC);

// register data
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_IMS_REGISTRATION_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_SIP_CALL_PROGRESS_INDICATOR);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_IMS_RTP_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_VOPS_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxSipRegInfoData, RFX_MSG_UNSOL_SIP_REG_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxImsRegInfoData, RFX_MSG_UNSOL_IMS_REGISTRATION_STATE_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxEiregData, RFX_MSG_UNSOL_EIREG_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxEregrtData, RFX_MSG_UNSOL_EREGRT_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RIL_MSG_UNSOL_IMS_REG_FLAG_IND);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_IMS_REPORT_EIREG);

RmmImsControlUrcHandler::RmmImsControlUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmmImsControlUrcHandler constructor");

    const int urc[] = {
        MIPC_IMS_STATE_IND, //"+CIREGU:", "+EWFC:","+EIMSREGURI", "+EIMSREGRESP"
        MIPC_CALL_SIP_IND, //"+ESIPCPI",
        MIPC_IMS_REG_IND, //"+EIREG",
        //MIPC_IMS_VOPS_IND, //"+CIREPI",
        //MIPC_IMS_SIP_REG_INFO_IND, //"+ESIPREGINFO",
        //MIPC_IMS_REG_REMAIN_TIME_IND, //"+EREGRT",
        MIPC_IMS_UI_IND, //"+EIMSUI"
    };

    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));

    const int EventList[] = {
        RFX_MSG_EVENT_IMS_REPORT_EIREG,
    };
    registerToHandleEvent(EventList, sizeof(EventList)/sizeof(int));
}

RmmImsControlUrcHandler::~RmmImsControlUrcHandler() {
}

void RmmImsControlUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    uint16_t mipcMsgId = (int) msg->getMipcData()->getMipcMsgId();
    logD(RFX_LOG_TAG, "onHandleUrc: mipc message %04x", mipcMsgId);
    uint8_t eventType;

    if (mipcMsgId == MIPC_IMS_STATE_IND) {
        eventType = msg->getMipcData()->getMipcUint8Val(MIPC_IMS_STATE_IND_T_EVENT, 0);
        if (eventType == MIPC_IMS_STATE_IND_EVENT_REG_STATE) { //"+CIREGU:"
            handleImsRegistrationInfo(msg);
        } else if (eventType == MIPC_IMS_STATE_IND_EVENT_WFC) { //"+EWFC:"
            handleWfcStateChanged(msg);
        } else if (eventType == MIPC_IMS_STATE_IND_EVENT_REG_URI) { //"+EIMSREGURI"
            handleImsRegInfoInd(msg);
        } else if (eventType == MIPC_IMS_STATE_IND_EVENT_REG_RESP) { //"+EIMSREGRESP"
            handleImsRegRespInd(msg);
        } else {
            logE(RFX_LOG_TAG, "onHandleUrc: mipc eventType wrong: %d", eventType);
        }
    } else if (mipcMsgId == MIPC_IMS_REG_IND) { //+EIREG
        handleImsEiregInfo(msg);
    } else if (mipcMsgId == MIPC_CALL_SIP_IND) { //+ESIPCPI
        handleSipMsgIndication(msg);
    } else if (mipcMsgId == MIPC_IMS_SIP_REG_INFO_IND) { //+ESIPREGINFO
        handleSipRegInfoInd(msg);
    } else if (mipcMsgId == MIPC_IMS_REG_REMAIN_TIME_IND) { //++EREGRT
        handleEregrtInfo(msg);
    } else if (mipcMsgId == MIPC_IMS_VOPS_IND) { //+CIREPI
        handleImsVopsIndication(msg);
    } else if (mipcMsgId == MIPC_IMS_UI_IND) {
        handleImsRegFlagInd(msg);
    } else {
        logE(RFX_LOG_TAG, "msg %d Should not be here", mipcMsgId);
    }
}

void RmmImsControlUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();
    switch(event) {
        case RFX_MSG_EVENT_IMS_REPORT_EIREG:
            handleEventReportEireg(msg);
            break;
        default:
            logD(RFX_LOG_TAG, "onHandleEvent, Not support the event %d!", event);
            break;
    }
}

void RmmImsControlUrcHandler::handleEventReportEireg(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    /* +EIREG= <reg_state>,<reg_type>[,<ext_info>,<dereg_cause>,<ims_retry>,<rat>,<sip_uri_type>] */
    RIL_EiregData response;

    response.reg_state = 0;
    response.reg_type = 0;
    response.ext_info = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_STATE, 0);
    if (response.ext_info > 0) {
        response.reg_state = 1;
    }
    response.dereg_cause = 0;
    response.ims_retry = 0;
    response.rat = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_RAT, 10);
    response.sip_uri_type = 0;

    logD(RFX_LOG_TAG, "handleEventReportEireg, +EIREG:%d,%d,%d,%d,%d,%d,%d",
            response.reg_state, response.reg_type, response.ext_info, response.dereg_cause,
            response.ims_retry, response.rat, response.sip_uri_type);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_EIREG_INFO_IND,
            m_slot_id, RfxEiregData(&response, sizeof(RIL_EiregData)));
    responseToTelCore(urc);
}

void RmmImsControlUrcHandler::handleImsRegistrationInfo(const sp<RfxMclMessage>& msg) {
    int response[3] = { 0 };
    sp<RfxMclMessage> urc;
    sp<RfxMclMessage> urc2;
    int wfcState;
    const int WFC_STATE_ON = 1 << 4;

    RfxMipcData* urc_data = msg->getMipcData();
    uint8_t reg_state = urc_data->getMipcUint8Val(MIPC_IMS_STATE_IND_T_REG_STATE, 0);
    uint32_t ext_info = urc_data->getMipcUint32Val(MIPC_IMS_STATE_IND_T_EXT_INFO, 0);

    // get reg_info
    response[0] = (int)reg_state;

    // get ext_info , value range is 1~FFFFFFFF
    if (response[0] == 0) {
        // report mode is 1 , no ext_info available
        // set invalid value 0 for upper layer to distinguish if ext_info is availble or not
        response[1] = 0;
    } else if (response[0] == 1) {
        response[1] = (int)ext_info;
    } else {
        logD(RFX_LOG_TAG, "handleImsRegistrationInfo, ignore: +CIREGU:%d,%x",
            response[0], ext_info);
        return;
    }

    logI(RFX_LOG_TAG, "handleImsRegistrationInfo, +CIREGU:%d,%x",
           reg_state, ext_info);

    // Fix DSDS bug : transfer rid socket information to IMS Service
    // to judgement for valid sim/phone id
    response[2] = m_slot_id;

    // 93MD IMS framework can NOT get ims RAN type, transfer EWFC state
    // to it. Indicate current RAN type IWLAN or not.
    wfcState = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_STATE, -1);
    logD(RFX_LOG_TAG, "handleImsRegistrationInfo, get WFC state : %d", wfcState);
    if (wfcState == 1 && response[0] != 0) {
        // enable VoWifi bit in ext_info if wfc state was 1
        response[1] |= WFC_STATE_ON;
    }

    /// M: ALPS03862134 Add for hVoLTE case.
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_STATE, response[1]);

    if (response[1] > 0) {
        bool ignoreIms =
            getMclStatusManager()->getString8Value(RFX_STATUS_KEY_IGNORE_VOLTE_VILTE_FOR_23G,
                                                   String8("false")) == "true";
        if (ignoreIms) {
            RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
            RfxNwServiceState serviceState = getMclStatusManager()
                    ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
            int dataRadioTech = serviceState.getRilDataRadioTech();
            int dataRegState = serviceState.getRilDataRegState();
            if ((dataRadioTech != RADIO_TECH_LTE && dataRadioTech != RADIO_TECH_LTE_CA
            && dataRadioTech != RADIO_TECH_NR && dataRadioTech != RADIO_TECH_IWLAN && wfcState != 1)
                && (dataRegState == 1 || dataRegState == 5)) {
                response[1] = (response[1] & 0xF6);
                if (response[1] == 0) {
                    response[0] = 0;
                }
                logD(RFX_LOG_TAG, "handleImsRegistrationInfo: <4G:%d,%d changed CIREGU:%d,%d",
                    dataRadioTech, dataRegState, response[0], response[1]);
            }
        }
    }

    // MTK defined UNSOL EVENT (with IMS data info attached)
    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_IMS_REGISTRATION_INFO,
            m_slot_id, RfxIntsData(response, 3));
    responseToTelCore(urc);
    // for AOSP defined UNSOL EVENT (no data)
    urc2 = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(urc2);
}

void RmmImsControlUrcHandler::handleWfcStateChanged(const sp<RfxMclMessage>& msg) {
    // +EWFC:
    RfxMipcData* urc_data = msg->getMipcData();
    int wfcState = (int) urc_data->getMipcUint8Val(MIPC_IMS_STATE_IND_T_WFC, 0);

    if (wfcState == 3) {
        // modem notify the VoData is used, consider it as VoWiFi case
        wfcState = 1;
    }
    logI(RFX_LOG_TAG, "handleWfcStateChanged, +EWFC:%d", wfcState);

    sendEvent(RFX_MSG_EVENT_WFC_STATE, RfxIntsData(&wfcState, 1), RIL_CMD_PROXY_3, m_slot_id);

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_WFC_STATE, wfcState);
}

void RmmImsControlUrcHandler::handleImsRegInfoInd(const sp<RfxMclMessage>& msg) {
    /*  + EIMSREGURI:<account_id>,<URI>,<expire_time>  */
    sp<RfxMclMessage> unsol;
    RIL_ImsRegInfo response;
    memset(&response, 0, sizeof(RIL_ImsRegInfo));

    RfxMipcData* urc_data = msg->getMipcData();

    // got account id
    uint32_t account_id = urc_data->getMipcUint32Val(MIPC_IMS_STATE_IND_T_ACCOUNT_ID, 0);
    response.account_id = (int)account_id;

    // got uri
    uint16_t val_len = 0;
    char* uri = (char*)(urc_data->getMipcVal(MIPC_IMS_STATE_IND_T_URI, &val_len));
    char *uri_string = (char*)calloc(1, sizeof(char)*(val_len + 1));
    RFX_ASSERT(uri_string != NULL);
    memcpy(uri_string, uri, val_len);
    uri_string[val_len] = '\0';
    response.uri = uri_string;

    // got expire time
    uint32_t expire_time = urc_data->getMipcUint32Val(MIPC_IMS_STATE_IND_T_EXPIRE_TIME, 0);
    response.expire_time = (int)expire_time;

    if (response.expire_time == 0) {
        response.report_type = IMS_REGISTERING;
    } else {
        response.report_type = IMS_REGISTERED;
    }

    logD(RFX_LOG_TAG,
        "handleImsRegInfoInd: +EIMSREGURI:account_id:%d, uri:%.3s, expire_time %d, report_type:%d",
        account_id, uri, expire_time, response.report_type);
    unsol = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_IMS_REGISTRATION_STATE_IND, m_slot_id,
            RfxImsRegInfoData(&response, sizeof(RIL_ImsRegInfo)));
    responseToTelCore(unsol);

    free(uri_string);
}

void RmmImsControlUrcHandler::handleImsRegRespInd(const sp<RfxMclMessage>& msg) {
    /* + EIMSREGRESP:<account_id>,<URI>,<errCode>,<ErrorMessage> */
    sp<RfxMclMessage> unsol;
    RIL_ImsRegInfo response;
    memset(&response, 0, sizeof(RIL_ImsRegInfo));

    RfxMipcData* urc_data = msg->getMipcData();

    // got account id
    uint32_t account_id = urc_data->getMipcUint32Val(MIPC_IMS_STATE_IND_T_ACCOUNT_ID, 0);
    response.account_id = (int)account_id;

    // got uri
    uint16_t val_len = 0;
    char* uri = (char*)(urc_data->getMipcVal(MIPC_IMS_STATE_IND_T_URI, &val_len));
    char *uri_string = (char*)calloc(1, sizeof(char)*(val_len + 1));
    RFX_ASSERT(uri_string != NULL);
    memcpy(uri_string, uri, val_len);
    uri_string[val_len] = '\0';
    response.uri = uri_string;

    // got error code
    uint32_t error_code = urc_data->getMipcUint32Val(MIPC_IMS_STATE_IND_T_ERROR_CODE, 0);
    response.error_code = (int) error_code;

    // got error message
    uint16_t len = 0;
    char* error_msg = (char*)(urc_data->getMipcVal(MIPC_IMS_STATE_IND_T_ERROR_MESSAGE, &len));
    response.error_msg = error_msg;

    response.report_type = IMS_REGISTER_FAIL;

    logD(RFX_LOG_TAG,
        "handleImsRegRespInd: +EIMSREGRESP:account_id:%d, uri:%.5s, error_code:%d, error_msg:%s",
        account_id, uri, error_code, error_msg);

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_IMS_REGISTRATION_STATE_IND, m_slot_id,
            RfxImsRegInfoData(&response, sizeof(RIL_ImsRegInfo)));
    responseToTelCore(unsol);
    free(uri_string);
}

void RmmImsControlUrcHandler::handleSipMsgIndication(const sp<RfxMclMessage>& msg) {
    /*
    * +ESIPCPI: <call_id>,<dir>,<SIP_msg_type>,<method>,<response_code>[,<reason_text>]
    * <call_id>: 0 = incoming call; 1~32 = call id
    * <SIP_msg_type>: 0 = request; 1 = response
    * <method>: 1~32 and mapping to INVITE, PRACK, UPDATE, ACK, CANCEL, BYE, REFER, OK
    * <response_code>: 0-only used when SIP_msg_type is 0(request), else 100~600
    * [<reason_text>]: Optional, The text in the SIP response reason header.
    */
    //const int maxLen = 6;
    int rfxMsg = RFX_MSG_UNSOL_SIP_CALL_PROGRESS_INDICATOR;
    //bool appendPhoneId = true;
    RfxMipcData* urc_data = msg->getMipcData();

    uint32_t call_id = urc_data->getMipcUint32Val(MIPC_CALL_SIP_IND_T_CALLID, 0);
    uint32_t dir = urc_data->getMipcUint32Val(MIPC_CALL_SIP_IND_T_DIRECTION, 0);
    uint32_t sip_msg_type = urc_data->getMipcUint32Val(MIPC_CALL_SIP_IND_T_MSG_TYPE, 0);
    uint32_t method = urc_data->getMipcUint32Val(MIPC_CALL_SIP_IND_T_METHOD, 0);
    uint32_t response_code = urc_data->getMipcUint32Val(MIPC_CALL_SIP_IND_T_RESPONSE_CODE, 0);
    char *reason_text = urc_data->getMipcStr(MIPC_CALL_SIP_IND_T_REASON_TEXT);

    String8 callIdStr = String8::format("%d", call_id);
    String8 dirStr = String8::format("%d", dir);
    String8 sipMsgTypeStr = String8::format("%d", sip_msg_type);
    String8 methodStr = String8::format("%d", method);
    String8 responseCodeStr = String8::format("%d", response_code);
    if (reason_text == NULL) {
        reason_text = (char*)"";
    }
    const char *data[] = {callIdStr, dirStr, sipMsgTypeStr, methodStr, responseCodeStr, reason_text};

    logD(RFX_LOG_TAG,
        "handleSipMsgIndication: +ESIPCPI:%s,%s,%s,%s,%s,%s",
        data[0], data[1], data[2], data[3], data[4], data[5]);

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id,
        RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
    responseToTelCore(unsol);
}

void RmmImsControlUrcHandler::handleImsEiregInfo(const sp<RfxMclMessage>& msg) {
    /* +EIREG: <reg_state>,<reg_type>[,<ext_info>,<dereg_cause>,<ims_retry>,<rat>,<sip_uri_type>] */
    sp<RfxMclMessage> urc;
    RIL_EiregData response;


    RfxMipcData* urc_data = msg->getMipcData();

    // get reg_state
    response.reg_state = (int)(urc_data->getMipcUint8Val(MIPC_IMS_REG_IND_T_REG_STATE, 0));

    // get reg_type
    response.reg_type = (int)(urc_data->getMipcUint8Val(MIPC_IMS_REG_IND_T_REG_TYPE, 0));

    // get ext_info , value range is 1~FFFFFFFF
    response.ext_info = (int)(urc_data->getMipcUint32Val(MIPC_IMS_REG_IND_T_EXT_INFO, 0));

    // get dereg_cause
    response.dereg_cause = (int)(urc_data->getMipcUint8Val(MIPC_IMS_REG_IND_T_DEREG_CAUSE, 0));

    // get ims_retry
    response.ims_retry = (int)(urc_data->getMipcUint8Val(MIPC_IMS_REG_IND_T_IMS_RETRY, 0));

    /** value type: integer
     * 0: LTE
     * 1: WIFI
     * 2: EHRPD
     * 3: GSM
     * 4: UMTS
     * 5: NR
     * 6: Data_1
     * 7: Data_2
     * 8: Data_3
     * 9: Data_4
     */
    // get rat
    response.rat = (int)(urc_data->getMipcUint8Val(MIPC_IMS_REG_IND_T_RAT, 0));

    // Add for VoData feature
    if (response.reg_type == 0) {
        if (response.reg_state == 1) {
            // update rat if success to register.
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_RAT, response.rat);
            /// M: ALPS03862134 Add for hVoLTE case.
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_STATE, response.ext_info);

            bool isInWfcState = response.rat == 1 || (response.rat > 5 && response.rat < 10);
            if (response.ext_info > 0) {
                bool ignoreIms =
                    getMclStatusManager()->getString8Value(RFX_STATUS_KEY_IGNORE_VOLTE_VILTE_FOR_23G,
                                                           String8("false")) == "true";
                if (ignoreIms) {
                    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
                    RfxNwServiceState serviceState = getMclStatusManager()
                            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
                    int dataRadioTech = serviceState.getRilDataRadioTech();
                    int dataRegState = serviceState.getRilDataRegState();
                    if ((dataRadioTech != RADIO_TECH_LTE && dataRadioTech != RADIO_TECH_LTE_CA
                    && dataRadioTech != RADIO_TECH_NR && dataRadioTech != RADIO_TECH_IWLAN  && !isInWfcState)
                        && (dataRegState == 1 || dataRegState == 5)) {
                        response.ext_info = (response.ext_info & 0xF6);
                        if (response.ext_info == 0) {
                            response.reg_state = 0;
                        }
                        logD(RFX_LOG_TAG, "handleImsEiregInfo, +EIREG: <4G:%d,%d changed CIREGU:%d,%d",
                            dataRadioTech, dataRegState, response.reg_state, response.ext_info);
                    }
                }
            }
        } else {
            // update rat -1 if fail to register.
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_RAT, -1);
            /// M: ALPS03862134 Add for hVoLTE case.
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_STATE, 0);
        }
    }

    // get sip_uri_type
    response.sip_uri_type = (int)(urc_data->getMipcUint8Val(MIPC_IMS_REG_IND_T_SIP_URI_TYPE, 0));

    logI(RFX_LOG_TAG, "handleImsEiregInfo, +EIREG:%d,%d,%d,%d,%d,%d,%d",
            response.reg_state, response.reg_type, response.ext_info, response.dereg_cause,
            response.ims_retry, response.rat, response.sip_uri_type);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_EIREG_INFO_IND,
            m_slot_id, RfxEiregData(&response, sizeof(RIL_EiregData)));
    responseToTelCore(urc);
}

void RmmImsControlUrcHandler::handleImsVopsIndication(const sp<RfxMclMessage>& msg) {
    RfxMipcData* urc_data = msg->getMipcData();
    int val = (int)(urc_data->getMipcUint8Val(MIPC_IMS_VOPS_IND_T_NWIMSVOPS, 0));
    std::vector<int> params;
    params.push_back(val);

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_VOPS, params[0]);

    logD(RFX_LOG_TAG, "handleImsVopsIndication, +CIREPI:%d",
            getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS));

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
        RFX_MSG_UNSOL_VOPS_INDICATION, m_slot_id, RfxIntsData(params.data(), (int)params.size()));
    responseToTelCore(urc);
}

void RmmImsControlUrcHandler::handleSipRegInfoInd(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> urc;
    RIL_SipRegInfo response;

    // +ESIPREGINFO : <account_id>,"<direction>","<SIP_msg_type>","<method>",<response_code>,
    // "<reason_phrase>","<warn_text>"

    RfxMipcData* urc_data = msg->getMipcData();
    // get account id
    response.account_id = (int)(urc_data->getMipcUint32Val(MIPC_IMS_SIP_REG_INFO_IND_T_ACCOUNT_ID, 0));

    // get direction
    response.direction = urc_data->getMipcStr(MIPC_IMS_SIP_REG_INFO_IND_T_DIRECTION);

    // get SIP_msg_type
    response.sip_msg_type = urc_data->getMipcStr(MIPC_IMS_SIP_REG_INFO_IND_T_SIP_MSG_TYPE);

    // get method
    response.method = urc_data->getMipcStr(MIPC_IMS_SIP_REG_INFO_IND_T_METHOD);

    // get response_code
    response.response_code =
        (int)(urc_data->getMipcUint32Val(MIPC_IMS_SIP_REG_INFO_IND_T_RESPONSE_CODE, 0));

    // get reason_phrase
    response.reason_phrase = urc_data->getMipcStr(MIPC_IMS_SIP_REG_INFO_IND_T_REASON_PHRASE);

    // get warn_text
    response.warn_text = urc_data->getMipcStr(MIPC_IMS_SIP_REG_INFO_IND_T_WARN_TEXT);

    uint32_t cseq = urc_data->getMipcUint32Val(MIPC_IMS_SIP_REG_INFO_IND_T_CSEQ, 0);
    uint32_t reason_cause = urc_data->getMipcUint32Val(MIPC_IMS_SIP_REG_INFO_IND_T_REASON_CAUSE, 0);
    char cseqStr[20] = { 0 };
    char causeStr[20] = { 0 };
    if (sprintf(cseqStr, "%d", cseq) < 0) {
        logE(RFX_LOG_TAG, "handleSipRegInfoInd, sprintf to cseq fail");
        response.cseq = NULL;
    } else {
        response.cseq = cseqStr;
    }
    if (sprintf(causeStr, "%d", reason_cause) < 0) {
        logE(RFX_LOG_TAG, "handleSipRegInfoInd, sprintf to reason_cause fail");
        response.reason_cause = NULL;
    } else {
        response.reason_cause = causeStr;
    }

    response.reason_text = urc_data->getMipcStr(MIPC_IMS_SIP_REG_INFO_IND_T_REASON_TEXT);
    response.callId = urc_data->getMipcStr(MIPC_IMS_SIP_REG_INFO_IND_T_CALL_ID);

    logD(RFX_LOG_TAG, "handleSipRegInfoInd, +ESIPREGINFO:%d,%.3s,%.3s,%s,%d,%.5s,%.5s,%s,%.5s,%.5s,%.5s",
            response.account_id, response.direction, response.sip_msg_type, response.method,
            response.response_code, response.reason_phrase, response.warn_text, response.cseq,
            response.reason_cause, response.reason_text, response.callId);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SIP_REG_INFO,
            m_slot_id, RfxSipRegInfoData(&response, sizeof(RIL_SipRegInfo)));
    responseToTelCore(urc);
}



void RmmImsControlUrcHandler::handleEregrtInfo(const sp<RfxMclMessage>& msg) {
    /* +EREGRT: <rereg_flag>,<rereg_remain_time>,<resub_flag>,<resub_remain_time> */

    RIL_EregrtInfo response;
    RfxMipcData* urc_data = msg->getMipcData();

    mipc_msg_tlv_t *val_ptr = mipc_msg_get_tlv(
        urc_data->getData(),
        MIPC_IMS_REG_REMAIN_TIME_IND_T_REG_REMAIN_TIME);

    if (val_ptr != NULL) {
        // get rereg_flag
        response.rereg_flag= 1;
        // get rereg_remain_time
        response.rereg_remain_time =
            (int)(urc_data->getMipcUint32Val(MIPC_IMS_REG_REMAIN_TIME_IND_T_REG_REMAIN_TIME, 0));
    } else {
        response.rereg_flag= 0;
        response.rereg_remain_time = 0;
    }

    val_ptr = mipc_msg_get_tlv(
        urc_data->getData(),
        MIPC_IMS_REG_REMAIN_TIME_IND_T_SUB_REMAIN_TIME);
    if (val_ptr != NULL) {
        // get resub_flag
        response.resub_flag = 1;
        // get resub_remain_time
        response.resub_remain_time =
            (int)(urc_data->getMipcUint32Val(MIPC_IMS_REG_REMAIN_TIME_IND_T_SUB_REMAIN_TIME, 0));
    } else {
        response.resub_flag = 0;
        response.resub_remain_time = 0;
    }

    logI(RFX_LOG_TAG, "handleEregrtInfo:rereg_flag<%d>rereg_remain_time<%d>"
            "resub_flag<%d>resub_remain_time<%d>",
            response.rereg_flag, response.rereg_remain_time, response.resub_flag,
            response.resub_remain_time);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_EREGRT_INFO_IND,
            m_slot_id, RfxEregrtData(&response, sizeof(RIL_EregrtInfo)));
    responseToTelCore(urc);
}

void RmmImsControlUrcHandler::handleImsRegFlagInd(const sp<RfxMclMessage>& msg) {
    /* +EIMSUI: <flag> */
    RfxMipcData* urc_data = msg->getMipcData();
    int flag = (int)(urc_data->getMipcUint8Val(MIPC_IMS_UI_IND_T_ICONFLAG, 0));

    logI(RFX_LOG_TAG, "handleImsRegFlagInd:+EIMSUI:%d", flag);
    if (flag != 1) {
        return;
    }
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RIL_MSG_UNSOL_IMS_REG_FLAG_IND,
            m_slot_id, RfxIntsData(&flag, 1));
    responseToTelCore(urc);
}

