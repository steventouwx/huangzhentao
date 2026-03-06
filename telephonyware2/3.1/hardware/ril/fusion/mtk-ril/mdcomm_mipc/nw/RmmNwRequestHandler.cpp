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

#include "RmmNwRequestHandler.h"
#include "libmtkrilutils.h"
#include <string>
#include <telephony/mtk_ril.h>
#include "RfxMipcData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxSsrcData.h"
#include "RfxStringData.h"
#include "RfxCellInfoData.h"
#include "RfxRegistrationFailedData.h"
#include "RfxViaUtils.h"
#include "ViaBaseHandler.h"
#include "mipc_msg_nw_const.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include <cutils/memory.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmmNwReqHdlr"

RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_REFRESH_CELL_INFO_LIST);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_REGISTRATION_FAILED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_RSP_DATA_CONTEXT_IDS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_OEM_HOOK_TO_NW);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_5G_ICON_SHOW_RULE_SYNC_EVENT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxCellInfoData, RFX_MSG_REQUEST_GET_CELL_INFO_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_SIGNAL_STRENGTH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData,
        RFX_MSG_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_GET_TOE_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_CARRIER_AGGREGATION_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_GET_CA_LINK_ENABLE_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_CA_LINK_ENABLE_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringsData, RFX_MSG_REQUEST_GET_CA_LINK_CAPABILITY_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_LTE_DATA);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_LTE_RRC_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_LTE_BAND_ENABLE_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_BAND_PRIORITY_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_BAND_PRIORITY_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_4X4MIMO_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_4X4MIMO_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_LTE_BSR_TIMER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_LTE_BSR_TIMER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_LTE_1XRTT_CELL_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_CLEAR_LTE_AVAILABLE_FILE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_CA_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_QAM_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_QAM_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_TM9_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_TM9_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_LTE_SCAN_DURATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_LTE_SCAN_DURATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_NRDC_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_NRDC_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SMART_RAT_SWITCH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxBarringInfoData, RFX_MSG_REQUEST_GET_BARRINGINFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringData, RFX_MSG_REQUEST_GET_TS25_NAME);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_ALL_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_NR_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_USAGE_SETTING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_USAGE_SETTING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSsrcData, RfxVoidData, RFX_MSG_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_N1_MODE_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_N1_MODE_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_GET_NITZ);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_FEMTOCELL_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_NR_OPTION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED);
// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmNwRequestHandler, RIL_CMD_PROXY_3);

RmmNwRequestHandler::RmmNwRequestHandler(int slot_id, int channel_id) :
        RmmNwHandler(slot_id, channel_id) {
        const int events[] = {
            RFX_MSG_EVENT_REFRESH_CELL_INFO_LIST,
            RFX_MSG_EVENT_FEMTOCELL_UPDATE,
            RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM,
            RFX_MSG_EVENT_REGISTRATION_FAILED,
            RFX_MSG_EVENT_RSP_DATA_CONTEXT_IDS,
            RFX_MSG_EVENT_CS_NETWORK_STATE,
            RFX_MSG_EVENT_PS_NETWORK_STATE,
            RFX_MSG_EVENT_WFC_STATE,
            RFX_MSG_EVENT_EXIT_EMERGENCY_CALLBACK_MODE,
            RFX_MSG_EVENT_OEM_HOOK_TO_NW,
            RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG,
            RFX_MSG_EVENT_5G_ICON_SHOW_RULE_SYNC_EVENT
        };

        const int request[] = {
            RFX_MSG_REQUEST_SET_N1_MODE_ENABLED,
            RFX_MSG_REQUEST_IS_N1_MODE_ENABLED,
            RFX_MSG_REQUEST_SET_USAGE_SETTING,
            RFX_MSG_REQUEST_GET_USAGE_SETTING,
            RFX_MSG_REQUEST_GET_ALL_BAND_MODE,
            RFX_MSG_REQUEST_SET_NR_OPTION,
            RFX_MSG_REQUEST_GET_CELL_INFO_LIST,
            RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE,
            RFX_MSG_REQUEST_SIGNAL_STRENGTH,
            RFX_MSG_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO,
            RFX_MSG_REQUEST_SMART_RAT_SWITCH,
            RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH,
            RFX_MSG_REQUEST_GET_BARRINGINFO,
            RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE,
            RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE,
            RFX_MSG_REQUEST_GET_TS25_NAME,
            RFX_MSG_REQUEST_GET_TOE_INFO,
            RFX_MSG_REQUEST_SET_CARRIER_AGGREGATION_MODE,
            RFX_MSG_REQUEST_GET_CA_LINK_ENABLE_STATUS,
            RFX_MSG_REQUEST_SET_CA_LINK_ENABLE_STATUS,
            RFX_MSG_REQUEST_GET_CA_LINK_CAPABILITY_LIST,
            RFX_MSG_REQUEST_GET_LTE_DATA,
            RFX_MSG_REQUEST_GET_LTE_RRC_STATE,
            RFX_MSG_REQUEST_SET_LTE_BAND_ENABLE_STATUS,
            RFX_MSG_REQUEST_GET_BAND_PRIORITY_LIST,
            RFX_MSG_REQUEST_SET_BAND_PRIORITY_LIST,
            RFX_MSG_REQUEST_SET_4X4MIMO_ENABLED,
            RFX_MSG_REQUEST_GET_4X4MIMO_ENABLED,
            RFX_MSG_REQUEST_GET_LTE_BSR_TIMER,
            RFX_MSG_REQUEST_SET_LTE_BSR_TIMER,
            RFX_MSG_REQUEST_GET_LTE_1XRTT_CELL_LIST,
            RFX_MSG_REQUEST_CLEAR_LTE_AVAILABLE_FILE,
            RFX_MSG_REQUEST_GET_BAND_MODE,
            RFX_MSG_REQUEST_GET_CA_BAND_MODE,
            RFX_MSG_REQUEST_SET_NR_BAND_MODE,
            RFX_MSG_REQUEST_SET_QAM_ENABLED,
            RFX_MSG_REQUEST_GET_QAM_ENABLED,
            RFX_MSG_REQUEST_SET_TM9_ENABLED,
            RFX_MSG_REQUEST_GET_TM9_ENABLED,
            RFX_MSG_REQUEST_SET_LTE_SCAN_DURATION,
            RFX_MSG_REQUEST_GET_LTE_SCAN_DURATION,
            RFX_MSG_REQUEST_SET_NRDC_STATE,
            RFX_MSG_REQUEST_GET_NRDC_STATE,
            RFX_MSG_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA,
            RFX_MSG_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED,
            RFX_MSG_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED,
            RFX_MSG_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
            RFX_MSG_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
            RFX_MSG_REQUEST_GET_NITZ
        };

        logV(LOG_TAG, "RmmNwRequestHandler[%d] start", slot_id);
        registerToHandleEvent(events, sizeof(events)/sizeof(int));
        registerToHandleRequest(request, sizeof(request)/sizeof(int));

        if (!s_enwscn_support) {
            sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_NETWORK_SCAN_SUPPORT_STATUS_REQ, m_slot_id);

            sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

            if (cnfData != NULL && cnfData->getResult() == MIPC_RESULT_SUCCESS) {
                s_enwscn_support = true;
                logD(LOG_TAG, "s_enwscn_support = %d", s_enwscn_support);
            }
        }

        if (slot_id == 0) { // only do it once
            char feature[] = "GEMINI_NR_PLUS_SUPPORTED";
            char NR_VER[] = "5G_SUPPORTED";

            if (getFeatureVersion(NR_VER, 0) == 1 && getFeatureVersion(feature, 0) == 0) {
                char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
                int val = 0;
                rfx_property_get(PROPERTY_NR_SLOTID, stgBuf, "0");
                val = strtol(stgBuf, NULL, 10);
                if (val == 0) {
                    val = 1;
                    rfx_property_set(PROPERTY_NR_SLOTID, String8::format("%d", 1).string());
                }
            } else if (getFeatureVersion(NR_VER, 0) == 1 && getFeatureVersion(feature, 0) == 1) {
                char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
                int val = 0;
                rfx_property_get(PROPERTY_NR_SLOTID, stgBuf, "-1");
                val = strtol(stgBuf, NULL, 10);
                if (val != -1) {
                    rfx_property_set(PROPERTY_NR_SLOTID, String8::format("%d", 0).string());
                }
            }
        }
    mCellInfoListRetryCountLeft = 0;
}

RmmNwRequestHandler::~RmmNwRequestHandler() {
}

void RmmNwRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    logV(LOG_TAG, "onHandleRequest:%s", RFX_ID_TO_STR(request));
    switch (request) {
        case RFX_MSG_REQUEST_GET_CELL_INFO_LIST:
            requestGetCellInfoList(msg);
            break;

        case RFX_MSG_REQUEST_SIGNAL_STRENGTH:
        case RFX_MSG_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO:
            requestSignalStrength(msg, false);
            break;

        case RFX_MSG_REQUEST_SMART_RAT_SWITCH:
            requestSmartRatSwitch(msg);
            break;

        case RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH:
            requestGetSmartRatSwitch(msg);
            break;

        case RFX_MSG_REQUEST_GET_BARRINGINFO:
            requestGetBarringInfo(msg);
            break;

        case RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE:
            requestSetCdmaRoamingPreference(msg);
            break;

        case RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE:
            requestQueryCdmaRoamingPreference(msg);
            break;

        case RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE:
            requestSetUnsolCellInfoListRate(msg);
            break;

        case RFX_MSG_REQUEST_GET_TS25_NAME:
            requestGetTs25Name(msg);
            break;

        case RFX_MSG_REQUEST_SET_NR_OPTION:
            requestSetNrOption(msg);
            break;

        case RFX_MSG_REQUEST_GET_TOE_INFO:
            requestGetTOEInfo(msg);
            break;
        case RFX_MSG_REQUEST_SET_CARRIER_AGGREGATION_MODE:
            requestSetCarrierAggregationMode(msg);
            break;
        case RFX_MSG_REQUEST_GET_CA_LINK_ENABLE_STATUS:
            requestGetCALinkEnableStatus(msg);
            break;
        case RFX_MSG_REQUEST_SET_CA_LINK_ENABLE_STATUS:
            requestSetCALinkEnableStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_CA_LINK_CAPABILITY_LIST:
            requestGetCALinkCapabilityList(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_DATA:
            requestGetLteData(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_RRC_STATE:
            requestGetLteRRCState(msg);
            break;
        case RFX_MSG_REQUEST_SET_LTE_BAND_ENABLE_STATUS:
            requestSetLteBandEnableStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_BAND_PRIORITY_LIST:
            requestGetBandPriorityList(msg);
            break;
        case RFX_MSG_REQUEST_SET_BAND_PRIORITY_LIST:
            requestSetBandPriorityList(msg);
            break;
        case RFX_MSG_REQUEST_SET_4X4MIMO_ENABLED:
            requestSet4x4MimoEnabled(msg);
            break;
        case RFX_MSG_REQUEST_GET_4X4MIMO_ENABLED:
            requestGet4x4MimoEnabled(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_BSR_TIMER:
            requestGetLteBsrTimer(msg);
            break;
        case RFX_MSG_REQUEST_SET_LTE_BSR_TIMER:
            requestSetLteBsrTimer(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_1XRTT_CELL_LIST:
            requestGetLte1xRttCellList(msg);
            break;
        case RFX_MSG_REQUEST_CLEAR_LTE_AVAILABLE_FILE:
            requestClearLteAvailableFile(msg);
            break;
        case RFX_MSG_REQUEST_GET_BAND_MODE:
            requestGetBandMode(msg);
            break;
        case RFX_MSG_REQUEST_GET_CA_BAND_MODE:
            requestGetCaBandMode(msg);
            break;
        case RFX_MSG_REQUEST_SET_QAM_ENABLED:
            requestSetQamEnabled(msg);
            break;
        case RFX_MSG_REQUEST_GET_QAM_ENABLED:
            requestGetQamEnabled(msg);
            break;
        case RFX_MSG_REQUEST_SET_TM9_ENABLED:
            requestSetTm9Enabled(msg);
            break;
        case RFX_MSG_REQUEST_GET_TM9_ENABLED:
            requestGetTm9Enabled(msg);
            break;
        case RFX_MSG_REQUEST_SET_LTE_SCAN_DURATION:
            requestSetLteScanDuration(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_SCAN_DURATION:
            requestGetLteScanDuration(msg);
            break;

        case RFX_MSG_REQUEST_SET_NRDC_STATE:
            requestSetNrdcState(msg);
            break;

        case RFX_MSG_REQUEST_GET_NRDC_STATE:
            requestGetNrdcState(msg);
            break;

        case RFX_MSG_REQUEST_GET_ALL_BAND_MODE:
            requestGetAllBandMode(msg);
            break;

        case RFX_MSG_REQUEST_SET_NR_BAND_MODE:
            requestSetNrBandMode(msg);
            break;
        case RFX_MSG_REQUEST_SET_USAGE_SETTING:
            requestSetUsageSetting(msg);
            break;
        case RFX_MSG_REQUEST_GET_USAGE_SETTING:
            requestGetUsageSetting(msg);
            break;
        case RFX_MSG_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA:
            handleSetSignalStrengthReportingCriteriaRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_N1_MODE_ENABLED:
            requestSetN1ModeEnabled(msg);
            break;
        case RFX_MSG_REQUEST_IS_N1_MODE_ENABLED:
            requestGetN1ModeEnabled(msg);
            break;
        case RFX_MSG_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED:
            requestSetNullCipherAndIntegrityEnabled(msg);
            break;
        case RFX_MSG_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED:
            requestIsNullCipherAndIntegrityEnabled(msg);
            break;
        case RFX_MSG_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED:
            requestIsCellularIdentifierTransparencyEnabled(msg);
            break;
        case RFX_MSG_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED:
            requestSetCellularIdentifierTransparencyEnabled(msg);
            break;
        case RFX_MSG_REQUEST_GET_NITZ:
            requestGetNitz(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

/*
void RmmNwRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {

}
*/

void RmmNwRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logV(LOG_TAG, "onHandleEvent: msg id:%d", id);
    switch (id) {
        case RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM:
            onHandleRetryNotifySystem(msg);
            break;

        case RFX_MSG_EVENT_REGISTRATION_FAILED:
            handleRegistrationFailedEvent(msg);
            break;

        case RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG:
            sendPhysicalChannelConfigs();
            break;

        case RFX_MSG_EVENT_RSP_DATA_CONTEXT_IDS:
            if (handleGetDataContextIds(msg)) {
                sendPhysicalChannelConfigs(true);
            }
            break;

        case RFX_MSG_EVENT_CS_NETWORK_STATE:
            handleCsNetworkStateEvent(msg);
            break;

        case RFX_MSG_EVENT_PS_NETWORK_STATE:
            handlePsNetworkStateEvent(msg);
            break;

        case RFX_MSG_EVENT_FEMTOCELL_UPDATE:
            updateFemtoCellInfo(msg);
            break;

        case RFX_MSG_EVENT_WFC_STATE:
            handleWfcStateChanged(msg);
            break;

        case RFX_MSG_EVENT_EXIT_EMERGENCY_CALLBACK_MODE:
            triggerPollNetworkState();
            break;

        case RFX_MSG_EVENT_OEM_HOOK_TO_NW:
            onListenAtFromOemHook(msg);
            break;

        case RFX_MSG_EVENT_5G_ICON_SHOW_RULE_SYNC_EVENT:
            onSync5GIconShowRuleWithOtherSlot(msg);
            break;

        case RFX_MSG_EVENT_REFRESH_CELL_INFO_LIST:
            handleRefreshCellInfoListEvent(msg);
            break;

        default:
            logD(LOG_TAG, "onHandleEvent: should not be here");
            break;
    }
}

void RmmNwRequestHandler::handleRegistrationFailedEvent(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> urc = NULL;
    int *pInt = (int *)msg->getData()->getData();
    RIL_Domain domain = (RIL_Domain) pInt[0];
    RIL_RegistrationFailedResponse *rilRegFailResp =
            (RIL_RegistrationFailedResponse*) calloc(1, sizeof(RIL_RegistrationFailedResponse));
    logD(LOG_TAG, "[%s] domain=%d", __FUNCTION__, domain);
    if (rilRegFailResp == NULL) goto error;
    if (updatePlmnInfo()  < 0) goto error;
    pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
    strlcpy(rilRegFailResp->plmn, sOpInfoCache[m_slot_id]->mccMnc, 8);
    pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
    rilRegFailResp->domain = domain;
    rilRegFailResp->additionalCauseCode = INT_MAX;
    switch (domain) {
        case RIL_DOMAIN_CS:
            fillVoiceCellIdentity(rilRegFailResp->cellIdentity);
            rilRegFailResp->causeCode = sCsRegStateCache[m_slot_id]->reject_cause;
            break;

        case RIL_DOMAIN_PS:
            fillDataCellIdentity(rilRegFailResp->cellIdentity);
            rilRegFailResp->causeCode = sPsRegStateCache[m_slot_id]->reject_cause;
            break;

        default:
            break;
    }
    if (rilRegFailResp->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_NONE) {
        logE(LOG_TAG, "[%s] no valid cell info", __FUNCTION__);
        goto error;
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_REGISTRATION_FAILED, m_slot_id,
            RfxRegistrationFailedData(rilRegFailResp, sizeof(RIL_RegistrationFailedResponse)));
    responseToTelCore(urc);

error:
    if (rilRegFailResp) {
        clearCellIdentity(rilRegFailResp->cellIdentity);
        free(rilRegFailResp);
        rilRegFailResp = NULL;
    }
    return;
}

int RmmNwRequestHandler::updatePlmnInfo() {
    sp<RfxMipcData> requestData =
        RfxMipcData::obtainMipcData(MIPC_NW_GET_REGISTER_STATE_REQ, m_slot_id);
    sp<RfxMipcData> confirmData = callToMipcMsgSync(requestData);
    sp<RfxMclMessage> response;
    if (confirmData->getResult() == MIPC_RESULT_SUCCESS) {
        uint16_t rspDataLen = 0;
        mipc_nw_reg_state_struct4* pInfo =
            (mipc_nw_reg_state_struct4*)(confirmData->getMipcVal(MIPC_NW_GET_REGISTER_STATE_CNF_T_STATE, &rspDataLen));
        if (pInfo && rspDataLen == sizeof(mipc_nw_reg_state_struct4)) {
            pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
            strlcpy(sOpInfoCache[m_slot_id]->mccMnc, pInfo->plmn, 8);
            pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
            return 0;
        } else {
            logE(LOG_TAG, "[%s] ptr:%p, len:%d, struct len:%d", __FUNCTION__, pInfo, (int)rspDataLen,
                 (int)sizeof(mipc_nw_reg_state_struct4));
        }
    }

    logE(LOG_TAG, "[%s] failed", __FUNCTION__);
    return -1;
}

void RmmNwRequestHandler::requestGetCellInfoList(const sp<RfxMclMessage>& msg) {
    int err, num = 0;
    RIL_CellInfo_v12 *data = NULL;
    sp<RfxMclMessage> resp;

    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_NW_GET_BASE_STATIONS_REQ,
            m_slot_id);
    sp<RfxMipcData> confirmData = callToMipcMsgSync(requestData);
    if (confirmData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    //total num
    num = confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_COUNT, 0);
    logD(LOG_TAG, "requestGetCellInfoList +++++, num=%d", num);

    if (num > 0) {
        RIL_CELl_INFO_TLV_KEYS keys = {
            MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST_V1,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST_V1
        };
        data = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
        memset(data, 0, num * sizeof(RIL_CellInfo_v12));
        err = getCellInfoListV12(confirmData.get(), data, keys);
        if (err == 0) {
            resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxCellInfoData(data, num * sizeof(RIL_CellInfo_v12)), msg, false);
            responseToTelCore(resp);
        }
        freeOperatorNameOfCellInfo(num, data);
        if (err == 0) {
            return;
        }
    }

error:
    // if error and last valid PLMN or CDMA PLMN is available, use it for fwk's country code.
    char valid_plmn[8];
    int mnc_len = 2;
    if (getLastValidPlmn(valid_plmn, 8) || getCdmaPlmn(valid_plmn, 8)) {
        if (strlen(valid_plmn) == 6) mnc_len = 3;
        char mcc_str[4] = {0};
        char mnc_str[4] = {0};
        char empty_string = '\0';
        strncpy(mcc_str, valid_plmn, 3);
        strncpy(mnc_str, valid_plmn+3, (strlen(valid_plmn)-3));

        num = 1;
        data = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
        memset(data, 0, num * sizeof(RIL_CellInfo_v12));
        data[0].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
        data[0].registered = 0;
        data[0].timeStampType = RIL_TIMESTAMP_TYPE_UNKNOWN;
        data[0].timeStamp = 0;
        data[0].connectionStatus = NONE_SERVING;
        data[0].CellInfo.gsm.cellIdentityGsm.mnc_len = mnc_len;
        data[0].CellInfo.gsm.cellIdentityGsm.mcc = atoi(mcc_str);
        data[0].CellInfo.gsm.cellIdentityGsm.mnc = atoi(mnc_str);
        data[0].CellInfo.gsm.cellIdentityGsm.operName.long_name = &empty_string;
        data[0].CellInfo.gsm.cellIdentityGsm.operName.short_name = &empty_string;
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
               RfxCellInfoData(data, num * sizeof(RIL_CellInfo_v12)), msg, false);
        responseToTelCore(resp);
    } else {
        logE(LOG_TAG, "requestGetCellInfoList must never return error when radio is on");
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_NO_NETWORK_FOUND,
               RfxVoidData(), msg, false);
        responseToTelCore(resp);
    }

    RIL_CELL_INFO_CACHE *pCellCache = sPrimaryCellInfoCache[m_slot_id];
    dumpPrimaryCellInfoCache(__func__, pCellCache);
    return;
}

bool RmmNwRequestHandler::handleGetDataContextIds(const sp<RfxMclMessage>& msg) {
    bool changed = false;
    int tmpContextIds[16] = {};
    int* pInt = (int *)msg->getData()->getData();
    int size = msg->getData()->getDataLength() / sizeof(int);
    memcpy(tmpContextIds, mDataContextIds, 16 * sizeof(int));
    for (int i = 0; i < 16; i++) {
        mDataContextIds[i] = (i < size) ? pInt[i] : -1;
    }
    logD(LOG_TAG, "[%s] [%d, %d, %d, %d, ...]", __FUNCTION__, mDataContextIds[0], mDataContextIds[1],
         mDataContextIds[2], mDataContextIds[3]);
    if (memcmp(tmpContextIds, mDataContextIds, 16 * sizeof(int)) != 0)
        changed = true;
    return changed;
}

void RmmNwRequestHandler::fillCidToPhysicalChannelConfig(RIL_PhysicalChannelConfig* pcc) {
    pcc->num_cids = 0;
    for (int i = 0; i < 16; i++) {
        if (mDataContextIds[i] > 0) {
            pcc->num_cids += 1;
            pcc->contextIds[i] = mDataContextIds[i];
        }
    }
}

void RmmNwRequestHandler::sendPhysicalChannelConfigs(bool forceUpdate) {
    logV(LOG_TAG, "[%s] entry", __FUNCTION__);
    if (!sPhysicalConfigSwitch[m_slot_id]) return;  // It's off.

    RIL_PhysicalChannelConfig* pCache = sPhysicalConfigCache[m_slot_id];
    RIL_PhysicalChannelConfig* pNotified = sPhysicalConfigNotified[m_slot_id];
    pthread_mutex_lock(&sPhysicalConfigMutex[m_slot_id]);

    int count = sPhysicalConfigCount[m_slot_id];
    for (int i = 0; i < count; i++) {
        fillCidToPhysicalChannelConfig(&pCache[i]);
    }

    //if ps is OOS and cs in service,
    //no need send physical channel config URC
    if (!isInService(convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false))) {
        logV(LOG_TAG, "[%s] ps not in service send empty", __FUNCTION__);
        count  = 0;
    }

    if (count > 0) {
        int psNetworkType = convertPSNetworkType(sPsRegStateCache[m_slot_id]->radio_technology);
        if (psNetworkType == 14 || psNetworkType == 19) {//LTE
            if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_LTE_RRC_STATE, 1) != 1) {
                logV(LOG_TAG, "[%s] lte rrc idel state, send empty", __FUNCTION__);
                count  = 0;
            }
        }
        if (psNetworkType == 20) {//SA
            pthread_mutex_lock(&nr_connection_state_Mutex[m_slot_id]);
            int t_nr_connection_state = nr_connection_state[m_slot_id];
            pthread_mutex_unlock(&nr_connection_state_Mutex[m_slot_id]);
            if (t_nr_connection_state != 1) {
                logV(LOG_TAG, "[%s] sa idel state, send empty", __FUNCTION__);
                count = 0;
            }
        }

        if (psNetworkType == 6 || psNetworkType == 8 || psNetworkType == 13) {//c2k
            logD(LOG_TAG, "[%s] cdma, send empty", __FUNCTION__);
            count = 0;
        }
    }

    bool needUpdate = false;
    if (forceUpdate) needUpdate = true;
    if (sPhysicalConfigCountNotified[m_slot_id] != count) {
        needUpdate = true;
    }
    if (!needUpdate && sPhysicalConfigCount[m_slot_id] > 0 && sPhysicalConfigCountNotified[m_slot_id] > 0) {
        for (int i = 0; i < sPhysicalConfigCount[m_slot_id]; i++) {
            if (pCache[i].rat != pNotified[i].rat
                || pCache[i].status != pNotified[i].status
                || pCache[i].cellBandwidthDownlink != pNotified[i].cellBandwidthDownlink
                || pCache[i].cellBandwidthUplink != pNotified[i].cellBandwidthUplink
                || pCache[i].downlinkChannelNumber != pNotified[i].downlinkChannelNumber
                || pCache[i].uplinkChannelNumber != pNotified[i].uplinkChannelNumber
                || pCache[i].band != pNotified[i].band
                || pCache[i].physicalCellId != pNotified[i].physicalCellId) {
                needUpdate = true;
                break;
            }
        }
    }

    if (needUpdate) {
        // if count = 0, send an empty one to clear
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_PHYSICAL_CHANNEL_CONFIGS_MTK, m_slot_id,
                RfxIntsData((void*)pCache, count * sizeof(RIL_PhysicalChannelConfig)));
        responseToTelCore(urc);

        sPhysicalConfigCountNotified[m_slot_id] = count;
        memcpy(sPhysicalConfigNotified[m_slot_id], pCache, count*sizeof(RIL_PhysicalChannelConfig));
    }

    logD(LOG_TAG, "[%s] count:%u, Update: %d", __FUNCTION__, count, needUpdate);
    pthread_mutex_unlock(&sPhysicalConfigMutex[m_slot_id]);
}

void RmmNwRequestHandler::handleCsNetworkStateEvent(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *) msg->getData()->getData();
    int prevRegState = pInt[0];
    int prevRat = pInt[1];
    int newRegState = pInt[2];
    int newRat = pInt[3];
    if ((prevRegState != newRegState
            || prevRat != newRat)
            && RfxNwServiceState::isInService(newRegState)) {
        logV(LOG_TAG, "[%s], need requestSignalStrength, "
             "prevRegState=%d, prevRat=%d, newRegState=%d, newRat=%d",
             __FUNCTION__, prevRegState, prevRat, newRegState, newRat);
        requestSignalStrength(NULL, false);
    }
}

void RmmNwRequestHandler::handlePsNetworkStateEvent(const sp<RfxMclMessage>& msg) {
    // response[0] = state, response[1] = mccmnc, response[2] = rat.
    int response[3];
    sp<RfxMclMessage> urc;
    bool forceRefresh = false;
    int *pInt = (int *)msg->getData()->getData();
    // state (consider iwlan)
    response[0] = pInt[2];
    // rat (consider iwlan)
    response[2] = pInt[3];

    // Update SignalStrength for PS state change
    int preRegState = pInt[0];
    int preRat = pInt[1];
    int newRegState = pInt[2];
    int newRat = pInt[3];
    if ((preRegState != newRegState || preRat != newRat)
        && RfxNwServiceState::isInService(newRegState)) {
        logV(LOG_TAG, "[%s], need requestSignalStrength, "
             "preRegState=%d, preRat=%d, newRegState=%d, newRat=%d",
             __FUNCTION__, preRegState, preRat, newRegState, newRat);
        requestSignalStrength(NULL, false);
    }

    if (isInService(preRegState) != isInService(newRegState)) {
        forceRefresh = true;
    }
    refreshCellInfoList(forceRefresh);
    if (sUrcNrCaBandEnable && isInService(preRegState) &&
        (!isInService(newRegState) || (preRat == RADIO_TECH_NR && newRat != RADIO_TECH_NR))) {
        int data[3] = {0};
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_CA_BAND_IND, m_slot_id, RfxIntsData(data, 3));
        responseToTelCore(urc);
    }

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_REGISTER_STATE_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    response[1] = 0;
    if (cnfData->getResult() == MIPC_RESULT_SUCCESS) {
        char *pPlmn = cnfData->getMipcStr(MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_ID);
        if (pPlmn && (pPlmn[0] != '\0')) {
            response[1] = atoi(pPlmn);
        }
    }
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED,
                                                     m_slot_id, RfxIntsData(response, 3));
    responseToTelCore(urc);
    return;
}

void RmmNwRequestHandler::requestSmartRatSwitch(const sp<RfxMclMessage>& msg) {
    int mode = 0, rat = 0;

    sp<RfxMclMessage> response = NULL;
    int *pInt = (int *)msg->getData()->getData();

    mode = pInt[0]; // SMART_RAT_SWITCH_MODE
    rat = pInt[1]; // 0: 4G, 1: 5G

    logD(LOG_TAG, "requestSmartRatSwitch, mode = %d, rat = %d", mode, rat);

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_SET_SMART_RAT_SWITCH_REQ, m_slot_id);
    reqData->addMipcTlvUint8(MIPC_NW_SET_SMART_RAT_SWITCH_REQ_T_MODE, (uint8_t)mode);
    reqData->addMipcTlvUint8(MIPC_NW_SET_SMART_RAT_SWITCH_REQ_T_RAT, (uint8_t)rat);

    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the requestSmartRatSwitch");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetSmartRatSwitch(const sp<RfxMclMessage>& msg) {
    int mode = 0, state = 0;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    mode = pInt[0];


    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_SMART_RAT_SWITCH_REQ, m_slot_id);
    reqData->addMipcTlvUint8(MIPC_NW_GET_SMART_RAT_SWITCH_REQ_T_MODE, (uint8_t)mode);

    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    state = cnfData->getMipcUint8Val(MIPC_NW_GET_SMART_RAT_SWITCH_CNF_T_STATE, 0);

    logD(LOG_TAG, "requestGetSmartRatSwitch, mode = %d, state = %d", mode, state);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&state, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the requestGetSmartRatSwitch");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetBarringInfo(const sp<RfxMclMessage>& msg) {
    const int umtsTypes[] = {0, 1, 2, 8}; /*CS_SERVICE, PS_SERVICE, CS_VOICE, EMERGENCY*/
    const int lteTypes[] = {3, 4, 5, 6, 7, 8, 9};
    const int saTypes[] = {3, 4, 5, 6, 7, 8, 9, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
                1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022,
                1023, 1024, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032};

    sp<RfxMclMessage> response;
    int err = 0, count = 0, rat = 0, i, j, number;
    const int *pType;
    sp<RfxMipcData> reqData = NULL, cnfData = NULL;

    RIL_BarringInforResponse *barringInfoResp =
            (RIL_BarringInforResponse*) calloc(1, sizeof(RIL_BarringInforResponse));
    RIL_BarringInfo *pBarringInfos = NULL;
    if (barringInfoResp == NULL) goto error;
    memset(barringInfoResp, 0, sizeof(RIL_BarringInforResponse));

    reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_BARRING_INFO_REQ, m_slot_id);
    cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    count = cnfData->getMipcUint8Val(MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT, 0);
    rat = cnfData->getMipcUint8Val(MIPC_NW_GET_BARRING_INFO_CNF_T_RAT, 0);
    logD(LOG_TAG, "[%s]: count = %d, rat = %d", __FUNCTION__, count, rat);
    if (rat == 2) { //UMTS
        number = 4;
        pType = umtsTypes;
    } else if (rat == 4) {//LTE
        number = 7;
        pType = lteTypes;
    } else if (rat == 128) {
        number = 39;
        pType = saTypes;
    } else {
        // not support
        logE(LOG_TAG, "[%s]unknown rat: %d", __FUNCTION__, rat);
        goto error;
    }

    pBarringInfos = (RIL_BarringInfo *) calloc(number, sizeof(RIL_BarringInfo));
    if (pBarringInfos == NULL) goto error;
    for ( i = 0; i < number; i++) {
        pBarringInfos[i].service_type = pType[i];
        pBarringInfos[i].barring_type = BT_NONE;
    }

    if (count > 0) {
        uint16_t len = 0;
        mipc_nw_barring_info_struct4* pBarringInfoList = NULL;

        pBarringInfoList =
            (mipc_nw_barring_info_struct4*)cnfData->getMipcVal(MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST, &len);

        if (pBarringInfoList == NULL || len != count * sizeof(mipc_nw_barring_info_struct4)) {
            logE(LOG_TAG, "[%s] failed, pBarringInfoList:%p, len:%d, struct len:%d",
                 __FUNCTION__, pBarringInfoList, len, (int) (count * sizeof(mipc_nw_barring_info_struct4)));
            goto error;
        }

        for (int i = 0; i < count; i++) {
            // find the item and replace the values
            for (j = 0; j < number; j++) {
                if (pBarringInfos[j].service_type == pBarringInfoList->service_type) {
                    pBarringInfos[j].barring_type = (RIL_BarringType)pBarringInfoList->barring_type;
                    pBarringInfos[j].factor = pBarringInfoList->factor;
                    pBarringInfos[j].timeSeconds = pBarringInfoList->time_seconds;
                    pBarringInfos[j].isBarred = pBarringInfoList->is_barred > 0 ? true : false ;
                    break;
                }
            }
            pBarringInfoList++;
        }
    }
    err = fillBarringInfosResponse((size_t) number, pBarringInfos, barringInfoResp);
    if (err < 0) goto error;

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxBarringInfoData(barringInfoResp, sizeof(RIL_BarringInforResponse)), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    clearCellIdentity(barringInfoResp->cellIdentity);
    free(barringInfoResp);
    barringInfoResp = NULL;
    free(pBarringInfos);
    return;

error:
    logE(LOG_TAG, "requestGetBarringInfo: there is a err");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    if (barringInfoResp) {
        clearCellIdentity(barringInfoResp->cellIdentity);
        free(barringInfoResp);
        barringInfoResp = NULL;
    }
    if (pBarringInfos) {
        free(pBarringInfos);
    }
    return;
}

void RmmNwRequestHandler::requestSetCdmaRoamingPreference(const sp<RfxMclMessage>& msg) {
    int reqRoamingType = 0xFF;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> reqData = NULL, cnfData = NULL;
    RIL_Errno ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;

    int *pInt = (int *) msg->getData()->getData();
    reqRoamingType = pInt[0];

    logD(LOG_TAG, "requestSetCdmaRoamingPreference, reqRoamingType = %d", reqRoamingType);

    if (reqRoamingType != CDMA_ROAMING_MODE_HOME &&
        reqRoamingType != CDMA_ROAMING_MODE_ANY) {
        logE(LOG_TAG, "requestSetCdmaRoamingPreference, Not support reqRoamingType=%d", reqRoamingType);
        goto error;
    }

    reqData = RfxMipcData::obtainMipcData(MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_REQ, m_slot_id);
    reqData->addMipcTlvUint8(MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_REQ_T_ROAMING_TYPE, (uint8_t)reqRoamingType);

    cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the requestSetCdmaRoamingPreference");
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);

}

void RmmNwRequestHandler::requestQueryCdmaRoamingPreference(const sp<RfxMclMessage>& msg) {
    int roamingType = 0xFF;
    sp<RfxMclMessage> response;
    RIL_Errno ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_REQ, m_slot_id);

    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    roamingType = cnfData->getMipcUint8Val(MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_CNF_T_ROAMING_TYPE, 0xFF);

    logD(LOG_TAG, "requestQueryCdmaRoamingPreference, roamingType = %d", roamingType);

    if (roamingType != CDMA_ROAMING_MODE_HOME &&
        roamingType != CDMA_ROAMING_MODE_ANY) {
        logE(LOG_TAG, "requestQueryCdmaRoamingPreference, Not support roamingType=%d", roamingType);
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&roamingType, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "requestQueryCdmaRoamingPreference error");
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestSetUnsolCellInfoListRate(const sp<RfxMclMessage>& msg) {
    int time = -1;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int *pInt = (int *)msg->getData()->getData();
    time = pInt[0];
    logD(LOG_TAG, "requestSetCellInfoListRate:%d", time);
    RIL_Errno errNum = RIL_E_SUCCESS;
    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_NW_SET_URC_ENABLE_REQ,
            m_slot_id);
    sp<RfxMipcData> confirmData;
    requestData->addMipcTlvUint8(MIPC_NW_SET_URC_ENABLE_REQ_T_TYPE, MIPC_NW_IND_TYPE_ECELL);
    if (time == 0) { //  enable
        requestData->addMipcTlvUint8(MIPC_NW_SET_URC_ENABLE_REQ_T_ENABLE, 1);
    } else if (time > 0) { //  disable
        requestData->addMipcTlvUint8(MIPC_NW_SET_URC_ENABLE_REQ_T_ENABLE, 0);
    } else {
        errNum = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    confirmData = callToMipcMsgSync(requestData);
    if (confirmData->getResult() != MIPC_RESULT_SUCCESS) {
        errNum = RIL_E_GENERIC_FAILURE;
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), errNum,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
    return;
error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), errNum,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmmNwRequestHandler::updateFemtoCellInfo(const sp<RfxMclMessage>& msg) {
    int type, isfemtoCell = 0;
    unsigned long i;
    char *responseStr[10] = { 0 };
    int *pInt = (int *)msg->getData()->getData();
    type = pInt[0];

    pthread_mutex_lock(&sFemtocellMutex[m_slot_id]);
    if (type == 2 || type == 3) {
        int reNotify = 0;
        if (sFemtocellCache[m_slot_id]->is_lte_femtocell) {
                isfemtoCell = 2;
        }
        if (sFemtocellCache[m_slot_id]->is_femtocell != isfemtoCell) {
            sFemtocellCache[m_slot_id]->is_femtocell = isfemtoCell;
            reNotify = 1;
        }
        if (reNotify == 0) {
            logD(LOG_TAG, "no need to notify for type = %d", type);
            pthread_mutex_unlock(&sFemtocellMutex[m_slot_id]);
            return;
        }
    }

    asprintf(&responseStr[0], "%d", sFemtocellCache[m_slot_id]->domain);
    asprintf(&responseStr[1], "%d", sFemtocellCache[m_slot_id]->state);
    asprintf(&responseStr[2], "%s", sFemtocellCache[m_slot_id]->optr_name);
    asprintf(&responseStr[3], "%d", sFemtocellCache[m_slot_id]->plmn_id);
    asprintf(&responseStr[4], "%d", sFemtocellCache[m_slot_id]->act);

    if (sFemtocellCache[m_slot_id]->is_femtocell == 2) { // for LTE/C2K femtocell
        asprintf(&responseStr[5], "%d", sFemtocellCache[m_slot_id]->is_femtocell);
    } else if (sFemtocellCache[m_slot_id]->is_csg_cell == 1) { // for GSM femtocell
        asprintf(&responseStr[5], "%d", sFemtocellCache[m_slot_id]->is_csg_cell);
    } else {
        asprintf(&responseStr[5], "0");
    }
    asprintf(&responseStr[6], "%d", sFemtocellCache[m_slot_id]->csg_id);
    asprintf(&responseStr[7], "%d", sFemtocellCache[m_slot_id]->csg_icon_type);
    asprintf(&responseStr[8], "%s", sFemtocellCache[m_slot_id]->hnbName);
    asprintf(&responseStr[9], "%d", sFemtocellCache[m_slot_id]->cause);
    pthread_mutex_unlock(&sFemtocellMutex[m_slot_id]);

    // <domain>,<state>,<lognname>,<plmn_id>,<act>,<is_csg_cell/is_femto_cell>,<csg_id>,<csg_icon_type>,<hnb_name>,<cause> */
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_FEMTOCELL_INFO,
            m_slot_id, RfxStringsData(responseStr, 10));
    responseToTelCore(urc);
    // free memory
    for (i=0 ; i<(sizeof(responseStr)/sizeof(char*)) ; i++) {
        if (responseStr[i]) {
            // logD(LOG_TAG, "free responseStr[%d]=%s", i, responseStr[i]);
            free(responseStr[i]);
            responseStr[i] = NULL;
        }
    }
}


void RmmNwRequestHandler::requestGetTs25Name(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *) msg->getData()->getData();
    int mcc = pInt[0];
    int mnc = pInt[1];
    char *mccmnc = NULL;
    char longname[MAX_OPER_NAME_LENGTH] = { 0 };
    if (mnc > 99) asprintf(&mccmnc, "%d%03d", mcc, mnc);
    else asprintf(&mccmnc, "%d%02d", mcc, mnc);

    matchPlmnName(longname, NULL, mccmnc, -1, 0xFF, MATCH_TS25);
    free(mccmnc);
    logD(LOG_TAG, "%s mcc:%d mnc:%d,long:%s", __FUNCTION__, mcc, mnc, longname);
    // response long name only.
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringData(longname, (strlen(longname)+1)), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestSetNrOption(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int old5gopt = 0, new5gopt = 0, oldNsa = 0, newNsa = 0;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    new5gopt = pInt[0];

    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_NR_REQ, msg->getSlotId());
    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        ril_errno = RIL_E_MODE_NOT_SUPPORTED;
        goto error;
    }
    old5gopt = mipcRsp->getMipcUint8Val(MIPC_NW_GET_NR_CNF_T_NR_OPT, 0xFF);
    if (old5gopt == 0xFF) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    if (old5gopt != new5gopt) {
        mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_NR_REQ, msg->getSlotId());
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_NR_REQ_T_NR_OPT, new5gopt);
        mipcRsp = callToMipcMsgSync(mipcReq);
        if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
            logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
            ril_errno = RIL_E_MODE_NOT_SUPPORTED;
            goto error;
        }
        // RILD cache nr info of a lte cell
        // we need to refresh the state when user disable/enable nr
        oldNsa = (old5gopt == 5 || old5gopt == 7) ? 1 : 0;
        newNsa = (new5gopt == 5 || new5gopt == 7) ? 1 : 0;
        if (oldNsa != newNsa) {
            // nsa capability changes, need notify.
            sp<RfxMclMessage> urc;
            urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                    m_slot_id, RfxVoidData());
            // response to TeleCore
            responseToTelCore(urc);
        }
    }

error:
    logV(LOG_TAG, "%s oldNsa=%d, newNsa=%d", __FUNCTION__, oldNsa, newNsa);
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

#define NR_OPT_LTE      (0x01)
#define NR_OPT_NSA      (0x04)
#define NR_OPT_SA       (0x02)

// not able include hidl gen header directly for now,
// need keep the value same
typedef enum {
    NRDC_ENABLE = 1,
    NRDC_DISABLE = 2,
    NRDC_DISABLE_IMMEDIATE = 3
} NrDcState;

void RmmNwRequestHandler::requestSetNrdcState(const sp<RfxMclMessage>& msg) {
    int *args = (int *)msg->getData()->getData();
    int argsCount = msg->getData()->getDataLength() / sizeof(int);
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    int nrOption, endcDeactivate;
    bool nsaChanged = false;

    if (args == NULL || argsCount != 1) {
        radioError = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    nrOption = getNrOption(msg->getSlotId());
    if (nrOption < 0) goto error;

    endcDeactivate = isEndcDeactivate(msg->getSlotId());
    if (endcDeactivate < 0) goto error;

    switch (*args) {

        case NRDC_ENABLE:
            if ((nrOption & NR_OPT_NSA) && !endcDeactivate)
                break;

            if (endcDeactivate) {
                setEndcDeactivate(false, msg->getSlotId());
            }
            if (!(nrOption & NR_OPT_NSA)) {
                nrOption |= NR_OPT_NSA;
                nsaChanged = true;
                setNrOption(nrOption, msg->getSlotId());
            }
            break;

        case NRDC_DISABLE:
            if (nrOption & NR_OPT_NSA) {
                nrOption &= ~NR_OPT_NSA;
                nsaChanged = true;
                setNrOption(nrOption, msg->getSlotId());
            }
            break;

        case NRDC_DISABLE_IMMEDIATE:
            if (!(nrOption & NR_OPT_NSA) && endcDeactivate)
                break;

            if (nrOption & NR_OPT_NSA) {
                nrOption &= ~NR_OPT_NSA;
                nsaChanged = true;
                setNrOption(nrOption, msg->getSlotId());
            }
            if (!endcDeactivate) {
                setEndcDeactivate(true, msg->getSlotId());
            }

            break;

        default:
            radioError = RIL_E_INVALID_ARGUMENTS;
            goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);

    if (nsaChanged) {
        // nsa capability changes, need notify.
        sp<RfxMclMessage> urc;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(urc);
    }
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(),
        radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetNrdcState(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    int nrOption = 0, endcDeactivate = 0, enabled;

    nrOption = getNrOption(msg->getSlotId());
    if (nrOption < 0) goto error;

    endcDeactivate = isEndcDeactivate(msg->getSlotId());
    if (endcDeactivate < 0) goto error;

    enabled = ((nrOption & NR_OPT_NSA) && !endcDeactivate) ? 1 : 0;

    response = RfxMclMessage::obtainResponse(msg->getId(),
        RIL_E_SUCCESS, RfxIntsData(&enabled, 1), msg, false);
    responseToTelCore(response);

    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(),
        radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

// todo: remove after md support en3epcreg
void RmmNwRequestHandler::handleWfcStateChanged(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int wfcState = (pInt != NULL) ? pInt[0] : 0;
    logV(LOG_TAG, "[%s] wfcState:%d", __FUNCTION__, wfcState);
    RIL_PS_REG_STATE_CACHE* pCache = sPsRegStateCache[m_slot_id];
    int preRegState = convertRegState(pCache->registration_state, false);
    int preRat = convertPSNetworkType(pCache->radio_technology);
    int newRegState = (wfcState == 1) ? 1 : preRegState;
    int newRat = (wfcState == 1) ? 18 : preRat;
    if ((preRegState != newRegState || preRat != newRat)
        && RfxNwServiceState::isInService(newRegState)) {
        logV(LOG_TAG, "[%s], need requestSignalStrength, "
             "preRegState=%d, preRat=%d, newRegState=%d, newRat=%d",
             __FUNCTION__, preRegState, preRat, newRegState, newRat);
        requestSignalStrength(NULL, false);
    }

    pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
    int response[3];
    response[0] = newRegState;
    // todo: need query dymically, since handlePsNetworkStateEvent may be before requestOperator
    response[1] = atoi(sOpInfoCache[m_slot_id]->mccMnc);
    response[2] = newRat;
    pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED,
                                                     m_slot_id, RfxIntsData(response, 3));
    responseToTelCore(urc);
    return;
}

void RmmNwRequestHandler::triggerPollNetworkState() {
    logD(LOG_TAG, "%s", __FUNCTION__);

    requestSignalStrength(NULL, true);

    // update voice/data/Operator
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(urc);
}

void RmmNwRequestHandler::onListenAtFromOemHook(const sp<RfxMclMessage>& msg) {
    char *oem_hook = NULL;
    if (msg != NULL && msg->getData() != NULL) {
        oem_hook = (char*)(msg->getData()->getData());
        logV(LOG_TAG, "%s %s", __FUNCTION__, oem_hook);
        if (strstr(oem_hook, "UPDATE_DISPLAY_RULE")) {
            int set_nr_display_rule = -1;
            int set_jpmode_timer1_delay_timer = -1;
            int set_jpmode_timer2_delay_timer = -1;
            char* comma = oem_hook;
            char* oem_comma = strsep(&comma, ",");
            bool is_rule_by_slot = false;
            //"UPDATE_DISPLAY_RULE"  just for some oem use
            //"UPDATE_DISPLAY_RULE_BY_SLOT" for common
            if (oem_comma != NULL) {
                is_rule_by_slot = !strcmp(oem_comma, "UPDATE_DISPLAY_RULE_BY_SLOT");
            }
            logD(LOG_TAG, "UPDATE_DISPLAY_RULE is_rule_by_slot %d", is_rule_by_slot);

            char* rule = NULL;
            if (comma != NULL) {
                rule = strsep(&comma, ",");
            }
            if (rule != NULL) {
                set_nr_display_rule = atoi(rule);
                if ((!(set_nr_display_rule >= 0 && set_nr_display_rule <= NR_DISPLAY_JP_MODE))
                    || (set_nr_display_rule == 0 && strcmp(rule, "0")) ) {
                    logE(LOG_TAG, "UPDATE_DISPLAY_RULE invalid value %d", set_nr_display_rule);
                    return;
                }
            } else {
                logE(LOG_TAG, "UPDATE_DISPLAY_RULE can not find display rule");
                return;
            }

            // skip lte connection timer
            if (comma != NULL) {
            } else {
                goto error;
            }

            if (comma != NULL) {
                strsep(&comma, ",");
            }

            // skip scg connection timer
            if (comma != NULL) {
            } else {
                goto error;
            }

            if (comma != NULL) {
                strsep(&comma, ",");
            }
            if (comma != NULL) {
                set_jpmode_timer1_delay_timer = atoi(comma);
            } else {
                goto error;
            }

            if (comma != NULL) {
                strsep(&comma, ",");
            }
            if (comma != NULL) {
                set_jpmode_timer2_delay_timer = atoi(comma);
            } else {
                goto error;
            }

            updateNrDisplayRule(is_rule_by_slot,
                    set_nr_display_rule,
                    set_jpmode_timer1_delay_timer,
                    set_jpmode_timer2_delay_timer);
            return;

            error:
                logE(LOG_TAG, "%s UPDATE_DISPLAY_RULE There is something wrong with the request=%d",
                     __FUNCTION__, set_nr_display_rule);
                if ((set_nr_display_rule == 0)
                    || (set_nr_display_rule == NR_DISPALY_NR_CACHE)
                    || ((nr_display_rule[m_slot_id] & NR_DISPLAY_JP_MODE) == NR_DISPLAY_JP_MODE)) {
                    updateNrDisplayRule(is_rule_by_slot, set_nr_display_rule,
                            set_jpmode_timer1_delay_timer,
                            set_jpmode_timer2_delay_timer);
                }
                return;
        }
    }
}


void RmmNwRequestHandler::updateNrDisplayRule(
        bool isRuleBySlot, int setRule, int setJpTimer1, int setJpTimer2) {
    logV(LOG_TAG, "[%s] to set value: %d, %d, %d, current value:%d, %d, %d, bySlot %d",
             __FUNCTION__, setRule, setJpTimer1, setJpTimer2,
            nr_display_rule[m_slot_id],
            jp_mode_timer1_delay_timer[m_slot_id],
            jp_mode_timer2_delay_timer[m_slot_id],
            isRuleBySlot);

    bool isRuleChange = true;
    if (nr_display_rule[m_slot_id] == setRule) {
        if (NR_DISPLAY_JP_MODE == setRule &&
                (setJpTimer1 != jp_mode_timer1_delay_timer[m_slot_id]
                        || setJpTimer2 != jp_mode_timer2_delay_timer[m_slot_id])) {
            //if set JP mode again but timer is not same as the old one,  should also change them.
            isRuleChange = true;
        } else {
            isRuleChange = false;
        }
    } else {
        String8 nrDisplayRule("persist.vendor.radio.nr_display_rule");
        nrDisplayRule.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
        rfx_property_set(nrDisplayRule.string(), String8::format("%d", setRule).string());
        nr_display_rule[m_slot_id] = setRule;
    }

    if ((jp_mode_timer1_delay_timer[m_slot_id] != setJpTimer1)
            && setJpTimer1 >= 0 && setJpTimer1 <= 60) {
        String8 jpTime1("persist.vendor.radio.jp_mode_timer1_delay_timer");
        jpTime1.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
        rfx_property_set(jpTime1.string(), String8::format("%d", setJpTimer1).string());
        jp_mode_timer1_delay_timer[m_slot_id] = setJpTimer1;
    }

    if ((jp_mode_timer2_delay_timer[m_slot_id] != setJpTimer2)
            && setJpTimer2 >= 0 && setJpTimer2 <= 60) {
        String8 jpTime2("persist.vendor.radio.jp_mode_timer2_delay_timer");
        jpTime2.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
        rfx_property_set(jpTime2.string(), String8::format("%d", setJpTimer2).string());
        jp_mode_timer2_delay_timer[m_slot_id] = setJpTimer2;
    }

    logV(LOG_TAG, "updateNrDisplayRule set value done,%d, %d, %d, isRuleChange:%d",
            nr_display_rule[m_slot_id],
            jp_mode_timer1_delay_timer[m_slot_id],
            jp_mode_timer2_delay_timer[m_slot_id],
            isRuleChange);

    if (isRuleChange) {
        setNrDisplayRuleToMd();
        sp<RfxMclMessage> urc;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(urc);
    }

    if (isRuleBySlot) {
        logV(LOG_TAG, "updateNrDisplayRule BY SLOT set rule, all done now return");
        return;
    }

    //set 5g icon rule to other slot, to compatibility with the scheme for some oem
    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        if (slotId == m_slot_id) {
            continue;
        }
        logV(LOG_TAG, "updateNrDisplayRule, sendEvent to slotId %d for sync 5g icon rule", slotId);
        int data[3] = {nr_display_rule[m_slot_id],
            jp_mode_timer1_delay_timer[m_slot_id],
            jp_mode_timer2_delay_timer[m_slot_id]};
        sendEvent(RFX_MSG_EVENT_5G_ICON_SHOW_RULE_SYNC_EVENT, RfxIntsData(data, 3), RIL_CMD_PROXY_3,
            slotId, -1, -1, 0, MTK_RIL_REQUEST_PRIORITY_MEDIUM);
    }
    return;
}

//To sync 5g icon rule to this slot
void RmmNwRequestHandler::onSync5GIconShowRuleWithOtherSlot(const sp<RfxMclMessage>& msg){
    logV(LOG_TAG, "%s", __FUNCTION__);
    int *pInt = (int *)msg->getData()->getData();
    int set_nr_display_rule = pInt[0];
    int set_jpmode_timer1_delay_timer = pInt[1];
    int set_jpmode_timer2_delay_timer = pInt[2];
    logD(LOG_TAG, "[%s] set_nr_display_rule:%d", __FUNCTION__, set_nr_display_rule);

    updateNrDisplayRule(true, set_nr_display_rule,
            set_jpmode_timer1_delay_timer,
            set_jpmode_timer2_delay_timer);
    return;
}

void RmmNwRequestHandler::requestGetTOEInfo(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    char *resp[3];
    sp<RfxMclMessage> response;
    char longname[MAX_OPER_NAME_LENGTH], shortname[MAX_OPER_NAME_LENGTH], operCode[8];
    char emptyString[MAX_OPER_NAME_LENGTH] = "";

    rfx_property_get(String8::format("%s_%d", PROPERTY_NITZ_OPER_CODE, m_slot_id).string(), operCode, "");
    rfx_property_get(String8::format("%s_%d", PROPERTY_NITZ_OPER_LNAME, m_slot_id).string(), longname, "");
    rfx_property_get(String8::format("%s_%d", PROPERTY_NITZ_OPER_SNAME, m_slot_id).string(), shortname, "");

    resp[0] = (strlen(longname) == 0) ? emptyString : longname;
    resp[1] = (strlen(shortname) == 0) ? emptyString : shortname;
    resp[2] = (strlen(operCode) == 0) ? emptyString : operCode;

    logD(LOG_TAG, "requestGetTOEInfo %d: %s %s %s", m_slot_id+1, resp[0], resp[1], resp[2]);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                       RfxStringsData(resp, 3), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;
}

void RmmNwRequestHandler::requestSetCarrierAggregationMode(const sp<RfxMclMessage>& msg) {
    /*AT+ECASW=<mode>*/
    sp<RfxMclMessage> resp;
    RIL_Errno ril_errno = RIL_E_INVALID_ARGUMENTS;
    sp<RfxMipcData> reqData = NULL, cnfData = NULL;

    int *pInt = (int*)msg->getData()->getData();

    // mode = 0, turn off CA
    // mode = 1, turn on CA
    int mode = pInt[0];

    // option = 0, LTE CA
    // option = 1, SA CA
    // option = 2, NSA CA
    int option = pInt[1];

    // linkType = 0, ca link type download
    // linkType = 1, ca link type upload
    // linkType = 2, ca link type both download and upload
    int linkType = pInt[2]; // NOT used now

    logD(LOG_TAG, "requestSetCarrierAggregationMode mode = %d, option = %d, linkType = %d",
        mode, option, linkType);
    if (option == 0) {
        reqData = RfxMipcData::obtainMipcData(MIPC_NW_SET_CA_REQ, m_slot_id);
        reqData->addMipcTlvUint8(MIPC_NW_SET_CA_REQ_T_RAT, MIPC_NW_SET_CA_RAT_LTE);
        reqData->addMipcTlvUint8(MIPC_NW_SET_CA_REQ_T_MODE,
            (mode == 0) ? MIPC_NW_CA_MODE_TURN_OFF : MIPC_NW_CA_MODE_TURN_ON);
        cnfData = callToMipcMsgSync(reqData);
    } else if (option == 1) {
        reqData = RfxMipcData::obtainMipcData(
                MIPC_SYS_SET_OR_GET_SBP_INFO_REQ, m_slot_id);
        reqData->addMipcTlvUint8(
                MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_MODE,
                MIPC_SYS_SBP_MODE_SET_CONFIG_WITH_FEATURE_STR);
        reqData->addMipcTlvStr(
                MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_FEATURE_STR, String8::format("SBP_NR_CA"));
        reqData->addMipcTlvUint8(
                MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_DATA, (mode == 0) ? 0 : 1);
        cnfData = callToMipcMsgSync(reqData);
    } else {
        goto error;
    }

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;

error:
    logE(LOG_TAG, "requestSetCarrierAggregationMode return error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestGetCALinkEnableStatus(const sp<RfxMclMessage>& msg) {
    /*AT+ECAULDL=<mode>[,<dir>,<band_combo>]*/
    sp<RfxMclMessage> resp;
    uint8_t mode = MIPC_CA_LINK_ENABLE_MODE_DOWNLINK;
    int status = 0;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxMipcData> reqData = NULL, cnfData = NULL;
    const char **strings = (const char **)msg->getData()->getData();

    // linkType = 0, ca link type download
    // linkType = 1, ca link type upload
    // linkType = 2, ca link type both download and upload
    int linkType = atoi(strings[1]);
    logD(LOG_TAG, "requestGetCALinkEnableStatus bandsCombo = %s, linkType = %d", strings[0], linkType);

    if (linkType != 0 && linkType != 1) {
        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    } else if (linkType == 0) {
        mode = MIPC_CA_LINK_ENABLE_MODE_DOWNLINK;
    } else {
        mode = MIPC_CA_LINK_ENABLE_MODE_UPLINK;
    }

    if (strings[0] == NULL) goto error;

    reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ, m_slot_id);
    reqData->addMipcTlvUint8(MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ_T_MODE, mode);
    reqData->addMipcTlvStr(MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ_T_BAND_COMBO, strings[0]);

    cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    status = (int)cnfData->getMipcUint8Val(MIPC_NW_GET_CA_LINK_ENABLE_STATUS_CNF_T_STATUS, MIPC_BAND_COMBO_ENABLE_STATUS_DISABLE);

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(&status, 1), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;

error:
    logE(LOG_TAG, "requestGetCALinkEnableStatus return error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestSetCALinkEnableStatus(const sp<RfxMclMessage>& msg) {
    /*AT+ECACOMB=<cmd_type>,<list_type>[,<ca_comb_list>]*/
    sp<RfxMclMessage> resp;
    sp<RfxMipcData> reqData = NULL, cnfData = NULL;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    const char **strings = (const char **)msg->getData()->getData();
    int enableStatus = atoi(strings[0]);

    // linkType = 0, ca link type download
    // linkType = 1, ca link type upload
    // linkType = 2, ca link type both download and upload
    int linkType = atoi(strings[2]);
    logD(LOG_TAG, "requestSetCALinkEnableStatus enableStatus = %d, bandsCombo = %s, linkType = %d",
        enableStatus, strings[1], linkType);

    if (linkType != 0 && linkType != 1) {
        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }
    if (strings[1] == NULL) goto error;

    reqData = RfxMipcData::obtainMipcData(MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ, m_slot_id);
    reqData->addMipcTlvUint8(MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_LIST_TYPE,
        enableStatus ? MIPC_CA_COMB_LIST_TYPE_WHITE : MIPC_CA_COMB_LIST_TYPE_BLACK);
    reqData->addMipcTlvStr(MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_CA_COMB_LIST, strings[1]);

    cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;

error:
    logE(LOG_TAG, "requestSetCALinkEnableStatus return error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestGetCALinkCapabilityList(const sp<RfxMclMessage>& msg) {
    /*AT+ECAULDL=<mode>[,<dir>,<band_combo>]*/
    sp<RfxMclMessage> resp;
    int count = 0;
    uint8_t mode = MIPC_CA_LINK_MODE_DOWNLINK;
    sp<RfxMipcData> reqData = NULL, cnfData = NULL;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    char **response = NULL; // response link capability list
    int *pInt = (int*)msg->getData()->getData();

    // linkType = 0, ca link type download
    // linkType = 1, ca link type upload
    // linkType = 2, ca link type both download and upload
    int linkType = pInt[0];
    logD(LOG_TAG, "requestGetCALinkCapabilityList linkType = %d", linkType);

    if (linkType != 0 && linkType != 2) {
        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    } else if (linkType == 0) {
        mode = MIPC_CA_LINK_MODE_DOWNLINK;
    } else {
        mode = MIPC_CA_LINK_MODE_DOWNLIINK_AND_UPLINK;
    }

    reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_CA_LINK_CAPABILITY_REQ, m_slot_id);
    reqData->addMipcTlvUint8(MIPC_NW_GET_CA_LINK_CAPABILITY_REQ_T_MODE, mode);

    cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    count = (int)cnfData->getMipcUint8Val(MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_COUNT, 0);
    logD(LOG_TAG, "requestGetCALinkCapabilityList Count = %d", count);

    if (count > 0) {
        uint16_t len = 0;
        mipc_band_combo_info_struct4* pComboList = NULL;

        pComboList =
            (mipc_band_combo_info_struct4*)cnfData->getMipcVal(MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_BAND_COMBO, &len);

        if (pComboList == NULL || len != count * sizeof(mipc_band_combo_info_struct4)) {
            logE(LOG_TAG, "[%s] failed, pComboList:%p, len:%d, struct len:%d",
                 __FUNCTION__, pComboList, len, (int) (count * sizeof(mipc_band_combo_info_struct4)));
            goto error;
        }

        response = (char **) calloc(1, sizeof(char*) * count); // for string, each one is 15 bytes
        if (response == NULL) {
            logE(LOG_TAG, "requestGetCALinkCapabilityList calloc fail");
            goto error;
        }

        for (int i = 0; i < count; i++) {
            response[i] = pComboList->band_combo;
            pComboList++;
        }
    } else {
        goto error;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringsData(response, count), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    free(response);
    return;

error:
    logE(LOG_TAG, "requestGetCALinkEnableStatus return error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestGetLteData(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    //default state value not registered, others default value INT_MAX
    int lteData[12] = {0};

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_LTE_DATA_REQ, m_slot_id);

    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    lteData[0] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_REG_STATE, 0);
    lteData[1] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_MCC, 0x7FFFFFFF);
    lteData[2] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_MNC, 0x7FFFFFFF);
    lteData[3] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_CELL_ID, 0x7FFFFFFF);
    lteData[4] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_BAND, 0x7FFFFFFF);
    lteData[5] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_BANDWIDTH, 0x7FFFFFFF);
    lteData[6] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_UL_CHANNEL, 0x7FFFFFFF);
    lteData[7] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_DL_CHANNEL, 0x7FFFFFFF);
    lteData[8] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_RSSI, 0x7FFFFFFF);
    lteData[9] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_RSRP, 0x7FFFFFFF);
    lteData[10] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_RSRQ, 0x7FFFFFFF);
    lteData[11] = (int)cnfData->getMipcUint32Val(MIPC_NW_GET_LTE_DATA_CNF_T_TX_POWER, 0x7FFFFFFF);

    logD(LOG_TAG, "requestGetLteData, %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            lteData[0],lteData[1],lteData[2],lteData[3],lteData[4],lteData[5],lteData[6],lteData[7],
            lteData[8],lteData[9],lteData[10],lteData[11]);
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(lteData, 12), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;

error:
    logE(LOG_TAG, "requestGetLteData return error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestGetLteRRCState(const sp<RfxMclMessage>& msg) {
    /*+EENCONST:<mode>, <state>*/
    sp<RfxMclMessage> resp;
    int state, lteRRCState = 0xFF;

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_LTE_RRC_STATE_REQ, m_slot_id);

    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    state = (int)cnfData->getMipcUint8Val(MIPC_NW_GET_LTE_RRC_STATE_CNF_T_STATE, 0xFF);

    logD(LOG_TAG, "requestGetLteRRCState, state = %d", state);

    switch (state) {
        case 1:
            lteRRCState = 0x00;
            break;
        case 2:
            lteRRCState = 0x01;
            break;
        case 3:
            lteRRCState = 0x02;
            break;
        case 4:
            lteRRCState = 0x03;
            break;
        default:
            break;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(&lteRRCState, 1), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;

error:
    logE(LOG_TAG, "requestGetLteRRCState return error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestSetLteBandEnableStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    const char **strings = (const char **)msg->getData()->getData();
    int setLteBandId = atoi(strings[0]);
    int enable = atoi(strings[1]);
    uint16_t len = 0;
    mipc_sys_lte_band_struct4* pLteBandClass = NULL;

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_BAND_MODE_REQ, m_slot_id);
    reqData->addMipcTlvUint8(MIPC_NW_GET_BAND_MODE_REQ_T_OPTION, MIPC_NW_BAND_OPTION_CURRENT_SETTING);

    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    pLteBandClass =
        (mipc_sys_lte_band_struct4*)cnfData->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_LTE_BAND, &len);

    if (pLteBandClass == NULL || len != sizeof(mipc_sys_lte_band_struct4)) {
        logE(LOG_TAG, "[%s] failed, pLteBandClass:%p, len:%d, struct len:%d",
             __FUNCTION__, pLteBandClass, len, (int) (sizeof(mipc_sys_lte_band_struct4)));
        goto error;
    }

    logD(LOG_TAG, "requestSetLteBandEnableStatus, setLteBandId: %d, enable: %d, "
        "lte_band_1_32 : %u, lte_band_33_64: %u, lte_band_65_96 : %u, lte_band_97_126: %u, "
        "lte_band_127_160 : %u, lte_band_161_192: %u, lte_band_193_224 : %u, lte_band_225_256: %u",
         setLteBandId, enable, pLteBandClass->lte_band_class[0], pLteBandClass->lte_band_class[1],
         pLteBandClass->lte_band_class[2], pLteBandClass->lte_band_class[3], pLteBandClass->lte_band_class[4],
         pLteBandClass->lte_band_class[5], pLteBandClass->lte_band_class[6], pLteBandClass->lte_band_class[7]);

    if (setLteBandId >= 1 && setLteBandId <= 256) {
        int index = (setLteBandId - 1) / 32;
        int bit = (setLteBandId - 1) % 32;
        pLteBandClass->lte_band_class[index] =
            enable ? (pLteBandClass->lte_band_class[index] | (1 << bit)) : (pLteBandClass->lte_band_class[index] & ~(1 << bit));
    } else {
        logE(LOG_TAG, "requestSetLteBandEnableStatus, wrong lte band id: %d", setLteBandId);
    }
    reqData = RfxMipcData::obtainMipcData(MIPC_NW_SET_BAND_MODE_REQ, m_slot_id);
    reqData->addMipcTlv(
        MIPC_NW_SET_BAND_MODE_REQ_T_LTE_BAND, (uint16_t)sizeof(mipc_sys_lte_band_struct4), pLteBandClass);

    cnfData = callToMipcMsgSync(reqData);

    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;

error:
    logE(LOG_TAG, "requestSetLteBandEnableStatus error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestGetBandPriorityList(const sp<RfxMclMessage>& msg) {
    /*AT+EGMC=0,"priority_band"*/
    char *tmp = NULL;
    sp<RfxMclMessage> resp;
    int bandPriList[15] = {0};  // maximum band support 15
    int bandListCount = 0;
    int err = 0;
    RfxAtLine *line = NULL;
    char *data = NULL;
    uint16_t dataLen = 0;

    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_GET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_GET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_GET_CONFIG_REQ_T_TYPE, String8::format("priority_band"));

    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        goto error;
    }

    data = (char*)mipcRsp->getMipcVal(MIPC_SYS_GET_CONFIG_CNF_T_DATA, &dataLen);
    if (data == NULL || dataLen == 0 || strlen(data) == 0) {
        logE(LOG_TAG, "+egmc data wrong");
        goto error;
    }

    line = new RfxAtLine(data, NULL);
    if (line == NULL) {
        logE(LOG_TAG, "new RfxAtLine fail");
        goto error;
    }

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // get <command>
    tmp = line->atTokNextstr(&err);
    if (err < 0) goto error;

    while (line->atTokHasmore()) {
        // get <band>
        bandPriList[bandListCount] = line->atTokNextint(&err);
        if (err < 0) goto error;
        bandListCount++;
        if (15 == bandListCount) break;
    }
    if (0 == bandListCount) goto error; //no bandprilist

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(bandPriList, bandListCount), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    delete line;
    return;

error:
    if (line) delete line;
    logE(LOG_TAG, "requestGetBandPriorityList must never return error when radio is on");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestSetBandPriorityList(const sp<RfxMclMessage>& msg) {
    /*AT+EGMC=1,"priority_band",<band1>,<band2>,<band3>,...*/
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    sp<RfxMclMessage> resp;
    String8 data("");
    int *pInt = (int*)msg->getData()->getData();
    int bandLen = msg->getData()->getDataLength() / sizeof(int);
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;

    if (bandLen > 15)
        bandLen = 15; //max support band number is 15

    if (bandLen == 0) {
        goto error;
    } else {
        for (int i = 0; i < bandLen; i++) {
            if (i != 0) data.append(String8::format("%s", ","));
            data.append(String8::format("%d", pInt[i]));
        }
    }
    logD(LOG_TAG, "requestSetBandPriorityList %s", data.string());

    mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, String8::format("priority_band"));
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA, data);

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }

error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmmNwRequestHandler::requestSet4x4MimoEnabled(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int *pInt = (int *)msg->getData()->getData();
    int enabled_bitmask = pInt[0];
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;
    sp<RfxMclMessage> response = NULL;

    logD(LOG_TAG, "requestSet4x4MimoEnabled, bitmask = %d", enabled_bitmask);

    int rx_mimo_enable = (enabled_bitmask & 0x20) == 0x20 ? 1 : 0;
    int mimo_4x4_enable = (enabled_bitmask & 0x10) == 0x10 ? 1 : 0;
    int mimo_4x4_cca_enable = (enabled_bitmask & 0x08) == 0x08 ? 1 : 0;
    int ras_4rx_2rx_enable = (enabled_bitmask & 0x04) == 0x04 ? 1 : 0;
    int ras_4rx_2rx_cca_enable = (enabled_bitmask & 0x02) == 0x02 ? 1 : 0;
    int ras_2rx_1rx_enable = (enabled_bitmask & 0x01) == 0x01 ? 1 : 0;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, String8::format("rx_mimo_set"));
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA, String8::format("%d,%d,%d,%d,%d,%d",
        rx_mimo_enable, mimo_4x4_enable, mimo_4x4_cca_enable, ras_4rx_2rx_enable, ras_4rx_2rx_cca_enable, ras_2rx_1rx_enable));

    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        ril_errno = RIL_E_GENERIC_FAILURE;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                       RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGet4x4MimoEnabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;
    sp<RfxMclMessage> response = NULL;
    int enabled_bitmask = 0;
    int err = 0, rx_mimo_enable = 0, mimo_4x4_enable = 0, mimo_4x4_cca_enable = 0;
    int ras_4rx_2rx_enable = 0, ras_4rx_2rx_cca_enable = 0, ras_2rx_1rx_enable = 0;
    uint16_t dataLen = 0;
    char *tmp = NULL;
    char *data = NULL;
    RfxAtLine *line = NULL;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_GET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_GET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_GET_CONFIG_REQ_T_TYPE, String8::format("rx_mimo_set"));

    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "[%s] mipc result error", __FUNCTION__);
        goto error;
    }
    data = (char*)mipcRsp->getMipcVal(MIPC_SYS_GET_CONFIG_CNF_T_DATA, &dataLen);
    if (data == NULL || dataLen == 0 || strlen(data) == 0) {
        logE(LOG_TAG, "[%s] data error", __FUNCTION__);
        goto error;
    }
    line = new RfxAtLine(data, NULL);
    if (line == NULL) goto error;

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // get <command>
    tmp = line->atTokNextstr(&err);
    if (err < 0) goto error;

    rx_mimo_enable = line->atTokNextint(&err);
    if (err < 0) goto error;
    mimo_4x4_enable = line->atTokNextint(&err);
    if (err < 0) goto error;
    mimo_4x4_cca_enable = line->atTokNextint(&err);
    if (err < 0) goto error;
    ras_4rx_2rx_enable = line->atTokNextint(&err);
    if (err < 0) goto error;
    ras_4rx_2rx_cca_enable = line->atTokNextint(&err);
    if (err < 0) goto error;
    ras_2rx_1rx_enable = line->atTokNextint(&err);
    if (err < 0) goto error;

    enabled_bitmask = (rx_mimo_enable << 5) | (mimo_4x4_enable << 4) | (mimo_4x4_cca_enable << 3) |
        (ras_4rx_2rx_enable << 2) | (ras_4rx_2rx_cca_enable << 1) | ras_2rx_1rx_enable;

    logD(LOG_TAG, "[%s], bitmask = %d", __FUNCTION__, enabled_bitmask);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                       RfxIntsData(&enabled_bitmask, 1), msg, false);
    responseToTelCore(response);
    delete line;
    return;
error:
    if (line) delete line;
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                       RfxIntsData(&enabled_bitmask, 1), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}


void RmmNwRequestHandler::requestGetLteBsrTimer(const sp<RfxMclMessage>& msg) {
    int timer = 0;
    char *str;
    sp<RfxMclMessage> response;

    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_OMADM_CONF_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_OMADM_CONF_REQ_T_TYPE, MIPC_OMADM_NODE_TYPE_MPSR_DURATION);
    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }
    str = mipcRsp->getMipcStr(MIPC_NW_GET_OMADM_CONF_CNF_T_NODE_VALUE);
    if (strlen(str) > 0) {
        timer = atoi(str);
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(&timer, 1), msg, false);
    logD(LOG_TAG, "requestGetLteBsrTimer: timer = %d", timer);
    responseToTelCore(response);
    return;
error:
    logE(LOG_TAG, "There is something wrong in %s", __FUNCTION__);
     response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
     responseToTelCore(response);
    return;
}

void RmmNwRequestHandler::requestSetLteBsrTimer(const sp<RfxMclMessage>& msg) {
    // AT+EGDM=1,1,<timer_value_in_sec>
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;
    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "requestSetLteBsrTimer: dataLen(%d) is wrong", dataLen);
        goto error;
    }
    logD(LOG_TAG, "requestSetLteBsrTimer: timer = %d", pInt[0]);
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_OMADM_CONF_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_OMADM_CONF_REQ_T_TYPE, MIPC_OMADM_NODE_TYPE_MPSR_DURATION);
    mipcReq->addMipcTlvStr(MIPC_NW_SET_OMADM_CONF_REQ_T_NODE_VALUE, String8::format("%d", pInt[0]));
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
            goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;
error:
    logE(LOG_TAG, "There is something wrong in requestSetLteBsrTimer with AT+EGDM");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetLte1xRttCellList(const sp<RfxMclMessage>& msg) {
    //  AT+ERTTCL=0
    // +ERTTCL:<number_of_lists>[, list of (<sid>,<nid>,<bsid>),...]
    int i, num, *data = NULL, respsenLen = 0, *pInt;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;
    uint8_t type = MIPC_CELL_LIST_TYPE_AVAILABLE;
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "requestGetLte1xRttCellList: dataLen(%d) is wrong", dataLen);
        goto error;
    }
    pInt = (int *)msg->getData()->getData();
    if (pInt[0] == 0) {
        type = MIPC_CELL_LIST_TYPE_UNAVAILABLE;
    }
    logD(LOG_TAG, "requestGetLte1xRttCellList, available = %d",  pInt[0]);
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_LTE_1XRTT_CELL_LIST_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_LTE_1XRTT_CELL_LIST_REQ_T_TYPE, type);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }
    num = mipcRsp->getMipcInt8Val(MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_COUNT, 0);
    if (num > 0) {
        uint16_t len = 0;
        void *resData = mipcRsp->getMipcVal(MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_CELL_LIST, &len);
        int size = sizeof(mipc_1xrtt_cell_info_struct4);
        if (resData == NULL || len == 0 || len / size != num) {
            logE(LOG_TAG, "%s: wrong  data, data= %p, len = %d, size = %d",
                    __FUNCTION__, resData, len, size);
            goto error;
        }
        respsenLen = 3 * num * sizeof(int);
        data = (int *) alloca(respsenLen);
        memset(data, 0, respsenLen);
        mipc_1xrtt_cell_info_struct4 * p = (mipc_1xrtt_cell_info_struct4 *) resData;
        for (i = 0; i < num; i++) {
            data[i * 3] = p->sid;
            data[i * 3 + 1] = p->nid;
            data[i * 3 + 2] = p->bsid;
            p ++;
        }
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(data, respsenLen / sizeof(int)), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with AT+EPBSE");
     response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
     responseToTelCore(response);
    return;
}

void RmmNwRequestHandler::requestClearLteAvailableFile(const sp<RfxMclMessage>& msg) {
    // AT+EGCMD=6,255,"GMSS_clear_lte_unavail_db"
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;

    logD(LOG_TAG, "requestClearLteAvailableFile");
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_CLEAR_LTE_AVAILABLE_FILE_REQ, msg->getSlotId());
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
            goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;
error:
    logE(LOG_TAG, "There is something wrong with AT+EGCMD");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetBandMode(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+EPBSE?
     *   +EPBSE: <gsm_band>,<umts_band><lte_band_1_32>,<lte_band_33_64>
     */
    int gsmBand, umtsBand, band[4];
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;
    uint16_t len =0;
    mipc_sys_lte_band_struct4* pLteBand;
//    mipc_sys_nr_band_struct4* pNrBand;
    void* p;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_BAND_MODE_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_BAND_MODE_REQ_T_OPTION, MIPC_NW_BAND_OPTION_CURRENT_SETTING);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
            goto error;
    }

    // gsm_band
    gsmBand = mipcRsp->getMipcUint32Val(MIPC_NW_GET_BAND_MODE_CNF_T_GSM_BAND, 0);
    band[0] = gsmBand;

    // umts_band
    umtsBand = mipcRsp->getMipcUint32Val(MIPC_NW_GET_BAND_MODE_CNF_T_UMTS_BAND, 0);
    band[1] = umtsBand;
    p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_LTE_BAND, &len);
    pLteBand = (mipc_sys_lte_band_struct4*)p;
    band[2] = pLteBand->lte_band_class[0];
    band[3] = pLteBand->lte_band_class[1];
/*
    p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_NR_BAND, &len);
    pNrBand = (mipc_sys_nr_band_struct4*)p;
*/
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(band, 4), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "[%s]There is something wrong", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
    return;

}

void RmmNwRequestHandler::requestGetCaBandMode(const sp<RfxMclMessage>& msg) {
    int primaryId = 0;
    uint16_t len = 0;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;
    void* data = NULL;
    mipc_nw_ca_band_struct4 *p;

    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "requestGetCaBandMode: dataLen(%d) is wrong", dataLen);
        goto error;
    }
    primaryId = pInt[0];
    logD(LOG_TAG, "requestGetCaBandMode: primaryId = %d", primaryId);

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_CA_BAND_MODE_REQ, msg->getSlotId());
    mipcReq->addMipcTlvInt32(MIPC_NW_GET_CA_BAND_MODE_REQ_T_PRIMARY_ID, primaryId);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
            goto error;
    }
    data = mipcRsp->getMipcVal(MIPC_NW_GET_CA_BAND_MODE_CNF_T_BAND, &len);
    if (data == NULL || len !=  sizeof(mipc_nw_ca_band_struct4)) {
        logE(LOG_TAG, "%s: wrong  data, data= %p, len = %d", __FUNCTION__, data, len);
        goto error;
    }
    p = (mipc_nw_ca_band_struct4*) data;
    logE(LOG_TAG, "requestGetCaBandMode: [%d, %d, %d, %d, %d, %d, %d, %d]",
            p->ca_band_class[0], p->ca_band_class[1], p->ca_band_class[2], p->ca_band_class[3],
            p->ca_band_class[4], p->ca_band_class[5], p->ca_band_class[6], p->ca_band_class[7]);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData((int*)(p->ca_band_class), 8), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "[%s]There is something wrong", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
    return;
}

void RmmNwRequestHandler::requestSetQamEnabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool ulOrDl, enabled;
    int protocol = 1;
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;

    if (msg->getData()->getDataLength() != 2 * sizeof(int)
        || msg->getData()->getData() == NULL) {
        logE(LOG_TAG, "requestSetQamEnabled invalid parameter");
        goto error;
    }

    ulOrDl = (bool)((int *)msg->getData()->getData())[0];
    enabled = (bool)((int *)msg->getData()->getData())[1];
    logV(LOG_TAG, "requestSetQamEnabled ulOrDl[%d] enabled[%d]", ulOrDl, enabled);
    // Not support DL256QAM
    if (!ulOrDl) {
        radioError = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }

    mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, String8::format("errc_feature_set"));
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA, String8::format("%d,UL64QAM,%d", protocol, enabled));

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        radioError = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetQamEnabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> atResp;
    RfxAtLine *line = NULL;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool ulOrDl;
    char *errc = NULL, *feature = NULL;
    int err = 0, protocol = 1, result[2] = {0, 0};
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;
    char *data = NULL;
    uint16_t dataLen = 0;

    if (msg->getData()->getDataLength() != sizeof(int)
        || msg->getData()->getData() == NULL) {
        logE(LOG_TAG, "requestGetQamEnabled invalid parameter");
        goto error;
    }

    ulOrDl = (bool)((int *)msg->getData()->getData())[0];
    logV(LOG_TAG, "requestGetQamEnabled ulOrDl[%d]", ulOrDl);
    // Not support DL256QAM
    if (!ulOrDl) {
        radioError = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }

    result[0] = ulOrDl;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE, String8::format("errc_feature_get"));
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA, String8::format("%d,UL64QAM", protocol));

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        goto error;
    }

    data = (char*)mipcRsp->getMipcVal(MIPC_SYS_SET_CONFIG_CNF_T_DATA, &dataLen);
    if (data == NULL || dataLen == 0 || strlen(data) == 0) {
        logE(LOG_TAG, "+egmc data wrong");
        goto error;
    }

    logD(LOG_TAG, "%s", data);

    line = new RfxAtLine(data, NULL);
    if (line == NULL) {
        logE(LOG_TAG, "new RfxAtLine fail");
        goto error;
    }

    line->atTokStart(&err);
    if (err < 0) goto error;

    errc = line->atTokNextstr(&err);
    if (err < 0 || strcmp("errc_feature_get", errc) != 0) goto error;

    feature = line->atTokNextstr(&err);
    if (err < 0 || strcmp("UL64QAM", feature) != 0) goto error;

    result[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxIntsData(result, 2), msg, false);
    responseToTelCore(response);
    delete line;
    return;

error:
    if (line) delete line;
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestSetTm9Enabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool fddOrTdd, enabled;
    bool fddEnabled, tddEnabled;
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;

    if (msg->getData()->getDataLength() != 2 * sizeof(int)
        || msg->getData()->getData() == NULL) {
        logE(LOG_TAG, "requestSetTm9Enabled invalid parameter");
        goto error;
    }

    fddOrTdd = (bool)((int *)msg->getData()->getData())[0];
    enabled = (bool)((int *)msg->getData()->getData())[1];
    logV(LOG_TAG, "requestSetTm9Enabled fddOrTdd[%d] enabled[%d]", fddOrTdd, enabled);

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_TM9_ENABLE_STATUS_REQ, msg->getSlotId());

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        goto error;
    }

    fddEnabled = (bool)mipcRsp->getMipcUint8Val(MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_FDD_SETTING, 0);
    tddEnabled = (bool)mipcRsp->getMipcUint8Val(MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_TDD_SETTING, 0);

    if ((fddOrTdd && enabled == fddEnabled)
        || (!fddOrTdd && enabled == tddEnabled)) {
        goto success;
    }

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_TM9_ENABLE_STATUS_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_TM9_ENABLE_STATUS_REQ_T_TYPE, fddOrTdd ? MIPC_TM9_SETTING_TYPE_FDD : MIPC_TM9_SETTING_TYPE_TDD);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_TM9_ENABLE_STATUS_REQ_T_STATUS, enabled ? 1 : 0);

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        goto error;
    }

success:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetTm9Enabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool fddOrTdd;
    int result[2] = {0, 0};
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;

    if (msg->getData()->getDataLength() != sizeof(int)
        || msg->getData()->getData() == NULL) {
        logE(LOG_TAG, "requestGetTm9Enabled invalid parameter");
        goto error;
    }

    fddOrTdd = (bool)((int *)msg->getData()->getData())[0];
    logV(LOG_TAG, "requestGetTm9Enabled fddOrTdd[%d]", fddOrTdd);
    result[0] = fddOrTdd;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_TM9_ENABLE_STATUS_REQ, msg->getSlotId());

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        goto error;
    }

    if (fddOrTdd)
        result[1] = (int)mipcRsp->getMipcUint8Val(MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_FDD_SETTING, 0);
    else
        result[1] = (int)mipcRsp->getMipcUint8Val(MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_TDD_SETTING, 0);

    response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxIntsData(result, 2), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestSetLteScanDuration(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    int duration;
    const int MAX_SCAN_DURATION = 3600;
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;

    if (msg->getData()->getDataLength() != sizeof(int)
        || msg->getData()->getData() == NULL) {
        logE(LOG_TAG, "requestSetLteScanDuration invalid parameter");
        goto error;
    }

    duration = *((int *)msg->getData()->getData());
    logV(LOG_TAG, "requestSetLteScanDuration duration[%d]", duration);
    if (duration <= 0 || duration >= MAX_SCAN_DURATION) {
        goto error;
    }

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_OMADM_CONF_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_OMADM_CONF_REQ_T_TYPE, MIPC_OMADM_NODE_TYPE_SCAN_DURATION);
    mipcReq->addMipcTlvStr(MIPC_NW_SET_OMADM_CONF_REQ_T_NODE_VALUE, String8::format("%d", duration));

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        goto error;
    }

    response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetLteScanDuration(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    char *duration = NULL;
    int result = 0;
    sp<RfxMipcData> mipcReq = NULL, mipcRsp = NULL;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_OMADM_CONF_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_OMADM_CONF_REQ_T_TYPE, MIPC_OMADM_NODE_TYPE_SCAN_DURATION);

    mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s[%d] error", __FUNCTION__, __LINE__);
        goto error;
    }

    duration = mipcRsp->getMipcStr(MIPC_NW_GET_OMADM_CONF_CNF_T_NODE_VALUE);

    result = atoi(duration);

    response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxIntsData(&result, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);

}

int RmmNwRequestHandler::setNrOption(int option, int slot) {

    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
            MIPC_NW_SET_NR_REQ, slot);
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_NR_REQ_T_NR_OPT, (uint8_t)option);
    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "+E5GOPT[%d] fail", option);
        return -1;
    }
    return 0;
}

int RmmNwRequestHandler::setEndcDeactivate(bool deactivate, int slot) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_SET_CONFIG_REQ, slot);

    mipcReq->addMipcTlvUint32(MIPC_SYS_SET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_TYPE,
            String8::format("endc_deactivation"));
    mipcReq->addMipcTlvStr(MIPC_SYS_SET_CONFIG_REQ_T_DATA,
            String8::format("%d,1", deactivate ? 1:0));

    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "+EGMC[%d] fail", deactivate);
        return -1;
    }

    return 0;
}

int RmmNwRequestHandler::getNrOption(int slot) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_NR_REQ, slot);
    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "+E5GOPT? fail");
        return -1;
    }
    int option = mipcRsp->getMipcUint8Val(MIPC_NW_GET_NR_CNF_T_NR_OPT, 0xFF);
    if (option == 0xFF) {
        logE(LOG_TAG, "+E5GOPT? fail");
        return -1;
    }

    return option;
}

int RmmNwRequestHandler::isEndcDeactivate(int slot) {
    int err = 0;
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_SYS_GET_CONFIG_REQ, slot);

    mipcReq->addMipcTlvUint32(MIPC_SYS_GET_CONFIG_REQ_T_CLASS, MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG);
    mipcReq->addMipcTlvStr(MIPC_SYS_GET_CONFIG_REQ_T_TYPE,
            String8::format("endc_deactivation"));

    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);

    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "+EGMC:endc_deactivation? fail");
        return -1;
    }

    uint16_t dataLen = 0;
    char *data = (char*)mipcRsp->getMipcVal(MIPC_SYS_GET_CONFIG_CNF_T_DATA, &dataLen);
    if (data == NULL || dataLen == 0 || strlen(data) == 0) {
        logE(LOG_TAG, "+EGMC:endc_deactivation? fail");
        return -1;
    }

    RfxAtLine *line = new RfxAtLine(data, NULL);
    if (line == NULL) {
        logE(LOG_TAG, "+EGMC:endc_deactivation? syntax error");
        return -1;
    }
    line->atTokStart(&err);
    if (err < 0) {
        logE(LOG_TAG, "+EGMC:endc_deactivation? tokStart error");
        delete line;
        return -1;
    }

    char *tag = line->atTokNextstr(&err);
    if (err < 0 || tag == NULL) {
        logE(LOG_TAG, "+EGMC:endc_deactivation? tag error");
        delete line;
        return -1;
    }

    // get <deactivate>
    int deactivate = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "+EGMC:endc_deactivation? deactivate error");
        delete line;
        return -1;
    }
    delete line;

    return deactivate;
}

void RmmNwRequestHandler::requestGetAllBandMode(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+EPBSH?
     *   +EPBSEH: <gsm_band>, <umts_band>,<lte_band>,<nr_band>[,<nr_nsa_band>]
     */
    int i, totalCount = 0;
    // gsm->  umts->lteCount-> lte-> saCount -> sa -> nsaCount ->nsa;
    int result[1+1+1+8+1+32+1+32] = {0};
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;
    uint16_t len =0;
    mipc_sys_lte_band_struct4* pLteBand = NULL;
    mipc_sys_nr_band_struct4* pNrBand = NULL;
    void* p;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_BAND_MODE_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_BAND_MODE_REQ_T_OPTION, MIPC_NW_BAND_OPTION_CURRENT_SETTING);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    // gsm
    result[totalCount++] = mipcRsp->getMipcUint32Val(MIPC_NW_GET_BAND_MODE_CNF_T_GSM_BAND, 0);
    logV(LOG_TAG, "gsmBand = %d", result[0]);

    // umts
    result[totalCount++] = mipcRsp->getMipcUint32Val(MIPC_NW_GET_BAND_MODE_CNF_T_UMTS_BAND, 0);

    // lte
    p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_LTE_BAND, &len);
    if ( p == NULL) {
        result[totalCount++] = 0;
    } else {
        pLteBand = (mipc_sys_lte_band_struct4*) p;
        result[totalCount++] = 8;
        for (i =0; i < 8; i++) {
            result[totalCount++] = pLteBand->lte_band_class[i];
        }
    }

    p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_NR_BAND, &len);
    if (p == NULL) {
        result[totalCount++] = 0;
    } else {
        pNrBand = (mipc_sys_nr_band_struct4 *) p;
        result[totalCount++] = 32;
        for (i =0; i < 32; i++) {
            result[totalCount++] = pNrBand->nr_band_class[i];
        }
    }

    p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_NR_NSA_BAND, &len);
    if (p != NULL) {
        pNrBand = (mipc_sys_nr_band_struct4 *) p;
    }

    if (pNrBand == NULL) {
        result[totalCount++] = 0;
    } else {
        // if NSA is null, use SA data
        result[totalCount++] = 32;
        for (i =0; i < 32; i++) {
            result[totalCount++] = pNrBand->nr_band_class[i];
        }
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(result, totalCount), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with AT+EPBSE");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
    return;
}

void RmmNwRequestHandler::requestSetNrBandMode(const sp<RfxMclMessage>& msg) {

    int *pInt = (int *)msg->getData()->getData();
    int index = 0;
    int *saEnabledBands = NULL;
    int *saDisabledBands = NULL;
    int *nsaEnabledBands = NULL;
    int *nsaDisabledBands = NULL;
    sp<RfxMclMessage> response;
    int saBands[32] = {0};
    int nsaBands[32] = {0};
    uint16_t len = 0;
    int saEnableBandsCount = 0, saDisableBandsCount = 0, nsaEnableBandsCount = 0, nsaDisableBandsCount = 0;
    sp<RfxMipcData> mipcReq, mipcRsp;
    mipc_sys_nr_band_struct4* pSaBand = NULL;
    mipc_sys_nr_band_struct4* pNsaBand = NULL;
    mipc_sys_nr_band_struct4 saBandsTlv;
    mipc_sys_nr_band_struct4 nsaBandsTlv;
    void* p;

    saEnableBandsCount = pInt[index++];
    if (saEnableBandsCount > 0) {
        saEnabledBands = (int *)calloc(saEnableBandsCount, sizeof(int));
        if (saEnabledBands == NULL) goto error;
        for (int i = 0; i < saEnableBandsCount; i++) saEnabledBands[i] = pInt[index++];
    }

    saDisableBandsCount = pInt[index++];
    if (saDisableBandsCount > 0) {
        saDisabledBands = (int *)calloc(saDisableBandsCount, sizeof(int));
        if (saDisabledBands == NULL) goto error;
        for (int i = 0; i < saDisableBandsCount; i++) saDisabledBands[i] = pInt[index++];
    }

    nsaEnableBandsCount = pInt[index++];
    if (nsaEnableBandsCount > 0) {
        nsaEnabledBands = (int *)calloc(nsaEnableBandsCount, sizeof(int));
        if (nsaEnabledBands == NULL) goto error;
        for (int i = 0; i < nsaEnableBandsCount; i++) nsaEnabledBands[i] = pInt[index++];
    }

    nsaDisableBandsCount = pInt[index++];
    if (nsaDisableBandsCount > 0) {
        nsaDisabledBands = (int *)calloc(nsaDisableBandsCount, sizeof(int));
        if (nsaDisabledBands == NULL) goto error;
        for (int i = 0; i < nsaDisableBandsCount; i++) nsaDisabledBands[i] = pInt[index++];
    }
    // get current enabled bands
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_BAND_MODE_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_BAND_MODE_REQ_T_OPTION, MIPC_NW_BAND_OPTION_CURRENT_SETTING);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }
    // SA bands
    p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_NR_BAND, &len);
    if (p == NULL) {
        goto error;
    } else {
        pSaBand = (mipc_sys_nr_band_struct4 *) p;
        for (int i = 0; i < 32; i++) {
            saBands[i] = pSaBand->nr_band_class[i];
        }
    }
    // NSA bands
    p = mipcRsp->getMipcVal(MIPC_NW_GET_BAND_MODE_CNF_T_NR_NSA_BAND, &len);
    if (p == NULL) {
        for (int i = 0; i < 32; i++) {
            nsaBands[i] = saBands[i];
        }
    } else {
        pNsaBand = (mipc_sys_nr_band_struct4 *) p;
        for (int i = 0; i < 32; i++) {
            nsaBands[i] = pNsaBand->nr_band_class[i];
        }
    }

    // format configured bands
    for (int i = 0; i < saEnableBandsCount; i++) {
        int saEnableIndex = (saEnabledBands[i] - 1) / 32;
        if (saEnableIndex < 32 && saEnabledBands[i] > 0) {
            saBands[saEnableIndex] = saBands[saEnableIndex] | (1 << ((saEnabledBands[i] - 1) % 32));
        }
    }
    for (int i = 0; i < saDisableBandsCount; i++) {
        int saDisableIndex = (saDisabledBands[i] - 1) / 32;
        if (saDisableIndex < 32 && saDisabledBands[i] > 0) {
            saBands[saDisableIndex] = saBands[saDisableIndex] & ~(1 << ((saDisabledBands[i] - 1) % 32));
        }
    }
    for (int i = 0; i < nsaEnableBandsCount; i++) {
        int nsaEnableIndex = (nsaEnabledBands[i] - 1) / 32;
        if (nsaEnableIndex < 32 && nsaEnabledBands[i] > 0) {
            nsaBands[nsaEnableIndex] = nsaBands[nsaEnableIndex] | (1 << ((nsaEnabledBands[i] - 1) % 32));
        }
    }
    for (int i = 0; i < nsaDisableBandsCount; i++) {
        int nsaDisableIndex = (nsaDisabledBands[i] - 1) / 32;
        if (nsaDisableIndex < 32 && nsaDisabledBands[i] > 0) {
            nsaBands[nsaDisableIndex] = nsaBands[nsaDisableIndex] & ~(1 << ((nsaDisabledBands[i] - 1) % 32));
        }
    }
    // set bands to modem
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_BAND_MODE_REQ, msg->getSlotId());
    for (int i = 0; i < 32; i++) {
        saBandsTlv.nr_band_class[i] = saBands[i];
        nsaBandsTlv.nr_band_class[i] = nsaBands[i];
    }
    mipcReq->addMipcTlv(MIPC_NW_SET_BAND_MODE_REQ_T_NR_BAND,
                        sizeof(saBandsTlv), &saBandsTlv);
    mipcReq->addMipcTlv(MIPC_NW_SET_BAND_MODE_REQ_T_NR_NSA_BAND,
                        sizeof(nsaBandsTlv), &nsaBandsTlv);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "SET_BAND_MODE_REQ got error[%d]", mipcRsp->getResult());
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg, false);
    responseToTelCore(response);
    if (saEnabledBands) free(saEnabledBands);
    if (saDisabledBands) free(saDisabledBands);
    if (nsaEnabledBands) free(nsaEnabledBands);
    if (nsaDisabledBands) free(nsaDisabledBands);
    return;

error:
    logE(LOG_TAG, "There is something wrong with requestSetNrBandMode");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    if (saEnabledBands) free(saEnabledBands);
    if (saDisabledBands) free(saDisabledBands);
    if (nsaEnabledBands) free(nsaEnabledBands);
    if (nsaDisabledBands) free(nsaDisabledBands);
    return;
}

void RmmNwRequestHandler::handleRefreshCellInfoListEvent(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int retryCount = 0;
    if (pInt)
        retryCount = pInt[0];
    logV(LOG_TAG, "[%s], retryCount = %d, mCellInfoListRetryCountLeft = %d",
            __FUNCTION__, retryCount, mCellInfoListRetryCountLeft);
    if (retryCount < mCellInfoListRetryCountLeft) {
        logV(LOG_TAG, "retryCount(%d) < mCellInfoListRetryCountLeft(%d), use the left",
            retryCount, mCellInfoListRetryCountLeft);
            return;
    }
    mCellInfoListRetryCountLeft = retryCount;
    bool result = refreshCellInfoList();
    if (result) {
        mCellInfoListRetryCountLeft = 0;
    } else if(mCellInfoListRetryCountLeft > 0) {
        logV(LOG_TAG, "[%s], failed, retry after 200s", __FUNCTION__);
        mCellInfoListRetryCountLeft --;
        sendEvent(RFX_MSG_EVENT_REFRESH_CELL_INFO_LIST,
                RfxIntsData(&mCellInfoListRetryCountLeft, 1), RIL_CMD_PROXY_3,
                m_slot_id, -1, -1, ms2ns(600), MTK_RIL_REQUEST_PRIORITY_LOW);
    }
}

void RmmNwRequestHandler::requestSetUsageSetting(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+CEUS= <setting>
     *    0: voice centric
     *    1: data centric
     */
    RIL_Errno err = RIL_E_INVALID_ARGUMENTS;
    int setting;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;

    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "requestSetLteBsrTimer: dataLen(%d) is wrong", dataLen);
        goto error;
    }
    setting = pInt[0];

    //transfer to Modem value from RIL value
    if (setting == RIL_USAGE_CS_CENTRIC) {
        setting = 0;
    } else if (setting == RIL_USAGE_PS_CENTRIC) {
        setting = 1;
    } else {
        goto error;
    }

    logD(LOG_TAG, "%s: setting = %d", __FUNCTION__, setting);

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_UE_USAGE_SETTING_REQ, msg->getSlotId());
    if (mipc_nw_set_ue_usage_setting_req_add_usage_setting(mipcReq->getData(), (uint8_t) setting)
                    == MIPC_MSG_API_RESULT_FAIL) {
        err = RIL_E_MODEM_ERR;
        goto error;
    }
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        err = RIL_E_MODEM_ERR;
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;
error:
    logE(LOG_TAG, "There is  error in %s, err = %d", __FUNCTION__, err);
    response = RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetUsageSetting(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+CEUS?
     *   +CEUS: <setting>
     *    0: voice centric
     *    1: data centric
     */
    int err = 0, setting = 0;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_UE_USAGE_SETTING_REQ, msg->getSlotId());
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        err = RIL_E_MODEM_ERR;
        goto error;
    }
    setting = mipc_nw_get_ue_usage_setting_cnf_get_usage_setting(mipcRsp->getData(), 0XFF);

    //transfer to RIL value from Modem value
    if (setting == 0) {
        setting = RIL_USAGE_CS_CENTRIC;
    } else if (setting ==1) {
        setting = RIL_USAGE_PS_CENTRIC;
    } else {
        logE(LOG_TAG, "%s: unknown setting: %d", __FUNCTION__, setting);
        goto error;
    }

    logV(LOG_TAG, "%s: setting = %d", __FUNCTION__, setting);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(&setting, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with AT+CEUS in %s", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
                RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetNitz(const sp<RfxMclMessage>& msg) {
    char* resp[2] = {0};
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq, mipcRsp;
    mipc_nw_nitz_info_struct4* pNitzInfo = NULL;
    uint16_t len = 0;
    uint8_t tz_valid = 0;
    void * ptr = NULL;

    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_NITZ_REQ, msg->getSlotId());
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "requestGetNitz getResult failed!");
        goto error;
    }
    tz_valid = mipcRsp->getMipcUint8Val(MIPC_NW_GET_NITZ_CNF_T_TZ_VALID, 0);
    ptr = mipcRsp->getMipcVal(MIPC_NW_GET_NITZ_CNF_T_INFO, (uint16_t *) &len);
    if (!ptr || len < sizeof(mipc_nw_nitz_info_struct4)) {
        logE(LOG_TAG, "unkown nitz IND, ptr= %p, len = %d", ptr, len);
        goto error;
    } else {
        pNitzInfo = (mipc_nw_nitz_info_struct4*) ptr;
        char *nitzString = NULL;
        asprintf(&nitzString, "%02d/%02d/%02d,%02d:%02d:%02d%+03d,%d",
                pNitzInfo->year % 100,
                pNitzInfo->month,
                pNitzInfo->day,
                pNitzInfo->hour,
                pNitzInfo->minute,
                pNitzInfo->second,
                pNitzInfo->time_zone_offset_minutes / 15,
                pNitzInfo->daylight_saving_offset_minutes / 60);
        logV(LOG_TAG, "NITZ:%s, tzvalid: %s", nitzString, std::to_string(tz_valid).c_str());
        resp[0] = nitzString;
        // Warning: std::to_string(tz_valid).c_str() returns a pointer to a temporary
        // which will be destroyed at the end of the expression, leading to undefined behavior.
        // Instead, we should convert tz_valid to a string and store it in a persistent variable.
        std::string tz_valid_str = std::to_string(tz_valid);
        resp[1] = const_cast<char*>(tz_valid_str.c_str());
        // Note that resp[1] now points to a temporary that will be destroyed when tz_valid_str goes out of scope.
        // You will need to handle this appropriately to avoid undefined behavior.
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxStringsData(resp, 2), msg, false);
        responseToTelCore(response);

        free(nitzString);
        // If tz_valid_str is not used elsewhere, you might need to duplicate the string for resp[1] as well.
        return;
    }
error:
    logE(LOG_TAG, "There is something wrong with getting NITZ string in %s", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
                RfxStringsData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::handleSetSignalStrengthReportingCriteriaRequest(
        const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipcReq, mipcRsp;
    RIL_SignalStrength_Reporting_Criteria * data = (RIL_SignalStrength_Reporting_Criteria*) msg->getData()->getData();
    int size = msg->getData()->getDataLength() / sizeof(RIL_SignalStrength_Reporting_Criteria);
    for (int index = 0; index < size; index ++) {
        RIL_SignalStrength_Reporting_Criteria* ssrc = data + index;
        logV(LOG_TAG, "[%s] signalMeasurement = %d, hysteresisMs = %d, hysteresisDb = %d, "
                "accessNetwork = %d, thresholdsDbm[0] = %d, thresholdsDbm[1] = %d, "
                "thresholdsDbm[2] = %d, thresholdsDbm[3] = %d, isEnabled = %d",
                __FUNCTION__, ssrc->signalMeasurement, ssrc->hysteresisMs, ssrc->hysteresisDb,
                (int) ssrc->accessNetwork, ssrc->thresholdsDbm[0], ssrc->thresholdsDbm[1],
                ssrc->thresholdsDbm[2], ssrc->thresholdsDbm[3], ssrc->isEnabled);

        mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ,
                                                              msg->getSlotId());
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_MODE,
                                 (uint8_t)ssrc->isEnabled);
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_SIGNAL_MEASUREMENT,
                                 (uint8_t)ssrc->signalMeasurement);
        mipcReq->addMipcTlvUint32(MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_HYSTERESIS_MS,
                                  (uint32_t)ssrc->hysteresisMs);
        mipcReq->addMipcTlvUint32(MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_HYSTERESIS_DB,
                                  (uint32_t)ssrc->hysteresisDb);
        /* The rat of whose signal you want to set. 1-GSM, 2-UMTS, 3-LTE, 4-C2K, 5-NR */
        uint8_t rat = 0;
        switch (ssrc->accessNetwork) {
            case GERAN:
                rat = MIPC_NW_SIGNAL_RAT_GSM;
                break;

            case UTRAN:
                rat = MIPC_NW_SIGNAL_RAT_UMTS;
                break;

            case EUTRAN:
                rat = MIPC_NW_SIGNAL_RAT_LTE;
                break;

            case CDMA2000:
                rat = MIPC_NW_SIGNAL_RAT_C2K;
                break;

            case NEW_RADIO:
                rat = MIPC_NW_SIGNAL_RAT_NR;
                break;

            default:
                logE(LOG_TAG, "[%s] unkown accessNetwork: %d", __FUNCTION__, ssrc->accessNetwork);
                continue;
        }
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_RAT, rat);
        mipc_nw_threshold_array_struct4 threshold;
        threshold.num_of_threshold = (uint8_t) ssrc->thresholdsCount;
        for (int i = 0; i < threshold.num_of_threshold; ++i) {
            threshold.threshold_dbm[i] = ssrc->thresholdsDbm[i];
        }
        mipcReq->addMipcTlv(MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_THRESHOLD,
                            sizeof(threshold), &threshold);
        mipcRsp = callToMipcMsgSync(mipcReq);
        if (mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
            logW(LOG_TAG, "[%s][%d], mipc call result failed", __FUNCTION__, mipcRsp->getResult());
            continue;
        }
    }

//error:
    // Older modems don't implement it, however AP shall return success always
    sp<RfxMclMessage> resp =
        RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(resp);
    return;
}

void RmmNwRequestHandler::requestSetN1ModeEnabled(const sp<RfxMclMessage>& msg) {

    RIL_Errno err = RIL_E_INVALID_ARGUMENTS;
    int enabled, nrOption;
    sp<RfxMclMessage> response;

    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "%s: dataLen(%d) is wrong", __FUNCTION__, dataLen);
        goto error;
    }
    enabled = pInt[0];
    if (enabled != 1 && enabled != 0) goto error;
    logD(LOG_TAG, "%s: enabled = %d", __FUNCTION__, enabled);

    nrOption = getNrOption(msg->getSlotId());
    if (nrOption < 0) goto error;
    if (((nrOption & NR_OPT_SA) == NR_OPT_SA) && enabled) goto done;
    if (((nrOption & NR_OPT_SA) == 0) && !enabled) goto done;
    nrOption = (enabled == 1) ? (nrOption | NR_OPT_SA) : (nrOption & (NR_OPT_LTE | NR_OPT_NSA));
    if(setNrOption(nrOption, msg->getSlotId()) < 0) {
        err = RIL_E_GENERIC_FAILURE;
        goto error;
    }
done:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;
error:
    logE(LOG_TAG, "There is  error in %s, err = %d", __FUNCTION__, err);
    response = RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestGetN1ModeEnabled(const sp<RfxMclMessage>& msg) {

    int nrOption, saEnabled;
    sp<RfxMclMessage> response;
    nrOption = getNrOption(msg->getSlotId());
    if (nrOption < 0) goto error;
    saEnabled = ((nrOption & NR_OPT_SA) == NR_OPT_SA) ? 1 : 0;
    logV(LOG_TAG, "%s: nrOption = %d", __FUNCTION__, nrOption);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(&saEnabled, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong in %s", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
                RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestSetNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg) {

    int enabled;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq;
    sp<RfxMipcData> mipcRsp;

    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "%s: dataLen(%d) is wrong", __FUNCTION__, dataLen);
        goto error;
    }
    if (pInt[0] != 0 && pInt[0] != 1) {
        logE(LOG_TAG, "%s: invalid value(%d)", __FUNCTION__, pInt[0]);
        goto error;
    }
    enabled = pInt[0];
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ_T_ALLOW_NULL_ALGO, (uint8_t)enabled);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s fail MIPC error", __FUNCTION__);
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestIsNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg) {

    int enabled;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_REQ, msg->getSlotId());
    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s MIPC REQ error", __FUNCTION__);
        goto error;
    }
    enabled = mipcRsp->getMipcUint8Val(MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_CNF_T_ALLOW_NULL_ALGO, 0xFF);
    if (enabled == 0xFF) {
        logE(LOG_TAG, "%s Modem gives invalid value", __FUNCTION__);
        goto error;
    }
    logV(LOG_TAG, "%s: enabled = %d", __FUNCTION__, enabled);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxIntsData(&enabled, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR, RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestIsCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg) {
    int enabled = 0xff;
    int result = 0xff;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_GET_CONFIG_REQ_T_RETRIEVE_CELLULAR_ID_DISCLOSURE, MIPC_BOOLEAN_TRUE);
    sp<RfxMipcData> mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s MIPC REQ error, Not Support", __FUNCTION__);
        goto error;
    }
    result = mipcRsp->getMipcUint8Val(MIPC_NW_GET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE_RESULT, 0xFF);
    if (result != MIPC_NW_CONFIG_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s cid feature not support", __FUNCTION__);
        goto error;
    }

    enabled = mipcRsp->getMipcUint8Val(MIPC_NW_GET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE, 0xff);
    if (enabled == 0xff) {
        logE(LOG_TAG, "%s Modem gives invalid value", __FUNCTION__);
        goto error;
    }

    logV(LOG_TAG, "%s: enabled = %d", __FUNCTION__, enabled);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxIntsData(&enabled, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR, RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRequestHandler::requestSetCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg) {
    int enabled = 0xff;
    int result = 0xff;
    sp<RfxMclMessage> response;
    sp<RfxMipcData> mipcReq;
    sp<RfxMipcData> mipcRsp;

    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "%s: dataLen(%d) is wrong", __FUNCTION__, dataLen);
        goto error;
    }
    if (pInt[0] != 0 && pInt[0] != 1) {
        logE(LOG_TAG, "%s: invalid value(%d)", __FUNCTION__, pInt[0]);
        goto error;
    }
    enabled = pInt[0];
    mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_CONFIG_REQ, msg->getSlotId());
    mipcReq->addMipcTlvUint8(MIPC_NW_SET_CONFIG_REQ_T_CELLULAR_ID_DISCLOSURE, (uint8_t)enabled);
    mipcRsp = callToMipcMsgSync(mipcReq);
    if (mipcRsp == NULL || mipcRsp->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s fail MIPC error, Not support", __FUNCTION__);
        goto error;
    }
    result = mipcRsp->getMipcUint8Val(MIPC_NW_SET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE_RESULT, 0xFF);
    if (result != MIPC_NW_CONFIG_RESULT_SUCCESS) {
        logE(LOG_TAG, "%s cid set fail", __FUNCTION__);
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR, RfxVoidData(), msg, false);
    responseToTelCore(response);
}
