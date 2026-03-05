/**
 * @file ql_data_call_common.h
 * @brief Data service common define
 *
 * @detailes
 * EDIT HISTORY
 * This section contains comments describing changes made to the file.
 * Notice that changes are listed in reverse chronological order.
 *
 * $Header: $
 * when      |what, where, why
 * ----------|---------------------------------------------------------
 * 20241230  |Created
 */

#ifndef __QL_DATA_CALL_COMMON_H__
#define __QL_DATA_CALL_COMMON_H__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ql_net_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void ql_data_call_param_t;

typedef struct
{
    int call_id;/**<Data call id*/
    char call_name[QL_NET_MAX_NAME_LEN];/**<Data call name*/
    QL_NET_IP_VER_E ip_ver;/**<IP version IPV4 IPV6 or IPV46*/
    QL_NET_DATA_CALL_STATUS_E call_status;/**<Data call status*/
    char device[QL_NET_MAX_NAME_LEN];/**<Last data call connected interface*/
    uint8_t has_addr;  /**< Valid when data call state is connected*/
    ql_net_addr_t addr;/**<IPV4 infomation*/
    uint8_t has_addr6; /**< Valid when data call state is connected*/
    ql_net_addr6_t addr6;/**<IPV6 infomation*/
    int call_end_reason_type;  /**< Last data call disconnect reason type */
    int call_end_reason_code;  /**< Last data call disconnect reason code */
} ql_data_call_status_t;

typedef struct
{
    int call_id; /**<IP version IPV4 IPV6 or IPV46.*/
    char call_name[QL_NET_MAX_NAME_LEN];/**<Data call name*/
} ql_data_call_item_t;

typedef struct
{
    QL_NET_IP_VER_E ip_ver; /**<IP version IPV4 IPV6 or IPV46*/
    QL_NET_AUTH_PREF_E auth_pref; /**<Data call auth pref*/
    char apn_name[QL_NET_MAX_APN_NAME_LEN]; /**<Data call apn name*/
    char username[QL_NET_MAX_APN_USERNAME_LEN]; /**<Data call username*/
    char password[QL_NET_MAX_APN_PASSWORD_LEN]; /**<Data call password*/
} ql_data_call_apn_config_t;

typedef struct
{
    int call_id; 
    ql_net_data_filter_addr_info_t src_addr;
    ql_net_data_filter_addr_info_t dst_addr;
    QL_NET_PROTOCOL_TYPE_E protocol;
    QL_NET_IP_VER_E ip_ver; 
    uint16_t sport;
    uint16_t sport_range;
    uint16_t dport;
    uint16_t dport_range;
} ql_data_call_data_filter_t;

typedef struct
{
    uint64_t tx_pkts; /**<The number of packets sent.*/
    uint64_t tx_bytes; /**<The number of packets sent bytes*/
    uint64_t tx_dropped_pkts; /**<The number of packets dropped by sent*/
    uint64_t rx_pkts; /**<The number of packets received*/
    uint64_t rx_bytes; /**<The number of packets received bytes*/
    uint64_t rx_dropped_pkts; /**<The number of packets dropped by received.*/
}ql_data_call_pkt_stats_t;


typedef struct
{
    unsigned long current_tx_rate;   /**< Current TX data rate, Not support now */
    unsigned long current_rx_rate;   /**< Current RX data rate, Not support now */
    unsigned long max_tx_rate;       /**< Maximun TX data rate Unit: bits/sec */
    unsigned long max_rx_rate;       /**< Maximum RX data rate Unit: bits/sec */
}ql_data_call_bit_rate_t;

typedef void (*ql_data_call_status_ind_cb_f)(int call_id,
        QL_NET_DATA_CALL_STATUS_E pre_call_status,
        ql_data_call_status_t *p_msg);

typedef void (*ql_data_call_service_error_cb_f)(int error);


#ifdef __cplusplus
}
#endif

#endif