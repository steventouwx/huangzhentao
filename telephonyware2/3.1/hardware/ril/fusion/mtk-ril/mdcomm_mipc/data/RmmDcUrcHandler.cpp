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
#include "NetAgentCommon.h"
#include "NetAgentService.h"
#include "RfxIntsData.h"
#include "RfxQualifiedNetworkInfo.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RfxPcoInfo.h"
#include "RmmDataDefs.h"
#include "RmmDcUrcHandler.h"
#include "RmmDcUtility.h"

#define RFX_LOG_TAG "RmmDcUrcHandler"

/*****************************************************************************
 * Class RmmDcUrcHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmmDcUrcHandler, RIL_CMD_PROXY_URC);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DATA_MODIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DATA_ME_PDN_ACT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DATA_DEACT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DATA_PDN_CHANGE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DATA_NW_ACT);
/// M: Ims Data Framework {@
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_IMS_PDN_NOTIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_PCSCF_ADDRESS_DISCOVERY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxQualifiedNetworkInfo, RFX_MSG_EVENT_QUALIFIED_NETWORK_TYPES_CHANGED);
/// @}
RFX_REGISTER_DATA_TO_EVENT_ID(RfxPcoInfo, RFX_MSG_EVENT_QUERY_PCO_WITH_AID);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_NETWORK_REJECT_CAUSE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_MOBILE_DATA_USAGE_NOTIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_KEEPALIVE_STATUS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DATA_NW_LIMIT_NOTIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DSDA_STATE_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_URSP_REEVALUATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_UE_POLICY_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_MD_DATA_RETRY_COUNT_RESET);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DYNAMIC_REGISTER_URC);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_ATTACH_APN_CHANGED);

RmmDcUrcHandler::RmmDcUrcHandler(int slot_id, int channel_id)
        : RfxBaseHandler(slot_id, channel_id) {

    const int urc_list[] = {
        MIPC_DATA_ACT_CALL_IND,
        MIPC_DATA_DEACT_CALL_IND,
        MIPC_DATA_MOD_CALL_IND,
        MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND,
        MIPC_DATA_MOBILE_DATA_USAGE_IND,
        MIPC_DATA_NW_LIMIT_IND,
        MIPC_DATA_MOD_PCO_IND,
        MIPC_DATA_DSDA_STATE_IND,
        MIPC_DATA_IWLAN_PRIORITY_LIST_IND,
        MIPC_INTERNAL_HO_IND,
        MIPC_DATA_RETRY_TIMER_IND,
        MIPC_DATA_KEEPALIVE_STATUS_IND,
        MIPC_DATA_URSP_REEVAL_IND,
        MIPC_DATA_URSP_UE_POLICY_CHG_IND,
        MIPC_NW_IA_IND,
    };

    registerToHandleMipcURC(urc_list, sizeof(urc_list)/sizeof(int));

    /// IMS {@
    const int eventList[] = {
        RFX_MSG_EVENT_DYNAMIC_REGISTER_URC
    };
    registerToHandleEvent(eventList, sizeof(eventList) / sizeof(int));
    /// @}
}

RmmDcUrcHandler::~RmmDcUrcHandler() {
}

void RmmDcUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() == NULL) {
        RFX_LOG_W(RFX_LOG_TAG, "onHandleUrc: msg data is NULL");
        return;
    }

    switch (msg->getMipcData()->getMipcMsgId()) {
        case MIPC_DATA_ACT_CALL_IND:
            onDataCallAct(msg);
            break;
        case MIPC_DATA_DEACT_CALL_IND:
            onDataCallDeact(msg);
            break;
        case MIPC_DATA_MOD_CALL_IND:
            onDataCallModified(msg);
            break;
        case MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND:
            onLinkCapacityEstimate(msg);
            break;
        case MIPC_DATA_MOBILE_DATA_USAGE_IND:
            onMobileDataUsage(msg);
            break;
        case MIPC_DATA_NETWORK_REJECT_CAUSE_IND:
            onNetworkRejectCause(msg);
            break;
        case MIPC_DATA_NW_LIMIT_IND:
            onNetworkLimitNotify(msg);
            break;
        case MIPC_DATA_MOD_PCO_IND:
            onPcoInfoNotify(msg);
            break;
        case MIPC_DATA_DSDA_STATE_IND:
            onDsdaStateChanged(msg);
            break;
        case MIPC_DATA_KEEPALIVE_STATUS_IND:
            onKeepAliveStateChanged(msg);
            break;
        case MIPC_DATA_IWLAN_PRIORITY_LIST_IND:
            onQualifiedNetworkTypesChanged(msg);
            break;
        case MIPC_INTERNAL_HO_IND:
            onPdnHandover(msg);
            break;
        case MIPC_DATA_RETRY_TIMER_IND:
            onDataCountReset(msg);
            break;
        case MIPC_DATA_URSP_REEVAL_IND:
            onUrspReevaluate(msg);
            break;
        case MIPC_DATA_URSP_UE_POLICY_CHG_IND:
            onUrspUePolicyChanged(msg);
            break;
        case MIPC_NW_IA_IND:
            onAttachApnNotify(msg);
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: Unknown urc, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
}

void RmmDcUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_EVENT_DYNAMIC_REGISTER_URC:
            onDynamicRegisterUrc(msg);
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: Unknown event, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
}

void RmmDcUrcHandler::onDynamicRegisterUrc(const sp<RfxMclMessage>& msg) {
    int *data = (int *)msg->getData()->getData();
    if (data == NULL) return;

    int count = data[0] - 1;
    if (count < 1) return; // at least has one URC
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] register %d", m_slot_id, __FUNCTION__, data[1]);

    int* registeredList = (int*)calloc(count, sizeof(int));
    if (registeredList == NULL) return;
    for (int i = 0; i < count; i++) {
        registeredList[i] = data[i + 2];
    }

    if (data[1] > 0) {
        registerToHandleMipcURC(registeredList, count);
    } else {
        unregisterToHandleMipcURC(registeredList, count);
    }

    FREEIF(registeredList);
}

void RmmDcUrcHandler::onLinkCapacityEstimate(const sp<RfxMclMessage>& msg) {
    int linkCapacityEstimate[COUNT_LINK_CAPACITY_ESTIMATE] = {0};
    linkCapacityEstimate[0] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_DL_KBPS, 0);
    linkCapacityEstimate[1] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_UL_KBPS, 0);
    linkCapacityEstimate[2] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_SECOND_DL_KBPS, 0);
    linkCapacityEstimate[3] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_SECOND_UL_KBPS, 0);
    sendEvent(RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE,
            RfxIntsData(linkCapacityEstimate, COUNT_LINK_CAPACITY_ESTIMATE),
            RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onMobileDataUsage(const sp<RfxMclMessage>& msg) {
    int mobileDataUsage[COUNT_MOBILE_DATA_USAGE] = {0};
    mobileDataUsage[0] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_MOBILE_DATA_USAGE_IND_T_TX_BYTES, 0);
    mobileDataUsage[1] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_MOBILE_DATA_USAGE_IND_T_TX_PACKETS, 0);
    mobileDataUsage[2] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_MOBILE_DATA_USAGE_IND_T_RX_BYTES, 0);
    mobileDataUsage[3] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_MOBILE_DATA_USAGE_IND_T_RX_PACKETS, 0);
    sendEvent(RFX_MSG_EVENT_MOBILE_DATA_USAGE_NOTIFY,
            RfxIntsData(mobileDataUsage, COUNT_MOBILE_DATA_USAGE),
            RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onNetworkRejectCause(const sp<RfxMclMessage>& msg) {
    int networkRejectCause[COUNT_NETWORK_REJECT_CAUSE] = {0};
    networkRejectCause[0] = (int)msg->getMipcData()->getMipcUint16Val(
            MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_EMM_CAUSE, 0);
    networkRejectCause[1] = (int)msg->getMipcData()->getMipcUint16Val(
            MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_ESM_CAUSE, 0);
    networkRejectCause[2] = (int)msg->getMipcData()->getMipcUint16Val(
            MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_EVENT, 0);
    sendEvent(RFX_MSG_EVENT_NETWORK_REJECT_CAUSE,
            RfxIntsData(networkRejectCause, COUNT_NETWORK_REJECT_CAUSE),
            RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onNetworkLimitNotify(const sp<RfxMclMessage>& msg) {
    int networkLimit[1] = {0};
    // 0: no limit, 1: has limit
    networkLimit[0] = (int)msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_NW_LIMIT_IND_T_STATE, -1);
    if (networkLimit[0] != -1) {
        sendEvent(RFX_MSG_EVENT_DATA_NW_LIMIT_NOTIFY,
                RfxIntsData(networkLimit, 1), RIL_CMD_PROXY_5, m_slot_id);
    }
}

void RmmDcUrcHandler::onDataCallAct(const sp<RfxMclMessage>& msg) {
    // 1. For fllback case:
    // aid: equals to p_id or fb_id. If it's equals to p_id in ACT_CONF,
    // then it's equals to fb_id in ACT_IND, vice versa.
    // p_id: valid and different from fb_id
    // fb_id: valid and different from p_id, must be valid in fallback
    // scenario, or else no value is put in.
    // For example:
    //              aid        p_id          fb_id
    // ACT_CNF      1/2          1             2
    // ACT_IND      2/1          1             2
    // 2. For non-fallback case:
    // p_id: L5 is not sure it's existed or not.
    // fb_id: not exist.
    int ret[4] = {0};
    ret[0] = (int) msg->getMipcData()->getResult();
    ret[1] = (int) msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_ACT_CALL_IND_T_ID, UINT8_MAX);
    ret[2] = (int) msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_ACT_CALL_IND_T_P_ID, UINT8_MAX);
    ret[3] = (int) msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_ACT_CALL_IND_T_FB_ID, UINT8_MAX);
    if (ret[1] == UINT8_MAX) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s]: invalid aid:%d",
                m_slot_id, __FUNCTION__, ret[1]);
        return;
    }

    if (ret[2] == UINT8_MAX) ret[2] = INVALID_AID;
    if (ret[3] == UINT8_MAX) ret[3] = INVALID_AID;
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s]: result:%d, aid:%d, p_id=%d, fb_id=%d",
            m_slot_id, __FUNCTION__, ret[0], ret[1], ret[2], ret[3]);
    if (ret[0] != MIPC_RESULT_SUCCESS) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: result is not SUCCESS, return",
                m_slot_id, __FUNCTION__);
        return;
    }

    if (ret[3] != INVALID_AID) {
        // fallback
        sendEvent(RFX_MSG_EVENT_DATA_ME_PDN_ACT, RfxIntsData(ret, 4),
                RIL_CMD_PROXY_5, m_slot_id);
    } else if (ret[1] != INVALID_AID && ret[2] != INVALID_AID && ret[1] != ret[2]) {
        // dedicate bearer
        sendEvent(RFX_MSG_EVENT_DATA_NW_ACT, RfxIntsData(ret, 4),
                RIL_CMD_PROXY_5, m_slot_id);
    }
}

void RmmDcUrcHandler::onDataCallDeact(const sp<RfxMclMessage>& msg) {
    int ret[2] = {0}; // [0]: aid, [1]:result,network error enum
    ret[0] = (int) msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_DEACT_CALL_IND_T_ID, UINT8_MAX);
    if (ret[0] == UINT8_MAX) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s]: invalid aid!", m_slot_id, __FUNCTION__);
        return;
    }
    ret[1] = (int) msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_DEACT_CALL_IND_T_NEW_RES, NO_REASON);
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s]: aid:%d, res:%d",
            m_slot_id, __FUNCTION__, ret[0], ret[1]);
    sendEvent(RFX_MSG_EVENT_DATA_DEACT, RfxIntsData(ret, 2),
            RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onDataCallModified(const sp<RfxMclMessage>& msg) {
    int ret[1] = {0};
    ret[0] = (int) msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_MOD_CALL_IND_T_ID, UINT8_MAX);
    // Currently, change reason and event type is not used.
    int reason = (int) mipc_data_mod_call_ind_get_change_reason(
            msg->getMipcData()->getData(), 0);
    mipc_data_mod_event_type_const_enum eventType = mipc_data_mod_call_ind_get_event_type(
            msg->getMipcData()->getData(), mipc_data_mod_event_type_const_NONE);
    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s]: aid:%d, change reason:%d, event type:%d",
            m_slot_id, __FUNCTION__, ret[0], reason, (int)eventType);
    if (ret[0] == UINT8_MAX) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s]: aid is invalid", m_slot_id, __FUNCTION__);
        return;
    }
    sendEvent(RFX_MSG_EVENT_DATA_MODIFY, RfxIntsData(ret, 1),
            RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onPcoInfoNotify(const sp<RfxMclMessage>& msg) {
    MipcPcoInfo pcoInfo;
    memset(&pcoInfo, 0, sizeof(pcoInfo));
    pcoInfo.pcoCount = (int)msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_MOD_PCO_IND_T_PCO_COUNT, 0);
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: pco count: %d", m_slot_id, __FUNCTION__, pcoInfo.pcoCount);
    if (pcoInfo.pcoCount > 0) {
        mipc_data_pco_ie_struct4 *mipcPcoItem = (mipc_data_pco_ie_struct4 *)mipc_msg_get_val_ptr(
                msg->getMipcData()->getData(), MIPC_DATA_MOD_PCO_IND_T_PCO_LIST, NULL);
        if (mipcPcoItem == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s]: no pco item!", m_slot_id, __FUNCTION__);
            return;
        }
        pcoInfo.pcoList = (MipcPcoItem *)calloc(pcoInfo.pcoCount, sizeof(MipcPcoItem));
        if (pcoInfo.pcoList == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s]: no memory!!!", m_slot_id, __FUNCTION__);
            return;
        }
        for (int i = 0; i < pcoInfo.pcoCount; i++) {
            strncpy(pcoInfo.pcoList[i].ie, mipcPcoItem[i].ie, MIPC_MAX_PCO_IE_LEN - 1);
            strncpy(pcoInfo.pcoList[i].content, mipcPcoItem[i].content,
                    MIPC_MAX_PCO_CONTENT_LEN - 1);
            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: pco ie: %s, content: %s", m_slot_id, __FUNCTION__,
                    pcoInfo.pcoList[i].ie, pcoInfo.pcoList[i].content);
        }
        pcoInfo.cid = (int)msg->getMipcData()->getMipcUint8Val(MIPC_DATA_MOD_PCO_IND_T_ID, 0);
        sendEvent(RFX_MSG_EVENT_QUERY_PCO_WITH_AID,
                RfxPcoInfo(&pcoInfo, sizeof(pcoInfo)), RIL_CMD_PROXY_5, m_slot_id);
        free(pcoInfo.pcoList);
    }
}

void RmmDcUrcHandler::onDsdaStateChanged(const sp<RfxMclMessage>& msg) {
    int dsdaState[COUNT_DSDA_STATUS] = {0};
    dsdaState[0] = (int)msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_DSDA_STATE_IND_T_DSDA_ALLOWED, 0);
    dsdaState[1] = (int)msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_DSDA_STATE_IND_T_DSDA_STATE, 0);
    dsdaState[2] = (int)msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_DSDA_STATE_IND_T_IS_DR_DSDA, DRDSDA_STATUS_NOT_SUPPORT);
    dsdaState[3] = (int)msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_DSDA_STATE_IND_T_IS_DR_DSDS, 0);
    sendEvent(RFX_MSG_EVENT_DSDA_STATE_CHANGED, RfxIntsData(dsdaState, COUNT_DSDA_STATUS),
            RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onKeepAliveStateChanged(const sp<RfxMclMessage>& msg) {
    int kaInd[2] = {0}; // [0]: session handle, [1]:status code
    kaInd[0] = (int) msg->getMipcData()->getMipcUint32Val(
            MIPC_DATA_KEEPALIVE_STATUS_IND_T_SESSION_HANDLE, UINT32_MAX);
    kaInd[1] = (int) msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_KEEPALIVE_STATUS_IND_T_STATUS_CODE, UINT8_MAX);
    if (kaInd[0] == UINT32_MAX || kaInd[1] == UINT8_MAX) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s]: invalid value!",
                m_slot_id, __FUNCTION__);
        return;
    }
    sendEvent(RFX_MSG_EVENT_KEEPALIVE_STATUS, RfxIntsData(kaInd, 2),
        RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onQualifiedNetworkTypesChanged(const sp<RfxMclMessage>& msg) {
    MipcQualifiedNetworkInfo info;
    memset(&info, 0, sizeof(info));
    info.ratNum = (int)msg->getMipcData()->getMipcUint8Val(
            MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_RAT_NUM, 0);
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: rat num: %d", m_slot_id, __FUNCTION__, info.ratNum);
    if (info.ratNum > 0) {
        char *pCmd =
                (char *)msg->getMipcData()->getMipcStr(MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_CMD);
        strncpy(info.cmd, pCmd, MIPC_MAX_EIWLAN_CMD_LEN - 1);
        char *pType =
                (char *)msg->getMipcData()->getMipcStr(MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_TYPE);
        strncpy(info.type, pType, MIPC_MAX_EIWLAN_TYPE_LEN - 1);
        info.ratList = (char *)calloc(info.ratNum, sizeof(char));
        if (info.ratList == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s]: no memory!!!", m_slot_id, __FUNCTION__);
            return;
        }
        char *pRatList = (char *)mipc_msg_get_val_ptr(
                msg->getMipcData()->getData(), MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_RAT_LIST, NULL);
        strncpy(info.ratList, pRatList, info.ratNum);

        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: cmd: %s, type: %s, rat: %d", m_slot_id, __FUNCTION__,
                info.cmd, info.type, info.ratList[0]);
        sendEvent(RFX_MSG_EVENT_QUALIFIED_NETWORK_TYPES_CHANGED,
                RfxQualifiedNetworkInfo(&info, sizeof(info)), RIL_CMD_PROXY_5, m_slot_id);
        free(info.ratList);
    }
}

void RmmDcUrcHandler::onPdnHandover(const sp<RfxMclMessage>& msg) {
    int info[PDN_HANDOVER_INFO_SIZE] = {0};
    info[PDN_HANDOVER_INFO_CID] =
            (int)msg->getMipcData()->getMipcUint8Val(MIPC_INTERNAL_HO_IND_T_ID, 0);
    info[PDN_HANDOVER_INFO_OLD_RAT] =
            (int)msg->getMipcData()->getMipcUint8Val(MIPC_INTERNAL_HO_IND_T_SRC_RAN, 0);
    info[PDN_HANDOVER_INFO_NEW_RAT] =
            (int)msg->getMipcData()->getMipcUint8Val(MIPC_INTERNAL_HO_IND_T_DST_RAN, 0);
    int progress = (int)msg->getMipcData()->getMipcUint8Val(MIPC_INTERNAL_HO_IND_T_PROGRESS, 0);
    int success = (int)msg->getMipcData()->getMipcUint8Val(MIPC_INTERNAL_HO_IND_T_IS_SUCC, 0);

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: cid: %d, srcrat: %d, dstrat: %d, pro:%d, succ:%d",
            m_slot_id, __FUNCTION__, info[PDN_HANDOVER_INFO_CID], info[PDN_HANDOVER_INFO_OLD_RAT],
            info[PDN_HANDOVER_INFO_NEW_RAT], progress, success);

    NetAgentService *pNetAgentService = NetAgentService::getInstance();
    if (NULL == pNetAgentService) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] NetAgentService is NULL, return error", m_slot_id,
                __FUNCTION__);
    } else {
        HandoverInfo hInfo;
        hInfo.cid = info[PDN_HANDOVER_INFO_CID];
        hInfo.hoState = progress;
        hInfo.is_succ = success;
        hInfo.src_ran = info[PDN_HANDOVER_INFO_OLD_RAT];
        hInfo.tgt_ran = info[PDN_HANDOVER_INFO_NEW_RAT];
        hInfo.transIntId =
                (int)msg->getMipcData()->getMipcUint32Val(MIPC_INTERNAL_HO_IND_T_TRANS_ID, 0);
        hInfo.addr_v4 = (mipc_data_v4_addr_struct4 *)mipc_msg_get_val_ptr(
                msg->getMipcData()->getData(), MIPC_INTERNAL_HO_IND_T_V4_ADDR, NULL);
        hInfo.addr_v6 = (mipc_data_v6_addr_struct4 *)mipc_msg_get_val_ptr(
                msg->getMipcData()->getData(), MIPC_INTERNAL_HO_IND_T_V6_ADDR, NULL);
        pNetAgentService->setHandoverInfo(hInfo);
    }
    if (progress == MIPC_INTERNAL_HO_PROGRESS_END && success == 1) {
        sendEvent(RFX_MSG_EVENT_DATA_PDN_CHANGE,
                RfxIntsData(info, sizeof(info)/sizeof(info[0])), RIL_CMD_PROXY_5, m_slot_id);
    }
}

void RmmDcUrcHandler::onDataCountReset(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: onDataCountReset.", m_slot_id, __FUNCTION__);
    sendEvent(RFX_MSG_EVENT_MD_DATA_RETRY_COUNT_RESET, RfxVoidData(), RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onUrspReevaluate(const sp<RfxMclMessage>& msg) {
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]", m_slot_id, __FUNCTION__);

    int count = (int)mipc_data_ursp_reeval_ind_get_id_list_len(msg->getMipcData()->getData(), 0);
    if (count > 0) {
        int *cidList = (int*)calloc(count + 1, sizeof(int));
        RFX_ASSERT(cidList != NULL);
        uint16_t listLen = 0;
        uint8_t *retList = (uint8_t*)mipc_data_ursp_reeval_ind_get_id_list(
                msg->getMipcData()->getData(), &listLen);
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] count:%d,listLen:%d", m_slot_id, __FUNCTION__,
                count, listLen);
        RFX_ASSERT(count == listLen);
        cidList[0] = count; // list[0] indicate cid list length
        for (int i = 0; i < count; i++) {
            cidList[i + 1] = (int)retList[i];
        }
        sendEvent(RFX_MSG_EVENT_URSP_REEVALUATE,
                    RfxIntsData(cidList, count + 1), RIL_CMD_PROXY_5, m_slot_id);
        free(cidList);
     }
}

void RmmDcUrcHandler::onUrspUePolicyChanged(const sp<RfxMclMessage>& msg) {
    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]", m_slot_id, __FUNCTION__);
    uint16_t valLen = 0;
    uint8_t *valList = (uint8_t*)mipc_data_ursp_ue_policy_chg_ind_get_plmn_id(
           msg->getMipcData()->getData(), &valLen);

    RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] PLMN length:%d", m_slot_id, __FUNCTION__, valLen);

    if (valLen < MIN_PLMN_LENTH) {
        RFX_LOG_W(RFX_LOG_TAG, "[%d][%s] invalide PLMN, length:%d",
                m_slot_id, __FUNCTION__, valLen);
        return;
    }

    String8 plmn("");
    for (int i = 0; i < valLen; i++) {
        // In order to be compatible with different MD versions,
        // we need check whether the 6th number is valid
        if (i == MIN_PLMN_LENTH && valList[i] == 0) {
            break;
        }
        plmn.append(String8::format("%c", valList[i]));
    }

    String8 homePlmn = getMclStatusManager()->getString8Value(
            RFX_STATUS_KEY_UICC_GSM_NUMERIC, String8("0"));

    RFX_LOG_I(RFX_LOG_TAG, "[%d][%s] PLMN length:%d, PLMN:%s, home PLMN:%s",
            m_slot_id, __FUNCTION__, valLen, plmn.string(), homePlmn.string());
    sendEvent(RFX_MSG_EVENT_UE_POLICY_CHANGED, RfxVoidData(), RIL_CMD_PROXY_5, m_slot_id);
}

void RmmDcUrcHandler::onAttachApnNotify(const sp<RfxMclMessage>& msg) {
    char *apn = (char *)msg->getMipcData()->getMipcStr(MIPC_NW_IA_IND_T_APN);
    if (apn != NULL && RmmDcUtility::isVerizonOnSiteLteSimCard(m_slot_id)) {
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]:%s", m_slot_id, __FUNCTION__, apn);
        sendEvent(RFX_MSG_EVENT_ATTACH_APN_CHANGED, RfxStringData(apn), RIL_CMD_PROXY_5, m_slot_id);
    }
}
