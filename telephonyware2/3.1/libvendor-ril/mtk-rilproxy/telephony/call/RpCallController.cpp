// SPDX-License-Identifier: MediaTekProprietary
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

#include "RfxMainThread.h"
#include <cutils/jstring.h>
//#include <SpeechDrv.h>
extern "C" {
    #include "mixer_ctrl.h"
}

#include "RpCallController.h"
#define LOG_TAG "RpCallController"
#include <mtk_log.h>

#define DEFAULT_CALL_DISALLOWED "0"
const char g_mixer_name[] = "Speech_on";
const char g_mixer_name_ecall[] = "Ecall_speech_on";
const char g_card_name[] = "mtk_phonecall";

/*****************************************************************************
 * Class RpCallController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RpCallController", RpCallController, RfxController);

RpCallController::RpCallController() :
 mCsPhone(0), mCallCount(0) {

}

RpCallController::~RpCallController() {
}

void RpCallController::onInit() {
    RfxController::onInit(); // Required: invoke super class implementation
    RLOGD("[RpCallController] onInit E!");

    const int request_id_list[] = {
        /* Common Request */
        RIL_REQUEST_GET_CURRENT_CALLS,
        RIL_REQUEST_UDUB
    };
    mixer_init();
    const int urc_id_list[] = {
        /* Common URC */
#if defined(TELE_FWK)
        RIL_UNSOL_INCOMING_CALL_INDICATION,
#endif
        RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED

    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(int));
    RLOGD("[RpCallController] request size:%d urc size:%d!",
        sizeof(request_id_list)/sizeof(int), sizeof(urc_id_list)/sizeof(int));
    //reset audio speech to prevent exception cases
    //speechdrv_set_speech_on(false, 0, 0);
}

void RpCallController::onDeinit() {
    RLOGD("[RpCallController] onDeinit E!");
    RfxController::onDeinit();
}

bool RpCallController::onHandleRequest(const sp<RfxMessage>& request) {
    int msg_id = request->getId();

    RLOGD("[RpCallController] handle req %d %s (slot %d) CsPhone %d", msg_id,
            requestToString(msg_id), getSlotId(), mCsPhone);

    if (RADIO_TECH_GROUP_GSM == mCsPhone) {
        requestToRild(request);
    } else {
        sp<RfxMessage> c2k_request = RfxMessage::obtainRequest(RADIO_TECH_GROUP_C2K,
                                                                msg_id, request, true);
        // fill in items in RfxMessage for c2k_request
        // such as new a parcel and attach it to c2k_request
        // NOTE. attached parcel will be free when the RfxMessage
        //       instance is destroyed, so don't attach one parcel
        //       object to more than one RfxMessage instance

        // send request to C2K RILD
        requestToRild(c2k_request);
    }

    return true;
}

bool RpCallController::onHandleResponse(const sp<RfxMessage>& response) {
    int msg_id = response->getId();
    sp<RfxMessage> rilResponse = response;

    RLOGD("[RpCallController] handle resp %d %s (slot %d)", msg_id, requestToString(msg_id),
            getSlotId());

    switch (msg_id) {
        case RIL_REQUEST_GET_CURRENT_CALLS:
            onHandleGetCurrentCalls(rilResponse);
            break;
        case RIL_REQUEST_UDUB:
        {
            RLOGD("[RpCallController][onHandleResponse]speech off");
            //speechdrv_set_speech_on(false, 0, 0);
            responseToRilj(response);
            break;
        }
        default:
            responseToRilj(response);
            break;
    }
    return true;
}

bool RpCallController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    RLOGD("[RpCallController] handle urc %d %s (slot %d)", msg_id, requestToString(msg_id),
            getSlotId());

    switch (msg_id) {
        case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED:
            onHandleCallStateChanged(message);
            break;
#if defined(TELE_FWK)
    case RIL_UNSOL_INCOMING_CALL_INDICATION:
            onHandleIncomingCallIndicate(message);
            break;
#endif
        default:
            responseToRilj(message);
            break;
    }
    return true;
}

void RpCallController::onHandleGetCurrentCalls(const sp<RfxMessage>& response) {
    bool reportToRilj = true;
    sp<RfxMessage> tmpMsg = RfxMessage::obtainResponse(response->getError(), response, true);
    tmpMsg->getParcel()->setDataPosition(3 * sizeof(int));
    int callCount = tmpMsg->getParcel()->readInt32();
    RLOGD("[RpCallController] onHandleGetCurrentCalls, callCount:%d", callCount);

    mCallCount = callCount;
    /* To notify the current call state to StatusManager */
    onNotifyCallState(tmpMsg);
    getStatusManager(getSlotId())->setIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, mCallCount);

    if (reportToRilj) {
        responseToRilj(response);
    }
}

void RpCallController::onHandleCallStateChanged(const sp<RfxMessage>& message) {

    sp<RfxMessage> rilRequest;
    rilRequest = RfxMessage::obtainRequest(message->getSlotId(),
            RADIO_TECH_GROUP_GSM, RIL_REQUEST_GET_CURRENT_CALLS);
    RfxMainThread::enqueueMessage(rilRequest);

    responseToRilj(message);
}

void RpCallController::onHandleIncomingCallIndicate(const sp<RfxMessage>& message) {

    int mode=0, call_id=0, seqnumber=0, call_number=0;
    int numStrings=0;
    size_t stringlen=0;
    const char16_t *s16 = NULL;
    char str_call_id[PROPERTY_VALUE_MAX] = {0};
    char str_seqno[PROPERTY_VALUE_MAX] = {0};
    char str_call_num[PROPERTY_VALUE_MAX] = {0};

    Parcel *payload = message->getParcel();

    payload->readInt32 (&numStrings);
    /*
    param->callId = data[0];
    param->seqNo = data[4];
    */
    /* call id*/
    s16 = payload->readString16Inplace(&stringlen);
    strnlen16to8(s16, stringlen);
    if (s16 != NULL) {
        size_t strLen = strnlen16to8(s16, stringlen);
        strncpy16to8(str_call_id, s16, strLen);
    }
    call_id = atoi(str_call_id);
    /*call number*/
    s16 = payload->readString16Inplace(&stringlen);
    strnlen16to8(s16, stringlen);
    if (s16 != NULL) {
        size_t strLen = strnlen16to8(s16, stringlen);
        strncpy16to8(str_call_num, s16, strLen);
    }
    call_number = atoi(str_call_num);

    payload->readString16Inplace(&stringlen);
    payload->readString16Inplace(&stringlen);

    /*call seq no*/
    s16 = payload->readString16Inplace(&stringlen);
    strnlen16to8(s16, stringlen);
    if (s16 != NULL) {
        size_t strLen = strnlen16to8(s16, stringlen);
        strncpy16to8(str_seqno, s16, strLen);
    }
    seqnumber = atoi(str_seqno);

    char CallAllowedKey[64];
    char prop_value[PROPERTY_VALUE_MAX] = {0};
    int retval = snprintf(CallAllowedKey, 64, "%s%s", "persist.call.disallowed.", str_call_num);
    if (retval < 0 || retval >= sizeof(CallAllowedKey)) {
        RLOGE("snprintf failed or buffer overflow in creating CallAllowedKey");
        return;
    }
    property_get(CallAllowedKey, prop_value, DEFAULT_CALL_DISALLOWED);

    mode = atoi(prop_value);

    RLOGD("onHandleIncomingCallIndicate call id= %s,%d, seqno=%s, %d,call number=%s, %d mode=%s, %d",
        str_call_id, call_id, str_seqno, seqnumber, str_call_num, call_number, prop_value, mode);

    sp<RfxMessage> rilRequest;
    rilRequest = RfxMessage::obtainRequest(message->getSlotId(),
            RADIO_TECH_GROUP_GSM, RIL_REQUEST_SET_CALL_INDICATION);

    Parcel* parcel = rilRequest->getParcel();
    // make sure get the right position
    if (parcel != NULL) {
        parcel->setDataPosition(sizeof(int32_t)*2);
        parcel->writeInt32(3);
        parcel->writeInt32(mode);
        parcel->writeInt32(call_id);
        parcel->writeInt32(seqnumber);
    }
    RfxMainThread::enqueueMessage(rilRequest);

    responseToRilj(message);
}


int RpCallController::mixer_init() {
    RLOGD("set_card_name: %s", g_card_name);
    int ret;

    // only need to set card name once
    ret = set_card_name(g_card_name);
    RLOGD("mixer_init(%s) = %d", g_card_name, ret);
    return ret;
}
void RpCallController::speechonoff(int count) {
    int old_count = getStatusManager(getSlotId())->getIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, -1);
    int ret = -1;
    RLOGD("[RpCallController][speechonoff]old_count=%d ,new_count=%d",old_count, count);
    char type[PROPERTY_VALUE_MAX];
    memset(type, 0, sizeof(type));
    property_get("ro.telephony.call.type", type, "0");
    if ((count == 1) && (old_count != count)) {
        RLOGD("[RpCallController][speechonoff]speech on");
        ret = set_mixer_ctrl_value_int(strstr(type, "0") ? g_mixer_name: g_mixer_name_ecall, 1);
    } else if ((count == 0) && (old_count != count)) {
        RLOGD("[RpCallController][speechonoff]speech off");
        ret = set_mixer_ctrl_value_int(strstr(type, "0") ? g_mixer_name: g_mixer_name_ecall, 0);
    }
}

/**
  * To notify the current call state to StatusManager.
  */
void RpCallController::onNotifyCallState(const sp<RfxMessage>& message) {
    message->getParcel()->setDataPosition(3 * sizeof(int));
    int count = message->getParcel()->readInt32();
    int state = message->getParcel()->readInt32();

    CallTypeEnum callType;
    CallStateEnum callState = CALL_STATE_UNKNOWN;
    if (count == 0) {
        callState = CALL_STATE_DISCONNECTED;
    } else if (state == 0) {     //Active
        callState = CALL_STATE_CONNECTED;
    }

    speechonoff(count);
    callType = (mCsPhone == RADIO_TECH_GROUP_C2K) ? CALL_TYPE_C2K : CALL_TYPE_UNKNOWN;

    //Todo: If the previous csPhone is C2K and the current csPhone is changed, also need to notify
    mCurrentCallState = RfxCallState(callType, callState, count);

    if ((callType != CALL_TYPE_UNKNOWN ) &&
         (callState != CALL_STATE_UNKNOWN)) {
        getStatusManager()->setCallStateValue(RFX_STATUS_KEY_CALL_STATE,
                mCurrentCallState);
    }

    RLOGD("[RpCallController][onNotifyCallState][CallState] = %s", mCurrentCallState.toString().string());
}
