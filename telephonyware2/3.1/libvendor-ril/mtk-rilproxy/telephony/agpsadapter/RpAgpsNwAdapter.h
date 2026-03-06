// SPDX-License-Identifier: MediaTekProprietary
/*
 * RpAgpsNwAdapter.h
 *
 *  Created on: Jun 9, 2017
 *      Author: Kaiduan Cao
 */

#ifndef MTK_RILPROXY_TELEPHONY_AGPSADAPTER_RPAGPSNWADAPTER_H_
#define MTK_RILPROXY_TELEPHONY_AGPSADAPTER_RPAGPSNWADAPTER_H_

#include <utils/Looper.h>
#include <utils/Thread.h>
#include <utils/RefBase.h>
#include <map>
#include <netinet/in.h>
#include <netdb.h>
//#include <resolv_netid.h>
#include "protocol/Agps2FrameworkInterface.h"
#include "protocol/Framework2AgpsInterface.h"
#include "protocol/mtk_socket_data_coder.h"
#include "RfxMessage.h"
#include "RpRilAgpsNwClient.h"

using namespace std;
using ::android::Looper;
using ::android::Thread;
using ::android::MessageHandler;
using ::android::Message;
using ::android::sp;

#define SOCKET_AGPS2FWK "mtk_agps2framework"
#define SOCKET_FWK2AGPS "mtk_framework2agps"

#define REQUEST_NETWORK_TIMEOUT 30000

typedef enum {
    MSG_TYPE_NONE,
    MSG_REQUEST_NET,
    MSG_QUERY_DNS,
    MSG_REQUEST_NET_TIMEOUT,
    MSG_REQUEST_NET_RESULT_FAIL,
    MSG_RELEASE_NET
} AGPS_NW_MESSAGE_TYPE;

struct AgpsNwRequestResult {
    bool isSuccess;
    int netId;
    int  type;       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                               For example, "IP" :1, "IPV6": 2, "IPV4V6" :3, or "PPP". If status is
                               PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED this is the type supported
                               such as "IP" or "IPV6" */
};

struct AgpsDnsRequestResult {
    bool isSuccess;
    bool hasIpv4;
    int ipv4;
    bool hasIpv6;
    char ipv6[16];
    int ipv6_size;
    bool hasNetId;
    int netId;
};

// Agps command handlers, handle the commands from Agpsd
void Agps2FrameworkInterface_isExist_handler();
void Agps2FrameworkInterface_acquireWakeLock_handler();
void Agps2FrameworkInterface_releaseWakeLock_handler();
void Agps2FrameworkInterface_requestDedicatedApnAndDnsQuery_handler(char* fqdn,
        bool isEmergencySupl, bool isApnEnabled);
void Agps2FrameworkInterface_releaseDedicatedApn_handler();
/**
 * when emergency call is dialed and location is requested from network
 * AGPSD will decide to send this message to framework to show
 * the GPS icon to meet the operator requirement
 */
void Agps2FrameworkInterface_requestGpsIcon_handler();
/**
 * if requestGpsIcon() was sent before, in the end of this AGPS session
 * AGPSD will send this message to framework to remove the GPS icon
 */
void Agps2FrameworkInterface_removeGpsIcon_handler();

void Agps2FrameworkInterface_requestC2kApn_handler();
void Agps2FrameworkInterface_releaseC2kApn_handler();
void Agps2FrameworkInterface_AgpsNiNotify_handler();

class RpAgpsNwAdapter: public RefBase {
public:
    static RpAgpsNwAdapter* sInstance;
    static RpAgpsNwAdapter *getInstance();
    static void closeInstance();

    // Agps command reader thread, read and parse command from Agpsd
    class AgpsCmdReaderThread: public Thread {
    public:
        AgpsCmdReaderThread(RpAgpsNwAdapter* agpsNwAdapter);
        virtual ~AgpsCmdReaderThread();

    protected:
        RpAgpsNwAdapter* mAgpsNwAdapter;
        virtual bool threadLoop();
        int mAgps2Fwk_fd;
        Agps2FrameworkInterface_callbacks mAgps2FrameworkInterface_callbacks = {
                Agps2FrameworkInterface_isExist_handler,
                Agps2FrameworkInterface_acquireWakeLock_handler,
                Agps2FrameworkInterface_releaseWakeLock_handler,
                Agps2FrameworkInterface_requestDedicatedApnAndDnsQuery_handler,
                Agps2FrameworkInterface_releaseDedicatedApn_handler,
                Agps2FrameworkInterface_requestGpsIcon_handler,
                Agps2FrameworkInterface_removeGpsIcon_handler,
                Agps2FrameworkInterface_requestC2kApn_handler,
                Agps2FrameworkInterface_releaseC2kApn_handler,
                Agps2FrameworkInterface_AgpsNiNotify_handler
        };
    };

    sp<AgpsCmdReaderThread> mAgpsCmdReaderThread;

    // Agps command handle thread, handle the request send response
    class RequestHandleThread: public Thread {
    public:
        RequestHandleThread(RpAgpsNwAdapter* agpsNwAdapter);
        virtual ~RequestHandleThread();
        sp<Looper> getLooper();

    protected:
        RpAgpsNwAdapter* mAgpsNwAdapter;
        virtual bool threadLoop();
    private:
        sp<Looper> m_looper;
    };

    class AgpsRequestMessage: public RefBase {
    public:
        AgpsRequestMessage();
        virtual ~AgpsRequestMessage() {if(mNwRequestResult != NULL) {
            delete mNwRequestResult;
            mNwRequestResult = NULL;
        }};

    public:
        int mToken;
        int mMsgType;
        char * mFqdn;bool mIsEmergencySupl;bool mIsApnEnabled;
        AgpsNwRequestResult* mNwRequestResult;
        AgpsDnsRequestResult* mDnsRequestResult;
    };

    class AgpsRequestHandler: public MessageHandler {
    public:
        AgpsRequestHandler(RpAgpsNwAdapter* agpsNwAdapter): mAgpsNwAdapter(agpsNwAdapter){}
        virtual ~AgpsRequestHandler() {}

    public:
        void handleMessage(const Message& message);
        // AGPS message reference
        sp<AgpsRequestMessage> mMsg;
        // dummy message that makes handler happy
        Message m_dummyMsg;
    private:
        RpAgpsNwAdapter* mAgpsNwAdapter;
    };

    sp<RequestHandleThread> mRequestHandleThread;
    sp<AgpsRequestHandler> mAgpsRequestHandler;
    // send message to request handler
    sp<AgpsRequestHandler> sendMessage(sp<AgpsRequestMessage> msg, int delayms);

    void requestDedicatedApnAndDnsQuery_handler(char* fqdn,
            bool isEmergencySupl, bool isApnEnabled);
    void releaseDedicatedApn_handler();

    mtk_socket_fd sFwk2Agps_fd;

    // Network API
    RpRilAgpsNwClient* mRilAgpsNwClient;
    void requestNetwork(char* apn_type, bool enable, int token);
    void onNwRequestComplete(int token, RIL_Errno e, void *response, size_t responselen);
    void responseToAgps(AgpsDnsRequestResult result);
    void queryDns(char* hostname, int netId,  struct addrinfo *hints, AgpsDnsRequestResult * agpsDnsRequestResult);

    map<int, sp<AgpsRequestHandler>> mPendingRequests;

private:
    RpAgpsNwAdapter();
    virtual ~RpAgpsNwAdapter();
    void init();
    void deInit();
    static pthread_mutex_t token_mutex;
    static pthread_mutex_t mMsg_mutex;
    static int tokenCounter;
    static int generateToken();
    void freeMem(const MTK_Data_Call_Response_v1 &rsp);
};

#endif /* MTK_RILPROXY_TELEPHONY_AGPSADAPTER_RPAGPSNWADAPTER_H_ */
