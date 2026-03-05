/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_wifi_common.h
  @brief  Wi-Fi common data struct

  @detailes
  Quectel AG55x series module Wi-Fi service.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly

  when       |   who       |    what, where, why
  --------   |   ---       |    ----------------------------------------------------------
  2020-04-03 |   ewen.li   |    Created .
  2021-11-08 |   ewen.li   |    Add Wi-Fi P2P data structure .

  Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/
#ifndef __QL_WIFI_COMMON_H__
#define __QL_WIFI_COMMON_H__

#include "ql_net_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QL_WIFI_MAX_STA_NUM        (32)   /**< Maximum number of external STA device that can be connected in AP mode */
#define QL_WIFI_MAX_ACL_MAC_CNT    (32)   /**< Maximum number of MAC address that can be set by access rule */
#define QL_WIFI_MAX_SCAN_INFO_CNT  (60)   /**< Maximum number of AP information that can be scanned in STA mode */

#define IS_VALID_QL_WIFI_SSID_HIDDEN(hidden) ((0 == hidden) || (1 == hidden))
#define IS_VALID_QL_WIFI_ISOLATE(isolate) ((0 == isolate) || (1 == isolate))
#define IS_VALID_QL_WIFI_MAX_STA_NUM(max_sta) ((1 <= max_sta) && (QL_WIFI_MAX_STA_NUM >= max_sta))

typedef enum QL_WIFI_WORK_MODE_ENUM
{
    QL_WIFI_WORK_MODE_MIN = -1,
    QL_WIFI_WORK_MODE_STA = 0,       /**< STA mode */
    QL_WIFI_WORK_MODE_AP0,           /**< Single AP mode */
    QL_WIFI_WORK_MODE_AP0_STA,       /**< STA and Single AP coexistence mode */
    QL_WIFI_WORK_MODE_AP0_AP1,       /**< Dual AP mode */
    QL_WIFI_WORK_MODE_AP0_AP1_STA,   /**< STA and dual AP coexistence mode */
    QL_WIFI_WORK_MODE_MAX
} QL_WIFI_WORK_MODE_E;
#define IS_VALID_QL_WIFI_WORK_MODE(mode) ((mode>QL_WIFI_WORK_MODE_MIN) && (mode<QL_WIFI_WORK_MODE_MAX))

typedef enum QL_WIFI_INDEX_ENUM
{
    QL_WIFI_AP_INDEX_MIN = -1,
    QL_WIFI_AP_INDEX_AP0 = 0,   /**< Index of hotspot 0 */
    QL_WIFI_AP_INDEX_AP1,       /**< Index of hotspot 1 */
    QL_WIFI_AP_INDEX_MAX
} QL_WIFI_AP_INDEX_E;
#define IS_VALID_QL_WIFI_INDEX(idx) ((idx>QL_WIFI_AP_INDEX_MIN) && (idx<QL_WIFI_AP_INDEX_MAX))

typedef enum
{
    QL_WIFI_SUB_REASON_NONE = 0,
    QL_WIFI_SUB_REASON_WRONG_KEY = 1,
    QL_WIFI_SUB_REASON_AUTH_FAILED = 2,
    QL_WIFI_SUB_REASON_NO_PSK_AVAILABLE = 3,
    QL_WIFI_SUB_REASON_CONN_FAILED = 4,

    QL_WIFI_SUB_REASON_P2P_GRP_REM_SILENT = 5,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_FORMATION_FAILED = 6,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_REQUESTED = 7,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_IDLE_TIMEOUT= 8,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_UNAVAILABLE = 9,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_GO_ENDING_SESSION = 10,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_PSK_FAILURE = 11,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_FREQ_CONFLICT = 12,
    QL_WIFI_SUB_REASON_P2P_GRP_REM_GO_LEAVE_CHANNEL = 13,

    QL_WIFI_SUB_REASON_P2P_FAIL_BASE = 15,
    QL_WIFI_SUB_REASON_P2P_FAIL_INFO_CURRENTLY_UNAVAILABLE = 16,
    QL_WIFI_SUB_REASON_P2P_FAIL_INCOMPATIBLE_PARAMS = 17,
    QL_WIFI_SUB_REASON_P2P_FAIL_LIMIT_REACHED = 18,
    QL_WIFI_SUB_REASON_P2P_FAIL_INVALID_PARAMS = 19,
    QL_WIFI_SUB_REASON_P2P_FAIL_UNABLE_TO_ACCOMMODATE = 20,
    QL_WIFI_SUB_REASON_P2P_FAIL_PREV_PROTOCOL_ERROR = 21,
    QL_WIFI_SUB_REASON_P2P_FAIL_NO_COMMON_CHANNELS = 22,
    QL_WIFI_SUB_REASON_P2P_FAIL_UNKNOWN_GROUP = 23,
    QL_WIFI_SUB_REASON_P2P_FAIL_BOTH_GO_INTENT_15 = 24,
    QL_WIFI_SUB_REASON_P2P_FAIL_INCOMPATIBLE_PROV_METHOD = 25,
    QL_WIFI_SUB_REASON_P2P_FAIL_REJECTED_BY_USER = 26,
    QL_WIFI_SUB_REASON_P2P_FAIL_TIMEOUT = 27,

    QL_WIFI_SUB_REASON_UNKNOW
} QL_WIFI_SUB_REASON_CODE_E;

typedef enum
{
    /**< Reason codes (IEEE Std 802.11-2016, 9.4.1.7, Table 9-45) */
    QL_WIFI_REASON_SUCCEED = 0,
    QL_WIFI_REASON_UNSPECIFIED = 1,
    QL_WIFI_REASON_PREV_AUTH_NOT_VALID = 2,
    QL_WIFI_REASON_DEAUTH_LEAVING = 3,
    QL_WIFI_REASON_DISASSOC_DUE_TO_INACTIVITY = 4,
    QL_WIFI_REASON_DISASSOC_AP_BUSY = 5,
    QL_WIFI_REASON_CLASS2_FRAME_FROM_NONAUTH_STA = 6,
    QL_WIFI_REASON_CLASS3_FRAME_FROM_NONASSOC_STA = 7,
    QL_WIFI_REASON_DISASSOC_STA_HAS_LEFT = 8,
    QL_WIFI_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,
    QL_WIFI_REASON_PWR_CAPABILITY_NOT_VALID = 10,
    QL_WIFI_REASON_SUPPORTED_CHANNEL_NOT_VALID = 11,
    QL_WIFI_REASON_BSS_TRANSITION_DISASSOC = 12,
    QL_WIFI_REASON_INVALID_IE = 13,
    QL_WIFI_REASON_MICHAEL_MIC_FAILURE = 14,
    QL_WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,
    QL_WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
    QL_WIFI_REASON_IE_IN_4WAY_DIFFERS = 17,
    QL_WIFI_REASON_GROUP_CIPHER_NOT_VALID = 18,
    QL_WIFI_REASON_PAIRWISE_CIPHER_NOT_VALID = 19,
    QL_WIFI_REASON_AKMP_NOT_VALID = 20,
    QL_WIFI_REASON_UNSUPPORTED_RSN_IE_VERSION = 21,
    QL_WIFI_REASON_INVALID_RSN_IE_CAPAB = 22,
    QL_WIFI_REASON_IEEE_802_1X_AUTH_FAILED = 23,
    QL_WIFI_REASON_CIPHER_SUITE_REJECTED = 24,
    QL_WIFI_REASON_TDLS_TEARDOWN_UNREACHABLE = 25,
    QL_WIFI_REASON_TDLS_TEARDOWN_UNSPECIFIED = 26,
    QL_WIFI_REASON_SSP_REQUESTED_DISASSOC = 27,
    QL_WIFI_REASON_NO_SSP_ROAMING_AGREEMENT = 28,
    QL_WIFI_REASON_BAD_CIPHER_OR_AKM = 29,
    QL_WIFI_REASON_NOT_AUTHORIZED_THIS_LOCATION = 30,
    QL_WIFI_REASON_SERVICE_CHANGE_PRECLUDES_TS = 31,
    QL_WIFI_REASON_UNSPECIFIED_QOS_REASON = 32,
    QL_WIFI_REASON_NOT_ENOUGH_BANDWIDTH = 33,
    QL_WIFI_REASON_DISASSOC_LOW_ACK = 34,
    QL_WIFI_REASON_EXCEEDED_TXOP = 35,
    QL_WIFI_REASON_STA_LEAVING = 36,
    QL_WIFI_REASON_END_TS_BA_DLS = 37,
    QL_WIFI_REASON_UNKNOWN_TS_BA = 38,
    QL_WIFI_REASON_TIMEOUT = 39,
    QL_WIFI_REASON_PEERKEY_MISMATCH = 45,
    QL_WIFI_REASON_AUTHORIZED_ACCESS_LIMIT_REACHED = 46,
    QL_WIFI_REASON_EXTERNAL_SERVICE_REQUIREMENTS = 47,
    QL_WIFI_REASON_INVALID_FT_ACTION_FRAME_COUNT = 48,
    QL_WIFI_REASON_INVALID_PMKID = 49,
    QL_WIFI_REASON_INVALID_MDE = 50,
    QL_WIFI_REASON_INVALID_FTE = 51,
    QL_WIFI_REASON_MESH_PEERING_CANCELLED = 52,
    QL_WIFI_REASON_MESH_MAX_PEERS = 53,
    QL_WIFI_REASON_MESH_CONFIG_POLICY_VIOLATION = 54,
    QL_WIFI_REASON_MESH_CLOSE_RCVD = 55,
    QL_WIFI_REASON_MESH_MAX_RETRIES = 56,
    QL_WIFI_REASON_MESH_CONFIRM_TIMEOUT = 57,
    QL_WIFI_REASON_MESH_INVALID_GTK = 58,
    QL_WIFI_REASON_MESH_INCONSISTENT_PARAMS = 59,
    QL_WIFI_REASON_MESH_INVALID_SECURITY_CAP = 60,
    QL_WIFI_REASON_MESH_PATH_ERROR_NO_PROXY_INFO = 61,
    QL_WIFI_REASON_MESH_PATH_ERROR_NO_FORWARDING_INFO = 62,
    QL_WIFI_REASON_MESH_PATH_ERROR_DEST_UNREACHABLE = 63,
    QL_WIFI_REASON_MAC_ADDRESS_ALREADY_EXISTS_IN_MBSS = 64,
    QL_WIFI_REASON_MESH_CHANNEL_SWITCH_REGULATORY_REQ = 65,
    QL_WIFI_REASON_MESH_CHANNEL_SWITCH_UNSPECIFIED = 66,

    /**< Reason codes (Quectel extentions) */
    QL_WIFI_REASON_SSID_NOT_FOUND = 10000,
    QL_WIFI_REASON_P2P_GO_NEG_FAILURE = 10001,
    QL_WIFI_REASON_P2P_GROUP_REMOVED = 10002
} QL_WIFI_REASON_CODE_E;

typedef enum QL_WIFI_AP_MODE_TYPE_ENUM
{
    QL_WIFI_AP_MODE_MIN = -1,
    QL_WIFI_AP_MODE_80211B = 0,   /**< IEEE 802.11b (2.4 GHz) */
    QL_WIFI_AP_MODE_80211BG,      /**< IEEE 802.11bg (2.4 GHz) */
    QL_WIFI_AP_MODE_80211BGN,     /**< IEEE 802.11bgn (2.4 GHz) */
    QL_WIFI_AP_MODE_80211AX_2G,   /**< IEEE 802.11ax (2.4 GHz) */
    QL_WIFI_AP_MODE_80211A,       /**< IEEE 802.11a (5 GHz) */
    QL_WIFI_AP_MODE_80211AN,      /**< IEEE 802.11an (5 GHz) */
    QL_WIFI_AP_MODE_80211AC,      /**< IEEE 802.11ac (5 GHz) */
    QL_WIFI_AP_MODE_80211AX_5G,   /**< IEEE 802.11ax (5 GHz) */
    QL_WIFI_AP_MODE_MAX
} QL_WIFI_AP_MODE_TYPE_E;
#define IS_VALID_QL_WIFI_AP_MODE(mode) ((mode > QL_WIFI_AP_MODE_MIN) && (mode < QL_WIFI_AP_MODE_MAX))

typedef enum QL_WIFI_BANDWIDTH_ENUM
{
    QL_WIFI_BANDWIDTH_MIN = -1,
    QL_WIFI_BANDWIDTH_20MHZ = 0,   /**< 20MHZ */
    QL_WIFI_BANDWIDTH_40MHZ,       /**< 40MHZ */
    QL_WIFI_BANDWIDTH_80MHZ,       /**< 80MHZ */
    QL_WIFI_BANDWIDTH_160MHZ,      /**< 160MHZ */
    QL_WIFI_BANDWIDTH_MAX
} QL_WIFI_BANDWIDTH_E;
#define IS_VALID_QL_WIFI_BANDWIDTH(bandwidth) ((bandwidth > QL_WIFI_BANDWIDTH_MIN) && (bandwidth < QL_WIFI_BANDWIDTH_MAX))

typedef enum QL_WIFI_AUTH_ENUM
{
    QL_WIFI_AUTH_MIN = -1,
    QL_WIFI_AUTH_OPEN = 0,                /**< OPEN */
    QL_WIFI_AUTH_WEP,                     /**< WEP */
    QL_WIFI_AUTH_WPA_PSK,                 /**< WPA Personal */
    QL_WIFI_AUTH_WPA2_PSK,                /**< WPA2 Personal */
    QL_WIFI_AUTH_WPA3_PSK,                /**< WPA3 Personal */
    QL_WIFI_AUTH_WPA_WPA2_PSK_BOTH,       /**< WPA&WPA2 Personal */
    QL_WIFI_AUTH_WPA2_WPA3_PSK_BOTH,      /**< WPA2&WPA3 Personal */
    QL_WIFI_AUTH_WPA_WPA2_WPA3_PSK_ALL,   /**< WPA&WPA2&WPA3 Personal */
    QL_WIFI_AUTH_MAX
} QL_WIFI_AUTH_E;
#define IS_VALID_QL_WIFI_AUTH(auth) ((auth > QL_WIFI_AUTH_MIN) && (auth < QL_WIFI_AUTH_MAX))

typedef enum QL_WIFI_AUTH_WPA_PSK_ENUM
{
    QL_WIFI_AUTH_WPA_PAIRWISE_MIN = -1,
    QL_WIFI_AUTH_WPA_PAIRWISE_AUTO = 0,   /**< TKIP and AES */
    QL_WIFI_AUTH_WPA_PAIRWISE_TKIP,       /**< TKIP */
    QL_WIFI_AUTH_WPA_PAIRWISE_AES,        /**< AES */
    QL_WIFI_AUTH_WPA_PAIRWISE_MAX
} QL_WIFI_AUTH_WPA_PSK_E;
#define IS_VALID_QL_WIFI_AUTH_WPA_PSK(pairwise) ((pairwise > QL_WIFI_AUTH_WPA_PAIRWISE_MIN) && (pairwise < QL_WIFI_AUTH_WPA_PAIRWISE_MAX))

typedef enum QL_WIFI_STATUS_ENUM
{
    QL_WIFI_STATUS_DISABLED = 0,   /**< Wi-Fi is disabled */
    QL_WIFI_STATUS_ENABLED,        /**< Wi-Fi is enabled */
    QL_WIFI_STATUS_ERROR           /**< An error occurs in enabling Wi-Fi */
} QL_WIFI_ENABLE_STATUS_E;

typedef enum QL_WIFI_MAC_RULE_ENUM
{
    QL_WIFI_MAC_ACL_RULE_MIN = -1,
    QL_WIFI_MAC_ACL_RULE_NONE = 0,   /**< No rules */
    QL_WIFI_MAC_ACL_RULE_BLACK,      /**< Blacklist */
    QL_WIFI_MAC_ACL_RULE_WHITE,      /**< Whitelist */
    QL_WIFI_MAC_ACL_RULE_MAX
} QL_WIFI_MAC_ACL_RULE_E;
#define IS_VALID_QL_WIFI_ACL_RULE(acl_rule) ((acl_rule > QL_WIFI_MAC_ACL_RULE_MIN) && (acl_rule < QL_WIFI_MAC_ACL_RULE_MAX))

typedef enum QL_WIFI_STA_STATUS_ENUM
{
    QL_WIFI_STA_STATUS_NONE,           /**< The corresponding Wi-Fi working mode is not set */
    QL_WIFI_STA_STATUS_IDLE,           /**< The corresponding Wi-Fi working mode is set, but the STA function is not enabled */
    QL_WIFI_STA_STATUS_CONNECTING,     /**< Connecting to AP */
    QL_WIFI_STA_STATUS_ASSOCIATED,     /**< Connected to AP, but no IP address is assigned */
    QL_WIFI_STA_STATUS_CONNECTED,      /**< Connected to the AP and an IP address is assigned */
    QL_WIFI_STA_STATUS_DISCONNECTED,   /**< Disconnected from AP */
    QL_WIFI_STA_STATUS_ERROR           /**< Error status */
} QL_WIFI_STA_STATUS_E;

typedef enum QL_WIFI_AP_STATUS_ENUM
{
    QL_WIFI_AP_STATUS_NONE,        /**< The corresponding Wi-Fi working mode is not set */
    QL_WIFI_AP_STATUS_IDLE,        /**< The corresponding Wi-Fi working mode is set, but the AP function is not enabled */
    QL_WIFI_AP_STATUS_ENABLING,    /**< This status indication is not currently supported */
    QL_WIFI_AP_STATUS_ENABLED,     /**< After the AP function is successfully enabled, this status indication will be activated */
    QL_WIFI_AP_STATUS_DISABLING,   /**< This status indication is not currently supported */
    QL_WIFI_AP_STATUS_ERROR        /**< Error status */
} QL_WIFI_AP_STATUS_E;

typedef struct ql_wifi_ind_ap_status_struct
{
    QL_WIFI_AP_STATUS_E status;   /**< The status of AP mode */
    char ifname[32];              /**< Linux network interface name, such as wlan0, wlan1, etc */
    char bssid[18];               /**< BSSID, basic service set identifier */
}ql_wifi_ap_status_t;

typedef struct ql_wifi_sta_status_struct
{
    QL_WIFI_STA_STATUS_E status;   /**< The status of STA mode */
    char ifname[32];               /**< Linux network interface name, such as wlan0, wlan1, etc */
    char ap_bssid[18];             /**< BSSID, basic service set identifier */
    int rssi;                      /**< The signal strength when the STA device is connected to the hotspot. Unit: dBm. -9999 indicates the invalid value */
    uint8_t has_addr;              /**< Whether it is an IPv4 address or not */
    ql_net_addr_t addr;            /**< IPv4 address. ql_net_addr_t is defined in the header file ql_net_common.h */
    uint8_t has_addr6;             /**< Whether it is an IPv6 address or not */
    ql_net_addr6_t addr6;          /**< IPv6 address. ql_net_addr6_t is defined in the header file ql_net_common.h */
    QL_WIFI_REASON_CODE_E reason_code;           /**< STA error reason code */
    QL_WIFI_SUB_REASON_CODE_E sub_reason_code;   /**< STA error sub-reason code */
} ql_wifi_sta_status_t;

typedef struct
{
    int is_connected;   /**< Connection status. 1 means connected; 0 means disconnected */
    char macaddr[18];   /**< MAC address of STA device */
}ql_wifi_sta_connect_status_t;

typedef struct ql_wifi_sta_scan_info_struct
{
    char bssid[18];   /**< BSSID, basic service set identifier */
    char essid[33];   /**< ESSID, extended service set identifier */
    int signal;       /**< Signal strength. Unit: dBm */
    int frequency;    /**< The current operating frequency. Unit: Hz */
    QL_WIFI_AUTH_E auth;
} ql_wifi_sta_scan_info_t;

typedef enum QL_WIFI_SCAN_REASON_CODE
{
    QL_WIFI_SCAN_REASON_CODE_SUCCESS = 0,  /**< Successfully */
    QL_WIFI_SCAN_REASON_CODE_BUSY = 1,  /** Scan is busy */
    QL_WIFI_SCAN_REASON_CODE_BREAK_START = 2,  /** Scan is broken by start sta */
    QL_WIFI_SCAN_REASON_CODE_BREAK_STOP = 3,  /** Scan is broken by stop sta */
    QL_WIFI_SCAN_REASON_CODE_UNKNOW = 4  /**< Unkown reasons */
}QL_WIFI_SCAN_REASON_CODE_E;

typedef struct ql_wifi_sta_scan_list_struct
{
    int cnt;  /**< Number of external hotspots scanned */
    ql_wifi_sta_scan_info_t info[QL_WIFI_MAX_SCAN_INFO_CNT];   /**< Information of scanned external hotspots */
}ql_wifi_sta_scan_list_t;

typedef struct ql_wifi_sta_scan_result_struct
{
    QL_WIFI_SCAN_REASON_CODE_E reason_code; /** Reason code */
    ql_wifi_sta_scan_list_t scan_list; /**< The list of scan result */
}ql_wifi_sta_scan_result_t;

typedef struct
{
    QL_WIFI_AUTH_E auth;   /**< Authentication type */
    union
    {
        /**< wep authentication information */
        struct
        {
            int default_index;    /**< WEP configuration index */
            char passwd[4][64];   /**< WEP password */
        } wep;

        /**< wpa_psk authentication information */
        struct
        {
            QL_WIFI_AUTH_WPA_PSK_E pairwise;   /**< Encryption type */
            char passwd[64];                   /**< WPA_PSK password */
            int group_rekey;                   /**< Group key update period */
        } wpa_psk;
    }info;
} ql_wifi_ap_auth_t;

typedef struct
{
    QL_WIFI_AUTH_E auth;   /**< Authentication type */
    union
    {
        /**< wep authentication information */
        struct
        {
            char passwd[64];   /**< WEP password */
        } wep;

        /**< wpa_psk authentication information */
        struct
        {
            QL_WIFI_AUTH_WPA_PSK_E pairwise;   /**< Encryption type */
            char passwd[64];                   /**< WPA_PSK password */
        } wpa_psk;
    }info;
} ql_wifi_sta_auth_t;

typedef struct
{
    char macaddr[18];   /**< MAC address. The format is: %02X:%02X:%02X:%02X:%02X:%02X. For example: 2F:3F:4F:5F:6F:7F */
}ql_wifi_mac_addr_t;

typedef struct
{
    int cnt;   /**< Number of MAC addresses, the maximum value is 32 */
    ql_wifi_mac_addr_t addr[QL_WIFI_MAX_ACL_MAC_CNT];   /**< MAC address list */
}ql_wifi_acl_mac_list_t;

typedef enum
{
    QL_WIFI_ACL_MAC_CMD_DEL = 0,   /**< Delete the MAC address */
    QL_WIFI_ACL_MAC_CMD_ADD        /**< Add the MAC address */
}QL_WIFI_ACL_MAC_CMD_E;

typedef enum
{
    QL_WIFI_P2P_ROLE_NONE = 0,  /**< No role */
    QL_WIFI_P2P_ROLE_CLI,       /**< client role */
    QL_WIFI_P2P_ROLE_GO         /**< group owner role */
}QL_WIFI_P2P_ROLE_E;

typedef enum
{
    QL_WIFI_P2P_EN_STATUS_DISABLED = 0,   /**< Wi-Fi P2P is disabled */
    QL_WIFI_P2P_EN_STATUS_ENABLED,        /**< Wi-Fi P2P is enabled */
    QL_WIFI_P2P_EN_STATUS_ERROR           /**< An error occurs in enabling Wi-Fi P2P */
}QL_WIFI_P2P_ENABLE_STATUS_E;

typedef enum
{
    QL_WIFI_P2P_STATUS_IDLE = 0,        /**< Initial status */
    QL_WIFI_P2P_STATUS_CONNECTING,      /**< P2P is connecting peer p2p device, including p2p role negotiation */
    QL_WIFI_P2P_STATUS_ASSOCIATED,      /**< P2P client connected with peer P2P device   but no IP address is assigned */
    QL_WIFI_P2P_STATUS_CONNECTED,       /**< P2P GO or client connected with peer P2P device, and an IP address is assigned */
    QL_WIFI_P2P_STATUS_DISCONNECTED,    /**< P2P disconnected */
    QL_WIFI_P2P_STATUS_ERROR,           /**< Error status */
}QL_WIFI_P2P_STATUS_E;

typedef struct
{
    int is_found;           /**< Found or Lost, 1 means P2P deivce found; 0 means P2P device lost */
    char macaddr[18];       /**< MAC address of P2P device */
    char dev_type[32];      /**< Type of P2P device */
    char dev_name[32];      /**< Name of P2P device */
}ql_wifi_p2p_found_dev_info_t;

typedef struct
{
    char macaddr[18];   /**< MAC address of peer P2P device */
}ql_wifi_p2p_req_peer_dev_info_t;

typedef struct
{
    QL_WIFI_P2P_STATUS_E status;   /**< The status of P2P */
    QL_WIFI_P2P_ROLE_E role;       /**< The role of P2P */
    int freq;                      /**< The frequency of P2P */
    char ssid[33];                 /**< The ssid of GO */
    char passwd[64];               /**< The passwd of ssid */
    char pin_code[9];              /**< Dynamically generated random from local display */
    char ifname[32];               /**< Linux network interface name, such as p2p0 */
    uint8_t has_addr;              /**< Whether it is an IPv4 address or not */
    ql_net_addr_t addr;            /**< IPv4 address. ql_net_addr_t is defined in the header file ql_net_common.h */
    uint8_t has_addr6;             /**< Whether it is an IPv6 address or not */
    ql_net_addr6_t addr6;          /**< IPv6 address. ql_net_addr6_t is defined in the header file ql_net_common.h */
    QL_WIFI_REASON_CODE_E reason_code;            /**< reason code */
    QL_WIFI_SUB_REASON_CODE_E sub_reason_code;    /**< sub-reason code */
} ql_wifi_p2p_status_t;

#ifdef __cplusplus
}
#endif
#endif

