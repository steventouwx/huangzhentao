/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2022. All rights reserved.
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
{RIL_REQUEST_HANGUP_ALL, mtkRadioExVoice::hangupAllResponse},
{RIL_REQUEST_HANGUP_WITH_REASON, mtkRadioExVoice::hangupWithReasonResponse},
{RIL_REQUEST_GET_CALL_SUB_ADDRESS, mtkRadioExVoice::getCallSubAddressResponse},
{RIL_REQUEST_GET_COLP, mtkRadioExVoice::getColpResponse},
{RIL_REQUEST_GET_COLR, mtkRadioExVoice::getColrResponse},
{RIL_REQUEST_GET_ECC_NUM, mtkRadioExVoice::getEccNumResponse},
{RIL_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT, mtkRadioExVoice::queryCallForwardInTimeSlotStatusResponse},
{RIL_REQUEST_RESET_SUPP_SERV, mtkRadioExVoice::resetSuppServResponse},
{RIL_REQUEST_SEND_CNAP, mtkRadioExVoice::sendCnapResponse},
{RIL_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT, mtkRadioExVoice::setCallForwardInTimeSlotResponse},
{RIL_REQUEST_SET_CALL_INDICATION, mtkRadioExVoice::setCallIndicationResponse},
{RIL_REQUEST_SET_CALL_SUB_ADDRESS, mtkRadioExVoice::setCallSubAddressResponse},
{RIL_REQUEST_SET_GWSD_CALL_VALID, mtkRadioExVoice::setCallValidTimerResponse},
{RIL_REQUEST_SET_CLIP, mtkRadioExVoice::setClipResponse},
{RIL_REQUEST_SET_COLP, mtkRadioExVoice::setColpResponse},
{RIL_REQUEST_SET_COLR, mtkRadioExVoice::setColrResponse},
{RIL_REQUEST_SET_ECC_MODE, mtkRadioExVoice::setEccModeResponse},
{RIL_REQUEST_SET_ECC_NUM, mtkRadioExVoice::setEccNumResponse},
{RIL_REQUEST_SET_GWSD_MODE, mtkRadioExVoice::setGwsdModeResponse},
{RIL_REQUEST_SET_GWSD_IGNORE_CALL_INTERVAL, mtkRadioExVoice::setIgnoreSameNumberIntervalResponse},
{RIL_REQUEST_SET_GWSD_KEEP_ALIVE_IPDATA, mtkRadioExVoice::setKeepAliveByIpDataResponse},
{RIL_REQUEST_SET_GWSD_KEEP_ALIVE_PDCP, mtkRadioExVoice::setKeepAliveByPDCPCtrlPDUResponse},
{RIL_REQUEST_SET_SS_PROPERTY, mtkRadioExVoice::setSuppServPropertyResponse},
{RIL_REQUEST_IMS_DIAL, radioVoice::dialResponse},
{RIL_REQUEST_IMS_EMERGENCY_DIAL, radioVoice::emergencyDialResponse},
