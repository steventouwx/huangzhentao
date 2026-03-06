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

#include "RmmSuppServRequestBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringsData.h"
#include "RfxStringData.h"
#include "RfxBarringCallInfosData.h"
#include "RfxCallForwardInfoData.h"
#include "RfxCallForwardInfosData.h"
#include "RfxCallForwardInfoExData.h"
#include "RfxCallForwardInfosExData.h"
#include "RfxMessageId.h"
#include "SSUtil.h"
//#include "SSConfig.h"
#include "RfxRilUtils.h"

//#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include "rfx_properties.h"

#include <compiler/compiler_utils.h>
#include <telephony/mtk_ril.h>
#include <libmtkrilutils.h>
#include <mtk_properties.h>
#include <string.h>

RmmSuppServRequestBaseHandler::RmmSuppServRequestBaseHandler(int slot_id,
        int channel_id):RfxBaseHandler(slot_id, channel_id){

    isResetSession = false;
}

RmmSuppServRequestBaseHandler::~RmmSuppServRequestBaseHandler() {
    // do nothing
}

/**
 * Handle MIPC response
 */
void RmmSuppServRequestBaseHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        logE(TAG, "[%s] %d getMipcData() return null", __FUNCTION__, msg->getId());
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INTERNAL_ERR, RfxVoidData(), msg, false);
        responseToTelCore(response);

        return;
    }

    logD(TAG, "[%s] msg response:%d", __FUNCTION__, msg->getId());

    switch(msg->getId()) {
        case RFX_MSG_REQUEST_SEND_USSD:
            // fall through
            TELEPHONYWARE_FALLTHROUGH;

        case RFX_MSG_REQUEST_SEND_USSI:
            handleSendUssdCnf(msg);
            break;

        case RFX_MSG_REQUEST_CANCEL_USSD:
            // fall through
            TELEPHONYWARE_FALLTHROUGH;

        case RFX_MSG_REQUEST_CANCEL_USSI:
            handleMipcVoidCnf(msg, "RFX_MSG_REQUEST_CANCEL_USSI");
            break;

        case RFX_MSG_REQUEST_GET_CLIR:
            handleClirOperationCnf(msg);
            break;

        case RFX_MSG_REQUEST_SET_CLIR:
            handleMipcVoidCnf(msg, "RFX_MSG_REQUEST_SET_CLIR");
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS:
            handleCallForwardOperationCnf(msg, CCFC_E_QUERY);
            break;

        case RFX_MSG_REQUEST_SET_CALL_FORWARD:
            handleCallForwardOperationCnf(msg, CCFC_E_SET);
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT:
            handleCallForwardExOperationCnf(msg, CCFC_E_QUERY);
            break;

        case RFX_MSG_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT:
            handleCallForwardExOperationCnf(msg, CCFC_E_SET);
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_WAITING:
            handleCallWaitingOperationCnf(msg, CW_E_QUERY);
            break;

        case RFX_MSG_REQUEST_SET_CALL_WAITING:
            handleCallWaitingOperationCnf(msg, CW_E_SET);
            break;

        case RFX_MSG_REQUEST_QUERY_CALL_BARRING:
            handleCallBarringCnf(msg, CB_E_QUERY);
            break;

        case RFX_MSG_REQUEST_SET_CALL_BARRING:
            handleCallBarringCnf(msg, CB_E_SET);
            break;

        case RFX_MSG_REQUEST_CHANGE_BARRING_PASSWORD:
            handleMipcVoidCnf(msg, "RFX_MSG_REQUEST_CHANGE_BARRING_PASSWORD");
            break;

        case RFX_MSG_REQUEST_QUERY_CLIP:
            handleQueryClipCnf(msg);
            break;

        case RFX_MSG_REQUEST_SET_CLIP:
            handleSetClipCnf(msg);
            break;

        case RFX_MSG_REQUEST_GET_COLP:
            // fall through
             TELEPHONYWARE_FALLTHROUGH;

        case RFX_MSG_REQUEST_SET_COLP:
            handleColpOperationCnf(msg);
            break;

        case RFX_MSG_REQUEST_GET_COLR:
            // fall through
             TELEPHONYWARE_FALLTHROUGH;

        case RFX_MSG_REQUEST_SET_COLR:
            handleColrOperationCnf(msg);
            break;

        case RFX_MSG_REQUEST_SEND_CNAP:
            handleSendCnapCnf(msg);
            break;

        case RFX_MSG_REQUEST_SET_SUPP_SVC_NOTIFICATION:
            handleMipcVoidCnf(msg, "RFX_MSG_REQUEST_SET_SUPP_SVC_NOTIFICATION");
            break;

        case RFX_MSG_REQUEST_SETUP_XCAP_USER_AGENT_STRING:
            handleMipcVoidCnf(msg, "RFX_MSG_REQUEST_SETUP_XCAP_USER_AGENT_STRING");
            break;

        case RFX_MSG_EVENT_SET_XCAP_CONFIG:
            handleMipcVoidCnf(msg, "RFX_MSG_EVENT_SET_XCAP_CONFIG");
            break;

        case RFX_MSG_REQUEST_GET_XCAP_STATUS:
            handleMipcVoidCnf(msg, "RFX_MSG_REQUEST_GET_XCAP_STATUS");
            break;

        case RFX_MSG_REQUEST_GET_BARRING_SEPCIFIC_CALL:
            handleGetBarringCallsCnf(msg);
            break;

        case RFX_MSG_REQUEST_SET_BARRING_SEPCIFIC_CALL:
            handleSetBarringCallsCnf(msg);
            break;

        default:
            logE(TAG, "[%s] should not be here", __FUNCTION__);
            break;

    }
}

int RmmSuppServRequestBaseHandler::requestMipcVoid(const sp<RfxMclMessage>& msg, uint16_t mipcMsgId) {
    sp<RfxMipcData> mipcData = RfxMipcData::obtainMipcData(mipcMsgId, msg->getSlotId());
    return callToMipcMsgAsync(msg, mipcData);
}

/**
 * handleMipcVoidCnf
 *
 * It only cares about the mipc result for success or not.
 *
 * Ex. mipc_ss_cancel_ussd_cnf_tlv_enum
 */
void RmmSuppServRequestBaseHandler::handleMipcVoidCnf(
        const sp<RfxMclMessage>& msg, const char *logTag, bool copyData) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    int msgId = msg->getId();

    logD(TAG, "[%s] %s mipcRet:%d", __FUNCTION__, logTag, mipcRet);

    // event id, need to covert to msg id to telcore.
    if (msgId == RFX_MSG_EVENT_SET_XCAP_CONFIG) {
        msgId = RFX_MSG_REQUEST_OEM_HOOK_STRINGS;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msgId,
            convertMipcResultToRil(__FUNCTION__, mipcRet), RfxVoidData(), msg, copyData);
    responseToTelCore(response);
}

/**
 * requestSendUssd
 * Send USSD and USSI request, MD will help to select domain.
 *
 * MIPC_SS_SEND_USSD_REQ
 * MIPC_SS_SEND_USSD_REQ_T_DCS default value 72 (UCS2)
 * MIPC_SS_SEND_USSD_REQ_T_PAYLOAD_LEN
 *
 * URC +CUSD: will be as result in CNF message.
 *
 * refer to mipc_ss_send_ussd_req_tlv_enum, mipc_ss_send_ussd_cnf_tlv_enum
 * handleSendUssdCnf()
 */
int RmmSuppServRequestBaseHandler::requestSendUssd(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> ussdMipc;

    int length = 0;
    const char* p_ussdRequest = (char*) msg->getData()->getData();

    // Check input USSD string first
    if (p_ussdRequest == NULL || strlen(p_ussdRequest) == 0) {
        logE(TAG, "[%s] p_ussdRequest null or empty.", __FUNCTION__);
        sendUssdFailureReport(msg, RIL_E_INTERNAL_ERR);

        // as USSD failure needs to handle specially, so return success.
        return RIL_E_SUCCESS;
    }

    length = strlen(p_ussdRequest);

    ussdMipc = RfxMipcData::obtainMipcData(MIPC_SS_SEND_USSD_REQ, msg->getSlotId());
    ussdMipc->addMipcTlvUint32(MIPC_SS_SEND_USSD_REQ_T_DCS, 68); // dcs =68, MD will convert it to ucs2

    if (length > MAX_RIL_USSD_DIGIT_TO_L5_LENGTH) {
        logW(TAG, "[%s] p_ussdRequest truncate to 160 byte", __FUNCTION__);
        length = MAX_RIL_USSD_DIGIT_TO_L5_LENGTH;
        std::string ussdString(p_ussdRequest);
        p_ussdRequest = ussdString.substr(0, MAX_RIL_USSD_DIGIT_TO_L5_LENGTH).c_str();
    }

    // check string length
    ussdMipc->addMipcTlvUint8(MIPC_SS_SEND_USSD_REQ_T_PAYLOAD_LEN, length);
    ussdMipc->addMipcTlvStr(MIPC_SS_SEND_USSD_REQ_T_PAYLOAD, p_ussdRequest);

    if(callToMipcMsgAsync(msg, ussdMipc) != MIPC_RESULT_SUCCESS) {
        sendUssdFailureReport(msg, RIL_E_INTERNAL_ERR);
    }

    // as USSD failure needs to handle specially, so return success.
    // URC +CUSD: will be as result in CNF message.
    return RIL_E_SUCCESS;
}

/**
 * sendUssdResponse
 * It's only care about the mipc result.
 *
 * refer to mipc_ss_send_ussd_cnf_tlv_enum
 */
void RmmSuppServRequestBaseHandler::handleSendUssdCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;
    sp<RfxMclMessage> urc;

    /* mipc_ss_ussd_const_enum */
    int mode = 0;
    int payloadLen = 0;
    uint16_t len = 0;
    char *finalData[2];
    char *payload = NULL;

    /* Initialize finalData */
    for(int i = 0; i < 2; i++) {
        finalData[i] = NULL;
    }

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    if (ret == RIL_E_SUCCESS) {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
    } else {
        sendUssdFailureReport(msg, ret);

        return;
    }

    // handle URC response
    /*
     * <m>:
     * 0   no further user action required (network initiated USSD Notify, or no further information needed after mobile initiated operation)
     * 1   further user action required (network initiated USSD Request, or further information needed after mobile initiated operation)
     * 2   USSD terminated by network
     * 3   other local client has responded
     * 4   operation not supported
     * 5   network time out
     */
    mode = (int) mipcData->getMipcUint8Val(MIPC_SS_SEND_USSD_CNF_T_USSD_RESPONSE, 0);

    if (mode < 0 || mode > 5) {
        mode = MIPC_SS_USSD_NO_ACTION_REQUIRED;
    }

    /**
     * According the USSD response format: "+CUSD: <m>[,<str>,<dcs>]",
     * the number of stings only could be 1 or 3. And we don't need to return dcs back
     * to framework if number of strings equal to 3.
     */
    asprintf(&finalData[0], "%d", mode);

    payloadLen = (int) mipcData->getMipcUint8Val(MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_LEN, 0);

    if (payloadLen > 0 && payloadLen < 255) {
        payload = (char *) mipcData->getMipcVal(MIPC_SS_SEND_USSD_CNF_T_PAYLOAD, &len);

        if (payload != NULL && len > 0) {
            finalData[1] = (char *) alloca(len + 1);

            if (finalData[1] != NULL) {
                memset(finalData[1], 0, len + 1 );
                memcpy(finalData[1], payload, len);
            } else {
                logE(TAG, "[%s] alloca failed! or playload is NULL", __FUNCTION__);
            }
        }
    } else {
        // if MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_LEN is more than or equal 255,
        // it shall use extern field.
        payloadLen = (int) mipcData->getMipcUint16Val(MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_LEN_EX, 0);

        if (payloadLen > 0) {
            payload = (char *) mipcData->getMipcVal(MIPC_SS_SEND_USSD_CNF_T_PAYLOAD_EX, &len);

            if (payload != NULL && len > 0) {
                finalData[1] = (char *) alloca(len + 1);

                if (finalData[1] != NULL) {
                    memset(finalData[1], 0, len + 1);
                    memcpy(finalData[1], payload, len);
                } else {
                    logE(TAG, "[%s] alloca failed! or playload is NULL", __FUNCTION__);
                }
            }
        }
    }

    logD(TAG, "[%s] mode:%d, payloadLen:%d, len:%d, content:%s", __FUNCTION__, mode, payloadLen,
            len, (finalData[1] == NULL ? "null" : finalData[1]));

    // change to notify mode, or else it'll be treated as error.
    if (mode == MIPC_SS_USSD_TERMINATED_BY_NW && finalData[1] != NULL) {
        finalData[0] = strdup("0");
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ON_USSD, m_slot_id, RfxStringsData(finalData, 2));
    responseToTelCore(urc);

    if (finalData[0] != NULL) {
        free(finalData[0]);
    }
}

void RmmSuppServRequestBaseHandler::sendUssdFailureReport(const sp<RfxMclMessage>& msg, RIL_Errno ret) {
    sp<RfxMclMessage> response;
    sp<RfxMclMessage> urc;

    char *genericUssdFail[2] = {(char *) "4", (char *) ""};  // Generate a generic failure USSD URC

    logD(TAG, "[%s] ret = %d", __FUNCTION__, ret);

    // For FDN check failure, we "directly" return FDN error cause when the request comes from
    // GSM pipe (ex. AOSP's GsmMmiCode). Because if USSD request is sent from GSM pipe, it can
    // accept error during request phase. And we should report it to let UI shows correspondnig
    // error meesage
    if (ret == RIL_E_FDN_CHECK_FAILURE) {
        // Let the UI have time to show up the "USSD code running" dialog
        sleepMsec(500);

        response = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    // Return SUCCESS first
    logD(TAG, "[%s] Return SUCCESS first by response", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(),
            msg, false);
    responseToTelCore(response);

    // Let the UI have time to show up the "USSD code running" dialog
    sleepMsec(500);

    // And then report the FAILIRUE by URC
    logD(TAG, "[%s] Report the FAILIRUE by URC", __FUNCTION__);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ON_USSD, m_slot_id,
            RfxStringsData(genericUssdFail, 2));
    responseToTelCore(urc);
}

/**
 * requestCancelUssd
 * Cancel USSD and USSI request, MD will help to select domain.
 *
 * MIPC_SS_CANCEL_USSD_REQ
 * MIPC_SS_CANCEL_USSD_CNF by handleMipcVoidCnf
 *
 * refer to mipc_ss_cancel_ussd_req_tlv_enum, mipc_ss_cancel_ussd_cnf_tlv_enum
 */
int RmmSuppServRequestBaseHandler::requestCancelUssd(const sp<RfxMclMessage>& msg) {
    logD(TAG, "[%s] cancel ussd", __FUNCTION__);
    return requestMipcVoid(msg, MIPC_SS_CANCEL_USSD_REQ);
}

/**
 * requestClirOperation
 * Terminal based CLIR is moved to MD.
 * MIPC_SS_GET_CLIR_REQ & MIPC_SS_GET_CLIR_CNF
 * MIPC_SS_SET_CLIR_REQ & MIPC_SS_SET_CLIR_CNF
 */
int RmmSuppServRequestBaseHandler::requestClirOperation(const sp<RfxMclMessage>& msg) {
    int *n = (int *) (msg->getData()->getData());

    if (msg->getData()->getDataLength() != 0) {
        sp<RfxMipcData> setClir = RfxMipcData::obtainMipcData(MIPC_SS_SET_CLIR_REQ, msg->getSlotId());
        setClir->addMipcTlvUint8(MIPC_SS_SET_CLIR_REQ_T_N_VALUE, (uint8_t) n[0]);

        logD(TAG, "[%s] setClir:%d", __FUNCTION__, n[0]);
        return callToMipcMsgAsync(msg, setClir);
    } else {
        logD(TAG, "[%s] getClir", __FUNCTION__);
        return requestMipcVoid(msg, MIPC_SS_GET_CLIR_REQ);
    }
}

/**
 * handleClirResponse
 * MIPC_SS_GET_CLIR_CNF
 * mipc_ss_get_clir_cnf_tlv_enum
 */
void RmmSuppServRequestBaseHandler::handleClirOperationCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;
    int responses[2] = {0};

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    if (RIL_E_SUCCESS != ret) {
        if (RFX_MSG_REQUEST_SET_CLIR == msg->getId()) {
            handleErrorMessage(mipcData, MIPC_SS_SET_CLIR_CNF_T_ERRMESSAGE);
        } else {
            handleErrorMessage(mipcData, MIPC_SS_GET_CLIR_CNF_T_ERRMESSAGE);
        }
    }

    if (RIL_E_SUCCESS != ret || RFX_MSG_REQUEST_SET_CLIR == msg->getId()) {
        goto done;
    }

    // Setting default value first
    // <n> : NETWORK_DEFAULT (0)
    // <m> : PRESENTATION_ALLOWED_TEMPORARY (4)
    responses[0] = (int) mipcData->getMipcUint8Val(MIPC_SS_GET_CLIR_CNF_T_CLIR_N, 0);
    responses[1] = (int) mipcData->getMipcUint8Val(MIPC_SS_GET_CLIR_CNF_T_CLIR_M, 4);

    logD(TAG, "[%s] clir n:%d, m:%d", __FUNCTION__, responses[0], responses[1]);

done:
    /* For SET CLIR responseVoid will ignore the responses */
    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);
    responseToTelCore(responseMsg);
}

/**
 * requestCallForwardOperation
 *
 * 3GPP 24.082 and 3GPP 24.030
 * Registration         **SC*DN*BS(*T)#
 * Erasure              ##SC**BS#
 * Activation           *SC**BS#
 * Deactivation         #SC**BS#
 * Interrogation        *#SC**BS#
 */
int RmmSuppServRequestBaseHandler::requestCallForwardOperation(const sp<RfxMclMessage>& msg, CallForwardOperationE op) {
    RIL_CallForwardInfo* p_args = (RIL_CallForwardInfo*) (msg->getData()->getData());

    sp<RfxMipcData> callForwardOpMipc;
    uint16_t msgIdMipc = MIPC_SS_SET_CALL_FORWARD_REQ;

    /* For Query Call Forwarding in O version, RILJ doesn't assign cf.status. The default value of
     * cf.status is 0, which means Deactivation in SsStatusE. We need chaneg it to Interrogation.
     */
    if (CCFC_E_QUERY == op) {
        p_args->status = SS_INTERROGATE;
        msgIdMipc      = MIPC_SS_QUERY_CALL_FORWARD_REQ;
    }

    if ((p_args->number != NULL) && (p_args->status == SS_ACTIVATE)) {
        p_args->status = SS_REGISTER;
    }

    callForwardOpMipc = RfxMipcData::obtainMipcData(msgIdMipc, msg->getSlotId());

    logD(TAG,"[%s] Call forwarding: status:%d, reason:%d, serviceClass:%d, toa:%d, number:%s",
            __FUNCTION__,
            p_args->status, p_args->reason, p_args->serviceClass, p_args->toa,
            RfxRilUtils::pii(TAG, p_args->number));

    if (CCFC_E_QUERY == op) {
        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_QUERY_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON,
                ssReasonToMipcReasonCode((CallForwardReasonE) p_args->reason));

        callForwardOpMipc->addMipcTlvUint16(
                MIPC_SS_QUERY_CALL_FORWARD_REQ_T_SERVICE_CLASS,
                (uint16_t) p_args->serviceClass);
    } else {
        /// for set request
        logD(TAG, "[%s] Call forwarding: set code[%d]", __FUNCTION__, p_args->status);
        if (p_args->status == SS_REGISTER && p_args->number == NULL) {
            logE(TAG, "[%s] Call forwarding: number is NULL", __FUNCTION__);
            return RIL_E_INVALID_ARGUMENTS;
        }

        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON,
                ssReasonToMipcReasonCode((CallForwardReasonE) p_args->reason));

        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_SS_OPERATION,
                ssStatusToMipcOpCode(SsStatusE(p_args->status)));

        if (p_args->number != NULL) {
            callForwardOpMipc->addMipcTlvStr(
                    MIPC_SS_SET_CALL_FORWARD_REQ_T_DIAL_NUMBER,
                    p_args->number);
        }

        callForwardOpMipc->addMipcTlvUint16(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_SERVICE_CLASS,
                (uint16_t) p_args->serviceClass);

        callForwardOpMipc->addMipcTlvUint16(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_TOA,
                (uint16_t) p_args->toa);

        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_TIMER_SECONDS,
                p_args->timeSeconds);
    }

    return callToMipcMsgAsync(msg, callForwardOpMipc);
}

void RmmSuppServRequestBaseHandler::handleCallForwardOperationCnf(const sp<RfxMclMessage>& msg, CallForwardOperationE op) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    RIL_CallForwardInfo **pp_CfInfoResponses = NULL;
    int resLength = 0;
    mipc_ss_call_forward_struct4 *p_mipcCfInfo = NULL;
    uint16_t len = 0;

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    logD(TAG, "[%s] ret = %d, op = %d", __FUNCTION__, ret, op);
    if (RIL_E_SUCCESS != ret) {
        if (CCFC_E_SET == op) {
            handleErrorMessage(mipcData, MIPC_SS_SET_CALL_FORWARD_CNF_T_ERRMESSAGE);
        } else {
            handleErrorMessage(mipcData, MIPC_SS_QUERY_CALL_FORWARD_CNF_T_ERRMESSAGE);
        }
    }

    if (RIL_E_SUCCESS != ret || CCFC_E_SET == op) {
        goto done;
    }

    // Query
    resLength = mipcData->getMipcUint8Val(MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_COUNT, 0);
    if (resLength == 0) {
        logD(TAG, "[%s] resLength = 0", __FUNCTION__);
        goto done;
    }

    pp_CfInfoResponses = (RIL_CallForwardInfo **) alloca(resLength * sizeof(RIL_CallForwardInfo *));
    if (pp_CfInfoResponses == NULL) {
        logD(TAG, "[%s] pp_CfInfoResponses malloc failed", __FUNCTION__);
        resLength = 0;
        goto done;
    }
    memset(pp_CfInfoResponses, 0, resLength * sizeof(RIL_CallForwardInfo *));

    for (int i = 0; i < resLength; i++) {
        p_mipcCfInfo = (mipc_ss_call_forward_struct4 *) mipcData->getMipcIdxVal(
                MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_LIST, i, &len);

        if (len != sizeof(mipc_ss_call_forward_struct4)) {
            logD(TAG, "[%s] len error, %u!, %zu", __FUNCTION__, len, sizeof(mipc_ss_call_forward_struct4));
            resLength = (i > 0 ? i-- : 0);
            break;
        }
        pp_CfInfoResponses[i] = (RIL_CallForwardInfo *) alloca(sizeof(RIL_CallForwardInfo));

        if (pp_CfInfoResponses[i] == NULL) {
            logD(TAG, "[%s] p_CfInfoResponse malloc failed", __FUNCTION__);
            resLength = (i > 0 ? i-- : 0);
            goto done;
        }
        memset(pp_CfInfoResponses[i], 0, sizeof(RIL_CallForwardInfo));

        pp_CfInfoResponses[i]->status         = (int) p_mipcCfInfo->call_forward_status;
        pp_CfInfoResponses[i]->reason         = mipcReasonCodeToSsReason(p_mipcCfInfo->reason);
        pp_CfInfoResponses[i]->serviceClass   = (int) p_mipcCfInfo->service_class;
        pp_CfInfoResponses[i]->toa            = (int) p_mipcCfInfo->type_of_address;
        pp_CfInfoResponses[i]->number         = p_mipcCfInfo->dial_number;
        pp_CfInfoResponses[i]->timeSeconds    = (int) p_mipcCfInfo->timer;

        logD(TAG, "[%s] CallForward[%d]: status:%d, reason:%d, serviceClass:%d, toa:%d, number:%s",
                __FUNCTION__, i,
                pp_CfInfoResponses[i]->status,
                pp_CfInfoResponses[i]->reason,
                pp_CfInfoResponses[i]->serviceClass,
                pp_CfInfoResponses[i]->toa,
                RfxRilUtils::pii(TAG, pp_CfInfoResponses[i]->number));
    }

    logD(TAG, "[%s] CallForward resLength[%d]", __FUNCTION__, resLength);

done:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxCallForwardInfosData(pp_CfInfoResponses, resLength * sizeof(RIL_CallForwardInfo *)),
            msg, false);
    responseToTelCore(response);
}

int RmmSuppServRequestBaseHandler::requestCallForwardExOperation(
        const sp<RfxMclMessage>& msg, CallForwardOperationE op) {
    RIL_CallForwardInfoEx* p_args = (RIL_CallForwardInfoEx*) (msg->getData()->getData());

    sp<RfxMipcData> callForwardOpMipc;
    uint16_t msgIdMipc = MIPC_SS_SET_CALL_FORWARD_REQ;

    /* For Query Call Forwarding in O version, RILJ doesn't assign cf.status. The default value of
     * cf.status is 0, which means Deactivation in SsStatusE. We need chaneg it to Interrogation.
     */
    if (CCFC_E_QUERY == op) {
        p_args->status = SS_INTERROGATE;
        msgIdMipc      = MIPC_SS_QUERY_CALL_FORWARD_REQ;
    }

    if ((p_args->number != NULL) && (p_args->status == SS_ACTIVATE)) {
        p_args->status = SS_REGISTER;
    }

    callForwardOpMipc = RfxMipcData::obtainMipcData(msgIdMipc, msg->getSlotId());

    logD(TAG,"[%s] Call forwarding: status:%d, reason:%d, serviceClass:%d, toa:%d, number:%s",
            __FUNCTION__,
            p_args->status, p_args->reason, p_args->serviceClass, p_args->toa,
            RfxRilUtils::pii(TAG, p_args->number));

    if (CCFC_E_QUERY == op) {
        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_QUERY_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON,
                ssReasonToMipcReasonCode((CallForwardReasonE) p_args->reason));

        callForwardOpMipc->addMipcTlvUint16(
                MIPC_SS_QUERY_CALL_FORWARD_REQ_T_SERVICE_CLASS,
                (uint16_t) p_args->serviceClass);
    } else {
        /// for set request
        logD(TAG, "[%s] Call forwarding: set code[%d]", __FUNCTION__, p_args->status);
        if (p_args->number == NULL) {
            logE(TAG, "[%s] Call forwarding: number is NULL", __FUNCTION__);
            return RIL_E_INVALID_ARGUMENTS;
        }

        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON,
                ssReasonToMipcReasonCode((CallForwardReasonE) p_args->reason));

        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_SS_OPERATION,
                ssStatusToMipcOpCode(SsStatusE(p_args->status)));

        callForwardOpMipc->addMipcTlvStr(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_DIAL_NUMBER,
                p_args->number);

        callForwardOpMipc->addMipcTlvUint16(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_SERVICE_CLASS,
                (uint16_t) p_args->serviceClass);

        callForwardOpMipc->addMipcTlvUint16(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_TOA,
                (uint16_t) p_args->toa);

        callForwardOpMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_FORWARD_REQ_T_TIMER_SECONDS,
                p_args->timeSeconds);

        if (p_args->timeSlotBegin != NULL && p_args->timeSlotEnd != NULL) {
            callForwardOpMipc->addMipcTlvStr(MIPC_SS_SET_CALL_FORWARD_REQ_T_TIME_SLOT_BEGIN,
                    p_args->timeSlotBegin);
            callForwardOpMipc->addMipcTlvStr(MIPC_SS_SET_CALL_FORWARD_REQ_T_TIME_SLOT_END,
                    p_args->timeSlotEnd);
            logD(TAG,"[%s] Call forwarding: time slot begin:%s, end:%s", __FUNCTION__,
                    p_args->timeSlotBegin, p_args->timeSlotEnd);
        }
    }

    return callToMipcMsgAsync(msg, callForwardOpMipc);
}

void RmmSuppServRequestBaseHandler::handleCallForwardExOperationCnf(
        const sp<RfxMclMessage>& msg, CallForwardOperationE op) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    RIL_CallForwardInfoEx **pp_CfInfoResponses = NULL;
    int resLength = 0;
    uint16_t len = 0;
    mipc_ss_call_forward_struct4 *p_mipcCfInfo = NULL;

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    logD(TAG, "[%s] ret = %d, op = %d", __FUNCTION__, ret, op);
    if (RIL_E_SUCCESS != ret) {
        if (CCFC_E_SET == op) {
            handleErrorMessage(mipcData, MIPC_SS_SET_CALL_FORWARD_CNF_T_ERRMESSAGE);
        } else {
            handleErrorMessage(mipcData, MIPC_SS_QUERY_CALL_FORWARD_CNF_T_ERRMESSAGE);
        }
    }

    if (RIL_E_SUCCESS != ret || CCFC_E_SET == op) {
        goto done;
    }

    // Query
    resLength = mipcData->getMipcUint8Val(MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_COUNT, 0);
    if (resLength == 0) {
        logD(TAG, "[%s] resLength = 0", __FUNCTION__);
        goto done;
    }

    pp_CfInfoResponses = (RIL_CallForwardInfoEx **) alloca(resLength * sizeof(RIL_CallForwardInfoEx *));
    if (pp_CfInfoResponses == NULL) {
        logD(TAG, "[%s] pp_CfInfoResponses malloc failed", __FUNCTION__);
        resLength = 0;
        goto done;
    }
    memset(pp_CfInfoResponses, 0, resLength * sizeof(RIL_CallForwardInfoEx *));

    for (int i = 0; i < resLength; i++) {
        p_mipcCfInfo = (mipc_ss_call_forward_struct4 *) mipcData->getMipcIdxVal(
                MIPC_SS_QUERY_CALL_FORWARD_CNF_T_CALL_FORWARD_LIST, i, &len);

        if (len != sizeof(mipc_ss_call_forward_struct4)) {
            logD(TAG, "[%s] len error!", __FUNCTION__);
            resLength = (i > 0 ? i-- : 0);
            break;
        }
        pp_CfInfoResponses[i] = (RIL_CallForwardInfoEx *) alloca(sizeof(RIL_CallForwardInfoEx));

        if (pp_CfInfoResponses[i] == NULL) {
            logD(TAG, "[%s] p_CfInfoResponse malloc failed", __FUNCTION__);
            resLength = (i > 0 ? i-- : 0);
            goto done;
        }
        memset(pp_CfInfoResponses[i], 0, sizeof(RIL_CallForwardInfoEx));

        pp_CfInfoResponses[i]->status         = (int) p_mipcCfInfo->call_forward_status;
        pp_CfInfoResponses[i]->reason         = mipcReasonCodeToSsReason(p_mipcCfInfo->reason);
        pp_CfInfoResponses[i]->serviceClass   = (int) p_mipcCfInfo->service_class;
        pp_CfInfoResponses[i]->toa            = (int) p_mipcCfInfo->type_of_address;
        pp_CfInfoResponses[i]->number         = p_mipcCfInfo->dial_number;
        pp_CfInfoResponses[i]->timeSeconds    = (int) p_mipcCfInfo->timer;

        logD(TAG, "[%s] CallForwardEx[%d]: status:%d, reason:%d, serviceClass:%d, toa:%d, number:%s",
                __FUNCTION__, i,
                pp_CfInfoResponses[i]->status,
                pp_CfInfoResponses[i]->reason,
                pp_CfInfoResponses[i]->serviceClass,
                pp_CfInfoResponses[i]->toa,
                RfxRilUtils::pii(TAG, pp_CfInfoResponses[i]->number));

        if (strlen(p_mipcCfInfo->time_slot_begin) != 0
                && strlen(p_mipcCfInfo->time_slot_end) != 0) {
            pp_CfInfoResponses[i]->timeSlotBegin  = p_mipcCfInfo->time_slot_begin;
            pp_CfInfoResponses[i]->timeSlotEnd    = p_mipcCfInfo->time_slot_end;

            logD(TAG,"[%s] Call forwarding: time slot begin:%s, end:%s", __FUNCTION__,
                    p_mipcCfInfo->time_slot_begin, p_mipcCfInfo->time_slot_end);
        }
    }

    logD(TAG, "[%s] CallForwardEx resLength[%d]", __FUNCTION__, resLength);

done:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxCallForwardInfosExData(pp_CfInfoResponses, resLength * sizeof(RIL_CallForwardInfoEx *)),
            msg, false);
    responseToTelCore(response);
}

int RmmSuppServRequestBaseHandler::requestCallWaitingOperation(const sp<RfxMclMessage>& msg,
            CallWaitingOperationE op) {
    int *p_int = (int *) (msg->getData()->getData());

    sp<RfxMipcData> callWaitingMipc = RfxMipcData::obtainMipcData(
            (CW_E_QUERY == op) ? MIPC_SS_QUERY_CALL_WAITING_REQ : MIPC_SS_SET_CALL_WAITING_REQ,
            msg->getSlotId());

    if (msg->getData()->getDataLength() == sizeof(int)) {
        // CW_E_QUERY
        logD(TAG, "[%s] BsCode = %d", __FUNCTION__, p_int[0]);

        callWaitingMipc->addMipcTlvUint16(MIPC_SS_QUERY_CALL_WAITING_REQ_T_SERVICE_CLASS, p_int[0]);
    } else if (msg->getData()->getDataLength() == 2 * sizeof(int)) {
        logD(TAG, "[%s] p_int[0] = %d, p_int[1] = %d", __FUNCTION__, p_int[0], p_int[1]);

        callWaitingMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_WAITING_REQ_T_CW_ENABLE_DISABLE, p_int[0]);
        callWaitingMipc->addMipcTlvUint16(MIPC_SS_SET_CALL_WAITING_REQ_T_SERVICE_CLASS, p_int[1]);
    } else {
        logD(TAG, "[%s] parameter error!", __FUNCTION__);
        return RIL_E_INVALID_ARGUMENTS;
    }

    return callToMipcMsgAsync(msg, callWaitingMipc);
}

void RmmSuppServRequestBaseHandler::handleCallWaitingOperationCnf(const sp<RfxMclMessage>& msg,
            CallWaitingOperationE op) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret    = RIL_E_INTERNAL_ERR;
    int responses[2] = {0};
    int count = 0;
    mipc_ss_call_waiting_struct4 *p_mipcCwInfo = NULL;
    uint16_t len = 0;

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    logD(TAG, "[%s] ret = %d, op = %d", __FUNCTION__, ret, op);
    if (RIL_E_SUCCESS != ret) {
        if (CW_E_SET == op) {
            handleErrorMessage(mipcData, MIPC_SS_SET_CALL_WAITING_CNF_T_ERRMESSAGE);
        } else {
            handleErrorMessage(mipcData, MIPC_SS_QUERY_CALL_WAITING_CNF_T_ERRMESSAGE);
        }
    }

    if (RIL_E_SUCCESS != ret || CW_E_SET == op) {
        goto done;
    }

    count = (int) mipcData->getMipcUint8Val(
            MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_COUNT, 0);
    p_mipcCwInfo = (mipc_ss_call_waiting_struct4 *) mipcData->getMipcVal(
            MIPC_SS_QUERY_CALL_WAITING_CNF_T_CW_LIST, &len);

    /* It should be only one item, as the class is added in request */
    if (p_mipcCwInfo != NULL) {
        responses[0] = p_mipcCwInfo[0].call_waiting_status;
        responses[1] = p_mipcCwInfo[0].service_class;
    }

    logD(TAG, "[%s] n = %d, m = %d, count = %d", __FUNCTION__, responses[0], responses[1], count);

done:
    /* For SET CCWA responseVoid will ignore the responses */
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);
    responseToTelCore(response);
}

int RmmSuppServRequestBaseHandler::requestCallBarring(const sp<RfxMclMessage>& msg,
        CallBarringOperationE op) {
    const char** strings = (const char**) (msg->getData()->getData());

    sp<RfxMipcData> callBarringMipc = RfxMipcData::obtainMipcData(
            (CB_E_QUERY == op) ? MIPC_SS_QUERY_CALL_BARRING_REQ : MIPC_SS_SET_CALL_BARRING_REQ,
            msg->getSlotId());

    if ( msg->getData()->getDataLength() == 4 * sizeof(char*) ) {
        /* Query Facility Lock */
        int sendBsCode = 0;

        if ((0 == strcmp("AB", strings[0]))
                || (0 == strcmp("AG", strings[0]))
                || (0 == strcmp("AC", strings[0]))) {

            logE(TAG, "[%s] Call Barring Error: %s Cannot be used for Query!",
                    __FUNCTION__, strings[0]);
            return RIL_E_INVALID_ARGUMENTS;
        }

        if ((NULL != strings[2]) && (0 != strcmp(strings[2], "0"))) {
            sendBsCode = atoi(strings[2]);
        }

        logD(TAG, "[%s] query fac:%s sendBsCode = %d", __FUNCTION__, strings[0], sendBsCode);

        callBarringMipc->addMipcTlvUint8(
                MIPC_SS_QUERY_CALL_BARRING_REQ_T_FACILITY,
                (uint8_t) callBarFacToServiceCode(strings[0]));
        callBarringMipc->addMipcTlvUint16(
                MIPC_SS_QUERY_CALL_BARRING_REQ_T_SERVICE_CLASS,
                (uint16_t) sendBsCode);
    } else if ( msg->getData()->getDataLength() == 5 * sizeof(char*) ) {
        uint8_t lock = MIPC_SS_CALL_BARRING_LOCK_LOCK; // 0: unlock, 1: lock

        if(NULL == strings[2]) {
            logE(TAG, "[%s] Set Facility Lock: Pwd cannot be null!", __FUNCTION__);
            return RIL_E_PASSWORD_INCORRECT;
        }

        // activate/deactivate
        if ( 0 == strcmp(strings[1], "0")) {
            lock = MIPC_SS_CALL_BARRING_LOCK_UNLOCK;
        }

        logD(TAG, "[%s] Set Fac:%s, lock:%s",
                __FUNCTION__, strings[0], lock == 0 ? "unlock" : "lock");

        callBarringMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_BARRING_REQ_T_LOCK,
                lock);
        callBarringMipc->addMipcTlvUint8(
                MIPC_SS_SET_CALL_BARRING_REQ_T_FACILITY,
                (uint8_t) callBarFacToServiceCode(strings[0]));

        if ( 0 != strlen(strings[2])) {
            callBarringMipc->addMipcTlvStr(MIPC_SS_SET_CALL_BARRING_REQ_T_PASSWORD, strings[2]);
            logD(TAG, "[%s] Set Facility Lock, p:%s", __FUNCTION__, strings[2]);
        }

        if ((NULL != strings[3]) && (0 != strcmp(strings[3], "0"))) {
            int sendBsCode = atoi(strings[3]);
            callBarringMipc->addMipcTlvUint16(
                    MIPC_SS_SET_CALL_BARRING_REQ_T_SERVICE_CLASS,
                    (uint16_t) sendBsCode);
            logD(TAG, "[%s] Set Facility Lock, sendBsCode:%d", __FUNCTION__, sendBsCode);
        }
    } else {
        logD(TAG, "[%s] error!", __FUNCTION__);
        return RIL_E_INVALID_ARGUMENTS;
    }

    return callToMipcMsgAsync(msg, callBarringMipc);
}

void RmmSuppServRequestBaseHandler::handleCallBarringCnf(const sp<RfxMclMessage>& msg,
        CallBarringOperationE op) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    int response = -1;

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    logD(TAG, "[%s] ret = %d, op = %d", __FUNCTION__, ret, op);
    if (RIL_E_SUCCESS != ret) {
        if (CB_E_SET == op) {
            handleErrorMessage(mipcData, MIPC_SS_SET_CALL_BARRING_CNF_T_ERRMESSAGE);
        } else {
            handleErrorMessage(mipcData, MIPC_SS_QUERY_CALL_BARRING_CNF_T_ERRMESSAGE);
        }
    }

    if (RIL_E_SUCCESS != ret || CB_E_SET == op) {
        goto done;
    }

    response = (int) mipcData->getMipcUint16Val(
            MIPC_SS_QUERY_CALL_BARRING_CNF_T_CALL_BARRING_STATUS, 0);

    logD(TAG, "[%s] response:%d", __FUNCTION__, response);

done:
    int msgid = (op == CB_E_QUERY) ?
            RFX_MSG_REQUEST_QUERY_FACILITY_LOCK : RFX_MSG_REQUEST_SET_FACILITY_LOCK;
    logD(TAG, "[%s] requestCallBarring response: %d, msg id: %d, ret: %d",
            __FUNCTION__, response, msgid, ret);
    /* For SET Facility Lock responseVoid will ignore the responses */
    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(msgid, ret,
            RfxIntsData((void*)&response, sizeof(int)), msg, false);
    responseToTelCore(responseMsg);
}

int RmmSuppServRequestBaseHandler::requestChangeBarringPassword(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> barringPwdMipc = RfxMipcData::obtainMipcData(
            MIPC_SS_CHANGE_BARRING_PASSWORD_REQ,
            msg->getSlotId());

    const char** strings = (const char**) (msg->getData()->getData());
    const char* cnfNewPwd = NULL;

    if (strings[0] == NULL || strlen(strings[0]) == 0 ||
            strings[1] == NULL || strlen(strings[1]) == 0 ||
            strings[2] == NULL || strlen(strings[2]) == 0) {
        logE(TAG, "[%s] Null parameters.", __FUNCTION__);

        return RIL_E_INVALID_ARGUMENTS;;
    }

    /**
     * "data" is const char **
     *
     * ((const char **)data)[0] = facility string code from TS 27.007 7.4 (eg "AO" for BAOC)
     * ((const char **)data)[1] = old password
     * ((const char **)data)[2] = new password
     * ((const char **)data)[3] = new password confirmed
     */
    if (msg->getData()->getDataLength() == 3 * sizeof(char*)) {
        cnfNewPwd = strings[2];
    } else if (msg->getData()->getDataLength() == 4 * sizeof(char*)) {
        if (strings[3] == NULL || strlen(strings[3]) == 0) {
            logE(TAG, "[%s] Null parameters 3.", __FUNCTION__);

            return RIL_E_INVALID_ARGUMENTS;
        }
        cnfNewPwd = strings[3];
    } else {
        return RIL_E_INVALID_ARGUMENTS;
    }

    barringPwdMipc->addMipcTlvUint8(
            MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_FACILITY,
            (uint8_t) callBarFacToServiceCode(strings[0]));

    barringPwdMipc->addMipcTlvStr(
            MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_OLD_PWD,
            strings[1]);

    barringPwdMipc->addMipcTlvStr(
            MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_NEW_PWD,
            strings[2]);

    barringPwdMipc->addMipcTlvStr(
            MIPC_SS_CHANGE_BARRING_PASSWORD_REQ_T_NEW_PWD_CONFIRM,
            cnfNewPwd);

    return callToMipcMsgAsync(msg, barringPwdMipc);
}

int RmmSuppServRequestBaseHandler::requestQueryClip(const sp<RfxMclMessage>& msg) {
    logD(TAG, "[%s]", __FUNCTION__);
    return requestMipcVoid(msg, MIPC_SS_QUERY_CLIP_REQ);
}

void RmmSuppServRequestBaseHandler::handleQueryClipCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    int responses[2] = {0};
    int result       = 0;

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    if (ret != RIL_E_SUCCESS) {
        handleErrorMessage(mipcData, MIPC_SS_QUERY_CLIP_CNF_T_ERRMESSAGE);

        goto done;
    }

    /**
     * <n> (parameter sets/shows the result code presentation status in the MT/TA):
     * 0   disable
     * 1   enable
     */
    responses[0] = (int) mipcData->getMipcUint8Val(MIPC_SS_QUERY_CLIP_CNF_T_CODE_STATUS, 0);

    /**
     * <m> (parameter shows the subscriber CLIP service status in the network):
     * 0   CLIP not provisioned
     * 1   CLIP provisioned
     * 2   unknown (e.g. no network, etc.)
     */
    responses[1] = (int) mipcData->getMipcUint8Val(MIPC_SS_QUERY_CLIP_CNF_T_NW_STATUS, 0);

    // check provision status first
    if (responses[1] == 0 || responses[1] == 2) {
        result = 0;
    } else {
        result = responses[0];
    }

    logD(TAG, "[%s] n = %d, m = %d", __FUNCTION__, responses[0], responses[1]);

done:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData(&result, sizeof(result)/sizeof(int)), msg, false);
    responseToTelCore(response);
}

int RmmSuppServRequestBaseHandler::requestSetClip(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> setClipMipc = RfxMipcData::obtainMipcData(MIPC_SS_SET_CLIP_REQ, msg->getSlotId());

    int *n = (int *) (msg->getData()->getData());

    if (msg->getData()->getDataLength() != 0) {
        logD(TAG, "[%s] n[0] = %d", __FUNCTION__, n[0]);
        // 0: disable, 1: enable
        setClipMipc->addMipcTlvUint8(MIPC_SS_SET_CLIP_REQ_T_STATUS, (uint8_t) n[0]);
    } else {
        return RIL_E_INVALID_ARGUMENTS;
    }

    return callToMipcMsgAsync(msg, setClipMipc);
}

void RmmSuppServRequestBaseHandler::handleSetClipCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    logD(TAG, "[%s]", __FUNCTION__);

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);
    if (ret != RIL_E_SUCCESS) {
        handleErrorMessage(mipcData, MIPC_SS_SET_CLIP_CNF_T_ERRMESSAGE);
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
            ret, RfxVoidData(), msg, false);
    responseToTelCore(response);

}

int RmmSuppServRequestBaseHandler::requestColpOperation(const sp<RfxMclMessage>& msg) {
    int* n = (int *) (msg->getData()->getData());

    if (msg->getData()->getDataLength() != 0) {
        /**
         * "data" is int *
         * ((int *)data)[0] is "n" parameter from TS 27.007 7.8
         *  <n> (parameter sets the adjustment for outgoing calls)
         */
        sp<RfxMipcData> setColpMipc = RfxMipcData::obtainMipcData(MIPC_SS_SET_COLP_REQ, msg->getSlotId());
        setColpMipc->addMipcTlvUint8(MIPC_SS_SET_COLP_REQ_T_N_VALUE, (uint8_t) n[0]);

        logD(TAG, "[%s] setColp:%d", __FUNCTION__, n[0]);
        return callToMipcMsgAsync(msg, setColpMipc);
    } else {
        logD(TAG, "[%s] getClir", __FUNCTION__);
        return requestMipcVoid(msg, MIPC_SS_GET_COLP_REQ);
    }
}

void RmmSuppServRequestBaseHandler::handleColpOperationCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;
    int responses[2] = {0};

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    if (RIL_E_SUCCESS != ret) {
        if (RFX_MSG_REQUEST_SET_COLP == msg->getId()) {
            handleErrorMessage(mipcData, MIPC_SS_SET_COLP_CNF_T_ERRMESSAGE);
        } else {
            handleErrorMessage(mipcData, MIPC_SS_GET_COLP_CNF_T_ERRMESSAGE);
        }
    }

    if (RIL_E_SUCCESS != ret || RFX_MSG_REQUEST_SET_COLP == msg->getId()) {
        goto done;
    }

    /**
     * <n> parameter sets/shows the result code presentation status to the TE
     * 0   COLP disabled in MS
     * 1   COLP enabled in MS
     *
     * <m> parameter shows the subscriber COLP service status in the network
     * 0   COLP not provisioned
     * 1   COLP provisioned in permanent mode
     */
    responses[0] = (int) mipcData->getMipcUint8Val(MIPC_SS_GET_COLP_CNF_T_COLP_N, 0);
    responses[1] = (int) mipcData->getMipcUint8Val(MIPC_SS_GET_COLP_CNF_T_COLP_M, 4);

    logD(TAG, "[%s] n = %d, m = %d", __FUNCTION__, responses[0], responses[1]);

done:
    /* For SET COLP responseVoid will ignore the responses */
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);
    responseToTelCore(response);
}

int RmmSuppServRequestBaseHandler::requestColrOperation(const sp<RfxMclMessage>& msg) {
    int* n = (int *) (msg->getData()->getData());

    if (msg->getData()->getDataLength() != 0) {
        /**
         * Set COLR:
         */
        sp<RfxMipcData> setColrMipc = RfxMipcData::obtainMipcData(MIPC_SS_SET_COLR_REQ, msg->getSlotId());
        setColrMipc->addMipcTlvUint8(MIPC_SS_SET_COLR_REQ_T_N_VALUE, (uint8_t) n[0]);

        logD(TAG, "[%s] setColr:%d", __FUNCTION__, n[0]);

        return callToMipcMsgAsync(msg, setColrMipc);
    } else {
        logD(TAG, "[%s] getColr", __FUNCTION__);

        return requestMipcVoid(msg, MIPC_SS_GET_COLR_REQ);
    }
}

void RmmSuppServRequestBaseHandler::handleColrOperationCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int responses[2] = {0};

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    if (RIL_E_SUCCESS != ret) {
        if (RFX_MSG_REQUEST_SET_COLR == msg->getId()) {
            handleErrorMessage(mipcData, MIPC_SS_SET_COLR_CNF_T_ERRMESSAGE);
        } else {
            handleErrorMessage(mipcData, MIPC_SS_GET_COLR_CNF_T_ERRMESSAGE);
        }
    }

    if (RIL_E_SUCCESS != ret || RFX_MSG_REQUEST_SET_COLR == msg->getId()) {
        goto done;
    }

    // query COLR
    /**
     * <n> parameter sets the adjustment for outgoing calls
     * 0   COLR not provisioned
     * 1   COLR provisioned
     * 2   unknown
     */
    responses[0] = (int) mipcData->getMipcUint8Val(MIPC_SS_GET_COLR_CNF_T_STATUS, 0);

    logD(TAG, "[%s] n = %d", __FUNCTION__, responses[0]);

done:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData(&responses[0], sizeof(responses[0])/sizeof(int)), msg, false);
    responseToTelCore(response);
}

/**
 * This command refers to the supplementary service CNAP (Calling Name Presentation)
 * according to 3GPP TS 22.096 that enables a called subscriber to get a calling name
 * indication (CNI) of the calling party when receiving a mobile terminated call.
 *
 * Set command enables or disables the presentation of the CNI at the TE.
 * It has no effect on the execution of the supplementary service CNAP in the network.
 * When <n>=1, the presentation of the calling name indication at the TE is enabled and
 * CNI is provided the unsolicited result code. Read command gives the status of<n>,
 * and also triggers an interrogation of the provision status of the CNAP service
 * according 3GPP TS 22.096 (given in <m>). Test command returns values supported
 * as a compound value.
 */
int RmmSuppServRequestBaseHandler::requestSendCnap(const sp<RfxMclMessage>& msg) {
    // Block CNAP request if during VoLTE call
    int wfcState = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_STATE, -1);
    bool isInImsCall = getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_IMS_CALL_EXIST, false);
    if (isInImsCall && wfcState != 1) {
        logD(TAG, "[%s] return error because there is a VoLTE call ongoing", __FUNCTION__);
        return RIL_E_GENERIC_FAILURE;
    }

    /**
     * AT+ECUSD=<m>,<n>,<str>
     * <m>: 1 for SS, 2 for USSD
     * <n>: 1 for execute SS or USSD, 2 for cancel USSD session
     * <str>: string type parameter, the SS or USSD string
     * AT+ECUSD=1,1,"*#300#"
     */
    return requestMipcVoid(msg, MIPC_SS_SEND_CNAP_REQ);
}

void RmmSuppServRequestBaseHandler::handleSendCnapCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int responses[2] = {0};

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);
    logD(TAG, "[%s] result %d", __FUNCTION__, ret);
    if (RIL_E_SUCCESS != ret) {
        goto done;
    }

    responses[0] = (int) mipcData->getMipcUint8Val(MIPC_SS_SEND_CNAP_CNF_T_CNAP_N, 0);
    responses[1] = (int) mipcData->getMipcUint8Val(MIPC_SS_SEND_CNAP_CNF_T_CNAP_M, 0);

    logD(TAG, "[%s] n = %d, m = %d", __FUNCTION__, responses[0], responses[1]);

done:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData(responses, sizeof(responses)/sizeof(int)), msg, false);
    responseToTelCore(response);
}

int RmmSuppServRequestBaseHandler::requestSetSuppSvcNotification(const sp<RfxMclMessage>& msg) {
    int *n = (int *) (msg->getData()->getData());

    sp<RfxMipcData> scvMipc = RfxMipcData::obtainMipcData(MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ,
            msg->getSlotId());

    if (msg->getData()->getDataLength() != 0) {
        logD(TAG, "[%s] n = %d", __FUNCTION__, n[0]);
        scvMipc->addMipcTlvUint8(MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ_T_STATUS_I, (uint8_t) n[0]);
        scvMipc->addMipcTlvUint8(MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ_T_STATUS_U, (uint8_t) n[0]);
    } else {
        return RIL_E_INVALID_ARGUMENTS;
    }

    return callToMipcMsgAsync(msg, scvMipc);
}

/**
 * AT+ECAPUAS=<str>
 * <str>:  Customized XCAP user agent string. The OEM name, model number and
 *         firmware version should be included. (e.g. "MTK/1.0.0 1.0.0")
 */
int RmmSuppServRequestBaseHandler::requestSetupXcapUserAgentString(const sp<RfxMclMessage>& msg) {
    const char** params = (const char**) (msg->getData()->getData());

    sp<RfxMipcData> setXuaMipc = RfxMipcData::obtainMipcData(MIPC_SS_SETUP_XCAP_USER_AGENT_REQ,
            msg->getSlotId());

    if (params[0] != NULL && strlen(params[0]) != 0) {
        logD(TAG, "[%s] userAgent = %s", __FUNCTION__, params[0]);
        setXuaMipc->addMipcTlvStr(MIPC_SS_SETUP_XCAP_USER_AGENT_REQ_T_STR, params[0]);
    } else {
        logE(TAG, "[%s] userAgent string is null or empty.", __FUNCTION__);

        return RIL_E_GENERIC_FAILURE;
    }

    return callToMipcMsgAsync(msg, setXuaMipc);
}

int RmmSuppServRequestBaseHandler::requestSetXcapConfig(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response = NULL;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    char **data = (char **)msg->getData()->getData();
    const char *delim = ",";
    char *savePtr = NULL;
    char *name  = NULL;
    char *value = NULL;

    sp<RfxMipcData> cnfMipc = RfxMipcData::obtainMipcData(MIPC_SS_SET_XCAP_CFG_REQ,
            msg->getSlotId());

    logD(TAG, "[%s] data1 = %s", __FUNCTION__, data[1]);

    name = strtok_r(data[1], delim, &savePtr);
    value = strtok_r(NULL, delim, &savePtr);

    if (name == NULL /* || value == NULL */) { // value is NULL when reset
        logE(TAG, "[%s] SetXcapConfig name/value is NULL", __FUNCTION__);
        return RIL_E_INVALID_ARGUMENTS;
    }

    logD(TAG, "[%s] name = %s, value = %s", __FUNCTION__, name, value);

    /// always true form Gen95
    int name_length = strlen(name);
    int prefix_length = strlen("persist.vendor.ss.");
    int config_length = name_length - prefix_length;
    char* config_name = (char *) alloca(config_length + 1);
    if (config_name == NULL) {
        logE(TAG, "[%s] Out of memory", __FUNCTION__);
        return RIL_E_GENERIC_FAILURE;
    }

    strncpy(config_name, name + prefix_length, config_length);
    config_name[config_length] = '\0';

    logD(TAG, "[%s] sub_name = %s, value = %s", __FUNCTION__, config_name, value);

    if (value != NULL && strlen(value) > 0) {
        rfx_property_set(name, value);
        if (!isResetSession) {
            cnfMipc->addMipcTlvStr(MIPC_SS_SET_XCAP_CFG_REQ_T_CFG_NAME, config_name);
            cnfMipc->addMipcTlvStr(MIPC_SS_SET_XCAP_CFG_REQ_T_VALUE, value);

            ret = (RIL_Errno) callToMipcMsgAsync(msg, cnfMipc);
            goto done;
        } else {
            // In reset session no need to send AT cmd to momem.
            ret = RIL_E_SUCCESS;
        }
    } else {
        if (strcmp(config_name, "reset") == 0) {
            logD(TAG, "[%s] reset session start", __FUNCTION__);
            isResetSession = true;
            cnfMipc->addMipcTlvStr(MIPC_SS_SET_XCAP_CFG_REQ_T_CFG_NAME, config_name);
            cnfMipc->addMipcTlvStr(MIPC_SS_SET_XCAP_CFG_REQ_T_VALUE, "");

            ret = (RIL_Errno) callToMipcMsgAsync(msg, cnfMipc);
            goto done;
        } else if (strcmp(config_name, "resetdone") == 0) {
            logD(TAG, "[%s] reset session end", __FUNCTION__);
            isResetSession = false;
            ret = RIL_E_SUCCESS;
        } else {
            rfx_property_set(name, "");
            ret = RIL_E_SUCCESS;
        }
    }

    response = RfxMclMessage::obtainResponse(RFX_MSG_REQUEST_OEM_HOOK_STRINGS,
            ret, RfxVoidData(), msg);
    responseToTelCore(response);

done:
    if (ret != RIL_E_SUCCESS) {
        response = RfxMclMessage::obtainResponse(
                RFX_MSG_REQUEST_OEM_HOOK_STRINGS, ret, RfxVoidData(), msg);
        responseToTelCore(response);
    }

    return ret;
}

int RmmSuppServRequestBaseHandler::requestGetXcapStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> callForwardQueryMipc = RfxMipcData::obtainMipcData(
            MIPC_SS_QUERY_CALL_FORWARD_REQ, msg->getSlotId());

    callForwardQueryMipc->addMipcTlvUint8(
            MIPC_SS_QUERY_CALL_FORWARD_REQ_T_CALL_FORWARD_REASON,
            MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_UNCONDITIONAL);

    callForwardQueryMipc->addMipcTlvUint16(
            MIPC_SS_QUERY_CALL_FORWARD_REQ_T_SERVICE_CLASS,
            MIPC_SS_SERVICE_CLASS_VOICE);
    logD(TAG, "[%s]", __FUNCTION__);

    return callToMipcMsgAsync(msg, callForwardQueryMipc);
}


int RmmSuppServRequestBaseHandler::requestGetBarringCalls(const sp<RfxMclMessage>& msg) {
    int *n = (int *)msg->getData()->getData();
    logD(TAG, "[%s] serviceClass = %d", __FUNCTION__, n[0]);

    sp<RfxMipcData> callBarringMipc = RfxMipcData::obtainMipcData(MIPC_SS_QUERY_CALL_BARRING_REQ,
            msg->getSlotId());

    char feature[] = "barring specific call";
    int featureVersion = getFeatureVersion(feature);
    if (featureVersion == 0) {
        logE(TAG, "[%s] feature not support", __FUNCTION__);
        return RIL_E_GENERIC_FAILURE;
    }

    callBarringMipc->addMipcTlvUint8(
            MIPC_SS_QUERY_CALL_BARRING_REQ_T_FACILITY,
            MIPC_SS_CALL_BARRING_FAC_MT);
    callBarringMipc->addMipcTlvUint16(
            MIPC_SS_QUERY_CALL_BARRING_REQ_T_SERVICE_CLASS,
            (uint16_t) n[0]);

    return callToMipcMsgAsync(msg, callBarringMipc);
}

void RmmSuppServRequestBaseHandler::handleGetBarringCallsCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    int count = -1;
    RIL_BarringCallInfo **pp_BarringCall = NULL;
    mipc_ss_cb_dialnumber_struct4 *p_mipcCfInfo = NULL;
    uint16_t len = 0;

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    logD(TAG, "[%s] ret = %d", __FUNCTION__, ret);
    if (RIL_E_SUCCESS != ret) {
        handleErrorMessage(mipcData, MIPC_SS_QUERY_CALL_BARRING_CNF_T_ERRMESSAGE);
        goto done;
    }

    count = (int) mipcData->getMipcUint8Val(
            MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_COUNT, 0);

    logD(TAG, "[%s] count = %d", __FUNCTION__, count);
    if (count == 0) {
        goto done;
    }

    pp_BarringCall = (RIL_BarringCallInfo **) alloca(count * sizeof(RIL_BarringCallInfo *));
    if (pp_BarringCall == NULL) {
        logD(TAG, "[%s] pp_BarringCall malloc failed", __FUNCTION__);
        count = 0;
        goto done;
    }
    memset(pp_BarringCall, 0, count * sizeof(RIL_BarringCallInfo *));

    for (int i = 0; i < count; i++) {
        p_mipcCfInfo = (mipc_ss_cb_dialnumber_struct4 *) mipcData->getMipcIdxVal(
                MIPC_SS_QUERY_CALL_BARRING_CNF_T_DIAL_NUMBER_LIST, i, &len);

        if (len != sizeof(mipc_ss_cb_dialnumber_struct4)) {
            logD(TAG, "[%s] len error, %u!, %zu", __FUNCTION__, len, sizeof(mipc_ss_cb_dialnumber_struct4));
            count = (i > 0 ? i-- : 0);
            break;
        }
        pp_BarringCall[i] = (RIL_BarringCallInfo *) alloca(sizeof(RIL_BarringCallInfo));

        if (pp_BarringCall[i] == NULL) {
            logD(TAG, "[%s] pp_BarringCall malloc failed", __FUNCTION__);
            count = (i > 0 ? i-- : 0);
            goto done;
        }
        memset(pp_BarringCall[i], 0, sizeof(RIL_BarringCallInfo));

        pp_BarringCall[i]->status = (int) p_mipcCfInfo->number_status;
        pp_BarringCall[i]->number = p_mipcCfInfo->dial_number;
        logD(TAG, "[%s] status = %d, number = %s", __FUNCTION__,
                pp_BarringCall[i]->status, pp_BarringCall[i]->number);
    }

done:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxBarringCallInfosData(pp_BarringCall, count * sizeof(RIL_BarringCallInfo *)),
            msg, false);
    responseToTelCore(response);
}

int RmmSuppServRequestBaseHandler::requestSetBarringCalls(const sp<RfxMclMessage>& msg) {
    RIL_BarringCallInfo **data = (RIL_BarringCallInfo **)msg->getData()->getData();
    RfxBarringCallInfosData *pRfxdata = (RfxBarringCallInfosData *)msg->getData();
    int count = pRfxdata->getDataLength() / sizeof(RIL_BarringCallInfo *);

    sp<RfxMipcData> callBarringMipc = RfxMipcData::obtainMipcData(MIPC_SS_SET_CALL_BARRING_REQ,
            msg->getSlotId());
    uint8_t lock = MIPC_SS_CALL_BARRING_LOCK_LOCK; // 0: unlock, 1: lock
    char* p_NumList[MIPC_MAX_SS_DIAL_COUNT];

    char feature[] = "barring specific call";
    int featureVersion = getFeatureVersion(feature);
    if (featureVersion == 0) {
        logE(TAG, "[%s] feature not support", __FUNCTION__);
        return RIL_E_GENERIC_FAILURE;
    }

    for (int i = 0; i < MIPC_MAX_SS_DIAL_COUNT; i++) {
        p_NumList[i] = NULL;
    }

    if (count == 0) {
        logE(TAG, "[%s] count is zero!", __FUNCTION__);
        return RIL_E_INVALID_ARGUMENTS;
    }

    for (int i = 0; i < count; i++) {
        p_NumList[i] = data[i]->number;
        logD(TAG, "[%s] status = %d, serviceClass = %d, toa = %d, number = %s", __FUNCTION__,
                data[i]->status, data[i]->serviceClass, data[i]->toa, data[i]->number);
    }

    // activate/deactivate
    lock = data[0]->status;

    logD(TAG, "[%s] count = %d", __FUNCTION__, count);

    callBarringMipc->addMipcTlvUint8(
            MIPC_SS_QUERY_CALL_BARRING_REQ_T_FACILITY,
            MIPC_SS_CALL_BARRING_FAC_MT);
    callBarringMipc->addMipcTlvUint8(
            MIPC_SS_SET_CALL_BARRING_REQ_T_LOCK,
            lock);
    callBarringMipc->addMipcTlvUint8(
            MIPC_SS_SET_CALL_BARRING_REQ_T_DIAL_NUMBER_COUNT,
            (uint8_t) count);
    callBarringMipc->addMipcTlvArrayStr(
            MIPC_SS_SET_CALL_BARRING_REQ_T_DIAL_NUMBER_LIST,
            (uint16_t) count,
            p_NumList);

    return callToMipcMsgAsync(msg, callBarringMipc);
}

void RmmSuppServRequestBaseHandler::handleSetBarringCallsCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    RIL_Errno ret = RIL_E_INTERNAL_ERR;

    ret = convertMipcResultToRil(__FUNCTION__, mipcRet);

    logD(TAG, "[%s] ret = %d", __FUNCTION__, ret);
    if (RIL_E_SUCCESS != ret) {
        handleErrorMessage(mipcData, MIPC_SS_SET_CALL_BARRING_CNF_T_ERRMESSAGE);
    }

    /* For SET Facility Lock responseVoid will ignore the responses */
    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(responseMsg);
}

void RmmSuppServRequestBaseHandler::handleErrorMessage(RfxMipcData* mipcData, uint16_t errNo) {
    mipc_result_const_enum mipcRet = (mipc_result_const_enum) mipcData->getResult();
    char* errMsg = mipcData->getMipcStr(errNo);

    if (errMsg != NULL) {
        logD(TAG, "[%s] errmsg:%s", __FUNCTION__, errMsg);
        setErrorMessageFromXcap(m_slot_id,
                (AT_CME_Error) (mipcRet & (~MIPC_RESULT_SS_EXT_BEGIN)), errMsg);
    }
}

void RmmSuppServRequestBaseHandler::sleepMsec(long long msec) {
    struct timespec ts;
    int err;

    ts.tv_sec = (msec / 1000);
    ts.tv_nsec = (msec % 1000) * 1000 * 1000;

    do
        err = nanosleep(&ts, &ts);
    while (err < 0 && errno == EINTR);
}

