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

#ifndef __MIPC_EVENT_HANDLER_H__
#define __MIPC_EVENT_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <mtk_log.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "NetAgentCommon.h"
#include "Utils.h"
#include "mipc_api.h"
#include "mipc_msg.h"
#include "mipc_msg_host.h"

#define IPV4_SEGMENT_LENGTH 4
#define IPV6_SEGMENT_LENGTH 16
#define MAX_MOD_NAME_LENGTH 16
#define MAX_FQDN_LENGTH 256
#define MAX_EXECV_ARG 24
#define SYSTEM_IP_WRAPPER_BIN_DIR "/system/bin/ip-wrapper-1.0"
#define MAX_PDN_REASON_LENGTH 4
#define MAC_STRING_LENGTH 17
#define UNIT_TEST (0)

typedef enum {
    NETAGENT_IO_RET_SUCCESS = 0,
    NETAGENT_IO_RET_GENERIC_FAILURE = 1,
    NETAGENT_IO_RET_AT_FAIL = 2,
} netagent_io_ret_e;

typedef enum {
    NETAGENT_IO_CMD_IFST = 1,
    NETAGENT_IO_CMD_IFUP = 2,
    NETAGENT_IO_CMD_IFDOWN = 3,
    NETAGENT_IO_CMD_SETMTU = 4,
    NETAGENT_IO_CMD_IPCHG = 5,
    NETAGENT_IO_CMD_IPUPDATE = 6,
    NETAGENT_IO_CMD_IFCHANGE = 7,
    NETAGENT_IO_CMD_PDNHO = 8,

    NETAGENT_IO_CMD_TCP_RSVN = 101,
    NETAGENT_IO_CMD_UDP_RSVN = 102,
    NETAGENT_IO_CMD_ARP_QUERY = 103,
    NETAGENT_IO_CMD_ARP_RESULT = 104,
    NETAGENT_IO_CMD_SPI_RSVN = 105,
    NETAGENT_IO_CMD_SPI_QUERY = 106,
    NETAGENT_IO_CMD_REALLOCATE_SPI = 107,

    NETAGENT_IO_CMD_MAX = 0xFFFFFFFF
} netagent_io_cmd_e;

typedef enum {
    NETAGENT_IO_IFST_DOWN = 0,
    NETAGENT_IO_IFST_UP = 1,
    NETAGENT_IO_IFST_MAX = 0xFFFFFFFF
} netagent_io_ifst_e;

typedef enum {
    NETAGENT_IO_MTU = 0,
    NETAGENT_IO_MTU_MAX = 0xFFFFFFFF
} netagent_io_mtu_e;

typedef enum {
    NETAGENT_IO_ADDR_TYPE_UNKNOWN = 0,
    NETAGENT_IO_ADDR_TYPE_IPv4 = 0x1,
    NETAGENT_IO_ADDR_TYPE_IPv6 = 0x2,
    NETAGENT_IO_ADDR_TYPE_IPv4v6 = 0x3,
    NETAGENT_IO_ADDR_TYPE_Any = 0x4,
    NETAGENT_IO_ADDR_TYPE_MAX = 0xFFFFFFFF
} netagent_io_addr_type_e;

typedef enum {
    NETAGENT_IO_SPI_ADDR_SRC = 0,
    NETAGENT_IO_SPI_ADDR_DST = 1,
} netagent_io_spi_addr_whom_e;

typedef enum {
    NETAGENT_IO_HO_RAN_UNKNOWN = 0,
    NETAGENT_IO_HO_RAN_MOBILE = 1,
    NETAGENT_IO_HO_RAN_WIFI = 2,
    NETAGENT_IO_HO_RAN_MAX = 0xFFFFFFFF
} netagent_io_ho_ran_e;

typedef enum {
    NETAGENT_IO_HO_STATE_START = 0,
    NETAGENT_IO_HO_STATE_STOP = 1,
    NETAGENT_IO_HO_STATE_MAX = 0xFFFFFFFF
} netagent_io_ho_state_e;

typedef enum {
    NETAGENT_IO_HO_RESULT_FAIL = 0,
    NETAGENT_IO_HO_RESULT_SUCCESS = 1,
    NETAGENT_IO_HO_RESULT_MAX = 0xFFFFFFFF
} netagent_io_ho_result_e;

typedef struct {
    int                             cid;
    netagent_io_ho_state_e          hostate;
    netagent_io_ho_result_e         is_succ;
    netagent_io_ho_ran_e            src_ran;
    netagent_io_ho_ran_e            tgt_ran;
    netagent_io_addr_type_e         addr_type;
} netagent_io_ho_info_t;

typedef struct {
    int                     fail_code;
    char                    ifname[64];
    char                    ip[64];
    char                    mac[64];
} netagent_io_arp_parameter_t;

typedef struct {
    uint8_t                 addr_v4[4];
    uint8_t                 addr_v6[16];
} netagent_io_addr_t;

typedef struct {
    unsigned int                 state;      /* netagent_io_ifst_e */
    netagent_io_addr_type_e      addr_type;  /* netagent_io_addr_type_e */
} netagent_io_ifst_parameter_t;

typedef struct {
    unsigned int            ipv4_mtu;
    unsigned int            ipv6_mtu;
} netagent_io_mtu_parameter_t;

typedef struct {
    int                           ipv6_prefix_length;
    netagent_io_mtu_parameter_t   mtu;
    netagent_io_addr_type_e       addr_type;
    netagent_io_addr_t            addr;
} netagent_io_ip_parameter_t;

typedef struct {
    netagent_io_ho_info_t   info;
    netagent_io_addr_t      addr;
} netagent_io_ho_parameter_t;

typedef struct {
    int                       interfaceId;
    unsigned int              action;
    netagent_io_addr_type_e   addr_type;
    netagent_io_addr_t        addr;
    int                       port;
    unsigned int              response;
} netagent_io_tcp_parameter_t;

typedef struct {
    unsigned int             action;
    netagent_io_addr_type_e  addr_type;
    netagent_io_addr_t       addrSrc;
    netagent_io_addr_t       addrDst;
    unsigned int             protocol;
    unsigned int             mode;
    unsigned int             min;
    unsigned int             max;
    unsigned int             response;
} netagent_io_spi_parameter_t;

typedef struct {
    // this is transaction_id and interface_id combination,
    // i.e, trans_intf_id = (transaction_id * 100) + interface_id
    unsigned int         trans_intf_id;
    netagent_io_cmd_e    cmd;
    char*                reason;
    mipc_msg_t*          header;
    union {
        netagent_io_ifst_parameter_t        ifst;   // cmd = NETAGENT_IO_CMD_IFST
        netagent_io_mtu_parameter_t         mtu;    // cmd = NETAGENT_IO_CMD_SETMTU
        netagent_io_ip_parameter_t          ip;     // cmd = NETAGENT_IO_CMD_IPUPDATE / NETAGENT_IO_CMD_IFUP / NETAGENT_IO_CMD_IFCHG
        netagent_io_ho_parameter_t          ho;     // cmd = NETAGENT_IO_CMD_PDNHO
        netagent_io_arp_parameter_t         arp;    // cmd = NETAGENT_IO_CMD_ARP_QUERY
        netagent_io_tcp_parameter_t         tcp;    // cmd = NETAGENT_IO_CMD_TCP_RSVN
        netagent_io_spi_parameter_t         spi;    // cmd = NETAGENT_IO_CMD_SPI_RSVN
    } parameter;
} netagent_io_cmd_obj_t;


class MipcEventHandler {
    public:
        MipcEventHandler();
        virtual ~MipcEventHandler();

        void registerToHandleMipcUrc();
        static void mipcUrcCallback(mipc_msg_t* msg_ptr, void* priv_ptr);
        static void mipcCmdCallback(mipc_msg_t* msg_ptr, void* priv_ptr);
        static void enqueueRequest(netagent_io_cmd_obj_t* cmdObj);
        static int confirmToMipc(void* o);

        static void handleIpportRequest(mipc_msg_t* msg_ptr);
        static void handleIpspiRequest(mipc_msg_t* msg_ptr);
        static void handleMultiIpspiRequest(mipc_msg_t* msg_ptr);
        static void handleArpRequest(mipc_msg_t* msg_ptr);
        static int handleIpportResponse(netagent_io_cmd_obj_t* cmdObj);
        static int handleIpspiResponse(netagent_io_cmd_obj_t* cmdObj);
        static int handleArpResponse(netagent_io_cmd_obj_t* cmdObj);
        static void printMipcMsgId(mipc_msg_t* msg_ptr, const char* caller);
        static int handleIpspiQuery();
        static void* allocIpportRsvn(unsigned int transId, netagent_io_cmd_e cmd,
                unsigned int action, unsigned int response, mipc_msg_t* header);
        static void* allocIpspiRsvn(unsigned int transId, unsigned int action,
                unsigned int response, unsigned int count, const char* reason,
                mipc_msg_t* header);
        static void* allocIpspiQuery();

        static netagent_io_cmd_obj_t* allocCmdObj(unsigned int ifId, netagent_io_cmd_e cmd);
        static int copyIpAddressToCmdObj(netagent_io_cmd_obj_t* cmdObj,
                mipc_v4_full_addr_struct4* ipv4List,
                mipc_v6_full_addr_struct4* ipv6List);
        static int copyHOIpAddressToCmdObj(netagent_io_cmd_obj_t* cmdObj,
                mipc_data_v4_addr_struct4* ipv4List,
                mipc_data_v6_addr_struct4* ipv6List);
        static netagent_io_addr_type_e getSourceAddrType(unsigned int ipv4ount,
                unsigned int ipv6Count);

        static int getAddrV4(void* o, unsigned int* addr);
        static int getAddrV6(void* o, unsigned int* addr);
        static int getInterfaceId(void* o, unsigned int* ifId);
        static int getAddrType(void* o, netagent_io_addr_type_e* addrType);
        static int getReason(void* o, char **reason);
        static int getModemPdnMtu(void *o, unsigned int *mtu);
        static int getMtu(void *o, unsigned int protocol, unsigned int *mtu);
        static int getCommandType(void *o, netagent_io_cmd_e *cmd);
        static int freeCommand(void *o);
        static int getHeader(void *o, mipc_msg_t** header);
        static int getRsvnAction(void *o, unsigned int *action);
        static int getRsvnIfId(void *o, int *interfaceId);
        static int getRsvnPort(void *o, int *port);
        static void parseIpspiAddr(netagent_io_cmd_obj_t *cmdObj,
                mipc_addr_struct4 *srcIpAddr, mipc_addr_struct4 *destIpAddr);
        static int getIpspiProtocol(void *o, unsigned int *protocol);
        static int getIpspiMode(void *o, unsigned int *mode);
        static int getIpspiMinMax(void *o, unsigned int *min, unsigned int *max);
        static int getIpspiAddrV4(void *o, netagent_io_spi_addr_whom_e whom,
                unsigned int *addr);
        static int getIpspiAddrV6(void *o, netagent_io_spi_addr_whom_e whom,
                unsigned int *addr);
        static int getHOInfo(void *o, netagent_io_ho_info_t *hoInfo);
        static int flushIpsecPolicy(const char *srcAddr,
                netagent_io_addr_type_e addrType, int isUserLoad);
        static int execFmt(int *status, const char *fmt, ...);
        static int forkExec(const char *cmd, int *status);
        static int systemExt(char *argv[]);

        static void setHandoverInfo(HandoverInfo hoInfo);

        static netagent_io_cmd_e mipcCmd2InternalCmd(unsigned int cmd);
        static unsigned int mipcSlotId2InternalSlotId(unsigned int slotId);
        static mipc_msg_sim_ps_id_enum internalSlotId2MipcSlotId(unsigned int slotId);
};
#endif /* __MIPC_EVENT_HANDLER_H__ */
