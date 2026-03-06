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

#include "RmmNwURCHandler.h"
#include "libmtkrilutils.h"
#include <compiler/compiler_utils.h>
#include <telephony/mtk_ril.h>
#include "RfxCellInfoData.h"
#include "RfxCidData.h"
#include "RfxMipcData.h"
#include "RfxSignalStrengthData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxViaUtils.h"
#include "ViaBaseHandler.h"
#include "RfxNetworkScanResultData.h"
#include "RfxRegistrationFailedData.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include <cutils/memory.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmmNwUrcHdlr"
#define PROPERTY_WFC_STATE "persist.vendor.radio.wfc_state"
#define PROPERTY_SAU_ENABLED    "persist.vendor.radio.sau_enabled"

RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_CS_NETWORK_STATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_PS_NETWORK_STATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_WFC_STATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_FEMTOCELL_UPDATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_UNSOLICITED_RESPONSE_FILTER_CHANGED);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_PHYSICAL_CHANNEL_CONFIGS_MTK);
RFX_REGISTER_DATA_TO_URC_ID(RfxNetworkScanResultData, RFX_MSG_URC_NETWORK_SCAN_RESULT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NETWORK_EVENT);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_NETWORK_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_MODULATION_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SMART_RAT_SWITCH_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_CDMA_OTA_PROVISION_STATUS);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_CDMA_PRL_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxBarringInfoData, RFX_MSG_URC_BARRING_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxCellInfoData, RFX_MSG_URC_CELL_INFO_LIST);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_PSEUDO_CELL_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_MCCMNC_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_NITZ_TIME_RECEIVED);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_SIB16_TIME_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_RESPONSE_CS_NETWORK_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIGNAL_STRENGTH_WITH_WCDMA_ECIO);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxRegistrationFailedData, RFX_MSG_URC_REGISTRATION_FAILED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIGNAL_STRENGTH);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_IWLAN_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NETWORK_BAND_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NR_SYS_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_5GUW_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NR_CA_BAND_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NW_CFG_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_RRC_STATE);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_TOE_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxCidData, RFX_MSG_URC_CELLULAR_IDENTIFIER_DISCLOSED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SECURITY_ALGORITHMS_UPDATED);

RFX_IMPLEMENT_HANDLER_CLASS(RmmNwURCHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED);

RmmNwURCHandler::RmmNwURCHandler(int slot_id, int channel_id) :
        RmmNwHandler(slot_id, channel_id) {

    const int urc[] = {
        MIPC_NW_NRUC_INFO_IND,
        MIPC_NW_NRUW_INFO_IND,
        MIPC_NW_ECELL_IND,
        MIPC_NW_EREGINFO_IND,
        MIPC_NW_NETWORK_INFO_IND,
        MIPC_NW_NETWORK_SCAN_IND,
        MIPC_NW_CSCON_IND,
        MIPC_NW_NR_SWITCH_IND,
        MIPC_NW_OTACMSG_IND,
        MIPC_NW_BARRING_INFO_IND,
        MIPC_NW_NITZ_IND,
        MIPC_NW_CS_IND,
        MIPC_NW_PS_IND,
        // MIPC_NW_IWLAN_IND,
        MIPC_NW_SIGNAL_IND,
        MIPC_NW_CIEV_IND,
        MIPC_NW_CAMP_STATE_IND,
        MIPC_NW_CURRENT_RAT_IND,
        MIPC_NW_EGMSS_IND,
        MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND,
        MIPC_NW_PSEUDO_CELL_IND,
        MIPC_IMS_SUPPORT_ECC_IND,
        MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND,
        MIPC_NW_CELLULAR_ID_DISCLOSURE_IND,
        MIPC_NW_FIRST_PLMN_IND
    };

    logD(LOG_TAG, "RmmNwURCHandler[%d] start", slot_id);
    registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));

    /* Enable EMCCMNC URC */
    if (supportMccMncUrc) {
        const int urcMccMnc[] = {
            MIPC_NW_MCCMNC_IND,
        };
        registerToHandleMipcURC(urcMccMnc, 1);
    }

    /* Enable EMODCFG URC */
    if (isEnableModulationReport()) {
        const int urcModulationInfo[] = {
            MIPC_NW_EMODCFG_IND,
        };
        registerToHandleMipcURC(urcModulationInfo, 1);
    }

    if (isFemtocellSupport()) {
        const int urcFemtoCell[] = {
            MIPC_NW_FEMTOCELL_INFO_IND,
        };
        registerToHandleMipcURC(urcFemtoCell, 1);
    }

    if (sUrcNrsysinfoEnable) {
        const int urcNrScs[] = {
            MIPC_NW_NR_SCS_IND,
        };
        registerToHandleMipcURC(urcNrScs, 1);
    }

    if (sUrcChInfoEnable) {
        const int urcChInfo[] = {
            MIPC_NW_CH_INFO_IND,
        };
        registerToHandleMipcURC(urcChInfo, 1);
    }

    if (enableCaInfoUrc) {
        const int urcCaInfo[] = {
            MIPC_NW_CA_INFO_IND,
        };
        registerToHandleMipcURC(urcCaInfo, 1);
    }

    if (sUrcNrCaBandEnable) {
        const int urcNrCaBand[] = {
            MIPC_NW_NR_CA_BAND_IND,
        };
        registerToHandleMipcURC(urcNrCaBand, 1);
    }

    if (sUrcNwCfgInfoEnable) {
        const int urcNwCfgInfo[] = {
            MIPC_NW_ENWCFGINFO_IND,
        };
        registerToHandleMipcURC(urcNwCfgInfo, 1);
    }

    const int urcSau[] = {
        MIPC_NW_SECURITY_ALGOS_IND
    };
    char sau_enabled[RFX_PROPERTY_VALUE_MAX] = {0};
    getMSimProperty(m_slot_id, (char*) PROPERTY_SAU_ENABLED, sau_enabled);
    if (atoi(sau_enabled) > 0) {
        registerToHandleMipcURC(urcSau, sizeof(urcSau)/sizeof(int));
    }

    const int request[] = {
        RFX_MSG_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED,
        RFX_MSG_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED,
        RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER
    };

    registerToHandleRequest(request, sizeof(request) / sizeof(int));

    if (s_iwlan_in_legacy_mode) {
        setMSimProperty(m_slot_id, (char*)PROPERTY_WFC_STATE, (char*)"0");
    }
}

RmmNwURCHandler::~RmmNwURCHandler() {
}

void RmmNwURCHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    uint16_t msgId = msg->getMipcData()->getMipcMsgId();
    logV(LOG_TAG, "[%s]: msgId:%d", __FUNCTION__, msgId);
    switch (msgId) {
        case MIPC_NW_NITZ_IND:
            handleNitzTzInd(msg);
            break;

        case MIPC_NW_CS_IND:
            handleCsInd(msg);
            break;
        case MIPC_NW_CURRENT_RAT_IND:
            handleCurrentRatInd(msg);
            break;
        case MIPC_NW_EGMSS_IND:
            handleEgmssInd(msg);
            break;

        case MIPC_NW_PS_IND:
            handlePsInd(msg);
            break;

        case MIPC_NW_ECELL_IND:
            handleEcellInd(msg);
            break;

        case MIPC_NW_PSEUDO_CELL_IND:
            handlePseudoCellInd(msg);
            break;

        case MIPC_IMS_SUPPORT_ECC_IND:
            handleImsEmergencySupportR9(msg);
            break;

        case MIPC_NW_CAMP_STATE_IND:
            handleEcampuiChanged(msg);
            break;

        case MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND:
            handlePhysicalChannelConfigs(msg);
            break;

        case MIPC_NW_SIGNAL_IND:
            handleSignalStrength(msg);
            break;

        case MIPC_NW_EMODCFG_IND:
            handleModulationInfoInd(msg);
            break;

        case MIPC_NW_EREGINFO_IND:
            handleNetworkEventInd(msg);
            break;

        case MIPC_NW_NETWORK_INFO_IND:
            handleNetworkInfoInd(msg);
            break;

        case MIPC_NW_CA_INFO_IND:
            handleCaInfoInd(msg);
            break;

        case MIPC_NW_NETWORK_SCAN_IND:
            handleNetworkScanInd(msg);
            break;

        case MIPC_NW_CSCON_IND:
            handleConnectionStateReportInd(msg);
            break;

        case MIPC_NW_NR_SWITCH_IND:
            handleNrSwitchInd(msg);
            break;

        case MIPC_NW_OTACMSG_IND:
            handleOtaProvisionStatusInd(msg);
            break;

        case MIPC_NW_BARRING_INFO_IND:
            handleBarringInfoInd(msg);
            break;

        case MIPC_NW_CIEV_IND:
            handleCievInd(msg);
            break;

        case MIPC_NW_FEMTOCELL_INFO_IND:
            handleFemtocellInfoInd(msg);
            break;

        case MIPC_NW_MCCMNC_IND:
            handleMccMncChanged(msg);
            break;

        case MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND:
            handleEmergencyBearerSupportInd(msg);
            break;

        case MIPC_NW_CH_INFO_IND:
            handleChannelInfoInd(msg);
            break;

        case MIPC_NW_NR_CA_BAND_IND:
            handleNrCaBandInfoInd(msg);
            break;

        case MIPC_NW_NR_SCS_IND:
            handleNrSysInfoChanged(msg);
            break;

        case MIPC_NW_IWLAN_IND:
            handleIWlanRegStateChanged(msg);
            break;

        case MIPC_NW_NRUW_INFO_IND:
            handle5GUWChanged(msg);
            break;

        case MIPC_NW_NRUC_INFO_IND:
            handle5GUCChanged(msg);
            break;

        case MIPC_NW_FIRST_PLMN_IND:
            handleFirstPlmnChanged(msg);
            break;

        case MIPC_NW_ENWCFGINFO_IND:
            handleNwCfgInfo(msg);
            break;

        case MIPC_NW_CELLULAR_ID_DISCLOSURE_IND:
            handleCellularIdentifierDisclosure(msg);
            break;

        case MIPC_NW_SECURITY_ALGOS_IND:
            handleSecurityAlgorithmsUpdated(msg);
            break;

        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}


void RmmNwURCHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    logV(LOG_TAG, "onHandleRequest:%s", RFX_ID_TO_STR(request));
    switch (request) {
        case RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER:
            requestSetUnsolicitedResponseFilter(msg);
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

void RmmNwURCHandler::handleNitzTzInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    mipc_nw_nitz_info_struct4* pNitzInfo;
    uint16_t len = 0;
    uint8_t type = msg->getMipcData()->getMipcUint8Val(MIPC_NW_NITZ_IND_T_TYPE, 0);
    void * ptr = msg->getMipcData()->getMipcVal(MIPC_NW_NITZ_IND_T_INFO, (uint16_t *) &len);
    if ( !ptr || len < sizeof(mipc_nw_nitz_info_struct4)) {
        logE(LOG_TAG, "unkown nitz IND, ptr= %p, len = %d", ptr, len);
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
        logV(LOG_TAG, "NITZ:%s", nitzString);
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NITZ_TIME_RECEIVED, msg->getSlotId(),
                RfxStringData(nitzString, (strlen(nitzString)+1)));
        responseToTelCore(urc);
        if (type == MIPC_NITZ_IND_TYPE_CIEV) {
            urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIB16_TIME_INFO_IND, msg->getSlotId(),
                    RfxStringData(nitzString, (strlen(nitzString)+1)));
            responseToTelCore(urc);
        }

        free(nitzString);
    }
}

void RmmNwURCHandler::handleCsInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    uint16_t len = 0;
    mipc_nw_reg_change_info_struct4* pInfo =
        (mipc_nw_reg_change_info_struct4*)(msg->getMipcData()->getMipcVal(MIPC_NW_CS_IND_T_INFO, &len));
    int tmpState[4];
    if (pInfo && len == sizeof(mipc_nw_reg_change_info_struct4)) {
        RIL_CS_REG_STATE_CACHE* pCache = sCsRegStateCache[m_slot_id];

        // notify AP that AT+ECAMPUI info should no longer be used.
        if (mReceivedEcampUI) {
            sendDisableEcampuiChanged();
        }

        pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
        tmpState[0] = convertRegState(pCache->registration_state, true);
        tmpState[1] = convertCSNetworkType(pCache->radio_technology);
        pCache->registration_state = pInfo->stat;
        pCache->lac = (pInfo->lac_tac <= 0x7FFFFFFF) ? pInfo->lac_tac : 0x7FFFFFFF;
        pCache->cid = pInfo->cell_id;
        pCache->radio_technology = pInfo->eact;
        //pCache->css = pInfo->
        pCache->roaming_indicator = pInfo->roam_indicator;
        //pCache->is_in_prl = pInfo->
        //pCache->default_roaming_indicator = pInfo->
        pCache->denied_reason = pInfo->cause_type;
        pCache->network_exist = pInfo->nw_existence;
        pCache->cause_type = pInfo->cause_type;
        pCache->reject_cause = pInfo->reject_cause;
        //pInfo->rac;
        pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
        dumpCsRegStateCache(__FUNCTION__, pCache);

#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        handleCsIndMisc(tmpState[0], tmpState[1]);
#else
        char *responseStr[6] = { 0 };
        asprintf(&responseStr[0], "%d", convertRegState(pInfo->stat, true));
        asprintf(&responseStr[1], "%x", pCache->lac);
        asprintf(&responseStr[2], "%lx", pCache->cid);
        asprintf(&responseStr[3], "%d", convertCSNetworkType(pInfo->eact));
        asprintf(&responseStr[4], "%d", pInfo->reject_cause);
        asprintf(&responseStr[5], "%d", pInfo->nw_existence);
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
                RFX_MSG_URC_RESPONSE_CS_NETWORK_STATE_CHANGED,
                m_slot_id, RfxStringsData(responseStr, 6));
        responseToTelCore(urc);

        isNeedNotifyStateChanged();

        for (int i = 0; i < 6; i++) {
            freeSetNull(responseStr[i]);
        }

        tmpState[2] = convertRegState(pInfo->stat, true);
        tmpState[3] = convertCSNetworkType(pInfo->eact);
        sendEvent(RFX_MSG_EVENT_CS_NETWORK_STATE, RfxIntsData(tmpState, 4),
                  RIL_CMD_PROXY_3, m_slot_id);
        if (pInfo->stat == 3) sendRegistrationFailedEvent(RIL_DOMAIN_CS);
#endif
    } else {
        logE(LOG_TAG, "[%s] invalid CS IND, pInfo:%p, len:%d, struct len:%d",
             __FUNCTION__, pInfo, (int)len, (int)sizeof(mipc_nw_reg_change_info_struct4));
    }
}

void RmmNwURCHandler::handleCurrentRatInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    int data[3] = { 0 };
    data[0] = msg->getMipcData()->getMipcUint8Val(MIPC_NW_CURRENT_RAT_IND_T_CURRENT_RAT, 0);
    data[1] = msg->getMipcData()->getMipcUint32Val(MIPC_NW_CURRENT_RAT_IND_T_CURRENT_BIT_RAT, 0);
    data[2] = msg->getMipcData()->getMipcUint8Val(MIPC_NW_CURRENT_RAT_IND_T_PREFER_RAT, 0);
    sendEvent(RFX_MSG_EVENT_CURRENT_RAT_CHANGED,
              RfxIntsData(data, 3), RIL_CMD_PROXY_9, m_slot_id);
}

void RmmNwURCHandler::handleEgmssInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    int data[5] = { 0 };
    data[0] = msg->getMipcData()->getMipcUint32Val(MIPC_NW_EGMSS_IND_T_RAT, 0);
    data[1] = msg->getMipcData()->getMipcUint32Val(MIPC_NW_EGMSS_IND_T_MCC, 0);
    data[2] = msg->getMipcData()->getMipcUint32Val(MIPC_NW_EGMSS_IND_T_STATUS, 0);
    data[3] = msg->getMipcData()->getMipcUint32Val(MIPC_NW_EGMSS_IND_T_CUR_REPORTED_RAT, 0);
    data[4] = msg->getMipcData()->getMipcUint32Val(MIPC_NW_EGMSS_IND_T_IS_HOME_COUNTRY, 0);
    sendEvent(RFX_MSG_EVENT_EGMSS_INFO_CHANGED,
              RfxIntsData(data, 5), RIL_CMD_PROXY_9, m_slot_id);
}

void RmmNwURCHandler::handlePsInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    uint16_t len = 0;
    mipc_nw_ps_reg_info_struct4* pInfo =
        (mipc_nw_ps_reg_info_struct4*)(msg->getMipcData()->getMipcVal(MIPC_NW_PS_IND_T_REG_INFO, &len));
    int tmpState[4];
    int upperlayerIndR15Available = 0;
    int info[2] = { 0 };
    sp<RfxMclMessage> urc;
    if (pInfo && len == sizeof(mipc_nw_ps_reg_info_struct4)) {
        RIL_PS_REG_STATE_CACHE* pCache = sPsRegStateCache[m_slot_id];

        // sendEvent(RFX_MSG_EVENT_EMBMS_POST_NETWORK_UPDATE, RfxStringData(urc_str, strlen(urc_str)),
        //           RIL_CMD_PROXY_EMBMS, msg->getSlotId());

        // notify AP that AT+ECAMPUI info should no longer be used.
        if (mReceivedEcampUI) {
            sendDisableEcampuiChanged();
        }

        pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
        tmpState[0] = convertRegState(pCache->registration_state, false);
        tmpState[1] = convertPSNetworkType(pCache->radio_technology);

        pCache->registration_state = pInfo->stat;
        pCache->lac = (pInfo->lac_tac <= 0x7FFFFFFF) ? pInfo->lac_tac : 0x7FFFFFFF;
        pCache->cid = pInfo->cell_id;
        pCache->radio_technology = pInfo->rat;
        pCache->denied_reason = pInfo->reason_for_denial;
        pCache->max_simultaneous_data_call = pInfo->max_data_calls;
        // NR none-standalone
        pCache->dcnr_restricted = pInfo->is_dcnr_restricted;
        pCache->endc_sib = pInfo->cache_endc_connect; // pInfo->is_nr_available is original value
        pCache->endc_available = pInfo->is_endc_available;
        // pCache->cause_type =
        pCache->reject_cause = pInfo->reason_for_denial;
        pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);
        dumpPsRegStateCache(__FUNCTION__, pCache);
#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        handlePsIndMisc(tmpState[0], tmpState[1]);
#else
        int wfcState = getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);
        tmpState[2] = (wfcState == 1) ? 1 : convertRegState(pInfo->stat, false);
        tmpState[3] = (wfcState == 1) ? RADIO_TECH_IWLAN : convertPSNetworkType(pInfo->rat);
        sendEvent(RFX_MSG_EVENT_PS_NETWORK_STATE, RfxIntsData(tmpState, 4),
                  RIL_CMD_PROXY_3, m_slot_id);

        if (convertRegState(pInfo->stat, false) != tmpState[0]) {
            //perhaps physical channel config not change, but  ps change, need notify.
            sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(), RIL_CMD_PROXY_3, m_slot_id);
        }

        if (isInService(convertRegState(pInfo->stat, false)) || isInService(tmpState[0])) {
            isNeedNotifyStateChanged();
        }

        // keep scg state
        if (isInService(convertRegState(pInfo->stat, false)) &&  // in service
            (pCache->radio_technology == 0x4000)) { // nsa
            scg_connection_state[m_slot_id] = 1;
        } else {
            scg_connection_state[m_slot_id] = 0;
        }

        if (pInfo->stat == 3) sendRegistrationFailedEvent(RIL_DOMAIN_PS);
#endif
        if (sUrcNrsysinfoEnable) {
            pthread_mutex_lock(&sNrSysInfoMutex[m_slot_id]);
            logV(LOG_TAG, "[%s] current NR sys info:%d, %d, endc_sib = %d", __FUNCTION__,
                sNrSysInfo[m_slot_id]->upperlayerIndR15Available,
                sNrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz, pInfo->is_nr_available);
            upperlayerIndR15Available = (pInfo->is_nr_available == 2) ? 1 : 0;
            if (sNrSysInfo[m_slot_id]->upperlayerIndR15Available != upperlayerIndR15Available) {
                sNrSysInfo[m_slot_id]->upperlayerIndR15Available = upperlayerIndR15Available;
                info[0] = sNrSysInfo[m_slot_id]->upperlayerIndR15Available;
                info[1] = sNrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz;
                urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_SYS_INFO, m_slot_id,
                                           RfxIntsData(info, 2));
                responseToTelCore(urc);
            }
            pthread_mutex_unlock(&sNrSysInfoMutex[m_slot_id]);
        }
    } else {
        logE(LOG_TAG, "[%s] invalid PS IND, pInfo:%p, len:%d, struct len:%d",
             __FUNCTION__, pInfo, (int)len, (int)sizeof(mipc_nw_ps_reg_info_struct4));
    }
}

void RmmNwURCHandler::sendDisableEcampuiChanged() {
    int response[7] ={0};
    /* response[0]
     * 0: out of service
     * 1: 1 in service
     * 4: notify AP to disable fake state as long as RILD gets EREG/EGREG.
     */
    response[0] = 4;
    mReceivedEcampUI = false;
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED,
                                                     m_slot_id, RfxIntsData(response, 7));
    responseToTelCore(urc);
}

void RmmNwURCHandler::handleImsEmergencySupportR9(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    int rat = msg->getMipcData()->getMipcInt8Val(MIPC_IMS_SUPPORT_ECC_IND_T_RAT, 0);
    int ecc = msg->getMipcData()->getMipcInt8Val(MIPC_IMS_SUPPORT_ECC_IND_T_SUPPORT_EMC, 0);
    if ((rat == 3 || rat == 4) && (ecc == 1)) {
        sImsEccState[m_slot_id] = true;
    } else {
        sImsEccState[m_slot_id] = false;
    }
    logD(LOG_TAG, "[%s]: rat:%d, ecc:%d", __FUNCTION__, rat, ecc);
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_EMERGENCY_SUPPORT_STATE,
            sImsEccState[m_slot_id]? 1: 0);
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(urc);
}

void RmmNwURCHandler::handleEcampuiChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    // format: +ECAMPUI: <reg>, <eAct>, <PLMN>, <lac>, <dcnr_restricted>, <endc_sib>, <endc_available>
    int response[7] = { 0 };
    uint16_t len = 0;
    sp<RfxMclMessage> urc;
    char* plmn;
    mipc_nw_ps_reg_info_struct4* pInfo =
        (mipc_nw_ps_reg_info_struct4*)(msg->getMipcData()->getMipcVal(MIPC_NW_CAMP_STATE_IND_T_REG_INFO, &len));
    logV(LOG_TAG, "[%s] enter", __FUNCTION__);
    if (pInfo && len == sizeof(mipc_nw_ps_reg_info_struct4)) {
        response[0] = pInfo->stat;                      // reg state
        response[1] = convertCSNetworkType(pInfo->rat); // eact
        response[3] = pInfo->lac_tac;                   // lac
        response[4] = pInfo->is_dcnr_restricted;        // dcnr_restricted
        response[5] = pInfo->cache_endc_connect;
        response[6] = pInfo->is_endc_available;
        logV(LOG_TAG, "[%s] stat:%d, eact:%d, dcnr_restricted:%d,nr_available:%d,endc_available:%d",
             __FUNCTION__, pInfo->stat, pInfo->rat,
             pInfo->is_dcnr_restricted, pInfo->cache_endc_connect, pInfo->is_endc_available);
    } else {
        logE(LOG_TAG, "[%s] failed, pInfo:%p, len:%d, struct len:%d",
             __FUNCTION__, pInfo, (int)len, (int)sizeof(mipc_nw_ps_reg_info_struct4));
        goto error;
    }

    plmn = msg->getMipcData()->getMipcStr(MIPC_NW_CAMP_STATE_IND_T_PLMN_ID);
    if (plmn != NULL && plmn[0] != '\0') {
        response[2] = atoi(plmn);
    } else {
        goto error;
    }

    mReceivedEcampUI = true;
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED,
                                   m_slot_id, RfxIntsData(response, 7));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "[%s] unknown error", __FUNCTION__);
}

void RmmNwURCHandler::handlePhysicalChannelConfigs(const sp<RfxMclMessage>& msg) {
    if (!sPhysicalConfigSwitch[m_slot_id]) return;  // It's off.
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    bool hasChanged = false;
    RIL_PhysicalChannelConfig* pCache = sPhysicalConfigCache[m_slot_id];

    uint16_t count = 0;
    count = mipc_msg_get_tlv_num(msg->getMipcData()->getData(),
        MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_LIST_V1);

    if (count > MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM) count = MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM;

    pthread_mutex_lock(&sPhysicalConfigMutex[m_slot_id]);
    for (int i = count; i < sPhysicalConfigCount[m_slot_id]; i++) {
        memset(&pCache[i], 0, sizeof(RIL_PhysicalChannelConfig));
    }
    if (sPhysicalConfigCount[m_slot_id] != count) {
        hasChanged = true;
        sPhysicalConfigCount[m_slot_id] = count;
    }

    if (count > 0) {
        mipc_physical_channel_info_v1_struct4* pInfo = NULL;
        uint16_t len = 0;

        for (int i = 0; i < count; i++) {
            pInfo = mipc_nw_physical_channel_configs_ind_get_physical_ch_info_list_v1(
                        msg->getMipcData()->getData(), &len, i);
            if (pInfo == NULL) {
                logE(LOG_TAG, "[%s] index :%d get null", __FUNCTION__, i);
                continue;
            }
            if (len != sizeof(mipc_physical_channel_info_v1_struct4)) {
                logE(LOG_TAG, "[%s] len :%u is not match sizeof(mipc_physical_channel_info_v1_struct4): %lu",
                      __FUNCTION__, len, sizeof(mipc_physical_channel_info_v1_struct4));
                continue;
            }

            if (!hasChanged) {
                int tempRat = (RIL_RadioTechnology)convertPSNetworkType(pInfo->rat);
                if (pCache[i].rat != tempRat) {
                    if (!((tempRat == RADIO_TECH_LTE_CA) && (pCache[i].rat == RADIO_TECH_LTE))) {
                        hasChanged = true;
                    }
                }
            }

            if (!hasChanged) {
                if (pCache[i].status != (RIL_CellConnectionStatus)pInfo->status
                    || pCache[i].cellBandwidthDownlink != pInfo->cell_bandwidth_downlink
                    || pCache[i].cellBandwidthUplink != pInfo->cell_bandwidth_uplink
                    || pCache[i].downlinkChannelNumber != pInfo->downlink_channel_number
                    || pCache[i].uplinkChannelNumber != pInfo->uplink_channel_number
                    || pCache[i].band != pInfo->band
                    || pCache[i].physicalCellId != pInfo->physical_cell_id) {
                    hasChanged = true;
                }
            }

            pCache[i].status = (RIL_CellConnectionStatus)pInfo->status;
            pCache[i].rat = (RIL_RadioTechnology)convertPSNetworkType(pInfo->rat);
            pCache[i].cellBandwidthDownlink = pInfo->cell_bandwidth_downlink;
            pCache[i].cellBandwidthUplink = pInfo->cell_bandwidth_uplink;
            pCache[i].downlinkChannelNumber = pInfo->downlink_channel_number;
            pCache[i].uplinkChannelNumber = pInfo->uplink_channel_number;
            pCache[i].band = pInfo->band;
            pCache[i].physicalCellId = pInfo->physical_cell_id;

            if (pCache[i].rat == RADIO_TECH_LTE_CA)
                pCache[i].rat = RADIO_TECH_LTE;

            char* phyCid = getMask(pInfo->physical_cell_id);
            logV(LOG_TAG, "[%s]hasChanged %d, pCache[%d].rat:%d, pInfo[%d].rat:%u, pInfo->status:%u, pInfo->cell_bandwidth_downlink:%d,"
                        "pInfo->cell_bandwidth_uplink:%d, pInfo->downlink_channel_number:%d, "
                        "pInfo->uplink_channel_number:%d, pInfo->physical_cell_id:%s, pInfo->band:%u",
                        __FUNCTION__, hasChanged, i, pCache[i].rat, i, pInfo->rat, pInfo->status, pInfo->cell_bandwidth_downlink,
                        pInfo->cell_bandwidth_uplink, pInfo->downlink_channel_number, pInfo->uplink_channel_number,
                        phyCid, pInfo->band);
            if (phyCid) free(phyCid);
        }
    }

    logD(LOG_TAG, "[%s] count:%u, hasChanged: %d ", __FUNCTION__, count, hasChanged);
    if (hasChanged) {
        sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(),
                  RIL_CMD_PROXY_3, m_slot_id);
    }
    pthread_mutex_unlock(&sPhysicalConfigMutex[m_slot_id]);
}

void RmmNwURCHandler::sendRegistrationFailedEvent(RIL_Domain domain) {
    logV(LOG_TAG, "[%s] enter", __FUNCTION__);
    if (sRegFailNotificationSwitch[m_slot_id]) {
        sendEvent(RFX_MSG_EVENT_REGISTRATION_FAILED,
                  RfxIntsData((int*)&domain, 1), RIL_CMD_PROXY_3, m_slot_id);
    }
}

void RmmNwURCHandler::handleSignalStrength(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    int signalType = msg->getMipcData()->getMipcUint8Val(MIPC_NW_SIGNAL_IND_T_SIGNAL_TYPE, 0xFF);
    RIL_SIGNAL_STRENGTH *signalCache = signal_strength_cache[m_slot_id];
    uint16_t len = 0;
    int sigUpdated = SIGNAL_GROUP_NONE;
    int sigRegisted = SIGNAL_GROUP_NONE;
    int activeGroup = SIGNAL_GROUP_NONE;
    RIL_SIGNAL_STRENGTH resp = {};
    sp<RfxMclMessage> urc;

    logV(LOG_TAG, "[%s] enter, signalType:%d", __FUNCTION__, signalType);
    pthread_mutex_lock(&s_signalStrengthMutex[m_slot_id]);
    switch (signalType) {
    case  MIPC_NW_SIGNAL_TYPE_GSM: {
        mipc_nw_gsm_signal_strength_struct4* pGsmSignal =
            (mipc_nw_gsm_signal_strength_struct4*)msg->getMipcData()->getMipcVal(MIPC_NW_SIGNAL_IND_T_GSM_SIGNAL, &len);
        if (pGsmSignal && len == sizeof(mipc_nw_gsm_signal_strength_struct4)) {
            signalCache->gsm_signal_strength = pGsmSignal->signal_strength;
            signalCache->gsm_bit_error_rate = pGsmSignal->bit_error_rate;
            signalCache->gsm_timing_advance = pGsmSignal->timing_advance;
            sigUpdated |= SIGNAL_GROUP_GSM;
        } else {
            logE(LOG_TAG, "[%s] failed, pGsmSignal:%p, len:%d, struct len:%d",
                 __FUNCTION__, pGsmSignal, (int)len, (int)sizeof(mipc_nw_gsm_signal_strength_struct4));
        }
        break;
    }

    case MIPC_NW_SIGNAL_TYPE_UMTS: {
        mipc_nw_umts_signal_strength_struct4* pUmtsSignal =
            (mipc_nw_umts_signal_strength_struct4*)msg->getMipcData()->getMipcVal(MIPC_NW_SIGNAL_IND_T_UMTS_SIGNAL, &len);
        if (pUmtsSignal && len == sizeof(mipc_nw_umts_signal_strength_struct4)) {
            signalCache->wcdma_signal_strength = pUmtsSignal->signal_strength;
            signalCache->wcdma_bit_error_rate = pUmtsSignal->bit_error_rate;
            signalCache->wcdma_scdma_rscp = pUmtsSignal->rscp;
            signalCache->wcdma_ecno = pUmtsSignal->ecno;
            sigUpdated |= SIGNAL_GROUP_UMTS;
        } else {
            logE(LOG_TAG, "[%s] failed, pUmtsSignal:%p, len:%d, struct len:%d",
                 __FUNCTION__, pUmtsSignal, (int)len, (int)sizeof(mipc_nw_umts_signal_strength_struct4));
        }
        break;
    }

    case MIPC_NW_SIGNAL_TYPE_LTE:
    case MIPC_NW_SIGNAL_TYPE_NR_NSA: {
        mipc_nw_lte_signal_strength_struct4* pLteSignal =
            (mipc_nw_lte_signal_strength_struct4*)msg->getMipcData()->getMipcVal(MIPC_NW_SIGNAL_IND_T_LTE_SIGNAL, &len);
        if (pLteSignal && len == sizeof(mipc_nw_lte_signal_strength_struct4)) {
            signalCache->lte_signal_strength = pLteSignal->signal_strength;
            signalCache->lte_rsrp = pLteSignal->rsrp;
            signalCache->lte_rsrq = pLteSignal->rsrq;
            signalCache->lte_rssnr = pLteSignal->rssnr;
            signalCache->lte_cqi = pLteSignal->cqi;
            signalCache->lte_timing_advance = pLteSignal->timing_advance;
            signalCache->lte_cqi_table_index = INT_MAX;
            sigUpdated |= SIGNAL_GROUP_LTE;
        } else {
            logE(LOG_TAG, "[%s] failed, pLteSignal:%p, len:%d, struct len:%d",
                 __FUNCTION__, pLteSignal, (int)len, (int)sizeof(mipc_nw_lte_signal_strength_struct4));
        }
        //extends CQI Table Index
        mipc_nw_lte_signal_strength_v1_struct4* pLteSignalV1 =
            mipc_nw_signal_ind_get_lte_signal_v1(msg->getMipcData()->getData(), &len);
        if (pLteSignalV1 != NULL) {
            signalCache->lte_cqi_table_index = pLteSignalV1->cqi_table_index;
        }

        if (signalType == MIPC_NW_SIGNAL_TYPE_LTE) {
            break;
        }
        // else fallthrough, NR_NSA has LTE & NR signal
        TELEPHONYWARE_FALLTHROUGH;
    }

    case MIPC_NW_SIGNAL_TYPE_NR: {
        mipc_nw_nr_signal_strength_struct4* pNrSignal =
            (mipc_nw_nr_signal_strength_struct4*)msg->getMipcData()->getMipcVal(MIPC_NW_SIGNAL_IND_T_NR_SIGNAL, &len);
        if (pNrSignal && len == sizeof(mipc_nw_nr_signal_strength_struct4)) {
            signalCache->ssRsrp = pNrSignal->ss_rsrp;
            signalCache->ssRsrq = pNrSignal->ss_rsrq;
            signalCache->ssSinr = pNrSignal->ss_sinr;
            signalCache->csiRsrp = pNrSignal->csi_rsrp;
            signalCache->csiRsrq = pNrSignal->csi_rsrq;
            signalCache->csiSinr = pNrSignal->csi_sinr;
            signalCache->nr_csicqi_table_index = INT_MAX;
            signalCache->nr_csicqi_report_count = 0;
            signalCache->nr_timing_advance = 0;
            sigUpdated |= SIGNAL_GROUP_NR;
        } else {
            logE(LOG_TAG, "[%s] failed, pNrSignal:%p, len:%d, struct len:%d",
                 __FUNCTION__, pNrSignal, (int)len, (int)sizeof(mipc_nw_nr_signal_strength_struct4));
        }
        //extends CQI Table Index
        mipc_nw_nr_signal_strength_v1_struct4 *pNrSignalV1 =
                mipc_nw_signal_ind_get_nr_signal_v1(msg->getMipcData()->getData(), &len);
        if (pNrSignalV1 != NULL) {
            signalCache->nr_csicqi_table_index = pNrSignalV1->csi_cqi_table_index;
            if (pNrSignalV1 ->csi_cqi_report_num > 0) {
                signalCache->nr_csicqi_report_count =
                        MIN(pNrSignalV1 ->csi_cqi_report_num, MAX_COUNT_NR_CSI_CQI_REPORT);
                for (int index = 0; index < signalCache->nr_csicqi_report_count; index ++) {
                    signalCache->nr_csicqi_reports[index] = pNrSignalV1->csi_cqi_report_list[index];
                }
            }
        }

        //extends nr timing_advance by mipc_nw_nr_signal_strength_v1_struct4
        mipc_nw_nr_signal_strength_v2_struct4 *pNrSignalV2 =
                mipc_nw_signal_ind_get_nr_signal_v2(msg->getMipcData()->getData(), &len);
        if (pNrSignalV2 != NULL) {
            signalCache->nr_timing_advance = pNrSignalV2->timing_advance;
        }
        break;
    }

    case MIPC_NW_SIGNAL_TYPE_CDMA: {
        mipc_nw_cdma_signal_strength_struct4* pCdmaSignal =
            (mipc_nw_cdma_signal_strength_struct4*)msg->getMipcData()->getMipcVal(MIPC_NW_SIGNAL_IND_T_CDMA_SIGNAL, &len);
        if (pCdmaSignal && len == sizeof(mipc_nw_cdma_signal_strength_struct4)) {
            if (pCdmaSignal->snr == CELLINFO_INVALID) { // 1X
                signalCache->cdma_dbm = pCdmaSignal->dbm;
                signalCache->cdma_ecio = pCdmaSignal->ecio;
                sigUpdated |= SIGNAL_GROUP_1XRTT;
            } else { // evdo
                signalCache->evdo_dbm = pCdmaSignal->dbm;
                signalCache->evdo_ecio = pCdmaSignal->ecio;
                signalCache->evdo_snr = pCdmaSignal->snr;
                sigUpdated |= SIGNAL_GROUP_EVDO;
            }
        } else {
            logE(LOG_TAG, "[%s] failed, pCdmaSignal:%p, len:%d, struct len:%d",
                 __FUNCTION__, pCdmaSignal, (int)len, (int)sizeof(mipc_nw_cdma_signal_strength_struct4));
        }
        break;
    }

    default:
        break;
    }

    if (sigUpdated == SIGNAL_GROUP_NONE) {
        // Either receive invalid or same signal strength as before
        goto done;
    }

    sigRegisted = getCurrentSignalGroup();
    logD(LOG_TAG, "[%s]:sigRegisted[%#04X], sigUpdated[%#04X]",
         __FUNCTION__, sigRegisted, sigUpdated);
    printSignalStrengthCache((char*) __FUNCTION__);

    activeGroup = sigUpdated & sigRegisted;
    if (sigRegisted == SIGNAL_GROUP_NONE) {
        // TODO: OOS case only report URC changed signal strength?
        fillSignalStrengthResponse((RIL_SIGNAL_STRENGTH *)&resp, sigUpdated);
    } else if (activeGroup != SIGNAL_GROUP_NONE) {
        fillSignalStrengthResponse((RIL_SIGNAL_STRENGTH *)&resp, sigRegisted);
    } else {
        // TODO: changed URC not match any of current network, report nothing?
        goto done;
    }
    pthread_mutex_unlock(&s_signalStrengthMutex[m_slot_id]);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIGNAL_STRENGTH, m_slot_id,
           RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)));
    responseToTelCore(urc);

    if (enableReportSignalStrengthWithWcdmaEcio()) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIGNAL_STRENGTH_WITH_WCDMA_ECIO, m_slot_id,
                RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)));
        responseToTelCore(urc);
    }

    return;

done:
    pthread_mutex_unlock(&s_signalStrengthMutex[m_slot_id]);
}

void RmmNwURCHandler::handleEcellInd(const sp<RfxMclMessage>& msg) {
    int num = 0, err = 0;
    RIL_CellInfo_v12 * response;
    sp<RfxMclMessage> urc;
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "handleEcellInd, mipcData is NULL");
        return;
    }
    // total num
    num = mipcData->getMipcUint32Val(MIPC_NW_ECELL_IND_T_GSM_CELL_COUNT, 0);
    num += mipcData->getMipcUint32Val(MIPC_NW_ECELL_IND_T_CDMA_CELL_COUNT, 0);
    num += mipcData->getMipcUint32Val(MIPC_NW_ECELL_IND_T_UMTS_CELL_COUNT, 0);
    num += mipcData->getMipcUint32Val(MIPC_NW_ECELL_IND_T_LTE_CELL_COUNT, 0);
    num += mipcData->getMipcUint32Val(MIPC_NW_ECELL_IND_T_NR_CELL_COUNT, 0);
    logD(LOG_TAG, "handleEcellInd +++++, num=%d", num);
    if (num > 0) {
        RIL_CELl_INFO_TLV_KEYS keys = {
            MIPC_NW_ECELL_IND_T_GSM_CELL_COUNT,
            MIPC_NW_ECELL_IND_T_GSM_CELL_LIST,
            MIPC_NW_ECELL_IND_T_CDMA_CELL_COUNT,
            MIPC_NW_ECELL_IND_T_CDMA_CELL_LIST,
            MIPC_NW_ECELL_IND_T_UMTS_CELL_COUNT,
            MIPC_NW_ECELL_IND_T_UMTS_CELL_LIST,
            MIPC_NW_ECELL_IND_T_LTE_CELL_COUNT,
            MIPC_NW_ECELL_IND_T_LTE_CELL_LIST,
            MIPC_NW_ECELL_IND_T_NR_CELL_COUNT,
            MIPC_NW_ECELL_IND_T_NR_CELL_LIST,
            MIPC_NW_ECELL_IND_T_LTE_CELL_LIST_V1,
            MIPC_NW_ECELL_IND_T_NR_CELL_LIST_V1
        };
        response = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
        memset(response, 0, num * sizeof(RIL_CellInfo_v12));
        err = getCellInfoListV12(mipcData, response, keys);
        if (err == 0) {
            urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CELL_INFO_LIST,
                m_slot_id, RfxCellInfoData(response, num * sizeof(RIL_CellInfo_v12)));
            responseToTelCore(urc);
        }
        freeOperatorNameOfCellInfo(num, response);
    }
    return;
}

void RmmNwURCHandler::requestSetUnsolicitedResponseFilter(const sp<RfxMclMessage>& msg) {
    /************************************
    * Control indication
    *************************************/

    RIL_UnsolicitedResponseFilter filter;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_UnsolicitedResponseFilter *pUnsolicitedResponseFilter
            = (RIL_UnsolicitedResponseFilter *)msg->getData()->getData();

    filter = pUnsolicitedResponseFilter[0];
    setUnsolResponseFilterDataDomain(filter);

    if ((filter & RIL_UR_SIGNAL_STRENGTH) == RIL_UR_SIGNAL_STRENGTH) {
        // enable
        setUnsolResponseFilterSignalStrength(true);
        requestSignalStrength(NULL, true);
        //other mipc indication
#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        setUnsolResponseFilterForAll(true);
#endif //MTK_HYP_DUAL_TELE_SUPPORT
    } else {
        // disable
        setUnsolResponseFilterSignalStrength(false);
        //other mipc indication
#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        setUnsolResponseFilterForAll(false);
#endif //MTK_HYP_DUAL_TELE_SUPPORT
    }
    if ((filter & RIL_UR_FULL_NETWORK_STATE) == RIL_UR_FULL_NETWORK_STATE) {
        // enable
        setUnsolResponseFilterNetworkState(true);
#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        updataPsIndicaton();
        updataCsIndicaton();
#endif //MTK_HYP_DUAL_TELE_SUPPORT
    } else {
        // disable
        setUnsolResponseFilterNetworkState(false);
    }

    if ((filter & PHYSICAL_CHANNEL_CONFIG) == PHYSICAL_CHANNEL_CONFIG) {
        // enable
        sPhysicalConfigSwitch[m_slot_id] = true;
        setUnsolResponseFilterPhyChlCfg(true);
    } else {
        // disable
        sPhysicalConfigSwitch[m_slot_id] = false;
        setUnsolResponseFilterPhyChlCfg(false);
    }

    if ((filter & REGISTRATION_FAILURE) == REGISTRATION_FAILURE) {
        // enable
        sRegFailNotificationSwitch[m_slot_id] = true;
    } else {
        // disable
        sRegFailNotificationSwitch[m_slot_id] = false;
    }
    if ((filter & BARRING_INFO) == BARRING_INFO) {
        // enable
        setUnsolResponseFilterBarringInfo(true);
    } else {
        // disable
        setUnsolResponseFilterBarringInfo(false);
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmmNwURCHandler::handlePseudoCellInd(const sp<RfxMclMessage>& msg) {
    /*
     * 1. URC when EAPC mode is 1
     *    +EAPC: <num>,<type>,<plmn>,<lac>,<cid>,<arfcn>,<bsic>[
     *                ,<type>,<plmn>,<lac>,<cid>,<arfcn>,<bsic>]
     *     num: 1 or 2
     *   type = 1: detected pseudo cells info
     *
     * 2. URC when EAPC mode is 2
     *  +EAPC: <num>,<type>[,<plmn>,<lac>,<cid>,<arfcn>,<bsic>]
     *   num: 1
     *   type = 2: In to a pseudo cell
     *   type = 3: Out of a pseudo cell
     */

    int response[13] = {0};
    RfxMipcData* mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    sp<RfxMclMessage> urc;
    uint8_t num;
    num = mipcData->getMipcUint8Val(MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_CELL_COUNT, 0);
    response[0] = num;
    if (num != 1 && num != 2) goto error;

    if (num > 0) {
        uint16_t length = 0;
        void * cellData = mipcData->getMipcVal(
                MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_LIST, &length);
        if (cellData == NULL || length != num * sizeof(mipc_nw_pseudocell_info_struct4)) {
            logE(LOG_TAG, "handleGetPseudoCellInfoReponse, length(%d) is wrong", length);
            goto error;
        }
        mipc_nw_pseudocell_info_struct4 * list = (mipc_nw_pseudocell_info_struct4 *) cellData;
        for (int i = 0; i < num; i++) {
            char* lac_s = getMask(list->lac);
            char* cid_s = getMask((unsigned long long)(list->cell_id));
            char* bsic_s = getMask(list->bsic);
            logV(LOG_TAG, "[%s]: type = %d, plmn = %s, lac = %s, cid = %s, arfcn = %d, bsic = %s",
                    __FUNCTION__, list->type, list->plmn, lac_s, cid_s, list->arfcn, bsic_s);
            if (lac_s) free(lac_s);
            if (cid_s) free(cid_s);
            if (bsic_s) free(bsic_s);
            response[i*6 + 1] = list->type;
            if (response[i*6 + 1] == 2) {
            logD(LOG_TAG, "PseudoCellInfo: attached to a pseudo cell");
            } else if (response[i*6 + 1] == 3) {
                logD(LOG_TAG, "PseudoCellInfo: attached cell changed and is not a pseudo cell");
            }
            response[i*6 + 2] = atoi(list->plmn);
            response[i*6 + 3] = list->lac;
            response[i*6 + 4] = (int) list->cell_id;
            response[i*6 + 5] = list->arfcn;
            response[i*6 + 6] = list->bsic;
            list ++;
        }
    }
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_PSEUDO_CELL_INFO,
            m_slot_id, RfxIntsData(response, 13));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "handle Pseudo Cell Info error: num = %d", num);
    return;
}

void RmmNwURCHandler::handleModulationInfoInd(const sp<RfxMclMessage>& msg) {
    int mode = 0;
    sp<RfxMclMessage> urc;
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        goto error;
    }

    mode = mipcData->getMipcUint8Val(MIPC_NW_EMODCFG_IND_T_MODULATION, 0);
    logD(LOG_TAG, "handleModulationInfoInd: %d", mode);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_MODULATION_INFO, m_slot_id,
            RfxIntsData(&mode, 1));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the handleModulationInfoInd");
}

void RmmNwURCHandler::handleNetworkEventInd(const sp<RfxMclMessage>& msg) {
    int response[2] = {0};
    sp<RfxMclMessage> urc;
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        goto error;
    }

    response[0] = mipcData->getMipcUint8Val(MIPC_NW_EREGINFO_IND_T_ACT, 0);
    response[1] = mipcData->getMipcUint8Val(MIPC_NW_EREGINFO_IND_T_EVENT_TYPE, 0);

    logD(LOG_TAG, "handleNetworkEventInd: <Act>:%d, <event_typ>:%d", response[0], response[1]);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_EVENT, m_slot_id, RfxIntsData(response, 2));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the handleNetworkEventInd");
}

void RmmNwURCHandler::handleNetworkInfoInd(const sp<RfxMclMessage>& msg) {
    int type = 0;
    char *responseStr[2];
    sp<RfxMclMessage> urc;
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        goto error;
    }

    type = mipcData->getMipcUint16Val(MIPC_NW_NETWORK_INFO_IND_T_TYPE, 0);

    asprintf(&responseStr[0], "%d", type);

    responseStr[1] = mipcData->getMipcStr(MIPC_NW_NETWORK_INFO_IND_T_NW_INFO);

    logD(LOG_TAG, "handleNetworkInfoInd: %s, len:%zu ,%s", responseStr[0], strlen(responseStr[1]),
            responseStr[1]);


    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_INFO,
        m_slot_id, RfxStringsData(responseStr, 2));
    responseToTelCore(urc);
    free(responseStr[0]);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the handleNetworkInfoInd");
}

void RmmNwURCHandler::handleCaInfoInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_nw_ecainfo_struct4* pCaInfo = NULL;
    uint16_t len = 0;
    int ca_info = 0;
    int pcell_bw = 0, scell_bw1 = 0, scell_bw2 = 0, scell_bw3 = 0, scell_bw4 = 0;

    if (mipcData == NULL) {
        goto error;
    }

    pCaInfo =
        (mipc_nw_ecainfo_struct4*)mipcData->getMipcVal(MIPC_NW_CA_INFO_IND_T_CAINFO, &len);

    if (pCaInfo && len == sizeof(mipc_nw_ecainfo_struct4)) {
        ca_info = pCaInfo->ca_info;
        pcell_bw = pCaInfo->pcell_bw;
        scell_bw1 = pCaInfo->scell_bw1;
        scell_bw2 = pCaInfo->scell_bw2;
        scell_bw3 = pCaInfo->scell_bw3;
        scell_bw4 = pCaInfo->scell_bw4;
    } else {
        logE(LOG_TAG, "[%s] failed, pCaInfo:%p, len:%d, struct len:%d",
             __FUNCTION__, pCaInfo, (int)len, (int)sizeof(mipc_nw_cscon_status_struct4));
        goto error;
    }

    logD(LOG_TAG, "handleCaInfoInd: %d, %d, %d, %d, %d, %d", ca_info, pcell_bw,
        scell_bw1, scell_bw2, scell_bw3, scell_bw4);
    // just trigger poll service state here, CA cache supposed to be updated then.
    isNeedNotifyStateChanged();
    return;

error:
    logE(LOG_TAG, "There is something wrong with the handleCaInfoInd");
}

void RmmNwURCHandler::handleNetworkScanInd(const sp<RfxMclMessage>& msg) {
    /*
    +ENWSCN: <scan_status>,<num_record>
    [,<rat>,<xarfcn>,<pcid>,"<plmn_id>","<cell_id>","<lac_or_tac>",<plmn_status>,<registered>,<srv_cell_status>,<sig_1>,<sig_2>,<sig_3>,<sig_4>,<sig_5>][,...]
    */
    bool isSuccess = false;
    int scan_status = COMPLETE, num_record = 0,
        rat = 0, xarfcn = 0, pcid = 0, lac_tac = 0,
        plmn_status = 0, registered = 0, srv_cell_status = 0, timestamp_type = 0, timestamp = 0,
        sig1 = 0, sig2 = 0, sig3 = 0, sig4 = 0, sig5 = 0, bit_error_rat = 99, timing_advance = 0, cqi = 0;
    char* numeric_oper = NULL;
    unsigned long long cellid = 0;

    int aosp_gsm_sig = 0;

    int mcc = 0, mnc = 0, len = 0, mnc_len = 0, index = 0;
    char mccmnc[8] = {0};
    char longname[MAX_OPER_NAME_LENGTH] = {0};
    char shortname[MAX_OPER_NAME_LENGTH] = {0};

    sp<RfxMclMessage> urc;
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_nw_scan_info_struct4 *pRecordInfo = NULL;
    uint16_t read = 0;

    if (!isNetworkScanFullSupport()) {
        return;
    }

    if (mipcData == NULL) {
        logE(LOG_TAG, "handleNetworkScanInd mipcData = null");
        return;
    }

    // <scan_status>
    scan_status = (int)mipcData->getMipcUint32Val(MIPC_NW_NETWORK_SCAN_IND_T_SCAN_STATUS, COMPLETE);

    //allocate memeroy
    RIL_NetworkScanResult* result = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
    if (result == NULL) {
        logE(LOG_TAG, "handleNetworkScanInd result = null");
        return;
    }
    memset(result, 0, sizeof(RIL_NetworkScanResult));

    /* init result */
    result->network_infos_length = 0;

    // <num_record>
    num_record = (int)mipc_msg_get_tlv_num(mipcData->getData(), MIPC_NW_NETWORK_SCAN_IND_T_RECORD_LIST);

    result->network_infos = (RIL_CellInfo_v12*) calloc(num_record, sizeof(RIL_CellInfo_v12));
    if (result->network_infos == NULL) {
        logE(LOG_TAG, "handleNetworkScanInd: result->network_infos = null");
        goto error;
    }
    memset(result->network_infos, 0, num_record*sizeof(RIL_CellInfo_v12));

    for (int i = 0; i < num_record ; i++) {
        pRecordInfo = (mipc_nw_scan_info_struct4 *)mipc_msg_get_idx_ptr(mipcData->getData(),
            MIPC_NW_NETWORK_SCAN_IND_T_RECORD_LIST, (uint16_t)i, &read);
        if (pRecordInfo == NULL) {
            logE(LOG_TAG, "[%s] index :%d get null", __FUNCTION__, i);
            continue;
        }
        if (read != sizeof(mipc_nw_scan_info_struct4)) {
            logE(LOG_TAG, "[%s] read len :%d is not match sizeof(mipc_nw_scan_info_struct4): %d",
                  __FUNCTION__, (int)read, (int)sizeof(mipc_nw_scan_info_struct4));
            continue;
        }
        if (pRecordInfo->rat == 128
            && 0xfffffffe == strtol(pRecordInfo->lac_or_tac, NULL, 16)) {
            logD(LOG_TAG, "Skip this NSA's NR cell");
            continue;
        }

        // clear the tmp value for this round.
        memset(mccmnc, 0, 8);
        mcc = 0;
        mnc = 0;
        len = 0;
        mnc_len = 0;
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);

        /* <rat> 1 - GSM; 2 - UMTS; 4 - LTE; 128 - NR */
        rat = pRecordInfo->rat;

        /* <xarfcn> ARFCN for 2G; UARFCN for 3G; EARFCN for 4G; NR-ARFCN for 5G */
        xarfcn = pRecordInfo->xarfcn;

        /* <pcid> */
        pcid = pRecordInfo->pcid;

        /* <plmn_id> */
        numeric_oper = pRecordInfo->plmn_id;

        /* <cell_id> */
        cellid = strtoull(pRecordInfo->cell_id, NULL, 16);

        /* <lac_or_tac> */
        lac_tac = (int)strtol(pRecordInfo->lac_or_tac, NULL, 16);

        /* <plmn_status> 0 - Unknown; 1 - Available; 2 - Current; 3 - Forbidden */
        plmn_status = pRecordInfo->plmn_status;

        /* <registered> 0 - not registered; 1 - registered */
        registered = pRecordInfo->registered;

        /* <srv_cell_status> 0 - None; 1 - Primary Serving Cell; 2 - Secondary serving cell */
        srv_cell_status = pRecordInfo->srv_cell_status;

        /* <sig1>
         * For GSM - (0-63, 99) Based on 27.007 8.69
         * For WCDMA - (0-31, 99) Based on 27.007 8.5
         * For TDSCDMA - (25 - 120) based on (dbm x -1)
         * For LTE - (0-31, 99) Based on 27.007 8.5
         * For NR - (0-255) Based on 27.007 8.69 <ss_rsrq>
         */
        sig1 = pRecordInfo->sig1;

        /* <sig2>
         * For GSM/WCDMA/LTE - RSSI in qdbM
         * For TDSCDMA - RSCP in qbdm
         * For NR - (0-255) Based on 27.007 8.69 <ss_rsrp>
         */
        sig2 = pRecordInfo->sig2;

        /* <sig3>
         * RSCP in qdbM for WCDMA
         * RSRP in qdbM for LTE
         * SS-RSRQ in qdb for NR
         */
        sig3 = pRecordInfo->sig3;

        /* <sig4>
         * ECNO in qdbM for WCDMA
         * RSRQ in qdbM for LTE
         * SS-RSRP in qdbm for NR
         */
        sig4 = pRecordInfo->sig4;

        /* <sig5>
         * RSSNR in qdbM for LTE
         * SS-SINR in qdb for NR
         */
        sig5 = pRecordInfo->sig5;

        char* s_lac_tac = NULL, *s_cellid = NULL, *s_bsic_psc_cpid_pci = NULL;
        s_lac_tac = getMask(lac_tac);
        s_cellid = getMask(cellid);
        s_bsic_psc_cpid_pci = getMask(pcid);
        if (!(s_lac_tac == NULL || s_cellid == NULL || s_bsic_psc_cpid_pci == NULL)) {
            logD(LOG_TAG, "%d: %d,%d,%s,%s,%s,%s,%d,%d,%d,%d,"
                "%d,%d,%d,%d,%d,%d,%d,%d,%d",
                i, rat, xarfcn, s_bsic_psc_cpid_pci, (numeric_oper==NULL?"":numeric_oper), s_cellid, s_lac_tac,
                plmn_status, registered, srv_cell_status, timestamp_type,
                timestamp, sig1, sig2, sig3, sig4, sig5, bit_error_rat, timing_advance, cqi);
        }
        if (s_lac_tac) free(s_lac_tac);
        if (s_cellid) free(s_cellid);
        if (s_bsic_psc_cpid_pci) free(s_bsic_psc_cpid_pci);
        /* parse end */

        len = strlen(numeric_oper);
        if (len == 5) {
            mnc_len = 2;
            mcc = (numeric_oper[0]-'0') * 100 + (numeric_oper[1]-'0') * 10 + (numeric_oper[2]-'0');
            mnc = (numeric_oper[3]-'0') * 10 + (numeric_oper[4]-'0');
        } else if (len == 6) {
            mnc_len = 3;
            mcc = (numeric_oper[0]-'0') * 100 + (numeric_oper[1]-'0') * 10 + (numeric_oper[2]-'0');
            mnc = (numeric_oper[3]-'0') * 100 + (numeric_oper[4]-'0') * 10 + (numeric_oper[5]-'0');
        } else {
            logE(LOG_TAG, "The length of the numeric code is incorrect");
            goto error;
        }

        /* start to fill the result */
        if (srv_cell_status == 1) {
            result->network_infos[index].connectionStatus = PRIMARY_SERVING;
        } else {
            result->network_infos[index].connectionStatus = NONE_SERVING;
        }

        if (registered == 1) {
            result->network_infos[index].registered = 1;
        } else {
            result->network_infos[index].registered = 0;
        }

        if (isOp07Support()) {
            matchPlmnName(longname, shortname,
                numeric_oper, lac_tac, (rat == 128) ? 11 : 0, MATCH_EONS|MATCH_TS25);
        } else {
            matchPlmnName(longname, shortname, numeric_oper, lac_tac, (rat == 128) ? 11 : 0);
        }
        if (strlen(longname) == 0) {
            strncpy(longname, numeric_oper, MAX_OPER_NAME_LENGTH);
            longname[MAX_OPER_NAME_LENGTH - 1] = '\0';
        }
        if (strlen(shortname) == 0) {
            strncpy(shortname, numeric_oper, MAX_OPER_NAME_LENGTH);
            shortname[MAX_OPER_NAME_LENGTH - 1] = '\0';
        }

        switch(rat) {
        case 1:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mcc = mcc;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mnc = mnc;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.lac = lac_tac;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.cid = (int) cellid;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.arfcn = xarfcn;
            result->network_infos[index].CellInfo.gsm.cellIdentityGsm.bsic =
                    pcid == INT_MAX ? 0xFF : pcid;
            asprintf(&(result->network_infos[index].CellInfo.gsm.cellIdentityGsm.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.gsm.cellIdentityGsm.operName.short_name),
                    "%s", shortname);
            aosp_gsm_sig = (sig1 >> 1) + 1; // (0~31)
            if (aosp_gsm_sig > 31) aosp_gsm_sig = 31;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.signalStrength =
                    sig1 == INT_MAX ? 99 : aosp_gsm_sig;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.bitErrorRate =
                    bit_error_rat;
            result->network_infos[index].CellInfo.gsm.signalStrengthGsm.timingAdvance = timing_advance;
            break;
        case 2:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mcc = mcc;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mnc = mnc;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.lac = lac_tac;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.cid = (int) cellid;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.psc = pcid;
            result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.uarfcn = xarfcn;
            asprintf(&(result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.wcdma.cellIdentityWcdma.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.signalStrength =
                    sig1 == INT_MAX ? 99 : sig1;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate =
                    bit_error_rat;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.rscp =
                    (sig3 == INT_MAX || sig3 == 32767) ? 255 : ((sig3>>2) + 120) ;
            result->network_infos[index].CellInfo.wcdma.signalStrengthWcdma.ecno =
                    sig4 == INT_MAX ? 255 : (49 + (sig4>>2));
            break;
        case 4:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_LTE;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mcc = mcc;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mnc = mnc;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.tac = lac_tac;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.ci = (int) cellid;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.pci = pcid;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.earfcn = xarfcn;
            result->network_infos[index].CellInfo.lte.cellIdentityLte.bandwidth = 0;
            asprintf(&(result->network_infos[index].CellInfo.lte.cellIdentityLte.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.lte.cellIdentityLte.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.lte.signalStrengthLte.signalStrength =
                    sig1 == INT_MAX ? 99 : sig1;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rsrp =
                    sig3 == 32767 ? INT_MAX : (sig3>>2)*-1;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rsrq =
                    sig4 == INT_MAX ? INT_MAX : (sig4>>2)*-1;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.rssnr =
                    sig5 == INT_MAX ? INT_MAX : (sig5>>2)*10;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.cqi = cqi;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.timingAdvance = timing_advance;
            result->network_infos[index].CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;
            pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
            if (result->network_infos[index].registered == 1 &&
                isInService(sPsRegStateCache[m_slot_id]->registration_state) &&
                sPsRegStateCache[m_slot_id]->dcnr_restricted == 0 &&
                sPsRegStateCache[m_slot_id]->endc_sib == 2) {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 1;
            } else {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 0;
            }
            pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);
            break;
        case 128:
            result->network_infos[index].cellInfoType = RIL_CELL_INFO_TYPE_NR;
            result->network_infos[index].CellInfo.nr.cellidentity.mcc = mcc;
            result->network_infos[index].CellInfo.nr.cellidentity.mnc = mnc;
            result->network_infos[index].CellInfo.nr.cellidentity.mnc_len = mnc_len;
            result->network_infos[index].CellInfo.nr.cellidentity.nci =
                    cellid == INT_MAX ? 0xFFFFFFFFFFFFFFFE : (uint64_t) cellid;
            result->network_infos[index].CellInfo.nr.cellidentity.nrarfcn = (int32_t) xarfcn;
            result->network_infos[index].CellInfo.nr.cellidentity.tac = (int32_t) lac_tac;
            result->network_infos[index].CellInfo.nr.cellidentity.pci = (uint32_t) pcid;
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrp = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrq = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiSinr =  INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiTableIndex = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiReportCount = 0;
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrp =
                    sig3 == INT_MAX ? INT_MAX : (sig3>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrq =
                    sig4 == INT_MAX ? INT_MAX : (sig4>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.ssSinr =
                    sig5 == INT_MAX ? INT_MAX : (sig5>>2)*-1;
            break;
        default:
            goto error;
        }
        index+=1;
        result->network_infos_length+=1;
    }

    result->status = (scan_status == 1) ? COMPLETE : PARTIAL;

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
            m_slot_id, RfxNetworkScanResultData((void*) result, sizeof(RIL_NetworkScanResult)));
    // response to TeleCore
    responseToTelCore(urc);

    isSuccess = true;

    // release any resource, it should be as the same as RfxNetworkScanResultData's destructor
error:
    if (result->status == COMPLETE) {
        sPlmnListOngoingType= NW_PLMN_LIST_TYPE_NONE;
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    }

    RIL_NetworkScanResult * pData = (RIL_NetworkScanResult *) result;
    releaseNetworkScanResultData(result);
    if (pData->network_infos != NULL) free(pData->network_infos);
    free(pData);

    // If failure happened but scan_status is COMPLETE, we need send complete status only to AP
    if (isSuccess != true && scan_status == 1) {
        RIL_NetworkScanResult* resp = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
        if (resp == NULL) {
            logE(LOG_TAG, "handleNetworkScanInd resp==null");
            sPlmnListAbort = 0;
            return;
        }
        memset(resp, 0, sizeof(RIL_NetworkScanResult));
        resp->status = COMPLETE;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_SCAN_RESULT,
                m_slot_id, RfxNetworkScanResultData((void*) resp, sizeof(RIL_NetworkScanResult)));
        // response to TeleCore
        responseToTelCore(urc);
        free(resp);
    }
    sPlmnListAbort = 0; /* always clear here to prevent race condition scenario */
}

void RmmNwURCHandler::handleConnectionStateReportInd(const sp<RfxMclMessage>& msg) {
    uint8_t mode = 0, state = 0, access = 0;
    uint16_t len = 0;
    sp<RfxMclMessage> urc;
    RfxMipcData* mipcData = msg->getMipcData();
    mipc_nw_cscon_status_struct4* pCsconStatus = NULL;

    if (mipcData == NULL) {
        goto error;
    }

    pCsconStatus =
        (mipc_nw_cscon_status_struct4*)mipcData->getMipcVal(MIPC_NW_CSCON_IND_T_STATUS, &len);
    if (pCsconStatus && len == sizeof(mipc_nw_cscon_status_struct4)) {
        mode = pCsconStatus->mode;
        state = pCsconStatus->state;
        access = pCsconStatus->access;
    } else {
        logE(LOG_TAG, "[%s] failed, pCsconStatus:%p, len:%d, struct len:%d",
             __FUNCTION__, pCsconStatus, (int)len, (int)sizeof(mipc_nw_cscon_status_struct4));
        goto error;
    }

    logD(LOG_TAG, "handleConnectionStateReportInd: %d, %d, %d", mode, state, access);
    handleConnectionStateReportInternal(mode, state, access);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the handleConnectionStateReportInd");
}

void RmmNwURCHandler::handleNrSwitchInd(const sp<RfxMclMessage>& msg) {
    int nrSim = 0;
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        goto error;
    }

    nrSim = mipcData->getMipcUint8Val(MIPC_NW_NR_SWITCH_IND_T_NR_SIM, 0);

    logD(LOG_TAG, "handleNrSwitchInd: %d", nrSim);

    rfx_property_set(PROPERTY_NR_SLOTID, String8::format("%d", (nrSim + 1)).string());
    getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_KEY_NR_SWITCH_SLOT, (nrSim + 1));
    return;

error:
    logE(LOG_TAG, "There is something wrong with the handleNrSwitchInd");
}

void RmmNwURCHandler::handleOtaProvisionStatusInd(const sp<RfxMclMessage>& msg) {
    // <status>: integer type
    //    1 programming started
    //    2 service programming lock unlocked
    //    3 NAM parameters downloaded successfully
    //    4 MDN downloaded successfully
    //    5 IMSI downloaded successfully
    //    6 PRL downloaded successfully
    //    7 commit successfully
    //    8 programming successfully
    //    9 programming unsuccessfully
    //    10 verify SPC failed
    //    11 a key exchanged
    //    12 SSD updated
    //    13 OTAPA started
    //    14 OTAPA stopped

    int otaState;
    int convertState;
    sp<RfxMclMessage> urc;
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        goto error;
    }

    otaState = mipcData->getMipcUint8Val(MIPC_NW_OTACMSG_IND_T_OTA_STATE, 1);

    // for OTA STATUS
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_OTA_STATUS, otaState);

    // for OTA URC
    convertState = convertOtaProvisionStatus(otaState);

    logD(LOG_TAG, "handleOtaProvisionStatusInd: otaState = %d, convertState = %d", otaState, convertState);

    if (convertState >= 0) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CDMA_OTA_PROVISION_STATUS, m_slot_id,
                RfxIntsData(&convertState, 1));
        responseToTelCore(urc);
    }
    return;

error:
    logE(LOG_TAG, "There is something wrong with the handleOtaProvisionStatusInd");
}

int RmmNwURCHandler::convertOtaProvisionStatus(int rawState) {
    int state;
    switch (rawState) {
        case 2: {  // service programming lock unlocked
            state = CDMA_OTA_PROVISION_STATUS_SPL_UNLOCKED;
            break;
        }
        case 3: {  // NAM parameters downloaded successfully
            state = CDMA_OTA_PROVISION_STATUS_NAM_DOWNLOADED;
            break;
        }
        case 4: {  // MDN downloaded successfully
            state = CDMA_OTA_PROVISION_STATUS_MDN_DOWNLOADED;
            break;
        }
        case 5: {  // IMSI downloaded successfully
            state = CDMA_OTA_PROVISION_STATUS_IMSI_DOWNLOADED;
            break;
        }
        case 6: {  // PRL downloaded successfully
            state = CDMA_OTA_PROVISION_STATUS_PRL_DOWNLOADED;
            break;
        }
        case 7: {  // commit successfully
            state = CDMA_OTA_PROVISION_STATUS_COMMITTED;
            break;
        }
        case 10: {  // verify SPC failed
            state = CDMA_OTA_PROVISION_STATUS_SPC_RETRIES_EXCEEDED;
            break;
        }
        case 11: {  // A key Exchanged
            state = CDMA_OTA_PROVISION_STATUS_A_KEY_EXCHANGED;
            break;
        }
        case 12: {  // SSD updated
            state = CDMA_OTA_PROVISION_STATUS_SSD_UPDATED;
            break;
        }
        case 13: {  // OTAPA strated
            state = CDMA_OTA_PROVISION_STATUS_OTAPA_STARTED;
            break;
        }
        case 14: {  // OTAPA stopped
            state = CDMA_OTA_PROVISION_STATUS_OTAPA_STOPPED;
            break;
        }
        default: {
            state = -1;
            break;
        }
    }
    return state;
}

void RmmNwURCHandler::handleBarringInfoInd(const sp<RfxMclMessage>& msg) {
    const int umtsTypes[] = {0, 1, 2, 8}; /*CS_SERVICE, PS_SERVICE, CS_VOICE, EMERGENCY*/
    const int lteTypes[] = {3, 4, 5, 6, 7, 8, 9};
    const int saTypes[] = {3, 4, 5, 6, 7, 8, 9, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
                    1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022,
                    1023, 1024, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032};
    RIL_BarringInforResponse *barringInfoResp =
            (RIL_BarringInforResponse*) calloc(1, sizeof(RIL_BarringInforResponse));
    int count = 0, rat = 0, i, j, number = 0, err = 0;
    const int *pType;
    sp<RfxMclMessage> urc = NULL;
    RfxMipcData* mipcData = msg->getMipcData();
    RIL_BarringInfo *pBarringInfos = NULL;

    if (barringInfoResp == NULL) goto error;
    memset(barringInfoResp, 0, sizeof(RIL_BarringInforResponse));

    if (mipcData == NULL) {
        goto error;
    }

    count = mipcData->getMipcUint8Val(MIPC_NW_BARRING_INFO_IND_T_COUNT, 0);
    rat = mipcData->getMipcUint8Val(MIPC_NW_BARRING_INFO_IND_T_RAT, 0);
    logV(LOG_TAG, "[%s]: count = %d, rat = %d", __FUNCTION__, count, rat);
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
            (mipc_nw_barring_info_struct4*)mipcData->getMipcVal(MIPC_NW_BARRING_INFO_IND_T_BARRING_LIST, &len);

        if (pBarringInfoList == NULL || len != count * sizeof(mipc_nw_barring_info_struct4)) {
            logE(LOG_TAG, "[%s] failed, pBarringInfoList:%p, len:%d, struct len:%d",
                 __FUNCTION__, pBarringInfoList, len, (int)(count * sizeof(mipc_nw_barring_info_struct4)));
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

    err = fillBarringInfosResponse((size_t)number, pBarringInfos, barringInfoResp);
    if (err < 0) goto error;

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_BARRING_INFO, m_slot_id,
            RfxBarringInfoData(barringInfoResp, sizeof(RIL_BarringInforResponse)));
    responseToTelCore(urc);

error:
    if (barringInfoResp) {
        clearCellIdentity(barringInfoResp->cellIdentity);
        free(barringInfoResp);
        barringInfoResp = NULL;
    }
    if (pBarringInfos) {
        free(pBarringInfos);
    }
    logV(LOG_TAG, "[%s], err = %d", __FUNCTION__, err);
    return;
}

void RmmNwURCHandler::setUnsolResponseFilterForAll(bool enable)
{
    const int urc[] = {
        //capabilityswitch
        MIPC_NW_RADIO_CAPABILITY_IND,
        //cat
        MIPC_STK_PAC_IND,
        MIPC_STK_SIM_REFRESH_IND,
        MIPC_STK_BIP_EVENT_NOTIFY_IND,
        //cc
        MIPC_CALL_SS_IND,
        MIPC_CALL_CRSS_IND,
        MIPC_CALL_CIPHER_IND,
        MIPC_CALL_ECBM_CHANGE_IND,
        MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND,
        MIPC_CALL_EXTENDED_DISPLAY_INFO_IND,
        MIPC_CALL_LINE_CONTROL_INFO_IND,
        MIPC_CALL_REDIRECTING_NUMBER_INFO_IND,
        MIPC_SYS_VODATA_STATISTICS_IND,
        MIPC_CALL_ECONF_IND,
        MIPC_CALL_MODE_IND,
        MIPC_CALL_ECT_IND,
        MIPC_CALL_IMS_EVENT_PACKAGE_IND,
        MIPC_CALL_CONFERENCE_IND,
        MIPC_CALL_IMS_SIP_HEADER_IND,
        MIPC_CALL_ECC_REDIAL_IND,
        MIPC_CALL_ECC_LIST_CHANGE_IND,
        //data
        MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND,
        MIPC_DATA_MOBILE_DATA_USAGE_IND,
        MIPC_DATA_NW_LIMIT_IND,
        MIPC_DATA_MOD_PCO_IND,
        MIPC_DATA_DSDA_STATE_IND,
        MIPC_DATA_IWLAN_PRIORITY_LIST_IND,
        MIPC_DATA_RETRY_TIMER_IND,
        MIPC_DATA_URSP_UE_POLICY_CHG_IND,
        MIPC_NW_IA_IND,
        //dch
        MIPC_CALL_DC_CONFIG_INIT_IND,
        MIPC_CALL_DC_CONFIG_UPDATE_IND,
        MIPC_CALL_DC_CONFIG_DEINIT_IND,
        MIPC_DATA_IFUP_PACKET_ROUTE_IND,
        MIPC_CALL_CONNECT_DATA_CHANNEL_IND,
        MIPC_CALL_CLOSE_DATA_CHANNEL_IND,
        MIPC_CALL_CREATE_DATA_CHANNEL_IND,
        MIPC_CALL_SIP_IND,
        MIPC_IMS_UI_IND,
        MIPC_CALL_RTT_AUDIO_IND,
        MIPC_CALL_RTT_CAPABILITY_IND,
        MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND,
        MIPC_CALL_RTT_TEXT_RECEIVE_IND,
        //ims
        MIPC_CALL_UIS_INFO_IND,
        //mwi
        MIPC_WFC_WIFI_PDN_COUNT_IND,
        MIPC_WFC_PDN_HO_IND,
        MIPC_WFC_ROVE_OUT_IND,
        MIPC_WFC_SSAC_IND,
        MIPC_WFC_WIFI_PDN_ERR_IND,
        MIPC_WFC_WIFI_PDN_OOS_IND,
        MIPC_WFC_WFC_IND,
        MIPC_SYS_GEO_LOCATION_IND,
        MIPC_PHB_READY_STATE_IND,
        //SIM
        MIPC_SIM_TEST_SIM_IND,
        MIPC_SIM_CT3G_IND,
        MIPC_SIM_CSIM_IMSI_CHANGE_IND,
        MIPC_SIM_SML_RSU_IND,
        MIPC_SMS_NEW_CBM_IND,
        MIPC_SMS_STORE_STATUS_IND,
        MIPC_SMS_SCBM_IND,
        //SS
        MIPC_SS_USSD_IND,
        MIPC_SS_CFU_IND,
        MIPC_SS_XCAP_RCN_IND,
        MIPC_SS_IMS_XUI_IND,
        //vt
        MIPC_NW_ANBR_IND,
        MIPC_NW_IRAT_IND,
        //netagent
        //MIPC_WFC_GET_MAC_CMD,
    };
    logI(LOG_TAG, "setUnsolResponseFilterForAll enable =%d", enable);
    if (enable) {
        registerToMipcInd(urc, sizeof(urc)/sizeof(int));
    } else {
        unregisterToMipcInd(urc, sizeof(urc)/sizeof(int));
    }
}

void RmmNwURCHandler::setUnsolResponseFilterSignalStrength(bool enable)
{
    const int urc[] = {
        MIPC_NW_SIGNAL_IND,
    };

    if (enable) {
        registerToHandleMipcURC(urc, 1);
    } else {
        unregisterToHandleMipcURC(urc, 1);
    }
}

void RmmNwURCHandler::setUnsolResponseFilterPhyChlCfg(bool enable)
{
    const int urc[] = {
        MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND,
    };

    if (enable) {
        registerToHandleMipcURC(urc, 1);
        queryPhysicalChannelConfigs();
    } else {
        unregisterToHandleMipcURC(urc, 1);
    }
}

void RmmNwURCHandler::setUnsolResponseFilterNetworkState(bool enable)
{
    const int urcCa[] = {
        MIPC_NW_CA_INFO_IND,
    };

    const int urcFemtoCell[] = {
        MIPC_NW_FEMTOCELL_INFO_IND,
    };

    const int urcModulationInfo[] = {
        MIPC_NW_EMODCFG_IND,
    };

    const int urcMccMnc[] = {
        MIPC_NW_MCCMNC_IND,
    };

    const int urcCscon[] = {
        MIPC_NW_CSCON_IND,
    };

    const int urc[] = {
        MIPC_NW_EREGINFO_IND,
        MIPC_NW_CAMP_STATE_IND,
        MIPC_NW_NETWORK_INFO_IND,
        MIPC_NW_ECELL_IND,
#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        //nw
        MIPC_NW_NRUC_INFO_IND,
        MIPC_NW_NRUW_INFO_IND,
        MIPC_NW_NETWORK_SCAN_IND,
        MIPC_NW_NR_SWITCH_IND,
        MIPC_NW_OTACMSG_IND,
        MIPC_NW_CIEV_IND,
        MIPC_NW_EGMSS_IND,
        MIPC_NW_PSEUDO_CELL_IND,
        MIPC_NW_CELLULAR_ID_DISCLOSURE_IND,
        //MIPC_NW_FIRST_PLMN_IND,
        //MIPC_NW_NR_SCS_IND,
        //MIPC_NW_CH_INFO_IND,
        //MIPC_NW_SECURITY_ALGOS_IND,
        MIPC_NW_ETXPWR_IND,
        MIPC_NW_ETXPWRSTUS_IND,
        MIPC_NW_CS_IND,
        MIPC_NW_PS_IND,
#endif //MTK_HYP_DUAL_TELE_SUPPORT
    };

    const int urcNrCaBand[] = {
        MIPC_NW_NR_CA_BAND_IND,
    };

    const int urcNwCfgInfo[] = {
        MIPC_NW_ENWCFGINFO_IND,
    };

    if (enable) {
        // enable; screen is on.
        if (enableCaInfoUrc) {
            /* Enable CA INFO URC */
            registerToHandleMipcURC(urcCa, 1);
        }

        if (isFemtocellSupport()) {
            /* Enable Femtocell Ind */
            registerToHandleMipcURC(urcFemtoCell, 1);
        }

        /* Enable EMODCFG URC */
        if (isEnableModulationReport()) {
            registerToHandleMipcURC(urcModulationInfo, 1);
        }

        registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));

        if (sUrcNrCaBandEnable) {
            registerToHandleMipcURC(urcNrCaBand, 1);
            updateNrCaBandInfo();
        }

        if (sUrcNwCfgInfoEnable) {
            registerToHandleMipcURC(urcNwCfgInfo, 1);
            updateNwCfgInfo();
        }
        /* Enable EMCCMNC URC */
        if (supportMccMncUrc) {
            registerToHandleMipcURC(urcMccMnc, 1);
        }
        if (!keep_rrc_urc) {
            registerToHandleMipcURC(urcCscon, 1);
            updateConnectionStateReport();
        }
    } else {
        // disable; screen is off
        if (enableCaInfoUrc) {
            /* Disable CA INFO URC */
            unregisterToHandleMipcURC(urcCa, 1);
        }

        if (isFemtocellSupport()) {
            /* Disable Femtocell Ind */
            unregisterToHandleMipcURC(urcFemtoCell, 1);
        }

        /* Disable EMODCFG URC */
        if (isEnableModulationReport()) {
            unregisterToHandleMipcURC(urcModulationInfo, 1);
        }

        unregisterToHandleMipcURC(urc, sizeof(urc)/sizeof(int));

        if (sUrcNrCaBandEnable) {
            unregisterToHandleMipcURC(urcNrCaBand, 1);
        }

        if (sUrcNwCfgInfoEnable) {
            unregisterToHandleMipcURC(urcNwCfgInfo, 1);
        }

        /* Disable EMCCMNC URC */
        if (supportMccMncUrc) {
            unregisterToHandleMipcURC(urcMccMnc, 1);
        }

        if (!keep_rrc_urc) {
            /* Disable connection state URC */
            unregisterToHandleMipcURC(urcCscon, 1);
        }
    }

/* when the RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER is received,
 * value = 0: MIPC_NW_CS_IND, MIPC_NW_PS_IND will be unregisted from the URC handler.
 * value > 0: mtktelephony service will request ps and cs status.
*/
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    sp<RfxMipcData> requestData =
        RfxMipcData::obtainMipcData(MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ, m_slot_id);
    requestData->addMipcTlvUint8(MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ_T_CS_STATE_LEVEL, enable?3:5);
    requestData->addMipcTlvUint8(MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ_T_PS_STATE_LEVEL, enable?3:5);

    sp<RfxMipcData> confirmData = callToMipcMsgSync(requestData);
    if (confirmData == NULL) {
        logW(LOG_TAG, "There is something wrong with the exectution of MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ");
    }
#endif
}

void RmmNwURCHandler::setUnsolResponseFilterDataDomain(RIL_UnsolicitedResponseFilter filter) {
    // notify other module about RIL_UnsolicitedResponseFilter.
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_UNSOLICITED_RESPONSE_FILTER_CHANGED, filter);
    sendEvent(RFX_MSG_EVENT_UNSOLICITED_RESPONSE_FILTER_CHANGED, RfxIntsData((int*)&filter, 1),
            RIL_CMD_PROXY_3, m_slot_id);
}

void RmmNwURCHandler::setUnsolResponseFilterBarringInfo(bool enable) {
    const int urc[] = {
        MIPC_NW_BARRING_INFO_IND,
    };

    if (enable) {
        registerToHandleMipcURC(urc, 1);
    } else {
        unregisterToHandleMipcURC(urc, 1);
    }
}

void RmmNwURCHandler::handleCievInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    uint32_t type = msg->getMipcData()->getMipcUint32Val(MIPC_NW_CIEV_IND_T_CIEV_TYPE, 0x7FFFFFFF);
    if (type == 10) {
        char plmn[8], longName[MAX_OPER_NAME_LENGTH] = {0}, shortName[MAX_OPER_NAME_LENGTH] = {0};
        char* pPlmn = msg->getMipcData()->getMipcStr(MIPC_NW_CIEV_IND_T_PLMN_ID);
        char* pLongName = msg->getMipcData()->getMipcStr(MIPC_NW_CIEV_IND_T_NW_NAME_LONG);
        char* pShortName = msg->getMipcData()->getMipcStr(MIPC_NW_CIEV_IND_T_NW_NAME_SHORT);
        if (pPlmn) {
            strlcpy(plmn, pPlmn, 8);
        } else {
            plmn[0] = '\0';
        }

        if (pLongName) {
            strlcpy(longName, pLongName, MAX_OPER_NAME_LENGTH);
            /* ALPS00459516 start */
            int length = strlen(longName);
            if (length > 0 && (length % 8) == 0) {
                logD(LOG_TAG, "strlen(longName)=%d", length);
                if (longName[length - 1] == '@') {
                    longName[length - 1] = '\0';
                    logD(LOG_TAG, "remove @ new longName:%s", longName);
                }
            }
        } else {
            longName[0] = '\0';
        }

        if (pShortName) {
            strlcpy(shortName, pShortName, MAX_OPER_NAME_LENGTH);
            int length = strlen(shortName);
            if (length > 0 && (length % 8) == 0) {
                logD(LOG_TAG, "strlen(shortName)=%d", length);
                if (shortName[length - 1] == '@') {
                    shortName[length - 1] = '\0';
                    logD(LOG_TAG, "remove @ new shortName:%s", shortName);
                }
            }
        } else {
            shortName[0] = '\0';
        }

        rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_CODE, m_slot_id).string(), (char*) "");
        rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_LNAME, m_slot_id).string(), (char*) "");
        rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_SNAME, m_slot_id).string(), (char*) "");
        rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_CODE, m_slot_id).string(), plmn);
        rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_LNAME, m_slot_id).string(), longName);
        rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_SNAME, m_slot_id).string(), shortName);

        //report toe info
        char result[MAX_TOE_BUFFER_LEN] = {0};
        int err = snprintf(result, MAX_TOE_BUFFER_LEN, "%04zu%s%04zu%s%04zu%s",
                    strlen(longName), longName, strlen(shortName), shortName, strlen(plmn), plmn);
        if (err < 0) return;
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_TOE_INFO_IND,
                                m_slot_id, RfxStringData(result, strlen(result)));
        logD(LOG_TAG, "handleCievInd, TOE len[%zu], content[%s]", strlen(result), result);
        responseToTelCore(urc);

        isNeedNotifyStateChanged();
    } else if (type != 102) {
        ViaBaseHandler *viaHandler = RfxViaUtils::getViaHandler();
        if (viaHandler != NULL) {
            viaHandler->handleViaUrc(msg, this, m_slot_id);
        }
    }
}

void RmmNwURCHandler::handleFemtocellInfoInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();

    uint8_t domain, state, act, isCsgCell, isLteFemtocell;
    int type, oldIsCsg, oldCsgId, err;
    uint32_t csgId;
    uint16_t iconType, cause;
    char* plmn, *hnbName;
    int INVALID = 0x7FFFFFFF;

    if (mipcData == NULL) {
        logE(LOG_TAG, "%s mipcData is null or error result", __FUNCTION__);
        return;
    }
    type = mipcData->getMipcUint8Val(MIPC_NW_FEMTOCELL_INFO_IND_T_TYPE, 0);
    pthread_mutex_lock(&sFemtocellMutex[m_slot_id]);
    switch(type) {
        case 1:  //  csg
            domain = mipcData->getMipcUint8Val(MIPC_NW_FEMTOCELL_INFO_IND_T_DOMAIN, 0);
            state = mipcData->getMipcUint8Val(MIPC_NW_FEMTOCELL_INFO_IND_T_STATE, 0);
            act = mipcData->getMipcUint8Val(MIPC_NW_FEMTOCELL_INFO_IND_T_ACT, 0);
            isCsgCell = mipcData->getMipcUint8Val(MIPC_NW_FEMTOCELL_INFO_IND_T_IS_CSG_CELL, 0);
            csgId = mipcData->getMipcUint32Val(MIPC_NW_FEMTOCELL_INFO_IND_T_CSG_ID, 0);

            iconType = mipcData->getMipcUint16Val(MIPC_NW_FEMTOCELL_INFO_IND_T_CSG_ICON_TYPE, 0);
            cause = mipcData->getMipcUint16Val(MIPC_NW_FEMTOCELL_INFO_IND_T_CAUSE, 0);

            plmn = mipcData->getMipcStr(MIPC_NW_FEMTOCELL_INFO_IND_T_PLMN_ID);
            hnbName = mipcData->getMipcStr(MIPC_NW_FEMTOCELL_INFO_IND_T_HNBNAME);
            oldIsCsg = sFemtocellCache[m_slot_id]->is_csg_cell;
            oldCsgId = sFemtocellCache[m_slot_id]->csg_id;
            sFemtocellCache[m_slot_id]->domain = domain;
            sFemtocellCache[m_slot_id]->state = state;
            sFemtocellCache[m_slot_id]->act = act;
            sFemtocellCache[m_slot_id]->plmn_id = plmn ? atoi(plmn) : 0;
            sFemtocellCache[m_slot_id] ->is_csg_cell = isCsgCell;
            if(plmn == NULL || strlen(plmn) <= 0) {
                logE(LOG_TAG, "[%s]: plmn is empty", __FUNCTION__);
                goto done;
            }
            matchPlmnName(sFemtocellCache[m_slot_id]->optr_name, NULL, plmn);
            if (isCsgCell == 1) {
                sFemtocellCache[m_slot_id]->csg_id = csgId;
                sFemtocellCache[m_slot_id]->csg_icon_type = iconType;
                strncpy(sFemtocellCache[m_slot_id]->hnbName, hnbName, MAX_OPER_NAME_LENGTH -1);
                sFemtocellCache[m_slot_id]->hnbName[MAX_OPER_NAME_LENGTH -1] = 0;
            } else {
                sFemtocellCache[m_slot_id]->csg_id = INVALID;
                sFemtocellCache[m_slot_id]->csg_icon_type = INVALID;
                err = sprintf(sFemtocellCache[m_slot_id]->hnbName, "%d", INVALID);
                if (err < 0) goto done;
            }
            sFemtocellCache[m_slot_id]->cause = cause;

            // trigger event
            sendEvent(RFX_MSG_EVENT_FEMTOCELL_UPDATE, RfxIntsData(&type, 1),
                    m_channel_id, m_slot_id);
            break;

        case 3: //  lte
            isLteFemtocell =  mipcData->getMipcUint8Val(
                    MIPC_NW_FEMTOCELL_INFO_IND_T_IS_FEMTOCELL, 0);
            if (isLteFemtocell != sFemtocellCache[m_slot_id]->is_lte_femtocell) {
                sFemtocellCache[m_slot_id]->is_lte_femtocell = isLteFemtocell;
            // trigger event
                sendEvent(RFX_MSG_EVENT_FEMTOCELL_UPDATE, RfxIntsData(&type, 1),
                        m_channel_id, m_slot_id);
            }
            break;

        default:
            logE(LOG_TAG, "unknown type: %u", type);
            break;
    }
done:
    pthread_mutex_unlock(&sFemtocellMutex[m_slot_id]);
}

void RmmNwURCHandler::handleMccMncChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    if (mipcData == NULL) {
       logE(LOG_TAG, "%s mipcData is null or error result", __FUNCTION__);
       return;
    }
    char* mccmnc = mipcData->getMipcStr(MIPC_NW_MCCMNC_IND_T_PLMN_ID);
    if (mccmnc == NULL || strlen(mccmnc) == 0 || strlen(mccmnc) > 6) {
        logE(LOG_TAG, "%s mccmnc is wrong", __FUNCTION__);
        return;
    }
    getMclStatusManager()->setString8Value(
                RFX_STATUS_KEY_MCCMNC_CHANGED, String8::format("%s", mccmnc));
    logD(LOG_TAG, "handleMccMncChanged: mccmnc=%s", mccmnc);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_MCCMNC_CHANGED,
            m_slot_id, RfxStringData(mccmnc, strlen(mccmnc)));
    responseToTelCore(urc);
}

void RmmNwURCHandler::handleEmergencyBearerSupportInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "handleEmergencyBearerSupportInd: invalid data");
       return;
    }
    uint8_t s1Support;
    uint8_t nrEmcSupport;
    uint8_t nrEmfSupport;

    s1Support = indData->getMipcUint8Val(
        MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_S1_SUPPORT, 0xff);
    nrEmcSupport = indData->getMipcUint8Val(
        MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMS_5G_SUPP, 0xff);
    nrEmfSupport = indData->getMipcUint8Val(
        MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMF_5G_SUPP, 0xff);

    if (s1Support == 0xff && nrEmcSupport == 0xff && nrEmfSupport == 0xff) {
        logE(LOG_TAG, "handleEmergencyBearerSupportInd: invalid arguments");
        return;
    }

    logD(LOG_TAG, "handleEmergencyBearerSupportInd: s1[%d], emc[%d], emf[%d]",
            s1Support, nrEmcSupport, nrEmfSupport);

    pthread_mutex_lock(&sImsEmergencyMutex[m_slot_id]);
    if (s1Support != sImsEmergencyInfo[m_slot_id]->volteEmcSupported
            && s1Support != 0xff) {
        sImsEmergencyInfo[m_slot_id]->volteEmcSupported = s1Support;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_VOLTE_EMC_SUPPORTED, s1Support);
    }
    if (nrEmcSupport != sImsEmergencyInfo[m_slot_id]->vonrEmcSupported
            && nrEmcSupport != 0xff) {
        sImsEmergencyInfo[m_slot_id]->vonrEmcSupported = nrEmcSupport;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_VONR_EMC_SUPPORTED, nrEmcSupport);
    }
    if (nrEmfSupport != sImsEmergencyInfo[m_slot_id]->vonrEmfSupported
            && nrEmfSupport != 0xff) {
        sImsEmergencyInfo[m_slot_id]->vonrEmfSupported = nrEmfSupport;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_VONR_EMF_SUPPORTED, nrEmfSupport);
    }
    pthread_mutex_unlock(&sImsEmergencyMutex[m_slot_id]);
}

void RmmNwURCHandler::handleChannelInfoInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "handleChannelInfoInd: invalid data");
       return;
    }

    int data[3] = {0};
    int is_endc = 0;
    sp<RfxMclMessage> urc;
    data[0] = indData->getMipcInt32Val(MIPC_NW_CH_INFO_IND_T_RAT, 0);
    data[1] = indData->getMipcInt32Val(MIPC_NW_CH_INFO_IND_T_BAND, -1);
    data[2] = indData->getMipcInt32Val(MIPC_NW_CH_INFO_IND_T_CHANNEL, -1);
    if (data[0] == 128) {
        is_endc = indData->getMipcInt32Val(MIPC_NW_CH_INFO_IND_T_IS_ENDC, 0);
//        if (is_endc) scgBand[m_slot_id] = data[1];
    }

    logD(LOG_TAG, "handleChInfoChanged, rat=%d, band=%d, channel=%d, is_endc=%d", data[0], data[1], data[2], is_endc);
    if (!is_endc) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_BAND_INFO_IND, m_slot_id, RfxIntsData(data, 3));
        responseToTelCore(urc);
    }
}

void RmmNwURCHandler::handleNrCaBandInfoInd(const sp<RfxMclMessage>& msg) {
    int is_endc = 0, band_1 = 0, band_2 = 0;

    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "handleNrCaBandInfoInd: invalid data");
       return;
    }

    int data[3] = {0};
    sp<RfxMclMessage> urc;
    uint8_t bandNum = 0;
    uint16_t len = 0;
    is_endc = indData->getMipcInt32Val(MIPC_NW_NR_CA_BAND_IND_T_IS_ENDC, 0);
    bandNum = indData->getMipcUint8Val(MIPC_NW_NR_CA_BAND_IND_T_BAND_NUM, 0);
    if (bandNum <= 0 || bandNum > 2) {
        logE(LOG_TAG, "handleNrCaBandInfoInd: ignore invalid band num: %d", bandNum);
        return;
    }
    mipc_nr_ca_band_struct4* pBands =
        (mipc_nr_ca_band_struct4*)(indData->getMipcVal(MIPC_NW_NR_CA_BAND_IND_T_BAND, &len));

    band_1 = pBands->band[0];
    band_2 = bandNum == 2 ? pBands->band[1] : 0;

    data[0] = is_endc;
    data[1] = band_1;
    data[2] = band_2;
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_CA_BAND_IND, m_slot_id, RfxIntsData(data, 3));
    responseToTelCore(urc);
}

void RmmNwURCHandler::handleNrSysInfoChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();

    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] URC error", __FUNCTION__);
        return;
    }

    int nrSbspKHz = 0;
    int info[2] = { 0 };
    sp<RfxMclMessage> urc;
    nrSbspKHz = mipcData->getMipcInt32Val(MIPC_NW_NR_SCS_IND_T_SCS, 0);

    pthread_mutex_lock(&sNrSysInfoMutex[m_slot_id]);
    logD(LOG_TAG, "[%s] current NR sys info:%d, %d, nrSbspKHz = %d", __FUNCTION__,
        sNrSysInfo[m_slot_id]->upperlayerIndR15Available,
        sNrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz, nrSbspKHz);

    if (sNrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz != nrSbspKHz) {
        sNrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz = nrSbspKHz;
        info[0] = sNrSysInfo[m_slot_id]->upperlayerIndR15Available;
        info[1] = sNrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_SYS_INFO, m_slot_id,
                                   RfxIntsData(info, 2));
        responseToTelCore(urc);
    }

    pthread_mutex_unlock(&sNrSysInfoMutex[m_slot_id]);
    return;
}

void RmmNwURCHandler::handleIWlanRegStateChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData *mipcData = msg->getMipcData();
    if (mipcData == NULL) {
        logE(LOG_TAG, "[%s] mipcData is NULL return", __FUNCTION__);
        return;
    }

    int status = msg->getMipcData()->getMipcUint32Val(MIPC_NW_IWLAN_IND_T_STATUS,
                                                      MIPC_NW_IWLAN_STATUS_NOT_READY);
    logD(LOG_TAG, "[%s] status:%d", __FUNCTION__, status);
    if (status == MIPC_NW_IWLAN_STATUS_NOT_READY || status == MIPC_NW_IWLAN_STATUS_READY) {
        int prevStatus = s_iwlan_reg_status[m_slot_id];
        pthread_mutex_lock(&s_iwlanRegStatusMutex[m_slot_id]);
        s_iwlan_reg_status[m_slot_id] = status;
        pthread_mutex_unlock(&s_iwlanRegStatusMutex[m_slot_id]);
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_IWLAN_REGISTRATION_STATE,
                                                         m_slot_id, RfxIntsData(&status, 1));
        responseToTelCore(urc);

        int stat[4] = { 0 };
        RIL_PS_REG_STATE_CACHE* pCache = sPsRegStateCache[m_slot_id];
        stat[0] = convertRegState(pCache->registration_state, false);
        stat[1] = convertPSNetworkType(pCache->radio_technology);
        stat[2] = (status == 1) ? 1 : stat[0];
        stat[3] = (status == 1) ? RADIO_TECH_IWLAN : stat[1];
        sendEvent(RFX_MSG_EVENT_PS_NETWORK_STATE, RfxIntsData(stat, 4),
                  RIL_CMD_PROXY_3, m_slot_id);

        if (s_iwlan_in_legacy_mode && (prevStatus != status)) {
            setMSimProperty(m_slot_id, (char*)PROPERTY_WFC_STATE,
                            ((status == 1) ? (char*)"1" : (char*)"0"));
            isNeedNotifyStateChanged();
        }
    }
}

void RmmNwURCHandler::handle5GUWChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "handle5GUWChanged: invalid data");
       return;
    }
    int param[4] = {0};
    sp<RfxMclMessage> urc;
    logD(LOG_TAG, "handle5GUWChanged +++");
    // +E5GUW : <display_5guw>,<on_n77_band>,<on_fr2_band>,<5guw_allowed>
    param[0] = indData->getMipcUint8Val(MIPC_NW_NRUW_INFO_IND_T_DISPLAY_5GUW, 0);
    param[1] = indData->getMipcUint8Val(MIPC_NW_NRUW_INFO_IND_T_ON_N77_BAND, 0);
    param[2] = indData->getMipcUint8Val(MIPC_NW_NRUW_INFO_IND_T_ON_FR2_BAND, 0);
    param[3] = indData->getMipcUint8Val(MIPC_NW_NRUW_INFO_IND_T_5GUW_ALLOWED, 0);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_5GUW_INFO,
                m_slot_id, RfxIntsData(param, 4));
    responseToTelCore(urc);
}

void RmmNwURCHandler::handle5GUCChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "handle5GUCChanged: invalid data");
       return;
    }
    int param[3] = {0};
    sp<RfxMclMessage> urc;
    logD(LOG_TAG, "handle5GUCChanged +++");
    // +E5GUC : <display_5guc>,<on_UC_band>,<agg_bw>
    param[0] = indData->getMipcUint8Val(MIPC_NW_NRUC_INFO_IND_T_DISPLAY_5GUC, 0);
    param[1] = indData->getMipcUint8Val(MIPC_NW_NRUC_INFO_IND_T_ON_UC_BAND, 0);
    param[2] = indData->getMipcUint8Val(MIPC_NW_NRUC_INFO_IND_T_AGG_BW, 0);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_5GUW_INFO,
                m_slot_id, RfxIntsData(param, 3));
    responseToTelCore(urc);
}

void RmmNwURCHandler::handleFirstPlmnChanged(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "handleFirstPlmnChanged: invalid data");
       return;
    }

    // +EFRPLMN : <mcc>,<mnc>
    char* mcc = indData->getMipcStr(MIPC_NW_FIRST_PLMN_IND_T_MCC);

    if (mcc == NULL || strlen(mcc) == 0 || strlen(mcc) >= MIPC_MAX_MCCMNC_LEN) {
        logE(LOG_TAG, "%s mcc is wrong", __FUNCTION__);
        return;
    }

    char* mnc = indData->getMipcStr(MIPC_NW_FIRST_PLMN_IND_T_MNC);

    if (mnc == NULL || strlen(mnc) == 0 || strlen(mnc) >= MIPC_MAX_MCCMNC_LEN) {
        logE(LOG_TAG, "%s mnc is wrong", __FUNCTION__);
        return;
    }

    const int urcFisrtPlmn[] = {
        MIPC_NW_FIRST_PLMN_IND,
    };

    /* Disable FIRST PLMN INFO URC */
    unregisterToHandleMipcURC(urcFisrtPlmn, 1);

    logD(LOG_TAG, "handleFirstPlmnChanged mcc=%s, mnc=%s", mcc, mnc);
}

void RmmNwURCHandler::handleNwCfgInfo(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "handleNwCfgInfo: invalid data");
       return;
    }

    RIL_NwCfgInfo nw_cfg_info;
    nw_cfg_info.mimo = indData->getMipcUint8Val(MIPC_NW_ENWCFGINFO_IND_T_4X4MINO_STATE, 0);
    nw_cfg_info.qam_256 = indData->getMipcUint8Val(MIPC_NW_ENWCFGINFO_IND_T_256QAM_STATE, 0);
    nw_cfg_info.qam_ul64 = indData->getMipcUint8Val(MIPC_NW_ENWCFGINFO_IND_T_64QAM_STATE, 0);
    logD(LOG_TAG, "handleNwCfgInfo mimo=%d, qam256=%d, qam64=%d",
        nw_cfg_info.mimo, nw_cfg_info.qam_256, nw_cfg_info.qam_ul64);
    sendNwCfgInfo(nw_cfg_info);
}


void RmmNwURCHandler::handleCellularIdentifierDisclosure(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "%s: invalid data", __FUNCTION__);
       return;
    }

    RIL_CellularIdentifierDisclosure* nw_cid =
        (RIL_CellularIdentifierDisclosure*) calloc(1, sizeof(RIL_CellularIdentifierDisclosure));
    if (nw_cid == NULL) {
        logE(LOG_TAG, "%s calloc fail", __FUNCTION__);
        return;
    }
    memset(nw_cid, 0, sizeof(RIL_CellularIdentifierDisclosure));
    char* plmn = indData->getMipcStr(MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_PLMN_ID);
    if (plmn == NULL || (strlen(plmn) != 5 && strlen(plmn) != 6)) {
        logE(LOG_TAG, "%s: invalid plmn", __FUNCTION__);
        return;
    }
    strlcpy(nw_cid->plmn, plmn, 8);
    nw_cid->identifier =
        (RIL_CellularIdentifier) (indData->getMipcUint8Val(MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_CELLULAR_ID_TYPE, 0));
    nw_cid->protocolMessage =
        (RIL_NasProtocolMessage) (indData->getMipcUint8Val(MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_NAS_MSG_TYPE, 0));
    nw_cid->isEmergency = indData->getMipcUint8Val(MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_IS_EMERGENCY, 0) == 1;
    logD(LOG_TAG, "%s, plmn[%s], identifier[%d], protocolMessage[%d], isEmergency[%d]", __FUNCTION__,
        nw_cid->plmn, nw_cid->identifier, nw_cid->protocolMessage, nw_cid->isEmergency);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CELLULAR_IDENTIFIER_DISCLOSED,
            m_slot_id, RfxCidData((void *)nw_cid, sizeof(RIL_CellularIdentifierDisclosure)));
    responseToTelCore(urc);
    free(nw_cid);

}

void RmmNwURCHandler::handleSecurityAlgorithmsUpdated(const sp<RfxMclMessage>& msg) {
    RfxMipcData *indData = msg->getMipcData();
    if (indData == NULL) {
       logE(LOG_TAG, "%s: invalid data", __FUNCTION__);
       return;
    }
    int param[4] = {0};
    sp<RfxMclMessage> urc;
    param[0] = indData->getMipcUint8Val(MIPC_NW_SECURITY_ALGOS_IND_T_CONNECTION_EVENT, 0xff);
    param[1] = indData->getMipcUint8Val(MIPC_NW_SECURITY_ALGOS_IND_T_ENCRYPTION_ALGO, 0xff);
    param[2] = indData->getMipcUint8Val(MIPC_NW_SECURITY_ALGOS_IND_T_INTEGRITY_ALGO, 0xff);
    param[3] = indData->getMipcUint8Val(MIPC_NW_SECURITY_ALGOS_IND_T_IS_UNPROTECTED_EMERGENCY, 0);
    logD(LOG_TAG, "%s, connectionEvent[%d], encryption[%d], integrity[%d], isUnprotectedEmergency[%d]",
        __FUNCTION__, param[0], param[1], param[2], param[3]);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SECURITY_ALGORITHMS_UPDATED, m_slot_id, RfxIntsData(param, 4));
    responseToTelCore(urc);
}

void RmmNwURCHandler::requestSetSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODEM_ERR;

    const int urcSau[] = {
        MIPC_NW_SECURITY_ALGOS_IND
    };
    sp<RfxMclMessage> response;
    char sau_enabled[RFX_PROPERTY_VALUE_MAX] = {0};
    int *pInt = (int *)msg->getData()->getData();
    int dataLen = msg->getData()->getDataLength();

    char feature[] = "NULL_ALGOS";
    if (getFeatureVersion(feature, 0) != 1) {
//        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }
    if (dataLen != sizeof(int)) {
        logE(LOG_TAG, "%s: dataLen(%d) is wrong", __FUNCTION__, dataLen);
        goto error;
    }
    if (pInt[0] != 0 && pInt[0] != 1) {
        logE(LOG_TAG, "%s: invalid value(%d)", __FUNCTION__, pInt[0]);
        goto error;
    }
    getMSimProperty(m_slot_id, (char*) PROPERTY_SAU_ENABLED, sau_enabled);
    if (atoi(sau_enabled) != pInt[0]) {
        if (pInt[0] > 0) {
            registerToHandleMipcURC(urcSau, sizeof(urcSau)/sizeof(int));
        } else {
            unregisterToHandleMipcURC(urcSau, sizeof(urcSau)/sizeof(int));
        }
        setMSimProperty(m_slot_id, (char*) PROPERTY_SAU_ENABLED, (char*) (pInt[0] ? "1" : "0"));
    } else {
        logV(LOG_TAG, "%s: already enabled[%d], give response directly", __FUNCTION__, pInt[0]);
    }
    logV(LOG_TAG, "%s: sau enabled[%d]", __FUNCTION__, pInt[0]);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(response);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwURCHandler::requestIsSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    char feature[] = "NULL_ALGOS";
    if (getFeatureVersion(feature, 0) != 1) {
//        ril_errno = RIL_E_REQUEST_NOT_SUPPORTED;
        ril_errno = RIL_E_MODEM_ERR;
    }

    char sau_enabled[RFX_PROPERTY_VALUE_MAX] = {0};
    getMSimProperty(m_slot_id, (char*) PROPERTY_SAU_ENABLED, sau_enabled);
    int enabled = atoi(sau_enabled);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxIntsData(&enabled, 1), msg, false);
    responseToTelCore(response);
}

void RmmNwURCHandler::updateNwCfgInfo() {
    RIL_NwCfgInfo nw_cfg_info;
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_ENWCFGINFO_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "updateNwCfgInfo: mipc request returns error");
        return;
    }

    nw_cfg_info.mimo = cnfData->getMipcUint8Val(MIPC_NW_GET_ENWCFGINFO_CNF_T_4X4MINO_STATE, 0);
    nw_cfg_info.qam_256 = cnfData->getMipcUint8Val(MIPC_NW_GET_ENWCFGINFO_CNF_T_256QAM_STATE, 0);
    nw_cfg_info.qam_ul64 = cnfData->getMipcUint8Val(MIPC_NW_GET_ENWCFGINFO_CNF_T_64QAM_STATE, 0);
    logD(LOG_TAG, "updateNwCfgInfo mimo=%d, qam256=%d, qam64=%d",
        nw_cfg_info.mimo, nw_cfg_info.qam_256, nw_cfg_info.qam_ul64);
    sendNwCfgInfo(nw_cfg_info);
}

void RmmNwURCHandler::sendNwCfgInfo(RIL_NwCfgInfo nw_cfg_info) {
    sp<RfxMclMessage> urc;
    RIL_NwCfgInfo *nw_cfg_p = nw_cfg_cache[m_slot_id];
    if (nw_cfg_p->mimo != nw_cfg_info.mimo
        || nw_cfg_p->qam_256 != nw_cfg_info.qam_256
        || nw_cfg_p->qam_ul64 != nw_cfg_info.qam_ul64) {
        pthread_mutex_lock(&s_nwCfgMutex[m_slot_id]);
        nw_cfg_p->mimo = nw_cfg_info.mimo;
        nw_cfg_p->qam_256 = nw_cfg_info.qam_256;
        nw_cfg_p->qam_ul64 = nw_cfg_info.qam_ul64;
        pthread_mutex_unlock(&s_nwCfgMutex[m_slot_id]);
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NW_CFG_INFO,
                                       m_slot_id,
                                       RfxIntsData((void*)nw_cfg_p, sizeof(RIL_NwCfgInfo)));
        responseToTelCore(urc);
    }
}

void RmmNwURCHandler::handleConnectionStateReportInternal(uint8_t mode, uint8_t state, uint8_t access) {
    uint8_t old_nr_state = 0;
    int old_state = 0, new_state = 0, tdd_fdd = 0;
    int samrtState = 0;
    sp<RfxMclMessage> urc;

    //SA idle/connected change
    if (access == 5) {
        pthread_mutex_lock(&nr_connection_state_Mutex[m_slot_id]);
        old_nr_state = nr_connection_state[m_slot_id];
        nr_connection_state[m_slot_id] = (mode == 1 && state == 8) ? 1 : 0;
        if (old_nr_state != nr_connection_state[m_slot_id]) {
            //Idle state no need to notify physical channel config, if rrc status change , perhaps need update.
            sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(), RIL_CMD_PROXY_3, m_slot_id);
        }
        logV(LOG_TAG, "[%s]:old_nr_state[%d], nr_connection_state[%d]",
                __FUNCTION__, old_nr_state, nr_connection_state[m_slot_id]);
        pthread_mutex_unlock(&nr_connection_state_Mutex[m_slot_id]);
        return;
    }

    // 3    Indicates usage of radio access of type E-UTRA TDD, see 3GPP TS 36.300 [145].
    // 4    Indicates usage of radio access of type E-UTRA FDD, see 3GPP TS 36.300 [145].
    if (access == 3 || access == 4) {
        if (access == 3) tdd_fdd = 0;
        else tdd_fdd = 1;

        if (mode == 1 && state == 7) new_state = 1;
        else new_state = 0;
    } else {
        // we don't care of non-LTE case.
        new_state = -1;
    }

    if (new_state == -1) {
        return;  // we ignore non-LTE case
    }

    old_state = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_LTE_RRC_STATE, 1);
    if (old_state != new_state) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_LTE_RRC_STATE, new_state);
        //Idle state no need to notify physical channel config, if rrc status change , perhaps need update.
        sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(), RIL_CMD_PROXY_3, m_slot_id);
    }

    if (tdd_fdd == 0) {
        samrtState = (new_state | 0x08) | 0x20; // for TDD RRC state
    } else {
        samrtState = (new_state | 0x04) | 0x20; // for FDD RRC state
    }
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SMART_RAT_SWITCH_INFO, m_slot_id,
            RfxIntsData(&samrtState, 1));
    responseToTelCore(urc);
}

void RmmNwURCHandler::updateConnectionStateReport() {
    uint8_t mode = 0, state = 0, access = 0;
    uint16_t len = 0;
    logV(LOG_TAG, "[%s]: +++++", __FUNCTION__);
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_CSCON_STATE_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(mipcReq);
    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "[%s]: mipc request returns error", __FUNCTION__);
        return;
    }
    mipc_nw_cscon_status_struct4* pCsconStatus =
        (mipc_nw_cscon_status_struct4*)cnfData->getMipcVal(MIPC_NW_GET_CSCON_STATE_CNF_T_CSCON_STATE, &len);
    if (pCsconStatus && len == sizeof(mipc_nw_cscon_status_struct4)) {
        mode = pCsconStatus->mode;
        state = pCsconStatus->state;
        access = pCsconStatus->access;
    } else {
        logE(LOG_TAG, "[%s] failed, pCsconStatus:%p, len:%d, struct len:%d",
             __FUNCTION__, pCsconStatus, (int)len, (int)sizeof(mipc_nw_cscon_status_struct4));
        return;
    }

    int technology = sPsRegStateCache[m_slot_id]->radio_technology;
    logV(LOG_TAG, "[%s]: mode=%d, state=%d, access=%d, technology = %d", __FUNCTION__, mode, state, access, technology);
    if (isInService(sPsRegStateCache[m_slot_id]-> registration_state) &&
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
            logE(LOG_TAG, "[%s]: unknown access", __FUNCTION__);
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
    } else {
        //process it as a normal URC
        handleConnectionStateReportInternal(mode, state, access);
    }
}

#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
void RmmNwURCHandler::updataPsIndicaton() {
    RIL_PS_REG_STATE_CACHE* pPsCache = sPsRegStateCache[m_slot_id];
    int psRegState = convertRegState(pPsCache->registration_state, false);
    bool ret = fillPsCache(pPsCache, psRegState);
    if (ret == 0) {
        int oldRegState = convertRegState(pPsCache->registration_state, false);
        int old_rat = convertPSNetworkType(pPsCache->radio_technology);
        handlePsIndMisc(oldRegState, old_rat);
    }
}

void RmmNwURCHandler::updataCsIndicaton() {
    RIL_CS_REG_STATE_CACHE* pCsCache = sCsRegStateCache[m_slot_id];
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_CS_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData->getResult() == MIPC_RESULT_SUCCESS) {
        bool m4gRat = false;
        bool m5gRat = false;
        bool mC2kRat = false;
        int csRegState = -1;
        int psRegState = -1;
        bool ret = fillCsCache(cnfData, m4gRat, m5gRat, mC2kRat, csRegState, psRegState);
        if (ret == 0) {
            int oldRegState = convertRegState(pCsCache->registration_state, true);
            int old_rat = convertCSNetworkType(pCsCache->radio_technology);
            handleCsIndMisc(oldRegState, old_rat);
        }
    }
}

void RmmNwURCHandler::handleCsIndMisc(int oldRegState, int old_rat) {
    int tmpState[4];
    tmpState[0] = oldRegState;
    tmpState[1] = old_rat;
    char* responseStr[6] = {0};
    asprintf(&responseStr[0], "%d",
             convertRegState(sCsRegStateCache[m_slot_id]->registration_state, true));
    asprintf(&responseStr[1], "%x", sCsRegStateCache[m_slot_id]->lac);
    asprintf(&responseStr[2], "%lx", sCsRegStateCache[m_slot_id]->cid);
    asprintf(&responseStr[3], "%d",
             convertCSNetworkType(sCsRegStateCache[m_slot_id]->radio_technology));
    asprintf(&responseStr[4], "%d", sCsRegStateCache[m_slot_id]->reject_cause);
    asprintf(&responseStr[5], "%d", sCsRegStateCache[m_slot_id]->network_exist);
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_CS_NETWORK_STATE_CHANGED,
                                                     m_slot_id, RfxStringsData(responseStr, 6));
    responseToTelCore(urc);

    isNeedNotifyStateChanged();

    for (int i = 0; i < 6; i++) {
        freeSetNull(responseStr[i]);
    }

    tmpState[2] = convertRegState(sCsRegStateCache[m_slot_id]->registration_state, true);
    tmpState[3] = convertCSNetworkType(sCsRegStateCache[m_slot_id]->radio_technology);
    sendEvent(RFX_MSG_EVENT_CS_NETWORK_STATE, RfxIntsData(tmpState, 4), RIL_CMD_PROXY_3, m_slot_id);
    if (sCsRegStateCache[m_slot_id]->registration_state == 3)
        sendRegistrationFailedEvent(RIL_DOMAIN_CS);
}

void RmmNwURCHandler::handlePsIndMisc(int oldRegState, int old_rat) {
    int tmpState[4];
    tmpState[0] = oldRegState;
    tmpState[1] = old_rat;
    int wfcState = getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);
    tmpState[2] = (wfcState == 1)
                      ? 1
                      : convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false);
    tmpState[3] = (wfcState == 1)
                      ? RADIO_TECH_IWLAN
                      : convertPSNetworkType(sPsRegStateCache[m_slot_id]->radio_technology);
    sendEvent(RFX_MSG_EVENT_PS_NETWORK_STATE, RfxIntsData(tmpState, 4), RIL_CMD_PROXY_3, m_slot_id);

    if (convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false) != tmpState[0]) {
        // perhaps physical channel config not change, but  ps change, need notify.
        sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(), RIL_CMD_PROXY_3, m_slot_id);
    }

    if (isInService(convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false)) ||
        isInService(tmpState[0])) {
        isNeedNotifyStateChanged();
    }

    // keep scg state
    if (isInService(convertRegState(sPsRegStateCache[m_slot_id]->registration_state,
                                    false)) &&                        // in service
        (sPsRegStateCache[m_slot_id]->radio_technology == 0x4000)) {  // nsa
        scg_connection_state[m_slot_id] = 1;
    } else {
        scg_connection_state[m_slot_id] = 0;
    }

    if (sPsRegStateCache[m_slot_id]->registration_state == 3)
        sendRegistrationFailedEvent(RIL_DOMAIN_PS);
}
#endif //MTK_HYP_DUAL_TELE_SUPPORT