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

#include "RmcRatSwitchHandler.h"
#include "RfxMclDispatcherThread.h"
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include <ratconfig.h>

#define RAF_GROUP_GSM (RAF_GSM | RAF_GPRS | RAF_EDGE)
#define RAF_GROUP_HS (RAF_HSUPA | RAF_HSDPA | RAF_HSPA | RAF_HSPAP)
#define RAF_GROUP_WCDMA (RAF_GROUP_HS | RAF_UMTS | RAF_TD_SCDMA)
#define RAF_GROUP_CDMA (RAF_IS95A | RAF_IS95B | RAF_1xRTT)
#define RAF_GROUP_EVDO (RAF_EVDO_0 | RAF_EVDO_A | RAF_EVDO_B | RAF_EHRPD)
#define RAF_GROUP_C2K (RAF_GROUP_CDMA | RAF_GROUP_EVDO)
#define RAF_GROUP_LTE (RAF_LTE | RAF_LTE_CA)
#define RAF_GROUP_NR (RAF_NR)

static const int RILD_ONLY_MAP_INDEX_START = 31;
static const int RAT_INDEX_CDMA = RILD_ONLY_MAP_INDEX_START;
static const int RAT_INDEX_EVDO = RILD_ONLY_MAP_INDEX_START + 1;
static const int RAT_INDEX_CDMA_GSM = RILD_ONLY_MAP_INDEX_START + 2;
static const int RAT_RAF_MAP_COUNT = RAT_INDEX_CDMA_GSM + 1;
static const int RAT_RAF_MAP[RAT_RAF_MAP_COUNT] = {
    (RAF_GROUP_GSM),    // 0
    (RAF_GROUP_WCDMA),  // 1
    (RAF_GROUP_GSM | RAF_GROUP_WCDMA),  // 2
    (RAF_GROUP_LTE),    // 3
    (RAF_GROUP_LTE | RAF_GROUP_GSM),    // 4
    (RAF_GROUP_LTE | RAF_GROUP_WCDMA),  // 5
    (RAF_GROUP_LTE | RAF_GROUP_GSM | RAF_GROUP_WCDMA),  // 6
    (RAF_GROUP_C2K),    // 7
    (RAF_GROUP_GSM | RAF_GROUP_C2K),    // 8
    (RAF_GROUP_WCDMA | RAF_GROUP_C2K), // 9
    (RAF_GROUP_GSM | RAF_GROUP_WCDMA | RAF_GROUP_C2K),  // 10
    (RAF_GROUP_LTE | RAF_GROUP_C2K),    // 11
    (RAF_GROUP_LTE | RAF_GROUP_GSM | RAF_GROUP_C2K),    // 12
    (RAF_GROUP_LTE | RAF_GROUP_WCDMA | RAF_GROUP_C2K),  // 13
    (RAF_GROUP_LTE | RAF_GROUP_GSM | RAF_GROUP_WCDMA | RAF_GROUP_C2K),  // 14
    (RAF_GROUP_NR), // 15
    (RAF_GROUP_NR | RAF_GROUP_GSM), // 16
    (RAF_GROUP_NR | RAF_GROUP_WCDMA),   // 17
    (RAF_GROUP_NR | RAF_GROUP_WCDMA | RAF_GROUP_GSM),   // 18
    (RAF_GROUP_NR | RAF_GROUP_LTE), // 19
    (RAF_GROUP_NR | RAF_GROUP_LTE | RAF_GROUP_GSM), // 20
    (RAF_GROUP_NR | RAF_GROUP_LTE | RAF_GROUP_WCDMA),   // 21
    (RAF_GROUP_NR | RAF_GROUP_LTE | RAF_GROUP_WCDMA | RAF_GROUP_GSM),   // 22
    (RAF_GROUP_NR | RAF_GROUP_C2K), // 23
    (RAF_GROUP_NR | RAF_GROUP_GSM | RAF_GROUP_C2K), // 24
    (RAF_GROUP_NR | RAF_GROUP_WCDMA | RAF_GROUP_C2K),   // 25
    (RAF_GROUP_NR | RAF_GROUP_WCDMA | RAF_GROUP_GSM | RAF_GROUP_C2K),   // 26
    (RAF_GROUP_NR | RAF_GROUP_LTE | RAF_GROUP_C2K), // 27
    (RAF_GROUP_NR | RAF_GROUP_LTE | RAF_GROUP_GSM | RAF_GROUP_C2K), // 28
    (RAF_GROUP_NR | RAF_GROUP_LTE | RAF_GROUP_WCDMA | RAF_GROUP_C2K),   // 29
    (RAF_GROUP_NR | RAF_GROUP_LTE | RAF_GROUP_GSM | RAF_GROUP_WCDMA | RAF_GROUP_C2K),   // 30
    (RAF_GROUP_CDMA),   // RAT_INDEX_CDMA   RID only rat, map to 7, prefer rat 32
    (RAF_GROUP_EVDO),   // RAT_INDEX_EVDO   RID only rat, map to 7, prefer rat 64
    (RAF_GROUP_CDMA | RAF_GROUP_GSM)    // RAT_INDEX_CDMA_GSM   RILD only rat, map to 8, prefer rat 32
};


const int request[] = {
    RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS,
    RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK,
    
    RFX_MSG_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP,
    RFX_MSG_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP,
    RFX_MSG_REQUEST_VOICE_RADIO_TECH,
    RFX_MSG_REQUEST_GET_GMSS_RAT_MODE,
    RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN,
    RFX_MSG_REQUEST_SET_DISABLE_2G,
    RFX_MSG_REQUEST_GET_DISABLE_2G
};

const int event[] = {
};

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_DISABLE_2G);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_DISABLE_2G);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcRatSwitchHandler, RIL_CMD_PROXY_9);

#define MAX_RAT_SWITCH_RETRY_COUNT 109
#define NORMAL_RAT_SWITCH_RETRY_INTERVAL 100
#define MAX_RAT_SWITCH_RETRY_INTERVAL 1000

RmcRatSwitchHandler::RmcRatSwitchHandler(int slot_id, int channel_id) :
        RmcNetworkHandler(slot_id, channel_id) {

    m_slot_id = slot_id;
    m_channel_id = channel_id;
    mLastReqRatType = -1;
    mRetryCount = 1;
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    registerToHandleEvent(event, sizeof(event)/sizeof(int));

    logD(LOG_TAG, "RmcRatSwitchHandler +++");
}

RmcRatSwitchHandler::~RmcRatSwitchHandler() {
}

void RmcRatSwitchHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    // logD(LOG_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(request));
    switch(request) {
        case RFX_MSG_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP:
            requestSetPreferredNetworkType(msg);
            break;
        case RFX_MSG_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP:
            requestGetPreferredNetworkType(msg);
            break;
        case RFX_MSG_REQUEST_VOICE_RADIO_TECH:
            requestVoiceRadioTech(msg);
            break;
        case RFX_MSG_REQUEST_GET_GMSS_RAT_MODE:
            requestGetGmssRatMode(msg);
            break;
        case RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS:
        case RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN:
        case RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK:
            handleAbortPlmnListQuery(msg);
            break;
        case RFX_MSG_REQUEST_SET_DISABLE_2G:
            requestSetDisable2G(msg);
            break;
        case RFX_MSG_REQUEST_GET_DISABLE_2G:
            requestGetDisable2G(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

void RmcRatSwitchHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    switch(id) {

        default:
            logE(LOG_TAG, "should not be here");
            break;
    }
}

bool RmcRatSwitchHandler::isRatPreferred() {
    bool isPreferred = true;

    if (isGcfTestMode() > 0 || isTestSim(m_slot_id) > 0) {
        isPreferred = false;  // no need preferred in test mode/test SIM
    }

    // logD(LOG_TAG, "isTddMD=%d,isWcdmaPreferred=%d,baseband=%d",
    //         isTddMD, isWcdmaPreferred, baseband);

    return isPreferred;
}

void RmcRatSwitchHandler::handleAbortPlmnListQuery(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RIL_Errno ril_errno = RIL_E_SUCCESS;

    logD(LOG_TAG, "requestStopNetworkScan execute while plmnListOngoing=%d", sPlmnListOngoingType);
    if (sPlmnListOngoingType != NW_PLMN_LIST_TYPE_NONE) {
        if (msg->getId() != RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK) { // skip local abort.
            mPlmnListAbort = 1;
        }
        if (sPlmnListOngoingType == NW_PLMN_LIST_TYPE_NORMAL) {
            p_response = atSendCommand("AT+CAPL");
        }
        else {
            p_response = atSendCommand("AT+ENWSCN=2");
        }
        if (p_response->getError() < 0 || p_response->getSuccess() == 0) {
            mPlmnListAbort = 0;
            ril_errno = RIL_E_GENERIC_FAILURE;
            logD(LOG_TAG, "requestStopNetworkScan fail, clear plmnListAbort flag");
        }
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}


bool RmcRatSwitchHandler::isInCall() {
    int ret = false;

    for (int slotId = RFX_SLOT_ID_0; slotId < RfxRilUtils::rfxGetSimCount(); slotId++) {
        if (getMclStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, 0) > 0) {
            ret = true;
            break;
        }
    }

    return ret;
}

void RmcRatSwitchHandler::requestSetPreferredNetworkType(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    int reqRaf, rat = -1, prefer_rat = 0, tempRat = -1;
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    bool isPreferred = false;
    int *pInt = (int *)msg->getData()->getData();
    char optr[RFX_PROPERTY_VALUE_MAX] = {0};

    reqRaf = pInt[0];

    rfx_property_get("persist.vendor.radio.simswitch", optr, "1");
    int currMajorSim = (atoi(optr)-1);
    logD(LOG_TAG, "%s currMajorSim = %d, reqRaf=%d", __FUNCTION__, currMajorSim, reqRaf);

    isPreferred = isRatPreferred();
    if (isPreferred && (reqRaf & RAF_GROUP_NR) != 0) {
        prefer_rat = 128;  // NR preferred
    } else if (isPreferred && (reqRaf & RAF_GROUP_LTE) != 0) {
        prefer_rat = 4; // LTE preferred
        }
    for (int i = 0; i < RAT_RAF_MAP_COUNT; i++) {
        if (reqRaf == RAT_RAF_MAP[i]) {
            tempRat = i;
            break;
            }
            }
    switch (tempRat) {
        case RAT_INDEX_CDMA:
            rat = 7;
            prefer_rat = 32;
            break;
        case RAT_INDEX_EVDO:
            rat = 7;
            prefer_rat = 64;
            break;
        case RAT_INDEX_CDMA_GSM:
            rat = 8;
            prefer_rat = 32;
            break;
        default:
            rat = tempRat;
            break;
    }

    if (rat >= 0) {
        if (getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false) == true) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            logE(LOG_TAG, "SetPreferredNetworkType: Skip retry in radio off");
            goto finish;
        } else if (isInCall() == true) {
            ril_errno = RIL_E_OPERATION_NOT_ALLOWED;
            logE(LOG_TAG, "requestSetPreferredNetworkType: Skip retry in call");
            goto finish;
        }
        // send AT command
        p_response = atSendCommand(String8::format("AT+ERAT=%d,%d", rat, prefer_rat));
        if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
            logV(LOG_TAG, "SetPreferredNetworkType: ok, count=%d, MajorSim=%d, Preferred=%d",
                    mRetryCount, currMajorSim, isPreferred);
            mLastReqRatType = reqRaf;
            ril_errno = RIL_E_SUCCESS;
        } else {
            logE(LOG_TAG, "SetPreferredNetworkType: fail, count=%d, error=%d, MajorSim=%d",
                    mRetryCount, p_response->atGetCmeError(), currMajorSim);
            if (p_response->atGetCmeError() == CME_OPERATION_NOT_SUPPORTED) {
                ril_errno = RIL_E_MODE_NOT_SUPPORTED;
                logE(LOG_TAG, "SetPreferredNetworkType: fail, modem not support dual C2K RAT");
            } else {  // retry 109 times if set rat fail.
                if (mRetryCount < MAX_RAT_SWITCH_RETRY_COUNT) {
                    mRetryCount++;
                    int retryTime;
                    if (mRetryCount <= 10) { // 1s
                        retryTime = NORMAL_RAT_SWITCH_RETRY_INTERVAL;
                    } else if (mRetryCount <= 20) { // 2s
                        retryTime = NORMAL_RAT_SWITCH_RETRY_INTERVAL*2;
                    } else if (mRetryCount <= 57) { // 37s
                        retryTime = MAX_RAT_SWITCH_RETRY_INTERVAL;
                    } else { // 52*5s
                        retryTime = MAX_RAT_SWITCH_RETRY_INTERVAL*5;
                    }
                    sp<RfxMclMessage> mclMessage = RfxMclMessage::obtainRequest(msg->getId(),
                            msg->getData(), msg->getSlotId(), msg->getToken(),
                            msg->getSendToMainProtocol(), msg->getRilToken(), ms2ns(retryTime),
                            msg->getTimeStamp(), msg->getAddAtFront());
                    RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
                    return;
                }
                if (p_response->atGetCmeError() == CME_OPERATION_NOT_ALLOWED_ERR) {
                    ril_errno = RIL_E_OPERATION_NOT_ALLOWED;
                }
            }
        }
        p_response = NULL;
    }

finish:
    mRetryCount = 1;
    getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_RAT_MODE_SWITCHING, false);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmcRatSwitchHandler::requestGetPreferredNetworkType(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    int err, skip, nt_type, prefer_type = 0, return_type;
    int mapIndex = -1;
    RfxAtLine* line1;
    sp<RfxMclMessage> response;

    if (mLastReqRatType > -1) {
        return_type = mLastReqRatType;
        goto queryDone;
    }

    p_response = atSendCommandSingleline("AT+ERAT?", "+ERAT:");

    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0)
        goto error;

    line1 = p_response->getIntermediates();

    // go to start position
    line1->atTokStart(&err);
    if(err < 0) goto error;

    //skip <curr_rat>
    skip = line1->atTokNextint(&err);
    if(err < 0) goto error;

    //skip <gprs_status>
    skip = line1->atTokNextint(&err);
    if(err < 0) goto error;

    //get <rat>
    nt_type = line1->atTokNextint(&err);
    if(err < 0) goto error;

    //get <prefer rat>
    prefer_type = line1->atTokNextint(&err);
    if(err < 0) goto error;

    if (nt_type == 7 && prefer_type == 32) {
        mapIndex = RAT_INDEX_CDMA;
    } else if (nt_type == 7 && prefer_type == 64) {
        mapIndex = RAT_INDEX_EVDO;
    } else if (nt_type == 8 && prefer_type == 32) {
        mapIndex = RAT_INDEX_CDMA_GSM;
    } else if (nt_type >= 0 && nt_type < RILD_ONLY_MAP_INDEX_START){
        mapIndex = nt_type;
    }
    if (mapIndex < 0) goto error;
    return_type = RAT_RAF_MAP[mapIndex];

queryDone:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&return_type, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcRatSwitchHandler::requestVoiceRadioTech(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    // do nothing
}

void RmcRatSwitchHandler::requestGetGmssRatMode(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    int err = 0;
    int data[5] = { 0 };
    RfxAtLine* line = NULL;

    p_response = atSendCommandSingleline("AT+EGMSS?", "+EGMSS:");

    if (p_response == NULL
            || p_response->getError() != 0
            || p_response->getSuccess() == 0
            || p_response->getIntermediates() == NULL) {
        goto error;
    }

    line = p_response->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    for (int i = 0; i < 5; i++) {
        if (i == 1) {  // MCC
            data[i] = atoi(line->atTokNextstr(&err));
        } else {
            data[i] = line->atTokNextint(&err);
        }
        if (err < 0) {
            goto error;
        }
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(data, 5), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(LOG_TAG, "requestGetGmssRatMode error");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmcRatSwitchHandler::requestSetDisable2G(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int mode = 0;

    int *pInt = (int *)msg->getData()->getData();
    mode = pInt[0];
    if (mode == 1 || mode == 0) {
        if (mode == 0) {
            rfx_property_get("persist.vendor.radio.disable.2g", property_value, "0");
            if (atoi(property_value) == 1) {
                p_response = atSendCommand(String8::format("AT+EDRAT=%d", mode));
                if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
                    rfx_property_set("persist.vendor.radio.disable.2g", String8::format("%d", mode).string());
                    ril_errno = RIL_E_SUCCESS;
                }
                goto done;
            }
        }
        if (isOp07Support()) {
            logD(LOG_TAG, "[OP07] requestSetDisable2G %d", mode);
            p_response = atSendCommand(String8::format("AT+EDRAT=%d", mode));
            if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
                rfx_property_set("persist.vendor.radio.disable.2g",
                        String8::format("%d", mode).string());
                ril_errno = RIL_E_SUCCESS;
            }
        } else if (isOp08Support()) {
            logD(LOG_TAG, "[OP08] requestSetDisable2G %d", mode);
            p_response = atSendCommand(String8::format("AT+EGMC=1,\"rat_disable_with_ecc\",%d", mode));
            if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
                ril_errno = RIL_E_SUCCESS;
            }
        } else {
            logD(LOG_TAG, "NOT SUPPORT, requestSetDisable2G %d", mode);
        }
    }
done:
    p_response = NULL;
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcRatSwitchHandler::requestGetDisable2G(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    int err=0, mode=0;
    char *type=NULL;
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};

    if (isOp07Support()) {
        rfx_property_get("persist.vendor.radio.disable.2g", property_value, "1");
        mode = atoi(property_value);
        logD(LOG_TAG, "[OP07] requestGetDisable2G %d", mode);
        ril_errno = RIL_E_SUCCESS;
    } else if (isOp08Support()) {
        p_response = atSendCommandSingleline("AT+EGMC=0,\"rat_disable_with_ecc\"", "+EGMC:");

        err = p_response->getError();
        if (err < 0 || p_response->getSuccess() == 0)
            goto error;

        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if(err < 0) goto error;

        // get "type"
        type = line->atTokNextstr(&err);
        if(err < 0) goto error;

        if (strncmp(type, "rat_disable_with_ecc", strlen("rat_disable_with_ecc")) != 0) {
            goto error;
        }

        // get <mode>
        mode = line->atTokNextint(&err);
        if(err < 0) goto error;

        logD(LOG_TAG, "[OP08] requestGetDisable2G %d", mode);
        ril_errno = RIL_E_SUCCESS;
    } else {
        logD(LOG_TAG, "NOT SUPPORT, requestGetDisable2G %d", mode);
    }

error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno,
            RfxIntsData(&mode, 1), msg, false);
    responseToTelCore(resp);
}

