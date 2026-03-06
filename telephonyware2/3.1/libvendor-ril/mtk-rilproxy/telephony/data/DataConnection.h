// SPDX-License-Identifier: MediaTekProprietary
/*
 * DataConnection.h
 *
 *  Created on: Apr 26, 2017
 *      Author: mtk54458
 */

#ifndef DATACONNECTION_H_
#define DATACONNECTION_H_
#include "ApnSetting.h"
#include "DataCallResponse.h"
#include "DcFail.h"
#include "RfxMessage.h"

#include <utils/RefBase.h>

#include <set>
using namespace std;

using ::android::sp;

const int SETUP_DATA_AUTH_NONE = 0;
const int SETUP_DATA_AUTH_PAP = 1;
const int SETUP_DATA_AUTH_CHAP = 2;
const int SETUP_DATA_AUTH_PAP_CHAP = 3;
class ApnContext;
class RpDataController;
class DataConnection {
public:
    DataConnection();
    DataConnection(int id, RpDataController *rpDataController);
    ~DataConnection();
    void setApnSetting(ApnSetting apnSetting);
    ApnSetting getApnSetting();
    void clearApnSetting();
    void connect(string apnType, const sp<RfxMessage>& request);
    void setupDataCall(string apnType, const sp<RfxMessage>& request, int radioTech, int profileId, string apn, string user,
            string password, int authType, string protocol, int id);
    void deactivateDataCall(int cId, const sp<RfxMessage>& request);
    DataCallResponse getDataCallResponse();
    void setDataCallResponse(DataCallResponse dataCallResponse);
    void setDcFail(DcFail mDcFail);
    void setcId(int cId);
    int getcId();
    void updateCompatibleApnContexts(ApnContext *apnContext);
    void addCompatibleApnContext(ApnContext *apnContext);
    void removeCompatibleApnContext(ApnContext *apnContext);
    set<ApnContext*> &getCompatibleApnContext();
    string logInfo();
    int getApntypeProfile(const char *type);
    bool operator!= (DataConnection &dataConnection) {
        return this -> mCId != dataConnection.mCId;
    }
    bool operator== (DataConnection &dataConnection) {
        return this -> mCId == dataConnection.mCId;
    }

private:

    ApnSetting mApnSetting;
    int mInterfaceId; // interface id
    int mCId; // connection id returned by rild
    DataCallResponse mDataCallResponse;
    DcFail mDcFail;
    set<ApnContext*> mCompatibleApnContexts;
    RpDataController *mRpDataController;
};



#endif /* DATACONNECTION_H_ */
