/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_data_call_common.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_data_call_common相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_DATA_CALL_COMMON_H__
#define __SV_DATA_CALL_COMMON_H__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sv_net_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void sv_data_call_param_t;

typedef struct
{
    int call_id;/**<Data call id*/
    char call_name[SV_NET_MAX_NAME_LEN];/**<Data call name*/
    SV_NET_IP_VER_E ip_ver;/**<IP version IPV4 IPV6 or IPV46*/
    SV_NET_DATA_CALL_STATUS_E call_status;/**<Data call status*/
    char device[SV_NET_MAX_NAME_LEN];/**<Last data call connected interface*/
    uint8_t has_addr;  /**< Valid when data call state is connected*/
    sv_net_addr_t addr;/**<IPV4 infomation*/
    uint8_t has_addr6; /**< Valid when data call state is connected*/
    sv_net_addr6_t addr6;/**<IPV6 infomation*/
    int call_end_reason_type;  /**< Last data call disconnect reason type */
    int call_end_reason_code;  /**< Last data call disconnect reason code */
} sv_data_call_status_t;

typedef struct
{
    int call_id; /**<IP version IPV4 IPV6 or IPV46.*/
    char call_name[SV_NET_MAX_NAME_LEN];/**<Data call name*/
} sv_data_call_item_t;

typedef struct
{
    SV_NET_IP_VER_E ip_ver; /**<IP version IPV4 IPV6 or IPV46*/
    SV_NET_AUTH_PREF_E auth_pref; /**<Data call auth pref*/
    char apn_name[SV_NET_MAX_APN_NAME_LEN]; /**<Data call apn name*/
    char username[SV_NET_MAX_APN_USERNAME_LEN]; /**<Data call username*/
    char password[SV_NET_MAX_APN_PASSWORD_LEN]; /**<Data call password*/
} sv_data_call_apn_config_t;

typedef struct
{
    int call_id; 
    sv_net_data_filter_addr_info_t src_addr;
    sv_net_data_filter_addr_info_t dst_addr;
    SV_NET_PROTOCOL_TYPE_E protocol;
    SV_NET_IP_VER_E ip_ver; 
    uint16_t sport;
    uint16_t sport_range;
    uint16_t dport;
    uint16_t dport_range;
} sv_data_call_data_filter_t;

typedef struct
{
    uint64_t tx_pkts; /**<The number of packets sent.*/
    uint64_t tx_bytes; /**<The number of packets sent bytes*/
    uint64_t tx_dropped_pkts; /**<The number of packets dropped by sent*/
    uint64_t rx_pkts; /**<The number of packets received*/
    uint64_t rx_bytes; /**<The number of packets received bytes*/
    uint64_t rx_dropped_pkts; /**<The number of packets dropped by received.*/
}sv_data_call_pkt_stats_t;

typedef struct
{
    unsigned long current_tx_rate;   /**< Current TX data rate, Not support now */
    unsigned long current_rx_rate;   /**< Current RX data rate, Not support now */
    unsigned long max_tx_rate;       /**< Maximun TX data rate Unit: bits/sec */
    unsigned long max_rx_rate;       /**< Maximum RX data rate Unit: bits/sec */
}sv_data_call_bit_rate_t;

typedef void (*sv_data_call_status_ind_cb_f)(int call_id,
        SV_NET_DATA_CALL_STATUS_E pre_call_status,
        sv_data_call_status_t *p_msg);

typedef void (*sv_data_call_service_error_cb_f)(int error);

#ifdef __cplusplus
}
#endif

#endif