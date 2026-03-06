// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
#include "RpNwStateController.h"
#include "RpNwPhoneTypeUpdater.h"
#include "RpNwDefs.h"
#include "power/RadioConstants.h"
#include <cutils/jstring.h>
#include "RfxMainThread.h"
#include "sim/RpSimControllerBase.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <cerrno>
#include "RpDataUtils.h"
#include "MbrainClient.h"
#if defined(TELE_FWK)
extern int rfx_get_data_allowed_slotid(void);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
#if defined(MODE_DSSS)
extern int rfx_get_disabled_sim(void);
#endif
#endif

RFX_IMPLEMENT_CLASS("RpNwStateController", RpNwStateController, RfxController);

RpNwStateController::RpNwStateController() {
    logD(RP_NW_STATE_TAG, " Constructor RpNwStateController");
    mVoiceRegState = 0;
    mDataRegState = 0;
    mVoiceRadioTech = 0;
    mDataRadioTech = 0;
    mDontPollSignalStrength = false;
}

RpNwStateController::~RpNwStateController() {
}

void RpNwStateController::onInit() {
    RfxController::onInit(); // Required: invoke super class implementation

    logD(RP_NW_STATE_TAG, " onInit() SIM%d", m_slot_id);

    const int request_id_list[] = {
        RIL_REQUEST_VOICE_REGISTRATION_STATE,
        RIL_REQUEST_DATA_REGISTRATION_STATE,
#if  defined(TELE_FWK)
        RIL_REQUEST_GET_RADIO_CAPABILITY,
        RIL_REQUEST_SET_RADIO_CAPABILITY,
#endif
        RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE,
        RIL_REQUEST_LOCAL_REG_STATE_TYPE,
        RIL_REQUEST_LOCAL_GET_OPERATOR_CODE,
        RIL_REQUEST_LOCAL_SIGNAL_STRENGTH,
        RIL_REQUEST_SIGNAL_STRENGTH,
     /* RIL_REQUEST_OPERATOR,
        RIL_REQUEST_GET_CELL_INFO_LIST*/
    };

    const int urc_id_list[] = {
        RIL_UNSOL_SIGNAL_STRENGTH,
        RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED,
        RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED,
        /*RIL_UNSOL_CDMA_OTA_PROVISION_STATUS,*/
        RIL_UNSOL_RESTRICTED_STATE_CHANGED,
#if (defined(MODE_DSSS)&& defined(TELE_FWK) )
        RIL_UNSOL_RIL_CONNECTED,
#endif
    };

    // register request & URC id list
    registerToHandleRequest(request_id_list, (sizeof(request_id_list)/sizeof(int)));
    registerToHandleUrc(urc_id_list, (sizeof(urc_id_list)/sizeof(int)));

    resetVoiceRegStateCache(&gsm_voice_reg_state_cache);
    resetVoiceRegStateCache(&cdma_voice_reg_state_cache);
    resetDataRegStateCache(&gsm_data_reg_state_cache);
    resetDataRegStateCache(&cdma_data_reg_state_cache);
    gsm_voice_reg_state_cache.cacheType = RADIO_TECH_GROUP_GSM;
    cdma_voice_reg_state_cache.cacheType = RADIO_TECH_GROUP_C2K;
    gsm_data_reg_state_cache.cacheType = RADIO_TECH_GROUP_GSM;
    cdma_data_reg_state_cache.cacheType = RADIO_TECH_GROUP_C2K;
    resetSignalStrengthCache(&signal_strength_cache);
    resetOperatorCache(&gsm_operator_cache);
    resetOperatorCache(&cdma_operator_cache);

    mVoiceRegState = 0;
    mDataRegState = 0;
    mVoiceRadioTech = 0;
    mDataRadioTech = 0;
    mVoiceRadioSys = -1;
    mDataRadioSys = -1;
    mSignalStrength.init();

    getStatusManager()->setServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
            RfxNwServiceState(mVoiceRegState, mDataRegState, mVoiceRadioTech, mDataRadioTech));
#if (!defined(MODE_DSSS)&& defined(TELE_FWK) )
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_GSM_OPERATOR_NUMERIC,
        RfxStatusChangeCallback(this, &RpNwStateController::onSimCardInsertStatusUpdate));
#endif

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SERVICE_STATE,
            RfxStatusChangeCallback(this, &RpNwStateController::onServiceStateChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallback(this, &RpNwStateController::onRadioStateChanged));
    RpNwPhoneTypeUpdater *phoneTypeUpdater;

    RFX_OBJ_CREATE(phoneTypeUpdater, RpNwPhoneTypeUpdater, this);
}

void RpNwStateController::handleRegStateType(const sp<RfxMessage>& riljReq) {
    RfxNwServiceState ss = getStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE);
    int voice_reg = ss.getRilVoiceRegState();
    int data_reg = ss.getRilDataRegState();
    int radio_status = getStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_UNAVAILABLE);
    logD(RP_NW_STATE_TAG, "RpNwStateController::handleRegStateType, voice_reg=%d, data_reg=%d, radio_status=%d", voice_reg, data_reg, radio_status);
    RIL_Errno e = RIL_Errno::RIL_E_SUCCESS;
    sp<RfxMessage> response = RfxMessage::obtainResponse(e, riljReq);
    Parcel* responseParcel = response->getParcel();
    responseParcel->setDataPosition(sizeof(int32_t) * 3);
    android::status_t status = responseParcel->writeInt32(voice_reg);
    if (status != android::NO_ERROR) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType voice_reg error");
        e = RIL_Errno::RIL_E_GENERIC_FAILURE;
        goto invalid;
    }
    status = responseParcel->writeInt32(data_reg);
    if (status != android::NO_ERROR) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType data_reg error");
        e = RIL_Errno::RIL_E_GENERIC_FAILURE;
    }
    status = responseParcel->writeInt32(radio_status);
    if (status != android::NO_ERROR) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType radio_status error");
        e = RIL_Errno::RIL_E_GENERIC_FAILURE;
    }
invalid:
    responseToRilj(response);
}

void RpNwStateController::handleRadioAccessType(const sp<RfxMessage>& riljReq) {
    int radioTech = RIL_RadioTechnology::RADIO_TECH_UNKNOWN;
    RfxNwServiceState ss = getStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE);
    int VoiceRadioTech = ss.getRilVoiceRadioTech();
    int DataRadioTech = ss.getRilDataRadioTech();
    logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType, VoiceRadioTech=%d, DataRadioTech=%d", VoiceRadioTech, DataRadioTech);

    if (VoiceRadioTech != 0) {
        radioTech = VoiceRadioTech;
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType, use VoiceRadioTech with result: %d", radioTech);
    } else if (DataRadioTech != 0) {
        radioTech = DataRadioTech;
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType, use DataRadioTech with result: %d", radioTech);
    } else {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType, unknow radio tech with result: %d", radioTech);
    }
    RIL_Errno e = RIL_Errno::RIL_E_SUCCESS;
    sp<RfxMessage> response = RfxMessage::obtainResponse(e, riljReq);
    Parcel* responseParcel = response->getParcel();
    responseParcel->setDataPosition(sizeof(int32_t) * 3);
    android::status_t  status = responseParcel->writeInt32(radioTech);
    if (status != android::NO_ERROR) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleRadioAccessType error");
        e = RIL_Errno::RIL_E_GENERIC_FAILURE;
    }
    responseToRilj(response);
}

void RpNwStateController::handleGetOpeCode(const sp<RfxMessage>& riljReq) {
    String8 mccmnc = getStatusManager()->getString8Value(RFX_STATUS_KEY_GSM_OPERATOR_NUMERIC);
    logD(RP_NW_STATE_TAG, "RpNwStateController::handleGetOpeCode, mccmnc: %s", mccmnc.c_str());
    RIL_Errno e = RIL_Errno::RIL_E_SUCCESS;
    std::vector<std::string> MCCMNC_TABLE_TYPE_CU{"46001", "46006", "46009", "45407"};
    std::vector<std::string> MCCMNC_TABLE_TYPE_CMCC { "46000", "46002", "46007", "46008", "46013", "45412", "45413", "45430", "00101", "00211",
            "00321", "00431", "00541", "00651", "00761", "00871", "00902", "01012", "01122", "01232", "46004", "46602", "50270" };
    int op = -1;
    std::string ind(mccmnc.c_str());
    if (std::find(std::begin(MCCMNC_TABLE_TYPE_CU), std::end(MCCMNC_TABLE_TYPE_CU), ind) != std::end(MCCMNC_TABLE_TYPE_CU)){
        op = 1;//CU
    } else if(std::find(std::begin(MCCMNC_TABLE_TYPE_CMCC), std::end(MCCMNC_TABLE_TYPE_CMCC), ind) != std::end(MCCMNC_TABLE_TYPE_CMCC)) {
        op = 0; //CMMC
    }
    if (op == -1) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleGetOpeCode don't support check");
        e = RIL_Errno::RIL_E_GENERIC_FAILURE;
    }
    sp<RfxMessage> response = RfxMessage::obtainResponse(e, riljReq);
    Parcel* responseParcel = response->getParcel();
    responseParcel->setDataPosition(sizeof(int32_t) * 3);
    android::status_t  status = responseParcel->writeInt32(op);
    if (status != android::NO_ERROR) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleGetOpeCode error");
        return;
    }
    responseToRilj(response);
}

bool RpNwStateController::checkSimAndRadio() {
    int radioState = getStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_UNAVAILABLE);
    int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_NOT_READY);
    logD(RP_NW_STATE_TAG, "radioState = %d simState = %d ", radioState, simState);
    if (radioState == RADIO_STATE_ON && simState == RFX_SIM_STATE_READY) {
        return true;
    }
    return false;
}

void RpNwStateController::handleGetSignalStrength(const sp<RfxMessage>& riljReq) {
    int sig_level = mSignalStrength.getLevel();
    int csq = mSignalStrength.getAsuLevel();
    RIL_Errno e = RIL_Errno::RIL_E_SUCCESS;

    sp<RfxMessage> response = RfxMessage::obtainResponse(e, riljReq);
    Parcel* responseParcel = response->getParcel();
    responseParcel->setDataPosition(sizeof(int32_t) * 3);
    if(!checkSimAndRadio()) {
        sig_level = 0;
    }
    android::status_t  status = responseParcel->writeInt32(sig_level);
    if (status != android::NO_ERROR) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleGetOpeCode error");
        return;
    }
    status = responseParcel->writeInt32(csq);
    if (status != android::NO_ERROR) {
        logD(RP_NW_STATE_TAG, "RpNwStateController::handleGetOpeCode error");
        return;
    }
    responseToRilj(response);
}

bool RpNwStateController::onHandleRequest(const sp<RfxMessage>& riljReq) {
    int reqId = riljReq->getId();
    int slotId = riljReq->getSlotId();

    logD(RP_NW_STATE_TAG, " onHandleRequest() SIM%d REQ=%s", slotId, requestToString(reqId));
    switch(reqId){
    case RIL_REQUEST_LOCAL_REG_STATE_TYPE:
    {
        handleRegStateType(riljReq);
        break;
    }
    case RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE:
    {
        handleRadioAccessType(riljReq);
        break;
    }
    case RIL_REQUEST_LOCAL_GET_OPERATOR_CODE:
    {
        handleGetOpeCode(riljReq);
        break;
    }
    case RIL_REQUEST_LOCAL_SIGNAL_STRENGTH:
    {
        handleGetSignalStrength(riljReq);
        break;
    }
    default:
        requestToRild(riljReq);
        break;
    }

    return true;
}

bool RpNwStateController::onHandleUrc(const sp<RfxMessage>& urc) {
    int urcId = urc->getId();
    int slotId = urc->getSlotId();
    Parcel *p;
    sp<RfxMessage> urcToRilj = urc;
    bool isNeedResponseUrc = true;
    sp<RfxMessage> gsmReq;

    //MT2635:gsm case only support GSM Only
    logD(RP_NW_STATE_TAG, " onHandleUrc() GSM SIM%d URC=%s", slotId,
            urcToString(urcId));
    if (urc->getError() == RIL_E_SUCCESS) {
        switch (urcId) {

#if (defined(MODE_DSSS)&& defined(TELE_FWK))
        case RIL_UNSOL_RIL_CONNECTED:
        {
            int radioState = getStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_UNAVAILABLE);
            if (radioState == RADIO_STATE_ON) {
                logD(RP_NW_STATE_TAG,"get data/voice reg state and signalstrength when RIL_UNSOL_RIL_CONNECTED");

                //query the voice/data reg state and signal strength
                gsmReq = RfxMessage::obtainRequest(m_slot_id,
                        RADIO_TECH_GROUP_GSM,
                        RIL_REQUEST_VOICE_REGISTRATION_STATE);
                RfxMainThread::enqueueMessage(gsmReq);

                gsmReq = RfxMessage::obtainRequest(m_slot_id,
                        RADIO_TECH_GROUP_GSM,
                        RIL_REQUEST_DATA_REGISTRATION_STATE);
                RfxMainThread::enqueueMessage(gsmReq);

                gsmReq = RfxMessage::obtainRequest(m_slot_id,
                        RADIO_TECH_GROUP_GSM,
                        RIL_REQUEST_SIGNAL_STRENGTH);
                RfxMainThread::enqueueMessage(gsmReq);
            }
#if 0
            else {
                logD(RP_NW_STATE_TAG,"radio on when RIL_UNSOL_RIL_CONNECTED and radio is not on");
                sp<RfxMessage> newMsg = RfxMessage::obtainRequest(m_slot_id, RADIO_TECH_GROUP_GSM, RIL_REQUEST_RADIO_POWER);
                Parcel* newParcel = newMsg->getParcel();
                newParcel->writeInt32(1);
                newParcel->writeInt32(1);
                RfxMainThread::enqueueMessage(newMsg);
            }
#endif
            break;
        }
#endif
        case RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED:
            // gsmReq = RfxMessage::obtainRequest(m_slot_id,
            //         RADIO_TECH_GROUP_GSM,
            //         RIL_REQUEST_VOICE_REGISTRATION_STATE);
            // RfxMainThread::enqueueMessage(gsmReq);
            updateVoiceRegStateCache(urc);
            combineVoiceRegState(urc);
            break;
        case RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED:
            // gsmReq = RfxMessage::obtainRequest(m_slot_id,
            //         RADIO_TECH_GROUP_GSM,
            //         RIL_REQUEST_DATA_REGISTRATION_STATE);
            // RfxMainThread::enqueueMessage(gsmReq);
            updateDataRegStateCache(urc);
            combineDataRegState(urc);
            break;
        case RIL_UNSOL_RESTRICTED_STATE_CHANGED:
        {
            int numInts = 0;
            int response = 0;
            p = urc->getParcel();
            p->readInt32(&numInts);
            if (numInts < 1) {
                logD(RP_NW_STATE_TAG, " URC error, no restricted data");
                break;
            }
            p->readInt32(&response);
            bool restricted = (response & RIL_RESTRICTED_STATE_PS_ALL) != 0;
            if (restricted
                    != getStatusManager()->getBoolValue(
                            RFX_STATUS_KEY_PS_RESTRICT_STATE, false)) {
                getStatusManager()->setBoolValue(
                        RFX_STATUS_KEY_PS_RESTRICT_STATE, restricted, true);
            }
            break;
        }
        case RIL_UNSOL_SIGNAL_STRENGTH:
        {
            onSignalStrengthResult(urcToRilj);
            mDontPollSignalStrength = true;
            break;
        }
        default:
            break;
        }
    }

    if (isNeedResponseUrc) {
        responseToRilj(urcToRilj);
    }
    return true;
}

void RpNwStateController::onSignalStrengthResult(const sp<RfxMessage>& message) {
    Parcel *p;
    p = message->getParcel();
    RIL_SignalStrength_v14 *p_cur = (RIL_SignalStrength_v14*)calloc(1,
            sizeof(RIL_SignalStrength_v14));
    if(p_cur == NULL) {
        return;
    }
    int32_t lte_cqiTableIndex = 0;
    p->readInt32(&p_cur->GW_SignalStrength.signalStrength);
    p->readInt32(&p_cur->GW_SignalStrength.bitErrorRate);
    p->readInt32(&p_cur->GW_SignalStrength.timingAdvance);
    p->readInt32(&p_cur->CDMA_SignalStrength.dbm);
    p->readInt32(&p_cur->CDMA_SignalStrength.ecio);
    p->readInt32(&p_cur->EVDO_SignalStrength.dbm);
    p->readInt32(&p_cur->EVDO_SignalStrength.ecio);
    p->readInt32(&p_cur->EVDO_SignalStrength.signalNoiseRatio);
    p->readInt32(&p_cur->LTE_SignalStrength.signalStrength);
    p->readInt32(&p_cur->LTE_SignalStrength.rsrp);
    p->readInt32(&p_cur->LTE_SignalStrength.rsrq);
    p->readInt32(&p_cur->LTE_SignalStrength.rssnr);
    p->readInt32(&p_cur->LTE_SignalStrength.cqi);
    p->readInt32(&p_cur->LTE_SignalStrength.timingAdvance);
    p->readInt32(&lte_cqiTableIndex);
    p->readInt32(&p_cur->TD_SCDMA_SignalStrength.signalStrength);
    p->readInt32(&p_cur->TD_SCDMA_SignalStrength.bitErrorRate);
    p->readInt32(&p_cur->TD_SCDMA_SignalStrength.rscp);
    p->readInt32(&p_cur->WCDMA_SignalStrength.signalStrength);
    p->readInt32(&p_cur->WCDMA_SignalStrength.bitErrorRate);
    p->readInt32(&p_cur->WCDMA_SignalStrength.rscp);
    p->readInt32(&p_cur->WCDMA_SignalStrength.ecno);
    p->readInt32(&p_cur->NR_SignalStrength.ssRsrp);
    p->readInt32(&p_cur->NR_SignalStrength.ssRsrq);
    p->readInt32(&p_cur->NR_SignalStrength.ssSinr);
    p->readInt32(&p_cur->NR_SignalStrength.csiRsrp);
    p->readInt32(&p_cur->NR_SignalStrength.csiRsrq);
    p->readInt32(&p_cur->NR_SignalStrength.csiSinr);

    mSignalStrength.update(p_cur->GW_SignalStrength.signalStrength,
        p_cur->GW_SignalStrength.bitErrorRate,
        p_cur->CDMA_SignalStrength.dbm,
        p_cur->CDMA_SignalStrength.ecio,
        p_cur->EVDO_SignalStrength.dbm,
        p_cur->EVDO_SignalStrength.ecio,
        p_cur->EVDO_SignalStrength.signalNoiseRatio,
        p_cur->LTE_SignalStrength.signalStrength,
        p_cur->LTE_SignalStrength.rsrp,
        p_cur->LTE_SignalStrength.rsrq,
        p_cur->LTE_SignalStrength.rssnr,
        p_cur->LTE_SignalStrength.cqi,
        p_cur->TD_SCDMA_SignalStrength.rscp,
        p_cur->WCDMA_SignalStrength.signalStrength,
        p_cur->WCDMA_SignalStrength.rscp,
        p_cur->NR_SignalStrength.csiRsrp,
        p_cur->NR_SignalStrength.csiRsrq,
        p_cur->NR_SignalStrength.csiSinr,
        p_cur->NR_SignalStrength.ssRsrp,
        p_cur->NR_SignalStrength.ssRsrq,
        p_cur->NR_SignalStrength.ssSinr);

    if (p_cur != NULL) {
        free(p_cur);
    }
    mSignalStrength.validateInput();

    responseToRilj(message);
}

void RpNwStateController::onSimCardInsertStatusUpdate(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value)
{
    char uiccType[PROPERTY_VALUE_MAX] = {0};
    int other_sim_slot = 0; //value is 0,1
    int new_data_sim_slot=rfx_get_data_allowed_slotid(); //value is 0,1

    /*
    *  Caculate the New Data sim on which slot
    *  The last boot-up data sim inserted: keep on old
    *  The last boot-up data sim not inserted & the other sim is not inserted: keep on old
    *  The last boot-up data sim not inserted & the other sim is inserted: move to other sim
    */
    property_get(PROPERTY_RIL_UICC_TYPE[new_data_sim_slot], uiccType, "");
    logD(RP_NW_STATE_TAG, "data sim %d SimCapabilitySwitch,%s= %s",
            new_data_sim_slot, PROPERTY_RIL_UICC_TYPE[new_data_sim_slot],uiccType);
    if (strlen(uiccType) == 0)
    {

        other_sim_slot = (new_data_sim_slot==RFX_SLOT_ID_0)?RFX_SLOT_ID_1: RFX_SLOT_ID_0;
        logD(RP_NW_STATE_TAG, "SimCapabilitySwitch, Current Data SIM is not insert, check other sim");
        property_get(PROPERTY_RIL_UICC_TYPE[other_sim_slot], uiccType, "");
        logD(RP_NW_STATE_TAG, "other sim %d SimCapabilitySwitch,%s= %s",
            other_sim_slot, PROPERTY_RIL_UICC_TYPE[other_sim_slot],uiccType);
        if (strlen(uiccType) != 0) //other sim was inserted
        {
            new_data_sim_slot = other_sim_slot;
            property_set("persist.vendor.radio.data.sim", ((0==new_data_sim_slot)?"1":"2"));
            {
#define SKIP_VALUE (-2)
#define TOTAL_PARAM (5)

                int data_sim = rfx_get_data_allowed_slotid();

                logD(RP_NW_STATE_TAG, "Data SIM change send RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD data_sim=%d", data_sim);

                sp<RfxMessage> newMsg = RfxMessage::obtainRequest(0,RADIO_TECH_GROUP_GSM, RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD);
                Parcel* newParcel = newMsg->getParcel();
                size_t pos = newParcel->dataPosition();
                newParcel->writeInt32(TOTAL_PARAM);
                newParcel->writeInt32(SKIP_VALUE);
                newParcel->writeInt32(SKIP_VALUE);
                newParcel->writeInt32(data_sim);
                newParcel->writeInt32(SKIP_VALUE);
                newParcel->writeInt32(SKIP_VALUE);
                requestToRild(newMsg);

            }

        }
    }
    /*Get the current sim capability*/
    sp<RfxMessage> Req = RfxMessage::obtainRequest(m_slot_id,
                    RADIO_TECH_GROUP_GSM, RIL_REQUEST_GET_RADIO_CAPABILITY);
    logD(RP_NW_STATE_TAG, "SimCapabilitySwitch, get the radio capabilty");
    RfxMainThread::enqueueMessage(Req);
    logD(RP_NW_STATE_TAG,"SimCapabilitySwitch Current slot_id=%d, Old data SIM is %d, New Data SIM is %d, ",
        m_slot_id,rfx_get_data_allowed_slotid(), new_data_sim_slot);

}

void RpNwStateController::onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value){
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    sp<RfxMessage> gsmReq;

    RIL_RadioState radioState = (RIL_RadioState) value.asInt();
    logD(RP_NW_STATE_TAG, "radioState %d", radioState);
    notifyMbrainRadiosChanged(m_slot_id, static_cast<int>(radioState));

    if (radioState == RADIO_STATE_ON) {
        gsmReq = RfxMessage::obtainRequest(m_slot_id,
                RADIO_TECH_GROUP_GSM,
                RIL_REQUEST_VOICE_REGISTRATION_STATE);
        RfxMainThread::enqueueMessage(gsmReq);

        gsmReq = RfxMessage::obtainRequest(m_slot_id,
                RADIO_TECH_GROUP_GSM,
                RIL_REQUEST_DATA_REGISTRATION_STATE);
        RfxMainThread::enqueueMessage(gsmReq);

        gsmReq = RfxMessage::obtainRequest(m_slot_id,
                RADIO_TECH_GROUP_GSM,
                RIL_REQUEST_SIGNAL_STRENGTH);
        RfxMainThread::enqueueMessage(gsmReq);
    }else {
        mSignalStrength.init();
    }
}

void RpNwStateController::onServiceStateChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant newValue) {
    RFX_UNUSED(key);
    sp<RfxMessage> gsmReq;

    RfxNwServiceState oldSS = (RfxNwServiceState) old_value.asServiceState();
    RfxNwServiceState newSS = (RfxNwServiceState) newValue.asServiceState();

    int oldVoiceRadioTech = oldSS.getRilVoiceRadioTech();
    int newVoiceRadioTech = newSS.getRilVoiceRadioTech();

    logD(RP_NW_STATE_TAG, "onServiceStateChanged with old voiceRadioTech %d, with new voiceRadioTech:%d",
            oldVoiceRadioTech, newVoiceRadioTech);

    if ((oldVoiceRadioTech == RADIO_TECH_UNKNOWN)
        && (newVoiceRadioTech != RADIO_TECH_UNKNOWN)) {
        gsmReq = RfxMessage::obtainRequest(m_slot_id,
                RADIO_TECH_GROUP_GSM,
                RIL_REQUEST_SIGNAL_STRENGTH);
        RfxMainThread::enqueueMessage(gsmReq);
    }
}

void RpNwStateController::updateVoiceRegStateCache(const sp<RfxMessage>& msg) {
    int source = msg->getSource();
    Parcel* p = msg->getParcel();
    int pos = p->dataPosition();
    int id = msg->getId();
    logD(RP_NW_STATE_TAG, " updateVoiceRegStateCache(): src=%s, id=%d", sourceToString(source), id);
    if (source == RADIO_TECH_GROUP_GSM) {
        int32_t val = 0;
        int32_t act = 0;
        int32_t reasonForDenial = 0;
        int64_t lac = 0xffffffff;
        int64_t ci = 0x0fffffff;
        if (id == RIL_REQUEST_VOICE_REGISTRATION_STATE) {
            p->readInt32(&val);
            p->readInt32(&act);
            p->readInt32(&reasonForDenial);
        } else if (id == RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED) {
            int num = 0;
            p->readInt32(&num);
            if (num == 6) {
                std::vector<uint64_t> data(num);
                for (int i = 0; i < num; ++i) {
                    char* tmp = RpDataUtils::strdupReadString(p);
                    char* endPtr = nullptr;
                    if (tmp == nullptr) {
                        logW(RP_NW_STATE_TAG, "updateVoiceRegStateCache: failed to read string, index=%d", i);
                        data[i] = 0;
                        continue;
                    }
                    data[i] = static_cast<uint64_t>(std::strtoll(tmp, &endPtr, 10));
                    if (*endPtr != '\0') {
                        logW(RP_NW_STATE_TAG, "updateVoiceRegStateCache: failed to convert string '%s' to integer, index=%d", tmp, i);
                        data[i] = 0;
                    }
                    RpDataUtils::freeMemory(tmp);
                }
                val = static_cast<int32_t>(data[0]);
                act = static_cast<int32_t>(data[3]);
                reasonForDenial = static_cast<int32_t>(data[4]);
            }
        }
        logD(RP_NW_STATE_TAG, " updateVoiceRegStateCache(): val=%d  radio_technology=%d, reasonForDenial = %d", val, act, reasonForDenial);
        gsm_voice_reg_state_cache.register_state = val;

        if (!RfxNwServiceState::isInService(gsm_voice_reg_state_cache.register_state)) {
            gsm_voice_reg_state_cache.lac = -1;
            gsm_voice_reg_state_cache.cid = -1;
            gsm_voice_reg_state_cache.radio_technology = 0;
        } else {
            gsm_voice_reg_state_cache.radio_technology = act;
            gsm_voice_reg_state_cache.deny_reason = reasonForDenial;
        }
        notifyMbrainCsAndEctChanged(m_slot_id, gsm_voice_reg_state_cache.register_state, gsm_voice_reg_state_cache.radio_technology);
        printVoiceCache(gsm_voice_reg_state_cache);
        p->setDataPosition(pos);
    }
}

int RpNwStateController::calculateCssValue() {
    char prop[PROPERTY_VALUE_MAX] = {0};
    int ret = 0;

    property_get("ro.mtk_svlte_support", prop, "0");
    if (atoi(prop) == 1 && getDeviceMode() == NWS_MODE_CDMALTE
            && gsm_data_reg_state_cache.radio_technology == RADIO_TECH_LTE) {
        ret = 1;
    }

    return ret;
}

void RpNwStateController::updateGsmDataRegStateCache(char **&pStrings, char *&endptr) {
  if (RfxNwServiceState::isInService(gsm_data_reg_state_cache.register_state)) {
    errno = 0; // reset errno before the next conversion
    long lac = strtol(pStrings[1], &endptr, 16);
    if (*endptr != '\0' || errno == ERANGE || errno == EINVAL) {
      logE(RP_NW_STATE_TAG,
           " updateDataRegStateCache() strtol() failed for lac %s",
           pStrings[1]);
      gsm_data_reg_state_cache.lac = -1;
    } else {
      gsm_data_reg_state_cache.lac = lac;
    }

    errno = 0; // reset errno before each conversion
    long cid = strtol(pStrings[2], &endptr, 16);
    if (*endptr != '\0' || errno == ERANGE || errno == EINVAL) {
      logE(RP_NW_STATE_TAG,
           " updateDataRegStateCache() strtol() failed for cid %s",
           pStrings[2]);
      gsm_data_reg_state_cache.cid = -1;
    } else {
     gsm_data_reg_state_cache.cid = cid;
    }

    errno = 0;
    long radio_technology = strtol(pStrings[3], &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE || errno == EINVAL) {
      logE(RP_NW_STATE_TAG,
           " updateDataRegStateCache() strtol() failed for radio_technology %s",
           pStrings[3]);
     gsm_data_reg_state_cache.radio_technology = 0;
    } else {
     gsm_data_reg_state_cache.radio_technology = radio_technology;
    }

    errno = 0;
    long deny_reason = strtol(pStrings[4], &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE || errno == EINVAL) {
      logE(RP_NW_STATE_TAG,
           " updateDataRegStateCache() strtol() failed for deny_reason %s",
           pStrings[4]);
     gsm_data_reg_state_cache.deny_reason = 0;
    } else {
     gsm_data_reg_state_cache.deny_reason = deny_reason;
    }

    errno = 0;
    long max_data_call = strtol(pStrings[5], &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE || errno == EINVAL) {
      logE(RP_NW_STATE_TAG,
           " updateDataRegStateCache() strtol() failed for max_data_call %s",
           pStrings[5]);
     gsm_data_reg_state_cache.max_data_call = -1;
    } else {
     gsm_data_reg_state_cache.max_data_call = max_data_call;
    }
  } else {
    gsm_data_reg_state_cache.lac = -1;
    gsm_data_reg_state_cache.cid = -1;
    gsm_data_reg_state_cache.radio_technology = 0;
  }
}

void RpNwStateController::updateDataRegStateCache(const sp<RfxMessage>& msg) {
    int source = msg->getSource();
    Parcel* p = msg->getParcel();
    int pos = p->dataPosition();
    int id = msg->getId();
    logD(RP_NW_STATE_TAG, " updateDataRegStateCache(): src=%s, id= %d", sourceToString(source), id);
    if (source == RADIO_TECH_GROUP_GSM) {
        int32_t val = 0;
        int32_t act = 0;
        int32_t reasonForDenial = 0;
        int32_t mccmnc = 0;
        if(id == RIL_REQUEST_DATA_REGISTRATION_STATE) {
            p->readInt32(&val);
            p->readInt32(&act);
            p->readInt32(&reasonForDenial);
        } else if (id == RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED) {
            int num = 0;
            p->readInt32(&num);
            if(num == 3) {
                p->readInt32(&val);
                p->readInt32(&mccmnc);
                p->readInt32(&act);
            }
        }
        logD(RP_NW_STATE_TAG, " updateDataRegStateCache(): val=%d  radio_technology=%d, reasonForDenial = %d", val, act, reasonForDenial);
        gsm_data_reg_state_cache.register_state = val;

        if (!RfxNwServiceState::isInService(gsm_data_reg_state_cache.register_state)) {
            gsm_data_reg_state_cache.lac = -1;
            gsm_data_reg_state_cache.cid = -1;
            gsm_data_reg_state_cache.radio_technology = 0;
        } else {
            gsm_data_reg_state_cache.radio_technology = act;
            gsm_data_reg_state_cache.deny_reason = reasonForDenial;
        }
        notifyMbrainPsAndEctChanged(m_slot_id, gsm_data_reg_state_cache.register_state, gsm_data_reg_state_cache.radio_technology);
        printDataCache(gsm_data_reg_state_cache);
        p->setDataPosition(pos);
    }
}

void RpNwStateController::updateSignalStrengthCache(const sp<RfxMessage>& msg) {
    int skip;
    int source = msg->getSource();
    Parcel *p = msg->getParcel();

    logD(RP_NW_STATE_TAG, " updateSignalStrengthCache(): src=%s", sourceToString(source));
    if (source == RADIO_TECH_GROUP_GSM) {
        p->readInt32(&signal_strength_cache.gsm_sig1);
        p->readInt32(&signal_strength_cache.gsm_sig2);
        for (int i = 0; i < 5; i++) {
            // skip C2K part
            p->readInt32(&skip);
        }
        p->readInt32(&signal_strength_cache.lte_sig);
        p->readInt32(&signal_strength_cache.lte_rsrp);
        p->readInt32(&signal_strength_cache.lte_rsrq);
        p->readInt32(&signal_strength_cache.lte_rssnr);
        p->readInt32(&signal_strength_cache.lte_cqi);
        p->readInt32(&signal_strength_cache.rssi_qdbm);
        p->readInt32(&signal_strength_cache.rscp_qdbm);
        p->readInt32(&signal_strength_cache.ecn0_qdbm);
    } else if (source == RADIO_TECH_GROUP_C2K) {
        for (int i = 0; i < 2; i++) {
            // skip GSM part
            p->readInt32(&skip);
        }
        p->readInt32(&signal_strength_cache.cdma_dbm);
        p->readInt32(&signal_strength_cache.cdma_ecio);
        p->readInt32(&signal_strength_cache.ecdo_dbm);
        p->readInt32(&signal_strength_cache.evdo_ecio);
        p->readInt32(&signal_strength_cache.evdo_snr);
    } else {
        // source type invalid!!!
    }
}

void RpNwStateController::updateOperatorCache(const sp<RfxMessage>& msg) {
    int source = msg->getSource();
    int32_t stgCount = -1;
    char **pStrings = NULL;
    Parcel *p = msg->getParcel();

    p->readInt32(&stgCount);
    logD(RP_NW_STATE_TAG, " updateOperatorCache(): src=%s, stgCount=%d",
            sourceToString(source), stgCount);

    if (stgCount > 0) {
        pStrings = (char **) alloca(sizeof(char *) * stgCount);
        for (int i = 0; i < stgCount; i++) {
            pStrings[i] = strdupReadString(p);
            //logD(RP_NW_STATE_TAG, " updateOperatorCache(): data=%s", pStrings[i]);
        }
        if (source == RADIO_TECH_GROUP_GSM) {
            gsm_operator_cache.count = stgCount;
            if (stgCount == 3) {
                if (pStrings[0] != NULL) {
                    strncpy(gsm_operator_cache.optrAlphaLong, pStrings[0], MAX_OPER_NAME_LENGTH);
                } else {
                    gsm_operator_cache.optrAlphaLong[0] = '\0';
                    logD(RP_NW_STATE_TAG, " updateOperatorCache(): pStrings[0]=null!!");
                }
                if (pStrings[1] != NULL) {
                    strncpy(gsm_operator_cache.optrAlphaShort, pStrings[1], MAX_OPER_NAME_LENGTH);
                } else {
                    gsm_operator_cache.optrAlphaShort[0] = '\0';
                    logD(RP_NW_STATE_TAG, " updateOperatorCache(): pStrings[1]=null!!");
                }
                if (pStrings[2] != NULL) {
                    strncpy(gsm_operator_cache.optrNumeric, pStrings[2], MAX_OPER_NUM_LENGTH);
                } else {
                    gsm_operator_cache.optrNumeric[0] = '\0';
                    logD(RP_NW_STATE_TAG, " updateOperatorCache(): pStrings[2]=null!!");
                }
                gsm_operator_cache.optrAlphaLong[MAX_OPER_NAME_LENGTH - 1] = '\0';
                gsm_operator_cache.optrAlphaShort[MAX_OPER_NAME_LENGTH - 1] = '\0';
                gsm_operator_cache.optrNumeric[MAX_OPER_NUM_LENGTH - 1] = '\0';
            } else if (stgCount == 1) {
                strncpy(gsm_operator_cache.optrAlphaLong, pStrings[0], MAX_OPER_NAME_LENGTH);
                gsm_operator_cache.optrAlphaLong[MAX_OPER_NAME_LENGTH - 1] = '\0';
                memset(gsm_operator_cache.optrAlphaShort, '\0',
                        MAX_OPER_NAME_LENGTH * sizeof(char));
                memset(gsm_operator_cache.optrNumeric, '\0',
                        MAX_OPER_NUM_LENGTH * sizeof(char));
            } else {
                resetOperatorCache(&gsm_operator_cache);
            }
            printOperatorCache(gsm_operator_cache);
        } else if (source == RADIO_TECH_GROUP_C2K) {
            cdma_operator_cache.count = stgCount;
            if (stgCount == 3) {
                strncpy(cdma_operator_cache.optrAlphaLong, pStrings[0], MAX_OPER_NAME_LENGTH);
                strncpy(cdma_operator_cache.optrAlphaShort, pStrings[1], MAX_OPER_NAME_LENGTH);
                strncpy(cdma_operator_cache.optrNumeric, pStrings[2], MAX_OPER_NUM_LENGTH);
                cdma_operator_cache.optrAlphaLong[MAX_OPER_NAME_LENGTH - 1] = '\0';
                cdma_operator_cache.optrAlphaShort[MAX_OPER_NAME_LENGTH - 1] = '\0';
                cdma_operator_cache.optrNumeric[MAX_OPER_NUM_LENGTH - 1] = '\0';
            } else {
                resetOperatorCache(&cdma_operator_cache);
            }
            printOperatorCache(cdma_operator_cache);
        } else {
            // source type invalid!!!
            logD(RP_NW_STATE_TAG, " updateOperatorCache(): source type invalid!!!");
        }
    }
    if (pStrings != NULL) {
        for (int i = 0; i < stgCount; i++) {
            free(pStrings[i]);
        }
    }
}

void RpNwStateController::combineVoiceRegState(const sp<RfxMessage>& msg) {
//    int stgCount = 16;
//    char *stgBuf = (char *) malloc(50);
    // decide common info
    if (RfxNwServiceState::isInService(gsm_voice_reg_state_cache.register_state)) {
        mVoiceRadioSys = RADIO_TECH_GROUP_GSM;
        mVoiceRegState = gsm_voice_reg_state_cache.register_state;
        mVoiceRadioTech = gsm_voice_reg_state_cache.radio_technology;
        if (RfxNwServiceState::isInService(cdma_voice_reg_state_cache.register_state)
                && getDeviceMode() == NWS_MODE_CDMALTE
                && mVoiceRadioTech == RADIO_TECH_LTE) {
            logD(RP_NW_STATE_TAG, " SRLTE mode 1x & LTE both connected!! Select 1x");
            mVoiceRadioSys = RADIO_TECH_GROUP_C2K;
            mVoiceRegState = cdma_voice_reg_state_cache.register_state;
            mVoiceRadioTech = cdma_voice_reg_state_cache.radio_technology;
        }
    } else if (RfxNwServiceState::isInService(cdma_voice_reg_state_cache.register_state)) {
        mVoiceRadioSys = RADIO_TECH_GROUP_C2K;
        mVoiceRegState = cdma_voice_reg_state_cache.register_state;
        mVoiceRadioTech = cdma_voice_reg_state_cache.radio_technology;
    } else {
        if (getDeviceMode() == NWS_MODE_CDMALTE) {
            mVoiceRegState = cdma_voice_reg_state_cache.register_state;
            mVoiceRadioTech = cdma_voice_reg_state_cache.radio_technology;
        } else {
            mVoiceRegState = gsm_voice_reg_state_cache.register_state;
            mVoiceRadioTech = gsm_voice_reg_state_cache.radio_technology;
        }
    }

    logI(RP_NW_STATE_TAG, " combineVoiceRegState(): reg=%d, rat=%d",
            mVoiceRegState, mVoiceRadioTech);
    getStatusManager()->setServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
            RfxNwServiceState(mVoiceRegState, mDataRegState, mVoiceRadioTech, mDataRadioTech));
}

void RpNwStateController::combineDataRegState(const sp<RfxMessage>& msg) {
    if (RfxNwServiceState::isInService(gsm_data_reg_state_cache.register_state)) {
        mDataRadioSys = RADIO_TECH_GROUP_GSM;
        mDataRegState = gsm_data_reg_state_cache.register_state;
        mDataRadioTech = gsm_data_reg_state_cache.radio_technology;
    } else if (RfxNwServiceState::isInService(cdma_data_reg_state_cache.register_state)) {
        mDataRadioSys = RADIO_TECH_GROUP_C2K;
        mDataRegState = cdma_data_reg_state_cache.register_state;
        mDataRadioTech = cdma_data_reg_state_cache.radio_technology;
    } else {
        mDataRegState = gsm_data_reg_state_cache.register_state;
        mDataRadioTech = gsm_data_reg_state_cache.radio_technology;
        if (getDeviceMode() == NWS_MODE_CDMALTE) {
            int radioCapability = getStatusManager()->getIntValue(
                    RFX_STATUS_KEY_RADIO_CAPABILITY, RIL_CAPABILITY_NONE);
            if (radioCapability == RIL_CAPABILITY_CDMA_ONLY) {
                mDataRegState = cdma_data_reg_state_cache.register_state;
                mDataRadioTech = cdma_data_reg_state_cache.radio_technology;
            } else {
                if (mDataRadioSys == RADIO_TECH_GROUP_C2K) {
                    mDataRegState = cdma_data_reg_state_cache.register_state;
                    mDataRadioTech = cdma_data_reg_state_cache.radio_technology;
                }
            }
        }
    }

    logI(RP_NW_STATE_TAG, " combineDataRegState(): reg=%d, rat=%d",
            mDataRegState, mDataRadioTech);
    getStatusManager()->setServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
            RfxNwServiceState(mVoiceRegState, mDataRegState, mVoiceRadioTech, mDataRadioTech));
}

void RpNwStateController::combineSignalStrength(const sp<RfxMessage>& msg) {
    Parcel *p = msg->getParcel();

    android::status_t status;
    std::vector<android::status_t> statusVector;
    status = p->writeInt32(signal_strength_cache.gsm_sig1);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.gsm_sig2);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.cdma_dbm);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.cdma_ecio);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.ecdo_dbm);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.evdo_ecio);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.evdo_snr);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.lte_sig);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.lte_rsrp);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.lte_rsrq);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.lte_rssnr);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.lte_cqi);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.rssi_qdbm);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.rscp_qdbm);
    statusVector.push_back(status);
    status = p->writeInt32(signal_strength_cache.ecn0_qdbm);
    statusVector.push_back(status);
    for(int i = 0; i < statusVector.size(); i++) {
        if (statusVector[i] != android::OK) {
            logE(RP_NW_STATE_TAG, "writeSignalStrengthCache failed! statusVector[%d]=%d", i, statusVector[i]);
            break;
        }
    }
    printSignalStrengthCache(signal_strength_cache);
}

void RpNwStateController::combineOperatorState(const sp<RfxMessage>& msg) {
    Parcel *p = msg->getParcel();
    RIL_OPERATOR_CACHE optrCache = gsm_operator_cache;

    if (getDeviceMode() == NWS_MODE_CDMALTE) {
        if (RfxNwServiceState::isInService(cdma_data_reg_state_cache.register_state)) {
            logD(RP_NW_STATE_TAG, " combineOperatorState(): C2K ps in service");
            optrCache = cdma_operator_cache;
        } else if (RfxNwServiceState::isInService(cdma_voice_reg_state_cache.register_state)
                && !RfxNwServiceState::isInService(gsm_data_reg_state_cache.register_state)) {
            logD(RP_NW_STATE_TAG, " combineOperatorState(): C2K cs in service");
            optrCache = cdma_operator_cache;
        } else if (mDataRadioSys == RADIO_TECH_GROUP_C2K) {
            logD(RP_NW_STATE_TAG, " combineOperatorState(): C2K = mDataRadioSys");
            optrCache = cdma_operator_cache;
        } else if (strncmp("000000", gsm_operator_cache.optrNumeric, 6) == 0) {
            logD(RP_NW_STATE_TAG, " combineOperatorState(): GSM numeric invalid");
            optrCache = cdma_operator_cache;
        }
    }
    logD(RP_NW_STATE_TAG, " combineOperatorState(): count=%d", optrCache.count);
    p->writeInt32(optrCache.count);
    if (optrCache.count == 3) {
        writeStringToParcel(p, optrCache.optrAlphaLong);
        writeStringToParcel(p, optrCache.optrAlphaShort);
        writeStringToParcel(p, optrCache.optrNumeric);
    } else if (optrCache.count == 1) {
        writeStringToParcel(p, optrCache.optrAlphaLong);
    }
}

bool RpNwStateController::onHandleResponse(const sp<RfxMessage>& response) {
    int reqId = response->getId();
    int slotId = response->getSlotId();
    int source = response->getSource();
    sp<RfxMessage> responseInQueue;
    sp<RfxMessage> responseMd1;
    sp<RfxMessage> responseMd3;
    ResponseStatus responseStatus;

    // MT2635: gsm case handle resposne
    logD(RP_NW_STATE_TAG, " onHandleResponse() only GSM SIM%d req=%s src=%s", slotId,
            requestToString(reqId), sourceToString(source));
    responseMd1 = response;
    responseMd3 = sp<RfxMessage>(NULL);
    switch (reqId) {
    case RIL_REQUEST_VOICE_REGISTRATION_STATE:
        onReceiveVoiceRegStateResponse(responseMd1, responseMd3);
        break;
    case RIL_REQUEST_DATA_REGISTRATION_STATE:
        onReceiveDataRegStateResponse(responseMd1, responseMd3);
        break;

#if defined(TELE_FWK)
    case RIL_REQUEST_GET_RADIO_CAPABILITY:
        {
            logD(RP_NW_STATE_TAG, "SimCapabilitySwitch GET RADIO CAPABILITY response");

            responseToRilj(response);  //no need ro report to rilj
        }
        break;
    case RIL_REQUEST_SET_RADIO_CAPABILITY:
        {
        logD(RP_NW_STATE_TAG, "SimCapabilitySwitch SET RADIO CAPABILITY response");

//set the SIM EDALLOWED
#if  defined(ANDROID_MULTI_SIM)
        sp<RfxMessage> newMsg;
        Parcel* newParcel;
        int main_slot;
        size_t pos;
        main_slot = rfx_get_data_allowed_slotid();
        if(rfx_check_if_DSSS_IC()){     //If it is DSSS IC, radio power on should be sent to main protocol slot by tele_fwk
            if(getSlotId() == main_slot){
                if(RIL_E_SUCCESS == response->getError()){
                    newMsg = RfxMessage::obtainRequest(getSlotId(),RADIO_TECH_GROUP_GSM, RIL_REQUEST_RADIO_POWER);
                    newParcel = newMsg->getParcel();
                    newParcel->writeInt32(1);
                    newParcel->writeInt32(1);
                    requestToRild(newMsg);
                }else{
                    logD(RP_NW_STATE_TAG, "set radio capability error!");
                }
            }
        }
        newMsg = RfxMessage::obtainRequest(getSlotId(),RADIO_TECH_GROUP_GSM, RIL_REQUEST_ALLOW_DATA);
        newParcel = newMsg->getParcel();
        pos = newParcel->dataPosition();
        logD(RP_NW_STATE_TAG,"SimCapabilitySwitch set the actually enable data capability");
        newParcel->writeInt32(1); //num of ints
#if  defined(MODE_DSSS)//dsss
        if(getSlotId() == rfx_get_data_allowed_slotid())
        {
            newParcel->writeInt32(1); //default set the allow data enabled.
            logD(RP_NW_STATE_TAG,"SimCapabilitySwitch dsss SetDataAttached = 1");
        }
        else
        {
            newParcel->writeInt32(0); //default set the allow data enabled.
            logD(RP_NW_STATE_TAG,"SimCapabilitySwitch dsss SetDataAttached = 0");
        }
#else //dsds
        if(getSlotId() == rfx_get_data_allowed_slotid())
        {
            newParcel->writeInt32(1); //default set the allow data enabled.
            logD(RP_NW_STATE_TAG,"SimCapabilitySwitch dsds SetDataAttached = 1");
        }
        else
        {
            newParcel->writeInt32(0); //default set the allow data enabled.
            logD(RP_NW_STATE_TAG,"SimCapabilitySwitch dsds SetDataAttached = 0");
        }

#endif
        requestToRild(newMsg);

#endif
        responseToRilj(response);  //no need ro report to rilj
        }
        break;
#endif
    case RIL_REQUEST_SIGNAL_STRENGTH: {
        onSignalStrengthResult(response);
        mDontPollSignalStrength = true;
        break;
    }
    default:
        responseToRilj(response);
        break;
    }

    return true;
}

void RpNwStateController::onReceiveVoiceRegStateResponse(
        const sp<RfxMessage>& responseMd1, const sp<RfxMessage>& responseMd3) {
    int errInd = 0;
    sp<RfxMessage> resToRilj;

    if (responseMd1 != NULL && responseMd1->getError() == RIL_E_SUCCESS) {
        updateVoiceRegStateCache(responseMd1);
        logD(RP_NW_STATE_TAG, " GSM voice response ok");
    } else {
        // response contains error
        errInd |= 1;
        resetVoiceRegStateCache(&gsm_voice_reg_state_cache);
        logD(RP_NW_STATE_TAG, " GSM voice response error");
    }
    if (responseMd3 != NULL && responseMd3->getError() == RIL_E_SUCCESS) {
        updateVoiceRegStateCache(responseMd3);
        logD(RP_NW_STATE_TAG, " C2K voice response ok");
    } else {
        // response contains error
        errInd |= 2;
        resetVoiceRegStateCache(&cdma_voice_reg_state_cache);
        logD(RP_NW_STATE_TAG, " C2K voice response error");
    }
    /** MT2635: only for GSM
    resToRilj = genResponseForRilj(errInd, responseMd1, responseMd3);
    **/
    resToRilj = responseMd1;
    if (errInd < 3) {
        combineVoiceRegState(resToRilj);
    }

    responseToRilj(resToRilj);
}

void RpNwStateController::onReceiveDataRegStateResponse(
        const sp<RfxMessage>& responseMd1, const sp<RfxMessage>& responseMd3) {
    int errInd = 0;
    sp<RfxMessage> resToRilj;

    if (responseMd1 != NULL && responseMd1->getError() == RIL_E_SUCCESS) {
        updateDataRegStateCache(responseMd1);
        logD(RP_NW_STATE_TAG, " GSM data response ok");
    } else {
        // response contains error
        errInd |= 1;
        resetDataRegStateCache(&gsm_data_reg_state_cache);
        logD(RP_NW_STATE_TAG, " GSM data response error");
    }
    if (responseMd3 != NULL && responseMd3->getError() == RIL_E_SUCCESS) {
        updateDataRegStateCache(responseMd3);
        logD(RP_NW_STATE_TAG, " C2K data response ok");
    } else {
        // response contains error
        errInd |= 2;
        resetDataRegStateCache(&cdma_data_reg_state_cache);
        logD(RP_NW_STATE_TAG, " C2K data response error");
    }
    /** MT2635: only for GSM
    resToRilj = genResponseForRilj(errInd, responseMd1, responseMd3);
    **/
    resToRilj = responseMd1;
    if (errInd < 3) {
        combineDataRegState(resToRilj);
    }

    responseToRilj(resToRilj);
}

void RpNwStateController::onReceiveSignalStrengthResponse(
        const sp<RfxMessage>& responseMd1, const sp<RfxMessage>& responseMd3) {
    int errInd = 0;
    sp<RfxMessage> resToRilj;

    if (responseMd1->getError() == RIL_E_SUCCESS) {
        updateSignalStrengthCache(responseMd1);
        logD(RP_NW_STATE_TAG, " GSM sig response ok");
    } else {
        // response contains error
        errInd |= 1;
        logD(RP_NW_STATE_TAG, " GSM sig response error");
    }
    if (responseMd3->getError() == RIL_E_SUCCESS) {
        updateSignalStrengthCache(responseMd3);
        logD(RP_NW_STATE_TAG, " C2K sig response ok");
    } else {
        // response contains error
        errInd |= 2;
        logD(RP_NW_STATE_TAG, " C2K sig response error");
    }
    resToRilj = genResponseForRilj(errInd, responseMd1, responseMd3);
    if (errInd < 3) {
        combineSignalStrength(resToRilj);
    }

    responseToRilj(resToRilj);
}

void RpNwStateController::onReceiveOperatorResponse(
        const sp<RfxMessage>& responseMd1, const sp<RfxMessage>& responseMd3) {
    int errInd = 0;
    sp<RfxMessage> resToRilj;

    if (responseMd1->getError() == RIL_E_SUCCESS) {
        updateOperatorCache(responseMd1);
        logD(RP_NW_STATE_TAG, " GSM optr response ok");
    } else {
        // response contains error
        errInd |= 1;
        resetOperatorCache(&gsm_operator_cache);
        logD(RP_NW_STATE_TAG, " GSM optr response error");
    }

    if (responseMd3->getError() == RIL_E_SUCCESS) {
        updateOperatorCache(responseMd3);
        logD(RP_NW_STATE_TAG, " C2K optr response ok");
    } else {
        // response contains error
        errInd |= 2;
        resetOperatorCache(&cdma_operator_cache);
        logD(RP_NW_STATE_TAG, " C2K optr response error");
    }

    resToRilj = genResponseForRilj(errInd, responseMd1, responseMd3);
    if (errInd < 3) {
        combineOperatorState(resToRilj);
    }

    responseToRilj(resToRilj);
}

void RpNwStateController::onReceiveCellInfoList(
            const sp<RfxMessage>& responseMd1, const sp<RfxMessage>& responseMd3) {
    int errInd = 0;
    sp<RfxMessage> resToRilj;

    if (responseMd1->getError() == RIL_E_SUCCESS) {
        //updateCellInfoCache(responseMd1);
        logD(RP_NW_STATE_TAG, " GSM optr response ok");
    } else {
        // response contains error
        errInd |= 1;
        logD(RP_NW_STATE_TAG, " GSM optr response error");
    }
    if (responseMd3->getError() == RIL_E_SUCCESS) {
        //updateCellInfoCache(responseMd3);
        logD(RP_NW_STATE_TAG, " C2K optr response ok");
    } else {
        // response contains error
        errInd |= 2;
        logD(RP_NW_STATE_TAG, " C2K optr response error");
    }
    resToRilj = genResponseForRilj(errInd, responseMd1, responseMd3);
    if (errInd < 3) {
        combineOperatorState(resToRilj);
    }

    responseToRilj(resToRilj);
}

sp<RfxMessage> RpNwStateController::genResponseForRilj(int errInd,
        const sp<RfxMessage>& responseMd1, const sp<RfxMessage>& responseMd3) {
    sp<RfxMessage> rlt;

    if (errInd == 0 || errInd == 2) {
        rlt = RfxMessage::obtainResponse(RIL_E_SUCCESS, responseMd1);
    } else if (errInd == 1) {
        rlt = RfxMessage::obtainResponse(RIL_E_SUCCESS, responseMd3);
    } else {
        rlt = responseMd1;
    }

    return rlt;
}

void RpNwStateController::onResponseTimeOut(const sp<RfxMessage>& response) {
    int reqId = response->getId();
    int slotId = response->getSlotId();
    logD(RP_NW_STATE_TAG, " onResponseTimeOut() SIM%d REQ=%s src=%s token=%d",
            slotId, requestToString(reqId), sourceToString(response->getSource()),
            response->getPToken());
    responseToRilj(response);
}

int RpNwStateController::getDeviceMode() {
    int mode = getStatusManager()->getIntValue(RFX_STATUS_KEY_NWS_MODE, NWS_MODE_CSFB);
    logD(RP_NW_STATE_TAG, " [getDeviceMode] mode=%d", mode);
    return mode;
}

int RpNwStateController::getChipTestMode() {
    int mode = 0;
    //char chipsetMode[PROPERTY_VALUE_MAX] = { 0 };
    //property_get("persist.chiptest.enable", chipsetMode, "0");
    //mode = atoi(chipsetMode);
    //logD(LOG_TAG,"getChipTestMode():%d", mode);
    return mode;
}

int RpNwStateController::convertOtaProvisionStatus(int rawState){
    int state;
    switch(rawState) {
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
        default:
            state = -1;
    }

    return state;
}


char *RpNwStateController::strdupReadString(Parcel *p) {
    size_t stringlen;
    const char16_t *s16;

    s16 = p->readString16Inplace(&stringlen);

    return strndup16to8(s16, stringlen);
}

void RpNwStateController::writeStringToParcel(Parcel *p, const char *s) {
    char16_t *s16;
    size_t s16_len = 0;
    s16 = strdup8to16(s, &s16_len);
    p->writeString16(s16, s16_len);
    free(s16);
}

void RpNwStateController::resetVoiceRegStateCache(RIL_VOICE_REG_STATE_CACHE *voiceCache) {
    (*voiceCache).register_state = 0;
    (*voiceCache).lac = 0xffffffff;
    (*voiceCache).cid = 0x0fffffff;
    (*voiceCache).radio_technology = 0;
    (*voiceCache).base_station_id = 0;
    (*voiceCache).base_station_latitude = 0;
    (*voiceCache).base_station_longitude = 0;
    (*voiceCache).css = 0;
    (*voiceCache).system_id = 0;
    (*voiceCache).network_id = 0;
    (*voiceCache).roaming_ind = 1;  // home
    (*voiceCache).prl = 0;
    (*voiceCache).prl_roaming_ind = 0;
    (*voiceCache).deny_reason = 0;
    (*voiceCache).psc = -1;
}

void RpNwStateController::resetDataRegStateCache(RIL_DATA_REG_STATE_CACHE *dataCache) {
    (*dataCache).register_state = 0;
    (*dataCache).lac = 0xffffffff;
    (*dataCache).cid = 0x0fffffff;
    (*dataCache).radio_technology = 0;
    (*dataCache).deny_reason = 0;
    (*dataCache).max_data_call = 1;
    (*dataCache).tac = 0;
    (*dataCache).physical_cid = 0;
    (*dataCache).eci = 0;
    (*dataCache).csgid = 0;
    (*dataCache).tadv = 0;
}

void RpNwStateController::resetSignalStrengthCache(RIL_SIGNAL_STRENGTH_CACHE *sigCache) {
    (*sigCache).gsm_sig1 = 99;
    (*sigCache).gsm_sig2 = -1;
    (*sigCache).cdma_dbm = -1;
    (*sigCache).cdma_ecio = -1;
    (*sigCache).ecdo_dbm = -1;
    (*sigCache).evdo_ecio = -1;
    (*sigCache).evdo_snr = -1;
    (*sigCache).lte_sig = -99;
    (*sigCache).lte_rsrp = 0x7FFFFFFF;
    (*sigCache).lte_rsrq = 0x7FFFFFFF;
    (*sigCache).lte_rssnr = 0x7FFFFFFF;
    (*sigCache).lte_cqi = 0x7FFFFFFF;
    (*sigCache).rssi_qdbm = 0;
    (*sigCache).rscp_qdbm = 0;
    (*sigCache).ecn0_qdbm = 0;
}

void RpNwStateController::resetOperatorCache(RIL_OPERATOR_CACHE *optrCache) {
    (*optrCache).count = 0;
    memset((*optrCache).optrAlphaLong, '\0', MAX_OPER_NAME_LENGTH * sizeof(char));
    memset((*optrCache).optrAlphaShort, '\0', MAX_OPER_NAME_LENGTH * sizeof(char));
    memset((*optrCache).optrNumeric, '\0', MAX_OPER_NUM_LENGTH * sizeof(char));
}

char *RpNwStateController::requestToString(int reqId) {
    switch (reqId) {
        case RIL_REQUEST_VOICE_REGISTRATION_STATE:
            return "REQ_VOICE_REG_STATE";
        case RIL_REQUEST_DATA_REGISTRATION_STATE:
            return "REQ_DATA_REG_STATE";
        case RIL_REQUEST_SIGNAL_STRENGTH:
            return "REQ_SIGNAL_STRENGTH";
        case RIL_REQUEST_OPERATOR:
            return "REQ_OPERATOR";
        case RIL_REQUEST_GET_CELL_INFO_LIST:
            return "REQ_GET_CELL_INFO_LIST";
        case RIL_REQUEST_GET_RADIO_CAPABILITY:
            return "REQUEST_GET_RADIO_CAPABILITY";
        case RIL_REQUEST_SET_RADIO_CAPABILITY:
            return "REQUEST_SET_RADIO_CAPABILITY";
        case RIL_REQUEST_LOCAL_REG_STATE_TYPE:
            return "RIL_REQUEST_LOCAL_REG_STATE_TYPE";
        case RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE:
            return "RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE";
        case RIL_REQUEST_LOCAL_GET_OPERATOR_CODE:
            return "RIL_REQUEST_LOCAL_GET_OPERATOR_CODE";
        case RIL_REQUEST_LOCAL_SIGNAL_STRENGTH:
            return "RIL_REQUEST_LOCAL_SIGNAL_STRENGTH";
        default:
            return "INVALID REQUEST";
    }
}

char *RpNwStateController::urcToString(int urcId) {
    switch (urcId) {
        case RIL_UNSOL_SIGNAL_STRENGTH:
            return "UNSOL_SIGNAL_STRENGTH";
        case RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED:
            return "UNSOL_PS_NW_STATE_CHANGED";
        case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED:
            return "UNSOL_CS_NW_STATE_CHANGED";
        case RIL_UNSOL_CDMA_OTA_PROVISION_STATUS:
            return "RIL_UNSOL_CDMA_OTA_PROVISION_STATUS";
        case RIL_UNSOL_RIL_CONNECTED:
            return "RIL_UNSOL_RIL_CONNECTED";
        default:
            return "INVALID URC";
    }
}

char *RpNwStateController::sourceToString(int srcId) {
    switch (srcId) {
        case RADIO_TECH_GROUP_GSM:
            return "GSM";
        case RADIO_TECH_GROUP_C2K:
            return "C2K";
        default:
            return "INVALID SRC";
    }
}

void RpNwStateController::printVoiceCache(RIL_VOICE_REG_STATE_CACHE cache) {
    if (cache.cacheType == RADIO_TECH_GROUP_GSM) {
        logD(RP_NW_STATE_TAG, " VoiceCache[GSM]: reg=%d lac=%x cid=%x rat=%d css=%d \
sid=%d nid=%d roam=%d css=%d",
                cache.register_state, cache.lac, cache.cid, cache.radio_technology,
                cache.css, cache.system_id, cache.network_id, cache.roaming_ind, cache.css);
    } else if (cache.cacheType == RADIO_TECH_GROUP_C2K) {
        logD(RP_NW_STATE_TAG, " VoiceCache[C2K]: reg=%d lac=%x cid=%x rat=%d css=%d \
sid = %d nid = %d roam = %d css = %d exsit = %d",
                cache.register_state, cache.lac, cache.cid, cache.radio_technology,
                cache.css, cache.system_id, cache.network_id, cache.roaming_ind, cache.css,
                cache.network_exsit);
    }
}

void RpNwStateController::printDataCache(RIL_DATA_REG_STATE_CACHE cache) {
    if (cache.cacheType == RADIO_TECH_GROUP_GSM) {
        logD(RP_NW_STATE_TAG, " DataCache[GSM]: reg=%d lac=%x cid=%x rat=%d den=%d mdc=%d",
                cache.register_state, cache.lac, cache.cid, cache.radio_technology,
                cache.deny_reason, cache.max_data_call);
    } else if (cache.cacheType == RADIO_TECH_GROUP_C2K) {
        logD(RP_NW_STATE_TAG, " DataCache[C2K]: reg=%d lac=%x cid=%x rat=%d den=%d mdc=%d",
                cache.register_state, cache.lac, cache.cid, cache.radio_technology,
                cache.deny_reason, cache.max_data_call);
    }
}

void RpNwStateController::printSignalStrengthCache(RIL_SIGNAL_STRENGTH_CACHE cache) {
    logD(RP_NW_STATE_TAG,
            " Sig: gsm[%d %d] c2k[%d %d %d %d %d] lte[%d %d %d %d %d] umts[%d %d %d]",
            cache.gsm_sig1, cache.gsm_sig2,
            cache.cdma_dbm, cache.cdma_ecio, cache.ecdo_dbm, cache.evdo_ecio, cache.evdo_snr,
            cache.lte_sig, cache.lte_rsrp, cache.lte_rsrq, cache.lte_rssnr, cache.lte_cqi,
            cache.rssi_qdbm, cache.rscp_qdbm, cache.ecn0_qdbm);
}

void RpNwStateController::printOperatorCache(RIL_OPERATOR_CACHE cache) {
    logD(RP_NW_STATE_TAG, " OptrCache: %s,%s,%s",
            cache.optrAlphaLong, cache.optrAlphaShort, cache.optrNumeric);
}
