// SPDX-License-Identifier: MediaTekProprietary

#include <iostream>
#include <android/binder_ibinder.h>
#include <android/binder_parcel.h>
#include <android/binder_status.h>
#include <android/binder_parcel_utils.h>
#include "android/binder_manager.h"
#include "android/binder_process.h"

#include "aidl/vendor/mediatek/hardware/tbox/BnTeleData.h"
#include <aidl/vendor/mediatek/hardware/tbox/ITeleData.h>
#include <aidl/vendor/mediatek/hardware/tbox/ITeleDataResponse.h>
#include <aidl/vendor/mediatek/hardware/tbox/ITeleDataIndication.h>
#include "aidl/vendor/mediatek/hardware/tbox/SetupDataCallResult.h"

using namespace aidl::vendor::mediatek::hardware::tbox;

class MyService : public ::aidl::vendor::mediatek::hardware::tbox::BnTeleData {
public:
    std::vector<std::shared_ptr<ITeleDataResponse>> myResponseList;
    std::vector<std::shared_ptr<ITeleDataIndication>> myIndicationList;

    MyService(){
        std::cout << "created MyService object." << std::endl;
    }

    ~MyService(){
        std::cout << "destroyed MyService object." << std::endl;
    }

    std::shared_ptr<ITeleData> getImpl() {
        std::cout << "getImpl called" << std::endl;
        std::shared_ptr<ITeleData> teleData;
        return teleData;
    }
    ::ndk::ScopedAStatus deactiveDataCall(int32_t in_profileid, int32_t* _aidl_return) {
        std::cout << "deactiveDataCall called,and profile:."<<in_profileid<<std::endl;
        *_aidl_return = 97;
        return ::ndk::ScopedAStatus::ok();
    }
    ::ndk::ScopedAStatus setResponseFunctions(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleDataResponse>& in_dataResponseParam, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleDataIndication>& in_dataIndicationParam) {
        std::cout << "setResponseFunctions called" << std::endl;

        for (auto& it : myResponseList) {
            if (it->asBinder().get() == in_dataResponseParam->asBinder().get()) {
                std::cout << "found response function" << std::endl;
                return ::ndk::ScopedAStatus::fromExceptionCode(EX_NONE);
            }
        }

        myResponseList.push_back(in_dataResponseParam);

        for (auto& it : myIndicationList) {
            if (it->asBinder().get() == in_dataIndicationParam->asBinder().get()) {
                std::cout << "found indication function" << std::endl;
                return ::ndk::ScopedAStatus::fromExceptionCode(EX_NONE);

            }
        }

        myIndicationList.push_back(in_dataIndicationParam);
        std::cout << "setResponseFunctions called done." << std::endl;
        return ::ndk::ScopedAStatus::ok();
    }
    ::ndk::ScopedAStatus setupDataCall(int32_t in_profileId, int32_t* _aidl_return) {
        std::cout << "setupDataCall called" << std::endl;
        SetupDataCallResult myServiceResult;
        myServiceResult.profileId = 1122;
        std::cout << "setupDataCall called pass here." << std::endl;

        for (auto& it : myResponseList) {
            std::cout << "[service]setupDataCallResponse called" << std::endl;
            it->setupDataCallResponse(0, myServiceResult);
        }
        myServiceResult.profileId = 1133;
        for (auto& it : myIndicationList) {
            std::cout << "[service]dataStatusChanged called" << std::endl;
            it->dataStatusChanged(myServiceResult);
        }

        *_aidl_return = 0;
        return ::ndk::ScopedAStatus::ok();
    }
};

int main() {
#if 0
    std::shared_ptr<MyService> myService_instance = new MyService();
    sp<IServiceManager> sm = defaultServiceManager();
    sm->addService(String16("my.service"), myService_instance, false, IServiceManager::DUMP_FLAG_PRIORITY_DEFAULT);
    std::cout << "service ready!!!" << std::endl;
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    std::cout << "service stopped!!!" << std::endl;
#endif
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    ABinderProcess_startThreadPool();
    // make a default vibrator service
    std::shared_ptr<MyService> myService_instance = ndk::SharedRefBase::make<MyService>();
    binder_status_t status = AServiceManager_addService(myService_instance->asBinder().get(), "my.service");

    ABinderProcess_joinThreadPool();

    return 0;
}
