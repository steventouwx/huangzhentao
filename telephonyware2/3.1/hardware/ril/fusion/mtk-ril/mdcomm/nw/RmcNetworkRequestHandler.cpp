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

#include "RmcNetworkRequestHandler.h"
#include "rfx_properties.h"
#include "ViaBaseHandler.h"
#include "RfxViaUtils.h"
#include <libmtkrilutils.h>
#include "RfxSignalStrengthData.h"

static const int request[] = {
    RFX_MSG_REQUEST_SET_N1_MODE_ENABLED,
    RFX_MSG_REQUEST_IS_N1_MODE_ENABLED,
    RFX_MSG_REQUEST_SET_USAGE_SETTING,
    RFX_MSG_REQUEST_GET_USAGE_SETTING,
    RFX_MSG_REQUEST_SIGNAL_STRENGTH,
    RFX_MSG_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO,
    RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC,
    RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL,
    RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN,
    RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT,
    RFX_MSG_REQUEST_SET_BAND_MODE,
    RFX_MSG_REQUEST_QUERY_AVAILABLE_BAND_MODE,
    RFX_MSG_REQUEST_SET_LOCATION_UPDATES,
    RFX_MSG_REQUEST_GET_CELL_INFO_LIST,
    RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE,
    RFX_MSG_REQUEST_GET_POL_CAPABILITY,
    RFX_MSG_REQUEST_GET_POL_LIST,
    RFX_MSG_REQUEST_SET_POL_ENTRY,
    RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE,
    RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE,
    RFX_MSG_REQUEST_ABORT_FEMTOCELL_LIST,
    RFX_MSG_REQUEST_SELECT_FEMTOCELL,
    RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER,
    RFX_MSG_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE,
    RFX_MSG_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE,
    RFX_MSG_REQUEST_SET_SERVICE_STATE,
    RFX_MSG_REQUEST_SET_PSEUDO_CELL_MODE,
    RFX_MSG_REQUEST_GET_PSEUDO_CELL_INFO,
    RFX_MSG_REQUEST_SET_ROAMING_ENABLE,
    RFX_MSG_REQUEST_GET_ROAMING_ENABLE,
    RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION,
    RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION,
    RFX_MSG_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA,
    RFX_MSG_REQUEST_SET_SYSTEM_SELECTION_CHANNELS,
    RFX_MSG_REQUEST_GET_SYSTEM_SELECTION_CHANNELS,
    RFX_MSG_REQUEST_GET_TS25_NAME,
    RFX_MSG_REQUEST_REGISTER_CELLULAR_QUALITY_REPORT,
    RFX_MSG_REQUEST_GET_SUGGESTED_PLMN_LIST,
    RFX_MSG_REQUEST_SMART_RAT_SWITCH,
    RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH,
    RFX_MSG_REQUEST_CONFIG_A2_OFFSET,
    RFX_MSG_REQUEST_CONFIG_B1_OFFSET,
    RFX_MSG_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION,
    RFX_MSG_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION,
    RFX_MSG_REQUEST_SET_NR_OPTION,
    RFX_MSG_REQUEST_SEARCH_STORED_FREQUENCY_INFO,
    RFX_MSG_REQUEST_SEARCH_RAT,
    RFX_MSG_REQUEST_SET_BACKGROUND_SEARCH_TIMER,
    RFX_MSG_REQUEST_GET_BARRINGINFO,
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
    RFX_MSG_REQUEST_SET_QAM_ENABLED,
    RFX_MSG_REQUEST_GET_QAM_ENABLED,
    RFX_MSG_REQUEST_SET_TM9_ENABLED,
    RFX_MSG_REQUEST_GET_TM9_ENABLED,
    RFX_MSG_REQUEST_SET_LTE_SCAN_DURATION,
    RFX_MSG_REQUEST_GET_LTE_SCAN_DURATION,
    RFX_MSG_REQUEST_SET_NRDC_STATE,
    RFX_MSG_REQUEST_GET_NRDC_STATE,
    RFX_MSG_REQUEST_GET_ALL_BAND_MODE,
    RFX_MSG_REQUEST_SET_NR_BAND_MODE,
    RFX_MSG_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED,
    RFX_MSG_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED,
    RFX_MSG_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
    RFX_MSG_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
    RFX_MSG_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED,
    RFX_MSG_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED
};

static const int events[] = {
    RFX_MSG_EVENT_EXIT_EMERGENCY_CALLBACK_MODE,
    RFX_MSG_EVENT_FEMTOCELL_UPDATE,
    RFX_MSG_EVENT_CONFIRM_RAT_BEGIN,
    RFX_MSG_EVENT_CS_NETWORK_STATE,
    RFX_MSG_EVENT_PS_NETWORK_STATE,
    RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG,
    RFX_MSG_EVENT_RSP_DATA_CONTEXT_IDS,
    RFX_MSG_EVENT_SMART_RAT_SWATCH_BACK,
    RFX_MSG_EVENT_OEM_HOOK_TO_NW,
    RFX_MSG_EVENT_REGISTRATION_FAILED,
    RFX_MSG_EVENT_JP_MODE_TIMER_EVENT,
    RFX_MSG_EVENT_JP_MODE_CONNECTION_EVENT,
    RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM,
    RFX_MSG_EVENT_5G_ICON_SHOW_RULE_SYNC_EVENT
};

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_SIGNAL_STRENGTH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_AVAILABLE_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_LOCATION_UPDATES);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxCellInfoData, RFX_MSG_REQUEST_GET_CELL_INFO_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_POL_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_GET_POL_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_POL_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ABORT_FEMTOCELL_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SELECT_FEMTOCELL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_SERVICE_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_PSEUDO_CELL_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_PSEUDO_CELL_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_ROAMING_ENABLE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_ROAMING_ENABLE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData,
        RFX_MSG_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSsrcData, RfxVoidData,
        RFX_MSG_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSscData, RfxVoidData,
        RFX_MSG_REQUEST_SET_SYSTEM_SELECTION_CHANNELS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxSscData,
        RFX_MSG_REQUEST_GET_SYSTEM_SELECTION_CHANNELS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringData, RFX_MSG_REQUEST_GET_TS25_NAME);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_REGISTER_CELLULAR_QUALITY_REPORT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringsData, RFX_MSG_REQUEST_GET_SUGGESTED_PLMN_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SMART_RAT_SWITCH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH);
// NR request
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CONFIG_A2_OFFSET);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_CONFIG_B1_OFFSET);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_NR_OPTION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SEARCH_STORED_FREQUENCY_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SEARCH_RAT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_BACKGROUND_SEARCH_TIMER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxBarringInfoData, RFX_MSG_REQUEST_GET_BARRINGINFO);
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
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_ALL_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_NR_BAND_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_USAGE_SETTING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_USAGE_SETTING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_N1_MODE_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_N1_MODE_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_FEMTOCELL_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_RSP_DATA_CONTEXT_IDS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SMART_RAT_SWATCH_BACK);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_OEM_HOOK_TO_NW);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_REGISTRATION_FAILED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_JP_MODE_TIMER_EVENT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_JP_MODE_CONNECTION_EVENT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_5G_ICON_SHOW_RULE_SYNC_EVENT);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcNetworkRequestHandler, RIL_CMD_PROXY_3);

RmcNetworkRequestHandler::RmcNetworkRequestHandler(int slot_id, int channel_id) :
        RmcNetworkHandler(slot_id, channel_id),
        m_emergency_only(0){
    int err;
    sp<RfxAtResponse> p_response;
    logV(LOG_TAG, "%s[%d] start", __FUNCTION__, slot_id);
    m_slot_id = (unsigned int) slot_id;
    m_channel_id = channel_id;
    enableCaInfoUrc = getUrcCaInfoConfigureValue();
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    registerToHandleEvent(events, sizeof(events)/sizeof(int));

    pthread_mutex_lock(&s_signalStrengthMutex[m_slot_id]);
    resetSignalStrengthCache(signal_strength_cache[m_slot_id], SIGNAL_GROUP_ALL);
    pthread_mutex_unlock(&s_signalStrengthMutex[m_slot_id]);
    ril_wfc_reg_status[m_slot_id] = 0;

    updatePseudoCellMode();

    /* Enable 93 network registration, location info and cause value unsolicited result */
    atSendCommand("AT+EREG=3");
    /* Enable 93 network registration, location info and GMM cause value unsolicited result */
    /* AT+EGREG=<report_mode>[,<sa_tac_supported>]
     * Tell MD that AP can hanle 24bit tac
     */
    p_response = atSendCommand("AT+EGREG=3,1");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        atSendCommand("AT+EGREG=3");
    }
    /* Enable packet switching data service capability URC */
    atSendCommand("AT+PSBEARER=1");

    /* Enable received signal level indication */
    atSendCommand("AT+ECSQ=1");
    /* Enable getting NITZ, include TZ and Operator Name*/
    /* To Receive +CTZE and +CIEV: 10*/
    atSendCommand("AT+CTZR=3");
    /* Enable CSG network URC */
    if (isFemtocellSupport()) {
        atSendCommand("AT+ECSG=4,1");
    }
    /* Check if modem support ECELL ext3,ext4 */
    p_response = atSendCommand("AT+ECELL=4");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "modem does not support AT+ECELL=4.");
        ECELLext3ext4Support = 0;
    }
    /* Config cell info list extend c2k */
    atSendCommand("AT+ECELL=7,1");
    /* To support return EONS if available in RIL_REQUEST_OPERATOR START */
    atSendCommand("AT+EONS=1");
    /* ALPS00574862 Remove redundant +COPS=3,2;+COPS? multiple cmd in REQUEST_OPERATOR */
    atSendCommand("AT+EOPS=3,2");
    /* check whether calibration data is downloaded or not */
    p_response = atSendCommand("AT+ECAL?");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "bootup get calibration status failed, err=%d", err);
    }
    /*Modulation report*/
    if (isEnableModulationReport()) {
        atSendCommand("AT+EMODCFG=1");
    }
    /* Disable +EMCCMNC URC */
    atSendCommand("AT+EMCCMNC=0");

    /* Enable IMS ECC URC*/
    atSendCommand("AT+EIMSESS=1");

    /* Enable connection state URC */
    atSendCommand("AT+CSCON=3");

    /* Enable Gen97 MCC MNC URC */
    p_response = atSendCommand("AT+EOPS=6,1");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        // For oder chips, even they return error, it still change the format.
        // We need to set it back.
        support_eops_urc = false;
        atSendCommand("AT+EOPS=3,2");
    }

    /* Enable Gen97 CAMP URC */
    atSendCommand("AT+ECAMPUI=1");

    /* Update Endc Capability URC*/
    atSendCommand("AT+ECAPABILITY=1");
    updateEndcCapability();

    /* Check if modem support +EPLWSS */
    /* disable this feature because we prefer AT+EXOPL
    p_response = atSendCommand("AT+EPLWSS=?");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "modem does not support AT+EPLWSS.");
        at_eplwss_support = false;
    }
    */

    char DMF_TC02[] = "DMF_TC02";
    support_dmftc02[m_slot_id] = getFeatureVersion(DMF_TC02, 0);
    if ((enableCaInfoUrc == 1) || ((enableCaInfoUrc == 2) && (support_dmftc02[m_slot_id] == 0))) {
        /* enable ECAINFO URC with full information */
        p_response = atSendCommand("AT+ECAINFO=1");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+ECAINFO=1");
    }

    // if (!s_en3epcreg_support) {
    //     sp<RfxAtResponse> p_response = atSendCommand("AT+EN3EPCREG=?");
    //     if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
    //         s_en3epcreg_support = true;
    //     }
    // }
    // if (s_en3epcreg_support) {
    //     atSendCommand("AT+EN3EPCREG=1");
    // }

    if (!s_enwscn_support) {
        sp<RfxAtResponse> p_response = atSendCommand("AT+ENWSCN=?");
        if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
            s_enwscn_support = true;
        }
    }

    if (isNrSupported()) {
        atSendCommand("AT+ENRCABAND=1");
    }
    if (urc_echinfo_enable) {
        atSendCommand("AT+ECHINFO=1");
    }
    atSendCommand("AT+E5GUW=1"); // enable 5GUW URC
    atSendCommand("AT+E5GUC=1"); // enable 5GUC URC

    if (urc_nrsysinfo_enable && isNrSupported()) {
        atSendCommand("AT+ENRSCS=1");
    }

    mPhyChlCfgMode = getPhyChlCfgMode();
    /* Enable EPHYCONFIG URC */
    if (mPhyChlCfgMode == 2) {
        p_response = atSendCommand("AT+EPHYCONFIG=1");
    }

    atSendCommand("AT+EGMC=1, \"vgmm_smart_idle_local_release\",1");
    atSendCommand("AT+EGMC=1, \"nrrc_data_inactive_timer\",15");
    atSendCommand("AT+EGMC=1, \"errc_nr_threshold\",0,-440,-76,-10,-460,-60,-4");
    atSendCommand("AT+ESBP=5, \"SBP_ERRC_THRESH_BASED_SKIP_NR_CELL_RESEL\",1");
    // TODO: consider boot up screen off test scenario
}

RmcNetworkRequestHandler::~RmcNetworkRequestHandler() {
}

void RmcNetworkRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    // logD(LOG_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(msg->getId()));
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_SIGNAL_STRENGTH:
        case RFX_MSG_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO:
            requestSignalStrength(msg, false);
            break;
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
            requestSetNetworkSelectionAutomatic(msg);
            break;
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL:
            requestSetNetworkSelectionManual(msg);
            break;
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN:
            requestSetNetworkSelectionManualWithRan(msg);
            break;
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT:
            requestSetNetworkSelectionManualWithAct(msg);
            break;
        case RFX_MSG_REQUEST_SET_BAND_MODE:
            requestSetBandMode(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_BAND_MODE:
            requestQueryAvailableBandMode(msg);
            break;
        case RFX_MSG_REQUEST_SET_LOCATION_UPDATES:
            requestSetLocationUpdates(msg);
            break;
        case RFX_MSG_REQUEST_GET_CELL_INFO_LIST:
            requestGetCellInfoList(msg);
            break;
        case RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE:
            requestSetCellInfoListRate(msg);
            break;
        case RFX_MSG_REQUEST_GET_POL_CAPABILITY:
            requestGetPOLCapability(msg);
            break;
        case RFX_MSG_REQUEST_GET_POL_LIST:
            requestGetPOLList(msg);
            break;
        case RFX_MSG_REQUEST_SET_POL_ENTRY:
            requestSetPOLEntry(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE:
            requestSetCdmaRoamingPreference(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE:
            requestQueryCdmaRoamingPreference(msg);
            break;
        case RFX_MSG_REQUEST_ABORT_FEMTOCELL_LIST:
            requestAbortFemtocellList(msg);
            break;
        case RFX_MSG_REQUEST_SELECT_FEMTOCELL:
            requestSelectFemtocell(msg);
            break;
        case RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER:
            requestSetUnsolicitedResponseFilter(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE:
            requestQueryFemtoCellSystemSelectionMode(msg);
            break;
        case RFX_MSG_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE:
            requestSetFemtoCellSystemSelectionMode(msg);
            break;
        case RFX_MSG_REQUEST_SET_SERVICE_STATE:
            requestSetServiceState(msg);
            break;
        case RFX_MSG_REQUEST_SET_PSEUDO_CELL_MODE:
            requestSetPseudoCellMode(msg);
            break;
        case RFX_MSG_REQUEST_GET_PSEUDO_CELL_INFO:
            requestGetPseudoCellInfo(msg);
            break;
        case RFX_MSG_REQUEST_SET_ROAMING_ENABLE:
            setRoamingEnable(msg);
            break;
        case RFX_MSG_REQUEST_GET_ROAMING_ENABLE:
            getRoamingEnable(msg);
            break;
        case RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION:
            requestSetLteReleaseVersion(msg);
            break;
        case RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION:
            requestGetLteReleaseVersion(msg);
            break;
        case RFX_MSG_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA:
            requestSetSignalStrengthReportingCriteria(msg);
            break;
        case RFX_MSG_REQUEST_SET_SYSTEM_SELECTION_CHANNELS:
            requestSetSystemSelectionChannels(msg);
            break;
        case RFX_MSG_REQUEST_GET_SYSTEM_SELECTION_CHANNELS:
            requestGetSystemSelectionChannels(msg);
            break;
        case RFX_MSG_REQUEST_GET_TS25_NAME:
            requestGetTs25Name(msg);
            break;
        case RFX_MSG_REQUEST_REGISTER_CELLULAR_QUALITY_REPORT:
            registerCellularQualityReport(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUGGESTED_PLMN_LIST:
            requestGetSuggestedPlmnList(msg);
            break;
        case RFX_MSG_REQUEST_SMART_RAT_SWITCH:
            requestSmartRatSwitch(msg);
            break;
        case RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH:
            requestGetSmartRatSwitch(msg);
            break;
// NR request
        case RFX_MSG_REQUEST_CONFIG_A2_OFFSET:
            requestConfigA2Offset(msg);
            break;
        case RFX_MSG_REQUEST_CONFIG_B1_OFFSET:
            requestConfigB1Offset(msg);
            break;
        case RFX_MSG_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION:
            requestDeactivateNrScgCommunication(msg);
            break;
        case RFX_MSG_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION:
            requestGetDeactivateNrScgCommunication(msg);
            break;
        case RFX_MSG_REQUEST_SET_NR_OPTION:
            requestSetNrOption(msg);
            break;
        case RFX_MSG_REQUEST_SEARCH_STORED_FREQUENCY_INFO:
            requestSearchStoreFrenquencyInfo(msg);
            break;
        case RFX_MSG_REQUEST_SEARCH_RAT:
            requestSearchRat(msg);
            break;
        case RFX_MSG_REQUEST_SET_BACKGROUND_SEARCH_TIMER:
            requestSetBackgroundSearchTimer(msg);
            break;
        case RFX_MSG_REQUEST_GET_BARRINGINFO:
            requestGetBarringInfo(msg);
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
        case RFX_MSG_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED:
            requestSetSecurityAlgorithmsUpdatedEnabled(msg);
            break;
        case RFX_MSG_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED:
            requestIsSecurityAlgorithmsUpdatedEnabled(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

void RmcNetworkRequestHandler::requestSignalStrength(
        const sp<RfxMclMessage>& msg, bool forceUpdate) {
    int err;
    int sigUpdated = SIGNAL_GROUP_NONE;
    int sigRegisted = SIGNAL_GROUP_NONE;
    sp<RfxMclMessage> response;
    RIL_SIGNAL_STRENGTH resp = {0};
    RIL_SIGNAL_STRENGTH *rspCache = &resp;

    pthread_mutex_lock(&s_signalStrengthMutex[m_slot_id]);

    RIL_RadioState state =
        (RIL_RadioState) getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE);
    if (RADIO_STATE_OFF == state) {
        fillSignalStrengthResponse(rspCache, SIGNAL_GROUP_NONE);
    } else {
        sigRegisted = getCurrentSignalGroup();
        // always force update in case OOS
        if (sigRegisted == SIGNAL_GROUP_NONE)
            forceUpdate = true;
        if (forceUpdate) {
            // send AT command
            sp<RfxAtResponse> atResp = atSendCommandMultiline("AT+ECSQ", "+ECSQ:");
            RfxAtLine* atLine;

            // check error
            err = atResp->getError();
            if (err != 0
                || atResp == NULL
                || atResp->getSuccess() == 0
                || atResp->getIntermediates() == NULL) {
                goto error;
            }

            for (atLine = atResp->getIntermediates();
                 atLine != NULL;
                 atLine = atLine->getNext()) {
                sigUpdated |= updateSignalStrength(atLine);
            }
        }

        logD(LOG_TAG, "requestSignalStrength:sigRegisted[%#04X], sigUpdated[%#04X]",
            sigRegisted, sigUpdated);
        printSignalStrengthCache((char*) __FUNCTION__);

        if (msg == NULL && forceUpdate && sigUpdated == SIGNAL_GROUP_NONE)
            goto error;

        if (sigRegisted == SIGNAL_GROUP_NONE) {
            fillSignalStrengthResponse(rspCache, getLastReportedSignal());
        } else {
            fillSignalStrengthResponse(rspCache, sigRegisted);
        }
    }

    pthread_mutex_unlock(&s_signalStrengthMutex[m_slot_id]);

    if (msg != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)), msg, false);
    } else {
        response = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIGNAL_STRENGTH, m_slot_id,
                RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)));
    }
    responseToTelCore(response);

    if (enableReportSignalStrengthWithWcdmaEcio() && msg == NULL) {
        response = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIGNAL_STRENGTH_WITH_WCDMA_ECIO, m_slot_id,
                RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)));
        responseToTelCore(response);
    }
    return;

error:
    pthread_mutex_unlock(&s_signalStrengthMutex[m_slot_id]);
    logE(LOG_TAG, "requestSignalStrength must never return an error when radio is on");
    if (msg != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxIntsData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
    }
}

void RmcNetworkRequestHandler::requestSetNetworkSelectionAutomatic(const sp<RfxMclMessage>& msg) {
    int err;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno ril_errno;

    // Use +COPS to do auto selection mode if it's a test
    if (isGcfTestMode() > 0 || isTestSim(m_slot_id) > 0) {
        p_response = atSendCommand("AT+COPS=0");
    } else {
        p_response = atSendCommand("AT+EOPS=0");
    }

    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
    } else {
        ril_errno = RIL_E_SUCCESS;
        mdEops[m_slot_id]->mode = 0;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    if (ril_errno == RIL_E_SUCCESS) {
        isNeedNotifyStateChanged();
    }
}

void RmcNetworkRequestHandler::requestSetNetworkSelectionManual(const sp<RfxMclMessage>& msg) {
    int err = 0;
    const char *numeric_code;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int len, i;

    numeric_code = (char*)(msg->getData()->getData());

    if (NULL == numeric_code) {
        logE(LOG_TAG, "numeric is null!");
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    len = strlen(numeric_code);
    if (len == 6 && (strncmp(numeric_code, "123456", len) == 0)) {
        logE(LOG_TAG, "numeric is incorrect %s", numeric_code);
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }
    if (len == 5 || len == 6) {
        // check if the numeric code is valid digit or not
        for(i = 0; i < len ; i++) {
            if(numeric_code[i] < '0' || numeric_code[i] > '9')
                break;
        }
        if (i == len) {
            if (err >= 0) {
                mdEops[m_slot_id]->mode = 1;
                p_response = atSendCommand(String8::format("AT+COPS=1, 2, \"%s\"", numeric_code));
                err = p_response->getError();
                if (!(err < 0 || p_response->getSuccess() == 0)) {
                    ril_errno = RIL_E_SUCCESS;
                } else {
                    ril_errno = RIL_E_INVALID_STATE;
                    updateSbpInfos();
                }
            }
        } else {
            logE(LOG_TAG, "the numeric code contains invalid digits");
        }
    } else {
        logE(LOG_TAG, "the data length is invalid for Manual Selection");
    }

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    if (ril_errno == RIL_E_SUCCESS) {
        isNeedNotifyStateChanged();
    }
}

void RmcNetworkRequestHandler::requestSetNetworkSelectionManualWithRan(const sp<RfxMclMessage>& msg) {
    int err, len, i, act;
    const char *numeric_code;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    const char **pReqData = (const char **)msg->getData()->getData();

    if (NULL == pReqData[0] || NULL == pReqData[1]) {
        logE(LOG_TAG, "numeric or act is null!");
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    numeric_code = pReqData[0];
    /* convert AccessNetwork to 3gpp 27.007 Sec 7.3
     * AccessNetwork 0: UNKNOWN
     * AccessNetwork 1 : GERAN (0 GSM)
     * AccessNetwork 2: UTRAN (2 UTRAN)
     * AccessNetwork 3: EUTRAN (7 E-UTRAN)
     * AccessNetwork 6: NGRAN (12 NG-RAN)
     */
    switch (atoi(pReqData[1])) {
        case 1: // 2g
            act = 0;
            break;
        case 2: // 3g
            act = 2;
            break;
        case 3: // 4g
            act = 7;
            break;
        case 6: // 5g
            act = 12;
            break;
        default:
            act = -1;
            break;
    }

    len = strlen(numeric_code);
    if (len == 5 || len == 6) {
        // check if the numeric code is valid digit or not
        for(i = 0; i < len ; i++) {
            if( numeric_code[i] < '0' || numeric_code[i] > '9')
                break;
        }

        if ((len == 6 && (strncmp(numeric_code, "123456", len) == 0)) ||
            (len == 5 && (strncmp(numeric_code, "12345", len) == 0))) {
            logE(LOG_TAG, "numeric is incorrect %s", numeric_code);
            ril_errno = RIL_E_INVALID_ARGUMENTS;
            goto error;
        }

        if (i == len) {
            mdEops[m_slot_id]->mode = 1;
            if (act != -1) {
                p_response = atSendCommand(String8::format("AT+COPS=1, 2, \"%s\", %d", numeric_code, act));
            } else {
                p_response = atSendCommand(String8::format("AT+COPS=1, 2, \"%s\"", numeric_code));
            }
            err = p_response->getError();
            if (!(err < 0 || p_response->getSuccess() == 0)) {
                ril_errno = RIL_E_SUCCESS;
            } else {
                updateSbpInfos();
            }
        } else {
            logE(LOG_TAG, "the numeric code contains invalid digits");
        }
    } else {
        logE(LOG_TAG, "the data length is invalid for Manual Selection");
    }
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    if (ril_errno == RIL_E_SUCCESS) {
        isNeedNotifyStateChanged();
    }
}

void RmcNetworkRequestHandler::requestSetNetworkSelectionManualWithAct(const sp<RfxMclMessage>& msg) {
    int err, len, i, rat = 0;
    const char *numeric_code, *act, *mode;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    bool isSemiAutoMode = false;
    bool noneBlockAutoMode = false;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    const char **pReqData = (const char **)msg->getData()->getData();

    numeric_code = pReqData[0];
    act = pReqData[1];

    if (NULL == numeric_code || NULL == act) {
        logE(LOG_TAG, "numeric or act is null!");
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    if (msg->getData()->getDataLength()/sizeof(char *) == 3) {
        mode = pReqData[2];
        if (mode != NULL) {
            if (strncmp(mode, "1", 1) == 0) {
                logD(LOG_TAG, "Semi auto network selection mode");
                isSemiAutoMode = true;
            } else if (strncmp(mode, "2", 1) == 0) {
                logD(LOG_TAG, "none blocking Auto Mode");
                noneBlockAutoMode = true;
            }
        }
    }

    len = strlen(numeric_code);

    if (len == 6 && (strncmp(numeric_code, "123456", len) == 0)) {
        logE(LOG_TAG, "numeric is incorrect %s", numeric_code);
        ril_errno = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }
    if (noneBlockAutoMode) {
        p_response = atSendCommand("AT+EOPS=0");
        err = p_response->getError();
        if (!(err < 0 || p_response->getSuccess() == 0)) {
            ril_errno = RIL_E_SUCCESS;
            mdEops[m_slot_id]->mode = 0;
        }
    } else if (len == 5 || len == 6) {
        // check if the numeric code is valid digit or not
        for(i = 0; i < len ; i++) {
            if( numeric_code[i] < '0' || numeric_code[i] > '9')
                break;
        }
        rat = atoi(act);
        logD(LOG_TAG, "numeric_code = %s, act = %d", numeric_code, rat);
        if (i == len) {
            if (rat == 0 || rat == 2 || rat == 7 || rat == 11) {
                if(isSemiAutoMode == true) {
                    p_response = atSendCommand(String8::format("AT+EOPS=5, 2, \"%s\", %s", numeric_code, act));
                } else {
                    mdEops[m_slot_id]->mode = 1;
                    p_response = atSendCommand(String8::format("AT+COPS=1, 2, \"%s\", %s", numeric_code, act));
                }
            } else {
                if (isSemiAutoMode == true) {
                    p_response = atSendCommand(String8::format("AT+EOPS=5, 2, \"%s\"", numeric_code));
                } else {
                    mdEops[m_slot_id]->mode = 1;
                    p_response = atSendCommand(String8::format("AT+COPS=1, 2, \"%s\"", numeric_code));
                }
            }
            err = p_response->getError();
            if (!(err < 0 || p_response->getSuccess() == 0)) {
                ril_errno = RIL_E_SUCCESS;
            } else {
                updateSbpInfos();
            }
        } else {
            logE(LOG_TAG, "the numeric code contains invalid digits");
        }
    } else {
        logE(LOG_TAG, "the data length is invalid for Manual Selection");
    }

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    if (ril_errno == RIL_E_SUCCESS) {
        isNeedNotifyStateChanged();
    }
}

void RmcNetworkRequestHandler::requestSetBandMode(const sp<RfxMclMessage>& msg) {
    int req, gsm_band, umts_band;
    RIL_Errno ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int *pInt = (int *)msg->getData()->getData();

    req = pInt[0];
    switch (req) {
        case BAND_MODE_UNSPECIFIED: //"unspecified" (selected by baseband automatically)
            gsm_band = 0xff;
            umts_band = 0xffff;
            break;
        case BAND_MODE_EURO: //"EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
            gsm_band = GSM_BAND_900 | GSM_BAND_1800;
            umts_band = UMTS_BAND_I;
            break;
        case BAND_MODE_USA: //"US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
            gsm_band = GSM_BAND_850 | GSM_BAND_1900;
            umts_band = UMTS_BAND_II | UMTS_BAND_V;
            break;
        case BAND_MODE_JPN: //"JPN band" (WCDMA-800 / WCDMA-IMT-2000)
            gsm_band = 0;
            umts_band = UMTS_BAND_I | UMTS_BAND_VI;
            break;
        case BAND_MODE_AUS: //"AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
            gsm_band = GSM_BAND_900 | GSM_BAND_1800;
            umts_band = UMTS_BAND_I | UMTS_BAND_V;
            break;
        case BAND_MODE_AUS_2: //"AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
            gsm_band = GSM_BAND_900 | GSM_BAND_1800;
            umts_band = UMTS_BAND_V;
            break;
        case BAND_MODE_CELL_800: //"Cellular (800-MHz Band)"
        case BAND_MODE_PCS: //"PCS (1900-MHz Band)"
        case BAND_MODE_JTACS: //"Band Class 3 (JTACS Band)"
        case BAND_MODE_KOREA_PCS: //"Band Class 4 (Korean PCS Band)"
        case BAND_MODE_5_450M: //"Band Class 5 (450-MHz Band)"
        case BAND_MODE_IMT2000: // "Band Class 6 (2-GMHz IMT2000 Band)"
        case BAND_MODE_7_700M_2: //"Band Class 7 (Upper 700-MHz Band)"
        case BAND_MODE_8_1800M: //"Band Class 8 (1800-MHz Band)"
        case BAND_MODE_9_900M: //"Band Class 9 (900-MHz Band)"
        case BAND_MODE_10_800M_2: //"Band Class 10 (Secondary 800-MHz Band)"
        case BAND_MODE_EURO_PAMR_400M: //"Band Class 11 (400-MHz European PAMR Band)"
        case BAND_MODE_AWS: //"Band Class 15 (AWS Band)"
        case BAND_MODE_USA_2500M: //"Band Class 16 (US 2.5-GHz Band)"
        default:
            gsm_band = -1;
            umts_band = -1;
            break;
    }

    if (gsm_band != -1 && umts_band != -1) {
        /******************************************************
        * If the modem doesn't support certain group of bands, ex. GSM or UMTS
        * It might just ignore the parameter.
        *******************************************************/
        p_response = atSendCommand(String8::format("AT+EPBSE=%d, %d", gsm_band, umts_band));
        if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
            ril_errno = RIL_E_SUCCESS;
        }
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestQueryAvailableBandMode(const sp<RfxMclMessage>& msg) {
    int err, gsm_band, umts_band;
    int band_mode[10], index = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;

    p_response = atSendCommandSingleline("AT+EPBSE?", "+EPBSE:");

    if (p_response->getError() < 0 || p_response->getSuccess() == 0)
        goto error;

    line = p_response->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) goto error;

    // get supported GSM bands
    gsm_band = line->atTokNextint(&err);
    if (err < 0) goto error;

    // get supported UMTS bands
    umts_band = line->atTokNextint(&err);
    if (err < 0) goto error;

    //0 for "unspecified" (selected by baseband automatically)
    band_mode[index++] = BAND_MODE_UNSPECIFIED;

    if (gsm_band !=0 || umts_band != 0) {
        // 1 for "EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
        if ((gsm_band == 0 || (gsm_band | GSM_BAND_900 | GSM_BAND_1800) == gsm_band) &&
                (umts_band == 0 || (umts_band | UMTS_BAND_I) == umts_band)) {
            band_mode[index++] = BAND_MODE_EURO;
        }

        // 2 for "US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
        if ((gsm_band == 0 || (gsm_band | GSM_BAND_850 | GSM_BAND_1900) == gsm_band) &&
                (umts_band == 0 || (umts_band | UMTS_BAND_II | UMTS_BAND_V) == umts_band)) {
            band_mode[index++] = BAND_MODE_USA;
        }

        // 3 for "JPN band" (WCDMA-800 / WCDMA-IMT-2000)
        if ((umts_band | UMTS_BAND_I | UMTS_BAND_VI) == umts_band) {
            band_mode[index++] = BAND_MODE_JPN;
        }

        // 4 for "AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
        if ((gsm_band == 0 || (gsm_band | GSM_BAND_900 | GSM_BAND_1800)==gsm_band) &&
                (umts_band == 0 || (umts_band | UMTS_BAND_I | UMTS_BAND_V)==umts_band)) {
            band_mode[index++] = BAND_MODE_AUS;
        }

        // 5 for "AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
        if ((gsm_band == 0 || (gsm_band | GSM_BAND_900 | GSM_BAND_1800)==gsm_band) &&
                (umts_band == 0 || (umts_band | UMTS_BAND_V)==umts_band)) {
            band_mode[index++] = BAND_MODE_AUS_2;
        }
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(band_mode, index), msg, false);
    responseToTelCore(resp);
    return;

error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestSetLocationUpdates(const sp<RfxMclMessage>& msg) {
    int enabled;
    RIL_Errno err_no = RIL_E_SUCCESS;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int *pInt = (int *)msg->getData()->getData();

    enabled = pInt[0];
    if (enabled == 1 || enabled == 0) {
        // EREG=3, full information; EREG=5 or 9, only <state> and <eAct>
        p_response = atSendCommand(String8::format("AT+EREG=%d", enabled ? 3 : 9));
        if (p_response->getError() < 0 || p_response->getSuccess() == 0) {
            p_response = atSendCommand(String8::format("AT+EREG=%d", enabled ? 3 : 5));
            if (p_response->getError() < 0 || p_response->getSuccess() == 0) {
                err_no = RIL_E_SIM_ABSENT;
            }
        }
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), err_no,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestGetCellInfoList(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int num = 0;
    RIL_CellInfo_v12 *response = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;

    p_response = atSendCommandSingleline("AT+ECELL", "+ECELL:");

    // +ECELL: <num_of_cell>...
    if (p_response->getError() < 0 || p_response->getSuccess() == 0)
        goto error;

    line = p_response->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) goto error;

    num = line->atTokNextint(&err);
    if (err < 0) goto error;
    if (num < 1) {
        logD(LOG_TAG, "No cell info listed, num=%d", num);
        goto error;
    }
    // logD(LOG_TAG, "Cell info listed, number=%d",num);

    response = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
    memset(response, 0, num * sizeof(RIL_CellInfo_v12));

    err = getCellInfoListV12(line, num, response);
    if (err < 0) goto error;

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxCellInfoData(response, num * sizeof(RIL_CellInfo_v12)), msg, false);
    responseToTelCore(resp);
    freeOperatorNameOfCellInfo(num, response);
    return;

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
        response = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
        memset(response, 0, num * sizeof(RIL_CellInfo_v12));
        response[0].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
        response[0].registered = 0;
        response[0].timeStampType = RIL_TIMESTAMP_TYPE_UNKNOWN;
        response[0].timeStamp = 0;
        response[0].connectionStatus = NONE_SERVING;
        response[0].CellInfo.gsm.cellIdentityGsm.mnc_len = mnc_len;
        response[0].CellInfo.gsm.cellIdentityGsm.mcc = atoi(mcc_str);
        response[0].CellInfo.gsm.cellIdentityGsm.mnc = atoi(mnc_str);
        response[0].CellInfo.gsm.cellIdentityGsm.operName.long_name = &empty_string;
        response[0].CellInfo.gsm.cellIdentityGsm.operName.short_name = &empty_string;
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxCellInfoData(response, num * sizeof(RIL_CellInfo_v12)), msg, false);
        responseToTelCore(resp);
    } else {
        logE(LOG_TAG, "requestGetCellInfoList must never return error when radio is on");
        resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_NO_NETWORK_FOUND,
                RfxVoidData(), msg, false);
        responseToTelCore(resp);
    }
}

void RmcNetworkRequestHandler::requestSetCellInfoListRate(const sp<RfxMclMessage>& msg) {
    int time = -1;
    RIL_Errno err_no = RIL_E_GENERIC_FAILURE;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int *pInt = (int *)msg->getData()->getData();

    time = pInt[0];

    // logE(LOG_TAG, "requestSetCellInfoListRate:%d", time);
    if (time == 0) {
        p_response = atSendCommand(String8::format("AT+ECELL=1"));
    } else if (time > 0) {
        p_response = atSendCommand(String8::format("AT+ECELL=0"));
    } else {
        goto finish;
    }

    if (p_response->getError() >= 0 &&
            p_response->getSuccess() > 0) {
        err_no = RIL_E_SUCCESS;
    }

finish:
    resp = RfxMclMessage::obtainResponse(msg->getId(), err_no,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestGetPOLCapability(const sp<RfxMclMessage>& msg) {
    int err;
    char *mClose, *mOpen, *mHyphen;
    int result[4] = {0};
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;

    p_response = atSendCommandSingleline("AT+CPOL=?", "+CPOL:");

    if (p_response->getError() < 0) {
        logE(LOG_TAG, "requestGetPOLCapability Fail");
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        logE(LOG_TAG, "CME ERROR: %d/n", p_response->atGetCmeError());
        goto error;
    }

    //+CPOL: (<bIndex>-<eIndex>), (<bformatValue>-<eformatValue>)
    line = p_response->getIntermediates();

    // AT< +CPOL: (0-39), (0-2)
    line->atTokStart(&err);
    if (err < 0) goto error;

    // AT< +CPOL: (0-39), (0-2)
    //            ^
    mOpen = line->atTokChar(&err);
    if (err < 0) goto error;

    mHyphen = strchr(mOpen, '-');
    if (mHyphen != NULL && mOpen < mHyphen ) {
        // AT< +CPOL: (0-39), (0-2)
        //             ^
        result[0] = strtol((mOpen+1), NULL, 10);
        logD(LOG_TAG, "requestGetPOLCapability result 0: %d", result[0]);
    } else {
        goto error;
    }

    mClose = strchr(mHyphen, ')');
    if (mClose != NULL && mHyphen < mClose) {
        // AT< +CPOL: (0-39), (0-2)
        //               ^^
        result[1] = strtol((mHyphen+1), NULL, 10);
        logD(LOG_TAG, "requestGetPOLCapability result 1: %d", result[1]);
    } else {
        goto error;
    }

    // AT< +CPOL: (0-39), (0-2)
    //                    ^
    mOpen = line->atTokChar(&err);
    if (err < 0) goto error;

    mHyphen = strchr(mOpen, '-');
    if (mHyphen != NULL && mOpen < mHyphen ) {
        // AT< +CPOL: (0-39), (0-2)
        //                     ^
        result[2] = strtol((mOpen+1), NULL, 10);
        logD(LOG_TAG, "requestGetPOLCapability result 2: %d", result[2]);
    } else {
        goto error;
    }

    mClose = strchr(mHyphen, ')');
    if (mClose != NULL && mHyphen < mClose) {
        // AT< +CPOL: (0-39), (0-2)
        //                       ^
        result[3] = strtol((mHyphen+1), NULL, 10);
        logD(LOG_TAG, "requestGetPOLCapability result 3: %d", result[3]);
    } else {
        goto error;
    }

    logD(LOG_TAG, "requestGetPOLCapability: %d %d %d %d", result[0],
            result[1], result[2], result[3]);

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(result, 4), msg, false);
    responseToTelCore(resp);
    return;

error:
    logD(LOG_TAG, "requestGetPOLCapability: goto error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestGetPOLList(const sp<RfxMclMessage>& msg) {
    int err = 0, i = 0, j = 0, count = 0, nAct = 0, tmpAct = 0;
    char **response = NULL;
    char *tmp, *block_p = NULL;
    sp<RfxAtResponse> p_response;
    RfxAtLine *p_cur;
    sp<RfxMclMessage> resp;

    p_response = atSendCommandMultiline("AT+CPOL?", "+CPOL:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    /* count the entries */
    for (count = 0, p_cur = p_response->getIntermediates()
            ; p_cur != NULL
            ; p_cur = p_cur->getNext()) {
        count++;
    }
    logD(LOG_TAG, "requestGetPOLList!! count is %d", count);

    response = (char **) calloc(1, sizeof(char*) * count *4); // for string, each one is 25 bytes
    /* In order to support E-UTRAN, nAct will be 2 digital,
    changed from 60 to 62 for addition 1 digital and buffer.*/
    block_p = (char *) calloc(1, count* sizeof(char)*62);

    if (response == NULL || block_p == NULL) {
        logE(LOG_TAG, "requestGetPOLList calloc fail");
        goto error;
    }
    //+CPOL: <index>, <format>, <oper>, <GSM_Act>, <GSM_Compact_Act>, <UTRAN_Act>, <E-UTRAN Act>
    for (i = 0,j=0, p_cur = p_response->getIntermediates()
            ; p_cur != NULL
            ; p_cur = p_cur->getNext(), i++,j+=4) {
        logD(LOG_TAG, "requestGetPOLList!! line is %s", p_cur->getLine());

        p_cur->atTokStart(&err);
        if (err < 0) goto error;

        /* get index*/
        tmp = p_cur->atTokNextstr(&err);
        if (err < 0) goto error;

        response[j+0] = &block_p[i*62];
        response[j+1] = &block_p[i*62+8];
        response[j+2] = &block_p[i*62+10];
        response[j+3] = &block_p[i*62+58];

        err = sprintf(response[j+0], "%s", tmp);
        if (err < 0) logE(LOG_TAG, "sprintf response[j+0] fail");

        logD(LOG_TAG, "requestGetPOLList!! index is %s",response[j+0]);
        /* get format*/
        tmp = p_cur->atTokNextstr(&err);
        if (err < 0) goto error;

        err = sprintf(response[j+1], "%s", tmp);
        if (err < 0) logE(LOG_TAG, "sprintf response[j+1] fail");
        logD(LOG_TAG, "requestGetPOLList!! format is %s",response[j+1]);
        /* get oper*/
        tmp = p_cur->atTokNextstr(&err);
        if (err < 0) goto error;

        err = sprintf(response[j+2], "%s", tmp);
        if (err < 0) logE(LOG_TAG, "sprintf response[j+2] fail");
        logD(LOG_TAG, "requestGetPOLList!! oper is %s",response[j+2]);
        nAct = 0;

        if(p_cur->atTokHasmore()) {
            /* get <GSM AcT> */
            tmpAct = p_cur->atTokNextint(&err);
            if (err < 0) goto error;

            if (tmpAct == 1) {
                nAct = 1;
            }

            /*get <GSM compact AcT> */
            tmpAct = p_cur->atTokNextint(&err);
            if (err < 0) goto error;

            if(tmpAct == 1) {
                nAct |= 0x02;
            }

            /*get <UTRAN AcT> */
            tmpAct = p_cur->atTokNextint(&err);
            if (err < 0) goto error;

            if (tmpAct == 1) {
                nAct |= 0x04;
            }

            /*get <E-UTRAN AcT> */
            if (p_cur->atTokHasmore()) {
                logD(LOG_TAG, "get E-UTRAN AcT");
                tmpAct = p_cur->atTokNextint(&err);
                if (err < 0) goto error;

                if(tmpAct == 1) {
                    nAct |= 0x08;
                }
            }

            /*get <NR> */
            if (isNrSupported()) {
                if (p_cur->atTokHasmore()) {
                    logD(LOG_TAG, "get NR AcT");
                    tmpAct = p_cur->atTokNextint(&err);
                    if (err < 0) goto error;

                    if(tmpAct == 1) {
                        nAct |= 0x10;
                    }
                }
            }
        }
        /* ALPS00368351 To distinguish SIM file without <AcT> support, we set AcT to zero */
        // if(nAct == 0) { nAct = 1;} // No act value for SIM. set to GSM
        logD(LOG_TAG, "Act = %d",nAct);
        err = sprintf(response[j+3], "%d", nAct);
        if (err < 0) logE(LOG_TAG, "sprintf response[j+3] fail");
        logD(LOG_TAG, "requestGetPOLList!! act is %s",response[j+3]);
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData(response, count*4), msg, false);
    responseToTelCore(resp);
    free(response);
    free(block_p);
    return;

error:
    logE(LOG_TAG, "requestGetPOLList return error");
    if (response) {
        free(response);
    }
    if (block_p) {
        free(block_p);
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestSetPOLEntry(const sp<RfxMclMessage>& msg) {
    int i;
    int nAct[5] = {0};
    int nActTmp = 0;
    const char **strings = (const char **)msg->getData()->getData();
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;

    if (msg->getData()->getDataLength() < (int)(3 * sizeof(char*))) {
        logE(LOG_TAG, "requestSetPOLEntry no enough input params. datalen is %d, size of char* is %zu",
                msg->getData()->getDataLength(), sizeof(char*));
        p_response = atSendCommand(String8::format("AT+CPOL=%s", strings[0]));
    } else if (strings[1] == NULL || strcmp("", strings[1]) == 0) { // no PLMN, then clean the entry
        p_response = atSendCommand(String8::format("AT+CPOL=%s", strings[0]));
    } else {
        nActTmp = atoi(strings[2]);
        logD(LOG_TAG, "requestSetPOLEntry Act = %d", nActTmp);

        // <bit4, bit3, bit2,bit1,bit0>  <NR, E-UTRAN_Act, UTRAN_Act, GSM_Compact_Act, Gsm_Act>
        for (i = 0; i < 5; i++) {
            if (((nActTmp >> i) & 1) == 1) {
                logD(LOG_TAG, "i = %d",i);
                nAct[i] = 1;
            }
        }

        /* ALPS00368351: To update file without <AcT> support, modem suggest not to set any nAcT parameter */
        if (nActTmp == 0) {
            logD(LOG_TAG, "requestSetPOLEntry no Act assigned,strings[2]=%s",strings[2]);
            p_response = atSendCommand(String8::format("AT+CPOL=%s,2,\"%s\"", strings[0], strings[1]));
        } else if (isNrSupported()) {
            logD(LOG_TAG, "NR supoorts 8 arguments");
            p_response = atSendCommand(String8::format("AT+CPOL=%s,2,\"%s\",%d,%d,%d,%d,%d", strings[0], strings[1], nAct[0], nAct[1], nAct[2], nAct[3], nAct[4]));
        } else {
            logD(LOG_TAG, "R8, MOLY and LR9 can supoort 7 arguments");
            p_response = atSendCommand(String8::format("AT+CPOL=%s,2,\"%s\",%d,%d,%d,%d", strings[0], strings[1], nAct[0], nAct[1], nAct[2], nAct[3]));
        }
    }
    if (p_response->getError() < 0) {
        logE(LOG_TAG, "requestSetPOLEntry Fail");
        goto finish;
    }

    if (p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            logD(LOG_TAG, "p_response = %d/n", p_response->atGetCmeError());
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
            break;
            case CME_UNKNOWN:
                logD(LOG_TAG, "p_response: CME_UNKNOWN");
            break;
            default:
            break;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }

finish:
    resp = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestSetCdmaRoamingPreference(const sp<RfxMclMessage>& msg) {
    int err;
    int reqRoamingType = -1;
    int roamingType = -1;
    RIL_Errno ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    int *pInt = (int *) msg->getData()->getData();
    reqRoamingType = pInt[0];

    // AT$ROAM=<type>
    // <type>=0: set the device to Sprint only mode
    // <type>=1: set the device to automatic mode

    if (reqRoamingType == CDMA_ROAMING_MODE_HOME) {
        // for Home Networks only
        roamingType = 0;
    } else if (reqRoamingType == CDMA_ROAMING_MODE_ANY) {
        // for Roaming on Any Network
        roamingType = 1;
    } else {
        logE(LOG_TAG, "requestSetCdmaRoamingPreference, Not support reqRoamingType=%d", reqRoamingType);
    }

    if (roamingType >= 0) {
        p_response = atSendCommand(String8::format("AT$ROAM=%d", roamingType));
        err = p_response->getError();
        if (err != 0 || p_response == NULL || p_response->getSuccess() == 0) {
            logE(LOG_TAG, "requestSetCdmaRoamingPreference error, reqRoamingType=%d", reqRoamingType);
        } else {
            ril_errno = RIL_E_SUCCESS;
        }
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestQueryCdmaRoamingPreference(const sp<RfxMclMessage>& msg) {
    int err;
    int roamingType = -1;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RfxAtLine* line;

    // AT$ROAM=<type>
    // <type>=0: set the device to Sprint only mode
    // <type>=1: set the device to automatic mode

    p_response = atSendCommandSingleline("AT$ROAM?", "$ROAM:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) goto error;

    line = p_response->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) goto error;

    // <type>
    roamingType = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (roamingType == 0) {
        // for Home Networks only
        roamingType = CDMA_ROAMING_MODE_HOME;
    } else if (roamingType == 1) {
        // for Roaming on Any Network
        roamingType = CDMA_ROAMING_MODE_ANY;
    } else {
        logE(LOG_TAG, "requestQueryCdmaRoamingPreference, Not support roamingType=%d", roamingType);
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&roamingType, 1), msg, false);
    responseToTelCore(response);
    return;
error:
    logE(LOG_TAG, "requestQueryCdmaRoamingPreference error");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_REQUEST_NOT_SUPPORTED,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::setUnsolResponseFilterSignalStrength(bool enable)
{
    sp<RfxAtResponse> p_response;

    // The ePDG/RNS framework need to monitoring the LTE RSRP signal strength across the threshold.
    char threshold[RFX_PROPERTY_VALUE_MAX] = {0};

    if (enable) {
        // enable
        /* Enable get ECSQ URC */
        p_response = atSendCommand("AT+ECSQ=1");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+ECSQ=1");

        // The ePDG/RNS framework need to monitoring the LTE RSRP signal strength across the threshold.
        // So we send command to adjust signaling threshold to MD1 whenever screen on/off.
        rfx_property_get("vendor.net.handover.thlte", threshold, "");
        /*
        if (strlen(threshold) == 0) {
            logD(LOG_TAG, "vendor.net.handover.thlte is empty");
        }
        */
    } else {
        // disable
        /* Disable get ECSQ URC */
        p_response = atSendCommand("AT+ECSQ=0");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+ECSQ=0");

        // The ePDG/RNS framework need to monitoring the LTE RSRP signal strength across the threshold.
        // So we send command to adjust signaling threshold to MD1 whenever screen on/off.
        rfx_property_get("vendor.net.handover.thlte", threshold, "");
        if (strlen(threshold) != 0) {
            p_response = atSendCommand(String8::format("AT+ECSQ=3,3,%s", threshold));
            logD(LOG_TAG, "%S, adjust signaling threshold %s", __FUNCTION__, threshold);
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+ECSQ=3,3..");
        } else {
            // logD(LOG_TAG, "vendor.net.handover.thlte is empty");
        }
    }
}

void RmcNetworkRequestHandler::setUnsolResponseFilterNetworkState(bool enable)
{
    sp<RfxAtResponse> p_response;

    if (enable) {
        // enable; screen is on.

        /* enable EREG URC with full information */
        p_response = atSendCommand("AT+EREG=3");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+EREG=3");

        /* enable EGERG URC with full information */
        p_response = atSendCommand("AT+EGREG=3");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+EGREG=3");

        /* Enable PSBEARER URC */
        p_response = atSendCommand("AT+PSBEARER=1");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+PSBEARER=1");

        /* Enable ECSG URC */
        if (isFemtocellSupport()) {
            p_response = atSendCommand("AT+ECSG=4,1");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+ECEREG=0");
        }

        /* Enable EMODCFG URC */
        if (isEnableModulationReport()) {
            p_response = atSendCommand("AT+EMODCFG=1");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EMODCFG=1");
        }

        /* Enable EREGINFO URC */
        p_response = atSendCommand("AT+EREGINFO=1");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+EREGINFO=1");

        /* Query EIPRL URC */
        p_response = atSendCommand("AT+EIPRL?");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            // logW(LOG_TAG, "There is something wrong with the exectution of AT+EIPRL?");

        /* Enable EDEFROAM URC */
        p_response = atSendCommand("AT+EDEFROAM=1");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
            logW(LOG_TAG, "There is something wrong with the exectution of AT+EDEFROAM=1");
        } else {
            p_response = atSendCommand("AT+EDEFROAM?");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EDEFROAM?");
        }

        /* Enable EMCCMNC URC */
        if (supportMccMncUrc()) {
            p_response = atSendCommand("AT+EMCCMNC=1");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EMCCMNC=1");
            }
        }

        if (!keep_rrc_urc) {
            /* Enable connection state URC */
            p_response = atSendCommand("AT+CSCON=3");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+CSCON=2");
            }
            // update LTE RRC state
            updateConnectionStateReport(UPDATE_CONNECTION_STATE_REASON_SCREEN_ON);
        }

        /* Enable Gen97 MCC MNC update URC */
        if (support_eops_urc) {
            p_response = atSendCommand("AT+EOPS=6,1");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EOPS=6,1");
            }
        }

        /* Enable Gen97 CAMP update URC */
        p_response = atSendCommand("AT+ECAMPUI=1");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
            logW(LOG_TAG, "There is something wrong with the exectution of AT+ECAMPUI=1");
        }

        /* Enable Gen97 NRCABAND URC */
        if (isNrSupported()) {
            p_response = atSendCommand("AT+ENRCABAND=1");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+ENRCABAND=1");
            }
            updateNrCaBandInfo();
        }

        /* Enable ENDC capability URC */
        p_response = atSendCommand("AT+ECAPABILITY=1");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
            logW(LOG_TAG, "There is something wrong with the exectution of AT+ECAPABILITY=1");
        }
        updateEndcCapability();

        if ((enableCaInfoUrc == 1) || ((enableCaInfoUrc == 2) && (support_dmftc02[m_slot_id] == 0))) {
            /* enable ECAINFO URC with full information */
            p_response = atSendCommand("AT+ECAINFO=1");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+ECAINFO=1");
        }
    } else {
        // disable; screen is off
        /* enable EREG URC when <stat>&<eAct> changes */
        p_response = atSendCommand("AT+EREG=9");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
            p_response = atSendCommand("AT+EREG=5");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EREG=5");
            }
        }

        /* enable EGREG URC when <stat>&<eAct> changes */
        p_response = atSendCommand("AT+EGREG=9");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
            p_response = atSendCommand("AT+EGREG=5");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EGREG=5");
            }
        }

        /* Disable PSBEARER URC */
        p_response = atSendCommand("AT+PSBEARER=0");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+PSBEARER=0");

        if (isFemtocellSupport()) {
            /* Disable ECSG URC */
            p_response = atSendCommand("AT+ECSG=4,0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+ECSG=4,0");
        }

        /* Disable EMODCFG URC */
        if (isEnableModulationReport()) {
            p_response = atSendCommand("AT+EMODCFG=0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EMODCFG=0");
        }

        /* Disable EREGINFO URC */
        p_response = atSendCommand("AT+EREGINFO=0");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+EREGINFO=0");

        /* Disable EDEFROAM URC */
        p_response = atSendCommand("AT+EDEFROAM=0");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0)
            logW(LOG_TAG, "There is something wrong with the exectution of AT+EDEFROAM=0");

        /* Disable EMCCMNC URC */
        if (supportMccMncUrc()) {
            p_response = atSendCommand("AT+EMCCMNC=0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EMCCMNC=0");
            }
        }

        if (!keep_rrc_urc) {
            /* Disable connection state URC */
            p_response = atSendCommand("AT+CSCON=0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+CSCON=0");
            }
        }

        /* Disable Gen97 MCC MNC URC */
        if (support_eops_urc) {
            p_response = atSendCommand("AT+EOPS=6,0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EOPS=6,0");
            }
        }

        /* Disable Gen97 CAMP update URC */
        p_response = atSendCommand("AT+ECAMPUI=0");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
            logW(LOG_TAG, "There is something wrong with the exectution of AT+ECAMPUI=0");
        }

        /* Disable Gen97 NRCABAND URC */
        if (isNrSupported()) {
            p_response = atSendCommand("AT+ENRCABAND=0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+ENRCABAND=0");
            }
        }
        /* Disable ENDC capability URC */
        p_response = atSendCommand("AT+ECAPABILITY=0");
        if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
            logW(LOG_TAG, "There is something wrong with the exectution of AT+ECAPABILITY=0");
        }

        if ((enableCaInfoUrc == 1) || ((enableCaInfoUrc == 2) && (support_dmftc02[m_slot_id] == 0))) {
            /* disable ECAINFO URC with full information */
            p_response = atSendCommand("AT+ECAINFO=0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0)
                logW(LOG_TAG, "There is something wrong with the exectution of AT+ECAINFO=0");
        }
    }
}

void RmcNetworkRequestHandler::setUnsolResponseFilterPhyChlCfg(bool enable){
    sp<RfxAtResponse> p_response;

    if (enable) {
        physicalConfigSwitch[m_slot_id] = true;

        /* Enable EPHYCONFIG URC */
        if (mPhyChlCfgMode == 2) {
            /* Enable EPHYCONFIG URC*/
            p_response = atSendCommand("AT+EPHYCONFIG=1");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EPHYCONFIG=1");
            }
            updatePhyChlCfgUrcByQuery();
        }
    } else {
        physicalConfigSwitch[m_slot_id] = false;

        /* Disable EPHYCONFIG URC */
        if (mPhyChlCfgMode == 2) {
            /* Disable EPHYCONFIG URC*/
            p_response = atSendCommand("AT+EPHYCONFIG=0");
            if (p_response->getError() != 0 || p_response->getSuccess() == 0) {
                logW(LOG_TAG, "There is something wrong with the exectution of AT+EPHYCONFIG=0");
            }
        }
    }
}

void RmcNetworkRequestHandler::setUnsolResponseFilterLinkCapacityEstimate(bool enable) {
    // We should not turn off link capacity update if one of the following condition is true.
    // 1. The device is charging.
    // 2. When the screen is on.
    // 3. When data tethering is on.
    // 4. When the update mode is IGNORE_SCREEN_OFF.
    // In all other cases, we turn off link capacity update.
    // AT+EXLCE=<mode> to turn on/off link capacity update.
    // <mode> = 0: disable; 1: enable;
    int tempLce = (enable ? 1 : 0);
    if (mCurrentLceMode == tempLce) return;

    mCurrentLceMode = tempLce;
    sp<RfxAtResponse> pAtResponse = atSendCommand(String8::format("AT+EXLCE=%d", enable));

    if (pAtResponse == NULL) {
        logE(LOG_TAG, "setUnsolResponseFilterLinkCapacityEstimate: Fail to set AT+EXLCE=%d", enable);
    } else if (pAtResponse->isAtResponseFail()) {
        logE(LOG_TAG, "setUnsolResponseFilterLinkCapacityEstimate: Fail to set AT+EXLCE=%d, ERROR:%d",
                enable, pAtResponse->atGetCmeError());
    }
}

void RmcNetworkRequestHandler::requestSetUnsolicitedResponseFilter(const sp<RfxMclMessage>& msg) {
    /************************************
    * Control the URC: ECSQ,CREG,CGREG,CEREG,
    * CIREG,PSBEARER,ECSG,EMODCFG,EREGINFO,
    * EXLCE.
    *************************************/

    RIL_UnsolicitedResponseFilter filter;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_UnsolicitedResponseFilter *pUnsolicitedResponseFilter
            = (RIL_UnsolicitedResponseFilter *)msg->getData()->getData();

    filter = pUnsolicitedResponseFilter[0];

    if ((filter & RIL_UR_SIGNAL_STRENGTH) == RIL_UR_SIGNAL_STRENGTH) {
        // enable
        setUnsolResponseFilterSignalStrength(true);
        requestSignalStrength(NULL, true);
    } else {
        // disable
        setUnsolResponseFilterSignalStrength(false);
    }
    if ((filter & RIL_UR_FULL_NETWORK_STATE) == RIL_UR_FULL_NETWORK_STATE) {
        // enable
        setUnsolResponseFilterNetworkState(true);
    } else {
        // disable
        setUnsolResponseFilterNetworkState(false);
    }
    if ((filter & LINK_CAPACITY_ESTIMATE) == LINK_CAPACITY_ESTIMATE) {
        // enable
        setUnsolResponseFilterLinkCapacityEstimate(true);
    } else {
        // disable
        setUnsolResponseFilterLinkCapacityEstimate(false);
    }
    if ((filter & PHYSICAL_CHANNEL_CONFIG) == PHYSICAL_CHANNEL_CONFIG) {
        // enable
        setUnsolResponseFilterPhyChlCfg(true);
    } else {
        // disable
        setUnsolResponseFilterPhyChlCfg(false);
    }
    if ((filter & REGISTRATION_FAILURE) == REGISTRATION_FAILURE) {
        // enable
        registrationFailNotificationSwitch[m_slot_id] = true;
    } else {
        // disable
        registrationFailNotificationSwitch[m_slot_id] = false;
    }
    if ((filter & BARRING_INFO) == BARRING_INFO) {
        // enable
        p_response = atSendCommand("AT+EBARRING=1");
    } else {
        // disable
        p_response = atSendCommand("AT+EBARRING=0");
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

int RmcNetworkRequestHandler::isEnableModulationReport()
{
    char optr[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.operator.optr", optr, "");

    return (strcmp("OP08", optr) == 0) ? 1 : 0;
}

void RmcNetworkRequestHandler::requestAbortFemtocellList(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> p_response;

    if (m_csgListOngoing[m_slot_id]) {
        m_csgListAbort[m_slot_id] = true;
        p_response = atSendCommand("AT+ECSG=2");
        logD(LOG_TAG, "requestAbortFemtocellList %d, %d", p_response->getError(), p_response->getSuccess());
        // check error
        if (p_response == NULL ||
                p_response->getError() != 0 ||
                p_response->getSuccess() == 0) {
            m_csgListAbort[m_slot_id] = false;
            logE(LOG_TAG, "requestAbortFemtocellList fail.");
            goto error;
        }
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "requestAbortFemtocellList must never return error when radio is on");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestSelectFemtocell(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> p_response;
    const char **strings = (const char **)msg->getData()->getData();
    //parameters:  <plmn> , <act> , <csg id>

    // check parameters
    if ((msg->getData()->getDataLength() < (int)(3 * sizeof(char*)))
            || (strings[0] == NULL)
            || (strings[1] == NULL)
            || (strings[2] == NULL)) {
        logE(LOG_TAG, "requestSelectFemtocell parameters wrong datalen = %d",
                msg->getData()->getDataLength());
        goto error;
    }

    // check <plmn> is valid digit
    for (size_t i = 0; i < strlen(strings[0]); i++) {
        if (strings[0][i] < '0' || strings[0][i] > '9') {
            logE(LOG_TAG, "requestSelectFemtocell parameters[0] wrong");
            goto error;
        }
    }

    // check <csg id>
    for (size_t i = 0; i < strlen(strings[2]); i++) {
        if (strings[2][i] < '0' || strings[2][i] > '9') {
            logE(LOG_TAG, "requestSelectFemtocell parameters[2] wrong");
            goto error;
        }
    }

    p_response = atSendCommand(String8::format("AT+ECSG=1,\"%s\",%s,%s", strings[0],strings[2],strings[1]));
    // check error
    if (p_response == NULL ||
            p_response->getError() != 0 ||
            p_response->getSuccess() == 0) {
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "requestSelectFemtocell must never return error when radio is on");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}
void RmcNetworkRequestHandler::requestQueryFemtoCellSystemSelectionMode(const sp<RfxMclMessage>& msg) {
    int mode = 0, err;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;

    logD(LOG_TAG, "requestQueryFemtoCellSystemSelectionMode sending AT command");
    p_response = atSendCommandSingleline("AT+EFSS?", "+EFSS:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) goto error;

    // handle intermediate
    line = p_response->getIntermediates();

    /* +EFSS: <mode>
       AT Response Example
       +EFSS: 0 */

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    mode = line->atTokNextint(&err);
    if (err < 0) goto error;

    logD(LOG_TAG, "requestQueryFemtoCellSystemSelectionMode sucess, free memory");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(resp);
    return;
error:
    logD(LOG_TAG,
        "requestQueryFemtoCellSystemSelectionMode must never return error when radio is on");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestSetFemtoCellSystemSelectionMode(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int *pInt = (int *)msg->getData()->getData();
    int mode = pInt[0];

    logD(LOG_TAG, "requestSetFemtoCellSystemSelectionMode: mode=%d", mode);

    if ((mode >= 0) && (mode <= 2)) {
        p_response = atSendCommand(String8::format("AT+EFSS=%d", mode));
        if (p_response->getError() >= 0 || p_response->getSuccess() != 0) {
            ril_errno = RIL_E_SUCCESS;
        }
    } else {
        logE(LOG_TAG, "mode is invalid");
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;
}

void RmcNetworkRequestHandler::requestSetServiceState(const sp<RfxMclMessage>& msg) {
    int voice_reg_state, data_reg_state;
    int voice_roaming_type, data_roaming_type;
    int ril_voice_reg_state, ril_rata_reg_state;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int *pInt = (int *)msg->getData()->getData();

    voice_reg_state = pInt[0];
    data_reg_state = pInt[1];
    voice_roaming_type = pInt[2];
    data_roaming_type = pInt[3];
    ril_voice_reg_state = pInt[4];
    ril_rata_reg_state = pInt[5];

    if ((voice_reg_state >= 0 && voice_reg_state <= 3) &&
            (data_reg_state >= 0 && data_reg_state <= 3) &&
            (voice_roaming_type >= 0 && voice_roaming_type <= 3) &&
            (data_roaming_type >= 0 && data_roaming_type <= 3) &&
            (ril_voice_reg_state >= 0 && ril_voice_reg_state <= 14) &&
            (ril_rata_reg_state >= 0 && ril_rata_reg_state <= 14)) {
        /*****************************
        * If all parameters are valid,
        * set to MD
        ******************************/
        p_response = atSendCommand(String8::format("AT+ESRVSTATE=%d,%d,%d,%d,%d,%d",
                voice_reg_state,
                data_reg_state,
                voice_roaming_type,
                data_roaming_type,
                ril_voice_reg_state,
                ril_rata_reg_state));
        if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
            ril_errno = RIL_E_SUCCESS;
        }
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::handleConfirmRatBegin(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    sp<RfxAtResponse> p_response;
    bool ret = true;
    int err;
    int count = 0;

    while (ret) {
        p_response = atSendCommand("AT+ERPRAT");
        err = p_response->getError();

        ret = (err < 0 || 0 == p_response->getSuccess()) ? true:false;
        logV(LOG_TAG, "confirmRatBegin, send command AT+ERPRAT, err = %d, ret=%d, count=%d",
            err, ret, count);
        count++;
        // If get wrong result, we need to check whether go on or not.
        if (ret) {
            if (count == 30) {
                logD(LOG_TAG, "confirmRatBegin, reach the maximum time, return directly.");
                break;
            }

            RIL_RadioState state = (RIL_RadioState) getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE);
            if (RADIO_STATE_UNAVAILABLE == state || RADIO_STATE_OFF == state) {
                logD(LOG_TAG, "confirmRatBegin, radio unavliable/off, return directly.");
                break;
            }
            // Go on retry after 5 seconds.
            sleep(1);
        }
    };
}

void RmcNetworkRequestHandler::handleCsNetworkStateEvent(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *) msg->getData()->getData();
    int prevRegState = pInt[0];
    int prevRat = pInt[1];
    int newRegState = pInt[2];
    int newRat = pInt[3];
    if ((prevRegState != newRegState
            || prevRat != newRat)
            && RfxNwServiceState::isInService(newRegState)) {
        logV(LOG_TAG, "handleCsNetworkStateEvent, need requestSignalStrength, "
                "prevRegState=%d, prevRat=%d, newRegState=%d, newRat=%d",
                prevRegState, prevRat, newRegState, newRat);
        requestSignalStrength(NULL, false);
    }
}

void RmcNetworkRequestHandler::handlePsNetworkStateEvent(const sp<RfxMclMessage>& msg) {
    // response[0] = state, response[1] = mccmnc, response[2] = rat.
    int response[3];
    int *pInt = (int *)msg->getData()->getData();
    // state (consider iwlan)
    response[0] = pInt[2];
    // rat (consider iwlan)
    response[2] = pInt[3];
    int operNumericLength = 0;

    int err;
    int skip;
    char *eops_response = NULL;
    sp<RfxMclMessage> urc;
    RfxAtLine* line;
    sp<RfxAtResponse> p_response;

    // Update SignalStrength for PS state change
    int prevRegState = pInt[0];
    int prevRat = pInt[1];
    int newRegState = pInt[2];
    int newRat = pInt[3];
    if ((prevRegState != newRegState
            || prevRat != newRat)
            && RfxNwServiceState::isInService(newRegState)) {
        logV(LOG_TAG, "handlePsNetworkStateEvent, need requestSignalStrength, "
                "prevRegState=%d, prevRat=%d, newRegState=%d, newRat=%d",
                prevRegState, prevRat, newRegState, newRat);
        requestSignalStrength(NULL, false);
    }
    if (urc_enrcaband_enable && isInService(prevRegState) &&
        (!isInService(newRegState) || (prevRat == RADIO_TECH_NR && newRat != RADIO_TECH_NR))) {
        int data[3] = {0};
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_CA_BAND_IND, m_slot_id, RfxIntsData(data, 3));
        responseToTelCore(urc);
    }

    /* Format should be set during initialization */
    p_response = atSendCommandSingleline("AT+EOPS?", "+EOPS:");

    // check error
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        logE(LOG_TAG, "EOPS got error response");
    } else {
        // handle intermediate
        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err >= 0) {
            /* <mode> */
            skip = line->atTokNextint(&err);
            if ((err >= 0) && (skip >= 0 && skip <= 4 && skip != 2)) {
                // a "+EOPS: 0" response is possible
                if (line->atTokHasmore()) {
                    /* <format> */
                    skip = line->atTokNextint(&err);
                    if (err >= 0 && skip == 2)
                    {
                        /* <oper> */
                        eops_response = line->atTokNextstr(&err);
                        /* Modem might response invalid PLMN ex: "", "000000" , "??????", all convert to "000000" */
                        if (!((eops_response[0] >= '0') && (eops_response[0] <= '9'))) {
                            // logE(LOG_TAG, "EOPS got invalid plmn response");
                            memset(eops_response, 0, operNumericLength);
                        }
                    }
                }
            }
        }
    }
    if (eops_response != NULL) {
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED,
                String8::format("%s", eops_response));
        response[1] = atoi(eops_response);
    } else {
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED, String8(""));
        response[1] = 0;
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED,
            m_slot_id, RfxIntsData(response, 3));
    // response to TeleCore
    responseToTelCore(urc);

    return;
}

void RmcNetworkRequestHandler::triggerPollNetworkState() {
    logD(LOG_TAG, "triggerPollNetworkState");

    // update signal strength
    atSendCommand("AT+ECSQ");

    // update voice/data/Operator
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(urc);
}

void RmcNetworkRequestHandler::onHandleTimer() {
    // do something
}

void RmcNetworkRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    switch (id) {
        case RFX_MSG_EVENT_EXIT_EMERGENCY_CALLBACK_MODE:
            triggerPollNetworkState();
            break;
        case RFX_MSG_EVENT_FEMTOCELL_UPDATE:
            updateFemtoCellInfo();
            break;
        case RFX_MSG_EVENT_CONFIRM_RAT_BEGIN:
            handleConfirmRatBegin(msg);
            break;
        case RFX_MSG_EVENT_CS_NETWORK_STATE:
            handleCsNetworkStateEvent(msg);
            break;
        case RFX_MSG_EVENT_PS_NETWORK_STATE:
            handlePsNetworkStateEvent(msg);
            break;
        case RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG:
            currentPhysicalChannelConfigs(false);
            break;
        case RFX_MSG_EVENT_RSP_DATA_CONTEXT_IDS:
            if (handleGetDataContextIds(msg)) {
                currentPhysicalChannelConfigs(false);
            }
            break;
        case RFX_MSG_EVENT_SMART_RAT_SWATCH_BACK:
            smartRatSwitchBack();
            break;
        case RFX_MSG_EVENT_OEM_HOOK_TO_NW:
            onListenAtFromOemHook(msg);
            break;
        case RFX_MSG_EVENT_REGISTRATION_FAILED:
            handleRegistrationFailedEvent(msg);
            break;
        case RFX_MSG_EVENT_JP_MODE_TIMER_EVENT:
            onHandleJpModeTimerEvent(msg);
            break;
        case RFX_MSG_EVENT_JP_MODE_CONNECTION_EVENT:
            onHandleJpModeConnectionEvent(msg);
            break;
        case RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM:
            onHandleRetryNotifySystem(msg);
            break;
        case RFX_MSG_EVENT_5G_ICON_SHOW_RULE_SYNC_EVENT:
            onSync5GIconShowRuleWithOtherSlot(msg);
            break;
        default:
            logE(LOG_TAG, "onHandleEvent, should not be here");
            break;
    }
}

void RmcNetworkRequestHandler::updatePseudoCellMode() {
    sp<RfxAtResponse> p_response;
    char *property = NULL;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    int err = 0;

    p_response = atSendCommandSingleline("AT+EAPC?", "+EAPC:");
    if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
        // set property if modem support APC, EM will check this property to show APC setting
        rfx_property_set("vendor.ril.apc.support", "1");
        // check if the APC mode was set before, if yes, send the same at command again
        // AT+EAPC? was apc query command, if return it, means APC mode was not set before
        err = asprintf(&property, "persist.vendor.radio.apc.mode%d", m_slot_id);
        if (err < 0) {
            logE(LOG_TAG, "updatePseudoCellMode::using asprintf and getting ERROR");
            return;
        }
        rfx_property_get(property, prop, "AT+EAPC?");
        logV(LOG_TAG, "updatePseudoCellMode: %s = %s", property, prop);
        free(property);
        if (strcmp("AT+EAPC?", prop) != 0) {
            atSendCommand(prop);
        }
    }
}

void RmcNetworkRequestHandler::requestSetPseudoCellMode(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    char *property = NULL;
    char *cmd = NULL;
    int *pInt = (int *)msg->getData()->getData();
    int apc_mode = pInt[0];
    int urc_enable = pInt[1];
    int timer = pInt[2];
    int err = 0;
    /*
    *  apc_mode = 0: disable APC feature
    *  apc_mode = 1: set APC mode I, if detect a pseudo cell, not attach it
    *  apc_mode = 2: set APC mode II, if detect a pseudo cell, also attach it
    */
    err = asprintf(&cmd, "AT+EAPC=%d,%d,%d", apc_mode, urc_enable, timer);
    if (err < 0) {
        logE(LOG_TAG, "requestSetPseudoCellMode::using asprintf and getting ERROR");
        goto error;
    }
    p_response = atSendCommand(cmd);
    if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
        ril_errno = RIL_E_SUCCESS;
    }
    if (ril_errno == RIL_E_SUCCESS) {
        err = asprintf(&property, "persist.vendor.radio.apc.mode%d", m_slot_id);
        if (err < 0) {
            logE(LOG_TAG, "requestSetPseudoCellMode::using asprintf and getting ERROR");
            goto error;
        }
        rfx_property_set(property, cmd);
        free(property);
    } else {
        logE(LOG_TAG, "requestSetPseudoCellMode failed");
    }
error:
    if (cmd) free(cmd);
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}


void RmcNetworkRequestHandler::setRoamingEnable(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    char *cmd = NULL;
    int *pInt = (int*)msg->getData()->getData();
    int err = 0;
    /*
        data[0] : phone id (0,1,2,3,...)
        data[1] : international_voice_text_roaming (0,1)
        data[2] : international_data_roaming (0,1)
        data[3] : domestic_voice_text_roaming (0,1)
        data[4] : domestic_data_roaming (0,1)
        data[5] : domestic_LTE_data_roaming (0,1)

    +EROAMBAR:<protocol_index>, (not ready now)
        <BAR_Dom_Voice_Roaming_Enabled>,
        <BAR_Dom_Data_Roaming_Enabled>,
        <Bar_Int_Voice_Roaming_Enabled>,
        <Bar_Int_Data_Roaming_Enabled>,
        <Bar_LTE_Data_Roaming_Enabled>
        NOTE: The order is different.
    */
    int (*p)[6] = (int(*)[6])pInt;
    // rever the setting from enable(fwk) to bar(md)
    for (int i = 1; i < 6; i++) {
        (*p)[i] = (*p)[i] == 0 ? 1: 0;
    }

    err = asprintf(&cmd, "AT+EROAMBAR=%d,%d,%d,%d,%d"
        , (*p)[3]  // BAR_Dom_Voice_Roaming_Enabled
        , (*p)[4]  // BAR_Dom_Data_Roaming_Enabled
        , (*p)[1]  // Bar_Int_Voice_Roaming_Enabled
        , (*p)[2]  // Bar_Int_Data_Roaming_Enabled
        , (*p)[5]);  // Bar_LTE_Data_Roaming_Enabled
    if (err < 0) {
        logE(LOG_TAG, "setRoamingEnable::using asprintf and getting ERROR");
        goto error;
    }
    logD(LOG_TAG, "setRoamingEnable %s", cmd);
    p_response = atSendCommand(cmd);
    if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
        ril_errno = RIL_E_SUCCESS;
    }
    free(cmd);
error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::getRoamingEnable(const sp<RfxMclMessage>& msg) {
    /* +EROAMBAR:<protocol_index>, (not ready now)
        <BAR_Dom_Voice_Roaming_Enabled>,
        <BAR_Dom_Data_Roaming_Enabled>,
        <Bar_Int_Voice_Roaming_Enabled>,
        <Bar_Int_Data_Roaming_Enabled>,
        <Bar_LTE_Data_Roaming_Enabled>
     Expected Result:
     response[0]: phone id (0,1,2,3,...)
     response[1] : international_voice_text_roaming (0,1)
     response[2] : international_data_roaming (0,1)
     response[3] : domestic_voice_text_roaming (0,1)
     response[4] : domestic_data_roaming (0,1)
     response[5] : domestic_LTE_data_roaming (1) */
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int roaming[6] = {1, 0, 1, 1, 1, 1};  // default value
    int err = 0;

    p_response = atSendCommandSingleline("AT+EROAMBAR?", "+EROAMBAR:");

    // check error
    err = p_response->getError();
    if (err != 0 ||
          p_response == NULL ||
          p_response->getSuccess() == 0 ||
          p_response->getIntermediates() == NULL)
        goto error;

    // handle intermediate
    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    /* //DS
     roaming[0] = line->atTokNextint(&err);
     if (err < 0) goto error; */

    roaming[0] = 0;

    // <BAR_Dom_Voice_Roaming_Enabled>
    roaming[3] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // <BAR_Dom_Data_Roaming_Enabled>
    roaming[4] = line->atTokNextint(&err);
    if (err < 0) goto error;


    // <Bar_Int_Voice_Roaming_Enabled>
    roaming[1] = line->atTokNextint(&err);
    if (err < 0) goto error;


    // <Bar_Int_Data_Roaming_Enabled>
    roaming[2] = line->atTokNextint(&err);
    if (err < 0) goto error;


    // <Bar_LTE_Data_Roaming_Enabled>
    roaming[5] = line->atTokNextint(&err);
    if (err < 0) goto error;


    // rever the setting from enable(fwk) to bar(md)
    for (int i = 1; i < 6; i++) {
        roaming[i] = roaming[i] == 0 ? 1:0;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(roaming, 6), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;
    error:
    logE(LOG_TAG, "getRoamingEnable must never return error when radio is on");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestGetPseudoCellInfo(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    int err;
    int apc_mode;
    int urc_enable;
    int timer;
    int num;

    // <apc_mode>[<urc_enable><time>
    // <num>[<type><plmn><lac><cid><arfcn><bsic>[<type><plmn><lac><cid><arfcn><bsic>]]]
    // num: 0 or 1 or 2
    int response[16] = {0};

    p_response = atSendCommandSingleline("AT+EAPC?", "+EAPC:");
    if (p_response->getError() < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    /*  response:
     *    +EAPC:<apc_mode>[,<urc_enable>,<time>,<count>
     *      [,<type>,<plmn>,<lac>,<cid>,<arfcn>,<bsic>[,<type>,<plmn>,<lac>,<cid>,<arfcn>,<bsic>]]]
     */
    line = p_response->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) goto error;

    apc_mode = line->atTokNextint(&err);
    if (err < 0) goto error;
    response[0] = apc_mode;

    if (line->atTokHasmore()) {
        urc_enable = line->atTokNextint(&err);
        if (err < 0) goto error;
        response[1] = urc_enable;

        timer = line->atTokNextint(&err);
        if (err < 0) goto error;
        response[2] = timer;

        num = line->atTokNextint(&err);
        if (err < 0) goto error;
        response[3] = num;

        for (int i = 0; i < num; i++) {
            response[i*6 + 4] = line->atTokNextint(&err);
            if (err < 0) goto error;

            if (line->atTokHasmore()) {
                response[i*6 + 5] = line->atTokNextint(&err);
                if (err < 0) goto error;

                response[i*6 + 6] = line->atTokNextint(&err);
                if (err < 0) goto error;

                response[i*6 + 7] = line->atTokNextint(&err);
                if (err < 0) goto error;

                response[i*6 + 8] = line->atTokNextint(&err);
                if (err < 0) goto error;

                response[i*6 + 9] = line->atTokNextint(&err);
                if (err < 0) goto error;
            }
        }
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(response, 16), msg, false);
    responseToTelCore(resp);
    return;

error:
    logE(LOG_TAG, "requestGetPseudoCellInfo failed err=%d", p_response->getError());
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestSetLteReleaseVersion(const sp<RfxMclMessage>& msg) {
    // +ECASW=<mode>
    // <mode>: integer type. Setting mode.
    // <mode>=0 turn off LTE Carrier Aggregation
    // <mode>=1 turn on LTE Carrier Aggregation

    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    int *pInt = (int *) msg->getData()->getData();
    int mode = pInt[0];

    if (mode < 0) {
        ril_errno = RIL_E_INVALID_ARGUMENTS;
    } else {
        // send AT command
        sp<RfxAtResponse> p_response = atSendCommand(String8::format("AT+ECASW=%d", mode));
        if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
            ril_errno = RIL_E_SUCCESS;
        }
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestGetLteReleaseVersion(const sp<RfxMclMessage>& msg) {
    int err, mode = 0;
    RfxAtLine* line;
    sp<RfxMclMessage> response;

    sp<RfxAtResponse> p_response = atSendCommandSingleline("AT+ECASW?", "+ECASW:");

    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // get <mode>
    mode = line->atTokNextint(&err);
    if (err < 0) goto error;

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::fillCidToPhysicalChannelConfig(RIL_PhysicalChannelConfig* pcc) {
    pcc->num_cids = 0;
    for (int i = 0; i < 16; i++) {
        if (data_context_ids[m_slot_id][i] > 0) {
            pcc->num_cids += 1;
            pcc->contextIds[i] = data_context_ids[m_slot_id][i];
        }
    }
}

void RmcNetworkRequestHandler::updatePhyChlCfgUrcByQuery() {
    logV(LOG_TAG, "updatePhyChlCfgUrcByQuery +++");
    if (mPhyChlCfgMode == 2) {
        int ret = false;
        int num = 0;
        RIL_PhysicalChannelConfig configs[MAX_PHY_CHL_CFG_COUNT];
        memset(configs, 0, MAX_PHY_CHL_CFG_COUNT*sizeof(RIL_PhysicalChannelConfig));
        RIL_PhysicalChannelConfig* pCache = sPhysicalConfigUrcCache[m_slot_id];
        ret = getPhyChlCfgInfoV2ByQuery(RADIO_TECH_UNKNOWN, configs, &num);
        if (ret) {
            pthread_mutex_lock(&sUrcPhysicalConfigMutex[m_slot_id]);
            sPhysicalConfigUrcCount[m_slot_id] = num;
            if (num > 0) {
                memcpy(pCache, configs, num*sizeof(RIL_PhysicalChannelConfig));
            }
            pthread_mutex_unlock(&sUrcPhysicalConfigMutex[m_slot_id]);
        }
    }
    currentPhysicalChannelConfigs(false);
}

void RmcNetworkRequestHandler::currentPhysicalChannelConfigs(bool forceQuery) {
    if (!physicalConfigSwitch[m_slot_id]) return;  // It's off.

    //No need to update in below situation:
    //1) ps oos. 2)NR idle state. 3)LTE idle state.
    bool needNotifyEmpty = needReportPccEmptyList();
    pthread_mutex_lock(&sPhysicalConfigMutex[m_slot_id]);
    int notifiedCount = sPhyChlCfgNotifiedCount[m_slot_id];
    pthread_mutex_unlock(&sPhysicalConfigMutex[m_slot_id]);
    if (needNotifyEmpty && (notifiedCount == 0)) {
        logD(LOG_TAG, "[%s] needNotifyEmpty and has notified empty list, return", __FUNCTION__);
        return;
    }

    int num = 0;
    RIL_PhysicalChannelConfig rilPhysicalconfig[MAX_PHY_CHL_CFG_COUNT];
    memset(rilPhysicalconfig, 0, MAX_PHY_CHL_CFG_COUNT*sizeof(RIL_PhysicalChannelConfig));

    if (!needNotifyEmpty) {
        RIL_RadioTechnology curRat = (RIL_RadioTechnology)convertPSNetworkType(urc_data_reg_state_cache[m_slot_id]->radio_technology);
        if (mPhyChlCfgMode == 2) {
            int ret = false;
            if (forceQuery) {
                ret = getPhyChlCfgInfoV2ByQuery(curRat, rilPhysicalconfig, &num);
                if (!ret) {
                    logE(LOG_TAG, "[%s] getPhyChlCfgInfoV2ByQuery error return false", __FUNCTION__);
                    return;
                }
            } else {
                ret = getPhyChlCfgInfoV2ByUrc(curRat, rilPhysicalconfig, &num);
                if (!ret) {
                    logE(LOG_TAG, "[%s] getPhyChlCfgInfoV2ByUrc error return false", __FUNCTION__);
                    return;
                }
            }
        } else {
            int ret = getPhyChlCfgInfoV1(curRat, rilPhysicalconfig, &num);
            if (!ret) {
                logE(LOG_TAG, "[%s] getPhyChlCfgInfoV1 error return false", __FUNCTION__);
                return;
            }
        }
    }

    RIL_PhysicalChannelConfig* pCache = sPhyChlCfgNotifiedCache[m_slot_id];
    bool hasChanged = false;
    pthread_mutex_lock(&sPhysicalConfigMutex[m_slot_id]);
    if (sPhyChlCfgNotifiedCount[m_slot_id] != num) {
        hasChanged = true;
    } else if (sPhyChlCfgNotifiedCount[m_slot_id] > 0 && num > 0) {
        for (int i = 0; i < sPhyChlCfgNotifiedCount[m_slot_id]; i++) {
            if (pCache[i].rat != rilPhysicalconfig[i].rat
                || pCache[i].status != rilPhysicalconfig[i].status
                || pCache[i].cellBandwidthDownlink != rilPhysicalconfig[i].cellBandwidthDownlink
                || pCache[i].cellBandwidthUplink != rilPhysicalconfig[i].cellBandwidthUplink
                || pCache[i].downlinkChannelNumber != rilPhysicalconfig[i].downlinkChannelNumber
                || pCache[i].uplinkChannelNumber != rilPhysicalconfig[i].uplinkChannelNumber
                || pCache[i].band != rilPhysicalconfig[i].band
                || pCache[i].physicalCellId != rilPhysicalconfig[i].physicalCellId) {
                hasChanged = true;
                break;
            }
        }
    }

    if (!hasChanged && sPhyChlCfgNotifiedCount[m_slot_id] > 0 && num > 0) {
        if (pCache[0].num_cids == rilPhysicalconfig[0].num_cids) {
            for (int i = 0; i < 16; i++) {
                if (pCache[0].contextIds[i] != rilPhysicalconfig[0].contextIds[i]) {
                    hasChanged = true;
                    break;
                }
            }
        } else {
            hasChanged = true;
        }
    }

    // only send URC when it's changed.
    if (hasChanged) {
        int physicalConfig_has_nr = 0;
        for (int i = 0; i < num; i++) {
            if (rilPhysicalconfig[i].rat == RADIO_TECH_NR) physicalConfig_has_nr = 1;
        }
        last_physical_config_has_nr[m_slot_id] = physicalConfig_has_nr;

        memcpy(sPhyChlCfgNotifiedCache[m_slot_id], rilPhysicalconfig, num*sizeof(RIL_PhysicalChannelConfig));
        for (int i = num; i < sPhyChlCfgNotifiedCount[m_slot_id]; i++) {
            memset(&pCache[i], 0, sizeof(RIL_PhysicalChannelConfig));
        }
        sPhyChlCfgNotifiedCount[m_slot_id] = num;

        // if count = 0, send an empty one to clear
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_PHYSICAL_CHANNEL_CONFIGS_MTK,
                m_slot_id,
                RfxIntsData((void*)rilPhysicalconfig, num*sizeof(RIL_PhysicalChannelConfig)));
        responseToTelCore(urc);
    }
    pthread_mutex_unlock(&sPhysicalConfigMutex[m_slot_id]);

    logD(LOG_TAG, "[%s] hasChanged=%d, num=%d, mPhyChlCfgMode=%d, forceQuery:%d,"
        " last_physical_config_has_nr=%d, physicalConfig_has_nr=%d", __FUNCTION__,
        (hasChanged? 1: 0), num, mPhyChlCfgMode, forceQuery,
        last_physical_config_has_nr[m_slot_id], last_physical_config_has_nr[m_slot_id]);
    return;
}

bool RmcNetworkRequestHandler::getPhyChlCfgInfoV1(RIL_RadioTechnology curRat,
        RIL_PhysicalChannelConfig * pInfo, int* count) {
    int num = 0;
    MD_ECELL *mMdEcell = mdEcell[m_slot_id];
    pthread_mutex_lock(&scg_connection_state_Mutex[m_slot_id]);
    int t_scg_connection_state = scg_connection_state[m_slot_id];
    pthread_mutex_unlock(&scg_connection_state_Mutex[m_slot_id]);

    if (curRat == 20) {
        //SA
        pInfo[num].rat = RADIO_TECH_NR;
        pInfo[num].status = PRIMARY_SERVING;
        pInfo[num].cellBandwidthDownlink = 20000;
        if (mMdEcell->pscPci >= 0 && mMdEcell->pscPci <= 1007) {
            pInfo[num].physicalCellId = mMdEcell->pscPci;
        } else {
            logE(LOG_TAG, "[%s]:invalid NR physicalCellId, set to 0.", __FUNCTION__);
            pInfo[num].physicalCellId = 0;
        }

        if (nrBand[m_slot_id] > 0) {
            pInfo[num].band = nrBand[m_slot_id];
        } else {
            logE(LOG_TAG, "[%s]:invalid nr band:%d, force set to 1.", __FUNCTION__, nrBand[m_slot_id]);
            pInfo[num].band = 1;
        }
        num++;

        //SA CA
        pthread_mutex_lock(&s_nrCaMutex[m_slot_id]);
        if (!nr_ca_cache[m_slot_id]->is_endc && nr_ca_cache[m_slot_id]->band_2 > 0) {
            pInfo[num].rat = RADIO_TECH_NR;
            pInfo[num].status = SECONDARY_SERVING;
            pInfo[num].cellBandwidthDownlink = 20000;
            pInfo[num].physicalCellId = 0;
            pInfo[num].band = nr_ca_cache[m_slot_id]->band_2;
            num ++;
        }
        pthread_mutex_unlock(&s_nrCaMutex[m_slot_id]);
    } else if (curRat == 14 || curRat == 19) {
        pthread_mutex_lock(&s_caMutex[m_slot_id]);
        int ca_pcell_bw = ca_cache[m_slot_id]->pcell_bw;
        int ca_scell_bw = ca_cache[m_slot_id]->scell_bw1;

        // LTE or LTE_CA
        if (ca_pcell_bw > 0) {
            // unit of ca_cache[m_slot_id]->pcell_bw is 0.1MHz
            // 1MHz = 1000 KHz
            pInfo[num].rat = RADIO_TECH_LTE;
            pInfo[num].status = PRIMARY_SERVING;
            pInfo[num].cellBandwidthDownlink = 100 * ca_pcell_bw;
            if (mMdEcell->pscPci >= 0 && mMdEcell->pscPci <= 503) {
                pInfo[num].physicalCellId = mMdEcell->pscPci;
            } else {
                logE(LOG_TAG, "[%s]:invalid LTE physicalCellId, set to 0.", __FUNCTION__);
                pInfo[num].physicalCellId = 0;
            }
            if (lteBand[m_slot_id] > 0) {
                pInfo[num].band = lteBand[m_slot_id];
            } else {
                logE(LOG_TAG, "[%s]:invalid lte band:%d, force set to 1.", __FUNCTION__, lteBand[m_slot_id]);
                pInfo[num].band = 1;
            }
            num++;
        }

        if (ca_scell_bw > 0) {
            pInfo[num].rat = RADIO_TECH_LTE;
            pInfo[num].status = SECONDARY_SERVING;
            pInfo[num].cellBandwidthDownlink = 100 * ca_scell_bw;
            pInfo[num].physicalCellId = 0;
            if (lteBand[m_slot_id] > 0) {
                pInfo[num].band = lteBand[m_slot_id];
            } else {
                logE(LOG_TAG, "[%s]:invalid lte band:%d, force set to 1.",
                        __FUNCTION__, lteBand[m_slot_id]);
                pInfo[num].band = 1;
            }
            num++;
        }
        pthread_mutex_unlock(&s_caMutex[m_slot_id]);

        pthread_mutex_lock(&s_nrCaMutex[m_slot_id]);
        int is_endc = nr_ca_cache[m_slot_id]->is_endc;
        int nr_band_1 = nr_ca_cache[m_slot_id]->band_1;
        int nr_band_2 = nr_ca_cache[m_slot_id]->band_2;
        // scg case
        if (t_scg_connection_state == 1) { // nsa is connecting
            pInfo[num].rat = RADIO_TECH_NR;
            pInfo[num].status = SECONDARY_SERVING;
            pInfo[num].cellBandwidthDownlink = 20000;
            pInfo[num].physicalCellId = 0;
            if (nr_band_1 > 0) {
                pInfo[num].band = nr_band_1;
            } else {
                logE(LOG_TAG, "[%s]:invalid lte band:%d, force set to 1.",
                        __FUNCTION__, nr_band_1);
                pInfo[num].band = 1;
            }
            num ++;

            if (is_endc == 1 && nr_band_2 > 0) {
                pInfo[num].rat = RADIO_TECH_NR;
                pInfo[num].status = SECONDARY_SERVING;
                pInfo[num].cellBandwidthDownlink = 20000;
                pInfo[num].physicalCellId = 0;
                pInfo[num].band = nr_band_2;
                num++;
            }
        }
        pthread_mutex_unlock(&s_nrCaMutex[m_slot_id]);
    } else if (curRat == 1 || curRat == 2) {
        // GPRS or EDGE
        pInfo[num].rat = RADIO_TECH_GSM;
        pInfo[num].status = PRIMARY_SERVING;
        pInfo[num].cellBandwidthDownlink = 200;
        if (mMdEcell->pscPci > 0) {
            pInfo[num].physicalCellId = mMdEcell->pscPci;
        } else {
            logE(LOG_TAG, "[%s]:invalid GSM physicalCellId, set to 0.", __FUNCTION__);
            pInfo[num].physicalCellId = 0;
        }

        pInfo[num].band = 1;
        num++;
    } else if (curRat == 6 || curRat == 8 || curRat == 13) {
        // C2K
    } else {
        // else 3G cases
        pInfo[num].rat = RADIO_TECH_UMTS;
        pInfo[num].status = PRIMARY_SERVING;
        pInfo[num].cellBandwidthDownlink = 5000;
        if (mMdEcell->pscPci >= 0 && mMdEcell->pscPci <= 511) {
            pInfo[num].physicalCellId = mMdEcell->pscPci;
        } else {
            logE(LOG_TAG, "[%s]:invalid UTRAN physicalCellId, set to 0.", __FUNCTION__);
            pInfo[num].physicalCellId = 0;
        }
        pInfo[num].band = 1;
        num++;

        if (m_dc_support[m_slot_id]) {
            pInfo[num].rat = RADIO_TECH_UMTS;
            pInfo[num].status = SECONDARY_SERVING;
            pInfo[num].cellBandwidthDownlink = 5000;
            pInfo[num].physicalCellId = 0;
            pInfo[num].band = 1;
            num++;
        }
    }

    for (int i = 0; i < num; i++) {
        pInfo[i].cellBandwidthUplink = INT_MAX;
        pInfo[i].downlinkChannelNumber = INT_MAX;
        pInfo[i].uplinkChannelNumber = INT_MAX;
        fillCidToPhysicalChannelConfig(&pInfo[i]);
    }

    *count = num;
    logV(LOG_TAG, "[%s]:count %d, curRat %d, scg %d ", __FUNCTION__, *count, curRat, t_scg_connection_state);
    return true;
}

bool RmcNetworkRequestHandler::getPhyChlCfgInfoV2ByQuery(RIL_RadioTechnology curRat,
        RIL_PhysicalChannelConfig * pInfo, int* count) {
    /*+EPHYCONFIG=2
        +EPHYCONFIG: <rat>, <dl_ch_num>, <ul_ch_num>, <dl_cell_bw>, <ul_cell_bw>,<band>, <pci>,<pcell or scell>
        ...list
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline("AT+EPHYCONFIG=2", "+EPHYCONFIG:");
    // check error
    int err = p_response->getError();
    if (err != 0 || p_response == NULL || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "[%s]:AT+EPHYCONFIG=2 error return false", __FUNCTION__);
        return false;
    }

    RfxAtLine *line = p_response->getIntermediates();
    int ret = false;
    int num = 0;
    for (; line != NULL; line = line->getNext()) {
        ret = parsePhyChlCfg(curRat, line, &pInfo[num]);
        if (!ret) {
            continue;
        }
        fillCidToPhysicalChannelConfig(&pInfo[num]);
        num++;
    }

    *count = num;
    logV(LOG_TAG, "[%s]:count %d,  curRat %d", __FUNCTION__, *count, curRat);
    return ret;
}

bool RmcNetworkRequestHandler::getPhyChlCfgInfoV2ByUrc(RIL_RadioTechnology curRat,
        RIL_PhysicalChannelConfig * pInfo, int* count) {
    pthread_mutex_lock(&scg_connection_state_Mutex[m_slot_id]);
    int t_scg_connection_state = scg_connection_state[m_slot_id];
    pthread_mutex_unlock(&scg_connection_state_Mutex[m_slot_id]);

    pthread_mutex_lock(&sUrcPhysicalConfigMutex[m_slot_id]);
    RIL_PhysicalChannelConfig* pUrcCache = sPhysicalConfigUrcCache[m_slot_id];
    int num = 0;

    //fill data with urc cache
    for (int i = 0; i < sPhysicalConfigUrcCount[m_slot_id]; i++) {
        if (pUrcCache[i].rat == RADIO_TECH_NR) {
            if (curRat == RADIO_TECH_NR) {
                memcpy(&pInfo[num], &pUrcCache[i], sizeof(RIL_PhysicalChannelConfig));
                num++;
            } else if ((curRat == RADIO_TECH_LTE || curRat == RADIO_TECH_LTE_CA)
                    && t_scg_connection_state == 1) {
                memcpy(&pInfo[num], &pUrcCache[i], sizeof(RIL_PhysicalChannelConfig));
                pInfo[num].status = SECONDARY_SERVING;
                num++;
            } else {
                logV(LOG_TAG, "[%s]: index %d of urc cache skip, pUrcCache[i].rat %d not match curRat %d"
                        " t_scg_connection_state %d",
                        __FUNCTION__, i,pUrcCache[i].rat, curRat, t_scg_connection_state);
            }
        } else if (((pUrcCache[i].rat == RADIO_TECH_LTE)
                    && (curRat == RADIO_TECH_LTE || curRat == RADIO_TECH_LTE_CA))
            || ((pUrcCache[i].rat == RADIO_TECH_UMTS)
                    && (curRat == 3 || curRat == 9 || curRat == 10
                        || curRat == 11 || curRat == 15))
            || ((pUrcCache[i].rat == RADIO_TECH_GSM)
                    && (curRat == 1 || curRat == 2))) {
            memcpy(&pInfo[num], &pUrcCache[i], sizeof(RIL_PhysicalChannelConfig));
            num++;
        } else {
            logV(LOG_TAG, "[%s]: index %d of urc cache skip, pUrcCache[i].rat %d not match curRat %d",
                    __FUNCTION__, i,pUrcCache[i].rat, curRat);
        }
    }
    pthread_mutex_unlock(&sUrcPhysicalConfigMutex[m_slot_id]);

    for (int i = 0; i < num; i++) {
        fillCidToPhysicalChannelConfig(&pInfo[i]);
    }

    *count = num;
    logV(LOG_TAG, "[%s]:get %d data from URC, sPhysicalConfigUrcCount[m_slot_id] %d, curRat %d, scg %d ",
            __FUNCTION__, *count,
            sPhysicalConfigUrcCount[m_slot_id], curRat, t_scg_connection_state);
    return true;
}

int RmcNetworkRequestHandler::getPhyChlCfgMode() {
    //get physical channel interface version of md
    //mode:1, all info need nw fill in
    //mode:2, md reprot both pcell and scell info to NW by at command
    int mode;
    int err;
    /*+EPHYCONFIG=2
        +EPHYCONFIG: <rat>, <dl_ch_num>, <ul_ch_num>, <dl_cell_bw>, <ul_cell_bw>,<band>, <pci>,<pcell or scell>
        ...list
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline("AT+EPHYCONFIG=2", "+EPHYCONFIG:");
    // check error
    err = p_response->getError();
    if (err != 0 || p_response->getSuccess() == 0) {
        logV(LOG_TAG, "[%s] AT+EPHYCONFIG=2 not support, all info need nw fill in", __FUNCTION__);
        mode = 1;
    } else {
        mode = 2;
        logV(LOG_TAG, "[%s] AT+EPHYCONFIG=2 support, mode = 2 get pcell/scell data from MD", __FUNCTION__);
    }
    return mode;
}

bool RmcNetworkRequestHandler::needReportPccEmptyList() {
    bool needNotify = false;
    pthread_mutex_lock(&nr_connection_state_Mutex[m_slot_id]);
    int t_nr_connection_state = nr_connection_state[m_slot_id];
    pthread_mutex_unlock(&nr_connection_state_Mutex[m_slot_id]);
    int regState = data_reg_state_cache[m_slot_id]->registration_state;
    int radioTec = data_reg_state_cache[m_slot_id]->radio_technology;

    if (isInService(regState)) {
        if (radioTec == 20) {
            //SA
            if (t_nr_connection_state == 1) {
                needNotify = true;
            } else {
                logV(LOG_TAG, "[%s] sa idel state", __FUNCTION__);
            }
        } else if (radioTec == 14 || radioTec == 19) {
            // LTE or LTE_CA
            pthread_mutex_lock(&lte_connection_state_Mutex[m_slot_id]);
            bool idle_state = (lte_tdd_connection_state[m_slot_id] == 0 &&
                    lte_fdd_connection_state[m_slot_id] == 0) ? true : false;
            pthread_mutex_unlock(&lte_connection_state_Mutex[m_slot_id]);
            if (!idle_state) {
                needNotify = true;
            } else {
                logV(LOG_TAG, "[%s] lte idle state", __FUNCTION__);
            }
        } else if (radioTec == 1 || radioTec == 2) {
            // GPRS or EDGE
            needNotify = true;
        } else if (radioTec == 6 || radioTec == 8 || radioTec == 13) {
            // C2K
        } else {
            // else 3G cases
            needNotify = true;
        }
    } else {
        logV(LOG_TAG, "[%s] ps not in service", __FUNCTION__);
    }
    return !needNotify;
}

int RmcNetworkRequestHandler::setSignalStrengthReportingCriteriaSingle(RIL_SignalStrength_Reporting_Criteria * ssrc) {
    logV(LOG_TAG, "setSignalStrengthReportingCriteriaSingle "
            "signalMeasurement = %d, "
            "hysteresisMs = %d, "
            "hysteresisDb = %d, "
            "accessNetwork = %d, "
            "thresholdsDbm[0] = %d, "
            "thresholdsDbm[1] = %d, "
            "thresholdsDbm[2] = %d, "
            "thresholdsDbm[3] = %d, "
            "isEnabled = %d",
            ssrc->signalMeasurement,
            ssrc->hysteresisMs,
            ssrc->hysteresisDb,
            (int) ssrc->accessNetwork,
            ssrc->thresholdsDbm[0],
            ssrc->thresholdsDbm[1],
            ssrc->thresholdsDbm[2],
            ssrc->thresholdsDbm[3],
            ssrc->isEnabled);

    sp<RfxAtResponse> p_response;
    int rat = 0, signal_type = 0, err = 0;
    String8 command("AT+ECSQ=4");
    switch (ssrc->accessNetwork) {
    case GERAN:
        rat = 1;
        switch(ssrc->signalMeasurement) {
        case RSSI:
            signal_type = 0;
            break;
        default:
            goto error;
            break;
        }
        break;
    case UTRAN:
        rat = 2;
        switch(ssrc->signalMeasurement) {
        case RSCP:
            signal_type = 0;
            break;
        case ECNO:
            signal_type = 1;
            break;
        default:
            goto error;
            break;
        }
        break;
    case EUTRAN:
        rat = 4;
        switch(ssrc->signalMeasurement) {
        case RSRP:
            signal_type = 0;
            break;
        case RSRQ:
            signal_type = 1;
            break;
        case RSSNR:
            signal_type = 2;
            break;
        default:
            goto error;
            break;
        }
        break;
    case CDMA2000:
        rat = 16;
        switch(ssrc->signalMeasurement) {
        case RSSI:
            signal_type = 0;
            break;
        default:
            goto error;
            break;
        }
        break;
    case NEW_RADIO:
        rat = 128;
        switch(ssrc->signalMeasurement) {
        case SSRSRP:
            signal_type = 0;
            break;
        case SSRSRQ:
            signal_type = 1;
            break;
        case SSSINR:
            signal_type = 2;
            break;
        default:
            goto error;
            break;
        }
        break;
    default:
        goto error;
        break;
    }

    /* AT+ECSQ=4, <mode>,<rat>,<signal_type>,<HysterisisMS>,<HysterisisDB>,<Num_of_thresholds>,[<Thresholds>] */
    command.append(String8::format(", %d, %d, %d, %d, %d, %d", ssrc->isEnabled, rat, signal_type,
        ssrc->hysteresisMs, ssrc->hysteresisDb, ssrc->thresholdsCount));
    for (int i = 0; i < ssrc->thresholdsCount; i++) {
        command.append(String8::format(", %d", ssrc->thresholdsDbm[i]));
    }
    p_response = atSendCommand(command);
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        // Older modems don't implement it, however AP shall return success always
        // ril_err = RIL_E_GENERIC_FAILURE;
    }
error:
    return 1;
}

void RmcNetworkRequestHandler::requestSetSignalStrengthReportingCriteria
        (const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    RIL_SignalStrength_Reporting_Criteria * data = (RIL_SignalStrength_Reporting_Criteria*) msg->getData()->getData();
    int size = msg->getData()->getDataLength() / sizeof(RIL_SignalStrength_Reporting_Criteria);
    for (int index = 0; index < size; index ++) {
        RIL_SignalStrength_Reporting_Criteria* ssrc = data + index;
        setSignalStrengthReportingCriteriaSingle(ssrc);
    }
    //// Older modems don't implement it, however AP shall return success always
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
    responseToTelCore(resp);
    return;
}

void RmcNetworkRequestHandler::requestSetSystemSelectionChannels
        (const sp<RfxMclMessage>& msg) {
    // +EMMCHLCK=<mode>,[<act>,<band indicator>,[<number of channel>,]<arfcn>,[<arfcn>,<arfcn>,...,<arfcn>],<cell id>]
    sp<RfxMclMessage> resp;
    RIL_Errno ril_err = RIL_E_RADIO_NOT_AVAILABLE;
    RIL_SystemSelectionChannels* ssc =
            (RIL_SystemSelectionChannels*) msg->getData()->getData();
    sp<RfxAtResponse> p_response;
    int err;
    String8 log("requestSetSystemSelectionChannels ");
    log.append(String8::format(",specifyChannels=%d", ssc->specifyChannels));
    RIL_GeranBands gband;
    RIL_UtranBands uBand;
    RIL_EutranBands eBand;
    RIL_NgranBands nBand;

    if (ssc->specifyChannels > 0) {
        if (!isNrSupported()) goto error;
        // config
        log.append(String8::format(",specifiers_length=%d", ssc->specifiers_length));
        for (int i = 0; i < ssc->specifiers_length; i++) {
            int bands[10] = {0};
            int index = 0;
            String8 cmd("AT+EMMCHLCK=2");
            log.append(String8::format(",specifiers[%d].radio_access_network=%d", i, ssc->specifiers[i].radio_access_network));
            if (ssc->specifiers[i].radio_access_network != EUTRAN && ssc->specifiers[i].radio_access_network != NEW_RADIO) {
                logE(LOG_TAG, "ONLY support LTE/NR on Gen97");
                goto error;
            }
            // <act>
            switch (ssc->specifiers[i].radio_access_network) {
                case GERAN:
                    cmd.append(",0");
                    break;
                case UTRAN:
                    cmd.append(",2");
                    break;
                case EUTRAN:
                    cmd.append(",7");
                    break;
                case NEW_RADIO:
                    cmd.append(",11");
                    break;
                default:
                    goto error;
            }
            log.append(String8::format(",specifiers[%d].bands_length=%d", i, ssc->specifiers[i].bands_length));
            // <band indicator>
            for (int j = 0; j < ssc->specifiers[i].bands_length; j++) {
                switch (ssc->specifiers[i].radio_access_network) {
                    case GERAN:
                        gband = ssc->specifiers[i].bands.geran_bands[j];
                        if (gband > 0 && gband <= GERAN_BAND_MAX) {
                            index = (gband - 1) / 32;
                            bands[index] |= (1 << ((gband - 1) % 32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, gband));
                        break;
                    case UTRAN:
                        uBand = ssc->specifiers[i].bands.utran_bands[j];
                        if (uBand > 0 && uBand <= UTRAN_BAND_MAX) {
                            index = (uBand - 1) / 32;
                            bands[index] |= (1 << ((uBand - 1) % 32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, uBand));
                        break;
                    case EUTRAN:
                        eBand = ssc->specifiers[i].bands.eutran_bands[j];
                        if (eBand > 0 && eBand <= EUTRAN_BAND_MAX) {
                            index = (eBand - 1) / 32;
                            bands[index] |= (1 << ((eBand - 1) % 32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, eBand));
                        break;
                    case NEW_RADIO:
                        nBand = ssc->specifiers[i].bands.ngranBands[j];
                        if (nBand > 0 && nBand <= NGRAN_BAND_MAX) {
                            index = (nBand - 1) / 32;
                            bands[index] |= (1 << ((nBand - 1) % 32));
                        }
                        log.append(String8::format(",specifiers[%d].bands[%d]=%d", i, j, nBand));
                        break;
                    default:
                        goto error;
                }
            }
            if (ssc->specifiers[i].radio_access_network == GERAN ||
                    ssc->specifiers[i].radio_access_network == UTRAN) {
                cmd.append(String8::format(",%08X", bands[0]));
            } else {
                for (int j = 0; j < 3; j++) {
                    if (j == 0) {
                        cmd.append(String8::format(",%08X", bands[j]));
                    } else {
                        cmd.append(String8::format("%08X", bands[j]));
                    }
                }
            }
            log.append(String8::format(",specifiers[%d].channels_length=%d", i, ssc->specifiers[i].channels_length));
            // <number of channel>
            cmd.append(String8::format(",%d", ssc->specifiers[i].channels_length));
            // <arfcn>
            for (int j = 0; j < ssc->specifiers[i].channels_length; j++) {
                log.append(String8::format(",specifiers[%d].channels[%d]=%d", i, j, ssc->specifiers[i].channels[j]));
                cmd.append(String8::format(",%d", ssc->specifiers[i].channels[j]));
            }
            p_response = atSendCommand(cmd.string());
            err = p_response->getError();
            if (err < 0 || p_response->getSuccess() == 0) {
                logE(LOG_TAG, "requestSetSystemSelectionChannels config fail");
                goto error;
            }
        }
    } else {
        if (isNrSupported()) {
            p_response = atSendCommand("AT+EMMCHLCK=0");
            err = p_response->getError();
            if (err < 0 || p_response->getSuccess() == 0) {
                logE(LOG_TAG, "requestSetSystemSelectionChannels config fail");
                goto error;
            }
            p_response = atSendCommand("AT+EPBSEH=,,\"FFFFFFFF\",\"FFFFFFFFFFFFFFFF\"");
            err = p_response->getError();
            if (err < 0 || p_response->getSuccess() == 0) {
                logE(LOG_TAG, "requestSetSystemSelectionChannels config fail");
                goto error;
            }
        }
    }
    logD(LOG_TAG, "%s", log.string());

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg, false);
    responseToTelCore(resp);
    return;
error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_err,
                RfxVoidData(), msg, false);
    responseToTelCore(resp);

}

void RmcNetworkRequestHandler::requestGetSystemSelectionChannels(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RIL_SystemSelectionChannels* pData = (RIL_SystemSelectionChannels*) alloca(sizeof(RIL_SystemSelectionChannels));
    if (pData == NULL) goto error;
    memset(pData, 0, sizeof(RIL_SystemSelectionChannels));

error:
    /*not support*/
    if (pData) {
        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
        pData->specifyChannels = 0;
        pData->specifiers_length = 0;
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                RfxSscData(pData,sizeof(RIL_SystemSelectionChannels)), msg, false);
        responseToTelCore(response);
    }
}


void RmcNetworkRequestHandler::requestGetTs25Name(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    int *pInt = (int *) msg->getData()->getData();
    int mcc = pInt[0];
    int mnc = pInt[1];
    char *mccmnc = NULL;
    char longname[MAX_OPER_NAME_LENGTH], shortname[MAX_OPER_NAME_LENGTH];
    int err = 0;

    logD(LOG_TAG, "requestGetTs25Name input %d %d", mcc, mnc);
    if (mnc > 99) {err = asprintf(&mccmnc, "%d%03d", mcc, mnc);}
    else {err = asprintf(&mccmnc, "%d%02d", mcc, mnc);}
    if (err < 0) {
        logE(LOG_TAG, "requestGetTs25Name::using asprintf and getting ERROR");
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxStringData(), msg, false);
        responseToTelCore(response);
        return;
    }
    getPLMNNameFromNumeric(mccmnc, longname, shortname, MAX_OPER_NAME_LENGTH);
    free(mccmnc);
    logD(LOG_TAG, "requestGetTs25Name result %s %s", longname, shortname);
    // response long name only.
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringData(longname, (strlen(longname)+1)), msg, false);
    responseToTelCore(response);
    return;
}

bool RmcNetworkRequestHandler::handleGetDataContextIds(const sp<RfxMclMessage>& msg) {
    bool changed = false;
    int m_data_context_ids[16] = {};
    int *pInt = (int *)msg->getData()->getData();
    int size = msg->getData()->getDataLength() / sizeof(int);

    memcpy((void*) m_data_context_ids, (void*) data_context_ids[m_slot_id], 16 * sizeof(int));
    for (int i = 0; i < 16; i++) {
        if (i < size) {
            data_context_ids[m_slot_id][i] = pInt[i];
        } else {
            data_context_ids[m_slot_id][i] = -1;
        }
    }
    if (memcmp((void*) m_data_context_ids, (void*) data_context_ids[m_slot_id], 16 * sizeof(int)) != 0)
        changed = true;
    logD(LOG_TAG, "handleGetDataContextIds [%d, %d, %d, %d, ...]",
            data_context_ids[m_slot_id][0],
            data_context_ids[m_slot_id][1],
            data_context_ids[m_slot_id][2],
            data_context_ids[m_slot_id][3]);
    return changed;
}

/* MUSE WFC requirement
 * AT+EHOMEAS=<mode>,<signal_type>,<TIn>,<TOut>,<timer>[,<num_of_extended_threshold>[,<threshold1>...]
 * <enable>: enabled, 0 = disable; 1 = enabled
 * <type>: quality type
 * <threshold_array>: threshold value array
 */
void RmcNetworkRequestHandler::registerCellularQualityReport(const sp<RfxMclMessage>& msg) {
    char** params = (char**) msg->getData()->getData();
    char* atCmd = (char*)"AT+EHOMEAS";
    sp<RfxAtResponse> p_response;
    int err = 0;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> response;
    char theshold1[6];memset(theshold1, 0, sizeof(theshold1));
    char theshold2[6];memset(theshold2, 0, sizeof(theshold2));
    int thresholdCount = 1, offset = 0;
    char* pExtended_thresholds = NULL;
    char* pStart = params[2];
    char* pComma;
    String8 cmd;

    pComma = strchr(pStart, ',');
    if (pComma == NULL) {
        // only one threshold
        strncpy(theshold1, pStart, sizeof(theshold1)-1);
        strncpy(theshold2, pStart, sizeof(theshold2)-1);
    } else {
        offset = pComma - pStart;
        if (offset < 0) {
            logE(LOG_TAG, "%s, theshold1 offset is negative", __FUNCTION__);
            goto error;
        }
        if (offset > (sizeof(theshold1)-1)) {
            logE(LOG_TAG, "%s, theshold1 offset:%d is out", __FUNCTION__, offset);
            goto error;
        }
        memcpy(theshold1, pStart, offset);
        theshold1[offset] = 0;
        thresholdCount++;
        pStart = pComma + 1;

        pComma = strchr(pStart, ',');
        if (pComma == NULL) {
            // only two thresholds
            strncpy(theshold2, pStart, sizeof(theshold2)-1);
        } else {
            offset = pComma - pStart;
            if (offset < 0) {
                logE(LOG_TAG, "%s, theshold2 offset is negative", __FUNCTION__);
                goto error;
            }
            if (offset > (sizeof(theshold2)-1)) {
                logE(LOG_TAG, "%s, theshold2 offset:%d is out", __FUNCTION__, offset);
                goto error;
            }
            memcpy(theshold2, pStart, offset);
            theshold2[offset] = 0;
            thresholdCount++;
            pStart = pComma + 1;
            pExtended_thresholds = pStart;

            pComma = strchr(pStart, ',');
            while (pComma != NULL) {
                thresholdCount++;
                pStart = pComma + 1;
                pComma = strchr(pStart, ',');
            }
        }
    }

    if (thresholdCount <= 2) {
        cmd = String8::format("%s=%s,%s,%s,%s,%s", atCmd, params[0], params[1], theshold1,
                theshold2, params[3]);
    } else {
        logD(LOG_TAG, "registerCellularQualityReport %d, %s",thresholdCount, pExtended_thresholds);
        cmd = String8::format("%s=%s,%s,%s,%s,%s,%d,%s", atCmd, params[0], params[1], theshold1,
                theshold2, params[3], thresholdCount-2, pExtended_thresholds);
    }

    p_response = atSendCommand(cmd);
    err = p_response->getError();
    if (!(err < 0 || p_response->getSuccess() == 0)) {
        ril_errno = RIL_E_SUCCESS;
    } else {
        ril_errno = RIL_E_INVALID_STATE;
    }

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestGetSuggestedPlmnList(const sp<RfxMclMessage>& msg) {
    int err, len, list_size, num_filter, i;
    int *pInt = (int *)msg->getData()->getData();
    int rat = pInt[0];
    int num = pInt[1];
    int timer = pInt[2];
    char** response = NULL;
    sp<RfxMclMessage> resp;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;
    char *tmp;

    logV(LOG_TAG, "requestGetSuggestedPlmnList rat=%d, num=%d, timer=%d", rat, num, timer);

    p_response = atSendCommandSingleline(
        String8::format("AT+EPLWSS=%d,%d,%d", rat, num, timer), "+EPLWSS:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }

    line = p_response->getIntermediates();

    // count the number of operator
    tmp = line->getLine();
    len = strlen(tmp);
    for(i = 0, list_size = 0, num_filter = 0; i < len ; i++ ) {
        // here we assume that there is no nested ()
        if (tmp[i] == '(') {
            list_size++;
            num_filter++;
        } else if (tmp[i] == ',' && tmp[i+1] == ',') {
            break;
        }
    }

    response = (char**) calloc(1, sizeof(char*) * list_size);
    if (response == NULL) goto error;
    memset(response, 0, sizeof(char*) * list_size);
    // +EPLWSS: (2,"Far EasTone","FET","46601",0),(...),...,,(0, 1, 3),(0-2)
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    for (i = 0; i < list_size ; i++) {
        // <state>
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // get long name
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // get short name
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // get <oper> numeric code
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
        asprintf(&(response[i]), "%s", tmp);

        // get <lac> numeric code
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // get <AcT>
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData(response, list_size), msg, false);
    responseToTelCore(resp);

    if (response != NULL) {
        for (i = 0; i < list_size ; i++) {
            if (response[i] != NULL) free(response[i]);
        }
        free(response);
    }
    return;
error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
                RfxVoidData(), msg, false);
    responseToTelCore(resp);
    if (response != NULL) {
        for (i = 0; i < list_size ; i++) {
            if (response[i] != NULL) free(response[i]);
        }
        free(response);
    }
}

void RmcNetworkRequestHandler::smartRatSwitchBack() {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> urc;
    int err = 0;
    int cause = 2;
    p_response = atSendCommand(String8::format("AT+ETRAT=1"));
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "smartRatSwitchBack:: SA back fail err:%d", err);
    }
    p_response = atSendCommand(String8::format("AT+EGMC=1, \"endc_deactivation\",0,1"));
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "smartRatSwitchBack:: NSA back fail err:%d", err);
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SMART_RAT_SWITCH_INFO, m_slot_id,
            RfxIntsData(&cause, 1));
    responseToTelCore(urc);
}

void RmcNetworkRequestHandler::requestSmartRatSwitch(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, mode = 0, rat = 0, isAllow = 0;
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxMclMessage> response = NULL;
    RfxAtLine* line = NULL;
    int *pInt = (int *)msg->getData()->getData();

    mode = pInt[0]; // SMART_RAT_SWITCH_MODE
    rat = pInt[1]; // 0: 4G, 1: 5G
    mSwitchRat = rat;

    if (mode == RAT_SWITCH_MODE_NSA_SCG_ALLOW) {
        isAllow = 1;
    }

    if (mode == RAT_SWITCH_MODE_NONE ||
            mode == RAT_SWITCH_MODE_NSA_SCG ||
            mode == RAT_SWITCH_MODE_NSA_SCG_ALLOW) {  // NSA only
        // NSA
        p_response = atSendCommand(String8::format("AT+EGMC=1,\"endc_deactivation\",%d,%d", rat? 0:1, 1)); // 1: deactivation NR, 0: activation NR
        err = p_response->getError();
        if (err < 0 || p_response->getSuccess() == 0) {
            logE(LOG_TAG, "requestSmartRatSwitch fail");
            if (mode != 0) goto error;
        }
    } else if (mode == RAT_SWITCH_MODE_SA_ONLY) {  // SA(UE trigger leave)
        // SA
        p_response = atSendCommand(String8::format("AT+EGMC=1,\"nwsel_try_switch_rat\",%d", rat? 15:3)); //3: LTE, 15: NR
        err = p_response->getError();
        if (err < 0 || p_response->getSuccess() == 0) {
            logE(LOG_TAG, "requestSmartRatSwitch fail");
            goto error;
        }
    } else if (mode == RAT_SWITCH_MODE_BOTH) {  // SA(UE trigger leave)/NSA
        // NSA
        p_response = atSendCommand(String8::format("AT+EGMC=1,\"endc_deactivation\",%d,%d", rat? 0:1, 1)); // 1: deactivation NR, 0: activation NR
        err = p_response->getError();
        if (err < 0 || p_response->getSuccess() == 0) {
            logE(LOG_TAG, "requestSmartRatSwitch fail");
            goto error;
        }
        // SA
        p_response = atSendCommand(String8::format("AT+EGMC=1,\"nwsel_try_switch_rat\",%d", rat? 15:3)); //3: LTE, 15: NR
        err = p_response->getError();
        if (err < 0 || p_response->getSuccess() == 0) {
            logE(LOG_TAG, "requestSmartRatSwitch fail");
            goto error;
        }
    } else if (mode == RAT_SWITCH_MODE_NSA_TAU ||
            mode == RAT_SWITCH_MODE_SA_TAU ||
            mode == RAT_SWITCH_MODE_BOTH_TAU) {  // TAU
        int option = 0;

        p_response = atSendCommandSingleline("AT+E5GOPT?", "+E5GOPT:");
        err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            goto error;
        }

        // handle intermediate
        line = p_response->getIntermediates();
        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        option = line->atTokNextint(&err);
        if (err < 0) goto error;

        if (mode == RAT_SWITCH_MODE_NSA_TAU) {
            if (rat == 0) {
                option = option & 0xFB;
            } else {
                option = option | 0x04;
            }
        } else if (mode == RAT_SWITCH_MODE_SA_TAU) {
            if (rat == 0) {
                option = option & 0xFD;
            } else {
                option = option |0x02;
            }
        } else { // RAT_SWITCH_MODE_BOTH_TAU
            if (rat == 0) {
                option = 1;
            } else {
                option = 7;
            }
        }

        p_response = atSendCommand(String8::format("AT+E5GOPT=%d", option));
        err = p_response->getError();
        if (err < 0 || p_response->getSuccess() == 0) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            goto error;
        }
    } else {
        logE(LOG_TAG, "requestSmartRatSwitch error mode=%d", mode);
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestGetSmartRatSwitch(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, mode = 0, state = 0;
    char *tmp = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RfxAtLine* line = NULL;
    int *pInt = (int *)msg->getData()->getData();
    mode = pInt[0];

    if (mode == RAT_SWITCH_MODE_NSA_SCG) {
        p_response = atSendCommandSingleline(
                String8::format("AT+EGMC=0,\"endc_deactivation\""), "+EGMC:");
        err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            goto error;
        }
        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        // get <command>
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // get <deactivate>
        state = line->atTokNextint(&err);
        if (err < 0) goto error;
        state = state ? 0:1;
        logV(LOG_TAG, "RAT_SWITCH_MODE_NSA_SCG cmd=%s state=%d", tmp, state);
    } else if (mode == RAT_SWITCH_MODE_SA_ONLY) {  // SA(UE trigger leave)/NSA

        p_response = atSendCommandSingleline(
                String8::format("AT+EGMC=0,\"endc_deactivation\""), "+EGMC:");
        err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            goto error;
        }
        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        // get <command>
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        // get <deactivate>
        state = line->atTokNextint(&err);
        if (err < 0) goto error;

        state = state ==3 ? 0:1;
        logV(LOG_TAG, "RAT_SWITCH_MODE_SA_ONLY cmd=%s state=%d",
                tmp, state);
    } else if (mode == RAT_SWITCH_MODE_NSA_TAU ||
            mode == RAT_SWITCH_MODE_SA_TAU ||
            mode == RAT_SWITCH_MODE_BOTH_TAU) {  // TAU

        p_response = atSendCommandSingleline("AT+E5GOPT?", "+E5GOPT:");
        err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            goto error;
        }

        // handle intermediate
        line = p_response->getIntermediates();
        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        state = line->atTokNextint(&err);
        if (err < 0) goto error;

        if (mode == RAT_SWITCH_MODE_NSA_TAU) {
            state = state & 0x04 ? 1:0;
            logV(LOG_TAG, "RAT_SWITCH_MODE_NSA_TAU state=%d", state);
        } else if (mode == RAT_SWITCH_MODE_SA_TAU) {
            state = state & 0x02 ? 1:0;
            logV(LOG_TAG, "RAT_SWITCH_MODE_SA_TAU state=%d", state);
        } else { // RAT_SWITCH_MODE_BOTH_TAU
            state = state & 0x06;
            logV(LOG_TAG, "RAT_SWITCH_MODE_BOTH_TAU state=%d", state);
        }
    } else if (mode == RAT_SWITCH_MODE_RRC_STATE) {
        state = lte_fdd_connection_state[m_slot_id] | lte_tdd_connection_state[m_slot_id];
        logV(LOG_TAG, "RAT_SWITCH_MODE_RRC_STATE RRC state=%d", state);
    } else {
        logE(LOG_TAG, "requestGetSmartRatSwitch error mode=%d", mode);
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxIntsData(&state, 1), msg, false);
    responseToTelCore(response);
        return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

// NR request
void RmcNetworkRequestHandler::requestConfigA2Offset(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, offset = 0, thresh_bound = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    offset = pInt[0];
    thresh_bound = pInt[1];

    int mainSlot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    logV(LOG_TAG, "requestConfigA2Offset mainSlot=%d, m_slot_id=%d", mainSlot, m_slot_id);
    if (mainSlot == m_slot_id) {
        p_response = atSendCommand(String8::format("AT+EGCMD=6,0,\"NL1MOB_CONFIG_A2_OFFSET,OFFSET=%d,THRESH_BOUND=%d\"", offset, thresh_bound));
    } else {
        p_response = atSendCommand(String8::format("AT+EGCMD=6,0,\"NL1MOB2_CONFIG_A2_OFFSET,OFFSET=%d,THRESH_BOUND=%d\"", offset, thresh_bound));
    }
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestConfigB1Offset(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, offset = 0, thresh_bound = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    offset = pInt[0];
    thresh_bound = pInt[1];

    int mainSlot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    logV(LOG_TAG, "requestConfigA2Offset mainSlot=%d, m_slot_id=%d", mainSlot, m_slot_id);
    if (mainSlot == m_slot_id) {
        p_response = atSendCommand(String8::format("AT+EGCMD=6,0,\"ERRC_CONFIG_B1_OFFSET,OFFSET=%d,THRESH_BOUND=%d\"", offset, thresh_bound));
    } else {
        p_response = atSendCommand(String8::format("AT+EGCMD=6,0,\"ERRC2_CONFIG_B1_OFFSET,OFFSET=%d,THRESH_BOUND=%d\"", offset, thresh_bound));
    }
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestDeactivateNrScgCommunication(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, deactivate = 0, allowSCGAdd = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    deactivate = pInt[0];
    allowSCGAdd = pInt[1];

    p_response = atSendCommand(String8::format("AT+EGMC=1,\"endc_deactivation\",%d,%d", deactivate, 1));
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestGetDeactivateNrScgCommunication(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0;
    int result[2] = {0};
    char *tmp = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RfxAtLine* line = NULL;

    p_response = atSendCommandSingleline(
        String8::format("AT+EGMC=0,\"endc_deactivation\""), "+EGMC:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // get <command>
    tmp = line->atTokNextstr(&err);
    if (err < 0) goto error;

    // get <deactivate>
    result[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // get <allowSCGAdd>
    result[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    logV(LOG_TAG, "requestGetDeactivateNrScgCommunication tmp=%s deactivate=%d allowSCGAdd=%d",
            tmp, result[0], result[1]);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxIntsData(result, 2), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestSetNrOption(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, old5gopt = 0, new5gopt = 0, oldNsa = 0, newNsa = 0;
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    new5gopt = pInt[0];

    p_response = atSendCommandSingleline("AT+E5GOPT?", "+E5GOPT:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto done;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto done;

    old5gopt = line->atTokNextint(&err);
    if (err < 0) goto done;
    if (old5gopt == new5gopt) goto done;
    p_response = atSendCommand(String8::format("AT+E5GOPT=%d", new5gopt));
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto done;
    }

    // RILD cache nr info of a lte cell
    // we need to refresh the state when user disable/enable nr
    oldNsa = (old5gopt == 5 || old5gopt == 7) ? 1 : 0;
    newNsa = (new5gopt == 5 || new5gopt == 7) ? 1 : 0;
    logV(LOG_TAG, "requestDisableNr oldNsa=%d, newNsa=%d", oldNsa, newNsa);
    if (oldNsa != newNsa) {
        // nsa capability changes, need notify.
        sp<RfxMclMessage> urc;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        // response to TeleCore
        responseToTelCore(urc);
    }

done:
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

void RmcNetworkRequestHandler::requestSetNrdcState(const sp<RfxMclMessage>& msg) {
    int *args = (int *)msg->getData()->getData();
    int argsCount = msg->getData()->getDataLength() / sizeof(int);
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_INTERNAL_ERR;
    int nrOption, endcDeactivate;
    bool nsaChanged = false;

    if (!isNrSupported()) {
        radioError = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }
    if (args == NULL || argsCount != 1) {
        radioError = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    nrOption = getNrOption();
    if (nrOption < 0) goto error;

    endcDeactivate = isEndcDeactivate();
    if (endcDeactivate < 0) goto error;

    switch (*args) {

        case NRDC_ENABLE:
            if ((nrOption & NR_OPT_NSA) && !endcDeactivate)
                break;

            if (endcDeactivate) {
                setEndcDeactivate(false);
            }
            if (!(nrOption & NR_OPT_NSA)) {
                nrOption |= NR_OPT_NSA;
                nsaChanged = true;
                setNrOption(nrOption);
            }
            break;

        case NRDC_DISABLE:
            if (nrOption & NR_OPT_NSA) {
                nrOption &= ~NR_OPT_NSA;
                nsaChanged = true;
                setNrOption(nrOption);
            }
            break;

        case NRDC_DISABLE_IMMEDIATE:
            if (!(nrOption & NR_OPT_NSA) && endcDeactivate)
                break;

            if (nrOption & NR_OPT_NSA) {
                nrOption &= ~NR_OPT_NSA;
                nsaChanged = true;
                setNrOption(nrOption);
            }
            if (!endcDeactivate) {
                setEndcDeactivate(true);
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

void RmcNetworkRequestHandler::requestGetNrdcState(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_INTERNAL_ERR;
    int nrOption = 0, endcDeactivate = 0, enabled;

    if (!isNrSupported()) goto error;

    nrOption = getNrOption();
    if (nrOption < 0) goto error;

    endcDeactivate = isEndcDeactivate();
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

int RmcNetworkRequestHandler::setNrOption(int option) {
    sp<RfxAtResponse> atResp;
    int err = 0;

    atResp = atSendCommand(String8::format("AT+E5GOPT=%d", option));
    err = atResp->getError();
    if (err < 0 || atResp->getSuccess() == 0) {
        logE(LOG_TAG, "+E5GOPT[%d] fail", option);
        return -1;
    }

    return 0;
}

int RmcNetworkRequestHandler::setEndcDeactivate(bool deactivate) {
    sp<RfxAtResponse> atResp;
    int err = 0;

    atResp = atSendCommand(String8::format(
            "AT+EGMC=1,\"endc_deactivation\",%d,1", deactivate ? 1:0));
    err = atResp->getError();
    if (err < 0 || atResp->getSuccess() == 0) {
        logE(LOG_TAG, "+EGMC[%d] fail", deactivate);
        return -1;
    }

    return 0;
}

int RmcNetworkRequestHandler::getNrOption() {
    sp<RfxAtResponse> atResp;
    RfxAtLine* line = NULL;
    int err = 0;
    int result = 0;

    atResp = atSendCommandSingleline("AT+E5GOPT?", "+E5GOPT:");
    err = atResp->getError();
    if (err != 0 || atResp == NULL
            || atResp->getSuccess() == 0
            || atResp->getIntermediates() == NULL) {
        return -1;
    }

    line = atResp->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) return -1;

    result = line->atTokNextint(&err);
    if (err < 0) return -1;

    return result;
}

int RmcNetworkRequestHandler::isEndcDeactivate() {
    sp<RfxAtResponse> atResp;
    RfxAtLine* line = NULL;
    char * command = NULL;
    int result = 0, err = 0;

    atResp = atSendCommandSingleline(
                String8::format("AT+EGMC=0,\"endc_deactivation\""), "+EGMC:");
    err = atResp->getError();
    if (err != 0 || atResp == NULL
            || atResp->getSuccess() == 0
            || atResp->getIntermediates() == NULL) {
        return -1;
    }
    line = atResp->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) return -1;

    command = line->atTokNextstr(&err);
    if (err < 0) return -1;

    result = line->atTokNextint(&err);
    if (err < 0) return -1;

    return result;
}

void RmcNetworkRequestHandler::requestSearchStoreFrenquencyInfo(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, optr = 0, plmn = 0, rat = 0, count = 0;
    String8 freq("");
    char *cmd = NULL;
    char string[RFX_PROPERTY_VALUE_MAX+1];memset(string, 0, RFX_PROPERTY_VALUE_MAX+1);
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();

    optr = pInt[0];
    plmn = pInt[1];
    rat = pInt[2];
    count = pInt[3];

    logV(LOG_TAG, "eopttSearchStoreFrenquencyInfo=%d plmn=%d rat=%d count=%d err=%d",
            optr, plmn, rat, count, err);
    // AT+EPLMNFREQ=<op>,<rat>,<plmn_id>[,<num_arfcn>[,<arfcn_1>,<arfcn_2>, ...]]
    if (optr == 0) {
        err = snprintf(string, RFX_PROPERTY_VALUE_MAX, "AT+EPLMNFREQ=%d,%d,%d", optr, rat, plmn);
        if (err < 0) {
            logE(LOG_TAG, "requestSearchStoreFrenquencyInfo::using snprintf and getting ERROR");
            goto error;
        }
    } else if (optr == 1) {
        for (int i = 0 ; i < count ; i++) {
            freq.append(String8::format(",%d", pInt[i+4]).string());
            logV(LOG_TAG, "pInt[%d]:%d, sting:%s", i, pInt[i+4], freq.string());
        }
        err = snprintf(string, RFX_PROPERTY_VALUE_MAX, "AT+EPLMNFREQ=%d,%d,%d,%d%s", optr, rat, plmn, count, freq.string());
        if (err < 0) {
            logE(LOG_TAG, "requestSearchStoreFrenquencyInfo::using snprintf and getting ERROR");
            goto error;
        }
    } else {
        logE(LOG_TAG, "requestSearchStoreFrenquencyInfo::incorrect operator");
        goto error;
    }

    err = asprintf(&cmd, "%s", string);
    if (err < 0 || cmd == NULL) {
        logE(LOG_TAG, "requestSearchStoreFrenquencyInfo::using asprintf and getting ERROR");
        goto error;
    }

    p_response = atSendCommand(cmd);
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }
    free(cmd);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    if (cmd) free(cmd);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestSearchRat(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, count = 0;
    char *cmd = NULL;
    String8 rat("");
    char string[RFX_PROPERTY_VALUE_MAX+1];memset(string, 0, RFX_PROPERTY_VALUE_MAX+1);
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    count = pInt[0];
    logV(LOG_TAG, "requestSearchRat count: %d", count);

    for (int i = 1 ; i <= count ; i++) {
        rat.append(String8::format(",%d", pInt[i]).string());
        logV(LOG_TAG, "pInt[%d]:%d, sting:%s", i, pInt[i], rat.string());
    }
    err = snprintf(string, RFX_PROPERTY_VALUE_MAX, "AT+EPRATL=%d%s", count, rat.string());
    if (err < 0) {
        logE(LOG_TAG, "requestSearchRat::using snprintf and getting ERROR");
        goto error;
    }

    err = asprintf(&cmd, "%s", string);
    if (err < 0 || cmd == NULL) {
        logE(LOG_TAG, "requestSearchRat::using asprintf and getting ERROR");
        goto error;
    }

    p_response = atSendCommand(cmd);
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }
    free(cmd);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    if (cmd) free(cmd);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestSetBackgroundSearchTimer(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int err = 0, duration = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    duration = pInt[0];

    logV(LOG_TAG, "requestSetBackgroundSearchTimer duration=%d", duration);

    // Delta duration(minutes),range:(0..25) default value: 5
    p_response = atSendCommand(String8::format("AT+EGMC=1,\"bgsrch_slp_delta\",%d", duration));
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        ril_errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}
void RmcNetworkRequestHandler::onListenAtFromOemHook(const sp<RfxMclMessage>& msg) {
    char *oem_hook = NULL;
    int old5gopt = current_5gopt_config[m_slot_id];
    int new5gopt = 0;
    int oldNsa = 0, newNsa = 0;
    if (msg != NULL && msg->getData() != NULL) {
        oem_hook = (char*)(msg->getData()->getData());
        logV(LOG_TAG, "onListenAtFromOemHook = %s", oem_hook);
        if (strstr(oem_hook, "AT+E5GOPT=")) {
            logV(LOG_TAG, "onListenAtFromOemHook = AT+E5GOPT~%s", &oem_hook[10]);
            new5gopt = atoi(&oem_hook[10]);
            oldNsa = (old5gopt == 5 || old5gopt == 7) ? 1 : 0;
            newNsa = (new5gopt == 5 || new5gopt == 7) ? 1 : 0;
            logV(LOG_TAG, "onListenAtFromOemHook oldNsa=%d, newNsa=%d", oldNsa, newNsa);
            if (oldNsa != newNsa) {
                // nsa capability changes, need notify.
                sp<RfxMclMessage> urc;
                urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                        m_slot_id, RfxVoidData());
                // response to TeleCore
                responseToTelCore(urc);
            }
        } else if (strstr(oem_hook, "UPDATE_DISPLAY_RULE")) {
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
                logE(LOG_TAG, "onListenAtFromOemHook UPDATE_DISPLAY_RULE "
                    "There is something wrong with the request %d =", set_nr_display_rule);
                if ((set_nr_display_rule == 0)
                    || (set_nr_display_rule == NR_DISPALY_NR_CACHE)
                    || ((nr_display_rule[m_slot_id] & NR_DISPALY_LTE_CONNECTED_STATE)
                        == NR_DISPALY_LTE_CONNECTED_STATE)) {
                    updateNrDisplayRule(is_rule_by_slot, set_nr_display_rule,
                            set_jpmode_timer1_delay_timer,
                            set_jpmode_timer2_delay_timer);
                }
                return;
        }
    }
}

//To sync 5g icon rule to this slot
void RmcNetworkRequestHandler::onSync5GIconShowRuleWithOtherSlot(const sp<RfxMclMessage>& msg){
    logV(LOG_TAG, "%s", __FUNCTION__);
    int *pInt = (int *)msg->getData()->getData();
    int set_nr_display_rule = pInt[0];
    int set_jpmode_timer1_delay_timer = pInt[1];
    int set_jpmode_timer2_delay_timer = pInt[2];
    logD(LOG_TAG, "onSync5GIconShowRuleWithOtherSlot set_nr_display_rule, %d", set_nr_display_rule);

    updateNrDisplayRule(true, set_nr_display_rule,
            set_jpmode_timer1_delay_timer,
            set_jpmode_timer2_delay_timer);
    return;
}

void RmcNetworkRequestHandler::updateNrDisplayRule(
        bool isRuleBySlot, int setRule, int setJpTimer1, int setJpTimer2) {
    logV(LOG_TAG, "updateNrDisplayRule want to set value: %d, %d, %d,"
            "current value:%d, %d, %d, bySlot %d",
            setRule, setJpTimer1, setJpTimer2,
            nr_display_rule[m_slot_id],
            jp_mode_timer1_delay_timer[m_slot_id],
            jp_mode_timer2_delay_timer[m_slot_id],
            isRuleBySlot);

    bool isRuleChange = true;
    if (nr_display_rule[m_slot_id] == setRule) {
        if (NR_DISPLAY_JP_MODE == setRule &&
                (setJpTimer1 != jp_mode_timer1_delay_timer[m_slot_id]
                        || setJpTimer2 != jp_mode_timer2_delay_timer[m_slot_id])) {
            /* if set JP mode again but timer is not same as the old one,
               don't stop the ongoing timer, just change it */
            isRuleChange = true;
        } else {
            isRuleChange = false;
        }
    } else {
        if (nr_display_rule[m_slot_id] == NR_DISPLAY_JP_MODE) {
            //current nr_display_rule is NR_DISPLAY_JP_MODE, stop JP timer first
            stopJpModeTimer1();
            stopJpModeTimer2();
        }
        String8 nrDisplayRule("persist.vendor.radio.nr_display_rule");
        nrDisplayRule.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
        rfx_property_set(nrDisplayRule.string(), String8::format("%d", setRule).string());
        nr_display_rule[m_slot_id] = setRule;
    }

    if ((nr_display_rule[m_slot_id] == 0) || (nr_display_rule[m_slot_id] == NR_DISPALY_NR_CACHE)) {
        // clear all timers
        setJpTimer1 = 0;
        setJpTimer2 = 0;
    } else if (nr_display_rule[m_slot_id] == NR_DISPLAY_JP_MODE) {
        //NR_DISPLAY_JP_MODE now, clear lte, scg delay timer
    } else if ((nr_display_rule[m_slot_id] & NR_DISPALY_LTE_CONNECTED_STATE) == NR_DISPALY_LTE_CONNECTED_STATE) {
        setJpTimer1 = 0;
        setJpTimer2 = 0;
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
        // update LTE RRC state
        updateConnectionStateReport(UPDATE_CONNECTION_STATE_REASON_NSA_RULE_CHANGED);
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

void RmcNetworkRequestHandler::requestGetBarringInfo(const sp<RfxMclMessage>& msg) {
    const int umtsTypes[] = {0, 1, 2, 8}; /*CS_SERVICE, PS_SERVICE, CS_VOICE, EMERGENCY*/
    const int lteTypes[] = {3, 4, 5, 6, 7, 8, 9};
    const int saTypes[] = {3, 4, 5, 6, 7, 8, 9, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
                1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022,
                1023, 1024, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032};
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RfxAtLine* line = NULL;

    /* +EBARRING: <mode>,<rat>,<count>,<serviceType>,<barringType>,<factor>,<timeSeconds>,<isBarred>
       Note: 1. If <barringType> is not CONDITIONAL（1）， the last item does not contain the last
               fields: <factor>,<timeSeconds>,<isBarred>.
               Because AT CMD can not end with ",,," as AT SPEC requirement .
             2. if count is 0, it means that all items are BT_NONE.*/

    int err = 0, count = 0, mode = 0, rat = 0, i, j, number;
    const int *pType;
    int serviceType, factor, timeSeconds, isBarred;
    RIL_BarringType barringType = BT_NONE;
    RIL_BarringInforResponse *barringInfoResp =
            (RIL_BarringInforResponse*) calloc(1, sizeof(RIL_BarringInforResponse));
    RIL_BarringInfo *pBarringInfos = NULL;
    if (barringInfoResp == NULL) goto error;
    memset(barringInfoResp, 0, sizeof(RIL_BarringInforResponse));

    p_response = atSendCommandSingleline("AT+EBARRING?", "+EBARRING:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    if (line == NULL) goto error;
    line->atTokStart(&err);
    if (err < 0) goto error;
    mode = line->atTokNextint(&err);
    if (err < 0) goto error;
    rat = line->atTokNextint(&err);
    if (err < 0) goto error;
    count = line->atTokNextint(&err);
    if (err < 0) goto error;

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

    for (int i = 0; i < count; i++) {
        serviceType = line->atTokNextint(&err);
        if (err < 0) goto error;
        barringType = (RIL_BarringType) line->atTokNextint(&err);
        if (err < 0) goto error;
        if (i != count - 1 || barringType == CONDITIONAL) {
            factor = line->atTokNextint(&err);
            if (err < 0) factor = 0;
            timeSeconds = line->atTokNextint(&err);
            if (err < 0) timeSeconds = 0;
            isBarred= line->atTokNextint(&err);
            if (err < 0) isBarred = 0;
        } else {
            factor = 0;
            timeSeconds = 0;
            isBarred = 0;
        }
        // find the item and replace the values
        for (j = 0; j < number; j++) {
            if (pBarringInfos[j].service_type == serviceType) {
                pBarringInfos[j].barring_type = barringType;
                pBarringInfos[j].factor = factor;
                pBarringInfos[j].timeSeconds = timeSeconds;
                pBarringInfos[j].isBarred = isBarred > 0 ? true : false ;
                break;
            }
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
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_REQUEST_NOT_SUPPORTED,
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

void RmcNetworkRequestHandler::handleRegistrationFailedEvent(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> urc = NULL;
    int *pInt = (int *)msg->getData()->getData();
    RIL_Domain domain = (RIL_Domain) pInt[0];
    RIL_RegistrationFailedResponse *rilRegFailResp =
            (RIL_RegistrationFailedResponse*) calloc(1, sizeof(RIL_RegistrationFailedResponse));
    if (rilRegFailResp == NULL) goto error;
    memset(rilRegFailResp, 0, sizeof(RIL_RegistrationFailedResponse));
    logD(LOG_TAG, "%s domain=%d", __FUNCTION__, domain);

    if (updatePlmnInfo()  < 0) goto error;
    pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
    strncpy(rilRegFailResp->plmn, op_info_cache[m_slot_id]->mccMnc, 7);
    pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
    rilRegFailResp->domain = domain;
    rilRegFailResp->additionalCauseCode = INT_MAX;
    switch (domain) {
        case RIL_DOMAIN_CS:
            fillVoiceCellIdentity(rilRegFailResp->cellIdentity);
            rilRegFailResp->causeCode = mdEreg[m_slot_id]->reject_cause;
            break;
        case RIL_DOMAIN_PS:
            fillDataCellIdentity(rilRegFailResp->cellIdentity);
            rilRegFailResp->causeCode = mdEgreg[m_slot_id]->reject_cause;
            break;
        default:
            break;
    }
    if (rilRegFailResp->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_NONE) {
        logE(LOG_TAG, "handleRegistrationFailedEvent no valid cell info");
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

void RmcNetworkRequestHandler::updateConnectionStateReport(int reason) {
    int err = 0;
    int n = 0, mode = 0, state = 0, access = 0;
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;

    p_response = atSendCommandSingleline("AT+CSCON?", "+CSCON:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    n = line->atTokNextint(&err);
    if (err < 0) goto error;

    mode = line->atTokNextint(&err);
    if (err < 0) goto error;

    state = line->atTokNextint(&err);
    if (err < 0) goto error;

    access = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (reason == UPDATE_CONNECTION_STATE_REASON_SCREEN_ON) {
        int technology = mdEgreg[m_slot_id]->eAct;
        logV(LOG_TAG, "[%s]: mode=%d, state=%d, access=%d, technology = %d", __FUNCTION__, mode, state, access, technology);
        if (isInService(mdEgreg[m_slot_id]->stat) &&
                (technology == 32768 || technology == 16384 || technology == 8192 || technology == 4096)) {
            bool changed = false;
            //default set NrRrcState as connected for 16384 and disconnected for others
            int newNrRrcState = (technology == 16384 || technology == 32768) ? 1 : 0;
            //default set LteRrcState as connected
            int newLteRrcState = technology == 32768 ? 0 : 1;

            if (access == 3 || access == 4) {
                //update LTE RRC State by queried cscon
                if (mode == 1 && state == 7) newLteRrcState = 1;
                else newLteRrcState = 0;
            } else if (access == 5) {
                //update NR RRC State by queried cscon
                newNrRrcState = (mode == 1 && state == 8) ? 1 : 0;
            } else {
                logE(LOG_TAG, "[%s]: don't care access[%d]", __FUNCTION__, access);
                return;
            }
            logV(LOG_TAG, "[%s]: calculate nrRrcState = %d, lteRrcState =%d", __FUNCTION__, newNrRrcState, newLteRrcState);
            //update NR RRC state
            pthread_mutex_lock(&nr_connection_state_Mutex[m_slot_id]);
            if (newNrRrcState != nr_connection_state[m_slot_id]) {
                nr_connection_state[m_slot_id] = newNrRrcState;
                changed = true;
            }
            pthread_mutex_unlock(&nr_connection_state_Mutex[m_slot_id]);

            //update LTE rrc state
            int oldLteRrcState = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_LTE_RRC_STATE, 1);
            if (oldLteRrcState != newLteRrcState) {
                getMclStatusManager()->setIntValue(RFX_STATUS_KEY_LTE_RRC_STATE, newLteRrcState);
                changed = true;
            }
            if (changed) {
                sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(), RIL_CMD_PROXY_3, m_slot_id);
            }
            return;
        }
    }
    connectionStateReport(mode, state, access);
    return;
error:
    logE(LOG_TAG, "There is something wrong with the updateConnectionStateReport URC");
    return;
}

void RmcNetworkRequestHandler::updateEndcCapability() {
    int err = 0;
    int is_endc_supported = 0;
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;

    p_response = atSendCommandSingleline("AT+ECAPABILITY?", "+ECAPABILITY:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    is_endc_supported = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (is_endc_supported == 0 || is_endc_supported == 1) {
        endc_capability[m_slot_id] = is_endc_supported;
    } else {
        // we have nr by default.
        endc_capability[m_slot_id] = 1;
    }
    return;
error:
    logE(LOG_TAG, "There is something wrong with AT+ECAPABILITY");
    return;
}

void RmcNetworkRequestHandler::requestGetTOEInfo(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    char *resp[3];
    sp<RfxMclMessage> response;
    char longname[MAX_OPER_NAME_LENGTH], shortname[MAX_OPER_NAME_LENGTH];
    char emptyString[MAX_OPER_NAME_LENGTH] = "";

    //get toe info when valid operator numeric which is the same with nitz operator name,
    //or return empty string
    memset(resp, 0, sizeof(resp));
    if ((strncmp(mdEops[m_slot_id]->oper, "000000", 6) != 0) &&
                (mdEops[m_slot_id]->oper[0] >= '0' && mdEops[m_slot_id]->oper[0] <= '9') &&
                strcmp(mdEops[m_slot_id]->oper, m_ril_nw_nitz_oper_code[m_slot_id]) == 0) {
        getOperatorNamesFromNumericCodeByDisplay(mdEops[m_slot_id]->oper, 0,
            longname, shortname, MAX_OPER_NAME_LENGTH, DISPLAY_NITZ, mdEgreg[m_slot_id]->eAct);
        resp[0] = longname;
        resp[1] = shortname;
        resp[2] = mdEops[m_slot_id]->oper;
    } else {
        resp[0] = emptyString;
        resp[1] = emptyString;
        resp[2] = emptyString;
    }

    logD(LOG_TAG, "requestGetTOEInfo %d: %s %s %s %s", m_slot_id+1, resp[0], resp[1], resp[2], mdEops[m_slot_id]->oper);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                       RfxStringsData(resp, 3), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;
}

void RmcNetworkRequestHandler::requestSetCarrierAggregationMode(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int err = 0;
    RIL_Errno ril_errno = RIL_E_INVALID_ARGUMENTS;

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
        p_response = atSendCommand(String8::format("AT+ECASW=%d", (mode == 0) ? 0 : 1)); //turn on/off LTE CA
    } else if (option == 1) {
        p_response = atSendCommand(String8::format("AT+ESBP=5,\"SBP_NR_CA\",%d", (mode == 0) ? 0 : 1));  //turn on/off SA CA
    } else {
        goto error;
    }

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0) {
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

void RmcNetworkRequestHandler::requestGetCALinkEnableStatus(const sp<RfxMclMessage>& msg) {
    /*AT+ECAULDL=<mode>[,<dir>,<band_combo>]*/
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int err = 0, mode = 2, dir = 1;
    int status = 0;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
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
        mode = 2;
        dir = 1;
    } else {
        mode = 3;
        dir = 0;
    }

    p_response = atSendCommandSingleline(String8::format("AT+ECAULDL=%d,%d,\"%s\"", mode, dir, strings[0]), "+ECAULDL:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // status
    status = line->atTokNextint(&err);
    if (err < 0) goto error;

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

void RmcNetworkRequestHandler::requestSetCALinkEnableStatus(const sp<RfxMclMessage>& msg) {
    /*AT+ECACOMB=<cmd_type>,<list_type>[,<ca_comb_list>]*/
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int err = 0;

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

    p_response = atSendCommand(String8::format("AT+ECACOMB=1,%d,\"%s\"", (enableStatus > 0) ? 0 : 1, strings[1]));

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0) {
        goto error;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    return;
error:
    logE(LOG_TAG, "requestSetCALinkEnableStatus return error");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestGetCALinkCapabilityList(const sp<RfxMclMessage>& msg) {
    /*AT+ECAULDL=<mode>[,<dir>,<band_combo>]*/
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int err = 0, count = 0, i = 0, k = 0;
    int mode = 0;
    int dir = 0;
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    char **response = NULL; // response link capability list
    char *tmp, *block_p = NULL;
    int maxComboNumber = 300; //max DL or UL number
    int maxComboStrLen = 15; //each band Combo max length
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
        mode = 0;
    } else {
        mode = 1;
        maxComboNumber = 600; //both UL and DL 2*300
    }

    response = (char **) calloc(1, sizeof(char*) * maxComboNumber); // for string, each one is 15 bytes
    if (response == NULL) {
        logE(LOG_TAG, "requestGetCALinkCapabilityList calloc fail");
        goto error;
    }
    block_p = (char *) calloc(1, maxComboNumber * sizeof(char) * (maxComboStrLen + 1));
    if (block_p == NULL) {
        logE(LOG_TAG, "requestGetCALinkCapabilityList calloc fail");
        goto error;
    }

    p_response = atSendCommandMultiline(String8::format("AT+ECAULDL=%d", mode), "+ECAULDL:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
        goto error;
    }

    for (line = p_response->getIntermediates();
         line != NULL;
         line = line->getNext()) {
        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;
        /* skip dir*/
        dir = line->atTokNextint(&err);
        if (err < 0) goto error;

        while (line->atTokHasmore()) {
            if (i == maxComboNumber) break;
            tmp = line->atTokNextstr(&err);
            if (err < 0) goto error;
            if (strlen(tmp) != 0) {
                response[i] = &block_p[i * (maxComboStrLen + 1)];
                // check if this item is redundant
                for (k = 0; k < i; k++)
                {
                    // compare
                    if (0 == strcmp(tmp, response[k])) {
                        break;
                    }
                }
                if (i == k) { // unique
                    strncpy(response[i], tmp, maxComboStrLen);
                    i++;
                }
            }
        }
    }

    count = i;
    logD(LOG_TAG, "requestGetCALinkCapabilityList Count = %d", count);

    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringsData(response, count), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
    free(response);
    free(block_p);
    return;
error:
    logE(LOG_TAG, "requestGetCALinkEnableStatus return error");
    if (block_p) free(block_p);
    if (response) free(response);
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestGetLteData(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    int err = 0, skip;
    //default state value not registered, others default value INT_MAX
    int lteData[12] =
        {0, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,
        0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};

    if (mdEgreg[m_slot_id]->eAct == 0x1000 || //LTE
        mdEgreg[m_slot_id]->eAct == 0x2000 || //LTE CA
        mdEgreg[m_slot_id]->eAct == 0x4000) {   //NSA
        lteData[0] = data_reg_state_cache[m_slot_id]->registration_state;//state
        lteData[1] = mdEcell[m_slot_id]->mcc; //mcc
        lteData[2] = mdEcell[m_slot_id]->mnc; //mnc
        lteData[3] = data_reg_state_cache[m_slot_id]->cid; // cell id
        lteData[4] = lteBand[m_slot_id]; // lte band id

        // get LTE band width
        // When we query LTE bw info, the only way is to turn on the URC.
        p_response = atSendCommandSingleline("AT+ELTEBWINFO=1", "+ELTEBWINFO:");
        // check error
        err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) goto error;

        // handle intermediate
        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        lteData[5] = line->atTokNextint(&err); //band width
        if (err < 0) goto error;

        // we turn off the URC right after we got lte bw info
        atSendCommand("AT+ELTEBWINFO=0");

        //get lte dl and ul
        p_response = atSendCommandSingleline("AT+ELTEGEN=0", "+ELTEGEN:");
        // check error
        err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) goto error;

        // handle intermediate
        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        skip = line->atTokNextint(&err); //op
        if (err < 0) goto error;

        lteData[6] = line->atTokNextint(&err); //ul
        if (err < 0) goto error;

        lteData[7] = line->atTokNextint(&err); //dl
        if (err < 0) goto error;

        lteData[8] = signal_strength_cache[m_slot_id]->lte_signal_strength; //lte rssi
        lteData[9] = signal_strength_cache[m_slot_id]->lte_rsrq; //lte rsrq
        lteData[10] = signal_strength_cache[m_slot_id]->lte_rsrp;//lte rsrp

        // get LTE tx power
        p_response = atSendCommandSingleline("AT+ETHERMAL", "+ETHERMAL:");
        // check error
        err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) goto error;

        // handle intermediate
        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        skip = line->atTokNextint(&err); //rat
        if (err < 0) goto error;

        skip = line->atTokNextint(&err); //temperature
        if (err < 0) goto error;

        lteData[11] = line->atTokNextint(&err); //tx power
        if (err < 0) goto error;
    }
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

void RmcNetworkRequestHandler::requestGetLteRRCState(const sp<RfxMclMessage>& msg) {
    /*+EENCONST:<mode>, <state>*/
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int err = 0;
    int mode, state, lteRRCState = 0xFF;

    p_response = atSendCommandSingleline("AT+EENCONST?", "+EENCONST:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // mode
    mode = line->atTokNextint(&err);
    if (err < 0) goto error;

    // state
    state = line->atTokNextint(&err);
    if (err < 0) goto error;

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

void RmcNetworkRequestHandler::requestSetLteBandEnableStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    const char **strings = (const char **)msg->getData()->getData();
    int err, gsm_band, umts_band;
    unsigned int lteb1to32, lteb33to64;
    int setLteBandId = atoi(strings[0]);
    int enable = atoi(strings[1]);

    p_response = atSendCommandSingleline("AT+EPBSE?", "+EPBSE:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        logE(LOG_TAG, "Query current band mode: fail, err=%d", p_response->getError());
        goto error;
    }

    line = p_response->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) goto error;

    // get supported GSM bands
    gsm_band = line->atTokNextint(&err);
    if (err < 0) goto error;

    // get supported UMTS bands
    umts_band = line->atTokNextint(&err);
    if (err < 0) goto error;

    // get supported LTE band1to32
    lteb1to32 = line->atTokNextint(&err);
    if (err < 0) goto error;

    // get supported LTE band33to64
    lteb33to64 = line->atTokNextint(&err);
    if (err < 0) goto error;

    logD(LOG_TAG, "requestSetLteBandEnableStatus, gsm_band:%d, umts_band : %d, lte_band_1_32 : %u, lte_band_33_64: %u, setLteBandId: %d, enable: %d",
            gsm_band, umts_band, lteb1to32, lteb33to64, setLteBandId, enable);

    if (setLteBandId >= 1 && setLteBandId <= 32) {
        lteb1to32 = enable ? (lteb1to32 | (1 << (setLteBandId - 1))) : (lteb1to32 & ~(1 << (setLteBandId - 1)));
    } else if (setLteBandId >= 33 && setLteBandId <= 64) {
        lteb33to64 = enable ? (lteb33to64 | (1 << (setLteBandId - 33))) : (lteb33to64 & ~(1 << (setLteBandId - 33)));
    } else {
        logE(LOG_TAG, "requestSetLteBandEnableStatus, wrong lte band id: %d", setLteBandId);
    }

    p_response = atSendCommand(String8::format("AT+EPBSE=%d,%d,%u,%u", gsm_band, umts_band, lteb1to32, lteb33to64));

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0) {
        logE(LOG_TAG, "set band mode: fail, err=%d", p_response->getError());
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

void RmcNetworkRequestHandler::requestGetBandPriorityList(const sp<RfxMclMessage>& msg) {
    /*AT+EGMC=0,"priority_band"*/
    RfxAtLine *line = NULL;
    char *tmp = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int bandPriList[15] = {0};  // maximum band support 15
    int bandListCount = 0;
    int err = 0;

    p_response = atSendCommandSingleline(
        String8::format("AT+EGMC=0,\"priority_band\""), "+EGMC:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();

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
    return;
error:
    logE(LOG_TAG, "requestGetBandPriorityList must never return error when radio is on");
    resp = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestSetBandPriorityList(const sp<RfxMclMessage>& msg) {
    /*AT+EGMC=1,"priority_band",<band1>,<band2>,<band3>,...*/
    RIL_Errno ril_errno = RIL_E_GENERIC_FAILURE;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    String8 cmd("");
    int *pInt = (int*)msg->getData()->getData();
    int bandLen = msg->getData()->getDataLength() / sizeof(int);

    if (bandLen > 15)
        bandLen = 15; //max support band number is 15

    cmd.append(String8::format("AT+EGMC=1,\"priority_band\""));
    for (int i = 0; i < bandLen; i++) {
        cmd.append(String8::format(",%d", pInt[i]));
    }

    logD(LOG_TAG, "requestSetBandPriorityList %s", cmd.string());
    p_response = atSendCommand(cmd);
    if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
        ril_errno = RIL_E_SUCCESS;
    }

    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcNetworkRequestHandler::requestSet4x4MimoEnabled(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    int enabled_bitmask = pInt[0];

    logD(LOG_TAG, "requestSet4x4MimoEnabled, bitmask = %d", enabled_bitmask);

    int rx_mimo_enable = (enabled_bitmask & 0x20) == 0x20 ? 1 : 0;
    int mimo_4x4_enable = (enabled_bitmask & 0x10) == 0x10 ? 1 : 0;
    int mimo_4x4_cca_enable = (enabled_bitmask & 0x08) == 0x08 ? 1 : 0;
    int ras_4rx_2rx_enable = (enabled_bitmask & 0x04) == 0x04 ? 1 : 0;
    int ras_4rx_2rx_cca_enable = (enabled_bitmask & 0x02) == 0x02 ? 1 : 0;
    int ras_2rx_1rx_enable = (enabled_bitmask & 0x01) == 0x01 ? 1 : 0;
    sp<RfxAtResponse> p_response = atSendCommand(String8::format("AT+EGMC=1,\"rx_mimo_set\",%d,%d,%d,%d,%d,%d",
        rx_mimo_enable, mimo_4x4_enable, mimo_4x4_cca_enable, ras_4rx_2rx_enable, ras_4rx_2rx_cca_enable, ras_2rx_1rx_enable));
    int err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "requestSet4x4MimoEnabled fail");
        ril_errno = RIL_E_GENERIC_FAILURE;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
                       RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;
}

void RmcNetworkRequestHandler::requestGet4x4MimoEnabled(const sp<RfxMclMessage>& msg) {
    int enabled_bitmask = 0;
    sp<RfxMclMessage> response;
    int err = 0, rx_mimo_enable = 0, mimo_4x4_enable = 0, mimo_4x4_cca_enable = 0;
    int ras_4rx_2rx_enable = 0, ras_4rx_2rx_cca_enable = 0, ras_2rx_1rx_enable = 0;
    char *tmp = NULL;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line = NULL;
    logD(LOG_TAG, "requestGet4x4MimoEnabled start");
    p_response = atSendCommandSingleline(
            String8::format("AT+EGMC=0,\"rx_mimo_set\""), "+EGMC:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }
    line = p_response->getIntermediates();

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

    logD(LOG_TAG, "requestGet4x4MimoEnabled, bitmask = %d", enabled_bitmask);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                       RfxIntsData(&enabled_bitmask, 1), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                       RfxIntsData(&enabled_bitmask, 1), msg, false);
    // response to TeleCore
    responseToTelCore(response);

}


void RmcNetworkRequestHandler::requestGetLteBsrTimer(const sp<RfxMclMessage>& msg) {
    int err = 0, timer = 0;
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    p_response = atSendCommandSingleline("AT+EGDM=0,1", "+EGDM:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    timer = line->atTokNextint(&err);
    if (err < 0) goto error;
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(&timer, 1), msg, false);
    logD(LOG_TAG, "requestGetLteBsrTimer: timer = %d", timer);
    responseToTelCore(response);
    return;
error:
    logE(LOG_TAG, "There is something wrong with AT+EGDM");
     response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
     responseToTelCore(response);
    return;
}
void RmcNetworkRequestHandler::requestSetLteBsrTimer(const sp<RfxMclMessage>& msg) {
    //TODO: lock is if needed
    // AT+EGDM=1,1,<timer_value_in_sec>
    int timer;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "requestSetLteBsrTimer: dataLen(%d) is wrong", dataLen);
        goto error;
    }
    timer = pInt[0];
    logD(LOG_TAG, "requestSetLteBsrTimer: timer = %d", timer);
    p_response = atSendCommand(String8::format("AT+EGDM=1,1,\"%d\"", timer));
    if (p_response->getError() < 0 || p_response->getSuccess() == 0) {
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
void RmcNetworkRequestHandler::requestGetLte1xRttCellList(const sp<RfxMclMessage>& msg) {

    //  AT+ERTTCL=0
    // +ERTTCL:<number_of_lists>[, list of (<sid>,<nid>,<bsid>),...]
    int err = 0, i, num, *data = NULL, respsenLen = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RfxAtLine* line;
    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "requestGetLte1xRttCellList: dataLen(%d) is wrong", dataLen);
        goto error;
    }
    logD(LOG_TAG, "requestGetLte1xRttCellList, available = %d",  pInt[0]);
    p_response = atSendCommandSingleline(String8::format("AT+ERTTCL=%d", pInt[0] == 0 ? 1 : 0),
                        "+ERTTCL:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // num
    num = line->atTokNextint(&err);
    if (err < 0) goto error;
    if (num > 0) {
        respsenLen = 3 * num * sizeof(int);
        data = (int *) alloca(respsenLen);
        memset(data, 0, respsenLen);

        for (i = 0; i < num; i++) {
            // sid
            data[i * 3] = line->atTokNextint(&err);
            if (err < 0) goto error;
            // nid
            data[i * 3 + 1] = line->atTokNextint(&err);
            if (err < 0) goto error;
            // bsid
            data[i * 3 + 2] = line->atTokNextint(&err);
            if (err < 0) goto error;
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
void RmcNetworkRequestHandler::requestClearLteAvailableFile(const sp<RfxMclMessage>& msg) {
    // AT+EGCMD=6,255,"GMSS_clear_lte_unavail_db"
    int err = 0;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    logD(LOG_TAG, "requestClearLteAvailableFile");

    p_response = atSendCommand("AT+EGCMD=6,255,\"GMSS_clear_lte_unavail_db\"");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
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
void RmcNetworkRequestHandler::requestGetBandMode(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+EPBSE?
     *   +EPBSE: <gsm_band>,<umts_band><lte_band_1_32>,<lte_band_33_64>
     */
    int err = 0, band[4];
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    p_response = atSendCommandSingleline("AT+EPBSE?", "+EPBSE:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // gsm_band
    band[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // umts_band
    band[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // lte_band_1_32
    band[2] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // lte_band_33_64
    band[3] = line->atTokNextint(&err);
    if (err < 0) goto error;
    logE(LOG_TAG, "requestGetBandMode: [%d, %d, %d, %d]", band[0], band[1], band[2], band[3]);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(band, 4), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with AT+EPBSE");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
    return;

}

void RmcNetworkRequestHandler::requestGetCaBandMode(const sp<RfxMclMessage>& msg) {
    /**
     *   +ECABFB=<primary band>
     *   +ECABFB: <bitmap_for_b1_b32>, <bitmap_for_b33_b64>, <bitmap_for_b65_b96>,
     *          <bitmap_for_b97_b128>,   <bitmap_for_b129_b160>, <bitmap_for_b161_b192>,
     *          <bitmap_for_b193_b224>, <bitmap_for_b225_b256>       OK  +CME ERROR: <err>
     */
    int primaryId, err = 0;
    int band[8] = {0};
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "requestGetCaBandMode: dataLen(%d) is wrong", dataLen);
        goto error;
    }
    primaryId = pInt[0];
    logD(LOG_TAG, "requestGetCaBandMode: primaryId = %d", primaryId);
    p_response = atSendCommandSingleline(String8::format("AT+ECABFB=%d", primaryId), "+ECABFB:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // bit1-32
    band[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // bit33-64
    band[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // bit65-96
    band[2] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // bit97-128
    band[3] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // bit129-160
    band[4] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // bit161-192
    band[5] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // bit193-224
    band[6] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // bit225-256
    band[7] = line->atTokNextint(&err);
    if (err < 0) goto error;
    logE(LOG_TAG, "requestGetCaBandMode: [%d, %d, %d, %d, %d, %d, %d, %d]",
            band[0], band[1], band[2], band[3], band[4], band[5], band[6], band[7]);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(band, 8), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with AT+EPBSE");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
    return;
}

void RmcNetworkRequestHandler::requestSetQamEnabled(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> atResp;
    sp<RfxMclMessage> response;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool ulOrDl, enabled;
    int protocol = 1;

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

    // AT+EGMC=1,"errc_feature_set",<protocol_idx>,<feature_enum>,<set_value>
    atResp = atSendCommand(String8::format(
            "AT+EGMC=1,\"errc_feature_set\",%d,UL64QAM,%d",
            protocol, enabled));
    if (atResp == NULL || atResp->isATCmdRspErr()) {
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

void RmcNetworkRequestHandler::requestGetQamEnabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> atResp;
    RfxAtLine *line = NULL;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool ulOrDl;
    char *errc = NULL, *feature = NULL;
    int err = 0, protocol = 1, result[2] = {0, 0};

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

    // AT+EGMC=1,"errc_feature_get",<protocol_idx>,<feature_enum>
    // +EGMC: "errc_featurE_get",<feature_enum>,<support>
    atResp = atSendCommandSingleline(String8::format(
            "AT+EGMC=1,\"errc_feature_get\",%d,UL64QAM", protocol), "+EGMC:");
    if (atResp == NULL
            || atResp->isATCmdRspErr()
            || atResp->getIntermediates() == NULL) {
        goto error;
    }
    line = atResp->getIntermediates();
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
    return;

error:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestSetTm9Enabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> atResp;
    RfxAtLine *line = NULL;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool fddOrTdd, enabled;
    bool fddEnabled, tddEnabled;
    int err = 0;

    if (msg->getData()->getDataLength() != 2 * sizeof(int)
        || msg->getData()->getData() == NULL) {
        logE(LOG_TAG, "requestSetTm9Enabled invalid parameter");
        goto error;
    }

    fddOrTdd = (bool)((int *)msg->getData()->getData())[0];
    enabled = (bool)((int *)msg->getData()->getData())[1];
    logV(LOG_TAG, "requestSetTm9Enabled fddOrTdd[%d] enabled[%d]", fddOrTdd, enabled);

    // +ETM9:<tm9_fdd_setting>,<tm9_tdd_setting>
    atResp = atSendCommandSingleline("AT+ETM9?", "+ETM9:");
    if (atResp == NULL
            || atResp->isATCmdRspErr()
            || atResp->getIntermediates() == NULL) {
        goto error;
    }
    line = atResp->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;

    fddEnabled = (bool)line->atTokNextint(&err);
    if (err < 0) goto error;

    tddEnabled = (bool)line->atTokNextint(&err);
    if (err < 0) goto error;

    // AT+ETM9=<tm9_fdd_setting>,<tm9_tdd_setting>
    if ((fddOrTdd && enabled == fddEnabled)
        || (!fddOrTdd && enabled == tddEnabled)) {
        goto success;
    }

    if (fddOrTdd) {
        atResp = atSendCommand(String8::format(
            "AT+ETM9=%d,%d", enabled, tddEnabled));
    } else {
        atResp = atSendCommand(String8::format(
            "AT+ETM9=%d,%d", fddEnabled, enabled));
    }
    if (atResp == NULL || atResp->isATCmdRspErr()) {
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

void RmcNetworkRequestHandler::requestGetTm9Enabled(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> atResp;
    RfxAtLine *line = NULL;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    bool fddOrTdd;
    bool fddEnabled, tddEnabled;
    int err = 0, result[2] = {0, 0};

    if (msg->getData()->getDataLength() != sizeof(int)
        || msg->getData()->getData() == NULL) {
        logE(LOG_TAG, "requestGetTm9Enabled invalid parameter");
        goto error;
    }

    fddOrTdd = (bool)((int *)msg->getData()->getData())[0];
    logV(LOG_TAG, "requestGetTm9Enabled fddOrTdd[%d]", fddOrTdd);
    result[0] = fddOrTdd;

    // +ETM9:<tm9_fdd_setting>,<tm9_tdd_setting>
    atResp = atSendCommandSingleline("AT+ETM9?", "+ETM9:");
    if (atResp == NULL
            || atResp->isATCmdRspErr()
            || atResp->getIntermediates() == NULL) {
        goto error;
    }
    line = atResp->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;

    fddEnabled = (bool)line->atTokNextint(&err);
    if (err < 0) goto error;

    tddEnabled = (bool)line->atTokNextint(&err);
    if (err < 0) goto error;

    if (fddOrTdd)
        result[1] = fddEnabled;
    else
        result[1] = tddEnabled;

    response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxIntsData(result, 2), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(
            msg->getId(), radioError, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestSetLteScanDuration(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> atResp;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    int duration;
    const int MAX_SCAN_DURATION = 3600;

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

    // AT+EGDM=<op>,<type>[,<node_value>]
    atResp = atSendCommand(String8::format(
            "AT+EGDM=1,2,\"%d\"", duration));
    if (atResp == NULL || atResp->isATCmdRspErr()) {
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

void RmcNetworkRequestHandler::requestGetLteScanDuration(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> atResp;
    RfxAtLine *line = NULL;
    RIL_Errno radioError = RIL_E_GENERIC_FAILURE;
    char *duration = NULL;
    int err = 0, result = 0;

    // AT+EGDM=<op>,<type>[,<node_value>]
    atResp = atSendCommandSingleline("AT+EGDM=0,2", "+EGDM:");
    if (atResp == NULL
            || atResp->isATCmdRspErr()
            || atResp->getIntermediates() == NULL) {
        goto error;
    }
    line = atResp->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;

    duration = line->atTokNextstr(&err);
    if (err < 0) goto error;

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

void RmcNetworkRequestHandler::requestGetAllBandMode(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+EPBSH?
     *   +EPBSEH: <gsm_band>, <umts_band>,<lte_band>,<nr_band>[,<nr_nsa_band>]
     */
    int err = 0, len, i, count = 0, totalCount = 0;
    // gsm->  umts->lteCount-> lte-> saCount -> sa -> nsaCount ->nsa;
    int result[1+1+1+8+1+32+1+32] = {0};
    RfxAtLine* line = NULL;
    char * hexIntStr, charTemp[9] = {0};
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    p_response = atSendCommandSingleline("AT+EPBSEH?", "+EPBSEH:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        return requestGetAllBandModeWithoutNr(msg);
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // gsm
    result[totalCount++] = line->atTokNexthexint(&err);
    if (err < 0) goto error;
    logE(LOG_TAG, "gsmBand = %d", result[0]);

    // umts
    result[totalCount++] = line->atTokNexthexint(&err);
    if (err < 0) goto error;
    logE(LOG_TAG, "umtsBand = %d", result[1]);


    // lte
    hexIntStr = line->atTokNextstr(&err);
    if (err < 0) goto error;
    if (hexIntStr == NULL) goto error;
    len = strlen(hexIntStr);
    if (len == 0 || len % 8 != 0) goto error;
    count = len / 8;
    if (count > 8) goto error;
    logE(LOG_TAG, "lteBands = %s", hexIntStr);
    result[totalCount++] = count;
    for ( i =0; i < count; i++) {
        strncpy(charTemp, hexIntStr + i * 8, 8);
        charTemp[8] = '\0';
        result[totalCount++] = strtol(charTemp, NULL, 16);
        logE(LOG_TAG, "lteBand[%d] = %d", i, result[totalCount-1]);
    }

    // NR/SA
    hexIntStr = line->atTokNextstr(&err);
    if (err < 0) goto error;
    if (hexIntStr == NULL) goto error;
    len = strlen(hexIntStr);
    if (len == 0 || len % 8 != 0) goto error;
    count = len / 8;
    if (count > 32) goto error;
    result[totalCount++] = count;
    logE(LOG_TAG, "saBands = %s. len = %d", hexIntStr, len);
    for ( i =0; i < count; i++) {
        strncpy(charTemp, hexIntStr + i * 8, 8);
        charTemp[8] = 0;
        result[totalCount++] = strtol(charTemp, NULL, 16);
        logE(LOG_TAG, "saBands[%d] = %d", i, result[totalCount-1]);
    }

    if (line->atTokHasmore()) {
            // NR/NSA
        hexIntStr = line->atTokNextstr(&err);
        if (err < 0) goto error;
        if (hexIntStr == NULL) goto error;
        len = strlen(hexIntStr);
        if (len == 0 || len % 8 != 0) goto error;
        count = len / 8;
        if (count > 32) goto error;
        result[totalCount++] = count;
        logE(LOG_TAG, "nsaBands = %s. len = %d", hexIntStr, len);
        for ( i =0; i < count; i++) {
            strncpy(charTemp, hexIntStr + i * 8, 8);
            charTemp[8] = 0;
            result[totalCount++] = strtol(charTemp, NULL, 16);
            logE(LOG_TAG, "nsaBands[%d] = %d", i, result[totalCount-1]);
        }
    } else {
        result[totalCount++] = count;
        for ( i = 0; i < count; i ++) {
            result[totalCount] = result[totalCount - count - 1];
            totalCount ++;
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

void RmcNetworkRequestHandler::requestSetNrBandMode(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+EPBSH?
     *   +EPBSEH: <gsm_band>,<umts_band>,<lte_band>,<nr_band>[,<nr_nsa_band>]
     */
    int *pInt = (int *)msg->getData()->getData();
    int index = 0;
    int *saEnabledBands = NULL;
    int *saDisabledBands = NULL;
    int *nsaEnabledBands = NULL;
    int *nsaDisabledBands = NULL;
    sp<RfxMclMessage> response;
    RfxAtLine* line = NULL;
    char *gsmHexStr, *umtsHexStr, *lteHexStr, *saHexStr, *nsaHexStr;
    sp<RfxAtResponse> p_response;
    int saBands[32] = {0};
    int nsaBands[32] = {0};
    int err = 0, len = 0, saCount = 0, nsaCount = 0;
    int saEnableBandsCount = 0, saDisableBandsCount = 0, nsaEnableBandsCount = 0, nsaDisableBandsCount = 0;
    char charTemp[9];
    String8 saBandsStr, nsaBandsStr;

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

    p_response = atSendCommandSingleline("AT+EPBSEH?", "+EPBSEH:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // gsm
    gsmHexStr = line->atTokNextstr(&err);
    if (err < 0) goto error;
    logE(LOG_TAG, "gsmBand = %s", gsmHexStr);

    // umts
    umtsHexStr = line->atTokNextstr(&err);
    if (err < 0) goto error;
    logE(LOG_TAG, "umtsBand = %s", umtsHexStr);

    // lte
    lteHexStr = line->atTokNextstr(&err);
    if (err < 0) goto error;

    // NR/SA
    saHexStr = line->atTokNextstr(&err);
    if (err < 0) goto error;
    if (saHexStr == NULL) goto error;
    len = strlen(saHexStr);
    if (len == 0 || len % 8 != 0) goto error;
    saCount = len / 8;
    if (saCount > 32) goto error;
    logE(LOG_TAG, "saBands = %s. len = %d", saHexStr, len);
    for (int i =0; i < saCount; i++) {
        strncpy(charTemp, saHexStr + i * 8, 8);
        charTemp[8] = 0;
        saBands[i] = strtol(charTemp, NULL, 16);
        logE(LOG_TAG, "saBands[%d] = %d", i, saBands[i]);
    }

    if (line->atTokHasmore()) {
            // NR/NSA
        nsaHexStr = line->atTokNextstr(&err);
        if (err < 0) goto error;
        if (nsaHexStr == NULL) goto error;
        len = strlen(nsaHexStr);
        if (len == 0 || len % 8 != 0) goto error;
        nsaCount = len / 8;
        if (nsaCount > 32) goto error;
        logE(LOG_TAG, "nsaBands = %s. len = %d", nsaHexStr, len);
        for (int i = 0; i < nsaCount; i++) {
            strncpy(charTemp, nsaHexStr + i * 8, 8);
            charTemp[8] = 0;
            nsaBands[i] = strtol(charTemp, NULL, 16);
            logE(LOG_TAG, "nsaBands[%d] = %d", i, nsaBands[i]);
        }
    } else {
        nsaCount = saCount;
        for (int i = 0; i < 32; i++) {
            nsaBands[i] = saBands[i];
        }
        // No nsa band, treat sa band as nsa band
    }
    for (int i = 0; i < saEnableBandsCount; i++) {
        int saEnableIndex = (saEnabledBands[i] - 1) / 32;
        if (saEnableIndex < saCount && saEnabledBands[i] > 0) {
            saBands[saEnableIndex] = saBands[saEnableIndex] | (1 << ((saEnabledBands[i] - 1) % 32));
        }
    }
    for (int i = 0; i < saDisableBandsCount; i++) {
        int saDisableIndex = (saDisabledBands[i] - 1) / 32;
        if (saDisableIndex < saCount && saDisabledBands[i] > 0) {
            saBands[saDisableIndex] = saBands[saDisableIndex] & ~(1 << ((saDisabledBands[i] - 1) % 32));
        }
    }
    for (int i = 0; i < nsaEnableBandsCount; i++) {
        int nsaEnableIndex = (nsaEnabledBands[i] - 1) / 32;
        if (nsaEnableIndex < nsaCount && nsaEnabledBands[i] > 0) {
            nsaBands[nsaEnableIndex] = nsaBands[nsaEnableIndex] | (1 << ((nsaEnabledBands[i] - 1) % 32));
        }
    }
    for (int i = 0; i < nsaDisableBandsCount; i++) {
        int nsaDisableIndex = (nsaDisabledBands[i] - 1) / 32;
        if (nsaDisableIndex < nsaCount && nsaDisabledBands[i] > 0) {
            nsaBands[nsaDisableIndex] = nsaBands[nsaDisableIndex] & ~(1 << ((nsaDisabledBands[i] - 1) % 32));
        }
    }

    for (int i = 0; i < saCount; i++) {
        if (i == 0) {
            saBandsStr = String8::format("%08X", saBands[i]);
        } else {
            saBandsStr = String8::format("%s%08X", saBandsStr.string(), saBands[i]);
        }
    }
    for (int i = 0; i < nsaCount; i++) {
        if (i == 0) {
            nsaBandsStr = String8::format("%08X", nsaBands[i]);
        } else {
            nsaBandsStr = String8::format("%s%08X", nsaBandsStr.string(), nsaBands[i]);
        }
    }
    logE(LOG_TAG, "saBandStr[%s], nsaBandsStr[%s]", saBandsStr.string(), nsaBandsStr.string());
    /** AT+EPBSEH: <gsm_band>,<umts_band>,<lte_band>,<nr_band>[,<is_nvram_save_needed>,
     *             <nr_nsa_band>,<is_rf_control_needed>]
     */
    p_response = atSendCommand(String8::format("AT+EPBSEH=,,,\"%s\",,\"%s\"", saBandsStr.string(), nsaBandsStr.string()));
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
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
    logE(LOG_TAG, "There is something wrong with AT+EPBSEH");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    if (saEnabledBands) free(saEnabledBands);
    if (saDisabledBands) free(saDisabledBands);
    if (nsaEnabledBands) free(nsaEnabledBands);
    if (nsaDisabledBands) free(nsaDisabledBands);
    return;
}


void RmcNetworkRequestHandler::requestGetAllBandModeWithoutNr(const sp<RfxMclMessage>& msg) {

    /**
     *   AT+EPBSE?
     *   +EPBSE: <gsm_band>,<umts_band><lte_band_1_32>,<lte_band_33_64>,<lte_band...>,...
     */
    int err = 0;
    // gsm->  umts->lteCount-> lte;
    int result[1+1+1+8] = {0};   //LTE max count is 8
    int LteCount = 0;
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    p_response = atSendCommandSingleline("AT+EPBSE?", "+EPBSE:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    // gsm_band
    result[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // umts_band
    result[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    LteCount = 0;
    result[2] = 0;  //result[2] to record the count of LTEs
    while (line->atTokHasmore()) {
        //from result[3] to record lte items
        result[3 + LteCount] = line->atTokNextint(&err);
        if (err < 0) break;
        LteCount ++;
        result[2] ++;
        if (result[2] == 8) {
            break;   //LTE max count is 8
        }
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxIntsData(result, LteCount + 3), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong with AT+EPBSE");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
    return;
}

void RmcNetworkRequestHandler::requestSetUsageSetting(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+CEUS= <setting>
     *    0: voice centric
     *    1: data centric
     */
    RIL_Errno err = RIL_E_INVALID_ARGUMENTS;
    int setting;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

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
    p_response = atSendCommand(String8::format("AT+CEUS=%d", setting));
    if (p_response->getError() < 0 || p_response->getSuccess() == 0) {
        err = RIL_E_MODE_NOT_SUPPORTED;
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

void RmcNetworkRequestHandler::requestGetUsageSetting(const sp<RfxMclMessage>& msg) {
    /**
     *   AT+CEUS?
     *   +CEUS: <setting>
     *    0: voice centric
     *    1: data centric
     */
    int err = 0, setting;
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;

    p_response = atSendCommandSingleline("AT+CEUS?", "+CEUS:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

   // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    setting = line->atTokNextint(&err);
    if (err < 0) goto error;

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

void RmcNetworkRequestHandler::requestSetN1ModeEnabled(const sp<RfxMclMessage>& msg) {

    RIL_Errno err = RIL_E_INVALID_ARGUMENTS;
    int enabled, nrOption;
    sp<RfxMclMessage> response;

    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "%s: dataLen(%d) is wrong", __FUNCTION__, dataLen);
        goto error;
    }
    if (!isNrSupported()) {
        err = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }
    enabled = pInt[0];
    if (enabled != 1 && enabled != 0) goto error;
    logD(LOG_TAG, "%s: enabled = %d", __FUNCTION__, enabled);

    nrOption = getNrOption();
    if (nrOption < 0) goto error;
    nrOption = (enabled == 1) ? (nrOption | NR_OPT_SA) : (nrOption & (NR_OPT_LTE | NR_OPT_NSA));
    if(setNrOption(nrOption) < 0) {
        err = RIL_E_GENERIC_FAILURE;
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

void RmcNetworkRequestHandler::requestGetN1ModeEnabled(const sp<RfxMclMessage>& msg) {

    int nrOption, saEnabled;
    sp<RfxMclMessage> response;
    RIL_Errno err = RIL_E_GENERIC_FAILURE;
    if (!isNrSupported()) {
        err = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }
    nrOption = getNrOption();
    if (nrOption < 0) goto error;
    saEnabled = ((nrOption & NR_OPT_SA) == NR_OPT_SA) ? 1 : 0;
    logV(LOG_TAG, "%s: nrOption = %d", __FUNCTION__, nrOption);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxIntsData(&saEnabled, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "There is something wrong in %s", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), err,
                RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestSetNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg) {
    // TODO: Not support on Gen97, to avoid VTS fail, return eror code RIL_E_MODEM_ERR
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestIsNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg) {
    // TODO: Not Support on Gen97, to avoid VTS fail, return eror code RIL_E_MODEM_ERR
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
                RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmcNetworkRequestHandler::requestIsCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg) {
    // TODO: Not support
    int enabled = 0;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
            RfxIntsData(&enabled, 1), msg, false);
    responseToTelCore(response);
}
void RmcNetworkRequestHandler::requestSetCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg) {
    // TODO: Not support
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}
void RmcNetworkRequestHandler::requestSetSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg) {
    // TODO: Not support
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}
void RmcNetworkRequestHandler::requestIsSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg) {
    // TODO: Not support
    int enabled = 0;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
            RfxIntsData(&enabled, 1), msg, false);
    responseToTelCore(response);
}
