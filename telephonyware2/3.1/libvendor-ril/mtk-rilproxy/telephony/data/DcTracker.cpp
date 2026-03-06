// SPDX-License-Identifier: MediaTekProprietary
/*
 * DcTracker.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: mtk54458
 */

#include "DcTracker.h"

#include <limits.h>
#include <iterator>
#include <utility>
#include <prop/properties.h>

#include "ApnSetting.h"
#include "DataCallResponse.h"
#include "DcFail.h"
#include "RetryManager.h"
#include "RfxLog.h"
#include "../include/telephony/ril.h"
#include "RpDataUtils.h"
#include "RpDataController.h"
#include "MbrainClient.h"

#define RFX_LOG_TAG "DcTracker"

DcTracker::DcTracker() {
    DcTracker::mAttached = false;
    DcTracker::mIsApnReady = false;
    DcTracker::mAllApnSettings.clear();
    DcTracker::mApnContexts.clear();
   // DcTracker::mDataConnections.clear();
    DcTracker::mPrioritySortedApnContexts.clear();
    DcTracker::mRpDataController = NULL;

    // init all the apn context the earlier the better.
    DcTracker::initApnContext();
    DcTracker::mDcController.setDcTracker(this);
    DcTracker::m_slot_id= -1;
    RFX_LOG_I(RFX_LOG_TAG, "initApnContext done!");
}

DcTracker::~DcTracker() {
    list<ApnContext*>::iterator listItor =
            DcTracker::mPrioritySortedApnContexts.begin();
    while (listItor != DcTracker::mPrioritySortedApnContexts.end()) {
        if (*listItor != NULL) {
            delete *listItor;
            *listItor = NULL;
        }
        listItor++;
    }
}


void DcTracker::initApnContext() {
    for (int i = 0; i < APN_CONTEXT_NUMBER; i++) {
        NetworkConfig networkConfig(NETWORK_CONFIG[i]);
        switch (networkConfig.mType) {
        case TYPE_MOBILE:
            DcTracker::addApnContext(TEXT_APN_TYPE_DEFAULT, networkConfig);
            break;
        case TYPE_MOBILE_MMS:
            DcTracker::addApnContext(TEXT_APN_TYPE_MMS, networkConfig);
            break;
        case TYPE_MOBILE_SUPL:
            DcTracker::addApnContext(TEXT_APN_TYPE_SUPL, networkConfig);
            break;
        case TYPE_MOBILE_DUN:
            DcTracker::addApnContext(TEXT_APN_TYPE_DUN, networkConfig);
            break;
        case TYPE_MOBILE_HIPRI:
            DcTracker::addApnContext(TEXT_APN_TYPE_HIPRI, networkConfig);
            break;
        case TYPE_MOBILE_FOTA:
            DcTracker::addApnContext(TEXT_APN_TYPE_FOTA, networkConfig);
            break;
        case TYPE_MOBILE_IMS:
            DcTracker::addApnContext(TEXT_APN_TYPE_IMS, networkConfig);
            break;
        case TYPE_MOBILE_CBS:
            DcTracker::addApnContext(TEXT_APN_TYPE_CBS, networkConfig);
            break;
        case TYPE_MOBILE_WAP:
            DcTracker::addApnContext(TEXT_APN_TYPE_WAP, networkConfig);
            break;
        case TYPE_MOBILE_RCS:
            DcTracker::addApnContext(TEXT_APN_TYPE_RCS, networkConfig);
            break;
        case TYPE_MOBILE_IA:
            DcTracker::addApnContext(TEXT_APN_TYPE_IA, networkConfig);
            break;
        case TYPE_MOBILE_BIP:
            DcTracker::addApnContext(TEXT_APN_TYPE_BIP, networkConfig);
            break;
        case TYPE_MOBILE_EMERGENCY:
            DcTracker::addApnContext(TEXT_APN_TYPE_EMERGENCY, networkConfig);
            break;
        //for IOT start
        case TYPE_IOT_DEFAULT:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_DEFAULT, networkConfig);
            break;
        case TYPE_IOT_NET_0:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_NET_0, networkConfig);
            break;
        case TYPE_IOT_NET_1:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_NET_1, networkConfig);
            break;
        case TYPE_IOT_NET_2:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_NET_2, networkConfig);
            break;
        case TYPE_IOT_NET_3:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_NET_3, networkConfig);
            break;
        case TYPE_IOT_NET_4:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_NET_4, networkConfig);
            break;
        case TYPE_IOT_NET_5:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_NET_5, networkConfig);
            break;
        case TYPE_IOT_NET_6:
            DcTracker::addApnContext(TEXT_IOT_APN_TYPE_NET_6, networkConfig);
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "initApnContext %s is not support ", networkConfig.mName.c_str());

            continue;
        }
    }
}

void DcTracker::addApnContext(string type, NetworkConfig networkConfig) {
    ApnContext *apnContext = new ApnContext(type, networkConfig);

    RFX_LOG_D(RFX_LOG_TAG, "addApnContext type %s", type.c_str());

    DcTracker::mApnContexts.insert(pair<string, ApnContext*>(type, apnContext));
    DcTracker::mPrioritySortedApnContexts.push_front(apnContext);
}

void DcTracker::requestNetwork(const sp<RfxMessage>& request) {

    Parcel* parcel = request->getParcel();

    // make sure get the right position
    parcel->setDataPosition(sizeof(int32_t)*2);
    char* apnInfo = RpDataUtils::strdupReadString(parcel);
    if (apnInfo == NULL ) {
        RFX_LOG_W(RFX_LOG_TAG, "requestNetwork with invalid argument %d", parcel->dataSize());
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }
    if (apnInfo != NULL && strlen(apnInfo) == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "requestNetwork with invalid argument %d", parcel->dataSize());
        RpDataUtils::freeMemory(apnInfo);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }

    // get the requested apn
    string apnType = std::string(apnInfo);
    RpDataUtils::freeMemory(apnInfo);

    RFX_LOG_D(RFX_LOG_TAG, "requestNetwork with apnType: %s, with request %s, and the mapped token: %d",
            apnType.c_str(), RpDataUtils::requestOrUrcToString(request->getId()), request->getToken());
    if (!DcTracker::isValidRequest(apnType)) {
        RFX_LOG_W(RFX_LOG_TAG, "requestNetwork with invalid apn type.");
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }
    RFX_LOG_D(RFX_LOG_TAG, "requestNetwork with valid apn type.");
    int profileId = RpDataUtils::type2profileId(apnType);
    if(DcTracker::mApnContexts.find(apnType)==DcTracker::mApnContexts.end())
    {
        RFX_LOG_W(RFX_LOG_TAG, "requestNetwork no APN DB not support.");
        notifyMbrainDataPoint(profileId, Data_Call_No_APN_DB_Support);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }
    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;
    if(apnContext == NULL)
    {
        RFX_LOG_W(RFX_LOG_TAG, "requestNetwork no APN DB exist.");
        notifyMbrainDataPoint(profileId, Data_Call_No_APN_DB_Exist);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }
    apnContext->incRefCount();
    if (apnContext->getRefCount() == 1) {
        apnContext->setClientSetupDataRequest(request);
        apnContext->setEnabled(true);

        DcTracker::trySetupData(apnType, request, "user requested");
    } else {
        RFX_LOG_W(RFX_LOG_TAG, "requestNetwork with apnType: %s , apnContext->getRefCount()=%d already exist %s, ignore "
            "this request!!!",apnType.c_str(), apnContext->getRefCount(), RpDataUtils::apnState2string(apnContext->getState()));
        apnContext->decRefCount();
        if((apnContext->getState() == RIL_Data_Call_PdnState::PDN_CONNECTING) ||
            (apnContext->getState() == RIL_Data_Call_PdnState::PDN_CONNECTED)) {
            DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_ABORTED, request);
            apnContext->notifyStateChange(apnContext->getState());
        }else {
            notifyMbrainDataPoint(profileId, Data_Call_Unkown_Error);
            DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        }
    }
}

void DcTracker::deactiveNetwork(const sp<RfxMessage>& request) {
    Parcel* parcel = request->getParcel();
    // make sure get the right position
    parcel->setDataPosition(sizeof(int32_t)*2);
    char* apnInfo = RpDataUtils::strdupReadString(parcel);
    if (apnInfo == NULL) {
        RFX_LOG_W(RFX_LOG_TAG, "deactiveNetwork with invalid argument %d", parcel->dataSize());
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }
    if (apnInfo != NULL && strlen(apnInfo) == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "deactiveNetwork with invalid argument %d", parcel->dataSize());
        RpDataUtils::freeMemory(apnInfo);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }

    // get the requested apn
    string requestApn = std::string(apnInfo);
    RpDataUtils::freeMemory(apnInfo);
    RFX_LOG_D(RFX_LOG_TAG, "deactiveNetwork with apnType: %s, with request %s, with token: %d, source %d",
            requestApn.c_str(), RpDataUtils::requestOrUrcToString(request->getId()),
            request->getToken(),request->getSource());

    if (!DcTracker::isValidRequest(requestApn)) {
        RFX_LOG_W(RFX_LOG_TAG, "deactiveNetwork with invalid apn type.");
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }

    ApnContext *apnContext = DcTracker::mApnContexts.find(requestApn)->second;
    int profileId = RpDataUtils::type2profileId(requestApn);

    if (apnContext->getRefCount() == 1) {
        apnContext->decRefCount();
        apnContext->setEnabled(false);

       // DcTracker::mPdnManager.addTearDownRequst(request);

        DcTracker::cleanUpConnection(requestApn, request);
    } else if (apnContext->getRefCount() < 1) {
        RFX_LOG_W(RFX_LOG_TAG, "deactiveNetwork with apnType: %s don't exist, ignore this request!!!", requestApn.c_str());
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_ABORTED, request);
        apnContext->notifyStateChange(RIL_Data_Call_PdnState::PDN_DISCONNECTED);
    } else {
        apnContext->decRefCount();
        RFX_LOG_W(RFX_LOG_TAG, "deactiveNetwork with apnType: %s is using, ignore this request!!!", requestApn.c_str());
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_ABORTED, request);
        apnContext->notifyStateChange(RIL_Data_Call_PdnState::PDN_DISCONNECTED);
    }
}

void DcTracker::cleanUpConnection(string apnType, const sp<RfxMessage>& request) {
    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;

    int profileId = RpDataUtils::type2profileId(apnType);
    DataConnection *dataConnection = apnContext->getDataConnection();
    if (dataConnection != NULL) {
//        if (DcTracker::hasCompatibleConnectedApnContext(apnType)) {
//            RFX_LOG_D(RFX_LOG_TAG,"hasCompatibleConnectedApnContext, just remove from compatible,"
//                "mark own state to disconnected!.");
//            dataConnection->removeCompatibleApnContext(apnContext);
//            apnContext->setDataConnection(NULL);
//            apnContext->setClientTearDownDataRequest(request);
//            //apnContext->setSate(RIL_Data_Call_PdnState::PDN_DISCONNECTING);
//            DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
//            //apnContext->setSate(RIL_Data_Call_PdnState::PDN_DISCONNECTED);
//            return;
//        }
        int cId = dataConnection->getcId();
        if (cId != DC_INVALID_ID) {
            // recored the tear down request.
            apnContext->setClientTearDownDataRequest(request);
            apnContext->setSate(RIL_Data_Call_PdnState::PDN_DISCONNECTING);
            DcTracker::mPdnManager.addTearDownRequst(request);
            dataConnection->deactivateDataCall(cId, request);
            if(!(DcTracker::mRpDataController->isAttached())) {
                DcTracker::onDeactiveConnectionCompleted(request);
            }
        } else {
            apnContext->incRefCount();
            apnContext->setEnabled(true);
            notifyMbrainDataPoint(profileId, Data_Call_DC_Invalid_Id);
            DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
            RFX_LOG_W(RFX_LOG_TAG,"cleanUpConnection, no valid cId! should not be here!");
        }
    } else {
        if(apnContext->getState() != RIL_Data_Call_PdnState::PDN_IDLE) {
            apnContext->incRefCount();
            apnContext->setEnabled(true);
        }
        notifyMbrainDataPoint(profileId, Data_Call_ClearUpConnection_No_Connection);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        RFX_LOG_W(RFX_LOG_TAG, "cleanUpConnection, no connection, should not be here!.");
    }
}

void DcTracker::onDeactiveConnectionCompleted(const sp<RfxMessage>& response) {

    int token = response->getToken();
    RFX_LOG_D(RFX_LOG_TAG, "onDeactiveConnectionCompleted, with token: %d", token);

    PdnInfo pdnInfo = DcTracker::mPdnManager.getTearDownPdnInfoByToken(token);
    sp<RfxMessage> requestMessage = pdnInfo.request;

    if (requestMessage.get() == 0) {
        RFX_LOG_W(RFX_LOG_TAG,"onDeactiveConnectionCompleted, not find valid tear down data request!!!");

        return;
    }

    // get the requested apn
    string apnType = pdnInfo.apnType;
    if (apnType.empty()) {
        RFX_LOG_W(RFX_LOG_TAG,"onDeactiveConnectionCompleted, with null apn info. should never be here!!!");

        return;
    }

    RFX_LOG_I(RFX_LOG_TAG, "onDeactiveConnectionCompleted, with apnType: %s", apnType.c_str());

    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;
    DataConnection *currentDataConnection = apnContext->getDataConnection();

    //the old connection is invalid.
    apnContext->setSate(RIL_Data_Call_PdnState::PDN_DISCONNECTED);

    if (currentDataConnection != NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "onDeactiveConnectionCompleted, free data connection with info: %s .",
                (currentDataConnection->logInfo()).c_str());
        currentDataConnection->removeCompatibleApnContext(apnContext);
        if (DcTracker::hasCompatibleConnectedApnContext(apnType)) {
            RFX_LOG_W(RFX_LOG_TAG,"hasCompatibleConnectedApnContext, just remove from compatible,"
                "should not be here!.");

        }
        else
        {
            DcTracker::mDcController.removeActiveDcByCid(currentDataConnection);
            // no need to teardown the network of default pdn.
            DataCallResponse dcResponse = currentDataConnection->getDataCallResponse();
            if (dcResponse.netId != -1)
            {
                RFX_LOG_D(RFX_LOG_TAG, "onDeactiveConnectionCompleted, free data connection with info: %s, netid %d, ifname = %s .",
                    (currentDataConnection->logInfo()).c_str(), dcResponse.netId, dcResponse.ifname.c_str());
                RpDataUtils::tearDownNetwork(dcResponse.netId, dcResponse.ifname);
            }

            // free the memory
            delete currentDataConnection;
        }
    }

    apnContext->setDataConnection(NULL);
    //the current apn setting should be invalid.
    apnContext->getApnSetting().clear();
    // remove the record
    DcTracker::mPdnManager.removeTearDownRequestByToken(token);

    RFX_LOG_I(RFX_LOG_TAG, "onDeactiveConnectionCompleted, deactivate PDN done, with apnType: %s",
            apnType.c_str());
}

bool DcTracker::hasCompatibleConnectedApnContext(string apnType) {

    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;
    DataConnection *dataConnection = apnContext->getDataConnection();

    bool result = false;

    if(dataConnection != NULL)
    {
        map<string, ApnContext*>::iterator itor = DcTracker::mApnContexts.begin();
        while (itor != DcTracker::mApnContexts.end()) {
            ApnContext *checkedApnContext = itor->second;
            DataConnection *checkedDataConnection = checkedApnContext->getDataConnection();
            if (apnContext->getApnType() != checkedApnContext->getApnType()) {
                if (checkedDataConnection != NULL) {
                    if (checkedDataConnection->getcId() == dataConnection->getcId()) {
                        result = true;
                        break;
                    }
                }
            }
            itor++;
        }
    }


    RFX_LOG_D(RFX_LOG_TAG, "hasCompatibleConnectedApnContext, checked with apn type: %s , with checked result: %d",
            (apnContext->getApnType()).c_str(), result);

    return result;
}

void DcTracker::onCallStateStart() {
    RFX_LOG_D(RFX_LOG_TAG, "onCallStateStart");
    // do nothing.
}

void DcTracker::onCallStateEnd() {
    RFX_LOG_D(RFX_LOG_TAG, "onCallStateEnd");

    DcTracker::setupDataOnConnectableApns(REASON_CALL_END);
}

void DcTracker::onDataConnectionDetached() {
    RFX_LOG_D(RFX_LOG_TAG, "onDataConnectionDetached");

    DcTracker::mAttached = false;
}

void DcTracker::onDataConnectionAttached() {
    RFX_LOG_D(RFX_LOG_TAG, "onDataConnectionAttached");

    DcTracker::mAttached = true;
    DcTracker::setupDataOnConnectableApns(REASON_DATA_ATTACHED);
}

void DcTracker::onPsRestrict(bool psRestrict) {
    RFX_LOG_D(RFX_LOG_TAG, "onPsRestrict");
    if (!psRestrict) {
        DcTracker::setupDataOnConnectableApns(REASON_PS_RESTRICT);
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "onPsRestrict, ps is restrict.");
    }
}

void DcTracker::onApnReady(Vector<ApnsettingT*>* list) {
    RFX_LOG_I(RFX_LOG_TAG, "onApnReady");

    DcTracker::mIsApnReady = true;

    DcTracker::createAllApnList(list);
    DcTracker::setupDataOnConnectableApns(REASON_APN_READY);
}

void DcTracker::createAllApnList(Vector<ApnsettingT*>* list) {
    RFX_LOG_D(RFX_LOG_TAG, "createAllApnList");
    DcTracker::mAllApnSettings.clear();

    if (list != NULL) {
        int size = list->size();
        for (int i = 0; i < size; i++) {
            ApnsettingT* apnSetting = list->itemAt(i);
            DcTracker::mAllApnSettings.push_back(ApnSetting(apnSetting->id, apnSetting->numberic, apnSetting->carrier,
                    apnSetting->apn, apnSetting->proxy, apnSetting->port, apnSetting->mmsc, apnSetting->mms_proxy,
                    apnSetting->mms_port, apnSetting->user, apnSetting->password, apnSetting->auth_type,
                    apnSetting->types, apnSetting->protocol, apnSetting->roaming_protocol, apnSetting->carrier_enabled,
                    apnSetting->bear, apnSetting->bear_bitmask, apnSetting->profile_id, apnSetting->modem_cognitive,
                    apnSetting->max_conns, apnSetting->wait_time, apnSetting->max_conns_time, apnSetting->mtu,
                    apnSetting->mvno_type, apnSetting->mvno_match_data));
        }
    }
    // free the memory
    if (list != NULL) {
           int size = list->size();
           for (int i = 0; i < size; i++) {
               delete list->itemAt(i);
           }
           delete list;
       }
}

void DcTracker::trySetupData(string apnType, const sp<RfxMessage>& request, string reason) {
    RFX_LOG_D(RFX_LOG_TAG, "trySetupData with apn type: %s, with reason: %s",
                    apnType.c_str(), reason.c_str());

    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;

    DataAllowFailReason dataAllowFailReason;
    bool isDataAllowed = DcTracker::isDataAllowed(dataAllowFailReason);

    RFX_LOG_I(RFX_LOG_TAG, "trySetupData with apnType: %s ,with dataAllowed result:%s ",
                             (apnContext->getApnType()).c_str(), RpDataUtils::b2s(isDataAllowed).c_str());

    if(apnContext->getState() == RIL_Data_Call_PdnState::PDN_FAILED){
        RFX_LOG_W(RFX_LOG_TAG, "trySetupData: make a FAILED ApnContext IDLE so its reusable");
        apnContext->setSate(RIL_Data_Call_PdnState::PDN_IDLE);
    }

    int profileId = RpDataUtils::type2profileId(apnType);
    if (apnContext->isEnabled() && isDataAllowed) {
        if (apnContext->getState() == RIL_Data_Call_PdnState::PDN_IDLE
                || apnContext->getState() == RIL_Data_Call_PdnState::PDN_DISCONNECTED
                || apnContext->getState() == RIL_Data_Call_PdnState::PDN_SCANNING) {
            vector<ApnSetting> waitingApns;
            waitingApns = DcTracker::buildWaitingApns(apnContext->getApnType());

            RFX_LOG_D(RFX_LOG_TAG, "trySetupData set WaitingApns size=%d ", waitingApns.size());
            if (waitingApns.size() == 0) {

                RFX_LOG_W(RFX_LOG_TAG, "trySetupData with apnType: %s ,but not find valid apn setting!!!! ",
                         (apnContext->getApnType()).c_str());

                apnContext->setSate(RIL_Data_Call_PdnState::PDN_IDLE);
                if(apnContext->getRefCount() == 1) {
                    apnContext->decRefCount();
                }
                notifyMbrainDataPoint(profileId, Data_Call_No_Valid_APN_Setting);
                DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
                return;
            } else {
                apnContext->setWaitingApns(waitingApns);
            }
        }

        if((apnContext->getState() != RIL_Data_Call_PdnState::PDN_CONNECTED)
            && apnContext->getState() != RIL_Data_Call_PdnState::PDN_CONNECTING)
        {

            DcTracker::setupData(apnType, request);
        }
        else
        {
            RFX_LOG_W(RFX_LOG_TAG, "trySetupData with apnType: %s ,the PDN was connecting or connected, not setup again!!! ",
                         (apnContext->getApnType()).c_str());
            if(apnContext->getRefCount() == 1) {
                apnContext->decRefCount();
            }
            DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_ABORTED, request);
        }
        return;
    }
    else {
        string str;
        if (!apnContext->isEnabled()) {
            str.append("isConnectable = false. ");
        }
        if (!isDataAllowed) {
            str.append("data not allowed: " + dataAllowFailReason.getDataAllowFailReason() + ". ");
        }
        RFX_LOG_W(RFX_LOG_TAG, "trySetupData with error: %s", str.c_str());
        if(apnContext->getState() != RIL_Data_Call_PdnState::PDN_DISCONNECTING
            && apnContext->getState() != RIL_Data_Call_PdnState::PDN_CONNECTING)
        {
            // no need retry in teleservice, app will retry.
            apnContext->setSate(RIL_Data_Call_PdnState::PDN_DISCONNECTED);
        }

        if(apnContext->getRefCount() == 1) {
            apnContext->decRefCount();
        }
        notifyMbrainDataPoint(profileId, Data_Call_PsDisabled_OR_DataNotAllowed);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return;
    }
}

bool DcTracker::setupData(string requestApn, const sp<RfxMessage>& request) {
    DataConnection *dataConnection = NULL;

    ApnContext *apnContext = DcTracker::mApnContexts.find(requestApn)->second;

    ApnSetting apnSetting = apnContext->getNextApnSetting();
    RFX_LOG_D(RFX_LOG_TAG, "setupData apnSetting with apn setting: %s", (apnSetting.logInfo()).c_str());

    int profileId = RpDataUtils::type2profileId(requestApn);
    if (!apnSetting.isValid()) {
        apnContext->setSate(RIL_Data_Call_PdnState::PDN_IDLE);
        //to be consider if is here exsit
        if(apnContext->getRefCount() == 1) {
            apnContext->decRefCount();
        }
        RFX_LOG_W(RFX_LOG_TAG, "setupData requestApn %s with apn setting: %s, should not be here", requestApn.c_str(), (apnSetting.logInfo()).c_str());
        notifyMbrainDataPoint(profileId, Data_Call_No_Valid_APN_Setting);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
        return false;
    }
    ApnContext *comApnContext = DcTracker::findCompatibleConnectedApnContext(requestApn);
    if(NULL !=comApnContext)
    {
        dataConnection = comApnContext->getDataConnection();
        if (dataConnection != NULL) {
            ApnSetting conApnSetting = dataConnection->getApnSetting();

            RFX_LOG_D(RFX_LOG_TAG, "setupData apnSetting get compatible apnSetting= %s, is_valid=%d",
                    (conApnSetting.logInfo()).c_str(),conApnSetting.isValid());

            if (conApnSetting.isValid())
            {
                apnSetting = conApnSetting;
            }
            else
            {
                dataConnection = NULL;
            }

        }
    }
    if(dataConnection != NULL)
    {
        RFX_LOG_W(RFX_LOG_TAG, "setupData apnSetting with apn setting: %s, reuse the compatible connection.",
                (apnSetting.logInfo()).c_str());

        apnContext->setDataConnection(dataConnection);
        apnContext->setApnSetting(apnSetting);

        apnContext->setSate(comApnContext->getState());
        dataConnection->addCompatibleApnContext(apnContext);
        DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_ABORTED, request);
        return true;
    }
    else
    {
        DataConnection *freeDataConnection = DcTracker::findFreeDataConnection(requestApn);
        int cId = -1;
        if (freeDataConnection != NULL) {

            ApnSetting freeApnSetting = freeDataConnection->getApnSetting();
            if (freeApnSetting.isValid()) {

                RFX_LOG_D(RFX_LOG_TAG, "setupData with apn setting: %s, reuse the free connection.",
                        (freeApnSetting.logInfo()).c_str());

                apnSetting = freeApnSetting;
            }
            cId = freeDataConnection->getcId();
        }

        RFX_LOG_I(RFX_LOG_TAG, "setupData with apn setting: %s , create new data connection.",
                (apnSetting.logInfo()).c_str());

        apnContext->setApnSetting(apnSetting);
        dataConnection = DcTracker::createDataConnection(requestApn, cId);
        if (dataConnection == NULL) {
            RFX_LOG_W(RFX_LOG_TAG, "setupData, get null data connection");
            apnContext->setSate(RIL_Data_Call_PdnState::PDN_IDLE);
            //to be consider if is here exsit
            if(apnContext->getRefCount() == 1) {
                apnContext->decRefCount();
            }
            RFX_LOG_W(RFX_LOG_TAG, "setupData requestApn %s with apn setting: %s, should not be here", requestApn.c_str(), (apnSetting.logInfo()).c_str());
            notifyMbrainDataPoint(profileId, Data_Call_Get_Null_Connection);
            DcTracker::mRpDataController->sendReponseToClient(RIL_Errno::RIL_E_CANCELLED, request);
            return false;
        }

        //before setting up the new data connection, we should delete the old data connection
        //to prevent memory leak;
        DataConnection *oldConnection = apnContext->getDataConnection();
        if (oldConnection != NULL) {
            // remove the invalid data connection from active list
            DcTracker::mDcController.removeActiveDcByCid(oldConnection);
            if(!DcTracker::findCompatibleConnectedApnContext(apnContext->getApnType()))
            {
                RFX_LOG_W(RFX_LOG_TAG, "setupData,should not be here free data connection with info: %s .",
                    (oldConnection->logInfo()).c_str());
                if (oldConnection->getDataCallResponse().netId != INVALID_NET_ID
                    && apnContext->getApnType() != TEXT_APN_TYPE_DEFAULT)
                {
                    RFX_LOG_D(RFX_LOG_TAG, "setupData this is the only connected apn type: %s,"
                            "teardown network ", apnContext->getApnType().c_str());
                        RpDataUtils::tearDownNetwork(oldConnection->getDataCallResponse().netId,
                                oldConnection->getDataCallResponse().ifname);
                }
                delete oldConnection;
            }

        }
        dataConnection->addCompatibleApnContext(apnContext);
        dataConnection->setApnSetting(apnSetting);
        apnContext->setDataConnection(dataConnection);
        apnContext->setSate(RIL_Data_Call_PdnState::PDN_CONNECTING);

        // recored the set up data request.
        DcTracker::mPdnManager.addSetupRequest(request);
        mRpDataController->isRequestToModem = true;
        dataConnection->connect(requestApn, request);
        return true;
    }
}

void DcTracker::setApnStateByApnType(string apnType, RIL_Data_Call_PdnState state) {
    RFX_LOG_D(RFX_LOG_TAG, "setApnStateByApnType(%d) with apnType: %s", (int) state, apnType.c_str());

    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;
    if(apnContext->getState() != state) {
        apnContext->setSate(state);
    }
}

ApnContext *DcTracker::findCompatibleConnectedApnContext(string apnType) {
    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;

    RFX_LOG_D(RFX_LOG_TAG, "findCompatibleConnectedApnContext with apnType: %s", apnType.c_str());

    map<string, ApnContext*>::iterator itor = DcTracker::mApnContexts.begin();
    while (itor != DcTracker::mApnContexts.end()) {

        ApnContext *curApnContext = itor->second;
        ApnSetting apnSetting = curApnContext->getApnSetting();
        if (apnSetting.canHandleType(apnType))
        {
            RFX_LOG_D(RFX_LOG_TAG, "findCompatibleConnectedApnContext with apnSetting= %s",
                        (apnSetting.logInfo()).c_str());
            DataConnection *curDataConnection = curApnContext->getDataConnection();
            if (curDataConnection != NULL)
            {

                RFX_LOG_D(RFX_LOG_TAG, "findCompatibleConnectedApnContext, current apn setting can handle the request apn type.");

                RIL_Data_Call_PdnState pdnState = curApnContext->getState();
                switch (pdnState)
                {
                case RIL_Data_Call_PdnState::PDN_CONNECTED:
                case RIL_Data_Call_PdnState::PDN_CONNECTING:
                case RIL_Data_Call_PdnState::PDN_RETRYING:
                    RFX_LOG_D(RFX_LOG_TAG, "findCompatibleConnectedApnContext with apnType: %s, find potentialDataConnection: %s",
                                apnType.c_str(), (curDataConnection->logInfo()).c_str());

                    return curApnContext;
                default:
                    break;
                }
            }
        }
        itor++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "findCompatibleConnectedApnContext, not find compatible connection.");
    return NULL;
}

DataConnection *DcTracker::findFreeDataConnection(string apnType) {
    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;

    RFX_LOG_D(RFX_LOG_TAG, "findFreeDataConnection with apnType: %s", apnType.c_str());

    map<string, ApnContext*>::iterator itor = DcTracker::mApnContexts.begin();
    DataConnection *freeDataConnection = NULL;
    while (itor != DcTracker::mApnContexts.end())
    {
        ApnContext *curApnContext = itor->second;

        ApnSetting apnSetting = curApnContext->getApnSetting();

        if (apnSetting.canHandleType(apnType))
        {
            RFX_LOG_D(RFX_LOG_TAG, "findFreeDataConnection with apnSetting= %s", (apnSetting.logInfo()).c_str());
            DataConnection *curDataConnection = curApnContext->getDataConnection();
            if (curDataConnection != NULL)
            {

                RFX_LOG_D(RFX_LOG_TAG, "findFreeDataConnection, current apn setting can handle the request apn type.");

                RIL_Data_Call_PdnState pdnState = curApnContext->getState();
                switch (pdnState) {
                case RIL_Data_Call_PdnState::PDN_DISCONNECTED:
                case RIL_Data_Call_PdnState::PDN_SCANNING:

                    freeDataConnection = curDataConnection;

                    RFX_LOG_I(RFX_LOG_TAG, "findFreeDataConnection with apnType: %s, find free data connection: %s",
                            apnType.c_str(), (freeDataConnection->logInfo()).c_str());

                    return freeDataConnection;
                default:
                    break;
                }
            }
        }
        itor++;
    }

    RFX_LOG_D(RFX_LOG_TAG, "findFreeDataConnection, not find free data connection.");
    return NULL;
}

vector<ApnSetting> DcTracker::buildWaitingApns(string apnType) {
    vector<ApnSetting> apnList;
    if (DcTracker::mAllApnSettings.size() > 0) {
        vector<ApnSetting>::iterator itor = DcTracker::mAllApnSettings.begin();
        while (itor != DcTracker::mAllApnSettings.end()) {
            if ((*itor).canHandleType(apnType)) {
                RFX_LOG_I(RFX_LOG_TAG, "buildWaitingApns with apnType: %s : %s",
                        apnType.c_str(), ((*itor).logInfo()).c_str());

                apnList.push_back(*itor);
            }

            itor++;
        }
    } else {
        RFX_LOG_W(RFX_LOG_TAG, "buildWaitingApns with empty apn infos!!! ");
    }
    return apnList;
}

void DcTracker::setupDataOnConnectableApns(string reason) {
    RFX_LOG_D(RFX_LOG_TAG, "setupDataOnConnectableApns, with reason: %s canceled", reason.c_str());
#if 1 // disable data connection retry
    return;
#else
    DcTracker::mPrioritySortedApnContexts.sort();

    DcTracker::dumpEnableApnList(DcTracker::mPrioritySortedApnContexts);

    list<ApnContext*>::iterator itor = DcTracker::mPrioritySortedApnContexts.begin();
    while (itor != DcTracker::mPrioritySortedApnContexts.end()) {

        if ((*itor)->isEnabled()) {
            string apnType = (*itor)->getApnType();
            ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;
            if(apnContext->getState() == PDN_RETRYING)
            {
                apnContext->setSate(RIL_Data_Call_PdnState::PDN_SCANNING);
            }
            else if((apnContext->getState() == PDN_CONNECTING)
                ||(apnContext->getState() == PDN_CONNECTED))
            {
                itor++;
                continue;
            }
            sp<RfxMessage> setupDataMessage = RfxMessage::obtainRequest(
                                                               mRpDataController->getSlotId(),
                                                               RADIO_TECH_GROUP_GSM,
                                                               RIL_REQUEST_SETUP_DATA_CALL);
            Parcel* parcel = setupDataMessage->getParcel();
            // make sure get the right position
            parcel->setDataPosition(sizeof(int32_t)*2);
            RpDataUtils::writeStringToParcel(parcel, apnType.c_str());

            DcTracker::trySetupData(apnType, setupDataMessage, reason);

        }
        itor++;
    }
#endif
}

bool DcTracker::isDataAllowed(DataAllowFailReason &dataAllowFailReason) {
    bool attachedState = DcTracker::mRpDataController->isAttached();
    bool isApnReady = DcTracker::mIsApnReady;
    bool isAllCallIdle = DcTracker::mRpDataController->isAllCallingStateIdle();
    bool isDataConcurent = DcTracker::mRpDataController->isDataSupportConcurrent();
    bool isPsRestricted = DcTracker::mRpDataController->isPsRestricted();

    dataAllowFailReason.clearAllReasons();
    if (attachedState == false) {
        dataAllowFailReason.addDataAllowFailReason(NOT_ATTACHED);
    }
    if (isApnReady == false) {
        dataAllowFailReason.addDataAllowFailReason(APN_NOT_READY);
    }
    if (isAllCallIdle == false && isDataConcurent == false) {
        dataAllowFailReason.addDataAllowFailReason(INVALID_PHONE_STATE);
        dataAllowFailReason.addDataAllowFailReason(
                CONCURRENT_VOICE_DATA_NOT_ALLOWED);
    }
    if (isPsRestricted == true) {
        dataAllowFailReason.addDataAllowFailReason(PS_RESTRICTED);
    }
    return !dataAllowFailReason.isFailed();
}

DataConnection *DcTracker::createDataConnection(string apnType, int defaultId) {
    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;

    RFX_LOG_D(RFX_LOG_TAG, "createDataConnection with apn type: %s, with default Id: %d",
            apnContext->getApnType().c_str(), defaultId);

    int id;
    if (defaultId != DC_INVALID_ID) {
        id = defaultId;
    } else {
        id = DcTracker::generationConnectionId(apnType);
    }
    if (id == DC_INVALID_ID) {
        RFX_LOG_W(RFX_LOG_TAG, "createDataConnection with with invalid cid, got error!!!");

        return NULL;
    }

    DataConnection *dc = new DataConnection(id, DcTracker::mRpDataController);
    dc->setApnSetting(apnContext->getApnSetting());

    RFX_LOG_D(RFX_LOG_TAG, "createDataConnection with apn type: %s, dataConnection: %s",
            apnContext->getApnType().c_str(), dc->logInfo().c_str());

    return dc;
}


int DcTracker::generationConnectionId(string apnType) {
    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;

    int id = DC_INVALID_ID;
    int index = 0;
    map<int, DataConnection*> dcListActiveByCid = DcTracker::mDcController.getActiveDataConnections();

//    for(index = MIN_ID_OTHERS_TROUGHPUT; index <= MAX_ID_OTHERS_TROUGHPUT;index++) {
//        if (dcListActiveByCid.find(index) == dcListActiveByCid.end()) {
//            id = index;
//            break;
//        }
//    }
    vector<int> all_interface = {1,2,3,7,0};
    for(auto &index : all_interface )
    {
        if (dcListActiveByCid.find(index) == dcListActiveByCid.end()) {
            id = index;
            break;
        }
    }
    for (index = 0; index < HIGH_THROUGHPUT_APN_NUM; index++) {
        RFX_LOG_D(RFX_LOG_TAG, "DcTracker::generationConnectionId with apnSetting= %s, id= %d",
                (apnContext->getApnSetting().logInfo()).c_str(), id);
        if (apnContext->getApnSetting().canHandleType(HIGH_THROUGHPUT_APN[index])) {
            id = 0;
            break;
        }
    }
    RFX_LOG_D(RFX_LOG_TAG, "DcTracker::generationConnectionId with apn type: %s, with Id: %d",
            apnContext->getApnType().c_str(), id);

    return id;
}
void DcTracker::onSetupConnectionCompleted(const sp<RfxMessage>& response) {

    int token = response->getToken();
    RFX_LOG_D(RFX_LOG_TAG, "onSetupConnectionCompleted, data call response with token: %d", token);

    PdnInfo pdnInfo = DcTracker::mPdnManager.getSetupPdnInfoByToken(token);

    // remove the record.
    DcTracker::mPdnManager.removeSetupRequestByToken(token);

    // get the requested apn
    string apnType = pdnInfo.apnType;
    if (apnType.empty()) {
        RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, with empty apn info. should not be here!!!");

        return;
    }

    RFX_LOG_D(RFX_LOG_TAG, "onSetupConnectionCompleted, with apn type: %s", apnType.c_str());

    ApnContext *apnContext;
    if (DcTracker::mApnContexts.find(apnType) != DcTracker::mApnContexts.end()) {
        apnContext = DcTracker::mApnContexts.find(apnType)->second;
    } else {
        RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, not find valid apnContext, should not be here!!!");
        return;
    }
    sp<RfxMessage> requestMessage = pdnInfo.request;
    if (requestMessage.get() == 0) {
        RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, should not be here,not find valid set up data request!!!");

        return;
    }

    if (response->getError() != RIL_Errno::RIL_E_SUCCESS) {
        RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, but with error info,reply this request,clear this request!!!");
        DataConnection *dataConnection = apnContext->getDataConnection();
        if(NULL!=dataConnection)
        {
            DataCallResponse dataCallResponse = dataConnection->getDataCallResponse();
            dataCallResponse.status = PDP_FAIL_ERROR_UNSPECIFIED;
            dataConnection->setDataCallResponse(dataCallResponse);
            apnContext->setSate(RIL_Data_Call_PdnState::PDN_FAILED);
            dataConnection->updateCompatibleApnContexts(apnContext);
            sp<RfxMessage> getLastDataCallFailCauseMessage = RfxMessage::obtainRequest(
                                                        mRpDataController->getSlotId(),
                                                               RADIO_TECH_GROUP_GSM,
                                                               RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE);

            // if setup data fail, we remove the data connection from the active list.

            DcTracker::mPdnManager.addGetFailCauseRequest(getLastDataCallFailCauseMessage->getToken(), pdnInfo);
            DcTracker::mRpDataController->sendToMD(getLastDataCallFailCauseMessage);
        }
        return;
    }

    Parcel* responseParcel = response->getParcel();

    int ver = responseParcel->readInt32();
    int num = responseParcel->readInt32();

    RFX_LOG_D(RFX_LOG_TAG , "onSetupConnectionCompleted, ver: %d, num: %d", ver, num);

    int status = responseParcel->readInt32();  // status
    int suggestedRetryTime = responseParcel->readInt32();  // suggestedRetryTime
    int cid = -1;
    int active = 0;
    string protocolType = "";
    string ifname = "";
    string addresses = "";
    string dnses = "";
    string gateways = "";
    string pcscf = "";
    int mtu = -1;
    int mtuV6 = -1;
    int netId = -1;
    if (status == 0)
    {
        cid = responseParcel->readInt32();  // cid(interfaeId in rilj)
        active = responseParcel->readInt32();  // active

        char * cp_protocolType = RpDataUtils::strdupReadString(responseParcel);
        if (cp_protocolType != NULL) {
            protocolType = std::string(cp_protocolType);
        }


        char * cp_ifname = RpDataUtils::strdupReadString(responseParcel);
        if (cp_ifname != NULL) {
            ifname = std::string(cp_ifname);
        }
        RpDataUtils::freeMemory(cp_ifname);

        char * cp_addresses = RpDataUtils::strdupReadString(responseParcel);
        if (cp_addresses != NULL) {
            addresses = std::string(cp_addresses);
        }
        RpDataUtils::freeMemory(cp_addresses);

        char * cp_dnses = RpDataUtils::strdupReadString(responseParcel);
        if (cp_dnses != NULL && strlen(cp_dnses) != 0) {
            dnses = std::string(cp_dnses);

            RFX_LOG_D(RFX_LOG_TAG, "onSetupConnectionCompleted, get valid DNS server: %s", dnses.c_str());
        }
        else if(cp_protocolType)
        {
            RFX_LOG_D(RFX_LOG_TAG, "onSetupConnectionCompleted, get NULL DNS server, so use the default DNS server.");

            char buf[100]={0};

            if(strcasecmp(cp_protocolType,"IPV6"))
            {
                property_get("persist.default.ipv6dns.server", buf, "2001:4860:4860::8888");
            }
            if(strcasecmp(cp_protocolType,"IPV4V6"))
            {
                char tmp_ipv4_dns_buf[PROPERTY_VALUE_MAX]={0};
                char tmp_ipv6_dns_buf[PROPERTY_VALUE_MAX]={0};
                int len;
                property_get("persist.default.dns.server", tmp_ipv4_dns_buf, "8.8.8.8");
                property_get("persist.default.ipv6dns.server", tmp_ipv6_dns_buf, "2001:4860:4860::8888");
                len = snprintf(buf, sizeof(buf) - 1, "%s %s", tmp_ipv4_dns_buf, tmp_ipv6_dns_buf);
                if(len < 0) {
                    RFX_LOG_W(RFX_LOG_TAG,"snprintf return error!!!");
                }
            }
            else
            {
                property_get("persist.default.dns.server", buf, "8.8.8.8");
            }

            RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, get NULL DNS server, so use the default DNS server %s.", buf);
            dnses = std::string(buf);
        }
        RpDataUtils::freeMemory(cp_dnses);
        RpDataUtils::freeMemory(cp_protocolType);

        char * cp_gateways = RpDataUtils::strdupReadString(responseParcel);
        if (cp_gateways != NULL) {
            gateways = std::string(cp_gateways);
        }
        RpDataUtils::freeMemory(cp_gateways);

        char * cp_pcscf = RpDataUtils::strdupReadString(responseParcel);
        if (cp_pcscf != NULL) {
            pcscf = std::string(cp_pcscf);
        }
        RpDataUtils::freeMemory(cp_pcscf);

        mtu = responseParcel->readInt32(); // mtu
        mtuV6 = responseParcel->readInt32(); // mtuV6

        netId = RpDataUtils::generateNetId();
    }
    RpDataUtils::saveCidByType(cid, apnType);
    DataCallResponse dataCallResponse(status, cid, active, protocolType, ifname, addresses,
                dnses, gateways, suggestedRetryTime, pcscf, mtu, mtuV6, netId);

    RFX_LOG_D(RFX_LOG_TAG, "onSetupConnectionCompleted, data call response: %s", dataCallResponse.logInfo().c_str());

    DataConnection *dataConnection = apnContext->getDataConnection();
    if (dataConnection != NULL) {
        dataConnection->setDataCallResponse(dataCallResponse);
        if (status != 0) {

            long delay = DcTracker::getModemSuggestedRetryDelay(dataCallResponse);
            RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, with fail cause: %d, delay=%d", status,delay);
            apnContext->setModemSuggestedDelay(delay);

            DcFail dcFail(dataCallResponse.status);
            dataConnection->setDcFail(dcFail);
            dataConnection->clearApnSetting();

            if (DcTracker::isPermanentFail(dcFail)) {
                ApnSetting apnSetting = apnContext->getApnSetting();

                RFX_LOG_D(RFX_LOG_TAG, "onSetupConnectionCompleted, mark permanent failed with apnSetting= %s",
                                apnSetting.logInfo().c_str());

                apnContext->markApnPermanentFailed(apnSetting);
            }

            RFX_LOG_D(RFX_LOG_TAG, "onSetupConnectionCompleted, clear up the pdn connection with apn type: %s",
                            apnContext->getApnType().c_str());
            apnContext->setSate(RIL_Data_Call_PdnState::PDN_FAILED);
            //apnContext->getApnSetting().clear();
            dataConnection->updateCompatibleApnContexts(apnContext);

            set<ApnContext*> compatibleApnContexts = dataConnection->getCompatibleApnContext();
            set<ApnContext*>::iterator comApnContextItor = compatibleApnContexts.begin();
            while (comApnContextItor != compatibleApnContexts.end())
            {
                RFX_LOG_W(RFX_LOG_TAG, "DcTracker::onSetupConnectionCompleted, the pdn with apn type: %s is failed",
                                (*comApnContextItor)->getApnType().c_str());

                // now the pdn is disconnected, need bring it up later.
                (*comApnContextItor)->setDataConnection(NULL);
                comApnContextItor++;

            }

#if 0 // don't retry setup data as APP will do the retry
            comApnContextItor = compatibleApnContexts.begin();
            while (comApnContextItor!=compatibleApnContexts.end()) {

                RFX_LOG_D(RFX_LOG_TAG, "DcTracker::onSetupConnectionCompleted, retry pdn with apn type: %s",
                        (*comApnContextItor)->getApnType().c_str());

                DcTracker::reTrySetupData((*comApnContextItor)->getApnType(), std::string("retry"));
                comApnContextItor++;
            }
#else
            if(apnContext->getRefCount() == 1) {
                apnContext->decRefCount();
            }
#endif
            //clear up the apn context which is binded with the data connection.
            compatibleApnContexts.clear();
            delete dataConnection;
        } else {

            // no need config the network for DEFAULT pdn.
            if (apnContext->getApnType()!= TEXT_APN_TYPE_DEFAULT) {
                RpDataUtils::configNetwork(dataCallResponse.netId, dataCallResponse.ifname,
                    dataCallResponse.addresses, dataCallResponse.dnses);
            }
            apnContext->setSate(RIL_Data_Call_PdnState::PDN_CONNECTED);
            dataConnection->updateCompatibleApnContexts(apnContext);
            dataConnection->setcId(dataCallResponse.cid);
            DcTracker::mDcController.addActiveDcByCid(dataConnection);

            RFX_LOG_I(RFX_LOG_TAG, "onSetupConnectionCompleted, set PDN done, with apn type: %s",
                                   apnType.c_str());
        }
    } else {
        RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, with NULL data connection,should not be here! with apn type: %s",
                        apnType.c_str());
    }
}

long DcTracker::getModemSuggestedRetryDelay(DataCallResponse dataCallResponse) {
    long delay;
    if (dataCallResponse.suggestedRetryTime < 0) {
        delay = NO_SUGGESTED_RETRY_DELAY;
    } else if (dataCallResponse.suggestedRetryTime == INT_MAX) {
        delay = NO_RETRY;
    } else {
        delay = dataCallResponse.suggestedRetryTime;
    }

    RFX_LOG_D(RFX_LOG_TAG, "getModemSuggestedRetryDelay, with delay: %ld", delay);
    return (long) delay;
}

void DcTracker::reTrySetupData(string apnType, string reason) {
    ApnContext *apnContext = DcTracker::mApnContexts.find(apnType)->second;
    DataConnection *dataConnection = apnContext->getDataConnection();

    RFX_LOG_D(RFX_LOG_TAG, "reTrySetupData, with apnContext: %s, with reason: %s",
            apnContext->logInfo().c_str(), reason.c_str());

    DataAllowFailReason dataAllowFailReason;
    bool isDataAllowed = DcTracker::isDataAllowed(dataAllowFailReason);

    RFX_LOG_I(RFX_LOG_TAG, "reTrySetupData with apnType: %s ,with dataAllowed result:%s ",
                             (apnContext->getApnType()).c_str(), RpDataUtils::b2s(isDataAllowed).c_str());

    if (apnContext->isEnabled() && isDataAllowed) {
        long delay = apnContext->getDelayForNextApn();
        //always retry,
        //long delay = 0;
        if (delay >= 0) {
            apnContext->setSate(RIL_Data_Call_PdnState::PDN_RETRYING);
            if (dataConnection != NULL) {
                dataConnection->updateCompatibleApnContexts(apnContext);

                if (!DcTracker::hasCompatibleConnectedApnContext(apnType)) {

                    RFX_LOG_D(RFX_LOG_TAG, "reTrySetupData, free data connection with info: %s .",
                            (dataConnection->logInfo()).c_str());

                    // remove the invalid data connection from active list
                    DcTracker::mDcController.removeActiveDcByCid(dataConnection);
                    // no need to teardown the network of default pdn.
                    DataCallResponse dcResponse = dataConnection->getDataCallResponse();
                    if (dcResponse.netId != -1)
                    {
                       RFX_LOG_D(RFX_LOG_TAG, "cleanUpConnection, free data connection with info: %s, netid %d, ifname = %s .",
                           (dataConnection->logInfo()).c_str(), dcResponse.netId, dcResponse.ifname.c_str());
                       RpDataUtils::tearDownNetwork(dcResponse.netId, dcResponse.ifname);
                    }

                    delete dataConnection;
                }
            }

            RFX_LOG_D(RFX_LOG_TAG, "reTrySetupData, delay: %ld ms", delay);

            //the old connection is invalid.
            apnContext->setDataConnection(NULL);
            //the current apn setting should be invalid.
            apnContext->getApnSetting().clear();

            sp<RfxMessage> setupDataMessage = RfxMessage::obtainRequest(
                                                               mRpDataController->getSlotId(),
                                                               RADIO_TECH_GROUP_GSM,
                                                               RIL_REQUEST_SETUP_DATA_CALL);
            Parcel* parcel = setupDataMessage->getParcel();
            // make sure get the right position
            parcel->setDataPosition(sizeof(int32_t)*2);
            RpDataUtils::writeStringToParcel(parcel, apnType.c_str());

//            RFX_LOG_D(RFX_LOG_TAG, "reTrySetupData, retry with apnType: %s, request: RIL_REQUEST_SETUP_DATA_CALL, delay time: %ld",
//                apnType.c_str(),
//                delay);
//            DcTracker::mRpDataController->retryRequest(apnType, setupDataMessage, reason, delay);
            //should immediately build PDN.
            RFX_LOG_D(RFX_LOG_TAG, "DcTracker::reTrySetupData, force immediate PDN creation, don't delay");
            DcTracker::trySetupData(apnType, setupDataMessage, reason);
            return;
        } else {
            while(apnContext->getRefCount() > 0) {
                apnContext->decRefCount();
            }
            apnContext->setEnabled(false);
        }
    } else {
        string str;
        if (!apnContext->isEnabled()) {
            str.append("isConnectable = false. ");
        }
        if (!isDataAllowed) {
            str.append("data not allowed: " + dataAllowFailReason.getDataAllowFailReason() + ". ");
        }
        RFX_LOG_W(RFX_LOG_TAG, "reTrySetupData with error: %s", str.c_str());

        apnContext->setSate(RIL_Data_Call_PdnState::PDN_SCANNING);
    }
}

bool DcTracker::isPermanentFail(DcFail dcFail) {
    bool isPermanentFail = dcFail.isPermanentFail()
            && (!DcTracker::mAttached || dcFail.mDcFailCause != RIL_DataCallFailCause::PDP_FAIL_SIGNAL_LOST);

    RFX_LOG_D(RFX_LOG_TAG, "isPermanentFail, with isPermanentFail: %d", isPermanentFail);

    return isPermanentFail;
}

void DcTracker::onDataCallListChanged(const sp<RfxMessage>& message) {
    DcTracker::mDcController.onDataStateChanged(message);
}

void DcTracker::setRpDataController(RpDataController* rpDataController) {
    DcTracker::mRpDataController = rpDataController;
    DcTracker::m_slot_id=rpDataController->getSlotId();

    list<ApnContext*>::iterator itor =
            DcTracker::mPrioritySortedApnContexts.begin();
    while (itor != DcTracker::mPrioritySortedApnContexts.end()) {
        (*itor)->setRpDataController(rpDataController);

        itor++;
    }
}

bool DcTracker::isValidRequest(string apnType) {
    bool result = false;
    for (int i = 0; i < SUPPORTED_APN_NUM; i++) {
        RFX_LOG_D(RFX_LOG_TAG, "isValidRequest, with apnType: %s, with check result: %s", apnType.c_str(),
            (SUPPORTED_APN[i]).c_str());
        if (apnType == SUPPORTED_APN[i]) {
            result = true;
            break;
        }
    }
    RFX_LOG_D(RFX_LOG_TAG, "isValidRequest, with apnType: %s, with check result: %s", apnType.c_str(), RpDataUtils::b2s(result).c_str());

    return result;
}

void DcTracker::onGetDataCallFailCauseCompleted(const sp<RfxMessage>& response) {
    int token = response->getToken();

    Parcel* responseParcel = response->getParcel();
    int num = responseParcel->readInt32();
    int failCause = responseParcel->readInt32();

    RFX_LOG_D(RFX_LOG_TAG, "onGetDataCallFailCauseCompleted, with fail cause: %d, with token: %d", failCause, token);

    PdnInfo pdnInfo= DcTracker::mPdnManager.getRetryPdnInfoByToken(token);
    // get the requested apn
    string apnType = pdnInfo.apnType;
    if (apnType.empty()) {
        RFX_LOG_W(RFX_LOG_TAG, "onGetDataCallFailCauseCompleted, with invalid apn type");
        return;
    }

    // remove the GetFailCause request record.
    DcTracker::mPdnManager.removeGetFailCauseRequestByToken(token);

    ApnContext *apnContext = NULL;
    if (DcTracker::mApnContexts.find(apnType)!= DcTracker::mApnContexts.end()) {
        apnContext = DcTracker::mApnContexts.find(apnType)->second;
    } else {
        RFX_LOG_W(RFX_LOG_TAG, "onSetupConnectionCompleted, not find valid apnContext, should never be here!!!");
        return;
    }

    DataConnection *dataConnection = apnContext->getDataConnection();
    if (dataConnection != NULL) {

        DcFail dcFail(failCause);
        dataConnection->setDcFail(dcFail);

        if (DcTracker::isPermanentFail(dcFail)) {
            ApnSetting apnSetting = apnContext->getApnSetting();

            RFX_LOG_D(RFX_LOG_TAG, "onGetDataCallFailCauseCompleted, mark permanent failed with apnSetting= %s",
                    apnSetting.logInfo().c_str());
            apnContext->markApnPermanentFailed(apnSetting);
        }

        set<ApnContext*> compatibleApnContexts = dataConnection->getCompatibleApnContext();
        set<ApnContext*>::iterator comApnContextItor = compatibleApnContexts.begin();
        while (comApnContextItor != compatibleApnContexts.end())
        {
            RFX_LOG_D(RFX_LOG_TAG, "DcTracker::onGetDataCallFailCauseCompleted, the pdn with apn type: %s is failed",
                            (*comApnContextItor)->getApnType().c_str());

            // now the pdn is disconnected, need bring it up later.
            (*comApnContextItor)->setDataConnection(NULL);
            comApnContextItor++;

        }

#if 0
        comApnContextItor = compatibleApnContexts.begin();
        while (comApnContextItor!=compatibleApnContexts.end()) {

            RFX_LOG_D(RFX_LOG_TAG, "DcTracker::onGetDataCallFailCauseCompleted, retry pdn with apn type: %s",
                    (*comApnContextItor)->getApnType().c_str());

            DcTracker::reTrySetupData((*comApnContextItor)->getApnType(), std::string("retry"));
            comApnContextItor++;
        }
#endif
        //clear up the apn context which is binded with the data connection.
        compatibleApnContexts.clear();
        delete dataConnection;
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "onGetDataCallFailCauseCompleted, with NULL data connection, with apn type: %s",
                apnType.c_str());
    }
}

bool DcTracker::isLocalSetupPdnRequest(const sp<RfxMessage>& request) {
    bool result = true;
    int token = request->getToken();
    PdnInfo pdnInfo = DcTracker::mPdnManager.getSetupPdnInfoByToken(token);
    sp<RfxMessage> requestMessage = pdnInfo.request;
    if (requestMessage.get() == 0) {
        result = false;
    }

    RFX_LOG_D(RFX_LOG_TAG, "isLocalSetupPdnRequest, with result: %s", RpDataUtils::b2s(result).c_str());

    return result;
}

bool DcTracker::isLocalCallFailPdnrequest(const sp<RfxMessage>& request) {
    bool result = true;
    int token = request->getToken();
    PdnInfo pdnInfo = DcTracker::mPdnManager.getRetryPdnInfoByToken(token);

    if ((pdnInfo.token == -1) && (pdnInfo.apnType.empty())) {
        result = false;
    }

    RFX_LOG_D(RFX_LOG_TAG, "isLocalSetupPdnRequest, with result: %s", RpDataUtils::b2s(result).c_str());

    return result;
}

void DcTracker::dumpEnableApnList(list<ApnContext*> prioritySortedApnContexts) {
    list<ApnContext*>::iterator itor = prioritySortedApnContexts.begin();
    RFX_LOG_D(RFX_LOG_TAG, "dumpEnableApnList. begin---------begin.");

    while (itor != prioritySortedApnContexts.end()) {
        if ((*itor)->isEnabled()) {
            RFX_LOG_D(RFX_LOG_TAG, "Enabled APN type: %s .", (*itor)->getApnType().c_str());
        }

        itor++;
    }
    RFX_LOG_D(RFX_LOG_TAG, "dumpEnableApnList. end---------end.");
}
