// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <android/binder_ibinder.h>
#include <mtk_log.h>
#include "radio_data.h"
#include "radio_data_response.h"
#include "radio_data_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "RadioData"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The radio data service has died!");
}

RadioData::RadioData(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IRadioData::descriptor + portNames[slot];
    RLOGD("init RadioData(%d), name = %s", slot, name.c_str());
    radio_data = IRadioData::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if (radio_data.get() == nullptr) {
        RLOGE("Failed to get radio data service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = radio_data->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the radio data service binder");
        }
    }

    radioRsp_data = ndk::SharedRefBase::make<RadioDataResponse>(slot,dispatchThread);
    if(radioRsp_data.get() == nullptr) {
        RLOGE("Failed to create radio data response");
    }
    radioInd_data = ndk::SharedRefBase::make<RadioDataIndication>(slot,dispatchThread);
        if(radioInd_data.get() == nullptr) {
        RLOGE("Failed to create radio data indication");
    }

    if (radio_data && (radio_data.get() != nullptr))  {
        radio_data->setResponseFunctions(radioRsp_data, radioInd_data);
    } else {
        RLOGE("radio_data is null");
    }

}

int32_t RadioData::deactivateDataCall(int32_t in_serial, int32_t in_cid, ::aidl::android::hardware::radio::data::DataRequestReason in_reason) {
    RLOGD("deactivateDataCall start, slotId = %d, serial = %d, cid = %d, reason = %d", mSlot, in_serial, in_cid, in_reason);
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->deactivateDataCall(in_serial, in_cid, in_reason);
    if (!ret.isOk()) {
        RLOGE("deactivateDataCall failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioData::getDataCallList(int32_t in_serial) {
    RLOGD("getDataCallList start, slotId = %d, serial = %d", mSlot, in_serial);
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->getDataCallList(in_serial);
    if (!ret.isOk()) {
        RLOGE("getDataCallList failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioData::setDataAllowed(int32_t in_serial, bool in_allow) {
    RLOGD("setDataAllowed start, slotId = %d, serial = %d, allow = %d", mSlot, in_serial, in_allow);
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->setDataAllowed(in_serial, in_allow);
    if (!ret.isOk()) {
        RLOGE("setDataAllowed failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioData::setDataProfile(int32_t in_serial, const std::vector<::aidl::android::hardware::radio::data::DataProfileInfo>& in_profiles){
    RLOGD("setDataProfile start, slotId = %d, serial = %d", mSlot, in_serial);
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->setDataProfile(in_serial, in_profiles);
    if (!ret.isOk()) {
        RLOGE("setDataProfile failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioData::setInitialAttachApn(int32_t in_serial, const std::optional<::aidl::android::hardware::radio::data::DataProfileInfo>& in_dataProfileInfo) {
    RLOGD("setInitialAttachApn start, slotId = %d, serial = %d", mSlot, in_serial);
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->setInitialAttachApn(in_serial, in_dataProfileInfo);
    if (!ret.isOk()) {
        RLOGE("setInitialAttachApn failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioData::setupDataCall(int32_t in_serial,
        ::aidl::android::hardware::radio::AccessNetwork in_accessNetwork,
        const ::aidl::android::hardware::radio::data::DataProfileInfo& in_dataProfileInfo,
        bool in_roamingAllowed,
        ::aidl::android::hardware::radio::data::DataRequestReason in_reason,
        const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& in_addresses,
        const std::vector<std::string>& in_dnses, int32_t in_pduSessionId,
        const std::optional<::aidl::android::hardware::radio::data::SliceInfo>& in_sliceInfo,
        bool in_matchAllRuleAllowed) {
    RLOGD("setupDataCall start, slotId = %d, serial = %d, in_accessNetwork =%d,in_dataProfileInfo = %s, in_roamingAllowed = %d",
        mSlot, in_serial, in_accessNetwork, in_dataProfileInfo.toString().c_str(), in_roamingAllowed);
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->setupDataCall(in_serial,
        in_accessNetwork,
        in_dataProfileInfo,
        in_roamingAllowed,
        in_reason,
        in_addresses,
        in_dnses,
        in_pduSessionId,
        in_sliceInfo,
        in_matchAllRuleAllowed);
    if (!ret.isOk()) {
        RLOGE("setupDataCall failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioData::startKeepalive(int32_t in_serial, const ::aidl::android::hardware::radio::data::KeepaliveRequest& in_keepalive) {
    RLOGD("startKeepalive start, slotId = %d, serial = %d, in_keepalive = %s", mSlot, in_serial, in_keepalive.toString().c_str());
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->startKeepalive(in_serial, in_keepalive);
    if (!ret.isOk()) {
        RLOGE("startKeepalive failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}

int32_t RadioData::stopKeepalive(int32_t in_serial, int32_t in_sessionHandle) {
    RLOGD("stopKeepalive start, slotId = %d, serial = %d, in_sessionHandle = %d", mSlot, in_serial, in_sessionHandle);
    if (radio_data == nullptr) {
        RLOGE("radio_data is null");
        return -1;
    }

    if (radio_data.get() == nullptr) {
        RLOGE("radio_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = radio_data->stopKeepalive(in_serial, in_sessionHandle);
    if (!ret.isOk()) {
        RLOGE("stopKeepalive failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}