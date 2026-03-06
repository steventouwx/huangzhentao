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
#include "RfxMipcData.h"
#include "RfxStatusDefs.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RmmCommSimDefs.h"
#include "RmmSimBaseHandler.h"
#include "RmmSimCommUrcHandler.h"
#include <telephony/mtk_ril.h>
#include "utils/String8.h"
#include "rfx_properties.h"
#include "RfxMessageId.h"
#include "RfxMisc.h"
#include "RfxRilUtils.h"
#include "RmmCapabilitySwitchUtil.h"
#include "RmmCapabilitySwitchRequestHandler.h"
#include "RfxSimSlotStatusData.h"
#include "vsim/RmmVsimRequestHandler.h"
#include <string>

#ifdef __cplusplus
extern "C"
{
#endif
#include <hardware_legacy/mtk_power.h>
#include "usim_fcp_parser.h"
#ifdef __cplusplus
}
#endif


using ::android::String8;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmSimCommUrcHandler, RIL_CMD_PROXY_URC);

static const int commUrcList[] = {
    MIPC_SIM_STATUS_CHANGED_IND,
    MIPC_SIM_CARD_TYPE_IND, //"+EUSIM:",
    //"+ECPIN:",
    MIPC_SIM_ICCID_IND,    //"+ICCID"
    //MIPC_SIM_STATUS_IND,   //"+ESIMS:"
    MIPC_SIM_EUICC_SLOTS_STATUS_IND,     //"+ESLOTSINFO"
    MIPC_SIM_MEP_SLOTS_INFO_IND,
    MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND, //+ESIMS:
    MIPC_SIM_SIMIND_IND,         //"+ESIMIND:",
    MIPC_SIM_SIMAPP_IND, //"+ESIMAPP:",
    MIPC_SIM_TEST_SIM_IND, //"+ETESTSIM:",
    MIPC_SIM_CT3G_IND, //"+ECT3G:",
    MIPC_SIM_CSIM_IMSI_CHANGE_IND, //"+ECSIMP:",
};

// Register unsolicited message
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_UICC_APPLICATIONS_ENABLEMENT_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_UICC_SUBSCRIPTION_STATUS_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_IMEI_LOCK);

int RmmSimCommUrcHandler::mTrayPluginCount = 0;



/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmmSimCommUrcHandler::RmmSimCommUrcHandler(int slot_id, int channel_id) :
        RmmSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmmSimCommUrc"));
    logD(mTag, "RmmSimCommUrcHandler init");

    int record_num = sizeof(commUrcList)/sizeof(int);;
    registerToHandleMipcURC(commUrcList, record_num);
}

RmmSimCommUrcHandler::~RmmSimCommUrcHandler() {
}

void RmmSimCommUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    //char* ss = urc->getLine();
    RfxMipcData* mipc_urc_data = msg->getMipcData();
    if (mipc_urc_data == NULL) {
        logE(mTag, "onHandleUrc: Error, mipc message data is null");
        return;
    }

    uint16_t mipcMsgId = mipc_urc_data->getMipcMsgId();
    logD(mTag, "onHandleUrc: mipc messageId:%04x", mipcMsgId);

    if (mipcMsgId == MIPC_SIM_STATUS_CHANGED_IND) {
        handleSimStatusChanged();
    } else if(mipcMsgId == MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND) { //"+ESIMS:"
        handleSimStateChanged(msg);
    } else if(mipcMsgId == MIPC_SIM_EUICC_SLOTS_STATUS_IND) { //"+ESLOTSINFO"
        if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MEP_VERSION, -1) <= 0) {
            handleSimSlotStatusChanged(msg);
        }
    } else if(mipcMsgId == MIPC_SIM_MEP_SLOTS_INFO_IND) {
        if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MEP_VERSION, -1) > 0) {
            handleSimMepSlotStatusChanged(msg);
        }
    } else if(mipcMsgId == MIPC_SIM_ICCID_IND) { //"+ICCID"
        handleIccidIndication(msg);
    } else if(mipcMsgId == MIPC_SIM_CARD_TYPE_IND) { //"+EUSIM:"
         handleCardType(msg);
    } else if(mipcMsgId == MIPC_SIM_SIMAPP_IND) { //"+ESIMAPP:"
        int appType = -1;
        appType = mipc_urc_data->getMipcInt8Val(MIPC_SIM_SIMAPP_IND_T_APP_ID, 0);
        if (appType == UICC_APP_USIM || appType == UICC_APP_SIM) {
            handleGsmMccMnc(msg);
        } else if (appType == UICC_APP_CSIM || appType == UICC_APP_RUIM) {
            handCmdaMccMnc(msg);
        }
    } else if(mipcMsgId == MIPC_SIM_SIMIND_IND) { //"+ESIMIND:"
        handleSimIndication(msg);
        int appType = -1;
        appType = mipc_urc_data->getMipcInt8Val(MIPC_SIM_SIMIND_IND_T_APP_ID, 0);
        if (appType == UICC_APP_USIM || appType == UICC_APP_SIM || appType == UICC_APP_ISIM) {
            handleGsmSimIndication(msg);
        } else if ((appType == UICC_APP_CSIM) || (appType == UICC_APP_RUIM)) {
            handleCdmaSimIndication(msg);
        }
    } else if(mipcMsgId == MIPC_SIM_TEST_SIM_IND) { //"+ETESTSIM:"
        handleTestSimDetected(msg);
    } else if(mipcMsgId == MIPC_SIM_CT3G_IND) { //"+ECT3G:"
        handleCdma3gDualmodeValue(msg);
    } else if(mipcMsgId == MIPC_SIM_CSIM_IMSI_CHANGE_IND) { //"+ECSIMP:"
        handleUiccSubscriptionStatus(msg);
    } else {
        logE(mTag, "onHandleUrc: mipc messageId:%04x no handle", mipcMsgId);
    }

/*
     if (strStartsWith(ss, "+ECPIN:")) { //Vsim used
        handleEcpin(msg, urc);
    }
*/
}

/*
void RmmSimCommUrcHandler::handleEcpin(const sp<RfxMclMessage>& msg, RfxAtLine *urc) {
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
} */

void RmmSimCommUrcHandler::handleSimStatusChanged() {
    logD(mTag, "handleSimStatusChanged, RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
    m_slot_id, RfxVoidData());

    responseToTelCore(unsol);
}

void RmmSimCommUrcHandler::handleCardType(const sp<RfxMclMessage>& msg) {
    int type3gpp = 0, type3gpp2 = 0, typeIsim = 0, typeValue = -1;
    String8 type("");
    String8 fullUiccCardType("vendor.gsm.ril.fulluicctype");
    String8 propUicc("vendor.gsm.ril.uicctype");
    bool aospEnabled = isEnableUiccApplicationsSupported();

    // Check whether the feature configuration of the SML is right
    onSmlConfigureCheck();
    propUicc.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
    fullUiccCardType.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));

    RfxMipcData* urc_data = msg->getMipcData();
    type3gpp = urc_data->getMipcUint8Val(MIPC_SIM_CARD_TYPE_IND_T_USIM_STATE, 2);
    type3gpp2 = urc_data->getMipcUint8Val(MIPC_SIM_CARD_TYPE_IND_T_CSIM_STATE, 2);
    typeIsim = urc_data->getMipcUint8Val(MIPC_SIM_CARD_TYPE_IND_T_ISIM_STATE, 2);

    logI(mTag, "handleCardType, +EUSIM:%d,%d,%d", type3gpp, type3gpp2, typeIsim);

    /* Check GSM card type */
    if (type3gpp == 1) {
        rfx_property_set(propUicc, "USIM");
        type.append("USIM");
        typeValue = RFX_CARD_TYPE_USIM;
    } else if (type3gpp == 0) {
        rfx_property_set(propUicc, "SIM");
        type.append("SIM");
        typeValue = RFX_CARD_TYPE_SIM;
    } else {
        logD(mTag, "handleCardType, The SIM card is neither USIM nor SIM!");
        typeValue = 0;
    }

    /* Check CDMA card type */
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
            logD(mTag, "handleCardType, The SIM card is neither RUIM nor CSIM!");
            break;
    }

    // Check ISIM
    switch (typeIsim) {
        case 1:
            typeValue |= RFX_CARD_TYPE_ISIM;
            break;
        default:
            break;
    }

    if ((type3gpp == 2 && type3gpp2 == 2) || (type3gpp == 2 && type3gpp2 == -1)) {
        logD(mTag, "handleCardType, There is no card type!!!");
        type.append("N/A");
        rfx_property_set(fullUiccCardType, "");
        rfx_property_set(propUicc, "");
        typeValue = 0;
    } else {
        // property_set(propUicc, card_type);
        rfx_property_set(fullUiccCardType, type.string());
    }

    int isSwitching = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_SWITCH_TYPE_STATE, 0);
    if (isSwitching == SIM_TYPE_STATE_PROCESSING) {
        int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, 0);
        if (cardType == -1) {
            sendEvent(RFX_MSG_EVENT_GET_SIM_TYPE_INFO, RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);
        }
    }

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, typeValue);
    if (typeValue >= 0) {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_SIM_TASK_READY, true, true);
    }

    if ((!aospEnabled && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_ON)) || (aospEnabled && (getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            == UICC_APPLICATIONS_ENABLE_STATE_ENABLED))) {
        // To query ICCID
        mtk_acquire_wake_lock("sim_hot_plug");
        sendEvent(RFX_MSG_EVENT_SIM_QUERY_ICCID, RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);
    }

    //CDMA specail
    //int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
    if ((typeValue & (RFX_CARD_TYPE_RUIM | RFX_CARD_TYPE_CSIM)) == 0) {
        resetCDMASimState();
    }
}

void RmmSimCommUrcHandler::handleSimStateChanged(const sp<RfxMclMessage>& msg) {
    bool common_slot_no_changed = false;
    sp<RfxMclMessage> unsol = NULL;

    RfxMipcData* urc_data = msg->getMipcData();
    uint8_t is_sim_present = urc_data->getMipcUint8Val(MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_IS_SIM_INSERTED, 0);
    uint8_t cause = urc_data->getMipcUint8Val(MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_CAUSE, 0);

    // +ESIMS: 0,0: SIM Missing
    // +ESIMS: 0,5: SIM Access Profile On
    // +ESIMS: 0,10: Virtual SIM off
    // +ESIMS: 0,11: SIM plug out
    // +ESIMS: 0,13: Recovery start
    // +ESIMS: 0,15: ATT IMEI Lock
    // +ESIMS: 0,19: SPR IMEI Lock is activated
    // MTK-START: AOSP SIM PLUG IN/OUT
    // +ESIMS: 0,26: SIM plug in, but no init
    // MTK-END
    // +ESIMS: 0,28: SIM off
    // +ESIMS: 1,9: Virtual SIM on
    // +ESIMS: 1,14: Recovery end
    // +ESIMS: 1,12: SIM plug in
    // +ESIMS: 1,29: SIM on

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ESIMS_CAUSE, (int)cause);

    logI(mTag, "handleSimStateChanged, +ESIMS:%d,%d", is_sim_present, cause);

    if (is_sim_present == 0) {
        if (cause == 0 || cause == 10 || cause == 11 || cause == 13 || cause == 15) {
            if (cause == 11 &&
                     RfxRilUtils::rfxGetSimCount() >= 2 &&
                     isCommontSlotSupport() == true) {
                if (!isSimInserted()) {
                    common_slot_no_changed = true;
                }
            }
            if (cause == 10 || cause == 11 || cause == 13) {
                logD(mTag, "handleSimStateChanged, common_slot_no_changed: %d", common_slot_no_changed);

                resetSimPropertyAndStatusKey();

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

                if (common_slot_no_changed == false) {
                    // External SIM [Start]
                    if ((isCommontSlotSupport() == true)
                            && (RmmVsimRequestHandler::getVsimPlugInOutEvent(m_slot_id) ==
                            VSIM_TRIGGER_PLUG_OUT)) {
                        logD(mTag, "vsim trigger plug out on common slot project.");
                        RmmVsimRequestHandler::setVsimPlugInOutEvent(m_slot_id, VSIM_TRIGGER_RESET);
                    } else if ((isCommontSlotSupport() == true) && RfxRilUtils::isVsimEnabled()) {
                        logD(mTag, "handleSimStateChanged, Ignore plug out if vsim enabled on common slot project");
                    } else {
                    // External SIM [End]
                        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_PLUG_OUT,
                                m_slot_id, RfxVoidData());

                        if (isCommontSlotSupport() == true) {
                            // To send the event plug out only when card removed in common slot project
                            if (cause == 11) {
                                responseToTelCore(unsol);
                            }
                        } else {
                            /// M: sim recovery start, don't report sim plug-out. @{
                            if (13 != cause) {
                                responseToTelCore(unsol);
                            }
                            /// @}
                        }
                    }
                    logD(mTag, "handleSimStateChanged, shall has RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
                    //unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                    //        m_slot_id, RfxVoidData());
                    //responseToTelCore(unsol);
                }
            } else if (cause == 15) {
                unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_IMEI_LOCK,
                        m_slot_id, RfxVoidData());
                responseToTelCore(unsol);
            } else {
                logD(mTag, "handleSimStateChanged, shall has RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
                //unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                //        m_slot_id, RfxVoidData());
                //responseToTelCore(unsol);
            }
        } else if (cause == 26 || cause == 5 || cause == 19) {
            //SIM Plug in but no init
            logD(mTag, "handleSimStateChanged, shall has RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
            //unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
            //        m_slot_id, RfxVoidData());
            //responseToTelCore(unsol);
        }
    } else if (is_sim_present == 1) {
        if (cause == 9 || cause == 14 || cause == 12) {
            if (cause == 12) {
                getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_EID, String8(""));
                getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8(""));
                sendIccidChanged("");

                String8 uiccType(PROPERTY_UICC_TYPE);
                uiccType.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
                rfx_property_set(uiccType.string(), "unKnown");
            }
            if (RfxRilUtils::rfxGetSimCount() >= 2) {
                if(isCommontSlotSupport() == true && (cause == 12)) {
                    // In this feature, when we receive "ESIMS: 1, 12", it does not mean SIM card plug,
                    // but means slot plug in. That is, it might be no SIM card in this slot.
                    // Thus, we need to query SIM state when detect SIM missing and update flag at that time.
                    logD(mTag, "handleSimStateChanged, Receive plug in in common slot project so do not set sim inserted status here");

                    int simCount = RfxRilUtils::rfxGetSimCount();
                    logD(mTag, "handleSimStateChanged, mTrayPluginCount: %d (slot %d)",
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

            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
            //RLOGD("detectSim before acquire_wake_lock");
            mtk_acquire_wake_lock("sim_hot_plug");
            //RLOGD("detectSim after acquire_wake_lock");
            sendEvent(RFX_MSG_EVENT_SIM_DETECT_SIM, RfxStringData(), RIL_CMD_PROXY_1, m_slot_id);
        } else if (cause == 2) {
            uint8_t ext_cause =
                urc_data->getMipcUint8Val(MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_ADDITIONAL_CAUSE, 0);
            logD(mTag, "handleSimStateChanged, SIM_REFRESH_DONE, ext_cause:%d", ext_cause);

            if (ext_cause == MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_SIM_RESET) {
                logD(mTag, "handleSimStateChanged, SIM_REFRESH: uicc RESET");
                resetSimPropertyAndStatusKey();
            }

            if (ext_cause == MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_END) {
                //unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                //        m_slot_id, RfxVoidData());
                //responseToTelCore(unsol);
                logD(mTag, "handleSimStateChanged,+ESIMS:1,2 shall has RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
                unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_IMSI_REFRESH_DONE,
                        m_slot_id, RfxVoidData());
                responseToTelCore(unsol);
            }
        } else if (cause == 6) {
            // SIM Access Profile Off case
            logD(mTag, "handleSimStateChanged, +ESIMS:1,6, shall has RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
            //unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
            //        m_slot_id, RfxVoidData());
            //responseToTelCore(unsol);
        }
    }

    if ((is_sim_present == 0 && cause == 28) || (is_sim_present == 1 && cause == 29)) {
        if (isEnableUiccApplicationsSupported()) {
            handleUiccApplicationsEnablementChanged(cause);
            return;
        }
        if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_SIMONOFF_SUPPORT)) {
            int state = -1;
            if (cause == 28) {
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
            logD(mTag, "handleSimStateChanged, state: %d, property: %d (slot %d)", state, atoi(proState), m_slot_id);

            sendSimStatusChanged();
        }
    }
    // TODO: is needed to plug in this data function?
    //onSimInsertChangedForData(rid, s);

    handleCdmaSimStateChanged(is_sim_present, cause);
}

void RmmSimCommUrcHandler::handleSimIndication(const sp<RfxMclMessage>& msg) {
    uint8_t indEvent = -1;
    int applist = 0;

    RfxMipcData* mipc_urc_data = msg->getMipcData();
    indEvent = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_EVENT, 0);

    switch (indEvent) {
        case 1:
            // Currently capability switch without RADIO_UNAVAILABLE, but modem SIM task
            // still has to do SIM initialization again.
            // After modem SIM task initialize SIM done, AP will get the URC "+ESIMINIT".
            // We have to notify the capability switch module the event and the proxy channels
            // will unlock
            logD(mTag, "handleSimIndication, +ESIMIND:1,...");
            if (RmmCapabilitySwitchUtil::isDssNoResetSupport()) {
                RmmCapabilitySwitchRequestHandler::notifySIMInitDone(m_slot_id);
            }
            break;
        case 2:
            // +ESIMIND: 2, <uicc_app_list>
            // uicc_app_list = is_csim_exist | is_usim_exist | is_isim_exist (currently isim always 0)
            // is_usim_exist:2 is_csim_exist:4 (is_csim_exist | is_usim_exist): 6
            // For icc card uicc_app_list:0
            applist = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_APP_ID, 0);
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_ESIMIND_APPLIST, applist);
            logD(mTag, "handleSimIndication, +ESIMIND:2,%d", applist);
            break;
        default:
            break;
    }
}

void RmmSimCommUrcHandler::handleIccidIndication(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> unsol = NULL;
    bool aospEnabled = isEnableUiccApplicationsSupported();
    String8 iccidKey("");
    uint16_t iccid_len = 0;
    char* p_temp_iccid = NULL;
    char *p_iccid = NULL;

    RfxMipcData* mipc_data = msg->getMipcData();
    if (mipc_data != NULL) {
        p_temp_iccid = (char*)(mipc_data->getMipcVal(MIPC_SIM_ICCID_IND_T_ICCID, &iccid_len));
    }

    //if (state == 1 && atLine->atTokHasmore()) {
        // Convert all the lower case characters in iccid to upper case
        if (p_temp_iccid != NULL && iccid_len > 0) {
            p_iccid = (char *)calloc(1, (sizeof(char)* (iccid_len + 1)));
            RFX_ASSERT(p_iccid != NULL);
            p_iccid[iccid_len] = '\0';
            memcpy(p_iccid, p_temp_iccid, iccid_len);

            stringToUpper(p_iccid);
            iccidKey.append(String8::format("%s", p_iccid));

            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, iccidKey);
            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_CARD_DETECTED_IND,
                    m_slot_id, RfxVoidData());
            responseToTelCore(unsol);
            sendIccidChanged(p_iccid);
        } else {
            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8("N/A"));
            sendIccidChanged("N/A");
        }
    //} else {
    //    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8("N/A"));
    //    sendIccidChanged("N/A");  ?????
    //}

    if ((!aospEnabled && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_OFF)) || (aospEnabled && (getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            == UICC_APPLICATIONS_ENABLE_STATE_DISABLED))) {
        sendSimStatusChanged();
    }

    logI(mTag, "handleIccidIndication, +iccid:len:%d, str:%.6s->%.6s", iccid_len, p_temp_iccid, p_iccid);

    // Generate CDMA card type.
    updateCdmaCardType(p_iccid);
    if (p_iccid != NULL) {
        free(p_iccid);
    }
}


void RmmSimCommUrcHandler::handleTestSimDetected(const sp<RfxMclMessage>& msg) {
    int isTestSim = 0;
    String8 testSimProp("vendor.gsm.sim.ril.testsim");
    String8 testSimStr("");

    testSimProp.append((m_slot_id == 0) ? "" : String8::format(".%d", (m_slot_id + 1)));
    RfxMipcData* mipc_urc_data = msg->getMipcData();

    isTestSim = mipc_urc_data->getMipcUint8Val(MIPC_SIM_TEST_SIM_IND_T_TEST_SIM, 0);

    logI(mTag, "handleTestSimDetected, +ETESTSIM:%d", isTestSim);

    testSimStr.append(String8::format("%d", isTestSim));
    rfx_property_set(testSimProp.string(), testSimStr.string());
}

//MTK-START:  SIM SLOT LOCK
void RmmSimCommUrcHandler::onSmlConfigureCheck() {
    bool smlConfigureChecked = getNonSlotMclStatusManager()->getBoolValue(
        RFX_STATUS_KEY_SML_SLOT_LOCK_CONFIG_CHECKED, false);
    if (!smlConfigureChecked) {
        getNonSlotMclStatusManager()->setBoolValue(
            RFX_STATUS_KEY_SML_SLOT_LOCK_CONFIG_CHECKED, true, false);
        if (getNonSlotMclStatusManager()->getBoolValue(
                RFX_STATUS_KEY_SML_SLOT_LOCK_MD_SUPPORTED, false) ) {
            if (!isSimSlotLockSupport()) {
                logE(mTag, "onSmlConfigureCheck, MD supports SML but AP doesn't support, please configure rightly!");
                // assert(0); // customer enable it to avoid maliciously falsify, like change image
            }
        } else {
            if (isSimSlotLockSupport()) {
                logE(mTag, "onSmlConfigureCheck, MD doesn't support SML but AP supports, please configure rightly!");
                // assert(0); // customer enable it to avoid maliciously falsify, like change image
            }
        }
    }
}
// MTK-END

void RmmSimCommUrcHandler::handleSimMepSlotStatusChanged(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> unsol;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    int slotNum = 0;
    int simCount = 0;
    int decodeerr = -1;

    RfxMipcData* data =  msg->getMipcData();

    pp_slot_status = decodeMepSlotsinfo(NULL, data, &decodeerr, &slotNum);

    if ((decodeerr != RIL_E_SUCCESS) || (pp_slot_status == NULL)) {
        logE(mTag, "handleSimMepSlotStatusChanged, +ESLOTSINFO decodeerr:%d", decodeerr);
        goto error;
    }

    simCount = (slotNum > RfxRilUtils::rfxGetSimCount()) ? RfxRilUtils::rfxGetSimCount() : slotNum;

    for (int i = 0; i < simCount; i++) {
        for (int j = 0; j < pp_slot_status[i]->port_count; j++) {
            int logicalSlotId = pp_slot_status[i]->info[j].logicalSlotId;
            if ((logicalSlotId < 0) || (logicalSlotId >= RfxRilUtils::rfxGetSimCount())) {
                logE(mTag, "handleSimMepSlotStatusChanged, invalid logical slotId:%d, portId:%d",
                        logicalSlotId,
                        j);
                continue;
            }

            String8 iccid = getMclStatusManager(logicalSlotId)
                    ->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

            logD(mTag, "handleSimMepSlotStatusChanged, +ESLOTSINFO:slotId:%d, iccid:%.6s,state:%d, comp:%d",
                    logicalSlotId,
                    iccid.string(),
                    pp_slot_status[i]->card_state,
                    strcmp(iccid.string(), "N/A"));
        }
    }

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ICC_SLOT_STATUS, m_slot_id,
            RfxSimSlotStatusData(pp_slot_status, sizeof(RIL_SimSlotStatus *) * slotNum));
    responseToTelCore(unsol);

    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
    }
    free(pp_slot_status);
    return;

error:
    if (decodeerr == RIL_E_INVALID_STATE) {
        // Retry to check slot status.
        logE(mTag, "handleSimMepSlotStatusChanged, +ESLOTSINFO retry to get slot status!");
        sendEvent(RFX_MSG_EVENT_GET_MEP_SLOT_STATUS,
                RfxStringData(), RIL_CMD_PROXY_7, m_slot_id,
                -1, -1, 0.5*1000000000);
    }

    logE(mTag, "handleSimMepSlotStatusChanged, +ESLOTSINFO error!, pp_slot_status = %p",
            pp_slot_status);
    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
        free(pp_slot_status);
    }
}

void RmmSimCommUrcHandler::handleSimSlotStatusChanged(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> unsol;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    int slotNum = 0;
    int simCount = 0;
    int decodeerr = -1;

    RfxMipcData* data =  msg->getMipcData();

    pp_slot_status = decodeEslotsinfo(NULL, data, &decodeerr, &slotNum);

    if ((decodeerr != RIL_E_SUCCESS) || (pp_slot_status == NULL)) {
        logE(mTag, "handleSimSlotStatusChanged, +ESLOTSINFO decodeerr:%d", decodeerr);
        goto error;
    }

    simCount = (slotNum > RfxRilUtils::rfxGetSimCount()) ? RfxRilUtils::rfxGetSimCount() : slotNum;

    for (int i = 0; i < simCount; i++) {
        String8 iccid = getMclStatusManager(pp_slot_status[i]->logicalSlotId)->getString8Value(
                RFX_STATUS_KEY_SIM_ICCID);

        logD(mTag, "handleSimSlotStatusChanged, +ESLOTSINFO:slotId:%d, iccid:%.6s, state:%d, comp:%d",
                pp_slot_status[i]->logicalSlotId, iccid.string(), pp_slot_status[i]->card_state,
                strcmp(iccid.string(), "N/A"));

        if ((pp_slot_status[i]->card_state == RIL_CARDSTATE_ABSENT) &&
                (strcmp(iccid.string(), "N/A") != 0)) {
            decodeerr = RIL_E_INVALID_STATE;
            goto error;
        }
    }

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ICC_SLOT_STATUS, m_slot_id,
            RfxSimSlotStatusData(pp_slot_status, sizeof(RIL_SimSlotStatus *) * slotNum));
    responseToTelCore(unsol);

    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->iccId);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
    }
    free(pp_slot_status);
    return;

error:
    if (decodeerr == RIL_E_INVALID_STATE) {
        // Retry to check slot status.
        logE(mTag, "handleSimSlotStatusChanged, +ESLOTSINFO retry to get slot status!");
        sendEvent(RFX_MSG_EVENT_GET_SLOT_STATUS,
                RfxStringData(), RIL_CMD_PROXY_7, m_slot_id,
                -1, -1, 0.5*1000000000);
    }

    logE(mTag, "handleSimSlotStatusChanged, +ESLOTSINFO error!, pp_slot_status = %p", pp_slot_status);
    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->iccId);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
        free(pp_slot_status);
    }
}

bool RmmSimCommUrcHandler::isEnableUiccApplicationsSupported() {
    if ((getMclStatusManager()->getIntValue(RFX_STATUS_KEY_AP_SIM_SWITCH_MODE)
            == SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE)
            && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MD_SIM_SWITCH_MODE)
            == SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE)) {
        return true;
    }
    return false;
}

void RmmSimCommUrcHandler::handleUiccApplicationsEnablementChanged(uint8_t cause) {
    sp<RfxMclMessage> unsol = NULL;

    // +ESIMS: 0,28: Uicc Applications Disabled
    // +ESIMS: 1,29: Uicc Applications Enabled
    if (cause == 28 || cause == 29) {
        int enable = -1;
        int state = -1;
        if (cause == 28) {
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

void RmmSimCommUrcHandler::handleGsmMccMnc(const sp<RfxMclMessage>& msg) {
    //+ESIMAPP:<appTypeId>,<channelId>,<pMcc>,<pMnc>
    char *pMcc = NULL, *pMnc = NULL;
    String8 numeric("");
    String8 prop("vendor.gsm.ril.uicc.mccmnc");

    prop.append((m_slot_id == 0)? "" : String8::format(".%d", m_slot_id));

    do {
        //pMcc = atLine->atTokNextstr(&err);
        //pMnc = atLine->atTokNextstr(&err);
        RfxMipcData* mipc_urc_data = msg->getMipcData();
        pMcc = mipc_urc_data->getMipcStr(MIPC_SIM_SIMAPP_IND_T_MCC);
        pMnc = mipc_urc_data->getMipcStr(MIPC_SIM_SIMAPP_IND_T_MNC);

        numeric.append(String8::format("%s%s", pMcc, pMnc));

        logD(mTag, "handleGsmMccMnc,+ESIMAPP:%s,%s", pMcc, pMnc);

        rfx_property_set(prop, numeric.string());

        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_UICC_GSM_NUMERIC, numeric);
    } while(0);

    int eusim = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
    String8 iccid = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

    if ((!RatConfig_isC2kSupported()) && (strStartsWith(iccid, "898601"))
            && ((eusim == RFX_CARD_TYPE_USIM) || (eusim == (
            RFX_CARD_TYPE_USIM | RFX_CARD_TYPE_ISIM)))) {
        int cardType = -1;

        if (!numeric.isEmpty()) {
            if (strStartsWith(numeric.string(), "46011") || strStartsWith(numeric.string(), "20404")) {
                cardType = CT_4G_UICC_CARD;
            } else {
                // cardType = SIM_CARD;
                // already update in updateCdmaCardType ALPS08656734
                return;
            }
        }

        if (cardType != -1) {
            // Set cdma card type.
            String8 cdmaCardType("vendor.ril.cdma.card.type");
            cdmaCardType.append(String8::format(".%d", (m_slot_id + 1)));
            rfx_property_set(cdmaCardType, String8::format("%d", cardType).string());
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE, cardType);
        }

        int aospType = -1;
        if (isAOSPPropSupport()) {
            String8 simType("gsm.sim");
            simType.append(String8::format("%d%s", (m_slot_id + 1), ".type"));

            if (cardType == CT_4G_UICC_CARD) {
                aospType = DUAL_MODE_TELECOM_LTE_CARD;
            } else if (cardType == SIM_CARD) {
                if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE)
                        == RFX_CARD_TYPE_SIM) {
                    aospType = SINGLE_MODE_SIM_CARD;
                } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE)
                        == RFX_CARD_TYPE_USIM) {
                    aospType = SINGLE_MODE_USIM_CARD;
                }
            }

            if (aospType != -1) {
                rfx_property_set(simType, String8::format("%d", aospType).string());
                logD(mTag, "aospType : %d !", aospType);
            }
        }

        logI(mTag, "handleGsmMccMnc, CDMA card type: %d, aospType: %d", cardType, aospType);
    }
}

void RmmSimCommUrcHandler::handleGsmSimIndication(const sp<RfxMclMessage>& msg) {
    int indEvent = -1, appId = -1, len = -1;
    int fileNum = -1;
    String8 key("");
    char* res = NULL;

    //indEvent = atLine->atTokNextint(&err);
    //appId = atLine->atTokNextint(&err);
    RfxMipcData* mipc_urc_data = msg->getMipcData();
    indEvent = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_EVENT, 0);
    appId = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_APP_ID, 0);

    switch (indEvent) {
        case 3:
            {
                logD(mTag, "handleGsmSimIndication, +ESIMIND Event type 3");
                // SPN->IMSI->GID1->PNN 1st item
                //if (appId == MIPC_SIM_APP_TYPE_USIM || appId == MIPC_SIM_APP_TYPE_MFSIM) { ????TODO
                if (appId == UICC_APP_USIM || appId == UICC_APP_SIM) {
                    len = parseSimIndication(MIPC_SIM_SIMIND_IND_T_SPN, RFX_STATUS_KEY_GSM_SPN, mipc_urc_data);
                    if (len <= 0) {
                        logD(mTag, "handleGsmSimIndication, Can't retrieve SPN, len=%d", len);
                    }
                    len = parseSimIndication(MIPC_SIM_SIMIND_IND_T_GID1, RFX_STATUS_KEY_GSM_GID1, mipc_urc_data);
                    if (len <= 0) {
                        logD(mTag, "handleGsmSimIndication, Can't retrieve GID1, len=%d", len);
                    }
                    len = parseSimIndication(MIPC_SIM_SIMIND_IND_T_PNN_FULL_NAME, RFX_STATUS_KEY_GSM_PNN, mipc_urc_data);
                    if (len <= 0) {
                        logD(mTag, "handleGsmSimIndication, Can't retrieve PNN, len=%d", len);
                    }
                    len = parseSimIndication(MIPC_SIM_SIMIND_IND_T_IMSI, RFX_STATUS_KEY_GSM_IMSI, mipc_urc_data);
                    if (len <= 0) {
                        logD(mTag, "handleGsmSimIndication, Can't retrieve IMSI, len=%d", len);
                    }
                } else if (appId == UICC_APP_ISIM) {
                    // Only parse IMPI when appId is UICC_APP_ISIM
                    len = parseSimIndication(MIPC_SIM_SIMIND_IND_T_IMPI, RFX_STATUS_KEY_GSM_IMPI, mipc_urc_data);
                    if (len <= 0) {
                        logD(mTag, "handleGsmSimIndication, Can't retrieve IMPI, len=%d", len);
                    }
                }
            }
            break;
        case 4:
            {
            fileNum = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_FILE_NUM, 0);
            uint16_t len = 0;
            char *p_list = NULL;
            unsigned char * fcpByte = NULL;
            //test makeSimRspFromUsimFcp API code
            /*asprintf(&res, "%s", "62168202412183026F148A01058B036F0607800200108800");
              int fcpLen = hexStringToByteArray((unsigned char*)res, &fcpByte);
              makeSimRspFromUsimFcp((unsigned char **)&fcpByte, strlen(res)/2);
              logI(mTag, "handleGsmSimIndication case 4, testdata, %s", fcpByte);
              free(res);
              res = NULL;
              free(fcpByte);
              fcpByte = NULL;
            */

            if (fileNum <= 0) {
                goto error;
            }
            key.append(String8::format("%d%s%d", appId, ",", fileNum));
            p_list = (char*)mipc_urc_data->getMipcVal(MIPC_SIM_SIMIND_IND_T_FILE_LIST, &len);
            for (int i = 0; i < fileNum ; i++) {
                mipc_file_list_struct4 *file =
                        (mipc_file_list_struct4*) &p_list[i*sizeof(mipc_file_list_struct4)];

                bool isCPHS = false;
                if ((appId == UICC_APP_USIM) && (strcmp(file->path, "7F206F14") == 0)) {
                    key.append(String8::format("%s%s", ",", "7FFF6F14"));
                    isCPHS = true;
                } else if ((appId == UICC_APP_USIM) && (strcmp(file->path, "7F206F18") == 0)) {
                    key.append(String8::format("%s%s", ",", "7FFF6F18"));
                    isCPHS = true;
                } else {
                    key.append(String8::format("%s%s", ",", file->path));
                }

                if (strlen(file->data) > 4) {
                    String8 sw(file->data);
                    key.append(String8::format("%s%s", ",", string(sw.string()).substr(0, 4).c_str()));

                    asprintf(&res, "%s", (file->data + 4));
                    if (appId == UICC_APP_USIM && !isCPHS) {
                        int fcpLen = hexStringToByteArray((unsigned char*)res, &fcpByte);
                        logD(mTag, "handleGsmSimIndication, isCPHS = false, %.4s", res);
                        makeSimRspFromUsimFcp(&fcpByte, fcpLen);
                        key.append(String8::format("%s", fcpByte));
                    } else {
                        key.append(String8::format("%s", res));
                    }
                } else {
                    key.append(String8::format("%s%s", ",", file->data));
                }

                if (res != NULL) {
                    free(res);
                    res = NULL;
                }
                if (fcpByte != NULL) {
                    free(fcpByte);
                    fcpByte = NULL;
                }
            }

            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_CACHE_FCP, key);
            logD(mTag, "handleGsmSimIndication, cache fcp +ESIMIND:4,%s", key.string());
            }
            break;
        case 5:
            {
                uint16_t len = 0;
                char *p_list = NULL;
                fileNum = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_FILE_NUM, 0);
                if (fileNum <= 0) {
                    goto error;
                }
                key.append(String8::format("%d%s%d", appId, ",", fileNum));
                p_list = (char*)mipc_urc_data->getMipcVal(MIPC_SIM_SIMIND_IND_T_FILE_LIST, &len);
                for (int i = 0; i < fileNum; i++) {
                    mipc_file_list_struct4 *file =
                            (mipc_file_list_struct4*) &p_list[i*sizeof(mipc_file_list_struct4)];
                    key.append(String8::format("%s%s", ",", file->path));
                    key.append(String8::format("%s%s", ",", file->data));
                }
                getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_CACHE_BINARY, key);
                logD(mTag, "handleGsmSimIndication, cache binary +ESIMIND:5,%s", key.string());
            }
            break;
        default:
            logD(mTag, "handleGsmSimIndication, +ESIMIND Not support event %d", indEvent);
            break;
    }

    return;

error:
    logE(mTag, "handleSimIndication, +ESIMIND Invalid parameters");
}

int RmmSimCommUrcHandler::parseSimIndication(mipc_sim_simind_ind_tlv_enum tlv_key,
        RfxStatusKeyEnum status_key, RfxMipcData* mipc_data) {


    int len = -1;
    char *raw = (char*)mipc_data->getMipcStr(tlv_key);

    if (raw == NULL || raw[0] == '\0') {
        getMclStatusManager()->setString8Value(status_key, String8(""));
        return len;
    }

    String8 value(raw);
    getMclStatusManager()->setString8Value(status_key, value);
    len = value.length();

    return len;
}

void RmmSimCommUrcHandler::resetCDMASimState() {
    logD(mTag, "resetCDMASimState");
    String8 cdmaMccMnc("vendor.cdma.ril.uicc.mccmnc");
    String8 cdmaSubscriberId("vendor.ril.uim.subscriberid");
    String8 tempString8Value;

    cdmaMccMnc.append((m_slot_id == 0)? "" : String8::format(".%d", m_slot_id));
    cdmaSubscriberId.append(String8::format(".%d", (m_slot_id + 1)));

    tempString8Value = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_UICC_CDMA_NUMERIC);
    if (!tempString8Value.isEmpty()) {
        rfx_property_set(cdmaMccMnc.string(), "");
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_UICC_CDMA_NUMERIC, String8(""));
    }

    tempString8Value = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_C2K_IMSI);
    if (!tempString8Value.isEmpty()) {
        rfx_property_set(cdmaSubscriberId.string(), "");
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_C2K_IMSI, String8(""));
    }

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_UICC_SUB_CHANGED_STATUS, -1) != -1) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_UICC_SUB_CHANGED_STATUS, -1);
    }
}

void RmmSimCommUrcHandler::handleCdmaSimStateChanged(uint8_t is_sim_present, uint8_t cause) {
    // +ESIMS: 0,0: SIM Missing
    // +ESIMS: 0,13: Recovery start
    // +ESIMS: 0,10: Virtual SIM off
    // +ESIMS: 0,11: SIM plug out
    // +ESIMS: 0.15: IMEI Lock
    // +ESIMS: 1,9: Virtual SIM on
    // +ESIMS: 1,14: Recovery end
    // +ESIMS: 1,12: SIM plug in
    String8 cdma3GDualMode("vendor.gsm.ril.ct3g");
    String8 cdmaCardType("vendor.ril.cdma.card.type");

    cdma3GDualMode.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));
    cdmaCardType.append(String8::format(".%d", (m_slot_id + 1)));

    if (is_sim_present == 0 && (cause == 11 || cause == 13)) {
        resetCDMASimState();

        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE, CARD_NOT_INSERTED);
        rfx_property_set(cdmaCardType, String8::format("%d", CARD_NOT_INSERTED).string());

        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD, false);
        rfx_property_set(cdma3GDualMode.string(), "");

        //getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA3G_SWITCH_CARD, -1);
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_LOCKED_CARD, false);
    } else if (is_sim_present == 0 && cause == 16) {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA_LOCKED_CARD, true);
    }
}

void RmmSimCommUrcHandler::handleCdma3gDualmodeValue(const sp<RfxMclMessage>& msg) {
    int value = -1;
    String8 cdma3GDualMode("vendor.gsm.ril.ct3g");
    cdma3GDualMode.append((m_slot_id == 0)? "" : String8::format(".%d", (m_slot_id + 1)));

    RfxMipcData* mipc_urc_data = msg->getMipcData();

    value = mipc_urc_data->getMipcUint8Val(MIPC_SIM_CT3G_IND_T_TYPE, 0);

    logD(mTag, "handleCdma3gDualmodeValue, +ECT3G:%d", value);

    if (value == 1) {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD, true);
        rfx_property_set(cdma3GDualMode, String8::format("%d", value).string());
    } else {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD, false);
        rfx_property_set(cdma3GDualMode, String8::format("%d", 0).string());
    }
}

void RmmSimCommUrcHandler::handleUiccSubscriptionStatus(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipc_urc_data = msg->getMipcData();

    uint8_t value = mipc_urc_data->getMipcUint8Val(MIPC_SIM_CSIM_IMSI_CHANGE_IND_T_STATUS, 255);

    if (value != 255) {
        int activate = (int) value;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_UICC_SUB_CHANGED_STATUS, activate);

        sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_UICC_SUBSCRIPTION_STATUS_CHANGED,
                m_slot_id, RfxIntsData((void*)&activate, sizeof(int)));
        responseToTelCore(unsol);

        logD(mTag, "handleUiccSubscriptionStatus, +ECSIMP:%d", activate);
    } else {
        logE(mTag, "handleUiccSubscriptionStatus +ECSIMP fail");
    }
}

void RmmSimCommUrcHandler::handCmdaMccMnc(const sp<RfxMclMessage>& msg) {
    //+ESIMAPP:<appTypeId>,<channelId>,<pMcc>,<pMnc>

    char *pMcc = NULL, *pMnc = NULL;
    String8 numeric("");
    String8 cdmaMccMnc("vendor.cdma.ril.uicc.mccmnc");
    cdmaMccMnc.append((m_slot_id == 0)? "" : String8::format(".%d", m_slot_id));

    do {
        // TODO: Check apptype if to continue handle imsi.
        RfxMipcData* mipc_urc_data = msg->getMipcData();
        pMcc = mipc_urc_data->getMipcStr(MIPC_SIM_SIMAPP_IND_T_MCC);
        pMnc = mipc_urc_data->getMipcStr(MIPC_SIM_SIMAPP_IND_T_MNC);

        numeric.append(String8::format("%s%s", pMcc, pMnc));

        logD(mTag, "handCmdaMccMnc, +ESIMAPP:%s,%s", pMcc, pMnc);

        rfx_property_set(cdmaMccMnc, numeric.string());
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_UICC_CDMA_NUMERIC, numeric);
    } while (0);

    int cardType = -1;
    int eusim = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
    if (((eusim & RFX_CARD_TYPE_CSIM) != 0) && ((eusim & RFX_CARD_TYPE_USIM) != 0)) {

        if (!numeric.isEmpty()) {
            if ((0 == strcmp(numeric.string(), "46003"))
                    || (0 == strcmp(numeric.string(), "45502"))
                    || (0 == strcmp(numeric.string(), "46012"))) {
                cardType = CT_4G_UICC_CARD;
            } else {
                cardType = NOT_CT_UICC_CARD;
            }
        } else {
            String8 iccid = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);
            if (!iccid.isEmpty()) {
                if (isOp09Card(iccid.string())) {
                    cardType = CT_4G_UICC_CARD;
                } else {
                    cardType = NOT_CT_UICC_CARD;
                }
            }
        }

        if (cardType != -1) {
            String8 cdmaCardType("vendor.ril.cdma.card.type");
            cdmaCardType.append(String8::format(".%d", (m_slot_id + 1)));
            // Set cdma card type.
            rfx_property_set(cdmaCardType, String8::format("%d", cardType).string());
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE, cardType);
        }
    }

    if (isAOSPPropSupport()) {
        String8 simType("gsm.sim");
        simType.append(String8::format("%d%s", (m_slot_id + 1), ".type"));
        int aospType = -1;

        if (cardType == CT_4G_UICC_CARD) {
            aospType = DUAL_MODE_TELECOM_LTE_CARD;
        } else if (cardType == NOT_CT_UICC_CARD) {
            aospType = DUAL_MODE_CG_CARD;
        }

        if (aospType != -1) {
            rfx_property_set(simType, String8::format("%d", aospType).string());
            logD(mTag, "handCmdaMccMnc, aospType : %d !", aospType);
        }
    }
}

void RmmSimCommUrcHandler::handleCdmaSimIndication(const sp<RfxMclMessage>& msg) {
    int indEvent = -1;
    int appId = 0;
    char *rawData = NULL;

    String8 cdmaSubscriberId("vendor.ril.uim.subscriberid");
    cdmaSubscriberId.append(String8::format(".%d", (m_slot_id + 1)));

    RfxMipcData* mipc_urc_data = msg->getMipcData();

    do {
        // +ESIMIND: <event id>, <app id> , <SPN>, <Full IMSI>, <GID1>, <PNN full name TLV>
        // CDMA only reports SPN and IMSI context.
        indEvent = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_EVENT, 0);

        if (indEvent == 3) {
            appId = mipc_urc_data->getMipcUint8Val(MIPC_SIM_SIMIND_IND_T_APP_ID, 0);

            parseSimIndication(MIPC_SIM_SIMIND_IND_T_SPN, RFX_STATUS_KEY_CDMA_SPN, mipc_urc_data);
            parseSimIndication(MIPC_SIM_SIMIND_IND_T_IMSI, RFX_STATUS_KEY_C2K_IMSI, mipc_urc_data);

            rawData = (char*)mipc_urc_data->getMipcStr(MIPC_SIM_SIMIND_IND_T_IMSI);
            if (rawData == NULL || rawData[0] == '\0') { ///??? TODO
                break;
            }
            rfx_property_set(cdmaSubscriberId, rawData);

            if (isAOSPPropSupport()) {
                int aospType = -1;
                int eusim = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
                if (((eusim & RFX_CARD_TYPE_CSIM) != 0) && ((eusim & RFX_CARD_TYPE_USIM) != 0)) {
                    String8 numeric("");
                    if (strlen(rawData) > 4) {
                        numeric = String8(string(rawData).substr(0, 5).c_str());
                    }
                    logD(mTag, "handleCdmaSimIndication, +ESIMIND numeric: %s", numeric.string());
                    if (!numeric.isEmpty()) {
                        if ((0 == strcmp(numeric.string(), "46003"))
                                || (0 == strcmp(numeric.string(), "45502"))
                                || (0 == strcmp(numeric.string(), "46012"))) {
                            aospType = DUAL_MODE_TELECOM_LTE_CARD;
                        } else {
                            aospType = DUAL_MODE_CG_CARD;
                        }
                    } else {
                        String8 iccid = getMclStatusManager()->getString8Value(
                                RFX_STATUS_KEY_SIM_ICCID);
                        if (!iccid.isEmpty()) {
                            if (isOp09Card(iccid.string())) {
                                aospType = DUAL_MODE_TELECOM_LTE_CARD;
                            } else {
                                aospType = DUAL_MODE_CG_CARD;
                            }
                        }
                    }
                }

                if (aospType != -1) {
                    String8 simType("gsm.sim");
                    simType.append(String8::format("%d%s", (m_slot_id + 1), ".type"));
                    rfx_property_set(simType, String8::format("%d", aospType).string());
                    logD(mTag, "handleCdmaSimIndication, +ESIMIND aospType : %d !", aospType);
                }
            }
            return;
        }
    } while (0);

    logE(mTag, "handleCdmaSimIndication, +ESIMIND Invalid parameters");
}


