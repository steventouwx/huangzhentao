// SPDX-License-Identifier: MediaTekProprietary

#ifndef RP_NW_ADAPTER
#define RP_NW_ADAPTER

//#include <utils/Looper.h>
//#include <utils/Thread.h>
//#include <utils/RefBase.h>
#include <condition_variable>
#include <utils/Mutex.h>
#include <utils/Condition.h>
#include <unordered_set>
#include <utils/Errors.h>
#include <map>
#include "RilSdkClient.h"
#include "../../include/telephony/ril.h"
#include <android/binder_ibinder.h>
#include <android/binder_parcel.h>
#include <android/binder_status.h>
//#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include <android/binder_parcel_utils.h>
#include "android/binder_manager.h"
#include "android/binder_process.h"

using ::android::RefBase;

using ::android::Parcel;
using namespace std;
using ::android::sp;
using ::android::Condition;
using ::android::Mutex;
using ::android::OK;
using ::android::TIMED_OUT;
using android::status_t;

#define  MAX_SLOT_NUM 2

extern "C" const char * requestToString(int request);

class RpAidlAdapter : public RefBase {
public:
    static RpAidlAdapter* sInstance;
    static RpAidlAdapter *getInstance();
    static void closeInstance();
    static int generateToken();
    static constexpr int DATARSP = 1;
    static constexpr int DATAIND = 2;
    static constexpr int INDEX_BASE = 1000;
    static constexpr int MAXCLIENT_NUM = 10;
    static int INDEX_TELE;
    static int INDEX_DATA;


    static std::mutex mtx;
    static std::condition_variable cv;
    static bool rilAidlThreadPoolCompleted;

    static const char* teleserviceNames[MAX_SLOT_NUM];
    static const char* dataserviceNames[MAX_SLOT_NUM];
    static const char* voiceserviceNames[MAX_SLOT_NUM];
    void registerService();
    int32_t sendRequest(Parcel& dataParcel, int slotId);
    void onRequestComplete(int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void onUnsolicitedResponse(int slotId, int unsolResponse, const void *data, size_t datalen);
    std::string convertCharPtrToAidlString(const char *ptr);
    void deathRecipientCallback(void* cookie);
    int32_t getDataCallInfo(int profileId, MTK_Data_Call_Response_v1 *dataCallRep);
    bool IsAsyncRequest(int request);

    RilSdkClient * mSdkClient;

    //wait response complete
    status_t wait();
    void wakeup();
    nsecs_t mRequestTimeout = 5e9; //5s
    int setTimeOut(nsecs_t time);
    void resetTimeout();

    //wait configureRpcThreadpool done
    status_t startupWait();
    void startupWakeup();
    Mutex mTeleDataRespMutex;
    Mutex mTeleServiceClientMapMutex;
    Mutex mTeleDataClientMapMutex;

    void addPendingRequest(int token, int requestId);
    void removePendingRequest(int token);
    bool isRequestPending(int token, int *requestId);

    void addPendingProfileId(int token, int profileId);
    void removePendingProfileId(int token);
    bool isProfileIdPending(int token, int *profileId);

    //record response to parcel
    Parcel parcelResponse;
    int responseErr;

    std::map<int, int> mPendingRequests; //token ~ request id
    std::map<int, int> mPendingProfileId; //profile id  ~ token

    int mTeleClientAlive[MAXCLIENT_NUM*2];
    int mDataClientAlive[MAXCLIENT_NUM*2];
    int getFreeTeleClientIndex(int *clientIdx);
    int getFreeDataClientIndex(int *clientIdx);
private:
    RpAidlAdapter();
    virtual ~RpAidlAdapter();

    void init();
    void deInit();
    Mutex m_RespMutex;
    Condition m_RespCondition;
    Mutex m_startupMutex;
    Condition m_startupCondition;

    static Mutex m_reqMapMutex;

    static Mutex token_mutex;
    static int tokenCounter;
    static int defaultAtSlot;
    static bool isLockRelease;
    static bool isStartLockRelease;
    static const std::unordered_set<int> allAsyncRequests;

};
#endif  // RP_NW_ADAPTER
