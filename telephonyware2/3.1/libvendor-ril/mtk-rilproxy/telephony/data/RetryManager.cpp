// SPDX-License-Identifier: MediaTekProprietary
/*
 * RetryManager.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: mtk54458
 */

#include "RetryManager.h"
#include "ApnSetting.h"
#include "RpDataUtils.h"
#include "RfxLog.h"
#include <stdlib.h>

#include <utility>
using namespace std;

#define RFX_LOG_TAG "RpDfwRetryManager"

RetryManager::RetryManager(string apnType) {
    RetryManager::mApnType = apnType;
    RetryManager::mCurrentIndex = -1;
    RetryManager::mWaitingApns.clear();
    RetryManager::mRetryArray.clear();
    RetryManager::mMaxRetryCount = 0;
    RetryManager::mRetryCount = 0;
    RetryManager::mModemSuggestedDelay = NO_SUGGESTED_RETRY_DELAY;
    RetryManager::mRetryForever = false;
    RetryManager::mDefaultRetryConfigString = DEFAULT_DATA_RETRY_CONFIG;
    RetryManager::mOtherRetryConfigString = OTHERS_DATA_RETRY_CONFIG;
    RetryManager::mSameApnRetryCount = 0;
}

RetryManager::RetryManager() {
    RetryManager::mApnType = TEXT_APN_TYPE_DEFAULT;
    RetryManager::mWaitingApns.clear();
    RetryManager::mCurrentIndex = -1;
    RetryManager::mRetryArray.clear();
    RetryManager::mMaxRetryCount = 0;
    RetryManager::mRetryCount = 0;
    RetryManager::mModemSuggestedDelay = NO_SUGGESTED_RETRY_DELAY;
    RetryManager::mRetryForever = false;
    RetryManager::mDefaultRetryConfigString = DEFAULT_DATA_RETRY_CONFIG;
    RetryManager::mOtherRetryConfigString = OTHERS_DATA_RETRY_CONFIG;
    RetryManager::mSameApnRetryCount = 0;
}

RetryManager::~RetryManager() {}

ApnSetting RetryManager::getNextApnSetting() {
    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s)::getNextApnSetting RetryManager::mModemSuggestedDelay=%d,"
            "RetryManager::mSameApnRetryCount=%d,mCurrentIndex=%d.", mApnType.c_str(),
            RetryManager::mModemSuggestedDelay,RetryManager::mSameApnRetryCount,RetryManager::mCurrentIndex);
    if (RetryManager::mWaitingApns.size() == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "RetryManager(%s):: Waiting APN list is null or empty.", mApnType.c_str());
        return ApnSetting();
    }

    if (mCurrentIndex != -1 && RetryManager::mModemSuggestedDelay != NO_SUGGESTED_RETRY_DELAY &&
            RetryManager::mSameApnRetryCount < MAX_SAME_APN_RETRY) {
        mSameApnRetryCount++;
        return RetryManager::mWaitingApns[mCurrentIndex];
    }
    RetryManager::mSameApnRetryCount = 0;

    vector<ApnSetting>::size_type index = RetryManager::mCurrentIndex;
    vector<ApnSetting>::size_type length = RetryManager::mWaitingApns.size();
    while (true) {
        if (++index == length)
            index = 0;
        if (RetryManager::mWaitingApns[index].permanentFailed == false)
            break;
        if (index == RetryManager::mCurrentIndex) {
            RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: index(%d)==RetryManager::mCurrentIndex.",
                mApnType.c_str(),index);
            return ApnSetting();
        }
    }
    RetryManager::mCurrentIndex = index;
    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: mCurrentIndex=%d, %s",
        mApnType.c_str(),RetryManager::mCurrentIndex,(RetryManager::mWaitingApns[mCurrentIndex].logInfo()).c_str());

    return RetryManager::mWaitingApns[mCurrentIndex];
}

void RetryManager::setWaitingApns(vector <ApnSetting> waittingApns) {
    if (waittingApns.empty()) {
        RFX_LOG_W(RFX_LOG_TAG, "RetryManager(%s):: No waiting APNs provided", mApnType.c_str());
        return;
    }
    RetryManager::mWaitingApns = waittingApns;
    RetryManager::configure(RetryManager::mApnType == TEXT_APN_TYPE_DEFAULT);
    vector<ApnSetting>::iterator itor = RetryManager::mWaitingApns.begin();
    while (itor != RetryManager::mWaitingApns.end()) {
        itor -> permanentFailed = false;
        itor++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: setWaitingApns with %lu waiting APNs.",
            mApnType.c_str(), mWaitingApns.size());
}

vector<ApnSetting> RetryManager::getWaitingApns() {
    return RetryManager::mWaitingApns;
}

void RetryManager::reset() {
    RetryManager::mMaxRetryCount = 0;
    RetryManager::mSameApnRetryCount = 0;
    RetryManager::mRetryCount = 0;
    RetryManager::mCurrentIndex = -1;
    RetryManager::mModemSuggestedDelay = NO_SUGGESTED_RETRY_DELAY;
    RetryManager::mRetryArray.clear();

    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: reset done", mApnType.c_str());
}

long RetryManager::getDelayForNextApn() {
    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s)::getDelayForNextApn "
            "RetryManager::mWaitingApns.size()=%d, "
            "RetryManager::mModemSuggestedDelay=%d,"
            "RetryManager::mSameApnRetryCount=%d.", mApnType.c_str(),
            RetryManager::mWaitingApns.size(),
            RetryManager::mModemSuggestedDelay,
            RetryManager::mSameApnRetryCount);
    if (RetryManager::mWaitingApns.size() == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "RetryManager(%s)::Waiting APN list is null or empty.", mApnType.c_str());
        return NO_RETRY;
    }
    if (RetryManager::mModemSuggestedDelay == NO_RETRY) {
        RFX_LOG_W(RFX_LOG_TAG, "RetryManager::Modem suggested not retrying.");
        return NO_RETRY;
    }
    if (RetryManager::mModemSuggestedDelay != NO_SUGGESTED_RETRY_DELAY &&
            RetryManager::mSameApnRetryCount < MAX_SAME_APN_RETRY) {
        RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s) Modem suggested retry in %ld ms.",
                mApnType.c_str(), mModemSuggestedDelay);
        return RetryManager::mModemSuggestedDelay;
    }
    vector<ApnSetting>::size_type index = RetryManager::mCurrentIndex;
    while(true) {
        if (++index >= RetryManager::mWaitingApns.size()) {
            index = 0;
        }
        if (RetryManager::mWaitingApns[index].permanentFailed == false) {
            break;
        }
        if (index == RetryManager::mCurrentIndex) {
            RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s) All APNs have permanently failed.", mApnType.c_str());
            return NO_RETRY;
        }
    }
    long delay;
    if (index <= RetryManager::mCurrentIndex) {
        if (!RetryManager::mRetryForever && RetryManager::mRetryCount + 1 > RetryManager::mMaxRetryCount) {
            RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s) Reached maximum retry count: %d ", mApnType.c_str(), mMaxRetryCount);
            return NO_RETRY;
        }
        delay = RetryManager::getRetryTimer();
        RetryManager::mRetryCount ++;
    } else {
        delay = DEFAULT_INTER_APN_DELAY;
    }
    return delay;
}

void RetryManager::configure(bool isDefault) {
    string configString;
    if (isDefault) {
        configString = RetryManager::mDefaultRetryConfigString;
    } else {
        configString = RetryManager::mOtherRetryConfigString;
    }
    RetryManager::configure(configString);
}

bool RetryManager::configure(string config) {
    RpDataUtils::trim(config);
    if (config.empty()) {
        RFX_LOG_W(RFX_LOG_TAG, "RetryManager(%s):: configure: cleared", mApnType.c_str());
        return true;
    }

    RetryManager::reset();
    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: configure: '%s'", mApnType.c_str(), config.c_str());

    int defaultRandomization = 0;
    vector<string> strArray;
    RpDataUtils::split(config, ',', strArray);
    for (vector<string>::size_type i = 0; i < strArray.size(); i++) {
       // log.log(string("RetryManager(" + RetryManager::mApnType + "):: configure: strArray[") + log.itos(i) + "]='" + strArray[i] + "'");

        vector<string> splitStr;
        RpDataUtils::split(strArray[i], '=', splitStr, 2);
        //log.log("RetryManager(" + RetryManager::mApnType + "):: configure: splitStr[0]='" + splitStr[0] + "'");

        if (splitStr.size() > 1) {
            if (splitStr[0] == "default_randomization") {
                if (atoi(splitStr[1].c_str()) != 0) {
                    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: configure default_randomization: splitStr[1]='%s'",
                            mApnType.c_str(), splitStr[1].c_str());

                    defaultRandomization = atoi(splitStr[1].c_str());
                } else {
                    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: configure default_randomization: false", mApnType.c_str());
                    return false;
                }
            } else if (splitStr[0] == "max_retries") {
                if (splitStr[1] == "infinite") {
                    RetryManager::mRetryForever = true;
                } else {
                    if (atoi(splitStr[1].c_str()) != 0) {
                        RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: configure mMaxRetryCount: splitStr[1]='%s'",
                                mApnType.c_str(), splitStr[1].c_str());

                        RetryManager::mMaxRetryCount = atoi(splitStr[1].c_str());
                    } else {
                        RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: configure mMaxRetryCount: false", mApnType.c_str());
                        return false;
                    }
                }
            } else {
                RFX_LOG_W(RFX_LOG_TAG, "RetryManager(%s):: configure: false", mApnType.c_str());
                return false;
            }
        } else {
            vector<string> splitDelayStr;
            RpDataUtils::split(strArray[i], ':', splitDelayStr, 2);
            RetryRec rr(0, 0);
            if (atoi(splitDelayStr[0].c_str()) != 0) {
                rr.mDelayTime = atoi(splitDelayStr[0].c_str());
            } else {
                RFX_LOG_W(RFX_LOG_TAG, "RetryManager(%s):: configure mDelayTime: false", mApnType.c_str());

                return false;
            }
            if (splitDelayStr.size() > 1) {
                if (atoi(splitDelayStr[1].c_str()) != 0) {
                //    log.log("RetryManager(" + RetryManager::mApnType + "):: configure: splitDelayStr[1]='" + splitDelayStr[1] + "'");

                    rr.mRandomizationTime = atoi(splitDelayStr[1].c_str());
                } else {
              //      log.log("RetryManager(" + RetryManager::mApnType + "):: configure mRandomizationTime: false");

                    return false;
                }
            } else {
                rr.mRandomizationTime = defaultRandomization;
              //  log.log(string("RetryManager(" + RetryManager::mApnType + "):: configure: defaultRandomization='") + log.itos(defaultRandomization) + "'");
            }
            RetryManager::mRetryArray.push_back(rr);
        }
    }
    if (RetryManager::mRetryArray.size() > RetryManager::mMaxRetryCount) {
        RetryManager::mMaxRetryCount = RetryManager::mRetryArray.size();
        RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s)::configure: setting mMaxRetryCount= %d",
                mApnType.c_str(), mMaxRetryCount);
    }

    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: configure: true", mApnType.c_str());
    return true;
}

long RetryManager::getRetryTimer() {
    vector<RetryRec>::size_type index;
    if (RetryManager::mRetryCount < RetryManager::mRetryArray.size()) {
        index = RetryManager::mRetryCount;
    } else {
        index = RetryManager::mRetryArray.size() - 1;
    }

    int retVal;
    if (index < RetryManager::mRetryArray.size()) {
        retVal = RetryManager::mRetryArray.at(index).mDelayTime + nextRandomizationTime(index);
    } else {
        retVal = 0;
    }
    RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: getRetryTimer: %d", mApnType.c_str(),retVal);

    return retVal;
}

int RetryManager::nextRandomizationTime(int index) {
    int randomTime = RetryManager::mRetryArray.at(index).mRandomizationTime;
    if (randomTime == 0) {
        return 0;
    } else {
        return RpDataUtils::random(randomTime);
    }
}

void RetryManager::setModemSuggestedDelay(long delay) {
    RetryManager::mModemSuggestedDelay = delay;
}

long RetryManager::getModemSuggestedDelay() {
    return RetryManager::mModemSuggestedDelay;
}

void RetryManager::markApnPermanentFailed(ApnSetting apnSetting) {
    vector<ApnSetting>::iterator itor = RetryManager::mWaitingApns.begin();
    while (itor != RetryManager::mWaitingApns.end()) {
        if (itor -> apn == apnSetting.apn) {
            itor -> permanentFailed = true;

            RFX_LOG_D(RFX_LOG_TAG, "RetryManager(%s):: RetryManager: mark %s as permanent failed.",
                    mApnType.c_str(), (itor -> apn).c_str());
        }
        itor++;
    }
}

void RetryManager::setDefaultRetryConfigString(string defaultConfig) {
    RetryManager::mDefaultRetryConfigString = defaultConfig;
}

string RetryManager::getDefaultRetryConfigString() {
    return RetryManager::mDefaultRetryConfigString;
}

void RetryManager::setOtherRetryConfigString(string otherConfig) {
    RetryManager::mOtherRetryConfigString = otherConfig;
}

string RetryManager::getOtherRetryConfigString() {
    return RetryManager::mOtherRetryConfigString;
}

string RetryManager::logInfo() {
    return RetryManager::mApnType;
}
