#ifndef __MIPC_MSG_IMS_CONST_H__
#define __MIPC_MSG_IMS_CONST_H__

enum MIPC_IMS_MSG_enum {
    MIPC_IMS_MSG_NONE = 0,
    /* set ims config */
    MIPC_IMS_SET_CONFIG_REQ                                 = 2560,
    MIPC_IMS_SET_CONFIG_CNF                                 = 2561,

    /* get modem configuration request */
    MIPC_IMS_GET_CONFIG_REQ                                 = 2562,
    MIPC_IMS_GET_CONFIG_CNF                                 = 2563,

    /* get modem ims state request, including register state, xui, uri,etc. */
    MIPC_IMS_GET_STATE_REQ                                  = 2564,
    MIPC_IMS_GET_STATE_CNF                                  = 2565,

    /* to set imspdis */
    MIPC_IMS_SET_PDIS_REQ                                   = 2566,
    MIPC_IMS_SET_PDIS_CNF                                   = 2567,

    /* to set NAPTR */
    MIPC_IMS_SET_NAPTR_REQ                                  = 2568,
    MIPC_IMS_SET_NAPTR_CNF                                  = 2569,

    /* to get network reporting status, e.g. AT+CIREP? */
    MIPC_IMS_GET_NW_RPT_REQ                                 = 2570,
    MIPC_IMS_GET_NW_RPT_CNF                                 = 2571,

    /* to set ims test mode */
    MIPC_IMS_SET_TEST_MODE_REQ                              = 2572,
    MIPC_IMS_SET_TEST_MODE_CNF                              = 2573,

    /* set ims registiation status request */
    MIPC_IMS_SET_EIREG_REQ                                  = 2574,
    MIPC_IMS_SET_EIREG_CNF                                  = 2575,

    /* to set Smart Congestion Mitigation, only for specific modem branch */
    MIPC_IMS_SET_SCM_REQ                                    = 2576,
    MIPC_IMS_SET_SCM_CNF                                    = 2577,

    /* to set IMS service session status,only for specific modem branch */
    MIPC_IMS_SET_SERVICE_SESSION_REQ                        = 2578,
    MIPC_IMS_SET_SERVICE_SESSION_CNF                        = 2579,

    /* to set IMS UAC status,only for specific modem branch */
    MIPC_IMS_SET_UAC_REQ                                    = 2580,
    MIPC_IMS_SET_UAC_CNF                                    = 2581,

    /* to set IMS VODATA REQ */
    MIPC_IMS_SET_EVODATA_REQ                                = 2582,
    MIPC_IMS_SET_EVODATA_CNF                                = 2583,

    /* This command is used to enable ims data channel. 1: enable, 0: disable. 3GPP TS26.114 data channel interface */
    MIPC_IMS_ENABLE_DATA_CHANNEL_REQ                        = 2584,
    MIPC_IMS_ENABLE_DATA_CHANNEL_CNF                        = 2585,

    /* Get modem IMS PDP status. */
    MIPC_IMS_GET_PDN_REQ                                    = 2586,
    MIPC_IMS_GET_PDN_CNF                                    = 2587,

    /* config IMS additional services capabilities, such as call composer from RCS */
    MIPC_IMS_CFG_ADDITION_SERVICE_REQ                       = 2588,
    MIPC_IMS_CFG_ADDITION_SERVICE_CNF                       = 2589,

    /* IMS configuration changed indication */
    MIPC_IMS_CONFIG_IND                                     = 18944,

    /* IMS state changed indication */
    MIPC_IMS_STATE_IND                                      = 18945,

    /* to notify AP the emergency service support status in the current cell */
    MIPC_IMS_SUPPORT_ECC_IND                                = 18946,

    /* IMS/emergency PDN status indication */
    MIPC_IMS_PDN_IND                                        = 18947,

    /* to notify AP the NAPTR information */
    MIPC_IMS_NAPTR_IND                                      = 18948,

    /* IMS eireg info indication */
    MIPC_IMS_REG_IND                                        = 18949,

    /* IMS sip reg info indication */
    MIPC_IMS_SIP_REG_INFO_IND                               = 18950,

    /* IMS cirepi indication */
    MIPC_IMS_VOPS_IND                                       = 18951,

    /* IMS register remain time indication */
    MIPC_IMS_REG_REMAIN_TIME_IND                            = 18952,

    /* IMS eimsui indication */
    MIPC_IMS_UI_IND                                         = 18953,

    /* to notify AP the p-cscf address discovery */
    MIPC_IMS_PDIS_CMD                                       = 35328,
    MIPC_IMS_PDIS_RSP                                       = 35329,


};

    /* MIPC_MSG.IMS_SET_CONFIG_REQ */
enum mipc_ims_set_config_req_tlv_enum {
    mipc_ims_set_config_req_tlv_NONE = 0,
    /* the configuration class, CLASS_IMS_FEATURE mapping AT+EIMSCFG, CLASS_IMS_CONFIG mapping AT+ECFGSET, IMS_PROVISION mapping AT+EPVSSET, IMS_CAPABILITY is used to set ims capability */
    /* type = uint32_t, refer to IMS_CONFIG_CLASS */
    MIPC_IMS_SET_CONFIG_REQ_T_CLASS                         = 0x100,
    /* the string of configuration type, for example, this string will be send to lowlayer with AT+ECFGSET = type,data */
    /* type = string */
    MIPC_IMS_SET_CONFIG_REQ_T_TYPE                          = 0x101,
    /* the configuration data,for example, this string will be send to lowlayer with AT+ECFGSET = type,data */
    /* type = byte_array */
    MIPC_IMS_SET_CONFIG_REQ_T_DATA                          = 0x102,
};

    /* MIPC_MSG.IMS_SET_CONFIG_CNF */
enum mipc_ims_set_config_cnf_tlv_enum {
    mipc_ims_set_config_cnf_tlv_NONE = 0,
    /* the response data */
    /* type = byte_array */
    MIPC_IMS_SET_CONFIG_CNF_T_DATA                          = 0x100,
};

    /* MIPC_MSG.IMS_GET_CONFIG_REQ */
enum mipc_ims_get_config_req_tlv_enum {
    mipc_ims_get_config_req_tlv_NONE = 0,
    /* the configuration class */
    /* type = uint32_t, refer to IMS_CONFIG_CLASS */
    MIPC_IMS_GET_CONFIG_REQ_T_CLASS                         = 0x100,
    /* the string of configuration type */
    /* type = string */
    MIPC_IMS_GET_CONFIG_REQ_T_TYPE                          = 0x101,
};

    /* MIPC_MSG.IMS_GET_CONFIG_CNF */
enum mipc_ims_get_config_cnf_tlv_enum {
    mipc_ims_get_config_cnf_tlv_NONE = 0,
    /* the response data */
    /* type = byte_array */
    MIPC_IMS_GET_CONFIG_CNF_T_DATA                          = 0x100,
};

    /* MIPC_MSG.IMS_GET_STATE_REQ */
enum mipc_ims_get_state_req_tlv_enum {
    mipc_ims_get_state_req_tlv_NONE = 0,
    /* the event of ims state */
    /* type = uint8_t, refer to IMS_STATE_IND_EVENT */
    MIPC_IMS_GET_STATE_REQ_T_EVENT                          = 0x100,
};

    /* MIPC_MSG.IMS_GET_STATE_CNF */
enum mipc_ims_get_state_cnf_tlv_enum {
    mipc_ims_get_state_cnf_tlv_NONE = 0,
    /* the event of ims state */
    /* type = uint8_t, refer to IMS_STATE_IND_EVENT */
    MIPC_IMS_GET_STATE_CNF_T_EVENT                          = 0x100,
    /* the ims registration state */
    /* type = uint8_t, refer to IMS_STATE */
    MIPC_IMS_GET_STATE_CNF_T_REG_STATE                      = 0x101,
    /* the ims registration ext info */
    /* type = uint32_t */
    MIPC_IMS_GET_STATE_CNF_T_EXT_INFO                       = 0x102,
    /* the ims wfc state */
    /* type = uint8_t */
    MIPC_IMS_GET_STATE_CNF_T_WFC                            = 0x103,
    /* the ims account id */
    /* type = uint32_t */
    MIPC_IMS_GET_STATE_CNF_T_ACCOUNT_ID                     = 0x104,
    /* the ims account id */
    /* type = byte_array */
    MIPC_IMS_GET_STATE_CNF_T_URI                            = 0x8105,
    /* expire time */
    /* type = uint32_t */
    MIPC_IMS_GET_STATE_CNF_T_EXPIRE_TIME                    = 0x106,
    /* error code */
    /* type = uint32_t, refer to IMS_REG_ERR_CODE */
    MIPC_IMS_GET_STATE_CNF_T_ERROR_CODE                     = 0x107,
    /* error message */
    /* type = string */
    MIPC_IMS_GET_STATE_CNF_T_ERROR_MESSAGE                  = 0x108,
    /* The ims registration type: 0 normal, 1 emergency */
    /* type = uint8_t, refer to IMS_REG_TYPE */
    MIPC_IMS_GET_STATE_CNF_T_REG_TYPE                       = 0x109,
};

    /* MIPC_MSG.IMS_SET_PDIS_REQ */
enum mipc_ims_set_pdis_req_tlv_enum {
    mipc_ims_set_pdis_req_tlv_NONE = 0,
    /* transaction id, 1~255 */
    /* type = uint8_t */
    MIPC_IMS_SET_PDIS_REQ_T_TRANSACTION_ID                  = 0x100,
    /* pcscf method */
    /* type = uint8_t, refer to PDIS_METHOD_TYPE */
    MIPC_IMS_SET_PDIS_REQ_T_METHOD                          = 0x101,
    /* to indicate pdis success or not */
    /* type = uint8_t */
    MIPC_IMS_SET_PDIS_REQ_T_IS_SUCCESS                      = 0x102,
};

    /* MIPC_MSG.IMS_SET_PDIS_CNF */
enum mipc_ims_set_pdis_cnf_tlv_enum {
    mipc_ims_set_pdis_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_SET_NAPTR_REQ */
enum mipc_ims_set_naptr_req_tlv_enum {
    mipc_ims_set_naptr_req_tlv_NONE = 0,
    /* transaction id, 1~255 */
    /* type = uint8_t */
    MIPC_IMS_SET_NAPTR_REQ_T_TRANSACTION_ID                 = 0x100,
    /* module id */
    /* type = string */
    MIPC_IMS_SET_NAPTR_REQ_T_MOD_ID                         = 0x101,
    /* to indicate pdis success or not */
    /* type = uint8_t */
    MIPC_IMS_SET_NAPTR_REQ_T_RESULT                         = 0x102,
    /* order */
    /* type = uint8_t */
    MIPC_IMS_SET_NAPTR_REQ_T_ORDER                          = 0x103,
    /* pref */
    /* type = uint8_t */
    MIPC_IMS_SET_NAPTR_REQ_T_PREF                           = 0x104,
    /* flags */
    /* type = string */
    MIPC_IMS_SET_NAPTR_REQ_T_FLAGS                          = 0x105,
    /* service */
    /* type = string */
    MIPC_IMS_SET_NAPTR_REQ_T_SERVICE                        = 0x106,
    /* pref */
    /* type = string */
    MIPC_IMS_SET_NAPTR_REQ_T_REGEXP                         = 0x107,
    /* FQDN */
    /* type = string */
    MIPC_IMS_SET_NAPTR_REQ_T_FQDN                           = 0x108,
};

    /* MIPC_MSG.IMS_SET_NAPTR_CNF */
enum mipc_ims_set_naptr_cnf_tlv_enum {
    mipc_ims_set_naptr_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_GET_NW_RPT_REQ */
enum mipc_ims_get_nw_rpt_req_tlv_enum {
    mipc_ims_get_nw_rpt_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_GET_NW_RPT_CNF */
enum mipc_ims_get_nw_rpt_cnf_tlv_enum {
    mipc_ims_get_nw_rpt_cnf_tlv_NONE = 0,
    /* the reporting status */
    /* type = uint8_t, refer to IMS_REPORT_STATUS */
    MIPC_IMS_GET_NW_RPT_CNF_T_REPORT_STATUS                 = 0x100,
    /* IMS VOPS supported indication from the network */
    /* type = uint8_t, refer to NW_IMS_VOPS_STATUS */
    MIPC_IMS_GET_NW_RPT_CNF_T_NW_IMS_VOPS                   = 0x101,
};

    /* MIPC_MSG.IMS_SET_TEST_MODE_REQ */
enum mipc_ims_set_test_mode_req_tlv_enum {
    mipc_ims_set_test_mode_req_tlv_NONE = 0,
    /* ims test mode */
    /* type = uint8_t */
    MIPC_IMS_SET_TEST_MODE_REQ_T_TEST_MODE                  = 0x100,
};

    /* MIPC_MSG.IMS_SET_TEST_MODE_CNF */
enum mipc_ims_set_test_mode_cnf_tlv_enum {
    mipc_ims_set_test_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_SET_EIREG_REQ */
enum mipc_ims_set_eireg_req_tlv_enum {
    mipc_ims_set_eireg_req_tlv_NONE = 0,
    /* the ims registration state */
    /* type = uint8_t, refer to IMS_REG_STATE_TYPE */
    MIPC_IMS_SET_EIREG_REQ_T_REG_STATE                      = 0x100,
    /* the ims registration type */
    /* type = uint8_t, refer to IMS_REG_TYPE */
    MIPC_IMS_SET_EIREG_REQ_T_REG_TYPE                       = 0x101,
    /* the ims registration ext info */
    /* type = uint32_t */
    MIPC_IMS_SET_EIREG_REQ_T_EXT_INFO                       = 0x102,
    /* the ims dereg cause */
    /* type = uint8_t, refer to IMS_DEREG_CAUSE_TYPE */
    MIPC_IMS_SET_EIREG_REQ_T_DEREG_CAUSE                    = 0x103,
    /* the ims rat */
    /* type = uint8_t, refer to IMS_RAT_TYPE */
    MIPC_IMS_SET_EIREG_REQ_T_IMS_RAT                        = 0x104,
    /* the ims sip uri type */
    /* type = uint8_t, refer to IMS_URI_TYPE */
    MIPC_IMS_SET_EIREG_REQ_T_SIP_URI_TYPE                   = 0x105,
    /* the ims detail state */
    /* type = uint8_t */
    MIPC_IMS_SET_EIREG_REQ_T_DETAIL_IMS_STATE               = 0x106,
    /* the ims reason */
    /* type = uint8_t */
    MIPC_IMS_SET_EIREG_REQ_T_REASON                         = 0x107,
    /* the ims retry */
    /* type = uint8_t, refer to IMS_RETRY_TYPE */
    MIPC_IMS_SET_EIREG_REQ_T_IMS_RETRY                      = 0x108,
};

    /* MIPC_MSG.IMS_SET_EIREG_CNF */
enum mipc_ims_set_eireg_cnf_tlv_enum {
    mipc_ims_set_eireg_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_SET_EVODATA_REQ */
enum mipc_ims_set_evodata_req_tlv_enum {
    mipc_ims_set_evodata_req_tlv_NONE = 0,
    /* IMS VoData status: on/off */
    /* type = uint8_t, refer to IMS_EVODATA_EN_ENUM */
    MIPC_IMS_SET_EVODATA_REQ_T_EN                           = 0x100,
    /* IMS VoData data mode: no limit or QOS mode */
    /* type = uint8_t, refer to IMS_EVODATA_MODE_ENUM */
    MIPC_IMS_SET_EVODATA_REQ_T_DATA_MODE                    = 0x101,
    /* IMS VoData allow rat: default or except */
    /* type = uint8_t, refer to IMS_EVODATA_ALLOWRAT_ENUM */
    MIPC_IMS_SET_EVODATA_REQ_T_ALLOW_RAT                    = 0x102,
    /* Trigger MDINTERNET to updata count (x) MB */
    /* type = uint32_t */
    MIPC_IMS_SET_EVODATA_REQ_T_REPORT_METRIC                = 0x103,
};

    /* MIPC_MSG.IMS_SET_EVODATA_CNF */
enum mipc_ims_set_evodata_cnf_tlv_enum {
    mipc_ims_set_evodata_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_ENABLE_DATA_CHANNEL_REQ */
enum mipc_ims_enable_data_channel_req_tlv_enum {
    mipc_ims_enable_data_channel_req_tlv_NONE = 0,
    /* ims data channel on/off. 1: enable/on, 0: disable/off */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_IMS_ENABLE_DATA_CHANNEL_REQ_T_ENABLE               = 0x100,
};

    /* MIPC_MSG.IMS_ENABLE_DATA_CHANNEL_CNF */
enum mipc_ims_enable_data_channel_cnf_tlv_enum {
    mipc_ims_enable_data_channel_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_GET_PDN_REQ */
enum mipc_ims_get_pdn_req_tlv_enum {
    mipc_ims_get_pdn_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_GET_PDN_CNF */
enum mipc_ims_get_pdn_cnf_tlv_enum {
    mipc_ims_get_pdn_cnf_tlv_NONE = 0,
    /* The type of indication, currently it is always notify */
    /* type = uint8_t, refer to IMS_IND_TYPE */
    MIPC_IMS_GET_PDN_CNF_T_IND_TYPE                         = 0x100,
    /* IMS/emergency PDN CID */
    /* type = uint32_t */
    MIPC_IMS_GET_PDN_CNF_T_ID                               = 0x101,
    /* IMS/emergency PDN state, activated or deactivated */
    /* type = uint8_t, refer to MIPC_CID_ACT_STATE */
    MIPC_IMS_GET_PDN_CNF_T_PDN_STATE                        = 0x102,
    /* APN type, it should be ims or emergency */
    /* type = uint32_t, refer to APN_TYPE */
    MIPC_IMS_GET_PDN_CNF_T_APN_TYPE                         = 0x103,
    /* network interface id */
    /* type = uint32_t */
    MIPC_IMS_GET_PDN_CNF_T_INTERFACE_ID                     = 0x104,
    /* ipv4 dns address count */
    /* type = uint8_t */
    MIPC_IMS_GET_PDN_CNF_T_V4_DNS_ADDR_COUNT                = 0x105,
    /* ipv4 dns address struct list */
    /* type = struct_array, refer to data_v4_addr */
    MIPC_IMS_GET_PDN_CNF_T_V4_DNS_ADDR_LIST                 = 0x8106,
    /* ipv6 dns address count */
    /* type = uint8_t */
    MIPC_IMS_GET_PDN_CNF_T_V6_DNS_ADDR_COUNT                = 0x107,
    /* ipv6 dns address struct list */
    /* type = struct_array, refer to data_v6_addr */
    MIPC_IMS_GET_PDN_CNF_T_V6_DNS_ADDR_LIST                 = 0x8108,
    /* error cause for IMS_PDN_REJECT */
    /* type = uint32_t, refer to IMS_PDN_ERR_CAUSE */
    MIPC_IMS_GET_PDN_CNF_T_ERROR_CAUSE                      = 0x109,
};

    /* MIPC_MSG.IMS_CFG_ADDITION_SERVICE_REQ */
enum mipc_ims_cfg_addition_service_req_tlv_enum {
    mipc_ims_cfg_addition_service_req_tlv_NONE = 0,
    /*
      An int array of Additional services capabilities, element number is 16.
      Index 0 is for call composer(value 0: disable, 1: enable).
      Other indexes are used for future new services.
      Service name refer to IMS_ADDITION_SERVICE.
    */
    /* type = byte_array, refer to IMS_ADDITION_SERVICE */
    MIPC_IMS_CFG_ADDITION_SERVICE_REQ_T_SERVICE_CAP         = 0x100,
};

    /* MIPC_MSG.IMS_CFG_ADDITION_SERVICE_CNF */
enum mipc_ims_cfg_addition_service_cnf_tlv_enum {
    mipc_ims_cfg_addition_service_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.IMS_CONFIG_IND */
enum mipc_ims_config_ind_tlv_enum {
    mipc_ims_config_ind_tlv_NONE = 0,
    /* the reason of ims configuration changed indication */
    /* type = uint8_t, refer to IMS_CONFIG_IND_REASON */
    MIPC_IMS_CONFIG_IND_T_REASON                            = 0x100,
    /* the reason of ims configuration changed indication */
    /* type = byte_array */
    MIPC_IMS_CONFIG_IND_T_CONFIG_DATA                       = 0x101,
};

    /* MIPC_MSG.IMS_STATE_IND */
enum mipc_ims_state_ind_tlv_enum {
    mipc_ims_state_ind_tlv_NONE = 0,
    /* the event of ims state changed indication */
    /* type = uint8_t, refer to IMS_STATE_IND_EVENT */
    MIPC_IMS_STATE_IND_T_EVENT                              = 0x100,
    /* the ims registration state */
    /* type = uint8_t, refer to IMS_STATE */
    MIPC_IMS_STATE_IND_T_REG_STATE                          = 0x101,
    /* the ims registration ext info */
    /* type = uint32_t */
    MIPC_IMS_STATE_IND_T_EXT_INFO                           = 0x102,
    /* the ims wfc state */
    /* type = uint8_t */
    MIPC_IMS_STATE_IND_T_WFC                                = 0x103,
    /* the ims account id */
    /* type = uint32_t */
    MIPC_IMS_STATE_IND_T_ACCOUNT_ID                         = 0x104,
    /* the ims account id */
    /* type = byte_array */
    MIPC_IMS_STATE_IND_T_URI                                = 0x8105,
    /* expire time */
    /* type = uint32_t */
    MIPC_IMS_STATE_IND_T_EXPIRE_TIME                        = 0x106,
    /* error code */
    /* type = uint32_t, refer to IMS_REG_ERR_CODE */
    MIPC_IMS_STATE_IND_T_ERROR_CODE                         = 0x107,
    /* error message */
    /* type = string */
    MIPC_IMS_STATE_IND_T_ERROR_MESSAGE                      = 0x108,
};

    /* MIPC_MSG.IMS_SUPPORT_ECC_IND */
enum mipc_ims_support_ecc_ind_tlv_enum {
    mipc_ims_support_ecc_ind_tlv_NONE = 0,
    /* current cell rat */
    /* type = uint8_t, refer to IMS_ECC_IND_RAT */
    MIPC_IMS_SUPPORT_ECC_IND_T_RAT                          = 0x100,
    /* emergency service support status. 0: doesn't support, 1: support */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_IMS_SUPPORT_ECC_IND_T_SUPPORT_EMC                  = 0x101,
};

    /* MIPC_MSG.IMS_PDN_IND */
enum mipc_ims_pdn_ind_tlv_enum {
    mipc_ims_pdn_ind_tlv_NONE = 0,
    /* the type of indication, currently it is always notify */
    /* type = uint8_t, refer to IMS_IND_TYPE */
    MIPC_IMS_PDN_IND_T_IND_TYPE                             = 0x100,
    /* ims/emergency PDN id */
    /* type = uint32_t */
    MIPC_IMS_PDN_IND_T_ID                                   = 0x101,
    /* ims/emergency PDN state, activated or deactivated */
    /* type = uint8_t, refer to MIPC_CID_ACT_STATE */
    MIPC_IMS_PDN_IND_T_PDN_STATE                            = 0x102,
    /* APN type, it should be ims or emergency */
    /* type = uint32_t, refer to APN_TYPE */
    MIPC_IMS_PDN_IND_T_APN_TYPE                             = 0x103,
    /* network interface id */
    /* type = uint32_t */
    MIPC_IMS_PDN_IND_T_INTERFACE_ID                         = 0x104,
    /* ipv4 dns address count */
    /* type = uint8_t */
    MIPC_IMS_PDN_IND_T_V4_DNS_ADDR_COUNT                    = 0x105,
    /* ipv4 dns address struct list */
    /* type = struct_array, refer to data_v4_addr */
    MIPC_IMS_PDN_IND_T_V4_DNS_ADDR_LIST                     = 0x8106,
    MIPC_IMS_PDN_IND_T_V4_DNS_ADDR_TLV_ARRAY                = 0x8106,
    /* ipv6 dns address count */
    /* type = uint8_t */
    MIPC_IMS_PDN_IND_T_V6_DNS_ADDR_COUNT                    = 0x107,
    /* ipv6 dns address struct list */
    /* type = struct_array, refer to data_v6_addr */
    MIPC_IMS_PDN_IND_T_V6_DNS_ADDR_LIST                     = 0x8108,
    MIPC_IMS_PDN_IND_T_V6_DNS_ADDR_TLV_ARRAY                = 0x8108,
    /* error cause for IMS_PDN_REJECTt */
    /* type = uint32_t, refer to IMS_PDN_ERR_CAUSE */
    MIPC_IMS_PDN_IND_T_ERROR_CAUSE                          = 0x109,
};

    /* MIPC_MSG.IMS_NAPTR_IND */
enum mipc_ims_naptr_ind_tlv_enum {
    mipc_ims_naptr_ind_tlv_NONE = 0,
    /* transaction id, 1~255 */
    /* type = uint8_t */
    MIPC_IMS_NAPTR_IND_T_TRANSACTION_ID                     = 0x100,
    /* module id */
    /* type = string */
    MIPC_IMS_NAPTR_IND_T_MOD_ID                             = 0x101,
    /* FQDN */
    /* type = string */
    MIPC_IMS_NAPTR_IND_T_FQDN                               = 0x102,
};

    /* MIPC_MSG.IMS_REG_IND */
enum mipc_ims_reg_ind_tlv_enum {
    mipc_ims_reg_ind_tlv_NONE = 0,
    /* the ims registration state */
    /* type = uint8_t, refer to IMS_STATE */
    MIPC_IMS_REG_IND_T_REG_STATE                            = 0x100,
    /* the ims registration type */
    /* type = uint8_t, refer to IMS_REG_TYPE */
    MIPC_IMS_REG_IND_T_REG_TYPE                             = 0x101,
    /* the ims registration ext info */
    /* type = uint32_t */
    MIPC_IMS_REG_IND_T_EXT_INFO                             = 0x102,
    /* the ims dereg cause */
    /* type = uint8_t */
    MIPC_IMS_REG_IND_T_DEREG_CAUSE                          = 0x103,
    /* the ims retry */
    /* type = uint8_t */
    MIPC_IMS_REG_IND_T_IMS_RETRY                            = 0x104,
    /* the ims rat */
    /* type = uint8_t */
    MIPC_IMS_REG_IND_T_RAT                                  = 0x105,
    /* the ims sip uri type */
    /* type = uint8_t */
    MIPC_IMS_REG_IND_T_SIP_URI_TYPE                         = 0x106,
    /* the ims reg sub state. REFER: IMS_SUB_STATE */
    /* type = uint8_t, refer to IMS_STATE */
    MIPC_IMS_REG_IND_T_REG_SUB_STATE                        = 0x107,
};

    /* MIPC_MSG.IMS_SIP_REG_INFO_IND */
enum mipc_ims_sip_reg_info_ind_tlv_enum {
    mipc_ims_sip_reg_info_ind_tlv_NONE = 0,
    /* the ims esipreginfo account id */
    /* type = uint32_t */
    MIPC_IMS_SIP_REG_INFO_IND_T_ACCOUNT_ID                  = 0x100,
    /* the ims esipreginfo direction */
    /* type = string */
    MIPC_IMS_SIP_REG_INFO_IND_T_DIRECTION                   = 0x101,
    /* the ims esipreginfo sip msg type */
    /* type = string */
    MIPC_IMS_SIP_REG_INFO_IND_T_SIP_MSG_TYPE                = 0x102,
    /* the ims esipreginfo method */
    /* type = string */
    MIPC_IMS_SIP_REG_INFO_IND_T_METHOD                      = 0x103,
    /* the ims esipreginfo response code */
    /* type = uint32_t */
    MIPC_IMS_SIP_REG_INFO_IND_T_RESPONSE_CODE               = 0x104,
    /* the ims esipreginfo reason phrase */
    /* type = string */
    MIPC_IMS_SIP_REG_INFO_IND_T_REASON_PHRASE               = 0x105,
    /* the ims esipreginfo warn text */
    /* type = string */
    MIPC_IMS_SIP_REG_INFO_IND_T_WARN_TEXT                   = 0x106,
    /* the ims esipreginfo cseq */
    /* type = uint32_t */
    MIPC_IMS_SIP_REG_INFO_IND_T_CSEQ                        = 0x107,
    /* the ims esipreginfo reason cause */
    /* type = uint32_t */
    MIPC_IMS_SIP_REG_INFO_IND_T_REASON_CAUSE                = 0x108,
    /* the ims esipreginfo reason text */
    /* type = string */
    MIPC_IMS_SIP_REG_INFO_IND_T_REASON_TEXT                 = 0x109,
    /* the ims esipreginfo call id */
    /* type = string */
    MIPC_IMS_SIP_REG_INFO_IND_T_CALL_ID                     = 0x10A,
};

    /* MIPC_MSG.IMS_VOPS_IND */
enum mipc_ims_vops_ind_tlv_enum {
    mipc_ims_vops_ind_tlv_NONE = 0,
    /* the ims vops supported indication from the network */
    /* type = uint8_t */
    MIPC_IMS_VOPS_IND_T_NWIMSVOPS                           = 0x100,
    /* the ims vops supported indication n3gpp value */
    /* type = uint8_t, refer to IMS_VOPS_N3GPP_ENUM */
    MIPC_IMS_VOPS_IND_T_NWIMSVOPS_N3GPP                     = 0x101,
};

    /* MIPC_MSG.IMS_REG_REMAIN_TIME_IND */
enum mipc_ims_reg_remain_time_ind_tlv_enum {
    mipc_ims_reg_remain_time_ind_tlv_NONE = 0,
    /* the ims register remain time */
    /* type = uint32_t */
    MIPC_IMS_REG_REMAIN_TIME_IND_T_REG_REMAIN_TIME          = 0x100,
    /* the sub remain time */
    /* type = uint32_t */
    MIPC_IMS_REG_REMAIN_TIME_IND_T_SUB_REMAIN_TIME          = 0x101,
};

    /* MIPC_MSG.IMS_UI_IND */
enum mipc_ims_ui_ind_tlv_enum {
    mipc_ims_ui_ind_tlv_NONE = 0,
    /* the ims icon advance display flag */
    /* type = uint8_t */
    MIPC_IMS_UI_IND_T_ICONFLAG                              = 0x100,
};

    /* MIPC_MSG.IMS_PDIS_CMD */
enum mipc_ims_pdis_cmd_tlv_enum {
    mipc_ims_pdis_cmd_tlv_NONE = 0,
    /* transaction id, 1~255 */
    /* type = uint8_t */
    MIPC_IMS_PDIS_CMD_T_TRANSACTION_ID                      = 0x100,
    /* emergency indication */
    /* type = uint8_t, refer to EM_ID_TYPE */
    MIPC_IMS_PDIS_CMD_T_EM_ID                               = 0x101,
    /* pcscf method */
    /* type = uint8_t, refer to PDIS_METHOD_TYPE */
    MIPC_IMS_PDIS_CMD_T_METHOD                              = 0x102,
    /* interface name , such as ccmni0 */
    /* type = string */
    MIPC_IMS_PDIS_CMD_T_NW_INTERFACE_NAME                   = 0x103,
};

    /* MIPC_MSG.IMS_PDIS_RSP */
enum mipc_ims_pdis_rsp_tlv_enum {
    mipc_ims_pdis_rsp_tlv_NONE = 0,
    /* transaction id, 1~255 */
    /* type = uint8_t */
    MIPC_IMS_PDIS_RSP_T_TRANSACTION_ID                      = 0x100,
    /* pcscf method */
    /* type = uint8_t, refer to PDIS_METHOD_TYPE */
    MIPC_IMS_PDIS_RSP_T_METHOD                              = 0x101,
    /* success or not */
    /* type = uint8_t */
    MIPC_IMS_PDIS_RSP_T_IS_SUCCESS                          = 0x102,
};

    /* MIPC_MSG.IMS_SET_SCM_REQ */
enum mipc_ims_set_scm_req_tlv_enum {
    mipc_ims_set_scm_req_tlv_NONE = 0,
    /* Smart Congestion Mitigation application */
    /* type = uint8_t, refer to SCM_APPLICATION_ENUM */
    MIPC_IMS_SET_SCM_REQ_T_APPLICATION                      = 0x100,
    /* start or end */
    /* type = uint8_t, refer to SCM_INDICATION_ENUM */
    MIPC_IMS_SET_SCM_REQ_T_INDICATION                       = 0x101,
};

    /* MIPC_MSG.IMS_SET_SCM_CNF */
enum mipc_ims_set_scm_cnf_tlv_enum {
    mipc_ims_set_scm_cnf_tlv_NONE = 0,
};

    /* MIPC_MSG.IMS_SET_SERVICE_SESSION_REQ */
enum mipc_ims_set_service_session_req_tlv_enum {
    mipc_ims_set_service_session_req_tlv_NONE = 0,
    /* IMS service session status */
    /* type = uint8_t, refer to IMS_SERVICE_TYPE_ENUM */
    MIPC_IMS_SET_SERVICE_SESSION_REQ_T_SERVICE_TYPE         = 0x100,
    /* start or end */
    /* type = uint8_t, refer to IMS_SERVICE_STATUS_ENUM */
    MIPC_IMS_SET_SERVICE_SESSION_REQ_T_SERVICE_STATUS       = 0x101,
};

    /* MIPC_MSG.IMS_SET_SERVICE_SESSION_CNF */
enum mipc_ims_set_service_session_cnf_tlv_enum {
    mipc_ims_set_service_session_cnf_tlv_NONE = 0,
};

    /* MIPC_MSG.IMS_SET_UAC_REQ */
enum mipc_ims_set_uac_req_tlv_enum {
    mipc_ims_set_uac_req_tlv_NONE = 0,
    /* IMS service session status */
    /* type = uint8_t, refer to IMS_SERVICE_TYPE_ENUM */
    MIPC_IMS_SET_UAC_REQ_T_SERVICE_TYPE                     = 0x100,
    /* start or end */
    /* type = uint8_t, refer to IMS_SERVICE_STATUS_ENUM */
    MIPC_IMS_SET_UAC_REQ_T_SERVICE_STATUS                   = 0x101,
};

    /* MIPC_MSG.IMS_SET_UAC_CNF */
enum mipc_ims_set_uac_cnf_tlv_enum {
    mipc_ims_set_uac_cnf_tlv_NONE = 0,
};


#endif /* __MIPC_MSG_IMS_CONST_H__ */
