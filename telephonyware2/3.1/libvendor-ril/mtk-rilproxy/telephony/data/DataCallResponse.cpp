// SPDX-License-Identifier: MediaTekProprietary
/*
 * DataCallResponse.cpp
 *
 *  Created on: May 8, 2017
 *      Author: mtk54458
 */
#include "DataCallResponse.h"
#include "RpDataUtils.h"
#include "RfxLog.h"

#include <vector>
using namespace std;

#define RFX_LOG_TAG "RpDfwDataCallResponse"

DataCallResponse::DataCallResponse() {
    DataCallResponse::status = -1;
    DataCallResponse::cid = -1;
    DataCallResponse::active = 0;
    DataCallResponse::type = "";
    DataCallResponse::ifname = "";
    DataCallResponse::addresses = "";
    DataCallResponse::dnses = "";
    DataCallResponse::gateways = "";
    DataCallResponse::suggestedRetryTime = -1;
    DataCallResponse::pcscf = "";
    DataCallResponse::mtu = -1;
    DataCallResponse::mtuV6 = -1;
    // DataCallResponse::eratType = -1;

    DataCallResponse::netId = -1;
    DataCallResponse::ipv4 = "";
    DataCallResponse::ipv6 = "";
}

DataCallResponse::DataCallResponse(int status, int cid, int active, string type, string ifname,
        string addresses, string dnses, string gateways, int suggestedRetryTime, string pcscf, int mtu,
        int eratType, int netId) {
    DataCallResponse::status = status;
    DataCallResponse::cid = cid;
    DataCallResponse::active = active;
    DataCallResponse::type = type;
    DataCallResponse::ifname = ifname;
    DataCallResponse::addresses = addresses;
    DataCallResponse::dnses = dnses;
    DataCallResponse::gateways = gateways;
    DataCallResponse::suggestedRetryTime = suggestedRetryTime;
    DataCallResponse::pcscf = pcscf;
    DataCallResponse::mtu = mtu;
    DataCallResponse::mtuV6 = -1;
    // DataCallResponse::eratType = eratType;

    DataCallResponse::netId = netId;
    initAddress();
}

void DataCallResponse::initAddress() {
    if (!DataCallResponse::addresses.empty()) {
        vector<string> fragments;
        RpDataUtils::split(DataCallResponse::addresses, ' ', fragments);

        vector<string>::iterator itor = fragments.begin();
        while (itor != fragments.end()) {
            //tricky method to parse ipv4 and ipv6
            if (itor->length() < 20) {
                DataCallResponse::ipv4 = *itor;
            } else {
                DataCallResponse::ipv6 = *itor;
            }
            itor++;
        }
    }
}

string DataCallResponse::logInfo() {
    string dataCallResponse;
    dataCallResponse += ("status: " + RpDataUtils::i2s(status));
    dataCallResponse += (", cid: " + RpDataUtils::i2s(cid));
    dataCallResponse += (", active: " + std::string(RpDataUtils::pdnState2string(active)));
    dataCallResponse += (", type: " + type);
    dataCallResponse += (", ifname: " + ifname);
    dataCallResponse += (", addresses: " + addresses);
    dataCallResponse += (", dnses: " + dnses);
    dataCallResponse += (", gateways: " + gateways);
    dataCallResponse += (", suggestedRetryTime: " + RpDataUtils::i2s(suggestedRetryTime));
    dataCallResponse += (", pcscf: " + pcscf);
    dataCallResponse += (", mtu: " + RpDataUtils::i2s(mtu));
    dataCallResponse += (", mtuV6: " + RpDataUtils::i2s(mtuV6));
    // dataCallResponse += (", eratType: " + RpDataUtils::i2s(eratType));
    dataCallResponse += (", netId: " + RpDataUtils::i2s(netId));

    return dataCallResponse;
}
