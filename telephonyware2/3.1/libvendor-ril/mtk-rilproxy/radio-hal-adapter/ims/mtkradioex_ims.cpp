// SPDX-License-Identifier: MediaTekProprietary
#include <android/binder_manager.h>
#include <android/binder_ibinder.h>
#include <mtk_log.h>
#include "mtkradioex_ims.h"
#include "mtkradioex_ims_response.h"
#include "mtkradioex_ims_indication.h"
#include "../radio_hal_utils.h"

#define LOG_TAG "MtkRadioExIms"

// Define a death recipient callback function
inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    RLOGE("The mtk radio ims service has died!");
}

MtkRadioExIms::MtkRadioExIms(int slot, RfxDispatchThread* dispatchThread) {
    mSlot = slot;
    std::string name = std::string() + IMtkRadioExIms::descriptor + imsPortNames[slot];
    RLOGD("init MtkRadioExIms(%d), name = %s", slot, name.c_str());
    mtkradioex_ims = IMtkRadioExIms::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(name.c_str())));
    if (mtkradioex_ims.get() == nullptr) {
        RLOGE("Failed to get mtk radio ims service");
    } else {
        // Set up 'link to death' to monitor if the service goes away
        auto binder = mtkradioex_ims->asBinder(); // Retrieve Native AIBinder from wrapper if needed
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);

        if (AIBinder_linkToDeath(binder.get(), deathRecipient, nullptr) != STATUS_OK) {
            RLOGE("Failed to link death observer to the mtk radio ims service binder");
        }
    }

    mtkradioexRsp_ims = ndk::SharedRefBase::make<MtkRadioExImsResponse>(slot, dispatchThread);
    if (mtkradioexRsp_ims.get() == nullptr) {
        RLOGE("Failed to create mtk radio ims response");
    }

    mtkradioexInd_ims = ndk::SharedRefBase::make<MtkRadioExImsIndication>(slot, dispatchThread);
    if (mtkradioexInd_ims.get() == nullptr) {
        RLOGE("Failed to create mtk radio ims indication");
    }

    if (mtkradioex_ims.get() != nullptr) {
        mtkradioex_ims->setResponseFunctionsMtk(mtkradioexRsp_ims, mtkradioexInd_ims);
    } else {
        RLOGE("mtkradioex_ims is null");
    }

}

int32_t MtkRadioExIms::setImscfg(int32_t in_serial, bool in_volteEnable,
                                 bool in_vilteEnable, bool in_vowifiEnable,
                                 bool in_viwifiEnable, bool in_smsEnable,
                                 bool in_eimsEnable, int32_t in_clientId) {
  RLOGD("setImscfg start, slotId = %d, serial = %d,clientId = %d, volteEnable "
        "= %d, vilteEnable = %d, vowifiEnable = %d, viwifiEnable = %d, "
        "smsEnable = %d, eimsEnable = %d",
        mSlot, in_serial, in_clientId, in_volteEnable, in_vilteEnable,
        in_vowifiEnable, in_viwifiEnable, in_smsEnable, in_eimsEnable);
  if (mtkradioex_ims.get() == nullptr) {
    RLOGE("Failed to get mtk radio modem service");
    return -1;
  }
  ndk::ScopedAStatus ret = mtkradioex_ims->setImscfg(
      in_serial, in_volteEnable, in_vilteEnable, in_vowifiEnable,
      in_viwifiEnable, in_smsEnable, in_eimsEnable, in_clientId);
  if (!ret.isOk()) {
    RLOGE("setImscfg failed!, slotId = %d", mSlot);
    return -1;
  }
  return 0;
}

int32_t MtkRadioExIms::controlCall(int32_t in_serial, int32_t controlType, int32_t callId, int32_t clientId) {
  RLOGD("controlCall start, slotId = %d , in_serial = %d, controlType = %d,callId = %d, clientId = %d ",
        mSlot, in_serial, controlType, callId, clientId);
  if (mtkradioex_ims.get() == nullptr) {
    RLOGE("Failed to get mtk radio modem service");
    return -1;
  }
  ndk::ScopedAStatus ret = mtkradioex_ims->controlCall(in_serial, controlType, callId, clientId);
  if (!ret.isOk()) {
    RLOGE("setImscfg failed!, slotId = %d", mSlot);
    return -1;
  }
  return 0;

}

int32_t MtkRadioExIms::getImscfg(int serial, int32_t in_clientId) {
    RLOGD("getImscfg start, slotId = %d, serial = %d, clientId = %d", mSlot, serial, in_clientId);
    if(mtkradioex_ims.get() == nullptr) {
        RLOGE("Failed to get mtk radio modem service");
        return -1;
    }
    ndk::ScopedAStatus ret = mtkradioex_ims->getImscfg(serial, in_clientId);
    if (!ret.isOk()) {
        RLOGE("getImscfg failed!, slotId = %d", mSlot);
        return -1;
    }
    return 0;
}