// SPDX-License-Identifier: MediaTekProprietary

#include <cstdint>
#include <mtk_log.h>
#define LOG_TAG "TELE_SERVICE"

#include <prop/properties.h>
#include "TeleService.h"
#include "RpAidlAdapter.h"
#include "../include/telephony/ril.h"
#include "RpDataUtils.h"
#include "RpApnController.h"

using namespace ::aidl::vendor::mediatek::hardware::tbox;

static const char PROPERTY_VONR_ENABLE[4][30] = {
    "persist.vendor.vonr.enable1",
    "persist.vendor.vonr.enable2",
    "persist.vendor.vonr.enable3",
    "persist.vendor.vonr.enable4"
};

Mutex gRequestLock;

void TeleService::setCallStatus(int status) {
    RLOGD("setCallStatus %d slot %d\n", status, mSlotId);
    mCallStatus = status;
}

void TeleService::setLastDataFailReason(std::string type, int reason) {
    RLOGD("setDataFailReason profile(%s)=reason(%d)", type.c_str(), reason);
    s_datafailReason[type] = reason;
}

int32_t TeleService::setUnsolicitedResponseFilter(int filter) {
    RLOGD("%s filter %x", __FUNCTION__, filter);
    int token = 0;
    int32_t ret = 0;
    status_t res  = OK;

    token = mAidlAdapter->generateToken();
    RLOGD("%s token2 %d",__FUNCTION__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return -1;
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return -1;
    }
    status = p.writeInt32(1);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return -1;
    }
    status = p.writeInt32(filter);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return -1;
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s (RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER) wait Response timeout", __FUNCTION__ );
                mAidlAdapter->removePendingRequest(token);
            }
            ret = -1;
        } else {
            ret = mAidlAdapter->responseErr;
        }
    }

    return ret;
}

// Methods from VENDOR_SPACE::ITeleService follow.
::ndk::ScopedAStatus TeleService::GetOperatorCode(VENDOR_SPACE::parcelable_GetOperatorCode* _aidl_return) {
    int32_t op = -1;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("GetOperatorCode token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LOCAL_GET_OPERATOR_CODE);

    status_t status = p.writeInt32(RIL_REQUEST_LOCAL_GET_OPERATOR_CODE);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            _aidl_return->ril_err = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("GetOperatorCode wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&op);
            }
        }
    }
    RLOGI("GetOperatorCode,%d\n",op);

    _aidl_return->ril_err = ret;
    _aidl_return->op_code = op;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::IVSPushMSD(int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("IVSPushMSD token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_IVS_PUSH_MSD);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_IVS_PUSH_MSD);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("IVSPushMSD wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }
    RLOGI("IVSPushMSD response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::PSAPPushMSD(int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("PSAPPushMSD token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_PSAP_PULL_MSD);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_PSAP_PULL_MSD);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("PSAPPushMSD wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }
    RLOGI("PSAPPushMSD response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::answerVcall(int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("answerVcall token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ANSWER);
    status_t status = p.writeInt32(RIL_REQUEST_ANSWER);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("answerVcall wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }
    RLOGD("answerVcall response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::deactiveDataCall(int32_t in_profile_id) {
    std::string type = RpDataUtils::profileId2Type(in_profile_id);

    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGD("deactiveDataCall token %d\n",token);

    mAidlAdapter->addPendingProfileId(token, in_profile_id);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_DEACTIVATE_DATA_CALL);
    status_t status = p.writeInt32(RIL_REQUEST_DEACTIVATE_DATA_CALL);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, type.c_str());
    mAidlAdapter->sendRequest(p,mSlotId);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::deleteReconfNumber(int32_t* _aidl_return) {
    Parcel p;
    int size;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_RECONF_NUM);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_RECONF_NUM);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(0);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(-1);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, "");
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("deleteReconfNumber wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }
    RLOGD("deleteReconfNumber response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::deleteTestNumber(int32_t* _aidl_return) {
    Parcel p;
    int size;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_TEST_NUM);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_TEST_NUM);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(0);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(-1);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, "");
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("deleteTestNumber wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }
    RLOGD("deleteTestNumber response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::enableIms(int32_t in_onoff, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    char value[PROPERTY_VALUE_MAX] = { 0 };
    int vonr_enable = property_get_int32(PROPERTY_VONR_ENABLE[mSlotId], 0);
    RLOGI("enableIms(%d) token %d, default vonr enable: %d\n",in_onoff,token, vonr_enable);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_IMSCFG);
    status_t status = p.writeInt32(RIL_REQUEST_SET_IMSCFG);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(6);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    if(in_onoff >= 1) {
        status = p.writeInt32(vonr_enable == 1 ? in_onoff : 1);
    } else {
        status = p.writeInt32(in_onoff);
    }
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }

    if ( in_onoff >= 1 ) {
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(1);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(1);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
    } else {
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(0);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        mAidlAdapter->setTimeOut(8e9);
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("enableIms wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("enableIms response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setDefaultDataSlot(int32_t in_slotId, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setDefaultDataSlot(%d) token %d\n", in_slotId, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_PREFERRED_DATA_MODEM);
    p.writeInt32(RIL_REQUEST_SET_PREFERRED_DATA_MODEM);
    p.writeInt32(token);
    p.writeInt32(1);
    p.writeInt32(in_slotId);
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        mAidlAdapter->setTimeOut(8e9);
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("enableIms wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("setDefaultDataSlot response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::enableRadio(int32_t in_onoff, const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    #ifdef MTK_HYP_DUAL_TELE_SUPPORT
        RLOGI("MTK_HYP_DUAL_TELE_SUPPORT CAN NOT SET RADIO\n");
        return ::ndk::ScopedAStatus::();
    #endif
    RLOGI("enableRadio(%d) token %d\n",in_onoff,token);
    status_t status;
    if (in_onoff < 2) {
        mAidlAdapter->addPendingRequest(token, RIL_REQUEST_RADIO_POWER);
        status = p.writeInt32(RIL_REQUEST_RADIO_POWER);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(token);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(1);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(in_onoff);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        mAidlAdapter->sendRequest(p,mSlotId);
        mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    } else if (in_onoff == 2) {
        RLOGI("enableRadio 2, send RIL_REQUEST_MODEM_POWEROFF\n");
        mAidlAdapter->addPendingRequest(token, RIL_REQUEST_MODEM_POWEROFF);
        status = p.writeInt32(RIL_REQUEST_MODEM_POWEROFF);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(token);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        mAidlAdapter->sendRequest(p,mSlotId);
        mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    } else if (in_onoff == 3) {
        RLOGI("enableRadio 3, send RIL_REQUEST_MODEM_POWERON\n");
        mAidlAdapter->addPendingRequest(token, RIL_REQUEST_MODEM_POWERON);
        status = p.writeInt32(RIL_REQUEST_MODEM_POWERON);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        status = p.writeInt32(token);
        if (status != OK ) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return = -1;
            return ::ndk::ScopedAStatus::ok();
        }
        mAidlAdapter->sendRequest(p,mSlotId);
        mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    } else {
        RLOGE("enableRadio(%d) invalid parameter for token %d\n",in_onoff,token);
    }

    RLOGI("enableRadio response %d\n", 0);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::endVcall(int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("endVcall token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_HANGUP_ALL);
    status_t status = p.writeInt32(RIL_REQUEST_HANGUP_ALL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("endVcall wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }
    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

static std::string convertCharPtrToAidlString(const char *ptr) {
    return std::string(ptr);
}

::ndk::ScopedAStatus TeleService::getDataCallInfo(int32_t in_profile_id, VENDOR_SPACE::parcelable_dataCallResultGet* _aidl_return) {
    RLOGD("getDataCallInfo");
    VENDOR_SPACE::SetupDataCallResult datacallinfo = {};
    int32_t ret = -1;

    std::string type = RpDataUtils::profileId2Type(in_profile_id);
    if(type.empty()) {
        RLOGW(" getDataCallInfo profileId(%d) error", in_profile_id);
        _aidl_return->ril_err = ret;
        _aidl_return->info = datacallinfo;
        return ::ndk::ScopedAStatus::ok();
    }

    MTK_Data_Call_Response_v1 mRsp;
    ret = mAidlAdapter->getDataCallInfo(in_profile_id, &mRsp);
    // RLOGD("input value in teleservice->getDataCallInfo, ret=%d, in_profile_id=%d, mRsp.pdnState=%d,"
    //     " mRsp.status=%d, mRsp.apnType=%s,mRsp.type=%s, mRsp.ifname=%s,"
    //     " mRsp.addresses=%s, mRsp.dnses=%s, mRsp.gateways=%s, mRsp.pcscf=%s,"
    //     " mRsp.mtu=%d\n", ret, in_profile_id, mRsp.pdnState, mRsp.status, mRsp.apnType, mRsp.type, mRsp.ifname,
    //     mRsp.addresses, mRsp.dnses, mRsp.gateways, mRsp.pcscf, mRsp.mtu);

    datacallinfo.profileId = in_profile_id;
    datacallinfo.pdnState = mRsp.pdnState;
    datacallinfo.failcause = mRsp.status;
    datacallinfo.apnType = convertCharPtrToAidlString(mRsp.apnType);
    datacallinfo.type = convertCharPtrToAidlString(mRsp.type);
    datacallinfo.ifname = convertCharPtrToAidlString(mRsp.ifname);
    datacallinfo.addresses = convertCharPtrToAidlString(mRsp.addresses);
    datacallinfo.dnses = convertCharPtrToAidlString(mRsp.dnses);
    datacallinfo.gateways = convertCharPtrToAidlString(mRsp.gateways);
    datacallinfo.pcscf = convertCharPtrToAidlString(mRsp.pcscf);
    datacallinfo.mtu = mRsp.mtu;

    _aidl_return->ril_err = ret;
    _aidl_return->info = datacallinfo;

    if (mRsp.apnType != NULL) {
        free(mRsp.apnType);
    }
    if (mRsp.type != NULL) {
        free(mRsp.type);
    }
    if (mRsp.ifname != NULL) {
        free(mRsp.ifname);
    }
    if (mRsp.addresses != NULL) {
        free(mRsp.addresses);
    }
    if (mRsp.dnses != NULL) {
        free(mRsp.dnses);
    }
    if (mRsp.gateways != NULL) {
        free(mRsp.gateways);
    }
    if (mRsp.pcscf  != NULL) {
        free(mRsp.pcscf);
    }

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getApn(int32_t in_profile_id, VENDOR_SPACE::parcelable_getApn* _aidl_return) {
    RLOGD("getApn");
    VENDOR_SPACE::ApnInfo apninfo;
    int32_t ret = -1;

    std::string type = RpDataUtils::profileId2Type(in_profile_id);
    if(type.empty()) {
        RLOGW(" getApn id(%d) error", in_profile_id);
        _aidl_return->ril_err = ret;
        _aidl_return->info = apninfo;
        return ::ndk::ScopedAStatus::ok();
    }
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGD("getApn token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LOCAL_GET_APN);
    status_t status = p.writeInt32(RIL_REQUEST_LOCAL_GET_APN);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, type.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getApn wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            Parcel *pResponse = &mAidlAdapter->parcelResponse;
            ret = mAidlAdapter->responseErr;
            if(ret == -1) {
                RLOGW("getApn fail! \n");
                goto out;
            }
            char* types = RpDataUtils::strdupReadString(pResponse);
            if(types == NULL) {
            ret = -1;
            goto out;
            }
            int profile_id = RpDataUtils::type2profileId(std::string(types));
            int pdp_type = pResponse->readInt32();
            int auth_type = pResponse->readInt32();
            char* apn = RpDataUtils::strdupReadString(pResponse);
            if(apn == NULL) {
                ret = -1;
                RpDataUtils::freeMemory(types);
                goto out;
            }
            char* user = RpDataUtils::strdupReadString(pResponse);
            if(user == NULL) {
                ret = -1;
                RpDataUtils::freeMemory(types);
                RpDataUtils::freeMemory(apn);
                goto out;
            }
            char* password = RpDataUtils::strdupReadString(pResponse);
            if(password == NULL) {
                ret = -1;
                RpDataUtils::freeMemory(types);
                RpDataUtils::freeMemory(apn);
                RpDataUtils::freeMemory(user);
                goto out;
            }
            apninfo.profile_id = profile_id;
            apninfo.pdp_type = (VENDOR_SPACE::Apn_Pdp_Type)pdp_type;
            apninfo.auto_proto = (VENDOR_SPACE::Apn_Auth_Proto)auth_type;
            apninfo.apn_name = apn;
            apninfo.username = user;
            apninfo.password = password;

            RpDataUtils::freeMemory(types);
            RpDataUtils::freeMemory(apn);
            RpDataUtils::freeMemory(user);
            RpDataUtils::freeMemory(password);
        }
    }

out:
    RLOGI("getApn response %d\n", ret);
    _aidl_return->ril_err = ret;
    _aidl_return->info = apninfo;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getCSQ(VENDOR_SPACE::parcelable_getCSQ* _aidl_return) {
    int32_t csq = 999;
    int32_t sig_level = 88;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("getCSQ token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LOCAL_SIGNAL_STRENGTH);
    status_t status = p.writeInt32(RIL_REQUEST_LOCAL_SIGNAL_STRENGTH);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getCSQ wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&sig_level);
                mAidlAdapter->parcelResponse.readInt32(&csq);
            }
        }
    }

    RLOGD("getCSQ %d response %d.\n",csq, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->csq = csq;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getCardStatus(VENDOR_SPACE::parcelable_getCardStatus* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse;
    int32_t status = -1;
    int32_t ret = -1;
    status_t res  = OK;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_GET_SIM_STATUS);
    status_t r = p.writeInt32(RIL_REQUEST_GET_SIM_STATUS);
    if (r != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    r = p.writeInt32(token);
    if (r != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                mAidlAdapter->removePendingRequest(token);
            }
            status = 2;
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK) {
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                pResponse->readInt32(&status);
            }
        }
    }

    RLOGI("%s: status %d response %d.",__FUNCTION__, status, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->status = status;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getCardFullStatus(VENDOR_SPACE::parcelable_getCardFullStatus* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse;
    int32_t status = -1;
    int32_t ret = -1;

    status_t res  = OK;
    RIL_CardStatus_v6 cardStatus;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_GET_SIM_STATUS);
    p.writeInt32(RIL_REQUEST_GET_SIM_STATUS);
    p.writeInt32(token);
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                mAidlAdapter->removePendingRequest(token);
            }
            status = 2;
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK) {
                int temp_int = 0;

                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                //pResponse->readInt32(&status);

                pResponse->readInt32(&temp_int);
                _aidl_return->cardStatus.card_state = (MTK_CardState)temp_int;
                pResponse->readInt32(&temp_int);
                _aidl_return->cardStatus.universal_pin_state = (MTK_PinState)temp_int;
                pResponse->readInt32(&(_aidl_return->cardStatus.gsm_umts_subscription_app_index));
                pResponse->readInt32(&(_aidl_return->cardStatus.cdma_subscription_app_index));
                pResponse->readInt32(&(_aidl_return->cardStatus.ims_subscription_app_index));
                pResponse->readInt32(&(_aidl_return->cardStatus.num_applications));
                RLOGD("getCardFullStatus card_state %d , universal_pin_state %d , gsm_umts_subscription_app_index %d , cdma_subscription_app_index %d , ims_subscription_app_index %d , num_applications %d ",
                        _aidl_return->cardStatus.card_state, _aidl_return->cardStatus.universal_pin_state, _aidl_return->cardStatus.gsm_umts_subscription_app_index,
                        _aidl_return->cardStatus.cdma_subscription_app_index, _aidl_return->cardStatus.ims_subscription_app_index, _aidl_return->cardStatus.num_applications);
                int temp_app_type = 0;
                int temp_app_state = 0;
                int temp_perso_substate = 0;
                int temp_pin = 0;
                std::vector<MTK_AppStatus> AppStatus;
                AppStatus.resize(_aidl_return->cardStatus.num_applications);
                for (int i = 0; i <_aidl_return->cardStatus.num_applications; i++) {
                    pResponse->readInt32(&temp_app_type);
                    AppStatus[i].app_type = (MTK_AppType)temp_app_type;
                    pResponse->readInt32(&temp_app_state);
                    AppStatus[i].app_state = (MTK_AppState)temp_app_state;
                    pResponse->readInt32(&temp_perso_substate);
                    AppStatus[i].perso_substate = (MTK_PersoSubstate)temp_perso_substate;
                    char* aidtemp = RpDataUtils::strdupReadString(pResponse);
                    if(aidtemp == NULL) {
                        RLOGE("%s readString failed", __FUNCTION__);
                        return ::ndk::ScopedAStatus::ok();
                    }
                    AppStatus[i].aid_ptr = std::string(aidtemp);
                    free(aidtemp);
                    char* applabeltemp = RpDataUtils::strdupReadString(pResponse);
                    if(applabeltemp == NULL) {
                        RLOGE("%s readString failed", __FUNCTION__);
                        return ::ndk::ScopedAStatus::ok();
                    }
                    AppStatus[i].app_label_ptr = std::string(applabeltemp);
                    free(applabeltemp);
                    pResponse->readInt32(&(AppStatus[i].pin1_replaced));
                    pResponse->readInt32(&(temp_pin));
                    AppStatus[i].pin1 = (MTK_PinState)temp_pin;
                    pResponse->readInt32(&(temp_pin));
                    AppStatus[i].pin2 = (MTK_PinState)temp_pin;
                    RLOGD("getCardFullStatus cardStatus  app_type %d , app_state %d , perso_substate %d , aid_ptr %s , app_label_ptr %s , pin1_replaced %d pin1 %d , pin2 %d",
                                AppStatus[i].app_type, AppStatus[i].app_state, AppStatus[i].perso_substate, AppStatus[i].aid_ptr.c_str(),
                                AppStatus[i].app_label_ptr.c_str(), AppStatus[i].pin1_replaced, AppStatus[i].pin1, AppStatus[i].pin2);
                }
                _aidl_return->cardStatus.applications = AppStatus;
            }
        }
    }

    RLOGI("%s: status %d response %d.",__FUNCTION__, _aidl_return->cardStatus.card_state, ret);
    _aidl_return->ril_err = ret;
    return ::ndk::ScopedAStatus::ok();
}


::ndk::ScopedAStatus TeleService::getCurrentCall(VENDOR_SPACE::parcelable_getCurrentCall* _aidl_return) {
    RLOGI("%s callid[%d] state[%d] isMT[%d] number[%s] for slot%d\n", __FUNCTION__, currentCallMsg.call_id, currentCallMsg.state, currentCallMsg.isMT, currentCallMsg.number.c_str(), mSlotId);
    VENDOR_SPACE::ECallStateMsg CallMsg;
    CallMsg.call_id = currentCallMsg.call_id;
    CallMsg.state = currentCallMsg.state;
    CallMsg.isMT = currentCallMsg.isMT;
    CallMsg.number = currentCallMsg.number;

    RLOGI("%s: response %d.",__FUNCTION__, 0);
    _aidl_return->ril_err = 0;
    _aidl_return->ecallMsg = CallMsg;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getDataCallReason(int32_t in_profile, VENDOR_SPACE::parcelable_getDataCallReason* _aidl_return) {
    RLOGD("%s: in_profile: %d", __FUNCTION__, in_profile);
    std::string type = RpDataUtils::profileId2Type(in_profile);
    if(s_datafailReason.find(type) == s_datafailReason.end()) {
        RLOGW("%s: can not find in_profile: %d,type: %s", __FUNCTION__, in_profile,type.c_str());
        _aidl_return->reason = VENDOR_SPACE::MTK_DataCallFailCause::PDP_FAIL_NONE;
        _aidl_return->ril_err = -1;
    } else {
        VENDOR_SPACE::MTK_DataCallFailCause reason = (VENDOR_SPACE::MTK_DataCallFailCause)s_datafailReason[type];
        RLOGI("%s: response %d.",__FUNCTION__, 0);
        _aidl_return->reason = reason;
        _aidl_return->ril_err = 0;
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getEcallType(VENDOR_SPACE::parcelable_getEcallType* _aidl_return) {
    RLOGI("getEcallType: %d slot %d\n", ecall_type, mSlotId);

    RLOGI("%s: response %d.",__FUNCTION__, 0);
    _aidl_return->ril_err = 0;
    _aidl_return->type = (int32_t)ecall_type;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getFullSignalStrength(VENDOR_SPACE::parcelable_getFullSignalStrength* _aidl_return) {
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    VENDOR_SPACE::MTK_SignalStrength msg;

    int token = mAidlAdapter->generateToken();
    RLOGD("getFullSignalStrength token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SIGNAL_STRENGTH);
    status_t status = p.writeInt32(RIL_REQUEST_SIGNAL_STRENGTH);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getFullSignalStrength Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                memset(&msg, 0 ,sizeof(VENDOR_SPACE::MTK_SignalStrength));
                int32_t lte_cqiTableIndex = 0;
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&msg.GW_SignalStrength.signalStrength);
                mAidlAdapter->parcelResponse.readInt32(&msg.GW_SignalStrength.bitErrorRate);
                mAidlAdapter->parcelResponse.readInt32(&msg.GW_SignalStrength.timingAdvance);
                mAidlAdapter->parcelResponse.readInt32(&msg.CDMA_SignalStrength.dbm);
                mAidlAdapter->parcelResponse.readInt32(&msg.CDMA_SignalStrength.ecio);
                mAidlAdapter->parcelResponse.readInt32(&msg.EVDO_SignalStrength.dbm);
                mAidlAdapter->parcelResponse.readInt32(&msg.EVDO_SignalStrength.ecio);
                mAidlAdapter->parcelResponse.readInt32(&msg.EVDO_SignalStrength.signalNoiseRatio);
                mAidlAdapter->parcelResponse.readInt32(&msg.LTE_SignalStrength.signalStrength);
                mAidlAdapter->parcelResponse.readInt32(&msg.LTE_SignalStrength.rsrp);
                mAidlAdapter->parcelResponse.readInt32(&msg.LTE_SignalStrength.rsrq);
                mAidlAdapter->parcelResponse.readInt32(&msg.LTE_SignalStrength.rssnr);
                mAidlAdapter->parcelResponse.readInt32(&msg.LTE_SignalStrength.cqi);
                mAidlAdapter->parcelResponse.readInt32(&msg.LTE_SignalStrength.timingAdvance);
                mAidlAdapter->parcelResponse.readInt32(&lte_cqiTableIndex);
                mAidlAdapter->parcelResponse.readInt32(&msg.TD_SCDMA_SignalStrength.signalStrength);
                mAidlAdapter->parcelResponse.readInt32(&msg.TD_SCDMA_SignalStrength.bitErrorRate);
                mAidlAdapter->parcelResponse.readInt32(&msg.TD_SCDMA_SignalStrength.rscp);
                mAidlAdapter->parcelResponse.readInt32(&msg.WCDMA_SignalStrength.signalStrength);
                mAidlAdapter->parcelResponse.readInt32(&msg.WCDMA_SignalStrength.bitErrorRate);
                mAidlAdapter->parcelResponse.readInt32(&msg.WCDMA_SignalStrength.rscp);
                mAidlAdapter->parcelResponse.readInt32(&msg.WCDMA_SignalStrength.ecno);
                mAidlAdapter->parcelResponse.readInt32(&msg.NR_SignalStrength.ssRsrp);
                mAidlAdapter->parcelResponse.readInt32(&msg.NR_SignalStrength.ssRsrq);
                mAidlAdapter->parcelResponse.readInt32(&msg.NR_SignalStrength.ssSinr);
                mAidlAdapter->parcelResponse.readInt32(&msg.NR_SignalStrength.csiRsrp);
                mAidlAdapter->parcelResponse.readInt32(&msg.NR_SignalStrength.csiRsrq);
                mAidlAdapter->parcelResponse.readInt32(&msg.NR_SignalStrength.csiSinr);
            }
        }
    }

    RLOGD("getFullSignalStrength response %d.\n", ret);
    _aidl_return->ril_err = ret;
    _aidl_return->signalStrength = msg;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getIccid(VENDOR_SPACE::parcelable_getIccid* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse=NULL;
    char * mIccid = NULL;
    int32_t ret = -1;
    status_t res  = OK;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SIM_GET_ICCID);
    status_t status = p.writeInt32(RIL_REQUEST_SIM_GET_ICCID);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        ret = mAidlAdapter->sendRequest(p, mSlotId);
        if(ret == -1) {
            RLOGW("%s mSdkClient is NULL, return\n", __FUNCTION__);

            RLOGW("%s response %d.\n", __FUNCTION__, ret);
            _aidl_return->ril_err = ret;
            if (mIccid != nullptr) {
                _aidl_return->iccid = std::string(mIccid);
            } else {
                _aidl_return->iccid = std::string(); // or some default value or handle the null case appropriately
            }
            return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
        }
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                mAidlAdapter->removePendingRequest(token);
            }
            mIccid = "";
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK) {
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                if(pResponse) {
                    mIccid = RpDataUtils::strdupReadString(pResponse);
                    if(mIccid == NULL) {
                        mIccid = "";
                    }
                } else {
                    RLOGW("%s response is NULL", __FUNCTION__);
                    mIccid = "";
                }
            } else {
                mIccid = "";
            }

        }
    }

    RLOGD("%s: %s",__FUNCTION__, mIccid);

    std::string result(mIccid);
    if(!(result.empty())) {
        RpDataUtils::freeMemory(mIccid);  // Free the memory only if mImsi is not null
    }

    RLOGD("%s response %d.\n", __FUNCTION__, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->iccid = result;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getImei(VENDOR_SPACE::parcelable_getImei* _aidl_return) {
    RLOGD("getImei");
    Parcel p;
    Parcel *pResponse;
    char * mImei = NULL;
    int32_t ret = -1;
    status_t res  = OK;

    int token = mAidlAdapter->generateToken();
    RLOGD("getImei token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_DEVICE_IDENTITY);
    status_t status = p.writeInt32(RIL_REQUEST_DEVICE_IDENTITY);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getImei wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
            mImei = "";
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                mImei = RpDataUtils::strdupReadString(pResponse);
                if(mImei == NULL) {
                    mImei = "";
                }
            } else {
                mImei = "";
            }
        }
    }

    RLOGD("getImei %s\n",mImei);
    std::string result(mImei);
    if(!(result.empty())) {
        RpDataUtils::freeMemory(mImei);  // Free the memory only if mImei is not null
    }

    RLOGD("%s response %d.\n", __FUNCTION__, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->imei = result;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getImsRegState(VENDOR_SPACE::parcelable_getImsRegState* _aidl_return) {
    RLOGD("getImsRegState");
    int n = 0;
    int32_t ims_reg = 0;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("getImsRegState token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_IMS_REGISTRATION_STATE);
    status_t status = p.writeInt32(RIL_REQUEST_IMS_REGISTRATION_STATE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getImsRegState ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&n);
                mAidlAdapter->parcelResponse.readInt32(&ims_reg);
            }
        }
    }

    RLOGD("getImsRegState %d response %d.\n", ims_reg, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->status = ims_reg;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getImsi(VENDOR_SPACE::parcelable_getImsi* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse = NULL;
    char * mImsi = NULL;
    int32_t ret = -1;
    status_t res  = OK;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_GET_IMSI);
    status_t status = p.writeInt32(RIL_REQUEST_GET_IMSI);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, "A0000000871002"); //only for GSM
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                mAidlAdapter->removePendingRequest(token);
            }
            mImsi = "";
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK) {
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                if(pResponse) {
                    mImsi = RpDataUtils::strdupReadString(pResponse);
                    if(mImsi == NULL) {
                        mImsi = "";
                    }
                } else {
                    RLOGW("%s response is NULL", __FUNCTION__);
                    mImsi = "";
                }
            } else {
                mImsi = "";
            }
        }
    }

    RLOGD("%s: %s",__FUNCTION__, mImsi);
    std::string result(mImsi);
    if(!(result.empty())) {
        RpDataUtils::freeMemory(mImsi);  // Free the memory only if mImsi is not null
    }
    RLOGD("%s: response %d.",__FUNCTION__, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->iccid = result;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getLastDataCallfailReason(VENDOR_SPACE::parcelable_getLastDataCallfailReason* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse;
    VENDOR_SPACE::MTK_DataCallFailCause reason = VENDOR_SPACE::MTK_DataCallFailCause::PDP_FAIL_NONE;
    int32_t ret = -1;
    status_t res  = OK;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d\n",__FUNCTION__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE);
    status_t status = p.writeInt32(RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                int num = pResponse->readInt32();
                int failCause = pResponse->readInt32();
                reason = (VENDOR_SPACE::MTK_DataCallFailCause)failCause;
            }

        }
    }

    RLOGD("%s reason %d response %d.\n",__FUNCTION__, reason, ret);
    _aidl_return->reason = reason;
    _aidl_return->ril_err = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getMccMnc(VENDOR_SPACE::parcelable_getMccMnc* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    char value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("vendor.gsm.ril.uicc.mccmnc", value, "");
    RLOGD("%s %s\n",__FUNCTION__, value);
    std::string result(value);

    RLOGD("%s response %d\n",__FUNCTION__, 0);
    _aidl_return->ril_err = 0;
    _aidl_return->mccmnc = result;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getModemStat(VENDOR_SPACE::parcelable_getModemStat* _aidl_return) {
    RLOGD("getModemStat");
    char value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("vendor.mtk.md1.status", value, "");
    RLOGD("%s modemstate=%s", __FUNCTION__, value);
    std::string result(value);

    RLOGD("%s response %d\n",__FUNCTION__, 0);
    _aidl_return->ril_err = 0;
    _aidl_return->stat = result;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getNetState(VENDOR_SPACE::parcelable_getNetState* _aidl_return) {
    VENDOR_SPACE::VoiceRegStateResult cs_result;
    VENDOR_SPACE::DataRegStateResult ps_result;
    int32_t ret = -1;
    Parcel p;
    Parcel *pResponse;
    status_t res  = OK;
    int32_t voice_reg = -1;
    int32_t data_reg = -1;
    int32_t radio_status = -1;

    int token = mAidlAdapter->generateToken();
    RLOGD("getNetState token %d, timout value %lld\n", token, mAidlAdapter->mRequestTimeout);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LOCAL_REG_STATE_TYPE);
    status_t status = p.writeInt32(RIL_REQUEST_LOCAL_REG_STATE_TYPE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        //parse response and set the cs_result
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGE("getNetState cs wait Response timeout %lld\n", mAidlAdapter->mRequestTimeout);
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&voice_reg);
                if((voice_reg == 1) || (voice_reg == 5) ) {
                    cs_result.regState = VENDOR_SPACE::SimpleRegState::NW_ATTACHED;
                } else if ((voice_reg == 4) || (voice_reg == 14 )) {
                    cs_result.regState = VENDOR_SPACE::SimpleRegState::NW_UNKNOWN;
                }else {
                    cs_result.regState = VENDOR_SPACE::SimpleRegState::NW_DETACHED;
                }
                mAidlAdapter->parcelResponse.readInt32(&data_reg);
                if((data_reg == 1) || (data_reg == 5) ) {
                    ps_result.regState = VENDOR_SPACE::SimpleRegState::NW_ATTACHED;
                } else if ((data_reg == 4) || (data_reg == 14 )) {
                    ps_result.regState = VENDOR_SPACE::SimpleRegState::NW_UNKNOWN;
                }else {
                    ps_result.regState = VENDOR_SPACE::SimpleRegState::NW_DETACHED;
                }
                mAidlAdapter->parcelResponse.readInt32(&radio_status);
            }
        }
    }

    RLOGD("%s, voice_reg=%d, data_reg=%d radio_status=%d response=%d .", __FUNCTION__, voice_reg, data_reg,radio_status, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->cs_state = cs_result;
    _aidl_return->ps_state = ps_result;
    _aidl_return->radioStatus = radio_status;

    return ::ndk::ScopedAStatus::ok();
}


int TeleService::getRafFromNetworkType(int type) {
    int raf;

    switch (type) {
        case PREF_NET_TYPE_GSM_WCDMA:
            raf = RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_GSM_ONLY:
            raf = RAF_GSM_GROUP;
            break;
        case PREF_NET_TYPE_WCDMA:
            raf = RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_GSM_WCDMA_AUTO:
            raf = RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_CDMA_EVDO_AUTO:
            raf = RAF_CDMA_GROUP | RAF_EVDO_GROUP;
            break;
        case PREF_NET_TYPE_CDMA_ONLY:
            raf = RAF_CDMA_GROUP;
            break;
        case PREF_NET_TYPE_EVDO_ONLY:
            raf = RAF_EVDO_GROUP;
            break;
        case PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO:
            raf = RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP;
            break;
        case PREF_NET_TYPE_LTE_CDMA_EVDO:
            raf = RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP;
            break;
        case PREF_NET_TYPE_LTE_GSM_WCDMA:
            raf = RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA:
            raf = RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP;
            break;
        case PREF_NET_TYPE_LTE_ONLY:
            raf = RAF_LTE | RAF_LTE_CA;
            break;
        case PREF_NET_TYPE_LTE_WCDMA:
            raf = RAF_LTE_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_ONLY:
            raf = RAF_TD_SCDMA;
            break;
        case PREF_NET_TYPE_TD_SCDMA_WCDMA:
            raf = RAF_TD_SCDMA | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_LTE:
            raf = RAF_TD_SCDMA | RAF_LTE_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_GSM:
            raf = RAF_GSM_GROUP | RAF_TD_SCDMA;
            break;
        case PREF_NET_TYPE_TD_SCDMA_GSM_LTE:
            raf = RAF_TD_SCDMA | RAF_GSM_GROUP | RAF_LTE_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA:
            raf = RAF_WCDMA_GROUP | RAF_TD_SCDMA | RAF_GSM_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_WCDMA_LTE:
            raf = RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA_LTE:
            raf = RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA_CDMA_EVDO_AUTO:
            raf = RAF_TD_SCDMA | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_TD_SCDMA_LTE_CDMA_EVDO_GSM_WCDMA:
            raf = RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_LTE_GSM:
            raf = RAF_LTE_GROUP | RAF_GSM_GROUP;
            break;
        case PREF_NET_TYPE_LTE_TDD_ONLY:
            raf = RAF_LTE_GROUP;
            break;
        case PREF_NET_TYPE_CDMA_GSM:
            raf = RAF_CDMA_GROUP | RAF_GSM_GROUP;
            break;
        case PREF_NET_TYPE_CDMA_EVDO_GSM:
            raf = RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP;
            break;
        case PREF_NET_TYPE_LTE_CDMA_EVDO_GSM:
            raf = RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP;
            break;
        case PREF_NET_TYPE_NR_ONLY:
            raf = RAF_NR;
            break;
        case PREF_NET_TYPE_NR_LTE:
            raf = RAF_NR | RAF_LTE_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_CDMA_EVDO:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_GSM_WCDMA:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_CDMA_EVDO_GSM_WCDMA:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_WCDMA:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_TDSCDMA:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_TD_SCDMA;
            break;
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_GSM_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_WCDMA:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM_WCDMA:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        case PREF_NET_TYPE_NR_LTE_TDSCDMA_CDMA_EVDO_GSM_WCDMA:
            raf = RAF_NR | RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP;
            break;
        default:
            raf = RAF_UNKNOWN;
            RLOGI("getRafFromNetworkType, type=%d, raf=%d", type, raf);
            break;
    }
    return raf;
}

int TeleService::getAdjustedRaf(int raf) {
    raf = ((RAF_GSM_GROUP & raf) > 0) ? (RAF_GSM_GROUP | raf) : raf;
    raf = ((RAF_WCDMA_GROUP & raf) > 0) ? (RAF_WCDMA_GROUP | raf) : raf;
    raf = ((RAF_CDMA_GROUP & raf) > 0) ? (RAF_CDMA_GROUP | raf) : raf;
    raf = ((RAF_EVDO_GROUP & raf) > 0) ? (RAF_EVDO_GROUP | raf) : raf;
    raf = ((RAF_LTE_GROUP & raf) > 0) ? (RAF_LTE_GROUP | raf) : raf;
    raf = ((RAF_NR & raf) > 0) ? (RAF_NR | raf) : raf;
    if ((raf & RAF_CDMA_GROUP) == 0 && (raf & RAF_EVDO_GROUP) != 0
            && (raf & ~RAF_EVDO_GROUP) != 0) {
        raf &= ~RAF_EVDO_GROUP;
        RLOGD("getAdjustedRaf after removing EVDO: %d", raf);
    }
    return raf;
}

int TeleService::getNetworkTypeFromRaf(int raf) {
    int type = -1, adj_raf = -1;

    adj_raf = getAdjustedRaf(raf);
    RLOGD("%s, raf: %d, adj_raf: %d", __FUNCTION__, raf, adj_raf);
    switch (adj_raf) {
        case (RAF_GSM_GROUP | RAF_WCDMA_GROUP):
            type = PREF_NET_TYPE_GSM_WCDMA;
            break;
        case (RAF_GSM_GROUP):
            type = PREF_NET_TYPE_GSM_ONLY;
            break;
        case (RAF_WCDMA_GROUP):
            type = PREF_NET_TYPE_WCDMA;
            break;
        case (RAF_CDMA_GROUP | RAF_EVDO_GROUP):
            type = PREF_NET_TYPE_CDMA_EVDO_AUTO;
            break;
        case (RAF_CDMA_GROUP):
            type = PREF_NET_TYPE_CDMA_ONLY;
            break;
        case (RAF_EVDO_GROUP):
            type = PREF_NET_TYPE_EVDO_ONLY;
            break;
        case (RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP):
            type = PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO;
            break;
        case (RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP):
            type = PREF_NET_TYPE_LTE_CDMA_EVDO;
            break;
        case (RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP):
            type = PREF_NET_TYPE_LTE_GSM_WCDMA;
            break;
        case (RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP):
            type = PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA;
            break;
        case (RAF_LTE_GROUP):
            type = PREF_NET_TYPE_LTE_ONLY;
            break;
        case (RAF_LTE_GROUP | RAF_WCDMA_GROUP):
            type = PREF_NET_TYPE_LTE_WCDMA;
            break;
        case (RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_ONLY;
            break;
        case (RAF_WCDMA_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_WCDMA;
            break;
        case (RAF_LTE_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_LTE;
            break;
        case (RAF_GSM_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_GSM;
            break;
        case (RAF_GSM_GROUP | RAF_LTE_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_GSM_LTE;
            break;
        case (RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA;
            break;
        case (RAF_LTE_GROUP | RAF_WCDMA_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_WCDMA_LTE;
            break;
        case (RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA_LTE;
            break;
        case (RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_GSM_WCDMA_CDMA_EVDO_AUTO;
            break;
        case (RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_TD_SCDMA_LTE_CDMA_EVDO_GSM_WCDMA;
            break;
        case (RAF_LTE_GROUP | RAF_GSM_GROUP):
            type = PREF_NET_TYPE_LTE_GSM;
            break;
        case (RAF_CDMA_GROUP | RAF_GSM_GROUP):
            type = PREF_NET_TYPE_CDMA_GSM;
            break;
        case (RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP):
            type = PREF_NET_TYPE_CDMA_EVDO_GSM;
            break;
        case (RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP):
            type = PREF_NET_TYPE_LTE_CDMA_EVDO_GSM;
            break;
        case (RAF_NR):
            type = PREF_NET_TYPE_NR_ONLY;
            break;
        case (RAF_NR | RAF_LTE_GROUP):
            type = PREF_NET_TYPE_NR_LTE;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP):
            type = PREF_NET_TYPE_NR_LTE_CDMA_EVDO;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP):
            type = PREF_NET_TYPE_NR_LTE_GSM_WCDMA;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP):
            type = PREF_NET_TYPE_NR_LTE_CDMA_EVDO_GSM_WCDMA;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_WCDMA_GROUP):
            type = PREF_NET_TYPE_NR_LTE_WCDMA;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_NR_LTE_TDSCDMA;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_TD_SCDMA | RAF_GSM_GROUP):
            type = PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_WCDMA_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_NR_LTE_TDSCDMA_WCDMA;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_NR_LTE_TDSCDMA_GSM_WCDMA;
            break;
        case (RAF_NR | RAF_LTE_GROUP | RAF_CDMA_GROUP | RAF_EVDO_GROUP | RAF_GSM_GROUP | RAF_WCDMA_GROUP | RAF_TD_SCDMA):
            type = PREF_NET_TYPE_NR_LTE_TDSCDMA_CDMA_EVDO_GSM_WCDMA;
            break;
        default:
            RLOGI("getNetworkTypeFromRaf, raf=%d, adj_raf=%d, type=%d", raf, adj_raf, type);
            break;
    }
    return type;
}

::ndk::ScopedAStatus TeleService::getPreferredNetworkType(
    VENDOR_SPACE::parcelable_getPreferredNetworkType *_aidl_return) {
    RLOGD("getPreferredNetworkType");
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGD("getPrefferredNetworkType token %d\n", token);
    mAidlAdapter->addPendingRequest(token,
                                    RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP);
    status_t status = p.writeInt32(RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    int32_t mode = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p, mSlotId);
        status_t res = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
        if (res == TIMED_OUT) {
            RLOGW("getPreferredNetworkType wait Response timeout\n");
            mAidlAdapter->removePendingRequest(token);
        }
        } else {
            Parcel *pResponse = &mAidlAdapter->parcelResponse;
            ret = mAidlAdapter->responseErr;
            if (ret == OK) {
                int32_t raf = pResponse->readInt32();
                mode = getNetworkTypeFromRaf(raf);
            }
        }
    }

    RLOGD("%s mode %d response %d\n", __FUNCTION__, mode, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->mode = mode;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getRadioAccessType(VENDOR_SPACE::parcelable_getRadioAccessType* _aidl_return) {
    VENDOR_SPACE::RadioTechnology radio_type = VENDOR_SPACE::RadioTechnology::UNKNOWN;
    int32_t ret = -1;
    Parcel p;
    Parcel *pResponse;
    status_t res  = OK;
    int32_t radioTechType = -1;

    int token = mAidlAdapter->generateToken();
    RLOGD("getRadioAccessType token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE);
    status_t status = p.writeInt32(RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getNetState cs wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&radioTechType);
                radio_type = (VENDOR_SPACE::RadioTechnology)radioTechType;
            }
        }
    }

    RLOGD("%s, radio_type %d response %d.",__FUNCTION__, radio_type, ret);
    _aidl_return->access_type = radio_type;
    _aidl_return->ril_err = ret;

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getSignalStrength(VENDOR_SPACE::parcelable_getSignalStrength* _aidl_return) {
    int32_t sig_level = 88;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("getSignalStrength token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LOCAL_SIGNAL_STRENGTH);
    status_t status = p.writeInt32(RIL_REQUEST_LOCAL_SIGNAL_STRENGTH);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getNetState ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&sig_level);
            }
        }
    }

    RLOGD("getSignalStrength,%d response %d.\n",sig_level, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->sig_level = sig_level;

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getStatusVcall(int32_t* _aidl_return) {
    RLOGI("getStatusVcall: %d slot %d\n", mCallStatus, mSlotId);
    *_aidl_return = mCallStatus;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::hangupEcall(int32_t* _aidl_return) {
    int32_t ret;
    TeleService::endVcall(&ret);
    RLOGI("hangupEcall: response %d\n", 0);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::holdVcall(int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("holdVcall token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE);
    status_t status = p.writeInt32(RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("holdVcall wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGD("holdVcall response %d\n",ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::supplyIccPinForApp(const std::string& in_pin, const std::string& in_aid, VENDOR_SPACE::parcelable_getPinInfo* _aidl_return){
    Parcel p;
    int32_t ret = -1;
    int32_t retryTimes = -1;
    int token = mAidlAdapter->generateToken();
    RLOGI("supplyIccPinForApp token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ENTER_SIM_PIN);
    status_t status = p.writeInt32(RIL_REQUEST_ENTER_SIM_PIN);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(2); // string number 2
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_pin.c_str());
    RpDataUtils::writeStringToParcel(&p, in_aid.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("supplyIccPinForApp wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        } else {
            ret = mAidlAdapter->responseErr;
            mAidlAdapter->parcelResponse.setDataPosition(0);
            mAidlAdapter->parcelResponse.readInt32(&retryTimes);
        }
    }

    RLOGD("supplyIccPinForApp is right? e = %d retryTimes = %d\n",ret,retryTimes);
    _aidl_return->ril_err = ret;
    _aidl_return->retryTimes = retryTimes;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::supplyIccPin2ForApp(const std::string& in_pin2, const std::string& in_aid, VENDOR_SPACE::parcelable_getPinInfo* _aidl_return){
        Parcel p;
    int32_t ret = -1;
    int32_t retryTimes = -1;
    int token = mAidlAdapter->generateToken();
    RLOGI("supplyIccPinForApp token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ENTER_SIM_PIN2);
    status_t status = p.writeInt32(RIL_REQUEST_ENTER_SIM_PIN2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(2); // string number 2
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_pin2.c_str());
    RpDataUtils::writeStringToParcel(&p, in_aid.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("supplyIccPinForApp wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        } else {
            ret = mAidlAdapter->responseErr;
            mAidlAdapter->parcelResponse.setDataPosition(0);
            mAidlAdapter->parcelResponse.readInt32(&retryTimes);
        }
    }

    RLOGD("supplyIccPinForApp is right? e = %d retryTimes = %d\n",ret,retryTimes);
    _aidl_return->ril_err = ret;
    _aidl_return->retryTimes = retryTimes;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::supplyIccPukForApp(const std::string& in_puk, const std::string& in_pin, const std::string& in_aid, VENDOR_SPACE::parcelable_getPinInfo* _aidl_return){
    Parcel p;
    int32_t ret = -1;
    int32_t retryTimes = -1;
    int token = mAidlAdapter->generateToken();
    RLOGI("supplyIccPukForApp token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ENTER_SIM_PUK);
    status_t status = p.writeInt32(RIL_REQUEST_ENTER_SIM_PUK);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(3); // string number 2
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_puk.c_str());
    RpDataUtils::writeStringToParcel(&p, in_pin.c_str());
    RpDataUtils::writeStringToParcel(&p, in_aid.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("supplyIccPukForApp wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        } else {
            ret = mAidlAdapter->responseErr;
            mAidlAdapter->parcelResponse.setDataPosition(0);
            mAidlAdapter->parcelResponse.readInt32(&retryTimes);
        }
    }

    RLOGD("supplyIccPinForApp is right? e = %d retryTimes = %d\n",ret,retryTimes);
    _aidl_return->ril_err = ret;
    _aidl_return->retryTimes = retryTimes;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::supplyIccPuk2ForApp(const std::string& in_puk2, const std::string& in_pin2, const std::string& in_aid, VENDOR_SPACE::parcelable_getPinInfo* _aidl_return){
    Parcel p;
    int32_t ret = -1;
    int32_t retryTimes = -1;
    int token = mAidlAdapter->generateToken();
    RLOGI("supplyIccPukForApp token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ENTER_SIM_PUK2);
    status_t status = p.writeInt32(RIL_REQUEST_ENTER_SIM_PUK2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(3); // string number 2
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_puk2.c_str());
    RpDataUtils::writeStringToParcel(&p, in_pin2.c_str());
    RpDataUtils::writeStringToParcel(&p, in_aid.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("supplyIccPukForApp wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        } else {
            ret = mAidlAdapter->responseErr;
            mAidlAdapter->parcelResponse.setDataPosition(0);
            mAidlAdapter->parcelResponse.readInt32(&retryTimes);
        }
    }

    RLOGD("supplyIccPinForApp is right? e = %d retryTimes = %d\n",ret,retryTimes);
    _aidl_return->ril_err = ret;
    _aidl_return->retryTimes = retryTimes;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::changeIccPinForApp(const std::string& old_pin, const std::string& new_pin, const std::string& in_aid, VENDOR_SPACE::parcelable_getPinInfo* _aidl_return){
    Parcel p;
    int32_t ret = -1;
    int32_t retryTimes = -1;
    int token = mAidlAdapter->generateToken();
    RLOGI("changeIccPinForApp token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_CHANGE_SIM_PIN);
    status_t status = p.writeInt32(RIL_REQUEST_CHANGE_SIM_PIN);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(3); // string number 2
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, old_pin.c_str());
    RpDataUtils::writeStringToParcel(&p, new_pin.c_str());
    RpDataUtils::writeStringToParcel(&p, in_aid.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("changeIccPinForApp wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        } else {
            ret = mAidlAdapter->responseErr;
            mAidlAdapter->parcelResponse.setDataPosition(0);
            mAidlAdapter->parcelResponse.readInt32(&retryTimes);
        }
    }

    RLOGD("supplyIccPinForApp is right? e = %d retryTimes = %d\n",ret,retryTimes);
    _aidl_return->ril_err = ret;
    _aidl_return->retryTimes = retryTimes;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::changeIccPin2ForApp(const std::string& old_pin2, const std::string& new_pin2, const std::string& in_aid, VENDOR_SPACE::parcelable_getPinInfo* _aidl_return){
    Parcel p;
    int32_t ret = -1;
    int32_t retryTimes = -1;
    int token = mAidlAdapter->generateToken();
    RLOGI("changeIccPin2ForApp token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_CHANGE_SIM_PIN2);
    status_t status = p.writeInt32(RIL_REQUEST_CHANGE_SIM_PIN2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(3); // string number 2
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, old_pin2.c_str());
    RpDataUtils::writeStringToParcel(&p, new_pin2.c_str());
    RpDataUtils::writeStringToParcel(&p, in_aid.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("changeIccPin2ForApp wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        } else {
            ret = mAidlAdapter->responseErr;
            mAidlAdapter->parcelResponse.setDataPosition(0);
            mAidlAdapter->parcelResponse.readInt32(&retryTimes);
        }
    }

    RLOGD("supplyIccPinForApp is right? e = %d retryTimes = %d\n",ret,retryTimes);
    _aidl_return->ril_err = ret;
    _aidl_return->retryTimes = retryTimes;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setFacilityLockForApp(const std::string& facility, int32_t lockState, const std::string& password, int32_t serviceClass, const std::string& appId, VENDOR_SPACE::parcelable_getPinInfo* _aidl_return){
    Parcel p;
    int32_t ret = -1;
    int32_t retryTimes = -1;
    int token = mAidlAdapter->generateToken();
    RLOGI("setFacilityLockForApp token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_FACILITY_LOCK);
    status_t status = p.writeInt32(RIL_REQUEST_SET_FACILITY_LOCK);
    RLOGI("setFacilityLockForApp1 facility = %s, lockState = %d, password = %s serviceClass = %d appId = %s", facility.c_str(), lockState, password.c_str(), serviceClass, appId.c_str());

    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(5); // string number 5
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = ret;
        return ::ndk::ScopedAStatus::ok();
    }
    RLOGI("setFacilityLockForApp facility = %s, lockState = %d, password = %s serviceClass = %d appId = %s", facility.c_str(), lockState, password.c_str(), serviceClass, appId.c_str());
    RpDataUtils::writeStringToParcel(&p, facility.c_str());
    p.writeInt32(lockState);
    RpDataUtils::writeStringToParcel(&p, password.c_str());
    p.writeInt32(serviceClass);
    RpDataUtils::writeStringToParcel(&p, appId.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setFacilityLockForApp wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        } else {
            ret = mAidlAdapter->responseErr;
            mAidlAdapter->parcelResponse.setDataPosition(0);
            mAidlAdapter->parcelResponse.readInt32(&retryTimes);
        }
    }

    RLOGD("supplyIccPinForApp is right? e = %d retryTimes = %d\n",ret,retryTimes);
    _aidl_return->ril_err = ret;
    _aidl_return->retryTimes = retryTimes;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setSimCardPower(int32_t CardPowerState, int32_t* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    int32_t ret = -1;
    status_t res  = OK;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, CardPowerState = %d",__FUNCTION__, token, CardPowerState);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_SIM_CARD_POWER);
    p.writeInt32(RIL_REQUEST_SET_SIM_CARD_POWER);
    p.writeInt32(token);
    p.writeInt32(1);
    p.writeInt32(CardPowerState);
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("iccCloseLogicalChannel wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGD("setSimCardPower response %d\n",ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::iccCloseLogicalChannel(int32_t in_channel, int32_t* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    int32_t ret = -1;
    status_t res  = OK;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, close in_channel=%d",__FUNCTION__, token, in_channel);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SIM_CLOSE_CHANNEL);
    status_t status = p.writeInt32(RIL_REQUEST_SIM_CLOSE_CHANNEL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_channel);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("iccCloseLogicalChannel wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGD("iccCloseLogicalChannel response %d\n",ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::iccOpenLogicalChannel(const std::string& in_aid, int32_t in_p2, VENDOR_SPACE::parcelable_iccOpenLogicalChannel* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse;
    int32_t mChannelId = -1;
    int32_t ret = -1;
    status_t res  = OK;
    int size = 0;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, in_aid=%s, in_p2=%d",__FUNCTION__, token, in_aid.c_str(), in_p2);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SIM_OPEN_CHANNEL);
    status_t status = p.writeInt32(RIL_REQUEST_SIM_OPEN_CHANNEL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_aid.c_str());
    status = p.writeInt32(in_p2);
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK) {
                pResponse = &mAidlAdapter->parcelResponse;
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse->readInt32(&mChannelId);
            }
        }
    }

    RLOGI("%s token %d: channelId=%d response=%d",__FUNCTION__, token, mChannelId, ret);

    _aidl_return->ril_err = ret;
    _aidl_return->channelId = mChannelId;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::iccTransmitApduLogicalChannel(int32_t in_channel, int32_t in_cla, int32_t in_instruction, int32_t in_p1, int32_t in_p2, int32_t in_p3, const std::string& in_data, const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, in_channel=%d, in_cla=%d, in_instruction=%d, in_p1=%d, in_p2=%d, in_p3=%d, in_data=%s",__FUNCTION__, token, in_channel, in_cla, in_instruction, in_p1, in_p2, in_p3, in_data.c_str());

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL);
    status_t status = p.writeInt32(RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_channel);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_cla);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_instruction);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_p1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_p2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_p3);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_data.c_str());
    mAidlAdapter->sendRequest(p,mSlotId);

    mapTokenResponsesV1_1.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));

    RLOGI("%s response=%d",__FUNCTION__, 0);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::iccIoForApp(int32_t command, int32_t fileid, const std::string& pathid, int32_t in_p1, int32_t in_p2, int32_t in_p3,const std::string& data, const std::string& pin2, const std::string& aid_ptr, VENDOR_SPACE::parcelable_getIccIo* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    int ret = -1;
    int32_t res = -1;
    int sw1 = -1;
    int sw2 = -1;
    char *ioResponse = NULL;
    Parcel p;
    Parcel *pResponse;
    int token = mAidlAdapter->generateToken();

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SIM_IO);
    status_t status = p.writeInt32(RIL_REQUEST_SIM_IO);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(9); //add param num 9
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(command);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(fileid);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, pathid.c_str());
    status = p.writeInt32(in_p1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_p2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_p3);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, data.c_str());
    RpDataUtils::writeStringToParcel(&p, pin2.c_str());
    RpDataUtils::writeStringToParcel(&p, aid_ptr.c_str());
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();

        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("iccIoForApp timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK) {
                RLOGW("%s response is OK", __FUNCTION__);
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                if(pResponse) {
                    pResponse->readInt32(&sw1);
                    pResponse->readInt32(&sw2);
                    ioResponse = RpDataUtils::strdupReadString(pResponse);
                    if(ioResponse == NULL) {
                        RpDataUtils::freeMemory(ioResponse);
                        RLOGE("%s readString failed", __FUNCTION__);
                        return ::ndk::ScopedAStatus::ok();
                    }
                    _aidl_return->sw1 = sw1;
                    _aidl_return->sw2 = sw2;
                    _aidl_return->response = ioResponse;
                } else {
                    RLOGW("%s response is NULL", __FUNCTION__);
                }
            }
        }
    }

    RLOGI("%s response=%d sw1 = %d , sw2 = %d, IoResponse = %s",__FUNCTION__, 0, sw1, sw2, ioResponse);
    _aidl_return->ril_err = ret;
    RpDataUtils::freeMemory(ioResponse);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::makeEcall(int8_t in_type, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d, in_type=%d\n", __FUNCTION__, token, in_type);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_MAKE_ECALL);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_MAKE_ECALL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32((int32_t)in_type);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("startVcall wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGD("startVcall response %d\n",ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::makeFastEcall(const VENDOR_SPACE::ECallReqMsg& in_msg, int32_t* _aidl_return) {
    Parcel p;
    int size;
    int digitLimit;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, ecall_cat=%d, ecall_variant=%d, address=%s, length=%d, domain=%d",__FUNCTION__, token,
        in_msg.ecall_cat, in_msg.ecall_variant, in_msg.address.c_str(), in_msg.length, in_msg.domain);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_FAST_MAKE_ECALL);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_FAST_MAKE_ECALL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32((RIL_ECall_Category)in_msg.ecall_cat);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32((RIL_ECall_Variant)in_msg.ecall_variant);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_msg.address.c_str());
    status = p.writeInt32((unsigned int)in_msg.length);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    digitLimit= MIN(in_msg.length, MSD_MAX_LENGTH);
    for (int digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
        p.write(&(in_msg.msd_data[digitCount]), sizeof(uint8_t));
        RLOGD("%s get from aidl msd_data[%d] = %d",__FUNCTION__, digitCount, in_msg.msd_data[digitCount]);
    }
    status = p.writeInt32((RIL_Dial_Domain)in_msg.domain);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("makeFastEcall wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGD("makeFastEcall response %d\n",ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::notifyIpoStatus(int32_t in_onoff, int32_t* _aidl_return) {
    std::string atCmd("");
    int filter = 0;
    int token = 0;
    if(in_onoff) {
        atCmd.append("AT+EURCFLT=1");
    } else {
        atCmd.append("AT+EURCFLT=0");
        filter = RIL_UnsolicitedResponseFilter::LINK_CAPACITY_ESTIMATE|
                RIL_UnsolicitedResponseFilter::PHYSICAL_CHANNEL_CONFIG|
                RIL_UnsolicitedResponseFilter::RIL_UR_DATA_CALL_DORMANCY_CHANGED|
                RIL_UnsolicitedResponseFilter::RIL_UR_FULL_NETWORK_STATE|
                RIL_UnsolicitedResponseFilter::REGISTRATION_FAILURE|
                RIL_UnsolicitedResponseFilter::BARRING_INFO|
                RIL_UnsolicitedResponseFilter::RIL_UR_SIGNAL_STRENGTH;
    }
    RLOGD("%s, in_onoff:%d, atCmd: %s, filter: %x", __FUNCTION__, in_onoff, atCmd.c_str(), filter);

    int32_t ret = -1;

    status_t res  = OK;
    //send AT
    token = mAidlAdapter->generateToken();
    RLOGD("%s token1 %d",__FUNCTION__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_OEM_HOOK_RAW);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_OEM_HOOK_RAW);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32((int32_t)atCmd.size());
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.write((const void*)(atCmd.c_str()),atCmd.size());
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s (RIL_REQUEST_OEM_HOOK_RAW) wait Response timeout", __FUNCTION__ );
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    if(ret == -1) {
        RLOGW("%s (RIL_REQUEST_OEM_HOOK_RAW) fail, just return", __FUNCTION__ );
        *_aidl_return = ret;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
    }
    ret = setUnsolicitedResponseFilter(filter);
    *_aidl_return = ret;
    RLOGD("notifyIpoStatus response %d\n", ret);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::resetIvs(int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d\n", __FUNCTION__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_RESET_IVS);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_RESET_IVS);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("resetIvs wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }
    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::restartNetwork(int32_t* _aidl_return) {
    RLOGI("%s, don't support, just return", __FUNCTION__);
    *_aidl_return = -1;
    return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
}

::ndk::ScopedAStatus TeleService::sendAT(const std::string& in_atCmd, int64_t in_timeout_ms, VENDOR_SPACE::parcelable_sendAT* _aidl_return) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse;
    char * rsp = NULL;
    int32_t ret = -1;
    status_t res  = OK;
    int size = 0;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_OEM_HOOK_RAW);
    status_t status = p.writeInt32(RIL_REQUEST_OEM_HOOK_RAW);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32((int32_t)in_atCmd.size());
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.write((const void*)(in_atCmd.c_str()),in_atCmd.size());
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        mAidlAdapter->setTimeOut(ms2ns(in_timeout_ms));
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                mAidlAdapter->removePendingRequest(token);
            }
            rsp = "";
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK) {
                pResponse = &mAidlAdapter->parcelResponse;
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse->readInt32(&size);
                rsp = (char *) calloc(1, size + 1);
                if(rsp != NULL) {
                    pResponse->read(rsp, size);
                    rsp[size] = '\0';
                }else {
                    rsp = "";
                    RLOGW("%s calloc fail\n", __FUNCTION__);
                }
            } else {
                rsp = "";
            }
        }
    }

    RLOGI("%s: size: %d, %s",__FUNCTION__, size, rsp);

    std::string result(rsp);
    if((ret == OK) && (!(result.empty()))) {
        RpDataUtils::freeMemory(rsp);
        rsp = nullptr;
    }
    RLOGI("%s response %d\n", __FUNCTION__, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->finalRsp = result;
    return ::ndk::ScopedAStatus::ok();
}
::ndk::ScopedAStatus TeleService::sendSms(const VENDOR_SPACE::SmsInfo& in_msg, const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();

    RLOGD("sendSms token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SEND_SMS);
    status_t status = p.writeInt32(RIL_REQUEST_SEND_SMS);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32((int32_t)in_msg.format);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_msg.phone_number.c_str());
    RpDataUtils::writeStringToParcel(&p, in_msg.sms_data.c_str());
    status = p.writeInt32(in_msg.sms_data_len);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    mAidlAdapter->sendRequest(p,mSlotId);

    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));

    RLOGI("%s response %d\n", __FUNCTION__, 0);
    *_aidl_return= 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setApn(const VENDOR_SPACE::ApnInfo& in_info, int32_t* _aidl_return) {
    RLOGD("setApn");
    std::string type = RpDataUtils::profileId2Type(in_info.profile_id);
    if(type.empty()) {
        RLOGW(" setApn id(%d) error", in_info.profile_id);
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_SECURITY);
    }

    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGD("setApn token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_LOCAL_UPDATE_APN);
    status_t status = p.writeInt32(RIL_REQUEST_LOCAL_UPDATE_APN);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, type.c_str());
    int32_t pdp_type = (int32_t)in_info.pdp_type;
    int32_t auto_proto = (int32_t)in_info.auto_proto;
    status = p.writeInt32(pdp_type);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(auto_proto);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_info.apn_name.c_str());
    RpDataUtils::writeStringToParcel(&p, in_info.username.c_str());
    RpDataUtils::writeStringToParcel(&p, in_info.password.c_str());
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        mAidlAdapter->setTimeOut(8e9);
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setApn wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setCTRLSequence(const VENDOR_SPACE::ECallCTRLSequence& in_quence, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setCTRLSequence token %d str1[%s] str2[%s] str3[%s]\n",token, in_quence.str1.c_str(), in_quence.str2.c_str(), in_quence.str3.c_str());
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_CTRL_SEQUENCE);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_CTRL_SEQUENCE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(3);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_quence.str1.c_str());
    RpDataUtils::writeStringToParcel(&p, in_quence.str2.c_str());
    RpDataUtils::writeStringToParcel(&p, in_quence.str3.c_str());
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setCTRLSequence wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setCellInfoInd(int32_t in_onoff, int32_t* _aidl_return) {
    int token = mAidlAdapter->generateToken();
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1); //number of ints
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    if(in_onoff == 1) {
        status = p.writeInt32(0);
        if (status != OK) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return= -1;
            return ::ndk::ScopedAStatus::ok();
        }
    } else {
        status = p.writeInt32(1);
        if (status != OK) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return= -1;
            return ::ndk::ScopedAStatus::ok();
        }
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setCellInfoInd wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setEcallType(int8_t in_type, int32_t* _aidl_return) {
    RLOGD("setEcallType %d slot %d\n", in_type, mSlotId);
    ecall_type = (RIL_Ecall_Type) in_type;
    RLOGD("%s response %d\n", __FUNCTION__, 0);
    *_aidl_return= 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setEmsdpri(const VENDOR_SPACE::ECallPRI& in_pri, int32_t* _aidl_return) {
    status_t res  = OK;
    int32_t ret = -1;
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setEmsdpri token %d\n", token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_PRI);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_PRI);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(4);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_pri.data1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_pri.data2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_pri.data3);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_pri.data4);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setEmsdpri wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setIVS(int8_t in_state, int32_t* _aidl_return) {
    status_t res  = OK;
    int32_t ret = -1;
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setIVS(%d) token %d\n",in_state,token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_IVS);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_IVS);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_state);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setIVS wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setMSD(const VENDOR_SPACE::ECallSetMSD& in_msd, int32_t* _aidl_return) {
    Parcel p;
    int digitLimit;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, call_id=%d, length=%d, in_msd_data=%s",__FUNCTION__, token,
        in_msd.call_id, in_msd.length, in_msd.msd_data.c_str());

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_MSD);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_MSD);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_msd.call_id);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_msd.length);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    digitLimit= MIN(in_msd.length, MSD_MAX_LENGTH);
    for (int digitCount = 0 ; digitCount < digitLimit; digitCount ++) {
        p.write(&(in_msd.msd_data.c_str()[digitCount]), sizeof(uint8_t));
        RLOGD("%s msd_data[%d] = %d",__FUNCTION__, digitCount, in_msd.msd_data.c_str()[digitCount]);
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setMSD wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setNadDeregTime(const VENDOR_SPACE::ECallTime& in_time, int32_t* _aidl_return) {
    status_t res  = OK;
    int32_t ret = -1;
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setNadDeregTime token %d\n", token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(4);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_time.purpose);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_time.mode);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_time.timer1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_time.timer2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setNadDeregTime wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setNadRegState(int8_t in_state, int32_t* _aidl_return) {
    status_t res  = OK;
    int32_t ret = -1;
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setNadRegState(%d) token %d\n",in_state,token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_REGISTRATION_STATE);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_REGISTRATION_STATE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_state);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setNadRegState wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setPASP(int8_t in_state, int32_t* _aidl_return) {
    status_t res  = OK;
    int32_t ret = -1;
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setPASP(%d) token %d\n",in_state,token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_PSAP);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_PSAP);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_state);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setIVS wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setOprtMode(int32_t in_mode, int32_t* _aidl_return) {
    status_t res  = OK;
    int32_t ret = -1;
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setOprtMode(%d) token %d\n", in_mode, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_OPRT_MODE);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_OPRT_MODE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_mode);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setOprtMode wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getOprtMode(VENDOR_SPACE::parcelable_getOprtMode* _aidl_return) {
    RLOGD("getOprtMode");
    int n = 0;
    int32_t mode = 0;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("getOprtMode token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_GET_OPRT_MODE);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_GET_OPRT_MODE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getOprtMode ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&n);
                mAidlAdapter->parcelResponse.readInt32(&mode);
            }
        }
    }

    RLOGD("getOprtMode %d response %d.\n", mode, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->mode = mode;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setPreferredNetworkType(int32_t in_mode, const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return) {
    RLOGD("setPreferredNetworkType");

    Parcel p;
    int token = mAidlAdapter->generateToken();
    int type = PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA;
    int raf = 0;
    RLOGI("setPrefferredNetworkType token %d, mode:%d",token,in_mode);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP);
    status_t status = p.writeInt32(RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    raf = getRafFromNetworkType(in_mode);
    RLOGI("setPreferredNetworkType,ml_mode = %d, raf = 0x%x, type = %d\n", in_mode, raf, type);

    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(raf);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);

    RLOGI("%s response %d\n", __FUNCTION__, 0);
    *_aidl_return= 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setReconfNumber(const VENDOR_SPACE::ECallSetNum& in_recNum, int32_t* _aidl_return) {
    Parcel p;
    int size;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, arg_num=%d, type=%d, address=%s",__FUNCTION__, token, in_recNum.arg_num, in_recNum.type, in_recNum.address.c_str());

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_RECONF_NUM);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_RECONF_NUM);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_recNum.arg_num);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    if (in_recNum.arg_num == 0) {
        status = p.writeInt32(-1);
        if (status != OK) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return= -1;
            return ::ndk::ScopedAStatus::ok();
        }
        RpDataUtils::writeStringToParcel(&p, "");
    } else if (in_recNum.arg_num == 1) {
        status = p.writeInt32(in_recNum.type);
        if (status != OK) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return= -1;
            return ::ndk::ScopedAStatus::ok();
        }
        RpDataUtils::writeStringToParcel(&p, "");
    } else if (in_recNum.arg_num == 2) {
        status = p.writeInt32(in_recNum.type);
        if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
        }
        RpDataUtils::writeStringToParcel(&p, in_recNum.address.c_str());
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setReconfNumber wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RpAidlAdapter::getInstance()->deathRecipientCallback(cookie);
}

::ndk::ScopedAStatus TeleService::setResponseFunctions(const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam, const std::shared_ptr<VENDOR_SPACE::ITeleServiceIndication>& in_teleServiceIndicationParam) {
    RLOGD("setResponseFunctions, slotId = %d", mSlotId);

    if (mAidlAdapter) {
        mAidlAdapter->mTeleServiceClientMapMutex.lock();
        if ((in_teleServiceResponseParam != nullptr) && (in_teleServiceIndicationParam != nullptr)) {
            int clientIdx = 0;
            int ret = -1;
            ret = mAidlAdapter->getFreeTeleClientIndex(&clientIdx);
            if (ret) {
                RLOGE("TeleService Client number exceeds!!!!");
                mAidlAdapter->mTeleServiceClientMapMutex.unlock();
                return ::ndk::ScopedAStatus::ok();
            }
            RLOGD("%s insert ind location: %d, slotid = %d", __FUNCTION__, clientIdx, mSlotId);
            mTeleIndications[clientIdx] = in_teleServiceIndicationParam;

            auto binder = mTeleIndications[clientIdx]->asBinder();  // Retrieve Native AIBinder from wrapper if needed
            if ((binder != nullptr) && (clientIdx % MAX_SLOT_NUM == 0)) {
                AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);
                if (AIBinder_linkToDeath(binder.get(), deathRecipient,
                                         (void*)(clientIdx)) != STATUS_OK) {
                    RLOGE("Failed to link death observer to the radio tele service binder");
                }
            } else {
                RLOGE("Binder retrieval failed");
            }
            mTeleResponses[clientIdx] = in_teleServiceResponseParam;
        }
        mAidlAdapter->mTeleServiceClientMapMutex.unlock();
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setTestNumber(const VENDOR_SPACE::ECallSetNum& in_testNum, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d, arg_num=%d, type=%d, address=%s",__FUNCTION__, token, in_testNum.arg_num, in_testNum.type, in_testNum.address.c_str());

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ECALL_SET_TEST_NUM);
    status_t status = p.writeInt32(RIL_REQUEST_ECALL_SET_TEST_NUM);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(in_testNum.arg_num);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return= -1;
        return ::ndk::ScopedAStatus::ok();
    }
    if (in_testNum.arg_num == 0) {
        status = p.writeInt32(-1);
        if (status != OK) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return= -1;
            return ::ndk::ScopedAStatus::ok();
        }
        RpDataUtils::writeStringToParcel(&p, "");
    } else if (in_testNum.arg_num == 1) {
        status = p.writeInt32(in_testNum.type);
        if (status != OK) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return= -1;
            return ::ndk::ScopedAStatus::ok();
        }
        RpDataUtils::writeStringToParcel(&p, "");
    } else if (in_testNum.arg_num == 2) {
        status = p.writeInt32(in_testNum.type);
        if (status != OK) {
            RLOGE("%s writeInt32 failed", __FUNCTION__);
            *_aidl_return= -1;
            return ::ndk::ScopedAStatus::ok();
        }
        RpDataUtils::writeStringToParcel(&p, in_testNum.address.c_str());
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setTestNumber wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setUnsolResponseFilter(VENDOR_SPACE::Mtk_IndicationFilter in_filter, int32_t* _aidl_return) {
    int32_t ret;
    ret = setUnsolicitedResponseFilter((int)in_filter);

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setupDataCall(int32_t in_profile_id) {
    std::string type = RpDataUtils::profileId2Type(in_profile_id);
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGD("setupDataCall token %d\n",token);

    mAidlAdapter->addPendingProfileId(token, in_profile_id);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SETUP_DATA_CALL);

    status_t status = p.writeInt32(RIL_REQUEST_SETUP_DATA_CALL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, type.c_str());
    mAidlAdapter->sendRequest(p,mSlotId);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::startVcall(const std::string& in_phone_number, int32_t cli, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("startVcall token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_DIAL);
    status_t status = p.writeInt32(RIL_REQUEST_DIAL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, in_phone_number.c_str());
    status = p.writeInt32(cli);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(0);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("startVcall wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::startDtmf(const std::string& phonenum, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("startDtmf token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_DTMF_START);
    status_t status = p.writeInt32(RIL_REQUEST_DTMF_START);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, phonenum.c_str());
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("startDtmf wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::stopDtmf(int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("stopDtmf token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_DTMF_STOP);
    status_t status = p.writeInt32(RIL_REQUEST_DTMF_STOP);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("stopDtmf wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}


::ndk::ScopedAStatus TeleService::endVcallId(int32_t call_id, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("endVcall token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_HANGUP);
    status_t status = p.writeInt32(RIL_REQUEST_HANGUP);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(call_id);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        //No Need wait, because rild don't handle hangup all return.
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("endVcallId wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::controlCall(int32_t controlType, int32_t callId, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("controlCall token %d\n",token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_HOLD_CALL);
    status_t status = p.writeInt32(RIL_REQUEST_HOLD_CALL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(controlType);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(callId);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        //No Need wait, because rild don't handle hangup all return.
        status_t res  = OK;
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("controlCall wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}


::ndk::ScopedAStatus TeleService::getAvailableNetworksWithAct(const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d\n",__func__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT);
    status_t status = p.writeInt32(RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);
    //No Need wait, because rild don't handle hangup all return.
    RLOGI("%s response \n", __func__);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::sendSmsNoEncoder(const VENDOR_SPACE::GsmSmsMessage& message, const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam) {
    const char* smscPdu = message.smscPdu.c_str();
    const char* pdu = message.pdu.c_str();
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d, smscPud: %s, pdu: %s", __func__, token, smscPdu, pdu);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SEND_SMS);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_SEND_SMS);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(2);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, smscPdu);
    RpDataUtils::writeStringToParcel(&p, pdu);
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);
    //No Need wait, because rild don't handle hangup all return.
    RLOGI("%s response \n", __func__);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getUsageSetting(VENDOR_SPACE::parcelable_getUsageSetting* _aidl_return) {
    RLOGD("getUsageSetting");
    int n = 0;
    int32_t usagesetting = 0;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("getUsageSetting token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_GET_USAGE_SETTING);
    status_t status = p.writeInt32(RIL_REQUEST_GET_USAGE_SETTING);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getUsageSetting ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readInt32(&usagesetting);
            }
        }
    }

    RLOGD("getUsageSetting %d response %d.\n", usagesetting, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->usageSetting = usagesetting;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setUsageSetting(VENDOR_SPACE::UsageSetting usageSetting, int32_t* _aidl_return) {
    Parcel p;
    int digitLimit;
    int token = mAidlAdapter->generateToken();
    RLOGD("%s(%d) token %d, usageSettings=%d", __FUNCTION__, mSlotId,  token, usageSetting);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_USAGE_SETTING);
    status_t status = p.writeInt32(RIL_REQUEST_SET_USAGE_SETTING);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32((int32_t)usageSetting);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setMSD wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("%s response %d\n", __FUNCTION__, ret);
    *_aidl_return= ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setNetworkSelectionModeAutomatic(const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam) {
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d", __func__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);
    //No Need wait, because rild don't handle hangup all return.
    RLOGI("%s response \n", __func__);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setNetworkSelectionModeManual(const std::string& operatorNumeric, VENDOR_SPACE::AccessNetwork ran, const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam) {
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d, operatorNumeric: %s, ran: %s", __func__, token, operatorNumeric.c_str(), toString(ran).c_str());
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, operatorNumeric.c_str());
    p.writeInt32(static_cast<int32_t>(ran));
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);
    //No Need wait, because rild don't handle hangup all return.
    RLOGI("%s response \n", __func__);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getNetworkSelectionMode(VENDOR_SPACE::parcelable_getNetworkSelectionMode* _aidl_return) {
    RLOGD("%s", __func__);
    int n = 0;
    bool mode = false;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d", __func__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE);
    status_t status = p.writeInt32(RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getNetworkSelectionMode ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readBool(&mode);
            }
        }
    }

    RLOGD("getNetworkSelectionMode %d response %d.\n", mode, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->mode = mode;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::isVoNrEnabled(VENDOR_SPACE::parcelable_isVoNrEnabled* _aidl_return) {
    RLOGD("%s", __func__);
    bool enable = false;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d", __func__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_IS_VONR_ENABLED);
    status_t status = p.writeInt32(RIL_REQUEST_IS_VONR_ENABLED);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getImsRegState ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readBool(&enable);
            }
        }
    }

    RLOGD("%s  enable: %d response %d.\n",__func__, enable, ret);
    _aidl_return->ril_err = ret;
    _aidl_return->enable = enable;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setVoNrEnabled(bool enable, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setVoNrEnabled(%d) token %d\n",enable,token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_ENABLE_VONR);
    status_t status = p.writeInt32(RIL_REQUEST_ENABLE_VONR);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeBool(enable);
    if (status != OK ) {
        RLOGE("%s writeBool failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        mAidlAdapter->setTimeOut(8e9);
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setVoNrEnabled wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("setVoNrEnabled response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getNitzTime(VENDOR_SPACE::parcelable_getNitzTime* _aidl_return) {
    RLOGD("%s", __func__);
    char* nitzTime = NULL;
    int32_t tzValid = 0;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d", __func__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_GET_NITZ);
    status_t status = p.writeInt32(RIL_REQUEST_GET_NITZ);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getImsRegState ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                if (pResponse) {
                    nitzTime = RpDataUtils::strdupReadString(pResponse);
                    tzValid = pResponse->readInt32();
                }
            }
        }
    }

    RLOGD("%s  nitzTime: %s, tzValid: %d", __func__, (nitzTime == NULL ? std::string("").c_str() : std::string(nitzTime)).c_str(), tzValid);
    _aidl_return->ril_err = ret;
    _aidl_return->nitzTime = (nitzTime == NULL ? std::string("") : std::string(nitzTime));
    _aidl_return->tz_valid = tzValid;
    if(nitzTime) {
        RpDataUtils::freeMemory(nitzTime);
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getVoiceRegistrationState(const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam) {
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d", __func__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_VOICE_REGISTRATION_STATE);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_VOICE_REGISTRATION_STATE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);
    //No Need wait, because rild don't handle hangup all return.
    RLOGI("%s response \n", __func__);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getDataRegistrationState(const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam) {
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d", __func__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_DATA_REGISTRATION_STATE);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_DATA_REGISTRATION_STATE);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);
    //No Need wait, because rild don't handle hangup all return.
    RLOGI("%s response \n", __func__);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getCellInfoList(const std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>& in_teleServiceResponseParam) {
    int token = mAidlAdapter->generateToken();
    RLOGI("%s token %d", __func__, token);
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_GET_CELL_INFO_LIST);
    Parcel p;
    status_t status = p.writeInt32(RIL_REQUEST_GET_CELL_INFO_LIST);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return ::ndk::ScopedAStatus::ok();
    }
    mapTokenResponses.insert(pair<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> > (token,in_teleServiceResponseParam));
    mAidlAdapter->sendRequest(p,mSlotId);
    //No Need wait, because rild don't handle hangup all return.
    RLOGI("%s response \n", __func__);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getOperator(VENDOR_SPACE::parcelable_getOperator* _aidl_return) {
    RLOGD("%s", __func__);
    char* longName = NULL;
    char* shortName = NULL;
    char* numeric = NULL;

    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d", __func__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_OPERATOR);
    status_t status = p.writeInt32(RIL_REQUEST_OPERATOR);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getOperator ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                pResponse = &mAidlAdapter->parcelResponse;
                if (pResponse) {
                    longName = RpDataUtils::strdupReadString(pResponse);
                    shortName = RpDataUtils::strdupReadString(pResponse);
                    numeric = RpDataUtils::strdupReadString(pResponse);
                }
            }
        }
    }

    _aidl_return->ril_err = ret;
    _aidl_return->longName = (longName == NULL ? std::string("") : std::string(longName));
    _aidl_return->shortName = (longName == NULL ? std::string("") : std::string(shortName));
    _aidl_return->numeric = (longName == NULL ? std::string("") : std::string(numeric));
    RLOGD("%s  ril_err = %d, longName = %s, shortName = %s, numeric = %s", __func__,
          _aidl_return->ril_err, _aidl_return->longName.c_str(), _aidl_return->shortName.c_str(), _aidl_return->numeric.c_str());
    if (longName) {
        RpDataUtils::freeMemory(longName);
    }
    if(shortName) {
        RpDataUtils::freeMemory(shortName);
    }
    if(numeric) {
        RpDataUtils::freeMemory(numeric);
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::setCallWaiting(bool enable, int32_t serviceClass, int32_t* _aidl_return) {
    Parcel p;
    int token = mAidlAdapter->generateToken();
    RLOGI("setCallWaiting(%d) token %d\n",enable,token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_CALL_WAITING);
    status_t status = p.writeInt32(RIL_REQUEST_SET_CALL_WAITING);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeBool(enable);
    if (status != OK ) {
        RLOGE("%s writeBool failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(serviceClass);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        mAidlAdapter->setTimeOut(8e9);
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("setCallWaiting wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
        }
    }

    RLOGI("setCallWaiting response %d\n", ret);
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getCallWaiting(int32_t serviceClass, VENDOR_SPACE::parcelable_getCallWaiting* _aidl_return) {
    Parcel p;
    bool enable;
    int32_t serviceClassOut;
    int token = mAidlAdapter->generateToken();
    RLOGI("getCallWaiting token %d\n",token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_QUERY_CALL_WAITING);
    status_t status = p.writeInt32(RIL_REQUEST_QUERY_CALL_WAITING);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err  = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(serviceClass);
    if (status != OK ) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        status_t res  = OK;
        mAidlAdapter->setTimeOut(8e9);
        res = mAidlAdapter->wait();
        mAidlAdapter->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getCallWaiting wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                mAidlAdapter->parcelResponse.readBool(&enable);
                mAidlAdapter->parcelResponse.readInt32(&serviceClassOut);
            }
        }
    }

    RLOGI("getCallWaiting response %d\n", ret);
    _aidl_return->ril_err = ret;
    _aidl_return->enable = enable;
    _aidl_return->serviceClass = serviceClass;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus TeleService::getImsCfg(VENDOR_SPACE::parcelable_getImsCfg* _aidl_return) {
    RLOGD("%s", __func__);
    char* result = NULL;
    int32_t tzValid = 0;
    int32_t ret = -1;
    Parcel p;
    status_t res  = OK;
    Parcel *pResponse;

    int token = mAidlAdapter->generateToken();
    RLOGD("%s token %d", __func__, token);

    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_GET_IMSCFG);
    status_t status = p.writeInt32(RIL_REQUEST_GET_IMSCFG);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        _aidl_return->ril_err = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p,mSlotId);
        res = mAidlAdapter->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("getImsRegState ps wait Response timeout\n");
                mAidlAdapter->removePendingRequest(token);
            }
        }else {
            ret = mAidlAdapter->responseErr;
            if(ret == OK){
                mAidlAdapter->parcelResponse.setDataPosition(0);
                pResponse = &mAidlAdapter->parcelResponse;
                if (pResponse) {
                    result = RpDataUtils::strdupReadString(pResponse);
                }
            }
        }
    }

    RLOGD("%s  result: %s", __func__, (result == NULL ? std::string("").c_str() : std::string(result)).c_str());
    _aidl_return->ril_err = ret;
    _aidl_return->result = (result == NULL ? std::string("") : std::string(result));
    if(result) {
        RpDataUtils::freeMemory(result);
    }
    return ::ndk::ScopedAStatus::ok();
}