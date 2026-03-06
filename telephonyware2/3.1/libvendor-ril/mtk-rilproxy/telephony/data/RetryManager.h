// SPDX-License-Identifier: MediaTekProprietary
/*
 * RetryManager.h
 *
 *  Created on: Apr 19, 2017
 *      Author: mtk54458
 */

#ifndef RETRYMANAGER_H_
#define RETRYMANAGER_H_

#include "ApnSetting.h"
#include <vector>
#include <string>

#include "Common.h"
using namespace std;

class RetryManager {
public:
    RetryManager();
    RetryManager(string apnType);
    ~RetryManager();
    ApnSetting getNextApnSetting();
    long getDelayForNextApn();
    void setWaitingApns(vector<ApnSetting> waittingApns);
    vector<ApnSetting> getWaitingApns();
    void configure(bool isDefault);
    bool configure(string config);
    void reset();
    long getRetryTimer();
    int nextRandomizationTime(int index);
    void setModemSuggestedDelay(long delay);
    long getModemSuggestedDelay();
    void markApnPermanentFailed(ApnSetting apnSetting);
    string getDefaultRetryConfigString();
    void setDefaultRetryConfigString(string defaultConfig);
    string getOtherRetryConfigString();
    void setOtherRetryConfigString(string otherConfig);
    string logInfo();

private:
    vector<ApnSetting> mWaitingApns;
    string mApnType;
    vector<ApnSetting>::size_type mCurrentIndex;
    int mMaxRetryCount;
    int mRetryCount;
    int mSameApnRetryCount;
    long mModemSuggestedDelay;
    bool mRetryForever;
    string mDefaultRetryConfigString;
    string mOtherRetryConfigString;

    class RetryRec {
    public:
        RetryRec(int delayTime, int randomizationTime) {
            mDelayTime = delayTime;
            mRandomizationTime = randomizationTime;
        }

        int mDelayTime;
        int mRandomizationTime;
    };

    vector<RetryRec> mRetryArray;

};

#endif /* RETRYMANAGER_H_ */
