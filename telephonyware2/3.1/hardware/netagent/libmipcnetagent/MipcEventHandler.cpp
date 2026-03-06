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
#include <mtk_log.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "MipcEventHandler.h"
#include "NetAgentService.h"

#define NA_LOG_TAG "MipcEventHandler"

/*****************************************************************************
 * Class MipcEventHandler
 *****************************************************************************/
MipcEventHandler::MipcEventHandler() {
}

MipcEventHandler::~MipcEventHandler() {
}

void MipcEventHandler::registerToHandleMipcUrc() {
    const int urcList[] = {
        MIPC_INTERNAL_EIF_IND,
    };

    const int cmdList[] = {
        MIPC_INTERNAL_EIPPORT_CMD,
        MIPC_INTERNAL_EIPSPI_CMD,
        MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD,
        MIPC_WFC_GET_MAC_CMD,
    };

    int simCount = getSimCount();
    int urcLength = sizeof(urcList) / sizeof(int);
    int cmdLength = sizeof(cmdList) / sizeof(int);

    for (unsigned int slotId = 0; slotId < simCount; slotId++) {
        for (unsigned int i = 0; i < urcLength; i++) {
            mipc_msg_register_ind(internalSlotId2MipcSlotId(slotId),
                    (mipc_msg_id_enum)urcList[i], (void*)mipcUrcCallback, NULL);
        }
    }

    for (unsigned int i = 0; i < cmdLength; i++) {
        mipc_msg_register_cmd((mipc_msg_id_enum)cmdList[i], (void*)mipcCmdCallback, NULL);
    }
}

void MipcEventHandler::mipcUrcCallback(mipc_msg_t* msg_ptr, void* priv_ptr) {
    UNUSED(priv_ptr);
    if (msg_ptr == NULL) {
        NA_LOG_E("[%s] invalid msg_ptr", __FUNCTION__);
        return;
    }

    netagent_io_cmd_obj_t* cmdObj = NULL;
    netagent_io_cmd_e cmd;
    netagent_io_addr_type_e addrType = NETAGENT_IO_ADDR_TYPE_UNKNOWN;
    unsigned int transId;
    unsigned int mipcCmd;
    unsigned int ipv4Count;
    unsigned int ipv6Count;
    unsigned int mtu;
    unsigned int reason;
    char* reasonStr = NULL;
    mipc_v4_full_addr_struct4* ipv4AddrList;
    mipc_v6_full_addr_struct4* ipv6AddrList;
    int nSnprintf = -1;

    transId = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIF_IND_T_TRANSID, 0);
    mipcCmd = mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIF_IND_T_CMD, 0xff);
    cmd = mipcCmd2InternalCmd(mipcCmd);
    NA_LOG_D("[%s] transId: %d cmd: %d", __FUNCTION__, transId, cmd);
    cmdObj = allocCmdObj(transId, cmd);
    if (cmdObj == NULL) {
        NA_LOG_E("[%s] invalid cmd object", __FUNCTION__);
        return;
    }

    switch (cmd) {
        case NETAGENT_IO_CMD_IFUP:
        case NETAGENT_IO_CMD_IPCHG:
        case NETAGENT_IO_CMD_IFCHANGE:
            ipv4Count = mipc_msg_get_val_uint8(msg_ptr,
                    MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_COUNT, 0);
            ipv4AddrList = (mipc_v4_full_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr,
                    MIPC_INTERNAL_EIF_IND_T_NET_V4_ADDR_LIST, NULL);
            ipv6Count = mipc_msg_get_val_uint8(msg_ptr,
                    MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_COUNT, 0);
            ipv6AddrList = (mipc_v6_full_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr,
                    MIPC_INTERNAL_EIF_IND_T_NET_V6_ADDR_LIST, NULL);
            reason = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIF_IND_T_CAUSE, 0);

            NA_LOG_D("[%s] ifup/ipchg/ifchange transId=%d ipv4Count=%d ipv6Count=%d reason=%d",
                    __FUNCTION__, transId, ipv4Count, ipv6Count, reason);

            addrType = getSourceAddrType(ipv4Count, ipv6Count);
            if (addrType == NETAGENT_IO_ADDR_TYPE_UNKNOWN) {
                NA_LOG_E("[%s] invalid address type", __FUNCTION__);
                goto error;
            } else {
                cmdObj->parameter.ip.addr_type = addrType;
            }

            if (ipv4Count >= 1 && ipv4AddrList != NULL) {
                cmdObj->parameter.ip.mtu.ipv4_mtu = ipv4AddrList[0].mtu;
            }

            if (ipv6Count >= 1 && ipv6AddrList != NULL) {
                // Only one mtuV6
                cmdObj->parameter.ip.mtu.ipv6_mtu = ipv6AddrList[0].mtu;
            }

            if (copyIpAddressToCmdObj(cmdObj, ipv4AddrList, ipv6AddrList) < 0) goto error;
            reasonStr = (char*)calloc(1, MAX_PDN_REASON_LENGTH);
            if (reasonStr == NULL) goto error;
            nSnprintf = snprintf(reasonStr, MAX_PDN_REASON_LENGTH, "%d", reason);
            if (nSnprintf < 0) goto error;
            cmdObj->reason = reasonStr;
            break;
        case NETAGENT_IO_CMD_IFDOWN:
            reason = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIF_IND_T_CAUSE, 0);
            NA_LOG_D("[%s] ifdown transId=%d reason=%d", __FUNCTION__, transId, reason);
            reasonStr = (char*)calloc(1, MAX_PDN_REASON_LENGTH);
            if (reasonStr == NULL) goto error;
            nSnprintf = snprintf(reasonStr, MAX_PDN_REASON_LENGTH, "%d", reason);
            if (nSnprintf < 0) goto error;
            cmdObj->reason = reasonStr;
            break;
        case NETAGENT_IO_CMD_SETMTU:
            mtu = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIF_IND_T_MTU, 0);
            cmdObj->parameter.mtu.ipv4_mtu = mtu;
            break;
        default:
            NA_LOG_E("[%s] invalid cmd=%d transId=%d", __FUNCTION__, cmd, transId);
            goto error;
    }

    enqueueRequest(cmdObj);
    return;

error:
    FREEIF(reasonStr);
    FREEIF(cmdObj);
}

void MipcEventHandler::mipcCmdCallback(mipc_msg_t* msg_ptr, void* priv_ptr) {
    UNUSED(priv_ptr);
    if (msg_ptr == NULL) {
        NA_LOG_E("[%s] invalid msg_ptr", __FUNCTION__);
        return;
    }

    int cmd = msg_ptr->hdr.msg_id;
    NA_LOG_I("[%s] cmd=%d", __FUNCTION__, cmd);
    switch(cmd) {
        case MIPC_INTERNAL_EIPPORT_CMD:
            handleIpportRequest(msg_ptr);
            break;
        case MIPC_INTERNAL_EIPSPI_CMD:
            handleIpspiRequest(msg_ptr);
            break;
        case MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD:
            handleMultiIpspiRequest(msg_ptr);
            break;
        case MIPC_WFC_GET_MAC_CMD:
            handleArpRequest(msg_ptr);
            break;
        default:
            NA_LOG_E("[%s] unknown cmd=%d", __FUNCTION__, cmd);
            break;
    }
}

void MipcEventHandler::enqueueRequest(netagent_io_cmd_obj_t* cmdObj) {
    if (NetAgentService::getInstance() != NULL) {
        NetAgentService::getInstance()->enqueueReqInfo(cmdObj, REQUEST_TYPE_DDM);
    }
}

int MipcEventHandler::confirmToMipc(void* o) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (cmdObj == NULL) {
        NA_LOG_E("[%s] invalid cmd obj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    switch (cmdObj->cmd) {
        case NETAGENT_IO_CMD_ARP_RESULT:
            return handleArpResponse(cmdObj);
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            return handleIpportResponse(cmdObj);
        case NETAGENT_IO_CMD_SPI_RSVN:
            return handleIpspiResponse(cmdObj);
        case NETAGENT_IO_CMD_SPI_QUERY:
            return handleIpspiQuery();
        default:
            NA_LOG_E("[%s] invalid cmd=%d", __FUNCTION__, cmdObj->cmd);
            break;
    }

    return NETAGENT_IO_RET_GENERIC_FAILURE;
}

void MipcEventHandler::handleIpportRequest(mipc_msg_t* msg_ptr) {
    unsigned int transId;
    unsigned int ifId;
    unsigned int action;
    int proto;
    int port;
    mipc_addr_struct4 *ipAddr;
    netagent_io_cmd_obj_t *cmdObj = NULL;

    /*<transaction id>, <0: alloc, 1: free>, <interface ID, range -1-99>,
              <local IP address>, <IANA protocol numbers, 6: tcp, 17: udp>,
              <port to be allocated or freed>

    <interface ID>:
        0-98: PDN connection identifier
        99  : WiFi interface
        -1  : No interface (reservation isn't interface specific)

    e.x. +EIPPORT: 123, 0, 1, "192.168.1.1", 6, 2000*/

    transId = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_TRANSID, 0);
    proto = mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_PROTO, -1);

    cmdObj = allocCmdObj(transId,
            proto == 6 ? NETAGENT_IO_CMD_TCP_RSVN : NETAGENT_IO_CMD_UDP_RSVN);
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmd object", __FUNCTION__);
        return;
    }
    cmdObj->header = mipc_msg_copy_hdr(msg_ptr);

    port = mipc_msg_get_val_uint16(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_PORT, -1);
    action = mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_ACTION, 0);
    ifId = mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPPORT_CMD_T_IFID, 0);
    if ((ipAddr = (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr,
            MIPC_INTERNAL_EIPPORT_CMD_T_ADDR, NULL)) == NULL) {
        NA_LOG_E("[%s] get addr fail", __FUNCTION__);
        FREEIF(cmdObj);
        return;
    }

    NA_LOG_D("[%s] transId: %d proto: %d port: %d action: %d ifId: %d",
            __FUNCTION__, transId, proto, port, action, ifId);

    cmdObj->parameter.tcp.port = port;
    cmdObj->parameter.tcp.interfaceId = ifId;
    cmdObj->parameter.tcp.action = action;
    if (ipAddr->addr_len == IPV4_SEGMENT_LENGTH) {
        cmdObj->parameter.tcp.addr_type = NETAGENT_IO_ADDR_TYPE_IPv4;
        for (int i = 0; i < IPV4_SEGMENT_LENGTH; i++) {
            cmdObj->parameter.tcp.addr.addr_v4[i] = ipAddr->addr[i];
        }
    } else if (ipAddr->addr_len == IPV6_SEGMENT_LENGTH) {
        cmdObj->parameter.tcp.addr_type = NETAGENT_IO_ADDR_TYPE_IPv6;
        for (int i = 0; i < IPV6_SEGMENT_LENGTH; i++) {
            cmdObj->parameter.tcp.addr.addr_v6[i] = ipAddr->addr[i];
        }
    } else {
        NA_LOG_E("[%s] unknown address length", __FUNCTION__);
        FREEIF(cmdObj);
        return;
    }

    enqueueRequest(cmdObj);
}

void MipcEventHandler::handleIpspiRequest(mipc_msg_t* msg_ptr) {
    unsigned int transId;
    unsigned int action;
    int proto;
    int mode;
    int minSpi;
    int maxSpi;
    int spi;
    mipc_addr_struct4 *srcIpAddr;
    mipc_addr_struct4 *destIpAddr;
    netagent_io_cmd_obj_t *cmdObj = NULL;

    /* URC which MD uses to request an SPI (similar as "ip xfrm state allocspi"):
       <transaction id>, <0: alloc>, <source IP address>, <destination IP address>,
       <"esp" | "ah" | "comp" | "route2" | "hao">, <mode>, <min spi>, <max spi>
       e.x. EIPSPI: 456, 0, "192.168.1.1", "192.168.1.2", "esp", "transport", 0, 4294967295
       URC which MD uses to free an SPI (similar as "ip xfrm state delete"):
       +EIPSPI: <transaction id>, <1: free>, <source IP address>, <destination IP address>,
       <"esp" | "ah" | "comp" | "route2" | "hao">, <spi to be freed>
       e.x. EIPSPI: 789, 1, "192.168.1.1", "192.168.1.2", "esp", 11223344
    */

    transId = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_TRANSID, 0);
    action = mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_ACTION, 0);
    proto = mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_PROTO, -1);
    mode = mipc_msg_get_val_uint8(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_MODE, -1);
    minSpi = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_MIN_SPI, -1);
    maxSpi = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_MAX_SPI, -1);
    spi = mipc_msg_get_val_uint32(msg_ptr, MIPC_INTERNAL_EIPSPI_CMD_T_SPI, -1);

    NA_LOG_D("[%s] transId=%d action=%d proto=%d mode=%d minSpi=%d maxSpi=%d spi=%d",
            __FUNCTION__, transId, action, proto, mode, minSpi, maxSpi, spi);

    cmdObj = allocCmdObj(transId, NETAGENT_IO_CMD_SPI_RSVN);
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmd object", __FUNCTION__);
        return;
    }
    cmdObj->header = mipc_msg_copy_hdr(msg_ptr);

    if (proto == MIPC_IPSEC_PROTO_ENUM_ESP) {
        cmdObj->parameter.spi.protocol = IPPROTO_ESP;
    } else if (proto == MIPC_IPSEC_PROTO_ENUM_AH) {
        cmdObj->parameter.spi.protocol = IPPROTO_AH;
    } else {
        NA_LOG_E("[%s] unknown protocol %d", __FUNCTION__, proto);
    }

    cmdObj->parameter.spi.action = action;
    if (action == 0) {
        if (mode == MIPC_IPSEC_MODE_ENUM_transport) {
            cmdObj->parameter.spi.mode = XFRM_MODE_TRANSPORT;
        } else if (mode == MIPC_IPSEC_MODE_ENUM_tunnel) {
            cmdObj->parameter.spi.mode = XFRM_MODE_TUNNEL;
        } else {
            NA_LOG_E("[%s] unknown mode %d", __FUNCTION__, mode);
        }
        cmdObj->parameter.spi.min = minSpi;
        cmdObj->parameter.spi.max = maxSpi;
    } else if (action == 1) {
        // using 'mode' to remember the spi to be freed
        cmdObj->parameter.spi.mode = spi;
    } else {
        NA_LOG_E("[%s] invalid action=%d", __FUNCTION__, action);
        FREEIF(cmdObj);
        return;
    }

    if ((srcIpAddr = (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr,
            MIPC_INTERNAL_EIPSPI_CMD_T_SRC_ADDR, NULL)) == NULL) {
        NA_LOG_E("[%s] get srcAddr fail", __FUNCTION__);
        FREEIF(cmdObj);
        return;
    }

    if ((destIpAddr = (mipc_addr_struct4*)mipc_msg_get_val_ptr(msg_ptr,
            MIPC_INTERNAL_EIPSPI_CMD_T_DST_ADDR, NULL)) == NULL) {
        NA_LOG_E("[%s] get destAddr fail", __FUNCTION__);
        FREEIF(cmdObj);
        return;
    }

    parseIpspiAddr(cmdObj, srcIpAddr, destIpAddr);
    enqueueRequest(cmdObj);
}

void MipcEventHandler::handleMultiIpspiRequest(mipc_msg_t* msg_ptr) {
    unsigned int transId;
    unsigned int count;
    uint8_t* protoList = NULL;
    uint32_t* spiList = NULL;
    netagent_io_cmd_obj_t *cmdObj = NULL;

    /* URC which MD uses to free multi SPI:
       <transaction id>, <2: free>, <source IP address>, <destination IP address>,
       <"esp" | "ah" | "comp" | "route2" | "hao">, <spi to be freed>, <index>, <count>
    */

    transId = mipc_msg_get_val_uint32(msg_ptr,
            MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_TRANSID, 0);
    count = mipc_msg_get_val_uint32(msg_ptr,
            MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_MULTI_FREE_COUNT, 0);
    if (count <= 0) {
        NA_LOG_E("[%s] count=0", __FUNCTION__);
        return;
    }
    protoList = (uint8_t*)mipc_msg_get_val_ptr(msg_ptr,
            MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_PROTO_LIST, NULL);
    if (protoList == NULL) {
        NA_LOG_E("[%s] protoList==NULL", __FUNCTION__);
        return;
    }
    spiList = (uint32_t*)mipc_msg_get_val_ptr(msg_ptr,
            MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SPI_LIST, NULL);
    if (spiList == NULL) {
        NA_LOG_E("[%s] spiList==NULL", __FUNCTION__);
        return;
    }

    for (int i = 0; i < count; i++) {
        cmdObj = allocCmdObj(transId, NETAGENT_IO_CMD_SPI_RSVN);
        if (!cmdObj) {
            NA_LOG_E("[%s] invalid cmd object or addr", __FUNCTION__);
            return;
        }

        cmdObj->header = mipc_msg_copy_hdr(msg_ptr);
        cmdObj->parameter.spi.action = SPI_MULTI_FREE;
        cmdObj->parameter.spi.mode = spiList[i];
        cmdObj->parameter.spi.min = (i + 1);
        cmdObj->parameter.spi.max = count;
        if (protoList[i] == MIPC_IPSEC_PROTO_ENUM_ESP) {
            cmdObj->parameter.spi.protocol = IPPROTO_ESP;
        } else if (protoList[i] == MIPC_IPSEC_PROTO_ENUM_AH) {
            cmdObj->parameter.spi.protocol = IPPROTO_AH;
        } else {
            NA_LOG_E("[%s] unknown protocol %d", __FUNCTION__, protoList[i]);
        }

        mipc_addr_struct4* srcIpAddr = (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr,
                MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_SRC_ADDR_LIST, i, NULL);
        mipc_addr_struct4* destIpAddr = (mipc_addr_struct4*)mipc_msg_get_idx_ptr(msg_ptr,
                MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD_T_DST_ADDR_LIST, i, NULL);
        if (srcIpAddr == NULL || destIpAddr == NULL) {
           NA_LOG_E("[%s] src or dest address is NULL", __FUNCTION__);
           FREEIF(cmdObj);
           return;
        }
        parseIpspiAddr(cmdObj, srcIpAddr, destIpAddr);

        NA_LOG_D("[%s] transId=%d proto=%d spi=%d count=%d index=%d",
                __FUNCTION__, transId, protoList[i], spiList[i], count, i);

        enqueueRequest(cmdObj);
    }
}

void MipcEventHandler::printMipcMsgId(mipc_msg_t* msg_ptr, const char* caller) {
    if (caller == NULL) {
        NA_LOG_E("[%s] invalid caller", __FUNCTION__);
        return;
    }
    if (msg_ptr == NULL) {
        NA_LOG_E("[%s] invalid msg", caller);
        return;
    }
    if (&msg_ptr->hdr == NULL) {
        NA_LOG_E("[%s] invalid header", caller);
        return;
    }
    NA_LOG_D("[%s][MIPC]MsgId: %d, simId: %d",
            caller, msg_ptr->hdr.msg_id, msg_ptr->hdr.msg_sim_ps_id);
}

void MipcEventHandler::handleArpRequest(mipc_msg_t* msg_ptr) {
    uint16_t valLenPtr = 0;
    char* ifname = mipc_msg_get_val_str(msg_ptr, MIPC_WFC_GET_MAC_CMD_T_IFNAME, &valLenPtr);
    if (ifname == NULL) {
        NA_LOG_E("handleArpRequest, invalid ifname");
        return;
    }
    if (valLenPtr > 64) {
        NA_LOG_E("handleArpRequest, invalid ifname len %d", valLenPtr);
        return;
    }

    valLenPtr = 0;
    void* rawData = mipc_msg_get_val_ptr(msg_ptr, MIPC_WFC_GET_MAC_CMD_T_IP, &valLenPtr);

    NA_LOG_I("handleArpRequest, len %d", valLenPtr);
    if (valLenPtr != MIPC_FIX_WFC_IPV4_LEN && valLenPtr != MIPC_FIX_WFC_IPV6_LEN) {
        //no expect len
        return;
    }

    uint8_t ip[MIPC_FIX_WFC_IPV6_LEN] = {0};
    memcpy(ip, rawData, valLenPtr);

    int family = AF_INET;
    if (valLenPtr == MIPC_FIX_WFC_IPV6_LEN) {
        family = AF_INET6;
    }

    int addrlen = (family == AF_INET) ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;
    char listen_ip[addrlen];
    memset(listen_ip, 0, addrlen);
    if (inet_ntop(family, ip, listen_ip, sizeof(listen_ip)) == NULL) {
        NA_LOG_E("handleArpRequest, inet_ntop ERROR");
        return;
    }

    if (UNIT_TEST) {
        NA_LOG_I("handleArpRequest, name %s, ip %s", ifname, listen_ip);
    } else {
        NA_LOG_I("handleArpRequest, name %s", ifname);
    }

    netagent_io_cmd_obj_t* cmdObj = NULL;
    cmdObj = (netagent_io_cmd_obj_t *)calloc(1, sizeof(netagent_io_cmd_obj_t));
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmd object", __FUNCTION__);
        return;
    }
    cmdObj->cmd = NETAGENT_IO_CMD_ARP_QUERY;
    //copy mipc header and deinit at freeNetAgentCmdObj of handleEvent.
    cmdObj->header = mipc_msg_copy_hdr(msg_ptr);
    strncpy(cmdObj->parameter.arp.ifname, ifname, strlen(ifname));
    strncpy(cmdObj->parameter.arp.ip, listen_ip, strlen(listen_ip));

    printMipcMsgId(cmdObj->header, "handleArpRequest");
    enqueueRequest(cmdObj);
}

int MipcEventHandler::handleIpportResponse(netagent_io_cmd_obj_t* cmdObj) {
    if (cmdObj->trans_intf_id == -1) {
        NA_LOG_E("[%s] invalid transaction id", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    mipc_msg_t* header = cmdObj->header;
    header->hdr.msg_id = MIPC_INTERNAL_EIPPORT_RSP;
    mipc_msg_add_tlv_uint32(header, MIPC_INTERNAL_EIPPORT_RSP_T_TRANSID,
            cmdObj->trans_intf_id);
    mipc_msg_add_tlv_uint8(header, MIPC_INTERNAL_EIPPORT_RSP_T_ACTION,
            cmdObj->parameter.tcp.action);
    mipc_msg_add_tlv_uint8(header, MIPC_INTERNAL_EIPPORT_RSP_T_RESULT,
            (cmdObj->parameter.tcp.response == 0 ? MIPC_INTERNAL_EIPPORT_RESULT_FAILURE :
                    MIPC_INTERNAL_EIPPORT_RESULT_SUCCESS));

    mipc_msg_rsp(header);
    NA_LOG_D("[%s] finish", __FUNCTION__);
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::handleArpResponse(netagent_io_cmd_obj_t* cmdObj) {
    if (cmdObj == NULL) {
        NA_LOG_E("[%s] invalid object", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }
    if (cmdObj->header == NULL) {
        NA_LOG_E("[%s] invalid header", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }
    if (&(cmdObj->parameter.arp) == NULL) {
        NA_LOG_E("[%s] invalid parameter", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    mipc_msg_t* header = cmdObj->header;
    printMipcMsgId(header, "handleArpResponse");
    header->hdr.msg_id = MIPC_WFC_GET_MAC_RSP;
    mipc_msg_add_tlv_int32(header, MIPC_WFC_GET_MAC_RSP_T_GET_RESULT,
            cmdObj->parameter.arp.fail_code);
    char* ifname = cmdObj->parameter.arp.ifname;
    char* ip = cmdObj->parameter.arp.ip;
    char* mac = cmdObj->parameter.arp.mac;
    if (UNIT_TEST) {
        NA_LOG_I("handleArpResponse, name %s, ip %s, mac %s", ifname, ip, mac);
    } else {
        NA_LOG_I("handleArpResponse, name %s", ifname);
    }

    if (strlen(mac) != MAC_STRING_LENGTH) {
        NA_LOG_E("handleArpResponse, mac len %d error", strlen(mac));
    } else {
        uint8_t macArray[MIPC_FIX_WFC_MAC_LEN] = {0};
        //for (uint16_t i = 0; i < MIPC_FIX_WFC_MAC_LEN; i++) {
        //    int result = sscanf(mac + 3*i, "%2hhx", &macArray[i]);
        //}
        int result = sscanf(mac, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
                &macArray[0], &macArray[1], &macArray[2],
                &macArray[3], &macArray[4], &macArray[5]);
        if (result < 0) {
            NA_LOG_E("handleArpResponse, sscanf failed");
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        }
        if (UNIT_TEST) {
            NA_LOG_D("MacByteArray=%02x:%02x:%02x:%02x:%02x:%02x",
                macArray[0], macArray[1], macArray[2], macArray[3],
                macArray[4], macArray[5]);
        }
        mipc_msg_add_tlv(header, MIPC_WFC_GET_MAC_RSP_T_MAC, MIPC_FIX_WFC_MAC_LEN, macArray);
    }

    if(!strlen(ip)) {
        NA_LOG_E("handleArpResponse, no ip string");
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    int family = AF_INET6;
    int ret = 1;
    if(strchr(ip, ':') == NULL) {
        //ipv4 case
        family = AF_INET;
    }
    int addrlen = (family == AF_INET) ? MIPC_FIX_WFC_IPV4_LEN : MIPC_FIX_WFC_IPV6_LEN;
    uint8_t ipArray[addrlen];
    memset(ipArray, 0, addrlen);

    if (family == AF_INET) {
        struct in_addr v4Address;
        memset(&v4Address, 0, sizeof(v4Address));
        // ret: -1, error occurs, ret: 0, invalid address, ret: 1, success;
        ret = inet_pton(AF_INET, ip, &v4Address);
        if (ret >= 0) {
            memcpy(ipArray, &v4Address, addrlen);
            if (UNIT_TEST) {
                NA_LOG_D("Ipv4ByteArray:%d.%d.%d.%d",
                        ipArray[0], ipArray[1], ipArray[2], ipArray[3]);
            }
        } else {
            NA_LOG_E("handleArpResponse, ipv4 error: %d", ret);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        }
    } else {
        //ipv6 case
        struct in6_addr v6Address;
        memset(&v6Address, 0, sizeof(v6Address));
        // ret: -1, error occurs, ret: 0, invalid address, ret: 1, success;
        ret = inet_pton(AF_INET6, ip, &v6Address);
        if (ret >= 0) {
            memcpy(ipArray, &v6Address, addrlen);
            if (UNIT_TEST) {
                NA_LOG_D("Ipv6ByteArray=%02X%02X:%02X%02X:%02X%02X:%02X%02X:"
                        "%02X%02X:%02X%02X:%02X%02X:%02X%02X",
                        ipArray[0], ipArray[1], ipArray[2], ipArray[3],
                        ipArray[4], ipArray[5], ipArray[6], ipArray[7],
                        ipArray[8], ipArray[9], ipArray[10], ipArray[11],
                        ipArray[12], ipArray[13], ipArray[14], ipArray[15]);
            }
        } else {
            NA_LOG_E("handleArpResponse, ipv6 error: %d", ret);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        }
    }
    mipc_msg_add_tlv(header, MIPC_WFC_GET_MAC_RSP_T_IP, addrlen, ipArray);
    mipc_msg_add_tlv(header, MIPC_WFC_GET_MAC_RSP_T_IFNAME, strlen(ifname) + 1, ifname);

    mipc_msg_rsp(header);
    //deinit at freeNetAgentCmdObj of handleEvent after sendArpResult.
    NA_LOG_D("[%s] finish", __FUNCTION__);
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::handleIpspiResponse(netagent_io_cmd_obj_t* cmdObj) {
    if (cmdObj->trans_intf_id == -1) {
        NA_LOG_E("[%s] invalid transaction id", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    int* spiValue = NULL;
    int* spiState = NULL;
    mipc_msg_t* header = cmdObj->header;
    if (cmdObj->parameter.spi.action == SPI_MULTI_FREE) {
        header->hdr.msg_id = MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP;
        char* spi = cmdObj->reason;
        int count = cmdObj->parameter.spi.max;
        spiValue = (int*)calloc(count, sizeof(int));
        if (spiValue == NULL) {
            NA_LOG_E("[%s] alloc spiValue fail", __FUNCTION__);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        }
        spiState = (int*)calloc(count, sizeof(int));
        if (spiState == NULL) {
            NA_LOG_E("[%s] alloc spiState fail", __FUNCTION__);
            FREEIF(spiValue);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        }

        int i = 0, index = 0;
        std::string responseValue("");
        char *saveptr = NULL;
        char *state = strtok_r(spi, ",", &saveptr);
        while (state != NULL && i < count * 2) {
            index = i / 2;
            if (index >= count) break;
            if (i % 2 == 0) {
                spiValue[index] = atoi(state);
                responseValue.append(std::to_string(spiValue[index]));
                responseValue.append(" ");
            } else {
                spiState[index] = atoi(state);
                responseValue.append(std::to_string(spiState[index]));
                responseValue.append(" ");
            }
            state = strtok_r(NULL, ",", &saveptr);
            i++;
        }
        mipc_msg_add_tlv_uint32(header, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_TRANSID,
                cmdObj->trans_intf_id);
        mipc_msg_add_tlv_uint32(header, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_MULTI_FREE_COUNT,
                count);
        mipc_msg_add_tlv(header, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_SPI_LIST,
                count * sizeof(int), spiValue);
        mipc_msg_add_tlv(header, MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP_T_STATUS_LIST,
                count * sizeof(int), spiState);
        NA_LOG_D("[%s] free mulit spi count:%d number:%d response:%s",
                __FUNCTION__, count, i, responseValue.c_str());
    } else {
        header->hdr.msg_id = MIPC_INTERNAL_EIPSPI_RSP;
        mipc_msg_add_tlv_uint32(header, MIPC_INTERNAL_EIPPORT_RSP_T_TRANSID,
                cmdObj->trans_intf_id);
        mipc_msg_add_tlv_uint8(header, MIPC_INTERNAL_EIPPORT_RSP_T_ACTION,
                cmdObj->parameter.spi.action);
        mipc_msg_add_tlv_uint32(header, MIPC_INTERNAL_EIPSPI_RSP_T_SPI,
                cmdObj->parameter.spi.response);
        NA_LOG_D("[%s] spi:%d", __FUNCTION__, cmdObj->parameter.spi.response);
    }

    mipc_msg_rsp(header);
    FREEIF(spiValue);
    FREEIF(spiState);
    NA_LOG_D("[%s] finish", __FUNCTION__);
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::handleIpspiQuery() {
    mipc_msg_t* request = mipc_msg_init(MIPC_SYS_AT_REQ, internalSlotId2MipcSlotId(0));
    mipc_msg_add_tlv(request, MIPC_SYS_AT_REQ_T_ATCMD, 11, (char*)"AT+EIPSPI?");
    mipc_msg_t* response = mipc_msg_sync(request);
    mipc_msg_deinit(request);
    mipc_msg_deinit(response);
    NA_LOG_D("[%s] finish", __FUNCTION__);
    return NETAGENT_IO_RET_SUCCESS;
}

void* MipcEventHandler::allocIpportRsvn(unsigned int transId, netagent_io_cmd_e cmd,
        unsigned int action, unsigned int response, mipc_msg_t* header) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t *)calloc(1,
            sizeof(netagent_io_cmd_obj_t));
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NULL;
    }

    cmdObj->trans_intf_id = transId;
    cmdObj->header = mipc_msg_copy_hdr(header);
    cmdObj->cmd = cmd;
    cmdObj->parameter.tcp.action = action;
    cmdObj->parameter.tcp.response= response;
    return cmdObj;
}

void* MipcEventHandler::allocIpspiRsvn(unsigned int transId, unsigned int action,
        unsigned int response, unsigned int count, const char* reason, mipc_msg_t* header) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t *)calloc(1,
            sizeof(netagent_io_cmd_obj_t));
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NULL;
    }

    cmdObj->trans_intf_id = transId;
    cmdObj->header = mipc_msg_copy_hdr(header);
    cmdObj->parameter.spi.action = action;
    cmdObj->cmd = NETAGENT_IO_CMD_SPI_RSVN;
    if (action == SPI_MULTI_FREE) {
        cmdObj->reason = strdup(reason);
        cmdObj->parameter.spi.max = count;
    } else {
        cmdObj->parameter.spi.response= response;
    }
    return cmdObj;
}

void * MipcEventHandler::allocIpspiQuery() {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t *)calloc(1,
            sizeof(netagent_io_cmd_obj_t));
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NULL;
    }

    cmdObj->cmd = NETAGENT_IO_CMD_SPI_QUERY;
    return cmdObj;
}

netagent_io_cmd_obj_t* MipcEventHandler::allocCmdObj(unsigned int if_id,
        netagent_io_cmd_e cmd) {
    netagent_io_cmd_obj_t* cmdObj = 0;
    cmdObj = (netagent_io_cmd_obj_t*)calloc(1, sizeof(netagent_io_cmd_obj_t));
    if (cmdObj == NULL) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return 0;
    }
    cmdObj->trans_intf_id = if_id;
    cmdObj->cmd = cmd;
    return cmdObj;
}

int MipcEventHandler::copyIpAddressToCmdObj(netagent_io_cmd_obj_t* cmdObj,
        mipc_v4_full_addr_struct4* ipv4List, mipc_v6_full_addr_struct4* ipv6List) {
    if (!cmdObj) return -1;

    switch (cmdObj->parameter.ip.addr_type) {
        case NETAGENT_IO_ADDR_TYPE_IPv4:
            if (ipv4List == NULL) return -1;
            for (int i = 0; i < IPV4_SEGMENT_LENGTH; i++) {
                cmdObj->parameter.ip.addr.addr_v4[i] = ipv4List[0].addr[i];
            }
            return 1;
        case NETAGENT_IO_ADDR_TYPE_IPv6:
            if (ipv6List == NULL) return -1;
            for (int i = 0; i < IPV6_SEGMENT_LENGTH; i++) {
                cmdObj->parameter.ip.addr.addr_v6[i] = ipv6List[0].addr[i];
            }
            return 1;
        case NETAGENT_IO_ADDR_TYPE_IPv4v6:
            if (ipv6List == NULL || ipv4List == NULL) return -1;
            for (int i = 0; i < IPV4_SEGMENT_LENGTH; i++) {
                cmdObj->parameter.ip.addr.addr_v4[i] = ipv4List[0].addr[i];
            }
            for (int j = 0; j < IPV6_SEGMENT_LENGTH; j++) {
                cmdObj->parameter.ip.addr.addr_v6[j] = ipv6List[0].addr[j];
            }
            return 1;
        default:
            NA_LOG_E("[%s] invalid address type", __FUNCTION__);
            return -1;
    }
}

int MipcEventHandler::copyHOIpAddressToCmdObj(netagent_io_cmd_obj_t* cmdObj,
                mipc_data_v4_addr_struct4* ipv4List, mipc_data_v6_addr_struct4* ipv6List) {
    if (!cmdObj) return -1;

    switch (cmdObj->parameter.ho.info.addr_type) {
        case NETAGENT_IO_ADDR_TYPE_IPv4:
            if (ipv4List != NULL) {
                for (int i = 0; i < IPV4_SEGMENT_LENGTH; i++) {
                    cmdObj->parameter.ho.addr.addr_v4[i] = ipv4List[0].addr[i];
                }
            }
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv6:
            if (ipv6List != NULL) {
                for (int i = 0; i < IPV6_SEGMENT_LENGTH; i++) {
                    cmdObj->parameter.ho.addr.addr_v6[i] = ipv6List[0].addr[i];
                }
            }
            break;
        case NETAGENT_IO_ADDR_TYPE_IPv4v6:
            if (ipv4List != NULL) {
                for (int i = 0; i < IPV4_SEGMENT_LENGTH; i++) {
                    cmdObj->parameter.ho.addr.addr_v4[i] = ipv4List[0].addr[i];
                }
            }
            if (ipv6List != NULL) {
                for (int j = 0; j < IPV6_SEGMENT_LENGTH; j++) {
                    cmdObj->parameter.ho.addr.addr_v6[j] = ipv6List[0].addr[j];
                }
            }
            break;
        default:
            NA_LOG_E("[%s] invalid address type", __FUNCTION__);
            break;
    }
    return 1;
}

netagent_io_addr_type_e MipcEventHandler::getSourceAddrType(unsigned int ipv4Count,
        unsigned int ipv6Count) {
    if (ipv4Count > 0 && ipv6Count > 0) return NETAGENT_IO_ADDR_TYPE_IPv4v6;
    if (ipv4Count > 0 && ipv6Count == 0) return NETAGENT_IO_ADDR_TYPE_IPv4;
    if (ipv4Count == 0 && ipv6Count > 0) return NETAGENT_IO_ADDR_TYPE_IPv6;
    return NETAGENT_IO_ADDR_TYPE_UNKNOWN;
}

int MipcEventHandler::getAddrV4(void* o, unsigned int* addr) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    switch (cmdObj->cmd) {
        case NETAGENT_IO_CMD_IFUP:
        case NETAGENT_IO_CMD_IPCHG:
        case NETAGENT_IO_CMD_IFCHANGE:
            if (cmdObj->parameter.ip.addr_type & NETAGENT_IO_ADDR_TYPE_IPv4) {
                memcpy(addr, &(cmdObj->parameter.ip.addr.addr_v4),
                        sizeof(cmdObj->parameter.ip.addr.addr_v4));
                return NETAGENT_IO_RET_SUCCESS;
            }
            NA_LOG_E("[%s] invalid addrType %d",
                    __FUNCTION__, cmdObj->parameter.ip.addr_type);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        case NETAGENT_IO_CMD_PDNHO:
            if (cmdObj->parameter.ho.info.addr_type & NETAGENT_IO_ADDR_TYPE_IPv4) {
                memcpy(addr, &(cmdObj->parameter.ho.addr.addr_v4),
                        sizeof(cmdObj->parameter.ho.addr.addr_v4));
                return NETAGENT_IO_RET_SUCCESS;
            }
            NA_LOG_E("[%s] invalid addrType %d",
                    __FUNCTION__, cmdObj->parameter.ho.info.addr_type);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            if (cmdObj->parameter.tcp.addr_type & NETAGENT_IO_ADDR_TYPE_IPv4) {
                memcpy(addr, &(cmdObj->parameter.tcp.addr.addr_v4),
                        sizeof(cmdObj->parameter.tcp.addr.addr_v4));
                return NETAGENT_IO_RET_SUCCESS;
            }
            NA_LOG_E("[%s] invalid addrType %d",
                    __FUNCTION__, cmdObj->parameter.tcp.addr_type);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        default:
            NA_LOG_E("[%s] invalid cmd %d", __FUNCTION__, cmdObj->cmd);
    }

    return NETAGENT_IO_RET_GENERIC_FAILURE;
}

int MipcEventHandler::getAddrV6(void* o, unsigned int* addr) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    switch (cmdObj->cmd) {
        case NETAGENT_IO_CMD_IFUP:
        case NETAGENT_IO_CMD_IPCHG:
        case NETAGENT_IO_CMD_IFCHANGE:
            if (cmdObj->parameter.ip.addr_type & NETAGENT_IO_ADDR_TYPE_IPv6) {
                memcpy(addr, cmdObj->parameter.ip.addr.addr_v6,
                        sizeof(cmdObj->parameter.ip.addr.addr_v6));
                return NETAGENT_IO_RET_SUCCESS;
            }
            NA_LOG_E("[%s] invalid addrType %d",
                   __FUNCTION__, cmdObj->parameter.tcp.addr_type);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        case NETAGENT_IO_CMD_PDNHO:
            if (cmdObj->parameter.ho.info.addr_type & NETAGENT_IO_ADDR_TYPE_IPv6) {
                memcpy(addr, cmdObj->parameter.ho.addr.addr_v6,
                        sizeof(cmdObj->parameter.ho.addr.addr_v6));
                return NETAGENT_IO_RET_SUCCESS;
            }
            NA_LOG_E("[%s] invalid addrType %d",
                    __FUNCTION__, cmdObj->parameter.ho.info.addr_type);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            if (cmdObj->parameter.tcp.addr_type & NETAGENT_IO_ADDR_TYPE_IPv6) {
                memcpy(addr, cmdObj->parameter.tcp.addr.addr_v6,
                        sizeof(cmdObj->parameter.tcp.addr.addr_v6));
                return NETAGENT_IO_RET_SUCCESS;
            }
            NA_LOG_E("[%s] invalid addrType %d",
                    __FUNCTION__, cmdObj->parameter.tcp.addr_type);
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        default:
            NA_LOG_E("[%s] invalid cmd %d", __FUNCTION__, cmdObj->cmd);
    }

    return NETAGENT_IO_RET_GENERIC_FAILURE;
}

int MipcEventHandler::getInterfaceId(void* o, unsigned int* ifId) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *ifId = cmdObj->trans_intf_id;
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getAddrType(void* o, netagent_io_addr_type_e* addrType) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    switch (cmdObj->cmd) {
        case NETAGENT_IO_CMD_IFUP:
        case NETAGENT_IO_CMD_IPCHG:
        case NETAGENT_IO_CMD_IFCHANGE:
            *addrType = cmdObj->parameter.ip.addr_type;
            return NETAGENT_IO_RET_SUCCESS;
        case NETAGENT_IO_CMD_IFDOWN:
            *addrType = NETAGENT_IO_ADDR_TYPE_UNKNOWN;
            return NETAGENT_IO_RET_SUCCESS;
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            *addrType = cmdObj->parameter.tcp.addr_type;
            return NETAGENT_IO_RET_SUCCESS;
        case NETAGENT_IO_CMD_SPI_RSVN:
            *addrType = cmdObj->parameter.spi.addr_type;
            return NETAGENT_IO_RET_SUCCESS;
        default:
            NA_LOG_E("[%s] invalid cmd %d", __FUNCTION__, cmdObj->cmd);
    }

    return NETAGENT_IO_RET_GENERIC_FAILURE;
}

int MipcEventHandler::getReason(void* o, char **reason) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *reason = cmdObj->reason;
    return NETAGENT_IO_RET_SUCCESS;
}

// MTU value is from +EIF:mtu, mainly used for PDN trigged by modem.
int MipcEventHandler::getModemPdnMtu(void *o, unsigned int *mtu) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    if ((cmdObj->parameter.mtu.ipv4_mtu) <= 0) {
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *mtu = cmdObj->parameter.mtu.ipv4_mtu;
    return NETAGENT_IO_RET_SUCCESS;
}

// MTU value is from +EIF:ifchange, can get mtuV4 and mtuV6 if network reports.
int MipcEventHandler::getMtu(void *o,
        unsigned int protocol, unsigned int *mtu) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    if (protocol == NETAGENT_IO_ADDR_TYPE_IPv4) {
        if ((cmdObj->parameter.ip.mtu.ipv4_mtu) <= 0) {
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        }
        *mtu = cmdObj->parameter.ip.mtu.ipv4_mtu;
    }

    if (protocol == NETAGENT_IO_ADDR_TYPE_IPv6) {
        if ((cmdObj->parameter.ip.mtu.ipv6_mtu) <= 0) {
            return NETAGENT_IO_RET_GENERIC_FAILURE;
        }
        *mtu = cmdObj->parameter.ip.mtu.ipv6_mtu;
    }

    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getCommandType(void *o, netagent_io_cmd_e *cmd) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *cmd = cmdObj->cmd;
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::freeCommand(void *o) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    FREEIF(cmdObj->reason);
    if (cmdObj->header != NULL) {
        mipc_msg_deinit(cmdObj->header);
    }
    FREEIF(cmdObj);
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getHeader(void *o, mipc_msg_t** header) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj || cmdObj->header == NULL) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *header = cmdObj->header;
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getRsvnAction(void *o, unsigned int *action) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    switch (cmdObj->cmd) {
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            *action = cmdObj->parameter.tcp.action;
            return NETAGENT_IO_RET_SUCCESS;
        case NETAGENT_IO_CMD_SPI_RSVN:
            *action = cmdObj->parameter.spi.action;
            return NETAGENT_IO_RET_SUCCESS;
        default:
            break;
    }

    NA_LOG_E("[%s] invalid command", __FUNCTION__);
    return NETAGENT_IO_RET_GENERIC_FAILURE;
}

int MipcEventHandler::getRsvnIfId(void *o, int *interfaceId) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    switch (cmdObj->cmd) {
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            *interfaceId = cmdObj->parameter.tcp.interfaceId;
            return NETAGENT_IO_RET_SUCCESS;
        default:
            break;
    }

    NA_LOG_E("[%s] invalid command", __FUNCTION__);
    return NETAGENT_IO_RET_GENERIC_FAILURE;
}

int MipcEventHandler::getRsvnPort(void *o, int *port) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    switch (cmdObj->cmd) {
        case NETAGENT_IO_CMD_TCP_RSVN:
        case NETAGENT_IO_CMD_UDP_RSVN:
            *port = cmdObj->parameter.tcp.port;
            return NETAGENT_IO_RET_SUCCESS;
        default:
            break;
    }

    NA_LOG_E("[%s] invalid command", __FUNCTION__);
    return NETAGENT_IO_RET_GENERIC_FAILURE;
}

void MipcEventHandler::parseIpspiAddr(netagent_io_cmd_obj_t *cmdObj,
        mipc_addr_struct4 *srcIpAddr, mipc_addr_struct4 *destIpAddr) {
    if (srcIpAddr->addr_len == IPV4_SEGMENT_LENGTH) {
        cmdObj->parameter.spi.addr_type = NETAGENT_IO_ADDR_TYPE_IPv4;
        for (int i = 0; i < IPV4_SEGMENT_LENGTH; i++) {
            cmdObj->parameter.spi.addrSrc.addr_v4[i] = srcIpAddr->addr[i];
        }
    } else if (srcIpAddr->addr_len == IPV6_SEGMENT_LENGTH) {
        cmdObj->parameter.spi.addr_type = NETAGENT_IO_ADDR_TYPE_IPv6;
        for (int i = 0; i < IPV6_SEGMENT_LENGTH; i++) {
            cmdObj->parameter.spi.addrSrc.addr_v6[i] = srcIpAddr->addr[i];
        }
    } else {
        NA_LOG_E("[%s] invalid srcIpAddr", __FUNCTION__);
    }

    if (destIpAddr->addr_len == IPV4_SEGMENT_LENGTH) {
        for (int i = 0; i < IPV4_SEGMENT_LENGTH; i++) {
            cmdObj->parameter.spi.addrDst.addr_v4[i] = destIpAddr->addr[i];
        }
    } else if (destIpAddr->addr_len == IPV6_SEGMENT_LENGTH) {
        for (int i = 0; i < IPV6_SEGMENT_LENGTH; i++) {
            cmdObj->parameter.spi.addrDst.addr_v6[i] = destIpAddr->addr[i];
        }
    } else {
        NA_LOG_E("[%s] invalid destIpAddr", __FUNCTION__);
    }
}

int MipcEventHandler::getIpspiProtocol(void *o, unsigned int *protocol) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *protocol = cmdObj->parameter.spi.protocol;
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getIpspiMode(void *o, unsigned int *mode) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *mode = cmdObj->parameter.spi.mode;
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getIpspiMinMax(void *o, unsigned int *min, unsigned int *max) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    *min = cmdObj->parameter.spi.min;
    *max = cmdObj->parameter.spi.max;
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getIpspiAddrV4(void *o, netagent_io_spi_addr_whom_e whom,
        unsigned int *addr) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    if (whom == NETAGENT_IO_SPI_ADDR_SRC) {
        memcpy(addr, &(cmdObj->parameter.spi.addrSrc.addr_v4),
                sizeof(cmdObj->parameter.spi.addrSrc.addr_v4));
    } else {
        memcpy(addr, &(cmdObj->parameter.spi.addrDst.addr_v4),
                sizeof(cmdObj->parameter.spi.addrDst.addr_v4));
    }
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getIpspiAddrV6(void *o, netagent_io_spi_addr_whom_e whom,
        unsigned int *addr) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    if (whom == NETAGENT_IO_SPI_ADDR_SRC) {
        memcpy(addr, &(cmdObj->parameter.spi.addrSrc.addr_v6),
                sizeof(cmdObj->parameter.spi.addrSrc.addr_v6));
    } else {
        memcpy(addr, &(cmdObj->parameter.spi.addrDst.addr_v6),
                sizeof(cmdObj->parameter.spi.addrDst.addr_v6));
    }
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::getHOInfo(void *o, netagent_io_ho_info_t *hoInfo) {
    netagent_io_cmd_obj_t* cmdObj = (netagent_io_cmd_obj_t*)o;
    if (!cmdObj) {
        NA_LOG_E("[%s] invalid cmdObj", __FUNCTION__);
        return NETAGENT_IO_RET_GENERIC_FAILURE;
    }

    memcpy(hoInfo, &(cmdObj->parameter.ho.info), sizeof(netagent_io_ho_info_t));
    return NETAGENT_IO_RET_SUCCESS;
}

int MipcEventHandler::flushIpsecPolicy(
    const char *srcAddr, netagent_io_addr_type_e addrType, int isUserLoad) {
    int status = 0;
    char prefix[22] = "";
    // Process prefix only for IPV6 type address
    if (srcAddr != NULL && strlen(srcAddr) >= 20 && addrType != NETAGENT_IO_ADDR_TYPE_IPv4) {
        memset(&prefix, 0, sizeof(prefix));
        strncpy(prefix, srcAddr, 20);
        prefix[20] = ':';
        prefix[21] = '\0';
    }
    if (isUserLoad != 1) {
        NA_LOG_D("flush IPSEC policy, addr %s, addrType %d,prefix %s",
                srcAddr, addrType, prefix);
    } else {
        NA_LOG_D("flush IPSEC policy, addr ***, addrType %d,prefix %s",
                addrType, prefix);
    }
    switch (addrType) {
        // Clear policy for 32 bit IPv4 address
        case NETAGENT_IO_ADDR_TYPE_IPv4:
            execFmt(&status, SYSTEM_IP_WRAPPER_BIN_DIR " xfrm policy delete dir out src %s",
                    srcAddr);
            execFmt(&status, SYSTEM_IP_WRAPPER_BIN_DIR " xfrm policy delete dir in dst %s",
                    srcAddr);
            execFmt(&status, SYSTEM_IP_WRAPPER_BIN_DIR " xfrm policy delete dir fwd dst %s",
                    srcAddr);
        break;
        // Clear policy for IPv6 address with 64 bits prefix
        case NETAGENT_IO_ADDR_TYPE_IPv6:
            execFmt(&status, SYSTEM_IP_WRAPPER_BIN_DIR " xfrm policy deleteall dir out src %s/64",
                    prefix);
            execFmt(&status, SYSTEM_IP_WRAPPER_BIN_DIR " xfrm policy deleteall dir in dst %s/64",
                    prefix);
            execFmt(&status, SYSTEM_IP_WRAPPER_BIN_DIR " xfrm policy deleteall dir fwd dst %s/64",
                    prefix);
            break;
        default:
            NA_LOG_E("Invalid addrType %d", addrType);
    }
    return 0;
}

int MipcEventHandler::execFmt(int *status, const char *fmt, ...) {
    char *s = NULL;
    va_list args;
    int ret = 0;
    va_start(args, fmt);
    vasprintf(&s, fmt, args);
    ret = forkExec(s, status);
    FREEIF(s);
    va_end(args);
    return ret;
}

int MipcEventHandler::forkExec(const char *cmd, int *status) {
    const char *DELIM = " ";
    char *clone;
    int argc;
    char *argv[MAX_EXECV_ARG] = { 0 };
    if (cmd == NULL) return 0;
    char* saveptr = NULL;

    clone = strdup(cmd);
    argv[0] = strtok_r(clone, DELIM, &saveptr);
    for (argc = 1; argc < MAX_EXECV_ARG; argc++) {
        if ((argv[argc] = strtok_r(NULL, DELIM, &saveptr)) == NULL) {
            break;
        }
    }

    if (systemExt(argv) != 0) {
        FREEIF(clone);
        return -1;
    }

    FREEIF(clone);
    return 0;
}

int MipcEventHandler::systemExt(char *argv[]) {
    int pid = fork();
    if (pid == 0) {
        execv(argv[0], argv);
        return -1;
    } else if (pid > 0) {
        int wstatus, wexit_status;
        waitpid(pid, &wstatus, 0);
        wexit_status = WEXITSTATUS(wstatus);
        NA_LOG_D("Child process exit status: %d", wexit_status);
    } else {
        NA_LOG_E("Can't fork process");
        return -1;
    }

    return 0;
}

void MipcEventHandler::setHandoverInfo(HandoverInfo hoInfo) {
    netagent_io_cmd_obj_t* cmdObj = NULL;
    netagent_io_addr_type_e addrType = NETAGENT_IO_ADDR_TYPE_UNKNOWN;

    unsigned int transId = hoInfo.transIntId;
    NA_LOG_I("[%s] transId: %d", __FUNCTION__, transId);
    cmdObj = allocCmdObj(transId, NETAGENT_IO_CMD_PDNHO);
    if (cmdObj == NULL) {
        NA_LOG_E("[%s] invalid cmd object", __FUNCTION__);
        return;
    }

    cmdObj->parameter.ho.info.hostate = (netagent_io_ho_state_e)hoInfo.hoState;
    cmdObj->parameter.ho.info.is_succ = (netagent_io_ho_result_e)hoInfo.is_succ;
    cmdObj->parameter.ho.info.src_ran = (netagent_io_ho_ran_e)hoInfo.src_ran;
    cmdObj->parameter.ho.info.tgt_ran = (netagent_io_ho_ran_e)hoInfo.tgt_ran;

    copyHOIpAddressToCmdObj(cmdObj, hoInfo.addr_v4, hoInfo.addr_v6);

    if ((hoInfo.addr_v4 != NULL && hoInfo.addr_v4[0].addr[0] != 0) &&
                  (hoInfo.addr_v6 != NULL && hoInfo.addr_v6[0].addr[0]  != 0)) {
        addrType = NETAGENT_IO_ADDR_TYPE_IPv4v6;
    } else if ((hoInfo.addr_v4 != NULL && hoInfo.addr_v4[0].addr[0] != 0)) {
        addrType = NETAGENT_IO_ADDR_TYPE_IPv4;
    } else if ((hoInfo.addr_v6 != NULL && hoInfo.addr_v6[0].addr[0] == 0)) {
        addrType = NETAGENT_IO_ADDR_TYPE_IPv6;
    } else {
        addrType = NETAGENT_IO_ADDR_TYPE_UNKNOWN;
    }
    if (addrType == NETAGENT_IO_ADDR_TYPE_UNKNOWN) {
        NA_LOG_E("[%s] invalid address type", __FUNCTION__);
        goto error;
    } else {
        cmdObj->parameter.ho.info.addr_type = addrType;
    }
    NA_LOG_I("[%s] addrType: %d", __FUNCTION__, addrType);
    enqueueRequest(cmdObj);
    return;

error:
    FREEIF(cmdObj);
}

netagent_io_cmd_e MipcEventHandler::mipcCmd2InternalCmd(unsigned int cmd) {
    switch (cmd) {
        case MIPC_INTERNAL_EIF_IND_CMD_IFUP: return NETAGENT_IO_CMD_IFUP;
        case MIPC_INTERNAL_EIF_IND_CMD_IFDOWN: return NETAGENT_IO_CMD_IFDOWN;
        case MIPC_INTERNAL_EIF_IND_CMD_IPCHG: return  NETAGENT_IO_CMD_IPCHG;
        case MIPC_INTERNAL_EIF_IND_CMD_IFCHANGE: return  NETAGENT_IO_CMD_IFCHANGE;
        case MIPC_INTERNAL_EIF_IND_CMD_MTU: return NETAGENT_IO_CMD_SETMTU;
        default: return NETAGENT_IO_CMD_MAX;
    }
}

mipc_msg_sim_ps_id_enum MipcEventHandler::internalSlotId2MipcSlotId(unsigned int slotId) {
    switch (slotId) {
        case 0: return MIPC_MSG_SIM0;
        case 1: return MIPC_MSG_SIM1;
        case 2: return MIPC_MSG_SIM2;
        case 3: return MIPC_MSG_SIM3;
        default:
            return MIPC_MSG_SIM0;
    }
}

unsigned int MipcEventHandler::mipcSlotId2InternalSlotId(unsigned int slotId) {
    switch (slotId) {
        case MIPC_MSG_SIM0: return 0;
        case MIPC_MSG_SIM1: return 1;
        case MIPC_MSG_SIM2: return 2;
        case MIPC_MSG_SIM3: return 3;
        default:
            return 0;
    }
}
