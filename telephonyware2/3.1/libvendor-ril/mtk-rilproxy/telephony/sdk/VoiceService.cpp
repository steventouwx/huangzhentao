// SPDX-License-Identifier: MediaTekProprietary

#include <cstdint>
#include <cstring>
#include <mtk_log.h>
#define LOG_TAG "VOICE_SERVICE"

#include "VoiceService.h"
#include "RpAidlAdapter.h"
#include "RpDataUtils.h"
#include "../include/telephony/ril.h"
#include <prop/properties.h>

using namespace ::aidl::vendor::mediatek::hardware::voice;
namespace VENDOR_SPACE = ::aidl::vendor::mediatek::hardware::tbox;

extern Mutex gRequestLock;

namespace {
// controlType for RIL_REQUEST_HOLD_CALL: 1=hold, 2=resume(unhold)
constexpr int32_t HOLD_CALL = 1;
constexpr int32_t RESUME_CALL = 2;
}  // namespace

::ndk::ScopedAStatus VoiceService::svVoiceInit(int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceDeinit(int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    mCallCallback.reset();
    mDtmfCallback.reset();
    mEcallEventCallback.reset();
    mEcallStatusCallback.reset();
    mServiceErrorCallback.reset();
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceDial(const std::string& num, int32_t len,
                                               int32_t* id, int32_t* _aidl_return) {
    RLOGD("%s num=%s len=%d slot %d", __FUNCTION__, num.c_str(), len, mSlotId);
    if (!mTeleService || !mAidlAdapter) {
        *_aidl_return = -1;
        *id = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    ::ndk::ScopedAStatus status = mTeleService->startVcall(num, 0, &ret);
    if (!status.isOk() || ret != 0) {
        *_aidl_return = ret;
        *id = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    VENDOR_SPACE::parcelable_getCurrentCall callInfo;
    mTeleService->getCurrentCall(&callInfo);
    *id = callInfo.ecallMsg.call_id;
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceHangupAll(int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->endVcall(_aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceAnswer(int32_t id, int32_t* _aidl_return) {
    RLOGD("%s id=%d slot %d", __FUNCTION__, id, mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->answerVcall(_aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceHangup(int32_t id, int32_t* _aidl_return) {
    RLOGD("%s id=%d slot %d", __FUNCTION__, id, mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->endVcallId(id, _aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceHold(int32_t id, int32_t* _aidl_return) {
    RLOGD("%s id=%d slot %d", __FUNCTION__, id, mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->controlCall(HOLD_CALL, id, _aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceUnhold(int32_t id, int32_t* _aidl_return) {
    RLOGD("%s id=%d slot %d", __FUNCTION__, id, mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->controlCall(RESUME_CALL, id, _aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceGetRecords(VoiceRecordArray* records,
                                                     int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    if (!mTeleService || !records) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    VENDOR_SPACE::parcelable_getCurrentCall callInfo;
    mTeleService->getCurrentCall(&callInfo);
    records->len = 0;
    records->records.clear();
    if (callInfo.ecallMsg.call_id >= 0) {
        records->records.resize(1);
        records->records[0].id = callInfo.ecallMsg.call_id;
        records->records[0].number = callInfo.ecallMsg.number;
        records->records[0].state = static_cast<VoiceCallState>(callInfo.ecallMsg.state);
        records->records[0].tech = VoiceCallTech::TECH_3GPP;
        records->records[0].dir = callInfo.ecallMsg.isMT ?
            VoiceCallDirection::DIR_MT : VoiceCallDirection::DIR_MO;
        records->records[0].endReason = 0;
        records->records[0].callType = VoiceCallType::VOICE;
        records->records[0].callAlertingType = VoiceCallAlertingType::LOCAL;
        records->records[0].sipErrorCode = 0;
        records->len = 1;
    }
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceForwarding(int32_t reg,
                                                    VoiceForwardingCondition cond,
                                                    const std::string& num, int32_t len,
                                                    int32_t* _aidl_return) {
    RLOGD("%s reg=%d cond=%d slot %d", __FUNCTION__, reg, static_cast<int>(cond), mSlotId);
    if (!mAidlAdapter) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    Parcel p;
    int token = mAidlAdapter->generateToken();
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_SET_CALL_FORWARD);
    status_t status = p.writeInt32(RIL_REQUEST_SET_CALL_FORWARD);
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t cfStatus = reg ? 3 : 0;  // 3=registration, 0=disable
    status = p.writeInt32(cfStatus);
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(static_cast<int32_t>(cond));
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);  // serviceClass: 1=voice
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(0);  // toa: type of address
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, num.c_str());
    status = p.writeInt32(0);  // timeSeconds
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p, mSlotId);
        status_t res = mAidlAdapter->wait();
        if (res == android::OK) {
            ret = mAidlAdapter->responseErr;
        } else if (res == android::TIMED_OUT) {
            mAidlAdapter->removePendingRequest(token);
        }
    }
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceGetForwardingStatus(
    VoiceForwardingCondition cond, VoiceForwardingStatus* status_out, int32_t* _aidl_return) {
    RLOGD("%s cond=%d slot %d", __FUNCTION__, static_cast<int>(cond), mSlotId);
    if (!mAidlAdapter || !status_out) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    Parcel p;
    int token = mAidlAdapter->generateToken();
    mAidlAdapter->addPendingRequest(token, RIL_REQUEST_QUERY_CALL_FORWARD_STATUS);
    status_t status = p.writeInt32(RIL_REQUEST_QUERY_CALL_FORWARD_STATUS);
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(token);
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(2);  // status: 2=interrogate
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(static_cast<int32_t>(cond));
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(1);  // serviceClass: 1=voice
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    status = p.writeInt32(0);  // toa
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    RpDataUtils::writeStringToParcel(&p, "");
    status = p.writeInt32(0);  // timeSeconds
    if (status != android::OK) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    {
        Mutex::Autolock autoLock(gRequestLock);
        mAidlAdapter->sendRequest(p, mSlotId);
        status_t res = mAidlAdapter->wait();
        if (res == android::OK) {
            ret = mAidlAdapter->responseErr;
            status_out->enabled = 0;
            status_out->len = 0;
            status_out->details.clear();
            if (ret == 0 && mAidlAdapter->parcelResponse.dataAvail() > 0) {
                mAidlAdapter->parcelResponse.setDataPosition(0);
                int32_t numResponses = 0;
                mAidlAdapter->parcelResponse.readInt32(&numResponses);
                for (int i = 0; i < numResponses && i < 13; i++) {
                    int32_t cfStatus, reason, serviceClass;
                    mAidlAdapter->parcelResponse.readInt32(&cfStatus);
                    mAidlAdapter->parcelResponse.readInt32(&reason);
                    mAidlAdapter->parcelResponse.readInt32(&serviceClass);
                    int32_t dummy;
                    mAidlAdapter->parcelResponse.readInt32(&dummy);  // toa
                    char* num = RpDataUtils::strdupReadString(&mAidlAdapter->parcelResponse);
                    mAidlAdapter->parcelResponse.readInt32(&dummy);  // timeSeconds
                    if (cfStatus == 1) {
                        status_out->enabled = 1;
                    }
                    if (num && strlen(num) > 0) {
                        status_out->details.resize(status_out->len + 1);
                        status_out->details[status_out->len].type = VoiceForwardingType::VOICE;
                        status_out->details[status_out->len].number = num;
                        status_out->len++;
                    }
                    free(num);
                }
            }
        } else if (res == android::TIMED_OUT) {
            mAidlAdapter->removePendingRequest(token);
        }
    }
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceWaiting(int32_t enable, int32_t* _aidl_return) {
    RLOGD("%s enable=%d slot %d", __FUNCTION__, enable, mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->setCallWaiting(enable != 0, 1, _aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceGetWaitingStatus(int32_t* enabled,
                                                           int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    if (!mTeleService || !enabled) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    VENDOR_SPACE::parcelable_getCallWaiting result;
    ::ndk::ScopedAStatus status = mTeleService->getCallWaiting(1, &result);
    *_aidl_return = result.ril_err;
    *enabled = result.enable ? 1 : 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceAutoanswer(int32_t enable, int32_t sec,
                                                    int32_t* _aidl_return) {
    RLOGD("%s enable=%d sec=%d slot %d", __FUNCTION__, enable, sec, mSlotId);
    if (enable) {
        property_set("ro.telephony.call.autocall", "1");
    } else {
        property_set("ro.telephony.call.autocall", "0");
    }
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceSendDtmfChar(int32_t id, const std::string& c,
                                                       int32_t* _aidl_return) {
    RLOGD("%s id=%d c=%s slot %d", __FUNCTION__, id, c.c_str(), mSlotId);
    if (!mTeleService || c.empty()) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->startDtmf(c, _aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceSetCallCb(
    const std::shared_ptr<IVoiceCallCallback>& cb, int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    mCallCallback = cb;
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceSetDtmfCb(
    const std::shared_ptr<IVoiceDtmfCallback>& cb, int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    mDtmfCallback = cb;
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallDial(const VoiceEcallInfo& info,
                                                    int32_t* _aidl_return) {
    RLOGD("%s type=%d slot %d", __FUNCTION__, static_cast<int>(info.type), mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    if (info.type == VoiceEcallType::TEST && !info.testNumber.empty()) {
        VENDOR_SPACE::ECallSetNum setNum = {};
        setNum.arg_num = 2;
        setNum.type = static_cast<int32_t>(info.type);
        setNum.address = info.testNumber;
        mTeleService->setTestNumber(setNum, &ret);
        if (ret != 0) {
            *_aidl_return = ret;
            return ::ndk::ScopedAStatus::ok();
        }
    }
    int8_t ecallType = static_cast<int8_t>(info.type);
    return mTeleService->makeEcall(ecallType, _aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallHangup(int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    if (!mTeleService) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    return mTeleService->hangupEcall(_aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallUpdateMsd(const std::vector<uint8_t>& msd,
                                                         int32_t msdLen,
                                                         int32_t* _aidl_return) {
    RLOGD("%s msdLen=%d slot %d", __FUNCTION__, msdLen, mSlotId);
    if (!mTeleService || msd.empty()) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    VENDOR_SPACE::ECallSetMSD setMsd = {};
    setMsd.call_id = 0;
    setMsd.length = std::min(msdLen, static_cast<int32_t>(msd.size()));
    size_t copyLen = std::min(static_cast<size_t>(msdLen), msd.size());
    setMsd.msd_data.assign(reinterpret_cast<const char*>(msd.data()), copyLen);
    return mTeleService->setMSD(setMsd, _aidl_return);
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallPushMsd(VoiceEcallState* state,
                                                        int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    if (!mTeleService || !state) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    int32_t ret = -1;
    mTeleService->IVSPushMSD(&ret);
    if (state) {
        *state = VoiceEcallState::INACTIVE;
    }
    *_aidl_return = ret;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallGetConfig(VoiceEcallConfig* config,
                                                         int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    if (!config) {
        *_aidl_return = -1;
        return ::ndk::ScopedAStatus::ok();
    }
    memset(config, 0, sizeof(*config));
    config->mode = VoiceEcallMode::NORMAL;
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallSetConfig(int32_t item,
                                                        const VoiceEcallConfig& config,
                                                        int32_t* _aidl_return) {
    RLOGD("%s item=%d slot %d", __FUNCTION__, item, mSlotId);
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallSetEventCb(
    const std::shared_ptr<IVoiceEcallEventCallback>& cb, int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    mEcallEventCallback = cb;
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceEcallSetStatusCb(
    const std::shared_ptr<IVoiceEcallStatusCallback>& cb, int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    mEcallStatusCallback = cb;
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus VoiceService::svVoiceSetServiceErrorCb(
    const std::shared_ptr<IVoiceServiceErrorCallback>& cb, int32_t* _aidl_return) {
    RLOGD("%s slot %d", __FUNCTION__, mSlotId);
    mServiceErrorCallback = cb;
    *_aidl_return = 0;
    return ::ndk::ScopedAStatus::ok();
}
