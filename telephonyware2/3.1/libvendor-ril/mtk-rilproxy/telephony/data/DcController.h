// SPDX-License-Identifier: MediaTekProprietary
/*
 * DcController.h
 *
 *  Created on: May 2, 2017
 *      Author: mtk54458
 */

#ifndef DCCONTROLLER_H_
#define DCCONTROLLER_H_

#include "DataCallResponse.h"
#include "DataConnection.h"
#include <map>
#include <vector>
using namespace std;

class DcTracker;
class DcController {
public:
    DcController();
    int cidToProfileId(int cid);
#ifdef NET_SOCKET_SUPPORT
    void sendDeacivePdnFromAndroid();
    int profileIdToMtu(int profileId);
    int getPreferMtu(int mtu_in, int profileId_in);
#endif
    void onDataStateChanged(const sp<RfxMessage>& message);
    void addActiveDcByCid(DataConnection *dc);
    void removeActiveDcByCid(DataConnection *dc);
    void setDcTracker(DcTracker *dct);
    map<int, DataConnection*> getActiveDataConnections();

    void retryDataConnections();
    void cleanupDataConnections();
    int getSlotId() const {
        return m_slot_id;
    }

private:
    map<int, DataConnection*> mDcListActiveByCid;
    vector<DataConnection*> mDcsToRetry;
    vector<DataConnection*> mDcsToCleanup;
    int m_slot_id;
    struct netSocketData pdn_connection[3];  // for public pdn of default type from android
    int mLastActivePdn[3];  // last active pdn state of profile id 1 2 3
    int mCurrentActivePdn[3];  // current active pdn state of profile id 1 2 3

    DcTracker *mDct;
};



#endif /* DCCONTROLLER_H_ */
