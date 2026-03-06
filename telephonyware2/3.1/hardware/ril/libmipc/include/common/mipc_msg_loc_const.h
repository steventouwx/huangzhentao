#ifndef __MIPC_MSG_LOC_CONST_H__
#define __MIPC_MSG_LOC_CONST_H__

enum MIPC_LOC_MSG_enum {
    MIPC_LOC_MSG_NONE = 0,
    /* AP GPS chip will use this command to update GPS status information to modem. */
    MIPC_LOC_AGPS_SYNC_STATUS_REQ                           = 5121,
    MIPC_LOC_AGPS_SYNC_STATUS_CNF                           = 5122,

    /* AP can use this command to send PMTK message to MD. */
    MIPC_LOC_AGPS_SEND_PMTK_REQ                             = 5123,
    MIPC_LOC_AGPS_SEND_PMTK_CNF                             = 5124,

    /*
      This command is used when modem boot up.
      AP will send this command to sync AP's satellites capabilities with MD.
    */
    MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ                     = 5125,
    MIPC_LOC_AGPS_SYNC_CAPABILITIES_CNF                     = 5126,

    /*
      AP can use this command with operation TIME_SYNC_REQ to request for sync MD time.
      After MD send MIPC_LOC_AGPS_MD_TIME_CMD_IND with operation MD_TIME_SYNC_IND, AP need to query this command again with operation TIME_SYNC_RSP.
    */
    MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ                     = 5127,
    MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_CNF                     = 5128,

    /* AP can use this command to send LBS config command to MD. */
    MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ                = 5129,
    MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF                = 5130,

    /* Ask modem to start/stop reporting debug information to AP. */
    MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_REQ             = 5131,
    MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_CNF             = 5132,

    /* AP can use this command to request for debug message of LBS service. */
    MIPC_LOC_LBS_GET_CUSTOM_DEBUG_REQ                       = 5133,
    MIPC_LOC_LBS_GET_CUSTOM_DEBUG_CNF                       = 5134,

    /*
      This command is provided for LPPe service in AP.
      AP will query this command to send information to MD based on the requested msg_type in MIPC_LOC_LBS_LPPE_SERVICE_IND.
    */
    MIPC_LOC_LBS_LPPE_SERVICE_REQ                           = 5135,
    MIPC_LOC_LBS_LPPE_SERVICE_CNF                           = 5136,

    /* AP can use this command to respond for the LPPe request/notification from MD. */
    MIPC_LOC_LBS_LPPE_SERVICE_FWK_REQ                       = 5137,
    MIPC_LOC_LBS_LPPE_SERVICE_FWK_CNF                       = 5138,

    /* On Android framework, some configuration needs to send to modem for LBS service. */
    MIPC_LOC_LBS_SYNC_AIDL_REQ                              = 5139,
    MIPC_LOC_LBS_SYNC_AIDL_CNF                              = 5140,

    /* AP can use this command to send response if received MIPC_LOC_AGPS_NI_IND from MD. */
    MIPC_LOC_AGPS_NI_REQ                                    = 5141,
    MIPC_LOC_AGPS_NI_CNF                                    = 5142,

    /* AP can use this command to request for aiding data. */
    MIPC_LOC_AGPS_QUERY_REAIDING_REQ                        = 5143,
    MIPC_LOC_AGPS_QUERY_REAIDING_CNF                        = 5144,

    /* AP can use this command to request for high accuracy GNSS assist data from MD. */
    MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_REQ               = 5145,
    MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_CNF               = 5146,

    /* AP can use this command to send ASN related request/response to MD. */
    MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ                   = 5147,
    MIPC_LOC_AGPS_SET_ASN_INFORMATION_CNF                   = 5148,

    /* AP GPS chip ask TSX (Temperature Sensor Crystal) information from modem. */
    MIPC_LOC_AGPS_GET_TSX_DATA_REQ                          = 5149,
    MIPC_LOC_AGPS_GET_TSX_DATA_CNF                          = 5150,

    /* AP MNLD send version information to MD. */
    MIPC_LOC_AGPS_SYNC_MNLD_VER_REQ                         = 5151,
    MIPC_LOC_AGPS_SYNC_MNLD_VER_CNF                         = 5152,

    /* Update debug information to engineer mode application in AP side. */
    MIPC_LOC_LBS_LOCATION_EM_MSG_IND                        = 21505,

    /* Update debug information to AP (special debug method for lead customer) */
    MIPC_LOC_LBS_CUSTOM_DEBUG_IND                           = 21506,

    /* This command is for MD to send request/notification to the LPPe service in AP. */
    MIPC_LOC_LBS_LPPE_SERVICE_IND                           = 21507,

    /* MD will use this command to send LPPe service framework request/notification to AP. */
    MIPC_LOC_LBS_LPPE_SERVICE_FWK_IND                       = 21508,

    /* Update/Request information to AP Android framework. */
    MIPC_LOC_LBS_AIDL_IND                                   = 21509,

    /* Synchronize and configure the GPS status. */
    MIPC_LOC_AGPS_STATUS_IND                                = 21510,

    /* MD will send this command to notify AP a Network INIT (NI) location request occur. */
    MIPC_LOC_AGPS_NI_IND                                    = 21511,

    /* MD will use this command to send AGPS location information to AP. */
    MIPC_LOC_AGPS_LOCATION_IND                              = 21512,

    /*
      If MD receive MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ with operation TIME_SYNC_REQ, MD will first send this command with operation MD_TIME_SYNC_CNF to inform AP that MD has received the REQ.
      After MD complete the time sync process, MD will query the MD time info via this IND with operation MD_TIME_SYNC_IND.
    */
    MIPC_LOC_AGPS_MD_TIME_CMD_IND                           = 21513,

    /* MD will use this command to send high accuracy GNSS assist data to AP. */
    MIPC_LOC_AGPS_HA_GNSS_ASSIST_DATA_IND                   = 21514,

    /* Modem provie TSX (Temperature Sensor Crystal) information to AP GPS chip. */
    MIPC_LOC_AGPS_TSX_IND                                   = 21515,

    /* MD will use this command to send ASN data to AP. */
    MIPC_LOC_AGPS_ASN_INFORMATION_IND                       = 21516,

    /*
      This command is used during the modem boot up flow.
      MD will send this command to sync MD's satellites capabilities with AP when operation is set to AGPS_VERSION_QUERY.
      If AP request for AGPS version by MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ, MD will used this command to respond AP with operation AGPS_VERSION_SYNC.
    */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND                     = 21517,

    /* MD will use this command to send PMTK message to AP. */
    MIPC_LOC_AGPS_PMTK_IND                                  = 21518,


};

    /* MIPC_MSG.LOC_AGPS_SYNC_STATUS_REQ */
enum mipc_loc_agps_sync_status_req_tlv_enum {
    mipc_loc_agps_sync_status_req_tlv_NONE = 0,
    /* GPS control operation */
    /* type = uint8_t, refer to GPS_STATUS_CONTROL_ENUM */
    MIPC_LOC_AGPS_SYNC_STATUS_REQ_T_OPERATION               = 0x100,
    /* when operation=GPS_OPEN, this field need to fill */
    /* type = int32_t */
    MIPC_LOC_AGPS_SYNC_STATUS_REQ_T_ASSIST_REQ              = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_SYNC_STATUS_CNF */
enum mipc_loc_agps_sync_status_cnf_tlv_enum {
    mipc_loc_agps_sync_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_SEND_PMTK_REQ */
enum mipc_loc_agps_send_pmtk_req_tlv_enum {
    mipc_loc_agps_send_pmtk_req_tlv_NONE = 0,
    /* type = byte_array */
    MIPC_LOC_AGPS_SEND_PMTK_REQ_T_PMTK                      = 0x100,
};

    /* MIPC_MSG.LOC_AGPS_SEND_PMTK_CNF */
enum mipc_loc_agps_send_pmtk_cnf_tlv_enum {
    mipc_loc_agps_send_pmtk_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_SYNC_CAPABILITIES_REQ */
enum mipc_loc_agps_sync_capabilities_req_tlv_enum {
    mipc_loc_agps_sync_capabilities_req_tlv_NONE = 0,
    /* GPS satellite support */
    /* type = int8_t */
    MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_GPS_SATELLITE_SUPPORT = 0x100,
    /* glonass satellite support */
    /* type = int8_t */
    MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_GLONASS_SATELLITE_SUPPORT = 0x101,
    /* BEIDOU satellite support */
    /* type = int8_t */
    MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_BEIDOU_SATELLITE_SUPPORT = 0x102,
    /* GEILEO satellite support */
    /* type = int8_t */
    MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ_T_GAILEO_SATELLITE_SUPPORT = 0x103,
};

    /* MIPC_MSG.LOC_AGPS_SYNC_CAPABILITIES_CNF */
enum mipc_loc_agps_sync_capabilities_cnf_tlv_enum {
    mipc_loc_agps_sync_capabilities_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_QUERY_MD_TIME_CMD_REQ */
enum mipc_loc_agps_query_md_time_cmd_req_tlv_enum {
    mipc_loc_agps_query_md_time_cmd_req_tlv_NONE = 0,
    /* type = uint8_t, refer to GPS_TIME_SYNC_OPERATION_ENUM */
    MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ_T_TIME_SYNC_OPERATION = 0x100,
    /* time sync data */
    /* type = byte_array */
    MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ_T_DATA              = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_QUERY_MD_TIME_CMD_CNF */
enum mipc_loc_agps_query_md_time_cmd_cnf_tlv_enum {
    mipc_loc_agps_query_md_time_cmd_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ */
enum mipc_loc_lbs_sync_location_em_config_req_tlv_enum {
    mipc_loc_lbs_sync_location_em_config_req_tlv_NONE = 0,
    /* Config Command */
    /* type = uint32_t */
    MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ_T_COMMAND      = 0x100,
    /* When length is 0, this field no need to fill. (config raw data max size 4300.) */
    /* type = byte_array */
    MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ_T_DATA         = 0x101,
};

    /* MIPC_MSG.LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF */
enum mipc_loc_lbs_sync_location_em_config_cnf_tlv_enum {
    mipc_loc_lbs_sync_location_em_config_cnf_tlv_NONE = 0,
    /* Always 0. */
    /* type = uint8_t */
    MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF_T_RESULT       = 0x100,
    /* When length is 0, this field no need to fill. (config raw data max size 4300.) */
    /* type = byte_array */
    MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF_T_DATA         = 0x101,
};

    /* MIPC_MSG.LOC_LBS_SET_LOCATION_EM_MSG_REPORT_REQ */
enum mipc_loc_lbs_set_location_em_msg_report_req_tlv_enum {
    mipc_loc_lbs_set_location_em_msg_report_req_tlv_NONE = 0,
    /* type = uint8_t, refer to LBS_EM_MSG_ENUM */
    MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_REQ_T_COMMAND   = 0x100,
};

    /* MIPC_MSG.LOC_LBS_SET_LOCATION_EM_MSG_REPORT_CNF */
enum mipc_loc_lbs_set_location_em_msg_report_cnf_tlv_enum {
    mipc_loc_lbs_set_location_em_msg_report_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_LBS_GET_CUSTOM_DEBUG_REQ */
enum mipc_loc_lbs_get_custom_debug_req_tlv_enum {
    mipc_loc_lbs_get_custom_debug_req_tlv_NONE = 0,
    /* type = uint8_t, refer to LBS_DEBUG_REQ_ENUM */
    MIPC_LOC_LBS_GET_CUSTOM_DEBUG_REQ_T_ACTION_TYPE         = 0x100,
    /* When the length is 0, no need to fill this field. */
    /* type = byte_array */
    MIPC_LOC_LBS_GET_CUSTOM_DEBUG_REQ_T_MSG_DATA            = 0x101,
};

    /* MIPC_MSG.LOC_LBS_GET_CUSTOM_DEBUG_CNF */
enum mipc_loc_lbs_get_custom_debug_cnf_tlv_enum {
    mipc_loc_lbs_get_custom_debug_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_LBS_LPPE_SERVICE_REQ */
enum mipc_loc_lbs_lppe_service_req_tlv_enum {
    mipc_loc_lbs_lppe_service_req_tlv_NONE = 0,
    /* type = uint8_t, refer to LPPE_PROTOCOL_ENUM */
    MIPC_LOC_LBS_LPPE_SERVICE_REQ_T_PROTOCOL_TYPE           = 0x100,
    /* type = uint8_t, refer to LPPE_MSG_ACTION_ENUM */
    MIPC_LOC_LBS_LPPE_SERVICE_REQ_T_MSG_TYPE                = 0x101,
    /* msg data */
    /* type = byte_array */
    MIPC_LOC_LBS_LPPE_SERVICE_REQ_T_MSG_DATA                = 0x102,
};

    /* MIPC_MSG.LOC_LBS_LPPE_SERVICE_CNF */
enum mipc_loc_lbs_lppe_service_cnf_tlv_enum {
    mipc_loc_lbs_lppe_service_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_LBS_LPPE_SERVICE_FWK_REQ */
enum mipc_loc_lbs_lppe_service_fwk_req_tlv_enum {
    mipc_loc_lbs_lppe_service_fwk_req_tlv_NONE = 0,
    /* type = uint8_t, refer to LPPE_SUPL_REQ_ACTION_ENUM */
    MIPC_LOC_LBS_LPPE_SERVICE_FWK_REQ_T_ACTION_TYPE         = 0x100,
    /* msg data */
    /* type = byte_array */
    MIPC_LOC_LBS_LPPE_SERVICE_FWK_REQ_T_MSG_DATA            = 0x101,
};

    /* MIPC_MSG.LOC_LBS_LPPE_SERVICE_FWK_CNF */
enum mipc_loc_lbs_lppe_service_fwk_cnf_tlv_enum {
    mipc_loc_lbs_lppe_service_fwk_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_LBS_SYNC_AIDL_REQ */
enum mipc_loc_lbs_sync_aidl_req_tlv_enum {
    mipc_loc_lbs_sync_aidl_req_tlv_NONE = 0,
    /* type = uint8_t, refer to LBS_AIDL_REQ_ACTION_ENUM */
    MIPC_LOC_LBS_SYNC_AIDL_REQ_T_ACTION_TYPE                = 0x100,
    /* msg data */
    /* type = byte_array */
    MIPC_LOC_LBS_SYNC_AIDL_REQ_T_MSG_DATA                   = 0x101,
};

    /* MIPC_MSG.LOC_LBS_SYNC_AIDL_CNF */
enum mipc_loc_lbs_sync_aidl_cnf_tlv_enum {
    mipc_loc_lbs_sync_aidl_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_NI_REQ */
enum mipc_loc_agps_ni_req_tlv_enum {
    mipc_loc_agps_ni_req_tlv_NONE = 0,
    /* type = int32_t */
    MIPC_LOC_AGPS_NI_REQ_T_SESSION_ID                       = 0x100,
    /* type = int32_t */
    MIPC_LOC_AGPS_NI_REQ_T_USER_RESPONSE                    = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_NI_CNF */
enum mipc_loc_agps_ni_cnf_tlv_enum {
    mipc_loc_agps_ni_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_QUERY_REAIDING_REQ */
enum mipc_loc_agps_query_reaiding_req_tlv_enum {
    mipc_loc_agps_query_reaiding_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_QUERY_REAIDING_CNF */
enum mipc_loc_agps_query_reaiding_cnf_tlv_enum {
    mipc_loc_agps_query_reaiding_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_REQ */
enum mipc_loc_agps_get_ha_gnss_assist_data_req_tlv_enum {
    mipc_loc_agps_get_ha_gnss_assist_data_req_tlv_NONE = 0,
    /* data */
    /* type = byte_array */
    MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_REQ_T_DATA        = 0x100,
};

    /* MIPC_MSG.LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_CNF */
enum mipc_loc_agps_get_ha_gnss_assist_data_cnf_tlv_enum {
    mipc_loc_agps_get_ha_gnss_assist_data_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_SET_ASN_INFORMATION_REQ */
enum mipc_loc_agps_set_asn_information_req_tlv_enum {
    mipc_loc_agps_set_asn_information_req_tlv_NONE = 0,
    /* type = uint8_t, refer to GPS_ASN_INFO_ACTION_ENUM */
    MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ_T_ACTION_TYPE     = 0x100,
    /* data */
    /* type = byte_array */
    MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ_T_DATA            = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_SET_ASN_INFORMATION_CNF */
enum mipc_loc_agps_set_asn_information_cnf_tlv_enum {
    mipc_loc_agps_set_asn_information_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_GET_TSX_DATA_REQ */
enum mipc_loc_agps_get_tsx_data_req_tlv_enum {
    mipc_loc_agps_get_tsx_data_req_tlv_NONE = 0,
    /* type = uint8_t, refer to GPS_TSX_DATA_ACTION_ENUM */
    MIPC_LOC_AGPS_GET_TSX_DATA_REQ_T_ACTION_TYPE            = 0x100,
    /* data */
    /* type = byte_array */
    MIPC_LOC_AGPS_GET_TSX_DATA_REQ_T_DATA                   = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_GET_TSX_DATA_CNF */
enum mipc_loc_agps_get_tsx_data_cnf_tlv_enum {
    mipc_loc_agps_get_tsx_data_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_AGPS_SYNC_MNLD_VER_REQ */
enum mipc_loc_agps_sync_mnld_ver_req_tlv_enum {
    mipc_loc_agps_sync_mnld_ver_req_tlv_NONE = 0,
    /* data */
    /* type = byte_array */
    MIPC_LOC_AGPS_SYNC_MNLD_VER_REQ_T_DATA                  = 0x100,
};

    /* MIPC_MSG.LOC_AGPS_SYNC_MNLD_VER_CNF */
enum mipc_loc_agps_sync_mnld_ver_cnf_tlv_enum {
    mipc_loc_agps_sync_mnld_ver_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.LOC_LBS_LOCATION_EM_MSG_IND */
enum mipc_loc_lbs_location_em_msg_ind_tlv_enum {
    mipc_loc_lbs_location_em_msg_ind_tlv_NONE = 0,
    /* 1: Toast, 2: View, 4: Dialog, 8: Vzw debug Message, 16: NI Statistic Message */
    /* type = uint32_t */
    MIPC_LOC_LBS_LOCATION_EM_MSG_IND_T_MSG_TYPE             = 0x100,
    /* MIP */
    /* type = byte_array */
    MIPC_LOC_LBS_LOCATION_EM_MSG_IND_T_MSG_DATA             = 0x101,
};

    /* MIPC_MSG.LOC_LBS_CUSTOM_DEBUG_IND */
enum mipc_loc_lbs_custom_debug_ind_tlv_enum {
    mipc_loc_lbs_custom_debug_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to LBS_DEBUG_IND_ENUM */
    MIPC_LOC_LBS_CUSTOM_DEBUG_IND_T_ACTION_TYPE             = 0x100,
    /* When the length is 0, no need to fill this field. */
    /* type = byte_array */
    MIPC_LOC_LBS_CUSTOM_DEBUG_IND_T_MSG_DATA                = 0x101,
};

    /* MIPC_MSG.LOC_LBS_LPPE_SERVICE_IND */
enum mipc_loc_lbs_lppe_service_ind_tlv_enum {
    mipc_loc_lbs_lppe_service_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to LPPE_PROTOCOL_ENUM */
    MIPC_LOC_LBS_LPPE_SERVICE_IND_T_PROTOCOL_TYPE           = 0x100,
    /* type = uint8_t, refer to LPPE_MSG_ACTION_ENUM */
    MIPC_LOC_LBS_LPPE_SERVICE_IND_T_MSG_TYPE                = 0x101,
    /* msg data */
    /* type = byte_array */
    MIPC_LOC_LBS_LPPE_SERVICE_IND_T_MSG_DATA                = 0x102,
};

    /* MIPC_MSG.LOC_LBS_LPPE_SERVICE_FWK_IND */
enum mipc_loc_lbs_lppe_service_fwk_ind_tlv_enum {
    mipc_loc_lbs_lppe_service_fwk_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to LPPE_SUPL_IND_ACTION_ENUM */
    MIPC_LOC_LBS_LPPE_SERVICE_FWK_IND_T_ACTION_TYPE         = 0x100,
    /* msg data */
    /* type = byte_array */
    MIPC_LOC_LBS_LPPE_SERVICE_FWK_IND_T_MSG_DATA            = 0x101,
};

    /* MIPC_MSG.LOC_LBS_AIDL_IND */
enum mipc_loc_lbs_aidl_ind_tlv_enum {
    mipc_loc_lbs_aidl_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to LBS_AIDL_IND_ACTION_ENUM */
    MIPC_LOC_LBS_AIDL_IND_T_ACTION_TYPE                     = 0x100,
    /* msg data */
    /* type = byte_array */
    MIPC_LOC_LBS_AIDL_IND_T_MSG_DATA                        = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_STATUS_IND */
enum mipc_loc_agps_status_ind_tlv_enum {
    mipc_loc_agps_status_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to AGPS_STATUS_CMD_ENUM */
    MIPC_LOC_AGPS_STATUS_IND_T_OPERATION                    = 0x100,
    /* need to fill when operation = 1 */
    /* type = int32_t */
    MIPC_LOC_AGPS_STATUS_IND_T_SHOW_GPS_ICON                = 0x101,
    /*
      need to fill when operation = 1 or 2
      This enum indicates the type of this request.
    */
    /* type = uint8_t, refer to MNL_AGPS_OPEN_TYPE_ENUM */
    MIPC_LOC_AGPS_STATUS_IND_T_OPEN_TYPE                    = 0x102,
    /*
      need to fill when operation = 1 or 2
      This enum indicates the source of this request.
    */
    /* type = uint8_t, refer to MNL_AGPS_OPEN_REQUESTOR_ENUM */
    MIPC_LOC_AGPS_STATUS_IND_T_REQUESTOR                    = 0x103,
    /* need to fill when operation = 1 or 2 */
    /* type = byte_array */
    MIPC_LOC_AGPS_STATUS_IND_T_REQUESTOR_ID                 = 0x104,
    /* need to fill when operation = 1 or 2 */
    /* type = uint8_t */
    MIPC_LOC_AGPS_STATUS_IND_T_EMERGENCY_CALL               = 0x105,
    /* need to fill when operation = 4 */
    /* type = int32_t */
    MIPC_LOC_AGPS_STATUS_IND_T_FLAG                         = 0x106,
};

    /* MIPC_MSG.LOC_AGPS_NI_IND */
enum mipc_loc_agps_ni_ind_tlv_enum {
    mipc_loc_agps_ni_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to MNL_AGPS_NI_NOTIFY_TYPE_ENUM */
    MIPC_LOC_AGPS_NI_IND_T_OPERATION                        = 0x100,
    /* type = int32_t */
    MIPC_LOC_AGPS_NI_IND_T_SESSION_ID                       = 0x101,
    /* type = uint8_t, refer to MNL_AGPS_NI_TYPE_ENUM */
    MIPC_LOC_AGPS_NI_IND_T_NI_TYPE                          = 0x102,
    /* type = byte_array */
    MIPC_LOC_AGPS_NI_IND_T_REQUESTOR_ID                     = 0x103,
    /* type = byte_array */
    MIPC_LOC_AGPS_NI_IND_T_CLIENT_NAME                      = 0x104,
    /* need to fill when opertion = 1 */
    /* type = uint8_t, refer to MNL_AGPS_NI_ENCODING_TYPE_ENUM */
    MIPC_LOC_AGPS_NI_IND_T_REQ_ID_ENCODING                  = 0x105,
    /* need to fill when opertion = 1 */
    /* type = uint8_t, refer to MNL_AGPS_NI_ENCODING_TYPE_ENUM */
    MIPC_LOC_AGPS_NI_IND_T_CL_NAME_ENCODING                 = 0x106,
};

    /* MIPC_MSG.LOC_AGPS_LOCATION_IND */
enum mipc_loc_agps_location_ind_tlv_enum {
    mipc_loc_agps_location_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to MNL_AGPS_LOCATION_CONTROL_ENUM */
    MIPC_LOC_AGPS_LOCATION_IND_T_OPERATION                  = 0x100,
    /* Represents latitude in degrees */
    /* type = byte_array */
    MIPC_LOC_AGPS_LOCATION_IND_T_LATITUDE                   = 0x101,
    /* Represents longitude in degrees */
    /* type = byte_array */
    MIPC_LOC_AGPS_LOCATION_IND_T_LONGITUDE                  = 0x102,
    /* disable: 0, enable: 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_LOCATION_IND_T_ALTITUDE_USED              = 0x103,
    /* Represents altitude in meters above the WGS 84 reference */
    /* type = byte_array */
    MIPC_LOC_AGPS_LOCATION_IND_T_ALTITUDE                   = 0x104,
    /* disable: 0, enable: 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_LOCATION_IND_T_SPEED_USED                 = 0x105,
    /* Represents speed in meters per second */
    /* type = byte_array */
    MIPC_LOC_AGPS_LOCATION_IND_T_SPEED                      = 0x106,
    /* disable: 0, enable: 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_LOCATION_IND_T_BEARING_USED               = 0x107,
    /* Represents bearing in degrees */
    /* type = byte_array */
    MIPC_LOC_AGPS_LOCATION_IND_T_BEARING                    = 0x108,
    /* disable: 0, enable: 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_LOCATION_IND_T_ACCURACY_USED              = 0x109,
    /* type = byte_array */
    MIPC_LOC_AGPS_LOCATION_IND_T_ACCURACY                   = 0x10A,
    /* disable: 0, enable: 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_LOCATION_IND_T_TIMESTAMP_USED             = 0x10B,
    /* Milliseconds since January 1, 1970 */
    /* type = byte_array */
    MIPC_LOC_AGPS_LOCATION_IND_T_TIMESTAMP                  = 0x10C,
    /* disable: 0, enable: 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_LOCATION_IND_T_SOURCE_USED                = 0x10D,
    /* type = uint8_t, refer to MNL_AGPS_LOCATION_TYPE_ENUM */
    MIPC_LOC_AGPS_LOCATION_IND_T_SOURCE                     = 0x10E,
};

    /* MIPC_MSG.LOC_AGPS_MD_TIME_CMD_IND */
enum mipc_loc_agps_md_time_cmd_ind_tlv_enum {
    mipc_loc_agps_md_time_cmd_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to AGPS_MD_TIME_ACTION_ENUM */
    MIPC_LOC_AGPS_MD_TIME_CMD_IND_T_OPERATION               = 0x100,
    /* data */
    /* type = byte_array */
    MIPC_LOC_AGPS_MD_TIME_CMD_IND_T_DATA                    = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_HA_GNSS_ASSIST_DATA_IND */
enum mipc_loc_agps_ha_gnss_assist_data_ind_tlv_enum {
    mipc_loc_agps_ha_gnss_assist_data_ind_tlv_NONE = 0,
    /* data */
    /* type = byte_array */
    MIPC_LOC_AGPS_HA_GNSS_ASSIST_DATA_IND_T_DATA            = 0x100,
};

    /* MIPC_MSG.LOC_AGPS_TSX_IND */
enum mipc_loc_agps_tsx_ind_tlv_enum {
    mipc_loc_agps_tsx_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to AGPS_TSX_DATA_ACTION_ENUM */
    MIPC_LOC_AGPS_TSX_IND_T_OPERATION                       = 0x100,
    /* data */
    /* type = byte_array */
    MIPC_LOC_AGPS_TSX_IND_T_DATA                            = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_ASN_INFORMATION_IND */
enum mipc_loc_agps_asn_information_ind_tlv_enum {
    mipc_loc_agps_asn_information_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to AGPS_ASN_DATA_TYPE_ENUM */
    MIPC_LOC_AGPS_ASN_INFORMATION_IND_T_ASN_TYPE            = 0x100,
    /* type = byte_array */
    MIPC_LOC_AGPS_ASN_INFORMATION_IND_T_ASN1                = 0x101,
};

    /* MIPC_MSG.LOC_AGPS_CAPABILITIES_SYNC_IND */
enum mipc_loc_agps_capabilities_sync_ind_tlv_enum {
    mipc_loc_agps_capabilities_sync_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to AGPS_VERSION_ACTION_ENUM */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_OPERATION         = 0x100,
    /* need to fill when operation = 0 */
    /* type = int32_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_VERSION           = 0x101,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_SIB8_16_ENABLE    = 0x102,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_GPS_SATELLITE_ENABLE = 0x103,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_GLONASS_SATELLITE_ENABLE = 0x104,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_BEIDOU_SATELLITE_ENABLE = 0x105,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_GALILEO_SATELLITE_ENABLE = 0x106,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_GPS_SATELLITE_ENABLE = 0x107,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_GLONASS_SATELLITE_ENABLE = 0x108,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_BEIDOU_SATELLITE_ENABLE = 0x109,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_GALILEO_SATELLITE_ENABLE = 0x10A,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_LPPE_ENABLE       = 0x10B,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_QZSS_SATELLITE_ENABLE = 0x10C,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_NAVIC_SATELLITE_ENABLE = 0x10D,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_QZSS_SATELLITE_ENABLE = 0x10E,
    /* need to fill when operation = 1 */
    /* type = int8_t */
    MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND_T_A_NAVIC_SATELLITE_ENABLE = 0x10F,
};

    /* MIPC_MSG.LOC_AGPS_PMTK_IND */
enum mipc_loc_agps_pmtk_ind_tlv_enum {
    mipc_loc_agps_pmtk_ind_tlv_NONE = 0,
    /* type = byte_array */
    MIPC_LOC_AGPS_PMTK_IND_T_PMTK                           = 0x100,
};




#endif /* __MIPC_MSG_LOC_CONST_H__ */
