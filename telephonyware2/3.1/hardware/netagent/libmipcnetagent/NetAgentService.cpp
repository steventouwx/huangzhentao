/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "NetAgentService.h"
#include "NetlinkEventHandler.h"
#include <prop/properties.h>
#include <linux/ipv6.h>
#include <fcntl.h>

/*****************************************************************************
 ** Defines
 ******************************************************************************/
#define NA_LOG_TAG "NetAgentService"
#define SIOCSTXQSTATE (SIOCDEVPRIVATE + 0)   // start/stop interface tx queue.
#define SIOPUSHPENDING (SIOCDEVPRIVATE + 4)  // Notify CCCI send RA package.
#define MASK_STRING "***"

/*****************************************************************************
 * Class NetAgentService
 *****************************************************************************/
pthread_mutex_t NetAgentService::sInitMutex = PTHREAD_MUTEX_INITIALIZER;
NetAgentService* NetAgentService::sInstance = NULL;

struct thread_args {
    NetAgentService *instance;
    NA_ARP_INFO *arp;
    mipc_msg_t *header;
};

NetAgentService::NetAgentService() {
    mIsUserBuild = (isUserLoad() == 1);
    init();
}

void NetAgentService::init() {
    mEventThread = 0;
    sock_fd = 0;
    sock6_fd = 0;
    m_pNetAgentReqInfo = NULL;
    mRouteSock = 0;
    m_pRouteHandler = NULL;
    pthread_mutex_init(&mDispatchMutex, NULL);
    pthread_cond_init(&mDispatchCond, NULL);
    m_lTransIntfId.clear();
    mSpiMonitorThread = 0;

    char thinModem[PROPERTY_VALUE_MAX] = {0};
    property_get("ro.vendor.mtk_thin_md_telephony_support_mode", thinModem, "0");
    mIsThinModemSupport = (strcmp(thinModem, "0") != 0);

    mSimCount = getSimCount();
    if (mSimCount <= 0) {
        NA_LOG_E("[%s] fail to get sim count", __FUNCTION__);
        mSimCount = DEFAULT_SIM_COUNT;
    }

    m_pNetAgentCallback1 = (NetAgentCallback1 *)calloc(mSimCount, sizeof(NetAgentCallback1));
    if (m_pNetAgentCallback1 == NULL) {
        NA_LOG_E("[%s] fail to allocate m_pNetAgentCallback1", __FUNCTION__);
    }

    startEventLoop();
    startMipcEventHandler();
    startNetlinkEventHandler();
    initInterfaceManager();
    syncSpiQueryToModem();
}

NetAgentService::~NetAgentService() {
    NetAgentReqInfo *pTmp = NULL;
    while (m_pNetAgentReqInfo != NULL) {
       pTmp = m_pNetAgentReqInfo;
       m_pNetAgentReqInfo = m_pNetAgentReqInfo->pNext;
       freeNetAgentCmdObj(pTmp);
       FREEIF(pTmp);
    }

    if (m_pRouteHandler != NULL) {
        if (m_pRouteHandler->stop() < 0) {
            NA_LOG_E("[%s] Unable to stop route NetlinkEventHandler: %s",
                    __FUNCTION__, strerror(errno));
        }
        delete m_pRouteHandler;
        m_pRouteHandler = NULL;
    }

    if (m_pNetAgentCallback1 != NULL) {
        for (int i = 0; i < mSimCount; i++) {
            if (m_pNetAgentCallback1[i].action1 != NULL) {
                m_pNetAgentCallback1[i].action1 = NULL;
            }
        }
        free(m_pNetAgentCallback1);
        m_pNetAgentCallback1 = NULL;
    }
    mSimCount = 0;
    mSpiMonitorThread = 0;
    m_lTransIntfId.clear();
    delete mInterfaceManager;
    delete mMipcEventHandler;

    sInstance = NULL;
}

NetAgentService* NetAgentService::getInstance() {
    if (sInstance != NULL) {
        return sInstance;
    }
    return NULL;
}

bool NetAgentService::createNetAgentService() {
    pthread_mutex_lock(&sInitMutex);
    if (sInstance == NULL) {
        try {
            sInstance = new (std::nothrow)NetAgentService();
        } catch (const std::overflow_error) {
            NA_LOG_E("[%s] overflow_error", __FUNCTION__);
            sInstance = NULL;
        }
        if (sInstance == NULL) {
            NA_LOG_E("[%s] new NetAgentService fail", __FUNCTION__);
            pthread_mutex_unlock(&sInitMutex);
            return false;
        }
    }
    pthread_mutex_unlock(&sInitMutex);
    return true;
}

void NetAgentService::startEventLoop(void) {
    int ret;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&mEventThread, &attr, NetAgentService::eventThreadStart, this);

    if (ret != 0) {
        NA_LOG_E("[%s] failed to create event thread ret:%d", __FUNCTION__, ret);
    } else {
        NA_LOG_D("[%s] create event thread OK ret:%d, mEventThread:%ld",
                __FUNCTION__, ret, mEventThread);
        ret = pthread_setname_np(mEventThread, "NA-EventLoop");
        if (ret != 0) NA_LOG_E("[%s] failed to set name ret:%d", __FUNCTION__, ret);
    }
}

void *NetAgentService::eventThreadStart(void *arg) {
    NetAgentService *me = reinterpret_cast<NetAgentService *>(arg);
    me->runEventLoop();
    return NULL;
}

void NetAgentService::runEventLoop() {
    while (1) {
        NetAgentReqInfo *pReq = NULL;

        pthread_mutex_lock(&mDispatchMutex);
        pReq = dequeueReqInfo();
        if (pReq != NULL) {
            pthread_mutex_unlock(&mDispatchMutex);
            handleEvent(pReq);
            FREEIF(pReq);
        } else {
            pthread_cond_wait(&mDispatchCond, &mDispatchMutex);
            pthread_mutex_unlock(&mDispatchMutex);
        }
    }
}

const char* NetAgentService::getInterfaceName() {
    if (mIsThinModemSupport) return NETWORK_IFNAME_WWAN;
    return NETWORK_IFNAME_CCMNI;
}

void NetAgentService::handleEvent(NetAgentReqInfo* pReqInfo) {
    switch (pReqInfo->cmdType) {
        case NETAGENT_IO_CMD_IFUP:
            configureNetworkInterface(pReqInfo, ENABLE);
            break;
        case NETAGENT_IO_CMD_IFDOWN:
            configureNetworkInterface(pReqInfo, DISABLE);
            break;
        case NETAGENT_IO_CMD_IPUPDATE:
            updateGlobalIpv6Address(pReqInfo);
            break;
        case NETAGENT_IO_CMD_SETMTU:
            configureKernelMtu(pReqInfo);
            break;
        case NETAGENT_IO_CMD_IFCHANGE:
            configureInterfaceChange(pReqInfo);
            break;
        case NETAGENT_IO_CMD_PDNHO:
            handlePdnHandoverControl(pReqInfo);
            break;
        case NETAGENT_IO_CMD_IPCHG:
            updatePdnHandoverAddr(pReqInfo);
            break;
        case NETAGENT_IO_CMD_ARP_QUERY:
            queryArp(pReqInfo);
            break;
        case NETAGENT_IO_CMD_ARP_RESULT:
            sendArpResult(pReqInfo);
            break;
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            reserveTcpUdpPort(pReqInfo);
            break;
        case NETAGENT_IO_CMD_SPI_RSVN:
            reserveSpi(pReqInfo);
            break;
        case NETAGENT_IO_CMD_REALLOCATE_SPI:
            reallocateSpi(pReqInfo);
            break;
        case NETAGENT_IO_CMD_SPI_QUERY:
            setSpiQueryToModem();
            break;
        default:
            break;
    }
    freeNetAgentCmdObj(pReqInfo);
}

NetAgentReqInfo *NetAgentService::createNetAgentReqInfo(void* obj, REQUEST_TYPE reqType, NA_CMD cmd) {
    NetAgentReqInfo* pNewReqInfo = NULL;
    pNewReqInfo = (NetAgentReqInfo *)calloc(1, sizeof(NetAgentReqInfo));
    if (pNewReqInfo == NULL) {
        NA_LOG_E("[%s] can't allocate NetAgentReqInfo", __FUNCTION__);
        return NULL;
    }

    pNewReqInfo->pNext = NULL;
    pNewReqInfo->pNetAgentCmdObj = obj;
    pNewReqInfo->reqType = reqType;
    pNewReqInfo->cmdType = cmd;
    return pNewReqInfo;
}

void NetAgentService::enqueueReqInfo(void* obj, REQUEST_TYPE reqType) {
    NetAgentReqInfo *pNew = NULL;
    NetAgentReqInfo *pCurrent = NULL;
    NA_CMD cmd = NETAGENT_IO_CMD_MAX;

    if (getCommand(obj, reqType, &cmd) < 0) {
        NA_LOG_E("[%s] get command fail", __FUNCTION__);
        return;
    }

    pNew = createNetAgentReqInfo(obj, reqType, cmd);
    if (pNew == NULL) {
        NA_LOG_E("[%s] create NetAgentReqInfo fail", __FUNCTION__);
        return;
    }

    pthread_mutex_lock(&mDispatchMutex);
    if (m_pNetAgentReqInfo == NULL) { /* No pending */
        m_pNetAgentReqInfo = pNew;
        pthread_cond_broadcast(&mDispatchCond);
    } else {
        pCurrent = m_pNetAgentReqInfo;
        while(pCurrent != NULL) {
            if (pCurrent->pNext == NULL) {
                pCurrent->pNext = pNew;
                break;
            }
            pCurrent = pCurrent->pNext;
        }
    }
    pthread_mutex_unlock(&mDispatchMutex);
}

NetAgentReqInfo *NetAgentService::dequeueReqInfo() {
    NetAgentReqInfo *pCurrent = m_pNetAgentReqInfo;
    if (pCurrent != NULL) {
        m_pNetAgentReqInfo = pCurrent->pNext;
    }
    return pCurrent;
}

void NetAgentService::setNwIntfDown(const char *interfaceName) {
    ifc_reset_connections(interfaceName, RESET_ALL_ADDRESSES);
    ifc_remove_default_route(interfaceName);
    ifc_disable(interfaceName);
}

void NetAgentService::nwIntfSetMtu(int s, struct ifreq *ifr, int mtu) {
    int ret = 0;
    ifr->ifr_mtu = mtu;

    ret = ioctl(s, SIOCSIFMTU, ifr);
    if (ret < 0) {
        NA_LOG_E("[%s] error in set SIOCSIFMTU:%d - %d:%s",
            __FUNCTION__, ret, errno, strerror(errno));
    }
}

void NetAgentService::nwIntfIoctlInit() {
    if (sock_fd > 0) {
        close(sock_fd);
    }

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        NA_LOG_E("[%s] couldn't create IP socket: errno=%d", __FUNCTION__, errno);
    }

    if (sock6_fd > 0) {
        close(sock6_fd);
    }

    sock6_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock6_fd < 0) {
        sock6_fd = -errno;    /* save errno for later */
        NA_LOG_E("[%s] couldn't create IPv6 socket: errno=%d", __FUNCTION__, errno);
    }
}

void NetAgentService::nwIntfIoctlDeInit() {
    close(sock_fd);
    close(sock6_fd);
    sock_fd = 0;
    sock6_fd = 0;
}

/* For setting IFF_UP: nwIntfSetFlags(s, &ifr, IFF_UP, 0) */
/* For setting IFF_DOWN: nwIntfSetFlags(s, &ifr, 0, IFF_UP) */
void NetAgentService::nwIntfSetFlags(int s, struct ifreq *ifr, int set, int clr) {
    int ret = 0;

    ret = ioctl(s, SIOCGIFFLAGS, ifr);
    if (ret < 0) {
        NA_LOG_E("[%s] error in set SIOCGIFFLAGS:%d - %d:%s",
                __FUNCTION__, ret, errno, strerror(errno));
        return;
    }

    ifr->ifr_flags = (ifr->ifr_flags & (~clr)) | set;
    ret = ioctl(s, SIOCSIFFLAGS, ifr);
    if (ret < 0) {
        NA_LOG_E("[%s] error in set SIOCSIFFLAGS:%d - %d:%s",
                __FUNCTION__, ret, errno, strerror(errno));
    }
}

inline void NetAgentService::nwIntfInitSockAddrIn(struct sockaddr_in *sin, const char *addr) {
    sin->sin_family = AF_INET;
    sin->sin_port = 0;
    sin->sin_addr.s_addr = inet_addr(addr);
}

void NetAgentService::nwIntfSetAddr(int s, struct ifreq *ifr, const char *addr) {
    int ret = 0;

    NA_LOG_D("[%s] configure IPv4 adress : %s", __FUNCTION__, isUserBuild() ? "xxx" : addr);
    nwIntfInitSockAddrIn((struct sockaddr_in *) &ifr->ifr_addr, addr);
    ret = ioctl(s, SIOCSIFADDR, ifr);
    if (ret < 0) {
        NA_LOG_E("[%s] error in set SIOCSIFADDR:%d - %d:%s",
                __FUNCTION__, ret, errno, strerror(errno));
    }
}

void NetAgentService::nwIntfSetIpv6Addr(int s, struct ifreq *ifr, const char *addr) {
    struct in6_ifreq ifreq6;
    int ret = 0;

    NA_LOG_D("[%s] configure IPv6 adress : %s", __FUNCTION__, isUserBuild() ? "xxx" : addr);
    ret = ioctl(s, SIOCGIFINDEX, ifr);
    if (ret < 0) {
        NA_LOG_E("[%s] error in set SIOCGIFINDEX:%d - %d:%s",
                __FUNCTION__, ret, errno, strerror(errno));
        return;
    }

    // ret: -1, error occurs, ret: 0, invalid address, ret: 1, success;
    ret = inet_pton(AF_INET6, addr, &ifreq6.ifr6_addr);
    if (ret <= 0) {
        NA_LOG_E("[%s] ipv6 address: %s, inet_pton ret: %d", __FUNCTION__, addr, ret);
        return;
    }
    ifreq6.ifr6_prefixlen = 64;
    ifreq6.ifr6_ifindex = ifr->ifr_ifindex;

    ret = ioctl(s, SIOCSIFADDR, &ifreq6);
    if (ret < 0) {
        NA_LOG_E("[%s] error in set SIOCSIFADDR:%d - %d:%s",
                __FUNCTION__, ret, errno, strerror(errno));
    }

    ret = ioctl(s, SIOPUSHPENDING, ifr);
    if (ret < 0) {
        NA_LOG_E("[%s] error in set SIOPUSHPENDING:%d - %d:%s",
                __FUNCTION__, ret, errno, strerror(errno));
    }
}

//Configure the IP address to the network interface.
void NetAgentService::configureNetworkInterface(NetAgentReqInfo* pReqInfo, STATUS config) {
    struct ifreq ifr;
    unsigned int interfaceId = 0;
    NA_ADDR_TYPE addrType;
    char addressV4[MAX_IPV4_ADDRESS_LENGTH] = {0};
    char addressV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
    char *reason = NULL;
    int n = 0;

    if (MipcEventHandler::getInterfaceId(pReqInfo->pNetAgentCmdObj,
            &interfaceId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interface id", __FUNCTION__);
        return;
    }

    if (config == ENABLE) {
        NA_LOG_D("[%s] push transIntfId %d to the list", __FUNCTION__, interfaceId);
        m_lTransIntfId.push_back(interfaceId);
    }
    interfaceId %= TRANSACTION_ID_OFFSET;

    if (MipcEventHandler::getAddrType(pReqInfo->pNetAgentCmdObj,
            &addrType) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get addr type", __FUNCTION__);
        return;
    }

    if (config == UPDATE) {
        if (MipcEventHandler::getReason(pReqInfo->pNetAgentCmdObj,
                &reason) != NETAGENT_IO_RET_SUCCESS) {
            NA_LOG_E("[%s] fail to get IP change reason", __FUNCTION__);
            reason = NULL;
        }

        NA_LOG_I("[%s] update interface %d, addr type : %s(%d), ip change reason: %s",
                __FUNCTION__, interfaceId, addrTypeToString(addrType), addrType,
                reason != NULL ? reason : "");
    } else {
        NA_LOG_D("[%s] interface %d to %s, addr type : %s(%d)", __FUNCTION__,
                interfaceId, config == ENABLE ? "UP" : "DOWN",
                addrTypeToString(addrType), addrType);
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    n = snprintf(ifr.ifr_name, IFNAMSIZ, "%s%d", getInterfaceName(), interfaceId);
    if (n < 0) {
        NA_LOG_E("[%s] fail to snprintf", __FUNCTION__);
        return;
    }

    nwIntfIoctlInit();

    if (config == ENABLE || config == UPDATE) {
        if (config == ENABLE) {
            if (mIsThinModemSupport) {
                manageNwIntf(RTM_NEWLINK, NLM_F_CREATE|NLM_F_EXCL, interfaceId);
            }
            // set the network interface down first before up
            // to prevent from unknown exception causing not close related
            // dev file description
            NA_LOG_D("[%s] set network interface down before up", __FUNCTION__);
            setNwIntfDown(ifr.ifr_name);
        } else {
            NA_LOG_D("[%s] reset connections", __FUNCTION__);
            ifc_reset_connections(ifr.ifr_name, addrType);
        }

        switch (addrType) {
            case NETAGENT_IO_ADDR_TYPE_IPv4:
                getIpv4Address(pReqInfo->pNetAgentCmdObj, addressV4);
                break;
            case NETAGENT_IO_ADDR_TYPE_IPv6:
                getIpv6Address(pReqInfo->pNetAgentCmdObj, addressV6);
                break;
            case NETAGENT_IO_ADDR_TYPE_IPv4v6:
                getIpv4v6Address(pReqInfo->pNetAgentCmdObj, addressV4, addressV6);
                break;
            default:
                NA_LOG_E("[%s] get addr type fail", __FUNCTION__);
                break;
        }

        // Configure addr gen mode before set interface up to avoid kernel RS/RA.
        if (config == ENABLE && (strlen(addressV4) > 0 || strlen(addressV6) > 0)) {
            configureAddrGenMode(interfaceId);
        }

        if (strlen(addressV4) > 0) {
            if (config == ENABLE) {
                nwIntfSetFlags(sock_fd, &ifr, IFF_UP, 0);
            }
            nwIntfSetAddr(sock_fd, &ifr, addressV4);
            mInterfaceManager->updateIpv4Addr(interfaceId, addressV4);
            invokeCallback(interfaceId, CALLBACK_REASON_IPV4_CHANGE);
        }

        if (strlen(addressV6) > 0) {
            if (config == ENABLE) {
                nwIntfSetFlags(sock6_fd, &ifr, IFF_UP, 0);
            } else {
                // clear old ipv6 info and wait for new global ipv6 info from kernel
                mInterfaceManager->clearIpv6Info(interfaceId);
            }
            nwIntfSetIpv6Addr(sock6_fd, &ifr, addressV6);
        }
    } else {
        setNwIntfDown(ifr.ifr_name);
        nwIntfSetMtu(sock_fd, &ifr, 1500);
        if (mIsThinModemSupport) {
            manageNwIntf(RTM_DELLINK, 0, interfaceId);
        }
    }

    nwIntfIoctlDeInit();

    if (config == DISABLE) {
        NA_LOG_D("[%s] remove transIntfId %d from the list and last ReqInfo",
                __FUNCTION__, getTransIntfId(interfaceId));
        m_lTransIntfId.remove(getTransIntfId(interfaceId));
        if (m_pRouteHandler != NULL) {
            m_pRouteHandler->removeLastReqInfo(interfaceId);
        }

        clearPdnHandoverInfo(interfaceId);
        mInterfaceManager->clearInterface(interfaceId);
    }
}

void NetAgentService::configureAddrGenMode(unsigned int interfaceId) {
    char path[LENGTH_ADDR_GEN_MODE] = {0};
    int length = snprintf(path, LENGTH_ADDR_GEN_MODE, "/proc/sys/net/ipv6/conf/%s%d/addr_gen_mode",
            getInterfaceName(), interfaceId);
    if (length < 0) {
        NA_LOG_E("[%s] Failed to set path", __FUNCTION__);
        return;
    }

    int fd = open(path, O_CLOEXEC | O_WRONLY);
    if (fd < 0) {
        NA_LOG_E("[%s] Failed to open %s error(%s)", __FUNCTION__, path, strerror(errno));
        return;
    }

    char value[] = "1";
    int ret = write(fd, value, strlen(value));
    if (ret < 0) {
        NA_LOG_E("[%s] Failed to write %s error(%s)", __FUNCTION__, path, strerror(errno));
    }

    if (close(fd) < 0) {
        NA_LOG_E("[%s] Failed to close fd error(%s)", __FUNCTION__, strerror(errno));
    }
}

void NetAgentService::configureKernelMtu(NetAgentReqInfo* pReqInfo) {
    struct ifreq ifr;
    unsigned int interfaceId = 0;
    unsigned int mtuSize = 0;
    int n = 0;

    if (MipcEventHandler::getInterfaceId(pReqInfo->pNetAgentCmdObj,
            &interfaceId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interface id", __FUNCTION__);
        return;
    }

    interfaceId %= TRANSACTION_ID_OFFSET;
    if (MipcEventHandler::getModemPdnMtu(pReqInfo->pNetAgentCmdObj, &mtuSize)
            != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get mtu size", __FUNCTION__);
        return;
    }
    NA_LOG_D("[%s] TransIntfId %d request to set mtuSize=%d",
            __FUNCTION__, interfaceId, mtuSize);

    if (mtuSize > 0) {
        memset(&ifr, 0, sizeof(struct ifreq));
        n = snprintf(ifr.ifr_name, IFNAMSIZ, "%s%d", getInterfaceName(), interfaceId);
        if (n < 0) {
            NA_LOG_E("[%s] fail to snprintf", __FUNCTION__);
            return;
        }

        if (sock_fd > 0) {
            if (close(sock_fd) == -1) {
                NA_LOG_E("[%s] close socket error", __FUNCTION__);
            }
        }

        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_fd < 0) {
            NA_LOG_E("[%s] couldn't create IP socket: errno=%d", __FUNCTION__, errno);
            return;
        }

        nwIntfSetMtu(sock_fd, &ifr, mtuSize);

        if (close(sock_fd) == -1) {
            NA_LOG_E("[%s] close socket error", __FUNCTION__);
        }
        sock_fd = 0;
    }
}

void NetAgentService::configureIpv6Mtu(unsigned int interfaceId, unsigned int mtuSize) {
    int mtuChange = mInterfaceManager->updateMtuV6(interfaceId, mtuSize);
    int ipv6count = mInterfaceManager->getIpv6Count(interfaceId);
    NA_LOG_D("[%s] interfaceId=%d mtuV6=%d mtuV6Change=%d ipv6count=%d",
            __FUNCTION__, interfaceId, mtuSize, mtuChange, ipv6count);

    if (mtuChange == 1 && ipv6count > 0) {
        invokeCallback(interfaceId, CALLBACK_REASON_MTUV6_CHANGE);
    }
}

#define WWAN_PARENT_DEVICE "wwan0"
int preferred_family = AF_UNSPEC;

int NetAgentService::manageNwIntf(unsigned int cmd, unsigned int flags, unsigned int interfaceId) {
    char ifrName[IFNAMSIZ] = {0};
    int index = 0;
    int n = 0;
    struct rtattr *ret_link_info = NULL;
    struct rtattr *ret_info_data = NULL;
    struct rtnl_handle_nl rth = { .fd = -1 };
    struct iplink_req req = {
        .n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
    };
    req.n.nlmsg_flags = NLM_F_REQUEST | flags;
    req.n.nlmsg_type = cmd;
    req.i.ifi_family = preferred_family;

    NA_LOG_I("[%s] enter cmd %d.", __FUNCTION__,cmd);

    if (rtnl_open_nl(&rth, 0) < 0) {
        fprintf(stderr, "%s", "Cannot open rtnetlink\n");
        NA_LOG_E("[%s] fail to rtnl_open_nl", __FUNCTION__);
        return -1;
    }
    if (RTM_NEWLINK == cmd) {
        //1. set parent dev wwan0
        addattr_l(&req.n, sizeof(req), IFLA_PARENT_DEV_NAME,
                WWAN_PARENT_DEVICE, strlen(WWAN_PARENT_DEVICE) + 1);
    }
    req.i.ifi_index = index;

    //2. set dev
    n = snprintf(ifrName, IFNAMSIZ, "%s%d", getInterfaceName(), interfaceId);
    if (n < 0) {
        NA_LOG_E("[%s] fail to snprintf", __FUNCTION__);
        return -1;
    }
    addattr_l(&req.n, sizeof(req),
            IFLA_IFNAME, ifrName, strlen(ifrName) + 1);
    if (RTM_NEWLINK == cmd) {
        //3. set link info
        ret_link_info = addattr_nest(&req.n, sizeof(req), IFLA_LINKINFO);
        addattr_l(&req.n, sizeof(req), IFLA_INFO_KIND, NETWORK_IFNAME_WWAN,
                strlen(NETWORK_IFNAME_WWAN));

        ret_info_data = addattr_nest(&req.n, sizeof(req), IFLA_INFO_DATA);
        //4. set link id
        addattr32(&req.n, 1024, IFLA_WWAN_LINK_ID, interfaceId);
        addattr_nest_end(&req.n, ret_info_data);
        addattr_nest_end(&req.n, ret_link_info);
    }
    //5. set request
    if (rtnl_talk(&rth, &req.n, NULL) < 0) {
        NA_LOG_E("[%s] fail to rtnl_talk", __FUNCTION__);
        return -1;
    }
    rtnl_close_nl(&rth);

    return 0;
}

void NetAgentService::addIpv4Info(unsigned int interfaceId, char* address, unsigned int mtuSize) {
    if (address != NULL && strlen(address) > 0) {
        struct ifreq ifr;
        memset(&ifr, 0, sizeof(struct ifreq));
        int n = snprintf(ifr.ifr_name, IFNAMSIZ, "%s%d", getInterfaceName(), interfaceId);
        if (n < 0) {
            NA_LOG_E("[%s] fail to snprintf", __FUNCTION__);
            return;
        }
        // Up interface and set address.
        nwIntfIoctlInit();
        ifc_reset_connections(ifr.ifr_name, NETAGENT_IO_ADDR_TYPE_IPv4);
        nwIntfSetFlags(sock_fd, &ifr, IFF_UP, 0);
        nwIntfSetAddr(sock_fd, &ifr, address);
        nwIntfIoctlDeInit();

        // Save address and update to rild.
        mInterfaceManager->updateIpv4Addr(interfaceId, address);
        int mtuChange = mInterfaceManager->updateMtuV4(interfaceId, mtuSize);
        int reason = CALLBACK_REASON_IPV4_CHANGE;
        if (mtuChange == 1) {
            reason |= CALLBACK_REASON_MTUV4_CHANGE;
        }
        invokeCallback(interfaceId, reason);
    }
}

void NetAgentService::configureInterfaceChange(NetAgentReqInfo* pReqInfo) {
    unsigned int interfaceId = 0;
    char *reason = NULL;
    unsigned int mtuSize = 0;
    //NA_ADDR_TYPE addrType;
    //char addressV4[MAX_IPV4_ADDRESS_LENGTH] = {0};

    if (MipcEventHandler::getInterfaceId(pReqInfo->pNetAgentCmdObj,
            &interfaceId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interface id", __FUNCTION__);
        return;
    }

    interfaceId %= TRANSACTION_ID_OFFSET;

    if (MipcEventHandler::getReason(pReqInfo->pNetAgentCmdObj,
            &reason) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get reason", __FUNCTION__);
        return;
    }

    // XLAT feature is IPv6 only network for IPv4 traffic.
    // 1. Modem reports IPv6 first.
    // 2. Then modem adds an IPv4 through IFCHANGE.
    // 3. AP set IPv4 to kernel and reported to framework.
    /*if (reason != NULL && strcasecmp(reason, REASON_IFCHANGE_XLAT) == 0) {
        if (mInterfaceManager->getIpv6Count(interfaceId) <= 0) {
            NA_LOG_E("[%s] no ipv6", __FUNCTION__);
            return;
        }

        int n = mInterfaceManager->getIpv4Addr(interfaceId, addressV4);
        if (n < 0 || (n > 0 && strlen(addressV4) > 0)) {
            NA_LOG_E("[%s] ipv4 exsisted or not initialize", __FUNCTION__);
            return;
        }

        if (MipcEventHandler::getAddrType(pReqInfo->pNetAgentCmdObj,
                &addrType) != NETAGENT_IO_RET_SUCCESS) {
            NA_LOG_E("[%s] fail to get addr type", __FUNCTION__);
            return;
        }

        if (addrType != NETAGENT_IO_ADDR_TYPE_IPv4
                || addrType != NETAGENT_IO_ADDR_TYPE_IPv4v6) {
            NA_LOG_E("[%s] wrong addr type %d", __FUNCTION__, addrType);
            return;
        }

        if (MipcEventHandler::getMtu(pReqInfo->pNetAgentCmdObj,
                NETAGENT_IO_ADDR_TYPE_IPv4, &mtuSize) != NETAGENT_IO_RET_SUCCESS) {
            NA_LOG_D("[%s] mtuV4 size is invalid", __FUNCTION__);
        }

        getIpv4Address(pReqInfo->pNetAgentCmdObj, addressV4);
        addIpv4Info(interfaceId, addressV4, mtuSize);
    }*/

    // IPv6 MTU feature.
    //if (reason != NULL && strcasecmp(reason, REASON_IFCHANGE_MTUV6) == 0) {
        if (MipcEventHandler::getMtu(pReqInfo->pNetAgentCmdObj,
                NETAGENT_IO_ADDR_TYPE_IPv6, &mtuSize) != NETAGENT_IO_RET_SUCCESS) {
            NA_LOG_D("[%s] mtuV6 size is invalid", __FUNCTION__);
            return;
        }
        configureIpv6Mtu(interfaceId, mtuSize);
    //}
}

void NetAgentService::queryArp(NetAgentReqInfo* pReqInfo) {
    /*+EWIFIMAC: <ifname>, <ip>*/
    thread_args *args;
    NA_LOG_D("[%s] Enter NETAGENT_IO_CMD_ARP_QUERY event", __FUNCTION__);

    NA_ARP_INFO *arp = (NA_ARP_INFO *)
            calloc(1, sizeof(NA_ARP_INFO));
    if (!arp) {
        NA_LOG_E("[%s] fail to alloc ARP info", __FUNCTION__);
        free(arp);
        return;
    }

    netagent_io_cmd_obj_t *cmd_obj = (netagent_io_cmd_obj_t*)pReqInfo->pNetAgentCmdObj;
    if (!cmd_obj) {
        NA_LOG_E("[%s] obj is NULL", __FUNCTION__);
        free(arp);
        return;
    }
    memcpy(arp, &(cmd_obj->parameter.arp), sizeof(netagent_io_arp_parameter_t));
    NA_LOG_D("arp->ifname: %s", arp->ifname);
    if (UNIT_TEST) {
        NA_LOG_D("arp->ip: %s", arp->ip);
    }

    mipc_msg_t *header = NULL;
    if (MipcEventHandler::getHeader(cmd_obj, &header) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get header", __FUNCTION__);
        free(arp);
        return;
    }

    args = (thread_args*) calloc(1, sizeof(thread_args));
    if (!args) {
        NA_LOG_E("[%s] fail to alloc thread_args info", __FUNCTION__);
        free(args);
        free(arp);
        return;
    }
    args->instance = this;
    args->arp = arp;
    //copy mipc header then
    //deinit at freeNetAgentCmdObj of handleEvent after sendArpResult.
    args->header = mipc_msg_copy_hdr(header);
    MipcEventHandler::printMipcMsgId(args->header, "queryArp");

    pthread_t mArpThread;
    pthread_create(&mArpThread, NULL, NetAgentService::queryArpThread, args);
}

void *NetAgentService::getIpToMacResult(mipc_msg_t *header, netagent_io_arp_parameter_t *arp) {
    netagent_io_cmd_obj_t *cmd_obj = 0;
    struct sockaddr *wlanSocketAddr = 0;
    int fail_code = 0;

    if (header == NULL) {
        NA_LOG_E("[%s] fail to get header", __FUNCTION__);
        return 0;
    }
    cmd_obj = (netagent_io_cmd_obj_t *)calloc(1, sizeof(netagent_io_cmd_obj_t));
    if (!cmd_obj) {
        NA_LOG_E("[%s] Can't allocate cmd obj", __FUNCTION__);
        return 0;
    }
    cmd_obj->cmd = NETAGENT_IO_CMD_ARP_RESULT;
    cmd_obj->header = header;
    wlanSocketAddr =
            (struct sockaddr *) calloc(1, sizeof(struct sockaddr));
    if (!wlanSocketAddr) {
        NA_LOG_E("[%s] Can't allocate wlanSocketAddr", __FUNCTION__);
        free(cmd_obj);
        return 0;
    }
    fail_code= Get_Mac_Addr((const char*)arp->ip,
            (const char*)arp->ifname, wlanSocketAddr);
    if (!fail_code) {
        SocketAddr_ntop((const struct sockaddr * )wlanSocketAddr, arp->mac);
        if (UNIT_TEST) {
            NA_LOG_I("get mac success, ip = %s, mac = %s", arp->ip, arp->mac);
        } else {
            NA_LOG_I("get mac success, mac = %s", arp->mac);
        }
    } else {
        if (UNIT_TEST) {
            NA_LOG_E("get mac fail, ip = %s, err = %d", arp->ip, fail_code);
        } else {
            NA_LOG_E("get mac fail, err = %d", fail_code);
        }
    }
    arp->fail_code = fail_code;
    memcpy (&(cmd_obj->parameter.arp), arp, sizeof(netagent_io_arp_parameter_t));
    free (wlanSocketAddr);
    return cmd_obj;
}

void *NetAgentService::queryArpThread(void *arg) {
    thread_args *args = (thread_args *) arg;
    NA_ARP_INFO *arp = args->arp;
    mipc_msg_t *header = args->header;
    void *pNetAgentCmdObj = 0;

    pthread_detach(pthread_self());
    pNetAgentCmdObj = getIpToMacResult(header, arp);
    if (pNetAgentCmdObj) {
        //enque the result, thus, at cmd sent in same thread.
        args->instance->enqueueReqInfo(pNetAgentCmdObj, REQUEST_TYPE_DDM);
    } else {
        //deinit mipc msg if no cmd obj.
        NA_LOG_E("[%s] no cmd obj", __FUNCTION__);
        mipc_msg_deinit(header);
    }
    free(arp);
    free(args);
    pthread_exit(NULL);
    return NULL;
}

void NetAgentService::sendArpResult(NetAgentReqInfo* pReqInfo) {
    void *pNetAgentCmdObj = pReqInfo->pNetAgentCmdObj;
    if (MipcEventHandler::confirmToMipc(pNetAgentCmdObj) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] send ARP Result info fail", __FUNCTION__);
    }
}

void NetAgentService::reserveTcpUdpPort(NetAgentReqInfo* pReqInfo) {
    unsigned int transactionId = 0;
    NA_CMD cmd;
    unsigned int rsvnAction = 0;
    int interfaceId = 0;
    NA_ADDR_TYPE addrType;
    char addressV4[MAX_IPV4_ADDRESS_LENGTH] = {0};
    char addressV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
    int port = 0;
    int socketDomain = 0;
    int socketType = 0;
    int socketFD = -1;
    char ifrName[IFNAMSIZ] = {0};
    struct sockaddr_in socketAddrV4;
    struct sockaddr_in6 socketAddrV6;
    unsigned int response = 0;
    mipc_msg_t* header = NULL;
    int ret = 0;

    if (MipcEventHandler::getInterfaceId(pReqInfo->pNetAgentCmdObj,
            &transactionId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interface id", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getCommandType(pReqInfo->pNetAgentCmdObj,
            &cmd) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get cmd type", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getRsvnAction(pReqInfo->pNetAgentCmdObj,
            &rsvnAction) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get rsvn action", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getRsvnIfId(pReqInfo->pNetAgentCmdObj,
            &interfaceId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get rsvn interfaceId", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getAddrType(pReqInfo->pNetAgentCmdObj,
            &addrType) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get rsvn addr type", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getRsvnPort(pReqInfo->pNetAgentCmdObj,
            &port) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get rsvn port", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getHeader(pReqInfo->pNetAgentCmdObj,
            &header) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get rsvn port", __FUNCTION__);
        return;
    }

    // 0: alloc, 1: free
    if (rsvnAction == 1) {
        if (m_rsvnFdMap.count(port) > 0) {
            socketFD = m_rsvnFdMap[port];
            close(socketFD);
            m_rsvnFdMap.erase(port);
            response = 1;
        }
    } else {
        if (addrType == NETAGENT_IO_ADDR_TYPE_IPv4) {
            socketDomain = AF_INET;
        } else {
            socketDomain = AF_INET6;
        }
        if (cmd == NETAGENT_IO_CMD_TCP_RSVN) {
            socketType = SOCK_STREAM;
        } else {
            socketType = SOCK_DGRAM;
        }
        if ((socketFD = socket(socketDomain, socketType, 0)) < 0) {
            NA_LOG_E("[%s] Unable to create socket: %s", __FUNCTION__, strerror(errno));
        } else {
            if (interfaceId != -1) {
                if (interfaceId == 99) {
                    strncpy(ifrName, WIFI_IF_NAME, IFNAMSIZ-1);
                } else {
                    int n = snprintf(ifrName, IFNAMSIZ, "%s%d", getInterfaceName(), interfaceId);
                    if (n < 0) {
                        NA_LOG_E("[%s] fail to snprintf", __FUNCTION__);
                        return;
                    }
                }
                if (setsockopt(socketFD, SOL_SOCKET, SO_BINDTODEVICE, ifrName, strlen(ifrName)) < 0) {
                    NA_LOG_E("[%s] Unable to set SO_BINDTODEVICE option: %s",
                            __FUNCTION__, strerror(errno));
                    close(socketFD);
                    socketFD = -1;
                }
            }
            if (socketFD >= 0) {
                if (addrType == NETAGENT_IO_ADDR_TYPE_IPv4) {
                    memset(&socketAddrV4, 0, sizeof(socketAddrV4));
                    socketAddrV4.sin_family = AF_INET;
                    socketAddrV4.sin_port = htons(port);
                    getIpv4Address(pReqInfo->pNetAgentCmdObj, addressV4);
                    socketAddrV4.sin_addr.s_addr = inet_addr(addressV4);
                    if (bind(socketFD, (struct sockaddr *) &socketAddrV4, sizeof(socketAddrV4)) < 0) {
                        NA_LOG_E("[%s] Unable to bind socket with v4 addresss: %s",
                                __FUNCTION__, strerror(errno));
                        close(socketFD);
                        socketFD = -1;
                    }
                } else {
                    memset(&socketAddrV6, 0, sizeof(socketAddrV6));
                    socketAddrV6.sin6_family = AF_INET6;
                    socketAddrV6.sin6_port = htons(port);
                    getIpv6Address(pReqInfo->pNetAgentCmdObj, addressV6);
                    ret = inet_pton(AF_INET6, addressV6, &(socketAddrV6.sin6_addr));
                    if (ret <= 0) {
                        NA_LOG_E("[%s] inet_pton error", __FUNCTION__);
                    }
                    if (bind(socketFD,
                            (struct sockaddr *) &socketAddrV6, sizeof(socketAddrV6)) < 0) {
                        NA_LOG_E("[%s] Unable to bind socket with v6 address: %s",
                                __FUNCTION__, strerror(errno));
                        close(socketFD);
                        socketFD = -1;
                    }
                }
            }
        }
        if (socketFD >= 0) {
            m_rsvnFdMap[port] = socketFD;
            response = 1;
        }
    }

    confirmTcpUdpRsvn(transactionId, cmd, rsvnAction, response, header);
}

int NetAgentService::nanl_open(struct nanl_handle *nah, unsigned int subscriptions, int protocol) {
    int sndbuf = 32768;
    int rcvbuf = 1024 * 1024;
    int one = 1;
    socklen_t addr_len;

    memset(nah, 0, sizeof(*nah));

    nah->proto = protocol;
    nah->fd = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, protocol);
    if (nah->fd < 0) {
        NA_LOG_E("[%s] Cannot open netlink socket: %s(%d)", __FUNCTION__, strerror(errno), errno);
        return -1;
    }
    if (setsockopt(nah->fd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf)) < 0) {
        NA_LOG_E("[%s] Fail to SO_SNDBUF: %s(%d)", __FUNCTION__, strerror(errno), errno);
        return -1;
    }
    if (setsockopt(nah->fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf)) < 0) {
        NA_LOG_E("[%s] Fail to SO_RCVBUF: %s(%d)", __FUNCTION__, strerror(errno), errno);
        return -1;
    }
    /* Older kernels may no support extended ACK reporting */
    setsockopt(nah->fd, SOL_NETLINK, NETLINK_EXT_ACK, &one, sizeof(one));

    memset(&nah->local, 0, sizeof(nah->local));
    nah->local.nl_family = AF_NETLINK;
    nah->local.nl_groups = subscriptions;

    if (bind(nah->fd, (struct sockaddr *)&nah->local, sizeof(nah->local)) < 0) {
        NA_LOG_E("[%s] Cannot bind netlink socket: %s(%d)", __FUNCTION__, strerror(errno), errno);
        return -1;
    }
    addr_len = sizeof(nah->local);
    if (getsockname(nah->fd, (struct sockaddr *)&nah->local, &addr_len) < 0) {
        NA_LOG_E("[%s] Cannot getsockname: %s(%d)", __FUNCTION__, strerror(errno), errno);
        return -1;
    }
    if (addr_len != sizeof(nah->local)) {
        NA_LOG_E("[%s] Wrong address length: %d", __FUNCTION__, addr_len);
        return -1;
    }
    if (nah->local.nl_family != AF_NETLINK) {
        NA_LOG_E("[%s] Wrong address family: %d", __FUNCTION__, nah->local.nl_family);
        return -1;
    }
    time_t t = time(NULL);
    if (-1 == t) {
        NA_LOG_E("[%s] Fail to time()", __FUNCTION__);
        return -1;
    }
    nah->seq = (unsigned int)t;
    return 0;
}

int NetAgentService::nanl_talk(struct nanl_handle *nah, struct nlmsghdr *n,
        struct nlmsghdr *answer, size_t maxlen) {
    int status;
    unsigned int seq = 0;
    struct nlmsghdr *h;
    struct sockaddr_nl nladdr = { .nl_family = AF_NETLINK };
    struct iovec iov = {
        .iov_base = n,
        .iov_len = n->nlmsg_len
    };
    struct msghdr msg = {
        .msg_name = &nladdr,
        .msg_namelen = sizeof(nladdr),
        .msg_iov = &iov,
        .msg_iovlen = 1,
    };
    char buf[32768] = {0};

    n->nlmsg_seq = seq = ++nah->seq;

    if (answer == NULL) {
        n->nlmsg_flags |= NLM_F_ACK;
    }
    status = sendmsg(nah->fd, &msg, 0);
    if (status < 0) {
        NA_LOG_E("[%s] Cannot talk to rtnetlink: %s(%d)", __FUNCTION__, strerror(errno), errno);
        return -1;
    }

    iov.iov_base = buf;
    while (1) {
        iov.iov_len = sizeof(buf);
        status = recvmsg(nah->fd, &msg, 0);

        if (status < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            NA_LOG_E("[%s] netlink receive error: %s(%d)", __FUNCTION__, strerror(errno), errno);
            return -1;
        }
        if (status == 0) {
            NA_LOG_E("[%s] EOF on netlink", __FUNCTION__);
            return -1;
        }
        if (msg.msg_namelen != sizeof(nladdr)) {
            NA_LOG_E("[%s] sender address length == %d", __FUNCTION__, msg.msg_namelen);
            return -1;
        }
        for (h = (struct nlmsghdr *)buf; status >= (int)sizeof(*h); ) {
            int len = h->nlmsg_len;
            int l = len - sizeof(*h);

            if (l < 0 || len > status) {
                if (msg.msg_flags >= 0 && (msg.msg_flags & MSG_TRUNC)) {
                    NA_LOG_E("[%s] Truncated message", __FUNCTION__);
                    return -1;
                }
                NA_LOG_E("[%s] !!!malformed message: len=%d", __FUNCTION__, len);
                return -1;
            }

            if (nladdr.nl_pid != 0 ||h->nlmsg_pid != nah->local.nl_pid ||h->nlmsg_seq != seq) {
                /* Don't forget to skip that message. */
                status -= NLMSG_ALIGN(len);
                h = (struct nlmsghdr *)((char *)h + NLMSG_ALIGN(len));
                continue;
            }

            if (h->nlmsg_type == NLMSG_ERROR) {
                struct nlmsgerr *err = (struct nlmsgerr *)NLMSG_DATA(h);

                if (l < (int)sizeof(struct nlmsgerr)) {
                    NA_LOG_E("[%s] ERROR truncated", __FUNCTION__);
                } else if (!err->error) {
                    if (answer) {
                        memcpy(answer, h, ((maxlen) < (h->nlmsg_len) ? (maxlen) : (h->nlmsg_len)));
                    }
                    return 0;
                }

                /*if (rtnl->proto != NETLINK_SOCK_DIAG && show_rtnl_err) {
                    rtnl_talk_error(h, err, errfn);
                }*/

                errno = -err->error;
                NA_LOG_D("[%s] NLMSG_ERROR: %s(%d)", __FUNCTION__, strerror(errno), errno);
                return -1;
            }

            if (answer) {
                memcpy(answer, h, ((maxlen) < (h->nlmsg_len) ? (maxlen) : (h->nlmsg_len)));
                return 0;
            }

            NA_LOG_E("[%s] Unexpected reply!!!", __FUNCTION__);

            status -= NLMSG_ALIGN(len);
            h = (struct nlmsghdr *)((char *)h + NLMSG_ALIGN(len));
        }

        if (msg.msg_flags >= 0 && (msg.msg_flags & MSG_TRUNC)) {
            NA_LOG_E("[%s] Message truncated", __FUNCTION__);
            continue;
        }

        if (status) {
            NA_LOG_E("[%s] !!!Remnant of size %d", __FUNCTION__, status);
            return -1;
        }
    }
}

void NetAgentService::nanl_close(struct nanl_handle *nah) {
    if (nah->fd >= 0) {
        close(nah->fd);
        nah->fd = -1;
    }
}

void NetAgentService::reserveSpi(NetAgentReqInfo* pReqInfo) {
    struct {
        struct nlmsghdr n;
        union {
            struct xfrm_userspi_info xspi;
            struct xfrm_usersa_id xsid;
        };
    } req ;
    //remove . for build error
    req.n.nlmsg_flags = NLM_F_REQUEST;

    unsigned int transactionId;
    unsigned int action;
    NA_ADDR_TYPE addrType = NETAGENT_IO_ADDR_TYPE_UNKNOWN;
    unsigned int protocol = 0;
    unsigned int mode = 0;
    unsigned int v4Address;
    unsigned int v6Address[4];
    char v6AddressString[MAX_IPV6_ADDRESS_LENGTH] = {0};
    unsigned int min = 0;
    unsigned int max = 0;
    unsigned int multiFreeIndex = 0;
    unsigned int multiFreeCount = 0;
    struct nanl_handle nah;
    mipc_msg_t* header = NULL;

    if (MipcEventHandler::getInterfaceId(pReqInfo->pNetAgentCmdObj,
            &transactionId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interface id", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getRsvnAction(pReqInfo->pNetAgentCmdObj,
            &action) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get rsvn action", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getAddrType(pReqInfo->pNetAgentCmdObj,
            &addrType) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get rsvn addr type", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getIpspiProtocol(pReqInfo->pNetAgentCmdObj,
            &protocol) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get spi protocol", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getIpspiMode(pReqInfo->pNetAgentCmdObj,
            &mode) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get spi mode", __FUNCTION__);
        return;
    }
    if (MipcEventHandler::getHeader(pReqInfo->pNetAgentCmdObj,
            &header) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get spi header", __FUNCTION__);
        return;
    }

    if (action == SPI_FREE || action == SPI_MULTI_FREE) {
        req.n.nlmsg_len = NLMSG_LENGTH(sizeof(req.xsid));
        req.n.nlmsg_type = XFRM_MSG_DELSA;
        req.xsid.spi = htonl(mode);
        req.xsid.proto = protocol;
        memset(&req.xsid.daddr, 0, sizeof(req.xsid.daddr));
        if (addrType == NETAGENT_IO_ADDR_TYPE_IPv4) {
            req.xsid.family = AF_INET;
            if (MipcEventHandler::getIpspiAddrV4(pReqInfo->pNetAgentCmdObj,
                    NETAGENT_IO_SPI_ADDR_DST, &v4Address) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_E("[%s] fail to get spi dst v4 addr", __FUNCTION__);
                return;
            }
            memcpy(&req.xsid.daddr, &v4Address, sizeof(v4Address));
        } else {
            req.xsid.family = AF_INET6;
            if (MipcEventHandler::getIpspiAddrV6(pReqInfo->pNetAgentCmdObj,
                    NETAGENT_IO_SPI_ADDR_DST, v6Address) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_E("[%s] fail to get spi dst v6 addr", __FUNCTION__);
                return;
            }
            convertIpv6ToString(v6AddressString, v6Address);
            convertIpv6ToBinary((unsigned int *)&req.xsid.daddr, v6AddressString);
        }
        if (action == SPI_MULTI_FREE && MipcEventHandler::getIpspiMinMax(
                pReqInfo->pNetAgentCmdObj, &min, &max) == NETAGENT_IO_RET_SUCCESS) {
            if (min > 0 && max > 0) {
                multiFreeIndex = min;
                multiFreeCount = max;
            }
        }
    } else {
        req.n.nlmsg_len = NLMSG_LENGTH(sizeof(req.xspi));
        req.n.nlmsg_type = XFRM_MSG_ALLOCSPI;
        memset(&req.xspi.info.saddr, 0, sizeof(req.xspi.info.saddr));
        memset(&req.xspi.info.id.daddr, 0, sizeof(req.xspi.info.id.daddr));
        if (addrType == NETAGENT_IO_ADDR_TYPE_IPv4) {
            req.xspi.info.family = AF_INET;
            if (MipcEventHandler::getIpspiAddrV4(pReqInfo->pNetAgentCmdObj,
                    NETAGENT_IO_SPI_ADDR_SRC, &v4Address) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_E("[%s] fail to get spi src v4 addr", __FUNCTION__);
                return;
            }
            memcpy(&req.xspi.info.saddr, &v4Address, sizeof(v4Address));
            if (MipcEventHandler::getIpspiAddrV4(pReqInfo->pNetAgentCmdObj,
                    NETAGENT_IO_SPI_ADDR_DST, &v4Address) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_E("[%s] fail to get spi dst v4 addr", __FUNCTION__);
                return;
            }
            memcpy(&req.xspi.info.id.daddr, &v4Address, sizeof(v4Address));
        } else {
            req.xspi.info.family = AF_INET6;
            if (MipcEventHandler::getIpspiAddrV6(pReqInfo->pNetAgentCmdObj,
                    NETAGENT_IO_SPI_ADDR_SRC, v6Address) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_E("[%s] fail to get spi src v6 addr", __FUNCTION__);
                return;
            }
            convertIpv6ToString(v6AddressString, v6Address);
            convertIpv6ToBinary((unsigned int *)&req.xspi.info.saddr, v6AddressString);
            if (MipcEventHandler::getIpspiAddrV6(pReqInfo->pNetAgentCmdObj,
                    NETAGENT_IO_SPI_ADDR_DST, v6Address) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_E("[%s] fail to get spi dst v6 addr", __FUNCTION__);
                return;
            }
            convertIpv6ToString(v6AddressString, v6Address);
            convertIpv6ToBinary((unsigned int *)&req.xspi.info.id.daddr, v6AddressString);
        }
        req.xspi.info.id.proto = protocol;
        req.xspi.info.mode = mode;
        if (MipcEventHandler::getIpspiMinMax(pReqInfo->pNetAgentCmdObj,
                &min, &max) != NETAGENT_IO_RET_SUCCESS) {
            NA_LOG_E("[%s] fail to get spi min/max", __FUNCTION__);
            return;
        }
        req.xspi.min = min;
        if (req.xspi.min < 0x100) {
            req.xspi.min = 0x100;
        }
        req.xspi.max = max;
        if (req.xspi.info.id.proto == IPPROTO_COMP) {
            if (req.xspi.max > 0xffff) {
                req.xspi.max = 0xffff;
            }
        } else {
            if (req.xspi.max > 0x0fffffff) {
                req.xspi.max = 0x0fffffff;
            }
        }
    }

    if (nanl_open(&nah, 0, NETLINK_XFRM) < 0) {
        NA_LOG_E("[%s] fail to nanl_open()", __FUNCTION__);
        if (action == SPI_MULTI_FREE) {
            handleFreeMultiSpi(transactionId,
                    ntohl(req.xsid.spi), 0, multiFreeIndex, multiFreeCount, header);
        } else {
            confirmSpi(transactionId, action, 0, 0, NULL, header);
        }
        if (action == SPI_FREE || action == SPI_MULTI_FREE) {
            unsigned int spi = ntohl(req.xsid.spi);
            if (freeSpi(spi) == 1) dumpSpi(action, spi);
        }
    } else {
        if (action == SPI_FREE || action == SPI_MULTI_FREE) {
            unsigned int spi = ntohl(req.xsid.spi);
            int response = (nanl_talk(&nah, &req.n, NULL, 0) < 0 ? 0 : 1);
            if (response == 0) NA_LOG_D("[%s] fail to nanl_talk()", __FUNCTION__);
            if (action == SPI_MULTI_FREE) {
                handleFreeMultiSpi(transactionId,
                        spi, response, multiFreeIndex, multiFreeCount, header);
            } else {
                confirmSpi(transactionId, action, response, 0, NULL, header);
            }
            if (freeSpi(spi) == 1) dumpSpi(action, spi);
        } else {
            char res_buf[NLMSG_BUF_SIZE] = {};
            struct nlmsghdr *res_n = (struct nlmsghdr *)res_buf;

            if (nanl_talk(&nah, &req.n, res_n, sizeof(res_buf)) < 0) {
                NA_LOG_D("[%s] fail to nanl_talk()", __FUNCTION__);
                confirmSpi(transactionId, action, 0, 0, NULL, header);
            } else {
                struct xfrm_usersa_info *xsinfo = (struct xfrm_usersa_info *)NLMSG_DATA(res_n);
                unsigned int spi = ntohl(xsinfo->id.spi);
                confirmSpi(transactionId, action, spi, 0, NULL, header);
                freeSpi(spi);
                struct xfrm_userspi_info *xspi =
                        (struct xfrm_userspi_info *)calloc(1, sizeof(struct xfrm_userspi_info));
                if (xspi == NULL) {
                    NA_LOG_E("[%s] fail to allocate xfrm_userspi_info", __FUNCTION__);
                } else {
                    memcpy(xspi, &req.xspi, sizeof(req.xspi));
                    m_mapSpiXspi[spi] = xspi;
                }
                dumpSpi(action, spi);

                startSpiMonitorLoop();
            }
        }
    }
    nanl_close(&nah);
}

int NetAgentService::freeSpi(unsigned int spi) {
    if (m_mapSpiXspi.count(spi) > 0) {
        struct xfrm_userspi_info *xspi = m_mapSpiXspi[spi];
        m_mapSpiXspi.erase(spi);
        FREEIF(xspi);
        return 1;
    }
    return 0;
}

void NetAgentService::dumpSpi(int action, unsigned int spi) {
    String8 content("");
    std::map<unsigned int, struct xfrm_userspi_info *>::iterator it = m_mapSpiXspi.begin();

    for (; it != m_mapSpiXspi.end(); ++it) {
        content.append(String8::format("%u,", it->first));
    }

    NA_LOG_D("[%s] %s spi: %u, remained size: %lu, content: %s", __FUNCTION__,
            action == SPI_ALLOC ? "alloc" : "free", spi, m_mapSpiXspi.size(), content.string());
}

void NetAgentService::startSpiMonitorLoop(void) {
    if (mSpiMonitorThread == 0) {
        int ret;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        ret = pthread_create(&mSpiMonitorThread, &attr,
                NetAgentService::spiMonitorThreadStart, this);

        if (ret != 0) {
            NA_LOG_E("[%s] failed to create spi monitor thread ret:%d", __FUNCTION__, ret);
        } else {
            NA_LOG_D("[%s] create spi monitor thread OK, ret:%d, mSpiMonitorThread:%ld",
                    __FUNCTION__, ret, mSpiMonitorThread);
        }
    }
}

void *NetAgentService::spiMonitorThreadStart(void *arg) {
    NetAgentService *me = reinterpret_cast<NetAgentService *>(arg);
    me->runSpiMonitorLoop();
    return NULL;
}

void NetAgentService::runSpiMonitorLoop() {
    do {
        struct nanl_handle nah;

        if (nanl_open(&nah, (1 << (XFRMNLGRP_EXPIRE - 1)), NETLINK_XFRM) < 0) {
            NA_LOG_E("[%s] fail to nanl_open() with group 'XFRMNLGRP_EXPIRE'", __FUNCTION__);
        } else {
            int status;
            struct nlmsghdr *h;
            struct sockaddr_nl nladdr = { .nl_family = AF_NETLINK };
            struct iovec iov;
            struct msghdr msg = {
                .msg_name = &nladdr,
                .msg_namelen = sizeof(nladdr),
                .msg_iov = &iov,
                .msg_iovlen = 1,
            };
            char buf[16384] = {0};

            iov.iov_base = buf;
            do {
                iov.iov_len = sizeof(buf);
                status = recvmsg(nah.fd, &msg, 0);

                if (status < 0) {
                    if (errno == EINTR || errno == EAGAIN) {
                        continue;
                    }
                    NA_LOG_E("[%s] netlink receive error: %s(%d)", __FUNCTION__,
                            strerror(errno), errno);
                    if (errno == ENOBUFS) {
                        continue;
                    }
                    break;
                }
                if (status == 0) {
                    NA_LOG_E("[%s] EOF on netlink", __FUNCTION__);
                    break;
                }
                if (msg.msg_namelen != sizeof(nladdr)) {
                    NA_LOG_E("[%s] sender address length == %d", __FUNCTION__, msg.msg_namelen);
                    break;
                }
                for (h = (struct nlmsghdr *)buf; status >= sizeof(*h); ) {
                    int len = h->nlmsg_len;
                    int l = len - sizeof(*h);

                    if (l < 0 || len > status) {
                        if (msg.msg_flags >= 0 && (msg.msg_flags & MSG_TRUNC)) {
                            NA_LOG_E("[%s] Truncated message", __FUNCTION__);
                            break;
                        }
                        NA_LOG_E("[%s] !!!malformed message: len=%d", __FUNCTION__, len);
                        break;
                    }

                    if (h->nlmsg_type == XFRM_MSG_EXPIRE) {
                        struct xfrm_user_expire *xexp = (struct xfrm_user_expire *)NLMSG_DATA(h);
                        struct xfrm_usersa_info *xsinfo = &xexp->state;
                        int lenExp = h->nlmsg_len;
                        lenExp -= NLMSG_SPACE(sizeof(*xexp));
                        if (lenExp < 0) {
                            NA_LOG_E("[%s] BUG: wrong nlmsg len %d", __FUNCTION__, lenExp);
                        } else {
                            unsigned int spiExpired = ntohl(xsinfo->id.spi);
                            NA_LOG_D("[%s] expired spi: %u", __FUNCTION__, spiExpired);
                            NetEventReqInfo *pNetEventObj =
                                    (NetEventReqInfo *)calloc(1, sizeof(NetEventReqInfo));
                            if (pNetEventObj == NULL) {
                                NA_LOG_E("[%s] can't allocate rild event obj", __FUNCTION__);
                            } else {
                                pNetEventObj->cmd = NETAGENT_IO_CMD_REALLOCATE_SPI;
                                pNetEventObj->parameter.spiExpired = spiExpired;
                                enqueueReqInfo(pNetEventObj, REQUEST_TYPE_NETAGENT);
                            }
                        }
                    }

                    status -= NLMSG_ALIGN(len);
                    h = (struct nlmsghdr *)((char *)h + NLMSG_ALIGN(len));
                }
                if (msg.msg_flags >= 0  && (msg.msg_flags & MSG_TRUNC)) {
                    NA_LOG_E("[%s] message truncated", __FUNCTION__);
                    continue;
                }
                if (status) {
                    NA_LOG_E("[%s] Remnant of size %d", __FUNCTION__, status);
                    break;
                }
            } while (mSpiMonitorThread != 0);
        }
        nanl_close(&nah);
    } while (mSpiMonitorThread != 0);
}

void NetAgentService::reallocateSpi(NetAgentReqInfo* pReqInfo) {
    NetEventReqInfo *pNetEventObj = (NetEventReqInfo *)pReqInfo->pNetAgentCmdObj;
    if (pNetEventObj == NULL) {
        NA_LOG_E("[%s] can't allocate rild event obj", __FUNCTION__);
        return;
    }

    NA_LOG_D("[%s] spiExpired = %u", __FUNCTION__, pNetEventObj->parameter.spiExpired);

    if (m_mapSpiXspi.count(pNetEventObj->parameter.spiExpired) > 0) {
        struct xfrm_userspi_info *xspi = m_mapSpiXspi[pNetEventObj->parameter.spiExpired];
        struct {
            struct nlmsghdr n;
            struct xfrm_userspi_info xspi;
    } req ;
    //remove . for build error
        req.n.nlmsg_flags = NLM_F_REQUEST;
        struct nanl_handle nah;

        req.n.nlmsg_len = NLMSG_LENGTH(sizeof(req.xspi));
        req.n.nlmsg_type = XFRM_MSG_ALLOCSPI;
        memcpy(&req.xspi, xspi, sizeof(req.xspi));
        req.xspi.min = pNetEventObj->parameter.spiExpired;
        req.xspi.max = pNetEventObj->parameter.spiExpired;

        if (nanl_open(&nah, 0, NETLINK_XFRM) < 0) {
            NA_LOG_E("[%s] fail to nanl_open()", __FUNCTION__);
        } else {
            char res_buf[NLMSG_BUF_SIZE] = {};
            struct nlmsghdr *res_n = (struct nlmsghdr *)res_buf;

            if (nanl_talk(&nah, &req.n, res_n, sizeof(res_buf)) < 0) {
                NA_LOG_D("[%s] fail to nanl_talk()", __FUNCTION__);
            } else {
                struct xfrm_usersa_info *xsinfo = (struct xfrm_usersa_info *)NLMSG_DATA(res_n);
                unsigned int spiReallocated = ntohl(xsinfo->id.spi);
                NA_LOG_D("[%s] spiReallocated: %u", __FUNCTION__, spiReallocated);
            }
        }
        nanl_close(&nah);
    } else {
        NA_LOG_D("[%s] spiExpired(%u) is freed, do not reallocate", __FUNCTION__,
                pNetEventObj->parameter.spiExpired);
    }
}

void NetAgentService::handleFreeMultiSpi(unsigned int transactionId, unsigned int spi,
        unsigned int response, unsigned int index, unsigned int count, mipc_msg_t* header) {
    if (index == 0 || count == 0) {
        NA_LOG_E("[%s] error index or count value", __FUNCTION__);
        return;
    }

    if (index <= count) {
        m_mapFreeMultiSpi[spi] = response;
    }

    if (index == count) {
        if (m_mapFreeMultiSpi.size() <= 0) {
            NA_LOG_E("[%s] error m_mapFreeMultiSpi size", __FUNCTION__);
            return;
        }

        std::string content("");
        std::map<unsigned int, unsigned int>::iterator it = m_mapFreeMultiSpi.begin();

        while(it != m_mapFreeMultiSpi.end()) {
            content.append(",");
            content.append(std::to_string(it->first));
            content.append(",");
            content.append(std::to_string(it->second));
            ++it;
        }

        content = content.substr(1, std::string::npos); // remove "," at the beginning
        m_mapFreeMultiSpi.clear();

        void *pNetAgentCmdObj = MipcEventHandler::allocIpspiRsvn(transactionId,
                SPI_MULTI_FREE, -1, count, content.c_str(), header);
        if (MipcEventHandler::confirmToMipc(pNetAgentCmdObj) != NETAGENT_IO_RET_SUCCESS) {
            NA_LOG_E("[%s] free multi spi confirm fail", __FUNCTION__);
        }
        MipcEventHandler::freeCommand(pNetAgentCmdObj);
    }
}

void NetAgentService::syncSpiQueryToModem() {
    NA_LOG_D("[%s]", __FUNCTION__);

    NetEventReqInfo *pNetEventObj = (NetEventReqInfo *)calloc(1, sizeof(NetEventReqInfo));

    if (pNetEventObj == NULL) {
        NA_LOG_E("[%s] can't allocate rild event obj", __FUNCTION__);
        return;
    }
    pNetEventObj->cmd = NETAGENT_IO_CMD_SPI_QUERY;
    enqueueReqInfo(pNetEventObj, REQUEST_TYPE_NETAGENT);
}

void NetAgentService::setSpiQueryToModem() {
    void *pNetAgentCmdObj = MipcEventHandler::allocIpspiQuery();
    if (MipcEventHandler::confirmToMipc(pNetAgentCmdObj) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail", __FUNCTION__);
    }
    MipcEventHandler::freeCommand(pNetAgentCmdObj);
}

void NetAgentService::updateGlobalIpv6Address(NetAgentReqInfo* pReqInfo) {
    char address[INET6_ADDRSTRLEN] = "";
    int ipv6PrefixLength = INVALID_IPV6_PREFIX_LENGTH;
    unsigned int interfaceId = INVALID_INTERFACE_ID;
    ACTION action = ACTION_UNKNOWN;
    int flags = -1;
    long deprecationTime = -1;
    long expirationTime = -1;
    int count = 0;

    if (m_pRouteHandler == NULL) {
        NA_LOG_E("[%s] m_pRouteHandler is NULL", __FUNCTION__);
        return;
    }

    if (m_pRouteHandler->getAddress(pReqInfo->pNetAgentCmdObj, address) != NETLINK_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get address", __FUNCTION__);
        return;
    }

    if (m_pRouteHandler->getIpv6PrefixLength(pReqInfo->pNetAgentCmdObj, &ipv6PrefixLength) != NETLINK_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get ipv6PrefixLength", __FUNCTION__);
        return;
    }

    if (m_pRouteHandler->getInterfaceId(pReqInfo->pNetAgentCmdObj, &interfaceId) != NETLINK_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interfaceId", __FUNCTION__);
        return;
    }

    if (m_pRouteHandler->getAction(pReqInfo->pNetAgentCmdObj, &action) != NETLINK_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get action", __FUNCTION__);
        return;
    }

    if (m_pRouteHandler->getFlags(pReqInfo->pNetAgentCmdObj, &flags) != NETLINK_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get flags", __FUNCTION__);
        return;
    }

    if (m_pRouteHandler->getDeprecationTime(pReqInfo->pNetAgentCmdObj, &deprecationTime) != NETLINK_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get deprecationTime", __FUNCTION__);
        return;
    }

    if (m_pRouteHandler->getExpirationTime(pReqInfo->pNetAgentCmdObj, &expirationTime) != NETLINK_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get expirationTime", __FUNCTION__);
        return;
    }

    if (isIpv6Global(address)) {
        count = mInterfaceManager->getMatchedIpv6Count(interfaceId, address);
        if (count > 0 && action == ACTION_ADDR_UPDATED) {
            NA_LOG_D("[%s] address %s/%d existed already", __FUNCTION__,
                    isUserBuild() ? "xxx" : std::string(address).c_str(), ipv6PrefixLength);
            return;
        }
        if (count == 0 && action == ACTION_ADDR_REMOVED) {
            NA_LOG_D("[%s] address %s/%d removed already", __FUNCTION__,
                    isUserBuild() ? "xxx" : std::string(address).c_str(), ipv6PrefixLength);
            return;
        }

        NA_LOG_I("[%s] %s: %s/%d", __FUNCTION__, NetlinkEventHandler::actionToString(action),
                isUserBuild() ? "xxx" : std::string(address).c_str(), ipv6PrefixLength);

        if (action == ACTION_ADDR_UPDATED) {
            mInterfaceManager->updateIpv6Addr(interfaceId, address, ipv6PrefixLength,
                    flags, deprecationTime, expirationTime);
            int reason = CALLBACK_REASON_IPV6_CHANGE;
            int mtuV6 = mInterfaceManager->getMtuV6(interfaceId);
            if (mtuV6 > 0) {
                reason |= CALLBACK_REASON_MTUV6_CHANGE;
            }
            invokeCallback(interfaceId, reason);
        } else if (action == ACTION_ADDR_REMOVED) {
            mInterfaceManager->removeIpv6Info(interfaceId, address);
            invokeCallback(interfaceId, CALLBACK_REASON_IPV6_CHANGE);
        }
    }
}

void NetAgentService::setHandoverInfo(HandoverInfo hoInfo) {
    MipcEventHandler::setHandoverInfo(hoInfo);
}

void NetAgentService::handlePdnHandoverControl(NetAgentReqInfo* pReqInfo) {
    unsigned int tranId = INVALID_INTERFACE_ID;
    unsigned int interfaceId = INVALID_INTERFACE_ID;
    char addressV4[MAX_IPV4_ADDRESS_LENGTH] = {0};
    char addressV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
    NA_PDN_HO_INFO hoInfo;


    if (MipcEventHandler::getInterfaceId(pReqInfo->pNetAgentCmdObj,
            &interfaceId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interface id", __FUNCTION__);
        return;
    }
    interfaceId %= TRANSACTION_ID_OFFSET;

    if (MipcEventHandler::getHOInfo(pReqInfo->pNetAgentCmdObj,
            &hoInfo) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get handover info", __FUNCTION__);
        return;
    }

    NA_ADDR_TYPE addrType = hoInfo.addr_type;
    if (NETAGENT_IO_HO_STATE_START == hoInfo.hostate) {
        NA_LOG_D("[%s] tid: %d, hostate: %s, result: %s, src_ran: %s, tgt_ran: %s",
                __FUNCTION__, interfaceId, hoStateToString(hoInfo.hostate),
                hoResultToString(hoInfo.is_succ), ranTypeToString(hoInfo.src_ran),
                ranTypeToString(hoInfo.tgt_ran));
        if (NETAGENT_IO_HO_RESULT_SUCCESS == hoInfo.is_succ) {
            switch (addrType) {
                case NETAGENT_IO_ADDR_TYPE_IPv4:
                    getIpv4Address(pReqInfo->pNetAgentCmdObj, addressV4);
                    break;
                case NETAGENT_IO_ADDR_TYPE_IPv6:
                    getIpv6Address(pReqInfo->pNetAgentCmdObj, addressV6);
                    break;
                case NETAGENT_IO_ADDR_TYPE_IPv4v6:
                    getIpv4v6Address(pReqInfo->pNetAgentCmdObj, addressV4, addressV6);
                    break;
                default:
                    // No address
                    break;
            }
            recordPdnHandoverInfo(interfaceId, addrType, addressV4, addressV6);
        }

    } else if (NETAGENT_IO_HO_STATE_STOP == hoInfo.hostate) {
        bool needFlushIpsecPolicy =
                hoInfo.is_succ == NETAGENT_IO_HO_RESULT_SUCCESS &&
                hoInfo.src_ran == NETAGENT_IO_HO_RAN_WIFI &&
                hoInfo.tgt_ran == NETAGENT_IO_HO_RAN_MOBILE;

        NA_LOG_D("[%s] tid: %d, hostate: %s, result: %s, src_ran: %s, tgt_ran: %s, flush_ipsec: %d",
                __FUNCTION__, tranId, hoStateToString(hoInfo.hostate),
                hoResultToString(hoInfo.is_succ), ranTypeToString(hoInfo.src_ran),
                ranTypeToString(hoInfo.tgt_ran), needFlushIpsecPolicy);

        if (needFlushIpsecPolicy) {
            clearIpsec(interfaceId);
        }

        //confirmPdnHandoverControl(tranId);
    }
}

NetAgentPdnInfo *NetAgentService::recordPdnHandoverInfo(
        unsigned int interfaceId, NA_ADDR_TYPE addrType, char *addressV4, char *addressV6) {

    NetAgentPdnInfo *pPdnSrcInfo = getPdnHandoverInfo(interfaceId);
    if (pPdnSrcInfo == NULL) {
        pPdnSrcInfo = (NetAgentPdnInfo *)calloc(1, sizeof(NetAgentPdnInfo));
        if (pPdnSrcInfo == NULL) {
            NA_LOG_E("[%s] can't allocate NetAgentPdnInfo", __FUNCTION__);
            return NULL;
        }
    }

    pPdnSrcInfo->interfaceId = interfaceId;
    pPdnSrcInfo->addrType = addrType;

    switch (addrType) {
        case NETAGENT_IO_ADDR_TYPE_IPv4:
            memcpy(pPdnSrcInfo->addressV4, addressV4, MAX_IPV4_ADDRESS_LENGTH);
            NA_LOG_D("[%s] interfaceId: %d, addrType: %s, addressV4: %s",
                    __FUNCTION__, interfaceId, addrTypeToString(addrType),
                    (isUserBuild() ? MASK_STRING : addressV4));
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv6:
            memcpy(pPdnSrcInfo->addressV6, addressV6, MAX_IPV6_ADDRESS_LENGTH);
            NA_LOG_D("[%s] interfaceId: %d, addrType: %s, addressV6: %s",
                    __FUNCTION__, interfaceId, addrTypeToString(addrType),
                    (isUserBuild()? MASK_STRING : addressV6));
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv4v6:
            memcpy(pPdnSrcInfo->addressV4, addressV4, MAX_IPV4_ADDRESS_LENGTH);
            memcpy(pPdnSrcInfo->addressV6, addressV6, MAX_IPV6_ADDRESS_LENGTH);
            NA_LOG_D("[%s] interfaceId: %d, addrType: %s, addressV4: %s, addressV6: %s",
                    __FUNCTION__, interfaceId, addrTypeToString(addrType),
                    (isUserBuild() ? MASK_STRING : addressV4),
                    (isUserBuild()? MASK_STRING : addressV6));
            break;
        default:
            // No address, shall not go to here.
            break;
    }
    m_pdnHoInfoMap[interfaceId] = pPdnSrcInfo;

    return pPdnSrcInfo;
}

NetAgentPdnInfo *NetAgentService::getPdnHandoverInfo(unsigned int interfaceId) {
    if (m_pdnHoInfoMap.count(interfaceId) > 0) {
        return m_pdnHoInfoMap[interfaceId];
    }
    return NULL;
}

bool NetAgentService::clearPdnHandoverInfo(unsigned int interfaceId) {
    if (m_pdnHoInfoMap.count(interfaceId) > 0) {
        NetAgentPdnInfo *pPdnSrcInfo = m_pdnHoInfoMap[interfaceId];
        m_pdnHoInfoMap.erase(interfaceId);
        if (pPdnSrcInfo != NULL) {
            free(pPdnSrcInfo);
        }
        return true;
    }

    return false;
}

void NetAgentService::clearIpsec(unsigned int interfaceId) {
    NetAgentPdnInfo *pPdnSrcInfo = getPdnHandoverInfo(interfaceId);
    if (pPdnSrcInfo != NULL) {
        switch (pPdnSrcInfo->addrType) {
            case NETAGENT_IO_ADDR_TYPE_IPv4:
                MipcEventHandler::flushIpsecPolicy(pPdnSrcInfo->addressV4,
                        NETAGENT_IO_ADDR_TYPE_IPv4, (isUserBuild() ? 1 : 0));
                break;
            case NETAGENT_IO_ADDR_TYPE_IPv6:
                MipcEventHandler::flushIpsecPolicy(pPdnSrcInfo->addressV6,
                        NETAGENT_IO_ADDR_TYPE_IPv6, (isUserBuild() ? 1 : 0));
                break;
            case NETAGENT_IO_ADDR_TYPE_IPv4v6:
                MipcEventHandler::flushIpsecPolicy(pPdnSrcInfo->addressV4,
                        NETAGENT_IO_ADDR_TYPE_IPv4, (isUserBuild() ? 1 : 0));
                MipcEventHandler::flushIpsecPolicy(pPdnSrcInfo->addressV6,
                        NETAGENT_IO_ADDR_TYPE_IPv6, (isUserBuild() ? 1 : 0));
                break;
            default:
                // No address
                break;
        }
    } else {
        NA_LOG_E("[%s] Can't find NetAgentPdnInfo for tid: %d", __FUNCTION__, interfaceId);
    }
}

void NetAgentService::updatePdnHandoverAddr(NetAgentReqInfo* pReqInfo) {
    struct ifreq ifr;
    unsigned int interfaceId = INVALID_INTERFACE_ID;
    NA_ADDR_TYPE addrType;
    char addressV4[MAX_IPV4_ADDRESS_LENGTH] = {0};
    char addressV6[MAX_IPV6_ADDRESS_LENGTH] = {0};
    char *reason = NULL;
    unsigned int addrV4_;
    unsigned int addrV6_[4];
    int n = 0;

    if (MipcEventHandler::getInterfaceId(pReqInfo->pNetAgentCmdObj,
            &interfaceId) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get interface id", __FUNCTION__);
        return;
    }
    interfaceId %= TRANSACTION_ID_OFFSET;

    if (MipcEventHandler::getAddrType(pReqInfo->pNetAgentCmdObj,
            &addrType) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get addr type", __FUNCTION__);
        return;
    }

    if (MipcEventHandler::getReason(pReqInfo->pNetAgentCmdObj,
            &reason) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] fail to get IP change reason", __FUNCTION__);
        reason = NULL;
    }

    NA_LOG_I("[%s] update interface %d, addr type : %s(%d), reason: %s", __FUNCTION__,
                interfaceId, addrTypeToString(addrType), addrType, reason != NULL ? reason : "");

    NetAgentPdnInfo *pPdnSrcInfo = getPdnHandoverInfo(interfaceId);
    if (pPdnSrcInfo == NULL) {
        NA_LOG_I("[%s] No Handover source NetAgentPdnInfo for id: %d, treat it as common IP change",
                __FUNCTION__, interfaceId);
        configureNetworkInterface(pReqInfo, UPDATE);
        return;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    n = snprintf(ifr.ifr_name, IFNAMSIZ, "%s%d", getInterfaceName(), interfaceId);
    if (n < 0) {
        NA_LOG_E("[%s] fail to snprintf", __FUNCTION__);
        return;
    }

    // add new interface address into kernel
    switch (addrType) {
        case NETAGENT_IO_ADDR_TYPE_IPv4:
            getIpv4Address(pReqInfo->pNetAgentCmdObj, addressV4);
            ifc_add_address(ifr.ifr_name, addressV4, IPV4_REFIX_LENGTH);
            NA_LOG_D("[%s] add addressV4: %s", __FUNCTION__,
                    (isUserBuild()? MASK_STRING : addressV4));
            if (MipcEventHandler::getAddrV4(pReqInfo->pNetAgentCmdObj,
                    &addrV4_) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_I("[%s] fail to get addrV4", __FUNCTION__);
            }
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv6:
            getIpv6Address(pReqInfo->pNetAgentCmdObj, addressV6);
            ifc_add_address(ifr.ifr_name, addressV6, IPV6_REFIX_LENGTH);
            NA_LOG_D("[%s] add addressV6: %s", __FUNCTION__,
                    (isUserBuild()? MASK_STRING : addressV6));
            if (MipcEventHandler::getAddrV6(pReqInfo->pNetAgentCmdObj,
                    addrV6_) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_I("[%s] fail to get addrV4", __FUNCTION__);
            }
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv4v6:
            getIpv4v6Address(pReqInfo->pNetAgentCmdObj, addressV4, addressV6);
            ifc_add_address(ifr.ifr_name, addressV4, IPV4_REFIX_LENGTH);
            ifc_add_address(ifr.ifr_name, addressV6, IPV6_REFIX_LENGTH);
            NA_LOG_D("[%s] add addressV4: %s, addressV6: %s", __FUNCTION__,
                    (isUserBuild()? MASK_STRING : addressV4),
                    (isUserBuild()? MASK_STRING : addressV6));
            if (MipcEventHandler::getAddrV4(pReqInfo->pNetAgentCmdObj,
                    &addrV4_) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_I("[%s] fail to get addrV4", __FUNCTION__);
            }
            if (MipcEventHandler::getAddrV6(pReqInfo->pNetAgentCmdObj,
                    addrV6_) != NETAGENT_IO_RET_SUCCESS) {
                NA_LOG_I("[%s] fail to get addrV4", __FUNCTION__);
            }
            break;
        default:
            NA_LOG_E("[%s] get addr type fail", __FUNCTION__);
            break;
    }

    NA_LOG_I("[%s] update interface %d, before addr type : %s(%d),  update addr type : %s(%d)",
             __FUNCTION__,interfaceId,addrTypeToString(pPdnSrcInfo->addrType),
            pPdnSrcInfo->addrType, addrTypeToString(addrType), addrType);
    // del old interface address into kernel
    switch (pPdnSrcInfo->addrType  & addrType) {
        case NETAGENT_IO_ADDR_TYPE_IPv4:
            ifc_del_address(ifr.ifr_name, pPdnSrcInfo->addressV4, IPV4_REFIX_LENGTH);
            NA_LOG_D("[%s] remove addressV4: %s", __FUNCTION__,
                    (isUserBuild()? MASK_STRING : pPdnSrcInfo->addressV4));
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv6:
            ifc_del_address(ifr.ifr_name, pPdnSrcInfo->addressV6, IPV6_REFIX_LENGTH);
            NA_LOG_D("[%s] remove addressV6: %s", __FUNCTION__,
                    (isUserBuild()? MASK_STRING : pPdnSrcInfo->addressV6));
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv4v6:
            ifc_del_address(ifr.ifr_name, pPdnSrcInfo->addressV4, IPV4_REFIX_LENGTH);
            ifc_del_address(ifr.ifr_name, pPdnSrcInfo->addressV6, IPV6_REFIX_LENGTH);
            NA_LOG_D("[%s] remove addressV4: %s, addressV6: %s",
                    __FUNCTION__,
                    (isUserBuild()? MASK_STRING : pPdnSrcInfo->addressV4),
                    (isUserBuild()? MASK_STRING : pPdnSrcInfo->addressV6));
            break;
        default:
            NA_LOG_E("[%s] remove addr type fail", __FUNCTION__);
            // No address, shall not go to here.
            break;
    }
}

/**
When IPv6 address is removed from kernel via netlink notification,
AP doesn't need to relay it to modem if delAddr is the same address
of interface before handover.

@param interfaceId interface id
@param delAddr IPv6 address be removed from kernel
*/
bool NetAgentService::isNeedNotifyIPv6RemovedToModem(unsigned int interfaceId, char* delAddr) {
    unsigned int addrV6_[4];
    NetAgentPdnInfo *pPdnSrcInfo = getPdnHandoverInfo(interfaceId);

    if (pPdnSrcInfo == NULL) {
        NA_LOG_E("[%s] can not found PdnSrcInfo", __FUNCTION__);
        return true;
    }

    switch (pPdnSrcInfo->addrType) {
        /**
        adjust the format of IP address to be consistent and comparable
        EX 2001:2001:0:0:1::11 -> 2001:0000:0000:0001:0000:0000:0000:0011
        */
        case NETAGENT_IO_ADDR_TYPE_IPv6:
        case NETAGENT_IO_ADDR_TYPE_IPv4v6:
            if (convertIpv6ToBinary(addrV6_, delAddr) < 0) {
                NA_LOG_E("[%s] fail to convert ipv6 address to binary", __FUNCTION__);
                return true;
             }

            if (convertIpv6ToString(delAddr, addrV6_) < 0) {
                NA_LOG_E("[%s] error occurs when converting ipv6 to string", __FUNCTION__);
                return true;
            }

            NA_LOG_I("[%s] compare PdnSrcInfo IPv6:%s, Netlink removed IPv6:%s", __FUNCTION__, pPdnSrcInfo->addressV6, delAddr);
            if (strncmp(delAddr, pPdnSrcInfo->addressV6, strlen(delAddr)) == 0) {
                return false;
            }
            break;
        default:
            // No address, shall not go to here.
            break;
    }
    return true;
}

void NetAgentService::confirmPdnHandoverControl(unsigned int tranId) {
    NA_LOG_D("[%s] tranId %d", __FUNCTION__, tranId);
}

void NetAgentService::confirmTcpUdpRsvn(unsigned int transactionId, NA_CMD cmd,
        unsigned int rsvnAction, unsigned int response, mipc_msg_t* header) {
    void *pNetAgentCmdObj = MipcEventHandler::allocIpportRsvn(transactionId,
            cmd, rsvnAction, response, header);
    if (MipcEventHandler::confirmToMipc(pNetAgentCmdObj) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] send TcpUdpRsvn confirm fail", __FUNCTION__);
    }
    MipcEventHandler::freeCommand(pNetAgentCmdObj);
}

void NetAgentService::confirmSpi(unsigned int transactionId, unsigned int spiAction,
        unsigned int response, unsigned int count, const char* reason, mipc_msg_t* header) {
    void *pNetAgentCmdObj = MipcEventHandler::allocIpspiRsvn(transactionId,
            spiAction, response, count, reason, header);
    if (MipcEventHandler::confirmToMipc(pNetAgentCmdObj) != NETAGENT_IO_RET_SUCCESS) {
        NA_LOG_E("[%s] send TcpUdpRsvn confirm fail", __FUNCTION__);
    }
    MipcEventHandler::freeCommand(pNetAgentCmdObj);
}

void NetAgentService::startMipcEventHandler(void) {
    mMipcEventHandler = new MipcEventHandler();
    mMipcEventHandler->registerToHandleMipcUrc();
}

void NetAgentService::initInterfaceManager(void) {
    mInterfaceManager = new InterfaceManager();
}

void NetAgentService::startNetlinkEventHandler(void) {
    if ((m_pRouteHandler = setupSocket(&mRouteSock, NETLINK_ROUTE,
                                      RTMGRP_IPV6_IFADDR | RTMGRP_IPV6_PREFIX,
                                      NetlinkListener::NETLINK_FORMAT_BINARY)) == NULL) {
        NA_LOG_E("[%s] setup socket fail", __FUNCTION__);
    }
}

NetlinkEventHandler *NetAgentService::setupSocket(int *sock, int netlinkFamily,
        int groups, int format) {

    struct sockaddr_nl nladdr;
    int sz = 64 * 1024;
    int on = 1;

    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    nladdr.nl_pid = getpid();
    nladdr.nl_groups = groups;

    if ((*sock = socket(PF_NETLINK, SOCK_DGRAM | SOCK_CLOEXEC, netlinkFamily)) < 0) {
        NA_LOG_E("[%s] Unable to create netlink socket: %s", __FUNCTION__, strerror(errno));
        return NULL;
    }

    if (setsockopt(*sock, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz)) < 0) {
        NA_LOG_E("[%s] Unable to set uevent socket SO_RCVBUFFORCE option: %s",
                __FUNCTION__, strerror(errno));
        close(*sock);
        return NULL;
    }

    if (setsockopt(*sock, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)) < 0) {
        NA_LOG_E("[%s] Unable to set uevent socket SO_PASSCRED option: %s",
                __FUNCTION__, strerror(errno));
        close(*sock);
        return NULL;
    }

    if (bind(*sock, (struct sockaddr *) &nladdr, sizeof(nladdr)) < 0) {
        NA_LOG_E("[%s] Unable to bind netlink socket: %s", __FUNCTION__, strerror(errno));
        close(*sock);
        return NULL;
    }

    NetlinkEventHandler *handler = new NetlinkEventHandler(this, *sock, format);
    if (handler == NULL) {
        NA_LOG_E("[%s] new NetlinkEventHandler fail", __FUNCTION__);
        close(*sock);
        return NULL;
    }

    if (handler->start() < 0) {
        NA_LOG_E("[%s] Unable to start NetlinkEventHandler: %s", __FUNCTION__, strerror(errno));
        delete handler;
        handler = NULL;
        close(*sock);
        return NULL;
    }

    return handler;
}

bool NetAgentService::isIpv6Global(const char *ipv6Addr) {
    if (ipv6Addr) {
        struct sockaddr_in6 sa;
        int ret = 0;

        if (strncasecmp("FE80", ipv6Addr, strlen("FE80")) == 0) {
            NA_LOG_I("[%s] not global", __FUNCTION__);
            return false;
        }

        // ret: -1, error occurs, ret: 0, invalid address, ret: 1, success;
        ret = inet_pton(AF_INET6, ipv6Addr, &(sa.sin6_addr));
        if (ret <= 0) {
            NA_LOG_E("[%s] ipv6 address: %s, inet_pton ret: %d", __FUNCTION__,
                    isUserBuild() ? "xxx" : ipv6Addr, ret);
            return false;
        }

        if (IN6_IS_ADDR_MULTICAST(&sa.sin6_addr)) {
            NA_LOG_I("[%s] multi-cast", __FUNCTION__);
            if (IN6_IS_ADDR_MC_GLOBAL(&sa.sin6_addr)) {
                NA_LOG_D("[%s] global", __FUNCTION__);
                return true;
            } else {
                NA_LOG_I("[%s] not global", __FUNCTION__);
            }
        } else {
            if (IN6_IS_ADDR_LINKLOCAL(&sa.sin6_addr)) {
                NA_LOG_I("[%s] link-local", __FUNCTION__);
            } else if (IN6_IS_ADDR_SITELOCAL(&sa.sin6_addr)) {
                NA_LOG_I("[%s] site-local", __FUNCTION__);
            } else if (IN6_IS_ADDR_V4MAPPED(&sa.sin6_addr)) {
                NA_LOG_I("[%s] v4mapped", __FUNCTION__);
            } else if (IN6_IS_ADDR_V4COMPAT(&sa.sin6_addr)) {
                NA_LOG_I("[%s] v4compat", __FUNCTION__);
            } else if (IN6_IS_ADDR_LOOPBACK(&sa.sin6_addr)) {
                NA_LOG_I("[%s] host", __FUNCTION__);
            } else if (IN6_IS_ADDR_UNSPECIFIED(&sa.sin6_addr)) {
                NA_LOG_I("[%s] unspecified", __FUNCTION__);
            } else if (_IN6_IS_ULA(&sa.sin6_addr)) {
                NA_LOG_D("[%s] uni-local", __FUNCTION__);
                return true;
            } else {
                NA_LOG_D("[%s] global", __FUNCTION__);
                return true;
            }
        }
    } else {
        NA_LOG_E("[%s] input ipv6 address is null!!", __FUNCTION__);
    }
    return false;
}

int NetAgentService::getCommand(void* obj, REQUEST_TYPE reqType, NA_CMD *cmd) {
    if (reqType == REQUEST_TYPE_DDM) {
        if (MipcEventHandler::getCommandType(obj, cmd) != NETAGENT_IO_RET_SUCCESS) {
            NA_LOG_E("[%s] get %s command fail", __FUNCTION__, reqTypeToString(reqType));
            return -1;
        }
    } else if (reqType == REQUEST_TYPE_NETLINK) {
        if (m_pRouteHandler->getCommandType(obj, cmd) != NETLINK_RET_SUCCESS) {
            NA_LOG_E("[%s] get %s command fail", __FUNCTION__, reqTypeToString(reqType));
            return -1;
        }
    } else if (reqType == REQUEST_TYPE_NETAGENT) {
        NetEventReqInfo *pNetEventReqInfo = (NetEventReqInfo *)obj;
        *cmd = pNetEventReqInfo->cmd;
    } else {
        NA_LOG_E("[%s] request is %s(%d)", __FUNCTION__, reqTypeToString(reqType), reqType);
        return -1;
    }
    return 0;
}

void NetAgentService::getIpv4Address(void *obj, char *addressV4) {
    unsigned int addrV4_;
    if (MipcEventHandler::getAddrV4(obj, &addrV4_) == NETAGENT_IO_RET_SUCCESS) {
        if (addrV4_ != 0) {
            if (convertIpv4ToString(addressV4, &addrV4_) < 0) {
                NA_LOG_E("[%s] error occurs when converting ipv4 to string", __FUNCTION__);
            }
        } else {
            NA_LOG_I("[%s] IPv4 address lost after IRAT", __FUNCTION__);
        }
    } else {
        NA_LOG_E("[%s] error occurs when parsing addressV4", __FUNCTION__);
    }
}

void NetAgentService::getIpv6Address(void *obj, char *addressV6) {
    unsigned int addrV6_[4];
    if (MipcEventHandler::getAddrV6(obj, addrV6_) == NETAGENT_IO_RET_SUCCESS) {
        if (!(addrV6_[0] == 0 && addrV6_[1] == 0 && addrV6_[2] == 0 && addrV6_[3] == 0)) {
            if (convertIpv6ToString(addressV6, addrV6_) < 0) {
                NA_LOG_E("[%s] error occurs when converting ipv6 to string", __FUNCTION__);
            }
        } else {
            NA_LOG_I("[%s] IPv6 address lost after IRAT", __FUNCTION__);
        }
    } else {
        NA_LOG_E("[%s] error occurs when parsing addressV6", __FUNCTION__);
    }
}

void NetAgentService::getIpv4v6Address(void *obj, char *addressV4, char *addressV6) {
    getIpv4Address(obj, addressV4);
    getIpv6Address(obj, addressV6);
}

int NetAgentService::convertIpv6ToBinary(unsigned int *output, char *input) {
    int ret = 1;
    struct in6_addr v6Address;
    memset(&v6Address, 0, sizeof(v6Address));
    // ret: -1, error occurs, ret: 0, invalid address, ret: 1, success;
    ret = inet_pton(AF_INET6, input, &v6Address);
    if (ret >= 0) {
        memcpy(output, &v6Address, 16);
        return 0;
    }
    return -1;
}

int NetAgentService::convertIpv4ToString(char *output, unsigned int *input) {
    unsigned char *address = reinterpret_cast<unsigned char *>(input);
    if (output == NULL || address == NULL) {
        NA_LOG_E("[%s] null occurs on output = %s or addressV4 = %s", __FUNCTION__, output, address);
        return -1;
    }
    int n = sprintf(output, "%d.%d.%d.%d", *address, *(address+1), *(address+2), *(address+3));
    if (n < 0) {
        NA_LOG_E("[%s] error on sprintf", __FUNCTION__);
        return -1;
    }
    return 0;
}

int NetAgentService::convertIpv6ToString(char *output, unsigned int *input) {
    unsigned char *address = reinterpret_cast<unsigned char *>(input);
    if (output == NULL || address == NULL) {
        NA_LOG_E("[%s] null occurs on output = %s or addressV6 = %s", __FUNCTION__, output, address);
        return -1;
    }
    int n = sprintf(output, "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
            *address, *(address+1), *(address+2), *(address+3),
            *(address+4), *(address+5), *(address+6), *(address+7),
            *(address+8), *(address+9), *(address+10), *(address+11),
            *(address+12), *(address+13), *(address+14), *(address+15));
    if (n < 0) {
        NA_LOG_E("[%s] error on sprintf", __FUNCTION__);
        return -1;
    }
    return 0;
}

void NetAgentService::freeNetAgentCmdObj(NetAgentReqInfo *pReqInfo) {
    if (pReqInfo->reqType == REQUEST_TYPE_DDM) {
        MipcEventHandler::freeCommand(pReqInfo->pNetAgentCmdObj);
    } else if (pReqInfo->reqType == REQUEST_TYPE_NETLINK) {
        m_pRouteHandler->freeNetlinkEventObj(pReqInfo->pNetAgentCmdObj);
    } else if (pReqInfo->reqType == REQUEST_TYPE_NETAGENT) {
        FREEIF(pReqInfo->pNetAgentCmdObj);
    }
}

const char *NetAgentService::cmdToString(NA_CMD cmd) {
    switch (cmd) {
        case NETAGENT_IO_CMD_IFST: return "IFST";
        case NETAGENT_IO_CMD_IPUPDATE: return "IPUPDATE";
        case NETAGENT_IO_CMD_IFUP: return "IFUP";
        case NETAGENT_IO_CMD_IFDOWN: return "IFDOWN";
        case NETAGENT_IO_CMD_SETMTU: return "SETMTU";
        case NETAGENT_IO_CMD_PDNHO: return "PDNHO";
        case NETAGENT_IO_CMD_IPCHG: return "IPCHG";
        default: return "UNKNOWN";
    }
}

const char *NetAgentService::addrTypeToString(NA_ADDR_TYPE addrType) {
    switch (addrType) {
        case NETAGENT_IO_ADDR_TYPE_IPv4: return "IPV4";
        case NETAGENT_IO_ADDR_TYPE_IPv6: return "IPV6";
        case NETAGENT_IO_ADDR_TYPE_IPv4v6: return "IPV4V6";
        default: return "UNKNOWN";
    }
}

const char *NetAgentService::reqTypeToString(REQUEST_TYPE reqType) {
    switch (reqType) {
        case REQUEST_TYPE_DDM: return "DDM";
        case REQUEST_TYPE_NETLINK: return "NETLINK";
        case REQUEST_TYPE_NETAGENT: return "NETAGENT";
        default: return "UNKNOWN";
    }
}

const char *NetAgentService::ranTypeToString(NA_RAN_TYPE ranType) {
    switch (ranType) {
        case NETAGENT_IO_HO_RAN_MOBILE: return "MOBILE";
        case NETAGENT_IO_HO_RAN_WIFI: return "WIFI";
        default: return "UNKNOWN";
    }
}

const char *NetAgentService::hoStateToString(int state) {
    switch (state) {
        case 0: return "START";
        case 1: return "STOP";
        default: return "UNKNOWN";
    }
}

const char *NetAgentService::hoResultToString(int result) {
    switch (result) {
        case 0: return "FAIL";
        case 1: return "SUCCESS";
        default: return "UNKNOWN";
    }
}

void NetAgentService::registerInterfaceInfoChanged(const sp<NetActionBase1>& action, int slotId) {
    NA_LOG_I("[%s] action=%p, slotId=%d, mSimCount=%d",
            __FUNCTION__, action.get(), slotId, mSimCount);
    if (m_pNetAgentCallback1 == NULL) {
        NA_LOG_E("[%s] m_pNetAgentCallback1 is null", __FUNCTION__);
        action->ack1(false);
        return;
    }

    if (slotId < 0 || slotId >= mSimCount) {
        NA_LOG_E("[%s] invalid slotId %d", __FUNCTION__, slotId);
        action->ack1(false);
        return;
    }
    m_pNetAgentCallback1[slotId].action1 = action;
}

int NetAgentService::queryInterfaceInfo(int interfaceId, InterfaceInfo** interfaceInfo) {
    NA_LOG_I("[%s] interfaceId=%d", __FUNCTION__, interfaceId);
    return mInterfaceManager->getInterfaceInfo(interfaceId, interfaceInfo);
}

void NetAgentService::invokeCallback(int interfaceId, int reason) {
    if (m_pNetAgentCallback1 == NULL) {
        NA_LOG_E("[%s] NetAgentCallback1 is null", __FUNCTION__);
        return;
    }

    CallbackInfo *pCallbackInfo = (CallbackInfo *)calloc(1, sizeof(CallbackInfo));
    if (pCallbackInfo == NULL) {
        NA_LOG_E("[%s] allocate CallbackInfo fail", __FUNCTION__);
        return;
    }

    pCallbackInfo->interfaceInfo = (InterfaceInfo *)calloc(1, sizeof(InterfaceInfo));
    if (pCallbackInfo->interfaceInfo == NULL) {
        NA_LOG_E("[%s] allocate InterfaceInfo fail", __FUNCTION__);
        FREEIF(pCallbackInfo);
        return;
    }

    pCallbackInfo->reason = reason;
    if (mInterfaceManager->getInterfaceInfo(interfaceId, &pCallbackInfo->interfaceInfo) == -1) {
        NA_LOG_E("[%s] get InterfaceInfo fail", __FUNCTION__);
        FREEIF(pCallbackInfo->interfaceInfo);
        FREEIF(pCallbackInfo);
        return;
    }

    NA_LOG_I("[%s] interfaceId=%d reason=%d mtuV4=%d mtuV6=%d addrV4=%s addrV6 count=%d",
            __FUNCTION__, interfaceId, pCallbackInfo->reason, pCallbackInfo->interfaceInfo->mtuV4,
            pCallbackInfo->interfaceInfo->mtuV6,
            isUserBuild() ? "xxx" : pCallbackInfo->interfaceInfo->addressV4,
            pCallbackInfo->interfaceInfo->globalIpv6Count);

    for (int i = 0; i < mSimCount; i++) {
        if (m_pNetAgentCallback1[i].action1 == NULL) {
            NA_LOG_E("[%s] NetAgentCallback1[%d].action1 is null", __FUNCTION__, i);
            continue;
        }
        m_pNetAgentCallback1[i].action1->ack1(true, pCallbackInfo);
    }

    FREEIF(pCallbackInfo->interfaceInfo);
    FREEIF(pCallbackInfo);
}

bool NetAgentService::isUserBuild() {
    return mIsUserBuild;
}

bool NetAgentService::isTransIntfIdMatched(int transIntfId) {
    for (std::list<int>::iterator it = m_lTransIntfId.begin(); it != m_lTransIntfId.end(); ++it) {
        if (*it == transIntfId) {
            return true;
        }
    }
    NA_LOG_I("[%s] transIntfId %d is not matched", __FUNCTION__, transIntfId);
    return false;
}

int NetAgentService::getTransIntfId(int interfaceId) {
    if (interfaceId == INVALID_INTERFACE_ID) {
        NA_LOG_E("[%s] invalid interfaceId", __FUNCTION__);
        return INVALID_TRANS_INTF_ID;
    }

    for (std::list<int>::iterator it = m_lTransIntfId.begin(); it != m_lTransIntfId.end(); it++) {
        if (((*it) % TRANSACTION_ID_OFFSET) == interfaceId) {
            return *it;
        }
    }
    return INVALID_TRANS_INTF_ID;
}


// Test mode start.
void NetAgentService::setTransactionInterfaceId(int transIntfId) {
    NA_LOG_D("[%s] transIntfId = %d", __FUNCTION__, transIntfId);
    if (transIntfId == INVALID_TRANS_INTF_ID) {
        return;
    }
    m_lTransIntfId.push_back(transIntfId);
}

void NetAgentService::removeTransactionInterfaceId(int transIntfId) {
    NA_LOG_D("[%s] transIntfId = %d", __FUNCTION__, transIntfId);
    if (transIntfId == INVALID_TRANS_INTF_ID) {
        return;
    }
    m_lTransIntfId.remove(transIntfId);
}

void NetAgentService::removeAllTransactionInterfaceId() {
    NA_LOG_D("[%s] X", __FUNCTION__);
    m_lTransIntfId.clear();
}
// Test mode end.
