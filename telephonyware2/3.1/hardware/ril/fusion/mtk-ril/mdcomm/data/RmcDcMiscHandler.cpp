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
#include "RmcDataDefs.h"
#include "RmcDcMiscHandler.h"
#include "RmcDcUtility.h"
#include "RfxMessageId.h"
#include "RfxIntsData.h"
#include "RfxSlicingConfigResponseData.h"
#include "RfxRawData.h"
#include "RfxStringData.h"

#define RFX_LOG_TAG "RmcDcMiscHandler"

/*****************************************************************************
 * Class RmcDcMiscHandler
 * Only receive request and event in this class.
 * Create RmcDcMiscImpl object to send AT Command to modem.
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcDcMiscHandler, RIL_CMD_PROXY_5);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NETWORK_REJECT_CAUSE);
RFX_REGISTER_DATA_TO_URC_ID(RfxSlicingConfigResponseData, RFX_MSG_UNSOL_SLICING_CONFIG_CHANGED);

RmcDcMiscHandler::RmcDcMiscHandler(int slotId, int channelId)
        : RfxBaseHandler(slotId, channelId), m_pRmcDcMiscImpl(NULL) {

    const int requestList[] = {
        RFX_MSG_REQUEST_START_LCE,
        RFX_MSG_REQUEST_STOP_LCE,
        RFX_MSG_REQUEST_PULL_LCEDATA,
        RFX_MSG_REQUEST_SET_FD_MODE,
        RFX_MSG_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA,
        RFX_MSG_REQUEST_SEND_DEVICE_STATE,
        RFX_MSG_REQUEST_GET_SLICING_CONFIG,
        RFX_MSG_REQUEST_UPDATE_SLICING_CONFIG,
    };

    const int eventList[] = {
        RFX_MSG_EVENT_DATA_LCE_STATUS_CHANGED,
        RFX_MSG_EVENT_RADIO_CAPABILITY_UPDATED,
        RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE,
        RFX_MSG_EVENT_NETWORK_REJECT_CAUSE,
        RFX_MSG_EVENT_UE_POLICY_CHANGED,
    };

    registerToHandleRequest(requestList, sizeof(requestList) / sizeof(int));
    registerToHandleEvent(eventList, sizeof(eventList) / sizeof(int));

    m_pRmcDcMiscImpl = new RmcDcMiscImpl(this, slotId);
    m_pRmcDcMiscImpl->init();
}

RmcDcMiscHandler::~RmcDcMiscHandler() {
    delete m_pRmcDcMiscImpl;
    m_pRmcDcMiscImpl = NULL;
}

void RmcDcMiscHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_START_LCE:
            handleStartLceRequest(msg);
            break;
        case RFX_MSG_REQUEST_STOP_LCE:
            handleStopLceRequest(msg);
            break;
        case RFX_MSG_REQUEST_PULL_LCEDATA:
            handlePullLceDataRequest(msg);
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
        case RFX_MSG_REQUEST_GET_SLICING_CONFIG:
            handleGetSlicingConfigRequest(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_SLICING_CONFIG:
            handleUpdateSlicingConfigRequest(msg);
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: Unknown request, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
}

void RmcDcMiscHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_EVENT_DATA_LCE_STATUS_CHANGED:
            handleLceStatusChanged(msg);
            break;
        case RFX_MSG_EVENT_RADIO_CAPABILITY_UPDATED:
            init();
            break;
        case RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE:
            handleLinkCapacityEstimate(msg);
            break;
        case RFX_MSG_EVENT_NETWORK_REJECT_CAUSE:
            handleNetworkRejectCause(msg);
            break;
        case RFX_MSG_EVENT_UE_POLICY_CHANGED:
            handleUePolicyChanged(msg);
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: Unknown event, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
}

void RmcDcMiscHandler::init() {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->init();
}

void RmcDcMiscHandler::handleStartLceRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestStartLce(msg);
}

void RmcDcMiscHandler::handleStopLceRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestStopLce(msg);
}

void RmcDcMiscHandler::handlePullLceDataRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestPullLceData(msg);
}

void RmcDcMiscHandler::handleLceStatusChanged(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->onLceStatusChanged(msg);
}

void RmcDcMiscHandler::handleSetFdModeRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->setFdMode(msg);
}

void RmcDcMiscHandler::handleSetLinkCapacityReportingCriteriaRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestSetLinkCapacityReportingCriteria(msg);
}

void RmcDcMiscHandler::handleLinkCapacityEstimate(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->onLinkCapacityEstimate(msg);
}

void RmcDcMiscHandler::handleNetworkRejectCause(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    std::string prefix = "ECCAUSE:";
    std::smatch m;
    static std::regex r1(prefix + " *(\\d+), *(\\d+), *(\\d+)");
    int emmCause = -1;
    int esmCause = -1;
    int event = -1;
    std::string urc = std::string((char*)msg->getData()->getData());

    // ECCAUSE: <emm_cause>,<esm_cause>,<event>
    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] urc %s", m_slot_id, __FUNCTION__, urc.c_str());
    if (urc.find(prefix) != std::string::npos) {
        std::regex_search(urc, m, r1);

        if (m.empty() || m.size() != 4) {
            for (std::size_t n = 0; n < m.size(); ++n) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] match error [%s]",
                    m_slot_id, __FUNCTION__, m.str(n).c_str());
            }
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ECCAUSE content error, #matched %d, return",
                m_slot_id, __FUNCTION__, (int) m.size());
            return;
        } else {
            emmCause = std::stoi(m.str(1), NULL, 10);
            esmCause = std::stoi(m.str(2), NULL, 10);
            event = std::stoi(m.str(3), NULL, 10);

            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] emm %d, esm %d, event %d",
                m_slot_id, __FUNCTION__, emmCause, esmCause, event);
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] urc format error, return",
            m_slot_id, __FUNCTION__);
        return;
    }

    sp<RfxMclMessage> rejCauseUrc;
    int response[3] = {0};
    response[0] = emmCause;
    response[1] = esmCause;
    response[2] = event;
    rejCauseUrc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_REJECT_CAUSE, m_slot_id,
            RfxIntsData(response, 3));
    responseToTelCore(rejCauseUrc);
    return;
}

void RmcDcMiscHandler::handleSendDeviceStateRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    int *pReqInt = (int *)msg->getData()->getData();

    // Only handle LOW_DATA_EXPECTED
    if (pReqInt[0] == FASTDORMANCY_LOW_DATA_EXPECTED) {
        int settings[FASTDORMANCY_PARAMETER_LENGTH] = {0};
        settings[0] = 1;          // args Num
        settings[1] = pReqInt[1]; // state
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: state: %d", m_slot_id, __FUNCTION__, settings[1]);

        RfxIntsData data(settings, FASTDORMANCY_PARAMETER_LENGTH);
        sp<RfxMclMessage> request = RfxMclMessage::obtainRequest(msg->getId(),
                &data, msg->getSlotId(), msg->getToken(),
                msg->getSendToMainProtocol(), msg->getRilToken(),
                msg->getTimeStamp(), msg->getAddAtFront());
        m_pRmcDcMiscImpl->setFdMode(request);
    } else {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_REQUEST_NOT_SUPPORTED, RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}

// Parse rule from AT< +EGUEPOLICYRDP: "<plmn_id>",<m-th rule precedence>,"<m-th TD>",
//                                      <num of RSD>,<1st RSD precedence>,"<1st RSD>"
//                                      [<n-th RSD precedence>,"<n-th RSD>"]]
void RmcDcMiscHandler::parseRulesFromAt(RIL_UrspRule *rule, RfxAtLine *pLine) {
    if (rule == NULL || pLine == NULL) {
        return;
    }

    int err = 0;
    char *outStr = NULL;
    int outInt = 0;
    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        return;
    }

    // "<plmn_id>", don't care, skip
    outStr = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing plmn_id",
                m_slot_id, __FUNCTION__);
        return;
    }

    // <m-th rule precedence>
    outInt = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing precedence",
                m_slot_id, __FUNCTION__);
        return;
    }
    rule->precedence = (uint8_t)outInt;

    // "<m-th TD>"
    outStr = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing TD",
                m_slot_id, __FUNCTION__);
    }
    if (outStr != NULL && strlen(outStr) > 0) {
        rule->tdCount = 1;
        rule->trafficDescriptors = (RIL_TrafficDescriptor*)calloc(1, sizeof(RIL_TrafficDescriptor));
        RFX_ASSERT(rule->trafficDescriptors != NULL);
        if (RmcDcUtility::parseTdFromString(rule->trafficDescriptors, outStr)) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] rule precedence: %d, TD:[%s,%d]",
                    m_slot_id, __FUNCTION__, rule->precedence,
                    rule->trafficDescriptors->dnn, rule->trafficDescriptors->osAppIdLen);
        } else {
            rule->tdCount = 0;
            rule->trafficDescriptors = NULL;
        }
    }

    // <num of RSD>
    outInt = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing num of RSD",
                m_slot_id, __FUNCTION__);
        return;
    }

    if (outInt < 1) {
        // No RSD, return
        return;
    }
    rule->rsdCount = outInt;
    rule->routeSelectionDescriptor = (RIL_RouteSelectionDescriptor*)calloc(outInt,
            sizeof(RIL_RouteSelectionDescriptor));
    RFX_ASSERT(rule->routeSelectionDescriptor != NULL);

    for (int i = 0; i < outInt; i++) {
        // <RSD precedence>
        outInt = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing RSD precedence",
                    m_slot_id, __FUNCTION__);
            return;
        }
        rule->routeSelectionDescriptor[i].precedence = outInt;

        // "<RSD>"
        outStr = pLine->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing RSD",
                    m_slot_id, __FUNCTION__);
        }
        RmcDcUtility::parseRsdFromString(&(rule->routeSelectionDescriptor[i]), outStr);

        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] rule RSD[%d]:[%d,%s,%d,%d,%d]",
                m_slot_id, __FUNCTION__, i,
                rule->routeSelectionDescriptor[i].precedence,
                rule->routeSelectionDescriptor[i].sessionType,
                rule->routeSelectionDescriptor[i].sscMode,
                rule->routeSelectionDescriptor[i].sliceCount,
                rule->routeSelectionDescriptor[i].dnnCount);
    }
}

void RmcDcMiscHandler::getUrspRules(RIL_SlicingConfig &sc, String8 plmn) {
    RFX_UNUSED(plmn);
    RfxAtLine *p_cur = NULL;
    int ruleCount = 0;
    int ruleIndex = 0;
    /* Query UE policy (URSP rule)
           AT> AT+EGUEPOLICYRDP=<msg_type>[,"<plmn_id>"]
           AT< +EGUEPOLICYRDP: "<plmn_id>",<m-th rule precedence>,"<m-th TD>",
                                <num of RSD>,<1st RSD precedence>,"<1st RSD>"
                                [<n-th RSD precedence>,"<n-th RSD>"]]
           AT< ...
           AT< OK
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline(
            String8::format("AT+EGUEPOLICYRDP=1"), "+EGUEPOLICYRDP:");
    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get p_response!", m_slot_id, __FUNCTION__);
        return;
    }
    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] At Response Fail!", m_slot_id, __FUNCTION__);
        return;
    }

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        if (p_cur == NULL) {
            break;
        }
        ruleCount++;
    }

    sc.ruleCount = ruleCount;
    sc.urspRules = (RIL_UrspRule*)calloc(ruleCount, sizeof(RIL_UrspRule));
    RFX_ASSERT(sc.urspRules != NULL);

    for (p_cur = p_response->getIntermediates(); p_cur != NULL; p_cur = p_cur->getNext()) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] rule %d, line: %s", m_slot_id, __FUNCTION__,
                ruleIndex, p_cur->getLine());
        parseRulesFromAt(&(sc.urspRules[ruleIndex]), p_cur);
        ruleIndex++;
        if (ruleIndex >= ruleCount) {
            break;
        }
    }
}

bool RmcDcMiscHandler::parseDefaultAndRejectedNssai(RIL_SliceInfo *nssais, int *index,
        RfxAtLine *pLine) {
    if (pLine == NULL) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] import is null.", m_slot_id, __FUNCTION__);
        return false;
    }
    int err = 0;
    int curCount = *index;
    int retCount = 0;
    int outInt = 0;
    char *outStr = NULL;

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        return false;
    }

    if (!pLine->atTokHasmore()) {
        /* no default configured NSSAI and no rejected NSSAI, but configured or allowed NAASI exist
            +C5GNSSAIRDP:
            +C5GNSSAIRDP:"00101",2...
        */
        return true;
    }

    // <default_configured_nssai_length>
    outInt = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing default configured length",
                m_slot_id, __FUNCTION__);
        return false;
    }
    // "<default_configured_nssai>"
    outStr = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing default configured nssai",
                m_slot_id, __FUNCTION__);
        return false;
    }
    RmcDcUtility::parseNssaiFromString(nssais, &retCount, SLICE_DEFAULT_CONFIGURED, outStr);
    curCount += retCount;

    if (pLine->atTokHasmore()) {
        // <rejected_nssai_3gpp_length>
        outInt = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing rejected 3gpp length",
                    m_slot_id, __FUNCTION__);
            return false;
        }
        // "<rejected_nssai_3gpp>"
        outStr = pLine->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing rejected 3gpp nssai",
                    m_slot_id, __FUNCTION__);
            return false;
        }
        RmcDcUtility::parseNssaiFromString(nssais, &retCount, SLICE_REJECTED_NOT_AVAILABLE_IN_PLMN, outStr);
        curCount += retCount;
    }

    *index = curCount;
    return true;
}

bool RmcDcMiscHandler::parseConfiguredAndAllowedNssai(RIL_SliceInfo *nssais, int *index,
        RfxAtLine *pLine) {
    if (pLine == NULL) {
        return false;
    }
    int err = 0;
    int curCount = *index;
    int retCount = 0;
    int outInt = 0;
    char *outStr = NULL;

    pLine->atTokStart(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                m_slot_id, __FUNCTION__);
        return false;
    }
    // <plmn_id>
    outInt = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing plmn_id",
                m_slot_id, __FUNCTION__);
        return false;
    }
    if (!pLine->atTokHasmore()) {
        // no configured and no allowed NSSAI, it is legal by spec
        return true;
    }
    // <configured_nssai_length>
    outInt = pLine->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing configured length",
                m_slot_id, __FUNCTION__);
        return false;
    }
    // "<configured_nssai>"
    outStr = pLine->atTokNextstr(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing configured nssai",
                m_slot_id, __FUNCTION__);
        return false;
    }
    RmcDcUtility::parseNssaiFromString(nssais, &retCount, SLICE_CONFIGURED, outStr);
    curCount += retCount;

    if (pLine->atTokHasmore()) {
        // <allowed_nssai_3gpp_length>
        outInt = pLine->atTokNextint(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing allowd 3gpp length",
                    m_slot_id, __FUNCTION__);
            return false;
        }
        // "<allowed_nssai_3gpp>"
        outStr = pLine->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing allowed 3gpp nssai",
                    m_slot_id, __FUNCTION__);
            return false;
        }
        RmcDcUtility::parseNssaiFromString(nssais, &retCount, SLICE_ALLOWED, outStr);
        curCount += retCount;
    }

    *index = curCount;
    return true;
}

void RmcDcMiscHandler::getSliceInfo(RIL_SlicingConfig &sc, String8 plmn) {
    RfxAtLine *p_cur = NULL;
    int err = 0;
    char *outStr = NULL;
    int outInt = 0;
    int curCount = 0;
    int retCount = 0;

    /* Query NSSAI (Slice info) +C5GNSSAIRDP, from 27.007
        AT> AT+C5GNSSAIRDP[=<nssai_type>[,<plmn_id>]]
        [AT< +C5GNSSAIRDP: [<default_configured_nssai_length>,<default_configured_nssai>
            [,<rejected_nssai_3gpp_length>,<rejected_nssai_3gpp>
            [,<rejected_nssai_non3gpp_length>,<rejected_nssai_non3gpp>]]]
        [AT< +C5GNSSAIRDP: <plmn_id>[,<configured_nssai_length>,<configured_nssai>
            [,<allowed_nssai_3gpp_length>,<allowed_nssai_3gpp>,
            <allowed_nssai_non3gpp_length>,<allowed_nssai_non3gpp>]]
        [AT< +C5GNSSAIRDP: <plmn_id>[,<configured_nssai_length>,<configured_nssai>
            [,<allowed_nssai_3gpp_length>,<allowed_nssai_3gpp>,
            <allowed_nssai_non3gpp_length>,<allowed_nssai_non3gpp>]]
        [...]]]]
        AT< OK
        <nssai_type>: integer type; specifies the type of NSSAI to be returned.
            0: return stored default configured NSSAI only
            1: return stored default configured NSSAI and rejected NSSAI(s)
            2: return stored default configured NSSAI, rejected NSSAI(s) and configured NSSAI(s)
            3: return stored default configured NSSAI, rejected NSSAI(s),
               configured NSSAI(s) and allowed NSSAI(s)

        Example:
        If no default configured NSSAI, but rejected NSSAI exist
            +C5GNSSAIRDP:0,"",<rejected_nssai_3gpp_length>...
        If no default configured NSSAI and no rejected NSSAI, but configured or allowed NAASI exist
            +C5GNSSAIRDP:
            +C5GNSSAIRDP:"00101",2...
        If ther are not any NSSAI, only OK response, no any +C5GNSSAIRDP:
            AT< OK
    */
    RIL_SliceInfo nssais[MAX_NSSAIS * MAX_NSSAI_TYPE];
    memset(nssais, 0, sizeof(RIL_SliceInfo) * MAX_NSSAIS * MAX_NSSAI_TYPE);

    sp<RfxAtResponse> p_response = atSendCommandMultiline(
            String8::format("AT+C5GNSSAIRDP=3,\"%s\"", plmn.string()), "+C5GNSSAIRDP:");
    if (p_response == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to get C5GNSSAIRDP p_response!",
                m_slot_id, __FUNCTION__);
        return;
    }
    if (p_response->isAtResponseFail()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] At Response C5GNSSAIRDP Fail!",
                m_slot_id, __FUNCTION__);
        return;
    }
    // 1st intermediate
    p_cur = p_response->getIntermediates();
    if (p_cur == NULL) {
        return;
    }
    if (false == parseDefaultAndRejectedNssai(nssais, &curCount, p_cur)) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] parseDefaultAndRejectedNssai Fail!",
                m_slot_id, __FUNCTION__);
    }

    // 2nd, 3rd ... intermediates
    for (p_cur = p_cur->getNext(); p_cur != NULL; ) {
        if (false ==  parseConfiguredAndAllowedNssai(nssais, &curCount, p_cur)) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] parseConfiguredAndAllowedNssai Fail!",
                    m_slot_id, __FUNCTION__);
            return;
        }
        p_cur = p_cur->getNext();
    }

    sc.sliceCount = curCount;
    sc.sliceInfo = (RIL_SliceInfo*)calloc(curCount, sizeof(RIL_SliceInfo));
    RFX_ASSERT(sc.sliceInfo != NULL);
    memcpy(sc.sliceInfo, nssais, curCount * sizeof(RIL_SliceInfo));
}

void RmcDcMiscHandler::handleGetSlicingConfigRequest(const sp<RfxMclMessage>& msg) {
    getSliceConfig(msg);
}

void RmcDcMiscHandler::handleUpdateSlicingConfigRequest(const sp<RfxMclMessage>& msg) {
    getSliceConfig(msg);
}

void RmcDcMiscHandler::handleUePolicyChanged(const sp<RfxMclMessage>& msg) {
    //AT< +EGUEPOLICYU: "<PLMN>"
    getSliceConfig(msg);
}

void RmcDcMiscHandler::getSliceConfig(const sp<RfxMclMessage>& msg) {
    RIL_SlicingConfig sc;
    memset(&sc, 0, sizeof(RIL_SlicingConfig));
    String8 homePlmn = getMclStatusManager()->getString8Value(
            RFX_STATUS_KEY_UICC_GSM_NUMERIC, String8("0"));
    RFX_LOG_D(RFX_LOG_TAG,"[%d][%s] plmn: %s", m_slot_id, __FUNCTION__, homePlmn.string());
    if (homePlmn.length() < MIN_PLMN_LENTH) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] currentMccmnc is invalid", m_slot_id, __FUNCTION__);
        if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
            sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_SUCCESS, RfxSlicingConfigResponseData(&sc, sizeof(RIL_SlicingConfig)),
                msg, false);
            responseToTelCore(response);
        }
        return;
    }

    // ignore policy change URC if it is not for home PLMN
    if (msg->getType() == RFX_MESSAGE_TYPE::EVENT_MESSAGE) {
        char *urc = (char*)msg->getData()->getData();
        int err = 0;
        RfxAtLine *pLine = NULL;
        char *plmn = NULL;
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] urc = %s", m_slot_id, __FUNCTION__, urc);

        pLine = new RfxAtLine(urc, NULL);

        if (pLine == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] fail to new pLine!",
                    m_slot_id, __FUNCTION__);
            AT_LINE_FREE(pLine);
            return;
        }

        pLine->atTokStart(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when token start",
                    m_slot_id, __FUNCTION__);
            AT_LINE_FREE(pLine);
            return;
        }

        plmn = pLine->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ERROR occurs when parsing PLMN",
                    m_slot_id, __FUNCTION__);
            AT_LINE_FREE(pLine);
            return;
        }

        if (0 != strcmp(plmn, homePlmn.string())) {
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] ignore UE policy URC of non-home PLMN",
                    m_slot_id, __FUNCTION__);
            AT_LINE_FREE(pLine);
            return;
        }
        AT_LINE_FREE(pLine);
    }

    getUrspRules(sc, homePlmn);
    getSliceInfo(sc, homePlmn);

    dumpSliceConfig(sc);

    // Response
    if (msg->getType() == RFX_MESSAGE_TYPE::REQUEST) {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_SUCCESS, RfxSlicingConfigResponseData(&sc, sizeof(RIL_SlicingConfig)),
                msg, false);
        responseToTelCore(response);
    }

    // URC
    if (msg->getType() == RFX_MESSAGE_TYPE::EVENT_MESSAGE ||
            msg->getId() == RFX_MSG_REQUEST_UPDATE_SLICING_CONFIG) {
        sp<RfxMclMessage> urcMsg = RfxMclMessage::obtainUrc(
                RFX_MSG_UNSOL_SLICING_CONFIG_CHANGED, m_slot_id,
                RfxSlicingConfigResponseData(&sc, sizeof(RIL_SlicingConfig)));
        responseToTelCore(urcMsg);
    }

    for (int i = 0; i < sc.ruleCount; i++) {
        // Free dnn and os APP id
        for (int j = 0; j < sc.urspRules[i].tdCount; j++) {
            FREEIF(sc.urspRules[i].trafficDescriptors[j].dnn);
            if (sc.urspRules[i].trafficDescriptors[j].osAppIdLen > 0) {
                FREEIF(sc.urspRules[i].trafficDescriptors[j].osAppId);
            }
        }
        // Free TD info
        FREEIF(sc.urspRules[i].trafficDescriptors);

        // Free sessionType and sliceInfo
        for (int j = 0; j < sc.urspRules[i].rsdCount; j++) {
            FREEIF(sc.urspRules[i].routeSelectionDescriptor[j].sessionType);
            FREEIF(sc.urspRules[i].routeSelectionDescriptor[j].sliceInfo);
        }
        // Free RSD info
        FREEIF(sc.urspRules[i].routeSelectionDescriptor);
    }
    FREEIF(sc.urspRules);
    FREEIF(sc.sliceInfo);
}

void RmcDcMiscHandler::dumpSliceConfig(RIL_SlicingConfig sc) {
    RFX_LOG_D(RFX_LOG_TAG,"%d][%s] rule number: %d", m_slot_id, __FUNCTION__, sc.ruleCount);
    for (int i = 0; i < sc.ruleCount; i++) {
        RFX_LOG_D(RFX_LOG_TAG,"%d][%s] rules[%d],precedence:%d,tdCount:%d,rsdCount:%d",
                m_slot_id, __FUNCTION__, i, sc.urspRules[i].precedence,
                sc.urspRules[i].tdCount, sc.urspRules[i].rsdCount);
        for (int j = 0; j < sc.urspRules[i].tdCount; j++) {
            RFX_LOG_D(RFX_LOG_TAG,"%d][%s] TD[%d]:{%s,%d}", m_slot_id, __FUNCTION__, j,
                    sc.urspRules[i].trafficDescriptors[j].dnn,
                    sc.urspRules[i].trafficDescriptors[j].osAppIdLen);
        }
        for (int j = 0; j < sc.urspRules[i].rsdCount; j++) {
            RFX_LOG_D(RFX_LOG_TAG,"%d][%s] RSD[%d]:{%d,%s,%d,%d,%d}", m_slot_id, __FUNCTION__, j,
                    sc.urspRules[i].routeSelectionDescriptor[j].precedence,
                    sc.urspRules[i].routeSelectionDescriptor[j].sessionType,
                    sc.urspRules[i].routeSelectionDescriptor[j].sscMode,
                    sc.urspRules[i].routeSelectionDescriptor[j].sliceCount,
                    sc.urspRules[i].routeSelectionDescriptor[j].dnnCount);
        }
    }
    for (int i = 0; i < sc.sliceCount; i++) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] Slice info count: %d, [%d]{%d,%d,%d,%d,%d}", __FUNCTION__,
                sc.sliceCount, i,
                sc.sliceInfo[i].sst,
                sc.sliceInfo[i].sliceDifferentiator,
                sc.sliceInfo[i].mappedHplmnSst,
                sc.sliceInfo[i].mappedHplmnSD,
                sc.sliceInfo[i].status);
    }
}
