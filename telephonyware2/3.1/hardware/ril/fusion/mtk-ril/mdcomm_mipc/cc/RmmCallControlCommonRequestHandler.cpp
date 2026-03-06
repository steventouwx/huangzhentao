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
// MTK fusion include
#include <telephony/mtk_ril.h>
#include "RfxCallListData.h"
#include "RfxDispatchThread.h"
#include "RfxRedialData.h"
#include "RfxVoidData.h"
#include "RfxCallFailCauseData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxRilUtils.h"

#include "RfxViaUtils.h"

#include "RfxGwsdUtils.h"

// local include
#include "RmmCallControlCommonRequestHandler.h"
#include "RmmCallControlUrcHandler.h"

#include <mtkconfigutils.h>


#define RFX_LOG_TAG "RmmCCReqHandler"

#define MAX_CDMA_FLASH_NUMBER_LENGTH 15

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmCallControlCommonRequestHandler, RIL_CMD_PROXY_2);

RmmCallControlCommonRequestHandler::RmmCallControlCommonRequestHandler(int slot_id,
        int channel_id) : RmmCallControlBaseHandler(slot_id, channel_id){
    const int requests[] = {
        RFX_MSG_REQUEST_GET_CURRENT_CALLS,         //AT+CLCC
        RFX_MSG_REQUEST_DIAL,                      //ATD
        RFX_MSG_REQUEST_ANSWER,                    //ATA
        RFX_MSG_REQUEST_UDUB,                      //ATH
        RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE,      //AT+CEER
        RFX_MSG_REQUEST_DTMF,                      //AT+VTS
        RFX_MSG_REQUEST_DTMF_START,                //AT+EVTS
        RFX_MSG_REQUEST_DTMF_STOP,                 //AT+EVTS
        RFX_MSG_REQUEST_SET_MUTE,                  //AT+CMUT
        RFX_MSG_REQUEST_GET_MUTE,                  //AT+CMUT?
        RFX_MSG_REQUEST_SET_TTY_MODE,              //AT+CTMCALL
        RFX_MSG_REQUEST_QUERY_TTY_MODE,            //AT+CTMCALL?
        RFX_MSG_REQUEST_SET_CALL_INDICATION,       //AT+EAIC
        RFX_MSG_REQUEST_EMERGENCY_DIAL,
        RFX_MSG_REQUEST_HANGUP_ALL,                           //AT+CHLD=6
        RFX_MSG_REQUEST_HANGUP,                               //AT+CHLD=1X
        RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND,         //AT+CHLD=0
        RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND,  //AT+CHLD=1
        RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE, //AT+CHLD=2
        RFX_MSG_REQUEST_CONFERENCE,                           //AT+CHLD=3
        RFX_MSG_REQUEST_EXPLICIT_CALL_TRANSFER,               //AT+CHLD=4
        RFX_MSG_REQUEST_SEPARATE_CONNECTION,                  //AT+CHLD=2X
        RFX_MSG_REQUEST_HANGUP_WITH_REASON,                   //AT+ECHLD=1X, reason
        RFX_MSG_REQUEST_FORCE_RELEASE_CALL,                   //AT+ECHUP
        RFX_MSG_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE,         //AT+VMEMEXIT
        RFX_MSG_REQUEST_LOCAL_SET_ECC_INDICATION,
        /// C2K specific start
        RFX_MSG_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE,      //AT+VP
        RFX_MSG_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE,    //AT+VP?
        RFX_MSG_REQUEST_CDMA_FLASH,                                 //AT+CFSH
        RFX_MSG_REQUEST_CDMA_BURST_DTMF,                            //AT+VTS
        /// C2K specific end
        RFX_MSG_REQUEST_SET_CALL_SUB_ADDRESS,      //AT+ESBP=5,"SBP_SUBADDRESS_SETTING",x
        RFX_MSG_REQUEST_GET_CALL_SUB_ADDRESS,      //AT+ESBP=7,"SBP_SUBADDRESS_SETTING"
        RFX_MSG_REQUEST_AUTO_ANSWER,
        /// IMS specific start
        RFX_MSG_REQUEST_IMS_DIAL,
        RFX_MSG_REQUEST_IMS_VT_DIAL,
        RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL,
        RFX_MSG_REQUEST_CONFERENCE_DIAL,
        RFX_MSG_REQUEST_DIAL_WITH_SIP_URI,
        RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI,
        RFX_MSG_REQUEST_HOLD_CALL,
        RFX_MSG_REQUEST_RESUME_CALL,
        RFX_MSG_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER,
        RFX_MSG_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER,
        RFX_MSG_REQUEST_PULL_CALL,
        RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT,
        RFX_MSG_REQUEST_ECC_REDIAL_APPROVE,
        RFX_MSG_REQUEST_IMS_ECT,
        RFX_MSG_REQUEST_ASYNC_HOLD_CALL,
        RFX_MSG_REQUEST_ASYNC_RESUME_CALL,
        RFX_MSG_REQUEST_SET_RTT_MODE,
        RFX_MSG_REQUEST_SET_SIP_HEADER,
        RFX_MSG_REQUEST_SIP_HEADER_REPORT,
        RFX_MSG_REQUEST_SET_IMS_CALL_MODE,
        RFX_MSG_REQUEST_SET_CALL_ADDITIONAL_INFO,
        RFX_MSG_REQUEST_SET_VOICE_DOMAIN_PREFERENCE,
        RFX_MSG_REQUEST_GET_VOICE_DOMAIN_PREFERENCE,
        /// IMS specific end
    };

    const int events[] = {
        RFX_MSG_EVENT_FORWARDED_NUMBER,
        RFX_MSG_EVENT_CLEAR_FORWARDED_NUMBER,
    };

    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
    registerToHandleEvent(events, sizeof(events) / sizeof(int));
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        RfxGwsdUtils::getCallControlHandler()->registerForGwsdRequest(this);
    }
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        RfxGwsdUtils::getCallControlHandler()->registerForGwsdEvent(this);
    }


    updateCallDataPreferSetting();
    rfx_property_set("vendor.ril.call.emci_support", "1");

    bUseLocalCallFailCause = 0;
    dialLastError = 0;
}

RmmCallControlCommonRequestHandler::~RmmCallControlCommonRequestHandler() {
}

void RmmCallControlCommonRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(RFX_LOG_TAG, "onHandleRequest: %s", RFX_ID_TO_STR(msg->getId()));
    }

    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        if (RfxGwsdUtils::getCallControlHandler()->handleGwsdRequest(this, msg)) {
            return;
        }
    }

    int ret = 0;
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_GET_CURRENT_CALLS:
            ret = requestGetCurrentCalls(msg);
            break;

        case RFX_MSG_REQUEST_DIAL:
        case RFX_MSG_REQUEST_IMS_DIAL:
            ret = requestDial(msg, false, false, (request == RFX_MSG_REQUEST_IMS_DIAL));
            break;

        case RFX_MSG_REQUEST_ANSWER:
            ret = requestAnswer(msg);
            break;

        case RFX_MSG_REQUEST_UDUB:
            ret = requestUdub(msg);
            break;

        case RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE:
            ret = requestLastCallFailCause(msg);
            break;

        case RFX_MSG_REQUEST_DTMF:
            ret = requestDtmf(msg);
            break;

        case RFX_MSG_REQUEST_DTMF_START:
            ret = requestDtmfStart(msg);
            break;

        case RFX_MSG_REQUEST_DTMF_STOP:
            ret = requestDtmfStop(msg);
            break;

        case RFX_MSG_REQUEST_SET_MUTE:
            requestSetMute(msg);
            break;

        case RFX_MSG_REQUEST_GET_MUTE:
            requestGetMute(msg);
            break;

        case RFX_MSG_REQUEST_SET_TTY_MODE:
            ret = requestSetTtyMode(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_TTY_MODE:
            ret = requestQueryTtyMode(msg);
            break;

        case RFX_MSG_REQUEST_SET_CALL_INDICATION:
            requestSetCallIndication(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP_ALL:
            ret = requestHangupAll(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP:
            ret = requestHangup(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND:
            if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false)) {
                logD(RFX_LOG_TAG, "flip hangup background to foreground");
                ret = requestHangupForegroundResumeBackground(msg);
                getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false);
                break;
            }
            ret = requestHangupWaitingOrBackground(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND:
            if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false)) {
                logD(RFX_LOG_TAG, "flip hangup foreground to background");
                ret = requestHangupWaitingOrBackground(msg);
                getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false);
                break;
            }
            ret = requestHangupForegroundResumeBackground(msg);
            break;

        case RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
            ret = requestSwitchWaitingOrHoldingAndActive(msg);
            break;

        case RFX_MSG_REQUEST_CONFERENCE:
            ret = requestConference(msg);
            break;

        case RFX_MSG_REQUEST_EXPLICIT_CALL_TRANSFER:
            ret = requestExplicitCallTransfer(msg);
            break;

        case RFX_MSG_REQUEST_SEPARATE_CONNECTION:
            ret = requestSeparateConnection(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP_WITH_REASON:
            ret = requestHangupWithReason(msg);
            break;

        case RFX_MSG_REQUEST_FORCE_RELEASE_CALL:
            ret = requestForceReleaseCall(msg);
            break;

        case RFX_MSG_REQUEST_EMERGENCY_DIAL:
        case RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL:
            ret = requestDial(msg, true, false, (request == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL));
            break;

        case RFX_MSG_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE:
            ret = requestExitEmergencyCallbackMode(msg);
            break;

        case RFX_MSG_REQUEST_LOCAL_SET_ECC_INDICATION:
            ret = requestLocalSetEccIndication(msg);
            break;

        /// C2K specific start
        case RFX_MSG_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE:
            requestSetPreferredVoicePrivacyMode(msg);
            break;

        case RFX_MSG_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE:
            requestQueryPreferredVoicePrivacyMode(msg);
            break;

        case RFX_MSG_REQUEST_CDMA_FLASH:
            ret = requestFlash(msg);
            break;

        case RFX_MSG_REQUEST_CDMA_BURST_DTMF:
            ret = requestBurstDtmf(msg);
            break;
        /// C2K specific end

        case RFX_MSG_REQUEST_SET_CALL_SUB_ADDRESS:
            ret = requestSetCallSubAddress(msg);
            break;

        case RFX_MSG_REQUEST_GET_CALL_SUB_ADDRESS:
            ret = requestGetCallSubAddress(msg);
            break;

        case RFX_MSG_REQUEST_AUTO_ANSWER:
            ret = updateAutoAnswerSetting(msg);
            break;

        /// IMS specific start
        case RFX_MSG_REQUEST_IMS_VT_DIAL:
            ret = requestDial(msg, false, true, true);
            break;

        case RFX_MSG_REQUEST_CONFERENCE_DIAL:
            ret = requestConferenceDial(msg);
            break;

        case RFX_MSG_REQUEST_DIAL_WITH_SIP_URI:
        case RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI:
            ret = requestDialWithSipUri(msg, (request == RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI));
            break;

        case RFX_MSG_REQUEST_HOLD_CALL:
        case RFX_MSG_REQUEST_RESUME_CALL:
        case RFX_MSG_REQUEST_ASYNC_HOLD_CALL:
        case RFX_MSG_REQUEST_ASYNC_RESUME_CALL:
            ret = requestControlCall(msg, request);
            break;

        case RFX_MSG_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER:
        case RFX_MSG_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER:
            ret = requestOperateImsConference(msg, request);
            break;

        case RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT:
            ret = requestAnswerVideoCall(msg);
            break;

        case RFX_MSG_REQUEST_ECC_REDIAL_APPROVE:
            ret = requestApproveEccRedial(msg);
            break;

        case RFX_MSG_REQUEST_IMS_ECT:
            ret = requestImsECT(msg);
            break;

        case RFX_MSG_REQUEST_PULL_CALL:
            ret = requestPullCall(msg);
            break;

        case RFX_MSG_REQUEST_SET_RTT_MODE:
            ret = requestSetRttMode(msg);
            break;

        case RFX_MSG_REQUEST_SET_SIP_HEADER:
            ret = requestSetSipHeader(msg);
            break;

        case RFX_MSG_REQUEST_SIP_HEADER_REPORT:
            ret = requestEnableSipHeaderReport(msg);
            break;

        case RFX_MSG_REQUEST_SET_IMS_CALL_MODE:
            ret = requestSetImsCallMode(msg);
            break;

        case RFX_MSG_REQUEST_SET_CALL_ADDITIONAL_INFO:
            ret = requestSetImsCallAdditionalInfo(msg);
            break;

        case RFX_MSG_REQUEST_SET_VOICE_DOMAIN_PREFERENCE:
            ret = requestSetVoiceDomainPreference(msg);
            break;

        case RFX_MSG_REQUEST_GET_VOICE_DOMAIN_PREFERENCE:
            ret = requestGetVoiceDomainPreference(msg);
            break;
        /// IMS specific end

        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }

    if (ret != 0) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
    }
}

void RmmCallControlCommonRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(RFX_LOG_TAG, "onHandleEvent: %d", msg->getId());
    }

    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        if (RfxGwsdUtils::getCallControlHandler()->handleGwsdEvent(this, msg)) {
            return;
        }
    }

    int id = msg->getId();
    switch (id) {
        case RFX_MSG_EVENT_FORWARDED_NUMBER:
            mForwardedNumber = String8((char *)msg->getData()->getData());
            break;
        case RFX_MSG_EVENT_CLEAR_FORWARDED_NUMBER: {
            int *callId = (int *)(msg->getData()->getData());
            if ((*callId >= 1) && (*callId <= MAX_GSMCALL_CONNECTIONS)) {
                mForwardedNumberBuf[*callId - 1].clear();
            }
            break;
        }
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmCallControlCommonRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(RFX_LOG_TAG, "onHandleResponse: %s", RFX_ID_TO_STR(msg->getId()));
    }

    RfxMipcData* data = msg->getMipcData();
    if (data == NULL) {
        logE(RFX_LOG_TAG, "onHandleResponse: getMipcData() return null");
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    switch (msg->getId()) {
        case RFX_MSG_REQUEST_GET_CURRENT_CALLS:
            handleCurrentCallsCnf(msg);
            break;
        case RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE:
            handleLastCallFailCauseCnf(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_TTY_MODE:
            handleQueryTtyModeCnf(msg);
            break;
        case RFX_MSG_REQUEST_GET_CALL_SUB_ADDRESS:
            handleGetCallSubAddressCnf(msg);
            break;
        case RFX_MSG_REQUEST_HANGUP:
        case RFX_MSG_REQUEST_HANGUP_WITH_REASON:
            handleHangupCnf(msg);
            break;
        /// IMS specific start
        case RFX_MSG_REQUEST_GET_VOICE_DOMAIN_PREFERENCE:
            handleGetVoiceDomainPreferenceCnf(msg);
            break;
        case RFX_MSG_REQUEST_HOLD_CALL:
        case RFX_MSG_REQUEST_RESUME_CALL:
        case RFX_MSG_REQUEST_ASYNC_HOLD_CALL:
        case RFX_MSG_REQUEST_ASYNC_RESUME_CALL:
            handleControlCallCnf(msg);
            break;
        /// IMS specific end
        default:
            responseVoidDataToTcl(msg);
            break;
    }
}

int RmmCallControlCommonRequestHandler::updateAutoAnswerSetting(const sp<RfxMclMessage>& msg) {
    String8 data = String8((char *)msg->getData()->getData());
    int enable = (data == "1") ? 1 : 0;
    logD(RFX_LOG_TAG, "MIPC_CALL_AUTO_ANSWER_REQ %d", enable);

    sp<RfxMipcData> mipcData = RfxMipcData::obtainMipcData(MIPC_CALL_AUTO_ANSWER_REQ, m_slot_id);
    mipcData->addMipcTlvUint8(MIPC_CALL_AUTO_ANSWER_REQ_T_ENABLE, enable);
    return callToMipcMsgAsync(msg, mipcData);
}

int RmmCallControlCommonRequestHandler::requestGetCurrentCalls(const sp<RfxMclMessage>& msg) {
    // GWSD
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        if (RfxGwsdUtils::getCallControlHandler()->fakeGetCurrentCalls(this, msg,
                RmmCallControlUrcHandler::getSpeechCodec())) {
            return 0;
        }
    }

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_GET_CALL_STATUS_REQ,
            msg->getSlotId());
    return callToMipcMsgAsync(msg, data);
}

void RmmCallControlCommonRequestHandler::handleCurrentCallsCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    if (data == NULL || data->getResult() != MIPC_RESULT_SUCCESS) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    uint32_t count = data->getMipcUint32Val(MIPC_CALL_GET_CALL_STATUS_CNF_T_COUNT, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_GET_CALL_STATUS count=%d", count);

    RIL_Call **pp_calls = NULL;
    RIL_Call *p_calls = NULL;
    pp_calls = (RIL_Call **)alloca(count * sizeof(RIL_Call *));
    RFX_ASSERT(pp_calls != NULL);
    p_calls = (RIL_Call *)alloca(count * sizeof(RIL_Call));
    RFX_ASSERT(p_calls != NULL);
    memset(p_calls, 0, count * sizeof(RIL_Call));
    for (int i = 0; i < count; i++) {
        pp_calls[i] = &(p_calls[i]);
    }

    RIL_Call *p_call = p_calls;
    int countValidCalls = 0;
    Vector<String8> numberBuf;
    for (int i = 0; i < count && i < MIPC_MAX_CALL_NUM; i++) {
        int callId = data->getMipcUint32IdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_CALLID, i, 0);
        int dir = data->getMipcUint32IdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_DIRECTION, i, 0);
        int mode = data->getMipcUint32IdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_MODE, i, 0);
        int state = data->getMipcUint32IdxVal(
                MIPC_CALL_GET_CALL_STATUS_CNF_T_CALL_CLCC_STATE, i, 0);
        int ton = data->getMipcUint32IdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_TON, i, 0);
        int rat = data->getMipcUint32IdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_RAT, i, 0);
        int type = data->getMipcUint32IdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_TYPE, i, 0);
        mipc_call_detail_info_struct4 *detail = (mipc_call_detail_info_struct4 *)
                data->getMipcIdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_DETAIL_INFO, i, NULL);
        uint16_t len = 0;
        char *number = (char *)data->getMipcIdxVal(MIPC_CALL_GET_CALL_STATUS_CNF_T_NUMBER, i, &len);
        numberBuf.add(String8(number, len));

        logD(RFX_LOG_TAG, "MIPC_CALL_GET_CALL_STATUS id=%d dir=%d mode=%d state=%d ton=%d number=%s rat=%d type=0x%X, presentation=%d",
                callId, dir, mode, state, ton,
                RfxRilUtils::pii(RFX_LOG_TAG, numberBuf[i]), rat, type,
                detail != NULL ? detail->number_present : 0);

        if ((mode >= MIPC_CALL_MODE_IMS_VOICE_CALL && mode < MIPC_CALL_MODE_C2K_VOICE_CALL)
                || hasImsCall(m_slot_id)) {
            logD(RFX_LOG_TAG, "Skip call with mode>=20, i.e. IMS/WFC call");
            continue;
        }
        if (mipcCallStateToRilType(state, &p_call->state) != 0) {
            logD(RFX_LOG_TAG, "Skip invalid state %d", state);
            continue;
        }
        p_call->index = callId;
        p_call->isMT = dir;
        p_call->isVoice = (mode == MIPC_CALL_MODE_VOICE) ? 1 : 0;
        p_call->isMpty = ((type & MIPC_CALL_TYPE_MPTY) != 0) ? 1 : 0;
        if (ton <= 7) {
            p_call->toa = 128 + ton * 16 + 1;
        } else {
            p_call->toa = ton;
        }
        if (number != NULL) {
            p_call->number = (char *)numberBuf[i].string();
            p_call->number = extractSipUri(p_call->number);
        }
        if (detail != NULL) {
            p_call->numberPresentation = mipcNumPresentationToRilType(detail->number_present);
            p_call->namePresentation = mipcNamePresentationToRilType(detail->name_present);
            p_call->name = detail->name;
        }
        p_call->uusInfo = NULL;
        p_call->speechCodec = RmmCallControlUrcHandler::getSpeechCodec();

        if (callId >= 1 && callId <= MAX_GSMCALL_CONNECTIONS) {
            if ((p_call->state == RIL_CALL_INCOMING || p_call->state == RIL_CALL_WAITING)
                    && mForwardedNumber.length() > 0) {
                mForwardedNumberBuf[callId - 1] = mForwardedNumber;
                mForwardedNumber.clear();
            }
            p_call->forwardedNumber = (char *)mForwardedNumberBuf[callId - 1].string();
        }

        countValidCalls++;
        p_call++;
    }

    if (count == 0) {
        RmmCallControlUrcHandler::resetSpeechCodec();
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxCallListData(pp_calls, sizeof(RIL_Call *) * countValidCalls), msg);
    responseToTelCore(response);
}

int RmmCallControlCommonRequestHandler::mipcCallStateToRilType(int state, RIL_CallState *p_state) {
    switch (state) {
    case MIPC_CALL_CLCC_STATE_ACTIVE:
        *p_state = RIL_CALL_ACTIVE;
        return 0;
    case MIPC_CALL_CLCC_STATE_HELD:
        *p_state = RIL_CALL_HOLDING;
        return 0;
    case MIPC_CALL_CLCC_STATE_DIALING:
        *p_state = RIL_CALL_DIALING;
        return 0;
    case MIPC_CALL_CLCC_STATE_ALERTING:
        *p_state = RIL_CALL_ALERTING;
        return 0;
    case MIPC_CALL_CLCC_STATE_INCOMING:
        *p_state = RIL_CALL_INCOMING;
        return 0;
    case MIPC_CALL_CLCC_STATE_WAITING:
        *p_state = RIL_CALL_WAITING;
        return 0;
    default:
        return -1;
    }
}

int RmmCallControlCommonRequestHandler::requestDial(const sp<RfxMclMessage>& msg,
        bool isEcc, bool isVt, bool isImsDial) {
    RIL_Emergency_Dial *pEccDial = NULL;
    RIL_Dial *pDial = NULL;
    int request = msg->getId();
    if (request == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL
            || request == RFX_MSG_REQUEST_EMERGENCY_DIAL) {
        pEccDial = (RIL_Emergency_Dial*)(msg->getData()->getData());
        pDial = pEccDial->dialData;
    } else {
        pDial = (RIL_Dial*)(msg->getData()->getData());
    }

    int clirMode = pDial->clir;
    if (request == RFX_MSG_REQUEST_IMS_DIAL ||
            request == RFX_MSG_REQUEST_IMS_VT_DIAL ||
            request == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
        clirMode = handleClirSpecial(isEcc, pDial->clir, pDial->address);
    }

    String8 dialString = handleNumberWithPause(pDial->address);

    if (!isValidDialString(dialString.string())) {
        bUseLocalCallFailCause = 1;
        dialLastError = CALL_FAIL_INVALID_NUMBER_FORMAT;
        responseVoidDataToTcl(msg, RIL_E_CANCELLED);
        return 0;
    }

    if (!isImsDial) {
        clirMode = handleNumberWithClirString(dialString, clirMode);
    }

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_DIAL_REQ, msg->getSlotId());
    uint32_t addrType = MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;
    uint32_t type = MIPC_CALL_DIAL_TYPE_VOICE;
    if (pEccDial != NULL && pEccDial->routing == ECC_ROUTING_EMERGENCY) {
        type = MIPC_CALL_DIAL_TYPE_EMERGENCY;
    } else if (isVt) {
        type = MIPC_CALL_DIAL_TYPE_VIDEO;
    }
    // [ALPS08011464]
    // AOSP cannot change CS call to IMS call, if framework dials CS call (isImsDial == false),
    // rild must not dial an IMS call, or the states in framework and rild are un-sync.
    uint32_t domain = getDomain(isImsDial, isEcc);
    data->addMipcTlvStr(MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS, dialString);
    data->addMipcTlvUint32(MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS_TYPE, addrType);
    data->addMipcTlvUint32(MIPC_CALL_DIAL_REQ_T_TYPE, type);
    data->addMipcTlvUint32(MIPC_CALL_DIAL_REQ_T_DOMAIN, domain);
    data->addMipcTlvUint32(MIPC_CALL_DIAL_REQ_T_ECC_RETRY_DOMAIN, getEccPreferredDomain());
    logD(RFX_LOG_TAG, "MIPC_CALL_DIAL_REQ %s addrType=%d type=%d domain=%d",
            RfxRilUtils::pii(RFX_LOG_TAG, (char *)dialString.string()), addrType, type, domain);
    if (pEccDial != NULL && type == MIPC_CALL_DIAL_TYPE_EMERGENCY) {
        logD(RFX_LOG_TAG, "MIPC_CALL_DIAL_REQ preferredDomain=%d category=%d",
                getEccPreferredDomain(), pEccDial->serviceCategory);
        data->addMipcTlvUint16(MIPC_CALL_DIAL_REQ_T_ECC_CATEGORY, pEccDial->serviceCategory);
    }
    data->addMipcTlvUint8(MIPC_CALL_DIAL_REQ_T_CLIR_EXT, clirMode);

    bUseLocalCallFailCause = 0;
    dialLastError = 0;
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestAnswer(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_ANSWER_REQ, msg->getSlotId());
    logD(RFX_LOG_TAG, "MIPC_CALL_ANSWER_REQ");
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestUdub(const sp<RfxMclMessage>& msg) {
    // VTS requirement
    int callCount = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, 0);
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
    if (callCount == 0 && cardType <= 0) {
        responseVoidDataToTcl(msg, RIL_E_INVALID_STATE);
        return 0;
    }

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_HANGUP_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_MODE, MIPC_CALL_HANGUP_MODE_HANGUP);
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_CALLID, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_HANGUP_MODE_HANGUP");
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestDtmf(const sp<RfxMclMessage>& msg) {
    char dtmf = ((char *)msg->getData()->getData())[0];
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_DTMF_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_DTMF_REQ_T_MODE, MIPC_DTMF_MODE_SINGLE_TONE);
    data->addMipcTlvStr(MIPC_CALL_DTMF_REQ_T_DIGIT, String8::format("%c", dtmf));
    logD(RFX_LOG_TAG, "MIPC_DTMF_MODE_SINGLE_TONE");
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestDtmfStart(const sp<RfxMclMessage>& msg) {
    char dtmf = ((char *)msg->getData()->getData())[0];
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_DTMF_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_DTMF_REQ_T_MODE, MIPC_DTMF_MODE_START);
    data->addMipcTlvStr(MIPC_CALL_DTMF_REQ_T_DIGIT, String8::format("%c", dtmf));
    logD(RFX_LOG_TAG, "MIPC_DTMF_MODE_START");
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestDtmfStop(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_DTMF_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_DTMF_REQ_T_MODE, MIPC_DTMF_MODE_STOP);
    logD(RFX_LOG_TAG, "MIPC_DTMF_MODE_STOP");
    return callToMipcMsgAsync(msg, data);
}

void RmmCallControlCommonRequestHandler::requestSetMute(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmmCallControlCommonRequestHandler::requestGetMute(const sp<RfxMclMessage>& msg) {
    int mute = 0;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&mute, 1), msg, false);
    responseToTelCore(response);
}

int RmmCallControlCommonRequestHandler::requestSetTtyMode(const sp<RfxMclMessage>& msg) {
    int mode = ((int *)msg->getData()->getData())[0];
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SET_TTY_MODE_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_CALL_SET_TTY_MODE_REQ_T_MODE, mode);
    logD(RFX_LOG_TAG, "MIPC_CALL_SET_TTY_MODE_REQ %d", mode);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestQueryTtyMode(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_GET_TTY_MODE_REQ, msg->getSlotId());
    logD(RFX_LOG_TAG, "MIPC_CALL_GET_TTY_MODE_REQ");
    return callToMipcMsgAsync(msg, data);
}

void RmmCallControlCommonRequestHandler::handleQueryTtyModeCnf(const sp<RfxMclMessage>& msg) {
    int mode = 0;
    RfxMipcData* data = msg->getMipcData();
    if (data->getResult() == MIPC_RESULT_SUCCESS) {
        mode = data->getMipcUint8Val(MIPC_CALL_GET_TTY_MODE_CNF_T_MODE, 0);
    }
    // VTS expects no error even SIM absent, so return RIL_E_SUCCESS always.
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&mode, 1), msg);
    responseToTelCore(response);
}

int RmmCallControlCommonRequestHandler::requestLastCallFailCause(const sp<RfxMclMessage>& msg) {
    RIL_LastCallFailCauseInfo callFailCause;
    memset(&callFailCause, 0, sizeof(RIL_LastCallFailCauseInfo));

    if (bUseLocalCallFailCause == 1) {
        callFailCause.cause_code = (RIL_LastCallFailCause)dialLastError;
        bUseLocalCallFailCause = 0;
        dialLastError = 0;
        logD(RFX_LOG_TAG, "Use local call fail cause = %d", callFailCause.cause_code);
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxCallFailCauseData(&callFailCause, sizeof(RIL_LastCallFailCauseInfo)), msg);
        responseToTelCore(response);
        return 0;
    } else {
        sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_GET_FINISH_REASON_REQ,
                msg->getSlotId());
        logD(RFX_LOG_TAG, "MIPC_CALL_GET_FINISH_REASON_REQ");
        return callToMipcMsgAsync(msg, data);
   }
}

void RmmCallControlCommonRequestHandler::handleLastCallFailCauseCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    if (data->getResult() != MIPC_RESULT_SUCCESS) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    uint16_t len = 0;
    uint32_t cause = data->getMipcUint32Val(MIPC_CALL_GET_FINISH_REASON_CNF_T_REASON, 0);
    char *causeStr = (char *)data->getMipcVal(MIPC_CALL_GET_FINISH_REASON_CNF_T_REASON_STR, &len);
    String8 vendorCause = String8(causeStr, len);

    RIL_LastCallFailCauseInfo callFailCause;
    callFailCause.cause_code = (RIL_LastCallFailCause)cause;
    callFailCause.vendor_cause = (char *)vendorCause.string();
    logD(RFX_LOG_TAG, "MIPC_CALL_GET_FINISH_REASON_CNF %d, %s",
            callFailCause.cause_code, callFailCause.vendor_cause);

    /*if there are more causes need to be translated in the future,
     * discussing with APP owner to implement this in upper layer.
     * For the hard coded value, please refer to modem code.*/
    if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED)) {
        if (callFailCause.cause_code == 1) {  // Phone is locked
            callFailCause.cause_code = CALL_FAIL_CDMA_LOCKED_UNTIL_POWER_CYCLE;
        } else if (callFailCause.cause_code == 22) {  // Call faded/dropped
            callFailCause.cause_code = CALL_FAIL_CDMA_DROP;
        } else if (callFailCause.cause_code == 23) {  // Received Intercept from base station
            callFailCause.cause_code = CALL_FAIL_CDMA_INTERCEPT;
        } else if (callFailCause.cause_code == 24) {  // Received Reorder from base station
            callFailCause.cause_code = CALL_FAIL_CDMA_REORDER;
        } else if (callFailCause.cause_code == 26) {  // Service option rejected by base station
            callFailCause.cause_code = CALL_FAIL_CDMA_SO_REJECT;
        } else if (callFailCause.cause_code == 241) {
            callFailCause.cause_code = CALL_FAIL_FDN_BLOCKED;
        } else if (callFailCause.cause_code == 0 ||  // Phone is offline
                callFailCause.cause_code == 12 ||   // Internal SW aborted the origination
                callFailCause.cause_code == 21 ||   // Phone is out of service
                callFailCause.cause_code == 27 ||   // There is incoming call
                callFailCause.cause_code == 28 ||   // Received an alert stop from base station
                callFailCause.cause_code == 30 ||   // Received end activation -OTASP calls only
                callFailCause.cause_code == 32 ||   // RUIM is not available
                callFailCause.cause_code == 99 ||   // NW directed system selection error
                callFailCause.cause_code == 100 ||  // Released by lower layer
                callFailCause.cause_code == 101 ||  // After a MS initiates a call, NW fails to respond
                callFailCause.cause_code == 102 ||  // Phone rejects an incoming call
                callFailCause.cause_code == 103 ||  // A call is rejected during the put-through process
                callFailCause.cause_code == 104 ||  // The release is from NW
                callFailCause.cause_code == 105 ||  // The phone fee is used up
                callFailCause.cause_code == 106) {  // The MS is out of the service area
            callFailCause.cause_code = CALL_FAIL_ERROR_UNSPECIFIED;
        } else {
            callFailCause.cause_code = CALL_FAIL_NORMAL;
        }
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED, false);
    } else {
        if (callFailCause.cause_code == 10)
            callFailCause.cause_code = CALL_FAIL_CALL_BARRED;
        else if (callFailCause.cause_code == 2600)
            callFailCause.cause_code = CALL_FAIL_FDN_BLOCKED;
        else if (callFailCause.cause_code == 2052)
            callFailCause.cause_code = CALL_FAIL_IMSI_UNKNOWN_IN_VLR;
        else if (callFailCause.cause_code == 2053)
            callFailCause.cause_code = CALL_FAIL_IMEI_NOT_ACCEPTED;
        else if (callFailCause.cause_code == 2004)
            callFailCause.cause_code = CALL_FAIL_ACCESS_CLASS_BLOCKED;
        else if (callFailCause.cause_code > 127
               && callFailCause.cause_code != 325
               && callFailCause.cause_code != 326
               && callFailCause.cause_code != 2165
               && callFailCause.cause_code != 380
               && callFailCause.cause_code != 999)
            callFailCause.cause_code = CALL_FAIL_ERROR_UNSPECIFIED;
    }
    logD(RFX_LOG_TAG, "RIL fail cause_code = %d, vendor_cause = %s",
            callFailCause.cause_code, callFailCause.vendor_cause);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxCallFailCauseData(&callFailCause, sizeof(RIL_LastCallFailCauseInfo)), msg);
    responseToTelCore(response);
}

void RmmCallControlCommonRequestHandler::requestSetCallIndication(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    uint8_t approve = pInt[0] == 0 ? 1 : 0;
    uint32_t callId = pInt[1];
    uint32_t seqNumber = pInt[2];

    if (msg->getData()->getDataLength() == 4 * sizeof(int)) {
        // Should not happen
        logW(RFX_LOG_TAG, "Wrong SeqNumber: %d has no txId", seqNumber);
        responseToTelCore(RfxMclMessage::obtainResponse(msg->getId(), RIL_E_INTERNAL_ERR,
                RfxVoidData(), msg));
        return;
    }
    uint16_t txId = pInt[4];
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_APPROVE_INCOMING_RSP,
            m_slot_id, txId);
    data->addMipcTlvUint8(MIPC_CALL_APPROVE_INCOMING_RSP_T_IS_APPROVE, approve);
    data->addMipcTlvUint32(MIPC_CALL_APPROVE_INCOMING_RSP_T_CALLID, callId);
    data->addMipcTlvUint32(MIPC_CALL_APPROVE_INCOMING_RSP_T_SEQ_NO, seqNumber);

    uint32_t causeToModem = -1;
    if (pInt[0] == 1) {
        if (pInt[3] == CALL_INDICATION_CAUSE_INCOMING_REJECT) {
            causeToModem = 21;
        }
    }
    if (pInt[0] == 1 && causeToModem != -1) {
        data->addMipcTlvUint32(MIPC_CALL_APPROVE_INCOMING_RSP_T_CAUSE, causeToModem);
        logD(RFX_LOG_TAG, "MIPC_CALL_APPROVE_INCOMING_RSP cause=%d", causeToModem);
    }
    logD(RFX_LOG_TAG, "MIPC_CALL_APPROVE_INCOMING_RSP approve=%d callId=%d seqNo=%d txId=%d",
            approve, callId, seqNumber, txId);
    callToMipcMsgCmdResponse(data);

    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

int RmmCallControlCommonRequestHandler::requestHangupAll(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_HANGUP_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_MODE, MIPC_CALL_HANGUP_MODE_HANGUP_ALL);
    logD(RFX_LOG_TAG, "MIPC_CALL_HANGUP_MODE_HANGUP_ALL");
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestHangup(const sp<RfxMclMessage>& msg) {
    int *pInt = (int*)msg->getData()->getData();
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_HANGUP_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_MODE, MIPC_CALL_HANGUP_MODE_HANGUP);
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_CALLID, pInt[0]);
    logD(RFX_LOG_TAG, "MIPC_CALL_HANGUP_MODE_HANGUP callId=%d", pInt[0]);
    return callToMipcMsgAsync(msg, data);
}

void RmmCallControlCommonRequestHandler::handleHangupCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    RIL_Errno err = (data != NULL && data->getResult() == MIPC_RESULT_SUCCESS) ?
            RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;
    int *callId = (int*)msg->getData()->getData();
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), err, RfxIntsData(callId, 1), msg);
    responseToTelCore(response);
}

int RmmCallControlCommonRequestHandler::requestHangupWithReason(const sp<RfxMclMessage>& msg) {
    int *pInt = (int*)msg->getData()->getData();
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_HANGUP_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_MODE, MIPC_CALL_HANGUP_MODE_HANGUP);
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_CALLID, pInt[0]);
    if (pInt[1] == INCOMING_REJECTED_NO_FORWARD) {
        data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_CAUSE,
                MIPC_CALL_HANGUP_CAUSE_CS_INCOMING_REJECTED_NO_FORWARD);
    } else {
        data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_CAUSE, pInt[1]);
    }
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestForceReleaseCall(const sp<RfxMclMessage>& msg) {
    int *pInt = (int*)msg->getData()->getData();
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_HANGUP_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_MODE, MIPC_CALL_HANGUP_MODE_FORCE_HANGUP);
    data->addMipcTlvUint32(MIPC_CALL_HANGUP_REQ_T_CALLID, pInt[0]);
    logD(RFX_LOG_TAG, "MIPC_CALL_HANGUP_MODE_FORCE_HANGUP callId=%d", pInt[0]);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestSsAction(const sp<RfxMclMessage>& msg, uint32_t action) {
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SS_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_SS_REQ_T_ACTION, action);
    if (action == MIPC_CALL_SS_ACTION_EXPLICIT_CALL_AND_TRANSFER) {
        data->addMipcTlvUint8(MIPC_CALL_SS_REQ_T_ECT_TYPE, MIPC_CALL_ECT_TYPE_CONSULTATIVE_ECT);
    }
    logD(RFX_LOG_TAG, "MIPC_CALL_SS_REQ action=%d", action);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestHangupWaitingOrBackground(const sp<RfxMclMessage>& msg) {
    return requestSsAction(msg, MIPC_CALL_SS_ACTION_RELEASE_ALL_HELD_OR_WAITING_CALL);
}

int RmmCallControlCommonRequestHandler::requestHangupForegroundResumeBackground(const sp<RfxMclMessage>& msg) {
    return requestSsAction(msg, MIPC_CALL_SS_ACTION_RELEASE_ALL_ACTIVE_AND_ACCEPT_CALL);
}

int RmmCallControlCommonRequestHandler::requestSwitchWaitingOrHoldingAndActive(const sp<RfxMclMessage>& msg) {
    return requestSsAction(msg, MIPC_CALL_SS_ACTION_PLACE_ALL_ACTIVE_CALL_ON_HOLD_AND_ACCEPT_CALL);
}

int RmmCallControlCommonRequestHandler::requestExplicitCallTransfer(const sp<RfxMclMessage>& msg) {
    return requestSsAction(msg, MIPC_CALL_SS_ACTION_EXPLICIT_CALL_AND_TRANSFER);
}

int RmmCallControlCommonRequestHandler::requestConference(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_CONFERENCE_REQ,
            msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_REQ_T_ACTION, MIPC_CALL_CONF_ACTION_MERGE);
    logD(RFX_LOG_TAG, "MIPC_CALL_CONF_ACTION_MERGE");
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestSeparateConnection(const sp<RfxMclMessage>& msg) {
    int *pInt = (int*)msg->getData()->getData();
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_CONFERENCE_REQ,
            msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_REQ_T_ACTION, MIPC_CALL_CONF_ACTION_SPLIT);
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_REQ_T_TARGET_CALLID, pInt[0]);
    logD(RFX_LOG_TAG, "MIPC_CALL_CONF_ACTION_SPLIT callId=%d", pInt[0]);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestLocalSetEccIndication(const sp<RfxMclMessage>& msg) {
    int *pInt = (int*)msg->getData()->getData();
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ,
            msg->getSlotId());
    data->addMipcTlvUint8(MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ_T_IS_FLIGHT_MODE, *pInt);
    logD(RFX_LOG_TAG, "MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ %d", pInt[0]);
    return callToMipcMsgAsync(msg, data);
}

/// C2K specific start
void RmmCallControlCommonRequestHandler::requestSetPreferredVoicePrivacyMode(const sp<RfxMclMessage>& msg) {
    RIL_Errno err;
    int *pInt = (int *)msg->getData()->getData();

    if (RfxViaUtils::getViaHandler() != NULL) {
        RfxViaUtils::getViaHandler()->requestSetPreferredVoicePrivacyMode(this, pInt[0], &err);
    } else {
        err = RIL_E_SUCCESS;
        logE(RFX_LOG_TAG, "Load don't support C2K rat!!/n");
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), err,
            RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmmCallControlCommonRequestHandler::requestQueryPreferredVoicePrivacyMode(const sp<RfxMclMessage>& msg) {
    RIL_Errno err;
    int result = 0;

    if (RfxViaUtils::getViaHandler() != NULL) {
        RfxViaUtils::getViaHandler()->requestQueryPreferredVoicePrivacyMode(this, &result, &err);
    } else {
        err = RIL_E_SUCCESS;
        logE(RFX_LOG_TAG, "Load don't support C2K rat!!/n");
    }

    if (err != RIL_E_SUCCESS) {
        responseVoidDataToTcl(msg, err);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData((void *)&result, sizeof(int)), msg);
    responseToTelCore(response);
}

int RmmCallControlCommonRequestHandler::requestFlash(const sp<RfxMclMessage>& msg) {
    char *number = (char *)msg->getData()->getData();
    String8 origNumber = String8();
    if (number != NULL && strlen(number) > 0) {
        if (isNumberIncludePause(number)) {
            origNumber = handleNumberWithPause(number);
        } else {
            origNumber = String8(number);
        }
        if (origNumber.length() > MAX_CDMA_FLASH_NUMBER_LENGTH) {
            origNumber = String8(origNumber, MAX_CDMA_FLASH_NUMBER_LENGTH);
        }
    }

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_CONFERENCE_REQ,
            msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_REQ_T_ACTION, MIPC_CALL_CONF_ACTION_CDMA_FLASH);
    if (origNumber.length() != 0) {
        data->addMipcTlvStr(MIPC_CALL_CONFERENCE_REQ_T_NUMBER, origNumber.string());
    }
    logD(RFX_LOG_TAG, "MIPC_CALL_CONF_ACTION_CDMA_FLASH %s",
            RfxRilUtils::pii(RFX_LOG_TAG, origNumber));
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestBurstDtmf(const sp<RfxMclMessage>& msg) {
    if (msg->getData()->getData() == NULL) {
        return -1;
    }
    char *dtmf = ((char **)msg->getData()->getData())[0];
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_DTMF_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_DTMF_REQ_T_MODE, MIPC_DTMF_MODE_BURST_TONE);
    data->addMipcTlvStr(MIPC_CALL_DTMF_REQ_T_DIGIT, dtmf);
    logD(RFX_LOG_TAG, "MIPC_DTMF_MODE_BURST_TONE");
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestExitEmergencyCallbackMode(const sp<RfxMclMessage>& msg) {
    // When MD is off, respond success directly.
    if (getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false)) {
        logE(RFX_LOG_TAG, "%s MD off, just return success!", __FUNCTION__);
        responseVoidDataToTcl(msg, RIL_E_SUCCESS);
        return 0;
    }
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SET_EXIT_ECBM_MODE_REQ,
            msg->getSlotId());
    return callToMipcMsgAsync(msg, data);
}
/// C2K specific end

bool RmmCallControlCommonRequestHandler::isNumberIncludePause(char* number) {
    char* tmp = strchr(number, ',');
    if (tmp == NULL) {
        return false;
    }
    return true;
}

bool RmmCallControlCommonRequestHandler::isValidDialString(const char* dialString) {
    if (dialString == NULL || strlen(dialString) > 40) return false;
    return true;
}

// [Softbank requirement]: Enable or disable call sub address feature.
int RmmCallControlCommonRequestHandler::requestSetCallSubAddress(const sp<RfxMclMessage>& msg) {
    int *pInt = (int*)msg->getData()->getData();
    int enable = (pInt[0] == 0) ? 0 : 1;

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, msg->getSlotId());
    data->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD,
            String8::format("AT+ESBP=5,\"SBP_SUBADDRESS_SETTING\",%d", enable));
    return callToMipcMsgAsync(msg, data);
}

// [Softbank requirement]: Query call sub address feature enable/disable status
int RmmCallControlCommonRequestHandler::requestGetCallSubAddress(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, msg->getSlotId());
    data->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD, "AT+ESBP=7,\"SBP_SUBADDRESS_SETTING\"");
    logD(RFX_LOG_TAG, "requestGetCallSubAddress");
    return callToMipcMsgAsync(msg, data);
}

void RmmCallControlCommonRequestHandler::handleGetCallSubAddressCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    if (data->getResult() != MIPC_RESULT_SUCCESS) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    char *atCnf = data->getMipcStr(MIPC_SYS_AT_CNF_T_ATCMD);
    if (atCnf == NULL) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    int err;
    RfxAtLine line = RfxAtLine(atCnf, NULL);
    line.atTokStart(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "requestGetCallSubAddress atTokStart error:%d", err);
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }
    int enabled = line.atTokNextint(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "requestGetCallSubAddress atTokNextint error:%d", err);
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }
    logD(RFX_LOG_TAG, "requestGetCallSubAddress, enabled=%d", enabled);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&enabled, 1), msg);
    responseToTelCore(response);
}

void RmmCallControlCommonRequestHandler::updateCallDataPreferSetting() {
    // persist.vendor.radio.gprs.prefer:
    // 0 : call prefer
    // 1 : data prefer
    char gprsPrefer[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.radio.gprs.prefer", gprsPrefer, "0");
    int callPrefer = (atoi(gprsPrefer) == 0) ? 1 : 0;
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_DATA_PREFER_SET_REQ, m_slot_id);
    data->addMipcTlvUint8(MIPC_CALL_DATA_PREFER_SET_REQ_T_STATE, callPrefer);
    data->addMipcTlvUint8(MIPC_CALL_DATA_PREFER_SET_REQ_T_MODE, callPrefer);
    logD(RFX_LOG_TAG, "MIPC_CALL_DATA_PREFER_SET_REQ %d", callPrefer);
    sp<RfxMipcData> result = callToMipcMsgSync(data);
    if (result == NULL || result->getResult() != MIPC_RESULT_SUCCESS) {
        // No error handling can be done
        logW(RFX_LOG_TAG, "MIPC_CALL_DATA_PREFER_SET_REQ error %d", result->getResult());
    }
}

/// IMS specific start
int RmmCallControlCommonRequestHandler::requestControlCall(const sp<RfxMclMessage>& msg, int request) {
    int *params = (int *)msg->getData()->getData();
    int callId = params[0];
    int operation = (request == RFX_MSG_REQUEST_HOLD_CALL ||
            request == RFX_MSG_REQUEST_ASYNC_HOLD_CALL) ? 131 : 132;
    logD(RFX_LOG_TAG, "MIPC_CALL_SS_REQ AT> AT+ECCTRL=%d,%d", callId, operation);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SS_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_SS_REQ_T_CALLID, callId);
    data->addMipcTlvUint32(MIPC_CALL_SS_REQ_T_ACTION, operation);
    return callToMipcMsgAsync(msg, data);
}

void RmmCallControlCommonRequestHandler::handleControlCallCnf(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    RIL_Errno result = RIL_E_SUCCESS;
    if (data->getResult() != MIPC_RESULT_SUCCESS) {
        result = RIL_E_GENERIC_FAILURE;
        if (data->getResult() == CME_HOLD_FAILED_CAUSED_BY_TERMINATED) {
            result = RIL_E_ERROR_IMS_CONTROL_CALL_FAILED_CALL_TERMINATED;
        } else if (data->getResult() == MIPC_RESULT_CC_CALL_IS_NOT_EXIST) {
            result = RIL_E_ERROR_IMS_CONTROL_CALL_FAILED_CALL_IS_NOT_EXIST;
        }
    }
    responseVoidDataToTcl(msg, result);
}

int RmmCallControlCommonRequestHandler::requestAnswerVideoCall(const sp<RfxMclMessage>& msg) {
    int* params = (int*)msg->getData()->getData();
    int mode = params[0];
    int callId = params[1];
    if (mode == 0) {
        logD(RFX_LOG_TAG, "MIPC_CALL_ANSWER_REQ AT> ATA");
    } else {
        logD(RFX_LOG_TAG, "MIPC_CALL_ANSWER_REQ AT> AT+EVTA=%d,%d", mode, callId);
    }

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_ANSWER_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_ANSWER_REQ_T_MODE, mode);
    data->addMipcTlvUint32(MIPC_CALL_ANSWER_REQ_T_CALLID, callId);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestConferenceDial(const sp<RfxMclMessage>& msg) {
    char** params = (char **)msg->getData()->getData();
    int type = atoi(params[0]);
    int count = atoi(params[1]);
    int clirMode = atoi(params[count + 2]);
    int offSet = 2;
    const int maxCount = 5;
    char* addresses[maxCount];
    for (int i = 0; i < maxCount; ++i) {
        addresses[i] = NULL;
    }
    if (count > maxCount) {
        logE(RFX_LOG_TAG, "MIPC_CALL_CONFERENCE_DIAL_REQ count = %d is wrong", count);
        return RIL_E_INVALID_ARGUMENTS;
    }
    for (int i = 0; i < count; ++i) {
        addresses[i] = params[i + offSet];
        logD(RFX_LOG_TAG, "MIPC_CALL_CONFERENCE_DIAL_REQ AT> AT+EDCONF=%d,%d,\"%s\"",
                type, i, RfxRilUtils::pii(RFX_LOG_TAG, addresses[i]));
    }

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_CONFERENCE_DIAL_REQ,
            msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_DIAL_REQ_T_TYPE,
            type == 0 ? MIPC_CALL_CONFERENCE_DIAL_TYPE_VOICE : MIPC_CALL_CONFERENCE_DIAL_TYPE_VIDEO);
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_DIAL_REQ_T_COUNT, count);
    if (clirMode != 0) {
        data->addMipcTlvUint8(MIPC_CALL_CONFERENCE_DIAL_REQ_T_CLIR,
                clirMode == 1 ? MIPC_BOOLEAN_TRUE : MIPC_BOOLEAN_FALSE);
    }
    data->addMipcTlvArrayStr(MIPC_CALL_CONFERENCE_DIAL_REQ_T_DIAL_ADDRESS, count, addresses);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestDialWithSipUri(const sp<RfxMclMessage>& msg, bool isVt) {
    logD(RFX_LOG_TAG, "MIPC_CALL_DIAL_REQ  AT> AT+CDU");
    char* sipUri = (char *)msg->getData()->getData();

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_DIAL_REQ, msg->getSlotId());
    data->addMipcTlvStr(MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS, sipUri);
    data->addMipcTlvUint32(MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS_TYPE,
            MIPC_CALL_DIAL_ADDRESS_TYPE_SIP_URI);
    data->addMipcTlvUint32(MIPC_CALL_DIAL_REQ_T_TYPE,
            isVt ? MIPC_CALL_DIAL_TYPE_VIDEO : MIPC_CALL_DIAL_TYPE_VOICE);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestOperateImsConference(const sp<RfxMclMessage>& msg, int request) {
    char **params = (char **)msg->getData()->getData();
    int hostCallId = atoi(params[0]);
    char *joinedCallNum = params[1];
    int joinedCallId = atoi(params[2]);
    int operation = (request == RFX_MSG_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER) ? 0 : 1; // For log
    logD(RFX_LOG_TAG, "MIPC_CALL_CONFERENCE_REQ AT> AT+ECONF=%d,%d,%s,%d",
            hostCallId, operation, RfxRilUtils::pii(RFX_LOG_TAG, joinedCallNum), joinedCallId);

    operation = (request == RFX_MSG_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER) ? 1 : 2;
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_CONFERENCE_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_REQ_T_CONF_CALLID, hostCallId);
    data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_REQ_T_ACTION, operation);
    data->addMipcTlvStr(MIPC_CALL_CONFERENCE_REQ_T_NUMBER, joinedCallNum);
    if (joinedCallId > 0) {
        data->addMipcTlvUint32(MIPC_CALL_CONFERENCE_REQ_T_TARGET_CALLID, joinedCallId);
    }
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestApproveEccRedial(const sp<RfxMclMessage>& msg) {
    int* params = (int*)msg->getData()->getData();
    int result = params[0];
    int callId = params[1];
    logD(RFX_LOG_TAG, "MIPC_CALL_ECC_REDIAL_APPROVE_REQ AT> AT+ERDECCAPV=%d,%d", result, callId);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_ECC_REDIAL_APPROVE_REQ,
            msg->getSlotId());
    data->addMipcTlvUint8(MIPC_CALL_ECC_REDIAL_APPROVE_REQ_T_APPROVE, result);
    data->addMipcTlvUint32(MIPC_CALL_ECC_REDIAL_APPROVE_REQ_T_CALL_ID, callId);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestPullCall(const sp<RfxMclMessage>& msg) {
    char** params = (char**)msg->getData()->getData();
    char* targetUri = params[0];
    uint8_t callType = (atoi(params[1]) == 1) ? 1 : 0;
    logD(RFX_LOG_TAG, "MIPC_CALL_PULL_REQ AT> AT+ECALLPULL");

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_PULL_REQ, msg->getSlotId());
    data->addMipcTlvStr(MIPC_CALL_PULL_REQ_T_URI, targetUri);
    data->addMipcTlvUint8(MIPC_CALL_PULL_REQ_T_TYPE, callType);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestImsECT(const sp<RfxMclMessage>& msg) {
    char** params = (char**)msg->getData()->getData();
    char* number = params[0];
    int type = atoi(params[1]);
    logD(RFX_LOG_TAG, "MIPC_CALL_SS_REQ AT> AT+ECT=%d,\"%s\"",
            type, RfxRilUtils::pii(RFX_LOG_TAG, number));

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SS_REQ, msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_SS_REQ_T_ACTION, MIPC_CALL_SS_ACTION_EXPLICIT_CALL_AND_TRANSFER);
    data->addMipcTlvUint8(MIPC_CALL_SS_REQ_T_ECT_TYPE, type);
    data->addMipcTlvStr(MIPC_CALL_SS_REQ_T_ECT_NUMBER, number);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestSetRttMode(const sp<RfxMclMessage>& msg) {
    int mode = ((int *)msg->getData()->getData())[0];
    logD(RFX_LOG_TAG, "MIPC_CALL_RTT_MODE_REQ AT> AT+EIMSRTT=%d", mode);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_RTT_MODE_REQ, msg->getSlotId());
    data->addMipcTlvUint8(MIPC_CALL_RTT_MODE_REQ_T_OP, mode);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestSetSipHeader(const sp<RfxMclMessage>& msg) {
    int minLen = 4;
    int length = msg->getData()->getDataLength();
    if (length < (minLen * sizeof(char *))) {
        logE(RFX_LOG_TAG, "Error: the length of params is %d", length);
        return RIL_E_INVALID_ARGUMENTS;
    }

    char** params = (char**)msg->getData()->getData();
    int total = atoi(params[0]);
    int index = atoi(params[1]);
    int headerCount = atoi(params[2]);
    char* headerValuePair = params[3];
    logD(RFX_LOG_TAG, "MIPC_CALL_SET_SIP_HEADER_REQ AT> AT+ESIPHEADER=%d,%d,%d",
            total, index, headerCount);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SET_SIP_HEADER_REQ,
            msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_SET_SIP_HEADER_REQ_T_TOTAL, total);
    data->addMipcTlvUint32(MIPC_CALL_SET_SIP_HEADER_REQ_T_INDEX, index);
    data->addMipcTlvUint32(MIPC_CALL_SET_SIP_HEADER_REQ_T_COUNT, headerCount);
    data->addMipcTlvStr(MIPC_CALL_SET_SIP_HEADER_REQ_T_VALUE_PAIR, headerValuePair);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestEnableSipHeaderReport(const sp<RfxMclMessage>& msg) {
    int minLen = 2;
    int length = msg->getData()->getDataLength();
    if (length < (minLen * sizeof(char *))) {
        logE(RFX_LOG_TAG, "Error: the length of params is %d", length);
        return RIL_E_INVALID_ARGUMENTS;
    }

    char** params = (char**)msg->getData()->getData();
    int callId = atoi(params[0]);
    int headerType = atoi(params[1]);
    logD(RFX_LOG_TAG, "MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ AT> AT+EIMSHEADER=%d,%d",
            callId, headerType);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ,
            msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ_T_CALL_ID, callId);
    data->addMipcTlvUint32(MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ_T_HEADER_TYPE, headerType);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestSetImsCallMode(const sp<RfxMclMessage>& msg) {
    int mode = ((int *)msg->getData()->getData())[0];
    logD(RFX_LOG_TAG, "MIPC_CALL_SET_IMS_CALL_MODE_REQ AT> AT+EIMSCALLMODE=%d", mode);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SET_IMS_CALL_MODE_REQ,
            msg->getSlotId());
    data->addMipcTlvUint8(MIPC_CALL_SET_IMS_CALL_MODE_REQ_T_OP, mode);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestSetImsCallAdditionalInfo(
        const sp<RfxMclMessage>& msg) {
    int minLen = 6;
    int length = msg->getData()->getDataLength();
    if (length < (minLen * sizeof(char *))) {
        logE(RFX_LOG_TAG, "Error: the length of params is %d", length);
        return RIL_E_INVALID_ARGUMENTS;
    }

    char** params = (char**)msg->getData()->getData();
    int mode = atoi(params[0]);
    int type = atoi(params[1]);
    int total = atoi(params[2]);
    int index = atoi(params[3]);
    int count = atoi(params[4]);
    char* additionalInfo = params[5];
    logD(RFX_LOG_TAG, "MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ AT> AT+EIMSCAI=%d,%d,%d,%d,%d",
            mode, type, total, index, count);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ,
            msg->getSlotId());
    data->addMipcTlvUint8(MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_MODE, mode);
    data->addMipcTlvUint8(MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_TYPE, type);
    data->addMipcTlvUint32(MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_TOTAL, total);
    data->addMipcTlvUint32(MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_INDEX, index);
    data->addMipcTlvUint32(MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_COUNT, count);
    data->addMipcTlvStr(MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_ADDITIONAL_INFO, additionalInfo);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestSetVoiceDomainPreference(const sp<RfxMclMessage>& msg) {
    int voiceDomain = ((int *)msg->getData()->getData())[0];
    logD(RFX_LOG_TAG, "MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ AT> AT+CEVDP=%d", voiceDomain);

    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ,
            msg->getSlotId());
    data->addMipcTlvUint32(MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ_T_SETTING, voiceDomain);
    return callToMipcMsgAsync(msg, data);
}

int RmmCallControlCommonRequestHandler::requestGetVoiceDomainPreference(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_REQ AT> AT+CEVDP?");
    sp<RfxMipcData> data = RfxMipcData::obtainMipcData(MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_REQ,
            msg->getSlotId());
    return callToMipcMsgAsync(msg, data);
}

void RmmCallControlCommonRequestHandler::handleGetVoiceDomainPreferenceCnf(const sp<RfxMclMessage>& msg) {
    int voiceDomain = 0;
    RfxMipcData* data = msg->getMipcData();
    if (data->getResult() == MIPC_RESULT_SUCCESS) {
        voiceDomain = data->getMipcUint32Val(MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF_T_SETTING, 0);
    }
    logD(RFX_LOG_TAG, "MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF AT< +CEVDP: %d", voiceDomain);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&voiceDomain, 1), msg);
    responseToTelCore(response);
}
/// IMS specific end

