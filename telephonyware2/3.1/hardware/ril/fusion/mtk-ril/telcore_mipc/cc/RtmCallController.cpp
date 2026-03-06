/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <string>

#include "ims/RtmImsConferenceController.h"
#include "nw/RtmNwDefs.h"
#include "MtkPhoneNumberUtils.h"
#include "RtmCallController.h"
#include "RfxRootController.h"

#include "RfxCallFailCauseData.h"
#include "RfxCallListData.h"
#include "RfxCdmaInfoRecData.h"
#include "RfxCdmaWaitingCallData.h"
#include "RfxEmergencyDialData.h"
#include "RfxDialData.h"
#include "RfxCrssNotificationData.h"
#include "RfxSuppServNotificationData.h"
#include "RfxIntsData.h"
#include "RfxRilUtils.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RtmEccNumberController.h"
#include "rfx_properties.h"
#include "SuppServDef.h"

#include "RfxGwsdUtils.h"

#include <compiler/compiler_utils.h>

#include <mtkconfigutils.h>
#include <libmtkrilutils.h>

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtmCC"
#define MIN_IMS_CALL_MODE 20
#define MAX_IMS_CALL_MODE 39
#define MIN_CDMA_CALL_MODE 40
#define MAX_CDMA_CALL_MODE 43
#define MIN_VODATA_RAT 6
#define MAX_VODATA_RAT 9
#define AUTO_ANSWER_DELAY 5 * 1000
#define CONF_CALL_ID_SHIFT 100
#define CALL_ID_NONE 0
#define SET_CALL_INDICATION_TIMER 6*1000
#define MCC_CHAR_LEN 3
#define CHINA_MCC "460"
#define DELAY_SEND_CALL_INFO_TIMER 500

#define AUTO_FAKE_ACCEPT_DELAY 6 * 1000

#define RADIO_ON_FOR_ECC_TIMEOUT (30 * 1000)
#define ANONYMOUS_INVAID  "@anonymous.invalid"


RFX_IMPLEMENT_CLASS("RtmCallController", RtmCallController, RfxController);

// register request to RfxData
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxCallListData, RFX_MSG_REQUEST_GET_CURRENT_CALLS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEmergencyDialData, RfxVoidData, RFX_MSG_REQUEST_EMERGENCY_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ANSWER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_UDUB);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_EXPLICIT_CALL_TRANSFER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxCallFailCauseData, RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_DTMF);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_DTMF_START);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_DTMF_STOP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_CALL_INDICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECC_REDIAL_APPROVE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_CURRENT_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_SET_ECC_INDICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_HANGUP_ALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_HANGUP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SEPARATE_CONNECTION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_MUTE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_MUTE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_TTY_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_TTY_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_FORCE_RELEASE_CALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEmergencyDialData, RfxVoidData, RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallData, RfxVoidData, RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_IMS_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_IMS_VT_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_HANGUP_WITH_REASON);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_CALL_SUB_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_CALL_SUB_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_AUTO_ANSWER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_SIMULTANEOUS_CALLING_SUPPORT);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_FAKE_ACCEPT); // GWSD
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_FAKE_REJECT); // GWSD
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_FAKE_ACCEPT); // GWSD
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_FAKE_REJECT); // GWSD


RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxSuppServNotificationData, RFX_MSG_UNSOL_SUPP_SVC_NOTIFICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxSuppServNotificationData, RFX_MSG_UNSOL_SUPP_SVC_NOTIFICATION_EX);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RINGBACK_TONE);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_CALL_RING);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData,  RFX_MSG_UNSOL_INCOMING_CALL_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData,  RFX_MSG_UNSOL_CALL_RF_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData,  RFX_MSG_UNSOL_CALL_ADDITIONAL_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_CIPHER_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_SPEECH_CODEC_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ESPEECH);
RFX_REGISTER_DATA_TO_URC_ID(RfxCrssNotificationData, RFX_MSG_UNSOL_CRSS_NOTIFICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_SIMULTANEOUS_CALLING_SUPPORT_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_EXIT_EMERGENCY_CALLBACK_MODE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_FORWARDED_NUMBER);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_CLEAR_FORWARDED_NUMBER);

// CDMA
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_CDMA_FLASH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_CDMA_BURST_DTMF);
RFX_REGISTER_DATA_TO_URC_ID(RfxCdmaInfoRecData, RFX_MSG_UNSOL_CDMA_INFO_REC);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_NO_EMERGENCY_CALLBACK_MODE);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_CDMA_CALL_ACCEPTED);
RFX_REGISTER_DATA_TO_URC_ID(RfxCdmaWaitingCallData, RFX_MSG_UNSOL_CDMA_CALL_WAITING);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_NO_CARRIER);

// IMS
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_CONFERENCE_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_DIAL_WITH_SIP_URI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_HOLD_CALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_RESUME_CALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ASYNC_HOLD_CALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ASYNC_RESUME_CALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_PULL_CALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_IMS_ECT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxCallListData, RFX_MSG_REQUEST_IMS_GET_CURRENT_CALLS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_IMS_HANGUP_WAITING_OR_BACKGROUND);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_IMS_HANGUP_FOREGROUND_RESUME_BACKGROUND);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_IMS_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_RTT_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_SIP_HEADER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SIP_HEADER_REPORT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_IMS_CALL_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_VOICE_DOMAIN_PREFERENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_VOICE_DOMAIN_PREFERENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_CALL_ADDITIONAL_INFO);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_CALL_INFO_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ECONF_SRVCC_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_ECONF_RESULT_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_CALLMOD_CHANGE_INDICATOR);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_VIDEO_CAPABILITY_INDICATOR);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_SRVCC_STATE_NOTIFY);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ECT_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_IMS_ASYNC_CALL_CONTROL_RESULT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_EMERGENCY_BEARER_SUPPORT_NOTIFY);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_REDIAL_EMERGENCY_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_FAKE_INCOMING_CALL_FAILURE);  // GWSD
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_SIP_HEADER);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_CALL_RAT_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_MT_SIP_INVITE_INFO);


RtmCallController::RtmCallController() :
mCallRat(CALL_RAT_NONE), mImsCallRat(CALL_RAT_NONE)
, mEccState(STATE_DISCONNECTED), mEccCallId(-1)
, mPreciseCallStateList(NULL)
, mUseLocalCallFailCause(false), mDialLastError(0)
, mEspeechOnInDialingEcc(false)
, mCsCallCount(0), mPendingCallControlMessage(NULL)
, mPendingIncomingCallId(-1), mAutoAnswerImsIncoming(AUTO_ANSWER_NONE)
, mAutoAnswerCallId(CALL_ID_NONE), mAutoAnswerVideoMode(-1)
, mWaitForSwitchCallResp(false)
, mPendingSrvccCallCount(0), mNeedResumeOrAnswerCall(false)
, mExpectedToResumeCallId(-1), mSpeechCodec(0), mCallIdToBeHeld(0), mCallIdToBeHangup(0)
, mCallIdExpectToResumeOrAnswer(0), mCallIdResuming(0), mCallIdAnswering(0)
, mPendingImsStkCallInfoMessage(NULL), mSeqNo(-1), mOriginalAnswerMessage(NULL)
, mOriginalHangupMessage(NULL), mImsFakeMode(false)
, mOriginalHangupRingingMessage(NULL)
, mRQ(0)
, mAutoFakeAcceptTimerHandle(NULL)
, mEccController(NULL) {
    unsigned int i;
    for (i = 0; i < (unsigned int)RfxRilUtils::rfxGetSimCount(); i++) {
        mSrvccState[i] = SRVCC_STATE_NONE;
    }
}

RtmCallController::~RtmCallController() {
}

void RtmCallController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    // create ECC controller
    createEccController();


    mPreciseCallStateList = new Vector<RfxPreciseCallState*>();

    const int request_id_list[] = {
        RFX_MSG_REQUEST_GET_CURRENT_CALLS,
        RFX_MSG_REQUEST_DIAL,
        RFX_MSG_REQUEST_HANGUP,
        RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE,
        RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND,
        RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND,
        RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE,
        RFX_MSG_REQUEST_HANGUP_ALL,
        RFX_MSG_REQUEST_CDMA_FLASH,
        RFX_MSG_REQUEST_EMERGENCY_DIAL,
        RFX_MSG_REQUEST_DIAL_WITH_SIP_URI,
        RFX_MSG_REQUEST_CONFERENCE_DIAL,
        RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI,
        RFX_MSG_REQUEST_PULL_CALL,
        RFX_MSG_REQUEST_IMS_DIAL,
        RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL,
        RFX_MSG_REQUEST_IMS_VT_DIAL,
        RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL,
        RFX_MSG_REQUEST_HOLD_CALL,
        RFX_MSG_REQUEST_RESUME_CALL,
        RFX_MSG_REQUEST_ASYNC_HOLD_CALL,
        RFX_MSG_REQUEST_ASYNC_RESUME_CALL,
        RFX_MSG_REQUEST_ANSWER,
        RFX_MSG_REQUEST_SET_CALL_INDICATION,
        RFX_MSG_REQUEST_FORCE_RELEASE_CALL,
        RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT,
        RFX_MSG_REQUEST_ECC_REDIAL_APPROVE,
        RFX_MSG_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE,
        RFX_MSG_REQUEST_UDUB,
        RFX_MSG_REQUEST_QUERY_TTY_MODE,
        RFX_MSG_REQUEST_SET_TTY_MODE,
        RFX_MSG_REQUEST_DTMF,
        RFX_MSG_REQUEST_DTMF_START,
        RFX_MSG_REQUEST_DTMF_STOP,
        RFX_MSG_REQUEST_HANGUP_WITH_REASON,
        RFX_MSG_REQUEST_LOCAL_FAKE_ACCEPT,  // GWSD
        RFX_MSG_REQUEST_LOCAL_FAKE_REJECT,  // GWSD
        RFX_MSG_REQUEST_IMS_GET_CURRENT_CALLS,
        RFX_MSG_REQUEST_IMS_HANGUP_WAITING_OR_BACKGROUND,
        RFX_MSG_REQUEST_IMS_HANGUP_FOREGROUND_RESUME_BACKGROUND,
        RFX_MSG_REQUEST_IMS_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE,
        RFX_MSG_REQUEST_SET_RTT_MODE,
        RFX_MSG_REQUEST_SET_SIP_HEADER,
        RFX_MSG_REQUEST_SIP_HEADER_REPORT,
        RFX_MSG_REQUEST_SET_IMS_CALL_MODE,
        RFX_MSG_REQUEST_SET_CALL_ADDITIONAL_INFO,
        RFX_MSG_REQUEST_SET_CALL_SUB_ADDRESS,
        RFX_MSG_REQUEST_GET_CALL_SUB_ADDRESS,
        RFX_MSG_REQUEST_EXPLICIT_CALL_TRANSFER,
        RFX_MSG_REQUEST_GET_SIMULTANEOUS_CALLING_SUPPORT,
    };

    const int urc_id_list[] = {
        /* Common URC */
        RFX_MSG_UNSOL_CALL_RING,
        RFX_MSG_UNSOL_CDMA_CALL_WAITING,
        /* MTK RIL URC */
        RFX_MSG_UNSOL_CALL_INFO_INDICATION,
        RFX_MSG_UNSOL_SPEECH_CODEC_INFO,
        RFX_MSG_UNSOL_ESPEECH,
        // GSM
        RFX_MSG_UNSOL_INCOMING_CALL_INDICATION,
        RFX_MSG_UNSOL_CALL_RF_INDICATION,
        RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED,
        // IMS
        RFX_MSG_UNSOL_ECONF_RESULT_INDICATION,
        RFX_MSG_UNSOL_CALLMOD_CHANGE_INDICATOR,
        RFX_MSG_UNSOL_VIDEO_CAPABILITY_INDICATOR,
        RFX_MSG_UNSOL_SIP_CALL_PROGRESS_INDICATOR,
        RFX_MSG_UNSOL_SRVCC_STATE_NOTIFY,
        RFX_MSG_UNSOL_ECT_INDICATION,
        RFX_MSG_UNSOL_IMS_ASYNC_CALL_CONTROL_RESULT,
        RFX_MSG_URC_EMERGENCY_BEARER_SUPPORT_NOTIFY,
        RFX_MSG_URC_REDIAL_EMERGENCY_INDICATION,
        RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED,
        RFX_MSG_URC_SIP_HEADER,
        RFX_MSG_URC_CALL_RAT_INDICATION,
        RFX_MSG_UNSOL_MT_SIP_INVITE_INFO,
        // ECBM
        RFX_MSG_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE,
        RFX_MSG_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE,
        RFX_MSG_UNSOL_NO_EMERGENCY_CALLBACK_MODE,
        RFX_MSG_UNSOL_FAKE_INCOMING_CALL_FAILURE,  // GWSD
        RFX_MSG_UNSOL_NO_CARRIER,
        RFX_MSG_UNSOL_SIMULTANEOUS_CALLING_SUPPORT_CHANGED,
    };

    FeatureValue featurevalue;
    memset(featurevalue.value, 0, sizeof(featurevalue.value));
    mtkGetFeature(CONFIG_CC, &featurevalue);
    if (strcmp(featurevalue.value, "1")) {
        logE(RFX_LOG_TAG, "CONFIG_CC is set to 0, disable voice call feature!");
        return;
    }

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));

    // register callbacks for CS phone
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_NWS_MODE,
        RfxStatusChangeCallback(this, &RtmCallController::onCsPhoneChanged));

    // register callback to get network type info
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SERVICE_STATE,
        RfxStatusChangeCallback(this, &RtmCallController::onServiceStateChanged));

    // register callback to get WFC state
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_WFC_STATE,
        RfxStatusChangeCallback(this, &RtmCallController::onWFCPDNStatusChanged));

    // register callbacks to get required information
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
        RfxStatusChangeCallback(this, &RtmCallController::onRadioStateChanged));

    // register callbacks to get ecc indication when radio power on status updated.
    getNonSlotScopeStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC,
            RfxStatusChangeCallback(this,
                    &RtmCallController::onRadioPowerEccStatusChanged));

    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_AUTO_ANSWER,
            RfxStatusChangeCallback(this, &RtmCallController::onAutoAnswerSettingChanged));
    rfx_property_get("persist.vendor.em.hidl.auto_answer", prop, "");
    if (strncmp(prop, "1", 1) == 0) {
        getNonSlotScopeStatusManager()->setString8Value(
                RFX_STATUS_KEY_AUTO_ANSWER, String8("1"), true);
    }

    if (m_slot_id == RFX_SLOT_ID_0) {
        getNonSlotScopeStatusManager()->registerStatusChanged(
                RFX_STATUS_DSDA_MODE,
                RfxStatusChangeCallback(this,
                        &RtmCallController::onDsdaModeChanged));
    }
}

void RtmCallController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    RfxController::onDeinit();
    freePreciseCallStateList(mPreciseCallStateList);
    mPreciseCallStateList = NULL;
}

bool RtmCallController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    bool needSendRequestToMcl = true;
    //logD(RFX_LOG_TAG, "onHandleRequest: %s", RFX_ID_TO_STR(msg_id));

    // GWSD
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        if (mAutoFakeAcceptTimerHandle != NULL) {
            if (msg_id == RFX_MSG_REQUEST_ANSWER ||
                    msg_id == RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT ||
                    msg_id == RFX_MSG_REQUEST_HANGUP ||
                    msg_id == RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND ||
                    msg_id == RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND) {
                RfxTimer::stop(mAutoFakeAcceptTimerHandle);
                mAutoFakeAcceptTimerHandle = NULL;
            }
        }
        if (RfxGwsdUtils::getCallControlHandler()->onHandleTelCoreRequest(this, message)) {
            return true;
        }
    }

    if (onHandleRequestForImsFakeMode(message)) {
        return true;
    }

    if (msg_id == RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE
          && mWaitForSwitchCallResp) {
        logE(RFX_LOG_TAG, "Cannot switch call since mWaitForSwitchCallResp = true");
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
        responseToRilj(responseMsg);
        return true;
    }

    if ((msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL
            || msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL)) {
        RIL_Emergency_Dial *pEccDial = (RIL_Emergency_Dial*) (message->getData()->getData());
        String8 dialNumber = String8::format("%s", pEccDial->dialData->address);
        RtmEccNumberController* eccNumberController = (RtmEccNumberController *)findController(
                getSlotId(), RFX_OBJ_CLASS_INFO(RtmEccNumberController));
        EmergencyCallRouting eccRouting = eccNumberController->getEmergencyCallRouting(dialNumber);

        if (msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            pEccDial->dialData->clir = 0;
        }

        // for debug
        RIL_Dial *pDial = (RIL_Dial*)(pEccDial->dialData);
        logD(RFX_LOG_TAG,
                "isTesting=%d, eccRouting=%d, routing=%d, serviceCategory=%d, clir=%d, number=%s ",
                pEccDial->isTesting, eccRouting,  pEccDial->routing, pEccDial->serviceCategory,
                pDial->clir, RfxRilUtils::pii(RFX_LOG_TAG, pDial->address));

        if (!pEccDial->isTesting && pEccDial->routing == ECC_ROUTING_EMERGENCY
                && pEccDial->serviceCategory == UNSPECIFIED) {
            // We use 0xFF to represent UNSPECIFIED service category in MIPC design.
            pEccDial->serviceCategory = (EmergencyServiceCategory)0xFF;
        }

        if (!pEccDial->isTesting &&
                pEccDial->routing == ECC_ROUTING_UNKNOWN && eccRouting == ECC_ROUTING_UNKNOWN) {
            // It's Google database ECC
            pEccDial->routing = ECC_ROUTING_EMERGENCY;
        }
        if (pEccDial->isTesting
                || pEccDial->routing == ECC_ROUTING_NORMAL || eccRouting == ECC_ROUTING_NORMAL) {
            cancelRadioOnForEcc(); // cancel radio on for ecc if enabled.
        } else {
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE, true);
        }
    }

    switch (msg_id) {
        case RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL:
        case RFX_MSG_REQUEST_IMS_DIAL:
        case RFX_MSG_REQUEST_IMS_VT_DIAL:
        case RFX_MSG_REQUEST_PULL_CALL:
        case RFX_MSG_REQUEST_CONFERENCE_DIAL:
        case RFX_MSG_REQUEST_DIAL_WITH_SIP_URI:
        case RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI:
            handleImsDialRequest(message);
            needSendRequestToMcl = handleClirSpecial(message);
            TELEPHONYWARE_FALLTHROUGH;
            // fall through
        case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL:
            handleEcallFastMakeEcall(message);
        break;
        case RFX_MSG_REQUEST_EMERGENCY_DIAL:
        case RFX_MSG_REQUEST_DIAL:
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, true);
            mUseLocalCallFailCause = false;
            mDialLastError = 0;
            break;
        case RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE  : {
            RtmImsConferenceController *imsConfCtrl = (RtmImsConferenceController *) findController(
                    m_slot_id, RFX_OBJ_CLASS_INFO(RtmImsConferenceController));

            // in the case of fake disconnect, we simulate as a local call end and return the cause directly
            if (imsConfCtrl->needProcessFakeDisconnect()) {
                mUseLocalCallFailCause = true;
                mDialLastError = CALL_FAIL_NORMAL;

                imsConfCtrl->processFakeDisconnectDone();
                logD(RFX_LOG_TAG, "Fake local call disconnect cause for slot%d", m_slot_id);
            }

            if (mUseLocalCallFailCause) {
                RIL_LastCallFailCauseInfo callFailCause;
                memset(&callFailCause, 0, sizeof(RIL_LastCallFailCauseInfo));
                callFailCause.cause_code = (RIL_LastCallFailCause) mDialLastError;
                callFailCause.vendor_cause = NULL;
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                        m_slot_id, RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE,
                        RIL_E_SUCCESS, RfxCallFailCauseData(&callFailCause, sizeof(callFailCause)),
                        message);
                responseToRilj(responseMsg);
                logD(RFX_LOG_TAG, "Use local call fail cause for slot%d", m_slot_id);
                mUseLocalCallFailCause = false;
                mDialLastError = 0;
                return true;

            }
            break;
        }
        case RFX_MSG_REQUEST_HANGUP:
        case RFX_MSG_REQUEST_HANGUP_WITH_REASON: {
            int hangupCallId = ((int *)message->getData()->getData())[0];
            CallType hangupCallType = getConnectionCallType(hangupCallId);

            if (hangupCallType == CALL_TYPE_EMERGENCY) {
                logI(RFX_LOG_TAG, "hangup call type ecc (callId=%d)", hangupCallId);
                mEccController->onEccHangupLocal();
            }

            if (hasPendingHangupRequest(hangupCallId)) {
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
                        true);
                responseToRilj(responseMsg);
                return true;
            }
            updateDisconnecting(mPreciseCallStateList, hangupCallId);
            break;
        }
        case RFX_MSG_REQUEST_HANGUP_ALL: {
            int callCount = getStatusManager(m_slot_id)->getIntValue(
                        RFX_STATUS_KEY_VOICE_CALL_COUNT, 0);
            if (callCount == 0) {
                logD(RFX_LOG_TAG, "No call, ignore hangup all: slot = %d", m_slot_id);
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                        RIL_E_GENERIC_FAILURE, message, true);
                responseToRilj(responseMsg);
                return true;
            }

            mPendingIncomingCallId = -1;
            break;
        }
        case RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND:
        case RFX_MSG_REQUEST_UDUB:
        {
            int hangupCallId = -1;
            if (hasPendingHangupRequest(false/*isForegnd*/)) {
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
                        true);
                responseToRilj(responseMsg);
                return true;
            }
            hangupCallId = findRingingConnection();
            if (hangupCallId != -1) {
                mOriginalHangupRingingMessage = message;
                // original message's pId & pToken are copied, id is replaced & token is created
                // id & token are used in MCL, while pId & pToken are used in RILC (id != pId)
                sp<RfxMessage> hangupRingingMessage = RfxMessage::obtainRequest(
                        RFX_MSG_REQUEST_HANGUP, RfxIntsData(&hangupCallId, 1), message, false);
                return onHandleRequest(hangupRingingMessage);
            } else {
                updateDisconnecting(mPreciseCallStateList, false/*isForegnd*/);
            }
            break;
        }
        case RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND:
            if (hasPendingHangupRequest(true/*isForegnd*/)) {
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
                        true);
                responseToRilj(responseMsg);
                return true;
            }
            updateDisconnecting(mPreciseCallStateList, true/*isForegnd*/);
            break;
        case RFX_MSG_REQUEST_HOLD_CALL:
        case RFX_MSG_REQUEST_RESUME_CALL:
            handleAsyncImsCallControlRequest(message);
            return true;
        case RFX_MSG_REQUEST_SET_CALL_INDICATION:
        {
            needSendRequestToMcl = handleSetCallIndicationRequest(message);
            if (needSendRequestToMcl) {
                RfxIntsData* messageData = (RfxIntsData*)message->getData();
                int* params = (int*)messageData->getData();
                int disapprove = ((int*)params)[0];
                int callId = ((int*)params)[1];
                int seqNo = ((int*)params)[2];
                int cause =  ((int*)params)[3];
                bool approve = disapprove ?  false : true;
                approveIncoming(callId, seqNo, approve, cause, message);
                return true;
            }
            break;
        }
        case RFX_MSG_REQUEST_IMS_GET_CURRENT_CALLS:
            handleImsGetCurrentCalls(message);
            return true;
        case RFX_MSG_REQUEST_IMS_HANGUP_WAITING_OR_BACKGROUND:
            handleImsHangupWaitingOrBackground(message);
            return true;
        case RFX_MSG_REQUEST_IMS_HANGUP_FOREGROUND_RESUME_BACKGROUND:
            handleImsHangupFgResumeBg(message);
            return true;
        case RFX_MSG_REQUEST_IMS_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
            handleImsSwitchFgBg(message);
            return true;
        case RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
            mWaitForSwitchCallResp = true;
            break;
        case RFX_MSG_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE:
            if (getStatusManager()->getIntValue(RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE, 0) == 0) {
                // ALPS05164131: Report exit ECBM URC in special case.
                // In BSP+ design RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE requests are sent to
                // both slots if phone process reset under ECBM (ALPS03254547), so the slot
                // not in ECBM also receives this request.
                // Report RIL_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE in this case because
                // framework expects it must be reported (23817768b7c71108fbd979ee6b385cdcf8c2239e).
                logI(RFX_LOG_TAG, "exitEmergencyCallbackMode when not in ECBM");
                responseToRilj(RfxMessage::obtainUrc(m_slot_id,
                        RFX_MSG_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE, RfxVoidData()));
            }
            break;
        case RFX_MSG_REQUEST_GET_SIMULTANEOUS_CALLING_SUPPORT:
            handleGetSimultaneousCallingSupport(message);
            return true;
        default:
            break;
    }

    if (needSendRequestToMcl) {
        if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                 msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            mEccController->onPreLocalEmergencyDial(message);
            mEccState = STATE_CONNECTING;
        }
        // when EMERGENCY_DIAL is converted to DIAL, message is different form message
        updateIsImsCallExistToStatusManager(message->getSlotId());
        requestToMcl(message);
    }
    return true;
}

bool RtmCallController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    int slotId = message->getSlotId();

    // GWSD
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        if (RfxGwsdUtils::getCallControlHandler()->onHandleTelCoreUrc(this, message)) {
            if (msg_id == RFX_MSG_UNSOL_INCOMING_CALL_INDICATION) {
                RfxStringsData *data = (RfxStringsData*)message->getData();
                char **params = (char**)data->getData();
                int callMode = atoi(params[3]);
                if (callMode < MIN_IMS_CALL_MODE || callMode > MAX_IMS_CALL_MODE) {
                    mCsCallCount++;
                    if (mCsCallCount > 0) {
                        updateRQ(1);
                    }
                    updateCallCount();
                }
            } else if (msg_id == RFX_MSG_UNSOL_CALL_INFO_INDICATION) {
               RfxStringsData *data = (RfxStringsData *)message->getData();
               char **params = (char **)data->getData();
               int msgType = atoi(params[1]);
               int callMode = atoi(params[5]);
               if (callMode < MIN_IMS_CALL_MODE || callMode > MAX_IMS_CALL_MODE) {
                   if (msgType == 133) {
                       if (mCsCallCount > 0) {
                           mCsCallCount--;
                       }
                       updateCallCount();
                   }
               }
            }
            return true;
        }
    }

    RtmImsConferenceController *imsConfCtrl = (RtmImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtmImsConferenceController));
    // logD(RFX_LOG_TAG, "onHandleUrc: %s", RFX_ID_TO_STR(msg_id));

    bool responseToRilJ = true;
    if (imsConfCtrl != NULL && imsConfCtrl->handleUrc(message)) {
       return true;
    }

    switch (msg_id) {
        case RFX_MSG_UNSOL_INCOMING_CALL_INDICATION: {
            RfxStringsData* messageData = (RfxStringsData*)message->getData();
            char** messageParams = (char**)messageData->getData();
            int countString = messageData->getDataLength() / sizeof(char *);
            sp<RfxMessage> newMessage;

            if (!handleIncomingCall(slotId, (RfxStringsData*)message->getData())) {
                return true;
            }

            // update data and not pass the last one (if exist) evoltesi_flow to RILJ
            // it is only used in RILD
            if (countString == 9) {
                logD(RFX_LOG_TAG, "RFX_MSG_UNSOL_INCOMING_CALL_INDICATION truncate evoltesi_flow");
                newMessage = RfxMessage::obtainUrc(
                        m_slot_id,
                        RFX_MSG_UNSOL_INCOMING_CALL_INDICATION,
                        RfxStringsData(messageParams, 7));
                responseToRilJAndUpdateIsImsCallExist(newMessage);
                return true;
            }
            break;
        }
        case RFX_MSG_UNSOL_CALL_RF_INDICATION: {
            RfxStringsData* messageData = (RfxStringsData*)message->getData();
            char** messageParams = (char**)messageData->getData();
            int call_id = atoi(messageParams[0]);
            int rq = atoi(messageParams[1]);
            if (rq) {
                if (RfxGwsdUtils::getCallControlHandler() != NULL) {
                    logD(RFX_LOG_TAG, "notify none gwsd ims call : %d", call_id);
                    RfxGwsdUtils::getCallControlHandler()->notifyDualSimGwsdImsIncomingCallChanged(
                                this, call_id, false);
                }
            }
            break;
        }
        case RFX_MSG_UNSOL_CALL_INFO_INDICATION: {
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED, false);
            RfxStringsData* data = (RfxStringsData*)message->getData();
            char** params = (char**)data->getData();
            int callMode = atoi(params[5]);
            int callId = atoi(params[0]);
            // ALPS04918529: VDM select IMS to dial but fail, STK call over CS eventually.
            if (mPendingImsStkCallInfoMessage != NULL &&
                    !(callMode >= MIN_IMS_CALL_MODE && callMode <= MAX_IMS_CALL_MODE)) {
                RfxStringsData* stkCallData =
                        (RfxStringsData*)mPendingImsStkCallInfoMessage->getData();
                char** stkParams = (char**)stkCallData->getData();
                int stkCallId = atoi(stkParams[1]);
                if (callId == stkCallId) {
                    mPendingImsStkCallInfoMessage = NULL;
                    RfxImsCallInfo* targetCall = getImsCallByCallId(mImsCalls[slotId], callId);
                    if (targetCall != NULL) {
                        targetCall->setCallState(RfxImsCallInfo::STATE_TERMINATED);
                        removeImsCall(slotId, targetCall);
                    }
                }
            }
            // IMS STK call => +ECPI 130 with call mode = 20
            // ATD => +ECPI: 130 with call mode = 0 and mEstablishingCall != NULL
            // IMS voice/video Conference => +ECPI 130 with call mode = 22/23
            if (hasImsCall(slotId) ||
                    (callMode >= MIN_IMS_CALL_MODE && callMode <= MAX_IMS_CALL_MODE)) {
                responseToRilJ = handleImsCallInfoUpdate(message);
            } else {
                responseToRilJ = handleCsCallInfoUpdate(message);
                //logD(RFX_LOG_TAG, "No ImsCall, report CALL_STATE_CHANGED");
                return true;
            }
            break;
        }
        case RFX_MSG_UNSOL_SPEECH_CODEC_INFO:
            if (hasImsCall(slotId)) {
                handleSpeechCodecInfo(message);
                responseToRilJ = true;
            } else {
                sp<RfxMessage> urcToRilj;
                urcToRilj = RfxMessage::obtainUrc(m_slot_id,
                        RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED,
                        RfxVoidData());
                responseToRilj(urcToRilj);
                return true;
            }
            break;

        case RFX_MSG_UNSOL_ESPEECH: {
            bool bEspeechOn = ((int*)(message->getData()->getData()))[0] == 1 ? true : false;
            if (isInChina() && bEspeechOn &&
                    (mEccState == STATE_DIALING || mEccState == STATE_ALERTING)) {
                mEspeechOnInDialingEcc = true;
                mEccController->onEccActive();
            }
            responseToRilJ = false;
            break;
        }
        case RFX_MSG_UNSOL_SRVCC_STATE_NOTIFY:
            handleSrvcc(slotId, message);
            break;
        case RFX_MSG_UNSOL_CALL_RING:
            // GWSD
            // Skip the following if no need to auto CSFB after 6 second timeout
            /*
            if (RfxGwsdUtils::getCallControlHandler() != NULL &&
                    mAutoFakeAcceptTimerHandle == NULL) {
                mAutoFakeAcceptTimerHandle = RfxTimer::start(RfxCallback0(this,
                        &RtmCallController::onAutoFakeAcceptTimer), ms2ns(AUTO_FAKE_ACCEPT_DELAY));
            }
            */
            if (hasImsCall(slotId)) {
                handleCallRing(slotId);
            }
            break;
        case RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED:
            if (hasImsCall(slotId)) {
                //logD(RFX_LOG_TAG, "ImsCall exist, ignore the CALL_STATE_CHANGED");
                return true;
            }
            break;
        case RFX_MSG_UNSOL_CALLMOD_CHANGE_INDICATOR:
            if (hasImsCall(slotId)) {
                responseToRilJ = handlePendingMTCallMode(message);
                responseToRilJ &= handleImsCallModeChange(message);
            }
            break;
        case RFX_MSG_UNSOL_IMS_ASYNC_CALL_CONTROL_RESULT:
            handleAsyncCallControlResult(message);
            return true;
        case RFX_MSG_URC_EMERGENCY_BEARER_SUPPORT_NOTIFY:
            handleEmergencyBearerSupportNotify(message);
            break;
        case RFX_MSG_UNSOL_ECONF_RESULT_INDICATION:
            handleEconfResult(message);
            break;
        case RFX_MSG_UNSOL_NO_CARRIER:
            if (isCdmaCallExist()) {
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED, true);
                sp<RfxMessage> urcToRilj = RfxMessage::obtainUrc(m_slot_id,
                        RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED,
                        RfxVoidData());
                responseToRilj(urcToRilj);
                return true;
            }
            break;
        case RFX_MSG_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE:
            getStatusManager()->setIntValue(RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE, 1);
            break;
        case RFX_MSG_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE:
        case RFX_MSG_UNSOL_NO_EMERGENCY_CALLBACK_MODE:
            getStatusManager()->setIntValue(RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE, 0);
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE, false);
            break;
        default:
            break;
    }

    if (responseToRilJ) {
        responseToRilJAndUpdateIsImsCallExist(message);
    }
    return true;
}

bool RtmCallController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    //logD(RFX_LOG_TAG, "onHandleResponse: %s", RFX_ID_TO_STR(msg_id));

    if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
             msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
        mEccController->onPostLocalEmergencyDial(message);
        setEccPowerOnStatus(false);
        if (message->getError() != RIL_E_SUCCESS) {
            mEccState = STATE_DISCONNECTED;
        }
    }

    // GWSD
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        if (RfxGwsdUtils::getCallControlHandler()->onHandleTelCoreResponse(this, message)) {
            return true;
        }
    }

    if (onHandleResponseForImsFakeMode(message)) {
        return true;
    }

    switch (msg_id) {
        case RFX_MSG_REQUEST_GET_CURRENT_CALLS:
            handleGetCurrentCallsResponse(message);
            responseToRilJAndUpdateIsImsCallExist(message);
            return true;
        case RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL:
        case RFX_MSG_REQUEST_EMERGENCY_DIAL:
        case RFX_MSG_REQUEST_IMS_DIAL:
        case RFX_MSG_REQUEST_DIAL_WITH_SIP_URI:
        case RFX_MSG_REQUEST_CONFERENCE_DIAL:
        case RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI:
        case RFX_MSG_REQUEST_IMS_VT_DIAL:
            if (message->getError() != RIL_E_SUCCESS) {
                handleImsDialFailResponse(message);
                responseToRilJAndUpdateIsImsCallExist(message);
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, false);
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE, false);
                return true;
            }
            break;
        case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL:
            if(message->getError() != RIL_E_SUCCESS) {
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, false);
                if(isVolteEnabled()) {
                    handleImsDialFailResponse(message);
                    responseToRilJAndUpdateIsImsCallExist(message);
                    getStatusManager()->setBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE, false);
                }
            }
        break;
        case RFX_MSG_REQUEST_DIAL:
            if (message->getError() != RIL_E_SUCCESS) {
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, false);
            }
            break;

        case RFX_MSG_REQUEST_UDUB:
            {
                int callCount = getStatusManager(m_slot_id)->getIntValue(
                        RFX_STATUS_KEY_VOICE_CALL_COUNT, 0);
                int card_types = getStatusManager(m_slot_id)->getIntValue(
                        RFX_STATUS_KEY_CARD_TYPE, -1);
                if (callCount == 0 && card_types <= 0) {
                    logI(RFX_LOG_TAG, "handle UDUB response, update error code (callCount:%d, card_types %d)",
                            callCount, card_types);
                    /// M: CC: update response code to fix VTS
                    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                            RIL_E_INVALID_STATE, message, true);
                    responseToRilj(responseMsg);
                    return true;
                }
            }
            break;
        case RFX_MSG_REQUEST_ASYNC_HOLD_CALL:
        case RFX_MSG_REQUEST_ASYNC_RESUME_CALL:
            handleAsyncCallControlResponse(message);
            return true;
        case RFX_MSG_REQUEST_HANGUP:
        case RFX_MSG_REQUEST_HANGUP_WITH_REASON:
            if (message->getError() != RIL_E_SUCCESS) {
                // ALPS03792709: In corner cases call will be aborted and removed from telecom
                // while hanging up is actually fail. So the call status in framework and
                // telecom will be different and cause problems. Retry to hang up the call.
                if (message->getData()->getData() != NULL) {
                    int hangupCallId = ((int *)message->getData()->getData())[0];
                    if (hasPendingHangupRequest(hangupCallId)) {
                        logD(RFX_LOG_TAG, "HANGUP error, force release");
                    }
                }
            }

            if (mHangupWaitingOrBgMessage != NULL) {
                logD(RFX_LOG_TAG, "mHangupWaitingOrBgMessage: send response");
                responseToRilj(RfxMessage::obtainResponse(message->getError(), mHangupWaitingOrBgMessage, false));
                mHangupWaitingOrBgMessage = NULL;
                return true;
            } else if (mHangupFgResumeBgMessage != NULL && !mNeedResumeOrAnswerCall) {
                logD(RFX_LOG_TAG, "mHangupFgResumeBgMessage: send response");
                responseToRilj(RfxMessage::obtainResponse(message->getError(), mHangupFgResumeBgMessage, false));
                mHangupFgResumeBgMessage = NULL;
                return true;
            }
            break;
        case RFX_MSG_REQUEST_ANSWER:
        case RFX_MSG_REQUEST_RESUME_CALL:
            // No need to wait +ECPI when resume/answer failed
            if (message->getError() != RIL_E_SUCCESS) {
                if (mCallIdResuming != 0 && msg_id == RFX_MSG_REQUEST_RESUME_CALL) {
                    mCallIdResuming = 0;
                } else if (mCallIdAnswering != 0 && msg_id == RFX_MSG_REQUEST_ANSWER) {
                    mCallIdAnswering = 0;
                }
            }

            // Swap back if resume/answer failed
            if (mCallIdToBeHeld != 0) {
                if (message->getError() != RIL_E_SUCCESS) {
                    int msg_data[1];
                    msg_data[0] = mCallIdToBeHeld;
                    logE(RFX_LOG_TAG, "Resume or answer failed, swap back %d", mCallIdToBeHeld);
                    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_RESUME_CALL,
                            RfxIntsData(msg_data, 1));
                    requestToMcl(msg);
                    mCallIdResuming = mCallIdToBeHeld;
                }
                mCallIdToBeHeld = 0;
            }

            if (mHangupFgResumeBgMessage != NULL) {
                logD(RFX_LOG_TAG, "mHangupFgResumeBgMessage: send response");
                responseToRilj(RfxMessage::obtainResponse(message->getError(), mHangupFgResumeBgMessage, false));
                mHangupFgResumeBgMessage = NULL;
                return true;
            } else if (mSwitchFgBgMessage != NULL) {
                logD(RFX_LOG_TAG, "mSwitchFgBgMessage: send response");
                responseToRilj(RfxMessage::obtainResponse(message->getError(), mSwitchFgBgMessage, false));
                mSwitchFgBgMessage = NULL;
                return true;
            }
            break;
        case RFX_MSG_REQUEST_HOLD_CALL:
            if (message->getError() != RIL_E_SUCCESS) {
                if (mSwitchFgBgMessage != NULL && mNeedResumeOrAnswerCall) {
                    logE(RFX_LOG_TAG, "Hold call failed, no need to resume background");
                    sp<RfxMessage> responseMsg =
                            RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, mSwitchFgBgMessage, true);
                    responseToRilj(responseMsg);
                    mSwitchFgBgMessage = NULL;
                    mNeedResumeOrAnswerCall = false;
                    mCallIdExpectToResumeOrAnswer = 0;
                    mCallIdToBeHeld = 0;
                    return true;
                }
            }

            if (mSwitchFgBgMessage != NULL && !mNeedResumeOrAnswerCall) {
                logD(RFX_LOG_TAG, "mSwitchFgBgMessage: send response");
                responseToRilj(RfxMessage::obtainResponse(message->getError(), mSwitchFgBgMessage, false));
                mSwitchFgBgMessage = NULL;
                return true;
            }
            break;
        case RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
            mWaitForSwitchCallResp = false;
            break;
        default:
            break;
    }

    if (mOriginalHangupRingingMessage != NULL &&
            msg_id == RFX_MSG_REQUEST_HANGUP &&
            message->getPToken() == mOriginalHangupRingingMessage->getPToken()) {
        // original message's pId & pToken are copied, id is replaced & token is created
        // since pId is correct here, it is not necessary to change id (nice to do)
        responseToRilj(RfxMessage::obtainResponse(mOriginalHangupRingingMessage->getId(), message));
        mOriginalHangupRingingMessage = NULL;
    } else {
        responseToRilj(message);
    }
    return true;
}

bool RtmCallController::onPreviewMessage(const sp<RfxMessage>& message) {
    if (!canHandleHangupForegndOrBackgndRequest(message)) {
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, true);
        return false;
    }

    return canHandleCsDialRequest(message)
        && canHandleRequestDuringComboOperations(message)
        && canHandleAnswerRequest(message);
}

void RtmCallController::handleSpeechCodecInfo(const sp<RfxMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int speechCodec = pInt[0];
    if (mSpeechCodec != speechCodec) {
        mSpeechCodec = speechCodec;
        if (hasImsCall(m_slot_id))  {
            sp<RfxMessage> urcToRilj = RfxMessage::obtainUrc(m_slot_id,
                    RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED, RfxVoidData());
            responseToRilj(urcToRilj);
        }
    }
}

void RtmCallController::handleAsyncCallControlResponse(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "handleAsyncCallControlResponse %d", message->getError());
    if (mPendingCallControlMessage == NULL) {
        logD(RFX_LOG_TAG, "No pending unsynchronize call control exist");
        return;
    }
    if (message->getError() == RIL_E_ERROR_ASYNC_IMS_CALL_CONTROL_WAIT_RESULT) {
        logD(RFX_LOG_TAG, "MD accept unsynchronize call control");
        return;
    }
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
            message->getError(), mPendingCallControlMessage, true);
    responseToRilj(responseMsg);
    mPendingCallControlMessage = NULL;
}

void RtmCallController::handleAsyncImsCallControlRequest(const sp<RfxMessage>& message) {
    if (mPendingCallControlMessage != NULL) {
        logD(RFX_LOG_TAG, "Pending unsynchronize call control exist");
        sp<RfxMessage> responseMsg =
                RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
        responseToRilj(responseMsg);
        return;
    }
    mPendingCallControlMessage = message;

    void* params = message->getData()->getData();
    int msg_data[1];
    int callId = ((int*)params)[0];
    msg_data[0] = callId;

    int requestId = (message->getId() == RFX_MSG_REQUEST_HOLD_CALL)?
            RFX_MSG_REQUEST_ASYNC_HOLD_CALL : RFX_MSG_REQUEST_ASYNC_RESUME_CALL;
    sp<RfxMessage> request =
            RfxMessage::obtainRequest(getSlotId(), requestId, RfxIntsData(msg_data, 1));
    requestToMcl(request);
}

void RtmCallController::handleAsyncCallControlResult(const sp<RfxMessage>& message) {
    /* +ECCTRL: <call_id>, <cmd>, <result>, [<failed cause>]
     * call_id: ignore
     * cmd: 131 (hold), 132 (resume)
     * result: 0 (success), 1 (failed)
     * failed cause: optional failed cause
     */
    if (mPendingCallControlMessage == NULL) {
        logD(RFX_LOG_TAG, "No pending unsynchronize call control exist");
        return;
    }
    void* params = message->getData()->getData();
    int len = message->getData()->getDataLength() / sizeof(int);
    int result = ((int*)params)[2];
    RIL_Errno cause = RIL_E_SUCCESS;
    if (len == 4 && result == 1) {
        int failedCause = ((int*)params)[3];
        if (failedCause == CME_HOLD_FAILED_CAUSED_BY_TERMINATED) {
            cause = RIL_E_ERROR_IMS_CONTROL_CALL_FAILED_CALL_TERMINATED;
        } else {
            cause = (RIL_Errno)failedCause;
        }
    }
    logD(RFX_LOG_TAG, "handleAsyncCallControlResult len= %d, error= %d", len, (int)cause);
    sp<RfxMessage> responseMsg =
            RfxMessage::obtainResponse(cause, mPendingCallControlMessage, true);
    responseToRilj(responseMsg);
    mPendingCallControlMessage = NULL;
}

bool RtmCallController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    return canHandleCsDialRequest(message)
        && canHandleRequestDuringComboOperations(message)
        && canHandleAnswerRequest(message)
        && canHandleHangupForegndOrBackgndRequest(message);
}

// [ALPS04642635] Block CS Dial if there is unfinished call switching
// Let modem to reject Dial if switching call fails
bool RtmCallController::canHandleCsDialRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_DIAL || msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL) {
            return (!mWaitForSwitchCallResp);
        }
    }
    return true;
}


bool RtmCallController::canHandleHangupForegndOrBackgndRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND ||
                msg_id == RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND) {
            return (!mWaitForSwitchCallResp);
        }
    }
    return true;
}

bool RtmCallController::canHandleRequestDuringComboOperations(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_IMS_GET_CURRENT_CALLS || msg_id == RFX_MSG_REQUEST_HANGUP) {
            if (mHangupWaitingOrBgMessage != NULL || mHangupFgResumeBgMessage != NULL ||
                    mSwitchFgBgMessage != NULL) {
                return false;
            }
            if (msg_id == RFX_MSG_REQUEST_IMS_GET_CURRENT_CALLS) {
                if (mCallIdResuming != 0 || mCallIdAnswering != 0 ||
                        mCallIdsDisconnecting.size() != 0 || mToBeMergedCalls.size() != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool RtmCallController::canHandleAnswerRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if ((msg_id == RFX_MSG_REQUEST_ANSWER || msg_id == RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT)
                && mPendingCallControlMessage != NULL
                && mPendingCallControlMessage->getId() == RFX_MSG_REQUEST_HOLD_CALL) {
            return false;
        }
    }
    return true;
}

bool RtmCallController::onCheckIfRejectMessage(
        const sp<RfxMessage>& message, bool isModemPowerOff, int radioState) {

    int isWfcSupport = RfxRilUtils::isWfcSupport();
    int msgId = message->getId();

    // M: CC: FLIGHT MODE POWER OFF MD check
    // ro.mtk_flight_mode_power_off_md=1 : isModemPowerOff=true, radioState=RADIO_STATE_OFF
    // ro.mtk_flight_mode_power_off_md=0 : isModemPowerOff=false, radioState=RADIO_STATE_OFF
    if (isModemPowerOff && msgId == RFX_MSG_REQUEST_GET_CURRENT_CALLS) {
       return true;
    }

    // Reject new ims combo operations when there's a combo operations on-going.
    if (msgId == RFX_MSG_REQUEST_IMS_HANGUP_WAITING_OR_BACKGROUND ||
            msgId == RFX_MSG_REQUEST_IMS_HANGUP_FOREGROUND_RESUME_BACKGROUND ||
            msgId == RFX_MSG_REQUEST_IMS_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE) {
        if (mHangupWaitingOrBgMessage != NULL || mHangupFgResumeBgMessage != NULL ||
                mSwitchFgBgMessage != NULL) {
            logW(RFX_LOG_TAG, "block consecutive combo request");
            return true;
        }
    }

    // The RIL Request should be bypass anyway for WFC
    // If you need to add any logic below this part, should consider carefully.
    if (isWfcSupport == 1 && !isModemPowerOff) {
        return false;
    }

    if (radioState == (int)RADIO_STATE_OFF) {
        // Need to unblock requests not handled by TCL here
        if (msgId == RFX_MSG_REQUEST_QUERY_TTY_MODE ||
                msgId == RFX_MSG_REQUEST_SET_TTY_MODE ||
                msgId == RFX_MSG_REQUEST_GET_CURRENT_CALLS ||
                msgId == RFX_MSG_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE) {
            return false;
        }
        // reset emergency Mode to false, before blocking EMERGENCY_DIAL
        if (msgId == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                msgId == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            cancelRadioOnForEcc(); // cancel radio on for ecc if enabled.
            return true;
        }
    }

    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

void RtmCallController::onCsPhoneChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    int newNws = newValue.asInt();
    int oldNws = oldValue.asInt();

    int preCsPhone = (oldNws == NWS_MODE_CDMALTE) ? RADIO_TECH_GROUP_C2K : RADIO_TECH_GROUP_GSM;
    int newCsPhone = (newNws == NWS_MODE_CDMALTE) ? RADIO_TECH_GROUP_C2K : RADIO_TECH_GROUP_GSM;

    logD(RFX_LOG_TAG, "[%s], preCsPhone:%d, newCsPhone:%d", __FUNCTION__, preCsPhone, newCsPhone);

    // If phone type is C2K, there will be no IMS call, don't need to consider IMS call case
    if (preCsPhone == RADIO_TECH_GROUP_C2K && newCsPhone == RADIO_TECH_GROUP_GSM &&
            isCsActiveOrIncomingCallExistAndNoEccExist() && isCdmaCallExist()) {
        logD(RFX_LOG_TAG,"[%s], hang up normal call due to phone switch from C2K to GSM",
                __FUNCTION__);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(
                getSlotId(), RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData());
        requestToMcl(msg);
        mPendingIncomingCallId = -1;
    }
}

void RtmCallController::onServiceStateChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    RFX_UNUSED(oldValue);
    RfxNwServiceState newSS = newValue.asServiceState();

    int dataRegState = newSS.getRilDataRegState();
    int dataRadioTech = newSS.getRilDataRadioTech();
    int wfcRegState = getStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);

    updateImsCallRat(dataRegState, dataRadioTech, wfcRegState);
}

void RtmCallController::onWFCPDNStatusChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    RFX_UNUSED(oldValue);

    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState serviceState = getStatusManager()
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    int dataRegState = serviceState.getRilDataRegState();
    int dataRadioTech = serviceState.getRilDataRadioTech();
    int wfcRegState = newValue.asInt();

    updateImsCallRat(dataRegState, dataRadioTech, wfcRegState);
}

void RtmCallController::updateImsCallRat(int dataRegState, int dataRadioTech, int wfcRegState) {
    if (!hasImsCall(m_slot_id)) {
        return;
    }

    logD(RFX_LOG_TAG, "updateImsCallRat dataRegState %d dataRadioTech %d wfcRegState %d",
            dataRegState, dataRadioTech, wfcRegState);
    // Only need report for 3gpp case.
    if (wfcRegState == 0 && RfxNwServiceState::isInService(dataRegState)) {
        // 1: LTE
        // 2: WiFi
        // 3: NR
        int callRat = 0;
        if (dataRadioTech == RADIO_TECH_LTE || dataRadioTech == RADIO_TECH_LTE_CA) {
            callRat = 1;
            mImsCallRat = CALL_RAT_LTE;
        } else if (dataRadioTech == RADIO_TECH_NR) {
            callRat = 3;
            mImsCallRat = CALL_RAT_NR;
        } else {
            return;
        }

        int urcData[2];
        urcData[0] = 1;  // IMS
        urcData[1] = callRat;
        sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(),
                RFX_MSG_URC_CALL_RAT_INDICATION, RfxIntsData(urcData, 2));
        responseToRilj(urc);
    } else if (wfcRegState == 1) {
        mImsCallRat = CALL_RAT_WIFI;
    } else {
        // Default set to LTE is safer.
        mImsCallRat = CALL_RAT_LTE;
    }
}

void RtmCallController::onRadioStateChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant value) {
    int oldState = -1, newState = -1;

    RFX_UNUSED(key);
    oldState = old_value.asInt();
    newState = value.asInt();

    if (newState == RADIO_STATE_UNAVAILABLE || newState == RADIO_STATE_OFF) {
        mEccCallId = -1;
        mEccState = STATE_DISCONNECTED;
        mRQ = 0;
        // GWSD
        if (RfxGwsdUtils::getCallControlHandler() != NULL) {
            RfxGwsdUtils::getCallControlHandler()->onRadioOff(this);
        }
        resetImsFakeMode();
    }
}

void RtmCallController::onAutoAnswerSettingChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    String8 enable = value.asString8();
    logD(RFX_LOG_TAG, "onAutoAnswerSettingChanged value : %s", enable.string());
    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_AUTO_ANSWER,
            RfxStringData((char *)enable.string(), enable.length()));
    requestToMcl(msg);
}

bool RtmCallController::isCsActiveOrIncomingCallExistAndNoEccExist() {
    int size = mPreciseCallStateList->size();

    if (size <= 0) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        if (mPreciseCallStateList->itemAt(i)->mCallType == CALL_TYPE_EMERGENCY) {
            return false;
        }
        if (mPreciseCallStateList->itemAt(i)->mOrigState == ORIG_ACTIVE
                || mPreciseCallStateList->itemAt(i)->mOrigState == ORIG_INCOMING) {
            return true;
        }
    }

    return false;
}

bool RtmCallController::isCdmaCallExist() {
    int size = mPreciseCallStateList->size();

    if (size <= 0) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        if (mPreciseCallStateList->itemAt(i)->mCallRat == CALL_RAT_CDMA) {
            return true;
        }
    }

    return false;
}

bool RtmCallController::isVolteEnabled() {
    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState ss = getStatusManager()
        ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    int data_reg = ss.getRilDataRegState();
    logD(RFX_LOG_TAG, "%s data_reg: %d", __FUNCTION__, data_reg);
    return (data_reg >= 1 && data_reg <= 3);
}

/* IMS Call Start */
bool RtmCallController::hasImsCall(int slotId) {
    if ((mImsCalls[slotId].size() == 0) && (mEstablishingCall[slotId] == NULL)) {
    mtk_property_set("vendor.ril.isimscall","0");
        return false;
    }
    mtk_property_set("vendor.ril.isimscall","1");
    return true;
}

bool RtmCallController::handleIncomingCall(int slotId, RfxStringsData* data) {
    /* Before MD 93 */
    /* +EAIC:<call_id>,<number>,<type>,<call_mode>,<seq_no>[,<redirect_num>[,<digit_to_line_num>]]*/
    /* After MD 95 */
    /* +EAIC: <call_id>,<number>,<type>,<call_mode>,<seq_no>
     *       ,[<redirect_num>],[<digit_to_line_num>],<evoltesi_flow>
     */

    char** params = (char**)data->getData();
    int callId = atoi(params[0]);
    char* number = params[1];
    int callMode = atoi(params[3]);
    int seqNo = atoi(params[4]);
    int txId = atoi(params[8]);
    mApproveIncomingCallTxIds[seqNo] = txId;
    logD(RFX_LOG_TAG, "handleIncomingCall seq : %d , txId : %d", seqNo, txId);

    RIL_CALL_INFO_TYPE status = shouldRejectIncomingCall(callMode);
    if (status == MT_CALL_REJECTED || status == MT_CALL_MISSED) {
        approveIncoming(callId, seqNo, false, -1, NULL);

        // Do not generate log for rejected call
        if (status == MT_CALL_MISSED) {
            char **urcData = (char **) calloc(3, sizeof(char *));
            RFX_ASSERT(urcData != NULL);
            asprintf(&urcData[0], "%d", MT_CALL_MISSED);
            urcData[1] = params[1];  //number
            urcData[2] = params[3];  //callMode
            sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(),
                    RFX_MSG_UNSOL_CALL_ADDITIONAL_INFO,
                    RfxStringsData(urcData, 3));
            responseToRilj(urc);
            free(urcData[0]);
            free(urcData);
        }

        return false;
    }

    if (callMode >= MIN_IMS_CALL_MODE && callMode <= MAX_IMS_CALL_MODE) {
        bool isEcc = false;
        bool isVideo = false;
        bool isConference = false;
        bool isConferenceHost = false;

        if (callMode == RfxImsCallInfo::IMS_VOICE_CONF ||
                callMode == RfxImsCallInfo::IMS_VIDEO_CONF ||
                callMode == RfxImsCallInfo::IMS_VOICE_CONF_PARTS ||
                callMode == RfxImsCallInfo::IMS_VIDEO_CONF_PARTS) {
            isConference = true;
        }

        if (callMode == RfxImsCallInfo::IMS_VOICE_CONF ||
                callMode == RfxImsCallInfo::IMS_VIDEO_CONF) {
            isConferenceHost = true;
        }

        if (callMode == RfxImsCallInfo::IMS_VIDEO ||
                callMode == RfxImsCallInfo::IMS_VIDEO_CONF ||
                callMode == RfxImsCallInfo::IMS_VIDEO_CONF_PARTS) {
            isVideo = true;
        }

        RfxImsCallInfo* call = new RfxImsCallInfo(callId, RfxImsCallInfo::STATE_ESTABLISHING,
                String8::format("%s", number), isConference, isConferenceHost, isEcc, isVideo, true);
        addImsCall(slotId, call);
        mPendingIncomingCallId = callId;

        // if evoltesi_flow exist, update
        if ((params[7] != NULL) && (strlen(params[7]) != 0)) {

            // evoltesi_flow = 0, original design, RF is ocuppied when MT come
            // evoltesi_flow = 1, RFwill be ocuppied until MT call is accepted
            int evoltesi_flow = atoi(params[7]);

            logD(RFX_LOG_TAG, "handleIncomingCall evoltesi_flow : %d", evoltesi_flow);

            if (evoltesi_flow == 0) {
                updateRQ(1);
            } else {
                updateRQ(0);
            }

        // if evoltesi_flow not exist, old design update
        } else {
            updateRQ(1);
        }

    } else {
        mCsCallCount++;
        if (mCsCallCount > 0) {
            /* RF is ocuppied when CS call exists */
            updateRQ(1);
        }
        updateCallCount();
    }

    bool isAutoApprove = false;
    isAutoApprove = isAutoApproveIncomingCallIndication() ||
        getStatusManager()->getString8Value(RFX_STATUS_KEY_CARRIER_AUTO_APPROVE_INCOMING) == "true";

    // For IMS call, approve it in sendIncomingCallIndication() of ImsService
    if (isAutoApprove && !hasImsCall(slotId)) {
        approveIncoming(callId, seqNo, true, -1, NULL);
        return false;
    }
    return true;
}

void RtmCallController::handleImsDialRequest(const sp<RfxMessage>& message) {
    int slotId = message->getSlotId();
    if (mEstablishingCall[slotId] != NULL) {
        logD(RFX_LOG_TAG, "An establishing ImsCall exist slot: %d", slotId);
        return;
    }

    String8 dialNumber = String8::format("%s", "");
    if (message->getId() == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL ||
        message->getId() == RFX_MSG_REQUEST_IMS_DIAL) {
        RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
        dialNumber = String8::format("%s", pDial->address);
    }

    bool isEcc = false;
    if (message->getId() == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL) {
        isEcc = true;
    }

    bool isVideo = false;
    if (message->getId() == RFX_MSG_REQUEST_IMS_VT_DIAL ||
        message->getId() == RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI) {
        isVideo = true;
    }

    bool isConference = false;
    bool isConferenceHost = false;
    if (message->getId() == RFX_MSG_REQUEST_CONFERENCE_DIAL) {
        isConference =  true;
        isConferenceHost = true;
    }
    RfxImsCallInfo* call = new RfxImsCallInfo(-1, RfxImsCallInfo::STATE_ESTABLISHING,
            dialNumber, isConference, isConferenceHost, isEcc, isVideo, false);

    /* RF is ocuppied when MO call exists */
    updateRQ(1);

    mEstablishingCall[slotId] = call;
    logD(RFX_LOG_TAG, "New establishing ImsCall in slot: %d", slotId);
}

void RtmCallController::handleEcallFastMakeEcall(const sp<RfxMessage>& message) {
    logD(RFX_LOG_TAG, "handleEcallFastMakeEcall");
    int slotId = message->getSlotId();
    if (mEstablishingCall[slotId] != NULL) {
        logD(RFX_LOG_TAG, "An establishing ImsCall exist slot: %d", slotId);
        return;
    }

    String8 dialNumber = String8::format("%s", "");
    if(isVolteEnabled()) {
        RIL_ECallReqMsg *eCallReqMsg = NULL; //middle struct for data from parcel to md
        eCallReqMsg = (RIL_ECallReqMsg*)(message->getData()->getData());
        dialNumber = String8::format("%s", eCallReqMsg->address);
        RfxImsCallInfo* call = new RfxImsCallInfo(-1, RfxImsCallInfo::STATE_ESTABLISHING,
                dialNumber, false, false, false, false, false);
        /* RF is ocuppied when MO call exists */
        updateRQ(1);
        mEstablishingCall[slotId] = call;
        logD(RFX_LOG_TAG, "New establishing ImsCall in slot: %d", slotId);
    }

}

void RtmCallController::handleImsDialFailResponse(const sp<RfxMessage>& message) {
    imsCallEstablishFailed(message->getSlotId());
}

bool RtmCallController::handleImsCallInfoUpdate(const sp<RfxMessage>& msg) {
    /* +ECPI: <call_id>, <msg_type>, <is_ibt>,
     *         <is_tch>, <dir>, <call_mode>, <number>, <type>, "<pau>", [<cause>] */
    int slotId = msg->getSlotId();

    RfxStringsData* data = (RfxStringsData*)msg->getData();
    char** params = (char**)data->getData();
    int callId = atoi(params[0]);
    int msgType = atoi(params[1]);
    int callMode = atoi(params[5]);
    char* ecpiNumber = params[6];
    char* pau = params[8];
    bool needSendCallStateChange = true;

    logD(RFX_LOG_TAG, "imsCallInfoUpdate slot: %d, callId: %d, ms: %d", slotId, callId, msgType);

    if (msgType == 133) {
        mUseLocalCallFailCause = false;
    }

    RtmImsConferenceController *imsConfCtrl = (RtmImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtmImsConferenceController));

    // for the case 133, it will be fake disconnect create by RtmImsConferenceController
    // need to let it pass
    if(imsConfCtrl->needSkipDueToFakeDisconnect(callId) && msgType != 133) {
        return false;
    }

    if (msgType == 133 && mPendingSrvccCallCount > 0) {
        --mPendingSrvccCallCount;
        logD(RFX_LOG_TAG, "handleImsCallInfoUpdate decSrvccCallCount: %d", mPendingSrvccCallCount);
    }

    if (msgType == 130 && mEccState == STATE_CONNECTING) {
        mEccCallId = callId;
        mEccState = STATE_DIALING;
    } else if (mEccCallId == callId) {
        // Emergency call end
        if (msgType == 133) {
            if (mEspeechOnInDialingEcc == true) {
                mUseLocalCallFailCause = true;
                mDialLastError = CALL_FAIL_NORMAL;
                mEspeechOnInDialingEcc = false;
            }
            maybeSendNoEcbm(false);
            mEccCallId = -1;
            mEccState = STATE_DISCONNECTED;
        } else if (msgType == 2) {
            mEccState = STATE_ALERTING;
        } else if (msgType == 6) {
            mEspeechOnInDialingEcc = false;
            mEccState = STATE_ACTIVE;
            mEccController->onEccActive();
        }
    }

    bool ret = true;

    RfxImsCallInfo* targetCall = getImsCallByCallId(mImsCalls[slotId], callId);
    if (targetCall == NULL && msgType != 130) {
        logD(RFX_LOG_TAG, "Can't find callId: %d, in slot: %d", callId, slotId);
        return ret;
    }

    string ecpiNumberStr = ecpiNumber == NULL ? "" :
            normalizeNumberFromCLIR(string(ecpiNumber));
    if (targetCall != NULL) {
        bool shouldUpdateAddressFromEcpi = getStatusManager()->getString8Value(
                RFX_STATUS_KEY_CARRIER_UPDATE_ADDRESS_FROM_ECPI) == "true";
        if (shouldUpdateAddressFromEcpi || targetCall->getNumber().isEmpty()) {
            logD(RFX_LOG_TAG, "Call %d update by ecpi number=%s",
                    callId, RfxRilUtils::pii(RFX_LOG_TAG, ecpiNumberStr.c_str()));
            if (strcmp(ecpiNumberStr.c_str(), targetCall->getNumber().string())) {
                targetCall->setNumber(String8::format("%s", ecpiNumberStr.c_str()));
            }
        }
        targetCall->updateByCallMode(callMode);
        if (ecpiNumber != NULL && pau != NULL) {
            updateNumberFromEcpiNumOrPau(targetCall, ecpiNumberStr, string(pau));
            updateDisplayNameFromPau(targetCall, string(pau));
        }
    }

    switch (msgType) {
        case 0: {
            if (waitCallRingForMT(msg)) ret = false;
            Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];
            Vector<RfxImsCallInfo*> activeCalls = getActiveCall(calls);
            if (activeCalls.size() > 0) {
                targetCall->setCallState(RfxImsCallInfo::STATE_WAITING);
            } else {
                targetCall->setCallState(RfxImsCallInfo::STATE_INCOMING);
            }
            break;
        }
        case 2:
            targetCall->setCallState(RfxImsCallInfo::STATE_ALERTING);
            if (mPendingImsStkCallInfoMessage != NULL) {
                RfxStringsData* stkCallData =
                        (RfxStringsData*)mPendingImsStkCallInfoMessage->getData();
                char** stkParams = (char**)stkCallData->getData();
                int stkCallId = atoi(stkParams[1]);
                if (stkCallId == callId) {
                    responseToRilj(mPendingImsStkCallInfoMessage);
                    mPendingImsStkCallInfoMessage = NULL;
                }
            }
            break;
        case 6:
            if (callId == mCallIdAnswering) {
                mCallIdAnswering = 0;
            }
            //handle mPendingImsStkCallInfoMessage if no alerting stage in stk call
            if (mPendingImsStkCallInfoMessage != NULL) {
                RfxStringsData* stkCallData =
                        (RfxStringsData*)mPendingImsStkCallInfoMessage->getData();
                char** stkParams = (char**)stkCallData->getData();
                int stkCallId = atoi(stkParams[1]);
                if (stkCallId == callId) {
                    responseToRilj(mPendingImsStkCallInfoMessage);
                    mPendingImsStkCallInfoMessage = NULL;

                    mDelaySendCallInfoMessage = msg;
                    mSendImsCallInfoTimerHandle = RfxTimer::start(RfxCallback0(this,
                            &RtmCallController::onSendDelayCallInfoTimer),
                            ms2ns(DELAY_SEND_CALL_INFO_TIMER));
                    ret = false;
                }
            }
            targetCall->setCallState(RfxImsCallInfo::STATE_ESTABLISHED);

            // in the case we didn't set in EAIC, we set here
            /* For IMS call, RF is ocuppied when call becomes active */
            updateRQ(1);
            break;
        case 130:
            // IMS STK call => +ECPI 130 with call mode = 20
            // ATD => +ECPI: 130 with call mode = 0 and mEstablishingCall != NULL
            // IMS voice/video Conference => +ECPI 130 with call mode = 22/23
            if (callMode == RfxImsCallInfo::IMS_VOICE) {
                handleImsStkCall(slotId, params);
            } else if (mEstablishingCall[slotId] != NULL) {
                assignImsCallId(slotId, callId);

                targetCall = getImsCallByCallId(mImsCalls[slotId], callId);
                if (targetCall != NULL) {
                    targetCall->updateByCallMode(callMode);
                    if (ecpiNumber != NULL && pau != NULL) {
                        updateNumberFromEcpiNumOrPau(targetCall, ecpiNumberStr, string(pau));
                        updateDisplayNameFromPau(targetCall, string(pau));
                    }
                    targetCall->setCallState(RfxImsCallInfo::STATE_DIALING);
                }
            } else {
                generateImsConference(slotId, callId);
            }
            break;
        case 131:
            targetCall->setCallState(RfxImsCallInfo::STATE_HELD);
            if (callId == mCallIdToBeHeld) {
                checkIfResumeBg(slotId);
                // mCallIdToBeHeld = 0;
            }
            break;
        case 132:
            if (callId == mCallIdResuming) {
                mCallIdResuming = 0;
            }
            if (callId == mCallIdAnswering) {
                mCallIdAnswering = 0;
            }
            targetCall->setCallState(RfxImsCallInfo::STATE_ESTABLISHED);
            break;
        case 133: {
            if (mPendingImsStkCallInfoMessage != NULL) {
                RfxStringsData* stkCallData =
                        (RfxStringsData*)mPendingImsStkCallInfoMessage->getData();
                char** stkParams = (char**)stkCallData->getData();
                int stkCallId = atoi(stkParams[1]);
                if (stkCallId == callId) {
                    mPendingImsStkCallInfoMessage = NULL;
                }
            }

            if (mSendImsCallInfoTimerHandle != NULL) {
                RfxTimer::stop(mSendImsCallInfoTimerHandle);
                mSendImsCallInfoTimerHandle = NULL;
            }

            if (mPendingCallControlMessage != NULL) {
                void* params = mPendingCallControlMessage->getData()->getData();
                int asyncControlCallId = ((int*)params)[0];
                if (asyncControlCallId == callId) {
                    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                            RIL_E_ERROR_IMS_CONTROL_CALL_FAILED_CALL_TERMINATED,
                            mPendingCallControlMessage, true);
                    responseToRilj(responseMsg);
                    mPendingCallControlMessage = NULL;
                }
            }
            if (callId == mCallIdResuming) {
                mCallIdResuming = 0;
            }
            targetCall->setCallState(RfxImsCallInfo::STATE_TERMINATED);

            removeImsCall(slotId, targetCall);
            if (callId == getPendingMTCallId()) {
                clearCallRingCache(slotId);
            }

            if (mPendingIncomingCallId == callId) {
                mPendingIncomingCallId = -1;
                resetImsFakeMode();
            }

            // Remove call id from disconnecting list.
            Vector<int>::iterator iter = mCallIdsDisconnecting.begin();
            while (iter != mCallIdsDisconnecting.end()) {
                if (((int)(*iter)) == callId) {
                    iter = mCallIdsDisconnecting.erase(iter);
                } else {
                    ++iter;
                }
            }

            if (callId == mCallIdToBeHangup) {
                checkIfResumeBg(slotId);
                mCallIdToBeHangup = 0;
            }

            // Don't send IMS_RESPONSE_CALL_STATE_CHANGED when this call is merging.
            RfxImsCallInfo* participantCall = getImsCallByCallId(mToBeMergedCalls, callId);
            if (participantCall != NULL) {
                needSendCallStateChange = false;
            }

            if (mAutoAnswerImsIncoming != AUTO_ANSWER_NONE && mAutoAnswerCallId == callId) {
                mAutoAnswerImsIncoming = AUTO_ANSWER_NONE;
                mAutoAnswerCallId = CALL_ID_NONE;
            }
            break;
        }
        default:
            break;
    }

    dumpImsCallList(mImsCalls[slotId]);

    if (needSendCallStateChange) {
        sp<RfxMessage> urcToRilj;
        urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED,
                RfxVoidData());
        responseToRilj(urcToRilj);
    }

    return ret;
}

bool RtmCallController::handleCsCallInfoUpdate(const sp<RfxMessage>& msg) {
    /* +ECPI: <call_id>, <msg_type>, <is_ibt>,
     *         <is_tch>, <dir>, <call_mode>, <number>, <type>, "<pau>", [<cause>] */
    RfxStringsData* data = (RfxStringsData*)msg->getData();
    char** params = (char**)data->getData();
    int callId = atoi(params[0]);
    int msgType = atoi(params[1]);
    int callMode = atoi(params[5]);

    //logD(RFX_LOG_TAG, "csCallInfoUpdate slot: %d, callId: %d, ms: %d, callMode",
    //        slotId, callId, msgType, callMode);

    if (msgType == 133) {
        mUseLocalCallFailCause = false;
    }

    if (msgType == 133 && mPendingSrvccCallCount > 0) {
        mPendingSrvccCallCount = 0;
        logD(RFX_LOG_TAG, "handleCsCallInfoUpdate resetSrvccCallCount");
    }

    if (msgType == 130 && mEccState == STATE_CONNECTING) {
        mEccCallId = callId;
        mEccState = STATE_DIALING;
    } else if (mEccCallId == callId) {
        // Emergency call end
        if (msgType == 133) {
            if (mEspeechOnInDialingEcc == true) {
                mUseLocalCallFailCause = true;
                mDialLastError = CALL_FAIL_NORMAL;
                mEspeechOnInDialingEcc = false;
            }
            maybeSendNoEcbm(false);
            mEccCallId = -1;
            mEccState = STATE_DISCONNECTED;
        } else if (msgType == 2) {
            mEccState = STATE_ALERTING;
        } else if (msgType == 6) {
            mEspeechOnInDialingEcc = false;
            mEccState = STATE_ACTIVE;
            mEccController->onEccActive();
        }
    }


    // Update call exist before poll call done, call end after poll call done
    switch (msgType) {
        case 130: //MO
            mCsCallCount++;
            if (mCsCallCount > 0) {
                /* RF is ocuppied when CS call exists */
                updateRQ(1);
            }
            break;
        case 133:
            if ((callMode >= MIN_CDMA_CALL_MODE && callMode <= MAX_CDMA_CALL_MODE)
                    // call mode could be unknown
                    || isCdmaCallExist()) {
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED, true);
            }

            if (mCsCallCount > 0) {
                mCsCallCount--;
            } else {
                logE(RFX_LOG_TAG, "something wrong with the call count of slot%d", m_slot_id);
            }
            break;
        default:
            break;
    }

    sp<RfxMessage> urcToRilj;
    urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED,
            RfxVoidData());
    responseToRilj(urcToRilj);

    updateCallCount();
    return true;
}

bool RtmCallController::isInChina() {
    RtmEccNumberController* eccNumberController = (RtmEccNumberController *)findController(
            getSlotId(), RFX_OBJ_CLASS_INFO(RtmEccNumberController));
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        String8 plmn = eccNumberController->getPlmn(i);
        char currentMcc[MCC_CHAR_LEN + 1] = {0};
        strncpy(currentMcc, plmn.string(), MCC_CHAR_LEN);
        currentMcc[MCC_CHAR_LEN] = '\0';
        if (strcmp(currentMcc, CHINA_MCC) == 0) {
            return true;
        }
    }
    return false;
}


void RtmCallController::handleSrvcc(int slotId, const sp<RfxMessage>& msg) {
    mSrvccState[slotId] = ((int*)(msg->getData()->getData()))[0];

    if (!hasImsCall(slotId)) {
        logD(RFX_LOG_TAG, "Abort SRVCC, no Ims Call in slotId: %d", slotId);
        mSrvccState[slotId] = SRVCC_STATE_NONE;
        return;
    }

    if (mSrvccState[slotId] == SRVCC_STATE_COMPLETED) {// SRVCC success
        mPendingSrvccCallCount = getValidImsCallCount();
        releaseEstablishingCall(slotId);
        clearAllImsCalls(slotId);
        clearCallRingCache(slotId);
        maybeSendNoEcbm(true);

        logD(RFX_LOG_TAG, "handleSrvcc in slot: %d, mSrvccState: %d, pendingSrvccCallCount: %d",
                slotId, mSrvccState[slotId], mPendingSrvccCallCount);

        mSrvccState[slotId] = SRVCC_STATE_NONE;
    }

    logD(RFX_LOG_TAG, "handleSrvcc in slot: %d, mSrvccState: %d, pendingSrvccCallCount: %d",
            slotId, mSrvccState[slotId], mPendingSrvccCallCount);
}

bool RtmCallController::isDuringSrvcc(int slotId) {
    return (mSrvccState[slotId] == SRVCC_STATE_STARTED || mSrvccState[slotId] == SRVCC_STATE_COMPLETED);
}

bool RtmCallController::waitCallRingForMT(const sp<RfxMessage>& msg) {
    int slotId = msg->getSlotId();
    // Keep ECPI 0, notify until CRING
    if (!mCallRingIndicated[slotId]) {
        logD(RFX_LOG_TAG, "Keep ECPI 0 for slot: %d", slotId);
        mPendingCallInfoForMT[slotId] = msg;
        return true;
    }

    // clear the CRING cache
    clearCallRingCache(slotId);
    mPendingIncomingCallId = -1;

    if (mAutoAnswerImsIncoming == AUTO_ANSWER_FAKE) {
        if (mAutoAnswerVideoMode != -1) {
            int msg_data[2];
            msg_data[0] = mAutoAnswerVideoMode;
            msg_data[1] = mAutoAnswerCallId;
            requestToMcl(RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT,
                    RfxIntsData(msg_data, 2)));
            mAutoAnswerVideoMode = -1;
        } else {
            requestToMcl(RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_ANSWER,
                     RfxVoidData()));
        }
        mAutoAnswerImsIncoming = AUTO_ANSWER_NONE;
        mAutoAnswerCallId = CALL_ID_NONE;
        return true;
    }
    return false;
}

bool RtmCallController::handlePendingMTCallMode(const sp<RfxMessage>& msg) {
    if (mPendingCallInfoForMT[m_slot_id] == NULL) return true;

    RfxStringsData* pendingMTData = (RfxStringsData*)mPendingCallInfoForMT[m_slot_id]->getData();
    RfxStringsData* callModeData = (RfxStringsData*)msg->getData();

    char** pendingMTParams = (char**)pendingMTData->getData();
    char** callModeParams = (char**)callModeData->getData();
    int pendingMTCallId = getPendingMTCallId();
    int pendingMTCallMode = atoi(pendingMTParams[5]);

    int callModeCallId = atoi(callModeParams[0]);
    int targetCallMode = atoi(callModeParams[1]);

    RtmImsConferenceController *imsConfCtrl = (RtmImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtmImsConferenceController));

    if(imsConfCtrl->needSkipDueToFakeDisconnect(callModeCallId)) {
        return false;
    }

    if (pendingMTCallId != callModeCallId) return true;
    if (pendingMTCallMode == targetCallMode) return true;

    logD(RFX_LOG_TAG, "handlePendingMTCallMode slot: %d, orig: %d, target: %d"
            , m_slot_id, pendingMTCallMode, targetCallMode);

    // modify the call mode
    pendingMTParams[5] = strdup(callModeParams[1]);

    int countString = pendingMTData->getDataLength() / sizeof(char *);
    RfxStringsData data(pendingMTParams, countString);

    mPendingCallInfoForMT[m_slot_id] =
            RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_CALL_INFO_INDICATION, data);

    return true;
}

bool RtmCallController::handleImsCallModeChange(const sp<RfxMessage>& msg) {
    /* +EIMSCMODE: <call_id>,<call_mode>,<camera_direction>,<audio_direction>,"<p_assert_uri>",<audio_codec> */

    /* +ECPI: <call_id>, <msg_type>, <is_ibt>,
     *         <is_tch>, <dir>, <call_mode>, <number>, <type>, "<pau>", [<cause>] */
    int slotId = msg->getSlotId();

    RfxStringsData* data = (RfxStringsData*)msg->getData();
    char** params = (char**)data->getData();
    int callId = atoi(params[0]);
    char* pau = params[4];

    RtmImsConferenceController *imsConfCtrl = (RtmImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtmImsConferenceController));

    if(imsConfCtrl->needSkipDueToFakeDisconnect(callId)) {
        return false;
    }

    RfxImsCallInfo* targetCall = getImsCallByCallId(mImsCalls[slotId], callId);
    if (targetCall == NULL) {
        return true;
    }

    int callMode = atoi(params[1]);
    bool changed = targetCall->updateByCallMode(callMode);
    if (pau != NULL) {
        changed |= updateNumberFromPau(targetCall, string(pau));
        changed |= updateDisplayNameFromPau(targetCall, string(pau));
    }

    if (changed) {
        dumpImsCallList(mImsCalls[slotId]);

        sp<RfxMessage> urcToRilj;
        urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED,
                RfxVoidData());
        responseToRilj(urcToRilj);
    }
    return true;
}

void RtmCallController::handleCallRing(int slotId) {
    if (mPendingCallInfoForMT[slotId] != NULL) {
        logD(RFX_LOG_TAG, "Handle Call Ring, notify ECPI 0 for slot: %d", slotId);

        if (mAutoAnswerImsIncoming == AUTO_ANSWER_EM) {
//            mAutoAnswerTimerHandle = RfxTimer::start(RfxCallback0(this,
//                    &RtmCallController::onAutoAnswerTimer), ms2ns(AUTO_ANSWER_DELAY));
            mAutoAnswerImsIncoming = AUTO_ANSWER_NONE;
            mAutoAnswerCallId = CALL_ID_NONE;
        } else if (mAutoAnswerImsIncoming == AUTO_ANSWER_FAKE) {
            if (mAutoAnswerVideoMode != -1) {
                int msg_data[2];
                msg_data[0] = mAutoAnswerVideoMode;
                msg_data[1] = mAutoAnswerCallId;
                //requestToMcl(RfxMessage::obtainRequest(getSlotId(),
                        //RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT, RfxIntsData(msg_data, 2)));
                requestToMcl(RfxMessage::obtainRequest(getSlotId(),
                        RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT, RfxIntsData(msg_data, 2)), false,  ms2ns(3000));
                mAutoAnswerVideoMode = -1;
            } else {
                //requestToMcl(RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_ANSWER,
                        //RfxVoidData()));
                requestToMcl(RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_ANSWER,
                        RfxVoidData()), false, ms2ns(3000));
            }
            mAutoAnswerImsIncoming = AUTO_ANSWER_NONE;
            clearCallRingCache(slotId);
            mAutoAnswerCallId = CALL_ID_NONE;
            mPendingIncomingCallId = -1;
            updateIsImsCallExistToStatusManager(slotId);
            return;
        }

        responseToRilJAndUpdateIsImsCallExist(mPendingCallInfoForMT[slotId]);
        clearCallRingCache(slotId);
        mPendingIncomingCallId = -1;
    } else {
        mCallRingIndicated[slotId] = true;
    }
}

void RtmCallController::addImsCall(int slotId, RfxImsCallInfo* call) {
    logD(RFX_LOG_TAG, "Add ImsCall to slot: %d, callId: %d, isConf:%s, isConfHost:%s",
            slotId, call->getCallId(), call->isConference() ? "True" : "False",
            call->isConferenceHost() ? "True" : "False");

    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];
    calls.push_back(call);
    mImsCalls[slotId] = calls;
    getStatusManager(m_slot_id)->setImsCallInfosValue(RFX_STATUS_KEY_IMS_CALL_LIST, calls);
}

void RtmCallController::removeImsCall(int slotId, RfxImsCallInfo* call) {
    if (mImsCalls[slotId].size() == 0) {
        logD(RFX_LOG_TAG, "Abort remove ImsCall, no Ims Call in slotId: %d", slotId);
        return;
    }
    logD(RFX_LOG_TAG, "Remove ImsCall in slot: %d, callId: %d", slotId, call->getCallId());

    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];
    int removeIdx = -1;
    for(int i = 0; i < (int)calls.size(); ++i) {
        if (calls[i]->getCallId() == call->getCallId()) {
            removeIdx = i;
            break;
        }
    }

    if (removeIdx == -1) {
        logD(RFX_LOG_TAG, "Remove failed in slot: %d, callId: %d", slotId, call->getCallId());
        return;
    }

    calls.erase(calls.begin() + removeIdx);
    mImsCalls[slotId] = calls;

    if ((int)calls.size() == 0) {
        mImsCalls.erase(slotId);
    }

    delete call;

    getStatusManager(m_slot_id)->setImsCallInfosValue(RFX_STATUS_KEY_IMS_CALL_LIST, calls);
}

void RtmCallController::assignImsCallId(int slotId, int callId) {
    RfxImsCallInfo* call = mEstablishingCall[slotId];
    call->setCallId(callId);
    mEstablishingCall.erase(slotId);
    addImsCall(slotId, call);
}

bool RtmCallController::updateNumberFromEcpiNumOrPau(RfxImsCallInfo* targetCall, string ecpiNumber, string pau) {
    bool changed = false;

    logD(RFX_LOG_TAG, "updateNumberFromEcpiNumOrPau() number=%s",
            RfxRilUtils::pii(RFX_LOG_TAG, ecpiNumber.data()));

    // Update number logic:
    // 1. Get number from pau
    // 2. If pau empty, get number from +ECPI number field
    //
    // Ex: +ECPI: 1,0,0,1,1,20,"01",129,"Tom<sip:02@ims.mnc001.mcc214.3gppnetwork.org>, <tel:03>"
    // First get sip number: 02
    // If sip number empty, then get tel number: 03
    // If all pau empty, then get number field of +ECPI: 01
    if (!pau.empty()) {
        changed = updateNumberFromPau(targetCall, pau);

    // Only update by ecpi number when number is empty.
    } else if (targetCall->getNumber().isEmpty() && pau.empty() && !ecpiNumber.empty()) {
        if (targetCall->getNumber() != ecpiNumber.data()) {
            targetCall->setNumber(String8::format("%s", ecpiNumber.data()));
            changed = true;
            logD(RFX_LOG_TAG, "updateNumberFromEcpiNum() number=%s",
                    RfxRilUtils::pii(RFX_LOG_TAG, ecpiNumber.data()));
        }
    }

    return changed;
}

bool RtmCallController::updateNumberFromPau(RfxImsCallInfo* targetCall, string pau) {
    bool changed = false;

    bool updateAddressByPauForMo = getStatusManager()->getString8Value(
            RFX_STATUS_KEY_CARRIER_UPDATE_ADDRESS_BY_PAU_FOR_MO) == "false";
    if (pau.empty() || (!targetCall->isMT() && updateAddressByPauForMo)) {
        return changed;
    }

    // Update number from pau logic:
    // 1. Get sip number of pau
    // 2. If sip number empty, get tel number of pau
    //
    // Ex: +ECPI: 1,0,0,1,1,20,"01",129,"Tom<sip:02@ims.mnc001.mcc214.3gppnetwork.org>, <tel:03>"
    // First get sip number: 02
    // If sip number empty, then get tel number: 03
    string sipNumber = getFieldValueFromPau(pau, PAU_SIP_NUMBER_FIELD);
    string telNumber = getFieldValueFromPau(pau, PAU_NUMBER_FIELD);

    String8 encodedTelNumber = String8::format(
            "%s", RfxRilUtils::pii(RFX_LOG_TAG, telNumber.data()));
    logD(RFX_LOG_TAG, "updateNumberFromPau() sipNumber=%s, telNumber=%s",
            RfxRilUtils::pii(RFX_LOG_TAG, sipNumber.data()), encodedTelNumber.string());

    if (sipNumber.empty() && telNumber.empty()) {
        return changed;
    }

    // use the sip field first, then use the tel field
    char* savePtr = NULL;
    bool updateAddressBySipField =  getStatusManager()->getString8Value(
            RFX_STATUS_KEY_CARRIER_UPDATE_ADDRESS_BY_SIP_FIELD) == "true";
    string number = (!sipNumber.empty() && updateAddressBySipField) ? sipNumber : telNumber;
    /// M: ALPS07471283 Setting the whole number to call when number contains @anonymous.invalid.
    /// When call is merged into conference, we use this number to restore and show private number
    /// on UI. @{
    char* tmpNumber = NULL;
    if (number.find(ANONYMOUS_INVAID) != string::npos) {
        tmpNumber = number.data();
    } else {
        tmpNumber = strtok_r((char *)number.data(), "[;@,]+", &savePtr);
    }
    /// @}

    if (tmpNumber == NULL) {
        return changed;
    }
    number = string(tmpNumber);

    const char* currentNumber = targetCall->getNumber().string();
    if (!number.empty() && currentNumber != NULL &&
        !MtkPhoneNumberUtils::compareLoosely(number, string(currentNumber))) {
        targetCall->setNumber(String8::format("%s", number.data()));
        changed = true;
        logD(RFX_LOG_TAG, "updateNumberFromPau() number=%s",
                RfxRilUtils::pii(RFX_LOG_TAG, number.data()));
    }

    return changed;
}

bool RtmCallController::updateDisplayNameFromPau(RfxImsCallInfo* targetCall, string pau) {
    bool changed = false;

    if (pau.empty()) {
        return changed;
    }

    // Ex: "Tom<sip:1234567@mediatek>"
    // Display name = "Tom"
    string name("");
    for (unsigned int index = 0; index < pau.length(); index++) {
        char aChar = pau.at(index);
        if (aChar == '"') {
            continue;
        }
        if (aChar == '<') {
            break;
        }
        name += aChar;
    }

    logD(RFX_LOG_TAG, "updateDisplayNameFromPau() name=%s",
            RfxRilUtils::pii(RFX_LOG_TAG, name.data()));

    const char* currentName = targetCall->getName().string();
    if (!name.empty() && currentName != NULL &&
        !MtkPhoneNumberUtils::compareLoosely(string(currentName), name)) {
        targetCall->setName(String8::format("%s", name.data()));
        logD(RFX_LOG_TAG, "updateDisplayNameFromPau() changed and set name=%s",
                RfxRilUtils::pii(RFX_LOG_TAG, name.data()));
        changed = true;
    }

    return changed;
}

string RtmCallController::getFieldValueFromPau(string pau, string field) {
    string value("");

    if (pau.empty() || field.empty()) {
        return value;
    }

    if (pau.find(field) == string::npos) {
        return value;
    }

    int startIndex = pau.find(field);
    startIndex += field.length();
    int endIndex = pau.find(PAU_END_FLAG_FIELD, startIndex);
    if (endIndex == string::npos) {
        endIndex = pau.length();
    }
    value = pau.substr(startIndex, endIndex - startIndex);
    return value;
}

void RtmCallController::imsCallEstablishFailed(int slotId) {
    RfxImsCallInfo* call = mEstablishingCall[slotId];
    if (call == NULL) {
        logD(RFX_LOG_TAG, "imsCallEstablishFailed() no establishing call in slot: %d", slotId);
        return;
    }
    mEstablishingCall.erase(slotId);
    delete call;
}

void RtmCallController::updateIsImsCallExistToStatusManager(int slotId) {
    RfxRootController* rootController = RFX_OBJ_GET_INSTANCE(RfxRootController);
    RfxStatusManager* slotStatusMgr  = rootController->getStatusManager(slotId);
    slotStatusMgr->setBoolValue(RFX_STATUS_KEY_IMS_CALL_EXIST, hasImsCall(slotId));
    updateCallCount();
}

void RtmCallController::clearAllImsCalls(int slotId) {
    if (mImsCalls[slotId].size() == 0) {
        logD(RFX_LOG_TAG, "No Ims Call in slot: %d", slotId);
        return;
    }
    logD(RFX_LOG_TAG, "clearAllImsCalls(): %d", slotId);

    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];

    for(int i = 0; i < (int)calls.size(); ++i) {
        RfxImsCallInfo* call = calls[i];
        delete call;
    }

    mImsCalls.erase(slotId);
    getStatusManager(m_slot_id)->setImsCallInfosValue(
            RFX_STATUS_KEY_IMS_CALL_LIST, Vector<RfxImsCallInfo*>());
}

void RtmCallController::releaseEstablishingCall(int slotId) {
    imsCallEstablishFailed(slotId);
}

void RtmCallController::onSendDelayCallInfoTimer() {
    responseToRilJAndUpdateIsImsCallExist(mDelaySendCallInfoMessage);
    mDelaySendCallInfoMessage = NULL;
}

void RtmCallController::responseToRilJAndUpdateIsImsCallExist(const sp<RfxMessage>& msg) {
    bool updateCallBeforeResponse = false;

    if (msg->getId() == RFX_MSG_UNSOL_INCOMING_CALL_INDICATION) {
        updateCallBeforeResponse = true;
    }

    if (updateCallBeforeResponse) {
        logD(RFX_LOG_TAG, "Update IsImsCallExist before response to RilJ");
        updateIsImsCallExistToStatusManager(msg->getSlotId());
        responseToRilj(msg);
    } else {
        responseToRilj(msg);
        updateIsImsCallExistToStatusManager(msg->getSlotId());
    }
}

void RtmCallController::clearCallRingCache(int slotId) {
    logD(RFX_LOG_TAG, "clearCallRingCache() slot: %d", slotId);
    mPendingCallInfoForMT.erase(slotId);
    mCallRingIndicated[slotId] = false;
}

void RtmCallController::generateImsConference(int slotId, int callId) {
    logD(RFX_LOG_TAG, "generateImsConference() slot: %d", slotId);
    RfxImsCallInfo* call = new RfxImsCallInfo(callId, RfxImsCallInfo::STATE_DIALING,
            String8::format("%s", ""), true, true, false, false, false);
    addImsCall(slotId, call);
}
/* IMS Call End */

int RtmCallController::findRingingConnection() {
    int origSize = mPreciseCallStateList->size();
    int ringingId = -1;
    for (int i = 0; i < origSize; i++) {
        RfxPreciseCallState* item = mPreciseCallStateList->itemAt(i);
        if (item->mSlot == getSlotId()
                && (item->mOrigState == ORIG_INCOMING || item->mOrigState == ORIG_WAITING)) {
            ringingId = item->mCallId;
            logD(RFX_LOG_TAG, "hangupRingingConnection found call (callId=%d, ori_state=%d)",
                    ringingId, item->mOrigState);
            break;
        }
    }
    return ringingId;
}

CallType RtmCallController::getConnectionCallType(int callId) {
    int origSize = mPreciseCallStateList->size();
    CallType callType = CALL_TYPE_NONE;
    for (int i = 0; i < origSize; i++) {
        RfxPreciseCallState* item = mPreciseCallStateList->itemAt(i);
        if (item->mSlot == getSlotId()
                && item->mCallId == callId) {
            callType = item->mCallType;
            logD(RFX_LOG_TAG, "getConnectionCallType (callId=%d, call_type=%d)",
                    callId, callType);
            break;
        }
    }
    return callType;
}

bool RtmCallController::hasPendingHangupRequest(bool isForegnd) {
    bool ret = false;
    int origSize = mPreciseCallStateList->size();
    for (int i = 0; i < origSize; i++) {
        RfxPreciseCallState* item = mPreciseCallStateList->itemAt(i);
        if ((isForegnd && item->mOrigState == ORIG_FOREGND_DISCONNECTING) ||
                (!isForegnd && item->mOrigState == ORIG_BACKGND_DISCONNECTING)) {
            int msg_data[1];
            msg_data[0] = item->mCallId;
            sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_FORCE_RELEASE_CALL,
                                 RfxIntsData(msg_data, 1));
            requestToMcl(msg);
            ret = true;
        }
    }
    return ret;
}

bool RtmCallController::hasPendingHangupRequest(int hangupCallId) {
    int origSize = mPreciseCallStateList->size();
    for (int i = 0; i < origSize; i++) {
        RfxPreciseCallState* item = mPreciseCallStateList->itemAt(i);
        if (item->mCallId == hangupCallId && item->mOrigState == ORIG_DISCONNECTING) {
            int msg_data[1];
            msg_data[0] = item->mCallId;
            sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_FORCE_RELEASE_CALL,
                                 RfxIntsData(msg_data, 1));
            requestToMcl(msg);
            return true;
        }
    }

    Vector<RfxImsCallInfo*> calls = mImsCalls[m_slot_id];
    RfxImsCallInfo* hangupCall = getImsCallByCallId(calls, hangupCallId);
    if (hangupCall != NULL && hangupCall->getCallState() == RfxImsCallInfo::STATE_TERMINATING) {
        int msg_data[1];
        msg_data[0] = hangupCallId;
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_FORCE_RELEASE_CALL,
                             RfxIntsData(msg_data, 1));
        requestToMcl(msg);
        return true;
    }
    return false;
}

void RtmCallController::handleGetCurrentCallsResponse(const sp<RfxMessage>& message) {
   // logD(RFX_LOG_TAG, "handleGetCurrentCalls() slot: %d", m_slot_id);

    /* Cache precise call state, previously for MDMI, currently for general purpose */
    int dataLen = message->getData()->getDataLength();
    int count = dataLen / sizeof(RIL_Call *);

    // Update call exist before poll call done, call end after poll call done
    mCsCallCount = count;
    if (mCsCallCount == 0) {
        getStatusManager()->setIntValue(RFX_STATUS_KEY_ECC_PREFERRED_RAT, 0);
    } else {
        logD(RFX_LOG_TAG, "CS Call polled, reset SrvccCallCount");
        mPendingSrvccCallCount = 0;
    }

    RIL_Call ** pp_calls = (RIL_Call **) message->getData()->getData();
    Vector<RfxPreciseCallState*>* currentList = parsePreciseCallState(pp_calls, count);
    updateDisconnected(mPreciseCallStateList, currentList);
    freePreciseCallStateList(mPreciseCallStateList);
    mPreciseCallStateList = currentList;
}

Vector<RfxPreciseCallState*>* RtmCallController::parsePreciseCallState(RIL_Call ** pp_calls, int count) {
    //logD(RFX_LOG_TAG, "parsePreciseCallState, count: %d", count);

    Vector<RfxPreciseCallState*>* list = new Vector<RfxPreciseCallState*>();

    int id, type, dir, mpty;
    RIL_CallState status;
    char* callNumber = NULL;
    int nonMptyHeldCount = 0;
    int mptyHeldCount = 0;

    for (int i = 0; i < count; i++) {
        RIL_Call *p_cur = ((RIL_Call **) pp_calls)[i];
        RfxPreciseCallState* preciseCallState = new RfxPreciseCallState();
        status = p_cur->state;  // state
        id = p_cur->index;  // index
        dir = p_cur->isMT;  // isMT
        type = p_cur->isVoice;  // isVoice
        callNumber = p_cur->number;  // number
        mpty = p_cur->isMpty; // isMultiparty

        /// M: CC: Handle 2 hold calls cases for handover. @{
        // [ALPS03579445][ALPS03104249]
        if (status == RIL_CALL_HOLDING) {
            if (mpty == 0) {
                nonMptyHeldCount++;
            }
            if (mpty == 1) {
                mptyHeldCount++;
            }
        }
        /// @}

        preciseCallState->mSlot = getSlotId();
        preciseCallState->mCallId = id;
        preciseCallState->mOrigState = RfxPreciseCallState::RILStateToOrigState(status);

        if (type == 1) {
            if (mEccCallId == id) {
                preciseCallState->mCallType = CALL_TYPE_EMERGENCY;
            } else {
                preciseCallState->mCallType = CALL_TYPE_VOICE;
            }
        } else {
            preciseCallState->mCallType = CALL_TYPE_VIDEO;
        }

        preciseCallState->mCallRat = mCallRat;
        preciseCallState->mCallDir = (CallDirection)dir;

        updatePreciseCallStateList(preciseCallState, list);
    }

    /// M: CC: Handle 2 hold calls cases for handover. @{
    // [ALPS03579445][ALPS03104249]
    if (nonMptyHeldCount >= 2 || (nonMptyHeldCount != 0 && mptyHeldCount >= 2)) {
        logD(RFX_LOG_TAG, "Hangup all calls due to abnormal held call exists, nonMpty=%d, mpty=%d",
            nonMptyHeldCount, mptyHeldCount);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(
                getSlotId(), RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData());
        requestToMcl(msg);
    }
    /// @}

    return list;
}

void RtmCallController::updateDisconnecting(
        Vector<RfxPreciseCallState*>* origList, int hangupCallId) {
    int origSize = origList->size();
    for (int i = 0; i < origSize; i++) {
        if (origList->itemAt(i)->mCallId == hangupCallId) {
            origList->itemAt(i)->mOrigState = ORIG_DISCONNECTING;
            break;
        }
    }
}

void RtmCallController::updateDisconnecting(
        Vector<RfxPreciseCallState*>* origList, bool isForegnd) {
    int origSize = origList->size();
    for (int i = 0; i < origSize; i++) {
        if (isForegnd) {
            if (origList->itemAt(i)->mOrigState == ORIG_ACTIVE) {
                origList->itemAt(i)->mOrigState = ORIG_FOREGND_DISCONNECTING;
            }
        } else {
            if (origList->itemAt(i)->mOrigState == ORIG_HOLDING ||
                    origList->itemAt(i)->mOrigState == ORIG_WAITING) {
                origList->itemAt(i)->mOrigState = ORIG_BACKGND_DISCONNECTING;
            }
        }
    }
}

void RtmCallController::updateDisconnected(
        Vector<RfxPreciseCallState*>* oldList, Vector<RfxPreciseCallState*>* newList) {
    int oldSize = oldList->size();
    int newSize = newList->size();
    for (int i = 0; i < oldSize; i++) {
        bool disconnected = true;
        for (int j = 0; j < newSize; j++) {
            if (oldList->itemAt(i)->mCallId == newList->itemAt(j)->mCallId) {
                disconnected = false;
                break;
            }
        }
        if (disconnected) {
            RfxPreciseCallState* preciseCallState = oldList->itemAt(i);
            preciseCallState->mOrigState = ORIG_DISCONNECTED;
            updatePreciseCallStateList(preciseCallState, newList);
        }
    }
}

void RtmCallController::freePreciseCallStateList(Vector<RfxPreciseCallState*>* list) {
    if (list != NULL) {
        int size = list->size();
        for (int i = 0; i < size; i++) {
            delete list->itemAt(i);
        }
        delete list;
    }
}

void RtmCallController::updatePreciseCallStateList(
        RfxPreciseCallState* preciseCallState, Vector<RfxPreciseCallState*>* list) {
    preciseCallState->dump();
    if (preciseCallState->mOrigState != ORIG_DISCONNECTED) {
        list->add(preciseCallState);
    }
}

void RtmCallController::responseDialFailed(const sp<RfxMessage>& message) {
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);


    responseToRilj(responseMsg);
}

void RtmCallController::notifyCallInfoUpdate(const sp<RfxMessage>& message) {

    handleImsCallInfoUpdate(message);
    responseToRilJAndUpdateIsImsCallExist(message);
}

RIL_CALL_INFO_TYPE RtmCallController::shouldRejectIncomingCall(int callMode) {
    // int dsdaMode = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_DSDA_MODE, 0);
    int imsRegistrationRat = getStatusManager()->getIntValue(
            RFX_STATUS_KEY_IMS_REGISTRATION_RAT, 0);
    bool isIncomingImsCall = (callMode >= MIN_IMS_CALL_MODE && callMode <= MAX_IMS_CALL_MODE);

    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        // Check other slot.
        if (i != m_slot_id) {
            bool otherSlotCallExist = getStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_VOICE_CALL_COUNT, 0) > 0;
            // Only need to check ongoing call exist slot.
            if (!otherSlotCallExist) continue;

            int otherSlotImsRegistrationRat = getStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_IMS_REGISTRATION_RAT, 0);

            RtmCallController *ctrl = (RtmCallController *) findController(
                    i, RFX_OBJ_CLASS_INFO(RtmCallController));
            bool allowImsIncoming = isIncomingImsCall && ctrl != NULL
                    && ctrl->allowImsIncomingOnAnotherSlot();

            logD(RFX_LOG_TAG, "allowImsIncoming:%d, imsRegistrationRat:%d,"
                    "otherSlotImsRegistrationRat:%d", allowImsIncoming,
                    imsRegistrationRat, otherSlotImsRegistrationRat);

            // By pass for 5G+4G DSDA.
            if ((isDsdaSupport() || isDrdsdaSupport()) && allowImsIncoming) {
                logD(RFX_LOG_TAG, "Allow incoming call from slot%d even if slot%d has alive call",
                            m_slot_id, i);
                continue;
            }

            // By pass for VoWifi incoming call while other slot has call and/or
            // IMS call incoming while other slot has VoWifi call
            if (allowImsIncoming && (imsRegistrationRat == 1 || otherSlotImsRegistrationRat == 1)) {
                continue;
            }

            // By pass for VoData incoming call while other slot has call and/or
            // IMS call incoming while other slot has VoData call
            if (allowImsIncoming && (
                    (imsRegistrationRat <= MAX_VODATA_RAT && imsRegistrationRat >= MIN_VODATA_RAT)
                    || (otherSlotImsRegistrationRat <= MAX_VODATA_RAT &&
                    otherSlotImsRegistrationRat >= MIN_VODATA_RAT))) {
                continue;
            }

            // Reject incoming call for DSDS
            logD(RFX_LOG_TAG, "reject MT on slot%d, since slot%d has call", m_slot_id, i);
            return MT_CALL_REJECTED;
        // Check self slot.
        } else {
            if (hasImsCall(m_slot_id)) {
                //Reject incoming for IMS if ECC exists
                Vector<RfxImsCallInfo*> calls = mImsCalls[m_slot_id];
                for (int i = 0; i < (int)calls.size(); ++i) {
                    RfxImsCallInfo* call = calls[i];
                    if (call->isEcc()) {
                        logD(RFX_LOG_TAG, "Reject MT on slot%d, since there is an IMS ECC",
                                m_slot_id);
                        return MT_CALL_REJECTED;
                    }
                }
            }
        }
    }

    // GWSD
    if (isIncomingImsCall) {
        int gwsdMode = getStatusManager()->getIntValue(RFX_STATUS_KEY_GWSD_MODE, 0);
        logD(RFX_LOG_TAG, "shouldRejectIncomingCall(): gwsdMode is %d", gwsdMode);
        if (gwsdMode == 2) { // silently reject.
            return MT_CALL_MISSED;
        } else if (gwsdMode == 1 && mImsCallRat == CALL_RAT_NR) {
            // 5G GWSD case and let user decide to avoid EPSFB
            // getStatusManager()->setBoolValue(RFX_STATUS_KEY_IMS_CC_FAKE_MODE, true);
        }
    }

    return MT_CALL_NONE;
}

void RtmCallController::approveIncoming(int callId, int seqNo, bool approve, int
            cause, const sp<RfxMessage>& message) {
    // AT+EAIC=X, callId, seqNo
    std::map<int, int>::iterator iter;
    int msg_data[5], len = -1;
    sp<RfxMessage> msg;
    msg_data[0] = approve ? 0 : 1;  // 0: approve, 1: disapprove
    msg_data[1] = callId;
    msg_data[2] = seqNo;
    msg_data[3] = cause;
    iter = mApproveIncomingCallTxIds.find(seqNo);
    if (iter != mApproveIncomingCallTxIds.end()) {
        msg_data[4] = mApproveIncomingCallTxIds[seqNo];
        len = 5;
    } else {
        len = 4;
    }
    if (message == NULL) {
        msg = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_SET_CALL_INDICATION,
                RfxIntsData(msg_data, len));
    } else {
        msg = RfxMessage::obtainRequest(
                RFX_MSG_REQUEST_SET_CALL_INDICATION,
                RfxIntsData(msg_data, len),
                message, false);
    }
    requestToMcl(msg);
    mApproveIncomingCallTxIds.erase(seqNo);
}

void RtmCallController::onAutoApproveTimer() {
    logD(RFX_LOG_TAG, "onAutoApproveTimer callId %d, SeqNo %d", mPendingIncomingCallId, mSeqNo);
    if (mPendingIncomingCallId == -1 || mSeqNo == -1) {
        return;
    }
    // AT+EAIC=X, callId, seqNo
    std::map<int, int>::iterator iter;
    int msg_data[5], len = -1;
    msg_data[0] = 0;
    msg_data[1] = mPendingIncomingCallId;
    msg_data[2] = mSeqNo;
    msg_data[3] = -1;

    iter = mApproveIncomingCallTxIds.find(mSeqNo);
    if (iter != mApproveIncomingCallTxIds.end()) {
        msg_data[4] = mApproveIncomingCallTxIds[mSeqNo];
        len = 5;
    } else {
        len = 4;
    }

    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_SET_CALL_INDICATION,
            RfxIntsData(msg_data, len));
    requestToMcl(msg);
    mApproveIncomingCallTxIds.erase(mSeqNo);
    mSeqNo = -1;
    resetImsFakeMode();
}

void RtmCallController::updateCallCount() {
    int callCount = getValidImsCallCount() + mCsCallCount; // PS + CS call count
    if (mCsCallCount == 0) {
        callCount += mPendingSrvccCallCount;
    }

    bool inCall = callCount > 0;
    if (inCall != getStatusManager()->getBoolValue(RFX_STATUS_KEY_IN_CALL, false)) {
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, inCall);
    }

    if (callCount != getStatusManager()->getIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, 0)) {
        getStatusManager()->setIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, callCount);
    }
    if (callCount != getStatusManager()->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0)) {
        getStatusManager()->setIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, callCount);
    }

    if (callCount == 0) {
        int status = getStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_INIT);
        if (status == BT_SAP_CONNECTION_SETUP) {
            sp<RfxMessage> msg = RfxMessage::obtainRequest(
                    getSlotId(), RFX_MSG_REQUEST_LOCAL_SIM_SAP_RESET, RfxVoidData());
            requestToMcl(msg);
        }
        /* reset RQ when no call exists */
        updateRQ(0);
    }
}

int RtmCallController::getValidImsCallCount() {
    int validImsCallCount = mImsCalls[m_slot_id].size();
    if (mEstablishingCall[m_slot_id] != NULL) {
        ++validImsCallCount;
    }
    return validImsCallCount;
}

int RtmCallController::getPendingMTCallId() {
    int callId = -1;
    if (mPendingCallInfoForMT[m_slot_id] != NULL) {
        RfxStringsData* data = (RfxStringsData*)mPendingCallInfoForMT[m_slot_id]->getData();
        char** params = (char**)data->getData();
        callId = atoi(params[0]);
    }
    logD(RFX_LOG_TAG, "getPendingMTCallId: %d", callId);
    return callId;
}

// ALPS04027132, IMS MT call & ECC call conflict.
// Better hang up the incoming call.
void RtmCallController::hangupIncomingCall() {
    if (mPendingIncomingCallId != -1) {
        logD(RFX_LOG_TAG,"[%s], hang up incoming call due to ECC dial", __FUNCTION__);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(
                getSlotId(), RFX_MSG_REQUEST_HANGUP, RfxIntsData(&mPendingIncomingCallId, 1));
        requestToMcl(msg);
    }
}

void RtmCallController::hangupIncomingCallOnAllSlot() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++){
        RtmCallController *ctrl =
                (RtmCallController *) findController(i, RFX_OBJ_CLASS_INFO(RtmCallController));
        if (ctrl != NULL) {
            ctrl->hangupIncomingCall();
        }
    }
}

bool RtmCallController::handleSetCallIndicationRequest(const sp<RfxMessage>& message) {
    void* params = message->getData()->getData();
    int disapprove = ((int*)params)[0];
    int callId = ((int*)params)[1];
    int seqNo = ((int*)params)[2];

    // handle the case MT is coming but RQ is still false
    if (mRQ == 0) {
        if (RfxGwsdUtils::getCallControlHandler() != NULL) {
            logD(RFX_LOG_TAG, "handleSetCallIndicationRequest notify gwsd ims call : %d", callId);
            RfxGwsdUtils::getCallControlHandler()->notifyDualSimGwsdImsIncomingCall(this, callId);
        }
        // Don't report fake ringing call for this case.
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_IMS_CC_FAKE_MODE, false);
        return true;
    }

    if (disapprove == 0) {
        return reportFakeImsRingingCall(message, seqNo);
    }
    return true;
}

bool RtmCallController::reportFakeImsRingingCall(const sp<RfxMessage>& msg, int seqNo) {
     if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_IMS_CC_FAKE_MODE)) {
        mImsFakeMode = true;
        // Report fake resposne.
        sp<RfxMessage> response =
                RfxMessage::obtainResponse(RIL_E_SUCCESS, msg, false);
        responseToRilj(response);

        // +ECPI:<call_id>, <msg_type>, <is_ibt>, <is_tch>, <dir>, <call_mode>[, <number>, <toa>], ""
        Vector<RfxImsCallInfo*> calls = mImsCalls[m_slot_id];
        RfxImsCallInfo* incomingCall = NULL;
        for (int i = 0; i < (int)calls.size(); i++) {
            RfxImsCallInfo* call = calls[i];
            if (call->getCallState() == RfxImsCallInfo::STATE_ESTABLISHING) {
                incomingCall = call;
                break;
            }
        }

        // Report fake IMS ringing call.
        if (incomingCall != NULL) {
            int callId = incomingCall->getCallId();
            const char* number = incomingCall->getNumber().string();
            int callMode = incomingCall->getCallMode();
            char *data[10] = { 0 };
            int ret = 0;

            data[0] = (char *) alloca(3);
            ret = sprintf(data[0], "%d", callId);
            if (ret < 0) return true;

            data[1] = (char *) alloca(2);
            ret = sprintf(data[1], "%d", 0);
            if (ret < 0) return true;

            data[2] = (char *) alloca(2);
            ret = sprintf(data[2], "%d", 0);
            if (ret < 0) return true;

            data[3] = (char *) alloca(2);
            ret = sprintf(data[3], "%d", 1);
            if (ret < 0) return true;

            data[4] = (char *) alloca(2);
            ret = sprintf(data[4], "%d", 1);
            if (ret < 0) return true;

            data[5] = (char *) alloca(3);
            ret = sprintf(data[5], "%d", callMode);
            if (ret < 0) return true;

            if (number != NULL) {
                data[6] = strdup(number);
            } else {
                data[6] = (char *) alloca(1);
                ret = sprintf(data[6], "");
                if (ret < 0) return true;
            }

            data[7] = (char *) alloca(4);
            ret = sprintf(data[7], "%d", 129);
            if (ret < 0) return true;

            data[8] = (char *) alloca(1);
            ret = sprintf(data[8], "");
            if (ret < 0) return true;

            data[9] = NULL;

            if (RfxGwsdUtils::getCallControlHandler() != NULL) {
                RfxGwsdUtils::getCallControlHandler()->notifyGwsdImsIncomingCall(this, callId);
            }

            logD(RFX_LOG_TAG, "notifyCallIncoming callId: %d", callId);
            sp<RfxMessage> urc = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_CALL_INFO_INDICATION,
                    RfxStringsData(data, 10));
            responseToRilj(urc);
            mSeqNo = seqNo;
            free(data[6]);
            return false;
        }
    }
    return true;
}

void RtmCallController::maybeSendNoEcbm(bool isForce) {
    bool emergencyMode = getStatusManager()->getBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE, false);
    if (emergencyMode == false || isForce) {
        sp<RfxMessage> urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_NO_EMERGENCY_CALLBACK_MODE,
            RfxVoidData());
        responseToRilj(urcToRilj);
    }
}

void RtmCallController::handleEmergencyBearerSupportNotify(const sp<RfxMessage>& message) {
    // +CNEMS1: <s1_support>
    int eccBearerSupport = ((int *)message->getData()->getData())[0];
    logD(RFX_LOG_TAG, "[%s], eccBearerSupport=%d", __FUNCTION__, eccBearerSupport);
    getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_EMERGENCY_BEARER_SUPPORT, eccBearerSupport);
}

void RtmCallController::handleImsGetCurrentCalls(const sp<RfxMessage>& message) {
    int slotId = message->getSlotId();
    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];
    int countSipCalls = (int)calls.size();
    int countAllCalls = 0;

    for (int i = 0; i < countSipCalls; i++) {
        RfxImsCallInfo* call = calls[i];
        Vector<RfxImsCallInfo*> participantList = call->getParticipantList();
        countAllCalls += (int)participantList.size();
    }
    countAllCalls += countSipCalls;

    logD(RFX_LOG_TAG, "handleImsGetCurrentCalls: countSipCalls %d, countVirtualCalls %d",
            countSipCalls, countAllCalls);

    RIL_Call ** pp_calls = (RIL_Call **)alloca(countAllCalls * sizeof(RIL_Call *));
    RFX_ASSERT(pp_calls != NULL);
    RIL_Call * p_calls = (RIL_Call *)alloca(countAllCalls * sizeof(RIL_Call));
    RFX_ASSERT(p_calls != NULL);
    memset(p_calls, 0, countAllCalls * sizeof(RIL_Call));

    if (countSipCalls == 0) {
        mSpeechCodec = 0;
        // RmmCallControlUrcHandler::resetSpeechCodec();
    }

    /* init the pointer array */
    for (int i = 0; i < countAllCalls; i++) {
        pp_calls[i] = &(p_calls[i]);
    }

    int index = 0;
    for (int i = 0; i < countSipCalls; ++i) {
        imsCallToRilCall(index, calls[i], p_calls);
        index++;
    }

    sp<RfxMessage> response = RfxMessage::obtainResponse(slotId, RFX_MSG_REQUEST_IMS_GET_CURRENT_CALLS,
            RIL_E_SUCCESS, RfxCallListData(pp_calls, sizeof(RIL_Call *) * countAllCalls), message);
    responseToRilj(response);
}

void RtmCallController::imsCallToRilCall(int &index, RfxImsCallInfo* call, RIL_Call* p_calls) {
    string number;
    string name;
    const char* temp = call->getNumber().string();
    if (temp != NULL) {
        number = temp;
    }
    temp = call->getName().string();
    if (temp != NULL) {
        name = temp;
    }
    Vector<RfxImsCallInfo*> participantList = call->getParticipantList();
    p_calls[index].state = call->getRilCallState();
    p_calls[index].index = call->getCallId();
    p_calls[index].toa = number.find("+") != string::npos;
    p_calls[index].isMpty = call->isConferenceHost() ? 1 : 0;
    p_calls[index].isMT = call->isMT() ? 1 : 0;
    p_calls[index].isVoice = call->isVideo() ? 0 : 1;
    p_calls[index].isVoicePrivacy = 0;
    p_calls[index].number = strdup(number.c_str());
    p_calls[index].numberPresentation = (number == "");
    p_calls[index].name = strdup(name.c_str());
    p_calls[index].namePresentation = (name == "");
    p_calls[index].uusInfo = NULL;
    p_calls[index].speechCodec = mSpeechCodec;  // RmmCallControlUrcHandler::getSpeechCodec();

    String8 encodedName = String8::format("%s",
            RfxRilUtils::pii(RFX_LOG_TAG, p_calls[index].name));
    logV(RFX_LOG_TAG, "imsCallToRilCall state %d, index %d, toa %d, isMpty %d, isMT %d, "
        "isVoice %d, isVoicePrivacy %d, number %s, numberPresentation %d, name %s,"
        "namePresentation %d, speechCodec %d", p_calls[index].state, p_calls[index].index,
        p_calls[index].toa, p_calls[index].isMpty, p_calls[index].isMT, p_calls[index].isVoice,
        p_calls[index].isVoicePrivacy, RfxRilUtils::pii(RFX_LOG_TAG, p_calls[index].number),
        p_calls[index].numberPresentation, encodedName.string(),
        p_calls[index].namePresentation, p_calls[index].speechCodec);

    for (int j = 0; j < (int)participantList.size(); j++) {
        index++;
        imsCallToRilCall(index, participantList[j], p_calls);
    }
}

void RtmCallController::handleImsHangupFgResumeBg(const sp<RfxMessage>& message) {
    int slotId = message->getSlotId();
    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];

    // Try to hang up foreground call. (1A, 1A1H, 1A1W, [1A1H1W])
    Vector<RfxImsCallInfo*> activeCalls = getActiveCall(calls);
    if (activeCalls.size() != 0) {
        int callId = activeCalls[0]->getCallId();
        int msg_data[1];
        msg_data[0] = callId;

        logD(RFX_LOG_TAG, "handleImsHangupFgResumeBg hangup call %d", callId);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_HANGUP,
                             RfxIntsData(msg_data, 1));
        requestToMcl(msg);
        activeCalls[0]->setCallState(RfxImsCallInfo::STATE_TERMINATING);
        mHangupFgResumeBgMessage = message;

        // Get candidate call need to be resume/answer.
        Vector<RfxImsCallInfo*> waitingCalls = getWaitingCall(calls);
        Vector<RfxImsCallInfo*> holdingCalls = getHoldingCall(calls);
        if (waitingCalls.size() != 0) {
            // 1A1W case
            mCallIdExpectToResumeOrAnswer = waitingCalls[0]->getCallId();
            mNeedResumeOrAnswerCall = true;
            mCallIdToBeHangup = callId;
        } else if (holdingCalls.size() != 0) {
            // 1A1H case
            mCallIdExpectToResumeOrAnswer = holdingCalls[0]->getCallId();
            mNeedResumeOrAnswerCall = true;
            mCallIdToBeHangup = callId;
        } else {
            // 1A case
            // return success when hangup success.
            logD(RFX_LOG_TAG, "handleImsHangupFgResumeBg no call need to resume or answer");
        }
    } else {
        // Return error if no any foregruond call. (1H, 1W, 1H1W)
        logE(RFX_LOG_TAG, "handleImsHangupFgResumeBg no active call");
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message,
                true);
        responseToRilj(responseMsg);
    }
}

void RtmCallController::checkIfResumeBg(int slotId) {
    // No any combo operation ongoing, just return.
    if (!mNeedResumeOrAnswerCall || mCallIdExpectToResumeOrAnswer == 0) {
        return;
    }

    RIL_Errno ret = RIL_E_SUCCESS;
    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];

    Vector<RfxImsCallInfo*> activeCalls = getActiveCall(calls);
    if (activeCalls.size() != 0) {
        int callId = activeCalls[0]->getCallId();
        if (callId != mCallIdExpectToResumeOrAnswer) {
            logE(RFX_LOG_TAG, "checkIfResumeBg active call %d exist, can't resume background call",
                    callId);
            ret = RIL_E_GENERIC_FAILURE;
        } else {
            logW(RFX_LOG_TAG, "checkIfResumeBg active call %d == mCallIdExpectToResumeOrAnswer"
                ", regards it as success.", callId);
        }
    } else {
        RfxImsCallInfo* call = getImsCallByCallId(calls, mCallIdExpectToResumeOrAnswer);
        if (call != NULL) {
            if (call->getCallState() == RfxImsCallInfo::STATE_INCOMING ||
                    call->getCallState() == RfxImsCallInfo::STATE_WAITING) {
                logD(RFX_LOG_TAG, "checkIfResumeBg answer waiting call %d", mCallIdExpectToResumeOrAnswer);
                sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_ANSWER,
                        RfxVoidData());
                requestToMcl(msg);
                mCallIdAnswering = mCallIdExpectToResumeOrAnswer;
                mCallIdExpectToResumeOrAnswer = 0;
                mNeedResumeOrAnswerCall = false;
                // Response the result when receive response of answer.
                return;
            } else if (call->getCallState() == RfxImsCallInfo::STATE_HELD) {
                int msg_data[1];
                msg_data[0] = mCallIdExpectToResumeOrAnswer;
                logD(RFX_LOG_TAG, "checkIfResumeBg resume background call %d", mCallIdExpectToResumeOrAnswer);
                sp<RfxMessage> msg =
                        RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_RESUME_CALL, RfxIntsData(msg_data, 1));
                requestToMcl(msg);
                mCallIdResuming = mCallIdExpectToResumeOrAnswer;
                mCallIdExpectToResumeOrAnswer = 0;
                mNeedResumeOrAnswerCall = false;
                // Response the result when receive response of resume.
                return;
            } else {
                // We expect no need to deal this case.
                logE(RFX_LOG_TAG, "checkIfResumeBg call %d, state %d, return error",
                        mCallIdExpectToResumeOrAnswer, call->getCallState());
                ret = RIL_E_GENERIC_FAILURE;
            }
        } else {
            logE(RFX_LOG_TAG, "checkIfResumeBg call %d not exist, return error",
                    mCallIdExpectToResumeOrAnswer);
            ret = RIL_E_GENERIC_FAILURE;
        }
    }

    // Return the result
    sp<RfxMessage> responseMsg;
    if (mHangupFgResumeBgMessage != NULL) {
        logD(RFX_LOG_TAG, "checkIfResumeBg hangupFgResumeBg Done");
        responseMsg = RfxMessage::obtainResponse(ret, mHangupFgResumeBgMessage, false);
        responseToRilj(responseMsg);
        mHangupFgResumeBgMessage = NULL;
    } else if (mSwitchFgBgMessage != NULL) {
        logD(RFX_LOG_TAG, "checkIfResumeBg SwitchFgBg Done");
        responseMsg = RfxMessage::obtainResponse(ret, mSwitchFgBgMessage, false);
        responseToRilj(responseMsg);
        mSwitchFgBgMessage = NULL;

        if (mCallIdToBeHeld != 0) {
            if (ret != RIL_E_SUCCESS) {
                int msg_data[1];
                msg_data[0] = mCallIdToBeHeld;
                logE(RFX_LOG_TAG, "Resume or answer failed, swap back %d", mCallIdToBeHeld);
                sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_RESUME_CALL,
                        RfxIntsData(msg_data, 1));
                requestToMcl(msg);
                mCallIdResuming = mCallIdToBeHeld;
            }
            mCallIdToBeHeld = 0;
        }
    }

    mCallIdExpectToResumeOrAnswer = 0;
    mNeedResumeOrAnswerCall = false;
}

void RtmCallController::handleImsHangupWaitingOrBackground(const sp<RfxMessage>& message) {
    int slotId = message->getSlotId();
    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];

    // Try to hang up waiting call. (1W, 1A1W, 1H1W, 1A1H1W)
    Vector<RfxImsCallInfo*> waitingCalls = getWaitingCall(calls);
    if (waitingCalls.size() != 0) {
        int callId = waitingCalls[0]->getCallId();
        int msg_data[1];
        msg_data[0] = callId;

        logD(RFX_LOG_TAG, "handleImsHangupWaitingOrBackground hangup waiting call %d", callId);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_HANGUP,
                RfxIntsData(msg_data, 1));
        requestToMcl(msg);
        waitingCalls[0]->setCallState(RfxImsCallInfo::STATE_TERMINATING);
        mHangupWaitingOrBgMessage = message;
        return;
    }
    // If no waiting call, hang up background call. (1H, 1A1H)
    Vector<RfxImsCallInfo*> holdingCalls = getHoldingCall(calls);
    if (holdingCalls.size() != 0) {
        int callId = holdingCalls[0]->getCallId();
        int msg_data[1];
        msg_data[0] = callId;

        logD(RFX_LOG_TAG, "handleImsHangupWaitingOrBackground hangup background call %d", callId);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_HANGUP,
                             RfxIntsData(msg_data, 1));
        requestToMcl(msg);
        holdingCalls[0]->setCallState(RfxImsCallInfo::STATE_TERMINATING);
        mHangupWaitingOrBgMessage = message;
        return;
    }

    // Return error if no any waiting or background call. (1A or no call)
    logE(RFX_LOG_TAG, "handleImsHangupWaitingOrBackground no any waiting or background call.");
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message,
            true);
    responseToRilj(responseMsg);
}

void RtmCallController::handleImsSwitchFgBg(const sp<RfxMessage>& message) {
    int slotId = message->getSlotId();
    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];

    Vector<RfxImsCallInfo*> activeCalls = getActiveCall(calls);
    Vector<RfxImsCallInfo*> waitingCalls = getWaitingCall(calls);
    Vector<RfxImsCallInfo*> holdingCalls = getHoldingCall(calls);

    int activeCallSize = activeCalls.size();
    int waitingCallSize = waitingCalls.size();
    int holdingCallSize = holdingCalls.size();

    logD(RFX_LOG_TAG, "handleImsSwitchFgBg Active:%d, Holding:%d, Waiting:%d",
            activeCallSize, waitingCallSize, holdingCallSize);

    // Return error when 1A1H1W.
    if (activeCallSize > 0 && waitingCallSize > 0 && holdingCallSize > 0) {
        logE(RFX_LOG_TAG, "handleImsSwitchFgBg 1A1H1W");
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message,
                true);
        responseToRilj(responseMsg);
        return;
    }

    // Try to hold the foreground call. (1A, 1A1H, 1A1W)
    if (activeCallSize > 0) {
        int callId = activeCalls[0]->getCallId();
        int msg_data[1];
        msg_data[0] = callId;
        logD(RFX_LOG_TAG, "handleImsSwitchFgBg hold foreground call %d", callId);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_HOLD_CALL,
                             RfxIntsData(msg_data, 1));
        requestToMcl(msg);
        mSwitchFgBgMessage = message;

        if (waitingCallSize > 0) {
            // 1A1W case.
            mCallIdExpectToResumeOrAnswer = waitingCalls[0]->getCallId();
            mNeedResumeOrAnswerCall = true;
            mCallIdToBeHeld = callId;
        } else if (holdingCallSize > 0) {
            // 1A1H case.
            mCallIdExpectToResumeOrAnswer = holdingCalls[0]->getCallId();
            mNeedResumeOrAnswerCall = true;
            mCallIdToBeHeld = callId;
        }
        return;
    }

    // If there is a waiting call, answer it. (1W, 1H1W)
    if (waitingCallSize > 0) {
        int callId = waitingCalls[0]->getCallId();
        logD(RFX_LOG_TAG, "handleImsSwitchFgBg answer waiting call %d", callId);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_ANSWER,
                RfxVoidData());
        mCallIdAnswering = callId;
        requestToMcl(msg);
        mSwitchFgBgMessage = message;
        return;
    }

    // If there is a background call, resume it. (1H)
    if (holdingCallSize != 0) {
        int callId = holdingCalls[0]->getCallId();
        int msg_data[1];
        msg_data[0] = callId;

        logD(RFX_LOG_TAG, "checkIfResumeBg resume background call %d", callId);
        sp<RfxMessage> request =
                RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_RESUME_CALL, RfxIntsData(msg_data, 1));
        requestToMcl(request);
        mCallIdResuming = callId;
        mSwitchFgBgMessage = message;
        return;
    }

    // Return error if no any IMS call.
    logE(RFX_LOG_TAG, "handleImsSwitchFgBg No any IMS call");
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message,
            true);
    responseToRilj(responseMsg);
}

Vector<RfxImsCallInfo*> RtmCallController::getWaitingCall(Vector<RfxImsCallInfo*> calls) {
    Vector<RfxImsCallInfo*> waitingCalls;
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallState() == RfxImsCallInfo::STATE_INCOMING ||
                call->getCallState() == RfxImsCallInfo::STATE_WAITING) {
            waitingCalls.push_back(call);
        }
    }
    return waitingCalls;
}

Vector<RfxImsCallInfo*> RtmCallController::getHoldingCall(Vector<RfxImsCallInfo*> calls) {
    Vector<RfxImsCallInfo*> holdingCalls;
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallState() == RfxImsCallInfo::STATE_HELD) {
            holdingCalls.push_back(call);
        }
    }
    return holdingCalls;
}

Vector<RfxImsCallInfo*> RtmCallController::getActiveCall(Vector<RfxImsCallInfo*> calls) {
    Vector<RfxImsCallInfo*> activeCalls;
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallState() == RfxImsCallInfo::STATE_ESTABLISHED) {
            activeCalls.push_back(call);
        }
    }
    return activeCalls;
}

RfxImsCallInfo* RtmCallController::getImsCallByCallId(Vector<RfxImsCallInfo*> calls, int callId) {
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallId() == callId) {
            return call;
        }
    }
    return NULL;
}

void RtmCallController::dumpImsCallList(Vector<RfxImsCallInfo*> calls) {
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        Vector<RfxImsCallInfo*> participantList = call->getParticipantList();
        logV(RFX_LOG_TAG, "dumpImsCallList %s", call->toString().string());
        for (int j = 0; j < (int)participantList.size(); j++) {
            RfxImsCallInfo* participant = participantList[j];
            logV(RFX_LOG_TAG, "dumpImsCallList participant %s", participant->toString().string());
        }
    }
    for (int i = 0; i < (int)mToBeMergedCalls.size(); i++) {
        RfxImsCallInfo* call = mToBeMergedCalls[i];
        logV(RFX_LOG_TAG, "dumpImsCallList mToBeMergedCalls %s", call->toString().string());
    }
    return;
}

Vector<RfxImsCallInfo*> RtmCallController::getConferenceHostCall(Vector<RfxImsCallInfo*> calls) {
    Vector<RfxImsCallInfo*> conferenceHostCalls;
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->isConferenceHost()) {
            conferenceHostCalls.push_back(call);
        }
    }
    return conferenceHostCalls;
}

void RtmCallController::handleEconfResult(const sp<RfxMessage>& message) {
    int slotId = message->getSlotId();
    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];
    // +ECONF:<conf_call_id>,<op>,<num>,<result>,<cause>[,<joined_call_id>]
    RfxStringsData* data = (RfxStringsData*)message->getData();
    char** params = (char**)data->getData();
    int result = atoi(params[3]);
    int confId = atoi(params[0]);
    int op = atoi(params[1]);
    int joinedCallId = atoi(params[5]);

    logD(RFX_LOG_TAG, "handleEconfResult op %d, result %d, confId %d, joindCallId %d",
            op, result, confId, joinedCallId);
    // <operation>: 0 = ADD; 1 = REMOVE
    if (op == 0) {
        // <result>: 0 = success; 1 = failed
        if (result == 0) {
            RfxImsCallInfo* confCall = getImsCallByCallId(calls, confId);
            // Add participant into conference.
            if (confCall != NULL) {
                RfxImsCallInfo* participantCall = getImsCallByCallId(mToBeMergedCalls, joinedCallId);
                if (participantCall != NULL) {
                    int newCallId = confId * CONF_CALL_ID_SHIFT + joinedCallId;
                    participantCall->setCallId(newCallId);
                    participantCall->setCallState(confCall->getCallState());
                    participantCall->setIsConference(true);
                    participantCall->setIsConferenceHost(true);
                    confCall->addParticipant(participantCall);
                    // When 133 later than +ECONF.
                    RfxImsCallInfo* OriginalParticipantCall = getImsCallByCallId(calls, joinedCallId);
                    if (OriginalParticipantCall != NULL) {
                        mCallIdsDisconnecting.push_back(joinedCallId);
                    }
                } else {
                    logE(RFX_LOG_TAG, "handleEconfResult can't find participantCall in mToBeMergedCalls");
                }
            }
        }
        // Remove it from ToBeMergedCalls no matter the result
        Vector<RfxImsCallInfo*>::iterator iter = mToBeMergedCalls.begin();
        while (iter != NULL && iter != mToBeMergedCalls.end()) {
            if ((((RfxImsCallInfo*)(*iter))->getCallId() % CONF_CALL_ID_SHIFT)
                    == joinedCallId) {
                iter = mToBeMergedCalls.erase(iter);
                // No need to delete here.
            } else {
                ++iter;
            }
        }
    }
    dumpImsCallList(calls);
}

void RtmCallController::onMergeStart() {
    logD(RFX_LOG_TAG, "onMergeStart");
    Vector<RfxImsCallInfo*> calls = mImsCalls[m_slot_id];
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->isConferenceHost() == false) {
            RfxImsCallInfo* newCall = new RfxImsCallInfo(call);
            mToBeMergedCalls.push_back(newCall);
        }
    }
    dumpImsCallList(calls);
}

void RtmCallController::handleImsStkCall(int slotId, char** params) {
    int callId = atoi(params[0]);
    char* number = params[6];

    logD(RFX_LOG_TAG, "handleImsStkCall() slot: %d callId: %d", slotId, callId);

    RfxImsCallInfo* call = new RfxImsCallInfo(callId, RfxImsCallInfo::STATE_DIALING,
            String8::format("%s", number), false, false, false, false, false);
    addImsCall(slotId, call);

    char **urcData = (char **) calloc(10, sizeof(char *));
    RFX_ASSERT(urcData != NULL);
    asprintf(&urcData[0], "%d", MT_CALL_DIAL_IMS_STK);
    for (int i = 0; i < 9; i++) {
        urcData[i+1] = strdup(params[i]);
    }
    // AOSP didn't handle IMS unknown call well, so we need to cache the urc
    // until it sucessfully dial out.
    mPendingImsStkCallInfoMessage = RfxMessage::obtainUrc(m_slot_id,
            RFX_MSG_UNSOL_CALL_ADDITIONAL_INFO, RfxStringsData(urcData, 10));

    for (int i = 0; i < 10; i++) {
        if (urcData[i] != NULL) {
            free(urcData[i]);
        }
    }
    free(urcData);
}

// To fit UX design, return false when there is any dialing/alerting MO IMS call.
bool RtmCallController::allowImsIncomingOnAnotherSlot() {
    if (mEstablishingCall[m_slot_id] != NULL) {
        return false;
    }

    Vector<RfxImsCallInfo*> calls = mImsCalls[m_slot_id];
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallState() == RfxImsCallInfo::STATE_DIALING ||
                call->getCallState() == RfxImsCallInfo::STATE_ALERTING) {
            return false;
        }
    }
    return true;
}

void RtmCallController::retrieveMergeFail(int activeCallIdBeforeMerge) {
    Vector<RfxImsCallInfo*> calls = mImsCalls[m_slot_id];
    Vector<RfxImsCallInfo*> activeCalls = getActiveCall(calls);
    Vector<RfxImsCallInfo*> holdingCalls = getHoldingCall(calls);

    if (holdingCalls.size() > 1 && activeCalls.size() == 0) {
        int msg_data[1];
        msg_data[0] = activeCallIdBeforeMerge;
        logD(RFX_LOG_TAG, "retrieveMergeFail() : two hold call and resume merge call %d", activeCallIdBeforeMerge);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_RESUME_CALL,
                RfxIntsData(msg_data, 1));
        requestToMcl(msg);
        mCallIdResuming = activeCallIdBeforeMerge;
    }
}

bool RtmCallController::handleClirSpecial(const sp<RfxMessage>& message) {
    if (message->getId() == RFX_MSG_REQUEST_IMS_DIAL) {
        RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
        String8 dialNumber = String8::format("%s", pDial->address);
        RtmEccNumberController* eccNumberController = (RtmEccNumberController *)findController(
                getSlotId(), RFX_OBJ_CLASS_INFO(RtmEccNumberController));
        bool isEmergencyNumber = eccNumberController->isEmergencyNumber(dialNumber);
        bool sendNewImsDialRequest = false;
        if (pDial->clir != 0 && isEmergencyNumber) {
            // Special ECC case.
            pDial->clir = 0;
            sendNewImsDialRequest = true;
        } else if (pDial->clir == 0 && !isEmergencyNumber) {
            int clirSetting = getStatusManager()->getIntValue(RFX_STATUS_KEY_TBCLIR_N, 0);
            // Some platforms may not bring clir setting's clir, so need to add here.
            if (pDial->clir != clirSetting) {
                pDial->clir = clirSetting;
                sendNewImsDialRequest = true;
            }
        }
        // sendNewImsDialRequest shouldn't be true on android Q because it will bring clir from fwk
        // and will use IMS_EMERGENCY_DIAL for special ecc case.
        if (sendNewImsDialRequest) {
            logD(RFX_LOG_TAG, "handleClirSpecial() : modify clir to %d", pDial->clir);
            sp<RfxMessage> normalDialMessage = RfxMessage::obtainRequest(
                    RFX_MSG_REQUEST_IMS_DIAL, RfxDialData((void*)pDial, sizeof(RIL_Dial)),
                    message, false);
            updateIsImsCallExistToStatusManager(message->getSlotId());
            requestToMcl(message);
            return false;
        }
    }
    return true;
}

// RQ update timing:
// CS: when mCsCallCount increases due to ECPI 130 or 0, and becomes 0 after ECPI 133
// IMS: when getValidImsCallCount() increases before ATD or EAIC, and becomes 0 after ECPI 133
void RtmCallController::updateRQ(int RQ) {
    if (mRQ != RQ) {
        logD(RFX_LOG_TAG, "updateRQ() : RQ   = %d", RQ);

        if (RfxGwsdUtils::getCallControlHandler() != NULL) {
            RfxGwsdUtils::getCallControlHandler()->updateRQ(this, m_slot_id, RQ==1 ? true : false);
        }
        mRQ = RQ;
    }
}

void RtmCallController::resetImsFakeMode() {
    if (mOriginalHangupMessage != NULL) {
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                RIL_E_GENERIC_FAILURE, mOriginalHangupMessage, true);
        responseToRilj(responseMsg);
        mOriginalHangupMessage = NULL;
    }
    if (mOriginalAnswerMessage != NULL) {
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                RIL_E_GENERIC_FAILURE, mOriginalAnswerMessage, true);
        responseToRilj(responseMsg);
        mOriginalAnswerMessage = NULL;
    }
    if (mImsFakeMode) {
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_IMS_CC_FAKE_MODE, false);
        mImsFakeMode = false;
    }
}

bool RtmCallController::onHandleRequestForImsFakeMode(const sp<RfxMessage>& message) {
    if (mImsFakeMode) {
        int msg_id = message->getId();
        bool rejectReq = false;

        if (msg_id == RFX_MSG_REQUEST_ANSWER || msg_id == RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT) {
            if (mOriginalAnswerMessage != NULL) {
                rejectReq = true;
            } else {
                // Stop auto approve if user already response
                RfxTimer::stop(mAutoApproveTimerHandle);
                mAutoApproveTimerHandle = NULL;
                mOriginalAnswerMessage = message;
                approveIncoming(mPendingIncomingCallId, mSeqNo, true, -1, NULL);
                mAutoAnswerImsIncoming = AUTO_ANSWER_FAKE;
                mAutoAnswerCallId = mPendingIncomingCallId;
                if (msg_id == RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT) {
                    int* params = (int*)message->getData()->getData();
                    int mode = params[0];
                    int callId = params[1];
                    mAutoAnswerVideoMode = mode;
                    if (callId != mAutoAnswerCallId) {
                        logW(RFX_LOG_TAG, "onHandleRequestForImsFakeMode callId mismatch");
                    }
                }
                mSeqNo = -1;
                return true;
            }
        } else if (msg_id == RFX_MSG_REQUEST_HANGUP) {
            if (mOriginalHangupMessage != NULL) {
                rejectReq = true;
            } else if (mOriginalAnswerMessage != NULL && mSeqNo == -1) {
            // User have answered the call but want to hangup this call later.
            // Just send AT+CHLD=1X to hangup this call.
                return false;
            } else {
                // Stop auto approve if user already response
                RfxTimer::stop(mAutoApproveTimerHandle);
                mAutoApproveTimerHandle = NULL;
                mOriginalHangupMessage = message;
                approveIncoming(mPendingIncomingCallId, mSeqNo, false, -1, NULL);
                mSeqNo = -1;
                return true;
            }
        }
        if (rejectReq) {
            sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                    RIL_E_GENERIC_FAILURE, message, false);
            responseToRilj(responseMsg);
            return true;
        }
    }

    return false;
}

bool RtmCallController::onHandleResponseForImsFakeMode(const sp<RfxMessage>& message) {
    if (mImsFakeMode) {
        int msg_id = message->getId();

        if (msg_id == RFX_MSG_REQUEST_ANSWER || msg_id == RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT) {
            sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                    RIL_E_SUCCESS, mOriginalAnswerMessage, false);
            responseToRilj(responseMsg);
            mOriginalAnswerMessage = NULL;
            // Also clear mOriginalHangupMessage to prevent some issue.
            resetImsFakeMode();
            return true;
        }
    }

    return false;
}

// GWSD
void RtmCallController::onAutoFakeAcceptTimer() {
    logD(RFX_LOG_TAG, "onAutoFakeAcceptTimer");
    mAutoFakeAcceptTimerHandle = NULL;
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        RfxGwsdUtils::getCallControlHandler()->onAutoFakeAcceptTimer(this);
    }
}

// ECC related control
void RtmCallController::createEccController() {
    logD(RFX_LOG_TAG, "create ecc controller");
    RFX_OBJ_CREATE(mEccController, RtmEccController, this);
}

// Power on for ECC related control
void RtmCallController::onRadioPowerEccStatusChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);

    bool oldStatus = oldValue.asBool();
    bool newStatus = newValue.asBool();
    int mainSlotId = RfxRilUtils::getMajorSim() - 1;

    if (getSlotId() == mainSlotId) {
        if (oldStatus == newStatus) {
            logD(RFX_LOG_TAG, "onRadioPowerEccStatusChanged status unchanged (%d)",
                        (oldStatus ? 1 : 0));
            return;
        }


        logD(RFX_LOG_TAG, "onRadioPowerEccStatusChanged (%d -> %d)",
                    (oldStatus ? 1 : 0), (newStatus ? 1 : 0));
        if (newStatus) {
            mEccController->setRadioOnForEcc(true,
                    RfxCallback0(this, &RtmCallController::onRadioOnForEccTimeout),
                    RADIO_ON_FOR_ECC_TIMEOUT);
        } else {
            mEccController->setRadioOnForEcc(false);
        }
    }
}

void RtmCallController::onRadioOnForEccTimeout() {
    logI(RFX_LOG_TAG, "onRadioOnForEccTimeout");
    cancelRadioOnForEcc();
}

void RtmCallController::cancelRadioOnForEcc() {
    if (getEccPowerOnStatus() == true) {
        logD(RFX_LOG_TAG, "cancelRadioOnForEcc, cancel radio on for ecc.");
        setLocalEccIndication(false); // for emci supported projects, do cancel flow
        setEccPowerOnStatus(false);
    }
}

void RtmCallController::setEccPowerOnStatus(bool isOnForEcc) {
    getNonSlotScopeStatusManager()->setBoolValue(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC, isOnForEcc);
}

bool RtmCallController::getEccPowerOnStatus() {
    return getNonSlotScopeStatusManager()->getBoolValue(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC, false);
}

void RtmCallController::setLocalEccIndication(bool onoff) {
    int msg_data[1];
    msg_data[0] = (onoff ? 1 : 0);  // 1: on, 0: off

    sp<RfxMessage> rilRequest = RfxMessage::obtainRequest(
            RfxRilUtils::getMajorSim() - 1,
            RFX_MSG_REQUEST_LOCAL_SET_ECC_INDICATION,
            RfxIntsData(msg_data, 1));
    requestToMcl(rilRequest);
}

bool RtmCallController::isDsdaSupport() {
    int mode = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_DSDA_MODE, 0);
    if (mode > 100) {
        // dsdaMode value -- 2:DSDA ongoing; 3:DSDA possible; 4:DSDA not possible
        int dsdaMode = mode / 100;
        return (dsdaMode == 2 || dsdaMode == 3);
    } else {
        return (mode == 2 || mode == 3);
    }
}

bool RtmCallController::isDsdaSupport(int mode) {
    if (mode > 100) {
        // dsdaMode value -- 2:DSDA ongoing; 3:DSDA possible; 4:DSDA not possible
        int dsdaMode = mode / 100;
        return (dsdaMode == 2 || dsdaMode == 3);
    } else {
        return (mode == 2 || mode == 3);
    }
}

bool RtmCallController::isDrdsdaSupport() {
    int mode = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_DSDA_MODE, 0);
    if (mode > 100) {
        // drdsdaMode value --  0:DR-DSDA inactive; 1:DR-DSDA active; 2:DR-DSDA not support
        int drdsdaMode = ((mode / 10) % 10);
        return (drdsdaMode == 1);
    } else {
        return false;
    }
}

string RtmCallController::normalizeNumberFromCLIR(string number) {
    number = replaceAll(number, "*31#", "");
    number = replaceAll(number, "#31#", "");
    return number;
}

string RtmCallController::replaceAll(string &str,
        const string &old_value, const string &new_value) {
    while(true) {
        string::size_type pos(0);
        if((pos=str.find(old_value)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    return str;
}

void RtmCallController::handleGetSimultaneousCallingSupport(const sp<RfxMessage>& message) {
    bool dsdaSupport = isDsdaSupport();
    int rilEnabledLogicalSlots[2] = {0, 1};
    int *p = dsdaSupport ? rilEnabledLogicalSlots : NULL;
    int count = dsdaSupport ? 2 : 0;
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
            m_slot_id, RFX_MSG_REQUEST_GET_SIMULTANEOUS_CALLING_SUPPORT,
            RIL_E_SUCCESS, RfxIntsData(p, count),
            message);
    responseToRilj(responseMsg);
}

void RtmCallController::onDsdaModeChanged(RfxStatusKeyEnum key,
            RfxVariant oldValue, RfxVariant newValue) {
    bool oldDsdaSupport = isDsdaSupport(oldValue.asInt());
    bool newDsdaSupport = isDsdaSupport(newValue.asInt());
    if (newDsdaSupport != oldDsdaSupport) {
        int rilEnabledLogicalSlots[2] = {0, 1};
        int *p = newDsdaSupport ? rilEnabledLogicalSlots : NULL;
        int count = newDsdaSupport ? 2 : 0;
        responseToRilj(RfxMessage::obtainUrc(m_slot_id,
            RFX_MSG_UNSOL_SIMULTANEOUS_CALLING_SUPPORT_CHANGED, RfxIntsData(p, count)));
    }
}