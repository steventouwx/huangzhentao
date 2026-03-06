// SPDX-License-Identifier: MediaTekProprietary
#include <cstdint>
#include <mtk_log.h>
#include "RpDataUtils.h"
#include "radio_modem_response.h"


#define LOG_TAG "RadioModemResponse"
// // Initialize the static instance pointer to nullptr
// RadioModemResponse* RadioModemResponse::instance = nullptr;

RadioModemResponse::RadioModemResponse(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    mDispatchThread = dispatchThread;
    // Initialization code (if needed)
}

// // Static method to get the Singleton instance
// RadioModemResponse* RadioModemResponse::getInstance() {
//     if (!instance) {
//         instance = new RadioModemResponse();
//     }
//     return instance;
// }

// // Destructor implementation
// RadioModemResponse::~RadioModemResponse() {
//     // Cleanup code (if needed)
// }

ndk::ScopedAStatus RadioModemResponse::acknowledgeRequest(int32_t serial)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::enableModemResponse(const RadioResponseInfo & info)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::getBasebandVersionResponse(
        const RadioResponseInfo &info, const std::string &version)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::getDeviceIdentityResponse(
        const RadioResponseInfo &info, const std::string &imei,
        const std::string &imeisv, const std::string &esn,
        const std::string &meid)
{
    RLOGD("getDeviceIdentityResponse(%d): response: info = %s",mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    if(info.error != RadioError::NONE) {
        p.writeInt32(0);
    } else {
        p.writeInt32(4);
        RpDataUtils::writeStringToParcel(&p, imei.c_str());
        RpDataUtils::writeStringToParcel(&p, imeisv.c_str());
        RpDataUtils::writeStringToParcel(&p, esn.c_str());
        RpDataUtils::writeStringToParcel(&p, meid.c_str());
    }
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::getImeiResponse(
        const RadioResponseInfo &info, const std::optional<ImeiInfo> &config)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::getHardwareConfigResponse(
        const RadioResponseInfo &info,
        const std::vector<HardwareConfig> &config)
        {
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::getModemActivityInfoResponse(
        const RadioResponseInfo &info, const ActivityStatsInfo &activityInfo)
        {
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::getModemStackStatusResponse(
        const RadioResponseInfo &info, const bool enabled)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::getRadioCapabilityResponse(
        const RadioResponseInfo &info, const RadioCapability &rc)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::nvReadItemResponse(
        const RadioResponseInfo &info, const std::string &result)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::nvResetConfigResponse(
        const RadioResponseInfo &info)
{
    return ndk::ScopedAStatus().ok();
}
ndk::ScopedAStatus RadioModemResponse::nvWriteCdmaPrlResponse(
        const RadioResponseInfo &info)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::nvWriteItemResponse(
        const RadioResponseInfo &info)
{
    return ndk::ScopedAStatus().ok();
}
ndk::ScopedAStatus RadioModemResponse::requestShutdownResponse(
        const RadioResponseInfo &info)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::sendDeviceStateResponse(
        const RadioResponseInfo &info)
{
    return ndk::ScopedAStatus().ok();
}

ndk::ScopedAStatus RadioModemResponse::setRadioCapabilityResponse(
        const RadioResponseInfo &info, const RadioCapability &rc)
{
    return ndk::ScopedAStatus().ok();
}
ndk::ScopedAStatus RadioModemResponse::setRadioPowerResponse(
        const RadioResponseInfo &info)
{
    RLOGD("setRadioPowerResponse(%d): response: info = %s", mSlot, info.toString().c_str());
    Parcel p;
    p.writeInt32((int32_t)(info.type));
    p.writeInt32(info.serial);
    p.writeInt32((int32_t)(info.error));
    p.setDataPosition(0);
    mDispatchThread->enqueueResponseMessage(RADIO_TECH_GROUP_GSM, mSlot, info.serial, (int)info.error, &p);
    return ndk::ScopedAStatus().ok();
}
