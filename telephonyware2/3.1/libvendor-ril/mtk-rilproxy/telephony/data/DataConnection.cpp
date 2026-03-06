// SPDX-License-Identifier: MediaTekProprietary
/*
 * DataConnection.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: mtk54458
 */
#include "DataConnection.h"
#include "ApnContext.h"
#include "RfxLog.h"
#include "RpDataUtils.h"
#include "RpDataController.h"
#include "MbrainClient.h"

#define RFX_LOG_TAG "DataConnection"

DataConnection::DataConnection() {
    DataConnection::mInterfaceId = DC_INVALID_ID;
    DataConnection::mCId = DC_INVALID_ID;
    DataConnection::mRpDataController = NULL;
    DataConnection::mCompatibleApnContexts.clear();
}

DataConnection::DataConnection(int id, RpDataController *rpDataController) {
    DataConnection::mInterfaceId = id;
    DataConnection::mCId = DC_INVALID_ID;
    DataConnection::mCompatibleApnContexts.clear();
    DataConnection::mRpDataController = rpDataController;
}

DataConnection::~DataConnection() {}

void DataConnection::setApnSetting(ApnSetting apnSetting) {
    DataConnection::mApnSetting = apnSetting;
}

ApnSetting DataConnection::getApnSetting() {
    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::getApnSetting: %s, with interfaceId: %d",
            mCId, mApnSetting.logInfo().c_str(), mInterfaceId);

    return DataConnection::mApnSetting;
}

void DataConnection::clearApnSetting() {
    DataConnection::mApnSetting.types.clear();
    DataConnection::mApnSetting.carrier_enabled = false;
}

int DataConnection::getApntypeProfile(const char *type)
{
    int len = sizeof(apntype_2_bitmask_id)/sizeof(ApnType_2_Bitmask_Id_T);
    for(int i = 0; i < len ; i++){
        if(strcasecmp(type,apntype_2_bitmask_id[i].type) == 0)
            return apntype_2_bitmask_id[i].mtk_profile_id;
     }
    return RIL_DATA_PROFILE_INVALID;
}


void DataConnection::setupDataCall(string apnType, const sp<RfxMessage>& setupDataRequest, int radioTech, int profileId, string apn,
        string user, string password, int authType, string protocol, int interface_id) {
    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::setupDataCall with original request %s", mCId, RpDataUtils::requestOrUrcToString(setupDataRequest->getId()));
    RFX_LOG_I(RFX_LOG_TAG, "DataConnection(%d)::setupDataCall, apnType: %s, radioTech: %d, profileId: %d, apn: %s, user: %s, password: %s, authType: %d, protocol: %s, id: %d",
            mCId, apnType.c_str(), radioTech, profileId, apn.c_str(), user.c_str(), password.c_str(), authType, protocol.c_str(), interface_id);
    Parcel* parcel = setupDataRequest->getParcel();
    // make sure the position for writing data right.
    parcel->setDataPosition(sizeof(int32_t)*2);
    // fill the parcel with apn details.
    parcel->writeInt32(16); // conent length
    RpDataUtils::writeStringToParcel(parcel, RpDataUtils::i2s(radioTech + 2).c_str()); // radioTechnology, TODO: double check the value
    #if defined(TELEPHONYWARE) ||defined(TELE_FWK)
    #define TEXT_APN_TYPE_DEFAULT "default"

    int mtk_profile_id;
    if(apnType.c_str())
    {
        if(strstr(apnType.c_str(), "iot_"))
        {
            RFX_LOG_D(RFX_LOG_TAG, "DataConnection::setupDataCall, apnType: %s", TEXT_APN_TYPE_DEFAULT);
            mtk_profile_id = getApntypeProfile(TEXT_APN_TYPE_DEFAULT);
        }
        else
        {
            RFX_LOG_D(RFX_LOG_TAG, "DataConnection::setupDataCall, apnType: %s", apnType.c_str());
            mtk_profile_id = getApntypeProfile( apnType.c_str());
        }
    }
    else
    {
        RFX_LOG_D(RFX_LOG_TAG, "DataConnection::setupDataCall apnType not exist, apnType: %s", apnType.c_str());
        mtk_profile_id = getApntypeProfile( apnType.c_str());
    }

    RpDataUtils::writeStringToParcel(parcel, RpDataUtils::i2s(mtk_profile_id).c_str());
    #else
    RpDataUtils::writeStringToParcel(parcel, RpDataUtils::i2s(profileId).c_str());
    #endif
    RpDataUtils::writeStringToParcel(parcel, apn.c_str());  // apn name
    RpDataUtils::writeStringToParcel(parcel, user.c_str());  // user
    RpDataUtils::writeStringToParcel(parcel, password.c_str()); // password
    RpDataUtils::writeStringToParcel(parcel, RpDataUtils::i2s(authType).c_str());   // authType
    RpDataUtils::writeStringToParcel(parcel, protocol.c_str()); // protocol
    //RpDataUtils::writeStringToParcel(parcel, RpDataUtils::i2s(interface_id + 1).c_str()); // interfaceID,
    RpDataUtils::writeStringToParcel(parcel, protocol.c_str()); // roaming protocol
    RpDataUtils::writeStringToParcel(parcel, "1023"); //  supportedApnTypesBitmap
    RpDataUtils::writeStringToParcel(parcel, "1"); // bearerBitmap
    RpDataUtils::writeStringToParcel(parcel, "1"); // "1"
    RpDataUtils::writeStringToParcel(parcel, "1"); // mtu
    RpDataUtils::writeStringToParcel(parcel, "1"); // ""
    RpDataUtils::writeStringToParcel(parcel, "1"); // ""
    RpDataUtils::writeStringToParcel(parcel, "1"); // roamingAllowed
    RpDataUtils::writeStringToParcel(parcel, "1"); // reason

    int ini_profile_id = RpDataUtils::type2profileId(apnType);
    notifyMbrainDataPoint(ini_profile_id, Data_Call_Error_None);
    DataConnection::mRpDataController->sendToMD(setupDataRequest);
}

void DataConnection::connect(string apnType, const sp<RfxMessage>& request) {
    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::connect", mCId);

    int radioTech = DataConnection::mRpDataController->getRilVoiceRegState();
    int profileId = DataConnection::mApnSetting.profile_id;
    string apn = DataConnection::mApnSetting.apn;
    string user = DataConnection::mApnSetting.user;
    string password = DataConnection::mApnSetting.password;
    int authType = DataConnection::mApnSetting.auth_type;
    if (authType == -1) {
        authType = user.empty() ? SETUP_DATA_AUTH_NONE : SETUP_DATA_AUTH_PAP_CHAP;
    }
    string protocol;
    if (DataConnection::mRpDataController->isRoaming()) {
        protocol = DataConnection::mApnSetting.roaming_protocol;
    } else {
        protocol = DataConnection::mApnSetting.protocol;
    }
    DataConnection::setupDataCall(apnType, request, radioTech, profileId, apn,
            user, password, authType, protocol, DataConnection::mInterfaceId);
}

void DataConnection::setDataCallResponse(DataCallResponse dataCallResponse) {
    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::setDataCallResponse %s",
            mCId, dataCallResponse.logInfo().c_str());

    DataConnection::mDataCallResponse = dataCallResponse;
    DataConnection::mCId = dataCallResponse.cid;
}

DataCallResponse DataConnection::getDataCallResponse() {
    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::getDataCallResponse: %s ",
            mCId, mDataCallResponse.logInfo().c_str());

    return DataConnection::mDataCallResponse;
}

void DataConnection::setDcFail(DcFail dcFail) {
    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::setDcFail %d", mCId, dcFail.mDcFailCause);
    DataConnection::mDcFail = dcFail;
}

void DataConnection::setcId(int cId) {
    DataConnection::mCId = cId;
}

int DataConnection::getcId() {
    return DataConnection::mCId;
}

void DataConnection::deactivateDataCall(int cId, const sp<RfxMessage>& tearDownDataRequest) {
    RFX_LOG_I(RFX_LOG_TAG, "DataConnection(%d)::deactivateDataCall: %d, with original request: %s",
            mCId, cId, RpDataUtils::requestOrUrcToString(tearDownDataRequest->getId()));

    Parcel* parcel = tearDownDataRequest->getParcel();
    // make sure the position for writing data right.
    parcel->setDataPosition(sizeof(int32_t)*2);

    parcel->writeInt32(2); // conent length
    RpDataUtils::writeStringToParcel(parcel, RpDataUtils::i2s(cId).c_str()); // cId
    RpDataUtils::writeStringToParcel(parcel, RpDataUtils::i2s(0).c_str()); // reason, 0 means DEACTIVATE_REASON_NONE
    DataConnection::mRpDataController->sendToMD(tearDownDataRequest);
}

void DataConnection::addCompatibleApnContext(ApnContext *apnContext) {
    DataConnection::mCompatibleApnContexts.insert(apnContext);

    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::addCompatibleApnContext. and now the compatible apn context number is: %lu",
            mCId, mCompatibleApnContexts.size());

}

void DataConnection::removeCompatibleApnContext(ApnContext *apnContext) {
    DataConnection::mCompatibleApnContexts.erase(apnContext);

    RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::removeCompatibleApnContext. and now the compatible apn context number is: %lu",
            mCId, mCompatibleApnContexts.size());
}

void DataConnection::updateCompatibleApnContexts(ApnContext *apnContext) {
    set<ApnContext*>::iterator itor = DataConnection::mCompatibleApnContexts.begin();
    while (itor != DataConnection::mCompatibleApnContexts.end()) {
        if (apnContext->getApnType() != (*itor)->getApnType()) {
            (*itor)->setSate(apnContext->getState());

            RFX_LOG_D(RFX_LOG_TAG, "DataConnection(%d)::updateCompatibleApnContexts set apnContext(%s) state as: %d",
                    mCId, (*itor)->getApnType().c_str(), apnContext->getState());

            (*itor)->setApnSetting(apnContext->getApnSetting());
        }
        itor++;
    }

}

set<ApnContext*> &DataConnection::getCompatibleApnContext() {
    return DataConnection::mCompatibleApnContexts;
}

string DataConnection::logInfo() {

    string dataConnection("DataConnection(" + RpDataUtils::i2s(mCId) + "):: ");
    dataConnection += (", mApnSetting: " + mApnSetting.logInfo());
    dataConnection += (", mInterfaceId: " + RpDataUtils::i2s(mInterfaceId));
    dataConnection += (", mCid: " + RpDataUtils::i2s(mCId));
    dataConnection += (", mDataCallResponse: " + mDataCallResponse.logInfo());
    dataConnection += (", mDcFail: " + RpDataUtils::i2s(mDcFail.mDcFailCause));
    return dataConnection;
}
