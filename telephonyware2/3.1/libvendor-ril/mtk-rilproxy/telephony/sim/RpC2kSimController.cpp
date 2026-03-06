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
#include "RpSimControllerBase.h"
#include "RpC2kSimController.h"
#include "RfxStatusDefs.h"
#include <mtk_log.h>
#include <prop/properties.h>
#include <cutils/jstring.h>


/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RpC2kSimController", RpC2kSimController, RpSimControllerBase);

RpC2kSimController::RpC2kSimController() {
}

RpC2kSimController::~RpC2kSimController() {
}

void RpC2kSimController::onInit() {
    RpSimControllerBase::onInit(); // Required: invoke super class implementation

    const int request_id_list[] = {
        //RIL_REQUEST_WRITE_PB_TO_RUIM  // FIXME Just for example
    };

    const int urc_id_list[] = {
        RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED
    };

    registerToHandleRequest(request_id_list, (sizeof(request_id_list)/sizeof(int)));
    registerToHandleUrc(urc_id_list, (sizeof(urc_id_list)/sizeof(int)));

    // Register RFX_STATUS_KEY_CDMA_SOCKET_SLOT.
    getNonSlotScopeStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CDMA_SOCKET_SLOT,
            RfxStatusChangeCallback(this, &RpC2kSimController::onCdmaSocketSlotChanged));

    RLOGD("[RpC2kSimController] onInit (slot %d)", getSlotId());
}

void RpC2kSimController::onDeinit() {
    // Unregister RFX_STATUS_KEY_CDMA_SOCKET_SLOT.
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CDMA_SOCKET_SLOT,
            RfxStatusChangeCallback(this, &RpC2kSimController::onCdmaSocketSlotChanged));
    RfxController::onDeinit();

    RLOGD("[RpC2kSimController] onDeinit (slot %d)", getSlotId());
}

bool RpC2kSimController::onHandleRequest(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handle req %s (slot %d)", requestToString(message->getId()),
            getSlotId());
    return true;
}

bool RpC2kSimController::onHandleUrc(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handle urc %s (slot %d)", urcToString(message->getId()),
            getSlotId());
    switch (message->getId()) {
        case RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED:
            handleUiccSubscriptionStatusChanged(message);
            break;
        default:
            return false;
    }
    return true;
}

bool RpC2kSimController::onHandleResponse(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handle %s response (slot %d)", requestToString(message->getId()),
            getSlotId());
    return true;
}

void RpC2kSimController::onCdmaSocketSlotChanged(RfxStatusKeyEnum key, RfxVariant oldValue,
        RfxVariant newValue) {
    int newcdmaslot = newValue.asInt();
    int oldcdmaslot = oldValue.asInt();
    RLOGD("[RpC2kSimController] onCdmaSocketSlotChanged newcdmaslot: %d oldcdmaslot: %d (slot %d)",
            newcdmaslot, oldcdmaslot, getSlotId());

    // Reset RpC2kSimController if cdma socket slot is changed.
    if (newcdmaslot != oldcdmaslot) {
        getStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_NOT_READY);
        getStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_IMSI, String8(""));
        resetCdmaCardStatus();
    }
}

void RpC2kSimController::handleCdmaCardTypeNotify(const sp<RfxMessage>& message) {
    int cardtype;
    int count;
    Parcel *p = message->getParcel();
    count = p->readInt32();
    cardtype = p->readInt32();
    RLOGD("[RpC2kSimController] handleCdmaCardTypeNotify (slot %d) count: %d, cardtype: %d",
            getSlotId(), count, cardtype);
    if (count == 1) {
        getStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE, cardtype);
        // Update card type for cdma only card's missing/recovery.
        if (isCdmaOnlyCard()) {
            if ((cardtype == CARD_NOT_INSERTED)) {
                getStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, 0);
            } else if ((cardtype == UIM_CARD) || (cardtype == UIM_SIM_CARD) ||
                    (cardtype == CT_3G_UIM_CARD) || (cardtype == CT_UIM_SIM_CARD) ||
                    (cardtype == NEED_TO_INPUT_PIN)) {
                setCardTypeByFullUiccTypeProperty();
                setCt3gDualmodeValueByCt3gProperty();
            }
        }
    } else {
        RLOGE("[RpC2kSimController] handleCdmaCardTypeNotify payload format is wrong! (slot %d)",
                getSlotId());
    }
}

bool RpC2kSimController::isCdmaOnlyCard() {
    int slotId = getSlotId();
    const char *fullUiccType = NULL;
    char tmp[PROPERTY_VALUE_MAX] = {0};
    if (slotId >= 0 && slotId <= 4) {
        fullUiccType = PROPERTY_RIL_FULL_UICC_TYPE[slotId];
        property_get(fullUiccType, tmp, "");
        RLOGD("[RpC2kSimController] isCdmaOnlyCard, fullcardtype is %s ", tmp);
    } else {
        RLOGE("[RpC2kSimController] isCdmaOnlyCard, slotId %d is wrong!", slotId);
    }

    if ((strncmp(tmp, "RUIM", 4) == 0) || (strncmp(tmp, "CSIM", 4) == 0) ||
            (strncmp(tmp, "RUIM,CSIM", 9) == 0)) {
        return true;
    } else {
        return false;
    }
}

void RpC2kSimController::setCardTypeByFullUiccTypeProperty() {
    int slotId = getSlotId();
    const char *fullUiccType = NULL;
    int cardType = 0;
    char tmp[PROPERTY_VALUE_MAX] = {0};
    do {
        if (slotId >= 0 && slotId <= 4) {
            fullUiccType = PROPERTY_RIL_FULL_UICC_TYPE[slotId];
            property_get(fullUiccType, tmp, "");
        } else {
            RLOGE("[RpC2kSimController] setCardTypeByFullUiccTypeProperty, slotId %d is wrong!",
                    slotId);
            break;
        }
        // If there is "SIM", it always must be put in the first!
        if (strncmp(tmp, "SIM", 3) == 0) {
            cardType |= RFX_CARD_TYPE_SIM;
        } else if (strncmp(tmp, "N/A", 3) == 0) {
            cardType = 0;
        } else if (strlen(tmp) == 0) {
            cardType = -1;
        }

        if (strstr(tmp, "USIM") != NULL)  {
            cardType |= RFX_CARD_TYPE_USIM;
        }

        if (strstr(tmp, "CSIM") != NULL)  {
            cardType |= RFX_CARD_TYPE_CSIM;
        }

        if (strstr(tmp, "RUIM") != NULL)  {
            cardType |= RFX_CARD_TYPE_RUIM;
        }

        RLOGD("[RpC2kSimController] setCardTypeByFullUiccTypeProperty uicctype (%s, %d) (slot %d)",
                fullUiccType, cardType, getSlotId());

        getStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, cardType);
    } while (0);
}

void RpC2kSimController::setCt3gDualmodeValue(const sp<RfxMessage>& message) {
    int mode;
    int count;
    Parcel *p = message->getParcel();
    count = p->readInt32();
    mode = p->readInt32();
    RLOGD("[RpC2kSimController] setCt3gDualmodeValue (slot %d) count: %d, mode: %d",
            getSlotId(), count, mode);
    if (count == 1) {
        if (mode == 1) {
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_CT3G_DUALMODE_CARD, true);
        } else {
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_CT3G_DUALMODE_CARD, false);
        }
    } else {
        RLOGE("[RpC2kSimController] setCt3gDualmodeValue but payload format is wrong! (slot %d)",
                getSlotId());
    }
}

void RpC2kSimController::setCt3gDualmodeValueByCt3gProperty() {
    int slotId = getSlotId();
    const char *ct3gproperty = NULL;
    int ct3g = 0;
    char tmp[PROPERTY_VALUE_MAX] = {0};
    do {
        if (slotId >= 0 && slotId <= 4) {
            ct3gproperty = PROPERTY_RIL_CT3G[slotId];
            property_get(ct3gproperty, tmp, "");
        } else {
            RLOGE("[RpC2kSimController] setCt3gDualmodeValueByCt3gProperty, slotId %d is wrong!",
                    slotId);
            break;
        }
        if (strncmp(tmp, "1", 1) == 0) {
            ct3g = 1;
        } else {
            ct3g = 0;
        }
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_CT3G_DUALMODE_CARD, ct3g);
        RLOGD("[RpC2kSimController] setCt3gDualmodeValueByCt3gProperty (%s, %d) (slot %d)",
                ct3gproperty, ct3g, getSlotId());
    } while (0);
}

void RpC2kSimController::resetCdmaCardStatus() {
    RLOGD("[RpC2kSimController] handle resetCdmaCardStatus (slot %d)", getSlotId());
    getStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE, -1);
    getStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_CARD_READY, false);
    getStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_FILE_READY, false);
    property_set(PROPERTY_RIL_CDMA_CARD_TYPE[getSlotId()], "");
    getStatusManager()->setString8Value(RFX_STATUS_KEY_C2K_IMSI, String8(""));
    property_set(PROPERTY_UIM_SUBSCRIBER_ID[getSlotId()], "");
}

sp<RfxMessage> RpC2kSimController::responseCdmaAbsentState(const sp<RfxMessage>& message) {
    sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_E_SUCCESS,
            message);
    if(response == NULL) {
        RLOGE("[RpC2kSimController] responseCdmaAbsentState obtainResponse fail!!!(slot %d)", getSlotId());
        return NULL;
    }
    Parcel* p = response->getParcel();
    p->writeInt32(RIL_CARDSTATE_ABSENT);
    p->writeInt32(RIL_PINSTATE_UNKNOWN);
    p->writeInt32(RIL_CARD_MAX_APPS);
    p->writeInt32(RIL_CARD_MAX_APPS);
    p->writeInt32(RIL_CARD_MAX_APPS);
    p->writeInt32(0);
    // For RUIM single mode card, iccid is not set in rild. If C capability is not in this
    // card for C+C mode, there is no chance to update iccid in rild. Here is to update
    // iccid to N/A in such situation.
    property_set(PROPERTY_ICCID_SIM[getSlotId()], "N/A");

    return response;
}

bool RpC2kSimController::onPreviewCheckRequestGetSimStatus() {
    RLOGD("[RpC2kSimController] handle onPreviewCheckRequestGetSimStatus (slot %d)", getSlotId());
#if  (defined(TELEPHONYWARE) || defined(TELE_FWK))
     return false;
#else
    if (supportCardType(ICC_CSIM) || supportCardType(ICC_RUIM)) {
        if (getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_CDMALTE_MODE_SLOT_READY)
                != CDMALTE_MODE_READY) {
            return true;
        } else if (getSlotId() == getNonSlotScopeStatusManager()->getIntValue(
                RFX_STATUS_KEY_ACTIVE_CDMALTE_MODE_SLOT)) {
            RLOGD("[RpC2kSimController] check result : %d",
                    !getStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA_CARD_READY, false));
            return (!getStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA_CARD_READY, false));
        } else {
            return false;
        }
    } else {
        return false;
    }
#endif
}

bool RpC2kSimController::onPreviewCheckRequestGetImsi(
        const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handle onPreviewCheckRequestGetImsi (slot %d)", getSlotId());
    Parcel *req = message->getParcel();
    size_t dataPos = 0;
    int count = 0;
#if  (defined(TELEPHONYWARE) || defined(TELE_FWK))
    return false;
#else
    dataPos = req->dataPosition();
    count = req->readInt32();
    if (count == 1) {
        // There is only one parameter "AID" in the request
        char *aid_ptr = strdupReadString(req);
        RLOGD("[RpC2kSimController] onPreviewCheckRequestGetImsi aid %s (slot %d)",
                aid_ptr, getSlotId());
        req->setDataPosition(dataPos);

        RILD_RadioTechnology_Group dest = choiceDestViaAid(aid_ptr);
        if (aid_ptr != NULL) {
            free(aid_ptr);
        }
        if (dest == RADIO_TECH_GROUP_C2K &&
                (!getStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA_FILE_READY, false))) {
            RLOGD("[RpC2kSimController] onPreviewCheckRequestGetImsi C2K file is ready (slot %d)",
                    getSlotId());
            return true;
        } else {
            RLOGE("[RpC2kSimController] onPreviewCheckRequestGetImsi out of scope");
            return false;
        }
    } else {
        RLOGE("[RpC2kSimController] onPreviewCheckRequestGetImsi format is wrong! (slot %d)",
                getSlotId());
        return false;
    }
#endif
}

void RpC2kSimController::handlePinPukReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handlePinPukReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handlePinPukRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handlePinPukRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleQuerySimFacilityReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleQuerySimFacilityReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleQuerySimFacilityRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleQuerySimFacilityRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleSetSimFacilityReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSetSimFacilityReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleSetSimFacilityRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSetSimFacilityRsp (slot %d)", getSlotId());

    sendRsp(message);
}


void RpC2kSimController::handleIccOpenChannelReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleIccOpenChannelReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleIccOpenChannelRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleIccOpenChannelRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleIccCloseChannelReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleIccCloseChannelReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleIccCloseChannelRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleIccCloseChannelRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleTransmitApduReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleTransmitApduReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleTransmitApduRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleTransmitApduRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleGetAtrReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleGetAtrReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleGetAtrRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleGetAtrRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleSetUiccSubscriptionReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSetUiccSubscriptionReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleSetUiccSubscriptionRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSetUiccSubscriptionRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleSimIoReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSimIoReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleSimIoRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSimIoRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleSimAuthenticationReq(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSimAuthenticationReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleSimAuthenticationRsp(const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSimAuthenticationRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::handleSimTransmitApduBasicReq(
        const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSimTransmitApduBasicReq (slot %d)", getSlotId());

    sendReq(message);
}

void RpC2kSimController::handleSimTransmitApduBasicRsp(
        const sp<RfxMessage>& message) {
    RLOGD("[RpC2kSimController] handleSimTransmitApduBasicRsp (slot %d)", getSlotId());

    sendRsp(message);
}

void RpC2kSimController::sendReq(const sp<RfxMessage>& message) {
    // Pass the request to RILD directly
    //size_t dataPos = 0;
    //Parcel* req = message->getParcel();
    sp<RfxMessage> request = RfxMessage::obtainRequest(RADIO_TECH_GROUP_C2K,
        message->getId(), message, true);

    // Clone Parcel
    //dataPos = req->dataPosition();
    //request->getParcel()->appendFrom(req, dataPos, (req->dataSize() - dataPos));
    //request->getParcel()->setDataPosition(0);

    // Send to GSM Rild
    requestToRild(request);
}

void RpC2kSimController::sendRsp(const sp<RfxMessage>& message) {
    Parcel *rsq = message->getParcel();
    size_t dataPos = 0;
    Parcel *p = NULL;

    sp<RfxMessage> response = RfxMessage::obtainResponse(message->getError(),
                message,true);

    // Clone parcel
    //dataPos = rsq->dataPosition();
    //p = response->getParcel();

    //p->appendFrom(rsq, dataPos, (rsq->dataSize() - dataPos));
    //p->setDataPosition(0);

    // Send to RILJ directly
    responseToRilj(response);
}

void RpC2kSimController::handleUiccSubscriptionStatusChanged(
        const sp<RfxMessage>& message) {
    Parcel *p = message->getParcel();
    int pos = p->dataPosition();
    int32_t numInts = p->readInt32();
    int32_t activate = p->readInt32();
    RLOGD("[RpC2kSimController] handleUiccSubscriptionStatusChanged, activate %d (slot %d)",
            activate, getSlotId());
    getStatusManager()->setIntValue(RFX_STATUS_KEY_UICC_SUB_CHANGED_STATUS, activate, true);
    p->setDataPosition(pos);
    responseToRilj(message);
}

