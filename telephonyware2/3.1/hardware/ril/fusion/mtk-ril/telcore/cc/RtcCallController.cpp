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

#include "ims/RtcImsConferenceController.h"
#include "ims/RtcImsConferenceHandler.h"
#include "modecontroller/RtcModeSwitchController.h"
#include "nw/RtcNwDefs.h"
#include "MtkPhoneNumberUtils.h"
#include "RtcCallController.h"
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
#include "RfxRawData.h"
#include "RfxRilUtils.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxEsvcData.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RtcEccNumberController.h"
#include "rfx_properties.h"
#include "SuppServDef.h"

#include "RfxGwsdUtils.h"

#include <compiler/compiler_utils.h>

#include <mtkconfigutils.h>
#include <libmtkrilutils.h>
#include "telephony/mtk_ril_ivt.h"
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtcCC"
#define MIN_IMS_CALL_MODE 20
#define MAX_IMS_CALL_MODE 39
#define MIN_CDMA_CALL_MODE 40
#define MAX_CDMA_CALL_MODE 43
#define MIN_VODATA_RAT 6
#define MAX_VODATA_RAT 9
#define AUTO_ANSWER_DELAY 5 * 1000
#define CONF_CALL_ID_SHIFT 100
#define CALL_ID_NONE 0
#define CALL_ID_ALL  -2
#define SET_CALL_INDICATION_TIMER 6*1000
#define MCC_CHAR_LEN 3
#define CHINA_MCC "460"
#define DELAY_SEND_CALL_INFO_TIMER 500

#define AUTO_FAKE_ACCEPT_DELAY 6 * 1000

#define RADIO_ON_FOR_ECC_TIMEOUT (30 * 1000)

#define DELAY_AUTO_ANSWER_WAIT_HANGUP_All_CALL 2 * 1000
#define ANONYMOUS_INVAID  "@anonymous.invalid"


RFX_IMPLEMENT_CLASS("RtcCallController", RtcCallController, RfxController);

// register request to RfxData
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxCallListData, RFX_MSG_REQUEST_GET_CURRENT_CALLS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEmergencyDialData, RfxVoidData, RFX_MSG_REQUEST_EMERGENCY_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEsvcData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_SET_ECC_SERVICE_CATEGORY);
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
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_ECC_MODE);
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
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_IMS_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDialData, RfxVoidData, RFX_MSG_REQUEST_IMS_VT_DIAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_HANGUP_WITH_REASON);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_CALL_SUB_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_CALL_SUB_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_FAKE_ACCEPT); // GWSD
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_FAKE_REJECT); // GWSD
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_SIMULTANEOUS_CALLING_SUPPORT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_FAKE_ACCEPT); // GWSD
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_FAKE_REJECT); // GWSD


RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxSuppServNotificationData, RFX_MSG_UNSOL_SUPP_SVC_NOTIFICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxSuppServNotificationData, RFX_MSG_UNSOL_SUPP_SVC_NOTIFICATION_EX);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RINGBACK_TONE);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_CALL_RING);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData,  RFX_MSG_UNSOL_INCOMING_CALL_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData,  RFX_MSG_UNSOL_CALL_ADDITIONAL_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_CIPHER_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_SPEECH_CODEC_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ESPEECH);
RFX_REGISTER_DATA_TO_URC_ID(RfxCrssNotificationData, RFX_MSG_UNSOL_CRSS_NOTIFICATION);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_CNAP_UPDATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_CLEAR_CLCCNAME);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_EXIT_EMERGENCY_CALLBACK_MODE);
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
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_MD_INTERNET_USAGE_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_REDIAL_EMERGENCY_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_FAKE_INCOMING_CALL_FAILURE);  // GWSD
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_SIP_HEADER);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_CALL_RAT_INDICATION);

RtcCallController::RtcCallController() :
mRedialCtrl(NULL)
, mCallRat(CALL_RAT_NONE), mImsCallRat(CALL_RAT_NONE)
, mEccState(STATE_DISCONNECTED), mEccCallId(-1)
, mPreciseCallStateList(NULL)
, mUseLocalCallFailCause(false), mDialLastError(0)
, mEspeechOnInDialingEcc(false)
, mCsCallCount(0), mPendingCallControlMessage(NULL)
, mPendingIncomingCallId(-1), mAutoAnswerImsIncoming(AUTO_ANSWER_NONE)
, mAutoAnswerCallId(CALL_ID_NONE), mAutoAnswerVideoMode(-1)
, mWaitForSwitchCallResp(false)
, mWaitForSetEccModeResponse(false), mPendingSrvccCallCount(0), mNeedResumeOrAnswerCall(false)
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

RtcCallController::~RtcCallController() {
}

void RtcCallController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    // create ECC controller
    createEccController();

    createRedialController(mEccController);

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
        RFX_MSG_REQUEST_SET_ECC_MODE,
        RFX_MSG_REQUEST_IMS_DIAL,
        RFX_MSG_REQUEST_IMS_VT_DIAL,
        RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL,
        RFX_MSG_REQUEST_HOLD_CALL,
        RFX_MSG_REQUEST_RESUME_CALL,
        RFX_MSG_REQUEST_ASYNC_HOLD_CALL,
        RFX_MSG_REQUEST_ASYNC_RESUME_CALL,
        RFX_MSG_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER,
        RFX_MSG_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER,
        RFX_MSG_REQUEST_ANSWER,
        RFX_MSG_REQUEST_SET_CALL_INDICATION,
        RFX_MSG_REQUEST_FORCE_RELEASE_CALL,
        RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT,
        RFX_MSG_REQUEST_ECC_REDIAL_APPROVE,
        RFX_MSG_REQUEST_CONFERENCE,
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
        RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL,
        RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL,
        RFX_MSG_REQUEST_LOCAL_SET_ECC_SERVICE_CATEGORY,
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
        RFX_MSG_URC_MD_INTERNET_USAGE_IND,
        RFX_MSG_URC_REDIAL_EMERGENCY_INDICATION,
        RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED,
        RFX_MSG_URC_SIP_HEADER,
        RFX_MSG_URC_CALL_RAT_INDICATION,
        // ECBM
        RFX_MSG_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE,
        RFX_MSG_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE,
        RFX_MSG_UNSOL_NO_EMERGENCY_CALLBACK_MODE,
        RFX_MSG_UNSOL_FAKE_INCOMING_CALL_FAILURE,  // GWSD
        RFX_MSG_UNSOL_NO_CARRIER,
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
        RfxStatusChangeCallback(this, &RtcCallController::onCsPhoneChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SERVICE_STATE,
        RfxStatusChangeCallback(this, &RtcCallController::onServiceStateChanged));

    // register callback to get WFC state
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_WFC_STATE,
        RfxStatusChangeCallback(this, &RtcCallController::onWFCPDNStatusChanged));

    // register callbacks to get required information
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
        RfxStatusChangeCallback(this, &RtcCallController::onRadioStateChanged));

    // register callbacks to get card type change event
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
        RfxStatusChangeCallback(this, &RtcCallController::onCardTypeChanged));

    // register callbacks to get ecc indication when radio power on status updated.
    getNonSlotScopeStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC,
            RfxStatusChangeCallback(this,
                    &RtcCallController::onRadioPowerEccStatusChanged));
}

void RtcCallController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    RfxController::onDeinit();
    freePreciseCallStateList(mPreciseCallStateList);
    mPreciseCallStateList = NULL;
}

void RtcCallController::createRedialController(RtcEccController *eccController) {
    RFX_OBJ_CREATE(mRedialCtrl, RtcRedialController, this);
    if (eccController != NULL) {
        logD(RFX_LOG_TAG, "createRedialController set ecc controller");
        mRedialCtrl->setEccController(eccController);
    } else {
        logD(RFX_LOG_TAG, "createRedialController invalid ecc controller");
    }
}

bool RtcCallController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    int slot_id = message->getSlotId();
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

    // Convert DIAL to LOCAL_EMERGENCY_DIAL
    // For TelephonyWare:
    // Android Q framework won't use DIAL for ECC
    // only KaiOS and Telematics without EMERGENCY_DIAL, use dial() to make ECC call,
    // check if the number is ECC and make an emergency dial request and save the original
    // request for sending response later.
    if ((msg_id == RFX_MSG_REQUEST_DIAL || msg_id == RFX_MSG_REQUEST_IMS_DIAL)
            && handleDialAsEmergencyDial(message)) {
        logD(RFX_LOG_TAG, "handleDialAsEmergencyDial");

        RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
        String8 dialNumber = String8::format("%s", pDial->address);

        mOriginalDialMessage = message;
        sp<RfxMessage> emergencyDialMessage = RfxMessage::obtainRequest(getSlotId(),
                    (message->getId() == RFX_MSG_REQUEST_DIAL
                            ? RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL
                            : RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL)
                            , message, true);
        return onHandleRequest(emergencyDialMessage);
    }

    // Convert EMERGENCY_DIAL to LOCAL_EMERGENCY_DIAL or NORMAL DIAL
    // convertedMessage changes only when converting to DIAL request
    sp<RfxMessage> convertedMessage = message;
    if ((msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL
            || msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL)) {
        mOriginalDialMessage = message;
        RIL_Emergency_Dial *pEccDial = (RIL_Emergency_Dial*) (message->getData()->getData());
        String8 dialNumber = String8::format("%s", pEccDial->dialData->address);
        RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
                getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));
        EmergencyCallRouting eccRouting = eccNumberController->getEmergencyCallRouting(dialNumber);
        String8 ignoreClirWhenEcc = getStatusManager()->getString8Value(
                RFX_STATUS_KEY_CARRIER_IGNORE_CLIR_WHEN_ECC);
        logD(RFX_LOG_TAG, "ignoreClirWhenEcc %s", ignoreClirWhenEcc.string());

        if (msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL && (ignoreClirWhenEcc == "1")) {
            pEccDial->dialData->clir = 0;
        }

        // for debug
        RIL_Dial *pDial = (RIL_Dial*)(pEccDial->dialData);
        logD(RFX_LOG_TAG,
                "isTesting=%d, eccRouting=%d, routing=%d, serviceCategory=%d, clir=%d, number=%s ",
                pEccDial->isTesting, eccRouting,  pEccDial->routing, pEccDial->serviceCategory,
                pDial->clir, RfxRilUtils::pii(RFX_LOG_TAG, pDial->address));

        if (!pEccDial->isTesting &&
                ((pEccDial->routing == ECC_ROUTING_UNKNOWN && eccRouting != ECC_ROUTING_NORMAL) ||
                pEccDial->routing == ECC_ROUTING_EMERGENCY)){
            int serviceCategory = pEccDial->serviceCategory;
            String8 urn;
            if (pEccDial->serviceCategory == UNSPECIFIED) {
                serviceCategory = eccNumberController->getServiceCategoryAndUrn(dialNumber, urn);
            }

            requestToMcl(RfxMessage::obtainRequest(getSlotId(),
                    RFX_MSG_REQUEST_LOCAL_SET_ECC_SERVICE_CATEGORY,
                    RfxEsvcData(serviceCategory, urn)));

            sp<RfxMessage> emergencyDialMessage = RfxMessage::obtainRequest(
                    (message->getId() == RFX_MSG_REQUEST_EMERGENCY_DIAL
                            ? RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL
                            : RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL)
                            , RfxDialData((void*)pEccDial->dialData, sizeof(RIL_Dial))
                            , message, false);
            return onHandleRequest(emergencyDialMessage);
        } else {
            // For either one of the following cases
            // pEccDial->isTesting
            // pEccDial->routing == NORMAL
            // pEccDial->routing == UNKNOWN && !isEccRouting
            unsetEccModeLocally();
            cancelRadioOnForEcc(); // cancel radio on for ecc if enabled.

            convertedMessage = RfxMessage::obtainRequest(
                        (message->getId() == RFX_MSG_REQUEST_EMERGENCY_DIAL
                                ? RFX_MSG_REQUEST_DIAL
                                : RFX_MSG_REQUEST_IMS_DIAL)
                                , RfxDialData((void*)pEccDial->dialData, sizeof(RIL_Dial))
                                , message, false);
        }
    }

    int converted_msg_id = convertedMessage->getId();

    switch (converted_msg_id) {
        case RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL:
            if (mRedialCtrl != NULL) {
                mRedialCtrl->notifyRilRequest(message);
            }
            TELEPHONYWARE_FALLTHROUGH;
            // fall through
        case RFX_MSG_REQUEST_DIAL:
            if (rejectDualDialForDSDS()  || eCallExists()) {
                responseDialFailed(convertedMessage);
                return true;
            }
            if (converted_msg_id == RFX_MSG_REQUEST_DIAL && hasImsCall(slot_id)) {
                logD(RFX_LOG_TAG, "cannot dial cs call when has ims call. (slot%d)", slot_id);
                responseDialFailed(convertedMessage);
                return true;
            }
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, true);
            mDialMessage = convertedMessage;
            break;
        case RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL:
            if (mRedialCtrl != NULL) {
                mRedialCtrl->notifyRilRequest(message);
            }
            hangupIncomingCallOnAllSlot();
            TELEPHONYWARE_FALLTHROUGH;
            // fall through
        case RFX_MSG_REQUEST_IMS_DIAL:
            mDialMessage = convertedMessage;
            TELEPHONYWARE_FALLTHROUGH;
            // fall through
        case RFX_MSG_REQUEST_IMS_VT_DIAL:
        case RFX_MSG_REQUEST_PULL_CALL:
        case RFX_MSG_REQUEST_CONFERENCE_DIAL:
        case RFX_MSG_REQUEST_DIAL_WITH_SIP_URI:
        case RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI:
            if (rejectDualDialForDSDS() || isDuringSrvcc(slot_id)  || eCallExists()) {
                responseDialFailed(convertedMessage);
                return true;
            }
            handleImsDialRequest(convertedMessage);
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, true);
            needSendRequestToMcl = handleClirSpecial(convertedMessage);
            break;
        case RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE  : {
            RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
                    m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));

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
        case RFX_MSG_REQUEST_ANSWER:
            if (isDuringSrvcc(slot_id) && hasImsCall(slot_id)) {
                logE(RFX_LOG_TAG, "Cannot accept call, mSrvccState[%d] = %d", slot_id, mSrvccState[slot_id]);
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
                responseToRilj(responseMsg);
                return true;
            }
            break;
        case RFX_MSG_REQUEST_HANGUP:
        case RFX_MSG_REQUEST_HANGUP_WITH_REASON: {
            int hangupCallId = ((int *)message->getData()->getData())[0];
            CallType hangupCallType = getConnectionCallType(hangupCallId);
            RfxImsCallInfo* targetCall = getImsCallByCallId(mImsCalls[slot_id], hangupCallId);

            if (targetCall != NULL && isDuringSrvcc(slot_id)) {
                logE(RFX_LOG_TAG, "Cannot hangup call %d, mSrvccState[%d] = %d", hangupCallId, slot_id, mSrvccState[slot_id]);
                sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
                responseToRilj(responseMsg);
                return true;
            }

            if (hangupCallType == CALL_TYPE_EMERGENCY) {
                logI(RFX_LOG_TAG, "hangup call type ecc (callId=%d)", hangupCallId);
                mEccController->onEccHangupLocal();
            }

            if (mRedialCtrl != NULL) {
                mRedialCtrl->notifyRilRequest(message);
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
            stopAutoAnswerTimer(CALL_ID_ALL);
            if (mRedialCtrl != NULL) {
                mRedialCtrl->notifyRilRequest(message);
            }
            mPendingIncomingCallId = -1;
            break;
        }
        // For ECC under flight mode:
        // new design (support EMCI): apply to DSDS and SS
        // Need to send AT+EMCS=1 with REQUEST_LOCAL_CURRENT_STATUS
        // 1) normal case:
        // AT+EMCI=1(main sim, radio channel) > AT+EFUN=1 > AT+EMCS=1,1(sim w/call, cc channel)
        // > ATDE > +CIEV:102,0 > AT+EMCS=0(sim w/call, cc channel)
        // 2) exception case:
        // AT+EMCS=1(main sim, radio channel) > AT+EFUN=1 > AT+EMCI=0(main sim, radio channel)

        // old design: only apply to SS
        // No need to send AT+EMCS=1 again with REQUEST_LOCAL_CURRENT_STATUS
        // 1) normal case:
        // AT+EMCS=1,1(single sim, radio channel) > AT+EFUN=1
        // > ATDE > +CIEV:102,0 > AT+EMCS=0(single sim, cc channel)
        // 2) exception case:
        // AT+EMCS=1,1(single sim, radio channel) > AT+EFUN=1 > AT+EMCS=0(main sim, radio channel)
        case RFX_MSG_REQUEST_SET_ECC_MODE: {
            RfxStringsData* data = (RfxStringsData*)message->getData();
            char** params = (char**)data->getData();
            String8 dialNumber = String8::format("%s", params[0]);

            // dialNumber is empty for SET_ECC_MODE(false)
            if (!dialNumber.isEmpty()) {
                RtcEccNumberController* eccNumberController =
                        (RtcEccNumberController *)findController(
                        getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));

                // Condition to set ECM: let RILD to check
                // - 6M(with C2K) project: only to C2K-enabled phone
                // - 5M(without C2K) project: specific OP such as Vzw

                // Skip if the number is not an ECC
                EmergencyCallRouting eccRouting =
                        eccNumberController->getEmergencyCallRouting(dialNumber);
                if (eccRouting == ECC_ROUTING_NORMAL) {
                    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                            RIL_E_GENERIC_FAILURE, message, true);
                    responseToRilj(responseMsg);
                    return true;
                }
            }

            if (!RfxRilUtils::isEmciSupport()) {
                // EMCI: always allow to main slot (SS & DSDS)
                // EMCS: only for SS and C2K
                if (RfxRilUtils::rfxGetSimCount() > 1 ||
                        !mRedialCtrl->isEmergencyModeSupported(message->getSlotId())) {
                    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(
                            RIL_E_GENERIC_FAILURE, message, true);
                    responseToRilj(responseMsg);
                    return true;
                }

                // No need to handle REQUEST_SET_ECC_MODE in RedialHandler if EMCI is enabled
                // because no need to suppress REQUEST_LOCAL_CURRENT_STATUS
                if (mRedialCtrl->notifyRilRequest(message)) {
                    return true;
                }
            }
            // else no need to go through RedialHandler, set AT+EMCI=1 to modem directly

            /* ALPS03346578: Emergency dial can be handled after receiving response of
                RFX_MSG_REQUEST_SET_ECC_MODE */
            mWaitForSetEccModeResponse = true;
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
        case RFX_MSG_REQUEST_CDMA_FLASH:
            handleCdmaFlashRequest(message);
            break;
        case RFX_MSG_REQUEST_HOLD_CALL:
        case RFX_MSG_REQUEST_RESUME_CALL:
            handleAsyncImsCallControlRequest(message);
            return true;
        case RFX_MSG_REQUEST_SET_CALL_INDICATION:
            needSendRequestToMcl = handleSetCallIndicationRequest(message);
            break;
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
        if (mOriginalDialMessage != NULL &&
                (converted_msg_id == RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL ||
                 converted_msg_id == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL ||
                 converted_msg_id == RFX_MSG_REQUEST_DIAL ||
                 converted_msg_id == RFX_MSG_REQUEST_IMS_DIAL)) {
            mEccController->onPreLocalEmergencyDial(convertedMessage);
            mEccState = STATE_CONNECTING;
        }
        // when EMERGENCY_DIAL is converted to DIAL, convertedMessage is different form message
        updateIsImsCallExistToStatusManager(convertedMessage->getSlotId());
        requestToMcl(convertedMessage);
    }
    return true;
}

bool RtcCallController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    int slotId = message->getSlotId();

    // GWSD
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        if (RfxGwsdUtils::getCallControlHandler()->onHandleTelCoreUrc(this, message)) {
            if (msg_id == RFX_MSG_UNSOL_INCOMING_CALL_INDICATION) {
                RfxStringsData *data = (RfxStringsData*)message->getData();
                char **params = (char**)data->getData();
                int callMode = atoi(params[3]);
                int callId = atoi(params[0]);
                if (callMode < MIN_IMS_CALL_MODE || callMode > MAX_IMS_CALL_MODE) {
                    mCsCallCount++;
                    if (mCsCallCount > 0) {
                        updateRQ(1);
                    }
                    updateCallCount();
                }
                if ((params[5] != NULL) && (strlen(params[5]) != 0)) {
                    ForwardNumberInfo info(callId, params[5]);
                    mForwardNumbers.add(info);
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
               int callId = atoi(params[0]);
               if (msgType == 133) {
                   removeForwardedNumberInfo(callId);
               }
            }
            return true;
        }
    }

    RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));
    // logD(RFX_LOG_TAG, "onHandleUrc: %s", RFX_ID_TO_STR(msg_id));

    bool responseToRilJ = true;
    if (imsConfCtrl != NULL && imsConfCtrl->handleUrc(message)) {
       return true;
    }

    if (mRedialCtrl != NULL && mRedialCtrl->notifyRilUrc(message)) {
       return true;
    }

    switch (msg_id) {
        case RFX_MSG_UNSOL_CDMA_CALL_WAITING:
            handleCdmaCallWait();
            break;
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
            if (countString == 8) {
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
        case RFX_MSG_UNSOL_CALL_INFO_INDICATION: {
            handleAutoAnswer(message);
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED, false);
            RfxStringsData* data = (RfxStringsData*)message->getData();
            char** params = (char**)data->getData();
            int callMode = atoi(params[5]);
            int callId = atoi(params[0]);
            int msgType = atoi(params[1]);
            if (msgType == 133) {
                removeForwardedNumberInfo(callId);
            }
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
                        &RtcCallController::onAutoFakeAcceptTimer), ms2ns(AUTO_FAKE_ACCEPT_DELAY));
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
        case RFX_MSG_URC_REDIAL_EMERGENCY_INDICATION:
            handleRedialEcc(message);
            break;
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
        default:
            break;
    }

    if (responseToRilJ) {
        responseToRilJAndUpdateIsImsCallExist(message);
    }
    return true;
}

bool RtcCallController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    //logD(RFX_LOG_TAG, "onHandleResponse: %s", RFX_ID_TO_STR(msg_id));

    if (mOriginalDialMessage != NULL &&
            (msg_id == RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL ||
             msg_id == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL ||
             msg_id == RFX_MSG_REQUEST_DIAL ||
             msg_id == RFX_MSG_REQUEST_IMS_DIAL)) {
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
            if (mRedialCtrl != NULL && mRedialCtrl->notifyRilResponse(message)) {
                return true;
            }
            fillForwardedNumberInfo(message);
            handleGetCurrentCallsResponse(message);
            responseToRilJAndUpdateIsImsCallExist(message);
            return true;
        case RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL:
        case RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL:
            if (mRedialCtrl != NULL) {
                mRedialCtrl->notifyRilResponse(message);
            }
            TELEPHONYWARE_FALLTHROUGH;
            // fall through
        case RFX_MSG_REQUEST_IMS_DIAL:
        case RFX_MSG_REQUEST_DIAL_WITH_SIP_URI:
        case RFX_MSG_REQUEST_CONFERENCE_DIAL:
        case RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI:
        case RFX_MSG_REQUEST_IMS_VT_DIAL:
            if (message->getError() != RIL_E_SUCCESS) {
                handleImsDialFailResponse(message);
                responseToRilJAndUpdateIsImsCallExist(message);
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, false);
                return true;
            }
            break;
        case RFX_MSG_REQUEST_DIAL:
            if (message->getError() != RIL_E_SUCCESS) {
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_IN_CALL, false);
            }
            break;
        case RFX_MSG_REQUEST_SET_ECC_MODE:
            /* ALPS03346578: Emergency dial can be handled after receiving response of
                RFX_MSG_REQUEST_SET_ECC_MODE */
                mWaitForSetEccModeResponse = false;
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
                int hangupCallId = ((int *)message->getData()->getData())[0];
                if (hasPendingHangupRequest(hangupCallId)) {
                    logD(RFX_LOG_TAG, "HANGUP error, force release");
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
        case RFX_MSG_REQUEST_LAST_CALL_FAIL_CAUSE:
            handleLastFailCauseResponse(message);
            break;
        default:
            break;
    }

    if (mOriginalDialMessage != NULL &&
           (msg_id == RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL ||
            msg_id == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL ||
            msg_id == RFX_MSG_REQUEST_DIAL ||
            msg_id == RFX_MSG_REQUEST_IMS_DIAL)) {
        // For the two scenarios
        // Convert DIAL to LOCAL_EMERGENCY_DIAL in handleDialAsEmergencyDial
        // Convert EMERGENCY_DIAL to LOCAL_EMERGENCY_DIAL or DIAL
        // original message's pId & pToken are copied, id is replaced & token is created
        // since pId is correct here, it is not necessary to change id (nice to do)
        responseToRilj(RfxMessage::obtainResponse(mOriginalDialMessage->getId(), message));
        mOriginalDialMessage = NULL;
    } else if (mOriginalHangupRingingMessage != NULL &&
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

void RtcCallController::handleLastFailCauseResponse(const sp<RfxMessage>& message) {
    // Non-null only when MD not support DMF and IMS call disconnected.
    if (mDisconnectedImsCallIdQueue.empty()) {
        return;
    }

    RfxCallFailCauseData* data = (RfxCallFailCauseData*)message->getData();
    RIL_LastCallFailCauseInfo* params = (RIL_LastCallFailCauseInfo*)data->getData();
    int causeCode = (int) params->cause_code;
    int callId = mDisconnectedImsCallIdQueue.front();
    mDisconnectedImsCallIdQueue.pop();

    // ALPS04374742: Call Preference event reporting via UNSOL_OEM_HOOK_RAW
    uint8_t* rawBytes = obtainCallPrefRawBytes(callId, causeCode);
    logD(RFX_LOG_TAG, "[%d] handleLastFailCauseResponse, (%d, %d)",
            m_slot_id, callId, causeCode);
    sp<RfxMessage> urc = RfxMessage::obtainUrc(
            m_slot_id, RFX_MSG_UNSOL_OEM_HOOK_RAW, RfxRawData(rawBytes, 17));
    if (rawBytes != NULL) {
        delete[] rawBytes;
    }
    RfxMainThread::enqueueMessage(urc);
}

bool RtcCallController::onPreviewMessage(const sp<RfxMessage>& message) {
    int ret = pendingOrResumeEcpi(message);
    if (ret == RET_BLOCK) {
        logD(RFX_LOG_TAG, "pending %s for CHLD=2", idToString(message->getId()));
        return false;
    } else if (ret == RET_PASS) {
        return true;
    }
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();

        // Hangup all current calls for the following conditions when dialing an new ECC
        String8 dialNumber("");
        RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
                getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));

        bool isEccRouting = false;
        bool isEccTesting = false;
        bool isEccRoutingNormal = false;
        if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            RIL_Emergency_Dial *pEccDial = (RIL_Emergency_Dial*) (message->getData()->getData());
            dialNumber = String8::format("%s", pEccDial->dialData->address);
            EmergencyCallRouting eccRouting =
                    eccNumberController->getEmergencyCallRouting(dialNumber);
            if (!pEccDial->isTesting &&
                ((pEccDial->routing == ECC_ROUTING_UNKNOWN && eccRouting != ECC_ROUTING_NORMAL) ||
                pEccDial->routing == ECC_ROUTING_EMERGENCY)){
                isEccRouting = true;
            } else if (pEccDial->isTesting) {
                isEccTesting = true;
            } else if (pEccDial->routing == ECC_ROUTING_NORMAL ||
                    (pEccDial->routing == ECC_ROUTING_UNKNOWN && eccRouting == ECC_ROUTING_NORMAL)) {
                isEccRoutingNormal = true;
            }
        } else if (msg_id == RFX_MSG_REQUEST_DIAL || msg_id == RFX_MSG_REQUEST_IMS_DIAL) {
            RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
            dialNumber = String8::format("%s", pDial->address);
            if (eccNumberController->getEmergencyCallRouting(dialNumber) == ECC_ROUTING_EMERGENCY) {
                isEccRouting = true;
            }
        }

        if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL || msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL
                || msg_id == RFX_MSG_REQUEST_DIAL || msg_id == RFX_MSG_REQUEST_IMS_DIAL) {
            if (isEccTesting || isEccRoutingNormal || !isEccRouting) {
                cancelRadioOnForEcc(); // cancel radio on for ecc if enabled.
            }
        }

        if (isEccRouting || isEccTesting || isEccRoutingNormal) {
            // 1) dialing ECC already exists
            // In VtsHalRadioV_4, "emergencyDial()" is triggered sequentailly by 3 times
            // and expects return value to be "OK"
            // Since isTesting is true in VTS case, RFX_MSG_REQUEST_EMERGENCY_DIAL
            // will convert to RFX_MSG_REQUEST_DIAL, need some special handling
            // Here the "OK" is returned by modem VDM, not L4C, to indicate "Ack".
            // It does NOT mean the call is successfully made to network
            // For the 1st ATD, VDM returns "OK" directly, and starts domain seleciton
            // For the 2nd ATD, if VDM is still doing domain selection (in case ECPI is not
            // turned to 2/3/6), VDM returns "+CME ERROR: 100" to reject the request
            // Therefore, RILD hangs up the 1st call before making 2nd one to modem.
            if (mEccState == STATE_DIALING) {
                logD(RFX_LOG_TAG, "Hang up all calls since dialing ECC exists");
                requestToMcl(RfxMessage::obtainRequest(
                        getSlotId(), RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData()));
                return false;
            }

            // 2) Other slot has calls
            // In VtsHalRadioV_4, "emergencyDial()" is triggered on one slot and then another
            // slot without hanging the previous one.
            // "emergencyDial()"on slot2 will be blocked with "GENERIC_FAILURE".
            // To avoid slot2 failure, hangup slot2 calls first.
            for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
                if (i != getSlotId()) {
                    if (getStatusManager(i)->getIntValue(
                            RFX_STATUS_KEY_VOICE_CALL_COUNT, 0) > 0) {
                        logD(RFX_LOG_TAG, "Hang up all calls since call exists on slot%d", i);
                        requestToMcl(RfxMessage::obtainRequest(
                                i, RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData()));
                        return false;
                    }
                }
            }

            // 3) IMS call exists when dialing CS ECC [ALPS04126362]
            if (msg_id == RFX_MSG_REQUEST_IMS_DIAL ||
                    msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
                if (mCsCallCount > 0) {
                    logD(RFX_LOG_TAG, "Hang up all calls since mCsCallCount=%d", mCsCallCount);
                    requestToMcl(RfxMessage::obtainRequest(
                            getSlotId(), RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData()));
                    return false;
                }
             // 4) CS call exists when dialing IMS ECC [ALPS04356814]
            } else if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                    msg_id == RFX_MSG_REQUEST_DIAL) {
                if (hasImsCall(getSlotId())) {
                // For GSI/AOSP-based FW only (In BSP+, call should be hung up by Telecom already)
                    logD(RFX_LOG_TAG, "Hang up all calls since hasImsCall=true");
                    requestToMcl(RfxMessage::obtainRequest(
                            getSlotId(), RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData()));
                    return false;
                }
            }
        }
    }

    if (!canHandleHangupForegndOrBackgndRequest(message)) {
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, true);
        return false;
    }

    return canHandleCsDialRequest(message)
        && canHandleEmergencyDialRequest(message)
        && canHandleRequestDuringComboOperations(message)
        && canHandleAnswerRequest(message);
}

void RtcCallController::handleSpeechCodecInfo(const sp<RfxMessage>& msg) {
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

void RtcCallController::handleAsyncCallControlResponse(const sp<RfxMessage>& message) {
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

void RtcCallController::handleAsyncImsCallControlRequest(const sp<RfxMessage>& message) {
    int slot_id = message->getSlotId();

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

    RfxImsCallInfo* targetCall = getImsCallByCallId(mImsCalls[slot_id], callId);
    if (targetCall != NULL && isDuringSrvcc(slot_id)) {
        logE(RFX_LOG_TAG, "Cannot hold/Resume call %d, mSrvccState[%d] = %d", callId, slot_id, mSrvccState[slot_id]);
        sp<RfxMessage> responseMsg =
                RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
        responseToRilj(responseMsg);
        return;
    }

    int requestId = (message->getId() == RFX_MSG_REQUEST_HOLD_CALL)?
            RFX_MSG_REQUEST_ASYNC_HOLD_CALL : RFX_MSG_REQUEST_ASYNC_RESUME_CALL;
    sp<RfxMessage> request =
            RfxMessage::obtainRequest(getSlotId(), requestId, RfxIntsData(msg_data, 1));
    requestToMcl(request);
}

void RtcCallController::handleAsyncCallControlResult(const sp<RfxMessage>& message) {
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

int RtcCallController::pendingOrResumeEcpi(const sp<RfxMessage>& message) {
    if (message->getId() == RFX_MSG_UNSOL_CALL_INFO_INDICATION) {
        RfxStringsData* data = (RfxStringsData*)message->getData();
        char** params = (char**)data->getData();
        int callMode = atoi(params[5]);
        if (callMode == 0) {
            bool chld2Ongoing = getStatusManager()->getBoolValue(
                    RFX_STATUS_KEY_IS_CHLD2_ONGOING, false);
            if (chld2Ongoing) {
                return RET_BLOCK;
            } else {
                return RET_PASS;
            }
        }
    }
    return RET_BYPASS;
}

bool RtcCallController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    int ret = pendingOrResumeEcpi(message);
    if (ret == RET_PASS) {
        logD(RFX_LOG_TAG, "resume %s for CHLD=2", idToString(message->getId()));
        return true;
    } else if (ret == RET_BLOCK) {
        return false;
    }
    if (message->getType() == REQUEST) {

        int msg_id = message->getId();
        // Pending ECC dial for the following conditions
        String8 dialNumber("");
        RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
                getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));

        bool isEccRouting = false;
        bool isEccTesting = false;
        bool isEccRoutingNormal = false;
        if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            RIL_Emergency_Dial *pEccDial = (RIL_Emergency_Dial*) (message->getData()->getData());
            dialNumber = String8::format("%s", pEccDial->dialData->address);
            EmergencyCallRouting eccRouting =
                    eccNumberController->getEmergencyCallRouting(dialNumber);
            if (!pEccDial->isTesting &&
                ((pEccDial->routing == ECC_ROUTING_UNKNOWN && eccRouting != ECC_ROUTING_NORMAL) ||
                pEccDial->routing == ECC_ROUTING_EMERGENCY)){
                isEccRouting = true;
            } else if (pEccDial->isTesting) {
                isEccTesting = true;
            } else if (pEccDial->routing == ECC_ROUTING_NORMAL ||
                    (pEccDial->routing == ECC_ROUTING_UNKNOWN && eccRouting == ECC_ROUTING_NORMAL)) {
                isEccRoutingNormal = true;
            }
        } else if (msg_id == RFX_MSG_REQUEST_DIAL || msg_id == RFX_MSG_REQUEST_IMS_DIAL) {
            RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
            dialNumber = String8::format("%s", pDial->address);
            if (eccNumberController->getEmergencyCallRouting(dialNumber) == ECC_ROUTING_EMERGENCY) {
                isEccRouting = true;
            }
        }

        if (isEccRouting || isEccTesting || isEccRoutingNormal) {
            // 1) dialing ECC already exists
            if (mEccState == STATE_DIALING) {
                return false;
            }

            // 2) Other slot has calls
            for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
                if (i != getSlotId()) {
                    if (getStatusManager(i)->getIntValue(
                            RFX_STATUS_KEY_VOICE_CALL_COUNT, 0) > 0) {
                        return false;
                    }
                }
            }

            // 3) IMS call exists when dialing CS ECC [ALPS04126362]
            if (msg_id == RFX_MSG_REQUEST_IMS_DIAL ||
                    msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
                if (mCsCallCount > 0) {
                    return false;
                }
             // 4) CS call exists when dialing IMS ECC [ALPS04356814]
            } else if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                    msg_id == RFX_MSG_REQUEST_DIAL) {
                if (hasImsCall(getSlotId())) {
                    return false;
                }
            }
        }
    }

    return canHandleCsDialRequest(message)
        && canHandleEmergencyDialRequest(message)
        && canHandleRequestDuringComboOperations(message)
        && canResumeHandleAnswerRequest(message)
        && canHandleHangupForegndOrBackgndRequest(message);
}

// [ALPS04642635] Block CS Dial if there is unfinished call switching
// Let modem to reject Dial if switching call fails
bool RtcCallController::canHandleCsDialRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_DIAL || msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL) {
            return (!mWaitForSwitchCallResp);
        }
    }
    return true;
}


bool RtcCallController::canHandleHangupForegndOrBackgndRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND ||
                msg_id == RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND) {
            return (!mWaitForSwitchCallResp);
        }
    }
    return true;
}

void RtcCallController::unsetEccModeLocally() {
    bool isEccModeInFlightMode = getNonSlotScopeStatusManager()->getBoolValue(
            RFX_STATUS_KEY_EMERGENCY_MODE_IN_FLIGHT_MODE, false);
    if (isEccModeInFlightMode) {
        if (!RfxRilUtils::isEmciSupport()) {
            // reset value in RedialHandler and reset modem by AT+EMCS=0
            mRedialCtrl->setEmergencyModeExt(false);
        } else {
            // no need to go through RedialHandler, reset modem by AT+EMCI=0
            char **requestData = (char **) calloc(3, sizeof(char *));
            RFX_ASSERT(requestData != NULL);
            asprintf(&requestData[1], "%d", 0);  // enable
            asprintf(&requestData[2], "%d", 1);  // airplane mode
            sp<RfxMessage> rilRequest = RfxMessage::obtainRequest(
                    RfxRilUtils::getMajorSim() - 1,
                    RFX_MSG_REQUEST_SET_ECC_MODE,
                    RfxStringsData(requestData, 3));
            free(requestData[1]);
            free(requestData[2]);
            free(requestData);

            requestToMcl(rilRequest);
        }
    }
}

/* ALPS03346578: Emergency dial can be handled after receiving response of
    RFX_MSG_REQUEST_SET_ECC_MODE */
bool RtcCallController::canHandleEmergencyDialRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                msg_id == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            // ALPS03833937: Wait for triggerModeSwitchByEcc()
            int modemOffState = getNonSlotScopeStatusManager()->getIntValue(
                    RFX_STATUS_KEY_MODEM_OFF_STATE, MODEM_OFF_IN_IDLE);
            // ALPS04329577: Wait for world mode switching done
            int worldModeState = getNonSlotScopeStatusManager()->getIntValue(
                    RFX_STATUS_KEY_WORLD_MODE_STATE, 1);
            return (!mWaitForSetEccModeResponse && modemOffState != MODEM_OFF_BY_MODE_SWITCH
                    && worldModeState != 0);
        }
    }
    return true;
}

bool RtcCallController::canHandleRequestDuringComboOperations(const sp<RfxMessage>& message) {
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

bool RtcCallController::canHandleAnswerRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_ANSWER || msg_id == RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT) {
            logD(RFX_LOG_TAG, "canHandleAnswerRequest");
            if (isExistDisallowHoldVideoOnOtherSub()) {
                hangupAllCallsOnOtherSub();
                return false;
            }
            Vector<RfxImsCallInfo*> holdingCalls = getHoldCallOnOtherSub();
            if (holdingCalls.size() != 0) {
                hangupHoldOnOtherSub(holdingCalls);
                return false;
            }
            if (mPendingCallControlMessage != NULL
                && mPendingCallControlMessage->getId() == RFX_MSG_REQUEST_HOLD_CALL) {
                return false;
            }
        }
    }
    return true;
}

bool RtcCallController::canResumeHandleAnswerRequest(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        int msg_id = message->getId();
        if (msg_id == RFX_MSG_REQUEST_ANSWER || msg_id == RFX_MSG_REQUEST_VIDEO_CALL_ACCEPT) {
            logD(RFX_LOG_TAG, "canResumeHandleAnswerRequest");
            if (isExistDisallowHoldVideoOnOtherSub()) {
                return false;
            }
            if (mPendingCallControlMessage != NULL
                && mPendingCallControlMessage->getId() == RFX_MSG_REQUEST_HOLD_CALL) {
                return false;
            }
        }
    }
    return true;
}

bool RtcCallController::onCheckIfRejectMessage(
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

    // [ALPS03598523] block all request if SIM switch ongoing
    int modemOffState = getNonSlotScopeStatusManager()->getIntValue(
        RFX_STATUS_KEY_MODEM_OFF_STATE, MODEM_OFF_IN_IDLE);

    // [ALPS04377333] block EMCS triggered by AP if mode switch ongoing
    if (modemOffState == MODEM_OFF_BY_MODE_SWITCH) {
        if (msgId == RFX_MSG_REQUEST_SET_ECC_MODE) {
            logD(RFX_LOG_TAG, "block request due to mode switch ongoing");
            return true;
        }
    }

    // [ALPS03598523] block all request if SIM switch ongoing
    if (modemOffState == MODEM_OFF_BY_SIM_SWITCH) {
        if (msgId == RFX_MSG_REQUEST_SET_ECC_MODE) {
            logD(RFX_LOG_TAG, "block request due to SIM switch ongoing");
            return true;
        }

        // reset emergency Mode to false, before blocking EMERGENCY_DIAL
        if (msgId == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                msgId == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            unsetEccModeLocally();
            cancelRadioOnForEcc(); // cancel radio on for ecc if enabled.
        }

        // Fake error cause as ERROR_UNSPECIFIED
        if (msgId == RFX_MSG_REQUEST_DIAL ||
                msgId == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                msgId == RFX_MSG_REQUEST_IMS_DIAL ||
                msgId == RFX_MSG_REQUEST_IMS_VT_DIAL ||
                msgId == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            mUseLocalCallFailCause = true;
            mDialLastError = CALL_FAIL_ERROR_UNSPECIFIED;
            logD(RFX_LOG_TAG, "block request due to SIM switch ongoing");
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
                msgId == RFX_MSG_REQUEST_SET_ECC_MODE ||
                msgId == RFX_MSG_REQUEST_GET_CURRENT_CALLS ||
                msgId == RFX_MSG_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE) {
            return false;
        }
        // reset emergency Mode to false, before blocking EMERGENCY_DIAL
        if (msgId == RFX_MSG_REQUEST_EMERGENCY_DIAL ||
                msgId == RFX_MSG_REQUEST_IMS_EMERGENCY_DIAL) {
            unsetEccModeLocally();
            cancelRadioOnForEcc(); // cancel radio on for ecc if enabled.
            return true;
        }
    }

    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

void RtcCallController::onCsPhoneChanged(RfxStatusKeyEnum key,
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

void RtcCallController::onServiceStateChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    RfxNwServiceState oldSS = oldValue.asServiceState();
    RfxNwServiceState newSS = newValue.asServiceState();
    int voiceRadioTech = oldSS.getRilVoiceRadioTech();
    int voiceRegState = oldSS.getRilVoiceRegState();

    if (oldSS.getRilVoiceRadioTech() != newSS.getRilVoiceRadioTech()) {
        voiceRadioTech = newSS.getRilVoiceRadioTech();
        //logD(RFX_LOG_TAG, "(slot %d) Voice service state changed: radioTech=%d",
        //        m_slot_id, voiceRadioTech);
    }
    if (oldSS.getRilVoiceRegState() != newSS.getRilVoiceRegState()) {
        voiceRegState = newSS.getRilVoiceRegState();
        //logD(RFX_LOG_TAG, "(slot %d) Voice service state changed: regState=%d",
        //        m_slot_id, voiceRegState);
    }

    if (voiceRegState != RIL_REG_STATE_HOME && voiceRegState != RIL_REG_STATE_ROAMING) {
        mCallRat = CALL_RAT_NO_SERIVCE;
    } else {
        switch (voiceRadioTech) {
            case RADIO_TECH_GPRS:
            case RADIO_TECH_EDGE:
            case RADIO_TECH_GSM:
                mCallRat = CALL_RAT_GSM;
                break;
            case RADIO_TECH_UMTS:
            case RADIO_TECH_HSDPA:
            case RADIO_TECH_HSUPA:
            case RADIO_TECH_HSPA:
            case RADIO_TECH_HSPAP:
            case RADIO_TECH_TD_SCDMA:
                mCallRat = CALL_RAT_UMTS;
                break;
            case RADIO_TECH_IS95A:
            case RADIO_TECH_IS95B:
            case RADIO_TECH_1xRTT:
            case RADIO_TECH_EVDO_0:
            case RADIO_TECH_EVDO_A:
            case RADIO_TECH_EVDO_B:
            case RADIO_TECH_EHRPD:
                mCallRat = CALL_RAT_CDMA;
                break;
            case RADIO_TECH_LTE:
                mCallRat = CALL_RAT_LTE;
                break;
            case RADIO_TECH_IWLAN:
                mCallRat = CALL_RAT_WIFI;
                break;
            default:
                mCallRat = CALL_RAT_NONE;
                break;
            }
    }

    int dataRegState = newSS.getRilDataRegState();
    int dataRadioTech = newSS.getRilDataRadioTech();
    int wfcRegState = getStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);

    updateImsCallRat(dataRegState, dataRadioTech, wfcRegState);
}

void RtcCallController::onWFCPDNStatusChanged(RfxStatusKeyEnum key,
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

void RtcCallController::updateImsCallRat(int dataRegState, int dataRadioTech, int wfcRegState) {
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

void RtcCallController::onRadioStateChanged(RfxStatusKeyEnum key,
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

void RtcCallController::onCardTypeChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    if (oldValue.asInt() != newValue.asInt()) {
        //logD(RFX_LOG_TAG, "[%s] oldValue: %d, newValue: %d", __FUNCTION__,
        //    oldValue.asInt(), newValue.asInt());
        if ((newValue.asInt() == 0) && isCsCallExistAndNoEccExist()) {
            // When SIM plugged out, hang up the normal call directly.
            logD(RFX_LOG_TAG,"[%s], hang up normal call due to SIM plug out", __FUNCTION__);
            sp<RfxMessage> msg = RfxMessage::obtainRequest(
                    getSlotId(), RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData());
            requestToMcl(msg);
            mPendingIncomingCallId = -1;
        }
    }
}

bool RtcCallController::isCsCallExistAndNoEccExist() {
    int size = mPreciseCallStateList->size();

    if (size <= 0) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        if (mPreciseCallStateList->itemAt(i)->mCallType == CALL_TYPE_EMERGENCY) {
            return false;
        }
    }

    return true;
}

bool RtcCallController::isCsActiveOrIncomingCallExistAndNoEccExist() {
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

bool RtcCallController::isCdmaCallExist() {
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

/* IMS Call Start */
bool RtcCallController::hasImsCall(int slotId) {
    if ((mImsCalls[slotId].size() == 0) && (mEstablishingCall[slotId] == NULL)) {
        return false;
    }
    return true;
}

bool RtcCallController::eCallExists() {
    return getStatusManager()->getBoolValue(RFX_STATUS_KEY_ECALL_EXIST, false);
}

bool RtcCallController::handleIncomingCall(int slotId, RfxStringsData* data) {
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

    RIL_CALL_INFO_TYPE status = shouldRejectIncomingCall(callMode);
    if (status == MT_CALL_REJECTED || status == MT_CALL_MISSED || eCallExists()){
        approveIncoming(callId, seqNo, false);

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

    if ((params[5] != NULL) && (strlen(params[5]) != 0)) {
        ForwardNumberInfo info(callId, params[5]);
        mForwardNumbers.add(info);
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
        approveIncoming(callId, seqNo, true);
        return false;
    }
    return true;
}

void RtcCallController::handleImsDialRequest(const sp<RfxMessage>& message) {
    int slotId = message->getSlotId();
    int msg_id = message->getId();
    if (mEstablishingCall[slotId] != NULL) {
        logD(RFX_LOG_TAG, "An establishing ImsCall exist slot: %d", slotId);
        return;
    }

    String8 dialNumber = String8::format("%s", "");
    if (message->getId() == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL ||
        message->getId() == RFX_MSG_REQUEST_IMS_DIAL) {
        RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
        dialNumber = String8::format("%s", pDial->address);
    }else if (msg_id == RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL) {
        RIL_ECallReqMsg *fastEcall = (RIL_ECallReqMsg *) message->getData()->getData();
        dialNumber = String8::format("%s", fastEcall->address);
    }

    bool isEcc = false;
    if (msg_id == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL) {
        isEcc = true;
    }

    bool isVideo = false;
    if (msg_id == RFX_MSG_REQUEST_IMS_VT_DIAL ||
        msg_id == RFX_MSG_REQUEST_VT_DIAL_WITH_SIP_URI) {
        isVideo = true;
    }

    bool isConference = false;
    bool isConferenceHost = false;
    if (msg_id == RFX_MSG_REQUEST_CONFERENCE_DIAL) {
        RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
                m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));
        imsConfCtrl->handleOneKeyConference(message);
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

void RtcCallController::handleImsDialFailResponse(const sp<RfxMessage>& message) {
    imsCallEstablishFailed(message->getSlotId());

    if (message->getId() == RFX_MSG_REQUEST_CONFERENCE_DIAL) {
        RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
               m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));
        imsConfCtrl->handleOneKeyConferenceFail(message);
    }
}

bool RtcCallController::handleImsCallInfoUpdate(const sp<RfxMessage>& msg) {
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

    RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));

    // for the case 133, it will be fake disconnect create by RtcImsConferenceController
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
            RtcImsConferenceHandler::normalizeNumberFromCLIR(string(ecpiNumber));
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
                            &RtcCallController::onSendDelayCallInfoTimer),
                            ms2ns(DELAY_SEND_CALL_INFO_TIMER));
                    ret = false;
                }
            }
            targetCall->setCallState(RfxImsCallInfo::STATE_ESTABLISHED);
            getStatusManager()->setIntValue(RFX_STATUS_KEY_VOICE_CALL_INDEX, callId, true);
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
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_IMS_CALL_DISCONNECTED, true);
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
            getStatusManager()->setIntValue(RFX_STATUS_KEY_LAST_CALL_FAIL_CAUSE,
                    data->getDataLength() > 9 * sizeof(char*) ? atoi(params[9]) : CALL_FAIL_NORMAL,
                    true);
            if (mAutoAnswerImsIncoming != AUTO_ANSWER_NONE && mAutoAnswerCallId == callId) {
                mAutoAnswerImsIncoming = AUTO_ANSWER_NONE;
                mAutoAnswerCallId = CALL_ID_NONE;
            }

            int support = getFeatureVersion((char*) "DMF_TC02");
            if (support == 0 && isApShouldReportImsCallDisconnectCause()) {
                mDisconnectedImsCallIdQueue.push(callId);
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

bool RtcCallController::handleCsCallInfoUpdate(const sp<RfxMessage>& msg) {
    /* +ECPI: <call_id>, <msg_type>, <is_ibt>,
     *         <is_tch>, <dir>, <call_mode>, <number>, <type>, "<pau>", [<cause>] */
    RfxStringsData* data = (RfxStringsData*)msg->getData();
    char** params = (char**)data->getData();
    int callId = atoi(params[0]);
    int msgType = atoi(params[1]);
    int callMode = atoi(params[5]);
    int discCause = atoi(params[9]);

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
        case 6:
            getStatusManager()->setIntValue(RFX_STATUS_KEY_VOICE_CALL_INDEX, callId, true);
            break;
        case 133:
        {
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_IMS_CALL_DISCONNECTED, false);
            if ((callMode >= MIN_CDMA_CALL_MODE && callMode <= MAX_CDMA_CALL_MODE)
                    // call mode could be unknown
                    || isCdmaCallExist()) {
                getStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED, true);
            }
            getStatusManager()->setIntValue(RFX_STATUS_KEY_LAST_CALL_FAIL_CAUSE,
                    data->getDataLength() > 9 * sizeof(char*) ? atoi(params[9]) : CALL_FAIL_NORMAL,
                    true);

            int support = getFeatureVersion((char*)"DMF_TC02");
            if (mCsCallCount > 0) {
                mCsCallCount--;
            } else {
                logE(RFX_LOG_TAG, "something wrong with the call count of slot%d", m_slot_id);

                // [ALPS05232205] Due to timing issue, SIM switch has chance to happen during call
                // and then +ECPI 133 is reported from opposite slot.
                // Report CALL_STATE_CHANGED to other slot to make sure its state be updated.
                for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
                    if (i != m_slot_id) {
                        responseToRilj(RfxMessage::obtainUrc(i,
                                RFX_MSG_UNSOL_RESPONSE_CALL_STATE_CHANGED, RfxVoidData()));
                    }
                }
            }
            if ((support == 0) && isApShouldReportCsCallDisconnectCause()) {
                // ALPS04374742: Call Preference event reporting via UNSOL_OEM_HOOK_RAW
                uint8_t* rawBytes = obtainCallPrefRawBytes(callId, discCause);
                logD(RFX_LOG_TAG, "[%d] handleCsCallInfoUpdate, handle call info: %d (%d, %d)",
                        m_slot_id, msgType, callId, discCause);
                sp<RfxMessage> urc = RfxMessage::obtainUrc(
                        m_slot_id, RFX_MSG_UNSOL_OEM_HOOK_RAW, RfxRawData(rawBytes, 17));
                if (rawBytes != NULL) {
                    delete[] rawBytes;
                }
                RfxMainThread::enqueueMessage(urc);
            }
            break;
        }
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

bool RtcCallController::isInChina() {
    RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
            getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));
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

uint8_t* RtcCallController::obtainCallPrefRawBytes(int callId, int discCause) {
    uint8_t* data = new uint8_t[17];
    // oem type (OEM_RIL_HOOK_HEADER_GENERIC: 0x00)
    data[0] = 0x00;
    // message id (OEM_RIL_UNSOL_CALL_PREF: 0x04000C)
    //            (This part in little-endian format is due to customer special request)
    data[1] = 0x0C;
    data[2] = 0x00;
    data[3] = 0x04;
    // payload length (0x00000009)
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x09;
    // payload data
    data[8] = 0x01;
    data[9] = 0x02;
    data[10] = 0x04;
    data[11] = ((discCause >> 24) & 0xFF);
    data[12] = ((discCause >> 16) & 0xFF);
    data[13] = ((discCause >> 8) & 0xFF);
    data[14] = (discCause & 0xFF);
    data[15] = 0x09;
    data[16] = (callId & 0xFF);
    return data;
}

void RtcCallController::handleAutoAnswer(const sp<RfxMessage>& msg) {
    // Answer MT call when Auto Answer is enabled in Engineer Mode
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.em.hidl.auto_answer", prop, "");

    if (strncmp(prop, "1", 1) == 0) {
        logD(RFX_LOG_TAG, "handleAutoAnswer, persist.vendor.em.hidl.auto_answer: %s, msg: %d",
                prop, msg->getId());
        if (msg->getId() == RFX_MSG_UNSOL_CALL_INFO_INDICATION) {
            /* +ECPI: <call_id>, <msg_type>, <is_ibt>,
             *         <is_tch>, <dir>, <call_mode>, <number>, <type>, "<pau>", [<cause>] */
            RfxStringsData* data = (RfxStringsData*)msg->getData();
            char** params = (char**)data->getData();
            int callId = atoi(params[0]);
            int msgType = atoi(params[1]);
            int callMode = atoi(params[5]);
            if (msgType == 0) {
                if (callMode >= MIN_IMS_CALL_MODE && callMode <= MAX_IMS_CALL_MODE) {
                    if (!mCallRingIndicated[msg->getSlotId()]) {
                        // Don't send ATA until receive +CRING.
                        mAutoAnswerImsIncoming = AUTO_ANSWER_EM;
                        mAutoAnswerCallId = callId;
                        return;
                    }
                }
                mAutoAnswerTimerHandle = RfxTimer::start(RfxCallback0(this,
                        &RtcCallController::onAutoAnswerTimer), ms2ns(AUTO_ANSWER_DELAY));
            } else if (msgType == 132 || msgType == 133) {
                // Stop auto answer if already connected/disconnected
                stopAutoAnswerTimer(callId);
            }
        }
    }
}

void RtcCallController::onAutoAnswerTimer() {
    logD(RFX_LOG_TAG, "onAutoAnswerTimer");
    if (mCsCallCount > 2 || getValidImsCallCount() > 2) {
            logD(RFX_LOG_TAG, "not support calls >= 3");
    } else if (mCsCallCount > 1 || getValidImsCallCount() > 1) {
        if (mCallRat == CALL_RAT_CDMA) {
            logD(RFX_LOG_TAG, "not support CDMA multiple call auto answer");
        } else {
            requestToMcl(RfxMessage::obtainRequest(getSlotId(),
                    RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE,
                    RfxVoidData()));
        }
    } else {
       for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
           // Check other slot.
           if (i != m_slot_id) {
               int otherSlotCall = getStatusManager(i)->getIntValue(
                   RFX_STATUS_KEY_VOICE_CALL_COUNT, 0);
               // Only need to check ongoing call exist slot.
               if (otherSlotCall != 0){
                   requestToMcl(RfxMessage::obtainRequest(i, RFX_MSG_REQUEST_HANGUP_ALL,
                           RfxVoidData()));
                   logD(RFX_LOG_TAG, "delay 2s wait hangup all calls, then auto answer");
                   mAutoAnswerTimerHandle = RfxTimer::start(RfxCallback0(this,
                           &RtcCallController::onAutoAnswerTimer),
                           ms2ns(DELAY_AUTO_ANSWER_WAIT_HANGUP_All_CALL));
                   return;
               }
               break;
           }
       }
       requestToMcl(RfxMessage::obtainRequest(getSlotId(),
               RFX_MSG_REQUEST_ANSWER,
               RfxVoidData()));
    }
}

void RtcCallController::handleSrvcc(int slotId, const sp<RfxMessage>& msg) {
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

bool RtcCallController::isDuringSrvcc(int slotId) {
    return (mSrvccState[slotId] == SRVCC_STATE_STARTED || mSrvccState[slotId] == SRVCC_STATE_COMPLETED);
}


bool RtcCallController::waitCallRingForMT(const sp<RfxMessage>& msg) {
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

bool RtcCallController::handlePendingMTCallMode(const sp<RfxMessage>& msg) {
    if (mPendingCallInfoForMT[m_slot_id] == NULL) return true;

    RfxStringsData* pendingMTData = (RfxStringsData*)mPendingCallInfoForMT[m_slot_id]->getData();
    RfxStringsData* callModeData = (RfxStringsData*)msg->getData();

    char** pendingMTParams = (char**)pendingMTData->getData();
    char** callModeParams = (char**)callModeData->getData();
    int pendingMTCallId = getPendingMTCallId();
    int pendingMTCallMode = atoi(pendingMTParams[5]);

    int callModeCallId = atoi(callModeParams[0]);
    int targetCallMode = atoi(callModeParams[1]);

    RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));

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

bool RtcCallController::handleImsCallModeChange(const sp<RfxMessage>& msg) {
    /* +EIMSCMODE: <call_id>,<call_mode>,<camera_direction>,<audio_direction>,"<p_assert_uri>",<audio_codec> */

    /* +ECPI: <call_id>, <msg_type>, <is_ibt>,
     *         <is_tch>, <dir>, <call_mode>, <number>, <type>, "<pau>", [<cause>] */
    int slotId = msg->getSlotId();

    RfxStringsData* data = (RfxStringsData*)msg->getData();
    char** params = (char**)data->getData();
    int callId = atoi(params[0]);
    char* pau = params[4];

    RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));

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

void RtcCallController::handleCallRing(int slotId) {
    if (mPendingCallInfoForMT[slotId] != NULL) {
        logD(RFX_LOG_TAG, "Handle Call Ring, notify ECPI 0 for slot: %d", slotId);

        if (mAutoAnswerImsIncoming == AUTO_ANSWER_EM) {
            mAutoAnswerTimerHandle = RfxTimer::start(RfxCallback0(this,
                    &RtcCallController::onAutoAnswerTimer), ms2ns(AUTO_ANSWER_DELAY));
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

void RtcCallController::addImsCall(int slotId, RfxImsCallInfo* call) {
    logD(RFX_LOG_TAG, "Add ImsCall to slot: %d, callId: %d, isConf:%s, isConfHost:%s",
            slotId, call->getCallId(), call->isConference() ? "True" : "False",
            call->isConferenceHost() ? "True" : "False");

    Vector<RfxImsCallInfo*> calls = mImsCalls[slotId];
    calls.push_back(call);
    mImsCalls[slotId] = calls;
    getStatusManager(m_slot_id)->setImsCallInfosValue(RFX_STATUS_KEY_IMS_CALL_LIST, calls);
}

void RtcCallController::removeImsCall(int slotId, RfxImsCallInfo* call) {
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

void RtcCallController::assignImsCallId(int slotId, int callId) {
    RfxImsCallInfo* call = mEstablishingCall[slotId];
    call->setCallId(callId);
    mEstablishingCall.erase(slotId);
    addImsCall(slotId, call);
}

bool RtcCallController::updateNumberFromEcpiNumOrPau(RfxImsCallInfo* targetCall, string ecpiNumber, string pau) {
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

bool RtcCallController::updateNumberFromPau(RfxImsCallInfo* targetCall, string pau) {
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

    String8 encodedTelNumber = String8::format("%s",
            RfxRilUtils::pii(RFX_LOG_TAG, telNumber.data()));
    logD(RFX_LOG_TAG, "updateNumberFromPau() sipNumber=%s, telNumber=%s",
            RfxRilUtils::pii(RFX_LOG_TAG, sipNumber.data()),
            encodedTelNumber.string());

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

bool RtcCallController::updateDisplayNameFromPau(RfxImsCallInfo* targetCall, string pau) {
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

string RtcCallController::getFieldValueFromPau(string pau, string field) {
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

void RtcCallController::imsCallEstablishFailed(int slotId) {
    RfxImsCallInfo* call = mEstablishingCall[slotId];
    if (call == NULL) {
        logD(RFX_LOG_TAG, "imsCallEstablishFailed() no establishing call in slot: %d", slotId);
        return;
    }
    mEstablishingCall.erase(slotId);
    delete call;
}

void RtcCallController::updateIsImsCallExistToStatusManager(int slotId) {
    RfxRootController* rootController = RFX_OBJ_GET_INSTANCE(RfxRootController);
    RfxStatusManager* slotStatusMgr  = rootController->getStatusManager(slotId);
    slotStatusMgr->setBoolValue(RFX_STATUS_KEY_IMS_CALL_EXIST, hasImsCall(slotId));
    updateCallCount();
}

void RtcCallController::clearAllImsCalls(int slotId) {
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

void RtcCallController::releaseEstablishingCall(int slotId) {
    imsCallEstablishFailed(slotId);
}

void RtcCallController::onSendDelayCallInfoTimer() {
    responseToRilJAndUpdateIsImsCallExist(mDelaySendCallInfoMessage);
    mDelaySendCallInfoMessage = NULL;
}

void RtcCallController::responseToRilJAndUpdateIsImsCallExist(const sp<RfxMessage>& msg) {
    bool updateCallBeforeResponse = false;

    if (msg->getId() == RFX_MSG_UNSOL_INCOMING_CALL_INDICATION) {
        updateCallBeforeResponse = true;
    }

    sp<RfxMessage> responseMsg = msg;
    if (mOriginalDialMessage != NULL &&
           (msg->getId() == RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL ||
            msg->getId() == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL ||
            msg->getId() == RFX_MSG_REQUEST_DIAL ||
            msg->getId() == RFX_MSG_REQUEST_IMS_DIAL)) {
        // For the two scenarios
        // Convert DIAL to LOCAL_EMERGENCY_DIAL in handleDialAsEmergencyDial
        // Convert EMERGENCY_DIAL to LOCAL_EMERGENCY_DIAL or DIAL
        // original message's pId & pToken are copied, id is replaced & token is created
        // since pId is correct here, it is not necessary to change id (nice to do)
        responseMsg = RfxMessage::obtainResponse(mOriginalDialMessage->getId(), msg);
        mOriginalDialMessage = NULL;
    }

    if (updateCallBeforeResponse) {
        logD(RFX_LOG_TAG, "Update IsImsCallExist before response to RilJ");
        updateIsImsCallExistToStatusManager(msg->getSlotId());
        responseToRilj(responseMsg);
    } else {
        responseToRilj(responseMsg);
        updateIsImsCallExistToStatusManager(msg->getSlotId());
    }
}

void RtcCallController::clearCallRingCache(int slotId) {
    logD(RFX_LOG_TAG, "clearCallRingCache() slot: %d", slotId);
    mPendingCallInfoForMT.erase(slotId);
    mCallRingIndicated[slotId] = false;
}

void RtcCallController::generateImsConference(int slotId, int callId) {
    logD(RFX_LOG_TAG, "generateImsConference() slot: %d", slotId);
    RfxImsCallInfo* call = new RfxImsCallInfo(callId, RfxImsCallInfo::STATE_DIALING,
            String8::format("%s", ""), true, true, false, false, false);
    addImsCall(slotId, call);
    RtcImsConferenceController *imsConfCtrl = (RtcImsConferenceController *) findController(
            m_slot_id, RFX_OBJ_CLASS_INFO(RtcImsConferenceController));
    imsConfCtrl->handleConferenceStart();
}
/* IMS Call End */

int RtcCallController::findRingingConnection() {
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

CallType RtcCallController::getConnectionCallType(int callId) {
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

bool RtcCallController::hasPendingHangupRequest(bool isForegnd) {
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

bool RtcCallController::hasPendingHangupRequest(int hangupCallId) {
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

void RtcCallController::handleCdmaFlashRequest(const sp<RfxMessage>& message) {
    char *address = (char*)(message->getData()->getData());
    if ((address != NULL) && (strlen(address) > 0)) {
        RfxPreciseCallState* preciseCallState = new RfxPreciseCallState();

        preciseCallState->mSlot = getSlotId();
        preciseCallState->mCallId = mPreciseCallStateList->size() + 1;
        preciseCallState->mOrigState = ORIG_ACTIVE;
        preciseCallState->mCallType = CALL_TYPE_VOICE;
        preciseCallState->mCallRat = mCallRat;
        preciseCallState->mCallDir = CALL_DIR_MO;

        updatePreciseCallStateList(preciseCallState, mPreciseCallStateList);
    }
}

void RtcCallController::handleCdmaCallWait() {
    RfxPreciseCallState* preciseCallState = new RfxPreciseCallState();

    preciseCallState->mSlot = getSlotId();
    preciseCallState->mCallId = mPreciseCallStateList->size() + 1;
    preciseCallState->mOrigState = ORIG_WAITING;
    preciseCallState->mCallType = CALL_TYPE_VOICE;
    preciseCallState->mCallRat = mCallRat;
    preciseCallState->mCallDir = CALL_DIR_MT;

    updatePreciseCallStateList(preciseCallState, mPreciseCallStateList);
}

void RtcCallController::handleGetCurrentCallsResponse(const sp<RfxMessage>& message) {
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

void RtcCallController::fillForwardedNumberInfo(const sp<RfxMessage>& message) {
    int dataLen = message->getData()->getDataLength();
    int count = dataLen / sizeof(RIL_Call *);
    if (count == 0) {
        mForwardNumbers.clear();
    }
    if (mForwardNumbers.isEmpty()) {
        return;
    }
    RIL_Call ** pp_calls = (RIL_Call **) message->getData()->getData();
    for (int i = 0; i < count; i++) {
        RIL_Call *p_cur = ((RIL_Call **) pp_calls)[i];
        if (p_cur->isMT == 0) {
            continue;
        }
        for (ForwardNumberInfo info: mForwardNumbers) {
            const char *forwardedNumber = info.getForwardedNumber(p_cur->index);
            if (forwardedNumber != NULL) {
                if (p_cur->forwardedNumber == NULL) {
                    p_cur->forwardedNumber = strdup(forwardedNumber);
                }
            }
        }
    }
}

void RtcCallController::removeForwardedNumberInfo(int callId) {
    Vector<ForwardNumberInfo>::iterator it;
    for (it = mForwardNumbers.begin(); it != mForwardNumbers.end();) {
        if (it == NULL) {
            break;
        }
        if (it->getCallId() == callId) {
            it = mForwardNumbers.erase(it);
        } else {
            it++;
        }
    }
}

Vector<RfxPreciseCallState*>* RtcCallController::parsePreciseCallState(RIL_Call ** pp_calls, int count) {
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

void RtcCallController::updateDisconnecting(
        Vector<RfxPreciseCallState*>* origList, int hangupCallId) {
    int origSize = origList->size();
    for (int i = 0; i < origSize; i++) {
        if (origList->itemAt(i)->mCallId == hangupCallId) {
            origList->itemAt(i)->mOrigState = ORIG_DISCONNECTING;
            break;
        }
    }
}

void RtcCallController::updateDisconnecting(
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

void RtcCallController::updateDisconnected(
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

void RtcCallController::freePreciseCallStateList(Vector<RfxPreciseCallState*>* list) {
    if (list != NULL) {
        int size = list->size();
        for (int i = 0; i < size; i++) {
            delete list->itemAt(i);
        }
        delete list;
    }
}

void RtcCallController::updatePreciseCallStateList(
        RfxPreciseCallState* preciseCallState, Vector<RfxPreciseCallState*>* list) {
    preciseCallState->dump();
    if (preciseCallState->mOrigState != ORIG_DISCONNECTED) {
        list->add(preciseCallState);
    }
}

void RtcCallController::responseDialFailed(const sp<RfxMessage>& message) {
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
    if (mRedialCtrl != NULL) {
        mRedialCtrl->notifyRilResponse(responseMsg);
    }

    if (mOriginalDialMessage != NULL &&
           (message->getId() == RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL ||
            message->getId() == RFX_MSG_REQUEST_LOCAL_IMS_EMERGENCY_DIAL ||
            message->getId() == RFX_MSG_REQUEST_DIAL ||
            message->getId() == RFX_MSG_REQUEST_IMS_DIAL)) {
        // For the two scenarios
        // Convert DIAL to LOCAL_EMERGENCY_DIAL in handleDialAsEmergencyDial
        // Convert EMERGENCY_DIAL to LOCAL_EMERGENCY_DIAL or DIAL
        // original message's pId & pToken are copied, id is replaced & token is created
        // since pId is correct here, it is not necessary to change id (nice to do)
        responseMsg = RfxMessage::obtainResponse(mOriginalDialMessage->getId(), responseMsg);
        mOriginalDialMessage = NULL;
    }
    responseToRilj(responseMsg);
}

void RtcCallController::notifyCallInfoUpdate(const sp<RfxMessage>& message) {

    handleImsCallInfoUpdate(message);
    responseToRilJAndUpdateIsImsCallExist(message);
}

bool RtcCallController::rejectDualDialForDSDS() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (i != m_slot_id) {
            if (getStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_VOICE_CALL_COUNT, 0) > 0) {
                logD(RFX_LOG_TAG, "reject dial on slot%d, since slot%d has call",
                        m_slot_id, i);
                mUseLocalCallFailCause = true;
                mDialLastError = CALL_FAIL_ERROR_UNSPECIFIED;
                return true;
            }
        }
    }
    mUseLocalCallFailCause = false;
    mDialLastError = 0;
    return false;
}

RIL_CALL_INFO_TYPE RtcCallController::shouldRejectIncomingCall(int callMode) {
    int dsdaMode = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_DSDA_MODE, 0);
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

            RtcCallController *ctrl = (RtcCallController *) findController(
                    i, RFX_OBJ_CLASS_INFO(RtcCallController));
            bool allowImsIncoming = isIncomingImsCall && ctrl != NULL
                    && ctrl->allowImsIncomingOnAnotherSlot();

            logD(RFX_LOG_TAG, "DSDA mode:%d, allowImsIncoming:%d, imsRegistrationRat:%d,"
                    "otherSlotImsRegistrationRat:%d", dsdaMode, allowImsIncoming,
                    imsRegistrationRat, otherSlotImsRegistrationRat);

            // By pass for 5G+4G DSDA.
            if (dsdaMode == 1 && allowImsIncoming) {
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
            //Reject incoming for Terminal Based Call Waiting
            char cwMode[RFX_PROPERTY_VALUE_MAX] = { 0 };
            getMSimProperty(m_slot_id, (char*)PROPERTY_TBCW_MODE, cwMode);
            int callCount = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, 0);
            if (!strcmp(cwMode, TBCW_OFF) && (callCount > 0)) {
                logE(RFX_LOG_TAG, "Reject MT on slot%d, since slot%d has call",
                        m_slot_id, i);
                return MT_CALL_REJECTED;
            }

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
                String8 rejectMode = getStatusManager()->getString8Value(
                        RFX_STATUS_KEY_IMS_INCOMING_CALL_RULE);

                logD(RFX_LOG_TAG, "rejectMode %s", rejectMode.string());
                if (rejectMode == "1") {
                    if (callMode == RfxImsCallInfo::IMS_VIDEO ||
                            callMode == RfxImsCallInfo::IMS_VIDEO_CONF ||
                            callMode == RfxImsCallInfo::IMS_VIDEO_CONF_PARTS ) {
                        logD(RFX_LOG_TAG, "Reject MT on slot%d, since MT video call during call",
                                m_slot_id);
                        return MT_CALL_REJECTED;
                    }

                    for (int i = 0; i < (int)calls.size(); ++i) {
                        RfxImsCallInfo* call = calls[i];
                        if (call->isVideo()) {
                            logD(RFX_LOG_TAG, "Reject MT on slot%d, since during video call",
                                    m_slot_id);
                            return MT_CALL_REJECTED;
                        }
                    }
                } else if (rejectMode == "2") {
                    for (int i = 0; i < (int)calls.size(); ++i) {
                        RfxImsCallInfo* call = calls[i];
                        if (call->isConferenceHost()) {
                            logD(RFX_LOG_TAG, "Reject MT on slot%d, since during conference call",
                                    m_slot_id);
                            return MT_CALL_REJECTED;
                        }
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

void RtcCallController::approveIncoming(int callId, int seqNo, bool approve) {
    // AT+EAIC=X, callId, seqNo
    int msg_data[4];
    msg_data[0] = approve ? 0 : 1;  // 0: approve, 1: disapprove
    msg_data[1] = callId;
    msg_data[2] = seqNo;
    msg_data[3] = -1;
    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_SET_CALL_INDICATION,
            RfxIntsData(msg_data, 4));
    requestToMcl(msg);
}

void RtcCallController::onAutoApproveTimer() {
    logD(RFX_LOG_TAG, "onAutoApproveTimer callId %d, SeqNo %d", mPendingIncomingCallId, mSeqNo);
    if (mPendingIncomingCallId == -1 || mSeqNo == -1) {
        return;
    }

    // AT+EAIC=X, callId, seqNo
    int msg_data[4];
    msg_data[0] = 0;
    msg_data[1] = mPendingIncomingCallId;
    msg_data[2] = mSeqNo;
    msg_data[3] = -1;
    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_SET_CALL_INDICATION,
            RfxIntsData(msg_data, 4));
    requestToMcl(msg);
    mSeqNo = -1;
    resetImsFakeMode();
}

void RtcCallController::updateCallCount() {
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

int RtcCallController::getValidImsCallCount() {
    int validImsCallCount = mImsCalls[m_slot_id].size();
    if (mEstablishingCall[m_slot_id] != NULL) {
        ++validImsCallCount;
    }
    return validImsCallCount;
}

int RtcCallController::getPendingMTCallId() {
    int callId = -1;
    if (mPendingCallInfoForMT[m_slot_id] != NULL) {
        RfxStringsData* data = (RfxStringsData*)mPendingCallInfoForMT[m_slot_id]->getData();
        char** params = (char**)data->getData();
        callId = atoi(params[0]);
    }
    logD(RFX_LOG_TAG, "getPendingMTCallId: %d", callId);
    return callId;
}

bool RtcCallController::handleDialAsEmergencyDial(const sp<RfxMessage>& message) {
    if (message->getId() == RFX_MSG_REQUEST_DIAL || message->getId() == RFX_MSG_REQUEST_IMS_DIAL) {
        RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
        String8 dialNumber = String8::format("%s", pDial->address);
        RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
                getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));

        ssize_t index = dialNumber.find(",");
        if (index > 0) {
            dialNumber = String8(dialNumber.string(), index);
        }
        if (eccNumberController->getEmergencyCallRouting(dialNumber) == ECC_ROUTING_EMERGENCY) {
            String8 urn;
            int serviceCategory = eccNumberController->getServiceCategoryAndUrn(dialNumber, urn);
            logD(RFX_LOG_TAG, "handleDialAsEmergencyDial: %s,%d",
                    RfxRilUtils::pii(RFX_LOG_TAG, pDial->address), serviceCategory);
            requestToMcl(RfxMessage::obtainRequest(getSlotId(),
                    RFX_MSG_REQUEST_LOCAL_SET_ECC_SERVICE_CATEGORY,
                    RfxEsvcData(serviceCategory, urn)));
            return true;
        }
    }
    return false;
}

void RtcCallController::handleRedialEcc(const sp<RfxMessage>& urc) {
    /* +ERDECCIND: <call_id> */
    RfxStringsData* data = (RfxStringsData*)urc->getData();
    char** params = (char**)data->getData();
    int callId = atoi(params[0]);

    logD(RFX_LOG_TAG,"[%s], callId=%d", __FUNCTION__, callId);

    if (callId != 0 && mRedialCtrl != NULL && mDialMessage != NULL) {
        logD(RFX_LOG_TAG,"[%s], check send +EMCS for ecc redialing", __FUNCTION__);
        sp<RfxMessage> emergencyDialMessage = RfxMessage::obtainRequest(getSlotId(),
                    RFX_MSG_REQUEST_LOCAL_EMERGENCY_DIAL, mDialMessage, true);
        mRedialCtrl->notifyRilRequest(emergencyDialMessage);
        mRedialCtrl->setEccCallId(callId);
        mDialMessage = NULL;
    }
}

// ALPS04027132, IMS MT call & ECC call conflict.
// Better hang up the incoming call.
void RtcCallController::hangupIncomingCall() {
    if (mPendingIncomingCallId != -1) {
        logD(RFX_LOG_TAG,"[%s], hang up incoming call due to ECC dial", __FUNCTION__);
        sp<RfxMessage> msg = RfxMessage::obtainRequest(
                getSlotId(), RFX_MSG_REQUEST_HANGUP, RfxIntsData(&mPendingIncomingCallId, 1));
        requestToMcl(msg);
    }
}

void RtcCallController::hangupIncomingCallOnAllSlot() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++){
        RtcCallController *ctrl =
                (RtcCallController *) findController(i, RFX_OBJ_CLASS_INFO(RtcCallController));
        if (ctrl != NULL) {
            ctrl->hangupIncomingCall();
        }
    }
}

bool RtcCallController::handleSetCallIndicationRequest(const sp<RfxMessage>& message) {
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

bool RtcCallController::reportFakeImsRingingCall(const sp<RfxMessage>& msg, int seqNo) {
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

void RtcCallController::maybeSendNoEcbm(bool isForce) {
    bool emergencyMode = getStatusManager()->getBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE, false);
    if (emergencyMode == false || isForce) {
        sp<RfxMessage> urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_NO_EMERGENCY_CALLBACK_MODE,
            RfxVoidData());
        responseToRilj(urcToRilj);
    }
}

void RtcCallController::handleEmergencyBearerSupportNotify(const sp<RfxMessage>& message) {
    // +CNEMS1: <s1_support>
    int eccBearerSupport = ((int *)message->getData()->getData())[0];
    logD(RFX_LOG_TAG, "[%s], eccBearerSupport=%d", __FUNCTION__, eccBearerSupport);
    getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_EMERGENCY_BEARER_SUPPORT, eccBearerSupport);
}

void RtcCallController::handleImsGetCurrentCalls(const sp<RfxMessage>& message) {
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
        // RmcCallControlUrcHandler::resetSpeechCodec();
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

void RtcCallController::imsCallToRilCall(int &index, RfxImsCallInfo* call, RIL_Call* p_calls) {
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
    p_calls[index].speechCodec = mSpeechCodec;  // RmcCallControlUrcHandler::getSpeechCodec();

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

void RtcCallController::handleImsHangupFgResumeBg(const sp<RfxMessage>& message) {
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

void RtcCallController::checkIfResumeBg(int slotId) {
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

void RtcCallController::handleImsHangupWaitingOrBackground(const sp<RfxMessage>& message) {
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

void RtcCallController::handleImsSwitchFgBg(const sp<RfxMessage>& message) {
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

Vector<RfxImsCallInfo*> RtcCallController::getWaitingCall(Vector<RfxImsCallInfo*> calls) {
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

Vector<RfxImsCallInfo*> RtcCallController::getHoldingCall(Vector<RfxImsCallInfo*> calls) {
    Vector<RfxImsCallInfo*> holdingCalls;
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallState() == RfxImsCallInfo::STATE_HELD) {
            holdingCalls.push_back(call);
        }
    }
    return holdingCalls;
}

Vector<RfxImsCallInfo*> RtcCallController::getActiveCall(Vector<RfxImsCallInfo*> calls) {
    Vector<RfxImsCallInfo*> activeCalls;
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallState() == RfxImsCallInfo::STATE_ESTABLISHED) {
            activeCalls.push_back(call);
        }
    }
    return activeCalls;
}

RfxImsCallInfo* RtcCallController::getImsCallByCallId(Vector<RfxImsCallInfo*> calls, int callId) {
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->getCallId() == callId) {
            return call;
        }
    }
    return NULL;
}

void RtcCallController::dumpImsCallList(Vector<RfxImsCallInfo*> calls) {
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

Vector<RfxImsCallInfo*> RtcCallController::getConferenceHostCall(Vector<RfxImsCallInfo*> calls) {
    Vector<RfxImsCallInfo*> conferenceHostCalls;
    for (int i = 0; i < (int)calls.size(); i++) {
        RfxImsCallInfo* call = calls[i];
        if (call->isConferenceHost()) {
            conferenceHostCalls.push_back(call);
        }
    }
    return conferenceHostCalls;
}

void RtcCallController::handleEconfResult(const sp<RfxMessage>& message) {
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

void RtcCallController::onParticipantsUpdate(vector<sp<ConferenceCallUser>> users,
        bool autoTerminate) {
    logD(RFX_LOG_TAG, "onParticipantsUpdate autoTerminate %d", autoTerminate);
    Vector<RfxImsCallInfo*> calls = mImsCalls[m_slot_id];
    Vector<RfxImsCallInfo*> conferenceHostCalls = getConferenceHostCall(calls);
    if (conferenceHostCalls.size() > 0) {
        Vector<RfxImsCallInfo*> participants = conferenceHostCalls[0]->getParticipantList();

        // Only update when someone drop
        int participantNum = (int)participants.size();
        for (int i = 0; i < participantNum; i++) {
            RfxImsCallInfo* participant = participants[i];
            bool matched = false;
            for (int j = 0; j < (int)users.size(); j++) {
                sp<ConferenceCallUser> user = users[j];
                const char* currentNumber = participant->getNumber().string();
                if (currentNumber != NULL &&
                        MtkPhoneNumberUtils::compareLoosely(currentNumber, user->mUserAddr)) {
                    matched = true;
                    logD(RFX_LOG_TAG, "onParticipantsUpdate: match address %s, status %s",
                            RfxRilUtils::pii(RFX_LOG_TAG, user->mUserAddr.data()),
                            user->mStatus.data());
                    if (user->mStatus == RtcImsConferenceCallMessageHandler::STATUS_DISCONNECTED) {
                        conferenceHostCalls[0]->removeParticipant(participant);
                        break;  // Just remove one participant because may exist multiple participants with same address.
                    }
                }
            }
            if (matched == false) {
                logD(RFX_LOG_TAG, "onParticipantsUpdate: not match, remove call %d",
                        participant->getCallId());
                conferenceHostCalls[0]->removeParticipant(participant);
            }
        }

        dumpImsCallList(calls);
        participants = conferenceHostCalls[0]->getParticipantList();
        if (autoTerminate) {
            int callId = conferenceHostCalls[0]->getCallId();
            logD(RFX_LOG_TAG, "onParticipantsUpdate: auto terminate call %d", callId);

            int msg_data[1];
            msg_data[0] = callId;
            sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_HANGUP,
                    RfxIntsData(msg_data, 1));
            requestToMcl(msg);
            conferenceHostCalls[0]->setCallState(RfxImsCallInfo::STATE_TERMINATING);
        } else {
            sp<RfxMessage> urcToRilj = RfxMessage::obtainUrc(m_slot_id,
                    RFX_MSG_UNSOL_IMS_RESPONSE_CALL_STATE_CHANGED, RfxVoidData());
            responseToRilj(urcToRilj);
        }
    }
}

void RtcCallController::onMergeStart() {
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

void RtcCallController::handleImsStkCall(int slotId, char** params) {
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
bool RtcCallController::allowImsIncomingOnAnotherSlot() {
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

void RtcCallController::retrieveMergeFail(int activeCallIdBeforeMerge) {
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

bool RtcCallController::handleClirSpecial(const sp<RfxMessage>& message) {
    if (message->getId() == RFX_MSG_REQUEST_IMS_DIAL) {
        RIL_Dial *pDial = (RIL_Dial*) (message->getData()->getData());
        String8 dialNumber = String8::format("%s", pDial->address);
        RtcEccNumberController* eccNumberController = (RtcEccNumberController *)findController(
                getSlotId(), RFX_OBJ_CLASS_INFO(RtcEccNumberController));
        bool isEmergencyNumber = eccNumberController->isEmergencyNumber(dialNumber);
        bool sendNewImsDialRequest = false;
        String8 ignoreClirWhenEcc = getStatusManager()->getString8Value(
                RFX_STATUS_KEY_CARRIER_IGNORE_CLIR_WHEN_ECC);
        logD(RFX_LOG_TAG, "ignoreClirWhenEcc %s", ignoreClirWhenEcc.string());

        if (pDial->clir != 0 && isEmergencyNumber && (ignoreClirWhenEcc == "1")) {
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
void RtcCallController::updateRQ(int RQ) {
    if (mRQ != RQ) {
        logD(RFX_LOG_TAG, "updateRQ() : RQ   = %d", RQ);

        if (RfxGwsdUtils::getCallControlHandler() != NULL) {
            RfxGwsdUtils::getCallControlHandler()->updateRQ(this, m_slot_id, RQ==1 ? true : false);
        }
        mRQ = RQ;
    }
}

void RtcCallController::resetImsFakeMode() {
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

bool RtcCallController::onHandleRequestForImsFakeMode(const sp<RfxMessage>& message) {
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
                approveIncoming(mPendingIncomingCallId, mSeqNo, true);
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
                approveIncoming(mPendingIncomingCallId, mSeqNo, false);
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

bool RtcCallController::onHandleResponseForImsFakeMode(const sp<RfxMessage>& message) {
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
void RtcCallController::onAutoFakeAcceptTimer() {
    logD(RFX_LOG_TAG, "onAutoFakeAcceptTimer");
    mAutoFakeAcceptTimerHandle = NULL;
    if (RfxGwsdUtils::getCallControlHandler() != NULL) {
        RfxGwsdUtils::getCallControlHandler()->onAutoFakeAcceptTimer(this);
    }
}

// ECC related control
void RtcCallController::createEccController() {
    logD(RFX_LOG_TAG, "create ecc controller");
    RFX_OBJ_CREATE(mEccController, RtcEccController, this);
}

// Power on for ECC related control
void RtcCallController::onRadioPowerEccStatusChanged(RfxStatusKeyEnum key,
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
                    RfxCallback0(this, &RtcCallController::onRadioOnForEccTimeout),
                    RADIO_ON_FOR_ECC_TIMEOUT);
        } else {
            mEccController->setRadioOnForEcc(false);
        }
    }
}

void RtcCallController::onRadioOnForEccTimeout() {
    logI(RFX_LOG_TAG, "onRadioOnForEccTimeout");
    cancelRadioOnForEcc();
}

void RtcCallController::cancelRadioOnForEcc() {
    if (getEccPowerOnStatus() == true) {
        logD(RFX_LOG_TAG, "cancelRadioOnForEcc, cancel radio on for ecc.");
        setLocalEccIndication(false); // for emci supported projects, do cancel flow
        setEccPowerOnStatus(false);
    }
}

void RtcCallController::setEccPowerOnStatus(bool isOnForEcc) {
    getNonSlotScopeStatusManager()->setBoolValue(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC, isOnForEcc);
}

bool RtcCallController::getEccPowerOnStatus() {
    return getNonSlotScopeStatusManager()->getBoolValue(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC, false);
}

void RtcCallController::setLocalEccIndication(bool onoff) {
    int msg_data[1];
    msg_data[0] = (onoff ? 1 : 0);  // 1: on, 0: off

    if (RfxRilUtils::isEmciSupport()) {
        // EMCI would be handled at main protocol only.
        sp<RfxMessage> rilRequest = RfxMessage::obtainRequest(
                RfxRilUtils::getMajorSim() - 1,
                RFX_MSG_REQUEST_LOCAL_SET_ECC_INDICATION,
                RfxIntsData(msg_data, 1));
        requestToMcl(rilRequest);
    }
}


void RtcCallController::stopAutoAnswerTimer(int callId) {
    if (mAutoAnswerTimerHandle != NULL) {
        RfxTimer::stop(mAutoAnswerTimerHandle);
        mAutoAnswerTimerHandle = NULL;
    }
    if (mAutoAnswerImsIncoming == AUTO_ANSWER_EM &&
            ((mAutoAnswerCallId == callId) || (callId == CALL_ID_ALL))) {
        mAutoAnswerImsIncoming = AUTO_ANSWER_NONE;
        mAutoAnswerCallId = CALL_ID_NONE;
    }
}

bool RtcCallController::isExistDisallowHoldVideoOnOtherSub() {
    bool isExistDisallowHoldVideo = false;
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        // Check other slot.
        if (i != m_slot_id) {
            int otherSlotCall = getStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_VOICE_CALL_COUNT, 0);
            if (otherSlotCall == 0) {
                logD(RFX_LOG_TAG, "isExistDisallowHoldVideoOnOtherSub return false");
                isExistDisallowHoldVideo = false;
                break;
            }else {
                String8 videoHoldMode = getStatusManager(i)->getString8Value(
                        RFX_STATUS_KEY_IMS_INCOMING_CALL_RULE);
                if (videoHoldMode == "1") {
                    Vector<RfxImsCallInfo*> calls = getStatusManager(i)->getImsCallInfosValue(
                            RFX_STATUS_KEY_IMS_CALL_LIST, calls);
                    for (int i = 0; i < (int)calls.size(); i++) {
                        if (calls[i]->isVideo()
                                && calls[i]->getCallState() != RfxImsCallInfo::STATE_INCOMING) {
                            logD(RFX_LOG_TAG, "isExistDisallowHoldVideoOnOtherSub return true");
                            isExistDisallowHoldVideo = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    return isExistDisallowHoldVideo;
}

void RtcCallController::hangupAllCallsOnOtherSub() {
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        // Check other slot.
        if (i != m_slot_id) {
            requestToMcl(RfxMessage::obtainRequest(i, RFX_MSG_REQUEST_HANGUP_ALL, RfxVoidData()));
        }
    }
}

Vector<RfxImsCallInfo*> RtcCallController::getHoldCallOnOtherSub() {
    Vector<RfxImsCallInfo*> holdingCalls;
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        // Check other slot.
        if (i != m_slot_id) {
            int otherSlotCall = getStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_VOICE_CALL_COUNT, 0);
            if (otherSlotCall == 0) {
                logD(RFX_LOG_TAG, "getHoldCallOnOtherSub return null");
            } else {
                Vector<RfxImsCallInfo*> calls = getStatusManager(i)->getImsCallInfosValue(
                        RFX_STATUS_KEY_IMS_CALL_LIST);
                holdingCalls = getHoldingCall(calls);
            }
            break;
        }
    }
    return holdingCalls;
}

void RtcCallController::hangupHoldOnOtherSub(Vector<RfxImsCallInfo*> holdingCalls) {
    if (holdingCalls.size() != 0) {
        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            // Check other slot.
            if (i != m_slot_id) {
                int callId = holdingCalls[0]->getCallId();
                int msg_data[1];
                msg_data[0] = callId;
                logD(RFX_LOG_TAG, "hangupHoldOnOtherSub call %d", callId);
                sp<RfxMessage> msg = RfxMessage::obtainRequest(i, RFX_MSG_REQUEST_HANGUP,
                        RfxIntsData(msg_data, 1));
                requestToMcl(msg);
                holdingCalls[0]->setCallState(RfxImsCallInfo::STATE_TERMINATING);
            }
       }
    }
}

void RtcCallController::handleGetSimultaneousCallingSupport(const sp<RfxMessage>& message) {
    sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message,
            true);
    responseToRilj(responseMsg);
}