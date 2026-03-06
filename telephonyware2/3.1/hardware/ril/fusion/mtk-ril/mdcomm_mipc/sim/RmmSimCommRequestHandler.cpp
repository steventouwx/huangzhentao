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
#include "RfxBaseHandler.h"
#include "RfxMclDispatcherThread.h"
#include "RfxMipcData.h"
#include "RfxSimIoRspData.h"
#include "RfxSimIoData.h"
#include "RfxSimAuthData.h"
#include "RfxSimGenAuthData.h"
#include "RfxSimOpenChannelData.h"
#include "RfxSimApduData.h"
#include "RfxSimTypeInfoData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxRawData.h"
#include "RfxSimStatusData.h"
#include "RfxSimSlotStatusData.h"
#include "RfxSimCarrRstData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxViaUtils.h"
#include "RmmCommSimDefs.h"
#include "RmmSimBaseHandler.h"
#include "RmmSimCommRequestHandler.h"
#include <compiler/compiler_utils.h>
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include "RfxChannel.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cstring>
#include <pthread.h>
#include "RfxMessageId.h"
#include <prop/properties.h>
#include "RfxRilUtils.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <hardware_legacy/mtk_power.h> //for wake lock
#include "base64.h"
#include "usim_fcp_parser.h"
#include <platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
#include "pb_decode.h"
#include "pb_encode.h"
#ifdef __cplusplus
}
#endif

// External SIM [Start]
//#include "RfxVsimEventData.h"
//#include "RfxVsimOpEventData.h"
#include "RmmSimCommUrcHandler.h"
#include "vsim/RmmVsimRequestHandler.h"
// External SIM [End]


using ::android::String8;
using namespace std;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmSimCommRequestHandler, RIL_CMD_PROXY_1);
RFX_REGISTER_HANDLER_CLASS(RmmSimCommRequestHandler, RIL_CMD_PROXY_3);
RFX_REGISTER_HANDLER_CLASS(RmmSimCommRequestHandler, RIL_CMD_PROXY_7);

static const int ch1ReqList[] = {
    RFX_MSG_REQUEST_SIM_IO,
    RFX_MSG_REQUEST_ISIM_AUTHENTICATION,
    RFX_MSG_REQUEST_GENERAL_SIM_AUTH,
    RFX_MSG_REQUEST_ENTER_SIM_PIN,
    RFX_MSG_REQUEST_ENTER_SIM_PUK,
    RFX_MSG_REQUEST_ENTER_SIM_PIN2,
    RFX_MSG_REQUEST_ENTER_SIM_PUK2,
    RFX_MSG_REQUEST_CHANGE_SIM_PIN,
    RFX_MSG_REQUEST_CHANGE_SIM_PIN2,
    RFX_MSG_REQUEST_SIM_OPEN_CHANNEL,
    RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL,
    RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL_WITH_SESSION,
    RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_BASIC,
    RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_CHANNEL,
    RFX_MSG_REQUEST_SET_SIM_CARD_POWER,
    RFX_MSG_REQUEST_SIM_GET_ATR,
    RFX_MSG_REQUEST_SIM_GET_ICCID,
    RFX_MSG_REQUEST_ENABLE_UICC_APPLICATIONS,
    RFX_MSG_REQUEST_ARE_UICC_APPLICATIONS_ENABLED,
    RFX_MSG_REQUEST_LOCAL_SIM_SAP_RESET,
    RFX_MSG_REQUEST_SIM_SAP_CONNECT,
    RFX_MSG_REQUEST_SIM_SAP_DISCONNECT,
    RFX_MSG_REQUEST_SIM_SAP_APDU,
    RFX_MSG_REQUEST_SIM_SAP_TRANSFER_ATR,
    RFX_MSG_REQUEST_SIM_SAP_POWER,
    RFX_MSG_REQUEST_SIM_SAP_RESET_SIM,
    RFX_MSG_REQUEST_SIM_SAP_SET_TRANSFER_PROTOCOL,
    RFX_MSG_REQUEST_SIM_SAP_ERROR_RESP,
    RFX_MSG_REQUEST_SET_LOGICAL_TO_PHYSICAL_SLOT_MAPPING,
    RFX_MSG_REQUEST_GET_IMSI,
    RFX_MSG_REQUEST_QUERY_FACILITY_LOCK,
    RFX_MSG_REQUEST_SET_FACILITY_LOCK,
    RFX_MSG_REQUEST_CDMA_SUBSCRIPTION,
    RFX_MSG_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE,
    RFX_MSG_REQUEST_SWITCH_SIM_TYPE,
    RFX_MSG_REQUEST_GET_SIM_TYPE_INFO,
};

static const int ch3ReqList[] = {
    RFX_MSG_REQUEST_SIM_AUTHENTICATION
};

static const int ch1EventList[] = {
    RFX_MSG_EVENT_SIM_QUERY_ICCID,
    RFX_MSG_EVENT_SIM_DETECT_SIM,
    //RFX_MSG_EVENT_SIM_SIM_AUTHENTICATION,
    //RFX_MSG_EVENT_SIM_USIM_AUTHENTICATION,
    RFX_MSG_EVENT_GET_SLOT_STATUS,
    RFX_MSG_EVENT_GET_MEP_SLOT_STATUS,
    RFX_MSG_EVENT_GET_SIM_TYPE_INFO,
    //RFX_MSG_EVENT_SIM_RESET, //sim_me lock used??
};

// Real-time
static const int ch7ReqList[] = {
    RFX_MSG_REQUEST_GET_SIM_STATUS,
    RFX_MSG_REQUEST_SET_SIM_POWER,
    RFX_MSG_REQUEST_GET_SLOT_STATUS,
    RFX_MSG_REQUEST_ACTIVATE_UICC_CARD,
    RFX_MSG_REQUEST_DEACTIVATE_UICC_CARD,
};

int RmmSimCommRequestHandler::mRetryGetSlotsInfoTimes = 0;

// CDMA subscription source
#define SUBSCRIPTION_FROM_RUIM 0
#define SUBSCRIPTION_FROM_NV 1

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxSimStatusData, RFX_MSG_REQUEST_GET_SIM_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimIoData, RfxSimIoRspData, RFX_MSG_REQUEST_SIM_IO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData, RFX_MSG_REQUEST_ISIM_AUTHENTICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimAuthData, RfxSimIoRspData, RFX_MSG_REQUEST_SIM_AUTHENTICATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimGenAuthData, RfxSimIoRspData, RFX_MSG_REQUEST_GENERAL_SIM_AUTH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_ENTER_SIM_PIN);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_ENTER_SIM_PUK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_ENTER_SIM_PIN2);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_ENTER_SIM_PUK2);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_CHANGE_SIM_PIN);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_CHANGE_SIM_PIN2);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimOpenChannelData, RfxIntsData, RFX_MSG_REQUEST_SIM_OPEN_CHANNEL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL_WITH_SESSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_SIM_CARD_POWER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimApduData, RfxSimIoRspData,
        RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_BASIC);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimApduData, RfxSimIoRspData,
        RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_CHANNEL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, RFX_MSG_REQUEST_SIM_GET_ATR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, RFX_MSG_REQUEST_SIM_GET_ICCID);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ENABLE_UICC_APPLICATIONS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_ARE_UICC_APPLICATIONS_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_SIM_POWER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SWITCH_SIM_TYPE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxSimTypeInfoData, RFX_MSG_REQUEST_GET_SIM_TYPE_INFO);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_CONNECT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_DISCONNECT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_APDU);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_TRANSFER_ATR);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_POWER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_RESET_SIM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_SET_TRANSFER_PROTOCOL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxRawData, \
        RFX_MSG_REQUEST_SIM_SAP_ERROR_RESP);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, \
        RFX_MSG_REQUEST_ACTIVATE_UICC_CARD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, \
        RFX_MSG_REQUEST_DEACTIVATE_UICC_CARD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxStringData, RFX_MSG_REQUEST_GET_IMSI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_QUERY_FACILITY_LOCK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_SET_FACILITY_LOCK);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringsData, \
        RFX_MSG_REQUEST_CDMA_SUBSCRIPTION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, \
        RFX_MSG_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SIM_QUERY_ICCID);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_SIM_DETECT_SIM);
//RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_SIM_SIM_AUTHENTICATION);
//RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringsData, RFX_MSG_EVENT_SIM_USIM_AUTHENTICATION);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_GET_SLOT_STATUS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_GET_MEP_SLOT_STATUS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_GET_SIM_TYPE_INFO);

//RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SIM_RESET);

RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_COMMON_SLOT_NO_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_TRAY_PLUG_IN);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_PLUG_OUT);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_PLUG_IN);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_IMSI_REFRESH_DONE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIM_POWER_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_CARD_DETECTED_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_ICCID_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxSimTypeInfoData, RFX_MSG_URC_SIM_TYPE_INFO_IND);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmmSimCommRequestHandler::RmmSimCommRequestHandler(int slot_id, int channel_id) :
        RmmSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmmSimCommReq"));
    int record_num = 0;
    mRetryGetSimStatusTimes = 0;
    mRetrySetFacilityTimes = 0;

    logI(mTag, "reset vendor.gsm.ril.uicc.mccmnc");
    String8 gsmMccMnc("vendor.gsm.ril.uicc.mccmnc");
    gsmMccMnc.append((m_slot_id == 0)? "" : String8::format(".%d", m_slot_id));
    rfx_property_set(gsmMccMnc.string(), "");
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_UICC_GSM_NUMERIC, String8(""));

    logI(mTag, "reset vendor.cdma.ril.uicc.mccmnc");
    String8 cdmaMccMnc("vendor.cdma.ril.uicc.mccmnc");
    cdmaMccMnc.append((m_slot_id == 0)? "" : String8::format(".%d", m_slot_id));
    rfx_property_set(cdmaMccMnc.string(), "");
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_UICC_CDMA_NUMERIC, String8(""));

    if (channel_id == RIL_CMD_PROXY_1) {
        /// M: GSMA TS.27 13.3.7 @{
        if (RfxRilUtils::isUiccCLFSupport()) {
            /* TS26_NFC_REQ_166:
             * Send Terminal Capability command to the UICC indicating that the
             * UICC-CLF interface (SWP) is supported as per ETSI TS 102 221. It
             * should send before sending AT+ESIMS and AT+EFUN.
             */
            //response = atSendCommand("AT+ESBP=5,\"SBP_TERMINAL_CAPABILITY_FLEX\",1");
            char atcmd[] = "AT+ESBP=5,\"SBP_TERMINAL_CAPABILITY_FLEX\",1";
            sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, m_slot_id);
            initMipc->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD, atcmd);
            sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
            logD(mTag, "RmmSimCommRequestHandler, enable clf, result:%d",
                cnfMipc->getResult());
        }
        /// M: GSMA TS.27 13.3.7 @}
        int mainSlotId = RfxRilUtils::getMajorSim() - 1;
        if (mainSlotId == slot_id) {
            //enableEuiccSupport();
            enableSilentReboot();
        }

        // Check if modem supports CDMA 3g dual activation. It is a const value set in modem and is
        // enough to query just once in a channel.
        queryCdma3gDualActivation();

        // Check if SIM on/off feature is supported by modem.
        querySimOnOffActivation();
        // Query whether the SIM card supports "AT+ECGLA"
        //queryEcglaCapability();

        // Query MEP version
        querySimMepSupport();

        querySimTypeInfo();

        record_num = sizeof(ch1ReqList)/sizeof(int);
        registerToHandleRequest(ch1ReqList, record_num);
        record_num = sizeof(ch1EventList)/sizeof(int);
        registerToHandleEvent(ch1EventList, record_num);
    } else if (channel_id == RIL_CMD_PROXY_3) {
        record_num = sizeof(ch3ReqList)/sizeof(int);
        registerToHandleRequest(ch3ReqList, record_num);
    } else if (channel_id == RIL_CMD_PROXY_7) {
        record_num = sizeof(ch7ReqList)/sizeof(int);
        registerToHandleRequest(ch7ReqList, record_num);
    }
}

RmmSimCommRequestHandler::~RmmSimCommRequestHandler() {
}

void RmmSimCommRequestHandler::querySimMepSupport() {
    char mepFeature[] = "MEP";
    int mepSupportVersion = getFeatureVersion(mepFeature);
    logD(mTag, "RmmSimCommRequestHandler, querySimMepSupport, mepSupportVersion:%d",
            mepSupportVersion);
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_MEP_VERSION, mepSupportVersion);
}

void RmmSimCommRequestHandler::querySimTypeInfo() {
    logD(mTag, "RmmSimCommRequestHandler, querySimTypeInfo");
    sendEvent(RFX_MSG_EVENT_GET_SIM_TYPE_INFO, RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);
}

void RmmSimCommRequestHandler::enableSilentReboot() {
    sp<RfxAtResponse> response;
    int muxreport_case = 0;
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    int auto_unlock_pin = -1;
    int isSilentReboot = -1;
    int isDisableEboot = 0;

    rfx_property_get("vendor.ril.disable.eboot", property_value, "0");
    isDisableEboot = atoi(property_value);

    if (isDisableEboot == 1) {
        isSilentReboot = 0;
    } else {
        rfx_property_get("vendor.ril.mux.report.case", property_value, "0");
        muxreport_case = atoi(property_value);
        logD(mTag, "getprop vendor.ril.mux.report.case %d", muxreport_case);
        switch (muxreport_case) {
            case 0:
                isSilentReboot = 0;
                break;
            case 1:
            case 2:
            case 5:
            case 6:
                isSilentReboot = 1;
                break;
        }
    }
    rfx_property_set("vendor.ril.disable.eboot", "0");
    rfx_property_set("vendor.ril.mux.report.case", "0");

    // eboot property will be set to 0 when ipo shutdown, no needs to silent reboot in this case
    // ebbot property will be set to 1 when flight mode turn on, and 3g switch reset modem
    rfx_property_get("vendor.gsm.ril.eboot", property_value, "0");
    auto_unlock_pin = atoi(property_value);
    logD(mTag, "getprop vendor.gsm.ril.eboot %d", auto_unlock_pin);
    isSilentReboot |= auto_unlock_pin;

    if (isSilentReboot != 1) {
        if (isNormalBootUp() == 0) {
            isSilentReboot = 1;
        } else {
            isSilentReboot = 0;
        }
    }

    /********************************
     * AT+EBOOT=<mode>
     *
     * 0: Normal boot up
     * 1: Silent boot up (Verify PIN by modem internally)
     switch (isSilentReboot) {
         case 0:
             response = atSendCommand("AT+EBOOT=0");
             break;
         case 1:
             response = atSendCommand("AT+EBOOT=1");
             break;
         default:
             response = atSendCommand("AT+EBOOT=0");
             break;
     }
     *********************************/
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SYS_SILENT_REBOOT_REQ, m_slot_id);
    if (isSilentReboot == 1) {
        mipc_data->addMipcTlvUint8(MIPC_SYS_SILENT_REBOOT_REQ_T_MODE, 1);
    } else {
        mipc_data->addMipcTlvUint8(MIPC_SYS_SILENT_REBOOT_REQ_T_MODE, 0);
    }
    sp<RfxMipcData> rsp_Data = callToMipcMsgSync(mipc_data);

    logD(mTag, "enableSilentReboot, isSilentReboot:%d, result:%d",
                    isSilentReboot, rsp_Data->getResult());

    rfx_property_set("vendor.gsm.ril.eboot", "0");
}

int RmmSimCommRequestHandler::isNormalBootUp() {
    int ret = 1;
    char boot_reason[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("sys.boot.reason", boot_reason, "0");

    if ((strstr(boot_reason, "watchdog") != NULL) ||
            (strstr(boot_reason, "kernel_panic") != NULL)) {
        ret = 0;
    }
    logI(mTag, "isNormalBootUp = %d, boot_reason = %s", ret, boot_reason);

    return ret;
}

void RmmSimCommRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_GET_SIM_STATUS:
            handleGetSimStatus(msg);
            break;
        case RFX_MSG_REQUEST_SIM_IO:
            handleSimIo(msg);
            break;
        case RFX_MSG_REQUEST_ISIM_AUTHENTICATION:
            handleIsimAuth(msg);
            break;
        case RFX_MSG_REQUEST_SIM_AUTHENTICATION:
            handleSimAuth(msg);
            break;
        case RFX_MSG_REQUEST_GENERAL_SIM_AUTH:
            handleGeneralSimAuth(msg);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PIN:
            handleSecurityOperation(msg,ENTER_PIN1);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PUK:
            handleSecurityOperation(msg,ENTER_PUK1);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PIN2:
            handleSecurityOperation(msg,ENTER_PIN2);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PUK2:
            handleSecurityOperation(msg,ENTER_PUK2);
            break;
        case RFX_MSG_REQUEST_CHANGE_SIM_PIN:
            handleSecurityOperation(msg,CHANGE_PIN1);
            break;
        case RFX_MSG_REQUEST_CHANGE_SIM_PIN2:
            handleSecurityOperation(msg,CHANGE_PIN2);
            break;
        case RFX_MSG_REQUEST_SIM_OPEN_CHANNEL:
            handleSimOpenChannel(msg);
            break;
        case RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL:
        case RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL_WITH_SESSION:
            handleSimCloseChannel(msg);
            break;
        case RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_BASIC:
            handleSimTransmitApdu(msg, true);
            break;
        case RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_CHANNEL:
            handleSimTransmitApdu(msg, false);
            break;
        case RFX_MSG_REQUEST_SET_SIM_CARD_POWER:
            handleSetSimCardPower(msg);
            break;
        case RFX_MSG_REQUEST_SIM_GET_ATR:
            handleSimGetAtr(msg);
            break;
        case RFX_MSG_REQUEST_SIM_GET_ICCID:
            handleSimGetIccid(msg);
            break;
        case RFX_MSG_REQUEST_ENABLE_UICC_APPLICATIONS:
            handleEnableUiccApplications(msg);
            break;
        case RFX_MSG_REQUEST_ARE_UICC_APPLICATIONS_ENABLED:
            handleAreUiccApplicationsEnabled(msg);
            break;
        case RFX_MSG_REQUEST_SET_SIM_POWER:
            handleSetSimPower(msg);
            break;

        case RFX_MSG_REQUEST_LOCAL_SIM_SAP_RESET:
            handleLocalBtSapReset(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_CONNECT:
            handleBtSapConnect(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_DISCONNECT:
            handleBtSapDisconnect(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_APDU:
            handleBtSapTransferApdu(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_TRANSFER_ATR:
            handleBtSapTransferAtr(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_POWER:
            handleBtSapPower(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_RESET_SIM:
            handleBtSapResetSim(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_SET_TRANSFER_PROTOCOL:
            handleBtSapSetTransferProtocol(msg);
            break;
        case RFX_MSG_REQUEST_GET_SLOT_STATUS:
            if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MEP_VERSION, -1) > 0) {
                handleGetMepSlotStatus(msg);
            } else {
                handleGetSlotStatus(msg);
            }
            break;
        case RFX_MSG_REQUEST_SET_LOGICAL_TO_PHYSICAL_SLOT_MAPPING:
            if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MEP_VERSION, -1) > 0) {
                handleSetMepSimSlotsMapping(msg);
            } else {
                handleSetSimSlotsMapping(msg);
            }
            break;
        case RFX_MSG_REQUEST_ACTIVATE_UICC_CARD:
            handleActiveUiccCard(msg);
            break;
        case RFX_MSG_REQUEST_DEACTIVATE_UICC_CARD:
            handleDeactivateUiccCard(msg);
            break;
        case RFX_MSG_REQUEST_GET_IMSI:
            handleGetImsi(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_FACILITY_LOCK:
            handleQueryFacilityLock(msg);
            break;
        case RFX_MSG_REQUEST_SET_FACILITY_LOCK:
            handleSetFacilityLock(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_SUBSCRIPTION:
            queryCdmaMlpl(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE:
            handleCdmaGetSubSource(msg);
            break;
        case RFX_MSG_REQUEST_GET_SIM_TYPE_INFO:
            handleGetSimTypeInfo(msg);
            break;
        case RFX_MSG_REQUEST_SWITCH_SIM_TYPE:
            handleSwitchSimType(msg);
            break;
        default:
            logD(mTag, "onHandleRequest, Not support the request %d!", request);
            break;
    }
}

void RmmSimCommRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_GET_SIM_STATUS:
            handleGetSimStatusResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_IO:
            handleSimIoResponse(msg);
            break;
        case RFX_MSG_REQUEST_ISIM_AUTHENTICATION:
            handleIsimAuthResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_AUTHENTICATION:
            handleSimAuthResponse(msg);
            break;
        case RFX_MSG_REQUEST_GENERAL_SIM_AUTH:
            handleGeneralSimAuthResponse(msg);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PIN:
            handleSecurityOperationResponse(msg,ENTER_PIN1);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PUK:
            handleSecurityOperationResponse(msg,ENTER_PUK1);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PIN2:
            handleSecurityOperationResponse(msg,ENTER_PIN2);
            break;
        case RFX_MSG_REQUEST_ENTER_SIM_PUK2:
            handleSecurityOperationResponse(msg,ENTER_PUK2);
            break;
        case RFX_MSG_REQUEST_CHANGE_SIM_PIN:
            handleSecurityOperationResponse(msg,CHANGE_PIN1);
            break;
        case RFX_MSG_REQUEST_CHANGE_SIM_PIN2:
            handleSecurityOperationResponse(msg,CHANGE_PIN2);
            break;
        case RFX_MSG_REQUEST_SIM_OPEN_CHANNEL:
            handleSimOpenChannelResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL:
        case RFX_MSG_REQUEST_SIM_CLOSE_CHANNEL_WITH_SESSION:
            handleSimCloseChannelResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_BASIC:
            handleSimTransmitApduResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_TRANSMIT_APDU_CHANNEL:
            handleSimTransmitApduResponse(msg);
            break;
        case RFX_MSG_REQUEST_SET_SIM_CARD_POWER:
            handleSetSimCardPowerResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_GET_ATR:
            handleSimGetAtrResponse(msg);
            break;
        //case RFX_MSG_REQUEST_SIM_GET_ICCID:
        //    break;
        case RFX_MSG_REQUEST_ENABLE_UICC_APPLICATIONS:
            handleEnableUiccApplicationsResponse(msg);
            break;
        case RFX_MSG_REQUEST_ARE_UICC_APPLICATIONS_ENABLED:
            handleAreUiccApplicationsEnabledResponse(msg);
            break;
        case RFX_MSG_REQUEST_SET_SIM_POWER:
            handleSetSimPowerResponse(msg);
            break;
        //case RFX_MSG_REQUEST_LOCAL_SIM_SAP_RESET:
        //    break;
        case RFX_MSG_REQUEST_SIM_SAP_CONNECT:
            handleBtSapConnectResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_DISCONNECT:
            handleBtSapDisconnectResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_APDU:
            handleBtSapTransferApduResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_POWER:
            handleBtSapPowerResponse(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SAP_RESET_SIM:
            handleBtSapResetSimResponse(msg);
            break;
        //case RFX_MSG_REQUEST_SIM_SAP_SET_TRANSFER_PROTOCOL:
        //    break;

        case RFX_MSG_REQUEST_GET_SLOT_STATUS:
            if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MEP_VERSION, -1) > 0) {
                handleGetMepSlotStatusResponse(msg);
            } else {
                handleGetSlotStatusResponse(msg);
            }
            break;
        case RFX_MSG_REQUEST_SET_LOGICAL_TO_PHYSICAL_SLOT_MAPPING:
            handleSetSimSlotsMappingResponse(msg);
            break;
        case RFX_MSG_REQUEST_ACTIVATE_UICC_CARD:
            handleActiveUiccCardResponse(msg);
            break;
        case RFX_MSG_REQUEST_DEACTIVATE_UICC_CARD:
            handleDeactivateUiccCardResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_IMSI:
            handleGetImsiResponse(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_FACILITY_LOCK:
            handleQueryFacilityLockResponse(msg);
            break;
        case RFX_MSG_REQUEST_SET_FACILITY_LOCK:
            handleSetFacilityLockResponse(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_SUBSCRIPTION:
            queryCdmaMspl(msg);
            break;
        case RFX_MSG_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE:
            handleCdmaGetSubSourceResponse(msg);
            break;
        case RFX_MSG_EVENT_SIM_QUERY_ICCID:
            handleEventQueryIccidResponse(msg);
            break;
        case RFX_MSG_EVENT_SIM_DETECT_SIM:
            handleDetectSimResponse(msg);
            break;
        case RFX_MSG_EVENT_GET_SLOT_STATUS:
            handleEventGetSlotStatusResponse(msg);
            break;
        case RFX_MSG_EVENT_GET_MEP_SLOT_STATUS:
            handleEventGetMepSlotStatusResponse(msg);
            break;
        case RFX_MSG_REQUEST_GET_SIM_TYPE_INFO:
            handleGetSimTypeInfoResponse(msg);
            break;
        case RFX_MSG_REQUEST_SWITCH_SIM_TYPE:
            handleSwitchSimTypeResponse(msg);
            break;
        case RFX_MSG_EVENT_GET_SIM_TYPE_INFO:
            handleEventGetSimTypeInfoResponse(msg);
            break;
        default:
            logD(mTag, "onHandleResponse, Not support the request %d!", request);
            break;
    }
}

void RmmSimCommRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();
    switch(event) {
        case RFX_MSG_EVENT_SIM_QUERY_ICCID:
            handleEventQueryIccid(msg);
            break;
        case RFX_MSG_EVENT_SIM_DETECT_SIM:
            handleDetectSim(msg);
            break;
        //case RFX_MSG_EVENT_SIM_SIM_AUTHENTICATION:
        //    handleLocalRequestSimAuthentication(msg); //not in use???
        //    break;
        //case RFX_MSG_EVENT_SIM_USIM_AUTHENTICATION:
        //    handleLocalRequestUsimAuthentication(msg); //not in use???
        //    break;
        case RFX_MSG_EVENT_GET_SLOT_STATUS:
            handleEventGetSlotStatus(msg);
            break;
        case RFX_MSG_EVENT_GET_MEP_SLOT_STATUS:
            handleEventGetMepSlotStatus(msg);
            break;
        //case RFX_MSG_EVENT_SIM_RESET:
        //     handleEventResetSim(msg); //sim_me lock used??
        //    break;
        case RFX_MSG_EVENT_GET_SIM_TYPE_INFO:
            handleEventGetSimTypeInfo(msg);
            break;
        default:
            logD(mTag, "onHandleEvent, Not support the event %d!", event);
            break;
    }
}

void RmmSimCommRequestHandler::sleepMsec(long long msec) {
    struct timespec ts;
    int err;

    ts.tv_sec = (msec / 1000);
    ts.tv_nsec = (msec % 1000) * 1000 * 1000;

    do {
        err = nanosleep(&ts, &ts);
    } while (err < 0 && errno == EINTR);
}

bool RmmSimCommRequestHandler::getSimAppInfo(mipc_app_status_desc_struct4 *src_app_info, RIL_AppStatus *p_app_info,
        UICC_Status uStatus, RfxMipcData* mipcData) {
    bool result = false;

    if (p_app_info != NULL) {
        memset(p_app_info, 0, sizeof(RIL_AppStatus));
    } else {
        return result;
    }

    p_app_info->app_type = getRilAppTypeFromMipc(src_app_info->app_type);
    p_app_info->app_state = getRilAppStateFromMipc(src_app_info->app_state);
    p_app_info->perso_substate = getPersoSubstateFromMipc(src_app_info->sub_status); //??TODO
    // Set aid for CDMA dual activation.
    if (p_app_info->app_type == RIL_APPTYPE_RUIM) {
        asprintf(&p_app_info->aid_ptr, "%s", "A000000000RUIM");
    } else {
        // Keep as default.
        asprintf(&p_app_info->aid_ptr, "%s", src_app_info->aid);
    }
    asprintf(&p_app_info->app_label_ptr, "%s", src_app_info->app_label);

    p_app_info->pin1_replaced = src_app_info->pin1_replaced;

    p_app_info->pin1 = (RIL_PinState) src_app_info->pin1_state; // mipc_sim_enabled_const_enum
    p_app_info->pin2 = (RIL_PinState) src_app_info->pin2_state; // mipc_sim_enabled_const_enum
    if (p_app_info->pin1 == 0) {
        p_app_info->pin1 = RIL_PINSTATE_DISABLED;
    }
    if (p_app_info->pin2 == 0) {
        p_app_info->pin2 = RIL_PINSTATE_DISABLED;
    }

    String8 logStr;
    logStr = String8::format("getSimAppInfo, app_type:%d, app_state:%d, AID:%.6s, label:%s, ",
            p_app_info->app_type, p_app_info->app_state, p_app_info->aid_ptr,
            p_app_info->app_label_ptr);
    logStr += String8::format("pin1:%d, pin2:%d, mipc_pin_status:%d", p_app_info->pin1,
            p_app_info->pin2,src_app_info->pin_status);
    logD(mTag, logStr.string());

    // Perso state and correct pin1/pin2 enable state
    switch (uStatus) {
        case UICC_READY:
            p_app_info->app_state = RIL_APPSTATE_READY;
            p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
            if (p_app_info->pin1 != RIL_PINSTATE_DISABLED) {
                p_app_info->pin1 = RIL_PINSTATE_ENABLED_VERIFIED;
            }

            if (p_app_info->pin2 != RIL_PINSTATE_DISABLED) {
                uint8_t pin1_count = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PIN1, 0xff);
                uint8_t puk1_count = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PUK1, 0xff);
                uint8_t pin2_count = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PIN2, 0xff);
                uint8_t puk2_count = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PUK2, 0xff);
                if (pin1_count != 0xff && puk1_count !=0xff && pin2_count != 0xff && puk2_count != 0xff) {
                    setPin1RetryCountProp(pin1_count);
                    setPuk1RetryCountProp(puk1_count);
                    setPin2RetryCountProp(pin2_count);
                    setPuk2RetryCountProp(puk2_count);
                }
                logD(mTag, "getSimAppInfo, PinPukRetryCount:%d,%d,%d,%d",
                        pin1_count, puk1_count, pin2_count, puk2_count);
                //RmmSimPinPukCount *retry = getPinPukRetryCount();
                if (pin2_count == 0 && puk2_count != 0xff && puk2_count > 0) {
                    p_app_info->pin2 = RIL_PINSTATE_ENABLED_BLOCKED;
                } else if (puk2_count == 0) {
                    p_app_info->pin2 = RIL_PINSTATE_ENABLED_PERM_BLOCKED;
                } else {
                    p_app_info->pin2 = RIL_PINSTATE_ENABLED_NOT_VERIFIED;
                }
            }
            break;
        case UICC_PIN:
            p_app_info->app_state = RIL_APPSTATE_PIN;
            p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
            p_app_info->pin1 = RIL_PINSTATE_ENABLED_NOT_VERIFIED;
            break;
        case UICC_PUK:
            p_app_info->app_state = RIL_APPSTATE_PUK;
            p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
            p_app_info->pin1 = RIL_PINSTATE_ENABLED_BLOCKED;
            break;
        case UICC_SIMP:
            p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
            p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SIM;
            break;
        case UICC_SIMP_PUK:
            p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
            p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SIM_PUK;
            break;
        case UICC_PERM_BLOCKED:
            p_app_info->app_state = RIL_APPSTATE_PUK;
            p_app_info->perso_substate = RIL_PERSOSUBSTATE_UNKNOWN;
            p_app_info->pin1 = RIL_PINSTATE_ENABLED_PERM_BLOCKED;
            p_app_info->pin2 = RIL_PINSTATE_UNKNOWN;
            break;
        default:
            //p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
            break;
    }

    result = true;

    return result;
}
/*
bool RmmSimCommRequestHandler::getSimAppInfo(int app_id, RIL_AppStatus *p_app_info,
        UICC_Status uStatus) {
    bool result = false;
    int err, intPara = -1;
    char *strPara = NULL;
    RfxAtLine *line = NULL;
    sp<RfxAtResponse> p_response = NULL;
    RmmSimPinPukCount *retry = NULL;

    if (p_app_info != NULL) {
        memset(p_app_info, 0, sizeof(RIL_AppStatus));
    } else {
        return result;
    }

    // Query SIM app info
    String8 cmd = String8::format("AT+ESIMAPP=%d,0", app_id);
    p_response = atSendCommandSingleline(cmd.string(), "+ESIMAPP:");

    if (p_response != NULL && p_response->getSuccess() > 0) {
        // Success
        line = p_response->getIntermediates();

        line->atTokStart(&err);

        // application id
        intPara = line->atTokNextint(&err);

        // channel id
        intPara = line->atTokNextint(&err);

        // AID
        strPara = line->atTokNextstr(&err);
        asprintf(&p_app_info->aid_ptr, "%s", strPara);

        // App label
        strPara = NULL;
        strPara = line->atTokNextstr(&err);
        asprintf(&p_app_info->app_label_ptr, "%s", strPara);

        // Pin1 enable
        intPara = -1;
        intPara = line->atTokNextint(&err);
        if (intPara == 0) {
            p_app_info->pin1 = RIL_PINSTATE_DISABLED;
        }

        // Pin2 enable
        intPara = -1;
        intPara = line->atTokNextint(&err);
        if (intPara == 0) {
            p_app_info->pin2 = RIL_PINSTATE_DISABLED;
        }

        retry = (RmmSimPinPukCount*)calloc(1, sizeof(RmmSimPinPukCount));
        RFX_ASSERT(retry != NULL);

        // PIN1 retry count
        retry->pin1 = line->atTokNextint(&err);
        // PIN2 retry count
        retry->pin2 = line->atTokNextint(&err);
        // PUK1 retry count
        retry->puk1 = line->atTokNextint(&err);
        // PUK2 retry count
        retry->puk2 = line->atTokNextint(&err);
        // Update the system properties of PIN and PUK
        // setPinPukRetryCountProp(retry);

        // Perso state and correct pin1/pin2 enable state
        switch (uStatus) {
            case UICC_READY:
                p_app_info->app_state = RIL_APPSTATE_READY;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
                if (p_app_info->pin1 != RIL_PINSTATE_DISABLED) {
                    p_app_info->pin1 = RIL_PINSTATE_ENABLED_VERIFIED;
                }
                if (p_app_info->pin2 != RIL_PINSTATE_DISABLED) {
                    if (retry->pin2 == 0 && retry->puk2 > 0) {
                        p_app_info->pin2 = RIL_PINSTATE_ENABLED_BLOCKED;
                    } else if (retry->puk2 == 0) {
                        p_app_info->pin2 = RIL_PINSTATE_ENABLED_PERM_BLOCKED;
                    } else {
                        p_app_info->pin2 = RIL_PINSTATE_ENABLED_NOT_VERIFIED;
                    }
                }
                break;
            case UICC_PIN:
                p_app_info->app_state = RIL_APPSTATE_PIN;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
                p_app_info->pin1 = RIL_PINSTATE_ENABLED_NOT_VERIFIED;
                break;
            case UICC_PUK:
                p_app_info->app_state = RIL_APPSTATE_PUK;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
                p_app_info->pin1 = RIL_PINSTATE_ENABLED_BLOCKED;
                break;
            case UICC_NETWORK_PERSONALIZATION:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK;
                break;
            case UICC_NP:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK;
                break;
            case UICC_NSP:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET;
                break;
            case UICC_SP:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER;
                break;
            case UICC_CP:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_CORPORATE;
                break;
            case UICC_SIMP:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SIM;
                break;
            case UICC_NP_PUK:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK_PUK;
                break;
            case UICC_NSP_PUK:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET_PUK;
                break;
            case UICC_SP_PUK:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER_PUK;
                break;
            case UICC_CP_PUK:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_CORPORATE_PUK;
                break;
            case UICC_SIMP_PUK:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SIM_PUK;
                break;
            case UICC_NS_SP:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_NS_SP;
                break;
            case UICC_NS_SP_PUK:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_NS_SP_PUK;
                break;
            case UICC_SIM_C:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SIM_C;
                break;
            case UICC_SIM_C_PUK:
                p_app_info->app_state = RIL_APPSTATE_SUBSCRIPTION_PERSO;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_SIM_SIM_C_PUK;
                break;
            case UICC_PERM_BLOCKED:
                p_app_info->app_state = RIL_APPSTATE_PUK;
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_UNKNOWN;
                p_app_info->pin1 = RIL_PINSTATE_ENABLED_PERM_BLOCKED;
                p_app_info->pin2 = RIL_PINSTATE_UNKNOWN;
                break;
            default:
                p_app_info->perso_substate = RIL_PERSOSUBSTATE_READY;
                break;
        }

        // App type

        String8 logStr;
        logStr = String8::format("app_id %d, app_type %d, app_state %d, AID %s, label %s, ",
                app_id, p_app_info->app_type, p_app_info->app_state, p_app_info->aid_ptr,
                p_app_info->app_label_ptr);
        logStr += String8::format("pin1 %d, pin2 %d, (%d, %d, %d, %d)", p_app_info->pin1,
                p_app_info->pin2, retry->pin1, retry->pin2, retry->puk1, retry->puk2);
        logD(mTag, logStr.string());
        result = true;
        free(retry);
    } else {
        logE(mTag, "Fail to get app info!");
    }

    p_response = NULL;

    // Set aid for CDMA dual activation.
    if (app_id == UICC_APP_RUIM) {
        asprintf(&p_app_info->aid_ptr, "%s", "A000000000RUIM");
    } else {
        // Keep as default.
    }

    return result;
}*/

void RmmSimCommRequestHandler::handleGetSimStatus(const sp<RfxMclMessage>& msg) {
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
    //RIL_CardStatus_v8 *p_card_status = NULL;
    //int cdma3GNT = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA3G_SWITCH_CARD);
    //bool cdma3GDM = getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD);
    //bool aospEnabled = isEnableUiccApplicationsSupported();

    if (cardType == -1) {
        // The UICC card is not ready.
        RIL_Errno ril_error = RIL_E_SIM_ERR;
        sp<RfxMclMessage> response;
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                        RfxSimStatusData(NULL, 0), msg, false);
        responseToTelCore(response);
    } /*else if ((!aospEnabled && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_OFF)) || (aospEnabled && (getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            != UICC_APPLICATIONS_ENABLE_STATE_ENABLED && getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            != UICC_APPLICATIONS_ENABLE_STATE_UNKNOWN))) {
        sp<RfxMclMessage> response;
        RIL_Errno ril_error = RIL_E_SIM_ERR;
        RIL_CardStatus_v8 *p_card_status = (RIL_CardStatus_v8*)calloc(1, sizeof(RIL_CardStatus_v8));
        RFX_ASSERT(p_card_status != NULL);
        p_card_status->iccId = (char *)calloc(1, RFX_PROPERTY_VALUE_MAX);
        RFX_ASSERT(p_card_status->iccId != NULL);
        p_card_status->eid = NULL;
        p_card_status->atr = NULL;

        if (!getIccId(p_card_status->iccId)) {
            ril_error = RIL_E_SIM_ERR;
        } else {
            // The UICC card is SIM off state.
            p_card_status->card_state = RIL_CARDSTATE_PRESENT;
            p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
            p_card_status->gsm_umts_subscription_app_index = -1;
            p_card_status->cdma_subscription_app_index = -1;
            p_card_status->ims_subscription_app_index = -1;
            p_card_status->num_applications = 0;
            ril_error = RIL_E_SUCCESS;
        }

        // The parameter used by AOSP_V1_2::CardStatus
        if (p_card_status != NULL) {
            p_card_status->physicalSlotId = m_slot_id;
        }

        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_NOT_READY);

        // Send TC layer the response
        if (ril_error == RIL_E_SUCCESS) {
            response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                    RfxSimStatusData((void*)p_card_status, sizeof(RIL_CardStatus_v8)), msg, false);
        } else {
            response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                    RfxSimStatusData(NULL, 0), msg, false);
        }
        responseToTelCore(response);

        if (p_card_status != NULL) {
            free(p_card_status->iccId);
            free(p_card_status);
        }
        logI(mTag, "handleGetSimStatus, uicc card disable case");
    } */
    else {
        // modem shall handle below case
        // 1. check sim switch
        // 2. check CDMA3G consistency between next card type and current card type
        // 3. uicc disable case
        bool aospEnabled = isEnableUiccApplicationsSupported();
        if ((!aospEnabled && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_OFF)) || (aospEnabled && (getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            != UICC_APPLICATIONS_ENABLE_STATE_ENABLED && getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
            != UICC_APPLICATIONS_ENABLE_STATE_UNKNOWN))) {
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_NOT_READY);
            //denbug
            logI(mTag, "handleGetSimStatus, uicc card disable case");
        }

        // Get SIM status
        logD(mTag, "handleGetSimStatus, token:%d, request start", msg->getToken());

        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_STATUS_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_STATUS_REQ_T_MODE, 1);
        callToMipcMsgAsync(msg, mipc_data);
    }
}

void RmmSimCommRequestHandler::handleGetSimStatusResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_error = RIL_E_SUCCESS;
    RIL_CardStatus_v8 *p_card_status = NULL;
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
    sp<RfxMclMessage> response;
    char tempstr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    uint8_t temp_uint8;
    UICC_Status uicc_status = UICC_NOT_READY;
    UICC_Status temp_status = UICC_NOT_READY;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    do {
        temp_uint8 = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_STATUS, 0xff);
        uicc_status = getUiccStatusFromMipc(temp_uint8);

        if (mipc_result == MIPC_RESULT_SUCCESS) {
            int sim_state = getSimStatebyUiccStatus(uicc_status);
            if (sim_state != RFX_SIM_STATE_UNKNOWN) {
                getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE, sim_state);
            }
        } else if (mipc_result == MIPC_RESULT_SIM_EXT_SIM_OFF) {
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_NOT_READY);
        } else if (mipc_result == MIPC_RESULT_SIM_NOT_INSERTED) {
            // MTK-START: AOSP SIM PLUG IN/OUT
            int esim_cause =
                    getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ESIMS_CAUSE);
            if (ESIMS_CAUSE_SIM_NO_INIT == esim_cause) {
                temp_status = UICC_NO_INIT;
            } else {
                temp_status = UICC_ABSENT;
            }
            logD(mTag, "handleGetSimStatusResponse, absent uicc_status:%d->%d", uicc_status, temp_status);
            uicc_status = temp_status;
        } else if (mipc_result == MIPC_RESULT_SIM_EXT_SIM_FAILURE) {
            logI(mTag, "handleGetSimStatusResponse, failure uicc_status:%d->UICC_ABSENT", uicc_status);
            uicc_status = UICC_ABSENT;
        } else if (mipc_result == MIPC_RESULT_BAD_SIM) {
            uint8_t pin1_count = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PIN1, 0xff);
            uint8_t puk1_count = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PUK1, 0xff);
            //RmmSimPinPukCount *retry = getPinPukRetryCount();
            if (pin1_count == 0 && puk1_count == 0) {
                temp_status = UICC_PERM_BLOCKED;            // PERM_DISABLED
                getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE,
                        RFX_SIM_STATE_PERM_DISABLED);
            } else {
                temp_status = UICC_NOT_READY;
            }
            logI(mTag, "handleGetSimStatusResponse, bad uicc_status:%d->%d", uicc_status, temp_status);
            uicc_status = temp_status;
        }

        p_card_status = (RIL_CardStatus_v8*)calloc(1, sizeof(RIL_CardStatus_v8));
        RFX_ASSERT(p_card_status != NULL);
        p_card_status->atr = (char *)calloc(1, (sizeof(char)* RFX_PROPERTY_VALUE_MAX));
        RFX_ASSERT(p_card_status->atr != NULL);
        p_card_status->iccId = (char *)calloc(1, (sizeof(char)* RFX_PROPERTY_VALUE_MAX));
        RFX_ASSERT(p_card_status->iccId != NULL);
        p_card_status->eid = (char *)calloc(1, (sizeof(char)* RFX_PROPERTY_VALUE_MAX));
        RFX_ASSERT(p_card_status->eid != NULL);

        temp_uint8 = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_GSM_APP_IDX, 0xff);
        if (temp_uint8 != 0xff) {
            p_card_status->gsm_umts_subscription_app_index= temp_uint8;
        } else {
           p_card_status->gsm_umts_subscription_app_index = -1;
        }

        temp_uint8 = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_CDMA_APP_IDX, 0xff);
        if (temp_uint8 != 0xff) {
            p_card_status->cdma_subscription_app_index= temp_uint8;
        } else {
           p_card_status->cdma_subscription_app_index = -1;
        }

        temp_uint8 = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_ISIM_APP_IDX, 0xff);
        if (temp_uint8 != 0xff) {
            p_card_status->ims_subscription_app_index = temp_uint8;
        } else {
           p_card_status->ims_subscription_app_index = -1;
        }

        temp_uint8 = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_APP_COUNT, 0);
        p_card_status->num_applications = temp_uint8;

        uint16_t list_size = 0;
        char *app_list =
            (char*) mipcData->getMipcVal(MIPC_SIM_STATUS_CNF_T_APP_LIST, &list_size);
        uint16_t count = list_size/sizeof(mipc_app_status_desc_struct4);
        for (int i = 0; i < count ; i++) {
            RIL_AppStatus *p_app_info = &(p_card_status->applications[i]);
            getSimAppInfo(
                (mipc_app_status_desc_struct4*)&(app_list[i*sizeof(mipc_app_status_desc_struct4)]),
                p_app_info,
                uicc_status,
                mipcData);
        }
        p_card_status->universal_pin_state = p_card_status->applications[0].pin1;

        uint16_t eid_len = 0;
        char *eid_string = (char*) mipcData->getMipcVal(MIPC_SIM_STATUS_CNF_T_EID, &eid_len);
        if (eid_string != NULL && eid_len > 0  && eid_len < RFX_PROPERTY_VALUE_MAX) {
            memcpy(p_card_status->eid, eid_string, eid_len);
            p_card_status->eid[eid_len] = '\0';
        } else {
            p_card_status->eid[0] = '\0';
        }

        uint16_t iccid_len = 0;
        char *iccid_string = (char*) mipcData->getMipcVal(MIPC_SIM_STATUS_CNF_T_ICCID, &iccid_len);
        if (iccid_string != NULL && iccid_len > 0  && iccid_len < RFX_PROPERTY_VALUE_MAX) {
            memcpy(p_card_status->iccId, iccid_string, iccid_len);
            p_card_status->iccId[iccid_len] = '\0';
        } else {
            p_card_status->iccId[0] = '\0';
            if (uicc_status != UICC_ABSENT && uicc_status != UICC_BUSY) {
                ril_error = RIL_E_SIM_ERR; // shall has iccid for sim interted
                break;
            }
        }

        uint16_t atr_len = 0;
        char *atr_string = (char*) mipcData->getMipcVal(MIPC_SIM_STATUS_CNF_T_ATR, &atr_len);
        if (atr_string != NULL && atr_len > 0  && atr_len < RFX_PROPERTY_VALUE_MAX) {
            memcpy(p_card_status->atr, atr_string, atr_len);
            p_card_status->atr[atr_len] = '\0';
        } else {
            p_card_status->atr[0] = '\0';
        }

        if (uicc_status == UICC_BUSY) {
            uicc_status = UICC_NOT_READY;
            //getMclStatusManager()->setIntValue(
            //        RFX_STATUS_KEY_ECPIN_STATE, RFX_WAIT_FOR_ECPIN);
        } else if (uicc_status == UICC_READY) {
            String8 testSimProp("vendor.gsm.sim.ril.testsim");
            char testStr[RFX_PROPERTY_VALUE_MAX] = { 0 };
            testSimProp.append(
                    (m_slot_id == 0) ? "" : String8::format(".%d", (m_slot_id + 1)));
            rfx_property_get(testSimProp, testStr, "0");
            // add check for ALPS05243227, query test sim timing issue
            if (strcmp("0", testStr) == 0) {
                temp_uint8 = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_TEST_SIM, 0);
                if (temp_uint8 == 1) {
                    rfx_property_set(testSimProp, "1");
                }
            }
        }

        p_card_status->universal_pin_state = (RIL_PinState) mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_UPIN_STATUS, 0);
        p_card_status->card_state = (RIL_CardState) mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_CARD_PRESENT_STATE, 0);
        p_card_status->mepMode = (RIL_MepMode) mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_MEP_MODE, 0);

        if (uicc_status == UICC_ABSENT) {
            // UICC absent
            //p_card_status->card_state = RIL_CARDSTATE_ABSENT;
            //p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
            resetSimPropertyAndStatusKey();
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_ABSENT);

            rfx_property_get("persist.vendor.radio.c_capability_slot", tempstr, "0");
            if (((cardType & RFX_CARD_TYPE_RUIM) || (cardType & RFX_CARD_TYPE_CSIM)) &&
                    !(cardType & RFX_CARD_TYPE_USIM) && !(cardType & RFX_CARD_TYPE_SIM) &&
                    (m_slot_id != atoi(tempstr) - 1)) {
                // For RUIM single mode card, iccid is not set in rild. If C capability is not in
                // card for C+C mode, there is no chance to update iccid in rild. Here is to update
                // iccid to N/A in such situation.
                logI(mTag, "handleGetSimStatusResponse, CDMA slot: %d, set sim iccid: N/A",
                        atoi(tempstr) - 1);
                getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8("N/A"));
                sendIccidChanged("N/A");
            }
        } else if (((mipc_result != MIPC_RESULT_SIM_EXT_SIM_OFF) && (uicc_status == UICC_NOT_READY))
                || (p_card_status->iccId[0] == '\0' && uicc_status != UICC_EMPTY_EUICC)) {
            // Card state error
            logI(mTag, "handleGetSimStatusResponse, card_state:%d->ERROR, pin_state:%d->unknown",
                p_card_status->card_state, p_card_status->universal_pin_state);

            p_card_status->card_state = RIL_CARDSTATE_ERROR;
            p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
        } else if ((uicc_status == UICC_NO_INIT) || (uicc_status == UICC_CARD_REBOOT)
                || (uicc_status == UICC_EMPTY_EUICC)) {
            logI(mTag, "handleGetSimStatusResponse, card_state:%d->present, pin_state:%d->unknown",
                p_card_status->card_state, p_card_status->universal_pin_state);

            p_card_status->card_state = RIL_CARDSTATE_PRESENT;
            p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
        } else {
            //if (uicc_status == UICC_PERM_BLOCKED) {
            //    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE,
            //            RFX_SIM_STATE_PERM_DISABLED);
            //}

            // Normal case
            if (uicc_status == UICC_USIM_RESTRICTED || uicc_status == UICC_SIM_RESTRICTED) {
                logI(mTag, "handleGetSimStatusResponse,pin_state:%d->RIL_CARDSTATE_RESTRICTED",
                     p_card_status->card_state);
                p_card_status->card_state = RIL_CARDSTATE_RESTRICTED;
            }
            // Correct universal pin state
            //p_card_status->universal_pin_state = p_card_status->applications[0].pin1;
        }

        logI(mTag,
            "handleGetSimStatusResponse, token:%d, result:%d,state:%d,pin:%d,appindex:%d,%d,%d, atr:%d,%.6s, icid:%d,%.6s, eid:%d,%.6s, mepMode:%d",
            msg->getToken(),
            mipc_result,
            p_card_status->card_state,
            p_card_status->universal_pin_state,
            p_card_status->gsm_umts_subscription_app_index,
            p_card_status->cdma_subscription_app_index,
            p_card_status->ims_subscription_app_index,
            atr_len, atr_string, iccid_len, iccid_string, eid_len, eid_string, p_card_status->mepMode);
    } while(0);

    if (mipc_result == MIPC_RESULT_SUCCESS) {
        mRetryGetSimStatusTimes = 0;
    } else if (mipc_result == MIPC_RESULT_OPERATION_NOT_ALLOWED) {
        mRetryGetSimStatusTimes = 0;
        // For sim switch or 3G dual mode card switch
        ril_error = RIL_E_INVALID_STATE;
    } else {
        switch (mipc_result) {
            case MIPC_RESULT_BUSY:
                uicc_status = UICC_BUSY;
                mRetryGetSimStatusTimes++;
                logI(mTag, "handleGetSimStatusResponse, sim busy, may retry:%d", mRetryGetSimStatusTimes);
                if (mRetryGetSimStatusTimes == MAX_RETRY_COUNT /*|| RfxRilUtils::isVsimEnabledBySlot(m_slot_id)*/) {
                    uicc_status = UICC_NOT_READY;
                    getMclStatusManager()->setIntValue(
                            RFX_STATUS_KEY_ECPIN_STATE, RFX_WAIT_FOR_ECPIN);
                    ril_error = RIL_E_SIM_ERR;
                } else {
                    //sleepMsec(200);
                    sp<RfxMclMessage> mclMessage =  RfxMclMessage::obtainRequest(
                                                            msg->getId(),
                                                            msg->getData(),
                                                            m_slot_id,
                                                            msg->getToken(),
                                                            msg->getSendToMainProtocol(),
                                                            msg->getRilToken(),
                                                            ms2ns(200), //200ms
                                                            msg->getTimeStamp(),
                                                            msg->getAddAtFront());
                    RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
                    // Release memory
                    for (int i = 0; i < p_card_status->num_applications; i++) {
                        if (p_card_status->applications[i].app_label_ptr != NULL) {
                            free(p_card_status->applications[i].app_label_ptr);
                        }
                        if (p_card_status->applications[i].aid_ptr != NULL) {
                            free(p_card_status->applications[i].aid_ptr);
                        }
                    }
                    free(p_card_status->atr);
                    free(p_card_status->iccId);
                    free(p_card_status->eid);
                    free(p_card_status);
                    return;
                }
                break;
            case MIPC_RESULT_SIM_EXT_SIM_FAILURE:
            case MIPC_RESULT_SIM_NOT_INSERTED:
            case MIPC_RESULT_BAD_SIM:
                break;
            default:
                //uicc_status = UICC_NOT_READY; TODO???
                break;
        }
        /*
        if (uicc_status == UICC_NOT_READY || !getIccId(p_card_status->iccId)) {
            // Card state error
            p_card_status->card_state = RIL_CARDSTATE_ERROR;
            p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
            p_card_status->gsm_umts_subscription_app_index = -1;
            p_card_status->cdma_subscription_app_index = -1;
            p_card_status->ims_subscription_app_index = -1;
            p_card_status->num_applications = 0;
            p_card_status->atr = (char *)calloc(1, (sizeof(char)* RFX_PROPERTY_VALUE_MAX));
            RFX_ASSERT(p_card_status->atr != NULL);
            if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, false)) {
                getAtr(p_card_status->atr);
            }
            logE(mTag, "handleGetSimStatus ret = not ready or iccId not ready.");
        } else if (uicc_status == UICC_ABSENT) {
            // UICC absent
            p_card_status->card_state = RIL_CARDSTATE_ABSENT;
            p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
            p_card_status->gsm_umts_subscription_app_index = -1;
            p_card_status->cdma_subscription_app_index = -1;
            p_card_status->ims_subscription_app_index = -1;
            p_card_status->num_applications = 0;
            resetSimPropertyAndStatusKey();
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_STATE, RFX_SIM_STATE_ABSENT);
            p_card_status->iccId = NULL;
            p_card_status->eid = NULL;
            p_card_status->atr = NULL;
        } else if ((uicc_status == UICC_NO_INIT) || (uicc_status == UICC_CARD_REBOOT)
                || (uicc_status == UICC_EMPTY_EUICC)) {//UICC_CARD_REBOOT UICC_EMPTY_EUICC can remove
            p_card_status->card_state = RIL_CARDSTATE_PRESENT;
            p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
            p_card_status->gsm_umts_subscription_app_index = -1;
            p_card_status->cdma_subscription_app_index = -1;
            p_card_status->ims_subscription_app_index = -1;
            p_card_status->num_applications = 0;
            p_card_status->atr = (char *)calloc(1, (sizeof(char)* RFX_PROPERTY_VALUE_MAX));
            RFX_ASSERT(p_card_status->atr != NULL);
            if (uicc_status == UICC_EMPTY_EUICC) {
                getAtr(p_card_status->atr);
            }
            p_card_status->iccId = NULL;
            p_card_status->eid = NULL;
        } */
    }


    uint8_t physicalSlotId = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PHY_SLOT_ID, 0xff);
    uint8_t portId = mipcData->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_PORT_ID, 0xff);
    /// Meaning invalid physical idx, modem do not support
    if (physicalSlotId == 0xff) {
        p_card_status->slotPort.physicalSlotId = m_slot_id;
        p_card_status->slotPort.portId = 0;
    } else {
        p_card_status->slotPort.physicalSlotId = physicalSlotId;
        p_card_status->slotPort.portId = portId;
    }
    logD(mTag,"handleGetSimStatusResponse, physicalSlotId: %d, portId: %d",
            p_card_status->slotPort.physicalSlotId , p_card_status->slotPort.portId);

    // The parameter used by AOSP_V1_2::CardStatus
    p_card_status->physicalSlotId = m_slot_id;

    if (!getIccId(p_card_status->iccId)
            || (mRetryGetSimStatusTimes >= MAX_RETRY_COUNT && uicc_status == UICC_NOT_READY)) {
        ril_error = RIL_E_SIM_ERR;
        logE(mTag, "handleGetSimStatusResponse, error: no iccid or retry fail.");
    }

    // External SIM [Start]
    // VSIM need more time to initialize, we keep SIM status as NOT_READY event if get BUSY
    // over than 2 seconds.
    //if (uicc_status == UICC_NOT_READY && RfxRilUtils::isVsimEnabledBySlot(m_slot_id)) {
    //    ril_error = RIL_E_SIM_ERR;
    //    logE(mTag, "handleGetSimStatusResponse, SIM status is not ready under vsim enabled.");
    //}
    // External SIM [End]

    // Send TC layer the response
    if (ril_error == RIL_E_SUCCESS) {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxSimStatusData((void*)p_card_status, sizeof(RIL_CardStatus_v8)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxSimStatusData(NULL, 0), msg, false);
    }
    responseToTelCore(response);

    // Release memory
    for (int i = 0; i < p_card_status->num_applications; i++) {
        if (p_card_status->applications[i].app_label_ptr != NULL) {
            free(p_card_status->applications[i].app_label_ptr);
        }
        if (p_card_status->applications[i].aid_ptr != NULL) {
            free(p_card_status->applications[i].aid_ptr);
        }
    }
    free(p_card_status->atr);
    free(p_card_status->iccId);
    free(p_card_status->eid);
    free(p_card_status);
}

void RmmSimCommRequestHandler::handleSimIo(const sp<RfxMclMessage>& msg) {
    RIL_SIM_IO_v6 *pData = (RIL_SIM_IO_v6*)(msg->getData()->getData());
    String8 aid((pData->aidPtr != NULL)? pData->aidPtr : "");
    int appTypeId = -1;
    RIL_Errno result = RIL_E_SIM_ERR;
    sp<RfxMipcData> mipc_data = NULL;
    uint8_t sim_app_id = 0;
    unsigned char *byteData = NULL;

    appTypeId = queryAppTypeId(aid);
    if (appTypeId == -1) {
        result = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }

    //modem will check if service active, if not, return not support.

    //if (pData->pin2 != NULL && strlen(pData->pin2) > 0) {
    // may be need enter pin2
    //cmd.append(String8::format("AT+EPIN2=\"%s\"", pData->pin2));

    // 1.SIM access
    // Remove 3F00 or 3f00 for path
    // if path has 7f25 or 7F25 and the CDMA 3G card is switched into SIM, go to error

    // 2. Query channel id
    // if channelId == 255,  result = RIL_E_SIM_BUSY, go to error
    //cmd.append(String8::format("AT+ESIMAPP=%d,0", appTypeId));
    //p_response = atSendCommandSingleline(cmd.string(), "+ESIMAPP:");

    logD(mTag, "handleSimIo, token:%d, SIM_IO(%d, %x, %d, %d, %d, %s)", msg->getToken(), pData->command,
            pData->fileid, pData->p1, pData->p2, pData->p3, pData->path);

    mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ, m_slot_id);
    sim_app_id = getMipcAppTypeFromRil(appTypeId);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_APP_ID, sim_app_id);
    //mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_SESSION_ID, (uint8_t)channelId);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_CMD, (uint8_t)pData->command);
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FILE_ID, (uint16_t)pData->fileid);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P1, (uint8_t)pData->p1);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P2, (uint8_t)pData->p2);
    if ((pData->command == 192) || (pData->command == 242)) {
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX, 0);
    } else {
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX, (uint16_t)pData->p3);
    }
    if ((pData->data != NULL)) {
        uint16_t bLen = 0;
        uint16_t len = strlen(pData->data);
        bLen = hexStringToByteArrayEx((unsigned char*)pData->data, len, &byteData);
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN, bLen);
        mipc_data->addMipcTlv(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA, bLen, byteData); //Byte_array
    } else {
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN, 0);
    }
    if (pData->path != NULL) {
        if ((strstr(pData->path, "3F00") || strstr(pData->path, "3f00")) && (strlen(pData->path) > 4)) {
            mipc_data->addMipcTlvStr(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH, pData->path + 4);
        } else {
            mipc_data->addMipcTlvStr(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH, pData->path);
        }
    }

    callToMipcMsgAsync(msg, mipc_data);

    if (byteData != NULL) {
        free(byteData);
    }

    //if (((pData->command == 192) || (pData->command == 242)) && (remain <= 0)) {
        // Set remain value same with AOSP define for get_response command.
    //    remain = 15; ///??? for what TODO
    //}
    /*while (remain > 0) {
        // 3. Send AT+ECRLA
        result = RIL_E_SIM_ERR;

        if ((pData->command == 192) || (pData->command == 242)) {
            // GET RESPONSE or STATUS
            cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d", appTypeId, channelId,
                    pData->command, pData->fileid, pData->p1, pData->p2, 0 ));
        } else if (data.isEmpty()) {
            // READ COMMAND, P3: Max value is 256 and use 0 to represent 256
            len = ((remain < 256) ? remain : 256);
            cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d", appTypeId, channelId,
                    pData->command, pData->fileid, (0xFF & (offset >> 8)),  ??????
                    (0xFF & offset),
                    ((remain < 256) ? remain : 0)));
        } else {
            // WRITE COMMAND, P3: Max value is 255
            len = (remain > 255) ? 255 : remain;

            cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d", appTypeId, channelId,
                    pData->command, pData->fileid, (0xFF & (offset >> 8)), ??????
                    (0xFF & offset),
                    len));
        }
    } */

    return;
error:
    sp<RfxMclMessage> response;
    RIL_SIM_IO_Response sr;
    memset(&sr, 0, sizeof(RIL_SIM_IO_Response));
    response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg, false);

    responseToTelCore(response);

}

void RmmSimCommRequestHandler::handleSimIoResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_IO_v6 *pData = (RIL_SIM_IO_v6*)(msg->getData()->getData());
    String8 path("");
    String8 aid((pData->aidPtr != NULL)? pData->aidPtr : "");
    //String8 data((pData->data != NULL)? pData->data : "");
    char *simResponse = NULL;
    int appTypeId = -1;// channelId = 0;
    //int remain = pData->p3;
    RIL_Errno result = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RIL_SIM_IO_Response sr;
    uint16_t sw = 0;
    uint16_t apdu_len = 0;

    memset(&sr, 0, sizeof(RIL_SIM_IO_Response));
    //p_response = atSendCommandSingleline(cmd.string(), "+ECRLA:");

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logD(mTag, "handleSimIoResponse, token:%d, mipc_result:%d",
                msg->getToken(), mipc_result);
        //if (channelId == 255) result = RIL_E_SIM_BUSY;
        switch (mipc_result) {
            case MIPC_RESULT_SIM_EXT_PIN2_REQUIRED:
                result = RIL_E_SIM_PIN2;
                break;
            case MIPC_RESULT_SIM_EXT_PUK2_REQUIRED:
                result = RIL_E_SIM_PUK2;
                break;
            case MIPC_RESULT_OPERATION_NOT_SUPPORTED:
                result = RIL_E_REQUEST_NOT_SUPPORTED;
                break;
            case MIPC_RESULT_BUSY:
                result = RIL_E_SIM_BUSY;
                break;
            default:
                result = RIL_E_SIM_ERR;
                break;
        }

        goto error;
    }

    sw = mipcData->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_SW, 0);
    apdu_len =
        mipcData->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_LEN, 0);
    if (apdu_len > 0) {
        uint16_t len = 0;
        simResponse =
            (char*) mipcData->getMipcVal(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_APDU, &len);
        sr.simResponse = (char*)calloc(1, (sizeof(char)* (apdu_len + 1)));
        RFX_ASSERT(sr.simResponse != NULL);
        memcpy(sr.simResponse, simResponse, apdu_len);
        sr.simResponse[apdu_len] = '\0';

        //logD(mTag, "handleSimIoResponse, md response:%.6s", sr.simResponse);

        // Run FCP parser for USIM and CSIM when the command is GET_RESPONSE
        appTypeId = queryAppTypeId(aid);
        if ((appTypeId == UICC_APP_USIM || appTypeId == UICC_APP_CSIM ||
                appTypeId == UICC_APP_ISIM || isSimIoFcp(sr.simResponse))
                && pData->command == 192 /* GET_RESPONSE */) {
            makeSimRspFromUsimFcp((unsigned char**)&(sr.simResponse), apdu_len, pData->fileid);
        } else {
            char* temp = sr.simResponse;
            sr.simResponse = (char*)byteArrayToHexString((unsigned char*)sr.simResponse, apdu_len);
            free(temp);
        }
    }

    sr.sw2 = sw & 0xFF;
    sr.sw1 = (sw >> 8) & 0xFF;

    logD(mTag, "handleSimIoResponse, token:%d, result:%d, (sw:%x, sw1:%x, sw2:%x, rsp:%d,%.6s)",
            msg->getToken(), mipc_result, sw, sr.sw1, sr.sw2, apdu_len, sr.simResponse);

    if ((sr.sw1 == 0x00) && (sr.sw2 == 0x00)) {
        result = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    } else if ((sr.sw1 == 0x69) && (sr.sw2 == 0x82)) {
        result = RIL_E_OPERATION_NOT_ALLOWED;
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg, false);
    responseToTelCore(response);

    if (sr.simResponse != NULL) {
        free(sr.simResponse);
    }
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg, false);

    responseToTelCore(response);

    if (sr.simResponse != NULL) {
        free(sr.simResponse);
    }
}

void RmmSimCommRequestHandler::handleIsimAuth(const sp<RfxMclMessage>& msg) {
    char *pData = (char*)(msg->getData()->getData());
    RIL_Errno result = RIL_E_SUCCESS;
    int currSimInsertedState = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_STATE);
    unsigned char *byteData = NULL;
    uint16_t cmd_len = 0;
    uint16_t bLen = 0;

    do {
        if (RFX_SIM_STATE_ABSENT == currSimInsertedState) {
            logE(mTag, "handleIsimAuth, sim absent");
            result = RIL_E_SIM_ABSENT;
            break;
        }
        if (pData == NULL || strlen(pData) <= 0) {
            logE(mTag, "handleIsimAuth, empty parameter");
            result = RIL_E_INVALID_ARGUMENTS;
            break;
        }

        /* 
        p2 is mode
        Table 7.1.2-1: Coding of the reference control P2
        Coding
        b8-b1
        Meaning
        '1-------' Specific reference data (e.g. DF
        specific/application dependant key)
        '----- XXX' Authentication context:
        000 GSM context
        001 3G context
        010 VGCS/VBS context
        */
        logD(mTag, "handleIsimAuth, req");
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_EXT_AUTH_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_GET_EXT_AUTH_REQ_T_MODE, 0);
        mipc_data->addMipcTlvUint8(MIPC_SIM_GET_EXT_AUTH_REQ_T_APP_ID, MIPC_SIM_APP_TYPE_ISIM);
        cmd_len = strlen((const char*)pData);
        bLen = hexStringToByteArrayEx((unsigned char*)pData, cmd_len, &byteData);
        mipc_data->addMipcTlvUint16(MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_LEN, bLen);
        mipc_data->addMipcTlv(MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_DATA, bLen, byteData); //byte_array
        callToMipcMsgAsync(msg, mipc_data);
        if (byteData != NULL) {
            free(byteData);
        }
    } while(0);

    sp<RfxMclMessage> response = NULL;
    if (RIL_E_SUCCESS != result) {
        response = RfxMclMessage::obtainResponse(msg->getId(), result,
                RfxStringData(), msg, false);
        responseToTelCore(response);
    }
}

void RmmSimCommRequestHandler::handleIsimAuthResponse(const sp<RfxMclMessage>& msg) {
    char *auth_rsp = NULL;
    char *simResponse = NULL;
    RIL_Errno result = RIL_E_SIM_ERR;

    /*
    do {
        // 1. Query channel id
        // TODO: Set ISIM app type id
        cmd.append(String8::format("AT+ESIMAPP=1,0"));

        p_response = atSendCommandSingleline(cmd.string(), "+ESIMAPP:");

        if (p_response != NULL && p_response->getSuccess() == 1) {
            // Success
            line = p_response->getIntermediates();
            line->atTokStart(&err);

            // application id
            intPara = line->atTokNextint(&err);

            // channel id
            channelId = line->atTokNextint(&err);
            if (channelId == 255) {
                logE(mTag, "channel id is 255!");  //???????
                result = RIL_E_SIM_BUSY;
                break;
            }

            p_response = NULL;
            cmd.clear();
            line = NULL;
        } else {
            logE(mTag, "Fail to query ISIM channel");
            break;
        }

        // 2. ISIM authentication
        cmd.append(String8::format("AT+ESIMAUTH=%d,0,\"%s\"", channelId, pData));
        p_response = atSendCommandSingleline(cmd.string(), "+ESIMAUTH:");

        if (p_response != NULL && p_response->getSuccess() == 1) {
            // Success
           ......

            if(line->atTokHasmore()){
                sr.simResponse = line->atTokNextstr(&err);
                if(err < 0) {
                    break;
                }

                auth_rsp = (char*)calloc(1, sizeof(char)*MAX_AUTH_RSP);
                RFX_ASSERT(auth_rsp != NULL);
                strcpy(auth_rsp, sr.simResponse);
                logI(mTag, "requestIsimAuthentication %s, %02x, %02x", auth_rsp, sr.sw1, sr.sw2);
            } else {
                logI(mTag, "No response data");
            }
            result = RIL_E_SUCCESS;
        } else {
            logE(mTag, "Fail to do ISIM authentication!");
            break;
        }

        p_response = NULL;
    } while(0); */

    do {
        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        if (mipc_result == MIPC_RESULT_SUCCESS) {
            uint16_t sw = mipcData->getMipcUint16Val(MIPC_SIM_GET_EXT_AUTH_CNF_T_SW, 0);
            uint16_t rsp_len =
                mipcData->getMipcUint16Val(MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_LEN, 0);
            if (rsp_len > 0) {
                uint16_t len = 0;
                simResponse =
                    (char*) mipcData->getMipcVal(MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_DATA, &len);
                if (simResponse == NULL) {
                    logI(mTag, "handleIsimAuthResponse, fail No response data");
                    break;
                }
            } else {
                logI(mTag, "handleIsimAuthResponse, No response data");
            }

            if (rsp_len > 0) {
                auth_rsp = (char*)byteArrayToHexString((unsigned char*)simResponse, rsp_len);
            }

            logD(mTag, "handleIsimAuthResponse, rsp:%s..., len:%d, sw:%04x", auth_rsp, rsp_len, sw);

            result = RIL_E_SUCCESS;
        } else {
             //if (channelId == 255) {
             //   logE(mTag, "channel id is 255!");  //??????? TODO
             //   result = RIL_E_SIM_BUSY;
             logI(mTag, "handleIsimAuthResponse, mipc_result:%d", mipc_result);
        }
    } while(0);

    sp<RfxMclMessage> response = NULL;
    if (RIL_E_SUCCESS == result && auth_rsp != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), result,
                RfxStringData((void*)auth_rsp, strlen(auth_rsp)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), result,
                RfxStringData(), msg, false);
    }
    responseToTelCore(response);
    if (NULL != auth_rsp) {
        free(auth_rsp);
    }
}

void RmmSimCommRequestHandler::handleGeneralSimAuth(const sp<RfxMclMessage>& msg) {
    RIL_SimAuthStructure *pSimAuthData = NULL;
    pSimAuthData = (RIL_SimAuthStructure*)(msg->getData()->getData());
    unsigned char *byteData = NULL;

    logI(mTag, "handleGeneralSimAuth, mode:%d, ch:%d",
            pSimAuthData->mode, pSimAuthData->sessionId);

    char *cmd_data = NULL;
    if(pSimAuthData->mode == 0) {
        // mode 0 is represent AKA authentication mode
        if (pSimAuthData->sessionId == 0) {
            // Session id is equal to 0, for backward compability, we use old AT command
            if (pSimAuthData->param2 != NULL && strlen(pSimAuthData->param2) > 0) {
                // There is param2 means that caller except to use USIM AUTH
                //cmd.append(String8::format("AT+EAUTH=\"%s\",\"%s\"",
                //        pSimAuthData->param1, pSimAuthData->param2));
                asprintf(&cmd_data, "%s%s", pSimAuthData->param1, pSimAuthData->param2); //???
            } else {
                // There is no param2 means that caller except to use SIM AUTH
                //cmd.append(String8::format("AT+EAUTH=\"%s\"", pSimAuthData->param1));
                asprintf(&cmd_data, "%s", pSimAuthData->param1);
            }
        } else {
            // Session id is not equal to 0, means we can use new AT command
            //cmd.append(String8::format("AT+ESIMAUTH=%d,%d,\"%s%s\"",
            //        pSimAuthData->sessionId, pSimAuthData->mode,
            //        pSimAuthData->param1, pSimAuthData->param2));
            asprintf(&cmd_data, "%s%s", pSimAuthData->param1, pSimAuthData->param2);
        }
    } else if (pSimAuthData->mode == 1) {
        // mode 1 is represent GBA authentication mode
        //cmd.append(String8::format("AT+ESIMAUTH=%d,%d,\"%02x%s%s\"",
        //        pSimAuthData->sessionId, pSimAuthData->mode, pSimAuthData->tag,
        //        pSimAuthData->param1, pSimAuthData->param2));
         asprintf(&cmd_data, "%02x%s%s", pSimAuthData->tag, pSimAuthData->param1, pSimAuthData->param2);
    }

    sp<RfxMipcData> mipc_data = NULL;
    if (pSimAuthData->mode == 0 && pSimAuthData->sessionId == 0 &&
        (pSimAuthData->param2 == NULL || strlen(pSimAuthData->param2) == 0)) {
        mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_GSM_AUTH_REQ, m_slot_id);
        uint16_t cmd_len = strlen((const char*)pSimAuthData->param1);
        uint16_t bLen = hexStringToByteArrayEx((unsigned char*)pSimAuthData->param1, cmd_len, &byteData);
        mipc_data->addMipcTlv(MIPC_SIM_GET_GSM_AUTH_REQ_T_RAND1, bLen, byteData);
    } else {
        mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_EXT_AUTH_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_GET_EXT_AUTH_REQ_T_CH, (uint8_t)pSimAuthData->sessionId);
        mipc_data->addMipcTlvUint8(MIPC_SIM_GET_EXT_AUTH_REQ_T_MODE, (uint8_t)pSimAuthData->mode);
        if (cmd_data != NULL) {
            uint16_t cmd_len = strlen((const char*)cmd_data);
            uint16_t bLen = hexStringToByteArrayEx((unsigned char*)cmd_data, cmd_len, &byteData);
            mipc_data->addMipcTlvUint16(MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_LEN, bLen);
            mipc_data->addMipcTlv(MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_DATA, bLen, byteData); //byte_array
        }
        //mipc_data->addMipcTlvUint8(MIPC_SIM_GET_EXT_AUTH_REQ_T_APP_ID, );
    }
    callToMipcMsgAsync(msg, mipc_data);
    if (byteData != NULL) {
        free(byteData);
    }
    if (cmd_data != NULL) {
        free(cmd_data);
    }
}

void RmmSimCommRequestHandler::handleGeneralSimAuthResponse(const sp<RfxMclMessage>& msg) {
    char *simResponse = NULL;
    char *auth_rsp = NULL;
    RIL_Errno result = RIL_E_GENERIC_FAILURE;
    RIL_SIM_IO_Response sim_auth_response;
    memset(&sim_auth_response, 0, sizeof(RIL_SIM_IO_Response));

    do {
        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        logD(mTag, "handleGeneralSimAuthResponse, mipc_result: %d", mipc_result);

        if (mipc_result == MIPC_RESULT_SUCCESS) {
            uint16_t sw = 0;
            uint16_t rsp_len = 0;
            if (mipcData->getMipcMsgId() == MIPC_SIM_GET_EXT_AUTH_REQ) {
                //uint16_t sw = mipcData->getMipcUint16Val(MIPC_SIM_GET_EXT_AUTH_CNF_T_SW, 0);
                uint16_t rsp_len =
                    mipcData->getMipcUint16Val(MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_LEN, 0);
                  if (rsp_len > 0) {
                    uint16_t len = 0;
                    simResponse =
                        (char*) mipcData->getMipcVal(MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_DATA, &len); //byte_arry
                    if (simResponse == NULL) {
                        logI(mTag, "handleGeneralSimAuthResponse, fail No response data");
                        break;
                    }
                } else {
                    logI(mTag, "handleGeneralSimAuthResponse, No response data");
                }

            } else {
                //uint16_t sw = mipcData->getMipcUint16Val(MIPC_SIM_GET_GSM_AUTH_CNF_T_SW, 0);
                simResponse =
                    (char*) mipcData->getMipcVal(MIPC_SIM_GET_GSM_AUTH_CNF_T_SRES1, &rsp_len);
                //simResponse =
                //    (char*) mipcData->getMipcVal(MIPC_SIM_GET_GSM_AUTH_CNF_T_KC1, &rsp_len); //???response
                if (simResponse == NULL) {
                    logI(mTag, "handleGeneralSimAuthResponse, fail No response data");
                    break;
                }
            }

            sim_auth_response.sw2 = sw & 0xFF;
            sim_auth_response.sw1 = (sw >> 8) & 0xFF;

            if (rsp_len > 0) {
                auth_rsp = (char*)byteArrayToHexString((unsigned char*)simResponse, rsp_len);
            }
            sim_auth_response.simResponse = auth_rsp;

            logI(mTag, "handleGeneralSimAuthResponse, rsp:%.6s..., len:%d, sw:%02x,%02x",
                    auth_rsp, rsp_len, sim_auth_response.sw1, sim_auth_response.sw2);

            result = RIL_E_SUCCESS;

        } else {
            break;
        }
    } while(0);

    sp<RfxMclMessage> response = NULL;
    if (result == RIL_E_SUCCESS) {
        response = RfxMclMessage::obtainResponse(msg->getId(), result,
                RfxSimIoRspData((void*)&sim_auth_response, sizeof(RIL_SIM_IO_Response)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), result,
                RfxSimIoRspData(NULL, 0), msg, false);
    }
    responseToTelCore(response);
    if (auth_rsp != NULL) {
        free(auth_rsp);
    }
}

void RmmSimCommRequestHandler::handleSimAuth(const sp<RfxMclMessage>& msg) {
    RIL_SimAuthentication *pSimAuthData = NULL;
    unsigned char *out_put = NULL;
    RIL_Errno result = RIL_E_SIM_ERR;
    unsigned char *byteData = NULL;

    do {
        pSimAuthData = (RIL_SimAuthentication*)(msg->getData()->getData());
        if (pSimAuthData == NULL) {
            break;
        }
        if (NULL == pSimAuthData->authData) {
            logE(mTag, "handleSimAuth, error authData is null!");
            result = RIL_E_INVALID_ARGUMENTS;
            break;
        }
        //if (pSimAuthData->aid != NULL) {
        //    logE(mTag, "handleSimAuth, error aid is null!");
        //    result = RIL_E_INVALID_ARGUMENTS;
        //    break;
        //}

        // 1. Query app type id
        // 2. Get channel id
        // 3. Decode data
        out_put = base64_decode_to_str((unsigned char *)pSimAuthData->authData,
                strlen(pSimAuthData->authData));
        logI(mTag, "handleSimAuth, output %s, aid:%s", out_put, pSimAuthData->aid);
        if (out_put == NULL) {
            logE(mTag, "handleSimAuth, out_put is null!");
            result = RIL_E_INVALID_ARGUMENTS;
            break;
        }

        // 4. Send authentication APDU
        // AT+CGLA="channel id","len","CLA+INS+P1+P2+Lc+Data+Le"
        // CLA: if channel id 0~3, then CLA=0X; otherwise, if channel id 4~19, then CLA=4X
        // INS: 88 for authentication command
        // P1:00
        // P2 is decided by user's parameter, will be 8X according authentication context type
        // Lc:length of authentication context
        // Data: authentication context decided by user's parameter
        // Le: max length of data expected in response. use 00 represent unknown.
        /*if (out_put != NULL) {
            if ((appTypeId == UICC_APP_USIM) || (appTypeId == UICC_APP_ISIM)) {
                //TODO: The case of USIM
                cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x%s00\"",
                        channelId, (unsigned int)(12 + strlen((char*)out_put)),
                        ((channelId <= 3) ? channelId : (4 * 16 + channelId - 4)), 0x88, 0,
                        pSimAuthData->authContext, (unsigned int)(strlen((char*)out_put)/2), out_put));
            } else {
                cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%s0C\"",
                        channelId, (unsigned int)(10 + strlen((char*)out_put)), 0xA0, 0x88, 0x00,
                        0x00, out_put));
            }
            p_response = atSendCommandSingleline(cmd.string(), "+CGLA:");
            cmd.clear();
            free(out_put);
            out_put = NULL;
        }*/

        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_SIM_AUTH_REQ, m_slot_id);

        mipc_data->addMipcTlvUint8(MIPC_SIM_GET_SIM_AUTH_REQ_T_P2, pSimAuthData->authContext);

        if (pSimAuthData->aid != NULL) {
            mipc_data->addMipcTlvStr(MIPC_SIM_GET_SIM_AUTH_REQ_T_AID, pSimAuthData->aid);
        }
        uint16_t cmd_len = strlen((const char*)out_put);
        uint16_t bLen = hexStringToByteArrayEx(out_put, cmd_len, &byteData);
        mipc_data->addMipcTlvUint16(MIPC_SIM_GET_SIM_AUTH_REQ_T_CMD_LEN, bLen);
        mipc_data->addMipcTlv(MIPC_SIM_GET_SIM_AUTH_REQ_T_CMD_DATA, bLen, byteData); //byte_array

        callToMipcMsgAsync(msg, mipc_data);
        if (out_put != NULL) {
            free(out_put);
        }
        if (byteData != NULL) {
            free(byteData);
        }
        return;
    } while(0);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData(NULL, 0), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleSimAuthResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_IO_Response *sim_auth_response = NULL;
    unsigned char *out_put = NULL;
    char *payload = NULL;
    char tmp[256] = {0};
    RIL_Errno result = RIL_E_SIM_ERR;
    char *simResponse = NULL;

    do {
        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        logD(mTag, "handleSimAuthResponse, mipc_result:%d", mipc_result);

        if (mipc_result == MIPC_RESULT_SUCCESS) {
            uint16_t sw = mipcData->getMipcUint16Val(MIPC_SIM_GET_SIM_AUTH_CNF_T_SW, 0);

            sim_auth_response = (RIL_SIM_IO_Response*)calloc(1, sizeof(RIL_SIM_IO_Response));
            RFX_ASSERT(sim_auth_response != NULL);
            sim_auth_response->sw2 = sw & 0xFF;
            sim_auth_response->sw1 = (sw >> 8) & 0xFF;
            if (sim_auth_response->sw1 != 0x90 && sim_auth_response->sw1 != 0x91) {
                sim_auth_response->simResponse = NULL;
                logE(mTag, "handleSimAuthResponse, Fail to do SIM authentication, sw:%04x", sw);
                result = RIL_E_INVALID_ARGUMENTS;
                break;
            }

            uint16_t len = mipcData->getMipcUint16Val(MIPC_SIM_GET_SIM_AUTH_CNF_T_RSP_LEN, 0);
            uint16_t rsp_len = 0;
            payload =
                (char*) mipcData->getMipcVal(MIPC_SIM_GET_SIM_AUTH_CNF_T_RSP_DATA, &rsp_len);

            if (rsp_len > 0) {
                simResponse = (char*)byteArrayToHexString((unsigned char*)payload, rsp_len);
                rsp_len = strlen(simResponse);
            } else {
                simResponse = NULL;
            }

            if (rsp_len > 255) {
                rsp_len = 255;
            }
            if (rsp_len > 0 && simResponse != NULL) {
                memset(tmp, 0, sizeof(char)*256);
                strncpy(tmp, simResponse, rsp_len);
                out_put = base64_encode_to_str((unsigned char*)tmp, strlen(tmp));
            }

            sim_auth_response->simResponse = (char*)out_put;

            logD(mTag, "handleSimAuthResponse, rsp:%.6s...->%s, sw:%02x,%02x, rsp_len:%d, len:%d",
                    tmp,sim_auth_response->simResponse, sim_auth_response->sw1, sim_auth_response->sw2,
                    rsp_len, len);
            result = RIL_E_SUCCESS;
        } else {
            switch (mipc_result) {
                case MIPC_RESULT_MEMORY_FULL:
                    result = RIL_E_MISSING_RESOURCE;
                    break;
                case MIPC_RESULT_NOT_FOUND:
                    result = RIL_E_NO_SUCH_ELEMENT;
                    break;
                case MIPC_RESULT_BUSY:
                    result = RIL_E_SIM_BUSY;
                    break;
                default:
                    result = RIL_E_INVALID_ARGUMENTS;
                    break;
            }
        }
    } while(0);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData((void*)sim_auth_response,
            ((sim_auth_response != NULL)? sizeof(RIL_SIM_IO_Response) : 0)), msg, false);

    responseToTelCore(response);

    if (out_put != NULL) {
        free(out_put);
    }
    if (simResponse != NULL) {
        free(simResponse);
    }
    if (sim_auth_response != NULL) {
        free(sim_auth_response);
    }

}

/*
int RmmSimCommRequestHandler::openChannel(const sp < RfxMclMessage > & msg,
        RIL_SIM_IO_Response *sr, int *len) {
    sp<RfxAtResponse> p_response;
    String8 cmd("");
    int err, ril_error = RIL_E_SIM_ERR;
    RfxAtLine *line = NULL;
    char *rData = NULL;
    int cmeError = CME_SUCCESS;
    int cdmaType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_CARD_TYPE);
    RFX_UNUSED(msg);

    if ((cdmaType == CT_UIM_SIM_CARD) || (cdmaType == CT_3G_UIM_CARD)) {
        goto error;
    }
    // AT+CGLA=<sessionid>,<length>,<command>
    // <sessionid> = 0
    // <command> = 00 70 00 00 01
    cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x\"",
            0, 10, 0x00, 0x70, 0x00, 0x00, 0x01));
    p_response = atSendCommandSingleline(cmd, "+CGLA:");
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(mTag, "Fail to open channel");
        if (p_response->getFinalResponse() != NULL) {
            if (!strcmp(p_response->getFinalResponse()->getLine(), "+CME ERROR: MEMORY FULL")) {
                ril_error = RIL_E_MISSING_RESOURCE;
            } else if (!strcmp(p_response->getFinalResponse()->getLine(), "+CME ERROR: NOT FOUND")) {
                ril_error = RIL_E_NO_SUCH_ELEMENT;
            } else {
                cmeError = p_response->atGetCmeError();
                logE(mTag, "openChannel: cme_ERROR: %d", cmeError);
                switch(cmeError) {
                    // GSMA NFC: IO Error if SIM has no response.
                    case MIPC_RESULT_ERROR_UNKOWN:
                        ril_error = RIL_E_OEM_ERROR_1;
                        break;
                    default:
                        break;
                }
            }
        }
        goto error;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;
    (*len) = line->atTokNextint(&err);
    if (err < 0) goto error;
    rData = line->atTokNextstr(&err);
    if (err < 0) goto error;

    sscanf(&(rData[(*len) - 4]), "%02x%02x", &(sr->sw1), &(sr->sw2));
    rData[(*len) - 4] = '\0';
    // Remember to free it!
    asprintf(&sr->simResponse, "%s", rData);

    p_response = NULL;

    if (sr->sw1 != 0x90 && sr->sw1 != 0x91) { // wrong sw1, sw2
        if ((sr->sw1 == 0x6A && sr->sw2 == 0x82) ||
                (sr->sw1 == 0x69 && sr->sw2 == 0x85) ||
                (sr->sw1 == 0x69 && sr->sw2 == 0x99)) {
            ril_error = RIL_E_NO_SUCH_ELEMENT;
        } else if ((sr->sw1 == 0x6A && sr->sw2 == 0x84) ||
                   (sr->sw1 == 0x6A && sr->sw2 == 0x81) ||
                   (sr->sw1 == 0x68 && sr->sw2 == 0x81)) {
            logE(mTag, "Not enough memory space in the file");
            ril_error = RIL_E_MISSING_RESOURCE;
        }
    } else {
        ril_error = RIL_E_SUCCESS;
    }

    return ril_error;

error:
    sr->sw1 = 0x6f;
    sr->sw2 = 0x00;
    sr->simResponse = NULL;

    return ril_error;
}*/

int RmmSimCommRequestHandler::checkRetryFCI(int sw1, int sw2) {
    int retry = 0;
    if (sw1 != 0x61 && sw1 != 0x91 && sw1 != 0x9F) {
        retry = 1;
    }

    if ((sw1 == 0x63 && sw2 == 0x10) || (sw1 == 0x92 && sw2 == 0x40)
            || (sw1 == 0x90 && sw2 == 0x00)) {
        retry = 0;
    }
    logD(mTag, "checkRetryFCI, sw1 %d sw2 %d retry %d", sw1, sw2, retry);
    return retry;
}
/*
int RmmSimCommRequestHandler::selectAid(RIL_SIM_IO_Response *sr, int *len, int channel,
        int p2, char* pAid) {
    sp<RfxAtResponse> p_response;
    String8 cmd("");
    int err, ril_error = RIL_E_SIM_ERR;
    RfxAtLine *line = NULL;
    char *rData = NULL;
    // Parameters
    char *aid = pAid;
    int length = (10 + strlen(aid));
    int cla = ((channel > 3)? 0x40|(channel-4) : channel);
    int ins = 0xA4;
    int p1 = 0x04;
    int mP2 = p2;
    int p3 = (strlen(aid)/2);
    bool retry = false;
    int cmeError = CME_SUCCESS;

redo:
    // Reset pointer
    *len = 0;
    rData = NULL;

    // Reset the response in the sr
    if (sr->simResponse != NULL) {
        free(sr->simResponse);
        sr->simResponse = NULL;
    }

    if (aid != NULL) {
        if(ins == 0xA4 && mP2==0x04) {
            cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x%s00\"",
                    channel, length+2, cla, ins, p1, mP2, p3, aid));
        } else {
            cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x%s\"",
                    channel, length, cla, ins, p1, mP2, p3, aid));
        }
    } else {
        if(ins == 0xA4 && mP2==0x04) {
            cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x00\"",
                    channel, length+2, cla, ins, p1, mP2, p3));
        } else {
            cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x\"",
                    channel, length, cla, ins, p1, mP2, p3));
        }
    }
    logD(mTag, "selectAid, cmd: %s", cmd.string());

    p_response = atSendCommandSingleline(cmd, "+CGLA:");
    err = p_response->getError();

    if (err < 0 || p_response->getSuccess() == 0) {
        logE(mTag, "Fail to open channel");
        if (p_response->getFinalResponse() != NULL) {
            if (!strcmp(p_response->getFinalResponse()->getLine(), "+CME ERROR: MEMORY FULL")) {
                ril_error = RIL_E_MISSING_RESOURCE;
            } else if (!strcmp(p_response->getFinalResponse()->getLine(), "+CME ERROR: NOT FOUND")) {
                ril_error = RIL_E_NO_SUCH_ELEMENT;
            } else {
                cmeError = p_response->atGetCmeError();
                logE(mTag, "openChannel: cme_ERROR: %d", cmeError);
                switch(cmeError) {
                    // GSMA NFC: IO Error if SIM has no response.
                    case MIPC_RESULT_ERROR_UNKOWN:
                        ril_error = RIL_E_OEM_ERROR_1;
                        break;
                    default:
                        break;
                }
            }
        }
        goto error;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;
    (*len) = line->atTokNextint(&err);
    if (err < 0) goto error;
    rData = line->atTokNextstr(&err);
    if (err < 0) goto error;

    sscanf(&(rData[(*len) - 4]), "%02x%02x", &(sr->sw1), &(sr->sw2));
    rData[(*len) - 4] = '\0';
    // Remember to free it!
    asprintf(&sr->simResponse, "%s", rData);

    p_response = NULL;
    cmd.clear();

    if(sr->sw1 == 0x61) { // GET RESPONSE setting p3 = sr.sw2
        // GET RESPONSE for selecting AID if T=0
        // AT+CGLA=<sessionid>,<length>,<command>
        // <length> = 10
        // <command> = 0X C0 00 00 sr.sw2
        // X = channel
        length = 10;
        ins = 0xC0;
        p1 = 0x00;
        mP2 = 0x00;
        p3 = sr->sw2;
        aid = NULL;
        goto redo;
    } else if(sr->sw1 == 0x6C) { // resend previous command header setting p3 = sr.sw2
        p3 = sr->sw2;
        goto redo;
    } else if (sr->sw1 != 0x90 && sr->sw1 != 0x91) { // wrong sw1, sw2
        if ((sr->sw1 == 0x6A && sr->sw2 == 0x82) ||
                (sr->sw1 == 0x6A && sr->sw2 == 0x81) ||
                (sr->sw1 == 0x69 && sr->sw2 == 0x85) ||
                (sr->sw1 == 0x69 && sr->sw2 == 0x99)) {
            ril_error = RIL_E_NO_SUCH_ELEMENT;
        } else if ((sr->sw1 == 0x6A && sr->sw2 == 0x84) ||
                   (sr->sw1 == 0x68 && sr->sw2 == 0x81)) {
            logE(mTag, "Not enough memory space in the file");
            ril_error = RIL_E_MISSING_RESOURCE;
        }
        if (ins == 0xA4 && aid != NULL && (mP2 == 0x04) && !retry) {
            // Retry only for "Select AID"
            retry = true;
            if (checkRetryFCI(sr->sw1, sr->sw2) == 1) {
                mP2 = 0;
                goto redo;
            }
        }
        // GSMA NFC test: We should treat warning code 0x62 or 0x63 as
        // success.
        if (sr->sw1 == 0x62 || sr->sw1 == 0x63) {
            logI(mTag, "0x62 || 0x63 warning code.");
            ril_error = RIL_E_SUCCESS;
        } else {
            return ril_error;
        }
    } else {
        ril_error = RIL_E_SUCCESS;
    }

    return ril_error;
error:
    sr->sw1 = 0x6f;
    sr->sw2 = 0x00;
    sr->simResponse = NULL;

    return ril_error;
}  */

// Hexa To Int
/*
unsigned int RmmSimCommRequestHandler::htoi(char* szHex) {
    unsigned int hex = 0;
    int nibble;
    while (*szHex) {
        hex <<= 4;

        if (*szHex >= '0' && *szHex <= '9') {
            nibble = *szHex - '0';
        } else if (*szHex >= 'a' && *szHex <= 'f') {
            nibble = *szHex - 'a' + 10;
        } else if (*szHex >= 'A' && *szHex <= 'F') {
            nibble = *szHex - 'A' + 10;
        } else {
            nibble = 0;
        }

        hex |= nibble;

        szHex++;
    }

    return hex;
}
*/

void RmmSimCommRequestHandler::handleSimOpenChannel(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_SIM_IO_Response sr;
    RIL_Errno ril_error = RIL_E_GENERIC_FAILURE;
    int channel = 0;
    int *finalRsp = NULL;
    char *pAid = NULL;
    RIL_OpenChannelParams *openChannelParams = NULL;
    int p2 = 0;
    sp<RfxMipcData> mipc_data = NULL;
    unsigned char *byteData = NULL;

    openChannelParams = (RIL_OpenChannelParams *)(msg->getData()->getData());
    if (openChannelParams != NULL) {
        pAid = openChannelParams->aidPtr;
        if (openChannelParams->p2 > 0) {
            p2 = openChannelParams->p2;
        }
    }

    // If p2 is invalid as TS 102 221 Table 11.2 defined.
    if ((pAid == NULL) && (p2 != 0) && ((p2 & 0x80) > 0 || ((p2 & 0x40) > 0 && (p2 & 0x20) > 0)
            || ((p2 & 0x40) == 0 && (p2 & 0x20) > 0) || (p2 & 0x10) > 0 || (p2 & 0x04) == 0)) {
        logI(mTag, "handleSimOpenChannel, invalid p2");
        sr.sw1 = 0xff;
        sr.sw2 = 0xff;
        sr.simResponse = NULL;
        ril_error = RIL_E_SIM_ERR;
        goto error;
    }

    logD(mTag, "handleSimOpenChannel, token:%d, pAid:%s, p2:%d", msg->getToken(), pAid, p2);

    // Open channel
    //ril_error = openChannel(msg, &sr, &len);
    //selectAid(&sr, &len, channel, p2, pAid);
    mipc_data =  RfxMipcData::obtainMipcData(MIPC_SIM_OPEN_CHANNEL_REQ, m_slot_id);
    if (pAid != NULL) {
        uint16_t len = strlen(pAid);
        uint16_t bLen = hexStringToByteArrayEx((unsigned char*)pAid, len, (unsigned char **)&byteData);
        mipc_data->addMipcTlvUint8(MIPC_SIM_OPEN_CHANNEL_REQ_T_APP_ID_LEN, bLen);
        mipc_data->addMipcTlv(MIPC_SIM_OPEN_CHANNEL_REQ_T_APP_ID, bLen, byteData);
    } else {
        mipc_data->addMipcTlvUint8(MIPC_SIM_OPEN_CHANNEL_REQ_T_APP_ID_LEN, 0);
    }
    mipc_data->addMipcTlvUint8(MIPC_SIM_OPEN_CHANNEL_REQ_T_P2, (uint8_t)p2);
    callToMipcMsgAsync(msg, mipc_data);
    if (byteData != NULL) {
        free(byteData);
    }
    return;

error:
    logE(mTag, "handleSimOpenChannel, error:%d, sw1:%x, sw2:%x, sr.simresponse %s",
        ril_error, sr.sw1, sr.sw2, sr.simResponse);

    finalRsp = (int*)calloc(1, sizeof(int) * 3);
    RFX_ASSERT(finalRsp != NULL);
    finalRsp[0] = channel;
    finalRsp[1] = sr.sw1;
    finalRsp[2] = sr.sw2;

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxIntsData((void*)finalRsp, 3*sizeof(int)), msg, false);

    responseToTelCore(response);

    free(finalRsp);
}

void RmmSimCommRequestHandler::handleSimOpenChannelResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_SIM_IO_Response sr;
    RIL_Errno ril_error = RIL_E_GENERIC_FAILURE;
    int channel = 0;
    int i = 0;
    int *finalRsp = NULL;

    //openChannel(msg, &sr, &len);
    //selectAid(&sr, &len, channel, p2, pAid); //??retry, error code TODO
    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    channel = (int) mipcData->getMipcUint8Val(MIPC_SIM_OPEN_CHANNEL_CNF_T_CHANNEL, 0);
    int sw = (int) mipcData->getMipcUint16Val(MIPC_SIM_OPEN_CHANNEL_CNF_T_SW, 0);
    sr.sw2 = sw & 0xFF;
    sr.sw1 = (sw >> 8) & 0xFF;
    sr.simResponse = NULL;
    uint16_t len = 0;
    char* simResponse = (char*) mipcData->getMipcVal(MIPC_SIM_OPEN_CHANNEL_CNF_T_RESP, &len);

    if (len > 0) { //simResponse not include sw
        sr.simResponse = (char*)byteArrayToHexString((unsigned char*)simResponse, len);

        finalRsp = (int*)calloc(1, sizeof(int) * (len + 3));
        RFX_ASSERT(finalRsp != NULL);
        for(i = 0; i < len /*- 2*/; i++) {
            int n = sscanf(&(sr.simResponse[i * 2]), "%02x", &finalRsp[i + 1]);
            if (n <= 0) {
                logE(mTag, "handleSimOpenChannelResponse, sscanf:%d,%d,%d", n, i, len);
            }
        }
    } else {
        //logI(mTag, "handleSimOpenChannelResponse, simResponse is null.");
        finalRsp = (int*)calloc(1, sizeof(int) * 3);
        RFX_ASSERT(finalRsp != NULL);
    }

    logD(mTag,
            "handleSimOpenChannelResponse, token:%d, result:%d, channel:%x, sw1:%x, sw2:%x, rsp:%d,%s, i:%d",
            msg->getToken(), mipc_result, channel, sr.sw1, sr.sw2, len, sr.simResponse, i);

    // Open channel successfully
    if (mipc_result == MIPC_RESULT_SUCCESS) {
        if(channel < 1 || channel > 19) {
            sr.sw1 = 0xff;
            sr.sw2 = 0xff;
            ril_error = RIL_E_SIM_ERR;
        } else {
            ril_error = RIL_E_SUCCESS;
        }
    } else if (mipc_result == MIPC_RESULT_NOT_FOUND || mipc_result == MIPC_RESULT_MEMORY_FULL || mipc_result == MIPC_RESULT_ERROR_UNKOWN) {
        if (mipc_result == MIPC_RESULT_MEMORY_FULL) {
            ril_error = RIL_E_MISSING_RESOURCE;
        } else if (mipc_result == MIPC_RESULT_NOT_FOUND) {
           ril_error = RIL_E_NO_SUCH_ELEMENT;
        } else if (mipc_result == MIPC_RESULT_ERROR_UNKOWN) {
           ril_error = RIL_E_OEM_ERROR_1;
        }
    } else {
        sr.sw1 = 0xff;
        sr.sw2 = 0xff;
    }

    if (finalRsp != NULL) {
        finalRsp[0] = channel;
        finalRsp[i + 1] = sr.sw1;
        finalRsp[i + 2] = sr.sw2;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxIntsData((void*)finalRsp, (i + 3)*sizeof(int)), msg, false);
    responseToTelCore(response);
    if (sr.simResponse != NULL) {
        free(sr.simResponse);
    }
    if (finalRsp != NULL) {
        free(finalRsp);
    }
}

void RmmSimCommRequestHandler::handleSimCloseChannel(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    int* params = (int*)(msg->getData()->getData());

    if (params == NULL || params[0] == 0) {
        logE(mTag, "handleSimCloseChannel, close channel 0 is not allowed.");
        response = RfxMclMessage::obtainResponse(
                msg->getId(),
                RIL_E_INVALID_ARGUMENTS,
                RfxVoidData(),
                msg,
                false);
        responseToTelCore(response);
    } else {
        logD(mTag, "handleSimCloseChannel, token:%d, channel: %d", msg->getToken(), params[0]);

        //cmd.append(String8::format("AT+CCHC=%d", (*channel)));
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_CLOSE_CHANNEL_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_CLOSE_CHANNEL_REQ_T_CHANNEL_ID, params[0]);
        if (params[1] > 0) {
            mipc_data->addMipcTlvUint8(MIPC_SIM_CLOSE_CHANNEL_REQ_T_IS_ES10, 1);
        }
        callToMipcMsgAsync(msg, mipc_data);
    }
}

void RmmSimCommRequestHandler::handleSimCloseChannelResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    logD(mTag, "handleSimCloseChannelResponse, token:%d, mipc_result: %d", msg->getToken(), mipc_result);

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        switch(mipc_result) {
            // GSMA NFC: IO Error if SIM has no response.
            case MIPC_RESULT_ERROR_UNKOWN:
                ril_error = RIL_E_OEM_ERROR_1;
                break;
            default:
                ril_error = RIL_E_GENERIC_FAILURE;
                break;
        }
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleSwitchSimType(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    int* params = (int*)(msg->getData()->getData());

    if (params == NULL || params[0] > 1 || params[0] < 0) {
        logE(mTag, "handleSwitchSimType, invalid switching type.");
        response = RfxMclMessage::obtainResponse(
                msg->getId(),
                RIL_E_INVALID_ARGUMENTS,
                RfxVoidData(),
                msg,
                false);
        responseToTelCore(response);
    } else {
        logD(mTag, "handleSimSwitchType, token:%d, channel: %d", msg->getToken(), params[0]);

        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_ESIM_SWITCH_SET_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_ESIM_SWITCH_SET_REQ_T_MODE, params[0]);
        callToMipcMsgAsync(msg, mipc_data);
    }
}

void RmmSimCommRequestHandler::handleSwitchSimTypeResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    logD(mTag, "handleSwitchSimTypeResponse, token:%d, mipc_result: %d", msg->getToken(), mipc_result);

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        switch(mipc_result) {
            case MIPC_RESULT_ERROR_NOT_FOUND:
                ril_error = RIL_E_REQUEST_NOT_SUPPORTED;
                break;
            default:
                ril_error = RIL_E_GENERIC_FAILURE;
                break;
        }
        // Need to trigger event to abort user response
        sendEvent(RFX_MSG_EVENT_GET_SIM_TYPE_INFO, RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleDetectSim(const sp < RfxMclMessage > & msg) {
    //p_response = atSendCommandSingleline("AT+CPIN?", "+CPIN:");
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_STATUS_REQ, m_slot_id);
    mipc_data->addMipcTlvUint8(MIPC_SIM_STATUS_REQ_T_MODE, 0);
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleDetectSimResponse(const sp < RfxMclMessage > & msg) {
    //char* urc = (char *) msg->getData()->getData();
    int cardPresent = 0;
    int mipc_result = MIPC_RESULT_SUCCESS;
    int uicc_status = UICC_ABSENT;

    do {
        //p_response = atSendCommandSingleline("AT+CPIN?", "+CPIN:");
        RfxMipcData* rsp_Data = msg->getMipcData();
        mipc_result = rsp_Data->getResult();

        logD(mTag, "handleDetectSimResponse, mipc_result:%d", mipc_result);

        //if (err != 0) {
        //    logE(mTag, "handleDetectSimResponse: err: %d", err);
        //    cardPresent = 0;
        //    break;
        //}
        if (mipc_result != MIPC_RESULT_SUCCESS) {
            switch (mipc_result) {
                case MIPC_RESULT_BUSY: // ??? try to do again until more than 20 times
                    cardPresent = 14; // SIM busy, framework needs to query again
                    break;
                case MIPC_RESULT_SIM_EXT_SIM_FAILURE:
                case MIPC_RESULT_SIM_NOT_INSERTED:
                    cardPresent = 0;
                    break;
                case MIPC_RESULT_BAD_SIM:
                    cardPresent = 1;
                    break;
                default:
                    cardPresent = 1; // NOT_READY
                    break;
            }
        } else {
            uint8_t temp_uint8 = rsp_Data->getMipcUint8Val(MIPC_SIM_STATUS_CNF_T_STATUS, 0xff);
            uicc_status = getUiccStatusFromMipc(temp_uint8);
            logD(mTag, "handleDetectSimResponse: success, uicc_status: %d", uicc_status);

            if (uicc_status == UICC_ABSENT) { //??? TODO
                cardPresent = 0;
            } else if (uicc_status == UICC_BUSY) {
                cardPresent = 14;
            } else {
                cardPresent = 1;
            }
            //if (strcmp(cpinResult, "SIM PIN") && strcmp(cpinResult, "SIM PUK")
            //        && strcmp(cpinResult, "PH-NET PIN") && strcmp(cpinResult, "PH-NET PUK")
            //        && strcmp(cpinResult, "PH-NETSUB PIN") && strcmp(cpinResult, "PH-NETSUB PUK")
            //        && strcmp(cpinResult, "PH-SP PIN") && strcmp(cpinResult, "PH-SP PUK")
            //        && strcmp(cpinResult, "PH-CORP PIN") && strcmp(cpinResult, "PH-CORP PUK")
            //        && strcmp(cpinResult, "PH-FSIM PIN") && strcmp(cpinResult, "PH-FSIM PUK")
            //        && strcmp(cpinResult, "READY") && strcmp(cpinResult, "SIM PIN-DEFER")) {
            //    logD(mTag, "handleDetectSimResponse: out of strcmp: %s", cpinResult);
            //    cardPresent = 0;
            //} else {
            //    cardPresent = 1;
            //}
            break;
        }

    } while(0);

    mtk_release_wake_lock("sim_hot_plug");
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);

    switch (cardPresent) {
        case 1:
            /* Card inserted */
            logD(mTag, "handleDetectSimResponse, check EUSIM (%d) in case of event plug in", cardType);

            if (cardType & RFX_CARD_TYPE_SIM || cardType & RFX_CARD_TYPE_USIM) {
                // External SIM [Start]
                if ((isCommontSlotSupport() == true)
                        && (RmmVsimRequestHandler::getVsimPlugInOutEvent(m_slot_id) ==
                        VSIM_TRIGGER_PLUG_IN)) {
                    logD(mTag, "vsim trigger plug in on common slot project.");
                    RmmVsimRequestHandler::setVsimPlugInOutEvent(m_slot_id, VSIM_TRIGGER_RESET);
                } else if ((isCommontSlotSupport() == true) && RfxRilUtils::isVsimEnabled()) {
                    logD(mTag, "handleDetectSimResponse, Ignore plug in if vsim enabled on common slot project");
                } else {
                // External SIM [End]
                    /// M: sim recovery end, don't report sim plug-in. @{
                    int esim_cause =
                            getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ESIMS_CAUSE);
                    if (14 != esim_cause) {
                        sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_PLUG_IN,
                                m_slot_id, RfxVoidData());
                        responseToTelCore(unsol);
                    }
                    /// @}
                }
            } else if ((cardType == -1 || cardType == 0)
                    && mipc_result != MIPC_RESULT_BAD_SIM
                    && uicc_status != UICC_EMPTY_EUICC) {
                // Wait for +EUSIM
                mtk_acquire_wake_lock("sim_hot_plug");
                sendEvent(RFX_MSG_EVENT_SIM_DETECT_SIM, RfxStringData(), RIL_CMD_PROXY_1, m_slot_id);
            }
            break;
        case 0:
        {
            sp<RfxMclMessage> unsol = NULL;
            String8 iccId(PROPERTY_ICCID_PREIFX);
            logD(mTag, "handleDetectSimResponse, sim plug, but no SIM inserted actually.(%d)", cardType);
            if (cardType != 0) {
                logE(mTag, "handleDetectSimResponse, It should not happen. cardtype = %d.", cardType);
            }
            if (isCommontSlotSupport()) {
                bool aospEnabled = isEnableUiccApplicationsSupported();
                if ((!aospEnabled && (getMclStatusManager()->getIntValue(
                        RFX_STATUS_KEY_SIM_ONOFF_STATE) != SIM_POWER_STATE_SIM_OFF))
                        || (aospEnabled && (getMclStatusManager()->getIntValue(
                        RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
                        == UICC_APPLICATIONS_ENABLE_STATE_ENABLED))) {
                    iccId.append(String8::format("%d", (m_slot_id + 1)));
                    //rfx_property_set(iccId.string(), "N/A");
                    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8("N/A"));
                    sendIccidChanged("N/A");
                }
            }
        }
            break;
        case 14:
            logD(mTag, "handleDetectSimResponse, detect sim and sim busy so try again");
            mtk_acquire_wake_lock("sim_hot_plug");
            sendEvent(RFX_MSG_EVENT_SIM_DETECT_SIM, RfxStringData(), RIL_CMD_PROXY_1, m_slot_id,
                -1, -1, 200000000);
            break;
        default:
            logE(mTag, "handleDetectSimResponse, Event plug in was happened, but error status");
            break;
    }
    return;
}

void RmmSimCommRequestHandler::handleSimTransmitApdu(const sp < RfxMclMessage > & msg,
        bool isBasicChannel) {
    RIL_SIM_APDU *p_args = (RIL_SIM_APDU *)(msg->getData()->getData());

    /*
    if (p_args->instruction == 0xE2 &&
        getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_ECGLA_CAPABILITY, false) &&
        getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_IS_EUICC_CARD, false)) {
        handleEsimTransmitChannel(msg);
        return;
    }
    char sim_extended_format_prop[RFX_PROPERTY_VALUE_MAX] = {0};
    String8 prop("vendor.gsm.sim.extended.format");
    prop.append(String8::format("%d", (m_slot_id+1)));
    rfx_property_get(prop.string(), sim_extended_format_prop, "0");
    logI(mTag, "handleSimTransmitApdu sim_extended_format_prop: %d",
            atoi(sim_extended_format_prop));
    if (!strncmp(sim_extended_format_prop, "1", 1)) {
        handleSimTransmitWithExtendedAPDU(msg, false);
        // +EXCGLA=<offset>,<sessionid>,<total_length>,<current_length>,<current_command_data>
        // +EXCGLA: <offset>,<total_length>,<current_length>,<current_response_data>
        return;
    }*/

    // Set class byte according TS 102.221
    if (isBasicChannel == true) {
        p_args->sessionid = 0;
    } else {
        // Set class byte according TS 102.221
        p_args->cla = setChannelToClassByte(p_args->cla, p_args->sessionid);
    }

    /*
    if ((p_args->data == NULL) || (strlen(p_args->data) == 0)) {
       logD(mTag, "handleSimTransmitApdu p3 = %d", p_args->p3);
       if (p_args->p3 < 0) {
           cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x\"",
                    p_args->sessionid, 8, p_args->cla, p_args->instruction,
                    p_args->p1, p_args->p2));
       } else {
           cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x\"",
                    p_args->sessionid, 10, p_args->cla, p_args->instruction,
                    p_args->p1, p_args->p2, p_args->p3));
       }
    } else {
        cmd.append(String8::format("AT+CGLA=%d,%d,\"%02x%02x%02x%02x%02x%s\"",
                p_args->sessionid,
                (unsigned int)(10 + strlen(p_args->data)), p_args->cla, p_args->instruction,
                p_args->p1, p_args->p2, p_args->p3,
                p_args->data));
    }*/

    logD(mTag, "handleSimTransmitApdu, token:%d,session:%d,cla:%x,ins:%x,p1:%x,p2:%d,p3:%d, data:%.10s...",
            msg->getToken(), p_args->sessionid, p_args->cla, p_args->instruction, p_args->p1,
            p_args->p2, p_args->p3, p_args->data);

    char *byteData = NULL;
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ, m_slot_id);
    mipc_data->addMipcTlvUint8(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_SESSION_ID, (uint8_t)p_args->sessionid);
    mipc_data->addMipcTlvUint8(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_CLA, (uint8_t)p_args->cla);
    mipc_data->addMipcTlvUint8(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_INS, (uint8_t)p_args->instruction);
    mipc_data->addMipcTlvUint8(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P1, (uint8_t)p_args->p1);
    mipc_data->addMipcTlvUint8(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P2, (uint8_t)p_args->p2);
    if (p_args->p3 >= 0) {
        mipc_data->addMipcTlvUint8(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P3, (uint8_t)p_args->p3);
    }
    if (p_args->data != NULL ) {
        uint16_t len = strlen(p_args->data);
        if (len > 0) {
            uint16_t len = strlen(p_args->data);
            uint16_t bLen = hexStringToByteArrayEx((unsigned char*)p_args->data, len, (unsigned char **)&byteData);
            mipc_data->addMipcTlvUint16(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_DATA_LEN, bLen);
            mipc_data->addMipcTlv(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_DATA, bLen, byteData);
        }
    } else {
        mipc_data->addMipcTlvUint16(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_DATA_LEN, 0);
    }
    if (p_args->isEs10) {
        mipc_data->addMipcTlvUint8(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_IS_ES10, 1);
    }
    callToMipcMsgAsync(msg, mipc_data);
    if (byteData != NULL) {
        free(byteData);
    }
}

void RmmSimCommRequestHandler::handleSimTransmitApduResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_SIM_IO_Response sr;
    RIL_Errno ril_error = RIL_E_SIM_ERR;

    //p_response = atSendCommandSingleline(cmd, "+CGLA:");
    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleSimTransmitApduResponse, token:%d, mipc_result:%d",
                msg->getToken(), mipc_result);
        ril_error = RIL_E_GENERIC_FAILURE;
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxSimIoRspData(NULL, 0), msg, false);
        responseToTelCore(response);
        return;
    }

    uint16_t sw = mipcData->getMipcUint16Val(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_SW, 0);
    sr.sw2 = sw & 0xFF;
    sr.sw1 = (sw >> 8) & 0xFF;

    //uint16_t temp_rsp_len = mipcData->getMipcUint16Val(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_LEN, 0);
    uint16_t a_len = 0;
    char* simResponse = (char*) mipcData->getMipcVal(MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_APDU, &a_len);
    if (a_len > 0) {
        sr.simResponse = (char*)byteArrayToHexString((unsigned char*)simResponse, a_len);
    } else {
        sr.simResponse = NULL;
    }

    logD(mTag, "handleSimTransmitApduResponse, token:%d, rsp = %02x%02x,%d:%.10s...",
            msg->getToken(), sr.sw1, sr.sw2, a_len, sr.simResponse);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg, false);
    responseToTelCore(response);
    if (sr.simResponse != NULL) {
        free(sr.simResponse);
    }
}

/**
 * Returns a copy of the given CLA byte where the channel number bits are
 * set as specified by the given channel number See GlobalPlatform Card
 * Specification 2.2.0.7: 11.1.4 Class Byte Coding.
 *
 * @param cla the CLA byte. Won't be modified
 * @param channelNumber within [0..3] (for first interindustry class byte
 *            coding) or [4..19] (for further interindustry class byte
 *            coding)
 * @return the CLA byte with set channel number bits. The seventh bit
 *         indicating the used coding (first/further interindustry class
 *         byte coding) might be modified
 */
 int RmmSimCommRequestHandler::setChannelToClassByte(int cla, int channelNumber) {
    if (channelNumber < 4) {
        // b7 = 0 indicates the first interindustry class byte coding
        cla = ((cla & 0xBC) | channelNumber);

    } else if (channelNumber < 20) {
        // b7 = 1 indicates the further interindustry class byte coding
        // bool isSM = (cla & 0x0C) != 0; //shall check 0x20 for extend channel
        cla = ((cla & 0xB0) | 0x40 | (channelNumber - 4));
        // if (isSM) {
        //    cla |= 0x20;
        // }
    }
    return cla;
}

void RmmSimCommRequestHandler::handleSimGetAtr(const sp < RfxMclMessage > & msg) {
    //atSendCommandSingleline("AT+ESIMINFO=0", "+ESIMINFO:");
    logD(mTag, "handleSimGetAtr");

    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_ATR_INFO_REQ, m_slot_id);
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleSimGetAtrResponse(const sp < RfxMclMessage > & msg) {
    // +ESIMINFO:<mode>, <data> => if<mode>=0,<data>=ATR Hex String
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SIM_ERR;
    char *atr = NULL;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result == MIPC_RESULT_SUCCESS) {
        uint32_t len = mipcData->getMipcUint32Val(MIPC_SIM_GET_ATR_INFO_CNF_T_ATR_LEN, 0);
        uint16_t a_len = 0;
        if (len > 0) {
            atr = (char*) mipcData->getMipcVal(MIPC_SIM_GET_ATR_INFO_CNF_T_ATR, &a_len);
        }

        logD(mTag, "handleSimGetAtrResponse, strlen is %d, len:%d", a_len, len);

        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                    RfxStringData((void*)atr, a_len), msg, false);
        responseToTelCore(response);
    } else {
        logD(mTag, "handleSimGetAtrResponse, Fail, mipc_result:%d", mipc_result);
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                    RfxStringData(), msg, false);
        responseToTelCore(response);
    }
}

void RmmSimCommRequestHandler::handleSimGetIccid(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SIM_ERR;

    String8 iccid = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

    logD(mTag, "handleSimGetIccid, %.6s", iccid.string());

    if (!iccid.isEmpty()) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringData((void *)iccid.string(), iccid.length()), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error, RfxStringData(), msg,
                false);
    }

    responseToTelCore(response);
}
void RmmSimCommRequestHandler::handleEventQueryIccid(const sp < RfxMclMessage > & msg) {
    //atSendCommandSingleline("AT+ICCID?", "+ICCID:");
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_SIM_ICCID_REQ, m_slot_id);
    callToMipcMsgAsync(msg, req_data);
}

void RmmSimCommRequestHandler::handleEventQueryIccidResponse(const sp < RfxMclMessage > & msg) {
    //RFX_UNUSED(msg);
    char *p_iccid = NULL;
    String8 iccidKey("");
    bool skipUpdateCardType = false;

    do {
        //atSendCommandSingleline("AT+ICCID?", "+ICCID:");
        RfxMipcData* mipc_data = msg->getMipcData();
        int  mipc_result = mipc_data->getResult();

        if (mipc_result == MIPC_RESULT_SUCCESS) {
            uint16_t i_len = 0;
            char *iccId = (char *) mipc_data->getMipcVal(MIPC_SIM_ICCID_CNF_T_ICCID, &i_len);
            p_iccid = (char *)calloc(1, (sizeof(char)* (i_len + 1)));
            RFX_ASSERT(p_iccid != NULL);
            memcpy(p_iccid, iccId, i_len);
            p_iccid[i_len] = '\0';

            // Convert all the lower case characters in iccId to upper case
            stringToUpper(p_iccid);
            iccidKey.append(String8::format("%s", p_iccid));
            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, iccidKey);
            sendIccidChanged(p_iccid);

            logD(mTag, "handleEventQueryIccidResponse, result:%d, iccid:%d,%.7s",
                    mipc_result, i_len, p_iccid);


            if (isAOSPPropSupport()) {
                String8 iccidPre("gsm.sim.preiccid_");
                String8 tempIccid(p_iccid);
                iccidPre.append(String8::format("%d", m_slot_id));
                const char *tempStr = tempIccid.string();
                if (tempStr != NULL) {
                    // close by default, can be customized to enable
                    rfx_property_set(iccidPre, string(tempStr).substr(0, 7).c_str());
                } else {
                    logE(mTag, "handleEventQueryIccidResponse, error, empty iccid");
                }
            }
        } else if (mipc_result == MIPC_RESULT_SIM_NOT_INSERTED) {
            bool aospEnabled = isEnableUiccApplicationsSupported();

            if ((!aospEnabled && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
                    != SIM_POWER_STATE_SIM_OFF))
                    || (aospEnabled && (getMclStatusManager()->getIntValue(
                    RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE)
                    != UICC_APPLICATIONS_ENABLE_STATE_DISABLED))) {
                logD(mTag, "handleEventQueryIccidResponse, result:%d, SIM card is absent", mipc_result);

                getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIM_ICCID, String8("N/A"));
                sendIccidChanged("N/A");
            } else {
                // sim power off case, can't get iccid, shall not update cdma card type to not insert.
                skipUpdateCardType = true;
                logD(mTag, "handleEventQueryIccidResponse, result:%d, SIM card is off", mipc_result);
            }
        } else if (mipc_result == MIPC_RESULT_BUSY) {
            logD(mTag, "handleEventQueryIccidResponse, result:%d, sim busy and retry", mipc_result);

            // sleepMsec(200);
            sendEvent(msg->getId(), RfxVoidData(), msg->getChannelId(), m_slot_id,
                    -1, -1, ms2ns(100), msg->getPriority());
            return;
        }
    } while(0); //ret == RIL_E_SIM_BUSY

    //RLOGD("queryIccId before release_wake_lock");
    mtk_release_wake_lock("sim_hot_plug");

    //logD(mTag, "handleEventQueryIccidResponse, shall has RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(unsol);

    RmmSimPinPukCount *retry = getPinPukRetryCount();
    if (retry != NULL) {
        free(retry);
    }

    // SIM Extended APDU.
    //querySimExtendedFormat();

    // Generate CDMA card type.
    if (!skipUpdateCardType) {
        updateCdmaCardType(p_iccid);
    }
    if (p_iccid != NULL) {
        free(p_iccid);
    }
}

void RmmSimCommRequestHandler::handleSecurityOperation(const sp<RfxMclMessage>& msg, UICC_Security_Operation op) {
    RfxBaseData *rbd = msg->getData();
    const char**  strings = (const char**)rbd->getData();
    size_t dataLen = rbd->getDataLength();
    int opRetryCount = -1;
    RIL_Errno result = RIL_E_GENERIC_FAILURE;
    int err = 0;

    do {
        sp<RfxMipcData> mipc_data = NULL;

        switch (op) {
            case ENTER_PIN1:
                //cmd.append(String8::format("AT+CPIN=\"%s\"", strings[0]));
                 mipc_data =  RfxMipcData::obtainMipcData(MIPC_SIM_VERIFY_PIN_REQ, m_slot_id);
                mipc_data->addMipcTlvUint8(MIPC_SIM_VERIFY_PIN_REQ_T_PIN_TYPE, MIPC_SIM_PIN_TYPE_PIN1);
                mipc_data->addMipcTlvStr(MIPC_SIM_VERIFY_PIN_REQ_T_PIN_CODE, strings[0]);
                break;
            case ENTER_PIN2:
                //cmd.append(String8::format("AT+EPIN2=\"%s\"", strings[0]));
                mipc_data =  RfxMipcData::obtainMipcData(MIPC_SIM_VERIFY_PIN_REQ, m_slot_id);
                mipc_data->addMipcTlvUint8(MIPC_SIM_VERIFY_PIN_REQ_T_PIN_TYPE, MIPC_SIM_PIN_TYPE_PIN2);
                mipc_data->addMipcTlvStr(MIPC_SIM_VERIFY_PIN_REQ_T_PIN_CODE, strings[0]);
                break;
            case ENTER_PUK1:
                //cmd.append(String8::format("AT+EPIN1=\"%s\",\"%s\"", strings[0], strings[1]));
                mipc_data =  RfxMipcData::obtainMipcData(MIPC_SIM_UNBLOCK_PIN_REQ, m_slot_id);
                mipc_data->addMipcTlvUint8(MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_TYPE, MIPC_SIM_PIN_TYPE_PUK1);
                mipc_data->addMipcTlvStr(MIPC_SIM_UNBLOCK_PIN_REQ_T_PUK_CODE, strings[0]);
                mipc_data->addMipcTlvStr(MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_CODE, strings[1]);
                break;
            case ENTER_PUK2:
                //cmd.append(String8::format("AT+EPIN2=\"%s\",\"%s\"", strings[0], strings[1]));
                mipc_data =  RfxMipcData::obtainMipcData(MIPC_SIM_UNBLOCK_PIN_REQ, m_slot_id);
                mipc_data->addMipcTlvUint8(MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_TYPE, MIPC_SIM_PIN_TYPE_PUK2);
                mipc_data->addMipcTlvStr(MIPC_SIM_UNBLOCK_PIN_REQ_T_PUK_CODE, strings[0]);
                mipc_data->addMipcTlvStr(MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_CODE, strings[1]);
                break;
            case CHANGE_PIN1:
                /* Check if SIM pin has enabled */
                //cmd.append(String8::format("AT+CLCK=\"SC\",2"));
                if (dataLen == 3 * sizeof(char*)) {
                    mipc_data =  RfxMipcData::obtainMipcData(MIPC_SIM_CHANGE_PIN_REQ, m_slot_id);
                    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANGE_PIN_REQ_T_PIN_TYPE, MIPC_SIM_PIN_TYPE_PIN1);
                    mipc_data->addMipcTlvStr(MIPC_SIM_CHANGE_PIN_REQ_T_OLD_PIN, strings[0]);
                    mipc_data->addMipcTlvStr(MIPC_SIM_CHANGE_PIN_REQ_T_NEW_PIN, strings[1]);
                } else {
                    err = -1;
                }
                break;
            case CHANGE_PIN2:
                /*  UI shall handle CHANGE PIN only operated on PIN which is enabled and nonblocking state. */
                if (dataLen == 3 * sizeof(char*)) {
                    mipc_data =  RfxMipcData::obtainMipcData(MIPC_SIM_CHANGE_PIN_REQ, m_slot_id);
                    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANGE_PIN_REQ_T_PIN_TYPE, MIPC_SIM_PIN_TYPE_PIN2);
                    mipc_data->addMipcTlvStr(MIPC_SIM_CHANGE_PIN_REQ_T_OLD_PIN, strings[0]);
                    mipc_data->addMipcTlvStr(MIPC_SIM_CHANGE_PIN_REQ_T_NEW_PIN, strings[1]);
                } else {
                    err = -1;
                }
                break;
            default:
                err = -1;
                break;
        }

        logD(mTag, "handleSecurityOperation, Type: %d, dataLen:%zu", op, dataLen);

        if (mipc_data != NULL) {
            callToMipcMsgAsync(msg, mipc_data);
        }
    } while(0);

    if (err == -1) {
        /* AT runtime error */
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
                RfxIntsData((void*)&opRetryCount, sizeof(opRetryCount)), msg, false);
        responseToTelCore(response);
    }
}

void RmmSimCommRequestHandler::handleSecurityOperationResponse(const sp<RfxMclMessage>& msg,
        UICC_Security_Operation op) {
    int opRetryCount = -1;
    RIL_Errno result = RIL_E_GENERIC_FAILURE;

    //int appTypeId = -1;

    do {
        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        switch (op) {
            case ENTER_PIN1:
            case ENTER_PIN2:
                opRetryCount = mipcData->getMipcUint32Val(MIPC_SIM_VERIFY_PIN_CNF_T_REMAINING_ATTEMPTS, 0);
                break;
            case ENTER_PUK1:
            case ENTER_PUK2:
                opRetryCount = mipcData->getMipcUint32Val(MIPC_SIM_UNBLOCK_PIN_CNF_T_REMAINING_ATTEMPTS, 0);
                break;
            case CHANGE_PIN1:
            case CHANGE_PIN2:
                 opRetryCount = mipcData->getMipcUint32Val(MIPC_SIM_CHANGE_PIN_CNF_T_REMAINING_ATTEMPTS, 0);
                break;
            default:
                break;
        }

        if ((ENTER_PIN1 == op) || (CHANGE_PIN1 == op)) {
            setPin1RetryCountProp(opRetryCount);
        } else if ((ENTER_PIN2 == op) || (CHANGE_PIN2 == op)) {
            setPin2RetryCountProp(opRetryCount);
        } else if (ENTER_PUK1 == op) {
            setPuk1RetryCountProp(opRetryCount);
        } else if (ENTER_PUK2 == op) {
            setPuk2RetryCountProp(opRetryCount);
        }

        logD(mTag, "handleSecurityOperationResponse, mipc_result:%d, op=%d, RetryCount=%d",
                mipc_result, op, opRetryCount);

        if (mipc_result != MIPC_RESULT_SUCCESS) {
            switch (mipc_result) {
                case MIPC_RESULT_SIM_EXT_INCORRECT_PASS_WORD:
                    result = RIL_E_PASSWORD_INCORRECT;
                    break;
                case MIPC_RESULT_PIN_REQUIRED:
                case MIPC_RESULT_SIM_EXT_PUK1_REQUIRED:
                    result = RIL_E_PASSWORD_INCORRECT;
                    break;
                case MIPC_RESULT_SIM_EXT_PIN2_REQUIRED:
                    result = RIL_E_SIM_PIN2;
                    break;
                case MIPC_RESULT_SIM_EXT_PUK2_REQUIRED:
                    result = RIL_E_SIM_PUK2;
                    break;
                case MIPC_RESULT_OPERATION_NOT_ALLOWED:
                    result = RIL_E_REQUEST_NOT_SUPPORTED;
                    break;
                case MIPC_RESULT_SIM_EXT_SIM_FAILURE:
                case MIPC_RESULT_ERROR_UNKOWN:
                    if ((ENTER_PUK1 == op) || (ENTER_PUK2 == op)) {
                        result = RIL_E_INVALID_SIM_STATE;
                    } else {
                        result = RIL_E_REQUEST_NOT_SUPPORTED;
                    }
                    break;
                case MIPC_RESULT_SIM_EXT_TECHNICAL_PROBLEM:  /// Only for FTA mode
                    result = RIL_E_GENERIC_FAILURE;;
                    break;
                default:
                    result = RIL_E_REQUEST_NOT_SUPPORTED;
                    break;
            }
        } else {
            /* L5 shall check if shall do verify for FTA mode */
            result = RIL_E_SUCCESS;
        }

        if ((result == RIL_E_SUCCESS) && ((ENTER_PIN1 == op) || (ENTER_PUK1 == op))) {
            if (op == ENTER_PIN1) {
                logD(mTag, "handleSecurityOperationResponse, ENTER_PIN1 Success!");
            } else {
                logD(mTag, "handleSecurityOperationResponse, ENTER_PUK1 Success!");
            }

            sendSimStatusChanged();
        }
    } while(0);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxIntsData((void*)&opRetryCount, sizeof(opRetryCount)), msg, false);

    responseToTelCore(response);
}

/*
void RmmSimCommRequestHandler::setSimInsertedStatus(int slotId, int isInserted) {
    int pivot = 1;
    int pivotSim = slotId;
    int simInsertedStatus = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_INSERT_STATE);

    if(isInserted == 1) {
        pivotSim = pivot << slotId;
        simInsertedStatus = simInsertedStatus | pivotSim;
    } else {
        pivotSim = pivot << slotId;
        pivotSim = ~pivotSim;
        simInsertedStatus = (simInsertedStatus & pivotSim );
    }
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_INSERT_STATE, simInsertedStatus);
}
*/

// BTSAP - Start
void RmmSimCommRequestHandler::decodeBtSapPayload(int msgId, const void *src,
        size_t srclen, void *dst) {
    pb_istream_t stream;
    const pb_field_t *fields = NULL;

    logD(mTag, "[BTSAP] decodeBtSapPayload, start (%s), srclen:%zu", idToString(msgId), srclen);
    if (dst == NULL || src == NULL) {
        logE(mTag, "[BTSAP] decodeBtSapPayload, dst or src is NULL!!");
        return;
    }

    switch (msgId) {
        case RFX_MSG_REQUEST_SIM_SAP_CONNECT:
            fields = RIL_SIM_SAP_CONNECT_REQ_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_DISCONNECT:
            fields = RIL_SIM_SAP_DISCONNECT_REQ_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_APDU:
            fields = RIL_SIM_SAP_APDU_REQ_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_TRANSFER_ATR:
            fields = RIL_SIM_SAP_TRANSFER_ATR_REQ_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_POWER:
            fields = RIL_SIM_SAP_POWER_REQ_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_RESET_SIM:
            fields = RIL_SIM_SAP_RESET_SIM_REQ_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_SET_TRANSFER_PROTOCOL:
            fields = RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ_fields;
            break;
        default:
            logE(mTag, "[BTSAP] decodeBtSapPayload, MsgId is mistake!");
            return;
    }

    stream = pb_istream_from_buffer((uint8_t *)src, srclen);
    if (!pb_decode(&stream, fields, dst) ) {
        logE(mTag, "[BTSAP] decodeBtSapPayload, Error decoding protobuf buffer : %s",
                PB_GET_ERROR(&stream));
    } else {
        logD(mTag, "[BTSAP] decodeBtSapPayload, Success!");
    }
}

void RmmSimCommRequestHandler::resetBtSapContext() {
    logD(mTag, "[BTSAP] resetBtSapContext");
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_INIT);
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL, 0);
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_SUPPORT_PROTOCOL, 0);
    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_BTSAP_ATR, String8(""));
}

int RmmSimCommRequestHandler::toByte(char c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    logE(mTag, "toByte, Error: %c",c);
    return 0;
}

int RmmSimCommRequestHandler::hexStringToByteArrayEx(unsigned char* hexString,
        int hexStringLen, unsigned char ** byte) {
    int length = hexStringLen/2;
    unsigned char* buffer = (unsigned char*)malloc((length + 1)*sizeof(char));
    RFX_ASSERT(buffer != NULL);
    int i = 0;

    memset(buffer, 0, ((length + 1)*sizeof(char)));
    for (i = 0 ; i < hexStringLen ; i += 2)
    {
        buffer[i / 2] = (unsigned char)((toByte(hexString[i]) << 4) | toByte(hexString[i+1]));
    }

    *byte = buffer;

    return (hexStringLen/2);
}

char* RmmSimCommRequestHandler::convertBtSapIntToHexString(uint8_t *data, size_t datalen) {
    char* output = NULL, *pOut = NULL;
    unsigned int i = 0;

    if (data == NULL || datalen <= 0) {
        return output;
    }

    output = (char*)calloc(1, (sizeof(char)*datalen+1)*2);
    RFX_ASSERT(output != NULL);
    pOut = output;

    for (i = 0; i < datalen; i++) {
        pOut = &output[2*i];
        if (sprintf(pOut, "%02x", data[i]) < 0) {
            logE(mTag, "[BTSAP] convertBtSapIntToHexString, Copy string fail.");
        }
    }

    logD(mTag, "[BTSAP] convertBtSapIntToHexString, (%zu, %s)", datalen, output);
    return output;
}

void RmmSimCommRequestHandler::sendBtSapResponseComplete(const sp<RfxMclMessage>& msg,
        RIL_Errno ret, int msgId, void *data) {
    const pb_field_t *fields = NULL;
    size_t encoded_size = 0;
    size_t buffer_size = 0;
    pb_ostream_t ostream;
    bool success = false;
    //ssize_t written_bytes;
    sp<RfxMclMessage> response = NULL;

    logD(mTag, "[BTSAP] sendBtSapResponseComplete, start (%s)", idToString(msgId));

    switch (msgId) {
        case RFX_MSG_REQUEST_SIM_SAP_CONNECT:
            fields = RIL_SIM_SAP_CONNECT_RSP_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_DISCONNECT:
            fields = RIL_SIM_SAP_DISCONNECT_RSP_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_APDU:
            fields = RIL_SIM_SAP_APDU_RSP_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_TRANSFER_ATR:
            fields = RIL_SIM_SAP_TRANSFER_ATR_RSP_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_POWER:
            fields = RIL_SIM_SAP_POWER_RSP_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_RESET_SIM:
            fields = RIL_SIM_SAP_RESET_SIM_RSP_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_SET_TRANSFER_PROTOCOL:
            fields = RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP_fields;
            break;
        case RFX_MSG_REQUEST_SIM_SAP_ERROR_RESP:
            fields = RIL_SIM_SAP_ERROR_RSP_fields;
            break;
        default:
            logE(mTag, "[BTSAP] sendBtSapResponseComplete, MsgId is mistake!");
            return;
    }

    if ((success = pb_get_encoded_size(&encoded_size, fields, data)) &&
            encoded_size <= INT32_MAX) {
        //buffer_size = encoded_size + sizeof(uint32_t);
        buffer_size = encoded_size;
        uint8_t buffer[buffer_size];
        //written_size = htonl((uint32_t) encoded_size);
        ostream = pb_ostream_from_buffer(buffer, buffer_size);
        //pb_write(&ostream, (uint8_t *)&written_size, sizeof(written_size));
        success = pb_encode(&ostream, fields, data);

        if(success) {
            logD(mTag, "[BTSAP] sendBtSapResponseComplete, Size: %zu (0x%zx)", encoded_size,
                    encoded_size);
            // Send response
            //RIL_SAP_onRequestComplete(t, ret, buffer, buffer_size);
            response = RfxMclMessage::obtainSapResponse(msgId, ret,
                    RfxRawData((void*)buffer, buffer_size), msg, false);
            responseToTelCore(response);
        } else {
            logE(mTag, "[BTSAP] sendBtSapResponseComplete, Encode failed!");
        }
    } else {
        logE(mTag, "sendBtSapResponseComplete, Not sending response type %d: encoded_size: %zu. encoded size result: %d",
        msgId, encoded_size, success);
    }
}

void RmmSimCommRequestHandler::handleLocalBtSapReset(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    sendBtSapStatusInd(RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_CARD_RESET);
}

void RmmSimCommRequestHandler::handleBtSapConnect(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_CONNECT_REQ *req = NULL;
    RIL_SIM_SAP_CONNECT_RSP rsp;
    int sysMaxSize = 32767;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    void* data = msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();

    logD(mTag, "[BTSAP] handleBtSapConnect, start");
    req = (RIL_SIM_SAP_CONNECT_REQ*)calloc(1, sizeof(RIL_SIM_SAP_CONNECT_REQ));
    RFX_ASSERT(req != NULL);
    decodeBtSapPayload(RFX_MSG_REQUEST_SIM_SAP_CONNECT, data, datalen, req);

    if (req->max_message_size > sysMaxSize) {
        // Max message size from request > project setting
        logE(mTag, "[BTSAP] handleBtSapConnect, fail, size: %d", req->max_message_size);

        rsp.response = RIL_SIM_SAP_CONNECT_RSP_Response_RIL_E_SAP_MSG_SIZE_TOO_LARGE;
        rsp.max_message_size = sysMaxSize;
        rsp.has_max_message_size = TRUE;
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_CONNECT, &rsp);
    } else {
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_ACCESS_PROFILE_CONNECT_REQ, m_slot_id);
        callToMipcMsgAsync(msg, mipc_data);
    }

    free(req);
}

void RmmSimCommRequestHandler::handleBtSapConnectResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_CONNECT_RSP rsp;
    uint8_t type = -1;
    char *pAtr = NULL;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    RIL_SIM_SAP_STATUS_IND_Status unsolMsg = RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_UNKNOWN_ERROR;


    //p_response = atSendCommandSingleline("AT+EBTSAP=0", "+EBTSAP:");
    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    logD(mTag, "[BTSAP] handleBtSapConnectResponse, mipc_result = %d", mipc_result);

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        rsp.response = RIL_SIM_SAP_CONNECT_RSP_Response_RIL_E_SAP_CONNECT_FAILURE;

        switch (mipc_result) {
            case MIPC_RESULT_SIM_EXT_BT_SAP_UNDEFINED:
                ret = RIL_E_REQUEST_NOT_SUPPORTED;
                break;
            case MIPC_RESULT_SIM_EXT_BT_SAP_NOT_ACCESSIBLE:
                ret = RIL_E_SIM_ERR;
                unsolMsg = RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_CARD_NOT_ACCESSIBLE;
                break;
            case MIPC_RESULT_SIM_EXT_BT_SAP_CARD_REMOVED:
            case MIPC_RESULT_SIM_NOT_INSERTED:
                ret = RIL_E_SIM_ABSENT;
                unsolMsg = RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_CARD_REMOVED;
                break;
            default:
                break;
        }

        rsp.has_max_message_size = false;
        rsp.max_message_size = 0;
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_CONNECT, &rsp);
    } else {
        type = mipcData->getMipcUint8Val(MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_CUR_TYPE, 0);
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL, (int)type);

        type = mipcData->getMipcUint8Val(MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_SUPPORT_TYPE, 0);
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_SUPPORT_PROTOCOL, (int)type);

        uint16_t Len = 0;
        int atrLen = 0;
        pAtr = (char*) mipcData->getMipcVal(MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_ATR, &Len);
        if (pAtr != NULL) {
            atrLen = strlen(pAtr);
            getMclStatusManager()->setString8Value(RFX_STATUS_KEY_BTSAP_ATR,
                    String8::format("%s", pAtr));
        }

        logD(mTag, "[BTSAP] handleBtSapConnectResponse, cur_type: %d, supp_type: %d, size: %d, atrLen: %d",
                getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL, -1),
                getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_SUPPORT_PROTOCOL, -1),
                Len, atrLen);

        ret = RIL_E_SUCCESS;
        // For AOSP BT SAP UT case, to set the flag has_max_message_size as true.
        //rsp.has_max_message_size = false;
        rsp.has_max_message_size = true;
        rsp.max_message_size = 0;
        if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_VOICE_CALL_COUNT, 0) > 0) {
            // There is an ongoing call so do not notify STATUS_IND now
            logI(mTag, "[BTSAP] handleBtSapConnectResponse, Success but there is ongoing call");
            rsp.response = RIL_SIM_SAP_CONNECT_RSP_Response_RIL_E_SAP_CONNECT_OK_CALL_ONGOING;
        } else {
            logD(mTag, "[BTSAP] handleBtSapConnectResponse, Success");
            rsp.response = RIL_SIM_SAP_CONNECT_RSP_Response_RIL_E_SUCCESS;
            unsolMsg = RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_CARD_RESET;
        }

        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_CONNECTION_SETUP);

        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_CONNECT, &rsp);

        if (unsolMsg != RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_UNKNOWN_ERROR) {
            sendBtSapStatusInd(unsolMsg);
        }
        //logD(mTag, "[BTSAP] handleBtSapConnectResponse end");
    }
}

void RmmSimCommRequestHandler::handleBtSapDisconnect(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_DISCONNECT_REQ *req = NULL;
    RIL_SIM_SAP_DISCONNECT_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    void *data = msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();

    logD(mTag, "[BTSAP] handleBtSapDisconnect, start");
    req = (RIL_SIM_SAP_DISCONNECT_REQ*)calloc(1, sizeof(RIL_SIM_SAP_DISCONNECT_REQ));
    RFX_ASSERT(req != NULL);
    decodeBtSapPayload(RFX_MSG_REQUEST_SIM_SAP_DISCONNECT, data, datalen, req);

    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, (int)BT_SAP_DISCONNECT);
    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1) > 0) {
        //p_response = atSendCommand("AT+EBTSAP=1");
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_ACCESS_PROFILE_DISCONNECT_REQ, m_slot_id);
        callToMipcMsgAsync(msg, mipc_data);
    } else {
        logD(mTag, "[BTSAP] handleBtSapDisconnect, but card was removed");
        rsp.dummy_field = 1;
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_INIT);
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_DISCONNECT, &rsp);
    }

    if (req != NULL) {
        free(req);
    }
}

void RmmSimCommRequestHandler::handleBtSapDisconnectResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_DISCONNECT_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    //if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1) > 0) {
        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        logD(mTag, "[BTSAP] handleBtSapDisconnectResponse, mipc_result = %d", mipc_result);

        if (mipc_result != MIPC_RESULT_SUCCESS) {
            switch (mipc_result) {
                case MIPC_RESULT_SIM_EXT_BT_SAP_UNDEFINED:
                    ret = RIL_E_REQUEST_NOT_SUPPORTED;
                    break;
                case MIPC_RESULT_SIM_EXT_BT_SAP_NOT_ACCESSIBLE:
                    ret = RIL_E_SIM_ERR;
                    break;
                case MIPC_RESULT_SIM_EXT_BT_SAP_CARD_REMOVED:
                case MIPC_RESULT_SIM_NOT_INSERTED:
                    ret = RIL_E_SIM_ABSENT;
                    break;
                default:
                    break;
            }

            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_INIT);
            rsp.dummy_field = 0;
            sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_DISCONNECT, &rsp);
        } else {
            resetBtSapContext();
            ret = RIL_E_SUCCESS;
            rsp.dummy_field = 1;
            sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_DISCONNECT, &rsp);
            //logD(mTag, "[BTSAP] handleBtSapDisconnectResponse end");
        }
    //} else {
    //    logD(mTag, "[BTSAP] requestBtSapDisconnect but card was removed");
    //    rsp.dummy_field = 1;
    //    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_INIT);
    //    ret = RIL_E_SUCCESS;
    //    sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_DISCONNECT, &rsp);
    //}
}

void RmmSimCommRequestHandler::handleBtSapTransferApdu(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_APDU_REQ *req = NULL;
    RIL_SIM_SAP_APDU_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    void *data = msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();

    req = (RIL_SIM_SAP_APDU_REQ*)calloc(1, sizeof(RIL_SIM_SAP_APDU_REQ));
    RFX_ASSERT(req != NULL);
    decodeBtSapPayload(RFX_MSG_REQUEST_SIM_SAP_APDU, data, datalen, req);

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1) <= 0) {
        logD(mTag, "[BTSAP] requestBtSapTransferApdu, but card was removed");
        rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_ABSENT;
        ret = RIL_E_SIM_ABSENT;
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_APDU, &rsp);
    } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS, 0) ==
            BT_SAP_POWER_OFF) {
        logD(mTag, "[BTSAP] requestBtSapTransferApdu, but card was already power off");
        rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF;
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_APDU, &rsp);
    } else {
        char * hexStr = convertBtSapIntToHexString(req->command->bytes, req->command->size);
        logD(mTag, "[BTSAP] requestLocalBtSapTransferApdu, start, (%d,%d,%s)", req->type,
                req->command->size, hexStr);
        //cmd.append(String8::format("AT+EBTSAP=5,0,\"%s\"", hexStr));
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_ACCESS_PROFILE_APDU_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_ACCESS_PROFILE_APDU_REQ_T_TYPE, 0);
        if (hexStr != NULL) {
            mipc_data->addMipcTlvStr(MIPC_SIM_ACCESS_PROFILE_APDU_REQ_T_APDU, hexStr);
        }
        callToMipcMsgAsync(msg, mipc_data);
        if (hexStr != NULL) {
            free(hexStr);
        }
    }

    if (req != NULL) {
        free(req);
    }
}

void RmmSimCommRequestHandler::handleBtSapTransferApduResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_APDU_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int apduLen = 0;
    int reslen = 0;

    //if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1) <= 0) {
    //    logD(mTag, "[BTSAP] requestBtSapTransferApdu but card was removed");
    //    rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_ABSENT;
    //    ret = RIL_E_SIM_ABSENT;
    //    sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_APDU, &rsp);
    //} else
    {
        memset(&rsp, 0, sizeof(RIL_SIM_SAP_APDU_RSP));

        //p_response = atSendCommandSingleline(cmd, "+EBTSAP:");

        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        logD(mTag, "[BTSAP]handleBtSapTransferApduResponse, mipc_result = %d", mipc_result);

        if (mipc_result != MIPC_RESULT_SUCCESS) {
            rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_GENERIC_FAILURE;

            switch (mipc_result) {
                case MIPC_RESULT_SIM_EXT_BT_SAP_UNDEFINED:
                    ret = RIL_E_REQUEST_NOT_SUPPORTED;
                    rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_GENERIC_FAILURE;
                    break;
                case MIPC_RESULT_SIM_EXT_BT_SAP_NOT_ACCESSIBLE:
                    ret = RIL_E_SIM_ERR;
                    rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_NOT_READY;
                    break;
                case MIPC_RESULT_SIM_EXT_BT_SAP_CARD_REMOVED:
                case MIPC_RESULT_SIM_NOT_INSERTED:
                    ret = RIL_E_SIM_ABSENT;
                    rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_ABSENT;
                    break;
                default:
                    break;
            }
            goto format_error;
        } else {
            char *pApduResponse = NULL;
            pb_bytes_array_t *apduResponse = NULL;
            char *pRes = NULL;

            uint16_t a_len = 0;
            pApduResponse = (char*) mipcData->getMipcVal(MIPC_SIM_ACCESS_PROFILE_APDU_CNF_T_APDU, &a_len);
            if (pApduResponse == NULL) {
                goto format_error;
            }

            logD(mTag, "[BTSAP] handleBtSapTransferApduResponse, rsp:%.6s..., len:%d", pApduResponse, a_len);

            apduLen = (int) a_len;
            reslen = hexStringToByteArrayEx((unsigned char *)pApduResponse, apduLen,
                    (unsigned char **)&pRes);
            RFX_ASSERT(pRes != NULL);

            apduResponse = (pb_bytes_array_t *)calloc(1, sizeof(pb_bytes_array_t) + reslen);
            RFX_ASSERT(apduResponse != NULL);
            memcpy(apduResponse->bytes, pRes, reslen);
            apduResponse->size = reslen;
            rsp.apduResponse = apduResponse;
            rsp.response = RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SUCCESS;
            ret = RIL_E_SUCCESS;
            free(pRes);

            sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_APDU, &rsp);
            free(apduResponse);
        }
    }
    //logD(mTag, "[BTSAP] handleBtSapTransferApduResponse end");
    return;

format_error:
    logE(mTag, "[BTSAP] handleBtSapTransferApduResponse, Fail to send APDU");
    sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_APDU, &rsp);
}

void RmmSimCommRequestHandler::handleBtSapTransferAtr(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_TRANSFER_ATR_REQ *req = NULL;
    RIL_SIM_SAP_TRANSFER_ATR_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS, 0);
    pb_bytes_array_t *atrResponse = NULL;
    void *data = msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();

    logD(mTag, "[BTSAP] requestBtSapTransferAtr, start");
    req = (RIL_SIM_SAP_TRANSFER_ATR_REQ*)calloc(1, sizeof(RIL_SIM_SAP_TRANSFER_ATR_REQ));
    RFX_ASSERT(req != NULL);
    decodeBtSapPayload(RFX_MSG_REQUEST_SIM_SAP_TRANSFER_ATR, data, datalen, req);

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1) <= 0) {
        logD(mTag, "[BTSAP] requestBtSapTransferAtr, but card was removed");
        rsp.response = RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_SIM_ABSENT;
        rsp.atr = NULL;
        ret = RIL_E_SIM_ABSENT;
    } else if (status == BT_SAP_POWER_OFF) {
        logD(mTag, "[BTSAP] requestBtSapTransferAtr, but card was already power off");
        rsp.response = RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF;
        rsp.atr = NULL;
    } else if (status == BT_SAP_POWER_ON ||
                status == BT_SAP_CONNECTION_SETUP ||
                status == BT_SAP_ONGOING_CONNECTION) {
        char *pRes = NULL;
        rsp.response = RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_SUCCESS;
        String8 atr = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_BTSAP_ATR);
        const char* atrStr = atr.string();
        int atrLen = strlen(atrStr);
        int reslen = hexStringToByteArrayEx((unsigned char *)atrStr, atrLen,
                (unsigned char **)&pRes);
        RFX_ASSERT(pRes != NULL);
        atrResponse = (pb_bytes_array_t *)calloc(1,sizeof(pb_bytes_array_t) + reslen);
        RFX_ASSERT(atrResponse != NULL);
        memcpy(atrResponse->bytes, pRes, reslen);
        atrResponse->size = reslen;
        rsp.atr = atrResponse;
        if (pRes != NULL) {
            free(pRes);
        }

        logD(mTag, "[BTSAP] requestBtSapTransferAtr, status: %d, size: %d", status, atrLen);

        if (status == BT_SAP_CONNECTION_SETUP) {
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_ONGOING_CONNECTION);
        }
    } else {
        rsp.response = RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_GENERIC_FAILURE;
        rsp.atr = NULL;
    }
    sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_TRANSFER_ATR, &rsp);

    if (atrResponse != NULL) {
        free(atrResponse);
    }

    free(req);
    logD(mTag, "[BTSAP] requestBtSapTransferAtr, end");
}

void RmmSimCommRequestHandler::handleBtSapPower(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_POWER_REQ *req = NULL;
    RIL_SIM_SAP_POWER_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS);
    void *data = msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();

    req = (RIL_SIM_SAP_POWER_REQ*)calloc(1, sizeof(RIL_SIM_SAP_POWER_REQ));
    RFX_ASSERT(req != NULL);
    decodeBtSapPayload(RFX_MSG_REQUEST_SIM_SAP_POWER, data, datalen, req);
    logD(mTag, "[BTSAP] requestBtSapPower, state: (%d)", req->state);

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) <= 0) {
        logD(mTag, "[BTSAP] requestBtSapPower, but card was removed");
        rsp.response = RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ABSENT;
        ret = RIL_E_SIM_ABSENT;
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_POWER, &rsp);
    } else if (req->state == TRUE && status == BT_SAP_POWER_ON) {
        logD(mTag, "[BTSAP] requestBtSapPower on but card was already power on");
        rsp.response = RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ALREADY_POWERED_ON;
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_POWER, &rsp);
    } else if (req->state == FALSE && status == BT_SAP_POWER_OFF) {
        logD(mTag, "[BTSAP] requestBtSapPower off but card was already power off");
        rsp.response = RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF;
        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_POWER, &rsp);
    } else {
        /*if (req->state == TRUE) {
            cmd.append(String8::format("AT+EBTSAP=2,%d", getMclStatusManager()->getIntValue(
                    RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL)));
            p_response = atSendCommandSingleline(cmd, "+EBTSAP:");
        } else {
            p_response = atSendCommand("AT+EBTSAP=3");
        }*/

        sp<RfxMipcData> mipc_data = NULL;
        if (req->state == TRUE) {
            mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_ACCESS_PROFILE_POWER_ON_REQ, m_slot_id);
            int type = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL);

            mipc_data->addMipcTlvUint8(MIPC_SIM_ACCESS_PROFILE_POWER_ON_REQ_T_TYPE, (uint8_t)type);
        } else {
            mipc_data =
                RfxMipcData::obtainMipcData(MIPC_SIM_ACCESS_PROFILE_POWER_OFF_REQ, m_slot_id);
        }

        callToMipcMsgAsync(msg, mipc_data);
    }

    free(req);
}

void RmmSimCommRequestHandler::handleBtSapPowerResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_POWER_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    uint8_t type = 0;
    char *pAtr = NULL;
    //int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS);

    memset(&rsp, 0, sizeof(RIL_SIM_SAP_RESET_SIM_RSP));

    do {
        //if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) <= 0) {
        //    logD(mTag, "[BTSAP] handleBtSapPowerResponse but card was removed");
        //    rsp.response = RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ABSENT;
        //    ret = RIL_E_SIM_ABSENT;
        //} else
        {
            RfxMipcData* mipcData = msg->getMipcData();
            int  mipc_result = mipcData->getResult();

            logD(mTag, "[BTSAP]handleBtSapPowerResponse, mipc_result = %d", mipc_result);

            if (mipc_result != MIPC_RESULT_SUCCESS) {
                rsp.response = RIL_SIM_SAP_POWER_RSP_Response_RIL_E_GENERIC_FAILURE;
                switch (mipc_result) {
                    case MIPC_RESULT_SIM_EXT_BT_SAP_CARD_REMOVED:
                    case MIPC_RESULT_SIM_NOT_INSERTED:
                        ret = RIL_E_SIM_ABSENT;
                        rsp.response = RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ABSENT;
                        break;
                    default:
                        break;
                }
                goto format_error;
            } else {
                if (mipcData->getMipcMsgId() == MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF) {
                    type = mipcData->getMipcUint8Val(MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF_T_CUR_TYPE, 0);
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL, (int)type);

                    uint16_t a_len = 0;
                    pAtr = (char*) mipcData->getMipcVal(MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF_T_ATR, &a_len);
                    int atrLen = 0;
                    if (pAtr != NULL) {

                        atrLen = strlen(pAtr);
                        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_BTSAP_ATR,
                                String8::format("%s", pAtr));
                    }

                    logD(mTag, "[BTSAP] handleBtSapPowerResponse, cur_type: %d, size: %d",
                            type, atrLen);

                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS,
                            BT_SAP_POWER_ON);
                } else {
                    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS,
                            BT_SAP_POWER_OFF);
                }

                logD(mTag, "[BTSAP] handleBtSapPowerResponse, success! status: %d",
                        getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS));
                ret = RIL_E_SUCCESS;
                rsp.response = RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SUCCESS;
            }
        }

        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_POWER, &rsp);
    } while(0);

    //logD(mTag, "[BTSAP] handleBtSapPowerResponse end");
    return;

format_error:
    logE(mTag, "[BTSAP] handleBtSapPowerResponse, Fail to Set Power");
    sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_POWER, &rsp);
}

void RmmSimCommRequestHandler::handleBtSapResetSim(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_RESET_SIM_REQ *req = NULL;
    RIL_SIM_SAP_RESET_SIM_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS);
    void *data = msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();

    logD(mTag, "[BTSAP] requestBtSapResetSim, start");
    req = (RIL_SIM_SAP_RESET_SIM_REQ*)calloc(1, sizeof(RIL_SIM_SAP_RESET_SIM_REQ));
    RFX_ASSERT(req != NULL);
    decodeBtSapPayload(RFX_MSG_REQUEST_SIM_SAP_RESET_SIM, data, datalen, req);

    do {
        if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) <= 0) {
            logD(mTag, "[BTSAP] requestBtSapResetSim, but card was removed");
            rsp.response = RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SIM_ABSENT;
            sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_RESET_SIM, &rsp);
        } else if (status == BT_SAP_POWER_OFF) {
            logD(mTag, "[BTSAP] requestBtSapResetSim off but card was already power off");
            rsp.response = RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF;
            sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_RESET_SIM, &rsp);
        } else {
            //cmd.append(String8::format("AT+EBTSAP=4,%d", getMclStatusManager()->getIntValue(
            //        RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL)));

            sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_ACCESS_PROFILE_RESET_REQ, m_slot_id);
            int type = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL);
            mipc_data->addMipcTlvUint8(MIPC_SIM_ACCESS_PROFILE_RESET_REQ_T_TYPE, (uint8_t)type);
            callToMipcMsgAsync(msg, mipc_data);
        }
    } while(0);

    free(req);
}

void RmmSimCommRequestHandler::handleBtSapResetSimResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_RESET_SIM_RSP rsp;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    char *pAtr = NULL;
    uint8_t type = 0;

    do {
        //if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) <= 0) {
        //    logD(mTag, "[BTSAP] handleBtSapResetSimResponse but card was removed");
        //    rsp.response = RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SIM_ABSENT;
        //} else
        {
            memset(&rsp, 0, sizeof(RIL_SIM_SAP_RESET_SIM_RSP));

            //p_response = atSendCommandSingleline(cmd, "+EBTSAP:");
            RfxMipcData* mipcData = msg->getMipcData();
            int  mipc_result = mipcData->getResult();

            logD(mTag, "[BTSAP]handleBtSapResetSimResponse, mipc_result = %d", mipc_result);

            if (mipc_result != MIPC_RESULT_SUCCESS) {
                rsp.response = RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_GENERIC_FAILURE;
                switch (mipc_result) {
                    case MIPC_RESULT_SIM_EXT_BT_SAP_CARD_REMOVED:
                    case MIPC_RESULT_SIM_NOT_INSERTED:
                        ret = RIL_E_SIM_ABSENT;
                        rsp.response = RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SIM_ABSENT;
                        break;
                    default:
                        break;
                }
                goto format_error;
            } else {
                type = mipcData->getMipcUint8Val(MIPC_SIM_ACCESS_PROFILE_RESET_CNF_T_CUR_TYPE, 0);
                getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL, (int)type);

                uint16_t a_len = 0;
                pAtr = (char*) mipcData->getMipcVal(MIPC_SIM_ACCESS_PROFILE_RESET_CNF_T_ATR, &a_len);
                int atrLen = 0;
                if (pAtr != NULL) {
                    atrLen = strlen(pAtr);
                    getMclStatusManager()->setString8Value(RFX_STATUS_KEY_BTSAP_ATR,
                            String8::format("%s", pAtr));
                }

                logD(mTag, "[BTSAP] handleBtSapResetSimResponse, cur_type: %d, size: %d",
                        getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL),
                        atrLen);
                ret = RIL_E_SUCCESS;
                rsp.response = RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SUCCESS;
            }
        }

        sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_RESET_SIM, &rsp);
    } while(0);

    logD(mTag, "[BTSAP] handleBtSapResetSimResponse, end");
    return;

format_error:
    logE(mTag, "[BTSAP] handleBtSapResetSimResponse, Fail to Reset SIM");
    sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_RESET_SIM, &rsp);
}

void RmmSimCommRequestHandler::handleBtSapSetTransferProtocol(const sp<RfxMclMessage>& msg) {
    RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP rsp;
    RIL_Errno ret = RIL_E_REQUEST_NOT_SUPPORTED;
    sp<RfxMclMessage> response;

    logD(mTag, "[BTSAP] requestBtSapSetTransferProtocol, start");
    rsp.response = RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP_Response_RIL_E_GENERIC_FAILURE;

    sendBtSapResponseComplete(msg, ret, RFX_MSG_REQUEST_SIM_SAP_SET_TRANSFER_PROTOCOL, &rsp);
}
// BTSAP - End

void RmmSimCommRequestHandler::handleSetSimCardPower(const sp < RfxMclMessage > & msg) {
    int* mode = (int*)(msg->getData()->getData());

    //cmd.append(String8::format("AT+ESIMPOWER=%d", (*mode)));  //0 or 1, 2
    logD(mTag, "handleSetSimCardPower(), token:%d, mode = %d", msg->getToken(), *mode);

    sp<RfxMipcData> mipc_data = NULL;
    if ((*mode) == 2) {
        // pass through mode
        mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_SET_RESET_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_SET_RESET_REQ_T_MODE, MIPC_SIM_PASS_THROUGH_MODE_ENABLE);
    } else {
        mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_SET_SIM_POWER_REQ, m_slot_id);
        if ((*mode) == 3 || (*mode) == 4) {
            uint8_t enable = 0;
            if ((*mode) == 4) {
                enable = 1;
            }
            mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_MODE, 2);
            mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER, enable);
        } else {
           mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_MODE, 1);
           mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER, (uint8_t)(*mode));
        }
    }
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleSetSimCardPowerResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_Errno err = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        err = RIL_E_REQUEST_NOT_SUPPORTED;
    }

    logD(mTag, "handleSetSimCardPowerResponse, token:%d, mipc_result = %d", msg->getToken(), mipc_result);

    response = RfxMclMessage::obtainResponse(msg->getId(), err,
            RfxVoidData(), msg, false);

    responseToTelCore(response);
}

bool RmmSimCommRequestHandler::isEnableUiccApplicationsSupported() {
    if ((getMclStatusManager()->getIntValue(RFX_STATUS_KEY_AP_SIM_SWITCH_MODE)
            == SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE)
            && (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_MD_SIM_SWITCH_MODE)
            == SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE)) {
        return true;
    }
    return false;
}

void RmmSimCommRequestHandler::handleEnableUiccApplications(const sp < RfxMclMessage > & msg) {
    int* enable = (int*)(msg->getData()->getData());

    // AT+ESIMPOWER=10 disable Uicc Applications
    // AT+ESIMPOWER=11 enable Uicc Applications
    //cmd.append(String8::format("AT+ESIMPOWER=%d", mode));
    logD(mTag, "handleEnableUiccApplications(), token:%d, AT+ESIMPOWER=%d",
            msg->getToken(), (*enable)+10);

    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_SET_SIM_POWER_REQ, m_slot_id);
    mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_MODE, 0);
    mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER, (uint8_t)(*enable));
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleEnableUiccApplicationsResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SUCCESS;
    String8 cmd("");

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    //if (p_response == NULL || p_response->getError() < 0) {
    //    logE(mTag, "%s() fail: Undesirable", __FUNCTION__);
    //    ril_error = RIL_E_OPERATION_NOT_ALLOWED;
    //} else
    if (mipc_result != MIPC_RESULT_SUCCESS) {
        switch (mipc_result) {
            case MIPC_RESULT_OPERATION_NOT_ALLOWED:
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                break;
            case MIPC_RESULT_SIM_NOT_INSERTED:
                ril_error = RIL_E_SIM_ABSENT;
                break;
            default:
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                break;
        }

    }

    logD(mTag, "handleEnableUiccApplicationsResponse, token:%d, mipc_result: %d",
            msg->getToken(), mipc_result);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error, RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleAreUiccApplicationsEnabled(const sp < RfxMclMessage > & msg) {
    // +ESIMPOWER: 10 Uicc Applications disabled
    // +ESIMPOWER: 11 Uicc Applications enabled
    //p_response = atSendCommandSingleline("AT+ESIMPOWER?", "+ESIMPOWER:");

    logD(mTag, "handleAreUiccApplicationsEnabled, token:%d", msg->getToken());

    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_RESET_REQ, m_slot_id);
    mipc_data->addMipcTlvUint8(MIPC_SIM_GET_RESET_REQ_T_TYPE, 1);
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleAreUiccApplicationsEnabledResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SUCCESS;
    int enable = 0;

    do {
        // +ESIMPOWER: 10 Uicc Applications disabled
        // +ESIMPOWER: 11 Uicc Applications enabled
        //p_response = atSendCommandSingleline("AT+ESIMPOWER?", "+ESIMPOWER:");
        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        if (mipc_result != MIPC_RESULT_SUCCESS) {
            switch (mipc_result) {
                case MIPC_RESULT_SIM_NOT_INSERTED:
                    ril_error = RIL_E_SIM_ABSENT;
                    enable = 1;
                    break;
                default:
                    ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                    break;
            }
        } else {
            enable = mipcData->getMipcUint8Val(MIPC_SIM_GET_RESET_CNF_T_MODE, 0);
            logD(mTag, "handleAreUiccApplicationsEnabledResponse, token:%d, mode:%d",
                    msg->getToken(), enable);
            if (enable == 10) {
                enable = 0;
            } else if (enable == 11) {
                enable = 1;
            } else {
                logE(mTag, "handleAreUiccApplicationsEnabledResponse, Undesirable:%d", enable);
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                enable = 0;
            }
        }

        /*if (p_response == NULL || p_response->getError() < 0) {
            logE(mTag, "%s() fail: Undesirable", __FUNCTION__);
            ril_error = RIL_E_OPERATION_NOT_ALLOWED;
        } else if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                case CME_SIM_NOT_INSERTED:
                    ril_error = RIL_E_SIM_ABSENT;
                    enable = 1;
                    break;
                default:
                    ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                    break;
            }
        } else if (p_response->getSuccess() > 0) {
            // Success
            line = p_response->getIntermediates();
            line->atTokStart(&err);
            if (err < 0) {
                logE(mTag, "%s() fail 1: Undesirable", __FUNCTION__);
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                break;
            }
            // Uicc Applications enable status
            enable = line->atTokNextint(&err);
            if (err < 0) {
                logE(mTag, "%s() fail 2: Undesirable", __FUNCTION__);
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                enable = 0;
            } else {
                if (enable == 10) {
                    enable = 0;
                } else if (enable == 11) {
                    enable = 1;
                } else {
                    logE(mTag, "%s() fail 3: Undesirable", __FUNCTION__);
                    ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                    enable = 0;
                }
            }
            line = NULL;
        } else {
            logE(mTag, "%s() fail 4: Undesirable", __FUNCTION__);
            ril_error = RIL_E_OPERATION_NOT_ALLOWED;
        } */
    } while (0);
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxIntsData((void*)&enable, sizeof(int)), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleSetSimPower(const sp < RfxMclMessage > & msg) {
    int* mode = (int*)(msg->getData()->getData());
    uint8_t enable = 0;

    logD(mTag, "handleSetSimPower, token:%d, AT+ESIMPOWER=%d", msg->getToken(), *mode);

    //cmd.append(String8::format("AT+ESIMPOWER=%d", (*mode))); // 10 or 11
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_SET_SIM_POWER_REQ, m_slot_id);
    mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_MODE, 0);
    if (*mode == 11) {
        enable = 1;
    }
    mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER, enable);
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleSetSimPowerResponse(const sp < RfxMclMessage > & msg) {
    sp<RfxMclMessage> response;
    RIL_Errno err = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        switch (mipc_result) {
            case MIPC_RESULT_OPERATION_NOT_ALLOWED:
                err = RIL_E_OPERATION_NOT_ALLOWED;
                break;
            case MIPC_RESULT_SIM_NOT_INSERTED:
                err = RIL_E_SIM_ABSENT;
                break;
            default:
                err = RIL_E_OPERATION_NOT_ALLOWED;
                break;
        }
    }

    logD(mTag, "handleSetSimPowerResponse, token:%d, mipc_result: %d",
            msg->getToken(), mipc_result);

    response = RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg,
            false);
    responseToTelCore(response);
}


void RmmSimCommRequestHandler::handleSetMepSimSlotsMapping(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(
            MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ, m_slot_id);
    int dataLength = msg->getData()->getDataLength();
    int *slotsMap = (int *)msg->getData()->getData();
    int item_count = dataLength / (2*sizeof(int));

    mipc_msg_add_tlv_uint8(mipc_data->getData(),
            MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ_T_SIM_COUNT, item_count);
    mipc_msg_tlv_array_t *p_array = mipc_msg_add_tlv_array(mipc_data->getData(),
            MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ_T_PHY_SLOT_ID, item_count);
    for (int i = 0; i < item_count; i++) {
        mipc_msg_add_idx_uint8(mipc_data->getData(), p_array, i, (uint8_t)slotsMap[2*i]);
    }

     p_array = mipc_msg_add_tlv_array(mipc_data->getData(),
            MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ_T_PORT_ID, item_count);
     for (int i = 0; i < item_count; i++) {
        mipc_msg_add_idx_uint8(mipc_data->getData(), p_array, i, (uint8_t)slotsMap[2*i + 1]);
        logD(mTag, "handleSetMepSimSlotsMapping, item_count:%d, dataLen:%d, slot:%d, port:%d",
                item_count, dataLength, slotsMap[2*i], slotsMap[2*i + 1]);
    }
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleSetSimSlotsMapping(const sp<RfxMclMessage>& msg) {
    //simcount ==1 "AT+ESLOTSMAP=%d,%d"
    //simcount ==2 "AT+ESLOTSMAP=%d,%d,%d"
    //simcount ==3 "AT+ESLOTSMAP=%d,%d,%d,%d"
    int *slotsMap = (int *)msg->getData()->getData();
    int dataLength = msg->getData()->getDataLength();
    int item_count = dataLength / (2*sizeof(int));
    uint8_t map_list[5] = {0};

    logD(mTag, "handleSetSimSlotsMapping, item_count = %d, dataLength = %d",
            item_count, dataLength);

    map_list[0] =  (uint8_t) slotsMap[0] + 1;
    if (item_count >= 2) {
        map_list[1] =  (uint8_t) slotsMap[2] + 1;
        if (item_count >= 3) {
            map_list[2] =  (uint8_t) slotsMap[4] + 1;
            if (item_count >= 4) {
                map_list[3] =  (uint8_t) slotsMap[6] + 1;
            }
        }
    }

    logD(mTag, "handleSetSimSlotsMapping, %d,%d,%d", item_count, map_list[0], map_list[1]);
    if (item_count == 1) {
        map_list[0] = 1;
        logI(mTag, "handleSetSimSlotsMapping, AT+ESLOTSMAP=1,1");
    }
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(
            MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ, m_slot_id);
    mipc_data->addMipcTlvUint8(
            MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ_T_SLOTS_NUM, (uint8_t) item_count);
    mipc_data->addMipcTlv(
            MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ_T_SLOTS_MAPPING_LIST, item_count, map_list);
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleSetSimSlotsMappingResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        rilErrNo = RIL_E_REQUEST_NOT_SUPPORTED;
    }

    logD(mTag, "handleSetSimSlotsMappingResponse, mipc_result: %d", mipc_result);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::querySimOnOffActivation() {
    char feature[] = "SWITCH_OFF_SIM";
    char featureOption[RFX_PROPERTY_VALUE_MAX] = { 0 };

    int support = getFeatureVersion(feature);

    // SIM ON OFF feature, 0:disable, 1:enable
    if (support == 1) {
        rfx_property_set(PROPERTY_SIM_ONOFF_SUPPORT, String8::format("%d", support).string());
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_SIMONOFF_SUPPORT, true);
    } else {
        rfx_property_set(PROPERTY_SIM_ONOFF_SUPPORT, String8::format("%d", 0).string());
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_SIMONOFF_SUPPORT, false);

        // Set SIM on/off state for modem non-supported project.
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE, SIM_POWER_STATE_SIM_ON);
        String8 simOnOff(PROPERTY_SIM_ONOFF_STATE);
        simOnOff.append(String8::format("%d", (m_slot_id + 1)));
        rfx_property_set(simOnOff.string(), String8::format("%d", SIM_POWER_STATE_SIM_ON)
                .string());
        // Notify SIM power state.
        int simPower = SIM_POWER_STATE_SIM_ON;
        sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_POWER_CHANGED, m_slot_id,
                RfxIntsData(&simPower, 1));
        responseToTelCore(unsol);
    }

    rfx_property_get(PROPERTY_SIM_CARD_ONOFF, featureOption, "1");
    // Set SIM on/off enabled key value
    if ((atoi(featureOption) == 2) && (support == 1)) {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_SIM_ONOFF_ENABLED, true);
    } else {
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_SIM_ONOFF_ENABLED, false);
    }

    // UICC Applications enable/disable feature
    if ((atoi(featureOption) != 1) && (atoi(featureOption) != 2)) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_AP_SIM_SWITCH_MODE,
                SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE);
    } else {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_AP_SIM_SWITCH_MODE, atoi(featureOption));
    }
    if (support == 1) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_MD_SIM_SWITCH_MODE,
                SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE);
    } else {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_MD_SIM_SWITCH_MODE,
                SIM_SWITCH_MODE_NONE);
    }
}

void RmmSimCommRequestHandler::handleEventGetMepSlotStatus(const sp<RfxMclMessage>& msg) {
    //p_response = atSendCommandSingleline("AT+ESLOTSINFO?", "+ESLOTSINFO:");
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(
        MIPC_SIM_MEP_SLOTS_INFO_GET_REQ, m_slot_id);
    callToMipcMsgAsync(msg, req_data);
}

void RmmSimCommRequestHandler::handleEventGetMepSlotStatusResponse(const sp<RfxMclMessage>& msg) {
    RIL_SimSlotStatus **pp_slot_status = NULL;
    sp<RfxMclMessage> response;
    int slotNum = 0;
    int simCount = 0;
    int decodeerr = -1;
    sp<RfxMclMessage> unsol;

    //p_response = atSendCommandSingleline("AT+ESLOTSINFO?", "+ESLOTSINFO:");
    RfxMipcData* rsp_data = msg->getMipcData();

    int  mipc_result = rsp_data->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleEventGetMepSlotStatusResponse, result error:%d", mipc_result);
        goto error;
    }

    pp_slot_status = decodeMepSlotsinfo(NULL, rsp_data, &decodeerr, &slotNum);

    if ((decodeerr != RIL_E_SUCCESS) || (pp_slot_status == NULL)) {
        logE(mTag, "handleEventGetMepSlotStatusResponse, decodeerr:%d", decodeerr);
        goto error;
    }

    simCount = (slotNum > RfxRilUtils::rfxGetSimCount()) ? RfxRilUtils::rfxGetSimCount() : slotNum;

    for (int i = 0; i < simCount; i++) {
        for (int j = 0; j < pp_slot_status[i]->port_count; j++) {
            int logicalSlotId = pp_slot_status[i]->info[j].logicalSlotId;
            if ((logicalSlotId >= 0) && (logicalSlotId < RfxRilUtils::rfxGetSimCount())) {
                String8 iccid = getMclStatusManager(pp_slot_status[i]->info[j].logicalSlotId)
                        ->getString8Value(RFX_STATUS_KEY_SIM_ICCID);

                logD(mTag, "handleEventGetMepSlotStatusResponse, slotId:%d, iccid:%.7s, state:%d, comp:%d",
                        logicalSlotId,
                        iccid.string(),
                        pp_slot_status[i]->card_state,
                        strcmp(iccid.string(), "N/A"));
                if ((pp_slot_status[i]->card_state == RIL_CARDSTATE_ABSENT)
                        && (strcmp(iccid.string(), "N/A") != 0)) {
                    decodeerr = RIL_E_INVALID_STATE;
                    goto error;
                }
            }
        }
    }

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ICC_SLOT_STATUS, m_slot_id,
            RfxSimSlotStatusData(pp_slot_status,  sizeof(RIL_SimSlotStatus *) * slotNum));
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
    logE(mTag, "handleEventGetMepSlotStatusResponse, decode error!, pp_slot_status = %p",
            pp_slot_status);
    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->iccId);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
        free(pp_slot_status);
    }
}


void RmmSimCommRequestHandler::handleEventGetSlotStatus(const sp<RfxMclMessage>& msg) {
    //p_response = atSendCommandSingleline("AT+ESLOTSINFO?", "+ESLOTSINFO:");
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_EUICC_SLOTS_STATUS_REQ, m_slot_id);
    callToMipcMsgAsync(msg, req_data);

}

void RmmSimCommRequestHandler::handleEventGetSlotStatusResponse(const sp<RfxMclMessage>& msg) {
    //char* urc = (char *) msg->getData()->getData();
    //logD(mTag, "handleEventGetSlotStatusResponse urc =  %s ", urc);

    //if (!strStartsWith(urc, "+ESLOTSINFO:")) {
    //    logE(mTag, "handleEventGetSlotStatusResponse invalid event!");
    //    return;
    //}
    RIL_SimSlotStatus **pp_slot_status = NULL;
    sp<RfxMclMessage> response;
    int slotNum = 0;
    int simCount = 0;
    int decodeerr = -1;
    sp<RfxMclMessage> unsol;

    //p_response = atSendCommandSingleline("AT+ESLOTSINFO?", "+ESLOTSINFO:");
    RfxMipcData* rsp_data = msg->getMipcData();

    int  mipc_result = rsp_data->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleEventGetSlotStatusResponse, result error:%d", mipc_result);
        goto error;
    }

    pp_slot_status = decodeEslotsinfo(NULL, rsp_data, &decodeerr, &slotNum);

    if ((decodeerr != RIL_E_SUCCESS) || (pp_slot_status == NULL)) {
        logE(mTag, "handleEventGetSlotStatusResponse, decodeerr:%d", decodeerr);
        goto error;
    }

    simCount = (slotNum > RfxRilUtils::rfxGetSimCount()) ? RfxRilUtils::rfxGetSimCount() : slotNum;

    for (int i = 0; i < simCount; i++) {
        String8 iccid = getMclStatusManager(pp_slot_status[i]->logicalSlotId)->getString8Value(
                RFX_STATUS_KEY_SIM_ICCID);

        logD(mTag, "handleEventGetSlotStatusResponse, slotId:%d, iccid:%.7s, state:%d, comp:%d",
                pp_slot_status[i]->logicalSlotId, iccid.string(), pp_slot_status[i]->card_state,
                strcmp(iccid.string(), "N/A"));

        if ((pp_slot_status[i]->card_state == RIL_CARDSTATE_ABSENT)
                && (strcmp(iccid.string(), "N/A") != 0)) {
            decodeerr = RIL_E_INVALID_STATE;
            goto error;
        }
    }

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ICC_SLOT_STATUS, m_slot_id,
            RfxSimSlotStatusData(pp_slot_status,  sizeof(RIL_SimSlotStatus *) * slotNum));
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
    logE(mTag, "handleEventGetSlotStatusResponse, decode error!, pp_slot_status = %p", pp_slot_status);
    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->iccId);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
        free(pp_slot_status);
    }
}

void RmmSimCommRequestHandler::handleGetSimTypeInfo(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(
        MIPC_SIM_ESIM_SWITCH_GET_REQ, m_slot_id);
    callToMipcMsgAsync(msg, mipc_data);
    logD(mTag, "handleGetSimTypeInfo, token:%d", msg->getToken());
}

void RmmSimCommRequestHandler::handleGetSimTypeInfoResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_error = RIL_E_SUCCESS;
    RIL_SimTypeInfo* p_sim_info = NULL;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();
    logD(mTag, "handleGetSimTypeInfoResponse, mipc_result: %d", mipc_result);

    p_sim_info = (RIL_SimTypeInfo*)calloc(1, sizeof(RIL_SimTypeInfo));
    RFX_ASSERT(p_sim_info != NULL);
    memset(p_sim_info, 0, sizeof(RIL_SimTypeInfo));

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        p_sim_info->isSupport = TYPE_RESULT_NO_SUPPORT;
    } else {
        if (mipcData != NULL) {
            p_sim_info->isSupport = mipcData->getMipcUint8Val(MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_SUPPORT, 0);
            p_sim_info->simType = (RIL_SimType) mipcData->getMipcUint8Val(MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_PSIM_STATE, 0);
        }
    }

    p_sim_info->actionSource = TYPE_URC;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxSimTypeInfoData((void*) p_sim_info, sizeof(RIL_SimTypeInfo)), msg, false);
    responseToTelCore(response);
    free(p_sim_info);
}

void RmmSimCommRequestHandler::handleEventGetSimTypeInfo(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(
        MIPC_SIM_ESIM_SWITCH_GET_REQ, m_slot_id);
    callToMipcMsgAsync(msg, mipc_data);
    logD(mTag, "handleEventGetSimTypeInfo, token:%d", msg->getToken());
}

void RmmSimCommRequestHandler::handleEventGetSimTypeInfoResponse(const sp<RfxMclMessage>& msg) {
    RIL_SimTypeInfo* p_sim_info = NULL;
    sp<RfxMclMessage> response;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();
    logD(mTag, "handleEventGetSimTypeInfoResponse, mipc_result: %d", mipc_result);

    p_sim_info = (RIL_SimTypeInfo*) calloc(1, sizeof(RIL_SimTypeInfo));
    RFX_ASSERT(p_sim_info != NULL);
    memset(p_sim_info, 0, sizeof(RIL_SimTypeInfo));

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        p_sim_info->isSupport = TYPE_RESULT_NO_SUPPORT;
    } else {
        if (mipcData != NULL) {
            p_sim_info->isSupport = mipcData->getMipcUint8Val(MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_SUPPORT, 0);
            p_sim_info->simType = (RIL_SimType) mipcData->getMipcUint8Val(MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_PSIM_STATE, 0);
        }
    }

    p_sim_info->actionSource = TYPE_USER_HANDLE;
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_TYPE_INFO_IND,
            m_slot_id, RfxSimTypeInfoData((void*) p_sim_info, sizeof(RIL_SimTypeInfo)));
    responseToTelCore(unsol);
    free(p_sim_info);
}

void RmmSimCommRequestHandler::handleGetMepSlotStatus(const sp<RfxMclMessage>& msg) {
    //atSendCommandSingleline("AT+ESLOTSINFO?", "+ESLOTSINFO:");
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(
        MIPC_SIM_MEP_SLOTS_INFO_GET_REQ, m_slot_id);
    callToMipcMsgAsync(msg, mipc_data);
    logD(mTag, "handleGetMepSlotStatus, token:%d", msg->getToken());
}

void RmmSimCommRequestHandler::handleGetSlotStatus(const sp<RfxMclMessage>& msg) {
    //atSendCommandSingleline("AT+ESLOTSINFO?", "+ESLOTSINFO:");
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_EUICC_SLOTS_STATUS_REQ, m_slot_id);
    callToMipcMsgAsync(msg, mipc_data);
    logD(mTag, "handleGetSlotStatus, token:%d", msg->getToken());
}

void RmmSimCommRequestHandler::handleGetMepSlotStatusResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_error = RIL_E_REQUEST_NOT_SUPPORTED;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    sp<RfxMclMessage> response;
    int slotNum = 0;
    int decodeerr = -1;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        if (mipc_result == MIPC_RESULT_BUSY) {
            ril_error = RIL_E_SIM_ERR;
        }
        logE(mTag, "handleGetMepSlotStatusResponse, token:%d, result error:%d",
                msg->getToken(), mipc_result);
    } else {
        pp_slot_status = decodeMepSlotsinfo(NULL, mipcData, &decodeerr, &slotNum);

        logD(mTag, "handleGetMepSlotStatusResponse, token:%d, slotNum:%d, pp_slot_status = %p",
                msg->getToken(), slotNum, pp_slot_status);
        if (decodeerr == RIL_E_SIM_BUSY && mRetryGetSlotsInfoTimes < 30) {
            if (pp_slot_status != NULL) {
                free(pp_slot_status[0]->atr);
                free(pp_slot_status[0]->eid);
                free(pp_slot_status[0]);
                free(pp_slot_status);
            }
            mRetryGetSlotsInfoTimes++;

            //sleepMsec(200);
            logD(mTag, "handleGetMepSlotStatusResponse, sim busy, retry:%d",
                    mRetryGetSlotsInfoTimes);
            //sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_EUICC_SLOTS_STATUS_REQ, m_slot_id);
            //callToMipcMsgAsync(msg, mipc_data);
            //sendEvent(RFX_MSG_REQUEST_GET_SLOT_STATUS, msg->getData(), RIL_CMD_PROXY_7, m_slot_id,
            //        msg->getClientId(), msg->getToken(), 0.2*1000000000);
            sp<RfxMclMessage> mclMessage = RfxMclMessage::obtainRequest(
                                                    msg->getId(),
                                                    msg->getData(),
                                                    m_slot_id,
                                                    msg->getToken(),
                                                    msg->getSendToMainProtocol(),
                                                    msg->getRilToken(),
                                                    ms2ns(200), //200ms
                                                    msg->getTimeStamp(),
                                                    msg->getAddAtFront());
            RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
            return;
        }  else if ((decodeerr != RIL_E_SUCCESS) || (pp_slot_status == NULL)) {
            ril_error = RIL_E_SIM_ERR;
        } else {
            ril_error = RIL_E_SUCCESS;
        }
    }

    mRetryGetSlotsInfoTimes = 0;

    if (ril_error == RIL_E_SUCCESS) {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxSimSlotStatusData(pp_slot_status, sizeof(RIL_SimSlotStatus *) * slotNum),
                msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxSimSlotStatusData(NULL, 0), msg, false);
    }
    responseToTelCore(response);

    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
        free(pp_slot_status);
    }
}

void RmmSimCommRequestHandler::handleGetSlotStatusResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_error = RIL_E_REQUEST_NOT_SUPPORTED;
    RIL_SimSlotStatus **pp_slot_status = NULL;
    sp<RfxMclMessage> response;
    int slotNum = 0;
    int decodeerr = -1;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        if (mipc_result == MIPC_RESULT_BUSY) {
            ril_error = RIL_E_SIM_ERR;
        }
        logE(mTag, "handleGetSlotStatusResponse, token:%d, result error:%d",
                msg->getToken(), mipc_result);
    } else {
        pp_slot_status = decodeEslotsinfo(NULL, mipcData, &decodeerr, &slotNum);

        logD(mTag, "handleGetSlotStatusResponse, token:%d, slotNum:%d, pp_slot_status = %p",
                msg->getToken(), slotNum, pp_slot_status);
        if (decodeerr == RIL_E_SIM_BUSY && mRetryGetSlotsInfoTimes < 30) {
            if (pp_slot_status != NULL) {
                free(pp_slot_status[0]->atr);
                free(pp_slot_status[0]->iccId);
                free(pp_slot_status[0]->eid);
                free(pp_slot_status[0]);
                free(pp_slot_status);
            }
            mRetryGetSlotsInfoTimes++;

            //sleepMsec(200);
            logD(mTag, "handleGetSlotStatusResponse, sim busy, retry:%d", mRetryGetSlotsInfoTimes);
            //sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_EUICC_SLOTS_STATUS_REQ, m_slot_id);
            //callToMipcMsgAsync(msg, mipc_data);
            //sendEvent(RFX_MSG_REQUEST_GET_SLOT_STATUS, msg->getData(), RIL_CMD_PROXY_7, m_slot_id,
            //        msg->getClientId(), msg->getToken(), 0.2*1000000000);
            sp<RfxMclMessage> mclMessage = RfxMclMessage::obtainRequest(
                                                    msg->getId(),
                                                    msg->getData(),
                                                    m_slot_id,
                                                    msg->getToken(),
                                                    msg->getSendToMainProtocol(),
                                                    msg->getRilToken(),
                                                    ms2ns(200), //200ms
                                                    msg->getTimeStamp(),
                                                    msg->getAddAtFront());
            RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
            return;
        }  else if ((decodeerr != RIL_E_SUCCESS) || (pp_slot_status == NULL)) {
            ril_error = RIL_E_SIM_ERR;
        } else {
            ril_error = RIL_E_SUCCESS;
        }
    }

    mRetryGetSlotsInfoTimes = 0;

    if (ril_error == RIL_E_SUCCESS) {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxSimSlotStatusData(pp_slot_status, sizeof(RIL_SimSlotStatus *) * slotNum), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxSimSlotStatusData(NULL, 0), msg, false);
    }
    responseToTelCore(response);

    if (pp_slot_status != NULL) {
        free(pp_slot_status[0]->atr);
        free(pp_slot_status[0]->iccId);
        free(pp_slot_status[0]->eid);
        free(pp_slot_status[0]);
        free(pp_slot_status);
    }
}

void RmmSimCommRequestHandler::handleActiveUiccCard(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno ril_error  = RIL_E_SUCCESS;
    int simPowerStateErrNumber = SIM_POWER_STATE_SUCCESS;
    bool direct_response = true;

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        simPowerStateErrNumber = SIM_POWER_STATE_GENERIC_FAILURE;
        ril_error = RIL_E_SIM_ABSENT;
    } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_ON) {
       ril_error = RIL_E_SUCCESS;
       simPowerStateErrNumber = SIM_POWER_STATE_SUCCESS;
    } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_SWITCHING_STATE)
            == SIM_POWER_STATE_SIM_SWITCHING_ON) {
        ril_error = RIL_E_REQUEST_RATE_LIMITED;
        simPowerStateErrNumber = SIM_POWER_STATE_REQUEST_IN_PROCESS;
    } else {
        ///cmd.append(String8::format("AT+ESIMPOWER=%d", SIM_POWER_STATE_SIM_ON));
        direct_response = false;

        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ONOFF_SWITCHING_STATE,
                    SIM_POWER_STATE_SIM_SWITCHING_ON);

        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_SET_SIM_POWER_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_MODE, 0);
        //SIM_POWER_STATE_SIM_ON 11->1
        mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER, 1);
        callToMipcMsgAsync(msg, mipc_data);
    }

    logD(mTag, "handleActiveUiccCard, token:%d, err:%d, direct_response:%d",
            msg->getToken(), ril_error, direct_response);

    if (direct_response) {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(&simPowerStateErrNumber, 1), msg, false);
        responseToTelCore(response);
    }
}

void RmmSimCommRequestHandler::handleActiveUiccCardResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    int simPowerStateErrNumber = SIM_POWER_STATE_SUCCESS;
    RIL_Errno ril_error = RIL_E_SUCCESS;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();


    if (mipc_result != MIPC_RESULT_SUCCESS) {
        simPowerStateErrNumber = SIM_POWER_STATE_GENERIC_FAILURE;
        switch (mipc_result) {
            case MIPC_RESULT_OPERATION_NOT_ALLOWED:
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                break;
            case MIPC_RESULT_SIM_NOT_INSERTED:
                ril_error = RIL_E_SIM_ABSENT;
                break;
            default:
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                break;
        }
    }

    logD(mTag, "handleActiveUiccCardResponse, token:%d, mipc_result: %d",
            msg->getToken(), mipc_result);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxIntsData(&simPowerStateErrNumber, 1), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleDeactivateUiccCard(const sp<RfxMclMessage>& msg) {
    int simPowerStateErrNumber = SIM_POWER_STATE_SUCCESS;
    RIL_Errno ril_error = RIL_E_SUCCESS;
    bool direct_response = true;

    if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE) == 0) {
        simPowerStateErrNumber = SIM_POWER_STATE_GENERIC_FAILURE;
        ril_error = RIL_E_SIM_ABSENT;
    } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_STATE)
            == SIM_POWER_STATE_SIM_OFF) {
       ril_error = RIL_E_SUCCESS;
       simPowerStateErrNumber = SIM_POWER_STATE_SUCCESS;
    } else if (getMclStatusManager()->getIntValue(RFX_STATUS_KEY_SIM_ONOFF_SWITCHING_STATE)
            == SIM_POWER_STATE_SIM_SWITCHING_OFF) {
        ril_error = RIL_E_REQUEST_RATE_LIMITED;
        simPowerStateErrNumber = SIM_POWER_STATE_REQUEST_IN_PROCESS;
    } else {
        //cmd.append(String8::format("AT+ESIMPOWER=%d", SIM_POWER_STATE_SIM_OFF));
        direct_response = false;

        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_ONOFF_SWITCHING_STATE,
                    SIM_POWER_STATE_SIM_SWITCHING_OFF);
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_SET_SIM_POWER_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_MODE, 0);
        //SIM_POWER_STATE_SIM_OFF 11->1
        mipc_data->addMipcTlvUint8(MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER, 0);
        callToMipcMsgAsync(msg, mipc_data);
    }

    logD(mTag, "handleDeactivateUiccCard, token:%d, err:%d, direct_response:%d",
            msg->getToken(), ril_error, direct_response);

    if (direct_response) {
        sp<RfxMclMessage> response;
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(&simPowerStateErrNumber, 1), msg, false);
        responseToTelCore(response);
    }
}

void RmmSimCommRequestHandler::handleDeactivateUiccCardResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    int simPowerStateErrNumber = SIM_POWER_STATE_SUCCESS;
    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();
    RIL_Errno ril_error = RIL_E_SUCCESS;

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        simPowerStateErrNumber = SIM_POWER_STATE_GENERIC_FAILURE;
        switch (mipc_result) {
            case MIPC_RESULT_OPERATION_NOT_ALLOWED:
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                break;
            case MIPC_RESULT_SIM_NOT_INSERTED:
                ril_error = RIL_E_SIM_ABSENT;
                break;
            default:
                ril_error = RIL_E_OPERATION_NOT_ALLOWED;
                break;
        }
    }

    logD(mTag, "handleDeactivateUiccCardResponse, token:%d, mipc_result: %d",
            msg->getToken(), mipc_result);

    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxIntsData(&simPowerStateErrNumber, 1), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleGetImsi(const sp<RfxMclMessage>& msg) {
    uint8_t mode = 3;
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
    char** pStrings = (msg->getData() != NULL) ? ((char**)(msg->getData()->getData()))
                : NULL;
    String8 aid(((pStrings != NULL) && (pStrings[0] != NULL))? pStrings[0] : "");

    //check if GSM
    if (pStrings == NULL && (cardType & RFX_CARD_TYPE_SIM)) {
        mode = 0;
    } else if (!aid.isEmpty()) {
        if (strncmp(aid.string(), "A0000000871002", 14) == 0 ||
            (aid.isEmpty() && (cardType & RFX_CARD_TYPE_SIM))) {
            mode = 0;
        }
    }

    //check if Cdma
    if (mode != 0) {
        if (!aid.isEmpty()) {
            if ((aid.find("A0000003431002") >= 0) || (aid.find("A000000000RUIM") >= 0)) {
                mode = 1;
            }
        } else {
            int cardType = getMclStatusManager()->getIntValue(
                    RFX_STATUS_KEY_CARD_TYPE,
                    -1);
            if ((cardType & RFX_CARD_TYPE_RUIM) || (cardType & RFX_CARD_TYPE_CSIM)) {
                mode = 1;
            }
        }
    }

    if (mode == 0 || mode == 1) {
        //GSM:atSendCommandNumeric("AT+CIMI");
        //cdma:cmd.append(String8::format("AT+ECIMI=%d", appTypeId));

        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_IMSI_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_IMSI_REQ_T_MODE,  mode);
        if (mode == 1) {
            // need add apptype id for c2k
            int appTypeId = queryAppTypeId(aid);
            uint8_t sim_app_id = getMipcAppTypeFromRil(appTypeId);
            if (appTypeId != -1) {
                mipc_data->addMipcTlvUint8(MIPC_SIM_IMSI_REQ_T_APP_ID, (uint8_t)sim_app_id);
            }
            logD(mTag, "handleGetImsi, mode:%d, appType:%d", mode, appTypeId);
        } else {
            logD(mTag, "handleGetImsi, mode:%d", mode);
        }
        callToMipcMsgAsync(msg, mipc_data);
    } else {
        logE(mTag, "handleGetImsi, fail!, aid:%s, cardType:%d", aid.string(), cardType);
        // Impossible case
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ERR,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}

void RmmSimCommRequestHandler::handleGetImsiResponse(const sp<RfxMclMessage>& msg) {
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
    char** pStrings = (msg->getData() != NULL) ? ((char**)(msg->getData()->getData()))
                : NULL;
    String8 aid(((pStrings != NULL) && (pStrings[0] != NULL))? pStrings[0] : "");
    bool handled = false;

    //check if GSM
    if (pStrings == NULL && (cardType & RFX_CARD_TYPE_SIM)) {
        handleGetImsiGsmResponse(msg);
        handled = true;
    } else if (!aid.isEmpty()) {
        if (strncmp(aid.string(), "A0000000871002", 14) == 0 ||
            (aid.isEmpty() && (cardType & RFX_CARD_TYPE_SIM))) {
            handleGetImsiGsmResponse(msg);
            handled = true;
        }
    }

    //Cdma
    if (!handled) {
         handleGetImsiCdmaResponse(msg);
    }
}

void RmmSimCommRequestHandler::handleGetImsiGsmResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleGetImsiGsmResponse, Fail, mipc_result:%d", mipc_result);
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxStringData(NULL, 0), msg, false);
    } else {
        uint16_t len = 0;
        char *gsm_imsi = (char*) mipcData->getMipcVal(MIPC_SIM_IMSI_CNF_T_IMSI, &len);

        String8 imsi(((gsm_imsi != NULL) ? gsm_imsi : ""));
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_IMSI, imsi);

        logD(mTag, "handleGetImsiGsmResponse, mipc_result:%d, %.6s", mipc_result, gsm_imsi);

        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringData((void*)imsi.string(), imsi.length()), msg, false);
    }
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleGetImsiCdmaResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logE(mTag, "handleGetImsiCdmaResponse, Fail, mipc_result:%d", mipc_result);
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SIM_ERR,
                RfxStringData(NULL, 0), msg, false);
    } else {
        uint16_t len = 0;
        char *c_imsi = (char*) mipcData->getMipcVal(MIPC_SIM_IMSI_CNF_T_IMSI, &len);
        String8 c2kimsi(((c_imsi != NULL) ? c_imsi : ""));

        String8 cdmaSubscriberId("vendor.ril.uim.subscriberid");
        cdmaSubscriberId.append(String8::format(".%d", (m_slot_id + 1)));
        rfx_property_set(cdmaSubscriberId, c2kimsi.string());
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_C2K_IMSI, c2kimsi);

        logD(mTag, "handleGetImsiCdmaResponse, mipc_result:%d, %.6s", mipc_result, c_imsi);

        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringData((void*)c2kimsi.string(), c2kimsi.length()), msg);
    }

    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleQueryFacilityLock(const sp<RfxMclMessage>& msg) {
    int appTypeId = -1;
    char** pStrings = (char**)(msg->getData()->getData());
    String8 facility(pStrings[0]);
    String8 aid((pStrings[3] != NULL)? pStrings[3] : "");

    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SIM_ERR;

    if (facility.isEmpty()) {
        logE(mTag, "handleQueryFacilityLock, The facility string is empty.");
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    // Get app type id
    appTypeId = queryAppTypeId(aid);

    if (appTypeId == -1) {
        logE(mTag, "handleQueryFacilityLock, The facility appTypeId is invalid.");
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    do {
        logD(mTag, "handleQueryFacilityLock, token:%d, facility:%s, appTypeId: %x",
                msg->getToken(), facility.string(), appTypeId);

        uint8_t sim_app_id = getMipcAppTypeFromRil(appTypeId);
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_GET_FACILITY_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_GET_FACILITY_REQ_T_APP_ID, sim_app_id);
        mipc_data->addMipcTlvStr(MIPC_SIM_GET_FACILITY_REQ_T_FACILITY, facility);
        callToMipcMsgAsync(msg, mipc_data);
    } while (0);

    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::handleQueryFacilityLockResponse(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SIM_ERR;

    do {
        /* No network related query. CLASS is unnecessary */
        // Use AT+ESSTQ or ESSTQ =<app>,<service> to query service table
        // 0:  Service not supported
        // 1:  Service supported
        // 2:  Service allocated but not activated
        //p_response = atSendCommandSingleline(cmd, "+ESSTQ:");
        // The same as AOSP. 0 - Available & Disabled, 1-Available & Enabled, 2-Unavailable.
        //cmd.append(String8::format("AT+ECLCK=%d,\"%s\",2", appTypeId, facility.string()));
        //p_response = atSendCommandSingleline(cmd, "+ECLCK:");

        RfxMipcData* mipcData = msg->getMipcData();
        int  mipc_result = mipcData->getResult();

        if (mipc_result != MIPC_RESULT_SUCCESS) {
            logE(mTag, "handleQueryFacilityLockResponse, token:%d, Fail, mipc_result: %d",
                msg->getToken(), mipc_result);

            switch (mipc_result) {
                case MIPC_RESULT_BUSY:
                    //MIPC shall retry max 13 times(200ms each time) for this case
                    ril_error = RIL_E_SIM_BUSY;
                    break;
                case MIPC_RESULT_PIN_REQUIRED:
                case MIPC_RESULT_SIM_EXT_PUK1_REQUIRED:
                    ril_error = RIL_E_PASSWORD_INCORRECT;
                    break;
                case MIPC_RESULT_SIM_EXT_PIN2_REQUIRED:
                    ril_error = RIL_E_SIM_PIN2;
                    break;
                case MIPC_RESULT_SIM_EXT_PUK2_REQUIRED:
                    ril_error = RIL_E_SIM_PUK2;
                    break;
                case MIPC_RESULT_SIM_EXT_INCORRECT_PASS_WORD:
                    ril_error = RIL_E_PASSWORD_INCORRECT;
                    break;
                case MIPC_RESULT_FDN_BLOCKED:
                    ril_error = RIL_E_FDN_CHECK_FAILURE;
                    break;
                default:
                   break;
            }

            response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                    RfxIntsData(), msg, false);
            responseToTelCore(response);

        } else {
            // Success
            uint8_t status = mipcData->getMipcUint8Val(MIPC_SIM_GET_FACILITY_CNF_T_STATUS, 0);
            logD(mTag, "handleQueryFacilityLockResponse, token:%d, status: %d",
                    msg->getToken(), status);
            int enable = (int) status;
            ril_error = RIL_E_SUCCESS;
            response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                    RfxIntsData((void*)&enable, sizeof(int)), msg, false);
            responseToTelCore(response);
        }
    } while (0);
}

void RmmSimCommRequestHandler::handleSetFacilityLock(const sp<RfxMclMessage>& msg) {
    int appTypeId = -1;
    int attemptsRemaining[1] = {0};

    String8 facFd("FD");
    String8 facSc("SC");
    char** pStrings = (msg->getData() != NULL) ? ((char**)(msg->getData()->getData())) : NULL;
    String8 facility(((pStrings != NULL) && (pStrings[0] != NULL))? pStrings[0] : "");
    String8 lockStr(((pStrings != NULL) && (pStrings[1] != NULL))? pStrings[1] : "");
    String8 pwd(((pStrings != NULL) && (pStrings[2] != NULL))? pStrings[2] : "");
    String8 aid(((pStrings != NULL) && (pStrings[4] != NULL))? pStrings[4] : "");

    sp<RfxMclMessage> response;
    RmmSimPinPukCount *retry = NULL;
    RIL_Errno ril_error = RIL_E_SIM_ERR;

    if (facility.isEmpty()) {
        logE(mTag, "handleSetFacilityLock, The facility string is empty.");
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    // Get app type id
    appTypeId = queryAppTypeId(aid);

    if (appTypeId == -1) {
         logE(mTag, "handleSetFacilityLock, The appTypeId is invalid");
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    do {
        if (pwd.isEmpty()) {
            logE(mTag, "handleSetFacilityLock, The password can't be empty");
            //ril_error = RIL_E_PASSWORD_INCORRECT;
            //goto error;
        }
        logD(mTag, "handleSetFacilityLock, token:%d, facility:%s, appTypeId:%x, mode:%s",
                msg->getToken(), facility.string(), appTypeId, lockStr.string());

        //cmd.append(String8::format("AT+ECLCK=%d,\"%s\",%s,\"%s\"", appTypeId, facility.string(),
        //        lockStr.string(), pwd.string()));

        uint8_t sim_app_id = getMipcAppTypeFromRil(appTypeId);
        sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_SET_FACILITY_REQ, m_slot_id);
        mipc_data->addMipcTlvUint8(MIPC_SIM_SET_FACILITY_REQ_T_APP_ID, sim_app_id);
        mipc_data->addMipcTlvStr(MIPC_SIM_SET_FACILITY_REQ_T_FACILITY, facility);
        mipc_data->addMipcTlvStr(MIPC_SIM_SET_FACILITY_REQ_T_PASS_WORD, pwd);

        uint8_t enable = 0;
        if (strncmp(lockStr.string(), "1", 1) == 0) {
            enable = 1;
        }
        mipc_data->addMipcTlvUint8(MIPC_SIM_SET_FACILITY_REQ_T_MODE, enable);
        callToMipcMsgAsync(msg, mipc_data);
    } while (0);

    return;

error:
    retry = getPinPukRetryCount();
    if (retry != NULL) {
        /* SIM operation we shall return pin retry counts */
        attemptsRemaining[0] = retry->pin1;
        if (facility == facFd) {
            attemptsRemaining[0] = retry->pin2;
        }
        free(retry);
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData((void*)attemptsRemaining, sizeof(int)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(), msg, false);
    }
    responseToTelCore(response);

    logD(mTag, "handleSetFacilityLock, set fail, send RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED");
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(unsol);
}

void RmmSimCommRequestHandler::handleSetFacilityLockResponse(const sp<RfxMclMessage>& msg) {
    int attemptsRemaining[1] = {0};
    String8 facFd("FD");
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SIM_ERR;
    uint8_t retry_count = 0;
    char** pStrings = (msg->getData() != NULL) ? ((char**)(msg->getData()->getData())) : NULL;
    String8 facility(((pStrings != NULL) && (pStrings[0] != NULL))? pStrings[0] : "");

    //cmd.append(String8::format("AT+ECLCK=%d,\"%s\",%s,\"%s\"", appTypeId, facility.string(),
    //        lockStr.string(), pwd.string()));

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();
    retry_count = mipcData->getMipcUint8Val(MIPC_SIM_SET_FACILITY_CNF_T_RETRY_COUNT, 0);
    attemptsRemaining[0] = retry_count;

    if (facility == facFd) {
        setPin2RetryCountProp(retry_count);
    } else {
        setPin1RetryCountProp(retry_count);
    }

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logE(mTag,
            "handleSetFacilityLockResponse, token:%d, result:%d, pinCnt:%d, shall has X_SIM_STATUS_CHANGED",
            msg->getToken(), mipc_result, retry_count);

        switch (mipc_result) {
            case MIPC_RESULT_BUSY:
                //MIPC shall retry max 13 times(200ms each time) for this case
                //sleepMsec(200);
                if (mRetrySetFacilityTimes <= 13) {
                    mRetrySetFacilityTimes++;
                    sp<RfxMclMessage> mclMessage =  RfxMclMessage::obtainRequest(
                                                            msg->getId(),
                                                            msg->getData(),
                                                            m_slot_id,
                                                            msg->getToken(),
                                                            msg->getSendToMainProtocol(),
                                                            msg->getRilToken(),
                                                            ms2ns(200), //200ms
                                                            msg->getTimeStamp(),
                                                            msg->getAddAtFront());
                    RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
                    return;
                }
                ril_error = RIL_E_SIM_BUSY;
                break;
            case MIPC_RESULT_PIN_REQUIRED:
            case MIPC_RESULT_SIM_EXT_PUK1_REQUIRED:
                ril_error = RIL_E_PASSWORD_INCORRECT;
                break;
            case MIPC_RESULT_SIM_EXT_PIN2_REQUIRED:
                ril_error = RIL_E_SIM_PIN2;
                break;
            case MIPC_RESULT_SIM_EXT_PUK2_REQUIRED:
                ril_error = RIL_E_SIM_PUK2;
                break;
            case MIPC_RESULT_SIM_EXT_INCORRECT_PASS_WORD:
                ril_error = RIL_E_PASSWORD_INCORRECT;
                break;
            case MIPC_RESULT_FDN_BLOCKED:
                ril_error = RIL_E_FDN_CHECK_FAILURE;
                break;
            default:
                break;
        }
    } else {
        //org: only GSM report RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED
        logD(mTag,
            "handleSetFacilityLockResponse, token:%d, result:%d, pinCnt:%d, shall has X_SIM_STATUS_CHANGED",
            msg->getToken(), mipc_result, retry_count);
        ril_error = RIL_E_SUCCESS;
    }

    mRetrySetFacilityTimes = 0;

    if (retry_count != 255) {
        /* SIM operation we shall return pin retry counts */
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData((void*)attemptsRemaining, sizeof(int)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(), msg, false);
    }
    responseToTelCore(response);
}


void RmmSimCommRequestHandler::handleCdmaSubscription(const sp<RfxMclMessage>& msg) {
    char *p_response[5] = {NULL, NULL, NULL, NULL, NULL};
    RIL_Errno *result = (RIL_Errno *)calloc(1, sizeof(RIL_Errno));
    RFX_ASSERT(result != NULL);

    logI(mTag, "handleCdmaSubscription!");
    // Query CDMA EFmlpl and EFmspl file.
    //queryCdmaMlplAndMspl();

    if (RfxViaUtils::getViaHandler() != NULL) {
        RfxViaUtils::getViaHandler()->handleCdmaSubscription(this, p_response, result); //TODO ??
    } else {
        *result = RIL_E_REQUEST_NOT_SUPPORTED;
        logE(mTag, "handleCdmaSubscription, RIL_E_REQUEST_NOT_SUPPORTED!");
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), *result,
            RfxStringsData((void*)p_response, sizeof(p_response)), msg);
    responseToTelCore(response);

    for (unsigned int i = 0; i < sizeof(p_response)/sizeof(p_response[0]); i++) {
        if (p_response[i] != NULL) {
            free(p_response[i]);
        }
    }

    free(result);
}

void RmmSimCommRequestHandler::handleCdmaGetSubSource(const sp<RfxMclMessage>& msg) {
    int source = -1;

    if (!RatConfig_isC2kSupported()) {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_REQUEST_NOT_SUPPORTED,
                RfxIntsData((void*)&source, sizeof(int)), msg);
        responseToTelCore(response);
        return;
    }

    logD(mTag, "handleCdmaGetSubSource, start");
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_REQ, m_slot_id);
    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::handleCdmaGetSubSourceResponse(const sp<RfxMclMessage>& msg) {
    int state = 255;
    int source = -1;
    RIL_Errno ril_eror = RIL_E_SIM_ERR;
    int mipc_result = 0;

    do {
        //atSendCommandSingleline("AT+EUIMCFG?", "+EUIMCFG");

        RfxMipcData* mipcData = msg->getMipcData();
        mipc_result = mipcData->getResult();

        if (mipc_result != MIPC_RESULT_SUCCESS) {
            break;
        }

        state = mipcData->getMipcUint8Val(
                    MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF_T_UIM_STATE,
                    0xff);

        // State value
        // 0: UIM disabled
        // 1: UIM enabled
        if (state == 1) {
            source = SUBSCRIPTION_FROM_RUIM;
        } else if (state == 0) {
            source = SUBSCRIPTION_FROM_NV;
        }
        logD(mTag, "handleCdmaGetSubSourceResponse, State:%d => source:%d", state, source);
        ril_eror = RIL_E_SUCCESS;
    } while (0);

    if (source == -1) {
        logE(mTag, "handleCdmaGetSubSourceResponse, failed! mipc_result:%d", mipc_result);
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ril_eror,
            RfxIntsData((void*)&source, sizeof(int)), msg);
    responseToTelCore(response);
}

void RmmSimCommRequestHandler::queryCdmaMlpl(const sp<RfxMclMessage>& msg) {
    // int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
    // if (!(cardType & RFX_CARD_TYPE_CSIM)) {
    //    logE(mTag, "queryCdmaMlplAndMspl Not support MLPL and MSPL!");
    //    goto error;
    // }

    // open channel
    // p_response = atSendCommandSingleline("AT+ESIMAPP=2,0", "+ESIMAPP:");
    // cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d,,\"%s\"", 2, channelId, 176, 20256,
    //        0/*P1*/, 0/*P2*/, 0/*P3*/, "7F105F3C"));
    // Read EFmlpl
    sp<RfxMipcData> mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ, m_slot_id);
    uint8_t sim_app_id = getMipcAppTypeFromRil(UICC_APP_CSIM);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_APP_ID, sim_app_id);
    //mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_SESSION_ID, (uint8_t)channelId);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_CMD, 176);
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FILE_ID, 20256);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P1, 0);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P2, 0);
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX, 0);
    const char* path = "7F105F3C";
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN, 0);
    mipc_data->addMipcTlvStr(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH, path);

    callToMipcMsgAsync(msg, mipc_data);
}

void RmmSimCommRequestHandler::queryCdmaMspl(const sp<RfxMclMessage>& msg) {
    String8 result("");
    RIL_SIM_IO_Response sr;
    int mlplVer = -1, msplVer = -1;
    String8 prop("ril.csim.mlpl_mspl_ver");
    sp<RfxMipcData> mspl_data = NULL;
    sp<RfxMipcData> mipc_data = NULL;
    const char* path = "7F105F3C";
    uint8_t sim_app_id = getMipcAppTypeFromRil(UICC_APP_CSIM);

    prop.append(String8::format("%d", m_slot_id));
    memset(&sr, 0, sizeof(RIL_SIM_IO_Response));

    RfxMipcData* mipcData = msg->getMipcData();
    int mipc_result = mipcData->getResult();
    if (mipc_result == MIPC_RESULT_SUCCESS) {
        // Success
        uint16_t sw =
            mipcData->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_SW, 0);
        uint16_t apdu_len =
            mipcData->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_LEN, 0);
        if (apdu_len > 0) {
            uint16_t len = 0;
            char* temp_simResponse =
                (char*) mipcData->getMipcVal(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_APDU, &len);
            char* simResponse = (char*)calloc(1, (sizeof(char)* (apdu_len + 1)));
            RFX_ASSERT(simResponse != NULL);
            simResponse[apdu_len] = '\0';
            memcpy(simResponse, temp_simResponse, apdu_len);

            //responseLen = hexStringToByteArray((unsigned char*)(simResponse), &response);
            if (len > 4) {
                mlplVer = (simResponse[3] << 4) | simResponse[4];
                logI(mTag, "queryCdmaMspl, mlplVer:%04x, rsp:%d,%02x%02x, sw:%04x",
                        mlplVer, len, simResponse[3], simResponse[4], sw);
                free(simResponse);
            } else {
                logD(mTag, "queryCdmaMspl, mlplVer:%04x, rsp:%d,%s, sw:%04x",
                        mlplVer, len, simResponse, sw);
                free(simResponse);
                goto error;
            }
        } else {
            logE(mTag, "queryCdmaMspl, error Mlpl apdu len is 0, sw:%04x", sw);
            //goto error;
        }
    } else {
        logE(mTag, "queryCdmaMspl, mlps mipc_result:%d", mipc_result);
    }

    // Read EFmspl
    //cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d,,\"%s\"", 2, channelId, 176, 20257,
    //        0/*P1*/, 0/*P2*/, 0/*P3*/, "7F105F3C"));
    //p_response = atSendCommandSingleline(cmd.string(), "+ECRLA:");
    mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ, m_slot_id);
    //same sim app id
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_APP_ID, sim_app_id);
    //mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_SESSION_ID, (uint8_t)channelId);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_CMD, 176);
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FILE_ID, 20257);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P1, 0);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P2, 0);
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX, 0);
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN, 0);
    mipc_data->addMipcTlvStr(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH, path);

    mspl_data = callToMipcMsgSync(mipc_data);
    mipc_result = mspl_data->getResult();

    if (mipc_result == MIPC_RESULT_SUCCESS) {
        // Success
        uint16_t sw =
            mspl_data->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_SW, 0);
        uint16_t apdu_len =
            mspl_data->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_LEN, 0);
        if (apdu_len > 0) {
            uint16_t len = 0;
            char* temp_simResponse =
                (char*) mspl_data->getMipcVal(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_APDU, &len);
            char* simResponse = (char*)calloc(1, (sizeof(char)* (apdu_len + 1)));
            RFX_ASSERT(simResponse != NULL);
            simResponse[apdu_len] = '\0';
            memcpy(simResponse, temp_simResponse, apdu_len);

            //responseLen = hexStringToByteArray((unsigned char*)(simResponse), &response);
            if (len > 4) {
                msplVer = (simResponse[3] << 4) | simResponse[4];
                logD(mTag, "queryCdmaMspl, msplVer:%04x, rsp:%d,%02x%02x, sw:%04x",
                        msplVer, len, simResponse[3], simResponse[4], sw);
                free(simResponse);
            } else {
                logD(mTag, "queryCdmaMspl, msplVer:%04x, rsp:%d,%s, sw:%04x",
                        msplVer, len, simResponse, sw);
                free(simResponse);
                goto error;
            }
        } else {
            logE(mTag, "queryCdmaMspl, error Mspl apdu len is 0, sw:sw:%04x", sw);
            goto error;
        }
    } else {
        logE(mTag, "queryCdmaMspl, Mspl mipc_result:%d", mipc_result);
    }

    // Set property
    if ((mlplVer != -1) && (msplVer != -1)) {
        result.append(String8::format("%x,%x", mlplVer, msplVer));
        rfx_property_set(prop, result);
        logI(mTag, "queryCdmaMspl, result:%s", result.string());
         handleCdmaSubscription(msg);
        return;
    }

error:
    rfx_property_set(prop, ",");
    logE(mTag, "queryCdmaMspl, Fail to query MLPL and MSPL!");
    handleCdmaSubscription(msg);
}

void RmmSimCommRequestHandler::queryCdma3gDualActivation() {
    char feature[] = "CDMA3G_DUAL_ACTIVATION";
    int support = getFeatureVersion(feature);

    // CDMA3G_DUAL_ACTIVATION Support, 0:disable, 1:enable
    if (support == 1) {
        rfx_property_set("vendor.ril.cdma.3g.dualact", String8::format("%d", support).string());
    } else {
        rfx_property_set("vendor.ril.cdma.3g.dualact", String8::format("%d", 0).string());
    }
}

/*
void RmmSimCommRequestHandler::enableEuiccSupport() {
    //atSendCommand(String8::format("AT+CIREP=1"));
    char atcmd[] = "AT+ESBP=5,\"SBP_SIM_EUICC_SUPPORT\",1";
    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_SYS_AT_REQ, m_slot_id);
    initMipc->addMipcTlvStr(MIPC_SYS_AT_REQ_T_ATCMD, atcmd);
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);

    logD(mTag, "enableEuiccSupport, enable esim result:%d",
            cnfMipc->getResult());
}
*/
