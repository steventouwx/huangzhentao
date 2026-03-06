// SPDX-License-Identifier: MediaTekProprietary

#ifndef VENDOR_MEDIATEK_HARDWARE_TBOX_TELEDATA_H
#define VENDOR_MEDIATEK_HARDWARE_TBOX_TELEDATA_H

#include <aidl/vendor/mediatek/hardware/tbox/BnTeleData.h>
#include <aidl/vendor/mediatek/hardware/tbox/ITeleDataResponse.h>
#include <aidl/vendor/mediatek/hardware/tbox/ITeleDataIndication.h>
#include <aidl/vendor/mediatek/hardware/tbox/KeepaliveRequest.h>
#include <aidl/vendor/mediatek/hardware/tbox/KeepaliveStatus.h>
#include <android/binder_parcel.h>
#include <android/binder_status.h>
#include "RpAidlAdapter.h"

namespace VENDOR_SPACE = ::aidl::vendor::mediatek::hardware::tbox;

struct TeleData : public VENDOR_SPACE::BnTeleData {
    std::shared_ptr<VENDOR_SPACE::ITeleDataResponse> mDataResponse;
    std::shared_ptr<VENDOR_SPACE::ITeleDataIndication> mDataIndication;
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleDataResponse>> mapDataClientResponses;
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleDataIndication>> mapDataClientIndications;
    // Methods from VENDOR_SPACE::ITeleData follow.
    ::ndk::ScopedAStatus deactiveDataCall(int32_t in_profileid, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setResponseFunctions(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleDataResponse>& in_dataResponseParam, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleDataIndication>& in_dataIndicationParam);
    ::ndk::ScopedAStatus setupDataCall(int32_t in_profileId, int32_t* _aidl_return);
    ::ndk::ScopedAStatus startKeepalive(const VENDOR_SPACE::KeepaliveRequest& in_keepalive, VENDOR_SPACE::KeepaliveStatus* _aidl_return);
    ::ndk::ScopedAStatus stopKeepalive(int32_t in_sessionHandle, int32_t* _aidl_return);
    RpAidlAdapter  * mAidlAdapter;
    int mLastToken;
    int mSlotId;
};

#endif  // VENDOR_MEDIATEK_HARDWARE_TBOX_TELEDATA_H
