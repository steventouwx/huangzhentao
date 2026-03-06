// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <android/binder_manager.h>
#include <mtk_log.h>
#include "mtkradioex_data.h"
#include "radio_data_response.h"
#include "radio_data_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "MtkRadioExData"
// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio data service has died!");
}

MtkRadioExData::MtkRadioExData(int slot, RfxDispatchThread* dispatchThread) {

    mSlot = slot;
    std::string name = std::string() + IMtkRadioExData::descriptor + portNames[slot];
    RLOGD("init MtkRadioExData(%d), name = %s", slot, name.c_str());
    mtkradioex_data = IMtkRadioExData::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if(mtkradioex_data.get() == nullptr) {
        RLOGE("Failed to get mtk radio data service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_data->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio data service binder");
        }
    }

    mtkradioexRsp_data = ndk::SharedRefBase::make<MtkRadioExDataResponse>(slot, dispatchThread);
    if(mtkradioexRsp_data.get() == nullptr) {
        RLOGE("Failed to create mtk radio data response");
    }


    mtkradioexInd_data = ndk::SharedRefBase::make<MtkRadioExDataIndication>(slot, dispatchThread);
    if(mtkradioexInd_data.get() == nullptr) {
        RLOGE("Failed to create mtk radio data indication");
    }

    mtkradioexRsp_assist = ndk::SharedRefBase::make<MtkRadioExAssistResponse>(slot, dispatchThread);
    if (mtkradioexRsp_assist.get() == nullptr) {
        RLOGE("Failed to create MtkRadioExAssistResponse");
    }

    if (mtkradioex_data && (mtkradioex_data.get() != nullptr))  {
        mtkradioex_data->setResponseFunctionsMtk(mtkradioexRsp_data, mtkradioexInd_data);
        mtkradioex_data->setResponseFunctionsAssist(mtkradioexRsp_assist);
    } else {
        RLOGE("mtkradioex_data is null");
    }

}

int32_t MtkRadioExData::syncDataSettingsToMd(int32_t serial, const std::vector<int32_t>& settings, int32_t clientId) {
    RLOGD("syncDataSettingsToMd start, slotId = %d, serial = %d", mSlot, serial);

    if (mtkradioex_data.get() == nullptr) {
        RLOGE("mtkradioex_data.get() is null");
        return -1;
    }

    ndk::ScopedAStatus ret = mtkradioex_data->syncDataSettingsToMd(serial, settings, clientId);
    if (!ret.isOk()) {
        RLOGE("syncDataSettingsToMd failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}