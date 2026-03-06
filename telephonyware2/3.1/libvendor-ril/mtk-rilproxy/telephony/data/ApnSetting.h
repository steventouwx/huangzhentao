// SPDX-License-Identifier: MediaTekProprietary
/*
 * apn_setting.h
 *
 *  Created on: Apr 18, 2017
 *      Author: mtk54458
 */

#ifndef APNSETTING_H_
#define APNSETTING_H_

#include <string>
#include <vector>
using namespace std;

class ApnSetting {
public:
    string carrier;
    string apn;
    string proxy;
    string port;
    string mmsc;
    string mms_proxy;
    string mms_port;
    string user;
    string password;
    int auth_type;
    vector<string> types;
    int id;
    string numberic;
    string protocol;
    string roaming_protocol;
    int mtu;
    bool carrier_enabled;
    int bear;
    int bear_bitmask;
    int profile_id;
    bool modem_cognitive;
    int max_conns;
    int wait_time;
    int max_conns_time;
    string mvno_type;
    string mvno_match_data;
    bool permanentFailed;

    ApnSetting();
    ApnSetting(int id, string numberic, string carrier, string apn,
            string proxy, string port, string mmsc, string mms_proxy,
            string mms_port, string user, string password, int auth_type,
            string types, string protocol, string roaming_protocol,
            bool carrier_enabled, int bear, int bear_bitmask, int profile_id,
            bool modem_cognitive, int max_conns, int wait_time, int max_conns_time,
            int mtu, string mvno_type, string mvno_match_data);
    string logInfo();
    ~ApnSetting();

    void clear();
    bool canHandleType(string type);
    bool isValid();
};

#endif /* APNSETTING_H_ */
