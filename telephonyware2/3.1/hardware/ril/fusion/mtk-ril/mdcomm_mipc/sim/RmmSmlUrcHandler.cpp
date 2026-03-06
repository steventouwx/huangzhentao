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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmmSimBaseHandler.h"
#include "RmmSmlDefs.h"
#include "RmmSmlUrcHandler.h"
#include "RfxRsuRequestData.h"
#include "RfxRsuResponseData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxSimMeLockCatData.h"
#include "RfxVoidData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include "RfxSimCarrRstData.h"

using ::android::String8;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmSmlUrcHandler, RIL_CMD_PROXY_URC);

static const int smlUrcList[] = {
    /*
     * +ESMLSTATUS: <rule>, <subRule>, <lockState>, <sercice>,<validCard>
     *
     */
    MIPC_SIM_SML_STATUS_IND,
    /*
     * OPERATOR_ID
     * EVENT_ID
     * EVENT_STRING
     */
    MIPC_SIM_SML_RSU_IND,

};

//Register unsolicited message
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIM_SLOT_LOCK_POLICY_NOTIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SIM_RESET);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_SML_RSU_EVENT);
//for op radio
//RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_RSU_EVENT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_ATT_SIM_LOCK_NOTIFICATION);
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmmSmlUrcHandler::RmmSmlUrcHandler(int slot_id, int channel_id) :
        RmmSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmmSmlUrc"));
    logD(mTag, "RmmSmlUrcHandler init");

    if (isSimSlotLockSupport()) {
        logD(mTag, "Slot Lock on and reset the properties and key-values: slot %d", slot_id);

        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_POLICY, "-1");
        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_STATE, "-1");
        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_SERVICE_CAPABILITY[slot_id], "-1");
        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_CARD_VALID[slot_id], "-1");

        getNonSlotMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY,
            SML_SLOT_LOCK_POLICY_UNKNOWN, true);
        getNonSlotMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_LOCK_STATE,
            SML_SLOT_LOCK_POLICY_LOCK_STATE_UNKNOWN, true);
        getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY,
            SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_UNKNOWN, true);
        getMclStatusManager()->setIntValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_VALID_CARD,
            SML_SLOT_LOCK_POLICY_VALID_CARD_UNKNOWN, true);
    } else {
        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_POLICY, "0");
        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_STATE, "1");
        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_SERVICE_CAPABILITY[slot_id], "0");
        rfx_property_set(PROPERTY_SIM_SLOT_LOCK_CARD_VALID[slot_id], "0");
    }

    int record_num = sizeof(smlUrcList)/sizeof(int);
    registerToHandleMipcURC(smlUrcList, record_num);
}

RmmSmlUrcHandler::~RmmSmlUrcHandler() {
}

void RmmSmlUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipc_urc_data = msg->getMipcData();
    if (mipc_urc_data == NULL) {
        logE(mTag, "onHandleUrc: Error, mipc message data is null");
        return;
    }

    uint16_t mipcMsgId = mipc_urc_data->getMipcMsgId();
    logD(mTag, "onHandleUrc: mipc messageId:%04x", mipcMsgId);

    switch (mipcMsgId) {
        case MIPC_SIM_SML_STATUS_IND:
            handleSmlStatusInd(msg);
            break;

        case MIPC_SIM_SML_RSU_IND:
            handleSmlRsuInd(msg);
            break;

        default:
            logD(mTag, "onHandleUrc: unhandled:%04x", mipcMsgId);
            break;
    }
}

void RmmSmlUrcHandler::handleSmlStatusInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipc_urc_data = msg->getMipcData();
    int sml_status[4] = {0};
    int old_service = 0;
    int old_valid_info = 0;
    bool is_cap_changed = false;
    unsigned char slot_lock_rule = 0;
    unsigned char capability_rule = 0;
    char *property_lock_policy = NULL;
    char *property_lock_state = NULL;
    char *property_service = NULL;
    char *property_valid_card = NULL;
    sp<RfxMclMessage> response;

    int rule = (int)mipc_urc_data->getMipcUint8Val(MIPC_SIM_SML_STATUS_IND_T_LOCK_RULE, 0);
    int sub_rule = (int)mipc_urc_data->getMipcUint16Val(
            MIPC_SIM_SML_STATUS_IND_T_LOCK_SUB_RULE,
            0);
    int lock_state = (int)mipc_urc_data->getMipcUint8Val(
            MIPC_SIM_SML_STATUS_IND_T_DEVICE_LOCK_STATE,
            0);
    int service = (int)mipc_urc_data->getMipcUint8Val(
            MIPC_SIM_SML_STATUS_IND_T_RULE_POLICY,
            0);
    int valid_card = (int)mipc_urc_data->getMipcUint8Val(
            MIPC_SIM_SML_STATUS_IND_T_SIM_VALIDITY,
            0);

    int permLockRemaining = (int)mipc_urc_data->getMipcUint8Val(
            MIPC_SIM_SML_STATUS_IND_T_DEVICE_LOCK_REMAIN_CNT,
            0);

    if (!getNonSlotMclStatusManager()->getBoolValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_MD_SUPPORTED, false)) {
        getNonSlotMclStatusManager()->setBoolValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_MD_SUPPORTED, true, false);
    }

    if (!isSimSlotLockSupport()) {
        logE(mTag, "handleSimSlotLockStatusChanged: not support SML so return! slot %d", m_slot_id);
        return;
    }

    logD(mTag, "handleSimSlotLockStatusChanged: rule: %d, sub_rule: %d, lock_state: %d, \
            service: %d, valid_card: %d, slot %d, perm_remaining %d", rule, sub_rule, lock_state,
            service, valid_card, m_slot_id, permLockRemaining);

    setSimSlotLockDeviceLockInfoProp(permLockRemaining);
    //Parse and update lock policy rule into sml_status[0], currently MD only uses 4 bits of sub_rule
    slot_lock_rule = sub_rule & SML_BITMASK_SLOT_LOCK_CONTEXT;
    capability_rule = (sub_rule & SML_BITMASK_INVALID_SIM_CAP) >> 1;
    sml_status[0] = -1;
    if (SML_NO_RULE == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_NONE;
        }
    } else if (SML_SIM1_SLOT_LOCK_ONLY == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_ONLY_SLOT1;
        }
    } else if (SML_SIM2_SLOT_LOCK_ONLY == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_ONLY_SLOT2;
        }
    } else if (SML_ALL_SIM_LOCK == rule) {
        if ((SML_SLOT_LOCK_NOT_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL;
        } else if (sub_rule == SML_MD_SUB_RULE_TRUE) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_TRUE;
        }
    } else if (SML_LINK_LOCK_WITH_SIM1_SLOT == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOT1;
        }
    } else if (SML_LINK_LOCK_WITH_SIM2_SLOT == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOT2;
        }
    } else if (SML_LINK_LOCK_WITH_ANY_SLOT == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_CS_ONLY == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_CS;
        } else if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)){
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOTA;
        }
    } else if (SML_SLOTA_RESTRICT_INVALID_VOICE == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_VOICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_VOICE;
        }
    } else if (SML_SLOTA_RESTRICT_REVERSE == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_REVERSE;
        }
    } else if (SML_RESTRICT_INVALID_ECC_FOR_VALID_NO_SERVICE == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] =
                SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_ECC_FOR_VALID_NO_SERVICE;
        } else if (sub_rule == SML_MD_SUB_RULE_JIO_PLMN) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_JIO_PLMN;
        }
    } else if (SML_ALL_SIM_LOCK_AND_RSU_VZW == rule) {
        if ((SML_SLOT_LOCK_NOT_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL_AND_RSU_VZW;
        }
    } else if (SML_SLOTA_RSU_KDDI == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOTA_RSU_KDDI;
        }
    } else if (SML_SLOTA_RSU_TMO == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LK_SLOTA_RSU_TMO;
        }
    } else if (SML_LEGACY_MODE == rule) {
        if ((SML_SLOT_LOCK_USE_SAME_CONTEXT == slot_lock_rule) &&
            (SML_INVALID_SIM_FULL_SERVICE == capability_rule)) {
            sml_status[0] = SML_SLOT_LOCK_POLICY_LEGACY;
        }
    }

    /*
     * Transfer the rule/sub_rule to upper layer avoiding modifying vendor part when add
     * a new policy.
     */
    if (sml_status[0] == -1) {
        logI(mTag, "handleSimSlotLockStatusChanged: no legacy policy! slot %d", m_slot_id);
        sml_status[0] = ((rule & 0xFFFF)) << 16 | (sub_rule & 0xFFFF);
    }

    //Update other slot lock related status
    sml_status[1] = lock_state;
    sml_status[2] = service - 1;
    sml_status[3] = valid_card - 1;

    //Set SIM slot lock related properties
    asprintf(&property_lock_policy, "%d", sml_status[0]);
    asprintf(&property_lock_state, "%d", sml_status[1]);
    asprintf(&property_service, "%d", sml_status[2]);
    asprintf(&property_valid_card, "%d", sml_status[3]);

    rfx_property_set(PROPERTY_SIM_SLOT_LOCK_POLICY, property_lock_policy);
    rfx_property_set(PROPERTY_SIM_SLOT_LOCK_STATE, property_lock_state);
    rfx_property_set(PROPERTY_SIM_SLOT_LOCK_SERVICE_CAPABILITY[m_slot_id], property_service);
    rfx_property_set(PROPERTY_SIM_SLOT_LOCK_CARD_VALID[m_slot_id], property_valid_card);

    free(property_lock_policy);
    free(property_lock_state);
    free(property_service);
    free(property_valid_card);

    //Check whether the service and valid info changed
    old_service = getMclStatusManager()->getIntValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY);
    old_valid_info = getMclStatusManager()->getIntValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_VALID_CARD);
    if (old_service != sml_status[2] || old_valid_info != sml_status[3]) {
        is_cap_changed = true;
        logD(mTag, "handleSimSlotLockStatusChanged: capability changed! slot %d", m_slot_id);
    }

    //Set SIM slot lock key-values
    getNonSlotMclStatusManager()->setIntValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY, sml_status[0], true);
    getNonSlotMclStatusManager()->setIntValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_LOCK_STATE, sml_status[1], true);
    getMclStatusManager()->setIntValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY, sml_status[2], true);
    getMclStatusManager()->setIntValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_VALID_CARD, sml_status[3], true);

    logD(mTag, "handleSimSlotLockStatusChanged sml_status: %d, %d, %d, %d, slot %d",
            sml_status[0], sml_status[1], sml_status[2], sml_status[3], m_slot_id);
    response = RfxMclMessage::obtainUrc(
        RFX_MSG_URC_SIM_SLOT_LOCK_POLICY_NOTIFY, m_slot_id, RfxIntsData(sml_status, 4));
    responseToTelCore(response);

    if (is_cap_changed) {
        sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
            m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
    }
    return;
}

bool RmmSmlUrcHandler::isSimSlotLockSupport() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("ro.vendor.sim_me_lock_mode", property_value, "");

    if ((strlen(property_value) > 0) && (strcmp(property_value, "3") == 0)){
        return true;
    }
    return false;
}

void RmmSmlUrcHandler::setSimSlotLockDeviceLockInfoProp(int remaining) {
    rfx_property_set(PROPERTY_SIM_SLOT_LOCK_DEVICE_LOCK_REMAIN_COUNT,
            String8::format("%d", remaining).string());
}


void RmmSmlUrcHandler::handleSmlRsuInd(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipc_urc_data = msg->getMipcData();
    sp<RfxMclMessage> p_urc = NULL;

    int opId = (int)mipc_urc_data->getMipcUint8Val(MIPC_SIM_SML_RSU_IND_T_OPERATOR_ID, 0);
    int eventId = (int)mipc_urc_data->getMipcUint16Val(MIPC_SIM_SML_RSU_IND_T_EVENT_ID, 0xFFFFU);
    char *eventString = mipc_urc_data->getMipcStr(MIPC_SIM_SML_RSU_IND_T_EVENT_STRING);
    char* event[2] = {NULL, NULL};

    logD(mTag, "[RSU-SIMLOCK] handleRsuIndication: opId=%d, eventId=%d, eventString=%s",
            opId, eventId, eventString == NULL ? "" : eventString);

    int opEventId = (opId << 16) | eventId;
    switch(opId) {
        case RSU_OP_TMO:
            handleTmoLockEvent(opId, eventId, eventString);
            break;
        case RSU_OP_VZW:
        case RSU_OP_TMO_LEGACY:
            asprintf(&event[0], "%d", opEventId);
            asprintf(&event[1], "%s", (eventString == NULL ? "" : eventString));
            p_urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SML_RSU_EVENT,
                    m_slot_id, RfxStringsData(event, 2));
            responseToTelCore(p_urc);
            if (event[0] != NULL) {
                free(event[0]);
            }
            if (event[1] != NULL) {
                free(event[1]);
            }
            break;

        case RSU_OP_ATT_LEGACY:
            p_urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_ATT_SIM_LOCK_NOTIFICATION,
                            m_slot_id, RfxIntsData(&eventId, 1));
            responseToTelCore(p_urc);
            break;

        case RSU_OP_JIO_LEGACY:
            handleSubsidyLockEvent(eventId, eventString);
            break;

        default:
            logD(mTag, "[RSU-SIMLOCK] handleRsuIndication: Unknown opId=%d", opId);
            break;
    }
}

void RmmSmlUrcHandler::handleTmoLockEvent(int opId, int eventId, char *eventString){
    int value = -1;
    char* event[2] = {NULL, NULL};
    eventId = eventId & 0xFF;

    if (eventId == 2) {
        if (eventString != NULL) {
            value = atoi(eventString);
        }
        logD(mTag, "handleTmoRsuEvent: lockState: %d", value);
        eventId = ((value & 0xFF) << 8) | eventId;
    }

    logD(mTag, "handleTmoLockEvent: 0x%X", eventId);
    int opEventId = (opId << 16) | eventId;
    asprintf(&event[0], "%d", opEventId);
    asprintf(&event[1], "%s", (eventString == NULL ? "" : eventString));

    logD(mTag, "handleTmoLockEvent: 0x%X", opEventId);
    sp<RfxMclMessage> p_urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SML_RSU_EVENT,
            m_slot_id, RfxStringsData(event, 2));
    responseToTelCore(p_urc);
    if (event[0] != NULL) {
        free(event[0]);
    }
    if (event[1] != NULL) {
        free(event[1]);
    }
    return;
}

/**
 * Handle subsidy lock event
 * Format: +ERJIOEVT:<event>,<value>
 *  <event>: integer
 *          0 -- unlock time expires, reset sim needed
 *             value -- 0
 *         1 -- Slot operation event
 *             value -- integer
 *                 B1 ~ B4: slotx disable/enable, 0 -- disable, 1 -- enable
 *                 B5: 0 -- value of B1 ~ B4 is ignored
 *                     1 -- value of B1 ~ B4 is valid
 *                 B6 ~ B8: RFU
 */
void RmmSmlUrcHandler::handleSubsidyLockEvent(int eventId, char *eventString) {
    int value = -1;
    if (eventString != NULL) {
        value = atoi(eventString);
    }
    if (eventId == 0 && value == 0) {   //reset sim
        logD(mTag, "handleSubsidyLockEvent: unlock time expires!");
        int simCount = RfxRilUtils::rfxGetSimCount();
        for (int i=0; i<simCount; i++) {
            logD(mTag, "handleSubsidyLockEvent: sendEvent to %d", i);
            sendEvent(RFX_MSG_EVENT_SIM_RESET, RfxVoidData(), RIL_CMD_PROXY_7, i);
        }
    } else if (eventId == 0 && value == 1) {
        logD(mTag, "handleSubsidyLockEvent: Slot status change!");
    } else {
        logE(mTag, "handleSubsidyLockEvent: Unknow URC!");
    }

    //Notify the event to upper layer avoiding modifying vendor for new event.
    char* event[2] = {NULL, NULL};
    eventId = eventId & 0xFF;

    int opEventId = (RSU_OP_JIO_LEGACY << 16) | eventId;
    asprintf(&event[0], "%d", opEventId);
    asprintf(&event[1], "%s", (eventString == NULL ? "" : eventString));

    logD(mTag, "handleSubsidyLockEvent: opEventId: 0x%X, eventString: %s",
            opEventId, event[1]);
    sp<RfxMclMessage> p_urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SML_RSU_EVENT,
            m_slot_id, RfxStringsData(event, 2));
    responseToTelCore(p_urc);
    if (event[0] != NULL) {
        free(event[0]);
    }
    if (event[1] != NULL) {
        free(event[1]);
    }
    return;
}

