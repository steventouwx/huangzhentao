/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_net_common.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_net_common相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_NET_COMMON_H__
#define __SV_NET_COMMON_H__
#include <stdint.h>

/**  Maximum length of the APN name. */
#define SV_NET_MAX_APN_NAME_LEN   150

/**  Maximum length of the APN user name. */
#define SV_NET_MAX_APN_USERNAME_LEN   127

/**  Maximum length of the password. */
#define SV_NET_MAX_APN_PASSWORD_LEN   127

/**  Maximum address length. */
#define SV_NET_MAX_ADDR_LEN       46

#define SV_NET_MAX_IPV6_PREFIX_LEN       64

#define SV_NET_MAX_DATA_CALL_NUM  16

/**  Maximum lenth of the device name. */
#define SV_NET_MAX_NAME_LEN         32
#define SV_NET_MAX_RECONNECT_INTERVAL_LEN  20
#define SV_NET_MAX_APN_ID                  16
#define SV_NET_USB_DEV_MAX_NUM             16

#define SV_NET_MAX_LANHOST_NUM      18
#define SV_NET_MAX_LANHOST_NAME_LEN 64

#define SV_NET_TCP_PAYLOAD_MAX_V01 100

typedef enum {
  SV_NET_DATA_CALL_STATUS_MIN = -1,
  SV_NET_DATA_CALL_STATUS_NONE = 0x0,    /**< Call state is NONE     */
  SV_NET_DATA_CALL_STATUS_CREATED = 0x1, /**< Call state was created  */
  SV_NET_DATA_CALL_STATUS_IDLE = 0x2,    /**< Call state is IDLE.    */
  SV_NET_DATA_CALL_STATUS_CONNECTING = 0x3,           /**< Call is connecting.    */
  SV_NET_DATA_CALL_STATUS_PARTIAL_V4_CONNECTED = 0x4, /**< V4V6 mode, V4 is connected.  */
  SV_NET_DATA_CALL_STATUS_PARTIAL_V6_CONNECTED = 0x5, /**< V4V6 mode, V6 is connected.  */
  SV_NET_DATA_CALL_STATUS_CONNECTED = 0x6,            /**< Call is connected.     */
  SV_NET_DATA_CALL_STATUS_DISCONNECTED = 0x7,         /**< Call is disconnected.  */
  SV_NET_DATA_CALL_STATUS_ERROR = 0x8,                /**< Error ocurred   */
  SV_NET_DATA_CALL_STATUS_DELETED = 0x9,              /**< Data Call was deleted   */
  SV_NET_DATA_CALL_STATUS_MAX
} SV_NET_DATA_CALL_STATUS_E;

#define IS_SV_NET_DATA_CALL_STATUS_VALID(status) \
    ((status)>SV_NET_DATA_CALL_STATUS_MIN && (status)<SV_NET_DATA_CALL_STATUS_MAX)

typedef enum {
  SV_NET_DATA_CALL_RECONNECT_MODE_MIN = -1,
  SV_NET_DATA_CALL_RECONNECT_DISABLE = 0x0, 
  SV_NET_DATA_CALL_RECONNECT_NORMAL = 0x1, /**< Normal mode, interval defined by reconnect_interval[0] in sec  */
  SV_NET_DATA_CALL_RECONNECT_MODE_1 = 0x2, /**< Mode 1, time interval list defined by reconnect_interval in sec,
    if the time interval list is T1,T2,T3...Tn, the reconnection interval is T1,T2,T3...Tn,Tn,Tn...Tn  */
  SV_NET_DATA_CALL_RECONNECT_MODE_2 = 0x3, /**< Mode 2, time interval list defined by reconnect_interval in sec,
    if the time interval list is T1,T2,T3...Tn, the reconnection interval is T1,T2,T3..Tn,T1,T2,T3..Tn,T1..   */
  SV_NET_DATA_CALL_RECONONECT_MODE_MAX
} SV_NET_DATA_CALL_RECONNECT_MODE_E;

#define IS_SV_NET_DATA_CALL_RECONNECT_MODE_VALID(mode) \
    ((mode)>SV_NET_DATA_CALL_RECONNECT_MODE_MIN && (mode)<SV_NET_DATA_CALL_RECONONECT_MODE_MAX)

typedef enum {
  SV_NET_IP_VER_MIN = -0,
  SV_NET_IP_VER_V4 = 0x1, 
  SV_NET_IP_VER_V6 = 0x2, 
  SV_NET_IP_VER_V4V6 = 0x3, 
  SV_NET_IP_VER_MAX
} SV_NET_IP_VER_E;

#define IS_SV_NET_IP_VER_VALID(ver) \
    ((ver)>SV_NET_IP_VER_MIN && (ver)<SV_NET_IP_VER_MAX)

typedef enum {
  SV_NET_AUTH_PREF_MIN = -1,
  SV_NET_AUTH_PREF_PAP_CHAP_NOT_ALLOWED = 0, 
  SV_NET_AUTH_PREF_PAP_ONLY_ALLOWED = 1, 
  SV_NET_AUTH_PREF_CHAP_ONLY_ALLOWED = 2, 
  SV_NET_AUTH_PREF_PAP_CHAP_BOTH_ALLOWED = 3, 
  SV_NET_AUTH_PREF_MAX
} SV_NET_AUTH_PREF_E;

#define IS_SV_NET_AUTH_PREF_VALID(pref) \
    ((pref)>SV_NET_AUTH_PREF_MIN && (pref)<SV_NET_AUTH_PREF_MAX)

typedef enum {
  SV_NET_USB_DEV_TYPE_MIN = 0,
  SV_NET_USB_DEV_TYPE_ECM = 1, 
  SV_NET_USB_DEV_TYPE_NCM = 2, 
  SV_NET_USB_DEV_TYPE_RNDIS = 3, 
  SV_NET_USB_DEV_TYPE_MBIM = 4, 
  SV_NET_USB_DEV_TYPE_MAX
} SV_NET_USB_DEV_TYPE_E;

typedef enum {
  SV_NET_USB_DEV_STATUS_MIN = -1,
  SV_NET_USB_DEV_STATUS_NONE = 0, 
  SV_NET_USB_DEV_STATUS_LINKDOWN = 1, 
  SV_NET_USB_DEV_STATUS_LINKUP = 2, 
  SV_NET_USB_DEV_STATUS_MAX
} SV_NET_USB_DEV_STATUS_E;

typedef enum {
  SV_NET_NF_CMD_FORMAT_MIN = -1,
  SV_NET_NF_CMD_FORMAT_COMMON = 0, 
  SV_NET_NF_CMD_FORMAT_JSON = 1, 
  SV_NET_NF_CMD_FORMAT_MAX
} SV_NET_NF_CMD_FORMAT_E;

typedef enum {
    SV_LANHOST_EVENT_ADD = 0,
    SV_LANHOST_EVENT_DEL,
    SV_LANHOST_EVENT_UPDATE,
} SV_LANHOST_EVENT_TYPE_E;

typedef struct {
  char device[SV_NET_MAX_NAME_LEN];
  SV_NET_USB_DEV_TYPE_E type;
  SV_NET_USB_DEV_STATUS_E status;
  int8_t inst_id;
} sv_net_usb_dev_t;

typedef struct {
  char addr[SV_NET_MAX_ADDR_LEN];    /**< IPV4 address format string */
  char netmask[SV_NET_MAX_ADDR_LEN]; /**< Subnet mask, format */
  uint8_t subnet_bits;                     /**< Subnet mask bit number, exp:
                                            255.255.255.0 is 24, 255.255.0.0 is 16 */
  char gateway[SV_NET_MAX_ADDR_LEN]; /**< Gateway, IPV4 address*/
  char dnsp[SV_NET_MAX_ADDR_LEN];    /**< Primary DNS server address. */
  char dnss[SV_NET_MAX_ADDR_LEN];    /**< Secondary DNS server address. */
} sv_net_addr_t;

typedef struct 
{
  char addr[SV_NET_MAX_ADDR_LEN];    /**< IPV6 address format string */ 
  char prefix[SV_NET_MAX_ADDR_LEN];
  int32_t prefix_bits;
  char gateway[SV_NET_MAX_ADDR_LEN]; /**< Gateway address. */
  char dnsp[SV_NET_MAX_ADDR_LEN];    /**< Primary DNS server address. */
  char dnss[SV_NET_MAX_ADDR_LEN];    /**< Secondary DNS server address. */
} sv_net_addr6_t;

typedef struct
{
    char addr[SV_NET_MAX_ADDR_LEN];    /**< IPV4 address format string */
    char macaddr[18];                   /**< MAC address, format: XX:XX:XX:XX:XX:XX */
    char name[SV_NET_MAX_LANHOST_NAME_LEN]; /**< hostname */
    char ifname[SV_NET_MAX_NAME_LEN];  /**< device name */
    int  uptime;                       /**< online time in sec */
} sv_lanhost_t;

typedef enum {
  SV_NET_PRIMARY_SUBSCRIPTION = 0x1, /**<  Primary \n  */
  SV_NET_SECONDARY_SUBSCRIPTION = 0x2, /**<  Secondary \n  */
}SV_NET_BIND_SUBSCRIPTION_E;

typedef enum {
  SV_NET_PROTOCOL_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  SV_NET_PROTO_TCP_V01 = 0x01, /**<  Transmission Control Protocol (TCP). \n  */
  SV_NET_PROTO_UDP_V01 = 0x02, /**<  User Datagram Protocol (UDP). \n  */
  SV_NET_PROTO_TCP_UDP_V01 = 0x03, /**<  TCP/UDP.  */
  SV_NET_PROTOCOL_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}SV_NET_PROTOCOL_TYPE_E;

#define IS_SV_NET_PROTOCOL_VALID(ver) \
    ((ver)>SV_NET_PROTOCOL_ENUM_MIN_ENUM_VAL_V01 && (ver)<SV_NET_PROTOCOL_ENUM_MAX_ENUM_VAL_V01)

typedef struct
{
    char addr[SV_NET_MAX_ADDR_LEN];
    char netmask[SV_NET_MAX_ADDR_LEN];
}sv_net_data_filter_addr_t;

typedef struct
{
    char addr6[SV_NET_MAX_ADDR_LEN];
    uint8_t prefix_len;
}sv_net_data_filter_addr6_t;
typedef struct
{
    sv_net_data_filter_addr_t ipv4_addr;
    sv_net_data_filter_addr6_t ipv6_addr;
} sv_net_data_filter_addr_info_t;

#endif

