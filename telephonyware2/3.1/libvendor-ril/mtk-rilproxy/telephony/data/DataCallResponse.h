// SPDX-License-Identifier: MediaTekProprietary
/*
 * DataCallResponse.h
 *
 *  Created on: Apr 27, 2017
 *      Author: mtk54458
 */

#ifndef DATACALLRESPONSE_H_
#define DATACALLRESPONSE_H_

#include <string>

#include "Common.h"
using namespace std;

class DataCallResponse {
public:
    int status;
    int cid;
    int active;
    int profileId;
    string type;
    string ifname;
    string addresses;
    string dnses;
    string gateways;
    int suggestedRetryTime;
    string pcscf;
    int mtu;
    int mtuV6;
    // int eratType;

    int netId;
    string ipv4;
    string ipv6;

    DataCallResponse();
    DataCallResponse(int status, int cid, int active, string type, string ifname, string addresses,
            string dnses, string gateways, int suggestedRetryTime, string pcscf, int mtu, int mtuV6, int netId);
    void initAddress();
    string logInfo();
};



#endif /* DATACALLRESPONSE_H_ */
