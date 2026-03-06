// SPDX-License-Identifier: MediaTekProprietary
/*
 * apn_setting.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: mtk54458
 */

#include "ApnSetting.h"
#include "RpDataUtils.h"

ApnSetting::ApnSetting(int id, string numberic, string carrier, string apn,
        string proxy, string port, string mmsc, string mms_proxy,
        string mms_port, string user, string password, int auth_type,
        string types, string protocol, string roaming_protocol,
        bool carrier_enabled, int bear, int bear_bitmask, int profile_id,
        bool modem_cognitive, int max_conns, int wait_time, int max_conns_time,
        int mtu, string mvno_type, string mvno_match_data) {
    ApnSetting::id = id;
    ApnSetting::numberic = numberic;
    ApnSetting::carrier = carrier;
    ApnSetting::apn = apn;
    ApnSetting::protocol = proxy;
    ApnSetting::port = port;
    ApnSetting::mmsc = mmsc;
    ApnSetting::mms_proxy = mms_proxy;
    ApnSetting::mms_port = mms_port;
    ApnSetting::user = user;
    ApnSetting::password = password;
    ApnSetting::auth_type = auth_type;
    RpDataUtils::split(types, ',', ApnSetting::types);
    ApnSetting::protocol = protocol;
    ApnSetting::roaming_protocol = roaming_protocol;
    ApnSetting::carrier_enabled = carrier_enabled;
    ApnSetting::bear = bear;
    ApnSetting::bear_bitmask = bear_bitmask;
    ApnSetting::profile_id = profile_id;
    ApnSetting::modem_cognitive = modem_cognitive;
    ApnSetting::max_conns = max_conns;
    ApnSetting::wait_time = wait_time;
    ApnSetting::max_conns_time = max_conns_time;
    ApnSetting::mtu = mtu;
    ApnSetting::mvno_type = mvno_type;
    ApnSetting::mvno_match_data = mvno_match_data;
    ApnSetting::permanentFailed = false;
}

ApnSetting::ApnSetting() {
    ApnSetting::types.clear();
    ApnSetting::carrier_enabled = false;
    ApnSetting::auth_type = 0;
    ApnSetting::id = 0;
    ApnSetting::mtu = 0;
    ApnSetting::bear = 0;
    ApnSetting::bear_bitmask = 0;
    ApnSetting::profile_id = 0;
    ApnSetting::modem_cognitive = 0;
    ApnSetting::max_conns = 0;
    ApnSetting::wait_time = 0;
    ApnSetting::max_conns_time= 0;
    ApnSetting::permanentFailed = false;
}

ApnSetting::~ApnSetting() {}

void ApnSetting::clear() {
    ApnSetting::types.clear();
    ApnSetting::carrier_enabled = false;
}

bool ApnSetting::isValid() {
    return (!ApnSetting::types.empty() ||
            ApnSetting::carrier_enabled);
}

bool ApnSetting::canHandleType(string type) {
    bool result = false;

    vector<string>::iterator itor = ApnSetting::types.begin();
    while (itor != ApnSetting::types.end()) {
        if (*itor == type) {
            result = true;
            break;
        }
        itor++;
    }

    if (!ApnSetting::carrier_enabled) {
        result = false;
    }

    return result;
}

string ApnSetting::logInfo() {
    // list the key info
    if (ApnSetting::types.empty()) {
        return string("--- no apn Setting!!!");
    }

    string types;
    vector<string>::iterator itor =  ApnSetting::types.begin();
    while (itor != ApnSetting::types.end()) {
        types += *itor;
        types += ",";
        itor++;
    }
    types.erase(types.end() - 1);
    string info = "apnName: "+ ApnSetting::apn + ", Type: " + types;
    return info;
}
