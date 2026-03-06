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
 * MediaTek Inc. (C) 2015. All rights reserved.
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

#include "RpDataUtils.h"

//#include <pal/pal_nm.h>
#include <cutils/jstring.h>
#include <prop/properties.h>
#include <linux/rtnetlink.h>
#include <string>


/*****************************************************************************
 * Class RpDataUtils
 *****************************************************************************/
#define RP_DC_UTIL_LOG_TAG  "RpDataUtils"
#ifdef UPDATE_APN
const int RpDataUtils::MOBILE_APN = 1;  // IOT_APN_1 = 1;
const int RpDataUtils::RCS_APN = 2;  // IOT_APN_2 = 2;
const int RpDataUtils::BIP_APN = 3;  // IOT_APN_3 = 3;
const int RpDataUtils::IOT_APN_4 = 4;
const int RpDataUtils::IOT_APN_5 = 5;
const int RpDataUtils::IOT_APN_6 = 6;
const int RpDataUtils::IOT_APN_7 = 7;
const int RpDataUtils::IOT_APN_8 = 8;

// const int RpDataUtils::MOBILE_APN = 10;
const int RpDataUtils::MMS_APN = 11;
const int RpDataUtils::WAP_APN = 12;
const int RpDataUtils::SUPL_APN = 13;

#endif //UPDATE_APN

int RpDataUtils::mNetId(6000);
RpDataUtils::RpDataUtils() {
}

RpDataUtils::~RpDataUtils() {
}

const char* RpDataUtils::requestOrUrcToString(int reqId) {
    switch (reqId) {
    case RIL_REQUEST_SETUP_DATA_CALL:
        return "SETUP_DATA_CALL";
    case RIL_REQUEST_DEACTIVATE_DATA_CALL:
        return "DEACTIVATE_DATA_CALL";
    case RIL_REQUEST_ALLOW_DATA:
        return "ALLOW_DATA";
    case RIL_UNSOL_DATA_CALL_LIST_CHANGED:
        return "DATA_CALL_LIST_CHANGED";
    case RIL_UNSOL_DATA_ALLOWED:
        return "DATA_ALLOWED";
    case RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED:
        return "RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED";
    case RIL_UNSOL_SETUP_PDN_STATE_CHANGED:
        return "RIL_UNSOL_SETUP_PDN_STATE_CHANGED";
    case RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE:
        return "RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE";
    case RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED:
        return "RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED";
#ifdef UPDATE_APN
        case RIL_REQUEST_LOCAL_UPDATE_APN:
            return "LOCAL_UPDATE_APN";
        case RIL_REQUEST_LOCAL_GET_APN:
            return "LOCAL_GET_APN";
#endif
    default:
        return "UNKNOWN_REQUEST";
    }
}

vector<DataCallResponse> RpDataUtils::parseDataConnectionInfo(Parcel* parcel) {
    int ver = parcel->readInt32();
    int num = parcel->readInt32();
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::RpDataConnectionInfo ver: %d, num: %d", ver, num);

    vector<DataCallResponse> list;
    for (int i = 0; i < num; i++) {
        DataCallResponse dataCallResponse;

        dataCallResponse.status  = parcel->readInt32();  // status
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:dataCallResponse.status %d",dataCallResponse.status);
        dataCallResponse.suggestedRetryTime = parcel->readInt32();  // suggestedRetryTime
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:dataCallResponse.suggestedRetryTime %d",dataCallResponse.suggestedRetryTime);
        dataCallResponse.cid = parcel->readInt32();  // cid(interfaceId in rilj)
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:dataCallResponse.cid %d",dataCallResponse.cid);
        dataCallResponse.active = parcel->readInt32();  // active
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:dataCallResponse.active %d",dataCallResponse.active);
        string protocolType; // protocolType
        char * cp_protocolType = RpDataUtils::strdupReadString(parcel);
        if (cp_protocolType != NULL) {
            protocolType = std::string(cp_protocolType);
        }
        RpDataUtils::freeMemory(cp_protocolType);
        dataCallResponse.type = protocolType;
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:protocolType %s",protocolType.c_str());
        string ifname; // ifname
        char * cp_ifname = RpDataUtils::strdupReadString(parcel);
        if (cp_ifname != NULL) {
            ifname = std::string(cp_ifname);
        }
        RpDataUtils::freeMemory(cp_ifname);
        dataCallResponse.ifname = ifname;
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:ifname %s",ifname.c_str());
        string addresses; // addresses
        char * cp_addresses = RpDataUtils::strdupReadString(parcel);
        if (cp_addresses != NULL) {
            addresses = std::string(cp_addresses);
        }
        RpDataUtils::freeMemory(cp_addresses);
        dataCallResponse.addresses = addresses;
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:addresses %s",addresses.c_str());
        string dnses; // dnses
        char * cp_dnses = RpDataUtils::strdupReadString(parcel);
        if (cp_dnses != NULL) {
            dnses = std::string(cp_dnses);
        }
        RpDataUtils::freeMemory(cp_dnses);
        dataCallResponse.dnses = dnses;
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:dnses %s",dnses.c_str());
        string gateways; // gateways
        char * cp_gateways = RpDataUtils::strdupReadString(parcel);
        if (cp_gateways != NULL) {
            gateways = std::string(cp_gateways);
        }
        RpDataUtils::freeMemory(cp_gateways);
        dataCallResponse.gateways = gateways;
        dataCallResponse.initAddress();
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:gateways %s",gateways.c_str());
        string pcscf; // pcscf
        char * cp_pcscf = RpDataUtils::strdupReadString(parcel);
        if (cp_pcscf != NULL) {
            pcscf = std::string(cp_pcscf);
        }
        RpDataUtils::freeMemory(cp_pcscf);
        dataCallResponse.pcscf = pcscf;
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:pcscf %s",pcscf.c_str());
        dataCallResponse.mtu = parcel->readInt32();  // mtu
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:mtu %d",dataCallResponse.mtu);
        dataCallResponse.mtuV6 = parcel->readInt32();  // mtu
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo:mtuV6 %d",dataCallResponse.mtuV6);
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "RpDataUtils::parseDataConnectionInfo dataCallResponse: %s", dataCallResponse.logInfo().c_str());
        list.push_back(dataCallResponse);
    }
    return list;
}

void RpDataUtils::writeStringToParcel(Parcel *p, const char *s) {
    char16_t *s16;
    size_t s16_len = 0;
    s16 = strdup8to16(s, &s16_len);
    if (s16 != NULL) {
        p->writeString16(s16, s16_len);
        free(s16);
    }
}

void RpDataUtils::fillDataCallResponse(Parcel* parcel, MTK_Data_Call_Response_v1 dataCallResponse) {

    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "RpDataUtils::fillDataCallResponse: netId: %d, pdnState: %s, status: %d, cId: %d, apnType: %s, protocolType: %s, ifaceName: %s, address: %s, dns: %s, gateway: %s, pcscf: %s, mtu: %d, mtuV6: %d",
            dataCallResponse.netId, RpDataUtils::apnState2string(RIL_Data_Call_PdnState(dataCallResponse.pdnState)), dataCallResponse.status, dataCallResponse.cId, dataCallResponse.apnType, dataCallResponse.type,
            dataCallResponse.ifname, dataCallResponse.addresses, dataCallResponse.dnses, dataCallResponse.gateways, dataCallResponse.pcscf, dataCallResponse.mtu, dataCallResponse.mtuV6);

    parcel->writeInt32(dataCallResponse.netId); // netId
    parcel->writeInt32(dataCallResponse.pdnState); // pdn state
    parcel->writeInt32(dataCallResponse.status); // status
    parcel->writeInt32(dataCallResponse.cId); // cid
    writeStringToParcel(parcel, dataCallResponse.apnType); // apn type
    writeStringToParcel(parcel, dataCallResponse.type);  // protocol type
    writeStringToParcel(parcel, dataCallResponse.ifname);  // ifname
    writeStringToParcel(parcel, dataCallResponse.addresses);  // address
    writeStringToParcel(parcel, dataCallResponse.dnses); // dns
    writeStringToParcel(parcel, dataCallResponse.gateways); // gateway
    writeStringToParcel(parcel, dataCallResponse.pcscf); // pcscf
    parcel->writeInt32(dataCallResponse.mtu); // mtu
    parcel->writeInt32(dataCallResponse.mtuV6); // mtuV6
}

void RpDataUtils::parseDataCallResponse(MTK_Data_Call_Response_v1 *dataCallResponse, Parcel* parcel) {
    dataCallResponse->netId = parcel->readInt32(); // netId
    dataCallResponse->pdnState = parcel->readInt32(); // pdn state
    dataCallResponse->status = parcel->readInt32(); // status
    dataCallResponse->cId = parcel->readInt32(); // cid
    dataCallResponse->apnType = strdupReadString(parcel); // apn type
    dataCallResponse->type = strdupReadString(parcel); // protocol type
    dataCallResponse->ifname = strdupReadString(parcel); // ifname
    dataCallResponse->addresses = strdupReadString(parcel); // address
    dataCallResponse->dnses = strdupReadString(parcel); // dns
    dataCallResponse->gateways = strdupReadString(parcel); // gateway
    dataCallResponse->pcscf = strdupReadString(parcel); // pcscf
    dataCallResponse->mtu = parcel->readInt32(); // mtu
    dataCallResponse->mtuV6 = parcel->readInt32(); // mtu

    RFX_LOG_I(RP_DC_UTIL_LOG_TAG, "RpDataUtils::parseDataCallResponse: netId: %d, pdnState: %s, status: %d, cId: %d, apnType: %s,"
            " protocolType: %s, ifaceName: %s, address: %s, dns: %s, gateway: %s, pcscf: %s, mtu: %d, mtuV6: %d",
            dataCallResponse->netId, RpDataUtils::apnState2string(RIL_Data_Call_PdnState(dataCallResponse->pdnState)),
            dataCallResponse->status, dataCallResponse->cId, dataCallResponse->apnType, dataCallResponse->type,
            dataCallResponse->ifname, dataCallResponse->addresses, dataCallResponse->dnses,
            dataCallResponse->gateways, dataCallResponse->pcscf, dataCallResponse->mtu, dataCallResponse->mtuV6);
}

void RpDataUtils::configNetwork(int netId, string interfaceName, string gateWay,
        string dns) {
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "configNetwork() don't support, just return");
    return;
#if 0
    int ret;

    vector<string> vGateWay;
    string gateWay_V4;
    string gateWay_V6;
    RpDataUtils::split(gateWay, ' ', vGateWay);
    vector<string>::iterator itorGate = vGateWay.begin();
    while (itorGate != vGateWay.end()) {
        if (itorGate->find('.') != string::npos) {
            gateWay_V4 = *itorGate;
        } else if (itorGate->find(':') != string::npos) {
            gateWay_V6 = *itorGate;
        }
        itorGate++;
    }

    vector<string> vSDns;
    vector<const char*> vCDns;
    RpDataUtils::split(dns, ' ', vSDns);
    vector<string>::iterator itorDns = vSDns.begin();
    while (itorDns != vSDns.end()) {
        vCDns.push_back(itorDns->c_str());
        itorDns++;
    }

    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "configNetwork() with netId: %d, interfaceName %s,"
            "gateWay_V4.c_str()=%s, gateWay_V6.c_str()=%s",
            netId, interfaceName.c_str(),
            gateWay_V4.c_str(), gateWay_V6.c_str());

    if (nm_network_create(netId, NULL) != 0) {
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG,"RpDataUtils::configNetwork: nm_network_create() fail,because the netid create"
            "before, clear the old interface configure!!");
        nm_network_interface_remove(netId, interfaceName.c_str());
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG,"RpDataUtils::configNetwork: nm_network_create() fail!!");
    }


    nm_network_policy_route_init();

    ret = nm_network_ipv4_policy_rule_modify(RTM_DELRULE,16000,netId,netId&0xFFFF,0xFFFF,NULL,NULL);
       RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv4_policy_rule_modify() DEL ret = %d", ret);

    ret = nm_network_ipv4_policy_route_modify(RTM_DELROUTE,netId,interfaceName.c_str());
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv4_policy_route_modify() DEL ret = %d", ret);

    ret = nm_network_ipv6_policy_rule_modify(RTM_DELRULE,16000,netId,netId&0xFFFF,0xFFFF,NULL,NULL);
      RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv6_policy_rule_modify() DEL ret = %d", ret);

    ret = nm_network_ipv6_policy_route_modify(RTM_DELROUTE,netId,interfaceName.c_str());
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv6_policy_route_modify() DEL ret = %d", ret);

    ret = nm_network_ipv4_policy_route_modify(RTM_NEWROUTE,netId,interfaceName.c_str());
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv4_policy_route_modify() NEW ret = %d", ret);

    ret = nm_network_ipv4_policy_rule_modify(RTM_NEWRULE,16000,netId,netId&0xFFFF,0xFFFF,NULL,NULL);
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv4_policy_rule_modify() NEW ret = %d", ret);

    ret = nm_network_ipv6_policy_route_modify(RTM_NEWROUTE,netId,interfaceName.c_str());
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv6_policy_route_modify() NEW ret = %d", ret);

    ret = nm_network_ipv6_policy_rule_modify(RTM_NEWRULE,16000,netId,netId&0xFFFF,0xFFFF,NULL,NULL);
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv6_policy_rule_modify() NEW ret = %d", ret);

    ret = nm_resolver_dns_cache_flush(netId);
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_resolver_dns_cache_flush() NEW ret = %d", ret);


    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_resolver_dns_set() with netId: %d, interface_name: %s, dns nubmer: %d",
            netId, interfaceName.c_str(), vCDns.size());
    if(vCDns.size() > 0 && nm_resolver_dns_set(netId, interfaceName.c_str(), &vCDns[0], vCDns.size(), NM_NETWORK_TYPE_INTERNET) !=0) {
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG,"RpDataUtils::configNetwork: nm_resolver_dns_set() fail!!");
        return;
    }
#endif
}

void RpDataUtils::tearDownNetwork(int netId, string interfaceName) {
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "tearDownNetwork() don't support, just return");
    return;
#if 0
    int ret;
    nm_network_policy_route_init();
    ret = nm_network_ipv4_policy_rule_modify(RTM_DELRULE,16000,netId,netId&0xFFFF,0xFFFF,NULL,NULL);
       RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv4_policy_rule_modify() DEL ret = %d", ret);

    ret = nm_network_ipv4_policy_route_modify(RTM_DELROUTE,netId,interfaceName.c_str());
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv4_policy_route_modify() DEL ret = %d", ret);

    ret = nm_network_ipv6_policy_rule_modify(RTM_DELRULE,16000,netId,netId&0xFFFF,0xFFFF,NULL,NULL);
      RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv6_policy_rule_modify() DEL ret = %d", ret);

    ret = nm_network_ipv6_policy_route_modify(RTM_DELROUTE,netId,interfaceName.c_str());
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_ipv6_policy_route_modify() DEL ret = %d", ret);


    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_resolver_dns_clear start.");
    if(nm_resolver_dns_clear(netId) != 0) {
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_resolver_dns_clear fail.");
    }

    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_interface_remove start.");
    if(nm_network_interface_remove(netId, interfaceName.c_str()) != 0) {
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_interface_remove fail.");
    }

    RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_destroy start.");
    if(nm_network_destroy(netId) != 0) {
        RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "nm_network_destroy fail.");
    }
#endif
}

char* RpDataUtils::strdupReadString(Parcel *p) {
    size_t stringlen;
    const char16_t *s16;
    s16 = p->readString16Inplace(&stringlen);
    return strndup16to8(s16, stringlen);
}

void RpDataUtils::freeMemory(char *&s) {
    free(s);
    s = NULL;
}

const char* RpDataUtils::apnState2string(RIL_Data_Call_PdnState apnState) {
    switch (apnState) {
    case RIL_Data_Call_PdnState::PDN_CONNECTED:
        return "PDN_CONNECTED";
    case RIL_Data_Call_PdnState::PDN_CONNECTING:
        return "PDN_CONNECTING";
    case RIL_Data_Call_PdnState::PDN_DISCONNECTED:
        return "PDN_DISCONNECTED";
    case RIL_Data_Call_PdnState::PDN_DISCONNECTING:
        return "PDN_DISCONNECTING";
    case RIL_Data_Call_PdnState::PDN_FAILED:
        return "PDN_FAILED";
    case RIL_Data_Call_PdnState::PDN_IDLE:
        return "PDN_IDLE";
    case RIL_Data_Call_PdnState::PDN_RETRYING:
        return "PDN_RETRYING";
    case RIL_Data_Call_PdnState::PDN_SCANNING:
        return "PDN_SCANNING";
    }
}

const char* RpDataUtils::pdnState2string(int pdnState) {
    switch (pdnState) {
    case 0:
        return "inactive";
    case 1:
        return "active (physical link down)";
    case 2:
        return "active (physical link up)";
    default:
        return "inactive";
    }
}
#ifdef UPDATE_APN
std::string  RpDataUtils::profileId2Type(int32_t profileId) {
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "profileId2Type id(%d), MOBILE_APN(%d), RCS_APN(%d)", profileId, MOBILE_APN, RCS_APN);
    std::string type("");
    switch (profileId) {
        case MOBILE_APN:  // public apn 1.
            type = std::string("default");
            break;
        case RCS_APN:  // private apn 2.
            type = std::string("rcs");
            break;
        case BIP_APN:  // private apn 3.
            type = std::string("bip");
            break;
        // case IOT_APN_1:
        //     type = std::string("iot_default");
        //     break;
        // case IOT_APN_2:
        //     type = std::string("iot_net_0");
        //     break;
        // case IOT_APN_3:
        //     type = std::string("iot_net_1");
        //     break;
        case IOT_APN_4:
            type = std::string("iot_net_2");
            break;
        case IOT_APN_5:
            type = std::string("iot_net_3");
            break;
        case IOT_APN_6:
            type = std::string("iot_net_4");
            break;
        case IOT_APN_7:
            type = std::string("iot_net_5");
            break;
        case IOT_APN_8:
            type = std::string("iot_net_6");
            break;
        case MMS_APN:
            type = std::string("mms");
            break;
        case WAP_APN:
            type = std::string("wap");
            break;
        case SUPL_APN:
            type = std::string("supl");
            break;
        default:
            break;
    }

    RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "profileId2Type id(%d)=>%s", profileId,type.c_str());
    return type;
}

int32_t RpDataUtils::type2profileId(std::string type) {
    int32_t profileId = -1;
    if(type == "iot_net_0") {
        // profileId = IOT_APN_2;
    } else if (type == "iot_default") {
        // profileId = IOT_APN_1;
    } else if(type == "iot_net_1") {
        // profileId = IOT_APN_3;
    } else if(type == "iot_net_2") {
        profileId = IOT_APN_4;
    } else if(type == "iot_net_3") {
        profileId = IOT_APN_5;
    } else if(type == "iot_net_4") {
        profileId = IOT_APN_6;
    } else if(type == "iot_net_5") {
        profileId = IOT_APN_7;
    } else if(type == "iot_net_5") {
        profileId = IOT_APN_8;
    } else if (type == "default") {
        //profileId = MOBILE_APN; //for test, networkmanger don't support
        profileId = MOBILE_APN; // change default to match rild of android;
    } else if (type == "mms") {
        //profileId = MMS_APN;
        // profileId = IOT_APN_2; //for test, networkmanger don't support
    } else if (type == "wap") {
        //profileId = MMS_APN;
        // profileId = IOT_APN_3; //for test, networkmanger don't support
    } else if (type == "supl") {
        //profileId = MMS_APN;
        profileId = IOT_APN_4; //for test, networkmanger don't support
    } else if (type == "rcs") {
        profileId = RCS_APN; //for test, networkmanger don't support
    } else if (type == "bip") {
        profileId = BIP_APN; //for test, networkmanger don't support
    }
    RFX_LOG_D(RP_DC_UTIL_LOG_TAG , "type2profileId type(%s)=>%d", type.c_str(), profileId);
    return profileId;
}
#endif //UPDATE_APN
int RpDataUtils::generateNetId() {
    if(mNetId +1 >= 7000 ) {
        mNetId =  6000;
    } else {
        mNetId++;
    }
    return mNetId;
}

::aidl::android::hardware::radio::data::PdpProtocolType RpDataUtils::convertStringToPdpProtocolType(const char *ptr) {
    if (ptr != NULL) {
        if (0 == strcasecmp(ptr, "IP")) {
            return ::aidl::android::hardware::radio::data::PdpProtocolType::IP;
        } else if (0 == strcasecmp(ptr, "IPV4V6")) {
            return ::aidl::android::hardware::radio::data::PdpProtocolType::IPV4V6;
        } else if (0 == strcasecmp(ptr, "IPV6")) {
            return ::aidl::android::hardware::radio::data::PdpProtocolType::IPV6;
        } else if (0 == strcasecmp(ptr, "PPP")) {
            return ::aidl::android::hardware::radio::data::PdpProtocolType::PPP;
        }
    }
    return ::aidl::android::hardware::radio::data::PdpProtocolType::IP;
}

void RpDataUtils::saveCidByType(int cid, std::string type) {
  int profileId = type2profileId(type);
  RFX_LOG_D(RP_DC_UTIL_LOG_TAG, "saveCidByType cid(%d), type(%s), profileId(%d)", cid, type.c_str(), profileId);
  switch (profileId) {
  case MOBILE_APN:
    property_set("vendor.ril.default.profile.cid", std::to_string(cid).c_str());
    break;
  case RCS_APN:
    property_set("vendor.ril.private1.profile.cid", std::to_string(cid).c_str());
    break;
  case BIP_APN:
    property_set("vendor.ril.private2.profile.cid", std::to_string(cid).c_str());
    break;
  default:
    RFX_LOG_E(RP_DC_UTIL_LOG_TAG,
              "saveCidProfileId cid(%d), profileId(%d) is invalid", cid,
              profileId);
  }
}

typedef struct  {
    char *mccmnc;
    unsigned int mtu;
}MccMnc_2_Mtu_T;

const MccMnc_2_Mtu_T  mccmnc_2_mtu[] = {
            {"46000", 1400},
            {"46001", 1400},
            {"46002", 1400},
            {"46003", 1400},
            {"46004", 1400},
            {"46005", 1400},
            {"46006", 1400},
            {"46007", 1400},
            {"46008", 1400},
            {"46009", 1400},
            {"46010", 1400},
            {"46011", 1400},
            {"46012", 1400},
};

int32_t RpDataUtils::getMtuByMccmnc(char *mccmnc)
{
    int len = sizeof(mccmnc_2_mtu)/sizeof(MccMnc_2_Mtu_T);
    for(int i = 0; i < len ; i++){
        if(strcasecmp(mccmnc,mccmnc_2_mtu[i].mccmnc) == 0)
            return mccmnc_2_mtu[i].mtu;
     }
    return 0;
}
