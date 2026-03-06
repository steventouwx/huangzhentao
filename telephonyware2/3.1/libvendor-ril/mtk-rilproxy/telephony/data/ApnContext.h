// SPDX-License-Identifier: MediaTekProprietary
/*
 * ApnContext.h
 *
 *  Created on: Apr 18, 2017
 *      Author: mtk54458
 */

#ifndef APNCONTEXT_H_
#define APNCONTEXT_H_
#include "ApnSetting.h"
#include "RetryManager.h"
#include "NetworkConfig.h"
#include "DataConnection.h"

#include <utils/RefBase.h>
#include "Common.h"

using ::android::sp;

class RpDataController;
class ApnContext {
public:
    ApnContext(string apnType, NetworkConfig networkConfig);
    ApnContext();
    ~ApnContext();

    string getApnType();
    ApnSetting &getApnSetting();
    void setApnSetting(ApnSetting apnSetting);
    void setWaitingApns(vector<ApnSetting> waitingApns);
    vector<ApnSetting> getWaitingApns();
    ApnSetting getNextApnSetting();
    long getDelayForNextApn();
    void setEnabled(bool enabled);
    bool isEnabled();
    void incRefCount();
    void decRefCount();
    int getRefCount();
    void setSate(RIL_Data_Call_PdnState state);
    RIL_Data_Call_PdnState getState();
    DataConnection *getDataConnection();
    void setDataConnection(DataConnection *dataConnection);
    void setModemSuggestedDelay(long delay);
    long getModemSuggestedDelay();
    void markApnPermanentFailed(ApnSetting apnSetting);
    void setDependencyMet(bool met);
    bool getDependencyMet();

    void setClientSetupDataRequest(const sp<RfxMessage>& setupDataRequest);
    void setClientTearDownDataRequest(const sp<RfxMessage>& tearDownDataRequest);

    void notifyStateChange(RIL_Data_Call_PdnState newState);
    void setRpDataController(RpDataController *rpDataController);

    bool operator < (ApnContext &apnContext) {
        return this->mPriority > apnContext.mPriority;
    }
    bool operator != (ApnContext &apnContext) {
        return this -> mApnType != apnContext.mApnType;
    }
    bool operator == (ApnContext &apnContext) {
        return this -> mApnType == apnContext.mApnType;
    }
    string logInfo();

private:
    ApnSetting mApnSetting;
    string mApnType;
    bool mDataEnalbed;
    int mRefCount;
    int mPriority;
    bool mDependencyMet;
    RetryManager mRetryManger;
    RIL_Data_Call_PdnState mState;
    DataConnection *mDataConnection;

    RpDataController *mRpDataController;
    vector<sp<RfxMessage>> mClientSetupDataRequest;
    vector<sp<RfxMessage>> mClientTearDownDataRequest;

};



#endif /* APNCONTEXT_H_ */
