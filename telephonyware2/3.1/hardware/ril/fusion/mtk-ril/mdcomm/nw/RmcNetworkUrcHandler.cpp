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
#include "RmcNetworkUrcHandler.h"
#include "RfxCellInfoData.h"
#include "RfxCidData.h"
#include "RfxNetworkScanResultData.h"
#include "RfxRawData.h"
#include "RfxSignalStrengthData.h"
/*ADD-BEGIN-JUNGO-20101008-CTZV support */
#include <time.h>
#include "ViaBaseHandler.h"
#include "RfxViaUtils.h"
#include <libmtkrilutils.h>

// register data
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIGNAL_STRENGTH);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIGNAL_STRENGTH_WITH_WCDMA_ECIO);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_RESPONSE_CS_NETWORK_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_CDMA_OTA_PROVISION_STATUS);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_CDMA_PRL_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_NETWORK_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxCellInfoData, RFX_MSG_URC_CELL_INFO_LIST);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_GMSS_RAT_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_NITZ_TIME_RECEIVED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NETWORK_EVENT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_MODULATION_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_PSEUDO_CELL_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxNetworkScanResultData, RFX_MSG_URC_NETWORK_SCAN_RESULT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_PHYSICAL_CHANNEL_CONFIGS_MTK);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_MCCMNC_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxBarringInfoData, RFX_MSG_URC_BARRING_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxRegistrationFailedData, RFX_MSG_URC_REGISTRATION_FAILED);

// MUSE WFC requirement
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_CELLULAR_QUALITY_CHANGED_IND);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_CONFIRM_RAT_BEGIN);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_FEMTOCELL_UPDATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_CS_NETWORK_STATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_PS_NETWORK_STATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SMART_RAT_SWITCH_INFO);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_TOE_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NETWORK_BAND_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_SIB16_TIME_INFO_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_IWLAN_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NR_CA_BAND_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_5GUW_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NR_SYS_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NW_CFG_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_RRC_STATE);
RFX_REGISTER_DATA_TO_URC_ID(RfxCidData, RFX_MSG_URC_CELLULAR_IDENTIFIER_DISCLOSED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SECURITY_ALGORITHMS_UPDATED);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcNetworkUrcHandler, RIL_CMD_PROXY_URC);

/// M: Local Time VZ_REQ_LTEDATA_6793 @{
int RmcNetworkUrcHandler::bSIB16Received = 0;
int RmcNetworkUrcHandler::bNitzTzAvailble = 0;
int RmcNetworkUrcHandler::bNitzDstAvailble = 0;
char RmcNetworkUrcHandler::ril_nw_nitz_tz[MAX_NITZ_TZ_DST_LENGTH] = {0};
char RmcNetworkUrcHandler::ril_nw_nitz_dst[MAX_NITZ_TZ_DST_LENGTH] = {0};
/// @}

RmcNetworkUrcHandler::RmcNetworkUrcHandler(int slot_id, int channel_id) :
        RmcNetworkHandler(slot_id, channel_id),
        allowed_urc(NULL),
        ril_data_urc_status(4),
        ril_data_urc_rat(0),
        receivedEcampUI(false) {
    int m_slot_id = slot_id;
    ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
    RFX_LOG_V(LOG_TAG, "%s[%d] start", __FUNCTION__, slot_id);
    if (mViaHandler != NULL) {
        mViaHandler->registerForViaUrc(this);
        allowed_urc = mViaHandler->getViaAllowedUrcForNw();
    }
    const char* urc[] = {
        "+E5GUW:",
        "+E5GUC:",
        "+EREG:",
        "+EGREG:",
        "+PSBEARER:",
        "+ECSQ:",
        "^OTACMSG:",
        "+ERPRAT:",
        "+EGMSS:",
        "+EIPRL:",
        "+EDEFROAM:",
        "+ENWINFO:",
        "+ECSG:",
        "+ECELL:",
        "+CTZEU:",
        "+CTZE:",
        "+CIEV: 10,",
        "+CIEV: 11,",
        "+EREGINFO:",
        "+EWFC:",
        "+EMODCFG:",
        "+EONS:",
        "+EAPC:",
        "+EMCCMNC:",
        "+EIMSESS:",
        "+CNEMS1:",
        "+CNEM5G:",
        "+EHOMEAS:",
        "+CSCON:",
        "+EOPSU:",
        "+ECAINFO:",
        "+EBARRING:",
        "+ECAMPUI:",
        "+ECAPABILITY:",
        "+ECHINFO:",
        "+ENRCABAND:",
        "+EN3EPCREG:",
        "+ENWSCN:",
        "+ENRSCS:",
        "+EPHYCONFIG"
    };

    registerToHandleURC(urc, sizeof(urc)/sizeof(char *));

    // reset WFC state because it's persist property
    if (s_iwlan_in_legacy_mode) {
        setMSimProperty(m_slot_id, (char*)PROPERTY_WFC_STATE, (char*)"0");
    }

    // reset ps bearer cache
    ril_ps_bearer_cache.cell_data_speed = 0;
    ril_ps_bearer_cache.max_data_bearer = 0;
}

RmcNetworkUrcHandler::~RmcNetworkUrcHandler() {
}

void RmcNetworkUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
    if (strStartsWith(msg->getRawUrc()->getLine(), "+EREG:")) {
        handleCsNetworkStateChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EGREG:")) {
        handlePsNetworkStateChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+PSBEARER:")) {
        handlePsDataServiceCapability(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ECSQ:")) {
        handleSignalStrength(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "^OTACMSG:")) {
        handleOtaProvisionStatus(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ERPRAT:")) {
        handleConfirmRatBegin(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EGMSS:")) {
        handleGmssRatChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EIPRL:")) {
        handleSystemInPrlIndication(msg);
    }  else if (strStartsWith(msg->getRawUrc()->getLine(), "+EDEFROAM:")) {
        handleDefaultRoamingIndicator(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ENWINFO:")) {
        handleNetworkInfo(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ECSG:")) {
        handleGsmFemtoCellInfo(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ECELL:")) {
        handleCellInfoList(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+CTZEU:")) {
        handleNitzTzReceived(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+CTZE:")) {
        handleLegacyNitzTzReceived(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+CIEV: 10")) {
        handleNitzOperNameReceived(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+CIEV: 11")) {
        handleSib16TimeInfoReceived(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EREGINFO:")) {
        handleNetworkEventReceived(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EWFC:")) {
        handleWfcStateChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EMODCFG:")) {
        handleModulationInfoReceived(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EONS:")) {
        handleEnhancedOperatorNameDisplay(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EAPC:")) {
        handlePseudoCellInfo(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EMCCMNC:")) {
        handleMccMncChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EIMSESS:")) {
        onImsEmergencySupportR9(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+CNEMS1:")) {
        handleVoLteEmergencySupportedChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+CNEM5G:")) {
        handleVoNrEmergencySupportedChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EHOMEAS:")) {
        handleCellularQualityReport(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+CSCON:")) {
        handleConnectionStateReport(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EOPSU:")) {
        handleEnhancedMccMncChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ECAINFO:")) {
        handleCaInfoChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EBARRING:")) {
        handleBarringInfoReport(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ECAMPUI:")) {
        handleEcampuiChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ECAPABILITY:")) {
        handleEndcCapabilityChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ECHINFO:")) {
        handleChInfoChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ENRCABAND:")) {
        handleNrCaBandChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EN3EPCREG:")) {
        handleIWlanRegStateChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+E5GUW:")) {
        handle5GUWChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+E5GUC:")) {
        handle5GUCChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ENWSCN:")) {
        handleNetworkScanInd(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ENRSCS:")) {
        handleNrSysInfoChanged(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EPHYCONFIG")) {
        handlePhysicalChannelConfig(msg);
    } else if (mViaHandler != NULL) {
        mViaHandler-> handleViaUrc(msg, this, m_slot_id);
    }
}

void RmcNetworkUrcHandler::handleCsNetworkStateChanged(const sp<RfxMclMessage>& msg) {
    /* +EREG: <stat>[,<lac>,<ci>,[<eAct>],[nwk_existence>],[<roam_indicator>][,<cause_type>,<reject_cause>]] */
    int err;
    int skip;
    int response[6] = {-1};
    unsigned long long ci_response = -1;
    char *responseStr[6] = { 0 };
    int i = 0;
    int state[4] = { 0 };
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();
    char* tmp = NULL;

    // notify AP that AT+ECAMPUI info should no longer be used.
    if (receivedEcampUI) {
        sendDisableEcampuiChanged();
    }

    pthread_mutex_lock(&mdEregMutex[m_slot_id]);
    MD_EREG* mMdEreg = mdEreg[m_slot_id];
    MD_ECELL *mMdEcell = mdEcell[m_slot_id];
    state[0] = convertRegState(mMdEreg->stat, true);
    state[1] = convertCSNetworkType(mMdEreg->eAct);
    memset(mdEreg[m_slot_id], 0, sizeof(MD_EREG));

    line->atTokStart(&err);
    if (err < 0) goto error;

    response[0] = line->atTokNextint(&err);
    if (err < 0) goto error;
    mMdEreg->stat = response[0];

    // For LTE and C2K
    response[0] = convertRegState(response[0], true);

    // set "-1" to indicate "the field is not present"
    for (i = 1; i < 6; i++) {
        asprintf(&responseStr[i], "-1");
    }

    // fill in <state>
    asprintf(&responseStr[0], "%d", response[0]);

    // get <lac>
    response[1] = line->atTokNexthexint(&err);
    if (err >= 0) {
        mMdEreg->lac = (unsigned int) response[1];

        free(responseStr[1]);
        responseStr[1] = NULL;
        asprintf(&responseStr[1], "%x", response[1]);
        eons_info[m_slot_id].lac_cs = (unsigned int)response[1];

        // get <ci>
        tmp = line->atTokNextstr(&err);
        if (err >= 0) {
            ci_response = strtoull(tmp, NULL, 16);
            mMdEreg->ci = ci_response;

            free(responseStr[2]);
            responseStr[2] = NULL;
            asprintf(&responseStr[2], "%llx", ci_response);

            // get <eAct>
            response[3] = line->atTokNextint(&err);
            if (err >= 0) {
                mMdEreg->eAct = response[3];

                free(responseStr[3]);
                responseStr[3] = NULL;
                asprintf(&responseStr[3], "%d", convertCSNetworkType(response[3]));

                // get <nwk_existence>
                response[5]= line->atTokNextint(&err);
                mMdEreg->nwk_existence = response[5];

                // ECC support
                if (response[5] != 1) {
                    // if cid is 0x0fffffff or -1 means it is invalid
                    // if lac is 0xffff or -1 means it is invalid
                    if (((response[0] == 0) || (response[0] == 2) || (response[0] == 3))
                            && ((ci_response != 0x0fffffff)  // cid
                            && (response[1] != 0xffff)       // lac
                            && (ci_response != -1)           // cid
                            && (response[1] != -1))          // lac
                            && convertCSNetworkType(response[3]) != 14) {   // not LTE & LET_CA
                        response[5] = 1;
                    }
                }
                if (response[5] != 1) {
                    response[5] = 0;
                }
                free(responseStr[5]);
                responseStr[5] = NULL;
                asprintf(&responseStr[5], "%d", response[5]);

                if (err >= 0) {
                    // get <roam_indicator>
                    skip = line->atTokNextint(&err);
                    if (err >= 0) {
                        mMdEreg->roam_indicator = skip;
                        // get <cause_type>
                        skip = line->atTokNextint(&err);
                        if (err >= 0) {
                            mMdEreg->cause_type = skip;
                            // get <reject_cause>
                            response[4] = line->atTokNextint(&err);
                            if (err >= 0) {
                                mMdEreg->reject_cause = response[4];
                                free(responseStr[4]);
                                responseStr[4] = NULL;
                                asprintf(&responseStr[4], "%d", response[4]);
                            }
                        }
                    }
                }
            }
        }
    }

    state[2] = convertRegState(mMdEreg->stat, true);
    state[3] = convertCSNetworkType(mMdEreg->eAct);

    //keep the cs urc
    pthread_mutex_lock(&s_urc_voiceRegStateMutex[m_slot_id]);
    if ((urc_voice_reg_state_cache[m_slot_id]->lac == (unsigned int)response[1]) &&
        (urc_voice_reg_state_cache[m_slot_id]->cid == ci_response) &&
        (urc_voice_reg_state_cache[m_slot_id]->denied_reason > 0) &&
        (response[4] == 0)) {
        // do nothing to keep the latest reject cause when lac/cid are the same
        // and we have reject cause before but md doesn't provide with new URC.
    } else {
        urc_voice_reg_state_cache[m_slot_id]->denied_reason = response[4];
    }
    urc_voice_reg_state_cache[m_slot_id]->registration_state = response[0];  // stat
    urc_voice_reg_state_cache[m_slot_id]->lac = response[1];  // lac
    urc_voice_reg_state_cache[m_slot_id]->cid = ci_response;  // cid
    urc_voice_reg_state_cache[m_slot_id]->radio_technology = response[3];  // act
    pthread_mutex_unlock(&s_urc_voiceRegStateMutex[m_slot_id]);
    dumpEregCache(__func__, mMdEreg, mMdEcell);
    pthread_mutex_unlock(&mdEregMutex[m_slot_id]);

    sendEvent(RFX_MSG_EVENT_CS_NETWORK_STATE, RfxIntsData(state, 4), RIL_CMD_PROXY_3, m_slot_id);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_CS_NETWORK_STATE_CHANGED,
            m_slot_id, RfxStringsData(responseStr, 6));
    responseToTelCore(urc);

    isNeedNotifyStateChanged();
    for (i = 0; i < 6; i++) {
        free(responseStr[i]);
    }

    if (mMdEreg->stat == 3) sendRegistrationFailedEvent(RIL_DOMAIN_CS);

    return;

error:
    pthread_mutex_unlock(&mdEregMutex[m_slot_id]);
    logE(LOG_TAG, "There is something wrong with the URC");
}

void RmcNetworkUrcHandler::handlePsDataServiceCapability(const sp<RfxMclMessage>& msg) {
    int err;
    int cell_data_speed = 0;
    int max_data_bearer = 0;
    RfxAtLine *line = msg->getRawUrc();
    bool ret = false;

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    cell_data_speed = line->atTokNextint(&err);
    if (err < 0) goto error;

    // <max_data_bearer_capability> is only support on 3G
    max_data_bearer = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (urc_data_reg_state_cache[m_slot_id]->registration_state == 1 ||
            urc_data_reg_state_cache[m_slot_id]->registration_state == 5) {
        if (cell_data_speed < 0x1000) { // 3G case
            if (max_data_bearer != ril_ps_bearer_cache.max_data_bearer &&
                    ((max_data_bearer > 3 && max_data_bearer < 18) ||
                    (ril_ps_bearer_cache.max_data_bearer > 3 &&
                    ril_ps_bearer_cache.max_data_bearer < 18))) {
                ret = true;
            }
        } else { // 4G case
            if (cell_data_speed != ril_ps_bearer_cache.cell_data_speed &&
                    (cell_data_speed > 0x1000 ||
                    ril_ps_bearer_cache.cell_data_speed > 0x1000)) {
                ret = true;
            }
        }
    }
    ril_ps_bearer_cache.max_data_bearer = max_data_bearer;
    ril_ps_bearer_cache.cell_data_speed = cell_data_speed;

    if (ret == true) {
        isNeedNotifyStateChanged();
    }
    return;
error:
    logE(LOG_TAG, "There is something wrong with the URC");
}

void RmcNetworkUrcHandler::handleSignalStrength(const sp<RfxMclMessage>& msg) {
    RIL_SIGNAL_STRENGTH resp = {0};
    int err;
    int sigUpdated, sigRegisted, activeGroup;
    RfxAtLine *line = msg->getRawUrc();
    sp<RfxMclMessage> urc;

    pthread_mutex_lock(&s_signalStrengthMutex[m_slot_id]);

    // go to start position
    line->atTokStart(&err);
    if (err < 0) {
        goto done;
    }

    sigUpdated = updateSignalStrength(line);
    if (sigUpdated == SIGNAL_GROUP_NONE) {
        // Either receive invalid or same signal strength as before
        goto done;
    }

    sigRegisted = getCurrentSignalGroup();

    logD(LOG_TAG, "requestSignalStrength:sigRegisted[%#04X], sigUpdated[%#04X]",
            sigRegisted, sigUpdated);
    printSignalStrengthCache((char*) __FUNCTION__);

    activeGroup = sigUpdated & sigRegisted;

    if (sigRegisted == SIGNAL_GROUP_NONE) {
        // TODO: OOS case only report URC changed signal strength?
        fillSignalStrengthResponse(&resp, sigUpdated);
    } else if (activeGroup != SIGNAL_GROUP_NONE) {
        fillSignalStrengthResponse(&resp, sigRegisted);
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


unsigned int RmcNetworkUrcHandler::combineWfcEgregState() {
    unsigned int uiRet = (unsigned int)ril_data_urc_status;
    if (ril_wfc_reg_status[m_slot_id] == 1) {
        uiRet = 1;
    }

    RFX_LOG_V(LOG_TAG, "combineWfcEgregState() slot_id=%d, data_urc=%d, wfc_reg=%d, uiRet=%d",
        m_slot_id, ril_data_urc_status, ril_wfc_reg_status[m_slot_id], uiRet);
    return uiRet;
}

void RmcNetworkUrcHandler::handlePsNetworkStateChanged(const sp<RfxMclMessage>& msg) {
    int err, skip;
    int stat[4] = { 0 };
    unsigned int response[4] = {0};
    unsigned long long ci_response = -1;
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();
    MD_EGREG *mMdEgreg = mdEgreg[m_slot_id];
    MD_ECELL *mMdEcell = mdEcell[m_slot_id];
    char* tmp = NULL;
    bool new_attach = false;
    int find = -1;
    int upperlayerIndR15Available = 0;
    int info[2] = { 0 };

    // +EGREG: <n>,<stat>[,[<lac>],[<ci>],[<eAct>],[<rac>],[<nwk_existence >],[<roam_indicator>],
    //          [<dcnr_restricted>],[<endc_sib>],[<endc_available>],[,<cause_type>,<reject_cause>]]
    int dcnr_restricted = 0, endc_sib = 0, endc_available = 0, reject_cause;

    // save it before mMdEgreg is reseted.
    int pre_stat = mMdEgreg->stat;

    // notify AP that AT+ECAMPUI info should no longer be used.
    if (receivedEcampUI) {
        sendDisableEcampuiChanged();
    }

    pthread_mutex_lock(&mdEgregMutex[m_slot_id]);
    stat[0] = convertRegState(mMdEgreg->stat, false);
    stat[1] = convertPSNetworkType(mMdEgreg->eAct);
    memset(mdEgreg[m_slot_id], 0, sizeof(MD_EGREG));
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    response[0] = line->atTokNextint(&err);
    if (err < 0) goto error;
    if (!isInService(pre_stat) && isInService(response[0])) new_attach = true;
    mMdEgreg->stat = response[0];

    // for LTE
    ril_data_urc_status = convertRegState(response[0], false);

    // get <lac>
    response[1] = line->atTokNexthexint(&err);
    if (err < 0) goto error;
    mMdEgreg->lac = (unsigned int) response[1];
    eons_info[m_slot_id].lac_ps = (unsigned int)response[1];

    // get <ci>
    tmp = line->atTokNextstr(&err);
    if (err < 0) goto error;
    ci_response = strtoull(tmp, NULL, 16);
    mMdEgreg->ci = ci_response;

    //get <Act>
    response[3] = line->atTokNextint(&err);
    if (err < 0) goto error;
    ril_data_urc_rat = response[3];
    mMdEgreg->eAct = response[3];

    // get rac
    skip = line->atTokNexthexint(&err);
    if (err < 0) goto error;
    mMdEgreg->rac = skip;

    // get nwk_existence
    skip = line->atTokNextint(&err);
    if (err < 0) goto error;
    mMdEgreg->nwk_existence = skip;

    // get roam_indicator
    skip = line->atTokNextint(&err);
    if (err < 0) goto error;
    mMdEgreg->roam_indicator = skip;

    if (isNrSupported()) {
        // get dcnr_restricted
        dcnr_restricted = line->atTokNextint(&err);
        if (err < 0) goto error;
        mMdEgreg->dcnr_restricted = dcnr_restricted;

        // get endc_sib
        endc_sib = line->atTokNextint(&err);
        if (err < 0) goto error;
        mMdEgreg->endc_sib = endc_sib;

        // get endc_available
        endc_available = line->atTokNextint(&err);
        if (err < 0) goto error;
        mMdEgreg->endc_available = endc_available;
    }

    // get cause_type
    skip = line->atTokNextint(&err);
    if (err < 0) goto error;
    mMdEgreg->cause_type = skip;

    // get reject_cause
    reject_cause = line->atTokNextint(&err);
    if (err < 0) goto error;
    mMdEgreg->reject_cause = reject_cause;

    stat[2] = combineWfcEgregState();
    if (ril_wfc_reg_status[m_slot_id] == 1) {
        stat[3] = RADIO_TECH_IWLAN;
    } else {
        stat[3] = convertPSNetworkType(response[3]);
    }

    sendEvent(RFX_MSG_EVENT_PS_NETWORK_STATE, RfxIntsData(stat, 4),
        RIL_CMD_PROXY_3, m_slot_id);

    isNeedNotifyStateChanged();

    //keep the ps urc
    pthread_mutex_lock(&s_urc_dataRegStateMutex[m_slot_id]);
    if ((urc_data_reg_state_cache[m_slot_id]->lac == (unsigned int)response[1]) &&
        (urc_data_reg_state_cache[m_slot_id]->cid == ci_response) &&
        (urc_data_reg_state_cache[m_slot_id]->denied_reason > 0) &&
        (reject_cause == 0)) {
        // do nothing to keep the latest reject cause when lac/cid are the same
        // and we have reject cause before but md doesn't provide with new URC.
    } else {
        urc_data_reg_state_cache[m_slot_id]->denied_reason = reject_cause;
    }
    urc_data_reg_state_cache[m_slot_id]->registration_state = response[0];  // stat
    urc_data_reg_state_cache[m_slot_id]->lac = response[1];  // lac
    urc_data_reg_state_cache[m_slot_id]->cid = ci_response;  // cid
    urc_data_reg_state_cache[m_slot_id]->radio_technology= response[3];  // act
    urc_data_reg_state_cache[m_slot_id]->dcnr_restricted = dcnr_restricted;
    urc_data_reg_state_cache[m_slot_id]->endc_sib = endc_sib;
    urc_data_reg_state_cache[m_slot_id]->endc_available = endc_available;
    pthread_mutex_unlock(&s_urc_dataRegStateMutex[m_slot_id]);
    dumpEgregCache(__func__, mMdEgreg, mMdEcell);
    pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);

    // Add to cache only if SCG configured and SIB not 2
    if ((nr_display_rule[m_slot_id] & NR_DISPALY_NR_CACHE) == NR_DISPALY_NR_CACHE
        && mMdEgreg->eAct == 0x4000 && mMdEgreg->endc_sib != 2) {
        pthread_mutex_lock(&nrCacheMutex);
        for (int i = 0; i < MAX_LTE_ENDC_MAX_CAPABILITY; i++) {
            if (lteMaxNr[i].cid == mMdEgreg->ci &&  // cid
                lteMaxNr[i].lacTac == mMdEgreg->lac) {  // lac
                find = i;
                break;
            }
        }
        if (find == -1) {
            // it's a new cell
            find = MAX_LTE_ENDC_MAX_CAPABILITY - 1;
        }
        for (int i = find; i > 0; i--) {
            memcpy(&lteMaxNr[i], &lteMaxNr[i-1], sizeof(LTE_ENDC_MAX_CAPABILITY));
        }
        // update index 0
        lteMaxNr[0].cid = mMdEgreg->ci;
        lteMaxNr[0].lacTac = mMdEgreg->lac;
        lteMaxNr[0].dcnr_restricted = 0;
        lteMaxNr[0].endc_sib = 1;
        lteMaxNr[0].endc_available = 1;
        pthread_mutex_unlock(&nrCacheMutex);
    }

    // keep scg state
    handleScgConnectionChange(mMdEgreg->stat, mMdEgreg->lac, mMdEgreg->ci, mMdEgreg->eAct);

    if (mMdEgreg->stat == 3) sendRegistrationFailedEvent(RIL_DOMAIN_PS);

    // trigger timer1
    if (isJpMode() && new_attach && endc_sib == 2 &&
        (lte_tdd_connection_state[m_slot_id] == 1 || lte_fdd_connection_state[m_slot_id] == 1) &&
        (mMdEgreg->eAct == 0x1000 || mMdEgreg->eAct == 0x2000)) {
         logV(LOG_TAG, "JpMode & new attach in LTE connected mode without NR on NR-abled cell");
         startJpModeTimer1();
    }

    if (urc_nrsysinfo_enable && isNrSupported()) {
        pthread_mutex_lock(&nrSysInfoMutex[m_slot_id]);
        logV(LOG_TAG, "[%s] current NR sys info:%d, %d", __FUNCTION__,
            nrSysInfo[m_slot_id]->upperlayerIndR15Available, nrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz);
        upperlayerIndR15Available = (endc_sib == 2) ? 1 : 0;
        if (nrSysInfo[m_slot_id]->upperlayerIndR15Available != upperlayerIndR15Available) {
            nrSysInfo[m_slot_id]->upperlayerIndR15Available = upperlayerIndR15Available;
            info[0] = nrSysInfo[m_slot_id]->upperlayerIndR15Available;
            info[1] = nrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz;
            urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_SYS_INFO, m_slot_id,
                                       RfxIntsData(info, 2));
            responseToTelCore(urc);
        }
        pthread_mutex_unlock(&nrSysInfoMutex[m_slot_id]);
    }
    return;

error:
    pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);
    logE(LOG_TAG, "There is something wrong with the URC");
}

void RmcNetworkUrcHandler::handleOtaProvisionStatus(const sp<RfxMclMessage>& msg) {
    // URC ^OTACMSG:<status>
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

    int err;
    int otaState;
    int convertState;
    RfxAtLine *line = msg->getRawUrc();
    sp<RfxMclMessage> urc;

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    otaState = line->atTokNextint(&err);
    if (err < 0) goto error;

    // for OTA STATUS
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_OTA_STATUS, otaState);

    // for OTA URC
    convertState = convertOtaProvisionStatus(otaState);
    if (convertState >= 0) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CDMA_OTA_PROVISION_STATUS, m_slot_id,
                RfxIntsData(&convertState, 1));
        responseToTelCore(urc);
    }

    return;

error:
    logE(LOG_TAG, "handle OTA error: %s", msg->getRawUrc()->getLine());
}

int RmcNetworkUrcHandler::convertOtaProvisionStatus(int rawState) {
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

void RmcNetworkUrcHandler::handleConfirmRatBegin(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    sendEvent(RFX_MSG_EVENT_CONFIRM_RAT_BEGIN, RfxVoidData(),
            m_channel_id, m_slot_id);
}

void RmcNetworkUrcHandler::handleGmssRatChanged(const sp<RfxMclMessage>& msg) {
    int i = 0;
    int err = 0;
    int data[5] = { 0 };
    RfxAtLine *line = msg->getRawUrc();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) {
        logE(LOG_TAG, "handle GMSS RAT Changed error: %s", msg->getRawUrc()->getLine());
        return;
    }

    for (i = 0; i < 5; i++) {
        if (i == 1) {  // MCC
            data[i] = atoi(line->atTokNextstr(&err));
        } else {
            data[i] = line->atTokNextint(&err);
        }
        if (err < 0) {
            logE(LOG_TAG, "handle GMSS RAT Changed, i: %d", i);
            return;
        }
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_GMSS_RAT_CHANGED,
            m_slot_id, RfxIntsData(data, 5));
    responseToTelCore(urc);
}

void RmcNetworkUrcHandler::updatePrlInfo(int system, int mode) {
    RFX_LOG_V(LOG_TAG, "updatePrlInfo() system = %d, mode = %d", system, mode);
    if ((system == 0) && (mode < 255)) {
        pthread_mutex_lock(&s_voiceRegStateMutex[m_slot_id]);
        voice_reg_state_cache[m_slot_id]->is_in_prl = mode;
        pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);
    } else {
        // logD(LOG_TAG, "updatePrlInfo() unsupport system = %d", system);
    }
}

void RmcNetworkUrcHandler::handleSystemInPrlIndication(const sp<RfxMclMessage>& msg) {
    // +EIPRL:<system1>,<mode> [,<system2>,<mode>]
    // <system> : integer type; system type
    // 0    1xRTT
    // 1    EvDO
    // <mode>:integer type
    // 0    The system is not in PRL
    // 1    The system is in PRL
    // 255  Invalid value(used in network lost)

    int err;
    RfxAtLine* line;
    int system = -1;
    int mode = 255;

    line = msg->getRawUrc();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) return;

    system = line->atTokNextint(&err);
    if (err < 0) return;

    mode = line->atTokNextint(&err);
    if (err < 0) return;

    updatePrlInfo(system, mode);

    if (line->atTokHasmore()) {
        system = line->atTokNextint(&err);
        if (err < 0) return;

        mode = line->atTokNextint(&err);
        if (err < 0) return;

        updatePrlInfo(system, mode);
    }

    isNeedNotifyStateChanged();
    return;
}

void RmcNetworkUrcHandler::handleDefaultRoamingIndicator(const sp<RfxMclMessage>& msg) {
    int err;
    RfxAtLine* line = msg->getRawUrc();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) return;

    int default_roaming_indicator = line->atTokNextint(&err);
    if(err < 0) return;

    if(line->atTokHasmore()) {
        default_roaming_indicator = line->atTokNextint(&err);
        if(err < 0) return;
    }

    pthread_mutex_lock(&s_voiceRegStateMutex[m_slot_id]);
    voice_reg_state_cache[m_slot_id]->default_roaming_indicator = default_roaming_indicator;
    pthread_mutex_unlock(&s_voiceRegStateMutex[m_slot_id]);

    isNeedNotifyStateChanged();
    return;
}

void RmcNetworkUrcHandler::handleNetworkInfo(const sp<RfxMclMessage>& msg) {
    /* +ENWINFO: <type>,<nw_info> */
    int err;
    int type;
    char *responseStr[2];
    RfxAtLine *line = msg->getRawUrc();
    sp<RfxMclMessage> urc;

    line->atTokStart(&err);
    if (err < 0) goto error;

    type = line->atTokNextint(&err);
    if (err < 0) goto error;

    asprintf(&responseStr[0], "%d", type);

    // get raw data of structure of NW info
    responseStr[1] = line->atTokNextstr(&err);
    if (err < 0) goto error;

    logD(LOG_TAG, "NWInfo: %s, len:%zu ,%s", responseStr[0], strlen(responseStr[1]),
            responseStr[1]);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_INFO,
            m_slot_id, RfxStringsData(responseStr, 2));
    responseToTelCore(urc);
    free(responseStr[0]);
    return;

error:
    logE(LOG_TAG, "handle Network Info error: %s", msg->getRawUrc()->getLine());
}

void RmcNetworkUrcHandler::handlePseudoCellInfo(const sp<RfxMclMessage>& msg) {
    /*
     * 1. URC when EAPC mode is 1
     *	  +EAPC: <num>,<type>,<plmn>,<lac>,<cid>,<arfcn>,<bsic>[
     *                ,<type>,<plmn>,<lac>,<cid>,<arfcn>,<bsic>]
     *	   num: 1 or 2
     *   type = 1: detected pseudo cells info
     *
     * 2. URC when EAPC mode is 2
     *  +EAPC: <num>,<type>[,<plmn>,<lac>,<cid>,<arfcn>,<bsic>]
     *   num: 1
     *   type = 2: In to a pseudo cell
     *   type = 3: Out of a pseudo cell
     */
    int err;
    int num;
    int response[13] = {0};
    RfxAtLine *line = msg->getRawUrc();
    sp<RfxMclMessage> urc;

    line->atTokStart(&err);
    if (err < 0) goto error;

    num = line->atTokNextint(&err);
    if (err < 0) goto error;

    response[0] = num;
    if (num != 1 && num != 2) goto error;

    // get raw data of structure of pseudo cell info
    for (int i = 0; i < num; i++) {
        // type
        response[i*6 + 1] = line->atTokNextint(&err);
        if (err < 0) goto error;

        if (response[i*6 + 1] == 2) {
            logD(LOG_TAG, "PseudoCellInfo: attached to a pseudo cell");
        } else if (response[i*6 + 1] == 3) {
            logD(LOG_TAG, "PseudoCellInfo: attached cell changed and is not a pseudo cell");
        }

        if (line->atTokHasmore()) {
            // plmn
            response[i*6 + 2] = line->atTokNextint(&err);
            if (err < 0) goto error;
            // lac
            response[i*6 + 3] = line->atTokNextint(&err);
            if (err < 0) goto error;
            // cid
            response[i*6 + 4] = line->atTokNextint(&err);
            if (err < 0) goto error;
            // arfcn
            response[i*6 + 5] = line->atTokNextint(&err);
            if (err < 0) goto error;
            // bsic
            response[i*6 + 6] = line->atTokNextint(&err);
            if (err < 0) goto error;
        }
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_PSEUDO_CELL_INFO,
            m_slot_id, RfxIntsData(response, 13));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "handle Pseudo Cell Info error: %s", msg->getRawUrc()->getLine());
}

void RmcNetworkUrcHandler::handleGsmFemtoCellInfo(const sp<RfxMclMessage>& msg) {
    int err = 0;
    char *responseStr = NULL;
    // Use INT_MAX: 0x7FFFFFFF denotes invalid value
    int INVALID = 0x7FFFFFFF;
    RfxAtLine *line = msg->getRawUrc();
    int is_csg_cell = 0;

    /* +ECSG:  <domain>,<state>,<plmn_id>,<act>,<is_csg_cell>,<csg_id>,<csg_icon_type>,<hnb_name>,<cause> */
    pthread_mutex_lock(&ril_nw_femtoCell_mutex);
    line->atTokStart(&err);
    if (err < 0) goto error;

    // domain
    femto_cell_cache[m_slot_id]->domain = line->atTokNextint(&err);
    if (err < 0) goto error;

    // state
    femto_cell_cache[m_slot_id]->state = line->atTokNextint(&err);
    if (err < 0) goto error;

    // plmn
    femto_cell_cache[m_slot_id]->plmn_id = line->atTokNextint(&err);
    if (err < 0) goto error;

    // act
    femto_cell_cache[m_slot_id]->act = line->atTokNextint(&err);
    if (err < 0) goto error;

    // is CSG
    is_csg_cell = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (is_csg_cell != femto_cell_cache[m_slot_id]->is_csg_cell) {
        femto_cell_cache[m_slot_id]->is_csg_cell = is_csg_cell;
        if (femto_cell_cache[m_slot_id]->is_csg_cell == 1) {
            // CSG id
            femto_cell_cache[m_slot_id]->csg_id = line->atTokNextint(&err);
            if (err < 0) goto error;

            // CSG icon type
            femto_cell_cache[m_slot_id]->csg_icon_type = line->atTokNextint(&err);
            if (err < 0) goto error;

            // hnb name
            responseStr = line->atTokNextstr(&err);
            if (err < 0) goto error;

            strncpy(femto_cell_cache[m_slot_id]->hnbName, responseStr, strlen(responseStr));
        } else {
            /* <csg_id>,<csg_icon_type>,<hnb_name> are only avaliable when <is_csg_cell> is 1. */
            femto_cell_cache[m_slot_id]->csg_id = INVALID;
            femto_cell_cache[m_slot_id]->csg_icon_type = INVALID;
            sprintf(femto_cell_cache[m_slot_id]->hnbName, "%d", INVALID);
        }

        // cause
        femto_cell_cache[m_slot_id]->cause = line->atTokNextint(&err);
        if (err < 0) goto error;

        // trigger event
        sendEvent(RFX_MSG_EVENT_FEMTOCELL_UPDATE, RfxVoidData(),
                m_channel_id, m_slot_id);
    }
    pthread_mutex_unlock(&ril_nw_femtoCell_mutex);
    return;

error:
    pthread_mutex_unlock(&ril_nw_femtoCell_mutex);
    logE(LOG_TAG, "There is something wrong with the onFemtoCellInfo URC, err=%d", err);
}

void RmcNetworkUrcHandler::handleCellInfoList(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int num = 0;
    RIL_CellInfo_v12 * response = NULL;
    RfxAtLine *line = msg->getRawUrc();
    sp<RfxMclMessage> urc;
    // +ECELL: <num_of_cell>...
    line->atTokStart(&err);
    if (err < 0) goto error;

    num = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (num < 1) {
            logE(LOG_TAG, "No cell info listed, num=%d", num);
            goto error;
    }

    logD(LOG_TAG, "Cell Info listed, number =%d", num);
    response = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
    memset(response, 0, num * sizeof(RIL_CellInfo_v12));
    err = getCellInfoListV12(line, num, response);
    if (err < 0) goto error;

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CELL_INFO_LIST,
            m_slot_id, RfxCellInfoData(response, num * sizeof(RIL_CellInfo_v12)));
    responseToTelCore(urc);
    freeOperatorNameOfCellInfo(num, response);
    return;

error:
    logE(LOG_TAG, "onCellInfoList parse error");
}

void RmcNetworkUrcHandler::handleNitzTzReceived(const sp<RfxMclMessage>& msg) {
    int err;
    char *utct = NULL;
    char *tz = NULL;
    int dst = 0;
    RfxAtLine *line = msg->getRawUrc();
    sp<RfxMclMessage> urc;

    char *nitz_string = NULL;
    time_t calendar_time;
    struct tm *t_info = NULL;

    /*+CTZEU: <tz>,<dst>,[<utime>].
    <utime>: string type value representing the universal time. The format is "YYYY/MM/DD,hh:mm:ss".
    <tz>: string type value representing the sum of the local time zone *NOT* plus dst.
          e.g. "-09", "+00" and "+09".
    <dst>: integer type value indicating whether <tz> includes daylight savings adjustment;*/

    /* Final format :  "yy/mm/dd,hh:mm:ss(+/-)tz[,dt]" */

    line->atTokStart(&err);
    if (err < 0) goto error;

    // <tz>
    tz = line->atTokNextstr(&err);
    if (err < 0) {
        logE(LOG_TAG, "There is no valid <tz>");
        goto error;
    }
    else {
        bNitzTzAvailble = 1;
        strncpy(ril_nw_nitz_tz, tz, MAX_NITZ_TZ_DST_LENGTH-1);
    }
    // <dst>
    dst = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "There is no valid <dst>");
        goto error;
    }
    else {
        bNitzDstAvailble = 1;
        if (snprintf(ril_nw_nitz_dst, MAX_NITZ_TZ_DST_LENGTH, "%d", dst) < 0) {
            logE(LOG_TAG, "save dst failed, write 0 by default");
            ril_nw_nitz_dst[0] = '0';
            ril_nw_nitz_dst[1] = 0;
        }
    }
    // <time>
    if (line->atTokHasmore()) {
        utct = line->atTokNextstr(&err);
        if (err < 0) goto error;

        struct tm utc_tm;
        memset(&utc_tm, 0, sizeof(struct tm));
        strptime(utct, "%Y/%m/%d,%H:%M:%S", &utc_tm);

        // "yy/mm/dd,hh:mm:ss(+/-)tz,dst"
        asprintf(&nitz_string, "%02d/%02d/%02d,%02d:%02d:%02d%s,%d",
                (utc_tm.tm_year)%100,
                utc_tm.tm_mon+1,
                utc_tm.tm_mday,
                utc_tm.tm_hour,
                utc_tm.tm_min,
                utc_tm.tm_sec,
                tz,
                dst);
        RFX_LOG_V(LOG_TAG, "NITZ:%s", nitz_string);
    } else {
        // get the system time to fullfit the NITZ string format
        calendar_time = time(NULL);
        if (-1 == calendar_time) return;

        t_info = gmtime(&calendar_time);
        if (NULL == t_info) return;

        // "yy/mm/dd,hh:mm:ss(+/-)tz,dst"
        asprintf(&nitz_string, "%02d/%02d/%02d,%02d:%02d:%02d%s,%d",
                (t_info->tm_year)%100,
                t_info->tm_mon+1,
                t_info->tm_mday,
                t_info->tm_hour,
                t_info->tm_min,
                t_info->tm_sec,
                tz,
                dst);
        RFX_LOG_V(LOG_TAG, "NITZ:%s", nitz_string);
    }
    // ignore local time information in the EMM INFORMATION if SIB16 is broadcast by the network
    if (!bSIB16Received && nitz_string != NULL) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NITZ_TIME_RECEIVED, m_slot_id,
                RfxStringData(nitz_string, (strlen(nitz_string)+1)));
        responseToTelCore(urc);
    }
    free(nitz_string);
    return;

error:
    logE(LOG_TAG, "handle NITZ error: %s", msg->getRawUrc()->getLine());
    bNitzTzAvailble = 0;
    bNitzDstAvailble = 0;
}

void RmcNetworkUrcHandler::handleLegacyNitzTzReceived(const sp<RfxMclMessage>& msg) {
    int err;
    char *localTime = NULL;
    char *tz = NULL;
    int dst = 0;
    RfxAtLine *line = msg->getRawUrc();
    sp<RfxMclMessage> urc;

    long offset = 0;
    struct tm lt_tm;
    struct tm utc_tm;

    char *nitz_string = NULL;
    time_t calendar_time;
    struct tm *t_info = NULL;

    /*
      +CTZE: <tz>,<dst>,[<time>].
        <tz>: string type value representing the sum of the local time zone
          *NOT* plus dst. e.g. "-09", "+00" and "+09".
        <dst>: integer type value indicating whether <tz> includes daylight savings adjustment;
        <time>: string type value representing the local time.
          The format is "YYYY/MM/DD,hh:mm:ss".
    */
    /* Final format :  "yy/mm/dd,hh:mm:ss(+/-)tz[,dt]" */

    line->atTokStart(&err);
    if (err < 0) goto error;

    // <tz>
    tz = line->atTokNextstr(&err);
    if (err < 0) {
        logE(LOG_TAG, "There is no valid <tz>");
        goto error;
    }
    bNitzTzAvailble = 1;
    strncpy(ril_nw_nitz_tz, tz, MAX_NITZ_TZ_DST_LENGTH-1);

    // <dst>
    dst = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "There is no valid <dst>");
        goto error;
    }
    bNitzDstAvailble = 1;
    if (snprintf(ril_nw_nitz_dst, MAX_NITZ_TZ_DST_LENGTH, "%d", dst) < 0) {
        logE(LOG_TAG, "save dst failed, write 0 by default");
        ril_nw_nitz_dst[0] = '0';
        ril_nw_nitz_dst[1] = 0;
    }

    offset = getTzOffset(tz);
    if (offset == -1) {
        logE(LOG_TAG, "fail to get valid TZ offset");
        goto error;
    }

    // <time>
    if (line->atTokHasmore()) {
        localTime = line->atTokNextstr(&err);
        if (err < 0) goto error;

        memset(&lt_tm, 0, sizeof(struct tm));
        strptime(localTime, "%Y/%m/%d,%H:%M:%S", &lt_tm);

        if (localToGmt(&utc_tm, &lt_tm, offset) < 0) {
            logE(LOG_TAG, "fail to convert local to GMT");
            goto error;
        }

        // "yy/mm/dd,hh:mm:ss(+/-)tz,dst"
        asprintf(&nitz_string, "%02d/%02d/%02d,%02d:%02d:%02d%s,%d",
                (utc_tm.tm_year)%100,
                utc_tm.tm_mon+1,
                utc_tm.tm_mday,
                utc_tm.tm_hour,
                utc_tm.tm_min,
                utc_tm.tm_sec,
                tz,
                dst);
        RFX_LOG_V(LOG_TAG, "NITZ:%s", nitz_string);
    } else {
        // get the system time to fullfit the NITZ string format
        calendar_time = time(NULL);
        if (-1 == calendar_time) return;

        t_info = gmtime(&calendar_time);
        if (NULL == t_info) return;

        // "yy/mm/dd,hh:mm:ss(+/-)tz,dst"
        asprintf(&nitz_string, "%02d/%02d/%02d,%02d:%02d:%02d%s,%d",
                (t_info->tm_year)%100,
                t_info->tm_mon+1,
                t_info->tm_mday,
                t_info->tm_hour,
                t_info->tm_min,
                t_info->tm_sec,
                tz,
                dst);
        RFX_LOG_V(LOG_TAG, "NITZ:%s", nitz_string);
    }
    // ignore local time information in the EMM INFORMATION if SIB16 is broadcast by the network
    if (!bSIB16Received && nitz_string != NULL) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NITZ_TIME_RECEIVED, m_slot_id,
                RfxStringData(nitz_string, (strlen(nitz_string)+1)));
        responseToTelCore(urc);
    }
    free(nitz_string);
    return;

error:
    logE(LOG_TAG, "handle legacy NITZ error: %s", msg->getRawUrc()->getLine());
    bNitzTzAvailble = 0;
    bNitzDstAvailble = 0;
}

void RmcNetworkUrcHandler::handleNitzOperNameReceived(const sp<RfxMclMessage>& msg) {
    int err;
    int length, id;
    char *oper_code;
    char *oper_lname;
    char *oper_sname;
    char *str;
    int is_lname_hex_str = 0;
    int is_sname_hex_str = 0;
    sp<RfxMclMessage> urc;
    char result[MAX_TOE_BUFFER_LEN] = {0};

    /* +CIEV: 10,"PLMN","long_name","short_name" */
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    id = line->atTokNextint(&err);
    if (err < 0 || id != 10) return;

    if (!line->atTokHasmore()) {
        logE(LOG_TAG, "There is no NITZ data");
        return;
    }

    oper_code  = m_ril_nw_nitz_oper_code[m_slot_id];
    oper_lname = m_ril_nw_nitz_oper_lname[m_slot_id];
    oper_sname = m_ril_nw_nitz_oper_sname[m_slot_id];

    /* FIXME: it is more good to transfer the OPERATOR NAME to the Telephony Framework directly */
    pthread_mutex_lock(&ril_nw_nitzName_mutex[m_slot_id]);
    // logD(LOG_TAG, "Get ril_nw_nitzName_mutex in the onNitzOperNameReceived");

    str = line->atTokNextstr(&err);
    if (err < 0) goto error;
    strncpy(oper_code, str, MAX_OPER_NAME_LENGTH);
    oper_code[MAX_OPER_NAME_LENGTH-1] = '\0';

    str = line->atTokNextstr(&err);
    if (err < 0) goto error;
    strncpy(oper_lname, str, MAX_OPER_NAME_LENGTH);
    oper_lname[MAX_OPER_NAME_LENGTH-1] = '\0';

    str = line->atTokNextstr(&err);
    if (err < 0) goto error;
    strncpy(oper_sname, str, MAX_OPER_NAME_LENGTH);
    oper_sname[MAX_OPER_NAME_LENGTH-1] = '\0';

    /* ALPS00459516 start */
    if ((strlen(oper_lname)%8) == 0) {
        logD(LOG_TAG, "strlen(oper_lname)=%zu", strlen(oper_lname));

        length = strlen(oper_lname);
        if (oper_lname[length-1] == '@') {
            oper_lname[length-1] = '\0';
            logD(LOG_TAG, "remove @ new oper_lname:%s", oper_lname);
        }
    }

    if ((strlen(oper_sname)%8) == 0) {
        logD(LOG_TAG, "strlen(oper_sname)=%zu", strlen(oper_sname));

        length = strlen(oper_sname);
        if (oper_sname[length-1] == '@') {
            oper_sname[length-1] = '\0';
            logD(LOG_TAG, "remove @ new oper_sname:%s", oper_sname);
        }
    }
    /* ALPS00459516 end */

    /* ALPS00262905 start
       +CIEV: 10, <plmn_str>,<full_name_str>,<short_name_str>,<is_full_name_hex_str>,<is_short_name_hex_str> for UCS2 string */
    is_lname_hex_str = line->atTokNextint(&err);
    if (err >= 0) {
        if (is_lname_hex_str == 1) {
            convertToUtf8String(oper_lname, MAX_OPER_NAME_LENGTH);
        } else {
            convertToUtf8StringFromAscii(oper_lname, MAX_OPER_NAME_LENGTH);
        }

        is_sname_hex_str = line->atTokNextint(&err);
        if (err >= 0) {
            if (is_sname_hex_str == 1) {
                convertToUtf8String(oper_sname, MAX_OPER_NAME_LENGTH);
            } else {
                convertToUtf8StringFromAscii(oper_sname, MAX_OPER_NAME_LENGTH);
            }
        }
    }
    /* ALPS00262905 end */

    logD(LOG_TAG, "Get NITZ Operator Name of RIL %d: %s %s %s", m_slot_id+1, oper_code, oper_lname, oper_sname);
    rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_CODE, m_slot_id).string(), (char*) "");
    rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_LNAME, m_slot_id).string(), (char*) "");
    rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_SNAME, m_slot_id).string(), (char*) "");
    rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_CODE, m_slot_id).string(), oper_code);
    rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_LNAME, m_slot_id).string(), oper_lname);
    rfx_property_set(String8::format("%s_%d", PROPERTY_NITZ_OPER_SNAME, m_slot_id).string(), oper_sname);

    //report toe info
    err = snprintf(result, MAX_TOE_BUFFER_LEN, "%04zu%s%04zu%s%04zu%s",
            strlen(oper_lname), oper_lname, strlen(oper_sname), oper_sname, strlen(oper_code), oper_code);
    if (err < 0) goto error;
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_TOE_INFO_IND,
                            m_slot_id, RfxStringData(result, strlen(result)));
    responseToTelCore(urc);
    pthread_mutex_unlock(&ril_nw_nitzName_mutex[m_slot_id]);
    isNeedNotifyStateChanged();
    return;

error:
    pthread_mutex_unlock(&ril_nw_nitzName_mutex[m_slot_id]);
}

void RmcNetworkUrcHandler::handleSib16TimeInfoReceived(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int id = 0;
    int dst = 0;
    int ls = 0;
    int lto = 0;
    int dt = 0;
    long long raw_utc = 0;

    // difference in milliseconds of epochs used by Android (1970) and the network (1900)
    long long epochDiffInMillis = 2208988800000;

    // abs_time = <raw_utc>*10 - <epochDiffInMillis>
    long long abs_time = 0;

    // currentUtcTimeMillis = raw_utc*10 - epochDiffInMillis + elapsedTimeSinceBroadcast
    time_t  currentUtcTimeMillis;  // time_t is measured in seconds.
    struct tm *ts;

    char* responseStr[5];
    char nitz_string[30];
    char sib16_time_string[70];
    int i;

    /* +CIEV: 11, <UTC>, [<daylightSavingTime >], [<leapSeconds >], [<localTimeOffset >], <delayTicks>
      <UTC>: The field counts the number of UTC seconds in 10 ms units since 00:00:00 on Gregorian calendar date 1 January, 1900
      <daylightSavingTime>: It indicates if and how daylight saving time (DST) is applied to obtain the local time.
      <leapSeconds>: GPS time - leapSeconds = UTC time.
      <localTimeOffset>: Offset between UTC and local time in units of 15 minutes.
      <delayTicks>: Time difference from AS receive SIB16 to L4 receive notify

      Final format :  "yy/mm/dd,hh:mm:ss(+/-)tz[,dst]" */

    /* +CIEV: 10,"PLMN","long_name","short_name" */
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) return;

    id = line->atTokNextint(&err);
    if (err < 0 || id != 11) return;

    if (!line->atTokHasmore()) {
        logE(LOG_TAG, "There is no SIB16 time info");
        return;
    }

    // get <UTC>
    raw_utc = line->atTokNextlonglong(&err);
    if (err < 0) {
        logE(LOG_TAG, "ERROR occurs when parsing <UTC> of the SIB16 time info URC");
        return;
    }
    asprintf(&responseStr[0], "%lld", raw_utc);

    // get <daylightSavingTime >
    dst = line->atTokNextint(&err);
    if (err < 0) {
        if (bNitzDstAvailble) {
            logE(LOG_TAG, "Use <daylightSavingTime> from (E)MM INFORMATION");
            asprintf(&responseStr[1], "%s", ril_nw_nitz_dst);
        }
        else {
            logE(LOG_TAG, "ERROR occurs when parsing <daylightSavingTime> of the SIB16 time info URC");
            responseStr[1] = (char*)"";
            dst = -1;
        }
    } else {
        asprintf(&responseStr[1], "%d", dst);
    }

    // get <leapSeconds >
    ls = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "ERROR occurs when parsing <leapSeconds> of the SIB16 time info URC");
        responseStr[2] = (char*)"";
    } else {
        asprintf(&responseStr[2], "%d", ls);
    }

    // get <localTimeOffset >
    lto = line->atTokNextint(&err);
    if (err < 0) {
        if (bNitzTzAvailble) {
            logE(LOG_TAG, "Use <localTimeOffset> from (E)MM INFORMATION");
            asprintf(&responseStr[3], "%s", ril_nw_nitz_tz);
        }
        else {
            logE(LOG_TAG, "ERROR occurs when parsing <localTimeOffset> of the SIB16 time info URC");
            asprintf(&responseStr[3], "%s", "+00");
        }
    } else {
        if (lto >= 0) {
            asprintf(&responseStr[3], "+%d", lto);
        } else {
            asprintf(&responseStr[3], "%d", lto);
        }
    }

    // get <delayTicks >
    dt = line->atTokNextint(&err);
    if (err < 0) {
        logD(LOG_TAG, "ERROR occurs when parsing <delayTicks> of the SIB16 time info URC");
        return;
    }
    asprintf(&responseStr[4], "%d", dt);

    // logD(LOG_TAG, "SIB16 time info: UTC %s,daylightSavingTime %s,leapSeconds %s,localTimeOffset %s,delayTicks %s",
    //        responseStr[0], responseStr[1], responseStr[2], responseStr[3], responseStr[4]);

    abs_time = (raw_utc * 10) - epochDiffInMillis;

    currentUtcTimeMillis = ((raw_utc * 10) - epochDiffInMillis + dt)/1000;
    logD(LOG_TAG, "currentUtcTimeMillis: %s", ctime(&currentUtcTimeMillis));

    ts = gmtime(&currentUtcTimeMillis);
    if (NULL == ts) return;

    memset(nitz_string, 0, sizeof(nitz_string));
    memset(sib16_time_string, 0, sizeof(sib16_time_string));
    if (dst != -1) {
        // nitz_string with dst
        sprintf(nitz_string, "%02d/%02d/%02d,%02d:%02d:%02d%s,%s",  // "yy/mm/dd,hh:mm:ss(+/-)tz[,dst]"
                (ts->tm_year)%100,
                ts->tm_mon+1,
                ts->tm_mday,
                ts->tm_hour,
                ts->tm_min,
                ts->tm_sec,
                responseStr[3],
                responseStr[1]);

        // sib16_time_string with dst
        // "yy/mm/dd,hh:mm:ss(+/-)tz,dst,abs_time"
        sprintf(sib16_time_string, "%02d/%02d/%02d,%02d:%02d:%02d%s,%s,%lli",
                (ts->tm_year)%100,
                ts->tm_mon+1,
                ts->tm_mday,
                ts->tm_hour,
                ts->tm_min,
                ts->tm_sec,
                responseStr[3],
                responseStr[1],
                abs_time);
    } else {
        // nitz_srting without dst
        sprintf(nitz_string, "%02d/%02d/%02d,%02d:%02d:%02d%s",  // "yy/mm/dd,hh:mm:ss(+/-)tz"
                (ts->tm_year)%100,
                ts->tm_mon+1,
                ts->tm_mday,
                ts->tm_hour,
                ts->tm_min,
                ts->tm_sec,
                responseStr[3]);
        // sib16_time_string with dst=0
        // "yy/mm/dd,hh:mm:ss(+/-)tz,dst,abs_time"
        sprintf(sib16_time_string, "%02d/%02d/%02d,%02d:%02d:%02d%s,0,%lli",  // "yy/mm/dd,hh:mm:ss(+/-)tz"
                (ts->tm_year)%100,
                ts->tm_mon+1,
                ts->tm_mday,
                ts->tm_hour,
                ts->tm_min,
                ts->tm_sec,
                responseStr[3],
                abs_time);
    }
    logD(LOG_TAG, "NITZ:%s, SIB16_Time:%s", nitz_string, sib16_time_string);

    bSIB16Received = 1;
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NITZ_TIME_RECEIVED, m_slot_id,
        RfxStringData(nitz_string, (strlen(nitz_string)+1)));
    responseToTelCore(urc);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIB16_TIME_INFO_IND, m_slot_id,
        RfxStringData(nitz_string, (strlen(nitz_string)+1)));
    responseToTelCore(urc);

    for (i = 0; i < 5; i++) {
        if ((responseStr[i] != NULL) && strcmp(responseStr[i], "")) {
            free(responseStr[i]);
        }
    }
}

void RmcNetworkUrcHandler::handleNetworkEventReceived(const sp<RfxMclMessage>& msg) {
    /* +EREGINFO: <Act>,<event_type>
     * <Act>: Access technology (RAT)
     * <event_type>: 0: for RAU event
     *               1: for TAU event
     */
    int err;
    int response[2] = {0};
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    // get <Act>
    response[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    // get <event_type>
    response[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    logV(LOG_TAG, "onNetworkEventReceived: <Act>:%d, <event_typ>:%d", response[0], response[1]);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_EVENT, m_slot_id, RfxIntsData(response, 2));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the URC +EREGINFO");
}

void RmcNetworkUrcHandler::handleWfcStateChanged(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int stat[4] = { 0 };
    int status = 0;
    stat[0] = convertRegState(urc_data_reg_state_cache[m_slot_id]->registration_state, false);
    stat[1] = convertPSNetworkType(urc_data_reg_state_cache[m_slot_id]->radio_technology);
    RfxAtLine *line = msg->getRawUrc();
    int prevStatus = ril_wfc_reg_status[m_slot_id];

    line->atTokStart(&err);
    if (err < 0) goto error;

    status = line->atTokNextint(&err);
    if (err < 0) goto error;

    // logD(LOG_TAG, "handleWfcStateChanged: <status>:%d", status);
    if (status == 3) {
        // modem notify the VoData is used, consider it as VoWiFi case
        status = 1;
    }
    if (status == 0 || status == 1) {
        updateWfcState(status);
        stat[2] = combineWfcEgregState();
    } else {
        // only 0 and 1 are valid values
        goto error;
    }

    if (ril_wfc_reg_status[m_slot_id] == 1) {
        stat[3] = RADIO_TECH_IWLAN;
    } else {
        stat[3] = convertPSNetworkType(ril_data_urc_rat);
    }

    sendEvent(RFX_MSG_EVENT_PS_NETWORK_STATE, RfxIntsData(stat, 4),
        RIL_CMD_PROXY_3, m_slot_id);
    RFX_LOG_V(LOG_TAG, "Send RFX_MSG_EVENT_PS_NETWORK_STATE");

    if (s_iwlan_in_legacy_mode && (prevStatus != status)) {
        isNeedNotifyStateChanged();
    }
    return;

error:
    logE(LOG_TAG, "There is something wrong with the URC");
}

void RmcNetworkUrcHandler::handleModulationInfoReceived(const sp<RfxMclMessage>& msg) {
    /* +EMODCFG: <mode> */
    int err;
    int response = 0;
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    // get <mode>
    response = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (response > 0xFF) goto error;

    logD(LOG_TAG, "handleModulationInfoReceived: <mode>:%d", response);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_MODULATION_INFO, m_slot_id, RfxIntsData(&response, 1));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the URC +EMODCFG");
}

void RmcNetworkUrcHandler::handleEnhancedOperatorNameDisplay(const sp<RfxMclMessage>& msg) {
    int err;
    int pnn, opl, opl5g = -1;
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    /* +EONS: <PNN_service>,<OPL_service> */

    line->atTokStart(&err);
    if (err < 0) return;

    pnn = line->atTokNextint(&err);
    if (err < 0 || pnn < 0 || pnn > 1) return;

    opl = line->atTokNextint(&err);
    if (err < 0 || opl < 0 || opl > 1) return;

    if (isNrSupported() && line->atTokHasmore()) {
        opl5g = line->atTokNextint(&err);
        if (err < 0 || opl5g < 0 || opl5g > 1) return;
    }
    logD(LOG_TAG, "Get EONS info of slot %d: %d %d %d", m_slot_id, pnn, opl, opl5g);

    if (pnn == 1 && (opl == 1 || opl5g == 1)) {
        eons_info[m_slot_id].eons_status = EONS_INFO_RECEIVED_ENABLED;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(urc);
    } else {
        eons_info[m_slot_id].eons_status = EONS_INFO_RECEIVED_DISABLED;
    }
}

void RmcNetworkUrcHandler::handleMccMncChanged(const sp<RfxMclMessage>& msg) {
    // +EMCCMNC: <mcc>,<mnc>
    // <mcc>: MCC id, string type;
    // <mnc>: MNC id, string type;

    int err = 0;
    char* mcc = NULL;
    char* mnc = NULL;
    char* mccmnc = NULL;
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    mcc = line->atTokNextstr(&err);
    if (err < 0) goto error;

    mnc = line->atTokNextstr(&err);
    if (err < 0) goto error;

    // Invalid MCC MNC information will be sent as +EMCCMNC:FFF,FFF, ignore it
    if ((mcc == NULL || mnc == NULL)
            || (strlen(mcc) == 0 || strlen(mnc) == 0)
            || (strcmp(mcc, "FFF") == 0 || strcmp(mnc, "FFF") == 0)) {
        goto error;
    }

    err = asprintf(&mccmnc, "%s%s", mcc, mnc);
    if (err < 0) goto error;

    if (mccmnc == NULL || strlen(mccmnc) == 0) {
        goto error;
    }

    logD(LOG_TAG, "handleMccMncChanged: mccmnc=%s", mccmnc);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_MCCMNC_CHANGED, m_slot_id,
            RfxStringData(mccmnc, strlen(mccmnc)));
    responseToTelCore(urc);
    free(mccmnc);
    return;

error:
    if (mccmnc) free(mccmnc);
    logE(LOG_TAG, "handleMccMncChanged: there is something wrong with the +EMCCMNC URC");
}

void RmcNetworkUrcHandler::onHandleTimer() {
    // do something
}

void RmcNetworkUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    // handle event
}

bool RmcNetworkUrcHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg,
        RIL_RadioState radioState) {
    bool reject = false;
    if (RADIO_STATE_UNAVAILABLE == radioState) {
        if (((strStartsWith(msg->getRawUrc()->getLine(), "+ERPRAT:"))
                || (strStartsWith(msg->getRawUrc()->getLine(), "+EIPRL:")))
                        && (RmcWpRequestHandler::isWorldModeSwitching())) {
            reject = false;
        } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EWFC:")) {
            reject = false;
        } else {
            if (allowed_urc != NULL) {
                int length = sizeof(allowed_urc) / sizeof(char *);
                for (int i = 0; i < length && allowed_urc[i] != NULL; i++) {
                    if ((strStartsWith(msg->getRawUrc()->getLine(), allowed_urc[i]))) {
                        if (RmcWpRequestHandler::isWorldModeSwitching()) {
                            reject = false;
                        } else {
                            logD(LOG_TAG, "onCheckIfRejectMessage, urc = %s, i = %d, length = %d.",
                                    allowed_urc[i], i, length);
                            reject = true;
                        }
                        break;
                    } else {
                        reject = true;
                    }
                }
            }
        }
    }
    return reject;
}

void RmcNetworkUrcHandler::onImsEmergencySupportR9(const sp<RfxMclMessage>& msg) {
    int err;
    sp<RfxAtResponse> p_response;
    int rat = 0, ecc = 0;
    sp<RfxMclMessage> urc;
    RfxAtLine* line = msg->getRawUrc();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    rat = line->atTokNextint(&err);
    if (err < 0) goto error;

    ecc = line->atTokNextint(&err);
    if (err < 0) goto error;

    if ( (rat == 3 || rat == 4) && (ecc == 1)) {
        ims_ecc_state[m_slot_id] = true;
    } else {
        ims_ecc_state[m_slot_id] = false;
    }
    logD(LOG_TAG, "onImsEmergencySupportR9: %d", ims_ecc_state[m_slot_id]? 1: 0);

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_EMERGENCY_SUPPORT_STATE,
            ims_ecc_state[m_slot_id]? 1: 0);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            m_slot_id, RfxVoidData());
    // response to TeleCore
    responseToTelCore(urc);
error:
    return;
}

void RmcNetworkUrcHandler::handleVoLteEmergencySupportedChanged(const sp<RfxMclMessage>& msg) {
    // +CNEMS1: <s1_support>
    int err = 0;
    int s1Support;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) {
        logE(LOG_TAG, "VoLteEmergency URC parse failed");
        return;
    }

    s1Support = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "VoLteEmergency URC parse failed");
        return;
    }
    logD(LOG_TAG, "handleVoLteEmergencySupportedChanged: s1Support[%d]", s1Support);

    pthread_mutex_lock(&imsEmergencyMutex[m_slot_id]);
    if (s1Support != imsEmergencyInfo[m_slot_id]->volteEmcSupported) {
        imsEmergencyInfo[m_slot_id]->volteEmcSupported = s1Support;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_VOLTE_EMC_SUPPORTED, s1Support);
    }
    pthread_mutex_unlock(&imsEmergencyMutex[m_slot_id]);
}

void RmcNetworkUrcHandler::handleVoNrEmergencySupportedChanged(const sp<RfxMclMessage>& msg) {
    // +CNEM5G: <emc_support>,<emf_support>
    int err = 0;
    int emcSupport = 0, emfSupport = 0;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) {
        logE(LOG_TAG, "VoNrEmergency URC parse failed");
        return;
    }

    emcSupport = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "VoNrEmergency URC emcSupport parse failed");
        return;
    }

    emfSupport = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "VoNrEmergency URC emfSupport parse failed");
        return;
    }

    logD(LOG_TAG, "handleVoLteEmergencySupportedChanged: emcSupport[%d], emfSupport[%d]", emcSupport, emfSupport);

    pthread_mutex_lock(&imsEmergencyMutex[m_slot_id]);
    if (emcSupport != imsEmergencyInfo[m_slot_id]->vonrEmcSupported) {
        imsEmergencyInfo[m_slot_id]->vonrEmcSupported = emcSupport;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_VONR_EMC_SUPPORTED, emcSupport);
    }
    if (emfSupport != imsEmergencyInfo[m_slot_id]->vonrEmfSupported) {
        imsEmergencyInfo[m_slot_id]->vonrEmfSupported = emfSupport;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_VONR_EMF_SUPPORTED, emfSupport);
    }
    pthread_mutex_unlock(&imsEmergencyMutex[m_slot_id]);
}

// MUSE WFC requirement
void RmcNetworkUrcHandler::handleCellularQualityReport(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int report[2];
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    report[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    report[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    logD(LOG_TAG, "onCellularQualityReport: %d, %d", report[0], report[1]);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CELLULAR_QUALITY_CHANGED_IND, m_slot_id,
            RfxIntsData(report, 2));
    responseToTelCore(urc);

    return;
error:
    logE(LOG_TAG, "There is something wrong with the onCellularQualityReport URC");
}
// MUSE WFC requirement

void RmcNetworkUrcHandler::handleConnectionStateReport(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int mode = 0, state = 0, access = 0;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);

    mode = line->atTokNextint(&err);
    if (err < 0) goto error;

    state = line->atTokNextint(&err);
    if (err < 0) goto error;

    access = line->atTokNextint(&err);
    if (err < 0) goto error;

    connectionStateReport(mode, state, access);
    return;
error:
    logE(LOG_TAG, "There is something wrong with the handleConnectionStateReport URC");
    return;
}

void RmcNetworkUrcHandler::handleScgConnectionChange(
        int stat, unsigned int lac, unsigned long long cid, int new_act) {
    RFX_UNUSED(lac);
    RFX_UNUSED(cid);

    pthread_mutex_lock(&scg_connection_state_Mutex[m_slot_id]);
    int old_stat = scg_connection_state[m_slot_id];  // current state
    int new_stat = 0;

    if ((stat == 1 || stat == 5) &&  // in service
        (new_act == 0x4000)) { // nsa
        new_stat = 1;
    }
    logD(LOG_TAG, "handleScgConnectionChange old=%d, new=%d",
            old_stat, new_stat);

    if (old_stat != new_stat) {
        scg_connection_state[m_slot_id] = new_stat;
        sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(), RIL_CMD_PROXY_3, m_slot_id);
    }
    pthread_mutex_unlock(&scg_connection_state_Mutex[m_slot_id]);

    // JP_mode
    if (isJpMode()) {
        if (old_stat == 1 && new_stat == 0) {
            startJpModeTimer2();
        } else if (old_stat == 0 && new_stat == 1) {
            stopJpModeTimer1();
            stopJpModeTimer2();
        }

        if (!(stat == 1 || stat == 5) ||  // not in service
                !(new_act == 0x1000 || new_act == 0x2000 || // or not lte/nr
                  new_act == 0x4000 ||new_act == 0x8000)) {
            stopJpModeTimer1();
            stopJpModeTimer2();
        }
    }
    // JP mode end
    return;
}

void RmcNetworkUrcHandler::handleEnhancedMccMncChanged(const sp<RfxMclMessage>& msg) {
    // +EOPSU: <rat>,<mcc>,<mnc>
    // <rat>: Integer type, the RAT of report PLMN
    // <mcc>: MCC id, string type;
    // <mnc>: MNC id, string type;

    int err = 0;
    int rat = 0; //not used now
    char* mcc = NULL;
    char* mnc = NULL;
    char* mccmnc = NULL;
    sp<RfxMclMessage> urc = NULL;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    rat = line->atTokNextint(&err);
    if (err < 0) goto error;

    mcc = line->atTokNextstr(&err);
    if (err < 0) goto error;

    mnc = line->atTokNextstr(&err);
    if (err < 0) goto error;

    // Invalid MCC MNC information will be sent as +EMCCMNC:FFF,FFF, ignore it
    if ((mcc == NULL || mnc == NULL)
            || (strlen(mcc) == 0 || strlen(mnc) == 0)
            || (strcmp(mcc, "FFF") == 0 || strcmp(mnc, "FFF") == 0)) {
       goto error;
    }

    err = asprintf(&mccmnc, "%s%s", mcc, mnc);
    if (err < 0) goto error;

    if (mccmnc == NULL || strlen(mccmnc) == 0) {
        goto error;
    }

    logD(LOG_TAG, "handleEnhancedMccMncChanged: mccmnc=%s", mccmnc);

    getMclStatusManager()->setString8Value(
            RFX_STATUS_KEY_MCCMNC_CHANGED,
            String8::format("%s", mccmnc));

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_MCCMNC_CHANGED, m_slot_id,
            RfxStringData(mccmnc, strlen(mccmnc)));
    responseToTelCore(urc);
    free(mccmnc);
    return;

error:
    if (mccmnc) free(mccmnc);
    logE(LOG_TAG, "handleEnhancedMccMncChanged: there is something wrong with the +EOPSU URC");
}

int getNrbByBw(int bw) {
    switch (bw) {
    case 14:
        return 6;
    case 30:
        return 15;
    case 50:
        return 25;
    case 100:
        return 50;
    case 150:
        return 75;
    case 200:
        return 100;
    default:
        return 0;
    }
}

void RmcNetworkUrcHandler::handleCaInfoChanged(const sp<RfxMclMessage>& msg) {
    // ca_info- CA_CONFIGURED(0)/CA_NOT_CONFIGURATED(1)/CA_ACTIVATED(2)/CA_DEACTIVATED(3)
    // +ECAINFO: <ca_info>,<pcell_bw>,<scell_bw1>,<scell_bw2>,<scell_bw3>,<scell_bw4>
    bool littleE = false;
    int i = 1;
    char *ip = (char*) &i;
    if (*ip == 0x01) littleE = true;

    int err = 0;
    int ca_info = 0;
    int pcell_bw = 0, scell_bw1 = 0, scell_bw2 = 0, scell_bw3 = 0, scell_bw4 = 0;
    int iTotalNRB = 0, pre_iTotalNRB = 0;
    char* iTotalNRBp;
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();
    OEM_CA_INFO* cainfo = (OEM_CA_INFO*) calloc(1, sizeof(OEM_CA_INFO));
    if (cainfo == NULL) goto error;
    memset(cainfo, 0, sizeof(OEM_CA_INFO));
    // initialize cainfo
    cainfo->messageId[0] = 0x04;
    cainfo->messageId[2] = 0x11;
    cainfo->payload_size[3] = 0x09;
    cainfo->tagOfCa = 0xA0;
    cainfo->lengthOfCa = 0x01;
    cainfo->tagForTotalNRB = 0xA1;
    cainfo->lengthOfTotalNRBfield = 0x04;

    line->atTokStart(&err);
    if (err < 0) goto error;

    ca_info = line->atTokNextint(&err);
    if (err < 0) goto error;

    pcell_bw = line->atTokNextint(&err);
    if (err < 0) goto error;

    scell_bw1 = line->atTokNextint(&err);
    if (err < 0) goto error;

    scell_bw2 = line->atTokNextint(&err);
    if (err < 0) goto error;

    scell_bw3 = line->atTokNextint(&err);
    if (err < 0) goto error;

    scell_bw4 = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (ca_info == 0) cainfo->statusOfCa = 0x01;

    iTotalNRB = getNrbByBw(pcell_bw) +
            getNrbByBw(scell_bw1) +
            getNrbByBw(scell_bw2) +
            getNrbByBw(scell_bw3) +
            getNrbByBw(scell_bw4);
    iTotalNRBp = (char*) &iTotalNRB;

    if (littleE) {
        // little to big
        cainfo->totalNRB[0] = iTotalNRBp[3];
        cainfo->totalNRB[1] = iTotalNRBp[2];
        cainfo->totalNRB[2] = iTotalNRBp[1];
        cainfo->totalNRB[3] = iTotalNRBp[0];
    }

    if (oem_ca_info_cache[m_slot_id] != NULL) {
        pre_iTotalNRB = oem_ca_info_cache[m_slot_id]->totalNRB[0];
        for (i = 1; i < 4; i++) {
            pre_iTotalNRB = pre_iTotalNRB<<2;
            pre_iTotalNRB += oem_ca_info_cache[m_slot_id]->totalNRB[i];
        }
    }
    logI(LOG_TAG, "littleE=%s, pre_statusOfCa=%02X, stattusOfCa=%02X,"
        " pre_iTotalNRB=%d, iTotalNRB=%d",
        littleE?"true":"false",
        oem_ca_info_cache[m_slot_id] == NULL?
            0x00 : oem_ca_info_cache[m_slot_id]->statusOfCa,
        cainfo->statusOfCa,
        pre_iTotalNRB, iTotalNRB);

    if (oem_ca_info_cache[m_slot_id] == NULL ||  // it's the first one
        (oem_ca_info_cache[m_slot_id] != NULL &&  // compare with the previous one
        (oem_ca_info_cache[m_slot_id]->statusOfCa != cainfo->statusOfCa ||
         pre_iTotalNRB != iTotalNRB))) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_OEM_HOOK_RAW, m_slot_id,
                    RfxRawData((char*) cainfo, sizeof(OEM_CA_INFO)));
        responseToTelCore(urc);
    }
    if (oem_ca_info_cache[m_slot_id] != NULL) {
        free(oem_ca_info_cache[m_slot_id]);
    }
    oem_ca_info_cache[m_slot_id] = cainfo;

    // just trigger poll service state here, CA cache supposed to be updated then.
    isNeedNotifyStateChanged();
    return;

error:
    if (cainfo) free(cainfo);
    return;
}

void RmcNetworkUrcHandler::handleBarringInfoReport(const sp<RfxMclMessage>& msg) {
    // +EBARRING: <rat>,<count>,<serviceType>,<barringType>,<factor>,<timeSeconds>,<isBarred>
    const int umtsTypes[] = {0, 1, 2, 8}; /*CS_SERVICE, PS_SERVICE, CS_VOICE, EMERGENCY*/
    const int lteTypes[] = {3, 4, 5, 6, 7, 8, 9};
    const int saTypes[] = {3, 4, 5, 6, 7, 8, 9, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
                1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022,
                1023, 1024, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032};
    logD(LOG_TAG, "[%s] ++++", __FUNCTION__);
    RIL_BarringInforResponse *barringInfoResp =
            (RIL_BarringInforResponse*) calloc(1, sizeof(RIL_BarringInforResponse));
    RIL_BarringInfo *pBarringInfos = NULL;
    int err = 0, count = 0, rat = 0, number = 0, i, j;
    const int *pType;
    int serviceType, factor, timeSeconds, isBarred;
    RIL_BarringType barringType = BT_NONE;
    sp<RfxMclMessage> urc = NULL;
    RfxAtLine *line = msg->getRawUrc();

    if (barringInfoResp == NULL) goto error;
    memset(barringInfoResp, 0, sizeof(RIL_BarringInforResponse));

    line->atTokStart(&err);
    if (err < 0) goto error;

    rat = line->atTokNextint(&err);
    if (err < 0) goto error;

    count = line->atTokNextint(&err);
    if (err < 0) goto error;
    logV(LOG_TAG, "[%s] ++++, rat = %d, count = %d", __FUNCTION__, rat, count);
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

    for (i = 0; i < count; i++) {
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
    return;
}

void RmcNetworkUrcHandler::sendDisableEcampuiChanged() {
    int response[7] ={0};
    sp<RfxMclMessage> urc;
    /* response[0]
     * 0: out of service
     * 1: 1 in service
     * 4: notify AP to disable fake state as long as RILD gets EREG/EGREG.
     */
    response[0] = 4;

    receivedEcampUI = false;

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED,
            m_slot_id, RfxIntsData(response, 7));
    // response to TeleCore
    responseToTelCore(urc);
    return;
}

void RmcNetworkUrcHandler::handleEcampuiChanged(const sp<RfxMclMessage>& msg) {
    int response[7] ={0};
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();
    int err;
    // format: +ECAMPUI: <reg>, <eAct>, <PLMN>, <lac>, <dcnr_restricted>, <endc_sib>, <endc_available>
    line->atTokStart(&err);
    if (err < 0) goto error;
    // reg, could be 0 or 1 only.
    response[0] = line->atTokNextint(&err);
    if (err < 0) goto error;
    // eAct
    response[1] = line->atTokNextint(&err);
    if (err < 0) goto error;
    // convert to CS network type so that UI looks well (eg, show 3G indead of E)
    response[1] = convertCSNetworkType(response[1]);
    // plmn
    response[2] = line->atTokNextint(&err);
    if (err < 0) goto error;
    // lac
    response[3] = line->atTokNextint(&err);
    if (err < 0) goto error;
    // dcnr_restricted
    response[4] = line->atTokNextint(&err);
    if (err < 0) goto error;
    // endc_sib
    response[5] = line->atTokNextint(&err);
    if (err < 0) goto error;
    // endc_available
    response[6] = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (checkNrCapability(m_slot_id) < 0) {
        // this slot doesn't support NSA. clear it.
        response[4] = 0;
        response[5] = 0;
        response[6] = 0;
    }

    receivedEcampUI = true;

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_PS_NETWORK_STATE_CHANGED,
            m_slot_id, RfxIntsData(response, 7));
    // response to TeleCore
    responseToTelCore(urc);
    return;
error:
    logE(LOG_TAG, "handleEcampuiChanged unknown error");
    return;
}

void RmcNetworkUrcHandler::handleEndcCapabilityChanged(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int is_endc_supported = 0;
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);

    is_endc_supported = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (is_endc_supported == 0 || is_endc_supported == 1) {
        if (endc_capability[m_slot_id] != is_endc_supported) {
            endc_capability[m_slot_id] = is_endc_supported;
            urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                        m_slot_id, RfxVoidData());
            responseToTelCore(urc);
        }
    } else {
        goto error;
    }
    return;
error:
    logE(LOG_TAG, "There is something wrong with the handleEndcCapabilityChanged URC");
    return;
}

long RmcNetworkUrcHandler::getTzOffset(const char *tzStr) {
    long tz;

    if (tzStr == NULL)
        return -1;

    tz = atol(tzStr);

    if (tz < -48 || tz > 56)
        return -1;

    return (long)(tz * 15);
}

int RmcNetworkUrcHandler::localToGmt(struct tm *gmt, const struct tm *local, long offset) {
    memcpy (gmt, local, sizeof(*local));
    if (offset == 0) return 0;

    int dHour = 0;
    int dDay = 0;
    int dMonth = 0;
    int dYear = 0;

    int min, hour;

    for (min = gmt->tm_min - offset; min < 0 || min > 59; ) {
        if (min < 0) {
            min += 60;
            dHour++;
        } else {
            min -= 60;
            dHour--;
        }
    }
    gmt->tm_min = min;
    if (dHour == 0) return 0;

    for (hour = gmt->tm_hour - dHour; hour < 0 || hour > 23; ) {
        if (hour < 0) {
            hour += 24;
            dDay++;
        } else {
            hour -= 24;
            dDay--;
        }
    }
    gmt->tm_hour = hour;
    if (dDay == 0) return 0;
    if (dDay < -1 || dDay > 1) return -1;

    const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day = gmt->tm_mday - dDay;
    if (day < 1) {
        int monIndex = gmt->tm_mon - 1 < 0 ? 11 : gmt->tm_mon-1;
        day += daysInMonth[monIndex];
        if (monIndex == 1 && leapYear(gmt->tm_year+1900))
            day++;
        dMonth++;
    } else if (gmt->tm_mon == 1 && leapYear(gmt->tm_year+1900)
        && day > daysInMonth[gmt->tm_mon]+1) {
        day -= daysInMonth[gmt->tm_mon]+1;
        dMonth--;
    } else if (day > daysInMonth[gmt->tm_mon]) {
        day -= daysInMonth[gmt->tm_mon];
        dMonth--;
    }
    gmt->tm_mday = day;
    if (dMonth == 0) return 0;

    int month = gmt->tm_mon - dMonth;
    if (month < 0) {
        month += 12;
        dYear++;
    } else if (month > 11) {
        month -= 12;
        dYear--;
    }
    gmt->tm_mon = month;
    if (dYear == 0) return 0;

    gmt->tm_year -= dYear;

    return 0;
}

bool RmcNetworkUrcHandler::leapYear(int year) {
    return ((year % 4) == 0 && (year % 100) != 0)
            || ((year % 400) == 0 && (year % 3200) != 0);
}

void RmcNetworkUrcHandler::handleChInfoChanged(const sp<RfxMclMessage>& msg) {
    // +ECHINFO: <rat>,<band>,<channel>[,<is_endc>]
    int err;
    int data[3] = {0};
    int is_endc = 0;
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    data[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    data[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    data[2] = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (data[0] == 128 && line->atTokHasmore()) {
        is_endc = line->atTokNextint(&err);
//        if (is_endc) scgBand[m_slot_id] = data[1];
    }

    logD(LOG_TAG, "handleChInfoChanged, rat=%d, band=%d, channel=%d, is_endc=%d", data[0], data[1], data[2], is_endc);
    if (!is_endc) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_BAND_INFO_IND, m_slot_id, RfxIntsData(data, 3));
        responseToTelCore(urc);
    }

error:
    return;
}

void RmcNetworkUrcHandler::handleNrCaBandChanged(const sp<RfxMclMessage>& msg) {
    // +ENRCABAND:<is_endc>,<band_1>[,<band_2>]
    int err;
    int is_endc = 0, band_1 = 0, band_2 = 0;

    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();
    int data[3] = {0};
    line->atTokStart(&err);
    if (err < 0) goto error;

    is_endc = line->atTokNextint(&err);
    if (err < 0) goto error;

    band_1 = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (line->atTokHasmore()) {
        band_2 = line->atTokNextint(&err);
        if (err < 0) goto error;
    }
    pthread_mutex_lock(&s_nrCaMutex[m_slot_id]);
    logD(LOG_TAG, "%s, old[%d, %d, %d], new[%d, %d, %d]", __FUNCTION__,
        nr_ca_cache[m_slot_id]->is_endc, nr_ca_cache[m_slot_id]->band_1, nr_ca_cache[m_slot_id]->band_2,
        is_endc, band_1, band_2);
    // report nr ca changed, for band_1 change, +EGREG will send this urc
    if (nr_ca_cache[m_slot_id]->band_2 != band_2) {
        isNeedNotifyStateChanged();
    }
    // update cache
    nr_ca_cache[m_slot_id]->is_endc = is_endc;
    nr_ca_cache[m_slot_id]->band_1 = band_1;
    nr_ca_cache[m_slot_id]->band_2 = band_2;
    pthread_mutex_unlock(&s_nrCaMutex[m_slot_id]);

    if (urc_enrcaband_enable) {
        data[0] = is_endc;
        data[1] = band_1;
        data[2] = band_2;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_CA_BAND_IND, m_slot_id, RfxIntsData(data, 3));
        responseToTelCore(urc);
    }
error:
    return;
}

void RmcNetworkUrcHandler::handleIWlanRegStateChanged(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int stat[4] = { 0 };
    int status = 0;
    int prevStatus = s_iwlan_reg_status[m_slot_id];
    sp<RfxMclMessage> urc = NULL;
    RfxAtLine *line = msg->getRawUrc();
    line->atTokStart(&err);
    if (err < 0) goto error;

    status = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (status == 0 || status == 1) {
        pthread_mutex_lock(&s_iwlanRegStatusMutex[m_slot_id]);
        s_iwlan_reg_status[m_slot_id] = status;
        pthread_mutex_unlock(&s_iwlanRegStatusMutex[m_slot_id]);
    } else {
        goto error;
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_IWLAN_REGISTRATION_STATE, m_slot_id,
                                   RfxIntsData(&status, 1));
    responseToTelCore(urc);
    logD(LOG_TAG, "[%s] status:%d", __FUNCTION__, status);
    stat[0] = convertRegState(urc_data_reg_state_cache[m_slot_id]->registration_state, false);
    stat[1] = convertPSNetworkType(urc_data_reg_state_cache[m_slot_id]->radio_technology);
    stat[2] = combineWfcEgregState();
    stat[3] = (status == 1) ? RADIO_TECH_IWLAN : convertPSNetworkType(ril_data_urc_rat);
    sendEvent(RFX_MSG_EVENT_PS_NETWORK_STATE, RfxIntsData(stat, 4),
        RIL_CMD_PROXY_3, m_slot_id);

    if (s_iwlan_in_legacy_mode && (prevStatus != status)) {
        setMSimProperty(m_slot_id, (char*)PROPERTY_WFC_STATE,
                        ((status == 1) ? (char*)"1" : (char*)"0"));
        isNeedNotifyStateChanged();
    }

    return;

error:
    logE(LOG_TAG, "[%s] URC error", __FUNCTION__);
}

void RmcNetworkUrcHandler::handle5GUWChanged(const sp<RfxMclMessage>& msg) {
    int err = 0, param[4] = {0};
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();
    logD(LOG_TAG, "handle5GUWChanged +++");

    // +E5GUW : <display_5guw>,<on_n77_band>,<on_fr2_band>,<5guw_allowed>

    line->atTokStart(&err);

    param[0] = line->atTokNextint(&err);
    if (err < 0) goto error;
    param[1] = line->atTokNextint(&err);
    if (err < 0) goto error;
    param[2] = line->atTokNextint(&err);
    if (err < 0) goto error;
    param[3] = line->atTokNextint(&err);
    if (err < 0) goto error;

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_5GUW_INFO,
                m_slot_id, RfxIntsData(param, 4));
    responseToTelCore(urc);
    return;
error:
    logE(LOG_TAG, "There is something wrong in %s", __FUNCTION__);
}

void RmcNetworkUrcHandler::handle5GUCChanged(const sp<RfxMclMessage>& msg) {
    int err = 0, param[3] = {0};
    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();
    logD(LOG_TAG, "handle5GUCChanged +++");

    // +E5GUC : <display_5guc>,<on_fr1UC_band>,<on_fr2_band>

    line->atTokStart(&err);

    param[0] = line->atTokNextint(&err);
    if (err < 0) goto error;
    param[1] = line->atTokNextint(&err);
    if (err < 0) goto error;
    param[2] = line->atTokNextint(&err);
    if (err < 0) goto error;

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_5GUW_INFO,
                m_slot_id, RfxIntsData(param, 3));
    responseToTelCore(urc);
    return;
error:
    logE(LOG_TAG, "There is something wrong in %s", __FUNCTION__);
}

void RmcNetworkUrcHandler::handleNetworkScanInd(const sp<RfxMclMessage>& msg) {
   /*
    +ENWSCN: <scan_status>,<num_record>
    [,<rat>,<xarfcn>,<pcid>,"<plmn_id>","<cell_id>","<lac_or_tac>",<plmn_status>,<registered>,<srv_cell_status>,<sig_1>,<sig_2>,<sig_3>,<sig_4>,<sig_5>][,...]
    */
    bool isSuccess = false;
    int err = 0;
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
    char *tmp = NULL;

    sp<RfxMclMessage> urc;
    RfxAtLine *line = msg->getRawUrc();

    if (!isNetworkScanFullSupport()) {
        return;
    }

    line->atTokStart(&err);
    if (err < 0) {
        logE(LOG_TAG, "handleNetworkScanInd format error");
        return;
    }

    // <scan_status>
    scan_status = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "handleNetworkScanInd can't get scan_status");
        return;
    }

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
    num_record = line->atTokNextint(&err);
    if (err < 0) goto error;

    result->network_infos = (RIL_CellInfo_v12*) calloc(num_record, sizeof(RIL_CellInfo_v12));
    if (result->network_infos == NULL) goto error;
    memset(result->network_infos, 0, num_record*sizeof(RIL_CellInfo_v12));

    /* example
    */
    for (int i = 0; i < num_record ; i++) {
        // clear the tmp value for this round.
        memset(mccmnc, 0, 8);
        mcc = 0;
        mnc = 0;
        len = 0;
        mnc_len = 0;
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);

        /* <rat> 1 - GSM; 2 - UMTS; 4 - LTE; 128 - NR */
        rat = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <xarfcn> ARFCN for 2G; UARFCN for 3G; EARFCN for 4G; NR-ARFCN for 5G */
        xarfcn = line->atTokNextint(&err);
        if (err < 0) xarfcn = INT_MAX;

        /* <pcid> */
        pcid = line->atTokNextint(&err);
        if (err < 0) pcid = INT_MAX;

        /* <plmn_id> */
        numeric_oper = line->atTokNextstr(&err);
        if (err < 0) goto error;

        /* <cell_id> */
        tmp = line->atTokNextstr(&err);
        if (err < 0) cellid = INT_MAX;
        else cellid = strtoull(tmp, NULL, 16);

        /* <lac_or_tac> */
        lac_tac = (unsigned int) line->atTokNexthexint(&err);
        if (err < 0) lac_tac = INT_MAX;

        /* <plmn_status> 0 - Unknown; 1 - Available; 2 - Current; 3 - Forbidden */
        plmn_status = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <registered> 0 - not registered; 1 - registered */
        registered = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <srv_cell_status> 0 - None; 1 - Primary Serving Cell; 2 - Secondary serving cell */
        srv_cell_status = line->atTokNextint(&err);
        if (err < 0) goto error;

        /* <sig1>
         * For GSM - (0-63, 99) Based on 27.007 8.69
         * For WCDMA - (0-31, 99) Based on 27.007 8.5
         * For TDSCDMA - (25 - 120) based on (dbm x -1)
         * For LTE - (0-31, 99) Based on 27.007 8.5
         * For NR - (0-255) Based on 27.007 8.69 <ss_rsrq>
         */
        sig1 = line->atTokNextint(&err);
        if (err < 0) sig1 = INT_MAX;

        /* <sig2>
         * For GSM/WCDMA/LTE - RSSI in qdbM
         * For TDSCDMA - RSCP in qbdm
         * For NR - (0-255) Based on 27.007 8.69 <ss_rsrp>
         */
        sig2 = line->atTokNextint(&err);
        if (err < 0) sig2 = INT_MAX;

        /* <sig3>
         * RSCP in qdbM for WCDMA
         * RSRP in qdbM for LTE
         * SS-RSRQ in qdb for NR
         */
        sig3 = line->atTokNextint(&err);
        if (err < 0) sig3 = INT_MAX;

        /* <sig4>
         * ECNO in qdbM for WCDMA
         * RSRQ in qdbM for LTE
         * SS-RSRP in qdbm for NR
         */
        sig4 = line->atTokNextint(&err);
        if (err < 0) sig4 = INT_MAX;

        /* <sig5>
         * RSSNR in qdbM for LTE
         * SS-SINR in qdb for NR
         */
        sig5 = line->atTokNextint(&err);
        if (err < 0) sig5 = INT_MAX;

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

        if (rat == 128 && lac_tac == 0xFFFFFE) {
            logD(LOG_TAG, "Skip this NSA's NR cell");
            continue;
        }

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
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH);
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
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH);
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
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH);
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
            pthread_mutex_lock(&mdEgregMutex[m_slot_id]);
            if (result->network_infos[index].registered == 1 &&
                isInService(mdEgreg[m_slot_id]->stat) &&
                mdEgreg[m_slot_id]->dcnr_restricted == 0 &&
                mdEgreg[m_slot_id]->endc_sib == 2) {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 1;
            } else {
                result->network_infos[index].CellInfo.lte.cellConfig.isEndcAvailable = 0;
            }
            pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);
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
            getOperatorNamesFromNumericCode(
                    numeric_oper, lac_tac, longname, shortname, MAX_OPER_NAME_LENGTH, 11);
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.long_name),
                    "%s", longname);
            asprintf(&(result->network_infos[index].CellInfo.nr.cellidentity.operName.short_name),
                    "%s", shortname);
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrp = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiRsrq = INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiSinr =  INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrp =
                    sig3 == INT_MAX ? INT_MAX : (sig3>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.ssRsrq =
                    sig4 == INT_MAX ? INT_MAX : (sig4>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.ssSinr =
                    sig5 == INT_MAX ? INT_MAX : (sig5>>2)*-1;
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiTableIndex =  INT_MAX;
            result->network_infos[index].CellInfo.nr.signalStrength.csiCqiReportCount = 0;
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
        sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING, false);
    }

    RIL_NetworkScanResult * pData = (RIL_NetworkScanResult *) result;
    cleanNetworkScanResultOperator(result);
    if (pData->network_infos != NULL) free(pData->network_infos);
    free(pData);

    // If failure happened but scan_status is COMPLETE, we need send complete status only to AP
    if (isSuccess != true && scan_status == 1) {
        RIL_NetworkScanResult* resp = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
        if (resp == NULL) {
            logE(LOG_TAG, "handleNetworkScanInd resp==null");
            mPlmnListAbort = 0;
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
    mPlmnListAbort = 0; /* always clear here to prevent race condition scenario */
}

void RmcNetworkUrcHandler::handleNrSysInfoChanged(const sp<RfxMclMessage>& msg) {
    int err = 0;
    int nrSbspKHz = 0;
    int info[2] = { 0 };
    sp<RfxMclMessage> urc = NULL;

    RfxAtLine *line = msg->getRawUrc();
    line->atTokStart(&err);
    if (err < 0) goto error;

    nrSbspKHz = line->atTokNextint(&err);
    if (err < 0) goto error;

    pthread_mutex_lock(&nrSysInfoMutex[m_slot_id]);
    logD(LOG_TAG, "[%s] current NR sys info:%d, %d", __FUNCTION__,
        nrSysInfo[m_slot_id]->upperlayerIndR15Available, nrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz);
    if (nrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz != nrSbspKHz) {
        nrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz = nrSbspKHz;
        info[0] = nrSysInfo[m_slot_id]->upperlayerIndR15Available;
        info[1] = nrSysInfo[m_slot_id]->nrSubcarrierSpacingKHz;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_SYS_INFO, m_slot_id,
                                   RfxIntsData(info, 2));
        responseToTelCore(urc);
    }
    pthread_mutex_unlock(&nrSysInfoMutex[m_slot_id]);

    return;

error:
    logE(LOG_TAG, "[%s] URC error", __FUNCTION__);
}

void RmcNetworkUrcHandler::handlePhysicalChannelConfig(const sp<RfxMclMessage>& msg) {
    // Urc cant be used to update phy chl info to ui because of ps reg info is unknow when the urc arrive
    // Urc must be used because there is no chance to trigger query when the bandwith/pci change
    int err = 0;
    bool hasChanged = false;
    RIL_PhysicalChannelConfig* pCache = sPhysicalConfigUrcCache[m_slot_id];
    RIL_PhysicalChannelConfig* pUrcPhyChlInfo;
    int count = 0;
    bool isEndUrc = false;

    pthread_mutex_lock(&sUrcPhysicalConfigMutex[m_slot_id]);
    /*+EPHYCONFIGN: <num_of_cells>
    +EPHYCONFIG: <rat>, <dl_ch_num>, <ul_ch_num>, <dl_cell_bw>, <ul_cell_bw>,<band>, <pci>,<pcell or scell>
    
    +EPHYCONFIGN: 0*/

    RfxAtLine *line = msg->getRawUrc();
    logV(LOG_TAG, "[%s] entry line: %s", __FUNCTION__, line->getLine());

    if (strStartsWith(line->getLine(), "+EPHYCONFIGN")) {
        line->atTokStart(&err);
        if (err < 0) goto error;
        count = line->atTokNextint(&err);
        if (err < 0) goto error;

        if (count > 0) {
            //first urc coming
            if (mPccUrcCache) {
                logE(LOG_TAG, "[%s] first urc coming but phyChlCfgUrcCache not null", __FUNCTION__);
                goto error;
            }

            mPccUrcCache = (PHY_CHL_CFG_URC_CACHE*) calloc(1, sizeof(PHY_CHL_CFG_URC_CACHE));
            if (mPccUrcCache == NULL) {
                logE(LOG_TAG, "[%s] phyChlCfgUrcCache = null", __FUNCTION__);
                goto error;
            }

            mPccUrcCache->phyChlCfgInfo = (RIL_PhysicalChannelConfig*) calloc(count, sizeof(RIL_PhysicalChannelConfig));
            if (mPccUrcCache->phyChlCfgInfo == NULL) goto error;

            mPccUrcCache->totalUrc = count;
            mPccUrcCache->currUrcCount = 0;
            logV(LOG_TAG, "[%s] first URC coming, count: %d ", __FUNCTION__, count);
        } else {
            isEndUrc = true;
            logV(LOG_TAG, "[%s] URC end ", __FUNCTION__);
        }
    } else if (mPccUrcCache
            && (mPccUrcCache->currUrcCount < mPccUrcCache->totalUrc)) {
        pUrcPhyChlInfo = mPccUrcCache->phyChlCfgInfo;
        if (parsePhyChlCfg(RADIO_TECH_UNKNOWN, line, &pUrcPhyChlInfo[mPccUrcCache->currUrcCount])) {
            (mPccUrcCache->currUrcCount)++;
        }
    } else {
        logE(LOG_TAG, "[%s]  unexpected urc", __FUNCTION__);
    }

    //all urcs have received
    if (isEndUrc) {
        if (!mPccUrcCache) {//new urc with count 0
            if (sPhysicalConfigUrcCount[m_slot_id] != 0) {
                hasChanged = true;
            }
        } else {
            pUrcPhyChlInfo = mPccUrcCache->phyChlCfgInfo;
            if (sPhysicalConfigUrcCount[m_slot_id] != mPccUrcCache->currUrcCount) {
                hasChanged = true;
            }

            if (!hasChanged && sPhysicalConfigUrcCount[m_slot_id] > 0 && mPccUrcCache->currUrcCount > 0) {
                for (int i = 0; i < sPhysicalConfigUrcCount[m_slot_id]; i++) {
                    if (pCache[i].rat != pUrcPhyChlInfo[i].rat
                        || pCache[i].status != pUrcPhyChlInfo[i].status
                        || pCache[i].cellBandwidthDownlink != pUrcPhyChlInfo[i].cellBandwidthDownlink
                        || pCache[i].cellBandwidthUplink != pUrcPhyChlInfo[i].cellBandwidthUplink
                        || pCache[i].downlinkChannelNumber != pUrcPhyChlInfo[i].downlinkChannelNumber
                        || pCache[i].uplinkChannelNumber != pUrcPhyChlInfo[i].uplinkChannelNumber
                        || pCache[i].band != pUrcPhyChlInfo[i].band
                        || pCache[i].physicalCellId != pUrcPhyChlInfo[i].physicalCellId) {
                        hasChanged = true;
                        break;
                    }
                }
            }
        }

        // query physical channel config if it has changed.
        if (hasChanged) {
            if (!mPccUrcCache) {
                sPhysicalConfigUrcCount[m_slot_id] = 0;
                memset(pCache, 0, MAX_PHY_CHL_CFG_COUNT*sizeof(RIL_PhysicalChannelConfig));
            } else {
                memcpy(pCache, pUrcPhyChlInfo, mPccUrcCache->currUrcCount*sizeof(RIL_PhysicalChannelConfig));
                sPhysicalConfigUrcCount[m_slot_id] = mPccUrcCache->currUrcCount;
            }
            sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(), RIL_CMD_PROXY_3, m_slot_id);
        }

        logD(LOG_TAG, "[%s] urc data hasChanged:%d, Urc Cache count: %d",
                __FUNCTION__,  (hasChanged? 1: 0), (mPccUrcCache? mPccUrcCache->currUrcCount: 0));

        if (mPccUrcCache != NULL && mPccUrcCache->phyChlCfgInfo != NULL)
            free(mPccUrcCache->phyChlCfgInfo);
        if (mPccUrcCache != NULL) free(mPccUrcCache);
        mPccUrcCache = NULL;
    }

    pthread_mutex_unlock(&sUrcPhysicalConfigMutex[m_slot_id]);
    return;

error:
    logE(LOG_TAG, "[%s] URC error", __FUNCTION__);
    if (mPccUrcCache != NULL && mPccUrcCache->phyChlCfgInfo != NULL)
        free(mPccUrcCache->phyChlCfgInfo);
    if (mPccUrcCache != NULL) free(mPccUrcCache);
    mPccUrcCache = NULL;
    pthread_mutex_unlock(&sUrcPhysicalConfigMutex[m_slot_id]);
}

