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
#include "RfxRilUtils.h"
#include "RfxLog.h"
#include "RfxAction.h"
#include "RfxMainThread.h"
#include "RfxNetworkScanResultData.h"
#include "RtmNwController.h"
#include "RfxDataRegStateData.h"
#include "libmtkrilutils.h"

#define NW_CTRL_TAG "RtmNwCtrl"


/*****************************************************************************
 * Class RtmNwController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtmNwController", RtmNwController, RfxController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_UPDATE_PHONE_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_VOICE_RADIO_TECH);

static const char *sNwOemRequestList[] = {
//    "AT+E5GOPT",
    "AT+ENRCACOMB=",
    "AT+ENRCACFG="
};

RtmNwController::RtmNwController() :
    mNetworkScanOngoing(false),
    mForceStopNetworkScan(false),
    mNetworkScanPending(false),
    mCancelNetworkScanPending(false),
    mStopNetworkScan(false),
    mPhoneMode(RADIO_TECH_UNKNOWN),
    inCall(false),
    mNwScanMessage(NULL),
    mAbortAction(NULL) {
    mFakeoosSupport = isSpecificPsStateConfigured();
}

RtmNwController::~RtmNwController() {

}

void RtmNwController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    const int request_id_list[] = {
        RFX_MSG_REQUEST_SET_PREFERRED_NETWORK_TYPE,
        RFX_MSG_REQUEST_GET_PREFERRED_NETWORK_TYPE,
        RFX_MSG_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP,
        RFX_MSG_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP,
        RFX_MSG_REQUEST_SET_BAND_MODE,
        RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE,
        RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE,
        RFX_MSG_REQUEST_DATA_REGISTRATION_STATE,
        RFX_MSG_REQUEST_OPERATOR,
        RFX_MSG_REQUEST_QUERY_NETWORK_SELECTION_MODE,
        RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER,
        RFX_MSG_REQUEST_SET_SERVICE_STATE,
        RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE,
        RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE,
        RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION,
        RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION,
        RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH,
        RFX_MSG_REQUEST_SMART_RAT_SWITCH,
        RFX_MSG_REQUEST_VOICE_RADIO_TECH,
        RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS,
        RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT,
        RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS,
        RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN,
        RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC,
        RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK,
        RFX_MSG_REQUEST_UPDATE_PHONE_MODE,
        RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE,
        RFX_MSG_REQUEST_GET_TS25_NAME,
        RFX_MSG_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED,
        RFX_MSG_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED,
        RFX_MSG_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
        RFX_MSG_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
        RFX_MSG_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED,
        RFX_MSG_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED,
        RFX_MSG_REQUEST_NW_OEM_HOOK_RAW
    };

    const int urc_id_list[] = {
        RFX_MSG_URC_VOICE_RADIO_TECH_CHANGED,
        RFX_MSG_URC_NETWORK_SCAN_RESULT
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int), DEFAULT);
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));

    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_MODEM_RESET,
            RfxStatusChangeCallback(this, &RtmNwController::onHandleModemReset));

    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        getStatusManager(slotId)->registerStatusChangedEx(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                RfxStatusChangeCallbackEx(this, &RtmNwController::onApVoiceCallCountChanged));
    }

    if (RfxRilUtils::isCtVolteSupport()) {
        getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_VOLTE_STATE,
                RfxStatusChangeCallback(this, &RtmNwController::onVolteStateChanged));
    }
    if (RfxRilUtils::isCtHvolteSupport()) {
        getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_IMS_REGISTRATION_STATE,
            RfxStatusChangeCallback(this, &RtmNwController::onImsRegistrationStatusChanged));
    }

    //inCall = false;
    // TODO - only support 2 sim now.
    if (mFakeoosSupport && RFX_SLOT_COUNT == 2) {
        int other_slot_id;
        if (m_slot_id == 0) other_slot_id = 1;
        else other_slot_id = 0;
        logD(NW_CTRL_TAG, "register RFX_STATUS_KEY_IN_CALL[%d]", other_slot_id);
        getStatusManager(other_slot_id)->registerStatusChanged(RFX_STATUS_KEY_IN_CALL,
            RfxStatusChangeCallback(this, &RtmNwController::onHandleInCallEvent));
        logD(NW_CTRL_TAG, "register RFX_STATUS_KEY_IMS_CALL_OCUPPY_RF[%d]", other_slot_id);
        getStatusManager(other_slot_id)->registerStatusChanged(RFX_STATUS_KEY_IMS_CALL_OCUPPY_RF,
            RfxStatusChangeCallback(this, &RtmNwController::onHandleOccuppyRFEvent));
        logD(NW_CTRL_TAG, "register RFX_STATUS_DSDA_MODE");
        getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_DSDA_MODE,
                RfxStatusChangeCallback(this, &RtmNwController::onHandleDsdaModeEvent));
        logD(NW_CTRL_TAG, "register RFX_STATUS_KEY_IMS_REGISTRATION_RAT");
        getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_IMS_REGISTRATION_RAT,
            RfxStatusChangeCallback(this, &RtmNwController::onHandleImsRegistrationRatChangedEvent));
        getStatusManager(other_slot_id)->registerStatusChanged(RFX_STATUS_KEY_IMS_REGISTRATION_RAT,
            RfxStatusChangeCallback(this, &RtmNwController::onHandleImsRegistrationRatChangedEvent));
    }

    logV(NW_CTRL_TAG, "[onInit]");
}

void RtmNwController::onDeinit() {
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_MODEM_RESET,
            RfxStatusChangeCallback(this, &RtmNwController::onHandleModemReset));

    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        getStatusManager(slotId)->unRegisterStatusChangedEx(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                RfxStatusChangeCallbackEx(this, &RtmNwController::onApVoiceCallCountChanged));
    }

    if (RfxRilUtils::isCtVolteSupport()) {
        getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_VOLTE_STATE,
                RfxStatusChangeCallback(this, &RtmNwController::onVolteStateChanged));
    }
    if (RfxRilUtils::isCtHvolteSupport()) {
        getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_IMS_REGISTRATION_STATE,
           RfxStatusChangeCallback(this, &RtmNwController::onImsRegistrationStatusChanged));
    }
}


bool RtmNwController::onHandleRequest(const sp<RfxMessage>& message) {
    int msgId = message->getId();
    sp<RfxMessage> request;
    sp<RfxMessage> response;

    switch (msgId) {
        case RFX_MSG_REQUEST_VOICE_RADIO_TECH:
            requestVoiceRadioTech(message);
            break;
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS:
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT:
            logV(NW_CTRL_TAG, "mNetworkScanOngoing:%d isAPInCall:%d",
                    mNetworkScanOngoing, isAPInCall());
            mNetworkScanPending = false;
            if (mCancelNetworkScanPending) {
                logV(NW_CTRL_TAG, "cancel pending network scan");
                mCancelNetworkScanPending = false;
                response = RfxMessage::obtainResponse(RIL_E_CANCELLED, message);
                responseToRilj(response);
                return true;
            }
            if (isAPInCall()) {
                response = RfxMessage::obtainResponse(RIL_E_MODEM_ERR, message);
                responseToRilj(response);
                return true;
            }

            if (mNetworkScanOngoing) {
                mNwScanMessage = message;
                mForceStopNetworkScan = true;
                request = RfxMessage::obtainRequest(m_slot_id,
                        RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS, RfxVoidData());
                requestToMcl(request);
            } else {
                mNetworkScanOngoing = true;
                requestToMcl(message);
            }
            break;

        case RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN:
            logV(NW_CTRL_TAG, "mNetworkScanOngoing:%d isAPInCall:%d",
                    mNetworkScanOngoing, isAPInCall());
            mNetworkScanPending = false;
            if (mCancelNetworkScanPending == true) {
                logV(NW_CTRL_TAG, "cancel pending network scan");
                mCancelNetworkScanPending = false;
                response = RfxMessage::obtainResponse(RIL_E_CANCELLED, message);
                responseToRilj(response);
                return true;
            }
            // VTS case will trigger netwrok scan many times but don't trigger cancel.
            // RILD send cancel between two network scan, avoid send scan twice to MD.
            if (mNetworkScanOngoing == true) {
                mNwScanMessage = message;
                mForceStopNetworkScan = true;
                request = RfxMessage::obtainRequest(m_slot_id,
                        RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN, RfxVoidData());
                requestToMcl(request);
            } else {
                mNetworkScanOngoing = true;
                requestToMcl(message);
            }
            break;

        case RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN:
            mStopNetworkScan = true;
            requestToMcl(message);
            break;

        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT:
        case RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
            logV(NW_CTRL_TAG, "mNetworkScanOngoing:%d", mNetworkScanOngoing);
            if (mNetworkScanOngoing) {
                mNwScanMessage = message;
                sp<RfxAction> action = new RfxAction0(this,
                        &RtmNwController::onLocalAbortAvailableNetworkDone);
                setLocalAbortAvailableNetwork(action);
            } else {
                requestToMcl(message);
            }
            break;

        default:
            requestToMcl(message);
            break;
    }
    return false;
}

bool RtmNwController::onHandleUrc(const sp<RfxMessage>& message) {
    int msgId = message->getId();

    if (msgId == RFX_MSG_URC_NETWORK_SCAN_RESULT) {
        RfxNetworkScanResultData *ptr = (RfxNetworkScanResultData *)message->getData();
        RIL_NetworkScanResult *data = (RIL_NetworkScanResult *)ptr->getData();
        logV(NW_CTRL_TAG, "mNetworkScanOngoing:%d data->status:%d",
                mNetworkScanOngoing, data->status);
        if (data->status == COMPLETE) {
            mNetworkScanOngoing = false;
        }
    } else if (msgId == RFX_MSG_URC_VOICE_RADIO_TECH_CHANGED) {
        RfxIntsData *intsData = (RfxIntsData*)message->getData();
        int *data = (int*)intsData->getData();
        mPhoneMode = data[0];
        logD(NW_CTRL_TAG, "updatePhoneMode to %d", mPhoneMode);
    }
    responseToRilj(message);
    return true;
}

bool RtmNwController::onHandleResponse(const sp<RfxMessage>& response) {
    int msgId = response->getId();
    switch (msgId) {
        case RFX_MSG_REQUEST_UPDATE_PHONE_MODE:
            logI(NW_CTRL_TAG, "RFX_MSG_REQUEST_UPDATE_PHONE_MODE, response!!");
            break;
        case RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN:
            responseToRilj(response);
            if (response->getError() != RIL_E_SUCCESS) {
                mNetworkScanOngoing = false;
            }
            break;
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS:
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT:
            mNetworkScanOngoing = false;
            responseToRilj(response);
            break;

        case RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS:
        case RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN:
            logV(NW_CTRL_TAG, "mNetworkScanOngoing:%d mForceStopNetworkScan:%d",
                    mNetworkScanOngoing, mForceStopNetworkScan);
            if (mForceStopNetworkScan) {
                if (mNwScanMessage != NULL) {
                    requestToMcl(mNwScanMessage);
                    mNwScanMessage = NULL;
                } else {
                    mNetworkScanOngoing = false;
                }
                mForceStopNetworkScan = false;
            } else {
                mNetworkScanOngoing = false;
                responseToRilj(response);
            }
            mStopNetworkScan = false;
            break;
        case RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK:
            if (mAbortAction != NULL) {
                mAbortAction->act();
                mAbortAction = NULL;
            }
            break;
        case RFX_MSG_REQUEST_DATA_REGISTRATION_STATE: {
            RfxDataRegStateData *ptr = (RfxDataRegStateData*) response->getData();

            if (ptr != NULL) {
                RIL_DataRegistrationStateResponse *data =
                    (RIL_DataRegistrationStateResponse*) ptr->getData();
                if (data != NULL) {
                    // For old version
                    // mode == 0: DSDA Inactive, mode == 1: DSda active
                    // For new version
                    // mode == 2: DSDA ongoing, mode == 3: DSDA possible, mode == 4: DSDA impossible
                    // we might need override only if slot cout = 2 and
                    // without DSDA active and dsda like
                    if (mFakeoosSupport && RFX_SLOT_COUNT == 2) {
                        int data_slot =
                            getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1);
                        logD(NW_CTRL_TAG, "data_slot=%d, inCall=%d", data_slot, inCall?1:0);
                        // Only need to detach data sim.
                        if (data_slot == m_slot_id) {
                            int dsdaModeRaw = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_DSDA_MODE, 0);
                            int dsdaMode = (int)(dsdaModeRaw / 100); //gen98 new change
                            int another_slot = (m_slot_id == 1) ? 0 : 1;
                            bool occupyRF = getStatusManager(another_slot)->getBoolValue(RFX_STATUS_KEY_IMS_CALL_OCUPPY_RF, true);
                            int imsRat = getStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_RAT, 0);
                            int otherSlotImsRat = getStatusManager(another_slot)->getIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_RAT, 0);
                            logD(NW_CTRL_TAG, "dsdaMode=%d, occupyRF=%d, imsRat=%d, otherSlotImsRat=%d", dsdaMode, occupyRF?1:0, imsRat, otherSlotImsRat);
                            if (inCall && occupyRF && dsdaMode != 1 && dsdaMode != 2 && dsdaMode != 3 && imsRat != 1 && otherSlotImsRat != 1) {
                                logD(NW_CTRL_TAG, "Override DataRegistrationState's state");
                                data->regState = RIL_NOT_REG_AND_NOT_SEARCHING;
                            }
                        }
                    }
                }
            }
            responseToRilj(response);
            }
            break;

        case RFX_MSG_REQUEST_NW_OEM_HOOK_RAW: {
            logV(NW_CTRL_TAG, "RFX_MSG_REQUEST_NW_OEM_HOOK_RAW response ++");
            sp<RfxMessage> rsp = RfxMessage::obtainResponse(RFX_MSG_REQUEST_OEM_HOOK_RAW, response);
            responseToRilj(rsp);
        }
            break;

        default:
            responseToRilj(response);
            break;
    }

    return true;
}

bool RtmNwController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
            bool isModemPowerOff, int radioState) {
    if (RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE == message->getId()) {
        return false;
    }

    /* Reject the request in radio unavailable or modem off */
    if ((radioState == (int)RADIO_STATE_UNAVAILABLE &&
            (RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER != message->getId() &&
            RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH != message->getId() &&
            RFX_MSG_REQUEST_SMART_RAT_SWITCH != message->getId())) ||
            isModemPowerOff == true) {
        logD(NW_CTRL_TAG, "onCheckIfRejectMessage, id = %d, isModemPowerOff = %d, rdioState = %d",
                message->getId(), isModemPowerOff, radioState);
        return true;
    }

    return false;
}

bool RtmNwController::onPreviewMessage(const sp<RfxMessage>& message) {
    int msgId = message->getId();

    if (mForceStopNetworkScan
        && msgId == RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN) {
        logV(NW_CTRL_TAG, "onPreviewMessage, delay network scan if nw scan stop ongoing!",
                RFX_ID_TO_STR(msgId));
        return false;
    }
    if (mNetworkScanPending
        && (msgId == RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN
             || msgId == RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS)) {
        logV(NW_CTRL_TAG, "onPreviewMessage, cancel pending netwrok scan",
                RFX_ID_TO_STR(msgId));
        mCancelNetworkScanPending = true;
    }

    return true;
}

bool RtmNwController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    int msgId = message->getId();

    if (mNetworkScanPending && mCancelNetworkScanPending
        && (msgId == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS
            || msgId == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT
            || msgId == RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN)) {
        logV(NW_CTRL_TAG, "resume netwrok scan to cancel", RFX_ID_TO_STR(msgId));
        return true;
    }

    return false;
}

void RtmNwController::onHandleModemReset(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    bool old_mode = old_value.asBool();
    bool new_mode = value.asBool();

    logE(NW_CTRL_TAG, "onHandleModemReset:old_mode:%d, new_mode:%d, mNetworkScanOngoing:%d",
            old_mode, new_mode, mNetworkScanOngoing);

    if (new_mode && mNetworkScanOngoing) {
        sp<RfxMessage> urcToRilj;
        RIL_NetworkScanResult* resp = (RIL_NetworkScanResult*)calloc(1, sizeof(RIL_NetworkScanResult));
        RFX_ASSERT(resp != NULL);
        resp->status = COMPLETE; // for now we don't support PARTIAL.
        urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_URC_NETWORK_SCAN_RESULT,
                RfxNetworkScanResultData((void*) resp, sizeof(RIL_NetworkScanResult)));
        responseToRilj(urcToRilj);
        free(resp);
        mNetworkScanOngoing = false;
    }
}


void RtmNwController::onVolteStateChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    int old_mode = old_value.asInt();
    int new_mode = value.asInt();
    if (old_mode != new_mode && new_mode == 1) {
        // volte status is changed and turn on the volte
        int updateType = 2;
        sp<RfxMessage> message = RfxMessage::obtainRequest(m_slot_id,
            RFX_MSG_REQUEST_UPDATE_PHONE_MODE, RfxIntsData(&updateType, 1));
        requestToMcl(message);
    }
}

void RtmNwController::onImsRegistrationStatusChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    int oldMode = old_value.asInt();
    int mode = value.asInt();
    if (oldMode != mode) {
        int updateType = 1;
        sp<RfxMessage> message = RfxMessage::obtainRequest(m_slot_id,
            RFX_MSG_REQUEST_UPDATE_PHONE_MODE, RfxIntsData(&updateType, 1));
        requestToMcl(message);
    }
}

void RtmNwController::onApVoiceCallCountChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    int oldMode = old_value.asInt();
    int mode = value.asInt();

    logD(NW_CTRL_TAG, "%s, slotId:%d, key:%d oldMode:%d, mode:%d",
            __FUNCTION__, slotId, key, oldMode, mode);
    if (mode == 1 && oldMode == 0 &&
            getNetworkScanSlot() != -1){
        sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(getNetworkScanSlot(),
                RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK, RfxVoidData());
        requestToMcl(reqToRild);
    }
}

int RtmNwController::getNetworkScanSlot() {
    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        if (getStatusManager(slotId)->getBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING)) {
            return slotId;
        }
    }
    return -1;
}

void RtmNwController::requestVoiceRadioTech(const sp<RfxMessage>& message) {
    logD(NW_CTRL_TAG, "requestVoiceRadioTech, phone mode[%d]", mPhoneMode);
    sp<RfxMessage> resToRilj;
    resToRilj = RfxMessage::obtainResponse(m_slot_id, RFX_MSG_REQUEST_VOICE_RADIO_TECH, RIL_E_SUCCESS,
            RfxIntsData(&mPhoneMode, 1), message);
    responseToRilj(resToRilj);
}

bool RtmNwController::isAPInCall() {
    bool ret = false;

    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        if (getStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0) > 0) {
            ret = true;
        }
    }
    return ret;
}

void RtmNwController::onLocalAbortAvailableNetworkDone() {
    logV(NW_CTRL_TAG, "Local abort done");
    if (mNwScanMessage != NULL) {
        requestToMcl(mNwScanMessage);
    }
    mNwScanMessage = NULL;
}


void RtmNwController::setLocalAbortAvailableNetwork(const sp<RfxAction>& action) {
    logV(NW_CTRL_TAG, "NetworkScanOngoing:%d",
            getStatusManager()->getBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING));
    if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_NETWORK_SCAN_ONGOING)) {
        sp<RfxMessage> request = RfxMessage::obtainRequest(getNetworkScanSlot(),
                RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK, RfxVoidData());
        requestToMcl(request);
        mAbortAction = action;
    } else {
        mAbortAction = NULL;
        action->act();
    }
}

void RtmNwController::onHandleInCallEvent(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    bool old_mode = old_value.asBool();
    bool new_mode = value.asBool();
    int data_slot = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1);
    logD(NW_CTRL_TAG, "onHandleInCallEvent: old_mode:%d, new_mode:%d",
            old_mode, new_mode);

    inCall = new_mode;
    if (m_slot_id != data_slot) return;

    sp<RfxMessage> urcToRilj;
    urcToRilj = RfxMessage::obtainUrc(m_slot_id,
            RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            RfxVoidData());
    // response to TeleCore
    responseToRilj(urcToRilj);
}

void RtmNwController::onHandleOccuppyRFEvent(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    bool old_mode = old_value.asBool();
    bool new_mode = value.asBool();
    int data_slot = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1);
    logD(NW_CTRL_TAG, "onHandleOccuppyRFEvent: old_mode:%d, new_mode:%d",
            old_mode, new_mode);

    if (m_slot_id != data_slot) return;
    if (!new_mode) return;

    sp<RfxMessage> urcToRilj;
    urcToRilj = RfxMessage::obtainUrc(m_slot_id,
            RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            RfxVoidData());
    // response to TeleCore
    responseToRilj(urcToRilj);
}

void RtmNwController::onHandleDsdaModeEvent(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    int data_slot = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1);
    int old_mode = old_value.asInt();
    int new_mode = value.asInt();
    logD(NW_CTRL_TAG, "onHandleDsdaModeEvent: old_mode:%d, new_mode:%d",
            old_mode, new_mode);

    if (m_slot_id != data_slot) return;
    if ((old_mode == new_mode) || !inCall) return;

    sp<RfxMessage> urcToRilj;
    urcToRilj = RfxMessage::obtainUrc(m_slot_id,
            RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            RfxVoidData());
    // response to TeleCore
    responseToRilj(urcToRilj);
}

bool RtmNwController::onHandleOemHookRawRequest(const sp<RfxMessage>& message) {
    char* data = (char *) message->getData()->getData();
    logV(NW_CTRL_TAG, "onHandleOemHookRawRequest: %s", data);
    if (data) {
        int reqListLen = sizeof(sNwOemRequestList)/sizeof(char*);
        for (int i = 0; i < reqListLen; i++) {
            if (strStartsWith(data, sNwOemRequestList[i])) {
                sp<RfxMessage> newMsg = RfxMessage::obtainRequest(message->getSlotId(),
                    RFX_MSG_REQUEST_NW_OEM_HOOK_RAW, message, true);
                requestToMcl(newMsg);
                return true;
            }
        }
    }
    return false;
}

void RtmNwController::onHandleImsRegistrationRatChangedEvent(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    int data_slot = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1);
    int old_mode = old_value.asInt();
    int new_mode = value.asInt();
    logD(NW_CTRL_TAG, "onHandleImsRegistrationRatChangedEvent: old_mode:%d, new_mode:%d",
            old_mode, new_mode);
    if (m_slot_id != data_slot) return;
    if ((old_mode == 1 && new_mode == 1) || (old_mode != 1 && new_mode != 1) || !inCall) return;

    sp<RfxMessage> urcToRilj;
    urcToRilj = RfxMessage::obtainUrc(m_slot_id,
            RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            RfxVoidData());
    // response to TeleCore
    responseToRilj(urcToRilj);
}