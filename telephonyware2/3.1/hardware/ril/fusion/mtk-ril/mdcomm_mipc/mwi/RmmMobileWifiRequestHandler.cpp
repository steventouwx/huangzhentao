/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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

// MTK fusion include
#include "RfxVoidData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"

// MWI Local include
#include "RmmMobileWifiRequestHandler.h"

#include <arpa/inet.h>
#include <dlfcn.h>

// MIPC interface
#include <mipc_msg.h>

#define RFX_LOG_TAG "RmmMwi"
#define LINK_LIBWPFA_PATH "libwpfa.so"

 // register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmMobileWifiRequestHandler, RIL_CMD_PROXY_1);

// register request to RfxData
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_ASSOCIATED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WFC_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_WFC_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_SIGNAL_LEVEL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_GEO_LOCATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_IP_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_SSAC_STATUS);

static const int requests[] = {
    RFX_MSG_REQUEST_SET_WIFI_ENABLED,
    RFX_MSG_REQUEST_SET_WIFI_ASSOCIATED,
    RFX_MSG_REQUEST_SET_WFC_CONFIG,
    RFX_MSG_REQUEST_GET_WFC_CONFIG,
    RFX_MSG_REQUEST_SET_WIFI_SIGNAL_LEVEL,
    RFX_MSG_REQUEST_SET_GEO_LOCATION,
    RFX_MSG_REQUEST_SET_WIFI_IP_ADDRESS,
    RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID,
    RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE,
    RFX_MSG_REQUEST_QUERY_SSAC_STATUS,
};

typedef enum{
    WFC_SETTING_WIFI_UEMAC = 0,
    WFC_SETTING_LOCATION_SETTING = 1,
    WFC_SETTING_WIFI_CONN_STATE = 2,
    WFC_SETTING_PACKET_FORWARD = 3,
    WFC_SETTING_WIFI_APM_STATE = 4,
}WfcConfigType;

typedef enum{
    WFC_FEATURE_UNSUPPORTED = 0,
    WFC_FEATURE_SUPPORTED = 1,
    WFC_FEATURE_UNKNOWN = 2,
}WfcFeatureState;

void *dlMdcommWpfaHandle = NULL;
void (*wpfaSetWifiConnectState) (char* ifname, uint8_t state) = NULL;
void (*wpfaSetWifiIp) (char* ifname, char* ipv4, char* ipv6) = NULL;
void (*wpfaSetIpRoute) (char* ifname) = NULL;
void (*wpfaFlashIpRoute) (char* ifname) = NULL;
void (*wpfaDelaySetPktForward) () = NULL;

RmmMobileWifiRequestHandler::RmmMobileWifiRequestHandler(
    int slot_id, int channel_id) : RfxBaseHandler(slot_id, channel_id) {
    // register to handle request

    dlMdcommWpfaHandle = dlopen(LINK_LIBWPFA_PATH, RTLD_NOW);
    if (dlMdcommWpfaHandle == NULL) {
        logI(RFX_LOG_TAG, "[%s] dlopen : module = %s, failed (%s)", __FUNCTION__, LINK_LIBWPFA_PATH, dlerror());
    } else {
        logI(RFX_LOG_TAG, "[%s] dlopen : module = %s, pass.", __FUNCTION__, LINK_LIBWPFA_PATH);
        wpfaSetWifiConnectState = (void (*) (char* ifname, uint8_t state)) dlsym(dlMdcommWpfaHandle, "setWifiConnectState");
        wpfaSetWifiIp = (void (*) (char* ifname, char* ipv4, char* ipv6)) dlsym(dlMdcommWpfaHandle, "setWifiIp");
        wpfaSetIpRoute = (void (*) (char* ifname)) dlsym(dlMdcommWpfaHandle, "setIpRoute");
        wpfaFlashIpRoute = (void (*) (char* ifname)) dlsym(dlMdcommWpfaHandle, "flashIpRoute");
        wpfaDelaySetPktForward = (void (*) ()) dlsym(dlMdcommWpfaHandle, "setPktForward");
    }

    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
    mWifiConnStateSupport = WFC_FEATURE_UNKNOWN;
}

RmmMobileWifiRequestHandler::~RmmMobileWifiRequestHandler() {
    if (dlMdcommWpfaHandle != NULL)
        dlclose(dlMdcommWpfaHandle);
}

void RmmMobileWifiRequestHandler::onHandleTimer() {
}

void RmmMobileWifiRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int requestId = msg->getId();
    //logD(RFX_LOG_TAG, "onHandleRequest: %d", requestId);
    switch (requestId) {
        case RFX_MSG_REQUEST_SET_WIFI_ENABLED:
            setWifiEnabled(msg);
            break;
        case RFX_MSG_REQUEST_SET_WIFI_ASSOCIATED:
            setWifiAssociated(msg);
            break;
        case RFX_MSG_REQUEST_SET_WFC_CONFIG:
            setWfcConfig(msg);
            break;
        case RFX_MSG_REQUEST_GET_WFC_CONFIG:
            getWfcConfig(msg);
            break;
        case RFX_MSG_REQUEST_SET_WIFI_SIGNAL_LEVEL:
            setWifiSignal(msg);
            break;
        case RFX_MSG_REQUEST_SET_GEO_LOCATION:
            setGeoLocation(msg);
            break;
        case RFX_MSG_REQUEST_SET_WIFI_IP_ADDRESS:
            setWifiIpAddress(msg);
            break;
        case RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID:
            setEmergencyAddressId(msg);
            break;
        case RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE:
            notifyEPDGScreenState(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_SSAC_STATUS:
            querySsacStatus(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "onHandleRequest: no handle");
            break;
    }
}

void RmmMobileWifiRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    //logD(RFX_LOG_TAG, "onHandleResponse: %d", request);
    switch(request) {
        case RFX_MSG_REQUEST_SET_GEO_LOCATION:
        case RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID:
        case RFX_MSG_REQUEST_SET_WFC_CONFIG:
            handleVoidResponse(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_SSAC_STATUS:
            querySsacStatusResponse(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "onHandleResponse: no handle");
            break;
    }
}

void RmmMobileWifiRequestHandler::handleVoidResponse(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    uint16_t msgId = mipcData->getMipcMsgId();
    unsigned int result = mipcData->getResult();
    logD(RFX_LOG_TAG, "handleVoidResponse, msgId:%d, reuslt: %d", msgId, result);

    RIL_Errno rilErrno = result == MIPC_RESULT_SUCCESS ? RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmMobileWifiRequestHandler::handleNtfCmdResponse(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleNtfResponse, msgId:%d", msg->getId());
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmMobileWifiRequestHandler::handleErrorResponse(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleErrorResponse, msgId:%d", msg->getId());
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmMobileWifiRequestHandler::handleDefaultResponse(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleDefaultResponse, msgId:%d", msg->getId());
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmMobileWifiRequestHandler::setWifiEnabled(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFIEN=<ifname>,<enabled>
     * <ifname>: interface name, such as wlan0
     * <enabled>: 0 = disable; 1 = enable
     * <allCause>: 0 = airplane mode off; 1 = airplane mode on, 2 = wifi enabled state, 
     * 4 = need send wifi enabled satte, 8 = airplane mode state, 16 = need send airplane mode state.
     */
    char** params = (char**)msg->getData()->getData();
    char* ifname = params[0]? params[0]: (char*)"";
    char* enabled_str = params[1]? params[1]: (char*)"0";
    if (strlen(enabled_str) != 1) {
        logE(RFX_LOG_TAG, "setWifiEnabled: enabled len error");
        handleErrorResponse(msg);
        return;
    }
    const uint8_t enabled = (uint8_t)atoi(enabled_str);

    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_WIFI_INFO_NTF, msg->getSlotId());
    requestData->addMipcTlvStr(MIPC_WFC_WIFI_INFO_NTF_T_IFNAME, ifname);
    uint8_t allCause = (uint8_t)atoi(params[2]);
    uint8_t cause = (allCause & (EWIFIEN_CAUSE)) == EWIFIEN_CAUSE ? 1 : 0;
    logD(RFX_LOG_TAG, "setWifiEnabled enabled: %d, allCause:%d", enabled, cause);
    requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_ENABLE, enabled);
    requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_CAUSE, cause);
    callToMipcMsgNtf(requestData);
    handleNtfCmdResponse(msg);
}

void RmmMobileWifiRequestHandler::convertStringMacToByteArray(
        char* mac, uint8_t macArray[], uint16_t arrayLen) {
    if (mac == NULL) {
        logE(RFX_LOG_TAG, "convertStringMacToByteArray params null");
        return;
    }
    string dataSet(mac);
    vector<string> byteArray = splitString(dataSet, ":");
    uint16_t len = byteArray.size();

    if (len > arrayLen) {
        //original data too long, arrayLen is 6 byte
        logE(RFX_LOG_TAG, "convertStringMacToByteArray len:%d, arrayLen:%d", len, arrayLen);
        return;
    }
    for (uint16_t i = 0; i < len; i++) {
        int result = sscanf(byteArray[i].data(), "%2hhx", &macArray[i]);
        if (result < 0) {
            logE(RFX_LOG_TAG, "convertStringMacToByteArray sscanf %s failed:%d",
                    byteArray[i].data(), result);
        }
    }

    if (UNIT_TEST) {
        logD(RFX_LOG_TAG, "convertStringMacToByteArray=%02x:%02x:%02x:%02x:%02x:%02x",
            macArray[0], macArray[1], macArray[2], macArray[3],
            macArray[4], macArray[5]);
    }
}

void RmmMobileWifiRequestHandler::setWifiAssociated(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFIASC=<ifname>,<conn_ready>,<ssid>,<ap_mac>,<wifi_type>,<mtu>,<ue_mac>,<conn_state>
     * <ifname>: interface name, such as wlan0
     * <conn_ready>: 0 = not associated; 1 = associated
     * <ssid>: wifi ap ssid when associated, 0 if assoc = 0
     * <ap_mac>: wifi ap mac addr, 0 if assoc = 0
     * <wifi_type>: wifi type: such as 802.11b, 802.11n...
     * <mtu>: mtu size
     * <ue_mac>: ue mac address
     * <conn_state> wifi connect state
     */
    char** params = (char**)msg->getData()->getData();
    int dataLen =  msg->getData()->getDataLength() / sizeof(char*);
    logD(RFX_LOG_TAG, "setWifiIpAddress, original dataLen: %d", dataLen);
    if (dataLen < WIFI_ASC_PARAMS_COUNT) {
        handleErrorResponse(msg);
        return;
    }
    char* ifname = params[0]? params[0]: (char*)"" ;

    char* assoc_str = params[1]? params[1]: (char*)"0";
    if (strlen(assoc_str) != 1) {
        logE(RFX_LOG_TAG, "setWifiAssociated: assoc len error");
        handleErrorResponse(msg);
        return;
    }
    const uint8_t assoc = (uint8_t)atoi(assoc_str);

    if (mWifiConnStateSupport == WFC_FEATURE_UNKNOWN) {
        getWifiConnStateSupport();
    }

    uint8_t wifi_state = 0;
    if (mWifiConnStateSupport == WFC_FEATURE_SUPPORTED) {
        char* wifi_state_str = params[6]? params[6]: (char*)"0";
        if (strlen(wifi_state_str) != 1) {
            logE(RFX_LOG_TAG, "setWifiAssociated: conn_state len error");
            handleErrorResponse(msg);
            return;
        }
        wifi_state = (uint8_t)atoi(wifi_state_str);
    }

    bool isWifiDisconnected = false;
    if (wifi_state == 0 && assoc == 0) {
        isWifiDisconnected = true;
    }

    if (wpfaSetWifiConnectState != NULL)
        wpfaSetWifiConnectState(ifname, assoc);

    char* ssid = (isWifiDisconnected)? (char*)"0": params[2];
    if (ssid == NULL) {
        ssid = (char*)"";
        logE(RFX_LOG_TAG, "setWifiAssociated: no ssid");
    }
    char* ap_mac = (isWifiDisconnected)? (char*)"0": params[3];

    char* mtu = (isWifiDisconnected)? (char*)"0": params[4];
    if (mtu == NULL) {
        mtu = (char*)"0";
        logE(RFX_LOG_TAG, "setWifiAssociated: no mtu");
    }
    //value greater than 10000 will be ignore
    if (strlen(mtu) >= WIFI_MTU_LEN) {
        logE(RFX_LOG_TAG, "setWifiAssociated: mtu len error");
        handleErrorResponse(msg);
        return;
    }
    uint16_t mtu_tlv = (uint16_t)atoi(mtu);

    char* ue_mac = (params[5])? params[5]: (char*)"0";
    char defaultWifiType[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("vendor.test.wifi.type", defaultWifiType, "802.11n");
    char* type = defaultWifiType;

    if (isWifiDisconnected) {
        type = (char*) "";
    }

    // Ut Test
    if (UNIT_TEST) {
        logD(RFX_LOG_TAG, "setWifiAssociated +EWIFIASC:%s,%d,%s,%s,%s,%d,%s,%d, wifi_state_support:%d",
            ifname, assoc, ssid, ap_mac, type, mtu_tlv, ue_mac, wifi_state, mWifiConnStateSupport);
    } else {
        logD(RFX_LOG_TAG, "setWifiAssociated assoc:%d, wifi_state:%d, wifi_state_support:%d",
                assoc, wifi_state, mWifiConnStateSupport);
    }

    uint8_t ap_mac_array[MIPC_FIX_WFC_MAC_LEN] = {0};
    convertStringMacToByteArray(ap_mac, ap_mac_array, MIPC_FIX_WFC_MAC_LEN);

    uint8_t ue_mac_array[MIPC_FIX_WFC_MAC_LEN] = {0};
    convertStringMacToByteArray(ue_mac, ue_mac_array, MIPC_FIX_WFC_MAC_LEN);

    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_WIFI_INFO_NTF, msg->getSlotId());
    requestData->addMipcTlvStr(MIPC_WFC_WIFI_INFO_NTF_T_IFNAME, ifname);
    requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_ASSOCIATED, assoc);
    requestData->addMipcTlvStr(MIPC_WFC_WIFI_INFO_NTF_T_SSID, ssid);
    requestData->addMipcTlv(MIPC_WFC_WIFI_INFO_NTF_T_AP_MAC, MIPC_FIX_WFC_MAC_LEN, ap_mac_array);
    requestData->addMipcTlvStr(MIPC_WFC_WIFI_INFO_NTF_T_WIFI_TYPE_STR, type);
    requestData->addMipcTlvUint16(MIPC_WFC_WIFI_INFO_NTF_T_MTU, mtu_tlv);
    requestData->addMipcTlv(MIPC_WFC_WIFI_INFO_NTF_T_UE_MAC, MIPC_FIX_WFC_MAC_LEN, ue_mac_array);
    if(mWifiConnStateSupport == WFC_FEATURE_SUPPORTED) {
        requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_CONN_STATE, wifi_state);
        requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_CONN_READY, assoc);
    }
    //send NTF notify
    callToMipcMsgNtf(requestData);
    handleNtfCmdResponse(msg);
}

void RmmMobileWifiRequestHandler::setWfcConfig(const sp<RfxMclMessage>& msg) {
    char** params = (char**)msg->getData()->getData();
    int configType = params[0]? atoi(params[0]): 0;
    logD(RFX_LOG_TAG, "setWfcConfig: %d", configType);

    switch (configType) {
        case WFC_SETTING_LOCATION_SETTING:
            setWfcConfig_LocationSetting(msg);
            break;
        case WFC_SETTING_WIFI_APM_STATE:
            setWfcConfig_WifiApmState(msg);
            break;
        case WFC_SETTING_PACKET_FORWARD:
            setWfcConfig_PktForward(msg);
            break;
        default:
            handleDefaultResponse(msg);
            break;
    }
}

void RmmMobileWifiRequestHandler::setWfcConfig_LocationSetting(const sp<RfxMclMessage>& msg) {
    char** params = (char**)msg->getData()->getData();
    char* ifname = params[1]? params[1]: (char*)""; //ifName. e.g. ""locenable""
    char* value = params[2]? params[2]: (char*)""; //setting value
    uint8_t value_tlv = 0;
    if (strlen(value) != 1) {
        logE(RFX_LOG_TAG, "setWfcConfig_LocationSetting: value len error");
        handleErrorResponse(msg);
        return;
    }
    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_CFG_REQ, msg->getSlotId());
    value_tlv = (uint8_t)atoi(value);
    logD(RFX_LOG_TAG, "setWfcConfig_LocationSetting: %s, %d", ifname, value_tlv);
    requestData->addMipcTlvUint8(MIPC_WFC_CFG_REQ_T_LOCATION_ENABLE, value_tlv);
    callToMipcMsgAsync(msg, requestData);
}

void RmmMobileWifiRequestHandler::setWfcConfig_WifiApmState(const sp<RfxMclMessage>& msg) {
    char** params = (char**)msg->getData()->getData();
    char* ifname = params[1]? params[1]: (char*)"";
    char* value = params[2]? params[2]: (char*)"";
    uint8_t value_tlv = 0;
    if (strlen(value) != 1) {
        logE(RFX_LOG_TAG, "setWfcConfig_WifiApmState: value len error");
        handleErrorResponse(msg);
        return;
    }
    sp<RfxMipcData> mipcData = RfxMipcData::obtainMipcData(MIPC_WFC_WIFI_INFO_NTF, msg->getSlotId());
            value_tlv = (uint8_t)atoi(value);
    logD(RFX_LOG_TAG, "setWfcConfig_WifiApmState: %s, %d", ifname, value_tlv);
    mipcData->addMipcTlvStr(MIPC_WFC_WIFI_INFO_NTF_T_IFNAME, ifname);
    mipcData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_APM_STATE, value_tlv);
    callToMipcMsgNtf(mipcData);
    handleNtfCmdResponse(msg);
}

void RmmMobileWifiRequestHandler::setWfcConfig_PktForward(const sp<RfxMclMessage>& msg) {
    if (wpfaDelaySetPktForward != NULL) {
        wpfaDelaySetPktForward();
        handleDefaultResponse(msg);
    } else {
        handleErrorResponse(msg);
    }
}

void RmmMobileWifiRequestHandler::getWfcConfig(const sp<RfxMclMessage>& msg) {
    int *params = (int *)msg->getData()->getData();
    int setting = params[0];
    logD(RFX_LOG_TAG, "getWfcConfig: %d", setting);
    switch (setting) {
        case WFC_SETTING_WIFI_CONN_STATE:
            getWfcConfig_WifiConnState(msg);
            break;
        default:
            getWfcConfig_Default(msg);
            break;
    }
}

void RmmMobileWifiRequestHandler::getWfcConfig_Default(const sp<RfxMclMessage>& msg) {
    RIL_Errno result = RIL_E_SUCCESS;
    int responses[1] = {0};
    responses[0] = WFC_FEATURE_UNKNOWN;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);

    // report to tcl
    responseToTelCore(response);
}

void RmmMobileWifiRequestHandler::getWifiConnStateSupport() {
    char feature[] = "EWIFIASC_CONN_STATE";
    if (getFeatureVersion(feature) >= WFC_FEATURE_SUPPORTED) {
        mWifiConnStateSupport = WFC_FEATURE_SUPPORTED;
    } else {
        mWifiConnStateSupport = WFC_FEATURE_UNSUPPORTED;
    }
    logI(RFX_LOG_TAG, "EWIFIASC_CONN_STATE:%d", mWifiConnStateSupport);
}

void RmmMobileWifiRequestHandler::getWfcConfig_WifiConnState(const sp<RfxMclMessage>& msg) {
    RIL_Errno result = RIL_E_SUCCESS;
    int responses[1] = {0};

    getWifiConnStateSupport();
    responses[0] = mWifiConnStateSupport;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);

    // report to tcl
    responseToTelCore(response);
}

void RmmMobileWifiRequestHandler::setWifiSignal(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFISIGLVL=<ifname>,<rssi>,<snr>
     * <ifname>: interface name, such as wlan0
     * <rssi>: rssi value
     * <snr>: string value
     */
    char** params = (char**)msg->getData()->getData();
    char* ifname = params[0]? params[0]: (char*)"";
    char* rssi_str = params[1]? params[1]: (char*)"0";
    int16_t rssi = (int16_t)atoi(rssi_str);
    char* snr = params[2]? params[2]: (char*)"unknow";
    int16_t snr_tlv = WIFI_SNR_UNKNOW;
    if (strncmp(snr, "unknow", 6) != 0) {
        snr_tlv = (int16_t)atoi(snr);
    }

    logD(RFX_LOG_TAG, "setWifiSignal +EWIFISIGLVL:%s,%d,%d", ifname, rssi, snr_tlv);
    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_WIFI_SIGNAL_NTF, msg->getSlotId());
    requestData->addMipcTlvStr(MIPC_WFC_WIFI_SIGNAL_NTF_T_IFNAME, ifname);
    requestData->addMipcTlvInt16(MIPC_WFC_WIFI_SIGNAL_NTF_T_RSSI, rssi);
    requestData->addMipcTlvInt16(MIPC_WFC_WIFI_SIGNAL_NTF_T_SNR, snr_tlv);
    //send NTF notify
    callToMipcMsgNtf(requestData);
    handleNtfCmdResponse(msg);
}

void RmmMobileWifiRequestHandler::convertStringIpv4ToByteArray(
        char* ipv4, uint8_t ipv4Array[], uint16_t arrayLen) {
    if (ipv4 == NULL) {
        logE(RFX_LOG_TAG, "convertStringIpv4ToByteArray params null");
        return;
    }

    string dataSet(ipv4);
    if (dataSet.find(".") == string::npos) {
        //no ipv4 format
        logE(RFX_LOG_TAG, "convertStringIpv4ToByteArray no ipv4 format");
        return;
    }

    vector<string> byteArray = splitString(dataSet, ".");
    uint16_t len = byteArray.size();

    if (len > arrayLen) {
        //original data too long, arrayLen is 4 byte
        logE(RFX_LOG_TAG, "convertStringIpv4ToByteArray len:%d, arrayLen:%d", len, arrayLen);
        return;
    }
    for (uint16_t i = 0; i < len; i++) {
        ipv4Array[i] = (uint8_t)atoi(byteArray[i].data());
    }

    if (UNIT_TEST) {
        logD(RFX_LOG_TAG, "convertStringIpv4ToByteArray:%d.%d.%d.%d",
            ipv4Array[0], ipv4Array[1], ipv4Array[2], ipv4Array[3]);
    }
}

int RmmMobileWifiRequestHandler::convertStringIpv6ToByteArray(
        char* ipv6, uint8_t Ipv6Array[], uint16_t arrayLen) {
    /**
    adjust the format of IP address to be consistent and comparable
    EX 2001:2001:0:0:1::11 -> 2001:0000:0000:0001:0000:0000:0000:0011
    */
    int ret = 1;
    struct in6_addr v6Address;
    memset(&v6Address, 0, sizeof(v6Address));
    // ret: -1, error occurs, ret: 0, invalid address, ret: 1, success;
    ret = inet_pton(AF_INET6, ipv6, &v6Address);
    if (ret > 0) {
        memcpy(Ipv6Array, &v6Address, arrayLen);
        if (UNIT_TEST) {
            logD(RFX_LOG_TAG, "convertStringIpv6ToByteArray ipv6_array=%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
                    Ipv6Array[0], Ipv6Array[1], Ipv6Array[2], Ipv6Array[3],
                    Ipv6Array[4], Ipv6Array[5], Ipv6Array[6], Ipv6Array[7],
                    Ipv6Array[8], Ipv6Array[9], Ipv6Array[10], Ipv6Array[11],
                    Ipv6Array[12], Ipv6Array[13], Ipv6Array[14], Ipv6Array[15]);
        } else {
            logD(RFX_LOG_TAG, "convertStringIpv6ToByteArray success:%d", ret);
        }
        return 0;
    }
    logE(RFX_LOG_TAG, "convertStringIpv6ToByteArray failed:%d", ret);
    return -1;
}

string RmmMobileWifiRequestHandler::filterIpv6InetAddress(string ipv6) {
    string Inet6Address;
    string scopeId;

    if (ipv6.find("%") != string::npos) {
        vector<string> address = splitString(ipv6, "%");
        uint16_t len = address.size();
        if (len > 1) {
            Inet6Address = address[0];
            scopeId = address[1];
            if (UNIT_TEST) {
                logD(RFX_LOG_TAG, "filterIpv6InetAddress Inet6Address:%s,scopeId:%s",
                        Inet6Address.data(), scopeId.data());
            } else {
                logD(RFX_LOG_TAG, "filterIpv6InetAddress scopeId:%s", scopeId.data());
            }
        } else {
            Inet6Address = ipv6;
        }
    } else {
        Inet6Address = ipv6;
    }
    return Inet6Address;
}

void RmmMobileWifiRequestHandler::setWifiIpAddress(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFIADDR=<ifname>,<ipv4>,<ipv6>,<ipv4PrefixLen>,<ipv6PrefixLen>,
     *         <ipv4gateway>,<ipv6gateway>,<dns_count>, <dns_1>, <dns_2>,..., <dns_n>
     * <ifname>: interface name, such as wlan0
     * <ipv4>: IPV4 address
     * <ipv6>: IPV6 address
     * <ipv4PrefixLen>: IPV4 address prefix length
     * <ipv6PrefixLen>: IPV6 address prefix length
     * <ipv4gateway>: IPV4 gateway address
     * <ipv6gateway>: IPV6 gateway address
     * <dns_count>: dns server count
     * <dns_1>, <dns_2>,... <dns_n>: dns server addresses
     */
    char** params = (char**)msg->getData()->getData();
    int dataLen =  msg->getData()->getDataLength() / sizeof(char*);
    logD(RFX_LOG_TAG, "setWifiIpAddress, original dataLen: %d", dataLen);
    if (dataLen < WIFI_ADDR_PARAMS_COUNT) {
        handleErrorResponse(msg);
        return;
    }

    // Google HIDL service changes "" in java as null in cpp
    char* ifname = params[0]? params[0]: (char*)"";
    char* ipv4 = (params[1] == NULL) ? (char*)"" : params[1];
    char* ipv6 = (params[2] == NULL) ? (char*)"" : params[2];
    char* ipv4PrefixLen = (params[3] == NULL) ? (char*)"-1" : params[3];
    char* ipv6PrefixLen = (params[4] == NULL) ? (char*)"-1" : params[4];
    char* ipv4gateway = (params[5] == NULL) ? (char*)"" : params[5];
    char* ipv6gateway = (params[6] == NULL) ? (char*)"" : params[6];
    char* dnsCount = (params[7] == NULL) ? (char*)"0" : params[7];
    int16_t dnsCount_tlv = atoi(dnsCount);
    char* dnsAddresses = (params[8] == NULL) ? (char*)"" : params[8];

    uint8_t ipv4PrefixLen_tlv = 0;
    if (strncmp(ipv4PrefixLen, "-1", 2) != 0) {
        ipv4PrefixLen_tlv = (uint8_t)atoi(ipv4PrefixLen);
    }
    uint8_t ipv6PrefixLen_tlv = 0;
    if (strncmp(ipv6PrefixLen, "-1", 2) != 0) {
        ipv6PrefixLen_tlv = (uint8_t)atoi(ipv6PrefixLen);
    }

    if (wpfaSetWifiIp != NULL && wpfaSetIpRoute != NULL) {
        wpfaSetWifiIp(ifname, ipv4, ipv6);
        wpfaSetIpRoute(ifname);
    }

    // Ut Test
    if (UNIT_TEST) {
        logD(RFX_LOG_TAG, "setWifiIpAddress +EWIFIADDR:%s,%s,%s,%d,%d,%s,%s,%d,%s",
            ifname, ipv4, ipv6, ipv4PrefixLen_tlv, ipv6PrefixLen_tlv,
            ipv4gateway, ipv6gateway, dnsCount_tlv, dnsAddresses);
    } else {
        logD(RFX_LOG_TAG, "setWifiIpAddress ipv4PrefixLen:%d, ipv6PrefixLen:%d, dnsCount:%d",
            ipv4PrefixLen_tlv, ipv6PrefixLen_tlv, dnsCount_tlv);
    }

    uint8_t ipv4_array[MIPC_FIX_WFC_IPV4_LEN] = {0};
    convertStringIpv4ToByteArray(ipv4, ipv4_array, MIPC_FIX_WFC_IPV4_LEN);

    uint8_t ipv6_array[MIPC_FIX_WFC_IPV6_LEN] = {0};
    convertStringIpv6ToByteArray(ipv6, ipv6_array, MIPC_FIX_WFC_IPV6_LEN);

    uint8_t ipv4gateway_array[MIPC_FIX_WFC_IPV4_LEN] = {0};
    convertStringIpv4ToByteArray(ipv4gateway, ipv4gateway_array, MIPC_FIX_WFC_IPV4_LEN);

    uint8_t ipv6gateway_array[MIPC_FIX_WFC_IPV6_LEN] = {0};
    convertStringIpv6ToByteArray(ipv6gateway, ipv6gateway_array, MIPC_FIX_WFC_IPV6_LEN);

    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_WIFI_INFO_NTF, msg->getSlotId());
    requestData->addMipcTlvStr(MIPC_WFC_WIFI_INFO_NTF_T_IFNAME, ifname);
    requestData->addMipcTlv(MIPC_WFC_WIFI_INFO_NTF_T_IPV4, MIPC_FIX_WFC_IPV4_LEN, ipv4_array);
    requestData->addMipcTlv(MIPC_WFC_WIFI_INFO_NTF_T_IPV6, MIPC_FIX_WFC_IPV6_LEN, ipv6_array);
    if (ipv4PrefixLen_tlv > 0) {
        requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_IPV4_PREFIX_LEN, ipv4PrefixLen_tlv);
    }
    if (ipv6PrefixLen_tlv > 0) {
        requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_IPV6_PREFIX_LEN, ipv6PrefixLen_tlv);
    }
    requestData->addMipcTlv(MIPC_WFC_WIFI_INFO_NTF_T_IPV4_GATEWAY, MIPC_FIX_WFC_IPV4_LEN, ipv4gateway_array);
    requestData->addMipcTlv(MIPC_WFC_WIFI_INFO_NTF_T_IPV6_GATEWAY, MIPC_FIX_WFC_IPV6_LEN, ipv6gateway_array);
    requestData->addMipcTlvUint8(MIPC_WFC_WIFI_INFO_NTF_T_IP_UPDATE, MIPC_BOOLEAN_TRUE);

    //add dns array
    if (dnsCount_tlv > 0) {
        string dataSet(dnsAddresses);
        vector<string> dnsArray = splitString(dataSet, ",");
        if (dnsArray.empty() || dnsCount_tlv > dnsArray.size()) {
            logE(RFX_LOG_TAG, "setWifiIpAddress: dns count error");
            handleErrorResponse(msg);
            return;
        }

        mipc_msg_t *request = requestData->getData();
        mipc_msg_tlv_array_t *array;
        array = mipc_msg_add_tlv_array(request,
                MIPC_WFC_WIFI_INFO_NTF_T_DNS, dnsCount_tlv);
        char* tmpDns;
        uint8_t ipv4dns_array[MIPC_FIX_WFC_IPV4_LEN] = {0};
        uint8_t ipv6dns_array[MIPC_FIX_WFC_IPV6_LEN] = {0};
        for (int i = 0; i < dnsCount_tlv; i++) {
            //remove "" in string
            string dns = dnsArray[i].substr(1, dnsArray[i].length() - 2);
            tmpDns = dns.data();
            if (UNIT_TEST) {
                logD(RFX_LOG_TAG, "setWifiIpAddress dns:%s", tmpDns);
            } else {
                logD(RFX_LOG_TAG, "setWifiIpAddress dns:%d", i);
            }
            if (dnsArray[i].find(".") == string::npos) {
                //filter ipv6 scope id
                string Inet6Address = filterIpv6InetAddress(dns);
                tmpDns = Inet6Address.data();
                //ipv6 dns
                memset(ipv6dns_array, 0, MIPC_FIX_WFC_IPV6_LEN);
                if (tmpDns != NULL) {
                    convertStringIpv6ToByteArray(tmpDns, ipv6dns_array, MIPC_FIX_WFC_IPV6_LEN);
                } else {
                    logE(RFX_LOG_TAG, "setWifiIpAddress tmpDns NULL");
                }
                mipc_msg_add_idx(request, array, i, MIPC_FIX_WFC_IPV6_LEN, ipv6dns_array);
            } else {
                //ipv4 dns
                memset(ipv4dns_array, 0, MIPC_FIX_WFC_IPV4_LEN);
                convertStringIpv4ToByteArray(tmpDns, ipv4dns_array, MIPC_FIX_WFC_IPV4_LEN);
                mipc_msg_add_idx(request, array, i, MIPC_FIX_WFC_IPV4_LEN, ipv4dns_array);
            }
        }
    }

    //send NTF notify
    callToMipcMsgNtf(requestData);
    handleNtfCmdResponse(msg);
}

void RmmMobileWifiRequestHandler::setGeoLocation(const sp<RfxMclMessage>& msg) {
    /* AT+EIMSGEO=
     * <account_id>: request id, 0~7
     * <broadcast_flag>: 0, 1
     * <latitude>: latitude from GPS, 0 as failed
     * <longitude>: longitude from GPS, 0 as failed
     * <accurate>: accurate from GPS, 0 as failed
     * <method>: Location information from Fwk type, Network or GPS
     * <city>: City
     * <state>: State
     * <zip>: Zip
     * <country>: country
     * <ue_wifi_mac>: UE Wi-Fi interface mac address
     * <confidence>: confidence which used for transferred accurate
     * <altitude>: Altitude
     * <majorAxisAccuracy>: Major axis accuracy
     * <minorAxisAccuracy>: Minor axis accuracy
     * <vericalAxisAccuracy>: Verical axis accuracy
     */
    char** params = (char**)msg->getData()->getData();
    int dataLen =  msg->getData()->getDataLength() / sizeof(char*);
    logD(RFX_LOG_TAG, "setGeoLocation, original dataLen: %d", dataLen);
    if (dataLen < GEO_PARAMS_COUNT) {
        handleErrorResponse(msg);
        return;
    }

    char* accountId = (params[0] == NULL) ? (char*)"0" : params[0];
    uint8_t AccountId_tlv = (uint8_t)atoi(accountId);
    char* broadcastFlag = (params[1] == NULL) ? (char*)"0" : params[1];
    uint8_t BroadcastFlag_tlv = (uint8_t)atoi(broadcastFlag);
    char* latitude = (params[2] == NULL) ? (char*)"0" : params[2];
    char* longitude = (params[3] == NULL) ? (char*)"0" : params[3];
    char* accurate = (params[4] == NULL) ? (char*)"0" : params[4];

    // Google HIDL service changes "" in java as null in cpp
    char* method = (params[5] == NULL) ? (char*)"" : params[5];
    char* city = (params[6] == NULL) ? (char*)"" : params[6];
    char* state = (params[7] == NULL) ? (char*)"" : params[7];
    char* zip = (params[8] == NULL) ? (char*)"" : params[8];
    char* country = (params[9] == NULL) ? (char*)"" : params[9];
    char* ueWlanMac = (params[10] == NULL) ? (char*)"" : params[10];

    char* confidence = (params[11] == NULL) ? (char*)"0" : params[11];
    uint32_t Confidence_tlv = (uint32_t)atoi(confidence);
    char* altitude = (params[12] == NULL) ? (char*)"0" : params[12];
    char* majorAxisAccuracy = (params[13] == NULL) ? (char*)"0" : params[13];
    char* minorAxisAccuracy = (params[14] == NULL) ? (char*)"0" : params[14];
    char* vericalAxisAccuracy = (params[15] == NULL) ? (char*)"0" : params[15];

    // Ut Test
    if (UNIT_TEST) {
        logD(RFX_LOG_TAG, "setGeoLocation +EIMSGEO:%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,%s",
            AccountId_tlv, BroadcastFlag_tlv, latitude, longitude, accurate,
            method, city, state, zip, country, ueWlanMac, Confidence_tlv,
            altitude, majorAxisAccuracy, minorAxisAccuracy, vericalAxisAccuracy);
    } else {
        logD(RFX_LOG_TAG, "setGeoLocation +EIMSGEO:%d,%d,%s,%d",
            AccountId_tlv, BroadcastFlag_tlv, accurate, Confidence_tlv);
    }

    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_SYS_SET_GEO_LOCATION_REQ, msg->getSlotId());
    requestData->addMipcTlvUint8(MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCOUNT_ID, AccountId_tlv);
    requestData->addMipcTlvUint8(MIPC_SYS_SET_GEO_LOCATION_REQ_T_BROADCAST_FLAG, BroadcastFlag_tlv);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_LATITUDE, latitude);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_LONGITUDE, longitude);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY, accurate);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_METHOD, method);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_CITY, city);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_STATE, state);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_ZIP, zip);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_COUNTRY_CODE, country);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_UE_WLAN_MAC, ueWlanMac);
    requestData->addMipcTlvUint32(MIPC_SYS_SET_GEO_LOCATION_REQ_T_CONFIDENCE, Confidence_tlv);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_ALTITUDE, altitude);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_SEMI_MAJOR_AXIS, majorAxisAccuracy);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_SEMI_MINOR_AXIS, minorAxisAccuracy);
    requestData->addMipcTlvStr(MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_VERTICAL_AXIS, vericalAxisAccuracy);
    callToMipcMsgAsync(msg, requestData);
}

void RmmMobileWifiRequestHandler::setEmergencyAddressId(const sp<RfxMclMessage>& msg) {
    /* AT+EIMSAID = <aid>
     * <aid>: Access Id from Settings UI
     */
    char** params = (char**)msg->getData()->getData();
    if (params == NULL || params[0] == NULL) {
        logE(RFX_LOG_TAG, "setEmergencyAddressId: params null");
        handleErrorResponse(msg);
        return;
    }

    logD(RFX_LOG_TAG, "setEmergencyAddressId aid: %s", params[0]);
    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_SET_EMC_AID_REQ, msg->getSlotId());
    requestData->addMipcTlvStr(MIPC_WFC_SET_EMC_AID_REQ_T_AID, params[0]);
    callToMipcMsgAsync(msg, requestData);
}

void RmmMobileWifiRequestHandler::notifyEPDGScreenState(const sp<RfxMclMessage>& msg) {
    /* AT+ESCREENSTATE= <state>
     * <state>: Screen state
     */
    int *params = (int *)msg->getData()->getData();
    uint8_t state = (uint8_t)params[0];

    logD(RFX_LOG_TAG, "notifyEPDGScreenState state:%d", state);
    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_EPDG_SCREEN_STATE_NTF, msg->getSlotId());
    requestData->addMipcTlvUint8(MIPC_WFC_EPDG_SCREEN_STATE_NTF_T_STATE, state);
    //send NTF notify
    callToMipcMsgNtf(requestData);
    handleNtfCmdResponse(msg);
}

void RmmMobileWifiRequestHandler::querySsacStatus(const sp<RfxMclMessage>& msg) {
    /* AT+CSSAC=?
     * return: <BFVoice>,<BFVideo>,<BTVoice>,<BTVoice>
     */
    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_WFC_CSSAC_REQ, msg->getSlotId());
    callToMipcMsgAsync(msg, requestData);
}

void RmmMobileWifiRequestHandler::querySsacStatusResponse(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    RIL_Errno rilErrno;
    int responses[4] = {0};

    if (result == MIPC_RESULT_SUCCESS) {
        rilErrno = RIL_E_SUCCESS;
    } else {
        rilErrno = RIL_E_GENERIC_FAILURE;
    }
    // BFVoice
    responses[0] = mipcData->getMipcUint16Val(MIPC_WFC_CSSAC_CNF_T_BF_VOICE, 0);
    // BFVideo
    responses[1] = mipcData->getMipcUint16Val(MIPC_WFC_CSSAC_CNF_T_BF_VIDEO, 0);
    // BTVoice
    responses[2] = mipcData->getMipcUint16Val(MIPC_WFC_CSSAC_CNF_T_BT_VOICE, 0);
    // BTVideo
    responses[3] = mipcData->getMipcUint16Val(MIPC_WFC_CSSAC_CNF_T_BT_VIDEO, 0);
    logD(RFX_LOG_TAG, "querySsacStatusResponse, reuslt: %d, BFVoice:%d", result, responses[0]);

    // response to Telcore
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrno,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);
    responseToTelCore(response);
}

vector<string> RmmMobileWifiRequestHandler::splitString(string str, string c) {
    string::size_type pos;
    vector<string> result;
    str += c;
    string::size_type len = str.size();
    for (string::size_type i = 0; i < len; i ++) {
        pos = str.find(c, i);
        if (pos < len) {
            string temp = str.substr(i, pos-i);
            result.push_back(temp);
            i = pos + c.size() -1;
        }
    }
    return result;
}
