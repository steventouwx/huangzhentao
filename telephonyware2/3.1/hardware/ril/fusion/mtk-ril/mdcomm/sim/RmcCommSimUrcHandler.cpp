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
#include "RfxStatusDefs.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RmcSimBaseHandler.h"
#include "RmcCommSimUrcHandler.h"
#include "RmcCommSimDefs.h"
#include <telephony/mtk_ril.h>
#include "utils/String8.h"
#include "rfx_properties.h"
#include "RfxMessageId.h"
#include "RfxMisc.h"
#include "RfxRilUtils.h"
#include "RmcCapabilitySwitchUtil.h"
#include "RmcCapabilitySwitchRequestHandler.h"
#include "RfxSimSlotStatusData.h"
#include "vsim/RmcVsimRequestHandler.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <hardware_legacy/mtk_power.h>
#ifdef __cplusplus
}
#endif


using ::android::String8;

static const char* commUrcList[] = {
    "+EUSIM:",
    "+ETESTSIM:",
    "+ESIMS:",
    "+ECPIN:",
    "+ESIMIND:",
    "+ICCID",
    "+ESMLSTATUS:",
    "+ESLOTSINFO:",
    "+ESMLRSUEVT:",
    "+EATTEVT:",
    "+ETMOEVT:",
    "+ERJIOEVT:",
};

// Register unsolicited message
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_UICC_APPLICATIONS_ENABLEMENT_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIM_INIT_STATUS);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_IMEI_LOCK);

int RmcCommSimUrcHandler::mTrayPluginCount = 0;

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcCommSimUrcHandler::RmcCommSimUrcHandler(int slot_id, int channel_id) :
        RmcSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcCommSimUrc"));

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
}

RmcCommSimUrcHandler::~RmcCommSimUrcHandler() {
}

RmcSimBaseHandler::SIM_HANDLE_RESULT RmcCommSimUrcHandler::needHandle(
        const sp<RfxMclMessage>& msg) {
    RmcSimBaseHandler::SIM_HANDLE_RESULT result = RmcSimBaseHandler::RESULT_IGNORE;
    char* ss = msg->getRawUrc()->getLine();

    if (strStartsWith(ss, "+EUSIM:") ||
            strStartsWith(ss, "+ESIMS: 0,0") ||
            strStartsWith(ss, "+ESIMS: 0,5") ||
            strStartsWith(ss, "+ESIMS: 0,10") ||
            strStartsWith(ss, "+ESIMS: 0,11") ||
            strStartsWith(ss, "+ESIMS: 0,13") ||
            strStartsWith(ss, "+ESIMS: 0,15") ||
            strStartsWith(ss, "+ESIMS: 0,19") ||
            // MTK-START: AOSP SIM PLUG IN/OUT
            strStartsWith(ss, "+ESIMS: 0,26") ||
            // MTK-EDN
            strStartsWith(ss, "+ESIMS: 1,2") ||
            strStartsWith(ss, "+ESIMS: 1,6") ||
            strStartsWith(ss, "+ESIMS: 1,9") ||
            strStartsWith(ss, "+ESIMS: 1,12") ||
            strStartsWith(ss, "+ESIMS: 1,14") ||
            strStartsWith(ss, "+ECPIN:") ||
            strStartsWith(ss, "+ESIMIND:") ||
            strStartsWith(ss, "+ICCID:") ||
            strStartsWith(ss, "+ESMLSTATUS:") ||
            strStartsWith(ss, "+ERJIOEVT:") ||
            strStartsWith(ss, "+ESIMS: 0,28") ||
            strStartsWith(ss, "+ESIMS: 1,29") ||
            strStartsWith(ss, "+ESLOTSINFO:") ||
            strStartsWith(ss, "+ETESTSIM:")   ||
            strStartsWith(ss, "+ESMLRSUEVT:")   ||
            strStartsWith(ss, "+EATTEVT:") ||
            strStartsWith(ss, "+ETMOEVT:")) {
        result = RmcSimBaseHandler::RESULT_NEED;
    }
    return result;
}

void RmcCommSimUrcHandler::handleUrc(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
    char* ss = urc->getLine();

    if(strStartsWith(ss, "+EUSIM:")) {
        handleCardType(msg, urc);
    } else if (strStartsWith(ss, "+ESIMS:")) {
        handleSimStateChanged(msg, (char*)ss);
    } else if (strStartsWith(ss, "+ECPIN:")) {
        handleEcpin(msg, urc);
    } else if (strStartsWith(ss, "+ESIMIND:")) {
        handleSimIndication(msg, urc);
    } else if (strStartsWith(ss, "+ICCID:")) {
        handleIccidIndication(msg, urc);
    } else if (strStartsWith(ss, "+ESMLSTATUS:")) {
        handleSimSlotLockStatusChanged(urc);
    } else if (strStartsWith(ss, "+ESLOTSINFO:")) {
        handleSimSlotStatusChanged(urc);
    } else if (strStartsWith(ss, "+ETESTSIM:")) {
        handleTestSimDetected(urc);
    } else if (strStartsWith(ss, "+ESMLRSUEVT:")) {
        handleRsuEvent(msg, urc);
    } else if (strStartsWith(ss, "+ETMOEVT:")) {
        handleTmoRsuEvent(msg, urc);
    } else if (strStartsWith(ss, "+ERJIOEVT:")) {
        handleSubsidyLockEvent(urc);
    } else if (strStartsWith(ss, "+EATTEVT:")) {
        handleAttRsuSimLockEvent(msg, ss);
    }


}

const char** RmcCommSimUrcHandler::queryUrcTable(int *record_num) {
    const char** p = commUrcList;
    *record_num = sizeof(commUrcList)/sizeof(char*);
    return p;
}

void RmcCommSimUrcHandler::handleEcpin(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
    int err = 0, cpin_status = 0, vsim_init = -1;
    RfxAtLine *atLine = urc;
    sp<RfxMclMessage> unsol = NULL;

    atLine->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    cpin_status = atLine->atTokNextint(&err);
    if (err < 0) {
        goto error;
    }

    vsim_init = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_ECPIN_STATE,
            RFX_ECPIN_DONE);
    logD(mTag, "CPIN status %d, VSIM initialization state %d", cpin_status, vsim_init);
    if ((cpin_status == 1) && (vsim_init == RFX_WAIT_FOR_ECPIN)) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_ECPIN_STATE, RFX_ECPIN_DONE);
        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
    }
    return;
error:
    logE(mTag, "Parse ECPIN fail: %s/n", msg->getRawUrc()->getLine());
}

void RmcCommSimUrcHandler::handleCardType(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
    int type3gpp = 0, type3gpp2 = 0, typeIsim = 0, typeValue = -1, err = 0;
    RfxAtLine *atLine = urc;
    String8 type("");
    String8 fullUiccCardType("vendor.gsm.ril.fulluicctype");
    String8 propUicc("vendor.gsm.ril.uicctype");
    bool isMdSupportIccid = getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MD_ICCID_SUPPORT, false);
    bool aospEnabled = isEnableUiccApplicationsSupported();

    // Check whether the feature configuration of the SML is right
    onSmlConfigureCheck();
    propUicc.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
    fullUiccCardType.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));

    atLine->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    type3gpp = atLine->atTokNextint(&err);
    if (err < 0) {
        goto error;
    }

    if (type3gpp == 1) {
        rfx_property_set(propUicc, "USIM");
        type.append("USIM");
        typeValue = RFX_CARD_TYPE_USIM;
    } else if (type3gpp == 0) {
        rfx_property_set(propUicc, "SIM");
        type.append("SIM");
        typeValue = RFX_CARD_TYPE_SIM;
    } else {
        logD(mTag, "The SIM card is neither USIM nor SIM!");
        typeValue = 0;
    }

    /* Check CDMA card type */
    if (atLine->atTokHasmore()) {
        type3gpp2 = atLine->atTokNextint(&err);
        if (err < 0) {
            logE(mTag, "Get CDMA type fail!");
        } else {
            switch (type3gpp2) {
                case 0:
                    rfx_property_set(propUicc, "RUIM");
                    if (type.length() > 0) {
                        type.append(",");
                    }
                    type.append("RUIM");
                    typeValue |= RFX_CARD_TYPE_RUIM;
                    break;
                case 1:
                    rfx_property_set(propUicc, "CSIM");
                    if (type.length() > 0) {
                        type.append(",");
                    }
                    type.append("CSIM");
                    typeValue |= RFX_CARD_TYPE_CSIM;
                    break;
                case 3:
                    rfx_property_set(propUicc, "CSIM");
                    if (type.length() > 0) {
                        type.append(",");
                    }
                    type.append("RUIM,CSIM");
                    typeValue |= RFX_CARD_TYPE_RUIM;
                    typeValue |= RFX_CARD_TYPE_CSIM;
                    break;
                default:
                    logD(mTag, "The SIM card is neither RUIM nor CSIM!");
                    break;
            }
        }
    }

    // Check ISIM
    if (atLine->atTokHasmore()) {
        typeIsim = atLine->atTokNextint(&err);
        if (err < 0) {
            logE(mTag, "Fail to get ISIM state!");
        } else {
            switch (typeIsim) {
                case 1:
                    typeValue |= RFX_CARD_TYPE_ISIM;
                    break;
                default:
                    break;
            }
        }
    }

    if ((type3gpp == 2 && type3gpp2 == 2) || (type3gpp == 2 && type3gpp2 == -1)) {
        logD(mTag, "There is no card type!!!");
        type.append("N/A");
        rfx_property_set(fullUiccCardType, "");
        rfx_property_set(propUicc, "");
        typeValue = 0;
    } else {
        // property_set(propUicc, card_type);
        rfx_property_set(fullUiccCardType, type.string());
    }

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, typeValue);
    if (typeValue >= 0) {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_SIM_TASK_READY, true, true);
    }
    if (typeValue == 0) {
        //getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_INIT_STATUS, -1);
        int ready = SIM_INIT_STATE_ABSENT;
        sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_INIT_STATUS,
                m_slot_id, RfxIntsData(&ready, 1));
        responseToTelCore(unsol);
    } else if (typeValue > 0) {
        int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_INIT_STATUS,
                SIM_INIT_STATE_ABSENT);
        if (status < SIM_INIT_STATE_DETECT) {
            int ready = SIM_INIT_STATE_DETECT;
            sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_INIT_STATUS,
                    m_slot_id, RfxIntsData(&ready, 1));
            responseToTelCore(unsol);
        }
    }
    if ((!aospEnabled && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_ON)) || (aospEnabled && (getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            == UICC_APPLICATIONS_ENABLE_STATE_ENABLED))) {
        // To query ICCID
        mtk_acquire_wake_lock("sim_hot_plug");
        sendEvent(RFX_MSG_EVENT_SIM_QUERY_ICCID, RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);
    }
    return;
error:
    logE(mTag, "Parse EUSIM fail: %s/n", msg->getRawUrc()->getLine());
}

void RmcCommSimUrcHandler::handleSimStateChanged(const sp<RfxMclMessage>& msg, char* urc) {
    bool common_slot_no_changed = false;

    RFX_UNUSED(msg);

    // +ESIMS: 0,0: SIM Missing
    // +ESIMS: 0,13: Recovery start
    // +ESIMS: 0,10: Virtual SIM off
    // +ESIMS: 0,11: SIM plug out
    // +ESIMS: 0,15: ATT IMEI Lock
    // +ESIMS: 0,19: SPR IMEI Lock
    // MTK-START: AOSP SIM PLUG IN/OUT
    // +ESIMS: 0,26: SIM plug in, but no init
    // MTK-END
    // +ESIMS: 1,9: Virtual SIM on
    // +ESIMS: 1,14: Recovery end
    // +ESIMS: 1,12: SIM plug in
    // +ESIMS: 0,28: SIM off
    // +ESIMS: 1,29: SIM on
    sp<RfxMclMessage> unsol = NULL;

    char* p_esim_cause = NULL;
    int esim_cause = -1;
    p_esim_cause = strchr(urc, ',');
    if (p_esim_cause != NULL) {
        esim_cause = atoi(p_esim_cause + 1);
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ESIMS_CAUSE, esim_cause);
    }

    if (strStartsWith(urc,"+ESIMS: 0,0") || strStartsWith(urc,"+ESIMS: 0,10") ||
            strStartsWith(urc,"+ESIMS: 0,11") || strStartsWith(urc,"+ESIMS: 0,13") ||
            strStartsWith(urc,"+ESIMS: 0,15")) {
        if (strStartsWith(urc,"+ESIMS: 0,11")) {
            //getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_INIT_STATUS, -1);
            int ready = SIM_INIT_STATE_ABSENT;
            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_INIT_STATUS,
                    m_slot_id, RfxIntsData(&ready, 1));
            responseToTelCore(unsol);
            unsol = NULL;

            if (RfxRilUtils::rfxGetSimCount() >= 2 && isCommontSlotSupport() == true) {
                if (!isSimInserted()) {
                    common_slot_no_changed = true;
                }
            }
        }
        if (strStartsWith(urc, "+ESIMS: 0,11") || strStartsWith(urc, "+ESIMS: 0,13") ||
                strStartsWith(urc, "+ESIMS: 0,10")) {
            logD(mTag, "common_slot_no_changed: %d", common_slot_no_changed);

            resetSimPropertyAndStatusKey();

            //String8 iccId(PROPERTY_ICCID_PREIFX);
            //iccId.append(String8::format("%d", (m_slot_id + 1)));
            //rfx_property_set(iccId.string(), "N/A");

            String8 fullIccType(PROPERTY_FULL_UICC_TYPE);
            fullIccType.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
            rfx_property_set(fullIccType.string(), "");

            String8 uiccType(PROPERTY_UICC_TYPE);
            uiccType.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
            rfx_property_set(uiccType.string(), "");

            // TODO: Stk patch plug in ?
            // TODO: Need reset Aid info?
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, 0);
            getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_SIM_TASK_READY, true, true);
            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8("N/A"));
            sendIccidChanged("N/A");

            // Set SIM on when card plugs out.
            if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_SIMONOFF_SUPPORT)) {
                getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE,
                        SIM_POWER_STATE_SIM_ON);
                String8 simOnOff(PROPERTY_SIM_ONOFF_STATE);
                simOnOff.append(String8::format("%d", (m_slot_id + 1)));
                rfx_property_set(simOnOff.string(), String8::format("%d", SIM_POWER_STATE_SIM_ON)
                        .string());
                getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ONOFF_SWITCHING_STATE, -1);
                // Notify SIM power state.
                int simPower = SIM_POWER_STATE_SIM_ON;
                sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_POWER_CHANGED,
                        m_slot_id, RfxIntsData(&simPower, 1));
                responseToTelCore(unsol);
            }

            if (isEnableUiccApplicationsSupported()) {
                getMclStatusManager()->setIntValue(
                        RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE,
                        UICC_APPLICATIONS_ENABLE_STATE_ENABLED);
                String8 onOffState(PROPERTY_UICC_APPLICATIONS_ENABLE_STATE);
                onOffState.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
                rfx_property_set(onOffState.string(), String8::format("%d",
                        UICC_APPLICATIONS_ENABLE_STATE_ENABLED).string());
            }

            if (common_slot_no_changed == true) {
                // External SIM [Start]
                if (RfxRilUtils::isVsimEnabled()) {
                    logD(mTag, "Ignore no changed event during vsim enabled on common slot project.");
                } else {
                // External SIM [End]
                    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_COMMON_SLOT_NO_CHANGED,
                            m_slot_id, RfxVoidData());
                    responseToTelCore(unsol);
                }
            } else {
                // External SIM [Start]
                if ((isCommontSlotSupport() == true)
                        && (RmcVsimRequestHandler::getVsimPlugInOutEvent(m_slot_id) ==
                        VSIM_TRIGGER_PLUG_OUT)) {
                    logD(mTag, "vsim trigger plug out on common slot project.");
                    RmcVsimRequestHandler::setVsimPlugInOutEvent(m_slot_id, VSIM_TRIGGER_RESET);
                } else if ((isCommontSlotSupport() == true) && RfxRilUtils::isVsimEnabled()) {
                    logD(mTag, "Ignore plug out event during vsim enabled on common slot project.");
                } else {
                // External SIM [End]
                    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_PLUG_OUT,
                            m_slot_id, RfxVoidData());

                    if (isCommontSlotSupport() == true) {
                        // To send the event plug out only when card removed in common slot project
                        if (strStartsWith(urc, "+ESIMS: 0,11")) {
                            responseToTelCore(unsol);
                        }
                    } else {
                        responseToTelCore(unsol);
                    }
                }

                unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                        m_slot_id, RfxVoidData());
                responseToTelCore(unsol);
            }
        } else if (strStartsWith(urc, "+ESIMS: 0,15")) {
            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_IMEI_LOCK,
                    m_slot_id, RfxVoidData());
            responseToTelCore(unsol);
        } else {
            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                    m_slot_id, RfxVoidData());
            responseToTelCore(unsol);
        }
    // MTK-START: AOSP SIM PLUG IN/OUT
    } else if (strStartsWith(urc, "+ESIMS: 0,26")) {
        logD(mTag, "SIM Plug in but no init.");
        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
    // MTK-END
    } else if (strStartsWith(urc, "+ESIMS: 1,9") || strStartsWith(urc, "+ESIMS: 1,14") || strStartsWith(urc, "+ESIMS: 1,12")) {
        if (strStartsWith(urc, "+ESIMS: 1,12")) {
            //String8 iccId(PROPERTY_ICCID_PREIFX);
            //iccId.append(String8::format("%d", (m_slot_id + 1)));
            //rfx_property_set(iccId.string(), "");
            //getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_INIT_STATUS, 0);
            int ready = SIM_INIT_STATE_INSERT;
            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_INIT_STATUS,
                    m_slot_id, RfxIntsData(&ready, 1));
            responseToTelCore(unsol);
            unsol = NULL;

            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_EID, String8(""));
            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8(""));
            sendIccidChanged("");

            String8 uiccType(PROPERTY_UICC_TYPE);
            uiccType.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
            rfx_property_set(uiccType.string(), "unKnown");
        }
        if (RfxRilUtils::rfxGetSimCount() >= 2) {
            if(isCommontSlotSupport() == true && (strStartsWith(urc, "+ESIMS: 1,12"))) {
               // External SIM [Start]
                if ((isCommontSlotSupport() == true)
                        && (RmcVsimRequestHandler::getVsimPlugInOutEvent(m_slot_id) ==
                        VSIM_TRIGGER_PLUG_IN)) {
                    logD(mTag, "vsim trigger tray plug in on common slot project.");
                    //setVsimPlugInOutEvent(m_slot_id, VSIM_TRIGGER_RESET);
                } else if ((isCommontSlotSupport() == true) && RfxRilUtils::isVsimEnabled()) {
                    logD(mTag, "Ignore tray plug in event during vsim enabled on common slot project.");
                } else {
                // External SIM [End]

                    // In this feature, when we receive "ESIMS: 1, 12", it does not mean SIM card plug,
                    // but means slot plug in. That is, it might be no SIM card in this slot.
                    // Thus, we need to query SIM state when detect SIM missing and update flag at that time.
                    logD(mTag, "Receive plug in in common slot project so do not set sim inserted status here");

                    int simCount = RfxRilUtils::rfxGetSimCount();
                    logD(mTag, "mTrayPluginCount: %d (slot %d)",
                            mTrayPluginCount, m_slot_id);
                    // Use static variable mTrayPluginCount to count the sim number and clear all
                    // of slot's task key vaule for the first tray_plug_in coming.It uses to reduce
                    // mode switch times for common slot plug in.
                    if (mTrayPluginCount == 0) {
                        mTrayPluginCount = simCount - 1;
                        for (int i = 0; i < simCount; i++) {
                            getMclStatusManager(i)->setBoolValue(RFX_STATUS_KEY_MODEM_SIM_TASK_READY,
                                    false);
                        }
                    } else {
                        mTrayPluginCount--;
                    }

                    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_TRAY_PLUG_IN, m_slot_id, RfxVoidData());
                    responseToTelCore(unsol);
                }
            }
        }
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
        //RLOGD("detectSim before acquire_wake_lock");
        mtk_acquire_wake_lock("sim_hot_plug");
        //RLOGD("detectSim after acquire_wake_lock");
        sendEvent(RFX_MSG_EVENT_SIM_DETECT_SIM, RfxStringData(urc), RIL_CMD_PROXY_1, m_slot_id);
    } else if (strStartsWith(urc, "+ESIMS: 1,2,")) {
        // +ESIMS: 1,2,x: x mean SIM REFRESH Additional Cause

        // +ESIMS: 1,2,0: NAA_INIT_AND_FULL_FCN
        // +ESIMS: 1,2,1: FCN
        // +ESIMS: 1,2,2: NAA_INIT_AND_FCN
        // +ESIMS: 1,2,3: NAA INIT

        // +ESIMS: 1,2,4: UICC RESET
        if (0 == strcmp(urc, "+ESIMS: 1,2,4")) {
            logD(mTag, "SIM_REFRESH: eUICC RESET");
            resetSimPropertyAndStatusKey();
        }

        // +ESIMS: 1,2,5: NAA APP RESET
        // +ESIMS: 1,2,6: NAA SESSION RESET
        // +ESIMS: 1,2,7: STEERING OF ROAMING
        // +ESIMS: 1,2,8: I-WLAN STEERING OF ROAMING
        // +ESIMS: 1,2,9: eUICC PROFILE STATE CHANGE
        // +ESIMS: 1,2,10: APP UPDATE

        // +ESIMS: 1,2,255: SIM REFRESH END
        if (0 == strcmp(urc, "+ESIMS: 1,2,255")) {
            logD(mTag, "SIM_REFRESH_DONE");
            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                    m_slot_id, RfxVoidData());
            responseToTelCore(unsol);
            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_IMSI_REFRESH_DONE,
                    m_slot_id, RfxVoidData());
            responseToTelCore(unsol);
        }
    } else if (strStartsWith(urc, "+ESIMS: 1,6")) {
        logD(mTag, "SIM Access Profile Off");
        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
    } else if (strStartsWith(urc, "+ESIMS: 0,5")) {
        logD(mTag, "SIM Access Profile On");
        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
    } else if ((0 == strcmp(urc, "+ESIMS: 0,28")) || (0 == strcmp(urc, "+ESIMS: 1,29"))) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_POWER_STATE, SIM_POWER_STATE_UNKNOWN);
        if (isEnableUiccApplicationsSupported()) {
            handleUiccApplicationsEnablementChanged(msg, urc);
            return;
        }
        if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_SIMONOFF_SUPPORT)) {
            int state = -1;
            if (strStartsWith(urc, "+ESIMS: 0,28")) {
                state = SIM_POWER_STATE_SIM_OFF;
            } else {
                state = SIM_POWER_STATE_SIM_ON;
            }

            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE, state);
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ONOFF_SWITCHING_STATE, -1);
            String8 simOnOff(PROPERTY_SIM_ONOFF_STATE);
            simOnOff.append(String8::format("%d", (m_slot_id + 1)));
            rfx_property_set(simOnOff.string(), String8::format("%d", state).string());

            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_POWER_CHANGED, m_slot_id,
                    RfxIntsData(&state, 1));
            responseToTelCore(unsol);

            char proState[RFX_PROPERTY_VALUE_MAX] = { 0 };
            rfx_property_get(simOnOff.string(), proState, "-1");
            logD(mTag, "state: %d, property: %d (slot %d)", state, atoi(proState), m_slot_id);

            sendSimStatusChanged();
        }
    } else if (strStartsWith(urc, "+ESIMS: 0,19")) {
        logD(mTag, "IMEI lock is activated");
        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
    }
    // TODO: is needed to plug in this data function?
    //onSimInsertChangedForData(rid, s);
}

void RmcCommSimUrcHandler::handleSimIndication(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
    int err = 0, indEvent = -1;
    RfxAtLine *atLine = urc;
    int applist = 0;
    RFX_UNUSED(msg);

    atLine->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    indEvent = atLine->atTokNextint(&err);
    if (err < 0) {
        goto error;
    }

    switch (indEvent) {
        case 1:
            // Currently capability switch without RADIO_UNAVAILABLE, but modem SIM task
            // still has to do SIM initialization again.
            // After modem SIM task initialize SIM done, AP will get the URC "+ESIMINIT".
            // We have to notify the capability switch module the event and the proxy channels
            // will unlock
            if (RmcCapabilitySwitchUtil::isDssNoResetSupport()) {
                RmcCapabilitySwitchRequestHandler::notifySIMInitDone(m_slot_id);
            }
            break;
        case 2:
            // +ESIMIND: 2, <uicc_app_list>
            // uicc_app_list = is_csim_exist | is_usim_exist | is_isim_exist (currently isim always 0)
            // is_usim_exist:2 is_csim_exist:4 (is_csim_exist | is_usim_exist): 6
            // For icc card uicc_app_list:0
            applist = atLine->atTokNextint(&err);
            if (err < 0) {
                goto error;
            }

            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_ESIMIND_APPLIST, applist);
            logD(mTag, "Notify uicc app list, applist : %d", applist);
            break;
        default:
            break;
    }

    return;
error:
    logE(mTag, "handleSimIndication, Invalid parameters");
}

void RmcCommSimUrcHandler::handleIccidIndication(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
    int err = 0, indEvent = -1;
    RfxAtLine *atLine = urc;
    int state = 0;
    char* iccid = NULL;
    String8 prop("vendor.ril.iccid.sim");
    sp<RfxMclMessage> unsol = NULL;
    String8 iccidKey("");
    bool aospEnabled = isEnableUiccApplicationsSupported();

    RFX_UNUSED(msg);
    prop.append(String8::format("%d", (m_slot_id + 1)));

    atLine->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    state = atLine->atTokNextint(&err);
    if (err < 0) {
        goto error;
    }

    if (state == 1 && atLine->atTokHasmore()) {
        iccid = atLine->atTokNextstr(&err);
        if (err < 0) {
            goto error;
        }
        // Convert all the lower case characters in iccid to upper case
        stringToUpper(iccid);
        //rfx_property_set(prop.string(), iccid);
        iccidKey.append(String8::format("%s", iccid));
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, iccidKey);
        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_CARD_DETECTED_IND,
                m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
        sendIccidChanged(iccid);
    } else {
        //rfx_property_set(prop.string(), "N/A");
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8("N/A"));
        sendIccidChanged("N/A");
    }

    if ((!aospEnabled && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_OFF)) || (aospEnabled && (getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            == UICC_APPLICATIONS_ENABLE_STATE_DISABLED))) {
        sendSimStatusChanged();
    }
    // Generate CDMA card type.
    handleCdmaCardType(iccid);
    return;
error:
    logE(mTag, "handleIccidIndication, Invalid parameters");
}


void RmcCommSimUrcHandler::handleTestSimDetected(RfxAtLine *urc) {
    int err = 0;
    RfxAtLine *atLine = urc;
    int isTestSim = 0;
    String8 testSimProp("vendor.gsm.sim.ril.testsim");
    String8 testSimStr("");

    testSimProp.append((m_slot_id == 0) ? "" : String8::format(".%d", (m_slot_id + 1)));

    atLine->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    isTestSim = atLine->atTokNextint(&err);
    if (err < 0) {
        goto error;
    }

    testSimStr.append(String8::format("%d", isTestSim));
    rfx_property_set(testSimProp.string(), testSimStr.string());
    logD(mTag, "handleTestSimDetected: %d", isTestSim);

    return;
error:
    logE(mTag, "handleTestSimDetected, Invalid parameters");
}

//MTK-START:  SIM SLOT LOCK
void RmcCommSimUrcHandler::handleSimSlotLockStatusChanged(RfxAtLine *urc) {
    int err = 0;
    int rule = 0;
    int sub_rule = 0;
    int lock_state = 0;
    int service = 0;
    int valid_card = 0;
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
    RfxAtLine *atLine = urc;
    sp<RfxMclMessage> response;

    if (!getNonSlotMclStatusManager()->getBoolValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_MD_SUPPORTED, false)) {
        getNonSlotMclStatusManager()->setBoolValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_MD_SUPPORTED, true, false);
    }

    if (!isSimSlotLockSupport()) {
        logE(mTag, "handleSimSlotLockStatusChanged: not support SML so return! slot %d", m_slot_id);
        return;
    }

    //+ESMLSTATUS: <rule>, <subRule>, <lockState>, <sercice>,<validCard>
    atLine->atTokStart(&err);
    if(err < 0) goto error;

    rule = atLine->atTokNextint(&err);
    if(err < 0) goto error;

    sub_rule = atLine->atTokNextint(&err);
    if(err < 0) goto error;

    lock_state = atLine->atTokNextint(&err);
    if(err < 0) goto error;

    service = atLine->atTokNextint(&err);
    if(err < 0) goto error;

    valid_card = atLine->atTokNextint(&err);
    if(err < 0) goto error;

    logD(mTag, "handleSimSlotLockStatusChanged: rule: %d, sub_rule: %d, lock_state: %d, \
            service: %d, valid_card: %d, slot %d", rule, sub_rule, lock_state,
            service, valid_card, m_slot_id);

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
error:
    logE(mTag, "handleSimSlotLockStatusChanged Error: %d, slot %d", err, m_slot_id);
}

void RmcCommSimUrcHandler::onSmlConfigureCheck() {
    bool smlConfigureChecked = getNonSlotMclStatusManager()->getBoolValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_CONFIG_CHECKED, false);
    if (!smlConfigureChecked) {
        getNonSlotMclStatusManager()->setBoolValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_CONFIG_CHECKED, true, false);
        if (getNonSlotMclStatusManager()->getBoolValue(
                RFX_STATUS_KEY_SML_SLOT_LOCK_MD_SUPPORTED, false) ) {
            if (!isSimSlotLockSupport()) {
                logE(mTag, "MD supports SML but AP doesn't support, please configure rightly!");
                // assert(0); // customer enable it to avoid maliciously falsify, like change image
            }
        } else {
            if (isSimSlotLockSupport()) {
                logE(mTag, "MD doesn't support SML but AP supports, please configure rightly!");
                // assert(0); // customer enable it to avoid maliciously falsify, like change image
            }
        }
    }
}
// MTK-END

void RmcCommSimUrcHandler::handleSimSlotStatusChanged(RfxAtLine *urc) {
    RfxAtLine *line = urc;
    char* ss = urc->getLine();
    sp<RfxMclMessage> unsol;
    // RIL_SimSlotStatus *p_slot_status = NULL;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    int slotNum = 0;
    int simCount = 0;
    int decodeerr = -1;

    pp_slot_status = decodeEslotsinfo(line, &decodeerr, &slotNum);
    logD(mTag, "handleSimSlotStatusChanged");

    if ((decodeerr != RIL_E_SUCCESS) || (pp_slot_status == NULL)) {
        goto error;
    }

    simCount = (slotNum > RfxRilUtils::rfxGetSimCount()) ? RfxRilUtils::rfxGetSimCount() : slotNum;

    for (int i = 0; i < simCount; i++) {
        String8 iccid = getMclStatusManager(pp_slot_status[i]->logicalSlotId)->getString8Value(
                RFX_STATUS_KEY_SIM_ICCID);

        logD(mTag, "handleSimSlotStatusChanged slotId:%d, iccid:%.9s, state:%d, comp:%d",
                pp_slot_status[i]->logicalSlotId, iccid.string(), pp_slot_status[i]->card_state,
                strcmp(iccid.string(), "N/A"));

        if ((pp_slot_status[i]->card_state == RIL_CARDSTATE_ABSENT) &&
                (strcmp(iccid.string(), "N/A") != 0)) {
            decodeerr = RIL_E_INVALID_STATE;
            goto error;
        }
    }

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ICC_SLOT_STATUS, m_slot_id,
            RfxSimSlotStatusData(pp_slot_status,  sizeof(RIL_SimSlotStatus *) * slotNum));
    responseToTelCore(unsol);

    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]);
    }
    free(pp_slot_status);
    return;

error:
    if (decodeerr == RIL_E_INVALID_STATE) {
        // Retry to check slot status.
        logE(mTag, "handleSimSlotStatusChanged retry to get slot status!");
        sendEvent(RFX_MSG_EVENT_GET_SLOT_STATUS, RfxStringData(ss), RIL_CMD_PROXY_7, m_slot_id,
                -1, -1, 0.5*1000000000);
    }

    logE(mTag, "handleSimSlotStatusChanged error!, pp_slot_status = %p", pp_slot_status);
    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]);
        free(pp_slot_status);
    }
}

bool RmcCommSimUrcHandler::isEnableUiccApplicationsSupported() {
    if ((getMclStatusManager()->getIntValue(RFX_STATUS_KEY_AP_SIM_SWITCH_MODE)
            == SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE)
            && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MD_SIM_SWITCH_MODE)
            == SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE)) {
        return true;
    }
    return false;
}

void RmcCommSimUrcHandler::handleUiccApplicationsEnablementChanged(
        const sp<RfxMclMessage>& msg, char* urc) {
    RFX_UNUSED(msg);

    sp<RfxMclMessage> unsol = NULL;

    // +ESIMS: 0,28: Uicc Applications Disabled
    // +ESIMS: 1,29: Uicc Applications Enabled
    if ((0 == strcmp(urc, "+ESIMS: 0,28")) || (0 == strcmp(urc, "+ESIMS: 1,29"))) {
        int enable = -1;
        int state = -1;
        if (strStartsWith(urc, "+ESIMS: 0,28")) {
            enable = 0;
            state = UICC_APPLICATIONS_ENABLE_STATE_DISABLED;
        } else {
            enable = 1;
            state = UICC_APPLICATIONS_ENABLE_STATE_ENABLED;
        }

        getMclStatusManager()->setIntValue(
                RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE, state);
        String8 simOnOff(PROPERTY_UICC_APPLICATIONS_ENABLE_STATE);
        simOnOff.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
        rfx_property_set(simOnOff.string(), String8::format("%d", state).string());

        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_UICC_APPLICATIONS_ENABLEMENT_CHANGED,
                m_slot_id, RfxIntsData(&enable, 1));
        responseToTelCore(unsol);

        sendSimStatusChanged();
    }
}

void RmcCommSimUrcHandler::handleRsuEvent(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
    int err = 0;
    int opId = 0;
    int eventId = -1;
    char* eventString = NULL;
    char* event[2] = {NULL, NULL};
    RfxAtLine *pLine = urc;

    RFX_UNUSED(msg);

    if (pLine == NULL) {
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    opId = pLine->atTokNextint(&err);
    if (err < 0) {
        goto error;
    }

    if (pLine->atTokHasmore()) {
        eventId = pLine->atTokNextint(&err);
        if (err < 0) {
            goto error;
        }
        eventId = (opId << 16) | eventId;
        asprintf(&event[0], "%d", eventId);
        if (pLine->atTokHasmore()) {
            eventString = pLine->atTokNextstr(&err);
        }
        asprintf(&event[1], "%s", (eventString == NULL ? "" : eventString));
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

error:
    logE(mTag, "handleRsuEvent: error");
}

/**
 * +ETMOEVT: <event_id>, [data]
 *
 *  event_id:
 *       0 -- need reboot
 *       2 -- lock state change
 *          data -- lock state
 *                 0 -- locked
 *                 1 -- temporary unlocked
 *                 2 -- permanently unlocked
 */
void RmcCommSimUrcHandler::handleTmoRsuEvent(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
    const int OPERATOR_TMO = 3;

    int err = 0;
    int eventId = -1;
    char* event[2] = {NULL, NULL};
    RfxAtLine *pLine = urc;
    sp<RfxMclMessage> p_urc = NULL;

    RFX_UNUSED(msg);

    logD(mTag, "handleTmoRsuEvent");
    if (pLine == NULL) {
        logE(mTag, "handleTmoRsuEvent: pLine is NULL");
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        logE(mTag, "handleTmoRsuEvent: atTokStart error");
        goto error;
    }

    eventId = pLine->atTokNextint(&err);
    if (err < 0) {
        logE(mTag, "handleTmoRsuEvent: get eventId error");
        goto error;
    }

    eventId = eventId & 0xFF;
    if (eventId == 2) {
        int lockState = pLine->atTokNextint(&err);
        if (err < 0) {
            logE(mTag, "handleTmoRsuEvent: get lock state error");
            goto error;
        }
        logD(mTag, "handleTmoRsuEvent: lockState: %d", lockState);
        eventId = ((lockState & 0xFF) << 8) | eventId;
    }

    eventId = (OPERATOR_TMO << 16) | eventId;
    asprintf(&event[0], "%d", eventId);
    asprintf(&event[1], "%s", "");
    logD(mTag, "handleTmoRsuEvent: 0x%X", eventId);
    p_urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SML_RSU_EVENT,
            m_slot_id, RfxStringsData(event, 2));
    responseToTelCore(p_urc);
    if (event[0] != NULL) {
        free(event[0]);
    }
    if (event[1] != NULL) {
        free(event[1]);
    }
    return;

error:
    logE(mTag, "handleTmoRsuEvent: error");
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
void RmcCommSimUrcHandler::handleSubsidyLockEvent(RfxAtLine *urc) {
    int err = 0;
    int eventId = -1, eventValue = -1;
    const int RSU_OP_JIO_LEGACY = 103;
    RfxAtLine *pLine = urc;
    char* event[2] = {NULL, NULL};
    eventId = eventId & 0xFF;
    int opEventId = (RSU_OP_JIO_LEGACY << 16) | eventId;
    sp<RfxMclMessage> p_urc = NULL;

    logD(mTag, "handleSubsidyLockEvent");
    if (pLine == NULL) {
        logE(mTag, "handleSubsidyLockEvent: pLine is NULL");
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        logE(mTag, "handleSubsidyLockEvent: atTokStart error");
        goto error;
    }

    eventId = pLine->atTokNextint(&err);
    if (err < 0) {
        logE(mTag, "handleSubsidyLockEvent: get eventId error");
        goto error;
    }

    if (pLine->atTokHasmore()) {
        eventValue = pLine->atTokNextint(&err);
        if (err < 0) {
            logE(mTag, "handleSubsidyLockEvent: get eventValue error");
            goto error;
        }
    }

    if (eventId == 0 && eventValue == 0) {   //reset sim
        logD(mTag, "handleSubsidyLockEvent: unlock time expires!");
        int simCount = RfxRilUtils::rfxGetSimCount();
        for (int i=0; i<simCount; i++) {
            logD(mTag, "handleSubsidyLockEvent: sendEvent to %d", i);
            sendEvent(RFX_MSG_EVENT_SIM_RESET, RfxVoidData(), RIL_CMD_PROXY_7, i);
        }
    } else if (eventId == 0 && eventValue == 1) {
        logD(mTag, "handleSubsidyLockEvent: Slot status change!");
    } else {
        logE(mTag, "handleSubsidyLockEvent: Unknow URC!");
    }

    //Notify the event to upper layer avoiding modifying vendor for new event.
    asprintf(&event[0], "%d", opEventId);
    asprintf(&event[1], "%d", eventValue);

    logD(mTag, "handleSubsidyLockEvent: opEventId: 0x%X, eventString: %s",
            opEventId, event[1]);
    p_urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_SML_RSU_EVENT,
            m_slot_id, RfxStringsData(event, 2));
    responseToTelCore(p_urc);
    if (event[0] != NULL) {
        free(event[0]);
    }
    if (event[1] != NULL) {
        free(event[1]);
    }

error:
    logE(mTag, "handleSubsidyLockEvent: error");
}

void RmcCommSimUrcHandler::handleAttRsuSimLockEvent(const sp<RfxMclMessage>& msg, char *urc) {
    int eventId = -1;

    RFX_UNUSED(msg);

    if (strStartsWith(urc, "+EATTEVT: 0")) {  // +EATTEVT
        eventId = 0;
    } else {
        logD(mTag, "[RSU-SIMLOCK] handleAttRsuSimLockEvent: unsupport type");
    }

    logD(mTag, "[RSU-SIMLOCK] handleAttRsuSimLockEvent: eventId = %d", eventId);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ATT_SIM_LOCK_NOTIFICATION,
                    m_slot_id, RfxIntsData(&eventId, 1));
    responseToTelCore(unsol);
}
