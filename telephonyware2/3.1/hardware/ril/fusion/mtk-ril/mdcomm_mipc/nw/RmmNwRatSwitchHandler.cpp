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

#include "RmmCapabilitySwitchUtil.h"
#include "RmmNwRatSwitchHandler.h"
#include "rfx_properties.h"
#include "libmtkrilutils.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmmNwRatSwHdlr"

#define VOLTE_REGISTERATION_STATE 0x01
#define PROPERTY_MTK_HVOLTE_INDICATOR "vendor.ril.mtk_hvolte_indicator"

#define RAF_GROUP_GSM (RAF_GSM | RAF_GPRS | RAF_EDGE)
#define RAF_GROUP_HS (RAF_HSUPA | RAF_HSDPA | RAF_HSPA | RAF_HSPAP)
#define RAF_GROUP_WCDMA (RAF_GROUP_HS | RAF_UMTS | RAF_TD_SCDMA)
#define RAF_GROUP_CDMA (RAF_IS95A | RAF_IS95B | RAF_1xRTT)
#define RAF_GROUP_EVDO (RAF_EVDO_0 | RAF_EVDO_A | RAF_EVDO_B | RAF_EHRPD)
#define RAF_GROUP_C2K (RAF_GROUP_CDMA | RAF_GROUP_EVDO)
#define RAF_GROUP_LTE (RAF_LTE | RAF_LTE_CA)
#define RAF_GROUP_NR (RAF_NR)

static char PROPERTY_RILJ_NW_TYPE[4][40] = {
    "persist.vendor.radio.rilj_nw_type1",
    "persist.vendor.radio.rilj_nw_type2",
    "persist.vendor.radio.rilj_nw_type3",
    "persist.vendor.radio.rilj_nw_type4",
};

static const char PROPERTY_TEST_SIM[4][35] = {
    "vendor.gsm.sim.ril.testsim",
    "vendor.gsm.sim.ril.testsim.2",
    "vendor.gsm.sim.ril.testsim.3",
    "vendor.gsm.sim.ril.testsim.4",
};

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
    RFX_MSG_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP,
    RFX_MSG_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP,
    RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS,
    RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK,
    RFX_MSG_REQUEST_SET_PREFERRED_NETWORK_TYPE,
    RFX_MSG_REQUEST_GET_PREFERRED_NETWORK_TYPE,
    RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN,
    RFX_MSG_REQUEST_UPDATE_PHONE_MODE,
};

const int event[] = {
    RFX_MSG_EVENT_CURRENT_RAT_CHANGED,
    RFX_MSG_EVENT_EGMSS_INFO_CHANGED,
};

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_PREFERRED_NETWORK_TYPE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_PREFERRED_NETWORK_TYPE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_VOICE_RADIO_TECH_CHANGED);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_CURRENT_RAT_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_EGMSS_INFO_CHANGED);


// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmNwRatSwitchHandler, RIL_CMD_PROXY_9);

RmmNwRatSwitchHandler::RmmNwRatSwitchHandler(int slot_id, int channel_id) :
        RmmNwHandler(slot_id, channel_id),
        mCurRatBit(0),
        mCurPreferRat(0),
        mPhoneMode(RADIO_TECH_UNKNOWN) {
    m_slot_id = slot_id;
    m_channel_id = channel_id;
    mDisable2GWithRat = isDisable2GWithRatConfigured();
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    registerToHandleEvent(event, sizeof(event)/sizeof(int));
    setNwsMode(NWS_MODE_CSFB);
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_PREFERRED_NW_TYPE, -1);
}

RmmNwRatSwitchHandler::~RmmNwRatSwitchHandler() {
}

void RmmNwRatSwitchHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    // logD(LOG_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(request));
    switch(request) {
        case RFX_MSG_REQUEST_SET_PREFERRED_NETWORK_TYPE:
        case RFX_MSG_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP:
            handleSetAllowedNetworkTypeRequest(msg);
            break;
        case RFX_MSG_REQUEST_GET_PREFERRED_NETWORK_TYPE:
        case RFX_MSG_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP:
            handleGetAllowedNetworkTypeRequest(msg);
            break;
        case RFX_MSG_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS:
        case RFX_MSG_RIL_REQUEST_STOP_NETWORK_SCAN:
        case RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK:
            handleAbortPlmnListQuery(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_PHONE_MODE:
            handleUpdatePhoneModeRequest(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

void RmmNwRatSwitchHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_SET_PREFERRED_NETWORK_TYPE:
        case RFX_MSG_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP:
            logV(LOG_TAG, "response of SET_ALLOWED_NETWORK_TYPE, result[%d]", msg->getMipcData()->getResult());
            handleVoidResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_PREFERRED_NETWORK_TYPE:
        case RFX_MSG_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP:
            handleGetAllowedNetworkTypeResponse(msg);
            break;

        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

void RmmNwRatSwitchHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    switch(id) {
        case RFX_MSG_EVENT_CURRENT_RAT_CHANGED:
            handleCurrentRatChangedEvent(msg);
            break;
        case RFX_MSG_EVENT_EGMSS_INFO_CHANGED:
            handleEgmssInfoChangedEvent(msg);
            break;
        default:
            logE(LOG_TAG, "should not be here");
            break;
    }
}

bool RmmNwRatSwitchHandler::isRatPreferred() {
    if (isGcfTestMode() > 0 || isTestSim(m_slot_id) > 0) {
        return false;  // no need preferred in test mode/test SIM
    }
    return true;
}

void RmmNwRatSwitchHandler::handleSetPreferredNetworkTypeRequest(const sp<RfxMclMessage>& msg) {
    uint8_t rat = 0xff, prefer_rat = 0;
    bool isPreferred = isRatPreferred();
    int *pInt = (int *)msg->getData()->getData();
    int req_type = pInt[0];

    disable2GOption(req_type);// sp only
    setIntSysProp(PROPERTY_RILJ_NW_TYPE[m_slot_id], req_type);

    if (getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false) == true) {
        logD(LOG_TAG, "handleSetPreferredNetworkTypeRequest, power off, send error response");
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
        return;
    }

    if (isPreferred && req_type > PREF_NET_TYPE_NR_ONLY &&
            req_type <= PREF_NET_TYPE_NR_LTE_TDSCDMA_CDMA_EVDO_GSM_WCDMA) {
        prefer_rat = 128;  // NR preferred
    }

    switch(req_type)
    {
        case PREF_NET_TYPE_GSM_WCDMA_AUTO:
            rat = 2;  // 2/3G AUTO
            break;
        case PREF_NET_TYPE_GSM_WCDMA:
        case PREF_NET_TYPE_TD_SCDMA_GSM:
        case PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA:
            rat = 2;  // 2/3G AUTO
            if(isPreferred){
                prefer_rat = 2;  // 3G preferred
            }
            break;
        case PREF_NET_TYPE_GSM_ONLY:
            rat = 0;  // 2G only
            break;
        case PREF_NET_TYPE_WCDMA:
        case PREF_NET_TYPE_TD_SCDMA_ONLY:
        case PREF_NET_TYPE_TD_SCDMA_WCDMA:
            rat = 1;  // 3G only
            break;
        case PREF_NET_TYPE_LTE_GSM_WCDMA:
        case PREF_NET_TYPE_TD_SCDMA_GSM_LTE:
        case PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA_LTE:
            rat = 6;  // 2/3/4G AUTO
            if (isPreferred) {
                prefer_rat = 4;  //4G preferred
            }
            break;
        case PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA:
        case PREF_NET_TYPE_TD_SCDMA_LTE_CDMA_EVDO_GSM_WCDMA:
            rat = 14;  // LTE CDMA EVDO GSM/WCDMA mode
            if (isPreferred) {
                prefer_rat = 4;  //4G preferred
            }
            break;
        case PREF_NET_TYPE_LTE_CDMA_EVDO_GSM:
            rat = 12;  // LTE CDMA EVDO GSM mode
            break;
        case PREF_NET_TYPE_LTE_ONLY:
            rat = 3;  // LTE only for EM mode
            break;
        case PREF_NET_TYPE_LTE_WCDMA:
        case PREF_NET_TYPE_TD_SCDMA_LTE:
        case PREF_NET_TYPE_TD_SCDMA_WCDMA_LTE:
            rat = 5;  // LTE/WCDMA for EM mode
            break;
        case PREF_NET_TYPE_LTE_GSM:
            rat = 4;  // 2/4G
            break;
        case PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO:
        case PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA_CDMA_EVDO_AUTO:
            rat = 10;  // GSM/WCDMA/C2K mode
            break;
        case PREF_NET_TYPE_CDMA_EVDO_GSM:
            rat = 8;  // CDMA EVDO GSM mode
            break;
        case PREF_NET_TYPE_CDMA_GSM:
            rat = 8;  // CDMA GSM mode
            prefer_rat = 32;
            break;
        case PREF_NET_TYPE_LTE_CDMA_EVDO:
            rat = 11;   // LTE/C2K mode
            break;
        case PREF_NET_TYPE_CDMA_EVDO_AUTO:
            rat = 7;    // C2K 1x/Evdo
            break;
        case PREF_NET_TYPE_CDMA_ONLY:
            rat = 7;    // C2K 1x/Evdo
            prefer_rat = 32;  // C2K 1x only
            break;
        case PREF_NET_TYPE_EVDO_ONLY:
            rat = 7;    // C2K 1x/Evdo
            prefer_rat = 64;  // C2K Evdo only
            break;
        case PREF_NET_TYPE_NR_ONLY:
            rat = 15;    // NR only
            break;
        case PREF_NET_TYPE_NR_LTE:
            rat = 19;    // NR/LTE
            break;
        case PREF_NET_TYPE_NR_LTE_CDMA_EVDO:
            rat = 27;    // NR/LTE/C2K
            break;
        case PREF_NET_TYPE_NR_LTE_GSM_WCDMA:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM_WCDMA:
            rat = 22;    // NR/LTE/GSM/WCDMA(TDS-CDMA)
            break;
        case PREF_NET_TYPE_NR_LTE_CDMA_EVDO_GSM_WCDMA:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_CDMA_EVDO_GSM_WCDMA:
            rat = 30;    // NR/LTE/C2K/GSM/WCDMA(TDS-CDMA)
            break;
        case PREF_NET_TYPE_NR_LTE_WCDMA:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA:
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_WCDMA:
            rat = 21;    //NR/LTE/WCDMA(TDS-CDMA)
            break;
        default:
            break;
    }
    logD(LOG_TAG, "handleSetPreferredNetworkTypeRequest: rat = %d, prefer_rat = %d", rat, prefer_rat);
    if (rat != 0xff) {
        sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_RAT_REQ, msg->getSlotId());
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_RAT_REQ_T_RAT, rat);
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_RAT_REQ_T_PREFER_RAT, prefer_rat);
        callToMipcMsgAsync(msg, mipcReq);
    } else {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODE_NOT_SUPPORTED,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
    }

}

void RmmNwRatSwitchHandler::handleSetAllowedNetworkTypeRequest(const sp<RfxMclMessage>& msg) {

    if (getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false) == true) {
        logD(LOG_TAG, "handleSetAllowedNetworkTypeRequest, power off, send error response");
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        // response to TeleCore
        responseToTelCore(response);
        return;
    }

    uint8_t rat = 0xff, prefer_rat = 0, tempRat = 0xff;
    bool isPreferred = isRatPreferred();
    int *pInt = (int *)msg->getData()->getData();
    int req_type = pInt[0];

    setIntSysProp(PROPERTY_RILJ_NW_TYPE[m_slot_id], req_type);
    disable2GOption(req_type);// sp only
    int adjustedRaf = adjustRaf(req_type);
    if (isPreferred && (adjustedRaf & RAF_GROUP_NR) != 0) {
        prefer_rat = 128;  // NR preferred
    } else if (isPreferred && (adjustedRaf & RAF_GROUP_LTE) != 0) {
        prefer_rat = 4; // LTE preferred
    }
    for (int i = 0; i < RAT_RAF_MAP_COUNT; i++) {
        if (adjustedRaf == RAT_RAF_MAP[i]) {
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

    logD(LOG_TAG, "[%s]: rat = %d, prefer_rat = %d", __FUNCTION__, rat, prefer_rat);
    if (rat != 0xff) {
        sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_SET_RAT_REQ, msg->getSlotId());
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_RAT_REQ_T_RAT, rat);
        mipcReq->addMipcTlvUint8(MIPC_NW_SET_RAT_REQ_T_PREFER_RAT, prefer_rat);
        callToMipcMsgAsync(msg, mipcReq);
    } else {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODE_NOT_SUPPORTED,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
    }

}

void RmmNwRatSwitchHandler::handleGetAllowedNetworkTypeRequest(const sp<RfxMclMessage>& msg) {
    logD(LOG_TAG, "handleGetPreferredNetworkTypeRequest");
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_RAT_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, mipcReq);
}

void RmmNwRatSwitchHandler::handleGetPreferredNetworkTypeResponse(const sp<RfxMclMessage>& msg) {
    // TODO: return cached network type ??
    uint8_t nt_type = 0, prefer_type = 0;
    int return_type;
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    if (result != MIPC_RESULT_SUCCESS) goto error;

    nt_type = mipcData->getMipcUint8Val(MIPC_NW_GET_RAT_CNF_T_RAT_MODE, 0);
    prefer_type = mipcData->getMipcUint8Val(MIPC_NW_GET_RAT_CNF_T_PREFER_RAT, 0);

    return_type = convertNetworkType(nt_type, prefer_type);
    if (return_type < 0) goto error;
    logD(LOG_TAG, "handleGetPreferredNetworkTypeResponse, rat=%d, prefer_rat=%d, nt_type=%d",
        nt_type, prefer_type, return_type);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&return_type, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logD(LOG_TAG, "handleGetPreferredNetworkTypeResponse error, result = %s",
        result == MIPC_RESULT_SUCCESS ? "success" : "fail");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRatSwitchHandler::handleGetAllowedNetworkTypeResponse(const sp<RfxMclMessage>& msg) {
    uint8_t nt_type = 0, prefer_type = 0;
    int return_type = -1;
    int mapIndex = -1;
    int ntTypeRilj = getIntSysProp(PROPERTY_RILJ_NW_TYPE[m_slot_id], -1);
    sp<RfxMclMessage> response;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    if (result != MIPC_RESULT_SUCCESS) goto error;

    nt_type = mipcData->getMipcUint8Val(MIPC_NW_GET_RAT_CNF_T_RAT_MODE, 0);
    prefer_type = mipcData->getMipcUint8Val(MIPC_NW_GET_RAT_CNF_T_PREFER_RAT, 0);
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
    return_type = return_type & ntTypeRilj;
    logD(LOG_TAG, "%s, rat=%d, prefer_rat=%d, nt_type=%d", __FUNCTION__, nt_type, prefer_type, return_type);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&return_type, 1), msg, false);
    responseToTelCore(response);
    return;

error:
    logD(LOG_TAG, "%s error, result = %s", __FUNCTION__, result == MIPC_RESULT_SUCCESS ? "success" : "fail");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwRatSwitchHandler::handleAbortPlmnListQuery(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> resp;
    RIL_Errno radioError = RIL_E_SUCCESS;

    logD(LOG_TAG, "handleAbortPlmnListQuery plmnListOngoing[%d]", sPlmnListOngoingType);
    if (sPlmnListOngoingType) {
        if (msg->getId() != RFX_MSG_REQUEST_LOCAL_ABORT_AVAILABLE_NETWORK)
            sPlmnListAbort = true;

        sp<RfxMipcData> reqData = NULL;
        if (sPlmnListOngoingType == NW_PLMN_LIST_TYPE_NORMAL) {
            reqData = RfxMipcData::obtainMipcData(
                    MIPC_NW_SET_ABORT_PLMN_LIST_SEARCH_REQ, msg->getSlotId());
        } else {
            reqData = RfxMipcData::obtainMipcData(
                    MIPC_NW_STOP_NETWORK_SCAN_REQ, msg->getSlotId());
        }
        sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
        if (cnfData->getData() == NULL
            || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
            logD(LOG_TAG, "handleStopNetworkScan fail");
            sPlmnListAbort = false;
            radioError = RIL_E_GENERIC_FAILURE;
        }
    }
    resp = RfxMclMessage::obtainResponse(msg->getId(),
            radioError, RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmmNwRatSwitchHandler::handleUpdatePhoneModeRequest(const sp<RfxMclMessage>& msg) {

    int *pInt = (int *)msg->getData()->getData();
    int updateType = pInt[0];
    logD(LOG_TAG, "handleUpdatePhoneModeRequest, updateType[%d]", updateType);

    if (updateType == 1) {
        if (isCurrentRatContainGsm() && isCurrentRatContainCdma()) {
            updateGmssRatMode();
        } else {
            setNwsMode(isCurrentRatContainCdma() == 1 ? NWS_MODE_CDMALTE : NWS_MODE_CSFB);
        }
    } else if (updateType == 2) {
        if (isCurrentRatContainGsm() && isCurrentRatContainCdma() && isCT4GCard()) {
            // volte status is changed and turn on the volte
            setNwsMode(NWS_MODE_CSFB);
        }
    }
    updatePhoneMode(GMSS_RAT);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&mPhoneMode, 1), msg, false);
    responseToTelCore(response);

}

void RmmNwRatSwitchHandler::handleCurrentRatChangedEvent(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int nt_type = pInt[0];
    int nt_type_bit = pInt[1];
    int prefer_type = pInt[2];
    if (nt_type == 0xFF) {
        logD(LOG_TAG, "handleCurrentRatChangedEvent, invalid rat, ignore!!!");
        return;
    }

    logD(LOG_TAG, "%s, nt_type=%d, prefer_type=%d, bit_rat=0x%02X", __FUNCTION__, nt_type, prefer_type, nt_type_bit);
    if (mCurRatBit != nt_type_bit || mCurPreferRat != prefer_type) {
        mCurRatBit = nt_type_bit;
        mCurPreferRat = prefer_type;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_PREFERRED_NW_TYPE, convertNetworkType(nt_type, prefer_type));
        if (isCurrentRatContainGsm() && isCurrentRatContainCdma()) {
            updateGmssRatMode();
            updatePhoneMode(GMSS_RAT);
        } else {
            setNwsMode(isCurrentRatContainCdma() == 1 ? NWS_MODE_CDMALTE : NWS_MODE_CSFB);
            updatePhoneMode(SWITCH_RAT);
        }
    }
}

void RmmNwRatSwitchHandler::handleEgmssInfoChangedEvent(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    GmssInfo gmssInfo;
    gmssInfo.rat = pInt[0];
    gmssInfo.mcc = pInt[1];
    gmssInfo.status = pInt[2];
    gmssInfo.mspl = pInt[3];
    gmssInfo.ishome = pInt[4];
    logD(LOG_TAG, "handleEgmssInd");
    handleGmssRat(gmssInfo);
    updatePhoneMode(GMSS_RAT);
}

void RmmNwRatSwitchHandler::updateGmssRatMode() {
    if (isGsmOnlySim()) {
        setNwsMode(NWS_MODE_CSFB);
        logD(LOG_TAG, "updateGmssRatMode, GSM only SIM, NO NEED!!");
        return;
    }
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(MIPC_NW_GET_EGMSS_REQ, m_slot_id);
    sp<RfxMipcData> mipcCnf = callToMipcMsgSync(mipcReq);
    if (mipcCnf->getResult() != MIPC_RESULT_SUCCESS) {
        logD(LOG_TAG, "updateGmssRatMode, L5 returns FAIL!!");
        return;
    }
    GmssInfo gmssInfo;
    gmssInfo.rat = mipcCnf->getMipcUint32Val(MIPC_NW_GET_EGMSS_CNF_T_RAT, 0);
    gmssInfo.mcc = mipcCnf->getMipcUint32Val(MIPC_NW_GET_EGMSS_CNF_T_MCC, 0);
    gmssInfo.status = mipcCnf->getMipcUint32Val(MIPC_NW_GET_EGMSS_CNF_T_STATUS, 0);
    gmssInfo.mspl = mipcCnf->getMipcUint32Val(MIPC_NW_GET_EGMSS_CNF_T_CUR_REPORTED_RAT, 0);
    gmssInfo.ishome = mipcCnf->getMipcUint32Val(MIPC_NW_GET_EGMSS_CNF_T_IS_HOME_COUNTRY, 0);
    handleGmssRat(gmssInfo);
}

void RmmNwRatSwitchHandler::handleGmssRat(GmssInfo gmssInfo) {
    if (isGsmOnlySim()) {
        setNwsMode(NWS_MODE_CSFB);
        logD(LOG_TAG, "handleGmssRat, gsm only sim, ignore!!");
        return;
    }
    int volteRegistration = 0;

    if (gmssInfo.rat == EXTEND_GMSS_RAT_INVALID
            && gmssInfo.mcc == 0
            && gmssInfo.status == GMSS_STATUS_SELECT
            && gmssInfo.mspl == MSPL_RAT_NONE
            && gmssInfo.ishome == false) {
        // logD(RAT_CTRL_TAG, "[handleGmssRat] invalid value");
        return;
    }
    bool hvolteIndicator = false;
    if (gmssInfo.status == GMSS_STATUS_ECC) {
        if (RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_C2K1X)
                || RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_C2KHRPD)
                || RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_C2KHRPD | EXTEND_GMSS_RAT_C2K1X)) {
            setNwsMode(NWS_MODE_CDMALTE);
        } else if (RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_GSM)
                || RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_WCDMA)) {
            setNwsMode(NWS_MODE_CSFB);
        } else {
            if (gmssInfo.ishome == false ||
                    isCdmaDualModeSimCard() == false ||
                    RmmCapabilitySwitchUtil::isDisableC2kCapability()) {
                setNwsMode(NWS_MODE_CSFB);
            } else {
                setNwsMode(NWS_MODE_CDMALTE);
            }
        }
    } else {
        if (RFX_FLAG_HAS(gmssInfo.mspl, MSPL_RAT_C2K) &&
                (RFX_FLAG_HAS(gmssInfo.mspl, MSPL_RAT_GSM)
                || RFX_FLAG_HAS(gmssInfo.mspl, MSPL_RAT_UMTS))) {
            if (gmssInfo.status == GMSS_STATUS_ATTACHED) {
                if (RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_C2K1X)
                        || RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_C2KHRPD)
                        || RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_C2KHRPD | EXTEND_GMSS_RAT_C2K1X)) {
                    setNwsMode(NWS_MODE_CDMALTE);
                } else if (RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_GSM)
                        || RFX_FLAG_HAS_ALL(gmssInfo.rat, EXTEND_GMSS_RAT_WCDMA)) {
                    setNwsMode(NWS_MODE_CSFB);
                } else {
                    // EUTRAN (LTE)
                    setNwsMode(gmssInfo.ishome ? NWS_MODE_CDMALTE : NWS_MODE_CSFB);
                }
            }
        } else if (RFX_FLAG_HAS(gmssInfo.mspl, MSPL_RAT_C2K)) {
            if (RfxRilUtils::isCtHvolteSupport() && gmssInfo.mcc == 460 &&
                isCurrentRatContainGsm() && isCurrentRatContainCdma()) {
                volteRegistration = getMclStatusManager()->getIntValue(
                    RFX_STATUS_KEY_IMS_REGISTRATION_STATE, 0);
                if ((volteRegistration & VOLTE_REGISTERATION_STATE)
                        == VOLTE_REGISTERATION_STATE) {
                    setNwsMode(NWS_MODE_CSFB);
                } else {
                    setNwsMode(NWS_MODE_CDMALTE);
                }
                hvolteIndicator = true;
            } else {
                setNwsMode(NWS_MODE_CDMALTE);
            }
        } else {
            setNwsMode(NWS_MODE_CSFB);
        }
    }
    setMSimProperty(m_slot_id, (char *)PROPERTY_MTK_HVOLTE_INDICATOR, (char*)(hvolteIndicator ? "1" : "0"));
    logD(LOG_TAG, "[handleGmssRat] rat:%d, mcc:%d, status:%d, mspl:%d, ishome:%d, NWS mode:%d,"
        "volteRegistration: %d, hvolteIndicator: %d", gmssInfo.rat, gmssInfo.mcc, gmssInfo.status, gmssInfo.mspl,
        gmssInfo.ishome, mNwsMode, volteRegistration, hvolteIndicator);
}

void RmmNwRatSwitchHandler::updatePhoneMode(PHONE_CHANGE_SOURCE source) {
    if (source != GMSS_RAT) {
        setMSimProperty(m_slot_id, (char *)PROPERTY_MTK_HVOLTE_INDICATOR, (char*) "0");
    }
    int tech = RADIO_TECH_UNKNOWN;

    if (mCurRatBit == RAT_UMTS) {
            tech = RADIO_TECH_UMTS;
    } else if (mCurRatBit == RAT_LTE || mCurRatBit == RAT_NR || mCurRatBit == (RAT_NR | RAT_LTE)) {
            if (mNwsMode == NWS_MODE_CSFB) {
                tech = RADIO_TECH_GPRS;
            }
    } else if (((mCurRatBit & (RAT_UMTS | RAT_GSM)) > 0) && !isCurrentRatContainCdma()) {
            tech = RADIO_TECH_GPRS;
    } else if (isCurrentRatContainCdma() && !isCurrentRatContainGsm()) {
            tech = RADIO_TECH_1xRTT;
    } else if (isCurrentRatContainCdma() && isCurrentRatContainGsm()) {
            tech = RADIO_TECH_1xRTT;
            if (mNwsMode == NWS_MODE_CSFB) {
                tech = RADIO_TECH_GPRS;
            }
    } else {
        logW(LOG_TAG, "[%s] unSupported Nw rat BIT: %d", __FUNCTION__, mCurRatBit);
    }

    if (tech != RADIO_TECH_UNKNOWN && mPhoneMode != tech) {
        mPhoneMode = tech;
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_VOICE_RADIO_TECH_CHANGED, m_slot_id,
            RfxIntsData(&mPhoneMode, 1));
        responseToTelCore(urc);
        logD(LOG_TAG, "[updatePhoneMode] mPhoneMode: %d", mPhoneMode);
    }
}

bool RmmNwRatSwitchHandler::isCurrentRatContainGsm() {
    return mCurRatBit & RAT_GSM_BIT_GROUP;
}

bool RmmNwRatSwitchHandler::isCurrentRatContainCdma() {
    return mCurRatBit & RAT_CDMA_BIT_GROUP;
}

bool RmmNwRatSwitchHandler::isCdmaDualModeSimCard() {
    if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD)) {
        logD(LOG_TAG, "isCdmaDualModeSimCard, is CT3G dual mode card");
        return true;
    } else {
        int nCardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
        bool ret = false;
        if (nCardType == -1) {//default value is -1
            logD(LOG_TAG, "isCdmaDualModeSimCard, nCardType=0x%x, ret = %d", nCardType, ret);
            return ret;
        }
        if (RFX_FLAG_HAS_ALL(nCardType, (RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_CSIM)) ||
                RFX_FLAG_HAS_ALL(nCardType, (RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_RUIM))) {
            logD(LOG_TAG, "isCdmaDualModeSimCard, nCardType=0x%x, ret = %d", nCardType, ret);
            ret = true;
        }
        return ret;
    }
}

bool RmmNwRatSwitchHandler::isGsmOnlySim() {
    bool ret = false;
    int nCardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);

    if ((nCardType == RFX_CARD_TYPE_SIM
            || nCardType == RFX_CARD_TYPE_USIM
            || nCardType == (RFX_CARD_TYPE_SIM | RFX_CARD_TYPE_ISIM)
            || nCardType == (RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_ISIM))
            && (!isCdmaDualModeSimCard())) {
        ret = true;
    }

    // logD(RAT_CTRL_TAG, "[isGsmOnlySim] GSM only: %s", ret ? "true" : "false");
    return ret;
}

bool RmmNwRatSwitchHandler::isCT4GCard() {
    int nCardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE);
    return CT_4G_UICC_CARD == nCardType ? true : false;
}

void RmmNwRatSwitchHandler::setNwsMode(NwsMode mode) {
    mNwsMode = mode;
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_NWS_MODE, mNwsMode);
}

void RmmNwRatSwitchHandler::setIntSysProp(char *propName, int val) {
    int err = 0;
    char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
    err = sprintf(stgBuf, "%d", val);
    if (err < 0) logE(LOG_TAG, "setIntSysProp sprintf fail");
    rfx_property_set(propName, stgBuf);
}

int RmmNwRatSwitchHandler::getIntSysProp(char *propName, int defaultVal) {
    int val = -1;
    char stgBuf[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(propName, stgBuf, "-1");
    val = strtol(stgBuf, NULL, 10);
    if (val == -1) {
        val = defaultVal;
    }
    return val;
}

int RmmNwRatSwitchHandler::convertNetworkType(int nt_type, int prefer_type) {
    if (nt_type == 0) {
        return PREF_NET_TYPE_GSM_ONLY;
    } else if (nt_type == 1) {
        return PREF_NET_TYPE_WCDMA;
    } else if (nt_type == 2 && prefer_type == 0) {
        return PREF_NET_TYPE_GSM_WCDMA_AUTO;
    } else if (nt_type == 2 && prefer_type == 1) {
        return PREF_NET_TYPE_GSM_WCDMA_AUTO;
    } else if (nt_type == 2 && prefer_type == 2) {
        return PREF_NET_TYPE_GSM_WCDMA;
    //for LTE -- START
    } else if (nt_type == 6 && prefer_type == 4) {
        //4G Preferred (4G, 3G/2G) item
        //Bause we are not defind LTE preferred,
        //so return by NT_LTE_GSM_WCDMA_TYPE temporary
        return PREF_NET_TYPE_LTE_GSM_WCDMA;
    } else if (nt_type == 6 && prefer_type == 0) {
        //4G/3G/2G(Auto) item
        return PREF_NET_TYPE_LTE_GSM_WCDMA;
    } else if (nt_type == 14) {
        // LTE CDMA EVDO GSM/WCDMA mode
        return PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA;
    } else if (nt_type == 12) {
        // LTE CDMA EVDO GSM mode
        return PREF_NET_TYPE_LTE_CDMA_EVDO_GSM;
    } else if (nt_type == 3 && prefer_type == 0) {
        //4G only
        return PREF_NET_TYPE_LTE_ONLY;
    } else if (nt_type == 5 && prefer_type == 0) {
        // 4G/3G
        return PREF_NET_TYPE_LTE_WCDMA;
    } else if (nt_type == 10) {
        // 2G/3G/C2K
        return PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO;
    } else if (nt_type == 8 && prefer_type == 0) {
        // 2G/C2K 1x/evdo
        return PREF_NET_TYPE_CDMA_EVDO_GSM;
    } else if (nt_type == 8 && prefer_type == 32) {
        // 2G/C2K 1x
        return PREF_NET_TYPE_CDMA_GSM;
    } else if(nt_type == 11) {
        // LC mode
        return PREF_NET_TYPE_LTE_CDMA_EVDO;
    } else if (nt_type == 7 && prefer_type == 0) {
        // C2K 1x/evdo
        return PREF_NET_TYPE_CDMA_EVDO_AUTO;
    } else if (nt_type == 7 && prefer_type == 32) {
        // C2K 1x only
        return PREF_NET_TYPE_CDMA_ONLY;
    } else if (nt_type == 7 && prefer_type == 64) {
        // C2K evdo only
        return PREF_NET_TYPE_EVDO_ONLY;
    } else if (nt_type == 4) {
        // 4G/2G
        return PREF_NET_TYPE_LTE_GSM;
    } else if (nt_type == 15) {
        // 5G only
        return PREF_NET_TYPE_NR_ONLY;
    } else if (nt_type == 19) {
        // 5G/4G
        return PREF_NET_TYPE_NR_LTE;
    } else if (nt_type == 27) {
        // 5G/4G/C2K
        return PREF_NET_TYPE_NR_LTE_CDMA_EVDO;
    } else if (nt_type == 22) {
        // 5G/4G/3G/2G
        return PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM_WCDMA; //NETWORK_MODE_NR_LTE_GSM_WCDMA; NETWORK_MODE_NR_LTE_TDSCDMA_GSM;
    } else if (nt_type == 30) {
        // 5G/4G/C2K/3G/2G
        return PREF_NET_TYPE_NR_LTE_TDSCDMA_CDMA_EVDO_GSM_WCDMA; //NETWORK_MODE_NR_LTE_CDMA_EVDO_GSM_WCDMA;
    } else if (nt_type == 21) {
        // 5G/4G/3G
        return PREF_NET_TYPE_NR_LTE_TDSCDMA_WCDMA; //NETWORK_MODE_NR_LTE_WCDMA; NETWORK_MODE_NR_LTE_TDSCDMA;
    } else {
        return -1;
    }
}

int RmmNwRatSwitchHandler::isGcfTestMode() {
    int isTestMode = 0;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    memset(prop,0,sizeof(prop));
    rfx_property_get("vendor.gsm.gcf.testmode", prop, "0");
    isTestMode = atoi(prop);
    return isTestMode;
}

int RmmNwRatSwitchHandler::isTestSim(int slotId) {
    int ret = 0;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get(PROPERTY_TEST_SIM[slotId], prop, "0");
    ret = atoi(prop);
    return ret;
}

/*
  return value:
  -1: donoting
   0: enable 2G
   1: disable 2G
*/
int RmmNwRatSwitchHandler::disable2GOption(int reqType) {
    int ret = -1, mode = 1;
    int oldType = -1;

    if (!isOp07Support() || !mDisable2GWithRat) goto done; //op07

    if (!isGsmOnlySim()) {
        logD(LOG_TAG, "disable2GOption: no gsm only sim.");
        goto done;
    }

    oldType = getIntSysProp(PROPERTY_RILJ_NW_TYPE[m_slot_id], -1);

    if (oldType == reqType) goto done; // not changed
    if ((reqType & RAF_GROUP_GSM) > 0) mode = 0;

    if (mode == 1 || mode == 0) {
        sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
                MIPC_NW_SET_DISABLE_2G_REQ, m_slot_id);
        reqData->addMipcTlvUint8(MIPC_NW_SET_DISABLE_2G_REQ_T_MODE, (uint8_t)mode);
        sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
        if (cnfData->getResult() != MIPC_RESULT_SUCCESS) {
            logD(LOG_TAG, "disable2GOption, L5 returns FAIL!!");
        }
    }

    ret = mode;

done:
    if (oldType != reqType) {
        logD(LOG_TAG, "disable2GOption %d, oldType = %d, reqType = %d", ret, oldType, reqType);
    } else {
        logV(LOG_TAG, "disable2GOption %d, reqType = %d", ret, reqType);
    }
    return ret;
}

int RmmNwRatSwitchHandler::adjustRaf(int raf) {
    raf = ((RAF_GROUP_GSM & raf) > 0) ? (RAF_GROUP_GSM | raf) : raf;
    raf = ((RAF_GROUP_WCDMA & raf) > 0) ? (RAF_GROUP_WCDMA | raf) : raf;
    raf = ((RAF_GROUP_CDMA & raf) > 0) ? (RAF_GROUP_CDMA | raf) : raf;
    raf = ((RAF_GROUP_EVDO & raf) > 0) ? (RAF_GROUP_EVDO | raf) : raf;
    raf = ((RAF_GROUP_LTE & raf) > 0) ? (RAF_GROUP_LTE | raf) : raf;
    raf = ((RAF_GROUP_NR & raf) > 0) ? (RAF_GROUP_NR | raf) : raf;

    if (((raf & RAF_GROUP_CDMA) != 0 || (raf & RAF_GROUP_EVDO) != 0) &&
       ((raf & RAF_GROUP_NR) != 0 || (raf & RAF_GROUP_LTE) != 0 || (raf & RAF_GROUP_WCDMA) != 0)) {
        raf = (raf | RAF_GROUP_C2K);
    }

    return raf;
}
