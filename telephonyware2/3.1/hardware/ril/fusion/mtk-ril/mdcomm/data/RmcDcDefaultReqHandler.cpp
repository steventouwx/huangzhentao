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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "NetAgentService.h"

#include "RmcDcDefaultReqHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"

// See AOSP define in DataConnection.java OS_ID
#define ANDROID_OS_ID "97a498e3fc925c9489860333d06e4e47"

/*****************************************************************************
 * Class RmcDcDefaultReqHandler
 *****************************************************************************/
RmcDcDefaultReqHandler::RmcDcDefaultReqHandler(int slot_id, int channel_id, RmcDcPdnManager* pdnManager)
: RmcDcCommonReqHandler(slot_id, channel_id, pdnManager) {
    // set Android OS ID for URSP registration
    // MUST set before modem radio on.
    atSendCommand(String8::format("AT+EOSID=1,\"%s\"", ANDROID_OS_ID));
}

RmcDcDefaultReqHandler::~RmcDcDefaultReqHandler() {
}

void RmcDcDefaultReqHandler::requestSetupDataCall(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestSetupDataCall(msg);
}

void RmcDcDefaultReqHandler::requestDeactivateDataCall(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestDeactivateDataCall(msg);
}

void RmcDcDefaultReqHandler::onNwPdnAct(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onNwPdnAct(msg);
}

void RmcDcDefaultReqHandler::onNwPdnDeact(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onNwPdnDeact(msg);
}

void RmcDcDefaultReqHandler::onNwAct(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onNwAct(msg);
}

void RmcDcDefaultReqHandler::onNwDeact(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onNwDeact(msg);
}

void RmcDcDefaultReqHandler::onNwModify(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onNwModify(msg);
}

void RmcDcDefaultReqHandler::onNwReact(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onNwReact(msg);
}

void RmcDcDefaultReqHandler::onMePdnAct(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onMePdnAct(msg);
}

void RmcDcDefaultReqHandler::onMePdnDeact(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onMePdnDeact(msg);
}

void RmcDcDefaultReqHandler::onPdnChange(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onPdnChange(msg);
}

void RmcDcDefaultReqHandler::onQualifiedNetworkTypeChanged(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onQualifiedNetworkTypeChanged(msg);
}

void RmcDcDefaultReqHandler::requestSyncApnTable(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestSyncApnTable(msg);
}

void RmcDcDefaultReqHandler::requestSyncDataSettingsToMd(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestSyncDataSettingsToMd(msg);
}

void RmcDcDefaultReqHandler::requestResetMdDataRetryCount(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestResetMdDataRetryCount(msg);
}

void RmcDcDefaultReqHandler::requestOrSendDataCallList(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestOrSendDataCallList(msg);
}

void RmcDcDefaultReqHandler::requestOrSendDataCallList(const sp<RfxMclMessage>& msg, Vector<int> *vAidList) {
    RmcDcCommonReqHandler::requestOrSendDataCallList(msg, vAidList);
}

void RmcDcDefaultReqHandler::requestOrSendDataCallList(const sp<RfxMclMessage>& msg, int aid) {
    RmcDcCommonReqHandler::requestOrSendDataCallList(msg, aid);
}

void RmcDcDefaultReqHandler::requestQueryPco(int aid, int ia, const char* apn, const char* iptype) {
    RmcDcCommonReqHandler::requestQueryPco(aid, ia, apn, iptype);
}

void RmcDcDefaultReqHandler::requestClearAllPdnInfo(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestClearAllPdnInfo(msg);
}

void RmcDcDefaultReqHandler::requestResendSyncDataSettingsToMd(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestResendSyncDataSettingsToMd(msg);
}

void RmcDcDefaultReqHandler::handleResetAllConnections(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::handleResetAllConnections(msg);
}

void RmcDcDefaultReqHandler::requestSetPreferredDataModem(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestSetPreferredDataModem(msg);
}

void RmcDcDefaultReqHandler::requestGetDataContextIds(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestGetDataContextIds(msg);
}

void RmcDcDefaultReqHandler::requestSendQualifiedNetworkTypesChanged(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestSendQualifiedNetworkTypesChanged(msg);
}

void RmcDcDefaultReqHandler::mobileDataUsageNotify(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::mobileDataUsageNotify(msg);
}

void RmcDcDefaultReqHandler::requestStartKeepalive(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestStartKeepalive(msg);
}

void RmcDcDefaultReqHandler::requestStopKeepalive(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::requestStopKeepalive(msg);
}

void RmcDcDefaultReqHandler::updateKeepaliveStatus(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::updateKeepaliveStatus(msg);
}

void RmcDcDefaultReqHandler::onNwLimitNotify(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onNwLimitNotify(msg);
}

void RmcDcDefaultReqHandler::onKernelAddressUpdated(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onKernelAddressUpdated(msg);
}

void RmcDcDefaultReqHandler::updateMtuV6FromKernel(int interfaceId, int mtuV6) {
    RmcDcCommonReqHandler::updateMtuV6FromKernel(interfaceId, mtuV6);
}

void RmcDcDefaultReqHandler::onUrspReevaluate(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onUrspReevaluate(msg);
}

void RmcDcDefaultReqHandler::onDataRatUpdated(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onDataRatUpdated(msg);
}

void RmcDcDefaultReqHandler::onAttachApnChanged(const char* apn) {
    RmcDcCommonReqHandler::onAttachApnChanged(apn);
}

void RmcDcDefaultReqHandler::onReleaseRfOccupy(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::onReleaseRfOccupy(msg);
}

void RmcDcDefaultReqHandler::releaseRfOccupyTimeout(const sp<RfxMclMessage>& msg) {
    RmcDcCommonReqHandler::releaseRfOccupyTimeout(msg);
}
