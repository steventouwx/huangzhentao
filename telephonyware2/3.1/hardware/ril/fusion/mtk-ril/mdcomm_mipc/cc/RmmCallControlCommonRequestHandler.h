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

#ifndef __RMM_CALL_CONTROL_COMMON_REQUEST_HANDLER_H__
#define __RMM_CALL_CONTROL_COMMON_REQUEST_HANDLER_H__

#include <utils/KeyedVector.h>

#include "RmmCallControlBaseHandler.h"

class RmmCallControlImsRequestHandler;

class RmmCallControlCommonRequestHandler : public RmmCallControlBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmCallControlCommonRequestHandler);

    public:
        RmmCallControlCommonRequestHandler(int slot_id, int channel_id);
        virtual ~RmmCallControlCommonRequestHandler();

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);
        virtual void onHandleEvent(const sp < RfxMclMessage > & msg);

    private:
        int requestGetCurrentCalls(const sp<RfxMclMessage>& msg);
        void handleCurrentCallsCnf(const sp<RfxMclMessage>& msg);
        int mipcCallStateToRilType(int state, RIL_CallState *p_state);
        int requestDial(const sp<RfxMclMessage>& msg, bool isEcc, bool isVt, bool isImsDial);
        int requestAnswer(const sp<RfxMclMessage>& msg);
        int requestUdub(const sp<RfxMclMessage>& msg);
        int requestDtmf(const sp<RfxMclMessage>& msg);
        int requestDtmfStart(const sp<RfxMclMessage>& msg);
        int requestDtmfStop(const sp<RfxMclMessage>& msg);
        void requestSetMute(const sp<RfxMclMessage>& msg);
        void requestGetMute(const sp<RfxMclMessage>& msg);
        int requestSetTtyMode(const sp<RfxMclMessage>& msg);
        int requestQueryTtyMode(const sp<RfxMclMessage>& msg);
        void handleQueryTtyModeCnf(const sp<RfxMclMessage>& msg);
        int requestLastCallFailCause(const sp<RfxMclMessage>& msg);
        void handleLastCallFailCauseCnf(const sp<RfxMclMessage>& msg);
        void requestSetCallIndication(const sp<RfxMclMessage>& msg);
        int updateAutoAnswerSetting(const sp<RfxMclMessage>& msg);

        int requestSsAction(const sp<RfxMclMessage>& msg, uint32_t action);
        int requestHangupAll(const sp<RfxMclMessage>& msg);
        int requestHangup(const sp<RfxMclMessage>& msg);
        void handleHangupCnf(const sp<RfxMclMessage>& msg);
        int requestHangupWaitingOrBackground(const sp<RfxMclMessage>& msg);
        int requestHangupForegroundResumeBackground(const sp<RfxMclMessage>& msg);
        int requestSwitchWaitingOrHoldingAndActive(const sp<RfxMclMessage>& msg);
        int requestConference(const sp<RfxMclMessage>& msg);
        int requestExplicitCallTransfer(const sp<RfxMclMessage>& msg);
        int requestSeparateConnection(const sp<RfxMclMessage>& msg);
        int requestHangupWithReason(const sp<RfxMclMessage>& msg);
        int requestForceReleaseCall(const sp<RfxMclMessage>& msg);
        void updateCallDataPreferSetting();
        int requestLocalSetEccIndication(const sp<RfxMclMessage>& msg);

        /// C2K specific start
        int requestExitEmergencyCallbackMode(const sp<RfxMclMessage>& msg);
        void requestSetPreferredVoicePrivacyMode(const sp<RfxMclMessage>& msg);
        void requestQueryPreferredVoicePrivacyMode(const sp<RfxMclMessage>& msg);
        int requestFlash(const sp<RfxMclMessage>& msg);
        int requestBurstDtmf(const sp<RfxMclMessage>& msg);
        /// C2K specific end

        bool isNumberIncludePause(char* number);
        bool isValidDialString(const char* dialString);

        int bUseLocalCallFailCause = 0;
        int dialLastError = 0;
        String8 mForwardedNumber;
        String8 mForwardedNumberBuf[MAX_GSMCALL_CONNECTIONS];

        int requestSetCallSubAddress(const sp<RfxMclMessage>& msg);
        int requestGetCallSubAddress(const sp<RfxMclMessage>& msg);
        void handleGetCallSubAddressCnf(const sp<RfxMclMessage>& msg);

        /// IMS specific start
        int requestOperateImsConference(const sp<RfxMclMessage>& msg, int request);
        int requestControlCall(const sp<RfxMclMessage>& msg, int request);
        void handleControlCallCnf(const sp<RfxMclMessage>& msg);
        int requestDialWithSipUri(const sp<RfxMclMessage>& msg, bool isVt);
        int requestConferenceDial(const sp<RfxMclMessage>& msg);
        int requestAnswerVideoCall(const sp<RfxMclMessage>& msg);
        int requestApproveEccRedial(const sp<RfxMclMessage>& msg);
        int requestPullCall(const sp<RfxMclMessage>& msg);
        int requestImsECT(const sp<RfxMclMessage>& msg);
        int requestSetRttMode(const sp<RfxMclMessage>& msg);
        int requestSetSipHeader(const sp<RfxMclMessage>& msg);
        int requestEnableSipHeaderReport(const sp<RfxMclMessage>& msg);
        int requestSetImsCallMode(const sp<RfxMclMessage>& msg);
        int requestSetImsCallAdditionalInfo(const sp<RfxMclMessage>& msg);
        int requestSetVoiceDomainPreference(const sp<RfxMclMessage>& msg);
        int requestGetVoiceDomainPreference(const sp<RfxMclMessage>& msg);
        void handleGetVoiceDomainPreferenceCnf(const sp<RfxMclMessage>& msg);
        /// IMS specific end
};

#endif
