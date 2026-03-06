/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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
#include <regex>
#include <string>
#include <mtk_properties.h>
#include "NetAgentCommon.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RfxMipcData.h"
#include "RmmDcRequestHandler.h"
#include "NetAgentService.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxOpUtils.h"
#include "RmmDcUtility.h"
#include "RfxNaCbInfoData.h"
#include "mipc_api.h"

#define RFX_LOG_TAG "RmmDcRequestHandler"

/*****************************************************************************
 * Class RmmDcRequestHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmDcRequestHandler, RIL_CMD_PROXY_5);

RmmDcRequestHandler::RmmDcRequestHandler(int slot_id, int channel_id) :
    RfxBaseHandler(slot_id, channel_id),
    m_pRmmDcEventHandler(NULL),
    m_pPdnManager(NULL) {
    mIsUserBuild = RmmDcUtility::isUserBuild();
    // Init NetAgent service.
    bool bSuccess = false;
    do {
        bSuccess = NetAgentService::createNetAgentService();
        if(!bSuccess) {
            RFX_LOG_E(RFX_LOG_TAG, "Fail to create NetAgent service!");
            sleep(10);
            /* never returns */
        }
    } while (!bSuccess);

    // notify modem we will send apn list, only need send once, always by slot 0 is ok.
    if (slot_id == 0) {
        sp<RfxMipcData> msgSyncApn =
                RfxMipcData::obtainMipcData(MIPC_APN_SET_PROFILE_STATUS_REQ, slot_id);
        msgSyncApn->addMipcTlvUint8(MIPC_APN_SET_PROFILE_STATUS_REQ_T_MODE,
                MIPC_APN_PROFILE_MODE_HOST_SET_APN_PROFILE);
        callToMipcMsgSync(msgSyncApn);
    }

    /**
     * For 97 SSC Mode3 requirement on IPv6 Multi-Homing case
     *
     *    Enable : Use ipadd/ipdel instead of ipupdate sending IPv6 address to modem.
     *    Disable: Keep using ipupdate as legacy mode sending IPv6 address to modem.
     */
    NetAgentService *pNetAgentService = NetAgentService::getInstance();
    sp<NetActionBase1> action;
    if (NULL == pNetAgentService) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d] NetAgentService is NULL, return error", m_slot_id);
    } else {
        action = new NetAction1(this, &RmmDcRequestHandler::handleInterfaceInfoUpdated);
        pNetAgentService->registerInterfaceInfoChanged(action, m_slot_id);
    }

    // Create Pdn manager first and pass to request handler as parameter.
    RFX_HANDLER_CREATE(m_pPdnManager, RmmDcPdnManager, (m_slot_id, m_channel_id));
    RFX_HANDLER_CREATE(m_pRmmDcEventHandler, RmmDcEventHandler, (m_slot_id, m_channel_id, m_pPdnManager));

    /**
     * For 93 VZW requirement
     *
     *    Enable : Set Attach PDN to VZWINTERNET via +ECNCFG,,,1
     *    Disable: Set Attach PDN to VZWIMS (Default)
     */
    char imsTestMode[MTK_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.imstestmode", imsTestMode, "0");
    RFX_LOG_D(RFX_LOG_TAG, "Property: imsTestMode=%s", imsTestMode);
    if (atoi(imsTestMode) == 1) {
        sp<RfxMipcData> msgImsTestMode =
                RfxMipcData::obtainMipcData(MIPC_DATA_SET_CONFIG_REQ, slot_id);
        msgImsTestMode->addMipcTlvUint8(MIPC_DATA_SET_CONFIG_REQ_T_IMS_TEST_MODE,
                MIPC_DATA_CONFIG_TYPE_ENABLE);
        callToMipcMsgSync(msgImsTestMode);
    }
    /// @}

    const int requestList[] = {
        RFX_MSG_REQUEST_SETUP_DATA_CALL,
        RFX_MSG_REQUEST_SETUP_DATA_CALL_SLICE,
        RFX_MSG_REQUEST_DEACTIVATE_DATA_CALL,
        RFX_MSG_REQUEST_DATA_CALL_LIST,
        RFX_MSG_REQUEST_SET_DATA_PROFILE,
        RFX_MSG_REQUEST_SET_INITIAL_ATTACH_APN,
        RFX_MSG_REQUEST_SYNC_DATA_SETTINGS_TO_MD,
        RFX_MSG_REQUEST_RESET_MD_DATA_RETRY_COUNT,
        RFX_MSG_REQUEST_RESET_ALL_CONNECTIONS,
        RFX_MSG_REQUEST_SET_PREFERRED_DATA_MODEM,
        RFX_MSG_REQUEST_START_KEEPALIVE,
        RFX_MSG_REQUEST_STOP_KEEPALIVE,
        RFX_MSG_REQUEST_SET_FD_MODE,
        RFX_MSG_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA,
        RFX_MSG_REQUEST_SEND_DEVICE_STATE,
        RFX_MSG_REQUEST_UPDATE_DATA_RAT,
        RFX_MSG_REQUEST_DATA_CONNECTION_ATTACH,
        RFX_MSG_REQUEST_DATA_CONNECTION_DETACH,
        RFX_MSG_REQUEST_ENABLE_DSDA_INDICATION,
        RFX_MSG_REQUEST_GET_DSDA_STATUS,
        RFX_MSG_REQUEST_GET_SLICING_CONFIG,
        RFX_MSG_REQUEST_UPDATE_SLICING_CONFIG,
        RFX_MSG_REQUEST_ENABLE_NETWORK_REJECT_CAUSE_IND,
        RFX_MSG_REQUEST_RELEASE_RF_OCCUPY,
        RFX_MSG_REQUEST_RESET_ALL_CONNECTIONS_LOCAL,
    };

    registerToHandleRequest(requestList, sizeof(requestList) / sizeof(int));
}

RmmDcRequestHandler::~RmmDcRequestHandler() {
}

void RmmDcRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_SETUP_DATA_CALL:
            // fall through
        case RFX_MSG_REQUEST_SETUP_DATA_CALL_SLICE:
            handleSetupDataCallRequest(msg);
            break;
        case RFX_MSG_REQUEST_DEACTIVATE_DATA_CALL:
            handleDeactivateDataCallRequest(msg);
            break;
        case RFX_MSG_REQUEST_DATA_CALL_LIST:
            handleDataCallListRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_DATA_PROFILE:
            handleSyncApnTableRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_INITIAL_ATTACH_APN:
            handleSetInitialAttachApnRequest(msg);
            break;
        case RFX_MSG_REQUEST_SYNC_DATA_SETTINGS_TO_MD:
            handleSyncDataSettingsToMdRequest(msg);
            break;
        case RFX_MSG_REQUEST_RESET_MD_DATA_RETRY_COUNT:
            handleResetMdDataRetryCount(msg);
            break;
        case RFX_MSG_REQUEST_RESET_ALL_CONNECTIONS:
            handleResetAllConnections(msg);
            break;
        case RFX_MSG_REQUEST_SET_PREFERRED_DATA_MODEM:
            handleSetPreferredDataModem(msg);
            break;
        case RFX_MSG_REQUEST_START_KEEPALIVE:
            handleStartKeepaliveRequest(msg);
            break;
        case RFX_MSG_REQUEST_STOP_KEEPALIVE:
            handleStopKeepaliveRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_FD_MODE:
            handleSetFdModeRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA:
            handleSetLinkCapacityReportingCriteriaRequest(msg);
            break;
        case RFX_MSG_REQUEST_SEND_DEVICE_STATE:
            handleSendDeviceStateRequest(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_DATA_RAT:
            handleUpdateDataRatRequest(msg);
            break;
        case RFX_MSG_REQUEST_DATA_CONNECTION_ATTACH:
            handleDcAttachDetachRequest(msg, DATA_CONNECTION_ATTACH);
            break;
        case RFX_MSG_REQUEST_DATA_CONNECTION_DETACH:
            handleDcAttachDetachRequest(msg, DATA_CONNECTION_DETACH);
            break;
        case RFX_MSG_REQUEST_ENABLE_DSDA_INDICATION:
            handleEnableDsdaIndication(msg);
            break;
        case RFX_MSG_REQUEST_GET_DSDA_STATUS:
            handleGetDsdaStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_SLICING_CONFIG:
            handleGetSlicingConfig(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_SLICING_CONFIG:
            handleUpdateSlicingConfig(msg);
            break;
        case RFX_MSG_REQUEST_ENABLE_NETWORK_REJECT_CAUSE_IND:
            hanleEnableNetworkRejectCauseIndication(msg);
            break;
        case RFX_MSG_REQUEST_RELEASE_RF_OCCUPY:
            handleReleaseRfOccupy(msg);
            break;
        case RFX_MSG_REQUEST_RESET_ALL_CONNECTIONS_LOCAL:
            handleResetAllConnections(msg);
            break;
        default:
            RFX_LOG_D(RFX_LOG_TAG, "unknown request, ignore!");
            break;
    }
}

void RmmDcRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_RESET_MD_DATA_RETRY_COUNT:
            m_pRmmDcEventHandler->handleResetMdDataRetryCountResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_DSDA_STATUS:
            m_pRmmDcEventHandler->handleDsdaStatusResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_SLICING_CONFIG:
        case RFX_MSG_REQUEST_UPDATE_SLICING_CONFIG:
            m_pRmmDcEventHandler->handleGetSliceConfigPlmnListResponse(msg);
            break;
        default:
            RFX_LOG_D(RFX_LOG_TAG, "ignore response: %d", msg->getId());
            break;
    }
}

void RmmDcRequestHandler::handleDataCallListRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestOrSendDataCallList(msg, ACTIVE_STATUS_AND_PROFILE_UPDATE_TYPE_ALL);
}

void RmmDcRequestHandler::handleSetupDataCallRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestSetupDataCall(msg);
}

void RmmDcRequestHandler::handleDeactivateDataCallRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestDeactivateDataCall(msg);
}

void RmmDcRequestHandler::handlePdnChange(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->onPdnChange(msg);
}

void RmmDcRequestHandler::handleSyncApnTableRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestSyncApnTable(msg);
}

void RmmDcRequestHandler::handleSetInitialAttachApnRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestSetInitialAttachApn(msg);
}

void RmmDcRequestHandler::handleSyncDataSettingsToMdRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestSyncDataSettingsToMd(msg);
}

void RmmDcRequestHandler::handleResetMdDataRetryCount(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestResetMdDataRetryCount(msg);
}

void RmmDcRequestHandler::handleResetAllConnections(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleResetAllConnections(msg);
}

void RmmDcRequestHandler::handleSetPreferredDataModem(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestSetPreferredDataModem(msg);
}

void RmmDcRequestHandler::handleStartKeepaliveRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestStartKeepalive(msg);
}

void RmmDcRequestHandler::handleStopKeepaliveRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestStopKeepalive(msg);
}

void RmmDcRequestHandler::handleInterfaceInfoUpdated(bool bSuccess, void* data) {
    CallbackInfo *p_cbInfo = (CallbackInfo*)data;
    if (bSuccess == false) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] failed.", m_slot_id, __FUNCTION__);
        return;
    }
    String8 info("");
    int globalIpv6Count = p_cbInfo->interfaceInfo->globalIpv6Count;
    info.append(String8::format("success=%d, reason=%d, interfaceId=%d, addressV4=%s, "
            "mtuV4=%d, mtuv6=%d, globalIpv6Count=%d, ",
            bSuccess, p_cbInfo->reason, p_cbInfo->interfaceInfo->interfaceId,
            mIsUserBuild ? "xxx" : p_cbInfo->interfaceInfo->addressV4,
            p_cbInfo->interfaceInfo->mtuV4, p_cbInfo->interfaceInfo->mtuV6,
            p_cbInfo->interfaceInfo->globalIpv6Count));
    for (int i = 0; i < globalIpv6Count; i++) {
        info.append(String8::format(
                "Ipv6Info[%d]:{addressV6=%s, prefixLength=%d, flags=%d, "
                "deprecationTime=%ld, expirationTime=%ld}, ",
                i,
                mIsUserBuild ? "xxx" : p_cbInfo->interfaceInfo->ipv6Info[i].addressV6,
                p_cbInfo->interfaceInfo->ipv6Info[i].prefixLength,
                p_cbInfo->interfaceInfo->ipv6Info[i].flags,
                p_cbInfo->interfaceInfo->ipv6Info[i].deprecationTime,
                p_cbInfo->interfaceInfo->ipv6Info[i].expirationTime));
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] data:%s", m_slot_id, __FUNCTION__, info.string());
    sendEvent(RFX_MSG_EVENT_KERNEL_ADDRESS_UPDATED,
            RfxNaCbInfoData(p_cbInfo, sizeof(*p_cbInfo)),
            RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcRequestHandler::handleSetFdModeRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestSetFdMode(msg);
}

void RmmDcRequestHandler::handleSetLinkCapacityReportingCriteriaRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->requestSetLinkCapacityReportingCriteria(msg);
}

void RmmDcRequestHandler::handleSendDeviceStateRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleSendDeviceStateRequest(msg);
}

void RmmDcRequestHandler::handleEnableDsdaIndication(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleEnableDsdaIndication(msg);
}

void RmmDcRequestHandler::handleGetDsdaStatus(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleGetDsdaStatus(msg);
}

void RmmDcRequestHandler::handleUpdateDataRatRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleUpdateDataRatRequest(msg);
}

/// Multi-SIM Start
void RmmDcRequestHandler::handleDcAttachDetachRequest(const sp<RfxMclMessage>& msg, int type) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleDcAttachDetachRequest(msg, type);
}
/// Multi-SIM end @}

void RmmDcRequestHandler::handleGetSlicingConfig(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleGetSlicingConfig(msg);
}

void RmmDcRequestHandler::handleUpdateSlicingConfig(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->handleUpdateSlicingConfig(msg);
}

void RmmDcRequestHandler::hanleEnableNetworkRejectCauseIndication(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->hanleEnableNetworkRejectCauseIndication(msg);
}

void RmmDcRequestHandler::handleReleaseRfOccupy(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmmDcEventHandler != NULL);
    m_pRmmDcEventHandler->onReleaseRfOccupy(msg);
}
