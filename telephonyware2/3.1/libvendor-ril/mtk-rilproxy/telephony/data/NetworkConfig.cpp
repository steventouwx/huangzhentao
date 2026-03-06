// SPDX-License-Identifier: MediaTekProprietary
/*
 * NetworkConfig.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: mtk54458
 */

#include "NetworkConfig.h"
#include "RpDataUtils.h"
#include <string>
using namespace std;

NetworkConfig::NetworkConfig(string init) {
    vector<string> fragments;
    RpDataUtils::split(init, ',', fragments);
    NetworkConfig::mName = fragments.at(0);
    NetworkConfig::mType = atoi(fragments.at(1).c_str());
    NetworkConfig::mRadio = atoi(fragments.at(2).c_str());
    NetworkConfig::mPriority = atoi(fragments.at(3).c_str());
    NetworkConfig::mRestoreTime = atoi(fragments.at(4).c_str());
    if (fragments.at(5).compare("true") == 0) {
        NetworkConfig::mDependencyMet = true;
    } else {
        NetworkConfig::mDependencyMet = false;
    }
}

bool NetworkConfig::isDefault() {
    return (NetworkConfig::mRadio == NetworkConfig::mType);
}
