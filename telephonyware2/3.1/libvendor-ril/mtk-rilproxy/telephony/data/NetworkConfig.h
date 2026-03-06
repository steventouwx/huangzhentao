// SPDX-License-Identifier: MediaTekProprietary
/*
 * NetworkConfig.h
 *
 *  Created on: Apr 24, 2017
 *      Author: mtk54458
 */

#ifndef NETWORKCONFIG_H_
#define NETWORKCONFIG_H_

#include <string>
using namespace std;

class NetworkConfig {
    public:
        string mName;
        int mType;
        int mRadio;
        int mPriority;
        int mRestoreTime;
        bool mDependencyMet;
        bool isDefault();
        NetworkConfig(string init);
    };

#endif /* NETWORKCONFIG_H_ */
