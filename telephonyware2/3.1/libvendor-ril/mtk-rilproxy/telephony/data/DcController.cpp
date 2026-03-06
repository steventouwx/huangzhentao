// SPDX-License-Identifier: MediaTekProprietary
/*
 * DcController.cpp
 *
 *  Created on: May 2, 2017
 *      Author: mtk54458
 */

#include "DcController.h"
#include "DcTracker.h"
#include "RfxLog.h"
#include "RpDataUtils.h"
#include <string.h>
#include <prop/properties.h>
#include <stdlib.h>

#define RFX_LOG_TAG "DcController"

#ifdef NET_SOCKET_SUPPORT
int writeToNetsocket(void *data, size_t len);
#endif

const int DATA_CONNECTION_ACTIVE_PH_LINK_INACTIVE = 0;
const int DATA_CONNECTION_ACTIVE_PH_LINK_DORMANT = 1;
const int DATA_CONNECTION_ACTIVE_PH_LINK_UP = 2;

DcController::DcController():mLastActivePdn{0, 0, 0},mCurrentActivePdn{0, 0, 0} {
    DcController::mDcListActiveByCid.clear();
    DcController::mDcsToCleanup.clear();
    DcController::mDcsToRetry.clear();
    DcController::mDct = NULL;
    DcController::m_slot_id = -1;
}

void DcController::setDcTracker(DcTracker *dct) {
    DcController::mDct = dct;
    DcController::m_slot_id = dct->getSlotId();

}

int DcController::cidToProfileId(int cid) {
        char value[PROPERTY_VALUE_MAX] = { 0 };
        property_get("vendor.ril.default.profile.cid", value, "");
        if (cid == atoi(value)) {
            return 1;  // default
        }
        property_get("vendor.ril.private1.profile.cid", value, "");
        if (cid == atoi(value)) {
            return 2;  // private1
        }
        property_get("vendor.ril.private2.profile.cid", value, "");
        if (cid == atoi(value)) {
            return 3;  // private2
        }
        property_get("vendor.ril.ims.profile.cid", value, "");
        if (cid == atoi(value)) {
            return -1;  // ims, no need send to networkmanager
        }

        return -1;  // default return public profile
}

#ifdef NET_SOCKET_SUPPORT
void DcController::sendDeacivePdnFromAndroid() {
    for (int i = 0; i < 3; i++) {
        RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, mCurrentActivePdn[%d]=%d, mLastActivePdn[%d]=%d", i, mCurrentActivePdn[i], i, mLastActivePdn[i]);
        if (mCurrentActivePdn[i] - mLastActivePdn[i] == -1) {
            pdn_connection[i].pdnState = 4;
            writeToNetsocket(&pdn_connection[i],sizeof(struct netSocketData));
            if (DcController::mDct != NULL) {
                DcController::mDct->setApnStateByApnType(pdn_connection[i].apnType, RIL_Data_Call_PdnState::PDN_DISCONNECTED);
            }
        }
        mLastActivePdn[i] = mCurrentActivePdn[i];
    }
}

int DcController::profileIdToMtu(int profileId) {
    char value[PROPERTY_VALUE_MAX] = { 0 };
    if (profileId == RpDataUtils::MOBILE_APN) {
        property_get("vendor.ril.default.profile.mtu", value, "0");
        return atoi(value);
    } else if (profileId == RpDataUtils::RCS_APN) {
        property_get("vendor.ril.rcs.profile.mtu", value, "0");
        return atoi(value);
    } else if (profileId == RpDataUtils::BIP_APN) {
        property_get("vendor.ril.bip.profile.mtu", value, "0");
        return atoi(value);
    } else {
        return 0;
    }
}

int DcController::getPreferMtu(int mtu_in, int profileId_in) {
    int mtuFromApnConfig = DcController::profileIdToMtu(profileId_in);

    char value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("vendor.gsm.ril.uicc.mccmnc", value, "");
    int mtuFromMccmnc = RpDataUtils::getMtuByMccmnc(value);
    if (mtu_in <= 0) {
        if (mtuFromApnConfig > 0) {
            return mtuFromApnConfig;
        } else if (mtuFromMccmnc > 0) {
            return mtuFromMccmnc;
        } else {
            return 1500;
        }
    } else {
        return mtu_in;
    }
}
#endif

void DcController::onDataStateChanged(const sp<RfxMessage>& message) {

    vector<DataCallResponse> dcsList = RpDataUtils::parseDataConnectionInfo(message->getParcel());
    RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, with DataCallResponse number: %u", dcsList.size());

    //insert all the unsol_data_call_list_change entries to dataCallResponseListByCid
    map<int, DataCallResponse> dataCallResponseListByCid;
    vector<DataCallResponse>::iterator itorCallList = dcsList.begin();
    mCurrentActivePdn[0] = 0;
    mCurrentActivePdn[1] = 0;
    mCurrentActivePdn[2] = 0;

    while (itorCallList != dcsList.end()) {
        RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, add dataCallResponse by cid: %d", itorCallList->cid);

        #ifdef NET_SOCKET_SUPPORT
        int mprofileId = cidToProfileId(itorCallList->cid);
        RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, cid to profileId: %d", mprofileId);
        if (mprofileId == -1) {
            RFX_LOG_D(RFX_LOG_TAG, "no need save and report to networkmanager +++++");
            dataCallResponseListByCid.insert(pair<int, DataCallResponse>(itorCallList->cid, *itorCallList));
            itorCallList++;
            continue;
        }

        mCurrentActivePdn[mprofileId - 1] = 1;

        // memset(pdn_connection[mprofileId - 1], 0, sizeof(pdn_connection[mprofileId - 1]));
        pdn_connection[mprofileId - 1].profileId = mprofileId;
        pdn_connection[mprofileId - 1].pdnState = itorCallList->active;
        pdn_connection[mprofileId - 1].failcause = itorCallList->status;
        pdn_connection[mprofileId - 1].online = 0;
        pdn_connection[mprofileId - 1].mtu = getPreferMtu(itorCallList->mtu, mprofileId);
        pdn_connection[mprofileId - 1].mtuV6 = getPreferMtu(itorCallList->mtuV6, mprofileId);

        RFX_LOG_I(RFX_LOG_TAG, "onDataStateChanged, output mtu: %d, mtuV6: %d",
            pdn_connection[mprofileId - 1].mtu, pdn_connection[mprofileId - 1].mtuV6);

        memcpy(pdn_connection[mprofileId - 1].apnType, (RpDataUtils::profileId2Type(mprofileId)).c_str(),
            strlen((RpDataUtils::profileId2Type(mprofileId)).c_str()));
        pdn_connection[mprofileId - 1].apnType[strlen((RpDataUtils::profileId2Type(mprofileId)).c_str())] = '\0';
        memcpy(pdn_connection[mprofileId - 1].type, (itorCallList->type).c_str(),
            strlen((itorCallList->type).c_str()));
        pdn_connection[mprofileId - 1].type[strlen((itorCallList->type).c_str())] = '\0';
        memcpy(pdn_connection[mprofileId - 1].ifname, (itorCallList->ifname).c_str(),
            strlen((itorCallList->ifname).c_str()));
        pdn_connection[mprofileId - 1].ifname[strlen((itorCallList->ifname).c_str())] = '\0';
        memcpy(pdn_connection[mprofileId - 1].addresses, (itorCallList->addresses).c_str(),
            strlen((itorCallList->addresses).c_str()));
        pdn_connection[mprofileId - 1].addresses[strlen((itorCallList->addresses).c_str())] = '\0';
        memcpy(pdn_connection[mprofileId - 1].dnses, (itorCallList->dnses).c_str(),
            strlen((itorCallList->dnses).c_str()));
        pdn_connection[mprofileId - 1].dnses[strlen((itorCallList->dnses).c_str())] = '\0';
        memcpy(pdn_connection[mprofileId - 1].gateways, (itorCallList->gateways).c_str(),
            strlen((itorCallList->gateways).c_str()));
        pdn_connection[mprofileId - 1].gateways[strlen((itorCallList->gateways).c_str())] = '\0';
        // memcpy(pdn_connection[mprofileId - 1].pcscf, (itorCallList->pcscf).c_str(),
        //     strlen((itorCallList->pcscf).c_str()));
        // pdn_connection[mprofileId - 1].pcscf[strlen((itorCallList->pcscf).c_str())] = '\0';
        writeToNetsocket(&pdn_connection[mprofileId - 1],sizeof(struct netSocketData));

        if ((pdn_connection[mprofileId - 1].pdnState == 2) && (DcController::mDct != NULL)) {
            DcController::mDct->setApnStateByApnType(pdn_connection[mprofileId - 1].apnType, RIL_Data_Call_PdnState::PDN_CONNECTED);
        }

        #endif

        dataCallResponseListByCid.insert(pair<int, DataCallResponse>(itorCallList->cid, *itorCallList));
        itorCallList++;
    }

#ifdef NET_SOCKET_SUPPORT
    DcController::sendDeacivePdnFromAndroid();
#endif

    //no need to go through the retry logic if there is no active pdn list.
    if (DcController::mDcListActiveByCid.size() == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "onDataStateChanged, with no active pdn list recored.");

        return;
    }

    // make sure get the valid clean/retry info, clear up the old info first.
    DcController::mDcsToCleanup.clear();
    DcController::mDcsToRetry.clear();

    //push all old active pdn but current not exsit PDN to mDcsToRetry
    map<int, DataConnection*>::iterator itorActiveList = DcController::mDcListActiveByCid.begin();
    while (itorActiveList != DcController::mDcListActiveByCid.end()) {

        if (dataCallResponseListByCid.count(itorActiveList->first) == 0) {

            RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, add data connection to retry list by cid: %d",
                            itorActiveList->first);

            mDcsToRetry.push_back(itorActiveList->second);
        }
        itorActiveList++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, mDcsToRetry.size=%u, dcsList or dataCallResponseListByCid.size=%u. mDcListActiveByCid.size=%d",
        mDcsToRetry.size(), dataCallResponseListByCid.size(), mDcListActiveByCid.size());

    //find current report active data list from old active data list
    itorCallList = dcsList.begin();
    while (itorCallList != dcsList.end()) {
        itorActiveList = DcController::mDcListActiveByCid.find(itorCallList->cid);
        if (itorActiveList == DcController::mDcListActiveByCid.end()) {
            itorCallList++;
            continue;
        }

        if (itorCallList->active == DATA_CONNECTION_ACTIVE_PH_LINK_INACTIVE) {
            DcFail dcFail(RIL_DataCallFailCause(itorCallList->status));
            if (mDct->isPermanentFail(dcFail)) {
                RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, add data connection to cleanup list by cid: %d",
                                itorActiveList->first);

                mDcsToCleanup.push_back(itorActiveList->second);
            } else {
                RFX_LOG_D(RFX_LOG_TAG,"onDataStateChanged, add data connection to retry list by cid: %d",
                                itorActiveList->first);

                mDcsToRetry.push_back(itorActiveList->second);
            }

        } else if (itorCallList->active == DATA_CONNECTION_ACTIVE_PH_LINK_UP) {
            // If the same address type was removed and added we need to cleanup
            if (itorActiveList->second != NULL) {
                if ((itorActiveList->second)->getDataCallResponse().ifname == itorCallList->ifname) {
                    //need clean up the pdn if ip address changed-->change to upate the saved database

                    RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, local ipv4: %s ,remote ipv4:%s,local ipv6:%s, remote ipv6:%s, %p, %p",
                                (itorActiveList->second)->getDataCallResponse().ipv4.c_str(), itorCallList->ipv4.c_str(),
                                (itorActiveList->second)->getDataCallResponse().ipv6.c_str(), itorCallList->ipv6.c_str(),
                                (itorActiveList->second));
                    if ((itorActiveList->second)->getDataCallResponse().ipv4 != itorCallList->ipv4
                        || (itorActiveList->second)->getDataCallResponse().ipv6 != itorCallList->ipv6
                        || (itorActiveList->second)->getDataCallResponse().dnses != itorCallList->dnses
                        || (itorActiveList->second)->getDataCallResponse().gateways != itorCallList->gateways)
                    {
                        itorCallList->netId = (itorActiveList->second)->getDataCallResponse().netId;
                        (itorActiveList->second)->setDataCallResponse((DataCallResponse)*itorCallList);
                        // if((itorActiveList->second)->getDataCallResponse() != *itorCallList)
                        {
                            RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged not eq");
                        }
                        set<ApnContext*> compatibleApnContexts = (itorActiveList->second)->getCompatibleApnContext();

                        set<ApnContext*>::iterator comApnContextItor = compatibleApnContexts.begin();
                        while (comApnContextItor != compatibleApnContexts.end())
                        {
                            (*comApnContextItor)->notifyStateChange(RIL_Data_Call_PdnState::PDN_CONNECTED);

                            RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged comApnContextItor->mApnType=%s",
                                (*comApnContextItor)->getApnType().c_str());

                            comApnContextItor++;

                        }
                    }
                    else
                    {
                        RFX_LOG_W(RFX_LOG_TAG, "onDataStateChanged, All the paramenter are the same.");
                    }

                }
                else
                {
                    RFX_LOG_W(RFX_LOG_TAG, "onDataStateChanged, Interface Name changed, Error!!!");
                }
            } else {
                RFX_LOG_W(RFX_LOG_TAG, "onDataStateChanged, with NULL data connection!!!");
            }
        }
        itorCallList++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "onDataStateChanged, mDcsToCleanup.size: %u, mDcsToRetry.size: %u",
        mDcsToCleanup.size(), mDcsToRetry.size());

    DcController::cleanupDataConnections();
    DcController::retryDataConnections();
}

void DcController::retryDataConnections() {
    vector<DataConnection*>::iterator itorToRetry = DcController::mDcsToRetry.begin();
    while (itorToRetry != DcController::mDcsToRetry.end()) {
        RFX_LOG_D(RFX_LOG_TAG, "retryDataConnections, retry pdn with cid: %d", (*itorToRetry)->getcId());

        //retry to set up the data connections
        if((*itorToRetry)->getcId() == -1) {
            RFX_LOG_W(RFX_LOG_TAG,"retryDataConnections should not be here!");
            return;
        }
        set<ApnContext*> compatibleApnContexts = (*itorToRetry)->getCompatibleApnContext();
        set<ApnContext*>::iterator comApnContextItor = compatibleApnContexts.begin();

        DataConnection *dataConnection = (*comApnContextItor)->getDataConnection();

        while (comApnContextItor != compatibleApnContexts.end())
        {
            RFX_LOG_D(RFX_LOG_TAG, "retryDataConnections, the pdn with apn type: %s is disconnected",
                            (*comApnContextItor)->getApnType().c_str());

            // now the pdn is disconnected, need bring it up later.
            (*comApnContextItor)->setDataConnection(NULL);
            (*comApnContextItor)->setSate(RIL_Data_Call_PdnState::PDN_DISCONNECTED);
            if ((*comApnContextItor)->getRefCount() == 1) {
                (*comApnContextItor)->decRefCount();
            }
            // remove the data connection from active list.
            DcController::removeActiveDcByCid(*itorToRetry);

            comApnContextItor++;
        }
        if(dataConnection)
        {
            delete dataConnection;
        }
        vector<string> mRetryApntype;
        mRetryApntype.clear();

        comApnContextItor = compatibleApnContexts.begin();
        while (comApnContextItor!=compatibleApnContexts.end())
        {

            RFX_LOG_D(RFX_LOG_TAG, "retryDataConnections, push_back retry pdn with apn type: %s",
                        (*comApnContextItor)->getApnType().c_str());

            mRetryApntype.push_back((*comApnContextItor)->getApnType());
            comApnContextItor++;
        }

        //clear up the apn context which is binded with the data connection.
        compatibleApnContexts.clear();

        if (DcController::mDct != NULL)
        {
            vector<string>::iterator itor_apn_type = mRetryApntype.begin();
            while (itor_apn_type != mRetryApntype.end())
            {
                RFX_LOG_D(RFX_LOG_TAG, "retryDataConnections, retry pdn with apn type: %s canceled",(*itor_apn_type).c_str());
                //mDct->reTrySetupData(*itor_apn_type, std::string("data lost"));
                itor_apn_type++;
            }
        }
        itorToRetry++;
    }

    //RFX_LOG_D(RFX_LOG_TAG, "retryDataConnections, retry pdn message send done.");
}

void DcController::cleanupDataConnections() {
    vector<DataConnection*>::iterator itorToCleanup = DcController::mDcsToCleanup.begin();
    while (itorToCleanup != DcController::mDcsToCleanup.end()) {
        RFX_LOG_I(RFX_LOG_TAG, "cleanupDataConnections, clean up pdn with cid: %d", (*itorToCleanup)->getcId());

        // clear up the data connections
        set<ApnContext*> compatibleApnContexts =  (*itorToCleanup)->getCompatibleApnContext();
        set<ApnContext*>::iterator comApnContextItor = compatibleApnContexts.begin();

        if (DcController::mDct != NULL) {
            while (comApnContextItor != compatibleApnContexts.end()) {
                string apnType = (*comApnContextItor)->getApnType();
                sp<RfxMessage> tearDowndataMessage = RfxMessage::obtainRequest(m_slot_id,
                                                                   RADIO_TECH_GROUP_GSM,
                                                                   RIL_REQUEST_DEACTIVATE_DATA_CALL);

                Parcel* parcel = tearDowndataMessage->getParcel();
                // make sure get the right position
                parcel->setDataPosition(sizeof(int32_t)*2);
                RpDataUtils::writeStringToParcel(parcel, apnType.c_str());

                RFX_LOG_D(RFX_LOG_TAG, "cleanupDataConnections with apnType: %s", apnType.c_str());

                mDct->cleanUpConnection(apnType, tearDowndataMessage);
                comApnContextItor++;
            }
        }

        //clear up the apn context which is binded with the data connection.
        compatibleApnContexts.clear();
        itorToCleanup++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "cleanupDataConnections, clean up pdn message send done.");
}

void DcController::addActiveDcByCid(DataConnection *dc) {
    RFX_LOG_D(RFX_LOG_TAG, "addActiveDcByCid, with cId: %d", dc->getcId());

    // remove the dc first.
    DcController::mDcListActiveByCid.erase(dc->getcId());
    DcController::mDcListActiveByCid.insert(pair<int, DataConnection*>(dc->getcId(), dc));

    RFX_LOG_D(RFX_LOG_TAG, "addActiveDcByCid, now the data connection number is: %u",mDcListActiveByCid.size());
}

void DcController::removeActiveDcByCid(DataConnection *dc) {
    RFX_LOG_D(RFX_LOG_TAG, "removeActiveDcByCid, with cId: %d", dc->getcId());

    DcController::mDcListActiveByCid.erase(dc->getcId());

    RFX_LOG_D(RFX_LOG_TAG, "removeActiveDcByCid, now the data connection number is: %u",
                    DcController::mDcListActiveByCid.size());
}

map<int, DataConnection*> DcController::getActiveDataConnections() {
    RFX_LOG_D(RFX_LOG_TAG, "getActiveDataConnections, now the data connection number is: %u",
                    DcController::mDcListActiveByCid.size());

    return DcController::mDcListActiveByCid;
}
