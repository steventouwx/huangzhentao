#ifndef __MIPC_MSG_WFC_CONST_H__
#define __MIPC_MSG_WFC_CONST_H__

enum MIPC_WFC_MSG_enum {
    MIPC_WFC_MSG_NONE = 0,
    /* register cell signal ind(CELL_SIGNAL_IND), diffirent SIGNAL_TYPE has different parameters */
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ                   = 2816,
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_CNF                   = 2817,

    /* Service specific access control restriction status query */
    MIPC_WFC_CSSAC_REQ                                      = 2818,
    MIPC_WFC_CSSAC_CNF                                      = 2819,

    /* used for PEI (P-Emergency-Info) Header in all emergency call as part of SIP Invite; Support for IMS AT&T project. Refer to <CDR-WiFi-1310> PEI Header;This command is suggested to be sent before emergency call is made; This ai for WiFi calling. */
    MIPC_WFC_SET_EMC_AID_REQ                                = 2820,
    MIPC_WFC_SET_EMC_AID_CNF                                = 2821,

    /* cfg relate operation */
    MIPC_WFC_CFG_REQ                                        = 2822,
    MIPC_WFC_CFG_CNF                                        = 2823,

    /* This command is used to receive eAP init and version information from AP */
    MIPC_WFC_SET_EAP_VERSION_REQ                            = 2824,
    MIPC_WFC_SET_EAP_VERSION_CNF                            = 2825,

    /* cell quality indication(configured by REGISTER_CELL_SIGNAL_IND_REQ); refer +EHOMEAS */
    MIPC_WFC_CELL_SIGNAL_IND                                = 19200,

    /* notify AP current active PDN count on WiFi; refer +EWIFIPDNACT */
    MIPC_WFC_WIFI_PDN_COUNT_IND                             = 19201,

    /* inform AP about handover progress; refer +EPDNHANDOVER */
    MIPC_WFC_PDN_HO_IND                                     = 19202,

    /* notify AP that UE is going to rove out WiFi coverage when WFC in call; refer +EWFCRVOUT */
    MIPC_WFC_ROVE_OUT_IND                                   = 19203,

    /* Service specific access control restriction status ind; refer +ESSAC */
    MIPC_WFC_SSAC_IND                                       = 19204,

    /* wifi pdn error from modem; refer +EWOPDNERR */
    MIPC_WFC_WIFI_PDN_ERR_IND                               = 19205,

    /*  IMS pdn out of service from modem; refer +EIMSPDNOOS */
    MIPC_WFC_WIFI_PDN_OOS_IND                               = 19206,

    /* some information about WFC; refer +EWFC&+EWIFILOCK */
    MIPC_WFC_WFC_IND                                        = 19207,

    /* request HOST to Ping NW and response with result; refer +EIWLPING */
    MIPC_WFC_PING_CMD                                       = 35584,
    MIPC_WFC_PING_RSP                                       = 35585,

    /* Modem request ARP/NDP query; refer +EWIFIMAC */
    MIPC_WFC_GET_MAC_CMD                                    = 35586,
    MIPC_WFC_GET_MAC_RSP                                    = 35587,

    /* request AP that WO need keep alive service; refer +EWOKEEPALIVE */
    MIPC_WFC_NATT_KEEP_ALIVE_CMD                            = 35588,
    MIPC_WFC_NATT_KEEP_ALIVE_RSP                            = 35589,

    /* MD use this CMD to register the WIFI_SIGNAL_NTF; refer +EWIFIRSSITHRCFG */
    MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD                   = 35590,
    MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_RSP                   = 35591,

    /* This MIPC is used to register the filter at eAP and waiting for command response eAP */
    MIPC_WFC_SET_DL_FILTER_CMD                              = 35592,
    MIPC_WFC_SET_DL_FILTER_RSP                              = 35593,

    /* This MIPC is used to deregister the filter at eAP and waiting for command response eAP */
    MIPC_WFC_DEL_DL_FILTER_CMD                              = 35594,
    MIPC_WFC_DEL_DL_FILTER_RSP                              = 35595,

    /* update WiFi signal value to the modem; refer AT+EWIFISIGLVL */
    MIPC_WFC_WIFI_SIGNAL_NTF                                = 51968,

    /* Notify ePDG screen state; refer AT+ESCREENSTATE */
    MIPC_WFC_EPDG_SCREEN_STATE_NTF                          = 51969,

    /* provide the wifi information to modem module; refer AT+EWIFIASC&AT+EWIFIEN&AT+EWIFIADDR */
    MIPC_WFC_WIFI_INFO_NTF                                  = 51970,


};

    /* MIPC_MSG.WFC_REGISTER_CELL_SIGNAL_IND_REQ */
enum mipc_wfc_register_cell_signal_ind_req_tlv_enum {
    mipc_wfc_register_cell_signal_ind_req_tlv_NONE = 0,
    /* witch SIGNAL_TYPE this req is set for */
    /* type = uint8_t, refer to WFC_SIGNAL_TYPE */
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_SIGNAL_TYPE     = 0x100,
    /* enable IND? */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_ENABLE_RPT      = 0x101,
    /* default value(if not carry this TLV):500; unit:ms; Default value is 500 */
    /* type = uint32_t */
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_TIME            = 0x102,
    /* unit:dbm; Default value is 0 */
    /* type = int16_t */
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_THRESHOLD_IN    = 0x103,
    /* unit:dbm; Default value is 0 */
    /* type = int16_t */
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_THRESHOLD_OUT   = 0x104,
    /* threshold value for extension; unit:dbm */
    /* type = int16_t */
    MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ_T_THRESHOLD_EXT   = 0x105,
};

    /* MIPC_MSG.WFC_REGISTER_CELL_SIGNAL_IND_CNF */
enum mipc_wfc_register_cell_signal_ind_cnf_tlv_enum {
    mipc_wfc_register_cell_signal_ind_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.WFC_CSSAC_REQ */
enum mipc_wfc_cssac_req_tlv_enum {
    mipc_wfc_cssac_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.WFC_CSSAC_CNF */
enum mipc_wfc_cssac_cnf_tlv_enum {
    mipc_wfc_cssac_cnf_tlv_NONE = 0,
    /* parameter shows the barring factor for MMTEL voice; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_CSSAC_CNF_T_BF_VOICE                           = 0x100,
    /* parameter shows the barring factor for MMTEL video; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_CSSAC_CNF_T_BF_VIDEO                           = 0x101,
    /* parameter shows the barring timer for MMTEL voice; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_CSSAC_CNF_T_BT_VOICE                           = 0x102,
    /* parameter shows the barring timer for MMTEL video; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_CSSAC_CNF_T_BT_VIDEO                           = 0x103,
};

    /* MIPC_MSG.WFC_SET_EMC_AID_REQ */
enum mipc_wfc_set_emc_aid_req_tlv_enum {
    mipc_wfc_set_emc_aid_req_tlv_NONE = 0,
    /* aid=xxxx-yyyy-wwww-zzzz (64 bit hex values) */
    /* type = string */
    MIPC_WFC_SET_EMC_AID_REQ_T_AID                          = 0x100,
};

    /* MIPC_MSG.WFC_SET_EMC_AID_CNF */
enum mipc_wfc_set_emc_aid_cnf_tlv_enum {
    mipc_wfc_set_emc_aid_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.WFC_CFG_REQ */
enum mipc_wfc_cfg_req_tlv_enum {
    mipc_wfc_cfg_req_tlv_NONE = 0,
    /* location enable */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_CFG_REQ_T_LOCATION_ENABLE                      = 0x100,
    /* set WiFi Calling Profile by AP */
    /* type = uint8_t, refer to WFC_PREFER */
    MIPC_WFC_CFG_REQ_T_WFC_PREFER                           = 0x101,
};

    /* MIPC_MSG.WFC_CFG_CNF */
enum mipc_wfc_cfg_cnf_tlv_enum {
    mipc_wfc_cfg_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.WFC_SET_EAP_VERSION_REQ */
enum mipc_wfc_set_eap_version_req_tlv_enum {
    mipc_wfc_set_eap_version_req_tlv_NONE = 0,
    /* Filter version of AP */
    /* type = uint32_t */
    MIPC_WFC_SET_EAP_VERSION_REQ_T_AP_FILTER_VER            = 0x100,
};

    /* MIPC_MSG.WFC_SET_EAP_VERSION_CNF */
enum mipc_wfc_set_eap_version_cnf_tlv_enum {
    mipc_wfc_set_eap_version_cnf_tlv_NONE = 0,
    /* Filter version of Modem */
    /* type = uint32_t */
    MIPC_WFC_SET_EAP_VERSION_CNF_T_MD_FILTER_VER            = 0x100,
};

    /* MIPC_MSG.WFC_CELL_SIGNAL_IND */
enum mipc_wfc_cell_signal_ind_tlv_enum {
    mipc_wfc_cell_signal_ind_tlv_NONE = 0,
    /* witch SIGNAL_TYPE this indication is for */
    /* type = uint8_t, refer to WFC_SIGNAL_TYPE */
    MIPC_WFC_CELL_SIGNAL_IND_T_SIGNAL_TYPE                  = 0x100,
    /* unit:dbm */
    /* type = int16_t */
    MIPC_WFC_CELL_SIGNAL_IND_T_VALUE                        = 0x101,
};

    /* MIPC_MSG.WFC_WIFI_PDN_COUNT_IND */
enum mipc_wfc_wifi_pdn_count_ind_tlv_enum {
    mipc_wfc_wifi_pdn_count_ind_tlv_NONE = 0,
    /* count */
    /* type = uint8_t */
    MIPC_WFC_WIFI_PDN_COUNT_IND_T_COUNT                     = 0x100,
};

    /* MIPC_MSG.WFC_PDN_HO_IND */
enum mipc_wfc_pdn_ho_ind_tlv_enum {
    mipc_wfc_pdn_ho_ind_tlv_NONE = 0,
    /* pdn type */
    /* type = uint8_t, refer to WFC_PDN_TYPE */
    MIPC_WFC_PDN_HO_IND_T_PDN_TYPE                          = 0x100,
    /* status */
    /* type = uint8_t, refer to WFC_PDN_HO_STATUS */
    MIPC_WFC_PDN_HO_IND_T_STATUS                            = 0x101,
    /* src_rat */
    /* type = uint8_t, refer to WFC_RAT */
    MIPC_WFC_PDN_HO_IND_T_SRC_RAT                           = 0x102,
    /* dst_rat */
    /* type = uint8_t, refer to WFC_RAT */
    MIPC_WFC_PDN_HO_IND_T_DST_RAT                           = 0x103,
};

    /* MIPC_MSG.WFC_ROVE_OUT_IND */
enum mipc_wfc_rove_out_ind_tlv_enum {
    mipc_wfc_rove_out_ind_tlv_NONE = 0,
    /* WiFi interface name, len contains /0 */
    /* type = string */
    MIPC_WFC_ROVE_OUT_IND_T_IFNAME                          = 0x100,
    /* TRUE: Wifi roveout, no LTE for H.O.; FALSE: Wifi RSSI acceptable */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_ROVE_OUT_IND_T_RVOUT                           = 0x101,
    /* TRUE: trigger mobike; FALSE: not trigger mobike */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_ROVE_OUT_IND_T_MOBIKE_IND                      = 0x102,
};

    /* MIPC_MSG.WFC_SSAC_IND */
enum mipc_wfc_ssac_ind_tlv_enum {
    mipc_wfc_ssac_ind_tlv_NONE = 0,
    /* parameter shows the barring factor for MMTEL voice; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_SSAC_IND_T_BF_VOICE                            = 0x100,
    /* parameter shows the barring factor for MMTEL video; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_SSAC_IND_T_BF_VIDEO                            = 0x101,
    /* parameter shows the barring timer for MMTEL voice; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_SSAC_IND_T_BT_VOICE                            = 0x102,
    /* parameter shows the barring timer for MMTEL video; refer to TS27.007 +CSSAC */
    /* type = uint16_t */
    MIPC_WFC_SSAC_IND_T_BT_VIDEO                            = 0x103,
};

    /* MIPC_MSG.WFC_WIFI_PDN_ERR_IND */
enum mipc_wfc_wifi_pdn_err_ind_tlv_enum {
    mipc_wfc_wifi_pdn_err_ind_tlv_NONE = 0,
    /* n3epc_public_defs.h -> n3epc_pdn_rejct_enum */
    /* type = int32_t */
    MIPC_WFC_WIFI_PDN_ERR_IND_T_CAUSE                       = 0x100,
    /* refer n3epc_public_defs.h -> n3epc_ike_error_notify_enum */
    /* type = int32_t */
    MIPC_WFC_WIFI_PDN_ERR_IND_T_SUB_CAUSE                   = 0x101,
    /* last retry */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_WIFI_PDN_ERR_IND_T_IS_LAST_RETRY               = 0x102,
};

    /* MIPC_MSG.WFC_WIFI_PDN_OOS_IND */
enum mipc_wfc_wifi_pdn_oos_ind_tlv_enum {
    mipc_wfc_wifi_pdn_oos_ind_tlv_NONE = 0,
    /* apn name */
    /* type = string */
    MIPC_WFC_WIFI_PDN_OOS_IND_T_APN                         = 0x100,
    /* context identifier */
    /* type = uint8_t */
    MIPC_WFC_WIFI_PDN_OOS_IND_T_CID                         = 0x101,
    /* oos state */
    /* type = uint8_t, refer to WFC_PDN_OOS_STATE */
    MIPC_WFC_WIFI_PDN_OOS_IND_T_OOS_STATE                   = 0x102,
};

    /* MIPC_MSG.WFC_WFC_IND */
enum mipc_wfc_wfc_ind_tlv_enum {
    mipc_wfc_wfc_ind_tlv_NONE = 0,
    /* [PHASE OUT]indicate the wifi ims pdn state, please use WIFI_IMS_PDN_STATE instead of this TLV(to avoid misunderstanding about naming) */
    /* type = uint8_t, refer to WFC_WIFI_PDN_STATE */
    MIPC_WFC_WFC_IND_T_WIFI_PDN_STATE                       = 0x100,
    /* VoData and data sim. <value> is the second parameter in URC +EWFC: 3, <value> */
    /* type = uint8_t */
    MIPC_WFC_WFC_IND_T_DATA_SIM                             = 0x101,
    /* WiFi interface name for LOCK action, len contains /0 */
    /* type = string */
    MIPC_WFC_WFC_IND_T_IFNAME                               = 0x102,
    /* TRUE:enable lock status; FALSE:disable lock status */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_WFC_IND_T_LOCK                                 = 0x103,
    /* indicate the wifi ims pdn state */
    /* type = uint8_t, refer to WFC_WIFI_PDN_STATE */
    MIPC_WFC_WFC_IND_T_WIFI_IMS_PDN_STATE                   = 0x104,
};

    /* MIPC_MSG.WFC_PING_CMD */
enum mipc_wfc_ping_cmd_tlv_enum {
    mipc_wfc_ping_cmd_tlv_NONE = 0,
    /* the rat to ping */
    /* type = uint8_t, refer to WFC_RAT */
    MIPC_WFC_PING_CMD_T_RAT                                 = 0x100,
};

    /* MIPC_MSG.WFC_PING_RSP */
enum mipc_wfc_ping_rsp_tlv_enum {
    mipc_wfc_ping_rsp_tlv_NONE = 0,
    /* the rat ping */
    /* type = uint8_t, refer to WFC_RAT */
    MIPC_WFC_PING_RSP_T_RAT                                 = 0x100,
    /* unit:ms */
    /* type = uint16_t */
    MIPC_WFC_PING_RSP_T_AVE_LATENCY                         = 0x101,
    /* loss_rate */
    /* type = uint8_t */
    MIPC_WFC_PING_RSP_T_LOSS_RATE                           = 0x102,
};

    /* MIPC_MSG.WFC_GET_MAC_CMD */
enum mipc_wfc_get_mac_cmd_tlv_enum {
    mipc_wfc_get_mac_cmd_tlv_NONE = 0,
    /* WiFi interface name, len contains /0 */
    /* type = string */
    MIPC_WFC_GET_MAC_CMD_T_IFNAME                           = 0x100,
    /* ip, len=4->IPv4;len=16->IPv6 */
    /* type = byte_array */
    MIPC_WFC_GET_MAC_CMD_T_IP                               = 0x101,
};

    /* MIPC_MSG.WFC_GET_MAC_RSP */
enum mipc_wfc_get_mac_rsp_tlv_enum {
    mipc_wfc_get_mac_rsp_tlv_NONE = 0,
    /* 0-success */
    /* type = int32_t */
    MIPC_WFC_GET_MAC_RSP_T_GET_RESULT                       = 0x100,
    /* WiFi interface name, len contains /0 */
    /* type = string */
    MIPC_WFC_GET_MAC_RSP_T_IFNAME                           = 0x101,
    /* ip, len=4->IPv4;len=16->IPv6 */
    /* type = byte_array */
    MIPC_WFC_GET_MAC_RSP_T_IP                               = 0x102,
    /* mac address */
    /* type = byte_array */
    MIPC_WFC_GET_MAC_RSP_T_MAC                              = 0x103,
};

    /* MIPC_MSG.WFC_NATT_KEEP_ALIVE_CMD */
enum mipc_wfc_natt_keep_alive_cmd_tlv_enum {
    mipc_wfc_natt_keep_alive_cmd_tlv_NONE = 0,
    /* enable? */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_ENABLE                   = 0x100,
    /* unit:s,if interval < 20s, wifi NATT keep alive will not start */
    /* type = uint16_t */
    MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_INTERVAL                 = 0x101,
    /* ip, len=4->IPv4; len=16->IPv6 */
    /* type = byte_array */
    MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_SRC_IP                   = 0x102,
    /* src_port */
    /* type = uint16_t */
    MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_SRC_PORT                 = 0x103,
    /* ip, len=4->IPv4; len=16->IPv6 */
    /* type = byte_array */
    MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_DST_IP                   = 0x104,
    /* dst_port */
    /* type = uint16_t, refer to BOOLEAN */
    MIPC_WFC_NATT_KEEP_ALIVE_CMD_T_DST_PORT                 = 0x105,
};

    /* MIPC_MSG.WFC_NATT_KEEP_ALIVE_RSP */
enum mipc_wfc_natt_keep_alive_rsp_tlv_enum {
    mipc_wfc_natt_keep_alive_rsp_tlv_NONE = 0,
    /* WiFi interface name, len contains /0 */
    /* type = string */
    MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_IFNAME                   = 0x100,
    /* natt enable? */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_ENABLE                   = 0x101,
    /* ip, len=4->IPv4; len=16->IPv6 */
    /* type = byte_array */
    MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_SRC_IP                   = 0x102,
    /* src_port */
    /* type = uint16_t */
    MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_SRC_PORT                 = 0x103,
    /* ip, len=4->IPv4; len=16->IPv6 */
    /* type = byte_array */
    MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_DST_IP                   = 0x104,
    /* dst_port */
    /* type = uint16_t */
    MIPC_WFC_NATT_KEEP_ALIVE_RSP_T_DST_PORT                 = 0x105,
};

    /* MIPC_MSG.WFC_REGISTER_WIFI_SIGNAL_NTF_CMD */
enum mipc_wfc_register_wifi_signal_ntf_cmd_tlv_enum {
    mipc_wfc_register_wifi_signal_ntf_cmd_tlv_NONE = 0,
    /* enable NTF(WIFI_SIGNAL_NTF)? */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD_T_ENABLE_NTF      = 0x100,
    /* [wtcfm]uint:dbm? */
    /* type = int16_t */
    MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD_T_RSSI_THRESHOLD  = 0x101,
};

    /* MIPC_MSG.WFC_REGISTER_WIFI_SIGNAL_NTF_RSP */
enum mipc_wfc_register_wifi_signal_ntf_rsp_tlv_enum {
    mipc_wfc_register_wifi_signal_ntf_rsp_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.WFC_SET_DL_FILTER_CMD */
enum mipc_wfc_set_dl_filter_cmd_tlv_enum {
    mipc_wfc_set_dl_filter_cmd_tlv_NONE = 0,
    /* Filter ID */
    /* type = uint32_t */
    MIPC_WFC_SET_DL_FILTER_CMD_T_FID                        = 0x100,
    /* The filter register at eAP */
    /* type = struct, refer to wfp_filter_struct */
    MIPC_WFC_SET_DL_FILTER_CMD_T_WFP_FILTER                 = 0x101,
};

    /* MIPC_MSG.WFC_SET_DL_FILTER_RSP */
enum mipc_wfc_set_dl_filter_rsp_tlv_enum {
    mipc_wfc_set_dl_filter_rsp_tlv_NONE = 0,
    /* Filter ID */
    /* type = uint32_t */
    MIPC_WFC_SET_DL_FILTER_RSP_T_FID                        = 0x100,
};

    /* MIPC_MSG.WFC_DEL_DL_FILTER_CMD */
enum mipc_wfc_del_dl_filter_cmd_tlv_enum {
    mipc_wfc_del_dl_filter_cmd_tlv_NONE = 0,
    /* Filter ID */
    /* type = uint32_t */
    MIPC_WFC_DEL_DL_FILTER_CMD_T_FID                        = 0x100,
};

    /* MIPC_MSG.WFC_DEL_DL_FILTER_RSP */
enum mipc_wfc_del_dl_filter_rsp_tlv_enum {
    mipc_wfc_del_dl_filter_rsp_tlv_NONE = 0,
    /* Filter ID */
    /* type = uint32_t */
    MIPC_WFC_DEL_DL_FILTER_RSP_T_FID                        = 0x100,
};

    /* MIPC_MSG.WFC_WIFI_SIGNAL_NTF */
enum mipc_wfc_wifi_signal_ntf_tlv_enum {
    mipc_wfc_wifi_signal_ntf_tlv_NONE = 0,
    /* WiFi interface name, len contains /0 */
    /* type = string */
    MIPC_WFC_WIFI_SIGNAL_NTF_T_IFNAME                       = 0x100,
    /* uint:dbm */
    /* type = int16_t */
    MIPC_WFC_WIFI_SIGNAL_NTF_T_RSSI                         = 0x101,
    /* unit: dB */
    /* type = int16_t */
    MIPC_WFC_WIFI_SIGNAL_NTF_T_SNR                          = 0x102,
};

    /* MIPC_MSG.WFC_EPDG_SCREEN_STATE_NTF */
enum mipc_wfc_epdg_screen_state_ntf_tlv_enum {
    mipc_wfc_epdg_screen_state_ntf_tlv_NONE = 0,
    /* screen state */
    /* type = uint8_t, refer to EPDG_SCREEN_STATE */
    MIPC_WFC_EPDG_SCREEN_STATE_NTF_T_STATE                  = 0x100,
};

    /* MIPC_MSG.WFC_WIFI_INFO_NTF */
enum mipc_wfc_wifi_info_ntf_tlv_enum {
    mipc_wfc_wifi_info_ntf_tlv_NONE = 0,
    /* WiFi interface name, len contains /0 */
    /* type = string */
    MIPC_WFC_WIFI_INFO_NTF_T_IFNAME                         = 0x100,
    /* enable? */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_WIFI_INFO_NTF_T_ENABLE                         = 0x101,
    /* cause of enable or disable; now only 1=flight_mode, expand to enum? */
    /* type = uint8_t */
    MIPC_WFC_WIFI_INFO_NTF_T_CAUSE                          = 0x102,
    /* [PHASE OUT]associated? */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_WIFI_INFO_NTF_T_ASSOCIATED                     = 0x103,
    /* ssid, len contains /0 */
    /* type = string */
    MIPC_WFC_WIFI_INFO_NTF_T_SSID                           = 0x104,
    /* ap mac address */
    /* type = byte_array */
    MIPC_WFC_WIFI_INFO_NTF_T_AP_MAC                         = 0x105,
    /* [PHASE OUT] replaced by WIFI_TYPE_STR */
    /* type = uint8_t, refer to WIFI_TYPE */
    MIPC_WFC_WIFI_INFO_NTF_T_WIFI_TYPE                      = 0x106,
    /* mtu size */
    /* type = uint16_t */
    MIPC_WFC_WIFI_INFO_NTF_T_MTU                            = 0x107,
    /* ue mac address */
    /* type = byte_array */
    MIPC_WFC_WIFI_INFO_NTF_T_UE_MAC                         = 0x108,
    /* ipv4 address */
    /* type = byte_array */
    MIPC_WFC_WIFI_INFO_NTF_T_IPV4                           = 0x109,
    /* ipv4 prefix len */
    /* type = uint8_t */
    MIPC_WFC_WIFI_INFO_NTF_T_IPV4_PREFIX_LEN                = 0x10A,
    /* ipv4 gateway */
    /* type = byte_array */
    MIPC_WFC_WIFI_INFO_NTF_T_IPV4_GATEWAY                   = 0x10B,
    /* ipv6 address */
    /* type = byte_array */
    MIPC_WFC_WIFI_INFO_NTF_T_IPV6                           = 0x10C,
    /* ipv6 prefix len */
    /* type = uint8_t */
    MIPC_WFC_WIFI_INFO_NTF_T_IPV6_PREFIX_LEN                = 0x10D,
    /* ipv6 gateway */
    /* type = byte_array */
    MIPC_WFC_WIFI_INFO_NTF_T_IPV6_GATEWAY                   = 0x10E,
    /* dns */
    /* type = byte_array */
    MIPC_WFC_WIFI_INFO_NTF_T_DNS                            = 0x10F,
    /* indicate if the IP&DNS info need update */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_WIFI_INFO_NTF_T_IP_UPDATE                      = 0x110,
    /* same as WIFI_TYPE, this use string format for backward compatible */
    /* type = string */
    MIPC_WFC_WIFI_INFO_NTF_T_WIFI_TYPE_STR                  = 0x111,
    /* [PHASE OUT]meaning has wifi transport capability and net capability validated */
    /* type = uint8_t, refer to WIFI_EXTEND_STATE */
    MIPC_WFC_WIFI_INFO_NTF_T_WIFI_EXTEND_STATE              = 0x112,
    /* wifi connect state */
    /* type = uint8_t, refer to WIFI_CONN_STATE */
    MIPC_WFC_WIFI_INFO_NTF_T_CONN_STATE                     = 0x113,
    /* Include Default Route to determine internet reachability for Wi-Fi and captive portal */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_WFC_WIFI_INFO_NTF_T_CONN_READY                     = 0x114,
    /*
      Value used to track whether Wi-Fi should remain On/Off in Airplane Mode.
      If TLV not available then previous setting will be available.
      Setting will be applicable till next reboot
    */
    /* type = uint8_t, refer to WIFI_APM_STATE */
    MIPC_WFC_WIFI_INFO_NTF_T_APM_STATE                      = 0x115,
};




#endif /* __MIPC_MSG_WFC_CONST_H__ */
