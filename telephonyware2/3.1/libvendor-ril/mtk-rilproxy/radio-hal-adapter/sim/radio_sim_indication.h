// SPDX-License-Identifier: MediaTekProprietary
#pragma once

#include <aidl/android/hardware/radio/sim/BnRadioSimIndication.h>
#include <aidl/android/hardware/radio/sim/BnRadioSimResponse.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>

#include "RfxDispatchThread.h"

using namespace aidl::android::hardware::radio;
using namespace aidl::android::hardware::radio::sim;

class RadioSimIndication : public BnRadioSimIndication {
public:
    RadioSimIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus carrierInfoForImsiEncryption(RadioIndicationType info) override;

    virtual ndk::ScopedAStatus cdmaSubscriptionSourceChanged(
            RadioIndicationType type, CdmaSubscriptionSource cdmaSource) override;

    virtual ndk::ScopedAStatus simPhonebookChanged(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus simPhonebookRecordsReceived(
            RadioIndicationType type, PbReceivedStatus status,
            const std::vector<PhonebookRecordInfo>& records) override;

    virtual ndk::ScopedAStatus simRefresh(RadioIndicationType type,
                                          const SimRefreshResult& refreshResult) override;

    virtual ndk::ScopedAStatus simStatusChanged(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus stkEventNotify(RadioIndicationType type,
                                              const std::string& cmd) override;

    virtual ndk::ScopedAStatus stkProactiveCommand(RadioIndicationType type,
                                                   const std::string& cmd) override;

    virtual ndk::ScopedAStatus stkSessionEnd(RadioIndicationType type) override;

    virtual ndk::ScopedAStatus subscriptionStatusChanged(RadioIndicationType type,
                                                         bool activate) override;

    virtual ndk::ScopedAStatus uiccApplicationsEnablementChanged(RadioIndicationType type,
                                                                 bool enabled) override;
};