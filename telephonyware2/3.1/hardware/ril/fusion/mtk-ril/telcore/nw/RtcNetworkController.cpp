/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2017. All rights reserved.
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
#include "RtcNetworkController.h"
#include "libmtkrilutils.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define NW_CTRL_TAG "RtcNwCtrl"

RFX_IMPLEMENT_CLASS("RtcNetworkController", RtcNetworkController, RfxController);

RtcNetworkController::RtcNetworkController() :
    mNetworkScanOngoing(false),
    mForceStopNetworkScan(false),
    mNetworkScanPending(false),
    mCancelNetworkScanPending(false),
    mStopNetworkScan(false),
    mNwScanMessage(NULL),
    mSignalReqToken(0),
    isDcNrRestricted(0),
    isEndcAvailable(0),
    isNrAvailable(0),
    inCall(false) {
    mFakeoosSupport = isSpecificPsStateConfigured();
}

RtcNetworkController::~RtcNetworkController() {
}

void RtcNetworkController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
   logV(NW_CTRL_TAG, "[onInit]");
    const int request_id_list[] = {
        RFX_MSG_REQUEST_SET_BAND_MODE,
        RFX_MSG_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE,
        RFX_MSG_REQUEST_VOICE_REGISTRATION_STATE,
        RFX_MSG_REQUEST_DATA_REGISTRATION_STATE,
        RFX_MSG_REQUEST_OPERATOR,
        RFX_MSG_REQUEST_QUERY_NETWORK_SELECTION_MODE,
        RFX_MSG_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER,
        RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN,
        RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT,
        RFX_MSG_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC,
        RFX_MSG_REQUEST_SET_SERVICE_STATE,
        RFX_MSG_REQUEST_CDMA_SET_ROAMING_PREFERENCE,
        RFX_MSG_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE,
        RFX_MSG_REQUEST_SET_LTE_RELEASE_VERSION,
        RFX_MSG_REQUEST_GET_LTE_RELEASE_VERSION,
        RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS,
        RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT,
        RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS,
        RFX_MSG_REQUEST_REGISTER_CELLULAR_QUALITY_REPORT,
        RFX_MSG_REQUEST_GET_SMART_RAT_SWITCH,
        RFX_MSG_REQUEST_SMART_RAT_SWITCH,
        RFX_MSG_REQUEST_SIGNAL_STRENGTH,
        RFX_MSG_REQUEST_IWLAN_REGISTRATION_STATE,
        RFX_MSG_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED,
        RFX_MSG_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED,
        RFX_MSG_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
        RFX_MSG_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED,
        RFX_MSG_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED,
        RFX_MSG_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED,
        RFX_MSG_REQUEST_GET_TS25_NAME
    };

    const int urc_id_list[] = {
        RFX_MSG_URC_NETWORK_SCAN_RESULT
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int), DEFAULT);
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));
    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_MODEM_RESET,
            RfxStatusChangeCallback(this, &RtcNetworkController::onHandleModemReset));

    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_NR_SWITCH_SLOT,
            RfxStatusChangeCallback(this, &RtcNetworkController::onHandleNrSwitchSlotChanged));

    getStatusManager(m_slot_id)->registerStatusChangedEx(RFX_STATUS_KEY_PREFERRED_NW_TYPE,
                    RfxStatusChangeCallbackEx(this, &RtcNetworkController::onPreferredNwTypeChange));

    getStatusManager()->registerStatusChangedEx(RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallbackEx(this, &RtcNetworkController::onRadioStateChanged));

    mSignalReqToken = 0;

    isDcNrRestricted = 0;
    isEndcAvailable = 0;
    isNrAvailable = 0;
    //inCall = false;
    // TODO - only support 2 sim now.
    if (mFakeoosSupport && RFX_SLOT_COUNT == 2) {
        int other_slot_id;
        if (m_slot_id == 0) other_slot_id = 1;
        else other_slot_id = 0;
        logD(NW_CTRL_TAG, "register RFX_STATUS_KEY_IN_CALL[%d]", other_slot_id);
        getStatusManager(other_slot_id)->registerStatusChanged(RFX_STATUS_KEY_IN_CALL,
            RfxStatusChangeCallback(this, &RtcNetworkController::onHandleInCallEvent));
        logD(NW_CTRL_TAG, "register RFX_STATUS_KEY_IMS_CALL_OCUPPY_RF[%d]", other_slot_id);
        getStatusManager(other_slot_id)->registerStatusChanged(RFX_STATUS_KEY_IMS_CALL_OCUPPY_RF,
            RfxStatusChangeCallback(this, &RtcNetworkController::onHandleOccuppyRFEvent));
        logD(NW_CTRL_TAG, "register RFX_STATUS_DSDA_MODE");
        getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_DSDA_MODE,
                RfxStatusChangeCallback(this, &RtcNetworkController::onHandleDsdaModeEvent));
        logD(NW_CTRL_TAG, "register RFX_STATUS_KEY_IMS_REGISTRATION_RAT");
        getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_IMS_REGISTRATION_RAT,
            RfxStatusChangeCallback(this, &RtcNetworkController::onHandleImsRegistrationRatChangedEvent));
        getStatusManager(other_slot_id)->registerStatusChanged(RFX_STATUS_KEY_IMS_REGISTRATION_RAT,
            RfxStatusChangeCallback(this, &RtcNetworkController::onHandleImsRegistrationRatChangedEvent));
    }
}

bool RtcNetworkController::onHandleRequest(const sp<RfxMessage>& message) {
    // logD(NW_CTRL_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(message->getId()));
    int msg_id = message->getId();

    switch (msg_id) {
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS:
        case RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT:
            logV(NW_CTRL_TAG, "mNetworkScanOngoing:%d isAPInCall:%d isNRSwitching:%d",
                    mNetworkScanOngoing, isAPInCall(), isNRSwitching());
            mNetworkScanPending = false;
            if (mCancelNetworkScanPending == true) {
                logV(NW_CTRL_TAG, "cancel pending network scan");
                mCancelNetworkScanPending = false;
                sp<RfxMessage> resToRilj = RfxMessage::obtainResponse(RIL_E_CANCELLED, message);
                responseToRilj(resToRilj);
                return true;
            }
            if (isAPInCall() == true || isNRSwitching() == true) {
                sp<RfxMessage> resToRilj = RfxMessage::obtainResponse(RIL_E_MODEM_ERR, message);
                responseToRilj(resToRilj);
                return true;
            }
            if (mNetworkScanOngoing == true) {
                mNwScanMessage = message;
                mForceStopNetworkScan = true;
                sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(m_slot_id,
                        RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS, RfxVoidData());
                requestToMcl(reqToRild);
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
                sp<RfxMessage> resToRilj = RfxMessage::obtainResponse(RIL_E_CANCELLED, message);
                responseToRilj(resToRilj);
                return true;
            }
            // VTS case will trigger netwrok scan many times but don't trigger cancel.
            // RILD send cancel between two network scan, avoid send scan twice to MD.
            if (mNetworkScanOngoing == true) {
                mNwScanMessage = message;
                mForceStopNetworkScan = true;
                sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(m_slot_id,
                        RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN, RfxVoidData());
                requestToMcl(reqToRild);
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
            if (mNetworkScanOngoing == true) {
                mNwScanMessage = message;
                RtcRatSwitchController* nwRatController = (RtcRatSwitchController *)findController
                        (RFX_OBJ_CLASS_INFO(RtcRatSwitchController));
                sp<RfxAction> action = new RfxAction0(this,
                        &RtcNetworkController::onLocalAbortAvailableNetworkDone);
                nwRatController->setLocalAbortAvailableNetwork(action);
            } else {
                requestToMcl(message);
            }
            break;
        default:
            requestToMcl(message);
            break;
    }
    return true;
}

bool RtcNetworkController::onHandleResponse(const sp<RfxMessage>& response) {
    int msg_id = response->getId();

    switch (msg_id) {
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
            if (mForceStopNetworkScan == true) {
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
                            int dsdaMode = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_DSDA_MODE, 0);
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
                    if (data->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_LTE) {
                        // keep nr info
                        isDcNrRestricted = data->nrIndicators.isDcNrRestricted;
                        isEndcAvailable = data->nrIndicators.isEndcAvailable;
                        isNrAvailable = data->nrIndicators.isNrAvailable;
                        logV(NW_CTRL_TAG, "Get LTE cell from data %d, %d, %d",
                            isDcNrRestricted, isEndcAvailable, isNrAvailable);
                    }
                }
            }
            responseToRilj(response);
            }
            break;

        case RFX_MSG_REQUEST_SIGNAL_STRENGTH:
            if (response->getToken() == mSignalReqToken) {
                // trans response to indication, response carries same format
                // of data as indication does.
                logD(NW_CTRL_TAG, "Forced signal strength request responsed");
                void *data = response->getData()->getData();
                int length = response->getData()->getDataLength();
                sp<RfxMessage> urc = RfxMessage::obtainUrc(
                        m_slot_id, RFX_MSG_URC_SIGNAL_STRENGTH, RfxIntsData(data, length));
                responseToRilj(urc);
                mSignalReqToken = 0;
            } else {
                responseToRilj(response);
            }
            break;

        default:
            responseToRilj(response);
            break;
    }
    return true;
}

bool RtcNetworkController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();

    if (msg_id == RFX_MSG_URC_NETWORK_SCAN_RESULT) {
        RfxNetworkScanResultData *ptr = (RfxNetworkScanResultData *)message->getData();
        RIL_NetworkScanResult *data = (RIL_NetworkScanResult *)ptr->getData();
        logV(NW_CTRL_TAG, "mNetworkScanOngoing:%d data->status:%d",
                mNetworkScanOngoing, data->status);
        if (data->status == COMPLETE) { // for now we don't support PARTIAL.
            mNetworkScanOngoing = false;
        }
    }
    responseToRilj(message);
    return true;
}

bool RtcNetworkController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff,int radioState) {
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

bool RtcNetworkController::onPreviewMessage(const sp<RfxMessage>& message) {
    if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_RAT_MODE_SWITCHING) == true &&
            (message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS ||
            message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT ||
            message->getId() == RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN ||
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN ||
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL ||
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT)) {
        logV(NW_CTRL_TAG, "onPreviewMessage, put %s into pending list",
                RFX_ID_TO_STR(message->getId()));
        if (message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS ||
            message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT ||
            message->getId() == RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN) {
            mNetworkScanPending = true;
            if (mCancelNetworkScanPending == true) {
                logV(NW_CTRL_TAG, "onPreviewMessage, resume nw list to cancel");
                return true;
            }
        }
        return false;
    } else {
        if (mNetworkScanPending == true &&
            (message->getId() == RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN ||
            message->getId() == RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS)) {
            logV(NW_CTRL_TAG, "onPreviewMessage, cancel pending netwrok scan",
                    RFX_ID_TO_STR(message->getId()));
            mCancelNetworkScanPending = true;
            return true;
        }
        if (mForceStopNetworkScan == true &&
                message->getId() == RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN) {
            logV(NW_CTRL_TAG, "onPreviewMessage,delay network scan if nw scan stop ongoing!",
                    RFX_ID_TO_STR(message->getId()));
            return false;
        } else if (mStopNetworkScan == true &&
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN) {
            logV(NW_CTRL_TAG, "onPreviewMessage,delay network select if nw scan stop ongoing!",
                    RFX_ID_TO_STR(message->getId()));
        }
        return true;
    }
}

bool RtcNetworkController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    if (getStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_RAT_MODE_SWITCHING) == false &&
            (message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS ||
            message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT ||
            message->getId() == RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN ||
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN ||
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL ||
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT)) {
        logV(NW_CTRL_TAG, "resume the request %s", RFX_ID_TO_STR(message->getId()));
        if (mStopNetworkScan == true &&
            message->getId() == RFX_MSG_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN){
            logV(NW_CTRL_TAG, "onPreviewMessage,delay network select if nw scan stop ongoing!",
                    RFX_ID_TO_STR(message->getId()));
            return false;
        } else {
            return true;
        }
    } else {
        if (mNetworkScanPending == true && mCancelNetworkScanPending == true &&
            (message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS ||
            message->getId() == RFX_MSG_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT ||
            message->getId() == RFX_MSG_RIL_REQUEST_START_NETWORK_SCAN)) {
            logV(NW_CTRL_TAG, "resume netwrok scan to cancel", RFX_ID_TO_STR(message->getId()));
            return true;
        }
        return false;
    }
}

void RtcNetworkController::onHandleModemReset(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    bool old_mode = old_value.asBool();
    bool new_mode = value.asBool();

    logE(NW_CTRL_TAG, "onHandleModemReset:old_mode:%d, new_mode:%d, mNetworkScanOngoing:%d",
            old_mode, new_mode, mNetworkScanOngoing);

    if (new_mode == true && mNetworkScanOngoing == true) { // send plmn list complete when TRM.
        sp<RfxMessage> urcToRilj;
        RIL_NetworkScanResult* resp = (RIL_NetworkScanResult*) calloc(1, sizeof(RIL_NetworkScanResult));
        RFX_ASSERT(resp != NULL);
        memset(resp, 0, sizeof(RIL_NetworkScanResult));
        resp->status = COMPLETE; // for now we don't support PARTIAL.
        urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_URC_NETWORK_SCAN_RESULT,
                RfxNetworkScanResultData((void*) resp, sizeof(RIL_NetworkScanResult)));
        // response to TeleCore
        responseToRilj(urcToRilj);
        free(resp);
        mNetworkScanOngoing = false;
    }
}

void RtcNetworkController::onLocalAbortAvailableNetworkDone() {
    logV(NW_CTRL_TAG, "Local abort done");
    if (mNwScanMessage != NULL) {
        requestToMcl(mNwScanMessage);
    }
    mNwScanMessage = NULL;
}

bool RtcNetworkController::isAPInCall() {
    bool ret = false;

    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        if (getStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0) > 0) {
            ret = true;
        }
    }
    return ret;
}

void RtcNetworkController::onPreferredNwTypeChange(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    int oldMode = old_value.asInt();
    int mode = value.asInt();

    logV(NW_CTRL_TAG, "%s, slotId:%d, key:%d oldMode:%d, mode:%d",
            __FUNCTION__, slotId, key, oldMode, mode);
    int oldNr = (oldMode >= 23 && oldMode <= 33) ? 1 : 0;
    int newNr = (mode >= 23 && mode <= 33) ? 1 : 0;
    if ((oldNr != newNr) &&
        (isDcNrRestricted != 0 || isEndcAvailable != 0 || isNrAvailable != 0)) {
        // nr network type change and the current cell has nr, we need to refresh
        logV(NW_CTRL_TAG, "onPreferredNwTypeChange VOICE_NETWORK_STATE_CHANGED");
        sp<RfxMessage> urcToRilj;
        urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                RfxVoidData());
        responseToRilj(urcToRilj);
    }
}

void RtcNetworkController::onRadioStateChanged(int slotId, RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    int old_state = old_value.asInt();
    int new_state = value.asInt();

    logD(NW_CTRL_TAG, "onRadioStateChanged: old_state:%d, new_state:%d",
            old_state, new_state);

    if (old_state == RADIO_STATE_ON && new_state == RADIO_STATE_OFF
            && mSignalReqToken == 0) {
        // force request signal strength in case radio get off,
        // suppose invalid signal strength should responded.
        sp<RfxMessage> request = RfxMessage::obtainRequest(slotId,
                        RFX_MSG_REQUEST_SIGNAL_STRENGTH, RfxVoidData());
        mSignalReqToken = request->getToken();
        requestToMcl(request);
     }
}


void RtcNetworkController::onHandleNrSwitchSlotChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    int old_slot = old_value.asInt();
    int new_slot = value.asInt();

    logE(NW_CTRL_TAG, "onHandleNrSwitchSlotChanged :old_slot:%d, new_slot:%d",
            old_slot, new_slot);
    bool old_slot_nr = false;
    bool new_slot_nr = false;
    if (old_slot == 0 ||  // no chose
        (old_slot == (m_slot_id+1))) { // chosen
        old_slot_nr = true;
    }

    if (new_slot == 0 ||  // no chose
        (new_slot == (m_slot_id+1))) { // chosen
        new_slot_nr = true;
    }

    if (old_slot_nr != new_slot_nr) {
        // nr slot change and current slot change nr support
        logV(NW_CTRL_TAG, "onHandleNrSwitchSlotChanged send VOICE_NETWORK_STATE_CHANGED");
        sp<RfxMessage> urcToRilj;
        urcToRilj = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                RfxVoidData());
        responseToRilj(urcToRilj);
    }
}

bool RtcNetworkController::isNRSwitching() {
    return getNonSlotScopeStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_NR_SWITCHING, false);
}

void RtcNetworkController::onHandleInCallEvent(RfxStatusKeyEnum key, RfxVariant old_value,
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

void RtcNetworkController::onHandleOccuppyRFEvent(RfxStatusKeyEnum key, RfxVariant old_value,
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

void RtcNetworkController::onHandleDsdaModeEvent(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    int old_mode = old_value.asInt();
    int new_mode = value.asInt();
    int data_slot = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1);
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

void RtcNetworkController::onHandleImsRegistrationRatChangedEvent(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    int old_mode = old_value.asInt();
    int new_mode = value.asInt();
    int data_slot = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM, -1);
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
