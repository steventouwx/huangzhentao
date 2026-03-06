// SPDX-License-Identifier: MediaTekProprietary

#ifndef SERVICE_TELEPHONY_MTK_RILPROXY_TELEPHONY_SDK_TELEDEATHREIPIENT_H_
#define SERVICE_TELEPHONY_MTK_RILPROXY_TELEPHONY_SDK_TELEDEATHREIPIENT_H_

#include <android/binder_auto_utils.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <android/binder_status.h>
#include <android/binder_ibinder.h>
#include <android/binder_parcel.h>

using ndk::SpAIBinder;

class TeleDeathReipient  : public AIBinder_DeathRecipient {
public:
    TeleDeathReipient();
    virtual ~TeleDeathReipient();
private:
    virtual void serviceDied(uint64_t cookie, const wp<IBinder>& who);
};

#endif /* SERVICE_TELEPHONY_MTK_RILPROXY_TELEPHONY_SDK_TELEDEATHREIPIENT_H_ */
