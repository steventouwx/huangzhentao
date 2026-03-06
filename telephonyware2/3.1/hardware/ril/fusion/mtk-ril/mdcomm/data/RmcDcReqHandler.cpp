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
#include <regex>
#include <string>
#include <mtk_properties.h>
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RmcDcReqHandler.h"
#include "NetAgentService.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxOpUtils.h"
#include "RmcDcUtility.h"
#include "RfxNetlinkIpv6InfoData.h"

#define RFX_LOG_TAG "RmcDcReqHandler"

/*****************************************************************************
 * Class RmcDcReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcDcReqHandler, RIL_CMD_PROXY_5);

RmcDcReqHandler::RmcDcReqHandler(int slot_id, int channel_id) :
    RfxBaseHandler(slot_id, channel_id),
    m_pRmcDcDefaultReqHdlr(NULL),
    m_pRmcDcImsReqHdlr(NULL),
    m_pRmcDcOnDemandReqHdlr(NULL),
    m_pPdnManager(NULL),
    iaMode(-1) {
    // Init NetAgent service.
    bool bSuccess = false;
    do {
        bSuccess = NetAgentService::createNetAgentService();
        if (RfxRilUtils::getRilRunMode() == RIL_RUN_MODE_MOCK) {
            break;
        }
        if(!bSuccess) {
            RFX_LOG_E(RFX_LOG_TAG, "Fail to create NetAgent service!");
            sleep(10);
            /* never returns */
        }
    } while (!bSuccess);

    /**
     * For 97 SSC Mode3 requirement on IPv6 Multi-Homing case
     *
     *    Enable : Use ipadd/ipdel instead of ipupdate sending IPv6 address to modem.
     *    Disable: Keep using ipupdate as legacy mode sending IPv6 address to modem.
     */
    char multiHomingFeature[] = "MD_MultiHoming";
    int multiHomingSupportVersion = getFeatureVersion(multiHomingFeature);
    char mdrsraFeature[] = "MD_RSRA";
    int mdRsraFeatureSupportVersion = getFeatureVersion(mdrsraFeature);
    char mdrsraLegacyVersion[] = "MD_RSRAV3";
    int mdrsraV3Supported = getFeatureVersion(mdrsraLegacyVersion);
    char mdrsraPrivacyVersion[] = "IP6PRIVACY";
    int mdrsraPrivacySupported = getFeatureVersion(mdrsraPrivacyVersion);

    RFX_LOG_D(RFX_LOG_TAG, "[%d] Property: MD_MultiHoming=%d, MD_RSRA=%d, MDRSRAV3=%d, PRIVACY=%d",
            m_slot_id, multiHomingSupportVersion, mdRsraFeatureSupportVersion,
            mdrsraV3Supported, mdrsraPrivacySupported);

    bool mdrsraSupport = RmcDcUtility::isMdrsraSupport(mdRsraFeatureSupportVersion,
                                                                        mdrsraV3Supported);
    if (mdrsraSupport) {
        atSendCommand("AT+EMDRSRA=1");
    }
    if (mdrsraPrivacySupported == 1) {
        atSendCommand("AT+EIP6PRIVACY=1");
    }

    if (RmcDcUtility::isXfrmSupport()) {
        sp<RfxAtResponse> response = atSendCommand(String8::format("AT+EXFRMEPDG=1"));
        if (response == NULL || response->isAtResponseFail()) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] xfrm support error, should never happen!",
                    m_slot_id, __FUNCTION__);
        }
    }

    NetAgentService *pNetAgentService = NetAgentService::getInstance();
    sp<NetActionBase1> action;
    if (NULL == pNetAgentService) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d] NetAgentService is NULL, return error", m_slot_id);
    } else {
        pNetAgentService->setMultiHomingFeatureSupport(multiHomingSupportVersion);
        if (mdrsraSupport) {
            int mdrsraBitmask = MDRSRA_SUPPORT_BITMASK;
            if (mdrsraV3Supported == 1) {
                mdrsraBitmask |= MDRSRA_LEGACY_VERSION_BITMASK;
            }
            if (mdrsraPrivacySupported == 1) {
                mdrsraBitmask |= MDRSRA_PRIVACY_BITMASK;
            }
            pNetAgentService->setMdRsraFeatureSupport(mdrsraBitmask);
        }
        action = new NetAction1(this, &RmcDcReqHandler::handleKernelAddressUpdatedCallback);
        pNetAgentService->registerCbKernelAddressUpdated(action, m_slot_id);
    }

    // Create Pdn manager first and pass to request handler as parameter.
    RFX_HANDLER_CREATE(m_pPdnManager, RmcDcPdnManager, (m_slot_id, m_channel_id));
    RFX_HANDLER_CREATE(m_pRmcDcDefaultReqHdlr, RmcDcDefaultReqHandler, (m_slot_id, m_channel_id, m_pPdnManager));
    RFX_HANDLER_CREATE(m_pRmcDcOnDemandReqHdlr, RmcDcOnDemandReqHandler, (m_slot_id, m_channel_id, m_pPdnManager));

    if (RfxOpUtils::getOpHandler() != NULL) {
        m_pRmcDcImsReqHdlr = (RmcDcImsReqHandler *)RfxOpUtils::getDcImsOpHandler(slot_id, channel_id, m_pPdnManager);
    } else {
        RFX_HANDLER_CREATE(m_pRmcDcImsReqHdlr, RmcDcImsReqHandler, (m_slot_id, m_channel_id, m_pPdnManager));
    }

    /**
     * For 93 VZW requirement
     *
     *    Enable : Set Attach PDN to VZWINTERNET via +ECNCFG,,,1
     *    Disable: Set Attach PDN to VZWIMS (Default)
     */
    char imsTestMode[MTK_PROPERTY_VALUE_MAX] = {0};
    char imsTestModefeature[] = "IMS_TEST_MODE";
    int imsTestModesupport = getFeatureVersion(imsTestModefeature);
    rfx_property_get("persist.vendor.radio.imstestmode", imsTestMode, "0");
    RFX_LOG_D(RFX_LOG_TAG,
            "Property: imsTestMode=%s, MD Feature: IMS_TEST_MODE=%d", imsTestMode, imsTestModesupport);
    if (atoi(imsTestMode) == 1 && imsTestModesupport == 1) {
        RFX_LOG_D(RFX_LOG_TAG, "Set imstestmode");
        atSendCommand(String8::format("AT+ECNCFG=,,,%s", imsTestMode));
    }
    /// @}

    /**
     * For 97 Ia
     * Full Parameter Support:
     * AT+EIAAPN=<apn>,<apn_index>,<pdp_type>,<roaming_pdp_type>,<auth_type>,<username>,<password>
     *
     * MD Preferred APN
     * AP set EIAMDPREFER=1 to MD
     */
    int iaFullParameterSupport = 0;
    int iaMdPreferredApnSupport = 0;
    char iaFullParameterFeature[] = "Full IA Parameter Supported";
    char iaMdPreferredApnFeature[] = "IA MD Preferred APN";
    iaFullParameterSupport = getFeatureVersion(iaFullParameterFeature, 0);
    iaMdPreferredApnSupport = getFeatureVersion(iaMdPreferredApnFeature, 0);
    if (iaFullParameterSupport == 0 && iaMdPreferredApnSupport == 0) {
        // Gen93/95 mode
        iaMode = 0;
    } else if (iaFullParameterSupport == 1 && iaMdPreferredApnSupport == 1) {
        // Gen97 mode: full parameter support and MD preferred apn support
        iaMode = 1;
    } else if (iaFullParameterSupport == 1 && iaMdPreferredApnSupport == 0) {
        // Only support full parameter.
        iaMode = 2;
    } else {
        // Only MD preferred apn.
        iaMode = 3;
    }
    RFX_LOG_D(RFX_LOG_TAG, "iaFullParameterSupport = %d, iaMdPreferredApnSupport = %d, iaMode = %d",
            iaFullParameterSupport, iaMdPreferredApnSupport, iaMode);
    /// @}

    const int requestList[] = {
        RFX_MSG_REQUEST_SETUP_DATA_CALL,
        RFX_MSG_REQUEST_DEACTIVATE_DATA_CALL,
        RFX_MSG_REQUEST_DATA_CALL_LIST,
        RFX_MSG_REQUEST_SET_DATA_PROFILE,
        RFX_MSG_REQUEST_SET_INITIAL_ATTACH_APN,
        RFX_MSG_REQUEST_SYNC_DATA_SETTINGS_TO_MD,
        RFX_MSG_REQUEST_RESET_MD_DATA_RETRY_COUNT,
        RFX_MSG_REQUEST_CLEAR_ALL_PDN_INFO,
        RFX_MSG_REQUEST_RESEND_SYNC_DATA_SETTINGS_TO_MD,
        RFX_MSG_REQUEST_RESET_ALL_CONNECTIONS,
        RFX_MSG_REQUEST_SET_PREFERRED_DATA_MODEM,
        RFX_MSG_REQUEST_START_KEEPALIVE,
        RFX_MSG_REQUEST_STOP_KEEPALIVE,
        RFX_MSG_REQUEST_UPDATE_DATA_RAT,
        RFX_MSG_REQUEST_RELEASE_RF_OCCUPY,
    };
    const int eventList[] = {
        RFX_MSG_EVENT_DATA_NW_PDN_ACT,
        RFX_MSG_EVENT_DATA_NW_PDN_DEACT,
        RFX_MSG_EVENT_DATA_NW_ACT,
        RFX_MSG_EVENT_DATA_NW_DEACT,
        RFX_MSG_EVENT_DATA_NW_MODIFY,
        RFX_MSG_EVENT_DATA_NW_REACT,
        RFX_MSG_EVENT_DATA_ME_PDN_ACT,
        RFX_MSG_EVENT_DATA_ME_PDN_DEACT,
        RFX_MSG_EVENT_DATA_PDN_CHANGE,
        RFX_MSG_EVENT_DATA_UT_TEST,
        RFX_MSG_EVENT_QUALIFIED_NETWORK_TYPES_CHANGED,
        RFX_MSG_EVENT_ATTACH_APN_CHANGED,
        RFX_MSG_EVENT_QUERY_PCO_WITH_URC,
        RFX_MSG_EVENT_QUERY_PCO_WITH_AID,
        RFX_MSG_EVENT_REQ_DATA_CONTEXT_IDS,
        RFX_MSG_EVENT_SEND_QUALIFIED_NETWORK_TYPES_CHANGED,
        RFX_MSG_EVENT_QUERY_SELF_IA,
        RFX_MSG_EVENT_MOBILE_DATA_USAGE_NOTIFY,
        RFX_MSG_EVENT_KEEPALIVE_STATUS,
        RFX_MSG_EVENT_DATA_NW_LIMIT_NOTIFY,
        RFX_MSG_EVENT_KERNEL_ADDRESS_UPDATED,
        RFX_MSG_EVENT_URSP_REEVALUATE,
        RFX_MSG_EVENT_RF_OCCUPY_TIMEOUT,
    };
    registerToHandleRequest(requestList, sizeof(requestList) / sizeof(int));
    registerToHandleEvent(eventList, sizeof(eventList) / sizeof(int));
}

RmcDcReqHandler::~RmcDcReqHandler() {
}

void RmcDcReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_SETUP_DATA_CALL:
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
        case RFX_MSG_REQUEST_CLEAR_ALL_PDN_INFO:
            handleClearAllPdnInfoRequest(msg);
            break;
        case RFX_MSG_REQUEST_RESEND_SYNC_DATA_SETTINGS_TO_MD:
            handleResendSyncDataSettingsToMd(msg);
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
        case RFX_MSG_REQUEST_UPDATE_DATA_RAT:
            handleUpdateDataRatRequest(msg);
            break;
        case RFX_MSG_REQUEST_RELEASE_RF_OCCUPY:
            handleReleaseRfOccupy(msg);
            break;
        default:
            RFX_LOG_D(RFX_LOG_TAG, "unknown request, ignore!");
            break;
    }
}

void RmcDcReqHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_EVENT_DATA_NW_PDN_ACT:
            handleNwPdnAct(msg);
            break;
        case RFX_MSG_EVENT_DATA_NW_PDN_DEACT:
            handleNwPdnDeact(msg);
            break;
        case RFX_MSG_EVENT_DATA_NW_ACT:
            handleNwAct(msg);
            break;
        case RFX_MSG_EVENT_DATA_NW_DEACT:
            handleNwDeact(msg);
            break;
        case RFX_MSG_EVENT_DATA_NW_MODIFY:
            handleNwModify(msg);
            break;
        case RFX_MSG_EVENT_DATA_NW_REACT:
            handleNwReact(msg);
            break;
        case RFX_MSG_EVENT_DATA_ME_PDN_ACT:
            handleMePdnAct(msg);
            break;
        case RFX_MSG_EVENT_DATA_ME_PDN_DEACT:
            handleMePdnDeact(msg);
            break;
        case RFX_MSG_EVENT_DATA_PDN_CHANGE:
            handlePdnChange(msg);
            break;
        case RFX_MSG_EVENT_DATA_UT_TEST:
            handleUtTest(msg);
            break;
        // Support IWLAN AP-ASSISTED mode
        case RFX_MSG_EVENT_QUALIFIED_NETWORK_TYPES_CHANGED:
            handleQualifiedNetworkTypeChanged(msg);
            break;
        case RFX_MSG_EVENT_ATTACH_APN_CHANGED:
            handleAttachApnChanged(msg);
            break;
        case RFX_MSG_EVENT_QUERY_PCO_WITH_URC:
        case RFX_MSG_EVENT_QUERY_PCO_WITH_AID:
            handleQueryPco(msg);
            break;
        case RFX_MSG_EVENT_REQ_DATA_CONTEXT_IDS:
            handleGetDataContextIds(msg);
            break;
        case RFX_MSG_EVENT_SEND_QUALIFIED_NETWORK_TYPES_CHANGED:
            handleSendQualifiedNetworkTypesChanged(msg);
            break;
        case RFX_MSG_EVENT_QUERY_SELF_IA:
            handleIfVendorSelfIaNeeded(msg);
            break;
        case RFX_MSG_EVENT_MOBILE_DATA_USAGE_NOTIFY:
            handleMobileDataUsageNotify(msg);
            break;
        case RFX_MSG_EVENT_KEEPALIVE_STATUS:
            handleKeepaliveStatus(msg);
            break;
        case RFX_MSG_EVENT_DATA_NW_LIMIT_NOTIFY:
            handleNwLimitNotify(msg);
            break;
        case RFX_MSG_EVENT_KERNEL_ADDRESS_UPDATED:
            handleKernelAddressUpdated(msg);
            break;
        case RFX_MSG_EVENT_URSP_REEVALUATE:
            handleUrspReevaluate(msg);
            break;
        case RFX_MSG_EVENT_RF_OCCUPY_TIMEOUT:
            handleReleaseRfOccupyTimeout(msg);
            break;
        default:
            RFX_LOG_E(RFX_LOG_TAG, "should not be here");
            break;
    }
}

void RmcDcReqHandler::handleDataCallListRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestOrSendDataCallList(msg);
}

void RmcDcReqHandler::handleSetupDataCallRequest(const sp<RfxMclMessage>& msg) {
    RIL_SetupDataCallRequest *pReq = (RIL_SetupDataCallRequest*)msg->getData()->getData();
    int nProfileId = pReq->dataProfileInfo.profileId;
    if (RIL_DATA_PROFILE_DEFAULT == nProfileId) {
        RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
        m_pRmcDcDefaultReqHdlr->requestSetupDataCall(msg);
    } else {
        RFX_ASSERT(m_pRmcDcOnDemandReqHdlr != NULL);
        m_pRmcDcOnDemandReqHdlr->requestSetupDataCall(msg);
    }
}

void RmcDcReqHandler::handleDeactivateDataCallRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestDeactivateDataCall(msg);
}

void RmcDcReqHandler::handleNwPdnAct(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onNwPdnAct(msg);
}

void RmcDcReqHandler::handleNwPdnDeact(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onNwPdnDeact(msg);
}

void RmcDcReqHandler::handleNwAct(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onNwAct(msg);
}

void RmcDcReqHandler::handleNwDeact(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onNwDeact(msg);
}

void RmcDcReqHandler::handleNwModify(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onNwModify(msg);
}

void RmcDcReqHandler::handleNwReact(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onNwReact(msg);
}

void RmcDcReqHandler::handleMePdnAct(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onMePdnAct(msg);
}

void RmcDcReqHandler::handleMePdnDeact(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onMePdnDeact(msg);
}

void RmcDcReqHandler::handlePdnChange(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onPdnChange(msg);
}

void RmcDcReqHandler::handleSyncApnTableRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestSyncApnTable(msg);
}

// Check whether the initial attach apn is in the apn list.
bool RmcDcReqHandler::isValidInitialAttachApn(const char* apn) {
    if (m_slot_id >= MAX_SIM_COUNT) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] : invalid m_slot_id = %d",
                m_slot_id, __FUNCTION__, m_slot_id);
        return false;
    }
    for (int i = 0; i < RmcDcCommonReqHandler::s_nLastReqNum[m_slot_id]; i++) {
        RIL_MtkDataProfileInfo *profile = &RmcDcCommonReqHandler::s_LastApnTable[m_slot_id][i];
        if (strcmp(apn, profile->apn) == 0) {
            return true;
        }
    }

    return false;
}

bool RmcDcReqHandler::isSupportSelfSetInitialAttach() {
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] : cardType = %d", m_slot_id, __FUNCTION__, cardType);
    if (m_slot_id >= MAX_SIM_COUNT || cardType == 0) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] : cardType = %d, SIM not inserted, return",
                m_slot_id, __FUNCTION__, cardType);
        return false;
    }

    int eccApn = 0;
    int imsApn = 0;
    int size = RmcDcCommonReqHandler::s_nLastReqNum[m_slot_id];
    for (int i = 0; i < size; i++) {
        RIL_MtkDataProfileInfo *profile = &RmcDcCommonReqHandler::s_LastApnTable[m_slot_id][i];
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] : [%d] of apn, supportedTypesBitmask = %d",
                m_slot_id, __FUNCTION__, i, profile->supportedTypesBitmask);
        if (profile->supportedTypesBitmask == RIL_APN_TYPE_EMERGENCY) eccApn++;
        if (profile->supportedTypesBitmask == RIL_APN_TYPE_IMS) imsApn++;
    }

    // android framework default will not send ia apn if only emergency/ims apn exist,
    // so we need trigger modem auto select one so that it can register LTE normally.
    // before android S0, framework will auto generate one emergency apn,
    // since android S0, framework will auto generate ims apn too,
    // some cases there maybe two emergency apns, like one "" for cellular, one "sos" for wifi.
    // android T0 will not generate ims apn, so only two emergency apns is possible.
    if ((size == 1 && eccApn) ||
            (size == 2 && eccApn == 2) ||
            (size == 2 && eccApn && imsApn) ||
            (size == 3 && eccApn == 2 && imsApn == 1)) {
        return true;
    }

    return false;
}

void RmcDcReqHandler::handleIfVendorSelfIaNeeded(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    sp<RfxAtResponse> pAtResponse;
    if (isSupportSelfSetInitialAttach()) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] : Send EIAAPN, iaMode = %d",
                    m_slot_id, __FUNCTION__, iaMode);
        RIL_MtkDataProfileInfo *profile = &RmcDcCommonReqHandler::s_LastApnTable[m_slot_id][0];
        switch (iaMode) {
            case 0:
                // Gen93/95 mode
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] : send EIAAPN, apn = %s",
                        m_slot_id, __FUNCTION__, profile->apn);
                pAtResponse = atSendCommand(
                        String8::format("AT+EIAAPN=\"%s\",%d", profile->apn, 0));
                break;
            case 1:
                // Gen97 mode: full parameter support and MD preferred apn support
                pAtResponse = atSendCommand(String8::format("AT+EIAMDPREFER=1"));
                break;
            case 2:
                // Only support full parameter.
                RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] : send EIAAPN=\"%s\",%d,\"%s\",\"%s\",%d",
                        m_slot_id, __FUNCTION__,
                        profile->apn, 0, profile->protocol, profile->roamingProtocol,
                        RmcDcUtility::getAuthType(profile->authType));
                pAtResponse = atSendCommand(
                        String8::format("AT+EIAAPN=\"%s\",%d,\"%s\",\"%s\",%d,\"%s\",\"%s\"",
                        profile->apn, 0, profile->protocol, profile->roamingProtocol,
                        RmcDcUtility::getAuthType(profile->authType), profile->user,
                        profile->password));
                break;
            case 3:
                // Only support MD preferred apn.
                pAtResponse = atSendCommand(String8::format("AT+EIAMDPREFER=1"));
                break;
            default:
                RFX_LOG_E(RFX_LOG_TAG, "iaMode = %d, should not be here", iaMode);
                break;
        }
        if (pAtResponse == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] : iaMode = %d, pAtResponse = null",
                    m_slot_id, __FUNCTION__, iaMode);
        } else if (pAtResponse->isAtResponseFail()) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] : send IA MD return failed", m_slot_id, __FUNCTION__);
        } else {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] : send IA ok", m_slot_id, __FUNCTION__);
        }
    }
}

void RmcDcReqHandler::handleSetInitialAttachApnRequest(const sp<RfxMclMessage>& msg) {
    RIL_InitialAttachApn_v15 *iaData = (RIL_InitialAttachApn_v15 *)msg->getData()->getData();
    sp<RfxAtResponse> pAtResponse;
    sp<RfxMclMessage> pMclResponse;
    ApnTableReq apnTableReq = {NULL, NULL, NULL, 0, NULL, 0};
    int cmdIndex = 0;
    char apn[MAX_APN_NAME_LENGTH] = {0};

    if (!isValidInitialAttachApn(iaData->apn)) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] invalid apn %s, let modem select it",
                m_slot_id, __FUNCTION__, iaData->apn);
        if (iaMode == 1 || iaMode == 3) {
            pAtResponse = atSendCommand(String8::format("AT+EIAMDPREFER=1"));
            if (pAtResponse == NULL || pAtResponse->isAtResponseFail()) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] : AT+EIAMDPREFER=1 response error",
                        m_slot_id, __FUNCTION__);
                pMclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                        RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
            } else {
                pMclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                        RIL_E_SUCCESS, RfxVoidData(), msg, false);
            }
        } else {
            pMclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                    RIL_E_SUCCESS, RfxVoidData(), msg, false);
        }
        responseToTelCore(pMclResponse);
        return;
    }

    asprintf(&apnTableReq.apn, "%s", iaData->apn);
    asprintf(&apnTableReq.username, "%s", iaData->username);
    asprintf(&apnTableReq.password, "%s", iaData->password);
    apnTableReq.apnTypeId = iaData->supportedTypesBitmask;
    asprintf(&apnTableReq.protocol, "%s", iaData->protocol);
    apnTableReq.authtype = iaData->authtype;
    cmdIndex = RmcDcCommonReqHandler::getCmdIndexFromApnTable(m_slot_id, &apnTableReq);

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] cmdIndex = %d", m_slot_id, __FUNCTION__, cmdIndex);

    strncpy(apn, iaData->apn, MAX_APN_NAME_LENGTH-1);
    if (strlen(iaData->apn) >= MAX_APN_NAME_LENGTH-1) {
        RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] iaData->apn length: %lu",
                m_slot_id, __FUNCTION__, strlen(iaData->apn));
        apn[MAX_APN_NAME_LENGTH-1] = '\0';
    }
    RmcDcCommonReqHandler::addEscapeSequence(apn);
    // Gen97 FULL IA Parameter to MD
    switch (iaMode) {
        case 0:
            // Gen93/95 mode
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] : send EIAAPN, apn = %s, id = %d",
                    m_slot_id, __FUNCTION__, apn, cmdIndex);
            pAtResponse = atSendCommand(String8::format("AT+EIAAPN=\"%s\",%d", apn, cmdIndex));
            break;
        case 1:
            // Gen97 mode: full parameter support and MD preferred apn support
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] : send EIAAPN=\"%s\",%d,\"%s\",\"%s\",%d",
                    m_slot_id, __FUNCTION__,
                    apn, 0, iaData->protocol, iaData->roamingProtocol,
                    RmcDcUtility::getAuthType(iaData->authtype));
            pAtResponse = atSendCommand(
                    String8::format("AT+EIAAPN=\"%s\",%d,\"%s\",\"%s\",%d,\"%s\",\"%s\"",
                    apn, 0, iaData->protocol, iaData->roamingProtocol,
                    RmcDcUtility::getAuthType(iaData->authtype), iaData->username,
                    iaData->password));
            break;
        case 2:
            // Only support full parameter.
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] : send EIAAPN=\"%s\",%d,\"%s\",\"%s\",%d",
                    m_slot_id, __FUNCTION__,
                    apn, 0, iaData->protocol, iaData->roamingProtocol,
                    RmcDcUtility::getAuthType(iaData->authtype));
            pAtResponse = atSendCommand(
                    String8::format("AT+EIAAPN=\"%s\",%d,\"%s\",\"%s\",%d,\"%s\",\"%s\"",
                    apn, 0, iaData->protocol, iaData->roamingProtocol,
                    RmcDcUtility::getAuthType(iaData->authtype), iaData->username,
                    iaData->password));
            break;
        case 3:
            // Only support MD preferred apn.
            RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] : send EIAAPN, apn = %s, id = %d",
                    m_slot_id, __FUNCTION__, apn, cmdIndex);
            pAtResponse = atSendCommand(String8::format("AT+EIAAPN=\"%s\",%d", apn, cmdIndex));
            break;
        default:
            RFX_LOG_E(RFX_LOG_TAG, "iaMode = %d, should not be here", iaMode);
            break;
    }

    if (pAtResponse == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "iaMode = %d, pAtResponse = null", iaMode);
        pMclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
    }
    else if (pAtResponse->isAtResponseFail()) {
        pMclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
    } else {
        pMclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_SUCCESS, RfxVoidData(), msg, false);
    }

    FREEIF(apnTableReq.apn);
    FREEIF(apnTableReq.username);
    FREEIF(apnTableReq.password);
    FREEIF(apnTableReq.protocol);

    // response to TeleCore
    responseToTelCore(pMclResponse);
}

void RmcDcReqHandler::handleSyncDataSettingsToMdRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestSyncDataSettingsToMd(msg);
}

void RmcDcReqHandler::handleResetMdDataRetryCount(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestResetMdDataRetryCount(msg);
}

void RmcDcReqHandler::handleUtTest(const sp<RfxMclMessage>& msg) {
    RfxAtLine *pLine = NULL;
    char *strParam = NULL;
    char *urc = (char*)msg->getData()->getData();
    int rid = m_slot_id;
    int aid = INVALID_AID;
    int err = 0;
    NetAgentService *pNetAgentService = NetAgentService::getInstance();
    int transIntfId = INVALID_TRANS_INTF_ID;

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] receive %s", rid, __FUNCTION__, urc);

    if (m_pPdnManager == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR due to PdnManager is NULL",
                rid, __FUNCTION__);
        return;
    }

    pLine = new RfxAtLine(urc, NULL);
    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    strParam = pLine->atTokNextstr(&err);
    if (err < 0){
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing string param.",
                rid, __FUNCTION__);
        AT_LINE_FREE(pLine);
        return;
    }

    if (strParam != NULL && strlen(strParam) > 0) {
        if (strncmp(CLEAR_ALL_PDN_TABLE, strParam, strlen(CLEAR_ALL_PDN_TABLE)) == 0) {
            m_pPdnManager->clearAllPdnInfo();
        } else if (strncmp(CLEAR_PDN_TABLE, strParam, strlen(CLEAR_PDN_TABLE)) == 0) {
            strParam += strlen(CLEAR_PDN_TABLE);
            aid = RmcDcUtility::stringToBinaryBase(strParam, 10, &err);
            if (err < 0) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting string aid"
                        " to binary, err = %d", rid, __FUNCTION__, err);
                AT_LINE_FREE(pLine);
                return;
            }
            m_pPdnManager->clearPdnInfo(aid);
        } else if (strncmp(SET_TRANSACTION_INTERFACE_ID, strParam,
                strlen(SET_TRANSACTION_INTERFACE_ID)) == 0) {
            if (NULL == pNetAgentService) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NetAgentService is NULL, return error",
                        rid, __FUNCTION__);
            } else {
                strParam += strlen(SET_TRANSACTION_INTERFACE_ID);
                transIntfId = RmcDcUtility::stringToBinaryBase(strParam, 10, &err);
                if (err < 0) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting string transIntfId"
                            " to binary, err = %d", rid, __FUNCTION__, err);
                    AT_LINE_FREE(pLine);
                    return;
                }
                pNetAgentService->setTransactionInterfaceId(transIntfId);
            }
        } else if (strncmp(REMOVE_TRANSACTION_INTERFACE_ID, strParam,
                strlen(REMOVE_TRANSACTION_INTERFACE_ID)) == 0) {
            if (NULL == pNetAgentService) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NetAgentService is NULL, return error",
                        rid, __FUNCTION__);
            } else {
                strParam += strlen(REMOVE_TRANSACTION_INTERFACE_ID);
                transIntfId = RmcDcUtility::stringToBinaryBase(strParam, 10, &err);
                if (err < 0) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when converting string transIntfId"
                            " to binary, err = %d", rid, __FUNCTION__, err);
                    AT_LINE_FREE(pLine);
                    return;
                }
                pNetAgentService->removeTransactionInterfaceId(transIntfId);
            }
        } else if (strncmp(REMOVE_ALL_TRANSACTION_INTERFACE_ID, strParam,
                strlen(REMOVE_ALL_TRANSACTION_INTERFACE_ID)) == 0) {
            if (NULL == pNetAgentService) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NetAgentService is NULL, return error",
                        rid, __FUNCTION__);
            } else {
                pNetAgentService->removeAllTransactionInterfaceId();
            }
        } else if (strncmp("AT", strParam, strlen("AT")) == 0) {
            atSendCommand("AT");
        } else if (strncmp("INIT AT CMDS", strParam, strlen("INIT AT CMDS")) == 0) {
            m_pPdnManager->initAtCmds();
        }
    }

    AT_LINE_FREE(pLine);
}

void RmcDcReqHandler::handleClearAllPdnInfoRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestClearAllPdnInfo(msg);
}

void RmcDcReqHandler::handleResendSyncDataSettingsToMd(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestResendSyncDataSettingsToMd(msg);
}

void RmcDcReqHandler::handleQualifiedNetworkTypeChanged(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onQualifiedNetworkTypeChanged(msg);
}

void RmcDcReqHandler::handleAttachApnChanged(const sp<RfxMclMessage>& msg) {
    std::string apn;
    std::string urc;
    std::smatch m;
    std::string prefixR2 = "EIAREG: ME ATTACH";
    static std::regex r2(prefixR2 + " *\"([^\"]+)\", *(\\w+), *(\\d+)");
    static std::regex r2a(prefixR2 + " *\"([^\"]+)\\.mnc\\d+\\.mcc\\d+[^\"]+\", *(\\w+), *(\\d+)");

    urc = std::string((char*)msg->getData()->getData());
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc %s", m_slot_id, __FUNCTION__, urc.c_str());

    std::regex_search(urc, m, r2a);
    if (m.empty() || m.size() != 4) {
        std::regex_search(urc, m, r2);
    }

    if (m.empty() || m.size() != 4) {
        for (std::size_t n = 0; n < m.size(); ++n) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] match error [%s]",
                m_slot_id, __FUNCTION__, m.str(n).c_str());
        }
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ME ATTACH content error, #matched %d, return",
            m_slot_id, __FUNCTION__, (int) m.size());
        return;
    } else {
        apn = std::string(m.str(1));
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] apn %s", m_slot_id, __FUNCTION__, apn.c_str());
        m_pRmcDcDefaultReqHdlr->onAttachApnChanged(apn.c_str());
    }
}

void RmcDcReqHandler::handleQueryPco(const sp<RfxMclMessage>& msg) {
    int msgId = msg->getId();
    int idx = INVALID_AID;
    int ia = 0;
    std::string apn;
    std::string iptype;
    std::string urc;
    std::smatch m;
    std::string prefixR1 = "CGEV: ME MODIFY";
    std::string prefixR2 = "EIAREG: ME ATTACH";
    static std::regex r1(prefixR1 + " *(\\d+)");
    static std::regex r2(prefixR2 + " *\"([^\"]+)\", *(\\w+), *(\\d+)");
    static std::regex r2a(prefixR2 + " *\"([^\"]+)\\.mnc\\d+\\.mcc\\d+[^\"]+\", *(\\w+), *(\\d+)");

    if (msgId == RFX_MSG_EVENT_QUERY_PCO_WITH_AID) {
        idx = *((int*) msg->getData()->getData());
    } else if (msgId == RFX_MSG_EVENT_QUERY_PCO_WITH_URC) {
        urc = std::string((char*)msg->getData()->getData());

        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc %s", m_slot_id, __FUNCTION__, urc.c_str());
        if (urc.find(prefixR1) != std::string::npos) {
            std::regex_search(urc, m, r1);
            ia = 0;

            if (m.empty() || m.size() != 2) {
                for (std::size_t n = 0; n < m.size(); ++n) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] match error [%s]",
                        m_slot_id, __FUNCTION__, m.str(n).c_str());
                }
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ME MODIFY content error, return",
                    m_slot_id, __FUNCTION__);
                return;
            } else {
                idx = std::stoi(m.str(1), NULL, 10);
            }
        } else if (urc.find(prefixR2) != std::string::npos) {
            std::regex_search(urc, m, r2a);
            if (m.empty() || m.size() != 4) {
                std::regex_search(urc, m, r2);
            }
            ia = 1;

            if (m.empty() || m.size() != 4) {
                for (std::size_t n = 0; n < m.size(); ++n) {
                    RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] match error [%s]",
                        m_slot_id, __FUNCTION__, m.str(n).c_str());
                }
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ME ATTACH content error, #matched %d, return",
                    m_slot_id, __FUNCTION__, (int) m.size());
                return;
            } else {
                apn = std::string(m.str(1));
                iptype = std::string(m.str(2));
                idx = std::stoi(m.str(3), NULL, 10);

                RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] apn %s, iptype %s, apn index %d",
                    m_slot_id, __FUNCTION__, apn.c_str(), iptype.c_str(), idx);
            }
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] urc format error, return",
                m_slot_id, __FUNCTION__);
            return;
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] msgid error, return",
            m_slot_id, __FUNCTION__);
        return;
    }

    m_pRmcDcDefaultReqHdlr->requestQueryPco(idx, ia, apn.c_str(), iptype.c_str());
}

void RmcDcReqHandler::handleResetAllConnections(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->handleResetAllConnections(msg);
}

void RmcDcReqHandler::handleSetPreferredDataModem(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestSetPreferredDataModem(msg);
}

void RmcDcReqHandler::handleGetDataContextIds(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestGetDataContextIds(msg);
}

void RmcDcReqHandler::handleSendQualifiedNetworkTypesChanged(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestSendQualifiedNetworkTypesChanged(msg);
}

void RmcDcReqHandler::handleMobileDataUsageNotify(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->mobileDataUsageNotify(msg);
}

void RmcDcReqHandler::handleStartKeepaliveRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestStartKeepalive(msg);
}

void RmcDcReqHandler::handleStopKeepaliveRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->requestStopKeepalive(msg);
}

void RmcDcReqHandler::handleKeepaliveStatus(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->updateKeepaliveStatus(msg);
}

void RmcDcReqHandler::handleNwLimitNotify(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onNwLimitNotify(msg);
}

void RmcDcReqHandler::handleKernelAddressUpdated(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onKernelAddressUpdated(msg);
}

void RmcDcReqHandler::handleKernelAddressUpdatedCallback(bool bSuccess, void *data) {
    MTK_RIL_NetlinkIpv6Info *netInfo = (MTK_RIL_NetlinkIpv6Info*)data;
    if (bSuccess == false) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] success=0, m_pNetAgentCallback1 is null or invalid slotId",
                m_slot_id, __FUNCTION__);
        return;
    }

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] success=%d, interfaceId=%d, address=%s, prefixLength=%d, "
            "flags=%d, deprecationTime=%ld, expirationTime=%ld, mtuV6=%d, notify=%d",
            m_slot_id, __FUNCTION__, bSuccess, netInfo->interfaceId, netInfo->address,
            netInfo->prefixLength, netInfo->flags, netInfo->deprecationTime,
            netInfo->expirationTime, netInfo->mtuV6, netInfo->notify);
    m_pRmcDcDefaultReqHdlr->updateMtuV6FromKernel(netInfo->interfaceId, netInfo->mtuV6);
    sendEvent(RFX_MSG_EVENT_KERNEL_ADDRESS_UPDATED,
            RfxNetlinkIpv6InfoData(netInfo, sizeof(*netInfo)), RIL_CMD_PROXY_5, m_slot_id);
}

void RmcDcReqHandler::handleUrspReevaluate(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onUrspReevaluate(msg);
}

void RmcDcReqHandler::handleUpdateDataRatRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onDataRatUpdated(msg);
}

void RmcDcReqHandler::handleReleaseRfOccupy(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->onReleaseRfOccupy(msg);
}

void RmcDcReqHandler::handleReleaseRfOccupyTimeout(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcDefaultReqHdlr != NULL);
    m_pRmcDcDefaultReqHdlr->releaseRfOccupyTimeout(msg);
}
