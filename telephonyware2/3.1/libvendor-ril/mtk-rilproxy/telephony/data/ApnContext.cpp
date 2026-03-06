// SPDX-License-Identifier: MediaTekProprietary
/*
 * ApnContext.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: mtk54458
 */

#include "ApnContext.h"
#include "RpDataUtils.h"

#include "RpDataController.h"
#include "RpRilClientController.h"
#include <mutex>

#define RFX_LOG_TAG "ApnContext"

static std::mutex itor_mutex;

ApnContext::ApnContext(string apnType, NetworkConfig networkConfig) {
    ApnContext::mApnType = apnType;
    ApnContext::mDataEnalbed = false;
    ApnContext::mRefCount = 0;
    ApnContext::mRetryManger = RetryManager(apnType);
    ApnContext::mDependencyMet = networkConfig.mDependencyMet;
    ApnContext::mPriority = networkConfig.mPriority;
    ApnContext::mState = RIL_Data_Call_PdnState::PDN_IDLE;
    ApnContext::mDataConnection = NULL;
    ApnContext::mRpDataController = NULL;
}

ApnContext::ApnContext() {
    ApnContext::mDataEnalbed = false;
    ApnContext::mRefCount = 0;
    ApnContext::mState = RIL_Data_Call_PdnState::PDN_IDLE;
    ApnContext::mPriority = -1;
    ApnContext::mDependencyMet = false;
    ApnContext::mDataConnection = NULL;
    ApnContext::mRpDataController = NULL;
}

ApnContext::~ApnContext() {}

ApnSetting &ApnContext::getApnSetting() {

    return ApnContext::mApnSetting;
}

void ApnContext::setApnSetting(ApnSetting apnSetting) {
    RFX_LOG_I(RFX_LOG_TAG, "ApnContext(%s) ::setApnSetting: apnSetting= %s", mApnType.c_str(), apnSetting.logInfo().c_str());

    ApnContext::mApnSetting = apnSetting;
}

void ApnContext::setWaitingApns(vector<ApnSetting> waitingApns) {
    ApnContext::mRetryManger.setWaitingApns(waitingApns);
}

vector<ApnSetting> ApnContext::getWaitingApns() {
    return ApnContext::mRetryManger.getWaitingApns();

}

ApnSetting ApnContext::getNextApnSetting() {
    return ApnContext::mRetryManger.getNextApnSetting();
}

long ApnContext::getDelayForNextApn() {
    return ApnContext::mRetryManger.getDelayForNextApn();
}

void ApnContext::setEnabled(bool enabled) {
    RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s)::setEnabled as %s, and current state is %s",
            mApnType.c_str(), RpDataUtils::b2s(enabled).c_str(), RpDataUtils::b2s(mDataEnalbed).c_str());

    ApnContext::mDataEnalbed = enabled;
}

bool ApnContext::isEnabled() {
    return ApnContext::mDataEnalbed;
}

void ApnContext::incRefCount() {
    ApnContext::mRefCount++;

    RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::incRefCount mRefCount = %d", mApnType.c_str(), mRefCount);
}

void ApnContext::decRefCount() {
    ApnContext::mRefCount--;

    RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::decRefCount mRefCount = %d", mApnType.c_str(), mRefCount);
}

int ApnContext::getRefCount() {
    RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::getRefCount mRefCount = %d", mApnType.c_str(), mRefCount);
    return ApnContext::mRefCount;


}

RIL_Data_Call_PdnState ApnContext::getState() {
    RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::getState mState = %s", mApnType.c_str(), RpDataUtils::apnState2string(mState));

    return ApnContext::mState;
}

void ApnContext::setSate(RIL_Data_Call_PdnState newState) {
    RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::setState with old state = %s, with new state: %s",
            mApnType.c_str(), RpDataUtils::apnState2string(ApnContext::mState), RpDataUtils::apnState2string(newState));

    if ((ApnContext::mState != newState)
        || (ApnContext::mClientSetupDataRequest.size() != 0)
        ||(ApnContext::mClientTearDownDataRequest.size() != 0)) {
        ApnContext::mState = newState;
        ApnContext::notifyStateChange(newState);
    }
}

string ApnContext::getApnType() {
    return ApnContext::mApnType;
}

DataConnection *ApnContext::getDataConnection() {
    return ApnContext::mDataConnection;
}

void ApnContext::setDataConnection(DataConnection *dataConnection) {
    if (dataConnection != NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::setDataConnection as %s", mApnType.c_str(), dataConnection->logInfo().c_str());
    }

    ApnContext::mDataConnection = dataConnection;
}

void ApnContext::setModemSuggestedDelay(long delay) {
    ApnContext::mRetryManger.setModemSuggestedDelay(delay);
}

long ApnContext::getModemSuggestedDelay() {
    return ApnContext::mRetryManger.getModemSuggestedDelay();
}

void ApnContext::markApnPermanentFailed(ApnSetting apnSetting) {
    ApnContext::mRetryManger.markApnPermanentFailed(apnSetting);
}

void ApnContext::setDependencyMet(bool met) {
    ApnContext::mDependencyMet = met;
}

bool ApnContext::getDependencyMet() {
    return ApnContext::mDependencyMet;
}

void ApnContext::setRpDataController(RpDataController* rpDataController) {
    ApnContext::mRpDataController = rpDataController;
}

void ApnContext::notifyStateChange(RIL_Data_Call_PdnState newState) {
    sp<RfxMessage> urc = RfxMessage::obtainUrc(
            ApnContext::mRpDataController->getSlotId(),
            RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED,
            0,
            false,
            ClientId::CLIENT_ID_SDK);

    DataCallResponse dcResponse;
    MTK_Data_Call_Response_v1 dataCallResponse;
    Parcel* urcParcel = NULL;
    Parcel* responseParcel = NULL;
    vector<sp<RfxMessage>>::iterator itor_request;

    if (ApnContext::mDataConnection != NULL)
    {
        dcResponse = ApnContext::mDataConnection->getDataCallResponse();

        dataCallResponse = {dcResponse.netId, newState, dcResponse.status, dcResponse.cid,
            (char *)ApnContext::mApnType.c_str(), (char *)dcResponse.type.c_str(), (char *)dcResponse.ifname.c_str(),
            (char *)dcResponse.addresses.c_str(), (char *)dcResponse.dnses.c_str(),
            (char *)dcResponse.gateways.c_str(), (char *)dcResponse.pcscf.c_str(), dcResponse.mtu};
    }
    else
    {
        dataCallResponse = {INVALID_NET_ID, newState, RIL_DataCallFailCause::PDP_FAIL_ERROR_UNSPECIFIED,
            INVALID_CID, (char *)ApnContext::mApnType.c_str(), "", "", "", "", "", "", INVALID_MTU};
    }

    RFX_LOG_I(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s",
            mApnType.c_str(), RpDataUtils::apnState2string(newState));

    switch (newState) {
    case RIL_Data_Call_PdnState::PDN_RETRYING:
    case RIL_Data_Call_PdnState::PDN_CONNECTING:
    case RIL_Data_Call_PdnState::PDN_SCANNING:
        urcParcel = urc->getParcel();
        urcParcel->setDataPosition(sizeof(int32_t) * 2);

        RpDataUtils::fillDataCallResponse(urcParcel, dataCallResponse);

        RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Urc: %s",
                   mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(urc->getId()));

        ApnContext::mRpDataController->notifyWorld(urc);
        break;

    case RIL_Data_Call_PdnState::PDN_DISCONNECTING:

        urcParcel = urc->getParcel();
        urcParcel->setDataPosition(sizeof(int32_t) * 2);

        RpDataUtils::fillDataCallResponse(urcParcel, dataCallResponse);

        RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Urc: %s",
                   mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(urc->getId()));

        ApnContext::mRpDataController->notifyWorld(urc);
        break;

    case RIL_Data_Call_PdnState::PDN_FAILED:
        itor_mutex.lock();
        itor_request = ApnContext::mClientSetupDataRequest.begin();
        while (itor_request != ApnContext::mClientSetupDataRequest.end()) {
            sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_Errno::RIL_E_SUCCESS, *itor_request);

            RFX_LOG_I(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with %X, token = %d",
                       mApnType.c_str(), itor_request, (*itor_request)->getToken());

            urc = RfxMessage::obtainUrc(
                                ApnContext::mRpDataController->getSlotId(),
                                RIL_UNSOL_SETUP_PDN_STATE_CHANGED,
                                0,
                                false,
                                ClientId::CLIENT_ID_SDK);

            responseParcel = response->getParcel();
            responseParcel->setDataPosition(sizeof(int32_t) * 3);

            RpDataUtils::fillDataCallResponse(responseParcel,dataCallResponse);

            RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Response: %s",
                       mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(response->getId()));

            ApnContext::mRpDataController->notifyWorld(response);
              // clear the request record.
            itor_request++;
        }
        itor_mutex.unlock();
        ApnContext::mClientSetupDataRequest.clear();
        {
            // notify urc
            urcParcel = urc->getParcel();
            urcParcel->setDataPosition(sizeof(int32_t) * 2);

            RpDataUtils::fillDataCallResponse(urcParcel,dataCallResponse);

            RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Urc: %s",
                       mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(urc->getId()));

            ApnContext::mRpDataController->notifyWorld(urc);
        }
        break;

    case RIL_Data_Call_PdnState::PDN_CONNECTED:
        itor_mutex.lock();
        itor_request = ApnContext::mClientSetupDataRequest.begin();
        while (itor_request != ApnContext::mClientSetupDataRequest.end()) {
            sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_Errno::RIL_E_SUCCESS, *itor_request);

            RFX_LOG_I(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with %X, token = %d",
                       mApnType.c_str(), itor_request, (*itor_request)->getToken());

            urc = RfxMessage::obtainUrc(
                                ApnContext::mRpDataController->getSlotId(),
                                RIL_UNSOL_SETUP_PDN_STATE_CHANGED,
                                0,
                                false,
                                ClientId::CLIENT_ID_SDK);

            responseParcel = response->getParcel();
            responseParcel->setDataPosition(sizeof(int32_t) * 3);

            RpDataUtils::fillDataCallResponse(responseParcel,dataCallResponse);

            RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Response: %s",
                       mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(response->getId()));

            // no need config the network for DEFAULT pdn.
            if (ApnContext::mApnType != TEXT_APN_TYPE_DEFAULT) {
                RpDataUtils::configNetwork(dcResponse.netId, dcResponse.ifname, dcResponse.gateways, dcResponse.dnses);
            }

            ApnContext::mRpDataController->notifyWorld(response);
             // clear the request record.
            itor_request++;
        }
        itor_mutex.unlock();
        ApnContext::mClientSetupDataRequest.clear();

        {
            // notify urc
            urcParcel = urc->getParcel();
            urcParcel->setDataPosition(sizeof(int32_t) * 2);

            RpDataUtils::fillDataCallResponse(urcParcel,dataCallResponse);

            RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Urc: %s",
                       mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(urc->getId()));

            // no need config the network for DEFAULT pdn.
            if (ApnContext::mApnType != TEXT_APN_TYPE_DEFAULT) {
                RpDataUtils::configNetwork(dcResponse.netId, dcResponse.ifname, dcResponse.gateways, dcResponse.dnses);
            }

            ApnContext::mRpDataController->notifyWorld(urc);
        }
        break;

    case RIL_Data_Call_PdnState::PDN_DISCONNECTED:
        if (ApnContext::mDataConnection != NULL)
        {
            dcResponse = ApnContext::mDataConnection->getDataCallResponse();

            dataCallResponse = {dcResponse.netId, newState, dcResponse.status, dcResponse.cid,
                (char *)ApnContext::mApnType.c_str(), (char *)dcResponse.type.c_str(), (char *)dcResponse.ifname.c_str(), (char *)dcResponse.addresses.c_str(), (char *)dcResponse.dnses.c_str(),
                (char *)dcResponse.gateways.c_str(), (char *)dcResponse.pcscf.c_str(), dcResponse.mtu};
        }
        else
        {
            dataCallResponse = {INVALID_NET_ID, newState, RIL_DataCallFailCause::PDP_FAIL_ERROR_UNSPECIFIED,
                INVALID_CID, (char *)ApnContext::mApnType.c_str(), "", "", "", "", "", "", INVALID_MTU};
        }

        itor_mutex.lock();
        itor_request = ApnContext::mClientTearDownDataRequest.begin();
        while (itor_request != ApnContext::mClientTearDownDataRequest.end()) {

            RFX_LOG_I(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with %X, token = %d",
                       mApnType.c_str(), itor_request, (*itor_request)->getToken());

            sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_Errno::RIL_E_SUCCESS, *itor_request);

            urc = RfxMessage::obtainUrc(
                                ApnContext::mRpDataController->getSlotId(),
                                RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED,
                                0,
                                false,
                                ClientId::CLIENT_ID_SDK);

            responseParcel = response->getParcel();
            responseParcel->setDataPosition(sizeof(int32_t) * 3);


            RpDataUtils::fillDataCallResponse(responseParcel, dataCallResponse);

            RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Response: %s",
                       mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(response->getId()));

            ApnContext::mRpDataController->notifyWorld(response);

            // clear the request record.
            itor_request++;
        }
        itor_mutex.unlock();

        ApnContext::mClientTearDownDataRequest.clear();
        {
            // notify urc
            urcParcel = urc->getParcel();
            urcParcel->setDataPosition(sizeof(int32_t) * 2);

            RpDataUtils::fillDataCallResponse(urcParcel, dataCallResponse);

            RFX_LOG_D(RFX_LOG_TAG, "ApnContext(%s) ::notifyStateChange with newState = %s, with Urc: %s",
                    mApnType.c_str(), RpDataUtils::apnState2string(newState), RpDataUtils::requestOrUrcToString(urc->getId()));

            ApnContext::mRpDataController->notifyWorld(urc);
        }
        break;

    case RIL_Data_Call_PdnState::PDN_IDLE:
    default:
        RFX_LOG_W(RFX_LOG_TAG, "ApnContext(%s)::notifyStateChange: unknown state.", mApnType.c_str());
        break;
    }
}

void ApnContext::setClientSetupDataRequest(const sp<RfxMessage>& setupDataRequest) {
    sp<RfxMessage> request=setupDataRequest;
    RFX_LOG_I(RFX_LOG_TAG, "ApnContext(%s)::setClientSetupDataRequest: %s", mApnType.c_str(),
            setupDataRequest != 0 ? RpDataUtils::requestOrUrcToString(setupDataRequest->getId()) : "NULL");

    ApnContext::mClientSetupDataRequest.push_back(request);
}

void ApnContext::setClientTearDownDataRequest(const sp<RfxMessage>& tearDownDataRequest) {
    sp<RfxMessage> request=tearDownDataRequest;
    RFX_LOG_I(RFX_LOG_TAG, "ApnContext(%s)::setClientTearDownDataRequest: %s", mApnType.c_str(),
            tearDownDataRequest != 0 ? RpDataUtils::requestOrUrcToString(tearDownDataRequest->getId()) : "NULL");

    ApnContext::mClientTearDownDataRequest.push_back(request);
}

string ApnContext::logInfo() {
    string apnContext("ApnContext(" + ApnContext::mApnType + "): ");

    apnContext += "current mApnSetting: " + mApnSetting.logInfo();
    apnContext += ", mApnType: " + mApnType;
    if (mDataEnalbed) {
        apnContext += ", mDataEnalbed: true";
    } else {
        apnContext += ", mDataEnalbed: false";
    }

    apnContext += ", mRefCount: " + RpDataUtils::i2s(mRefCount);
    apnContext += ", mPriority: " + RpDataUtils::i2s(mPriority);
    if (mDependencyMet) {
        apnContext += ", mDependencyMet: true";
    } else {
        apnContext += ", mDependencyMet: false";
    }

    apnContext += ", mRetryManger: " + mRetryManger.logInfo();
    apnContext += ", mState: " + std::string(RpDataUtils::apnState2string(mState));

    if (mDataConnection != NULL) {
        apnContext += ", mDataConnection: " + mDataConnection->logInfo();
    }

    return apnContext;
}
