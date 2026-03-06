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
// system include
#include <string>
#include <vector>

// MTK fusion include
#include "RfxRootController.h"
#include "RfxVoidData.h"

// MTK CC local include
#include "RmmCallControlBaseHandler.h"

#include "MtkPhoneNumberUtils.h"

#define RFX_LOG_TAG "RmmCCBaseHandler"

#define OPERATOR_VZW "OP12"

RmmCallControlBaseHandler::RmmCallControlBaseHandler(int slot_id,
        int channel_id) : RfxBaseHandler(slot_id, channel_id) {
    // do nothing
}

RmmCallControlBaseHandler::~RmmCallControlBaseHandler() {
    // do nothing
}

char* RmmCallControlBaseHandler::getClirPrefix(int clir) {
    char* clirPrefix = (char*)"";   /*subscription default*/
    if (clir == 1) {  /*invocation*/
        clirPrefix = (char*)"#31#";
    } else if (clir == 2) {  /*suppression*/
        clirPrefix = (char*)"*31#";
    }
    return clirPrefix;
}

void RmmCallControlBaseHandler::responseVoidDataToTcl(const sp<RfxMclMessage>& msg) {
    RfxMipcData* data = msg->getMipcData();
    RIL_Errno err = (data != NULL && data->getResult() == MIPC_RESULT_SUCCESS) ?
            RIL_E_SUCCESS : RIL_E_INTERNAL_ERR;
    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

void RmmCallControlBaseHandler::responseVoidDataToTcl(const sp<RfxMclMessage>& msg, RIL_Errno err) {
    //if (err != RIL_E_SUCCESS) {
    //    logE(RFX_LOG_TAG, "%d failed!", msg->getId());
    //}
    sp<RfxMclMessage> mclResponse =
        RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

int RmmCallControlBaseHandler::getDomain(bool isAuto, bool isEcc) {
    int prefRat = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_ECC_PREFERRED_RAT, 0);

    int mode = 0;
    if (isAuto) {
        mode = CALL_DOMAIN_AUTO;
    } else {
        mode = CALL_DOMAIN_CS_AND_1X;
    }
    if (isEcc && !isVoiceGsmRat() && !isCdmaRat()) {
        if (isAuto) {
            if (prefRat == ECC_PREFER_RAT_GSM_ONLY) {
                mode = CALL_DOMAIN_3GPP_ONLY;
            } else if (prefRat == ECC_PREFER_RAT_CDMA_ONLY) {
                mode = CALL_DOMAIN_IMS_AND_1X;
            }
        } else {
            if (prefRat == ECC_PREFER_RAT_GSM_ONLY) {
                mode = CALL_DOMAIN_CS_ONLY;
            } else if (prefRat == ECC_PREFER_RAT_CDMA_ONLY) {
                mode = CALL_DOMAIN_3GPP2_ONLY;
            }
        }
    }
    return mode;
}

int RmmCallControlBaseHandler::getEccPreferredDomain() {
    int prefRat = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_ECC_PREFERRED_RAT, 0);

    int prefMode = ECC_PREFERRED_DOMAIN_AUTO;
    if (isVoiceGsmRat() && prefRat == ECC_PREFER_RAT_GSM_PREF) {
        prefMode = ECC_PREFERRED_DOMAIN_3GPP;
    } else if (!isVoiceGsmRat() && !isCdmaRat()) {
        if (prefRat == ECC_PREFER_RAT_GSM_PREF) {
            prefMode = ECC_PREFERRED_DOMAIN_3GPP;
        } else if (prefRat == ECC_PREFER_RAT_CDMA_PREF) {
            prefMode = ECC_PREFERRED_DOMAIN_3GPP2;
        }
    }

    return prefMode;
}

bool RmmCallControlBaseHandler::hasImsCall(int slotId) {
    RfxRootController* rootController = RFX_OBJ_GET_INSTANCE(RfxRootController);
    RfxStatusManager* slotStatusMgr  = rootController->getStatusManager(slotId);
    return slotStatusMgr->getBoolValue(RFX_STATUS_KEY_IMS_CALL_EXIST, false);
}

char* RmmCallControlBaseHandler::extractSipUri(char *num) {
    // sip:0123456;234,4@xxx.com
    char *result;
    result = strsep(&num, ":");
    if (NULL != num) {
        result = num;
    }
    result = strsep(&result, "@");
    result = strsep(&result, ";");
    result = strsep(&result, ",");
    return result;
}

/// C2K specific start
bool RmmCallControlBaseHandler::isGsmRat() {
    bool bGsmRat;
    //RAT can be unknown, neither Gsm or Cdma
    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState ss = getMclStatusManager()
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    int voiceRadioTech = ss.getRilVoiceRadioTech();
    bGsmRat = RfxNwServiceState::isGsmGroup(voiceRadioTech);
    if (!bGsmRat) {
        logD(RFX_LOG_TAG, "%s, voiceRadioTech:%d", __FUNCTION__, voiceRadioTech);
    }
    return bGsmRat;
}

// Check if this slot is in CDMA RAT
bool RmmCallControlBaseHandler::isCdmaRat() {
    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState ss = getMclStatusManager()
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    int voiceRadioTech = ss.getRilVoiceRadioTech();
    logD(RFX_LOG_TAG, "%s, voiceRadioTech:%d", __FUNCTION__, voiceRadioTech);
    return RfxNwServiceState::isCdmaGroup(voiceRadioTech);
}

bool RmmCallControlBaseHandler::isVoiceGsmRat() {
    // If CS state of CDMA card is NR, it only means "SMS over NAS". Ignore it.
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, 0);
    RfxNwServiceState defaultServiceState (0, 0, 0, 0);
    RfxNwServiceState ss = getMclStatusManager()
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    if (RFX_FLAG_HAS(cardType, (RFX_CARD_TYPE_RUIM | RFX_CARD_TYPE_CSIM))
            && ss.getRilVoiceRegState() != RIL_REG_ROAMING
            && ss.getRilVoiceRadioTech() == RADIO_TECH_NR) {
        logD(RFX_LOG_TAG, "%s, is CDMA card, cardType:%d, ss:%s, return false",
                __FUNCTION__, cardType, ss.toString().string());
        return false;
    }

    return isGsmRat();
}
/// C2K specific end

String8 RmmCallControlBaseHandler::handleNumberWithPause(char* number) {
    if (number == NULL) {
        return String8::format("");
    }
    char *formatNumber = NULL;
    char *origNumber = NULL;
    int len = strlen(number);
    int pos = 0;
    char* tmp = strchr(number, ',');
    if (tmp != NULL) {
        pos = (tmp - number) / sizeof(char);
        formatNumber = (char *)alloca(pos + 1);
        origNumber = (char *)alloca(len - pos);
        memset(formatNumber, 0, pos + 1);
        memset(origNumber, 0, len - pos);
        strncpy(formatNumber, number, pos);
        strncpy(origNumber, tmp + 1, len - pos - 1);
        if (RfxRilUtils::isUserLoad() != 1) {
            logD(RFX_LOG_TAG, "%s, number:%s, formatNum:%s, origNum:%s", __FUNCTION__,
                    RfxRilUtils::pii(RFX_LOG_TAG, number), RfxRilUtils::pii(RFX_LOG_TAG, formatNumber),
                    RfxRilUtils::pii(RFX_LOG_TAG, origNumber));
        }
        return String8::format("%s", origNumber);
    } else {
        return String8::format("%s", number);
    }
}

// Check if Clir should be modify
int RmmCallControlBaseHandler::handleClirSpecial(bool isEcc, int clir, char* number) {
    String8 priorityString = getMclStatusManager()
            ->getString8Value(RFX_STATUS_KEY_HIGH_PRIORITY_CLIR_PREFIX_SUPPORTED);
    if ((strcmp(priorityString.string(), "true") == 0 && clir == 1 /*invocation*/ &&
            strncmp(number, "*82", 3) == 0) || isEcc) {
        clir = 0; // use subscription default value
    }
    return clir;
}

int RmmCallControlBaseHandler::handleNumberWithClirString(String8& number, int clir) {
    RfxNwServiceState defaultServiceState (0, 0, 0 ,0);
    RfxNwServiceState ss = getMclStatusManager()
            ->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE, defaultServiceState);
    if (isVzWProject() && (ss.getRilVoiceRegState() == 5)) {
        if (number.find("*67") == 0) {
            number.setTo(number.string() + 3);
            return CLIR_INVOCATION;
        } else if (number.find("*82") == 0) {
            number.setTo(number.string() + 3);
            return CLIR_SUPPRESSTION;
        }
    }
    return clir;
}

bool RmmCallControlBaseHandler::isVzWProject() {
    char optr_value[RFX_PROPERTY_VALUE_MAX] = { 0 };

    rfx_property_get("persist.vendor.operator.optr", optr_value, "0");

    return (strncmp(optr_value, OPERATOR_VZW, strlen(OPERATOR_VZW)) == 0);
}

int RmmCallControlBaseHandler::mipcNumPresentationToRilType(int presentation) {
    switch (presentation) {
    case MIPC_CALL_CLI_VALIDITY_CLI_VALID:
        return PRESENTATION_ALLOWED;
    case MIPC_CALL_CLI_VALIDITY_CLI_WITHHELD:
        return PRESENTATION_RESTRICTED;
    case MIPC_CALL_CLI_VALIDITY_CLI_INTERWORK:
    case MIPC_CALL_CLI_VALIDITY_CLI_PAYPHONE:
    case MIPC_CALL_CLI_VALIDITY_CLI_OTHERS:
        return PRESENTATION_UNKNOWN;
    default:
        return PRESENTATION_ALLOWED;
    }
}

int RmmCallControlBaseHandler::mipcNamePresentationToRilType(int presentation) {
    switch (presentation) {
    case MIPC_CALL_CNI_VALIDITY_CNI_VALID:
        return PRESENTATION_ALLOWED;
    case MIPC_CALL_CNI_VALIDITY_CNI_WITHHELD:
        return PRESENTATION_RESTRICTED;
    case MIPC_CALL_CNI_VALIDITY_CNI_INTERWORK:
    case MIPC_CALL_CNI_VALIDITY_CNI_PAYPHONE:
    case MIPC_CALL_CNI_VALIDITY_CNI_OTHERS:
        return PRESENTATION_UNKNOWN;
    default:
        return PRESENTATION_ALLOWED;
    }
}
