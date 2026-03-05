#ifndef __QL_NET_COMMON_H__
#define __QL_NET_COMMON_H__
#include <stdint.h>

/**  Maximum length of the APN name. */
#define QL_NET_MAX_APN_NAME_LEN   150

/**  Maximum length of the APN user name. */
#define QL_NET_MAX_APN_USERNAME_LEN   127

/**  Maximum length of the password. */
#define QL_NET_MAX_APN_PASSWORD_LEN   127

/**  Maximum address length. */
#define QL_NET_MAX_ADDR_LEN       46

#define QL_NET_MAX_IPV6_PREFIX_LEN       64

#define QL_NET_MAX_DATA_CALL_NUM  16

/**  Maximum lenth of the device name. */
#define QL_NET_MAX_NAME_LEN         32
#define QL_NET_MAX_RECONNECT_INTERVAL_LEN  20
#define QL_NET_MAX_APN_ID                  16
#define QL_NET_USB_DEV_MAX_NUM             16

#define QL_NET_MAX_LANHOST_NUM      18
#define QL_NET_MAX_LANHOST_NAME_LEN 64

#define QL_NET_TCP_PAYLOAD_MAX_V01 100

typedef enum {
  QL_NET_DATA_CALL_STATUS_MIN = -1,
  QL_NET_DATA_CALL_STATUS_NONE = 0x0,    /**< Call state is NONE     */
  QL_NET_DATA_CALL_STATUS_CREATED = 0x1, /**< Call state was created  */
  QL_NET_DATA_CALL_STATUS_IDLE = 0x2,    /**< Call state is IDLE.    */
  QL_NET_DATA_CALL_STATUS_CONNECTING = 0x3,           /**< Call is connecting.    */
  QL_NET_DATA_CALL_STATUS_PARTIAL_V4_CONNECTED = 0x4, /**< V4V6 mode, V4 is connected.  */
  QL_NET_DATA_CALL_STATUS_PARTIAL_V6_CONNECTED = 0x5, /**< V4V6 mode, V6 is connected.  */
  QL_NET_DATA_CALL_STATUS_CONNECTED = 0x6,            /**< Call is connected.     */
  QL_NET_DATA_CALL_STATUS_DISCONNECTED = 0x7,         /**< Call is disconnected.  */
  QL_NET_DATA_CALL_STATUS_ERROR = 0x8,                /**< Error ocurred   */
  QL_NET_DATA_CALL_STATUS_DELETED = 0x9,              /**< Data Call was deleted   */
  QL_NET_DATA_CALL_STATUS_MAX
} QL_NET_DATA_CALL_STATUS_E;

#define IS_QL_NET_DATA_CALL_STATUS_VALID(status) \
    ((status)>QL_NET_DATA_CALL_STATUS_MIN && (status)<QL_NET_DATA_CALL_STATUS_MAX)

typedef enum {
  QL_NET_DATA_CALL_RECONNECT_MODE_MIN = -1,
  QL_NET_DATA_CALL_RECONNECT_DISABLE = 0x0, 
  QL_NET_DATA_CALL_RECONNECT_NORMAL = 0x1, /**< Normal mode, interval defined by reconnect_interval[0] in sec  */
  QL_NET_DATA_CALL_RECONNECT_MODE_1 = 0x2, /**< Mode 1, time interval list defined by reconnect_interval in sec,
    if the time interval list is T1,T2,T3...Tn, the reconnection interval is T1,T2,T3...Tn,Tn,Tn...Tn  */
  QL_NET_DATA_CALL_RECONNECT_MODE_2 = 0x3, /**< Mode 2, time interval list defined by reconnect_interval in sec,
    if the time interval list is T1,T2,T3...Tn, the reconnection interval is T1,T2,T3..Tn,T1,T2,T3..Tn,T1..   */
  QL_NET_DATA_CALL_RECONONECT_MODE_MAX
} QL_NET_DATA_CALL_RECONNECT_MODE_E;

#define IS_QL_NET_DATA_CALL_RECONNECT_MODE_VALID(mode) \
    ((mode)>QL_NET_DATA_CALL_RECONNECT_MODE_MIN && (mode)<QL_NET_DATA_CALL_RECONONECT_MODE_MAX)

typedef enum {
  QL_NET_IP_VER_MIN = -0,
  QL_NET_IP_VER_V4 = 0x1, 
  QL_NET_IP_VER_V6 = 0x2, 
  QL_NET_IP_VER_V4V6 = 0x3, 
  QL_NET_IP_VER_MAX
} QL_NET_IP_VER_E;

#define IS_QL_NET_IP_VER_VALID(ver) \
    ((ver)>QL_NET_IP_VER_MIN && (ver)<QL_NET_IP_VER_MAX)

typedef enum {
  QL_NET_AUTH_PREF_MIN = -1,
  QL_NET_AUTH_PREF_PAP_CHAP_NOT_ALLOWED = 0, 
  QL_NET_AUTH_PREF_PAP_ONLY_ALLOWED = 1, 
  QL_NET_AUTH_PREF_CHAP_ONLY_ALLOWED = 2, 
  QL_NET_AUTH_PREF_PAP_CHAP_BOTH_ALLOWED = 3, 
  QL_NET_AUTH_PREF_MAX
} QL_NET_AUTH_PREF_E;

#define IS_QL_NET_AUTH_PREF_VALID(pref) \
    ((pref)>QL_NET_AUTH_PREF_MIN && (pref)<QL_NET_AUTH_PREF_MAX)

typedef enum {
  QL_NET_USB_DEV_TYPE_MIN = 0,
  QL_NET_USB_DEV_TYPE_ECM = 1, 
  QL_NET_USB_DEV_TYPE_NCM = 2, 
  QL_NET_USB_DEV_TYPE_RNDIS = 3, 
  QL_NET_USB_DEV_TYPE_MBIM = 4, 
  QL_NET_USB_DEV_TYPE_MAX
} QL_NET_USB_DEV_TYPE_E;

typedef enum {
  QL_NET_USB_DEV_STATUS_MIN = -1,
  QL_NET_USB_DEV_STATUS_NONE = 0, 
  QL_NET_USB_DEV_STATUS_LINKDOWN = 1, 
  QL_NET_USB_DEV_STATUS_LINKUP = 2, 
  QL_NET_USB_DEV_STATUS_MAX
} QL_NET_USB_DEV_STATUS_E;

typedef enum {
  QL_NET_NF_CMD_FORMAT_MIN = -1,
  QL_NET_NF_CMD_FORMAT_COMMON = 0, 
  QL_NET_NF_CMD_FORMAT_JSON = 1, 
  QL_NET_NF_CMD_FORMAT_MAX
} QL_NET_NF_CMD_FORMAT_E;

typedef enum {
    QL_LANHOST_EVENT_ADD = 0,
    QL_LANHOST_EVENT_DEL,
    QL_LANHOST_EVENT_UPDATE,
} QL_LANHOST_EVENT_TYPE_E;

typedef struct {
  char device[QL_NET_MAX_NAME_LEN];
  QL_NET_USB_DEV_TYPE_E type;
  QL_NET_USB_DEV_STATUS_E status;
  int8_t inst_id;
} ql_net_usb_dev_t;

typedef struct {
  char addr[QL_NET_MAX_ADDR_LEN];    /**< IPV4 address format string */
  char netmask[QL_NET_MAX_ADDR_LEN]; /**< Subnet mask, format */
  uint8_t subnet_bits;                     /**< Subnet mask bit number, exp:
                                            255.255.255.0 is 24, 255.255.0.0 is 16 */
  char gateway[QL_NET_MAX_ADDR_LEN]; /**< Gateway, IPV4 address*/
  char dnsp[QL_NET_MAX_ADDR_LEN];    /**< Primary DNS server address. */
  char dnss[QL_NET_MAX_ADDR_LEN];    /**< Secondary DNS server address. */
} ql_net_addr_t;

typedef struct 
{
  char addr[QL_NET_MAX_ADDR_LEN];    /**< IPV6 address format string */ 
  char prefix[QL_NET_MAX_ADDR_LEN];
  int32_t prefix_bits;
  char gateway[QL_NET_MAX_ADDR_LEN]; /**< Gateway address. */
  char dnsp[QL_NET_MAX_ADDR_LEN];    /**< Primary DNS server address. */
  char dnss[QL_NET_MAX_ADDR_LEN];    /**< Secondary DNS server address. */
} ql_net_addr6_t;

typedef struct
{
    char addr[QL_NET_MAX_ADDR_LEN];    /**< IPV4 address format string */
    char macaddr[18];                   /**< MAC address, format: XX:XX:XX:XX:XX:XX */
    char name[QL_NET_MAX_LANHOST_NAME_LEN]; /**< hostname */
    char ifname[QL_NET_MAX_NAME_LEN];  /**< device name */
    int  uptime;                       /**< online time in sec */
} ql_lanhost_t;

typedef enum {
  QL_NET_PRIMARY_SUBSCRIPTION = 0x1, /**<  Primary \n  */
  QL_NET_SECONDARY_SUBSCRIPTION = 0x2, /**<  Secondary \n  */
}QL_NET_BIND_SUBSCRIPTION_E;

typedef enum {
  QL_NET_PROTOCOL_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  QL_NET_PROTO_TCP_V01 = 0x01, /**<  Transmission Control Protocol (TCP). \n  */
  QL_NET_PROTO_UDP_V01 = 0x02, /**<  User Datagram Protocol (UDP). \n  */
  QL_NET_PROTO_TCP_UDP_V01 = 0x03, /**<  TCP/UDP.  */
  QL_NET_PROTOCOL_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}QL_NET_PROTOCOL_TYPE_E;

#define IS_QL_NET_PROTOCOL_VALID(ver) \
    ((ver)>QL_NET_PROTOCOL_ENUM_MIN_ENUM_VAL_V01 && (ver)<QL_NET_PROTOCOL_ENUM_MAX_ENUM_VAL_V01)


typedef struct
{
    char addr[QL_NET_MAX_ADDR_LEN];
    char netmask[QL_NET_MAX_ADDR_LEN];
}ql_net_data_filter_addr_t;

typedef struct
{
    char addr6[QL_NET_MAX_ADDR_LEN];
    uint8_t prefix_len;
}ql_net_data_filter_addr6_t;
typedef struct
{
    ql_net_data_filter_addr_t ipv4_addr;
    ql_net_data_filter_addr6_t ipv6_addr;
} ql_net_data_filter_addr_info_t;

#endif

