/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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
{RIL_REQUEST_ANSWER, radioVoice::acceptCallResponse},
{RIL_REQUEST_CONFERENCE, radioVoice::conferenceResponse},
{RIL_REQUEST_DIAL, radioVoice::dialResponse},
{RIL_REQUEST_EMERGENCY_DIAL, radioVoice::emergencyDialResponse},
{RIL_REQUEST_EXPLICIT_CALL_TRANSFER, radioVoice::explicitCallTransferResponse},
{RIL_REQUEST_QUERY_CALL_FORWARD_STATUS, radioVoice::getCallForwardStatusResponse},
{RIL_REQUEST_QUERY_CALL_WAITING, radioVoice::getCallWaitingResponse},
{RIL_REQUEST_QUERY_CLIP, radioVoice::getClipResponse},
{RIL_REQUEST_GET_CLIR, radioVoice::getClirResponse},
{RIL_REQUEST_GET_CURRENT_CALLS, radioVoice::getCurrentCallsResponse},
{RIL_REQUEST_LAST_CALL_FAIL_CAUSE, radioVoice::getLastCallFailCauseResponse},
{RIL_REQUEST_GET_MUTE, radioVoice::getMuteResponse},
{RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE, radioVoice::getPreferredVoicePrivacyResponse},
{RIL_REQUEST_QUERY_TTY_MODE, radioVoice::getTTYModeResponse},
{RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM, radioVoice::handleStkCallSetupRequestFromSimResponse},
{RIL_REQUEST_HANGUP, radioVoice::hangupConnectionResponse},
{RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND, radioVoice::hangupForegroundResumeBackgroundResponse},
{RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND, radioVoice::hangupWaitingOrBackgroundResponse},
{RIL_REQUEST_IS_VONR_ENABLED, radioVoice::isVoNrEnabledResponse},
{RIL_REQUEST_UDUB, radioVoice::rejectCallResponse},
{RIL_REQUEST_CDMA_BURST_DTMF, radioVoice::sendBurstDtmfResponse},
{RIL_REQUEST_CDMA_FLASH, radioVoice::sendCDMAFeatureCodeResponse},
{RIL_REQUEST_DTMF, radioVoice::sendDtmfResponse},
{RIL_REQUEST_SEPARATE_CONNECTION, radioVoice::separateConnectionResponse},
{RIL_REQUEST_SET_CALL_FORWARD, radioVoice::setCallForwardResponse},
{RIL_REQUEST_SET_CALL_WAITING, radioVoice::setCallWaitingResponse},
{RIL_REQUEST_SET_CLIR, radioVoice::setClirResponse},
{RIL_REQUEST_SET_MUTE, radioVoice::setMuteResponse},
{RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE, radioVoice::setPreferredVoicePrivacyResponse},
{RIL_REQUEST_SET_TTY_MODE, radioVoice::setTTYModeResponse},
{RIL_REQUEST_ENABLE_VONR, radioVoice::setVoNrEnabledResponse},
{RIL_REQUEST_DTMF_START, radioVoice::startDtmfResponse},
{RIL_REQUEST_DTMF_STOP, radioVoice::stopDtmfResponse},
{RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE, radioVoice::switchWaitingOrHoldingAndActiveResponse},
{RIL_REQUEST_SEND_USSD, radioVoice::sendUssdResponse},
{RIL_REQUEST_CANCEL_USSD, radioVoice::cancelPendingUssdResponse},
{RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE, radioVoice::exitEmergencyCallbackModeResponse},
