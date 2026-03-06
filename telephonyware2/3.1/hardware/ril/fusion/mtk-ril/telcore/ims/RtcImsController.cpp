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
#include "ims/config/ImsConfigUtils.h"
#include "RtcImsConferenceHandler.h"
#include "RtcImsController.h"
#include "RfxEiregData.h"
#include "RfxMainThread.h"
#include "RfxRilUtils.h"
#include "RfxStatusDefs.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxImsRegInfoData.h"
#include "rfx_properties.h"
#include <string>
#include <openssl/sha.h>

/// M: add for op09 default volte setting @{
#define IMS_PROPERTY_3G_SIM          "persist.vendor.radio.simswitch"
#define PROPERTY_VOLTE_STATE     "persist.vendor.radio.volte_state"
#define PROPERTY_VOLTE_ENABLE    "persist.vendor.mtk.volte.enable"
#define IMS_CARD_TYPE_INVALID      UNKOWN_CARD       // 4
#define IMS_CARD_TYPE_NONE         CARD_NOT_INSERTED //cdma card type 255 is no sim inserted
#define READ_ICCID_RETRY_TIME      500
#define READ_ICCID_MAX_RETRY_COUNT 12

// hashed iccid
static const char PROPERTY_LAST_ICCID_SIM[4][29] = {
    "persist.vendor.radio.ls1icid",
    "persist.vendor.radio.ls2icid",
    "persist.vendor.radio.ls3icid",
    "persist.vendor.radio.ls4icid"
};
bool RtcImsController::sInitDone = false;
// last hashed iccid
char RtcImsController::sLastBootIccId[4][RFX_PROPERTY_VALUE_MAX] = {{0}, {0}, {0}, {0}};
int RtcImsController::sLastBootVolteState = 0;
/// @}

bool RtcImsController::mImsDisByMms[4] = {false, false, false, false};
int RtcImsController::imscfgcache[4][6] = {{-1}, {-1}, {-1}, {-1}};

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtcIms"

RFX_IMPLEMENT_CLASS("RtcImsController", RtcImsController, RfxController);

// register request to RfxData
/// M: add for op09 volte setting @{
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RIL_MSG_UNSOL_VOLTE_SETTING);
/// @}

RtcImsController::RtcImsController() :
    mNoIccidTimerHandle(NULL),
    mNoIccidRetryCount(0),
    mMainSlotId(0),
    mIsBootUp(true),
    mIsSimSwitch(false),
    mIsImsDisabling(false),
    mCachedXuiUrc(NULL) {
}

RtcImsController::~RtcImsController() {
}

void RtcImsController::requestImsDisable(int slotId, const sp<RfxAction>& action) {
    RFX_UNUSED(action);
    int cfgparams[6] = { 0, 0, 0, 0, 0, 0 };
    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_SET_IMSCFG_EX, RfxIntsData(cfgparams, 6));

    logD(RFX_LOG_TAG, "requestImsDisable(), slot = %d", slotId);
    mImsDisByMms[slotId] = true;

    requestToMcl(msg);
}

void RtcImsController::requestImsResume(int slotId, const sp<RfxAction>& action) {
    RFX_UNUSED(action);
    if (slotId < 0 || slotId >= 4) {
        logE(RFX_LOG_TAG, "requestImsResume(), slot:%d", slotId);
        return;
    }

    logD(RFX_LOG_TAG, "requestImsResume(), slot = %d, cache<%d,%d,%d,%d,%d,%d>", slotId,
                       imscfgcache[slotId][0], imscfgcache[slotId][1], imscfgcache[slotId][2],
                       imscfgcache[slotId][3], imscfgcache[slotId][4], imscfgcache[slotId][5]);
    mImsDisByMms[slotId] = false;

    if(imscfgcache[slotId][5] != -1){
        sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
                RFX_MSG_REQUEST_SET_IMSCFG_EX, RfxIntsData(imscfgcache[slotId], 6));
        requestToMcl(msg);
    }
}

void RtcImsController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logD(RFX_LOG_TAG, "onInit()");

    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT,
        RfxStatusChangeCallback(this, &RtcImsController::onMainCapabilitySlotChanged));
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SERVICE_STATE,
        RfxStatusChangeCallback(this, &RtcImsController::onServiceStateChanged));

    /// M: add for op09 default volte setting @{
    initOp09Ims();
    /// @}

    const int request_id_list[] = {
        RFX_MSG_REQUEST_IMS_REGISTRATION_STATE,
        /// M: add for op09 default volte setting @{
        RFX_MSG_REQUEST_SET_IMSCFG,
        RFX_MSG_REQUEST_SET_IMSCFG_EX, // rild local
        /// @}
        RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL,
        RFX_MSG_REQUEST_SET_WFC_PROFILE,
        RFX_MSG_REQUEST_IMS_BEARER_STATE_CONFIRM,
        RFX_MSG_REQUEST_SET_IMS_BEARER_NOTIFICATION,
        RFX_MSG_REQUEST_RUN_GBA,
        RFX_MSG_REQUEST_SET_IMS_REGISTRATION_REPORT,
        RFX_MSG_REQUEST_SET_VODATA_CFG,
        RFX_MSG_REQUEST_SET_IMS_ADD_CAP,
    };

    const int urc_id_list[] = {
        RFX_MSG_UNSOL_IMS_DISABLE_START,
        RFX_MSG_UNSOL_IMS_ENABLE_START,
        RFX_MSG_UNSOL_IMS_DISABLE_DONE,
        RFX_MSG_UNSOL_IMS_ENABLE_DONE,
        RFX_MSG_UNSOL_IMS_REGISTRATION_INFO,
        RFX_MSG_UNSOL_IMS_DEREG_DONE,
        RFX_MSG_UNSOL_ON_XUI,
        RFX_MSG_URC_IMS_BEARER_STATE_NOTIFY,
        RFX_MSG_URC_IMS_BEARER_INIT,
        RFX_MSG_URC_IMS_DATA_INFO_NOTIFY,
        RFX_MSG_UNSOL_IMS_SUPPORT_ECC,
        // RFX_MSG_UNSOL_IMS_MULTIIMS_COUNT,
        RFX_MSG_UNSOL_IMS_REGISTRATION_STATE_IND,
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));
}

void RtcImsController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT,
        RfxStatusChangeCallback(this, &RtcImsController::onMainCapabilitySlotChanged));
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_SERVICE_STATE,
        RfxStatusChangeCallback(this, &RtcImsController::onServiceStateChanged));
    /// M: add for op09 default volte setting @{
    deinitOp09Ims();
    /// @}
    RfxController::onDeinit();
}

bool RtcImsController::onHandleRequest(const sp<RfxMessage>& message) {
    //UT for MMSoverrule
    sp<RfxAction> action;
    int msg_id = message->getId();
    //logD(RFX_LOG_TAG, "onHandleRequest - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
    case RFX_MSG_REQUEST_IMS_REGISTRATION_STATE:
        handleImsRegStateRequest(message);
        break;
    /// M: add for op09 volte setting @{
    case RFX_MSG_REQUEST_SET_IMSCFG:
    case RFX_MSG_REQUEST_SET_IMSCFG_EX:
        /// M: add for tc16sp customization @{
        cacheimscfgparams(message);
        if(mImsDisByMms[getSlotId()] == true){
            logD(RFX_LOG_TAG, "mmsoverruleimscfg  for slot %d", getSlotId());
            mmsoverruleimscfg(message);
        }
        /// @}
        handleSetImsConfigRequest(message);
        break;
    /// @}
    case RFX_MSG_REQUEST_SET_WFC_PROFILE:
    case RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL:
    case RFX_MSG_REQUEST_SET_VODATA_CFG:
    case RFX_MSG_REQUEST_SET_IMS_ADD_CAP:
    case RFX_MSG_REQUEST_IMS_BEARER_STATE_CONFIRM:
    case RFX_MSG_REQUEST_SET_IMS_BEARER_NOTIFICATION:
    case RFX_MSG_REQUEST_RUN_GBA:
        requestToMcl(message);
        break;
    case RFX_MSG_REQUEST_SET_IMS_REGISTRATION_REPORT:
        sendCachedXui();
        requestToMcl(message);
        break;
    default:
        break;
    }
    return true;
}

bool RtcImsController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    //logD(RFX_LOG_TAG, "onHandleUrc - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
        case RFX_MSG_UNSOL_IMS_DISABLE_DONE:
            handleImsDisableDoneUrc(message);
            break;
        case RFX_MSG_UNSOL_ON_XUI:
            handleXui(message);
            break;
        case RFX_MSG_UNSOL_IMS_REGISTRATION_STATE_IND:
            handleImsRegInfoInd(message);
            break;
        default:
            break;
    }
    responseToRilj(message);
    return true;
}

bool RtcImsController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    //logD(RFX_LOG_TAG, "onHandleResponse - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
    case RFX_MSG_REQUEST_IMS_REGISTRATION_STATE:
        handleImsRegStateResponse(message);
        break;
    /// M: add for op09 volte setting @{
    // the request is from RtcImsConfigController, so do not response to RILJ
    case RFX_MSG_REQUEST_SET_IMSCFG_EX: //no need rsp
    case RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL:
    case RFX_MSG_REQUEST_SET_VODATA_CFG:
    case RFX_MSG_REQUEST_SET_IMS_ADD_CAP:
        break;
    case RFX_MSG_REQUEST_SET_IMSCFG:
        handleSetImsConfigResponse(message);
        break;
    /// @}
    case RFX_MSG_REQUEST_IMS_BEARER_STATE_CONFIRM:
    case RFX_MSG_REQUEST_SET_IMS_BEARER_NOTIFICATION:
    case RFX_MSG_REQUEST_SET_WFC_PROFILE:
    case RFX_MSG_REQUEST_RUN_GBA:
        responseToRilj(message);
        break;
    case RFX_MSG_REQUEST_SET_IMS_REGISTRATION_REPORT:
        responseToRilj(message);
        sendCachedImsRegInfo();
        break;
    default:
        logD(RFX_LOG_TAG, "unknown request, ignore!");
        return false;
    }
    return true;
}

void RtcImsController::handleXui(const sp<RfxMessage>& message) {
    RfxStringsData* data = (RfxStringsData*)message->getData();
    char** params = (char**)data->getData();
    char* xui = params[2];
    if (xui == NULL) {
        logE(RFX_LOG_TAG, "handleXui, xui is null!");
        return;
    }
    std::vector<std::string> uris = RtcImsConferenceHandler::splitString(xui, ",");
    Vector<String8> xuis;
    for(int i = 0; i < (int)uris.size(); i++) {
        std::string uri = RtcImsConferenceHandler::getUserNameFromSipTelUriString(uris[i]);
        if (!uri.empty()) {
            xuis.push_back(String8::format("%s", uri.c_str()));
        } else {
            logD(RFX_LOG_TAG, "handleXui empty uri");
        }
    }
    for (int i = 0; i < (int)xuis.size(); ++i) {
        const char *cTemp = xuis[i].string();
        string c = cTemp != NULL ? cTemp : "";
        if (!c.empty()) {
            logD(RFX_LOG_TAG,
                     "handleXui setValue: %s", RfxRilUtils::pii(RFX_LOG_TAG, c.data()));
        }
    }
    getStatusManager(m_slot_id)->setString8VectorValue(RFX_STATUS_KEY_XUI_INFO, xuis);

    Vector<String8> xuis_ = getStatusManager(m_slot_id)->getString8VectorValue(
            RFX_STATUS_KEY_XUI_INFO);
    for (int i = 0; i < (int)xuis_.size(); ++i) {
        const char *cTemp = xuis_[i].string();
        string c = cTemp != NULL ? cTemp : "";
        if (!c.empty()) {
            logD(RFX_LOG_TAG,
                    "handleXui KEY_XUI_INFO: %s", RfxRilUtils::pii(RFX_LOG_TAG, c.data()));
        }
    }
    mCachedXuiUrc = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_ON_XUI, message);
}

void RtcImsController::handleImsRegStateRequest(const sp<RfxMessage>& message) {
    requestToMcl(message);
}

void RtcImsController::handleImsRegStateResponse(const sp<RfxMessage>& message) {
    if (RIL_E_REQUEST_NOT_SUPPORTED == message->getError()
        || message->getData() == NULL
        || message->getData()->getData() == NULL) {
        logI(RFX_LOG_TAG, "handleImsRegStateResponse getError = %d, Slot = %d",
        message->getError(), message->getSlotId());
        sp<RfxMessage> msg = RfxMessage::obtainRequest(message->getSlotId(), message->getPId(),
            message->getPToken(), NULL, 0, message->getRilToken(), message->getClientId(),
            RADIO_TECH_GROUP_GSM);
        requestToMcl(msg, true, ms2ns(500));
        return;
    }

    int isImsReg = ((int *)message->getData()->getData())[0];
    int rilDataRadioTech = 0;
    RfxPsRatFamily ratFamily = RFX_PS_RAT_FAMILY_UNKNOWN;
    // Check SMSoIMS debug flag first
    char smsformat[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.smsformat", smsformat, "");

    if ((strlen(smsformat) == 4) && (strncmp(smsformat, "3gpp", 4) == 0)) {
        ((int *)message->getData()->getData())[1] = RFX_PS_RAT_FAMILY_GSM;
    } else if ((strlen(smsformat) == 5) &&  (strncmp(smsformat, "3gpp2", 5) == 0)) {
        ((int *)message->getData()->getData())[1] = RFX_PS_RAT_FAMILY_CDMA;
    } else {
        RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
        RfxNwServiceState serviceState = getStatusManager()
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
        rilDataRadioTech = serviceState.getRilDataRadioTech();
        ratFamily = RfxRilUtils::getPsRatFamily(rilDataRadioTech);

        if (ratFamily == RFX_PS_RAT_FAMILY_IWLAN) {
            // treat IWLAN as 3GPP
            ((int *)message->getData()->getData())[1] = RFX_PS_RAT_FAMILY_GSM;
        } else {
            //RFX_PS_RAT_FAMILY_UNKNOWN = 0,
            //RFX_PS_RAT_FAMILY_GSM = 1,
            //RFX_PS_RAT_FAMILY_CDMA = 2,
            if (isImsReg == 1 && ratFamily == RFX_PS_RAT_FAMILY_UNKNOWN) {
                ((int *)message->getData()->getData())[1] = RFX_PS_RAT_FAMILY_GSM;
            } else {
                ((int *)message->getData()->getData())[1] = ratFamily;
            }
        }
    }

    int format = ((int *)message->getData()->getData())[1];

    logD(RFX_LOG_TAG, "rilDataRadioTech=%d, smsformat=%s, ratFamily=%d, isImsReg=%d, format=%d",
            rilDataRadioTech, smsformat, ratFamily, isImsReg, format);

    responseToRilj(message);
}

void RtcImsController::onServiceStateChanged(RfxStatusKeyEnum key,
        RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    RfxNwServiceState oldSS = oldValue.asServiceState();
    RfxNwServiceState newSS = newValue.asServiceState();
    int oDataRadioTech = oldSS.getRilDataRadioTech();
    int oDataRegState = oldSS.getRilDataRegState();
    int dataRadioTech = newSS.getRilDataRadioTech();
    int dataRegState = newSS.getRilDataRegState();
    int oWfcRegState = oldSS.getIwlanRegState();
    int wfcRegState = newSS.getIwlanRegState();
    int ims_reg = getStatusManager()->getIntValue(
            RFX_STATUS_KEY_IMS_REGISTRATION_STATE, 0);
    int response[3];
    RIL_EiregData eiregRsp;
    const int WFC_STATE_ON = 1 << 4;
    bool ignoreIms =
            getStatusManager()->getString8Value(RFX_STATUS_KEY_IGNORE_VOLTE_VILTE_FOR_23G,
                                                String8("false")) == "true";

    logD(RFX_LOG_TAG, "onServiceStateChanged: %d, dataRadioTech:%d->%d, dataRegState=%d->%d, ims_reg:%d",
        ignoreIms, oDataRadioTech, dataRadioTech, oDataRegState, dataRegState, ims_reg);

    if ((oDataRadioTech == dataRadioTech && oDataRegState == dataRegState && oWfcRegState== wfcRegState)
            || ims_reg == 0) {
        return;
    }

    if (!ignoreIms) {
        return;
    }
    if ((oDataRadioTech == RADIO_TECH_LTE || oDataRadioTech == RADIO_TECH_LTE_CA
            || oDataRadioTech == RADIO_TECH_NR) && (dataRadioTech == RADIO_TECH_LTE
            || dataRadioTech == RADIO_TECH_LTE_CA || dataRadioTech == RADIO_TECH_NR)) {
        return;
    }

    response[0] = 1;
    response[2] = m_slot_id;

    int wfcState = getStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_STATE, -1);

    //if (voiceRegState != RIL_REG_STATE_HOME && voiceRegState != RIL_REG_STATE_ROAMING) {
    if ((dataRadioTech == RADIO_TECH_LTE || dataRadioTech == RADIO_TECH_LTE_CA
        || dataRadioTech == RADIO_TECH_NR || dataRadioTech == RADIO_TECH_IWLAN  || wfcState == 1)
            || (dataRegState != 1 && dataRegState != 5)) {
        response[1] = ims_reg;
        eiregRsp.ext_info = ims_reg;
        int wfcState = getStatusManager()->getIntValue(RFX_STATUS_KEY_WFC_STATE, -1);
        if (wfcState == 1) {
            // enable VoWifi bit in ext_info if wfc state was 1
            response[1] |= WFC_STATE_ON;
        } else if ((response[1] & WFC_STATE_ON) == WFC_STATE_ON) {
            response[1] ^= WFC_STATE_ON;
        }
        logD(RFX_LOG_TAG, "onServiceStateChanged: changed CIREGU:1,%d", response[1]);
    } else {
        response[1] = (ims_reg & 0xF6); //ignore voice and video
        if (response[1] == 0) {
            response[0] = 0;
        }
        eiregRsp.ext_info = response[1];
        logD(RFX_LOG_TAG, "onServiceStateChanged: 3G changed CIREGU:1,%d->%d,%d", ims_reg, response[0], response[1]);
    }

    // send +EIREG
    eiregRsp.reg_state = 0;
    eiregRsp.reg_type = response[0];
    eiregRsp.dereg_cause = 0;
    eiregRsp.ims_retry = 0;
    eiregRsp.rat = getStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_RAT, 10);
    eiregRsp.sip_uri_type = 0;
    logD(RFX_LOG_TAG, "onServiceStateChanged, +EIREG:%d,%d,%d,%d,%d,%d,%d",
            eiregRsp.reg_state, eiregRsp.reg_type, eiregRsp.ext_info, eiregRsp.dereg_cause,
            eiregRsp.ims_retry, eiregRsp.rat, eiregRsp.sip_uri_type);
    sp<RfxMessage> eiregUrc = RfxMessage::obtainUrc(m_slot_id,
            RFX_MSG_UNSOL_EIREG_INFO_IND,
            RfxEiregData(&eiregRsp, sizeof(RIL_EiregData)));
    responseToRilj(eiregUrc);

    // send +CIREGU
    sp<RfxMessage> urc = RfxMessage::obtainUrc(
            m_slot_id,
            RFX_MSG_UNSOL_IMS_REGISTRATION_INFO,
            RfxIntsData(response, 3));
    responseToRilj(urc);
}

bool RtcImsController::responseToRilj(const sp<RfxMessage>& msg) {
    return RfxController::responseToRilj(msg);
}

bool RtcImsController::onCheckIfRejectMessage(
        const sp<RfxMessage>& message, bool isModemPowerOff, int radioState) {

    bool isWfcSupport = RfxRilUtils::isWfcSupport();

    //logD(RFX_LOG_TAG, "onCheckIfRejectMessage, id = %d, isModemPowerOff = %d, radioState = %d, msg = %s",
            //message->getId(), isModemPowerOff, radioState, idToString(message->getId()));

    // The RIL Request should be bypass anyway
    if (isWfcSupport && !isModemPowerOff) {
        //default dont need to log false case since this value will not cause this msg to be rejected
        //logD(RFX_LOG_TAG, "onCheckIfRejectMessage ret=false: isWfcSupport=%d, isModemPowerOff=%d", isWfcSupport, isModemPowerOff);
        return false;
    }

    int msgId = message->getId();
    if((radioState == (int)RADIO_STATE_UNAVAILABLE ||
        radioState == (int)RADIO_STATE_OFF) &&
            (msgId == RFX_MSG_REQUEST_SET_IMSCFG ||
             msgId == RFX_MSG_REQUEST_SET_IMSCFG_EX ||
             msgId == RFX_MSG_REQUEST_IMS_BEARER_STATE_CONFIRM ||
             msgId == RFX_MSG_REQUEST_RUN_GBA ||
             msgId == RFX_MSG_REQUEST_SET_IMS_BEARER_NOTIFICATION ||
             msgId == RFX_MSG_REQUEST_SET_WFC_PROFILE ||
             msgId == RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL ||
             msgId == RFX_MSG_REQUEST_IMS_REGISTRATION_STATE ||
             msgId == RFX_MSG_REQUEST_SET_VODATA_CFG ||
             msgId == RFX_MSG_REQUEST_SET_IMS_ADD_CAP)) {
        //default dont need to log false case since this value will not cause this msg to be rejected
        //logD(RFX_LOG_TAG, "onCheckIfRejectMessage ret=false: radioState=%d, msgId=%d", radioState, msgId);
        return false;
    }
    //others will print in RfxController::onCheckIfRejectMessage
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

/// M: add for op09 volte setting @{
void RtcImsController::initOp09Ims() {
    if (!RfxRilUtils::isCtVolteSupport() && !RfxRilUtils::isCtMixVolteSupport()) {
        return;
    }

    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(IMS_PROPERTY_3G_SIM, tempstr, "1");
    mMainSlotId = atoi(tempstr) - 1;

    char volteEnable[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(PROPERTY_VOLTE_ENABLE, volteEnable, "0");
    int volteValue = atoi(volteEnable);
    logD(RFX_LOG_TAG, "initOp09Ims: volteEnable = %d, mMainSlotId = %d", volteValue, mMainSlotId);

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CDMA_CARD_TYPE,
        RfxStatusChangeCallback(this, &RtcImsController::onCardTypeChanged));

    if (sInitDone == false) {
        for (int slot = 0; slot < RFX_SLOT_COUNT; slot++) {
            rfx_property_get(PROPERTY_LAST_ICCID_SIM[slot], sLastBootIccId[slot], "null");
        }
        char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get(PROPERTY_VOLTE_STATE, tempstr, "0");
        sLastBootVolteState = atoi(tempstr);
        logD(RFX_LOG_TAG, "initOp09Ims, sLastBootVolteState = %d", sLastBootVolteState);

        char cryptoState[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get("ro.crypto.state", cryptoState, "");
        if (strcmp("unencrypted", cryptoState) != 0 && strcmp("unsupported", cryptoState) != 0) {
            char cryptoType[RFX_PROPERTY_VALUE_MAX] = { 0 };
            rfx_property_get("ro.crypto.type", cryptoType, "");
            char decrypt[RFX_PROPERTY_VALUE_MAX] = { 0 };
            rfx_property_get("vold.decrypt", decrypt, "");
            logD(RFX_LOG_TAG, "initOp09Ims: crypto.type = %s, decrypt = %s", cryptoType, decrypt);
            if (strlen(cryptoType) == 0 || strcmp("file", cryptoType) == 0 ||
                strcmp("trigger_restart_framework", decrypt) == 0) {
                sInitDone = true;
            }
        } else {
            sInitDone = true;
        }
    }
}

void RtcImsController::deinitOp09Ims() {
    if (!RfxRilUtils::isCtVolteSupport() && !RfxRilUtils::isCtMixVolteSupport()) {
        return;
    }
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CDMA_CARD_TYPE,
        RfxStatusChangeCallback(this, &RtcImsController::onCardTypeChanged));
}

void RtcImsController::onMainCapabilitySlotChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant new_value) {
    RFX_UNUSED(key);
    if (RfxRilUtils::isMultipleImsSupport() == 0)  {
        // query IMS registration status again after SIM switch for single ims
        updateImsRegStatus();
    }

    if (!RfxRilUtils::isCtVolteSupport() && !RfxRilUtils::isCtMixVolteSupport()) {
        return;
    }
    int oldType = old_value.asInt();
    int newType = new_value.asInt();
    logD(RFX_LOG_TAG, "onMainCapabilitySlotChanged()  cap %d->%d, mMainSlotId:%d",
        oldType, newType, mMainSlotId);

    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(IMS_PROPERTY_3G_SIM, tempstr, "1");
    int mainSlotId = atoi(tempstr) - 1;
    int curSlotId = getSlotId();

    if (mMainSlotId == mainSlotId) {
        logD(RFX_LOG_TAG, "onMainCapabilitySlotChanged()  not real slot change");
        return;
    }

    if (RfxRilUtils::isMultipleImsSupport() == 0) {
        String8 currentIccid = getStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);
        const char* newIccIdStr = currentIccid.string();
        logD(RFX_LOG_TAG, "onMainCapabilitySlotChanged, newIccIdStr = %s",
             givePrintableStr(newIccIdStr));
        if (strlen(newIccIdStr) == 0 || strcmp("N/A", newIccIdStr) == 0) {
            logD(RFX_LOG_TAG, "onMainCapabilitySlotChanged(), iccid not ready");
            // iccid change case, cover by onCardTypeChanged()
            // also set mMainSlotId in onCardTypeChanged()
        } else {
            mMainSlotId = mainSlotId;
            mIsSimSwitch = true;
            int card_type = getStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE);
            setDefaultVolteState(curSlotId, newIccIdStr, card_type);
        }
    } else {
        mMainSlotId = mainSlotId;
    }
}

void RtcImsController::onCardTypeChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);
    // main slot status only update after switching finish, so must use property.
    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(IMS_PROPERTY_3G_SIM, tempstr, "1");
    int mainSlotId = atoi(tempstr) - 1;
    int curSlotId = getSlotId();

    if (mainSlotId != mMainSlotId) {
        mMainSlotId = mainSlotId;
        mIsSimSwitch = true;
    }
    int old_type = old_value.asInt();
    int new_type = value.asInt();
    logD(RFX_LOG_TAG, "onCardTypeChanged, old_type is %d, new_type is %d, mMainSlotId = %d",
         old_type, new_type, mMainSlotId);

    // RFX_STATUS_KEY_CARD_TYPE -1 -> RFX_STATUS_KEY_CDMA_CARD_TYPE 4
    // RFX_STATUS_KEY_CARD_TYPE 0 -> RFX_STATUS_KEY_CDMA_CARD_TYPE 255
    // if old_type == -1 and new_type == 0, it is bootup without sim.
    // if old_type == -1 and new_type > 0, it is bootup with sim or sim switch finish.
    // if old_type == 0 and new_type > 0, it is plug in sim.
    // if old_type == 0 and new_type == 0, it is still no sim.
    // if old_type > 0 and new_type == -1, it is sim switch start, no need deal yet.
    if (old_type == IMS_CARD_TYPE_INVALID || old_type == IMS_CARD_TYPE_NONE) {
        if (new_type == IMS_CARD_TYPE_NONE) {
            // no sim inserted case
            rfx_property_set(PROPERTY_LAST_ICCID_SIM[curSlotId], "null");
            mIsBootUp = false;
            mIsSimSwitch = false;
            mCachedXuiUrc = NULL;
            getStatusManager(m_slot_id)->setString8VectorValue(RFX_STATUS_KEY_XUI_INFO, Vector<String8>());
        } else if (new_type > 0 && new_type != IMS_CARD_TYPE_INVALID) {
            String8 currentIccid = getStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);
            const char* newIccIdStr = currentIccid.string();
            logD(RFX_LOG_TAG, "onCardTypeChanged, newIccIdStr = %s",
                 givePrintableStr(newIccIdStr));
            int waitCount = 1;
            if (sInitDone == false) {
                char cryptoState[RFX_PROPERTY_VALUE_MAX] = { 0 };
                rfx_property_get("ro.crypto.state", cryptoState, "");
                if (strcmp("unencrypted", cryptoState) != 0
                        && strcmp("unsupported", cryptoState) != 0) {
                    char cryptoType[RFX_PROPERTY_VALUE_MAX] = { 0 };
                    rfx_property_get("ro.crypto.type", cryptoType, "");
                    char decrypt[RFX_PROPERTY_VALUE_MAX] = { 0 };
                    rfx_property_get("vold.decrypt", decrypt, "");
                    logD(RFX_LOG_TAG, "onCardTypeChanged: crypto.type = %s, decrypt = %s",
                         cryptoType, decrypt);
                    if (strlen(cryptoType) == 0 || strcmp("file", cryptoType) == 0 ||
                        strcmp("trigger_restart_framework", decrypt) == 0) {
                        if (sInitDone == false) {
                            waitCount = 2;
                        } else {
                            waitCount = 1;
                        }
                    } else {
                        waitCount = 4; //this value can be customized
                    }
                }
            }
            if (waitCount > 1 || strlen(newIccIdStr) == 0 || strcmp("N/A", newIccIdStr) == 0) {
                logD(RFX_LOG_TAG, "onCardTypeChanged, iccid not ready");
                // start timer waiting icc id loaded
                if (mNoIccidTimerHandle != NULL) {
                    RfxTimer::stop(mNoIccidTimerHandle);
                }
                mNoIccidRetryCount = 0;
                mNoIccidTimerHandle =
                    RfxTimer::start(RfxCallback0(this, &RtcImsController::onNoIccIdTimeout),
                                    ms2ns(waitCount*READ_ICCID_RETRY_TIME));
            } else {
                setDefaultVolteState(curSlotId, newIccIdStr, new_type);
            }
        }
    } else {
        // if old_type > 0 and new_type == 0, it is plug out sim.
        if (new_type == IMS_CARD_TYPE_NONE) {
            if (curSlotId >= 0 && curSlotId < 4) {
                rfx_property_set(PROPERTY_LAST_ICCID_SIM[curSlotId], "null");
            }
            mIsBootUp = false;
        }

        mIsSimSwitch = false;
    }
}

void RtcImsController::onNoIccIdTimeout() {
    mNoIccidTimerHandle = NULL;
    int slotId = getSlotId();
    String8 currentIccid = getStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);
    const char* newIccIdStr = currentIccid.string();
    logD(RFX_LOG_TAG, "onNoIccIdTimeout, newIccIdStr = %s", givePrintableStr(newIccIdStr));

    if (strlen(newIccIdStr) == 0 || strcmp("N/A", newIccIdStr) == 0) {
        // if still fail, need revise timer
        if (mNoIccidRetryCount < READ_ICCID_MAX_RETRY_COUNT &&
            mNoIccidTimerHandle == NULL) {
            mNoIccidRetryCount++;
            mNoIccidTimerHandle = RfxTimer::start(RfxCallback0(this,
                            &RtcImsController::onNoIccIdTimeout), ms2ns(READ_ICCID_RETRY_TIME));
        }
    } else {
        int newType = getStatusManager(slotId)->getIntValue(
                RFX_STATUS_KEY_CDMA_CARD_TYPE, IMS_CARD_TYPE_NONE);
        if (newType <= 255) {
            setDefaultVolteState(slotId, newIccIdStr, newType);
        }
    }
}

void RtcImsController::setNonOp09VolteState(int slot_id, const char new_iccid[]) {
    char oldIccIdStr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    // multi ims need save every sim iccid
    rfx_property_get(PROPERTY_LAST_ICCID_SIM[slot_id], oldIccIdStr, "null");
    logD(RFX_LOG_TAG, "setNonOp09VolteState, oldIccIdStr = %s", givePrintableStr(oldIccIdStr));

    char hashedNewIccIdStr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    string hashedResult = hash256(new_iccid);
    if (!hashedResult.empty()) {
        int length =
            snprintf(hashedNewIccIdStr, sizeof(hashedNewIccIdStr), "%s", hashedResult.c_str());
        if (length < 0) {
            logD(RFX_LOG_TAG, "setNonOp09VolteState, hash256 ICC id error.");
        }
    } else {
        logD(RFX_LOG_TAG, "setNonOp09VolteState, hash256 result is empty.");
    }
    rfx_property_set(PROPERTY_LAST_ICCID_SIM[slot_id], hashedNewIccIdStr);
    int setValue = 0;
    if (RfxRilUtils::isMultipleImsSupport() == 0) {
        if (strlen(oldIccIdStr) != 0 &&
            strcmp(hashedNewIccIdStr, oldIccIdStr) == 0) {
            if (mIsBootUp) {
                mIsBootUp = false;
                for (int slot = 0; slot < RFX_SLOT_COUNT; slot++) {
                    if (strcmp(hashedNewIccIdStr, sLastBootIccId[slot]) == 0) {
                        // For switch sim when powoff case, boot up need get before setting
                        int state = sLastBootVolteState;
                        setValue = (state >> slot) & 1;
                        mIsBootUp = true;
                        logD(RFX_LOG_TAG,
                            "setNonOp09VolteState, change sim slot reboot, last= %d, %d",
                            state, setValue);
                        break;
                    }
                }
            }

            if (mIsBootUp) {
                setVolteStateProperty(slot_id, setValue);
                // for single ims, only set volte state and send request for main capability sim.
                if (mMainSlotId == slot_id) {
                    // state diff, send volte state to IMS FW
                    sendDefaultVolteStateUrc(slot_id, setValue);
                }
            }
        } else {
            setVolteStateProperty(slot_id, 1);
        }
    } else {
        if (strlen(oldIccIdStr) != 0 &&
            strcmp(hashedNewIccIdStr, oldIccIdStr) == 0) {
            if (mIsBootUp) {
                mIsBootUp = false;
                for (int slot = 0; slot < RFX_SLOT_COUNT; slot++) {
                    if (strcmp(hashedNewIccIdStr, sLastBootIccId[slot]) == 0) {
                        // For switch sim when powoff case, boot up need get before setting
                        if (slot != slot_id
                            && strcmp(hashedNewIccIdStr, sLastBootIccId[slot_id]) == 0) {
                            // use same slot firstly
                            slot = slot_id;
                        }
                        int state = sLastBootVolteState;
                        setValue = (state >> slot) & 1;
                         mIsBootUp = true;
                        logD(RFX_LOG_TAG,
                            "setNonOp09VolteState, change sim slot reboot, last= %d, %d",
                            state, setValue);
                        break;
                    }
                }
            }

            if (mIsBootUp) {
                setVolteStateProperty(slot_id, setValue);
                // send volte state to IMS FW
                sendDefaultVolteStateUrc(slot_id, setValue);
            }
        } else {
            setVolteStateProperty(slot_id, 1);
        }
    }
}

void RtcImsController::checkNonOp09VolteState(int slot_id, const char new_iccid[]) {
    char oldIccIdStr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    // multi ims need save every sim iccid
    rfx_property_get(PROPERTY_LAST_ICCID_SIM[slot_id], oldIccIdStr, "null");
    logD(RFX_LOG_TAG, "checkNonOp09VolteState, oldIccIdStr = %s", givePrintableStr(oldIccIdStr));

    char hashedNewIccIdStr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    string hashedResult = hash256(new_iccid);
    if (!hashedResult.empty()) {
        int length =
            snprintf(hashedNewIccIdStr, sizeof(hashedNewIccIdStr), "%s", hashedResult.c_str());
        if (length < 0) {
            logE(RFX_LOG_TAG, "checkNonOp09VolteState, hash256 ICC id error.");
        }
    } else {
        logE(RFX_LOG_TAG, "checkNonOp09VolteState, hash256 result is empty.");
    }
    rfx_property_set(PROPERTY_LAST_ICCID_SIM[slot_id], hashedNewIccIdStr);
    int setValue = 0;
    if (RfxRilUtils::isMultipleImsSupport() == 0) {
        if (mIsBootUp && mMainSlotId == slot_id) {
            if (strcmp(hashedNewIccIdStr, sLastBootIccId[slot_id]) == 0) {
                int state = sLastBootVolteState;
                setValue = (state >> slot_id) & 1;
                logD(RFX_LOG_TAG,
                    "checkNonOp09VolteState, reboot case, last= %d, %d",
                    state, setValue);
                // send volte state to modem
                sendImsSettingstoMcl(setValue);
            }
        }
    } else {
        if (mIsBootUp) {
            mIsBootUp = false;
            for (int slot = 0; slot < RFX_SLOT_COUNT; slot++) {
                if (strcmp(hashedNewIccIdStr, sLastBootIccId[slot]) == 0) {
                    // For swap sim when powoff case, boot up need get before setting
                    if (slot != slot_id
                        && strcmp(hashedNewIccIdStr, sLastBootIccId[slot_id]) == 0) {
                        // use same slot firstly
                        slot = slot_id;
                    }
                    int state = sLastBootVolteState;
                    setValue = (state >> slot) & 1;
                    logD(RFX_LOG_TAG,
                        "checkNonOp09VolteState, reboot case, last= %d, %d",
                        state, setValue);
                    // send volte state to modem
                    sendImsSettingstoMcl(setValue);
                    break;
                }
            }
        }
    }
}

void RtcImsController::sendImsSettingstoMcl(int value) {
    RFX_UNUSED(value);
    char feature[] = "IMS over 3gpp";
    int featureVersion = getFeatureVersion(feature);
    //int isVonrSetSupport = ImsConfigUtils::getSystemPropValue(
    //        ImsConfigUtils::PROPERTY_VONR_SETTING_SUPPORT);
    int isAllowTurnOff =
            getStatusManager()->getString8Value(RFX_STATUS_KEY_CARRIER_ALLOW_TURN_OFF_IMS) == "1";
    int slotId = getSlotId();
    int params[6];
    params[0] = RfxRilUtils::isVolteSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VOLTE_ENALBE, slotId) : 0;  // volte
    params[1] = RfxRilUtils::isVilteSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VILTE_ENALBE, slotId) : 0;  // vilte
    params[2] = RfxRilUtils::isWfcSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_WFC_ENALBE, slotId) : 0;    // vowifi
    params[3] = RfxRilUtils::isViwifiSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VIWIFI_ENALBE, slotId) : 0; // viwifi
    params[4] = RfxRilUtils::isSmsSupport() ? ImsConfigUtils::getSystemPropValue(
            ImsConfigUtils::PROPERTY_IMS_SUPPORT) : 0;   // sms

    if (value != params[0]) {
        logD(RFX_LOG_TAG,
        "sendImsSettingstoMcl, error cache inconsistent, property:%d, state:%d, phoneId:%d",
        params[0], value, slotId);
        return;
    }

    if (featureVersion > 1) {
        //if (isVonrSetSupport == 1) {
            int vonr = RfxRilUtils::isVolteSupport() ? ImsConfigUtils::getFeaturePropValue(
                ImsConfigUtils::PROPERTY_VONR_ENALBE, slotId) : 0;  // vonr
            int vinr = RfxRilUtils::isVilteSupport() ? ImsConfigUtils::getFeaturePropValue(
                ImsConfigUtils::PROPERTY_VINR_ENALBE, slotId) : 0;  // vinr
            vinr = vonr & vinr;
            params[0] = (params[0] | (vonr << 1));
            params[1] = (params[1] | (vinr << 1));
        //}
    }

    char nrFeature[] = "N3IWF_SUPPORT";
    int nrFeatureVersion = getFeatureVersion(nrFeature);
    if (nrFeatureVersion == 1) {
        int nrVowifi = getStatusManager()->getString8Value(
                RFX_STATUS_KEY_CONFIG_DEVICE_5G_VOWIFI_AVAILABLE) == "1";
        int value = ImsConfigUtils::getSystemPropValueWithDefault(
                        ImsConfigUtils::PROPERTY_5G_VOWIFI_FORCE_ENABLE, "-1");
        if (value != -1) {
            //already set force value if != -1
            //only 1 is force enabled
            if (value == 1) {
                nrVowifi = 1;
            } else {
                nrVowifi = 0;
            }
        }

        if (nrVowifi == 1) {
            if (params[2] == 1) {
                params[2] = 3;
            }
            if (params[3] == 1) {
                params[3] = 3;
            }
        }
    }

    char boot_reason[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("sys.boot.reason", boot_reason, "0");
    if (strstr(boot_reason, "reboot,ota") != NULL
            || strstr(boot_reason, "system-update") != NULL) {
        logI(RFX_LOG_TAG, "sendImsSettingstoMcl, boot_reason = %s", boot_reason);
        if (params[0] > 0) {
            mIsOtaWait = true;
        }
    }

    logD(RFX_LOG_TAG,
         "sendImsSettingstoMcl, volte:%d, vilte:%d, vowifi:%d, viwifi:%d, sms:%d, phoneId:%d",
         params[0], params[1], params[2], params[3], params[4], slotId);
      if (params[0] | params[1] | params[2] | params[3] | params[4] | (isAllowTurnOff^1)) {
          params[5] = 1;  // eims enable
      } else {
          params[5] = 0;  // eims disable
      }

      sp<RfxMessage> message =  RfxMessage::obtainRequest(
                                          getSlotId(),
                                          RFX_MSG_REQUEST_SET_IMSCFG_EX,
                                          RfxIntsData(params, 6));
      // delay 600ms wait AT+EFUN and exist ims disable in RtcImsConfigController
      if (mIsOtaWait) {
          RfxMainThread::enqueueMessage(message);
      } else {
          requestToMcl(message, false, ms2ns(600));
      }
}

void RtcImsController::setDefaultVolteState(int slot_id, const char new_iccid[], int card_type) {
    if (sInitDone == false) {
        for (int slot = 0; slot < RFX_SLOT_COUNT; slot++) {
            rfx_property_get(PROPERTY_LAST_ICCID_SIM[slot], sLastBootIccId[slot], "null");
        }
        char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get(PROPERTY_VOLTE_STATE, tempstr, "0");
        sLastBootVolteState = atoi(tempstr);
        logD(RFX_LOG_TAG, "ReinitOp09Ims, sLastBootVolteState = %d", sLastBootVolteState);
        sInitDone = true;
    }

    if (!isOp09SimCard(slot_id, new_iccid, card_type)) {
        // To speed up bootup ims registration, we add checkNonOp09VolteState
        // if open setNonOp09VolteState, need disable checkNonOp09VolteState
        checkNonOp09VolteState(slot_id, new_iccid);
        // setNonOp09VolteState(slot_id, new_iccid);
        mIsSimSwitch = false;
        mIsBootUp = false;
        return;
    }

    char oldIccIdStr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    // multi ims need save every sim iccid
    rfx_property_get(PROPERTY_LAST_ICCID_SIM[slot_id], oldIccIdStr, "null");
    logD(RFX_LOG_TAG, "setDefaultVolteState, oldIccIdStr = %s", givePrintableStr(oldIccIdStr));

    char hashedNewIccIdStr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    string hashedResult = hash256(new_iccid);
    if (!hashedResult.empty()) {
        int length =
            snprintf(hashedNewIccIdStr, sizeof(hashedNewIccIdStr), "%s", hashedResult.c_str());
        if (length < 0) {
            logD(RFX_LOG_TAG, "setDefaultVolteState, hash256 ICC id error.");
        }
    } else {
        logD(RFX_LOG_TAG, "setDefaultVolteState, hash256 result is empty.");
    }

    // CT 3G SIM shall be off
    if (card_type == CT_3G_UIM_CARD ||
            card_type == CT_UIM_SIM_CARD) {
        rfx_property_set(PROPERTY_LAST_ICCID_SIM[slot_id], hashedNewIccIdStr);
        setVolteStateProperty(slot_id, 0);
        if (RfxRilUtils::isMultipleImsSupport() == 1 || mMainSlotId == slot_id) {
            sendDefaultVolteStateUrc(slot_id, 0);
        }
        mIsBootUp = false;
        mIsSimSwitch = false;
        logD(RFX_LOG_TAG,"setDefaultVolteState, CT 3G case");
        return;
    }

    int setValue = 1;
    if (RfxRilUtils::isMultipleImsSupport() == 0) {
        if (strlen(oldIccIdStr) == 0 || strcmp("null", oldIccIdStr) == 0 ||
            strcmp(hashedNewIccIdStr, oldIccIdStr) != 0 ||
            mIsSimSwitch ||
            mIsBootUp) {
            rfx_property_set(PROPERTY_LAST_ICCID_SIM[slot_id], hashedNewIccIdStr);
            if (mIsBootUp) {
                for (int slot = 0; slot < RFX_SLOT_COUNT; slot++) {
                    if (strcmp(hashedNewIccIdStr, sLastBootIccId[slot]) == 0) {
                        // For switch sim when powoff case, boot up need get before setting
                        int state = sLastBootVolteState;
                        setValue = (state >> slot) & 1;
                        logD(RFX_LOG_TAG,
                            "setDefaultVolteState, change sim slot reboot, last= %d, %d",
                            state, setValue);
                        break;
                    }
                }
                mIsBootUp = false;
            }

            if (mIsSimSwitch && (strcmp(hashedNewIccIdStr, oldIccIdStr) == 0)) {
                char volteState[RFX_PROPERTY_VALUE_MAX] = { 0 };
                rfx_property_get(PROPERTY_VOLTE_STATE, volteState, "0");
                int stateValue = atoi(volteState);
                setValue = (stateValue >> slot_id) & 1;
                logD(RFX_LOG_TAG, "setDefaultVolteState, sim switch, setValue = %d", setValue);
            } else {
                setVolteStateProperty(slot_id, setValue);
            }
            mIsSimSwitch = false;

            // for single ims, only set volte state and send request for main capability sim.
            if (mMainSlotId == slot_id) {
                // state diff, send volte state to IMS FW
                sendDefaultVolteStateUrc(slot_id, setValue);
            }
        }
    } else {
        if (strlen(oldIccIdStr) == 0 || strcmp("null", oldIccIdStr) == 0 ||
            strcmp(hashedNewIccIdStr, oldIccIdStr) != 0 ||
            mIsBootUp) {
            rfx_property_set(PROPERTY_LAST_ICCID_SIM[slot_id], hashedNewIccIdStr);
            if (mIsBootUp) {
                for (int slot = 0; slot < RFX_SLOT_COUNT; slot++) {
                    if (strcmp(hashedNewIccIdStr, sLastBootIccId[slot]) == 0) {
                        // For switch sim when powoff case, boot up need get before setting
                        if (slot != slot_id
                            && strcmp(hashedNewIccIdStr, sLastBootIccId[slot_id]) == 0) {
                            // use same slot firstly
                            slot = slot_id;
                        }
                        int state = sLastBootVolteState;
                        setValue = (state >> slot) & 1;
                        logD(RFX_LOG_TAG,
                            "setDefaultVolteState, change sim slot reboot, last= %d, %d",
                            state, setValue);
                        break;
                    }
                }
                mIsBootUp = false;
            }

            setVolteStateProperty(slot_id, setValue);

            // send volte state to IMS FW
            sendDefaultVolteStateUrc(slot_id, setValue);
        }
    }
}

void RtcImsController::setVolteStateProperty(int slot_id, bool isEnable) {
    char volteState[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(PROPERTY_VOLTE_STATE, volteState, "0");
    int stateValue = atoi(volteState);

    if (isEnable == 1) {
        stateValue = stateValue | (1 << slot_id);
    } else {
        stateValue = stateValue & (~(1 << slot_id));
    }
    char temp[3] = {0};
    if (stateValue > 10) {
        temp[0] = '1';
        temp[1] = '0' + (stateValue - 10);
    } else {
        temp[0] = '0' + stateValue;
    }
    rfx_property_set(PROPERTY_VOLTE_STATE, temp);
    logD(RFX_LOG_TAG, "setVolteStateProperty, new volte_state = %d, %s", stateValue,
        temp);
}

bool RtcImsController::isOp09SimCard(int slot_id, const char icc_id[], int card_type) {
    RFX_UNUSED(slot_id);
    RFX_UNUSED(card_type);
    bool isOp09Card = false;
    if (strncmp(icc_id, "898603", 6) == 0 ||
        strncmp(icc_id, "898611", 6) == 0 ||
        strncmp(icc_id, "8985302", 7) == 0 ||
        strncmp(icc_id, "8985307", 7) == 0 ||
        strncmp(icc_id, "8985231", 7) == 0) {
        isOp09Card = true;
    }
    return isOp09Card;
}

void RtcImsController::sendDefaultVolteStateUrc(int slot_id, int value) {
    // send default volte state to ims ril adapter
    int response[2];
    response[0] = value;
    response[1] = slot_id;
    sp<RfxMessage> msg = RfxMessage::obtainUrc(
            slot_id,
            RIL_MSG_UNSOL_VOLTE_SETTING,
            RfxIntsData(response, 2));
    responseToRilj(msg);
    ImsConfigUtils::setAllowForceNotify(slot_id, true, value);
}

void RtcImsController::handleSetImsConfigRequest(const sp<RfxMessage>& message) {
    //void *data = message->getData()->getData();
    //int imsEnable = ((int*)data)[5];
    mIsImsDisabling = false;
    //logD(RFX_LOG_TAG, "imsEnable=%d, mIsImsDisabling=%d", imsEnable, mIsImsDisabling);

    setVolteSettingStatus(message);
    requestToMcl(message);
}

void RtcImsController::handleSetImsConfigResponse(const sp<RfxMessage>& message) {
    responseToRilj(message);
    logD(RFX_LOG_TAG, "handleSetImsConfigResponse");
}

void RtcImsController::setVolteSettingStatus(const sp<RfxMessage>& message) {
    if (!RfxRilUtils::isCtVolteSupport() && !RfxRilUtils::isCtMixVolteSupport()) {
        return;
    }

    void *data = message->getData()->getData();
    int value = ((int*)data)[0];
    int slot_id = getSlotId();
    logD(RFX_LOG_TAG, "setVolteSettingStatus: value = %d, slot:%d ", value, slot_id);

    getStatusManager()-> setIntValue(RFX_STATUS_KEY_VOLTE_STATE, value);

    if (message->getId() == RFX_MSG_REQUEST_SET_IMSCFG) {
        //req is directly from hidl
        logI(RFX_LOG_TAG, "setVolteSettingStatus, change volte.enable to %d", value);
        char isEnable[10];
        if (sprintf(isEnable, "%d", value) < 0) {
            logE(RFX_LOG_TAG, "setVolteSettingStatus, sprintf to isEnable fail");
        }
        ImsConfigUtils::setFeaturePropValue(
            ImsConfigUtils::PROPERTY_VOLTE_ENALBE,
            isEnable,
            slot_id);
    }

    int sim_state = getStatusManager(slot_id)->getIntValue(
                                    RFX_STATUS_KEY_SIM_STATE);
    if (sim_state == RFX_SIM_STATE_NOT_READY) {
        logD(RFX_LOG_TAG, "setVolteSettingStatus: skip set for invalid sim type");
    } else if ((RfxRilUtils::isMultipleImsSupport() == 0) && (slot_id != mMainSlotId)) {
        logD(RFX_LOG_TAG, "setVolteSettingStatus: skip set for it's not main sim");
    } else if (message->getId() == RFX_MSG_REQUEST_SET_IMSCFG_EX) {
        char volteEnable[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get(PROPERTY_VOLTE_ENABLE, volteEnable, "0");
        int enalbeValue = atoi(volteEnable);
        char volteState[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get(PROPERTY_VOLTE_STATE, volteState, "0");
        int stateValue = atoi(volteState);
        if (RfxRilUtils::isMultipleImsSupport() == 0) {
            if ((enalbeValue & 0x01) == 1) {
                stateValue = stateValue | (1 << slot_id);
            } else {
                stateValue = stateValue & (~(1 << slot_id));
            }
        } else {
            if (((enalbeValue >> slot_id) & 0x01) == 1) {
                stateValue = stateValue | (1 << slot_id);
            } else {
                stateValue = stateValue & (~(1 << slot_id));
            }
        }
        char temp[3] = { 0 };
        if (stateValue > 10) {
            temp[0] = '1';
            temp[1] = '0' + (stateValue - 10);
        } else {
            temp[0] = '0' + stateValue;
        }
        rfx_property_set(PROPERTY_VOLTE_STATE, temp);
        logD(RFX_LOG_TAG, "setVolteSettingStatus: state = %d, %s", stateValue, temp);
    }
}

const std::string RtcImsController::hash256(const char* str) {
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    SHA256_Final(hash, &sha256);

    string tmp = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        int length = snprintf(buf, sizeof(buf), "%02x", hash[i]);
        if (length >= 0) {
            tmp = tmp + buf;
        }
    }
    return tmp;
}

const char* RtcImsController::givePrintableStr(const char* iccId) {
    static char iccIdToPrint[RFX_PROPERTY_VALUE_MAX] = { 0 };
    if (strlen(iccId) > 6) {
        strncpy(iccIdToPrint, iccId, 6);
        return iccIdToPrint;
    }
    return iccId;
}
/// @}

void RtcImsController::updateImsRegStatus() {
    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_SET_IMS_REGISTRATION_REPORT, RfxVoidData());
    requestToMcl(msg);
}

void RtcImsController::handleImsDisableDoneUrc(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    mIsImsDisabling = false;
    //logD(RFX_LOG_TAG, "mIsImsDisabling = %d", mIsImsDisabling);
}

bool RtcImsController::onPreviewMessage(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    //logD(RFX_LOG_TAG, "onPreviewMessage - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
        case RFX_MSG_REQUEST_SET_IMSCFG_EX: {
            if (message->getType() != RFX_MESSAGE_TYPE::REQUEST) {
                return true;
            }
            int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
            if (mIsOtaWait && simState == RFX_SIM_STATE_NOT_READY) {
                logI(RFX_LOG_TAG, "onPreviewMessage - %s, simState:%d", RFX_ID_TO_STR(msg_id), simState);
                return false;
            } else {
                mIsOtaWait = false;
            }

            // Queue the AT+EIMSCFG during IMS disabling
            if(mIsImsDisabling != false){
                logD(RFX_LOG_TAG, "onPreviewMessage - %s, mIsImsDisabling -  %d", RFX_ID_TO_STR(msg_id), mIsImsDisabling);
            }
            return (mIsImsDisabling == false);
        }
        default:
            break;
    }
    return true;
}

bool RtcImsController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    // logD(RFX_LOG_TAG, "onCheckIfResumeMessage - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
        case RFX_MSG_REQUEST_SET_IMSCFG_EX: {
            int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
            bool isSimOff = (getStatusManager()->getIntValue(RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
                               == UICC_APPLICATIONS_ENABLE_STATE_DISABLED);
            if (mIsOtaWait && simState == RFX_SIM_STATE_NOT_READY && !isSimOff) {
                return false;
            } else {
                if (mIsOtaWait) {
                    logI(RFX_LOG_TAG, "onCheckIfResumeMessage -Ota resume %s, simState:%d",
                        RFX_ID_TO_STR(msg_id), simState);
                }
                mIsOtaWait = false;
            }

            // Resume any AT+EIMSCFG after IMS disabled
            return (mIsImsDisabling == false);
        }
        default:
            break;
    }
    return true;
}

void RtcImsController::cacheimscfgparams(const sp<RfxMessage>& message) {
    int curSlotId = getSlotId();
    if (curSlotId < 0 || curSlotId >= 4) {
        logE(RFX_LOG_TAG, "cacheimscfgparams(), error slot:%d", curSlotId);
        return;
    }

    void *data = message->getData()->getData();

    if (message->getId() == RFX_MSG_REQUEST_SET_IMSCFG) {
        char feature[] = "IMS over 3gpp";
        int featureVersion = getFeatureVersion(feature);
        int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
        if (featureVersion > 1 && simState == RFX_SIM_STATE_ABSENT) {
            logD(RFX_LOG_TAG, "cacheimscfgparams(), sim absent case, need check VoNR");
            int allEnable = 3; //VoLTE and VoNR enable
            if (((int*)data)[0] == 1) { //VoLTE enabled
                ((int*)data)[0] = allEnable;
            }
            if (((int*)data)[1] == 1) {//VoiLTE enabled
                ((int*)data)[1] = allEnable;
            }
        }
    }

    imscfgcache[curSlotId][0] = ((int*)data)[0];
    imscfgcache[curSlotId][1] = ((int*)data)[1];
    imscfgcache[curSlotId][2] = ((int*)data)[2];
    imscfgcache[curSlotId][3] = ((int*)data)[3];
    imscfgcache[curSlotId][4] = ((int*)data)[4];
    imscfgcache[curSlotId][5] = ((int*)data)[5];
}

void RtcImsController::mmsoverruleimscfg(const sp<RfxMessage>& message) {
    void *data = message->getData()->getData();
    ((int*)data)[0] = 0;
    ((int*)data)[1] = 0;
    ((int*)data)[2] = 0;
    ((int*)data)[3] = 0;
    ((int*)data)[4] = 0;
    ((int*)data)[5] = 0;
}

void RtcImsController::sendCachedXui() {
    if (mCachedXuiUrc != NULL) {
        responseToRilj(mCachedXuiUrc);
    }
}

void RtcImsController::handleImsRegInfoInd(const sp<RfxMessage>& message) {
    RfxImsRegInfoData* data = (RfxImsRegInfoData*)message->getData();
    RIL_ImsRegInfo* params = (RIL_ImsRegInfo*)data->getData();
    getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REG_IND_TYPE, params->report_type);
    getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REG_IND_ACCOUNT_ID, params->account_id);
    if (params->report_type == IMS_REGISTER_FAIL) {
        // Update cache for the case registration fail
        getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REG_IND_EXPIRE_TIME, -1);
        getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REG_IND_ERROR_CODE, params->error_code);
        getStatusManager()->setString8Value(RFX_STATUS_KEY_IMS_REG_IND_URI, String8(params->uri));
        getStatusManager()->setString8Value(RFX_STATUS_KEY_IMS_REG_IND_ERROR_MSG,
                String8(params->error_msg));
    } else {
        // Update cache for the case registering and registered
        getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REG_IND_EXPIRE_TIME,
                params->expire_time);
        getStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_REG_IND_ERROR_CODE, 0);
        getStatusManager()->setString8Value(RFX_STATUS_KEY_IMS_REG_IND_URI, String8(params->uri));
        getStatusManager()->setString8Value(RFX_STATUS_KEY_IMS_REG_IND_ERROR_MSG, String8(""));
    }

    logD(RFX_LOG_TAG, "[%d]handleImsRegInfoInd, (%d, %d, %d, %d, %s)", m_slot_id,
            params->report_type, params->account_id, params->expire_time,
            params->error_code, params->error_msg);
}

void RtcImsController::sendCachedImsRegInfo() {
    sp<RfxMessage> unsol;
    RIL_ImsRegInfo response;

    memset(&response, 0, sizeof(RIL_ImsRegInfo));

    response.report_type = getStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REG_IND_TYPE, -1);
    if (response.report_type >= 0) {
        response.account_id =
                getStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REG_IND_ACCOUNT_ID, -1);
        response.expire_time =
                getStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REG_IND_EXPIRE_TIME, -1);
        response.error_code =
                getStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REG_IND_ERROR_CODE, 0);
        response.uri =
                (char*)getStatusManager()->getString8Value(
                RFX_STATUS_KEY_IMS_REG_IND_URI, String8("")).string();
        response.error_msg =
                (char*)getStatusManager()->getString8Value(
                RFX_STATUS_KEY_IMS_REG_IND_ERROR_MSG, String8("")).string();

        logD(RFX_LOG_TAG, "[%d]sendCachedImsRegInfo, (%d, %d, %d, %d, %s)", m_slot_id,
                response.report_type, response.account_id, response.expire_time,
                response.error_code, response.error_msg);

        unsol = RfxMessage::obtainUrc(RFX_MSG_UNSOL_IMS_REGISTRATION_STATE_IND, m_slot_id,
                RfxImsRegInfoData(&response, sizeof(RfxImsRegInfoData)));
        responseToRilj(unsol);
    }
}
