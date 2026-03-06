// SPDX-License-Identifier: MediaTekProprietary

#ifndef VENDOR_MEDIATEK_HARDWARE_VOICE_VOICESERVICE_H
#define VENDOR_MEDIATEK_HARDWARE_VOICE_VOICESERVICE_H

#include <aidl/vendor/mediatek/hardware/voice/BnVoiceService.h>
#include <aidl/vendor/mediatek/hardware/voice/IVoiceCallCallback.h>
#include <aidl/vendor/mediatek/hardware/voice/IVoiceDtmfCallback.h>
#include <aidl/vendor/mediatek/hardware/voice/IVoiceEcallEventCallback.h>
#include <aidl/vendor/mediatek/hardware/voice/IVoiceEcallStatusCallback.h>
#include <aidl/vendor/mediatek/hardware/voice/IVoiceServiceErrorCallback.h>
#include "RpAidlAdapter.h"
#include "TeleService.h"
#include <map>
#include <memory>

namespace VOICE_SPACE = ::aidl::vendor::mediatek::hardware::voice;

/**
 * @brief VoiceService - AIDL服务端实现类
 *
 * 继承 IVoiceService AIDL 接口，参照 TeleService 的实现模式，
 * 通过 RpAidlAdapter 和 TeleService 实现语音通话相关功能。
 */
struct VoiceService : public VOICE_SPACE::BnVoiceService {
    int32_t mSlotId;
    RpAidlAdapter* mAidlAdapter;
    std::shared_ptr<TeleService> mTeleService;

    std::shared_ptr<VOICE_SPACE::IVoiceCallCallback> mCallCallback;
    std::shared_ptr<VOICE_SPACE::IVoiceDtmfCallback> mDtmfCallback;
    std::shared_ptr<VOICE_SPACE::IVoiceEcallEventCallback> mEcallEventCallback;
    std::shared_ptr<VOICE_SPACE::IVoiceEcallStatusCallback> mEcallStatusCallback;
    std::shared_ptr<VOICE_SPACE::IVoiceServiceErrorCallback> mServiceErrorCallback;

    // IVoiceService 接口实现
    ::ndk::ScopedAStatus svVoiceInit(int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceDeinit(int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceDial(const std::string& num, int32_t len, int32_t* id,
                                     int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceHangupAll(int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceAnswer(int32_t id, int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceHangup(int32_t id, int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceHold(int32_t id, int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceUnhold(int32_t id, int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceGetRecords(VOICE_SPACE::VoiceRecordArray* records,
                                           int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceForwarding(int32_t reg,
                                          VOICE_SPACE::VoiceForwardingCondition cond,
                                          const std::string& num, int32_t len,
                                          int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceGetForwardingStatus(
        VOICE_SPACE::VoiceForwardingCondition cond,
        VOICE_SPACE::VoiceForwardingStatus* status,
        int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceWaiting(int32_t enable, int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceGetWaitingStatus(int32_t* enabled,
                                                 int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceAutoanswer(int32_t enable, int32_t sec,
                                           int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceSendDtmfChar(int32_t id, const std::string& c,
                                             int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceSetCallCb(
        const std::shared_ptr<VOICE_SPACE::IVoiceCallCallback>& cb,
        int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceSetDtmfCb(
        const std::shared_ptr<VOICE_SPACE::IVoiceDtmfCallback>& cb,
        int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallDial(const VOICE_SPACE::VoiceEcallInfo& info,
                                          int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallHangup(int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallUpdateMsd(const std::vector<uint8_t>& msd,
                                               int32_t msdLen,
                                               int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallPushMsd(VOICE_SPACE::VoiceEcallState* state,
                                             int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallGetConfig(VOICE_SPACE::VoiceEcallConfig* config,
                                              int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallSetConfig(int32_t item,
                                               const VOICE_SPACE::VoiceEcallConfig& config,
                                               int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallSetEventCb(
        const std::shared_ptr<VOICE_SPACE::IVoiceEcallEventCallback>& cb,
        int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceEcallSetStatusCb(
        const std::shared_ptr<VOICE_SPACE::IVoiceEcallStatusCallback>& cb,
        int32_t* _aidl_return) override;
    ::ndk::ScopedAStatus svVoiceSetServiceErrorCb(
        const std::shared_ptr<VOICE_SPACE::IVoiceServiceErrorCallback>& cb,
        int32_t* _aidl_return) override;
};

#endif  // VENDOR_MEDIATEK_HARDWARE_VOICE_VOICESERVICE_H
