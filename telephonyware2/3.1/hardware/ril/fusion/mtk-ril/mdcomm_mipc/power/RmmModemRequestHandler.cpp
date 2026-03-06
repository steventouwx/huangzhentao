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

#include "RmmModemRequestHandler.h"
#include "RfxVoidData.h"

#define RFX_LOG_TAG "RmmModemRequestHandler"

#define WWAM_PWR_IOC_CHANNEL "/dev/wwan_pwrctl"
/* Power control command magic number */
#define PWRCTL_CMD_MAGIC 'G'
/* Power control command through ioctl of /dev/wwan_pwrctl */
#define PWRCTL_CMD_POWER_ON  _IOW(PWRCTL_CMD_MAGIC, 0, int)
#define PWRCTL_CMD_POWER_OFF _IOW(PWRCTL_CMD_MAGIC, 1, int)
#define PWRCTL_CMD_COLD_RESET _IOW(PWRCTL_CMD_MAGIC, 3, int)

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmModemRequestHandler, RIL_CMD_PROXY_9);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData,
        RFX_MSG_REQUEST_MODEM_POWERON);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData,
        RFX_MSG_REQUEST_MODEM_POWEROFF);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData,
        RFX_MSG_REQUEST_RESET_RADIO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData,
        RFX_MSG_REQUEST_RESTART_RILD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData,
        RFX_MSG_REQUEST_SHUTDOWN);

RmmModemRequestHandler::RmmModemRequestHandler(int slotId, int channelId) :
        RfxBaseHandler (slotId, channelId) {
    logD(RFX_LOG_TAG, "RmmModemRequestHandler constructor");
    const int request[] = {
        RFX_MSG_REQUEST_MODEM_POWERON,
        RFX_MSG_REQUEST_MODEM_POWEROFF,
        RFX_MSG_REQUEST_RESET_RADIO,
        RFX_MSG_REQUEST_RESTART_RILD,
        RFX_MSG_REQUEST_SHUTDOWN,
    };
    registerToHandleRequest(request, sizeof(request)/sizeof(int));
}

RmmModemRequestHandler::~RmmModemRequestHandler() {
}

void RmmModemRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch(id) {
        case RFX_MSG_REQUEST_MODEM_POWERON:
            requestModemPowerOn(msg);
            break;
        case RFX_MSG_REQUEST_MODEM_POWEROFF:
        case RFX_MSG_REQUEST_SHUTDOWN:
            requestModemPowerOff(msg);
            break;
        case RFX_MSG_REQUEST_RESET_RADIO:
        case RFX_MSG_REQUEST_RESTART_RILD:
            requestResetRadio(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmModemRequestHandler::requestModemPowerOn(const sp<RfxMclMessage> &msg) {
    logD(RFX_LOG_TAG, "%s", __FUNCTION__);
    rfx_property_set("ril.getccci.response","0");

    bool mdOn = !getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    if (mdOn) {
        logD(RFX_LOG_TAG, "requestModemPowerOn but already modem powered on");
        sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);

    logD(RFX_LOG_TAG, "SET vendor.ril.ipo.radiooff to 0");
    rfx_property_set("vendor.ril.ipo.radiooff","0");

    logD(RFX_LOG_TAG, "Flight mode power on modem, trigger CCCI power on modem (new versio)");
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
    RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_LEAVE_DEEP_FLIGHT_ENHANCED);
#else
    wwan_pcie_pwrctl(PWRCTL_CMD_POWER_ON);
#endif

    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
    rfx_property_set("ril.getccci.response","1");
}

void RmmModemRequestHandler::requestModemPowerOff(const sp<RfxMclMessage> &msg) {
        int mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF,
                false);
        logD(RFX_LOG_TAG, "%s(): mdOff:%d, rid:%d.", __FUNCTION__, mdOff, m_slot_id);

        if (mdOff) {
            logD(RFX_LOG_TAG, "Flight mode power off modem but already modem powered off");
            sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                    RfxVoidData(), msg);
            responseToTelCore(responseMsg);
            return;
        }
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
        RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_RILD_POWER_OFF_MD);
#endif
        sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(
                MIPC_NW_SET_RADIO_STATE_REQ, msg->getSlotId());
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE, 0);
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_HW_STATE, 0);
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_CAUSE, 0x80);
        uint32_t timeout = 10 * 60 * 1000;
        callToMipcMsgSync(mipcMsg, timeout);

        // atSendCommand("AT+EFUN=0");
        // Normal AT may be blocked if s_md_off is 1
        getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, true);

        logD(RFX_LOG_TAG, "ENTER requestRadio PowerOff, set vendor.ril.ipo.radiooff to -1");
        rfx_property_set("vendor.ril.ipo.radiooff","-1");

        // atSendCommand("AT+ECUSD=2,2");
        // atSendCommand("AT+EMDT=0");
        // atSendCommand("AT+EPOF");
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
        RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_ENTER_DEEP_FLIGHT_ENHANCED);
#else
        wwan_pcie_pwrctl(PWRCTL_CMD_POWER_OFF);
#endif
        logD(RFX_LOG_TAG, "requestRadioPowerOff SET vendor.ril.ipo.radiooff to 1");
        rfx_property_set("vendor.ril.ipo.radiooff","1");

        for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++){
            getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
        }
        logD(RFX_LOG_TAG, "Flight mode power off modem, trigger CCCI level 2 power off");

        sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
                RfxVoidData(), msg);
        responseToTelCore(responseMsg);

        /*if (queryBtSapStatus(rid) != BT_SAP_INIT) {
            LOGD("Flight mode power off modem, during SAP connection => disconnect SAP connection");
            disconnectSapConnection(rid);
        }*/
}

void RmmModemRequestHandler::requestResetRadio(const sp<RfxMclMessage> &msg) {
    logD(RFX_LOG_TAG, "start to reset radio - requestResetRadio");
    int mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);

    // only do power off when it is on
    if (!mdOff) {
        getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, true);
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
        RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_RILD_POWER_OFF_MD);
#endif
        sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(
                MIPC_NW_SET_RADIO_STATE_REQ, msg->getSlotId());
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE, 0);
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_HW_STATE, 0);
        mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_CAUSE, 0x80);
        uint32_t timeout = 10 * 60 * 1000;
        callToMipcMsgSync(mipcMsg, timeout);

        //atSendCommand("AT+EMDT=0");
        //atSendCommand("AT+EFUN=0");
        logD(RFX_LOG_TAG, "%s(): set radio off before power off MD.", __FUNCTION__);
        //atSendCommand("AT+EPOF");

        // power off modem
        logD(RFX_LOG_TAG, "requestRadioPowerOff SET vendor.ril.ipo.radiooff to 1");
        rfx_property_set("vendor.ril.ipo.radiooff","1");
        for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++){
            getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
        }
        logD(RFX_LOG_TAG, "Flight mode power off modem, trigger CCCI level 2 power off");
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
        RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_ENTER_DEEP_FLIGHT_ENHANCED);
#else
        wwan_pcie_pwrctl(PWRCTL_CMD_COLD_RESET);
#endif
    }

    // power on modem
    logD(RFX_LOG_TAG, "SET vendor.ril.ipo.radiooff to 0");
    rfx_property_set("vendor.ril.ipo.radiooff","0");
    logD(RFX_LOG_TAG, "Flight mode power on modem, trigger CCCI power on modem (new versio)");
#if !defined(MTK_RIL_THIN_MD_SUPPORT)
    RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_LEAVE_DEEP_FLIGHT_ENHANCED);
#else
    if (mdOff) {
        logD(RFX_LOG_TAG, "MD is off, power on modem");
        wwan_pcie_pwrctl(PWRCTL_CMD_POWER_ON);
    }
#endif
    sp<RfxMclMessage> responseMsg = RfxMclMessage::obtainResponse(RIL_E_SUCCESS,
            RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}

int RmmModemRequestHandler::wwan_pcie_pwrctl(int request) {
    int ccci_sys_fd = -1;
    int reset_parm = -1;
    logD(RFX_LOG_TAG, "wwan_pcie_pwrctl %d", request);
    ccci_sys_fd = open(WWAM_PWR_IOC_CHANNEL, O_RDWR | O_NONBLOCK);
    if (ccci_sys_fd < 0) {
        logE(RFX_LOG_TAG, "open %s, errno: %d", WWAM_PWR_IOC_CHANNEL, errno);
        return -1;
    }

    if (ioctl(ccci_sys_fd, request, &reset_parm) == 0) {
        close(ccci_sys_fd);
        return 0;
    } else {
        logD(RFX_LOG_TAG, "ioctl %d, errno: %d", request, errno);
        close(ccci_sys_fd);
        return -1;
    }
}
