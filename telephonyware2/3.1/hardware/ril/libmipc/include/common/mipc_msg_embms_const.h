#ifndef __MIPC_MSG_EMBMS_CONST_H__
#define __MIPC_MSG_EMBMS_CONST_H__

enum MIPC_EMBMS_MSG_enum {
    MIPC_EMBMS_MSG_NONE = 0,
    /* get the latest information regarding the MBMS sessions available in the current MBSFN req. */
    MIPC_EMBMS_EMSLU_REQ                                    = 3328,
    MIPC_EMBMS_EMSLU_CNF                                    = 3329,

    /* get the latest SAI list req. */
    MIPC_EMBMS_GET_SAI_LIST_REQ                             = 3330,
    MIPC_EMBMS_GET_SAI_LIST_CNF                             = 3331,

    /* Notify MBMSAL the beginning or the end of the hVoLTE scenarios req. */
    MIPC_EMBMS_NOTIFY_HVOLTE_STATUS_REQ                     = 3332,
    MIPC_EMBMS_NOTIFY_HVOLTE_STATUS_CNF                     = 3333,

    /* Set service enable status. */
    MIPC_EMBMS_SET_SERVICE_ENABLE_REQ                       = 3334,
    MIPC_EMBMS_SET_SERVICE_ENABLE_CNF                       = 3335,

    /* Get service enable status. */
    MIPC_EMBMS_GET_SERVICE_ENABLE_REQ                       = 3336,
    MIPC_EMBMS_GET_SERVICE_ENABLE_CNF                       = 3337,

    /* To config broadcast status. */
    MIPC_EMBMS_SET_BROADCAST_CONFIG_REQ                     = 3338,
    MIPC_EMBMS_SET_BROADCAST_CONFIG_CNF                     = 3339,

    /* To update mbs session. */
    MIPC_EMBMS_UPDATE_MBS_SESSION_REQ                       = 3340,
    MIPC_EMBMS_UPDATE_MBS_SESSION_CNF                       = 3341,

    /* To query multicast remain time. */
    MIPC_EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_REQ          = 3342,
    MIPC_EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_CNF          = 3343,

    /* This interface is used to report any changes in eMBMS service coverage status to host. */
    MIPC_EMBMS_EMSRV_IND                                    = 19712,

    /* This interface is used to report MBMS sessions list update information to host. */
    MIPC_EMBMS_EMSLUI_IND                                   = 19713,

    /* This interface is used to report neighbouring frequency with bands list and MBMS SAI list to host. */
    MIPC_EMBMS_EMSAILNF_IND                                 = 19714,

    /* This interface is used to report the status of session to host,during session activation and deactivation. */
    MIPC_EMBMS_EMSESS_IND                                   = 19715,

    /* This interface is used to report the hVoLTE mode to host. */
    MIPC_EMBMS_EHVOLTE_IND                                  = 19716,

    /* This interface is used to report the service coverage status. */
    MIPC_EMBMS_SERVICE_COVERAGE_IND                         = 19717,

    /* This interface is used to report the broadcast session status. */
    MIPC_EMBMS_BROADCAST_SESSION_LIST_IND                   = 19718,

    /* This interface is used to report the mbs_freq_info. */
    MIPC_EMBMS_MBS_FREQ_IND                                 = 19719,

    /* This interface is used to report the session status. */
    MIPC_EMBMS_SESSION_STATUS_IND                           = 19720,

    /* This interface is used to report the sai intra list. */
    MIPC_EMBMS_SAI_INTRA_LIST_IND                           = 19721,

    /* This interface is used to report the sai neighbor list. */
    MIPC_EMBMS_SAI_NEIGHBOR_LIST_IND                        = 19722,

    /* This interface is used to report the multicast area update info . */
    MIPC_EMBMS_MBS_MULTICAST_AREA_INFO_UPDATE_IND           = 19723,

    /* This interface is used to report the SESSION update info . */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND                       = 19724,


};

    /* MIPC_MSG.EMBMS_EMSLU_REQ */
enum mipc_embms_emslu_req_tlv_enum {
    mipc_embms_emslu_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EMBMS_EMSLU_CNF */
enum mipc_embms_emslu_cnf_tlv_enum {
    mipc_embms_emslu_cnf_tlv_NONE = 0,
    /* 0: disable eMBMS Session List uptate information, 1: enable eMBMS Session List uptate information. */
    /* type = uint8_t */
    MIPC_EMBMS_EMSLU_CNF_T_IS_ENABLED                       = 0x100,
    /* number of MBMS sessions in the list. */
    /* type = uint16_t */
    MIPC_EMBMS_EMSLU_CNF_T_SESSION_COUNT                    = 0x101,
    /* sessions list element. */
    /* type = struct_array, refer to embms_session_info */
    MIPC_EMBMS_EMSLU_CNF_T_SESSION_LIST                     = 0x102,
    /* sessions list element. */
    /* type = struct, refer to embms_session_info */
    MIPC_EMBMS_EMSLU_CNF_T_SESSION_TLV_ARRAY                = 0x103,
};

    /* MIPC_MSG.EMBMS_GET_SAI_LIST_REQ */
enum mipc_embms_get_sai_list_req_tlv_enum {
    mipc_embms_get_sai_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EMBMS_GET_SAI_LIST_CNF */
enum mipc_embms_get_sai_list_cnf_tlv_enum {
    mipc_embms_get_sai_list_cnf_tlv_NONE = 0,
    /* 0: disable eMBMS Session List uptate information, 1: enable eMBMS Session List uptate information. */
    /* type = uint8_t */
    MIPC_EMBMS_GET_SAI_LIST_CNF_T_IS_ENABLED                = 0x100,
    /* The MBMS SAI info of the current intra frequencies */
    /* type = struct, refer to embms_sai_cf_info */
    MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_CF_INFO               = 0x101,
    /* The number of neighboring frequencies. Range is from 0 to 8. */
    /* type = uint8_t */
    MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_NF_COUNT              = 0x102,
    /* List of neighbouring frequencies and their bands,if any, with their SAI list. */
    /* type = struct_array, refer to embms_sai_nf_info */
    MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_NF_LIST               = 0x103,
    /* List of neighbouring frequencies and their bands,if any, with their SAI list. */
    /* type = struct, refer to embms_sai_nf_info */
    MIPC_EMBMS_GET_SAI_LIST_CNF_T_SAI_NF_TLV_ARRAY          = 0x104,
};

    /* MIPC_MSG.EMBMS_NOTIFY_HVOLTE_STATUS_REQ */
enum mipc_embms_notify_hvolte_status_req_tlv_enum {
    mipc_embms_notify_hvolte_status_req_tlv_NONE = 0,
    /* the status of the hVoLTE scenarios. 0: the end of the scenario, 1: The beginning of the scenario. */
    /* type = uint8_t */
    MIPC_EMBMS_NOTIFY_HVOLTE_STATUS_REQ_T_STATUS            = 0x100,
};

    /* MIPC_MSG.EMBMS_NOTIFY_HVOLTE_STATUS_CNF */
enum mipc_embms_notify_hvolte_status_cnf_tlv_enum {
    mipc_embms_notify_hvolte_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EMBMS_SET_SERVICE_ENABLE_REQ */
enum mipc_embms_set_service_enable_req_tlv_enum {
    mipc_embms_set_service_enable_req_tlv_NONE = 0,
    /* The broadcast enable status.0-disable; 1-enable */
    /* type = uint8_t */
    MIPC_EMBMS_SET_SERVICE_ENABLE_REQ_T_BROADCAST_ENABLE    = 0x100,
    /* The multicast enable status.0-disable; 1-enable */
    /* type = uint8_t */
    MIPC_EMBMS_SET_SERVICE_ENABLE_REQ_T_MULTICAST_ENABLE    = 0x101,
};

    /* MIPC_MSG.EMBMS_SET_SERVICE_ENABLE_CNF */
enum mipc_embms_set_service_enable_cnf_tlv_enum {
    mipc_embms_set_service_enable_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EMBMS_GET_SERVICE_ENABLE_REQ */
enum mipc_embms_get_service_enable_req_tlv_enum {
    mipc_embms_get_service_enable_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EMBMS_GET_SERVICE_ENABLE_CNF */
enum mipc_embms_get_service_enable_cnf_tlv_enum {
    mipc_embms_get_service_enable_cnf_tlv_NONE = 0,
    /* The broadcast enable status.0-disable; 1-enable */
    /* type = uint8_t */
    MIPC_EMBMS_GET_SERVICE_ENABLE_CNF_T_BROADCAST_ENABLE    = 0x100,
    /* The multicast enable status.0-disable; 1-enable */
    /* type = uint8_t */
    MIPC_EMBMS_GET_SERVICE_ENABLE_CNF_T_MULTICAST_ENABLE    = 0x101,
};

    /* MIPC_MSG.EMBMS_SET_BROADCAST_CONFIG_REQ */
enum mipc_embms_set_broadcast_config_req_tlv_enum {
    mipc_embms_set_broadcast_config_req_tlv_NONE = 0,
    /* The action of the broadcast config,1-activate, 0-deactivate */
    /* type = uint8_t */
    MIPC_EMBMS_SET_BROADCAST_CONFIG_REQ_T_ACTION            = 0x100,
    /* The tmgi info of the broadcast config */
    /* type = struct, refer to mbms_tmgi_info */
    MIPC_EMBMS_SET_BROADCAST_CONFIG_REQ_T_TMGI              = 0x101,
    /* The usd fsai info of the broadcast config */
    /* type = struct, refer to mbms_usd_fsai_info */
    MIPC_EMBMS_SET_BROADCAST_CONFIG_REQ_T_USD_FSAI          = 0x102,
};

    /* MIPC_MSG.EMBMS_SET_BROADCAST_CONFIG_CNF */
enum mipc_embms_set_broadcast_config_cnf_tlv_enum {
    mipc_embms_set_broadcast_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EMBMS_UPDATE_MBS_SESSION_REQ */
enum mipc_embms_update_mbs_session_req_tlv_enum {
    mipc_embms_update_mbs_session_req_tlv_NONE = 0,
    /* The mbs option of the interface */
    /* type = uint8_t */
    MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_MBS_OPTION          = 0x100,
    /* The session ID type */
    /* type = uint8_t */
    MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_SESSION_ID_TYPE     = 0x101,
    /* The tmgi of the session update info */
    /* type = struct, refer to mbms_tmgi_info */
    MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_TMGI                = 0x102,
    /* The ip address of the session update info */
    /* type = struct, refer to mbms_ip_address_info */
    MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_IP_ADDRESS          = 0x103,
    /* The CELL_ID of the MBS session update */
    /* type = struct, refer to mbms_cell_id_info */
    MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_CELL_ID_TLV_ARRAY   = 0x104,
    /* The tac of the MBS session update */
    /* type = struct, refer to mbms_tac_info */
    MIPC_EMBMS_UPDATE_MBS_SESSION_REQ_T_TAC_TLV_ARRAY       = 0x105,
};

    /* MIPC_MSG.EMBMS_UPDATE_MBS_SESSION_CNF */
enum mipc_embms_update_mbs_session_cnf_tlv_enum {
    mipc_embms_update_mbs_session_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_REQ */
enum mipc_embms_query_mbs_multicast_remain_time_req_tlv_enum {
    mipc_embms_query_mbs_multicast_remain_time_req_tlv_NONE = 0,
    /* The tgmi of the interface */
    /* type = struct, refer to mbms_tmgi_info */
    MIPC_EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_REQ_T_TGMI   = 0x100,
};

    /* MIPC_MSG.EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_CNF */
enum mipc_embms_query_mbs_multicast_remain_time_cnf_tlv_enum {
    mipc_embms_query_mbs_multicast_remain_time_cnf_tlv_NONE = 0,
    /* The MBS multicast session of Timer remain time. */
    /* type = uint8_t */
    MIPC_EMBMS_QUERY_MBS_MULTICAST_REMAIN_TIME_CNF_T_REMAINING_TIME = 0x100,
};

    /* MIPC_MSG.EMBMS_EMSRV_IND */
enum mipc_embms_emsrv_ind_tlv_enum {
    mipc_embms_emsrv_ind_tlv_NONE = 0,
    /* The eMBMS service status, 0:No LTE service, 1:Only Unicast available(LTE coverage), 2:In eMBMS supporting area, 3:E911 call is on-going, 4:In SRLTE mode, 5:In flight mode */
    /* type = uint8_t */
    MIPC_EMBMS_EMSRV_IND_T_STATUS                           = 0x100,
    /* The MBSFN area ID info. */
    /* type = struct, refer to embms_area_id_info */
    MIPC_EMBMS_EMSRV_IND_T_AREA_ID_INFO                     = 0x101,
};

    /* MIPC_MSG.EMBMS_EMSLUI_IND */
enum mipc_embms_emslui_ind_tlv_enum {
    mipc_embms_emslui_ind_tlv_NONE = 0,
    /* The number of MBMS sessions in the list. */
    /* type = uint16_t */
    MIPC_EMBMS_EMSLUI_IND_T_NUM_SESSIONS                    = 0x100,
    /* The MBMS session info. */
    /* type = struct, refer to embms_session_info */
    MIPC_EMBMS_EMSLUI_IND_T_SESSIONS_INFO                   = 0x101,
};

    /* MIPC_MSG.EMBMS_EMSAILNF_IND */
enum mipc_embms_emsailnf_ind_tlv_enum {
    mipc_embms_emsailnf_ind_tlv_NONE = 0,
    /* The neighbouring frequency with bands list and MBMS SAI list. */
    /* type = struct, refer to embms_nb_req_info */
    MIPC_EMBMS_EMSAILNF_IND_T_MBMS_NB_FREQ_INFO             = 0x100,
};

    /* MIPC_MSG.EMBMS_EMSESS_IND */
enum mipc_embms_emsess_ind_tlv_enum {
    mipc_embms_emsess_ind_tlv_NONE = 0,
    /* The number of MBMS sessions in the list. */
    /* type = uint16_t */
    MIPC_EMBMS_EMSESS_IND_T_NUM_SESSIONS                    = 0x100,
    /* The MBMS session info. */
    /* type = struct, refer to embms_session_info */
    MIPC_EMBMS_EMSESS_IND_T_MBMS_SESSION_INFO               = 0x101,
    /* The cause for session deactivation. */
    /* type = uint8_t */
    MIPC_EMBMS_EMSESS_IND_T_CAUSE                           = 0x102,
    /* The sub-cause for activating session failed when <cause> is 1. */
    /* type = uint8_t */
    MIPC_EMBMS_EMSESS_IND_T_SUB_CAUSE                       = 0x103,
};

    /* MIPC_MSG.EMBMS_EHVOLTE_IND */
enum mipc_embms_ehvolte_ind_tlv_enum {
    mipc_embms_ehvolte_ind_tlv_NONE = 0,
    /* The hVolte mode. */
    /* type = uint8_t */
    MIPC_EMBMS_EHVOLTE_IND_T_MODE                           = 0x100,
};

    /* MIPC_MSG.EMBMS_SERVICE_COVERAGE_IND */
enum mipc_embms_service_coverage_ind_tlv_enum {
    mipc_embms_service_coverage_ind_tlv_NONE = 0,
    /* The mbms status. */
    /* type = uint8_t */
    MIPC_EMBMS_SERVICE_COVERAGE_IND_T_MBMS_STATUS           = 0x100,
};

    /* MIPC_MSG.EMBMS_BROADCAST_SESSION_LIST_IND */
enum mipc_embms_broadcast_session_list_ind_tlv_enum {
    mipc_embms_broadcast_session_list_ind_tlv_NONE = 0,
    /* The mbms session status */
    /* type = struct, refer to mbms_session_status */
    MIPC_EMBMS_BROADCAST_SESSION_LIST_IND_T_SESSION_STATUS_TLV_ARRAY = 0x100,
};

    /* MIPC_MSG.EMBMS_MBS_FREQ_IND */
enum mipc_embms_mbs_freq_ind_tlv_enum {
    mipc_embms_mbs_freq_ind_tlv_NONE = 0,
    /* The mbms cell info */
    /* type = struct, refer to mbms_freq_info */
    MIPC_EMBMS_MBS_FREQ_IND_T_FREQ_TLV_ARRAY                = 0x100,
};

    /* MIPC_MSG.EMBMS_SESSION_STATUS_IND */
enum mipc_embms_session_status_ind_tlv_enum {
    mipc_embms_session_status_ind_tlv_NONE = 0,
    /* The service status of the session */
    /* type = uint8_t */
    MIPC_EMBMS_SESSION_STATUS_IND_T_SERVICE_STATUS          = 0x100,
    /* The tmgi struct of the session */
    /* type = struct, refer to mbms_tmgi_info */
    MIPC_EMBMS_SESSION_STATUS_IND_T_TMGI_INFO               = 0x101,
};

    /* MIPC_MSG.EMBMS_SAI_INTRA_LIST_IND */
enum mipc_embms_sai_intra_list_ind_tlv_enum {
    mipc_embms_sai_intra_list_ind_tlv_NONE = 0,
    /* The usd fsai info of the interface */
    /* type = struct, refer to mbms_usd_fsai_info */
    MIPC_EMBMS_SAI_INTRA_LIST_IND_T_USD_FSAI                = 0x100,
};

    /* MIPC_MSG.EMBMS_SAI_NEIGHBOR_LIST_IND */
enum mipc_embms_sai_neighbor_list_ind_tlv_enum {
    mipc_embms_sai_neighbor_list_ind_tlv_NONE = 0,
    /* The usd fsai info of the interface */
    /* type = struct, refer to mbms_fsai_neighbor_info */
    MIPC_EMBMS_SAI_NEIGHBOR_LIST_IND_T_FSAI_NEIGHBOR_TLV_ARRAY = 0x100,
};

    /* MIPC_MSG.EMBMS_MBS_MULTICAST_AREA_INFO_UPDATE_IND */
enum mipc_embms_mbs_multicast_area_info_update_ind_tlv_enum {
    mipc_embms_mbs_multicast_area_info_update_ind_tlv_NONE = 0,
    /* The tmgi info of the interface */
    /* type = struct, refer to mbms_tmgi_info */
    MIPC_EMBMS_MBS_MULTICAST_AREA_INFO_UPDATE_IND_T_TMGI_TLV_ARRAY = 0x100,
    /* The area info of the interface */
    /* type = byte_array */
    MIPC_EMBMS_MBS_MULTICAST_AREA_INFO_UPDATE_IND_T_AREA_INFO_LIST = 0x101,
};

    /* MIPC_MSG.EMBMS_MBS_SESSION_UPDATE_IND */
enum mipc_embms_mbs_session_update_ind_tlv_enum {
    mipc_embms_mbs_session_update_ind_tlv_NONE = 0,
    /* The tmgi info of the interface */
    /* type = struct, refer to mbms_tmgi_info */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_TMGI_INFO           = 0x100,
    /* The psi of the interface */
    /* type = uint8_t */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_PSI                 = 0x101,
    /* The ip address of the session update info */
    /* type = struct, refer to mbms_ip_address_info */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_IP_ADDRESS          = 0x102,
    /* The decision of the session update info */
    /* type = uint8_t, refer to MBS_DECISION */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_MBS_DECISION        = 0x103,
    /* The reject cause of the session update info */
    /* type = uint8_t, refer to MBS_REJECT_CAUSE */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_MBS_REJ_CAUSE       = 0x104,
    /* The security container raw of the session update info */
    /* type = byte_array */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_MBS_SEC_CONTAINER_RAW = 0x105,
    /* The cell global id struct of the session update info */
    /* type = struct, refer to mbms_cell_id_info */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_CELL_ID_TLV_ARRAY   = 0x106,
    /* The tac struct of the session update info */
    /* type = struct, refer to mbms_tac_info */
    MIPC_EMBMS_MBS_SESSION_UPDATE_IND_T_TAC_TLV_ARRAY       = 0x107,
};




#endif /* __MIPC_MSG_EMBMS_CONST_H__ */
