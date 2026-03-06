// SPDX-License-Identifier: MediaTekProprietary

#include <iostream>
#include <unistd.h>
#include <android/binder_ibinder.h>
#include <android/binder_parcel.h>
#include <android/binder_status.h>
#include <android/binder_parcel_utils.h>
#include "android/binder_manager.h"
#include "android/binder_process.h"
#include "aidl/vendor/mediatek/hardware/tbox/ITeleData.h"
#include "aidl/vendor/mediatek/hardware/tbox/SetupDataCallResult.h"
#include "aidl/vendor/mediatek/hardware/tbox/BnTeleDataResponse.h"
#include "aidl/vendor/mediatek/hardware/tbox/BnTeleDataIndication.h"

using namespace aidl::vendor::mediatek::hardware::tbox;

class MyClientResponse : public ::aidl::vendor::mediatek::hardware::tbox::BnTeleDataResponse {
    public:
      MyClientResponse(){};
      ~MyClientResponse(){};
        ::ndk::ScopedAStatus setupDataCallResponse(int32_t in_ril_err, const ::aidl::vendor::mediatek::hardware::tbox::SetupDataCallResult& in_dcResponse) {
            printf("Got response from server!\n");
            printf("DC Response profileId: %d\n", in_dcResponse.profileId);
            return ::ndk::ScopedAStatus::ok();
        }
};
class MyClientIndication : public ::aidl::vendor::mediatek::hardware::tbox::BnTeleDataIndication {
    public:
        MyClientIndication(){};
        ~MyClientIndication(){};
        ::ndk::ScopedAStatus dataStatusChanged(const ::aidl::vendor::mediatek::hardware::tbox::SetupDataCallResult& in_dc) {
            printf("Got indication from server!\n");
            printf("DC indication profileId: %d\n", in_dc.profileId);
            return ::ndk::ScopedAStatus::ok();
        }
};

int main() {
#if 0
    sp<ProcessState> proc(ProcessState::self());
    ProcessState::self()->startThreadPool();
    printf("Starting client...\n");

    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder_client = sm->getService(String16("my.service"));
    if (binder_client == 0) {
        printf("Failed to get service\n");
        exit(-1);
        return -1;
    }
#endif
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    ABinderProcess_startThreadPool();
    //::ndk::SpAIBinder binder(AServiceManager_checkService("my.service"));
    std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleData> teleDataClient = ITeleData::fromBinder(::ndk::SpAIBinder(AServiceManager_getService("my.service")));
    //std::shared_ptr<ITeleData> teleDataClient = ITeleData::fromBinder(binder);
    //std::shared_ptr<ITeleData> teleDataClient = interface_cast<ITeleData>(binder_client);
    if (teleDataClient == 0) {
        printf("Failed to cast to ITeleData\n");
        exit(-1);
        return -1;
    }
    printf("[1116]Successfully cast to ITeleData\n");
    printf("calling deactiveDataCall...\n");
    int32_t retval;
    if (teleDataClient->deactiveDataCall(1, &retval).isOk()) {
        printf("deactiveDataCall returned %d\n", retval);
    }

    std::shared_ptr<MyClientResponse> myTeleDataResponseCb = ndk::SharedRefBase::make<MyClientResponse>(); //new MyClientResponse();
    std::shared_ptr<MyClientIndication> myTeleDataIndicationCb = ndk::SharedRefBase::make<MyClientIndication>(); //new MyClientIndication();

    teleDataClient->deactiveDataCall(2, &retval);

    printf("calling setResponseFunctions...\n");
    ::ndk::ScopedAStatus ret = teleDataClient->setResponseFunctions(myTeleDataResponseCb, myTeleDataIndicationCb);
    if (ret.isOk()) {
        printf("setResponseFunctions returned OK!\n");
        } else {
            printf("setResponseFunctions failed!\n");
            }
    sleep(3);
    printf("[sleep 5]calling setupDataCall...\n");
    ret = teleDataClient->setupDataCall(1, &retval);
    if (ret.isOk()) {
        printf("setupDataCall returned OK!\n");
        } else {
            printf("setupDataCall failed!\n");
            }

    ABinderProcess_joinThreadPool();
    return 0;
}
