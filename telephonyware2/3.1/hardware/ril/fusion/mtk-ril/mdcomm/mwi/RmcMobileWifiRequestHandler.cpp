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

// MTK fusion include
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"

// MWI Local include
#include "RmcMobileWifiRequestHandler.h"
#include "RmcMobileWifiInterface.h"

#define RFX_LOG_TAG "RmcMwi"

 // register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcMobileWifiRequestHandler, RIL_CMD_PROXY_1);

// register request to RfxData
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_ENABLED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_ASSOCIATED);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WFC_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_GET_WFC_CONFIG);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_SIGNAL_LEVEL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_GEO_LOCATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_WIFI_IP_ADDRESS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_NATT_KEEP_ALIVE_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE);


static const int requests[] = {
    RFX_MSG_REQUEST_SET_WIFI_ENABLED,
    RFX_MSG_REQUEST_SET_WIFI_ASSOCIATED,
    RFX_MSG_REQUEST_SET_WFC_CONFIG,
    RFX_MSG_REQUEST_GET_WFC_CONFIG,
    RFX_MSG_REQUEST_SET_WIFI_SIGNAL_LEVEL,
    RFX_MSG_REQUEST_SET_GEO_LOCATION,
    RFX_MSG_REQUEST_SET_WIFI_IP_ADDRESS,
    RFX_MSG_REQUEST_SET_EMERGENCY_ADDRESS_ID,
    RFX_MSG_REQUEST_SET_NATT_KEEP_ALIVE_STATUS,
    RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE,
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

RmcMobileWifiRequestHandler::RmcMobileWifiRequestHandler(
    int slot_id, int channel_id) : RfxBaseHandler(slot_id, channel_id) {
    // register to handle request
    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcMobileWifiRequestHandler::~RmcMobileWifiRequestHandler() {
}

void RmcMobileWifiRequestHandler::onHandleTimer() {
}

void RmcMobileWifiRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int requestId = msg->getId();

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
        case RFX_MSG_REQUEST_SET_NATT_KEEP_ALIVE_STATUS:
            setNattKeepAliveStatus(msg);
            break;
        case RFX_MSG_REQUEST_NOTIFY_EPDG_SCREEN_STATE:
            notifyEPDGScreenState(msg);
            break;
        default:
            break;
    }
}

void RmcMobileWifiRequestHandler::setWifiEnabled(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFIEN=<ifname>,<enabled>
     * <ifname>: interface name, such as wlan0
     * <enabled>: 0 = disable; 1 = enable
     * <flightModeOn>: 0 = disable; 1 = enable
     */
    char** params = (char**)msg->getData()->getData();
    char* atWifiEnCmd = AT_SET_WIFI_ENABLE;
    char* atAPMCmd = AT_SET_AIRPLANE_MODE;
    char* ifname = params[0];
    int enabled = atoi(params[1]);
    int allCause = atoi(params[2]);
    sp<RfxAtResponse> p_response;

    if (enabled == 0) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_IMS_ESIPCPI_REASON, -1);
    }

    if ((allCause & EWIFIEN_NEED_SEND_WIFI_ENABLED) == EWIFIEN_NEED_SEND_WIFI_ENABLED) {
        int cause = (allCause & (EWIFIEN_CAUSE)) == EWIFIEN_CAUSE ? 1 : 0;
        int wifiEnabled = (allCause & (EWIFIEN_WIFI_ENABLED_STATE)) == EWIFIEN_WIFI_ENABLED_STATE ? 1 : 0;
        String8 cmd = String8::format("%s=\"%s\",%d,%d",
                atWifiEnCmd, ifname, wifiEnabled, cause);
        handleCmdWithVoidResponse(msg, cmd);
    }

    if ((allCause & EWIFIEN_NEED_SEND_AP_MODE) == EWIFIEN_NEED_SEND_AP_MODE) {
        int flightModeOn = (allCause & (EWIFIEN_AP_MODE_STATE)) == EWIFIEN_AP_MODE_STATE ? 1 : 0;
        String8 apCmd = String8::format("%s=%d", atAPMCmd, flightModeOn);
        p_response = atSendCommand(apCmd);
        if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
            logD(RFX_LOG_TAG, "Not support EAPMODE command.");
        }
    }
}

void RmcMobileWifiRequestHandler::setWifiAssociated(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFIASC=<ifname>,<assoc>,<ssid>,<ap_mac>,<wifi_type>,<mtu>,<ue_mac>,<conn_state>
     * <ifname>: interface name, such as wlan0
     * <assoc>: 0 = not associated; 1 = associated
     * <ssid>: wifi ap ssid when associated, 0 if assoc = 0
     * <ap_mac>: wifi ap mac addr, 0 if assoc = 0
     * <wifi_type>: wifi type: such as 802.11b, 802.11n...
     * <mtu>: mtu size
     * <ue_mac>: ue mac address
     * <conn_state> wifi connect state
     */
    char** params = (char**)msg->getData()->getData();
    int dataLen =  msg->getData()->getDataLength() / sizeof(char*);
    logD(RFX_LOG_TAG, "setWifiAssociated, original dataLen: %d", dataLen);
    if (dataLen < WIFI_ASC_PARAMS_COUNT) {
        handleErrorResponse(msg);
        return;
    }

    char* atCmd = AT_SET_WIFI_ASSOCIATED;
    char* ifname = params[0]? params[0]: (char*)"" ;
    const char* assoc = params[1]? params[1]: (char*)"0" ;
    char* ssid = (atoi(assoc) == 0)? (char*)"0": params[2];
    char* ap_mac = (atoi(assoc) == 0)? (char*)"0": params[3];
    char* mtu = (atoi(assoc) == 0)? (char*)"0": params[4];
    char* ue_mac = (params[5])? params[5]: (char*)"0";
    char defaultWifiType[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("vendor.test.wifi.type", defaultWifiType, "802.11n");
    char* type = defaultWifiType;

    if (atoi(assoc) == 0) {
        type = (char*) "";
    }

    String8 cmd = String8::format("%s=\"%s\",%s,\"%s\",\"%s\",\"%s\",%s,\"%s\"",
            atCmd, ifname, assoc, ssid, ap_mac, type, mtu, ue_mac);
    handleCmdWithVoidResponse(msg, cmd);
}

void RmcMobileWifiRequestHandler::setWfcConfig(const sp<RfxMclMessage>& msg) {
    char** params = (char**)msg->getData()->getData();
    int setting = params[0]? atoi(params[0]): 0;
    logD(RFX_LOG_TAG, "setWfcConfig: %d", setting);
    switch (setting) {
        case WFC_SETTING_LOCATION_SETTING:
            setWfcConfig_LocationSetting(msg);
            break;
        default:
            handleVoidResponse(msg);
            break;
    }
}

void RmcMobileWifiRequestHandler::getWfcConfig(const sp<RfxMclMessage>& msg) {
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

void RmcMobileWifiRequestHandler::getWfcConfig_Default(const sp<RfxMclMessage>& msg) {
    RIL_Errno result = RIL_E_SUCCESS;
    int responses[1] = {0};
    responses[0] = WFC_FEATURE_UNKNOWN;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);

    // report to tcl
    responseToTelCore(response);
}

void RmcMobileWifiRequestHandler::getWfcConfig_WifiConnState(const sp<RfxMclMessage>& msg) {
    RIL_Errno result = RIL_E_SUCCESS;
    int responses[1] = {0};
    responses[0] = WFC_FEATURE_UNSUPPORTED;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxIntsData(responses, sizeof(responses) / sizeof(int)), msg, false);

    // report to tcl
    responseToTelCore(response);
}

void RmcMobileWifiRequestHandler::setWfcConfig_LocationSetting(const sp<RfxMclMessage>& msg) {
    char* atCmd = AT_SET_LOCATION_ENABLE;
    char** params = (char**)msg->getData()->getData();
    char* ifname = params[1]? params[1]: (char*)"" ; //ifName. e.g. ""locenable""
    char* setting = params[2]? params[2]: (char*)"" ; //setting value
    int enabled = atoi(setting);

    logD(RFX_LOG_TAG, "setWfcConfig_LocationSetting: %s, %d", ifname, enabled);
    String8 cmd = String8::format("%s=%d", atCmd, enabled);
    handleCmdWithVoidResponse(msg, cmd);
}

void RmcMobileWifiRequestHandler::setWifiSignal(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFISIGLVL=<ifname>,<rssi>,<snr>
     * <ifname>: interface name, such as wlan0
     * <rssi>: rssi value
     * <snr>: string value
     */
    char** params = (char**)msg->getData()->getData();

    char* atCmd = AT_SET_WIFI_SIGNAL_LEVEL;
    char* ifname = params[0];
    int rssi = atoi(params[1]);
    char* snr = params[2];

    String8 cmd = String8::format("%s=\"%s\",%d,\"%s\"", atCmd, ifname, rssi, snr);
    handleCmdWithVoidResponse(msg, cmd);
}

void RmcMobileWifiRequestHandler::setWifiIpAddress(const sp<RfxMclMessage>& msg) {
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

    char* atCmd = AT_SET_WIFI_IP_ADDRESS;
    // Google HIDL service changes "" in java as null in cpp
    char* ifname = params[0]? params[0]: (char*)"" ;
    char* ipv4 = (params[1] == NULL) ? (char*)"" : params[1];
    char* ipv6 = (params[2] == NULL) ? (char*)"" : params[2];
    char* ipv4PrefixLen = (params[3] == NULL) ? (char*)"-1" : params[3];
    if (strncmp(ipv4PrefixLen,"-1", 2) == 0) ipv4PrefixLen = (char*)"";
    char* ipv6PrefixLen = (params[4] == NULL) ? (char*)"-1" : params[4];
    if (strncmp(ipv6PrefixLen,"-1", 2) == 0) ipv6PrefixLen = (char*)"";
    char* ipv4gateway = (params[5] == NULL) ? (char*)"" : params[5];
    char* ipv6gateway = (params[6] == NULL) ? (char*)"" : params[6];
    char* dnsCount = (params[7] == NULL) ? (char*)"0" : params[7];
    char* dnsAddresses = (params[8] == NULL) ? (char*)"" : params[8];

    String8 cmd = String8::format("%s=\"%s\",\"%s\",\"%s\",%s,%s,\"%s\",\"%s\",%s,%s",
            atCmd, ifname, ipv4, ipv6,
            ipv4PrefixLen, ipv6PrefixLen,
            ipv4gateway, ipv6gateway,
            dnsCount, dnsAddresses);
    handleCmdWithVoidResponse(msg, cmd);
}

void RmcMobileWifiRequestHandler::handleVoidResponse(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleVoidResponse, msgId:%d", msg->getId());
    // report to tcl
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcMobileWifiRequestHandler::handleErrorResponse(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleErrorResponse, msgId:%d", msg->getId());
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcMobileWifiRequestHandler::setGeoLocation(const sp<RfxMclMessage>& msg) {
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
    char* atCmd = AT_SET_GEO_LOCATION;
    int dataLen =  msg->getData()->getDataLength() / sizeof(char*);
    logD(RFX_LOG_TAG, "setGeoLocation, original dataLen: %d", dataLen);
    if (dataLen < GEO_PARAMS_COUNT) {
        handleErrorResponse(msg);
        return;
    }

    // Google HIDL service changes "" in java as null in cpp
    char* method = (params[5] == NULL) ? (char*)"" : params[5];
    char* city = (params[6] == NULL) ? (char*)"" : params[6];
    char* state = (params[7] == NULL) ? (char*)"" : params[7];
    char* zip = (params[8] == NULL) ? (char*)"" : params[8];
    char* country = (params[9] == NULL) ? (char*)"" : params[9];
    char* ueWlanMac = (params[10] == NULL) ? (char*)"" : params[10];

    String8 cmd = String8::format("%s=%s,%s,%s,%s,%s,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%s,%s,%s,%s,%s",
        atCmd, params[0], params[1], params[2], params[3], params[4],
        method, city, state, zip, country, ueWlanMac, params[11],
        params[12], params[13], params[14], params[15]);
    //logD(RFX_LOG_TAG,"setGeoLocation, send AT: %s", cmd.string());
    handleCmdWithVoidResponse(msg, cmd);
}

void RmcMobileWifiRequestHandler::setEmergencyAddressId(const sp<RfxMclMessage>& msg) {
    /* AT+EIMSAID = <aid>
     * <aid>: Access Id from Settings UI
     */
    char* atCmd = AT_SET_ECC_AID;
    char** params = (char**)msg->getData()->getData();

    logD(RFX_LOG_TAG, "setEmergencyAddressId aid: %s", params[0]);
    String8 cmd = String8::format("%s=\"%s\"", atCmd, params[0]);
    handleCmdWithVoidResponse(msg, cmd);
}

void RmcMobileWifiRequestHandler::setNattKeepAliveStatus(const sp<RfxMclMessage>& msg) {
    /* AT+EWIFINATT= <ifname>,<enable>,<src_ip>,<src_port>,<dst_ip>,<dst_port>
     * <ifname>: interface name, such as wlan0
     * <enable>: enabled, 0 = disable; 1 = enabled
     * <src_ip>: source IP
     * <src_port>: source port
     * <dst_ip>: destination IP
     * <dst_port>: destination port
     */
    char** params = (char**)msg->getData()->getData();
    char* atCmd = AT_SET_NATT_KEEP_ALIVE_STATUS;

    String8 cmd = String8::format("%s=\"%s\",%s,\"%s\",%s,\"%s\",%s", atCmd,
        params[0], params[1], params[2], params[3], params[4], params[5]);

    handleCmdWithVoidResponse(msg, cmd);
}

void RmcMobileWifiRequestHandler::notifyEPDGScreenState(const sp<RfxMclMessage>& msg) {
    /* AT+ESCREENSTATE= <state>
     * <state>: Screen state
     */
    char* atCmd = AT_SET_EPDG_SCREEN_STATE;
    int *params = (int *)msg->getData()->getData();

    logD(RFX_LOG_TAG, "notifyEPDGScreenState: state:%d", params[0]);
    String8 cmd = String8::format("%s = %d", atCmd, params[0]);
    handleCmdWithVoidResponse(msg, cmd);
}
