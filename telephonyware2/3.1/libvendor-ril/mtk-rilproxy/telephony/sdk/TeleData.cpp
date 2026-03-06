// SPDX-License-Identifier: MediaTekProprietary

#define LOG_TAG "TELEDATA_SERVICE"

#include <mtk_log.h>
#include <map>
#include "RpDataUtils.h"
#include "../include/telephony/ril.h"
#include "RpApnController.h"
#include "TeleData.h"

using namespace VENDOR_SPACE;

::ndk::ScopedAStatus TeleData::deactiveDataCall(int32_t in_profileid, int32_t* _aidl_return) {
    std::string type = RpDataUtils::profileId2Type(in_profileid);
    if(type.empty()) {
        RLOGD(" deactiveDataCall id(%d) error", in_profileid);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
    }

    if(mLastToken != -1) {
        int request_id;
        bool ret = false;
        ret = mAidlAdapter->isRequestPending(mLastToken, &request_id);
        if (ret) {
            if((request_id == RIL_REQUEST_SETUP_DATA_CALL) || (request_id == RIL_REQUEST_DEACTIVATE_DATA_CALL)) {
                //should never go here
                RLOGE("setupdatacall or deactivedatacall on going!\
                   mLastToken=%d directly return\n", mLastToken);
                *_aidl_return = -1;
                return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
            }
        }
    }

    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("deactiveDataCall token %d, id:%d",token, in_profileid);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_DEACTIVATE_DATA_CALL);
    mAidlAdapter->addPendingProfileId(token, in_profileid);
    mLastToken = token;
    p.writeInt32(RIL_REQUEST_DEACTIVATE_DATA_CALL);
    p.writeInt32(token);
    RpDataUtils::writeStringToParcel(&p, type.c_str());
    mAidlAdapter->sendRequest(p,mSlotId);

    RLOGI("deactiveDataCall token %d, id:%d done",token, in_profileid);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RpAidlAdapter::getInstance()->deathRecipientCallback(cookie);
}

::ndk::ScopedAStatus TeleData::setResponseFunctions(const std::shared_ptr<VENDOR_SPACE::ITeleDataResponse>& in_dataResponseParam,
    const std::shared_ptr<VENDOR_SPACE::ITeleDataIndication>& in_dataIndicationParam) {
    RLOGD("setResponseFunctions, slotId = %d", mSlotId);
    if(in_dataIndicationParam == nullptr || in_dataResponseParam == nullptr) {
        RLOGE("TeleData setResponseFunctions error");
        return ::ndk::ScopedAStatus::ok();
    }
    if(mAidlAdapter) {
        mAidlAdapter->mTeleDataClientMapMutex.lock();
        RLOGD(" TeleData setResponseFunctions lock slotId = %d", mSlotId);
        mDataResponse = in_dataResponseParam;
        mDataIndication = in_dataIndicationParam;
        int clientIdx = 0;
        int ret = -1;
        ret = mAidlAdapter->getFreeDataClientIndex(&clientIdx);
        if (ret) {
            RLOGE("DataService Client number exceeds!!!!");
            mAidlAdapter->mTeleDataClientMapMutex.unlock();
            return ::ndk::ScopedAStatus::ok();
        }
        RLOGD("%s insert data ind location: %d, slotid = %d", __FUNCTION__, clientIdx, mSlotId);
        mapDataClientIndications[clientIdx] = in_dataIndicationParam;
        mapDataClientResponses[clientIdx] = in_dataResponseParam;

        mAidlAdapter->mTeleDataClientMapMutex.unlock();
        RLOGD(" TeleData setResponseFunctions unlock slotId = %d", mSlotId);
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleData::setupDataCall(int32_t in_profileid, int32_t* _aidl_return) {
    std::string type = RpDataUtils::profileId2Type(in_profileid);
    if(type.empty()) {
        RLOGE(" setupDataCall id(%d) error", in_profileid);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
    }

    if(mLastToken != -1) {
        int request_id;
        bool ret = false;
        ret = mAidlAdapter->isRequestPending(mLastToken, &request_id);
        if (ret) {
            if((request_id == RIL_REQUEST_SETUP_DATA_CALL) || (request_id == RIL_REQUEST_DEACTIVATE_DATA_CALL)) {
                //should never go here
                RLOGE("setupdatacall or deactivedatacall on going!\
                   mLastToken=%d directly return\n", mLastToken);
                *_aidl_return = -1;
                return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
            }
        }
    }

    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setupDataCall token:%d, id: %d\n",token, in_profileid);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SETUP_DATA_CALL);
    mAidlAdapter->addPendingProfileId(token, in_profileid);
    mLastToken = token;
    p.writeInt32(RIL_REQUEST_SETUP_DATA_CALL);
    p.writeInt32(token);
    RpDataUtils::writeStringToParcel(&p, type.c_str());
    mAidlAdapter->sendRequest(p,mSlotId);
    RLOGI("setupDataCall token:%d, id: %d done",token, in_profileid);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleData::startKeepalive(const VENDOR_SPACE::KeepaliveRequest& in_keepalive, VENDOR_SPACE::KeepaliveStatus* _aidl_return) {
    Parcel p;
    Parcel *pResponse=NULL;
    int32_t ret = -1;
    status_t res  = OK;
    int token = mAidlAdapter->generateToken();
    RLOGI("startKeepalive token %d, keepalive=%s",token, in_keepalive.toString().c_str());
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_START_KEEPALIVE);
    p.writeInt32(RIL_REQUEST_START_KEEPALIVE);
    p.writeInt32(token);
    p.writeInt32(in_keepalive.type);
    RpDataUtils::writeStringToParcel(&p, in_keepalive.sourceAddress.c_str());
    p.writeInt32(in_keepalive.sourcePort);
    RpDataUtils::writeStringToParcel(&p, in_keepalive.destinationAddress.c_str());
    p.writeInt32(in_keepalive.destinationPort);
    p.writeInt32(in_keepalive.maxKeepaliveIntervalMillis);
    p.writeInt32(in_keepalive.cid);
    ret = mAidlAdapter->sendRequest(p, mSlotId);
    if(ret == -1) {
        RLOGW("%s response %d.\n", __FUNCTION__, ret);
        _aidl_return->code = -1;
        _aidl_return->sessionHandle = -1;
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
    }
    res = mAidlAdapter->wait();

    if (res != OK) {
        if (res == TIMED_OUT) {
            RLOGW("%s wait Response timeout\n", __FUNCTION__);
            mAidlAdapter->removePendingRequest(token);
        }
        _aidl_return->code = -1;
        _aidl_return->sessionHandle = -1;
        _aidl_return->ril_err = -1;
    }else {
        ret = mAidlAdapter->responseErr;
        if(ret == OK) {
            mAidlAdapter->parcelResponse.setDataPosition(0);
            pResponse = &mAidlAdapter->parcelResponse;
            if(pResponse) {
                int n = 0;
                mAidlAdapter->parcelResponse.readInt32(&n);
                _aidl_return->sessionHandle = n;
                mAidlAdapter->parcelResponse.readInt32(&n);
                _aidl_return->code = n;
                _aidl_return->ril_err = 0;
            } else {
                RLOGW("%s response is NULL", __FUNCTION__);
                _aidl_return->code = -1;
                _aidl_return->sessionHandle = -1;
                _aidl_return->ril_err = -1;
            }
        } else {
            _aidl_return->code = -1;
            _aidl_return->sessionHandle = -1;
            _aidl_return->ril_err = -1;
        }
    }

    RLOGD("%s response %d, sessionHandle: %d, code = %d", __FUNCTION__, ret, _aidl_return->sessionHandle, _aidl_return->code);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleData::stopKeepalive(int32_t in_sessionHandle, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("stopKeepalive token %d, in_sessionHandle=%d",token, in_sessionHandle);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_STOP_KEEPALIVE);
    p.writeInt32(RIL_REQUEST_STOP_KEEPALIVE);
    p.writeInt32(token);
    p.writeInt32(in_sessionHandle);
    mAidlAdapter->sendRequest(p,mSlotId);
    status_t res  = OK;
    int32_t ret = -1;
    res = mAidlAdapter->wait();
    if (res != OK) {
        if (res == TIMED_OUT) {
            RLOGW("IVSPushMSD wait Response timeout\n");
            mAidlAdapter->removePendingRequest(token);
        }
    }else {
        ret = mAidlAdapter->responseErr;
    }

    RLOGI("stopKeepalive response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}