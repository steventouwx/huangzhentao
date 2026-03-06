// SPDX-License-Identifier: MediaTekProprietary
/*
 * RpAgpsNwAdapter.cpp
 *
 *  Created on: Jun 9, 2017
 *      Author: Kaiduan Cao
 */

#include "RpAgpsNwAdapter.h"
#include <mtk_log.h>
#include <arpa/inet.h>
#include "../include/telephony/ril.h"


#include "data/RpDataUtils.h"

#define LOG_TAG "RpAgpsNwAdapter"
extern int rfx_get_data_allowed_slotid(void);

pthread_mutex_t RpAgpsNwAdapter::token_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t RpAgpsNwAdapter::mMsg_mutex = PTHREAD_MUTEX_INITIALIZER;
int RpAgpsNwAdapter::tokenCounter = 0;

RpAgpsNwAdapter* RpAgpsNwAdapter::sInstance = NULL;

int RpAgpsNwAdapter::generateToken() {
    int next = 0;
    pthread_mutex_lock(&token_mutex);
    next = tokenCounter++;
    if (next == __INT_MAX__) {
        tokenCounter = 0;
    }
    pthread_mutex_unlock(&token_mutex);
    return next;
}

RpAgpsNwAdapter::RpAgpsNwAdapter() : mRilAgpsNwClient(NULL) {
}

RpAgpsNwAdapter::~RpAgpsNwAdapter() {
}

RpAgpsNwAdapter *RpAgpsNwAdapter::getInstance() {
    if (sInstance == NULL) {
        sInstance = new RpAgpsNwAdapter();
        sInstance->init();
    }
    return sInstance;
}

void RpAgpsNwAdapter::closeInstance() {
    if (sInstance != NULL) {
        sInstance->deInit();
        sInstance = NULL;
    }
}

void RpAgpsNwAdapter::init() {
    //Start input thread to read commands from agpsd
    mAgpsCmdReaderThread = new AgpsCmdReaderThread(this);
    mAgpsCmdReaderThread->run("AgpsCmdReaderThread");
    mRequestHandleThread = new RequestHandleThread(this);
    mRequestHandleThread->run("AgpsCmdReaderThread");
}

void RpAgpsNwAdapter::deInit() {

}

// Method implements of AgpsCmdReaderThread
RpAgpsNwAdapter::AgpsCmdReaderThread::AgpsCmdReaderThread(RpAgpsNwAdapter* adapter): mAgps2Fwk_fd(-1) {
    mAgpsNwAdapter = adapter;
    RLOGD("AgpsCmdReaderThread created");
}

RpAgpsNwAdapter::AgpsCmdReaderThread::~AgpsCmdReaderThread() {
    mAgpsNwAdapter = NULL;
    RLOGD("AgpsCmdReaderThread destroyed");
}

bool RpAgpsNwAdapter::AgpsCmdReaderThread::threadLoop() {
    RLOGD("AgpsCmdReaderThread threadLoop enter");
    // Create agpsd to framework socket to listen the commands from agpsd.
    int ret;

    mAgps2Fwk_fd = mtk_socket_server_bind_local(SOCKET_AGPS2FWK, SOCK_NS_ABSTRACT);
    if (mAgps2Fwk_fd < 0) {
        RLOGD("AgpsCmdReaderThread mtk_socket_server_bind_local() fail");
        return false;
    }
    mtk_socket_client_init_local(&(mAgpsNwAdapter->sFwk2Agps_fd), SOCKET_FWK2AGPS, SOCK_NS_ABSTRACT);
    RLOGD("AgpsCmdReaderThread bind socket success, mAgps2Fwk_fd=%d, sFwk2Agps_fd=%d", mAgps2Fwk_fd,
            mAgpsNwAdapter->sFwk2Agps_fd.fd);
    do {
        RLOGD("AgpsCmdReaderThread threadLoop reading command...");
        ret = Agps2FrameworkInterface_receiver_read_and_decode(mAgps2Fwk_fd,
                &mAgps2FrameworkInterface_callbacks);
    } while (ret != 0);
    RLOGD("AgpsCmdReaderThread Agps2FrameworkInterface_receiver_read_and_decode() fail %d", ret);

    return true;
}

// Method implements of RequestHandleThread
RpAgpsNwAdapter::RequestHandleThread::RequestHandleThread(RpAgpsNwAdapter* adapter) : m_looper(NULL) {
    mAgpsNwAdapter = adapter;
    RLOGD("RequestHandleThread created");
}

RpAgpsNwAdapter::RequestHandleThread::~RequestHandleThread() {
    mAgpsNwAdapter = NULL;
    RLOGD("RequestHandleThread destroyed");
}

bool RpAgpsNwAdapter::RequestHandleThread::threadLoop() {
    RLOGD("RequestHandleThread threadLoop");
    // start message loop
    m_looper = Looper::prepare(0);
    int result;
    do {
        result = m_looper->pollAll(-1);
        RLOGD("RequestHandleThread threadLoop, pull message result = %d", result);
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);
    return true;
}

sp<Looper> RpAgpsNwAdapter::RequestHandleThread::getLooper() {
    return m_looper;
}

RpAgpsNwAdapter::AgpsRequestMessage::AgpsRequestMessage() : mMsgType(MSG_TYPE_NONE),
        mFqdn(NULL), mIsEmergencySupl(false), mIsApnEnabled(false),
        mNwRequestResult(NULL), mDnsRequestResult(NULL) {
    mToken = generateToken();
}

void RpAgpsNwAdapter::AgpsRequestHandler::handleMessage(const Message& message) {
    AgpsDnsRequestResult result = {false, false, -1, false, {0}, 0, false, -1};
    int msgType = mMsg->mMsgType;
    RLOGD("AgpsRequestHandler handleMessage mMsg->mToken= %d, msgType=%d", mMsg->mToken, msgType);
    switch (msgType) {
    case AGPS_NW_MESSAGE_TYPE::MSG_REQUEST_NET:

        /* TODO Handle emergency supl
        if (msg->mIsEmergencySupl) {
            // Try to use the EIMS or IMS network
        }*/
        RLOGD("AgpsRequestHandler handleMessage MSG_REQUEST_NET mIsApnEnabled = %d", mMsg->mIsApnEnabled);
        if (mMsg->mIsApnEnabled) {
            // Send ril request to setup apn data
            mAgpsNwAdapter->requestNetwork("supl", true, mMsg->mToken);
            mMsg->mMsgType = AGPS_NW_MESSAGE_TYPE::MSG_REQUEST_NET_TIMEOUT;
            sp<RpAgpsNwAdapter::AgpsRequestHandler> handler = mAgpsNwAdapter->sendMessage(mMsg,
                    REQUEST_NETWORK_TIMEOUT);
            // Save the handler to handle repose
            mAgpsNwAdapter->mPendingRequests[mMsg->mToken] = handler;
            RLOGD("AgpsRequestHandler handleMessage MSG_REQUEST_NET mPendingRequests add mMsg->mToken = %d",
                    mMsg->mToken);
        }
        break;
    case AGPS_NW_MESSAGE_TYPE::MSG_QUERY_DNS:

        struct addrinfo hints;

        memset(&hints,0,sizeof(hints));

        RLOGD("AgpsRequestHandler handleMessage MSG_QUERY_DNS mFqdn=%s, netId=%d", mMsg->mFqdn,
                mMsg->mNwRequestResult->netId);
        // To query DNS
        if(1 == mMsg->mNwRequestResult->type)
        {
            hints.ai_family = PF_INET;
        }
        else if(2 == mMsg->mNwRequestResult->type)
        {
            hints.ai_family = PF_INET6;
        }
        else
        {
            hints.ai_family = PF_UNSPEC;
        }
        mAgpsNwAdapter->queryDns(mMsg->mFqdn, mMsg->mNwRequestResult->netId, &hints, &result);
        if (!result.isSuccess) {
            // Send ril request to release apn data connection
            mAgpsNwAdapter->requestNetwork("supl", false, mMsg->mToken);
        }
        mAgpsNwAdapter->responseToAgps(result);
        // The message life cycle finished. Delete the message
        mMsg = NULL;
        break;
    case AGPS_NW_MESSAGE_TYPE::MSG_REQUEST_NET_RESULT_FAIL:
    case AGPS_NW_MESSAGE_TYPE::MSG_REQUEST_NET_TIMEOUT:
        pthread_mutex_lock(&mMsg_mutex);
        RLOGD("AgpsRequestHandler handleMessage MSG_REQUEST_NET_TIMEOUT"
                " or MSG_REQUEST_NET_RESULT_FAIL");
        // Send ril request to release apn data connection
        mAgpsNwAdapter->requestNetwork("supl", false, mMsg->mToken);
        result.isSuccess = false;
        mAgpsNwAdapter->responseToAgps(result);
        // The message life cycle finished. Delete the message
        mMsg = NULL;
        pthread_mutex_unlock(&mMsg_mutex);
        break;
    case AGPS_NW_MESSAGE_TYPE::MSG_RELEASE_NET:
        RLOGD("AgpsRequestHandler handleMessage MSG_RELEASE_NET");
        // Send ril request to release apn data connection
        mAgpsNwAdapter->requestNetwork("supl", false, mMsg->mToken);
        // The message life cycle finished. Delete the message
        mMsg = NULL;
        break;
    default:
        break;
    }
}

sp<RpAgpsNwAdapter::AgpsRequestHandler> RpAgpsNwAdapter::sendMessage(sp<AgpsRequestMessage> msg, int delayms) {
    RLOGD("sendMessage msg token=%d delayms=%d", msg->mToken, delayms);
    // Create a handler to handle this message
    sp<AgpsRequestHandler> handler = new AgpsRequestHandler(this);
    handler->mMsg = msg;
    // Sand message to looper
    if (delayms > 0) {
        mRequestHandleThread->getLooper()->sendMessageDelayed(ms2ns(delayms),
                handler, handler->m_dummyMsg);
    } else {
        mRequestHandleThread->getLooper()->sendMessage(handler, handler->m_dummyMsg);
    }
    return handler;
}

// Agps command handlers, handle the commands from Agpsd
void Agps2FrameworkInterface_isExist_handler() {
    RLOGD("Agps2FrameworkInterface_isExist_handler called");
}
void Agps2FrameworkInterface_acquireWakeLock_handler() {
    RLOGD("Agps2FrameworkInterface_acquireWakeLock_handler called");
}
void Agps2FrameworkInterface_releaseWakeLock_handler() {
    RLOGD("Agps2FrameworkInterface_releaseWakeLock_handler called");
}
void Agps2FrameworkInterface_requestDedicatedApnAndDnsQuery_handler(char* fqdn,
        bool isEmergencySupl, bool isApnEnabled) {
    RLOGD("Agps2FrameworkInterface_requestDedicatedApnAndDnsQuery_handler called");
    RpAgpsNwAdapter::getInstance()->requestDedicatedApnAndDnsQuery_handler(fqdn, isEmergencySupl, isApnEnabled);
}

void Agps2FrameworkInterface_releaseDedicatedApn_handler() {
    RLOGD("Agps2FrameworkInterface_releaseDedicatedApn_handler called");
    RpAgpsNwAdapter::getInstance()->releaseDedicatedApn_handler();
}
/**
 * when emergency call is dialed and location is requested from network
 * AGPSD will decide to send this message to framework to show
 * the GPS icon to meet the operator requirement
 */
void Agps2FrameworkInterface_requestGpsIcon_handler() {
    RLOGD("Agps2FrameworkInterface_requestGpsIcon_handler called");
}
/**
 * if requestGpsIcon() was sent before, in the end of this AGPS session
 * AGPSD will send this message to framework to remove the GPS icon
 */
void Agps2FrameworkInterface_removeGpsIcon_handler() {
    RLOGD("Agps2FrameworkInterface_removeGpsIcon_handler called");
}

void Agps2FrameworkInterface_requestC2kApn_handler() {
    RLOGD("Agps2FrameworkInterface_isExist_handler called");
}

void Agps2FrameworkInterface_releaseC2kApn_handler() {
    RLOGD("Agps2FrameworkInterface_releaseC2kApn_handler called");
}

void Agps2FrameworkInterface_AgpsNiNotify_handler() {
    RLOGD("Agps2FrameworkInterface_AgpsNiNotify_handler called");
}
void RpAgpsNwAdapter::requestDedicatedApnAndDnsQuery_handler(char* fqdn,
        bool isEmergencySupl, bool isApnEnabled) {
    RLOGD("requestDedicatedApnAndDnsQuery_handler fqdn=%s, isEmergencySupl=%d, isApnEnabled=%d",
            fqdn, isEmergencySupl, isApnEnabled);
    // Create a request message
    sp<AgpsRequestMessage> msg = new AgpsRequestMessage();
    msg->mMsgType = AGPS_NW_MESSAGE_TYPE::MSG_REQUEST_NET;
    msg->mFqdn = fqdn;
    msg->mIsEmergencySupl = isEmergencySupl;
    msg->mIsApnEnabled = isApnEnabled;

    sendMessage(msg, 0);
}


void RpAgpsNwAdapter::releaseDedicatedApn_handler() {
    RLOGD("releaseDedicatedApn_handler");
    // Create a request message
    sp<AgpsRequestMessage> msg = new AgpsRequestMessage();
    msg->mMsgType = AGPS_NW_MESSAGE_TYPE::MSG_RELEASE_NET;

    sendMessage(msg, 0);
}

void RpAgpsNwAdapter::requestNetwork(char* apn_type, bool enable, int token) {
    int requestId = enable ? RIL_REQUEST_SETUP_DATA_CALL : RIL_REQUEST_DEACTIVATE_DATA_CALL;

    int data_slotId = rfx_get_data_allowed_slotid();

    Parcel p;
    p.writeInt32(requestId);
    p.writeInt32(token);
    if (apn_type != nullptr) {
        RLOGD("requestNetwork apn_type=%s, requestId=%d, token=%d", apn_type, requestId, token);
        RpDataUtils::writeStringToParcel(&p, apn_type);
    } else {
        RLOGE("requestNetwork apn_type could not be null.");
        return;
    }
    mRilAgpsNwClient->sendRequest(p, data_slotId);
}

void RpAgpsNwAdapter::freeMem(const MTK_Data_Call_Response_v1 &rsp) {
    if (rsp.apnType) {
        free(rsp.apnType);
    }
    if (rsp.type) {
        free(rsp.type);
    }
    if (rsp.ifname) {
        free(rsp.ifname);
    }
    if (rsp.addresses) {
        free(rsp.addresses);
    }
    if (rsp.dnses) {
        free(rsp.dnses);
    }
    if (rsp.gateways) {
        free(rsp.gateways);
    }
    if (rsp.pcscf) {
        free(rsp.pcscf);
    }
}

void RpAgpsNwAdapter::onNwRequestComplete(int token, RIL_Errno e, void *response, size_t responselen) {
    pthread_mutex_lock(&mMsg_mutex);
    RLOGD("onNwRequestComplete mPendingRequests remove token = %d", token);
    // Find the request
    map<int, sp<AgpsRequestHandler>>::iterator  iter = mPendingRequests.find(token);
    if (mPendingRequests.end()== iter) {
        RLOGD("onNwRequestComplete no pending request for this response!");
        pthread_mutex_unlock(&mMsg_mutex);
        return;
    }
    sp<AgpsRequestHandler> handler = iter->second;
    // Cancel the timeout and remove the pending request
    mRequestHandleThread->getLooper()->removeMessages(handler);
    mPendingRequests.erase(token);

    // when AGPS_NW_MESSAGE_TYPE is MSG_REQUEST_NET_TIMEOUT, first call handleMessage,
    // then mMsg is NULL.
    if (handler->mMsg == NULL) {
         RLOGD("onNwRequestComplete handler mMsg is NULL, just return");
         handler = NULL;
         pthread_mutex_unlock(&mMsg_mutex);
         return;
     }

    AgpsNwRequestResult* result = new AgpsNwRequestResult({false, -1});
    // If request network failed, need report to agps
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("onNwRequestComplete request failed. e=%d", e);
        result->isSuccess = false;
        handler->mMsg->mMsgType = AGPS_NW_MESSAGE_TYPE::MSG_REQUEST_NET_RESULT_FAIL;
        handler->mMsg->mNwRequestResult = result;
        sendMessage(handler->mMsg, 0);
        pthread_mutex_unlock(&mMsg_mutex);
        return;
    }
    // The request network is success, next action is to query dns
    MTK_Data_Call_Response_v1 rsp;
    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    RpDataUtils::parseDataCallResponse(&rsp, &p);
    if (rsp.netId <= 0) {
        RLOGD("onNwRequestComplete failed to get net id. netId=%d", rsp.netId);
        result->isSuccess = false;
        handler->mMsg->mMsgType = AGPS_NW_MESSAGE_TYPE::MSG_REQUEST_NET_RESULT_FAIL;
        handler->mMsg->mNwRequestResult = result;
        sendMessage(handler->mMsg, 0);
        pthread_mutex_unlock(&mMsg_mutex);

        freeMem(rsp);
        return;
    }
    RLOGD("onNwRequestComplete get net id. netId=%d", rsp.netId);
    result->isSuccess = true;
    result->netId = rsp.netId;
    if(strcmp("IPV4V6",rsp.type)==0)
    {
        result->type = 3;
    }
    else if(strcmp("IPV6",rsp.type)==0)
    {
        result->type = 2;

    }
    else
    {
        result->type = 1;

    }
    handler->mMsg->mMsgType = AGPS_NW_MESSAGE_TYPE::MSG_QUERY_DNS;
    handler->mMsg->mNwRequestResult = result;
    RLOGD("onNwRequestComplete result.netId=%d, handler->mMsg->mNwRequestResult->netId=%d",
            result->netId, handler->mMsg->mNwRequestResult->netId);
    freeMem(rsp);
    sendMessage(handler->mMsg, 0);
    pthread_mutex_unlock(&mMsg_mutex);
}

void RpAgpsNwAdapter::responseToAgps(AgpsDnsRequestResult result) {
    RLOGD("responseToAgps result isSuccess=%d, hasIpv4=%d, ipv4=%d, hasIpv6=%d, ipv6=%s,"
            " ipv6_size=%d, hasNetId=%d, netId=%d", result.isSuccess, result.hasIpv4, result.ipv4,
            result.hasIpv6, result.ipv6, result.ipv6_size, result.hasNetId, result.netId);
    bool ok = Framework2AgpsInterface_DnsQueryResult2(&sFwk2Agps_fd, result.isSuccess, result.hasIpv4, result.ipv4,
            result.hasIpv6, result.ipv6, result.ipv6_size, result.hasNetId, result.netId);
    RLOGD("responseToAgps done ok=%d", ok);
}

void RpAgpsNwAdapter::queryDns(char* hostname, int netId, struct addrinfo *hints, AgpsDnsRequestResult * agpsDnsRequestResult) {
    int error;
    const struct addrinfo *ai;
    struct addrinfo *aihead = NULL;
    size_t ss_size;
    agpsDnsRequestResult->hasNetId = netId > 0;
    agpsDnsRequestResult->netId = netId;

    RLOGD("queryDns start..., hostname=%s, netId=%d", hostname, netId);

    error = 1;//android_getaddrinfofornet(hostname, NULL, hints, netId, netId, &aihead);
    if (error) {
        agpsDnsRequestResult->isSuccess = false;
        RLOGD("queryDns error=%d", error);
        return;
    }
    /* traverse the addrinfo list */
    for (ai = aihead; ai != NULL; ai = ai->ai_next) {
        RLOGD("queryDns traverse ai->ai_family=%d, ai->ai_addrlen=%d", ai->ai_family, ai->ai_addrlen);
        /* ignore elements with unsupported address family, */
        /* settle family-specific sockaddr structure size.  */
        if (ai->ai_family == AF_INET)
            ss_size = sizeof(struct sockaddr_in);
        else if (ai->ai_family == AF_INET6)
            ss_size = sizeof(struct sockaddr_in6);
        else
            continue;

        /* ignore elements without required address info */
        if ((ai->ai_addr == NULL) || !(ai->ai_addrlen > 0))
            continue;

        /* ignore elements with bogus address size */
        if ((size_t) ai->ai_addrlen < ss_size)
            continue;

        if (ai->ai_family == AF_INET) {
            sockaddr_in* addr = (struct sockaddr_in*)ai->ai_addr;
            agpsDnsRequestResult->hasIpv4 = true;
            agpsDnsRequestResult->ipv4 = addr->sin_addr.s_addr;
            char s4[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(addr->sin_addr), s4, INET_ADDRSTRLEN);
            RLOGD("queryDns traverse has ipv4 ipv4=%s, addr->sin_addr.s_addr=%d",
                    s4, addr->sin_addr.s_addr);
        } else if (ai->ai_family == AF_INET6) {
            sockaddr_in6* addr = (struct sockaddr_in6*)ai->ai_addr;
            agpsDnsRequestResult->hasIpv6 = true;
            agpsDnsRequestResult->ipv6_size = sizeof(addr->sin6_addr.s6_addr);
            memcpy(agpsDnsRequestResult->ipv6, addr->sin6_addr.s6_addr, sizeof(addr->sin6_addr.s6_addr));
            char s6[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &(agpsDnsRequestResult->ipv6), s6, INET6_ADDRSTRLEN);
            RLOGD("queryDns traverse has ipv6 ipv6=%s, ipv6_size=%d",
                    s6, agpsDnsRequestResult->ipv6_size);
        }
    }
    if (agpsDnsRequestResult->hasIpv4 || agpsDnsRequestResult->hasIpv6) {
        agpsDnsRequestResult->isSuccess = true;
    }
    RLOGD("queryDns isSuccess=%d", agpsDnsRequestResult->isSuccess);
    return;
}
