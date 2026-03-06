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
#include <string>
#include "RmmCallControlUrcHandler.h"
#include "RfxCdmaInfoRecData.h"
#include "RfxCdmaWaitingCallData.h"
#include "RfxCrssNotificationData.h"
#include "RfxSuppServNotificationData.h"
#include "RfxNwServiceState.h"
#include "RfxMessageId.h"
#include "RfxImsConfData.h"
#include "RfxIntsData.h"
#include "RfxRilUtils.h"
#include "RfxRawData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"

#include "RfxGwsdUtils.h"

#include <telephony/mtk_ril.h>
//#include <ss/GsmUtil.h>

#define RFX_LOG_TAG "RmmCCUrcHandler"

#define DISPLAY_TAG_BLANK            0x80
#define DISPLAY_TAG_SKIP             0x81
#define MODE_ENRICHED_CALLING        3

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmCallControlUrcHandler, RIL_CMD_PROXY_URC);

int RmmCallControlUrcHandler::mSpeechCodec = 0;

RmmCallControlUrcHandler::RmmCallControlUrcHandler(int slot_id, int channel_id) :
        RmmCallControlBaseHandler(slot_id, channel_id) {
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        RfxGwsdUtils::getCallControlHandler()->registerForGwsdUrc(this);
    }

    const int mipcUrc[] = {
        MIPC_CALL_STATUS_IND,
        MIPC_CALL_EVENT_IND,
        MIPC_CALL_SS_IND,
        MIPC_CALL_CRSS_IND,
        MIPC_CALL_CIPHER_IND,
        MIPC_CALL_ECBM_CHANGE_IND,
        MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND,
        MIPC_CALL_EXTENDED_DISPLAY_INFO_IND,
        MIPC_CALL_LINE_CONTROL_INFO_IND,
        MIPC_CALL_REDIRECTING_NUMBER_INFO_IND,
        MIPC_SYS_VODATA_STATISTICS_IND,
        MIPC_CALL_ECONF_IND,
        MIPC_CALL_MODE_IND,
        MIPC_CALL_ECT_IND,
        MIPC_CALL_IMS_EVENT_PACKAGE_IND,
        MIPC_CALL_CONFERENCE_IND,
        MIPC_CALL_ECC_REDIAL_IND,
        MIPC_CALL_IMS_SIP_HEADER_IND,
        MIPC_CALL_CALL_ADDITIONAL_INFO_IND,
        MIPC_CALL_MT_SIP_INVITE_IND,
    };

    registerToHandleMipcURC(mipcUrc, sizeof(mipcUrc)/sizeof(int));

    const int mipcCmd[] = {
        MIPC_CALL_APPROVE_INCOMING_CMD
    };
    registerToHandleMipcCmd(mipcCmd, sizeof(mipcCmd) / sizeof(const int));
    for (int i = 0; i < MTSIPINVIT_NOTIFY_BUFFER_LEN; i++) {
        mMTSipTepData[i] = NULL;
    }
}

RmmCallControlUrcHandler::~RmmCallControlUrcHandler() {
}

void RmmCallControlUrcHandler::onHandleCmd(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        logD(RFX_LOG_TAG, "onHandleCmd MipcMsgId:0x%X, MipcSlotId:%d",
                msg->getMipcData()->getMipcMsgId(), msg->getMipcData()->getMipcSlotId());
        if (msg->getMipcData()->getMipcMsgId() == MIPC_CALL_APPROVE_INCOMING_CMD) {
            handleIncomingCallIndication(msg);
        }
    }
}
void RmmCallControlUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        switch (msg->getMipcData()->getMipcMsgId()) {
        case MIPC_CALL_STATUS_IND:
            handleCallProgressIndicationMessage(msg);
            break;
        case MIPC_CALL_EVENT_IND:
            handleCallEventIndicationMessage(msg);
            break;
        case MIPC_CALL_CRSS_IND:
            handleCrssNotification(msg);
            break;
        case MIPC_CALL_SS_IND:
            handleSuppSvcNotification(msg);
            break;
        case MIPC_CALL_CIPHER_IND:
            handleCipherIndicationMessage(msg);
            break;
        case MIPC_CALL_ECBM_CHANGE_IND:
            handleEmergencyCallbackModeMessage(msg);
            break;
        case MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND:
            handleDisplayAndSignalsInfoMessage(msg);
            break;
        case MIPC_CALL_EXTENDED_DISPLAY_INFO_IND:
            handleExtendedDisplayInfoMessage(msg);
            break;
        case MIPC_CALL_LINE_CONTROL_INFO_IND:
            handleLineControlInfoMessage(msg);
            break;
        case MIPC_CALL_REDIRECTING_NUMBER_INFO_IND:
            handleCdmaRedirectingNumberInfoMessage(msg);
            break;
        case MIPC_SYS_VODATA_STATISTICS_IND:
            handleMDInternetUsage(msg);
            break;
        case MIPC_CALL_ECONF_IND:
            handleConfModifiedResult(msg);
            break;
        case MIPC_CALL_MODE_IND:
            handleCallModeChanged(msg);
            break;
        case MIPC_CALL_ECT_IND:
            handleECT(msg);
            break;
        case MIPC_CALL_IMS_EVENT_PACKAGE_IND:
            handleImsEventPackage(msg);
            break;
        case MIPC_CALL_CONFERENCE_IND:
            handleImsConfInfoUpdate(msg);
            break;
        case MIPC_CALL_ECC_REDIAL_IND:
            handleRedialEmergencyIndication(msg);
            break;
        case MIPC_CALL_IMS_SIP_HEADER_IND:
            handleImsHeaderInfo(msg);
            break;
        case MIPC_CALL_CALL_ADDITIONAL_INFO_IND:
            handleCallAdditionalInfo(msg);
            break;
        case MIPC_CALL_MT_SIP_INVITE_IND:
            handleRCSMTSIPInfo(msg);
            break;
        default:
            // GWSD
            if (RfxGwsdUtils::getCallControlHandler() != NULL) {
                RfxGwsdUtils::getCallControlHandler()->handleGwsdUrc(this, msg, m_slot_id);
            }
            break;
        }
    }
}

void RmmCallControlUrcHandler::handleCallEventIndicationMessage(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    switch (mipcData->getMipcUint32Val(MIPC_CALL_EVENT_IND_T_EVENT, 0)) {
    case MIPC_CALL_EVENT_MT_REDIRECT:
        handleRedirectNumber(msg);
        break;
    case MIPC_CALL_EVENT_SRVCC:
        handleSrvccStateChange(msg);
        break;
    case MIPC_CALL_EVENT_AUDIO_CODEC_CHANGE:
        handleSpeechCodecInfo(msg);
        break;
    case MIPC_CALL_EVENT_SPEECH_ATTACH:
        handleEspeechMessage(msg);
        break;
    case MIPC_CALL_EVENT_CRING:
        handleRingMessage(msg);
        break;
    case MIPC_CALL_EVENT_RAT:
        handleCallRatChange(msg);
        break;
    case MIPC_CALL_EVENT_ECONFSRVCC:
        handleConfSrvcc(msg);
        break;
    }
}

void RmmCallControlUrcHandler::handleCallProgressIndicationMessage(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint32Val(MIPC_CALL_STATUS_IND_T_CALLID, 0);
    int dir = mipcData->getMipcUint32Val(MIPC_CALL_STATUS_IND_T_DIRECTION, 0);
    int callMode  = mipcData->getMipcUint32Val(MIPC_CALL_STATUS_IND_T_MODE, 0);
    int ton = mipcData->getMipcUint32Val(MIPC_CALL_STATUS_IND_T_TON, 0);
    int type = mipcData->getMipcUint32Val(MIPC_CALL_STATUS_IND_T_TYPE, 0);
    int msgType = mipcData->getMipcUint32Val(MIPC_CALL_STATUS_IND_T_MSG_TYPE, 0);
    int cause = mipcData->getMipcUint32Val(MIPC_CALL_STATUS_IND_T_DISC_CAUSE, 0);
    char *number = mipcData->getMipcStr(MIPC_CALL_STATUS_IND_T_NUMBER);
    char *pau = mipcData->getMipcStr(MIPC_CALL_STATUS_IND_T_PAU);

    int isIbt = 0;
    if ((type & MIPC_CALL_TYPE_IN_BAND_TONE_VIDEO_VOICE) == MIPC_CALL_TYPE_IN_BAND_TONE_VIDEO_VOICE) {
        isIbt = 3;
    } else if ((type & MIPC_CALL_TYPE_IN_BAND_TONE_VIDEO) == MIPC_CALL_TYPE_IN_BAND_TONE_VIDEO) {
        isIbt = 2;
    } else if ((type & MIPC_CALL_TYPE_IN_BAND_TONE) == MIPC_CALL_TYPE_IN_BAND_TONE) {
        isIbt = 1;
    }
    int isTch = (type & MIPC_CALL_TYPE_TCH) != 0 ? 1 : 0;

    logD(RFX_LOG_TAG, "MIPC_CALL_STATUS_IND id=%d msgType=%d isIbt=%d mode=%d number=%s type=0x%X cause=%d",
            callId, msgType, isIbt, callMode, RfxRilUtils::pii(RFX_LOG_TAG, number), type, cause);

    handleECPI(callId, msgType, isIbt);

    // notify CALL_INFO_INDICATION to TCL both for IMS or CS call, will be dispatched by TCL
    if (shoudNotifyCallInfo(msgType)) {
        //logD(RFX_LOG_TAG, "Send RFX_MSG_UNSOL_CALL_INFO_INDICATION for ECPI");
        // +ECPI:<call_id>, <msg_type>, <is_ibt>, <is_tch>, <dir>, <call_mode>, <number>, <type>, "<pau>", [<cause>]
        String8 callIdStr = String8::format("%d", callId);
        String8 msgTypeStr = String8::format("%d", msgType);
        String8 isIbtStr = String8::format("%d", isIbt);
        String8 isTchStr = String8::format("%d", isTch);
        String8 dirStr = String8::format("%d", dir);
        String8 callModeStr = String8::format("%d", callMode);
        String8 tonStr = String8::format("%d", ton);
        String8 causeStr = String8::format("%d", cause);
        const char *data[] = {
            callIdStr, msgTypeStr, isIbtStr, isTchStr,
            dirStr, callModeStr, number, tonStr, pau, causeStr
        };
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CALL_INFO_INDICATION,
                m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
        responseToTelCore(urc);
    }

    // Only notify for IMS call
    if (callMode == MIPC_CALL_MODE_IMS_VOICE_CALL
            || callMode == MIPC_CALL_MODE_IMS_VIDEO_CALL
            || callMode == MIPC_CALL_MODE_IMS_VOICE_CONFERENCE
            || callMode == MIPC_CALL_MODE_IMS_VIDEO_CONFERENCE
            || callMode == MIPC_CALL_MODE_IMS_VOICE_CONFERENCE_PARTICIPANT
            || callMode == MIPC_CALL_MODE_IMS_VIDEO_CONFERENCE_PARTICIPANT) {
        mipc_call_video_cap_struct4 *pVideoCap = (mipc_call_video_cap_struct4 *)mipcData->getMipcVal(
                MIPC_CALL_STATUS_IND_T_VIDEO_CAP, NULL);
        if (pVideoCap == NULL) {
            logE(RFX_LOG_TAG, "MIPC_CALL_STATUS_IND: pVideoCap is NULL");
            return;
        }

        logD(RFX_LOG_TAG, "MIPC_CALL_STATUS_IND AT< +EIMSVCAP: %d, %d, %d",
                callId, pVideoCap->local_video_cap, pVideoCap->remote_video_cap);
        String8 callIdStr = String8::format("%d", callId);
        String8 LocalCapStr = String8::format("%d", pVideoCap->local_video_cap);
        String8 RemoteCapStr = String8::format("%d", pVideoCap->remote_video_cap);
        const char *data[] = {callIdStr, LocalCapStr, RemoteCapStr};

        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_VIDEO_CAPABILITY_INDICATOR,
                m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
        responseToTelCore(urc);
    }
}

void RmmCallControlUrcHandler::sendRingbackToneNotification(int isStart) {
    int response[1] = { 0 };

    //logD(RFX_LOG_TAG, "Stop ringback tone.");
    mIsRingBackTonePlaying = isStart;
    response[0] = mIsRingBackTonePlaying;
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_RINGBACK_TONE,
            m_slot_id, RfxIntsData(response, 1));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleRingMessage(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    // Both ViLTE, VoLTE call need CRING, so do not skip CRING: VIDEO here
    sp < RfxMclMessage > urc_state = RfxMclMessage::obtainUrc(
            RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED, m_slot_id, RfxVoidData());
    responseToTelCore (urc_state);

    sp < RfxMclMessage > urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CALL_RING, m_slot_id,
            RfxVoidData());
    responseToTelCore (urc);
    //logD(RFX_LOG_TAG, "[handleRingMessage]Send RFX_MSG_UNSOL_CALL_RING");

    // To notify SS module that CRING is received
    sendEvent(RFX_MSG_EVENT_URC_CRING_NOTIFY, RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);

    mCallState = INCOMING;
}

void RmmCallControlUrcHandler::handleEspeechMessage(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int speechOn = mipcData->getMipcUint8Val(MIPC_CALL_EVENT_IND_T_SPEECH_ON, 0);

    sp<RfxMclMessage> urc =
            RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ESPEECH, m_slot_id, RfxIntsData(&speechOn, 1));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleCipherIndicationMessage(const sp<RfxMclMessage>& msg) {
    const int urcParamNum = 4;
    const char *data[urcParamNum];
    String8 buf[urcParamNum];
    const int params[] = {
        MIPC_CALL_CIPHER_IND_T_SIM_CIPHER_IND,
        MIPC_CALL_CIPHER_IND_T_MM_CONNECTION,
        MIPC_CALL_CIPHER_IND_T_CS_CIPHER_ON,
        MIPC_CALL_CIPHER_IND_T_PS_CIPHER_ON,
    };

    RfxMipcData* mipcData = msg->getMipcData();
    for (int i = 0; i < urcParamNum; i++) {
        buf[i] = String8::format("%d", mipcData->getMipcUint8Val(params[i], 0));
        data[i] = buf[i].string();
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CIPHER_INDICATION,
            m_slot_id, RfxStringsData(data, urcParamNum));
    responseToTelCore(urc);
    //logD(RFX_LOG_TAG, "Send RFX_MSG_UNSOL_CIPHER_INDICATION");
}

void RmmCallControlUrcHandler::handleSpeechCodecInfo(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int codec = mipcData->getMipcUint16Val(MIPC_CALL_EVENT_IND_T_AUDIO_CODEC, 0);

    if (mSpeechCodec != codec) {
        logD(RFX_LOG_TAG, "handleSpeechCodecInfo set mSpeechCodec: %d->%d",
                mSpeechCodec, codec);
        mSpeechCodec = codec;
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SPEECH_CODEC_INFO,
            m_slot_id, RfxIntsData(&codec, 1));
    responseToTelCore(urc);
    //logD(RFX_LOG_TAG, "Send RFX_MSG_UNSOL_SPEECH_CODEC_INFO type=%d", response[0]);
}

void RmmCallControlUrcHandler::handleCrssNotification(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    int type = data->getMipcUint8Val(MIPC_CALL_CRSS_IND_T_CRSS_TYPE, -1);
    char *number = data->getMipcStr(MIPC_CALL_CRSS_IND_T_NUMBER);
    int numberType = data->getMipcUint8Val(MIPC_CALL_CRSS_IND_T_CALL_NUMBER_TYPE, 0);
    char *name = data->getMipcStr(MIPC_CALL_CRSS_IND_T_ALPHAID);
    int numberPresentation = mipcNumPresentationToRilType(
            data->getMipcUint8Val(MIPC_CALL_CRSS_IND_T_NUMBER_PRESENTATION, 0));

    logD(RFX_LOG_TAG, "MIPC_CALL_CRSS_IND type=%d number=%s type=%d name=%s, presentation=%d",
            type, RfxRilUtils::pii(RFX_LOG_TAG, number), numberType,
            RfxRilUtils::pii(RFX_LOG_TAG, name), numberPresentation);

    int code = 0;
    switch (type) {
        case MIPC_CRSS_TYPE_CRSS_CALL_WAITING:
            code = CRSS_CALL_WAITING;
            break;
        case MIPC_CRSS_TYPE_CRSS_CALLING_LINE_ID_PRESET:
            code = CRSS_CALLING_LINE_ID_PREST;
            break;
        case MIPC_CRSS_TYPE_CRSS_CALLED_LINE_ID_PRESET:
            code = CRSS_CALLED_LINE_ID_PREST;
            break;
        case MIPC_CRSS_TYPE_CRSS_CONNECTED_LINE_ID_PRESET:
            code = CRSS_CONNECTED_LINE_ID_PREST;
            break;
        case MIPC_CRSS_TYPE_CDMA_CALL_WAITING:
            handleCdmaCallWaitingMessage(number, numberType);
            return;
        case MIPC_CRSS_TYPE_CDMA_CALLING_LINE_ID_PRESET:
            handleCdmaCallingPartyNumberInfoMessage(number, numberType, numberPresentation);
            return;
        default:
            return;
    }

    RIL_CrssNotification crssNotify = { code, numberType, number, name, numberPresentation };
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CRSS_NOTIFICATION, m_slot_id,
            RfxCrssNotificationData((void *)&crssNotify, sizeof(RIL_CrssNotification)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleSuppSvcNotification(const sp<RfxMclMessage>& msg) {
    /**
     * +CSSN=[<n>[,<m>]]
     * +CSSN?  +CSSN: <n>,<m>
     * +CSSN=? +CSSN: (list of supported <n>s),(list of supported <m>s)
     */
    /**
     * When <n>=1 and a supplementary service notification is received
     * after a mobile originated call setup, intermediate result code
     * +CSSI: <code1>[,<index>] is sent to TE before any other MO call
     * setup result codes presented in the present document or in V.25ter [14].
     * When several different <code1>s are received from the network,
     * each of them shall have its own +CSSI result code.
     * <code1> (it is manufacturer specific, which of these codes are supported):
     * 0   unconditional call forwarding is active
     * 1   some of the conditional call forwardings are active
     * 2   call has been forwarded
     * 3   call is waiting
     * 4   this is a CUG call (also <index> present)
     * 5   outgoing calls are barred
     * 6   incoming calls are barred
     * 7   CLIR suppression rejected
     * 8   call has been deflected
     * <index>: refer "Closed user group +CCUG"
     */
     /**
      * When <m>=1 and a supplementary service notification is received
      * during a mobile terminated call setup or during a call, or when
      * a forward check supplementary service notification is received,
      * unsolicited result code +CSSU: <code2>[,<index>[,<number>,<type>[,<subaddr>,<satype>]]]
      * is sent to TE. In case of MT call setup, result code is sent after every +CLIP result code
      * (refer command "Calling line identification presentation +CLIP")
      * and when several different <code2>s are received from the network,
      * each of them shall have its own +CSSU result code.
      * <code2> (it is manufacturer specific, which of these codes are supported):
      * 0   this is a forwarded call (MT call setup)
      * 1   this is a CUG call (also <index> present) (MT call setup)
      * 2   call has been put on hold (during a voice call)
      * 3   call has been retrieved (during a voice call)
      * 4   multiparty call entered (during a voice call)
      * 5   call on hold has been released (this is not a SS notification) (during a voice call)
      * 6   forward check SS message received (can be received whenever)
      * 7   call is being connected (alerting) with the remote party in alerting state in explicit call transfer operation (during a voice call)
      * 8   call has been connected with the other remote party in explicit call transfer operation (also number and subaddress parameters may be present) (during a voice call or MT call setup)
      * 9   this is a deflected call (MT call setup)
      * 10  additional incoming call forwarded
      * 11  MT is a forwarded call (CF)
      * 12  MT is a forwarded call (CFU)
      * 13  MT is a forwarded call (CFC)
      * 14  MT is a forwarded call (CFB)
      * 15  MT is a forwarded call (CFNRy)
      * 16  MT is a forwarded call (CFNRc)
      * <number>: string type phone number of format specified by <type>
      * <type>: type of address octet in integer format (refer GSM 04.08 [8] subclause 10.5.4.7)
      */
    RIL_SuppSvcNotification svcNotify;
    memset(&svcNotify, 0, sizeof(RIL_SuppSvcNotification));

    RfxMipcData* data = msg->getMipcData();
    int code1 = data->getMipcUint32Val(MIPC_CALL_SS_IND_T_CODE1, -1);
    int code2 = data->getMipcUint32Val(MIPC_CALL_SS_IND_T_CODE2, -1);
    svcNotify.index = data->getMipcUint32Val(MIPC_CALL_SS_IND_T_INDEX, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_SS_IND code1=%d code2=%d", code1, code2);
    if (code1 >= 0 && code1 <= 8) {
        svcNotify.notificationType = SUPP_SVC_CSSI;
        svcNotify.code = code1;
    } else if (code2 >= 0 && code2 <= 16) {
        svcNotify.notificationType = SUPP_SVC_CSSU;
        svcNotify.code = code2;
    } else if (code1 == MIPC_CALL_SS_CODE1_CMOS) {
        svcNotify.notificationType = 0;
        svcNotify.code = code1;
    } else {
        // Should not happen
        return;
    }

    if (hasImsCall(m_slot_id)) {
        // ril_service.cpp will dispatch code larger than 100 to IMS
        svcNotify.code += 100;
    }

    if (svcNotify.notificationType == SUPP_SVC_CSSU) {
        svcNotify.number = data->getMipcStr(MIPC_CALL_SS_IND_T_NUMBER);
        svcNotify.type = data->getMipcUint32Val(MIPC_CALL_SS_IND_T_TOA, 0);

        // M: CC: to adapt to AOSP framework
        if (svcNotify.code == 0 || (svcNotify.code >= 11 && svcNotify.code <= 16)) {
            logD(RFX_LOG_TAG, "convert proprietary CSSU <code2> for MT forwarded call to 0");
            sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
                    RFX_MSG_UNSOL_SUPP_SVC_NOTIFICATION_EX, m_slot_id,
                    RfxSuppServNotificationData((void *)&svcNotify, sizeof(RIL_SuppSvcNotification)));
            responseToTelCore(urc);
            svcNotify.code = 0;
        }
    }

    if(code1 == MIPC_CALL_SS_CODE1_CMOS) {
        svcNotify.number = data->getMipcStr(MIPC_CALL_SS_IND_T_RAW_STRING);
        if (svcNotify.number != NULL && strlen(svcNotify.number) > 0) {
            logD(RFX_LOG_TAG, "CMOS content: %s", svcNotify.number);
        } else {
            logD(RFX_LOG_TAG, "Invalid CMOS content");
            return;
        }
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
            RFX_MSG_UNSOL_SUPP_SVC_NOTIFICATION, m_slot_id,
            RfxSuppServNotificationData((void *)&svcNotify, sizeof(RIL_SuppSvcNotification)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleRedirectNumber(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    char *number = data->getMipcStr(MIPC_CALL_EVENT_IND_T_REDIRECT_NUMBER);
    if (number != NULL && strlen(number) > 0) {
        sendEvent(RFX_MSG_EVENT_FORWARDED_NUMBER, RfxStringData(number, strlen(number)),
                RIL_CMD_PROXY_2, m_slot_id);
    }
}

void RmmCallControlUrcHandler::handleNoCarrierMessage() {
    // We handle NO CARRIER URC because in some cases it indicates CDMA call disconnected.
    // No need to handle it when no call exists, to avoid unnecessary call polling.
    if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_IN_CALL)) {
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_NO_CARRIER,
                m_slot_id, RfxVoidData());
        responseToTelCore(urc);
    }
}

// +CLIP:<number>,<type>,,,,<cli_validity>
void RmmCallControlUrcHandler::handleCdmaCallingPartyNumberInfoMessage(
        char *number, int numberType, int numberPresentation) {
    RIL_CDMA_InformationRecords *cdmaInfo = (RIL_CDMA_InformationRecords *) alloca(
            sizeof(RIL_CDMA_InformationRecords));
    RFX_ASSERT(cdmaInfo != NULL);
    memset(cdmaInfo, 0, sizeof(RIL_CDMA_InformationRecords));

    cdmaInfo->infoRec[0].name = RIL_CDMA_CALLING_PARTY_NUMBER_INFO_REC;
    if (number != NULL && strlen(number) > 0 && strlen(number) < CDMA_NUMBER_INFO_BUFFER_LENGTH_MTK) {
        logD(RFX_LOG_TAG, "%s, number != NULL", __FUNCTION__);
        cdmaInfo->infoRec[0].rec.number.len = strlen(number);
        strncpy(cdmaInfo->infoRec[0].rec.number.buf, number,
                cdmaInfo->infoRec[0].rec.number.len + 1);
    } else {
        logD(RFX_LOG_TAG, "%s, number is NULL or invalid", __FUNCTION__);
        cdmaInfo->infoRec[0].rec.number.len = 0;
        strncpy(cdmaInfo->infoRec[0].rec.number.buf, "", 1);
    }

    cdmaInfo->infoRec[0].rec.number.number_type = numberType;
    cdmaInfo->infoRec[0].rec.number.number_plan = 1;
    cdmaInfo->infoRec[0].rec.number.pi = numberPresentation;
    cdmaInfo->infoRec[0].rec.number.si = 0;  // User provided, not screened

    logD(RFX_LOG_TAG, "RIL_CDMA_CALLING_PARTY_NUMBER_INFO_REC: len:%d, \
            number_type:%d, number_plan:%d, pi:%d, si:%d",
            cdmaInfo->infoRec[0].rec.number.len,
            cdmaInfo->infoRec[0].rec.number.number_type,
            cdmaInfo->infoRec[0].rec.number.number_plan,
            cdmaInfo->infoRec[0].rec.number.pi,
            cdmaInfo->infoRec[0].rec.number.si);

    cdmaInfo->numberOfInfoRecs = 1;
    logD(RFX_LOG_TAG, "RIL_UNSOL_CDMA_INFO_REC: numOfRecs:1");
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CDMA_INFO_REC, m_slot_id,
            RfxCdmaInfoRecData((void *)cdmaInfo, sizeof(RIL_CDMA_InformationRecords)));
    responseToTelCore(urc);
}

// +REDIRNUM:<ext_bit_1>,<number_type>,<number_plan>,<ext_bit_2>,<pi>,<si>,
//           <ext_bit_3>,<redirection_reason>,<number>
void RmmCallControlUrcHandler::handleCdmaRedirectingNumberInfoMessage(const sp<RfxMclMessage>& msg) {
    RIL_CDMA_InformationRecords *cdmaInfo = (RIL_CDMA_InformationRecords *) alloca(
            sizeof(RIL_CDMA_InformationRecords));
    RFX_ASSERT(cdmaInfo != NULL);

    RfxMipcData* data = msg->getMipcData();
    char *number = data->getMipcStr(MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER);
    int numberType = data->getMipcUint32Val(MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER_TYPE, 0);
    int numberPlan = data->getMipcUint32Val(MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER_PLAN, 0);
    int redirReason
            = data->getMipcUint32Val(MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_REDIRECTION_REASON, 0);
    int pi = data->getMipcUint32Val(MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_PI, 0);
    int si = data->getMipcUint32Val(MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_SI, 0);

    cdmaInfo->infoRec[0].name = RIL_CDMA_REDIRECTING_NUMBER_INFO_REC;
    if (number != NULL && strlen(number) > 0 && strlen(number) < CDMA_NUMBER_INFO_BUFFER_LENGTH_MTK) {
        logD(RFX_LOG_TAG, "%s, number != NULL", __FUNCTION__);
        cdmaInfo->infoRec[0].rec.redir.redirectingNumber.len = strlen(number);
        strncpy(cdmaInfo->infoRec[0].rec.redir.redirectingNumber.buf, number,
                cdmaInfo->infoRec[0].rec.redir.redirectingNumber.len + 1);
    } else {
        logD(RFX_LOG_TAG, "%s, number is NULL or invalid", __FUNCTION__);
        cdmaInfo->infoRec[0].rec.redir.redirectingNumber.len = 0;
        strncpy(cdmaInfo->infoRec[0].rec.redir.redirectingNumber.buf, "", 1);
    }
    cdmaInfo->infoRec[0].rec.redir.redirectingNumber.number_type = (char)numberType;
    cdmaInfo->infoRec[0].rec.redir.redirectingNumber.number_plan = (char)numberPlan;
    cdmaInfo->infoRec[0].rec.redir.redirectingNumber.pi = (char)pi;
    cdmaInfo->infoRec[0].rec.redir.redirectingNumber.si = (char)si;
    cdmaInfo->infoRec[0].rec.redir.redirectingReason =
            (RIL_CDMA_RedirectingReason)redirReason;

    logD(RFX_LOG_TAG, "RIL_CDMA_REDIRECTING_NUMBER_INFO_REC: len:%d, number_type:%d, \
            number_plan:%d, pi:%d, si:%d, redirectingReason:%d",
            cdmaInfo->infoRec[0].rec.redir.redirectingNumber.len,
            cdmaInfo->infoRec[0].rec.redir.redirectingNumber.number_type,
            cdmaInfo->infoRec[0].rec.redir.redirectingNumber.number_plan,
            cdmaInfo->infoRec[0].rec.redir.redirectingNumber.pi,
            cdmaInfo->infoRec[0].rec.redir.redirectingNumber.si,
            cdmaInfo->infoRec[0].rec.redir.redirectingReason);

    cdmaInfo->numberOfInfoRecs = 1;
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CDMA_INFO_REC, m_slot_id,
            RfxCdmaInfoRecData((void *)cdmaInfo, sizeof(RIL_CDMA_InformationRecords)));
    responseToTelCore(urc);
}

// +LINECON:<polarity_included>,<toggle_mode>,<reverse_polarity>,<power_denial_time>
void RmmCallControlUrcHandler::handleLineControlInfoMessage(const sp<RfxMclMessage>& msg) {
    RIL_CDMA_InformationRecords *cdmaInfo = (RIL_CDMA_InformationRecords *) alloca(
            sizeof(RIL_CDMA_InformationRecords));
    RFX_ASSERT(cdmaInfo != NULL);

    RfxMipcData* data = msg->getMipcData();
    int polarityIncluded
            = data->getMipcUint32Val(MIPC_CALL_LINE_CONTROL_INFO_IND_T_POLARITY_INCLUDED, 0);
    int toggleMode
            = data->getMipcUint32Val(MIPC_CALL_LINE_CONTROL_INFO_IND_T_TOGGLE_MODE, 0);
    int reversePolarity
            = data->getMipcUint32Val(MIPC_CALL_LINE_CONTROL_INFO_IND_T_REVERSE_POLARITY, 0);
    int powerDenialTime
            = data->getMipcUint32Val(MIPC_CALL_LINE_CONTROL_INFO_IND_T_POWER_DENIAL_TIME, 0);

    cdmaInfo->infoRec[0].name = RIL_CDMA_LINE_CONTROL_INFO_REC;
    cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlPolarityIncluded = (char)polarityIncluded;
    cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlToggle = (char)toggleMode;
    cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlReverse = (char)reversePolarity;
    cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlPowerDenial = (char)powerDenialTime;

    logD(RFX_LOG_TAG, "RIL_CDMA_LINE_CONTROL_INFO_REC: lineCtrlPolarityIncluded:%d, \
            lineCtrlToggle:%d, lineCtrlReverse:%d, lineCtrlPowerDenial:%d",
            cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlPolarityIncluded,
            cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlToggle,
            cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlReverse,
            cdmaInfo->infoRec[0].rec.lineCtrl.lineCtrlPowerDenial);

    cdmaInfo->numberOfInfoRecs = 1;
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CDMA_INFO_REC, m_slot_id,
            RfxCdmaInfoRecData((void *)cdmaInfo, sizeof(RIL_CDMA_InformationRecords)));
    responseToTelCore(urc);
}

// +CEXTD:<display_tag>,<info>
void RmmCallControlUrcHandler::handleExtendedDisplayInfoMessage(const sp<RfxMclMessage>& msg) {
    RIL_CDMA_InformationRecords *cdmaInfo = (RIL_CDMA_InformationRecords *) alloca(
            sizeof(RIL_CDMA_InformationRecords));
    RFX_ASSERT(cdmaInfo != NULL);

    RfxMipcData* data = msg->getMipcData();
    int displayTag = data->getMipcUint32Val(MIPC_CALL_EXTENDED_DISPLAY_INFO_IND_T_DISPLAY_TAG, 0);
    char *info = data->getMipcStr(MIPC_CALL_EXTENDED_DISPLAY_INFO_IND_T_INFO);

    logD(RFX_LOG_TAG, "MIPC_CALL_EXTENDED_DISPLAY_INFO_IND: %d %s",
            displayTag, RfxRilUtils::pii(RFX_LOG_TAG, info));

    if (info != NULL && strlen(info) > 0 && strlen(info) < CDMA_ALPHA_INFO_BUFFER_LENGTH_MTK
            && strcmp(info, "N/A") != 0
            && displayTag != DISPLAY_TAG_BLANK && displayTag != DISPLAY_TAG_SKIP) {
        logD(RFX_LOG_TAG, "callState=%d, displaytag=%d", mCallState, displayTag);
        if ((displayTag == DISPLAY_TAG_CALLING_PARTY_NAME
                || displayTag == DISPLAY_TAG_ORIG_CALLED_NAME) && mCallState == INCOMING) {
            sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
                    RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED, m_slot_id, RfxVoidData());
            responseToTelCore(urc);
            logD(RFX_LOG_TAG, "skip RIL_CDMA_EXTENDED_DISPLAY_INFO_REC for incoming call");
        } else {
            cdmaInfo->infoRec[0].name = RIL_CDMA_EXTENDED_DISPLAY_INFO_REC;
            cdmaInfo->infoRec[0].rec.display.alpha_len = strlen(info);
            strncpy(cdmaInfo->infoRec[0].rec.display.alpha_buf, info,
                    cdmaInfo->infoRec[0].rec.display.alpha_len + 1);

            logD(RFX_LOG_TAG, "RIL_CDMA_EXTENDED_DISPLAY_INFO_REC: alpha_len:%d",
                    cdmaInfo->infoRec[0].rec.display.alpha_len);

            cdmaInfo->numberOfInfoRecs = 1;
            sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CDMA_INFO_REC, m_slot_id,
                    RfxCdmaInfoRecData((void *)cdmaInfo, sizeof(RIL_CDMA_InformationRecords)));
            responseToTelCore(urc);
        }
    }
}

// +CFNM:<display>,<signal_type>,<alert_pitch>,<signal>
void RmmCallControlUrcHandler::handleDisplayAndSignalsInfoMessage(const sp<RfxMclMessage>& msg) {
    RIL_CDMA_InformationRecords *cdmaInfo = (RIL_CDMA_InformationRecords *) alloca(
            sizeof(RIL_CDMA_InformationRecords));
    RFX_ASSERT(cdmaInfo != NULL);
    int numOfRecs = 0;

    RfxMipcData* data = msg->getMipcData();
    char *display = data->getMipcStr(MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_DISPLAY);
    int signalType
            = data->getMipcUint32Val(MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_SIGNAL_TYPE, -1);
    int alertPitch
            = data->getMipcUint32Val(MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_ALERT_PITCH, -1);
    int signal = data->getMipcUint32Val(MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_SIGNAL, -1);
    mCachedSignal = signal;

    logD(RFX_LOG_TAG, "MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND: %s, %d, %d, %d",
            RfxRilUtils::pii(RFX_LOG_TAG, display), signalType, alertPitch, signal);

    if (display != NULL && strlen(display) > 0 && strlen(display) < CDMA_ALPHA_INFO_BUFFER_LENGTH_MTK
            && strcmp(display, "N/A") != 0) {
        if (mCallState == INCOMING) {
            sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
                    RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED, m_slot_id, RfxVoidData());
            responseToTelCore(urc);
            logD(RFX_LOG_TAG, "skip RIL_CDMA_DISPLAY_INFO_REC for there's incoming call");
            return;
        } else {
            cdmaInfo->infoRec[numOfRecs].name = RIL_CDMA_DISPLAY_INFO_REC;
            cdmaInfo->infoRec[numOfRecs].rec.display.alpha_len = strlen(display);
            strncpy(cdmaInfo->infoRec[numOfRecs].rec.display.alpha_buf, display,
                    cdmaInfo->infoRec[numOfRecs].rec.display.alpha_len + 1);

            logD(RFX_LOG_TAG, "RIL_CDMA_DISPLAY_INFO_REC: alpha_len:%d",
                    cdmaInfo->infoRec[numOfRecs].rec.display.alpha_len);
            numOfRecs++;
        }
    }

    // SIGNAL_TYPE/ALERT_PITCH/SIGNAL are optional TLV
    if (signalType != -1 && alertPitch != -1 && signal != -1
            && (mCallState == DIALING || mCallState == IDLE || mCallState == CONNECTED)) {
        cdmaInfo->infoRec[numOfRecs].name = RIL_CDMA_SIGNAL_INFO_REC;
        cdmaInfo->infoRec[numOfRecs].rec.signal.isPresent = 1;
        cdmaInfo->infoRec[numOfRecs].rec.signal.signalType = (char)signalType;
        cdmaInfo->infoRec[numOfRecs].rec.signal.alertPitch = (char)alertPitch;
        cdmaInfo->infoRec[numOfRecs].rec.signal.signal = (char)signal;

        logD(RFX_LOG_TAG, "RIL_CDMA_SIGNAL_INFO_REC: signalType:%d, alertPitch:%d, signal:%d",
                cdmaInfo->infoRec[numOfRecs].rec.signal.signalType,
                cdmaInfo->infoRec[numOfRecs].rec.signal.alertPitch,
                cdmaInfo->infoRec[numOfRecs].rec.signal.signal);
        numOfRecs++;
    }
    if (numOfRecs > 0) {
        cdmaInfo->numberOfInfoRecs = numOfRecs;
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CDMA_INFO_REC, m_slot_id,
                RfxCdmaInfoRecData((void *)cdmaInfo, sizeof(RIL_CDMA_InformationRecords)));
        responseToTelCore(urc);
    }
}

// +CIEV: <type>,<value>
void RmmCallControlUrcHandler::handleEmergencyCallbackModeMessage(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    int value = data->getMipcUint8Val(MIPC_CALL_ECBM_CHANGE_IND_T_MODE, -1);

    logD(RFX_LOG_TAG, "MIPC_CALL_ECBM_CHANGE_IND_T_MODE: %d", value);

    int urcType = 0;
    if (value == MIPC_CALL_ECBM_MODE_ECBM_ON) {
        urcType = RFX_MSG_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE;
    } else if (value == MIPC_CALL_ECBM_MODE_ECBM_OFF) {
        urcType = RFX_MSG_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE;
    } else if (value == MIPC_CALL_ECBM_MODE_NO_ECBM) {
        urcType = RFX_MSG_UNSOL_NO_EMERGENCY_CALLBACK_MODE;
    } else {
        return;
    }
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(urcType, m_slot_id, RfxVoidData());
    responseToTelCore(urc);
}

// +CCWA: <number>, <callType>
void RmmCallControlUrcHandler::handleCdmaCallWaitingMessage(char *number, int numberType) {
    if (mCallState == WAITING && mCachedSignal == SIGNAL_TONES_OFF
            && mCachedWaitingCallNumber == number) {
        logI(RFX_LOG_TAG, "Skip duplicate CCWA w/ signal tones off");
        mCachedSignal = 0;
        mCachedWaitingCallNumber.clear();
        return;
    }
    mCachedWaitingCallNumber = String8(number);

    RIL_CDMA_CallWaiting_v6 *callWaiting =
            (RIL_CDMA_CallWaiting_v6 *) alloca(sizeof(RIL_CDMA_CallWaiting_v6));
    RFX_ASSERT(callWaiting != NULL);
    memset(callWaiting, 0, sizeof(RIL_CDMA_CallWaiting_v6));

    callWaiting->number = number;
    callWaiting->number_type = numberType;
    if (!strcmp(callWaiting->number, "Restricted")) {
        callWaiting->numberPresentation = PRESENTATION_RESTRICTED;
    } else if (!strcmp(callWaiting->number, "UNKNOWN")
            || !strcmp(callWaiting->number, "Unknown")
            || !strcmp(callWaiting->number, "Unavailable")
            || !strcmp(callWaiting->number, "NotAvailable")
            || !strcmp(callWaiting->number, "LOOPBACK CALL")) {
        callWaiting->numberPresentation = PRESENTATION_UNKNOWN;
    }
    if (callWaiting->numberPresentation != PRESENTATION_ALLOWED) {
        callWaiting->number = NULL;
    }

    mCallState = WAITING;

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CDMA_CALL_WAITING, m_slot_id,
            RfxCdmaWaitingCallData((void *)callWaiting, sizeof(RIL_CDMA_CallWaiting_v6)));
    responseToTelCore(urc);
}
/// C2K specific end

void RmmCallControlUrcHandler::handleECPI(int callId, int msgType, int isIbt) {
    // +ESPEECH is not used for RINGBACK_TONE notification in Gen93 RIL
    // RINGBACK_TONE is set to outband (played by APP) if +ECPI: <msgType=2>, <isIbt=0>
    // RINGBACK_TONE is reset to inband (played by network) if the following cases happen:
    //   1) +ECPI: <msgType=2>, <isIbt=1> : ALERT
    //   2) +ECPI: <msgType=4>, <isIbt=1> : SYNC
    //   3) +ECPI: <msgType=5>, <isIbt=1> : PROGRESS
    //   4) +ECPI: <msgType=6>            : ACTIVE
    //   5) +ECPI: <msgType=133>          : DISCONNECTED
    switch (msgType) {
        case 2:
            if (isIbt == 1) {
                if (mIsRingBackTonePlaying) {
                    sendRingbackToneNotification(STOP_RING_BACK_TONE);
                }
            } else if (isIbt == 0) {
                sendRingbackToneNotification(START_RING_BACK_TONE);
            }
            break;
        case 4:
        case 5:
            if (isIbt == 1) {
                if (mIsRingBackTonePlaying) {
                    sendRingbackToneNotification(STOP_RING_BACK_TONE);
                }
            }
            break;
        case 6:
            if (mIsRingBackTonePlaying) {
                sendRingbackToneNotification(STOP_RING_BACK_TONE);
            }
            mCallState = CONNECTED;
            break;
        case 130:
            mCallState = DIALING;
            break;
        case 133:
            sendEvent(RFX_MSG_EVENT_CLEAR_FORWARDED_NUMBER, RfxIntsData(&callId, 1),
                    RIL_CMD_PROXY_2, m_slot_id);
            if (mIsRingBackTonePlaying) {
                sendRingbackToneNotification(STOP_RING_BACK_TONE);
            }
            mCallState = IDLE;
            mCachedSignal = 0;
            mCachedWaitingCallNumber.clear();
            break;
        default:
            break;
    }
}

bool RmmCallControlUrcHandler::shoudNotifyCallInfo(int msgType) {
    int supportedMsg[11] = {0, 2, 6, 7, 130, 131, 132, 133, 135, 136, 137};
    for (int i = 0; i < 11; ++i) {
        if (supportedMsg[i] == msgType) {
            return true;
        }
    }
    return false;
}

bool RmmCallControlUrcHandler::shouldNotifyCallStateChanged(int msgType) {
    int supportedMsg[8] = {0, 2, 6, 130, 131, 132, 133, 254};
    for (int i = 0; i < 8; ++i) {
        if (supportedMsg[i] == msgType) {
            return true;
        }
    }
    return false;
}

int RmmCallControlUrcHandler::getSpeechCodec() {
    return mSpeechCodec;
}

void RmmCallControlUrcHandler::resetSpeechCodec() {
    mSpeechCodec = 0;
}

/// IMS specific start
void RmmCallControlUrcHandler::handleSrvccStateChange(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int state = mipcData->getMipcUint32Val(MIPC_CALL_EVENT_IND_T_SRVCCH, -1);
    logD(RFX_LOG_TAG, "MIPC_CALL_EVENT_SRVCC AT< +CIREPH: %d", state);
    if (state >= SRVCC_STARTED && state <= SRVCC_CANCELED) {
        notifySrvccState(state);
    }
}

void RmmCallControlUrcHandler::notifySrvccState(int state) {
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SRVCC_STATE_NOTIFY,
            m_slot_id, RfxIntsData(&state, 1));
    responseToTelCore(urc);

    // Trigger GsmFwk poll call after SRVCC
    if (state == SRVCC_COMPLETED) {
        sp<RfxMclMessage> callStateChangeUrc = RfxMclMessage::obtainUrc(
            RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED, m_slot_id, RfxVoidData());
        responseToTelCore(callStateChangeUrc);
    }
}

void RmmCallControlUrcHandler::handleCallRatChange(const sp<RfxMclMessage>& msg) {
    if (!hasImsCall(msg->getSlotId())) {
        logD(RFX_LOG_TAG, "No ImsCall, ignore MIPC_CALL_EVENT_RAT");
        return;
    }

    RfxMipcData* mipcData = msg->getMipcData();
    int rat = mipcData->getMipcUint8Val(MIPC_CALL_EVENT_IND_T_RAT, MIPC_CALL_RAT_UNKNOWN);
    if (rat == MIPC_CALL_RAT_UNKNOWN) {
        return;
    }

    int voiceDomain = rat == MIPC_CALL_RAT_CS ? 0 : 1;
    int callRat = -1;
    if (rat == MIPC_CALL_RAT_VoLTE) {
        callRat = VDM_ADS_RESULT_IMS_VOLTE;
    } else if (rat == MIPC_CALL_RAT_WFC) {
        callRat = VDM_ADS_RESULT_IMS_WIFI;
    } else if (rat == MIPC_CALL_RAT_VoNR) {
        callRat = VDM_ADS_RESULT_IMS_NR;
    }
    logD(RFX_LOG_TAG, "MIPC_CALL_EVENT_RAT AT< +EVADSREP: %d, %d", voiceDomain, callRat);

    int data[] = {voiceDomain, callRat};
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CALL_RAT_INDICATION,
            m_slot_id, RfxIntsData(data, sizeof(data) / sizeof(int)));
    responseToTelCore(urc);

    if (rat == MIPC_CALL_RAT_CS) {
        notifySrvccState(SRVCC_STARTED);
        notifySrvccState(SRVCC_COMPLETED);
    }
}

void RmmCallControlUrcHandler::handleConfSrvcc(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int count = mipcData->getMipcUint32Val(MIPC_CALL_EVENT_IND_T_COUNT, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_EVENT_ECONFSRVCC AT< +ECONFSRVCC: %d", count);
    if (count < 1) return;

    const int maxDataLen = 6;
    const int maxMembers = 5;  // not inlcude the host
    int data[maxDataLen] = {0};
    data[0] = count;
    if (data[0] > maxMembers) {
        logE(RFX_LOG_TAG, "The count of calls to SRVCC is greater than limitation");
        return;
    }
    for (int i = 0; i < count; ++i) {
        data[i + 1] = mipcData->getMipcUint32IdxVal(MIPC_CALL_EVENT_IND_T_CALL_ID, i, 0);
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
        RFX_MSG_UNSOL_ECONF_SRVCC_INDICATION, m_slot_id, RfxIntsData(data, maxDataLen));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleConfModifiedResult(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int confId = mipcData->getMipcUint32Val(MIPC_CALL_ECONF_IND_T_CONF_CALL_ID, 0);
    int operation = mipcData->getMipcUint32Val(MIPC_CALL_ECONF_IND_T_OPERATION, 0);
    char *number = mipcData->getMipcStr(MIPC_CALL_ECONF_IND_T_NUMBER);
    int result = mipcData->getMipcUint32Val(MIPC_CALL_ECONF_IND_T_RESULT, 0);
    int cause = mipcData->getMipcUint32Val(MIPC_CALL_ECONF_IND_T_CAUSE, 0);
    int joinedCallId = mipcData->getMipcUint32Val(MIPC_CALL_ECONF_IND_T_JOINED_CALL_ID, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_ECONF_IND AT< +ECONF: %d, %d, %s, %d, %d, %d",
            confId, operation, RfxRilUtils::pii(RFX_LOG_TAG, number), result, cause, joinedCallId);

    String8 confIdStr = String8::format("%d", confId);
    String8 operationStr = String8::format("%d", operation);
    String8 resultStr = String8::format("%d", result);
    String8 causeStr = String8::format("%d", cause);
    String8 joinedCallIdStr = String8::format("%d", joinedCallId);
    const char *data[] = {confIdStr, operationStr, number, resultStr, causeStr, joinedCallIdStr};

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ECONF_RESULT_INDICATION,
            m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleImsEventPackage(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint32Val(MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_CALLID, 0);
    int type = mipcData->getMipcUint32Val(MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_TYPE, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_IMS_EVENT_PACKAGE_IND AT< +EIMSEVTPKG: %d, %d", callId, type);
    // Conference event package is handled by handleImsConfInfoUpdate
    if (type == 1) {
        return;
    }
    char *pkgInfo = mipcData->getMipcStr(MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_DATA);
    int urcIndex = 1;
    int totalUrcCount = 1;

    String8 callIdStr = String8::format("%d", callId);
    String8 typeStr = String8::format("%d", type);
    String8 urcIndexStr = String8::format("%d", urcIndex);
    String8 totalUrcCountStr = String8::format("%d", totalUrcCount);
    const char *data[] = {callIdStr, typeStr, urcIndexStr, totalUrcCountStr, pkgInfo};

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_IMS_EVENT_PACKAGE_INDICATION,
            m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleImsConfInfoUpdate(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint32Val(MIPC_CALL_CONFERENCE_IND_T_CONF_CALLID, 0);
    int count = mipcData->getMipcUint32Val(MIPC_CALL_CONFERENCE_IND_T_COUNT, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_CONFERENCE_IND callId %d participantCount %d", callId, count);
    if (count == 0) {
        return;
    }

    RIL_Conference_Participants *p_participants
            = (RIL_Conference_Participants *)alloca(count * sizeof(RIL_Conference_Participants));
    RFX_ASSERT(p_participants != NULL);
    memset(p_participants, 0, count * sizeof(RIL_Conference_Participants));

    Vector<String8> statusBuf;
    for (int i = 0; i < count && i < MIPC_MAX_CALL_CONFERENCE_PARTICIPANT_NUM; i++) {
        char *userAddr = (char *)mipcData->getMipcIdxVal(
                MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_NUMBER, i, NULL);
        char *displayName = (char *)mipcData->getMipcIdxVal(
                MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_NAME, i, NULL);
        char *userEntity = (char *)mipcData->getMipcIdxVal(
                MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_USER_ENTITY, i, NULL);
        char *endpoint = (char *)mipcData->getMipcIdxVal(
                MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_ENDPOINT_ENTITY, i, NULL);
        int status = mipcData->getMipcUint32IdxVal(
                MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_STATUS, i, 0);
        switch (status) {
            case MIPC_CONF_PARTICIPANT_STATUS_CONNECTED:
                statusBuf.add(String8("connected"));
                break;
            case MIPC_CONF_PARTICIPANT_STATUS_ON_HOLD:
                statusBuf.add(String8("on-hold"));
                break;
            case MIPC_CONF_PARTICIPANT_STATUS_DISCONNECTED:
                statusBuf.add(String8("disconnected"));
                break;
            default:
                statusBuf.add(String8("connected"));
                break;
        }

        logD(RFX_LOG_TAG, "MIPC_CALL_CONFERENCE_IND userAddr=%s status=%s",
                RfxRilUtils::pii(RFX_LOG_TAG, userAddr), statusBuf[i].string());

        p_participants[i].useraddr = userAddr;
        p_participants[i].display_text = displayName;
        p_participants[i].status = (char *)statusBuf[i].string();
        p_participants[i].entity = userEntity;
        p_participants[i].end_point = endpoint;
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_IMS_CONFERENCE_INFO_INDICATION,
            m_slot_id, RfxImsConfData(p_participants, sizeof(RIL_Conference_Participants) * count));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleCallModeChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint32Val(MIPC_CALL_MODE_IND_T_CALLID, 0);
    int callMode = mipcData->getMipcUint32Val(MIPC_CALL_MODE_IND_T_MODE, 0);
    int cameraDir = mipcData->getMipcUint32Val(MIPC_CALL_MODE_IND_T_SDP_CAMERA_DIRECTION, 0);
    int audioDir = mipcData->getMipcUint32Val(MIPC_CALL_MODE_IND_T_SDP_AUDIO_DIRECTION, 0);
    int audioCodec = mipcData->getMipcUint32Val(MIPC_CALL_MODE_IND_T_SDP_AUDIO_CODEC, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_MODE_IND AT< +EIMSCMODE: %d, %d", callId, callMode);

    String8 callIdStr = String8::format("%d", callId);
    String8 callModeStr = String8::format("%d", callMode);
    String8 cameraDirStr = String8::format("%d", cameraDir);
    String8 audioDirStr = String8::format("%d", audioDir);
    String8 audioCodecStr = String8::format("%d", audioCodec);
    const char *data[] = {callIdStr, callModeStr, cameraDirStr, audioDirStr, "", audioCodecStr};

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CALLMOD_CHANGE_INDICATOR,
            m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleECT(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint32Val(MIPC_CALL_ECT_IND_T_CALL_ID, 0);
    int result = mipcData->getMipcUint8Val(MIPC_CALL_ECT_IND_T_ECT_RESULT, 0);
    int cause = mipcData->getMipcUint32Val(MIPC_CALL_ECT_IND_T_CAUSE, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_ECT_IND AT< +ECT: %d, %d, %d", callId, result, cause);

    int data[] = {callId, result, cause};
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ECT_INDICATION,
            m_slot_id, RfxIntsData(data, sizeof(data) / sizeof(int)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleRedialEmergencyIndication(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint8Val(MIPC_CALL_ECC_REDIAL_IND_T_CALL_ID, 0);
    logD(RFX_LOG_TAG, "MIPC_CALL_ECC_REDIAL_IND AT< +ERDECCIND: %d", callId);

    String8 callIdStr = String8::format("%d", callId);
    const char *data[] = {callIdStr};

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_REDIAL_EMERGENCY_INDICATION,
            m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleImsHeaderInfo(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint32Val(MIPC_CALL_IMS_SIP_HEADER_IND_T_CALL_ID, 0);
    int headerType = mipcData->getMipcUint32Val(MIPC_CALL_IMS_SIP_HEADER_IND_T_HEADER_TYPE, 0);
    int totalCount = mipcData->getMipcUint32Val(MIPC_CALL_IMS_SIP_HEADER_IND_T_TOTAL_COUNT, 0);
    int index = mipcData->getMipcUint32Val(MIPC_CALL_IMS_SIP_HEADER_IND_T_INDEX, 0);
    char *value = mipcData->getMipcStr(MIPC_CALL_IMS_SIP_HEADER_IND_T_VALUE);
    logD(RFX_LOG_TAG, "MIPC_CALL_IMS_SIP_HEADER_IND AT< +EIMSHEADER: %d, %d, %d, %d",
            callId, headerType, totalCount, index);

    String8 callIdStr = String8::format("%d", callId);
    String8 headerTypeStr = String8::format("%d", headerType);
    String8 totalCountStr = String8::format("%d", totalCount);
    String8 indexStr = String8::format("%d", index);
    const char *data[] = {callIdStr, headerTypeStr, totalCountStr, indexStr, value};

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIP_HEADER,
            m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleCallAdditionalInfo(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    int callId = mipcData->getMipcUint32Val(MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_CALLID, 0);
    int mode = mipcData->getMipcUint8Val(MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_MODE, 0);
    int type = mipcData->getMipcUint8Val(MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_TYPE, 0);
    int total = mipcData->getMipcUint32Val(MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_TOTAL, 0);
    int index = mipcData->getMipcUint32Val(MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_INDEX, 0);
    int count = mipcData->getMipcUint32Val(MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_COUNT, 0);
    char *info = mipcData->getMipcStr(MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_ADDITIONAL_INFO);
    if (mode == MODE_ENRICHED_CALLING) {
        mode = MT_CALL_ENRICHED_CALLING;
    }
    logD(RFX_LOG_TAG, "MIPC_CALL_CALL_ADDITIONAL_INFO_IND AT< +EIMSCAI: %d, %d, %d, %d, %d, %d",
            callId, mode, type, total, index, count);

    String8 callIdStr = String8::format("%d", callId);
    String8 modeStr = String8::format("%d", mode);
    String8 typeStr = String8::format("%d", type);
    String8 totalStr = String8::format("%d", total);
    String8 indexStr = String8::format("%d", index);
    String8 countStr = String8::format("%d", count);
    const char *data[] = {modeStr, callIdStr, typeStr, totalStr, indexStr, countStr, info};

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_CALL_ADDITIONAL_INFO,
            m_slot_id, RfxStringsData((char **)data, sizeof(data) / sizeof(const char *)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleMDInternetUsage(const sp<RfxMclMessage>& msg) {
    int data[5] = { 0 };

    RfxMipcData* mipcData = msg->getMipcData();
    int sim_id = mipcData->getMipcUint8Val(MIPC_SYS_VODATA_STATISTICS_IND_T_SIM_ID, 0);
    int tx_byte = mipcData->getMipcUint32Val(MIPC_SYS_VODATA_STATISTICS_IND_T_TX_BYTES, 0);
    int rx_byte = mipcData->getMipcUint32Val(MIPC_SYS_VODATA_STATISTICS_IND_T_RX_BYTES, 0);
    int tx_pkt = mipcData->getMipcUint32Val(MIPC_SYS_VODATA_STATISTICS_IND_T_TX_PKT, 0);
    int rx_pkt = mipcData->getMipcUint32Val(MIPC_SYS_VODATA_STATISTICS_IND_T_RX_PKT, 0);

    data[0] = sim_id;
    data[1] = tx_byte;
    data[2] = rx_byte;
    data[3] = tx_pkt;
    data[4] = rx_pkt;

    logD(RFX_LOG_TAG, "RFX_MSG_URC_MD_INTERNET_USAGE_IND AT< +MDINTERNET: %d, %d, %d, %d, %d",
            sim_id, tx_byte, rx_byte, tx_pkt, rx_pkt);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_MD_INTERNET_USAGE_IND,
            m_slot_id, RfxIntsData(data, sizeof(data) / sizeof(int)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleIncomingCallIndication(const sp<RfxMclMessage>& msg) {
    RfxMipcData *data = msg->getMipcData();
    int callId = data->getMipcUint32Val(MIPC_CALL_APPROVE_INCOMING_CMD_T_CALLID, 0);
    int toa = data->getMipcUint32Val(MIPC_CALL_APPROVE_INCOMING_CMD_T_TOA, 0);
    int seqNumber = data->getMipcUint32Val(MIPC_CALL_APPROVE_INCOMING_CMD_T_SEQ_NO, 0);
    int callMode = data->getMipcUint32Val(MIPC_CALL_APPROVE_INCOMING_CMD_T_MODE, 0);
    int evoltesiFlow = data->getMipcUint8Val(MIPC_CALL_APPROVE_INCOMING_CMD_T_EVOLTESI_FLOW, 0);
    uint16_t len = 0;
    char *number = (char *)data->getMipcVal(MIPC_CALL_APPROVE_INCOMING_CMD_T_NUMBER, &len);
    String8 numberBuf = String8(number, len);
    int txid = msg->getMipcData()->getMipcMsgTxId();

    logD(RFX_LOG_TAG, "MIPC_CALL_APPROVE_INCOMING_CMD AT< +EAIC:%d, %s, %d, %d, %d, %s, %s, %d",
            callId, RfxRilUtils::pii(RFX_LOG_TAG, numberBuf), toa, seqNumber,
            callMode, "", "", evoltesiFlow);

    String8 callIdStr = String8::format("%d", callId);
    String8 toaStr = String8::format("%d", toa);
    String8 callModeStr = String8::format("%d", callMode);
    String8 seqNumberStr = String8::format("%d", seqNumber);
    String8 evoltesiFlowStr = String8::format("%d", evoltesiFlow);
    String8 txIdStr = String8::format("%d", txid);
    const char *urcData[] = {
        callIdStr.string(), numberBuf, toaStr.string(), callModeStr.string(), seqNumberStr.string(),
        "", "", evoltesiFlowStr.string(), txIdStr.string()
    };
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_INCOMING_CALL_INDICATION,
            m_slot_id, RfxStringsData((char **)urcData, sizeof(urcData) / sizeof(const char*)));
    responseToTelCore(urc);
}

void RmmCallControlUrcHandler::handleRCSMTSIPInfo(const sp<RfxMclMessage>& msg) {
    // +ERCSMTINV: <from number>, <total count>, <index>, <MT SIP INVITE>
    RfxMipcData* data = msg->getMipcData();
    int revCount=0;
    char* fromNumber = data->getMipcStr(MIPC_CALL_MT_SIP_INVITE_IND_T_FROM_NUMBER);
    int total = data->getMipcUint8Val(MIPC_CALL_MT_SIP_INVITE_IND_T_TOTAL_COUNT, 0);
    int index = data->getMipcUint8Val(MIPC_CALL_MT_SIP_INVITE_IND_T_INDEX, 0);
    char* mtSipInvite = data->getMipcStr(MIPC_CALL_MT_SIP_INVITE_IND_T_MT_SIP_INVITE);
    logD(RFX_LOG_TAG, "MIPC_CALL_IMS_SIP_INVITE_IND AT< +ERCSMTINV: ***, %d, %d",total, index);

    if (fromNumber == NULL || mtSipInvite == NULL) {
        logD(RFX_LOG_TAG, "MIPC_CALL_IMS_SIP_INVITE_IND number or content error");
        return;
    }

    if (index < 0 || index >= MTSIPINVIT_NOTIFY_BUFFER_LEN) {
        logD(RFX_LOG_TAG, "MIPC_CALL_IMS_SIP_INVITE_IND index error");
        return;
    }

    std::string sipInvite(mtSipInvite);
    logD(RFX_LOG_TAG, "handleRCSMTSIPInfo sipInvite len: %zu", sipInvite.length());
    mMTSipTepData[index - 1] = strdup(sipInvite.c_str());

    for (int i= 0; i < MTSIPINVIT_NOTIFY_BUFFER_LEN; i++) {
       if(mMTSipTepData[i] != NULL) {
           revCount++;
       }
    }

    if (revCount != total) {
        return;
    }

    std::string content = "";
    int wholelen = 0;
    for (int i = 0; i < total; i++) {
       if(mMTSipTepData[i] != NULL ) {
           std::string tmpStr(mMTSipTepData[i]);
           logD(RFX_LOG_TAG, "handleRCSMTSIPInfo : %d ,%zu", i, tmpStr.length());
           wholelen += tmpStr.length();
           content.append(mMTSipTepData[i]);
       }
    }

    const char *text[] = {fromNumber, content.c_str()};
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_MT_SIP_INVITE_INFO,
            m_slot_id, RfxStringsData((char **)text, sizeof(text) / sizeof(const char *)));
    responseToTelCore(urc);

    std::string atStr = "+ERCSMTINV: " + std::string(fromNumber) + ",";
    std::string rawStr = atStr + content;

    logD(RFX_LOG_TAG, "handleRCSMTSIPInfo len: %zu", rawStr.length());
    sp<RfxMclMessage> urc_msg = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_OEM_HOOK_RAW, m_slot_id,
         RfxStringData((char*)(rawStr.c_str()), rawStr.length()));
    responseToTelCore(urc_msg);

    for (int i = 0; i < total; i++) {
       free(mMTSipTepData[i]);
       mMTSipTepData[i] = NULL;
    }
}
/// IMS specific end
