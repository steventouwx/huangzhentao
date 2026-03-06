/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2018. All rights reserved.
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
#include <stdlib.h>
#include <string>
#include <memory>
#include <sstream>

#include "RtmImsConfigController.h"
#include "RtmImsConfigDef.h"
#include "ImsConfigUtils.h"
#include "rfx_properties.h"
#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "ImsConfigDataHelper.h"
#include "RfxMainThread.h"

#include <prop/properties.h>
#include <libmtkrilutils.h>
#include <mtkconfigutils.h>

using ::android::String8;

#define RFX_LOG_TAG "RtmImsConfigController"

#define AP_WIFI_ONLY           0
#define AP_CELLULAR_PREFERRED  1
#define AP_WIFI_PREFERRED      2
#define AP_CELLULAR_ONLY       3
#define AP_4G_PREFER           4

#define RIL_4G_PREFER          4
#define RIL_WIFI_ONLY          3
#define RIL_CELLULAR_PREFERRED 2
#define RIL_WIFI_PREFERRED     1

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtmImsConfigController", RtmImsConfigController, RfxController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_IMS_CONFIG_SET_FEATURE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_IMS_CONFIG_GET_FEATURE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringData, RFX_MSG_REQUEST_IMS_CONFIG_GET_PROVISION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_IMS_CONFIG_GET_RESOURCE_CAP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_IMS_ENABLE_VONR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_IMS_IS_VONR_ENABLED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_IMS_CONFIG_DYNAMIC_IMS_SWITCH_COMPLETE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_IMS_CONFIG_FEATURE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_IMS_CONFIG_CONFIG_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_IMS_CONFIG_CONFIG_LOADED);

void RtmImsConfigController::onInit() {
    RfxController::onInit();  // Required: invoke super class implementation

    logD(RFX_LOG_TAG, "onInit()");

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SIM_STATE,
            RfxStatusChangeCallback(this, &RtmImsConfigController::onSimStateChanged));
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallback(this, &RtmImsConfigController::onRadioStateChanged));
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CARRIER_CONFIG_CHANGED,
            RfxStatusChangeCallback(this, &RtmImsConfigController::onCarrierConfigChanged));
    if (RfxRilUtils::isCtVolteSupport() || RfxRilUtils::isCtMixVolteSupport()) {
        getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SIM_ICCID,
                RfxStatusChangeCallback(this, &RtmImsConfigController::onSimIccidChanged));
    }
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_UICC_GSM_NUMERIC,
        RfxStatusChangeCallback(this, &RtmImsConfigController::onUiccMccMncChanged));

    resetFeatureSendCmd();
    resetFeatureResource();
    resetFeatureValue();
    resetFeatureSendValue();

    const int request_id_list[] = {
            RFX_MSG_REQUEST_IMS_CONFIG_SET_FEATURE,
            RFX_MSG_REQUEST_IMS_CONFIG_GET_FEATURE,
            RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION,
            RFX_MSG_REQUEST_IMS_CONFIG_GET_PROVISION,
            RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION,
            RFX_MSG_REQUEST_IMS_CONFIG_GET_RESOURCE_CAP,
            RFX_MSG_REQUEST_IMS_ENABLE_VONR,
            RFX_MSG_REQUEST_IMS_IS_VONR_ENABLED,
            RFX_MSG_REQUEST_SET_IMS_UA
    };
    //const int urc_id_list[] = {
    //        RFX_MSG_UNSOL_GET_PROVISION_DONE
    //};

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int));
    //registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));
}


void RtmImsConfigController::onDeinit() {
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_SIM_STATE,
            RfxStatusChangeCallback(this, &RtmImsConfigController::onSimStateChanged));
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallback(this, &RtmImsConfigController::onRadioStateChanged));
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CARRIER_CONFIG_CHANGED,
            RfxStatusChangeCallback(this, &RtmImsConfigController::onCarrierConfigChanged));
    if (RfxRilUtils::isCtVolteSupport() || RfxRilUtils::isCtMixVolteSupport()) {
        getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_SIM_ICCID,
                RfxStatusChangeCallback(this, &RtmImsConfigController::onSimIccidChanged));
    }
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_UICC_GSM_NUMERIC,
        RfxStatusChangeCallback(this, &RtmImsConfigController::onUiccMccMncChanged));
    RfxController::onDeinit();
}

void RtmImsConfigController::initProvisionValue() {
    int mccMnc = getCurrentMccMnc();

    logD(RFX_LOG_TAG, "initProvisionValue, mccMnc = %d", mccMnc);

    mProvisionValue.clear();

    vector<shared_ptr<ConfigValue>> data = ImsConfigDataHelper::getConfigData();

    for (int i = 0; i < (int)data.size(); i++) {
        if (data[i]->statusKey != RFX_STATUS_KEY_START) {
            String8 provStr = getStatusManager()->getString8Value(data[i]->statusKey, String8("n/a"));
            mProvisionValue[data[i]->configId] = std::string(provStr.string());
            logD(RFX_LOG_TAG, "initProvisionValue, configId:%d, Str:%s", data[i]->configId, provStr.string());
        } else {
            mProvisionValue[data[i]->configId] = data[i]->provsionValue;
        }
    }

    mInitDone = true;

    logD(RFX_LOG_TAG, "send config loaded urc");
    sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(),
                                               RFX_MSG_UNSOL_IMS_CONFIG_CONFIG_LOADED,
                                               RfxVoidData());
    responseToRilj(urc);
}

bool RtmImsConfigController::onHandleRequest(const sp<RfxMessage>& message){
    int msg_id = message->getId();
    logD(RFX_LOG_TAG, "onHandleRequest - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
        case RFX_MSG_REQUEST_IMS_CONFIG_SET_FEATURE:
            setFeatureValue(message);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_GET_FEATURE:
            getFeatureValue(message);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION:
            setProvisionValue(message);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_GET_PROVISION:
            getProvisionValue(message);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_GET_RESOURCE_CAP:
            getImsResourceCapability(message);
            break;
        case RFX_MSG_REQUEST_IMS_ENABLE_VONR:
            setVoNREnable(message);
            break;
        case RFX_MSG_REQUEST_IMS_IS_VONR_ENABLED:
            isVoNREnabled(message);
            break;
      case RFX_MSG_REQUEST_SET_IMS_UA:
            requestToMcl(message);
            break;
        default:
            break;
    }

    return true;
}

bool RtmImsConfigController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    logI(RFX_LOG_TAG, "onHandleUrc - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
        case RFX_MSG_UNSOL_GET_PROVISION_DONE:
            // handleGetProvisionUrc(message);
            break;
        default:
            responseToRilj(message);
    }
    return true;
}

bool RtmImsConfigController::onHandleResponse(const sp<RfxMessage>& message){
    int msg_id = message->getId();
    logI(RFX_LOG_TAG, "onHandleResponse - %s", RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
        case RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION:
            handleGetProvisionResponse(message);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION:
            handleSetProvisionResponse(message);
            break;
        case RFX_MSG_REQUEST_SET_IMS_UA:
            break;
        default:
            responseToRilj(message);
    }
    return true;
}

bool RtmImsConfigController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
            bool isModemPowerOff, int radioState) {
    RFX_UNUSED(isModemPowerOff);

    int msg_id = message->getId();

    if (msg_id == RFX_MSG_REQUEST_IMS_CONFIG_SET_FEATURE ||
        msg_id == RFX_MSG_REQUEST_IMS_CONFIG_GET_FEATURE ||
        msg_id == RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION ||
        msg_id == RFX_MSG_REQUEST_IMS_CONFIG_GET_PROVISION ||
        msg_id == RFX_MSG_REQUEST_IMS_CONFIG_GET_RESOURCE_CAP ||
        msg_id == RFX_MSG_REQUEST_IMS_ENABLE_VONR ||
        msg_id == RFX_MSG_REQUEST_IMS_IS_VONR_ENABLED ||
        msg_id == RFX_MSG_REQUEST_SET_IMS_UA) {
        return false;
    }
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

void RtmImsConfigController::onSimStateChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);

    int simState = value.asInt();
    logI(RFX_LOG_TAG, "onSimStateChanged, simState = %d", simState);

    if (simState == RFX_SIM_STATE_ABSENT || simState == RFX_SIM_STATE_PIN_REQUIRED || simState ==
            RFX_SIM_STATE_PUK_REQUIRED || simState == RFX_SIM_STATE_NETWORK_LOCKED
            || simState == RFX_SIM_STATE_PERM_DISABLED) {
        mECCAllowSendCmd = true;

        int slot_id = getSlotId();
        int volte = ImsConfigUtils::getFeaturePropValue(ImsConfigUtils::PROPERTY_VOLTE_ENALBE,
                                                        slot_id);
        int vilte = ImsConfigUtils::getFeaturePropValue(ImsConfigUtils::PROPERTY_VILTE_ENALBE,
                                                        slot_id);
        int wfc = ImsConfigUtils::getFeaturePropValue(ImsConfigUtils::PROPERTY_WFC_ENALBE, slot_id);
        if (volte == 1 && vilte == 1 && wfc == 1) {
            mECCAllowNotify = true;
        }
    }

    if(simState == RFX_SIM_STATE_ABSENT || simState == RFX_SIM_STATE_NOT_READY || simState ==
            RFX_SIM_STATE_PIN_REQUIRED || simState == RFX_SIM_STATE_PUK_REQUIRED || simState ==
            RFX_SIM_STATE_NETWORK_LOCKED || simState == RFX_SIM_STATE_PERM_DISABLED) {
        resetFeatureSendCmd();
    }

    if(simState != RFX_SIM_STATE_READY) {
        // For SIM state ready, will wait for onCarrierConfigChanged()
        processDynamicImsSwitch();
    }
}

void RtmImsConfigController::onUiccMccMncChanged(RfxStatusKeyEnum key,
        RfxVariant old_value, RfxVariant value) {
    RFX_UNUSED(key);

    const String8 old_mccmnc = old_value.asString8();
    const String8 new_mccmnc = value.asString8();
    if (new_mccmnc.compare(old_mccmnc) == 0) {
        logI(RFX_LOG_TAG, "onUiccMccMncChanged(), same mccmnc");
        return;
    }

    int currentMccMnc = getCurrentMccMnc();
    logD(RFX_LOG_TAG, "onUiccMccMncChanged(), mccmnc:%d", currentMccMnc);
    if (isImsEnabledOp15ProvisioningConfig() && currentMccMnc == 23410) {
        triggerImsDisableCommand(getSlotId());
    } else if (isImsEnabledSetUserAgent()
            // For more mccmnc, it can be customized
            && (currentMccMnc == 26006 || currentMccMnc == 20801 || currentMccMnc == 26003)) {
        sp<RfxMessage> message =  RfxMessage::obtainRequest(
                                            getSlotId(),
                                            RFX_MSG_REQUEST_SET_IMS_UA,
                                            RfxVoidData());
         RfxMainThread::enqueueMessage(message);
    }
}

void RtmImsConfigController::triggerImsDisableCommand(int slot_id) {
    if (!ImsConfigUtils::isPhoneIdSupportIms(slot_id)) {
        logI(RFX_LOG_TAG,
             "triggerImsDisableCommand, Do not send EIMS feature values for phone = %d",slot_id);
        return;
     }

    char isEnable[10] = { 0 };

    if (sprintf(isEnable, "%d", 0) < 0) {
        logE(RFX_LOG_TAG, "triggerImsDisableCommand, sprintf to isEnable fail");
    }

    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VOLTE_ENALBE,
                            isEnable,
                            slot_id);
    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_WFC_ENALBE,
                            isEnable,
                            slot_id);
    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VIWIFI_ENALBE,
                            isEnable,
                            slot_id);
    triggerImsCfgCommand(slot_id);
}

void RtmImsConfigController::processDynamicImsSwitch() {
    if (ImsConfigUtils::getSystemPropValue(ImsConfigUtils::PROPERTY_DYNAMIC_IMS_SWITCH_SUPPORT) !=
        1) {
        logI(RFX_LOG_TAG, "Do not support DYNAMIC IMS SWITCH, return!");
        return;
    }

    int currentMccMnc  = getCurrentMccMnc();

    logD(RFX_LOG_TAG, "processDynamicImsSwitch, currentMccMnc = %d", currentMccMnc);

    // Dynamic SIM Switch start
    int volte = FEATURE_VALUE_OFF;
    int vilte = FEATURE_VALUE_OFF;
    int vowifi = FEATURE_VALUE_OFF;
    int viwifi = FEATURE_VALUE_OFF;
    int vonr = FEATURE_VALUE_OFF;
    int vinr = FEATURE_VALUE_OFF;
    char feature[] = "IMS over 3gpp";
    int featureVersion = getFeatureVersion(feature);

    if (mMccmnc != currentMccMnc) {
        mMccmnc = currentMccMnc;

        if (mMccmnc == 0) {
            volte = FEATURE_VALUE_ON;
            vilte = FEATURE_VALUE_ON;
            if (featureVersion > 1) {
                vonr = FEATURE_VALUE_ON;
                vinr = FEATURE_VALUE_ON;
            }
        } else {
            if (!ImsConfigUtils::isTestSim(getSlotId())) {
                volte = getStatusManager()->getString8Value(
                        RFX_STATUS_KEY_CONFIG_DEVICE_VOLTE_AVAILABLE) == "1";
                vilte = getStatusManager()->getString8Value(
                        RFX_STATUS_KEY_CONFIG_DEVICE_VT_AVAILABLE) == "1";
                vowifi = getStatusManager()->getString8Value(
                        RFX_STATUS_KEY_CONFIG_DEVICE_WFC_AVAILABLE) == "1";
                viwifi = vilte; // Currently, only one DIMS config for ViLTE & ViWifi.
                if (featureVersion > 1) {
                    vonr = getStatusManager()->getString8Value(
                            RFX_STATUS_KEY_CONFIG_DEVICE_VONR_AVAILABLE) == "1";
                    vinr = getStatusManager()->getString8Value(
                            RFX_STATUS_KEY_CONFIG_DEVICE_VINR_AVAILABLE) == "1";
                }

                String8 iccid = getStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);
                logD(RFX_LOG_TAG, "DIMS volte:%d, vilte:%d, wfc:%d, vonr:%d, vinr:%d, imsVersion:%d",
                            volte, vilte, vowifi, vonr, vinr, featureVersion);

                int ctMccmnc = mMccmnc;
                if (!iccid.isEmpty()) {
                    //int mcc, mnc;
                    String8 APTG_ICCID = String8("8988605");
                    String8 CT_ICCID_1 = String8("898603");
                    String8 CT_ICCID_2 = String8("898611");
                    if (strncmp(APTG_ICCID, iccid.string(), 7) == 0) {
                        //mcc = 466;
                        //mnc = 5;
                        // special case for 46605
                        logI(RFX_LOG_TAG, "special case for APTG");
                        volte = FEATURE_VALUE_ON;
                        vilte = FEATURE_VALUE_ON;
                        vowifi = FEATURE_VALUE_ON;
                        viwifi = FEATURE_VALUE_ON;
                    } else if (mMccmnc == 46011 || mMccmnc == 46003
                               || strncmp(CT_ICCID_1, iccid.string(), 6) == 0
                               || strncmp(CT_ICCID_2, iccid.string(), 6) == 0) {
                        ctMccmnc = 46003;
                        // special case for 46003 in case some CT sim use 20404 to get wrong result
                        logI(RFX_LOG_TAG, "special case for CT");
                        volte = FEATURE_VALUE_ON;
                        vilte = FEATURE_VALUE_ON;
                        vowifi = FEATURE_VALUE_ON;
                        viwifi = FEATURE_VALUE_ON;
                        if (featureVersion > 1) {
                            vonr = FEATURE_VALUE_ON;
                            vinr = FEATURE_VALUE_ON;
                        }
                    }

                    // cnop vowifi support by AP config
                    int cnopVoWifiSupport = ImsConfigUtils::getCnopVoWifiSupportState();
                    // cnop vowifi support by modem config
                    char wfcFeature[] = "CN_WFC";
                    int wfcFeatureVersion = getFeatureVersion(wfcFeature);
                    logD(RFX_LOG_TAG, "CNOP vowifi modem config: %d, ap config: %d",
                            wfcFeatureVersion, cnopVoWifiSupport);
                    if (wfcFeatureVersion >= 1 || cnopVoWifiSupport >= 1) {
                        cnopVoWifiSupport = FEATURE_VALUE_ON;
                    }
                    if (vowifi == FEATURE_VALUE_ON && cnopVoWifiSupport == FEATURE_VALUE_OFF &&
                            ImsConfigUtils::isOp09SimCard(iccid.string())) {
                        vowifi = FEATURE_VALUE_OFF;
                        viwifi = FEATURE_VALUE_OFF;
                    }
                }
            } else {
                volte = FEATURE_VALUE_ON;
                vilte = FEATURE_VALUE_ON;
                // ALPS05557725 operator 313110 and 313120 doesn't support WFC
                if (mMccmnc != 313110 && mMccmnc != 313120) {
                    vowifi = FEATURE_VALUE_ON;
                    viwifi = FEATURE_VALUE_ON;
                }
                if (featureVersion > 1) {
                    vonr = FEATURE_VALUE_ON;
                    vinr = FEATURE_VALUE_ON;
                }
            }
        }

        logI(RFX_LOG_TAG,
                "DIMS setImsResourceCapability, volte:%d, vilte:%d, wfc:%d, vonr:%d, vinr:%d",
                volte, vilte, vowifi, vonr, vinr);

        setImsResourceCapability(FEATURE_TYPE_VOICE_OVER_LTE, volte, false);
        setImsResourceCapability(FEATURE_TYPE_VIDEO_OVER_LTE, vilte, false);
        setImsResourceCapability(FEATURE_TYPE_VOICE_OVER_WIFI, vowifi, false);
        setImsResourceCapability(FEATURE_TYPE_VIDEO_OVER_WIFI, viwifi, false);
        setImsResourceCapability(FEATURE_TYPE_VOICE_OVER_NR, vonr, false);
        setImsResourceCapability(FEATURE_TYPE_VIDEO_OVER_NR, vinr, true);
    }
}

void RtmImsConfigController::onCarrierConfigChanged(RfxStatusKeyEnum key,
                                               RfxVariant old_value, RfxVariant value) {
    logI(RFX_LOG_TAG, "onCarrierConfigChanged()");
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);
    RFX_UNUSED(value);

    resetFeatureResource();
    resetFeatureValue();
    resetFeatureSendCmd();
    initProvisionValue();

    loadProvisionedValue();

    processDynamicImsSwitch();
}

void RtmImsConfigController::onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value,
        RfxVariant value) {
    int oldState = -1, newState = -1;

    RFX_UNUSED(key);
    oldState = old_value.asInt();
    newState = value.asInt();

    logI(RFX_LOG_TAG, "onRadioStateChanged, state %d->%d", oldState, newState);
    int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
    if (newState == RADIO_STATE_ON && getCurrentMccMnc() == 0
        && (getStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, false)
            && simState == RFX_SIM_STATE_NOT_READY)) {
        // for sim off and empty euicc case ALPS08471315
        processDynamicImsSwitch();
    }

    if (newState != RADIO_STATE_UNAVAILABLE) {
        int slot_id = getSlotId();
        int currentMccmnc = getCurrentMccMnc();
        String8 currentIccid = getStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

        if ((mSendCfgMccmnc != -1 && mSendCfgMccmnc != 0) && !mSendCfgIccid.isEmpty() &&
                (currentIccid.compare(mSendCfgIccid) == 0) && (currentMccmnc == mSendCfgMccmnc)) {
            logD(RFX_LOG_TAG, "onRadioStateChanged, same sim, triggerImsCfgCommand");
            triggerImsCfgCommand(slot_id);
            // triggerVodataCfgCommand(slot_id);
            // triggerImsAddCapabilityCommand(mFeatureSendValue[FEATURE_TYPE_CALL_COMPOSER]);
        } else if (newState == RADIO_STATE_ON && currentMccmnc == 0
                   && (simState == RFX_SIM_STATE_ABSENT
                       || (getStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, false)
                           && simState == RFX_SIM_STATE_NOT_READY))) {
            logD(RFX_LOG_TAG, "onRadioStateChanged, sim absent||notready and radio on, triggerImsCfgCommand");
            triggerImsCfgCommand(slot_id);
            // triggerVodataCfgCommand(slot_id);
            // triggerImsAddCapabilityCommand(mFeatureSendValue[FEATURE_TYPE_CALL_COMPOSER]);
        } else {
            resetFeatureSendCmd();
        }
    }
    /// M: ALPS04270086 airplane mode ECC issue. @{
    if (newState == RADIO_STATE_ON && (simState == RFX_SIM_STATE_ABSENT || simState ==
            RFX_SIM_STATE_PIN_REQUIRED || simState == RFX_SIM_STATE_PUK_REQUIRED || simState ==
            RFX_SIM_STATE_NETWORK_LOCKED || simState == RFX_SIM_STATE_PERM_DISABLED)) {
        mECCAllowSendCmd = true;
        logI(RFX_LOG_TAG, "onRadioStateChanged, mECCAllowSendCmd = true");
    }
    /// @}
}

// CT card only special deal for speeding up ims register
void RtmImsConfigController::onSimIccidChanged(RfxStatusKeyEnum key, RfxVariant old_value,
          RfxVariant value) {
      RFX_UNUSED(key);
      String8 oldIccid = old_value.asString8();
      String8 newIccid = value.asString8();
      logD(RFX_LOG_TAG, "onSimIccidChanged()");
      // If the ICCID is not valid (SIM not ready / absent), then just return directly
      if (newIccid.isEmpty() || strcmp(newIccid.string(), "N/A") == 0) {
          return;
      }

      char feature[] = "IMS over 3gpp";
      int featureVersion = getFeatureVersion(feature);
      // Compare the ICCID to know if SIM is changed
      if (newIccid.compare(oldIccid) != 0) {
          if (ImsConfigUtils::isOp09SimCard(newIccid.string())
                  || ImsConfigUtils::isOp01SimCard(newIccid.string())) {
              setImsResourceCapability(FEATURE_TYPE_VOICE_OVER_LTE, FEATURE_VALUE_ON, false);
              setImsResourceCapability(FEATURE_TYPE_VIDEO_OVER_LTE, FEATURE_VALUE_ON, false);
              if (featureVersion > 1) {
                  setImsResourceCapability(FEATURE_TYPE_VOICE_OVER_NR, FEATURE_VALUE_ON, false);
                  setImsResourceCapability(FEATURE_TYPE_VIDEO_OVER_NR, FEATURE_VALUE_ON, false);
              }
          }
      }
  }

string RtmImsConfigController::convertToString(int value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

void RtmImsConfigController::resetFeatureSendCmd() {
    mFeatureSendCmd[FEATURE_TYPE_VOICE_OVER_LTE] = false;
    mFeatureSendCmd[FEATURE_TYPE_VIDEO_OVER_LTE] = false;
    mFeatureSendCmd[FEATURE_TYPE_VOICE_OVER_WIFI] = false;
    mFeatureSendCmd[FEATURE_TYPE_VIDEO_OVER_WIFI] = false;
    mFeatureSendCmd[FEATURE_TYPE_VOICE_OVER_NR] = false;
    mFeatureSendCmd[FEATURE_TYPE_VIDEO_OVER_NR] = false;
    mFeatureSendCmd[FEATURE_TYPE_VOICE_CROSS_SIM] = false;
    mFeatureSendCmd[FEATURE_TYPE_CALL_COMPOSER] = false;
}

void RtmImsConfigController::resetFeatureResource() {
    mMccmnc = -1;
    mFeatureResource[FEATURE_TYPE_VOICE_OVER_LTE] = FEATURE_RESOURCE_ON;
    mFeatureResource[FEATURE_TYPE_VIDEO_OVER_LTE] = FEATURE_RESOURCE_OFF;
    mFeatureResource[FEATURE_TYPE_VOICE_OVER_WIFI] = FEATURE_RESOURCE_OFF;
    mFeatureResource[FEATURE_TYPE_VIDEO_OVER_WIFI] = FEATURE_RESOURCE_OFF;
    mFeatureResource[FEATURE_TYPE_VOICE_OVER_NR] = FEATURE_RESOURCE_OFF;
    mFeatureResource[FEATURE_TYPE_VIDEO_OVER_NR] = FEATURE_RESOURCE_OFF;
}

void RtmImsConfigController::resetFeatureValue() {
    int slot_id = getSlotId();

    mVoLteFeatureValue.clear();
    mViLteFeatureValue.clear();
    mVoWifiFeatureValue.clear();
    mViWifiFeatureValue.clear();
    mVoNrFeatureValue.clear();
    mViNrFeatureValue.clear();
    mVoDataFeatureValue.clear();

    mVoLteFeatureValue[NETWORK_TYPE_LTE] = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VOLTE_ENALBE, slot_id);
    mViLteFeatureValue[NETWORK_TYPE_LTE] = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VILTE_ENALBE, slot_id);
    mVoWifiFeatureValue[NETWORK_TYPE_IWLAN] = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_WFC_ENALBE, slot_id);
    mViWifiFeatureValue[NETWORK_TYPE_IWLAN] = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VIWIFI_ENALBE, slot_id);
    mVoNrFeatureValue[NETWORK_TYPE_NR] = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VONR_ENALBE, slot_id);
    mViNrFeatureValue[NETWORK_TYPE_NR] = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VINR_ENALBE, slot_id);
    mVoDataFeatureValue[NETWORK_TYPE_LTE] = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VODATA_ENALBE, slot_id);
}

void RtmImsConfigController::resetFeatureSendValue() {
    mFeatureSendValue[FEATURE_TYPE_VOICE_OVER_LTE] = FEATURE_VALUE_OFF;
    mFeatureSendValue[FEATURE_TYPE_VIDEO_OVER_LTE] = FEATURE_VALUE_OFF;
    mFeatureSendValue[FEATURE_TYPE_VOICE_OVER_WIFI] = FEATURE_VALUE_OFF;
    mFeatureSendValue[FEATURE_TYPE_VIDEO_OVER_WIFI] = FEATURE_VALUE_OFF;
    mFeatureSendValue[FEATURE_TYPE_VOICE_OVER_NR] = FEATURE_VALUE_OFF;
    mFeatureSendValue[FEATURE_TYPE_VIDEO_OVER_NR] = FEATURE_VALUE_OFF;
    mFeatureSendValue[FEATURE_TYPE_VOICE_CROSS_SIM] = FEATURE_VALUE_OFF;
    mFeatureSendValue[FEATURE_TYPE_CALL_COMPOSER] = FEATURE_VALUE_OFF;
}

void RtmImsConfigController::setFeatureValue(const sp<RfxMessage>& message) {
    int *params = (int *)message->getData()->getData();
    int feature_id = params[0];
    int network = params[1];
    int value = params[2];

    logD(RFX_LOG_TAG, "setFeatureValue, feature_id:%d,network:%d,value:%d,isLast:%d",
                feature_id, network, value, params[3]);

    if (feature_id == FEATURE_TYPE_VOICE_OVER_LTE) {
        mVoLteFeatureValue[network] = value;
    } else if (feature_id == FEATURE_TYPE_VIDEO_OVER_LTE) {
        mViLteFeatureValue[network] = value;
    } else if (feature_id == FEATURE_TYPE_VOICE_OVER_WIFI) {
        mVoWifiFeatureValue[network] = value;
    } else if (feature_id == FEATURE_TYPE_VIDEO_OVER_WIFI) {
        mViWifiFeatureValue[network] = value;
    } else if (feature_id == FEATURE_TYPE_VOICE_OVER_NR) {
        mVoNrFeatureValue[network] = value;
    } else if (feature_id == FEATURE_TYPE_VIDEO_OVER_NR) {
        mViNrFeatureValue[network] = value;
    } else if (feature_id == FEATURE_TYPE_VOICE_CROSS_SIM) {
        mVoDataFeatureValue[network] = value;
    } else if (feature_id == FEATURE_TYPE_CALL_COMPOSER) {
        mCallComposerFeatureValue[network] = value;
    } else {
        logI(RFX_LOG_TAG, "setFeatureValue, feature_id:%d, value:%d not support",
             feature_id, value);
        sp<RfxMessage> response = RfxMessage::obtainResponse(
                            RIL_E_INVALID_ARGUMENTS, message, true);
        responseToRilj(response);

        return;
    }

    handleSetFeatureValue(true, message);
}

void RtmImsConfigController::handleSetFeatureValue(bool success, const sp<RfxMessage>& message) {
    int slot_id = getSlotId();
    int *params = (int *)message->getData()->getData();
    int featureId = params[0];
    int network = params[1];
    int value = params[2];
    int isLast = params[3];

    int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
    bool isAllowSendAT = false;
    int oldFeatureValue = 0;

    logD(RFX_LOG_TAG,
            "handleSetFeatureValue(), featureId:%d, network:%d, value:%d, isLast:%d, simState:%d",
            featureId, network, value, isLast, simState);

    if (simState == RFX_SIM_STATE_READY) {
        if (mFeatureSendCmd[featureId]) {
            switch(featureId) {
                case FEATURE_TYPE_VOICE_OVER_LTE:
                    oldFeatureValue = ImsConfigUtils::getFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VOLTE_ENALBE,
                            slot_id);
                    break;
                case FEATURE_TYPE_VIDEO_OVER_LTE:
                    oldFeatureValue = ImsConfigUtils::getFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VILTE_ENALBE,
                            slot_id);
                    break;
                case FEATURE_TYPE_VOICE_OVER_WIFI:
                    oldFeatureValue = ImsConfigUtils::getFeaturePropValue(
                            ImsConfigUtils::PROPERTY_WFC_ENALBE,
                            slot_id);
                    break;
                case FEATURE_TYPE_VIDEO_OVER_WIFI:
                    oldFeatureValue = ImsConfigUtils::getFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VIWIFI_ENALBE,
                            slot_id);
                    break;
                case FEATURE_TYPE_VOICE_OVER_NR:
                    oldFeatureValue = ImsConfigUtils::getFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VONR_ENALBE,
                            slot_id);
                    break;
                case FEATURE_TYPE_VIDEO_OVER_NR:
                    oldFeatureValue = ImsConfigUtils::getFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VINR_ENALBE,
                            slot_id);
                    break;
                case FEATURE_TYPE_VOICE_CROSS_SIM:
                    oldFeatureValue = ImsConfigUtils::getFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VODATA_ENALBE,
                            slot_id);
                    break;
                case FEATURE_TYPE_CALL_COMPOSER:
                    // TODO if need
                    break;
                default:
                    break;
            }

            logD(RFX_LOG_TAG,
                     "Already send feature %d once, compare feature value old: %d, new: %d",
                     featureId,
                     oldFeatureValue, value);

            if (value != oldFeatureValue || value != mFeatureSendValue[featureId]) {
                isAllowSendAT = true;
            }
        } else {
            isAllowSendAT = true;
            //mFeatureSendCmd[featureId] = true;
        }
    } else if (simState == RFX_SIM_STATE_ABSENT || simState == RFX_SIM_STATE_PIN_REQUIRED ||
            simState == RFX_SIM_STATE_PUK_REQUIRED || simState == RFX_SIM_STATE_NETWORK_LOCKED
            || simState == RFX_SIM_STATE_PERM_DISABLED) {
        logD(RFX_LOG_TAG, "ECCAllow : %d", mECCAllowSendCmd);
        if (mECCAllowSendCmd) {
            isAllowSendAT = ((value == FEATURE_VALUE_ON || isAllowImsOffIfSimAbsentConfigured()) &&
                    ((featureId == FEATURE_TYPE_VOICE_OVER_LTE) ||
                     (featureId == FEATURE_TYPE_VOICE_OVER_NR)));
            if (isAllowSendAT) {
                logI(RFX_LOG_TAG, "Allow to send enable VoLTE AT cmd once for ECC");
                mECCAllowSendCmd = false;
            }
        }
    }

    bool forceNotify = ((featureId == FEATURE_TYPE_VOICE_OVER_LTE ||
                         featureId == FEATURE_TYPE_VIDEO_OVER_LTE ||
                         featureId == FEATURE_TYPE_VOICE_OVER_NR ||
                         featureId == FEATURE_TYPE_VIDEO_OVER_NR) &&
                     ImsConfigUtils::isAllowForceNotify(getSlotId(), value));
    if (!isAllowSendAT) {
        isAllowSendAT = forceNotify;
    }

    logD(RFX_LOG_TAG,
         "Check if send AT cmd, feature = %d, value = %d, simState = %d, isAllowSendAT = %d",
         featureId, value, simState, isAllowSendAT);

    if (isAllowSendAT || (isLast == ISLAST_TRUE)) {
        if (isAllowSendAT) {
            char isEnable[10];
            if (sprintf(isEnable, "%d", value) < 0) {
                logE(RFX_LOG_TAG, "Check if send AT cmd, sprintf to isEnable fail");
            }

            switch(featureId) {
                case FEATURE_TYPE_VOICE_OVER_LTE:
                    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VOLTE_ENALBE,
                            isEnable,
                            slot_id);
                    break;
                case FEATURE_TYPE_VIDEO_OVER_LTE:
                    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VILTE_ENALBE,
                            isEnable,
                            slot_id);
                    break;
                case FEATURE_TYPE_VOICE_OVER_WIFI:
                    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_WFC_ENALBE,
                            isEnable,
                            slot_id);
                    break;
                case FEATURE_TYPE_VIDEO_OVER_WIFI:
                    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VIWIFI_ENALBE,
                            isEnable,
                            slot_id);
                    break;
                case FEATURE_TYPE_VOICE_OVER_NR:
                    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VONR_ENALBE,
                            isEnable,
                            slot_id);
                    break;
                case FEATURE_TYPE_VIDEO_OVER_NR:
                    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VINR_ENALBE,
                            isEnable,
                            slot_id);
                    break;
                case FEATURE_TYPE_VOICE_CROSS_SIM:
                    ImsConfigUtils::setFeaturePropValue(
                            ImsConfigUtils::PROPERTY_VODATA_ENALBE,
                            isEnable,
                            slot_id);
                    break;
                case FEATURE_TYPE_CALL_COMPOSER:
                    // TODO if need
                    break;
                default:
                    break;
            }
        }

        //mFeatureSendValue[featureId] = value;

        if (featureId == FEATURE_TYPE_VOICE_CROSS_SIM) {
            triggerVodataCfgCommand(slot_id);
        } else if (featureId == FEATURE_TYPE_CALL_COMPOSER) {
            triggerImsAddCapabilityCommand(value);
        } else if (isLast == ISLAST_NULL || (isLast == ISLAST_TRUE)) {
            triggerImsCfgCommand(slot_id);
        }
    }

    RIL_Errno error = success ? RIL_E_SUCCESS:RIL_E_GENERIC_FAILURE;
    sp<RfxMessage> response = RfxMessage::obtainResponse(error, message, true);
    responseToRilj(response);

    if (success) {
        bool allowNotify = false;
        int simState = getStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
        bool featureOn = false;
        if(mECCAllowNotify) {
            allowNotify = ((simState == RFX_SIM_STATE_ABSENT || simState ==
                    RFX_SIM_STATE_PIN_REQUIRED || simState == RFX_SIM_STATE_PUK_REQUIRED ||
                    simState == RFX_SIM_STATE_NETWORK_LOCKED || simState == RFX_SIM_STATE_PERM_DISABLED)
                    && ((featureId == FEATURE_TYPE_VOICE_OVER_LTE) ||
                     (featureId == FEATURE_TYPE_VOICE_OVER_NR)));
            featureOn = value == FEATURE_VALUE_ON;
            if(allowNotify && !featureOn) {
                mECCAllowNotify = false;
            }
        }

        bool simStateAllow = simState == RFX_SIM_STATE_READY;
        if(simStateAllow || (allowNotify && featureOn) || forceNotify) {
            logD(RFX_LOG_TAG,
                 "Notify feature changed, simStateAllow = %d allowNotify = %d featureOn = %d forceNotify = %d",
                 simStateAllow, allowNotify, featureOn, forceNotify);
            int data[2] = {featureId, value};
            sp <RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(),
                                                        RFX_MSG_UNSOL_IMS_CONFIG_FEATURE_CHANGED,
                                                        RfxIntsData(data, 2));
            responseToRilj(urc);
            mECCAllowNotify = false;
        }
    }

    if (((featureId == FEATURE_TYPE_VOICE_OVER_LTE) ||
         (featureId == FEATURE_TYPE_VOICE_OVER_NR)) &&
        ImsConfigUtils::isAllowForceNotify(getSlotId(), value)) {
        ImsConfigUtils::setAllowForceNotify(getSlotId(), false, 0);
    }
}

void RtmImsConfigController::getFeatureValue(const sp<RfxMessage>& message) {
    int *params = (int *)message->getData()->getData();
    int feature_id = params[0];
    int network = params[1];
    std::map<int, int>::iterator iter;
    RIL_Errno error = RIL_E_SUCCESS;

    logD(RFX_LOG_TAG, "getFeatureValue, feature_id:%d, network:%d", feature_id, network);

    int value = FEATURE_VALUE_OFF;

    if (feature_id == FEATURE_TYPE_VOICE_OVER_LTE) {
        iter = mVoLteFeatureValue.find(network);

        if (iter != mVoLteFeatureValue.end()) {
            value = mVoLteFeatureValue[network];
        }
    } else if (feature_id == FEATURE_TYPE_VIDEO_OVER_LTE) {
        iter = mViLteFeatureValue.find(network);

        if (iter != mViLteFeatureValue.end()) {
            value = mViLteFeatureValue[network];
        }
    } else if (feature_id == FEATURE_TYPE_VOICE_OVER_WIFI) {
        iter = mVoWifiFeatureValue.find(network);

        if (iter != mVoWifiFeatureValue.end()) {
            value = mVoWifiFeatureValue[network];
        }
    } else if (feature_id == FEATURE_TYPE_VIDEO_OVER_WIFI) {
        iter = mViWifiFeatureValue.find(network);

        if (iter != mViWifiFeatureValue.end()) {
            value = mViWifiFeatureValue[network];
        }
    } else if (feature_id == FEATURE_TYPE_VOICE_OVER_NR) {
        iter = mVoNrFeatureValue.find(network);

        if (iter != mVoNrFeatureValue.end()) {
            value = mVoNrFeatureValue[network];
        }
    } else if (feature_id == FEATURE_TYPE_VIDEO_OVER_NR) {
        iter = mViNrFeatureValue.find(network);

        if (iter != mViNrFeatureValue.end()) {
            value = mViNrFeatureValue[network];
        }
    } else if (feature_id == FEATURE_TYPE_VOICE_CROSS_SIM) {
        iter = mVoDataFeatureValue.find(network);

        if (iter != mVoDataFeatureValue.end()) {
            value = mVoDataFeatureValue[network];
        }
    } else if (feature_id == FEATURE_TYPE_CALL_COMPOSER) {
       iter = mCallComposerFeatureValue.find(network);

        if (iter != mCallComposerFeatureValue.end()) {
            value = mCallComposerFeatureValue[network];
        }
    } else {
        logI(RFX_LOG_TAG, "getFeatureValue, feature_id:%d, network:%d not support",
             feature_id, network);
        error = RIL_E_GENERIC_FAILURE;
    }

    sp<RfxMessage> response = RfxMessage::obtainResponse(message->getSlotId(),
                                                         message->getId(),
                                                         error,
                                                         RfxIntsData((void*)&value, sizeof(int)),
                                                         message);
    responseToRilj(response);
}

void RtmImsConfigController::setImsResourceCapability(int feature_id, int value,
                                                      bool isDynamicImsSwitchLast) {
    logD(RFX_LOG_TAG, "setImsResourceCapability, feature_id:%d,value:%d", feature_id, value);

    if (feature_id == FEATURE_TYPE_VOICE_OVER_LTE ||
        feature_id == FEATURE_TYPE_VIDEO_OVER_LTE ||
        feature_id == FEATURE_TYPE_VOICE_OVER_WIFI ||
        feature_id == FEATURE_TYPE_VIDEO_OVER_WIFI ||
        feature_id == FEATURE_TYPE_VOICE_OVER_NR ||
        feature_id == FEATURE_TYPE_VIDEO_OVER_NR) {
        mFeatureResource[feature_id] = value;
    } else {
        logI(RFX_LOG_TAG, "setImsResourceCapability, feature_id:%d, value:%d not support",
             feature_id, value);
    }

    if (isDynamicImsSwitchLast) {
        sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(),
                                                   RFX_MSG_UNSOL_IMS_CONFIG_DYNAMIC_IMS_SWITCH_COMPLETE,
                                                   RfxVoidData());
        responseToRilj(urc);
    }
}

void RtmImsConfigController::getImsResourceCapability(const sp<RfxMessage>& message) {
    int *params = (int *)message->getData()->getData();
    int feature_id = params[0];
    RIL_Errno error = RIL_E_SUCCESS;

    logD(RFX_LOG_TAG, "getImsResourceCapability, feature_id:%d",feature_id);

    if (feature_id == FEATURE_TYPE_VOICE_OVER_LTE ||
        feature_id == FEATURE_TYPE_VIDEO_OVER_LTE ||
        feature_id == FEATURE_TYPE_VOICE_OVER_WIFI ||
        feature_id == FEATURE_TYPE_VIDEO_OVER_WIFI ||
        feature_id == FEATURE_TYPE_VOICE_OVER_NR ||
        feature_id == FEATURE_TYPE_VIDEO_OVER_NR) {
        params[0] = mFeatureResource[feature_id];
    } else {
        logI(RFX_LOG_TAG, "getImsResourceCapability, feature_id:%d not support", feature_id);
        error = RIL_E_GENERIC_FAILURE;
        params[0] = FEATURE_RESOURCE_OFF;
    }

    sp<RfxMessage> response = RfxMessage::obtainResponse(error, message, true);
    responseToRilj(response);
}

bool RtmImsConfigController::ensureStorageInitStatus(const sp<RfxMessage> &message) {
    int msg_id = message->getId();
    if (!mInitDone) {
        logI(RFX_LOG_TAG, "storage not ready while - %s", RFX_ID_TO_STR(msg_id));
        switch (msg_id) {
            case RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION: {
                sp<RfxMessage> response = RfxMessage::obtainResponse(message->getSlotId(),
                                                                     message->getId(),
                                                                     RIL_E_GENERIC_FAILURE,
                                                                     RfxVoidData(),
                                                                     message);
                responseToRilj(response);
            }
                break;
            case RFX_MSG_REQUEST_IMS_CONFIG_GET_PROVISION: {
                char data[] = "n/a";
                sp<RfxMessage> response = RfxMessage::obtainResponse(message->getSlotId(),
                                                                     message->getId(),
                                                                     RIL_E_GENERIC_FAILURE,
                                                                     RfxStringData(data),
                                                                     message);
                responseToRilj(response);
            }
                break;
            default:
                break;
        }

        return false;
    } else {
        return true;
    }
}

void RtmImsConfigController::setVoNREnable(const sp<RfxMessage> &message) {
    RIL_Errno error = RIL_E_SUCCESS;
    int *params = (int*) message->getData()->getData();

    if (params != NULL) {
        int value = params[0];
        char isEnable[10];
        if (sprintf(isEnable, "%d", value) < 0) {
            logE(RFX_LOG_TAG, "setVoNREnable, sprintf to isEnable fail");
        }

        logD(RFX_LOG_TAG, "setVoNREnable(), params[0]: %d, value: %s",
                value, isEnable);

        ImsConfigUtils::setFeaturePropValue(ImsConfigUtils::PROPERTY_VONR_ENALBE,
                isEnable, getSlotId());

        char feature[] = "IMS over 3gpp";
        int featureVersion = getFeatureVersion(feature);
        if (featureVersion > 1) {
            triggerImsCfgCommand(getSlotId());
        }
    } else {
        logD(RFX_LOG_TAG, "setVoNREnable(), params is null");
        error = RIL_E_GENERIC_FAILURE;
    }

    sp<RfxMessage> response = RfxMessage::obtainResponse(message->getSlotId(),
                    message->getId(), error, RfxVoidData(), message);
    responseToRilj(response);
}

void RtmImsConfigController::isVoNREnabled(const sp<RfxMessage> &message) {
    RIL_Errno error = RIL_E_SUCCESS;

    int value = ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VONR_ENALBE,
            getSlotId());

    logD(RFX_LOG_TAG, "isVoNREnabled(), value: %d", value);

    sp<RfxMessage> response = RfxMessage::obtainResponse(message->getSlotId(),
                    message->getId(), error, RfxIntsData((void *) &value, sizeof(int)), message);
    responseToRilj(response);
}

void RtmImsConfigController::setProvisionValue(const sp<RfxMessage> &message) {
    if (!ensureStorageInitStatus(message)) {
        return;
    }

    char** params = (char**) message->getData()->getData();
    int config_id = std::atoi(params[0]);
    const char* value = (params[1] == NULL) ? "" : params[1];

    logD(RFX_LOG_TAG, "setProvisionValue(), config_id: %d, params: %s, %s, value: %s",
            config_id, params[0], params[1], value);

    mProvisionValue[config_id] = value;

    handleImsConfigExt(message);

    if (mProvisionedValue[config_id] != value) {
        mProvisionedValue[config_id] = value;
        saveProvisionedValue();
    }

    //special VoNR config IMS_CONFIG_VONR_SETTING_ENABLED
    if (config_id == IMS_CONFIG_VONR_SETTING_ENABLED) {
        char feature[] = "IMS over 3gpp";
        int featureVersion = getFeatureVersion(feature);
        if (featureVersion > 1) {
            triggerImsCfgCommand(getSlotId());
        }
    }

    handleSetProvisionValue(true, message);
}

bool RtmImsConfigController::handleImsConfigExt(const sp<RfxMessage> &message) {
    char** params = (char**) message->getData()->getData();
    int config_id = std::atoi(params[0]);
    const char* value = (params[1] == NULL) ? "" : params[1];
    int iValue;
    sp<RfxMessage> newMsg;

    logD(RFX_LOG_TAG, "handleImsConfigExt(), config_id: [%d], value: [%s]", config_id, value);

    switch (config_id) {
        case IMS_CONFIG_VOICE_OVER_WIFI_MODE: {
            iValue = (strcmp(value, "") == 0) ? -1 : std::atoi(value);
            if (iValue == -1) {
                return true;
            }

            logD(RFX_LOG_TAG, "RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL, value: [%d]", iValue);

            int rilValue = RIL_WIFI_PREFERRED;

            switch(iValue) {
                case AP_WIFI_ONLY:
                    rilValue = RIL_WIFI_ONLY;
                    break;
                case AP_CELLULAR_PREFERRED:
                    rilValue = RIL_CELLULAR_PREFERRED;
                    break;
                case AP_WIFI_PREFERRED:
                    rilValue = RIL_WIFI_PREFERRED;
                    break;
                case AP_4G_PREFER:
                    rilValue = RIL_4G_PREFER;
                    break;
                default:
                    logI(RFX_LOG_TAG, "setWfcMode mapping error, value is invalid!");
                    break;
            }

            newMsg = RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_SET_WFC_PROFILE_LOCAL,
                    RfxIntsData((void *) &rilValue, sizeof(int)));
            RfxMainThread::enqueueMessage(newMsg);
            return true;
        }
        default: {
            // Do nothing.
            return false;
        }
    }
}

void RtmImsConfigController::handleSetProvisionValue(bool success, const sp<RfxMessage> &message) {
    char **params = (char **) message->getData()->getData();
    int config_id = std::atoi(params[0]);
    const char* value = (params[1] == NULL) ? "" : params[1];

    bool isProvisionToModem = ImsConfigDataHelper::isProvisionToModem(config_id);
    if (isProvisionToModem) {
        // Store message for response to RILJ.
        mSetProvisionMessage = RfxMessage::obtainRequest(message->getSlotId(), message->getId(),
                message);

        //request to MCL
        string provisionStr = ImsConfigDataHelper::getConfigProvisionStr(config_id);

        logD(RFX_LOG_TAG, "set provision to MD, provisionStr: %s, value: %s", provisionStr.c_str(),
             value);

        char **data = (char **) calloc(2, sizeof(char *));

        if (data != NULL) {
            data[0] = strdup(provisionStr.c_str());
            data[1] = strdup(value);
        }

        if ((data != NULL) && (data[0] != NULL) && (data[1] != NULL)) {
            sp<RfxMessage> message = RfxMessage::obtainRequest(
                    getSlotId(),
                    RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION,
                    RfxStringsData(data, 2));
            requestToMcl(message);
            free(data[0]);
            free(data[1]);
            free(data);
        } else {
            logD(RFX_LOG_TAG, "handleSetProvisionValue(), calloc failed!");

            if (data != NULL) {
                if (data[0] != NULL) {
                    free(data[0]);
                }

                if (data[1] != NULL) {
                    free(data[1]);
                }

                free(data);
            }

            sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message,
                                                                 true);
            responseToRilj(response);
        }
    } else {
        RIL_Errno error = success ? RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;
        sp<RfxMessage> response = RfxMessage::obtainResponse(error, message,
                                                             true);
        responseToRilj(response);
    }

    if (success) {
        logD(RFX_LOG_TAG, "send config changed urc, data: %s,%s", params[0], params[1]);
        sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(),
                                                   RFX_MSG_UNSOL_IMS_CONFIG_CONFIG_CHANGED,
                                                   RfxStringsData(params, 2));
        responseToRilj(urc);
    }
    if(!isProvisionToModem) {
    }
}

void RtmImsConfigController::handleSetProvisionResponse(const sp <RfxMessage> &message) {
    logD(RFX_LOG_TAG, "handleSetProvisionResponse, result:%d", message->getError());
    sp<RfxMessage> response = RfxMessage::obtainResponse(mSetProvisionMessage->getSlotId(),
            mSetProvisionMessage->getId(), message->getError(), RfxVoidData(),
            mSetProvisionMessage);
    responseToRilj(response);
}

void RtmImsConfigController::getProvisionValue(const sp <RfxMessage> &message) {
    if (!ensureStorageInitStatus(message)) {
        return;
    }

    int slot_id = getSlotId();
    int *params = (int *) message->getData()->getData();
    int configId = params[0];

    logD(RFX_LOG_TAG, "getProvisionValue(), configId: %d", configId);

    if (ImsConfigDataHelper::isProvisionToModem(configId)) {
        // Store message for response when receive URC.
        mGetProvisionId = configId;
        mGetProvisionMessage = RfxMessage::obtainRequest(message->getSlotId(), message->getId(),
                message);

        //request to MCL
        string provisionStr = ImsConfigDataHelper::getConfigProvisionStr(configId);

        char* data = strdup(provisionStr.c_str());

        if (data != NULL) {
            logD(RFX_LOG_TAG, "get provision from MD, provisionStr: %s", data);
            sp<RfxMessage> message = RfxMessage::obtainRequest(
                    slot_id,
                    RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION,
                    RfxStringData(data));
            requestToMcl(message);
            free(data);
        } else {
            logE(RFX_LOG_TAG, "getProvisionValue(), data is NULL!");

            sp<RfxMessage> response = RfxMessage::obtainResponse(message->getSlotId(),
                    message->getId(), RIL_E_INVALID_ARGUMENTS, RfxVoidData(), message);

            responseToRilj(response);
        }
    } else {
        sp<RfxMessage> response;
        char* data = strdup(mProvisionValue[configId].c_str());

        if (data != NULL) {
            response = RfxMessage::obtainResponse(message->getSlotId(),
                    message->getId(), RIL_E_SUCCESS, RfxStringData(data), message);
        } else {
            logE(RFX_LOG_TAG, "getProvisionValue(), data is NULL!");

            response = RfxMessage::obtainResponse(message->getSlotId(),
                    message->getId(), RIL_E_INVALID_ARGUMENTS, RfxVoidData(), message);
        }

        responseToRilj(response);

        if (data != NULL) {
            free(data);
        }
    }
}

void RtmImsConfigController::handleGetProvisionResponse(const sp <RfxMessage> &message) {
    // RIL_E_OEM_ERROR_24 means no MD default value, need to get from AP
    if (message->getError() == RIL_E_OEM_ERROR_24) {
        sp<RfxMessage> response;

        char* data = strdup(mProvisionValue[mGetProvisionId].c_str());

        if (data != NULL) {
            response = RfxMessage::obtainResponse(mGetProvisionMessage->getSlotId(),
                    mGetProvisionMessage->getId(), RIL_E_SUCCESS, RfxStringData(data),
                    mGetProvisionMessage);
        } else {
            logE(RFX_LOG_TAG, "handleGetProvisionResponse(), data is NULL!");

            response = RfxMessage::obtainResponse(mGetProvisionMessage->getSlotId(),
                    mGetProvisionMessage->getId(), RIL_E_INVALID_ARGUMENTS, RfxVoidData(),
                    mGetProvisionMessage);
        }


        responseToRilj(response);

        if (data != NULL) {
            free(data);
        }
    } else if (message->getError() != RIL_E_SUCCESS) {
        sp<RfxMessage> response = RfxMessage::obtainResponse(mGetProvisionMessage->getSlotId(),
                mGetProvisionMessage->getId(), message->getError(), RfxVoidData(),
                mGetProvisionMessage);
        responseToRilj(response);
    } else {
        char *value = (char *) message->getData()->getData();
        int len = 0;
        if (value != NULL) {
            len = strlen(value);
        }
        logD(RFX_LOG_TAG, "handleGetProvisionResponse responsed, %s, %d", value, message->getData()->getDataLength());
        sp<RfxMessage> response = RfxMessage::obtainResponse(mGetProvisionMessage->getSlotId(),
                mGetProvisionMessage->getId(), message->getError(), RfxStringData(value, len),
                mGetProvisionMessage);
        responseToRilj(response);
    }
}

void RtmImsConfigController::handleGetProvisionUrc(const sp <RfxMessage> &message) {
    char **params = (char **) message->getData()->getData();
    char *config_item = params[0];
    char *value = params[1];

    logE(RFX_LOG_TAG, "handleGetProvisionUrc(), config_item:%s value: %s", config_item, value);
    // shall removed
    // RIL_E_OEM_ERROR_24 means no MD default value, need to get from AP
    if (message->getError() == RIL_E_OEM_ERROR_24) {
        RFX_ASSERT(config_item != NULL);
        int config_id = ImsConfigDataHelper::getConfigId(string(config_item));
        sp<RfxMessage> response;
        char* data = strdup(mProvisionValue[config_id].c_str());

        if (data != NULL) {
            response = RfxMessage::obtainResponse(mGetProvisionMessage->getSlotId(),
                    mGetProvisionMessage->getId(), RIL_E_SUCCESS, RfxStringData(data),
                    mGetProvisionMessage);
        } else {
            logE(RFX_LOG_TAG, "handleGetProvisionUrc(), data is NULL!");

            response = RfxMessage::obtainResponse(mGetProvisionMessage->getSlotId(),
                    mGetProvisionMessage->getId(), RIL_E_INVALID_ARGUMENTS, RfxVoidData(),
                    mGetProvisionMessage);
        }

        responseToRilj(response);

        if (data != NULL) {
            free(data);
        }
    } else {
        sp<RfxMessage> response = RfxMessage::obtainResponse(mGetProvisionMessage->getSlotId(),
                mGetProvisionMessage->getId(), message->getError(), RfxStringData(value),
                mGetProvisionMessage);
        responseToRilj(response);
    }
}

void RtmImsConfigController::triggerVodataCfgCommand(int slot_id) {
    if (!ImsConfigUtils::isPhoneIdSupportIms(slot_id)) {
        logI(RFX_LOG_TAG, "triggerVodataCfgCommand, Do not send AT+VODATA as not support ims");
        return;
    }

    int params[1];
    params[0] = RfxRilUtils::isWfcSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VODATA_ENALBE, slot_id) : 0; // vodata

    if (params[0] != 1) {
        params[0] = 0;
    }
    if (mFeatureSendCmd[FEATURE_TYPE_VOICE_CROSS_SIM] == true
            && mFeatureSendValue[FEATURE_TYPE_VOICE_CROSS_SIM] == params[0]) {
        logD(RFX_LOG_TAG, "triggerVodataCfgCommand, same state, don't send AT+VODATA");
        return;
    }
    mFeatureSendCmd[FEATURE_TYPE_VOICE_CROSS_SIM] = true;
    mFeatureSendValue[FEATURE_TYPE_VOICE_CROSS_SIM] = params[0];

    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_SET_VODATA_CFG, RfxIntsData(params, 1));

    logD(RFX_LOG_TAG, "triggerVodataCfgCommand(), AT+EVODATA=%d,...", params[0]);

    RfxMainThread::enqueueMessage(msg);
}

void RtmImsConfigController::triggerImsAddCapabilityCommand(int value) {
    int params[1];
    params[0] = value;

    if (params[0] != 1) {
        params[0] = 0;
    }
    if (mFeatureSendCmd[FEATURE_TYPE_CALL_COMPOSER] == true
            && mFeatureSendValue[FEATURE_TYPE_CALL_COMPOSER] == params[0]) {
        logD(RFX_LOG_TAG, "triggerImsAddCapabilityCommand, same state, don't send callcomposer");
        return;
    }
    mFeatureSendCmd[FEATURE_TYPE_CALL_COMPOSER] = true;
    mFeatureSendValue[FEATURE_TYPE_CALL_COMPOSER] = params[0];

    sp<RfxMessage> msg = RfxMessage::obtainRequest(getSlotId(),
            RFX_MSG_REQUEST_SET_IMS_ADD_CAP, RfxIntsData(params, 1));

    logD(RFX_LOG_TAG, "triggerImsAddCapabilityCommand(), callcomposer value:%d", params[0]);

    RfxMainThread::enqueueMessage(msg);
}

void RtmImsConfigController::triggerImsCfgCommand(int slot_id) {
    if (!ImsConfigUtils::isPhoneIdSupportIms(slot_id)) {
        logI(RFX_LOG_TAG, "triggerImsCfgCommand, Do not send AT+EIMSCFG as not support ims");
        return;
    }

    int simSwitchState =
        getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE);
    int currentRadioPower = getStatusManager()->getIntValue(
                RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
    if (simSwitchState == CAPABILITY_SWITCH_STATE_START && currentRadioPower == RADIO_STATE_OFF) {
        mSendCfgMccmnc = getCurrentMccMnc();
        mSendCfgIccid = getStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);
        logI(RFX_LOG_TAG, "triggerImsCfgCommand, Do not send AT+EIMSCFG when sim switching and radio off");
        return;
    }

    int params[6];
    int vonr = FEATURE_VALUE_OFF;
    int vinr = FEATURE_VALUE_OFF;
    int isAllowTurnOff = 1;
    char feature[] = "IMS over 3gpp";
    int featureVersion = getFeatureVersion(feature);
    int isDimsSupport = ImsConfigUtils::getSystemPropValue(
            ImsConfigUtils::PROPERTY_DYNAMIC_IMS_SWITCH_SUPPORT);
    //int isVonrSetSupport = ImsConfigUtils::getSystemPropValue(
    //        ImsConfigUtils::PROPERTY_VONR_SETTING_SUPPORT);

    isAllowTurnOff =
            getStatusManager()->getString8Value(RFX_STATUS_KEY_CARRIER_ALLOW_TURN_OFF_IMS) == "1";

        // Get latest feature value from sys props
    params[0] = RfxRilUtils::isVolteSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VOLTE_ENALBE, slot_id) : 0;  // volte
    params[1] = RfxRilUtils::isVilteSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VILTE_ENALBE, slot_id) : 0;  // vilte
    params[2] = RfxRilUtils::isWfcSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_WFC_ENALBE, slot_id) : 0;    // vowifi
    params[3] = RfxRilUtils::isViwifiSupport() ? ImsConfigUtils::getFeaturePropValue(
            ImsConfigUtils::PROPERTY_VIWIFI_ENALBE, slot_id) : 0; // viwifi
    params[4] = RfxRilUtils::isSmsSupport() ? ImsConfigUtils::getSystemPropValue(
            ImsConfigUtils::PROPERTY_IMS_SUPPORT) : 0;   // sms

    if (featureVersion > 1) {
        //if (isVonrSetSupport == 1) {
            vonr = RfxRilUtils::isVolteSupport() ? ImsConfigUtils::getFeaturePropValue(
                ImsConfigUtils::PROPERTY_VONR_ENALBE, slot_id) : 0;  // vonr
            vinr = RfxRilUtils::isVilteSupport() ? ImsConfigUtils::getFeaturePropValue(
                ImsConfigUtils::PROPERTY_VINR_ENALBE, slot_id) : 0;  // vinr
        //} else {
        //    vonr = params[0];
        //    vinr = params[1];
        //}
    }

    // For Android Only.
    // Android will only set "IMS Voice over 3gpp" and "IMS Video over 3gpp" to IMS Config.
    if (isDimsSupport == 1) {
        // IMS Config need to check DIMS first.
        // VoLTE and ViLTE
        if (params[0] == FEATURE_VALUE_ON) {
            params[0] = mFeatureResource[FEATURE_TYPE_VOICE_OVER_LTE];
        }

        if (params[1] == FEATURE_VALUE_ON) {
            params[1] = mFeatureResource[FEATURE_TYPE_VIDEO_OVER_LTE];
        }

        // VoNR and ViNR
        if (featureVersion > 1) {
            if (vonr == FEATURE_VALUE_ON) {
                vonr = mFeatureResource[FEATURE_TYPE_VOICE_OVER_NR];
            }

            if (vinr == FEATURE_VALUE_ON) {
                vinr = mFeatureResource[FEATURE_TYPE_VIDEO_OVER_NR];
            }
        }
    }

    //if VoLTE disabled, ViLTE/VoNR also need disabled
    params[1] = params[0] & params[1];

    mFeatureSendValue[FEATURE_TYPE_VOICE_OVER_LTE] = params[0];
    mFeatureSendValue[FEATURE_TYPE_VIDEO_OVER_LTE] = params[1];
    mFeatureSendValue[FEATURE_TYPE_VOICE_OVER_WIFI] = params[2];
    mFeatureSendValue[FEATURE_TYPE_VIDEO_OVER_WIFI] = params[3];
    mFeatureSendCmd[FEATURE_TYPE_VOICE_OVER_LTE] = true;
    mFeatureSendCmd[FEATURE_TYPE_VIDEO_OVER_LTE] = true;
    mFeatureSendCmd[FEATURE_TYPE_VOICE_OVER_WIFI] = true;
    mFeatureSendCmd[FEATURE_TYPE_VIDEO_OVER_WIFI] = true;
    mFeatureSendCmd[FEATURE_TYPE_VOICE_OVER_NR] = true;
    mFeatureSendCmd[FEATURE_TYPE_VIDEO_OVER_NR] = true;

    char isEnabled[10];
    char isDisabled[10];
    if (sprintf(isEnabled, "%d", 1) < 0) {
        logE(RFX_LOG_TAG, "Check if send AT cmd, sprintf to isEnable fail");
    }
    if (sprintf(isDisabled, "%d", 0) < 0) {
        logE(RFX_LOG_TAG, "Check if send AT cmd, sprintf to isEnable fail");
    }
    ImsConfigUtils::setFeaturePropValue(ImsConfigUtils::PROPERTY_VOLTE_ENALBE,
            params[0] == 1 ? isEnabled : isDisabled, slot_id);
    ImsConfigUtils::setFeaturePropValue(ImsConfigUtils::PROPERTY_VILTE_ENALBE,
            params[1] == 1 ? isEnabled : isDisabled, slot_id);

    if (featureVersion > 1) {
        //if VoLTE disabled, VoNR also need disabled
        vonr = vonr & params[0];
        //if VoLTE, ViLTE snd VoNR disabled, ViNR also need disabled
        vinr = vonr & vinr & params[1];

        ImsConfigUtils::setFeaturePropValue(ImsConfigUtils::PROPERTY_VONR_ENALBE,
                vonr == 1 ? isEnabled : isDisabled, slot_id);
        ImsConfigUtils::setFeaturePropValue(ImsConfigUtils::PROPERTY_VINR_ENALBE,
                vinr == 1 ? isEnabled : isDisabled, slot_id);

        //special VoNR config IMS_CONFIG_VONR_SETTING_ENABLED
        char* data = strdup(mProvisionValue[IMS_CONFIG_VONR_SETTING_ENABLED].c_str());
        if (data != NULL && strncmp(data, "0", 1) == 0) {
            vonr = 0;
            vinr = 0;
            logI(RFX_LOG_TAG, "triggerImsCfgCommand, disable VoNR/ViNR");
        }
        if (data != NULL) {
            free(data);
        }

        if (ImsConfigUtils::hasForceSetVoNRValue()) {
            vonr = ImsConfigUtils::getforceSetVoNRValue();
            if (vonr == FEATURE_VALUE_OFF) {
                vinr = FEATURE_VALUE_OFF;
            }
        }

        mFeatureSendValue[FEATURE_TYPE_VOICE_OVER_NR] = vonr;
        mFeatureSendValue[FEATURE_TYPE_VIDEO_OVER_NR] = vinr;
        params[0] = (params[0] | (vonr << 1));
        params[1] = (params[1] | (vinr << 1));
    }

    mSendCfgMccmnc = getCurrentMccMnc();
    mSendCfgIccid = getStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

    int viwifiIndependentVoWifi = std::atoi(getStatusManager()->getString8Value(
                                                    RFX_STATUS_KEY_CARRIER_VIWIFI_INDEPENDENT_VOWIFI));
    if (!viwifiIndependentVoWifi) {
        logD(RFX_LOG_TAG, "vowifi: %d, viwifi: %d, viwifiIndependentVoWifi: %d",
            params[2], params[3], viwifiIndependentVoWifi);
        //No add-on components, so add viwifi to follow the vowifi state.
        params[3] = params[2] & params[3];
    }

    int ignoreSmsForEims = std::atoi(getStatusManager()->getString8Value(
                                                    RFX_STATUS_KEY_CARRIER_IGNORE_SMS_FOR_EIMS));

    if (params[0] | params[1] | params[2] | params[3] | (params[4] && !ignoreSmsForEims) | (isAllowTurnOff^1)) {
        params[5] = 1;  // eims enable
    } else {
        params[5] = 0;  // eims disable
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
            if (params[2] == FEATURE_VALUE_ON) {
                params[2] = 3;
            }
            if (params[3] == FEATURE_VALUE_ON) {
                params[3] = 3;
            }
        }
    }

    logD(RFX_LOG_TAG,
         "triggerImsCfg, +EIMSCFG:%d,%d,%d,%d,%d,%d, imsVersion:%d, isAllowTurnOff:%d, mccmnc:%d",
         params[0], params[1], params[2], params[3], params[4], params[5],
         featureVersion, isAllowTurnOff, mSendCfgMccmnc);

    // Allow to send with AT+EIMSCFG
    sp<RfxMessage> message =  RfxMessage::obtainRequest(
                                            getSlotId(),
                                            RFX_MSG_REQUEST_SET_IMSCFG_EX,
                                            RfxIntsData(params, 6));
    // send to RtmImsController
    // TODO: for long term should handle RFX_MSG_REQUEST_SET_IMSCFG_EX here
    RfxMainThread::enqueueMessage(message);
}

int RtmImsConfigController::getCurrentMccMnc() {
    int mccmnc = 0;
    String8 defaultValue = String8("0");
    String8 mccmncStr = getStatusManager()->getString8Value(RFX_STATUS_KEY_UICC_GSM_NUMERIC,
                                                 defaultValue);

    if ((mccmncStr.string() == NULL) || (mccmncStr == "0") || (mccmncStr == "")) {
        mccmncStr = getStatusManager()->getString8Value(
                RFX_STATUS_KEY_UICC_CDMA_NUMERIC, defaultValue);
    }

    if (mccmncStr.string() == NULL) {
        mccmncStr = String8("0");
    }

    if (mccmncStr.string() != NULL) {
        mccmnc = atoi(mccmncStr.string());
    }
    return mccmnc;
}

void RtmImsConfigController::saveProvisionedValue() {
    int mccMnc = getCurrentMccMnc();
    int slot_id = getSlotId();
    std::map<int, std::string>::iterator iter;
    std::stringstream ss;   // For mimeType is INTEGER only
    int propCount = 1;

    if (mccMnc == 0) {
        logD(RFX_LOG_TAG, "saveProvisionedValue, mccMnc is 0");
        return;
    }

    resetProvisionedValue();

    ss.clear();
    ss.str("");

    for (iter = mProvisionedValue.begin(); iter != mProvisionedValue.end(); iter++) {
        int mimeType = ImsConfigDataHelper::getConfigDataType(iter->first);
        if (mimeType == INTEGER) {
            // If the length property value is over maximum, save previous property value first.
            if ((ss.str().length() + std::to_string(iter->first).length() + iter->second.length() +
                    2) >= RFX_PROPERTY_VALUE_MAX) {
                std::string propName = "persist.vendor.mtk.provision.int.";
                propName.append(std::to_string(slot_id)).append(std::to_string(propCount));
                std::string propValue = "";

                ss >> propValue;
                rfx_property_set(propName.c_str(), propValue.c_str());

                propCount++;
                ss.clear();
                ss.str("");
            }

            ss << iter->first,
            ss << ",";
            ss << iter->second;
            ss << ";";
        } else if (mimeType == STRING) {
            if ((std::to_string(iter->first).length() + iter->second.length() + 1) <
                    RFX_PROPERTY_VALUE_MAX) {
                std::string propName = "persist.vendor.mtk.provision.str.";
                propName.append(std::to_string(slot_id)).append(std::to_string(iter->first));

                rfx_property_set(propName.c_str(), iter->second.c_str());
            } else {
                logE(RFX_LOG_TAG, "saveProvisionedValue, configId: %d, value: %s too long",
                        iter->first, iter->second.c_str());
            }
        } else {
            logE(RFX_LOG_TAG, "saveProvisionedValue, not support configId = %d, mimeType = %d",
                    iter->first, mimeType);
        }
    }

    // For mimeType is INTEGER only
    if (ss.str().length() > 0) {
        std::string propName = "persist.vendor.mtk.provision.int.";
        propName.append(std::to_string(slot_id)).append(std::to_string(propCount));
        std::string propValue = "";

        ss >> propValue;

        rfx_property_set(propName.c_str(), propValue.c_str());
    }
}

void RtmImsConfigController::loadProvisionedValue() {
    int mccMnc = getCurrentMccMnc();
    int slot_id = getSlotId();
    bool isEmpty = false;
    std::stringstream ss;
    int propCount = 1;

    mProvisionedValue.clear();

    if (mccMnc == 0) {
        logD(RFX_LOG_TAG, "loadProvisionedValue, mccMnc is 0");
        return;
    }

    // Get previous stored mccmnc to check if load or reset.
    std::string propMccMncName = "persist.vendor.mtk.provision.mccmnc.";
    propMccMncName.append(std::to_string(slot_id));
    char propMccMncValue[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(propMccMncName.c_str(), propMccMncValue, "");

    logD(RFX_LOG_TAG, "loadProvisionedValue, curMccMnc: %d, preMccMnc: %d", mccMnc,
            atoi(propMccMncValue));

    if (strlen(propMccMncValue)) {
        if (mccMnc != atoi(propMccMncValue)) {
            // If current mccmnc is not equal to previous stored mccmnc,
            // No need to load the previous provisioned value.
            // Previous provisioned value will been reset when setProvisionValue.
            return;
        }
    } else {
        // If no stored mccmnc, it means no previous provisioned value.
        return;
    }

    ss.clear();
    ss.str("");

    // Load provisioned value and update mProvisionedValue (mimeType is integer)
    while (!isEmpty) {
        std::string propName = "persist.vendor.mtk.provision.int.";
        propName.append(std::to_string(slot_id)).append(std::to_string(propCount));
        char propValue[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get(propName.c_str(), propValue, "");

        if (strlen(propValue)) {
            ss << propValue;
            propCount++;
        } else {
            isEmpty = true;
        }
    }

    if (ss.str().length() > 0) {
        std::string provisionedId = "";
        std::string provisionedValue = "";

        while (std::getline(ss, provisionedId, ',')) {
            std::getline(ss, provisionedValue, ';');

            mProvisionedValue[atoi(provisionedId.c_str())] = provisionedValue;
            mProvisionValue[atoi(provisionedId.c_str())] = provisionedValue;

            provisionedId = "";
            provisionedValue = "";
        }
    }

    // Load provisioned value and update mProvisionedValue (mimeType is string)
    for (int i = IMS_CONFIG_PROVISIONED_START; i <= IMS_CONFIG_MTK_PROVISIONED_END; i++) {
        int mimeType = ImsConfigDataHelper::getConfigDataType(i);
        if (mimeType == STRING) {
            std::string propName = "persist.vendor.mtk.provision.str.";
            propName.append(std::to_string(slot_id)).append(std::to_string(i));
            char propValue[RFX_PROPERTY_VALUE_MAX] = { 0 };

            rfx_property_get(propName.c_str(), propValue, "");

            logD(RFX_LOG_TAG, "loadProvisionedValue, propName = %s, propValue=%s",
                    propName.c_str(), propValue);

            if (strlen(propValue)) {
                mProvisionedValue[i] = propValue;
                mProvisionValue[i] = propValue;
            }
        }

        if (i == IMS_CONFIG_PROVISIONED_END) {
            i = (IMS_CONFIG_MTK_PROVISIONED_START - 1);
        }
    }
}

void RtmImsConfigController::resetProvisionedValue() {
    int mccMnc = getCurrentMccMnc();
    int slot_id = getSlotId();
    int propCount = 1;

    std::string propMccMncName = "persist.vendor.mtk.provision.mccmnc.";
    propMccMncName.append(std::to_string(slot_id));
    std::string propMccMncValue = std::to_string(mccMnc);
    rfx_property_set(propMccMncName.c_str(), propMccMncValue.c_str());

    std::string propIntName = "persist.vendor.mtk.provision.int.";
    propIntName.append(std::to_string(slot_id)).append(std::to_string(propCount));
    char propPreviousIntValue[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(propIntName.c_str(), propPreviousIntValue, "");

    if (strlen(propPreviousIntValue) > 0) {
        std::string propIntValue = "";
        rfx_property_set(propIntName.c_str(), propIntValue.c_str());
    }

    for (int i = IMS_CONFIG_PROVISIONED_START; i <= IMS_CONFIG_MTK_PROVISIONED_END; i++) {
        int mimeType = ImsConfigDataHelper::getConfigDataType(i);
        if (mimeType == STRING) {
            std::string propStrName = "persist.vendor.mtk.provision.str.";
            propStrName.append(std::to_string(slot_id)).append(std::to_string(i));
            char propPreviousStrValue[RFX_PROPERTY_VALUE_MAX] = { 0 };
            rfx_property_get(propStrName.c_str(), propPreviousStrValue, "");

            if (strlen(propPreviousStrValue) > 0) {
                std::string propStrValue = "";
                rfx_property_set(propStrName.c_str(), propStrValue.c_str());
            }
        }

        if (i == IMS_CONFIG_PROVISIONED_END) {
            i = (IMS_CONFIG_MTK_PROVISIONED_START - 1);
        }
    }
}

