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

#include "RmcNetworkRealTimeRequestHandler.h"
#include "RfxVoiceRegStateData.h"
#include "RfxDataRegStateData.h"
#include "rfx_properties.h"
#include "ViaBaseHandler.h"
#include "RfxViaUtils.h"
#include <libmtkrilutils.h>

static const char *SKIP_DCNR_RESTRICTED_STATE_MCCMNC[] = {
    "42507"
};

static const int request[] = {
    RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE,
    RFX_MSG_REQUEST_DATA_REGISTRATION_STATE,
    RFX_MSG_REQUEST_OPERATOR,
    RFX_MSG_REQUEST_QUERY_NETWORK_SELECTION_MODE,
    RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE
};

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoiceRegStateData, RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxDataRegStateData, RFX_MSG_REQUEST_DATA_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, RFX_MSG_REQUEST_OPERATOR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_NETWORK_SELECTION_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcNetworkRealTimeRequestHandler, RIL_CMD_PROXY_7);

RmcNetworkRealTimeRequestHandler::RmcNetworkRealTimeRequestHandler(int slot_id, int channel_id) :
        RmcNetworkHandler(slot_id, channel_id),
        m_emergency_only(0) {
    sp<RfxAtResponse> p_response;
    logV(LOG_TAG, "%s[%d] start", __FUNCTION__, slot_id);

    m_slot_id = (unsigned int) slot_id;
    m_channel_id = channel_id;

    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    resetVoiceRegStateCache(voice_reg_state_cache[m_slot_id], CACHE_GROUP_ALL);
    resetDataRegStateCache(data_reg_state_cache[m_slot_id], CACHE_GROUP_ALL);
    resetVoiceRegStateCache(urc_voice_reg_state_cache[m_slot_id], CACHE_GROUP_ALL);
    resetDataRegStateCache(urc_data_reg_state_cache[m_slot_id], CACHE_GROUP_ALL);
    resetCaCache(ca_cache[m_slot_id]);
    updateServiceStateValue();
}

RmcNetworkRealTimeRequestHandler::~RmcNetworkRealTimeRequestHandler() {
}

void RmcNetworkRealTimeRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE:
            requestVoiceRegistrationState(msg);
            break;
        case RFX_MSG_REQUEST_DATA_REGISTRATION_STATE:
            requestDataRegistrationState(msg);
            break;
        case RFX_MSG_REQUEST_OPERATOR:
            requestOperator(msg);
            mIsNeedNotifyState[m_slot_id] = true;
            // increase Id so that RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM will be skiped
            mIsNeedNotifyState_Id[m_slot_id] = mIsNeedNotifyState_Id[m_slot_id] + 1;
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

void RmcNetworkRealTimeRequestHandler::sendVoiceRegResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_VoiceRegistrationStateResponse *pData = (RIL_VoiceRegistrationStateResponse *)
            alloca(sizeof(RIL_VoiceRegistrationStateResponse));
    if (pData == NULL) goto error;
    memset(pData, 0, sizeof(RIL_VoiceRegistrationStateResponse));

    pthread_mutex_lock(&s_voiceRegStateMutex[m_slot_id]);
    pData->regState = (RIL_RegState)voice_reg_state_cache[m_slot_id]->registration_state;
    pData->rat = (RIL_RadioTechnology)voice_reg_state_cache[m_slot_id]->radio_technology;
    pData->cssSupported = voice_reg_state_cache[m_slot_id]->css;
    pData->roamingIndicator = voice_reg_state_cache[m_slot_id]->roaming_indicator;
    pData->systemIsInPrl = voice_reg_state_cache[m_slot_id]->is_in_prl;
    pData->defaultRoamingIndicator = voice_reg_state_cache[m_slot_id]->default_roaming_indicator;
    if ((urc_voice_reg_state_cache[m_slot_id]->denied_reason > 0) &&
        (urc_voice_reg_state_cache[m_slot_id]->lac == voice_reg_state_cache[m_slot_id]->lac) &&
        (urc_voice_reg_state_cache[m_slot_id]->cid == voice_reg_state_cache[m_slot_id]->cid)) {
         // we have reject cause with the same lac/cid, we use it.
        pData->reasonForDenial = urc_voice_reg_state_cache[m_slot_id]->denied_reason;
    } else {
        pData->reasonForDenial = voice_reg_state_cache[m_slot_id]->denied_reason;
    }
    if (isInService(voice_reg_state_cache[m_slot_id]->registration_state)) {
        pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
        strncpy(pData->rplmn, op_info_cache[m_slot_id]->mccMnc, 7);
        pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
    }

    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);

    if (isInService(voice_reg_state_cache[m_slot_id]->registration_state)) {
        fillVoiceCellIdentity(pData->cellIdentity);
        switch(pData->cellIdentity.cellInfoType) {
            case RIL_CELL_INFO_TYPE_LTE: {
                // lte vops info
                pData->lteVopsInfo.isVopsSupported = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS);
                pthread_mutex_lock(&imsEmergencyMutex[m_slot_id]);
                pData->lteVopsInfo.isEmcBearerSupported = imsEmergencyInfo[m_slot_id]->volteEmcSupported;
                pthread_mutex_unlock(&imsEmergencyMutex[m_slot_id]);
                // NR non-standalone
                pData->nrIndicators.isDcNrRestricted = data_reg_state_cache[m_slot_id]->dcnr_restricted;
                pData->nrIndicators.isNrAvailable = data_reg_state_cache[m_slot_id]->endc_sib;
                pData->nrIndicators.isEndcAvailable = data_reg_state_cache[m_slot_id]->endc_available;
                pData->attachResultType =
                    isInService(data_reg_state_cache[m_slot_id]->registration_state) ? COMBINED : NONE;
                pData->extraInfo = (mdEreg[m_slot_id]->stat == 6 ?
                    EXTRA_SMS_ONLY : (mdEreg[m_slot_id]->stat == 9 ? EXTRA_CSFB_NOT_PREFERRED : EXTRA_NONE));
                break;
            }
            case RIL_CELL_INFO_TYPE_NR:
                // TODO: NR vops info need updated
                pData->nrVopsInfo.vopsSupported = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS);
                pthread_mutex_lock(&imsEmergencyMutex[m_slot_id]);
                pData->nrVopsInfo.emcSupported = imsEmergencyInfo[m_slot_id]->vonrEmcSupported;
                pData->nrVopsInfo.emfSupported = imsEmergencyInfo[m_slot_id]->vonrEmfSupported;
                pthread_mutex_unlock(&imsEmergencyMutex[m_slot_id]);
                break;

            case RIL_CELL_INFO_TYPE_GSM:
            case RIL_CELL_INFO_TYPE_WCDMA:
            case RIL_CELL_INFO_TYPE_TD_SCDMA:
            case RIL_CELL_INFO_TYPE_CDMA:
            default:
                break;
        }
    } else {
        pData->cellIdentity.cellInfoType = RIL_CELL_INFO_TYPE_NONE;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoiceRegStateData(pData, sizeof(RIL_VoiceRegistrationStateResponse)), msg, false);

    // response to TeleCore
    responseToTelCore(response);
    clearCellIdentity(pData->cellIdentity);
    return;
error:
    logE(LOG_TAG, "sendVoiceRegResponse something wroing");
    return;
}

void RmcNetworkRealTimeRequestHandler::combineVoiceRegState(const sp<RfxMclMessage>& msg) {
    sendVoiceRegResponse(msg);
    updateServiceStateValue();
}

void RmcNetworkRealTimeRequestHandler::printVoiceCache(RIL_VOICE_REG_STATE_CACHE cache) {
    char* lac_s = getMask(cache.lac);
    char* cid_s = getMask(cache.cid);
    char* bsla_s = getMask(cache.base_station_latitude);
    char* bslo_s = getMask(cache.base_station_longitude);
    char* bsid_s = getMask(cache.base_station_id);
    char* sid_s = getMask(cache.system_id);
    char* nid_s = getMask(cache.network_id);
    if (!lac_s || !cid_s || !bsla_s || !bslo_s || !bsid_s || !sid_s || !nid_s) {
        logE(LOG_TAG, "[%s] can not get memeory to print log", __func__);
    } else {
        logV(LOG_TAG, " VoiceCache: "
            "registration_state=%d lac=%s cid=%s radio_technology=%d base_station_id=%s "
            "base_station_latitude=%s base_station_longitude=%s css=%d system_id=%s "
            "network_id=%s roaming_indicator=%d is_in_prl=%d default_roaming_indicator=%d "
            "denied_reason=%d network_exist=%d", cache.registration_state, lac_s,
            cid_s, cache.radio_technology, bsid_s, bsla_s,
            bslo_s, cache.css, sid_s, nid_s,
            cache.roaming_indicator, cache.is_in_prl, cache.default_roaming_indicator,
            cache.denied_reason, cache.network_exist);
    }
    if (lac_s) free(lac_s);
    if (cid_s) free(cid_s);
    if (bsla_s) free(bsla_s);
    if (bslo_s) free(bslo_s);
    if (bsid_s) free(bsid_s);
    if (sid_s) free(sid_s);
    if (nid_s) free(nid_s);
}

int RmcNetworkRealTimeRequestHandler::convertToAndroidRegState(unsigned int uiRegState, bool isVoice) {
    unsigned int uiRet = 0, convertedState = 0;

    logV(LOG_TAG, "convertToAndroidRegState s_emergencly_only=%d"
        ", m_cdma_emergency_only=%d"
        ", m_ims_ecc_only=%d",
        m_emergency_only,
        m_cdma_emergency_only,
        m_ims_ecc_only);
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

void RmcNetworkRealTimeRequestHandler::requestVoiceRegistrationState(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    int count = 3, cntCdma = 0;
    int cause_type;
    bool m4gRat = false;
    bool m5gRat = false;
    bool mC2kRat = false;

    int cdma_network_exist = 0;

    int radio_state = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, 0);
    MD_EREG *mMdEreg = mdEreg[m_slot_id];
    MD_ECELL *mMdEcell = mdEcell[m_slot_id];

    resetVoiceRegStateCache(voice_reg_state_cache[m_slot_id], CACHE_GROUP_COMMON_REQ);

    /* +EREG: <n>,<stat> */
    /* +EREG: <n>,<stat>,<lac>,<ci>,<eAct>,<nwk_existence >,<roam_indicator> */
    /* +EREG: <n>,<stat>,<lac>,<ci>,<eAct>,<nwk_existence >,<roam_indicator>,<cause_type>,<reject_cause> */

    dumpEregCache(__func__, mMdEreg, mMdEcell, true);

    pthread_mutex_lock(&mdEregMutex[m_slot_id]);
    pthread_mutex_lock(&s_voiceRegStateMutex[m_slot_id]);

    /* <stat> */
    voice_reg_state_cache[m_slot_id]->registration_state = mMdEreg->stat;
    if (voice_reg_state_cache[m_slot_id]->registration_state > 10 )  // for LTE
    {
        // TODO: add C2K flag
        if (voice_reg_state_cache[m_slot_id]->registration_state < 101 ||
                voice_reg_state_cache[m_slot_id]->registration_state > 104) {// for C2K
            logE(LOG_TAG, "The value in the field <stat> is not valid: %d",
                 voice_reg_state_cache[m_slot_id]->registration_state);
            goto error;
        }
    }

    // For LTE and C2K
    voice_reg_state_cache[m_slot_id]->registration_state =
            convertRegState(voice_reg_state_cache[m_slot_id]->registration_state, true);

    /* <lac> */
    voice_reg_state_cache[m_slot_id]->lac = mMdEreg->lac;

    /* <ci> */
    voice_reg_state_cache[m_slot_id]->cid = mMdEreg->ci;

    /* <eAct> */
    voice_reg_state_cache[m_slot_id]->radio_technology = mMdEreg->eAct;

    count = 4;
    if (voice_reg_state_cache[m_slot_id]->radio_technology > 0x8000)  // LTE-CA is 0x2000
    {
        logE(LOG_TAG, "The value in the eAct is not valid: %d",
             voice_reg_state_cache[m_slot_id]->radio_technology);
        goto error;
    }

    /* <eAct> mapping */
    if (!isInService(voice_reg_state_cache[m_slot_id]->registration_state))
    {
        int cs_type = convertCSNetworkType(voice_reg_state_cache[m_slot_id]->radio_technology);
        if (cs_type == RADIO_TECH_LTE) {
            m4gRat = true;
        } else if (cs_type == RADIO_TECH_NR) {
            m5gRat = true;
        } else if (cs_type == RADIO_TECH_1xRTT ||
                cs_type == RADIO_TECH_EVDO_A ||
                cs_type == RADIO_TECH_EHRPD) {
            mC2kRat = true;
        }
        voice_reg_state_cache[m_slot_id]->radio_technology = 0;
    }
    else
    {
        voice_reg_state_cache[m_slot_id]->radio_technology =
                convertCSNetworkType(voice_reg_state_cache[m_slot_id]->radio_technology);
    }

    voice_reg_state_cache[m_slot_id]->network_exist = mMdEreg->nwk_existence;

    voice_reg_state_cache[m_slot_id]->roaming_indicator = mMdEreg->roam_indicator;

    if (!isInService(voice_reg_state_cache[m_slot_id]->registration_state)) {
        voice_reg_state_cache[m_slot_id]->roaming_indicator = m_ps_roaming_ind;
    }

    if (voice_reg_state_cache[m_slot_id]->roaming_indicator >= 255) {
        voice_reg_state_cache[m_slot_id]->roaming_indicator = 1;
    }

    /* <cause_type> */
    cause_type = mMdEreg->cause_type;

    /* <reject_cause> */
    voice_reg_state_cache[m_slot_id]->denied_reason = mMdEreg->reject_cause;
    count = 14;
    pthread_mutex_unlock(&mdEregMutex[m_slot_id]);

    if(isSpecificVoiceStateConfigured()
        && !isInService(voice_reg_state_cache[m_slot_id]->registration_state)) {
        int reg_info = 0;
        int ext_info = 0;
        p_response = NULL;

        // +CIREG: <n>,<reg_info>[,<ext_info>]
        p_response = atSendCommandSingleline("AT+CIREG?", "+CIREG:");

        // check error
        int err = p_response->getError();
        if (err != 0 ||
                p_response == NULL ||
                p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) goto done;

        // handle intermediate
        RfxAtLine* line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto done;

        /* Enables or disables reporting of changes */
        int skip = line->atTokNextint(&err);
        if (err < 0) goto done;

        /* Indicates the IMS registration status */
        reg_info = line->atTokNextint(&err);
        if (err < 0) goto done;

        if (reg_info == 1) {
            ext_info = line->atTokNexthexint(&err);
            if (err < 0) goto done;
            if ((ext_info & 0x1) == 1) {
                // VoLTE attach, query CGREG state and return
                p_response = NULL;
                p_response = atSendCommandSingleline("AT+EGREG?", "+EGREG:");

                // check error
                err = p_response->getError();
                if (err != 0 ||
                        p_response == NULL ||
                        p_response->getSuccess() == 0 ||
                        p_response->getIntermediates() == NULL) goto done;

                // handle intermediate
                line = p_response->getIntermediates();

                // go to start position
                line->atTokStart(&err);
                if (err < 0) goto done;

                /* <n> */
                skip = line->atTokNextint(&err);
                if (err < 0) goto done;

                /* <stat> */
                reg_info = line->atTokNextint(&err);
                if (err < 0 || reg_info > 10) {
                    logE(LOG_TAG, "The value in the field <stat> is not valid: %d", reg_info);
                    goto done;
                }
                int state = convertRegState(reg_info, true);
                voice_reg_state_cache[m_slot_id]->registration_state = state;
                if (isInService(state)) {
                    voice_reg_state_cache[m_slot_id]->denied_reason = 0;
                }

                if (line->atTokHasmore()) {
                    /* <lac> */
                    voice_reg_state_cache[m_slot_id]->lac = line->atTokNexthexint(&err);
                    if (err < 0) {
                        logE(LOG_TAG, "The value in the field <lac> is not valid: %d",
                                voice_reg_state_cache[m_slot_id]->lac);
                        goto done;
                    }

                    /* <cid> */
                    char* strCid = line->atTokNextstr(&err);
                    if (err < 0) {
                        logE(LOG_TAG, "Failed to get valid CID");
                        goto done;
                    }
                    voice_reg_state_cache[m_slot_id]->cid = strtoull(strCid, NULL, 16);

                    /* <eAct> */
                    voice_reg_state_cache[m_slot_id]->radio_technology =
                        convertCSNetworkType(line->atTokNextint(&err));
                    if (err < 0) {
                        logE(LOG_TAG, "No eAct in command");
                        goto done;
                    }
                }
            }
        }
    }
done:

    // ECC support - START
    // CDMA ECC
    if (voice_reg_state_cache[m_slot_id]->network_exist == 1) {
        cdma_network_exist = 1;
    } else {
        if (((voice_reg_state_cache[m_slot_id]->registration_state == 0) ||
                    (voice_reg_state_cache[m_slot_id]->registration_state == 2) ||
                    (voice_reg_state_cache[m_slot_id]->registration_state == 3)) &&
                    ((voice_reg_state_cache[m_slot_id]->cid != 0x0fffffff) &&
                    (voice_reg_state_cache[m_slot_id]->lac != 0xffff) &&
                    (voice_reg_state_cache[m_slot_id]->cid != (unsigned int)-1) &&
                    (voice_reg_state_cache[m_slot_id]->lac != (unsigned int)-1)) &&
                    (!m4gRat) &&
                    (!m5gRat)) {
                cdma_network_exist = 1;
            }
    }
    if ((cdma_network_exist == 1) && (radio_state == RADIO_STATE_ON) &&
            !isInService(voice_reg_state_cache[m_slot_id]->registration_state) &&
            !isInService(data_reg_state_cache[m_slot_id]->registration_state)) {
        m_cdma_emergency_only = 1;
    } else {
        m_cdma_emergency_only = 0;
    }
    // TODO: restricted state for dual SIM
    if(voice_reg_state_cache[m_slot_id]->registration_state == 4) {
        /* registration_state(4) is 'Unknown' */
        // logD(LOG_TAG, "No valid info to distinguish limited service and no service");
    } else {
        // if cid is 0x0fffffff means it is invalid
        // if lac is 0xffff means it is invalid
        if ((!isInService(voice_reg_state_cache[m_slot_id]->registration_state) &&
                !isInService(data_reg_state_cache[m_slot_id]->registration_state)) &&
                ((voice_reg_state_cache[m_slot_id]->cid != 0x0fffffff) &&
                (voice_reg_state_cache[m_slot_id]->lac != 0xffff) &&
                (voice_reg_state_cache[m_slot_id]->cid != (unsigned int)-1) &&
                (voice_reg_state_cache[m_slot_id]->lac != (unsigned int)-1)) &&
                // do not set ECC when it is LTE/NR/C2K. ECC depends IMS.
                (!m4gRat) &&
                (!m5gRat) &&
                (!mC2kRat)) {
            if (m_emergency_only == 0) {
                m_emergency_only = 1;
                // logD(LOG_TAG, "Set slot%d m_emergencly_only to true", m_slot_id);
            }
        } else {
            if(m_emergency_only == 1) {
                m_emergency_only = 0;
                // logD(LOG_TAG, "Set slot%d s_emergencly_only to false", m_slot_id);
            }
        }
    }

    voice_reg_state_cache[m_slot_id]->registration_state =
            convertToAndroidRegState(voice_reg_state_cache[m_slot_id]->registration_state, true);
    // ECC support -END

    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);

    if (RfxNwServiceState::isCdmaGroup(voice_reg_state_cache[m_slot_id]->radio_technology) ||
        RfxNwServiceState::isCdmaGroup(data_reg_state_cache[m_slot_id]->radio_technology)) {
        //only if current rat is CDMA group, there is meaning to query cdma information.
        cntCdma = requestVoiceRegistrationStateCdma(msg);
    }
    if (cntCdma == 0) {
        // response contains error
        resetVoiceRegStateCache(voice_reg_state_cache[m_slot_id], CACHE_GROUP_C2K);
        // logD(LOG_TAG, "C2K voice response error");
    } else if (cntCdma > count) {
        count = cntCdma;
    }
    combineVoiceRegState(msg);

    return;

error:
    pthread_mutex_unlock(&mdEregMutex[m_slot_id]);
    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);
    logE(LOG_TAG, "requestVoiceRegistrationState must never return an error when radio is on");
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxStringsData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;
}

int RmcNetworkRealTimeRequestHandler::requestVoiceRegistrationStateCdma(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    int err = 0;
    ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
    pthread_mutex_lock(&s_voiceRegStateMutex[m_slot_id]);
    if (mViaHandler != NULL) {
        CDMA_CELL_LOCATION_INFO cdma_location;
        memset(&cdma_location, 0, sizeof(CDMA_CELL_LOCATION_INFO));
        err = mViaHandler->getCdmaLocationInfo(this, &cdma_location);
        if (err < 0) {
            goto error;
        }
        voice_reg_state_cache[m_slot_id]->system_id = cdma_location.system_id;
        voice_reg_state_cache[m_slot_id]->network_id = cdma_location.network_id;
        voice_reg_state_cache[m_slot_id]->base_station_id = cdma_location.base_station_id;
        voice_reg_state_cache[m_slot_id]->base_station_latitude = cdma_location.base_station_latitude;
        voice_reg_state_cache[m_slot_id]->base_station_longitude = cdma_location.base_station_longitude;
    }
    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);
    return 16;

error:
    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);
    // logE(LOG_TAG, "AT+VLOCINFO? response error");
    return 0;
}

static unsigned int convertCellSpeedSupport(unsigned int uiResponse)
{
    // Cell speed support is bitwise value of cell capability:
    // bit7 0x80  bit6 0x40  bit5 0x20  bit4 0x10  bit3 0x08  bit2 0x04  bit1 0x02  bit0 0x01
    // Dual-Cell  HSUPA+     HSDPA+     HSUPA      HSDPA      UMTS       EDGE       GPRS
    unsigned int uiRet = 0;

    if ((uiResponse & 0x2000) != 0) {
        //support carrier aggregation (LTEA)
        uiRet = 19; // ServiceState.RIL_RADIO_TECHNOLOGY_LTE_CA
    } else if ((uiResponse & 0x1000) != 0 ||
            (uiResponse & 0x4000) != 0) {
        uiRet = 14; // ServiceState.RIL_RADIO_TECHNOLOGY_LTE
    } else if ((uiResponse & 0x80) != 0 ||
            (uiResponse & 0x40) != 0 ||
            (uiResponse & 0x20) != 0) {
        uiRet = 15; // ServiceState.RIL_RADIO_TECHNOLOGY_HSPAP
    } else if ((uiResponse & 0x10) != 0 &&
            (uiResponse & 0x08) != 0) {
        uiRet = 11; // ServiceState.RIL_RADIO_TECHNOLOGY_HSPA
    } else if ((uiResponse & 0x10) != 0) {
        uiRet = 10; // ServiceState.RIL_RADIO_TECHNOLOGY_HSUPA
    } else if ((uiResponse & 0x08) != 0) {
        uiRet = 9;  // ServiceState.RIL_RADIO_TECHNOLOGY_HSDPA
    } else if ((uiResponse & 0x04) != 0) {
        uiRet = 3;  // ServiceState.RIL_RADIO_TECHNOLOGY_UMTS
    } else if ((uiResponse & 0x02) != 0) {
        uiRet = 2;  // ServiceState.RIL_RADIO_TECHNOLOGY_EDGE
    } else if ((uiResponse & 0x01) != 0) {
        uiRet = 1;  // ServiceState.RIL_RADIO_TECHNOLOGY_GPRS
    } else {
        uiRet = 0;  // ServiceState.RIL_RADIO_TECHNOLOGY_UNKNOWN
    }

    return uiRet;
}

static unsigned int convertPSBearerCapability(unsigned int uiResponse)
{
    /*
     *typedef enum
     *{
     *    L4C_NONE_ACTIVATE = 0,
     *    L4C_GPRS_CAPABILITY,
     *    L4C_EDGE_CAPABILITY,
     *    L4C_UMTS_CAPABILITY,
     *    L4C_HSDPA_CAPABILITY, //mac-hs
     *    L4C_HSUPA_CAPABILITY, //mac-e/es
     *    L4C_HSDPA_HSUPA_CAPABILITY, //mac-hs + mac-e/es
     *
     *    L4C_HSDPAP_CAPABILITY, //mac-ehs
     *    L4C_HSDPAP_UPA_CAPABILITY, //mac-ehs + mac-e/es
     *    L4C_HSUPAP_CAPABILITY, //mac-i/is
     *    L4C_HSUPAP_DPA_CAPABILITY, //mac-hs + mac-i/is
     *    L4C_HSPAP_CAPABILITY, //mac-ehs + mac-i/is
     *    L4C_DC_DPA_CAPABILITY, //(DC) mac-hs
     *    L4C_DC_DPA_UPA_CAPABILITY, //(DC) mac-hs + mac-e/es
     *    L4C_DC_HSDPAP_CAPABILITY, //(DC) mac-ehs
     *    L4C_DC_HSDPAP_UPA_CAPABILITY, //(DC) mac-ehs + mac-e/es
     *    L4C_DC_HSUPAP_DPA_CAPABILITY, //(DC) mac-hs + mac-i/is
     *    L4C_DC_HSPAP_CAPABILITY, //(DC) mac-ehs + mac-i/is
     *    L4C_LTE_CAPABILITY
     *} l4c_data_bearer_capablility_enum;
     */

    unsigned int uiRet = 0;

    switch (uiResponse)
    {
        case 1:
            uiRet = 1;                           // ServiceState.RIL_RADIO_TECHNOLOGY_GPRS
            break;
        case 2:
            uiRet = 2;                           // ServiceState.RIL_RADIO_TECHNOLOGY_EDGE
            break;
        case 3:
            uiRet = 3;                           // ServiceState.RIL_RADIO_TECHNOLOGY_UMTS
            break;
        case 4:
            uiRet = 9;                           // ServiceState.RIL_RADIO_TECHNOLOGY_HSDPA
            break;
        case 5:
            uiRet = 10;                          // ServiceState.RIL_RADIO_TECHNOLOGY_HSUPA
            break;
        case 6:
            uiRet = 11;                          // ServiceState.RIL_RADIO_TECHNOLOGY_HSPA
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            uiRet = 15;                          // ServiceState.RIL_RADIO_TECHNOLOGY_HSPAP
            break;
        case 18:
            uiRet = 14;                          // ServiceState.RIL_RADIO_TECHNOLOGY_LTE
            break;
        default:
            uiRet = 0;                           // ServiceState.RIL_RADIO_TECHNOLOGY_UNKNOWN
            break;
    }

    return uiRet;
}

void RmcNetworkRealTimeRequestHandler::sendDataRegResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    MD_EREG* mMdEreg = mdEreg[m_slot_id];
    int csRegState = mMdEreg->stat;
    int csAct = mMdEreg->eAct;
    RIL_DataRegistrationStateResponse *pData = (RIL_DataRegistrationStateResponse *)
            alloca(sizeof(RIL_DataRegistrationStateResponse));
    if (pData == NULL) goto error;
    memset(pData, 0, sizeof(RIL_DataRegistrationStateResponse));
    // consider iwlan network type for legacy. keep state as cellular ps state.
    if (s_iwlan_in_legacy_mode && (ril_wfc_reg_status[m_slot_id] == 1)) {
        data_reg_state_cache[m_slot_id]->radio_technology = RADIO_TECH_IWLAN;
        if (!isInService(data_reg_state_cache[m_slot_id]->registration_state)) {
            // if cellular ps is oos, set state as in service for iwlan
            data_reg_state_cache[m_slot_id]->registration_state = RIL_REG_HOME;
        }
    }
    pData->regState = (RIL_RegState)data_reg_state_cache[m_slot_id]->registration_state;
    pData->rat = (RIL_RadioTechnology)data_reg_state_cache[m_slot_id]->radio_technology;
    if (urc_data_reg_state_cache[m_slot_id]->denied_reason > 0) {
         // we have reject cause with the same lac/cid, we use it.
        pData->reasonDataDenied = urc_data_reg_state_cache[m_slot_id]->denied_reason;
    } else {
        pData->reasonDataDenied = data_reg_state_cache[m_slot_id]->denied_reason;
    }
    pData->maxDataCalls = data_reg_state_cache[m_slot_id]->max_simultaneous_data_call;
    if (isInService(data_reg_state_cache[m_slot_id]->registration_state)) {
        pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
        strncpy(pData->rplmn, op_info_cache[m_slot_id]->mccMnc, 7);
        pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
    }

    // provide cell info only when it's in service as usual.
    if (isInService(data_reg_state_cache[m_slot_id]->registration_state)) {
        fillDataCellIdentity(pData->cellIdentity);
        switch(pData->cellIdentity.cellInfoType) {
            case RIL_CELL_INFO_TYPE_LTE: {
                // lte vops info
                pData->lteVopsInfo.isVopsSupported = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOPS);
                pthread_mutex_lock(&imsEmergencyMutex[m_slot_id]);
                pData->lteVopsInfo.isEmcBearerSupported = imsEmergencyInfo[m_slot_id]->volteEmcSupported;
                pthread_mutex_unlock(&imsEmergencyMutex[m_slot_id]);
                // NR non-standalone
                pData->nrIndicators.isDcNrRestricted = data_reg_state_cache[m_slot_id]->dcnr_restricted;
                pData->nrIndicators.isNrAvailable = data_reg_state_cache[m_slot_id]->endc_sib;
                pData->nrIndicators.isEndcAvailable = data_reg_state_cache[m_slot_id]->endc_available;
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
                pthread_mutex_lock(&imsEmergencyMutex[m_slot_id]);
                pData->nrVopsInfo.emcSupported = imsEmergencyInfo[m_slot_id]->vonrEmcSupported;
                pData->nrVopsInfo.emfSupported = imsEmergencyInfo[m_slot_id]->vonrEmfSupported;
                pthread_mutex_unlock(&imsEmergencyMutex[m_slot_id]);
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
    // response to TeleCore
    responseToTelCore(response);
    clearCellIdentity(pData->cellIdentity);
    return;
error:
    logE(LOG_TAG, "sendDataRegResponse something wroing");
    return;
}

void RmcNetworkRealTimeRequestHandler::combineDataRegState(const sp<RfxMclMessage>& msg) {
    updateServiceStateValue();
    sendDataRegResponse(msg);
}

void RmcNetworkRealTimeRequestHandler::printDataCache() {
    char* lac_s = getMask(data_reg_state_cache[m_slot_id]->lac);
    char* cid_s = getMask((int) data_reg_state_cache[m_slot_id]->cid);
    if (!lac_s || !cid_s) {
        logE(LOG_TAG, "[%s] can not get memeory to print log", __func__);
    } else {
        logV(LOG_TAG, "printDataCache: reg=%d, lac=%s, cid=%s, rat=%d, reason=%d",
                 data_reg_state_cache[m_slot_id]->registration_state,
                 lac_s,
                 cid_s,
                 data_reg_state_cache[m_slot_id]->radio_technology,
                 data_reg_state_cache[m_slot_id]->denied_reason);
    }
    if (lac_s) free(lac_s);
    if (cid_s) free(cid_s);
}

void RmcNetworkRealTimeRequestHandler::requestDataRegistrationState(const sp<RfxMclMessage>& msg)
{
    /* +EGREG: <n>, <stat>, <lac>, <cid>, <eAct>, <rac>,
            <nwk_existence>, <roam_indicator>, <cause_type>, <reject_cause> */

    int exist = 0;
    sp<RfxAtResponse> p_response;
    int cause_type;
    MD_EGREG *mMdEgreg = mdEgreg[m_slot_id];
    MD_ECELL *mMdEcell = mdEcell[m_slot_id];
    int i = 0, nr_cap = -1;
    nr_cap = checkNrCapability(m_slot_id);
    // if support IMS ECC, this will be 1
    int ims_ecc_support = ims_ecc_state[m_slot_id];
    pthread_mutex_lock(&mdEregMutex[m_slot_id]);
    MD_EREG *mMdEreg = mdEreg[m_slot_id];
    int voiceRegState = convertRegState(mMdEreg->stat, true);
    pthread_mutex_unlock(&mdEregMutex[m_slot_id]);

    pthread_mutex_lock(&mdEgregMutex[m_slot_id]);

    resetDataRegStateCache(data_reg_state_cache[m_slot_id], CACHE_GROUP_COMMON_REQ);

    dumpEgregCache(__func__, mMdEgreg, mMdEcell, true);
    //logV(LOG_TAG, "requestDataRegistrationState nr_display_rule: %d", nr_display_rule[m_slot_id]);

    /* <stat> */
    data_reg_state_cache[m_slot_id]->registration_state = mMdEgreg->stat;

    if (data_reg_state_cache[m_slot_id]->registration_state > 10 )  //for LTE
    {
        // TODO: add C2K flag
        if (data_reg_state_cache[m_slot_id]->registration_state < 101 ||
                data_reg_state_cache[m_slot_id]->registration_state > 104)  // for C2K
        {
            logE(LOG_TAG, "The value in the field <stat> is not valid: %d",
                 data_reg_state_cache[m_slot_id]->registration_state);
            goto error;
        }
    }

    //For LTE and C2K
    data_reg_state_cache[m_slot_id]->registration_state =
            convertRegState(data_reg_state_cache[m_slot_id]->registration_state, false);

    /* <lac> */
    data_reg_state_cache[m_slot_id]->lac = mMdEgreg->lac;

    /* <ci> */
    data_reg_state_cache[m_slot_id]->cid = mMdEgreg->ci;

    /* <eAct> */
    data_reg_state_cache[m_slot_id]->radio_technology = mMdEgreg->eAct;
    if (data_reg_state_cache[m_slot_id]->radio_technology > 0x8000)  // NR is 0x8000
    {
        logE(LOG_TAG, "The value in the eAct is not valid: %d",
             data_reg_state_cache[m_slot_id]->radio_technology);
        goto error;
    }

    /* <eAct> mapping */
    if (!isInService(data_reg_state_cache[m_slot_id]->registration_state))
    {
        data_reg_state_cache[m_slot_id]->lac = -1;
        data_reg_state_cache[m_slot_id]->cid = -1;
        data_reg_state_cache[m_slot_id]->radio_technology = 0;
    }
    else
    {
        data_reg_state_cache[m_slot_id]->radio_technology =
                convertPSNetworkType(data_reg_state_cache[m_slot_id]->radio_technology);
    }

    exist = mMdEgreg->nwk_existence;

    m_ps_roaming_ind = mMdEgreg->roam_indicator;

    // if NR support, [<dcnr_restricted>],[<endc_sib>],[<endc_available>]
    if (isNrSupported()) {
        if (skipDCNRRestrictedState((const char *)op_info_cache[m_slot_id]->mccMnc)) {
            //logD(LOG_TAG, "skip DCNR Restricted State for %s", op_info_cache[m_slot_id]->mccMnc);
            data_reg_state_cache[m_slot_id]->dcnr_restricted = 0;
        } else {
            data_reg_state_cache[m_slot_id]->dcnr_restricted = mMdEgreg->dcnr_restricted;
        }

        data_reg_state_cache[m_slot_id]->endc_sib = mMdEgreg->endc_sib == 2 ? 1 : 0;

        data_reg_state_cache[m_slot_id]->endc_available = mMdEgreg->endc_available;
    }

    // if eact is 4000, we can force it.
    if (mMdEgreg->eAct == 0x4000) {
        data_reg_state_cache[m_slot_id]->dcnr_restricted = 0;
        data_reg_state_cache[m_slot_id]->endc_sib = 1;
        data_reg_state_cache[m_slot_id]->endc_available = 1;
    }
    if (nr_cap == -1) {
        // clear when ue is without nr
        data_reg_state_cache[m_slot_id]->dcnr_restricted = 0;
        data_reg_state_cache[m_slot_id]->endc_sib = 0;
        data_reg_state_cache[m_slot_id]->endc_available = 0;
    } else if (((nr_display_rule[m_slot_id] & NR_DISPALY_NR_CACHE) == NR_DISPALY_NR_CACHE) &&
        (data_reg_state_cache[m_slot_id]->radio_technology == 14 ||  // LTE
         data_reg_state_cache[m_slot_id]->radio_technology == 19) &&   // LTE_CA
        (data_reg_state_cache[m_slot_id]->cid != -1) &&  // ignore invalid cell
        (data_reg_state_cache[m_slot_id]->lac != -1)) {
        pthread_mutex_lock(&nrCacheMutex);
        dumpLteEndcMaxCapability(m_slot_id);
        for (i = 0; i < MAX_LTE_ENDC_MAX_CAPABILITY; i++) {
            if (lteMaxNr[i].cid == data_reg_state_cache[m_slot_id]->cid &&  // cid
                lteMaxNr[i].lacTac == data_reg_state_cache[m_slot_id]->lac) {  // lac
                // if lac/cid are the same as previous one,
                // we use maximum capability among previous and current one.
                /* we should keep dcnr_restricted 0 as possible as we can.
                data_reg_state_cache[m_slot_id]->dcnr_restricted =
                    lteMaxNr[m_slot_id][i].dcnr_restricted > 0 ?
                    1 : data_reg_state_cache[m_slot_id]->dcnr_restricted;
                */
                data_reg_state_cache[m_slot_id]->endc_sib = 1;
                data_reg_state_cache[m_slot_id]->endc_available = 1;
                break;
            }
        }
        pthread_mutex_unlock(&nrCacheMutex);
    }

    if (((nr_display_rule[m_slot_id] & NR_DISPALY_LTE_CONNECTED_STATE)
                == NR_DISPALY_LTE_CONNECTED_STATE) &&
        (data_reg_state_cache[m_slot_id]->radio_technology == 14 ||
        data_reg_state_cache[m_slot_id]->radio_technology == 19) &&  // LTE or LTE_CA
        (lte_tdd_connection_state[m_slot_id] == 1 ||
        lte_fdd_connection_state[m_slot_id] == 1) &&  // connected state
        mMdEgreg->eAct != 0x4000) {  // NSA is not connected.
        // clear sib info so UE shows 4G when LTE connected instead of NR connected.
        logD(LOG_TAG, "Clear sib because LTE connected state");
        data_reg_state_cache[m_slot_id]->dcnr_restricted = 0;
        data_reg_state_cache[m_slot_id]->endc_sib = 0;
        data_reg_state_cache[m_slot_id]->endc_available = 0;
    }

    if ((nr_cap == 1) && isJpMode()) {
        // rrc connected mode && lte (if it's nsa, all timers stop)
        if ((lte_tdd_connection_state[m_slot_id] == 1 ||
                lte_fdd_connection_state[m_slot_id] == 1) &&
            (data_reg_state_cache[m_slot_id]->radio_technology == 14 || // 14: LTE or NSA
                data_reg_state_cache[m_slot_id]->radio_technology == 19)) { // 19: LTE_CA
            // check going state first so UE show 5g if one of timer is on going
            if (jp_mode_timer1_state[m_slot_id] == JP_MODE_TIMER_STATE_GOING) {
                data_reg_state_cache[m_slot_id]->endc_sib = 1;
                data_reg_state_cache[m_slot_id]->endc_available = 1;
            } else if (jp_mode_timer1_state[m_slot_id] == JP_MODE_TIMER_STATE_STARTING &&
                mMdEgreg->endc_sib == 2 &&
                isInService(data_reg_state_cache[m_slot_id]->registration_state)) {
                //still show 5G at STARTING state && network support 5G
                data_reg_state_cache[m_slot_id]->endc_sib = 1;
                data_reg_state_cache[m_slot_id]->endc_available = 1;
            } else if (jp_mode_timer2_state[m_slot_id] == JP_MODE_TIMER_STATE_GOING) {
                data_reg_state_cache[m_slot_id]->endc_sib = 1;
                data_reg_state_cache[m_slot_id]->endc_available = 1;
            } else if (mMdEgreg->eAct != 0x4000) {
                // as long as timerX is not ongoing
                // we clear NR info when UE is in connected mode and not in NSA.
                data_reg_state_cache[m_slot_id]->endc_sib = 0;
                data_reg_state_cache[m_slot_id]->endc_available = 0;
            }
        } else {
        // idle mode or others, do nothing
        }
    }

    /* <cause_type> */
    cause_type = mMdEgreg->cause_type;

    if (isFemtocellSupport()) {
        isFemtoCell(data_reg_state_cache[m_slot_id]->registration_state,
                (int) data_reg_state_cache[m_slot_id]->cid,
                data_reg_state_cache[m_slot_id]->radio_technology);
    }

    /* <reject_cause> */
    data_reg_state_cache[m_slot_id]->denied_reason = mMdEgreg->reject_cause;

    // update nr ca band info for SA or scg connected state
    if (!(isInService(data_reg_state_cache[m_slot_id]->registration_state) &&
        (mMdEgreg->eAct == 0x4000 || mMdEgreg->eAct == 0x8000))) {
        resetNrCaCache(nr_ca_cache[m_slot_id]);
    }

    pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);
    // Query PSBEARER when PS registered
    if (isInService(data_reg_state_cache[m_slot_id]->registration_state)
               && data_reg_state_cache[m_slot_id]->radio_technology != 6
               && data_reg_state_cache[m_slot_id]->radio_technology != 8
               && data_reg_state_cache[m_slot_id]->radio_technology != 13
               && data_reg_state_cache[m_slot_id]->radio_technology != 20) {
        requestDataRegistrationStateGsm();
    } else {
        resetNwCfgCache(nw_cfg_cache[m_slot_id]);
        resetCaCache(ca_cache[m_slot_id]);
        m_dc_support[m_slot_id] = false;
    }

    // IMS ecc
    if(!isInService(data_reg_state_cache[m_slot_id]->registration_state) &&
            !isInService(voiceRegState) && (ims_ecc_support > 0)) {
        m_ims_ecc_only = 1;
    } else {
        m_ims_ecc_only = 0;
    }
    data_reg_state_cache[m_slot_id]->registration_state =
        convertToAndroidRegState(data_reg_state_cache[m_slot_id]->registration_state, false);
    combineDataRegState(msg);

    // After data_reg & ca_cache is updated, send PhysicalChannelConfigs URC
    sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(),
            RIL_CMD_PROXY_3, m_slot_id);
    return;

error:
    pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);
    logE(LOG_TAG, "requestDataRegistrationState must never return an error when radio is on");
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxStringsData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;
}

void RmcNetworkRealTimeRequestHandler::requestDataRegistrationStateGsm()
{
    int err;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;
    int psBearerCount = 0;
    int cell_data_speed = 0;
    int max_data_bearer = 0;

    //support carrier aggregation (LTEA)
    int ignoreMaxDataBearerCapability = 0;

    // send AT command
    p_response = atSendCommandSingleline("AT+PSBEARER?", "+PSBEARER:");

    // check error
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) goto skipR7R8;

    // handle intermediate
    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto skipR7R8;

    /* <cell_data_speed_support> */
    cell_data_speed = line->atTokNextint(&err);
    if (err < 0) goto skipR7R8;
    psBearerCount++;

    // <max_data_bearer_capability> is only support on 3G
    if (cell_data_speed >= 0x1000){
        ignoreMaxDataBearerCapability = 1;
    }

    cell_data_speed = convertCellSpeedSupport(cell_data_speed);

    /* <max_data_bearer_capability> */
    max_data_bearer = line->atTokNextint(&err);
    if (err < 0) goto skipR7R8;
    psBearerCount++;

    if (!ignoreMaxDataBearerCapability) {
        max_data_bearer = convertPSBearerCapability(max_data_bearer);
    } else {
        max_data_bearer = 0;  // ServiceState.RIL_RADIO_TECHNOLOGY_UNKNOWN
    }

    updateDcStatus(cell_data_speed, max_data_bearer);

skipR7R8:
    if (psBearerCount == 2 && (cell_data_speed > 0 || max_data_bearer > 0)) {
        data_reg_state_cache[m_slot_id]->radio_technology =
                (cell_data_speed > max_data_bearer) ? cell_data_speed : max_data_bearer;
    }
    // update CA band info
    updateNwCfgInfo();
    updateCaBandInfo();
    return;
}

void RmcNetworkRealTimeRequestHandler::requestOperator(const sp<RfxMclMessage>& msg)
{
    int err;
    int skip;
    char *resp[3];
    sp<RfxMclMessage> response;
    sp<RfxMclMessage> urc;
    RfxAtLine* line;
    char simNumeric[10] = {0};
    char longname[MAX_OPER_NAME_LENGTH], shortname[MAX_OPER_NAME_LENGTH];
    sp<RfxAtResponse> p_response;
    MD_EOPS *mMdEops = mdEops[m_slot_id];

    memset(resp, 0, sizeof(resp));

    if (!isInService(urc_data_reg_state_cache[m_slot_id]->registration_state)
            && !isInService(urc_voice_reg_state_cache[m_slot_id]->registration_state)
            && isInService(ril_wfc_reg_status[m_slot_id])) {
        //for VoWifi & CS/PS OOS, use SIM's operator numeric.
        int size = sizeof(simNumeric);
        String8 simMccMnc = getMclStatusManager()->
                getString8Value(RFX_STATUS_KEY_UICC_GSM_NUMERIC, String8("0"));
        if ((simMccMnc.string()  == NULL) || (simMccMnc == "0")) {
            simMccMnc = getMclStatusManager()->
                    getString8Value(RFX_STATUS_KEY_UICC_CDMA_NUMERIC, String8("0"));
        }
        if ((simMccMnc.string()  != NULL) && (simMccMnc != "0")) {
            strncpy(simNumeric, simMccMnc, size -1);
            simNumeric[size - 1] = '\0';
        }
        logD(LOG_TAG, "VoWifi Only, use SIM Operator Numeric: %s", simNumeric);
        if (strlen(simNumeric) > 0) {
            resp[2] = simNumeric;
            err = getOperatorNamesFromNumericCode(resp[2], longname, shortname, MAX_OPER_NAME_LENGTH);
            if(err >= 0) {
                resp[0] = longname;
                resp[1] = shortname;
            }
            response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxStringsData(resp, 3), msg, false);
            // response to TeleCore
            responseToTelCore(response);
            return;
        }
    }

    /* Format should be set during initialization */
    p_response = atSendCommandSingleline("AT+EOPS?", "+EOPS:");

    pthread_mutex_lock(&mdEopsMutex[m_slot_id]);
    memset(mdEops[m_slot_id], 0, sizeof(MD_EOPS));

    // check error
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        logE(LOG_TAG, "EOPS got error response");
        goto error;
    } else {
        // handle intermediate
        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err >= 0) {
            /* <mode> */
            skip = line->atTokNextint(&err);
            if ((err >= 0) &&( skip >= 0 && skip <= 4 && skip != 2)) {
                mMdEops->mode = skip;
                // a "+EOPS: 0" response is possible
                if (line->atTokHasmore()) {
                    /* <format> */
                    skip = line->atTokNextint(&err);
                    if (err >= 0 && skip == 2)
                    {
                        mMdEops->format = skip;
                        /* <oper> */
                        resp[2] = line->atTokNextstr(&err);
                        strncpy(mMdEops->oper, resp[2], strlen(resp[2]) > 9 ? 9 : (1+strlen(resp[2])));
                        /* Modem might response invalid PLMN ex: "", "000000" , "??????", ignore it */
                        // request_operator returns valid plmn when in service.
                        if (strlen(resp[2]) > 0 && (0 != strcmp(resp[2], "000000"))) {
                            getMclStatusManager()->setString8Value(
                                    RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED,
                                    String8::format("%s", resp[2]));
                        } else {
                            getMclStatusManager()->setString8Value(
                                    RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED, String8(""));
                        }
                        setLastValidPlmn(resp[2]);
                        bool hasService =
                            isInService(urc_data_reg_state_cache[m_slot_id]->registration_state)
                            || isInService(urc_voice_reg_state_cache[m_slot_id]->registration_state)
                            || isInService(ril_wfc_reg_status[m_slot_id]);
                        if ((!hasService) ||
                                (!((*resp[2] >= '0') && (*resp[2] <= '9'))) ||
                                (0 == strcmp(resp[2], "000000"))) {
                            memset(resp, 0, sizeof(resp));
                            pthread_mutex_unlock(&mdEopsMutex[m_slot_id]);
                            if (!hasService) {
                                // clear key/cache when we are not in service
                                // keep the previous value when MD report invalid.
                                getMclStatusManager()->setString8Value(RFX_STATUS_KEY_OPERATOR,
                                        String8(""));
                                pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
                                memset(op_info_cache[m_slot_id]->mccMnc, 0, 8);
                                memset(op_info_cache[m_slot_id]->longName, 0, MAX_OPER_NAME_LENGTH);
                                memset(op_info_cache[m_slot_id]->shortName, 0, MAX_OPER_NAME_LENGTH);
                                pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
                            }
                            response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                                    RfxStringsData(resp, 3), msg, false);
                            // response to TeleCore
                            responseToTelCore(response);
                            return;
                        }
                        int act = line->atTokNextint(&err);
                        if (err < 0) {
                           logE(LOG_TAG, "at tok act fail");
                        }
                        unsigned int lac = isInService(urc_data_reg_state_cache[m_slot_id]->registration_state) ?
                            eons_info[m_slot_id].lac_ps : eons_info[m_slot_id].lac_cs;
                        err = getOperatorNamesFromNumericCode(
                                resp[2], lac, longname, shortname, MAX_OPER_NAME_LENGTH, (act == 0x8000 ? 11 : 0));
                        String8 oper(resp[2]);
                        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_OPERATOR, oper);
                        if(err >= 0)
                        {
                            resp[0] = longname;
                            resp[1] = shortname;
                        }
                        pthread_mutex_unlock(&mdEopsMutex[m_slot_id]);
                        pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
                        strncpy(op_info_cache[m_slot_id]->mccMnc, resp[2], strlen(resp[2]));
                        strncpy(op_info_cache[m_slot_id]->longName, longname, strlen(longname));
                        strncpy(op_info_cache[m_slot_id]->shortName, shortname, strlen(shortname));
                        pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
                        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                                RfxStringsData(resp, 3), msg, false);
                        // response to TeleCore
                        responseToTelCore(response);
                        return;
                    }
                }
            }
        }
    }
    pthread_mutex_unlock(&mdEopsMutex[m_slot_id]);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxStringsData(resp, 3), msg, false);
    // response to TeleCore
    responseToTelCore(response);

    return;
error:
    pthread_mutex_unlock(&mdEopsMutex[m_slot_id]);
    logE(LOG_TAG, "requestOperator must never return error when radio is on");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxStringsData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmcNetworkRealTimeRequestHandler::requestQueryNetworkSelectionMode(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int resp = 0;

    resp = mdEops[m_slot_id]->mode;
    if (resp < 0 || resp > 4 || resp == 3) {
        goto error;
    }

    logD(LOG_TAG, "requestQueryNetworkSelectionMode return %d", mdEops[m_slot_id]->mode);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&resp, 1), msg, false);
    // response to TeleCore
    responseToTelCore(response);

    return;
error:
    logE(LOG_TAG, "requestQueryNetworkSelectionMode must never return error when radio is on");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmcNetworkRealTimeRequestHandler::resetVoiceRegStateCache(RIL_VOICE_REG_STATE_CACHE *voiceCache, RIL_CACHE_GROUP source) {
    pthread_mutex_lock(&s_voiceRegStateMutex[m_slot_id]);
    if (source == CACHE_GROUP_ALL) {
        (*voiceCache).registration_state = 0;
        (*voiceCache).lac = 0xffffffff;
        (*voiceCache).cid = 0x0fffffff;
        (*voiceCache).radio_technology = 0;
        (*voiceCache).base_station_id = 0;
        (*voiceCache).base_station_latitude = 0;
        (*voiceCache).base_station_longitude = 0;
        (*voiceCache).css = 0;
        (*voiceCache).system_id = 0;
        (*voiceCache).network_id = 0;
        (*voiceCache).roaming_indicator = -1;
        (*voiceCache).is_in_prl = -1;
        (*voiceCache).default_roaming_indicator = -1;
        (*voiceCache).denied_reason = 0;
        (*voiceCache).network_exist = 0;
    } else if (source == CACHE_GROUP_COMMON_REQ) {
        (*voiceCache).registration_state = 0;
        (*voiceCache).lac = 0xffffffff;
        (*voiceCache).cid = 0x0fffffff;
        (*voiceCache).radio_technology = 0;
        (*voiceCache).roaming_indicator = 0;
        (*voiceCache).denied_reason = 0;
        (*voiceCache).network_exist = 0;
    } else if (source == CACHE_GROUP_C2K) {
        (*voiceCache).base_station_id = 0;
        (*voiceCache).base_station_latitude = 0;
        (*voiceCache).base_station_longitude = 0;
        (*voiceCache).css = 0;
        (*voiceCache).system_id = 0;
        (*voiceCache).network_id = 0;
    } else {
        // source type invalid!!!
        logD(LOG_TAG, "updateVoiceRegStateCache(): source type invalid!!!");
    }
    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);
}

void RmcNetworkRealTimeRequestHandler::resetDataRegStateCache(RIL_DATA_REG_STATE_CACHE *dataCache,
        RIL_CACHE_GROUP source) {
    RFX_UNUSED(source);
    (*dataCache).registration_state = 0;
    (*dataCache).lac = 0xffffffff;
    (*dataCache).cid = 0x0fffffff;
    (*dataCache).radio_technology = 0;
    (*dataCache).denied_reason = 0;
    (*dataCache).max_simultaneous_data_call = 1;
    // NR none-standalone
    (*dataCache).dcnr_restricted = 0;
    (*dataCache).endc_sib = 0;
    (*dataCache).endc_available = 0;
}

void RmcNetworkRealTimeRequestHandler::updateServiceStateValue() {
    pthread_mutex_lock(&s_voiceRegStateMutex[m_slot_id]);
    getMclStatusManager()->setServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
            RfxNwServiceState(
                    voice_reg_state_cache[m_slot_id]->registration_state,
                    data_reg_state_cache[m_slot_id]->registration_state,
                    voice_reg_state_cache[m_slot_id]->radio_technology,
                    data_reg_state_cache[m_slot_id]->radio_technology,
                    ril_wfc_reg_status[m_slot_id]));
    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);
}

void RmcNetworkRealTimeRequestHandler::resetCaCache(RIL_CA_CACHE* cache) {
    pthread_mutex_lock(&s_caMutex[m_slot_id]);
    cache->ca_info = 1;
    cache->pcell_bw = 0;
    cache->scell_bw1 = 0;
    cache->scell_bw2 = 0;
    cache->scell_bw3 = 0;
    cache->scell_bw4 = 0;
    pthread_mutex_unlock(&s_caMutex[m_slot_id]);
}

void RmcNetworkRealTimeRequestHandler::resetNwCfgCache(RIL_NwCfgInfo* cache) {
    pthread_mutex_lock(&s_nwCfgMutex[m_slot_id]);
    cache->mimo = 0;
    cache->qam_256 = 0;
    cache->qam_ul64 = 0;
    pthread_mutex_unlock(&s_nwCfgMutex[m_slot_id]);
}

// get Ca Band info, Unit is 0.1MHz
void RmcNetworkRealTimeRequestHandler::updateCaBandInfo() {
    int err;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;

    resetCaCache(ca_cache[m_slot_id]);

    int *ca_info = &(ca_cache[m_slot_id]->ca_info);
    int *pcell_bw = &(ca_cache[m_slot_id]->pcell_bw);
    int *scell_bw1 = &(ca_cache[m_slot_id]->scell_bw1);
    int *scell_bw2 = &(ca_cache[m_slot_id]->scell_bw2);
    int *scell_bw3 = &(ca_cache[m_slot_id]->scell_bw3);
    int *scell_bw4 = &(ca_cache[m_slot_id]->scell_bw4);

    pthread_mutex_lock(&s_caMutex[m_slot_id]);

    // ca_info- CA_CONFIGURED(0)/CA_NOT_CONFIGURATED(1)/CA_ACTIVATED(2)/CA_DEACTIVATED(3)
    // +ECAINFO: <ca_info>,<pcell_bw>,<scell_bw1>,<scell_bw2>,<scell_bw3>,<scell_bw4>
    p_response = atSendCommandSingleline("AT+ECAINFO?", "+ECAINFO:");

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

    *ca_info = line->atTokNextint(&err);
    if (err < 0) goto error;

    *pcell_bw = line->atTokNextint(&err);
    if (err < 0) goto error;

    *scell_bw1 = line->atTokNextint(&err);
    if (err < 0) goto error;

    *scell_bw2 = line->atTokNextint(&err);
    if (err < 0) goto error;

    *scell_bw3 = line->atTokNextint(&err);
    if (err < 0) goto error;

    *scell_bw4 = line->atTokNextint(&err);
    if (err < 0) goto error;

    // get LTE band when pcell_bw is 0 and it's LTE in service now.
    if (*pcell_bw == 0) {
        if (isInService(urc_data_reg_state_cache[m_slot_id]->registration_state) &&
            convertPSNetworkType(urc_data_reg_state_cache[m_slot_id]->radio_technology) == 14) {  // 14:LTE, 19:LTE_CA
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

            *pcell_bw = line->atTokNextint(&err);
            if (err < 0) goto error;

            // we turn off the URC right after we got lte bw info
            atSendCommand("AT+ELTEBWINFO=0");
        }
    }

    pthread_mutex_unlock(&s_caMutex[m_slot_id]);
    return;
error:
    pthread_mutex_unlock(&s_caMutex[m_slot_id]);
    resetCaCache(ca_cache[m_slot_id]);
    return;
}

void RmcNetworkRealTimeRequestHandler::updateNwCfgInfo() {
    int err;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;
    RIL_NwCfgInfo nw_cfg_info;
    RIL_NwCfgInfo *nw_cfg_p = nw_cfg_cache[m_slot_id];
    sp<RfxMclMessage> urc;

    p_response = atSendCommandSingleline("AT+ENWCFGINFO?", "+ENWCFGINFO:");
    err = p_response->getError();
    if (err != 0 ||
        p_response == NULL ||
        p_response->getSuccess() == 0 ||
        p_response->getIntermediates() == NULL) {
        goto error;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;

    nw_cfg_info.mimo = line->atTokNexthexint(&err);
    if (err < 0) goto error;

    nw_cfg_info.qam_256 = line->atTokNexthexint(&err);
    if (err < 0) goto error;

    nw_cfg_info.qam_ul64 = line->atTokNexthexint(&err);
    if (err < 0) goto error;

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
    return;

error:
    logD(LOG_TAG, "%s error!", __FUNCTION__);
    return;
}

void RmcNetworkRealTimeRequestHandler::updateDcStatus(int cell, int max) {
    if ((cell & 0x80) == 0x80) m_dc_support[m_slot_id] = true;
    else if (max >= 12 && max <= 17) m_dc_support[m_slot_id] = true;
    else m_dc_support[m_slot_id] = false;
}

void RmcNetworkRealTimeRequestHandler::requestIWlanRegistrationState(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    int state = s_en3epcreg_support ? s_iwlan_reg_status[m_slot_id] : -1;
    logD(LOG_TAG, "[%s] support:%d, state:%d", __FUNCTION__, s_en3epcreg_support, state);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxIntsData(&state, 1), msg, false);
    responseToTelCore(response);
}

bool RmcNetworkRealTimeRequestHandler::skipDCNRRestrictedState(const char *mccmnc) {
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
