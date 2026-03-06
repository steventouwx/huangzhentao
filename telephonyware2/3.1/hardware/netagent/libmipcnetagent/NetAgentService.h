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

#ifndef __NET_AGENT_SERVICE_H__
#define __NET_AGENT_SERVICE_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/xfrm.h>
#include <sysutils/NetlinkListener.h>
#include <list>
#include <map>
#include <dirent.h>
#include <mtk_log.h>
#include <sys/time.h>
#include <pthread.h>
#include <string>

#include <fcntl.h>
#include <netinet/in.h>
#include <linux/if_link.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
//#include "ares/mtk_ares.h"
#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#include "netutils/ifc.h"
#include "mtkrilutils/libmtkrilutils.h"

#include "MipcEventHandler.h"
#include "InterfaceManager.h"
#include "NetAction.h"
#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "netlink/libnetlink.h"
#ifdef __cplusplus
} // closing brace for extern "C"
#endif
//#include "netlink/utils.h"
/*****************************************************************************
 * Defines
 *****************************************************************************/

#define NA_CMD netagent_io_cmd_e
#define NA_ADDR_TYPE netagent_io_addr_type_e
#define NA_RAN_TYPE netagent_io_ho_ran_e
#define NA_IFST netagent_io_ifst_e
#define NA_MTU_SIZE netagent_io_mtu_e
#define NA_RA netagent_io_ra_e
#define NA_PDN_HO_INFO netagent_io_ho_info_t
#define NA_ARP_INFO netagent_io_arp_parameter_t

#ifdef __cplusplus
extern "C" {
#endif
int Get_Mac_Addr(const char *ipStr, const char *ifname, struct sockaddr *WlanSocketAddr);
char *SocketAddr_ntop(const struct sockaddr *SockAddr, char *bufp);
#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#define DEFAULT_SIM_COUNT 2
#define NETWORK_IFNAME_CCMNI "ccmni"
#define NETWORK_IFNAME_WWAN "wwan"
#define LENGTH_ADDR_GEN_MODE 100
#define INVALID_AID -1
#define MAX_MTU_SIZE_LENGTH 5
#define IPV6_PREFIX "FE80:0000:0000:0000:"
#define NULL_IPV6_ADDRESS "0::0"
#define INVALID_IPV6_PREFIX_LENGTH -1
#define TRANSACTION_ID_OFFSET 100
#define INVALID_TRANS_INTF_ID -1
#define IPV4_REFIX_LENGTH 32
#define IPV6_REFIX_LENGTH 64
#define NLMSG_BUF_SIZE 4096
#define WIFI_IF_NAME "wlan0"
#define IFA_F_DEPRECATED 0x20
#define NIP6(addr) \
        ntohs((addr).s6_addr16[0]), \
        ntohs((addr).s6_addr16[1]), \
        ntohs((addr).s6_addr16[2]), \
        ntohs((addr).s6_addr16[3]), \
        ntohs((addr).s6_addr16[4]), \
        ntohs((addr).s6_addr16[5]), \
        ntohs((addr).s6_addr16[6]), \
        ntohs((addr).s6_addr16[7])
#define NIP6_FMT "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"
#define _IN6_IS_ULA(a)  \
    ((((a)->s6_addr[0] & 0xff) == 0xfc) || (((a)->s6_addr[0] & 0xff) == 0xfd))

#define REASON_IFCHANGE_NORMAL     "0"
#define REASON_IFCHANGE_MTUV6      "1"
#define REASON_IFCHANGE_XLAT       "2"

/*****************************************************************************
 * Class NetAgentService
 *****************************************************************************/
class NetlinkEventHandler;

typedef enum {
    // Request is from Data Domain Manager(DDM) which is used for NW-IF operation.
    REQUEST_TYPE_DDM = 0,
    // Request is from Netlink Event Handler which is used for Netlink operation.
    REQUEST_TYPE_NETLINK = 1,
    // Request is from NetAgent which is used to control network state currently.
    REQUEST_TYPE_NETAGENT = 2,
} REQUEST_TYPE;

typedef struct NetAgentReqInfo {
    struct NetAgentReqInfo *pNext;
    void *pNetAgentCmdObj;
    NA_CMD cmdType;
    REQUEST_TYPE reqType;
} NetAgentReqInfo;

typedef enum {
    DISABLE = 0,
    ENABLE = 1,
    UPDATE = 2
} STATUS;

typedef enum {
    SPI_ALLOC = 0,
    SPI_FREE = 1,
    SPI_MULTI_FREE = 2,
} SpiReservation;

typedef struct {
    NA_CMD cmd;
    sp<NetActionBase> action;
    union {
        unsigned int spiExpired;
        int interfaceId;
    } parameter;
} NetEventReqInfo;

typedef struct {
    int interfaceId;
    int cid;
    NA_ADDR_TYPE addrType;
    char addressV4[MAX_IPV4_ADDRESS_LENGTH];
    char addressV6[MAX_IPV6_ADDRESS_LENGTH];
} NetAgentPdnInfo;

struct nanl_handle {
    int fd;
    struct sockaddr_nl local;
    unsigned int seq;
    int proto;
};

typedef struct {
    sp<NetActionBase1> action1;
} NetAgentCallback1;

enum {
    IFLA_WWAN_UNSPEC,
    IFLA_WWAN_LINK_ID, /* u32 */

    __IFLA_WWAN_MAX
};
#define IFLA_WWAN_MAX (__IFLA_WWAN_MAX - 1)

struct iplink_req {
    struct nlmsghdr n;
    struct ifinfomsg i;
    char buf[1024];
};

class NetAgentService {
    public:
        NetAgentService();
        virtual ~NetAgentService();
        static bool createNetAgentService();

    public:
        static NetAgentService* getInstance();
        const char* getInterfaceName();
        void enqueueReqInfo(void* obj, REQUEST_TYPE reqType);
        static const char *cmdToString(NA_CMD cmd);
        static const char *addrTypeToString(NA_ADDR_TYPE addrType);
        static const char *reqTypeToString(REQUEST_TYPE reqType);
        static const char *ranTypeToString(NA_RAN_TYPE ranType);
        static const char *hoStateToString(int state);
        static const char *hoResultToString(int result);
        void registerInterfaceInfoChanged(const sp<NetActionBase1>& action, int slotId);
        void invokeCallback(int interfaceId, int type);
        int queryInterfaceInfo(int interfaceId, InterfaceInfo** interfaceInfo);
        bool isUserBuild();
        int getTransIntfId(int interfaceId);
        bool isTransIntfIdMatched(int transIntfId);
        // Test mode start.
        void setTransactionInterfaceId(int transIntfId);
        void setHandoverInfo(HandoverInfo hoInfo);
        void removeTransactionInterfaceId(int transIntfId);
        void removeAllTransactionInterfaceId();
        // Test mode end.

    private:
        void init();
        void startEventLoop(void);
        static void *eventThreadStart(void *arg);
        static void *queryArpThread(void *arp);
        void runEventLoop();
        void handleEvent(NetAgentReqInfo* pReqInfo);
        NetAgentReqInfo *createNetAgentReqInfo(void* obj, REQUEST_TYPE reqType, NA_CMD cmd);
        NetAgentReqInfo *dequeueReqInfo();
        void setNwIntfDown(const char *interfaceName);
        void nwIntfSetMtu(int s, struct ifreq *ifr, int mtu);
        void nwIntfIoctlInit();
        void nwIntfIoctlDeInit();
        void nwIntfSetFlags(int s, struct ifreq *ifr, int set, int clr);
        inline void nwIntfInitSockAddrIn(struct sockaddr_in *sin, const char *addr);
        void nwIntfSetAddr(int s, struct ifreq *ifr, const char *addr);
        void nwIntfSetIpv6Addr(int s, struct ifreq *ifr, const char *addr);
        void configureNetworkInterface(NetAgentReqInfo* pReqInfo, STATUS isUp);
        void configureKernelMtu(NetAgentReqInfo* pReqInfo);
        void configureAddrGenMode(unsigned int interfaceId);
        void configureIpv6Mtu(unsigned int interfaceId, unsigned int mtuSize);
        void addIpv4Info(unsigned int interfaceId, char* address, unsigned int mtuSize);
        void configureInterfaceChange(NetAgentReqInfo* pReqInfo);
        void updateGlobalIpv6Address(NetAgentReqInfo* pReqInfo);
        void confirmPdnHandoverControl(unsigned int tranId);
        void handlePdnHandoverControl(NetAgentReqInfo* pReqInfo);
        void updatePdnHandoverAddr(NetAgentReqInfo* pReqInfo);
        NetAgentPdnInfo *recordPdnHandoverInfo(unsigned int interfaceId, NA_ADDR_TYPE addrType, char *addressV4, char *addressV6);
        NetAgentPdnInfo *getPdnHandoverInfo(unsigned int interfaceId);
        bool clearPdnHandoverInfo(unsigned int interfaceId);
        void clearIpsec(unsigned int interfaceId);
        bool isNeedNotifyIPv6RemovedToModem(unsigned int interfaceId, char* delAddr);

        void startNetlinkEventHandler(void);
        void startMipcEventHandler(void);
        void initInterfaceManager(void);
        NetlinkEventHandler *setupSocket(int *sock, int netlinkFamily, int groups, int format);
        bool isIpv6Global(const char *ipv6Addr);
        int getCommand(void* obj, REQUEST_TYPE reqType, NA_CMD *cmd);
        void getIpv4Address(void *obj, char *addressV4);
        void getIpv6Address(void *obj, char *addressV6);
        void getIpv4v6Address(void *obj, char *addressV4, char *addressV6);
        int convertIpv6ToBinary(unsigned int *output, char *input);
        int convertIpv4ToString(char *output, unsigned int *input);
        int convertIpv6ToString(char *output, unsigned int *input);
        void freeNetAgentCmdObj(NetAgentReqInfo *pReqInfo);
        void queryArp(NetAgentReqInfo* pReqInfo);
        void sendArpResult(NetAgentReqInfo* pReqInfo);
        static void *getIpToMacResult(mipc_msg_t *header, netagent_io_arp_parameter_t *arp);
        void reserveTcpUdpPort(NetAgentReqInfo* pReqInfo);
        void reserveSpi(NetAgentReqInfo* pReqInfo);
        int freeSpi(unsigned int spi);
        void dumpSpi(int action, unsigned int spi);
        void startSpiMonitorLoop(void);
        static void *spiMonitorThreadStart(void *arg);
        void runSpiMonitorLoop();
        void reallocateSpi(NetAgentReqInfo* pReqInfo);
        void handleFreeMultiSpi(unsigned int transactionId, unsigned int spi, unsigned int response,
                unsigned int index, unsigned int count, mipc_msg_t* header);
        void syncSpiQueryToModem();
        void setSpiQueryToModem();
        void confirmTcpUdpRsvn(unsigned int transactionId, NA_CMD cmd, unsigned int rsvnAction,
                unsigned int response, mipc_msg_t* header);
        void confirmSpi(unsigned int transactionId, unsigned int spiAction,
                unsigned int response, unsigned int count, const char* reason,
                mipc_msg_t* header);
        int nanl_open(struct nanl_handle *nah, unsigned int subscriptions, int protocol);
        int nanl_talk(struct nanl_handle *nah, struct nlmsghdr *n, struct nlmsghdr *answer, size_t maxlen);
        void nanl_close(struct nanl_handle *nah);
        int manageNwIntf(unsigned int cmd, unsigned int flags, unsigned int interfaceId);

    private:
        static pthread_mutex_t sInitMutex;
        static NetAgentService* sInstance;

        InterfaceManager *mInterfaceManager;
        MipcEventHandler *mMipcEventHandler;
        NetAgentCallback1 *m_pNetAgentCallback1;
        NetlinkEventHandler *m_pRouteHandler;
        NetAgentReqInfo* m_pNetAgentReqInfo;

        pthread_t mEventThread;
        pthread_mutex_t mDispatchMutex;
        pthread_cond_t mDispatchCond;
        pthread_t mSpiMonitorThread;

        int sock_fd;
        int sock6_fd;
        int mRouteSock;

        std::list<int> m_lTransIntfId; // element: tran_id * 100 + interface_id
        // Hashmap to store handover PDN information, data: <tid, NetAgentPdnInfo>
        std::map<int, NetAgentPdnInfo *> m_pdnHoInfoMap;
        std::map<int, int> m_rsvnFdMap;
        std::map<unsigned int, unsigned int> m_mapFreeMultiSpi;
        std::map<unsigned int, struct xfrm_userspi_info *> m_mapSpiXspi;

        int mSimCount;
        bool mIsUserBuild;
        bool mIsThinModemSupport;
};

#endif /* __NET_AGENT_SERVICE_H__ */
