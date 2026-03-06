// SPDX-License-Identifier: MediaTekProprietary
/*
 * PdnManager.h
 *
 *  Created on: Jun 28, 2017
 *      Author: mtk54458
 */

#ifndef MTK_RILPROXY_TELEPHONY_DATA_PDNMANAGER_H_
#define MTK_RILPROXY_TELEPHONY_DATA_PDNMANAGER_H_

#include "RfxMessage.h"
#include <utils/RefBase.h>

#include <string>
#include <vector>
#include <map>
#include <mutex>
using namespace std;

using ::android::sp;

typedef struct {
    int token;
    string apnType;
    sp<RfxMessage> request;
}PdnInfo;

class PdnManager {

public:
    PdnManager();

    void addSetupRequest(sp<RfxMessage> setupRequest);
    void removeSetupRequestByToken(int token);
    PdnInfo getSetupPdnInfoByToken(int token);

    void addTearDownRequst(sp<RfxMessage> tearDownRequest);
    void removeTearDownRequestByToken(int token);
    PdnInfo getTearDownPdnInfoByToken(int token);
    PdnInfo getTearDownPdnInfoByApn(string apnType);

    void addGetFailCauseRequest(int token, PdnInfo pdnInfo);
    void removeGetFailCauseRequestByToken(int token);
    PdnInfo getRetryPdnInfoByToken(int token);

    void dumpPdnInfo(vector<PdnInfo> pdnInfoPool);

private:
    vector<PdnInfo> mSetupRequestPool;
    vector<PdnInfo> mTearDownRequestPool;

    map<int, PdnInfo> mRetryPdnInfoPool;

    mutex m_setupReqVecMutex;
    mutex m_teardownReqVecMutex;
    mutex m_retrypdninfoMapMutex;
};

#endif /* MTK_RILPROXY_TELEPHONY_DATA_PDNMANAGER_H_ */
