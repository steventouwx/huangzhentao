// SPDX-License-Identifier: MediaTekProprietary
/*
 * DcTracker.h
 *
 *  Created on: Apr 21, 2017
 *      Author: mtk54458
 */

#ifndef DCTRACKER_H_
#define DCTRACKER_H_

#include "ApnContext.h"
#include "NetworkConfig.h"
#include "DataConnection.h"
#include "DcController.h"
#include <binder/Parcel.h>
#include "apn/RpApnController.h"
#include "PdnManager.h"

#include <utils/RefBase.h>
#include <utils/Vector.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

#include "Common.h"
using namespace std;

using ::android::sp;
using ::android::Parcel;

class RpDataController;
class DcTracker {
public:
    class DataAllowFailReason;

    DcTracker();
    ~DcTracker();
    void requestNetwork(const sp<RfxMessage>& request);
    void deactiveNetwork(const sp<RfxMessage>& request);
    void initApnContext();
    void addApnContext(string type, NetworkConfig networkConfig);
    void trySetupData(string apnType, const sp<RfxMessage>& request, string reason);
    void cleanUpConnection(string apnType, const sp<RfxMessage>& request);
    bool isDataAllowed(DataAllowFailReason &dataAllowFailReason);
    void onDataConnectionDetached();
    void onDataConnectionAttached();
    void onPsRestrict(bool psRestrict);
    void onCallStateStart();
    void onCallStateEnd();
    void onApnReady(Vector<ApnsettingT*>* list);
    void setupDataOnConnectableApns(string reason);
    vector<ApnSetting> buildWaitingApns(string requestApnType);
    void createAllApnList(Vector<ApnsettingT*>* list);
    bool setupData(string apnType, const sp<RfxMessage>& request);
    ApnContext *findCompatibleConnectedApnContext(string apnType);
    DataConnection *findFreeDataConnection(string apnType);
    DataConnection *createDataConnection(string apnType, int defaultId);
    int generationConnectionId(string apnType);
    void onSetupConnectionCompleted(const sp<RfxMessage>& response);
    void onDeactiveConnectionCompleted(const sp<RfxMessage>& response);
    long getModemSuggestedRetryDelay(DataCallResponse dataCallResponse);
    void reTrySetupData(string apnType, string reason);
    bool isPermanentFail(DcFail dcFailCause);
    bool hasCompatibleConnectedApnContext(string apnType);
    void onDataCallListChanged(const sp<RfxMessage>& message);

    void setRpDataController(RpDataController *rpDataController);
    bool isValidRequest(string apnType);
    void dumpEnableApnList(list<ApnContext*> prioritySortedApnContexts);

    void onGetDataCallFailCauseCompleted(const sp<RfxMessage>& response);

    bool isLocalSetupPdnRequest(const sp<RfxMessage>& request);
    bool isLocalCallFailPdnrequest(const sp<RfxMessage>& request);

    void setApnStateByApnType(string apnType, RIL_Data_Call_PdnState state);

    int getSlotId() const {
        return m_slot_id;
    }

    class DataAllowFailReason {
    private:
        vector<string> mDataAllowFailReasonSet;
    public:
        DataAllowFailReason() {
            mDataAllowFailReasonSet.clear();
        }
        ~ DataAllowFailReason() {}

        void addDataAllowFailReason(string type) {
            mDataAllowFailReasonSet.push_back(type);
        }

        string getDataAllowFailReason() {
            string failReason("isDataAllowed: No");
            vector<string>::iterator itor = mDataAllowFailReasonSet.begin();
            while (itor != mDataAllowFailReasonSet.end()) {
                failReason = failReason.append(*itor);
                itor++;
            }
            return failReason;
        }

        void clearAllReasons() {
            mDataAllowFailReasonSet.clear();
        }

        bool isFailed() {
            return mDataAllowFailReasonSet.size() > 0;
        }
    };

private:
    map<string, ApnContext*> mApnContexts;
    vector<ApnSetting> mAllApnSettings;
    list<ApnContext*> mPrioritySortedApnContexts;
    bool mAttached;
    DcController mDcController;
    RpDataController *mRpDataController;
    PdnManager mPdnManager;

    int m_slot_id;
    bool mIsApnReady;
};

#endif /* DCTRACKER_H_ */
