#ifndef __MIPC_MSG_DATA_CONST_H__
#define __MIPC_MSG_DATA_CONST_H__

enum MIPC_DATA_MSG_enum {
    MIPC_DATA_MSG_NONE = 0,
    /* This command is adopted to activate a data call to data network. It triggers to establish data stream with specific APN and APN type. When the command is complete, it replies the data call state and related information to user. If the data call has existed, it remains the state unchanged and replies with existed data. The ID in the MIPC_DATA_ACT_CALL_CNF is context identifier, it is used to recognize the data call session. User can use it to operate related actions such as deactivate data call, get data call information, get data call QoS, etc. */
    MIPC_DATA_ACT_CALL_REQ                                  = 513,
    MIPC_DATA_ACT_CALL_CNF                                  = 514,

    /* This command is adopted to deactivate the data call to data network. It uses context identifier which gets from MIPC_DATA_CALL_CNF to deactivate the data call. */
    MIPC_DATA_DEACT_CALL_REQ                                = 515,
    MIPC_DATA_DEACT_CALL_CNF                                = 516,

    /* This command is adopted to acquire the data call information by established context identifier. */
    MIPC_DATA_GET_CALL_REQ                                  = 517,
    MIPC_DATA_GET_CALL_CNF                                  = 518,

    /* This command is adopted to set packet filter white list (unmatched packets will be dropped). */
    MIPC_DATA_SET_PACKET_FILTER_REQ                         = 519,
    MIPC_DATA_SET_PACKET_FILTER_CNF                         = 520,

    /* This command is adopted to acquire current packet filter setting. */
    MIPC_DATA_GET_PACKET_FILTER_REQ                         = 521,
    MIPC_DATA_GET_PACKET_FILTER_CNF                         = 522,

    /* This command is adopted to acquire operator-specific PCO (Protocol Configuration Options). It should use context identifier to get the data. */
    MIPC_DATA_GET_PCO_REQ                                   = 523,
    MIPC_DATA_GET_PCO_CNF                                   = 524,

    /* This command is adopted to set specific SIM slot data to allow data transmission. */
    MIPC_DATA_SET_DATA_ALLOW_REQ                            = 525,
    MIPC_DATA_SET_DATA_ALLOW_CNF                            = 526,

    /* This command is adopted to acquire data call state list from modem. It uses array to represent all data call state. The index of array is context identifier, and the value of array is the context identifier state. */
    MIPC_DATA_GET_MD_DATA_CALL_LIST_REQ                     = 527,
    MIPC_DATA_GET_MD_DATA_CALL_LIST_CNF                     = 528,

    /* This command is adopted to set data configuration, including mobile data, data roaming, VoLTE, IMS test mode, data domestic roaming, and data international roaming. */
    MIPC_DATA_SET_CONFIG_REQ                                = 529,
    MIPC_DATA_SET_CONFIG_CNF                                = 530,

    /* This command is adopted to acquire data configuration, including mobile data, data roaming, VoLTE, IMS test mode, data domestic roaming, and data international roaming. */
    MIPC_DATA_GET_CONFIG_REQ                                = 531,
    MIPC_DATA_GET_CONFIG_CNF                                = 532,

    /* This command is adopted to abort the data call activation before acquiring the response of MIPC_DATA_ACT_CALL_REQ. It uses APN defined in MIPC_DATA_ACT_CALL_REQ to abort the data call. */
    MIPC_DATA_ABORT_CALL_REQ                                = 533,
    MIPC_DATA_ABORT_CALL_CNF                                = 534,

    /* This command is adopted to acquire related info of data call connection, such as established time, end time, reject cause. This command should include at least one APN or ID. If call is connected, ID will be suggested. If call is not connected, APN will be suggested. */
    MIPC_DATA_GET_CALL_INFO_REQ                             = 535,
    MIPC_DATA_GET_CALL_INFO_CNF                             = 536,

    /* This command is adopted to acquire the range of supported PDP context identifier. */
    MIPC_DATA_GET_PDP_CID_REQ                               = 537,
    MIPC_DATA_GET_PDP_CID_CNF                               = 538,

    /* This command is used to query or reset retry timer. */
    MIPC_DATA_RETRY_TIMER_REQ                               = 539,
    MIPC_DATA_RETRY_TIMER_CNF                               = 540,

    /* This command is used to enable and set the extended link capacity estimation reporting criteria. */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ      = 541,
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_CNF      = 542,

    /* This command is used to read dynamic parameters of secondary PDP context. It should use context identifier of secondary PDP context to get the data. */
    MIPC_DATA_GET_DEDICATE_BEARER_INFO_REQ                  = 543,
    MIPC_DATA_GET_DEDICATE_BEARER_INFO_CNF                  = 544,

    /* This command is used to acquire the QoS parameters of the established PDP context. */
    MIPC_DATA_GET_QOS_REQ                                   = 545,
    MIPC_DATA_GET_QOS_CNF                                   = 546,

    /* This command is used to acquire the relevant information about Traffic Flow Template. */
    MIPC_DATA_GET_TFT_REQ                                   = 547,
    MIPC_DATA_GET_TFT_CNF                                   = 548,

    /* Only for specific customer, command used to defined parameters which should be set before initiating a PDN connection */
    MIPC_DATA_SET_LGDCONT_REQ                               = 549,
    MIPC_DATA_SET_LGDCONT_CNF                               = 550,

    /* Send the S-NSSAI value to modem for next PDU establishment. */
    MIPC_DATA_SET_NSSAI_REQ                                 = 551,
    MIPC_DATA_SET_NSSAI_CNF                                 = 552,

    /* This command is used to start or stop Keep Alive. */
    MIPC_DATA_KEEPALIVE_REQ                                 = 553,
    MIPC_DATA_KEEPALIVE_CNF                                 = 554,

    /* command is used to get DSDA state */
    MIPC_DATA_GET_DSDA_STATE_REQ                            = 555,
    MIPC_DATA_GET_DSDA_STATE_CNF                            = 556,

    /* This command is used to acquire the 5G QoS parameters of the established PDU session. */
    MIPC_DATA_GET_5GQOS_REQ                                 = 557,
    MIPC_DATA_GET_5GQOS_CNF                                 = 558,

    /* This command is used to manage PDU session ID. */
    MIPC_DATA_SET_PSI_REQ                                   = 559,
    MIPC_DATA_SET_PSI_CNF                                   = 560,

    /* This command is adopted to acquire UE Network Slice Selection Assistance Information (NSSAI). */
    MIPC_DATA_GET_NSSAI_REQ                                 = 561,
    MIPC_DATA_GET_NSSAI_CNF                                 = 562,

    /* to get URSP route profile */
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_REQ                    = 563,
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF                    = 564,

    /* to SET URSP UE policy */
    MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ                = 565,
    MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_CNF                = 566,

    /* to GET URSP UE policy */
    MIPC_DATA_GET_URSP_UE_POLICY_REQ                        = 567,
    MIPC_DATA_GET_URSP_UE_POLICY_CNF                        = 568,

    /* This commond is used to set reserved interface id. These if id will be used by ethernet PDU. Id range is 0~19. */
    MIPC_DATA_SET_RESERVED_IF_ID_REQ                        = 569,
    MIPC_DATA_SET_RESERVED_IF_ID_CNF                        = 570,

    /* This command is uesed to set pco info before active data call. It can also used to delete seted pco info with NULL PCO_LIST */
    MIPC_DATA_SET_PCO_REQ                                   = 571,
    MIPC_DATA_SET_PCO_CNF                                   = 572,

    /* Enables the MUSIM capable UE to specify the paging restriction preferences to the network for 3GPP access in EPS. */
    MIPC_DATA_SET_PAGING_REQ                                = 573,
    MIPC_DATA_SET_PAGING_CNF                                = 574,

    /* Returns the current paging restrictions. */
    MIPC_DATA_GET_PAGING_REQ                                = 575,
    MIPC_DATA_GET_PAGING_CNF                                = 576,

    /* This command is used to modify a data call information. */
    MIPC_DATA_MOD_CALL_REQ                                  = 577,
    MIPC_DATA_MOD_CALL_CNF                                  = 578,

    /* This command is uesed to set TSN information. */
    MIPC_DATA_SET_TSN_REQ                                   = 579,
    MIPC_DATA_SET_TSN_CNF                                   = 580,

    /* Returns the current paging restrictions. */
    MIPC_DATA_SET_BEARER_INFO_REQ                           = 581,
    MIPC_DATA_SET_BEARER_INFO_CNF                           = 582,

    /* Set packet filter with IP tuple white list (not matched packets will be dropped), filter will be stored in modem, and it is needed MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ or enter sleep mode to trigger it activated, when Host leave sleep mode, flter will be deactivated automatically. */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ                = 583,
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_CNF                = 584,

    /* Delete packet filter which is set by SET_IP_TUPLE_PACKET_FILTER_REQ. */
    MIPC_DATA_DEL_IP_TUPLE_PACKET_FILTER_REQ                = 585,
    MIPC_DATA_DEL_IP_TUPLE_PACKET_FILTER_CNF                = 586,

    /* Trigger the packet filter which is set by SET_IP_TUPLE_PACKET_FILTER_REQ activated or deactivated. */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ           = 587,
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_CNF           = 588,

    /* It is used to get current filter count and mode. */
    MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_REQ           = 589,
    MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_CNF           = 590,

    /* It is used to get all the PLMN that configed UE policy. */
    MIPC_DATA_GET_URSP_UE_POLICY_PLMN_LIST_REQ              = 591,
    MIPC_DATA_GET_URSP_UE_POLICY_PLMN_LIST_CNF              = 592,

    /* This command is uesed to act packet route. */
    MIPC_DATA_ACT_PACKET_ROUTE_REQ                          = 607,
    MIPC_DATA_ACT_PACKET_ROUTE_CNF                          = 608,

    /* This command is uesed to set ifup packet route information. */
    MIPC_DATA_IFUP_PACKET_ROUTE_REQ                         = 609,
    MIPC_DATA_IFUP_PACKET_ROUTE_CNF                         = 610,

    /* REQ used to trigger modem start data channel data path config.CNF used to provide modem configurations on data channel data path to AP. */
    MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_REQ                  = 611,
    MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF                  = 612,

    /* Network activates a data call indication or fallback data call indication. */
    MIPC_DATA_ACT_CALL_IND                                  = 16897,

    /* Network deactivates a data call indication. */
    MIPC_DATA_DEACT_CALL_IND                                = 16898,

    /* Network modifies a data call indication. */
    MIPC_DATA_MOD_CALL_IND                                  = 16899,

    /* Network modifiy PCO (Protocol Configuration Options) indication. */
    MIPC_DATA_MOD_PCO_IND                                   = 16900,

    /* host activate a data call indication */
    MIPC_DATA_WWAN_ACT_CALL_IND                             = 16901,

    /* host de-activate a data call indication */
    MIPC_DATA_WWAN_DEACT_CALL_IND                           = 16902,

    /* register call act ind */
    MIPC_DATA_CALL_ACT_REG_IND                              = 16903,

    /* register decall act ind */
    MIPC_DATA_CALL_DEACT_REG_IND                            = 16904,

    /* register mod call act ind */
    MIPC_DATA_CALL_MOD_REG_IND                              = 16905,

    /* This command is adopted to indicate the indication of activated data call from modem. */
    MIPC_DATA_MD_ACT_CALL_IND                               = 16906,

    /* This command is adopted to indicate the indication of deactivated data call from modem. */
    MIPC_DATA_MD_DEACT_CALL_IND                             = 16907,

    /* iwlan priority list indication */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND                       = 16908,

    /* This command is used to report uplink and downlink link capacity estimation. */
    MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND                    = 16909,

    /* This command is used to report Network Limit State. */
    MIPC_DATA_NW_LIMIT_IND                                  = 16910,

    /* This command is used to report T3396 or back-off timer information when data session is rejected from ESM network.  */
    MIPC_DATA_TIMER_IND                                     = 16911,

    /* This command is used to report Keep Alive info. */
    MIPC_DATA_KEEPALIVE_STATUS_IND                          = 16912,

    /* This command is used to report mobile data usage. */
    MIPC_DATA_MOBILE_DATA_USAGE_IND                         = 16913,

    /* This command is used to report network reject cause. */
    MIPC_DATA_NETWORK_REJECT_CAUSE_IND                      = 16914,

    /* This command is used to report DSDA (Dual SIM Dual Active) state. */
    MIPC_DATA_DSDA_STATE_IND                                = 16915,

    /* The command is to report UMTS PS domain state for specific modem branch. */
    MIPC_DATA_UMTS_PS_STATE_IND                             = 16916,

    /* This command is used to indicate the APN timer which has been reset. */
    MIPC_DATA_RETRY_TIMER_IND                               = 16917,

    /* This command is to indicate URSP re-evaluation */
    MIPC_DATA_URSP_REEVAL_IND                               = 16918,

    /* This command will be reported when UE policy is changed. */
    MIPC_DATA_URSP_UE_POLICY_CHG_IND                        = 16919,

    /* This command is used to report indication info of PDN netowrk cause for internet APN. */
    MIPC_DATA_PDN_NW_CAUSE_IND                              = 16920,

    /* Report the paging restriction result. */
    MIPC_DATA_PAGING_RESTRICTIONS_IND                       = 16921,

    /* This command is used to report TSN time information from NW-TT. */
    MIPC_DATA_TSN_TIME_IND                                  = 16922,

    /* The commond is to indicate a white list packet have received, the filter is set by MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ. */
    MIPC_DATA_IP_PACKET_RECV_IND                            = 16923,

    /* This command is used to ifup packet route send ind to AP. */
    MIPC_DATA_IFUP_PACKET_ROUTE_IND                         = 16928,

    /* ifup packet route cmd. */
    MIPC_DATA_IFUP_PACKET_ROUTE_CMD                         = 33280,
    MIPC_DATA_IFUP_PACKET_ROUTE_RSP                         = 33281,

    /* This command is adopted to notify whether the fallback CID is used or unused. */
    MIPC_DATA_ACT_CALL_NTF                                  = 49664,

    /* notify use or unuse fallback cid. */
    MIPC_DATA_ACT_PACKET_ROUTE_NTF                          = 49665,


};

    /* MIPC_MSG.DATA_ACT_CALL_REQ */
enum mipc_data_act_call_req_tlv_enum {
    mipc_data_act_call_req_tlv_NONE = 0,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_ACT_CALL_REQ_T_APN                            = 0x101,
    /* The data connection type for APN */
    /* type = uint32_t, refer to APN_TYPE */
    MIPC_DATA_ACT_CALL_REQ_T_APN_TYPE                       = 0x102,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_DATA_ACT_CALL_REQ_T_PDP_TYPE                       = 0x103,
    /* The IP address type for roaming */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_DATA_ACT_CALL_REQ_T_ROAMING_TYPE                   = 0x104,
    /* Authentication type for the data call */
    /* type = uint8_t, refer to APN_AUTH_TYPE */
    MIPC_DATA_ACT_CALL_REQ_T_AUTH_TYPE                      = 0x105,
    /* Authentication username */
    /* type = string */
    MIPC_DATA_ACT_CALL_REQ_T_USERID                         = 0x8106,
    /* Authentication password */
    /* type = string */
    MIPC_DATA_ACT_CALL_REQ_T_PASSWORD                       = 0x8107,
    /* Specify IPv4v6 fallback mode */
    /* type = uint8_t, refer to DATA_FALLBACK_TYPE */
    MIPC_DATA_ACT_CALL_REQ_T_IPV4V6_FALLBACK                = 0x108,
    /* Specifiy allowed bearer type. Default value is 0xFFFDFFFF */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_REQ_T_BEARER_BITMASK                 = 0x109,
    /* Specify whether to reuse PDN which is activated by modem */
    /* type = uint8_t, refer to DATA_REUSE_TYPE */
    MIPC_DATA_ACT_CALL_REQ_T_REUSE_FLAG                     = 0x10A,
    /* It indicates whether to bind PDN to RNDIS; 1: Bind to RNDIS; 0: Not bind to RNDIS, ONLY USED for datacard */
    /* type = uint8_t, refer to DATA_BIND_TYPE */
    MIPC_DATA_ACT_CALL_REQ_T_IF_TYPE                        = 0x10B,
    /* It is used to distinguish different data call session with same APN */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_REQ_T_APN_INDEX                      = 0x10C,
    /* to indicate URSP traffic description */
    /* type = struct, refer to ursp_traffic_desc_struct */
    MIPC_DATA_ACT_CALL_REQ_T_URSP_TRAFFIC_DESC              = 0x10D,
    /* to indicate URSP ue local configuration */
    /* type = struct, refer to ursp_ue_local_conf_struct */
    MIPC_DATA_ACT_CALL_REQ_T_URSP_UE_LOCAL_CONF             = 0x10E,
    /* URSP EVAL MATCH ALL DISALLOWED */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_REQ_T_URSP_EVAL_FLAG                 = 0x10F,
    /* interface id, range is 0~19 */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_REQ_T_IF_ID                          = 0x110,
    /* boolean value to indicate it is a HANDOVER or INITIAL request */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_DATA_ACT_CALL_REQ_T_IS_HANDOVER                    = 0x111,
    /* MBS(multicast and broadcast service) session information list */
    /* type = struct, refer to mbs_session_info */
    MIPC_DATA_ACT_CALL_REQ_T_MBS_SESSION_INFO_TLV_ARRAY     = 0x112,
    /* Information required to establish TSN PDU session. */
    /* type = struct, refer to data_tsn_info */
    MIPC_DATA_ACT_CALL_REQ_T_TSN_INFO                       = 0x113,
};

    /* MIPC_MSG.DATA_ACT_CALL_CNF */
enum mipc_data_act_call_cnf_tlv_enum {
    mipc_data_act_call_cnf_tlv_NONE = 0,
    /* Context identifier; it is used to recognize established data call */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_ID                             = 0x100,
    MIPC_DATA_ACT_CALL_CNF_T_CID                            = 0x100,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_ACT_CALL_CNF_T_APN                            = 0x101,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_DATA_ACT_CALL_CNF_T_PDP_TYPE                       = 0x102,
    /* IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_V4_COUNT                       = 0x103,
    /* First IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V4_0                           = 0x8104,
    /* Second IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V4_1                           = 0x8105,
    /* Third IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V4_2                           = 0x8106,
    /* Fourth IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V4_3                           = 0x8107,
    /* IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_V6_COUNT                       = 0x108,
    /* First IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V6_0                           = 0x8109,
    /* Second IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V6_1                           = 0x810A,
    /* Third IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V6_2                           = 0x810B,
    /* Fourth IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_V6_3                           = 0x810C,
    /* DNS IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_COUNT                   = 0x10D,
    /* First DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_0                       = 0x810E,
    /* Second DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_1                       = 0x810F,
    /* Third DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_2                       = 0x8110,
    /* Fourth DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V4_3                       = 0x8111,
    /* DNS IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_COUNT                   = 0x112,
    /* First DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_0                       = 0x8113,
    /* Second DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_1                       = 0x8114,
    /* Third DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_2                       = 0x8115,
    /* Fourth DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_V6_3                       = 0x8116,
    /* PCSCF IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_COUNT                 = 0x117,
    /* First PCSCF IPv4 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_0                     = 0x8118,
    /* Second PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_1                     = 0x8119,
    /* Third PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_2                     = 0x811A,
    /* Fourth PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V4_3                     = 0x811B,
    /* PCSCF IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_COUNT                 = 0x11C,
    /* First PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_0                     = 0x811D,
    /* Second PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_1                     = 0x811E,
    /* Third PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_2                     = 0x811F,
    /* Fourth PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_V6_3                     = 0x8120,
    /* Gateway IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_GW_V4                          = 0x8121,
    /* Gateway IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_GW_V6                          = 0x8122,
    /* IPv4 MTU */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_MTU_V4                         = 0x123,
    /* IPv6 MTU */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_MTU_V6                         = 0x124,
    /* Network interface ID */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_INTERFACE_ID                   = 0x125,
    /* Data call primary ID; If the value of this field is equal to data call ID, it means the data call uses default bearer, otherwise it uses specific bearer. */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_P_ID                           = 0x126,
    /* Fallback data call ID */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_FB_ID                          = 0x127,
    /* IPv4 network mask */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_IPv4_NETMASK                   = 0x128,
    /* IPv6 prefix */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_IPv6_NETMASK                   = 0x129,
    /* Network transmission ID */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_TRANS_ID                       = 0x12A,
    /* RAN info, Wi-Fi, cellular, or unknown */
    /* type = uint8_t, refer to RAN */
    MIPC_DATA_ACT_CALL_CNF_T_RAN_INFO                       = 0x12B,
    /* Network ESM/5GSM bearer ID */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_BEARER_ID                      = 0x12C,
    /* IM CN signalling flag from network; it indicates EPS bearer dedicated for IMS signalling. Refer to 3GPP 23.228 annex E.2.1.2. */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_CNF_T_IM_CN_SIGNALLING_FLAG          = 0x12D,
    /* ethernet MTU */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_CNF_T_MTU_ETHERNET                   = 0x12E,
    /* IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_IPV4_TLV_ARRAY                 = 0x812F,
    /* IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_IPV6_TLV_ARRAY                 = 0x8130,
    /* DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_IPV4_TLV_ARRAY             = 0x8131,
    /* DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_DNS_IPV6_TLV_ARRAY             = 0x8132,
    /* PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_IPV4_TLV_ARRAY           = 0x8133,
    /* PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_CNF_T_PCSCF_IPV6_TLV_ARRAY           = 0x8134,
    /* MBS session update indication information */
    /* type = struct, refer to mbs_session_update_ind */
    MIPC_DATA_ACT_CALL_CNF_T_MBS_SESSION_UPDATE_IND         = 0x135,
    /* MBS service area information, cell ID list */
    /* type = struct, refer to cell_global_id_struct */
    MIPC_DATA_ACT_CALL_CNF_T_CELL_ID_TLV_ARRAY              = 0x136,
    /* MBS service area information, tracking area code list */
    /* type = struct, refer to tac_struct */
    MIPC_DATA_ACT_CALL_CNF_T_TAC_TLV_ARRAY                  = 0x137,
};

    /* MIPC_MSG.DATA_DEACT_CALL_REQ */
enum mipc_data_deact_call_req_tlv_enum {
    mipc_data_deact_call_req_tlv_NONE = 0,
    /* The context identifier which gets from MIPC_DATA_ACT_CALL_CNF. */
    /* type = uint8_t */
    MIPC_DATA_DEACT_CALL_REQ_T_ID                           = 0x101,
    MIPC_DATA_DEACT_CALL_REQ_T_CID                          = 0x101,
    /* Deactivate reason */
    /* type = uint8_t, refer to DEACT_REASON_ENUM */
    MIPC_DATA_DEACT_CALL_REQ_T_DEACT_REASON                 = 0x102,
};

    /* MIPC_MSG.DATA_DEACT_CALL_CNF */
enum mipc_data_deact_call_cnf_tlv_enum {
    mipc_data_deact_call_cnf_tlv_NONE = 0,
    /* Deactivated data call ID */
    /* type = uint8_t */
    MIPC_DATA_DEACT_CALL_CNF_T_ID                           = 0x100,
    MIPC_DATA_DEACT_CALL_CNF_T_CID                          = 0x100,
};

    /* MIPC_MSG.DATA_GET_CALL_REQ */
enum mipc_data_get_call_req_tlv_enum {
    mipc_data_get_call_req_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_REQ_T_ID                             = 0x101,
    MIPC_DATA_GET_CALL_REQ_T_CID                            = 0x101,
};

    /* MIPC_MSG.DATA_GET_CALL_CNF */
enum mipc_data_get_call_cnf_tlv_enum {
    mipc_data_get_call_cnf_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_ID                             = 0x100,
    MIPC_DATA_GET_CALL_CNF_T_CID                            = 0x100,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_GET_CALL_CNF_T_APN                            = 0x101,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_DATA_GET_CALL_CNF_T_PDP_TYPE                       = 0x102,
    /* IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_V4_COUNT                       = 0x103,
    /* First IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_V4_0                           = 0x8104,
    /* Second IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_V4_1                           = 0x8105,
    /* Third IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_V4_2                           = 0x8106,
    /* Fourth IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_V4_3                           = 0x8107,
    /* IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_V6_COUNT                       = 0x108,
    /* First IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_V6_0                           = 0x8109,
    /* Second IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_V6_1                           = 0x810A,
    /* Third IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_V6_2                           = 0x810B,
    /* Fourth IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_V6_3                           = 0x810C,
    /* DNS IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V4_COUNT                   = 0x10D,
    /* First DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V4_0                       = 0x810E,
    /* Second DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V4_1                       = 0x810F,
    /* Third DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V4_2                       = 0x8110,
    /* Fourth DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V4_3                       = 0x8111,
    /* DNS IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V6_COUNT                   = 0x112,
    /* First DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V6_0                       = 0x8113,
    /* Second DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V6_1                       = 0x8114,
    /* Third DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V6_2                       = 0x8115,
    /* Fourth DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_V6_3                       = 0x8116,
    /* PCSCF IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_COUNT                 = 0x117,
    /* First PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_0                     = 0x8118,
    /* Second PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_1                     = 0x8119,
    /* Third PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_2                     = 0x811A,
    /* Fourth PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V4_3                     = 0x811B,
    /* PCSCF IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_COUNT                 = 0x11C,
    /* First PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_0                     = 0x811D,
    /* Second PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_1                     = 0x811E,
    /* Third PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_2                     = 0x811F,
    /* Fourth PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_V6_3                     = 0x8120,
    /* Gateway IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_GW_V4                          = 0x8121,
    /* Gateway IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_GW_V6                          = 0x8122,
    /* IPv4 MTU */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_MTU_V4                         = 0x123,
    /* IPv6 MTU */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_MTU_V6                         = 0x124,
    /* Network interface ID */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_INTERFACE_ID                   = 0x125,
    /*  IPv4 network mask */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_IPv4_NETMASK                   = 0x126,
    /*  IPv6 prefix */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_IPv6_NETMASK                   = 0x127,
    /* The data connection type for APN */
    /* type = uint32_t, refer to APN_TYPE */
    MIPC_DATA_GET_CALL_CNF_T_APN_TYPE                       = 0x128,
    /* Network transaction ID */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_TRANS_ID                       = 0x129,
    /* RAN info, Wi-Fi, cellular, or unknown */
    /* type = uint8_t, refer to RAN */
    MIPC_DATA_GET_CALL_CNF_T_RAN_INFO                       = 0x12A,
    /* Network ESM/5GSM bearer ID */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_BEARER_ID                      = 0x12B,
    /* IM CN signalling flag from network; it indicates EPS bearer dedicated for IMS signalling. Refer to 3GPP 23.228 annex E.2.1.2. */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_IM_CN_SIGNALLING_FLAG          = 0x12C,
    /* QoS Flow Identifier; refer to 3GPP 24.501. */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_CNF_T_QFI                            = 0x12D,
    /* ethernet MTU */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_CNF_T_MTU_ETHERNET                   = 0x12E,
    /* IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_IPV4_TLV_ARRAY                 = 0x812F,
    /* IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_IPV6_TLV_ARRAY                 = 0x8130,
    /* DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_IPV4_TLV_ARRAY             = 0x8131,
    /* DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_DNS_IPV6_TLV_ARRAY             = 0x8132,
    /* PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_IPV4_TLV_ARRAY           = 0x8133,
    /* PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_GET_CALL_CNF_T_PCSCF_IPV6_TLV_ARRAY           = 0x8134,
};

    /* MIPC_MSG.DATA_SET_PACKET_FILTER_REQ */
enum mipc_data_set_packet_filter_req_tlv_enum {
    mipc_data_set_packet_filter_req_tlv_NONE = 0,
    /* Data call ID (indicating the data call applied) */
    /* type = uint8_t */
    MIPC_DATA_SET_PACKET_FILTER_REQ_T_ID                    = 0x101,
    MIPC_DATA_SET_PACKET_FILTER_REQ_T_CID                   = 0x101,
    /* Filter count */
    /* type = uint8_t */
    MIPC_DATA_SET_PACKET_FILTER_REQ_T_FILTER_COUNT          = 0x102,
    /* Filter content */
    /* type = struct_array, refer to data_packet_filter */
    MIPC_DATA_SET_PACKET_FILTER_REQ_T_FILTER_LIST           = 0x103,
    /* Filter content TLV array */
    /* type = struct, refer to data_packet_filter */
    MIPC_DATA_SET_PACKET_FILTER_REQ_T_FILTER_TLV_ARRAY      = 0x104,
};

    /* MIPC_MSG.DATA_SET_PACKET_FILTER_CNF */
enum mipc_data_set_packet_filter_cnf_tlv_enum {
    mipc_data_set_packet_filter_cnf_tlv_NONE = 0,
    /* Data call ID */
    /* type = uint8_t */
    MIPC_DATA_SET_PACKET_FILTER_CNF_T_ID                    = 0x100,
    MIPC_DATA_SET_PACKET_FILTER_CNF_T_CID                   = 0x100,
    /* Filter count */
    /* type = uint8_t */
    MIPC_DATA_SET_PACKET_FILTER_CNF_T_FILTER_COUNT          = 0x101,
    /* Filter content */
    /* type = struct_array, refer to data_packet_filter */
    MIPC_DATA_SET_PACKET_FILTER_CNF_T_FILTER_LIST           = 0x102,
    /* Filter content TLV array */
    /* type = struct, refer to data_packet_filter */
    MIPC_DATA_SET_PACKET_FILTER_CNF_T_FILTER_TLV_ARRAY      = 0x103,
};

    /* MIPC_MSG.DATA_GET_PACKET_FILTER_REQ */
enum mipc_data_get_packet_filter_req_tlv_enum {
    mipc_data_get_packet_filter_req_tlv_NONE = 0,
    /* Data call ID */
    /* type = uint8_t */
    MIPC_DATA_GET_PACKET_FILTER_REQ_T_ID                    = 0x101,
    MIPC_DATA_GET_PACKET_FILTER_REQ_T_CID                   = 0x101,
};

    /* MIPC_MSG.DATA_GET_PACKET_FILTER_CNF */
enum mipc_data_get_packet_filter_cnf_tlv_enum {
    mipc_data_get_packet_filter_cnf_tlv_NONE = 0,
    /* Data call ID */
    /* type = uint8_t */
    MIPC_DATA_GET_PACKET_FILTER_CNF_T_ID                    = 0x100,
    MIPC_DATA_GET_PACKET_FILTER_CNF_T_CID                   = 0x100,
    /* Filter count */
    /* type = uint8_t */
    MIPC_DATA_GET_PACKET_FILTER_CNF_T_FILTER_COUNT          = 0x101,
    /* Filter content */
    /* type = struct_array, refer to data_packet_filter */
    MIPC_DATA_GET_PACKET_FILTER_CNF_T_FILTER_LIST           = 0x102,
    /* Filter content TLV array */
    /* type = struct, refer to data_packet_filter */
    MIPC_DATA_GET_PACKET_FILTER_CNF_T_FILTER_TLV_ARRAY      = 0x103,
};

    /* MIPC_MSG.DATA_GET_PCO_REQ */
enum mipc_data_get_pco_req_tlv_enum {
    mipc_data_get_pco_req_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_GET_PCO_REQ_T_ID                              = 0x101,
    MIPC_DATA_GET_PCO_REQ_T_CID                             = 0x101,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_GET_PCO_REQ_T_APN                             = 0x102,
    /* It is used to distinguish different data call session with same APN. */
    /* type = uint32_t */
    MIPC_DATA_GET_PCO_REQ_T_APN_INDEX                       = 0x103,
    /* It is adopted to query the content of protocol configuration options; refer to 3GPP 24.008 clause 10.5.6.3. */
    /* type = string */
    MIPC_DATA_GET_PCO_REQ_T_PCO_IE                          = 0x104,
};

    /* MIPC_MSG.DATA_GET_PCO_CNF */
enum mipc_data_get_pco_cnf_tlv_enum {
    mipc_data_get_pco_cnf_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_GET_PCO_CNF_T_ID                              = 0x100,
    MIPC_DATA_GET_PCO_CNF_T_CID                             = 0x100,
    /* PCO count */
    /* type = uint8_t */
    MIPC_DATA_GET_PCO_CNF_T_PCO_COUNT                       = 0x101,
    /* The protocol configuration options contents */
    /* type = struct, refer to data_pco_ie */
    MIPC_DATA_GET_PCO_CNF_T_PCO_LIST                        = 0x8102,
    /* The protocol configuration options contents */
    /* type = struct, refer to data_pco_ie */
    MIPC_DATA_GET_PCO_CNF_T_PCO_TLV_ARRAY                   = 0x8103,
};

    /* MIPC_MSG.DATA_SET_DATA_ALLOW_REQ */
enum mipc_data_set_data_allow_req_tlv_enum {
    mipc_data_set_data_allow_req_tlv_NONE = 0,
    /* It indicates whether data are clear or not, if this TLV is not NULL, data allowed to be configured will be cleared */
    /* type = uint8_t */
    MIPC_DATA_SET_DATA_ALLOW_REQ_T_CLEAR                    = 0x100,
};

    /* MIPC_MSG.DATA_SET_DATA_ALLOW_CNF */
enum mipc_data_set_data_allow_cnf_tlv_enum {
    mipc_data_set_data_allow_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_MD_DATA_CALL_LIST_REQ */
enum mipc_data_get_md_data_call_list_req_tlv_enum {
    mipc_data_get_md_data_call_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_MD_DATA_CALL_LIST_CNF */
enum mipc_data_get_md_data_call_list_cnf_tlv_enum {
    mipc_data_get_md_data_call_list_cnf_tlv_NONE = 0,
    /* The list of activated data call state. Array index is CID, array value is state. */
    /* type = byte_array, refer to MIPC_CID_ACT_STATE */
    MIPC_DATA_GET_MD_DATA_CALL_LIST_CNF_T_CID_LIST          = 0x100,
};

    /* MIPC_MSG.DATA_SET_CONFIG_REQ */
enum mipc_data_set_config_req_tlv_enum {
    mipc_data_set_config_req_tlv_NONE = 0,
    /* Mobile data configuration: enable, disable, or no change; Default value is 0x7f */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_SET_CONFIG_REQ_T_MOBILE_DATA                  = 0x100,
    /* Data roaming configuration: enable, disable, or no change; Default value is 0x7f */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_SET_CONFIG_REQ_T_DATA_ROAMING                 = 0x101,
    /* VOLTE configuration: enable, disable, or no change; Default value is 0x7f */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_SET_CONFIG_REQ_T_VOLTE                        = 0x102,
    /* IMS test mode configuration: enable, disable, or no change; Default value is 0x7f */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_SET_CONFIG_REQ_T_IMS_TEST_MODE                = 0x103,
    /* Data domestic roaming configuration: enable, disable, or no change; Default value is 0x7f */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_SET_CONFIG_REQ_T_DATA_DOMESTIC_ROAMING        = 0x104,
    /* Data international roaming configuration: enable, disable, or no change; Default value is 0x7f */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_SET_CONFIG_REQ_T_DATA_INTERNATIONAL_ROAMING   = 0x105,
    /* Default SIM card; Default value is 0x7f */
    /* type = uint8_t */
    MIPC_DATA_SET_CONFIG_REQ_T_DEFAULT_DATA_SIM_CARD        = 0x106,
};

    /* MIPC_MSG.DATA_SET_CONFIG_CNF */
enum mipc_data_set_config_cnf_tlv_enum {
    mipc_data_set_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_CONFIG_REQ */
enum mipc_data_get_config_req_tlv_enum {
    mipc_data_get_config_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_CONFIG_CNF */
enum mipc_data_get_config_cnf_tlv_enum {
    mipc_data_get_config_cnf_tlv_NONE = 0,
    /* Mobile data: enable or disable */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_GET_CONFIG_CNF_T_MOBILE_DATA                  = 0x100,
    /* Data roaming configuration: enable or disable */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_GET_CONFIG_CNF_T_DATA_ROAMING                 = 0x101,
    /* VOLTE configuration: enable, disable, or no change */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_GET_CONFIG_CNF_T_VOLTE                        = 0x102,
    /* IMS test mode configuration: enable or disable */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_GET_CONFIG_CNF_T_IMS_TEST_MODE                = 0x103,
    /* Data domestic roaming configuration: enable, disable, or no change */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_GET_CONFIG_CNF_T_DATA_DOMESTIC_ROAMING        = 0x104,
    /* Data international roaming configuration: enable, disable, or no change */
    /* type = uint8_t, refer to DATA_CONFIG_TYPE */
    MIPC_DATA_GET_CONFIG_CNF_T_DATA_INTERNATIONAL_ROAMING   = 0x105,
};

    /* MIPC_MSG.DATA_ABORT_CALL_REQ */
enum mipc_data_abort_call_req_tlv_enum {
    mipc_data_abort_call_req_tlv_NONE = 0,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_ABORT_CALL_REQ_T_APN                          = 0x100,
};

    /* MIPC_MSG.DATA_ABORT_CALL_CNF */
enum mipc_data_abort_call_cnf_tlv_enum {
    mipc_data_abort_call_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_CALL_INFO_REQ */
enum mipc_data_get_call_info_req_tlv_enum {
    mipc_data_get_call_info_req_tlv_NONE = 0,
    /* APN name */
    /* type = string */
    MIPC_DATA_GET_CALL_INFO_REQ_T_APN                       = 0x100,
    /* Data call ID */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_INFO_REQ_T_ID                        = 0x101,
    MIPC_DATA_GET_CALL_INFO_REQ_T_CID                       = 0x101,
};

    /* MIPC_MSG.DATA_GET_CALL_INFO_CNF */
enum mipc_data_get_call_info_cnf_tlv_enum {
    mipc_data_get_call_info_cnf_tlv_NONE = 0,
    /* APN name */
    /* type = string */
    MIPC_DATA_GET_CALL_INFO_CNF_T_APN                       = 0x100,
    /* Data call ID */
    /* type = uint8_t */
    MIPC_DATA_GET_CALL_INFO_CNF_T_ID                        = 0x101,
    MIPC_DATA_GET_CALL_INFO_CNF_T_CID                       = 0x101,
    /* The UTC time of call established */
    /* type = struct, refer to data_nitz_info */
    MIPC_DATA_GET_CALL_INFO_CNF_T_ESTBLISHED_TIME           = 0x102,
    /* The UTC time of call disconnected */
    /* type = struct, refer to data_nitz_info */
    MIPC_DATA_GET_CALL_INFO_CNF_T_END_TIME                  = 0x103,
    /* If PDN is rejected, it indicates that the cause is rejected from netowrk; 0: success */
    /* type = uint32_t */
    MIPC_DATA_GET_CALL_INFO_CNF_T_REJECT_CAUSE              = 0x104,
};

    /* MIPC_MSG.DATA_GET_PDP_CID_REQ */
enum mipc_data_get_pdp_cid_req_tlv_enum {
    mipc_data_get_pdp_cid_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_PDP_CID_CNF */
enum mipc_data_get_pdp_cid_cnf_tlv_enum {
    mipc_data_get_pdp_cid_cnf_tlv_NONE = 0,
    /* The minimum value of PDP CID range */
    /* type = uint8_t */
    MIPC_DATA_GET_PDP_CID_CNF_T_MIN_CID                     = 0x100,
    /* The maximum value of PDP CID range */
    /* type = uint8_t */
    MIPC_DATA_GET_PDP_CID_CNF_T_MAX_CID                     = 0x101,
};

    /* MIPC_MSG.DATA_RETRY_TIMER_REQ */
enum mipc_data_retry_timer_req_tlv_enum {
    mipc_data_retry_timer_req_tlv_NONE = 0,
    /*
      0: Query retry time suggested by modem
      1: Reset data retry count
      2: Retry after a given time period
    */
    /* type = uint8_t, refer to DATA_RETRY_MODE */
    MIPC_DATA_RETRY_TIMER_REQ_T_MODE                        = 0x100,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_RETRY_TIMER_REQ_T_APN_NAME                    = 0x101,
};

    /* MIPC_MSG.DATA_RETRY_TIMER_CNF */
enum mipc_data_retry_timer_cnf_tlv_enum {
    mipc_data_retry_timer_cnf_tlv_NONE = 0,
    /* Retry type suggested by modem */
    /* type = uint8_t, refer to DATA_RETRY_TYPE */
    MIPC_DATA_RETRY_TIMER_CNF_T_RETRY_TYPE                  = 0x100,
    /* The retry time suggested by modem; the unit is second */
    /* type = uint32_t */
    MIPC_DATA_RETRY_TIMER_CNF_T_RETRY_TIME                  = 0x101,
};

    /* MIPC_MSG.DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ */
enum mipc_data_set_link_capacity_reporting_criteria_req_tlv_enum {
    mipc_data_set_link_capacity_reporting_criteria_req_tlv_NONE = 0,
    /*
      0: Disable
      1: Enable
      2: Set LCE reporting criteria
    */
    /* type = uint8_t, refer to DATA_LCE_MODE */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_MODE = 0x100,
    /* Minimum time interval between two URC reports; Default value is 0 */
    /* type = uint32_t */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_HYSTERESIS_MS = 0x101,
    /* Minimum DL throughput change between two URC reports; Default value is 0 */
    /* type = uint32_t */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_HYSTERESIS_DL_KBPS = 0x102,
    /* Minimum UL throughput change between two URC reports; Default value is 0 */
    /* type = uint32_t */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_HYSTERESIS_UL_KBPS = 0x103,
    /* Number of threshold DL Kbps List; Default value is 0 */
    /* type = uint8_t */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_DL_KBPS_NUM = 0x104,
    /* Threshold DL Kbps List */
    /* type = byte_array */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_DL_KBPS_LIST = 0x105,
    /* Number of threshold UL Kbps List; Default value is 0 */
    /* type = uint8_t */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_UL_KBPS_NUM = 0x106,
    /* Threshold UL Kbps List */
    /* type = byte_array */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_THRESHOLD_UL_KBPS_LIST = 0x107,
    /* It indicates the RAT to which the Link capacity reporting criteria is applicable; Default value is 0 */
    /* type = uint32_t */
    MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ_T_ACCESS_NETWORK = 0x108,
};

    /* MIPC_MSG.DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_CNF */
enum mipc_data_set_link_capacity_reporting_criteria_cnf_tlv_enum {
    mipc_data_set_link_capacity_reporting_criteria_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_DEDICATE_BEARER_INFO_REQ */
enum mipc_data_get_dedicate_bearer_info_req_tlv_enum {
    mipc_data_get_dedicate_bearer_info_req_tlv_NONE = 0,
    /* Context identifier; Default value is 0xff */
    /* type = uint8_t */
    MIPC_DATA_GET_DEDICATE_BEARER_INFO_REQ_T_CID            = 0x100,
};

    /* MIPC_MSG.DATA_GET_DEDICATE_BEARER_INFO_CNF */
enum mipc_data_get_dedicate_bearer_info_cnf_tlv_enum {
    mipc_data_get_dedicate_bearer_info_cnf_tlv_NONE = 0,
    /* PDP contexts */
    /* type = struct, refer to data_secondary_pdp_context_info */
    MIPC_DATA_GET_DEDICATE_BEARER_INFO_CNF_T_CONTEXT_LIST   = 0x100,
    MIPC_DATA_GET_DEDICATE_BEARER_INFO_CNF_T_CONTEXT_TLV_ARRAY = 0x100,
};

    /* MIPC_MSG.DATA_GET_QOS_REQ */
enum mipc_data_get_qos_req_tlv_enum {
    mipc_data_get_qos_req_tlv_NONE = 0,
    /* Context identifier; Default value is 0xff */
    /* type = uint8_t */
    MIPC_DATA_GET_QOS_REQ_T_CID                             = 0x100,
};

    /* MIPC_MSG.DATA_GET_QOS_CNF */
enum mipc_data_get_qos_cnf_tlv_enum {
    mipc_data_get_qos_cnf_tlv_NONE = 0,
    /* QoS information */
    /* type = struct, refer to data_qos_info */
    MIPC_DATA_GET_QOS_CNF_T_QOS_LIST                        = 0x100,
    MIPC_DATA_GET_QOS_CNF_T_QOS_TLV_ARRAY                   = 0x100,
};

    /* MIPC_MSG.DATA_GET_TFT_REQ */
enum mipc_data_get_tft_req_tlv_enum {
    mipc_data_get_tft_req_tlv_NONE = 0,
    /* Context identifier; Default value is 0xff */
    /* type = uint8_t */
    MIPC_DATA_GET_TFT_REQ_T_CID                             = 0x100,
};

    /* MIPC_MSG.DATA_GET_TFT_CNF */
enum mipc_data_get_tft_cnf_tlv_enum {
    mipc_data_get_tft_cnf_tlv_NONE = 0,
    /* [Phase out] Please use TFT_V1_TLV_ARRAY. */
    /* type = struct, refer to data_tft_info */
    MIPC_DATA_GET_TFT_CNF_T_TFT_LIST                        = 0x100,
    MIPC_DATA_GET_TFT_CNF_T_TFT_TLV_ARRAY                   = 0x100,
    /* The list of Traffic Flow Template information. */
    /* type = struct, refer to data_tft_info_v1 */
    MIPC_DATA_GET_TFT_CNF_T_TFT_V1_TLV_ARRAY                = 0x101,
};

    /* MIPC_MSG.DATA_SET_LGDCONT_REQ */
enum mipc_data_set_lgdcont_req_tlv_enum {
    mipc_data_set_lgdcont_req_tlv_NONE = 0,
    /* APN name */
    /* type = string */
    MIPC_DATA_SET_LGDCONT_REQ_T_APN                         = 0x100,
    /* apn index */
    /* type = uint32_t */
    MIPC_DATA_SET_LGDCONT_REQ_T_APN_INDEX                   = 0x101,
    /* indicates the type of PDP context activation request for the PDP context */
    /* type = uint8_t, refer to DATA_LGDCONT_REQ_TYPE */
    MIPC_DATA_SET_LGDCONT_REQ_T_REQUEST_TYPE                = 0x102,
    /* the target RAT of this PDN connection; Default value is All_RAT */
    /* type = uint8_t, refer to DATA_LGDCONT_RAT_TYPE */
    MIPC_DATA_SET_LGDCONT_REQ_T_RAT_TYPE                    = 0x103,
};

    /* MIPC_MSG.DATA_SET_LGDCONT_CNF */
enum mipc_data_set_lgdcont_cnf_tlv_enum {
    mipc_data_set_lgdcont_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_SET_NSSAI_REQ */
enum mipc_data_set_nssai_req_tlv_enum {
    mipc_data_set_nssai_req_tlv_NONE = 0,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_SET_NSSAI_REQ_T_APN                           = 0x100,
    /* It is used to distinguish different data call session with same APN */
    /* type = uint32_t */
    MIPC_DATA_SET_NSSAI_REQ_T_APN_INDEX                     = 0x101,
    /* S-NSSAI value string; refer to 3GPP 24.501 subclause 9.11.2.8. */
    /* type = string */
    MIPC_DATA_SET_NSSAI_REQ_T_SNSSAI                        = 0x102,
};

    /* MIPC_MSG.DATA_SET_NSSAI_CNF */
enum mipc_data_set_nssai_cnf_tlv_enum {
    mipc_data_set_nssai_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_KEEPALIVE_REQ */
enum mipc_data_keepalive_req_tlv_enum {
    mipc_data_keepalive_req_tlv_NONE = 0,
    /* Session management for stopping Keep Alive */
    /* type = uint32_t */
    MIPC_DATA_KEEPALIVE_REQ_T_STOP_KEEPALIVE                = 0x100,
    /* Info for starting Keep Alive */
    /* type = struct, refer to data_start_keepalive_request */
    MIPC_DATA_KEEPALIVE_REQ_T_START_KEEPALIVE               = 0x101,
};

    /* MIPC_MSG.DATA_KEEPALIVE_CNF */
enum mipc_data_keepalive_cnf_tlv_enum {
    mipc_data_keepalive_cnf_tlv_NONE = 0,
    /* Connection ID */
    /* type = uint32_t */
    MIPC_DATA_KEEPALIVE_CNF_T_SESSION_HANDLE                = 0x100,
    /* Keep Alive status code */
    /* type = uint8_t, refer to DATA_KEEPALIVE_STATUS_CODE */
    MIPC_DATA_KEEPALIVE_CNF_T_STATUS_CODE                   = 0x101,
};

    /* MIPC_MSG.DATA_GET_DSDA_STATE_REQ */
enum mipc_data_get_dsda_state_req_tlv_enum {
    mipc_data_get_dsda_state_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_DSDA_STATE_CNF */
enum mipc_data_get_dsda_state_cnf_tlv_enum {
    mipc_data_get_dsda_state_cnf_tlv_NONE = 0,
    /* type = uint8_t, refer to DSDA_ALLOWED_TYPE */
    MIPC_DATA_GET_DSDA_STATE_CNF_T_DSDA_ALLOWED             = 0x100,
    /* type = uint8_t, refer to DSDA_STATE_TYPE */
    MIPC_DATA_GET_DSDA_STATE_CNF_T_DSDA_STATE               = 0x101,
    /* type = uint8_t, refer to DR_DSDA_TYPE */
    MIPC_DATA_GET_DSDA_STATE_CNF_T_IS_DR_DSDA               = 0x102,
    /* type = uint8_t, refer to DR_DSDS_TYPE */
    MIPC_DATA_GET_DSDA_STATE_CNF_T_IS_DR_DSDS               = 0x103,
};

    /* MIPC_MSG.DATA_GET_5GQOS_REQ */
enum mipc_data_get_5gqos_req_tlv_enum {
    mipc_data_get_5gqos_req_tlv_NONE = 0,
    /* Context identifier; Default value is 0xff */
    /* type = uint8_t */
    MIPC_DATA_GET_5GQOS_REQ_T_CID                           = 0x100,
};

    /* MIPC_MSG.DATA_GET_5GQOS_CNF */
enum mipc_data_get_5gqos_cnf_tlv_enum {
    mipc_data_get_5gqos_cnf_tlv_NONE = 0,
    /* [Phase out] Please use QOS_V2_TLV_ARRAY. */
    /* type = struct, refer to data_5gqos_info */
    MIPC_DATA_GET_5GQOS_CNF_T_QOS_LIST                      = 0x100,
    MIPC_DATA_GET_5GQOS_CNF_T_QOS_TLV_ARRAY                 = 0x100,
    /* 5G QoS information, refer to 3GPP 27.007 clause 10.1.50. */
    /* type = struct, refer to data_5gqos_info_v1 */
    MIPC_DATA_GET_5GQOS_CNF_T_QOS_V1_TLV_ARRAY              = 0x101,
};

    /* MIPC_MSG.DATA_SET_PSI_REQ */
enum mipc_data_set_psi_req_tlv_enum {
    mipc_data_set_psi_req_tlv_NONE = 0,
    /* PDU session ID handle type */
    /* type = uint8_t, refer to PSI_ACTION_ENUM */
    MIPC_DATA_SET_PSI_REQ_T_ACTION                          = 0x100,
    /* PDU session ID */
    /* type = uint8_t */
    MIPC_DATA_SET_PSI_REQ_T_PSI                             = 0x101,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_SET_PSI_REQ_T_APN                             = 0x102,
    /* It is used to distinguish different data call session with same APN */
    /* type = uint32_t */
    MIPC_DATA_SET_PSI_REQ_T_APN_INDEX                       = 0x103,
};

    /* MIPC_MSG.DATA_SET_PSI_CNF */
enum mipc_data_set_psi_cnf_tlv_enum {
    mipc_data_set_psi_cnf_tlv_NONE = 0,
    /* PDU session ID */
    /* type = uint8_t */
    MIPC_DATA_SET_PSI_CNF_T_PSI                             = 0x100,
};

    /* MIPC_MSG.DATA_GET_NSSAI_REQ */
enum mipc_data_get_nssai_req_tlv_enum {
    mipc_data_get_nssai_req_tlv_NONE = 0,
    /* It indicates NSSAI type to be acquired */
    /* type = uint8_t, refer to NSSAI_TYPE_ENUM */
    MIPC_DATA_GET_NSSAI_REQ_T_NSSAI_TYPE                    = 0x100,
    /* PLMN of operator network */
    /* type = byte_array */
    MIPC_DATA_GET_NSSAI_REQ_T_PLMN_ID                       = 0x101,
};

    /* MIPC_MSG.DATA_GET_NSSAI_CNF */
enum mipc_data_get_nssai_cnf_tlv_enum {
    mipc_data_get_nssai_cnf_tlv_NONE = 0,
    /* It indicates the number of default configured S-NSSAI in default configured NSSAI list */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_NUM  = 0x100,
    /* Default configured NSSAI list */
    /* type = struct, refer to s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_LIST = 0x101,
    MIPC_DATA_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_TLV_ARRAY = 0x101,
    /* It indicate the number of rejected S-NSSAI in rejected NSSAI list for 3GPP */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_NUM       = 0x102,
    /* Rejected NSSAI list for 3GPP */
    /* type = struct, refer to rejected_s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_LIST      = 0x103,
    MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_TLV_ARRAY = 0x103,
    /* It indicates the number of rejected S-NSSAI in rejected NSSAI list for non-3GPP */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_NUM    = 0x104,
    /* Rejected NSSAI list for non-3GPP */
    /* type = struct, refer to rejected_s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_LIST   = 0x105,
    MIPC_DATA_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_TLV_ARRAY = 0x105,
    /* It indicates the number of configured S-NSSAI in configured NSSAI list */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_NUM          = 0x106,
    /* Configured NSSAI list */
    /* type = struct, refer to plmn_specific_s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_LIST         = 0x107,
    MIPC_DATA_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_TLV_ARRAY    = 0x107,
    /* It indicates the number of allowed S-NSSAI in allowed NSSAI list for 3GPP */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_NUM        = 0x108,
    /* Allowed NSSAI list for 3GPP */
    /* type = struct, refer to plmn_specific_s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_LIST       = 0x109,
    MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_TLV_ARRAY  = 0x109,
    /* It indicates the number of allowed S-NSSAI in allowed NSSAI list for non-3GPP */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_NUM     = 0x10A,
    /* Allowed NSSAI list for non-3GPP */
    /* type = struct, refer to plmn_specific_s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_LIST    = 0x10B,
    MIPC_DATA_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_TLV_ARRAY = 0x10B,
    /* It indicates the number of preferrd S-NSSAI in preferrd NSSAI list for 3GPP */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_NUM      = 0x10C,
    /* Preferred NSSAI list for 3GPP */
    /* type = struct, refer to s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_LIST     = 0x10D,
    MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_TLV_ARRAY = 0x10D,
    /* It indicates the number of preferrd S-NSSAI in preferrd NSSAI list for non-3GPP */
    /* type = uint8_t */
    MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_NUM   = 0x10E,
    /* Preferred NSSAI list for non-3GPP */
    /* type = struct, refer to s_nssai_struct */
    MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_LIST  = 0x10F,
    MIPC_DATA_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_TLV_ARRAY = 0x10F,
};

    /* MIPC_MSG.DATA_GET_URSP_ROUTE_PROFILE_REQ */
enum mipc_data_get_ursp_route_profile_req_tlv_enum {
    mipc_data_get_ursp_route_profile_req_tlv_NONE = 0,
    /* data call id */
    /* type = uint32_t */
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_REQ_T_ID               = 0x100,
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_REQ_T_CID              = 0x100,
};

    /* MIPC_MSG.DATA_GET_URSP_ROUTE_PROFILE_CNF */
enum mipc_data_get_ursp_route_profile_cnf_tlv_enum {
    mipc_data_get_ursp_route_profile_cnf_tlv_NONE = 0,
    /* data call id */
    /* type = uint32_t */
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ID               = 0x100,
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_CID              = 0x100,
    /* the establishment request info send to  */
    /* type = struct, refer to ursp_ue_local_conf_struct */
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_EST_REQ_PARAM    = 0x101,
    /* establish attribute */
    /* type = uint32_t */
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ATTR             = 0x102,
    /* route supp profile list count */
    /* type = uint32_t */
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ROUTE_SUPP_PROFILE_LIST_NUM = 0x103,
    /* route supp profile list, struct array */
    /* type = struct_array, refer to ursp_get_route_supp_profile_ind_struct */
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ROUTE_SUPP_PROFILE_LIST = 0x104,
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF_T_ROUTE_SUPP_PROFILE_TLV_ARRAY = 0x104,
};

    /* MIPC_MSG.DATA_SET_URSP_PRECONF_UE_POLICY_REQ */
enum mipc_data_set_ursp_preconf_ue_policy_req_tlv_enum {
    mipc_data_set_ursp_preconf_ue_policy_req_tlv_NONE = 0,
    /* plmn id, mcc1, mcc2, mcc3, mnc1, mnc2, mnc3 */
    /* type = byte_array */
    MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ_T_PLMN_ID      = 0x100,
    /* the number of rule in rule list */
    /* type = uint8_t */
    MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ_T_RULE_NUM     = 0x101,
    /* rule list */
    /* type = struct, refer to ursp_rule_struct */
    MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ_T_RULE_LIST    = 0x102,
    MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ_T_RULE_TLV_ARRAY = 0x102,
};

    /* MIPC_MSG.DATA_SET_URSP_PRECONF_UE_POLICY_CNF */
enum mipc_data_set_ursp_preconf_ue_policy_cnf_tlv_enum {
    mipc_data_set_ursp_preconf_ue_policy_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_URSP_UE_POLICY_REQ */
enum mipc_data_get_ursp_ue_policy_req_tlv_enum {
    mipc_data_get_ursp_ue_policy_req_tlv_NONE = 0,
    /* plmn id, mcc1, mcc2, mcc3, mnc1, mnc2, mnc3 */
    /* type = byte_array */
    MIPC_DATA_GET_URSP_UE_POLICY_REQ_T_PLMN_ID              = 0x100,
};

    /* MIPC_MSG.DATA_GET_URSP_UE_POLICY_CNF */
enum mipc_data_get_ursp_ue_policy_cnf_tlv_enum {
    mipc_data_get_ursp_ue_policy_cnf_tlv_NONE = 0,
    /* plmn id, mcc1, mcc2, mcc3, mnc1, mnc2, mnc3 */
    /* type = byte_array */
    MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_PLMN_ID              = 0x100,
    /* the number of rule in rule list */
    /* type = uint8_t */
    MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RULE_NUM             = 0x101,
    /* rule list */
    /* type = struct, refer to ursp_rule_struct */
    MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RULE_LIST            = 0x102,
    MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RULE_TLV_ARRAY       = 0x102,
    /* The UE policy raw data */
    /* type = byte_array */
    MIPC_DATA_GET_URSP_UE_POLICY_CNF_T_RAW_DATA             = 0x103,
};

    /* MIPC_MSG.DATA_SET_RESERVED_IF_ID_REQ */
enum mipc_data_set_reserved_if_id_req_tlv_enum {
    mipc_data_set_reserved_if_id_req_tlv_NONE = 0,
    /* bool array, length is 20. if_id(range 0~19) is array index. Set value to 1 for reseved if_id index, otherwise set to 0 */
    /* type = byte_array, refer to BOOLEAN */
    MIPC_DATA_SET_RESERVED_IF_ID_REQ_T_IF_ID_LIST           = 0x100,
};

    /* MIPC_MSG.DATA_SET_RESERVED_IF_ID_CNF */
enum mipc_data_set_reserved_if_id_cnf_tlv_enum {
    mipc_data_set_reserved_if_id_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_SET_PCO_REQ */
enum mipc_data_set_pco_req_tlv_enum {
    mipc_data_set_pco_req_tlv_NONE = 0,
    /* apn name */
    /* type = string */
    MIPC_DATA_SET_PCO_REQ_T_APN                             = 0x100,
    /* apn index; Default value is 0xffffffff */
    /* type = uint32_t */
    MIPC_DATA_SET_PCO_REQ_T_APN_INDEX                       = 0x101,
    /* pco info list, it is a tlv array */
    /* type = struct, refer to data_pco_ie */
    MIPC_DATA_SET_PCO_REQ_T_PCO_LIST                        = 0x8102,
    MIPC_DATA_SET_PCO_REQ_T_PCO_TLV_ARRAY                   = 0x8102,
};

    /* MIPC_MSG.DATA_SET_PCO_CNF */
enum mipc_data_set_pco_cnf_tlv_enum {
    mipc_data_set_pco_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_SET_PAGING_REQ */
enum mipc_data_set_paging_req_tlv_enum {
    mipc_data_set_paging_req_tlv_NONE = 0,
    /* Indicates the paging restriction preference of the UE */
    /* type = uint8_t, refer to DATA_PAGING_RESTRICTIONS */
    MIPC_DATA_SET_PAGING_REQ_T_PAGING_RESTRICTIONS          = 0x100,
    /* Specifies a particular EPS bearer context or a 5GS QoS flow */
    /* type = uint8_t */
    MIPC_DATA_SET_PAGING_REQ_T_CID                          = 0x101,
};

    /* MIPC_MSG.DATA_SET_PAGING_CNF */
enum mipc_data_set_paging_cnf_tlv_enum {
    mipc_data_set_paging_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_PAGING_REQ */
enum mipc_data_get_paging_req_tlv_enum {
    mipc_data_get_paging_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_PAGING_CNF */
enum mipc_data_get_paging_cnf_tlv_enum {
    mipc_data_get_paging_cnf_tlv_NONE = 0,
    /* Indicates the paging restriction preference of the UE */
    /* type = uint8_t, refer to DATA_PAGING_RESTRICTIONS */
    MIPC_DATA_GET_PAGING_CNF_T_PAGING_RESTRICTIONS          = 0x100,
    /* Specifies a particular EPS bearer context or a 5GS QoS flow */
    /* type = uint8_t */
    MIPC_DATA_GET_PAGING_CNF_T_CID                          = 0x101,
};

    /* MIPC_MSG.DATA_MOD_CALL_REQ */
enum mipc_data_mod_call_req_tlv_enum {
    mipc_data_mod_call_req_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_REQ_T_CID                            = 0x100,
    /* MBS(multicast and broadcast service) session information list */
    /* type = struct, refer to mbs_session_info */
    MIPC_DATA_MOD_CALL_REQ_T_MBS_SESSION_INFO_TLV_ARRAY     = 0x101,
};

    /* MIPC_MSG.DATA_MOD_CALL_CNF */
enum mipc_data_mod_call_cnf_tlv_enum {
    mipc_data_mod_call_cnf_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_CNF_T_CID                            = 0x100,
    /* MBS session update indication information */
    /* type = struct, refer to mbs_session_update_ind */
    MIPC_DATA_MOD_CALL_CNF_T_MBS_SESSION_UPDATE_IND         = 0x101,
    /* MBS service area information, cell ID list. */
    /* type = struct, refer to cell_global_id_struct */
    MIPC_DATA_MOD_CALL_CNF_T_CELL_ID_TLV_ARRAY              = 0x102,
    /* MBS service area information, tracking area code list. */
    /* type = struct, refer to tac_struct */
    MIPC_DATA_MOD_CALL_CNF_T_TAC_TLV_ARRAY                  = 0x103,
};

    /* MIPC_MSG.DATA_SET_TSN_REQ */
enum mipc_data_set_tsn_req_tlv_enum {
    mipc_data_set_tsn_req_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_SET_TSN_REQ_T_CID                             = 0x100,
    /* DS-TT port management information container */
    /* type = byte_array */
    MIPC_DATA_SET_TSN_REQ_T_PMIC                            = 0x101,
    /* NW-TT indication identifier. User can receive MT_PMIC_TRACK_ID from MIPC_DATA_TSN_INFO_IND and set its value to this item to reply NW-TT indication. */
    /* type = uint8_t */
    MIPC_DATA_SET_TSN_REQ_T_MT_PMIC_TRACK_ID                = 0x102,
};

    /* MIPC_MSG.DATA_SET_TSN_CNF */
enum mipc_data_set_tsn_cnf_tlv_enum {
    mipc_data_set_tsn_cnf_tlv_NONE = 0,
    /* NW-TT port management information container */
    /* type = byte_array */
    MIPC_DATA_SET_TSN_CNF_T_PMIC                            = 0x100,
};

    /* MIPC_MSG.DATA_SET_BEARER_INFO_REQ */
enum mipc_data_set_bearer_info_req_tlv_enum {
    mipc_data_set_bearer_info_req_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_SET_BEARER_INFO_REQ_T_CID                     = 0x100,
    /* Start or stop IMS bearer optimization. */
    /* type = uint8_t, refer to DATA_IMS_BEARER_STATE */
    MIPC_DATA_SET_BEARER_INFO_REQ_T_STATE                   = 0x101,
    /* IMS bearer type */
    /* type = uint8_t, refer to DATA_IMS_BEARER_TYPE */
    MIPC_DATA_SET_BEARER_INFO_REQ_T_TYPE                    = 0x102,
    /* IP address type */
    /* type = uint8_t, refer to DATA_IMS_BEARER_PDP_TYPE */
    MIPC_DATA_SET_BEARER_INFO_REQ_T_PDP_TYPE                = 0x103,
};

    /* MIPC_MSG.DATA_SET_BEARER_INFO_CNF */
enum mipc_data_set_bearer_info_cnf_tlv_enum {
    mipc_data_set_bearer_info_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_SET_IP_TUPLE_PACKET_FILTER_REQ */
enum mipc_data_set_ip_tuple_packet_filter_req_tlv_enum {
    mipc_data_set_ip_tuple_packet_filter_req_tlv_NONE = 0,
    /* Indicate this is for all CID or not, if it is set and it is 1, means this filter is for all cid, ID TLV will be not used, if filter is for specific cid, please do not set this field. */
    /* type = uint8_t */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_GLOBAL_SET   = 0x100,
    /* Data call id (apply to which data call. */
    /* type = uint8_t */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_CID          = 0x101,
    /* Refer to PACKET_FILTER_IP_TYPE, 0 means IPv4, 1 means IPv6. */
    /* type = uint8_t, refer to PACKET_FILTER_IP_TYPE */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_IP_TYPE      = 0x102,
    /* Refer to PACKET_FILTER_PROTOCOL_TYPE, 0 means TCP, 1 means UDP. */
    /* type = uint8_t, refer to PACKET_FILTER_PROTOCOL_TYPE */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_PROTOCOL     = 0x103,
    /* Source ip. */
    /* type = byte_array */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_IP_SRC       = 0x104,
    /* Dest ip. */
    /* type = byte_array */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_IP_DST       = 0x105,
    /* Source port. */
    /* type = uint16_t */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_SRC_PORT     = 0x106,
    /* Dest port. */
    /* type = uint16_t */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_REQ_T_DST_PORT     = 0x107,
};

    /* MIPC_MSG.DATA_SET_IP_TUPLE_PACKET_FILTER_CNF */
enum mipc_data_set_ip_tuple_packet_filter_cnf_tlv_enum {
    mipc_data_set_ip_tuple_packet_filter_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_DEL_IP_TUPLE_PACKET_FILTER_REQ */
enum mipc_data_del_ip_tuple_packet_filter_req_tlv_enum {
    mipc_data_del_ip_tuple_packet_filter_req_tlv_NONE = 0,
    /* Indicate this is for all CID or not, if it is set and it is 1, means all cid flter will be removed, ID TLV will be not used, if filter is for specific cid, please do not set this field. Delete filter will not disable the activated filter, please deactivate the filter before delete it. */
    /* type = uint8_t */
    MIPC_DATA_DEL_IP_TUPLE_PACKET_FILTER_REQ_T_GLOBAL_SET   = 0x100,
    /* Indicate the CID which filter will be removed. */
    /* type = uint8_t */
    MIPC_DATA_DEL_IP_TUPLE_PACKET_FILTER_REQ_T_CID          = 0x101,
};

    /* MIPC_MSG.DATA_DEL_IP_TUPLE_PACKET_FILTER_CNF */
enum mipc_data_del_ip_tuple_packet_filter_cnf_tlv_enum {
    mipc_data_del_ip_tuple_packet_filter_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ */
enum mipc_data_set_ip_tuple_packet_filter_mode_req_tlv_enum {
    mipc_data_set_ip_tuple_packet_filter_mode_req_tlv_NONE = 0,
    /* Indicate this is for all CID or not, if it is set and it is 1, means all flter will be activated or deactivated, ID TLV will be not used, if filter is for specific cid, please do not set this field. */
    /* type = uint8_t */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_GLOBAL_SET = 0x100,
    /* Indicate the CID which filter will be activated, currently it is not supported to deactivate filter by cid. */
    /* type = uint8_t */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_CID     = 0x101,
    /* Indicate the filter to be activated or not, 1 means activated, 0 means deactivated. */
    /* type = uint8_t */
    MIPC_DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_OPERATION = 0x102,
};

    /* MIPC_MSG.DATA_SET_IP_TUPLE_PACKET_FILTER_MODE_CNF */
enum mipc_data_set_ip_tuple_packet_filter_mode_cnf_tlv_enum {
    mipc_data_set_ip_tuple_packet_filter_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_REQ */
enum mipc_data_get_ip_tuple_packet_filter_mode_req_tlv_enum {
    mipc_data_get_ip_tuple_packet_filter_mode_req_tlv_NONE = 0,
    /* If this field is 1, It is to get all global set filter count and mode. */
    /* type = uint8_t */
    MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_GLOBAL_SET = 0x100,
    /* It is used to get all filter count and mode for cid. */
    /* type = uint8_t */
    MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_REQ_T_CID     = 0x101,
};

    /* MIPC_MSG.DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_CNF */
enum mipc_data_get_ip_tuple_packet_filter_mode_cnf_tlv_enum {
    mipc_data_get_ip_tuple_packet_filter_mode_cnf_tlv_NONE = 0,
    /* The filter count. */
    /* type = uint8_t */
    MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_CNF_T_FILTER_COUNT = 0x100,
    /* Filter Mode,1 means activated, 0 means deactivated. */
    /* type = uint8_t */
    MIPC_DATA_GET_IP_TUPLE_PACKET_FILTER_MODE_CNF_T_FILTER_MODE = 0x101,
};

    /* MIPC_MSG.DATA_GET_URSP_UE_POLICY_PLMN_LIST_REQ */
enum mipc_data_get_ursp_ue_policy_plmn_list_req_tlv_enum {
    mipc_data_get_ursp_ue_policy_plmn_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_GET_URSP_UE_POLICY_PLMN_LIST_CNF */
enum mipc_data_get_ursp_ue_policy_plmn_list_cnf_tlv_enum {
    mipc_data_get_ursp_ue_policy_plmn_list_cnf_tlv_NONE = 0,
    /* UE policy PLMN list. */
    /* type = struct, refer to ursp_ue_policy_plmn */
    MIPC_DATA_GET_URSP_UE_POLICY_PLMN_LIST_CNF_T_PLMN_TLV_ARRAY = 0x100,
};
    /* MIPC_MSG.DATA_ACT_PACKET_ROUTE_REQ */
enum mipc_data_act_packet_route_req_tlv_enum {
    mipc_data_act_packet_route_req_tlv_NONE = 0,
    /* ipc packet route app_id eap_wifi,0 */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_ACT_PACKET_ROUTE_REQ_T_APP_ID                 = 0x100,
};

    /* MIPC_MSG.DATA_ACT_PACKET_ROUTE_CNF */
enum mipc_data_act_packet_route_cnf_tlv_enum {
    mipc_data_act_packet_route_cnf_tlv_NONE = 0,
    /* IPC_PACKET_ROUTE_APP_ID_EAP_WIFI,0 */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_ACT_PACKET_ROUTE_CNF_T_APP_ID                 = 0x100,
};

    /* MIPC_MSG.DATA_IFUP_PACKET_ROUTE_REQ */
enum mipc_data_ifup_packet_route_req_tlv_enum {
    mipc_data_ifup_packet_route_req_tlv_NONE = 0,
    /* ipc packet route app_id eap_wifi,0 */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_IFUP_PACKET_ROUTE_REQ_T_APP_ID                = 0x100,
};

    /* MIPC_MSG.DATA_IFUP_PACKET_ROUTE_CNF */
enum mipc_data_ifup_packet_route_cnf_tlv_enum {
    mipc_data_ifup_packet_route_cnf_tlv_NONE = 0,
    /* IPC_PACKET_ROUTE_APP_ID_EAP_WIFI,0 */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_IFUP_PACKET_ROUTE_CNF_T_APP_ID                = 0x100,
};

    /* MIPC_MSG.DATA_DC_DATA_PATH_CONFIG_SYNC_REQ */
enum mipc_data_dc_data_path_config_sync_req_tlv_enum {
    mipc_data_dc_data_path_config_sync_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_DC_DATA_PATH_CONFIG_SYNC_CNF */
enum mipc_data_dc_data_path_config_sync_cnf_tlv_enum {
    mipc_data_dc_data_path_config_sync_cnf_tlv_NONE = 0,
    /* client(AP) IP address in binary, 16 bytes. */
    /* type = byte_array */
    MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_CLIENT_IP      = 0x100,
    /* server(MD) IP address in binary, 16 bytes */
    /* type = byte_array */
    MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_SERVER_IP      = 0x101,
    /* Max payload size (data channel application data) for one-time transmission across MD/AP */
    /* type = uint16_t */
    MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_DC_MSS         = 0x102,
};

    /* MIPC_MSG.DATA_ACT_CALL_IND */
enum mipc_data_act_call_ind_tlv_enum {
    mipc_data_act_call_ind_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_ID                             = 0x100,
    MIPC_DATA_ACT_CALL_IND_T_CID                            = 0x100,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_ACT_CALL_IND_T_APN                            = 0x101,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_DATA_ACT_CALL_IND_T_PDP_TYPE                       = 0x102,
    /* IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_V4_COUNT                       = 0x103,
    /* First IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_V4_0                           = 0x8104,
    /* Second IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_V4_1                           = 0x8105,
    /* Third IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_V4_2                           = 0x8106,
    /* Fourth IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_V4_3                           = 0x8107,
    /* IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_V6_COUNT                       = 0x108,
    /* First IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_V6_0                           = 0x8109,
    /* Second IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_V6_1                           = 0x810A,
    /* Third IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_V6_2                           = 0x810B,
    /* Fourth IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_V6_3                           = 0x810C,
    /* DNS IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V4_COUNT                   = 0x10D,
    /* First DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V4_0                       = 0x810E,
    /* Second DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V4_1                       = 0x810F,
    /* Third DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V4_2                       = 0x8110,
    /* Fourth DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V4_3                       = 0x8111,
    /* DNS IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V6_COUNT                   = 0x112,
    /* First DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V6_0                       = 0x8113,
    /* Second DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V6_1                       = 0x8114,
    /* Third DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V6_2                       = 0x8115,
    /* Fourth DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_V6_3                       = 0x8116,
    /* PCSCF IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_COUNT                 = 0x117,
    /* First PCSCF IPv4 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_0                     = 0x8118,
    /* Second PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_1                     = 0x8119,
    /* Third PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_2                     = 0x811A,
    /* Fourth PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V4_3                     = 0x811B,
    /* PCSCF IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_COUNT                 = 0x11C,
    /* First PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_0                     = 0x811D,
    /* Second PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_1                     = 0x811E,
    /* Third PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_2                     = 0x811F,
    /* Fourth PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_V6_3                     = 0x8120,
    /* Gateway IPv4 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_GW_V4                          = 0x8121,
    /* Gateway IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_GW_V6                          = 0x8122,
    /* IPv4 MTU */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_IND_T_MTU_V4                         = 0x123,
    /* IPv6 MTU */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_IND_T_MTU_V6                         = 0x124,
    /* Network interface ID */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_IND_T_INTERFACE_ID                   = 0x125,
    /* Data call primary ID; If the value of this field is equal to data call ID, it means the data call uses default bearer, otherwise it uses specific bearer */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_P_ID                           = 0x126,
    /* Fallback data call ID */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_FB_ID                          = 0x127,
    /* Network transaction ID */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_IND_T_TRANS_ID                       = 0x128,
    /*  IPv4 network mask */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_IND_T_IPv4_NETMASK                   = 0x129,
    /* RAN info, wifi, cellular, or unknown */
    /* type = uint8_t, refer to RAN */
    MIPC_DATA_ACT_CALL_IND_T_RAN_INFO                       = 0x12A,
    /* Network ESM/5GSM bearer ID */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_IND_T_BEARER_ID                      = 0x12B,
    /* IM CN signalling flag from network; it indicates EPS bearer dedicated for IMS signalling; refer to 3GPP 23.228 annex E.2.1.2. */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_IND_T_IM_CN_SIGNALLING_FLAG          = 0x12C,
    /* ethernet MTU */
    /* type = uint32_t */
    MIPC_DATA_ACT_CALL_IND_T_MTU_ETHERNET                   = 0x12D,
    /* IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_IPV4_TLV_ARRAY                 = 0x812E,
    /* IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_IPV6_TLV_ARRAY                 = 0x812F,
    /* DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_IPV4_TLV_ARRAY             = 0x8130,
    /* DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_DNS_IPV6_TLV_ARRAY             = 0x8131,
    /* PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_IPV4_TLV_ARRAY           = 0x8132,
    /* PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_ACT_CALL_IND_T_PCSCF_IPV6_TLV_ARRAY           = 0x8133,
    /*
      It indicates whether this data call indication is dedicated bearer or not.
      True: Dedicated bearer
      False: Default bearer
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_DATA_ACT_CALL_IND_T_IS_DEDICATED_BEARER            = 0x134,
};

    /* MIPC_MSG.DATA_DEACT_CALL_IND */
enum mipc_data_deact_call_ind_tlv_enum {
    mipc_data_deact_call_ind_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_DEACT_CALL_IND_T_ID                           = 0x100,
    MIPC_DATA_DEACT_CALL_IND_T_CID                          = 0x100,
    /* Network error enum; refer to 3GPP 24.008 Annex G */
    /* type = uint8_t */
    MIPC_DATA_DEACT_CALL_IND_T_RES                          = 0x101,
    /* Refer to RESULT */
    /* type = uint32_t */
    MIPC_DATA_DEACT_CALL_IND_T_NEW_RES                      = 0x102,
};

    /* MIPC_MSG.DATA_MOD_CALL_IND */
enum mipc_data_mod_call_ind_tlv_enum {
    mipc_data_mod_call_ind_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_ID                             = 0x100,
    MIPC_DATA_MOD_CALL_IND_T_CID                            = 0x100,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_MOD_CALL_IND_T_APN                            = 0x101,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_DATA_MOD_CALL_IND_T_PDP_TYPE                       = 0x102,
    /* IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_V4_COUNT                       = 0x103,
    /* First IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_V4_0                           = 0x8104,
    /* Second IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_V4_1                           = 0x8105,
    /* Third IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_V4_2                           = 0x8106,
    /* Forth ipv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_V4_3                           = 0x8107,
    /* IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_V6_COUNT                       = 0x108,
    /* First IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_V6_0                           = 0x8109,
    /* Second IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_V6_1                           = 0x810A,
    /* Third IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_V6_2                           = 0x810B,
    /* Forth IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_V6_3                           = 0x810C,
    /* DNS IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V4_COUNT                   = 0x10D,
    /* First DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V4_0                       = 0x810E,
    /* Second DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V4_1                       = 0x810F,
    /* Third DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V4_2                       = 0x8110,
    /* Forth DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V4_3                       = 0x8111,
    /* DNS IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V6_COUNT                   = 0x112,
    /* First DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V6_0                       = 0x8113,
    /* Second DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V6_1                       = 0x8114,
    /* Third DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V6_2                       = 0x8115,
    /* Forth DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_V6_3                       = 0x8116,
    /* P-CSCF IPv4 address count */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_COUNT                 = 0x117,
    /* First P-CSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_0                     = 0x8118,
    /* Second P-CSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_1                     = 0x8119,
    /* Third P-CSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_2                     = 0x811A,
    /* Forth P-CSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V4_3                     = 0x811B,
    /* P-CSCF IPv6 address count */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_COUNT                 = 0x11C,
    /* First P-CSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_0                     = 0x811D,
    /* Second P-CSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_1                     = 0x811E,
    /* Third P-CSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_2                     = 0x811F,
    /* Forth P-CSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_V6_3                     = 0x8120,
    /* Gateway IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_GW_V4                          = 0x8121,
    /* Gateway IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_GW_V6                          = 0x8122,
    /* IPv4 MTU */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_IND_T_MTU_V4                         = 0x123,
    /* IPv6 MTU */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_IND_T_MTU_V6                         = 0x124,
    /* Network interface ID */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_IND_T_INTERFACE_ID                   = 0x125,
    /* Network transaction ID */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_IND_T_TRANS_ID                       = 0x126,
    /* RAN info, wifi, cellular, or unknown */
    /* type = uint8_t, refer to RAN */
    MIPC_DATA_MOD_CALL_IND_T_RAN_INFO                       = 0x127,
    /* Network ESM/5GSM bearer ID */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_IND_T_BEARER_ID                      = 0x128,
    /* IM CN signalling flag from network; it indicates EPS bearer dedicated for IMS signalling; refer to 3GPP 23.228 annex E.2.1.2. */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_IM_CN_SIGNALLING_FLAG          = 0x129,
    /* Data call primary ID; if its value is equal to data call ID, it means the data call uses default bearer, otherwise it uses specific bearer */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_P_ID                           = 0x12A,
    /* ethernet MTU */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_IND_T_MTU_ETHERNET                   = 0x12B,
    /* a bitmap that indicates what kind of change occurred, Bit1 TFT changed, Bit 2 Qos changed, Bit 3 WLAN Offload changed, Bit 4 Relocation of PDU session anchor requested, Bit 5 PDP address or PDP type changed, Bit 6 ATSSS parameters changed, Bit 7 P-CSCF restoration requested */
    /* type = uint32_t */
    MIPC_DATA_MOD_CALL_IND_T_CHANGE_REASON                  = 0x12C,
    /* indicates whether this is an informational event or whether the TE has to acknowledge it. 0 Informational event, 1 Information request: Acknowledgement required. */
    /* type = uint8_t, refer to DATA_MOD_EVENT_TYPE */
    MIPC_DATA_MOD_CALL_IND_T_EVENT_TYPE                     = 0x12D,
    /* IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_IPV4_TLV_ARRAY                 = 0x812E,
    /* IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_IPV6_TLV_ARRAY                 = 0x812F,
    /* DNS IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_IPV4_TLV_ARRAY             = 0x8130,
    /* DNS IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_DNS_IPV6_TLV_ARRAY             = 0x8131,
    /* PCSCF IPv4 address */
    /* type = struct, refer to data_v4_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_IPV4_TLV_ARRAY           = 0x8132,
    /* PCSCF IPv6 address */
    /* type = struct, refer to data_v6_addr */
    MIPC_DATA_MOD_CALL_IND_T_PCSCF_IPV6_TLV_ARRAY           = 0x8133,
    /*
      It indicates whether this data call indication is dedicated bearer or not.
      True: Dedicated bearer
      False: Default bearer
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_DATA_MOD_CALL_IND_T_IS_DEDICATED_BEARER            = 0x134,
    /* NW-TT indication Identifier. It is used to distinguish different NW-TT indication. */
    /* type = uint8_t */
    MIPC_DATA_MOD_CALL_IND_T_MT_PMIC_TRACK_ID               = 0x135,
    /* NW-TT port management information container */
    /* type = byte_array */
    MIPC_DATA_MOD_CALL_IND_T_PMIC                           = 0x136,
};

    /* MIPC_MSG.DATA_MOD_PCO_IND */
enum mipc_data_mod_pco_ind_tlv_enum {
    mipc_data_mod_pco_ind_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_MOD_PCO_IND_T_ID                              = 0x100,
    MIPC_DATA_MOD_PCO_IND_T_CID                             = 0x100,
    /* PCO count */
    /* type = uint8_t */
    MIPC_DATA_MOD_PCO_IND_T_PCO_COUNT                       = 0x101,
    /* The protocol configuration options contents */
    /* type = struct_array, refer to data_pco_ie */
    MIPC_DATA_MOD_PCO_IND_T_PCO_LIST                        = 0x8102,
    /* The protocol configuration options contents */
    /* type = struct, refer to data_pco_ie */
    MIPC_DATA_MOD_PCO_IND_T_PCO_TLV_ARRAY                   = 0x8103,
};

    /* MIPC_MSG.DATA_WWAN_ACT_CALL_IND */
enum mipc_data_wwan_act_call_ind_tlv_enum {
    mipc_data_wwan_act_call_ind_tlv_NONE = 0,
    /* interface ID */
    /* type = uint32_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_INTERFACE_ID              = 0x100,
    /* data call ID */
    /* type = uint32_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_CID                       = 0x101,
    /* APN name */
    /* type = string */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_APN                       = 0x102,
    /* APN type */
    /* type = uint32_t, refer to APN_TYPE */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_APN_TYPE                  = 0x103,
    /* PDP type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_PDP_TYPE                  = 0x104,
    /* mtu of ipv4 */
    /* type = uint32_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_MTU                    = 0x105,
    /* mtu of ipv6 */
    /* type = uint32_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_MTU                    = 0x106,
    /* ipv4 address count */
    /* type = uint8_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_ADDR_COUNT             = 0x107,
    /* ipv4 address struct list */
    /* type = struct_array, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_ADDR_LIST              = 0x8108,
    /* ipv6 address count */
    /* type = uint8_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_ADDR_COUNT             = 0x109,
    /* ipv6 address struct list */
    /* type = struct_array, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_ADDR_LIST              = 0x810A,
    /* ipv4 DNS address count */
    /* type = uint8_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V4_ADDR_COUNT         = 0x10B,
    /* ipv4 DNS address struct list */
    /* type = struct_array, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V4_ADDR_LIST          = 0x810C,
    /* ipv6 DNS address count */
    /* type = uint8_t */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V6_ADDR_COUNT         = 0x10D,
    /* ipv6 DNS address struct list */
    /* type = struct_array, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V6_ADDR_LIST          = 0x810E,
    /* ipv4 address struct list */
    /* type = struct, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V4_ADDR_TLV_ARRAY         = 0x810F,
    /* ipv6 address struct list */
    /* type = struct, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_V6_ADDR_TLV_ARRAY         = 0x8110,
    /* ipv4 DNS address struct list */
    /* type = struct, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V4_ADDR_TLV_ARRAY     = 0x8111,
    /* ipv6 DNS address struct list */
    /* type = struct, refer to addr */
    MIPC_DATA_WWAN_ACT_CALL_IND_T_DNS_V6_ADDR_TLV_ARRAY     = 0x8112,
};

    /* MIPC_MSG.DATA_WWAN_DEACT_CALL_IND */
enum mipc_data_wwan_deact_call_ind_tlv_enum {
    mipc_data_wwan_deact_call_ind_tlv_NONE = 0,
    /* interface ID */
    /* type = uint32_t */
    MIPC_DATA_WWAN_DEACT_CALL_IND_T_INTERFACE_ID            = 0x100,
    /* data call ID */
    /* type = uint32_t */
    MIPC_DATA_WWAN_DEACT_CALL_IND_T_CID                     = 0x101,
    /* apn name */
    /* type = string */
    MIPC_DATA_WWAN_DEACT_CALL_IND_T_APN                     = 0x102,
    /* apn type */
    /* type = uint32_t, refer to APN_TYPE */
    MIPC_DATA_WWAN_DEACT_CALL_IND_T_APN_TYPE                = 0x103,
};

    /* MIPC_MSG.DATA_CALL_ACT_REG_IND */
enum mipc_data_call_act_reg_ind_tlv_enum {
    mipc_data_call_act_reg_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_CALL_DEACT_REG_IND */
enum mipc_data_call_deact_reg_ind_tlv_enum {
    mipc_data_call_deact_reg_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_CALL_MOD_REG_IND */
enum mipc_data_call_mod_reg_ind_tlv_enum {
    mipc_data_call_mod_reg_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_MD_ACT_CALL_IND */
enum mipc_data_md_act_call_ind_tlv_enum {
    mipc_data_md_act_call_ind_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint32_t */
    MIPC_DATA_MD_ACT_CALL_IND_T_CID                         = 0x100,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_MD_ACT_CALL_IND_T_APN                         = 0x101,
    /* It is used to distinguish different data call session with same APN */
    /* type = uint32_t */
    MIPC_DATA_MD_ACT_CALL_IND_T_APN_IDX                     = 0x102,
};

    /* MIPC_MSG.DATA_MD_DEACT_CALL_IND */
enum mipc_data_md_deact_call_ind_tlv_enum {
    mipc_data_md_deact_call_ind_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint32_t */
    MIPC_DATA_MD_DEACT_CALL_IND_T_CID                       = 0x100,
};

    /* MIPC_MSG.DATA_IWLAN_PRIORITY_LIST_IND */
enum mipc_data_iwlan_priority_list_ind_tlv_enum {
    mipc_data_iwlan_priority_list_ind_tlv_NONE = 0,
    /* notification */
    /* type = string */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_CMD                 = 0x100,
    /* pdn type */
    /* type = string */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_TYPE                = 0x101,
    /* setup priority */
    /* type = uint8_t, refer to MIPC_EIWLPL_PRIORITY_TYPE */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_SETUP_PRIORITY      = 0x102,
    /* cellular priority */
    /* type = uint8_t, refer to MIPC_EIWLPL_PRIORITY_TYPE */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_CELLULAR_PRIORITY   = 0x103,
    /* cellular priority */
    /* type = uint8_t, refer to MIPC_EIWLPL_PRIORITY_TYPE */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_WIFI_PRIORITY       = 0x104,
    /* description for long print */
    /* type = string */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_DESCRIPTION         = 0x105,
    /* cellular priority */
    /* type = uint8_t */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_RAT_NUM             = 0x106,
    /* cellular priority */
    /* type = byte_array */
    MIPC_DATA_IWLAN_PRIORITY_LIST_IND_T_RAT_LIST            = 0x107,
};

    /* MIPC_MSG.DATA_LINK_CAPACITY_ESTIMATE_IND */
enum mipc_data_link_capacity_estimate_ind_tlv_enum {
    mipc_data_link_capacity_estimate_ind_tlv_NONE = 0,
    /* Downlink capacity in kbps */
    /* type = uint32_t */
    MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_DL_KBPS          = 0x100,
    /* Uplink capacity in kbps */
    /* type = uint32_t */
    MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_UL_KBPS          = 0x101,
    /* Estimated downlink capacity of secondary carrier in a dual connected NR mode in kbps */
    /* type = uint32_t */
    MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_SECOND_DL_KBPS   = 0x102,
    /* Estimated uplink capacity of secondary carrier in a dual connected NR mode in kbps */
    /* type = uint32_t */
    MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND_T_SECOND_UL_KBPS   = 0x103,
};

    /* MIPC_MSG.DATA_NW_LIMIT_IND */
enum mipc_data_nw_limit_ind_tlv_enum {
    mipc_data_nw_limit_ind_tlv_NONE = 0,
    /* The network limit state */
    /* type = uint32_t */
    MIPC_DATA_NW_LIMIT_IND_T_STATE                          = 0x100,
};

    /* MIPC_MSG.DATA_TIMER_IND */
enum mipc_data_timer_ind_tlv_enum {
    mipc_data_timer_ind_tlv_NONE = 0,
    /* T3396 or back-off */
    /* type = string */
    MIPC_DATA_TIMER_IND_T_SRC_ID                            = 0x100,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_TIMER_IND_T_APN                               = 0x101,
    /* Reject cause from network ESM; the reject cause defined in 3GPP 24.008 Annex H */
    /* type = uint32_t */
    MIPC_DATA_TIMER_IND_T_CAUSE                             = 0x102,
    /*
      0: Start
      1: Stop
      2: Expired
    */
    /* type = uint32_t */
    MIPC_DATA_TIMER_IND_T_TIMER_STATE                       = 0x103,
    /* Expired time in millisecond */
    /* type = uint32_t */
    MIPC_DATA_TIMER_IND_T_EXPIRE_TIME                       = 0x104,
};

    /* MIPC_MSG.DATA_KEEPALIVE_STATUS_IND */
enum mipc_data_keepalive_status_ind_tlv_enum {
    mipc_data_keepalive_status_ind_tlv_NONE = 0,
    /* Connection ID */
    /* type = uint32_t */
    MIPC_DATA_KEEPALIVE_STATUS_IND_T_SESSION_HANDLE         = 0x100,
    /* Keep Alive status code */
    /* type = uint8_t, refer to DATA_KEEPALIVE_STATUS_CODE */
    MIPC_DATA_KEEPALIVE_STATUS_IND_T_STATUS_CODE            = 0x101,
};

    /* MIPC_MSG.DATA_MOBILE_DATA_USAGE_IND */
enum mipc_data_mobile_data_usage_ind_tlv_enum {
    mipc_data_mobile_data_usage_ind_tlv_NONE = 0,
    /* Transmit data in bytes */
    /* type = uint32_t */
    MIPC_DATA_MOBILE_DATA_USAGE_IND_T_TX_BYTES              = 0x100,
    /* Transmit data in packets count */
    /* type = uint32_t */
    MIPC_DATA_MOBILE_DATA_USAGE_IND_T_TX_PACKETS            = 0x101,
    /* Receive data in bytes */
    /* type = uint32_t */
    MIPC_DATA_MOBILE_DATA_USAGE_IND_T_RX_BYTES              = 0x102,
    /* Receive data in packets count */
    /* type = uint32_t */
    MIPC_DATA_MOBILE_DATA_USAGE_IND_T_RX_PACKETS            = 0x103,
};

    /* MIPC_MSG.DATA_NETWORK_REJECT_CAUSE_IND */
enum mipc_data_network_reject_cause_ind_tlv_enum {
    mipc_data_network_reject_cause_ind_tlv_NONE = 0,
    /* Reject cause from network EMM; the reject cause defined in 3GPP 24.008 Annex G */
    /* type = uint16_t */
    MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_EMM_CAUSE          = 0x100,
    /* Reject cause from network ESM; the reject cause defined in 3GPP 24.008 Annex H */
    /* type = uint16_t */
    MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_ESM_CAUSE          = 0x101,
    /*
      EMM cause source
      0: Others
      1: Attach reject
      2: Tracking area update reject
      3: Network detach
    */
    /* type = uint16_t */
    MIPC_DATA_NETWORK_REJECT_CAUSE_IND_T_EVENT              = 0x102,
};

    /* MIPC_MSG.DATA_DSDA_STATE_IND */
enum mipc_data_dsda_state_ind_tlv_enum {
    mipc_data_dsda_state_ind_tlv_NONE = 0,
    /* dual sim dual active allowed state */
    /* type = uint8_t, refer to DSDA_ALLOWED_TYPE */
    MIPC_DATA_DSDA_STATE_IND_T_DSDA_ALLOWED                 = 0x100,
    /* dual sim dual active state */
    /* type = uint8_t, refer to DSDA_STATE_TYPE */
    MIPC_DATA_DSDA_STATE_IND_T_DSDA_STATE                   = 0x101,
    /* type = uint8_t, refer to DR_DSDA_TYPE */
    MIPC_DATA_DSDA_STATE_IND_T_IS_DR_DSDA                   = 0x102,
    /* type = uint8_t, refer to DR_DSDS_TYPE */
    MIPC_DATA_DSDA_STATE_IND_T_IS_DR_DSDS                   = 0x103,
};

    /* MIPC_MSG.DATA_UMTS_PS_STATE_IND */
enum mipc_data_umts_ps_state_ind_tlv_enum {
    mipc_data_umts_ps_state_ind_tlv_NONE = 0,
    /* UMTS PS domain state */
    /* type = uint8_t */
    MIPC_DATA_UMTS_PS_STATE_IND_T_CONN_STATUS               = 0x100,
};

    /* MIPC_MSG.DATA_RETRY_TIMER_IND */
enum mipc_data_retry_timer_ind_tlv_enum {
    mipc_data_retry_timer_ind_tlv_NONE = 0,
    /* Access Point Name */
    /* type = string */
    MIPC_DATA_RETRY_TIMER_IND_T_APN_NAME                    = 0x100,
};

    /* MIPC_MSG.DATA_URSP_REEVAL_IND */
enum mipc_data_ursp_reeval_ind_tlv_enum {
    mipc_data_ursp_reeval_ind_tlv_NONE = 0,
    /* data call ID list length */
    /* type = uint8_t */
    MIPC_DATA_URSP_REEVAL_IND_T_ID_LIST_LEN                 = 0x100,
    /* data call ID list */
    /* type = byte_array */
    MIPC_DATA_URSP_REEVAL_IND_T_ID_LIST                     = 0x101,
    /* URSP re-evaluation event */
    /* type = uint32_t, refer to URSP_REEVAL_TYPE_ENUM */
    MIPC_DATA_URSP_REEVAL_IND_T_EVENT                       = 0x102,
};

    /* MIPC_MSG.DATA_URSP_UE_POLICY_CHG_IND */
enum mipc_data_ursp_ue_policy_chg_ind_tlv_enum {
    mipc_data_ursp_ue_policy_chg_ind_tlv_NONE = 0,
    /* plmn id, mcc1, mcc2, mcc3, mnc1, mnc2, mnc3 */
    /* type = byte_array */
    MIPC_DATA_URSP_UE_POLICY_CHG_IND_T_PLMN_ID              = 0x100,
    /* ue policy change event */
    /* type = uint32_t, refer to URSP_REEVAL_TYPE_ENUM */
    MIPC_DATA_URSP_UE_POLICY_CHG_IND_T_EVENT                = 0x101,
};

    /* MIPC_MSG.DATA_PDN_NW_CAUSE_IND */
enum mipc_data_pdn_nw_cause_ind_tlv_enum {
    mipc_data_pdn_nw_cause_ind_tlv_NONE = 0,
    /* EMM cause returned by network */
    /* type = uint32_t */
    MIPC_DATA_PDN_NW_CAUSE_IND_T_EMM_CAUSE                  = 0x100,
    /* ESM cause returned by network */
    /* type = uint32_t */
    MIPC_DATA_PDN_NW_CAUSE_IND_T_ESM_CAUSE                  = 0x101,
    /* APN name of which the network cause is reported */
    /* type = string */
    MIPC_DATA_PDN_NW_CAUSE_IND_T_APN                        = 0x102,
};

    /* MIPC_MSG.DATA_PAGING_RESTRICTIONS_IND */
enum mipc_data_paging_restrictions_ind_tlv_enum {
    mipc_data_paging_restrictions_ind_tlv_NONE = 0,
    /* Indicates whether the requested paging restriction preferences of the UE are accept or not */
    /* type = uint8_t, refer to DATA_PAGING_RESTRICT_RESULT */
    MIPC_DATA_PAGING_RESTRICTIONS_IND_T_PAGING_RESTRICT_RESULT = 0x100,
};

    /* MIPC_MSG.DATA_TSN_TIME_IND */
enum mipc_data_tsn_time_ind_tlv_enum {
    mipc_data_tsn_time_ind_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_TSN_TIME_IND_T_CID                            = 0x100,
    /* Network reference time */
    /* type = struct, refer to data_tsn_time */
    MIPC_DATA_TSN_TIME_IND_T_NW_REF_TIME                    = 0x101,
};

    /* MIPC_MSG.DATA_IP_PACKET_RECV_IND */
enum mipc_data_ip_packet_recv_ind_tlv_enum {
    mipc_data_ip_packet_recv_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.DATA_IFUP_PACKET_ROUTE_IND */
enum mipc_data_ifup_packet_route_ind_tlv_enum {
    mipc_data_ifup_packet_route_ind_tlv_NONE = 0,
    /* IPC_PACKET_ROUTE_APP_ID_EAP_WIFI,0 */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_APP_ID                = 0x100,
    /* net type */
    /* type = uint8_t */
    MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_NET_TYPE              = 0x101,
    /* nw interface id */
    /* type = uint8_t */
    MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_NW_ITF_ID             = 0x102,
    /* hw upload queue */
    /* type = uint8_t */
    MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_HW_UL_Q               = 0x103,
};

    /* MIPC_MSG.DATA_IFUP_PACKET_ROUTE_CMD */
enum mipc_data_ifup_packet_route_cmd_tlv_enum {
    mipc_data_ifup_packet_route_cmd_tlv_NONE = 0,
    /* IPC_PACKET_ROUTE_APP_ID_EAP_WIFI,0 */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_APP_ID                = 0x100,
    /* net type */
    /* type = uint8_t */
    MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_NET_TYPE              = 0x101,
    /* nw itf id */
    /* type = uint8_t */
    MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_NW_ITF_ID             = 0x102,
    /* hw upload q */
    /* type = uint8_t */
    MIPC_DATA_IFUP_PACKET_ROUTE_CMD_T_HW_UL_Q               = 0x103,
};

    /* MIPC_MSG.DATA_IFUP_PACKET_ROUTE_RSP */
enum mipc_data_ifup_packet_route_rsp_tlv_enum {
    mipc_data_ifup_packet_route_rsp_tlv_NONE = 0,
    /* IPC_PACKET_ROUTE_APP_ID_EAP_WIFI,0 */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_IFUP_PACKET_ROUTE_RSP_T_APP_ID                = 0x100,
};

    /* MIPC_MSG.DATA_ACT_CALL_NTF */
enum mipc_data_act_call_ntf_tlv_enum {
    mipc_data_act_call_ntf_tlv_NONE = 0,
    /* Context identifier */
    /* type = uint8_t */
    MIPC_DATA_ACT_CALL_NTF_T_ID                             = 0x100,
    MIPC_DATA_ACT_CALL_NTF_T_CID                            = 0x100,
    /* It indicates whether the fallback CID is used or unused */
    /* type = uint8_t, refer to ACT_CALL_NTF_ENUM */
    MIPC_DATA_ACT_CALL_NTF_T_RESPONSE                       = 0x101,
};

    /* MIPC_MSG.DATA_ACT_PACKET_ROUTE_NTF */
enum mipc_data_act_packet_route_ntf_tlv_enum {
    mipc_data_act_packet_route_ntf_tlv_NONE = 0,
    /* indicate the cid */
    /* type = uint8_t, refer to IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_DATA_ACT_PACKET_ROUTE_NTF_T_APP_ID                 = 0x100,
};


#endif /* __MIPC_MSG_DATA_CONST_H__ */
