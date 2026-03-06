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

#include "RmmNwRealTimeRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxMipcData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxVoiceRegStateData.h"
#include "RfxDataRegStateData.h"
#include "RfxViaUtils.h"
#include "ViaBaseHandler.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include "libmtkrilutils.h"
#include <cutils/memory.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmmNwRTReqHdlr"

#ifdef  MTK_HYP_DUAL_TELE_SUPPORT
static const char *SKIP_DCNR_RESTRICTED_STATE_MCCMNC[] = {
    "42507"
};
#endif //MTK_HYP_DUAL_TELE_SUPPORT

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_OPERATOR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxDataRegStateData, RFX_MSG_REQUEST_DATA_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoiceRegStateData, RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_NETWORK_SELECTION_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE);
// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmNwRealTimeRequestHandler, RIL_CMD_PROXY_7);

RmmNwRealTimeRequestHandler::RmmNwRealTimeRequestHandler(int slot_id, int channel_id) :
        RmmNwHandler(slot_id, channel_id) {
        const int request[] = {
            RFX_MSG_REQUEST_OPERATOR,
            RFX_MSG_REQUEST_DATA_REGISTRATION_STATE,
            RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE,
            RFX_MSG_REQUEST_QUERY_NETWORK_SELECTION_MODE,
            RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE
        };

        logI(LOG_TAG, "RmmNwRealTimeRequestHandler[%d] start", slot_id);
        registerToHandleRequest(request, sizeof(request)/sizeof(int));
        resetVoiceRegStateCache(sCsRegStateCache[m_slot_id], CACHE_GROUP_ALL);
        resetDataRegStateCache(sPsRegStateCache[m_slot_id], CACHE_GROUP_ALL);
        updateServiceStateValue();
}

RmmNwRealTimeRequestHandler::~RmmNwRealTimeRequestHandler() {
}

void RmmNwRealTimeRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    logV(LOG_TAG, "onHandleRequest:%s", RFX_ID_TO_STR(request));
    switch (request) {
        case RFX_MSG_REQUEST_OPERATOR:
            pthread_mutex_lock(&sNeedNotifyStateMutex[m_slot_id]);
            sIsNeedNotifyState[m_slot_id] = true;
            // increase Id so that RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM will be skiped
            ++sIsNeedNotifyStateId[m_slot_id];
            pthread_mutex_unlock(&sNeedNotifyStateMutex[m_slot_id]);
            requestOperator(msg);
            break;

        case RFX_MSG_REQUEST_DATA_REGISTRATION_STATE:
            requestDataRegistrationState(msg);
            break;

        case RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE:
            requestVoiceRegistrationState(msg);
            break;

        case RFX_MSG_REQUEST_QUERY_NETWORK_SELECTION_MODE:
            requestQueryNetworkSelectionMode(msg);
            break;

        case RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE:
            requestIWlanRegistrationState(msg);
            break;

        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

/*

void RmmNwRealTimeRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {

}
*/

void RmmNwRealTimeRequestHandler::requestOperator(const sp<RfxMclMessage>& msg) {
    logV(LOG_TAG, "[%s] enter", __FUNCTION__);
    char *resp[3] = {0};
    sp<RfxMclMessage> response;
    pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
    int csState = convertRegState(sCsRegStateCache[m_slot_id]->registration_state, true);
    unsigned int csLac = sCsRegStateCache[m_slot_id]->lac;
    pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
    pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
    int psState = convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false);
    unsigned int psLac = sPsRegStateCache[m_slot_id]->lac;
    int csRat = sCsRegStateCache[m_slot_id]->radio_technology == 0x8000 ? 11 : 0;
    int psRat = sPsRegStateCache[m_slot_id]->radio_technology == 0x8000 ? 11 : 0;
    int rat = isInService(psState) ? psRat: csRat;

    pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);
    int wfcState = getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);
    //for VoWifi & CS/PS OOS, use SIM's operator numeric.
    if (!isInService(csState) && !isInService(psState) && isInService(wfcState)) {
        const char* simNumeric = getMclStatusManager()->
                getString8Value(RFX_STATUS_KEY_UICC_GSM_NUMERIC, String8("")).string();
        if (strlen(simNumeric) == 0) {
            simNumeric = getMclStatusManager()->
                    getString8Value(RFX_STATUS_KEY_UICC_CDMA_NUMERIC, String8("")).string();
        }
        logV(LOG_TAG, "VoWifi Only, use SIM Operator Numeric:%s", simNumeric);
        if (strlen(simNumeric) > 0) {
            resp[2] = (char*)simNumeric;
            sp<RfxMipcData>  nameReq =
                RfxMipcData::obtainMipcData(MIPC_NW_GET_PROVIDER_NAME_REQ, m_slot_id);
            nameReq->addMipcTlvStr(MIPC_NW_GET_PROVIDER_NAME_REQ_T_PLMN_ID, simNumeric);
            sp<RfxMipcData> nameCnfData = callToMipcMsgSync(nameReq);
            if (nameCnfData->getResult() == MIPC_RESULT_SUCCESS) {
                resp[1] = nameCnfData->getMipcStr(MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME);
                resp[0] = nameCnfData->getMipcStr(MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME_LONG);
            }
            response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxStringsData(resp, 3), msg, false);
            responseToTelCore(response);
            return;
        }
    }

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_REGISTER_STATE_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData->getResult() == MIPC_RESULT_SUCCESS) {
        char *pPlmn = cnfData->getMipcStr(MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_ID);
        if (pPlmn && (pPlmn[0] != '\0')) {
            resp[2] = pPlmn;
            logV(LOG_TAG, "[%s] plmn:%s", __FUNCTION__, pPlmn);
            if (strlen(resp[2]) > 0 && (0 != strcmp(resp[2], "000000"))) {
                getMclStatusManager()->setString8Value(
                    RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED, String8::format("%s", resp[2]));
            } else {
                getMclStatusManager()->setString8Value(
                    RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED, String8(""));
            }
            setLastValidPlmn(resp[2]);
            bool hasService = isInService(csState) || isInService(psState) || isInService(wfcState);
            if ((!hasService) || !((*resp[2] >= '0') && (*resp[2] <= '9'))
                || !strcmp(resp[2], "000000")) {
                memset(resp, 0, sizeof(resp));
                if (!hasService) {
                    // clear key/cache when we are not in service
                    // keep the previous value when MD report invalid.
                    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_OPERATOR, String8(""));
                    pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
                    memset(sOpInfoCache[m_slot_id]->mccMnc, 0, 8);
                    memset(sOpInfoCache[m_slot_id]->longName, 0, MAX_OPER_NAME_LENGTH);
                    memset(sOpInfoCache[m_slot_id]->shortName, 0, MAX_OPER_NAME_LENGTH);
                    pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
                    logD(LOG_TAG, "[%s] oos, reset op info cache", __FUNCTION__);
                }

                response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                                                         RfxStringsData(resp, 3), msg, false);
                responseToTelCore(response);
                return;
            }

            String8 oper(resp[2]);
            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_OPERATOR, oper);
            sp<RfxMipcData>  nameReq =
                RfxMipcData::obtainMipcData(MIPC_NW_GET_PROVIDER_NAME_REQ, m_slot_id);
            nameReq->addMipcTlvStr(MIPC_NW_GET_PROVIDER_NAME_REQ_T_PLMN_ID, pPlmn);
            nameReq->addMipcTlvUint32(MIPC_NW_GET_PROVIDER_NAME_REQ_T_LAC,
                                      isInService(psState) ? psLac : csLac);
            nameReq->addMipcTlvUint8(MIPC_NW_GET_PROVIDER_NAME_REQ_T_OPL_INDEX,
                                      (rat == 11) ? 2 : 1);
            sp<RfxMipcData> nameCnfData = callToMipcMsgSync(nameReq);
            if (nameCnfData->getResult() == MIPC_RESULT_SUCCESS) {
                resp[1] = nameCnfData->getMipcStr(MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME);
                resp[0] = nameCnfData->getMipcStr(MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME_LONG);
                logV(LOG_TAG, "[%s] pName:%s, pLongName:%s", __FUNCTION__, resp[1], resp[0]);
                pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
                strlcpy(sOpInfoCache[m_slot_id]->longName, resp[0] ? resp[0] : "", MAX_OPER_NAME_LENGTH);
                strlcpy(sOpInfoCache[m_slot_id]->shortName, resp[1] ? resp[1] : "", MAX_OPER_NAME_LENGTH);
                strlcpy(sOpInfoCache[m_slot_id]->mccMnc, resp[2], 8);
                pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
            } else {
                logE(LOG_TAG, "[%s] nameCnfData result:%d", __FUNCTION__, nameCnfData->getResult());
                pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
                strlcpy(sOpInfoCache[m_slot_id]->mccMnc, resp[2], 8);
                pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
            }
            response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                                                     RfxStringsData(resp, 3), msg, false);
            responseToTelCore(response);
            return;
        } else {
            getMclStatusManager()->setString8Value(
                RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED, String8(""));
            logD(LOG_TAG, "[%s] plmn is null or empty!", __FUNCTION__);
            response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                                                         RfxStringsData(resp, 3), msg, false);
            responseToTelCore(response);
            return;
        }
    } else {
        logE(LOG_TAG, "[%s] register state cnfData result:%d", __FUNCTION__, cnfData->getResult());
    }

    logE(LOG_TAG, "[%s] must never return error when radio is on", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                                             RfxStringsData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRealTimeRequestHandler::requestVoiceRegistrationState(const sp<RfxMclMessage>& msg) {
    logV(LOG_TAG, "[%s] enter", __FUNCTION__);
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_CS_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData->getResult() == MIPC_RESULT_SUCCESS) {
#ifdef  MTK_HYP_DUAL_TELE_SUPPORT
        uint16_t rspDataLen = 0;
        mipc_nw_cs_reg_info_struct4* pInfo =
            (mipc_nw_cs_reg_info_struct4*)(cnfData->getMipcVal(MIPC_NW_GET_CS_CNF_T_REG_INFO, &rspDataLen));
        if (!pInfo || rspDataLen != sizeof(mipc_nw_cs_reg_info_struct4)) {
            logE(LOG_TAG, "[%s] pInfo:%p, rspDataLen:%d, struct len:%d", __FUNCTION__,
                 pInfo, (int)rspDataLen, (int)sizeof(mipc_nw_cs_reg_info_struct4));
            goto error;
        }

        RIL_CS_REG_STATE_CACHE* pCsCache = sCsRegStateCache[m_slot_id];
        RIL_PS_REG_STATE_CACHE* pPsCache = sPsRegStateCache[m_slot_id];
        pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
        pCsCache->registration_state = pInfo->stat;
        pCsCache->radio_technology = pInfo->rat;
        pCsCache->roaming_indicator = pInfo->roaming_ind;
        pCsCache->css = pInfo->css;
        pCsCache->is_in_prl = pInfo->is_in_prl;
        pCsCache->default_roaming_indicator = pInfo->def_roaming_ind;
        pCsCache->denied_reason = pInfo->reason_for_denial;
        pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
        dumpCsRegStateCache(__FUNCTION__, pCsCache, true);
        bool m4gRat = false;
        bool m5gRat = false;
        bool mC2kRat = false;
        int csRegState = convertRegState(pCsCache->registration_state, true);
        int psRegState = convertRegState(pPsCache->registration_state, false);
        if (!isInService(csRegState)) {
            int cs_type = convertCSNetworkType(pCsCache->radio_technology);
            if (cs_type == RADIO_TECH_LTE) {
                m4gRat = true;
            } else if (cs_type == RADIO_TECH_NR) {
                m5gRat = true;
            } else if (cs_type == RADIO_TECH_1xRTT ||
                       cs_type == RADIO_TECH_EVDO_A ||
                       cs_type == RADIO_TECH_EHRPD) {
                mC2kRat = true;
            }
            pCsCache->radio_technology = 0;
        }

        if (isSpecificVoiceStateConfigured() && !isInService(csRegState)) {
            int volteReg = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_STATE, 0);
            if ((volteReg & 0x01) == 0x01) { // IMS registered and VOPS
                if (isInService(psRegState)) {
                    pCsCache->denied_reason = 0;
                }
                pCsCache->registration_state = pPsCache->registration_state;
                pCsCache->radio_technology = pPsCache->radio_technology;
                pCsCache->lac = pPsCache->lac;
                pCsCache->cid = pPsCache->cid;
            }
        }

#else
        bool m4gRat = false;
        bool m5gRat = false;
        bool mC2kRat = false;
        int csRegState = -1;
        int psRegState = -1;
        bool ret = fillCsCache(cnfData, m4gRat, m5gRat, mC2kRat, csRegState, psRegState);
        if (ret == -1) {
            goto error;
        }

        RIL_CS_REG_STATE_CACHE* pCsCache = sCsRegStateCache[m_slot_id];
        RIL_PS_REG_STATE_CACHE* pPsCache = sPsRegStateCache[m_slot_id];
#endif //MTK_HYP_DUAL_TELE_SUPPORT
        // CDMA ECC
        int cdma_network_exist = 0;
        if (pCsCache->network_exist == 1) {
            cdma_network_exist = 1;
        } else {
            if (((csRegState == 0) ||
                 (csRegState == 2) ||
                 (csRegState == 3)) &&
                ((pCsCache->cid != 0x0fffffff) &&
                 (pCsCache->lac != 0xffff) &&
                 (pCsCache->cid != (unsigned int)-1) &&
                 (pCsCache->lac != (unsigned int)-1)) &&
                (!m4gRat) &&
                (!m5gRat)) {
                cdma_network_exist = 1;
            }
        }

        int radio_state = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, 0);
        if ((cdma_network_exist == 1) && (radio_state == RADIO_STATE_ON) &&
            !isInService(csRegState) && !isInService(psRegState)) {
            m_cdma_emergency_only = 1;
        } else {
            m_cdma_emergency_only = 0;
        }

        int cntCdma = 0;
        if (RfxNwServiceState::isCdmaGroup(pCsCache->radio_technology) ||
            RfxNwServiceState::isCdmaGroup(pPsCache->radio_technology)) {
            //only if current rat is CDMA group, there is meaning to query cdma information.
            cntCdma = requestVoiceRegistrationStateCdma(msg);
        }
        if (cntCdma == 0) {
            resetVoiceRegStateCache(sCsRegStateCache[m_slot_id], CACHE_GROUP_C2K);
        }

        if (csRegState != 4) {
            // if cid is 0x0fffffff means it is invalid
            // if lac is 0xffff means it is invalid
            if ((!isInService(csRegState) &&
                 !isInService(psRegState)) &&
                ((pCsCache->cid != 0x0fffffff) &&
                 (pCsCache->lac != 0xffff) &&
                 (pCsCache->cid != (unsigned int)-1) &&
                 (pCsCache->lac != (unsigned int)-1)) &&
                // do not set ECC when it is LTE/NR/C2K. ECC depends IMS.
                (!m4gRat) &&
                (!m5gRat) &&
                (!mC2kRat)) {
                if (m_emergency_only == 0) {
                    m_emergency_only = 1;
                    logV(LOG_TAG, "Set slot%d m_emergencly_only to true", m_slot_id);
                }
            } else {
                if(m_emergency_only == 1) {
                    m_emergency_only = 0;
                    logV(LOG_TAG, "Set slot%d s_emergencly_only to false", m_slot_id);
                }
            }
        }

        combineVoiceRegState(msg);
        return;
    } else {
        logE(LOG_TAG, "[%s] cnfData result:%d", __FUNCTION__, cnfData->getResult());
    }

error:
    logE(LOG_TAG, "[%s] error happened!", __FUNCTION__);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxStringsData(), msg, false);
    responseToTelCore(response);
}


void RmmNwRealTimeRequestHandler::sendVoiceRegResponse(const sp<RfxMclMessage>& msg) {
    RIL_CS_REG_STATE_CACHE* pCache = sCsRegStateCache[m_slot_id];
    sp<RfxMclMessage> response;
    RIL_VoiceRegistrationStateResponse *pData = (RIL_VoiceRegistrationStateResponse *)
            alloca(sizeof(RIL_VoiceRegistrationStateResponse));
    if (pData == NULL) goto error;
    memset(pData, 0, sizeof(RIL_VoiceRegistrationStateResponse));

    pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
    pData->regState =
        (RIL_RegState)convertToAndroidRegState(convertRegState(pCache->registration_state, true), true);
    pData->rat = isInService(convertRegState(pCache->registration_state, true))
        ? (RIL_RadioTechnology)convertCSNetworkType(pCache->radio_technology) : RADIO_TECH_UNKNOWN;
    pData->cssSupported = pCache->css;
    pData->roamingIndicator = pCache->roaming_indicator;
    pData->systemIsInPrl = pCache->is_in_prl;
    pData->defaultRoamingIndicator = pCache->default_roaming_indicator;
    // todo: need check whether need cache urc denied_reason
    pData->reasonForDenial = sCsRegStateCache[m_slot_id]->denied_reason;
    if (isInService(pData->regState)) {
        pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
        strncpy(pData->rplmn, sOpInfoCache[m_slot_id]->mccMnc, 7);
        pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
    }
    pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);

    if (isInService(pData->regState)) {
        fillVoiceCellIdentity(pData->cellIdentity);
        switch(pData->cellIdentity.cellInfoType) {
            case RIL_CELL_INFO_TYPE_LTE: {
                // lte vops info
                pData->lteVopsInfo.isVopsSupported = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS);
                pthread_mutex_lock(&sImsEmergencyMutex[m_slot_id]);
                pData->lteVopsInfo.isEmcBearerSupported = sImsEmergencyInfo[m_slot_id]->volteEmcSupported;
                pthread_mutex_unlock(&sImsEmergencyMutex[m_slot_id]);
                // NR non-standalone
                pData->nrIndicators.isDcNrRestricted = sPsRegStateCache[m_slot_id]->dcnr_restricted;
                pData->nrIndicators.isNrAvailable = (sPsRegStateCache[m_slot_id]->endc_sib == 2) ? 1 : 0;
                pData->nrIndicators.isEndcAvailable = sPsRegStateCache[m_slot_id]->endc_available;
                pData->attachResultType = isInService(
                    convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false)) ? COMBINED : NONE;
                pData->extraInfo = (pCache->registration_state == 6 ?
                    EXTRA_SMS_ONLY : (pCache->registration_state == 9 ? EXTRA_CSFB_NOT_PREFERRED : EXTRA_NONE));
                break;
            }
            case RIL_CELL_INFO_TYPE_NR:
                // TODO: NR vops info need updated
                pData->nrVopsInfo.vopsSupported = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS);
                pthread_mutex_lock(&sImsEmergencyMutex[m_slot_id]);
                pData->nrVopsInfo.emcSupported = sImsEmergencyInfo[m_slot_id]->vonrEmcSupported;
                pData->nrVopsInfo.emfSupported = sImsEmergencyInfo[m_slot_id]->vonrEmfSupported;
                pthread_mutex_unlock(&sImsEmergencyMutex[m_slot_id]);
                break;

            default:
                break;
        }
    } else {
        pData->cellIdentity.cellInfoType = RIL_CELL_INFO_TYPE_NONE;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoiceRegStateData(pData, sizeof(RIL_VoiceRegistrationStateResponse)), msg, false);

    responseToTelCore(response);
    clearCellIdentity(pData->cellIdentity);
    return;

error:
    logE(LOG_TAG, "sendVoiceRegResponse something wroing");
}


void RmmNwRealTimeRequestHandler::updateServiceStateValue() {
    // 0 - Not registered
    // 1 - Registered, home network
    // 2 - Not registered, but MT is currently searching a new operator to register
    // 3 - Registration denied
    // 4 - Unknown
    // 5 - Registered, roaming
    int wfcState = getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);
    pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
    int csRegState = convertRegState(sCsRegStateCache[m_slot_id]->registration_state, true);
    int psRegState = convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false);
    int csNetworkType = isInService(csRegState) ?
        convertCSNetworkType(sCsRegStateCache[m_slot_id]->radio_technology) : 0;
    int psNetworkType = isInService(psRegState) ?
        convertPSNetworkType(sPsRegStateCache[m_slot_id]->radio_technology) : 0;
    getMclStatusManager()->setServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
            RfxNwServiceState(csRegState, psRegState, csNetworkType, psNetworkType, wfcState));
    pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
}

void RmmNwRealTimeRequestHandler::combineVoiceRegState(const sp<RfxMclMessage>& msg) {
    sendVoiceRegResponse(msg);
    updateServiceStateValue();
}


int RmmNwRealTimeRequestHandler::convertToAndroidRegState(unsigned int uiRegState, bool isVoice) {
    unsigned int uiRet = 0, convertedState = 0;

    logD(LOG_TAG, "convertToAndroidRegState s_emergencly_only=%d, "
         "m_cdma_emergency_only=%d, m_ims_ecc_only=%d",
         m_emergency_only, m_cdma_emergency_only, m_ims_ecc_only);
    switch (uiRegState) {
        case 0:
            convertedState = isVoice ? 10 : 20;
            break;
        case 2:
            convertedState = isVoice ? 12 : 20;
            break;
        case 3:
            convertedState = isVoice ? 13 : 20;
            break;
        case 4:
            convertedState = isVoice ? 14 : 20;
            break;
        default:
            convertedState = uiRegState;
            break;
    }
    /* Ref RIL_RegState in RIL.h*/
    if (isVoice && (m_emergency_only == 1 || m_cdma_emergency_only == 1)) {
        uiRet = convertedState;
    } else if (!isVoice && m_ims_ecc_only == 1) {
        uiRet = convertedState;
    } else {
        uiRet = uiRegState;
    }
    return uiRet;
}

void RmmNwRealTimeRequestHandler::requestDataRegistrationState(const sp<RfxMclMessage>& msg) {
    logV(LOG_TAG, "[%s] enter", __FUNCTION__);
    int retryCount = 2;
    sendEvent(RFX_MSG_EVENT_REFRESH_CELL_INFO_LIST,
                    RfxIntsData(&retryCount, 1), RIL_CMD_PROXY_3, m_slot_id);
#ifdef  MTK_HYP_DUAL_TELE_SUPPORT

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_PS_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    RIL_CS_REG_STATE_CACHE* pCsCache = sCsRegStateCache[m_slot_id];
    RIL_PS_REG_STATE_CACHE* pPsCache = sPsRegStateCache[m_slot_id];
    int csRegState = convertRegState(pCsCache->registration_state, true);
    int psRegState = convertRegState(pPsCache->registration_state, false);
    if (cnfData->getResult() == MIPC_RESULT_SUCCESS) {
        uint16_t rspDataLen = 0;
        mipc_nw_ps_reg_info_struct4* pInfo =
            (mipc_nw_ps_reg_info_struct4*)(cnfData->getMipcVal(MIPC_NW_GET_PS_CNF_T_REG_INFO, &rspDataLen));
        if (!pInfo || rspDataLen != sizeof(mipc_nw_ps_reg_info_struct4)) {
            logE(LOG_TAG, "[%s] pInfo:%p, rspDataLen:%d, struct len:%d", __FUNCTION__,
                 pInfo, (int)rspDataLen, (int)sizeof(mipc_nw_ps_reg_info_struct4));
            goto error;
        }
        pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
        pPsCache->registration_state = pInfo->stat;
        psRegState = convertRegState(pPsCache->registration_state, false);
        pPsCache->lac = (pInfo->lac_tac <= 0x7FFFFFFF) ? pInfo->lac_tac : 0x7FFFFFFF;
        pPsCache->cid = pInfo->cell_id;
        pPsCache->radio_technology =
            isInService(convertRegState(pInfo->stat, false)) ? pInfo->rat : 0;
        pPsCache->denied_reason = pInfo->reason_for_denial;
        pPsCache->max_simultaneous_data_call = pInfo->max_data_calls;
        if (skipDCNRRestrictedState((const char *)sOpInfoCache[m_slot_id]->mccMnc)) {
            //logD(LOG_TAG, "skip DCNR Restricted State for %s", sOpInfoCache[m_slot_id]->mccMnc);
            pPsCache->dcnr_restricted = 0;
        } else {
            pPsCache->dcnr_restricted = pInfo->is_dcnr_restricted;
        }
        pPsCache->endc_sib = pInfo->cache_endc_connect; // pInfo->is_nr_available is original value
        pPsCache->endc_available = pInfo->is_endc_available;
        //pPsCache->reject_cause = pInfo->;
        pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);
        dumpPsRegStateCache(__FUNCTION__, pPsCache, true);
        if (pInfo->cache_endc_connect != pInfo->is_nr_available) {
            logV(LOG_TAG, "[%s] cache_endc:%d, endc_sib:%d",
                 __FUNCTION__, pInfo->cache_endc_connect, pInfo->is_nr_available);
        }
    } else {
        logE(LOG_TAG, "[%s] cnfData result:%d", __FUNCTION__, cnfData->getResult());
    }

#else
    RIL_CS_REG_STATE_CACHE* pCsCache = sCsRegStateCache[m_slot_id];
    RIL_PS_REG_STATE_CACHE* pPsCache = sPsRegStateCache[m_slot_id];
    int csRegState = convertRegState(pCsCache->registration_state, true);
    int psRegState = convertRegState(pPsCache->registration_state, false);
    bool ret = fillPsCache(pPsCache, psRegState);
    if (ret == -1) {
        goto error;
    }
#endif //MTK_HYP_DUAL_TELE_SUPPORT
    if(!isInService(csRegState) && !isInService(psRegState) && sImsEccState[m_slot_id]) {
        m_ims_ecc_only = 1;
    } else {
        m_ims_ecc_only = 0;
    }
    combineDataRegState(msg);
    return;

error:
    logE(LOG_TAG, "[%s] must never return an error when radio is on", __FUNCTION__);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxStringsData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRealTimeRequestHandler::sendDataRegResponse(const sp<RfxMclMessage>& msg) {
    RIL_PS_REG_STATE_CACHE* pCache = sPsRegStateCache[m_slot_id];
    sp<RfxMclMessage> response;
    unsigned int regState;
    int wfcState;
    RIL_CS_REG_STATE_CACHE* pCsCache = sCsRegStateCache[m_slot_id];
    int csRegState = pCsCache->registration_state;
    int csAct = pCsCache->radio_technology;
    RIL_DataRegistrationStateResponse *pData = (RIL_DataRegistrationStateResponse *)
            alloca(sizeof(RIL_DataRegistrationStateResponse));
    if (pData == NULL) goto error;
    memset(pData, 0, sizeof(RIL_DataRegistrationStateResponse));
    regState = convertRegState(pCache->registration_state, false);
    pData->regState = (RIL_RegState) convertToAndroidRegState(regState, false);
    pData->rat = isInService(regState)
        ? (RIL_RadioTechnology)convertPSNetworkType(pCache->radio_technology) : RADIO_TECH_UNKNOWN;
    if (s_iwlan_in_legacy_mode) {
        wfcState = getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_WFC_STATE, 0);
        if (wfcState == 1) {
            pData->regState = RIL_REG_HOME;
            pData->rat = RADIO_TECH_IWLAN;
            logV(LOG_TAG, "[%s] wfcState:%d, for legacy mode", __FUNCTION__, wfcState);
        }
    }
    pData->reasonDataDenied = sPsRegStateCache[m_slot_id]->denied_reason;
    pData->maxDataCalls = sPsRegStateCache[m_slot_id]->max_simultaneous_data_call;
    if (isInService(regState)) {
        pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
        strncpy(pData->rplmn, sOpInfoCache[m_slot_id]->mccMnc, 7);
        pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
    }
    if (isInService(regState)) {
        fillDataCellIdentity(pData->cellIdentity);
        switch(pData->cellIdentity.cellInfoType) {
            case RIL_CELL_INFO_TYPE_LTE: {
                // lte vops info
                pData->lteVopsInfo.isVopsSupported = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS);
                pthread_mutex_lock(&sImsEmergencyMutex[m_slot_id]);
                pData->lteVopsInfo.isEmcBearerSupported = sImsEmergencyInfo[m_slot_id]->volteEmcSupported;
                pthread_mutex_unlock(&sImsEmergencyMutex[m_slot_id]);
                // NR non-standalone
                pData->nrIndicators.isDcNrRestricted = sPsRegStateCache[m_slot_id]->dcnr_restricted;
                pData->nrIndicators.isNrAvailable = (sPsRegStateCache[m_slot_id]->endc_sib == 2) ? 1 : 0;
                pData->nrIndicators.isEndcAvailable = sPsRegStateCache[m_slot_id]->endc_available;
                if (isInService(convertRegState(csRegState, true)) && (csAct == 4096 || csAct == 8192)) {
                    pData->attachResultType = COMBINED;
                } else {
                    pData->attachResultType = EPS_ONLY;
                }
                pData->extraInfo = (csRegState == 6 ?
                    EXTRA_SMS_ONLY : (csRegState == 9 ? EXTRA_CSFB_NOT_PREFERRED : EXTRA_NONE));
                break;
            }
            case RIL_CELL_INFO_TYPE_NR:
                // TODO: NR vops info need updated
                pData->nrVopsInfo.vopsSupported = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS);
                pthread_mutex_lock(&sImsEmergencyMutex[m_slot_id]);
                pData->nrVopsInfo.emcSupported = sImsEmergencyInfo[m_slot_id]->vonrEmcSupported;
                pData->nrVopsInfo.emfSupported = sImsEmergencyInfo[m_slot_id]->vonrEmfSupported;
                pthread_mutex_unlock(&sImsEmergencyMutex[m_slot_id]);
                break;

            case RIL_CELL_INFO_TYPE_GSM:
            case RIL_CELL_INFO_TYPE_WCDMA:
            case RIL_CELL_INFO_TYPE_CDMA:
            case RIL_CELL_INFO_TYPE_TD_SCDMA:
            default:
                break;
        }
    } else {
        pData->cellIdentity.cellInfoType = RIL_CELL_INFO_TYPE_NONE;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxDataRegStateData(pData, sizeof(RIL_DataRegistrationStateResponse)), msg, false);
    responseToTelCore(response);
    clearCellIdentity(pData->cellIdentity);
    return;

error:
    logE(LOG_TAG, "[%s] something wroing", __FUNCTION__);
    return;
}

void RmmNwRealTimeRequestHandler::combineDataRegState(const sp<RfxMclMessage>& msg) {
    updateServiceStateValue();
    sendDataRegResponse(msg);
}

void RmmNwRealTimeRequestHandler::requestQueryNetworkSelectionMode(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    int mode = 0;
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_REGISTER_STATE_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData->getData() == NULL
        || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    mode = cnfData->getMipcUint8Val(
        MIPC_NW_GET_REGISTER_STATE_CNF_T_MODE, 0xff);

    if (mode < 0 || mode == 0xff) {
        logE(LOG_TAG, "requestQueryNetworkSelectionMode: invalid mode[%d]", mode);
        goto error;
    }

    pthread_mutex_lock(&(sNwSelModeMutex[m_slot_id]));
    if (mode == MIPC_NW_REGISTER_MODE_AUTOMATIC
        || mode == MIPC_NW_REGISTER_MODE_SEMI_AUTO) {
        sNwSelMode[m_slot_id] = false;
        mode = 0;
    } else {
        sNwSelMode[m_slot_id] = true;
        mode = 1;
    }
    pthread_mutex_unlock(&(sNwSelModeMutex[m_slot_id]));

    logV(LOG_TAG, "requestQueryNetworkSelectionMode: mode[%d]", mode);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "[%s] must never return error when radio is on", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

int RmmNwRealTimeRequestHandler::requestVoiceRegistrationStateCdma(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    int err = 0;
    ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
    if (mViaHandler != NULL) {
        CDMA_CELL_LOCATION_INFO cdma_location;
        memset(&cdma_location, 0, sizeof(CDMA_CELL_LOCATION_INFO));
        err = mViaHandler->getCdmaLocationInfo(this, &cdma_location);
        if (err < 0) {
            goto error;
        }
        pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
        RIL_CS_REG_STATE_CACHE* pCache = sCsRegStateCache[m_slot_id];
        pCache->system_id = cdma_location.system_id;
        pCache->network_id = cdma_location.network_id;
        pCache->base_station_id = cdma_location.base_station_id;
        pCache->base_station_latitude = cdma_location.base_station_latitude;
        pCache->base_station_longitude = cdma_location.base_station_longitude;
        pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
    }
    return 1;

error:
    return 0;
}

void RmmNwRealTimeRequestHandler::resetVoiceRegStateCache(RIL_CS_REG_STATE_CACHE *pCache, RIL_CACHE_GROUP source) {
    pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
    if (source == CACHE_GROUP_ALL) {
        pCache->registration_state = 0;
        pCache->lac = 0xffffffff;
        pCache->cid = 0x0fffffff;
        pCache->radio_technology = 0;
        pCache->base_station_id = 0;
        pCache->base_station_latitude = 0;
        pCache->base_station_longitude = 0;
        pCache->css = 0;
        pCache->system_id = 0;
        pCache->network_id = 0;
        pCache->roaming_indicator = -1;
        pCache->is_in_prl = -1;
        pCache->default_roaming_indicator = -1;
        pCache->denied_reason = 0;
        pCache->network_exist = 0;
    } else if (source == CACHE_GROUP_COMMON_REQ) {
        pCache->registration_state = 0;
        pCache->lac = 0xffffffff;
        pCache->cid = 0x0fffffff;
        pCache->radio_technology = 0;
        pCache->roaming_indicator = 0;
        pCache->denied_reason = 0;
        pCache->network_exist = 0;
    } else if (source == CACHE_GROUP_C2K) {
        pCache->base_station_id = 0;
        pCache->base_station_latitude = 0;
        pCache->base_station_longitude = 0;
        pCache->css = 0;
        pCache->system_id = 0;
        pCache->network_id = 0;
    } else {
        logD(LOG_TAG, "[%s]: source type:%d invalid!", __FUNCTION__, source);
    }
    pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
}

void RmmNwRealTimeRequestHandler::resetDataRegStateCache(RIL_PS_REG_STATE_CACHE *pCache,
        RIL_CACHE_GROUP source) {
    RFX_UNUSED(source);
    pCache->registration_state = 0;
    pCache->lac = 0xffffffff;
    pCache->cid = 0x0fffffff;
    pCache->radio_technology = 0;
    pCache->denied_reason = 0;
    pCache->max_simultaneous_data_call = 1;
    // NR none-standalone
    pCache->dcnr_restricted = 0;
    pCache->endc_sib = 0;
    pCache->endc_available = 0;
}

void RmmNwRealTimeRequestHandler::requestIWlanRegistrationState(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int state = -1; //s_iwlan_reg_status[m_slot_id];
    logD(LOG_TAG, "[%s] state:%d", __FUNCTION__, state);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxIntsData(&state, 1), msg, false);
    responseToTelCore(response);
}

#ifdef  MTK_HYP_DUAL_TELE_SUPPORT
bool RmmNwRealTimeRequestHandler::skipDCNRRestrictedState(const char *mccmnc) {
    bool ret = false;
    // Compare with mccmnc to skip dcnr restricted state
    for (size_t i = 0; i < NUM_ELEMS(SKIP_DCNR_RESTRICTED_STATE_MCCMNC); i++) {
        if (RfxMisc::strStartsWith(mccmnc, SKIP_DCNR_RESTRICTED_STATE_MCCMNC[i])) {
            ret = true;
            break;
        }
    }
    return ret;
}
#endif //MTK_HYP_DUAL_TELE_SUPPORT